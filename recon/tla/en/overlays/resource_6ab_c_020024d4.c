// Draft, differ 35: overlay 6ab's copy of the function drafted in
// resource_6ac_c_02002870.c, whose header says what it is and what still differs.
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
    object = Func_020041e4(0x1e9, x, y, z);
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
      /* FAKEMATCH(barrier): the empty asm on half changes its allocation rank; as no
       * asm, 39 of 168 instructions differ in tla-en. */
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
