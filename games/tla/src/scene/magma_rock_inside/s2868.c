#include "local.h"


void Func_02002868(void) {
  if (!Event_GetFlag(0x305)) {
    Actor_SetSpriteColor(Actor_Get(11), 15);
    Actor_SetPosition(11, PIXELS(136), PIXELS(360));
    Actor_Get(11)->scaleX = Actor_Get(11)->scaleY = 0x1000;
  }
}
