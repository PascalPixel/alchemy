#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

s32 Math_Cosine(s32 angle);
void Func_02005480(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Func_02001894(struct Actor *self);
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const struct EffectOptions *options);

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): MulQ16 forwarded inline; as direct calls to
   * Iwram_MulQ16, 69 of 92 instructions differ in tla-en. */
  return Iwram_MulQ16(a, b);
}

struct GustObject *Func_02001a54(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + MulQ16(0x80000, Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = (struct GustObject *)FieldActor_Create(280, position.x, source->y, position.z);
  Func_02005480(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001894;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_020035b8(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

#include "frame.h"
#include "field_event.h"

s32 Func_02001b2c(struct Actor *object) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    Func_020035b8(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}

#include "call.h"
#include "field_coordinate.h"
#include "field_sprite.h"

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02001bcc(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 24, 0x2000000, 0xf80000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 23, 0x2000000, 0xf80000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, 0x1d80000, 0x1000000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (GameFlag_Test(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (GameFlag_Test(0xa0a) && GameFlag_Test(0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

#include "room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

s32 Func_02001d5c(struct Actor *a, struct Actor *b) {
  if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&
      ABS(a->z - b->z) < 0x80000)
    return 1;
  return 0;
}

s32 Func_02001dc0(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)
      return 1;
  }
  return 0;
}
