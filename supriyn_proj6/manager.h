#ifndef MANAGER_H
#define MANAGER_H
#include <SDL.h>
#include <vector>
#include <list>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "scaledSprite.h"
#include "hud.h"
#include "health.h"
#include "explodingSprite.h"
#include "smartSprite.h"
#include "sound.h"


class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void setHUD(bool showHUD) {
    hudFlag = showHUD;  
  }
  //void makeOrbs();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  //SDL_Surface * const orbSurface1;
  World kungfuback;
  World kungfu;
  Viewport& viewport;
  bool hudFlag;
  Hud hud;
  Health healthLine;
  std::vector<Drawable*> sprites;
  //std::vector<Drawable*> orbs1;
  std::list<ExplodingSprite*> explodingSprite;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  unsigned int numberOfkungfubullets;
  int score;
  SDLSound sound;
  bool healthLeft;
  bool godmode;
  void draw() const;
  void update();
  void makeScaledBullets();
  void drawHUD() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
#endif
