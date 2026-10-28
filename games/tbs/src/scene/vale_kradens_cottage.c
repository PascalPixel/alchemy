/// @file
/// Vale (SceneId_ValeKradensCottage): the part of the village around Kraden's
/// cottage, by the fence that closes off the lands beyond. Kraden, "a trained
/// Alchemy sage" in his own words, came to Vale to study Mt. Aleph and Alchemy.
/// One actor placement table serves every visit:
/// ValeKradensCottage_Initialize removes the people a visit does not use, and
/// ValeKradensCottage_GetTriggers picks the events for the time of the story.
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). The storm's scene
/// (Vale,
///   SceneId_ValeStormyNight) leads here by
///   VALE_STORMY_NIGHT_TO_VALE_KRADENS_COTTAGE_NORTH and
///   VALE_STORMY_NIGHT_TO_VALE_KRADENS_COTTAGE_SOUTH, arriving by
///   VALE_KRADENS_COTTAGE_FROM_VALE_NORTH and
///   VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH, and the two paths lead back
///   (VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH and
///   VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH). A rockslide has broken
///   the fence and let monsters in: Initialize starts the lightning, leaves the
///   two rocks that Isaac cannot budge across the way, copies
///   CELL_BEHAVIOR_WALL over the path's six cells below the west rock and under
///   the east one, two of which by day hold
///   ValeKradensCottage_MeetSaturosAndMenardi's cell behavior 7, and lays a
///   hurt man by the fence. Talking to him, or walking up to him (cell behavior
///   20), plays ValeKradensCottage_TalkToRockslideSurvivor: asked whether he
///   will die, a yes leaves him giving up
///   (FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP), and a no has him find he is not
///   hurt at all, warn them of the monsters on their way to the plaza and run
///   off (FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED), after which he is no longer
///   placed. Two cells start a monster battle each, once (the first two slots
///   of the encounter zone of the scripted battles of Vale, Vault and the world
///   map).
/// - Three years later, the day the story begins, Isaac comes up with Garet and
///   Jenna to find two strangers outside. Walking past them (cell behavior
///   7) plays ValeKradensCottage_MeetSaturosAndMenardi: the three overhear the
///   two speak of Sol Sanctum and of using Kraden, confront them and are let
///   go, the two calling each other Saturos and Menardi
///   (FLAG_MET_SATUROS_AND_MENARDI), and the strangers then stand aside,
///   turning to watch the leader. Near the cottage (cell behavior 5), the
///   party overhears Kraden wondering what the strangers know of the Elemental
///   Stars (ValeKradensCottage_OverhearKraden,
///   FLAG_KRADEN_WONDERED_ABOUT_STRANGERS). Talking to Kraden plays
///   ValeKradensCottage_KradenPlansSanctumVisit: Garet brings up Saturos and
///   Menardi, and Kraden talks the three into a secret look at Sol Sanctum on
///   the forbidden mountain, then goes in to prepare, and
///   VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG takes the party into
///   the cottage (SceneId_CottageRoomsB,
///   COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG). Coming back out by
///   VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH, Kraden joins them for Mt.
///   Aleph (ValeKradensCottage_KradenSetsOff, FLAG_SANCTUM_VISIT_PLANNED); from
///   then on he and the strangers are gone. A small boy at the fence hears
///   growls beyond it, and a jar holds an Elixir.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT) the child means to
///   climb the fence one day, and after Kalay (FLAG_KALAY_REACHED) a woman with
///   fair hair visits the empty cottage, missing Kraden.
///
/// By day the cottage's door (ValeKradensCottage_EnterCottage) leads into the
/// cottage (VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH, to
/// SceneId_CottageRoomsB's COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH),
/// VALE_KRADENS_COTTAGE_TO_VALE_NORTH and VALE_KRADENS_COTTAGE_TO_VALE_SOUTH go
/// down into the village (SceneId_Vale) and
/// VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST and
/// VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST into Vale Cave's way in
/// (SceneId_ValeCaveMouth). Coming back from it by
/// VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST clears
/// FLAG_SHOW_AREA_NAME, so the area name is not shown again. The jar's row
/// finds nothing on the stormy night, and after Kalay it is there only from the
/// third patch on.

#include "adventure.h"
#include "unit/import.h"
#include "unit/scene/vale_kradens_cottage.h"
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
#include "call.h"
#include "types.h"
#include "event_layer.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// The placed actors, numbered in the order of
/// gValeKradensCottage_ActorPlacements; Garet and Jenna, placed between
/// Kraden and Saturos, keep their own slots.
enum ValeKradensCottageActorId {
  /// A boulder by the fence, with the sprite of the boulder Lift raises at the
  /// Alpine Crossing (MsgMountainIsaacDidLiftBoulder) and in Gondowan Cave. Its
  /// Lift row runs no event, like ACTOR_RIVER_BOULDER's in gondowan_cave.c,
  /// and the stormy night's events have no row for it. Cast on it, Lift's two
  /// hands close on it and burst into sparkles, the boulder stays where it is,
  /// and no message follows, since the row answers the cast (render probe
  /// kraden-lift, by day and after the party has left Vale).
  /// ★ Named from the game: "did you lift the boulder?"
  /// (MsgMountainIsaacDidLiftBoulder), said of the same sprite
  ACTOR_FENCE_BOULDER = ACTOR_FIRST_PLACED,
  /// The west rock. "It's a huge rock that Isaac's strength can't even budge."
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  ACTOR_WEST_ROCK,
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  ACTOR_EAST_ROCK,
  /// A young man with brown hair who lies by the broken fence on the stormy
  /// night. "Rockslide...destroyed fence... Monsters everywhere..."
  /// ★ Named from the game: "Rockslide...destroyed fence..."
  /// (MsgValeRockslideDestroyedFence)
  ACTOR_ROCKSLIDE_SURVIVOR,
  /// A small boy with fair hair, by day, younger than Isaac. "Do you hear those
  /// awful growls coming from the other side of this fence?"
  /// ★ Named from the game: "I can't stop wondering what lies beyond this
  /// fence." (MsgValeWonderBeyondFence)
  ACTOR_BOY_BY_THE_FENCE,
  /// ★ Named from the game: "from you, Kraden?" (MsgValeKradenOhIsaac)
  ACTOR_KRADEN,
  /// ★ Named from the game: "Saturos and Menardi were their names"
  /// (MsgValeKradenOhIsaac)
  ACTOR_SATUROS,
  /// ★ Named from the game: "Saturos and Menardi were their names"
  /// (MsgValeKradenOhIsaac)
  ACTOR_MENARDI,
  /// A woman with fair hair, after Kalay. "You must have so many memories of
  /// this cottage, don't you?"
  /// ★ Named from the game: "You must have so many memories of this cottage"
  /// (MsgValeMemoriesOfThisCottage)
  ACTOR_COTTAGE_VISITOR
};

#include "shared/actor_facing.h"

/// The front door of Kraden's cottage stands open since Kraden set off, so
/// walking in (cell behavior 3, the exit into the cottage) skips its
/// opening.
/// ★ Named from what the player sees: the arched wooden door of Kraden's
/// thatched cottage (its render)
#define FLAG_VALE_KRADENS_DOOR_OPEN FLAG_SCENE_TEMP_42

const struct SceneEntrance *ValeKradensCottage_GetEntrances(void) {
  return gValeKradensCottage_Entrances;
}

const struct SceneExitRegion *ValeKradensCottage_GetRegions(void) {
  return NULL;
}

const u32 *ValeKradensCottage_GetExits(void) {
  return gValeKradensCottage_Exits;
}

/// Returns the one actor placement table for every visit;
/// ValeKradensCottage_Initialize removes the actors a visit does not
/// use.
const struct ActorPlacement *ValeKradensCottage_GetActorPlacements(void) {
  return gValeKradensCottage_ActorPlacements;
}

/// Selects the events for the current visit to Vale: after Kalay
/// (FLAG_KALAY_REACHED), on the stormy night (FLAG_VALE_STORM), else the day
/// the story begins.
const struct Trigger *ValeKradensCottage_GetTriggers(void) {
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gValeKradensCottage_TriggersAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValeKradensCottage_StormTriggers;
  }
  return gValeKradensCottage_Triggers;
}

/// Finishes the party's nod together before the next line of dialogue.
static __inline__ void ValeKradensCottage_NodTogether(void) {
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
}

/// Turns all three party members toward the strangers.
static __inline__ void ValeKradensCottage_FacePartySouth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
}

/// Starts a companion walking back to Isaac's current pixel position.
static __inline__ void ValeKradensCottage_WalkTowardLeader(s32 companionId) {
  struct Actor *leader;

  Import_ActorSetAnimation(companionId, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(companionId, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
}

/// In Vale by Kraden's cottage, the child at the fence asks whether Isaac hears
/// the growls beyond it; once the party has left Vale, he means to climb it
/// someday.
///
/// Reached by talking to the boy by the fence (ACTOR_BOY_BY_THE_FENCE) in the
/// day's events.
static void ValeKradensCottage_TalkToBoyByTheFence(void) {
  s32 message;

  Event_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Event_SetMessageId(MsgValeIllClimbTheFenceSomeday);
    Event_ShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
  } else {
    message = MsgValeHearAwfulGrowls;
    Event_SetMessageId(message);
    Actor_FaceActor(ACTOR_BOY_BY_THE_FENCE, ACTOR_PARTY_LEADER, 10);
    Actor_RunRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 2);
    Event_WaitFrames(6);
    Event_OpenMessage(ACTOR_BOY_BY_THE_FENCE, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId(message + 1);
    } else {
      Event_SetMessageId(message + 2);
    }
    Actor_StartRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 3);
    Event_ShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
    Actor_FaceDirection(ACTOR_BOY_BY_THE_FENCE, FACING_NORTH, 10);
  }
  Event_End();
}

/// In Vale, talking to Kraden outside his cottage: he talks Isaac, Garet and
/// Jenna into a secret look at Sol Sanctum, then goes inside to prepare.
///
/// Reached by talking to Kraden in the day's events, until
/// FLAG_SANCTUM_VISIT_PLANNED is set. Ends by taking
/// VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG into the cottage
/// (SceneId_CottageRoomsB,
/// COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG); the party comes back
/// by VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH, where
/// ValeKradensCottage_KradenSetsOff goes on.
static void ValeKradensCottage_KradenPlansSanctumVisit(void) {
  struct Actor *leader;
  s32 x, z;
  s32 sneakingMessage;

  if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    // Isaac walks up to Kraden, who greets him.
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Event_SetMessageId(MsgValeKradenOhIsaac);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 264);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_KRADEN, 20);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // Garet and Jenna step out of Isaac and stand either side of him.
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    Actor_SetPosition(ACTOR_JENNA, x * Q16_ONE, z * Q16_ONE);
    Actor_SetPosition(ACTOR_GARET, x * Q16_ONE, z * Q16_ONE);
    Actor_SetSpeed(ACTOR_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkTo(ACTOR_JENNA, 248, 264);
    Actor_WalkToAndWait(ACTOR_GARET, 216, 264);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Event_WaitFrames(4);
    // Jenna sees something is bothering Kraden; he hedges.
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // Garet names Saturos and Menardi; Kraden finds them persistent.
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 8);
    // Jenna asks whether they wanted something from him; it seems so.
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST + FACING_STEP, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 6);
    // The three are puzzled, and Jenna asks what he means.
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    // Kraden: they spoke of Mt. Aleph and Sol Sanctum as if they had seen
    // them.
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    // Kraden looks from one to another and asks whether that is not strange.
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId(MsgValeYouCannotEnterMtAleph);
    } else {
      Event_SetMessageId(MsgValeKradenIWaitedYearsFor);
    }
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // Jenna fears they are thieves; Garet would tell the high elders.
    Event_SetMessageId(MsgValeJennaTheyMightBeThieves);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 6);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    Actor_Jump(ACTOR_GARET, 4, 30);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 6);
    // The three agree and turn to go.
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
    Actor_FaceActor(ACTOR_KRADEN, ACTOR_GARET, 10);
    Actor_FaceActor(ACTOR_KRADEN, ACTOR_JENNA, 10);
    ValeKradensCottage_NodTogether();
    Event_WaitFrames(10);
    Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 16);
    // Kraden stops them: nothing can be done until what he was told is
    // confirmed.
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 6);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 6);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 6);
    // Jenna asks what to do; Kraden means to go and confirm it.
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(6);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // The three start; Garet asks whether he means Sol Sanctum itself, and
    // Jenna thinks it impossible.
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_Jump(ACTOR_GARET, 2, 0);
    Actor_Jump(ACTOR_JENNA, 2, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(16);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 40);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 30);
    // Kraden asks whether the ban on climbing Mt. Aleph should stop them;
    // either way, they will only look.
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId(MsgValeKradenWeOnlyCheckThe);
    } else {
      Event_SetMessageId(MsgValeKradenWeOnlyCheckMt);
    }
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
    // Garet asks Isaac whether it is fine if they are not seen.
    sneakingMessage = MsgValeFineIfTheyDontSee;
    Event_SetMessageId(sneakingMessage);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId((sneakingMessage + 1));
    } else {
      Event_SetMessageId((sneakingMessage + 2));
    }
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 6);
    // Jenna makes it their secret; a yes earns the warning not even to tell
    // Isaac's mother, a no only nods and skips it.
    Event_SetMessageId(MsgValeJennaOurSecret);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
    Event_OpenMessage(ACTOR_JENNA, 0);
    Event_WaitFrames(4);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Actor_Jump(ACTOR_JENNA, 2, 20);
      FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    } else {
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
      Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
      Event_WaitFrames(8);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
      gFieldWork->message++;
    }
    // Kraden: then it is decided.
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 10);
    ValeKradensCottage_NodTogether();
    Event_WaitFrames(10);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Event_WaitFrames(20);
    // Kraden makes Isaac the leader; Garet and Jenna squabble over who would
    // be nervous.
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    Event_WaitFrames(4);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId(MsgValeGaretYouCanHandleThe);
    } else {
      Event_SetMessageId(MsgValeGaretIllTakeOverIf);
    }
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 10);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 6);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 6);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    // Kraden hops, eager to start.
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_Jump(ACTOR_KRADEN, 4, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
    // Kraden opens his door and goes in to prepare; the screen closes and the
    // party follows him through
    // VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG.
    Event_PlaySong(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gValeKradensCottage_DoorOpens, 43, 8);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KRADEN, 232, 218);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(
        VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG);
    Event_End();
  }
}

/// In Vale, back out of Kraden's cottage by
/// VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH: Kraden comes out ready, and
/// everyone joins Isaac for Mt. Aleph. "This is our best bet."
///
/// Called by ValeKradensCottage_Initialize on
/// VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH unless the party keeps its
/// position. Sets FLAG_SANCTUM_VISIT_PLANNED and FLAG_VALE_KRADENS_DOOR_OPEN
/// (the door stays open), and removes Kraden, Garet, Jenna, Saturos and
/// Menardi.
static void ValeKradensCottage_KradenSetsOff(void) {
  struct Actor *actor;
  s32 facing;

  Event_Start();
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(216), PIXELS(264));
  Actor_SetPosition(ACTOR_JENNA, PIXELS(248), PIXELS(264));
  actor = Actor_GetById(ACTOR_GARET);
  facing = FACING_NORTH;
  actor->facing = facing;
  actor = Actor_GetById(ACTOR_JENNA);
  actor->facing = facing;
  MapGrid_AnimateCells(gValeKradensCottage_DoorOpens, 43, 8);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  // Kraden comes out of the door: this is their best bet.
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(230), PIXELS(220));
  Actor_WalkToAndWait(ACTOR_KRADEN, 230, 232);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_SetMessageId(MsgValeKradenOurBestBet);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  // Isaac nods to Garet, then to Jenna.
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 10);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  // Garet, Jenna and Kraden walk into Isaac and vanish; the strangers are
  // gone too.
  ValeKradensCottage_WalkTowardLeader(ACTOR_GARET);
  ValeKradensCottage_WalkTowardLeader(ACTOR_JENNA);
  ValeKradensCottage_WalkTowardLeader(ACTOR_KRADEN);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_SetPosition(ACTOR_SATUROS, 0, 0);
  Actor_SetPosition(ACTOR_MENARDI, 0, 0);
  GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  GameFlag_Set(FLAG_VALE_KRADENS_DOOR_OPEN);
  Event_End();
}

/// In Vale, walking into Kraden's cottage: the door opens unless Kraden left
/// it open.
///
/// Reached by walking north into cell behavior 3, by day and after Kalay;
/// takes VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH into the cottage.
static void ValeKradensCottage_EnterCottage(void) {
  if (GameFlag_Get(FLAG_VALE_KRADENS_DOOR_OPEN) == 0) {
    Sound_PlaySong(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gValeKradensCottage_DoorOpens, 43, 8);
  }
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 229, 217);
  Import_EventSetExitRequest(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH);
}

/// In Vale on the way to Kraden's cottage, walking past the two strangers:
/// Isaac, Garet and Jenna overhear Saturos and Menardi, confront them and are
/// warned off.
///
/// Reached by stepping on cell behavior 7 in the day's events; runs once,
/// setting FLAG_MET_SATUROS_AND_MENARDI. Saturos and Menardi are left turning
/// to watch the leader.
static void ValeKradensCottage_MeetSaturosAndMenardi(void) {
  struct Actor *actor;
  s32 x, z;
  s32 strangersMessage;
  s32 accusationMessage;

  if (Event_GetFlag(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
    return;
  }

  Event_Start();
  Event_PlaySong(SONG_FADE_OUT_BGM);
  GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

  // Saturos is overheard: the villagers know little of Sol Sanctum.
  strangersMessage = MsgValeTheyKnowLittleOfThe;
  Event_SetMessageId(strangersMessage);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);

  // Isaac hurries forward and Garet and Jenna step out beside him.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 392, 328);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Actor_SetPosition(ACTOR_JENNA, x * Q16_ONE, z * Q16_ONE);
  Actor_SetPosition(ACTOR_GARET, x * Q16_ONE, z * Q16_ONE);

  Actor_SetSpeed(ACTOR_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(ACTOR_JENNA, 376, 328);
  Actor_WalkToAndWait(ACTOR_GARET, 408, 328);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STILL);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  ValeKradensCottage_FacePartySouth();
  // The camera pans down to the strangers, and their theme plays.
  FieldCamera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
  FieldCamera_MoveTo(PIXELS(215), -1, PIXELS(345), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Event_PlaySong(MUS_SATUROS_AND_MENARDI);

  // Saturos and Menardi talk of the elders and of using Kraden, stubborn
  // and cunning as he is.
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH, 60);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 6);
  // They sense they are watched; Saturos looks about, and both walk round to
  // face the party.
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
  Actor_WalkToAndWait(ACTOR_SATUROS, 232, 360);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_WalkTo(ACTOR_SATUROS, 392, 360);
  Actor_WalkToAndWait(ACTOR_MENARDI, 216, 360);
  Actor_WalkTo(ACTOR_MENARDI, 376, 360);
  ValeKradensCottage_FacePartySouth();
  FieldCamera_MoveTo(PIXELS(393), -1, PIXELS(339), 1);
  Actor_WalkTo(ACTOR_SATUROS, 392, 360);
  Actor_WalkToAndWait(ACTOR_MENARDI, 376, 360);
  Import_ActorSetAnimation(ACTOR_SATUROS, 0);
  Import_ActorSetAnimation(ACTOR_MENARDI, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 30);
  // Saturos demands what they are doing; Garet throws it back; Menardi asks
  // whether they were eavesdropping.
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId(strangersMessage + 10);
  } else {
    Event_SetMessageId(strangersMessage + 11);
  }

  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);

  // Garet accuses them of sneaking around; Menardi guesses Kraden's
  // engagement was with the children, and Jenna says they are off to meet
  // him.
  accusationMessage = MsgValeYoureTheOnesSneakingAround;
  Event_SetMessageId(accusationMessage);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_TurnToFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 60);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  // Saturos asks whether their appointment is important enough to drive the
  // strangers away.
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST, 20);
  Event_OpenMessage(ACTOR_SATUROS, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId(accusationMessage + 5);
  } else {
    Event_SetMessageId(accusationMessage + 6);
  }

  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_GARET, 30);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_JENNA, 30);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  // Saturos lets them go; Jenna is surprised and Menardi doubts him, but he
  // sees no reason to delay the children.
  Event_SetMessageId(MsgValeSaturosGo);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 6);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 1);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);

  // The pair back away south without turning round.
  actor = Actor_GetById(ACTOR_SATUROS);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_GetById(ACTOR_MENARDI);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;

  Actor_WalkTo(ACTOR_SATUROS, 392, 376);
  Actor_WalkToAndWait(ACTOR_MENARDI, 376, 376);
  Event_WaitFrames(6);

  actor = Actor_GetById(ACTOR_SATUROS);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_GetById(ACTOR_MENARDI);
  actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;

  Import_ActorSetAnimation(ACTOR_SATUROS, 0);
  Import_ActorSetAnimation(ACTOR_MENARDI, 0);
  Event_WaitFrames(20);
  // Garet: let's go. The village music returns and Garet and Jenna walk back
  // into Isaac; the strangers keep turning to watch him.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_PlaySong(SONG_FADE_OUT_BGM);

  ValeKradensCottage_WalkTowardLeader(ACTOR_GARET);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);

  ValeKradensCottage_WalkTowardLeader(ACTOR_JENNA);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  FieldActor_SetTargetAndCallback(ACTOR_SATUROS,
                                  ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gValeKradensCottage_FaceTargetActions);
  FieldActor_SetTargetAndCallback(ACTOR_MENARDI,
                                  ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gValeKradensCottage_FaceTargetActions);
  Scene_PlayMusic();
  Event_End();
}

/// In Vale, walking up to Kraden's cottage: Kraden paces, wondering aloud who
/// the strangers were and what they know of Sol Sanctum and the Elemental
/// Stars.
///
/// Reached by stepping on cell behavior 5 in the day's events; runs once,
/// setting FLAG_KRADEN_WONDERED_ABOUT_STRANGERS.
static void ValeKradensCottage_OverhearKraden(void) {
  struct Actor *leader;

  if (Event_GetFlag(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS) != 0) {
  } else {
    Event_Start();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    // Kraden wonders aloud who they were, while Garet and Jenna step out of
    // Isaac behind him.
    Event_SetMessageId(MsgValeWho);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    {
      s32 x = leader->x.part.pixel;
      s32 z = leader->z.part.pixel;

      Import_ActorSetPosition(ACTOR_JENNA, x * Q16_ONE, z * Q16_ONE);
      Import_ActorSetPosition(ACTOR_GARET, x * Q16_ONE, z * Q16_ONE);
    }
    Actor_SetSpeed(ACTOR_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(ACTOR_JENNA, 296, 328);
    Actor_WalkToAndWait(ACTOR_GARET, 280, 328);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STILL);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
    // The camera goes up to the door. Pacing, Kraden says they know more of
    // Sol Sanctum than Vale's elders.
    FieldCamera_MoveWithinMapBounds(PIXELS(232), -1, PIXELS(240), 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_SetPositionAndReset(ACTOR_KRADEN, 216, 232);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
    Event_WaitFrames(10);
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
    Actor_Jump(ACTOR_KRADEN, 2, 10);
    Actor_ArmCallback(ACTOR_KRADEN, FACING_EAST, 10);
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // He turns over what they said of the elements and the Elemental Stars,
    // things even he does not know.
    Actor_SetPositionAndReset(ACTOR_KRADEN, 248, 232);
    Event_WaitFrames(40);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 2);
    Actor_SetPositionAndReset(ACTOR_KRADEN, 232, 232);
    Event_WaitFrames(2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 4);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_WaitFrames(4);
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 2);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    // The camera returns to Isaac, and Garet and Jenna walk back into him.
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
    FieldCamera_WaitForArrival();
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Import_ActorSetPosition(ACTOR_GARET, 0, 0);
    Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_JENNA, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_JENNA);
    Import_ActorSetPosition(ACTOR_JENNA, 0, 0);
    GameFlag_Set(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
    Event_End();
  }
}

/// Arriving in Kraden's part of Vale: places the actors for the storm or
/// for the day, and plays the arrival from the cottage.
///
/// The scene's initialize entry. On the stormy night (FLAG_VALE_STORM) it
/// starts the lightning, removes the day's people, lays the survivor by the
/// fence (unless FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) and blocks the cells the
/// rockslide covers. By day it removes the rocks and Garet, runs
/// ValeKradensCottage_KradenSetsOff from
/// VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH, removes Kraden and the
/// strangers once FLAG_SANCTUM_VISIT_PLANNED is set, and places the cottage
/// visitor after Kalay. Clears FLAG_VALE_KRADENS_DOOR_OPEN when the party keeps
/// its position.
s32 ValeKradensCottage_Initialize(void) {
  struct Actor *actor;
  s32 debrisRow;

  if (gAdventure.entrance == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_UNUSED ||
      gAdventure.entrance == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_VALE_KRADENS_DOOR_OPEN);
  }
  // The rockslide chapter replaces residents with debris and a survivor.
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Weather_StartRain();
    Weather_StartLightning();
    Event_Start();
    Import_ActorSetPosition(ACTOR_BOY_BY_THE_FENCE, 0, 0);
    Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Import_ActorSetPosition(ACTOR_SATUROS, 0, 0);
    Import_ActorSetPosition(ACTOR_MENARDI, 0, 0);
    Import_ActorSetPosition(ACTOR_JENNA, 0, 0);
    {
      struct Actor *actor = Actor_GetById(ACTOR_FENCE_BOULDER);

      actor->collisionFlags = 8 | actor->collisionFlags;
    }
    Actor_SetPosition(ACTOR_ROCKSLIDE_SURVIVOR, PIXELS(83), PIXELS(265));
    Actor_SetPositionAndReset(ACTOR_ROCKSLIDE_SURVIVOR, 83, 273);
    Import_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, 5);
    actor = Actor_GetById(ACTOR_ROCKSLIDE_SURVIVOR);
    {
      s32 collisionRadius = 12;

      actor->radius = collisionRadius;
    }
    Actor_SetScript(ACTOR_ROCKSLIDE_SURVIVOR,
                    gValeKradensCottage_RockslideSurvivorSquashes);
    if (GameFlag_Get(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) != 0) {
      Import_ActorSetPosition(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
    }
    debrisRow = 21;
    Event_End();
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 14, debrisRow);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 15, debrisRow);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 23, 19);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 24, 19);
    debrisRow = 20;
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 23, 20);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 24, 20);
  } else {
    Import_ActorSetPosition(ACTOR_WEST_ROCK, 0, 0);
    Import_ActorSetPosition(ACTOR_EAST_ROCK, 0, 0);
    Import_ActorSetPosition(ACTOR_GARET, 0, 0);
    Import_ActorSetPosition(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
    Event_Start();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gAdventure.entrance ==
          VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH) {
        ValeKradensCottage_KradenSetsOff();
      }
    }
    if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) != 0) {
      Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
      Import_ActorSetPosition(ACTOR_SATUROS, 0, 0);
      Import_ActorSetPosition(ACTOR_MENARDI, 0, 0);
    } else {
      if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        Actor_SetPosition(ACTOR_SATUROS, PIXELS(392), PIXELS(376));
        Actor_SetPosition(ACTOR_MENARDI, PIXELS(376), PIXELS(376));
        FieldActor_SetTargetAndCallback(
            ACTOR_SATUROS, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeKradensCottage_FaceTargetActions);
        FieldActor_SetTargetAndCallback(
            ACTOR_MENARDI, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeKradensCottage_FaceTargetActions);
      }
    }
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      Actor_SetPosition(ACTOR_COTTAGE_VISITOR, PIXELS(132), PIXELS(264));
    }
    Event_End();
  }
  return 0;
}

/// In Vale on the stormy night, the man hurt in the rockslide asks whether he
/// will die: yes, and he gives up; no, and he finds he is unhurt, warns them
/// about the monsters and runs off.
///
/// Reached on the stormy night by talking to him (ACTOR_ROCKSLIDE_SURVIVOR),
/// or by stepping on cell behavior 20 until he has given up. Sets
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP on yes, or
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED on no; once he has escaped it does
/// nothing.
static void ValeKradensCottage_TalkToRockslideSurvivor(void) {
  struct Actor *leader;
  s32 message;

  if (Event_GetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) == 0) {
    if (Event_GetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP) != 0) {
      Event_Start();
      Actor_RunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
      Event_SetMessageId(MsgValeSaveYourselves);
      Event_ShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0);
      Event_End();
    } else {
      Event_Start();
      Actor_Stop(ACTOR_ROCKSLIDE_SURVIVOR);
      Actor_RunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 1);
      // He gasps out the rockslide and the monsters; Isaac and Garet hurry
      // over to him.
      message = MsgValeRockslideDestroyedFence;
      Event_SetMessageId(message);
      Event_ShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      30);
      FieldCamera_MoveWithinMapBounds(PIXELS(98), -1, PIXELS(283), 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 94, 293);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Import_ActorSetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
      }
      Actor_WalkToAndWait(ACTOR_GARET, 110, 279);
      Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
      Actor_RunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
      Event_WaitFrames(40);
      // He asks whether he will die. Yes: he gives up and tells them to save
      // themselves.
      FieldMessage_ShowAtActor(ACTOR_ROCKSLIDE_SURVIVOR, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        Actor_RunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
        Event_WaitFrames(20);
        Event_SetMessageId((message + 2));
        Event_ShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0);
        GameFlag_Set(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP);
      } else {
        // No: he laments, then jumps up, finds he is not hurt at all, and
        // warns them of the monsters on the way to the plaza.
        Actor_RunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
        Event_WaitFrames(20);
        Event_SetMessageId((message + 3));
        Event_ShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 40);
        Actor_SetAngleToward(ACTOR_ROCKSLIDE_SURVIVOR, ACTOR_PARTY_LEADER, 0);
        Import_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_STAND);
        Actor_Jump(ACTOR_ROCKSLIDE_SURVIVOR, 4, 40);
        Import_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, 6);
        Actor_ShowEmote(ACTOR_ROCKSLIDE_SURVIVOR,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
        Event_ShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
        Import_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_STAND);
        Event_WaitFrames(10);
        Actor_SetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_NOD);
        Event_ShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
        Actor_SetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_NOD);
        // Follow his escape until the party can see that he is unharmed.
        FieldActor_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                        ACTOR_WATCH_KEEP_MOTION |
                                            ACTOR_ROCKSLIDE_SURVIVOR,
                                        gValeKradensCottage_FaceTargetActions);
        FieldActor_SetTargetAndCallback(
            ACTOR_GARET, ACTOR_WATCH_KEEP_MOTION | ACTOR_ROCKSLIDE_SURVIVOR,
            gValeKradensCottage_FaceTargetActions);
        Actor_RunScriptAndWait(
            ACTOR_ROCKSLIDE_SURVIVOR,
            gValeKradensCottage_RockslideSurvivorEscapeActions);
        Actor_Stop(ACTOR_PARTY_LEADER);
        Actor_Stop(ACTOR_GARET);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                                  120);
        GameFlag_Set(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
      }
      // Garet walks back into Isaac.
      Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                                  leader->z.part.pixel);
      }
      Actor_WaitForMoveById(ACTOR_GARET);
      Import_ActorSetPosition(ACTOR_GARET, 0, 0);
      Event_End();
    }
  }
}

/// In Vale on the stormy night, a monster attacks by the broken fence.
///
/// Reached by stepping on cell behavior 10 until
/// FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts the first slot of the encounter
/// zone of the scripted battles of Vale, Vault and the World Map.
static void ValeKradensCottage_FightFirstMonster(void) {
  struct FieldWork *work;

  Event_Start();

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  GameFlag_Set(FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 0);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Event_End();
}

/// In Vale on the stormy night, a second monster attacks by the broken fence.
///
/// Reached by stepping on cell behavior 11 until
/// FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts its second slot.
static void ValeKradensCottage_FightSecondMonster(void) {
  struct FieldWork *work;

  Event_Start();

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  GameFlag_Set(FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 1);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Event_End();
}

/// A villager at Kraden's cottage asks Isaac about his memories of it. This is
/// in Vale after the party has reached Kalay.
///
/// Reached by talking to the cottage visitor (ACTOR_COTTAGE_VISITOR) in the
/// events after Kalay.
static void ValeKradensCottage_TalkToCottageVisitor(void) {
  Event_Start();
  Event_SetMessageId(MsgValeMemoriesOfThisCottage);
  Event_AskYesNoAndShowResponse(ACTOR_COTTAGE_VISITOR, 0);
  Event_End();
}

/// Leads the party away from the broken fence, then leaves the scene.
s32 gValeKradensCottage_RockslideSurvivorEscapeActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(72), 0, PIXELS(294)),
    SC_SYNC,         SC_MOVE(PIXELS(77), 0, PIXELS(329)),
    SC_SYNC,         SC_MOVE(PIXELS(70), 0, PIXELS(415)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_END,
};

/// The survivor squashes flat and recovers every 68 frames as he lies by the
/// fence.
s32 gValeKradensCottage_RockslideSurvivorSquashes[] = {
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 32),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 64),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 64),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 128),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Keeps a watched actor facing its current target.
s32 gValeKradensCottage_FaceTargetActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gValeKradensCottage_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_NORTH, CONDITION_ALWAYS, 454, 0, 290,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH, CONDITION_ALWAYS, 454, 0, 443,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_NORTH, CONDITION_ALWAYS, 229, 0,
     233, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_WEST, CONDITION_ALWAYS, 72, 0,
     80, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST, CONDITION_ALWAYS, 104, 0,
     104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, 232, 0,
     264, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValeKradensCottage_Exits[] = {
    SCENE_EXITS(ValeKradensCottage),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_NORTH, Vale,
               VALE_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_SOUTH, Vale,
               VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH, CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST, ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST, ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG,
               CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gValeKradensCottage_ActorPlacements[] = {
    { 0x127, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(256), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(240), 0, PIXELS(353), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(384), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gValeKradensCottage_RockslideSurvivorSquashes,
        PIXELS(83), 0, PIXELS(273), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(268), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 3, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gValeKradensCottage_Triggers[] = {
    { TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, EVENT(ValeKradensCottage_KradenPlansSanctumVisit) },
    { TRIGGER_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, EVENT(ValeKradensCottage_TalkToBoyByTheFence) },
    { TRIGGER_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgValeDoNotKeepKradenWaiting) },
    { TRIGGER_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, MSGNO(MsgValeLeaveBeforeWeChangeMinds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeCuriousBeyondFence) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(ValeKradensCottage_EnterCottage) },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(ValeKradensCottage_OverhearKraden) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(ValeKradensCottage_MeetSaturosAndMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { TRIGGERS_END },
};

struct Trigger gValeKradensCottage_StormTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH },
    { TRIGGER_TALK, ACTOR_WEST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { TRIGGER_TALK, ACTOR_EAST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { TRIGGER_TALK, ACTOR_ROCKSLIDE_SURVIVOR, CONDITION_ALWAYS, EVENT(ValeKradensCottage_TalkToRockslideSurvivor) },
    { TRIGGER_STEP, 20, FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP, EVENT(ValeKradensCottage_TalkToRockslideSurvivor) },
    { TRIGGER_STEP, 10, FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT, EVENT(ValeKradensCottage_FightFirstMonster) },
    { TRIGGER_STEP, 11, FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT, EVENT(ValeKradensCottage_FightSecondMonster) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, CONDITION_ALWAYS, 0 },
    { TRIGGERS_END },
};

struct Trigger gValeKradensCottage_TriggersAfterKalay[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(ValeKradensCottage_EnterCottage) },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { TRIGGER_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeWonderBeyondFence) },
    { TRIGGER_TALK, ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, EVENT(ValeKradensCottage_TalkToCottageVisitor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeAdventuresBeyondFence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, MSGNO(MsgValeLonelyWithoutKraden) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
#if TBS_PATCH_3
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
#endif
    { TRIGGERS_END },
};
// clang-format on

/// The cottage's arched front door opening: two pictures from the layout's
/// cells below the village, drawn over the closed door on the middle layer.
u16 gValeKradensCottage_DoorOpens[] = {
    MAPGRID_ANIMATION_STEP(2, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEP(4, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEPS_END,
};
