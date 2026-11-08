// Differ 8 of 168 (tla-en, 2026-11-08, -mtune=gba), from 22. Func_020024d4 of
// overlay 6ab. Scale computed before the calls gives the mul pointer r8 and
// the angle r10, as natively.
// Left: scheduling only. Natively the 0x8000 copy (mov r1, r9) and the zero
// come before the stores to scaleX and scaleY, the sprite load (ldr r5,
// [r7, #80]) follows them and the owner's animation load (ldr r3, [r3, #80])
// comes after the stores to bounce, velocityX and acceleration; here both
// loads issue first. sched2 shows the owner's animation load (void *) does not
// depend on the s32 stores; read through an s32 it waits for all of them (12:
// the scale computation then moves ahead of the gravity store), so the native
// load seems to wait on the bounce, velocity and acceleration stores but not
// the scale stores. Tried 8 November: statement orders of gravity, scale,
// bounce, sprite, velocityX, acceleration and source (8 or worse), once-only
// blocks (14 to 20), a zero local (91), a union or s32 read of the animation
// (12), and 8 minutes of permute from the s32 form (best 8). Tried later on 8
// November: the 0x8000 local and the cosine block dropped (free, so gone);
// chained scale stores, block-scoped sprite and velocity locals, radius
// shifted at its read, bounce, acceleration or velocityY moved (8 or worse);
// both animation loads read as s32 (12); 10 minutes of permute (best 8). In
// sched2 the two loads rank 54 and 53 against 52 for the zero and the bounce
// copy, so they always issue first.
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
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      scale = (sum >> 2) + 0x8000;
      Func_020041dc(object, gData_02004774);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      velocity = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
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
