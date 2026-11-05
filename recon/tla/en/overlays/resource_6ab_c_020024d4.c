// Draft, differ 8 of 168 (tla-en, 2026-11-05), from 22. Func_020024d4 of
// overlay 6ab. Scale computed before the calls gives the mul pointer r8 and
// the angle r10, as natively, but moves its shift; natively lsrs and add come
// after the gravity store. Left: the shift's place, and the sprite's and
// owner's loads after the zero and the half's copy natively.
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

  if (work->owner == NULL)
    return;
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
    y = c * 8 + actor->y + 0x40000;
    object = Func_020041e4(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      s32 half;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      half = 0x8000;
      scale = (sum >> 2) + half;
      Func_020041dc(object, gData_02004774);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      {
        s32 cosine = Math_Cosine(angle);
        radius <<= 3;
        object->velocityZ = Iwram_MulQ16(radius, cosine);
      }
      velocity = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = half;
      sprite = object->animation;
      object->velocityX = velocity;
      object->acceleration = 0;
      source = work->owner->animation;
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
}
