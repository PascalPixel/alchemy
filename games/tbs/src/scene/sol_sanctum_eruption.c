/// @file
/// Elemental Star Room (SceneId_SolSanctumEruption), the chamber of
/// SceneId_SolSanctumElementalStarRoom (sol_sanctum_elemental_star_room.c) as
/// it collapses once the Mars Star is taken.
///
/// The scene is one event, with no triggers of its own.
/// Scene_Initialize plays it on arrival at
/// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, in two parts:
/// - SolSanctumEruption_PlayHostageEvent: the chamber quakes, and the Wise One
///   appears and stills it. Saturos's group leaves with Jenna and Kraden,
///   rising out of the chamber one by one. Jenna leaves the party.
/// - SolSanctumEruption_PlayWiseOneRescue: the Wise One has Isaac keep the Mars
///   Star, then sends Isaac and Garet away in a white flash. It sets
///   FLAG_SOL_SANCTUM_ERUPTED, which every room of Sol Sanctum tests from then
///   on, and FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED. Then it takes
///   SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS to the Luna and Sol
///   rooms (SceneId_SolSanctumLunaSolRooms).
///
/// The effects are the event's own.
///
/// Sixteen blue crystals are thrown out in a ring
/// (SolSanctumEruption_ThrowEruptionRing). They become embers launched from a
/// pool at a varying rate. Two tasks do that, SolSanctumEruption_StepEmbers and
/// SolSanctumEruption_UpdateEmberFrequency, which the first part adds and the
/// second removes. Once Saturos's group starts to leave
/// (FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING), the embers fall silently.
///
/// The breaking floor's cells flicker by task until the Wise One holds them.
/// Ten arcs of light rise at each of the chamber's four corners as the Wise One
/// tells of the stars (SolSanctumEruption_RaiseElementalArcs). The Wise One
/// glows through a cycle of palettes.
///
/// Saturos's group and their hostages leave one by one from the platform where
/// the party arrived (SolSanctumEruption_RaiseAndShrinkActor). Each walks onto
/// it and rises 30 pixels at full size. Then he rises on as he shrinks away,
/// over a shadow that shrinks with him. The platform's actor,
/// ACTOR_PLATFORM_SHADOW, draws that shadow.

#include "script_operands.h"
#include "main_routine_work.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gSolSanctumEruption_Entrances[];
extern u32 gSolSanctumEruption_Exits[];
extern struct ActorPlacement gSolSanctumEruption_ActorPlacements[];
extern struct Trigger gSolSanctumEruption_Triggers[];
extern u16 gSolSanctumEruption_UnusedStarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctum_RingMaxScales[];
extern s32 gSolSanctum_RingScaleSteps[];

#include "module/scene/shared/event_psynergy.h"
#include "module/scene/shared/sol_sanctum_elemental_star_ring_setup.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"

enum CollapseActorId {
  /// The shadow on the platform under each who leaves with Saturos: a
  /// blank actor, whose sprite draws only its shadow, shrunk with
  /// the departing actor, whose own shadow is turned off
  /// (SolSanctumEruption_RaiseAndShrinkActor).
  ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  /// Felix in his mask, before the star room unmasks him.
  ACTOR_FELIX_MASKED,
  ACTOR_FELIX,
  ACTOR_ALEX
};

#include "ground_mask.h"

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern struct Actor *gArcEffects[];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#include "event_inline.h"
#include "iwram_call.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "heap_slots.h"
#include "vram_cache.h"
#include "macros.h"
#include "flags.h"
#include "io_reg.h"
#include "sound_driver_symbols.h"
#include "item_ids.h"
#include "field_psynergy_work.h"

/// Saturos's group has started to leave, and the embers fall without their
/// sound.
#define FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING FLAG_SCENE_TEMP_46

static void SolSanctumEruption_RaiseAndShrinkActor(s32 actorId);
static void SolSanctumEruption_ThrowEruptionRing(void);
static void SolSanctumEruption_ResetEmberEmission(void);
static void SolSanctumEruption_StepEmbers(void);
static void SolSanctumEruption_UpdateEmberFrequency(void);
static void SolSanctumEruption_PlayWiseOneRescue(void);
static void SolSanctumEruption_PlaceBeforeStar(s32 actorId);
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
static void SolSanctumEruption_FlashMaterializingActor(struct Actor *actor);
static void SolSanctumEruption_SpawnEffectPair(struct Actor *parent);
static void SolSanctumEruption_UpdateWiseOnePalette(void);

// Scene flags, cleared on every ordinary arrival:
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
  ACTOR_WISE_ONE = ACTOR_ALEX + 1,
  /// The sixteen crystals thrown out in a ring.
  ACTOR_FIRST_CRYSTAL = ACTOR_WISE_ONE + 1,
  CRYSTAL_COUNT = 16
};

/// The star the Wise One has the leader take out, lifted above him.
enum { ACTOR_ELEMENTAL_STAR = 22 };

#define ELEMENTAL_ARC_SCALE_X_DECREASE 7372
#define EMBER_EMISSION_DISABLED 999
#define ARC_SCAN_STILL_ACTIVE 888

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumEruption_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumEruption_Exits; }

/// Returns everyone the event moves, all placed off the map until it does.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gSolSanctumEruption_ActorPlacements;
}

/// Returns the empty trigger table, because the scene is one event.
const struct Trigger *Scene_GetTriggers(void) {
  return gSolSanctumEruption_Triggers;
}

/// Stages Saturos, Menardi, Alex and Felix around Jenna and Kraden.
static inline void SolSanctumEruption_PlaceHostageGroup(void) {
  Actor_SetPosition(ACTOR_JENNA, 475, 332);
  Actor_SetPosition(ACTOR_KRADEN, 491, 332);
  Actor_SetPosition(ACTOR_MENARDI, 459, 348);
  Actor_SetPosition(ACTOR_SATUROS, 507, 348);
  Actor_SetPosition(ACTOR_FELIX, 471, 306);
  Actor_SetPosition(ACTOR_ALEX, 479, 362);
}

/// Elemental Star Room, collapsing: Saturos's group leaves with its hostages.
static void SolSanctumEruption_PlayHostageEvent(void) {
  struct Actor *actor;
  struct WeatherWork *sparkles;

  sparkles = HEAP_SLOT_BLOCK(HEAP_SLOT_WEATHER);
  Event_Start();
  sparkles->stopped = 0;
  // Reopen the chamber during the first quake and stage the two groups.
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 232, 156);
  Actor_SetPosition(ACTOR_GARET, 218, 172);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  SolSanctumEruption_PlaceHostageGroup();
  Actor_SetSpritePriority(ACTOR_WISE_ONE, 1);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 156, 0);
  MapGrid_CenterOnFollowedPoint();
  gFieldWork->transitionFrames = 8;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  Event_Wait(4);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  Event_Wait(4);
  SolSanctumEruption_ThrowEruptionRing();
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  Event_Wait(16);
  Sound_Play(SE_CLATTER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  Event_Wait(4);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(4);
  Event_Wait(4);
  Sound_Play(SE_CLATTER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(48);
  Event_Wait(48);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 20);
  // Off screen, Menardi: "What happened?" Saturos: "Not again!" Jenna: the
  // water's gone. Alex: without the Elemental Stars the chamber is
  // collapsing. Kraden: is this the "terrible retribution"?
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 40);
  Event_SetMessageId(MsgSolSanctumJennaWhatHappened);
  // "What's the matter? What happened?"
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  // "No! Not again!"
  Event_ShowMessage(ACTOR_SATUROS, 0);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 30);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  // The Wise One, a floating rock, appears over the chamber and the quake
  // stops.
  actor = Actor_Get(ACTOR_WISE_ONE);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPosition(ACTOR_WISE_ONE, 325, 302);
  actor = Actor_Get(ACTOR_WISE_ONE);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
  sparkles->stopped = 1;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(150);
  // Menardi: "It's over... But what was it?" Jenna: "What is that?!?"
  // Saturos: "No!" The camera pans to the Wise One.
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 10);
  SolSanctumEruption_PlaceHostageGroup();
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 299, 1);
  FieldCamera_WaitForMove();
  Sound_Play(SE_MAP_TRANSITION);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 20, 20),
                          PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(20);
  wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(20);
  Event_Wait(200);
  // Garet: "That rock... It's floating... Do you know what it is?" Yes:
  // "Yeah, I know it's a rock, stupid!" No: someone is lifting it with
  // Psynergy. Kraden: "Could that be... the Wise One?!?"
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId(MsgSolSanctumIKnowItsARock);
  } else {
    Event_SetMessageId(MsgSolSanctumSomeoneIsLiftingIt);
  }
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 80);
  Event_SetMessageId(MsgSolSanctumKradenCouldThatBeThe);
  // "Could that be... the Wise One?!?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(80);
  sparkles->stopped = 1;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  SolSanctumEruption_SetMaterializationTint(0, 20, 60);
  // Alex: "Such power... Incredible!" Garet: "!!!" Jenna: "Wow..."
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 30);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_NORTHWEST, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 30);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 30);
  // Embers start to rain from the Wise One.
  SolSanctumEruption_ResetEmberEmission();
  Task_AddFrame(SolSanctumEruption_StepEmbers, TASK_PASS_FRAME);
  Task_AddFrame(SolSanctumEruption_UpdateEmberFrequency, TASK_PASS_FRAME);
  Event_Wait(240);
  // Saturos: "It must be the guardian of the Elemental Stars!" The camera
  // moves to his group, all facing west.
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 30);
  SolSanctumEruption_PlaceHostageGroup();
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  actor = Actor_Get(ACTOR_JENNA);
  FieldCamera_MoveToFixed(px(actor->x.part.pixel), CAMERA_KEEP,
                          px(actor->z.part.pixel), 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_Wait(20);
  // Felix: they should escape while they can. Menardi: not without the Mars
  // Star. Alex sides with Felix, and Saturos agrees to leave it.
  Event_ShowMessage(ACTOR_FELIX, 0);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 10);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  // Jenna: "What about Isaac and Garet? You can't just leave them here!"
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 40);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
  // Saturos: "Forget about them." Menardi: they might survive. Saturos: "So
  // what do you suggest we do?" Alex: take Jenna, and they will bring the
  // Star to get her back.
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 10);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 20);
  // Alex: "Regardless, Kraden must come with us."
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  // Felix steps forward: Alex is breaking their promise. Alex: conditions
  // have changed. Felix will not let them hurt Jenna; Kraden offers himself
  // in her place.
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 314);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 10);
  Actor_SetAnimation(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
  // Saturos: "you cannot help Jenna now." Menardi: if they leave her, she
  // will die; is that what Felix wants?
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MENARDI, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // Alex: "Perhaps we ought to continue this elsewhere, Felix." Jenna:
  // "Felix..." Alex: "Please...can we go now?"
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SATUROS, 0.6);
  Actor_SetSpeed(ACTOR_MENARDI, 0.6);
  Actor_WalkTo(ACTOR_MENARDI, 475, 348);
  Actor_WalkTo(ACTOR_SATUROS, 491, 348);
  Actor_WaitForMoveById(ACTOR_MENARDI);
  Actor_WaitForMoveById(ACTOR_SATUROS);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_STAND);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 10);
  // Menardi: "Let's make haste!" She shoves Jenna, who stumbles away.
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI, 0);
  Actor_SetSpeed(ACTOR_MENARDI, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 338);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->steeringFlags = 254 & actor->steeringFlags;
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 348);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 459, 316);
  Actor_SetSpeed(ACTOR_JENNA, 0.5);
  // Felix: "Hey, don't push my sister!" Menardi: "You owe us your life!"
  // Saturos: "Menardi!"
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageThenWait(ACTOR_FELIX, 0, 30);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_Wait(20);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_MENARDI, 1);
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 332);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
  Event_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_JENNA, 432, 316);
  Actor_WalkToAndWait(ACTOR_FELIX, 422, 311);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
  // Felix leads Jenna aside. Saturos: "Master Kraden, you will be joining us,
  // of course!" All gather under the departure point.
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 20);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SATUROS, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
  Actor_SetSpeed(ACTOR_KRADEN, 0.5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 491, 296);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  Actor_SetSpeed(ACTOR_SATUROS, 1);
  Actor_SetSpeed(ACTOR_MENARDI, 1);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_WalkToAndWait(ACTOR_SATUROS, 471, 308);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_MENARDI, 455, 308);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 487, 308);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(472, CAMERA_KEEP, 300, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING);
  // One by one they rise out of the chamber: Saturos, Kraden, Menardi, then
  // Jenna ("Isaac, Garet! Don't die!") and Felix, and last Alex, who regrets
  // leaving the Mars Star.
  Actor_SetSpeed(ACTOR_SATUROS, 0.5);
  Actor_SetSpeed(ACTOR_MENARDI, 0.5);
  Actor_SetSpeed(ACTOR_ALEX, 0.5);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_SATUROS);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_KRADEN);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_MENARDI);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_JENNA);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST_SOUTHWEST, 40);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALEX, 0, 30);
  SolSanctumEruption_RaiseAndShrinkActor(ACTOR_ALEX);
  SolSanctumEruption_PlayWiseOneRescue();
  Party_RemoveMemberRestored(JENNA);
}

/// Lifts one departing actor and shrinks it together with its shadow on
/// the platform (ACTOR_PLATFORM_SHADOW).
static void SolSanctumEruption_RaiseAndShrinkActor(s32 actorId) {
  struct Actor *actor;
  struct Actor *shadow;
  struct Actor *target;
  u8 frame;

  shadow = Actor_Get(ACTOR_PLATFORM_SHADOW);
  shadow->scaleX = Q16_ONE;
  shadow->scaleY = Q16_ONE;
  Actor_WalkToAndWait(actorId, 471, 290);
  Actor_FaceDirection(actorId, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 471, 290);
  actor = Actor_Get(actorId);
  target = Actor_Get(actorId);
  Actor_SetSpriteFlags(target, 0);
  Actor_SetSpriteColorById(actorId, SPRITE_COLOR_BLINK);
  actor->motionFlags = 0;
  Sound_Play(SE_ASCEND);
  frame = 0;
  do {
    actor->y.fixed += px(1) / 2;
    Event_Wait(1);
    frame++;
  } while (frame != 60);
  Sound_Play(SE_WARP);
  frame = 0;
  do {
    actor->y.fixed += px(1) / 10;
    actor->scaleX -= Q16_ONE / 100;
    actor->scaleY -= Q16_ONE / 100;
    shadow->scaleX -= Q16_ONE / 100;
    shadow->scaleY -= Q16_ONE / 100;
    Event_Wait(1);
    frame++;
  } while (frame != 90);
  Actor_SetPosition(actorId, 0, 0);
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
}

/// Unused: an empty function nothing refers to.
UNUSED static void SolSanctumEruption_UnusedNoOpA(void) {}

/// Unused: an empty function nothing refers to.
UNUSED static void SolSanctumEruption_UnusedNoOpB(void) {}

/// Plays the collapse after the cut from the Elemental Star Room.
s32 Scene_Initialize(void) {
  if (gMainRoutineWork.entrance ==
      SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM) {
    FieldEffect_InitializeSparkles();
    SolSanctumEruption_PlayHostageEvent();
  }
  return 0;
}

/// Keeps a flying ember sprite rotated a quarter-turn from its facing.
static s32 SolSanctumEruption_AlignEmberToHeading(struct Actor *actor) {
  actor->sprite->rotation = actor->facing + TRIG_TURN / 4;
  return 1;
}

/// Launches sixteen crystals outward, then resets their actors for ember
/// emission.
static void SolSanctumEruption_ThrowEruptionRing(void) {
  struct Actor *actor;
  struct Actor *spun;
  u32 crystalIndex;
  s32 angle;
  struct Sprite *sprite;
  s32 offsetX;
  s32 offsetZ;

  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    actor = Actor_Get(crystalIndex);
    Actor_Stop(crystalIndex);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 2);
    actor->sprite->image.priority = 0;
    actor->motionFlags = 0;
    actor->speedLimit = px(8);
    actor->acceleration = px(3) / 4;
    actor->scaleX = 9 * Q16_ONE / 5;
    actor->scaleY = 9 * Q16_ONE / 5;
    actor->x.fixed = px(232);
    actor->y.fixed = px(20);
    actor->z.fixed = px(132);
  }
  Sound_Play(SE_CRASH);
  for (crystalIndex = 0; crystalIndex <= 15; crystalIndex++) {
    spun = Actor_Get(crystalIndex + 16);
    sprite = spun->sprite;
    angle = crystalIndex << 12;
    sprite->rotation = angle - TRIG_TURN / 4;
    offsetX = Iwram_MulQ16(Trig_Cos(angle), px(256));
    offsetZ = Iwram_MulQ16(Trig_Sin(angle), px(256));
    FieldActor_SetMoveTargetFixed(spun, spun->x.fixed + offsetX, spun->y.fixed,
                                  spun->z.fixed + offsetZ);
  }
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_FIRST_CRYSTAL);
  for (crystalIndex = 16; crystalIndex <= 31; crystalIndex++) {
    actor = Actor_Get(crystalIndex);
    Actor_Stop(crystalIndex);
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    SET_VELOCITY(actor, 0, 0, 0);
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  }
}

/// Resets the sixteen ember slots and their emission timers.
static void SolSanctumEruption_ResetEmberEmission(void) {
  u32 emberIndex;

  gEmberMask = 63;
  gEmberTimer = 0;
  gEmberLevel = 0;
  gEmberLevelTimer = 120;
  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    gEmberState[emberIndex] = 0;
  }
}

/// Retires landed embers and launches new ones, shaking the map.
static void SolSanctumEruption_StepEmbers(void) {
  struct WeatherWork *sparkles;
  s32 shakePixels;
  u8 emberIndex;
  struct Actor *landedEmber;
  struct Actor *ember;
  s16 angle;
  s32 x;
  s32 z;

  sparkles = gWeatherWork;
  shakePixels = (u32)gEmberTimer / 10;
  if (shakePixels != 0) {
    sparkles->stopped = 0;
    MapGrid_SetShakeFixed(px(shakePixels), px(shakePixels), Q16_ONE);
  } else {
    sparkles->stopped = 1;
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  }
  if (gEmberTimer != 0)
    gEmberTimer -= 3;

  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    if (gEmberState[emberIndex] != 0) {
      landedEmber = Actor_Get(emberIndex + 16);
      if (landedEmber->targetX == ACTOR_NO_TARGET &&
          landedEmber->targetZ == ACTOR_NO_TARGET) {
        gEmberState[emberIndex]++;
        if (gEmberState[emberIndex] == 2)
          Actor_SelectAnimation(landedEmber, 3);
        if (gEmberState[emberIndex] == 19) {
          landedEmber->x.fixed = 0;
          landedEmber->y.fixed = 0;
          landedEmber->z.fixed = 0;
          SET_VELOCITY(landedEmber, 0, 0, 0);
          landedEmber->targetX = ACTOR_NO_TARGET;
          landedEmber->targetY = ACTOR_NO_TARGET;
          landedEmber->targetZ = ACTOR_NO_TARGET;
          Actor_SetSpriteColor(landedEmber, SPRITE_COLOR_HIDDEN);
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
    angle = Math_RemainderUnsigned(Random_Next(), 0xffff);
    ember = Actor_Get(emberIndex + 16);
    if (gEmberState[emberIndex] == 0) {
      if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING) == 0)
        Sound_Play(SE_GLITTER);
      gEmberState[emberIndex] = 1;
      ember->motionFlags = 0;
      ember->speedLimit = px(8);
      ember->acceleration = px(1);
      Actor_SetSpriteFlags(ember, 0);
      ember->sprite->image.priority = 1;
      Actor_SelectAnimation(ember, 2);
      Actor_StartScript(ember, gEruption_EmberScript);
      ember->x.fixed = Iwram_MulQ16(Trig_Cos((u16)angle),
                                    ((Random_Below(256)) << 16) + px(256)) +
                       px(325);
      ember->y.fixed = 0;
      ember->z.fixed = Iwram_MulQ16(Trig_Sin((u16)angle),
                                    ((Random_Below(256)) << 16) + px(256)) +
                       px(302);
      x = Iwram_MulQ16(Trig_Cos((u16)angle),
                       ((Random_Next() & 63) << 16) + px(8));
      z = Iwram_MulQ16(Trig_Sin((u16)angle),
                       ((Random_Next() & 63) << 16) + px(8));
      FieldActor_SetMoveTargetFixed(ember, x + px(325), 0, z + px(286));
      Actor_SetSpriteColor(ember, SPRITE_COLOR_NORMAL);
      gEmberTimer = 30;
      return;
    }
  }
}

/// Varies the ember emission interval and its duration.
static void SolSanctumEruption_UpdateEmberFrequency(void) {
  u32 emissionLevel;

  if (gEmberLevelTimer != 0) {
    gEmberLevelTimer--;
    return;
  }
  if (gEmberLevel != 0) {
    gEmberLevel--;
  } else {
    gEmberLevel = Random_Below(4);
  }
  emissionLevel = gEmberLevel;
  switch (emissionLevel) {
  case 3:
    gEmberMask = emissionLevel;
    gEmberLevelTimer = (Random_Below(20)) + 40;
    break;
  case 2:
    gEmberMask = 15;
    gEmberLevelTimer = (Random_Below(40)) + 80;
    break;
  case 1:
    gEmberMask = 63;
    gEmberLevelTimer = (Random_Below(80)) + 160;
    break;
  default:
    gEmberMask = 127;
    gEmberLevelTimer = (Random_Below(160)) + 320;
    break;
  }
}

/// Ends the Wise One's palette animation and clears its attached effect.
static inline void SolSanctumEruption_FinishMaterialization(void) {
  Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
  Task_Remove(SolSanctumEruption_UpdateWiseOnePalette);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
}

/// Elemental Star Room, collapsing: the Wise One sends Isaac and Garet out.
static void SolSanctumEruption_PlayWiseOneRescue(void) {
  struct WeatherWork *sparkles;
  struct Actor *wiseOne;
  struct Actor *center;
  struct Actor *leader;
  struct Actor *starOrigin;
  struct Actor *garet;
  struct Sprite *leaderSprite;
  struct Sprite *garetSprite;
  struct Actor *star;
  struct Sprite *starSprite;
  u8 *iconPixels;
  u8 crystalIndex;
  u32 frame;
  s32 flickerDelay;

  sparkles = gWeatherWork;
  wiseOne = Actor_Get(ACTOR_WISE_ONE);
  Task_Remove(SolSanctumEruption_UpdateEmberFrequency);
  gEmberMask = 3;
  Event_Wait(80);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Event_Wait(40);
  Task_Remove(SolSanctumEruption_StepEmbers);
  for (crystalIndex = 0; crystalIndex < CRYSTAL_COUNT; crystalIndex++) {
    Actor_SetPosition(ACTOR_FIRST_CRYSTAL + crystalIndex, 0, 0);
  }
  wait(1);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  SolSanctumEruption_PlaceBeforeStar(ACTOR_PARTY_LEADER);
  SolSanctumEruption_PlaceBeforeStar(ACTOR_GARET);
  sparkles->stopped = 0;
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(80);
  SolSanctumEruption_RestoreCornerCellAttributes();
  center = FieldCamera_GetActor();
  center->motionFlags = 0;
  center->x.fixed = px(231);
  center->z.fixed = px(144);
  center->targetX = ACTOR_NO_TARGET;
  center->targetY = ACTOR_NO_TARGET;
  center->targetZ = ACTOR_NO_TARGET;
  center->y.fixed = 0;
  center->velocityX = 0;
  center->velocityZ = 0;
  wait(4);
  MapGrid_CenterOnFollowedPoint();
  wait(4);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Event_Wait(40);

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  garet = Actor_Get(ACTOR_GARET);
  leaderSprite = leader->sprite;
  garetSprite = garet->sprite;
  for (frame = 0; frame < 20; frame++) {
    leaderSprite->rotation += TRIG_TURN / 256;
    garetSprite->rotation -= TRIG_TURN / 256;
    leader->x.fixed += px(3) / 8;
    garet->x.fixed -= px(3) / 8;
    wait(1);
  }
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_GARET, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
  Actor_Get(ACTOR_GARET)->sprite->rotation = 0;
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 246, 150);
  Actor_MoveToAndWait(ACTOR_GARET, 220, 150);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_NORTHWEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
  Event_Wait(60);

  // Garet: "Well, this turned out badly, wouldn't you say?" Yes: "It's a
  // miracle we survived." No: "How can you be so calm?"
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_SetMessageId(MsgSolSanctumWellTurnedBadly);
  // "Well, this turned out badly, wouldn't you say?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_SetMessageId(MsgSolSanctumWellTurnedBadly + 1);
  } else {
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_SetMessageId(MsgSolSanctumWellTurnedBadly + 2);
  }
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(40);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  Event_SetMessageId(MsgSolSanctumFriendsGone);
  // "Jenna and Kraden are gone!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // Garet: "You don't think they'll hurt Jenna and Kraden, do you?" Either
  // way, he trusts Felix to look after them.
  // "You don't think they'll hurt Jenna and Kraden, do you?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgSolSanctumThanksALot);
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgSolSanctumTheyllBeSafe);
  }
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(40);

  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(284, CAMERA_KEEP, 92, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  FieldCamera_SetSpeed(1.5);
  FieldCamera_MoveTo(127, CAMERA_KEEP, 162, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 294, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(400, CAMERA_KEEP, 215, 1);
  FieldCamera_WaitForMove();
  Event_Wait(60);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(273, CAMERA_KEEP, 145, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  // Garet: "This really is terrible..." and they have a long way to go.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_SetMessageId(MsgSolSanctumThisIsTerrible);
  // "This really is terrible..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
  Event_Wait(10);
  // "We've got a long way to go if we're gonna make it out of here."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);

  // A violent tremor throws them aside.
  Sound_Play(MUS_DANGER);
  SolSanctumEruption_SetMaterializationTint(1, 4, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(5, 5, 1);
  Event_Wait(10);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_GARET, 2);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 243, 144);
  Actor_SetDestination(ACTOR_GARET, 202, 144);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_Wait(20);
  sparkles->stopped = 1;
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(80);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Actor_SetSpeed(ACTOR_GARET, 0.4);
  Actor_WalkTo(ACTOR_GARET, 220, 150);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 150);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(40);
  // Garet: "We're in big trouble..." Yes: "Let's get out of here..." No:
  // "Quit acting so tough!"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  } else {
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgSolSanctumQuitActingTough);
  }
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);

  // The Wise One drifts in from the west, unseen by Isaac; Garet walks off
  // and calls him: "I'm over here. Come on, we've gotta go."
  Actor_SetPosition(ACTOR_WISE_ONE, 110, 152);
  wait(1);
  Actor_SetSpeed(ACTOR_WISE_ONE, 1.2);
  Actor_SetDestination(ACTOR_WISE_ONE, 171, 152);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkTo(ACTOR_GARET, 217, 182);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_SOUTHWEST, 0);
  Event_Wait(10);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(217, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_Wait(4);
  Actor_WalkToAndWait(ACTOR_GARET, 231, 175);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Event_SetMessageId(MsgSolSanctumOverHere);
  // "Hey, Isaac... Isaac, I'm over here. Come on, we've gotta go."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);

  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  sparkles->stopped = 1;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(120);
  // The Wise One: "Why are you standing there?" and "This place has become
  // dangerous..." A jolt; Garet: "You're the dangerous one!"
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(100);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  SolSanctumEruption_SetMaterializationTint(1, 10, 0);
  sparkles->stopped = 0;
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_GARET, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 256, 150);
  Actor_SetDestination(ACTOR_GARET, 231, 180);
  Actor_WaitForMoveById(ACTOR_GARET);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);

  // The floor by the Wise One starts to break up; the Wise One glows and
  // holds it together with Psynergy.
  Sound_Play(SE_TREMOR);
  Task_AddFrame(SolSanctumEruption_FlickerFirstCollapseTiles, TASK_PASS_FRAME);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(186, CAMERA_KEEP, 166, 1);
  Actor_SetDestination(ACTOR_WISE_ONE, 130, 113);
  Actor_WaitForMoveById(ACTOR_WISE_ONE);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  sparkles->stopped = 0;
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  Scene_StartEventPsynergy();
  Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
  }
  Task_AddFrame(SolSanctumEruption_UpdateWiseOnePalette, TASK_PASS_FRAME);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Task_Remove(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(1, 1, 1);
  flickerDelay = 16;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    wait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    wait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  sparkles->stopped = 1;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  SolSanctumEruption_FinishMaterialization();
  Event_Wait(60);
  // The Wise One: "The volcano will soon erupt... I cannot hold it long."
  // and, after drifting closer, "You must leave this place."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(218, CAMERA_KEEP, 181, 1);
  Actor_SetSpeed(ACTOR_WISE_ONE, 1);
  Actor_SetDestination(ACTOR_WISE_ONE, 169, 151);
  Actor_WaitForMoveById(ACTOR_WISE_ONE);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 0);
  Event_Wait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST_NORTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  FieldCamera_MoveTo(224, CAMERA_KEEP, 158, 1);
  FieldCamera_WaitForMove();
  // Lights rise at the chamber's first corner. The Wise One: the Elemental
  // Djinn have returned; "Take out the Elemental Star..." Refusing, "Isaac
  // cannot resist."
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_NORTHWEST);
  Actor_RunRepeatedMotion(ACTOR_WISE_ONE, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(40);
  Event_OpenMessage(ACTOR_WISE_ONE, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(40);
  } else {
    FieldMessage_Show(MsgSolSanctumCannotResist, 1);
    Event_Wait(40);
  }

  starOrigin = Actor_Get(ACTOR_PARTY_LEADER);
  star = FieldActor_Create(ACTOR_ELEMENTAL_STAR, starOrigin->x.fixed,
                           starOrigin->y.fixed + px(36), starOrigin->z.fixed);
  if (star != NULL) {
    iconPixels =
        HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    starSprite = star->sprite;
    starSprite->flags = 0;
    starSprite->count = 0;
    starSprite->image.fullColor = 0;
    starSprite->image.palette = 0;
    starSprite->image.priority = 1;
    Icon_DrawItem(ITEM_MARS_STAR);
    VramCache_Load(starSprite->slot, 128, iconPixels + 1024);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
    ItemDisplay_ShowPicture(star, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  }
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  for (frame = 0; frame < 24; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
    star->scaleX = 2 * Q16_ONE / 5;
    star->scaleY = 2 * Q16_ONE / 5;
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
    star->scaleX = Q16_ONE;
    star->scaleY = Q16_ONE;
  }
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  // The Wise One: "Return it to its bag." and the Elemental Stars are moving
  // away from here.
  Event_SetMessageId(MsgSolSanctumReturnStarToBag);
  // "Return it to its bag."
  Event_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 20);
  if (star != NULL) {
    Actor_Release(star);
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 60);
  // "The Elemental Stars are... moving away from here..."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(40);

  // The camera visits the other three corners as lights rise there. The Wise
  // One: the magma flows freely, there will be no chamber for the Stars to
  // return to, and the world is exposed to Alchemy. Garet: "Alchemy? A
  // threat?"
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  FieldCamera_SetSpeed(4);
  SolSanctumEruption_FadeCameraTo(232, 464);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_SOUTHWEST);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 144);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_NORTHEAST);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 464);
  SolSanctumEruption_RaiseElementalArcs(CHAMBER_SOUTHEAST);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_GARET, 582, 345);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetPosition(ACTOR_GARET, 231, 180);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  wait(20);
  // Back with the party, the Wise One: misused, Alchemy is dangerous; its
  // power is released if the Stars light the four lighthouses.
  SolSanctumEruption_FadeCameraTo(219, 171);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(40);

  sparkles->stopped = 0;
  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  // The floor breaks up again in two places; the Wise One floats to each and
  // holds it together.
  Sound_Play(SE_TREMOR);
  Task_AddFrame(SolSanctumEruption_FlickerSecondCollapseTiles, TASK_PASS_FRAME);
  Event_Wait(20);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 132, 1);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 245, 145);
  Actor_SetDestination(ACTOR_GARET, 215, 168);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(80);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_WISE_ONE, 184, 87);
  Actor_WaitForMoveById(ACTOR_WISE_ONE);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
  }
  Task_AddFrame(SolSanctumEruption_UpdateWiseOnePalette, TASK_PASS_FRAME);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Task_Remove(SolSanctumEruption_FlickerSecondCollapseTiles);
  MapGrid_SetShake(2, 2, 1);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawSecondCollapseTiles(0);
    wait(flickerDelay);
    SolSanctumEruption_DrawSecondCollapseTiles(1);
    wait(flickerDelay);
  }
  SolSanctumEruption_DrawSecondCollapseTiles(0);
  MapGrid_SetShake(1, 1, 1);
  SolSanctumEruption_FinishMaterialization();
  Event_Wait(60);

  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  Sound_Play(SE_TREMOR);
  Task_AddFrame(SolSanctumEruption_FlickerFirstCollapseTiles, TASK_PASS_FRAME);
  Event_Wait(40);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 127, 110);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
  }
  Task_AddFrame(SolSanctumEruption_UpdateWiseOnePalette, TASK_PASS_FRAME);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(30);
  Task_Remove(SolSanctumEruption_FlickerFirstCollapseTiles);
  MapGrid_SetShake(2, 2, 1);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    wait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    wait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  MapGrid_SetShake(1, 1, 1);
  Sound_Play(SE_TREMOR);
  Sound_Play(MUS_ERUPTION);
  MapGrid_SetShake(4, 4, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  Sound_Play(SE_TREMOR);
  Task_AddFrame(SolSanctumEruption_FlickerSecondCollapseTiles, TASK_PASS_FRAME);
  SolSanctumEruption_FinishMaterialization();
  Event_Wait(60);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 184, 87);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Event_Wait(10);
  SolSanctumEruption_FinishMaterialization();
  Sound_Play(SE_RUMBLE);
  Event_Wait(100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // The Wise One: "It's erupting... Flee, now!" Garet: "But how..." The Wise
  // One: "I will help you..." It glows and the chamber fades to white.
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    wait(1);
  }
  Task_AddFrame(SolSanctumEruption_UpdateWiseOnePalette, TASK_PASS_FRAME);
  Event_Wait(20);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(60);
  wait(100);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  Task_Remove(SolSanctumEruption_UpdateWiseOnePalette);
  sparkles->stopped = 1;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Scene_EndEventPsynergy();
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
  Import_EventSetExitRequest(
      SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS);
  // XXX (no evidence): what flag 0x100 records; nothing else sets it and
  // nothing in the game reads it.
  GameFlag_Set(0x100);
}

/// Places an actor before the star, facing south, and draws it above the
/// chamber floor.
static void SolSanctumEruption_PlaceBeforeStar(s32 actorId) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  Actor_SetPosition(actorId, 232, 125);
  actor->facing = FACING_SOUTH;
  Actor_SetSpritePriority(actorId, 3);
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

/// Randomly alternates the first tile patch on even frames.
static void SolSanctumEruption_FlickerFirstCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Random_Next(), 100) > 50) {
      SolSanctumEruption_DrawFirstCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawFirstCollapseTiles(0);
    }
  }
}

/// Randomly alternates the second tile patch on even frames, as the first.
static void SolSanctumEruption_FlickerSecondCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Random_Next(), 100) > 50) {
      SolSanctumEruption_DrawSecondCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawSecondCollapseTiles(0);
    }
  }
}

/// Stages and raises ten arc effects at one of the four chamber positions.
static void SolSanctumEruption_RaiseElementalArcs(u32 corner) {
  void Actor_ShowEmote(s32, s32, s32);
  void PaletteFade_ApplyTarget(s32, s32);
  void Sound_Play(s32);

  u32 arcIndex, clear;
  s32 x, y, z;
  s32 *origin;
  struct Actor *actor;
  struct Sprite *sprite;

  for (arcIndex = 0; arcIndex <= 15; arcIndex++)
    Actor_ReleaseById(arcIndex + ACTOR_FIRST_CRYSTAL);
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
  Sound_Play(SE_RISING_CHIMES);
  arcIndex = 0;
  clear = arcIndex;
  for (origin = &gEruption_ArcOrigins[0]; arcIndex <= 9;
       arcIndex++, origin += 2) {
    x = origin[0];
    y = origin[1];
    z = 0;
    switch (corner) {
    case CHAMBER_NORTHWEST:
      x += px(232);
      z = px(144);
      break;
    case CHAMBER_SOUTHWEST:
      x += px(232);
      z = px(464);
      break;
    case CHAMBER_NORTHEAST:
      x += px(711);
      z = px(144);
      break;
    case CHAMBER_SOUTHEAST:
      x += px(711);
      z = px(464);
      break;
    }
    gArcEffectTimers[arcIndex] = clear;
    actor = FieldActor_Create(SPRITE_PARTICLE, x, y, z);
    gArcEffects[arcIndex] = actor;
    actor->motionFlags = clear;
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->image.priority = 1;
    Actor_SelectAnimation(actor, 6);
    wait(6);
  }
  if (corner == CHAMBER_NORTHWEST) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  }
  wait(20);
  Task_AddFrame(SolSanctumEruption_UpdateElementalArcs, TASK_PASS_FRAME);
  Sound_Play(SE_GLITTER);
  gArcEffectTimers[0] = 1;
  wait(6);
  gArcEffectTimers[1] = 1;
  wait(6);
  gArcEffectTimers[2] = 1;
  wait(6);
  gArcEffectTimers[3] = 1;
  wait(6);
  gArcEffectTimers[4] = 1;
  wait(6);
  gArcEffectTimers[5] = 1;
  wait(6);
  gArcEffectTimers[6] = 1;
  wait(6);
  gArcEffectTimers[7] = 1;
  wait(6);
  gArcEffectTimers[8] = 1;
  wait(6);
  gArcEffectTimers[9] = 1;
  wait(6);
  for (;;) {
    for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
      if (gArcEffectTimers[arcIndex] != 0) {
        arcIndex = ARC_SCAN_STILL_ACTIVE;
        break;
      }
    }
    if (arcIndex != ARC_SCAN_STILL_ACTIVE)
      break;
    wait(1);
  }
  wait(40);
  Task_Remove(SolSanctumEruption_UpdateElementalArcs);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(40);
}

/// Stretches each active arc for eight frames, then lifts it out of view.
static void SolSanctumEruption_UpdateElementalArcs(void) {
  u32 arcIndex;
  s32 age;
  struct Actor *actor;

  for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
    age = gArcEffectTimers[arcIndex];
    if (age != 0) {
      actor = gArcEffects[arcIndex];
      if ((u32)age <= 8) {
        actor->scaleX -= ELEMENTAL_ARC_SCALE_X_DECREASE;
        actor->scaleY += Q16_ONE / 2;
        actor->y.fixed += px(3) / 10;
        actor->targetY += px(3) / 10;
      } else {
        actor->y.fixed += px(20);
        actor->targetY += px(20);
      }
      age = gArcEffectTimers[arcIndex] + 1;
      gArcEffectTimers[arcIndex] = age;
      if ((u32)age > 14) {
        gArcEffectTimers[arcIndex] = 0;
      }
    }
  }
}

/// Fades out, moves the camera and its focus actor, then fades back in.
static void SolSanctumEruption_FadeCameraTo(s32 x, s32 z) {
  struct Actor *center;

  center = FieldCamera_GetActor();
  z *= px(1);
  x *= px(1);
  FieldCamera_MoveWithinMapBounds(x, CAMERA_KEEP, z, 1);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  wait(40);
  center->x.fixed = x;
  center->z.fixed = z;
  center->targetX = ACTOR_NO_TARGET;
  center->targetZ = ACTOR_NO_TARGET;
  center->velocityX = 0;
  center->velocityZ = 0;
  wait(5);
  MapGrid_CenterOnFollowedPoint();
  wait(5);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  wait(30);
}

/// Restores eight corner-cell cell behaviors from the chamber placements.
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

/// Fades the sprite palette into or out of the materialization tint.
static void SolSanctumEruption_SetMaterializationTint(s32 enabled, s32 frames,
                                                      s32 waitFrames) {
  if (enabled == 1) {
    Sound_Play(SE_MATERIALIZE);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(18, 18, 14), PALETTES_BG);
  } else {
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  }
  PaletteFade_StartInterpolation(frames);
  if (waitFrames != 0) {
    Event_Wait(waitFrames);
  }
}

/// Flashes the palette and spawns paired vertical effects every sixteen frames.
static void SolSanctumEruption_FlashMaterializingActor(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetColorMode(actor, 7);
  } else {
    Actor_SetColorMode(actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(actor);
  }
}

/// Cycles six palettes while spawning the actor's vertical effects.
static void SolSanctumEruption_CycleActorPalette(struct Actor *actor) {
  if (gFrameCount & 1) {
    Actor_SetColorMode(actor, Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(actor);
  }
}

#include "shared/field_effect_vertical.c"

/// Creates two vertical effects sharing the parent position and cached tiles.
static void SolSanctumEruption_SpawnEffectPair(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct Sprite *sprite;
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  s32 effectIndex;

  Sound_Play(SE_ERUPTION_COLUMNS);
  for (effectIndex = 0; effectIndex < 2; ++effectIndex) {
    child = FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                              parent->y.fixed, parent->z.fixed);
    pair[effectIndex] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = effectWork->tileSlot;
        sprite->retainCacheEntry = 1;
        sprite->image.tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & 0x3ff;
        sprite->image.fullColor = 0;
        sprite->image.shape = 1;
        sprite->image.size = 2;
        sprite->entries[0]->picture = 0;
      }
    }
  }
  pair[0]->update = FieldEffect_AnimateVerticalNegative;
  pair[0]->sprite->image.priority =
      Actor_Get(ACTOR_WISE_ONE)->sprite->image.priority;
  {
    struct Actor *wiseOne = Actor_Get(ACTOR_WISE_ONE);
    struct Actor *risingEffect = pair[1];

    risingEffect->sprite->image.priority = wiseOne->sprite->image.priority;
    risingEffect->update = FieldEffect_AnimateVerticalPositive;
    risingEffect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

#include "shared/event_psynergy.c"

/// Cycles the Wise One's palette while it glows.
static void SolSanctumEruption_UpdateWiseOnePalette(void) {
  SolSanctumEruption_CycleActorPalette(Actor_Get(ACTOR_WISE_ONE));
}

#include "shared/sol_sanctum_elemental_star_ring_setup.c"

s32 gEruption_ArcOriginsHead[] = {
    SC_SET(SW_VELOCITY_Y, px(6)),
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
    -px(6), 0,
    px(10), px(10),
    px(8), px(25),
    px(4), px(30),
    -px(5), px(20),
    px(2), px(5),
    -px(6), px(35),
    -px(8), px(15),
    px(2), px(40),
    -px(2), px(15),
};

struct SceneEntrance gSolSanctumEruption_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSolSanctumEruption_Exits[] = {
    SCENE_EXITS(SolSanctumEruption),
    SCENE_EXIT(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, SolSanctumLunaSolRooms, SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION),
    SCENE_EXITS_END,
};

struct ActorPlacement gSolSanctumEruption_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX_MASKED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gSolSanctumEruption_Triggers[] = {
    { TRIGGER_TABLE_END },
};

/// Unused: a copy of the Elemental Star room's cell animation
/// (gSolSanctumElementalStarRoom_StarCells), nine steps of a 3-by-4 picture.
/// Nothing in the eruption plays it.
u16 gSolSanctumEruption_UnusedStarCells[] = {
    MAPGRID_ANIMATION_STEP(59, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(62, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(65, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(68, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(71, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(74, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(77, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(80, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(83, 40, 3, 4, 0), MAPGRID_ANIMATION_STEPS_END,
};

s32 gEruption_EmberScript[] = {
    SC_CALL(SolSanctumEruption_AlignEmberToHeading),
    SC_END,
};

#include "shared/sol_sanctum_elemental_star_ring_tables.c"
// clang-format on
