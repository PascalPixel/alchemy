#include "game_state.h"
#include "version.h"
#include "engine_call.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/vale_cave.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"

#include "field_obj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
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
#include "field_effect.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"

#include "fixed_math.h"
#include "songs.h"

// The cave above Vale: three rooms of Psynergy puzzles, where the Jupiter
// Djinni Kite hides. Djinn_JoinParty(ACTOR_KITE, 3, 4) gives Jupiter Djinni 4,
// and the fifth Jupiter Djinni the text names is Kite.

// Kite is placement 8 of the second room; the objects moved with Move or
// pushed are placements 9 to 11 of the second and third rooms.
#define ACTOR_KITE 8
#define WESTERN_MOVABLE_OBJECT 9
#define EASTERN_MOVABLE_OBJECT 10
#define SLIDING_OBJECT 11
#define OBJECT_SINK_FRAMES 68
#define OBJECT_SINK_STEP 0x8000
#define ACTOR_LANDING_TIMEOUT 60
#define FIRST_DJINNI_ROUTE_TRIGGER 45

// The flag of Kite, set once it is caught.
#define FLAG_KITE (FLAG_DJINNI_0 + 3 * 20 + 4)

// FLAG_SCENE_TEMP_00 and _01: in the second room, the western and eastern
// objects sunk into their holes, or, in Kite's part of it, its first and second
// escape. FLAG_SCENE_TEMP_02 to _05: in the third room, the object pushed off
// the ledge, the object pushed over its edge, and the object slid north, ending
// on the northern or the southern row (row 20). Group flag 0x301: the sliding
// objects are at their eastern end. Group flag 0x302: Kite has run behind the
// ledge and keeps away from the party. Both last while the party stays in the
// cave.

// Forward declarations.
static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row);
static void ValeCave_ApplyEntryState(void);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

// Reaching Kite: it joins the party.
static void Event_TalkToKite(void) { Djinn_JoinParty(ACTOR_KITE, 3, 4); }

// Wait for an actor to land, bounded by a one-second timeout.
static void ValeCave_WaitForActorLanding(struct FieldActor *actor) {
  s32 framesRemaining = ACTOR_LANDING_TIMEOUT;

  while (framesRemaining != 0) {
    Engine_TaskWait(1);
    framesRemaining--;
    if (actor->y.fixed <= actor->terrainHeight)
      break;
  }
  actor->y.fixed = actor->terrainHeight;
}

// Keep an actor facing the party leader after the escape sequence.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void ValeCave_FaceActorTowardLeader(union FieldObject *fieldObject) {
#else
static s32 ValeCave_FaceActorTowardLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 distanceZ = leader->z.fixed - actor->z.fixed;
  s32 distanceX = leader->x.fixed - actor->x.fixed;

  actor->facing = ArcTan2(distanceZ, distanceX);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

#ifdef UBFIX
#define VALE_CAVE_FACE_LEADER_CALLBACK ValeCave_FaceActorTowardLeader
#else
#define VALE_CAVE_FACE_LEADER_CALLBACK                                         \
  ((void (*)(union FieldObject *))ValeCave_FaceActorTowardLeader)
#endif

// Select entrance positions and camera bounds for the current cave room.
const struct SceneEntrance *ValeCave_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCave1) {
    return gValeCave_Entrances1;
  }
  if (scene == (s32)&SceneId_ValeCave2) {
    return gValeCave_Entrances2;
  }
  if (scene == (s32)&SceneId_ValeCave3) {
    return gValeCave_Entrances3;
  }
  return gValeCave_EntrancesOther;
}

// The cave rooms have no additional region table.
const struct SceneRegion *ValeCave_GetRegions(void) { return NULL; }

// Return the shared packed exits for all cave rooms.
const u32 *ValeCave_GetExits(void) { return gValeCave_Exits; }

// Select the current room's actors and movable objects.
const struct ScenePlacement *ValeCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCave1) {
    return gValeCave_Placements1;
  }
  if (scene == (s32)&SceneId_ValeCave2) {
    return gValeCave_Placements2;
  }
  if (scene == (s32)&SceneId_ValeCave3) {
    return gValeCave_Placements3;
  }
  return gValeCave_Placements1;
}

// Reveal ends: the hidden tile is hidden again.
static void Event_HideRevealedTile(void) {
  Map_CopyCells(16, 15, 1, 1, 15, 15);
}

// Reveal shows a hidden tile in the first room.
static void Event_RevealHiddenTile(void) {
  Map_CopyCells(16, 17, 1, 1, 15, 15);
}

// Lower one object while emitting a particle each frame around its opening.
static inline void
ValeCave_SinkObjectWithParticles(s32 actorId, s32 baseX, s32 baseZ,
                                 struct EffectOptions *particleOptions) {
  u32 frame;

  for (frame = 0; frame < OBJECT_SINK_FRAMES; frame++) {
    s32 particleX = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + baseX;
    s32 particleZ = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + baseZ;

    Effect_Spawn(particleX, 0, particleZ, 0, 0, 0,
                 EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, particleOptions);
    Object_GetById(actorId)->y.fixed -= OBJECT_SINK_STEP;
    Engine_EventWait(1);
  }
}

// Move on the western object: once over its hole, Isaac steps aside and the
// object sinks into the floor in a cloud of dust.
static void Event_MoveWestObjectIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct FieldActor *leftEffect;
  struct FieldActor *rightEffect;

  Engine_EventBegin();
  if (Object_GetById(WESTERN_MOVABLE_OBJECT)->x.fixed >> 20 == 23) {
    // Move the leader aside before the object starts descending.
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 360, 664);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
    Object_GetById(WESTERN_MOVABLE_OBJECT)->x.fixed += PIXELS(2);
    leftEffect = OverlayObject_PrepareObject(
        Object_GetById(WESTERN_MOVABLE_OBJECT)->x.fixed, 0,
        Object_GetById(WESTERN_MOVABLE_OBJECT)->z.fixed + PIXELS(52), 241);
    rightEffect = OverlayObject_PrepareObject(
        Object_GetById(WESTERN_MOVABLE_OBJECT)->x.fixed + PIXELS(16), 0,
        Object_GetById(WESTERN_MOVABLE_OBJECT)->z.fixed + PIXELS(52), 241);
    Object_GetById(WESTERN_MOVABLE_OBJECT)->motionFlags = 0;
    // Surround the descending object with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 0x9999;
    particleOptions->startScaleY = 0x9999;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(WESTERN_MOVABLE_OBJECT, PIXELS(368),
                                     PIXELS(624), particleOptions);
    Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
    Object_GetById(WESTERN_MOVABLE_OBJECT)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    Object_GetById(WESTERN_MOVABLE_OBJECT)->y.fixed = -PIXELS(8);
    Engine_ActorSetAnimation(WESTERN_MOVABLE_OBJECT, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_EventWait(30);
  }
  Engine_EventEnd();
}

// Move on the eastern object: once over its hole, it sinks into the floor.
static void Event_MoveEastObjectIntoHole(void) {
  struct EffectOptions options;
  struct EffectOptions *particleOptions;
  struct FieldActor *leftEffect;
  struct FieldActor *rightEffect;

  Engine_EventBegin();
  if (Object_GetById(EASTERN_MOVABLE_OBJECT)->x.fixed >> 20 == 27) {
    leftEffect = OverlayObject_PrepareObject(
        Object_GetById(EASTERN_MOVABLE_OBJECT)->x.fixed - PIXELS(8), 0,
        Object_GetById(EASTERN_MOVABLE_OBJECT)->z.fixed + PIXELS(52), 241);
    rightEffect = OverlayObject_PrepareObject(
        Object_GetById(EASTERN_MOVABLE_OBJECT)->x.fixed + PIXELS(8), 0,
        Object_GetById(EASTERN_MOVABLE_OBJECT)->z.fixed + PIXELS(52), 241);
    Object_GetById(WESTERN_MOVABLE_OBJECT)->motionFlags = 0;
    // Surround the descending object with one particle per frame.
    particleOptions = &options;
    particleOptions->startScaleX = 0x9999;
    particleOptions->startScaleY = 0x9999;
    particleOptions->palette = 7;
    Engine_AudioPlayCue(SE_216);
    ValeCave_SinkObjectWithParticles(EASTERN_MOVABLE_OBJECT, PIXELS(432),
                                     PIXELS(656), particleOptions);
    Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
    Object_GetById(EASTERN_MOVABLE_OBJECT)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
    Object_GetById(EASTERN_MOVABLE_OBJECT)->y.fixed = -PIXELS(8);
    Engine_ActorSetAnimation(EASTERN_MOVABLE_OBJECT, 2);
    Engine_ObjectDispatchRelease(leftEffect);
    Engine_ObjectDispatchRelease(rightEffect);
    Engine_EventWait(30);
  }
  Engine_EventEnd();
}

// Pushing the eastern object: it moves a cell, and sinks if over its hole.
static void Event_PushEastObjectIntoHole(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  Event_MoveEastObjectIntoHole();
}

// Walking south off the ledge: Isaac hops down if the cell below is free.
static s32 Event_HopDownLedge(s32 unusedArgument) {

  union FieldCoordinate landingPosition[3];
  struct FieldActor *leader;
  u8 *motionFlags;
  s32 savedMotionFlags;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  landingPosition[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = (leader->z.fixed & -0x100000) + 0x280000;
#else
  landingPosition[0].fixed = leader->x.fixed;
  landingPosition[1].fixed = leader->y.fixed;
  landingPosition[2].fixed = leader->z.fixed + 0x200000;
#endif
  // Keep the normal motion flags while staging the hop.
  if (FieldObject_CheckMovementCollision(leader, landingPosition) == 0) {
    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_152);
    Object_SetMode(leader, 7);
    leader->speed = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *motionFlags &= 126;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(0, landingPosition[0].part.pixel,
                              landingPosition[2].part.pixel);
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}

// Coming near Kite: the camera finds it; it notices the party and hops away
// up the ledge.
static void Event_KiteRunsOff(void) {

  struct FieldActor *djinni;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_00);
    Engine_EventBegin();
    Camera_SetSpeed(Q16_ONE, 0x2000);
    Engine_CameraFollowActor(ACTOR_KITE, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_KITE, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_KITE, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_KITE, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KITE, 792, 248);
    Engine_AudioPlayCue(SE_152);
    djinni = Actor_Get(ACTOR_KITE);
    djinni->velocityY = 0x80000;
    Actor_WalkToAndWait(ACTOR_KITE, 792, 280);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
    Engine_EventWait(30);
    Engine_EventEnd();
  }
}

// Coming near again: Kite runs behind the ledge and turns to watch the party.
static void Event_KiteRunsBehindLedge(void) {

  struct FieldActor *djinni;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) != 0) {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Set(0x302);
      Engine_EventBegin();
      Actor_SetAttachedEffect(ACTOR_KITE, 0x102);
      Engine_ActorRunRepeatedMotion(ACTOR_KITE, 2);
      Engine_EventWait(20);
      Actor_SetSpeed(ACTOR_KITE, PIXELS(2), PIXELS(1));
      Actor_WalkToAndWait(ACTOR_KITE, 760, 280);
      Actor_WalkToAndWait(ACTOR_KITE, 760, 312);
      Actor_WalkToAndWait(ACTOR_KITE, 792, 312);
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_KITE, FACING_NORTH, 20);
      djinni = Actor_Get(ACTOR_KITE);
      djinni->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      Engine_EventEnd();
    }
  }
}

// Walking around the ledge: Kite keeps to the far side of it, unless Halt has
// stopped it.
static void Event_KiteKeepsAway(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 routeIndex;
  const u8 *walkTargetBytes;
  s32 targetZOffset;
  struct EventWork *event;

  event = gEventWork;
  if (Engine_GameFlagIsSet(0x302) != 0) {
    if (gGameState.pausedObject != ACTOR_KITE) {
      // Preserve the speed reads before resolving this route point.
      routeIndex = event->touchedTrigger;
      djinni = Object_GetById(ACTOR_KITE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->speed = leader->speed;
      djinni = Object_GetById(ACTOR_KITE);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      djinni->acceleration = leader->acceleration;
      routeIndex -= FIRST_DJINNI_ROUTE_TRIGGER;
      // X/Z pairs occupy eight bytes. Plain pair indexing changes six
      // instruction words in the English build, so keep separate offsets.
      walkTargetBytes = (const u8 *)gValeCave_WalkTargets;
      routeIndex <<= 3;
      targetZOffset = routeIndex + sizeof(s32);
      Engine_ActorWalkTo(ACTOR_KITE,
                         *(const s32 *)(walkTargetBytes + routeIndex),
                         *(const s32 *)(walkTargetBytes + targetZOffset));
    }
  }
}

// Halt on Kite: it stops where it stands.
static void Event_HaltKite(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_KITE, 0);
  Engine_EventEnd();
}

// Pushing an object along the ledge: at its edge the object falls off,
// opening the way.
static void Event_PushObjectOverEdge(void) {
  struct FieldActor *movedObject;
  s32 stationaryMotion;
  s32 clearedRows;
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    if ((probe.positionX >> 20) == 17) {
      Engine_ActorSetAnimation(probe.actorSlot, 3);
      stationaryMotion = 0;
      Object_GetById(probe.actorSlot)->motionFlags = stationaryMotion;
      movedObject = Object_GetById(probe.actorSlot);
      movedObject->bounce = stationaryMotion;
      Engine_ActorSetDestinationOffset(probe.actorSlot, -12, 0);
      Engine_ActorWaitForMove(probe.actorSlot);
      Engine_ActorSetAnimation(probe.actorSlot, 3);
      Engine_ActorSetSpritePriority(EASTERN_MOVABLE_OBJECT, 3);
      Object_GetById(probe.actorSlot)->motionFlags =
          ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Engine_ActorSetDestinationOffset(probe.actorSlot, -6, 0);
      ValeCave_WaitForActorLanding(Object_GetById(probe.actorSlot));
      Engine_ActorSetAnimation(probe.actorSlot, 8);
      {
        struct FieldActor *movedObject = Object_GetById(probe.actorSlot);

        movedObject->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      }
      clearedRows = 4;
      StagedActor_FillGridAttributeRectangle(0, (probe.positionX >> 20),
                                             ((probe.positionZ >> 20) - 2), 1,
                                             clearedRows, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(2, (probe.positionX >> 20),
                                             ((probe.positionZ >> 20) - 2), 1,
                                             clearedRows, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, 2, stationaryMotion);
      StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, clearedRows,
                                             stationaryMotion);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
      Engine_AudioPlayCue(SE_240);
    }
  }
  Engine_EventEnd();
}

// Pushing the western object east: it falls off the ledge, opening the way.
static void Event_PushObjectOffLedge(void) {

  struct FieldActor *pushedObject;
  s32 stationaryMotion;

  Engine_EventBegin();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_EventWait(15);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
  Engine_EventWait(4);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_239);
  Actor_SetSpeed(WESTERN_MOVABLE_OBJECT, 0x8000, 0x1999);
  Engine_ActorSetAnimation(WESTERN_MOVABLE_OBJECT, 2);
  stationaryMotion = 0;
  Object_GetById(WESTERN_MOVABLE_OBJECT)->motionFlags = stationaryMotion;
  pushedObject = Actor_Get(WESTERN_MOVABLE_OBJECT);
  pushedObject->bounce = stationaryMotion;
  Engine_ActorSetDestinationOffset(WESTERN_MOVABLE_OBJECT, 12, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorWaitForMove(WESTERN_MOVABLE_OBJECT);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_213);
  Engine_ActorSetAnimation(WESTERN_MOVABLE_OBJECT, 3);
  Object_GetById(WESTERN_MOVABLE_OBJECT)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_ActorSetDestinationOffset(WESTERN_MOVABLE_OBJECT, 6, 0);
  ValeCave_WaitForActorLanding(Actor_Get(WESTERN_MOVABLE_OBJECT));
  Engine_ActorSetAnimation(WESTERN_MOVABLE_OBJECT, 8);
  Engine_ActorSetSpritePriority(WESTERN_MOVABLE_OBJECT, 3);
  Object_GetById(WESTERN_MOVABLE_OBJECT)->priorityFlags = 2;
  StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
  StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  Event_PlayCue(SE_240);
  Engine_EventEnd();
}

// Pushing the object north: it slides a cell, opening the way.
static void Event_PushObjectNorth(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    // Slide into the new cell before replacing the collision footprint.
    Engine_ActorSetAnimation(SLIDING_OBJECT, 3);
    Actor_SetSpeed(SLIDING_OBJECT, 0x4000, 0x8000);
    Engine_ActorSetDestinationOffset(SLIDING_OBJECT, 0, -16);
    Engine_EventWait(45);
    Engine_AudioPlayCue(SE_240);
    Engine_ActorSetAnimation(SLIDING_OBJECT, 8);
    {
      struct FieldActor *slidingObject = Object_GetById(SLIDING_OBJECT);
      s32 clearedRows = 2;
      s32 emptyAttributes = 0;

      slidingObject->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      StagedActor_FillGridAttributeRectangle(0, 13, (probe.positionZ >> 20) - 1,
                                             4, clearedRows, emptyAttributes);
    }
    if ((probe.positionZ >> 20) == 20) {
      Engine_GameFlagSet(FLAG_SCENE_TEMP_05);
    } else {
      Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
      {

        Map_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        Map_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      }
    }
  }
  Engine_EventEnd();
}

// Select the current room's touch, Psynergy and search bindings.
const struct SceneEvent *ValeCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ValeCave1) {
    return gValeCave_Events1;
  }
  if (scene == (s32)&SceneId_ValeCave2) {
    return gValeCave_Events2;
  }
  if (scene == (s32)&SceneId_ValeCave3) {
    return gValeCave_Events3;
  }
  return gValeCave_EventsOther;
}

// Choose the entry transition and restore the room's puzzle state.
s32 ValeCave_Initialize(void) {
  s32 *request = &gEventWork->startTransition;

  *request = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_ValeCave1) {
    *request = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_TaskWait(1);
    Engine_ActorSetSpritePriority(SLIDING_OBJECT, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  ValeCave_ApplyEntryState();
  return 0;
}

// The first room's exits play the exit sound before leaving.
static void Event_LeaveRoom(void) {
  s32 exitId = gEventWork->touchedTrigger;

  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(exitId);
}

// Rebuild the passage cells around the sliding object's saved side.
static void ValeCave_UpdateSlidingObjectPassage(void) {
  Map_CopyCellAttributes(0, 34, 13, 3, 23, 34);

  if (Engine_GameFlagIsSet(0x301) != 0) {
    ValeCave_CenterActorOnCell(SLIDING_OBJECT, 35, 35);
    Map_CopyCellAttributes(24, 34, 1, 3, 23, 34);
  } else {
    ValeCave_CenterActorOnCell(SLIDING_OBJECT, 23, 35);
    Map_CopyCellAttributes(24, 34, 1, 3, 35, 34);
  }
}

// Shared source is included here in ROM order.
#include "common/object_pairmove.h"

// Walking east onto the sliding objects: they slide to the eastern end.
static void Event_SlideObjectsEast(void) {

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(SLIDING_OBJECT, 112, 0);
  SceneActor_PlacePairAtOffset(SLIDING_OBJECT, 80, 0);
  Engine_GameFlagSet(0x301);
  Engine_TaskWait(2);
  ValeCave_UpdateSlidingObjectPassage();
  Engine_AudioPlayCue(SE_289);
}

// Walking west onto the sliding objects: they slide back to the western end.
static void Event_SlideObjectsWest(void) {

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(SLIDING_OBJECT, -112, 0);
  SceneActor_PlacePairAtOffset(SLIDING_OBJECT, -80, 0);
  Engine_GameFlagClear(0x301);
  Engine_TaskWait(2);
  ValeCave_UpdateSlidingObjectPassage();
  Engine_AudioPlayCue(SE_289);
}

// Center an actor on a map cell and give it underfoot priority.
static void ValeCave_CenterActorOnCell(s32 actorId, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    Engine_ActorSetSpritePriority(actorId, 3);
    actor->terrainId = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = (column << 20) + 0x80000;
    actor->z.fixed = (row << 20) + 0x80000;
  }
}

// Restore the Djinni route and completed movable-object puzzles on entry.
static void ValeCave_ApplyEntryState(void) {
  s32 northernObjectMoved;

  // Each room reuses the temporary flags for its own puzzle state.
  if (gGameState.scene == (s32)&SceneId_ValeCave2) {
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      Engine_ActorSetSpritePriority(15, 3);
      Engine_ActorSetSpritePriority(13, 3);
      OverlayObject_CreateConfiguredObject(PIXELS(120), 0, PIXELS(232), 223);
      break;
    case 5:
    case 6:
    case 7:
      if (Engine_GameFlagIsSet(FLAG_KITE) != 0) {
        break;
      }
      if (Engine_GameFlagIsSet(0x302) == 0) {
        break;
      }
      Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
      if (gGameState.entrance == 5) {
        Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
      }
      Engine_TaskWait(1);
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        break;
      }
      Actor_SetPosition(ACTOR_KITE, PIXELS(792), PIXELS(280));
      Object_GetById(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
      break;
    case 8:
    case 9:
    case 10:
      OverlayObject_CreateConfiguredObject(PIXELS(642), 0, PIXELS(552), 20);
      Map_CopyCellAttributes(23, 34, 13, 3, 0, 34);
      ValeCave_UpdateSlidingObjectPassage();
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
        Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
        Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
      }
      break;
    }
  } else if (gGameState.scene == (s32)&SceneId_ValeCave3) {
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
        StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
        StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
      } else {
        FieldScene_RedrawActorFootprint(WESTERN_MOVABLE_OBJECT);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03) != 0) {
        StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
        StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
      } else {
        FieldScene_RedrawActorFootprint(EASTERN_MOVABLE_OBJECT);
      }
      northernObjectMoved = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05);
      if (northernObjectMoved != 0) {
        StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
      } else if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04) != 0) {
        StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2,
                                               northernObjectMoved);
        Map_CopyCellAttributes(14, 17, 2, 1, 14, 16);
        Map_CopyCellAttributes(14, 13, 1, 1, 14, 15);
      } else {
        FieldScene_RedrawActorFootprint(SLIDING_OBJECT);
        Engine_ActorSetSpritePriority(SLIDING_OBJECT, 3);
      }
      break;
    }
  }
}

// Touching the sliding objects restarts their animation.
static void Event_TouchSlidingObjects(void) {
  Engine_ActorSetAnimation(SLIDING_OBJECT, 1);
  Engine_ActorSetAnimation(SLIDING_OBJECT, 2);
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

// Pixel step by direction: X in the upper halfword, Z in the lower.
// UB: negative pixel steps cannot be shifted as signed values.
#ifdef UBFIX
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))
#else
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

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

s32 gValeCave_WalkTargets[] = {
    760, 296,
    760, 280,
    776, 280,
    792, 280,
    808, 280,
    824, 280,
    824, 296,
    824, 312,
    808, 312,
    792, 312,
    776, 312,
    760, 312,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gValeCave_EntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_Entrances1[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { 1, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
    { 3, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_Entrances2[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 328, 0, 392, FACING_NORTH, 0, 48, 32, 544, 416, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 160, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { 3, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { 4, CONDITION_ALWAYS, 504, 0, 136, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
    { 5, CONDITION_ALWAYS, 792, 0, 360, FACING_NORTH, 0, 624, 16, 944, 384, 0 },
    { 6, CONDITION_ALWAYS, 680, 0, 248, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
    { 7, CONDITION_ALWAYS, 904, 0, 232, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
    { 8, CONDITION_ALWAYS, 408, 0, 872, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
    { 9, CONDITION_ALWAYS, 488, 0, 744, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
    { 10, CONDITION_ALWAYS, 632, 0, 552, FACING_SOUTH, 0, 312, 472, 704, 896, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gValeCave_Entrances3[] = {
    { 0, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { 2, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
    { 3, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
    { 4, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { 5, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
    { SCENE_TABLE_END },
};

u32 gValeCave_Exits[] = {
    SCENE_EXITS(93),
    SCENE_EXIT(1, 6, 4),
    SCENE_EXIT(2, 94, 1),
    SCENE_EXIT(3, 6, 6),
    SCENE_EXIT(0, 94, 0),
    SCENE_EXIT(1, 93, 2),
    SCENE_EXIT(2, 94, 5),
    SCENE_EXIT(3, 94, 8),
    SCENE_EXIT(4, 95, 1),
    SCENE_EXIT(5, 94, 2),
    SCENE_EXIT(6, 94, 9),
    SCENE_EXIT(7, 95, 3),
    SCENE_EXIT(8, 94, 3),
    SCENE_EXIT(9, 94, 6),
    SCENE_EXIT(10, 95, 4),
    SCENE_EXIT(0, 95, 0),
    SCENE_EXIT(1, 94, 4),
    SCENE_EXIT(2, 95, 5),
    SCENE_EXIT(3, 94, 7),
    SCENE_EXIT(4, 94, 10),
    SCENE_EXIT(5, 95, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gValeCave_Placements1[] = {
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeCave_Placements2[] = {
    { 0x5d, FLAG_KITE, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(344), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(456), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(376), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(422), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeCave_Placements3[] = {
    { 0x5d, FLAG_KITE, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(240), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeCave_EventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeCave_Events1[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveRoom) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeCave_Events2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { TOUCH_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideObjectsEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideObjectsWest) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopDownLedge) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TouchSlidingObjects) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TouchSlidingObjects) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 24, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_PushEastObjectIntoHole) },
    { EVENT_TOUCH, 40, FLAG_KITE, EVENT_SCRIPT(Event_KiteRunsOff) },
    { EVENT_TOUCH, 41, FLAG_KITE, EVENT_SCRIPT(Event_KiteRunsBehindLedge) },
    { EVENT_TOUCH, 45, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 46, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 47, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 48, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 49, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 50, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 51, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 52, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 53, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 54, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 55, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { EVENT_TOUCH, 56, FLAG_KITE, EVENT_SCRIPT(Event_KiteKeepsAway) },
    { 0x7, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKite) },
    { SEARCH(SEARCH_CHEST), 100, 0xfc2, FIND_ITEM(0xcc) }, // the Halt Gem
    { SEARCH(SEARCH_CHEST), 101, 0xfc3, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealHiddenTile) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideRevealedTile) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), WESTERN_MOVABLE_OBJECT, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MoveWestObjectIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), EASTERN_MOVABLE_OBJECT, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MoveEastObjectIntoHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KITE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HaltKite) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gValeCave_Events3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushObjectOverEdge)},
    {TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushObjectOffLedge)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushObjectNorth)},
    {SCENE_EVENTS_END},
};
