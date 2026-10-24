/// @file
/// Vale (MapId_ValeKradensCottage): the part of the village around Kraden's
/// cottage, by the fence that closes off the lands beyond. Kraden, "a trained
/// Alchemy sage" in his own words, came to Vale to study Mt. Aleph and Alchemy.
/// One object event template table serves every visit:
/// ValeKradensCottage_Initialize removes the people a visit does not use, and
/// ValeKradensCottage_GetEvents picks the events for the time of the story.
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). The storm's map (Vale,
///   MapId_ValeStormyNight) leads here by
///   VALE_STORMY_NIGHT_TO_VALE_KRADENS_COTTAGE_NORTH and
///   VALE_STORMY_NIGHT_TO_VALE_KRADENS_COTTAGE_SOUTH, arriving by
///   VALE_KRADENS_COTTAGE_FROM_VALE_NORTH and
///   VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH, and the two paths lead back
///   (VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH and
///   VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH). A rockslide has broken
///   the fence and let monsters in: Initialize starts the lightning, leaves the
///   two rocks that Isaac cannot budge across the way, copies MB_WALL over the
///   path's cells below the west rock and under the east one, two of which by
///   day hold EventScript_MeetSaturosAndMenardi's metatile behavior 7
///   (https://alchemy.pascalpixel.com/places/vale-kraden/rockslide-cells.png:
///   the room on the stormy night and by day, the rocks boxed and the six
///   cells outlined with their metatile behaviors), and lays a hurt man by the
///   fence. Talking to
///   him, or walking up to him (metatile behavior 20), plays
///   EventScript_TalkToRockslideSurvivor: asked whether he will die, a yes
///   leaves him giving up (FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP), and a no has
///   him find he is not hurt at all, warn them of the monsters on their way to
///   the plaza and run off (FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED), after which
///   he is no longer placed. Two cells start a monster battle each, once (the
///   first two slots of the encounter zone of the scripted battles of Vale,
///   Vault and the world map).
/// - Three years later, the day the story begins, Isaac comes up with Garet and
///   Jenna to find two strangers outside. Walking past them (metatile behavior
///   7) plays EventScript_MeetSaturosAndMenardi: the three overhear the two
///   speak of Sol Sanctum and of using Kraden, confront them and are let go,
///   the two calling each other Saturos and Menardi
///   (FLAG_MET_SATUROS_AND_MENARDI), and the strangers then stand aside,
///   turning to watch the leader. Near the cottage (metatile behavior 5), the
///   party overhears Kraden wondering what the strangers know of the Elemental
///   Stars (EventScript_OverhearKraden, FLAG_KRADEN_WONDERED_ABOUT_STRANGERS).
///   Talking to Kraden plays EventScript_KradenPlansSanctumVisit: Garet brings
///   up Saturos and Menardi, and Kraden talks the three into a secret look at
///   Sol Sanctum on the forbidden mountain, then goes in to prepare, and
///   VALE_KRADENS_COTTAGE_TO_VALE_INN_MYTHRIL_BAG takes the party into the
///   cottage (MapId_ValeInn,
///   VALE_INN_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG). Coming back out by
///   VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH, Kraden joins them for Mt. Aleph
///   (EventScript_KradenSetsOff, FLAG_SANCTUM_VISIT_PLANNED); from then on he
///   and the strangers are gone. A small boy at the fence hears growls beyond
///   it, and a jar holds an Elixir.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT) the child means to
///   climb the fence one day, and after Kalay (FLAG_KALAY_REACHED) a woman with
///   fair hair visits the empty cottage, missing Kraden.
///
/// By day the cottage's door (EventScript_EnterCottage) leads into the cottage
/// (VALE_KRADENS_COTTAGE_TO_VALE_INN_NORTH, to MapId_ValeInn's
/// VALE_INN_FROM_VALE_KRADENS_COTTAGE_NORTH),
/// VALE_KRADENS_COTTAGE_TO_VALE_NORTH and VALE_KRADENS_COTTAGE_TO_VALE_SOUTH go
/// down into the village (MapId_Vale) and
/// VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST and
/// VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST into Vale Cave's way in
/// (MapId_ValeCaveMouth). Coming back from it by
/// VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST clears
/// FLAG_SHOW_LOCATION_NAME, so the area name is not shown again. The jar's row
/// finds nothing on the stormy night, and after Kalay it is there only from the
/// third patch on.
///
/// Open questions: none.

#include "save_block.h"
#include "unit/import.h"
#include "unit/map/vale_kradens_cottage.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// FLAG_VALE_KRADENS_DOOR_OPEN: the cottage's door stands open since Kraden went
// in.

/// The placed object events, numbered in the order of
/// gValeKradensCottage_ObjectEventTemplates; Garet and Jenna, placed between
/// Kraden and Saturos, keep their own slots.
enum ValeKradensCottageLocalId {
  /// A boulder by the fence, with the sprite of the boulder Lift raises at the
  /// Alpine Crossing (MsgMountainIsaacDidLiftBoulder) and in Gondowan Cave. Its
  /// Lift row runs no script, like LOCALID_RIVER_BOULDER's in gondowan_cave.c,
  /// and the stormy night's events have no row for it. Cast on it, Lift's two
  /// hands close on it and burst into sparkles, the boulder stays where it is,
  /// and no message follows, since the row answers the cast (render probe
  /// kraden-lift, by day and after the party has left Vale).
  /// ★ Named from the game: "did you lift the boulder?"
  /// (MsgMountainIsaacDidLiftBoulder), said of the same sprite
  LOCALID_FENCE_BOULDER = LOCALID_FIRST_PLACED,
  /// The west rock. "It's a huge rock that Isaac's strength can't even budge."
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  LOCALID_WEST_ROCK,
  /// ★ Named from the game: "It's a huge rock" (MsgValeRockCannotBeMoved)
  LOCALID_EAST_ROCK,
  /// A young man with brown hair who lies by the broken fence on the stormy
  /// night. "Rockslide...destroyed fence... Monsters everywhere..."
  /// ★ Named from the game: "Rockslide...destroyed fence..."
  /// (MsgValeRockslideDestroyedFence)
  LOCALID_ROCKSLIDE_SURVIVOR,
  /// A small boy with fair hair, by day, younger than Isaac. "Do you hear those
  /// awful growls coming from the other side of this fence?"
  /// ★ Named from the game: "I can't stop wondering what lies beyond this
  /// fence." (MsgValeWonderBeyondFence)
  LOCALID_BOY_BY_THE_FENCE,
  /// ★ Named from the game: "from you, Kraden?" (MsgValeKradenOhIsaac)
  LOCALID_KRADEN,
  /// ★ Named from the game: "Saturos and Menardi were their names"
  /// (MsgValeKradenOhIsaac)
  LOCALID_SATUROS,
  /// ★ Named from the game: "Saturos and Menardi were their names"
  /// (MsgValeKradenOhIsaac)
  LOCALID_MENARDI,
  /// A woman with fair hair, after Kalay. "You must have so many memories of
  /// this cottage, don't you?"
  /// ★ Named from the game: "You must have so many memories of this cottage"
  /// (MsgValeMemoriesOfThisCottage)
  LOCALID_COTTAGE_VISITOR
};

#include "common/object_facing.h"

/// The door of Kraden's cottage stands open since Kraden set off.
/// ★ Named from what the player sees: the arched wooden door of Kraden's
/// thatched cottage (its render)
#define FLAG_VALE_KRADENS_DOOR_OPEN FLAG_MAP_TEMP_42

const struct MapWarpDest *ValeKradensCottage_GetWarpDests(void) {
  return gValeKradensCottage_WarpDests;
}

const struct MapWarpRegion *ValeKradensCottage_GetRegions(void) { return NULL; }

const u32 *ValeKradensCottage_GetWarps(void) {
  return gValeKradensCottage_Warps;
}

/// Returns the one object event template table for every visit;
/// ValeKradensCottage_Initialize removes the object events a visit does not
/// use.
const struct ObjectEventTemplate *
ValeKradensCottage_GetObjectEventTemplates(void) {
  return gValeKradensCottage_ObjectEventTemplates;
}

/// Selects the events for the current visit to Vale: after Kalay
/// (FLAG_KALAY_REACHED), on the stormy night (FLAG_VALE_STORM), else the day
/// the story begins.
const struct MapEvent *ValeKradensCottage_GetEvents(void) {
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gValeKradensCottage_EventsAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    return gValeKradensCottage_StormEvents;
  }
  return gValeKradensCottage_Events;
}

/// Finishes the party's nod together before the next line of dialogue.
static __inline__ void ValeKradensCottage_NodTogether(void) {
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
}

/// Turns all three party members toward the strangers.
static __inline__ void ValeKradensCottage_FacePartySouth(void) {
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTH, 0);
}

/// Starts a companion walking back to Isaac's current pixel position.
static __inline__ void ValeKradensCottage_WalkTowardLeader(s32 companionId) {
  struct ObjectEvent *leader;

  Import_ObjectEventSetAnimation(companionId, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(companionId, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
}

/// In Vale by Kraden's cottage, the child at the fence asks whether Isaac hears
/// the growls beyond it; once the party has left Vale, he means to climb it
/// someday.
///
/// Reached by talking to the boy by the fence (LOCALID_BOY_BY_THE_FENCE) in the
/// day's events.
static void EventScript_TalkToBoyByTheFence(void) {
  s32 message;

  FieldScript_Start();
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    FieldScript_SetMessageId(MsgValeIllClimbTheFenceSomeday);
    Script_ShowMessage(LOCALID_BOY_BY_THE_FENCE, 0);
  } else {
    message = MsgValeHearAwfulGrowls;
    FieldScript_SetMessageId(message);
    ObjectEvent_FaceObjectEvent(LOCALID_BOY_BY_THE_FENCE, LOCALID_PARTY_LEADER,
                                10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_BOY_BY_THE_FENCE, 2);
    FieldScript_WaitFrames(6);
    Script_OpenMessage(LOCALID_BOY_BY_THE_FENCE, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_SetMessageId(message + 1);
    } else {
      FieldScript_SetMessageId(message + 2);
    }
    ObjectMotion_SetVariantCallback(LOCALID_BOY_BY_THE_FENCE, 3);
    Script_ShowMessage(LOCALID_BOY_BY_THE_FENCE, 0);
    ObjectEvent_FaceDirection(LOCALID_BOY_BY_THE_FENCE, FACING_NORTH, 10);
  }
  FieldScript_End();
}

/// In Vale, talking to Kraden outside his cottage: he talks Isaac, Garet and
/// Jenna into a secret look at Sol Sanctum, then goes inside to prepare.
///
/// Reached by talking to Kraden in the day's events, until
/// FLAG_SANCTUM_VISIT_PLANNED is set. Ends by taking
/// VALE_KRADENS_COTTAGE_TO_VALE_INN_MYTHRIL_BAG into the cottage
/// (MapId_ValeInn, VALE_INN_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG);
/// the party comes back by VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH, where
/// EventScript_KradenSetsOff goes on.
static void EventScript_KradenPlansSanctumVisit(void) {
  struct ObjectEvent *leader;
  s32 x, z;
  s32 sneakingMessage;

  if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    // Isaac walks up to Kraden, who greets him.
    FieldScript_Start();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    FieldScript_SetMessageId(MsgValeKradenOhIsaac);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 232, 264);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_KRADEN, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    // Garet and Jenna step out of Isaac and stand either side of him.
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    ObjectEvent_SetPosition(LOCALID_JENNA, x * Q16_ONE, z * Q16_ONE);
    ObjectEvent_SetPosition(LOCALID_GARET, x * Q16_ONE, z * Q16_ONE);
    ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectEvent_WalkTo(LOCALID_JENNA, 248, 264);
    ObjectEvent_WalkToAndWait(LOCALID_GARET, 216, 264);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
    Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
    FieldScript_WaitFrames(4);
    // Jenna sees something is bothering Kraden; he hedges.
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    // Garet names Saturos and Menardi; Kraden finds them persistent.
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP,
                              40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 8);
    // Jenna asks whether they wanted something from him; it seems so.
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP,
                              20);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              10);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 6);
    // The three are puzzled, and Jenna asks what he means.
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP,
                              20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
    // Kraden: they spoke of Mt. Aleph and Sol Sanctum as if they had seen
    // them.
    ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    // Kraden looks from one to another and asks whether that is not strange.
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 20);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              40);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 40);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              20);
    Script_OpenMessage(LOCALID_KRADEN, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_SetMessageId(MsgValeYouCannotEnterMtAleph);
    } else {
      FieldScript_SetMessageId(MsgValeKradenIWaitedYearsFor);
    }
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    // Jenna fears they are thieves; Garet would tell the high elders.
    FieldScript_SetMessageId(MsgValeJennaTheyMightBeThieves);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 6);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    ObjectMotion_Launch(LOCALID_GARET, 4, 30);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 10);
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 6);
    // The three agree and turn to go.
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 10);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_JENNA, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_KRADEN, LOCALID_GARET, 10);
    ObjectEvent_FaceObjectEvent(LOCALID_KRADEN, LOCALID_JENNA, 10);
    ValeKradensCottage_NodTogether();
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTH, 16);
    // Kraden stops them: nothing can be done until what he was told is
    // confirmed.
    ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 6);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 6);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 6);
    // Jenna asks what to do; Kraden means to go and confirm it.
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP,
                              10);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 6);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              10);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
    FieldScript_WaitFrames(6);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    // The three start; Garet asks whether he means Sol Sanctum itself, and
    // Jenna thinks it impossible.
    ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
    ObjectMotion_Launch(LOCALID_GARET, 2, 0);
    ObjectMotion_Launch(LOCALID_JENNA, 2, 10);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 20);
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 6);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
    FieldScript_WaitFrames(16);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_JENNA, 40);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP,
                              10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 6);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP,
                              30);
    // Kraden asks whether the ban on climbing Mt. Aleph should stop them;
    // either way, they will only look.
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
    Script_OpenMessage(LOCALID_KRADEN, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_SetMessageId(MsgValeKradenWeOnlyCheckThe);
    } else {
      FieldScript_SetMessageId(MsgValeKradenWeOnlyCheckMt);
    }
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
    // Garet asks Isaac whether it is fine if they are not seen.
    sneakingMessage = MsgValeFineIfTheyDontSee;
    FieldScript_SetMessageId(sneakingMessage);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    Script_OpenMessage(LOCALID_GARET, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_SetMessageId((sneakingMessage + 1));
    } else {
      FieldScript_SetMessageId((sneakingMessage + 2));
    }
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 6);
    // Jenna makes it their secret; a yes earns the warning not even to tell
    // Isaac's mother, a no only nods and skips it.
    FieldScript_SetMessageId(MsgValeJennaOurSecret);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
    Script_OpenMessage(LOCALID_JENNA, 0);
    FieldScript_WaitFrames(4);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
      ObjectMotion_Launch(LOCALID_JENNA, 2, 20);
      Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
    } else {
      Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
      Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
      FieldScript_WaitFrames(8);
      Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
      gFieldWork->message++;
    }
    // Kraden: then it is decided.
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP,
                              10);
    ValeKradensCottage_NodTogether();
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    // Kraden makes Isaac the leader; Garet and Jenna squabble over who would
    // be nervous.
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    Script_OpenMessage(LOCALID_KRADEN, 0);
    FieldScript_WaitFrames(4);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_SetMessageId(MsgValeGaretYouCanHandleThe);
    } else {
      FieldScript_SetMessageId(MsgValeGaretIllTakeOverIf);
    }
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 10);
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 6);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 6);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 10);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
    Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
    // Kraden hops, eager to start.
    ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    ObjectMotion_Launch(LOCALID_KRADEN, 4, 40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP,
                              20);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP,
                              10);
    // Kraden opens his door and goes in to prepare; the screen closes and the
    // party follows him through VALE_KRADENS_COTTAGE_TO_VALE_INN_MYTHRIL_BAG.
    Script_PlaySong(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gValeKradensCottage_GateCells, 43, 8);
    ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 232, 218);
    ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
    Import_ScriptRequestWarp(VALE_KRADENS_COTTAGE_TO_VALE_INN_MYTHRIL_BAG);
    FieldScript_End();
  }
}

/// In Vale, back out of Kraden's cottage by
/// VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH: Kraden comes out ready, and
/// everyone joins Isaac for Mt. Aleph. "This is our best bet."
///
/// Called by ValeKradensCottage_Initialize on
/// VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH unless the party keeps its
/// position. Sets FLAG_SANCTUM_VISIT_PLANNED and FLAG_VALE_KRADENS_DOOR_OPEN
/// (the door stays open), and removes Kraden, Garet, Jenna, Saturos and
/// Menardi.
static void EventScript_KradenSetsOff(void) {
  struct ObjectEvent *objectEvent;
  s32 facing;

  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(216), PIXELS(264));
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(248), PIXELS(264));
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  facing = FACING_NORTH;
  objectEvent->facing = facing;
  objectEvent = ObjectEvent_GetById(LOCALID_JENNA);
  objectEvent->facing = facing;
  MapGrid_AnimateCells(gValeKradensCottage_GateCells, 43, 8);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  // Kraden comes out of the door: this is their best bet.
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(230), PIXELS(220));
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 230, 232);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_SetMessageId(MsgValeKradenOurBestBet);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  // Isaac nods to Garet, then to Jenna.
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 10);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  // Garet, Jenna and Kraden walk into Isaac and vanish; the strangers are
  // gone too.
  ValeKradensCottage_WalkTowardLeader(LOCALID_GARET);
  ValeKradensCottage_WalkTowardLeader(LOCALID_JENNA);
  ValeKradensCottage_WalkTowardLeader(LOCALID_KRADEN);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_JENNA, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  ObjectEvent_SetPosition(LOCALID_SATUROS, 0, 0);
  ObjectEvent_SetPosition(LOCALID_MENARDI, 0, 0);
  GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  GameFlag_Set(FLAG_VALE_KRADENS_DOOR_OPEN);
  FieldScript_End();
}

/// In Vale, walking into Kraden's cottage: the door opens unless Kraden left
/// it open.
///
/// Reached by walking north into metatile behavior 3, by day and after Kalay;
/// takes VALE_KRADENS_COTTAGE_TO_VALE_INN_NORTH into the cottage.
static void EventScript_EnterCottage(void) {
  if (GameFlag_Get(FLAG_VALE_KRADENS_DOOR_OPEN) == 0) {
    Sound_PlaySong(SE_DOOR_OPEN);
    MapGrid_PlayMetatileCopySequence(gValeKradensCottage_GateCells, 43, 8);
  }
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 229, 217);
  Import_ScriptRequestWarp(VALE_KRADENS_COTTAGE_TO_VALE_INN_NORTH);
}

/// In Vale on the way to Kraden's cottage, walking past the two strangers:
/// Isaac, Garet and Jenna overhear Saturos and Menardi, confront them and are
/// warned off.
///
/// Reached by stepping on metatile behavior 7 in the day's events; runs once,
/// setting FLAG_MET_SATUROS_AND_MENARDI. Saturos and Menardi are left turning
/// to watch the leader.
static void EventScript_MeetSaturosAndMenardi(void) {
  struct ObjectEvent *objectEvent;
  s32 x, z;
  s32 strangersMessage;
  s32 accusationMessage;

  if (Script_GetFlag(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
    return;
  }

  FieldScript_Start();
  Script_PlaySong(SONG_FADE_OUT_BGM);
  GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

  // Saturos is overheard: the villagers know little of Sol Sanctum.
  strangersMessage = MsgValeTheyKnowLittleOfThe;
  FieldScript_SetMessageId(strangersMessage);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);

  // Isaac hurries forward and Garet and Jenna step out beside him.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 392, 328);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  x = objectEvent->x.part.pixel;
  z = objectEvent->z.part.pixel;
  ObjectEvent_SetPosition(LOCALID_JENNA, x * Q16_ONE, z * Q16_ONE);
  ObjectEvent_SetPosition(LOCALID_GARET, x * Q16_ONE, z * Q16_ONE);

  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkTo(LOCALID_JENNA, 376, 328);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 408, 328);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  ValeKradensCottage_FacePartySouth();
  // The camera pans down to the strangers, and their theme plays.
  FieldCamera_SetSpeed(PIXELS(6), PIXELS(3) / 4);
  FieldCamera_MoveTo(PIXELS(215), -1, PIXELS(345), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  Script_PlaySong(MUS_SATUROS_AND_MENARDI);

  // Saturos and Menardi talk of the elders and of using Kraden, stubborn
  // and cunning as he is.
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 10);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 6);
  // They sense they are watched; Saturos looks about, and both walk round to
  // face the party.
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 10);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_EAST, 60);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_EAST, 40);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 232, 360);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_EAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_WalkTo(LOCALID_SATUROS, 392, 360);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 216, 360);
  ObjectEvent_WalkTo(LOCALID_MENARDI, 376, 360);
  ValeKradensCottage_FacePartySouth();
  FieldCamera_MoveTo(PIXELS(393), -1, PIXELS(339), 1);
  ObjectEvent_WalkTo(LOCALID_SATUROS, 392, 360);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 376, 360);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, 0);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 30);
  // Saturos demands what they are doing; Garet throws it back; Menardi asks
  // whether they were eavesdropping.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_MENARDI, 0);

  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_SetMessageId(strangersMessage + 10);
  } else {
    FieldScript_SetMessageId(strangersMessage + 11);
  }

  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_MENARDI, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);

  // Garet accuses them of sneaking around; Menardi guesses Kraden's
  // engagement was with the children, and Jenna says they are off to meet
  // him.
  accusationMessage = MsgValeYoureTheOnesSneakingAround;
  FieldScript_SetMessageId(accusationMessage);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
  ObjectEvent_LinkPair(LOCALID_SATUROS, LOCALID_MENARDI, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_MENARDI, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_JENNA, 0, 20);
  // Saturos asks whether their appointment is important enough to drive the
  // strangers away.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST, 20);
  Script_OpenMessage(LOCALID_SATUROS, 0);

  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_SetMessageId(accusationMessage + 5);
  } else {
    FieldScript_SetMessageId(accusationMessage + 6);
  }

  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_FaceObjectEvent(LOCALID_SATUROS, LOCALID_GARET, 30);
  ObjectEvent_FaceObjectEvent(LOCALID_SATUROS, LOCALID_JENNA, 30);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  // Saturos lets them go; Jenna is surprised and Menardi doubts him, but he
  // sees no reason to delay the children.
  FieldScript_SetMessageId(MsgValeSaturosGo);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 6);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTH, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_JENNA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_MENARDI, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 20);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);

  // The pair back away south without turning round.
  objectEvent = ObjectEvent_GetById(LOCALID_SATUROS);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;

  ObjectEvent_WalkTo(LOCALID_SATUROS, 392, 376);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 376, 376);
  FieldScript_WaitFrames(6);

  objectEvent = ObjectEvent_GetById(LOCALID_SATUROS);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->steeringFlags =
      OBJ_EVENT_FACES_MOTION | objectEvent->steeringFlags;

  Import_ObjectEventSetAnimation(LOCALID_SATUROS, 0);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, 0);
  FieldScript_WaitFrames(20);
  // Garet: let's go. The village music returns and Garet and Jenna walk back
  // into Isaac; the strangers keep turning to watch him.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, 1, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  Script_PlaySong(SONG_FADE_OUT_BGM);

  ValeKradensCottage_WalkTowardLeader(LOCALID_GARET);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);

  ValeKradensCottage_WalkTowardLeader(LOCALID_JENNA);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_JENNA);
  ObjectEvent_SetPosition(LOCALID_JENNA, 0, 0);
  Object_SetTargetAndCallback(LOCALID_SATUROS,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
                              gValeKradensCottage_FaceTargetActions);
  Object_SetTargetAndCallback(LOCALID_MENARDI,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
                              gValeKradensCottage_FaceTargetActions);
  Map_PlayMusic();
  FieldScript_End();
}

/// In Vale, walking up to Kraden's cottage: Kraden paces, wondering aloud who
/// the strangers were and what they know of Sol Sanctum and the Elemental
/// Stars.
///
/// Reached by stepping on metatile behavior 5 in the day's events; runs once,
/// setting FLAG_KRADEN_WONDERED_ABOUT_STRANGERS.
static void EventScript_OverhearKraden(void) {
  struct ObjectEvent *leader;

  if (Script_GetFlag(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS) != 0) {
  } else {
    FieldScript_Start();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    // Kraden wonders aloud who they were, while Garet and Jenna step out of
    // Isaac behind him.
    FieldScript_SetMessageId(MsgValeWho);
    FieldScript_ShowMessageAndWait(LOCALID_KRADEN, 0);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    {
      s32 x = leader->x.part.pixel;
      s32 z = leader->z.part.pixel;

      Import_ObjectEventSetPosition(LOCALID_JENNA, x * Q16_ONE, z * Q16_ONE);
      Import_ObjectEventSetPosition(LOCALID_GARET, x * Q16_ONE, z * Q16_ONE);
    }
    ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkTo(LOCALID_JENNA, 296, 328);
    ObjectEvent_WalkToAndWait(LOCALID_GARET, 280, 328);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
    Import_ObjectEventSetAnimation(LOCALID_JENNA, 0);
    Import_ObjectEventSetAnimation(LOCALID_GARET, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                              FACING_NORTHWEST + FACING_STEP, 0);
    // The camera goes up to the door. Pacing, Kraden says they know more of
    // Sol Sanctum than Vale's elders.
    FieldCamera_MoveWithinMapBounds(PIXELS(232), -1, PIXELS(240), 1);
    FieldCamera_WaitForArrival();
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    FieldScript_ShowMessageAndWait(LOCALID_KRADEN, 0);
    ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
    ObjectMotion_SetPositionAndReset(LOCALID_KRADEN, 216, 232);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
    FieldScript_WaitFrames(10);
    FieldScript_ShowMessageThenWait(LOCALID_KRADEN, 0, 40);
    ObjectMotion_Launch(LOCALID_KRADEN, 2, 10);
    ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_EAST, 10);
    FieldScript_ShowMessageThenWait(LOCALID_KRADEN, 0, 10);
    // He turns over what they said of the elements and the Elemental Stars,
    // things even he does not know.
    ObjectMotion_SetPositionAndReset(LOCALID_KRADEN, 248, 232);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_KRADEN, 0);
    ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
    FieldScript_ShowMessageThenWait(LOCALID_KRADEN, 0, 2);
    ObjectMotion_SetPositionAndReset(LOCALID_KRADEN, 232, 232);
    FieldScript_WaitFrames(2);
    ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 4);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
    FieldScript_WaitFrames(4);
    FieldScript_ShowMessageThenWait(LOCALID_KRADEN, 0, 2);
    ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    // The camera returns to Isaac, and Garet and Jenna walk back into him.
    FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
    FieldCamera_WaitForArrival();
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
    Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_WALK);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectMotion_ResetAndSetPosition(LOCALID_JENNA, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_JENNA);
    Import_ObjectEventSetPosition(LOCALID_JENNA, 0, 0);
    GameFlag_Set(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
    FieldScript_End();
  }
}

/// Arriving in Kraden's part of Vale: places the object events for the storm or
/// for the day, and plays the arrival from the cottage.
///
/// The map's initialize entry. On the stormy night (FLAG_VALE_STORM) it starts
/// the lightning, removes the day's people, lays the survivor by the fence
/// (unless FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) and blocks the cells the
/// rockslide covers. By day it removes the rocks and Garet, runs
/// EventScript_KradenSetsOff from VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH,
/// removes Kraden and the strangers once FLAG_SANCTUM_VISIT_PLANNED is set, and
/// places the cottage visitor after Kalay. Clears FLAG_VALE_KRADENS_DOOR_OPEN
/// when the party keeps its position.
s32 ValeKradensCottage_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 debrisRow;

  if (gSaveBlock.warpDest == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_UNUSED ||
      gSaveBlock.warpDest == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  if (Script_GetFlag(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_VALE_KRADENS_DOOR_OPEN);
  }
  // The rockslide chapter replaces residents with debris and a survivor.
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Weather_StartRain();
    Weather_StartLightning();
    FieldScript_Start();
    Import_ObjectEventSetPosition(LOCALID_BOY_BY_THE_FENCE, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_KRADEN, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_SATUROS, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_MENARDI, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_JENNA, 0, 0);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_GetById(LOCALID_FENCE_BOULDER);

      objectEvent->collisionFlags = 8 | objectEvent->collisionFlags;
    }
    ObjectEvent_SetPosition(LOCALID_ROCKSLIDE_SURVIVOR, PIXELS(83),
                            PIXELS(265));
    ObjectMotion_SetPositionAndReset(LOCALID_ROCKSLIDE_SURVIVOR, 83, 273);
    Import_ObjectEventSetAnimation(LOCALID_ROCKSLIDE_SURVIVOR, 5);
    objectEvent = ObjectEvent_GetById(LOCALID_ROCKSLIDE_SURVIVOR);
    {
      s32 collisionRadius = 12;

      objectEvent->radius = collisionRadius;
    }
    ObjectEvent_SetActionScript(LOCALID_ROCKSLIDE_SURVIVOR,
                                gValeKradensCottage_RockslideSurvivorSquashes);
    if (GameFlag_Get(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) != 0) {
      Import_ObjectEventSetPosition(LOCALID_ROCKSLIDE_SURVIVOR, 0, 0);
    }
    debrisRow = 21;
    FieldScript_End();
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 14, debrisRow);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 15, debrisRow);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 23, 19);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 24, 19);
    debrisRow = 20;
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 23, 20);
    MapGrid_CopyCellAttributes(9, 24, 1, 1, 24, 20);
  } else {
    Import_ObjectEventSetPosition(LOCALID_WEST_ROCK, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_EAST_ROCK, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
    Import_ObjectEventSetPosition(LOCALID_ROCKSLIDE_SURVIVOR, 0, 0);
    FieldScript_Start();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gSaveBlock.warpDest == VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH) {
        EventScript_KradenSetsOff();
      }
    }
    if (GameFlag_Get(FLAG_SANCTUM_VISIT_PLANNED) != 0) {
      Import_ObjectEventSetPosition(LOCALID_KRADEN, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_SATUROS, 0, 0);
      Import_ObjectEventSetPosition(LOCALID_MENARDI, 0, 0);
    } else {
      if (GameFlag_Get(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(392), PIXELS(376));
        ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(376), PIXELS(376));
        Object_SetTargetAndCallback(
            LOCALID_SATUROS, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
            gValeKradensCottage_FaceTargetActions);
        Object_SetTargetAndCallback(
            LOCALID_MENARDI, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
            gValeKradensCottage_FaceTargetActions);
      }
    }
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      ObjectEvent_SetPosition(LOCALID_COTTAGE_VISITOR, PIXELS(132),
                              PIXELS(264));
    }
    FieldScript_End();
  }
  return 0;
}

/// In Vale on the stormy night, the man hurt in the rockslide asks whether he
/// will die: yes, and he gives up; no, and he finds he is unhurt, warns them
/// about the monsters and runs off.
///
/// Reached on the stormy night by talking to him (LOCALID_ROCKSLIDE_SURVIVOR),
/// or by stepping on metatile behavior 20 until he has given up. Sets
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP on yes, or
/// FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED on no; once he has escaped it does
/// nothing.
static void EventScript_TalkToRockslideSurvivor(void) {
  struct ObjectEvent *leader;
  s32 message;

  if (Script_GetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) == 0) {
    if (Script_GetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP) != 0) {
      FieldScript_Start();
      ObjectMotion_SetVarCbAndRefresh(LOCALID_ROCKSLIDE_SURVIVOR, 2);
      FieldScript_SetMessageId(MsgValeSaveYourselves);
      FieldScript_ShowMessageAndWait(LOCALID_ROCKSLIDE_SURVIVOR, 0);
      FieldScript_End();
    } else {
      FieldScript_Start();
      ObjectMotion_EnableActionAndResetMotion(LOCALID_ROCKSLIDE_SURVIVOR);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_ROCKSLIDE_SURVIVOR, 1);
      // He gasps out the rockslide and the monsters; Isaac and Garet hurry
      // over to him.
      message = MsgValeRockslideDestroyedFence;
      FieldScript_SetMessageId(message);
      FieldScript_ShowMessageThenWait(LOCALID_ROCKSLIDE_SURVIVOR, 0, 20);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      FieldCamera_MoveWithinMapBounds(PIXELS(98), -1, PIXELS(283), 1);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 94, 293);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (leader != 0) {
        Import_ObjectEventSetPosition(LOCALID_GARET, leader->x.fixed,
                                      leader->z.fixed);
      }
      ObjectEvent_WalkToAndWait(LOCALID_GARET, 110, 279);
      ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 40);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_ROCKSLIDE_SURVIVOR, 2);
      FieldScript_WaitFrames(40);
      // He asks whether he will die. Yes: he gives up and tells them to save
      // themselves.
      FieldMessage_ShowAtObject(LOCALID_ROCKSLIDE_SURVIVOR, 0);
      if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
        ObjectMotion_SetVarCbAndRefresh(LOCALID_ROCKSLIDE_SURVIVOR, 2);
        FieldScript_WaitFrames(20);
        FieldScript_SetMessageId((message + 2));
        FieldScript_ShowMessageAndWait(LOCALID_ROCKSLIDE_SURVIVOR, 0);
        GameFlag_Set(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP);
      } else {
        // No: he laments, then jumps up, finds he is not hurt at all, and
        // warns them of the monsters on the way to the plaza.
        ObjectMotion_SetVarCbAndRefresh(LOCALID_ROCKSLIDE_SURVIVOR, 2);
        FieldScript_WaitFrames(20);
        FieldScript_SetMessageId((message + 3));
        FieldScript_ShowMessageThenWait(LOCALID_ROCKSLIDE_SURVIVOR, 0, 40);
        ObjectMotion_SetAngleToward(LOCALID_ROCKSLIDE_SURVIVOR,
                                    LOCALID_PARTY_LEADER, 0);
        Import_ObjectEventSetAnimation(LOCALID_ROCKSLIDE_SURVIVOR, ANIM_STAND);
        ObjectMotion_Launch(LOCALID_ROCKSLIDE_SURVIVOR, 4, 40);
        Import_ObjectEventSetAnimation(LOCALID_ROCKSLIDE_SURVIVOR, 6);
        ObjectEvent_ShowEmote(LOCALID_ROCKSLIDE_SURVIVOR,
                              EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
        FieldScript_ShowMessageThenWait(LOCALID_ROCKSLIDE_SURVIVOR, 0, 10);
        Import_ObjectEventSetAnimation(LOCALID_ROCKSLIDE_SURVIVOR, ANIM_STAND);
        FieldScript_WaitFrames(10);
        ObjectMotion_SetAnimationAndWait(LOCALID_ROCKSLIDE_SURVIVOR, ANIM_NOD);
        FieldScript_ShowMessageThenWait(LOCALID_ROCKSLIDE_SURVIVOR, 0, 10);
        ObjectMotion_SetAnimationAndWait(LOCALID_ROCKSLIDE_SURVIVOR, ANIM_NOD);
        // Follow his escape until the party can see that he is unharmed.
        Object_SetTargetAndCallback(LOCALID_PARTY_LEADER,
                                    OBJ_EVENT_LINK_KEEP_MOTION |
                                        LOCALID_ROCKSLIDE_SURVIVOR,
                                    gValeKradensCottage_FaceTargetActions);
        Object_SetTargetAndCallback(LOCALID_GARET,
                                    OBJ_EVENT_LINK_KEEP_MOTION |
                                        LOCALID_ROCKSLIDE_SURVIVOR,
                                    gValeKradensCottage_FaceTargetActions);
        ObjectEvent_RunActionScriptAndWait(
            LOCALID_ROCKSLIDE_SURVIVOR,
            gValeKradensCottage_RockslideSurvivorEscapeActions);
        ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
        ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
        ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
        ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 60);
        ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        FieldEffect_SpawnLinked(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                                120);
        GameFlag_Set(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
      }
      // Garet walks back into Isaac.
      Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (leader != 0) {
        ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                         leader->z.part.pixel);
      }
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
      Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
      FieldScript_End();
    }
  }
}

/// In Vale on the stormy night, a monster attacks by the broken fence.
///
/// Reached by stepping on metatile behavior 10 until
/// FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts the first slot of the encounter
/// zone of the scripted battles of Vale, Vault and the world map.
static void EventScript_FightFirstMonster(void) {
  struct FieldWork *work;

  FieldScript_Start();

  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  GameFlag_Set(FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 0);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  FieldScript_End();
}

/// In Vale on the stormy night, a second monster attacks by the broken fence.
///
/// Reached by stepping on metatile behavior 11 until
/// FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT is set. Sets it and
/// FLAG_VALE_STORM_MONSTER_FOUGHT, and starts its second slot.
static void EventScript_FightSecondMonster(void) {
  struct FieldWork *work;

  FieldScript_Start();

  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 64;

  GameFlag_Set(FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 1);
  GameFlag_Set(FLAG_VALE_STORM_MONSTER_FOUGHT);
  FieldScript_End();
}

/// A villager at Kraden's cottage asks Isaac about his memories of it. This is
/// in Vale after the party has reached Kalay.
///
/// Reached by talking to the cottage visitor (LOCALID_COTTAGE_VISITOR) in the
/// events after Kalay.
static void EventScript_TalkToCottageVisitor(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeMemoriesOfThisCottage);
  FieldScript_AskYesNoAndShowResponse(LOCALID_COTTAGE_VISITOR, 0);
  FieldScript_End();
}

/// Leads the party away from the broken fence, then leaves the map.
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

/// Keeps a linked object event facing its current target.
s32 gValeKradensCottage_FaceTargetActions[] = {
    SC_CALL(Map_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct MapWarpDest gValeKradensCottage_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_NORTH, CONDITION_ALWAYS, 454, 0, 290,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH, CONDITION_ALWAYS, 454, 0, 443,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_INN_NORTH, CONDITION_ALWAYS, 229, 0, 233,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_WEST, CONDITION_ALWAYS, 72, 0,
     80, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST, CONDITION_ALWAYS, 104, 0,
     104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_INN_SOUTH, CONDITION_ALWAYS, 232, 0, 264,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gValeKradensCottage_Warps[] = {
    MAP_WARPS(ValeKradensCottage),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_NORTH, Vale,
             VALE_FROM_VALE_KRADENS_COTTAGE_NORTH),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_SOUTH, Vale,
             VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_INN_NORTH, ValeInn,
             VALE_INN_FROM_VALE_KRADENS_COTTAGE_NORTH),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST, ValeCaveMouth,
             VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST, ValeCaveMouth,
             VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH, ValeStormyNight,
             VALE_STORMY_NIGHT_FROM_VALE_KRADENS_COTTAGE_NORTH),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH, ValeStormyNight,
             VALE_STORMY_NIGHT_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    MAP_WARP(VALE_KRADENS_COTTAGE_TO_VALE_INN_MYTHRIL_BAG, ValeInn,
             VALE_INN_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gValeKradensCottage_ObjectEventTemplates[] = {
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
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 3, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gValeKradensCottage_Events[] = {
    { MAP_EVENT_TALK, LOCALID_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_KradenPlansSanctumVisit) },
    { MAP_EVENT_TALK, LOCALID_BOY_BY_THE_FENCE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBoyByTheFence) },
    { MAP_EVENT_TALK, LOCALID_SATUROS, CONDITION_ALWAYS, MSGNO(MsgValeDoNotKeepKradenWaiting) },
    { MAP_EVENT_TALK, LOCALID_MENARDI, CONDITION_ALWAYS, MSGNO(MsgValeLeaveBeforeWeChangeMinds) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeCuriousBeyondFence) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterCottage) },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OverhearKraden) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetSaturosAndMenardi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
    { BG_EVENT(BG_EVENT_JAR), 100, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeKradensCottage_StormEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_STORMY_NIGHT_SOUTH },
    { MAP_EVENT_TALK, LOCALID_WEST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { MAP_EVENT_TALK, LOCALID_EAST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { MAP_EVENT_TALK, LOCALID_ROCKSLIDE_SURVIVOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToRockslideSurvivor) },
    { MAP_EVENT_COORD, 20, FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP, EVENT_SCRIPT(EventScript_TalkToRockslideSurvivor) },
    { MAP_EVENT_COORD, 10, FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT, EVENT_SCRIPT(EventScript_FightFirstMonster) },
    { MAP_EVENT_COORD, 11, FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT, EVENT_SCRIPT(EventScript_FightSecondMonster) },
    { BG_EVENT(BG_EVENT_JAR), 100, CONDITION_ALWAYS, 0 },
    { MAP_EVENTS_END },
};

struct MapEvent gValeKradensCottage_EventsAfterKalay[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterCottage) },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { MAP_EVENT_TALK, LOCALID_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeWonderBeyondFence) },
    { MAP_EVENT_TALK, LOCALID_COTTAGE_VISITOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToCottageVisitor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeAdventuresBeyondFence) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COTTAGE_VISITOR, CONDITION_ALWAYS, MSGNO(MsgValeLonelyWithoutKraden) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
#if TBS_PATCH_3
    { BG_EVENT(BG_EVENT_JAR), 100, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
#endif
    { MAP_EVENTS_END },
};
// clang-format on

u16 gValeKradensCottage_GateCells[] = {
    MAPGRID_ANIMATION_STEP(2, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEP(4, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEPS_END,
};
