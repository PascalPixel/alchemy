#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/sol_sanctum_elemental_star_room.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/sol_sanctum_elemental_star_ring_setup.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "overlay_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_unit.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "glyph.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"

// Forward declarations.
static void SolSanctumElementalStarRoom_BagJupiterStar(void);
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void);
static void SolSanctumElementalStarRoom_DemandElementalStars(void);
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void);
static void SolSanctumElementalStarRoom_UnmaskFelix(void);
static void SolSanctumElementalStarRoom_AlexTakesStars(void);
static void SolSanctumElementalStarRoom_LineUpFollowers(void);
static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames);
static s32 SolSanctumElementalStarRoom_PresentStar(s32 item, s32 x, s32 y,
                                                   s32 z);
static void SolSanctumElementalStarRoom_GiveMythrilBags(void);
static void SolSanctumElementalStarRoom_AnimateAlexTeleport(void);

enum StarRoomActor {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED + 1,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  ACTOR_FELIX_MASKED,
  ACTOR_FELIX,
  ACTOR_ALEX
};

enum { ITEM_MYTHRIL_BAG = 224 };

#define NULL ((void *)0)

struct ObjectRuntime;

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Advance through the current dialogue's consecutive messages.
static __inline__ void SolSanctumElementalStarRoom_SkipMessages(s32 amount) {
  gEventWork->message += amount;
}

// Shared source is included here in ROM order.
#include "common/object_facing.h"

// Return the Elemental Star room entrances.
struct SceneEntrance *SolSanctumElementalStarRoom_GetEntrances(void) {
  return gSolSanctumElementalStarRoom_Entrances;
}

// This room has no additional scene regions.
struct SceneRegion *SolSanctumElementalStarRoom_GetRegions(void) {
  return NULL;
}

// Return the room's exits.
u32 *SolSanctumElementalStarRoom_GetExits(void) {
  return gSolSanctumElementalStarRoom_Exits;
}

// Return the room's actor placements.
struct ScenePlacement *SolSanctumElementalStarRoom_GetPlacements(void) {
  return gSolSanctumElementalStarRoom_Placements;
}

// Return the star, dialogue, and boundary interactions.
struct SceneEvent *SolSanctumElementalStarRoom_GetEvents(void) {
  return gSolSanctumElementalStarRoom_Events;
}

// Bag the Venus Star and reveal its pedestal crossings.
static void SolSanctumElementalStarRoom_CollectVenusStar(void) {
  u32 i;
  s32 itemOwner;
  s32 message;

  Engine_EventBegin();
  // Flash the room before revealing the crossings for this star.
  Engine_AudioPlayCue(SE_141);
  for (i = 0; i != 6; i++) {
    Engine_ColorBufferApplyTarget(0x403a52, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    if (i == 1) {
      Engine_WorkSetValuesIfNonNegative(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(30);
  Engine_CameraSetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(664), -1, PIXELS(497), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_AudioPlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 96, 29);
  Map_CopyCellAttributes(0, 0, 1, 1, 41, 29);
  Map_CopyCellsTo(87, 42, 41, 31, 1, 2);
  Engine_EventWait(40);
  Engine_WorkSetValuesIfNonNegative(0, 0, 0);
  Engine_CameraSetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(311), -1, PIXELS(497), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 74, 29);
  Map_CopyCellAttributes(0, 0, 1, 1, 19, 29);
  Map_CopyCellsTo(87, 42, 19, 31, 1, 2);
  Engine_EventWait(40);
  Engine_WorkSetValuesIfNonNegative(0, 0, 0);
  Camera_MoveTo(PIXELS(663), -1, PIXELS(192), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Engine_AudioPlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 96, 10);
  Map_CopyCellAttributes(0, 0, 1, 1, 41, 10);
  Map_CopyCellsTo(87, 42, 41, 12, 1, 2);
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(710), -1, PIXELS(474), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(20);
  Map_CopyCellsTo(0, 40, 43, 66, 3, 3);
  Engine_EventWait(20);
  // Present the collected star, then return to Kraden's explanation.
  itemOwner = SolSanctumElementalStarRoom_PresentStar(220, PIXELS(712),
                                                      PIXELS(16), PIXELS(464));
  Engine_EventWait(40);
  UiWork_PushValueSlot(itemOwner, 1);
  message = MsgSolSanctumVenusStarBagged;
  Engine_MessageShowCentered(message, 1);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(462), -1, PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorJump(9, 4, 30);
  Engine_EventSetMessage(message - 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_CameraMoveTo(PIXELS(710), -1, PIXELS(474), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  Engine_EventEnd();
}

// Bag the Mercury Star and reveal its pedestal crossings.
static void SolSanctumElementalStarRoom_CollectMercuryStar(void) {
  u32 i;
  s32 itemOwner;
  s32 message;

  Engine_EventBegin();
  Event_PlayCue(SE_141);
  for (i = 0; i != 6; i++) {
    ColorBuffer_ApplyTarget(0x404a4e, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    if (i == 1) {
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(30);
  Map_SetShake(0, 0, 0);
  Camera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  Camera_MoveTo(PIXELS(472), -1, PIXELS(98), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 84, 4);
  Map_CopyCellAttributes(0, 0, 1, 1, 29, 4);
  Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
  Engine_EventWait(40);
  Map_SetShake(0, 0, 0);
  Camera_MoveTo(PIXELS(343), -1, PIXELS(369), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 76, 21);
  Map_CopyCellAttributes(0, 0, 1, 1, 21, 21);
  Map_CopyCellsTo(87, 42, 21, 23, 1, 2);
  Engine_EventWait(40);
  Map_SetShake(0, 0, 0);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(343), -1, PIXELS(497), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 76, 29);
  Map_CopyCellAttributes(0, 0, 1, 1, 21, 29);
  Map_CopyCellsTo(87, 42, 21, 31, 1, 2);
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(712), -1, PIXELS(152), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(20);
  Map_CopyCellsTo(0, 40, 43, 46, 3, 3);
  Engine_EventWait(20);
  // Present the collected star, then return to Kraden's explanation.
  itemOwner = SolSanctumElementalStarRoom_PresentStar(221, PIXELS(712),
                                                      PIXELS(16), PIXELS(144));
  Engine_EventWait(40);
  UiWork_PushValueSlot(itemOwner, 1);
  message = MsgSolSanctumPutMercuryStar;
  Engine_MessageShowCentered(message, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(462), -1, PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_KRADEN, 4, 30);
  Engine_EventSetMessage(message - 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(712), -1, PIXELS(152), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  Engine_EventEnd();
}

// Bag Jupiter, meet the intruders, and surrender the collected stars.
static void
SolSanctumElementalStarRoom_CollectJupiterStarAndMeetIntruders(void) {
  Engine_EventBegin();
  SolSanctumElementalStarRoom_BagJupiterStar();
  SolSanctumElementalStarRoom_RevealSaturosAndMenardi();
  SolSanctumElementalStarRoom_DemandElementalStars();
  SolSanctumElementalStarRoom_OfferHostageGuarantee();
  SolSanctumElementalStarRoom_UnmaskFelix();
  SolSanctumElementalStarRoom_AlexTakesStars();
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Engine_EventEnd();
  SolSanctumElementalStarRoom_LineUpFollowers();
}

// Reveal the Jupiter crossings and place the star in a mythril bag.
static void SolSanctumElementalStarRoom_BagJupiterStar(void) {
  u32 i;
  s32 itemOwner;

  Event_PlayCue(SE_141);
  for (i = 0; i != 6; i++) {
    ColorBuffer_ApplyTarget(0x4049d2, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    if (i == 1) {
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Engine_EventWait(30);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(167), -1, PIXELS(529), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 65, 31);
  Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
  Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
  Engine_EventWait(40);
  Map_SetShake(0, 0, 0);
  Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(391), -1, PIXELS(177), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 79, 9);
  Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
  Map_CopyCellsTo(87, 42, 24, 11, 1, 2);
  Engine_EventWait(40);
  Map_SetShake(0, 0, 0);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(583), -1, PIXELS(193), 1);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Event_PlayCue(SE_144);
  Engine_MapAnimateCells(gSolSanctumElementalStarRoom_StarCells, 91, 10);
  Map_CopyCellAttributes(0, 0, 1, 1, 36, 10);
  Map_CopyCellsTo(87, 42, 36, 12, 1, 2);
  Engine_EventWait(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(232), -1, PIXELS(477), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(20);
  Map_CopyCellsTo(0, 40, 13, 66, 3, 3);
  Engine_EventWait(20);
  itemOwner = SolSanctumElementalStarRoom_PresentStar(223, PIXELS(232),
                                                      PIXELS(16), PIXELS(464));
  Engine_EventWait(40);
  UiWork_PushValueSlot(itemOwner, 1);
  Engine_MessageShowCentered(MsgSolSanctumJupiterStarBagged, 1);
}

// Bring Isaac and Garet back to discover Jenna and Kraden's captors.
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void) {
  struct FieldActor *leader;

  Event_PlayCue(MUS_017);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 231, 490);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(180);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 479);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(GARET, 257, 491);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 265, 453);
  Actor_WalkToAndWait(GARET, 282, 469);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 60);
  Actor_SetPosition(JENNA, PIXELS(475), PIXELS(332));
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(491), PIXELS(332));
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(459), PIXELS(348));
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(507), PIXELS(348));
  Camera_SetSpeed(PIXELS(36) / 5, PIXELS(9) / 10);
  Camera_MoveTo(PIXELS(485), -1, PIXELS(345), 1);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
}

// Reveal the masked intruder while the captors demand the stars.
static void SolSanctumElementalStarRoom_DemandElementalStars(void) {
  u8 *motionFlags;
  u8 *spriteFlags;
  struct FieldActor *felix;
  struct FieldActor *glow;
  s32 i;
  u8 zero;

  Engine_AudioPlayCue(MUS_061);
  Engine_ActorSetAnimation(10, 4);
  Engine_EventSetMessage(MsgSolSanctumLooksLikeTheyveSpottedUs);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Engine_ActorSetAnimation(11, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 30);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorJump(9, 4, 10);
  Engine_ActorJump(9, 6, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Engine_ActorRunRepeatedMotion(10, 1);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 20);
  Engine_ActorRunRepeatedMotion(11, 1);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 30);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorFaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_SOUTHWEST + FACING_STEP, 80);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | 2, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_ActorSetAnimation(9, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  felix = Object_GetById(12);
  glow = Object_GetById(8);
  spriteFlags = &felix->sprite->flags;
  zero = 0;
  *spriteFlags = zero;
  felix->scaleX = Q16_ONE / 10;
  felix->scaleY = Q16_ONE / 10;
  glow->scaleX = Q16_ONE / 10;
  glow->scaleY = Q16_ONE / 10;
  Actor_SetChildValue(12, 0x100);
  Actor_SetPosition(12, PIXELS(471), PIXELS(290));
  motionFlags = &felix->motionFlags;
  *motionFlags = zero;
  felix->y.fixed = PIXELS(40);
  Engine_EventWait(1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(12, 10);
  Actor_ShowEmote(5, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 30);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetPosition(8, PIXELS(471), PIXELS(290));
  Event_PlayCue(SE_190);
  Engine_ActorSetSpritePriority(12, 2);
  for (i = 0; i != 90; i++) {
    felix->y.fixed -= PIXELS(1) / 10;
    felix->scaleX += Q16_ONE / 100;
    felix->scaleY += Q16_ONE / 100;
    glow->scaleX += Q16_ONE / 100;
    glow->scaleY += Q16_ONE / 100;
    Engine_EventWait(1);
  }
  *motionFlags = 5;
  Engine_EventWait(80);
  for (i = 0; i != 60; i++) {
    felix->y.fixed -= PIXELS(1) / 2;
    Engine_EventWait(1);
  }
  *motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Engine_EventWait(30);
  *spriteFlags = 1;
  Engine_ActorSetPosition(8, 0, 0);
  Engine_ActorSetSpritePriority(12, 1);
  {
    u8 *flags = &Object_GetById(12)->priorityFlags;

    /* FAKEMATCH: the bit held in the loop counter itemSlot keeps the 1 in r5
     * ahead of the actor 8 call's arguments; a plain |= loads it after. Plain
     * |=: differ 31, one instruction shorter; *flags = ACTOR_PRIORITY_AUTOMATIC
     * | *flags, either operand itemSlot, differ 35. */
    i = ACTOR_PRIORITY_AUTOMATIC;
    i |= *flags;
    *flags = i;
  }
  Engine_ActorSetChildValue(12, 0);
  Actor_SetSpeed(12, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(12, 471, 306);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(12, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x400c, 20);
  Engine_ActorFaceEachOther(5, 9, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(10, 1);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(10, 4);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 30);
  Engine_ActorSetAttachedEffect(12, 0x102);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 30);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 30);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(11, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(10, 3);
  Engine_EventWait(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(264), -1, PIXELS(460), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 40);
  Engine_ActorStartRepeatedMotion(0, 3);
  Engine_ActorRunRepeatedMotion(1, 3);
  Engine_EventWait(80);
  Engine_EventOpenMessage(11, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    SolSanctumElementalStarRoom_SkipMessages(1);
  }
  BattlePres_RunActionThenWaitIfModeZero(9, 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_CameraMoveTo(PIXELS(477), -1, PIXELS(334), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_EventSetMessage(MsgSolSanctumWhyDenyDont);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 20);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(9, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x5009, 40);
  Engine_ActorRunRepeatedMotion(11, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 10);
  Engine_ActorStartRepeatedMotion(5, 2);
  Engine_ActorRunRepeatedMotion(9, 2);
}

// Have the captors offer a guarantee for their hostages' safety.
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void) {
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED, 0x102);
  Engine_EventWait(60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(12, 10);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(JENNA, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX_MASKED, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED, 0x102);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Engine_EventWait(40);
  Actor_SetPosition(GARET, PIXELS(346), PIXELS(440));
  Engine_TaskWait(1);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Actor_SetPosition(GARET, PIXELS(280), PIXELS(470));
  Camera_MoveTo(PIXELS(261), -1, PIXELS(466), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgSolSanctumMenardiDontYouWantThem);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(477), -1, PIXELS(334), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 20);
  Actor_FaceDirection(JENNA, FACING_EAST, 40);
  Engine_ActorSetAnimationAndWait(JENNA, 4);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Engine_EventWait(80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(12, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX_MASKED, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(12, 20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_NORTHWEST + FACING_STEP, 40);
}

// Reveal Felix's identity and let Isaac decide how to hand over the stars.
static void SolSanctumElementalStarRoom_UnmaskFelix(void) {
  struct FieldActor *actor;
  struct FieldActor *other;
  u16 *inventoryEntry;
  s32 remainingSlots;
  s32 starCount;
  s32 messageA;
  s32 messageB;

  Event_PlayCue(SE_161);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Engine_EventWait(40);
  other = Object_GetById(ACTOR_FELIX_MASKED);
  if (other != 0) {
    Actor_SetPosition(ACTOR_FELIX, other->x.fixed, other->z.fixed);
  }
  Actor_SetPosition(ACTOR_FELIX_MASKED, 0, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(JENNA, 3);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 40);
  Engine_ActorSetAnimationAndWait(JENNA, 3);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(13, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 3);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(13, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 80);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(13, 40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(JENNA, 4);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 80);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(13, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 5, 80);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(11, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(261), -1, PIXELS(466), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 244, 478);
  Actor_WalkToAndWait(GARET, 260, 490);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  starCount = 0;
  inventoryEntry = UnitGet(1)->inventory;
  remainingSlots = 14;
  do {
    u32 item = *inventoryEntry & 0x1ff;
    inventoryEntry++;
    if (item == 220 || item == 221 || item == 223)
      starCount++;
    remainingSlots--;
  } while (remainingSlots >= 0);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    messageA = MsgSolSanctumGuessTakeElemental;
    Engine_EventSetMessage(messageA);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(10);
    if (starCount <= 2) {
      SolSanctumElementalStarRoom_ShowMessageAndWait(1, 30);
      Actor_WalkToAndWait(GARET, 252, 486);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(10);
      UiText_ShowCenteredMessage((messageA + 1), 1, 0);
    } else {
      Engine_EventSetMessage(MsgSolSanctumRightTake);
      SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 30);
    }
  } else {
    if (starCount <= 2) {
      messageB = MsgSolSanctumDontHandOver;
      Engine_EventSetMessage(messageB);
      Engine_ActorRunRepeatedMotion(GARET, 3);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(1, 10);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Engine_ActorRunRepeatedMotion(GARET, 1);
      Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      actor->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(GARET, 244, 478);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
      Actor_SetDestination(ACTOR_PARTY_LEADER, 218, 471);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      UiText_ShowCenteredMessage((messageB + 1), 1, 0);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
      actor->steeringFlags |= ACTOR_FACES_MOTION;
    } else {
      Engine_EventSetMessage(MsgSolSanctumWontLetGo);
      Engine_ActorRunRepeatedMotion(GARET, 3);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(1, 10);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
    }
  }
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Engine_CameraFollowActor(GARET, 1);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  actor = Object_GetById(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(GARET, 264, 482);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(GARET, 278, 480);
  actor->speed = PIXELS(3);
  actor->acceleration = PIXELS(2);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 312, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 342, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 376, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
}

// Have Alex collect the stars and return the party to the final pedestal.
static void SolSanctumElementalStarRoom_AlexTakesStars(void) {
  u32 i;
  struct FieldActor *actor;
  struct FieldActor *alex;
  u8 none;

  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  alex = Actor_Get(ACTOR_ALEX);
  Engine_ActorSetSpriteFlags(alex, 0);
  Actor_SetChildValue(ACTOR_ALEX, 15);
  Actor_SetPosition(ACTOR_ALEX, PIXELS(392), PIXELS(454));
  SolSanctumElementalStarRoom_AnimateAlexTeleport();
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSolSanctumPermitRelieveElemental);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(469), PIXELS(348));
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x200a, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x200a, 40);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(507), PIXELS(348));
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(GARET, 389, 468);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 20);
  UiText_ShowCenteredMessage(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  actor = Object_GetById(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  none = 0;
  Actor_WalkToAndWait(GARET, 376, 470);
  Engine_EventWait(30);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgSolSanctumPermitRelieveElemental + 5);
  SolSanctumElementalStarRoom_ShowMessageAndWait(14, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(14, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 20);
  Actor_SetChildValue(ACTOR_ALEX, 0x100);
  alex = Actor_Get(ACTOR_ALEX);
  Engine_ActorSetSpriteFlags(alex, 0);
  actor = Object_GetById(ACTOR_ALEX);
  actor->motionFlags = none;
  Event_PlayCue(SE_220);
  for (i = 0; i != 30; i++) {
    actor->y.fixed += PIXELS(1);
    Engine_EventWait(1);
  }
  actor->motionFlags = 5;
  Engine_ActorStartRepeatedMotion(GARET, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(14, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | 5, 80);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(477), -1, PIXELS(334), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(10, 10);
  Event_OpenMessage(ACTOR_MENARDI, 0);
  Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(374), -1, PIXELS(470), 1);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  if (Engine_EventChooseYesNo(1, 0) != 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventSetMessage(MsgSolSanctumDontWantAnything);
    Event_OpenMessage(ACTOR_ALEX, 0);
    if (Engine_EventChooseYesNo(1, 0) == 0) {
      do {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(14, 4);
        Engine_EventWait(10);
        Engine_EventSetMessage(MsgSolSanctumDoubtHowFeel);
        Event_OpenMessage(ACTOR_ALEX, 0);
      } while (Engine_EventChooseYesNo(1, 0) == 0);
    }
  }
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSolSanctumThankCooperation);
  SolSanctumElementalStarRoom_ShowMessageAndWait(14, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(14, 30);
  actor->motionFlags = 0;
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(12) / 5, PIXELS(6) / 5);
  FieldObject_SetPosition(actor, PIXELS(460), 0, PIXELS(360));
  Engine_ActorWaitForMove(ACTOR_ALEX);
  Actor_SetChildValue(ACTOR_ALEX, 0);
  alex = Actor_Get(ACTOR_ALEX);
  Engine_ActorSetSpriteFlags(alex, 1);
  Engine_EventWait(30);
  Engine_CameraFollowActor(GARET, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(20);
  actor = Object_GetById(GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->speed = PIXELS(3);
  actor->acceleration = PIXELS(2);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 342, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 312, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_153);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 278, 480);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  PartyInventory_Discard(220);
  PartyInventory_Discard(221);
  PartyInventory_Discard(223);
}

// Arrange the captors and hostages after the exchange.
static void SolSanctumElementalStarRoom_LineUpFollowers(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  struct FieldActor *alex;

  leader = Object_GetById(0);
  Engine_EventBegin();
  Actor_SetBuiltinAction(5, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(9, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(11, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(10, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(14, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(13, ACTOR_ACTION_IDLE);
  Actor_SetPosition(5, PIXELS(475), PIXELS(332));
  Actor_SetPosition(9, PIXELS(491), PIXELS(332));
  Actor_SetPosition(11, PIXELS(459), PIXELS(348));
  Actor_SetPosition(10, PIXELS(507), PIXELS(348));
  Actor_SetPosition(14, PIXELS(460), PIXELS(360));
  Actor_SetPosition(13, PIXELS(471), PIXELS(306));
  actor = Object_GetById(5);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(9);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(11);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(10);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  alex = Object_GetById(14);
  alex->linkedObject = leader;
  alex->steeringFlags |= 1;
  alex->scaleX = Q16_ONE;
  alex->scaleY = Q16_ONE;
  alex->motionFlags = Object_GetById(11)->motionFlags;
  alex->y.fixed = 0;
  Engine_ObjectSetScript(alex, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(13);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  Engine_EventEnd();
}

// Bag the final star and escape as the room starts shaking.
static void SolSanctumElementalStarRoom_CollectMarsStar(void) {
  u8 pass;
  s32 itemOwner;
  struct FieldActor *leader;

  Engine_EventBegin();
  Event_PlayCue(SE_141);
  for (pass = 0; pass != 6; pass++) {
    ColorBuffer_ApplyTarget(0x4039d2, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(8);
    Engine_EventWait(8);
    if (pass == 1) {
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Map_CopyCellsTo(0, 40, 13, 46, 3, 3);
  Engine_EventWait(20);
  itemOwner = SolSanctumElementalStarRoom_PresentStar(222, PIXELS(232),
                                                      PIXELS(16), PIXELS(144));
  Engine_EventWait(40);
  UiWork_PushValueSlot(itemOwner, 1);
  Engine_MessageShowCentered(MsgSolSanctumMarsStarBagged, 1);
  Actor_SetPosition(JENNA, PIXELS(307), PIXELS(277));
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(307), PIXELS(277));
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(307), PIXELS(277));
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(307), PIXELS(277));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(307), PIXELS(277));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
  Engine_EventWait(10);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(GARET, 218, 172);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_PlayCue(SE_145);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Engine_EventWait(20);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 50);
  Event_PlayCue(SE_144);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 50);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Event_PlayCue(SE_144);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(30);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_ActorJump(GARET, 2, 20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(2);
}

// Let Kraden comment on the remaining stars or the hostage exchange.
static void SolSanctumElementalStarRoom_TalkToKraden(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    Engine_EventSetMessage(MsgSolSanctumKradenIAmResponsible);
    Engine_EventShowMessage(ACTOR_KRADEN, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      Engine_EventSetMessage(MsgSolSanctumKradenBringTheStarsHere);
    else
      Engine_EventSetMessage(MsgSolSanctumKradenFetchTheOthers);
    Engine_ActorFaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventShowMessage(ACTOR_KRADEN, 0);
  }
  Engine_EventEnd();
}

// Let Jenna comment on collecting the stars or finding Felix alive.
static void SolSanctumElementalStarRoom_TalkToJenna(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    Engine_EventSetMessage(MsgSolSanctumJennaMyBrotherIsAlive);
    Engine_EventShowMessage(JENNA, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      Engine_EventSetMessage(MsgSolSanctumJennaBeCareful);
    else
      Engine_EventSetMessage(MsgSolSanctumJennaKradenIsGiddy);
    Engine_ActorFaceEachOther(JENNA, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventShowMessage(JENNA, 0);
  }
  Engine_EventEnd();
}

// Have Saturos demand the final star.
static void SolSanctumElementalStarRoom_TalkToSaturos(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumSaturosBringTheFinalStar);
  Engine_EventShowMessage(ACTOR_SATUROS, 0);
  Engine_EventEnd();
}

// Have Menardi demand the final gem.
static void SolSanctumElementalStarRoom_TalkToMenardi(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumMenardiBringTheFinalGem);
  Engine_EventShowMessage(ACTOR_MENARDI, 0);
  Engine_EventEnd();
}

// Have Felix remain silent.
static void SolSanctumElementalStarRoom_TalkToFelix(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumFelixSilence);
  Engine_EventShowMessage(ACTOR_FELIX, 0);
  Engine_EventEnd();
}

// Have Alex remind Isaac that one star remains.
static void SolSanctumElementalStarRoom_TalkToAlex(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumAlexOnlyOneLeft);
  Engine_EventShowMessage(ACTOR_ALEX, 0);
  Engine_EventEnd();
}

// Have Kraden remind Isaac that the stars are inside this room.
static void
SolSanctumElementalStarRoom_PreventLeavingBeforeCollectingStars(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumNoStarsOutThere);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Engine_EventEnd();
}

// Perform the ordinary interaction in front of the leader.
static void SolSanctumElementalStarRoom_CheckLeaderAhead(void) {
  Engine_LeaderCheckAhead();
}

// Show a pedestal while Kraden describes it, then return to the party.
static inline void
SolSanctumElementalStarRoom_ShowPedestal(s32 cameraX, s32 cameraZ, s32 actorX,
                                         s32 actorZ, s32 speaker) {
  Camera_MoveTo(cameraX, -1, cameraZ, 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(ACTOR_KRADEN, actorX << 16, actorZ << 16);
  Engine_EventWait(40);
  Event_ShowMessage(speaker, 0);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(471), PIXELS(360));
  Engine_EventWait(80);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(370), 1);
  Engine_CameraWaitForMove();
}

// Introduce the room, explain the Elemental Stars, and give out mythril bags.
static void SolSanctumElementalStarRoom_EnterStarRoom(void) {
  u16 i;
  u8 *leaderMotion;
  u8 *kradenMotion;
  u8 *leaderSprite;
  u8 *kradenSprite;
  struct FieldActor *rising;
  struct FieldActor *glow;
  struct FieldActor *leader;
  u8 *kradenSteeringFlags;
  s32 kradenSpeaker;
  s32 height;
  s32 kradenMessageSpeaker;
  s32 zero;

  Engine_EventBegin();
  Map_CopyCellAttributes(27, 16, 5, 1, 27, 17);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(261), 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  // Materialize Isaac above the arrival platform with the expanding glow.
  glow = Actor_Get(8);
  glow->scaleX = Q16_ONE / 10;
  glow->scaleY = Q16_ONE / 10;
  rising = Actor_Get(ACTOR_PARTY_LEADER);
  leaderSprite = &rising->sprite->flags;
  *leaderSprite = 0;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(471), PIXELS(290));
  leaderMotion = &rising->motionFlags;
  *leaderMotion = 0;
  rising->y.fixed = PIXELS(40);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(8, PIXELS(471), PIXELS(290));
  Event_PlayCue(SE_190);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    glow->scaleX += Q16_ONE / 100;
    glow->scaleY += Q16_ONE / 100;
    Engine_EventWait(1);
  }
  *leaderMotion = 5;
  Engine_EventWait(80);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(290), 1);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    Engine_EventWait(1);
  }
  *leaderMotion = 3;
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  {
    struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
    s32 flags = 1 | actor->priorityFlags;
    actor->priorityFlags = flags;
  }
  // Finish Isaac's arrival and let the companions inspect the room.
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  {
    s32 shown = 1;

    *leaderSprite = shown;
  }
  Engine_ActorSetPosition(8, 0, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 471, 310);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  zero = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(JENNA, 453, 302);
  Actor_WalkToAndWait(GARET, 489, 302);
  Engine_ActorSetAnimation(JENNA, 1);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_ActorJump(GARET, 2, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(JENNA, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 471, 346);
  Actor_WalkTo(JENNA, 431, 338);
  Actor_WalkTo(GARET, 511, 338);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorWaitForMove(JENNA);
  Engine_ActorSetAnimation(JENNA, 1);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 60);
  // Materialize Kraden after the companions have explored the chamber.
  rising = Actor_Get(ACTOR_KRADEN);
  kradenSprite = &rising->sprite->flags;
  *kradenSprite = zero;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  glow->scaleX = Q16_ONE / 10;
  glow->scaleY = Q16_ONE / 10;
  Actor_SetChildValue(ACTOR_KRADEN, 0x100);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(471), PIXELS(290));
  kradenMotion = &rising->motionFlags;
  *kradenMotion = zero;
  rising->y.fixed = PIXELS(40);
  Engine_EventWait(1);
  Engine_EventSetMessage(MsgSolSanctumKradenWhereAreWe);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorJump(JENNA, 4, 0);
  Engine_ActorJump(GARET, 4, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(8, PIXELS(471), PIXELS(290));
  Event_PlayCue(SE_190);
  Engine_ActorSetSpritePriority(ACTOR_KRADEN, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    glow->scaleX += Q16_ONE / 100;
    glow->scaleY += Q16_ONE / 100;
    Engine_EventWait(1);
  }
  *kradenMotion = 5;
  Engine_EventWait(80);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    Engine_EventWait(1);
  }
  *kradenMotion = 3;
  Engine_EventWait(30);
  Engine_ActorSetSpritePriority(ACTOR_KRADEN, 1);
  {
    struct FieldActor *actor = Actor_Get(ACTOR_KRADEN);
    s32 flags = 1 | actor->priorityFlags;
    actor->priorityFlags = flags;
  }
  Actor_SetChildValue(ACTOR_KRADEN, 0);
  {
    s32 shown = 1;

    *kradenSprite = shown;
  }
  Actor_SetPosition(8, 0, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 306);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
  Engine_ActorJump(ACTOR_KRADEN, 2, 80);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 20);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 423, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(40);
  Actor_WalkToAndWait(ACTOR_KRADEN, 519, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  kradenSpeaker = 0x4009;
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 30);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(533), -1, PIXELS(339), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 533, 339);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(30);
  Engine_CameraFollowActor(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(ACTOR_KRADEN, 455, 360);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 455, 360);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 360);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 30);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(JENNA, 440, 346);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 495, 346);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 10);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  // Kraden explains the elements and the stars at the room's four corners.
  Engine_EventSetMessage(MsgSolSanctumKradenTheElementsAreThe);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  Engine_ActorStop(ACTOR_KRADEN);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(JENNA);
  Engine_ActorStop(GARET);
  Camera_SetSpeed(PIXELS(8), PIXELS(1));
  // Tour the four pedestals, returning the view to the party after each.
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(711), PIXELS(464), 589, 353,
                                           0x1009);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(711), PIXELS(147), 596, 238,
                                           0x1009);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(JENNA, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(231), PIXELS(147), 308, 250,
                                           0x2009);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(231), PIXELS(464), 306, 362,
                                           0x2009);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 4, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 40);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0xa009, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0xa009, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 4);
  Event_OpenMessage(0x8009, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
  // Discuss the Wisdom Stone and the power stored in the Elemental Stars.
  Engine_EventSetMessage(MsgSolSanctumKradenTheWisdomStone);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 6, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 6, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 6, 60);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Event_OpenMessage(0x8009, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 40);
  Engine_EventSetMessage(MsgSolSanctumTheStarsHaveEnormousPower);
  Engine_ActorSetAnimation(JENNA, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0xa009, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(JENNA, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 120);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 0);
  Engine_ActorJump(GARET, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 10);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(JENNA, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 40);
  rising = Actor_Get(ACTOR_KRADEN);
  // Kraden demonstrates reaching the platforms, then asks the party to help.
  height = 192;
  Engine_TaskWait(6);
  rising->speed = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Event_PlayCue(SE_153);
  rising->velocityY = (height << 11);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 395);
  Engine_TaskWait(6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
  kradenSteeringFlags = &rising->steeringFlags;
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_KRADEN, 473, 395);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 469, 395);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 471, 395);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 411);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_KRADEN, 474, 411);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetDestination(ACTOR_KRADEN, 468, 411);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetDestination(ACTOR_KRADEN, 471, 411);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(9, 10);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 395);
  Actor_SetBuiltinAction(ACTOR_KRADEN, ACTOR_ACTION_IDLE);
  Engine_EventWait(30);
  {
    s32 flags = 1 | *kradenSteeringFlags;
    *kradenSteeringFlags = flags;
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  Engine_TaskWait(6);
  rising->speed = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Event_PlayCue(SE_153);
  rising->velocityY = (height << 11);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 360);
  Engine_TaskWait(6);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 7, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0xa009, 30);
  Actor_FaceDirection(JENNA, FACING_EAST, 30);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 6, 60);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(0x8009, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(JENNA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_EAST, 30);
  Event_OpenMessage(JENNA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  // Jenna keeps asking until Isaac agrees to collect the stars.
  while (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventSetMessage(MsgSolSanctumJennaWontYouPlease);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 7, 0);
    Engine_ActorJump(JENNA, 4, 60);
    Event_OpenMessage(JENNA, 0);
  }
  Engine_EventSetMessage(MsgSolSanctumJennaThankYou);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(JENNA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 352);
  Engine_EventWait(20);
  kradenMessageSpeaker = 0x8009;
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 60);
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_KRADEN, 456, 360);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSolSanctumTheBagsAreFromKradens);
  SolSanctumElementalStarRoom_ShowMessageAndWait(5, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(1, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 30);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Party_RemoveOwnerRestored(5);
  // Give out the bags and make the pedestal crossings available.
  SolSanctumElementalStarRoom_GiveMythrilBags();
  Map_CopyCellAttributes(8, 0, 5, 1, 27, 17);
  gEventWork->transitionFrames = 16;
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  Engine_EventEnd();
}

// Restore pedestal state and run the first-arrival scene when needed.
s32 SolSanctumElementalStarRoom_Initialize(void) {
  s32 ringActor;

  Engine_ColorBufferApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  // Retreat does not work here (flag 0x144).
  Engine_GameFlagSet(0x144);
  ringActor = 15;
  do {
    Object_GetById(ringActor)->collisionFlags = 0;
    Engine_ActorSetSpritePriority(ringActor++, 1);
  } while ((u32)ringActor <= 24);
  SolSanctum_SetupElementalRings(15, 16);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) != 0) {
    Actor_SetPosition(9, PIXELS(456), PIXELS(360));
    Actor_SetPosition(5, PIXELS(440), PIXELS(346));
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) != 0) {
    Map_CopyCellsTo(0, 40, 43, 66, 3, 3);
    Map_CopyCellsTo(83, 40, 96, 29, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 41, 29);
    Map_CopyCellsTo(87, 42, 41, 31, 1, 2);
    Map_CopyCellsTo(83, 40, 74, 29, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 19, 29);
    Map_CopyCellsTo(87, 42, 19, 31, 1, 2);
    Map_CopyCellsTo(83, 40, 96, 10, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 41, 10);
    Map_CopyCellsTo(87, 42, 41, 12, 1, 2);
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) != 0) {
    Map_CopyCellsTo(0, 40, 43, 46, 3, 3);
    Map_CopyCellsTo(83, 40, 84, 4, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 29, 4);
    Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
    Map_CopyCellsTo(83, 40, 76, 21, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 21);
    Map_CopyCellsTo(87, 42, 21, 23, 1, 2);
    Map_CopyCellsTo(83, 40, 76, 29, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 29);
    Map_CopyCellsTo(87, 42, 21, 31, 1, 2);
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX) != 0) {
    Map_CopyCellsTo(0, 40, 13, 66, 3, 3);
    Map_CopyCellsTo(83, 40, 65, 31, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
    Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
    Map_CopyCellsTo(83, 40, 79, 9, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
    Map_CopyCellsTo(87, 42, 24, 11, 1, 2);
    Map_CopyCellsTo(83, 40, 91, 10, 3, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 36, 10);
    Map_CopyCellsTo(87, 42, 36, 12, 1, 2);
    SolSanctumElementalStarRoom_LineUpFollowers();
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
    if (gGameState.entrance == 10) {
      SolSanctumElementalStarRoom_EnterStarRoom();
    }
  }
  return 0;
}

// Show the current message and wait the requested number of frames.
static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

// Animate a star's icon and replace its owner's mythril bag with the star.
static s32 SolSanctumElementalStarRoom_PresentStar(s32 item, s32 x, s32 y,
                                                   s32 z) {
  GlyphTransfer *glyph;
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  s32 owner;
  s32 slot;

  glyph = NULL;
  actor = Engine_ObjectCreate(22, x, y, z);
  owner = PartyInventory_FindOwner(ITEM_MYTHRIL_BAG);
  slot = Inventory_Find(owner, ITEM_MYTHRIL_BAG);
  if (actor == NULL)
    return owner;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_PresentItemScript);
  sprite = actor->sprite;
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  actor->velocityY = PIXELS(5) / 2;
  actor->gravity = PIXELS(1) / 4;
  glyph = Engine_HeapAllocate(17, sizeof(*glyph));
  Engine_ItemLoadIcon(item);
  Engine_VramLoad(sprite->slot, 128, glyph->tiles);
  Engine_HeapRelease(17);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  Engine_RunRisingObjectSequence(actor, 3);
  Inventory_Discard(owner, slot);
  Inventory_AddItem(owner, item);
  Engine_ObjectDispatchRelease(actor);
  Engine_ActorSetAnimation(0, ANIM_STAND);
  return owner;
}

// Make inventory room and give the party four bags, preserving the dialogue.
static void SolSanctumElementalStarRoom_GiveMythrilBags(void) {
  struct EventWork *work = gEventWork;
  s16 savedMessage = work->message;
  s32 itemSlot;
  s32 itemOwner;
  s32 freeSlots;

  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  Engine_ItemShowFound(ITEM_MYTHRIL_BAG, 3);
  Engine_MessageShowCentered(MsgSolSanctumGotFourMythrilBags, 1);
  do {
    freeSlots = 30 - Inventory_Count(0);
    freeSlots -= Inventory_Count(1);

    if (freeSlots <= 3) {
      Engine_MessageShowCentered(MsgSolSanctumTooManyItems, 1);
      if (Shop_PickUnitItem(&itemOwner, &itemSlot) != -1)
        Inventory_Discard(itemOwner, itemSlot);
    }
  } while (freeSlots <= 3);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  work->message = savedMessage;
}

// Emit sparks while Alex disappears and reappears.
static void SolSanctumElementalStarRoom_AnimateAlexTeleport(void) {
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct FieldActor *alex;
  u32 frame;
  s32 x;
  s32 xOffset;

  alex = Object_GetById(ACTOR_ALEX);
  Engine_AudioPlayCue(SE_190);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), 0);
  effectOptions = &options;
  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 0x11c;
  effectOptions->startScaleX = 2 * Q16_ONE / 5;
  effectOptions->startScaleY = 3 * Q16_ONE;
  for (frame = 0; frame <= 31; frame++) {
    Engine_EventWait(1);
    if (!(frame & 1)) {
      xOffset = (((u32)Engine_RandomNext() * 24) >> 16) << 16;
      x = alex->x.fixed;
      x += xOffset;
      x += -PIXELS(12);
      Effect_Spawn(x,
                   alex->y.fixed +
                       ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                       PIXELS(32),
                   alex->z.fixed, 0, -PIXELS(4), 0,
                   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                       EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                   effectOptions);
    }
    if (frame == 20)
      Engine_ActorSetChildValue(14, 0x100);
  }
  Engine_ActorSetChildValue(14, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), 1);
}

// Shared source is included here in ROM order.
#include "common/sol_sanctum_elemental_star_ring_setup.h"

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gSolSanctumElementalStarRoom_AngleScript[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gSolSanctumElementalStarRoom_PresentItemScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 15 * Q16_ONE / 16),
    SC_SET(SW_SCALE_Y, 15 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(16),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_END,
};

struct SceneEntrance gSolSanctumElementalStarRoom_Entrances[] = {
    {0, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumElementalStarRoom_Exits[] = {
    SCENE_EXITS(17),
    SCENE_EXIT(2, 18, 10),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumElementalStarRoom_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
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
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(617), 0, PIXELS(408), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(429), 0, PIXELS(221), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(113), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(170), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(308), 0, PIXELS(511), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(397), 0, PIXELS(418), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(542), 0, PIXELS(269), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(421), 0, PIXELS(117), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(295), 0, PIXELS(210), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(742), 0, PIXELS(310), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolSanctumElementalStarRoom_Events[] = {
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToKraden) },
    { EVENT_TALK, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToJenna) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToSaturos) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToMenardi) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToFelix) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToAlex) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSolSanctumKradenIAmResponsible) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToJenna) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSolSanctumSaturosBringTheFinalStar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_TalkToMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSolSanctumAlexOnlyOneLeft) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_PreventLeavingBeforeCollectingStars) },
    { TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SolSanctumElementalStarRoom_CheckLeaderAhead) },
    { SEARCH(SEARCH_UNNAMED), 2, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, EVENT_SCRIPT(SolSanctumElementalStarRoom_CollectVenusStar) },
    { SEARCH(SEARCH_UNNAMED), 3, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, EVENT_SCRIPT(SolSanctumElementalStarRoom_CollectMercuryStar) },
    { SEARCH(SEARCH_UNNAMED), 4, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, EVENT_SCRIPT(SolSanctumElementalStarRoom_CollectJupiterStarAndMeetIntruders) },
    { SEARCH(SEARCH_UNNAMED), 5, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, EVENT_SCRIPT(SolSanctumElementalStarRoom_CollectMarsStar) },
    { SCENE_EVENTS_END },
};

u16 gSolSanctumElementalStarRoom_StarCells[] = {
    MAP_CELL_STEP(59, 40, 3, 4, 6), MAP_CELL_STEP(62, 40, 3, 4, 6), MAP_CELL_STEP(65, 40, 3, 4, 6), MAP_CELL_STEP(68, 40, 3, 4, 6), MAP_CELL_STEP(71, 40, 3, 4, 6), MAP_CELL_STEP(74, 40, 3, 4, 6), MAP_CELL_STEP(77, 40, 3, 4, 6), MAP_CELL_STEP(80, 40, 3, 4, 6), MAP_CELL_STEP(83, 40, 3, 4, 0), MAP_CELL_STEPS_END,
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
    0x9999, 0xcccc, 0xb333, 0x9999, 0xcccc, 0x9999, 0xb333, 0x9999,
    0x9999, 0xb333,
};

s32 gSolSanctumRing_ScaleSteps[] = {
    655, 458, 655, 458, 655, 458, 655, 655,
    524, 655,
};
// clang-format on
