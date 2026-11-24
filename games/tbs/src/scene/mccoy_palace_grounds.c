/// @file
/// The grounds of McCoy's Palace, at the top of Bilibin.
///
/// The grounds are their own scene, SceneId_McCoyPalaceGrounds, sharing the
/// area name McCoy's Palace with the palace itself (mccoy_palace.c). Steps
/// lead up from Bilibin to a sandy yard before the palace door, where two
/// soldiers in spiked helmets stand guard. West of the yard, workers with
/// pickaxes have been building Lady McCoy a new palace, stopped since the
/// lumber from Kolima vanished, and Lady McCoy herself complains of it. A Nut
/// hangs in the orange tree west of the steps for Catch, and a dry stone well
/// stands by the palace's east corner.
///
/// Talking to the east guard is the way in
/// (McCoyPalaceGrounds_TalkToEastGuard). He asks whether the party is up to
/// meeting his lord; on yes the two guards confer, the west guard opens the
/// door (McCoyPalaceGrounds_OpenDoor) and goes in ahead, and walking north into
/// the doorway takes the party inside (McCoyPalaceGrounds_EnterPalace). After
/// McCoy has turned it away, the guards only talk.
///
/// The first time the party comes back once Kolima's curse is lifted, the
/// guards stop it on the steps, ask whether it broke the curse of the holy
/// tree until Isaac says yes, put it "under arrest" and march it straight into
/// McCoy's room for its reward (McCoyPalaceGrounds_GuardsArrestParty). After
/// the curse, Lady McCoy stands at the west edge of the yard.
///
/// Its people follow three story flags: FLAG_MCCOY_PALACE_ADMITTED,
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY (set once McCoy has turned the
/// party away) and FLAG_KOLIMA_CURSE_LIFTED; McCoyPalaceGrounds_RestoreStory
/// places them by those flags.
///
/// From TBS_PATCH_3 on, a patch of ivy (SPRITE_IVY) grows at cell (8, 24), over
/// a dark opening at the foot of the cliff on the grounds' south edge. The
/// earlier editions leave the opening bare. No exit or trigger answers that
/// opening, so the ivy only hides a way in that leads nowhere.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "sound/sound.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern u16 gMcCoyPalaceGroundsDoorOpenSteps[];
extern s32 gMcCoyPalaceGroundsGuardWatchesParty[];
extern struct SceneEntrance gMcCoyPalaceGroundsEntrances[];
extern u32 gMcCoyPalaceGroundsExits[];
extern struct ActorPlacement gMcCoyPalaceGroundsNoActorPlacements[];
extern struct ActorPlacement gMcCoyPalaceGroundsActorPlacements[];
extern struct Trigger gMcCoyPalaceGroundsNoTriggers[];
extern struct Trigger gMcCoyPalaceGroundsTriggers[];

/// The placed actors, numbered in the order of
/// gMcCoyPalaceGroundsActorPlacements.
enum McCoyPalaceGroundsActorId {
  /// A worker with a pickaxe at the west edge of the yard, by the new palace.
  /// Before the curse is lifted: "The lumber I ordered from Kolima disappeared
  /// after the incident."
  ACTOR_WEST_WORKER = ACTOR_FIRST_PLACED,
  /// The worker with a pickaxe who wanders by the new palace, north of the
  /// other. "Have you seen the barricade we built on the road to Kolima?"
  ACTOR_NORTH_WORKER,
  /// Lady McCoy, a woman with curly purple hair in a pink dress, at the foot of
  /// the new palace. Once the curse is lifted she stands at the west edge of
  /// the yard: "Hmph! My palace is not finished!"
  ACTOR_LADY_MCCOY,
  /// The west guard at the palace door, a soldier in a spiked helmet, who goes
  /// in ahead of the party. "Lord McCoy is so upset about this curse that he
  /// can't even eat."
  ACTOR_WEST_GUARD,
  /// The east guard at the palace door, who decides who may see McCoy. "The
  /// lord will only meet with brave and mighty warriors."
  ACTOR_EAST_GUARD,
  /// The Nut in the orange tree west of the steps, gone once Catch has taken it
  /// (FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT).
  ACTOR_FLOATING_NUT
};

/// The stone well by the palace's east corner, cells (28, 16) to (29, 17),
/// which the triggers answer besides the exits, whose cell behaviors are their
/// exits' numbers.
enum McCoyPalaceGroundsCellBehavior {
  CELL_BEHAVIOR_MCCOY_PALACE_GROUNDS_WELL = 90
};

/// The grounds' temporary flags.
enum {
  /// The palace door stands open (McCoyPalaceGrounds_OpenDoor), until the scene
  /// is left.
  FLAG_DOOR_OPEN = FLAG_SCENE_TEMP_00,
  /// The east guard has scolded the party for keeping McCoy waiting.
  FLAG_GUARD_ASKED_WHERE_YOU_WERE = FLAG_SCENE_TEMP_01,
  /// Set by Bilibin (bilibin.c) while the guards have let the party in but it
  /// has not yet entered the palace.
  /// The grounds are in another area than Bilibin, so entering them clears it
  /// before the guard can read it.
  FLAG_KEEPING_MCCOY_WAITING = FLAG_AREA_TEMP_04
};

static void McCoyPalaceGrounds_RestoreStory(void);
static void McCoyPalaceGrounds_GuardsArrestParty(void);
static void McCoyPalaceGrounds_OpenDoor(void);

#include "shared/actor_facing.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceGroundsEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceGroundsExits; }

/// The people outside McCoy's Palace.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_MC_COY_PALACE_GROUNDS) {
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

/// Catch on the Nut in the orange tree west of the steps pulls it down and
/// gives it to the party.
static void McCoyPalaceGrounds_CatchFloatingNut(void) {
  Scene_CatchNut(ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT);
}

/// Searching the stone well by the palace's east corner (cells (28, 16) to
/// (29, 17)): "Isaac peered into the well... Upon closer inspection, it seems
/// the well is dried up." Nothing is found.
static void McCoyPalaceGrounds_SearchWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgBilibinUponCloserInspectionSeemsDried, 1);
  Event_End();
}

/// What the party can talk to, search and catch outside McCoy's Palace.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_MC_COY_PALACE_GROUNDS) {
    return gMcCoyPalaceGroundsTriggers;
  }
  return gMcCoyPalaceGroundsNoTriggers;
}

/// Before the curse is lifted, the wandering worker asks whether the party has
/// seen the barricade they built on the road to Kolima. On yes he admits it is
/// pretty shabby, built on short notice; on no he says it is off to the east.
static void McCoyPalaceGrounds_TalkToNorthWorker(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveYouSeenBarricadeWe);
  // "Have you seen the barricade we built on the road to Kolima?"
  // Yes: "It's pretty shabby, because it was built on short notice......"
  // No: "It's off to the east, if you want to see it. It was a rush..."
  Event_AskYesNo(ACTOR_NORTH_WORKER, 0);
  Event_End();
}

/// Before the curse is lifted, Lady McCoy asks "Are you telling me that I'm
/// responsible for this?" On yes she owns that she had the tree cut down for
/// her palace, but calls herself the victim; on no she calls the curse a
/// foolish old superstition.
static void McCoyPalaceGrounds_TalkToLadyMcCoy(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinTellingMeImResponsibleFor);
  // "Are you telling me that I'm responsible for this?"
  // Yes: "Sure, I had them cut the tree down so I could build my..."
  // No: "That curse of the tree spirit is a foolish old superstition!..."
  Event_AskYesNo(ACTOR_LADY_MCCOY, 0);
  Event_End();
}

/// After the reward, the west guard asks "Do you think I can become as fine a
/// warrior as you?" On yes he will try his best; on no he begs Isaac to just
/// say yes.
static void McCoyPalaceGrounds_TalkToWestGuard(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoThinkCanBecomeAs);
  // "Do you think I can become as fine a warrior as you?"
  // Yes: "Really? Then I will try my best!"
  // No: "Don't be so harsh... Just say yes, even if you don't mean it."
  Event_AskYesNo(ACTOR_WEST_GUARD, 0);
  Event_End();
}

/// The east guard at the palace door asks whether the party is up to meeting
/// his lord. On yes he and the west guard confer, decide nobody else seems up
/// to the challenge, and the west guard says "All right. I shall let you pass.
/// Follow me!", opens the door and goes in; on no the guard says he cannot
/// send such inexperienced children. Once the party is let in, he warns that
/// Lord McCoy is a very difficult man, and keeps turning to watch Isaac.
/// After McCoy has turned the party away, he only talks.
///
/// Unused: the branch for a party back from Bilibin never runs. In it he
/// scolds the party once for keeping McCoy waiting (MsgBilibinWhere,
/// `Where were you?`, and MsgBilibinMasterQuiteCranky). The branch is never
/// reached because FLAG_KEEPING_MCCOY_WAITING is cleared on entering the
/// grounds.
static void McCoyPalaceGrounds_TalkToEastGuard(void) {
  Event_Start();
  // Once the party is let in, the guard only warns it about McCoy.
  if (Event_GetFlag(FLAG_MCCOY_PALACE_ADMITTED) != 0) {
    if (GameFlag_Get(FLAG_KEEPING_MCCOY_WAITING) != 0) {
      if (Event_GetFlag(FLAG_GUARD_ASKED_WHERE_YOU_WERE) == 0) {
        Event_SetMessageId(MsgBilibinWhere);
        // "Where were you?"
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
        // "Lord McCoy is angry because he couldn't find you."
        Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
        GameFlag_Set(FLAG_GUARD_ASKED_WHERE_YOU_WERE);
      }
      Event_SetMessageId(MsgBilibinMasterQuiteCranky);
      // "The master can be quite cranky. Hurry along, now!"
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
      Event_SetMessageId(MsgBilibinLordMccoyDifficult);
      // "Lord McCoy is a very difficult man. Be careful how you act..."
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
      FieldActor_WatchAndStartScript(
          ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
          gMcCoyPalaceGroundsGuardWatchesParty);
    }
  } else {
    // On the first visit, the guards confer before opening the door.
    Event_SetMessageId(MsgBilibinLordOnlyMeet);
    // "The lord will only meet with brave and mighty warriors. You..."
    Event_OpenMessage(ACTOR_EAST_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldState->message++;
      // "Hmmm, I see... You seem pretty confident..."
      Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
      if (Actor_Get(ACTOR_PARTY_LEADER)->z.fixed < toFIXED(270)) {
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
      McCoyPalaceGrounds_OpenDoor();
      Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 246);
      Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
      GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
      FieldActor_WatchAndStartScript(
          ACTOR_EAST_GUARD, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
          gMcCoyPalaceGroundsGuardWatchesParty);
    } else {
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
      Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH_SOUTHEAST, 10);
    }
  }
  Event_End();
}

/// Walking north into the palace doorway (cell (20, 16)), once the guards have
/// let the party in and stepped aside: the door swings open if it
/// is shut, Isaac walks through, and the palace's hall appears.
static void McCoyPalaceGrounds_EnterPalace(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_DOOR_OPEN) == 0) {
    McCoyPalaceGrounds_OpenDoor();
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE);
  Event_End();
}

/// Sets up the grounds outside McCoy's Palace, and plays the guards' arrest
/// once the curse is lifted.
s32 Scene_Initialize(void) {
  struct FieldState *state = gFieldState;
  struct Actor *actor;

  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  actor = Actor_Get(ACTOR_WEST_WORKER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 1;

  if (gMainRoutineState.next == SCENE_MC_COY_PALACE_GROUNDS) {
    McCoyPalaceGrounds_RestoreStory();
  }

  return 0;
}

/// Places the people by the story. After the guards let the party in, the
/// west guard is gone inside and the east guard stands aside, watching Isaac,
/// until McCoy has turned the party away; then both stand either side of the
/// door. Once the curse is lifted, Lady McCoy moves to the west edge of the
/// yard and the worker by the new palace turns east, and the guards' arrest
/// plays until McCoy has rewarded the party. The Nut is hung in its tree until
/// it is caught.
static void McCoyPalaceGrounds_RestoreStory(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_DOOR_OPEN);
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
            gMcCoyPalaceGroundsGuardWatchesParty);
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

/// The first time the party climbs the steps after lifting Kolima's curse, the
/// guards cry "Oh! Sir!" and "We've been looking everywhere for you!", and ask
/// "The curse of the holy tree... You're the ones who broke it, right?" Each
/// no brings "That's not true. We have witnesses who say they saw you in the
/// forest!" and the question again. On yes the west guard declares "You're
/// under arrest!", opens the door and marches Isaac straight into McCoy's
/// room.
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
  // Each No brings the accusation again, until Isaac says yes.
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
  // The west guard marches Isaac through the door into the palace.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkTo(ACTOR_WEST_GUARD, 328, 262);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 278);
  Actor_SetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
  McCoyPalaceGrounds_OpenDoor();
  Event_Wait(40);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 242);
  Actor_WalkToAndWait(ACTOR_WEST_GUARD, 328, 242);
  Actor_SetPosition(ACTOR_WEST_GUARD, 0, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST);
  Event_End();
}

/// The palace door swings open with a thump.
static void McCoyPalaceGrounds_OpenDoor(void) {
  Sound_Play(SE_THUMP);
  MapGrid_PlayMapEdit(gMcCoyPalaceGroundsDoorOpenSteps, 52, 11);
  GameFlag_Set(FLAG_DOOR_OPEN);
}

#include "shared/floating_nut.c"

// clang-format off
/// The door's opening, drawn in three steps of 5 frames each from the spare
/// cells at (34, 25) to (36, 27).
u16 gMcCoyPalaceGroundsDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(34, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(35, 25, 1, 3, 5), MAPGRID_ANIMATION_STEP(36, 25, 1, 3, 5), MAPGRID_ANIMATION_STEPS_END,
};

/// A guard turns to keep facing Isaac.
s32 gMcCoyPalaceGroundsGuardWatchesParty[] = {
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
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, CONDITION_ALWAYS, EVENT(McCoyPalaceGrounds_EnterPalace) },
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_GROUNDS_WELL, CONDITION_ALWAYS, EVENT(McCoyPalaceGrounds_SearchWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT, FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT, EVENT(McCoyPalaceGrounds_CatchFloatingNut) },
    { TRIGGER_TABLE_END },
};
// clang-format on
