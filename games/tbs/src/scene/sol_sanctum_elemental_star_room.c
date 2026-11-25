/// @file
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom), the heart of Sol
/// Sanctum, reached through the portal in the Luna and Sol rooms
/// (SceneId_SolSanctumLunaSolRooms). Stone pillars stand in a starry dark, and
/// at the four corners a statue holds up a glowing Elemental Star: the Venus
/// Star in the south east, the Mercury Star in the north east, the Jupiter Star
/// in the south west and the Mars Star in the north west. The party arrives on
/// the platform in the middle, under the beam of light it came through.
///
/// In story order:
/// - On the first arrival (SolSanctumElementalStarRoom_EnterStarRoom) Isaac,
///   Garet, Jenna and Kraden appear on the middle platform. Kraden names each
///   star as the view goes to it, is too unsteady on the slippery stone to
///   fetch them himself, and gives Isaac four Mythril Bags. Jenna leaves the
///   party and waits with him on the platform, and Kraden calls the boys back
///   if they walk back into the light
///   (SolSanctumElementalStarRoom_KradenStopsPartyLeaving).
/// - Only the Venus Star can be reached at first. Searching a statue puts its
///   star in a Mythril Bag; the room shakes and three sunken pillars rise out
///   of the dark as stepping stones to the next. The Venus Star opens the way
///   to the Mercury Star, and the two open the way to the Jupiter Star.
/// - Taking the Jupiter Star brings out Saturos and Menardi, holding Jenna and
///   Kraden on the middle platform
///   (SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos). Their masked
///   companion takes off his mask and is Felix, Jenna's brother, and Alex
///   appears and takes the three stars from Garet. From then on the four stand
///   round Jenna and Kraden, each with a word about the last star.
/// - Taking the Mars Star, in the north west, sets the room quaking, and the
///   story goes on in the eruption (SolSanctumElementalStarRoom_TakeMarsStar,
///   SceneId_SolSanctumEruption, sol_sanctum_eruption.c).
///
/// A star is taken once, because each statue's examine trigger row holds only
/// until its flag is set. A return after a battle finds the room as it was: on
/// every entry Scene_Initialize puts out the taken stars' glow and redraws the
/// pillars each star raised, and once Alex holds the stars it lines up the
/// captors again (SolSanctumElementalStarRoom_StandAroundHostages).
///
/// Ten lights float about the room (SolSanctum_StartFloatingLights), and
/// Retreat does not work here.
///
/// Whoever arrives on the platform appears 40 pixels above it at a tenth of his
/// size. He grows to full size and floats down onto it, over a shadow that
/// grows with him. The platform's actor, ACTOR_PLATFORM_SHADOW, draws that
/// shadow.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "scene/sol_sanctum_elemental_star_lights.h"
#include "scene/sparkle_warp.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/io_reg.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern s32 gSolSanctumElementalStarRoom_AngleScript[];
extern s32 gItemIcon_GrowScript[];
extern struct SceneEntrance gSolSanctum_StarRoomEntrances[];
extern u32 gSolSanctumElementalStarRoom_Exits[];
extern struct ActorPlacement gSolSanctumElementalStarRoom_ActorPlacements[];
extern struct Trigger gSolSanctumElementalStarRoom_Triggers[];
extern u16 gSolSanctum_StarCells[];
extern u8 gSolSanctum_LightDrift[][3];
extern u8 gSolSanctum_LightSwing[][3];
extern s8 gSolSanctum_LightDirection[][3];
extern FIXED gSolSanctum_LightMaxScales[];
extern FIXED gSolSanctum_LightScaleSteps[];

static void SolSanctumElementalStarRoom_BagJupiterStar(void);
static void SolSanctumElementalStarRoom_RevealSaturosAndMenardi(void);
static void SolSanctumElementalStarRoom_DemandElementalStars(void);
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void);
static void SolSanctumElementalStarRoom_FelixRemovesMask(void);
static void SolSanctumElementalStarRoom_AlexTakesStars(void);
static void SolSanctumElementalStarRoom_StandAroundHostages(void);
static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames);
static s32 SolSanctumElementalStarRoom_PresentStarFX(s32 item, FIXED x, FIXED y,
                                                     FIXED z);
/// SolSanctumElementalStarRoom_PresentStarFX at a point in whole pixels.
#define SolSanctumElementalStarRoom_PresentStar(item, x, y, z)                 \
  SolSanctumElementalStarRoom_PresentStarFX(item, toFIXED(x), toFIXED(y),      \
                                            toFIXED(z))
static void SolSanctumElementalStarRoom_GiveMythrilBags(void);
static void SolSanctumElementalStarRoom_AlexWarpsIn(void);

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
  ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN,
  ACTOR_SATUROS,
  ACTOR_MENARDI,
  /// Felix in his mask, until SolSanctumElementalStarRoom_FelixRemovesMask.
  ACTOR_FELIX_MASKED,
  ACTOR_FELIX,
  ACTOR_ALEX,
  /// The first of the ten lights that float about the room, all one sprite
  /// (SolSanctum_StartFloatingLights).
  ACTOR_FIRST_FLOATING_LIGHT,
  ACTOR_LAST_FLOATING_LIGHT = ACTOR_FIRST_FLOATING_LIGHT + 9
};

/// The cell behaviors the events answer.
enum SolSanctumElementalStarRoomCellBehavior {
  /// The beam of light north of the middle platform, the way back to the Luna
  /// and Sol rooms: cells (29, 17) and (29, 18).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK = 1,
  /// The statue holding the Venus Star, in the south east: cell (44, 28).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_STATUE = 2,
  /// The statue holding the Mercury Star, in the north east: cell (44, 8).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_STATUE = 3,
  /// The statue holding the Jupiter Star, in the south west: cell (14, 28).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_STATUE = 4,
  /// The statue holding the Mars Star, in the north west: cell (14, 8).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_STATUE = 5,
  /// A pillar's edge to jump east from: (15, 21) and (45, 28).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST = 10,
  /// The middle platform's south edge, to jump south from: (29, 22).
  CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH = 11
};

/// The room's map (sol_sanctum_elemental_star_room) draws its middle layer from
/// the grid 40 rows down and its overhead layer from 56 columns across, so a
/// cell of either layer is written as the room's cell plus that start.
#define MIDDLE_LAYER_ROW 40
#define OVERHEAD_LAYER_COLUMN 56

/// Puts out the glow round a statue's star once the star is taken.
///
/// It draws a blank 3-by-3 of the middle layer over the glow drawn above the
/// statue, whose top-left cell is (column, row).
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
/// OPEN_RISEN_PILLAR. The events play the rise (gSolSanctum_StarCells) where
/// this draws its last picture.
#define DRAW_RISEN_PILLAR(column, row)                                         \
  (MapGrid_CopyCellsTo(83, 40, RISEN_PILLAR_LEFT(column), row, 3, 4),          \
   OPEN_RISEN_PILLAR(column, row))

struct Actor;

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Advances through the current dialogue's consecutive messages.
static inline void SolSanctumElementalStarRoom_SkipMessages(s32 amount) {
  gFieldState->message += amount;
}

#include "shared/actor_facing.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctum_StarRoomEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumElementalStarRoom_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gSolSanctumElementalStarRoom_ActorPlacements;
}

/// Returns the statues' examine triggers, the talk rows, the beam of light
/// that Kraden calls the party back from, and the edges to jump from. The Mind
/// Read rows are there too, though no one in the party knows Mind Read yet.
const struct Trigger *Scene_GetTriggers(void) {
  return gSolSanctumElementalStarRoom_Triggers;
}

/// Isaac searches the statue in the south east of the room (cell (44, 28)) and
/// puts the Venus Star in a Mythril Bag, and three pillars rise out of the
/// dark.
///
/// The room rumbles and flashes the yellow of earth, and the view goes to each
/// sunken pillar in turn as it rises with a clatter: one west of the Venus
/// statue, one east of the Jupiter statue in the south west, and one west of
/// the Mercury statue in the north east. Back at the Venus statue the star's
/// glow goes out and the star rises over Isaac's head. Kraden, on the middle
/// platform, hops for joy: "Good, good! Now, go fetch the others!"
static void SolSanctumElementalStarRoom_BagVenusStar(void) {
  u32 i;
  s32 member;
  s32 message;

  Event_Start();
  Sound_Play(SE_RUMBLE);
  for (i = 0; i != 6; i++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(18, 18, 14), PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(8);
    Event_Wait(8);
    if (i == 1) {
      MapGrid_SetScreenShake(toFIXED(1), toFIXED(1), FIXED_ONE);
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
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(41), 29);
  OPEN_RISEN_PILLAR(41, 29);
  Event_Wait(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(311, CAMERA_KEEP, 497, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(19), 29);
  OPEN_RISEN_PILLAR(19, 29);
  Event_Wait(40);
  MapGrid_SetScreenShake(0, 0, 0);
  FieldCamera_MoveTo(663, CAMERA_KEEP, 192, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(41), 10);
  OPEN_RISEN_PILLAR(41, 10);
  Event_Wait(40);
  gFieldState->transition =
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
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_VENUS_STAR, 712, 16, 464);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  message = MsgSolSanctumVenusStarBagged;
  // "...put the Venus Star in a Mythril Bag."
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
  // "Good, good! Now, go fetch the others!"
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

/// Isaac searches the statue in the north east of the room (cell (44, 8)) and
/// puts the Mercury Star in a Mythril Bag, and three more pillars rise.
///
/// It plays as the Venus Star does, the room flashing the blue of water. The
/// pillars rise north of the middle platform, west of it, and in the south
/// west by the Jupiter statue, which can now be reached. Kraden hops again:
/// "Good, good! Now, go fetch the others!"
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
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(29), 4);
  OPEN_RISEN_PILLAR(29, 4);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_MoveTo(343, CAMERA_KEEP, 369, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(21), 21);
  OPEN_RISEN_PILLAR(21, 21);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(343, CAMERA_KEEP, 497, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(21), 29);
  OPEN_RISEN_PILLAR(21, 29);
  Event_Wait(40);
  gFieldState->transition =
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
  member =
      SolSanctumElementalStarRoom_PresentStar(ITEM_MERCURY_STAR, 712, 16, 144);
  Event_Wait(40);
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  message = MsgSolSanctumPutMercuryStar;
  // "...put the Mercury Star in a Mythril Bag."
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
  // "Good, good! Now, go fetch the others!"
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

/// Isaac searches the statue in the south west of the room (cell (14, 28)) and
/// puts the Jupiter Star in a Mythril Bag. Turning back, he and Garet find
/// Saturos and Menardi holding Jenna and Kraden on the middle platform.
///
/// The captors demand the stars. Their masked companion takes off his mask and
/// is Felix, Jenna's brother, believed dead since the day of the Boulder, and
/// he becomes their guarantee that the hostages will not be harmed. Garet
/// carries the three stars over, Alex warps in to take them, and the captors
/// send the boys for the last one. The four then stand round Jenna and Kraden
/// until the Mars Star is taken.
static void SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos(void) {
  Event_Start();
  SolSanctumElementalStarRoom_BagJupiterStar();
  SolSanctumElementalStarRoom_RevealSaturosAndMenardi();
  SolSanctumElementalStarRoom_DemandElementalStars();
  SolSanctumElementalStarRoom_OfferHostageGuarantee();
  SolSanctumElementalStarRoom_FelixRemovesMask();
  SolSanctumElementalStarRoom_AlexTakesStars();
  GameFlag_Set(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
  Event_End();
  SolSanctumElementalStarRoom_StandAroundHostages();
}

/// The room flashes the purple of wind and three pillars rise, by the Jupiter
/// statue and on the way north to the Mars Star; then the star's glow goes
/// out and Isaac puts the Jupiter Star in a Mythril Bag.
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
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(10), 31);
  OPEN_RISEN_PILLAR(10, 31);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(6.4);
  FieldCamera_MoveTo(391, CAMERA_KEEP, 177, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(24), 9);
  OPEN_RISEN_PILLAR(24, 9);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(583, CAMERA_KEEP, 193, 1);
  FieldCamera_WaitForMove();
  MapGrid_SetShake(1, 2, 1);
  Event_Wait(20);
  Sound_Play(SE_CLATTER);
  MapGrid_PlayMapEdit(gSolSanctum_StarCells, RISEN_PILLAR_LEFT(36), 10);
  OPEN_RISEN_PILLAR(36, 10);
  Event_Wait(40);
  gFieldState->transition =
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
  // "...put the Jupiter Star in a Mythril Bag."
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
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
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
  struct Actor *felix;
  struct Actor *shadow;
  s32 frame;
  struct Sprite *sprite;

  Sound_Play(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgSolSanctumLooksLikeTheyveSpottedUs);
  // "It looks like they've spotted us."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
  // "I thought we'd be safe until they'd gotten all the Elemental..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_Jump(ACTOR_KRADEN, 4, 10);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  // "You! You mean to steal the Elemental Stars!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
  // "Master Kraden... Are you calling us thieves?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
  // "As I recall, aren't you the one who stole our information?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_SOUTHWEST, 80);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What will you do with us once you've got the Elemental Stars?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "I'm sure they'll do us in once they no longer need us!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  felix = Actor_Get(ACTOR_FELIX_MASKED);
  shadow = Actor_Get(ACTOR_PLATFORM_SHADOW);
  sprite = felix->sprite;
  sprite->flags = 0;
  felix->scaleX = FIXED_ONE / 10;
  felix->scaleY = FIXED_ONE / 10;
  shadow->scaleX = FIXED_ONE / 10;
  shadow->scaleY = FIXED_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_FELIX_MASKED, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_FELIX_MASKED, 471, 290);
  felix->motionFlags = 0;
  felix->y.fixed = toFIXED(40);
  Event_Wait(1);
  // "Relax... You will not be hurt."
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
  for (frame = 0; frame != 90; frame++) {
    felix->y.fixed -= toFIXED(1) / 10;
    felix->scaleX += FIXED_ONE / 100;
    felix->scaleY += FIXED_ONE / 100;
    shadow->scaleX += FIXED_ONE / 100;
    shadow->scaleY += FIXED_ONE / 100;
    Event_Wait(1);
  }
  felix->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
  Event_Wait(80);
  for (frame = 0; frame != 60; frame++) {
    felix->y.fixed -= toFIXED(1) / 2;
    Event_Wait(1);
  }
  felix->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(30);
  sprite->flags = SPRITE_FLAG_SHADOW;
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
  Actor_SetSpritePriority(ACTOR_FELIX_MASKED, 1);
  Actor_Get(ACTOR_FELIX_MASKED)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteColorById(ACTOR_FELIX_MASKED, SPRITE_COLOR_NORMAL);
  Actor_SetSpeed(ACTOR_FELIX_MASKED, 0.5);
  Actor_WalkToAndWait(ACTOR_FELIX_MASKED, 471, 306);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  // "That was our agreement, was it not?"
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
  // "That all depends on how well they cooperate."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 30);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_Wait(10);
  // "They'll be safe, as long as they hand over the Elemental Stars."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 30);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
  Event_Wait(20);
  // "Isn't that enough?"
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
  // "You heard us... If you wish to save your friends, then give us..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(80);
  // "Do you accept our terms?"
  Event_OpenMessage(ACTOR_MENARDI, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    SolSanctumElementalStarRoom_SkipMessages(1);
  }
  // Yes: "No, Isaac! You must not give them the Elemental Stars!" No: "Very
  // good, Isaac! You must not give them the Elemental Stars!"
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
  // "Why would you deny us? Don't you want your friends to be safe?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "What guarantee do we have once we've given you the Elemental..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "Guarantee... Ah. I have one."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
}

/// Menardi offers a guarantee of the hostages' safety, and the masked man must
/// remove his mask.
static void SolSanctumElementalStarRoom_OfferHostageGuarantee(void) {
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
  // "You will have to remove your mask."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 20);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "Th- That means..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
  Actor_FaceActor(ACTOR_SATUROS, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_FELIX_MASKED, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX_MASKED, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  // "Ah, of course. A fine idea... He will be our guarantee."
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
  // "Under the circumstances, we have little choice. Felix!.."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX_MASKED,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Event_Wait(40);
  Actor_SetPosition(ACTOR_GARET, 346, 440);
  wait(1);
  // "What? What did he say, Isaac?"
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
  // "He called that guy Felix... That's what he said, isn't it?" Yes: "You
  // heard it too? Felix? It can't be..." No: "But that's what I heard. Felix?
  // It can't be..."
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  // "That day... The Boulder... We saw him die, didn't we?" Yes: "You remember
  // it, too, Isaac?.." No: "We searched for weeks, but we still couldn't find
  // him."
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
  // "Felix... Wasn't that the name of your brother?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "But that accident... My brother, he..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
  Event_Wait(80);
  // "I'll do it."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
  // "I'll . . .take off my mask."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_FELIX_MASKED, FACING_NORTH_NORTHWEST, 40);
}

/// Felix unmasks and tells Jenna he survived, saved by Saturos and Menardi.
static void SolSanctumElementalStarRoom_FelixRemovesMask(void) {
  struct Actor *actor;
  struct Actor *other;
  u16 *inventoryEntry;
  s32 slot;
  s32 starCount;
  s32 messageA;
  s32 messageB;

  Sound_Play(SE_RATTLE);
  Actor_RunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
  Event_Wait(40);
  other = Actor_Get(ACTOR_FELIX_MASKED);
  if (other != NULL) {
    Actor_SetPositionFX(ACTOR_FELIX, other->x.fixed, other->z.fixed);
  }
  Actor_SetPosition(ACTOR_FELIX_MASKED, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 3);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 3);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  // "I -It's not possible..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "Is he..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  // "I know I've caused you much grief, Jenna."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  // "It was a miracle that I survived that day..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Actor_StartRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
  Event_Wait(10);
  // "We are the ones who saved him."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_SetAnimation(ACTOR_MENARDI, ANIM_NOD);
  // "We saw him floating unconscious in the river as we passed."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 80);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  // "I've been with them ever since... I've experienced a lot."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "But why?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Why did you leave me all alone..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 80);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  // "Jenna..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_FELIX, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(4);
  // "My own brother... I thought you were dead!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Actor_SetAnimation(ACTOR_SATUROS, ANIM_NOD);
  // "Save this teary reunion for later."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  // "That's right. The Elemental Stars come first."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_MENARDI, 10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  // "Is this acceptable? Felix would never allow us to harm you..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_MENARDI, 2);
  // "Felix will be our guarantee. Now, give us the Elemental Stars!"
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
  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    u32 item = *inventoryEntry & INVENTORY_ITEM_ID_MASK;
    inventoryEntry++;
    if (item == ITEM_VENUS_STAR || item == ITEM_MERCURY_STAR ||
        item == ITEM_JUPITER_STAR)
      starCount++;
  }
  // "We don't have any choice..."
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    messageA = MsgSolSanctumGuessTakeElemental;
    Event_SetMessageId(messageA);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    if (starCount <= 2) {
      // "I guess I should take the Elemental Stars to them."
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 30);
      Actor_WalkToAndWait(ACTOR_GARET, 252, 486);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(10);
      // "Isaac handed over the Elemental Stars."
      FieldMessage_ShowCentered((messageA + 1), 1, 0);
    } else {
      Event_SetMessageId(MsgSolSanctumRightTake);
      // "All right, I'll take them."
      SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 30);
    }
  } else {
    if (starCount <= 2) {
      messageB = MsgSolSanctumDontHandOver;
      Event_SetMessageId(messageB);
      Actor_RunRepeatedMotion(ACTOR_GARET, 3);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      // "If you don't hand them over, they won't let Jenna go!"
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
      // "Garet took the Elemental Stars."
      FieldMessage_ShowCentered((messageB + 1), 1, 0);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
      actor->steeringFlags |= ACTOR_FACES_MOTION;
    } else {
      Event_SetMessageId(MsgSolSanctumWontLetGo);
      Actor_RunRepeatedMotion(ACTOR_GARET, 3);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      // "They won't let Jenna go if we don't hand them over!"
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
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
  Actor_MoveToAndWait(ACTOR_GARET, 312, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
  Actor_MoveToAndWait(ACTOR_GARET, 342, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
  Actor_MoveToAndWait(ACTOR_GARET, 376, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
}

/// Alex appears, takes the three stars from Garet and demands the last one.
static void SolSanctumElementalStarRoom_AlexTakesStars(void) {
  u32 frame;
  struct Actor *actor;

  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_ALEX, 392, 454);
  SolSanctumElementalStarRoom_AlexWarpsIn();
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
  // "Oh, Alex... You're late!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 10);
  // "Alex is another of our companions. Hand the stones to him."
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
  // "Oh, great. After we did all that work..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 20);
  // "Garet gave him the Elemental Stars."
  FieldMessage_ShowCentered(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 376, 470);
  Event_Wait(30);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Event_SetMessageId(MsgSolSanctumPermitRelieveElemental + 5);
  // "I'm afraid I must apologize in advance..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  // "For you will be bringing us the remaining Elemental Star, as..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 20);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  actor = Actor_Get(ACTOR_ALEX);
  actor->motionFlags = 0;
  Sound_Play(SE_SHIMMER);
  for (frame = 0; frame != 30; frame++) {
    actor->y.fixed += toFIXED(1);
    Event_Wait(1);
  }
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "Wait a second!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 10);
  // "You want us to grab the other one for you, too?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
  // "Oh, did you not understand me?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "You said you'd set Jenna free once we gave you the Elemental..."
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
  // "No, my friend... We want you to bring us ALL the Elemental..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_SATUROS, 10);
  // "Stop grumbling and bring us the last Star!"
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
  // "Thank you for your cooperation."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  Event_Wait(10);
  // "We'll be waiting for the rest."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_ALEX, 30);
  actor->motionFlags = 0;
  Actor_SetSpeed(ACTOR_ALEX, 2.4);
  FieldActor_SetMoveTarget(actor, 460, 0, 360);
  Actor_WaitForMoveById(ACTOR_ALEX);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
  Event_Wait(30);
  FieldCamera_FollowActor(ACTOR_GARET, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(20);
  actor = Actor_Get(ACTOR_GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
  Actor_MoveToAndWait(ACTOR_GARET, 342, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
  Actor_MoveToAndWait(ACTOR_GARET, 312, 470);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_Wait(30);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
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
  Scene_WalkIntoLeader(ACTOR_GARET);
  PartyInventory_Discard(ITEM_VENUS_STAR);
  PartyInventory_Discard(ITEM_MERCURY_STAR);
  PartyInventory_Discard(ITEM_JUPITER_STAR);
}

/// Stands Saturos, Menardi, Felix and Alex round Jenna and Kraden after the
/// exchange, all turning to face the leader.
static void SolSanctumElementalStarRoom_StandAroundHostages(void) {
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
  actor->watchedActor = (union ActorOrEffect *)leader;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_KRADEN);
  actor->watchedActor = (union ActorOrEffect *)leader;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_MENARDI);
  actor->watchedActor = (union ActorOrEffect *)leader;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_SATUROS);
  actor->watchedActor = (union ActorOrEffect *)leader;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  alex = Actor_Get(ACTOR_ALEX);
  alex->watchedActor = (union ActorOrEffect *)leader;
  alex->steeringFlags |= ACTOR_FACES_MOTION;
  alex->scaleX = FIXED_ONE;
  alex->scaleY = FIXED_ONE;
  alex->motionFlags = Actor_Get(ACTOR_MENARDI)->motionFlags;
  alex->y.fixed = 0;
  Actor_StartScript(alex, gSolSanctumElementalStarRoom_AngleScript);
  actor = Actor_Get(ACTOR_FELIX);
  actor->watchedActor = (union ActorOrEffect *)leader;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartScript(actor, gSolSanctumElementalStarRoom_AngleScript);
  Event_End();
}

/// Isaac searches the statue in the north west of the room (cell (14, 8)) and
/// puts the Mars Star in a Mythril Bag, and the Sanctum starts to shake apart.
///
/// The room flashes red, the star's glow goes out and the star rises over
/// Isaac's head. Garet steps out beside him; a crash shakes the room, the two
/// look about them as it shakes again, and jump in alarm as the screen fades.
/// The party goes on in the eruption, the same room breaking up (SceneId_
/// SolSanctumEruption). The statue can be searched only until then
/// (FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED).
static void SolSanctumElementalStarRoom_TakeMarsStar(void) {
  u8 pass;
  s32 member;

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
  // "...put the Mars Star in a Mythril Bag."
  FieldMessage_Show(MsgSolSanctumMarsStarBagged, 1);
  Actor_SetPosition(ACTOR_JENNA, 307, 277);
  Actor_SetPosition(ACTOR_KRADEN, 307, 277);
  Actor_SetPosition(ACTOR_MENARDI, 307, 277);
  Actor_SetPosition(ACTOR_SATUROS, 307, 277);
  Actor_SetPosition(ACTOR_ALEX, 307, 277);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
  Event_Wait(10);
  Scene_PlaceAtLeader(ACTOR_GARET);
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
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION);
}

/// Kraden, on the middle platform, sends the boys for the stars: "Now, put the
/// Elemental Stars in those bags and bring them here," and once the Venus
/// Star is taken, "Good, good. Now go fetch the others!"
///
/// Once Saturos holds him, he says only, "I am sorry, everyone... I am
/// responsible for all of this." His Mind Read row shows that line at any
/// time, though neither Isaac nor Garet knows Mind Read yet.
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
    // "Good, good. Now go fetch the others!"
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
  Event_End();
}

/// Jenna, on the middle platform, says "Be careful, both of you," and once the
/// Venus Star is taken, teases "Kraden, you're as giddy as a schoolgirl."
///
/// Once Saturos holds her, she wonders, "My brother is alive... but...why
/// didn't he tell me sooner?" Her Mind Read row runs the same.
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
    // "Kraden, you're as giddy as a schoolgirl."
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Event_End();
}

/// Saturos, standing over the hostages, says "If you want to save them, you'll
/// bring us the final Elemental Star." His Mind Read row shows the same.
static void SolSanctumElementalStarRoom_TalkToSaturos(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumSaturosBringTheFinalStar);
  // "If you want to save them, you'll bring us the final..."
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_End();
}

/// Menardi, standing over the hostages, snaps "What are you doing? Bring us
/// the final gem!" Her Mind Read row runs the same.
static void SolSanctumElementalStarRoom_TalkToMenardi(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumMenardiBringTheFinalGem);
  // "What are you doing? Bring us the final gem!"
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_End();
}

/// Felix, unmasked beside the hostages, says nothing: "..."
static void SolSanctumElementalStarRoom_TalkToFelix(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumFelixSilence);
  // "..."
  Event_ShowMessage(ACTOR_FELIX, 0);
  Event_End();
}

/// Alex, holding the three stars, says "There is only one left. You're almost
/// done." His Mind Read row shows the same.
static void SolSanctumElementalStarRoom_TalkToAlex(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumAlexOnlyOneLeft);
  // "There is only one left. You're almost done."
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_End();
}

/// Walking north from the middle platform into the beam of light the party
/// came through (cells (29, 17) and (29, 18)) has Kraden call out "There
/// aren't any Elemental Stars out there!", and Isaac stays in the room.
static void SolSanctumElementalStarRoom_KradenStopsPartyLeaving(void) {
  Event_Start();
  Event_SetMessageId(MsgSolSanctumNoStarsOutThere);
  // "There aren't any Elemental Stars out there!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Event_End();
}

/// Isaac jumps across a gap to the next pillar when he walks off one of three
/// edges: east off the Venus statue's pillar to the slender pillar beside it
/// (cell (45, 28)), east at cell (15, 21) on the west side, or south off the
/// middle platform (cell (29, 22)).
///
/// Field_TryJumpForward makes the jump, two cells ahead.
static void SolSanctumElementalStarRoom_JumpAhead(void) {
  Field_TryJumpForward();
}

/// Shows one of the Elemental Stars, Kraden beside it, while he describes it,
/// then turns the view back to the party; the camera's point and Kraden's spot
/// are in whole pixels.
static inline void SolSanctumElementalStarRoom_ShowElementalStar(
    s32 cameraX, s32 cameraZ, s32 actorX, s32 actorZ, s32 speaker) {
  FieldCamera_MoveToFX(toFIXED(cameraX), CAMERA_KEEP, toFIXED(cameraZ), 1);
  FieldCamera_WaitForMove();
  Actor_SetPosition(ACTOR_KRADEN, actorX, actorZ);
  Event_Wait(40);
  Event_ShowMessage(speaker, 0);
  Actor_SetPosition(ACTOR_KRADEN, 471, 360);
  Event_Wait(80);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 370, 1);
  FieldCamera_WaitForMove();
}

/// The first time through the portal, Isaac floats down onto the middle
/// platform with Garet and Jenna, Kraden follows, and he tells them of the
/// Elemental Stars before sending the boys to fetch them with four Mythril
/// Bags.
///
/// Kraden names each star as the view goes to its statue, speaks of the Wisdom
/// Stone, then steps out onto the stone and nearly slips ("Be careful, Kraden!
/// It looks slippery."). Jenna asks Isaac to fetch the stars for him, and asks
/// again until he says yes. Jenna leaves the party here and stays with
/// Kraden.
static void SolSanctumElementalStarRoom_EnterStarRoom(void) {
  u16 frame;
  struct Actor *rising;
  struct Sprite *sprite;
  struct Actor *shadow;

  Event_Start();
  MapGrid_CopyCellAttributes(27, 16, 5, 1, 27, 17);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 261, 0);
  FieldCamera_WaitForMove();
  MapGrid_CenterOnFollowedPoint();
  // Materialize Isaac above the arrival platform with the expanding shadow.
  shadow = Actor_Get(ACTOR_PLATFORM_SHADOW);
  shadow->scaleX = FIXED_ONE / 10;
  shadow->scaleY = FIXED_ONE / 10;
  rising = Actor_Get(ACTOR_PARTY_LEADER);
  sprite = rising->sprite;
  sprite->flags = 0;
  rising->scaleX = FIXED_ONE / 10;
  rising->scaleY = FIXED_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 471, 290);
  rising->motionFlags = 0;
  rising->y.fixed = toFIXED(40);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  gFieldState->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 471, 290);
  Sound_Play(SE_WARP);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  for (frame = 0; frame != 90; frame++) {
    rising->y.fixed -= toFIXED(1) / 10;
    rising->scaleX += FIXED_ONE / 100;
    rising->scaleY += FIXED_ONE / 100;
    shadow->scaleX += FIXED_ONE / 100;
    shadow->scaleY += FIXED_ONE / 100;
    Event_Wait(1);
  }
  rising->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
  Event_Wait(80);
  FieldCamera_SetSpeed(0.3);
  FieldCamera_MoveTo(471, CAMERA_KEEP, 290, 1);
  for (frame = 0; frame != 60; frame++) {
    rising->y.fixed -= toFIXED(1) / 2;
    Event_Wait(1);
  }
  rising->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
  sprite->flags = SPRITE_FLAG_SHADOW;
  Actor_SetPosition(ACTOR_PLATFORM_SHADOW, 0, 0);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 471, 310);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Scene_PlaceAtLeader(ACTOR_JENNA);
  Scene_PlaceAtLeader(ACTOR_GARET);
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
  sprite = rising->sprite;
  sprite->flags = 0;
  rising->scaleX = FIXED_ONE / 10;
  rising->scaleY = FIXED_ONE / 10;
  shadow->scaleX = FIXED_ONE / 10;
  shadow->scaleY = FIXED_ONE / 10;
  Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
  Actor_SetPosition(ACTOR_KRADEN, 471, 290);
  rising->motionFlags = 0;
  rising->y.fixed = toFIXED(40);
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
  for (frame = 0; frame != 90; frame++) {
    rising->y.fixed -= toFIXED(1) / 10;
    rising->scaleX += FIXED_ONE / 100;
    rising->scaleY += FIXED_ONE / 100;
    shadow->scaleX += FIXED_ONE / 100;
    shadow->scaleY += FIXED_ONE / 100;
    Event_Wait(1);
  }
  rising->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
  Event_Wait(80);
  for (frame = 0; frame != 60; frame++) {
    rising->y.fixed -= toFIXED(1) / 2;
    Event_Wait(1);
  }
  rising->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(30);
  Actor_SetSpritePriority(ACTOR_KRADEN, 1);
  Actor_Get(ACTOR_KRADEN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_NORMAL);
  sprite->flags = SPRITE_FLAG_SHADOW;
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
  // "Is that. . . the ocean?"
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(40);
  // "No, it couldn't be... The ocean has waves."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 30);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "The ocean... What's that?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  // "The ocean is a thing of endless water at the end of the world."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "You would not understand... Someday, I'll show it to you myself."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 10);
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
  // "The Elemental Stars..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(20);
  // "They really exist... The Elemental Stars exist!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 440, 346);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 10);
  // "You taught us about those..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 495, 346);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  // "Stones, wood, even us... All things are made from those four elements.
  // Did I get it right, Isaac?"
  // Yes: Kraden, "Exactly..."
  // No: Kraden, "No, Isaac... Garet is quite correct."
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldState->message++;
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 30);
  // Kraden explains the elements and the stars at the room's four corners.
  Event_SetMessageId(MsgSolSanctumKradenTheElementsAreThe);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "As I have told you many times, the elements are the source of..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Earth, water, fire, and wind-these are the four elements."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "Legend holds that the four Elemental Stars contain the purified..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "And now, those legendary stones are before us!"
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
  // Kraden tours the four Elemental Stars, the view returning to the party
  // after each.
  // "The Venus Star... The elemental stone of earth!"
  SolSanctumElementalStarRoom_ShowElementalStar(
      711, 464, 589, 353, SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  // "Mercury Star, possessing the power of water's many forms!"
  SolSanctumElementalStarRoom_ShowElementalStar(
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
  // "There is the Mars Star, glowing red with the fire contained within!"
  SolSanctumElementalStarRoom_ShowElementalStar(
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
  // "And Jupiter Star, bound with the wind's fury!!!"
  SolSanctumElementalStarRoom_ShowElementalStar(
      231, 464, 306, 362, SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  // "Kraden... Why are you getting so excited?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "How can you be so calm in the face of such wonders!?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  // "You're looking at the very source of all Alchemy!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "...Haven't I told you all this before?"
  // Yes: Garet, "What? I must have fallen asleep, then, because I don't have a
  // clue what he's talking about!"
  // No: Garet, "Then this must be a lot for you to take in."
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldState->message++;
  }
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  // Discuss the Wisdom Stone and the power stored in the Elemental Stars.
  Event_SetMessageId(MsgSolSanctumKradenTheWisdomStone);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  // "The Wisdom Stone... the Stone of Sages."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "This stone dominates everything..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  // "You understand now, don't you?"
  // Yes: "The study of Alchemy began with the search for this very stone."
  // No: "You still do not understand? Alchemy exists because of the Elemental
  // Stars."
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldState->message++;
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Event_SetMessageId(MsgSolSanctumTheStarsHaveEnormousPower);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "I don't really get how it works, but they have enormous power..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  // "Beyond enormous... With a single gem, one could conquer the..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  // "Now, perhaps, you begin to understand."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_Jump(ACTOR_GARET, 4, 40);
  // "Don't scare us like that, Kraden..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 80);
  // "Who can say what the truth is?"
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
  // "That's why I've been searching for the Elemental Stars..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
  rising = Actor_Get(ACTOR_KRADEN);
  // Kraden steps out towards the stars and nearly slips, then asks the boys to
  // help.
  wait(6);
  rising->speedLimit = toFIXED(3);
  rising->acceleration = toFIXED(2);
  Sound_Play(SE_HIGH_JUMP);
  rising->velocityY = toFIXED(6);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 395);
  wait(6);
  Actor_SetSpeed(ACTOR_KRADEN, 0.3);
  rising->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_KRADEN, 473, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 469, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_SetDestination(ACTOR_KRADEN, 471, 395);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  // "Now they lie before me..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 411);
  Actor_SetSpeed(ACTOR_KRADEN, 0.3);
  rising->steeringFlags &= ~ACTOR_FACES_MOTION;
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
  // "Whoa..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_KRADEN, 10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 395);
  Actor_SetMovementTypeById(ACTOR_KRADEN, MOVEMENT_TYPE_STAND);
  Event_Wait(30);
  rising->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  wait(6);
  rising->speedLimit = toFIXED(3);
  rising->acceleration = toFIXED(2);
  Sound_Play(SE_HIGH_JUMP);
  rising->velocityY = toFIXED(6);
  Actor_MoveToAndWait(ACTOR_KRADEN, 471, 360);
  wait(6);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  // "Be careful, Kraden! It looks slippery."
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "But I must examine the Elemental Stars..."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 30);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 30);
  // "Make the boys do it!"
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
  // "Oh, I couldn't ask . . . Would you mind?"
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
  // "He'd be glad to get the stones for you, right, Isaac?"
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
  // "Thank you!"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_SetSpeed(ACTOR_KRADEN, 0.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 471, 352);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  // "You'll need this, Isaac."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 60);
  rising->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_KRADEN, 456, 360);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgSolSanctumTheBagsAreFromKradens);
  // "You brought those from your cottage, right, Kraden?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_JENNA, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What are those bags?"
  SolSanctumElementalStarRoom_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "They're made of woven threads of Mythril."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  // "I prepared them for just such an occasion."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "Now, go fetch the Elemental Stars and put them in those bags."
  SolSanctumElementalStarRoom_ShowMessageAndWait(
      SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 30);
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
  Scene_WalkIntoLeader(ACTOR_GARET);
  GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
  Party_RemoveMemberRestored(JENNA);
  SolSanctumElementalStarRoom_GiveMythrilBags();
  MapGrid_CopyCellAttributes(8, 0, 5, 1, 27, 17);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sets up the Elemental Star Room.
s32 Scene_Initialize(void) {
  u32 lightActorId;

  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  lightActorId = ACTOR_FIRST_FLOATING_LIGHT;
  do {
    Actor_Get(lightActorId)->collisionFlags = 0;
    Actor_SetSpritePriority(lightActorId++, 1);
  } while (lightActorId <= ACTOR_LAST_FLOATING_LIGHT);
  // It asks for 16 lights; the task keeps the ten the room places.
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
    SolSanctumElementalStarRoom_StandAroundHostages();
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
    if (gMainRoutineState.entrance ==
        SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
      SolSanctumElementalStarRoom_EnterStarRoom();
    }
  }
  return 0;
}

/// Shows the event's next line at the speaker, then waits the frames.
static void SolSanctumElementalStarRoom_ShowMessageAndWait(s32 speaker,
                                                           s32 frames) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(frames);
}

/// Animates a star's icon and replaces its owner's mythril bag with the star.
static s32 SolSanctumElementalStarRoom_PresentStarFX(s32 item, FIXED x, FIXED y,
                                                     FIXED z) {
  IconTransfer *glyph;
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
  Actor_StartScript(actor, gItemIcon_GrowScript);
  sprite = actor->sprite;
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  actor->velocityY = toFIXED(5) / 2;
  actor->gravity = toFIXED(1) / 4;
  glyph = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(*glyph));
  Icon_DrawItem(item);
  VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  Sound_Play(MUS_TREASURE_FOUND);
  ItemDisplay_ShowPicture(actor, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
  Inventory_Discard(member, slot);
  Inventory_AddItem(member, item);
  Actor_Release(actor);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  return member;
}

/// Makes room among the items and gives the party four Mythril Bags, keeping
/// the dialogue's place.
static void SolSanctumElementalStarRoom_GiveMythrilBags(void) {
  struct FieldState *state = gFieldState;
  s16 savedMessage = state->message;
  s32 itemSlot;
  s32 itemOwner;
  s32 freeSlots;

  Sound_Play(MUS_TREASURE_FOUND);
  Item_ShowFound(ITEM_MYTHRIL_BAG, 3);
  // "Isaac got four Mythril Bags."
  FieldMessage_Show(MsgSolSanctumGotFourMythrilBags, 1);
  do {
    freeSlots = 2 * INVENTORY_SLOTS - Inventory_Count(ISAAC);
    freeSlots -= Inventory_Count(GARET);

    if (freeSlots <= 3) {
      // "You have too many items. Will you drop something?"
      FieldMessage_Show(MsgSolSanctumTooManyItems, 1);
      if (ItemPicker_Run(&itemOwner, &itemSlot) != -1)
        Inventory_Discard(itemOwner, itemSlot);
    }
  } while (freeSlots <= 3);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  PartyInventory_Add(ITEM_MYTHRIL_BAG);
  state->message = savedMessage;
}

/// Alex warps in where he has been placed, hidden.
static void SolSanctumElementalStarRoom_AlexWarpsIn(void) {
  struct EffectOptions options;
  struct Actor *alex;
  u32 frame;
  FIXED x;
  FIXED xOffset;

  alex = Actor_Get(ACTOR_ALEX);
  Sound_Play(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_PARTICLE;
  options.startScaleX = 2 * FIXED_ONE / 5;
  options.startScaleY = 3 * FIXED_ONE;
  for (frame = 0; frame < WARP_FRAMES; frame++) {
    Event_Wait(1);
    if (!(frame & 1)) {
      xOffset = toFIXED(Random_Below(24));
      x = alex->x.fixed + xOffset;
      x -= toFIXED(12);
      FieldEffect_Spawn(x,
                        alex->y.fixed + toFIXED(Random_Below(32)) + toFIXED(32),
                        alex->z.fixed, 0, -toFIXED(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    }
    if (frame == WARP_IN_BLINK_FRAME)
      Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  }
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

#include "shared/sol_sanctum_elemental_star_lights.c"

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSolSanctumElementalStarRoom_AngleScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/item_icon_grow_script.c"

#include "shared/sol_sanctum_star_room_entrances.c"

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
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_STATUE, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, EVENT(SolSanctumElementalStarRoom_BagVenusStar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_STATUE, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, EVENT(SolSanctumElementalStarRoom_BagMercuryStar) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_STATUE, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, EVENT(SolSanctumElementalStarRoom_BagJupiterStarAndMeetSaturos) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_STATUE, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, EVENT(SolSanctumElementalStarRoom_TakeMarsStar) },
    { TRIGGER_TABLE_END },
};

#include "shared/sol_sanctum_star_cells.c"

#include "shared/sol_sanctum_elemental_star_light_tables.c"
// clang-format on
