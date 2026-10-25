#include "types.h"
#include "animspr.h"
#include "ram_buffer.h"

struct AnimationEntry *AnimationObject_AllocateEmpty(void);
s32 Animation_ApplyChildArgumentFar(void *, s32);

struct EffectSprite {
  u8 unk00[0x0c];
  s32 unk0c;
  u8 unk10;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 kind;
  u8 unk15;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  u8 unk1c[0x0c];
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

// Takes the first free effect sprite of the 64 with a new animation entry.
struct EffectSprite *Func_0815b3b0(s32 kind, s32 value) {
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  struct EffectSprite *found = NULL;
  s32 i = 0;

  while (i <= 63) {
    if (sprite->kind == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->kind = kind;
  sprite->unk15 = value;
  sprite->unk0c = 0x10000;
  sprite->unk11_0 = 0;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  sprite->unk12 = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);
  return found;
}
