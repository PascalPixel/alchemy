/// @file The grounds of McCoy's Palace in Bilibin.
///
/// The scene is SceneId_McCoyPalaceGrounds, with the area name McCoy's Palace,
/// which the palace itself shares (mccoy_palace.c). Workers build Lady McCoy a
/// new palace here, and two guards keep the gate of McCoy's own.
///
/// Its people and talk follow two story flags. One is
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, set once McCoy has turned the
/// party away. The other is FLAG_KOLIMA_CURSE_LIFTED.
///
/// Talking to the east guard (McCoyPalaceGrounds_TalkToEastGuard) is the way
/// in. On yes the guards confer, the gate opens (McCoyPalaceGrounds_OpenGate)
/// and the west guard goes in ahead (FLAG_MCCOY_PALACE_ADMITTED). Walking north
/// into the gate (step trigger 2, McCoyPalaceGrounds_EnterPalace) takes
/// MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE into the palace.
///
/// The guard's scolding for keeping McCoy waiting is unused
/// (McCoyPalaceGrounds_TalkToEastGuard).
///
/// Once the curse is lifted, the first arrival until McCoy has rewarded the
/// party (FLAG_MCCOY_PALACE_PARTY_REWARDED) plays
/// McCoyPalaceGrounds_GuardsArrestParty. The guards repeat their accusation
/// until the player says yes. Then they march the party into the palace
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST), where McCoy rewards it.
///
/// McCoyPalaceGrounds_RestoreStory places the guards by the same flags. It
/// also floats a Nut over the grounds until Catch takes it
/// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT).
///
/// A dry well can be searched, and MCCOY_PALACE_GROUNDS_TO_BILIBIN leads back
/// to Bilibin.
///
/// From TBS_PATCH_3 on, a patch of ivy (SPRITE_IVY) stands at (132, 388), over
/// a dark opening at the foot of the cliff on the grounds' south edge. The
/// earlier editions leave the opening bare. No
/// exit or trigger answers that opening, so the ivy only hides a way in that
/// leads nowhere.

#include "system/main_routine_work.h"
#include "types.h"
#include "field/scene_tables.h"

extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct SceneEntrance gMcCoyPalaceGroundsEntrances[];
extern u32 gMcCoyPalaceGroundsExits[];
extern struct ActorPlacement gMcCoyPalaceGroundsNoActorPlacements[];
extern struct ActorPlacement gMcCoyPalaceGroundsActorPlacements[];
extern struct Trigger gMcCoyPalaceGroundsNoTriggers[];
extern struct Trigger gMcCoyPalaceGroundsTriggers[];

#include "scene/actor_orbit.h"
#include "field/event_commands.h"
#include "system/system.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/event.h"

#include "item_ids.h"

#include "field/event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field/actor_script.h"
#include "flags.h"
#include "scene/catch_nut.h"

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define MCCOY_GARDEN_SET_ACTOR_POSITION Import_ActorSetPosition
#else
#define MCCOY_GARDEN_SET_ACTOR_POSITION ((s32 (*)())Import_ActorSetPosition)
#endif

/// The placed actors, numbered in the order of
/// gMcCoyPalaceGroundsActorPlacements.
enum McCoyPalaceGroundsActorId {
  /// A worker with a pickaxe by the new palace, drawn in front. "The lumber I
  /// ordered from Kolima disappeared..."
  ACTOR_WEST_WORKER = ACTOR_FIRST_PLACED,
  /// The worker with a pickaxe who wanders by the new palace. "Have you seen
  /// the barricade we built on..."
  ACTOR_NORTH_WORKER,
  /// A woman with curly purple hair in a pink dress. "Hmph! My palace is not
  /// finished!"
  ACTOR_LADY_MCCOY,
  /// The soldier in a spiked helmet at the gate who goes in ahead of the party.
  /// "Lord McCoy is so upset about this curse that he can't even eat."
  ACTOR_WEST_GUARD,
  /// "The lord will only meet with brave..."
  ACTOR_EAST_GUARD,
  /// The Nut over the grounds, gone once Catch has taken it
  /// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT).
  ACTOR_FLOATING_NUT
};

/// The grounds' temporary flags.
enum {
  /// The palace gate stands open (McCoyPalaceGrounds_OpenGate), until the scene
  /// is left.
  FLAG_GATE_OPEN = FLAG_SCENE_TEMP_00,
  /// The east guard has scolded the party for keeping McCoy waiting.
  FLAG_GUARD_SCOLDED = FLAG_SCENE_TEMP_01,
  /// Set by Bilibin (bilibin.c) while the guards have let the party in but it
  /// has not yet entered the palace.
  /// The grounds are in another area than Bilibin, so entering them clears it
  /// before the guard can read it.
  FLAG_KEEPING_MCCOY_WAITING = FLAG_AREA_TEMP_04
};

static void McCoyPalaceGrounds_RestoreStory(void);
static void McCoyPalaceGrounds_GuardsArrestParty(void);
static void McCoyPalaceGrounds_OpenGate(void);

#include "shared/actor_facing.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceGroundsEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceGroundsExits; }

/// The people outside McCoy's Palace.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
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
  Scene_CatchNut(ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT);
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
  if (gMainRoutineWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
    return gMcCoyPalaceGroundsTriggers;
  }
  return gMcCoyPalaceGroundsNoTriggers;
}

/// McCoy's Palace, outside, before the curse is lifted: the north worker asks
/// whether the party has seen the barricade they built on the road to Kolima.
static void McCoyPalaceGrounds_TalkToNorthWorker(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveYouSeenBarricadeWe);
  // "Have you seen the barricade we built on the road to Kolima?"
  // Yes: "It's pretty shabby, because it was built on short notice......"
  // No: "It's off to the east, if you want to see it. It was a rush..."
  Event_AskYesNo(ACTOR_NORTH_WORKER, 0);
  Event_End();
}

/// McCoy's Palace, outside, before the curse is lifted: Lady McCoy asks
/// whether the party blames her for the curse.
static void McCoyPalaceGrounds_TalkToLadyMcCoy(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinTellingMeImResponsibleFor);
  // "Are you telling me that I'm responsible for this?"
  // Yes: "Sure, I had them cut the tree down so I could build my..."
  // No: "That curse of the tree spirit is a foolish old superstition!..."
  Event_AskYesNo(ACTOR_LADY_MCCOY, 0);
  Event_End();
}

/// McCoy's Palace, outside, after the reward: the west guard asks whether he
/// can become as fine a warrior as the leader.
static void McCoyPalaceGrounds_TalkToWestGuard(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoThinkCanBecomeAs);
  // "Do you think I can become as fine a warrior as you?"
  // Yes: "Really? Then I will try my best!"
  // No: "Don't be so harsh... Just say yes, even if you don't mean it."
  Event_AskYesNo(ACTOR_WEST_GUARD, 0);
  Event_End();
}

/// McCoy's Palace, outside, until McCoy turns the party away: the east guard
/// asks whether the party is brave enough to meet his lord.
///
/// Unused: the branch for a party back from Bilibin never runs. In it he
/// scolds the party once for keeping McCoy waiting (MsgBilibinWhere,
/// `Where were you?`, and MsgBilibinMasterQuiteCranky). The branch is never
/// reached because FLAG_KEEPING_MCCOY_WAITING is cleared on entering the
/// grounds.
static void McCoyPalaceGrounds_TalkToEastGuard(void) {
  Event_Start();
  // Once admitted, the party only receives the guard's current reminder.
  if (Event_GetFlag(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    if (GameFlag_Get(FLAG_KEEPING_MCCOY_WAITING) != 0) {
      if (Event_GetFlag(FLAG_GUARD_SCOLDED) == 0) {
        Event_SetMessageId(MsgBilibinWhere);
        // "Where were you?"
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
        // "Lord McCoy is angry because he couldn't find you."
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
        GameFlag_Set(FLAG_GUARD_SCOLDED);
      }
      Event_SetMessageId(MsgBilibinMasterQuiteCranky);
      // "The master can be quite cranky. Hurry along, now!"
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
      Event_SetMessageId(MsgBilibinLordMccoyDifficult);
      // "Lord McCoy is a very difficult man. Be careful how you act..."
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
      goto guard;
    }
  } else {
    // On the first visit, the guards confer before opening the gate.
    Event_SetMessageId(MsgBilibinLordOnlyMeet);
    // "The lord will only meet with brave and mighty warriors. You..."
    Event_OpenMessage(ACTOR_EAST_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldWork->message++;
      // "Hmmm, I see... You seem pretty confident..."
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
      if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed < px(270)) {
        Actor_SetSpeed(ACTOR_EAST_GUARD, 0.8);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 346, 274);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 282);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
      }
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_WEST_SOUTHWEST, 20);
      Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
      Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      // "What do you say? They look a little young to get involved in..."
      Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
      Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
      Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 1);
      // "Nobody else seems up to the challenge, though. What do we..."
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
      Actor_SetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
      Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 1);
      // "All right. I shall let you pass. Follow me!"
      Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST_NORTHEAST, 0);
      Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
      Actor_Get(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
      Event_Wait(1);
      Actor_Get(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
      Actor_SetSpeed(ACTOR_WEST_GUARD, 0.6);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 263);
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 252);
      Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 10);
      McCoyPalaceGrounds_OpenGate();
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 246);
      MCCOY_GARDEN_SET_ACTOR_POSITION(ACTOR_WEST_GUARD, 0, 0);
      /* FAKEMATCH(dead): an empty once-only loop. */
      do {
      } while (0);
      GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
    guard:
      FieldActor_WatchAndStartScript(
          ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
          gGuardWatchesParty);
    } else {
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHEAST, 10);
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
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE);
  Event_End();
}

/// Sets up the grounds outside McCoy's Palace.
s32 Scene_Initialize(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *actor;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  actor = Actor_Get(ACTOR_WEST_WORKER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 1;

  if (gMainRoutineWork.next == SCENE_MC_COY_PALACE_GROUNDS) {
    McCoyPalaceGrounds_RestoreStory();
  }

  return 0;
}

/// The guards stand where the party left them.
static void McCoyPalaceGrounds_RestoreStory(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_GATE_OPEN);
  }
  if (GameFlag_Get(FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT) == 0) {
    Scene_FloatNut(ACTOR_FLOATING_NUT);
  }
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    Actor_SetPosition(ACTOR_WEST_GUARD, 308, 263);
    Actor_SetPosition(ACTOR_EAST_GUARD, 346, 263);
    if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
      if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
        Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
        FieldActor_WatchAndStartScript(
            ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gGuardWatchesParty);
      }
    }
  }
  // Lady McCoy's new place and the arrest come only after the curse lifts.
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(ACTOR_LADY_MCCOY, 224, 292);
    Actor_FaceDirection(ACTOR_LADY_MCCOY, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_WEST_WORKER, FACING_EAST, 0);
    if (GameFlag_Get(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
      McCoyPalaceGrounds_GuardsArrestParty();
    }
  }
}

/// McCoy's Palace, outside, on entry once Kolima's curse is lifted: the guards
/// stop the party, ask whether they broke the curse of the holy tree until
/// they answer yes, and march them into the palace to McCoy.
static void McCoyPalaceGrounds_GuardsArrestParty(void) {
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveTo(320, CAMERA_KEEP, 320, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 320, 372);
  Event_StartScreenTransition();
  FieldCamera_SetSpeed(0.2);
  FieldCamera_MoveTo(320, CAMERA_KEEP, 290, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 320, 310);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_SetMessageId(MsgBilibinOhItS);
  // "Oh! It's..."
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 2);
  Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "Oh! Sir!"
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_WEST_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  // "We've been looking everywhere for you!"
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 1);
  // "The curse of the holy tree... You're the ones who broke it,..."
  Event_OpenMessage(ACTOR_EAST_GUARD, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Refusals repeat the accusation until the player acknowledges it.
  while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Event_SetMessageId(MsgBilibinNotTrueWitnesses);
    // "That's not true. We have witnesses who say they saw you in..."
    Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 2);
    // "It was you, wasn't it?"
    Event_OpenMessage(ACTOR_EAST_GUARD, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimation(ACTOR_WEST_GUARD, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 1);
  Event_SetMessageId(MsgBilibinUnderArrest);
  // "You're under arrest!"
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 314, 280);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  // "Let's go. Lord McCoy is waiting."
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) == 0) {
    Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
    Actor_Get(ACTOR_EAST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_EAST_GUARD, 346, 263);
    Event_Wait(1);
    Actor_Get(ACTOR_EAST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0.8);
  // Escort Isaac through the opened gate before changing to the palace.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkTo(ACTOR_WEST_GUARD, 328, 262);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 278);
  Actor_SetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
  McCoyPalaceGrounds_OpenGate();
  Event_Wait(40);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 242);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 242);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST);
  Event_End();
}

/// The gate of McCoy's Palace swings open.
static void McCoyPalaceGrounds_OpenGate(void) {
  Sound_Play(SE_THUMP);
  MapGrid_PlayMapEdit(gPalaceGateOpenSteps, 52, 11);
  GameFlag_Set(FLAG_GATE_OPEN);
}

#include "shared/actor_orbit.c"

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
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        103, 0, 280, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        135, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        128, 0, 327, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        320, 0, 263, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        334, 0, 263, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
#if TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 388, FACING_SOUTH, TALK_FACING_KEEP, 0),
#endif
    { SCENE_TABLE_END },
};

/// Unused, as gMcCoyPalaceGroundsNoActorPlacements is.
struct Trigger gMcCoyPalaceGroundsNoTriggers[] = {
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};
// clang-format on
