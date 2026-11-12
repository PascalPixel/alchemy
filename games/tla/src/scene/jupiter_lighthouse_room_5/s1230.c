/// @file
/// Scene code of overlay 0x69e, in ROM order, from s1230's offset up to the next object the overlay links.

#include "types.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "frame.h"

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
void Func_02002810(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_02001070(struct GustObject *object);
void Func_02001c88(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_02001230(struct GustObject *source) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 69 of 92 instructions differ in
   * tla-en. */
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                    position.z));
  Func_02002810(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001070;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02001c88(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

u32 Random16Far(void);

s32 Func_02001308(struct FieldActor *object) {
  struct EffectOptions options;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase != 0)
    goto done;
  options.priority = 3;
  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  options.palette = 14;
  x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
  speed = (((s32)((u32)(Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
  Func_02001c88(x, y, object->z, 0, speed, phase,
                EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                    EFFECT_USE_START_SCALE,
                &options);
done:
  return 0;
}

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

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

void Func_020013a8(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 =
      ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 =
      ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(9)), 4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(11)),
                               4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(10)),
                               4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(12)),
                               4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
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

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 duration;
  s32 age;
};

struct SparkSprite {
  u8 unk00[0x0c];
  s32 x;
  s32 z;
  u8 unk14[0x14];
};

struct SparkWork {
  u8 unk0000[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
};

void Func_020029b0(s32 *position);
void Func_020029e8(struct SparkSprite *sprite);

void Func_02001538(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; sprite++, i--, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      s32 sine = Math_Sin((s32)((spark->age << 15) / duration));
      s32 *p = position;
      p[0] = work->from[0] +
             (work->to[0] - work->from[0]) * spark->age / duration +
             sine * spark->dx;
      sine = Math_Sin((s32)((spark->age << 15) / duration));
      p[1] = work->from[1] +
             (work->to[1] - work->from[1]) * spark->age / duration +
             sine * spark->dy;
      p[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Func_020029b0(p);
      sprite->x = p[0];
      sprite->z = p[2];
      Func_020029e8(sprite);
      spark->age++;
    }
  }
}
