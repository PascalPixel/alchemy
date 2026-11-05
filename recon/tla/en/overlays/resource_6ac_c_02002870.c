// Draft, differ 67 of 168, skeleton gap 16/13 (tla-en, 2026-11-01)
// One burst particle thrown from the actor: kind 0x1e9 at a jittered position,
// random speed and size, its sprite taking the owner sprite's shape and tile.
// Same shape as 6ab's Func_020024d4 and 6aa's Func_020029c8; no device here.
// What the form rests on, each measured here:
// - goto loop: for or do/while (differ 125) hoists the zero and 0x8000 into
//   high registers and frees i from the stack; the listing rebuilds them.
//   `i = 0 ... --i >= 0` is loop.c's reversed count, kept by hand.
// - c, d and e read raw, shifted at the sum: closes the lsls order.
// - sprite->tile as a bit-field: 5 fewer than an s32 word, and swaps r1 and r3
//   to the listing's, but its mask pool lands after the epilogue. The listing's
//   pool before the epilogue, with a `b` over it, needs `s32 word` with
//   `(word & ~0x3ff) | ...` (differ 72, or 71 with word = ~0x3ff; word &= ...).
// Left:
// - source takes r4 with a caller-save slot at sp+0, so the frame is 8 not 4,
//   i moves to sp+4 and the tail differs; the listing has it in r8. The lreg
//   dump gives it 12 refs across 2 calls, and 4 * 2 < 12 makes r4 profitable;
//   it needs 8 refs or 3 calls, and five reads plus a set after the last
//   multiply give neither. This is most of the 67.
// - Sharing x and source in one variable lands it in r8 (differ 38 with y
//   shifted early, 52 with the raw c): rejected, a local reused for a second
//   value is a `temporary` device, and this round adds none. `register`, y
//   reused and scoped copies gave nothing.
// - The stores before Resource_ResetEntry: movs r6, #0 issues after the
//   animation load; statement order does not move it (three orders here, 1440
//   in 6ab's draft), and it cannot be judged until source is in r8.
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
  u16 tile : 10;
  u16 attr2Rest : 6;
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
extern u8 gData_02003304[];
struct Actor *Func_02003128(s32 kind, s32 x, s32 y, s32 z);
void Func_02003120(struct Actor *object, const void *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Random16Far(void);
void ResourceMetadata_ClearRecord(void *record);
void Resource_ResetEntry(s32 entry);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_02002870(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  i = 0;
next:
  {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16Far();
    b = Random16Far();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16Far();
    d = Random16Far();
    e = Random16Far();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = Func_02003128(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      s32 half;
      s32 (*multiply)(s32, s32);
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      half = 0x8000;
      Func_02003120(object, gData_02003304);
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
      source = work->owner->animation;
      object->acceleration = 0;
      object->velocityX = velocity;
      Resource_ResetEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      ResourceMetadata_ClearRecord(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
  if (--i >= 0)
    goto next;
}
