#include "../../../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "version.h"

void Func_02000188(void) {
  struct Actor *object = Actor_GetById(10);

#if !TLA_PATCH_1
  if (GameFlag_Test(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (GameFlag_Test(0x238) == 0 || GameFlag_Test(SPLIT_CONSTANT(0xa00, 75)) == 0) {
#else
  if (GameFlag_Test(0x238) == 0 && GameFlag_Test(0xa4b) == 0) {
#endif
    Func_0200446c(SPLIT_CONSTANT(0x13e, 255));
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    if (GameFlag_Test(0xa4a) == 0) {
      object->scaleX = SPLIT_CONSTANT(0x9900, 153);
      object->scaleY = 0x9999;
    }
  }
}

void Func_020001e8(void) {
  struct Actor *object = Actor_GetById(10);

#if !TLA_PATCH_1
  if (GameFlag_Test(0xa4b) == 0) {
#elif !TLA_PATCH_2
  if (GameFlag_Test(0x238) == 0 || GameFlag_Test(SPLIT_CONSTANT(0xa00, 75)) == 0) {
#else
  if (GameFlag_Test(0x238) == 0 && GameFlag_Test(0xa4b) == 0) {
#endif
    Func_0200446c(SPLIT_CONSTANT(0x13e, 255));
    GameFlag_SetBit(SPLIT_CONSTANT(0x134, 255));
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 0x66) << 16;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    GameFlag_SetBit(0xa4a);
  }
}
