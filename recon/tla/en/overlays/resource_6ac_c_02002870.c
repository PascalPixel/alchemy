// Matches: differ 0 (tla-en, 2026-11-08)
// Func_02002870 of overlay 6ac; differ 0 of 168 in tla-en, tla-ja, tla-de,
// tla-es, tla-fr and tla-it. Overlay 6ab's Func_020024d4 with this overlay's
// symbols. The last 8 were sched2 ties, closed by two plain-C changes: the
// sprite typed as FieldSprite lays it out, with its s32 scale at 0x0c, so the
// vramBlock read may alias the six s32 stores to the object and they all rank
// 54, ahead of the sprite and owner animation loads; and the bounce store
// written after acceleration, so the zero (r6) comes before the Q16_ONE / 2
// copy (r1) in insn order and wins their tie at 55. No devices.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "iwram_call.h"
#include "fixed_math.h"

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
  u8 unk0a[2];
  s32 scale;
  u8 vramBlock;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
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
s32 Resource_ResetEntry(u32 slot);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_02002870(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  // A once-only loop: one object per call.
  for (i = 0; i >= 0; i--) {
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
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = Func_02003128(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      scale = (sum >> 2) + Q16_ONE / 2;
      Func_02003120(object, gData_02003304);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      velocity = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->animation;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      source = work->owner->animation;
      Resource_ResetEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
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
}
