/// @file
/// McCoy's Palace (McCoyPalaceGarden, scene 34), outside the palace of Lord
/// McCoy, the lord of Bilibin. The palace itself (McCoyPalace, scene 33,
/// mccoy_palace.c) has the same area name. Workers build a new palace here
/// for Lady McCoy, who waits among them, and two guards keep the gate of
/// McCoy's own.
///
/// Where it falls in the story: the people of Kolima have been turned into
/// trees, and Bilibin calls it the curse of the holy tree, the tree of Kolima
/// Forest that McCoy had cut down to build his wife a grand palace. McCoy is
/// hiring warriors to end the curse, and his guards let in only brave ones.
/// Before the curse is lifted (FLAG_KOLIMA_CURSE_LIFTED clear), the workers
/// speak of the lumber from Kolima that vanished and of the barricade they
/// built on the road to Kolima, and Lady McCoy, who had the tree cut down for
/// her palace, calls herself the victim: she only wants to live in a big
/// palace.
///
/// Talking to the east guard (Event_TalkToEastGuard) is the way in. Until the
/// party is admitted (FLAG_MCCOY_PALACE_ADMITTED), he asks whether they are
/// up to meeting his lord; on yes the guards confer, the gate opens
/// (Garden_OpenPalaceGate, which sets FLAG_SCENE_TEMP_00) and the west guard
/// goes in ahead. Walking north into the open gate (trigger 2,
/// Event_EnterPalace) takes exit 2 into the palace, its entrance 1. Admitted
/// but not yet inside the palace, the party that comes back from Bilibin
/// (which sets place flag 0x304) is scolded once (FLAG_SCENE_TEMP_01): McCoy
/// is angry because he could not find them.
///
/// In the palace McCoy turns the party away as too young, and his captain
/// lets slip that the barricade may not need a key
/// (FLAG_MCCOY_PALACE_BARRICADE_HINT); after that the guards will tell no more.
/// Once the curse is lifted the building has stopped and Lady McCoy complains
/// that McCoy loves the holy tree more than her. The first time the party
/// arrives then, until McCoy has rewarded them
/// (FLAG_MCCOY_PALACE_PARTY_REWARDED), the guards stop them
/// (Event_GuardsArrestParty): witnesses saw them in the forest, and the guards
/// repeat the accusation until the player admits breaking the curse, then put
/// them under arrest and march them to McCoy (exit 10, the palace's entrance
/// 29), where he rewards them. Afterwards the guards admire the party.
///
/// Setup (Garden_ApplyProgress) places the guards by the same flags and floats
/// a Nut over the grounds until Catch takes it
/// (FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT). A dry well can be searched. Exit 1
/// leads back to Bilibin (Bilibin, scene 30, entrance 2). The patched
/// editions (TBS_PATCH_3) add one more actor, sprite 0xf8 (XXX: what it is is
/// not known).

#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace_garden.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mccoy_palace_garden.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "flags.h"
#include "facing_object.h"
#include "songs.h"

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared. Its use is tagged.
#ifdef UBFIX
#define MCCOY_GARDEN_SET_ACTOR_POSITION Engine_ActorSetPosition
#else
#define MCCOY_GARDEN_SET_ACTOR_POSITION ((s32 (*)())Engine_ActorSetPosition)
#endif

enum {
  ACTOR_WEST_WORKER = 8,
  ACTOR_NORTH_WORKER = 9,
  ACTOR_LADY_MCCOY = 10,
  ACTOR_WEST_GUARD = 11,
  ACTOR_EAST_GUARD = 12,
  ACTOR_FLOATING_NUT = 13
};

static void Garden_ApplyProgress(void);
static void Event_GuardsArrestParty(void);
static void Garden_OpenPalaceGate(void);

#include "common/object_facing.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceGardenEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceGardenExits; }

/// The people outside McCoy's Palace. Once McCoy has turned the party away,
/// the east guard turns back after talking, and once the curse is lifted Lady
/// McCoy faces the party.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) != 0)
      gMcCoyPalaceGardenPlacements[4].talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
      gMcCoyPalaceGardenPlacements[2].talkFacing = TALK_FACE_PARTY;
    return gMcCoyPalaceGardenPlacements;
  }
  return gMcCoyPalaceGardenPlacementsOther;
}

/// McCoy's Palace, outside: Catch on the Nut floating over the grounds gives
/// it to the party.
static void Event_CatchFloatingNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, outside: searching the dry well.
static void Event_SearchWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgBilibinUponCloserInspectionSeemsDried, 1);
  Engine_EventEnd();
}

/// What the party can talk to, search and catch outside McCoy's Palace.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    return gMcCoyPalaceGardenEvents;
  }
  return gMcCoyPalaceGardenEventsOther;
}

/// McCoy's Palace, outside, before the curse is lifted: the north worker asks
/// whether the party has seen the barricade they built on the road to Kolima.
static void Event_TalkToNorthWorker(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveYouSeenBarricadeWe);
  Engine_EventAskYesNo(ACTOR_NORTH_WORKER, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, outside, before the curse is lifted: Lady McCoy asks
/// whether the party blames her for the curse: on yes she calls herself the
/// victim, on no she calls the curse a superstition, and either way she wants
/// her palace.
static void Event_TalkToLadyMcCoy(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinTellingMeImResponsibleFor);
  Engine_EventAskYesNo(ACTOR_LADY_MCCOY, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, outside, after the reward: the west guard asks whether he
/// can become as fine a warrior as the leader.
static void Event_TalkToWestGuard(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoThinkCanBecomeAs);
  Engine_EventAskYesNo(ACTOR_WEST_GUARD, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, outside, until McCoy turns the party away: the east guard
/// asks whether they are brave enough to meet his lord and, on yes, the gate
/// opens and the west guard walks in; once they are admitted, he reminds them
/// that McCoy is a difficult man, and scolds them once for keeping him
/// waiting.
static void Event_TalkToEastGuard(void) {
  Engine_EventBegin();
  // Once admitted, the party only receives the guard's current reminder.
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    if (Engine_GameFlagIsSet(0x304) != 0) {
      if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
        Engine_EventSetMessage(MsgBilibinWhere);
        Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | 7, 40);
        Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 10);
        Engine_ActorRunRepeatedMotion(ACTOR_EAST_GUARD, 2);
        Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
      }
      Engine_EventSetMessage(MsgBilibinMasterQuiteCranky);
      Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
      Engine_EventSetMessage(MsgBilibinLordMccoyDifficult);
      Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
      goto guard;
    }
  } else {
    // On the first visit, the guards confer before opening the gate.
    Engine_EventSetMessage(MsgBilibinLordOnlyMeet);
    Engine_EventOpenMessage(ACTOR_EAST_GUARD, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      gEventWork->message++;
      Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 10);
      if (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed < PIXELS(270)) {
        Actor_SetSpeed(ACTOR_EAST_GUARD, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 346, 274);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 282);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
      }
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHWEST + FACING_STEP, 20);
      Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 20);
      Engine_ActorStartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 10);
      Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | 8, 60);
      Engine_ActorStartRepeatedMotion(ACTOR_EAST_GUARD, 1);
      Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 20);
      Engine_ActorSetAnimation(ACTOR_WEST_GUARD, 3);
      Engine_ActorSetAnimationAndWait(ACTOR_EAST_GUARD, 3);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 20);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTHEAST + FACING_STEP, 0);
      Engine_ActorSetSpeed(ACTOR_EAST_GUARD, 0x10000, 0x8000);
      Object_GetById(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
      Engine_EventWait(1);
      Object_GetById(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
      Actor_SetSpeed(ACTOR_WEST_GUARD, 0x9999, 0x4ccc);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 263);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 252);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 10);
      Garden_OpenPalaceGate();
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 246);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which loads the actor id after the two zeros; as a call by its
       * void declaration, 1 of 233 instructions differ in tbs-en. */
      MCCOY_GARDEN_SET_ACTOR_POSITION(ACTOR_WEST_GUARD, 0, 0);
      /* FAKEMATCH(one-pass): the block keeps r0 last among the call above's
         arguments; as a plain call, 1 of 233 instructions differ in tbs-en. */
      do {
        Engine_GameFlagSet(FLAG_MCCOY_PALACE_ADMITTED);
      } while (0);
    guard:
      FieldObject_SetTargetAndCallback(
          ACTOR_EAST_GUARD, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
          gGuardWatchesParty);
    } else {
      Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
    }
  }
  Engine_EventEnd();
}

/// McCoy's Palace, outside, trigger 2: walking north into the palace gate opens
/// it if it is still shut, and the leader walks into the palace.
static void Event_EnterPalace(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    Garden_OpenPalaceGate();
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(MCCOY_PALACE_GARDEN_TO_MCCOY_PALACE);
  Engine_EventEnd();
}

/// Sets up the grounds outside McCoy's Palace: the west worker drawn in
/// front, and the guards, Lady McCoy and the arrest as the story stands.
s32 Scene_Initialize(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *actor;

  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  actor = Object_GetById(ACTOR_WEST_WORKER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 1;

  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    Garden_ApplyProgress();
  }

  return 0;
}

/// The guards stand where the party left them; once the curse is lifted,
/// Lady McCoy stands before the palace and, until McCoy has rewarded the
/// party, the guards arrest them.
static void Garden_ApplyProgress(void) {
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  }
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_FLOATING_NUT);
  }
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    Actor_SetPosition(ACTOR_WEST_GUARD, PIXELS(308), PIXELS(263));
    Engine_ActorSetPosition(ACTOR_EAST_GUARD, PIXELS(346), PIXELS(263));
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0) {
      if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
        Engine_ActorSetPosition(ACTOR_WEST_GUARD, 0, 0);
        FieldObject_SetTargetAndCallback(
            ACTOR_EAST_GUARD, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gGuardWatchesParty);
      }
    }
  }
  // Lady McCoy's new place and the arrest come only after the curse lifts.
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(ACTOR_LADY_MCCOY, PIXELS(224), PIXELS(292));
    Actor_FaceDirection(ACTOR_LADY_MCCOY, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(ACTOR_WEST_WORKER, FACING_EAST, 0);
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
      Event_GuardsArrestParty();
    }
  }
}

/// McCoy's Palace, outside, on entry once Kolima's curse is lifted: the guards
/// stop the party, ask whether they broke the curse of the holy tree until
/// they answer yes, and march them into the palace to McCoy.
static void Event_GuardsArrestParty(void) {

  // Bring the leader into view before the guards interrupt.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Camera_MoveTo(0x1400000, -1, 0x1400000, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(320), PIXELS(372));
  Engine_EventOpenScreen();
  Engine_CameraSetSpeed(0x3333, 0x666);
  Engine_CameraMoveTo(0x1400000, -1, 0x1220000, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 320, 310);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT, 60);
  Engine_EventSetMessage(MsgBilibinOhItS);
  Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_EAST_GUARD, 2);
  Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT, 60);
  Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_GUARD, 1);
  Engine_EventOpenMessage(ACTOR_EAST_GUARD, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Refusals repeat the accusation until the player acknowledges it.
  while (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
    Engine_ActorShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT, 60);
    Engine_EventSetMessage(MsgBilibinNotTrueWitnesses);
    Engine_EventShowMessageAndWait(ACTOR_EAST_GUARD, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_EAST_GUARD, 2);
    Engine_EventOpenMessage(ACTOR_EAST_GUARD, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_WEST_GUARD, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  Engine_EventSetMessage(MsgBilibinUnderArrest);
  Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 314, 280);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_EventShowMessageAndWait(ACTOR_WEST_GUARD, 0, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_ADMITTED) == 0) {
    Engine_ActorSetSpeed(ACTOR_EAST_GUARD, 0x10000, 0x8000);
    Object_GetById(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
    Engine_EventWait(1);
    {
      struct FieldActor *guard = Object_GetById(ACTOR_EAST_GUARD);

      guard->steeringFlags = ACTOR_FACES_MOTION | guard->steeringFlags;
    }
  }
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0xcccc, 0x6666);
  // Escort Isaac through the opened gate before changing to the palace.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_WEST_GUARD, 328, 262);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 278);
  Engine_ActorSetAnimation(ACTOR_WEST_GUARD, 1);
  Garden_OpenPalaceGate();
  Engine_EventWait(40);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 242);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 242);
  Engine_ActorSetPosition(ACTOR_WEST_GUARD, 0, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(MCCOY_PALACE_GARDEN_TO_MCCOY_PALACE_EAST);
  Engine_EventEnd();
}

/// The gate of McCoy's Palace swings open.
static void Garden_OpenPalaceGate(void) {
  Engine_AudioPlayCue(SE_188);
  Engine_MapAnimateCells(gPalaceGateOpenSteps, 52, 11);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

#include "common/object_orbit.h"

// clang-format off
/// The gate opening: three steps of cells, 5 frames each.
u16 gPalaceGateOpenSteps[] = {
    MAP_CELL_STEP(34, 25, 1, 3, 5), MAP_CELL_STEP(35, 25, 1, 3, 5), MAP_CELL_STEP(36, 25, 1, 3, 5), MAP_CELL_STEPS_END,
};

/// A guard turns to keep facing the party.
s32 gGuardWatchesParty[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gMcCoyPalaceGardenEntrances[] = {
    { MCCOY_PALACE_GARDEN_FROM_BILIBIN, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MCCOY_PALACE_GARDEN_FROM_MCCOY_PALACE, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gMcCoyPalaceGardenExits[] = {
    SCENE_EXITS(McCoyPalaceGarden),
    SCENE_EXIT(MCCOY_PALACE_GARDEN_TO_BILIBIN, Bilibin, BILIBIN_FROM_MCCOY_PALACE_GARDEN),
    SCENE_EXIT(MCCOY_PALACE_GARDEN_TO_MCCOY_PALACE, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GARDEN_WEST),
    SCENE_EXIT(MCCOY_PALACE_GARDEN_TO_MCCOY_PALACE_EAST, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GARDEN_EAST),
    SCENE_EXITS_END,
};

struct ScenePlacement gMcCoyPalaceGardenPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gMcCoyPalaceGardenPlacements[] = {
    { 0x87, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(103), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x87, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(135), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x45, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(128), 0, PIXELS(327), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(320), 0, PIXELS(263), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(334), 0, PIXELS(263), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x16, FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT, 7,
        PIXELS(216), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if TBS_PATCH_3
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(388), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
    { SCENE_TABLE_END },
};

struct SceneEvent gMcCoyPalaceGardenEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gMcCoyPalaceGardenEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterPalace) },
    { EVENT_TALK, ACTOR_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
    { EVENT_TALK, ACTOR_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
    { EVENT_TALK, ACTOR_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToNorthWorker) },
    { EVENT_TALK, ACTOR_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
    { EVENT_TALK, ACTOR_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToLadyMcCoy) },
    { EVENT_TALK, ACTOR_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
    { EVENT_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_HINT, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
    { EVENT_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinICantTellYouAnythingElse) },
    { EVENT_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWestGuard) },
    { EVENT_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_HINT, EVENT_SCRIPT(Event_TalkToEastGuard) },
    { EVENT_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
    { EVENT_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIAmGladIDidntKnow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_HINT, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT, EVENT_SCRIPT(Event_CatchFloatingNut) },
    { SCENE_EVENTS_END },
};
// clang-format on
