/// @file
/// Venus Lighthouse, in six scenes: floor switches that slide walls aside,
/// blocks that sink into sockets to open gates, and sparkling passages the
/// leader steps through.

#include "script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/venus_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "object_effect.h"
#include "macros.h"

#define NULL ((void *)0)
#include "global_cells.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "overlay_object.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "iwram_call.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "io_reg.h"
#include "scene_ids.h"
#include "map.h"
#include "map_scroll.h"
#include "field_runtime.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

// The floor switches have slid the walls aside.
#define FLAG_VENUS_LIGHTHOUSE_WALL_MOVED 0x300
#define FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED 0x306
#define FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED 0x307
// The gates the sunken blocks opened: one block, three blocks (and which of
// two sets of sockets they sank into) and the pillar room's four.
#define FLAG_VENUS_LIGHTHOUSE_GATE_OPEN 0x301
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_A 0x302
#define FLAG_VENUS_LIGHTHOUSE_SOCKETS_B 0x303
#define FLAG_VENUS_LIGHTHOUSE_THREE_BLOCK_GATE_OPEN 0x304
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN 0x308

// UB: passage offsets and dust directions can be negative. The callers use
// bounded pixel offsets or directions -1..1, so these products fit s32.
#ifdef UBFIX
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) * (1 << (bits)))
#else
#define VENUS_LIGHTHOUSE_FIXED_OFFSET(value, bits) ((value) << (bits))
#endif

#ifdef UBFIX
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  VenusLighthouse_ArriveThroughSparkPassage
#else
#define VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE                                  \
  ((s32 (*)(void))VenusLighthouse_ArriveThroughSparkPassage)
#endif

#if TBS_PATCH_3
#define VENUS_DESCENT_COLLISION_X 33
#define VENUS_DESCENT_COLLISION_WIDTH 7
#else
#define VENUS_DESCENT_COLLISION_X 34
#define VENUS_DESCENT_COLLISION_WIDTH 5
#endif

struct DirXZ;
static void VenusLighthouse_StepActorWithDust(struct DirXZ *direction);
static void VenusLighthouse_AnimatePairedTileWaves(void);
static void VenusLighthouse_EmitWestPassageParticles(void);
static void VenusLighthouse_EmitEastPassageParticles(void);
static void VenusLighthouse_AnimateWestTileWave(void);
static void VenusLighthouse_AnimateEastTileWave(void);
static void VenusLighthouse_RaiseLeaderWithSpray(void);

// The lighthouse's scene flags:
// FLAG_SCENE_TEMP_00: in the room with the floor switch, the block holds the
// switch down; in the room of floating blocks, FLAG_SCENE_TEMP_00 plus a
// block's number (0 to 3): that block is set down in its place.
// FLAG_SCENE_TEMP_01: the leader stands on the floor switch.

/// Actors the events name, by room. Outside the entrance: the man who tells of
/// the attack and the scholar waiting to go in. In the room with the floor
/// switch: the scholar by the door, the block Move pushes to the wall and the
/// block that holds the switch down.
enum {
  ACTOR_ENTRANCE_MAN = 8,
  ACTOR_ENTRANCE_SCHOLAR = 11,
  ACTOR_DOOR_SCHOLAR = 10,
  ACTOR_WALL_BLOCK = 12,
  ACTOR_SWITCH_BLOCK = 13
};

/// The way a drifting floor carries the party: -1, 0 or 1 on x and on z.
struct DirXZ {
  s32 x;
  s32 unused;
  s32 z;
};

/// The shared radial-effect helper uses this module-provided vector type.
struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

/// A field position, read as halfwords for its whole-pixel parts.
union VecView {
  struct FieldPosition v;
  s16 h[6];
};

static __inline__ void SetFlagBits(u8 *flags, u8 bits) { *flags |= bits; }

/// Sets up a passage effect's motion, collision and child value.
static void VenusLighthouse_ConfigurePassageEffect(struct FieldActor *object,
                                                   s32 childValue) {
  object->motionFlags = 0;
  object->collisionFlags = 8;
  Engine_ActorSetSpriteFlags(object, 0);
  ObjectGroup_SetChildValue(object, childValue);
}

/// Creates a blended passage effect behind the actors.
static struct FieldActor *
VenusLighthouse_CreateBlendedPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *effect;

  effect = Engine_ObjectCreate(type, x, y, z);
  if (effect != 0) {
    ((struct AnimationObject *)effect->sprite)->image.priority = 0;
    VenusLighthouse_ConfigurePassageEffect(effect, 14);
    Engine_ObjectSetBlendMode(effect, 1);
    return effect;
  }
  return 0;
}

/// Creates a passage effect that preserves the overlapping actors' draw order.
static struct FieldActor *
VenusLighthouse_CreateOccludingPassageEffect(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *object;

  object = Engine_ObjectCreate(type, x, y, z);
  if (object != 0) {
    ((struct AnimationObject *)object->sprite)->image.priority = 1;
    VenusLighthouse_ConfigurePassageEffect(object, 15);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return object;
  }
  return 0;
}

#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "objcmd.h"

#include "common/object_distance.h"
#include "common/object_findslot.h"

/// Pushes the adjacent block only when its destination and overhead space are
/// clear.
static void VenusLighthouse_PushAdjacentBlock(void) {
  struct FieldPosition targetPosition;
  struct FieldActor *leader;
  struct FieldActor *block;
  struct FieldActor *blockingActor;
  s32 directionIndex;
  u32 packedStep;
  s32 moveRate;
  s32 transitionBusy;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  directionIndex = leader->facing >> 12;
  // Look ahead for a block before testing its destination and overhead space.
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = leader->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = leader->y.fixed;
  packedStep <<= 16;
  targetPosition.z = leader->z.fixed + packedStep;
  block = SceneData_FindSlotAtPosition(&targetPosition, leader);
  if (block == 0)
    return;

  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = block->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = block->y.fixed;
  packedStep <<= 16;
  targetPosition.z = block->z.fixed + packedStep;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, block);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  targetPosition.x = block->x.fixed;
  targetPosition.y = block->y.fixed + 0x100000;
  targetPosition.z = block->z.fixed;
  blockingActor = SceneData_FindSlotAtPosition(&targetPosition, block);
  if (blockingActor != 0 && (blockingActor->collisionFlags & 1) != 0)
    return;

  block->terrainId = 2;
  packedStep = gStagedActor_DirectionSteps[directionIndex];
  targetPosition.x = block->x.fixed + (packedStep & 0xffff0000);
  targetPosition.y = block->y.fixed;
  packedStep <<= 16;
  targetPosition.z = block->z.fixed + packedStep;
  if (FieldObject_CheckMovementCollision(block, &targetPosition) > 0)
    return;

  transitionBusy = block->riseCounter;
  if (transitionBusy != 0)
    return;

  // Move the block and leader together, then restore their standing state.
  Object_SetMode(leader, 8);
  moveRate = 0x3333;
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);
  block->speed = moveRate;
  block->acceleration = moveRate;
  Engine_ObjectSetPosition(block, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  leader->speed = moveRate;
  leader->acceleration = moveRate;
  Engine_ObjectSetPosition(leader, targetPosition.x, targetPosition.y,
                           targetPosition.z);
  FieldObject_WaitForMove(block);
  BattleFx_PlayQueuedSound();
  block->x.fixed = targetPosition.x;
  block->z.fixed = targetPosition.z;
  block->velocityX = transitionBusy;
  block->velocityZ = transitionBusy;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  leader->velocityX = transitionBusy;
  leader->velocityZ = transitionBusy;
  leader->x.fixed = leader->x.part.pixel << 16;
  leader->z.fixed = leader->z.part.pixel << 16;
  Object_SetMode(leader, 1);
}

/// Fills collision codes in a rectangular area of the requested map layer.
static UNUSED s32 StagedActor_FillGridAttributeRectangle(u32 layerIndex, s32 x,
                                                         s32 z, u32 width,
                                                         u32 height,
                                                         s32 collisionCode) {
  struct MapScrollWork *map = gMapWork[0];
  struct MapCell *cells;
  u32 row;
  u32 column;

  if (map != 0) {
    if (layerIndex <= 2) {
      cells = map->layers[layerIndex].cells;
    } else {
      cells = (struct MapCell *)gMapCellBuffer;
    }
    cells += x + (z << 7);
    for (row = 0; row < height; row++) {
      struct MapCell *cell = cells + (row << 7);

      for (column = 0; column < width; column++) {
        cell->collisionCode = collisionCode;
        cell++;
      }
    }
  }
  return 0;
}

/// Restores child values encoded in the placed actors' script timers.
static void VenusLighthouse_RestoreScriptedChildValues(void) {
  struct FieldActor **actors = gEventWork->placedActors;
  struct FieldActor *actor;
  u32 actorId;

  for (actorId = 8; actorId <= 65; actorId++) {
    actor = actors[actorId - 8];
    if (actor->timer >> 4 == 0x212)
      ObjectGroup_SetChildValue(actor, actor->timer & 15);
  }
}

/// Applies the current script timer's child value and advances the script.
static s32
VenusLighthouse_UpdateChildFromTimer(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer & 15);
  return 0;
}

// Pulse the child effect and play its timed sound cue.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 6 bytes of the 56-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_PulseChildEffect(union FieldObject *fieldObject) {
#endif
  void *actor = (void *)fieldObject;
  if ((gFrameCount & 3) == 0)
    ObjectGroup_SetChildValue(actor, 7);
  else
    ObjectGroup_SetChildValue(actor, 0);

  if ((gFrameCount & 7) == 0)
    Engine_AudioPlayCue(SE_ITEM_BREAK);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Emit a rising particle near the actor every eight frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 172-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_EmitRisingParticles(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    options.priority = 3 - ((u32)(Engine_RandomNext() * 2) >> 16);
    options.startScaleX = 0x6666;
    options.startScaleY = 0x6666;
    options.palette = 14;
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16);
    particleY =
        actor->y.fixed + ((32 - ((u32)(Engine_RandomNext() * 32) >> 16)) << 16);
    riseSpeed = Math_Divide(
        (((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x00050000, 10);
    Effect_Spawn(particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
                 0x000b0000, &options);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Clears the scripted actor's sprite flags and advances its script.
static s32 VenusLighthouse_ClearScriptedSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  void *actor = (void *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

// Emit the two particle patterns used by the leader's passage trail.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 180-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_EmitPassageSparkTrail(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 fallSpeed;
  s32 framePhase;

  options.startScaleX = 0x0000cccc;
  options.startScaleY = 0x0000cccc;
  options.priority = 0;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 0x3333;
  framePhase = gFrameCount & 15;

  Effect_Spawn(actor->x.fixed +
                   VENUS_LIGHTHOUSE_FIXED_OFFSET(8 - framePhase, 16),
               actor->y.fixed + 0x001a0000, actor->z.fixed, 0, -fallSpeed, 0,
               0x000a0000, &options);

  framePhase = gFrameCount & 15;
  if (framePhase == 0) {
    options.startScaleX = 0x00008000;
    options.startScaleY = 0x00008000;
    Effect_Spawn(actor->x.fixed +
                     ((((u32)(Engine_RandomNext() * 9) >> 16) - 4) << 16),
                 actor->y.fixed, actor->z.fixed, 0, 0, 0, 0x000a0000, &options);
  }
#ifdef UBFIX

  return;
#else

  return 0;
#endif
}

/// Walking north into a sparkling passage: the leader dissolves in sparks and
/// comes out at the passage's exit.
static void Event_EnterSparklingPassage(void) {
  struct FieldActor *leader;
  struct EventWork *eventWork;

  eventWork = gEventWork;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
  leader->speed = 0x3333;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  leader->update = 0;
  Engine_EventWait(30);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(eventWork->touchedTrigger);
  Engine_EventEnd();
}

/// Brings the leader out of a sparkling passage on a fresh entrance.
static void VenusLighthouse_ArriveThroughSparkPassage(void) {
  struct FieldActor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    leader->motionFlags = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, leader->x.part.pixel << 16,
                      (leader->z.part.pixel << 16) - 0x100000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Event_PlayCue(SE_228);
#ifdef UBFIX
    leader->update = VenusLighthouse_EmitPassageSparkTrail;
#else
    leader->update =
        (void (*)(union FieldObject *))VenusLighthouse_EmitPassageSparkTrail;
#endif
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 1);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    leader->motionFlags = 3;
    leader->update = 0;
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

/// Stepping onto this spot makes the leader the actor the room's effect
/// follows.
static void Event_SetLeaderAsEffectTarget(void) {
  gActorEffectWork->actor = Object_GetById(ACTOR_PARTY_LEADER);
}

/// Stepping onto this spot: the room's effect follows no one.
static void Event_ClearEffectTarget(void) { gActorEffectWork->actor = NULL; }

#include "common/object_depth.h"
#include "common/object_waitidle.h"
#include "common/effect_advance.h"

/// Drops the leader onto the floor and emits the radial landing dust.
static void VenusLighthouse_LandWithRadialDust(void) {
  struct FieldActor *leader;
  u32 particleIndex;
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  leader->y.fixed = PIXELS(130);
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Event_PlayCue(SE_204);
  leader->motionFlags = 3;
  Engine_EventWait(24);
  options.palette = 7;
  options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    s32 angle;
    s32 velocity[3];

    angle = particleIndex << 12;
    SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
    velocity[0] += velocity[0] / 2;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0],
                 velocity[1], velocity[2], 0x1090001, &options);
  }
  Event_PlayCue(SE_188);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT);
  BattleFx_PlayQueuedSound();
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Engine_EventEnd();
}

#include "common/effect_radial.h"

// Emit shrinking particles around a moving gate effect every four frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 172 bytes (extent 196 to 192 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_EmitGateParticles(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_EmitGateParticles(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 velocityY;
  s32 velocityX;
  s32 velocityZ;
  s32 random;

  if ((gFrameCount & 3) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  if ((u32)(Engine_RandomNext() * 6) >> 16 == 0) {
    if (actor->targetX != ACTOR_NO_TARGET ||
        actor->targetZ != ACTOR_NO_TARGET) {
      Engine_AudioPlayCue(SE_246);
    }
  }
  velocityY = 0;
  options.type = 286;
  options.startScaleX = 0x10000;
  options.startScaleY = 0x10000;
  options.targetScaleX = -327;
  options.targetScaleY = -327;
  random = Engine_RandomNext();
  velocityX = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  random = Engine_RandomNext();
  velocityZ = Math_Divide(((((u32)(random * 9)) >> 16) - 4) << 16, 10);
  Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed - 0x10000,
               velocityX, velocityY, velocityZ, 0x001c0001, &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Creates a scripted gate effect at the supplied pixel coordinates.
static struct FieldActor *
VenusLighthouse_CreateScriptedGateEffect(s32 x, s32 z, const s32 *script) {
  struct FieldActor *effect;
  s32 fixedX;
  s32 fixedZ;

  fixedX = x << 16;
  fixedZ = z << 16;
  effect = Engine_ObjectCreate(284, fixedX, 0, fixedZ);
  if (effect == 0) {
    return 0;
  }
  effect->speed = 0x10000;
  effect->acceleration = 0x10000;
  Engine_ActorSetSpriteFlags(effect, 0);
  Object_SetMode(effect, 7);
  effect->motionFlags = 0;
  effect->y.fixed = 0;
  effect->collisionFlags = 0;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  effect->update = VenusLighthouse_EmitGateParticles;
#else
  effect->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
  effect->riseEnabled = 0;
  Engine_ObjectSetScript(effect, script);
  return effect;
}

/// Walking into a one-tile gap: the leader jumps it if the far side is clear.
/// Returns 1 after a jump, 0 when there was none.
static s32 Event_JumpAcrossGap(void) {
  struct FieldActor *leader;
  struct FieldPosition landing;
  u8 *motionFlags;
  u8 savedMotionFlags;
  s32 direction;
#if !TBS_PATCH_1
  u32 step;
#endif

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  // Check the intervening cell before looking for a clear landing two cells
  // ahead.
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landing.x = (leader->x.fixed & 0xfff00000) + 0x80000;
  landing.y = leader->y.fixed;
  landing.z = (leader->z.fixed & 0xfff00000) + 0x80000;
  direction = (leader->facing + 0x2000) & 0xc000;
  Vector_AddPolarOffset(0x100000, direction, &landing);
#else
  direction = leader->facing >> 12;
  step = gStagedActor_DirectionSteps[direction];
  landing.x = leader->x.fixed + (step & 0xffff0000);
  landing.y = leader->y.fixed;
  step <<= 16;
  landing.z = leader->z.fixed + step;
#endif
  if (FieldObject_CheckMovementCollision(leader, &landing) != 1 &&
      SceneData_FindSlotAtPosition(&landing, leader) == 0) {
#if TBS_PATCH_1
    landing.x = (leader->x.fixed & 0xfff00000) + 0x80000;
    landing.y = leader->y.fixed;
    landing.z = (leader->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, (leader->facing + 0x2000) & 0xc000,
                          &landing);
#else
    step = gStagedActor_DirectionSteps[direction];
    landing.x = leader->x.fixed + ((step & 0xffff0000) << 1);
    landing.y = leader->y.fixed;
    step <<= 17;
    landing.z = leader->z.fixed + step;
#endif
    if (SceneData_FindSlotAtPosition(&landing, leader) == 0 &&
        FieldObject_CheckMovementCollision(leader, &landing) == 0) {
      Engine_EventBegin();
      Object_SetMode(leader, 6);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      Object_SetMode(leader, 7);
      leader->speed = 0x30000;
      leader->acceleration = 0x20000;
      leader->velocityY = 0x40000;
      *motionFlags &= 0x7e;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                                ((union VecView *)&landing)->h[1],
                                ((union VecView *)&landing)->h[5]);
      Object_SetMode(leader, 6);
      Engine_ActorSetSpriteFlags(leader, 1);
      *motionFlags = savedMotionFlags;
      Engine_EventEnd();
      return 1;
    }
  }
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouse1) {
    return gVenusLighthouseEntrances1;
  }
  if (scene == (s32)&SceneId_VenusLighthouse3) {
    return gVenusLighthouseEntrances3;
  }
  if (scene == (s32)&SceneId_VenusLighthouse4) {
    return gVenusLighthouseEntrances4;
  }
  if (scene == (s32)&SceneId_VenusLighthouse5) {
    return gVenusLighthouseEntrances5;
  }
  if (scene == (s32)&SceneId_VenusLighthouse6) {
    return gVenusLighthouseEntrances6;
  }
  return gVenusLighthouseEntrancesOther;
}

/// These scenes supply no exit regions.
const struct SceneRegion *SceneData_ReturnZero(void) { return 0; }

/// Returns the packed exits shared by the lighthouse scenes.
const u32 *SceneData_GetPrimaryTable(void) {
  return gVenusLighthousePrimaryTable;
}

/// Selects and clips the placement rows using the original scene dispatch.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *placements;
  s16 scene;

  scene = gGameState.scene;
  if (scene == (s32)&SceneId_VenusLighthouse1) {
    return gVenusLighthousePlacements1;
  }
  if (scene == (s32)&SceneId_VenusLighthouse2) {
    placements = gVenusLighthousePlacements2;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouse3) {
    placements = gVenusLighthousePlacements3;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouse4) {
    placements = gVenusLighthousePlacements4;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouse5) {
    placements = gVenusLighthousePlacements5;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  if (scene == (s32)&SceneId_VenusLighthouse6) {
    placements = gVenusLighthousePlacements6;
    FieldScene_PrepareActors(placements);
    return placements;
  }
  return gVenusLighthousePlacementsOther;
}

/// Searching a locked door. "The door is tightly locked."
static void Event_SearchLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

/// Talking to the man at the entrance (or reading his mind): he flickers away.
/// "Venus Lighthouse was attacked by strange wanderers..."
static void Event_HearOfTheAttack(void) {
  u32 flickersLeft;
  u32 flickerDelay;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_ENTRANCE_MAN, 3);
  Engine_EventSetMessage(MsgFieldVenusLighthouseWasAttackedBy);
  flickersLeft = 10;
  flickerDelay = 8;
  Engine_EventShowMessageAndWait(ACTOR_ENTRANCE_MAN, 0, 20);
  do {
    Engine_ActorSetChildValue(ACTOR_ENTRANCE_MAN, 15);
    Engine_TaskWait(2);
    Engine_ActorSetChildValue(ACTOR_ENTRANCE_MAN, 0);
    Engine_TaskWait(flickerDelay);
    if (flickerDelay > 3) {
      flickerDelay--;
    }
    flickersLeft--;
  } while (flickersLeft != 0);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD);
  Engine_ActorSetPosition(ACTOR_ENTRANCE_MAN, 0, 0);
  Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
  Engine_EventEnd();
}

/// Talking to the scholar who has waited to see inside Venus Lighthouse.
static void Event_TalkToWaitingScholar(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusIveWaitedLongSeeIts);
  Engine_EventShowMessageAndWait(ACTOR_ENTRANCE_SCHOLAR, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ENTRANCE_SCHOLAR, 2);
  Engine_EventShowMessage(ACTOR_ENTRANCE_SCHOLAR, 0);
  Engine_EventEnd();
}

/// Carry on the block Move could not push past the wall: once it is set down
/// beyond, it stays there (FLAG_VENUS_LIGHTHOUSE_BLOCK_CARRIED).
static void Event_CarryBlockPastWall(void) {
  struct FieldActor *block;

  block = Object_GetById(ACTOR_WALL_BLOCK);
  Engine_EventBegin();
  if (block->x.fixed >> 20 == 54 || block->z.fixed >> 20 == 6) {
    Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_BLOCK_CARRIED);
  }
  Engine_EventEnd();
}

/// Searching a relief shows its words; each relief has its own message, except
/// in English, which keeps one. "There are words carved into the relief..."
static void Event_ReadRelief(void) {
  s32 message;
#if TBS_PATCH_LEVEL != 1
  struct EventWork *work = gEventWork;
#endif
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
#if TBS_PATCH_LEVEL == 1
  message = (s32)MSGNO(MsgVenusThereWordsCarvedIntoRelief);
#else
  message =
      work->facingTrigger + MSGNO(MsgVenusThereWordsCarvedIntoRelief) - 35;
#endif
  Engine_MessageShowCentered(message, 1);
  Engine_EventEnd();
}

/// Reveal fading: the hidden path disappears again.
static void Event_HidePath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagClear(0x161);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(35, 8, 1, 3, column, row);
  Map_CopyCellsTo(35, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(99, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(57, 55, 3, 3, column, row);
  Map_CopyCellsTo(57, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(121, 55, 110, 55, 3, 3);
}

/// Reveal: the hidden path appears and can be walked on.
static void Event_RevealPath(void) {
  s32 column;
  s32 row;

#if TBS_PATCH_1
  Engine_GameFlagSet(0x161);
#endif
  column = 23;
  row = 8;
  Map_CopyCellAttributes(36, 8, 1, 3, column, row);
  Map_CopyCellsTo(36, 8, 23, 8, 1, 3);
  Map_CopyCellsTo(100, 8, 87, 8, 1, 3);
  column = 46;
  row = 55;
  Map_CopyCellAttributes(53, 55, 3, 3, column, row);
  Map_CopyCellsTo(53, 55, 46, 55, 3, 3);
  Map_CopyCellsTo(117, 55, 110, 55, 3, 3);
}

/// Walking off the ledge before the floor switch is held down: the leader jumps
/// down to the floor below and shakes his head.
static void Event_JumpDown(void) {
  struct FieldActor *leader;
  s32 landingQuarterZ;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gVenusLeaderApproachScript);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
  leader->velocityY = 0x40000;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
  if (leader->z.fixed >> 20 <= 54) {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 210;
  } else {
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    landingQuarterZ = 238;
  }
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel,
                      landingQuarterZ << 2);
  Engine_EventWait(1);
  SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags,
              ACTOR_FACES_MOTION);
  Engine_EventWait(20);
#ifdef UBFIX
  leader->update = VenusLighthouse_EmitRisingParticles;
#else
  leader->update =
      (void (*)(union FieldObject *))VenusLighthouse_EmitRisingParticles;
#endif
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  leader->update = 0;
  Engine_EventEnd();
}

/// Searching the statue: the room flashes and the path it opens changes, and
/// the camera shows it. "The statue speaks to Isaac's soul..."
static void Event_SearchStatue(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgVenusStatueSpeaksIsaacSoulYe, 1);
  ColorBuffer_ApplySource(0x10000, 0);
  ColorBuffer_ApplyTarget(0x10005, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(100);
  Event_PlayCue(SE_142);
  Engine_EventWait(30);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(70);
  // The first consultation selects a route; later consultations alternate it.
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) == 0) {
      if ((gFrameCount & 1) != 0) {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
      } else {
        GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
      }
    }
  }
  if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) == 0) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
    Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
    Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
    Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
    Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
  } else {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH);
    GameFlag_Clear(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH);
    Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
    Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
    Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
    Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
    Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
    Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
    Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
    Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
  }
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  // Follow the revealed path after the statue and color effect have finished.
  Camera_SetSpeed(0x8000, 0x1000);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(542), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(50);
  Camera_MoveTo(PIXELS(456), -1, PIXELS(423), 1);
  Engine_CameraWaitForMove();
  Engine_EventEnd();
  work->busy = 0;
}

/// Move on the block by the wall: once it reaches the wall, the first time
/// only, Garet comes out. "Hmmm. We can't push the block farther than this."
static void Event_MoveBlockTowardWall(void) {
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 conversationFlag;
  s32 walkSpeed;
  s32 walkAcceleration;
  s32 emote;
  s32 garetX;

  conversationFlag = FLAG_VENUS_LIGHTHOUSE_BLOCK_AT_WALL;
  walkSpeed = 0xcccc;
  walkAcceleration = 0x6666;
  emote = EMOTE_IN_FRONT;
  garetX = 0x338;
  Engine_EventBegin();
  actor = Object_GetById(ACTOR_WALL_BLOCK);
  if (actor->x.fixed >> 20 == 53) {
    if (Engine_GameFlagIsSet(conversationFlag) == 0) {
      // Record the one-time conversation before bringing Garet out to inspect
      // the block.
      Engine_GameFlagSet(conversationFlag);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
      }
      Engine_ActorSetSpeed(GARET, walkSpeed, walkAcceleration);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorWalkToAndWait(GARET, garetX, 104);
      Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVenusHmmmWeCantPushBlock);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorFaceDirection(GARET, FACING_EAST, 10);
      Engine_ActorShowEmote(GARET, emote, 60);
      Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(20);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(20);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      Engine_EventWait(30);
      Engine_ActorWalkToAndWait(GARET, garetX, 88);
      Engine_ActorSetAnimation(GARET, ANIM_WALK);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(GARET);
      Engine_ActorSetPosition(GARET, 0, 0);
      Engine_EventEnd();
    }
  }
}

/// Pushing the block by the wall: as Move, with Garet's remark at the wall.
static void Event_PushBlockTowardWall(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentBlock();
  Engine_EventWait(20);
  Engine_EventEnd();
  Event_MoveBlockTowardWall();
}

/// Move on the block by the floor switch: on the switch it sinks in and holds
/// it down.
static void Event_MoveBlockOntoSwitch(void) {
  struct FieldActor *block;
  s32 switchHeight;

  block = Object_GetById(ACTOR_SWITCH_BLOCK);
  Engine_EventBegin();
  if (block->x.fixed >> 20 == 42) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(SE_188);
    block->motionFlags = 0;
    switchHeight = 0xfffe0000;
    block->terrainHeight = switchHeight;
    block->y.fixed = switchHeight;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
  }
  Engine_EventEnd();
}

/// Standing on the floor switch presses it; stepping off releases it.
static void Event_StepOnFloorSwitch(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 x = leader->x.part.pixel;
  s32 z = leader->z.part.pixel;

  if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
    Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    leader->motionFlags |= 1;
    leader->terrainHeight = 0;
    leader->y.fixed = 0;
  } else if (!GameFlag_IsSet(FLAG_SCENE_TEMP_01)) {
    Engine_EventBegin();
    Engine_EventWait(5);
    Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
    Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
    GameFlag_Set(FLAG_SCENE_TEMP_01);
    Engine_AudioPlayCue(SE_161);
    leader->motionFlags &= ~1;
    leader->terrainHeight = -PIXELS(2);
    leader->y.fixed = -PIXELS(2);
    Engine_EventEnd();
  }
}

/// Talking to the scholar by the door: he explored once it opened, until
/// Kraden and the others appeared; the camera looks at the door.
static void Event_TalkToScholarByDoor(void) {
  struct FieldWork *work;

  work = (struct FieldWork *)gEventWork;
  work->revealFrames = 0;
  work->busy = 1;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVenusThoughtIdExploreAfterDoor);
  Actor_FaceActor(ACTOR_DOOR_SCHOLAR, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DOOR_SCHOLAR, 0, 20);
  Actor_FaceDirection(ACTOR_DOOR_SCHOLAR, FACING_NORTHEAST, 0);
  Camera_SetSpeed(0x10000, 0x2000);
  Camera_MoveTo(PIXELS(448), -1, PIXELS(432), 1);
  Engine_CameraWaitForMove();
  Event_ShowMessage(ACTOR_DOOR_SCHOLAR, 0);
  Engine_EventEnd();
}

/// Stepping onto the hole in the floor: the leader looks down, falls through
/// and lands in the room below.
static void Event_FallThroughFloor(void) {
  struct FieldActor *leader;
  struct FieldActor *leaderActor;
  s32 zero;
#if TBS_PATCH_3
  u8 *motionFlags;
#endif

#if !TBS_PATCH_3
  zero = 0;
#endif
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
#if TBS_PATCH_3
  motionFlags = (u8 *)&leader->motionFlags;
  zero = 0;
  *motionFlags = zero;
#endif
  Camera_MoveTo(-1, -1, -1, 0);
  leaderActor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leaderActor, 0);
  {
    u16 facing = FACING_SOUTH;

    leader->facing = facing;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x30000, 0x18000);
  Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 0x228);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  Engine_EventWait(30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
  Object_SetActionById(ACTOR_PARTY_LEADER, 24);
  Engine_EventWait(40);
  // Start the fall only after the approach and descent animation have finished.
  leader->gravity = 0x9999;
  {
    s32 z = leader->z.fixed + PIXELS(72);

    leader->bounce = zero;
    VenusLighthouse_CreateBlendedPassageEffect(leader->x.fixed, 0, z, 223);
  }
  Map_CopyCellAttributes(VENUS_DESCENT_COLLISION_X, 35,
                         VENUS_DESCENT_COLLISION_WIDTH, 1,
                         VENUS_DESCENT_COLLISION_X, 34);
#if TBS_PATCH_3
  *motionFlags = 3;
#endif
  OverlayObject_WaitUntilIdle(0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_EventRequestExit(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Move on one of the three blocks in a row: the floor under each block is
/// solid again and the nearer block draws in front.
static void Event_MoveDepthBlock(void) {

  u32 blockIndex;
  struct FieldActor *block;
  s32 destinationColumn;
  s32 destinationRow;
  s32 column;
  s32 row;

  Object_GetById(8);
  Engine_EventBegin();
  destinationColumn = 12;
  destinationRow = 44;
  Map_CopyCellAttributes(19, 44, 4, 1, destinationColumn, destinationRow);
  destinationColumn = 11;
  destinationRow = 51;
  Map_CopyCellAttributes(17, 51, 2, 2, destinationColumn, destinationRow);
  blockIndex = 0;
  do {
    block = Object_GetById(blockIndex + 8);
    column = block->x.fixed >> 20;
    row = block->z.fixed >> 20;
    Map_CopyCellAttributes(12, 50, 1, 1, column, row);
    blockIndex++;
  } while (blockIndex <= 2);
  SceneActor_SwapPositionsByDepth(10, 9);
  Engine_EventEnd();
}

/// Pushing one of the three blocks in a row.
static void Event_PushDepthBlock(void) {

  Engine_EventBegin();
  Map_CopyCellAttributes(19, 44, 4, 1, 12, 44);
  VenusLighthouse_PushAdjacentBlock();
  Event_MoveDepthBlock();
  Engine_EventEnd();
}

/// Move or Carry starting on one of the two blocks by the gap: their tiles
/// are marked before they move.
static void Event_LiftCarryBlock(void) {
  u32 blockIndex;
  struct FieldActor *block;

  blockIndex = 0;
  do {
    block = Object_GetById(blockIndex + 11);
    blockIndex++;
    SetMapCellCollision(0, block->x.fixed, block->z.fixed, 45);
  } while (blockIndex <= 1);
}

/// Move on one of the two blocks by the gap: blocks still above the floor are
/// solid again.
static void Event_MoveCarryBlock(void) {
  u32 blockIndex;
  struct FieldActor *block;

  blockIndex = 0;
  do {
    block = Object_GetById(blockIndex + 11);
    if (block->y.fixed > -0x100000) {
      SetMapCellCollision(0, block->x.fixed, block->z.fixed, 255);
    }
    blockIndex++;
  } while (blockIndex <= 1);
}

/// Walking into a gap or a block by it: jump the gap, else push the block.
static void Event_JumpOrPushCarryBlock(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Event_LiftCarryBlock();
    VenusLighthouse_PushAdjacentBlock();
    Event_MoveCarryBlock();
  }
  Engine_EventEnd();
}

/// Fixes a block at its destination and restores the cell beneath it.
static void VenusLighthouse_FixBlockAtDestination(struct FieldActor *block) {
  s32 column;
  s32 row;

  block->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  block->motionFlags = 0;
  column = block->x.fixed >> 20;
  row = block->z.fixed >> 20;
  Map_CopyCellAttributes(9, 24, 1, 1, column, row);
}

/// Removes a fallen Carry block, or restores collision under the remaining
/// blocks.
static void VenusLighthouse_ReleaseFallenCarryBlock(struct FieldActor *block) {
  struct AnimationObject *sprite;
  s32 zero;
  struct FieldActor *passageEffect;

  sprite = block->sprite;
  if (sprite->image.priority == 3) {
    sprite->image.priority = 1;
    zero = 0;
    block->bounce = zero;
    passageEffect = VenusLighthouse_CreateBlendedPassageEffect(
        block->x.fixed, 0, 0x2000000, 223);
    OverlayObject_WaitUntilIdle(block);
    block->x.fixed = zero;
    block->z.fixed = zero;
    Engine_ObjectDispatchRelease(passageEffect);
  } else {
    Event_MoveCarryBlock();
  }
}

/// Carry on one of the two blocks: set down in its place it stays fixed;
/// dropped into the gap it falls through the floor.
static void Event_CarryBlockAcrossGap(void) {
  struct FieldActor *block;

  Engine_EventBegin();
  block = Object_GetById(11);
  if (block->x.fixed >> 20 == 8) {
    OverlayObject_WaitUntilIdle(block);
    VenusLighthouse_FixBlockAtDestination(block);
  } else {
    VenusLighthouse_ReleaseFallenCarryBlock(block);
  }
  block = Object_GetById(12);
  if (block->x.fixed >> 20 == 7) {
    OverlayObject_WaitUntilIdle(block);
    VenusLighthouse_FixBlockAtDestination(block);
  } else {
    VenusLighthouse_ReleaseFallenCarryBlock(block);
  }
  Engine_EventEnd();
}

/// Stepping on the floor switch: the wall slides aside in a cloud of dust, the
/// first time only, and the party goes on through the next exit.
static void Event_StepOnWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 308 && column <= 315 && leaderZ >= 532 && leaderZ < 540) {
    leader->y.fixed = -PIXELS(2);
    if (!GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
      Engine_EventBegin();
      Engine_AudioPlayCue(SE_161);
      GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED);
      Map_CopyCellsTo(26, 33, 19, 33, 1, 1);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_239);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_EventWait(20);
      dustX = PIXELS(288);
      column = 29;
      tileDelay = 40;
      // Scroll the layer while replacing the wall edge and emitting dust along
      // it.
      for (frame = 0; frame <= 479; frame++, tileDelay--) {
        scrollLayer->offsetX += 0x3333;
        dustX += -0x3333;
        options.priority = 2;
        options.startScaleX =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.startScaleY =
            ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(dustX, 0, 0x2100000, 0, -(((gFrameCount & 1) * 3) << 16),
                     0, 0x8a0000, &options);
        if (frame == 240) {
          dustX += -PIXELS(48);
        }
        if (tileDelay == 0) {
          tileDelay = 40;
          if (frame <= 240) {
            column -= 4;
            Map_CopyCellsTo(column, 50, 15, 32, 3, 4);
          } else {
            column += 4;
            Map_CopyCellsTo(column, 45, 9, 32, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
      Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(11);
      Engine_EventEnd();
    }
  }
}

/// Move on the block before the gate: in its socket it sinks in and the gate
/// opens, the first time only.
static void Event_MoveGateBlock(void) {
  u8 *flags;
  s32 row;
  s32 column;
  s32 gateOpened;
  struct FieldActor *leader;
  struct FieldActor *block;
  struct FieldActor *blockActor;
  struct FieldActor *occlusionEffect;

  occlusionEffect = 0;
  block = Object_GetById(9);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
  flags = &block->priorityFlags;
  if ((2 & block->priorityFlags) != 0) {
    Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
  } else {
    Map_CopyCellAttributes(48, 42, 1, 1, block->x.fixed >> 20,
                           block->z.fixed >> 20);
  }
  column = block->x.fixed >> 20;
  if (column != 46) {
  } else {
    row = block->z.fixed >> 20;
    if (row != 45) {
    } else {
      // Sink an unsolved block only when both of its target cell coordinates
      // match.
      gateOpened = Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      if (gateOpened != 0) {
      } else {
        if ((leader->z.fixed >> 20) <= 45) {
          occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
              0x2e80000, 0, 0x2c00000, 20);
          Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
        }
        blockActor = Object_GetById(9);
        Engine_ActorSetSpriteFlags(blockActor, 0);
        block->terrainId = gateOpened;
        block->motionFlags = 3;
        block->gravity = 0x1999;
        block->bounce = gateOpened;
        Map_CopyCellAttributes(43, 45, 1, 1, column, row);
        OverlayObject_WaitUntilIdle(block);
        Engine_AudioPlayCue(SE_188);
        block->motionFlags = gateOpened;
        block->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(9, 3);
        {
          u8 two = 2;

          *flags = two;
        }
        Map_CopyCellAttributes(45, 45, 1, 1, column, row);
        Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
        Engine_ObjectDispatchRelease(occlusionEffect);
        block->collisionFlags = gateOpened;
        *flags |= 2;
        Object_GetById(10)->collisionFlags = gateOpened;
        Object_GetById(10)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetPosition(10, PIXELS(808), PIXELS(728));
        Actor_SetActionScript(10, gVenusPushScript);
        Object_RefreshSelectorById(10);
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusPushCells, 110, 41);
        Map_CopyCellAttributes(46, 41, 1, 1, column, 42);
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN);
      }
    }
  }
  Engine_EventEnd();
}

/// Walking into the gap or the block before the gate: jump the gap, else push
/// the block, which may sink into its socket.
static void Event_JumpOrPushGateBlock(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(109, 43, 7, 5, 45, 43);
    VenusLighthouse_PushAdjacentBlock();
  }
  Engine_EventEnd();
  Event_MoveGateBlock();
}

/// Move on one of the three blocks: a block on a free socket sinks in, and
/// with all three sunk the gate opens.
static void Event_MoveSwitchBlock(void) {
  struct FieldActor *occlusionEffect = 0;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *block;
  struct FieldActor *other;
  struct SwitchEffect *firstGateEffect;
  struct SwitchEffect *secondGateEffect;
  u8 *flags;
  u32 blockId;
  u32 candidateIndex;
  u32 switchIndex;
  s32 savedPriority;

  Engine_EventBegin();
  Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
  // Find blocks on unoccupied switch cells before committing any sinking
  // motion.
  for (blockId = 9; blockId <= 11; blockId++) {
    block = Object_GetById(blockId);
    flags = &block->priorityFlags;
    if (*flags != 2) {
      Map_CopyCellAttributes(47, 39, 1, 1, block->x.fixed >> 20,
                             block->z.fixed >> 20);
    } else {
      Map_CopyCellAttributes(46, 39, 1, 1, block->x.fixed >> 20,
                             block->z.fixed >> 20);
    }
    switchIndex = 5;
    for (candidateIndex = 0; candidateIndex <= 3; candidateIndex++) {
      if (block->x.fixed >> 20 == gVenusSwitchCells[candidateIndex].x &&
          block->z.fixed >> 20 == gVenusSwitchCells[candidateIndex].z &&
          block->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    for (candidateIndex = 9; candidateIndex <= 11; candidateIndex++) {
      other = Object_GetById(candidateIndex);
      if (blockId != candidateIndex &&
          block->x.fixed >> 20 == other->x.fixed >> 20 &&
          block->z.fixed >> 20 == other->z.fixed >> 20) {
        switchIndex = 5;
        break;
      }
    }
    if (switchIndex == 5) {
      continue;
    }
    // Preserve overlapping actors while the selected block sinks into its
    // socket.
    savedPriority = leader->sprite->image.priority;
    if (leader->z.fixed >> 20 <= gVenusSwitchCells[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          block->x.fixed, block->y.fixed, block->z.fixed - 0x40000, 20);
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(blockId), 0);
    block->terrainId = 0;
    block->motionFlags = 3;
    block->bounce = 0;
    block->gravity = 0x1999;
    Map_CopyCellAttributes(42, 41, 1, 1, gVenusSwitchCells[switchIndex].x,
                           gVenusSwitchCells[switchIndex].z);
    OverlayObject_WaitUntilIdle(block);
    Engine_AudioPlayCue(SE_188);
    block->collisionFlags = 0;
    block->motionFlags = 0;
    block->y.fixed = -PIXELS(16);
    Engine_ActorSetSpritePriority(blockId, 3);
    {
      u8 two = 2;

      *flags = two;
    }
    Map_CopyCellAttributes(46, 39, 1, 1, gVenusSwitchCells[switchIndex].x,
                           gVenusSwitchCells[switchIndex].z);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    if (occlusionEffect != 0) {
      Engine_ObjectDispatchRelease(occlusionEffect);
    }
    if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_BLOCK_GATE_OPEN)) {
      Engine_EventEnd();
      return;
    }
    if ((Object_GetById(9)->priorityFlags & Object_GetById(10)->priorityFlags &
         Object_GetById(11)->priorityFlags & 2) == 0) {
      continue;
    }
    // Once all three blocks settle, run the paired scripts that open the gate.
    firstGateEffect =
        (struct SwitchEffect *)VenusLighthouse_CreateScriptedGateEffect(
            888, 680, gVenusSettleScriptA);
    secondGateEffect =
        (struct SwitchEffect *)VenusLighthouse_CreateScriptedGateEffect(
            888, 680, gVenusSettleScriptB);
    while (firstGateEffect->active != 0 || secondGateEffect->active != 0) {
      if (firstGateEffect->finished != 0 || secondGateEffect->finished != 0) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_DOOR_OPEN);
        Engine_MapAnimateCells(gVenusSettleCells, 109, 37);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Object_GetById(9)->x.fixed >> 20 == gVenusSwitchCells[0].x &&
            Object_GetById(9)->z.fixed >> 20 == gVenusSwitchCells[0].z) {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A);
        } else {
          Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_B);
        }
        Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_THREE_BLOCK_GATE_OPEN);
        break;
      }
      Engine_TaskWait(1);
    }
  }
  Engine_EventEnd();
}

/// Walking into the gap or one of the three blocks: jump the gap, else push the
/// block, which may sink into a socket.
static void Event_JumpOrPushSwitchBlock(void) {
  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    Map_CopyCellAttributes(108, 39, 13, 7, 44, 39);
    VenusLighthouse_PushAdjacentBlock();
  }
  Engine_EventEnd();
  Event_MoveSwitchBlock();
}

// Keep a stacked block sixteen pixels above its supporting actor.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 7 bytes (extent 34 to 32 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void
VenusLighthouse_FollowSupportingBlockHeight(union FieldObject *fieldObject) {
#else
static s32
VenusLighthouse_FollowSupportingBlockHeight(union FieldObject *fieldObject) {
#endif
  struct FieldActor *block = (struct FieldActor *)fieldObject;
  struct FieldActor *support;

  support = Object_GetById(block->timer);
  block->y.fixed = support->y.fixed + PIXELS(16);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Moves the floating supports to the heights selected by their timers.
static void VenusLighthouse_LowerFloatingBlocks(s32 waitFrames) {
  struct FieldActor *westSupport = Object_GetById(8);
  struct FieldActor *eastSupport = Object_GetById(9);
  u32 blockIndex;

  Actor_SetSpeed(8, 0x8000, 0x4000);
  Engine_ActorSetSpeed(9, 0x8000, 0x4000);
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_180);
  }
  FieldObject_SetPosition((s32)westSupport, westSupport->x.fixed,
                          gVenusBlockHeights[westSupport->timer],
                          westSupport->z.fixed);
  FieldObject_SetPosition((s32)eastSupport, eastSupport->x.fixed,
                          gVenusBlockHeights[eastSupport->timer],
                          eastSupport->z.fixed);
  Engine_ActorWaitForMove(8);
  Engine_ActorWaitForMove(9);
  westSupport->y.fixed = gVenusBlockHeights[westSupport->timer];
  eastSupport->y.fixed = gVenusBlockHeights[eastSupport->timer];
  if (waitFrames != 0) {
    Engine_AudioPlayCue(SE_289);
  }
  for (blockIndex = 0; blockIndex < 5; blockIndex++) {
    struct FieldActor *actor = Object_GetById(blockIndex + 8);

    if (actor->y.fixed / 0x10000 < 0 && actor->y.fixed / 0x10000 > -30) {
      Map_CopyCellAttributes(4, 19, 1, 1, actor->x.fixed >> 20,
                             actor->z.fixed >> 20);
    }
  }
  Engine_EventWait(waitFrames);
}

/// Moves a floating block atop the highest support in its map cell.
static void VenusLighthouse_StackFloatingBlock(s32 blockId) {

  struct FieldActor *candidate;
  s32 supportHeight = 0xffb00000;
  struct FieldActor *block = 0;
  u32 candidateIndex;

  for (candidateIndex = 0; candidateIndex <= 5; candidateIndex++) {
    s32 candidateId = candidateIndex + 8;
    s32 candidateTop;

    if (candidateId == blockId) {
      continue;
    }

    candidate = Object_GetById(candidateId);
    block = Object_GetById(blockId);

    if ((candidate->x.fixed >> 20) != (block->x.fixed >> 20) ||
        (candidate->z.fixed >> 20) != (block->z.fixed >> 20)) {
      continue;
    }

    candidateTop = candidate->y.fixed + PIXELS(16);
    if (supportHeight <= candidateTop) {
      block->timer = candidateId;
      supportHeight = candidateTop;
    }
  }

  Engine_ActorSetSpeed(blockId, 0x40000, 0x20000);
  Engine_ObjectSetPosition(block, block->x.fixed, supportHeight,
                           block->z.fixed);
  Engine_ActorWaitForMove(blockId);
  Engine_AudioPlayCue(SE_188);
  SceneEffect_SpawnNineRadialEffects(blockId);
  Engine_EventWait(30);
}

/// Stops a floating block and resets its vertical motion and timer.
static __inline__ void FloatingBlock_Settle(struct FieldActor *block,
                                            s32 resetValue) {
  block->targetY = ACTOR_NO_TARGET;
  block->terrainHeight = resetValue;
  block->velocityY = resetValue;
  block->motionFlags = resetValue;
  block->timer = resetValue;
}

/// Copies all three fixed-point position components without changing motion.
static __inline__ void Actor_CopyPosition(struct FieldActor *destination,
                                          struct FieldActor *source) {
  destination->x.fixed = source->x.fixed;
  destination->y.fixed = source->y.fixed;
  destination->z.fixed = source->z.fixed;
}

/// Carry on one of the four floating blocks: it settles where it is set down
/// and the platforms under the blocks rise or sink to their weight.
static void Event_CarryFloatingBlock(void) {
  struct FieldActor savedPosition;
  struct FieldActor *firstPassageEffect = NULL;
  struct FieldActor *secondPassageEffect = NULL;
  struct FieldActor *block;
  struct FieldActor *other;
  s32 blockIndex;
  s32 earlierIndex;
  s32 settledIndex;
  s32 column;
  s32 row;

  Engine_EventBegin();
  // Handle completed destinations and fallen blocks before reordering a new
  // stack.
  for (blockIndex = 0; blockIndex < 4; blockIndex++) {
    block = Object_GetById(blockIndex + 10);
    column = block->x.fixed >> 20;
    if (column == 13 && (row = block->z.fixed >> 20) == 7 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + blockIndex)) {
      OverlayObject_WaitUntilIdle(block);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + blockIndex);
      block->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      block->collisionFlags = 0;
      block->motionFlags = 0;
      Map_CopyCellAttributes(4, 19, 1, 1, column, row);
      break;
    }
    if (block->sprite->image.priority == 3 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + blockIndex)) {
      /* FAKEMATCH: forced temporary: the search counter's zero, set before the
       * priority call, puts movs r5, #0 after the call; as block->bounce = 0
       * after the call, 6 instructions differ in tbs-en. */
      earlierIndex = 0;
      Engine_ActorSetSpritePriority(blockIndex + 10, 1);
      block->bounce = earlierIndex;
      if (block->z.fixed >> 20 <= 12) {
        firstPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            block->x.fixed, 0, 0xe00000, 253);
        secondPassageEffect = VenusLighthouse_CreateBlendedPassageEffect(
            block->x.fixed, 0, 0xf00000, 253);
      }
      OverlayObject_WaitUntilIdle(block);
      Engine_ActorSetPosition(blockIndex + 10, 0, 0);
      Engine_ObjectDispatchRelease(firstPassageEffect);
      Engine_ObjectDispatchRelease(secondPassageEffect);
      GameFlag_Set(FLAG_SCENE_TEMP_00 + blockIndex);
      break;
    }
    if (block->z.fixed >> 20 == 19 &&
        !GameFlag_IsSet(FLAG_SCENE_TEMP_00 + blockIndex)) {
      // Keep settled blocks at the front of the actor slots before finding
      // their support.
      FloatingBlock_Settle(block, 0);
      settledIndex = blockIndex;
      for (earlierIndex = 0; earlierIndex < blockIndex; earlierIndex++) {
        if (!GameFlag_IsSet(FLAG_SCENE_TEMP_00 + earlierIndex)) {
          other = Object_GetById(earlierIndex + 10);
          Actor_CopyPosition(&savedPosition, block);
          Actor_CopyPosition(block, other);
          Actor_CopyPosition(other, &savedPosition);
          settledIndex = earlierIndex;
          break;
        }
      }
      other = Object_GetById(settledIndex + 10);
      FloatingBlock_Settle(other, 0);
      Engine_CameraSetSpeed(0x30000, 0x6000);
      Camera_GetEventObject()->motionFlags = 0;
      Engine_CameraMoveTo(PIXELS(136), PIXELS(8), PIXELS(344), 1);
      Engine_CameraWaitForMove();
      // Stack the block, then rebalance the two supports and attach the height
      // follower.
      VenusLighthouse_StackFloatingBlock(settledIndex + 10);
      if (Object_GetById(settledIndex + 10)->x.fixed >> 20 == 6) {
        Object_GetById(8)->timer++;
        Object_GetById(9)->timer--;
      } else {
        Object_GetById(8)->timer--;
        Object_GetById(9)->timer++;
      }
#ifdef UBFIX
      Object_GetById(settledIndex + 10)->update =
          VenusLighthouse_FollowSupportingBlockHeight;
#else
      Object_GetById(settledIndex + 10)->update = (void (*)(
          union FieldObject *))VenusLighthouse_FollowSupportingBlockHeight;
#endif
      VenusLighthouse_LowerFloatingBlocks(40);
      Object_GetById(settledIndex + 10)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  Engine_EventEnd();
}

/// Pushing a block in the pillar room.
static void Event_PushPuzzleBlock(void) {

  Engine_EventBegin();
  VenusLighthouse_PushAdjacentBlock();
  Engine_EventEnd();
}

/// Stepping on the west floor switch: the west wall slides aside, the first
/// time only, and the party goes on through the next exit.
static void Event_StepOnWestWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 532 && column <= 539 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Map_CopyCellsTo(44, 83, 44, 80, 3, 3);
      Engine_EventWait(30);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(616);
      column = 0;
      tileDelay = 60;
      // Move the west wall while its visible edge exposes successive tile
      // strips.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX += 0x3333;
        if (dustX >= 0x2400000 && frame > 40) {
          dustX += -0x3333;
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          Effect_Spawn(dustX, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16),
                       0, 0x8a0000, &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column += 4;
            Map_CopyCellsTo(column, 56, 36, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(18);
      Engine_EventEnd();
    }
  }
}

/// Stepping on the east floor switch: the east wall slides aside, the first
/// time only, and the party goes on through the next exit.
static void Event_StepOnEastWallSwitch(void) {
  struct EffectOptions options;
  struct MapLayerScroll *scrollLayer;
  struct FieldActor *leader;
  s32 column;
  s32 leaderZ;
  s32 tileDelay;
  u32 frame;
  s32 dustX;

  scrollLayer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  column = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  leader->y.fixed = 0;
  if (column >= 788 && column <= 795 && leaderZ >= 324 && leaderZ < 332) {
    leader->y.fixed = -PIXELS(2);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
      Engine_EventBegin();
      Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
      Engine_AudioPlayCue(SE_161);
      Engine_EventWait(30);
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_AudioPlayCue(SE_239);
      Engine_EventWait(20);
      dustX = PIXELS(704);
      column = 61;
      tileDelay = 60;
      // Move the east wall in the opposite direction, keeping its distinct dust
      // limits.
      for (frame = 0; frame <= 319; frame++, tileDelay--) {
        scrollLayer->offsetX += -0x3333;
        dustX += 0x3333;
        if (dustX >= 0x2c80000 && dustX < 0x2f00000) {
          options.priority = 2;
          options.startScaleX =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.startScaleY =
              ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
          options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
          Effect_Spawn(dustX, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16),
                       0, 0x8a0000, &options);
          if (tileDelay == 0) {
            tileDelay = 40;
            column -= 4;
            Map_CopyCellsTo(column, 56, 44, 17, 3, 4);
          }
        }
        Engine_TaskWait(1);
      }
      scrollLayer->offsetX += 0x8000;
      scrollLayer->offsetX = scrollLayer->offsetX / 0x10000 << 16;
      Engine_AudioPlayCue(SE_288);
      Engine_AudioPlayCue(SE_188);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Engine_EventRequestExit(19);
      Engine_EventEnd();
    }
  }
}

/// Move on one of the four blocks in the pillar room: on a free socket it sinks
/// in, and with all four sunk the gate opens.
static void Event_MovePillarSwitchBlock(void) {
  struct FieldActor *occlusionEffect;
  struct FieldActor *block;
  struct FieldActor *otherBlock;
  struct FieldActor *leader;
  struct FieldActor *firstBlock;
  struct FieldActor *secondBlock;
  struct FieldActor *thirdBlock;
  struct FieldActor *fourthBlock;
  u8 *flags;
  u8 *motionFlags;
  u32 blockId;
  u32 candidateIndex;
  u32 switchIndex;
  u32 savedPriority;

  occlusionEffect = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Begin();
#if TBS_PATCH_3
  Battle_ResetEffectCounter();
#endif
  Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
  Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
  // Find a block on a clear pillar socket and preserve overlapping draw order.
  for (blockId = 15; blockId <= 18; blockId++) {
    block = Object_GetById(blockId);
    flags = &block->priorityFlags;
    if (*flags != 2)
      Map_CopyCellAttributes(72, 48, 1, 1, block->x.fixed >> 20,
                             block->z.fixed >> 20);
    else
      Map_CopyCellAttributes(73, 48, 1, 1, block->x.fixed >> 20,
                             block->z.fixed >> 20);

    switchIndex = 8;
    for (candidateIndex = 0; candidateIndex < 8; candidateIndex++) {
      if ((block->x.fixed >> 20) == gVenusPillarCells[candidateIndex].x &&
          (block->z.fixed >> 20) == gVenusPillarCells[candidateIndex].z &&
          block->y.fixed >= 0) {
        switchIndex = candidateIndex;
        break;
      }
    }
    if (switchIndex == 8)
      continue;
    for (candidateIndex = 15; candidateIndex <= 18; candidateIndex++) {
      otherBlock = Object_GetById(candidateIndex);
      if (blockId != candidateIndex &&
          (block->x.fixed >> 20) == (otherBlock->x.fixed >> 20) &&
          (block->z.fixed >> 20) == (otherBlock->z.fixed >> 20)) {
        switchIndex = 8;
        break;
      }
    }
    if (switchIndex == 8)
      continue;

    savedPriority = leader->sprite->image.priority;
    if ((leader->z.fixed >> 20) <= gVenusPillarCells[switchIndex].z) {
      occlusionEffect = VenusLighthouse_CreateOccludingPassageEffect(
          block->x.fixed, block->y.fixed, block->z.fixed - 0x40000, 20);
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    }
    for (candidateIndex = 15; candidateIndex <= 18; candidateIndex++) {
      otherBlock = Object_GetById(candidateIndex);
      if (blockId != candidateIndex &&
          (block->x.fixed >> 20) == (otherBlock->x.fixed >> 20) &&
          (block->z.fixed >> 20) - 1 == (otherBlock->z.fixed >> 20))
        Actor_SetSpritePriority(candidateIndex, 3);
    }
    Actor_SetSpriteFlags(Object_GetById(blockId), 0);
    block->terrainId = 0;
    motionFlags = &block->motionFlags;
    *motionFlags = 3;
    ((union FieldObject *)block)->effect.velocity.y = 0x1999;
    ((union FieldObject *)block)->effect.velocity.x = 0;
    Map_CopyCellAttributes(6, 44, 1, 1, gVenusPillarCells[switchIndex].x,
                           gVenusPillarCells[switchIndex].z);
    OverlayObject_WaitUntilIdle(block);
    Event_PlayCue(SE_188);
    block->collisionFlags = 0;
    *motionFlags = 0;
    block->y.fixed = -PIXELS(16);
    Actor_SetSpritePriority(blockId, 3);
    *flags = 2;
    Map_CopyCellAttributes(73, 48, 1, 1, gVenusPillarCells[switchIndex].x,
                           gVenusPillarCells[switchIndex].z);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, savedPriority);
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
    for (candidateIndex = 15; candidateIndex <= 18; candidateIndex++) {
      otherBlock = Object_GetById(candidateIndex);
      if (blockId != candidateIndex &&
          (block->x.fixed >> 20) == (otherBlock->x.fixed >> 20) &&
          (block->z.fixed >> 20) - 1 == (otherBlock->z.fixed >> 20)) {
        Actor_SetSpritePriority(candidateIndex, 1);
        Object_GetById(candidateIndex)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
      }
    }
    Engine_ObjectDispatchRelease(occlusionEffect);
    if (GameFlag_IsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
      Event_End();
      return;
    }
    // Open the paired gate only after all four blocks are seated.
    firstBlock = Actor_Get(15);
    secondBlock = Actor_Get(16);
    thirdBlock = Actor_Get(17);
    fourthBlock = Object_GetById(18);
    if ((firstBlock->priorityFlags & secondBlock->priorityFlags &
         thirdBlock->priorityFlags & fourthBlock->priorityFlags) &
        2) {
      struct SwitchEffect *firstGateEffect;
      struct SwitchEffect *secondGateEffect;

      Camera_SetSpeed(0x10000, 0x2000);
      Object_PlaceCurrentWithinCameraBounds(14, 1);
      Camera_WaitForMove();
      firstGateEffect =
          (struct SwitchEffect *)VenusLighthouse_CreateScriptedGateEffect(
              136, FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN,
              gVenusFirstGateScript);
      Event_Wait(30);
      Camera_SetSpeed(0x6666, 0xccc);
      Camera_MoveTo(PIXELS(216), -1, PIXELS(632), 1);
      ObjectDispatch_WaitForValue16(firstGateEffect);
      Object_SetScript((struct FieldActor *)firstGateEffect,
                       gVenusFirstGateOpenScript);
      secondGateEffect =
          (struct SwitchEffect *)VenusLighthouse_CreateScriptedGateEffect(
              216, 0x2f8, gVenusSecondGateScript);
      while (firstGateEffect->active != 0 || secondGateEffect->active != 0) {
        if (firstGateEffect->finished != 0 || secondGateEffect->finished != 0) {
          Event_Wait(30);
          Map_AnimateCells(gVenusGateCells, 77, 35);
          Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
          GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN);
          break;
        }
        Task_Wait(1);
      }
    }
  }
  Event_End();
}

/// Walking into the gap or a pillar-room block: jump the gap, else push the
/// block, which may sink into a socket.
static void Event_JumpOrPushPillarBlock(void) {

  Engine_EventBegin();
  if (Event_JumpAcrossGap() == 0) {
    {
      Map_CopyCellAttributes(69, 48, 4, 2, 5, 48);
    }
    {
      Map_CopyCellAttributes(73, 37, 9, 13, 9, 37);
    }
    VenusLighthouse_PushAdjacentBlock();
  }
  Engine_EventEnd();
  Event_MovePillarSwitchBlock();
}

/// Walking south into the passage: the leader rises out of the room to the
/// floor above.
static void Event_EnterRisingPassage(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x208, 0x2c8);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  (void)VenusLighthouse_CreateBlendedPassageEffect(PIXELS(520), 0, PIXELS(784),
                                                   223);
  FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
  Engine_EventWait(60);
  Engine_EventRequestExit(20);
  Engine_EventEnd();
}

/// Selects the puzzle interactions and dialogue for the current lighthouse
/// scene.
const struct SceneEvent *Scene_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VenusLighthouse1) {
    return gVenusLighthouseEvents1;
  }
  if (scene == (s32)&SceneId_VenusLighthouse2) {
    return gVenusLighthouseEvents2;
  }
  if (scene == (s32)&SceneId_VenusLighthouse3) {
    return gVenusLighthouseEvents3;
  }
  if (scene == (s32)&SceneId_VenusLighthouse4) {
    return gVenusLighthouseEvents4;
  }
  if (scene == (s32)&SceneId_VenusLighthouse5) {
    return gVenusLighthouseEvents5;
  }
  if (scene == (s32)&SceneId_VenusLighthouse6) {
    return gVenusLighthouseEvents6;
  }
  return gVenusLighthouseEvents2;
}

/// Prepares a stationary actor with the room's initial collision and draw
/// state.
static void VenusLighthouse_PrepareStandingActor(s32 actorId) {
  struct FieldActor *actor;
  s32 collisionFlags;

  actor = Object_GetById(actorId);
  actor->motionFlags = 0;
  collisionFlags = 252;
  collisionFlags &= actor->collisionFlags;
  actor->collisionFlags = collisionFlags;
  Engine_ActorSetSpriteFlags(actor, 0);
  Object_SetMode(actor, 5);
  Engine_ActorSetSpritePriority(actorId, 3);
  {
    s32 priorityFlags = 2;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
  }
}

/// Visits the four pillar actors through the object lookup service.
static void VenusLighthouse_VisitPillarActors(void) {
  u32 actorId;

  actorId = 15;
  do {
    Object_GetById(actorId);
    actorId++;
  } while (actorId <= 18);
}

/// Restores puzzle progress, actors and entrance effects for all six scenes.
s32 Scene_RunEntrySetup(void) {
  struct FieldActor *actor;
  s32 scene;
  u32 blockIndex;
  s32 column;
  s32 row;

  Engine_EventWait(1);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
    VenusLighthouse_RestoreScriptedChildValues();
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_LIT);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gGameState.scene;
  // Restore the outdoor scholars and each interior puzzle by its entrance.
  if (scene == (s32)&SceneId_VenusLighthouse1) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD))
      VenusLighthouse_PrepareStandingActor(8);
    else
      Map_CopyCellAttributes(7, 17, 2, 1, 7, 16);
    VenusLighthouse_PrepareStandingActor(9);
    VenusLighthouse_PrepareStandingActor(10);
    VenusLighthouse_PrepareStandingActor(11);
    Engine_ActorSetSpritePriority(11, 2);
    VenusLighthouse_PrepareStandingActor(12);
    Engine_ActorSetSpritePriority(12, 2);
    VenusLighthouse_PrepareStandingActor(13);
    VenusLighthouse_PrepareStandingActor(14);
  } else if (scene == (s32)&SceneId_VenusLighthouse2) {
    switch (gGameState.entrance) {
    case 1:
    case 2:
      VenusLighthouse_PrepareStandingActor(8);
      break;
    case 5:
      Engine_GameFlagClear(0x120);
    case 3:
    case 4:
    case 6:
      VenusLighthouse_PrepareStandingActor(9);
      break;
    case 20:
    case 21:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
      } else {
        if (!Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
      }
      break;
    case 26:
      Engine_GameFlagClear(0x121);
    case 22:
    case 23:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
        Map_CopyCellsTo(44, 117, 41, 117, 3, 5);
      break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_BLOCK_CARRIED)) {
        Actor_SetPosition(12, PIXELS(872), PIXELS(88));
        actor = Object_GetById(12);
        actor->y.fixed = -PIXELS(24);
        actor->targetY = ACTOR_NO_TARGET;
      }
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouse3) {
    // Restore depth blocks, the Carry puzzle and the paired tile-wave passage.
    switch (gGameState.entrance) {
    case 16:
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      break;
    case 21:
      VenusLighthouse_AnimatePairedTileWaves();
      break;
    case 20:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
        VenusLighthouse_LandWithRadialDust();
      break;
    case 9:
    case 10:
      actor = Object_GetById(11);
      Event_MoveCarryBlock();
      if (actor->x.fixed >> 20 == 8)
        VenusLighthouse_FixBlockAtDestination(actor);
      actor = Object_GetById(12);
      if (actor->x.fixed >> 20 == 7)
        VenusLighthouse_FixBlockAtDestination(actor);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(206), 0, PIXELS(449),
                                                 223);
      VenusLighthouse_CreateBlendedPassageEffect(PIXELS(210), 0, PIXELS(449),
                                                 223);
      break;
    case 7:
    case 8:
      BattleFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      actor = Object_GetById(8);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(9);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      actor = Object_GetById(10);
      actor->motionFlags = 0;
      actor->update = SceneActor_SetFlagBitByRelativeDepth;
      Event_MoveDepthBlock();
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      BattleFx_StartFadeOverlay(0);
      Engine_TaskWait(2);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
        Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
        Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
        Map_CopyCells(48, 3, 3, 10, 54, 3);
        Map_CopyCells(55, 26, 3, 10, 48, 3);
      }
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 19:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouse4) {
    // Restore the first sliding wall and its sunken-block gate.
    switch (gGameState.entrance) {
    case 2:
      VenusLighthouse_ArriveThroughSparkPassage();
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case 4:
    case 6:
      BattleFx_StartFadeOverlay(0);
      break;
    case 9:
    case 10:
      actor = Object_GetById(8);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      actor = Object_GetById(9);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(124, 41, 110, 41, 1, 2);
        Map_CopyCellAttributes(46, 41, 1, 1, 46, 42);
        Actor_SetPosition(9, PIXELS(744), PIXELS(728));
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(16);
        Engine_ActorSetSpritePriority(9, 3);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(45, 45, 1, 1, 46, 45);
        Engine_ActorSetAnimation(10, 7);
        Engine_ActorSetSpritePriority(10, 1);
        actor = Object_GetById(10);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(10, PIXELS(743), PIXELS(696));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MoveGateBlock();
      break;
    case 11:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
    case 7:
    case 8:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      DisplayBlend_SuspendScript();
      {
        s32 zero = 0;

        /* FAKEMATCH: the empty asm gives the zero the refs that put it in r3
         * ahead of the register address; as REG_BLDCNT = 0, 95 instructions
         * differ in tbs-en. */
        asm("" : "+l"(zero));
        /* FAKEMATCH: needed to match: this CSE block keeps the blend
         * store before the next call setup, at the same size and frame. */
        do {
          REG_BLDCNT = zero;
        } while (0);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WALL_MOVED)) {
        Map_CopyCellsTo(15, 96, 9, 96, 3, 3);
        Map_CopyCellsTo(12, 96, 15, 96, 3, 3);
        Map_CopyCellsTo(5, 50, 15, 32, 3, 4);
        Map_CopyCellsTo(25, 45, 9, 32, 3, 4);
        Map_CopyCellAttributes(15, 32, 3, 1, 9, 32);
        Map_CopyCellAttributes(12, 32, 3, 1, 15, 32);
      }
      if (gGameState.entrance != 11)
        break;
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      break;
    case 1:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouse5) {
    // Restore the two tile waves and the three-block socket puzzle.
    switch (gGameState.entrance) {
    case 19:
      VenusLighthouse_AnimateWestTileWave();
      break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 20:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        Map_CopyCells(53, 12, 3, 13, 26, 12);
        Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   0xc80);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCells(58, 12, 3, 13, 34, 12);
        Map_CopyCellsTo(81, 41, 97, 14, 5, 2);
        Engine_TaskWait(1);
        FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles,
                                   0xc80);
      }
      if (gGameState.entrance == 11)
        VenusLighthouse_ArriveThroughSparkPassage();
      else if (gGameState.entrance == 20)
        VenusLighthouse_AnimateEastTileWave();
      break;
    case 4:
    case 5:
      /* FAKEMATCH: a value-returning cast keeps this call from cross-jumping
       * into the identical one before it; as a call by its void prototype, 42
       * instructions differ in tbs-en. */
      VENUS_LIGHTHOUSE_RUN_OPENING_SEQUENCE();
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case 15:
    case 16:
      actor = Object_GetById(8);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(9)->motionFlags = 0;
      Object_GetById(10)->motionFlags = 0;
      Object_GetById(11)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_THREE_BLOCK_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(111, 59, 109, 37, 1, 2);
        Map_CopyCellAttributes(45, 37, 1, 1, 45, 38);
        if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_SOCKETS_A)) {
          Actor_SetPosition(9, PIXELS(776), PIXELS(664));
          Actor_SetPosition(10, PIXELS(840), PIXELS(664));
          Actor_SetPosition(11, PIXELS(776), PIXELS(696));
        } else {
          Actor_SetPosition(9, PIXELS(840), PIXELS(664));
          Actor_SetPosition(10, PIXELS(776), PIXELS(696));
          Actor_SetPosition(11, PIXELS(840), PIXELS(696));
        }
        Engine_ActorSetSpritePriority(9, 3);
        actor = Object_GetById(9);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(10, 3);
        actor = Object_GetById(10);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(11, 3);
        actor = Object_GetById(11);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetAnimation(12, 7);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorSetSpritePriority(12, 1);
        actor = Object_GetById(12);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(12, PIXELS(727), PIXELS(632));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MoveSwitchBlock();
      break;
    case 6:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  } else if (scene == (s32)&SceneId_VenusLighthouse6) {
    // Restore floating supports, opposing walls and the four-block pillar
    // puzzle.
    switch (gGameState.entrance) {
    case 1:
    case 2:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        VenusLighthouse_LowerFloatingBlocks(0);
        OverlayObject_WaitUntilIdle(Object_GetById(ACTOR_PARTY_LEADER));
        for (blockIndex = 0; blockIndex <= 3; blockIndex++) {
          actor = Object_GetById(blockIndex + 10);
          column = actor->x.fixed >> 20;
          if (column == 13) {
            row = actor->z.fixed >> 20;
            if (row == 7 &&
                Engine_GameFlagIsSet(blockIndex + FLAG_SCENE_TEMP_00)) {
              actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
              actor->collisionFlags = 0;
              actor->motionFlags = 0;
              Map_CopyCellAttributes(4, 19, 1, 1, column, row);
              break;
            }
          }
        }
      } else {
        actor = Object_GetById(8);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(8, 1);
        actor = Object_GetById(9);
        actor->motionFlags = 0;
        actor->y.fixed = -PIXELS(48);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= 0xfe;
        actor->timer = 3;
        Engine_ActorSetSpritePriority(9, 1);
        actor = Object_GetById(10);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        actor = Object_GetById(11);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
        actor = Object_GetById(12);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        actor = Object_GetById(13);
        actor->motionFlags = 0;
        actor->timer = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
      }
      break;
    case 13:
    case 14:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case 18:
    case 19:
      gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(2);
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 20:
      Object_GetById(20)->motionFlags = 4;
      Object_GetById(20)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Object_GetById(20)->y.fixed = -0x108000;
      DisplayBlend_SuspendScript();
      Io_Write16(0, &REG_BLDCNT);
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED)) {
        BattleFx_SetQueuedSoundAndPlay(SE_170);
        Map_CopyCellsTo(36, 81, 32, 81, 3, 2);
        Map_CopyCellsTo(36, 83, 36, 81, 3, 2);
        Map_CopyCellAttributes(36, 17, 3, 1, 32, 17);
        Map_CopyCellAttributes(36, 18, 3, 1, 36, 17);
        Map_CopyCellsTo(63, 29, 33, 20, 1, 1);
        Map_CopyCellsTo(20, 56, 36, 17, 3, 4);
      }
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED)) {
        Map_CopyCellsTo(44, 81, 48, 81, 3, 2);
        Map_CopyCellsTo(44, 83, 44, 81, 3, 2);
        Map_CopyCellAttributes(44, 17, 3, 1, 48, 17);
        Map_CopyCellAttributes(44, 18, 3, 1, 44, 17);
        Map_CopyCellsTo(63, 29, 49, 20, 1, 1);
        Map_CopyCellsTo(41, 56, 44, 17, 3, 4);
      }
      if (gGameState.entrance == 18 || gGameState.entrance == 19) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      }
      if (gGameState.entrance == 20)
        VenusLighthouse_RaiseLeaderWithSpray();
      break;
    case 15:
    case 16:
      Engine_TaskWait(1);
      FieldScheduler_AddCallback(VenusLighthouse_VisitPillarActors, 0xc80);
      actor = Object_GetById(14);
      actor->motionFlags = 0;
      actor->y.fixed = 0;
      Object_GetById(15)->motionFlags = 0;
      Object_GetById(16)->motionFlags = 0;
      Object_GetById(17)->motionFlags = 0;
      Object_GetById(18)->motionFlags = 0;
      if (Engine_GameFlagIsSet(FLAG_VENUS_LIGHTHOUSE_PILLAR_GATE_OPEN)) {
        Engine_TaskWait(1);
        Map_CopyCellsTo(95, 56, 77, 35, 1, 2);
        Map_CopyCellAttributes(13, 35, 1, 1, 13, 36);
        Engine_ActorSetPosition(15, PIXELS(264), PIXELS(744));
        actor = Object_GetById(15);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(15, 3);
        Actor_SetPosition(16, PIXELS(184), PIXELS(632));
        actor = Object_GetById(16);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(16, 3);
        Engine_ActorSetPosition(17, PIXELS(232), PIXELS(696));
        actor = Object_GetById(17);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(17, 3);
        Engine_ActorSetPosition(18, PIXELS(184), PIXELS(664));
        actor = Object_GetById(18);
        actor->y.fixed = -PIXELS(16);
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(18, 3);
        Engine_ActorSetAnimation(19, 7);
        Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
        Engine_ActorSetSpritePriority(19, 1);
        actor = Object_GetById(19);
        actor->collisionFlags = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(19, PIXELS(215), PIXELS(600));
#ifdef UBFIX
        actor->update = VenusLighthouse_EmitGateParticles;
#else
        actor->update =
            (void (*)(union FieldObject *))VenusLighthouse_EmitGateParticles;
#endif
      }
      Event_MovePillarSwitchBlock();
      break;
    }
  }
  return 0;
}

/// Standing on this floor carries the party north in a trail of dust; at a
/// ledge they drop down.
static void Event_DriftNorth(void) {
  struct DirXZ args;

  args.x = 0;
  args.z = -1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, south.
static void Event_DriftSouth(void) {
  struct DirXZ args;

  args.x = 0;
  args.z = 1;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, west.
static void Event_DriftWest(void) {
  struct DirXZ args;

  args.x = -1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// As Event_DriftNorth, east.
static void Event_DriftEast(void) {
  struct DirXZ args;

  args.x = 1;
  args.z = 0;
  VenusLighthouse_StepActorWithDust(&args);
}

/// Follows the selected actor through the passage, adjusting for walls and
/// drops.
static void VenusLighthouse_StepActorWithDust(struct DirXZ *direction) {
  s32 actorId;
  struct FieldActor *viewCenter;
  struct FieldActor *actor;
  s32 nextPosition[3];
  struct EffectOptions options;
  s32 collision;
  s32 framePhase;
  s32 particleX;

  {
    struct EventWork *event = gEventWork;

    actorId = gGameState.selectedActor;
    viewCenter = event->viewCenter;
  }
  actor = Object_GetById(actorId);
  SET_VECTOR(
      nextPosition,
      actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15),
      actor->y.fixed,
      actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15));
  collision = FieldObject_CheckMovementCollision(actor, nextPosition);
  framePhase = gFrameCount & 4;
  if (framePhase == 0) {
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    particleX = actor->x.fixed +
                (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
    Effect_Spawn(particleX, actor->y.fixed, actor->z.fixed,
                 (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd) *
                     direction->x,
                 framePhase,
                 direction->z *
                     (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd),
                 0x800000, &options);
  }
  // A drop uses the fall animation; solid terrain instead constrains each step.
  if (collision < 0) {
    Engine_ActorSetAttachedEffect(actorId, EMOTE_IN_FRONT | 2);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                             actor->z.fixed + PIXELS(8));
    Object_SetMode(actor, 7);
    FieldObject_WaitForMove(actor);
    do {
      Engine_TaskWait(1);
    } while (actor->y.fixed != actor->terrainHeight);
    Object_SetMode(actor, 6);
    Engine_TaskWait(3);
    return;
  }
  SET_VECTOR(nextPosition,
             actor->x.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x, 19),
             actor->y.fixed,
             actor->z.fixed + VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z, 19));
  collision = FieldObject_CheckMovementCollision(actor, nextPosition);
  if (collision > 0) {
    return;
  }
  SET_VECTOR(nextPosition,
             actor->x.fixed + direction->x * 0x5b333 - direction->z * 0x5b333,
             actor->y.fixed,
             actor->z.fixed + direction->z * 0x5b333 - direction->x * 0x5b333);
  collision = FieldObject_CheckMovementCollision(actor, nextPosition);
  if (collision > 0) {
    actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  SET_VECTOR(
      nextPosition, (direction->x + direction->z) * 0x5b333 + actor->x.fixed,
      actor->y.fixed, (direction->x + direction->z) * 0x5b333 + actor->z.fixed);
  collision = FieldObject_CheckMovementCollision(actor, nextPosition);
  if (collision > 0) {
    actor->x.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
    actor->z.fixed -= VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
    return;
  }
  // Advance the camera anchor together with an unobstructed actor.
  viewCenter->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  actor->x.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->x * 3, 15);
  viewCenter->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
  actor->z.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(direction->z * 3, 15);
}

/// Chooses the particle scales and spin used by the tile-wave animations.
static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *options) {
  options->startScaleX =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->startScaleY =
      ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
  options->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

/// Animates the two tile waves before taking the linked passage exit.
static void VenusLighthouse_AnimatePairedTileWaves(void) {
  struct EffectOptions options;
  u32 extraBursts;
  u32 row;
  u32 particleIndex;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  extraBursts = 0;
  // Advance the first tile wave, holding its fourth row for three extra bursts.
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (;;) {
      for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
        Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000,
                     0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0,
                     0, 0, 0x880000, &options);
      }
      Engine_TaskWait(3);
      if (row != 3 || extraBursts >= 3) {
        break;
      }
      extraBursts++;
    }
    Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
  }
  Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
  Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
  Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
  for (row = 0; row < 10; row++) {
    Rubble_SetScaleAndSpin(&options);
    for (particleIndex = 0; particleIndex < 4 && row < 8; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000,
                   0, 0x300000 + row * 0x100000 + particleIndex * 0x40000, 0, 0,
                   0, 0x880000, &options);
    }
    Engine_TaskWait(3);
    Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
  }
  Engine_AudioPlayCue(SE_289);
  Engine_EventWait(60);
  Engine_EventRequestExit(21);
  Engine_EventEnd();
}

/// Emits the scheduled particles at the supplied passage coordinates.
static void VenusLighthouse_SpawnRandomParticles(s32 x, s32 z) {
  struct EffectOptions options;

  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
  if ((gFrameCount & 3) == 0) {
    if (((u32)(Engine_RandomNext() << 1) >> 16) != 0) {
      Effect_Spawn(
          (x + (((u32)(Engine_RandomNext() << 1) >> 16) << 4)) << 16, 0,
          z << 19, 0, 0,
          (s32)((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000) / 10,
          0x880000, &options);
    } else {
      Effect_Spawn((x + ((u32)(Engine_RandomNext() * 17) >> 16)) << 16, 0,
                   (x << 19) - 0x40000, 0, 0, 0, 0x880000, &options);
    }
  }
}

static void VenusLighthouse_EmitWestPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(432, 32);
}

static void VenusLighthouse_EmitEastPassageParticles(void) {
  VenusLighthouse_SpawnRandomParticles(0x230, 44);
}

/// Animates the western tile wave and starts its lingering particle task.
static void VenusLighthouse_AnimateWestTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(0x8000, 0x1000);
  Engine_CameraMoveTo(PIXELS(440), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Repeat particle bursts on the fourth row before advancing the copied tiles.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x1a00000,
                   0, particleZ, 0, zero, zero, 0x880000, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3) {
      if (extraBursts <= 2) {
        extraBursts++;
        goto again;
      }
      if (extraBursts == 3)
        FieldScheduler_AddCallback(VenusLighthouse_EmitWestPassageParticles,
                                   3200);
    }
    Map_CopyCellsTo(53, row + 12, 26, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(81, 41, 89, 14, 9, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_WEST_WALL_MOVED);
  Engine_EventRequestExit(19);
  Engine_EventEnd();
}

/// Animates the eastern tile wave and starts its lingering particle task.
static void VenusLighthouse_AnimateEastTileWave(void) {
  struct EffectOptions options;
  u32 extraBursts;
  s32 zero;
  u32 row;
  s32 rowOffset;
  s32 particleZ;
  u32 particleIndex;
  struct EffectOptions *effectOptions;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventBegin();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_162);
  Engine_CameraSetSpeed(0x8000, 0x1000);
  Engine_CameraMoveTo(PIXELS(568), -1, PIXELS(360), 1);
  extraBursts = 0;
  row = 0;
  effectOptions = &options;
  rowOffset = 0;
  do {
    Rubble_SetScaleAndSpin(&options);
  // Keep this route task scheduling after each completed row.
  again:
    particleZ = 0xc00000;
    for (particleIndex = 0, zero = 0, particleZ += rowOffset;
         particleIndex <= 3; particleIndex++) {
      Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000,
                   0, particleZ, 0, zero, zero, 0x880000, effectOptions);
      particleZ += 0x40000;
    }
    Engine_TaskWait(3);
    if (row == 3 && extraBursts <= 2) {
      extraBursts++;
      goto again;
    }
    FieldScheduler_AddCallback(VenusLighthouse_EmitEastPassageParticles, 3200);
    Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
    rowOffset += 0x100000;
    row++;
  } while (row <= 12);
  Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
  Engine_CameraWaitForMove();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventWait(60);
  Engine_GameFlagSet(FLAG_VENUS_LIGHTHOUSE_EAST_WALL_MOVED);
  Engine_EventRequestExit(20);
  Engine_EventEnd();
}

// Emit rising particles around the actor every four frames.
// UB: update hooks return void. In tbs-en venus_lighthouse, plain
// void differs in 14 bytes of the 196-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#else
static s32 VenusLighthouse_UpdateRisingSpray(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  struct EffectOptions *effectOptions = &options;
  s32 framePhase;
  s32 particleX, particleY, particleZ, riseSpeed;

  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 286;
  effectOptions->script = gVenusSprayScript;
  framePhase = gFrameCount & 3;
  if (framePhase == 0) {
    if ((gFrameCount & 7) == 0) {
      Engine_AudioPlayCue(SE_246);
    }
    particleX =
        actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleY =
        actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    particleZ =
        actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
    riseSpeed = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
    Effect_Spawn(particleX, particleY, particleZ, 0, riseSpeed, framePhase,
                 0x330000, effectOptions);
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Lands the leader on the spray, then raises both toward the exit.
static void VenusLighthouse_RaiseLeaderWithSpray(void) {
  u32 riseFrame;
  struct FieldActor *leader;
  struct FieldActor *spray;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  spray = Object_GetById(20);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  // Land the leader on the spray, then synchronize their small settling
  // offsets.
  leader->y.fixed = PIXELS(130);
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Event_PlayCue(SE_204);
  Engine_EventWait(30);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_EventWait(24);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
  leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
  spray->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-3, 16);
  Engine_TaskWait(2);
  spray->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->y.fixed += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  leader->terrainHeight += VENUS_LIGHTHOUSE_FIXED_OFFSET(-2, 16);
  Engine_TaskWait(10);
  spray->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  spray->y.fixed += PIXELS(2);
  leader->y.fixed += PIXELS(2);
  leader->terrainHeight += PIXELS(2);
  Engine_TaskWait(4);
  spray->y.fixed += PIXELS(1);
  leader->y.fixed += PIXELS(1);
  leader->terrainHeight += PIXELS(1);
  leader->motionFlags = 0;
  spray->motionFlags = 0;
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
#ifdef UBFIX
  leader->update = VenusLighthouse_UpdateRisingSpray;
#else
  leader->update = (void *)VenusLighthouse_UpdateRisingSpray;
#endif
  Engine_EventWait(60);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetSpritePriority(20, 1);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_308);
  GameFlag_Set(0x101);
  // Raise the leader, terrain contact and spray by the same pixel each frame.
  for (riseFrame = 0; riseFrame < 128; riseFrame++) {
    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
    spray->y.fixed += PIXELS(1);
    Engine_TaskWait(1);
  }
  Engine_EventRequestExit(21);
}

/// Selects the block's push script from its direction, support animation and
/// socket.
static s32
VenusLighthouse_DispatchPushScript(struct ScriptInterpreter *interpreter) {
  struct FieldActor *block = (struct FieldActor *)interpreter;
  struct FieldPosition supportPosition;
  struct FieldActor *support;
  struct AnimationObject *sprite;
  const u8 *allowedShapes;
  u8 shapeId;
  u32 switchIndex;
  s32 shapeIndex;
  const u8 *shapeCursor;

  supportPosition.x = block->x.fixed;
  supportPosition.y = block->y.fixed - 0x100000;
  supportPosition.z = block->z.fixed;
  support = SceneData_FindSlotAtPosition(&supportPosition, 0);
  // XXX: Do the block placement and action-script tables guarantee a support
  // with an animation entry every time this script runs?
  sprite = support->sprite;
  if (sprite->entries[0]->animId == 0x100) {
    s32 velocityX = block->velocityX;
    s32 speedX = velocityX < 0 ? -velocityX : velocityX;
    s32 velocityZ = block->velocityZ;
    s32 speedZ = velocityZ < 0 ? -velocityZ : velocityZ;

    if (speedX > speedZ) {
      if (velocityX / 0x10000 < 0) {
        allowedShapes = gVenusShapesPushedLeft;
      } else {
        allowedShapes = gVenusShapesPushedRight;
      }
    } else {
      if (velocityZ / 0x10000 < 0) {
        allowedShapes = gVenusShapesPushedUp;
      } else {
        allowedShapes = gVenusShapesPushedDown;
      }
    }
    for (; *allowedShapes != 0; allowedShapes++) {
      if (sprite->lastNo == *allowedShapes) {
        break;
      }
    }
    if (*allowedShapes == 0) {
      Engine_ObjectSetScript(block, gVenusRefusedPushScript);
      return 0;
    }
    // The socket tables retain their moving cursors between push-script calls.
    if (gGameState.scene == (s32)&SceneId_VenusLighthouse5) {
      for (switchIndex = 0;
           switchIndex <= 3 &&
           !(block->x.fixed >> 20 == gVenusSwitchCells[switchIndex].x &&
             block->z.fixed >> 20 == gVenusSwitchCells[switchIndex].z);
           switchIndex++) {
      }
      for (shapeIndex = 0;; shapeIndex++) {
        shapeCursor = gVenusSwitchShapes[switchIndex];
        shapeId = *shapeCursor;
        if (shapeId == 0) {
          Engine_ObjectSetScript(block, gVenusRefusedPushScript);
          return 0;
        }
        if (shapeId == support->sprite->lastNo) {
          break;
        }
        gVenusSwitchShapes[switchIndex] = shapeCursor + 1;
      }
      Engine_ObjectSetScript(block,
                             gVenusSwitchPushScripts[switchIndex][shapeIndex]);
    } else {
      for (switchIndex = 0;
           switchIndex <= 7 &&
           !(block->x.fixed >> 20 == gVenusPillarCells[switchIndex].x &&
             block->z.fixed >> 20 == gVenusPillarCells[switchIndex].z);
           switchIndex++) {
      }
      for (shapeIndex = 0;; shapeIndex++) {
        shapeCursor = gVenusPillarShapes[switchIndex];
        shapeId = *shapeCursor;
        if (shapeId == 0) {
          Engine_ObjectSetScript(block, gVenusRefusedPushScript);
          return 0;
        }
        if (shapeId == support->sprite->lastNo) {
          break;
        }
        gVenusPillarShapes[switchIndex] = shapeCursor + 1;
      }
      Engine_ObjectSetScript(block,
                             gVenusPillarPushScripts[switchIndex][shapeIndex]);
    }
  } else {
    Engine_ObjectSetScript(block, gVenusRefusedPushScript);
  }
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Step for each of 16 directions: X in the upper halfword, Z in the lower.
// UB: westward rows contain a negative X step; pack its unsigned bits.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

u32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SwitchCell gVenusSwitchCells[] = {
    {48, 41},
    {52, 41},
    {48, 43},
    {52, 43},
};

// clang-format off
u32 gVenusBlockHeights[] = {
    0xfff00000, 0xfff00000, 0xffe00000, 0xffd00000, 0xffc00000, 0xffb00000, 0xffb00000,
};

struct SwitchCell gVenusPillarCells[] = {
    { 11, 39 },
    { 14, 39 },
    { 11, 41 },
    { 16, 42 },
    { 10, 43 },
    { 14, 43 },
    { 10, 46 },
    { 16, 46 },
};

u8 gVenusShapesPushedLeft[] = {
    5, 6, 7, 0,
};

u8 gVenusShapesPushedRight[] = {
    4, 6, 8, 0,
};

u8 gVenusShapesPushedUp[] = {
    3, 7, 8, 0,
};

u8 gVenusShapesPushedDown[] = {
    3, 4, 5, 0,
};

u8 gVenusSwitchShape0[] = {
    6, 7, 8, 0,
};

u8 gVenusSwitchShape1[] = {
    6, 7, 8, 0,
};

u8 gVenusSwitchShape2[] = {
    4, 5, 6, 0,
};

u8 gVenusSwitchShape3[] = {
    4, 5, 6, 0,
};

u8 gVenusPillarShape0[] = {
    7, 0,
};

u8 gVenusPillarShape1[] = {
    0, 5,
};

u8 gVenusPillarShape2[] = {
    0, 8,
};

u8 gVenusPillarShape3[] = {
    0, 5,
};

u8 gVenusPillarShape4[] = {
    0, 7, 8, 0,
};

u8 gVenusPillarShape5[] = {
    3, 0,
};

u8 gVenusPillarShape6[] = {
    3, 0,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVenusLighthousePlacements2_0[] = {
    SC_SET(0x23, 0x2126),
    SC_SET(0x27, 1),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

s32 gVenusLighthousePlacements3_Data[] = {
    SC_SET(0x23, 0x2126),
    SC_CALL(VenusLighthouse_UpdateChildFromTimer),
    SC_END,
};

s32 gVenusLeaderApproachScript[] = {
    SC_ARG(12),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_PulseChildEffect),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 0x11999),
    SC_SET(SW_SCALE_Y, 0x11999),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0xe666),
    SC_SET(SW_SCALE_Y, 0xe666),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s32 gVenusLighthousePlacements2_1[] = {
    SC_SET(SW_SCALE_X, 0x9999),
    SC_SET(SW_SCALE_Y, 0x9999),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_END,
};

s32 gVenusSprayScript[] = {
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0x4000),
    SC_SET(SW_SCALE_X, 0x4000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gVenusLighthousePlacements4_1[] = {
    SC_ARG(3),
    SC_END,
};

s32 gVenusLighthousePlacements5_3[] = {
    SC_ARG(4),
    SC_END,
};

s32 gVenusLighthousePlacements6_4[] = {
    SC_ARG(5),
    SC_END,
};

s32 gVenusLighthousePlacements5_2[] = {
    SC_ARG(6),
    SC_END,
};

s32 gVenusLighthousePlacements5_1[] = {
    SC_ARG(7),
    SC_END,
};

s32 gVenusLighthousePlacements6_3[] = {
    SC_ARG(8),
    SC_END,
};

s32 gVenusPushScript[] = {
    SC_CALL(VenusLighthouse_ClearScriptedSpriteFlags),
    SC_ARG(7),
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_HOOK, (s32)VenusLighthouse_EmitGateParticles),
    SC_MOVE(PIXELS(760), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(728)),
    SC_MOVE(PIXELS(760), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(760), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(744)),
    SC_MOVE(PIXELS(744), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(743), 0, PIXELS(696)),
    SC_WAIT(1),
    SC_END,
};

s32 gVenusSettleScriptA[] = {
    SC_MOVE(PIXELS(872), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(680)),
    SC_MOVE(PIXELS(872), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(872), 0, PIXELS(664)),
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSettleScriptB[] = {
    SC_MOVE(PIXELS(888), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(888), 0, PIXELS(696)),
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusRefusedPushScript[] = {
    SC_SET(SW_HOOK, 0),
    SC_WAIT(10),
    SC_FREE,
};

s32 gVenusSwitchPushScript0_0[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(664)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(0x28, 1),
    SC_END,
};

s32 gVenusSwitchPushScript0_1[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript1_0[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript1_1[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript2_1[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript2_0[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(696)),
    SC_MOVE(PIXELS(728), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(728), 0, PIXELS(632)),
    SC_SET(0x28, 1),
    SC_END,
};

s32 gVenusSwitchPushScript3_1[] = {
    SC_MOVE(PIXELS(840), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(840)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSwitchPushScript3_0[] = {
    SC_MOVE(PIXELS(776), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(776)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusFirstGateScript[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(776)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(776)),
    SC_MOVE(PIXELS(216), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(760)),
    SC_END,
};

s32 gVenusFirstGateOpenScript[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(760)),
    SC_MOVE(PIXELS(264), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript7_0[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(232), 0, PIXELS(728)),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript5_1[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(696)),
    SC_MOVE(PIXELS(216), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript5_0[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(696)),
    SC_MOVE(PIXELS(264), 0, PIXELS(680)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(264)),
    SC_SET(SW_Z, PIXELS(680)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript2_0[] = {
    SC_MOVE(PIXELS(184), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript3_0[] = {
    SC_MOVE(PIXELS(248), 0, PIXELS(680)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(680)),
    SC_MOVE(PIXELS(248), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(664)),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(264), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(648)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(648)),
    SC_MOVE(PIXELS(248), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(248), 0, PIXELS(632)),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(232)),
    SC_SET(SW_Z, PIXELS(632)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusSecondGateScript[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(744)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(744)),
    SC_MOVE(PIXELS(184), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(760)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(760)),
    SC_MOVE(PIXELS(168), 0, PIXELS(744)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(744)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript6_0[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(728)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(728)),
    SC_MOVE(PIXELS(184), 0, PIXELS(696)),
    SC_SYNC,
    SC_POS(PIXELS(184), 0, PIXELS(696)),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(168)),
    SC_SET(SW_Z, PIXELS(696)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript4_0[] = {
    SC_MOVE(PIXELS(168), 0, PIXELS(664)),
    SC_SYNC,
    SC_POS(PIXELS(168), 0, PIXELS(664)),
    SC_MOVE(PIXELS(184), 0, PIXELS(664)),
    SC_SYNC,
    SC_SET(SW_X, PIXELS(184)),
    SC_SET(SW_Z, PIXELS(664)),
    SC_CALL(VenusLighthouse_DispatchPushScript),
    SC_END,
};

s32 gVenusPillarPushScript0_0[] = {
    SC_MOVE(PIXELS(216), 0, PIXELS(632)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(632)),
    SC_MOVE(PIXELS(216), 0, PIXELS(600)),
    SC_SYNC,
    SC_POS(PIXELS(216), 0, PIXELS(600)),
    SC_SET(0x28, 1),
    SC_END,
};

u16 gVenusPushCells[] = {
    MAP_CELL_STEP(124, 50, 1, 2, 6), MAP_CELL_STEP(124, 47, 1, 2, 6), MAP_CELL_STEP(124, 44, 1, 2, 6), MAP_CELL_STEP(124, 41, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusSettleCells[] = {
    MAP_CELL_STEP(117, 59, 1, 2, 6), MAP_CELL_STEP(115, 59, 1, 2, 6), MAP_CELL_STEP(113, 59, 1, 2, 6), MAP_CELL_STEP(111, 59, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gVenusGateCells[] = {
    MAP_CELL_STEP(96, 58, 1, 2, 6), MAP_CELL_STEP(95, 58, 1, 2, 6), MAP_CELL_STEP(96, 56, 1, 2, 6), MAP_CELL_STEP(95, 56, 1, 2, 6), MAP_CELL_STEPS_END,
};

struct SceneEntrance gVenusLighthouseEntrances1[] = {
    { 1, CONDITION_ALWAYS, 40, 0, 264, FACING_EAST, 0, 0, 0, 512, 544, 0 },
    { 2, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 544, 0 },
    { 3, CONDITION_ALWAYS, 488, 0, 424, FACING_WEST, 0, 0, 0, 512, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrancesOther[] = {
    { 1, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0 },
    { 2, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0 },
    { 3, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0 },
    { 4, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 5, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 6, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 7, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0 },
    { 8, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 9, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 10, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 11, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 12, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { 13, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { 14, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0 },
    { 20, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { 21, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { 22, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 23, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 24, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { 25, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { 26, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 30, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { 31, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { 32, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrances3[] = {
    { 1, CONDITION_ALWAYS, 456, 0, 552, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { 2, CONDITION_ALWAYS, 456, 0, 592, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { 3, CONDITION_ALWAYS, 696, 0, 472, FACING_SOUTH, 0, 376, 344, 736, 624, 0 },
    { 4, CONDITION_ALWAYS, 696, 0, 512, FACING_NORTH, 0, 376, 344, 736, 624, 0 },
    { 5, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 344, 48, 592, 216, 0 },
    { 6, CONDITION_ALWAYS, 520, 0, 176, FACING_NORTH, 0, 344, 48, 592, 216, 0 },
    { 7, CONDITION_ALWAYS, 104, 0, 752, FACING_NORTH, 0, 56, 584, 304, 880, 0 },
    { 8, CONDITION_ALWAYS, 200, 0, 680, FACING_SOUTH, 0, 56, 584, 304, 880, 0 },
    { 9, CONDITION_ALWAYS, 88, 0, 344, FACING_SOUTH, 0, 40, 280, 288, 496, 0 },
    { 10, CONDITION_ALWAYS, 248, 0, 432, FACING_NORTH, 0, 40, 280, 288, 496, 0 },
    { 11, CONDITION_ALWAYS, 664, 0, 264, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { 12, CONDITION_ALWAYS, 728, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { 13, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { 14, CONDITION_ALWAYS, 952, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { 15, CONDITION_ALWAYS, 840, 0, 360, FACING_NORTH, 0, 616, 40, 1000, 360, 0 },
    { 16, CONDITION_ALWAYS, 88, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { 17, CONDITION_ALWAYS, 184, 0, 104, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { 18, CONDITION_ALWAYS, 280, 0, 152, FACING_SOUTH, 0, 40, 40, 328, 208, 0 },
    { 19, CONDITION_ALWAYS, 472, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { 20, CONDITION_ALWAYS, 600, 0, 776, FACING_SOUTH, 0, 424, 688, 672, 848, 0 },
    { 21, CONDITION_ALWAYS, 840, 0, 136, FACING_SOUTH, 0, 616, 40, 1000, 360, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrances4[] = {
    { 1, CONDITION_ALWAYS, 104, 0, 120, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 200, FACING_SOUTH, 0, 56, 56, 368, 272, 0 },
    { 3, CONDITION_ALWAYS, 600, 0, 216, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { 4, CONDITION_ALWAYS, 696, 0, 312, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { 5, CONDITION_ALWAYS, 648, 0, 536, FACING_NORTH, 0, 552, 88, 848, 544, 0 },
    { 6, CONDITION_ALWAYS, 776, 0, 536, FACING_SOUTH, 0, 552, 88, 848, 544, 0 },
    { 7, CONDITION_ALWAYS, 104, 0, 520, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { 8, CONDITION_ALWAYS, 312, 0, 584, FACING_NORTH, 0, 56, 392, 352, 608, 0 },
    { 9, CONDITION_ALWAYS, 744, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { 10, CONDITION_ALWAYS, 808, 0, 696, FACING_SOUTH, 0, 648, 632, 896, 800, 0 },
    { 11, CONDITION_ALWAYS, 312, 0, 536, FACING_SOUTH, 0, 56, 392, 352, 608, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrances5[] = {
    { 1, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, 8, 56, 256, 224, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 184, FACING_NORTH, 0, 8, 56, 256, 224, 0 },
    { 3, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, 8, 264, 256, 432, 0 },
    { 4, CONDITION_ALWAYS, 312, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { 5, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { 6, CONDITION_ALWAYS, 472, 0, 680, FACING_SOUTH, 0, 248, 600, 512, 768, 0 },
    { 7, CONDITION_ALWAYS, 136, 0, 792, FACING_SOUTH, 0, 8, 728, 256, 928, 0 },
    { 8, CONDITION_ALWAYS, 136, 0, 904, FACING_NORTH, 0, 8, 728, 256, 928, 0 },
    { 9, CONDITION_ALWAYS, 312, 0, 232, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { 10, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { 11, CONDITION_ALWAYS, 504, 0, 248, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { 12, CONDITION_ALWAYS, 680, 0, 120, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { 13, CONDITION_ALWAYS, 312, 0, 440, FACING_SOUTH, 0, 264, 56, 720, 480, 0 },
    { 14, CONDITION_ALWAYS, 632, 0, 392, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { 15, CONDITION_ALWAYS, 728, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { 16, CONDITION_ALWAYS, 888, 0, 632, FACING_SOUTH, 0, 680, 568, 928, 768, 0 },
    { 17, CONDITION_ALWAYS, 136, 0, 568, FACING_SOUTH, 0, 8, 480, 256, 648, 0 },
    { 18, CONDITION_ALWAYS, 136, 0, 600, FACING_NORTH, 0, 8, 480, 256, 648, 0 },
    { 19, CONDITION_ALWAYS, 440, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { 20, CONDITION_ALWAYS, 568, 0, 264, FACING_NORTH, 0, 264, 56, 720, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVenusLighthouseEntrances6[] = {
    { 1, CONDITION_ALWAYS, 120, 0, 248, FACING_SOUTH, 0, 72, 56, 320, 416, 0 },
    { 2, CONDITION_ALWAYS, 280, 0, 280, FACING_NORTH, 0, 72, 56, 320, 416, 0 },
    { 3, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 4, CONDITION_ALWAYS, 536, 0, 216, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 5, CONDITION_ALWAYS, 664, 0, 88, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 6, CONDITION_ALWAYS, 744, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 7, CONDITION_ALWAYS, 840, 0, 152, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 8, CONDITION_ALWAYS, 936, 0, 280, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { 9, CONDITION_ALWAYS, 472, 0, 472, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 10, CONDITION_ALWAYS, 616, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { 11, CONDITION_ALWAYS, 728, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { 12, CONDITION_ALWAYS, 824, 0, 456, FACING_NORTH, 0, 424, 24, 992, 560, 0 },
    { 13, CONDITION_ALWAYS, 792, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { 14, CONDITION_ALWAYS, 904, 0, 648, FACING_SOUTH, 0, 712, 584, 976, 864, 0 },
    { 15, CONDITION_ALWAYS, 104, 0, 776, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { 16, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, 56, 536, 304, 848, 0 },
    { 17, CONDITION_ALWAYS, 600, 0, 744, FACING_SOUTH, 0, 392, 648, 640, 832, 0 },
    { 18, CONDITION_ALWAYS, 536, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 19, CONDITION_ALWAYS, 792, 0, 328, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { 20, CONDITION_ALWAYS, 664, 0, 136, FACING_SOUTH, 0, 424, 24, 992, 560, 0 },
    { SCENE_TABLE_END },
};

u32 gVenusLighthousePrimaryTable[] = {
    SCENE_EXITS(181),
    SCENE_EXIT(1, 2, 32),
    SCENE_EXIT(2, 182, 1),
    SCENE_EXIT(3, 2, 61),
    SCENE_EXIT(0, 182, 0),
    SCENE_EXIT(1, 181, 2),
    SCENE_EXIT(2, 182, 3),
    SCENE_EXIT(3, 182, 2),
    SCENE_EXIT(4, 182, 31),
    SCENE_EXIT(5, 182, 30),
    SCENE_EXIT(6, 182, 7),
    SCENE_EXIT(7, 182, 6),
    SCENE_EXIT(8, 182, 32),
    SCENE_EXIT(9, 182, 22),
    SCENE_EXIT(10, 182, 12),
    SCENE_EXIT(11, 182, 13),
    SCENE_EXIT(12, 182, 10),
    SCENE_EXIT(13, 182, 11),
    SCENE_EXIT(14, 182, 24),
    SCENE_EXIT(20, 182, 26),
    SCENE_EXIT(21, 182, 25),
    SCENE_EXIT(22, 182, 9),
    SCENE_EXIT(23, 183, 16),
    SCENE_EXIT(24, 182, 14),
    SCENE_EXIT(25, 182, 21),
    SCENE_EXIT(26, 182, 20),
    SCENE_EXIT(30, 182, 5),
    SCENE_EXIT(31, 182, 4),
    SCENE_EXIT(32, 182, 8),
    SCENE_EXIT(0, 183, 0),
    SCENE_EXIT(1, 183, 19),
    SCENE_EXIT(2, 183, 9),
    SCENE_EXIT(3, 184, 1),
    SCENE_EXIT(4, 183, 5),
    SCENE_EXIT(5, 183, 4),
    SCENE_EXIT(6, 183, 13),
    SCENE_EXIT(7, 183, 14),
    SCENE_EXIT(8, 184, 4),
    SCENE_EXIT(9, 183, 2),
    SCENE_EXIT(10, 183, 11),
    SCENE_EXIT(11, 183, 10),
    SCENE_EXIT(12, 184, 7),
    SCENE_EXIT(13, 183, 6),
    SCENE_EXIT(14, 183, 7),
    SCENE_EXIT(15, 183, 17),
    SCENE_EXIT(16, 182, 23),
    SCENE_EXIT(17, 183, 15),
    SCENE_EXIT(18, 173, 16),
    SCENE_EXIT(19, 183, 1),
    SCENE_EXIT(20, 183, 20),
    SCENE_EXIT(21, 184, 11),
    SCENE_EXIT(0, 184, 0),
    SCENE_EXIT(1, 183, 3),
    SCENE_EXIT(2, 184, 3),
    SCENE_EXIT(3, 184, 2),
    SCENE_EXIT(4, 183, 8),
    SCENE_EXIT(5, 184, 10),
    SCENE_EXIT(6, 185, 17),
    SCENE_EXIT(7, 183, 12),
    SCENE_EXIT(8, 184, 9),
    SCENE_EXIT(9, 184, 8),
    SCENE_EXIT(10, 184, 5),
    SCENE_EXIT(11, 183, 21),
    SCENE_EXIT(0, 185, 0),
    SCENE_EXIT(1, 186, 1),
    SCENE_EXIT(2, 185, 4),
    SCENE_EXIT(3, 185, 5),
    SCENE_EXIT(4, 185, 2),
    SCENE_EXIT(5, 185, 3),
    SCENE_EXIT(6, 185, 9),
    SCENE_EXIT(7, 186, 5),
    SCENE_EXIT(8, 185, 11),
    SCENE_EXIT(9, 185, 6),
    SCENE_EXIT(10, 186, 4),
    SCENE_EXIT(11, 185, 8),
    SCENE_EXIT(12, 186, 7),
    SCENE_EXIT(13, 186, 9),
    SCENE_EXIT(14, 185, 15),
    SCENE_EXIT(15, 185, 14),
    SCENE_EXIT(16, 185, 18),
    SCENE_EXIT(17, 184, 6),
    SCENE_EXIT(18, 185, 16),
    SCENE_EXIT(19, 186, 18),
    SCENE_EXIT(20, 186, 19),
    SCENE_EXIT(0, 186, 0),
    SCENE_EXIT(1, 185, 1),
    SCENE_EXIT(2, 186, 3),
    SCENE_EXIT(3, 186, 2),
    SCENE_EXIT(4, 185, 10),
    SCENE_EXIT(5, 185, 7),
    SCENE_EXIT(6, 186, 17),
    SCENE_EXIT(7, 185, 12),
    SCENE_EXIT(8, 186, 16),
    SCENE_EXIT(9, 185, 13),
    SCENE_EXIT(10, 186, 13),
    SCENE_EXIT(11, 186, 14),
    SCENE_EXIT(12, 186, 15),
    SCENE_EXIT(13, 186, 10),
    SCENE_EXIT(14, 186, 11),
    SCENE_EXIT(15, 186, 12),
    SCENE_EXIT(16, 186, 8),
    SCENE_EXIT(17, 186, 6),
    SCENE_EXIT(18, 185, 19),
    SCENE_EXIT(19, 185, 20),
    SCENE_EXIT(20, 186, 20),
    SCENE_EXIT(21, 187, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gVenusLighthousePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements1[] = {
    { 0x98, FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(268), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(428), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(368), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(172), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(412), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(364), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements2[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, 0x0, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x71, 0x0, ACTOR_STAND,
        PIXELS(304), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x70, 0x0, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(592), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(88), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(664), 0, PIXELS(792), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x114, 0x0, (s32)gVenusLighthousePlacements2_1,
        PIXELS(488), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(72), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(104), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(136), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(168), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements3[] = {
    { 0xfd, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xfd, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(184), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xfd, 0x0, (s32)gVenusLighthousePlacements3_Data,
        PIXELS(200), 0, PIXELS(824), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(376), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements4[] = {
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(808), 0, PIXELS(728), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements4_1,
        PIXELS(776), 0, PIXELS(712), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x11c, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements5[] = {
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(888), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_1,
        PIXELS(792), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_2,
        PIXELS(808), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_3,
        PIXELS(824), 0, PIXELS(680), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x11c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVenusLighthousePlacements6[] = {
    { 0x101, 0x0, 7,
        PIXELS(104), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x101, 0x0, 7,
        PIXELS(168), 0, PIXELS(312), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x100, 0x0, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(120), FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x1f4, 0x0, (s32)gVenusLighthousePlacements2_0,
        PIXELS(136), 0, PIXELS(776), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements4_1,
        PIXELS(184), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements5_1,
        PIXELS(200), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements6_3,
        PIXELS(232), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, 0x0, (s32)gVenusLighthousePlacements6_4,
        PIXELS(248), 0, PIXELS(616), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x11c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xf2, 0x0, 7,
        PIXELS(664), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVenusLighthouseEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIodemBeHappyTheEntranceTo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseOhhhHowCouldTheyAttackMere) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWaitingScholar) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseHowHowDidTheyOpenThe) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyAreAlreadyHeadedForBabi) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseBabisSoldiersAreAllColossoVictors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HearOfTheAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseUhnnnIfOnlyICouldMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheirPowersWereSoStrangeWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIMustGetInsideVenusLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheYoungBoySeemedToGlow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseTheyWerentInVenusLighthouseVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIThoughtIRecognizedOneOf) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { 0x31, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { 0x31, 8, CONDITION_ALWAYS, 8 },
    { 0x21, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { 0x21, 14, CONDITION_ALWAYS, 14 },
    { 0x31, 20, CONDITION_ALWAYS, 20 },
    { 0x31, 21, CONDITION_ALWAYS, 21 },
    { 0x31, 22, CONDITION_ALWAYS, 22 },
    { 0x31, 23, CONDITION_ALWAYS, 23 },
    { 0x31, 24, CONDITION_ALWAYS, 24 },
    { 0x21, 25, CONDITION_ALWAYS, 25 },
    { 0x21, 26, CONDITION_ALWAYS, 26 },
    { 0x21, 30, CONDITION_ALWAYS, 30 },
    { 0x21, 31, CONDITION_ALWAYS, 31 },
    { 0x21, 32, CONDITION_ALWAYS, 32 },
    { EVENT_TOUCH, 45, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_JumpDown) },
    { EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnFloorSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlockTowardWall) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenToldThemToStopBut) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenWentInsideButThenCame) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToScholarByDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIfKradenHadntBeenThereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseKradenSaidSomethingAboutThisBeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVenusLighthouseIWonderWhatMysteriesTheStatue) },
    { SEARCH(SEARCH_UNNAMED), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#if !TBS_PATCH_1
    { SEARCH(SEARCH_UNNAMED), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 38, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
    { SEARCH(SEARCH_UNNAMED), 39, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
#if TBS_PATCH_LEVEL != 1
    { SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRelief) },
#endif
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchStatue) },
    { SEARCH(SEARCH_CHEST), 100, 0xf37, FIND_ITEM(ITEM_LUCKY_CAP) },
    { SEARCH(SEARCH_CHEST), 101, 0xf38, FIND_ITEM(ITEM_CARRY_STONE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveBlockTowardWall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MoveBlockOntoSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryBlockPastWall) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HidePath) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseEvents3[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { 0x21, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { 0x21, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { 0x21, 16, CONDITION_ALWAYS, 16 },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
    { 0x31, 18, CONDITION_ALWAYS, 18 },
    { 0x21, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FallThroughFloor) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthBlock) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDepthBlock) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearEffectTarget) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetLeaderAsEffectTarget) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushCarryBlock) },
    { SEARCH(SEARCH_CHEST), 100, 0xf34, FIND_ITEM(ITEM_THUNDER_CROWN) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveDepthBlock) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryBlock) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryBlock) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, 1), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryBlock) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, 1), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftCarryBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveCarryBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryBlockAcrossGap) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryBlockAcrossGap) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseEvents4[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWallSwitch) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 36, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_JumpOrPushGateBlock) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_VENUS_LIGHTHOUSE_GATE_OPEN, EVENT_SCRIPT(Event_MoveGateBlock) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseEvents5[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { 0x21, 10, CONDITION_ALWAYS, 10 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSparklingPassage) },
    { 0x21, 12, CONDITION_ALWAYS, 12 },
    { 0x21, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { 0x31, 17, CONDITION_ALWAYS, 17 },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushSwitchBlock) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), 100, 0xf35, FIND_ITEM(ITEM_DRAGON_SCALES) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveSwitchBlock) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVenusLighthouseEvents6[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { 0x31, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { 0x31, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { 0x31, 17, CONDITION_ALWAYS, 17 },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterRisingPassage) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzleBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzleBlock) },
    { TOUCH_WALKING_INTO(FACING_EAST), 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossGap) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 37, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPuzzleBlock) },
    { EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnWestWallSwitch) },
    { EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnEastWallSwitch) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftSouth) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftNorth) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftWest) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DriftEast) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushPillarBlock) },
    { SEARCH(SEARCH_UNNAMED), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor) },
    { SEARCH(SEARCH_CHEST), 100, 0xf36, FIND_ITEM(ITEM_GAIA_BLADE) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x351, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CarryFloatingBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarSwitchBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarSwitchBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarSwitchBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillarSwitchBlock) },
    { SCENE_EVENTS_END },
};
// clang-format on

u8 *gVenusSwitchShapes[] = {
    gVenusSwitchShape0,
    gVenusSwitchShape1,
    gVenusSwitchShape2,
    gVenusSwitchShape3,
};

s32 *gVenusSwitchPushScript0[] = {
    gVenusSwitchPushScript0_0,
    gVenusSwitchPushScript0_1,
    gVenusRefusedPushScript,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript1[] = {
    gVenusSwitchPushScript1_0,
    gVenusSwitchPushScript1_1,
    gVenusRefusedPushScript,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript2[] = {
    gVenusSwitchPushScript2_0,
    gVenusSwitchPushScript2_1,
    gVenusSwitchPushScript2_0,
    gVenusRefusedPushScript,
};

s32 *gVenusSwitchPushScript3[] = {
    gVenusSwitchPushScript3_0,
    gVenusSwitchPushScript3_1,
    gVenusSwitchPushScript3_0,
    gVenusRefusedPushScript,
};

s32 **gVenusSwitchPushScripts[] = {
    gVenusSwitchPushScript0,
    gVenusSwitchPushScript1,
    gVenusSwitchPushScript2,
    gVenusSwitchPushScript3,
};

u8 *gVenusPillarShapes[] = {
    gVenusPillarShape0,     gVenusPillarShape1,     gVenusPillarShape1 + 1,
    gVenusPillarShape2 + 1, gVenusPillarShape3 + 1, gVenusPillarShape4 + 1,
    gVenusPillarShape5,     gVenusPillarShape6,
};

s32 *gVenusPillarPushScript0[] = {
    gVenusPillarPushScript0_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript1[] = {
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript2[] = {
    gVenusPillarPushScript2_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript3[] = {
    gVenusPillarPushScript3_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript4[] = {
    gVenusPillarPushScript4_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript5[] = {
    gVenusPillarPushScript5_0,
    gVenusPillarPushScript5_1,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript6[] = {
    gVenusPillarPushScript6_0,
    gVenusRefusedPushScript,
};

s32 *gVenusPillarPushScript7[] = {
    gVenusPillarPushScript7_0,
    gVenusRefusedPushScript,
};

s32 **gVenusPillarPushScripts[] = {
    gVenusPillarPushScript0, gVenusPillarPushScript1, gVenusPillarPushScript2,
    gVenusPillarPushScript3, gVenusPillarPushScript4, gVenusPillarPushScript5,
    gVenusPillarPushScript6, gVenusPillarPushScript7,
};
