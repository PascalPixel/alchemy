/// @file
/// Sol Sanctum (SceneId_SolSanctumLunaSolRooms), the room of Sol and the room
/// of Luna at the heart of the sanctum.
///
/// The rooms lie between SceneId_SolSanctumReliefs (sol_sanctum_entrance.c),
/// the statue room (SceneId_SolSanctumStatue, sol_sanctum_statue.c) and the way
/// up to SceneId_SolSanctumElementalStarRoom.
///
/// The way in from the second relief room is at the south-west (cell
/// (3, 31)). Its left branch leads to a chest with a Herb (cell (3, 15)); the
/// middle one runs north into the room of Sol, four winged statues round a
/// golden sun on a purple and gold checkered floor, entered by its south
/// doorway (cells (17, 15) to (18, 18)). A short passage east (cells (25, 10)
/// to (28, 11)) joins it to the room of Luna, the same room bathed in blue,
/// with a moon on its floor. South of the room of Luna a corridor leads to the
/// arched stairs up to the statue room (cell (40, 19)).
///
/// Kraden's events here follow the story flags in order:
/// - SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum, on the first step onto
///   the symbol of the sun on the floor (FLAG_SOL_SANCTUM_FOUND). From then
///   until the way up opens, Retreat is blocked and
///   SolSanctumLunaSolRooms_KradenStopsPartyLeaving turns the party back from
///   the way out.
/// - SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms, the first time the party
///   crosses into the Luna room (FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN). Kraden
///   finds the picture of Luna and stays by it from then on.
/// - Each statue pushed onto a seal in the statue room lights a quarter of the
///   pictures (FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN to _SOUTHEAST_SEAL_OPEN),
///   and the first one cuts here to Kraden. Before the trap is disarmed, lights
///   fall through the Luna room (SolSanctumLunaSolRooms_SpringTrap,
///   FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) and every quarter goes dark again.
///   Afterwards the quarter only flashes and Kraden runs off to the statue room
///   (SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed,
///   FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING). Both take the party back to
///   the statue room by the exit named for the event.
/// - With all four quarters lit, Kraden sees that the picture of Luna has
///   become Sol (SolSanctumLunaSolRooms_KradenSeesLunaPictureChange,
///   FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) and that the floor's symbol has
///   become Luna (SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna,
///   FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN). Searching the symbol then opens the
///   floor, and Isaac and Kraden rise through it to the Elemental Star Room
///   (SolSanctumLunaSolRooms_RiseToElementalStarRoom,
///   FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN).
/// - After the eruption, the Wise One sends Isaac and Garet here from the
///   collapsing Elemental Star Room (SceneId_SolSanctumEruption,
///   sol_sanctum_eruption.c), and they set off for the way out
///   (SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue,
///   FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN). From then on the rooms rumble.
///
/// The rooms' colours follow the party. Stepping into the room of Luna fades
/// everything to blue, stepping back into the room of Sol or out into the
/// corridors fades it back, and the passage between them is dimmed brown.
/// Once all four quarters are lit the two rooms swap: the room of Sol turns
/// blue and the room of Luna plain. Cells in the doorways do this (enum
/// SolSanctumLunaSolRoomsCellBehavior), and one of three scene flags
/// remembers which colours show, so a return after a battle keeps them.
///
/// On entry Scene_Initialize copies each lit quarter into its
/// FLAG_SOL_SANCTUM_*_LIGHT_SHOWN. It redraws the pictures, the floor and the
/// open way up from the story flags, unless the entrance's own event draws
/// them.
///
/// The falling lights are four tasks, each with a random timer of its own, that
/// the trap adds and removes.
///
/// A third statue-room event is never reached. In it the quarters lit since the
/// room last showed them flash for Kraden
/// (SolSanctumLunaSolRooms_UnusedAnimateSavedStatueLights).

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

u16 gSolSanctumLunaSolRooms_Light2Timer BSS_DATA;
u16 gSolSanctumLunaSolRooms_Light3Timer BSS_DATA ALIGNED(4);
u16 gSolSanctumLunaSolRooms_Light1Timer BSS_DATA ALIGNED(4);
/// Set to 2 as a falling light lands: the rooms shake for a frame, then
/// settle.
s32 gSolSanctumLunaSolRooms_LandingShake BSS_DATA;
u16 gSolSanctumLunaSolRooms_Light4Timer BSS_DATA;

extern struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[];
extern u32 gSolSanctumLunaSolRoomsExits[];
extern struct ActorPlacement gSolSanctumLunaSolRoomsActorPlacements[];
extern struct Trigger gSolSanctumLunaSolRoomsTriggers[];

/// The placed actors, numbered in the order of
/// gSolSanctumLunaSolRoomsActorPlacements; Jenna and Garet, its last rows,
/// keep their characters' ids.
enum SolSanctumLunaSolRoomsActorId {
  /// Eight statues, four in each room, with the sprite of
  /// SceneId_SolSanctumStatue's statues (sol_sanctum_statue.c).
  ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN = ACTOR_FIRST_STATUE + 8
};

/// The rooms of Sol and Luna show their plain colours (PALETTE_TINT_NONE), so a
/// return after a battle keeps them.
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT FLAG_SCENE_TEMP_00
/// The rooms of Sol and Luna show SOL_SANCTUM_BLUE_TINT, so a return after a
/// battle keeps it.
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED FLAG_SCENE_TEMP_01
/// The rooms of Sol and Luna show SOL_SANCTUM_WARM_TINT, so a return after a
/// battle keeps it.
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED FLAG_SCENE_TEMP_02

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// Set once Isaac and Garet's arrival after the Wise One's rescue has played.
#define FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN FLAG_AREA_TEMP_09

/// The blue tint the rooms fade to (PaletteTint's TINT form).
#define SOL_SANCTUM_BLUE_TINT (PALETTE_TINT_CUSTOM | RGB(12, 14, 20))
/// The dim red tint the rooms fade to.
#define SOL_SANCTUM_WARM_TINT (PALETTE_TINT_CUSTOM | RGB(17, 13, 11))

// A falling light in the Luna room is a column of five cells kept in a
// staging column on row 59, with the ten cells it falls through, unlit, ten
// rows above it on row 49.
/// Draws a light with its top at (column, row).
#define DRAW_LIGHT(staging, column, row)                                       \
  MapGrid_CopyCellsTo(staging, 59, column, row, 1, 5)
/// Moves a light a row down to (column, row). Its first cell is copied over the
/// row it leaves, then it is drawn again.
#define MOVE_LIGHT_DOWN(staging, column, row)                                  \
  (MapGrid_CopyCellsTo(staging, 59, column, (row) - 1, 1, 1),                  \
   DRAW_LIGHT(staging, column, row))
/// Puts back the ten unlit cells a light falls through, from (column, row).
#define CLEAR_LIGHT_PATH(staging, column, row)                                 \
  MapGrid_CopyCellsTo(staging, 49, column, row, 1, 10)

/// The first of the two layout rows holding the Luna room's picture with its
/// quarters lit, the picture of Sol, which MapGrid_CopyCellsTo draws into the
/// room.
#define STATUE_LIGHTS_LIT_ROW 28
/// The first of the two layout rows holding the same picture unlit, the
/// picture of Luna.
#define STATUE_LIGHTS_DARK_ROW 30

/// The first of the map's two tileset animations that stay paused until the
/// floor opens the way up to the Elemental Star Room.
#define WAY_UP_ANIMATION_A 9
/// The second of them.
#define WAY_UP_ANIMATION_B 10

/// The cell behaviors of the floor cells the events answer.
///
/// Behaviors 1 and 2 are the exits.
enum SolSanctumLunaSolRoomsCellBehavior {
  /// The room of Luna's south doorway, where Kraden calls Isaac over once the
  /// picture has changed.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_LUNA_DOORWAY = 8,
  /// The way back out to SceneId_SolSanctumReliefs.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WAY_OUT = 9,
  /// The symbol of the sun on the floor, first stepped on.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_HEART_OF_SANCTUM = 11,
  /// Fades to SOL_SANCTUM_WARM_TINT.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINT = 12,
  /// Fades to plain colours, or to SOL_SANCTUM_BLUE_TINT once all four
  /// quarters are lit.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT = 13,
  /// Fades to SOL_SANCTUM_BLUE_TINT, or to plain colours once all four
  /// quarters are lit.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT_INVERSE = 14,
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_PLAIN =
      15, ///< fades to plain colours
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BLACK_BACKDROP =
      19, ///< turns the backdrop black
  /// The symbol on the floor, searched to open the way up.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_FLOOR_SYMBOL = 20,
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST = 100
};

static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void);
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void);
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void);
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames);
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumLunaSolRoomsEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumLunaSolRoomsExits; }

/// Returns the eight statues, four in each room, Kraden, Jenna and Garet, all
/// always placed.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gSolSanctumLunaSolRoomsActorPlacements;
}

/// Returns the floor's cell behaviors that tint the rooms, Kraden's talk
/// and events, the floor symbol and the Herb's chest.
const struct Trigger *Scene_GetTriggers(void) {
  return gSolSanctumLunaSolRoomsTriggers;
}

/// Isaac steps into the room of Sol, from the passage at its east (cells
/// (24, 10) and (24, 11)) or through its south doorway (cells (17, 16) and
/// (18, 16)), and the colours fade back from blue to plain. Once all four
/// quarters are lit it is the other way round: the room of Sol fades to blue,
/// and the first time Kraden comes to point out that the symbol on the floor
/// has changed from Sol to Luna.
static void SolSanctumLunaSolRooms_TintSolRoom(void) {
  s32 paletteActive;

  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
    if (paletteActive == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
        if (GameFlag_Get(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
          SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna();
        }
      }
      Event_End();
    }
  } else {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT) == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      Event_End();
    }
  }
}

/// Isaac walks south out of the room of Sol or of Luna into the corridor below
/// (cells (17, 18) to (18, 18) or (35, 17) to (36, 17)), and the colours fade
/// back to plain if they were tinted. Coming from a plain room, nothing
/// visible happens.
static void SolSanctumLunaSolRooms_RestorePlainTint(void) {
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT) == 0) {
    Event_Start();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(20);
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
    Event_End();
  }
}

/// Isaac steps into the room of Luna, from the passage at its west (cells
/// (29, 10) and (29, 11)) or through its south doorway (cells (35, 15) and
/// (36, 15)), and everything fades to blue. The first time, Kraden steps out:
/// "Wh-What is this room?" He finds the picture of Luna, a moon in the centre,
/// and asks Isaac to look farther up the passage. Once all four quarters are
/// lit the room of Luna fades back to plain colours instead.
static void SolSanctumLunaSolRooms_TintLunaRoom(void) {
  s32 paletteActive;

  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
    if (paletteActive == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      Event_End();
    }
  } else {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED) == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      Event_End();
    }
  }
}

/// Isaac walks along the short passage between the rooms of Sol and Luna
/// (cells (25, 10) to (28, 11)), and the colours dim to a warm brown
/// (SOL_SANCTUM_WARM_TINT).
static void SolSanctumLunaSolRooms_TintPassageBetweenRooms(void) {
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) == 0) {
    PaletteFade_ApplyTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
    PaletteFade_StartInterpolation(20);
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  }
}

/// Nothing visible happens when Isaac walks towards either way out, the arched
/// stairs up to the statue room (cells (39, 20) to (41, 20)) or the way back
/// to the relief rooms (cells (4, 29) and (4, 30)). The game sets the colour
/// behind the map back to pure black, which the room tints also shade, so that
/// the next room fades in from black.
static void SolSanctumLunaSolRooms_ClearBackdropColor(void) {
  s32 black = RGB(0, 0, 0);
  u16 *backdropColor = (u16 *)&PLTT_BACKDROP;

  *backdropColor = black;
}

/// Draws the light of each lit quarter of the Luna room's picture, from four
/// consecutive flags, with the tiles of the given source row.
static inline void SolSanctumLunaSolRooms_DrawStatueLightTiles(s32 firstFlag,
                                                               s32 sourceRow) {
  if (GameFlag_Get(firstFlag) != 0)
    MapGrid_CopyCellsTo(45, sourceRow, 34, 10, 2, 1);
  if (GameFlag_Get(firstFlag + 1) != 0)
    MapGrid_CopyCellsTo(47, sourceRow, 36, 10, 2, 1);
  if (GameFlag_Get(firstFlag + 2) != 0)
    MapGrid_CopyCellsTo(45, sourceRow + 1, 34, 11, 2, 1);
  if (GameFlag_Get(firstFlag + 3) != 0)
    MapGrid_CopyCellsTo(47, sourceRow + 1, 36, 11, 2, 1);
}

/// Draws the lights of the quarters lit now (the _SEAL_OPEN flags).
static void SolSanctumLunaSolRooms_DrawCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, STATUE_LIGHTS_LIT_ROW);
}

/// Draws the lights the room last showed (the _LIGHT_SHOWN flags).
static void SolSanctumLunaSolRooms_DrawSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, STATUE_LIGHTS_LIT_ROW);
}

/// Draws the quarters lit now unlit again.
static void SolSanctumLunaSolRooms_DarkenCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, STATUE_LIGHTS_DARK_ROW);
}

/// Draws the quarters the room last showed lit unlit again.
static void SolSanctumLunaSolRooms_DarkenSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, STATUE_LIGHTS_DARK_ROW);
}

/// Flashes the lit quarters' lights on and off at this pace.
static inline void SolSanctumLunaSolRooms_FlashCurrentStatueLights(s32 frames) {
  Sound_Play(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Event_Wait(frames);
  Sound_Play(SE_GLITTER);
  SolSanctumLunaSolRooms_DarkenCurrentStatueLights();
  Event_Wait(frames);
}

/// Flashes the lights last shown on and off at this pace.
static inline void SolSanctumLunaSolRooms_FlashSavedStatueLights(s32 frames) {
  Sound_Play(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Event_Wait(frames);
  Sound_Play(SE_GLITTER);
  SolSanctumLunaSolRooms_DarkenSavedStatueLights();
  Event_Wait(frames);
}

/// The first time Isaac walks into the room of Sol (cells (17, 15) and
/// (18, 15)), the music changes and Kraden hurries past him to the sun on the
/// floor: "This is it! We've found it!" It is the mark of the sun, the heart of
/// Sol Sanctum, so Saturos and Menardi told the truth and must be thieves.
/// Jenna wants to hurry back to the village, but Kraden asks to go a little
/// farther, and whatever Isaac answers, Garet sides with him.
///
/// From then on Retreat does not work here. After the eruption this only fades
/// the room back to plain colours.
static void SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    SolSanctumLunaSolRooms_TintSolRoom();
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_FOUND) == 0) {
    Event_Start();
    // The leader walks in as the music changes.
    Event_SetMessageId(MsgSolSanctumFound);
    Sound_Play(SOUND_FADE_OUT_BGM);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Event_Wait(20);
    Sound_Play(MUS_FOREST_THEME);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    // Kraden hurries out of the leader to the symbol on the floor.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_KRADEN, 1.4);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 206);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_Jump(ACTOR_KRADEN, 4, 60);
    // Kraden: "This is it! We've found it!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    // Garet and Jenna step out behind the leader.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GARET, 0.5);
    Actor_SetSpeed(ACTOR_JENNA, 0.5);
    Actor_WalkTo(ACTOR_GARET, 280, 248);
    Actor_WalkToAndWait(ACTOR_JENNA, 296, 248);
    Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(288, CAMERA_KEEP, 213, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 0.4);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 176);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    // Kraden: the mark of the sun, the heart of Sol Sanctum; so those two told
    // the truth.
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 60);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 10);
    // Jenna: then they must have been... Garet: "They were thieves!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_JENNA, 10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 6);
    Actor_SetAttachedEffect(ACTOR_JENNA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
    // Jenna: "Let's hurry back to the village and tell everybody!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 20);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // Garet: "Kraden? What's the matter?" Kraden: "We're so close..." The
    // three wonder at turning back from the heart of the sanctum.
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_Wait(40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_JENNA, 60);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 60);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
    Event_Wait(6);
    // Kraden: "Shall we go a little farther?" Yes: "Thank you!" No: then go
    // back without him.
    Event_OpenMessage(ACTOR_KRADEN, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessageId(MsgSolSanctumThank);
    } else {
      Event_SetMessageId(MsgSolSanctumGoBackVillage);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    }
    Actor_Jump(ACTOR_KRADEN, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    // Either way Jenna gives in and Garet sides with Kraden, who is
    // delighted: "Good! It's decided. Let's move ahead!"
    Event_SetMessageId(MsgSolSanctumPutWayDont);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 6);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 20);
    Actor_Jump(ACTOR_KRADEN, 6, 20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_SetSpeed(ACTOR_GARET, 1);
    Actor_SetSpeed(ACTOR_JENNA, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 2);
    Scene_WalkIntoLeader(ACTOR_KRADEN);
    Scene_WalkIntoLeader(ACTOR_GARET);
    Scene_WalkIntoLeader(ACTOR_JENNA);
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    GameFlag_Set(FLAG_SOL_SANCTUM_FOUND);
    Event_End();
  }
}

/// The first time the party steps into the room of Luna, Kraden finds the
/// picture of Luna on its floor, "a moon in the center", wonders how the two
/// rooms are connected, and asks Isaac to look farther up the passage. He
/// stays by the picture from then on.
static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void) {
  struct Actor *leader;
  s32 request;

  // Kraden steps out beside Isaac: "What is this room?"
  Event_SetMessageId(MsgSolSanctumWhRoom);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 488, 176);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 472, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(575, CAMERA_KEEP, 181, 1);
  FieldCamera_WaitForMove();
  Event_Wait(120);
  SolSanctumLunaSolRooms_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 80);
  FieldCamera_MoveTo(492, CAMERA_KEEP, 168, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  // Kraden: "These rooms must be connected somehow, don't you think?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgSolSanctumLunaSolRooms);
  } else {
    Event_SetMessageId(MsgSolSanctumRoomLunaOne);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            10);
  // Kraden: would Isaac look farther up the passage? Yes: "Let me know what
  // you find." No: "Then just wait there."
  request = MsgSolSanctumMeanLookFarther;
  Event_SetMessageId(request);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "What does it all mean? Isaac, would you look farther up the..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(request + 1);
    GameFlag_Set(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN);
  } else {
    Event_SetMessageId(request + 2);
  }
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            4);
  FieldCamera_FollowActor(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 486, 131);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 120);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 2);
  FieldCamera_SetSpeed(4);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
}

/// Isaac walks south out of the room of Sol (cells (17, 17) and (18, 17))
/// after Kraden has found the heart of the sanctum, and Kraden hurries over:
/// "That way leads out of the sanctum. It should be in these rooms." Isaac
/// nods and steps back in. This stops once the way up is open, and after the
/// eruption the cell only fades the colours back to plain.
static void SolSanctumLunaSolRooms_KradenStopsPartyLeaving(void) {
  struct Actor *kraden;
  struct Actor *leader;

  kraden = Actor_Get(ACTOR_KRADEN);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_FOUND) == 0) {
    return;
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    SolSanctumLunaSolRooms_RestorePlainTint();
    return;
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    return;
  }
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Event_SetMessageId(MsgSolSanctumWayLeadsOutSanctum);
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Event_Wait(4);
    Actor_SetSpeed(ACTOR_KRADEN, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
             kraden->x.fixed > toFIXED(340)) {
    Actor_SetPosition(ACTOR_KRADEN, 392, 168);
    Event_Wait(4);
    Actor_SetSpeed(ACTOR_KRADEN, 2);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      kraden->x.fixed > toFIXED(340)) {
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  } else {
    // DEAD: the flag's value is dropped; the ROM keeps its read.
    GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
  // "That way leads out of the sanctum. It should be in these rooms."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    Scene_WalkIntoLeader(ACTOR_KRADEN);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 248);
  }
  Event_End();
}

/// One frame of a falling light in the Luna room, counted on `timer`.
///
/// Three frames in four it advances its timer: the light appears at (column,
/// top) from its staging column, falls four rows and flashes, and on frame
/// `cleared` its path is put back. It starts again after `restart` frames and
/// up to 40 more.
#define STEP_FALLING_LIGHT(timer, staging, column, top, cleared, restart)      \
  if ((Random_Next() & 3) != 0) {                                              \
    switch (timer) {                                                           \
    case 0:                                                                    \
      Sound_Play(SE_FALLING_LIGHT);                                            \
      DRAW_LIGHT(staging, column, top);                                        \
      break;                                                                   \
    case 1:                                                                    \
      MOVE_LIGHT_DOWN(staging, column, (top) + 1);                             \
      break;                                                                   \
    case 2:                                                                    \
      MOVE_LIGHT_DOWN(staging, column, (top) + 2);                             \
      break;                                                                   \
    case 3:                                                                    \
      MOVE_LIGHT_DOWN(staging, column, (top) + 3);                             \
      break;                                                                   \
    case 4:                                                                    \
      gSolSanctumLunaSolRooms_LandingShake = 2;                                \
      MOVE_LIGHT_DOWN(staging, column, (top) + 4);                             \
      break;                                                                   \
    case cleared:                                                              \
      CLEAR_LIGHT_PATH(staging, column, top);                                  \
      break;                                                                   \
    }                                                                          \
    timer++;                                                                   \
    if (timer > (Random_Below(40)) + (restart)) {                              \
      timer = 0;                                                               \
    }                                                                          \
  }

/// The first light falling through the Luna room, which also shakes the rooms
/// as any of the four lands.
static void SolSanctumLunaSolRooms_UpdateFirstFallingLight(void) {
  STEP_FALLING_LIGHT(gSolSanctumLunaSolRooms_Light1Timer, 46, 30, 33, 80, 90)
  if (gSolSanctumLunaSolRooms_LandingShake != 0) {
    if (gSolSanctumLunaSolRooms_LandingShake == 2) {
      MapGrid_SetShake(1, 1, 1);
    } else if (gSolSanctumLunaSolRooms_LandingShake == 1) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
    gSolSanctumLunaSolRooms_LandingShake--;
  }
}

/// Defines `name`, the task of one more falling light in the Luna room, with
/// its own timer and random restart delay (STEP_FALLING_LIGHT).
#define SOL_SANCTUM_FALLING_LIGHT(name, timer, staging, column, top, cleared,  \
                                  restart)                                     \
  static void name(void) {                                                     \
    STEP_FALLING_LIGHT(timer, staging, column, top, cleared, restart)          \
  }

/// The Luna room's second falling light.
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateSecondFallingLight,
                          gSolSanctumLunaSolRooms_Light2Timer, 47, 42, 33, 90,
                          100)
/// The Luna room's third falling light.
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateThirdFallingLight,
                          gSolSanctumLunaSolRooms_Light3Timer, 48, 31, 36, 95,
                          105)
/// The Luna room's fourth falling light.
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateFourthFallingLight,
                          gSolSanctumLunaSolRooms_Light4Timer, 46, 41, 36, 85,
                          95)

/// Opens on Kraden in the Luna room in SOL_SANCTUM_BLUE_TINT, as a flash
/// crosses it.
static void SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom(void) {
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CopyCellsTo(30, 43, 32, 40, 8, 3);
  MapGrid_CopyCellsTo(30, 43, 33, 39, 8, 1);
  MapGrid_CopyCellsTo(30, 43, 36, 38, 3, 3);
  MapGrid_CopyCellsTo(14, 41, 32, 41, 8, 4);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 158, 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_KRADEN, 574, 120);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  wait(1);
  PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldState->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Sound_Play(SE_THUNDER);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
  PaletteFade_StartInterpolation(8);
  Event_Wait(32);
  PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(24);
}

/// The scene cuts to Kraden by the picture of Luna when Isaac sets the first
/// statue on its seal after the sun-disc statue has dropped into the floor. A
/// quarter of the picture lights up and flashes faster and faster; Kraden,
/// startled, cries "What's happening?", decides Isaac must have found
/// something, and runs off to the statue room.
static void SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed(void) {
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Event_Start();
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  // Kraden turns, startled: "What's happening?"
  Event_SetMessageId(MsgSolSanctumWhatsHappeningAtTheTrap);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 174, 1);
  FieldCamera_WaitForMove();
  Event_Wait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  // Shorten each flash interval as the flashing accelerates.
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Event_Wait(6);
  // Kraden: "Isaac must have found something!" He runs off south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldState->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED);
}

/// The scene cuts to Kraden by the picture of Luna when Isaac first sets a
/// statue on its seal, before the trap is disarmed. The lights flash, then
/// lights start falling through the room of Luna, and Kraden runs off to the
/// statue room crying, "Isaac! Stop!!! You're about to make a big mistake!"
static void SolSanctumLunaSolRooms_SpringTrap(void) {
  s32 flash;
  Event_Start();
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  gSolSanctumLunaSolRooms_Light1Timer = 0;
  gSolSanctumLunaSolRooms_Light2Timer = 0;
  gSolSanctumLunaSolRooms_Light3Timer = 0;
  gSolSanctumLunaSolRooms_Light4Timer = 0;
  // Kraden turns, startled: "What's happening?" The lights flash four times.
  Event_SetMessageId(MsgSolSanctumWhatsHappening);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 174, 1);
  FieldCamera_WaitForMove();
  Event_Wait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  for (flash = 0; flash != 4; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  // Kraden: "Isaac must have found something!"
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  // Each light starts falling after 20 to 79 frames.
  gSolSanctumLunaSolRooms_Light1Timer = (Random_Below(60)) + 20;
  gSolSanctumLunaSolRooms_Light2Timer = (Random_Below(60)) + 20;
  gSolSanctumLunaSolRooms_Light3Timer = (Random_Below(60)) + 20;
  gSolSanctumLunaSolRooms_Light4Timer = (Random_Below(60)) + 20;
  gSolSanctumLunaSolRooms_LandingShake = 0;
  Task_AddFrame(SolSanctumLunaSolRooms_UpdateFirstFallingLight,
                TASK_PASS_FRAME);
  Task_AddFrame(SolSanctumLunaSolRooms_UpdateSecondFallingLight,
                TASK_PASS_FRAME);
  Task_AddFrame(SolSanctumLunaSolRooms_UpdateThirdFallingLight,
                TASK_PASS_FRAME);
  Task_AddFrame(SolSanctumLunaSolRooms_UpdateFourthFallingLight,
                TASK_PASS_FRAME);
  for (flash = 0; flash != 6; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(5);
  }
  for (flash = 0; flash != 8; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (flash = 0; flash != 10; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(3);
  }
  for (flash = 0; flash != 12; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  MapGrid_CopyCellsTo(45, STATUE_LIGHTS_DARK_ROW, 34, 10, 4, 2);
  Actor_Jump(ACTOR_KRADEN, 6, 40);
  // Kraden: "Stop!!! You're about to make a big mistake!" He runs south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  Task_Remove(SolSanctumLunaSolRooms_UpdateFirstFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateSecondFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateThirdFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateFourthFallingLight);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldState->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED);
}

/// Unused: the only way here is a branch of SolSanctumStatue_OpenSeal that
/// cannot run (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN).
static void SolSanctumLunaSolRooms_UnusedAnimateSavedStatueLights(void) {
  s32 speaker;
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Event_Start();
  // Toggle the shown lights only where the quarters are lit now.
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(45, STATUE_LIGHTS_LIT_ROW, 34, 10, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(47, STATUE_LIGHTS_LIT_ROW, 36, 10, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(45, STATUE_LIGHTS_LIT_ROW + 1, 34, 11, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(47, STATUE_LIGHTS_LIT_ROW + 1, 36, 11, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 174, 1);
  FieldCamera_WaitForMove();
  Event_Wait(30);
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(2);
  }
  Sound_Play(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Event_Wait(6);
  // Kraden, once: "You found it, Isaac." and "Keep doing that until the light
  // bathes all the remaining quarters."
  if (GameFlag_Get(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT) == 0) {
    speaker = SPEAKER_WINDOW_BELOW | ACTOR_KRADEN;
    Event_SetMessageId(MsgSolSanctumYouFoundIt);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
  }
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldState->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN);
}

/// Isaac comes back through the room of Luna's south doorway (cells (35, 16)
/// and (36, 16)) once all four quarters are lit, and Kraden, by the picture,
/// calls to him: "Look, Isaac! The picture of Luna changed to Sol!" But
/// nothing else looks different, and he decides something else must have
/// changed somewhere.
static void SolSanctumLunaSolRooms_KradenSeesLunaPictureChange(void) {
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) == 0) {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
      Event_Start();
      Actor_SetPosition(ACTOR_KRADEN, 577, 147);
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 1);
      FieldCamera_MoveTo(574, CAMERA_KEEP, 184, 1);
      // Isaac walks up to Kraden, who hops: "The picture of Luna changed to
      // Sol!"
      Event_SetMessageId(MsgSolSanctumThePictureOfLunaChanged);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 576, 232);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
      FieldCamera_WaitForMove();
      Event_Wait(10);
      Actor_SetSpeed(ACTOR_KRADEN, 1);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 152);
      Event_Wait(6);
      Actor_Jump(ACTOR_KRADEN, 6, 30);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Event_Wait(2);
      Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
      // Kraden: "But other than that, nothing looks different..."
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
      Event_Wait(30);
      // Kraden: a trap here, and this is all that happens?
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
      Event_Wait(6);
      Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
      Event_Wait(40);
      // Kraden: "Somewhere, something else must have changed." He walks back
      // into the leader.
      SolSanctumLunaSolRooms_ShowMessageAndWait(
          SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 208);
      Event_Wait(40);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Event_Wait(6);
      Actor_SetSpeed(ACTOR_KRADEN, 0.5);
      Scene_WalkIntoLeader(ACTOR_KRADEN);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
      Event_End();
    }
  }
}

/// Kraden comes out beside Isaac in the room of Sol once all four quarters are
/// lit: "Look! The symbol on the floor! It changed from Sol to Luna!" Surely
/// there must be something in this room.
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void) {
  Sound_Play(MUS_FOREST_THEME);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 184);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Actor_SetPosition(ACTOR_KRADEN, 376, 184);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 392, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_SetMessageId(MsgSolSanctumLookSymbolFloor);
  Actor_Jump(ACTOR_KRADEN, 4, 30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 376, 184);
  // Off the map, until he next comes out of the party.
  Actor_SetPosition(ACTOR_KRADEN, 1608, 1608);
  Event_Wait(4);
  GameFlag_Set(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
}

/// Isaac searches the north wall of the room of Sol, behind the symbol (cells
/// (17, 6) and (18, 6)), once Kraden has seen the symbol change. Kraden comes
/// out beside him, a portal opens with a shimmer ("Ah ha! This has to be it."),
/// and Isaac rises slowly into the air, blinking, and vanishes upward; Kraden
/// follows, "Wait for me, Isaac... I'm on my way!", and they arrive in the
/// Elemental Star Room.
///
/// Before Kraden has seen the symbol change, nothing happens.
static void SolSanctumLunaSolRooms_RiseToElementalStarRoom(void) {
  struct Actor *actor;
  s32 frame;
  struct Sprite *sprite;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) != 0) {
    Event_Start();
    // Isaac steps onto the symbol and Kraden comes out beside him.
    FieldCamera_SetSpeed(1);
    FieldCamera_MoveTo(287, CAMERA_KEEP, 148, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 30);
    Actor_SetPosition(ACTOR_KRADEN, 288, 120);
    Actor_SetSpeed(ACTOR_KRADEN, 1);
    Actor_WalkToAndWait(ACTOR_KRADEN, 276, 136);
    Actor_WalkTo(ACTOR_KRADEN, 264, 136);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 136);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
    if (Event_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
      Sound_Play(SE_SHIMMER);
    }
    Event_Wait(40);
    // Reveal the passage only once; later visits reuse its open state.
    if (Event_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
      MapGrid_CopyCellsTo(36, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(44, 59, 17, 38, 2, 1);
      Event_Wait(10);
      MapGrid_CopyCellsTo(38, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(44, 59, 17, 39, 2, 1);
      Event_Wait(10);
      MapGrid_CopyCellsTo(40, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(0, 32, 17, 39, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 40, 2, 1);
      Event_Wait(10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      0);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      MapGrid_CopyCellsTo(42, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(0, 32, 17, 40, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 41, 2, 1);
      Event_Wait(10);
      MapGrid_CopyCellsTo(0, 32, 17, 41, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 42, 2, 1);
      Event_Wait(10);
      MapGrid_CopyCellsTo(0, 32, 17, 42, 2, 3);
      Event_Wait(80);
      TilesetAnim_Resume(WAY_UP_ANIMATION_A);
      TilesetAnim_Resume(WAY_UP_ANIMATION_B);
      GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
    }
    Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 30);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    // Kraden: "Ah ha! This has to be it." and "Let's go, Isaac!"
    Event_SetMessageId(MsgSolSanctumHa);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(60);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(287, CAMERA_KEEP, 114, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.3);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    // Default: NULL until it is pointed at the leader's sprite below.
    sprite = NULL;
    actor->motionFlags = 0;
    Sound_Play(SE_ASCEND);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
    sprite = actor->sprite;
    frame = 0;
    sprite->flags = 0;
    do {
      actor->y.fixed += toFIXED(1) / 5;
      wait(1);
      frame++;
    } while (frame != 120);
    Sound_Play(SE_WARP);
    for (frame = 0; frame != 60; frame++) {
      actor->y.fixed += toFIXED(1) / 10;
      sprite->scale -= FIXED_ONE / 64;
      wait(1);
    }
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_Jump(ACTOR_KRADEN, 4, 20);
    // Kraden: "Wait for me, Isaac... I'm on my way!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 120);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_KRADEN, 0.3);
    actor = Actor_Get(ACTOR_KRADEN);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    actor->motionFlags = 0;
    Sound_Play(SE_ASCEND);
    Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
    sprite = actor->sprite;
    sprite->flags = 0;
    frame = 0;
    do {
      actor->y.fixed += toFIXED(1) / 5;
      wait(1);
      frame++;
    } while (frame != 120);
    Sound_Play(SE_WARP);
    for (frame = 0; frame != 60; frame++) {
      actor->y.fixed += toFIXED(1) / 10;
      sprite->scale -= FIXED_ONE / 64;
      wait(1);
    }
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Event_Wait(80);
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    gFieldState->transitionFrames = 24;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    wait(1);
    Import_EventSetExitRequest(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM);
    Event_End();
  }
}

/// Sets up Sol Sanctum's rooms of Sol and Luna and plays the event this
/// entrance calls for.
s32 Scene_Initialize(void) {
  s32 playedEntranceEvent;
  s32 anyLightLit;

  wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  playedEntranceEvent = 0;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_FOUND) != 0 &&
      GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) == 0 &&
      GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED) != 0) {
      PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(1);
      wait(1);
    } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) != 0) {
      PaletteFade_ApplyTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(1);
      wait(1);
    }
  } else {
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) != 0) {
      Actor_SetPosition(ACTOR_KRADEN, 576, 120);
    }
  }
  if (gMainRoutineState.entrance ==
      SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
      SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
      playedEntranceEvent = 1;
    }
  } else if (
      gMainRoutineState.entrance ==
      SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
      SolSanctumLunaSolRooms_SpringTrap();
      GameFlag_Set(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN);
      GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
      GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
      GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
      GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
      playedEntranceEvent = 1;
    }
  } else if (gMainRoutineState.entrance ==
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN) {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0) {
      SolSanctumLunaSolRooms_UnusedAnimateSavedStatueLights();
      GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT);
    }
    playedEntranceEvent = 1;
  }
  if (Event_GetFlag(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (Event_GetFlag(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  wait(4);
  if (playedEntranceEvent == 0) {
    // With all four seals open, the sun symbol glowing on the floor is
    // redrawn as a crescent moon under a falling beam of light, and the
    // room's glow is gone; the same is copied for the other room's symbol.
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
      MapGrid_CopyCellsTo(30, 44, 30, 38, 12, 5);
      MapGrid_CopyCellsTo(30, 44, 34, 37, 4, 1);
      MapGrid_CopyCellsTo(14, 41, 32, 41, 8, 4);
      MapGrid_CopyCellsTo(45, STATUE_LIGHTS_LIT_ROW, 34, 10, 4, 2);
      MapGrid_CopyCellsTo(45, STATUE_LIGHTS_DARK_ROW, 16, 10, 4, 2);
      MapGrid_CopyCellsTo(14, 45, 14, 41, 8, 4);
      if (gMainRoutineState.entrance !=
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
        if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) == 0) {
          // The cells SolSanctumLunaSolRooms_RiseToElementalStarRoom copies as
          // it sets the flag, redrawn on arrival.
          if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
            MapGrid_CopyCellsTo(0, 32, 17, 39, 2, 1);
            MapGrid_CopyCellsTo(42, 62, 17, 36, 2, 3);
            MapGrid_CopyCellsTo(0, 32, 17, 40, 2, 1);
            MapGrid_CopyCellsTo(0, 32, 17, 41, 2, 1);
            MapGrid_CopyCellsTo(0, 32, 17, 42, 2, 3);
          } else {
            MapGrid_CopyCellsTo(44, 59, 17, 37, 2, 6);
          }
          TilesetAnim_Resume(WAY_UP_ANIMATION_A);
          TilesetAnim_Resume(WAY_UP_ANIMATION_B);
        }
      }
      Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    } else {
      anyLightLit = 0;
      // Short of all four, each open seal redraws its quarter of the symbol
      // on each room's floor: two cells of the four-by-two picture.
      if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
        MapGrid_CopyCellsTo(45, 28, 34, 10, 2, 1);
        MapGrid_CopyCellsTo(45, 30, 16, 10, 2, 1);
        anyLightLit = 1;
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
        MapGrid_CopyCellsTo(47, 28, 36, 10, 2, 1);
        MapGrid_CopyCellsTo(47, 30, 18, 10, 2, 1);
        anyLightLit = 1;
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
        MapGrid_CopyCellsTo(45, 29, 34, 11, 2, 1);
        MapGrid_CopyCellsTo(45, 31, 16, 11, 2, 1);
        anyLightLit = 1;
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
        MapGrid_CopyCellsTo(47, 29, 36, 11, 2, 1);
        MapGrid_CopyCellsTo(47, 31, 18, 11, 2, 1);
        anyLightLit = 1;
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0 ||
          anyLightLit != 0) {
        MapGrid_CopyCellsTo(30, 43, 32, 40, 8, 3);
        MapGrid_CopyCellsTo(30, 43, 33, 39, 8, 1);
        MapGrid_CopyCellsTo(30, 43, 36, 38, 3, 3);
        MapGrid_CopyCellsTo(36, 58, 32, 41, 8, 4);
      }
      // The wall behind the symbol takes the plain wall's attributes beside
      // it, so searching there does nothing.
      MapGrid_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  // The arrival after the Wise One's rescue plays only once.
  if (Event_GetFlag(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN) == 0 &&
      gMainRoutineState.entrance ==
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
    SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue();
    GameFlag_Set(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN);
    MapGrid_CopyCellAttributes(15, 6, 2, 1, 17, 6);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
      Scene_QueueMusic(SE_RUMBLE);
      MapGrid_SetShake(1, 1, 1);
      FieldEffect_InitializeSparkles();
      MapGrid_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  return 0;
}

/// Returns 1 when all four quarters are lit.
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void) {
  s32 allLit = 1;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) == 0)
    allLit = 0;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) == 0)
    allLit = 0;

  return allLit;
}

/// Shows the next message of the event, then waits.
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(waitFrames);
}

/// Sol Sanctum after the eruption: Isaac and Garet appear here in a white
/// flash.
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void) {
  struct PaletteFade *palette;

  Event_Start();
  // The screen opens all white on Isaac and Garet, then fades to the tint.
  FieldCamera_MoveTo(286, CAMERA_KEEP, 134, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_ApplySource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Actor_SetPosition(ACTOR_GARET, 280, 134);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  gFieldState->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(120);
  wait(120);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(60);
  wait(100);
  // Both look around, bewildered, as the rooms give a jolt.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 60);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // Garet: "Did that floating eye thing save us?" A stronger tremor throws
  // them both aside.
  Event_SetMessageId(MsgSolSanctumFloatingEyeThing);
  // "Did that floating eye thing save us?"
  Event_OpenMessage(ACTOR_GARET, 0);
  Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Sound_Play(SE_TREMOR);
  MapGrid_SetShake(4, 4, 1);
  Event_Wait(40);
  FieldEffect_InitializeSparkles();
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 300, 130);
  Actor_SetDestination(ACTOR_GARET, 266, 144);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(40);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  // Recover from the tremor and bring Garet back into the party.
  Scene_QueueMusic(SE_RUMBLE);
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // Garet: "Yeah, right! Let's get out of here..."
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Scene_WalkIntoLeader(ACTOR_GARET);
  palette = gPaletteFadeState;
  palette->target[0] = 0;
  palette->target[1] = 0;
  palette->target[2] = 0;
  palette->page = 0;
  palette->duration = 1;
  palette->step = 1;
  // DEAD: nothing reads the fade's last byte; the ROM keeps its store.
  palette->padding = 1;
  Io_Write16(0, &PLTT_BACKDROP);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  Event_End();
}

/// Isaac talks to Kraden by the picture of Luna: "Let me know what you find" if
/// Isaac agreed to look farther up the passage, or "Just wait over there."
static void SolSanctumLunaSolRooms_TalkToKraden(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN) != 0) {
    Event_SetMessageId(MsgSolSanctumKradenLetMeKnowWhat);
  } else {
    Event_SetMessageId(MsgSolSanctumKradenJustWaitOverThere);
  }
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Event_End();
}

struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 288, 0, 157, FACING_SOUTH, 0, 0, 0,
     752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 55,
     0, 477, FACING_NORTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE, CONDITION_ALWAYS, 647,
     0, 327, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED, CONDITION_ALWAYS, 287, 0, 218,
     FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION, CONDITION_ALWAYS,
     297, 0, 119, FACING_NORTHWEST, 0, 0, 0, 752, 496, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumLunaSolRoomsExits[] = {
    SCENE_EXITS(SolSanctumLunaSolRooms),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS,
               SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    SCENE_EXIT(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED,
        SolSanctumStatue,
        SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED),
    SCENE_EXIT(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED,
        SolSanctumStatue,
        SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_UNUSED,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM,
               SolSanctumElementalStarRoom,
               SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    EXIT_TO_BATTLE(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_BATTLE, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gSolSanctumLunaSolRoomsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gSolSanctumLunaSolRoomsTriggers[] = {
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(SolSanctumLunaSolRooms_TalkToKraden)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT_INVERSE,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_TintLunaRoom)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_TintSolRoom)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_PLAIN,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_RestorePlainTint)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINT,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_TintPassageBetweenRooms)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BLACK_BACKDROP,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_ClearBackdropColor)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT),
     CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_FLOOR_SYMBOL, CONDITION_ALWAYS,
     EVENT(SolSanctumLunaSolRooms_RiseToElementalStarRoom)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_LUNA_DOORWAY,
     CONDITION_ALWAYS,
     EVENT(SolSanctumLunaSolRooms_KradenSeesLunaPictureChange)},
    {TRIGGER_EXIT, SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS,
     CONDITION_ALWAYS, SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE, CONDITION_ALWAYS,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_HEART_OF_SANCTUM,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WAY_OUT,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_KradenStopsPartyLeaving)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST),
     CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST,
     FLAG_SOL_SANCTUM_HERB_FOUND, FIND_ITEM(ITEM_HERB)},
    {TRIGGER_TABLE_END},
};
