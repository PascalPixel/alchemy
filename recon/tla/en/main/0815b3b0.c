// Near miss of Func_0815b3b0 (listing 46 instructions, draft 46, differ 2,
// one reordered row): the native reads the flag byte at +0x11 (ldrb r2)
// between the two halves of 0x10000 (movs r3, #128 / lsls r3, r3, #9); here
// the constant is finished first. In sched2 the shift outranks the load by
// one, which a load latency of 3 would reverse. Tried: the flag clear before
// the 0x10000 store (differ 10), the constant through
// asm("" : "+l"(one)) volatile or not (differ 8 and 2), one-pass blocks
// (differ 17), an empty asm (differ 8), the flag byte read into a local held
// by an asm (differ 11, the mask becomes 254), an asm on the sprite pointer
// (differ 22).
// 2026-10-18: the flags as a plain u8 cleared with &= ~1 (3), each of the ten
// stores moved to every other place (2 at best), the call's result held in a
// local and stored after the flag clear (14), alchemy permute 4 minutes (2).
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
