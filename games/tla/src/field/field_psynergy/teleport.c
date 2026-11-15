/// @file
/// Teleport: its whirl and sparks.
// Part of field_psynergy.c.

#include "../field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_work.h"
#include "battle/battle_unit.h"
#include "window.h"
#include "bg_scroll.h"

#define WHIRL_COUNT 100

// Heap block 92: the decoded picture, then the sparks and their sprites.
struct TeleportWhirlWork {
  u8 picture[0x400];
  struct WhirlSpark sparks[WHIRL_COUNT];
  struct EffectSprite sprites[WHIRL_COUNT];
};

extern char ResourceId_ColouredOrbs;

/// A whirl of coloured orbs (ResourceId_ColouredOrbs) round the leader, with
/// sound 200. The scenes with Teleport circles call it (Mars Lighthouse, the
/// Anemos Inner Sanctum, the Yampi Desert and Islet caves, Contigo).
// XXX: whether the whirl is the leaving or the arriving half of a teleport;
// a render of a Teleport circle would settle it.
void Teleport_PlayWhirl(void) {
  s32 position[3];
  struct TeleportWhirlWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct WhirlSpark *spark;
  struct EffectSprite *sprite;
  s32 next;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 angle;
  s32 frame;
  s32 radius;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, spark = work->sparks; i < WHIRL_COUNT; i++, spark++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    spark->age = -1;
  }
  Sound_PlayFar(SE_SPARKLE_FALL);
  wait(20);
  object->scaleX = 0;
  object->scaleY = 0;
  Actor_SetSpriteFlagsFar(object, 0);

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    if (count != 0) {
      radius = 0x500000;
      i = count;
      do {
        angle = Random16();
        spark = &work->sparks[next];
        spark->age = 0;
        SET_POSITION(spark, object->x.fixed, object->y.fixed + Random16() * 20,
                     object->z.fixed);
        Vector_AddPolarOffset(Random16() << 3, angle, &spark->x);
        next++;
        spark->angle = angle;
        spark->radius = radius;
        next %= WHIRL_COUNT;
        i--;
      } while (i != 0);
    }

    spark = work->sparks;
    sprite = work->sprites;
    for (i = 0; i < WHIRL_COUNT; i++, sprite++, spark++) {
      if (spark->age >= 0) {
        if ((u32)spark->age < 20) {
          s32 sparkAngle = spark->angle;
          s32 sparkRadius = spark->radius;
          position[0] = spark->x;
          position[1] = spark->y;
          position[2] = spark->z;
          Vector_AddPolarOffset(sparkRadius, sparkAngle, position);
          Camera_WorldToScreen(position);
          sprite->x = position[0];
          sprite->y = position[2];
          EffectSprite_Draw(sprite);
          spark->angle += TRIG_TURN / 32;
          spark->radius += -0x40000;
        }
        if (++spark->age == 20)
          spark->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 20)
        count = 3;
      if (frame == 30) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += TRIG_TURN / 8;
      object->scaleX = (frame << 16) / 30;
      if (object->scaleX > Q16_ONE)
        object->scaleX = Q16_ONE;
      object->scaleY = object->scaleX;
      if (frame == 40) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      if (frame == 0) {
        object->facing = TRIG_TURN / 4;
        Actor_SetSpriteFlagsFar(object, 1);
        count = 3;
      }
      if (frame == 10)
        count = 1;
      if (frame == 20)
        count = 0;
      if (frame == 40)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  FieldPsynergy_MarkWorldMapTilesUsed();
}

#define SPARK_COUNT 100

// Heap block 92: the decoded picture, then the sparks and their sprites.
struct SparkWork {
  u8 picture[0x400];
  struct FieldPsynergyParticle particles[SPARK_COUNT];
  struct EffectSprite sprites[SPARK_COUNT];
};

/// Sparks burst from the leader, Teleport_PlayWhirl's companion in the same
/// scenes.
void Teleport_PlaySparks(void) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  s32 owner = Party_GetLeaderActor();
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct FieldPsynergyParticle *particle;
  struct EffectSprite *sprite;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 spin;
  s32 angle;
  s32 next;
  s32 frame;
  s32 zero = 0;
  s32 age;

  wait(20);
  FieldPsynergy_FadeToCastColor();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, particle = work->particles; i < SPARK_COUNT; i++, particle++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->timer = -1;
  }
  wait(10);
  Actor_FaceDirectionById(owner, 0x4000, 0);
  wait(20);
  Actor_SelectAnimationFar(object, 28);
  object->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  wait(30);
  Sound_PlayFar(SE_SHIMMER);
  Actor_SelectAnimationFar(object, 3);
  for (spin = 0, frame = 49; frame >= 0; frame--, spin += 60) {
    object->facing += spin + TRIG_TURN / 16;
    wait(1);
  }

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    for (i = 0; i < count; i++) {
      angle = Random16();
      particle = &work->particles[next];
      particle->timer = zero;
      SET_POSITION(particle, object->x.fixed, object->y.fixed + Random16() * 20,
                   object->z.fixed);
      Vector_AddPolarOffset(Random16() << 3, angle, &particle->x);
      if (next & 1) {
        particle->velocityX = zero;
        particle->velocityY = -(px(4));
        particle->velocityZ = zero;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      } else {
        particle->velocityX = 0;
        particle->velocityY = px(5);
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      }
      next++;
      next %= SPARK_COUNT;
    }

    for (i = 0, particle = work->particles, sprite = work->sprites;
         i < SPARK_COUNT; i++, sprite++, particle++) {
      age = particle->timer;
      if (age >= 0) {
        if ((u32)age < 20) {
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          // Reread in each arm: the two calls share one bl only this way.
          if (i & 1) {
            BattleEffect_IntegrateVector3(&particle->x, 63, 0x8000);
            age = particle->timer;
          } else {
            BattleEffect_IntegrateVector3(&particle->x, 63, -0xa000);
            age = particle->timer;
          }
        }
        particle->timer = ++age;
        if (age == 20)
          particle->timer = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 10)
        Sound_PlayFar(SE_KOLIMA_BRIDGE_POST);
      object->facing += TRIG_TURN / 8;
      if (frame == 40)
        count = 3;
      if (frame == 50)
        count = 4;
      if (frame == 60) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += TRIG_TURN / 8;
      object->scaleX = Q16_ONE - (frame << 16) / 30;
      if (object->scaleX < 0)
        object->scaleX = 0;
      object->scaleY = object->scaleX;
      Actor_SetSpriteFlagsFar(object, 0);
      if (frame == 30) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      count = 0;
      if (frame == 20)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Runs Teleport_PlaySparks, fades the palettes back to the saved ones and
/// waits 10 frames.
void Teleport_PlaySparksAndFade(void) {
  Teleport_PlaySparks();
  FieldPsynergy_FadeFromCastColor();
  wait(10);
}

/// Teleport cast on a target: does nothing.
void FieldPsynergy_TeleportOnTarget(void) {}

/// Teleport: raises FIELD_RAISED_TRIGGER_TELEPORT, which shows the picture of
/// the whole world for the player to choose where to go.
void FieldPsynergy_Teleport(void) {
  ObjectTable_Get(Party_GetLeaderActor());
  gFieldWork->raisedTrigger = FIELD_RAISED_TRIGGER_TELEPORT;
}
