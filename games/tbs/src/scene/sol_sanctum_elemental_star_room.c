/// @file
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom), the chamber above
/// the Luna and Sol rooms (SceneId_SolSanctumLunaSolRooms) where the four
/// Elemental Stars lie on their pedestals.
///
/// The room follows the story flags in order:
/// - The first arrival from the Luna and Sol rooms
///   (SolSanctumElementalStarRoom_EnterStarRoom): Isaac rises into the room
///   with Garet, Jenna and Kraden, who shows each pedestal in turn and gives
///   the party four Mythril Bags to carry the stars in
///   (FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED). Kraden calls the party back if it
///   walks back the way it came (Event_KradenStopsPartyLeaving).
/// - Searching a pedestal bags its star and opens new crossings between the
///   pedestals: the Venus Star (FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) and the
///   Mercury Star (FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) first.
/// - Bagging the Jupiter Star brings out Saturos and Menardi with Jenna and
///   Kraden as hostages (Event_BagJupiterStarAndMeetSaturos). Their masked
///   companion is unmasked as Felix, and Alex appears and takes the three
///   stars (FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX). From then on the captors
///   stand round Jenna and Kraden, each with a word about the last star.
/// - Bagging the Mars Star sets the room quaking and takes the party to the
///   eruption (Event_TakeMarsStar, SceneId_SolSanctumEruption,
///   sol_sanctum_eruption.c).
///
/// Each pedestal's search row holds only until its flag is set, so a star is
/// taken once. On every entry SolSanctumElementalStarRoom_Initialize redraws
/// the crossings each bagged star opened and, once Alex holds the stars, lines
/// up the captors again (SolSanctumElementalStarRoom_LineUpCaptors), so a
/// return after a battle finds the room as it was. Ten lights float about the
/// room (SolSanctum_StartFloatingLights), and Retreat does not work here.
///
/// Open questions:
/// - XXX: MUS_017, MUS_061, SE_141, SE_144, SE_145, SE_161, SE_190, SE_220 and
///   SE_289 have no proven names; a recording of the arrival, a star being
///   bagged and Alex's arrival would settle them.

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
#include "item_ids.h"

static void SolSanctumElementalStarRoom_BagJupiterStar(void);
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void);
static void SolSanctumElementalStarRoom_DemandElementalStars(void);
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void);
static void SolSanctumElementalStarRoom_UnmaskFelix(void);
static void SolSanctumElementalStarRoom_AlexTakesStars(void);
static void SolSanctumElementalStarRoom_LineUpCaptors(void);
static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames);
static s32 SolSanctumElementalStarRoom_PresentStar(s32 item, s32 x, s32 y,
                                                   s32 z);
static void SolSanctumElementalStarRoom_GiveMythrilBags(void);
static void SolSanctumElementalStarRoom_AnimateAlexReappearing(void);

/// The placed actors, numbered in the order of
/// gSolSanctumElementalStarRoom_Placements; Garet and Jenna, placed first,
/// keep their own slots.
enum StarRoomActor {
  /// The glow that grows or shrinks round an actor rising through the floor.
  /// ★ Named from what the player sees: a glow round Isaac as he rises into
  /// the room (SolSanctumElementalStarRoom_EnterStarRoom)
  ACTOR_RISING_GLOW = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  ACTOR_KRADEN,
  /// ★ Named from the game: "But, Saturos, there's still a chance they might
  /// survive." (MsgSolSanctumKradenCouldThatBeThe)
  ACTOR_SATUROS,
  /// ★ Named from the game: "Menardi!" (MsgSolSanctumKradenCouldThatBeThe)
  ACTOR_MENARDI,
  /// Felix in his mask, until SolSanctumElementalStarRoom_UnmaskFelix.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX_MASKED,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX,
  /// ★ Named from the game: "Oh, Alex... You're late!"
  /// (MsgSolSanctumPermitRelieveElemental)
  ACTOR_ALEX,
  /// The first of the ten lights that float about the room, all one sprite
  /// (SolSanctum_StartFloatingLights).
  /// ★ Named from what the player sees: small blue-green lights floating in
  /// the dark (its render)
  ACTOR_FIRST_FLOATING_LIGHT,
  /// ★ Named from what the player sees: small blue-green lights floating in
  /// the dark (its render)
  ACTOR_LAST_FLOATING_LIGHT = ACTOR_FIRST_FLOATING_LIGHT + 9
};

/// The collision codes of the cells the events answer.
enum SolSanctumStarRoomCell {
  /// The way back down to the Luna and Sol rooms.
  SOL_SANCTUM_STAR_ROOM_CELL_WAY_BACK = 1,
  SOL_SANCTUM_STAR_ROOM_CELL_VENUS_PEDESTAL = 2,
  SOL_SANCTUM_STAR_ROOM_CELL_MERCURY_PEDESTAL = 3,
  SOL_SANCTUM_STAR_ROOM_CELL_JUPITER_PEDESTAL = 4,
  SOL_SANCTUM_STAR_ROOM_CELL_MARS_PEDESTAL = 5,
  SOL_SANCTUM_STAR_ROOM_CELL_JUMP_EAST = 10, ///< a jump spot, walked into east
  SOL_SANCTUM_STAR_ROOM_CELL_JUMP_SOUTH = 11 ///< a jump spot, walked into south
};

#define NULL ((void *)0)

struct ObjectRuntime;

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Advances through the current dialogue's consecutive messages.
static __inline__ void SolSanctumElementalStarRoom_SkipMessages(s32 amount) {
  gEventWork->message += amount;
}

#include "common/object_facing.h"

const struct SceneEntrance *SolSanctumElementalStarRoom_GetEntrances(void) {
  return gSolSanctumElementalStarRoom_Entrances;
}

const struct SceneRegion *SolSanctumElementalStarRoom_GetRegions(void) {
  return NULL;
}

const u32 *SolSanctumElementalStarRoom_GetExits(void) {
  return gSolSanctumElementalStarRoom_Exits;
}

const struct ScenePlacement *SolSanctumElementalStarRoom_GetPlacements(void) {
  return gSolSanctumElementalStarRoom_Placements;
}

/// Returns the pedestals' searches, the talk and Mind Read rows, and the cells
/// that call the party back or check the way ahead.
const struct SceneEvent *SolSanctumElementalStarRoom_GetEvents(void) {
  return gSolSanctumElementalStarRoom_Events;
}

/// Elemental Star Room: searching the Venus Star's pedestal. The
/// room flashes, new crossings open between the pedestals, and the star goes
/// into a Mythril Bag. Sets FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED.
static void Event_BagVenusStar(void) {
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(462), -1, PIXELS(350), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_KRADEN, 4, 30);
  Engine_EventSetMessage(message - 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
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

/// Elemental Star Room: searching the Mercury Star's pedestal, as
/// the Venus Star; Kraden: "Good, good! Now, go fetch the others!" Sets
/// FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED.
static void Event_BagMercuryStar(void) {
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
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

/// Elemental Star Room: searching the Jupiter Star's pedestal.
/// The star is bagged, then Saturos and Menardi appear with Jenna and Kraden as
/// hostages, Felix is unmasked, and Alex takes the three stars. Sets
/// FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, which ends the row.
static void Event_BagJupiterStarAndMeetSaturos(void) {
  Engine_EventBegin();
  SolSanctumElementalStarRoom_BagJupiterStar();
  SolSanctumElementalStarRoom_RevealSaturosAndMenardi();
  SolSanctumElementalStarRoom_DemandElementalStars();
  SolSanctumElementalStarRoom_OfferHostageGuarantee();
  SolSanctumElementalStarRoom_UnmaskFelix();
  SolSanctumElementalStarRoom_AlexTakesStars();
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Engine_EventEnd();
  SolSanctumElementalStarRoom_LineUpCaptors();
}

/// Reveals the Jupiter Star's crossings and puts the star in a Mythril Bag.
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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

/// Isaac and Garet turn back to find Saturos and Menardi holding Jenna and
/// Kraden on the centre platform.
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 265, 453);
  Actor_WalkToAndWait(GARET, 282, 469);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
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

/// Saturos and Menardi demand the stars, and the masked man with them, Felix,
/// appears beside them.
static void SolSanctumElementalStarRoom_DemandElementalStars(void) {
  u8 *motionFlags;
  u8 *spriteFlags;
  struct FieldActor *felix;
  struct FieldActor *glow;
  s32 i;
  u8 zero;

  Engine_AudioPlayCue(MUS_061);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgSolSanctumLooksLikeTheyveSpottedUs);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorJump(ACTOR_KRADEN, 4, 10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorFaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST + FACING_STEP, 80);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  felix = Object_GetById(ACTOR_FELIX_MASKED);
  glow = Object_GetById(ACTOR_RISING_GLOW);
  spriteFlags = &felix->sprite->flags;
  zero = 0;
  *spriteFlags = zero;
  felix->scaleX = Q16_ONE / 10;
  felix->scaleY = Q16_ONE / 10;
  glow->scaleX = Q16_ONE / 10;
  glow->scaleY = Q16_ONE / 10;
  Actor_SetChildValue(ACTOR_FELIX_MASKED, 0x100);
  Actor_SetPosition(ACTOR_FELIX_MASKED, PIXELS(471), PIXELS(290));
  motionFlags = &felix->motionFlags;
  *motionFlags = zero;
  felix->y.fixed = PIXELS(40);
  Engine_EventWait(1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetPosition(ACTOR_RISING_GLOW, PIXELS(471), PIXELS(290));
  Event_PlayCue(SE_190);
  Engine_ActorSetSpritePriority(ACTOR_FELIX_MASKED, 2);
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
  Engine_ActorSetPosition(ACTOR_RISING_GLOW, 0, 0);
  Engine_ActorSetSpritePriority(ACTOR_FELIX_MASKED, 1);
  {
    u8 *flags = &Object_GetById(ACTOR_FELIX_MASKED)->priorityFlags;

    /* FAKEMATCH(temporary): the loop counter i takes the 1 afresh, so the store
     * to *spriteFlags does not keep it in r5; as *flags |=
     * ACTOR_PRIORITY_AUTOMATIC, 6 of 464 instructions differ in tbs-en. */
    i = ACTOR_PRIORITY_AUTOMATIC;
    i |= *flags;
    *flags = i;
  }
  Engine_ActorSetChildValue(ACTOR_FELIX_MASKED, 0);
  Actor_SetSpeed(ACTOR_FELIX_MASKED, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_FELIX_MASKED, 471, 306);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_FELIX_MASKED, 20);
  Engine_ActorFaceEachOther(JENNA, ACTOR_KRADEN, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 30);
  Engine_ActorSetAttachedEffect(ACTOR_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(264), -1, PIXELS(460), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(80);
  Engine_EventOpenMessage(ACTOR_MENARDI, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    SolSanctumElementalStarRoom_SkipMessages(1);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_CameraMoveTo(PIXELS(477), -1, PIXELS(334), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_EventSetMessage(MsgSolSanctumWhyDenyDont);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
}

/// Menardi offers a guarantee of the hostages' safety: the masked man must
/// remove his mask. Isaac and Garet hear the name Felix, Jenna's brother, whom
/// they thought lost on the day of the Boulder.
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void) {
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(JENNA, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX_MASKED, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_FaceDirection(JENNA, FACING_EAST, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Engine_EventWait(80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_NORTHWEST + FACING_STEP, 40);
}

/// Felix unmasks and tells Jenna he survived, saved by Saturos and Menardi;
/// Garet asks whether to give them the stars, and either way takes them over.
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Engine_ActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 80);
  Engine_ActorRunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_FELIX, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 80);
  Engine_ActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  Engine_ActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH + FACING_STEP, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 20);
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
      SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 30);
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
      SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
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
      SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
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
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 312, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 342, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 376, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
}

/// Alex appears, takes the three stars from Garet and demands the last one;
/// the Venus, Mercury and Jupiter Stars leave the party's items.
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
  SolSanctumElementalStarRoom_AnimateAlexReappearing();
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSolSanctumPermitRelieveElemental);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(469), PIXELS(348));
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 40);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(507), PIXELS(348));
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(GARET, 389, 468);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 20);
  UiText_ShowCenteredMessage(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  actor = Object_GetById(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  none = 0;
  Actor_WalkToAndWait(GARET, 376, 470);
  Engine_EventWait(30);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgSolSanctumPermitRelieveElemental + 5);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
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
  Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Event_OpenMessage(ACTOR_MENARDI, 0);
  Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  Camera_MoveTo(PIXELS(374), -1, PIXELS(470), 1);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  if (Engine_EventChooseYesNo(1, 0) != 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgSolSanctumDontWantAnything);
    Event_OpenMessage(ACTOR_ALEX, 0);
    if (Engine_EventChooseYesNo(1, 0) == 0) {
      do {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
        Engine_EventWait(10);
        Engine_EventSetMessage(MsgSolSanctumDoubtHowFeel);
        Event_OpenMessage(ACTOR_ALEX, 0);
      } while (Engine_EventChooseYesNo(1, 0) == 0);
    }
  }
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSolSanctumThankCooperation);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Engine_EventWait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  actor->motionFlags = 0;
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(12) / 5, PIXELS(6) / 5);
  FieldObject_SetPosition(actor, PIXELS(460), 0, PIXELS(360));
  Engine_ActorWaitForMove(ACTOR_ALEX);
  Actor_SetChildValue(ACTOR_ALEX, 0);
  alex = Actor_Get(ACTOR_ALEX);
  Engine_ActorSetSpriteFlags(alex, SPRITE_FLAG_SHADOW);
  Engine_EventWait(30);
  Engine_CameraFollowActor(GARET, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(20);
  actor = Object_GetById(GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->speed = PIXELS(3);
  actor->acceleration = PIXELS(2);
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 342, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Engine_ActorSetAnimation(GARET, 7);
  Actor_MoveToAndWait(GARET, 312, 470);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_EventWait(30);
  Event_PlayCue(SE_HIGH_JUMP);
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

/// Stands Saturos, Menardi, Felix and Alex round Jenna and Kraden after the
/// exchange, all turning to face the leader.
static void SolSanctumElementalStarRoom_LineUpCaptors(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  struct FieldActor *alex;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetBuiltinAction(JENNA, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(ACTOR_KRADEN, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(ACTOR_MENARDI, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(ACTOR_SATUROS, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(ACTOR_ALEX, ACTOR_ACTION_IDLE);
  Actor_SetBuiltinAction(ACTOR_FELIX, ACTOR_ACTION_IDLE);
  Actor_SetPosition(JENNA, PIXELS(475), PIXELS(332));
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(491), PIXELS(332));
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(459), PIXELS(348));
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(507), PIXELS(348));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(460), PIXELS(360));
  Actor_SetPosition(ACTOR_FELIX, PIXELS(471), PIXELS(306));
  actor = Object_GetById(JENNA);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(ACTOR_KRADEN);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(ACTOR_MENARDI);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(ACTOR_SATUROS);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  alex = Object_GetById(ACTOR_ALEX);
  alex->linkedObject = leader;
  alex->steeringFlags |= 1;
  alex->scaleX = Q16_ONE;
  alex->scaleY = Q16_ONE;
  alex->motionFlags = Object_GetById(ACTOR_MENARDI)->motionFlags;
  alex->y.fixed = 0;
  Engine_ObjectSetScript(alex, gSolSanctumElementalStarRoom_AngleScript);
  actor = Object_GetById(ACTOR_FELIX);
  actor->linkedObject = leader;
  actor->steeringFlags |= 1;
  Engine_ObjectSetScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  Engine_EventEnd();
}

/// Elemental Star Room: searching the Mars Star's pedestal, until
/// FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED is set. Isaac bags the last star, and
/// the room quakes until the scene cuts away by
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION to the chamber's
/// collapse (SceneId_SolSanctumEruption).
static void Event_TakeMarsStar(void) {
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
  Engine_EventRequestExit(
      SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION);
}

/// Elemental Star Room: talking to Kraden, about the stars still to bag, or,
/// once Alex holds them, Kraden's regret: "I am responsible for all of this."
static void Event_TalkToKraden(void) {
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

/// Elemental Star Room: talking to Jenna, or Mind Read on her, about the stars,
/// or once Alex holds them, her brother alive.
static void Event_TalkToJenna(void) {
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

/// Elemental Star Room: talking to Saturos, who demands the last star.
static void Event_TalkToSaturos(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumSaturosBringTheFinalStar);
  Engine_EventShowMessage(ACTOR_SATUROS, 0);
  Engine_EventEnd();
}

/// Elemental Star Room: talking to Menardi, or Mind Read on her; she demands
/// the last gem.
static void Event_TalkToMenardi(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumMenardiBringTheFinalGem);
  Engine_EventShowMessage(ACTOR_MENARDI, 0);
  Engine_EventEnd();
}

/// Elemental Star Room: talking to Felix, who says nothing. "..."
static void Event_TalkToFelix(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumFelixSilence);
  Engine_EventShowMessage(ACTOR_FELIX, 0);
  Engine_EventEnd();
}

/// Elemental Star Room: talking to Alex: only one star is left.
static void Event_TalkToAlex(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumAlexOnlyOneLeft);
  Engine_EventShowMessage(ACTOR_ALEX, 0);
  Engine_EventEnd();
}

/// Elemental Star Room: walking north back the way the party
/// came, has Kraden call it back. "There aren't any Elemental Stars out there!"
static void Event_KradenStopsPartyLeaving(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSolSanctumNoStarsOutThere);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Engine_EventEnd();
}

/// Elemental Star Room: walking east or south into a jump spot jumps the leader
/// two cells ahead (Field_TryJumpForward).
static void Event_JumpAhead(void) { Engine_LeaderCheckAhead(); }

/// Shows a pedestal while Kraden describes it, then returns to the party.
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

/// Elemental Star Room, the first arrival: Isaac appears, Garet and Jenna step
/// out, Kraden follows, explains the Elemental Stars at the four pedestals, and
/// gives out the Mythril Bags. Called by SolSanctumElementalStarRoom_Initialize
/// at SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS until
/// FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED is set, which it sets; clears
/// FLAG_SHOW_LOCATION_NAME.
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
  glow = Actor_Get(ACTOR_RISING_GLOW);
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_RISING_GLOW, PIXELS(471), PIXELS(290));
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
  Engine_ActorSetPosition(ACTOR_RISING_GLOW, 0, 0);
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
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_ActorJump(GARET, 2, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
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
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
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
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(ACTOR_RISING_GLOW, PIXELS(471), PIXELS(290));
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
  Actor_SetPosition(ACTOR_RISING_GLOW, 0, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 306);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(12) / 5, PIXELS(6) / 5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 423, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkToAndWait(ACTOR_KRADEN, 519, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  kradenSpeaker = 0x4009;
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 30);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
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
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(JENNA, 440, 346);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
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
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Engine_EventSetMessage(MsgSolSanctumTheStarsHaveEnormousPower);
  Engine_ActorSetAnimation(JENNA, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(JENNA, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Engine_ActorJump(GARET, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
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
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  rising = Actor_Get(ACTOR_KRADEN);
  // Kraden demonstrates reaching the platforms, then asks the party to help.
  height = 192;
  Engine_TaskWait(6);
  rising->speed = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Event_PlayCue(SE_HIGH_JUMP);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
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
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
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
  Event_PlayCue(SE_HIGH_JUMP);
  rising->velocityY = (height << 11);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 360);
  Engine_TaskWait(6);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 30);
  Actor_FaceDirection(JENNA, FACING_EAST, 30);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 20);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_EAST, 30);
  Event_OpenMessage(JENNA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  // Jenna keeps asking until Isaac agrees to collect the stars.
  while (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventSetMessage(MsgSolSanctumJennaWontYouPlease);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    Engine_ActorJump(JENNA, 4, 60);
    Event_OpenMessage(JENNA, 0);
  }
  Engine_EventSetMessage(MsgSolSanctumJennaThankYou);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
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
  SolSanctumElementalStarRoom_ShowMessageAndWait(JENNA, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(GARET, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
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
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Sets up the Elemental Star Room: Retreat is blocked, the floating lights
/// start, the crossings of each bagged star are redrawn, Kraden and Jenna stand
/// where the explanation left them, the captors line up once Alex holds the
/// stars, and the first arrival by
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS plays its
/// scene.
s32 SolSanctumElementalStarRoom_Initialize(void) {
  s32 ringActor;

  Engine_ColorBufferApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  ringActor = ACTOR_FIRST_FLOATING_LIGHT;
  do {
    Object_GetById(ringActor)->collisionFlags = 0;
    Engine_ActorSetSpritePriority(ringActor++, 1);
  } while ((u32)ringActor <= ACTOR_LAST_FLOATING_LIGHT);
  SolSanctum_StartFloatingLights(ACTOR_FIRST_FLOATING_LIGHT, 16);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) != 0) {
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(456), PIXELS(360));
    Actor_SetPosition(JENNA, PIXELS(440), PIXELS(346));
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
    SolSanctumElementalStarRoom_LineUpCaptors();
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
    if (gGameState.entrance ==
        SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
      SolSanctumElementalStarRoom_EnterStarRoom();
    }
  }
  return 0;
}

static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

/// Animates a star's icon and replaces its owner's mythril bag with the star.
static s32 SolSanctumElementalStarRoom_PresentStar(s32 item, s32 x, s32 y,
                                                   s32 z) {
  GlyphTransfer *glyph;
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  s32 owner;
  s32 slot;

  glyph = NULL;
  actor = Engine_ObjectCreate(SPRITE_BLANK, x, y, z);
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
  Engine_ItemShowPicture(actor, 3);
  Inventory_Discard(owner, slot);
  Inventory_AddItem(owner, item);
  Engine_ObjectDispatchRelease(actor);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  return owner;
}

/// Makes room among the items and gives the party four Mythril Bags, keeping
/// the dialogue's place.
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

/// Emits sparks while Alex disappears and reappears.
static void SolSanctumElementalStarRoom_AnimateAlexReappearing(void) {
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
      FieldFx_Spawn(x,
                    alex->y.fixed +
                        ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                        PIXELS(32),
                    alex->z.fixed, 0, -PIXELS(4), 0,
                    EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                        EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    effectOptions);
    }
    if (frame == 20)
      Engine_ActorSetChildValue(ACTOR_ALEX, 0x100);
  }
  Engine_ActorSetChildValue(ACTOR_ALEX, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

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
    SC_CALL(Scene_TurnTowardTarget),
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
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumElementalStarRoom_Exits[] = {
    SCENE_EXITS(SolSanctumElementalStarRoom),
    SCENE_EXIT(SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION,
               SolSanctumEruption,
               SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumElementalStarRoom_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MENARDI, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FELIX_MASKED, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(617), 0, PIXELS(408), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(429), 0, PIXELS(221), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(113), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(170), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(308), 0, PIXELS(511), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(397), 0, PIXELS(418), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(542), 0, PIXELS(269), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(421), 0, PIXELS(117), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(295), 0, PIXELS(210), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(742), 0, PIXELS(310), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolSanctumElementalStarRoom_Events[] = {
    { EVENT_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKraden) },
    { EVENT_TALK, JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToJenna) },
    { EVENT_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSaturos) },
    { EVENT_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToMenardi) },
    { EVENT_TALK, ACTOR_FELIX, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToFelix) },
    { EVENT_TALK, ACTOR_ALEX, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToAlex) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgSolSanctumKradenIAmResponsible) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToJenna) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgSolSanctumSaturosBringTheFinalStar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MENARDI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgSolSanctumAlexOnlyOneLeft) },
    { TOUCH_WALKING_INTO(FACING_NORTH), SOL_SANCTUM_STAR_ROOM_CELL_WAY_BACK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_KradenStopsPartyLeaving) },
    { TOUCH_WALKING_INTO(FACING_EAST), SOL_SANCTUM_STAR_ROOM_CELL_JUMP_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), SOL_SANCTUM_STAR_ROOM_CELL_JUMP_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAhead) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_STAR_ROOM_CELL_VENUS_PEDESTAL, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, EVENT_SCRIPT(Event_BagVenusStar) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_STAR_ROOM_CELL_MERCURY_PEDESTAL, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, EVENT_SCRIPT(Event_BagMercuryStar) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_STAR_ROOM_CELL_JUPITER_PEDESTAL, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, EVENT_SCRIPT(Event_BagJupiterStarAndMeetSaturos) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_STAR_ROOM_CELL_MARS_PEDESTAL, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, EVENT_SCRIPT(Event_TakeMarsStar) },
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
