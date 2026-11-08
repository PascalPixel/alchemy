// Matches: differ 0 (tla-en, 2026-11-08)
// Func_020029c8 of overlay 6aa, overlay 6ab's Func_020024d4 with this
// overlay's symbols; tla-ja, tla-de, tla-es, tla-fr and tla-it also measure
// differ 0. Closed from 8 by plain C, no device: the sprite's s32 scale at
// 0x0c (FieldSprite's) gives struct Sprite an int member, so its alias set
// overlaps the object's s32 stores and the ldrb of sprite->vramBlock must wait
// for them; that lowers the two animation loads' sched2 priority under the
// stores (2 left). Storing velocityX before acceleration and bounce then puts
// the zero ahead of the 0x8000 copy, which settles their tie (0). The half
// local, the cosine block, the velocity local and the split sum and x were
// then dropped at no cost.
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
  u8 unk0a[2];
  s32 scale; ///< 16.16, as FieldSprite (field_sprite.h) has it
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
extern u8 gData_020056d0[];
struct Actor *Func_02004bd8(s32 kind, s32 x, s32 y, s32 z);
void Func_02004bd0(struct Actor *object, const void *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Random16Far(void);
void ResourceMetadata_ClearRecord(void *record);
void Resource_ResetEntry(s32 entry);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_020029c8(struct Actor *actor) {
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
    x = actor->x + a * 16 - b * 16;
    c = Random16Far();
    d = Random16Far();
    e = Random16Far();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = Func_02004bd8(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16Far();
      radius = Random16Far() * 8;
      sum = Random16Far() + Random16Far();
      scale = (sum >> 2) + 0x8000;
      Func_02004bd0(object, gData_020056d0);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = 0x8000;
      sprite = object->animation;
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
