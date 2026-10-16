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

#ifdef UBFIX
#define MCCOY_GARDEN_SET_ACTOR_POSITION Engine_ActorSetPosition
#else
#define MCCOY_GARDEN_SET_ACTOR_POSITION ((s32 (*)())Engine_ActorSetPosition)
#endif

// The garden before Lord McCoy's palace in Bilibin. Two guards keep the
// palace gate and let only brave warriors through; once Kolima's curse is
// lifted, they arrest the party for breaking it and take them to McCoy.

enum {
  WORKER_WEST = 8,
  WORKER_NORTH = 9,
  MCCOY = 10,
  GATE_GUARD_WEST = 11,
  GATE_GUARD_EAST = 12,
  FLOATING_NUT = 13
};

enum {
  FLAG_GATE_OPEN = 0x200,
  FLAG_SCOLDED_FOR_LATENESS = 0x201,
  FLAG_MCCOY_WAITING = 0x304, // McCoy is looking for the party
  FLAG_KOLIMA_CURSE_LIFTED = 0x845,
  FLAG_GUARDS_LET_PARTY_IN = 0x84a,
  // Two later steps of McCoy's request; they change the guards' lines.
  FLAG_84E = 0x84e,
  FLAG_84F = 0x84f,
  FLAG_ARREST_SEEN = 0x85e,
  FLAG_NUT_CAUGHT = 0xfd2
};

// Played as the palace gate swings open.
enum { SOUND_PALACE_GATE = 188 };

static void Garden_ApplyProgress(void);
static void Guards_ArrestParty(void);
static void PalaceGate_Open(void);

// Shared scene code, included where the ROM places it.
#include "common/object_facing.h"

// Where the party can enter the garden.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceGardenEntrances;
}

// The garden has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit leads.
const u32 *Scene_GetExits(void) { return gMcCoyPalaceGardenExits; }

// The garden's people. Once McCoy's later step is done the east guard
// turns back after talking, and once the curse is lifted McCoy faces the
// party.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    if (Engine_GameFlagIsSet(FLAG_84F) != 0)
      gMcCoyPalaceGardenPlacements[4].talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
      gMcCoyPalaceGardenPlacements[2].talkFacing = TALK_FACE_PARTY;
    return gMcCoyPalaceGardenPlacements;
  }
  return gMcCoyPalaceGardenPlacementsOther;
}

// Catch on the nut floating over the garden: the party gets it.
static void FloatingNut_Catch(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// Searching the dry well.
static void Well_Search(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgBilibinUponCloserInspectionSeemsDried, 1);
  Engine_EventEnd();
}

// What the party can talk to, search and catch in the garden.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    return gMcCoyPalaceGardenEvents;
  }
  return gMcCoyPalaceGardenEventsOther;
}

// The workers and McCoy each ask the party a question.
static void NorthWorker_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveYouSeenBarricadeWe);
  Engine_EventAskYesNo(WORKER_NORTH, 0);
  Engine_EventEnd();
}

// McCoy asks whether the palace work makes him responsible for the curse.
static void McCoy_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinTellingMeImResponsibleFor);
  Engine_EventAskYesNo(MCCOY, 0);
  Engine_EventEnd();
}

// The young west guard asks whether he can become a warrior like them.
static void WestGuard_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoThinkCanBecomeAs);
  Engine_EventAskYesNo(GATE_GUARD_WEST, 0);
  Engine_EventEnd();
}

// The east guard: until he lets them in, he asks whether they are brave
// enough to meet McCoy and, on yes, opens the gate and walks in with his
// partner; after, he scolds them once for keeping McCoy waiting.
static void EastGuard_Talk(void) {
  Engine_EventBegin();
  // Once admitted, the party only receives the guard's current reminder.
  if (GameFlag_IsSet(FLAG_GUARDS_LET_PARTY_IN) != 0) {
    if (Engine_GameFlagIsSet(FLAG_MCCOY_WAITING) != 0) {
      if (GameFlag_IsSet(FLAG_SCOLDED_FOR_LATENESS) == 0) {
        Engine_EventSetMessage(MsgBilibinWhere);
        Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 10);
        Actor_ShowEmote(GATE_GUARD_EAST, EMOTE_IN_FRONT | 7, 40);
        Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 10);
        Engine_ActorRunRepeatedMotion(GATE_GUARD_EAST, 2);
        Engine_GameFlagSet(FLAG_SCOLDED_FOR_LATENESS);
      }
      Engine_EventSetMessage(MsgBilibinMasterQuiteCranky);
      Engine_EventShowMessage(GATE_GUARD_EAST, 0);
    } else {
      Engine_EventSetMessage(MsgBilibinLordMccoyDifficult);
      Engine_EventShowMessage(GATE_GUARD_EAST, 0);
      goto guard;
    }
  } else {
    // On the first visit, the guards confer before opening the gate.
    Engine_EventSetMessage(MsgBilibinLordOnlyMeet);
    Engine_EventOpenMessage(GATE_GUARD_EAST, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      gEventWork->message++;
      Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 10);
      if (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed < PIXELS(270)) {
        Actor_SetSpeed(GATE_GUARD_EAST, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15a, 0x112);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 0x11a);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
      }
      Actor_FaceDirection(GATE_GUARD_WEST, FACING_EAST + FACING_STEP, 0);
      Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTHWEST + FACING_STEP, 20);
      Actor_ShowEmote(GATE_GUARD_WEST, EMOTE_IN_FRONT | 2, 20);
      Engine_ActorStartRepeatedMotion(GATE_GUARD_WEST, 1);
      Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 10);
      Actor_ShowEmote(GATE_GUARD_EAST, EMOTE_IN_FRONT | 8, 60);
      Engine_ActorStartRepeatedMotion(GATE_GUARD_EAST, 1);
      Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 20);
      Engine_ActorSetAnimation(GATE_GUARD_WEST, 3);
      Engine_ActorSetAnimationAndWait(GATE_GUARD_EAST, 3);
      Actor_FaceDirection(GATE_GUARD_WEST, FACING_SOUTHEAST + FACING_STEP, 0);
      Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTH + FACING_STEP, 10);
      Engine_ActorStartRepeatedMotion(GATE_GUARD_WEST, 1);
      Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 20);
      Actor_FaceDirection(GATE_GUARD_WEST, FACING_NORTHEAST + FACING_STEP, 0);
      Engine_ActorSetSpeed(GATE_GUARD_EAST, 0x10000, 0x8000);
      Object_GetById(GATE_GUARD_EAST)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(GATE_GUARD_EAST, 0x15a, 0x107);
      Engine_EventWait(1);
      Object_GetById(GATE_GUARD_EAST)->steeringFlags |= ACTOR_FACES_MOTION;
      Actor_SetSpeed(GATE_GUARD_WEST, 0x9999, 0x4ccc);
      Actor_WalkToAndWait(GATE_GUARD_WEST, 0x148, 0x107);
      Actor_WalkToAndWait(GATE_GUARD_WEST, 0x148, 252);
      Actor_FaceDirection(GATE_GUARD_WEST, FACING_NORTH, 10);
      PalaceGate_Open();
      Actor_WalkToAndWait(GATE_GUARD_WEST, 0x148, 246);
      /* FAKEMATCH: instruction order only: called through a cast with no
         prototype that returns a value, the call sets r0, so sched2 loads
         the actor id after the two zeros, as native. Plain forms measured
         (2026-10-13): Engine_ActorSetPosition or Actor_SetPosition by
         their prototype load r0 first (differ 2). */
      MCCOY_GARDEN_SET_ACTOR_POSITION(11, 0, 0);
      /* FAKEMATCH: instruction order only: the one-pass block's loop note
         after the call above is a scheduling barrier; with the cast it
         keeps r0 last among that call's arguments. A plain call or
         GameFlag_Set here loads r0 first (differ 2). */
      do {
        Engine_GameFlagSet(FLAG_GUARDS_LET_PARTY_IN);
      } while (0);
    guard:
      FieldObject_SetTargetAndCallback(
          12, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER, gGuardWatchesParty);
    } else {
      Engine_EventShowMessage(GATE_GUARD_EAST, 0);
      Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTHEAST + FACING_STEP, 10);
    }
  }
  Engine_EventEnd();
}

// Trigger 2, the palace gate: opens it if still shut and walks in.
static void PalaceGate_Enter(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_GATE_OPEN) == 0) {
    PalaceGate_Open();
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(2);
  Engine_EventEnd();
}

// Sets the garden up: McCoy drawn in front, and the guards, McCoy and the
// arrest as the story stands.
s32 Scene_Initialize(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *actor;

  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  actor = Object_GetById(WORKER_WEST);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 1;

  if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden) {
    Garden_ApplyProgress();
  }

  return 0;
}

// The guards stand where the party left them; once the curse is lifted,
// McCoy waits in the garden and, the first time, the guards arrest the
// party.
static void Garden_ApplyProgress(void) {
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_GATE_OPEN);
  }
  if (Engine_GameFlagIsSet(FLAG_NUT_CAUGHT) == 0) {
    InitializeStagedActorSceneOrbitingEffect(13);
  }
  if (Engine_GameFlagIsSet(FLAG_GUARDS_LET_PARTY_IN) != 0) {
    Actor_SetPosition(GATE_GUARD_WEST, PIXELS(308), PIXELS(263));
    Engine_ActorSetPosition(GATE_GUARD_EAST, PIXELS(346), PIXELS(263));
    if (Engine_GameFlagIsSet(FLAG_84F) == 0) {
      if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
        Engine_ActorSetPosition(GATE_GUARD_WEST, 0, 0);
        FieldObject_SetTargetAndCallback(
            12, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gGuardWatchesParty);
      }
    }
  }
  // McCoy's outdoor meeting and the arrest happen only after the curse lifts.
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(MCCOY, PIXELS(224), PIXELS(292));
    Actor_FaceDirection(MCCOY, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(WORKER_WEST, FACING_EAST, 0);
    if (Engine_GameFlagIsSet(FLAG_ARREST_SEEN) == 0) {
      Guards_ArrestParty();
    }
  }
}

// Kolima's curse is lifted: the guards stop the party, ask whether they
// broke the holy tree's curse until they answer yes, and march them into
// the palace to McCoy.
static void Guards_ArrestParty(void) {

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
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x140, 0x136);
  Actor_FaceDirection(GATE_GUARD_WEST, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GATE_GUARD_WEST, 2);
  Actor_ShowEmote(GATE_GUARD_WEST, EMOTE_IN_FRONT, 60);
  Engine_EventSetMessage(MsgBilibinOhItS);
  Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 10);
  Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GATE_GUARD_EAST, 2);
  Actor_ShowEmote(GATE_GUARD_EAST, EMOTE_IN_FRONT, 60);
  Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 20);
  Actor_FaceDirection(GATE_GUARD_WEST, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(GATE_GUARD_WEST, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(GATE_GUARD_WEST, 1);
  Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 10);
  Engine_ActorRunRepeatedMotion(GATE_GUARD_EAST, 1);
  Engine_EventOpenMessage(GATE_GUARD_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Refusals repeat the accusation until the player acknowledges it.
  while (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
    Engine_ActorShowEmote(GATE_GUARD_EAST, EMOTE_IN_FRONT, 60);
    Engine_EventSetMessage(MsgBilibinNotTrueWitnesses);
    Engine_EventShowMessageAndWait(GATE_GUARD_EAST, 0, 10);
    Engine_ActorStartRepeatedMotion(GATE_GUARD_EAST, 2);
    Engine_EventOpenMessage(GATE_GUARD_EAST, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(GATE_GUARD_WEST, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GATE_GUARD_EAST, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(GATE_GUARD_WEST, 3);
  Engine_ActorSetAnimationAndWait(GATE_GUARD_EAST, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GATE_GUARD_WEST, 1);
  Engine_EventSetMessage(MsgBilibinUnderArrest);
  Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 10);
  Actor_SetSpeed(GATE_GUARD_WEST, 0x10000, 0x8000);
  Actor_WalkToAndWait(GATE_GUARD_WEST, 0x13a, 0x118);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_EventShowMessageAndWait(GATE_GUARD_WEST, 0, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_EventWait(60);
  if (Engine_GameFlagIsSet(FLAG_GUARDS_LET_PARTY_IN) == 0) {
    Engine_ActorSetSpeed(GATE_GUARD_EAST, 0x10000, 0x8000);
    Object_GetById(GATE_GUARD_EAST)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkToAndWait(GATE_GUARD_EAST, 0x15a, 0x107);
    Engine_EventWait(1);
    {
      struct FieldActor *guard = Object_GetById(GATE_GUARD_EAST);

      guard->steeringFlags = ACTOR_FACES_MOTION | guard->steeringFlags;
    }
  }
  Actor_SetSpeed(GATE_GUARD_WEST, 0xcccc, 0x6666);
  // Escort Isaac through the opened gate before changing to the palace.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkTo(GATE_GUARD_WEST, 0x148, 0x106);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 0x116);
  Engine_ActorSetAnimation(GATE_GUARD_WEST, 1);
  PalaceGate_Open();
  Engine_EventWait(40);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 0x148, 242);
  Actor_WalkToAndWait(GATE_GUARD_WEST, 0x148, 242);
  Engine_ActorSetPosition(GATE_GUARD_WEST, 0, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(10);
  Engine_EventEnd();
}

// The palace gate swings open.
static void PalaceGate_Open(void) {
  Engine_AudioPlayCue(SOUND_PALACE_GATE);
  Engine_MapAnimateCells(gPalaceGateOpenSteps, 52, 11);
  Engine_GameFlagSet(FLAG_GATE_OPEN);
}

// Shared scene code, included where the ROM places it.
#include "common/object_orbit.h"

// clang-format off
// The gate opening: three steps of cells, 5 frames each.
u16 gPalaceGateOpenSteps[] = {
    MAP_CELL_STEP(34, 25, 1, 3, 5), MAP_CELL_STEP(35, 25, 1, 3, 5), MAP_CELL_STEP(36, 25, 1, 3, 5), MAP_CELL_STEPS_END,
};

// A guard turns to keep facing the party.
s32 gGuardWatchesParty[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gMcCoyPalaceGardenEntrances[] = {
    { 1, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gMcCoyPalaceGardenExits[] = {
    SCENE_EXITS(34),
    SCENE_EXIT(1, 30, 2),
    SCENE_EXIT(2, 33, 1),
    SCENE_EXIT(10, 33, 29),
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
    { 0x16, FLAG_NUT_CAUGHT, 7,
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(PalaceGate_Enter) },
    { EVENT_TALK, 8, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
    { EVENT_TALK, 9, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(NorthWorker_Talk) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
    { EVENT_TALK, 10, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(McCoy_Talk) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
    { EVENT_TALK, 11, FLAG_84F, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
    { EVENT_TALK, 11, FLAG_84E, MSGNO(MsgBilibinICantTellYouAnythingElse) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(WestGuard_Talk) },
    { EVENT_TALK, 12, FLAG_84F, EVENT_SCRIPT(EastGuard_Talk) },
    { EVENT_TALK, 12, FLAG_84E, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIAmGladIDidntKnow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_GUARDS_LET_PARTY_IN, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_84E, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_GUARDS_LET_PARTY_IN, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_84F, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_84E, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Well_Search) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 13, FLAG_NUT_CAUGHT, EVENT_SCRIPT(FloatingNut_Catch) },
    { SCENE_EVENTS_END },
};
// clang-format on
