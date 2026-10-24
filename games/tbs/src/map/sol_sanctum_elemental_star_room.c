/// @file
/// Elemental Star Room (MapId_SolSanctumElementalStarRoom), the chamber above
/// the Luna and Sol rooms (MapId_SolSanctumLunaSolRooms) where the four
/// Elemental Stars lie on their pedestals.
///
/// The room follows the story flags in order:
/// - The first arrival from the Luna and Sol rooms
///   (SolSanctumElementalStarRoom_EnterStarRoom): Isaac rises into the room
///   with Garet, Jenna and Kraden, who shows each pedestal in turn and gives
///   the party four Mythril Bags to carry the stars in
///   (FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED). Kraden calls the party back if it
///   walks back the way it came (EventScript_KradenStopsPartyLeaving).
/// - Searching a pedestal bags its star and opens new crossings between the
///   pedestals: the Venus Star (FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) and the
///   Mercury Star (FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) first.
/// - Bagging the Jupiter Star brings out Saturos and Menardi with Jenna and
///   Kraden as hostages (EventScript_BagJupiterStarAndMeetSaturos). Their
///   masked companion is unmasked as Felix, and Alex appears and takes the
///   three stars (FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX). From then on the
///   captors stand round Jenna and Kraden, each with a word about the last
///   star.
/// - Bagging the Mars Star sets the room quaking and takes the party to the
///   eruption (EventScript_TakeMarsStar, MapId_SolSanctumEruption,
///   sol_sanctum_eruption.c).
///
/// Each pedestal's bg event row holds only until its flag is set, so a star is
/// taken once. On every entry SolSanctumElementalStarRoom_Initialize redraws
/// the crossings each bagged star opened and, once Alex holds the stars, lines
/// up the captors again (SolSanctumElementalStarRoom_LineUpCaptors), so a
/// return after a battle finds the room as it was. Ten lights float about the
/// room (SolSanctum_StartFloatingLights), and Retreat does not work here.
///
/// Whoever arrives on the platform appears 40 pixels above it at a tenth of
/// his size, grows to full size and floats down onto it, over a shadow that
/// grows with him
/// (LOCALID_PLATFORM_SHADOW;
/// https://alchemy.pascalpixel.com/places/sol-sanctum-elemental-star-room/rising-shadow.gif:
/// Isaac's arrival, the platform's object event boxed on the screen, beside the
/// sprites and the sprites without it, where the shadow is gone).
///
/// Open questions: none.

#include "save_block.h"
#include "unit/import.h"
#include "unit/map/sol_sanctum_elemental_star_room.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/map/common/sol_sanctum_elemental_star_ring_setup.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
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
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
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

/// The placed object events, numbered in the order of
/// gSolSanctumElementalStarRoom_ObjectEventTemplates; Garet and Jenna, placed
/// first, keep their own slots.
enum StarRoomLocalId {
  /// The shadow on the arrival platform under whoever appears there: a blank
  /// object event, whose sprite draws only its shadow, set at (471, 290) and
  /// grown from a tenth of its size with the arriving object event, whose own
  /// shadow is turned off while he floats above the platform (Isaac and Kraden
  /// in SolSanctumElementalStarRoom_EnterStarRoom, Felix in
  /// SolSanctumElementalStarRoom_DemandElementalStars).
  /// ★ Named from what the player sees: the shadow that grows on the platform
  /// as Isaac comes in (its render),
  /// https://alchemy.pascalpixel.com/places/sol-sanctum-elemental-star-room/rising-shadow.gif
  LOCALID_PLATFORM_SHADOW = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  LOCALID_KRADEN,
  /// ★ Named from the game: "But, Saturos, there's still a chance they might
  /// survive." (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_SATUROS,
  /// ★ Named from the game: "Menardi!" (MsgSolSanctumKradenCouldThatBeThe)
  LOCALID_MENARDI,
  /// Felix in his mask, until SolSanctumElementalStarRoom_UnmaskFelix.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX_MASKED,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX,
  /// ★ Named from the game: "Oh, Alex... You're late!"
  /// (MsgSolSanctumPermitRelieveElemental)
  LOCALID_ALEX,
  /// The first of the ten lights that float about the room, all one sprite
  /// (SolSanctum_StartFloatingLights).
  /// ★ Named from what the player sees: small blue-green lights floating in
  /// the dark (its render)
  LOCALID_FIRST_FLOATING_LIGHT,
  /// ★ Named from what the player sees: small blue-green lights floating in
  /// the dark (its render)
  LOCALID_LAST_FLOATING_LIGHT = LOCALID_FIRST_FLOATING_LIGHT + 9
};

/// The metatile behaviors the events answer.
enum SolSanctumElementalStarRoomMetatileBehavior {
  /// The way back down to the Luna and Sol rooms.
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK = 1,
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_PEDESTAL = 2,
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_PEDESTAL = 3,
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_PEDESTAL = 4,
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_PEDESTAL = 5,
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST =
      10, ///< a jump spot, walked into east
  MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH =
      11 ///< a jump spot, walked into south
};

#define NULL ((void *)0)

struct Object;

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Advances through the current dialogue's consecutive messages.
static __inline__ void SolSanctumElementalStarRoom_SkipMessages(s32 amount) {
  gFieldWork->message += amount;
}

#include "common/object_facing.h"

const struct MapWarpDest *SolSanctumElementalStarRoom_GetWarpDests(void) {
  return gSolSanctumElementalStarRoom_WarpDests;
}

const struct MapWarpRegion *SolSanctumElementalStarRoom_GetRegions(void) {
  return NULL;
}

const u32 *SolSanctumElementalStarRoom_GetWarps(void) {
  return gSolSanctumElementalStarRoom_Warps;
}

const struct ObjectEventTemplate *
SolSanctumElementalStarRoom_GetObjectEventTemplates(void) {
  return gSolSanctumElementalStarRoom_ObjectEventTemplates;
}

/// Returns the pedestals' bg events, the talk and Mind Read rows, and the cells
/// that call the party back or check the way ahead.
const struct MapEvent *SolSanctumElementalStarRoom_GetEvents(void) {
  return gSolSanctumElementalStarRoom_Events;
}

/// Elemental Star Room: searching the Venus Star's pedestal. The
/// room flashes, new crossings open between the pedestals, and the star goes
/// into a Mythril Bag. Sets FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED.
static void EventScript_BagVenusStar(void) {
  u32 i;
  s32 member;
  s32 message;

  FieldScript_Start();
  // Flash the room before revealing the crossings for this star.
  Sound_PlaySong(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_SetTarget(0x403a52, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    if (i == 1) {
      MapGrid_SetScreenShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(30);
  FieldCamera_SetSpeedParameters(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(664), -1, PIXELS(497), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  Sound_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 96,
                                   29);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 41, 29);
  MapGrid_CopyCellsTo(87, 42, 41, 31, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_SetSpeedParameters(PIXELS(32) / 5, PIXELS(4) / 5);
  FieldCamera_MoveTo(PIXELS(311), -1, PIXELS(497), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Sound_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 74,
                                   29);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 19, 29);
  MapGrid_CopyCellsTo(87, 42, 19, 31, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_MoveTo(PIXELS(663), -1, PIXELS(192), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Sound_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 96,
                                   10);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 41, 10);
  MapGrid_CopyCellsTo(87, 42, 41, 12, 1, 2);
  FieldScript_WaitFrames(40);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(710), -1, PIXELS(474), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(20);
  MapGrid_CopyCellsTo(0, 40, 43, 66, 3, 3);
  FieldScript_WaitFrames(20);
  // Present the collected star, then return to Kraden's explanation.
  member = SolSanctumElementalStarRoom_PresentStar(220, PIXELS(712), PIXELS(16),
                                                   PIXELS(464));
  FieldScript_WaitFrames(40);
  MessageArgument_Push(member, 1);
  message = MsgSolSanctumVenusStarBagged;
  FieldMessage_Show(message, 1);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(462), -1, PIXELS(350), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 30);
  FieldScript_SetMessageId(message - 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveWithinMapBounds(PIXELS(710), -1, PIXELS(474), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
  FieldScript_End();
}

/// Elemental Star Room: searching the Mercury Star's pedestal, as
/// the Venus Star; Kraden: "Good, good! Now, go fetch the others!" Sets
/// FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED.
static void EventScript_BagMercuryStar(void) {
  u32 i;
  s32 member;
  s32 message;

  FieldScript_Start();
  Script_PlaySong(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(0x404a4e, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    if (i == 1) {
      MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(30);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(98), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 84,
                                   4);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 29, 4);
  MapGrid_CopyCellsTo(87, 42, 29, 6, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_MoveTo(PIXELS(343), -1, PIXELS(369), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 76,
                                   21);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 21);
  MapGrid_CopyCellsTo(87, 42, 21, 23, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(343), -1, PIXELS(497), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 76,
                                   29);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 29);
  MapGrid_CopyCellsTo(87, 42, 21, 31, 1, 2);
  FieldScript_WaitFrames(40);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(712), -1, PIXELS(152), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(20);
  MapGrid_CopyCellsTo(0, 40, 43, 46, 3, 3);
  FieldScript_WaitFrames(20);
  // Present the collected star, then return to Kraden's explanation.
  member = SolSanctumElementalStarRoom_PresentStar(221, PIXELS(712), PIXELS(16),
                                                   PIXELS(144));
  FieldScript_WaitFrames(40);
  MessageArgument_Push(member, 1);
  message = MsgSolSanctumPutMercuryStar;
  FieldMessage_Show(message, 1);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHEAST, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(462), -1, PIXELS(350), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 30);
  FieldScript_SetMessageId(message - 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(712), -1, PIXELS(152), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
  FieldScript_End();
}

/// Elemental Star Room: searching the Jupiter Star's pedestal.
/// The star is bagged, then Saturos and Menardi appear with Jenna and Kraden as
/// hostages, Felix is unmasked, and Alex takes the three stars. Sets
/// FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, which ends the row.
static void EventScript_BagJupiterStarAndMeetSaturos(void) {
  FieldScript_Start();
  SolSanctumElementalStarRoom_BagJupiterStar();
  SolSanctumElementalStarRoom_RevealSaturosAndMenardi();
  SolSanctumElementalStarRoom_DemandElementalStars();
  SolSanctumElementalStarRoom_OfferHostageGuarantee();
  SolSanctumElementalStarRoom_UnmaskFelix();
  SolSanctumElementalStarRoom_AlexTakesStars();
  GameFlag_Set(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  FieldScript_End();
  SolSanctumElementalStarRoom_LineUpCaptors();
}

/// Reveals the Jupiter Star's crossings and puts the star in a Mythril Bag.
static void SolSanctumElementalStarRoom_BagJupiterStar(void) {
  u32 i;
  s32 member;

  Script_PlaySong(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(0x4049d2, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    if (i == 1) {
      MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(30);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(167), -1, PIXELS(529), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 65,
                                   31);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 31);
  MapGrid_CopyCellsTo(87, 42, 10, 33, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  FieldCamera_MoveTo(PIXELS(391), -1, PIXELS(177), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 79,
                                   9);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 9);
  MapGrid_CopyCellsTo(87, 42, 24, 11, 1, 2);
  FieldScript_WaitFrames(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(583), -1, PIXELS(193), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CLATTER);
  MapGrid_PlayMetatileCopySequence(gSolSanctumElementalStarRoom_StarCells, 91,
                                   10);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 36, 10);
  MapGrid_CopyCellsTo(87, 42, 36, 12, 1, 2);
  FieldScript_WaitFrames(40);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(477), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(20);
  MapGrid_CopyCellsTo(0, 40, 13, 66, 3, 3);
  FieldScript_WaitFrames(20);
  member = SolSanctumElementalStarRoom_PresentStar(223, PIXELS(232), PIXELS(16),
                                                   PIXELS(464));
  FieldScript_WaitFrames(40);
  MessageArgument_Push(member, 1);
  FieldMessage_Show(MsgSolSanctumJupiterStarBagged, 1);
}

/// Isaac and Garet turn back to find Saturos and Menardi holding Jenna and
/// Kraden on the centre platform.
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void) {
  struct ObjectEvent *leader;

  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 231, 490);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(180);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(80);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 246, 479);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 10);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 257, 491);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 265, 453);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 282, 469);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 60);
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(475), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(491), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(459), PIXELS(348));
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(507), PIXELS(348));
  FieldCamera_SetSpeed(PIXELS(36) / 5, PIXELS(9) / 10);
  FieldCamera_MoveTo(PIXELS(485), -1, PIXELS(345), 1);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
}

/// Saturos and Menardi demand the stars, and the masked man with them, Felix,
/// appears beside them.
static void SolSanctumElementalStarRoom_DemandElementalStars(void) {
  u8 *motionFlags;
  u8 *spriteFlags;
  struct ObjectEvent *felix;
  struct ObjectEvent *shadow;
  s32 i;
  u8 zero;

  Sound_PlaySong(MUS_SATUROS_AND_MENARDI);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(MsgSolSanctumLooksLikeTheyveSpottedUs);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 30);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 10);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 30);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_ArmCallback(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHWEST + FACING_STEP, 80);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  felix = ObjectEvent_GetById(LOCALID_FELIX_MASKED);
  shadow = ObjectEvent_GetById(LOCALID_PLATFORM_SHADOW);
  spriteFlags = &felix->sprite->flags;
  zero = 0;
  *spriteFlags = zero;
  felix->scaleX = Q16_ONE / 10;
  felix->scaleY = Q16_ONE / 10;
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  ObjectEvent_SetSpriteColorById(LOCALID_FELIX_MASKED, 0x100);
  ObjectEvent_SetPosition(LOCALID_FELIX_MASKED, PIXELS(471), PIXELS(290));
  motionFlags = &felix->motionFlags;
  *motionFlags = zero;
  felix->y.fixed = PIXELS(40);
  FieldScript_WaitFrames(1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX_MASKED, 10);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  FieldCamera_WaitForArrival();
  Import_ObjectEventSetPosition(LOCALID_PLATFORM_SHADOW, PIXELS(471),
                                PIXELS(290));
  Script_PlaySong(SE_WARP);
  ObjectMotion_SetActionVariant(LOCALID_FELIX_MASKED, 2);
  for (i = 0; i != 90; i++) {
    felix->y.fixed -= PIXELS(1) / 10;
    felix->scaleX += Q16_ONE / 100;
    felix->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    FieldScript_WaitFrames(1);
  }
  *motionFlags = 5;
  FieldScript_WaitFrames(80);
  for (i = 0; i != 60; i++) {
    felix->y.fixed -= PIXELS(1) / 2;
    FieldScript_WaitFrames(1);
  }
  *motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  FieldScript_WaitFrames(30);
  *spriteFlags = 1;
  Import_ObjectEventSetPosition(LOCALID_PLATFORM_SHADOW, 0, 0);
  ObjectMotion_SetActionVariant(LOCALID_FELIX_MASKED, 1);
  {
    u8 *flags = &ObjectEvent_GetById(LOCALID_FELIX_MASKED)->priorityFlags;

    /* FAKEMATCH(temporary): the loop counter i takes the 1 afresh, so the store
     * to *spriteFlags does not keep it in r5; as *flags |=
     * OBJ_EVENT_PRIORITY_AUTOMATIC, 6 of 464 instructions differ in tbs-en. */
    i = OBJ_EVENT_PRIORITY_AUTOMATIC;
    i |= *flags;
    *flags = i;
  }
  ObjectEvent_ConfigureSpriteColor(LOCALID_FELIX_MASKED, 0);
  ObjectEvent_SetSpeed(LOCALID_FELIX_MASKED, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX_MASKED, 471, 306);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX_MASKED, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | LOCALID_FELIX_MASKED, 20);
  ObjectEvent_LinkPair(LOCALID_JENNA, LOCALID_KRADEN, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_SATUROS, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 30);
  FieldEffect_ConfigureLinkedObject(
      LOCALID_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 30);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX_MASKED, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(460), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  FieldScript_WaitFrames(80);
  FieldMessage_ShowAtObject(LOCALID_MENARDI, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    SolSanctumElementalStarRoom_SkipMessages(1);
  }
  FieldScript_ShowMessageThenWait(LOCALID_KRADEN, 0, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveWithinMapBounds(PIXELS(477), -1, PIXELS(334), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            10);
  FieldScript_SetMessageId(MsgSolSanctumWhyDenyDont);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 10);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
}

/// Menardi offers a guarantee of the hostages' safety: the masked man must
/// remove his mask. Isaac and Garet hear the name Felix, Jenna's brother, whom
/// they thought lost on the day of the Boulder.
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void) {
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_FELIX_MASKED, FACING_SOUTH + FACING_STEP,
                            40);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX_MASKED, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX_MASKED, 10);
  ObjectEvent_FaceObjectEvent(LOCALID_SATUROS, LOCALID_FELIX_MASKED, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_JENNA, LOCALID_FELIX_MASKED, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_KRADEN, LOCALID_FELIX_MASKED, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_WEST, 10);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            40);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX_MASKED, 3);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(346), PIXELS(440));
  WaitFrames(1);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(280), PIXELS(470));
  FieldCamera_MoveTo(PIXELS(261), -1, PIXELS(466), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  Script_AskYesNo(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  Script_AskYesNo(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  FieldScript_SetMessageId(MsgSolSanctumMenardiDontYouWantThem);
  Script_ShowMessage(LOCALID_MENARDI, 0);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(477), -1, PIXELS(334), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX_MASKED, 2);
  FieldScript_WaitFrames(80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX_MASKED, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX_MASKED, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX_MASKED, 20);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  ObjectEvent_FaceDirection(LOCALID_FELIX_MASKED,
                            FACING_NORTHWEST + FACING_STEP, 40);
}

/// Felix unmasks and tells Jenna he survived, saved by Saturos and Menardi;
/// Garet asks whether to give them the stars, and either way takes them over.
static void SolSanctumElementalStarRoom_UnmaskFelix(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *other;
  u16 *inventoryEntry;
  s32 remainingSlots;
  s32 starCount;
  s32 messageA;
  s32 messageB;

  Script_PlaySong(SE_RATTLE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX_MASKED, 3);
  FieldScript_WaitFrames(40);
  other = ObjectEvent_GetById(LOCALID_FELIX_MASKED);
  if (other != 0) {
    ObjectEvent_SetPosition(LOCALID_FELIX, other->x.fixed, other->z.fixed);
  }
  ObjectEvent_SetPosition(LOCALID_FELIX_MASKED, 0, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 3);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 3);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_NOD);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX, 40);
  ObjectMotion_SetVariantCallback(LOCALID_SATUROS, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_NOD);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  Import_ObjectEventSetAnimation(LOCALID_MENARDI, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 10);
  ObjectMotion_SetVariantCallback(LOCALID_FELIX, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 80);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_FELIX, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldScript_WaitFrames(4);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  Import_ObjectEventSetAnimation(LOCALID_SATUROS, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 80);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_SOUTH + FACING_STEP, 40);
  ObjectMotion_SetVariantCallback(LOCALID_MENARDI, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_MENARDI, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(261), -1, PIXELS(466), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 244, 478);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 260, 490);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  starCount = 0;
  inventoryEntry = Unit_Get(1)->inventory;
  remainingSlots = 14;
  do {
    u32 item = *inventoryEntry & 0x1ff;
    inventoryEntry++;
    if (item == 220 || item == 221 || item == 223)
      starCount++;
    remainingSlots--;
  } while (remainingSlots >= 0);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    messageA = MsgSolSanctumGuessTakeElemental;
    FieldScript_SetMessageId(messageA);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(10);
    if (starCount <= 2) {
      SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 30);
      ObjectEvent_WalkToAndWait(LOCALID_GARET, 252, 486);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
      FieldScript_WaitFrames(10);
      FieldMessage_ShowCentered((messageA + 1), 1, 0);
    } else {
      FieldScript_SetMessageId(MsgSolSanctumRightTake);
      SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 30);
    }
  } else {
    if (starCount <= 2) {
      messageB = MsgSolSanctumDontHandOver;
      FieldScript_SetMessageId(messageB);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
      ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
      ObjectEvent_WalkToAndWait(LOCALID_GARET, 244, 478);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
      ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 218, 471);
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
      FieldMessage_ShowCentered((messageB + 1), 1, 0);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 30);
      objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    } else {
      FieldScript_SetMessageId(MsgSolSanctumWontLetGo);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
      SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 30);
    }
  }
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_FollowObjectEvent(LOCALID_GARET, 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 264, 482);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 278, 480);
  objectEvent->speed = PIXELS(3);
  objectEvent->acceleration = PIXELS(2);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 312, 470);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 342, 470);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 376, 470);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
}

/// Alex appears, takes the three stars from Garet and demands the last one;
/// the Venus, Mercury and Jupiter Stars leave the party's items.
static void SolSanctumElementalStarRoom_AlexTakesStars(void) {
  u32 i;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *alex;
  u8 none;

  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  alex = ObjectEvent_Get(LOCALID_ALEX);
  ObjectEvent_SetSpriteFlags(alex, 0);
  ObjectEvent_SetSpriteColorById(LOCALID_ALEX, 15);
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(392), PIXELS(454));
  SolSanctumElementalStarRoom_AnimateAlexReappearing();
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 10);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTH + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgSolSanctumPermitRelieveElemental);
  Script_ShowMessage(LOCALID_ALEX, 0);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(469), PIXELS(348));
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_SATUROS, 40);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(507), PIXELS(348));
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 389, 468);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 20);
  FieldMessage_ShowCentered(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  none = 0;
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 376, 470);
  FieldScript_WaitFrames(30);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  FieldScript_SetMessageId(MsgSolSanctumPermitRelieveElemental + 5);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_ALEX, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_ALEX, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTH, 20);
  ObjectEvent_SetSpriteColorById(LOCALID_ALEX, 0x100);
  alex = ObjectEvent_Get(LOCALID_ALEX);
  ObjectEvent_SetSpriteFlags(alex, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_ALEX);
  objectEvent->motionFlags = none;
  Script_PlaySong(SE_SHIMMER);
  for (i = 0; i != 30; i++) {
    objectEvent->y.fixed += PIXELS(1);
    FieldScript_WaitFrames(1);
  }
  objectEvent->motionFlags = 5;
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
  ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTH + FACING_STEP, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_ALEX, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 30);
  ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 10);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(477), -1, PIXELS(334), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_SATUROS, 10);
  Script_OpenMessage(LOCALID_MENARDI, 0);
  FieldCamera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
  FieldCamera_MoveTo(PIXELS(374), -1, PIXELS(470), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  if (FieldScript_AskYesNoWithObjectEventAnimation(1, 0) != 0) {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
    FieldScript_SetMessageId(MsgSolSanctumDontWantAnything);
    Script_OpenMessage(LOCALID_ALEX, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(1, 0) == 0) {
      do {
        FieldScript_WaitFrames(20);
        ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_SHAKE_HEAD);
        FieldScript_WaitFrames(10);
        FieldScript_SetMessageId(MsgSolSanctumDoubtHowFeel);
        Script_OpenMessage(LOCALID_ALEX, 0);
      } while (FieldScript_AskYesNoWithObjectEventAnimation(1, 0) == 0);
    }
  }
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgSolSanctumThankCooperation);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_ALEX, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_ALEX, 30);
  objectEvent->motionFlags = 0;
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(12) / 5, PIXELS(6) / 5);
  Object_SetPosition(objectEvent, PIXELS(460), 0, PIXELS(360));
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_ALEX);
  ObjectEvent_SetSpriteColorById(LOCALID_ALEX, 0);
  alex = ObjectEvent_Get(LOCALID_ALEX);
  ObjectEvent_SetSpriteFlags(alex, SPRITE_FLAG_SHADOW);
  FieldScript_WaitFrames(30);
  FieldCamera_FollowObjectEvent(LOCALID_GARET, 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  FieldScript_WaitFrames(20);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  objectEvent->speed = PIXELS(3);
  objectEvent->acceleration = PIXELS(2);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 342, 470);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 312, 470);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 7);
  ObjectEvent_MoveToAndWait(LOCALID_GARET, 278, 480);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  FieldScript_WaitFrames(30);
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != NULL) {
      ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  PartyInventory_Discard(220);
  PartyInventory_Discard(221);
  PartyInventory_Discard(223);
}

/// Stands Saturos, Menardi, Felix and Alex round Jenna and Kraden after the
/// exchange, all turning to face the leader.
static void SolSanctumElementalStarRoom_LineUpCaptors(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *alex;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  FieldScript_Start();
  ObjectEvent_SetBuiltinAction(LOCALID_JENNA, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetBuiltinAction(LOCALID_KRADEN, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetBuiltinAction(LOCALID_MENARDI, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetBuiltinAction(LOCALID_SATUROS, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetBuiltinAction(LOCALID_ALEX, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetBuiltinAction(LOCALID_FELIX, OBJ_EVENT_ACTION_IDLE);
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(475), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(491), PIXELS(332));
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(459), PIXELS(348));
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(507), PIXELS(348));
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(460), PIXELS(360));
  ObjectEvent_SetPosition(LOCALID_FELIX, PIXELS(471), PIXELS(306));
  objectEvent = ObjectEvent_GetById(LOCALID_JENNA);
  objectEvent->linkedObject = leader;
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetScript(objectEvent, gSolSanctumElementalStarRoom_AngleScript);
  objectEvent = ObjectEvent_GetById(LOCALID_KRADEN);
  objectEvent->linkedObject = leader;
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetScript(objectEvent, gSolSanctumElementalStarRoom_AngleScript);
  objectEvent = ObjectEvent_GetById(LOCALID_MENARDI);
  objectEvent->linkedObject = leader;
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetScript(objectEvent, gSolSanctumElementalStarRoom_AngleScript);
  objectEvent = ObjectEvent_GetById(LOCALID_SATUROS);
  objectEvent->linkedObject = leader;
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetScript(objectEvent, gSolSanctumElementalStarRoom_AngleScript);
  alex = ObjectEvent_GetById(LOCALID_ALEX);
  alex->linkedObject = leader;
  alex->steeringFlags |= 1;
  alex->scaleX = Q16_ONE;
  alex->scaleY = Q16_ONE;
  alex->motionFlags = ObjectEvent_GetById(LOCALID_MENARDI)->motionFlags;
  alex->y.fixed = 0;
  ObjectEvent_SetScript(alex, gSolSanctumElementalStarRoom_AngleScript);
  objectEvent = ObjectEvent_GetById(LOCALID_FELIX);
  objectEvent->linkedObject = leader;
  objectEvent->steeringFlags |= 1;
  ObjectEvent_SetScript(objectEvent, gSolSanctumElementalStarRoom_AngleScript);
  FieldScript_End();
}

/// Elemental Star Room: searching the Mars Star's pedestal, until
/// FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED is set. Isaac bags the last star, and
/// the room quakes until the scene cuts away by
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION to the chamber's
/// collapse (MapId_SolSanctumEruption).
static void EventScript_TakeMarsStar(void) {
  u8 pass;
  s32 member;
  struct ObjectEvent *leader;

  FieldScript_Start();
  Script_PlaySong(SE_RUMBLE);
  for (pass = 0; pass != 6; pass++) {
    PaletteFade_ApplyTarget(0x4039d2, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    PaletteFade_StartInterpolation(8);
    FieldScript_WaitFrames(8);
    if (pass == 1) {
      MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    }
  }
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_CopyCellsTo(0, 40, 13, 46, 3, 3);
  FieldScript_WaitFrames(20);
  member = SolSanctumElementalStarRoom_PresentStar(222, PIXELS(232), PIXELS(16),
                                                   PIXELS(144));
  FieldScript_WaitFrames(40);
  MessageArgument_Push(member, 1);
  FieldMessage_Show(MsgSolSanctumMarsStarBagged, 1);
  ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(307), PIXELS(277));
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(307), PIXELS(277));
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(307), PIXELS(277));
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(307), PIXELS(277));
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(307), PIXELS(277));
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 232, 156);
  FieldScript_WaitFrames(10);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 218, 172);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  Script_PlaySong(SE_CRASH);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  FieldScript_WaitFrames(20);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 50);
  Script_PlaySong(SE_CLATTER);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 50);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  Script_PlaySong(SE_CLATTER);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  FieldScript_WaitFrames(30);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  ObjectMotion_Launch(LOCALID_GARET, 2, 20);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 40);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(
      SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION);
}

/// Elemental Star Room: talking to Kraden, about the stars still to bag, or,
/// once Alex holds them, Kraden's regret: "I am responsible for all of this."
static void EventScript_TalkToKraden(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    FieldScript_SetMessageId(MsgSolSanctumKradenIAmResponsible);
    FieldScript_ShowMessageAndWait(LOCALID_KRADEN, 0);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      FieldScript_SetMessageId(MsgSolSanctumKradenBringTheStarsHere);
    else
      FieldScript_SetMessageId(MsgSolSanctumKradenFetchTheOthers);
    ObjectEvent_LinkPair(LOCALID_KRADEN, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(10);
    FieldScript_ShowMessageAndWait(LOCALID_KRADEN, 0);
  }
  FieldScript_End();
}

/// Elemental Star Room: talking to Jenna, or Mind Read on her, about the stars,
/// or once Alex holds them, her brother alive.
static void EventScript_TalkToJenna(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
    FieldScript_SetMessageId(MsgSolSanctumJennaMyBrotherIsAlive);
    FieldScript_ShowMessageAndWait(LOCALID_JENNA, 0);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
      FieldScript_SetMessageId(MsgSolSanctumJennaBeCareful);
    else
      FieldScript_SetMessageId(MsgSolSanctumJennaKradenIsGiddy);
    ObjectEvent_LinkPair(LOCALID_JENNA, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(10);
    FieldScript_ShowMessageAndWait(LOCALID_JENNA, 0);
  }
  FieldScript_End();
}

/// Elemental Star Room: talking to Saturos, who demands the last star.
static void EventScript_TalkToSaturos(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSolSanctumSaturosBringTheFinalStar);
  FieldScript_ShowMessageAndWait(LOCALID_SATUROS, 0);
  FieldScript_End();
}

/// Elemental Star Room: talking to Menardi, or Mind Read on her; she demands
/// the last gem.
static void EventScript_TalkToMenardi(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSolSanctumMenardiBringTheFinalGem);
  FieldScript_ShowMessageAndWait(LOCALID_MENARDI, 0);
  FieldScript_End();
}

/// Elemental Star Room: talking to Felix, who says nothing. "..."
static void EventScript_TalkToFelix(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSolSanctumFelixSilence);
  FieldScript_ShowMessageAndWait(LOCALID_FELIX, 0);
  FieldScript_End();
}

/// Elemental Star Room: talking to Alex: only one star is left.
static void EventScript_TalkToAlex(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSolSanctumAlexOnlyOneLeft);
  FieldScript_ShowMessageAndWait(LOCALID_ALEX, 0);
  FieldScript_End();
}

/// Elemental Star Room: walking north back the way the party
/// came, has Kraden call it back. "There aren't any Elemental Stars out there!"
static void EventScript_KradenStopsPartyLeaving(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSolSanctumNoStarsOutThere);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  FieldScript_End();
}

/// Elemental Star Room: walking east or south into a jump spot jumps the leader
/// two cells ahead (Field_TryJumpForward).
static void EventScript_JumpAhead(void) { Field_TryJumpForward(); }

/// Shows a pedestal while Kraden describes it, then returns to the party.
static inline void SolSanctumElementalStarRoom_ShowPedestal(
    s32 cameraX, s32 cameraZ, s32 objectEventX, s32 objectEventZ, s32 speaker) {
  FieldCamera_MoveTo(cameraX, -1, cameraZ, 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetPosition(LOCALID_KRADEN, objectEventX << 16,
                          objectEventZ << 16);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(speaker, 0);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(471), PIXELS(360));
  FieldScript_WaitFrames(80);
  FieldCamera_MoveTo(PIXELS(471), -1, PIXELS(370), 1);
  FieldCamera_WaitForArrival();
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
  struct ObjectEvent *rising;
  struct ObjectEvent *shadow;
  struct ObjectEvent *leader;
  u8 *kradenSteeringFlags;
  s32 kradenSpeaker;
  s32 height;
  s32 kradenMessageSpeaker;
  s32 zero;

  FieldScript_Start();
  MapGrid_CopyCellAttributes(27, 16, 5, 1, 27, 17);
  FieldCamera_MoveTo(PIXELS(471), -1, PIXELS(261), 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  // Materialize Isaac above the arrival platform with the expanding shadow.
  shadow = ObjectEvent_Get(LOCALID_PLATFORM_SHADOW);
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  rising = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  leaderSprite = &rising->sprite->flags;
  *leaderSprite = 0;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 0x100);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(471), PIXELS(290));
  leaderMotion = &rising->motionFlags;
  *leaderMotion = 0;
  rising->y.fixed = PIXELS(40);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  gFieldWork->transitionFrames = 32;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, PIXELS(471), PIXELS(290));
  Script_PlaySong(SE_WARP);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    FieldScript_WaitFrames(1);
  }
  *leaderMotion = 5;
  FieldScript_WaitFrames(80);
  FieldCamera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  FieldCamera_MoveTo(PIXELS(471), -1, PIXELS(290), 1);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    FieldScript_WaitFrames(1);
  }
  *leaderMotion = 3;
  FieldScript_WaitFrames(20);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    s32 flags = 1 | objectEvent->priorityFlags;
    objectEvent->priorityFlags = flags;
  }
  // Finish Isaac's arrival and let the companions inspect the room.
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 0);
  {
    s32 shown = 1;

    *leaderSprite = shown;
  }
  Import_ObjectEventSetPosition(LOCALID_PLATFORM_SHADOW, 0, 0);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 471, 310);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(20);
  zero = 0;
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_JENNA, 453, 302);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 489, 302);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  ObjectMotion_Launch(LOCALID_JENNA, 2, 0);
  ObjectMotion_Launch(LOCALID_GARET, 2, 40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 3);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 3);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_JENNA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 471, 346);
  ObjectEvent_WalkTo(LOCALID_JENNA, 431, 338);
  ObjectEvent_WalkTo(LOCALID_GARET, 511, 338);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_JENNA);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 60);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 60);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 60);
  // Materialize Kraden after the companions have explored the chamber.
  rising = ObjectEvent_Get(LOCALID_KRADEN);
  kradenSprite = &rising->sprite->flags;
  *kradenSprite = zero;
  rising->scaleX = Q16_ONE / 10;
  rising->scaleY = Q16_ONE / 10;
  shadow->scaleX = Q16_ONE / 10;
  shadow->scaleY = Q16_ONE / 10;
  ObjectEvent_SetSpriteColorById(LOCALID_KRADEN, 0x100);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(471), PIXELS(290));
  kradenMotion = &rising->motionFlags;
  *kradenMotion = zero;
  rising->y.fixed = PIXELS(40);
  FieldScript_WaitFrames(1);
  FieldScript_SetMessageId(MsgSolSanctumKradenWhereAreWe);
  Script_ShowMessage(LOCALID_KRADEN, 0);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectMotion_Launch(LOCALID_JENNA, 4, 0);
  ObjectMotion_Launch(LOCALID_GARET, 4, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(471), -1, PIXELS(309), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, PIXELS(471), PIXELS(290));
  Script_PlaySong(SE_WARP);
  ObjectMotion_SetActionVariant(LOCALID_KRADEN, 2);
  for (i = 0; i != 90; i++) {
    rising->y.fixed += -PIXELS(1) / 10;
    rising->scaleX += Q16_ONE / 100;
    rising->scaleY += Q16_ONE / 100;
    shadow->scaleX += Q16_ONE / 100;
    shadow->scaleY += Q16_ONE / 100;
    FieldScript_WaitFrames(1);
  }
  *kradenMotion = 5;
  FieldScript_WaitFrames(80);
  for (i = 0; i != 60; i++) {
    rising->y.fixed += -PIXELS(1) / 2;
    FieldScript_WaitFrames(1);
  }
  *kradenMotion = 3;
  FieldScript_WaitFrames(30);
  ObjectMotion_SetActionVariant(LOCALID_KRADEN, 1);
  {
    struct ObjectEvent *objectEvent = ObjectEvent_Get(LOCALID_KRADEN);
    s32 flags = 1 | objectEvent->priorityFlags;
    objectEvent->priorityFlags = flags;
  }
  ObjectEvent_SetSpriteColorById(LOCALID_KRADEN, 0);
  {
    s32 shown = 1;

    *kradenSprite = shown;
  }
  ObjectEvent_SetPosition(LOCALID_PLATFORM_SHADOW, 0, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 471, 306);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_Launch(LOCALID_KRADEN, 2, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 20);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(12) / 5, PIXELS(6) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 423, 306);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 519, 306);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(30);
  kradenSpeaker = 0x4009;
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 30);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_JENNA, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHEAST, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenSpeaker, 10);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  FieldCamera_MoveTo(PIXELS(533), -1, PIXELS(339), 1);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 533, 339);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(30);
  FieldCamera_FollowObjectEvent(LOCALID_KRADEN, 1);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(LOCALID_KRADEN, 455, 360);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 455, 360);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 471, 360);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 30);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 30);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_JENNA, 440, 346);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 495, 346);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 10);
  Script_OpenMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 30);
  // Kraden explains the elements and the stars at the room's four corners.
  FieldScript_SetMessageId(MsgSolSanctumKradenTheElementsAreThe);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST, 20);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_KRADEN);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_JENNA);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  FieldCamera_SetSpeed(PIXELS(8), PIXELS(1));
  // Tour the four pedestals, returning the view to the party after each.
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(711), PIXELS(464), 589, 353,
                                           0x1009);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(711), PIXELS(147), 596, 238,
                                           0x1009);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(231), PIXELS(147), 308, 250,
                                           0x2009);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowPedestal(PIXELS(231), PIXELS(464), 306, 362,
                                           0x2009);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 40);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_KRADEN, 10);
  ObjectMotion_SetVariantCallback(LOCALID_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_KRADEN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 20);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
  // Discuss the Wisdom Stone and the power stored in the Elemental Stars.
  FieldScript_SetMessageId(MsgSolSanctumKradenTheWisdomStone);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 20);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 20);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 40);
  FieldScript_SetMessageId(MsgSolSanctumTheStarsHaveEnormousPower);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_KRADEN, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectMotion_Launch(LOCALID_GARET, 4, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 80);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 30);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 30);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_JENNA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 40);
  rising = ObjectEvent_Get(LOCALID_KRADEN);
  // Kraden demonstrates reaching the platforms, then asks the party to help.
  height = 192;
  WaitFrames(6);
  rising->speed = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Script_PlaySong(SE_HIGH_JUMP);
  rising->velocityY = (height << 11);
  ObjectEvent_MoveToAndWait(LOCALID_KRADEN, 471, 395);
  WaitFrames(6);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
  kradenSteeringFlags = &rising->steeringFlags;
  *kradenSteeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_KRADEN, 473, 395);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  ObjectEvent_SetDestination(LOCALID_KRADEN, 469, 395);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  ObjectEvent_SetDestination(LOCALID_KRADEN, 471, 395);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 471, 411);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
  *kradenSteeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetDestination(LOCALID_KRADEN, 474, 411);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  ObjectEvent_SetDestination(LOCALID_KRADEN, 468, 411);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  ObjectEvent_SetDestination(LOCALID_KRADEN, 471, 411);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVariantCallback(LOCALID_KRADEN, 3);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_KRADEN, 10);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 471, 395);
  ObjectEvent_SetBuiltinAction(LOCALID_KRADEN, OBJ_EVENT_ACTION_IDLE);
  FieldScript_WaitFrames(30);
  {
    s32 flags = 1 | *kradenSteeringFlags;
    *kradenSteeringFlags = flags;
  }
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 60);
  WaitFrames(6);
  rising->speed = PIXELS(3);
  rising->acceleration = PIXELS(2);
  Script_PlaySong(SE_HIGH_JUMP);
  rising->velocityY = (height << 11);
  ObjectEvent_MoveToAndWait(LOCALID_KRADEN, 471, 360);
  WaitFrames(6);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | LOCALID_KRADEN, 30);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 30);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 20);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  ObjectMotion_Launch(LOCALID_GARET, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 30);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH + FACING_STEP, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | LOCALID_KRADEN, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 30);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 30);
  Script_OpenMessage(LOCALID_JENNA, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  // Jenna keeps asking until Isaac agrees to collect the stars.
  while (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    FieldScript_SetMessageId(MsgSolSanctumJennaWontYouPlease);
    ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    ObjectMotion_Launch(LOCALID_JENNA, 4, 60);
    Script_OpenMessage(LOCALID_JENNA, 0);
  }
  FieldScript_SetMessageId(MsgSolSanctumJennaThankYou);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_JENNA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_SOUTHEAST, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 30);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 471, 352);
  FieldScript_WaitFrames(20);
  kradenMessageSpeaker = 0x8009;
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 60);
  *kradenSteeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 456, 360);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgSolSanctumTheBagsAreFromKradens);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_JENNA, 30);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  SolSanctumElementalStarRoom_ShowMessageAndWait(LOCALID_GARET, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(kradenMessageSpeaker, 30);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Party_RemoveMemberRestored(5);
  // Give out the bags and make the pedestal crossings available.
  SolSanctumElementalStarRoom_GiveMythrilBags();
  MapGrid_CopyCellAttributes(8, 0, 5, 1, 27, 17);
  gFieldWork->transitionFrames = 16;
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FieldScript_End();
}

/// Sets up the Elemental Star Room: Retreat is blocked, the floating lights
/// start, the crossings of each bagged star are redrawn, Kraden and Jenna stand
/// where the explanation left them, the captors line up once Alex holds the
/// stars, and the first arrival by
/// SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS plays its
/// scene.
s32 SolSanctumElementalStarRoom_Initialize(void) {
  s32 ringLocalId;

  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  ringLocalId = LOCALID_FIRST_FLOATING_LIGHT;
  do {
    ObjectEvent_GetById(ringLocalId)->collisionFlags = 0;
    ObjectMotion_SetActionVariant(ringLocalId++, 1);
  } while ((u32)ringLocalId <= LOCALID_LAST_FLOATING_LIGHT);
  SolSanctum_StartFloatingLights(LOCALID_FIRST_FLOATING_LIGHT, 16);
  if (Script_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) != 0) {
    ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(456), PIXELS(360));
    ObjectEvent_SetPosition(LOCALID_JENNA, PIXELS(440), PIXELS(346));
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) != 0) {
    MapGrid_CopyCellsTo(0, 40, 43, 66, 3, 3);
    MapGrid_CopyCellsTo(83, 40, 96, 29, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 41, 29);
    MapGrid_CopyCellsTo(87, 42, 41, 31, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 74, 29, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 19, 29);
    MapGrid_CopyCellsTo(87, 42, 19, 31, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 96, 10, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 41, 10);
    MapGrid_CopyCellsTo(87, 42, 41, 12, 1, 2);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) != 0) {
    MapGrid_CopyCellsTo(0, 40, 43, 46, 3, 3);
    MapGrid_CopyCellsTo(83, 40, 84, 4, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 29, 4);
    MapGrid_CopyCellsTo(87, 42, 29, 6, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 76, 21, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 21);
    MapGrid_CopyCellsTo(87, 42, 21, 23, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 76, 29, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 21, 29);
    MapGrid_CopyCellsTo(87, 42, 21, 31, 1, 2);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX) != 0) {
    MapGrid_CopyCellsTo(0, 40, 13, 66, 3, 3);
    MapGrid_CopyCellsTo(83, 40, 65, 31, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 31);
    MapGrid_CopyCellsTo(87, 42, 10, 33, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 79, 9, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 9);
    MapGrid_CopyCellsTo(87, 42, 24, 11, 1, 2);
    MapGrid_CopyCellsTo(83, 40, 91, 10, 3, 4);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 36, 10);
    MapGrid_CopyCellsTo(87, 42, 36, 12, 1, 2);
    SolSanctumElementalStarRoom_LineUpCaptors();
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
    if (gSaveBlock.warpDest ==
        SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
      SolSanctumElementalStarRoom_EnterStarRoom();
    }
  }
  return 0;
}

static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(frames);
}

/// Animates a star's icon and replaces its owner's mythril bag with the star.
static s32 SolSanctumElementalStarRoom_PresentStar(s32 item, s32 x, s32 y,
                                                   s32 z) {
  GlyphTransfer *glyph;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  s32 member;
  s32 slot;

  glyph = NULL;
  objectEvent = ObjectEvent_Create(SPRITE_BLANK, x, y, z);
  member = PartyInventory_FindMember(ITEM_MYTHRIL_BAG);
  slot = Inventory_Find(member, ITEM_MYTHRIL_BAG);
  if (objectEvent == NULL)
    return member;
  ObjectEvent_SetScript(objectEvent,
                        gSolSanctumElementalStarRoom_PresentItemScript);
  sprite = objectEvent->sprite;
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  objectEvent->velocityY = PIXELS(5) / 2;
  objectEvent->gravity = PIXELS(1) / 4;
  glyph = AllocHeapSlot(17, sizeof(*glyph));
  Icon_DrawItem(item);
  VramCache_Load(sprite->slot, 128, glyph->tiles);
  FreeHeapSlot(17);
  Sound_PlaySong(MUS_TREASURE_FOUND);
  ItemDisplay_ShowPicture(objectEvent, 3);
  Inventory_Discard(member, slot);
  Inventory_AddItem(member, item);
  ObjectEvent_Release(objectEvent);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
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

  Sound_PlaySong(MUS_TREASURE_FOUND);
  ItemDisplay_ShowItem(ITEM_MYTHRIL_BAG, 3);
  FieldMessage_Show(MsgSolSanctumGotFourMythrilBags, 1);
  do {
    freeSlots = 30 - Inventory_Count(0);
    freeSlots -= Inventory_Count(1);

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
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct ObjectEvent *alex;
  u32 frame;
  s32 x;
  s32 xOffset;

  alex = ObjectEvent_GetById(LOCALID_ALEX);
  Sound_PlaySong(SE_WARP);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ALEX), 0);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = 0x11c;
  options->startScaleX = 2 * Q16_ONE / 5;
  options->startScaleY = 3 * Q16_ONE;
  for (frame = 0; frame <= 31; frame++) {
    FieldScript_WaitFrames(1);
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
                        options);
    }
    if (frame == 20)
      ObjectEvent_ConfigureSpriteColor(LOCALID_ALEX, 0x100);
  }
  ObjectEvent_ConfigureSpriteColor(LOCALID_ALEX, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ALEX),
                             SPRITE_FLAG_SHADOW);
}

#include "common/sol_sanctum_elemental_star_ring_setup.h"

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSolSanctumElementalStarRoom_AngleScript[] = {
    SC_CALL(Map_TurnTowardTarget),
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

struct MapWarpDest gSolSanctumElementalStarRoom_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gSolSanctumElementalStarRoom_Warps[] = {
    MAP_WARPS(SolSanctumElementalStarRoom),
    MAP_WARP(SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION,
             SolSanctumEruption,
             SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gSolSanctumElementalStarRoom_ObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, 3, 0 },
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
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(617), 0, PIXELS(408), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(429), 0, PIXELS(221), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(113), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(170), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(308), 0, PIXELS(511), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(397), 0, PIXELS(418), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(542), 0, PIXELS(269), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(421), 0, PIXELS(117), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(295), 0, PIXELS(210), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(742), 0, PIXELS(310), FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gSolSanctumElementalStarRoom_Events[] = {
    { MAP_EVENT_TALK, LOCALID_KRADEN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToKraden) },
    { MAP_EVENT_TALK, LOCALID_JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToJenna) },
    { MAP_EVENT_TALK, LOCALID_SATUROS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToSaturos) },
    { MAP_EVENT_TALK, LOCALID_MENARDI, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMenardi) },
    { MAP_EVENT_TALK, LOCALID_FELIX, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToFelix) },
    { MAP_EVENT_TALK, LOCALID_ALEX, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToAlex) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KRADEN, CONDITION_ALWAYS, MSGNO(MsgSolSanctumKradenIAmResponsible) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JENNA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToJenna) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SATUROS, CONDITION_ALWAYS, MSGNO(MsgSolSanctumSaturosBringTheFinalStar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MENARDI, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMenardi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ALEX, CONDITION_ALWAYS, MSGNO(MsgSolSanctumAlexOnlyOneLeft) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_KradenStopsPartyLeaving) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAhead) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAhead) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_PEDESTAL, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, EVENT_SCRIPT(EventScript_BagVenusStar) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_PEDESTAL, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, EVENT_SCRIPT(EventScript_BagMercuryStar) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_PEDESTAL, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, EVENT_SCRIPT(EventScript_BagJupiterStarAndMeetSaturos) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_PEDESTAL, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, EVENT_SCRIPT(EventScript_TakeMarsStar) },
    { MAP_EVENTS_END },
};

u16 gSolSanctumElementalStarRoom_StarCells[] = {
    MAPGRID_ANIMATION_STEP(59, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(62, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(65, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(68, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(71, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(74, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(77, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(80, 40, 3, 4, 6), MAPGRID_ANIMATION_STEP(83, 40, 3, 4, 0), MAPGRID_ANIMATION_STEPS_END,
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
