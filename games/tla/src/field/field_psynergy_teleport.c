/// @file
/// Teleport: the leader leaving in a burst of sparks and arriving in a whirl
/// of them, and the cast that opens the picture of the world.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_work.h"
#include "battle/battle_unit.h"
#include "window/window.h"
#include "system/bg_scroll.h"

#define WHIRL_COUNT 100 ///< sparks that whirl round the leader
/// The frames a spark flies before it vanishes.
#define TELEPORT_SPARK_FRAMES 20
/// Before the sparks, the leader spins for TELEPORT_SPIN_FRAMES, its turn each
/// frame TELEPORT_SPIN_ACCELERATION (1/65536 turns) faster than the last.
#define TELEPORT_SPIN_FRAMES 50
#define TELEPORT_SPIN_ACCELERATION 60

/// The stages of Teleport_PlayWhirl.
enum WhirlStage {
  WHIRL_FALLS_IN = 0, ///< more and more sparks fall in round the hidden leader
  WHIRL_LEADER_GROWS = 1,
  WHIRL_DIES_AWAY = 2
};

/// The stages of Teleport_PlaySparks.
enum SparkStage {
  SPARKS_BURST = 0, ///< more and more sparks burst from the spinning leader
  SPARKS_LEADER_SHRINKS = 1,
  SPARKS_DIE_AWAY = 2
};

/// Teleport's whirl (heap slot battleEffect): the decoded spark picture,
/// then the 100 sparks and their sprites.
struct TeleportWhirlWork {
  u8 picture[1024];
  struct WhirlSpark sparks[WHIRL_COUNT];
  struct EffectSprite sprites[WHIRL_COUNT];
};

/// The arriving half of a teleport: a whirl of sparks, the coloured orbs of
/// RESOURCE_COLOURED_ORBS, falls in round the hidden leader with
/// SE_SPARKLE_FALL, the leader spins and grows back to full size inside it, and
/// the whirl dies away. The scenes with Teleport circles call it (Mars
/// Lighthouse, the Anemos Inner Sanctum, the Yampi Desert and Islet caves,
/// Contigo).
void Teleport_PlayWhirl(void) {
  s32 position[3];
  struct TeleportWhirlWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  struct Actor *actor = Actor_Lookup(Party_GetLeaderActor());
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
  FIXED radius;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_COLOURED_ORBS), work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 1024, work);
  for (i = 0, spark = work->sparks; i < WHIRL_COUNT; i++, spark++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0,
                      tiles +
                          (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    spark->age = -1;
  }
  Sound_PlayFar(SE_SPARKLE_FALL);
  wait(20);
  actor->scaleX = 0;
  actor->scaleY = 0;
  Actor_SetSpriteFlagsFar(actor, 0);

  count = 2;
  state = WHIRL_FALLS_IN;
  next = 0;
  frame = 0;
  do {
    if (count != 0) {
      radius = toFIXED(80);
      i = count;
      do {
        angle = Random16();
        spark = &work->sparks[next];
        spark->age = 0;
        SET_POSITION(spark, actor->x.fixed, actor->y.fixed + Random16() * 20,
                     actor->z.fixed);
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
        if ((u32)spark->age < TELEPORT_SPARK_FRAMES) {
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
          spark->radius += -toFIXED(4);
        }
        if (++spark->age == TELEPORT_SPARK_FRAMES)
          spark->age = -1;
      }
    }

    switch (state) {
    case WHIRL_FALLS_IN:
      if (frame == 20)
        count = 3;
      if (frame == 30) {
        count = 5;
        state = WHIRL_LEADER_GROWS;
        frame = -1;
      }
      break;
    case WHIRL_LEADER_GROWS:
      actor->facing += DEGtoANG(45);
      actor->scaleX = FIXED_ONE * frame / 30;
      if (actor->scaleX > FIXED_ONE)
        actor->scaleX = FIXED_ONE;
      actor->scaleY = actor->scaleX;
      if (frame == 40) {
        state = WHIRL_DIES_AWAY;
        frame = -1;
      }
      break;
    case WHIRL_DIES_AWAY:
      if (frame == 0) {
        actor->facing = DEGtoANG(90);
        Actor_SetSpriteFlagsFar(actor, SPRITE_FLAG_SHADOW);
        count = 3;
      }
      if (frame == 10)
        count = 1;
      if (frame == 20)
        count = 0;
      if (frame == 40)
        state = FIELD_PSYNERGY_FINISHED;
      break;
    }
    wait(1);
    frame++;
  } while (state != FIELD_PSYNERGY_FINISHED);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  FieldPsynergy_MarkWorldMapTilesUsed();
}

/// The sparks of Teleport's leaving burst.
#define TELEPORT_SPARK_COUNT 100

/// Teleport's burst of sparks (heap slot battleEffect): the decoded picture,
/// then the sparks and their sprites.
struct SparkWork {
  u8 picture[1024];
  struct FieldPsynergyParticle particles[TELEPORT_SPARK_COUNT];
  struct EffectSprite sprites[TELEPORT_SPARK_COUNT];
};

/// The leaving half of a teleport, Teleport_PlayWhirl's companion in the same
/// scenes: the leader raises both arms in changing colours, spins faster and
/// faster as sparks burst from it, shrinks away and is hidden.
void Teleport_PlaySparks(void) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  s32 owner = Party_GetLeaderActor();
  struct Actor *actor = Actor_Lookup(Party_GetLeaderActor());
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
  s32 age;

  wait(20);
  FieldPsynergy_FadeToCastColor();
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_COLOURED_ORBS), work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 1024, work);
  for (i = 0, particle = work->particles; i < TELEPORT_SPARK_COUNT;
       i++, particle++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0,
                      tiles +
                          (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    particle->timer = -1;
  }
  wait(10);
  Actor_FaceDirectionById(owner, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(actor, ANIM_LEADER_ARMS_RAISED);
  actor->update = Actor_CycleSpriteColors;
  Sound_PlayFar(SE_PSYNERGY_SPARKLE);
  wait(30);
  Sound_PlayFar(SE_SHIMMER);
  Actor_SelectAnimationFar(actor, ANIM_NOD);
  for (spin = 0, frame = 0; frame < TELEPORT_SPIN_FRAMES;
       frame++, spin += TELEPORT_SPIN_ACCELERATION) {
    actor->facing += spin + FACING_STEP;
    wait(1);
  }

  count = 2;
  state = SPARKS_BURST;
  next = 0;
  frame = 0;
  do {
    for (i = 0; i < count; i++) {
      angle = Random16();
      particle = &work->particles[next];
      particle->timer = 0;
      SET_POSITION(particle, actor->x.fixed, actor->y.fixed + Random16() * 20,
                   actor->z.fixed);
      Vector_AddPolarOffset(Random16() << 3, angle, &particle->x);
      if (next & 1) {
        particle->velocityX = 0;
        particle->velocityY = -toFIXED(4);
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + toFIXED(4), angle,
                              &work->particles[next].velocityX);
      } else {
        particle->velocityX = 0;
        particle->velocityY = toFIXED(5);
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + toFIXED(4), angle,
                              &work->particles[next].velocityX);
      }
      next++;
      next %= TELEPORT_SPARK_COUNT;
    }

    for (i = 0, particle = work->particles, sprite = work->sprites;
         i < TELEPORT_SPARK_COUNT; i++, sprite++, particle++) {
      age = particle->timer;
      if (age >= 0) {
        if ((u32)age < TELEPORT_SPARK_FRAMES) {
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (i & 1)
            Field_AdvanceWithGravity3D(&particle->x, 63, toFIXED(1) / 2);
          else
            Field_AdvanceWithGravity3D(&particle->x, 63, -(toFIXED(5) / 8));
        }
        if (++particle->timer == TELEPORT_SPARK_FRAMES)
          particle->timer = -1;
      }
    }

    switch (state) {
    case SPARKS_BURST:
      if (frame == 10)
        Sound_PlayFar(SE_KOLIMA_BRIDGE_POST);
      actor->facing += DEGtoANG(45);
      if (frame == 40)
        count = 3;
      if (frame == 50)
        count = 4;
      if (frame == 60) {
        count = 5;
        state = SPARKS_LEADER_SHRINKS;
        frame = -1;
      }
      break;
    case SPARKS_LEADER_SHRINKS:
      actor->facing += DEGtoANG(45);
      actor->scaleX = FIXED_ONE - FIXED_ONE * frame / 30;
      if (actor->scaleX < 0)
        actor->scaleX = 0;
      actor->scaleY = actor->scaleX;
      Actor_SetSpriteFlagsFar(actor, 0);
      if (frame == 30) {
        state = SPARKS_DIE_AWAY;
        frame = -1;
      }
      break;
    case SPARKS_DIE_AWAY:
      count = 0;
      if (frame == 20)
        state = FIELD_PSYNERGY_FINISHED;
      break;
    }
    wait(1);
    frame++;
  } while (state != FIELD_PSYNERGY_FINISHED);
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
  // DEAD: the leader looked up and never used; the ROM keeps the calls.
  Actor_Lookup(Party_GetLeaderActor());
  gFieldWork->raisedTrigger = FIELD_RAISED_TRIGGER_TELEPORT;
}
#undef WHIRL_COUNT
#undef TELEPORT_SPARK_FRAMES
#undef TELEPORT_SPIN_FRAMES
#undef TELEPORT_SPIN_ACCELERATION
#undef TELEPORT_SPARK_COUNT
