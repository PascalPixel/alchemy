// Draft, differ 42 of 168 (tla-en, 2026-11-05). Older notes below may predate this figure.
// One burst particle thrown from the actor: kind 0x1e9 at a jittered position,
// random speed and size, its sprite taking the owner sprite's shape and tile.
// What the form rests on, each measured here:
// - The tile mask: a pool placed before the epilogue, with a branch over it,
//   is thumb_movhi's 64-byte pool range. It needs `s32 word`, so the HImode AND
//   takes (subreg:HI word) and swaps the mask first; combine then cannot fold
//   it. A bit-field store or a u16 local gives movs or an SImode pool entry.
// - x and the source sprite share one variable. Otherwise source is local to
//   the block, 4 * 2 calls < 12 refs, and it takes caller-saved r4 and a stack
//   slot. Set before the last object use it conflicts with r7 and lands in r8.
// - `multiply` is set before `radius <<= 3`: one instruction of live length
//   puts radius above it, so radius takes r6 and multiply r8.
// Left, three places:
// - The x sum: the listing has `mov r8, r3; add r8, r5`, then the
//   `mov r1, r8; subs; mov r8, r1` triple, which a block-local x gives (the
//   load ties to it). A shared x cannot tie, and `x = actor->x; x += ...` is 24
//   refs, over the 20 that keep it from r4. `left` here costs the 5 instructions.
// - The stores before Resource_ResetEntry: `ldr r5, [r7, #80]` and
//   `ldr r3, [r3, #80]` issue too early and `movs r6, #0; mov r1, r9` too late.
//   1440 statement orders, and an explicit zero local, gave nothing better, and
//   the order does not move the loads: the twins' load-after-store family (K2).
// - The tile store: word and mask swap r3 and r1. word must outrank tile and
//   tile the mask; fifteen spellings gave r1/r3 or r3/r2 (word first needs tile
//   shifted early, which then drops below the mask).
// Tried and out: asm operands to make source non-local (spills, 123), scoped
// copies of source (copy-propagated), one-pass blocks (no effect).
#include "types.h"
#include "call.h"
#include "actor.h"
#include "iwram_call.h"

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 attr2;
  u8 unk0a[6];
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u8 gData_020023c4[];
extern u8 gData_02004774[];
struct Actor *Func_020041e4(s32 kind, s32 x, s32 y, s32 z);
void Func_020041dc(struct Actor *object, const void *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Random16Far(void);
void ResourceMetadata_ClearRecord(void *record);
void Resource_ResetEntry(s32 entry);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_020024d4(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;
  s32 x;

  if (work->owner == NULL)
    return;
  i = 0;
next:
  {
    struct Actor *object;
    s32 a, b, d;
    s32 left, y, z;

    a = Random16Far();
    b = Random16Far();
    left = actor->x + a * 16;
    x = left - b * 16;
    y = Random16Far() * 8;
    d = Random16Far() * 16;
    z = actor->z + d - Random16Far() * 16;
    y = y + actor->y + 0x40000;
    object = Func_020041e4(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      s32 half;
      s32 (*multiply)(s32, s32);
      struct Sprite *sprite;
      s32 word, tile, velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      half = 0x8000;
      /* FAKEMATCH(barrier): the empty asm on half changes its allocation rank; as no
       * asm, 66 of 168 instructions differ in tla-en. */
      asm("" : : "r"(half));
      Func_020041dc(object, gData_02004774);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      {
        s32 cosine = Math_Cosine(angle);
        multiply = Iwram_MulQ16;
        radius <<= 3;
        object->velocityZ = multiply(radius, cosine);
      }
      velocity = multiply(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      scale = (sum >> 2) + half;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = half;
      sprite = object->animation;
      x = (s32)work->owner->animation;
      object->acceleration = 0;
      object->velocityX = velocity;
      Resource_ResetEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      ResourceMetadata_ClearRecord(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = ((struct Sprite *)x)->unk14[0];
      sprite->unk14[1] = ((struct Sprite *)x)->unk14[1];
      sprite->shape = ((struct Sprite *)x)->shape;
      sprite->size = ((struct Sprite *)x)->size;
      tile = ((struct Sprite *)x)->attr2;
      /* FAKEMATCH(barrier): the empty asm on tile changes its allocation rank; as no
       * asm, 44 of 168 instructions differ in tla-en. */
      asm("" : : "r"(tile));
      /* FAKEMATCH(barrier): the empty asm on tile changes its allocation rank; as no
       * asm, 62 of 168 instructions differ in tla-en. */
      asm("" : "=r"(tile) : "0"(tile));
      word = sprite->attr2;
      /* FAKEMATCH(barrier): the empty asm on word changes its allocation rank; as no
       * asm, 64 of 168 instructions differ in tla-en. */
      asm("" : "=r"(word) : "0"(word));
      sprite->attr2 = (word & ~0x3ff) | ((u32)(tile << 22) >> 22);
    }
  }
  if (--i >= 0)
    goto next;
}
