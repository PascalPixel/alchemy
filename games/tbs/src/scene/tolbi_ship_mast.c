#include "script.h"
#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/tolbi_ship_mast.h"
#include "unit/object/object.h"
#include "unit/field/field.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "item_ids.h"
#include "flags.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "macros.h"

// Scene sequences and the mast's per-frame updates.
static void TolbiShipMast_SetupHiddenLeaderLookout(void);
static void TolbiShipMast_SetupVisibleLeaderLookout(void);
static void TolbiShipMast_AnnounceClearCourse(void);
static void TolbiShipMast_RunAvastScene(void);
static void TolbiShipMast_ShowMonsters(void);
static void TolbiShipMast_InitializeDriftingActor(s32 actorId);
static void TolbiShipMast_RunLandingSequence(void);
static void TolbiShipMast_AnnounceLand(void);
static void TolbiShipMast_UpdateSway(void);
static s32 TolbiShipMast_GetFlagPairDifference(void);
static s32 TolbiShipMast_GetFlagGroupValue(u32 groupIndex);

s32 gTolbiShipMast_DriftY __attribute__((section(".bss")));
s32 gTolbiShipMast_DriftX __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayY __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayUnused __attribute__((section(".bss")));
s32 gTolbiShipMast_CameraCenter[2] __attribute__((section(".bss")));
s32 gTolbiShipMast_CameraOrigin[2] __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayX __attribute__((section(".bss")));

// Keeps the effect actor relative to the saved camera origin and rotates it.
static s32
TolbiShipMast_FollowCameraAndRotate(struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  s32 *position = ((struct MapScrollWork *)gMapWork[0])->origin;
  s32 *origin = gTolbiShipMast_CameraOrigin;
  s32 *center = gTolbiShipMast_CameraCenter;
  s32 cameraX = *position++;
  s32 cameraY = *position;

  object->x.fixed = center[0] + (cameraX - origin[0]);
  object->y.fixed = center[1] + (cameraY - origin[1]) / 2;
  object->sprite->rotation += 0x600;
  return 0;
}

// A random sample of 6 or 9 turns the lookout to either side of north.
static s32
TolbiShipMast_RandomizeLookoutFacing(struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  u32 sample = (u32)(Engine_RandomNext() << 6) >> 16;

  if (sample == 6) {
    s32 value = FACING_NORTH + FACING_STEP;

    object->facing = value;
  } else if (sample == 9) {
    s32 value = FACING_NORTHWEST + FACING_STEP;

    object->facing = value;
  }

  return 1;
}

// Grows both axes by the same amount until the actor reaches full scale.
static void TolbiShipMast_GrowActorScale(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor->scaleX < Q16_ONE) {
    actor->scaleX += 160;
    actor->scaleY += 160;
  }
}

// Randomly reverses horizontal drift and vertical bobbing at their limits.
static s32 TolbiShipMast_SwayActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 *state = &actor->timer;
  s32 next;

  // State 9 zeros drift or vertical position; 0 and 1 alternate direction.
  if (*state == 9) {
    actor->riseBaseY = 0;
  } else if (*state != 0) {
    actor->riseBaseY -= (u32)(Engine_RandomNext() << 11) >> 16;
    if (actor->riseBaseY < -3 * PIXELS(1) / 4) {
      next = 0;
      *state = next;
    }
  } else {
    actor->riseBaseY += (u32)(Engine_RandomNext() << 11) >> 16;
    if (actor->riseBaseY > 3 * PIXELS(1) / 4) {
      next = 1;
      *state = next;
    }
  }
  if (actor->x.fixed > PIXELS(40) && actor->x.fixed < PIXELS(320)) {
    actor->x.fixed += actor->riseBaseY;
  }
  state = (s16 *)&actor->work;
  if (*state == 9) {
    actor->y.fixed = 0;
  } else if (*state != 0) {
    actor->y.fixed -= ((u32)Engine_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed < 0) {
      next = 0;
      *state = next;
    }
  } else {
    actor->y.fixed += ((u32)Engine_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed > PIXELS(16)) {
      next = 1;
      *state = next;
    }
  }
  return 1;
}

// Shrinks both axes; once small enough, clears the actor's position and speed.
static s32
TolbiShipMast_ShrinkAndStopActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct AnimationObject *sprite = actor->sprite;

  sprite->image.priority = 3;
  actor->speed = PIXELS(2);
  actor->acceleration = PIXELS(1);
  if (actor->scaleX > Q16_ONE / 16) {
    actor->scaleX -= Q16_ONE / 64;
    actor->scaleY -= Q16_ONE / 64;
  } else {
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    SET_VELOCITY(actor, 0, 0, 0);
  }
  return 1;
}

// Returns the entrances from the ship's deck and scripted sequences.
const struct SceneEntrance *TolbiShipMast_GetEntrances(void) {
  return gTolbiShipMast_Entrances;
}

// Returns the ladder's three-dimensional activation region.
const struct SceneRegion *TolbiShipMast_GetRegions(void) {
  return gTolbiShipMast_Regions;
}

// Returns exits to the deck and the continuing ship scenes.
const u32 *TolbiShipMast_GetExits(void) { return gTolbiShipMast_Exits; }

// Returns the lookout's initial placement table.
const struct ScenePlacement *TolbiShipMast_GetPlacements(void) {
  return gTolbiShipMast_Placements;
}

// Returns the ladder, lookout dialogue and Anchor Charm search events.
const struct SceneEvent *TolbiShipMast_GetEvents(void) {
  return gTolbiShipMast_Events;
}

// Touching the ladder starts the descent.
static void TolbiShipMast_ClimbDownLadder(void) { Field_ClimbDown(); }

// Gives the Anchor Charm once either discovery flag permits the search.
static void TolbiShipMast_FindAnchorCharm(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED) != 0 ||
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    Engine_EventBegin();
    Engine_ItemShowFound(ITEM_ANCHOR_CHARM, 3);
    Engine_PartyGiveItem(ITEM_ANCHOR_CHARM, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND);
    Engine_EventEnd();
  }
}

// Starts the ship sway when allowed, then runs this entrance's mast sequence.
s32 TolbiShipMast_Initialize(void) {
  struct FieldActor *leader;

  Engine_GameFlagSet(0x144);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if ((Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING) != 0 ||
       Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
    gTolbiShipMast_SwayX = (u16)Engine_RandomNext();
    gTolbiShipMast_SwayY = (u16)Engine_RandomNext();
    FieldScheduler_AddCallback(TolbiShipMast_UpdateSway, 0xc80);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
    Actor_SetPosition(8, PIXELS(164), PIXELS(328));
  }
  // The ship's scripted entrances resume their matching lookout scenes.
  switch (gGameState.entrance) {
  case 1:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      Field_ClimbUp();
      leader->y.fixed = PIXELS(56);
      Camera_MoveTo(-1, -1, -1, 0);
      Engine_TaskWait(1);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Engine_MapRedraw();
      Engine_TaskWait(1);
      Engine_EventEnd();
    }
    break;
  case 10:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      TolbiShipMast_SetupVisibleLeaderLookout();
    } else {
      TolbiShipMast_SetupHiddenLeaderLookout();
    }
    break;
  case 11:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = 30;
    TolbiShipMast_RunAvastScene();
    break;
  case 12:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = 30;
    TolbiShipMast_ShowMonsters();
    break;
  case 13:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = 30;
    TolbiShipMast_RunLandingSequence();
    break;
  case 14:
    TolbiShipMast_AnnounceLand();
    break;
  }
  return 0;
}

// Frames the lookout with the leader moved out of view, then announces course.
static void TolbiShipMast_SetupHiddenLeaderLookout(void) {
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_MoveTo(PIXELS(164), PIXELS(64), PIXELS(321), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  TolbiShipMast_AnnounceClearCourse();
  Engine_EventEnd();
}

// Keeps the leader visible for the course announcement.
static void TolbiShipMast_SetupVisibleLeaderLookout(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(164), PIXELS(321));
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leader, 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipMast_AnnounceClearCourse();
  Engine_EventEnd();
}

// The lookout scans both sides, announces a clear course and leaves by exit 10.
static void TolbiShipMast_AnnounceClearCourse(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(8, 164, 321);
  Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_WalkToAndWait(8, 164, 334);
  Engine_ActorJump(8, 4, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipShipsCourseClear);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(10);
}

// Stages four approaching actors and the lookout's "Avast!" reaction.
static void TolbiShipMast_RunAvastScene(void) {
  s32 *placement = ((struct MapScrollWork *)gMapWork[0])->origin;
  struct FieldActor *actor;
  struct EventWork **event;
  s32 x;
  s32 zero = 0;
  s32 scale;
  void (*update)(union FieldObject *);

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(gTolbiShipMast_StagingObjects);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Actor_SetActionScript(8, gTolbiShipMast_LookoutActions);
  event = (struct EventWork **)(gMapWork + (HEAP_SLOT_EVENT - HEAP_SLOT_MAP));
  (*event)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  // Save the camera origin before the rotating approach effect begins.
  Engine_EventWait(20);
  gTolbiShipMast_CameraOrigin[0] = *placement++;
  gTolbiShipMast_CameraOrigin[1] = *placement;
  Actor_SetPosition(9, PIXELS(80), PIXELS(210));
  x = PIXELS(80);
  Object_GetById(9)->motionFlags = zero;
  gTolbiShipMast_CameraCenter[0] = x;
  gTolbiShipMast_CameraCenter[1] = zero;
  Actor_SetActionScript(9, gTolbiShipMast_FollowCameraActions);
  Engine_EventWait(20);
  Engine_AudioPlayCue(29);
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Engine_ActorStop(8);
  Engine_TaskWait(1);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 0);
  Engine_ActorFaceDirection(8, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventSetMessage(MsgTolbiShipAvast);
  Engine_EventShowMessageAndWait(8, 0, 10);
  // Three trailing actors grow from half scale while converging on the mast.
  Actor_SetPosition(10, x, PIXELS(210));
  Actor_SetPosition(11, x, PIXELS(210));
  Engine_ActorSetPosition(12, x, PIXELS(210));
  Engine_ActorSetSpritePriority(10, 3);
  Engine_ActorSetSpritePriority(11, 3);
  Engine_ActorSetSpritePriority(12, 3);
  Engine_ActorSetChildValue(10, 3);
  Engine_ActorSetChildValue(11, 3);
  Engine_ActorSetChildValue(12, 3);
  // Grow the three trailing actors as they approach the lookout.
  actor = Object_GetById(10);
  scale = Q16_ONE / 2;
  update = TolbiShipMast_GrowActorScale;
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Object_GetById(11);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Object_GetById(12);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  Engine_TaskWait(1);
  Actor_SetSpeed(10, 13 * PIXELS(1) / 25, 13 * PIXELS(1) / 50);
  Actor_SetSpeed(11, 9 * PIXELS(1) / 20, 9 * PIXELS(1) / 40);
  Actor_SetSpeed(12, 3 * PIXELS(1) / 5, 3 * PIXELS(1) / 10);
  Actor_SetDestination(10, 128, 345);
  Actor_SetDestination(11, 136, 330);
  Engine_ActorSetDestination(12, 156, 340);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(8, 2);
  Actor_WalkToAndWait(8, 164, 344);
  Engine_ActorJump(8, 4, 10);
  Engine_ActorJump(8, 6, 40);
  Engine_ActorStartRepeatedMotion(8, 3);
  Engine_EventShowMessageAndWait(8, 0, 20);
  (*event)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(11);
  Engine_EventEnd();
}

// Clears the movement left by a drifting script before its staged approach.
static inline void TolbiShipMast_StopDriftingActor(struct FieldActor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(actor, 0, 0, 0);
  actor->riseBaseY = 0;
}

// Give each of the seven approaching actors its own random drift and bob.
static inline void TolbiShipMast_InitializeDriftingGroup(void) {
  TolbiShipMast_InitializeDriftingActor(9);
  TolbiShipMast_InitializeDriftingActor(10);
  TolbiShipMast_InitializeDriftingActor(11);
  TolbiShipMast_InitializeDriftingActor(12);
  TolbiShipMast_InitializeDriftingActor(13);
  TolbiShipMast_InitializeDriftingActor(14);
  TolbiShipMast_InitializeDriftingActor(15);
}

// Stop the drifting actions and give the group its departure walking speed.
static inline void TolbiShipMast_PrepareGroupDeparture(void) {
  Engine_ActorStop(9);
  Engine_ActorStop(10);
  Engine_ActorStop(11);
  Engine_ActorStop(12);
  Engine_ActorStop(13);
  Engine_ActorStop(14);
  Engine_ActorStop(15);
  Actor_SetSpeed(9, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(10, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(11, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(12, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(13, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(14, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(15, PIXELS(3), 3 * PIXELS(1) / 2);
}

// Repeats actor 9's approach before the lookout reports monsters.
static void TolbiShipMast_ShowMonsters(void) {
  struct FieldActor *approachingActor;
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(0);
  Engine_ActorSetSpriteFlags(leader, 0);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(8, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(120);
  // Repeat the foreground approach while the rest of the group keeps drifting.
  approachingActor = Object_GetById(9);
  Engine_ActorStop(9);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Engine_EventWait(20);
  Actor_SetSpeed(9, PIXELS(8), PIXELS(4));
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(144),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(204), PIXELS(124),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(144), 0, PIXELS(169));
  Engine_ActorStop(8);
  Engine_TaskWait(1);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 3, 60);
  Actor_SetSpeed(9, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(9);
  Engine_EventWait(20);
  Actor_SetActionScript(8, gTolbiShipMast_LookoutActions);
  Engine_EventWait(120);
  Engine_ActorStop(9);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Engine_EventWait(20);
  Actor_SetSpeed(9, PIXELS(8), PIXELS(4));
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(144),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  Actor_SetSpeed(9, PIXELS(5), 5 * PIXELS(1) / 2);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(114),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(204), PIXELS(124),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  Engine_ObjectSetPosition(approachingActor, PIXELS(144), 0, PIXELS(169));
  Engine_ActorStop(8);
  Engine_TaskWait(1);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 3, 60);
  Actor_SetSpeed(9, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(9);
  // The lookout recognizes the approaching group and reports monsters.
  Engine_ActorJump(8, 4, 20);
  Engine_ActorJump(8, 6, 40);
  Event_PlayCue(29);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Engine_EventSetMessage(MsgTolbiShipMonsters2);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 0);
  Actor_WalkToAndWait(8, 164, 344);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(12);
  Engine_EventEnd();
}

// Initializes a drifting actor, retaining the random draws' original order.
static void TolbiShipMast_InitializeDriftingActor(s32 actorId) {
  struct FieldActor *actor;
  u32 randomValue;

  actor = Object_GetById(actorId);
  Engine_ActorSetSpritePriority(actorId, 1);
  actor->motionFlags = 0;
  actor->timer = (u32)Engine_RandomNext() >> 15;
  actor->work = (u32)Engine_RandomNext() >> 15;
  randomValue = Engine_RandomNext();
  randomValue <<= 2;
  randomValue >>= 16;
  randomValue <<= 16;
  randomValue += PIXELS(6);
  actor->y.fixed = randomValue;
  randomValue = Engine_RandomNext();
  actor->riseBaseY = ((randomValue * 3 << 13) >> 16) - 3 * PIXELS(1) / 16;
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Actor_SetActionScript(actorId, gTolbiShipMast_DriftActions);
}

// Moves the drifting group aside, then stages the two landing actors.
static void TolbiShipMast_RunLandingSequence(void) {
  u32 frame;
  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_LandingObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(8, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(300);
  Event_PlayCue(147);
  Engine_EventWait(100);
  // Clear the approaching group from the foreground before the landing reveal.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetDestination(9, 0, 100);
  Actor_SetDestination(10, 60, 100);
  Actor_SetDestination(11, 120, 100);
  Actor_SetDestination(12, 180, 100);
  Actor_SetDestination(13, 240, 100);
  Actor_SetDestination(14, 320, 100);
  Actor_SetDestination(15, 380, 100);
  Engine_EventWait(40);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(20);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(15, 0, 0);
  Engine_EventWait(100);
  // Grow the first landing actor into view, then drop the second beside it.
  actor = Object_GetById(18);
  actor->scaleX = Q16_ONE / 10;
  actor->scaleY = Q16_ONE / 10;
  Actor_SetPosition(18, PIXELS(172), PIXELS(340));
  Engine_ActorStop(8);
  Engine_TaskWait(1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_PlayCue(29);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  // Grow actor 18 over 32 frames before staging actor 17 beside it.
  for (frame = 0; frame < 32; frame++) {
    actor->scaleX += Q16_ONE / 20;
    actor->scaleY += Q16_ONE / 20;
    Engine_TaskWait(1);
  }
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(8, 2);
  Actor_WalkToAndWait(8, 168, 340);
  Actor_WalkToAndWait(8, 200, 340);
  Actor_FaceDirection(8, FACING_WEST, 0);
  actor = Object_GetById(17);
  actor->scaleX = 23 * Q16_ONE / 20;
  actor->scaleY = 23 * Q16_ONE / 20;
  actor->x.fixed = PIXELS(172);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(340);
  actor->facing = 0;
  actor->bounce = 2 * Q16_ONE / 5;
  actor->gravity = PIXELS(3);
  Engine_EventWait(20);
  Engine_ActorJump(8, 6, 20);
  Event_PlayCue(147);
  Engine_EventWait(20);
  Actor_SetActionScript(8, gTolbiShipMast_WaveActions);
  Engine_EventWait(80);
  // Let the landing actor bounce, then move both actors out of the shot.
  Engine_ActorSetSpritePriority(17, 1);
  Actor_SetSpeed(17, PIXELS(1), PIXELS(1) / 2);
  actor->bounce = Q16_ONE / 10;
  actor->gravity = 7 * PIXELS(1) / 10;
  Event_PlayCue(153);
  actor->velocityY = PIXELS(8);
  Actor_SetDestination(17, 132, 360);
  Actor_SetDestination(18, 132, 360);
  Engine_EventWait(40);
  Actor_SetPosition(17, 0, 0);
  actor = Object_GetById(8);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  actor->facing = FACING_SOUTH + FACING_STEP;
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(13);
  Engine_EventEnd();
}

// Walks the seven actors away, then the lookout announces land.
static void TolbiShipMast_AnnounceLand(void) {
  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(8, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(400);
  // Each actor takes its own departure path while shrinking out of view.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetActionScript(9, gTolbiShipMast_EnsembleWalk9);
  Actor_SetActionScript(10, gTolbiShipMast_EnsembleWalk10);
  Actor_SetActionScript(11, gTolbiShipMast_EnsembleWalk11);
  Actor_SetActionScript(12, gTolbiShipMast_EnsembleWalk12);
  Actor_SetActionScript(13, gTolbiShipMast_EnsembleWalk13);
  Actor_SetActionScript(14, gTolbiShipMast_EnsembleWalk14);
  Actor_SetActionScript(15, gTolbiShipMast_EnsembleWalk15);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(8, 3);
  Actor_SetAttachedEffect(8, 258);
  Engine_EventWait(120);
  Engine_ActorStartRepeatedMotion(8, 1);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 60);
  Actor_SetSpeed(8, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(8, 164, 344);
  Engine_ActorJump(8, 4, 10);
  Engine_ActorJump(8, 6, 20);
  Engine_EventSetMessage(MsgTolbiShipLandHo);
  Event_ShowMessageAndWait(8, 0, 20);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  // The flag-group score chooses the continuing scene's exit.
  gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
  gGameState.savedEntrance = 2;
  if (TolbiShipMast_GetFlagPairDifference() == 11) {
    Engine_EventRequestExit(15);
  } else {
    Engine_EventRequestExit(14);
  }
  Engine_EventEnd();
}

// Adds horizontal and vertical sway to the camera origin each frame.
static void TolbiShipMast_UpdateSway(void) {
  s32 *position = ((struct MapScrollWork *)gMapWork[0])->origin;
  s32 swayX = Engine_MathCos(gTolbiShipMast_SwayX);
  s32 swayY = Engine_MathSin(gTolbiShipMast_SwayY);

  *position++ += swayX;
  swayY *= 4;
  *position += swayY;
  gTolbiShipMast_DriftX += swayX;
  gTolbiShipMast_DriftY += swayY;
  gTolbiShipMast_SwayX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
  gTolbiShipMast_SwayY += (u32)(Engine_RandomNext() << 9) >> 16;
  gTolbiShipMast_SwayX &= 0xffff;
  gTolbiShipMast_SwayY &= 0xffff;
}

// Subtracts the odd flag groups' values from the even groups' values.
static s32 TolbiShipMast_GetFlagPairDifference(void) {
  s32 evenGroups;
  s32 oddGroups;

  evenGroups = TolbiShipMast_GetFlagGroupValue(0);
  evenGroups += TolbiShipMast_GetFlagGroupValue(2);
  oddGroups = TolbiShipMast_GetFlagGroupValue(1);
  oddGroups += TolbiShipMast_GetFlagGroupValue(3);
  return evenGroups - oddGroups;
}

// Returns the value of the first set flag in one of four nine-flag groups.
static s32 TolbiShipMast_GetFlagGroupValue(u32 groupIndex) {
  s32 firstFlag = 0;
  u32 flagIndex;

  switch (groupIndex) {
  case 0:
    firstFlag = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    firstFlag = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    firstFlag = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    firstFlag = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (flagIndex = 0; flagIndex <= 8; flagIndex++) {
    if (Engine_GameFlagIsSet(firstFlag + flagIndex) != 0)
      return gTolbiShipMast_FlagValues[flagIndex];
  }
  return 0;
}

s32 gTolbiShipMast_FollowCameraActions[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 32),
    SC_CALL(TolbiShipMast_FollowCameraAndRotate),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_CALL(TolbiShipMast_FollowCameraAndRotate),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gTolbiShipMast_LookoutActions[] = {
    SC_CALL(TolbiShipMast_RandomizeLookoutFacing),
};

s32 gTolbiShipMast_DriftActions[] = {
    SC_CALL(TolbiShipMast_SwayActor),
};

s32 gTolbiShipMast_WaveActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -5 * Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gTolbiShipMast_EnsembleWalk9[] = {
    SC_MOVE(PIXELS(72), 0, PIXELS(216)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_EnsembleWalk10[] = {
    SC_MOVE(PIXELS(132), 0, PIXELS(242)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_EnsembleWalk11[] = {
    SC_MOVE(PIXELS(88), 0, PIXELS(166)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_EnsembleWalk12[] = {
    SC_MOVE(PIXELS(220), 0, PIXELS(134)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_EnsembleWalk13[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(228)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_EnsembleWalk14[] = {
    SC_MOVE(PIXELS(282), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

// clang-format off
u32 gTolbiShipMast_EnsembleWalk15[] = {
    SC_MOVE(PIXELS(154), 0, PIXELS(135)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
    // Unidentified words following the script terminator.
    0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
    0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

struct SceneEntrance gTolbiShipMast_Entrances[] = {
    { 0, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 11, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 12, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 13, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 14, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneRegion gTolbiShipMast_Regions[] = {
    {176, 16, 348, 192, 32, 364, -1, 1},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTolbiShipMast_Exits[] = {
    SCENE_EXITS(110),
    SCENE_EXIT(1, 109, 4),
    SCENE_EXIT(10, 111, 10),
    SCENE_EXIT(11, 111, 13),
    SCENE_EXIT(12, 109, 13),
    SCENE_EXIT(13, 109, 14),
    SCENE_EXIT(14, 109, 17),
    SCENE_EXIT(15, 109, 19),
    SCENE_EXITS_END,
};

struct ScenePlacement gTolbiShipMast_Placements[] = {
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_StagingObjects[] = {
    { 0x11e, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_EnsembleObjects[] = {
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(169), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(134), 0, PIXELS(189), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(124), 0, PIXELS(209), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(129), 0, PIXELS(174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(139), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(149), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(154), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(420), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_LandingObjects[] = {
    { 0xc5, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gTolbiShipMast_Events[] = {
    { TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipMast_ClimbDownLadder) },
    { EVENT_TALK, 8, FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
    { EVENT_TALK, 8, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipLookoutCannotRow) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandMonstersUnknown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipSeaTooVast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
    { SEARCH(SEARCH_UNNAMED), 11, FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND, EVENT_SCRIPT(TolbiShipMast_FindAnchorCharm) },
    { SCENE_EVENTS_END },
};

u32 gTolbiShipMast_FlagValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on
