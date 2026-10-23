/// @file The grounds of McCoy's Palace in Bilibin: MapId_McCoyPalaceGrounds,
/// with the area name McCoy's Palace, which the palace itself shares
/// (mccoy_palace.c). Workers build Lady McCoy a new palace here, and two
/// guards keep the gate of McCoy's own. Its people and talk follow two story
/// flags: FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, set once McCoy has turned
/// the party away, and FLAG_KOLIMA_CURSE_LIFTED.
///
/// Talking to the east guard (EventScript_TalkToEastGuard) is the way in: on
/// yes the guards confer, the gate opens (McCoyPalaceGrounds_OpenGate) and the
/// west guard goes in ahead (FLAG_MCCOY_PALACE_ADMITTED). Walking north into
/// the gate (coord event 2, EventScript_EnterPalace) takes
/// MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE into the palace. The guard's scolding
/// for keeping McCoy waiting is unused (EventScript_TalkToEastGuard).
///
/// Once the curse is lifted, the first arrival until McCoy has rewarded the
/// party (FLAG_MCCOY_PALACE_PARTY_REWARDED) plays
/// EventScript_GuardsArrestParty: the guards repeat their accusation until the
/// player says yes, then march the party into the palace
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST), where McCoy rewards it.
///
/// McCoyPalaceGrounds_RestoreStory places the guards by the same flags and
/// floats a Nut over the grounds until Catch takes it
/// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT). A dry well can be searched, and
/// MCCOY_PALACE_GROUNDS_TO_BILIBIN leads back to Bilibin. From TBS_PATCH_3 on
/// a patch of ivy (SPRITE_IVY) stands at (132, 388), over a dark opening at the
/// foot of the cliff on the grounds' south edge, which the earlier editions
/// show bare (renders of tbs-en and tbs-fr); no warp or map event answers that
/// opening, so the ivy only hides a way in that leads nowhere.
///
/// Open questions: none.

#include "game_state.h"
#include "map_ids.h"
#include "unit/map/mccoy_palace_grounds.h"
#include "unit/map/common/object_orbit.h"
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
#include "mccoy_palace_grounds.h"
#include "types.h"
#include "call.h"
#include "script_layer.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "objcmd.h"
#include "flags.h"
#include "facing_object.h"
#include "songs.h"

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared. Its use is tagged.
#ifdef UBFIX
#define MCCOY_GARDEN_SET_OBJ_EVENT_POSITION Engine_ObjectEventSetPosition
#else
#define MCCOY_GARDEN_SET_OBJ_EVENT_POSITION                                    \
  ((s32 (*)())Engine_ObjectEventSetPosition)
#endif

/// The placed object events, numbered in the order of
/// gMcCoyPalaceGroundsObjectEventTemplates.
enum McCoyPalaceGroundsLocalId {
  /// A worker with a pickaxe by the new palace, drawn in front. "The lumber I
  /// ordered from
  /// Kolima disappeared..."
  /// ★ Named from the game: "We'd still be working if we hadn't tried building
  /// using the holy tree." (MsgBilibinWedStillBeWorkingIfWe)
  LOCALID_WEST_WORKER = LOCALID_FIRST_PLACED,
  /// The worker with a pickaxe who wanders by the new palace. "Have you seen
  /// the barricade
  /// we built on..."
  /// ★ Named from the game: "Have you seen the barricade we built on the road
  /// to Kolima?" (MsgBilibinHaveYouSeenBarricadeWe)
  LOCALID_NORTH_WORKER,
  /// A woman with curly purple hair in a pink dress. "Hmph! My palace is not
  /// finished!"
  /// ★ Named from the game: "Lady McCoy" (MsgDebugGlyphName)
  LOCALID_LADY_MCCOY,
  /// The soldier in a spiked helmet at the gate who goes in ahead of the party.
  /// "Lord McCoy is so upset about this curse that he can't even eat."
  /// ★ Named from what the player sees: the western of the two soldiers with
  /// spears at the palace door (its render)
  LOCALID_WEST_GUARD,
  /// "The lord will only meet with brave..."
  /// ★ Named from what the player sees: the eastern of the two soldiers with
  /// spears at the palace door (its render)
  LOCALID_EAST_GUARD,
  /// The Nut over the grounds, gone once Catch has taken it
  /// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT).
  /// ★ Named from the game: "Nut" (MsgItemName)
  LOCALID_FLOATING_NUT
};

/// The grounds' temporary flags.
enum {
  /// The palace gate stands open (McCoyPalaceGrounds_OpenGate), until the map
  /// is left.
  /// ★ Named from its area name: McCoy's Palace
  FLAG_GATE_OPEN = FLAG_MAP_TEMP_00,
  /// The east guard has scolded the party for keeping McCoy waiting.
  /// ★ Named from the game: "Where were you?" (MsgBilibinWhere)
  FLAG_GUARD_SCOLDED = FLAG_MAP_TEMP_01,
  /// Set by Bilibin (bilibin.c) while the guards have let the party in but it
  /// has not yet entered the palace. The grounds are in another map section
  /// than Bilibin, so entering them clears it before the guard can read it.
  /// ★ Named from the game: "The master can be quite cranky. Hurry along,
  /// now!" (MsgBilibinMasterQuiteCranky)
  FLAG_KEEPING_MCCOY_WAITING = FLAG_MAPSEC_TEMP_04
};

static void McCoyPalaceGrounds_RestoreStory(void);
static void EventScript_GuardsArrestParty(void);
static void McCoyPalaceGrounds_OpenGate(void);

#include "common/object_facing.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gMcCoyPalaceGroundsWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gMcCoyPalaceGroundsWarps; }

/// The people outside McCoy's Palace. Once McCoy has turned the party away,
/// the east guard turns back after talking, and once the curse is lifted Lady
/// McCoy faces the party.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (gGameState.mapId == (s32)&MapId_McCoyPalaceGrounds) {
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0)
      gMcCoyPalaceGroundsObjectEventTemplates[LOCALID_EAST_GUARD -
                                              LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
      gMcCoyPalaceGroundsObjectEventTemplates[LOCALID_LADY_MCCOY -
                                              LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
    return gMcCoyPalaceGroundsObjectEventTemplates;
  }
  return gMcCoyPalaceGroundsNoObjectEventTemplates;
}

/// McCoy's Palace, outside: Catch on the Nut floating over the grounds gives
/// it to the party.
static void EventScript_CatchFloatingNut(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetPosition(LOCALID_FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, outside: searching the dry well.
static void EventScript_SearchWell(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgBilibinUponCloserInspectionSeemsDried, 1);
  Engine_ScriptEnd();
}

/// What the party can talk to, search and catch outside McCoy's Palace.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_McCoyPalaceGrounds) {
    return gMcCoyPalaceGroundsEvents;
  }
  return gMcCoyPalaceGroundsNoEvents;
}

/// McCoy's Palace, outside, before the curse is lifted: the north worker asks
/// whether the party has seen the barricade they built on the road to Kolima.
static void EventScript_TalkToNorthWorker(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinHaveYouSeenBarricadeWe);
  Engine_ScriptAskYesNo(LOCALID_NORTH_WORKER, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, outside, before the curse is lifted: Lady McCoy asks
/// whether the party blames her for the curse: on yes she calls herself the
/// victim, on no she calls the curse a superstition, and either way she wants
/// her palace.
static void EventScript_TalkToLadyMcCoy(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinTellingMeImResponsibleFor);
  Engine_ScriptAskYesNo(LOCALID_LADY_MCCOY, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, outside, after the reward: the west guard asks whether he
/// can become as fine a warrior as the leader.
static void EventScript_TalkToWestGuard(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinDoThinkCanBecomeAs);
  Engine_ScriptAskYesNo(LOCALID_WEST_GUARD, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, outside, until McCoy turns the party away: the east guard
/// asks whether the party is brave enough to meet his lord and, on yes, the
/// gate opens and the west guard walks in; once it is admitted, he reminds it
/// that McCoy is a difficult man.
///
/// Unused: the branch for a party back from Bilibin, in which he scolds it once
/// for keeping McCoy waiting (MsgBilibinWhere, `Where were you?`, and
/// MsgBilibinMasterQuiteCranky), never runs, because FLAG_KEEPING_MCCOY_WAITING
/// is cleared on entering the grounds.
static void EventScript_TalkToEastGuard(void) {
  Engine_ScriptStart();
  // Once admitted, the party only receives the guard's current reminder.
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    if (Engine_GameFlagIsSet(FLAG_KEEPING_MCCOY_WAITING) != 0) {
      if (GameFlag_IsSet(FLAG_GUARD_SCOLDED) == 0) {
        Engine_ScriptSetMessage(MsgBilibinWhere);
        Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 10);
        ObjectEvent_ShowEmote(LOCALID_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SCOWL,
                              40);
        Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 10);
        Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_GUARD, 2);
        Engine_GameFlagSet(FLAG_GUARD_SCOLDED);
      }
      Engine_ScriptSetMessage(MsgBilibinMasterQuiteCranky);
      Engine_ScriptShowMessage(LOCALID_EAST_GUARD, 0);
    } else {
      Engine_ScriptSetMessage(MsgBilibinLordMccoyDifficult);
      Engine_ScriptShowMessage(LOCALID_EAST_GUARD, 0);
      goto guard;
    }
  } else {
    // On the first visit, the guards confer before opening the gate.
    Engine_ScriptSetMessage(MsgBilibinLordOnlyMeet);
    Engine_ScriptOpenMessage(LOCALID_EAST_GUARD, 0);
    if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      gEventWork->message++;
      Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 10);
      if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed < PIXELS(270)) {
        ObjectEvent_SetSpeed(LOCALID_EAST_GUARD, 0xcccc, 0x6666);
        ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 346, 274);
        ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 328, 282);
        ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
      }
      ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_EAST + FACING_STEP,
                                0);
      ObjectEvent_FaceDirection(LOCALID_EAST_GUARD,
                                FACING_SOUTHWEST + FACING_STEP, 20);
      ObjectEvent_ShowEmote(LOCALID_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
                            20);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_GUARD, 1);
      Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
      ObjectEvent_ShowEmote(LOCALID_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SMILE,
                            60);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_GUARD, 1);
      Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 20);
      Engine_ObjectEventSetAnimation(LOCALID_WEST_GUARD, ANIMATION_NOD);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_GUARD, ANIMATION_NOD);
      ObjectEvent_FaceDirection(LOCALID_WEST_GUARD,
                                FACING_SOUTHEAST + FACING_STEP, 0);
      ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH + FACING_STEP,
                                10);
      Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_GUARD, 1);
      Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 20);
      ObjectEvent_FaceDirection(LOCALID_WEST_GUARD,
                                FACING_NORTHEAST + FACING_STEP, 0);
      Engine_ObjectEventSetSpeed(LOCALID_EAST_GUARD, 0x10000, 0x8000);
      ObjectEvent_GetById(LOCALID_EAST_GUARD)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(LOCALID_EAST_GUARD, 346, 263);
      Engine_ScriptWait(1);
      ObjectEvent_GetById(LOCALID_EAST_GUARD)->steeringFlags |=
          OBJ_EVENT_FACES_MOTION;
      ObjectEvent_SetSpeed(LOCALID_WEST_GUARD, 0x9999, 0x4ccc);
      ObjectEvent_WalkToAndWait(LOCALID_WEST_GUARD, 328, 263);
      ObjectEvent_WalkToAndWait(LOCALID_WEST_GUARD, 328, 252);
      ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_NORTH, 10);
      McCoyPalaceGrounds_OpenGate();
      ObjectEvent_WalkToAndWait(LOCALID_WEST_GUARD, 328, 246);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which loads the actor id after the two zeros; as a call by its
       * void declaration, 1 of 233 instructions differ in tbs-en. */
      MCCOY_GARDEN_SET_OBJ_EVENT_POSITION(LOCALID_WEST_GUARD, 0, 0);
      /* FAKEMATCH(one-pass): the block keeps r0 last among the call above's
         arguments; as a plain call, 1 of 233 instructions differ in tbs-en. */
      do {
        Engine_GameFlagSet(FLAG_MCCOY_PALACE_ADMITTED);
      } while (0);
    guard:
      FieldObject_SetTargetAndCallback(
          LOCALID_EAST_GUARD, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
          gGuardWatchesParty);
    } else {
      Engine_ScriptShowMessage(LOCALID_EAST_GUARD, 0);
      ObjectEvent_FaceDirection(LOCALID_EAST_GUARD,
                                FACING_SOUTHEAST + FACING_STEP, 10);
    }
  }
  Engine_ScriptEnd();
}

/// McCoy's Palace, outside, coord event 2: walking north into the palace gate
/// opens it if it is still shut, and the leader walks into the palace.
static void EventScript_EnterPalace(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_GATE_OPEN) == 0) {
    McCoyPalaceGrounds_OpenGate();
  }
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -16);
  Engine_ScriptWait(16);
  Engine_ScriptRequestWarp(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE);
  Engine_ScriptEnd();
}

/// Sets up the grounds outside McCoy's Palace: the west worker drawn in
/// front, and the guards, Lady McCoy and the arrest as the story stands.
s32 Map_Initialize(void) {
  struct EventWork *work = gEventWork;
  struct ObjectEvent *objectEvent;

  work->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  objectEvent = ObjectEvent_GetById(LOCALID_WEST_WORKER);
  objectEvent->priorityFlags = 0;
  objectEvent->sprite->image.priority = 1;

  if (gGameState.mapId == (s32)&MapId_McCoyPalaceGrounds) {
    McCoyPalaceGrounds_RestoreStory();
  }

  return 0;
}

/// The guards stand where the party left them; once the curse is lifted,
/// Lady McCoy stands before the palace and, until McCoy has rewarded the
/// party, the guards arrest them.
static void McCoyPalaceGrounds_RestoreStory(void) {
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_GATE_OPEN);
  }
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT) == 0) {
    Map_FloatNut(LOCALID_FLOATING_NUT);
  }
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    ObjectEvent_SetPosition(LOCALID_WEST_GUARD, PIXELS(308), PIXELS(263));
    Engine_ObjectEventSetPosition(LOCALID_EAST_GUARD, PIXELS(346), PIXELS(263));
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
      if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
        Engine_ObjectEventSetPosition(LOCALID_WEST_GUARD, 0, 0);
        FieldObject_SetTargetAndCallback(LOCALID_EAST_GUARD,
                                         OBJ_EVENT_LINK_KEEP_MOTION |
                                             LOCALID_PARTY_LEADER,
                                         gGuardWatchesParty);
      }
    }
  }
  // Lady McCoy's new place and the arrest come only after the curse lifts.
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    ObjectEvent_SetPosition(LOCALID_LADY_MCCOY, PIXELS(224), PIXELS(292));
    ObjectEvent_FaceDirection(LOCALID_LADY_MCCOY, FACING_SOUTH, 0);
    Engine_ObjectEventFaceDirection(LOCALID_WEST_WORKER, FACING_EAST, 0);
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
      EventScript_GuardsArrestParty();
    }
  }
}

/// McCoy's Palace, outside, on entry once Kolima's curse is lifted: the guards
/// stop the party, ask whether they broke the curse of the holy tree until
/// they answer yes, and march them into the palace to McCoy.
static void EventScript_GuardsArrestParty(void) {

  // Bring the leader into view before the guards interrupt.
  Engine_ScriptStart();
  Camera_MoveTo(-1, -1, -1, 0);
  Camera_MoveTo(PIXELS(320), -1, PIXELS(320), 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(320), PIXELS(372));
  Engine_ScriptOpenScreen();
  Engine_CameraSetSpeed(0x3333, 0x666);
  Engine_CameraMoveTo(PIXELS(320), -1, PIXELS(290), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x9999, 0x4ccc);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 320, 310);
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_GUARD, 2);
  ObjectEvent_ShowEmote(LOCALID_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        60);
  Engine_ScriptSetMessage(MsgBilibinOhItS);
  Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_GUARD, 2);
  ObjectEvent_ShowEmote(LOCALID_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        60);
  Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_EAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTHWEST + FACING_STEP,
                            40);
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_GUARD, 1);
  Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_GUARD, 1);
  Engine_ScriptOpenMessage(LOCALID_EAST_GUARD, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Refusals repeat the accusation until the player acknowledges it.
  while (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) != 0) {
    Engine_ObjectEventShowEmote(LOCALID_EAST_GUARD,
                                EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Engine_ScriptSetMessage(MsgBilibinNotTrueWitnesses);
    Engine_ScriptShowMessageAndWait(LOCALID_EAST_GUARD, 0, 10);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_GUARD, 2);
    Engine_ScriptOpenMessage(LOCALID_EAST_GUARD, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_GUARD, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_GUARD, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_GUARD, 1);
  Engine_ScriptSetMessage(MsgBilibinUnderArrest);
  Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
  ObjectEvent_SetSpeed(LOCALID_WEST_GUARD, 0x10000, 0x8000);
  ObjectEvent_WalkToAndWait(LOCALID_WEST_GUARD, 314, 280);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  Engine_ScriptShowMessageAndWait(LOCALID_WEST_GUARD, 0, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_ADMITTED) == 0) {
    Engine_ObjectEventSetSpeed(LOCALID_EAST_GUARD, 0x10000, 0x8000);
    ObjectEvent_GetById(LOCALID_EAST_GUARD)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    Engine_ObjectEventWalkToAndWait(LOCALID_EAST_GUARD, 346, 263);
    Engine_ScriptWait(1);
    {
      struct ObjectEvent *guard = ObjectEvent_GetById(LOCALID_EAST_GUARD);

      guard->steeringFlags = OBJ_EVENT_FACES_MOTION | guard->steeringFlags;
    }
  }
  ObjectEvent_SetSpeed(LOCALID_WEST_GUARD, 0xcccc, 0x6666);
  // Escort Isaac through the opened gate before changing to the palace.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0xcccc, 0x6666);
  ObjectEvent_WalkTo(LOCALID_WEST_GUARD, 328, 262);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 328, 278);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_GUARD, ANIMATION_STAND);
  McCoyPalaceGrounds_OpenGate();
  Engine_ScriptWait(40);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 328, 242);
  ObjectEvent_WalkToAndWait(LOCALID_WEST_GUARD, 328, 242);
  Engine_ObjectEventSetPosition(LOCALID_WEST_GUARD, 0, 0);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST);
  Engine_ScriptEnd();
}

/// The gate of McCoy's Palace swings open.
static void McCoyPalaceGrounds_OpenGate(void) {
  Engine_AudioPlayCue(SE_188);
  Engine_MapGridAnimateCells(gPalaceGateOpenSteps, 52, 11);
  Engine_GameFlagSet(FLAG_GATE_OPEN);
}

#include "common/object_orbit.h"

// clang-format off
/// The gate opening: three steps of cells, 5 frames each.
u16 gPalaceGateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(34, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(35, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(36, 25, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

/// A guard turns to keep facing the party.
s32 gGuardWatchesParty[] = {
    SC_CALL(Map_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct MapWarpDest gMcCoyPalaceGroundsWarpDests[] = {
    { MCCOY_PALACE_GROUNDS_FROM_BILIBIN, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gMcCoyPalaceGroundsWarps[] = {
    MAP_WARPS(McCoyPalaceGrounds),
    MAP_WARP(MCCOY_PALACE_GROUNDS_TO_BILIBIN, Bilibin, BILIBIN_FROM_MCCOY_PALACE_GROUNDS),
    MAP_WARP(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST),
    MAP_WARP(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
    MAP_WARPS_END,
};

/// Unused: the getters return it for any map but MapId_McCoyPalaceGrounds,
/// and the overlay serves no other.
struct ObjectEventTemplate gMcCoyPalaceGroundsNoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMcCoyPalaceGroundsObjectEventTemplates[] = {
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(103), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(135), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(327), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(320), 0, PIXELS(263), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(334), 0, PIXELS(263), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if TBS_PATCH_3
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(388), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
    { MAP_TABLE_END },
};

/// Unused, as gMcCoyPalaceGroundsNoObjectEventTemplates is.
struct MapEvent gMcCoyPalaceGroundsNoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gMcCoyPalaceGroundsEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, MCCOY_PALACE_GROUNDS_TO_BILIBIN },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterPalace) },
    { MAP_EVENT_TALK, LOCALID_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
    { MAP_EVENT_TALK, LOCALID_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
    { MAP_EVENT_TALK, LOCALID_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToNorthWorker) },
    { MAP_EVENT_TALK, LOCALID_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
    { MAP_EVENT_TALK, LOCALID_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToLadyMcCoy) },
    { MAP_EVENT_TALK, LOCALID_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
    { MAP_EVENT_TALK, LOCALID_WEST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
    { MAP_EVENT_TALK, LOCALID_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinICantTellYouAnythingElse) },
    { MAP_EVENT_TALK, LOCALID_WEST_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWestGuard) },
    { MAP_EVENT_TALK, LOCALID_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, EVENT_SCRIPT(EventScript_TalkToEastGuard) },
    { MAP_EVENT_TALK, LOCALID_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
    { MAP_EVENT_TALK, LOCALID_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIAmGladIDidntKnow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, FLAG_MCCOY_PALACE_ADMITTED, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
    { BG_EVENT(BG_EVENT_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchWell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchFloatingNut) },
    { MAP_EVENTS_END },
};
// clang-format on
