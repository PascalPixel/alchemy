// Draft, differ 31 of 80 (was 57), rewritten from the listing: creates object
// kind 0x1e9 (Data_02004cec) at (1408, 224) with full-colour off, palette 0 and
// priority 1, loads its tiles into VRAM through a heap block, and with rise set
// lifts it with sound 135 twice. Left: register choice: the block (and the 0 it
// starts as, which CSE uses for the two byte stores) is in r6 and the sprite in
// r5, the other way round from the native; and the native loads Data_02004cec's
// address before the arguments. Tried: the block declared first, its 0 set
// after the create call (no change).
#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"

extern struct Actor *Data_02004cec;

void Func_02000ab4(void);
void Func_020042ac(s32 sound);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void VramCache_Load(s32 slot, s32 size, const u8 *tiles);

void Func_02000ae4(s32 rise) {
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;

  object = Data_02004cec = Func_020041e4(0x1e9, 0x2be0000, 0, 0xe00000);
  block = NULL;
  if (object == NULL)
    return;
  sprite = object->animation;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 3;
  object->unk5c[0] = 1;
  object->update = Func_02000ab4;
  sprite->nextFrame = 0;
  sprite->unk1b[0] = 0;
  block = HeapSlot_AllocFar(68, 0x608);
  Func_020042ac(247);
  block += 0x400;
  VramCache_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(68);
  if (rise) {
    WaitFrames(1);
    Data_02004cec->velocityY = 0x28000;
    WaitFrames(4);
    Func_0200446c(135);
    WaitFrames(8);
    Func_0200446c(135);
  }
}
