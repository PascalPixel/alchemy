#include "local.h"


void Func_0200217c(void) {
  if (!Event_GetFlag(0x303)) {
    Actor_SetSpriteColor(Actor_Get(21), 15);
    Actor_SetPosition(21, PIXELS(600), PIXELS(312));
    Actor_Get(21)->scaleX = Actor_Get(21)->scaleY = 0x1000;
  }
}
