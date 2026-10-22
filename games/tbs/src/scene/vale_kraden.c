/// @file
/// Vale (SceneId_ValeKraden): the part of the village around Kraden's cottage,
/// by the fence that closes off the lands beyond. Kraden, "a trained Alchemy
/// sage" in his own words, came to Vale to study Mt. Aleph and Alchemy. One
/// placement table serves every visit: ValeKraden_Initialize removes the people
/// a visit does not use, and ValeKraden_GetEvents picks the events for the time
/// of the story.
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). The storm's map (Vale,
///   SceneId_ValeStorm) leads here by VALE_STORM_TO_VALE_KRADEN_NORTH and
///   VALE_STORM_TO_VALE_KRADEN_SOUTH, arriving by VALE_KRADEN_FROM_VALE_NORTH
///   and VALE_KRADEN_FROM_VALE_SOUTH, and the two paths lead back
///   (VALE_KRADEN_TO_VALE_STORM_NORTH and VALE_KRADEN_TO_VALE_STORM_SOUTH). A
///   rockslide has broken the fence and let monsters in: Initialize starts the
///   lightning, leaves the two rocks that Isaac cannot budge across the way,
///   copies MAP_CELL_WALL over the path's cells below the west rock and under
///   the east one, two of which by day hold Event_MeetSaturosAndMenardi's cell
///   code 7
///   (https://alchemy.pascalpixel.com/places/vale-kraden/rockslide-cells.png:
///   the room on the stormy night and by day, the rocks boxed and the six
///   cells outlined with their collision codes), and lays a hurt man by the
///   fence. Talking to
///   him, or walking up to him (cell code 20), plays
///   Event_TalkToRockslideSurvivor: asked whether he will die, a yes leaves him
///   giving up (FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP), and a no has him find he
///   is not hurt at all, warn them of the monsters on their way to the plaza
///   and run off (FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED), after which he is no
///   longer placed. Two cells start a monster battle each, once (the first two
///   slots of the encounter zone of the scripted battles of Vale, Vault and the
///   world map).
/// - Three years later, the day the story begins, Isaac comes up with Garet and
///   Jenna to find two strangers outside. Walking past them (cell code 7) plays
///   Event_MeetSaturosAndMenardi: the three overhear the two speak of Sol
///   Sanctum and of using Kraden, confront them and are let go, the two calling
///   each other Saturos and Menardi (FLAG_MET_SATUROS_AND_MENARDI), and the
///   strangers then stand aside, turning to watch the leader. Near the cottage
///   (cell code 5), the party overhears Kraden wondering what the strangers
///   know of the Elemental Stars (Event_OverhearKraden,
///   FLAG_KRADEN_WONDERED_ABOUT_STRANGERS). Talking to Kraden plays
///   Event_KradenPlansSanctumVisit: Garet brings up Saturos and Menardi, and
///   Kraden talks the three into a secret look at Sol Sanctum on the forbidden
///   mountain, then goes in to prepare, and
///   VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG takes the party into the
///   cottage (SceneId_ValeInteriorsInn,
///   VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG). Coming back out by
///   VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH, Kraden joins them for Mt. Aleph
///   (Event_KradenSetsOff, FLAG_SANCTUM_VISIT_PLANNED); from then on he and the
///   strangers are gone. A small boy at the fence hears growls beyond it, and a
///   jar holds an Elixir.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT) the child means to
///   climb the fence one day, and after Kalay (FLAG_KALAY_REACHED) a woman with
///   fair hair visits the empty cottage, missing Kraden.
///
/// By day the cottage's gate (Event_EnterCottage) leads into the cottage
/// (VALE_KRADEN_TO_VALE_INTERIORS_INN_NORTH, to SceneId_ValeInteriorsInn's
/// VALE_INTERIORS_INN_FROM_VALE_KRADEN_NORTH), VALE_KRADEN_TO_VALE_NORTH and
/// VALE_KRADEN_TO_VALE_SOUTH go down into the village (SceneId_Vale) and
/// VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST and VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST
/// into Vale Cave's way in (SceneId_ValeCaveMouth). Coming back from it by
/// VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST clears FLAG_SHOW_LOCATION_NAME, so the
/// place name is not shown again. The jar's row finds nothing on the stormy
/// night, and after Kalay it is there only from the third patch on.
///
/// Open questions: none.

#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/vale_kraden.h"
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
#include "call.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// FLAG_SCENE_TEMP_42: the cottage's gate stands open since Kraden went in.

/// The placed actors, numbered in the order of gValeKraden_Placements; Garet
/// and Jenna, placed between Kraden and Saturos, keep their own slots.
enum ValeKradenActor {
  /// A boulder by the fence, with the sprite of the boulder Lift raises at the
  /// Alpine Crossing (MsgMountainIsaacDidLiftBoulder) and in Gondowan Cave. Its
  /// Lift row runs no script, like ACTOR_RIVER_BOULDER's in gondowan_cave.c,
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

#include "common/object_facing.h"

const struct SceneEntrance *ValeKraden_GetEntrances(void) {
  return gValeKraden_Entrances;
}

const struct SceneRegion *ValeKraden_GetRegions(void) { return NULL; }

const u32 *ValeKraden_GetExits(void) { return gValeKraden_Exits; }

/// Returns the one placement table for every visit; ValeKraden_Initialize
/// removes the actors a visit does not use.
const struct ScenePlacement *ValeKraden_GetPlacements(void) {
  return gValeKraden_Placements;
}

/// Selects the events for the current visit to Vale: after Kalay
/// (FLAG_KALAY_REACHED), on the stormy night (FLAG_VALE_STORM), else the day
/// the story begins.
const struct SceneEvent *ValeKraden_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
    return gValeKraden_EventsAfterKalay;
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    return gValeKraden_StormEvents;
  }
  return gValeKraden_Events;
}

/// Finishes the party's nod together before the next line of dialogue.
static __inline__ void ValeKraden_NodTogether(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIMATION_NOD);
}

/// Turns all three party members toward the strangers.
static __inline__ void ValeKraden_FacePartySouth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 0);
}

/// Starts a companion walking back to Isaac's current pixel position.
static __inline__ void ValeKraden_WalkTowardLeader(s32 companionId) {
  struct FieldActor *leader;

  Engine_ActorSetAnimation(companionId, ANIMATION_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
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
static void Event_TalkToBoyByTheFence(void) {
  s32 message;

  Engine_EventStart();
  if (GameFlag_IsSet(FLAG_VALE_PARTY_LEFT) != 0) {
    Engine_EventSetMessage(MsgValeIllClimbTheFenceSomeday);
    Event_ShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
  } else {
    message = MsgValeHearAwfulGrowls;
    Engine_EventSetMessage(message);
    Actor_FaceActor(ACTOR_BOY_BY_THE_FENCE, ACTOR_PARTY_LEADER, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 2);
    Engine_EventWait(6);
    Event_OpenMessage(ACTOR_BOY_BY_THE_FENCE, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(message + 1);
    } else {
      Engine_EventSetMessage(message + 2);
    }
    Engine_ActorStartRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 3);
    Event_ShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
    Actor_FaceDirection(ACTOR_BOY_BY_THE_FENCE, FACING_NORTH, 10);
  }
  Engine_EventEnd();
}

/// In Vale, talking to Kraden outside his cottage: he talks Isaac, Garet and
/// Jenna into a secret look at Sol Sanctum, then goes inside to prepare.
///
/// Reached by talking to Kraden in the day's events, until
/// FLAG_SANCTUM_VISIT_PLANNED is set. Ends by taking
/// VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG into the cottage
/// (SceneId_ValeInteriorsInn, VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG);
/// the party comes back by VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH, where
/// Event_KradenSetsOff goes on.
static void Event_KradenPlansSanctumVisit(void) {
  struct FieldActor *leader;
  s32 x, z;
  s32 sneakingMessage;

  if (GameFlag_IsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    // Isaac walks up to Kraden, who greets him.
    Engine_EventStart();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Engine_EventSetMessage(MsgValeKradenOhIsaac);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 264);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_KRADEN, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    // Garet and Jenna step out of Isaac and stand either side of him.
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    Actor_SetPosition(JENNA, x * Q16_ONE, z * Q16_ONE);
    Actor_SetPosition(GARET, x * Q16_ONE, z * Q16_ONE);
    Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkTo(JENNA, 248, 264);
    Actor_WalkToAndWait(GARET, 216, 264);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
    Engine_ActorSetAnimation(JENNA, ANIMATION_STAND);
    Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
    Engine_EventWait(4);
    // Jenna sees something is bothering Kraden; he hedges.
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
    Engine_ActorSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(JENNA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    // Garet names Saturos and Menardi; Kraden finds them persistent.
    Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 40);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Event_ShowMessageAndWait(GARET, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorSetAnimation(ACTOR_KRADEN, ANIMATION_NOD);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 8);
    // Jenna asks whether they wanted something from him; it seems so.
    Actor_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 20);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 6);
    // The three are puzzled, and Jenna asks what he means.
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(JENNA, 2);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    // Kraden: they spoke of Mt. Aleph and Sol Sanctum as if they had seen
    // them.
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_SHAKE_HEAD);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(GARET, FACING_EAST, 0);
    Actor_FaceDirection(JENNA, FACING_WEST, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    // Kraden looks from one to another and asks whether that is not strange.
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(MsgValeYouCannotEnterMtAleph);
    } else {
      Engine_EventSetMessage(MsgValeKradenIWaitedYearsFor);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    // Jenna fears they are thieves; Garet would tell the high elders.
    Engine_EventSetMessage(MsgValeJennaTheyMightBeThieves);
    Engine_ActorRunRepeatedMotion(JENNA, 2);
    Actor_FaceDirection(JENNA, FACING_WEST, 10);
    Event_ShowMessageAndWait(JENNA, 0, 6);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    Engine_ActorJump(GARET, 4, 30);
    Actor_FaceDirection(GARET, FACING_EAST, 10);
    Event_ShowMessageAndWait(GARET, 0, 6);
    // The three agree and turn to go.
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
    Actor_FaceActor(ACTOR_KRADEN, GARET, 10);
    Actor_FaceActor(ACTOR_KRADEN, JENNA, 10);
    ValeKraden_NodTogether();
    Engine_EventWait(10);
    Engine_ActorSetAnimation(JENNA, ANIMATION_STAND);
    Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(JENNA, FACING_SOUTH, 16);
    // Kraden stops them: nothing can be done until what he was told is
    // confirmed.
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 6);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 6);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 6);
    // Jenna asks what to do; Kraden means to go and confirm it.
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 10);
    Event_ShowMessageAndWait(JENNA, 0, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
    Engine_EventWait(6);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    // The three start; Garet asks whether he means Sol Sanctum itself, and
    // Jenna thinks it impossible.
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Engine_ActorJump(GARET, 2, 0);
    Engine_ActorJump(JENNA, 2, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
    Event_ShowMessageAndWait(GARET, 0, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
    Engine_EventWait(16);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 40);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
    // Kraden asks whether the ban on climbing Mt. Aleph should stop them;
    // either way, they will only look.
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_SHAKE_HEAD);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(MsgValeKradenWeOnlyCheckThe);
    } else {
      Engine_EventSetMessage(MsgValeKradenWeOnlyCheckMt);
    }
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
    // Garet asks Isaac whether it is fine if they are not seen.
    sneakingMessage = MsgValeFineIfTheyDontSee;
    Engine_EventSetMessage(sneakingMessage);
    Actor_FaceDirection(GARET, FACING_EAST, 10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage((sneakingMessage + 1));
    } else {
      Engine_EventSetMessage((sneakingMessage + 2));
    }
    Event_ShowMessageAndWait(GARET, 0, 6);
    // Jenna makes it their secret; a yes earns the warning not even to tell
    // Isaac's mother, a no only nods and skips it.
    Engine_EventSetMessage(MsgValeJennaOurSecret);
    Actor_FaceDirection(JENNA, FACING_WEST, 10);
    Engine_ActorRunRepeatedMotion(JENNA, 1);
    Event_OpenMessage(JENNA, 0);
    Engine_EventWait(4);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_ActorJump(JENNA, 2, 20);
      Event_ShowMessageAndWait(JENNA, 0, 10);
    } else {
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
      Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
      Engine_ActorSetAnimationAndWait(JENNA, ANIMATION_NOD);
      Engine_EventWait(8);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
      gEventWork->message++;
    }
    // Kraden: then it is decided.
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 10);
    ValeKraden_NodTogether();
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    // Kraden makes Isaac the leader; Garet and Jenna squabble over who would
    // be nervous.
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_OpenMessage(ACTOR_KRADEN, 0);
    Engine_EventWait(4);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(MsgValeGaretYouCanHandleThe);
    } else {
      Engine_EventSetMessage(MsgValeGaretIllTakeOverIf);
    }
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceDirection(GARET, FACING_EAST, 10);
    Event_ShowMessageAndWait(GARET, 0, 6);
    Actor_FaceDirection(JENNA, FACING_WEST, 10);
    Engine_ActorSetAnimationAndWait(JENNA, ANIMATION_SHAKE_HEAD);
    Event_ShowMessageAndWait(JENNA, 0, 6);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceDirection(GARET, FACING_WEST, 10);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    Event_ShowMessageAndWait(GARET, 0, 10);
    // Kraden hops, eager to start.
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Engine_ActorJump(ACTOR_KRADEN, 4, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
    // Kraden opens his gate and goes in to prepare; the screen closes and the
    // party follows him through VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG.
    Event_PlayCue(SE_DOOR_OPEN);
    Map_AnimateCells(gValeKraden_GateCells, 43, 8);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KRADEN, 232, 218);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG);
    Engine_EventEnd();
  }
}

/// In Vale, back out of Kraden's cottage by
/// VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH: Kraden comes out ready, and
/// everyone joins Isaac for Mt. Aleph. "This is our best bet."
///
/// Called by ValeKraden_Initialize on VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH
/// unless the party keeps its position. Sets FLAG_SANCTUM_VISIT_PLANNED and
/// FLAG_SCENE_TEMP_42 (the gate stays open), and removes Kraden, Garet, Jenna,
/// Saturos and Menardi.
static void Event_KradenSetsOff(void) {
  struct FieldActor *actor;
  s32 facing;

  Engine_EventStart();
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(GARET, PIXELS(216), PIXELS(264));
  Actor_SetPosition(JENNA, PIXELS(248), PIXELS(264));
  actor = Object_GetById(GARET);
  facing = FACING_NORTH;
  actor->facing = facing;
  actor = Object_GetById(JENNA);
  actor->facing = facing;
  Map_AnimateCells(gValeKraden_GateCells, 43, 8);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  // Kraden comes out of the gate: this is their best bet.
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(230), PIXELS(220));
  Actor_WalkToAndWait(ACTOR_KRADEN, 230, 232);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_NOD);
  Engine_EventSetMessage(MsgValeKradenOurBestBet);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  // Isaac nods to Garet, then to Jenna.
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 10);
  Engine_ActorSetAnimation(JENNA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  // Garet, Jenna and Kraden walk into Isaac and vanish; the strangers are
  // gone too.
  ValeKraden_WalkTowardLeader(GARET);
  ValeKraden_WalkTowardLeader(JENNA);
  ValeKraden_WalkTowardLeader(ACTOR_KRADEN);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Engine_ActorSetAnimation(JENNA, ANIMATION_STAND);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIMATION_STAND);
  Actor_SetPosition(ACTOR_SATUROS, 0, 0);
  Actor_SetPosition(ACTOR_MENARDI, 0, 0);
  GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  GameFlag_Set(FLAG_SCENE_TEMP_42);
  Engine_EventEnd();
}

/// In Vale, walking into Kraden's cottage: the gate opens unless Kraden left
/// it open.
///
/// Reached by walking north into cell code 3, by day and after Kalay; takes
/// VALE_KRADEN_TO_VALE_INTERIORS_INN_NORTH into the cottage.
static void Event_EnterCottage(void) {
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_42) == 0) {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gValeKraden_GateCells, 43, 8);
  }
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 229, 217);
  Engine_EventRequestExit(VALE_KRADEN_TO_VALE_INTERIORS_INN_NORTH);
}

/// In Vale on the way to Kraden's cottage, walking past the two strangers:
/// Isaac, Garet and Jenna overhear Saturos and Menardi, confront them and are
/// warned off.
///
/// Reached by stepping on cell code 7 in the day's events; runs once, setting
/// FLAG_MET_SATUROS_AND_MENARDI. Saturos and Menardi are left turning to watch
/// the leader.
static void Event_MeetSaturosAndMenardi(void) {
  struct FieldActor *actor;
  s32 x, z;
  s32 strangersMessage;
  s32 accusationMessage;

  if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
    return;
  }

  Engine_EventStart();
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

  // Saturos is overheard: the villagers know little of Sol Sanctum.
  strangersMessage = MsgValeTheyKnowLittleOfThe;
  Engine_EventSetMessage(strangersMessage);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);

  // Isaac hurries forward and Garet and Jenna step out beside him.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 392, 328);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Actor_SetPosition(JENNA, x * Q16_ONE, z * Q16_ONE);
  Actor_SetPosition(GARET, x * Q16_ONE, z * Q16_ONE);

  Actor_SetSpeed(JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(JENNA, 376, 328);
  Actor_WalkToAndWait(GARET, 408, 328);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  ValeKraden_FacePartySouth();
  // The camera pans down to the strangers, and their theme plays.
  Camera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
  Camera_MoveTo(PIXELS(215), -1, PIXELS(345), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Event_PlayCue(MUS_061);

  // Saturos and Menardi talk of the elders and of using Kraden, stubborn
  // and cunning as he is.
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 6);
  // They sense they are watched; Saturos looks about, and both walk round to
  // face the party.
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
  Actor_WalkToAndWait(ACTOR_SATUROS, 232, 360);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  Engine_EventWait(10);
  Actor_WalkTo(ACTOR_SATUROS, 392, 360);
  Actor_WalkToAndWait(ACTOR_MENARDI, 216, 360);
  Actor_WalkTo(ACTOR_MENARDI, 376, 360);
  ValeKraden_FacePartySouth();
  Camera_MoveTo(PIXELS(393), -1, PIXELS(339), 1);
  Actor_WalkTo(ACTOR_SATUROS, 392, 360);
  Actor_WalkToAndWait(ACTOR_MENARDI, 376, 360);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 30);
  // Saturos demands what they are doing; Garet throws it back; Menardi asks
  // whether they were eavesdropping.
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_SHAKE_HEAD);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0);

  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(strangersMessage + 10);
  } else {
    Engine_EventSetMessage(strangersMessage + 11);
  }

  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);

  // Garet accuses them of sneaking around; Menardi guesses Kraden's
  // engagement was with the children, and Jenna says they are off to meet
  // him.
  accusationMessage = MsgValeYoureTheOnesSneakingAround;
  Engine_EventSetMessage(accusationMessage);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_ActorSetAnimationAndWait(JENNA, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 20);
  // Saturos asks whether their appointment is important enough to drive the
  // strangers away.
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST, 20);
  Event_OpenMessage(ACTOR_SATUROS, 0);

  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(accusationMessage + 5);
  } else {
    Engine_EventSetMessage(accusationMessage + 6);
  }

  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceActor(ACTOR_SATUROS, GARET, 30);
  Actor_FaceActor(ACTOR_SATUROS, JENNA, 30);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  // Saturos lets them go; Jenna is surprised and Menardi doubts him, but he
  // sees no reason to delay the children.
  Engine_EventSetMessage(MsgValeSaturosGo);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 6);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(JENNA, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);

  // The pair back away south without turning round.
  actor = Object_GetById(ACTOR_SATUROS);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Object_GetById(ACTOR_MENARDI);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;

  Actor_WalkTo(ACTOR_SATUROS, 392, 376);
  Actor_WalkToAndWait(ACTOR_MENARDI, 376, 376);
  Engine_EventWait(6);

  actor = Object_GetById(ACTOR_SATUROS);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Object_GetById(ACTOR_MENARDI);
  actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;

  Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
  Engine_EventWait(20);
  // Garet: let's go. The village music returns and Garet and Jenna walk back
  // into Isaac; the strangers keep turning to watch him.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);

  ValeKraden_WalkTowardLeader(GARET);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);

  ValeKraden_WalkTowardLeader(JENNA);
  Engine_ActorWaitForMove(JENNA);
  Actor_SetPosition(JENNA, 0, 0);
  FieldObject_SetTargetAndCallback(ACTOR_SATUROS,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gValeKraden_FaceTargetActions);
  FieldObject_SetTargetAndCallback(ACTOR_MENARDI,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gValeKraden_FaceTargetActions);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

/// In Vale, walking up to Kraden's cottage: Kraden paces, wondering aloud who
/// the strangers were and what they know of Sol Sanctum and the Elemental
/// Stars.
///
/// Reached by stepping on cell code 5 in the day's events; runs once, setting
/// FLAG_KRADEN_WONDERED_ABOUT_STRANGERS.
static void Event_OverhearKraden(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS) != 0) {
  } else {
    Engine_EventStart();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    // Kraden wonders aloud who they were, while Garet and Jenna step out of
    // Isaac behind him.
    Engine_EventSetMessage(MsgValeWho);
    Engine_EventShowMessage(ACTOR_KRADEN, 0);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    {
      s32 x = leader->x.part.pixel;
      s32 z = leader->z.part.pixel;

      Engine_ActorSetPosition(JENNA, x * Q16_ONE, z * Q16_ONE);
      Engine_ActorSetPosition(GARET, x * Q16_ONE, z * Q16_ONE);
    }
    Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(JENNA, 296, 328);
    Actor_WalkToAndWait(GARET, 280, 328);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimation(JENNA, 0);
    Engine_ActorSetAnimation(GARET, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
    // The camera goes up to the gate. Pacing, Kraden says they know more of
    // Sol Sanctum than Vale's elders.
    Engine_CameraMoveTo(PIXELS(232), -1, PIXELS(240), 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventShowMessage(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
    Engine_ActorWalkToAndWait(ACTOR_KRADEN, 216, 232);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
    Engine_EventWait(10);
    Engine_EventShowMessageAndWait(ACTOR_KRADEN, 0, 40);
    Engine_ActorJump(ACTOR_KRADEN, 2, 10);
    Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
    Engine_EventShowMessageAndWait(ACTOR_KRADEN, 0, 10);
    // He turns over what they said of the elements and the Elemental Stars,
    // things even he does not know.
    Engine_ActorWalkToAndWait(ACTOR_KRADEN, 248, 232);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
    Engine_EventShowMessageAndWait(ACTOR_KRADEN, 0, 2);
    Engine_ActorWalkToAndWait(ACTOR_KRADEN, 232, 232);
    Engine_EventWait(2);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 4);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(4);
    Engine_EventShowMessageAndWait(ACTOR_KRADEN, 0, 2);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    // The camera returns to Isaac, and Garet and Jenna walk back into him.
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetAnimation(JENNA, ANIMATION_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(JENNA);
    Engine_ActorSetPosition(JENNA, 0, 0);
    Engine_GameFlagSet(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
    Engine_EventEnd();
  }
}

/// Arriving in Kraden's part of Vale: places the actors for the storm or for
/// the day, and plays the arrival from the cottage.
///
/// The scene's initialize entry. On the stormy night (FLAG_VALE_STORM) it
/// starts the lightning, removes the day's people, lays the survivor by the
/// fence (unless FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) and blocks the cells the
/// rockslide covers. By day it removes the rocks and Garet, runs
/// Event_KradenSetsOff from VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH, removes
/// Kraden and the strangers once FLAG_SANCTUM_VISIT_PLANNED is set, and places
/// the cottage visitor after Kalay. Clears FLAG_SCENE_TEMP_42 when the party
/// keeps its position.
s32 ValeKraden_Initialize(void) {
  struct FieldActor *actor;
  s32 debrisRow;

  if (gGameState.entrance == VALE_KRADEN_FROM_VALE_CAVE_MOUTH_UNUSED ||
      gGameState.entrance == VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_42);
  }
  // The rockslide chapter replaces residents with debris and a survivor.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    FieldEffect_StartGroundParticles();
    FieldEffect_StartLightning();
    Engine_EventStart();
    Engine_ActorSetPosition(ACTOR_BOY_BY_THE_FENCE, 0, 0);
    Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Engine_ActorSetPosition(ACTOR_SATUROS, 0, 0);
    Engine_ActorSetPosition(ACTOR_MENARDI, 0, 0);
    Engine_ActorSetPosition(JENNA, 0, 0);
    {
      struct FieldActor *actor = Object_GetById(ACTOR_FENCE_BOULDER);

      actor->collisionFlags = 8 | actor->collisionFlags;
    }
    Actor_SetPosition(ACTOR_ROCKSLIDE_SURVIVOR, PIXELS(83), PIXELS(265));
    Engine_ActorWalkToAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 83, 273);
    Engine_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, 5);
    actor = Object_GetById(ACTOR_ROCKSLIDE_SURVIVOR);
    {
      s32 collisionRadius = 12;

      actor->radius = collisionRadius;
    }
    Actor_SetActionScript(ACTOR_ROCKSLIDE_SURVIVOR,
                          gValeKraden_RockslideSurvivorSquashes);
    if (Engine_GameFlagIsSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) != 0) {
      Engine_ActorSetPosition(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
    }
    debrisRow = 21;
    Engine_EventEnd();
    Map_CopyCellAttributes(9, 24, 1, 1, 14, debrisRow);
    Map_CopyCellAttributes(9, 24, 1, 1, 15, debrisRow);
    Map_CopyCellAttributes(9, 24, 1, 1, 23, 19);
    Map_CopyCellAttributes(9, 24, 1, 1, 24, 19);
    debrisRow = 20;
    Map_CopyCellAttributes(9, 24, 1, 1, 23, 20);
    Map_CopyCellAttributes(9, 24, 1, 1, 24, 20);
  } else {
    Engine_ActorSetPosition(ACTOR_WEST_ROCK, 0, 0);
    Engine_ActorSetPosition(ACTOR_EAST_ROCK, 0, 0);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetPosition(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
    Engine_EventStart();
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gGameState.entrance == VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH) {
        Event_KradenSetsOff();
      }
    }
    if (Engine_GameFlagIsSet(FLAG_SANCTUM_VISIT_PLANNED) != 0) {
      Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
      Engine_ActorSetPosition(ACTOR_SATUROS, 0, 0);
      Engine_ActorSetPosition(ACTOR_MENARDI, 0, 0);
    } else {
      if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        Actor_SetPosition(ACTOR_SATUROS, PIXELS(392), PIXELS(376));
        Actor_SetPosition(ACTOR_MENARDI, PIXELS(376), PIXELS(376));
        FieldObject_SetTargetAndCallback(
            ACTOR_SATUROS, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeKraden_FaceTargetActions);
        FieldObject_SetTargetAndCallback(
            ACTOR_MENARDI, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
            gValeKraden_FaceTargetActions);
      }
    }
    if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
      Actor_SetPosition(ACTOR_COTTAGE_VISITOR, PIXELS(132), PIXELS(264));
    }
    Engine_EventEnd();
  }
  return 0;
}

/// In Vale on the stormy night, the man hurt in the rockslide asks whether he
/// will die: yes, and he gives up; no, and he finds he is unhurt, warns them
/// about the monsters and runs off.
///
/// Reached on the stormy night by talking to him (ACTOR_ROCKSLIDE_SURVIVOR),
/// or by stepping on cell code 20 until he has given up. Sets
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP on yes, or
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED on no; once he has escaped it does
/// nothing.
static void Event_TalkToRockslideSurvivor(void) {
  struct FieldActor *leader;
  s32 message;

  if (GameFlag_IsSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) == 0) {
    if (GameFlag_IsSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP) != 0) {
      Engine_EventStart();
      Engine_ActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
      Engine_EventSetMessage(MsgValeSaveYourselves);
      Engine_EventShowMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
      Engine_EventEnd();
    } else {
      Engine_EventStart();
      Engine_ActorStop(ACTOR_ROCKSLIDE_SURVIVOR);
      Engine_ActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 1);
      // He gasps out the rockslide and the monsters; Isaac and Garet hurry
      // over to him.
      message = MsgValeRockslideDestroyedFence;
      Engine_EventSetMessage(message);
      Engine_EventShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      30);
      Engine_CameraMoveTo(PIXELS(98), -1, PIXELS(283), 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 94, 293);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);
      }
      Actor_WalkToAndWait(GARET, 110, 279);
      Actor_FaceDirection(GARET, FACING_NORTHWEST, 40);
      Engine_ActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
      Engine_EventWait(40);
      // He asks whether he will die. Yes: he gives up and tells them to save
      // themselves.
      Engine_EventOpenMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((message + 2));
        Engine_EventShowMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
        Engine_GameFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP);
      } else {
        // No: he laments, then jumps up, finds he is not hurt at all, and
        // warns them of the monsters on the way to the plaza.
        Engine_ActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((message + 3));
        Engine_EventShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 40);
        Engine_ActorFaceActor(ACTOR_ROCKSLIDE_SURVIVOR, ACTOR_PARTY_LEADER, 0);
        Engine_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIMATION_STAND);
        Engine_ActorJump(ACTOR_ROCKSLIDE_SURVIVOR, 4, 40);
        Engine_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, 6);
        Actor_ShowEmote(ACTOR_ROCKSLIDE_SURVIVOR,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
        Engine_EventShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
        Engine_ActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIMATION_STAND);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR,
                                        ANIMATION_NOD);
        Engine_EventShowMessageAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR,
                                        ANIMATION_NOD);
        // Follow his escape until the party can see that he is unharmed.
        FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                         ACTOR_LINK_KEEP_MOTION |
                                             ACTOR_ROCKSLIDE_SURVIVOR,
                                         gValeKraden_FaceTargetActions);
        FieldObject_SetTargetAndCallback(
            GARET, ACTOR_LINK_KEEP_MOTION | ACTOR_ROCKSLIDE_SURVIVOR,
            gValeKraden_FaceTargetActions);
        Actor_RunActionScriptAndWait(
            ACTOR_ROCKSLIDE_SURVIVOR,
            gValeKraden_RockslideSurvivorEscapeActions);
        Engine_ActorStop(ACTOR_PARTY_LEADER);
        Engine_ActorStop(GARET);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        Actor_FaceDirection(GARET, FACING_SOUTH, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
        Engine_GameFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
      }
      // Garet walks back into Isaac.
      Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(GARET);
      Engine_ActorSetPosition(GARET, 0, 0);
      Engine_EventEnd();
    }
  }
}

/// In Vale on the stormy night, a monster attacks by the broken fence.
///
/// Reached by stepping on cell code 10 until
/// FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts the first slot of the encounter
/// zone of the scripted battles of Vale, Vault and the world map.
static void Event_FightFirstMonster(void) {
  struct EventWork *work;

  Engine_EventStart();

  work = gEventWork;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  Engine_GameFlagSet(FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 0);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

/// In Vale on the stormy night, a second monster attacks by the broken fence.
///
/// Reached by stepping on cell code 11 until
/// FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts its second slot.
static void Event_FightSecondMonster(void) {
  struct EventWork *work;

  Engine_EventStart();

  work = gEventWork;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  Engine_GameFlagSet(FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 1);
  Engine_GameFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
  Engine_EventEnd();
}

/// A villager at Kraden's cottage asks Isaac about his memories of it. This is
/// in Vale after the party has reached Kalay.
///
/// Reached by talking to the cottage visitor (ACTOR_COTTAGE_VISITOR) in the
/// events after Kalay.
static void Event_TalkToCottageVisitor(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeMemoriesOfThisCottage);
  Engine_EventAskYesNo(ACTOR_COTTAGE_VISITOR, 0);
  Engine_EventEnd();
}

/// Leads the party away from the broken fence, then leaves the map.
s32 gValeKraden_RockslideSurvivorEscapeActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(72), 0, PIXELS(294)),
    SC_SYNC,         SC_MOVE(PIXELS(77), 0, PIXELS(329)),
    SC_SYNC,         SC_MOVE(PIXELS(70), 0, PIXELS(415)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_END,
};

/// The survivor squashes flat and recovers every 68 frames as he lies by the
/// fence.
s32 gValeKraden_RockslideSurvivorSquashes[] = {
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

/// Keeps a linked actor facing its current target.
s32 gValeKraden_FaceTargetActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gValeKraden_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_NORTH, CONDITION_ALWAYS, 454, 0, 290, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_SOUTH, CONDITION_ALWAYS, 454, 0, 443, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_INTERIORS_INN_NORTH, CONDITION_ALWAYS, 229, 0, 233,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_CAVE_MOUTH_WEST, CONDITION_ALWAYS, 72, 0, 80,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_CAVE_MOUTH_EAST, CONDITION_ALWAYS, 104, 0, 104,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADEN_FROM_VALE_INTERIORS_INN_SOUTH, CONDITION_ALWAYS, 232, 0, 264,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValeKraden_Exits[] = {
    SCENE_EXITS(ValeKraden),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_NORTH, Vale, VALE_FROM_VALE_KRADEN_NORTH),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_SOUTH, Vale, VALE_FROM_VALE_KRADEN_SOUTH),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_INTERIORS_INN_NORTH, ValeInteriorsInn,
               VALE_INTERIORS_INN_FROM_VALE_KRADEN_NORTH),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST, ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADEN_WEST),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST, ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADEN_EAST),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_STORM_NORTH, ValeStorm,
               VALE_STORM_FROM_VALE_KRADEN_NORTH),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_STORM_SOUTH, ValeStorm,
               VALE_STORM_FROM_VALE_KRADEN_SOUTH),
    SCENE_EXIT(VALE_KRADEN_TO_VALE_INTERIORS_INN_MYTHRIL_BAG, ValeInteriorsInn,
               VALE_INTERIORS_INN_FROM_VALE_KRADEN_MYTHRIL_BAG),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeKraden_Placements[] = {
    { 0x127, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(353), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_ROCK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(384), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gValeKraden_RockslideSurvivorSquashes,
        PIXELS(83), 0, PIXELS(273), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(268), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 3, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeKraden_Events[] = {
    { EVENT_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_KradenPlansSanctumVisit) },
    { EVENT_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBoyByTheFence) },
    { EVENT_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgValeDoNotKeepKradenWaiting) },
    { EVENT_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, MSGNO(MsgValeLeaveBeforeWeChangeMinds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeCuriousBeyondFence) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_SOUTH },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCottage) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OverhearKraden) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetSaturosAndMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_JAR), 100, FLAG_VALE_KRADEN_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeKraden_StormEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_STORM_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_STORM_SOUTH },
    { EVENT_TALK, ACTOR_WEST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { EVENT_TALK, ACTOR_EAST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { EVENT_TALK, ACTOR_ROCKSLIDE_SURVIVOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToRockslideSurvivor) },
    { EVENT_TOUCH, 20, FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP, EVENT_SCRIPT(Event_TalkToRockslideSurvivor) },
    { EVENT_TOUCH, 10, FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT, EVENT_SCRIPT(Event_FightFirstMonster) },
    { EVENT_TOUCH, 11, FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT, EVENT_SCRIPT(Event_FightSecondMonster) },
    { SEARCH(SEARCH_JAR), 100, CONDITION_ALWAYS, 0 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeKraden_EventsAfterKalay[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_NORTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_SOUTH },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCottage) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_CAVE_MOUTH_WEST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VALE_KRADEN_TO_VALE_CAVE_MOUTH_EAST },
    { EVENT_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeWonderBeyondFence) },
    { EVENT_TALK, ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToCottageVisitor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeAdventuresBeyondFence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, MSGNO(MsgValeLonelyWithoutKraden) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
#if TBS_PATCH_3
    { SEARCH(SEARCH_JAR), 100, FLAG_VALE_KRADEN_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
#endif
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gValeKraden_GateCells[] = {
    MAP_CELL_STEP(2, 36, 2, 2, 1),
    MAP_CELL_STEP(4, 36, 2, 2, 1),
    MAP_CELL_STEPS_END,
};
