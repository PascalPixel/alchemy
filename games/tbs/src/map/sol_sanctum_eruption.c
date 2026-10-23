/// @file
/// Elemental Star Room (MapId_SolSanctumEruption), the chamber of
/// MapId_SolSanctumElementalStarRoom (sol_sanctum_elemental_star_room.c) as
/// it collapses once the Mars Star is taken: one scripted scene, with no events
/// of its own.
///
/// SolSanctumEruption_Initialize plays it on arrival at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, in two parts:
/// - SolSanctumEruption_PlayHostageScene: the chamber quakes, the Wise One
///   appears and stills it, and Saturos's group leaves with Jenna and Kraden,
///   rising out of the chamber one by one. Jenna leaves the party.
/// - SolSanctumEruption_PlayWiseOneRescue: the Wise One has Isaac keep the
///   Mars Star, then sends Isaac and Garet away in a white flash. It sets
///   FLAG_SOL_SANCTUM_ERUPTED, which every room of Sol Sanctum tests from then
///   on, and FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, and takes
///   SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS to the Luna and Sol
///   rooms (MapId_SolSanctumLunaSolRooms).
///
/// The effects are the scene's own. Sixteen blue crystals thrown out in a ring
/// (SolSanctumEruption_ThrowEruptionRing) become embers launched from a pool at
/// a varying rate (SolSanctumEruption_StepEmbers and
/// SolSanctumEruption_UpdateEmberFrequency, callbacks the first part adds and
/// the second removes); once Saturos's group starts to leave
/// (FLAG_MAP_TEMP_46), they fall silently. The breaking floor's cells flicker
/// by callback until the Wise One holds them, ten arcs of light rise at each of
/// the chamber's four corners as the Wise One tells of the stars
/// (SolSanctumEruption_RaiseElementalArcs), and the Wise One glows through a
/// cycle of palettes.
///
/// Saturos's group and their hostages leave one by one from the platform
/// where the party arrived (SolSanctumEruption_RaiseAndShrinkObjectEvent): each
/// walks onto it, rises 30 pixels at full size, then rises on as he shrinks
/// away, over a shadow that shrinks with him (LOCALID_PLATFORM_SHADOW;
/// https://alchemy.pascalpixel.com/places/sol-sanctum-eruption/departing-shadow.gif:
/// the departures, the platform's object event boxed on the screen, beside the
/// sprites and the sprites without it, where the shadow is gone).
///
/// Open questions: none.

#include "action_script.h"
#include "game_state.h"
#include "unit/map/sol_sanctum_eruption.h"
#include "unit/map/common/effect_psynergy.h"
#include "unit/map/common/sol_sanctum_elemental_star_ring_setup.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "b5_context.h"
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
#include "global_cells.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"
#include "fx_work.h"

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
// - FLAG_MAP_TEMP_46: set as Saturos's group starts to leave; from then on
//   the embers fall without their sound (SE_246).

/// The four corners of the chamber where SolSanctumEruption_RaiseElementalArcs
/// raises lights, in the order the Wise One's scene visits them.
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
/// LOCALID_FIRST_CRYSTAL), all placed off the map until the cutscene moves
/// them.
const struct ObjectEventTemplate *
SolSanctumEruption_GetObjectEventTemplates(void) {
  return gSolSanctumEruption_ObjectEventTemplates;
}

/// Returns the empty event table: the map is one cutscene.
const struct MapEvent *SolSanctumEruption_GetEvents(void) {
  return gSolSanctumEruption_Events;
}

/// Stages Saturos, Menardi, Alex and Felix around Jenna and Kraden.
static inline void SolSanctumEruption_PlaceHostageGroup(void) {
  ObjectEvent_SetPosition(JENNA, PIXELS(475), PIXELS(332));
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
/// MapId_SolSanctumElementalStarRoom. Sets FLAG_MAP_TEMP_46, plays
/// SolSanctumEruption_PlayWiseOneRescue and then removes Jenna from the party.
static void SolSanctumEruption_PlayHostageScene(void) {
  void ObjectEvent_ShowEmote();
  void ColorBuffer_ApplyTarget();
  void Script_PlayCue();

  struct ObjectEvent *objectEvent;
  struct GroundParticleWork *sparkles;
#ifndef UBFIX
  struct GroundParticleWork **particleWorkSlot;
  struct EventWork **eventWorkSlot;
#endif

#ifdef UBFIX
  sparkles = gParticleWork;
#else
  particleWorkSlot = &gParticleWork;
  sparkles = *particleWorkSlot;
#endif
  Engine_ScriptStart();
  sparkles->stopped = 0;
  // Reopen the chamber during the first quake and stage the two groups.
  Script_PlayCue(SE_141);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(232), PIXELS(156));
  ObjectEvent_SetPosition(GARET, PIXELS(218), PIXELS(172));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  SolSanctumEruption_PlaceHostageGroup();
  Engine_ObjectEventSetSpritePriority(LOCALID_WISE_ONE, 1);
#ifndef UBFIX
  // UB: these are separate work-pointer globals. The native sequence reaches
  // the event slot through the particle slot; named globals add four bytes
  // and change this scene's register allocation in tbs-en.
  eventWorkSlot = (struct EventWork **)((u8 *)particleWorkSlot - 8);
#endif
  Camera_MoveTo(PIXELS(232), -1, PIXELS(156), 0);
  Engine_MapGridRedraw();
#ifdef UBFIX
  gEventWork->transitionFrames = 8;
#else
  (*eventWorkSlot)->transitionFrames = 8;
#endif
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_ScriptWait(4);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_ScriptWait(4);
  SolSanctumEruption_ThrowEruptionRing();
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_ScriptWait(16);
  Script_PlayCue(SE_144);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_ScriptWait(4);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_ScriptWait(4);
  Script_PlayCue(SE_144);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(48);
  Engine_ScriptWait(48);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ObjectEventJump(GARET, 6, 20);
  // Off screen, Menardi: "What happened?" Saturos: "Not again!" Jenna: the
  // water's gone. Alex: without the Elemental Stars the chamber is
  // collapsing. Kraden: is this the "terrible retribution"?
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 40);
  Engine_ScriptSetMessage(MsgSolSanctumJennaWhatHappened);
  Engine_ScriptShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  Engine_ScriptShowMessage(LOCALID_SATUROS, 0);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 30);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  // The Wise One, a floating rock, appears over the chamber and the quake
  // stops.
  objectEvent = ObjectEvent_GetById(LOCALID_WISE_ONE);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(325), PIXELS(302));
  objectEvent = ObjectEvent_GetById(LOCALID_WISE_ONE);
  objectEvent->motionFlags = 5;
  sparkles->stopped = 1;
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  Engine_ScriptWait(150);
  // Menardi: "It's over... But what was it?" Jenna: "What is that?!?"
  // Saturos: "No!" The camera pans to the Wise One.
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  Script_ShowMessage(JENNA, 0);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  SolSanctumEruption_PlaceHostageGroup();
  Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(328), -1, PIXELS(299), 1);
  Engine_CameraWaitForMove();
  Script_PlayCue(SE_MAP_TRANSITION);
  ColorBuffer_ApplyTarget(0x205294, 2);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(200);
  // Garet: "That rock... It's floating... Do you know what it is?" Yes:
  // "Yeah, I know it's a rock, stupid!" No: someone is lifting it with
  // Psynergy. Kraden: "Could that be... the Wise One?!?"
  Script_OpenMessage(SPEAKER_PORTRAIT_LEFT | GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptSetMessage(MsgSolSanctumIKnowItsARock);
  } else {
    Engine_ScriptSetMessage(MsgSolSanctumSomeoneIsLiftingIt);
  }
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 80);
  Engine_ScriptSetMessage(MsgSolSanctumKradenCouldThatBeThe);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  Script_PlayCue(SE_141);
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(80);
  sparkles->stopped = 1;
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  SolSanctumEruption_SetMaterializationTint(0, 20, 60);
  // Alex: "Such power... Incredible!" Garet: "!!!" Jenna: "Wow..."
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_NORTHWEST, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST + FACING_STEP, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Script_ShowMessageAndWait(JENNA, 0, 30);
  // Embers start to rain from the Wise One.
  SolSanctumEruption_ResetEmberEmission();
  Task_AddCallback(SolSanctumEruption_StepEmbers, TASK_PRIORITY_MAP);
  Task_AddCallback(SolSanctumEruption_UpdateEmberFrequency, TASK_PRIORITY_MAP);
  Engine_ScriptWait(240);
  // Saturos: "It must be the guardian of the Elemental Stars!" The camera
  // moves to his group, all facing west.
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 30);
  SolSanctumEruption_PlaceHostageGroup();
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(JENNA);
  Camera_MoveTo(objectEvent->x.part.pixel * PIXELS(1), -1,
                objectEvent->z.part.pixel * PIXELS(1), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FELIX, 2);
  Engine_ScriptWait(20);
  // Felix: they should escape while they can. Menardi: not without the Mars
  // Star. Alex sides with Felix, and Saturos agrees to leave it.
  Script_ShowMessage(LOCALID_FELIX, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MENARDI, 2);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_ALEX, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  Engine_ObjectEventFaceEachOther(LOCALID_SATUROS, LOCALID_MENARDI, 40);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  // Jenna: "What about Isaac and Garet? You can't just leave them here!"
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 40);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | JENNA, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 40);
  Engine_ObjectEventFaceEachOther(LOCALID_SATUROS, LOCALID_MENARDI, 40);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            10);
  // Saturos: "Forget about them." Menardi: they might survive. Saturos: "So
  // what do you suggest we do?" Alex: take Jenna, and they will bring the
  // Star to get her back.
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MENARDI, 2);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ALEX, ANIMATION_NOD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTH + FACING_STEP, 20);
  // Alex: "Regardless, Kraden must come with us."
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ALEX, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // Felix steps forward: Alex is breaking their promise. Alex: conditions
  // have changed. Felix will not let them hurt Jenna; Kraden offers himself
  // in her place.
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 471, 314);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_ALEX, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_KRADEN, 2);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  // Saturos: "you cannot help Jenna now." Menardi: if they leave her, she
  // will die; is that what Felix wants?
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MENARDI, 2);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FELIX, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceEachOther(LOCALID_SATUROS, LOCALID_MENARDI, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FELIX, 1);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_WEST, 0);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(30);
  // Alex: "Perhaps we ought to continue this elsewhere, Felix." Jenna:
  // "Felix..." Alex: "Please...can we go now?"
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ALEX, ANIMATION_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_ALEX, 0, 30);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FELIX, ANIMATION_NOD);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_ALEX, 2);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FELIX, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ObjectEventFaceEachOther(LOCALID_SATUROS, LOCALID_MENARDI, 20);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkTo(LOCALID_MENARDI, 475, 348);
  ObjectEvent_WalkTo(LOCALID_SATUROS, 491, 348);
  Engine_ObjectEventWaitForMove(LOCALID_MENARDI);
  Engine_ObjectEventWaitForMove(LOCALID_SATUROS);
  Engine_ObjectEventSetAnimation(LOCALID_MENARDI, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 10);
  // Menardi: "Let's make haste!" She shoves Jenna, who stumbles away.
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MENARDI, 2);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI, 0);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 338);
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->steeringFlags = 254 & objectEvent->steeringFlags;
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 348);
  Engine_ObjectEventRunRepeatedMotion(JENNA, 1);
  Engine_ObjectEventJump(JENNA, 4, 0);
  ObjectEvent_WalkToAndWait(JENNA, 459, 316);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  // Felix: "Hey, don't push my sister!" Menardi: "You owe us your life!"
  // Saturos: "Menardi!"
  ObjectEvent_ShowEmote(LOCALID_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FELIX, 3);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MENARDI, 1);
  Engine_ScriptWait(20);
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_MENARDI, 475, 332);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_MENARDI, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_SATUROS, 2);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTHEAST + FACING_STEP,
                            20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_SATUROS, ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FELIX, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_WalkTo(JENNA, 432, 316);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 422, 311);
  Engine_ObjectEventSetAnimation(JENNA, ANIMATION_STAND);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  // Felix leads Jenna aside. Saturos: "Master Kraden, you will be joining us,
  // of course!" All gather under the departure point.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SATUROS, 2);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_SATUROS, 0, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_KRADEN, 2);
  Engine_ScriptWait(20);
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
  ObjectEvent_FaceDirection(JENNA, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(472), -1, PIXELS(300), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  GameFlag_Set(FLAG_MAP_TEMP_46);
  // One by one they rise out of the chamber: Saturos, Kraden, Menardi, then
  // Jenna ("Isaac, Garet! Don't die!") and Felix, and last Alex, who regrets
  // leaving the Mars Star.
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_SATUROS, ANIMATION_NOD);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_SATUROS);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_KRADEN, 2);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_KRADEN);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MENARDI, ANIMATION_NOD);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_MENARDI);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 40);
  Engine_ObjectEventStartRepeatedMotion(JENNA, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTHEAST, 0);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(JENNA);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_FELIX);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHWEST + FACING_STEP, 40);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_ALEX, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 30);
  SolSanctumEruption_RaiseAndShrinkObjectEvent(LOCALID_ALEX);
  SolSanctumEruption_PlayWiseOneRescue();
  Party_RemoveOwnerRestored(JENNA);
}

/// Lifts one departing object event and shrinks it together with its shadow on
/// the platform (LOCALID_PLATFORM_SHADOW).
static void SolSanctumEruption_RaiseAndShrinkObjectEvent(s32 localId) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *shadow;
  struct ObjectEvent *spriteOwner;
  u8 frame;

  shadow = ObjectEvent_GetById(LOCALID_PLATFORM_SHADOW);
  shadow->scaleX = Q16_ONE;
  shadow->scaleY = Q16_ONE;
  Engine_ObjectEventWalkToAndWait(localId, 471, 290);
  ObjectEvent_FaceDirection(localId, FACING_NORTH, 0);
  Engine_ScriptWait(10);
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, PIXELS(471), PIXELS(290));
  objectEvent = ObjectEvent_GetById(localId);
  spriteOwner = ObjectEvent_GetById(localId);
  Engine_ObjectEventSetSpriteFlags(spriteOwner, 0);
  ObjectEvent_SetChildValue(localId, 0x100);
  objectEvent->motionFlags = 0;
  Script_PlayCue(SE_201);
  // Rise at full size, then shrink the object event and its shadow together.
  frame = 0;
  do {
    objectEvent->y.fixed += PIXELS(1) / 2;
    Engine_ScriptWait(1);
    frame++;
  } while (frame != 60);
  Script_PlayCue(SE_190);
  frame = 0;
  do {
    objectEvent->y.fixed += PIXELS(1) / 10;
    objectEvent->scaleX -= Q16_ONE / 100;
    objectEvent->scaleY -= Q16_ONE / 100;
    shadow->scaleX -= Q16_ONE / 100;
    shadow->scaleY -= Q16_ONE / 100;
    Engine_ScriptWait(1);
    frame++;
  } while (frame != 90);
  ObjectEvent_SetPosition(localId, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, 0, 0);
}

/// Unused: an empty function nothing calls.
UNUSED static void SolSanctumEruption_UnusedCallbackA(void) {}

/// Unused: a second empty function nothing calls.
UNUSED static void SolSanctumEruption_UnusedCallbackB(void) {}

/// Plays the collapse after the cut from the Elemental Star Room. It plays at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, the cut from the
/// Elemental Star Room (MapId_SolSanctumElementalStarRoom). Called by
/// Field_RunMap once the object events exist.
s32 SolSanctumEruption_Initialize(void) {
  if (gGameState.warpDest ==
      SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM) {
    FieldEffect_InitializeSparkles();
    SolSanctumEruption_PlayHostageScene();
  }
  return 0;
}

/// Keeps a flying ember sprite rotated a quarter-turn from its facing. Run by
/// each ember's object script, gEruption_EmberScript.
static s32
SolSanctumEruption_AlignEmberToHeading(struct ScriptInterpreter *interpreter) {
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
  struct AnimationObject *sprite;
  s32 offsetX;
  s32 offsetZ;

  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    objectEvent = ObjectEvent_GetById(crystalIndex);
    Engine_ObjectEventStop(crystalIndex);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    ObjectEvent_SetMode(objectEvent, 2);
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
  Engine_AudioPlayCue(SE_145);
  for (crystalIndex = 0; crystalIndex <= 15; crystalIndex++) {
    spun = ObjectEvent_GetById(crystalIndex + 16);
    sprite = spun->sprite;
    angle = crystalIndex << 12;
    sprite->rotation = angle - 0x4000;
    offsetX = Iwram_MulQ16(Engine_MathCos(angle), PIXELS(256));
    offsetZ = Iwram_MulQ16(Engine_MathSin(angle), PIXELS(256));
    Engine_ObjectSetPosition(spun, spun->x.fixed + offsetX, spun->y.fixed,
                             spun->z.fixed + offsetZ);
  }
  Engine_ScriptWait(20);
  Engine_ObjectEventWaitForMove(LOCALID_FIRST_CRYSTAL);
  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    objectEvent = ObjectEvent_GetById(crystalIndex);
    Engine_ObjectEventStop(crystalIndex);
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
/// each frame as a callback that SolSanctumEruption_PlayHostageScene adds and
/// SolSanctumEruption_PlayWiseOneRescue removes.
static void SolSanctumEruption_StepEmbers(void) {
  struct GroundParticleWork *sparkles;
  s32 shakePixels;
  s32 shakeTimer;
  u8 emberIndex;
  struct ObjectEvent *landedEmber;
  struct ObjectEvent *ember;
  s16 angle;
  s32 x;
  s32 z;

  sparkles = gParticleWork;
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
          ObjectEvent_SetMode(landedEmber, 3);
        if (gEmberState[emberIndex] == 19) {
          landedEmber->x.fixed = 0;
          landedEmber->y.fixed = 0;
          landedEmber->z.fixed = 0;
          SET_VELOCITY(landedEmber, 0, 0, 0);
          landedEmber->targetX = OBJ_EVENT_NO_TARGET;
          landedEmber->targetY = OBJ_EVENT_NO_TARGET;
          landedEmber->targetZ = OBJ_EVENT_NO_TARGET;
          ObjectGroup_SetChildValue(landedEmber, 15);
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
    angle = Math_RemainderUnsigned(Engine_RandomNext(), 0xffff);
    ember = ObjectEvent_GetById(emberIndex + 16);
    if (gEmberState[emberIndex] == 0) {
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_46) == 0)
        Engine_AudioPlayCue(SE_246);
      gEmberState[emberIndex] = 1;
      ember->motionFlags = 0;
      ember->speed = PIXELS(8);
      ember->acceleration = PIXELS(1);
      Engine_ObjectEventSetSpriteFlags(ember, 0);
      ember->sprite->image.priority = 1;
      ObjectEvent_SetMode(ember, 2);
      Engine_ObjectSetScript(ember, gEruption_EmberScript);
      ember->x.fixed =
          Iwram_MulQ16(Engine_MathCos((u16)angle),
                       (((u32)(Engine_RandomNext() << 8) >> 16) << 16) +
                           PIXELS(256)) +
          PIXELS(325);
      ember->y.fixed = 0;
      ember->z.fixed =
          Iwram_MulQ16(Engine_MathSin((u16)angle),
                       (((u32)(Engine_RandomNext() << 8) >> 16) << 16) +
                           PIXELS(256)) +
          PIXELS(302);
      x = Iwram_MulQ16(Engine_MathCos((u16)angle),
                       ((Engine_RandomNext() & 63) << 16) + PIXELS(8));
      z = Iwram_MulQ16(Engine_MathSin((u16)angle),
                       ((Engine_RandomNext() & 63) << 16) + PIXELS(8));
      Engine_ObjectSetPosition(ember, x + PIXELS(325), 0, z + PIXELS(286));
      ObjectGroup_SetChildValue(ember, 0);
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
    gEmberLevel = (u32)(Engine_RandomNext() << 2) >> 16;
  }
  emissionLevel = gEmberLevel;
  switch (emissionLevel) {
  case 3:
    gEmberMask = emissionLevel;
    gEmberLevelTimer = ((u32)(Engine_RandomNext() * 20) >> 16) + 40;
    break;
  case 2:
    gEmberMask = 15;
    gEmberLevelTimer = ((u32)(Engine_RandomNext() * 40) >> 16) + 80;
    break;
  case 1:
    gEmberMask = 63;
    gEmberLevelTimer = ((u32)(Engine_RandomNext() * 80) >> 16) + 160;
    break;
  default:
    gEmberMask = 127;
    gEmberLevelTimer = ((u32)(Engine_RandomNext() * 160) >> 16) + 320;
    break;
  }
}

/// Ends the Wise One's palette animation and clears its attached effect.
static inline void SolSanctumEruption_FinishMaterialization(void) {
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WISE_ONE, 3);
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateWiseOnePalette);
  Engine_TaskWait(1);
  ObjectEvent_SetChildValue(LOCALID_WISE_ONE, 0);
}

/// Elemental Star Room, collapsing: left behind, Isaac and Garet face the Wise
/// One, which makes the Mars Star glow, warns them of Alchemy's threat and
/// sends them out as the volcano erupts. Isaac keeps the Star. Called by
/// SolSanctumEruption_PlayHostageScene. Sets FLAG_SOL_SANCTUM_ERUPTED and
/// FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED and takes
/// SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, to the rooms of Sol and
/// Luna (SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION).
static void SolSanctumEruption_PlayWiseOneRescue(void) {
  struct GroundParticleWork *sparkles;
  struct ObjectEvent *wiseOne;
  struct ObjectEvent *center;
  struct ObjectEvent *leader;
  struct ObjectEvent *starOrigin;
  struct ObjectEvent *garet;
  struct AnimationObject *leaderSprite;
  struct AnimationObject *garetSprite;
  struct ObjectEvent *star;
  struct AnimationObject *starSprite;
  u8 *iconPixels;
  u8 crystalIndex;
  u32 frame;
  s32 flickerDelay;

  sparkles = gParticleWork;
  wiseOne = ObjectEvent_Get(LOCALID_WISE_ONE);
  // Clear the ember object events and bring Isaac and Garet back into view.
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateEmberFrequency);
  gEmberMask = 3;
  Engine_ScriptWait(80);
  Script_PlayCue(AUDIO_CUE_FADE_BGM);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(40);
  Engine_TaskRemoveCallback(SolSanctumEruption_StepEmbers);
  for (crystalIndex = 0; crystalIndex < CRYSTAL_COUNT; crystalIndex++) {
    ObjectEvent_SetPosition(LOCALID_FIRST_CRYSTAL + crystalIndex, 0, 0);
  }
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, 0, 0);
  SolSanctumEruption_PlaceBeforeStar(LOCALID_PARTY_LEADER);
  SolSanctumEruption_PlaceBeforeStar(GARET);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ScriptWait(80);
  SolSanctumEruption_RestoreCornerCellAttributes();
  center = Engine_ScriptGetViewCenter();
  center->motionFlags = 0;
  center->x.fixed = PIXELS(231);
  center->z.fixed = PIXELS(144);
  center->targetX = OBJ_EVENT_NO_TARGET;
  center->targetY = OBJ_EVENT_NO_TARGET;
  center->targetZ = OBJ_EVENT_NO_TARGET;
  center->y.fixed = 0;
  center->velocityX = 0;
  center->velocityZ = 0;
  Engine_TaskWait(4);
  Engine_MapGridRedraw();
  Engine_TaskWait(4);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 3);
  Engine_ObjectEventSetSpritePriority(GARET, 3);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(40);

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  garet = ObjectEvent_Get(GARET);
  leaderSprite = leader->sprite;
  garetSprite = garet->sprite;
  for (frame = 0; frame < 20; frame++) {
    leaderSprite->rotation += 0x100;
    garetSprite->rotation -= 0x100;
    leader->x.fixed += PIXELS(3) / 8;
    garet->x.fixed -= PIXELS(3) / 8;
    Engine_TaskWait(1);
  }
  Engine_ScriptWait(40);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->sprite->rotation = 0;
  ObjectEvent_Get(GARET)->sprite->rotation = 0;
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ObjectEventJump(GARET, 6, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 246, 150);
  Engine_ObjectMotionSetPositionAndCommit(GARET, 220, 150);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventSetSpritePriority(GARET, 2);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_Get(GARET)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  Engine_ObjectEventStop(LOCALID_PARTY_LEADER);
  Engine_ObjectEventStop(GARET);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Engine_ScriptWait(60);

  // Garet: "Well, this turned out badly, wouldn't you say?" Yes: "It's a
  // miracle we survived." No: "How can you be so calm?"
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptSetMessage(MsgSolSanctumWellTurnedBadly);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    Engine_ScriptSetMessage(MsgSolSanctumWellTurnedBadly + 1);
  } else {
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Engine_ScriptSetMessage(MsgSolSanctumWellTurnedBadly + 2);
  }
  Script_ShowMessageAndWait(GARET, 0, 60);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventJump(GARET, 2, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(40);
  // Garet: "Jenna and Kraden are gone!"
  Engine_ScriptSetMessage(MsgSolSanctumFriendsGone);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(80);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  // Garet: "You don't think they'll hurt Jenna and Kraden, do you?" Either
  // way, he trusts Felix to look after them.
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptWait(10);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    Engine_ScriptWait(10);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Engine_ScriptSetMessage(MsgSolSanctumThanksALot);
  } else {
    Engine_ScriptWait(10);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    Engine_ScriptWait(10);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Engine_ScriptSetMessage(MsgSolSanctumTheyllBeSafe);
  }
  Script_ShowMessageAndWait(GARET, 0, 40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(40);

  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  // Pan around the chamber before another tremor interrupts the conversation.
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  Camera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(60);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  // Garet: "This really is terrible..." and they have a long way to go.
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptSetMessage(MsgSolSanctumThisIsTerrible);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);

  // A violent tremor throws them aside.
  Script_PlayCue(MUS_023);
  SolSanctumEruption_SetMaterializationTint(1, 4, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Engine_ScriptWait(10);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ObjectEventJump(GARET, 6, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 243, 144);
  ObjectEvent_SetDestination(GARET, 202, 144);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ScriptWait(20);
  sparkles->stopped = 1;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ScriptWait(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(80);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkTo(GARET, 220, 150);
  Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, 246, 150);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  // Garet: "We're in big trouble..." Yes: "Let's get out of here..." No:
  // "Quit acting so tough!"
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  } else {
    ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Engine_ScriptSetMessage(MsgSolSanctumQuitActingTough);
  }
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);

  // The Wise One drifts in from the west, unseen by Isaac; Garet walks off
  // and calls him: "I'm over here. Come on, we've gotta go."
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(110), PIXELS(152));
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 171, 152);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST, 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_Get(LOCALID_WISE_ONE), 1);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(GARET, 217, 182);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(20);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(4);
  Engine_ObjectEventWalkToAndWait(GARET, 231, 175);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ScriptSetMessage(MsgSolSanctumOverHere);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);

  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_ScriptWait(40);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(40);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptWait(120);
  // The Wise One: "Why are you standing there?" and "This place has become
  // dangerous..." A jolt; Garet: "You're the dangerous one!"
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(100);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  SolSanctumEruption_SetMaterializationTint(1, 10, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ObjectEventJump(GARET, 4, 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 256, 150);
  ObjectEvent_SetDestination(GARET, 231, 180);
  Engine_ObjectEventWaitForMove(GARET);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  Engine_ScriptWait(20);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ScriptWait(40);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WISE_ONE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);

  // The floor by the Wise One starts to break up; the Wise One glows and
  // holds it together with Psynergy.
  Script_PlayCue(SE_107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_MAP);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_WISE_ONE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  Engine_ScriptWait(40);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 130, 113);
  Engine_ObjectEventWaitForMove(LOCALID_WISE_ONE);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTHEAST + FACING_STEP,
                            0);
  Engine_ScriptWait(20);
  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(20);
  Map_StartCutscenePsynergy();
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_ScriptWait(30);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  flickerDelay = 16;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  SolSanctumEruption_FinishMaterialization();
  Engine_ScriptWait(60);
  // The Wise One: "The volcano will soon erupt... I cannot hold it long."
  // and, after drifting closer, "You must leave this place."
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 169, 151);
  Engine_ObjectEventWaitForMove(LOCALID_WISE_ONE);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_EAST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_NORTHEAST + FACING_STEP,
                            0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
  Engine_CameraWaitForMove();
  // Lights rise at the chamber's first corner. The Wise One: the Elemental
  // Djinn have returned; "Take out the Elemental Star..." Refusing, "Isaac
  // cannot resist."
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_NORTHWEST);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WISE_ONE, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(40);
  Script_OpenMessage(LOCALID_WISE_ONE, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptWait(40);
  } else {
    Engine_MessageShowCentered(MsgSolSanctumCannotResist, 1);
    Engine_ScriptWait(40);
  }

  starOrigin = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  // Lift the Mars Star icon and pulse its scale during the Wise One's effect.
  star =
      ObjectEvent_Create(OBJECT_ELEMENTAL_STAR, starOrigin->x.fixed,
                         starOrigin->y.fixed + PIXELS(36), starOrigin->z.fixed);
  if (star != NULL) {
    iconPixels = Engine_HeapAllocate(17, 0x608);
    starSprite = star->sprite;
    starSprite->flags = 0;
    starSprite->count = 0;
    starSprite->image.fullColor = 0;
    starSprite->image.palette = 0;
    starSprite->image.priority = 1;
    Engine_ItemLoadIcon(ITEM_MARS_STAR);
    Engine_VramLoad(starSprite->slot, 128, iconPixels + 0x400);
    Engine_HeapRelease(17);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_ARMS_RAISED);
    Engine_ItemShowPicture(star, 3);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_ARMS_RAISED);
  }
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  for (frame = 0; frame < 24; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
    star->scaleX = 2 * Q16_ONE / 5;
    star->scaleY = 2 * Q16_ONE / 5;
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
    star->scaleX = Q16_ONE;
    star->scaleY = Q16_ONE;
  }
  ObjectEvent_SetChildValue(LOCALID_WISE_ONE, 0);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  // The Wise One: "Return it to its bag." and the Elemental Stars are moving
  // away from here.
  Engine_ScriptSetMessage(MsgSolSanctumReturnStarToBag);
  Script_ShowMessageAndWait(LOCALID_WISE_ONE, 0, 20);
  if (star != NULL) {
    Engine_ObjectDispatchRelease(star);
  }
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 60);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(40);

  // The camera visits the other three corners as lights rise there. The Wise
  // One: the magma flows freely, there will be no chamber for the Stars to
  // return to, and the world is exposed to Alchemy. Garet: "Alchemy? A
  // threat?"
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
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
  ObjectEvent_SetPosition(GARET, PIXELS(582), PIXELS(345));
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_SetPosition(GARET, PIXELS(231), PIXELS(180));
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_TaskWait(20);
  // Back with the party, the Wise One: misused, Alchemy is dangerous; its
  // power is released if the Stars light the four lighthouses.
  SolSanctumEruption_FadeCameraTo(219, 171);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(40);

  sparkles->stopped = 0;
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(20);
  // The floor breaks up again in two places; the Wise One floats to each and
  // holds it together.
  Script_PlayCue(SE_107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_MAP);
  Engine_ScriptWait(20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ObjectEventJump(GARET, 6, 0);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 245, 145);
  ObjectEvent_SetDestination(GARET, 215, 168);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ScriptWait(80);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_WISE_ONE, 184, 87);
  Engine_ObjectEventWaitForMove(LOCALID_WISE_ONE);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_ScriptWait(30);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerSecondCollapseTiles);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawSecondCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawSecondCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawSecondCollapseTiles(0);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  SolSanctumEruption_FinishMaterialization();
  Engine_ScriptWait(60);

  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_MAP);
  Engine_ScriptWait(40);
  Engine_ObjectMotionSetPositionAndCommit(LOCALID_WISE_ONE, 127, 110);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_ScriptWait(30);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Script_PlayCue(SE_107);
  Script_PlayCue(MUS_063);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_MAP);
  SolSanctumEruption_FinishMaterialization();
  Engine_ScriptWait(60);
  Engine_ObjectMotionSetPositionAndCommit(LOCALID_WISE_ONE, 184, 87);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTH, 0);
  Engine_ScriptWait(10);
  SolSanctumEruption_FinishMaterialization();
  Script_PlayCue(SE_141);
  Engine_ScriptWait(100);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  // The Wise One: "It's erupting... Flee, now!" Garet: "But how..." The Wise
  // One: "I will help you..." It glows and the chamber fades to white.
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ObjectEventRunRepeatedMotion(GARET, 3);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  ObjectEvent_FaceDirection(LOCALID_WISE_ONE, FACING_SOUTHEAST + FACING_STEP,
                            0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingObjectEvent(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_MAP);
  Engine_ScriptWait(20);
  ColorBuffer_ApplyTarget(0x7fff, 2);
  // Fade the chamber away, stop its effects and leave through the rescue warp.
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(100);
  ColorBuffer_ApplyTarget(0x7fff, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateWiseOnePalette);
  sparkles->stopped = 1;
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapGridRenderWaitForValues();
  Map_EndCutscenePsynergy();
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  Engine_ScriptRequestWarp(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS);
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
  Engine_ObjectEventSetPosition(localId, PIXELS(232), PIXELS(125));
  objectEvent->facing = FACING_SOUTH;
  Engine_ObjectEventSetSpritePriority(localId, 3);
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
  Engine_MapGridRedraw();
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
  Engine_MapGridRedraw();
}

/// Randomly alternates the first tile patch on even frames. Run each frame as
/// a callback while the floor breaks up, until the Wise One holds it.
static void SolSanctumEruption_FlickerFirstCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawFirstCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawFirstCollapseTiles(0);
    }
  }
}

/// Randomly alternates the second tile patch on even frames, as the first.
static void SolSanctumEruption_FlickerSecondCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawSecondCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawSecondCollapseTiles(0);
    }
  }
}

/// Stages and raises ten arc effects at one of the four chamber positions.
static void SolSanctumEruption_RaiseElementalArcs(u32 corner) {
  void ObjectEvent_ShowEmote(s32, s32, s32);
  void ColorBuffer_ApplyTarget(s32, s32);
  void Script_PlayCue(s32);

  u32 arcIndex, clear;
  s32 x, y, z;
  s32 *origin;
  struct ObjectEvent *objectEvent;
  struct AnimationObject *sprite;

  for (arcIndex = 0; arcIndex <= 15; arcIndex++)
    Engine_ObjectEventDestroy(arcIndex + LOCALID_FIRST_CRYSTAL);
  switch (corner) {
  case CHAMBER_NORTHWEST:
    ColorBuffer_ApplyTarget(0x4039d2, 1);
    break;
  case CHAMBER_SOUTHWEST:
    ColorBuffer_ApplyTarget(0x4049d2, 1);
    break;
  case CHAMBER_NORTHEAST:
    ColorBuffer_ApplyTarget(0x404a4e, 1);
    break;
  case CHAMBER_SOUTHEAST:
    ColorBuffer_ApplyTarget(0x403a52, 1);
    break;
  }
  Engine_ColorBufferInterpolate(60);
  Script_PlayCue(SE_214);
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
    objectEvent = Engine_ObjectCreate(SPRITE_PARTICLE, x, y, z);
    gArcEffects[arcIndex] = objectEvent;
    objectEvent->motionFlags = clear;
    sprite = objectEvent->sprite;
    sprite->flags = clear;
    sprite->image.priority = 1;
    ObjectEvent_SetMode(objectEvent, 6);
    Engine_TaskWait(6);
  }
  if (corner == CHAMBER_NORTHWEST) {
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  }
  Engine_TaskWait(20);
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateElementalArcs,
                             TASK_PRIORITY_MAP);
  Script_PlayCue(SE_246);
  gArcEffectTimers[0] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[1] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[2] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[3] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[4] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[5] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[6] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[7] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[8] = 1;
  Engine_TaskWait(6);
  gArcEffectTimers[9] = 1;
  Engine_TaskWait(6);
  for (;;) {
    for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
      if (gArcEffectTimers[arcIndex] != 0) {
        arcIndex = ARC_SCAN_STILL_ACTIVE;
        break;
      }
    }
    if (arcIndex != ARC_SCAN_STILL_ACTIVE)
      break;
    Engine_TaskWait(1);
  }
  Engine_TaskWait(40);
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateElementalArcs);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(40);
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

  center = Engine_ScriptGetViewCenter();
  z *= PIXELS(1);
  x *= PIXELS(1);
  Engine_CameraMoveTo(x, -1, z, 1);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  center->x.fixed = x;
  center->z.fixed = z;
  center->targetX = OBJ_EVENT_NO_TARGET;
  center->targetZ = OBJ_EVENT_NO_TARGET;
  center->velocityX = 0;
  center->velocityZ = 0;
  Engine_TaskWait(5);
  Engine_MapGridRedraw();
  Engine_TaskWait(5);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(30);
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
    Engine_AudioPlayCue(SE_308);
    Engine_ColorBufferApplyTarget(0x203a52, 1);
  } else {
    Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
    Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  }
  Engine_ColorBufferInterpolate(frames);
  if (waitFrames != 0) {
    Engine_ScriptWait(waitFrames);
  }
}

/// Flashes the palette and spawns paired vertical effects every sixteen frames.
static void SolSanctumEruption_FlashMaterializingObjectEvent(
    struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(objectEvent, 7);
  } else {
    Engine_ObjectSetPartPalettes(objectEvent, 0);
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(objectEvent);
  }
}

/// Cycles six palettes while spawning the object event's vertical effects.
static void
SolSanctumEruption_CycleObjectEventPalette(struct ObjectEvent *objectEvent) {
  if (gFrameCount & 1) {
    Engine_ObjectSetPartPalettes(objectEvent,
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
  struct AnimationObject *sprite;
  struct FieldEffectWork *effectWork = gEffectWork;
  s32 effectIndex;

  Engine_AudioPlayCue(SE_292);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = Engine_ObjectCreate(26, parent->x.fixed, parent->y.fixed,
                                parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->linkedObject = parent;
      if (sprite != NULL) {
        AnimationObject_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
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
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FELIX_MASKED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xfc, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
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
