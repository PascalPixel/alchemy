#include "local.h"


void Func_02002128(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;


  *(work + 0xd38) = 1;
  if (!Event_GetFlag(0x304)) {
    Actor_SetSpriteColor(Actor_Get(22), 15);
    Actor_SetPosition(22, PIXELS(632), PIXELS(312));
    Actor_Get(22)->scaleX = Actor_Get(22)->scaleY = 0x1000;
  }
}
