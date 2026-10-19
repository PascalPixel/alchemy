// Gondowan Cave: the soldiers guarding the bridge to Gondowan, Iodem waiting
// there on the way to Babi Lighthouse, and the cave's moving puzzles.
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
#include "songs.h"
#include "event.h"

// The first room's actors: the soldiers who guard the bridge, and Iodem.
#define SOLDIER_1 8
#define SOLDIER_2 9
#define SOLDIER_3 10
#define IODEM 11

// The third room's actors: two objects the party moves, and the one that
// stands in for the second once it has moved.
#define FIRST_OBJECT 8
#define SECOND_OBJECT 9
#define SECOND_OBJECT_MOVED 10

// The leap event also runs after the shared staged-actor movement.
static void Event_MoveSecondObject(void);
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
const struct SceneEntrance *Scene_GetEntrances(void) {
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
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// All cave sections share the exit table.
const u32 *Scene_GetExits(void) { return gGondowanCaveExits; }

// Iodem waits in the first room once he is on his way to Babi Lighthouse.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanCave1) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanCavePlacements1WithIodem;
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

// Each room's events, with Iodem's in the first room when he is there.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_GondowanCave1) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gGondowanCaveEvents1WithIodem;
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

// Walking back (trigger 10) with Iodem in the party: he steps out and
// asks why Isaac is going back. Yes: he waits here for the party's return.
// No: he rejoins the party and Isaac steps away from the trigger.
static void Event_IodemAsksWhyTurningBack(void) {
  struct FieldActor *leader;

  if (FLAG_CHECK(0x98a) == 0 && FLAG_CHECK(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
    EV_BEGIN();
    // Bring Iodem out of the party before opening his question.
    Engine_ActorSetSpeed(IODEM, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(IODEM, leader->x.fixed, leader->z.fixed);
    }
    Engine_ActorSetDestinationOffset(IODEM, -8, 16);
    Engine_ActorWaitForMove(IODEM);
    OBJ_DIR(IODEM, FACING_NORTH + FACING_STEP, 0);
    WAIT(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IODEM, 0);
    MESSAGE_SET(MsgKaragolWhyGoingBackIsaacDo);
    Event_OpenMessage(IODEM, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      // "I see... In that case, I shall wait here for you to return." The
      // saved place becomes Tolbi's.
      Event_ShowMessage(IODEM, 0);
      OBJ_WALK(IODEM, 152, 232);
      GameFlag_Clear(FLAG_IODEM_FOLLOWING_PARTY);
      Engine_ActorWaitForMove(IODEM);
      OBJ_MODE(IODEM, ANIM_STAND);
      gGameState.savedScene = (s32)&SceneId_TolbiInteriors;
      gGameState.savedEntrance = 30;
    } else {
      // "Then we should continue on our way to the lighthouse." He rejoins
      // the party, and Isaac steps away from the trigger.
      gEventWork->message += 1;
      Event_ShowMessage(IODEM, 0);
      OBJ_MODE(IODEM, ANIM_WALK);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(IODEM, leader->x.part.pixel, leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(IODEM);
      OBJ_POS_SET(IODEM, 0, 0);
      WAIT(30);
      OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
      Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
    }
    EV_END();
  }
}

// Talking to Iodem where he waits, or walking up to him (trigger 11): "I've
// been waiting for you, Isaac! We should go to Babi Lighthouse soon." He
// joins the party again.
static void Event_IodemRejoins(void) {
  EV_BEGIN();
  OBJ_FACE(IODEM, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, IODEM, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  WAIT(10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IODEM, 0);
  MESSAGE_SET(MsgKaragolIveBeenWaitingForIsaac);
  MESSAGE_PUT(IODEM, 0);
  OBJ_MODE(IODEM, ANIM_WALK);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0)
      Engine_ActorSetDestination(IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IODEM);
  Engine_ActorSetPosition(IODEM, 0, 0);
  WAIT(20);
  Engine_GameFlagSet(FLAG_IODEM_FOLLOWING_PARTY);
  EV_END();
}

// Talking to the first soldier: "Do you wish to cross into Gondowan?" Yes:
// "I won't stop you, but I wouldn't go unless I had business there." No:
// "That's a good idea. They wouldn't let you into Babi Lighthouse, anyway."
static void Event_SoldierAsksAboutCrossing(void) {
  s32 firstMessage;

  firstMessage = MsgKaragolDoYouWishCrossInto;
  MESSAGE_SET(firstMessage);
  MESSAGE_OPEN(SOLDIER_1, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0) {
        // The shorter answer, without the advice to watch Colosso.
        MESSAGE_SET(firstMessage + 8);
      }
    }
    MESSAGE_PUT(SOLDIER_1, 0);
  } else {
    gEventWork->message++;
    MESSAGE_PUT(SOLDIER_1, 0);
  }
}

// Mind Read on the second soldier: once flag 0x950 is set and until Iodem
// waits here, he grumbles that guard duty made him miss all of Colosso;
// before, that the warriors fight while he guards the bridge.
static void Event_ReadSoldierMind(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0 &&
      Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
    MESSAGE_SET(MsgKaragolWeMissedColossoBecauseWe);
  else
    MESSAGE_SET(MsgKaragolWarriorsHaveBeenFightingWhile);
  MESSAGE_PUT(SOLDIER_2, 0);
}

// Leaving the third room by its first exit (trigger 1). With temporary flag
// 0x204 set, the puzzle's saved flags return to one arrangement first.
static void Event_LeaveThroughEntrance(void) {
  struct EventWork *event;

  event = gEventWork;
  EV_BEGIN();
  if (FLAG_CHECK(FLAG_SCENE_TEMP_04) != 0) {
    GameFlag_Clear(0x9a3);
    GameFlag_Clear(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
    GameFlag_Clear(0x9a4);
    GameFlag_Clear(0x9a6);
    FLAG_SET(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
    FLAG_SET(0x9a4);
  }
  Engine_EventRequestExit(event->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  EV_END();
}

// Walking into the second object from the east or the west (triggers 10
// and 11): look at what is ahead, which starts the push.
static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

// Move on the first object: from column 30 or less it spins and leaps across,
// freeing the cell where it stood.
static void Event_MoveFirstObject(void) {
  Object_GetById(FIRST_OBJECT);
  EV_BEGIN();
  {
    struct FieldActor *actor = Object_GetById(FIRST_OBJECT);

    if ((actor->x.fixed >> 20) <= 30) {
      GondowanCave_RunSpinningLeap(FIRST_OBJECT);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
      Engine_GameFlagSet(FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE);
    }
  }
  EV_END();
}

// Pushing the second object (trigger 50): it moves a step, then may have
// reached its place.
static void Event_PushSecondObject(void) {
  StagedActor_AdvancePair();
  Event_MoveSecondObject();
}

// Hide the second object and show the one that stands in for it in its place.
static inline void GondowanCave_PlaceMovedSecondObject(s32 animation) {
  OBJ_POS_SET(SECOND_OBJECT, 0, 0);
  OBJ_POS_SET(SECOND_OBJECT_MOVED, 696, 288);
  OBJ_MODE(SECOND_OBJECT_MOVED, animation);
}

// Move on the second object, and after each push: past column 42 it drops
// into its place for good.
static void Event_MoveSecondObject(void) {
  EV_BEGIN();
  if ((Object_GetById(SECOND_OBJECT)->x.fixed >> 20) > 42) {
    s32 destinationCellX = 107;
    s32 destinationCellZ = 17;

    Map_CopyCellAttributes(108, 17, 1, 1, destinationCellX, destinationCellZ);
    WAIT(8);
    GondowanCave_PlaceMovedSecondObject(3);
    SE(SE_CAST);
    FLAG_SET(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED);
  }
  EV_END();
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

// The object spins in place, leaps and lands with a shake and a ring of dust,
// then comes to rest a little further on.
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
  Engine_AudioPlayCue(SE_136);
  OBJ_SPEED(actorId, PIXELS(2), PIXELS(1));
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
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(actorId, 3);
  Map_WaitForShakeToSettle();
  WAIT(10);
  OBJ_MODE(actorId, 2);
}

// Unused empty scene callback retained with the overlay's routines.
static UNUSED void GondowanCave_UnusedSceneCallback(void) {}

// Lift on the third room's actor 11 also takes away actor 12 beside it.
static void Event_LiftObject(void) { Engine_ActorSetPosition(12, 0, 0); }

// The first room keeps Iodem out of sight while he travels with the party;
// the third puts the moved objects where the party left them.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;

  if (gGameState.scene == (s32)&SceneId_GondowanCave1) {
    FLAG_SET(FLAG_RETREAT_BLOCKED);
    if (FLAG_CHECK(FLAG_IODEM_FOLLOWING_PARTY) != 0) {
      OBJ_POS_SET(IODEM, 0, 0);
    }
  }
  if (gGameState.scene == (s32)&SceneId_GondowanCave3) {
    if (gGameState.entrance == 1) {
      Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
    }
    if (FLAG_CHECK(FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE) != 0) {
      OBJ_POS_SET(FIRST_OBJECT, 440, 308);
      OBJ_MODE(FIRST_OBJECT, 2);
      Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
    }
    if (FLAG_CHECK(FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED) != 0) {
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

struct ScenePlacement gGondowanCavePlacements1WithIodem[] = {
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    // Iodem
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
    { EVENT_TALK, SOLDIER_1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SoldierAsksAboutCrossing) },
    { EVENT_TALK, SOLDIER_2, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { EVENT_TALK, SOLDIER_3, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_1, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadSoldierMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_3, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveEvents1WithIodem[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IodemAsksWhyTurningBack) },
    { EVENT_TOUCH, 11, FLAG_IODEM_FOLLOWING_PARTY, EVENT_SCRIPT(Event_IodemRejoins) },
    { EVENT_TALK, SOLDIER_1, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { EVENT_TALK, SOLDIER_2, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { EVENT_TALK, SOLDIER_3, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { EVENT_TALK, IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IodemRejoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_1, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_2, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SOLDIER_3, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gGondowanCaveEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

// Temporary flag 0x205 ends Move on the second object.
struct SceneEvent gGondowanCaveEvents3[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughEntrance) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LiftObject) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), FIRST_OBJECT, FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE, EVENT_SCRIPT(Event_MoveFirstObject) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), SECOND_OBJECT, FLAG_SCENE_TEMP_05, EVENT_SCRIPT(Event_MoveSecondObject) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushSecondObject) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};
// clang-format on
