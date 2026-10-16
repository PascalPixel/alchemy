#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/gondowan_cave.h"
#include "unit/scene/common/effect_spawn.h"
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
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "flags.h"

// The leap event also runs after the shared staged-actor movement.
static void GondowanCave_UpdateSecondObjectPosition(void);
static void GondowanCave_RunSpinningLeap(s32 actorId);

// Shared effect routines retain their position before the scene handlers.
#include "common/effect_create.h"

// Particle and movable-object routines precede this scene's entry points.
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// Select the entrance bounds for the current cave section.
const struct SceneEntrance *GondowanCave_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanCave1) {
    return gGondowanCaveEntrances1;
  }
  if (scene == (s32)&SceneId_GondowanCave2) {
    return gGondowanCaveEntrances2;
  }
  if (scene == (s32)&SceneId_GondowanCave3) {
    return gGondowanCaveEntrances3;
  }
  return gGondowanCaveEntrancesOther;
}

// This overlay has no clipping regions.
const struct SceneRegion *GondowanCave_GetRegions(void) { return NULL; }

// All cave sections share the exit table.
const u32 *GondowanCave_GetExits(void) { return gGondowanCaveExits; }

// The first section gains Iodem in its alternate placement table.
const struct ScenePlacement *GondowanCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanCave1) {
    if (Engine_GameFlagIsSet(0x96f) != 0) {
      return gGondowanCavePlacements1Flag96f;
    }
    return gGondowanCavePlacements1;
  }
  if (scene == (s32)&SceneId_GondowanCave2) {
    return gGondowanCavePlacements2;
  }
  if (scene == (s32)&SceneId_GondowanCave3) {
    return gGondowanCavePlacements3;
  }
  return gGondowanCavePlacementsOther;
}

// Match each section's events to its current placement table.
const struct SceneEvent *GondowanCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanCave1) {
    if (Engine_GameFlagIsSet(0x96f) != 0) {
      return gGondowanCaveEvents1Flag96f;
    }
    return gGondowanCaveEvents1;
  }
  if (scene == (s32)&SceneId_GondowanCave2) {
    return gGondowanCaveEvents2;
  }
  if (scene == (s32)&SceneId_GondowanCave3) {
    return gGondowanCaveEvents3;
  }
  return gGondowanCaveEventsOther;
}

// Iodem asks whether Isaac intends to turn back.
static void GondowanCave_AskIodemAboutTurningBack(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(0x98a) == 0 &&
      GameFlag_IsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    Engine_EventBegin();
    // Bring Iodem out of the party before opening his question.
    Engine_ActorSetSpeed(11, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(11, leader->x.fixed, leader->z.fixed);
    }
    Engine_ActorSetDestinationOffset(11, -8, 16);
    Engine_ActorWaitForMove(11);
    Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventSetMessage(MsgKaragolWhyGoingBackIsaacDo);
    Event_OpenMessage(11, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      // Leave him waiting and retain the return entrance in Tolbi.
      Event_ShowMessage(11, 0);
      Actor_WalkTo(11, 152, 232);
      GameFlag_Clear(FLAG_IODEM_FOLLOWING_PARTY);
      Engine_ActorWaitForMove(11);
      Engine_ActorSetAnimation(11, 1);
      gGameState.savedScene = (s32)&SceneId_TolbiInteriors;
      gGameState.savedEntrance = 30;
    } else {
      // Rejoin the party, then move Isaac away from the trigger.
      gEventWork->message += 1;
      Event_ShowMessage(11, 0);
      Engine_ActorSetAnimation(11, 2);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(11, leader->x.part.pixel, leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(11);
      Actor_SetPosition(11, 0, 0);
      Engine_EventWait(30);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
      Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    }
    Engine_EventEnd();
  }
}

// Iodem greets Isaac and resumes travelling with the party.
static void GondowanCave_RejoinIodem(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(11, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 11, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
  Engine_EventSetMessage(MsgKaragolIveBeenWaitingForIsaac);
  Engine_EventShowMessage(11, 0);
  Engine_ActorSetAnimation(11, 2);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0)
      Engine_ActorSetDestination(11, leader->x.part.pixel,
                                 leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(11);
  Engine_ActorSetPosition(11, 0, 0);
  Engine_EventWait(20);
  Engine_GameFlagSet(FLAG_IODEM_FOLLOWING_PARTY);
  Engine_EventEnd();
}

// Ask about crossing into Gondowan, with the alternate Colosso response.
static void GondowanCave_AskAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      if (Engine_GameFlagIsSet(0x96f) == 0) {
        Engine_EventSetMessage(firstMessage + 8);
      }
    }
    Engine_EventShowMessage(8, 0);
  } else {
    gEventWork->message++;
    Engine_EventShowMessage(8, 0);
  }
}

// Mind Read reveals whether the soldier missed Colosso.
static void GondowanCave_ReadSoldierThoughts(void) {
  if (Engine_GameFlagIsSet(0x950) != 0 && Engine_GameFlagIsSet(0x96f) == 0)
    Engine_EventSetMessage(MsgKaragolWeMissedColossoBecauseWe);
  else
    Engine_EventSetMessage(MsgKaragolWarriorsHaveBeenFightingWhile);
  Engine_EventShowMessage(9, 0);
}

// Restore the affected puzzle flags before leaving through the touched exit.
static void GondowanCave_ResetPuzzleAndExit(void) {
  struct EventWork *event;

  event = gEventWork;
  Engine_EventBegin();
  // Temporary bit 4 selects the alternate saved arrangement on departure.
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    GameFlag_Clear(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
    GameFlag_Clear(0x9a4);
    GameFlag_Clear(0x9a6);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
    GameFlag_Set(0x9a4);
  }
  Engine_EventRequestExit(event->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// Probe ahead when walking into either of the paired movable actors.
static void GondowanCave_CheckAheadForMovableObject(void) {
  Engine_LeaderCheckAhead();
}

// Move makes actor 8 leap across and opens its former collision cell.
static void GondowanCave_BeginFirstObjectLeap(void) {
  Object_GetById(8);
  Engine_EventBegin();
  {
    struct FieldActor *actor = Object_GetById(8);

    if ((actor->x.fixed >> 20) <= 30) {
      GondowanCave_RunSpinningLeap(8);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      Engine_GameFlagSet(FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE);
    }
  }
  Engine_EventEnd();
}

// Advance the paired actors, then test the second actor's destination.
static void GondowanCave_PushPairedObject(void) {
  StagedActor_AdvancePair();
  GondowanCave_UpdateSecondObjectPosition();
}

// Hide the movable actor and show its replacement at the saved destination.
static inline void GondowanCave_PlaceMovedSecondObject(s32 animation) {
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, PIXELS(696), PIXELS(288));
  Engine_ActorSetAnimation(10, animation);
}

// Replace actor 9 after it crosses the destination cell boundary.
static void GondowanCave_UpdateSecondObjectPosition(void) {
  Engine_EventBegin();
  if ((Object_GetById(9)->x.fixed >> 20) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    Map_CopyCellAttributes(108, 17, 1, 1, destinationCellX, destinationCellZ);
    Engine_EventWait(8);
    GondowanCave_PlaceMovedSecondObject(3);
    Event_PlayCue(154);
    GameFlag_Set(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
  }
  Engine_EventEnd();
}

// Unused empty actor callback retained with the overlay's routines.
static UNUSED void GondowanCave_UnusedActorCallback(void) {}

// Rotate the sprite during the airborne part of the leap.
static void GondowanCave_RotateLeapingObject(union FieldObject *fieldObject) {
  fieldObject->actor.sprite->rotation -= 0x400;
}

// Wait at most forty frames for the actor to descend to the height limit.
static void GondowanCave_WaitForDescent(struct FieldActor *actor,
                                        s32 heightLimit) {
  s32 framesRemaining = 40;

  while (framesRemaining != 0) {
    Engine_TaskWait(1);
    framesRemaining--;
    if (actor->y.fixed <= heightLimit) {
      break;
    }
  }
}

// Spin, leap to the far side, scatter particles, and settle into place.
static void GondowanCave_RunSpinningLeap(s32 actorId) {
  struct FieldActor *actor;
  u32 stepIndex;
  struct Vector3 particleVelocity;
  s32 angle;

  actor = Object_GetById(actorId);
  // Wind up the spin while horizontal motion is disabled.
  actor->motionFlags = 0;
  for (stepIndex = 0; stepIndex < 9; stepIndex++) {
    Engine_TaskWait(1);
    actor->sprite->rotation -= 0x100;
    actor->x.fixed -= Engine_MathCos(actor->sprite->rotation) / 2;
    actor->targetX = ACTOR_NO_TARGET;
  }
  actor->update = GondowanCave_RotateLeapingObject;
  // Continue spinning through the first jump.
  Engine_AudioPlayCue(136);
  Actor_SetSpeed(actorId, PIXELS(2), PIXELS(1));
  Actor_SetDestination(actorId, 472, 288);
  actor->gravity = 4 * PIXELS(1) / 5;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->terrainId = 0;
  Engine_ActorWaitForMove(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Emit a complete particle fan, including its closing direction.
  for (stepIndex = 0; stepIndex < 17; stepIndex++) {
    angle = stepIndex << 12;
    particleVelocity.x = Engine_MathCos(angle);
    particleVelocity.y = 0;
    particleVelocity.z = Engine_MathSin(angle);
    particleVelocity.x -= particleVelocity.x / 4;
    particleVelocity.z -= particleVelocity.z / 2;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                 particleVelocity.x, particleVelocity.y, particleVelocity.z, 0,
                 0);
  }
  Actor_SetDestination(actorId, 440, 308);
  // Finish the second movement before restoring the resting animation.
  Engine_ActorWaitForMove(actorId);
  GondowanCave_WaitForDescent(actor, PIXELS(32));
  actor->update = 0;
  actor->sprite->rotation = 0x1000;
  Engine_AudioPlayCue(154);
  Engine_ActorSetAnimation(actorId, 3);
  Map_WaitWorkValuesBelow256();
  Engine_EventWait(10);
  Engine_ActorSetAnimation(actorId, 2);
}

// Unused empty scene callback retained with the overlay's routines.
static UNUSED void GondowanCave_UnusedSceneCallback(void) {}

// Lift hides the additional actor at the movable pair.
static void GondowanCave_HideLiftedObject(void) {
  Engine_ActorSetPosition(12, 0, 0);
}

// Restore actor positions and collision cells from the saved puzzle flags.
s32 GondowanCave_Initialize(void) {
  struct FieldActor *actor;

  if (gGameState.scene == (s32)&SceneId_GondowanCave1) {
    GameFlag_Set(0x144);
    if (GameFlag_IsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      Actor_SetPosition(11, 0, 0);
    }
  }
  if (gGameState.scene == (s32)&SceneId_GondowanCave3) {
    if (gGameState.entrance == 1) {
      Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (GameFlag_IsSet(FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE) != 0) {
      Actor_SetPosition(8, PIXELS(440), PIXELS(308));
      Engine_ActorSetAnimation(8, 2);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (GameFlag_IsSet(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED) != 0) {
      GondowanCave_PlaceMovedSecondObject(2);
    }
    actor = Actor_Get(12);
    Engine_ActorSetSpriteFlags(actor, 0);
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

// Pack signed pixel steps with X in the upper halfword and Z in the lower.
// Shift the unsigned representation so westward steps remain defined C.
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

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gGondowanCaveEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gGondowanCaveEntrances1[] = {
    { 1, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
    { 2, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gGondowanCaveEntrances2[] = {
    { 1, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { 2, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { 3, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
    { 4, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gGondowanCaveEntrances3[] = {
    { 1, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { 2, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
    { 3, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { 4, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { 5, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { 6, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
    { SCENE_TABLE_END },
};

u32 gGondowanCaveExits[] = {
    SCENE_EXITS(152),
    SCENE_EXIT(1, 2, 25),
    SCENE_EXIT(2, 2, 54),
    SCENE_EXIT(0, 157, 0),
    SCENE_EXIT(1, 158, 2),
    SCENE_EXIT(2, 157, 3),
    SCENE_EXIT(3, 157, 2),
    SCENE_EXIT(4, 158, 5),
    SCENE_EXIT(0, 158, 0),
    SCENE_EXIT(1, 2, 26),
    SCENE_EXIT(2, 157, 1),
    SCENE_EXIT(3, 158, 4),
    SCENE_EXIT(4, 158, 3),
    SCENE_EXIT(5, 157, 4),
    SCENE_EXIT(6, 2, 41),
    SCENE_EXITS_END,
};

struct ScenePlacement gGondowanCavePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCavePlacements1[] = {
    { 0x96, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCavePlacements1Flag96f[] = {
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCavePlacements2[] = {
    { 0x127, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gGondowanCavePlacements3[] = {
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x127, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gGondowanCaveEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_AskAboutCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_ReadSoldierThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveEvents1Flag96f[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_AskIodemAboutTurningBack) },
    { EVENT_TOUCH, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT_SCRIPT(GondowanCave_RejoinIodem) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_RejoinIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

// Temporary bit 5 gates Move on the second movable object.
struct SceneEvent gGondowanCaveEvents3[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_ResetPuzzleAndExit) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), 11, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_HideLiftedObject) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE, EVENT_SCRIPT(GondowanCave_BeginFirstObjectLeap) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_SCENE_TEMP_05, EVENT_SCRIPT(GondowanCave_UpdateSecondObjectPosition) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_CheckAheadForMovableObject) },
    { TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_CheckAheadForMovableObject) },
    { 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(GondowanCave_PushPairedObject) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};
// clang-format on
