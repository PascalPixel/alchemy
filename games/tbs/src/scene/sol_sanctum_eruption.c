/// @file
/// Elemental Star Room (SceneId_SolSanctumEruption), the chamber of
/// SceneId_SolSanctumElementalStarRoom (sol_sanctum_elemental_star_room.c) as
/// it collapses once the Mars Star is taken: one event, with no map events of
/// its own.
///
/// SolSanctumEruption_Initialize plays it on arrival at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, in two parts:
/// - SolSanctumEruption_PlayHostageEvent: the chamber quakes, the Wise One
///   appears and stills it, and Saturos's group leaves with Jenna and Kraden,
///   rising out of the chamber one by one. Jenna leaves the party.
/// - SolSanctumEruption_PlayWiseOneRescue: the Wise One has Isaac keep the Mars
///   Star, then sends Isaac and Garet away in a white flash. It sets
///   FLAG_SOL_SANCTUM_ERUPTED, which every room of Sol Sanctum tests from then
///   on, and FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, and takes
///   SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS to the Luna and Sol
///   rooms (SceneId_SolSanctumLunaSolRooms).
///
/// The effects are the event's own. Sixteen blue crystals thrown out in a ring
/// (SolSanctumEruption_ThrowEruptionRing) become embers launched from a pool at
/// a varying rate (SolSanctumEruption_StepEmbers and
/// SolSanctumEruption_UpdateEmberFrequency, callbacks the first part adds and
/// the second removes); once Saturos's group starts to leave
/// (FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING), they fall silently. The
/// breaking floor's cells flicker by callback until the Wise One holds them,
/// ten arcs of light rise at each of the chamber's four corners as the Wise One
/// tells of the stars (SolSanctumEruption_RaiseElementalArcs), and the Wise One
/// glows through a cycle of palettes.
///
/// Saturos's group and their hostages leave one by one from the platform where
/// the party arrived (SolSanctumEruption_RaiseAndShrinkObjectEvent): each walks
/// onto it, rises 30 pixels at full size, then rises on as he shrinks away,
/// over a shadow that shrinks with him: the platform's object event,
/// LOCALID_PLATFORM_SHADOW, draws that shadow.

#include "action_script.h"
#include "save_block.h"
#include "unit/scene/sol_sanctum_eruption.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/scene/common/sol_sanctum_elemental_star_ring_setup.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
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
#include "resource.h"
#include "sol_sanctum_eruption.h"
#include "types.h"
#include "animspr.h"
#include "script_layer.h"
#include "field_service.h"
#include "iwram_call.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "map_tables.h"
#include "objcmd.h"
#include "object_event.h"
#include "heap_slots.h"
#include "vram_cache.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "heap.h"
#include "io_reg.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"
#include "fx_work.h"

/// Saturos's group has started to leave, and the embers fall without their
/// sound.
/// ★ Named from the game: "Please...can we go now?"
/// (MsgSolSanctumKradenCouldThatBeThe)
#define FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING FLAG_SCENE_TEMP_46

static void SolSanctumEruption_RaiseAndShrinkObjectEvent(s32 localId);
static void SolSanctumEruption_ThrowEruptionRing(void);
static void SolSanctumEruption_ResetEmberEmission(void);
static void SolSanctumEruption_StepEmbers(void);
static void SolSanctumEruption_UpdateEmberFrequency(void);
static void SolSanctumEruption_PlayWiseOneRescue(void);
static void SolSanctumEruption_PlaceBeforeStar(s32 localId);
static void SolSanctumEruption_DrawFirstCollapseTiles(s32 alternate);
static void SolSanctumEruption_DrawSecondCollapseTiles(s32 alternate);
static void SolSanctumEruption_FlickerFirstCollapseTiles(void);
static void SolSanctumEruption_FlickerSecondCollapseTiles(void);
static void SolSanctumEruption_RaiseElementalArcs(u32 corner);
static void SolSanctumEruption_UpdateElementalArcs(void);
static void SolSanctumEruption_FadeCameraTo(s32 x, s32 z);
static void SolSanctumEruption_RestoreCornerCellAttributes(void);
static void SolSanctumEruption_SetMaterializationTint(s32 enabled, s32 frames,
                                                      s32 waitFrames);
static void SolSanctumEruption_FlashMaterializingObjectEvent(
    struct ObjectEvent *objectEvent);
static void SolSanctumEruption_SpawnEffectPair(struct ObjectEvent *parent);
static void SolSanctumEruption_UpdateWiseOnePalette(void);

// Map flags, cleared on every ordinary arrival:
// - FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING: set as Saturos's group starts to
//   leave; from then on the embers fall without their sound (SE_GLITTER).

/// The four corners of the chamber where SolSanctumEruption_RaiseElementalArcs
/// raises lights, in the order the Wise One's event visits them.
enum SolSanctumEruptionCorner {
  CHAMBER_NORTHWEST = 0,
  CHAMBER_SOUTHWEST = 1,
  CHAMBER_NORTHEAST = 2,
  CHAMBER_SOUTHEAST = 3
};

enum {
  /// ★ Named from the game: "Could that be . . . the Wise One?!?"
  /// (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_WISE_ONE = LOCALID_ALEX + 1,
  /// The sixteen crystals thrown out in a ring.
  /// ★ Named from what the player sees: a blue crystal that bursts into
  /// sparkles (its render)
  LOCALID_FIRST_CRYSTAL = LOCALID_WISE_ONE + 1,
  CRYSTAL_COUNT = 16
};

enum { OBJECT_ELEMENTAL_STAR = 22 };

#define ELEMENTAL_ARC_SCALE_X_DECREASE 7372
#define EMBER_EMISSION_DISABLED 999
#define ARC_SCAN_STILL_ACTIVE 888

const struct MapWarpDest *SolSanctumEruption_GetWarpDests(void) {
  return gSolSanctumEruption_WarpDests;
}

const struct MapWarpRegion *SolSanctumEruption_GetRegions(void) { return NULL; }

const u32 *SolSanctumEruption_GetWarps(void) {
  return gSolSanctumEruption_Warps;
}

/// Returns Garet and Jenna, then the platform's shadow
/// (LOCALID_PLATFORM_SHADOW), Kraden, Saturos, Menardi, Felix masked and
/// unmasked, Alex, the Wise One (LOCALID_WISE_ONE) and sixteen crystals (from
/// LOCALID_FIRST_CRYSTAL), all placed off the map until the event moves
/// them.
const struct ObjectEventTemplate *
SolSanctumEruption_GetObjectEventTemplates(void) {
  return gSolSanctumEruption_ObjectEventTemplates;
}

/// Returns the empty event table: the map is one event.
const struct MapEvent *SolSanctumEruption_GetEvents(void) {
  return gSolSanctumEruption_Events;
}

/// Stages Saturos, Menardi, Alex and Felix around Jenna and Kraden.
static inline void SolSanctumEruption_PlaceHostageGroup(void) {
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(475), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(491), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(459), PIXELS(348));
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(507), PIXELS(348));
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(471), PIXELS(306));
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(479), PIXELS(362));
}

/// Elemental Star Room, collapsing: the Wise One stills the first quake,
/// Saturos's group argues over the Mars Star and leaves with Jenna and Kraden
/// as hostages, and the Wise One then rescues Isaac and Garet. Called by
/// SolSanctumEruption_Initialize at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, the cut from
/// SceneId_SolSanctumElementalStarRoom. Sets
/// FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING, plays
/// SolSanctumEruption_PlayWiseOneRescue and then removes Jenna from the party.
static void SolSanctumEruption_PlayHostageEvent(void) {
  void ObjectEvent_ShowEmote();
  void PaletteFade_ApplyTarget();
  void Script_PlaySong();

  struct ObjectEvent *objectEvent;
  struct WeatherWork *sparkles;
#ifndef UBFIX
  struct WeatherWork **particleWorkSlot;
  struct FieldWork **fieldWorkSlot;
#endif

#ifdef UBFIX
  sparkles = gWeatherWork;
#else
  particleWorkSlot = &gWeatherWork;
  sparkles = *particleWorkSlot;
#endif
  FieldScript_Start();
  sparkles->stopped = 0;
  // Reopen the chamber during the first quake and stage the two groups.
  Script_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(232), PIXELS(156));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(218), PIXELS(172));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  SolSanctumEruption_PlaceHostageGroup();
  ObjectMotion_SetActionVariant(LOCALID_WISE_ONE, 1);
#ifndef UBFIX
  // UB: these are separate work-pointer globals. The native sequence reaches
  // the field heap slot through the particle heap slot; named globals add
  // four bytes and change this event's register allocation in tbs-en.
  fieldWorkSlot = (struct FieldWork **)((u8 *)particleWorkSlot - 8);
#endif
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(156), 0);
  MapGrid_CenterOnFollowedPoint();
#ifdef UBFIX
  gFieldWork->transitionFrames = 8;
#else
  (*fieldWorkSlot)->transitionFrames = 8;
#endif
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  FieldScript_WaitFrames(4);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  FieldScript_WaitFrames(4);
  SolSanctumEruption_ThrowEruptionRing();
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  FieldScript_WaitFrames(16);
  Script_PlaySong(SE_CLATTER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  FieldScript_WaitFrames(4);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  FieldScript_WaitFrames(4);
  Script_PlaySong(SE_CLATTER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(48);
  FieldScript_WaitFrames(48);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 20);
  // Off screen, Menardi: "What happened?" Saturos: "Not again!" Jenna: the
  // water's gone. Alex: without the Elemental Stars the chamber is
  // collapsing. Kraden: is this the "terrible retribution"?
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 40);
  FieldScript_SetMessageId(MsgSolSanctumJennaWhatHappened);
  FieldScript_ShowMessageThenWait(LOCALID_MENARDI, 0, 20);
  FieldScript_ShowMessageAndWait(LOCALID_SATUROS, 0);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 30);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  // The Wise One, a floating rock, appears over the chamber and the quake
  // stops.
  objectEvent = ObjectEvent_GetById(LOCALID_WISE_ONE);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(325), PIXELS(302));
  objectEvent = ObjectEvent_GetById(LOCALID_WISE_ONE);
  objectEvent->motionFlags = 5;
  sparkles->stopped = 1;
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(150);
  // Menardi: "It's over... But what was it?" Jenna: "What is that?!?"
  // Saturos: "No!" The camera pans to the Wise One.
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  Script_ShowMessage(LOCALID_JENNA, 0);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  SolSanctumEruption_PlaceHostageGroup();
  FieldCamera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(299), 1);
  FieldCamera_WaitForArrival();
  Script_PlaySong(SE_MAP_TRANSITION);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 20, 20),
                          PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(20);
  WaitFrames(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(200);
  // Garet: "That rock... It's floating... Do you know what it is?" Yes:
  // "Yeah, I know it's a rock, stupid!" No: someone is lifting it with
  // Psynergy. Kraden: "Could that be... the Wise One?!?"
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_SetMessageId(MsgSolSanctumIKnowItsARock);
  } else {
    FieldScript_SetMessageId(MsgSolSanctumSomeoneIsLiftingIt);
  }
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0, 80);
  FieldScript_SetMessageId(MsgSolSanctumKradenCouldThatBeThe);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  Script_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(80);
  sparkles->stopped = 1;
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  SolSanctumEruption_SetMaterializationTint(0, 20, 60);
  // Alex: "Such power... Incredible!" Garet: "!!!" Jenna: "Wow..."
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_NORTHWEST, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_GARET, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST + FACING_STEP, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 30);
  // Embers start to rain from the Wise One.
  SolSanctumEruption_ResetEmberEmission();
  Task_AddCallback(SolSanctumEruption_StepEmbers, TASK_PRIORITY_MAP);
  Task_AddCallback(SolSanctumEruption_UpdateEmberFrequency, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(240);
  // Saturos: "It must be the guardian of the Elemental Stars!" The camera
  // moves to his group, all facing west.
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 30);
  SolSanctumEruption_PlaceHostageGroup();
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_JENNA);
  FieldCamera_MoveTo(objectEvent->x.part.pixel * PIXELS(1), -1,
                     objectEvent->z.part.pixel * PIXELS(1), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  FieldScript_WaitFrames(20);
  // Felix: they should escape while they can. Menardi: not without the Mars
  // Star. Alex sides with Felix, and Saturos agrees to leave it.
  Script_ShowMessage(LOCALID_FELIX, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  Import_ObjectEventSetAnimation(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_LinkPair(LOCALID_SATUROS, LOCALID_MENARDI, 40);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  // Jenna: "What about Isaac and Garet? You can't just leave them here!"
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 40);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_JENNA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_LinkPair(LOCALID_SATUROS, LOCALID_MENARDI, 40);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            10);
  // Saturos: "Forget about them." Menardi: they might survive. Saturos: "So
  // what do you suggest we do?" Alex: take Jenna, and they will bring the
  // Star to get her back.
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTH + FACING_STEP, 20);
  // Alex: "Regardless, Kraden must come with us."
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // Felix steps forward: Alex is breaking their promise. Alex: conditions
  // have changed. Felix will not let them hurt Jenna; Kraden offers himself
  // in her place.
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 471, 314);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 10);
  Import_ObjectEventSetAnimation(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  // Saturos: "you cannot help Jenna now." Menardi: if they leave her, she
  // will die; is that what Felix wants?
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_LinkPair(LOCALID_SATUROS, LOCALID_MENARDI, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_WEST, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  // Alex: "Perhaps we ought to continue this elsewhere, Felix." Jenna:
  // "Felix..." Alex: "Please...can we go now?"
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_ALEX, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_JENNA, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ObjectEvent_LinkPair(LOCALID_SATUROS, LOCALID_MENARDI, 20);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkTo(LOCALID_MENARDI, 475, 348);
  ObjectEvent_WalkTo(LOCALID_SATUROS, 491, 348);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MENARDI);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SATUROS);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 10);
  // Menardi: "Let's make haste!" She shoves Jenna, who stumbles away.
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI, 0);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 338);
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->steeringFlags = 254 & objectEvent->steeringFlags;
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 348);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  ObjectMotion_Launch(LOCALID_JENNA, 4, 0);
  ObjectEvent_WalkToAndWait(LOCALID_JENNA, 459, 316);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  // Felix: "Hey, don't push my sister!" Menardi: "You owe us your life!"
  // Saturos: "Menardi!"
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  FieldScript_WaitFrames(20);
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 332);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectMotion_SetVariantCallback(LOCALID_SATUROS, 2);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP,
                            20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkTo(LOCALID_JENNA, 432, 316);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 422, 311);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  // Felix leads Jenna aside. Saturos: "Master Kraden, you will be joining us,
  // of course!" All gather under the departure point.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_SATUROS, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 491, 296);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 471, 308);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 455, 308);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 487, 308);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(300), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING);
  // One by one they rise out of the chamber: Saturos, Kraden, Menardi, then
  // Jenna ("Isaac, Garet! Don't die!") and Felix, and last Alex, who regrets
  // leaving the Mars Star.
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_SATUROS);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_KRADEN);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_MENARDI);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST + FACING_STEP, 40);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_JENNA, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTHEAST, 0);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_JENNA);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHWEST + FACING_STEP, 40);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_ALEX);
  SolSanctumEruption_PlayWiseOneRescue();
  Party_RemoveMemberRestored(JENNA);
}

/// Lifts one departing object event and shrinks it together with its shadow on
/// the platform (LOCALID_PLATFORM_SHADOW).
static void SolSanctumEruption_RaiseAndShrinkObjectEvent(s32 localId) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *shadow;
  struct ObjectEvent *target;
  u8 frame;

  shadow = ObjectEvent_GetById(LOCALID_PLATFORM_SHADOW);
  shadow->scaleX = Q16_ONE;
  shadow->scaleY = Q16_ONE;
  ObjectMotion_SetPositionAndReset(localId, 471, 290);
  ObjectEvent_FaceDirection(localId, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, PIXELS(471), PIXELS(290));
  objectEvent = ObjectEvent_GetById(localId);
  target = ObjectEvent_GetById(localId);
  ObjectEvent_SetSpriteFlags(target, 0);
  ObjectEvent_SetSpriteColorById(localId, SPRITE_COLOR_BLINK);
  objectEvent->motionFlags = 0;
  Script_PlaySong(SE_ASCEND);
  // Rise at full size, then shrink the object event and its shadow together.
  frame = 0;
  do {
    objectEvent->y.fixed += PIXELS(1) / 2;
    FieldScript_WaitFrames(1);
    frame++;
  } while (frame != 60);
  Script_PlaySong(SE_WARP);
  frame = 0;
  do {
    objectEvent->y.fixed += PIXELS(1) / 10;
    objectEvent->scaleX -= Q16_ONE / 100;
    objectEvent->scaleY -= Q16_ONE / 100;
    shadow->scaleX -= Q16_ONE / 100;
    shadow->scaleY -= Q16_ONE / 100;
    FieldScript_WaitFrames(1);
    frame++;
  } while (frame != 90);
  ObjectEvent_SetPosition(localId, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, 0, 0);
}

UNUSED static void SolSanctumEruption_UnusedCallbackA(void) {}

/// Unused: a second empty function nothing calls.
UNUSED static void SolSanctumEruption_UnusedCallbackB(void) {}

/// Plays the collapse after the cut from the Elemental Star Room. It plays at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, the cut from the
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom). Called by
/// SceneManager_Run once the object events exist.
s32 SolSanctumEruption_Initialize(void) {
  if (gSaveBlock.warpDest ==
      SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM) {
    FieldEffect_InitializeSparkles();
    SolSanctumEruption_PlayHostageEvent();
  }
  return 0;
}

/// Keeps a flying ember sprite rotated a quarter-turn from its facing. Run by
/// each ember's object script, gEruption_EmberScript.
static s32 SolSanctumEruption_AlignEmberToHeading(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  objectEvent->sprite->rotation = objectEvent->facing + 0x4000;
  return 1;
}

/// Launches sixteen crystals outward, then resets their object events for ember
/// emission.
static void SolSanctumEruption_ThrowEruptionRing(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *spun;
  u32 crystalIndex;
  s32 angle;
  struct Sprite *sprite;
  s32 offsetX;
  s32 offsetZ;

  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    objectEvent = ObjectEvent_GetById(crystalIndex);
    ObjectMotion_EnableActionAndResetMotion(crystalIndex);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    Object_SelectAnimation(objectEvent, 2);
    objectEvent->sprite->image.priority = 0;
    objectEvent->motionFlags = 0;
    objectEvent->speed = PIXELS(8);
    objectEvent->acceleration = PIXELS(3) / 4;
    objectEvent->scaleX = 9 * Q16_ONE / 5;
    objectEvent->scaleY = 9 * Q16_ONE / 5;
    objectEvent->x.fixed = PIXELS(232);
    objectEvent->y.fixed = PIXELS(20);
    objectEvent->z.fixed = PIXELS(132);
  }
  Sound_PlaySong(SE_CRASH);
  for (crystalIndex = 0; crystalIndex <= 15; crystalIndex++) {
    spun = ObjectEvent_GetById(crystalIndex + 16);
    sprite = spun->sprite;
    angle = crystalIndex << 12;
    sprite->rotation = angle - 0x4000;
    offsetX = Iwram_MulQ16(Trig_Cos(angle), PIXELS(256));
    offsetZ = Iwram_MulQ16(Trig_Sin(angle), PIXELS(256));
    Object_SetPosition(spun, spun->x.fixed + offsetX, spun->y.fixed,
                       spun->z.fixed + offsetZ);
  }
  FieldScript_WaitFrames(20);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_FIRST_CRYSTAL);
  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    objectEvent = ObjectEvent_GetById(crystalIndex);
    ObjectMotion_EnableActionAndResetMotion(crystalIndex);
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
    objectEvent->x.fixed = 0;
    objectEvent->y.fixed = 0;
    objectEvent->z.fixed = 0;
    SET_VELOCITY(objectEvent, 0, 0, 0);
    objectEvent->targetX = OBJ_EVENT_NO_TARGET;
    objectEvent->targetY = OBJ_EVENT_NO_TARGET;
    objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  }
}

/// Resets the sixteen ember slots and their emission timers.
static void SolSanctumEruption_ResetEmberEmission(void) {
  s32 *stateSlot;
  u32 emberIndex;

  gEmberMask = 63;
  gEmberTimer = 0;
  gEmberLevel = 0;
  gEmberLevelTimer = 120;
  stateSlot = gEmberState;
  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    *stateSlot++ = 0;
  }
}

/// Retires landed embers and launches new ones, shaking the map. It launches
/// the next free one at the current frequency, shaking the map after each. Run
/// each frame as a callback that SolSanctumEruption_PlayHostageEvent adds and
/// SolSanctumEruption_PlayWiseOneRescue removes.
static void SolSanctumEruption_StepEmbers(void) {
  struct WeatherWork *sparkles;
  s32 shakePixels;
  s32 shakeTimer;
  u8 emberIndex;
  struct ObjectEvent *landedEmber;
  struct ObjectEvent *ember;
  s16 angle;
  s32 x;
  s32 z;

  sparkles = gWeatherWork;
  shakePixels = (u32)gEmberTimer / 10;
  if (shakePixels != 0) {
    sparkles->stopped = 0;
    MapGrid_SetShake(shakePixels * PIXELS(1), shakePixels * PIXELS(1), Q16_ONE);
  } else {
    sparkles->stopped = 1;
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  }
  shakeTimer = gEmberTimer;
  if (shakeTimer != 0)
    gEmberTimer = shakeTimer - 3;

  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    if (gEmberState[emberIndex] != 0) {
      landedEmber = ObjectEvent_GetById(emberIndex + 16);
      if (landedEmber->targetX == OBJ_EVENT_NO_TARGET &&
          landedEmber->targetZ == OBJ_EVENT_NO_TARGET) {
        gEmberState[emberIndex]++;
        if (gEmberState[emberIndex] == 2)
          Object_SelectAnimation(landedEmber, 3);
        if (gEmberState[emberIndex] == 19) {
          landedEmber->x.fixed = 0;
          landedEmber->y.fixed = 0;
          landedEmber->z.fixed = 0;
          SET_VELOCITY(landedEmber, 0, 0, 0);
          landedEmber->targetX = OBJ_EVENT_NO_TARGET;
          landedEmber->targetY = OBJ_EVENT_NO_TARGET;
          landedEmber->targetZ = OBJ_EVENT_NO_TARGET;
          ObjectEvent_SetSpriteColor(landedEmber, SPRITE_COLOR_HIDDEN);
        } else if (gEmberState[emberIndex] == 20) {
          gEmberState[emberIndex] = 0;
        }
      }
    }
  }

  if (gEmberMask == EMBER_EMISSION_DISABLED)
    return;
  if (gEmberMask & gFrameCount)
    return;
  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    angle = Math_RemainderUnsigned(Import_RandomNext(), 0xffff);
    ember = ObjectEvent_GetById(emberIndex + 16);
    if (gEmberState[emberIndex] == 0) {
      if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING) == 0)
        Sound_PlaySong(SE_GLITTER);
      gEmberState[emberIndex] = 1;
      ember->motionFlags = 0;
      ember->speed = PIXELS(8);
      ember->acceleration = PIXELS(1);
      ObjectEvent_SetSpriteFlags(ember, 0);
      ember->sprite->image.priority = 1;
      Object_SelectAnimation(ember, 2);
      ObjectEvent_SetScript(ember, gEruption_EmberScript);
      ember->x.fixed =
          Iwram_MulQ16(Trig_Cos((u16)angle),
                       (((u32)(Import_RandomNext() << 8) >> 16) << 16) +
                           PIXELS(256)) +
          PIXELS(325);
      ember->y.fixed = 0;
      ember->z.fixed =
          Iwram_MulQ16(Trig_Sin((u16)angle),
                       (((u32)(Import_RandomNext() << 8) >> 16) << 16) +
                           PIXELS(256)) +
          PIXELS(302);
      x = Iwram_MulQ16(Trig_Cos((u16)angle),
                       ((Import_RandomNext() & 63) << 16) + PIXELS(8));
      z = Iwram_MulQ16(Trig_Sin((u16)angle),
                       ((Import_RandomNext() & 63) << 16) + PIXELS(8));
      Object_SetPosition(ember, x + PIXELS(325), 0, z + PIXELS(286));
      ObjectEvent_SetSpriteColor(ember, SPRITE_COLOR_NORMAL);
      gEmberTimer = 30;
      return;
    }
  }
}

/// Varies the ember emission interval and its duration. Run each frame as a
/// callback beside SolSanctumEruption_StepEmbers.
static void SolSanctumEruption_UpdateEmberFrequency(void) {
  u32 emissionLevel;

  if (gEmberLevelTimer != 0) {
    gEmberLevelTimer--;
    return;
  }
  if (gEmberLevel != 0) {
    gEmberLevel--;
  } else {
    gEmberLevel = (u32)(Import_RandomNext() << 2) >> 16;
  }
  emissionLevel = gEmberLevel;
  switch (emissionLevel) {
  case 3:
    gEmberMask = emissionLevel;
    gEmberLevelTimer = ((u32)(Import_RandomNext() * 20) >> 16) + 40;
    break;
  case 2:
    gEmberMask = 15;
    gEmberLevelTimer = ((u32)(Import_RandomNext() * 40) >> 16) + 80;
    break;
  case 1:
    gEmberMask = 63;
    gEmberLevelTimer = ((u32)(Import_RandomNext() * 80) >> 16) + 160;
    break;
  default:
    gEmberMask = 127;
    gEmberLevelTimer = ((u32)(Import_RandomNext() * 160) >> 16) + 320;
    break;
  }
}

/// Ends the Wise One's palette animation and clears its attached effect.
static inline void SolSanctumEruption_FinishMaterialization(void) {
  ObjectMotion_SetAnimationAndWait(LOCALID_WISE_ONE, 3);
  Task_Remove(SolSanctumEruption_UpdateWiseOnePalette);
  WaitFrames(1);
  ObjectEvent_SetSpriteColorById(LOCALID_WISE_ONE, SPRITE_COLOR_NORMAL);
}

/// Elemental Star Room, collapsing: left behind, Isaac and Garet face the Wise
/// One, which makes the Mars Star glow, warns them of Alchemy's threat and
/// sends them out as the volcano erupts. Isaac keeps the Star. Called by
/// SolSanctumEruption_PlayHostageEvent. Sets FLAG_SOL_SANCTUM_ERUPTED and
/// FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED and takes
/// SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, to the rooms of Sol and
/// Luna (SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION).
static void SolSanctumEruption_PlayWiseOneRescue(void) {
  struct WeatherWork *sparkles;
  struct ObjectEvent *wiseOne;
  struct ObjectEvent *center;
  struct ObjectEvent *leader;
  struct ObjectEvent *starOrigin;
  struct ObjectEvent *garet;
  struct Sprite *leaderSprite;
  struct Sprite *garetSprite;
  struct ObjectEvent *star;
  struct Sprite *starSprite;
  u8 *iconPixels;
  u8 crystalIndex;
  u32 frame;
  s32 flickerDelay;

  sparkles = gWeatherWork;
  wiseOne = ObjectEvent_Get(LOCALID_WISE_ONE);
  // Clear the ember object events and bring Isaac and Garet back into view.
  Task_Remove(SolSanctumEruption_UpdateEmberFrequency);
  gEmberMask = 3;
  FieldScript_WaitFrames(80);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  FieldScript_WaitFrames(40);
  Task_Remove(SolSanctumEruption_StepEmbers);
  for (crystalIndex = 0; crystalIndex < CRYSTAL_COUNT; crystalIndex++) {
    ObjectEvent_SetPosition(LOCALID_FIRST_CRYSTAL + crystalIndex, 0, 0);
  }
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, 0, 0);
  SolSanctumEruption_PlaceBeforeStar(LOCALID_PARTY_LEADER);
  SolSanctumEruption_PlaceBeforeStar(LOCALID_GARET);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(80);
  SolSanctumEruption_RestoreCornerCellAttributes();
  center = FieldCamera_GetScriptObject();
  center->motionFlags = 0;
  center->x.fixed = PIXELS(231);
  center->z.fixed = PIXELS(144);
  center->targetX = OBJ_EVENT_NO_TARGET;
  center->targetY = OBJ_EVENT_NO_TARGET;
  center->targetZ = OBJ_EVENT_NO_TARGET;
  center->y.fixed = 0;
  center->velocityX = 0;
  center->velocityZ = 0;
  WaitFrames(4);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(4);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(LOCALID_GARET, 3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  FieldScript_WaitFrames(40);

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  garet = ObjectEvent_Get(LOCALID_GARET);
  leaderSprite = leader->sprite;
  garetSprite = garet->sprite;
  for (frame = 0; frame < 20; frame++) {
    leaderSprite->rotation += 0x100;
    garetSprite->rotation -= 0x100;
    leader->x.fixed += PIXELS(3) / 8;
    garet->x.fixed -= PIXELS(3) / 8;
    WaitFrames(1);
  }
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->sprite->rotation = 0;
  ObjectEvent_Get(LOCALID_GARET)->sprite->rotation = 0;
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 246, 150);
  ObjectMotion_SetPositionAndCommit(LOCALID_GARET, 220, 150);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetActionVariant(LOCALID_GARET, 2);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_Get(LOCALID_GARET)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
  FieldScript_WaitFrames(60);

  // Garet: "Well, this turned out badly, wouldn't you say?" Yes: "It's a
  // miracle we survived." No: "How can you be so calm?"
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_SetMessageId(MsgSolSanctumWellTurnedBadly);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_SetMessageId(MsgSolSanctumWellTurnedBadly + 1);
  } else {
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_SetMessageId(MsgSolSanctumWellTurnedBadly + 2);
  }
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 60);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(LOCALID_GARET, 2, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(40);
  // Garet: "Jenna and Kraden are gone!"
  FieldScript_SetMessageId(MsgSolSanctumFriendsGone);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(80);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  // Garet: "You don't think they'll hurt Jenna and Kraden, do you?" Either
  // way, he trusts Felix to look after them.
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId(MsgSolSanctumThanksALot);
  } else {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId(MsgSolSanctumTheyllBeSafe);
  }
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 40);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(40);

  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  // Pan around the chamber before another tremor interrupts the conversation.
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(60);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  // Garet: "This really is terrible..." and they have a long way to go.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_SetMessageId(MsgSolSanctumThisIsTerrible);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);

  // A violent tremor throws them aside.
  Script_PlaySong(MUS_DANGER);
  SolSanctumEruption_SetMaterializationTint(1, 4, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  FieldScript_WaitFrames(10);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 243, 144);
  ObjectEvent_SetDestination(LOCALID_GARET, 202, 144);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  FieldScript_WaitFrames(20);
  sparkles->stopped = 1;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(80);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkTo(LOCALID_GARET, 220, 150);
  ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 246, 150);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  // Garet: "We're in big trouble..." Yes: "Let's get out of here..." No:
  // "Quit acting so tough!"
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  } else {
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId(MsgSolSanctumQuitActingTough);
  }
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);

  // The Wise One drifts in from the west, unseen by Isaac; Garet walks off
  // and calls him: "I'm over here. Come on, we've gotta go."
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(110), PIXELS(152));
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 171, 152);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_WISE_ONE), 1);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_GARET, 217, 182);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(4);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 231, 175);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  FieldScript_SetMessageId(MsgSolSanctumOverHere);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);

  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldScript_WaitFrames(120);
  // The Wise One: "Why are you standing there?" and "This place has become
  // dangerous..." A jolt; Garet: "You're the dangerous one!"
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(100);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  SolSanctumEruption_SetMaterializationTint(1, 10, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectMotion_Launch(LOCALID_GARET, 4, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 256, 150);
  ObjectEvent_SetDestination(LOCALID_GARET, 231, 180);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_WISE_ONE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);

  // The floor by the Wise One starts to break up; the Wise One glows and
  // holds it together with Psynergy.
  Script_PlaySong(SE_TREMOR);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_WISE_ONE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 130, 113);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WISE_ONE);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTHEAST + FACING_STEP,
                            0);
  FieldScript_WaitFrames(20);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(20);
  Scene_StartEventPsynergy();
  ObjectMotion_SetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Task_Remove(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  flickerDelay = 16;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    WaitFrames(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    WaitFrames(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  SolSanctumEruption_FinishMaterialization();
  FieldScript_WaitFrames(60);
  // The Wise One: "The volcano will soon erupt... I cannot hold it long."
  // and, after drifting closer, "You must leave this place."
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 169, 151);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WISE_ONE);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_NORTHEAST + FACING_STEP,
                            0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
  FieldCamera_WaitForArrival();
  // Lights rise at the chamber's first corner. The Wise One: the Elemental
  // Djinn have returned; "Take out the Elemental Star..." Refusing, "Isaac
  // cannot resist."
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_NORTHWEST);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WISE_ONE, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(40);
  Script_OpenMessage(LOCALID_WISE_ONE, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(40);
  } else {
    FieldMessage_Show(MsgSolSanctumCannotResist, 1);
    FieldScript_WaitFrames(40);
  }

  starOrigin = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  // Lift the Mars Star icon and pulse its scale during the Wise One's effect.
  star =
      ObjectEvent_Create(OBJECT_ELEMENTAL_STAR, starOrigin->x.fixed,
                         starOrigin->y.fixed + PIXELS(36), starOrigin->z.fixed);
  if (star != NULL) {
    iconPixels = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    starSprite = star->sprite;
    starSprite->flags = 0;
    starSprite->count = 0;
    starSprite->image.fullColor = 0;
    starSprite->image.palette = 0;
    starSprite->image.priority = 1;
    Icon_DrawItem(ITEM_MARS_STAR);
    VramCache_Load(starSprite->slot, 128, iconPixels + 0x400);
    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIM_LEADER_ARMS_RAISED);
    ItemDisplay_ShowPicture(star, 3);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIM_LEADER_ARMS_RAISED);
  }
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  for (frame = 0; frame < 24; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
    star->scaleX = 2 * Q16_ONE / 5;
    star->scaleY = 2 * Q16_ONE / 5;
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
    star->scaleX = Q16_ONE;
    star->scaleY = Q16_ONE;
  }
  ObjectEvent_SetSpriteColorById(LOCALID_WISE_ONE, SPRITE_COLOR_NORMAL);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  // The Wise One: "Return it to its bag." and the Elemental Stars are moving
  // away from here.
  FieldScript_SetMessageId(MsgSolSanctumReturnStarToBag);
  Script_ShowMessageAndWait(LOCALID_WISE_ONE, 0, 20);
  if (star != NULL) {
    ObjectEvent_Release(star);
  }
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 60);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(40);

  // The camera visits the other three corners as lights rise there. The Wise
  // One: the magma flows freely, there will be no chamber for the Stars to
  // return to, and the world is exposed to Alchemy. Garet: "Alchemy? A
  // threat?"
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  SolSanctumEruption_FadeCameraTo(232, 464);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_SOUTHWEST);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 144);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_NORTHEAST);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 464);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_SOUTHEAST);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST + FACING_STEP, 0);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(582), PIXELS(345));
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(231), PIXELS(180));
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  WaitFrames(20);
  // Back with the party, the Wise One: misused, Alchemy is dangerous; its
  // power is released if the Stars light the four lighthouses.
  SolSanctumEruption_FadeCameraTo(219, 171);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(40);

  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(20);
  // The floor breaks up again in two places; the Wise One floats to each and
  // holds it together.
  Script_PlaySong(SE_TREMOR);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 245, 145);
  ObjectEvent_SetDestination(LOCALID_GARET, 215, 168);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(80);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 184, 87);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WISE_ONE);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Task_Remove(SolSanctumEruption_FlickerSecondCollapseTiles);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawSecondCollapseTiles(0);
    WaitFrames(flickerDelay);
    SolSanctumEruption_DrawSecondCollapseTiles(1);
    WaitFrames(flickerDelay);
  }
  SolSanctumEruption_DrawSecondCollapseTiles(0);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  SolSanctumEruption_FinishMaterialization();
  FieldScript_WaitFrames(60);

  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_TREMOR);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetPositionAndCommit(LOCALID_WISE_ONE, 127, 110);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(30);
  Task_Remove(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    WaitFrames(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    WaitFrames(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_PlaySong(SE_TREMOR);
  Script_PlaySong(MUS_ERUPTION);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_TREMOR);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_MAP);
  SolSanctumEruption_FinishMaterialization();
  FieldScript_WaitFrames(60);
  ObjectMotion_SetPositionAndCommit(LOCALID_WISE_ONE, 184, 87);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  SolSanctumEruption_FinishMaterialization();
  Script_PlaySong(SE_RUMBLE);
  FieldScript_WaitFrames(100);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  // The Wise One: "It's erupting... Flee, now!" Garet: "But how..." The Wise
  // One: "I will help you..." It glows and the chamber fades to white.
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTHEAST + FACING_STEP,
                            0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    WaitFrames(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_OBJECTS);
  // Fade the chamber away, stop its effects and leave through the rescue warp.
  PaletteFade_StartInterpolation(60);
  WaitFrames(100);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  Task_Remove(SolSanctumEruption_UpdateWiseOnePalette);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Scene_EndEventPsynergy();
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  Import_ScriptRequestWarp(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS);
  // Flag 0x100 marks nothing: it is set nowhere else and read nowhere, by no
  // table, script or function, and render probe flag-reads, watching every
  // flag test and every byte and nibble read in the maps after the
  // eruption, sees none.
  GameFlag_Set(0x100);
}

/// Places an object event before the star, facing south, and draws it above the
/// chamber floor.
static void SolSanctumEruption_PlaceBeforeStar(s32 localId) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(localId);
  Import_ObjectEventSetPosition(localId, PIXELS(232), PIXELS(125));
  objectEvent->facing = FACING_SOUTH;
  ObjectMotion_SetActionVariant(localId, 3);
}

/// Selects the first collapsing tile patch and redraws the map.
static void SolSanctumEruption_DrawFirstCollapseTiles(s32 alternate) {
  if (alternate != 0) {
    s32 cellSize;
    cellSize = 1;
    MapGrid_CopyCellsTo(8, 47, 64, 7, cellSize, cellSize);
    MapGrid_CopyCellsTo(7, 48, 63, 8, 2, cellSize);
    MapGrid_CopyCellsTo(7, 49, 63, 9, 2, cellSize);
  } else {
    s32 cellSize;
    cellSize = 1;
    MapGrid_CopyCellsTo(56, 0, 64, 7, cellSize, cellSize);
    MapGrid_CopyCellsTo(56, 0, 63, 8, cellSize, cellSize);
    MapGrid_CopyCellsTo(56, 0, 63, 9, 2, cellSize);
    MapGrid_CopyCellsTo(58, 25, 64, 8, cellSize, cellSize);
  }
  MapGrid_CenterOnFollowedPoint();
}

/// Selects the second collapsing tile patch and redraws the map.
static void SolSanctumEruption_DrawSecondCollapseTiles(s32 alternate) {
  if (alternate != 0) {
    s32 patchSize;
    patchSize = 2;
    MapGrid_CopyCellsTo(9, 45, 65, 5, patchSize, patchSize);
    MapGrid_CopyCellsTo(11, 46, 67, 6, 1, patchSize);
  } else {
    s32 patchSize;
    patchSize = 2;
    MapGrid_CopyCellsTo(89, 2, 65, 5, patchSize, patchSize);
    MapGrid_CopyCellsTo(102, 32, 67, 6, 1, patchSize);
  }
  MapGrid_CenterOnFollowedPoint();
}

/// Randomly alternates the first tile patch on even frames. Run each frame as
/// a callback while the floor breaks up, until the Wise One holds it.
static void SolSanctumEruption_FlickerFirstCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Import_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawFirstCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawFirstCollapseTiles(0);
    }
  }
}

/// Randomly alternates the second tile patch on even frames, as the first.
static void SolSanctumEruption_FlickerSecondCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Import_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawSecondCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawSecondCollapseTiles(0);
    }
  }
}

/// Stages and raises ten arc effects at one of the four chamber positions.
static void SolSanctumEruption_RaiseElementalArcs(u32 corner) {
  void ObjectEvent_ShowEmote(s32, s32, s32);
  void PaletteFade_ApplyTarget(s32, s32);
  void Script_PlaySong(s32);

  u32 arcIndex, clear;
  s32 x, y, z;
  s32 *origin;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;

  for (arcIndex = 0; arcIndex <= 15; arcIndex++)
    ObjectEvent_DestroyById(arcIndex + LOCALID_FIRST_CRYSTAL);
  switch (corner) {
  case CHAMBER_NORTHWEST:
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 14), PALETTES_BG);
    break;
  case CHAMBER_SOUTHWEST:
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 18), PALETTES_BG);
    break;
  case CHAMBER_NORTHEAST:
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(14, 18, 18), PALETTES_BG);
    break;
  case CHAMBER_SOUTHEAST:
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 18, 14), PALETTES_BG);
    break;
  }
  PaletteFade_StartInterpolation(60);
  Script_PlaySong(SE_RISING_CHIMES);
  arcIndex = 0;
  clear = arcIndex;
  for (origin = &gEruption_ArcOrigins[0]; arcIndex <= 9;
       arcIndex++, origin += 2) {
    x = origin[0];
    y = origin[1];
    z = 0;
    switch (corner) {
    case CHAMBER_NORTHWEST:
      x += PIXELS(232);
      z = PIXELS(144);
      break;
    case CHAMBER_SOUTHWEST:
      x += PIXELS(232);
      z = PIXELS(464);
      break;
    case CHAMBER_NORTHEAST:
      x += PIXELS(711);
      z = PIXELS(144);
      break;
    case CHAMBER_SOUTHEAST:
      x += PIXELS(711);
      z = PIXELS(464);
      break;
    }
    gArcEffectTimers[arcIndex] = clear;
    objectEvent = ObjectEvent_Create(SPRITE_PARTICLE, x, y, z);
    gArcEffects[arcIndex] = objectEvent;
    objectEvent->motionFlags = clear;
    sprite = objectEvent->sprite;
    sprite->flags = clear;
    sprite->image.priority = 1;
    Object_SelectAnimation(objectEvent, 6);
    WaitFrames(6);
  }
  if (corner == CHAMBER_NORTHWEST) {
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  }
  WaitFrames(20);
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateElementalArcs,
                             TASK_PRIORITY_MAP);
  Script_PlaySong(SE_GLITTER);
  gArcEffectTimers[0] = 1;
  WaitFrames(6);
  gArcEffectTimers[1] = 1;
  WaitFrames(6);
  gArcEffectTimers[2] = 1;
  WaitFrames(6);
  gArcEffectTimers[3] = 1;
  WaitFrames(6);
  gArcEffectTimers[4] = 1;
  WaitFrames(6);
  gArcEffectTimers[5] = 1;
  WaitFrames(6);
  gArcEffectTimers[6] = 1;
  WaitFrames(6);
  gArcEffectTimers[7] = 1;
  WaitFrames(6);
  gArcEffectTimers[8] = 1;
  WaitFrames(6);
  gArcEffectTimers[9] = 1;
  WaitFrames(6);
  for (;;) {
    for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
      if (gArcEffectTimers[arcIndex] != 0) {
        arcIndex = ARC_SCAN_STILL_ACTIVE;
        break;
      }
    }
    if (arcIndex != ARC_SCAN_STILL_ACTIVE)
      break;
    WaitFrames(1);
  }
  WaitFrames(40);
  Task_Remove(SolSanctumEruption_UpdateElementalArcs);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(40);
}

/// Stretches each active arc for eight frames, then lifts it out of view. Run
/// each frame as a callback while SolSanctumEruption_RaiseElementalArcs runs.
static void SolSanctumEruption_UpdateElementalArcs(void) {

  u32 arcIndex;
  s32 age;
  struct ObjectEvent *objectEvent;

  for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
    age = gArcEffectTimers[arcIndex];
    if (age != 0) {
      objectEvent = gArcEffects[arcIndex];
      if ((u32)age <= 8) {
        objectEvent->scaleX += -ELEMENTAL_ARC_SCALE_X_DECREASE;
        objectEvent->scaleY += Q16_ONE / 2;
        objectEvent->y.fixed += PIXELS(3) / 10;
        objectEvent->targetY += PIXELS(3) / 10;
      } else {
        objectEvent->y.fixed += PIXELS(20);
        objectEvent->targetY += PIXELS(20);
      }
      age = gArcEffectTimers[arcIndex] + 1;
      gArcEffectTimers[arcIndex] = age;
      if ((u32)age > 14) {
        gArcEffectTimers[arcIndex] = 0;
      }
    }
  }
}

/// Fades out, moves the camera and its focus object event, then fades back in.
static void SolSanctumEruption_FadeCameraTo(s32 x, s32 z) {
  struct ObjectEvent *center;

  center = FieldCamera_GetScriptObject();
  z *= PIXELS(1);
  x *= PIXELS(1);
  FieldCamera_MoveWithinMapBounds(x, -1, z, 1);
  PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  center->x.fixed = x;
  center->z.fixed = z;
  center->targetX = OBJ_EVENT_NO_TARGET;
  center->targetZ = OBJ_EVENT_NO_TARGET;
  center->velocityX = 0;
  center->velocityZ = 0;
  WaitFrames(5);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(5);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  WaitFrames(30);
}

/// Restores eight corner-cell metatile behaviors from the chamber templates.
static void SolSanctumEruption_RestoreCornerCellAttributes(void) {

  MapGrid_CopyCellAttributes(14, 8, 1, 1, 10, 0);
  MapGrid_CopyCellAttributes(14, 28, 1, 1, 11, 0);
  MapGrid_CopyCellAttributes(44, 8, 1, 1, 12, 0);
  MapGrid_CopyCellAttributes(44, 28, 1, 1, 13, 0);
  MapGrid_CopyCellAttributes(13, 8, 1, 1, 14, 8);
  MapGrid_CopyCellAttributes(13, 28, 1, 1, 14, 28);
  MapGrid_CopyCellAttributes(43, 8, 1, 1, 44, 8);
  MapGrid_CopyCellAttributes(43, 28, 1, 1, 44, 28);
}

/// Fades the object palette into or out of the materialization tint.
static void SolSanctumEruption_SetMaterializationTint(s32 enabled, s32 frames,
                                                      s32 waitFrames) {
  if (enabled == 1) {
    Sound_PlaySong(SE_MATERIALIZE);
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(18, 18, 14), PALETTES_BG);
  } else {
    Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
  }
  PaletteFade_StartInterpolation(frames);
  if (waitFrames != 0) {
    FieldScript_WaitFrames(waitFrames);
  }
}

/// Flashes the palette and spawns paired vertical effects every sixteen frames.
static void SolSanctumEruption_FlashMaterializingObjectEvent(
    struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 2) != 0) {
    ObjectEvent_SetColorMode(objectEvent, 7);
  } else {
    ObjectEvent_SetColorMode(objectEvent, 0);
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(objectEvent);
  }
}

/// Cycles six palettes while spawning the object event's vertical effects.
static void
SolSanctumEruption_CycleObjectEventPalette(struct ObjectEvent *objectEvent) {
  if (gFrameCount & 1) {
    ObjectEvent_SetColorMode(objectEvent,
                             Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(objectEvent);
  }
}

#include "common/effect_vertical.h"

/// Creates two vertical effects sharing the parent position and cached tiles.
static void SolSanctumEruption_SpawnEffectPair(struct ObjectEvent *parent) {
  struct ObjectEvent *pair[2];
  struct ObjectEvent *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 effectIndex;

  Sound_PlaySong(SE_ERUPTION_COLUMNS);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = ObjectEvent_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                               parent->y.fixed, parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        Sprite_StartAnim(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->frame = 0;
      }
    }
  }
  pair[0]->update = FieldEffect_AnimateVerticalNegative;
  pair[0]->sprite->image.priority =
      ObjectEvent_GetById(LOCALID_WISE_ONE)->sprite->image.priority;
  {
    struct ObjectEvent *wiseOne = ObjectEvent_GetById(LOCALID_WISE_ONE);
    struct ObjectEvent *risingEffect = pair[1];

    risingEffect->sprite->image.priority = wiseOne->sprite->image.priority;
    risingEffect->update = FieldEffect_AnimateVerticalPositive;
    risingEffect->priorityFlags = 2;
  }
}

#include "common/effect_psynergy.h"

/// Cycles the Wise One's palette while it glows. Run each frame as a callback
/// that SolSanctumEruption_PlayWiseOneRescue adds and
/// SolSanctumEruption_FinishMaterialization removes.
static void SolSanctumEruption_UpdateWiseOnePalette(void) {
  SolSanctumEruption_CycleObjectEventPalette(
      ObjectEvent_GetById(LOCALID_WISE_ONE));
}

#include "common/sol_sanctum_elemental_star_ring_setup.h"

s32 gEruption_ArcOriginsHead[] = {
    SC_SET(SW_VELOCITY_Y, PIXELS(6)),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 2621),
    SC_ADD(SW_SCALE_Y, 2621),
    SC_WAIT(1),
    SC_LOOP(18, SC_TO(1)),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_END,
};

// clang-format off
s32 gEruption_ArcOrigins[] = {
    -PIXELS(6), 0,
    PIXELS(10), PIXELS(10),
    PIXELS(8), PIXELS(25),
    PIXELS(4), PIXELS(30),
    -PIXELS(5), PIXELS(20),
    PIXELS(2), PIXELS(5),
    -PIXELS(6), PIXELS(35),
    -PIXELS(8), PIXELS(15),
    PIXELS(2), PIXELS(40),
    -PIXELS(2), PIXELS(15),
};

struct MapWarpDest gSolSanctumEruption_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gSolSanctumEruption_Warps[] = {
    MAP_WARPS(SolSanctumEruption),
    MAP_WARP(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, SolSanctumLunaSolRooms, SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSolSanctumEruption_ObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_FELIX_MASKED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xfc, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gSolSanctumEruption_Events[] = {
    { MAP_EVENTS_END },
};

/// Unused: a copy of the Elemental Star room's cell animation
/// (gSolSanctumElementalStarRoom_StarCells), nine steps of a 3-by-4 picture;
/// nothing in the eruption plays it.
u16 gSolSanctumEruption_UnusedStarCells[] = {
    MAPGRID_ANIMATION_STEP(59, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(62, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(65, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(68, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(71, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(74, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(77, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(80, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(83, 40, 3, 4, 0), MAPGRID_ANIMATION_STEPS_END,
};

s32 gEruption_EmberScript[] = {
    SC_CALL(SolSanctumEruption_AlignEmberToHeading),
    SC_END,
};

u8 gSolSanctum_RingDrift[][3] = {
    { 4, 4, 4 },
    { 4, 0, 3 },
    { 4, 0, 4 },
    { 4, 0, 4 },
    { 3, 0, 4 },
    { 4, 6, 4 },
    { 6, 0, 4 },
    { 4, 0, 3 },
    { 2, 0, 1 },
    { 2, 0, 2 },
};

u8 gSolSanctum_RingSwing[][3] = {
    { 1, 1, 2 },
    { 1, 0, 2 },
    { 1, 0, 2 },
    { 1, 0, 1 },
    { 1, 0, 2 },
    { 1, 1, 2 },
    { 1, 0, 2 },
    { 1, 0, 2 },
    { 1, 0, 2 },
    { 1, 0, 2 },
};

s8 gSolSanctum_RingDirection[][3] = {
    { 1, 1, 1 },
    { 1, 0, 1 },
    { 1, 0, 1 },
    { -1, 0, 1 },
    { -1, 0, -1 },
    { 1, 1, 1 },
    { -1, 0, -1 },
    { -1, 0, -1 },
    { -1, 0, -1 },
    { -1, 0, -1 },
};

s32 gSolSanctumRing_MaxScales[] = {
    3 * Q16_ONE / 5, 4 * Q16_ONE / 5, 7 * Q16_ONE / 10, 3 * Q16_ONE / 5, 4 * Q16_ONE / 5, 3 * Q16_ONE / 5, 7 * Q16_ONE / 10, 3 * Q16_ONE / 5,
    3 * Q16_ONE / 5, 7 * Q16_ONE / 10,
};

s32 gSolSanctumRing_ScaleSteps[] = {
    655, 458, 655, 458, 655, 458, 655, 655,
    524, 655,
};
// clang-format on
