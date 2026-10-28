/// @file The grounds of McCoy's Palace in Bilibin: SceneId_McCoyPalaceGrounds,
/// with the area name McCoy's Palace, which the palace itself shares
/// (mccoy_palace.c). Workers build Lady McCoy a new palace here, and two
/// guards keep the gate of McCoy's own. Its people and talk follow two story
/// flags: FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, set once McCoy has turned
/// the party away, and FLAG_KOLIMA_CURSE_LIFTED.
///
/// Talking to the east guard (McCoyPalaceGrounds_TalkToEastGuard) is the way
/// in: on yes the guards confer, the gate opens (McCoyPalaceGrounds_OpenGate)
/// and the west guard goes in ahead (FLAG_MCCOY_PALACE_ADMITTED). Walking north
/// into the gate (step trigger 2, McCoyPalaceGrounds_EnterPalace) takes
/// MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE into the palace. The guard's scolding
/// for keeping McCoy waiting is unused (McCoyPalaceGrounds_TalkToEastGuard).
///
/// Once the curse is lifted, the first arrival until McCoy has rewarded the
/// party (FLAG_MCCOY_PALACE_PARTY_REWARDED) plays
/// McCoyPalaceGrounds_GuardsArrestParty: the guards repeat their accusation
/// until the player says yes, then march the party into the palace
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST), where McCoy rewards it.
///
/// McCoyPalaceGrounds_RestoreStory places the guards by the same flags and
/// floats a Nut over the grounds until Catch takes it
/// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT). A dry well can be searched, and
/// MCCOY_PALACE_GROUNDS_TO_BILIBIN leads back to Bilibin. From TBS_PATCH_3 on
/// a patch of ivy (SPRITE_IVY) stands at (132, 388), over a dark opening at the
/// foot of the cliff on the grounds' south edge, which the earlier editions
/// show bare (renders of tbs-en and tbs-fr); no exit or trigger answers that
/// opening, so the ivy only hides a way in that leads nowhere.

#include "adventure.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace_grounds.h"
#include "unit/scene/shared/actor_orbit.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "actor_script.h"
#include "flags.h"
#include "songs.h"
#include "heap.h"

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared. Its use is tagged.
#ifdef UBFIX
#define MCCOY_GARDEN_SET_ACTOR_POSITION Import_ActorSetPosition
#else
#define MCCOY_GARDEN_SET_ACTOR_POSITION ((s32 (*)())Import_ActorSetPosition)
#endif

/// The placed actors, numbered in the order of
/// gMcCoyPalaceGroundsActorPlacements.
enum McCoyPalaceGroundsActorId {
  /// A worker with a pickaxe by the new palace, drawn in front. "The lumber I
  /// ordered from
  /// Kolima disappeared..."
  /// ★ Named from the game: "We'd still be working if we hadn't tried building
  /// using the holy tree." (MsgBilibinWedStillBeWorkingIfWe)
  ACTOR_WEST_WORKER = ACTOR_FIRST_PLACED,
  /// The worker with a pickaxe who wanders by the new palace. "Have you seen
  /// the barricade
  /// we built on..."
  /// ★ Named from the game: "Have you seen the barricade we built on the road
  /// to Kolima?" (MsgBilibinHaveYouSeenBarricadeWe)
  ACTOR_NORTH_WORKER,
  /// A woman with curly purple hair in a pink dress. "Hmph! My palace is not
  /// finished!"
  /// ★ Named from the game: "Lady McCoy" (MsgDebugGlyphName)
  ACTOR_LADY_MCCOY,
  /// The soldier in a spiked helmet at the gate who goes in ahead of the party.
  /// "Lord McCoy is so upset about this curse that he can't even eat."
  /// ★ Named from what the player sees: the western of the two soldiers with
  /// spears at the palace door (its render)
  ACTOR_WEST_GUARD,
  /// "The lord will only meet with brave..."
  /// ★ Named from what the player sees: the eastern of the two soldiers with
  /// spears at the palace door (its render)
  ACTOR_EAST_GUARD,
  /// The Nut over the grounds, gone once Catch has taken it
  /// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT).
  /// ★ Named from the game: "Nut" (MsgItemName)
  ACTOR_FLOATING_NUT
};

/// The grounds' temporary flags.
enum {
  /// The palace gate stands open (McCoyPalaceGrounds_OpenGate), until the scene
  /// is left.
  /// ★ Named from its area name: McCoy's Palace
  FLAG_GATE_OPEN = FLAG_SCENE_TEMP_00,
  /// The east guard has scolded the party for keeping McCoy waiting.
  /// ★ Named from the game: "Where were you?" (MsgBilibinWhere)
  FLAG_GUARD_SCOLDED = FLAG_SCENE_TEMP_01,
  /// Set by Bilibin (bilibin.c) while the guards have let the party in but it
  /// has not yet entered the palace. The grounds are in another area
  /// than Bilibin, so entering them clears it before the guard can read it.
  /// ★ Named from the game: "The master can be quite cranky. Hurry along,
  /// now!" (MsgBilibinMasterQuiteCranky)
  FLAG_KEEPING_MCCOY_WAITING = FLAG_AREA_TEMP_04
};

static void McCoyPalaceGrounds_RestoreStory(void);
static void McCoyPalaceGrounds_GuardsArrestParty(void);
static void McCoyPalaceGrounds_OpenGate(void);

#include "shared/actor_facing.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceGroundsEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceGroundsExits; }

/// The people outside McCoy's Palace. Once McCoy has turned the party away,
/// the east guard turns back after talking, and once the curse is lifted Lady
/// McCoy faces the party.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gAdventure.sceneId == (s32)&SceneId_McCoyPalaceGrounds) {
    if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0)
      gMcCoyPalaceGroundsActorPlacements[ACTOR_EAST_GUARD - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0)
      gMcCoyPalaceGroundsActorPlacements[ACTOR_LADY_MCCOY - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
    return gMcCoyPalaceGroundsActorPlacements;
  }
  return gMcCoyPalaceGroundsNoActorPlacements;
}

/// McCoy's Palace, outside: Catch on the Nut floating over the grounds gives
/// it to the party.
static void McCoyPalaceGrounds_CatchFloatingNut(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_FLOATING_NUT, 0, 0);
  GameFlag_Set(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// McCoy's Palace, outside: searching the dry well.
static void McCoyPalaceGrounds_SearchWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgBilibinUponCloserInspectionSeemsDried, 1);
  Event_End();
}

/// What the party can talk to, search and catch outside McCoy's Palace.
const struct Trigger *Scene_GetTriggers(void) {
  if (gAdventure.sceneId == (s32)&SceneId_McCoyPalaceGrounds) {
    return gMcCoyPalaceGroundsTriggers;
  }
  return gMcCoyPalaceGroundsNoTriggers;
}

/// McCoy's Palace, outside, before the curse is lifted: the north worker asks
/// whether the party has seen the barricade they built on the road to Kolima.
static void McCoyPalaceGrounds_TalkToNorthWorker(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveYouSeenBarricadeWe);
  Event_AskYesNoAndShowResponse(ACTOR_NORTH_WORKER, 0);
  Event_End();
}

/// McCoy's Palace, outside, before the curse is lifted: Lady McCoy asks
/// whether the party blames her for the curse: on yes she calls herself the
/// victim, on no she calls the curse a superstition, and either way she wants
/// her palace.
static void McCoyPalaceGrounds_TalkToLadyMcCoy(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinTellingMeImResponsibleFor);
  Event_AskYesNoAndShowResponse(ACTOR_LADY_MCCOY, 0);
  Event_End();
}

/// McCoy's Palace, outside, after the reward: the west guard asks whether he
/// can become as fine a warrior as the leader.
static void McCoyPalaceGrounds_TalkToWestGuard(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoThinkCanBecomeAs);
  Event_AskYesNoAndShowResponse(ACTOR_WEST_GUARD, 0);
  Event_End();
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
static void McCoyPalaceGrounds_TalkToEastGuard(void) {
  Event_Start();
  // Once admitted, the party only receives the guard's current reminder.
  if (Event_GetFlag(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    if (GameFlag_Get(FLAG_KEEPING_MCCOY_WAITING) != 0) {
      if (Event_GetFlag(FLAG_GUARD_SCOLDED) == 0) {
        Event_SetMessageId(MsgBilibinWhere);
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
        GameFlag_Set(FLAG_GUARD_SCOLDED);
      }
      Event_SetMessageId(MsgBilibinMasterQuiteCranky);
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
    } else {
      Event_SetMessageId(MsgBilibinLordMccoyDifficult);
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
      goto guard;
    }
  } else {
    // On the first visit, the guards confer before opening the gate.
    Event_SetMessageId(MsgBilibinLordOnlyMeet);
    FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldWork->message++;
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
      if (Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed < PIXELS(270)) {
        Actor_SetSpeed(ACTOR_EAST_GUARD, 0xcccc, 0x6666);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 346, 274);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 282);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
      }
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHWEST + FACING_STEP, 20);
      Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
      Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
      Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
      Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 1);
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
      Import_ActorSetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
      Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTHEAST + FACING_STEP, 0);
      Actor_SetSpeedParameters(ACTOR_EAST_GUARD, 0x10000, 0x8000);
      Actor_GetById(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
      Event_WaitFrames(1);
      Actor_GetById(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
      Actor_SetSpeed(ACTOR_WEST_GUARD, 0x9999, 0x4ccc);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 263);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 252);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 10);
      McCoyPalaceGrounds_OpenGate();
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 246);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which loads the actor id after the two zeros; as a call by its
       * void declaration, 1 of 233 instructions differ in tbs-en. */
      MCCOY_GARDEN_SET_ACTOR_POSITION(ACTOR_WEST_GUARD, 0, 0);
      /* FAKEMATCH(one-pass): the block keeps r0 last among the call above's
         arguments; as a plain call, 1 of 233 instructions differ in tbs-en. */
      do {
        GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
      } while (0);
    guard:
      FieldActor_SetTargetAndCallback(
          ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
          gGuardWatchesParty);
    } else {
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
    }
  }
  Event_End();
}

/// McCoy's Palace, outside, step trigger 2: walking north into the palace gate
/// opens it if it is still shut, and the leader walks into the palace.
static void McCoyPalaceGrounds_EnterPalace(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_GATE_OPEN) == 0) {
    McCoyPalaceGrounds_OpenGate();
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_WaitFrames(16);
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE);
  Event_End();
}

/// Sets up the grounds outside McCoy's Palace: the west worker drawn in
/// front, and the guards, Lady McCoy and the arrest as the story stands.
s32 Scene_Initialize(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *actor;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  actor = Actor_GetById(ACTOR_WEST_WORKER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 1;

  if (gAdventure.sceneId == (s32)&SceneId_McCoyPalaceGrounds) {
    McCoyPalaceGrounds_RestoreStory();
  }

  return 0;
}

/// The guards stand where the party left them; once the curse is lifted,
/// Lady McCoy stands before the palace and, until McCoy has rewarded the
/// party, the guards arrest them.
static void McCoyPalaceGrounds_RestoreStory(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_GATE_OPEN);
  }
  if (GameFlag_Get(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_FLOATING_NUT);
  }
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    Actor_SetPosition(ACTOR_WEST_GUARD, PIXELS(308), PIXELS(263));
    Import_ActorSetPosition(ACTOR_EAST_GUARD, PIXELS(346), PIXELS(263));
    if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
      if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
        Import_ActorSetPosition(ACTOR_WEST_GUARD, 0, 0);
        FieldActor_SetTargetAndCallback(
            ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gGuardWatchesParty);
      }
    }
  }
  // Lady McCoy's new place and the arrest come only after the curse lifts.
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(ACTOR_LADY_MCCOY, PIXELS(224), PIXELS(292));
    Actor_FaceDirection(ACTOR_LADY_MCCOY, FACING_SOUTH, 0);
    Actor_ArmCallback(ACTOR_WEST_WORKER, FACING_EAST, 0);
    if (GameFlag_Get(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
      McCoyPalaceGrounds_GuardsArrestParty();
    }
  }
}

/// McCoy's Palace, outside, on entry once Kolima's curse is lifted: the guards
/// stop the party, ask whether they broke the curse of the holy tree until
/// they answer yes, and march them into the palace to McCoy.
static void McCoyPalaceGrounds_GuardsArrestParty(void) {

  // Bring the leader into view before the guards interrupt.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_MoveTo(PIXELS(320), -1, PIXELS(320), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(320), PIXELS(372));
  Event_StartScreenTransition();
  FieldCamera_SetSpeedParameters(0x3333, 0x666);
  FieldCamera_MoveWithinMapBounds(PIXELS(320), -1, PIXELS(290), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 320, 310);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_SetMessageId(MsgBilibinOhItS);
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 2);
  Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 1);
  FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Refusals repeat the accusation until the player acknowledges it.
  while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    FieldEffect_SpawnAttached(ACTOR_EAST_GUARD,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Event_SetMessageId(MsgBilibinNotTrueWitnesses);
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 2);
    FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
  }
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  Event_SetMessageId(MsgBilibinUnderArrest);
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 314, 280);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) == 0) {
    Actor_SetSpeedParameters(ACTOR_EAST_GUARD, 0x10000, 0x8000);
    Actor_GetById(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_SetPositionAndReset(ACTOR_EAST_GUARD, 346, 263);
    Event_WaitFrames(1);
    {
      struct Actor *guard = Actor_GetById(ACTOR_EAST_GUARD);

      guard->steeringFlags = ACTOR_FACES_MOTION | guard->steeringFlags;
    }
  }
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0xcccc, 0x6666);
  // Escort Isaac through the opened gate before changing to the palace.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_WEST_GUARD, 328, 262);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 278);
  Import_ActorSetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
  McCoyPalaceGrounds_OpenGate();
  Event_WaitFrames(40);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 242);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 242);
  Import_ActorSetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST);
  Event_End();
}

/// The gate of McCoy's Palace swings open.
static void McCoyPalaceGrounds_OpenGate(void) {
  Sound_PlaySong(SE_THUMP);
  MapGrid_PlayMapEdit(gPalaceGateOpenSteps, 52, 11);
  GameFlag_Set(FLAG_GATE_OPEN);
}

#include "shared/actor_orbit.h"

// clang-format off
/// The gate opening: three steps of cells, 5 frames each.
u16 gPalaceGateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(34, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(35, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(36, 25, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

/// A guard turns to keep facing the party.
s32 gGuardWatchesParty[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gMcCoyPalaceGroundsEntrances[] = {
    { MCCOY_PALACE_GROUNDS_FROM_BILIBIN, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gMcCoyPalaceGroundsExits[] = {
    SCENE_EXITS(McCoyPalaceGrounds),
    SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_BILIBIN, Bilibin, BILIBIN_FROM_MCCOY_PALACE_GROUNDS),
    SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST),
    SCENE_EXIT(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST, McCoyPalace, MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
    SCENE_EXITS_END,
};

/// Unused: the getters return it for any scene but SceneId_McCoyPalaceGrounds,
/// and the overlay serves no other.
struct ActorPlacement gMcCoyPalaceGroundsNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gMcCoyPalaceGroundsActorPlacements[] = {
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(103), 0, PIXELS(280), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(135), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(327), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(320), 0, PIXELS(263), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(334), 0, PIXELS(263), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(344), FACING_SOUTH, TALK_FACING_KEEP, 0 },
#if TBS_PATCH_3
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(388), FACING_SOUTH, TALK_FACING_KEEP, 0 },
#endif
    { SCENE_TABLE_END },
};

/// Unused, as gMcCoyPalaceGroundsNoActorPlacements is.
struct Trigger gMcCoyPalaceGroundsNoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gMcCoyPalaceGroundsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_GROUNDS_TO_BILIBIN },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(McCoyPalaceGrounds_EnterPalace) },
    { TRIGGER_TALK, ACTOR_WEST_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
    { TRIGGER_TALK, ACTOR_WEST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
    { TRIGGER_TALK, ACTOR_NORTH_WORKER, FLAG_KOLIMA_CURSE_LIFTED, EVENT(McCoyPalaceGrounds_TalkToNorthWorker) },
    { TRIGGER_TALK, ACTOR_NORTH_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
    { TRIGGER_TALK, ACTOR_LADY_MCCOY, FLAG_KOLIMA_CURSE_LIFTED, EVENT(McCoyPalaceGrounds_TalkToLadyMcCoy) },
    { TRIGGER_TALK, ACTOR_LADY_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinICantTellYouAnythingElse) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, EVENT(McCoyPalaceGrounds_TalkToWestGuard) },
    { TRIGGER_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, EVENT(McCoyPalaceGrounds_TalkToEastGuard) },
    { TRIGGER_TALK, ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
    { TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 90, CONDITION_ALWAYS, EVENT(McCoyPalaceGrounds_SearchWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, EVENT(McCoyPalaceGrounds_CatchFloatingNut) },
    { TRIGGERS_END },
};
// clang-format on
