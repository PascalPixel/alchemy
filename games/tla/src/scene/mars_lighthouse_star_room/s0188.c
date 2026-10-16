#include "mars_lighthouse_star_room.h"

void Func_02000188(void) {
  struct ObjectRuntime *object = Object_GetById(10);

#if defined(TLA_EDITION_JA)
  if (GameFlag_Test(0xa4b) == 0) {
#elif defined(TLA_EDITION_EN)
  if (GameFlag_Test(0x238) == 0 || GameFlag_Test(0xa4b) == 0) {
#else
  if (GameFlag_Test(0x238) == 0 && GameFlag_Test(0xa4b) == 0) {
#endif
    Func_0200446c(0x23d);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    if (GameFlag_Test(0xa4a) == 0) {
      object->scaleX = 0x9999;
      object->scaleY = 0x9999;
    }
  }
}

void Func_020001e8(void) {
  struct ObjectRuntime *object = Object_GetById(10);

#if defined(TLA_EDITION_JA)
  if (GameFlag_Test(0xa4b) == 0) {
#elif defined(TLA_EDITION_EN)
  if (GameFlag_Test(0x238) == 0 || GameFlag_Test(0xa4b) == 0) {
#else
  if (GameFlag_Test(0x238) == 0 && GameFlag_Test(0xa4b) == 0) {
#endif
    Func_0200446c(0x23d);
    GameFlag_SetBit(0x233);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    GameFlag_SetBit(0xa4a);
  }
}
