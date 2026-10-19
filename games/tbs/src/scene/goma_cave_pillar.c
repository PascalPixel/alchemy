// Goma Cave: the room with two pillars, a stump and the Djinni Forge.
#include "script.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/goma_cave_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "b5_context.h"
#include "edition.h"
#include "tbs_edition.h"
#include "field_obj.h"
#include "game_state.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "fixed_point_position.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"

#include "flags.h"
#include "songs.h"
#include "event.h"

enum GomaCavePillarFragmentPlane { PLANE_XY = 0, PLANE_XZ = 1, PLANE_YZ = 2 };

// The room's actors, in placement order.
#define WEST_PILLAR 8
#define STUMP 9
#define EAST_PILLAR 10
#define FORGE 12
#define WITNESS 13 // the man who fought Forge and lost it

// Forge's Djinni number; its name is message 1119 plus the number.
#define DJINNI_FORGE 40

// Shared movement routines stay in their linked order.
#include "common/object_staged_actor.h"

// Talking to Forge: the man beside it jumps in surprise, and the battle with
// the Djinni begins.
static void Event_FightForge(void) {
  OBJ_EMOTE(WITNESS, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(WITNESS, 2, 0);
  Field_RequestDjinnBattle(FORGE, DJINNI_FORGE);
}

// Advances the fragment's selected coordinate plane and keeps its targets in
// step.
static void GomaCavePillar_UpdateFragment(union FieldObject *fieldObject) {
  struct FieldActor *fragment = (struct FieldActor *)fieldObject;
  // The actor's speed and acceleration slots carry the two selected axis steps.
  s16 plane = (s16)fragment->work;

  switch (plane) {
  case PLANE_XY:
    fragment->x.fixed += fragment->speed;
    fragment->targetX = fragment->x.fixed;
    fragment->y.fixed += fragment->acceleration;
    fragment->targetY = fragment->y.fixed;
    break;
  case PLANE_XZ:
    fragment->x.fixed += fragment->speed;
    fragment->targetX = fragment->x.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  case PLANE_YZ:
    fragment->y.fixed += fragment->speed;
    fragment->targetY = fragment->y.fixed;
    fragment->z.fixed += fragment->acceleration;
    fragment->targetZ = fragment->z.fixed;
    break;
  }
}

// Applies child value 15 at the end of a fragment's lifetime.
static s32
GomaCavePillar_FinishFragment(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, 15);
  return 0;
}

// Creates a fragment with two axis rates, a script and packed plane/priority.
static void GomaCavePillar_SpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
                                         s32 secondRate,
                                         s32 scriptAndChildValue,
                                         s32 planeAndPriority) {
  struct GomaCavePillarFragmentScripts table;
  struct FieldActor *leader;
  struct FieldActor *fragment;
  struct AnimationObject *sprite;
  u32 priority;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  table = gGomaCavePillar_FragmentScripts;
  fragment = Engine_ObjectCreate(222, x, y, z);
  if (fragment == 0)
    return;
  sprite = fragment->sprite;
  Object_SetMode(fragment, (scriptAndChildValue + 1) & 15);
  Engine_ObjectSetScript(fragment, table.scripts[scriptAndChildValue & 15]);
  ObjectGroup_SetChildValue(fragment, ((u32)scriptAndChildValue >> 16) & 15);
  fragment->motionFlags = 0;
  sprite->flags = 0;
  fragment->update = GomaCavePillar_UpdateFragment;
  fragment->speed = firstRate;
  fragment->acceleration = secondRate;
  fragment->work = (u16)planeAndPriority;
  priority = (u32)planeAndPriority >> 16;
  switch (priority) {
  case 0:
    sprite->image.priority = leader->sprite->image.priority;
    break;
  case 1:
  case 2:
  case 3:
    fragment->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = priority;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gGomaCavePillar_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gGomaCavePillar_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gGomaCavePillar_Placements;
}

// Copy the map cells for the west pillar on either side of its track.
static inline void GomaCavePillar_CopyWestPillarCells(s32 sourceColumn) {
  Map_CopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

// Move on the west pillar: it slides between its two places, and the map
// cells follow it.
static void Event_MoveWestPillar(void) {
  struct FieldActor *actor;
  s32 x;

  actor = Object_GetById(WEST_PILLAR);
  x = actor->x.fixed;
  if (x < 0)
    x += 0xfffff;
  x >>= 20;
  EV_BEGIN();
  if (x == 20) {
    GomaCavePillar_CopyWestPillarCells(18);
    Engine_GameFlagClear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  } else {
    GomaCavePillar_CopyWestPillarCells(24);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
  }
  EV_END();
}

// While the stump teeters, dust falls from it: one fragment every eighth
// frame, two on every sixteenth.
static void GomaCavePillar_SpawnStumpDust(void) {
  s32 framePhase;
  s32 fixedX;
  s32 fixedZ;
  struct FieldActor *heightSource;
  struct FieldActor *firstXSource, *firstZSource;
  s32 firstRandomX, firstRandomSpeed, firstStyle, firstSpeedBand,
      firstSpeedTimes13, firstFallingSpeed;
  struct FieldActor *secondXSource, *secondZSource;
  s32 secondRandomX, secondRandomSpeed, secondStyle, secondSpeedBand,
      secondSpeedTimes13, secondFallingSpeed;

  framePhase = gFrameCount & 7;
  if (framePhase == 0) {
    firstXSource = Object_GetById(STUMP);
    firstRandomX = Engine_RandomNext();
    fixedX = firstXSource->x.fixed;
    fixedX +=
        ((((u32)(((firstRandomX << 1) + firstRandomX) << 2)) >> 16) << 16);
    heightSource = Object_GetById(STUMP);
    firstZSource = Object_GetById(STUMP);
    fixedZ = firstZSource->z.fixed;
    fixedZ += PIXELS(6);
    firstRandomSpeed = Engine_RandomNext();
    firstSpeedBand = ((u32)((firstRandomSpeed << 2) + firstRandomSpeed) >> 16);
    // These shifts produce speedBand * 6553: a tenth-pixel falling step.
    firstSpeedTimes13 =
        (((firstSpeedBand << 1) + firstSpeedBand) << 2) + firstSpeedBand;
    firstFallingSpeed = firstSpeedTimes13 << 6;
    firstFallingSpeed -= firstSpeedTimes13;
    firstFallingSpeed <<= 3;
    firstFallingSpeed += firstSpeedBand;
    firstStyle = (u32)(Engine_RandomNext() << 1) >> 16;
    firstFallingSpeed = -firstFallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                 firstFallingSpeed, firstStyle, framePhase);
    framePhase = gFrameCount & 15;
    if (framePhase == 0) {
      secondXSource = Object_GetById(STUMP);
      secondRandomX = Engine_RandomNext();
      fixedX = secondXSource->x.fixed;
      fixedX +=
          ((((u32)(((secondRandomX << 1) + secondRandomX) << 2)) >> 16) << 16);
      heightSource = Object_GetById(STUMP);
      secondZSource = Object_GetById(STUMP);
      fixedZ = secondZSource->z.fixed;
      fixedZ += PIXELS(6);
      secondRandomSpeed = Engine_RandomNext();
      secondSpeedBand =
          ((u32)((secondRandomSpeed << 2) + secondRandomSpeed) >> 16);
      // Keep the same five falling speeds for the second fragment.
      secondSpeedTimes13 =
          (((secondSpeedBand << 1) + secondSpeedBand) << 2) + secondSpeedBand;
      secondFallingSpeed = secondSpeedTimes13 << 6;
      secondFallingSpeed -= secondSpeedTimes13;
      secondFallingSpeed <<= 3;
      secondFallingSpeed += secondSpeedBand;
      secondStyle = (u32)(Engine_RandomNext() << 1) >> 16;
      secondFallingSpeed = -secondFallingSpeed;
      GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
                                   secondFallingSpeed, secondStyle, framePhase);
    }
  }
}

s32 gGomaCavePillar_WitnessFrames;

// Move on the stump: once it is pushed to the edge (column 25), it teeters
// shedding dust, tips over and falls into the room below with a crash that
// shakes the screen.
static void Event_PushStumpOver(void) {
  s32 column;
  s32 zero;
  void (*fragmentCallback)(void);
  s32 fixedX;
  s32 fixedZ;
  struct FieldActor *heightSource;
  struct FieldActor *stump;
  struct FieldActor *spriteActor;
  struct FieldActor *blendActor;
  struct FieldActor *bounceActor, *gravityActor;
  struct FieldActor *movedActor;
  struct FieldActor *zSource, *xSource;
  s32 randomX, randomSpeed, style, speedBand, speedTimes13, fallingSpeed;
  struct FieldActor *overlayActor;

  stump = Object_GetById(STUMP);
  column = stump->x.fixed / PIXELS(16);
  EV_BEGIN();
  if (column == 25) {
    overlayActor = Object_GetById(11);
    zero = 0;
    overlayActor->terrainId = 1;
    spriteActor = Object_GetById(11);
    Engine_ActorSetSpriteFlags(spriteActor, 0);
    Engine_ActorSetChildValue(11, 14);
    blendActor = Object_GetById(11);
    Engine_ObjectSetBlendMode(blendActor, 1);
    Engine_ActorSetPosition(11, PIXELS(414), PIXELS(240));
    WAIT(10);
    // Dust falls while the stump rocks on the edge.
    fragmentCallback = GomaCavePillar_SpawnStumpDust;
    FieldScheduler_AddCallback(fragmentCallback, 3200);
    Engine_AudioPlayCue(SE_141);
    Engine_ActorSetDestinationOffset(STUMP, 1, 0);
    Engine_ActorWaitForMove(STUMP);
    WAIT(10);
    Engine_ActorSetDestinationOffset(STUMP, 2, 0);
    Engine_ActorWaitForMove(STUMP);
    bounceActor = Object_GetById(STUMP);
    bounceActor->bounce = zero;
    gravityActor = Object_GetById(STUMP);
    gravityActor->gravity = PIXELS(3) / 5;
    WAIT(3);
    // It tips over the edge.
    Engine_ActorSetSpeed(STUMP, 5 * PIXELS(1) / 2, PIXELS(1) / 4);
    Engine_AudioPlayCue(SE_288);
    Engine_ActorSetDestination(STUMP, 416, 200);
    movedActor = Object_GetById(STUMP);
    Engine_ActorSetSpriteFlags(movedActor, 0);
    Engine_TaskRemoveCallback(fragmentCallback);
    WAIT(12);
    Engine_AudioPlayCue(SE_189);
    xSource = Object_GetById(STUMP);
    randomX = Engine_RandomNext();
    fixedX = xSource->x.fixed;
    fixedX += ((((u32)(((randomX << 1) + randomX) << 2)) >> 16) << 16);
    heightSource = Object_GetById(STUMP);
    zSource = Object_GetById(STUMP);
    fixedZ = zSource->z.fixed;
    fixedZ += PIXELS(6);
    randomSpeed = Engine_RandomNext();
    speedBand = ((u32)((randomSpeed << 2) + randomSpeed) >> 16);
    // A last piece of dust, with the same tenth-pixel speed bands.
    speedTimes13 = (((speedBand << 1) + speedBand) << 2) + speedBand;
    fallingSpeed = speedTimes13 << 6;
    fallingSpeed -= speedTimes13;
    fallingSpeed <<= 3;
    fallingSpeed += speedBand;
    style = (u32)(Engine_RandomNext() << 1) >> 16;
    fallingSpeed = -fallingSpeed;
    GomaCavePillar_SpawnFragment(fixedX, heightSource->y.fixed, fixedZ, zero,
                                 fallingSpeed, style, zero);
    WAIT(20);
    // It lands below; the shake dies down before the map cells change.
    Engine_AudioPlayCue(SE_CAST);
    Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Engine_MapRenderWaitForValues();
    Engine_ActorSetPosition(STUMP, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_STUMP_FELL);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  EV_END();
}

// Once the east pillar reaches its place (cell 38, 14), it sinks two pixels
// into the floor and six fragments fly out around it.
static void GomaCavePillar_SettleEastPillar(void) {
  s32 x;
  s32 z;

  x = Object_GetById(EAST_PILLAR)->x.fixed / PIXELS(16);
  z = Object_GetById(EAST_PILLAR)->z.fixed / PIXELS(16);
  if (x == 38 && z == 14) {
    Object_GetById(EAST_PILLAR)->y.fixed = -PIXELS(2);
    Object_GetById(EAST_PILLAR)->targetY = Object_GetById(EAST_PILLAR)->y.fixed;
    Engine_AudioPlayCue(SE_188);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 -PIXELS(1) / 2, 0, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
    GomaCavePillar_SpawnFragment(Object_GetById(EAST_PILLAR)->x.fixed,
                                 Object_GetById(EAST_PILLAR)->y.fixed,
                                 Object_GetById(EAST_PILLAR)->z.fixed,
                                 -PIXELS(2) / 5, -PIXELS(2) / 5, 0, PLANE_XZ);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
  }
}

// Move on the east pillar.
static void Event_MoveEastPillar(void) {
  EV_BEGIN();
  GomaCavePillar_SettleEastPillar();
  EV_END();
}

// Walking into the east pillar pushes it a step.
static void Event_PushEastPillar(void) {
  EV_BEGIN();
  StagedActor_AdvancePair();
  GomaCavePillar_SettleEastPillar();
  EV_END();
}

// Walking west at the gap: the leader jumps across it. Later patches use the
// shared forward jump.
static void Event_JumpGapWest(void) {
#if TBS_PATCH_1
  Field_TryJumpForward();
#else
  struct FieldActor *leader;
  struct FieldPosition target;
  struct FieldPosition *destination;
  u8 *motionFlags;
  u8 savedMotionFlags;

  leader = Object_GetById(0);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
  target.x = Object_GetById(0)->x.fixed - PIXELS(32);
  destination = &target;
  destination->y = Object_GetById(0)->y.fixed;
  destination->z = Object_GetById(0)->z.fixed;
  if (FieldObject_CheckMovementCollision(leader, destination) == 0) {
    EV_BEGIN();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *motionFlags &= 0x7e;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(0, *(s16 *)((u8 *)destination + 2),
                              *(s16 *)((u8 *)destination + 10));
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    EV_END();
  }
#endif
}

const struct SceneEvent *Scene_GetEvents(void) {
  return gGomaCavePillar_Events;
}

// While the battle with Forge runs, the man reacts on a timer; once the
// Djinni event is over, it raises event 99.
static void GomaCavePillar_FidgetWhileForgeFights(void) {
  struct EventWork *work;
  s32 frame;

  work = gEventWork;
  frame = ++gGomaCavePillar_WitnessFrames;
  switch (frame) {
  case 60:
    OBJ_DIR(WITNESS, FACING_SOUTHEAST, 0);
    Engine_ActorShowEmote(WITNESS, 2, 0);
    break;
  case 180:
    Engine_ActorStartRepeatedMotion(WITNESS, 3);
    break;
  case 240:
  case 270:
    Engine_ActorJump(WITNESS, 4, 0);
    break;
  case 480:
    OBJ_MODE(WITNESS, ANIM_SHAKE_HEAD);
    break;
  }
  if (gGameState.pendingDjinnEvent == 0)
    work->raisedTrigger = 99;
}

// Raised once Forge is caught: the man is amazed, then walks off.
static void Event_WitnessCheersForgeCaught(void) {
  Engine_TaskRemoveCallback(GomaCavePillar_FidgetWhileForgeFights);
  EV_BEGIN();
  OBJ_EMOTE(WITNESS, EMOTE_IN_FRONT, 30);
  Engine_ActorRunRepeatedMotion(WITNESS, 2);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  MESSAGE_SET(MsgGomaGotWowThatsPrettyImpressive);
  Event_ShowMessage(WITNESS, 0);
  Engine_ActorSetAnimationAndWait(WITNESS, ANIM_NOD);
  WAIT(30);
  Object_GetById(EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  OBJ_SPEED(WITNESS, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(WITNESS, 600, 216);
  Actor_WalkToAndWait(WITNESS, 600, 248);
  Actor_WalkToAndWait(WITNESS, 568, 296);
  OBJ_POS_SET(WITNESS, 0, 0);
  Object_GetById(EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_GOMA_CAVE_WITNESS_LEFT);
  EV_END();
}

// Put the pillars and the stump where the party left them. Entrance 99 shows
// the stump falling again; after a Djinni battle, the man's reaction starts.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  struct EventWork *work;

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 24;
  Object_GetById(STUMP)->collisionFlags |= 16;
  if (FLAG_CHECK(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
    OBJ_POS_SET(WEST_PILLAR, 344, 104);
    GomaCavePillar_CopyWestPillarCells(24);
  } else {
    GomaCavePillar_CopyWestPillarCells(18);
  }
  if (FLAG_CHECK(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
    OBJ_POS_SET(STUMP, 0, 0);
    Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
  }
  if (FLAG_CHECK(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
    OBJ_POS_SET(EAST_PILLAR, 616, 232);
    if (gGameState.entrance == 2 || gGameState.entrance == 3) {
      Object_GetById(EAST_PILLAR)->terrainId = 2;
      actor = Object_GetById(EAST_PILLAR);
      actor->y.fixed--;
      Object_GetById(EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
  }
  // Entrance 99 shows the stump being pushed to the edge and falling.
  if (gGameState.entrance == 99) {
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    OBJ_POS_SET(STUMP, 384, 192);
    WAIT(60);
    Object_GetById(STUMP)->terrainId = 2;
    Actor_MoveToAndWait(STUMP, 408, 192);
    WAIT(60);
    Event_PushStumpOver();
  }
  // Coming back from a Djinni battle: the man reacts until it is over.
  if (gGameState.pendingDjinnEvent != 0) {
    gGomaCavePillar_WitnessFrames = 0;
    Task_AddCallback(GomaCavePillar_FidgetWhileForgeFights, 3200);
  }
  return 0;
}

// One step per facing: X pixels in the high halfword, Z pixels in the low half.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

struct GomaCavePillarFragmentScripts gGomaCavePillar_FragmentScripts = {{
    gGomaCavePillar_FragmentScript0,
    gGomaCavePillar_FragmentScript1,
    gGomaCavePillar_FragmentScript2,
}};

s32 gGomaCavePillar_FragmentScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_FragmentScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
};

s32 gGomaCavePillar_FragmentScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(GomaCavePillar_FinishFragment),
    SC_WAIT(1),
    SC_FREE,
};

struct SceneEntrance gGomaCavePillar_Entrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 2, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 3, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 4, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 5, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { 99, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { SCENE_TABLE_END },
};

u32 gGomaCavePillar_Exits[] = {
    SCENE_EXITS(27),
    SCENE_EXIT(1, 28, 2),
    SCENE_EXIT(2, 26, 1),
    SCENE_EXIT(3, 26, 2),
    SCENE_EXIT(4, 26, 3),
    SCENE_EXIT(5, 28, 3),
    SCENE_EXITS_END,
};

struct ScenePlacement gGomaCavePillar_Placements[] = {
    {211, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {212, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {211, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {223, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {92, FLAG_DJINNI_0 + DJINNI_FORGE, ACTOR_STAND, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {134, FLAG_GOMA_CAVE_WITNESS_LEFT, ACTOR_STAND, PIXELS(628), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gGomaCavePillar_Events[] = {
    { 0x21, 5, CONDITION_ALWAYS, 1 },
    { 0x31, 6, CONDITION_ALWAYS, 2 },
    { 0x31, 7, CONDITION_ALWAYS, 3 },
    { 0x31, 8, CONDITION_ALWAYS, 4 },
    { 0x21, 9, CONDITION_ALWAYS, 5 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_PushEastPillar) },
    { TOUCH_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpGapWest) },
    { EVENT_TALK, FORGE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FightForge) },
    { EVENT_TALK, WITNESS, 0xa8, MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { EVENT_TALK, WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), WITNESS, 0xa8, MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT_SCRIPT(Event_PushStumpOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT_SCRIPT(Event_MoveEastPillar) },
    { SEARCH(SEARCH_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WitnessCheersForgeCaught) },
    { SCENE_EVENTS_END },
};
// clang-format on
