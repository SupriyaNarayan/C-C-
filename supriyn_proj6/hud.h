#ifndef HUD__H
#define HUD__H
#include <string>
#include <SDL.h>
#include "ioManager.h"
#include "vector2f.h"

class Hud {
public:
  Hud();
  void drawHUD(SDL_Surface * const&, const IOManager&, int seconds, int fps , unsigned int b , unsigned int f) const;

private:
  const int width;
  const int height;
  int x;
  int y; 
};
#endif
