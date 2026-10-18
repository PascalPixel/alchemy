#include "script.h"
#include "game_state.h"
#include "unit/scene/sol_sanctum_eruption.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/scene/common/sol_sanctum_elemental_star_ring_setup.h"
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
#include "field_event.h"
#include "field_service.h"
#include "iwram_call.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "field_obj.h"
#include "global_cells.h"
#include "vram_tab.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "macros.h"

// Forward declarations.
static void SolSanctumEruption_RaiseAndShrinkActor(s32 actorId);
static void SolSanctumEruption_ThrowEruptionRing(void);
static void SolSanctumEruption_ResetEmberEmission(void);
static void SolSanctumEruption_StepEmbers(void);
static void SolSanctumEruption_UpdateEmberFrequency(void);
static void SolSanctumEruption_PlayWiseOneRescue(void);
static void SolSanctumEruption_PlaceBeforeStar(s32 actor);
static void SolSanctumEruption_DrawFirstCollapseTiles(s32 alternate);
static void SolSanctumEruption_DrawSecondCollapseTiles(s32 alternate);
static void SolSanctumEruption_FlickerFirstCollapseTiles(void);
static void SolSanctumEruption_FlickerSecondCollapseTiles(void);
static void SolSanctumEruption_RaiseElementalArcs(u32 mode);
static void SolSanctumEruption_UpdateElementalArcs(void);
static void SolSanctumEruption_FadeCameraTo(s32 x, s32 z);
static void SolSanctumEruption_RestoreCornerCellAttributes(void);
static void SolSanctumEruption_SetMaterializationTint(s32 enabled, s32 frames,
                                                      s32 waitFrames);
static void
SolSanctumEruption_FlashMaterializingActor(struct FieldActor *actor);
static void SolSanctumEruption_SpawnEffectPair(struct FieldActor *parent);
static void SolSanctumEruption_UpdateWiseOnePalette(void);

s32 Math_RemainderUnsigned(s32 value, s32 modulus);

enum { ACTOR_WISE_ONE = 15, ACTOR_FIRST_ROCK = 16, ROCK_COUNT = 16 };

enum { OBJECT_ELEMENTAL_STAR = 22, ITEM_MARS_STAR = 222 };

#define ELEMENTAL_ARC_SCALE_X_DECREASE 7372
#define EMBER_EMISSION_DISABLED 999
#define ARC_SCAN_STILL_ACTIVE 888

enum { FLAG_SOL_SANCTUM_ERUPTED = 0x814, FLAG_STAR_ROOM_COLLAPSED = 0x83f };

struct PairWork {
  u8 unk00[70];
  u16 vramBlock;
};

// Return the arrival positions for the collapsing star chamber.
struct SceneEntrance *SolSanctumEruption_GetEntrances(void) {
  return gSolSanctumEruption_Entrances;
}

// This scene has no separate trigger-region table.
struct SceneRegion *SolSanctumEruption_GetRegions(void) { return NULL; }

// Return the scene destinations for the collapsing chamber.
u32 *SolSanctumEruption_GetExits(void) { return gSolSanctumEruption_Exits; }

// Return the hostages, antagonists and eruption effects.
struct ScenePlacement *SolSanctumEruption_GetPlacements(void) {
  return gSolSanctumEruption_Placements;
}

// Return the scene event table, which has no scripted interactions.
struct SceneEvent *SolSanctumEruption_GetEvents(void) {
  return gSolSanctumEruption_Events;
}

// Stage Saturos, Menardi, Alex and Felix around Jenna and Kraden.
static inline void SolSanctumEruption_PlaceHostageGroup(void) {
  Actor_SetPosition(JENNA, PIXELS(475), PIXELS(332));
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(491), PIXELS(332));
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(459), PIXELS(348));
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(507), PIXELS(348));
  Actor_SetPosition(ACTOR_FELIX, PIXELS(471), PIXELS(306));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(479), PIXELS(362));
}

// Play Saturos taking the hostages before the Wise One rescues Isaac and Garet.
static void SolSanctumEruption_PlayHostageScene(void) {
  void Actor_ShowEmote();
  void ColorBuffer_ApplyTarget();
  void Event_PlayCue();

  struct FieldActor *actor;
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
  Engine_EventBegin();
  sparkles->stopped = 0;
  // Reopen the chamber during the first quake and stage the two groups.
  Event_PlayCue(141);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(232), PIXELS(156));
  Actor_SetPosition(GARET, PIXELS(218), PIXELS(172));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  SolSanctumEruption_PlaceHostageGroup();
  Engine_ActorSetSpritePriority(15, 1);
#ifndef UBFIX
  // UB: these are separate work-pointer globals. The native sequence reaches
  // the event slot through the particle slot; named globals add four bytes
  // and change this scene's register allocation (EN, 2026-10-16).
  eventWorkSlot = (struct EventWork **)((u8 *)particleWorkSlot - 8);
#endif
  Camera_MoveTo(PIXELS(232), -1, PIXELS(156), 0);
  Engine_MapRedraw();
#ifdef UBFIX
  gEventWork->transitionFrames = 8;
#else
  (*eventWorkSlot)->transitionFrames = 8;
#endif
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_EventWait(4);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_EventWait(4);
  SolSanctumEruption_ThrowEruptionRing();
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_EventWait(16);
  Event_PlayCue(144);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_EventWait(4);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(4);
  Engine_EventWait(4);
  Event_PlayCue(144);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(48);
  Engine_EventWait(48);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 20);
  // Jenna and the others react to the shaking before noticing the Wise One.
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 40);
  Engine_EventSetMessage(MsgSolSanctumJennaWhatHappened);
  Engine_EventShowMessageAndWait(11, 0, 20);
  Engine_EventShowMessage(10, 0);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 30);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  actor = Object_GetById(15);
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_SetPosition(15, PIXELS(325), PIXELS(302));
  actor = Object_GetById(15);
  actor->motionFlags = 5;
  sparkles->stopped = 1;
  Event_PlayCue(0x121);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(150);
  // Turn from Isaac and Garet toward the hostages and the Wise One.
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
  Event_ShowMessage(JENNA, 0);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  SolSanctumEruption_PlaceHostageGroup();
  Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(328), -1, PIXELS(299), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(167);
  ColorBuffer_ApplyTarget(0x205294, 2);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(200);
  Event_OpenMessage(0x1001, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumIKnowItsARock);
  } else {
    Engine_EventSetMessage(MsgSolSanctumSomeoneIsLiftingIt);
  }
  Event_ShowMessageAndWait(0x1001, 0, 80);
  Engine_EventSetMessage(MsgSolSanctumKradenCouldThatBeThe);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  Event_PlayCue(141);
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(80);
  sparkles->stopped = 1;
  Event_PlayCue(0x121);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  SolSanctumEruption_SetMaterializationTint(0, 20, 60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
  Actor_FaceDirection(15, FACING_NORTHWEST, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Event_ShowMessageAndWait(0x1001, 0, 30);
  Actor_FaceDirection(15, FACING_EAST + FACING_STEP, 40);
  SolSanctumEruption_SetMaterializationTint(1, 20, 20);
  SolSanctumEruption_SetMaterializationTint(0, 20, 20);
  Event_ShowMessageAndWait(JENNA, 0, 30);
  SolSanctumEruption_ResetEmberEmission();
  Task_AddCallback(SolSanctumEruption_StepEmbers, 0xc80);
  Task_AddCallback(SolSanctumEruption_UpdateEmberFrequency, 0xc80);
  Engine_EventWait(240);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
  SolSanctumEruption_PlaceHostageGroup();
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  actor = Object_GetById(JENNA);
  Camera_MoveTo(actor->x.part.pixel * PIXELS(1), -1,
                actor->z.part.pixel * PIXELS(1), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FELIX, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
  Engine_ActorSetAnimation(ACTOR_ALEX, 4);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 40);
  Event_ShowMessageAndWait(0x4005, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 5, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 0);
  // Felix steps forward while Alex and Kraden join the argument.
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_FELIX, 471, 314);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_ALEX, 4);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 1);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessageAndWait(0x200e, 0, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessageAndWait(0x2005, 0, 40);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(40);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 40);
  Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_MENARDI, 475, 348);
  Actor_WalkTo(ACTOR_SATUROS, 491, 348);
  Engine_ActorWaitForMove(ACTOR_MENARDI);
  Engine_ActorWaitForMove(ACTOR_SATUROS);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 1);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  Event_ShowMessage(0x200b, 0);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(8) / 5, PIXELS(4) / 5);
  // Stage Menardi and Jenna for the hostage exchange.
  Actor_SetSpeed(JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 338);
  actor = Object_GetById(11);
  actor->steeringFlags = 254 & actor->steeringFlags;
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 348);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_ActorJump(JENNA, 4, 0);
  Actor_WalkToAndWait(JENNA, 459, 316);
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 3);
  Event_ShowMessageAndWait(ACTOR_FELIX, 0, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_EventWait(20);
  actor->steeringFlags |= 1;
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_MENARDI, 475, 332);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Event_ShowMessage(0x200b, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
  Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | 2, 20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(20);
  Actor_WalkTo(JENNA, 432, 316);
  Actor_WalkToAndWait(ACTOR_FELIX, 422, 311);
  Engine_ActorSetAnimation(JENNA, 1);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(0x100a, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 491, 296);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_SATUROS, 471, 308);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_MENARDI, 455, 308);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 487, 308);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(472), -1, PIXELS(300), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  GameFlag_Set(0x246);
  // Raise and remove the departing group, leaving Isaac and Garet behind.
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  SolSanctumEruption_RaiseAndShrinkActor(10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumEruption_RaiseAndShrinkActor(9);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  SolSanctumEruption_RaiseAndShrinkActor(11);
  Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 40);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(0x2005, 0, 40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  SolSanctumEruption_RaiseAndShrinkActor(5);
  SolSanctumEruption_RaiseAndShrinkActor(13);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST + FACING_STEP, 40);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
  SolSanctumEruption_RaiseAndShrinkActor(14);
  SolSanctumEruption_PlayWiseOneRescue();
  Party_RemoveOwnerRestored(5);
}

// Lift one departing actor and shrink it together with the paired effect.
static void SolSanctumEruption_RaiseAndShrinkActor(s32 actorId) {
  struct FieldActor *actor;
  struct FieldActor *departureEffect;
  struct FieldActor *spriteOwner;
  u8 frame;

  departureEffect = Object_GetById(8);
  departureEffect->scaleX = Q16_ONE;
  departureEffect->scaleY = Q16_ONE;
  Engine_ActorWalkToAndWait(actorId, 471, 290);
  Actor_FaceDirection(actorId, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_SetPosition(8, PIXELS(471), PIXELS(290));
  actor = Object_GetById(actorId);
  spriteOwner = Object_GetById(actorId);
  Engine_ActorSetSpriteFlags(spriteOwner, 0);
  Actor_SetChildValue(actorId, 0x100);
  actor->motionFlags = 0;
  Event_PlayCue(201);
  // Rise at full size, then shrink the actor and its departure effect together.
  frame = 0;
  do {
    actor->y.fixed += PIXELS(1) / 2;
    Engine_EventWait(1);
    frame++;
  } while (frame != 60);
  Event_PlayCue(190);
  frame = 0;
  do {
    actor->y.fixed += PIXELS(1) / 10;
    actor->scaleX -= Q16_ONE / 100;
    actor->scaleY -= Q16_ONE / 100;
    departureEffect->scaleX -= Q16_ONE / 100;
    departureEffect->scaleY -= Q16_ONE / 100;
    Engine_EventWait(1);
    frame++;
  } while (frame != 90);
  Actor_SetPosition(actorId, 0, 0);
  Actor_SetPosition(8, 0, 0);
}

// Unused empty callback.
UNUSED static void SolSanctumEruption_UnusedCallbackA(void) {}

// Unused empty callback.
UNUSED static void SolSanctumEruption_UnusedCallbackB(void) {}

// Start the hostage scene only for arrival ten.
s32 SolSanctumEruption_Initialize(void) {
  if (gGameState.entrance == 10) {
    FieldEffect_InitSparkles();
    SolSanctumEruption_PlayHostageScene();
  }
  return 0;
}

// Keep a flying ember sprite rotated a quarter-turn from its facing.
static s32
SolSanctumEruption_AlignEmberToHeading(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  actor->sprite->rotation = actor->facing + 0x4000;
  return 1;
}

// Launch sixteen rocks outward, then reset their actors for ember emission.
static void SolSanctumEruption_ThrowEruptionRing(void) {
  struct FieldActor *actor;
  struct FieldActor *spun;
  u32 rockIndex;
  s32 angle;
  struct AnimationObject *sprite;
  s32 offsetX;
  s32 offsetZ;

  for (rockIndex = 16; rockIndex <= 31; rockIndex++) {
    actor = Object_GetById(rockIndex);
    Engine_ActorStop(rockIndex);
    Engine_ActorSetSpriteFlags(actor, 0);
    Object_SetMode(actor, 2);
    actor->sprite->image.priority = 0;
    actor->motionFlags = 0;
    actor->speed = PIXELS(8);
    actor->acceleration = PIXELS(3) / 4;
    actor->scaleX = 9 * Q16_ONE / 5;
    actor->scaleY = 9 * Q16_ONE / 5;
    actor->x.fixed = PIXELS(232);
    actor->y.fixed = PIXELS(20);
    actor->z.fixed = PIXELS(132);
  }
  Engine_AudioPlayCue(145);
  for (rockIndex = 0; rockIndex <= 15; rockIndex++) {
    spun = Object_GetById(rockIndex + 16);
    sprite = spun->sprite;
    angle = rockIndex << 12;
    sprite->rotation = angle - 0x4000;
    offsetX = Iwram_MulQ16(Engine_MathCos(angle), PIXELS(256));
    offsetZ = Iwram_MulQ16(Engine_MathSin(angle), PIXELS(256));
    Engine_ObjectSetPosition(spun, spun->x.fixed + offsetX, spun->y.fixed,
                             spun->z.fixed + offsetZ);
  }
  Engine_EventWait(20);
  Engine_ActorWaitForMove(16);
  for (rockIndex = 16; rockIndex <= 31; rockIndex++) {
    actor = Object_GetById(rockIndex);
    Engine_ActorStop(rockIndex);
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

// Reset the sixteen ember slots and their emission timers.
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

// Retire landed embers and launch the next free one at the current frequency.
static void SolSanctumEruption_StepEmbers(void) {
  struct GroundParticleWork *sparkles;
  s32 shakePixels;
  s32 shakeTimer;
  u8 emberIndex;
  struct FieldActor *landedEmber;
  struct FieldActor *ember;
  s16 angle;
  s32 x;
  s32 z;

  sparkles = gParticleWork;
  shakePixels = (u32)gEmberTimer / 10;
  if (shakePixels != 0) {
    sparkles->stopped = 0;
    Map_SetShake(shakePixels * PIXELS(1), shakePixels * PIXELS(1), Q16_ONE);
  } else {
    sparkles->stopped = 1;
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  }
  shakeTimer = gEmberTimer;
  if (shakeTimer != 0)
    gEmberTimer = shakeTimer - 3;

  for (emberIndex = 0; emberIndex < 16; emberIndex++) {
    if (gEmberState[emberIndex] != 0) {
      landedEmber = Object_GetById(emberIndex + 16);
      if (landedEmber->targetX == ACTOR_NO_TARGET &&
          landedEmber->targetZ == ACTOR_NO_TARGET) {
        gEmberState[emberIndex]++;
        if (gEmberState[emberIndex] == 2)
          Object_SetMode(landedEmber, 3);
        if (gEmberState[emberIndex] == 19) {
          landedEmber->x.fixed = 0;
          landedEmber->y.fixed = 0;
          landedEmber->z.fixed = 0;
          SET_VELOCITY(landedEmber, 0, 0, 0);
          landedEmber->targetX = ACTOR_NO_TARGET;
          landedEmber->targetY = ACTOR_NO_TARGET;
          landedEmber->targetZ = ACTOR_NO_TARGET;
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
    ember = Object_GetById(emberIndex + 16);
    if (gEmberState[emberIndex] == 0) {
      if (Engine_GameFlagIsSet(0x246) == 0)
        Engine_AudioPlayCue(246);
      gEmberState[emberIndex] = 1;
      ember->motionFlags = 0;
      ember->speed = PIXELS(8);
      ember->acceleration = PIXELS(1);
      Engine_ActorSetSpriteFlags(ember, 0);
      ember->sprite->image.priority = 1;
      Object_SetMode(ember, 2);
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

// Vary the ember emission interval and its duration.
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

// End the Wise One's palette animation and clear its attached effect.
static inline void SolSanctumEruption_FinishMaterialization(void) {
  Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 3);
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateWiseOnePalette);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_WISE_ONE, 0);
}

// Show the collapsing chamber, the remaining star and the Wise One rescue.
static void SolSanctumEruption_PlayWiseOneRescue(void) {
  struct GroundParticleWork *sparkles;
  struct FieldActor *wiseOne;
  struct FieldActor *center;
  struct FieldActor *leader;
  struct FieldActor *starOrigin;
  struct FieldActor *garet;
  struct AnimationObject *leaderSprite;
  struct AnimationObject *garetSprite;
  struct FieldActor *star;
  struct AnimationObject *starSprite;
  u8 *iconPixels;
  u8 rockIndex;
  u32 frame;
  s32 flickerDelay;

  sparkles = gParticleWork;
  wiseOne = Actor_Get(ACTOR_WISE_ONE);
  // Clear the ember actors and bring Isaac and Garet back into view.
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateEmberFrequency);
  gEmberMask = 3;
  Engine_EventWait(80);
  Event_PlayCue(17);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(40);
  Engine_TaskRemoveCallback(SolSanctumEruption_StepEmbers);
  for (rockIndex = 0; rockIndex < ROCK_COUNT; rockIndex++) {
    Actor_SetPosition(ACTOR_FIRST_ROCK + rockIndex, 0, 0);
  }
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  SolSanctumEruption_PlaceBeforeStar(ACTOR_PARTY_LEADER);
  SolSanctumEruption_PlaceBeforeStar(GARET);
  sparkles->stopped = 0;
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(80);
  SolSanctumEruption_RestoreCornerCellAttributes();
  center = Engine_EventGetViewCenter();
  center->motionFlags = 0;
  center->x.fixed = PIXELS(231);
  center->z.fixed = PIXELS(144);
  center->targetX = ACTOR_NO_TARGET;
  center->targetY = ACTOR_NO_TARGET;
  center->targetZ = ACTOR_NO_TARGET;
  center->y.fixed = 0;
  center->velocityX = 0;
  center->velocityZ = 0;
  Engine_TaskWait(4);
  Engine_MapRedraw();
  Engine_TaskWait(4);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(40);

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  garet = Actor_Get(GARET);
  leaderSprite = leader->sprite;
  garetSprite = garet->sprite;
  for (frame = 0; frame < 20; frame++) {
    leaderSprite->rotation += 0x100;
    garetSprite->rotation -= 0x100;
    leader->x.fixed += PIXELS(3) / 8;
    garet->x.fixed -= PIXELS(3) / 8;
    Engine_TaskWait(1);
  }
  Engine_EventWait(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  Actor_Get(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
  Actor_Get(GARET)->sprite->rotation = 0;
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 246, 150);
  Engine_ObjectMotionSetPositionAndCommit(GARET, 220, 150);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetSpritePriority(GARET, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Engine_EventWait(60);

  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgSolSanctumWellTurnedBadly);
  // Let the survivors discuss their missing friends and survey the damage.
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventSetMessage(MsgSolSanctumWellTurnedBadly + 1);
  } else {
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventSetMessage(MsgSolSanctumWellTurnedBadly + 2);
  }
  Event_ShowMessageAndWait(GARET, 0, 60);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Engine_EventSetMessage(MsgSolSanctumFriendsGone);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgSolSanctumThanksALot);
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgSolSanctumTheyllBeSafe);
  }
  Event_ShowMessageAndWait(GARET, 0, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);

  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  // Pan around the chamber before another tremor interrupts the conversation.
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  Camera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgSolSanctumThisIsTerrible);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);

  Event_PlayCue(23);
  SolSanctumEruption_SetMaterializationTint(1, 4, 0);
  sparkles->stopped = 0;
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Engine_EventWait(10);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 243, 144);
  Actor_SetDestination(GARET, 202, 144);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(20);
  sparkles->stopped = 1;
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(80);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkTo(GARET, 220, 150);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 150);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  } else {
    Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgSolSanctumQuitActingTough);
  }
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);

  Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(110), PIXELS(152));
  // The Wise One returns and draws the party toward the remaining star.
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_WISE_ONE, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetDestination(ACTOR_WISE_ONE, 171, 152);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), 1);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(GARET, 217, 182);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_EventWait(4);
  Engine_ActorWalkToAndWait(GARET, 231, 175);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventSetMessage(MsgSolSanctumOverHere);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Engine_EventWait(60);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);

  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  sparkles->stopped = 0;
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Event_PlayCue(0x121);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(40);
  sparkles->stopped = 1;
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(120);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(100);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  SolSanctumEruption_SetMaterializationTint(1, 10, 0);
  sparkles->stopped = 0;
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 256, 150);
  Actor_SetDestination(GARET, 231, 180);
  Engine_ActorWaitForMove(GARET);
  SolSanctumEruption_SetMaterializationTint(0, 40, 0);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
  Engine_EventWait(40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);

  Event_PlayCue(107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(10);
  // Alternate the damaged tiles while the Wise One materializes beside them.
  Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT, 0);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
  Actor_SetDestination(ACTOR_WISE_ONE, 130, 113);
  Engine_ActorWaitForMove(ACTOR_WISE_ONE);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(20);
  sparkles->stopped = 0;
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(20);
  SceneState_SetValue140Mode0();
  Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_SCENE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(30);
  Event_PlayCue(0x121);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerFirstCollapseTiles);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  flickerDelay = 16;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  sparkles->stopped = 1;
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  SolSanctumEruption_FinishMaterialization();
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
  Actor_SetSpeed(ACTOR_WISE_ONE, PIXELS(1), PIXELS(1) / 2);
  Actor_SetDestination(ACTOR_WISE_ONE, 169, 151);
  Engine_ActorWaitForMove(ACTOR_WISE_ONE);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_NORTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
  Engine_CameraWaitForMove();
  SolSanctumEruption_RaiseElementalArcs(0);
  Engine_ActorRunRepeatedMotion(ACTOR_WISE_ONE, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(40);
  Event_OpenMessage(ACTOR_WISE_ONE, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(40);
  } else {
    Engine_MessageShowCentered(MsgSolSanctumCannotResist, 1);
    Engine_EventWait(40);
  }

  starOrigin = Actor_Get(ACTOR_PARTY_LEADER);
  // Lift the Mars Star icon and pulse its scale during the Wise One's effect.
  star = Object_Create(OBJECT_ELEMENTAL_STAR, starOrigin->x.fixed,
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
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
    Engine_RunRisingObjectSequence(star, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
  }
  SolSanctumEruption_SetMaterializationTint(1, 20, 0);
  for (frame = 0; frame < 24; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
    star->scaleX = 2 * Q16_ONE / 5;
    star->scaleY = 2 * Q16_ONE / 5;
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
    star->scaleX = Q16_ONE;
    star->scaleY = Q16_ONE;
  }
  Actor_SetChildValue(ACTOR_WISE_ONE, 0);
  SolSanctumEruption_SetMaterializationTint(0, 20, 0);
  Engine_EventSetMessage(MsgSolSanctumReturnStarToBag);
  Event_ShowMessageAndWait(ACTOR_WISE_ONE, 0, 20);
  if (star != NULL) {
    Engine_ObjectDispatchRelease(star);
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 60);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(40);

  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  // Show the other three chamber positions and their rising arc effects.
  SolSanctumEruption_FadeCameraTo(232, 464);
  SolSanctumEruption_RaiseElementalArcs(1);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 144);
  SolSanctumEruption_RaiseElementalArcs(2);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  SolSanctumEruption_FadeCameraTo(711, 464);
  SolSanctumEruption_RaiseElementalArcs(3);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
  Actor_SetPosition(GARET, PIXELS(582), PIXELS(345));
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Actor_SetPosition(GARET, PIXELS(231), PIXELS(180));
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_TaskWait(20);
  SolSanctumEruption_FadeCameraTo(219, 171);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(40);

  sparkles->stopped = 0;
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(20);
  Event_PlayCue(107);
  // Repeat the collapsing tile effects as the Wise One moves across the room.
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 245, 145);
  Actor_SetDestination(GARET, 215, 168);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(80);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_WISE_ONE, 184, 87);
  Engine_ActorWaitForMove(ACTOR_WISE_ONE);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_SCENE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(30);
  Event_PlayCue(0x121);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerSecondCollapseTiles);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawSecondCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawSecondCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawSecondCollapseTiles(0);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  SolSanctumEruption_FinishMaterialization();
  Engine_EventWait(60);

  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(20);
  Event_PlayCue(107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerFirstCollapseTiles,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(40);
  Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 127, 110);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_SCENE);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Event_PlayCue(0x121);
  Engine_EventWait(30);
  Engine_TaskRemoveCallback(SolSanctumEruption_FlickerFirstCollapseTiles);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  flickerDelay = 8;
  while (flickerDelay--) {
    SolSanctumEruption_DrawFirstCollapseTiles(0);
    Engine_TaskWait(flickerDelay);
    SolSanctumEruption_DrawFirstCollapseTiles(1);
    Engine_TaskWait(flickerDelay);
  }
  SolSanctumEruption_DrawFirstCollapseTiles(0);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_PlayCue(107);
  Event_PlayCue(63);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  ColorBuffer_ApplyTarget(0x20119e, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(20);
  Event_PlayCue(107);
  FieldScheduler_AddCallback(SolSanctumEruption_FlickerSecondCollapseTiles,
                             TASK_PRIORITY_SCENE);
  SolSanctumEruption_FinishMaterialization();
  Engine_EventWait(60);
  Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 184, 87);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
  Engine_EventWait(10);
  SolSanctumEruption_FinishMaterialization();
  Event_PlayCue(141);
  Engine_EventWait(100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Event_PlayCue(0x121);
  Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  for (frame = 0; frame < 40; frame++) {
    SolSanctumEruption_FlashMaterializingActor(wiseOne);
    Engine_TaskWait(1);
  }
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateWiseOnePalette,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(20);
  ColorBuffer_ApplyTarget(0x7fff, 2);
  // Fade the chamber away, stop its effects and leave through the rescue exit.
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(100);
  ColorBuffer_ApplyTarget(0x7fff, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_TaskRemoveCallback(SolSanctumEruption_UpdateWiseOnePalette);
  sparkles->stopped = 1;
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  SceneEffect_CleanupSceneObjects();
  GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
  GameFlag_Set(FLAG_STAR_ROOM_COLLAPSED);
  Engine_EventRequestExit(5);
  GameFlag_Set(0x100);
}

// Place an actor before the star and draw it above the chamber floor.
static void SolSanctumEruption_PlaceBeforeStar(s32 actor) {
  struct FieldActor *object;

  object = Object_GetById(actor);
  Engine_ActorSetPosition(actor, PIXELS(232), PIXELS(125));
  object->facing = FACING_SOUTH;
  Engine_ActorSetSpritePriority(actor, 3);
}

// Select the first collapsing tile patch and redraw the map.
static void SolSanctumEruption_DrawFirstCollapseTiles(s32 alternate) {
  if (alternate != 0) {
    s32 cellSize;
    cellSize = 1;
    Map_CopyCellsTo(8, 47, 64, 7, cellSize, cellSize);
    Map_CopyCellsTo(7, 48, 63, 8, 2, cellSize);
    Map_CopyCellsTo(7, 49, 63, 9, 2, cellSize);
  } else {
    s32 cellSize;
    cellSize = 1;
    Map_CopyCellsTo(56, 0, 64, 7, cellSize, cellSize);
    Map_CopyCellsTo(56, 0, 63, 8, cellSize, cellSize);
    Map_CopyCellsTo(56, 0, 63, 9, 2, cellSize);
    Map_CopyCellsTo(58, 25, 64, 8, cellSize, cellSize);
  }
  Engine_MapRedraw();
}

// Select the second collapsing tile patch and redraw the map.
static void SolSanctumEruption_DrawSecondCollapseTiles(s32 alternate) {
  if (alternate != 0) {
    s32 patchSize;
    patchSize = 2;
    Map_CopyCellsTo(9, 45, 65, 5, patchSize, patchSize);
    Map_CopyCellsTo(11, 46, 67, 6, 1, patchSize);
  } else {
    s32 patchSize;
    patchSize = 2;
    Map_CopyCellsTo(89, 2, 65, 5, patchSize, patchSize);
    Map_CopyCellsTo(102, 32, 67, 6, 1, patchSize);
  }
  Engine_MapRedraw();
}

// Randomly alternate the first tile patch on even frames.
static void SolSanctumEruption_FlickerFirstCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawFirstCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawFirstCollapseTiles(0);
    }
  }
}

// Randomly alternate the second tile patch on even frames.
static void SolSanctumEruption_FlickerSecondCollapseTiles(void) {
  if ((gFrameCount & 1) == 0) {
    if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50) {
      SolSanctumEruption_DrawSecondCollapseTiles(1);
    } else {
      SolSanctumEruption_DrawSecondCollapseTiles(0);
    }
  }
}

// Stage and raise ten arc effects at one of the four chamber positions.
static void SolSanctumEruption_RaiseElementalArcs(u32 mode) {
  void Actor_ShowEmote(s32, s32, s32);
  void ColorBuffer_ApplyTarget(s32, s32);
  void Event_PlayCue(s32);

  u32 arcIndex, clear;
  s32 x, y, z;
  s32 *origin;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  for (arcIndex = 0; arcIndex <= 15; arcIndex++)
    Engine_ActorDestroy(arcIndex + 16);
  switch (mode) {
  case 0:
    ColorBuffer_ApplyTarget(0x4039d2, 1);
    break;
  case 1:
    ColorBuffer_ApplyTarget(0x4049d2, 1);
    break;
  case 2:
    ColorBuffer_ApplyTarget(0x404a4e, 1);
    break;
  case 3:
    ColorBuffer_ApplyTarget(0x403a52, 1);
    break;
  }
  Engine_ColorBufferInterpolate(60);
  Event_PlayCue(214);
  arcIndex = 0;
  clear = arcIndex;
  for (origin = &gEruption_ArcOrigins[0]; arcIndex <= 9;
       arcIndex++, origin += 2) {
    x = origin[0];
    y = origin[1];
    z = 0;
    switch (mode) {
    case 0:
      x += PIXELS(232);
      z = PIXELS(144);
      break;
    case 1:
      x += PIXELS(232);
      z = PIXELS(464);
      break;
    case 2:
      x += PIXELS(711);
      z = PIXELS(144);
      break;
    case 3:
      x += PIXELS(711);
      z = PIXELS(464);
      break;
    }
    gArcEffectTimers[arcIndex] = clear;
    actor = Engine_ObjectCreate(284, x, y, z);
    gArcEffects[arcIndex] = actor;
    actor->motionFlags = clear;
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->image.priority = 1;
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
  }
  if (mode == 0) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  }
  Engine_TaskWait(20);
  FieldScheduler_AddCallback(SolSanctumEruption_UpdateElementalArcs, 3200);
  Event_PlayCue(246);
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

// Stretch each active arc for eight frames, then lift it out of view.
static void SolSanctumEruption_UpdateElementalArcs(void) {

  u32 arcIndex;
  s32 age;
  struct FieldActor *actor;

  for (arcIndex = 0; arcIndex <= 9; arcIndex++) {
    age = gArcEffectTimers[arcIndex];
    if (age != 0) {
      actor = gArcEffects[arcIndex];
      if ((u32)age <= 8) {
        actor->scaleX += -ELEMENTAL_ARC_SCALE_X_DECREASE;
        actor->scaleY += Q16_ONE / 2;
        actor->y.fixed += PIXELS(3) / 10;
        actor->targetY += PIXELS(3) / 10;
      } else {
        actor->y.fixed += PIXELS(20);
        actor->targetY += PIXELS(20);
      }
      age = gArcEffectTimers[arcIndex] + 1;
      gArcEffectTimers[arcIndex] = age;
      if ((u32)age > 14) {
        gArcEffectTimers[arcIndex] = 0;
      }
    }
  }
}

// Fade out, move the camera and its focus actor, then fade back in.
static void SolSanctumEruption_FadeCameraTo(s32 x, s32 z) {
  struct FieldActor *center;

  center = Engine_EventGetViewCenter();
  z *= PIXELS(1);
  x *= PIXELS(1);
  Engine_CameraMoveTo(x, -1, z, 1);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  center->x.fixed = x;
  center->z.fixed = z;
  center->targetX = ACTOR_NO_TARGET;
  center->targetZ = ACTOR_NO_TARGET;
  center->velocityX = 0;
  center->velocityZ = 0;
  Engine_TaskWait(5);
  Engine_MapRedraw();
  Engine_TaskWait(5);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(30);
}

// Restore eight corner-cell attributes from the chamber templates.
static void SolSanctumEruption_RestoreCornerCellAttributes(void) {

  Map_CopyCellAttributes(14, 8, 1, 1, 10, 0);
  Map_CopyCellAttributes(14, 28, 1, 1, 11, 0);
  Map_CopyCellAttributes(44, 8, 1, 1, 12, 0);
  Map_CopyCellAttributes(44, 28, 1, 1, 13, 0);
  Map_CopyCellAttributes(13, 8, 1, 1, 14, 8);
  Map_CopyCellAttributes(13, 28, 1, 1, 14, 28);
  Map_CopyCellAttributes(43, 8, 1, 1, 44, 8);
  Map_CopyCellAttributes(43, 28, 1, 1, 44, 28);
}

// Fade the object palette into or out of the materialization tint.
static void SolSanctumEruption_SetMaterializationTint(s32 enabled, s32 frames,
                                                      s32 waitFrames) {
  if (enabled == 1) {
    Engine_AudioPlayCue(0x134);
    Engine_ColorBufferApplyTarget(0x203a52, 1);
  } else {
    Engine_AudioPlayCue(0x121);
    Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  }
  Engine_ColorBufferInterpolate(frames);
  if (waitFrames != 0) {
    Engine_EventWait(waitFrames);
  }
}

// Flash the palette and spawn paired vertical effects every sixteen frames.
static void
SolSanctumEruption_FlashMaterializingActor(struct FieldActor *actor) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(actor);
  }
}

// Cycle six palettes while spawning the actor's vertical effects.
static void SolSanctumEruption_CycleActorPalette(struct FieldActor *actor) {
  if (gFrameCount & 1) {
    Engine_ObjectSetPartPalettes(actor,
                                 Math_RemainderUnsigned(gFrameCount >> 1, 6));
  }
  if ((gFrameCount & 15) == 0) {
    SolSanctumEruption_SpawnEffectPair(actor);
  }
}

// Shared source is included here in ROM order.
#include "common/effect_vertical.h"

// Create two vertical effects sharing the parent position and cached tiles.
static void SolSanctumEruption_SpawnEffectPair(struct FieldActor *parent) {
  struct FieldActor *pair[2];
  struct FieldActor *child;
  struct AnimationObject *sprite;
  struct PairWork *work = gEffectWork;
  s32 effectIndex;

  Engine_AudioPlayCue(292);
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
        AnimationObjects_SelectAnimation(sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = work->vramBlock;
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
  pair[0]->update = Effect_AnimateVerticalNegative;
  pair[0]->sprite->image.priority = Object_GetById(15)->sprite->image.priority;
  {
    struct FieldActor *wiseOne = Object_GetById(15);
    struct FieldActor *risingEffect = pair[1];

    risingEffect->sprite->image.priority = wiseOne->sprite->image.priority;
    risingEffect->update = Effect_AnimateVerticalPositive;
    risingEffect->priorityFlags = 2;
  }
}

// Shared source is included here in ROM order.
#include "common/effect_psynergy.h"

// Advance the Wise One palette cycle.
static void SolSanctumEruption_UpdateWiseOnePalette(void) {
  SolSanctumEruption_CycleActorPalette(Object_GetById(ACTOR_WISE_ONE));
}

// Shared source is included here in ROM order.
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

struct SceneEntrance gSolSanctumEruption_Entrances[] = {
    { 0, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSolSanctumEruption_Exits[] = {
    SCENE_EXITS(18),
    SCENE_EXIT(5, 11, 8),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumEruption_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x20, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x21, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x22, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x23, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x2b, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xfc, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolSanctumEruption_Events[] = {
    { SCENE_EVENTS_END },
};

u16 gSolSanctumEruption_StarCells[] = {
    MAP_CELL_STEP(59, 40, 3, 4, 6), MAP_CELL_STEP(62, 40, 3, 4, 6), MAP_CELL_STEP(65, 40, 3, 4, 6), MAP_CELL_STEP(68, 40, 3, 4, 6), MAP_CELL_STEP(71, 40, 3, 4, 6), MAP_CELL_STEP(74, 40, 3, 4, 6), MAP_CELL_STEP(77, 40, 3, 4, 6), MAP_CELL_STEP(80, 40, 3, 4, 6), MAP_CELL_STEP(83, 40, 3, 4, 0), MAP_CELL_STEPS_END,
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
