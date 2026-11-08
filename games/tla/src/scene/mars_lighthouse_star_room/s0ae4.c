#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"

extern struct Actor *Data_02004cec;

void Func_02000ab4(void);
void Func_020042ac(s32 sound);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void VramCache_Load(s32 slot, s32 size, const u8 *tiles);

void Func_02000ae4(s32 rise) {
  struct Actor **slot = &Data_02004cec;
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 zero;
  s32 size;

  zero = 0;
  object = Func_020041e4(0x1e9, 0x2be0000, 0, 0xe00000);
  *slot = object;
  if (object == NULL)
    return;
  sprite = object->animation;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 3;
  object->unk5c[0] = 1;
  object->update = Func_02000ab4;
  size = 0x608;
  sprite->nextFrame = zero;
  sprite->unk1b[0] = zero;
  block = HeapSlot_AllocFar(68, size);
  Func_020042ac(247);
  block += 0x400;
  VramCache_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(68);
  if (rise) {
    wait(1);
    (*slot)->velocityY = 0x28000;
    wait(4);
    Func_0200446c(135);
    wait(8);
    Func_0200446c(135);
  }
}
