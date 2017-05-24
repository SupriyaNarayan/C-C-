#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

class ScaledSpriteCompare {
public:
  bool operator() (Drawable * lhs, Drawable * rhs) {
    return (static_cast<ScaledSprite*>(lhs)->getScale() < static_cast<ScaledSprite*>(rhs)->getScale());
  }
};

Manager::~Manager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  if(explodingSprite.size() != 0 ){
    std::list<ExplodingSprite*>::iterator itr = 
    explodingSprite.begin();
    while( itr != explodingSprite.end()){
      delete *itr;
      itr = explodingSprite.erase(itr);
    }
  }
  sprites.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  //orbSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("leaf/file"), 
    //          Gamedata::getInstance().getXmlBool("leaf/transparency")) ),
  //orbSurface1(io.loadAndSet(Gamedata::getInstance().getXmlStr("leaf1/file"), 
    //          Gamedata::getInstance().getXmlBool("leaf1/transparency"))),
  kungfuback("kungfuback", Gamedata::getInstance().getXmlInt("kungfuback/factor") ),
  kungfu("kungfu", Gamedata::getInstance().getXmlInt("kungfu/factor") ),
  viewport( Viewport::getInstance() ),
  hudFlag(false),
  hud(),
  healthLine(),
  sprites(),
  explodingSprite(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  numberOfkungfubullets(Gamedata::getInstance().getXmlInt("kungfubullet/numberOfkungfubullets")),
  score(0),
  sound(),
  healthLeft(true),
  godmode(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  //makeOrbs();
  sprites.push_back(new Player("player"));
  makeScaledBullets();
  viewport.setObjectToTrack(sprites[0]);
}

void Manager::makeScaledBullets() {
  for (unsigned i = 0; i < numberOfkungfubullets; ++i){
    sprites.push_back(new ScaledSprite("kungfubullet"));
  }
  sort(sprites.begin()+1 , sprites.end() , ScaledSpriteCompare());
}
void Manager::draw() const {
  kungfuback.draw();
  for (unsigned i = 1; i < sprites.size(); ++i){
    sprites[i]->draw();
  }
  kungfu.draw();
  std::list<ExplodingSprite*>::const_iterator itr = explodingSprite.begin();
  while(itr != explodingSprite.end() ){
    (*itr)->draw();
    ++itr;
  }
  if(healthLeft) sprites[0]->draw();
  io.printMessageAt(title, 10, 460);
  if(!healthLeft) {
    clock.pause();
    io.printMessageValueAt("Your final score is ", score, 290, 200);
  }
  viewport.draw();
  io.printMessageValueAt("Your score is ", score , 10 , 440);
  drawHUD();
  SDL_Flip(screen);
}

void Manager::drawHUD() const {
  if(clock.getSeconds() <3){
    hud.drawHUD(screen, io, clock.getSeconds(), clock.getFps(), static_cast<Player*>(sprites[0])->getBulletListSize() , static_cast<Player*>(sprites[0])->getFreeListSize());
}
  else if(hudFlag==true) {
    hud.drawHUD(screen, io, clock.getSeconds(), clock.getFps(), static_cast<Player*>(sprites[0])->getBulletListSize() , static_cast<Player*>(sprites[0])->getFreeListSize());
  }
  healthLine.draw();
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}


void Manager::update() {
  clock.update();
  kungfuback.update();
  kungfu.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::list<ExplodingSprite*>::iterator itr = explodingSprite.begin();
  while( itr != explodingSprite.end()){
    (*itr)->update(ticks);
    if((*itr)->done()){
      delete *itr;
      itr = explodingSprite.erase(itr);
    }
    else ++itr;
  }

  for (unsigned int i = 1; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }



  std::vector<Drawable*>::iterator ptr = sprites.begin() + 1;
  while(ptr != sprites.end() && healthLeft){
    if(static_cast<Player*>(sprites[0])->collidWith(*ptr) ){
      explodingSprite.push_back(new ExplodingSprite(*(static_cast<ScaledSprite*>(*ptr))));
      ScaledSprite* player = static_cast<ScaledSprite*>( sprites[0] );
      ScaledSprite* sprite = static_cast<ScaledSprite*>( *ptr );      
      *ptr = new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
      delete sprite;
      ptr++;      
      ++score;                                                 
    }
    else ++ptr;
  }

  std::vector<Drawable*>::iterator pos = sprites.begin() + 1;
  while(pos != sprites.end()){
    (*pos)->update(ticks);
    if(static_cast<Player*>(sprites[0])->playerCollidWith(*pos) && healthLeft){
      explodingSprite.push_back(new ExplodingSprite(*(static_cast<ScaledSprite*>(*pos))));
      ScaledSprite* player = static_cast<ScaledSprite*>(sprites[0]);
      ScaledSprite* sprite = static_cast<ScaledSprite*>(*pos);      
      *pos = new SmartSprite(sprite->getName(), sprite->getPosition() + Vector2f(100,100), *player); 
      delete sprite;
      pos++;
      ++score;
    }
    else ++pos;
  }
  
  if (makeVideo && frameCount < frameMax) makeFrame();
  if(godmode) healthLine.reset();
  healthLine.update(ticks,godmode);
  if(healthLine.getCurrentHealth() == 0) healthLeft = false;  
  if(healthLeft) sprites[0]->update(ticks);
  viewport.update();
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool flag = false;
  int count =1;
  clock.start();

  while ( not done ) {
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      flag = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }
    if (keystate[SDLK_r]  && !flag) {
        flag = true;
        healthLeft = true;
        healthLine.reset();
        score = 0;
        clock.unpause();
        viewport.setObjectToTrack(sprites[0]);
        std::vector<Drawable*>::iterator itr = sprites.begin();
        while(itr != sprites.end()){
          delete *itr;
          itr = sprites.erase(itr);
        }
        sprites.push_back(new Player("player"));
      	for(unsigned i = 0; i < numberOfkungfubullets; i++){
    	  sprites.push_back( new ScaledSprite("kungfubullet") );
  	}
  	sort(sprites.begin()+1, sprites.end(),ScaledSpriteCompare());
  	viewport.setObjectToTrack(sprites[0]);   
      }
	if(keystate[SDLK_F1] && !flag)
	{
         count++;
         if(count%2==0)
          {
             hudFlag=true;
	  }
         else
	  {
             hudFlag=false;
	  }

     }
      if (keystate[SDLK_g]  && !flag) {
        flag =true;
        godmode = !godmode;
      }
      if ( keystate[SDLK_p] && !flag) {
        flag = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }
      if ( keystate[SDLK_f]  && !flag) {
        flag = true;
        clock.toggleCapFrame();
      }
      
      if (keystate[SDLK_h]) {
        static_cast<Player*>(sprites[0])->hit();
        sound[0];
      }
      if (keystate[SDLK_a]) {
        static_cast<Player*>(sprites[0])->goLeft();
      }
      if (keystate[SDLK_d]) {
        static_cast<Player*>(sprites[0])->goRight();
      }
      if (keystate[SDLK_w]) {
        static_cast<Player*>(sprites[0])->goUp();
      }
      if (keystate[SDLK_s]) {
        static_cast<Player*>(sprites[0])->goDown();
      }
      if (keystate[SDLK_a] && keystate[SDLK_d]) {
        static_cast<Player*>(sprites[0])->dontmove();
      }  
      if (keystate[SDLK_w] && keystate[SDLK_s]) {
        static_cast<Player*>(sprites[0])->dontmove();
      }
      if (keystate[SDLK_F4] && !makeVideo  && !flag) {
        flag = true;
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
    }    
    draw();
    update();
  }
}
