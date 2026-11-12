/// @file
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom), the chamber above
/// the Luna and Sol rooms (SceneId_SolSanctumLunaSolRooms) where the four
/// Elemental Stars lie on their pedestals.
///
/// The room follows the story flags in order:
/// - The first arrival from the Luna and Sol rooms
///   (SolSanctumElementalStarRoom_EnterStarRoom). Isaac rises into the room
///   with Garet, Jenna and Kraden, who shows each pedestal in turn and gives
///   the party four Mythril Bags to carry the stars in
///   (FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED). Kraden calls the party back if it
///   walks back the way it came
///   (SolSanctumElementalStarRoom_KradenStopsPartyLeaving).
/// - Searching a pedestal bags its star and opens new crossings between the
///   pedestals. The Venus Star (FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) and the
///   Mercury Star (FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) come first.
/// - Bagging the Jupiter Star brings out Saturos and Menardi with Jenna and
///   Kraden as hostages
///   (SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos). Their masked
///   companion is unmasked as Felix, and Alex appears and takes the three stars
///   (FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX). From then on the captors stand
///   round Jenna and Kraden, each with a word about the last star.
/// - Bagging the Mars Star sets the room quaking and takes the party to the
///   eruption (SolSanctumElementalStarRoom_TakeMarsStar,
///   SceneId_SolSanctumEruption, sol_sanctum_eruption.c).
///
/// A star is taken once, because each pedestal's examine trigger row holds only
/// until its flag is set.
///
/// A return after a battle finds the room as it was. On every entry
/// SolSanctumElementalStarRoom_Initialize redraws the crossings each bagged
/// star opened. Once Alex holds the stars, it also lines up the captors again
/// (SolSanctumElementalStarRoom_LineUpCaptors).
///
/// Ten lights float about the room (SolSanctum_StartFloatingLights), and
/// Retreat does not work here.
///
/// Whoever arrives on the platform appears 40 pixels above it at a tenth of his
/// size. He grows to full size and floats down onto it, over a shadow that
/// grows with him. The platform's actor, ACTOR_PLATFORM_SHADOW, draws that
/// shadow.

#include "main_routine_work.h"
#include "module/import.h"
#include "module/scene/sol_sanctum_elemental_star_room.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/scene/shared/sol_sanctum_elemental_star_ring_setup.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/shop/shop.h"
#include "module/system/system.h"
#include "actor.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "create_configured_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "battle_unit.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "glyph.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "palette_buffers.h"
#include "io_reg.h"
#include "flags.h"
#include "sounds.h"
#include "heap.h"
#include "sound_driver_symbols.h"
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
static s32 SolSanctumElementalStarRoom_PresentStarFixed(s32 item, s32 x, s32 y,
                                                        s32 z);
/// SolSanctumElementalStarRoom_PresentStarFixed at a point in whole pixels.
#define SolSanctumElementalStarRoom_PresentStar(item, x, y, z)                 \
  SolSanctumElementalStarRoom_PresentStarFixed(item, px(x), px(y), px(z))
static void SolSanctumElementalStarRoom_GiveMythrilBags(void);
static void SolSanctumElementalStarRoom_AnimateAlexReappearing(void);

/// The placed actors, numbered in the order of
/// gSolSanctumElementalStarRoom_ActorPlacements.
///
/// Garet and Jenna, placed first, keep their own slots.
enum StarRoomActorId {
  /// The shadow on the arrival platform under whoever appears there.
  ///
  /// It is a blank actor, whose sprite draws only its shadow, set at (471,
  /// 290). It grows from a tenth of its size with the arriving actor, whose own
  /// shadow is turned off while he floats above the platform. That is Isaac and
  /// Kraden in SolSanctumElementalStarRoom_EnterStarRoom, and Felix in
  /// SolSanctumElementalStarRoom_DemandElementalStars.
  /// ★ Named from what the player sees: the shadow that grows on the platform
  /// as Isaac comes in (its render)
  ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
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

/// The cell behaviors the events answer.
enum SolSanctumElementalStarRoomCellBehavior {
  /// The way back down to the Luna and Sol rooms.
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK = 1,
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_PEDESTAL = 2,
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_PEDESTAL = 3,
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_PEDESTAL = 4,
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_PEDESTAL = 5,
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST =
      10, ///< a jump spot, walked into east
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH =
      11 ///< a jump spot, walked into south
};

// The room's map (map_014) draws its middle layer from the grid 40 rows down
// and its overhead layer from 56 columns across, so a cell of either layer is
// written as the room's cell plus that start.
#define MIDDLE_LAYER_ROW 40
#define OVERHEAD_LAYER_COLUMN 56

/// Puts out a star's light.
///
/// It draws a blank 3-by-3 of the middle layer over the light drawn above its
/// pedestal, whose top-left cell is (column, row).
#define PUT_OUT_STAR_LIGHT(column, row)                                        \
  MapGrid_CopyCellsTo(0, MIDDLE_LAYER_ROW, column, MIDDLE_LAYER_ROW + (row),   \
                      3, 3)

/// The overhead layer's column where a risen pillar's picture starts, three
/// cells wide about the pillar's own column.
#define RISEN_PILLAR_LEFT(column) (OVERHEAD_LAYER_COLUMN + (column) - 1)

/// Lets the party onto a risen pillar whose top is the cell (column, row).
///
/// The top takes plain ground, and the two ground-layer cells under its shaft
/// are redrawn.
#define OPEN_RISEN_PILLAR(column, row)                                         \
  (MapGrid_CopyCellAttributes(0, 0, 1, 1, column, row),                        \
   MapGrid_CopyCellsTo(87, 42, column, (row) + 2, 1, 2))

/// Draws a sunken pillar risen, as bagging a star raises three of them into
/// stepping stones: its picture, 3 by 4 cells on the overhead layer, then
/// OPEN_RISEN_PILLAR. The events play the rise (gSolSanctumElementalStarRoom_
/// StarCells) where this draws its last picture.
#define DRAW_RISEN_PILLAR(column, row)                                         \
  (MapGrid_CopyCellsTo(83, 40, RISEN_PILLAR_LEFT(column), row, 3, 4),          \
   OPEN_RISEN_PILLAR(column, row))

#define NULL ((void *)0)

struct Actor;

#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// Advances through the current dialogue's consecutive messages.
static __inline__ void SolSanctumElementalStarRoom_SkipMessages(s32 amount) {
  gFieldWork->message += amount;
}

#include "shared/actor_facing.h"

const struct SceneEntrance *SolSanctumElementalStarRoom_GetEntrances(void) {
  return gSolSanctumElementalStarRoom_Entrances;
}

const struct SceneExitRegion *SolSanctumElementalStarRoom_GetRegions(void) {
  return NULL;
}

const u32 *SolSanctumElementalStarRoom_GetExits(void) {
  return gSolSanctumElementalStarRoom_Exits;
}

const struct ActorPlacement *
SolSanctumElementalStarRoom_GetActorPlacements(void) {
  return gSolSanctumElementalStarRoom_ActorPlacements;
}

/// Returns the pedestals' examine triggers, the talk and Mind Read rows, and
/// the cells that call the party back or check the way ahead.
const struct Trigger *SolSanctumElementalStarRoom_GetTriggers(void) {
  return gSolSanctumElementalStarRoom_Triggers;
}

/// Elemental Star Room: searching the Venus Star's pedestal.
///
/// The room flashes, new crossings open between the pedestals, and the star
/// goes into a Mythril Bag. Sets FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED.
static void SolSanctumElementalStarRoom_BagVenusStar(void) {
  u32 i;
  s32 member;
  s32 message;

  Event_Start();
  // Flash the room before revealing the crossings for this star.
  Sound_Play(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 18, 14), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    if (i == 1) {
      MapGrid_SetScreenShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(30);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(664, CAMERA_KEEP, 497, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  MapGrid_SetShake(1, 2, 1);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(41), 29);
  OPEN_RISEN_PILLAR(41, 29);
  Event_Wait(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(311, CAMERA_KEEP, 497, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(19), 29);
  OPEN_RISEN_PILLAR(19, 29);
  Event_Wait(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_MoveTo(663, CAMERA_KEEP, 192, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(41), 10);
  OPEN_RISEN_PILLAR(41, 10);
  Event_Wait(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(710, CAMERA_KEEP, 474, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_SetShake(2, 1, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(20);
  PUT_OUT_STAR_LIGHT(43, 26);
  Event_Wait(20);
  // Present the collected star, then return to Kraden's explanation.
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_VENUS_STAR, 712, 16, 464);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  message = MsgSolSanctumVenusStarBagged;
  FieldMessage_Show(message, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(462, CAMERA_KEEP, 350, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_Jump(ACTOR_KRADEN, 4, 30);
  Event_SetMessageId(message - 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(710, CAMERA_KEEP, 474, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  Event_End();
}

/// Elemental Star Room: searching the Mercury Star's pedestal, as the Venus
/// Star.
///
/// Kraden: "Good, good! Now, go fetch the others!" Sets
/// FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED.
static void SolSanctumElementalStarRoom_BagMercuryStar(void) {
  u32 i;
  s32 member;
  s32 message;

  Event_Start();
  Sound_Play(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(14, 18, 18), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    if (i == 1) {
      MapGrid_SetShake(1, 1, 1);
    }
  }
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(30);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(5.6);
  FieldCamera_MoveTo(472, CAMERA_KEEP, 98, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(29), 4);
  OPEN_RISEN_PILLAR(29, 4);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_MoveTo(343, CAMERA_KEEP, 369, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(21), 21);
  OPEN_RISEN_PILLAR(21, 21);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(343, CAMERA_KEEP, 497, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(21), 29);
  OPEN_RISEN_PILLAR(21, 29);
  Event_Wait(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(712, CAMERA_KEEP, 152, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_SetShake(2, 1, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(20);
  PUT_OUT_STAR_LIGHT(43, 6);
  Event_Wait(20);
  // Present the collected star, then return to Kraden's explanation.
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_MERCURY_STAR, 712, 16, 144);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  message = MsgSolSanctumPutMercuryStar;
  FieldMessage_Show(message, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(462, CAMERA_KEEP, 350, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_Jump(ACTOR_KRADEN, 4, 30);
  Event_SetMessageId(message - 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(712, CAMERA_KEEP, 152, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  Event_End();
}

/// Elemental Star Room: searching the Jupiter Star's pedestal.
///
/// The star is bagged. Then Saturos and Menardi appear with Jenna and Kraden as
/// hostages, Felix is unmasked, and Alex takes the three stars. Sets
/// FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, which ends the row.
static void SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos(void) {
  Event_Start();
  SolSanctumElementalStarRoom_BagJupiterStar();
  SolSanctumElementalStarRoom_RevealSaturosAndMenardi();
  SolSanctumElementalStarRoom_DemandElementalStars();
  SolSanctumElementalStarRoom_OfferHostageGuarantee();
  SolSanctumElementalStarRoom_UnmaskFelix();
  SolSanctumElementalStarRoom_AlexTakesStars();
  GameFlag_Set(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Event_End();
  SolSanctumElementalStarRoom_LineUpCaptors();
}

/// Reveals the Jupiter Star's crossings and puts the star in a Mythril Bag.
static void SolSanctumElementalStarRoom_BagJupiterStar(void) {
  u32 i;
  s32 member;

  Sound_Play(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 18), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    if (i == 1) {
      MapGrid_SetShake(1, 1, 1);
    }
  }
  MapGrid_SetShake(2, 1, 1);
  Event_Wait(30);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(167, CAMERA_KEEP, 529, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(10), 31);
  OPEN_RISEN_PILLAR(10, 31);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(391, CAMERA_KEEP, 177, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(24), 9);
  OPEN_RISEN_PILLAR(24, 9);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(583, CAMERA_KEEP, 193, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctumElementalStarRoom_StarCells,
                      RISEN_PILLAR_LEFT(36), 10);
  OPEN_RISEN_PILLAR(36, 10);
  Event_Wait(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(232, CAMERA_KEEP, 477, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  MapGrid_SetShake(2, 1, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(20);
  PUT_OUT_STAR_LIGHT(13, 26);
  Event_Wait(20);
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_JUPITER_STAR, 232, 16, 464);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  FieldMessage_Show(MsgSolSanctumJupiterStarBagged, 1);
}

/// Isaac and Garet turn back to find Saturos and Menardi holding Jenna and
/// Kraden on the centre platform.
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void) {
  struct Actor *leader;

  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 231, 490);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(180);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 479);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkToAndWait(ACTOR_GARET, 257, 491);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.2);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 265, 453);
  Actor_WalkToAndWait(ACTOR_GARET, 282, 469);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 60);
  Actor_SetPosition(ACTOR_JENNA, 475, 332);
  Actor_SetPosition(ACTOR_KRADEN, 491, 332);
  Actor_SetPosition(ACTOR_MENARDI, 459, 348);
  Actor_SetPosition(ACTOR_SATUROS, 507, 348);
  FieldCamera_SetSpeed(7.2);
  FieldCamera_MoveTo(485, CAMERA_KEEP, 345, 1);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  FieldCamera_WaitForMove();
  Event_Wait(40);
}

/// Saturos and Menardi demand the stars, and the masked man with them, Felix,
/// appears beside them.
static void SolSanctumElementalStarRoom_DemandElementalStars(void) {
  u8 *motionFlags;
  u8 *spriteFlags;
  struct Actor *felix;
  struct Actor *shadow;
  s32 i;
  u8 zero;

  Sound_Play(MUS_OMINOUS);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgSolSanctumLooksLikeTheyveSpottedUs);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_Jump(ACTOR_KRADEN, 4, 10);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_SOUTHWEST, 80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  felix = Actor_Get(ACTOR_FELIX_MASKED);
  shadow = Actor_Get(ACTOR_PLATFORM_SHADOW);
  spriteFlags = &felix->sprite->flags;
  zero = 0;
  *spriteFlags = zero;
  felix->scaleX = Q16_ONE / 10;
  felix->scaleY = Q16_ONE / 10;
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_FELIX_MASKED, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_FELIX_MASKED, 471, 290);
  motionFlags = &felix->motionFlags;
  *motionFlags = zero;
  felix->y.fixed = PIXELS(40);
  Event_Wait(1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 309, 1);
  FieldCamera_WaitForMove();
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 471, 290);
  Sound_Play(SE_WARP);
  Actor_SetSpritePriority(ACTOR_FELIX_MASKED, 2);
  for (i = 0; i != 90; i++) {
    felix->y.fixed -= PIXELS(1) / 10;
    felix->scaleX += Q16_ONE / 100;
    felix->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    Event_Wait(1);
  }
  *motionFlags = 5;
  Event_Wait(80);
  for (i = 0; i != 60; i++) {
    felix->y.fixed -= PIXELS(1) / 2;
    Event_Wait(1);
  }
  *motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(30);
  *spriteFlags = 1;
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX_MASKED, 1);
  {
    u8 *flags = &Actor_Get(ACTOR_FELIX_MASKED)->priorityFlags;

    // i is reused for the flags; *flags |= ACTOR_PRIORITY_AUTOMATIC differs.
    i = ACTOR_PRIORITY_AUTOMATIC;
    i |= *flags;
    *flags = i;
  }
  Actor_SetSpriteColorById(ACTOR_FELIX_MASKED, SPRITE_COLOR_NORMAL);
  Actor_SetSpeed(ACTOR_FELIX_MASKED, 0.5);
  Actor_WalkToAndWait(ACTOR_FELIX_MASKED, 471, 306);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_FELIX_MASKED, 20);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 30);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_SetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(264, CAMERA_KEEP, 460, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(80);
  Event_OpenMessage(ACTOR_MENARDI, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    SolSanctumElementalStarRoom_SkipMessages(1);
  }
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(477, CAMERA_KEEP, 334, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
  Event_SetMessageId(MsgSolSanctumWhyDenyDont);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
}

/// Menardi offers a guarantee of the hostages' safety, and the masked man must
/// remove his mask.
///
/// Isaac and Garet hear the name Felix, Jenna's brother, whom they thought lost
/// on the day of the Boulder.
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void) {
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 20);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX_MASKED, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Event_Wait(40);
  Actor_SetPosition(ACTOR_GARET, 346, 440);
  wait(1);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPosition(ACTOR_GARET, 280, 470);
  FieldCamera_MoveTo(261, CAMERA_KEEP, 466, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(10);
  Event_SetMessageId(MsgSolSanctumMenardiDontYouWantThem);
  // "What's wrong, Felix... Don't you want them to feel safe?"
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(477, CAMERA_KEEP, 334, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Event_Wait(80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_NORTH_NORTHWEST, 40);
}

/// Felix unmasks and tells Jenna he survived, saved by Saturos and Menardi.
///
/// Garet asks whether to give them the stars, and either way takes them over.
static void SolSanctumElementalStarRoom_UnmaskFelix(void) {
  struct Actor *actor;
  struct Actor *other;
  u16 *inventoryEntry;
  s32 remainingSlots;
  s32 starCount;
  s32 messageA;
  s32 messageB;

  Sound_Play(SE_RATTLE);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Event_Wait(40);
  other = Actor_Get(ACTOR_FELIX_MASKED);
  if (other != 0) {
    Actor_SetPositionFixed(ACTOR_FELIX, other->x.fixed, other->z.fixed);
  }
  Actor_SetPosition(ACTOR_FELIX_MASKED, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 3);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 80);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 80);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(261, CAMERA_KEEP, 466, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 244, 478);
  Actor_WalkToAndWait(ACTOR_GARET, 260, 490);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  starCount = 0;
  inventoryEntry = Unit_Get(GARET)->inventory;
  remainingSlots = 14;
  do {
    u32 item = *inventoryEntry & 0x1ff;
    inventoryEntry++;
    if (item == ITEM_VENUS_STAR || item == ITEM_MERCURY_STAR ||
        item == ITEM_JUPITER_STAR)
      starCount++;
    remainingSlots--;
  } while (remainingSlots >= 0);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    messageA = MsgSolSanctumGuessTakeElemental;
    Event_SetMessageId(messageA);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    if (starCount <= 2) {
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 30);
      Actor_WalkToAndWait(ACTOR_GARET, 252, 486);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(10);
      FieldMessage_ShowCentered((messageA + 1), 1, 0);
    } else {
      Event_SetMessageId(MsgSolSanctumRightTake);
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 30);
    }
  } else {
    if (starCount <= 2) {
      messageB = MsgSolSanctumDontHandOver;
      Event_SetMessageId(messageB);
      Actor_RunRepeatedMotion(ACTOR_GARET, 3);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      Actor_RunRepeatedMotion(ACTOR_GARET, 1);
      Actor_SetSpeed(ACTOR_GARET, 2);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      actor->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_GARET, 244, 478);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
      Actor_SetDestination(ACTOR_PARTY_LEADER, 218, 471);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      FieldMessage_ShowCentered((messageB + 1), 1, 0);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
      actor->steeringFlags |= ACTOR_FACES_MOTION;
    } else {
      Event_SetMessageId(MsgSolSanctumWontLetGo);
      Actor_RunRepeatedMotion(ACTOR_GARET, 3);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
    }
  }
  FieldCamera_SetSpeed(0.5);
  FieldCamera_FollowActor(ACTOR_GARET, 1);
  FieldCamera_WaitForMove();
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 264, 482);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 278, 480);
  actor->speedLimit = PIXELS(3);
  actor->acceleration = PIXELS(2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 312, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 342, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 376, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
}

/// Alex appears, takes the three stars from Garet and demands the last one.
///
/// The Venus, Mercury and Jupiter Stars leave the party's items.
static void SolSanctumElementalStarRoom_AlexTakesStars(void) {
  u32 i;
  struct Actor *actor;
  struct Actor *alex;
  u8 none;

  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  alex = Actor_Get(ACTOR_ALEX);
  Actor_SetSpriteFlags(alex, 0);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_ALEX, 392, 454);
  SolSanctumElementalStarRoom_AnimateAlexReappearing();
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgSolSanctumPermitRelieveElemental);
  // "Will you permit me to relieve you of those Elemental Stars?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetPosition(ACTOR_SATUROS, 469, 348);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 40);
  Actor_SetPosition(ACTOR_SATUROS, 507, 348);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_WalkToAndWait(ACTOR_GARET, 389, 468);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 20);
  FieldMessage_ShowCentered(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  none = 0;
  Actor_WalkToAndWait(ACTOR_GARET, 376, 470);
  Event_Wait(30);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_SetMessageId(MsgSolSanctumPermitRelieveElemental + 5);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 20);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  alex = Actor_Get(ACTOR_ALEX);
  Actor_SetSpriteFlags(alex, 0);
  actor = Actor_Get(ACTOR_ALEX);
  actor->motionFlags = none;
  Sound_Play(SE_SHIMMER);
  for (i = 0; i != 30; i++) {
    actor->y.fixed += PIXELS(1);
    Event_Wait(1);
  }
  actor->motionFlags = 5;
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 10);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(477, CAMERA_KEEP, 334, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Event_OpenMessage(ACTOR_MENARDI, 0);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(374, CAMERA_KEEP, 470, 1);
  FieldCamera_WaitForMove();
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  if (Event_AskYesNoWithActorAnimation(1, 0) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgSolSanctumDontWantAnything);
    // "You don't want anything to happen to your friends, do you?"
    Event_OpenMessage(ACTOR_ALEX, 0);
    if (Event_AskYesNoWithActorAnimation(1, 0) == 0) {
      do {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
        Event_Wait(10);
        Event_SetMessageId(MsgSolSanctumDoubtHowFeel);
        // "I doubt that's how you really feel. I'll ask you once again...."
        Event_OpenMessage(ACTOR_ALEX, 0);
      } while (Event_AskYesNoWithActorAnimation(1, 0) == 0);
    }
  }
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  Event_SetMessageId(MsgSolSanctumThankCooperation);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  actor->motionFlags = 0;
  Actor_SetSpeed(ACTOR_ALEX, 2.4);
  FieldActor_SetMoveTarget(actor, 460, 0, 360);
  Actor_WaitForMoveById(ACTOR_ALEX);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  alex = Actor_Get(ACTOR_ALEX);
  Actor_SetSpriteFlags(alex, SPRITE_FLAG_SHADOW);
  Event_Wait(30);
  FieldCamera_FollowActor(ACTOR_GARET, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(20);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->speedLimit = PIXELS(3);
  actor->acceleration = PIXELS(2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 342, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 312, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(6);
  Actor_SetAnimation(ACTOR_GARET, 7);
  Actor_MoveToAndWait(ACTOR_GARET, 278, 480);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  PartyInventory_Discard(ITEM_VENUS_STAR);
  PartyInventory_Discard(ITEM_MERCURY_STAR);
  PartyInventory_Discard(ITEM_JUPITER_STAR);
}

/// Stands Saturos, Menardi, Felix and Alex round Jenna and Kraden after the
/// exchange, all turning to face the leader.
static void SolSanctumElementalStarRoom_LineUpCaptors(void) {
  struct Actor *leader;
  struct Actor *actor;
  struct Actor *alex;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  Actor_SetMovementTypeById(ACTOR_JENNA, MOVEMENT_TYPE_STAND);
  Actor_SetMovementTypeById(ACTOR_KRADEN, MOVEMENT_TYPE_STAND);
  Actor_SetMovementTypeById(ACTOR_MENARDI, MOVEMENT_TYPE_STAND);
  Actor_SetMovementTypeById(ACTOR_SATUROS, MOVEMENT_TYPE_STAND);
  Actor_SetMovementTypeById(ACTOR_ALEX, MOVEMENT_TYPE_STAND);
  Actor_SetMovementTypeById(ACTOR_FELIX, MOVEMENT_TYPE_STAND);
  Actor_SetPosition(ACTOR_JENNA, 475, 332);
  Actor_SetPosition(ACTOR_KRADEN, 491, 332);
  Actor_SetPosition(ACTOR_MENARDI, 459, 348);
  Actor_SetPosition(ACTOR_SATUROS, 507, 348);
  Actor_SetPosition(ACTOR_ALEX, 460, 360);
  Actor_SetPosition(ACTOR_FELIX, 471, 306);
  actor = Actor_Get(ACTOR_JENNA);
  actor->watchedActor = leader;
  actor->steeringFlags |= 1;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_KRADEN);
  actor->watchedActor = leader;
  actor->steeringFlags |= 1;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->watchedActor = leader;
  actor->steeringFlags |= 1;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->watchedActor = leader;
  actor->steeringFlags |= 1;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  alex = Actor_Get(ACTOR_ALEX);
  alex->watchedActor = leader;
  alex->steeringFlags |= 1;
  alex->scaleX = Q16_ONE;
  alex->scaleY = Q16_ONE;
  alex->motionFlags = Actor_Get(ACTOR_MENARDI)->motionFlags;
  alex->y.fixed = 0;
  Actor_StartScript(alex, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_FELIX);
  actor->watchedActor = leader;
  actor->steeringFlags |= 1;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  Event_End();
}

/// Elemental Star Room: searching the Mars Star's pedestal, until
/// FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED is set.
///
/// Isaac bags the last star, and the room quakes. Then the event cuts away by
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION to the chamber's
/// collapse (SceneId_SolSanctumEruption).
static void SolSanctumElementalStarRoom_TakeMarsStar(void) {
  u8 pass;
  s32 member;
  struct Actor *leader;

  Event_Start();
  Sound_Play(SE_RUMBLE);
  for (pass = 0; pass != 6; pass++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 14), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    if (pass == 1) {
      MapGrid_SetShake(1, 1, 1);
    }
  }
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  PUT_OUT_STAR_LIGHT(13, 6);
  Event_Wait(20);
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_MARS_STAR, 232, 16, 144);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  FieldMessage_Show(MsgSolSanctumMarsStarBagged, 1);
  Actor_SetPosition(ACTOR_JENNA, 307, 277);
  Actor_SetPosition(ACTOR_KRADEN, 307, 277);
  Actor_SetPosition(ACTOR_MENARDI, 307, 277);
  Actor_SetPosition(ACTOR_SATUROS, 307, 277);
  Actor_SetPosition(ACTOR_ALEX, 307, 277);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
  Event_Wait(10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_WalkToAndWait(ACTOR_GARET, 218, 172);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Sound_Play(SE_CRASH);
  MapGrid_SetShake(4, 4, 1);
  Event_Wait(20);
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 50);
  Sound_Play(SE_CLATTER);
  MapGrid_SetShake(3, 3, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 50);
  MapGrid_SetShake(1, 1, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Sound_Play(SE_CLATTER);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(30);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_Jump(ACTOR_GARET, 2, 20);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 40);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION);
}

/// Elemental Star Room: talking to Kraden.
///
/// He speaks about the stars still to bag or, once Alex holds them, his regret:
/// "I am responsible for all of this."
static void SolSanctumElementalStarRoom_TalkToKraden(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    Event_SetMessageId(MsgSolSanctumKradenIAmResponsible);
    // "I am sorry, everyone... I am responsible for all of this."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      Event_SetMessageId(MsgSolSanctumKradenBringTheStarsHere);
    else
      Event_SetMessageId(MsgSolSanctumKradenFetchTheOthers);
    Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Event_End();
}

/// Elemental Star Room: talking to Jenna, or Mind Read on her, about the stars,
/// or once Alex holds them, her brother alive.
static void SolSanctumElementalStarRoom_TalkToJenna(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    Event_SetMessageId(MsgSolSanctumJennaMyBrotherIsAlive);
    // "My brother is alive... but...why didn't he tell me sooner?"
    Event_ShowMessage(ACTOR_JENNA, 0);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      Event_SetMessageId(MsgSolSanctumJennaBeCareful);
    else
      Event_SetMessageId(MsgSolSanctumJennaKradenIsGiddy);
    Actor_FaceEachOther(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Event_End();
}

/// Elemental Star Room: talking to Saturos, who demands the last star.
static void SolSanctumElementalStarRoom_TalkToSaturos(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumSaturosBringTheFinalStar);
  // "If you want to save them, you'll bring us the final..."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_End();
}

/// Elemental Star Room: talking to Menardi, or Mind Read on her, and she
/// demands the last gem.
static void SolSanctumElementalStarRoom_TalkToMenardi(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumMenardiBringTheFinalGem);
  // "What are you doing? Bring us the final gem!"
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_End();
}

/// Elemental Star Room: talking to Felix, who says nothing. "..."
static void SolSanctumElementalStarRoom_TalkToFelix(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumFelixSilence);
  // "..."
  Event_ShowMessage(ACTOR_FELIX, 0);
  Event_End();
}

/// Elemental Star Room: talking to Alex, who says only one star is left.
static void SolSanctumElementalStarRoom_TalkToAlex(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumAlexOnlyOneLeft);
  // "There is only one left. You're almost done."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_End();
}

/// Elemental Star Room: walking north back the way the party came has Kraden
/// call it back.
///
/// "There aren't any Elemental Stars out there!"
static void SolSanctumElementalStarRoom_KradenStopsPartyLeaving(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumNoStarsOutThere);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Event_End();
}

/// Elemental Star Room: walking east or south into a jump spot jumps the leader
/// two cells ahead (Field_TryJumpForward).
static void SolSanctumElementalStarRoom_JumpAhead(void) {
  Field_TryJumpForward();
}

/// Shows a pedestal while Kraden describes it, then returns to the party; the
/// camera's point and Kraden's spot are in whole pixels.
static inline void
SolSanctumElementalStarRoom_ShowPedestal(s32 cameraX, s32 cameraZ, s32 actorX,
                                         s32 actorZ, s32 speaker) {
  FieldCamera_MoveToFixed(px(cameraX), CAMERA_KEEP, px(cameraZ), 1);
  FieldCamera_WaitForMove();
  Actor_SetPosition(ACTOR_KRADEN, actorX, actorZ);
  Event_Wait(40);
  Event_ShowMessage(speaker, 0);
  Actor_SetPosition(ACTOR_KRADEN, 471, 360);
  Event_Wait(80);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 370, 1);
  FieldCamera_WaitForMove();
}

/// Elemental Star Room, the first arrival: Kraden explains the Elemental Stars.
///
/// Isaac appears, Garet and Jenna step out, and Kraden follows. He explains the
/// Elemental Stars at the four pedestals, and gives out the Mythril Bags.
///
/// Called by SolSanctumElementalStarRoom_Initialize at
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS until
/// FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED is set, which it sets. Clears
/// FLAG_SHOW_AREA_NAME.
static void SolSanctumElementalStarRoom_EnterStarRoom(void) {
  u16 i;
  u8 *leaderMotion;
  u8 *kradenMotion;
  u8 *leaderSprite;
  u8 *kradenSprite;
  struct Actor *rising;
  struct Actor *shadow;
  struct Actor *leader;
  u8 *kradenSteeringFlags;
  s32 kradenSpeaker;
  s32 height;
  s32 kradenMessageSpeaker;
  s32 zero;

  Event_Start();
  MapGrid_CopyCellAttributes(27, 16, 5, 1, 27, 17);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 261, 0);
  FieldCamera_WaitForMove();
  MapGrid_CenterOnFollowedPoint();
  // Materialize Isaac above the arrival platform with the expanding shadow.
  shadow = Actor_Get(ACTOR_PLATFORM_SHADOW);
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  rising = Actor_Get(ACTOR_PARTY_LEADER);
  leaderSprite = &rising->sprite->flags;
  *leaderSprite = 0;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 471, 290);
  leaderMotion = &rising->motionFlags;
  *leaderMotion = 0;
  rising->y.fixed = PIXELS(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 471, 290);
  Sound_Play(SE_WARP);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    Event_Wait(1);
  }
  *leaderMotion = 5;
  Event_Wait(80);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 290, 1);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    Event_Wait(1);
  }
  *leaderMotion = 3;
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  {
    struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
    s32 flags = 1 | actor->priorityFlags;
    actor->priorityFlags = flags;
  }
  // Finish Isaac's arrival and let the companions inspect the room.
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  {
    s32 shown = 1;

    *leaderSprite = shown;
  }
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 471, 310);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  zero = 0;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_SetSpeed(ACTOR_JENNA, 0.5);
  Actor_WalkTo(ACTOR_JENNA, 453, 302);
  Actor_WalkToAndWait(ACTOR_GARET, 489, 302);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Actor_Jump(ACTOR_GARET, 2, 40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.2);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 471, 346);
  Actor_WalkTo(ACTOR_JENNA, 431, 338);
  Actor_WalkTo(ACTOR_GARET, 511, 338);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
  Actor_SetSpeed(ACTOR_JENNA, 0.3);
  Actor_SetSpeed(ACTOR_GARET, 0.3);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 60);
  // Materialize Kraden after the companions have explored the chamber.
  rising = Actor_Get(ACTOR_KRADEN);
  kradenSprite = &rising->sprite->flags;
  *kradenSprite = zero;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_KRADEN, 471, 290);
  kradenMotion = &rising->motionFlags;
  *kradenMotion = zero;
  rising->y.fixed = PIXELS(40);
  Event_Wait(1);
  Event_SetMessageId(MsgSolSanctumKradenWhereAreWe);
  // "Wha-Where are we?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_Jump(ACTOR_JENNA, 4, 0);
  Actor_Jump(ACTOR_GARET, 4, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 309, 1);
  FieldCamera_WaitForMove();
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 471, 290);
  Sound_Play(SE_WARP);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    Event_Wait(1);
  }
  *kradenMotion = 5;
  Event_Wait(80);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    Event_Wait(1);
  }
  *kradenMotion = 3;
  Event_Wait(30);
  Actor_SetSpritePriority(ACTOR_KRADEN, 1);
  {
    struct Actor *actor = Actor_Get(ACTOR_KRADEN);
    s32 flags = 1 | actor->priorityFlags;
    actor->priorityFlags = flags;
  }
  Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_NORMAL);
  {
    s32 shown = 1;

    *kradenSprite = shown;
  }
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 306);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_KRADEN, 2, 80);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 2.4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 423, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_KRADEN, 519, 306);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(30);
  kradenSpeaker = 0x4009;
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 30);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 10);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(40);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(533, CAMERA_KEEP, 339, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.5);
  Actor_WalkToAndWait(ACTOR_KRADEN, 533, 339);
  FieldCamera_WaitForMove();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 60);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(30);
  FieldCamera_FollowActor(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_WalkTo(ACTOR_KRADEN, 455, 360);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 455, 360);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 360);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 440, 346);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 495, 346);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  // Kraden explains the elements and the stars at the room's four corners.
  Event_SetMessageId(MsgSolSanctumKradenTheElementsAreThe);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_GARET);
  FieldCamera_SetSpeed(8);
  // Tour the four pedestals, returning the view to the party after each.
  SolSanctumElementalStarRoom_ShowPedestal(
      711, 464, 589, 353, SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(
      711, 147, 596, 238, SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(
      231, 147, 308, 250, SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(
      231, 464, 306, 362, SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  // Discuss the Wisdom Stone and the power stored in the Elemental Stars.
  Event_SetMessageId(MsgSolSanctumKradenTheWisdomStone);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Event_SetMessageId(MsgSolSanctumTheStarsHaveEnormousPower);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_Jump(ACTOR_GARET, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  rising = Actor_Get(ACTOR_KRADEN);
  // Kraden demonstrates reaching the platforms, then asks the party to help.
  height = 192;
  wait(6);
  rising->speedLimit = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Sound_Play(SE_HIGH_JUMP);
  rising->velocityY = (height << 11);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 395);
  wait(6);
  Actor_SetSpeed(ACTOR_KRADEN, 0.3);
  kradenSteeringFlags = &rising->steeringFlags;
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_KRADEN, 473, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 469, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 471, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 411);
  Actor_SetSpeed(ACTOR_KRADEN, 0.3);
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_KRADEN, 474, 411);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetDestination(ACTOR_KRADEN, 468, 411);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetDestination(ACTOR_KRADEN, 471, 411);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 395);
  Actor_SetMovementTypeById(ACTOR_KRADEN, MOVEMENT_TYPE_STAND);
  Event_Wait(30);
  {
    s32 flags = 1 | *kradenSteeringFlags;
    *kradenSteeringFlags = flags;
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  wait(6);
  rising->speedLimit = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Sound_Play(SE_HIGH_JUMP);
  rising->velocityY = (height << 11);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 360);
  wait(6);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 30);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 30);
  Event_OpenMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  // Jenna keeps asking until Isaac agrees to collect the stars.
  while (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Event_SetMessageId(MsgSolSanctumJennaWontYouPlease);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    Actor_Jump(ACTOR_JENNA, 4, 60);
    // "Don't make me ask you again! Won't you please do it?"
    Event_OpenMessage(ACTOR_JENNA, 0);
  }
  Event_SetMessageId(MsgSolSanctumJennaThankYou);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_SetSpeed(ACTOR_KRADEN, 0.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 352);
  Event_Wait(20);
  kradenMessageSpeaker = 0x8009;
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 60);
  *kradenSteeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_KRADEN, 456, 360);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgSolSanctumTheBagsAreFromKradens);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 30);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Party_RemoveMemberRestored(JENNA);
  // Give out the bags and make the pedestal crossings available.
  SolSanctumElementalStarRoom_GiveMythrilBags();
  MapGrid_CopyCellAttributes(8, 0, 5, 1, 27, 17);
  gFieldWork->transitionFrames = 16;
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sets up the Elemental Star Room.
///
/// - Retreat is blocked, and the floating lights start.
/// - The crossings of each bagged star are redrawn.
/// - Kraden and Jenna stand where the explanation left them.
/// - The captors line up once Alex holds the stars.
/// - The first arrival by
///   SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS plays its
///   event.
s32 SolSanctumElementalStarRoom_Initialize(void) {
  s32 ringActorId;

  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  ringActorId = ACTOR_FIRST_FLOATING_LIGHT;
  do {
    Actor_Get(ringActorId)->collisionFlags = 0;
    Actor_SetSpritePriority(ringActorId++, 1);
  } while ((u32)ringActorId <= ACTOR_LAST_FLOATING_LIGHT);
  SolSanctum_StartFloatingLights(ACTOR_FIRST_FLOATING_LIGHT, 16);
  if (Event_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) != 0) {
    Actor_SetPosition(ACTOR_KRADEN, 456, 360);
    Actor_SetPosition(ACTOR_JENNA, 440, 346);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) != 0) {
    PUT_OUT_STAR_LIGHT(43, 26);
    DRAW_RISEN_PILLAR(41, 29);
    DRAW_RISEN_PILLAR(19, 29);
    DRAW_RISEN_PILLAR(41, 10);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) != 0) {
    PUT_OUT_STAR_LIGHT(43, 6);
    DRAW_RISEN_PILLAR(29, 4);
    DRAW_RISEN_PILLAR(21, 21);
    DRAW_RISEN_PILLAR(21, 29);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX) != 0) {
    PUT_OUT_STAR_LIGHT(13, 26);
    DRAW_RISEN_PILLAR(10, 31);
    DRAW_RISEN_PILLAR(24, 9);
    DRAW_RISEN_PILLAR(36, 10);
    SolSanctumElementalStarRoom_LineUpCaptors();
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
    if (gMainRoutineWork.entrance ==
        SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
      SolSanctumElementalStarRoom_EnterStarRoom();
    }
  }
  return 0;
}

static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(frames);
}

/// Animates a star's icon and replaces its owner's mythril bag with the star.
static s32 SolSanctumElementalStarRoom_PresentStarFixed(s32 item, s32 x, s32 y,
                                                        s32 z) {
  GlyphTransfer *glyph;
  struct Actor *actor;
  struct Sprite *sprite;
  s32 member;
  s32 slot;

  glyph = NULL;
  actor = FieldActor_Create(SPRITE_BLANK, x, y, z);
  member = PartyInventory_FindMember(ITEM_MYTHRIL_BAG);
  slot = Inventory_Find(member, ITEM_MYTHRIL_BAG);
  if (actor == NULL)
    return member;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_PresentItemScript);
  sprite = actor->sprite;
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  actor->velocityY = PIXELS(5) / 2;
  actor->gravity = PIXELS(1) / 4;
  glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*glyph));
  Icon_DrawItem(item);
  VramCache_Load(sprite->slot, 128, glyph->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  Sound_Play(MUS_TREASURE_FOUND);
  ItemDisplay_ShowPicture(actor, 3);
  Inventory_Discard(member, slot);
  Inventory_AddItem(member, item);
  Actor_Release(actor);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  return member;
}

/// Makes room among the items and gives the party four Mythril Bags, keeping
/// the dialogue's place.
static void SolSanctumElementalStarRoom_GiveMythrilBags(void) {
  struct FieldWork *work = gFieldWork;
  s16 savedMessage = work->message;
  s32 itemSlot;
  s32 itemOwner;
  s32 freeSlots;

  Sound_Play(MUS_TREASURE_FOUND);
  Item_ShowFound(ITEM_MYTHRIL_BAG, 3);
  FieldMessage_Show(MsgSolSanctumGotFourMythrilBags, 1);
  do {
    freeSlots = 30 - Inventory_Count(ISAAC);
    freeSlots -= Inventory_Count(GARET);

    if (freeSlots <= 3) {
      FieldMessage_Show(MsgSolSanctumTooManyItems, 1);
      if (ItemPicker_Run(&itemOwner, &itemSlot) != -1)
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
  struct Actor *alex;
  u32 frame;
  s32 x;
  s32 xOffset;

  alex = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = 5;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * Q16_ONE / 5;
  options.startScaleY = 3 * Q16_ONE;
  for (frame = 0; frame <= 31; frame++) {
    Event_Wait(1);
    if (!(frame & 1)) {
      xOffset = (((u32)Import_RandomNext() * 24) >> 16) << 16;
      x = alex->x.fixed;
      x += xOffset;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        alex->y.fixed +
                            ((((u32)Import_RandomNext() << 5) >> 16) << 16) +
                            PIXELS(32),
                        alex->z.fixed, 0, -PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    }
    if (frame == 20)
      Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  }
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

#include "shared/sol_sanctum_elemental_star_ring_setup.h"

#include "shared/field_effect_duration_scripts.h"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
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
struct ActorPlacement gSolSanctumElementalStarRoom_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0),
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
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        617, 0, 408, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        429, 0, 221, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        696, 0, 113, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        170, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        308, 0, 511, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        397, 0, 418, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        542, 0, 269, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        421, 0, 117, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        295, 0, 210, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        742, 0, 310, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gSolSanctumElementalStarRoom_Triggers[] = {
    { TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToKraden) },
    { TRIGGER_TALK, ACTOR_JENNA, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToJenna) },
    { TRIGGER_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToSaturos) },
    { TRIGGER_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToMenardi) },
    { TRIGGER_TALK, ACTOR_FELIX, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToFelix) },
    { TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToAlex) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgSolSanctumKradenIAmResponsible) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNA, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToJenna) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgSolSanctumSaturosBringTheFinalStar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MENARDI, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_TalkToMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgSolSanctumAlexOnlyOneLeft) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_KradenStopsPartyLeaving) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_JumpAhead) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH, CONDITION_ALWAYS, EVENT(SolSanctumElementalStarRoom_JumpAhead) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_PEDESTAL, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, EVENT(SolSanctumElementalStarRoom_BagVenusStar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_PEDESTAL, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, EVENT(SolSanctumElementalStarRoom_BagMercuryStar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_PEDESTAL, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, EVENT(SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_PEDESTAL, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, EVENT(SolSanctumElementalStarRoom_TakeMarsStar) },
    { TRIGGERS_END },
};

u16 gSolSanctumElementalStarRoom_StarCells[] = {
    MAPGRID_ANIMATION_STEP(59, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(62, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(65, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(68, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(71, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(74, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(77, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(80, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(83, 40, 3, 4, 0), MAPGRID_ANIMATION_STEPS_END,
};

#include "shared/sol_sanctum_elemental_star_ring_tables.h"
// clang-format on
