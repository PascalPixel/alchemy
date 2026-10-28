/// @file
/// Sol Sanctum (SceneId_SolSanctumLunaSolRooms), the room of Sol and the room
/// of Luna at the heart of the sanctum, between SceneId_SolSanctumReliefs
/// (sol_sanctum_entrance.c), the statue room (SceneId_SolSanctumStatue,
/// sol_sanctum_statue.c) and the way up to SceneId_SolSanctumElementalStarRoom.
///
/// Kraden's events here follow the story flags in order:
/// - SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum, on the first step onto
/// the symbol of
///   the sun on the floor (FLAG_SOL_SANCTUM_FOUND). From then until the way up
///   opens, Retreat is blocked and
///   SolSanctumLunaSolRooms_KradenStopsPartyLeaving turns the party back from
///   the way out.
/// - SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms, the first time the
///   party crosses into the Luna room (FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN):
///   Kraden finds the picture of Luna and stays by it from then on.
/// - Each statue pushed onto a seal in the statue room lights a quarter of
///   the pictures (FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN to
///   _SOUTHEAST_SEAL_OPEN), and the first one cuts here to Kraden: before the
///   trap is disarmed lights fall through the Luna room
///   (SolSanctumLunaSolRooms_SpringTrap, FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN)
///   and every quarter goes dark again; afterwards the quarter only flashes and
///   Kraden runs off to the statue room
///   (SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed,
///   FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING). Both take the party back
///   to the statue room by the exit named for the event.
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
/// The rooms' colours follow the party: cells on the floor between them fade
/// the palette to a tint or back to plain colours (enum
/// SolSanctumLunaSolRoomsCellBehavior), and one of three scene flags
/// remembers which colours show, so a return after a battle keeps them. On
/// entry SolSanctumLunaSolRooms_Initialize copies each lit quarter into its
/// FLAG_SOL_SANCTUM_*_LIGHT_SHOWN and redraws the pictures, the floor and the
/// open way up from the story flags, unless the entrance's own event
/// draws them. The falling lights are four tasks, each with a random timer
/// of its own, that the trap adds and removes. A third statue-room event, the
/// lit quarters flashing for Kraden
/// (SolSanctumLunaSolRooms_UnusedAnimateSavedStatueLights), is never reached.

#include "adventure.h"
#include "unit/scene/sol_sanctum_luna_sol_rooms.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "sol_sanctum_luna_sol_rooms.h"
#include "call.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "types.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "iowrite.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "heap.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

/// The rooms of Sol and Luna show their plain colours (PALETTE_TINT_NONE), so a
/// return after a battle keeps them.
/// ★ Named from the game: "But a room for Luna and one for Sol?"
/// (MsgSolSanctumRoomLunaOne)
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED FLAG_SCENE_TEMP_00
/// The rooms of Sol and Luna show SOL_SANCTUM_BLUE_TINT, so a return after a
/// battle keeps it.
/// ★ Named from the game: "But a room for Luna and one for Sol?"
/// (MsgSolSanctumRoomLunaOne)
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED FLAG_SCENE_TEMP_01
/// The rooms of Sol and Luna show SOL_SANCTUM_WARM_TINT, so a return after a
/// battle keeps it.
/// ★ Named from the game: "But a room for Luna and one for Sol?"
/// (MsgSolSanctumRoomLunaOne)
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED FLAG_SCENE_TEMP_02

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// Set once Isaac and Garet's arrival after the Wise One's rescue has played.
/// ★ Named from the game: "Did that floating eye thing save us?"
/// (MsgSolSanctumFloatingEyeThing)
#define FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN 0x309

/// The blue tint the rooms fade to (PaletteTint's TINT form).
#define SOL_SANCTUM_BLUE_TINT (PALETTE_TINT_CUSTOM | RGB(12, 14, 20))
/// The dim red tint the rooms fade to.
#define SOL_SANCTUM_WARM_TINT (PALETTE_TINT_CUSTOM | RGB(17, 13, 11))

/// The cell behaviors of the floor cells the events answer; behaviors 1
/// and 2 are the exits.
enum SolSanctumLunaSolRoomsCellBehavior {
  /// Before Kraden in the Luna room.
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BEFORE_KRADEN = 8,
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
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST = 100
};

// Scene flags, cleared at every ordinary entrance; one of the three is
// set at a time, for the colours the rooms show, so a return after a battle
// keeps them:
// - FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED: the plain colours
//   (PALETTE_TINT_NONE);
// - FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED: SOL_SANCTUM_BLUE_TINT;
// - FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED: SOL_SANCTUM_WARM_TINT.

static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void);
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void);
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void);
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames);
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void);

void FieldEffect_InitializeSparkles(void);

const struct SceneEntrance *SolSanctumLunaSolRooms_GetEntrances(void) {
  return gSolSanctumLunaSolRoomsEntrances;
}

const struct SceneExitRegion *SolSanctumLunaSolRooms_GetRegions(void) {
  return 0;
}

const u32 *SolSanctumLunaSolRooms_GetExits(void) {
  return gSolSanctumLunaSolRoomsExits;
}

/// Returns the eight statues, four in each room, Kraden, Jenna and Garet, all
/// always placed.
const struct ActorPlacement *SolSanctumLunaSolRooms_GetActorPlacements(void) {
  return gSolSanctumLunaSolRoomsActorPlacements;
}

/// Returns the floor's cell behaviors that tint the rooms, Kraden's talk
/// and events, the floor symbol and the Herb's chest.
const struct Trigger *SolSanctumLunaSolRooms_GetTriggers(void) {
  return gSolSanctumLunaSolRoomsTriggers;
}

/// Sol Sanctum: crossing CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT. Until
/// all four quarters are lit the rooms fade back to plain colours; afterwards
/// to SOL_SANCTUM_BLUE_TINT, and Kraden sees the floor symbol changed the first
/// time (and the Luna room, had he not yet). Also called by
/// SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum after the eruption. Sets
/// one of FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED and
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED, clearing the other tint flags;
/// Kraden's events set FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN and
/// FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN.
static void SolSanctumLunaSolRooms_TintSolRoom(void) {
  s32 paletteActive;

  // Test all four quarters' flags without skipping any flag reads.
  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
    if (paletteActive == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
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
    if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED) == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      Event_End();
    }
  }
}

/// Sol Sanctum: crossing CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_PLAIN fades
/// the rooms back to plain colours. Also called by
/// SolSanctumLunaSolRooms_KradenStopsPartyLeaving after the eruption. Sets
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED and clears the other two tint
/// flags.
static void SolSanctumLunaSolRooms_RestorePlainTint(void) {
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED) == 0) {
    Event_Start();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    PaletteFade_StartInterpolation(20);
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
    Event_End();
  }
}

/// Sol Sanctum: crossing CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT_INVERSE,
/// as CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT the other way round. Until
/// all four quarters are lit the rooms fade to SOL_SANCTUM_BLUE_TINT, and the
/// first time Kraden sees the Luna room; afterwards back to plain colours. Sets
/// one of FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED and
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED, clearing the other tint flags.
static void SolSanctumLunaSolRooms_TintLunaRoom(void) {
  s32 paletteActive;

  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
    if (paletteActive == 0) {
      Event_Start();
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
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
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
      GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      Event_End();
    }
  }
}

/// Sol Sanctum: crossing CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINT
/// fades the rooms to SOL_SANCTUM_WARM_TINT. Sets
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED and clears the other two tint
/// flags.
static void SolSanctumLunaSolRooms_TintPassageBetweenRooms(void) {
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) == 0) {
    PaletteFade_ApplyTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
    PaletteFade_StartInterpolation(20);
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
    GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  }
}

/// Sol Sanctum: crossing
/// CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BLACK_BACKDROP turns the backdrop
/// black.
static void SolSanctumLunaSolRooms_ClearBackdropColor(void) {
  s32 black = 0;
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
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 28);
}

/// Draws the lights the room last showed (the _LIGHT_SHOWN flags).
static void SolSanctumLunaSolRooms_DrawSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 28);
}

/// Draws the current lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 30);
}

/// Draws the saved lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 30);
}

/// Flashes both tile rows of the lit quarters' lights at this pace.
static inline void SolSanctumLunaSolRooms_FlashCurrentStatueLights(s32 frames) {
  Event_PlaySong(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Event_WaitFrames(frames);
  Event_PlaySong(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights();
  Event_WaitFrames(frames);
}

/// Flashes both tile rows of the lights last shown at this pace.
static inline void SolSanctumLunaSolRooms_FlashSavedStatueLights(s32 frames) {
  Event_PlaySong(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Event_WaitFrames(frames);
  Event_PlaySong(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights();
  Event_WaitFrames(frames);
}

/// Sol Sanctum: stepping on the symbol of the sun the first time. Kraden runs
/// to the symbol on the floor, the mark of the sun: "This is it! We've found
/// it!" The strangers told the truth and must be thieves, but Kraden talks the
/// others into going a little farther. Plays until FLAG_SOL_SANCTUM_FOUND is
/// set, which it sets with FLAG_RETREAT_BLOCKED; after the eruption the
/// crossing tints the rooms as CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT
/// does.
static void SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    SolSanctumLunaSolRooms_TintSolRoom();
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_FOUND) == 0) {
    Event_Start();
    // The leader walks in as the music changes.
    Event_SetMessageId(MsgSolSanctumFound);
    Sound_PlaySong(SONG_FADE_OUT_BGM);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Event_WaitFrames(20);
    Sound_PlaySong(MUS_FOREST);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    // Kraden hurries out of the leader to the symbol on the floor.
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Import_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(7) / 5, PIXELS(7) / 10);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 206);
    Event_WaitFrames(40);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_Jump(ACTOR_KRADEN, 4, 60);
    // Kraden: "This is it! We've found it!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    // Garet and Jenna step out behind the leader.
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Import_ActorSetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Import_ActorSetPosition(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_SetSpeed(ACTOR_JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkTo(ACTOR_GARET, 280, 248);
    Actor_WalkToAndWait(ACTOR_JENNA, 296, 248);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST + FACING_STEP, 30);
    FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveWithinMapBounds(PIXELS(288), -1, PIXELS(213), 1);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_SetPositionAndReset(ACTOR_KRADEN, 288, 176);
    Event_WaitFrames(40);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    // Kraden: the mark of the sun, the heart of Sol Sanctum; so those two told
    // the truth.
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 60);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_ArmCallback(ACTOR_KRADEN, FACING_EAST, 40);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST + FACING_STEP, 10);
    // Jenna: then they must have been... Garet: "They were thieves!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_JENNA, 10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST + FACING_STEP, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 6);
    Actor_SetAttachedEffect(ACTOR_JENNA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
    // Jenna: "Let's hurry back to the village and tell everybody!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 20);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // Garet: "Kraden? What's the matter?" Kraden: "We're so close..." The
    // three wonder at turning back from the heart of the sanctum.
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
    Event_WaitFrames(60);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_JENNA, 60);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST + FACING_STEP, 60);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
    Event_WaitFrames(6);
    // Kraden: "Shall we go a little farther?" Yes: "Thank you!" No: then go
    // back without him.
    FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
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
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 6);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 20);
    Actor_Jump(ACTOR_KRADEN, 6, 20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 30);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    // End the discussion and return the followers to Isaac.
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_KRADEN, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Import_ActorSetPosition(ACTOR_GARET, 0, 0);
    Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_ResetAndSetPosition(ACTOR_JENNA, leader->x.part.pixel,
                                leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_JENNA);
    Import_ActorSetPosition(ACTOR_JENNA, 0, 0);
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    GameFlag_Set(FLAG_SOL_SANCTUM_FOUND);
    Event_End();
  }
}

/// Sol Sanctum, the first time the party crosses into the Luna room: Kraden
/// steps out beside Isaac, finds the picture of Luna, wonders how the rooms of
/// Luna and Sol are connected, and asks Isaac to look farther up the passage.
/// Called by SolSanctumLunaSolRooms_TintSolRoom and
/// SolSanctumLunaSolRooms_TintLunaRoom until
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN is set, which it sets; Isaac agreeing
/// sets FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN. Leaves Kraden standing by
/// the picture on the northern wall.
static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void) {
  struct Actor *leader;
  s32 request;

  // Kraden steps out beside Isaac: "What is this room?"
  Event_SetMessageId(MsgSolSanctumWhRoom);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 488, 176);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 472, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(575), -1, PIXELS(181), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(120);
  SolSanctumLunaSolRooms_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 80);
  FieldCamera_MoveTo(PIXELS(492), -1, PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  // Kraden: "These rooms must be connected somehow, don't you think?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
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
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId((request + 1));
    GameFlag_Set(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN);
  } else {
    Event_SetMessageId((request + 2));
  }
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            4);
  FieldCamera_FollowActor(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 486, 131);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 120);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 2);
  // Leave Kraden by the northern wall for the search interactions.
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
}

/// Sol Sanctum: heading back out once Kraden has found the heart
/// of the sanctum and until the way up is open: he hurries over. "That way
/// leads out of the sanctum." After the eruption the rooms only fade back to
/// plain colours (SolSanctumLunaSolRooms_RestorePlainTint).
static void SolSanctumLunaSolRooms_KradenStopsPartyLeaving(void) {
  struct Actor *kraden;
  struct Actor *leader;
  struct Actor *destinationActor;

  kraden = Actor_GetById(ACTOR_KRADEN);
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
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Event_SetMessageId(MsgSolSanctumWayLeadsOutSanctum);
  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Event_WaitFrames(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
             kraden->x.fixed > PIXELS(340)) {
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(392), PIXELS(168));
    Event_WaitFrames(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      kraden->x.fixed > PIXELS(340)) {
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  } else {
    // The result is unused.
    GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
    destinationActor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (destinationActor != 0) {
      Actor_SetDestination(ACTOR_KRADEN, destinationActor->x.part.pixel,
                           destinationActor->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 248);
  }
  Event_End();
}

/// Animates the first light falling through the Luna room and manages the
/// shared shake pulse. Run each frame as a task that
/// SolSanctumLunaSolRooms_SpringTrap adds and removes.
static void SolSanctumLunaSolRooms_UpdateFirstFallingLight(void) {
  if ((Random_Next() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light1Timer) {
    case 0:
      Event_PlaySong(SE_FALLING_LIGHT);
      MapGrid_CopyCellsTo(46, 59, 30, 33, 1, 5);
      break;
    case 1:
      MapGrid_CopyCellsTo(46, 59, 30, 33, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 30, 34, 1, 5);
      break;
    case 2:
      MapGrid_CopyCellsTo(46, 59, 30, 34, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 30, 35, 1, 5);
      break;
    case 3:
      MapGrid_CopyCellsTo(46, 59, 30, 35, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 30, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      MapGrid_CopyCellsTo(46, 59, 30, 36, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 30, 37, 1, 5);
      break;
    case 0x50:
      MapGrid_CopyCellsTo(46, 49, 30, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light1Timer++;
    if (gSolSanctumLunaSolRooms_Light1Timer >
        ((u32)(Random_Next() * 40) >> 16) + 90) {
      gSolSanctumLunaSolRooms_Light1Timer = 0;
    }
  }
  if (gSolSanctumLunaSolRooms_FlashState != 0) {
    if (gSolSanctumLunaSolRooms_FlashState == 2) {
      MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    } else if (gSolSanctumLunaSolRooms_FlashState == 1) {
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    gSolSanctumLunaSolRooms_FlashState--;
  }
}

/// Animates the second falling light with its own random restart delay, as
/// the first.
static void SolSanctumLunaSolRooms_UpdateSecondFallingLight(void) {
  if ((Import_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light2Timer) {
    case 0:
      Sound_PlaySong(SE_FALLING_LIGHT);
      MapGrid_CopyCellsTo(47, 59, 42, 33, 1, 5);
      break;
    case 1:
      MapGrid_CopyCellsTo(47, 59, 42, 33, 1, 1);
      MapGrid_CopyCellsTo(47, 59, 42, 34, 1, 5);
      break;
    case 2:
      MapGrid_CopyCellsTo(47, 59, 42, 34, 1, 1);
      MapGrid_CopyCellsTo(47, 59, 42, 35, 1, 5);
      break;
    case 3:
      MapGrid_CopyCellsTo(47, 59, 42, 35, 1, 1);
      MapGrid_CopyCellsTo(47, 59, 42, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      MapGrid_CopyCellsTo(47, 59, 42, 36, 1, 1);
      MapGrid_CopyCellsTo(47, 59, 42, 37, 1, 5);
      break;
    case 0x5a:
      MapGrid_CopyCellsTo(47, 49, 42, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light2Timer++;
    if (gSolSanctumLunaSolRooms_Light2Timer >
        ((u32)(Import_RandomNext() * 40) >> 16) + 100) {
      gSolSanctumLunaSolRooms_Light2Timer = 0;
    }
  }
}

/// Animates the third falling light with its own random restart delay, as the
/// first.
static void SolSanctumLunaSolRooms_UpdateThirdFallingLight(void) {
  if ((Import_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light3Timer) {
    case 0:
      Sound_PlaySong(SE_FALLING_LIGHT);
      MapGrid_CopyCellsTo(48, 59, 31, 36, 1, 5);
      break;
    case 1:
      MapGrid_CopyCellsTo(48, 59, 31, 36, 1, 1);
      MapGrid_CopyCellsTo(48, 59, 31, 37, 1, 5);
      break;
    case 2:
      MapGrid_CopyCellsTo(48, 59, 31, 37, 1, 1);
      MapGrid_CopyCellsTo(48, 59, 31, 38, 1, 5);
      break;
    case 3:
      MapGrid_CopyCellsTo(48, 59, 31, 38, 1, 1);
      MapGrid_CopyCellsTo(48, 59, 31, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      MapGrid_CopyCellsTo(48, 59, 31, 39, 1, 1);
      MapGrid_CopyCellsTo(48, 59, 31, 40, 1, 5);
      break;
    case 0x5f:
      MapGrid_CopyCellsTo(48, 49, 31, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light3Timer++;
    if (gSolSanctumLunaSolRooms_Light3Timer >
        ((u32)(Import_RandomNext() * 40) >> 16) + 105) {
      gSolSanctumLunaSolRooms_Light3Timer = 0;
    }
  }
}

/// Animates the fourth falling light with its own random restart delay, as
/// the first.
static void SolSanctumLunaSolRooms_UpdateFourthFallingLight(void) {
  if ((Import_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light4Timer) {
    case 0:
      Sound_PlaySong(SE_FALLING_LIGHT);
      MapGrid_CopyCellsTo(46, 59, 41, 36, 1, 5);
      break;
    case 1:
      MapGrid_CopyCellsTo(46, 59, 41, 36, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 41, 37, 1, 5);
      break;
    case 2:
      MapGrid_CopyCellsTo(46, 59, 41, 37, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 41, 38, 1, 5);
      break;
    case 3:
      MapGrid_CopyCellsTo(46, 59, 41, 38, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 41, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      MapGrid_CopyCellsTo(46, 59, 41, 39, 1, 1);
      MapGrid_CopyCellsTo(46, 59, 41, 40, 1, 5);
      break;
    case 0x55:
      MapGrid_CopyCellsTo(46, 49, 41, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light4Timer++;
    if (gSolSanctumLunaSolRooms_Light4Timer >
        ((u32)(Import_RandomNext() * 40) >> 16) + 95) {
      gSolSanctumLunaSolRooms_Light4Timer = 0;
    }
  }
}

/// Opens on Kraden in the Luna room in SOL_SANCTUM_BLUE_TINT, as a flash
/// crosses it. Shared by the three events that play on arriving from
/// SceneId_SolSanctumStatue's statue puzzle. Sets
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED and clears the other two tint
/// flags.
static void SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom(void) {
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_CopyCellsTo(30, 43, 32, 40, 8, 3);
  MapGrid_CopyCellsTo(30, 43, 33, 39, 8, 1);
  MapGrid_CopyCellsTo(30, 43, 36, 38, 3, 3);
  MapGrid_CopyCellsTo(14, 41, 32, 41, 8, 4);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(158), 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(574), PIXELS(120));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  WaitFrames(1);
  PaletteFade_ApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  Event_PlaySong(SE_THUNDER);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
  PaletteFade_StartInterpolation(8);
  Event_WaitFrames(32);
  PaletteFade_SetTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(24);
}

/// Sol Sanctum, the first statue placed in SceneId_SolSanctumStatue after the
/// centre statue has dropped into the floor, disarming the trap: the quarter's
/// light flashes ever faster around Kraden in the Luna room. "What's
/// happening?" "Isaac must have found something!" He runs off to see. Called by
/// SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED until
/// FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING is set, which it sets; the flag
/// marks this event, not the trap. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED back to
/// SceneId_SolSanctumStatue.
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
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
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
  Event_WaitFrames(6);
  // Kraden: "Isaac must have found something!" He runs off south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED);
}

/// Sol Sanctum, the first statue placed in SceneId_SolSanctumStatue before the
/// trap is disarmed: the quarter's light flashes and lights fall through the
/// Luna room while Kraden cries "Stop!!! You're about to make a big mistake!"
/// Called by SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED until
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set; the caller then sets it and
/// darkens all four quarters. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED back to
/// SceneId_SolSanctumStatue, where Kraden explains the trap.
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
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  for (flash = 0; flash != 4; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  // Kraden: "Isaac must have found something!"
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  // Each light starts falling after 20 to 79 frames.
  gSolSanctumLunaSolRooms_Light1Timer =
      (((u32)Import_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light2Timer =
      (((u32)Import_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light3Timer =
      (((u32)Import_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light4Timer =
      (((u32)Import_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_FlashState = 0;
  // Run the falling lights independently while the quarters keep flashing.
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
  MapGrid_CopyCellsTo(45, 30, 34, 10, 4, 2);
  Actor_Jump(ACTOR_KRADEN, 6, 40);
  // Kraden: "Stop!!! You're about to make a big mistake!" He runs south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  // Stop all light tasks before leaving this room.
  Task_Remove(SolSanctumLunaSolRooms_UpdateFirstFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateSecondFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateThirdFallingLight);
  Task_Remove(SolSanctumLunaSolRooms_UpdateFourthFallingLight);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED);
}

/// Unused: the only way here is a branch of SolSanctumStatue_OpenSeal that
/// cannot run (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN).
/// The player would have seen the quarters lit since the room last showed them
/// flash ever faster in the Luna room, and Kraden, once: "You found it, Isaac."
/// Called by SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN once
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set. Toggles
/// each lit quarter's FLAG_SOL_SANCTUM_*_LIGHT_SHOWN; Kraden speaks until the
/// caller sets FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN back to
/// SceneId_SolSanctumStatue.
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
    MapGrid_CopyCellsTo(45, 28, 34, 10, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(47, 28, 36, 10, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(45, 29, 34, 11, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
    MapGrid_CopyCellsTo(47, 29, 36, 11, 2, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
             Event_GetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_Jump(ACTOR_KRADEN, 6, 30);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
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
  Event_PlaySong(SE_GLITTER);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Event_WaitFrames(6);
  // Kraden, once: "You found it, Isaac." and "Keep doing that until the light
  // bathes all the remaining quarters."
  if (GameFlag_Get(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT) == 0) {
    speaker = SPEAKER_WINDOW_BELOW | ACTOR_KRADEN;
    Event_SetMessageId(MsgSolSanctumYouFoundIt);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
  }
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN);
}

/// Sol Sanctum: walking up to Kraden in the Luna room once all
/// four quarters are lit. The picture of Luna has changed to Sol: "Look,
/// Isaac! The picture of Luna changed to Sol!" But nothing else has, and he
/// sends Isaac to look for what must have changed. Until
/// FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED is set, which it sets.
static void SolSanctumLunaSolRooms_KradenSeesLunaPictureChange(void) {
  struct Actor *leader;

  if (Event_GetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0) {
  } else {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() == 0) {
    } else {
      Event_Start();
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(577), PIXELS(147));
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 1);
      FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(184), 1);
      // Isaac walks up to Kraden, who hops: "The picture of Luna changed to
      // Sol!"
      Event_SetMessageId(MsgSolSanctumThePictureOfLunaChanged);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 576, 232);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(10);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 152);
      Event_WaitFrames(6);
      Actor_Jump(ACTOR_KRADEN, 6, 30);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Event_WaitFrames(2);
      Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
      // Kraden: "But other than that, nothing looks different..."
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_WaitFrames(40);
      Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
      Event_WaitFrames(30);
      // Kraden: a trap here, and this is all that happens?
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
      Event_WaitFrames(6);
      Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
      Event_WaitFrames(40);
      // Kraden: "Somewhere, something else must have changed." He walks back
      // into the leader.
      SolSanctumLunaSolRooms_ShowMessageAndWait(
          SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 208);
      Event_WaitFrames(40);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Event_WaitFrames(6);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
      Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      Actor_WaitForMoveById(ACTOR_KRADEN);
      Actor_SetPosition(ACTOR_KRADEN, 0, 0);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
      Event_End();
    }
  }
}

/// Sol Sanctum: Kraden notices that the symbol on the floor has changed from
/// Sol to Luna. Called by SolSanctumLunaSolRooms_TintSolRoom once all four
/// quarters are lit, until FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN is set, which it
/// sets. Leaves Kraden off the map.
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void) {
  Sound_PlaySong(MUS_FOREST);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 184);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(376), PIXELS(184));
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 392, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_SetMessageId(MsgSolSanctumLookSymbolFloor);
  Actor_Jump(ACTOR_KRADEN, 4, 30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 376, 184);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(1608), PIXELS(1608));
  Event_WaitFrames(4);
  GameFlag_Set(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
}

/// Sol Sanctum: searching the floor symbol once Kraden has seen
/// it change. The floor opens, and Isaac, then Kraden ("Wait for me, Isaac...
/// I'm on my way!"), rise through it. Does nothing until
/// FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN is set. Sets
/// FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN the first time and takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, to the
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom).
static void SolSanctumLunaSolRooms_RiseToElementalStarRoom(void) {
  struct Actor *actor;
  s32 frame;
  struct Sprite *sprite;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
  } else {
    Event_Start();
    // Isaac steps onto the symbol and Kraden comes out beside him.
    FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(148), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 30);
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(288), PIXELS(120));
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KRADEN, 276, 136);
    Actor_WalkTo(ACTOR_KRADEN, 264, 136);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 136);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_ArmCallback(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
    if (Event_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
      Sound_PlaySong(SE_SHIMMER);
    }
    Event_WaitFrames(40);
    // Reveal the passage only once; later visits reuse its open state.
    if (Event_GetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    } else {
      MapGrid_CopyCellsTo(36, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(44, 59, 17, 38, 2, 1);
      Event_WaitFrames(10);
      MapGrid_CopyCellsTo(38, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(44, 59, 17, 39, 2, 1);
      Event_WaitFrames(10);
      MapGrid_CopyCellsTo(40, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(0, 32, 17, 39, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 40, 2, 1);
      Event_WaitFrames(10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      0);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      MapGrid_CopyCellsTo(42, 62, 17, 36, 2, 3);
      MapGrid_CopyCellsTo(0, 32, 17, 40, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 41, 2, 1);
      Event_WaitFrames(10);
      MapGrid_CopyCellsTo(0, 32, 17, 41, 2, 1);
      MapGrid_CopyCellsTo(44, 59, 17, 42, 2, 1);
      Event_WaitFrames(10);
      MapGrid_CopyCellsTo(0, 32, 17, 42, 2, 3);
      Event_WaitFrames(80);
      TilesetAnim_Resume(9);
      TilesetAnim_Resume(10);
      GameFlag_Set(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
    }
    Actor_TurnToFaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 30);
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    // Kraden: "Ah ha! This has to be it." and "Let's go, Isaac!"
    Event_SetMessageId(MsgSolSanctumHa);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(60);
    // Raise Isaac first, then shrink his sprite into the passage.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveWithinMapBounds(PIXELS(287), -1, PIXELS(114), 1);
    Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 288, 120);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    /* FAKEMATCH(temporary): the zero goes through sprite, whose register is
     * r7; as motionFlags = 0, 2 of 442 instructions differ in tbs-en. */
    sprite = 0;
    actor->motionFlags = (u32)sprite;
    Sound_PlaySong(SE_ASCEND);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
    sprite = actor->sprite;
    frame = 0;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      WaitFrames(1);
      frame++;
    } while (frame != 120);
    Sound_PlaySong(SE_WARP);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      WaitFrames(1);
      frame++;
    } while (frame != 60);
    Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_Jump(ACTOR_KRADEN, 4, 20);
    // Kraden: "Wait for me, Isaac... I'm on my way!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 120);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
    // Repeat the ascent for Kraden before closing the room transition.
    Actor_SetSpeedParameters(ACTOR_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
    actor = Actor_GetById(ACTOR_KRADEN);
    /* FAKEMATCH(temporary): the mask in a variable, so both byte stores wait
     * past the zero; as &= ~ACTOR_FACES_MOTION, 5 of 442 instructions
     * differ in tbs-en. */
    {
      s32 steeringFlags = 254;

      steeringFlags &= actor->steeringFlags;
      frame = 0;
      actor->steeringFlags = steeringFlags;
      actor->motionFlags = frame;
    }
    Sound_PlaySong(SE_ASCEND);
    Actor_SetSpriteColorById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
    sprite = actor->sprite;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      WaitFrames(1);
      frame++;
    } while (frame != 120);
    Sound_PlaySong(SE_WARP);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      WaitFrames(1);
      frame++;
    } while (frame != 60);
    Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Event_WaitFrames(80);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    gFieldWork->transitionFrames = 24;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    WaitFrames(1);
    Import_EventSetExitRequest(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM);
    Event_End();
  }
}

/// Sets up Sol Sanctum's rooms of Sol and Luna and plays the event this
/// entrance calls for. Called by SceneSequence_Run once the actors
/// exist. Blocks Retreat (FLAG_RETREAT_BLOCKED) from Kraden's find until the
/// way up opens, before the eruption; sets
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED at an ordinary entrance;
/// copies each lit quarter into its FLAG_SOL_SANCTUM_*_LIGHT_SHOWN; plays the
/// disarmed trap's flash at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED, the trap
/// at SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED (then
/// darkening the four quarters), the newly lit quarters at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN, and the
/// arrival after the Wise One's rescue at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION until
/// FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN is set. Otherwise it redraws the
/// quarters, pictures, floor and open way up from the story flags, and after
/// the eruption the rooms rumble.
s32 SolSanctumLunaSolRooms_Initialize(void) {
  s32 playedEntranceEvent;
  s32 anyLightLit;

  WaitFrames(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  playedEntranceEvent = 0;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_FOUND) != 0 &&
      GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) == 0 &&
      GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
  }
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED) != 0) {
      PaletteFade_SetTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(1);
      WaitFrames(1);
    } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) != 0) {
      PaletteFade_SetTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
      PaletteFade_StartInterpolation(1);
      WaitFrames(1);
    }
  } else {
    GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) != 0) {
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(120));
    }
  }
  // Play the one-time event this entrance comes back from
  // SceneId_SolSanctumStatue for.
  if (gAdventure.entrance ==
      SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
      SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
      playedEntranceEvent = 1;
    }
  } else if (
      gAdventure.entrance ==
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
  } else if (gAdventure.entrance ==
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
  WaitFrames(4);
  // Restore completed puzzle tiles without replaying the
  // entrance's event.
  if (playedEntranceEvent == 0) {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
      MapGrid_CopyCellsTo(30, 44, 30, 38, 12, 5);
      MapGrid_CopyCellsTo(30, 44, 34, 37, 4, 1);
      MapGrid_CopyCellsTo(14, 41, 32, 41, 8, 4);
      MapGrid_CopyCellsTo(45, 28, 34, 10, 4, 2);
      MapGrid_CopyCellsTo(45, 30, 16, 10, 4, 2);
      MapGrid_CopyCellsTo(14, 45, 14, 41, 8, 4);
      if (gAdventure.entrance !=
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
        if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) == 0) {
          if (GameFlag_Get(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
            MapGrid_CopyCellsTo(0, 32, 17, 39, 2, 1);
            MapGrid_CopyCellsTo(42, 62, 17, 36, 2, 3);
            MapGrid_CopyCellsTo(0, 32, 17, 40, 2, 1);
            MapGrid_CopyCellsTo(0, 32, 17, 41, 2, 1);
            MapGrid_CopyCellsTo(0, 32, 17, 42, 2, 3);
          } else {
            MapGrid_CopyCellsTo(44, 59, 17, 37, 2, 6);
          }
          TilesetAnim_Resume(9);
          TilesetAnim_Resume(10);
        }
      }
      Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    } else {
      anyLightLit = 0;
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
      MapGrid_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  // The arrival after the Wise One's rescue plays only once.
  if (Event_GetFlag(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN) == 0 &&
      gAdventure.entrance ==
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
    SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue();
    GameFlag_Set(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN);
    MapGrid_CopyCellAttributes(15, 6, 2, 1, 17, 6);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
      Scene_QueueMusic(SE_RUMBLE);
      MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
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

/// Advances the active dialogue and pauses for the requested frames.
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames) {
  Event_ShowMessageAndWait(speaker, 0);
  Event_WaitFrames(waitFrames);
}

/// Sol Sanctum after the eruption: the Wise One has sent Isaac and Garet out of
/// the collapsing Elemental Star Room, and they appear here in a white flash,
/// bewildered, as the rooms shake. Garet: "Did that floating eye thing save
/// us?" A stronger tremor throws them aside, and they set off: "Let's get out
/// of here..." Called by SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION until
/// FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN is set; the caller sets it. Sets
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED and clears the other two tint
/// flags.
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void) {
  struct Actor *leader;
  struct PaletteFade *palette;

  Event_Start();
  // The screen opens all white on Isaac and Garet, then fades to the tint.
  FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(134), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  PaletteFade_ApplySource(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_SetTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  Actor_SetPosition(ACTOR_GARET, PIXELS(280), PIXELS(134));
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  // This event reads its heap slots from one address: HEAP_SLOT_PALETTE_FADE
  // is reached from HEAP_SLOT_FIELD's slot. As gFieldWork and
  // gPaletteFadeWork they would load two.
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_SetTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
  PaletteFade_StartInterpolation(120);
  WaitFrames(120);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_UNTINTED);
  GameFlag_Clear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(60);
  WaitFrames(100);
  // Both look around, bewildered, as the rooms give a jolt.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEffect_SetAttached(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 60);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // Garet: "Did that floating eye thing save us?" A stronger tremor throws
  // them both aside.
  Event_SetMessageId(MsgSolSanctumFloatingEyeThing);
  FieldMessage_ShowAtActor(1, 0);
  Event_AskYesNoWithActorAnimation(0, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_WaitFrames(40);
  Sound_PlaySong(SE_TREMOR);
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Event_WaitFrames(40);
  FieldEffect_InitializeSparkles();
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 300, 130);
  Actor_ResetAndSetPosition(ACTOR_GARET, 266, 144);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_WaitFrames(40);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  // Recover from the tremor and bring Garet back into the party.
  Scene_QueueMusic(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // Garet: "Yeah, right! Let's get out of here..."
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_GARET, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  // Through its heap slot rather than gPaletteFadeWork, the spelling this
  // site matched with; the other is not measured.
  palette = HEAP_SLOT_BLOCK(HEAP_SLOT_PALETTE_FADE);
  palette->target[0] = 0;
  palette->target[1] = 0;
  palette->target[2] = 0;
  palette->page = 0;
  palette->duration = 1;
  palette->step = 1;
  palette->padding2a03 = 1;
  Io_Write16(0, &PLTT_BACKDROP);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transitionFrames = 16;
  Event_End();
}

/// Sol Sanctum: talking to Kraden in the Luna room. "Let me know what you find"
/// once Isaac has agreed to look farther up the passage, otherwise "Just wait
/// over there."
static void SolSanctumLunaSolRooms_TalkToKraden(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN) != 0) {
    Event_SetMessageId(MsgSolSanctumKradenLetMeKnowWhat);
  } else {
    Event_SetMessageId(MsgSolSanctumKradenJustWaitOverThere);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
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
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACING_PARTY, 0 },
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
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BEFORE_KRADEN,
     CONDITION_ALWAYS,
     EVENT(SolSanctumLunaSolRooms_KradenSeesLunaPictureChange)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_HEART_OF_SANCTUM,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_KradenFindsHeartOfSanctum)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WAY_OUT,
     CONDITION_ALWAYS, EVENT(SolSanctumLunaSolRooms_KradenStopsPartyLeaving)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST),
     CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST,
     FLAG_SOL_SANCTUM_HERB_FOUND, FIND_ITEM(ITEM_HERB)},
    {TRIGGERS_END},
};
