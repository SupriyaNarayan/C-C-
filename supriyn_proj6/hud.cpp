#include "hud.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "aaline.h"

Hud::Hud() :
  width(Gamedata::getInstance().getXmlInt("hud/width")),
  height(Gamedata::getInstance().getXmlInt("hud/height")),
  x(Gamedata::getInstance().getXmlInt("hud/src_x")),
  y(Gamedata::getInstance().getXmlInt("hud/src_y"))  
{}
void Hud::drawHUD(SDL_Surface * const& screen, const IOManager& io,int seconds,int fps , unsigned int bulletCount , unsigned freeCount) const{
 
  const Uint32 green = SDL_MapRGB(screen->format, 0, 255, 0);
  Draw_AALine(screen, x, y+height/2, 
                      x+width,y+height/2, 
                      height, 0, 0, 0, 0xff/2);
  Draw_AALine(screen, x,y, x+width,y, green);
  Draw_AALine(screen, x,y,x,y+height, green);
  Draw_AALine(screen,x+width,y,x+width,y+height,green);
  Draw_AALine(screen,x,y+height,x+width,y+height,green);
  io.printMessageValueAt("Seconds: ", seconds, 25, 25);
  io.printMessageValueAt("fps: ", fps, 25,45);
  io.printMessageAt("Press a to move left", 25,65);
  io.printMessageAt("Press d to move right", 25, 85);
  io.printMessageAt("Press w to move up", 25,105);
  io.printMessageAt("Press s to move down", 25,125);
  io.printMessageAt("Press h to destroy blade", 25,145);
  io.printMessageAt("Press g to change to God mode", 25,165);
  io.printMessageValueAt("Bullet List Count: ", bulletCount,25,185);
  io.printMessageValueAt("Free List Count: ", freeCount, 25,205);

}
