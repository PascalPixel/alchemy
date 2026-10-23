/// @file
/// Sol Sanctum (MapId_SolSanctumStatue), the room of what Kraden calls the
/// statue puzzle, off the Luna and Sol rooms (MapId_SolSanctumLunaSolRooms)
/// and leading on to a room with a Psynergy Stone
/// (MapId_SolSanctumPsynergyStone).
///
/// Eight winged statues stand around a ninth, the centre statue, which bears a
/// sun disc. Four can be pushed, or moved with Move, onto seals in the floor;
/// two others, pushed in front of the centre statue, shine a beam at it; the
/// last two never move. Walking into a statue pushes it a cell
/// (SolSanctumStatue_PushStatue), and Move ends in a check of where it stands.
/// Each move's map event carries the flag of what it achieves (a seal open, a
/// beam shining, the centre statue moved), so a statue stops answering once it
/// has done its part, and SolSanctumStatue_Initialize redraws the room from
/// those story flags on every entry.
///
/// The puzzle is a trap, and the room follows the story flags in order:
/// - Until the centre statue has moved (FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED),
///   the first seal opened cuts away to the Luna and Sol rooms, where the trap
///   is sprung
///   (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
///   FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN). Kraden then comes here to explain
///   that moving one statue springs the sanctum's trap
///   (SolSanctumStatue_ExplainStatuePuzzle,
///   FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED), and from then on a seal opened
///   too soon closes again: its statue slides back and the leader jumps clear.
/// - With both beams shining, the floor before the centre statue opens
///   (SolSanctumStatue_OpenCentralHole), and the centre statue, moved south
///   with Move, drops into it (EventScript_CenterStatueMoved).
/// - From then on the seals stay open. The first one opened cuts away to the
///   Luna and Sol rooms with the trap disarmed
///   (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED), and
///   Kraden comes here to find the hole (SolSanctumStatue_ExplainTrapDisarmed,
///   FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED). Once all four seals are open
///   (FLAG_SOL_SANCTUM_ALL_SEALS_OPEN), the way on is found in the Luna and Sol
///   rooms.
///
/// Both of Kraden's visits hold Jenna and Garet in place beside the leader,
/// then walk them back into the party; any other arrival hides them. Retreat
/// does not work here, a callback draws three rows of sprites that drift
/// sideways at different speeds (SolSanctumStatue_UpdateScrollingSprites): the
/// clouds of the sky seen between the pillars of the colonnade along the
/// room's north wall, 80 pixels down the map at the lowest priority, so they
/// pass in front of the sky and behind the pillars; the rows move an eighth, a
/// quarter and a half of a pixel a frame, the fastest drawn lowest
/// (https://alchemy.pascalpixel.com/places/sol-sanctum-statue/sky-rows.gif: the
/// top of the screen twice its size, above the sprites alone and the
/// backgrounds alone, the clouds drifting behind the pillars), and
/// once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED) the room rumbles and
/// shakes, as the rest of the sanctum does.
///
/// Open questions: none.

#include "game_state.h"
#include "version.h"
#include "unit/map/sol_sanctum_statue.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"
#include "task.h"
#include "resource.h"
#include "mem.h"
#include "heap_slots.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "field_service.h"
#include "call.h"
#include "text/msg_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "animspr.h"
#include "map_grid_scroll.h"
#include "fixed_math.h"
#include "resource_37de_map.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "ability_ids.h"

// Map section flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another map section, and other map sections use the numbers for their
// own.
/// Set once Kraden has found the hole and explained the disarmed trap.
/// ★ Named from the game: "So the statue puzzle was the trap..."
/// (MsgSolSanctumKradenYouFoundIt)
#define FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED 0x30a
/// Set once Kraden has explained the statue puzzle's trap.
/// ★ Named from the game: "But if you just move one statue, you'll be caught
/// in the sanctum's trap." (MsgSolSanctumWait)
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED 0x30b

// The third patch draws the back row of drifting sprites 6 pixels below the
// other two rather than 8.
#if TBS_PATCH_3
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 6
#else
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 8
#endif

#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH 32

/// The metatile behaviors the coord events answer: the jump spots,
/// the four seal statues' and the centre statue's cells. The beam statues'
/// rows answer the behaviors of the cells each can stand in, 20 to 29.
enum SolSanctumStatueCell {
  SOL_SANCTUM_STATUE_CELL_NORTHWEST_STATUE = 5,
  SOL_SANCTUM_STATUE_CELL_NORTHEAST_STATUE = 6,
  SOL_SANCTUM_STATUE_CELL_SOUTHWEST_STATUE = 7,
  SOL_SANCTUM_STATUE_CELL_SOUTHEAST_STATUE = 8,
  SOL_SANCTUM_STATUE_CELL_JUMP_NORTH = 10, ///< a jump spot, crossed northward
  SOL_SANCTUM_STATUE_CELL_JUMP_SOUTH = 11, ///< a jump spot, crossed southward
  SOL_SANCTUM_STATUE_CELL_CENTER_STATUE = 32
};

// Map flags, cleared on every ordinary arrival:
// - FLAG_MAP_TEMP_07: both beams have opened the hole in the floor;
//   until then, Move on the centre statue first walks the leader clear of it.

static void SolSanctumStatue_InitializeScrollingSprites(void);
static void EventScript_CenterStatueMoved(void);
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn,
                                        s32 statueRow, s32 leaderColumn,
                                        s32 leaderRow);
static s32 SolSanctumStatue_IsObjectEventAtCell(s32 localId, s32 column,
                                                s32 row);
static void SolSanctumStatue_OpenNorthwestSeal(void);
static void SolSanctumStatue_OpenNortheastSeal(void);
static void SolSanctumStatue_OpenSouthwestSeal(void);
static void SolSanctumStatue_OpenSoutheastSeal(void);
static void SolSanctumStatue_OpenSeal(void);
static void SolSanctumStatue_ShineLeftBeam(void);
static void SolSanctumStatue_ShineRightBeam(void);
static void EventScript_SayThenWait(s32 speaker, s32 frames);

#define NULL ((void *)0)

/// A block of cells copied from one place in the map grid to another.
struct CellCopy {
  s32 sourceX;      ///< map cells
  s32 sourceY;      ///< map cells
  s32 destinationX; ///< map cells
  s32 destinationY; ///< map cells
  s32 width;        ///< map cells
  s32 height;       ///< map cells
};

/// The seal being opened: its cell animations, its flag, where the leader
/// jumps back to and where its statue returns if the story does not go on.
struct SealScene {
  struct CellCopy open;
  struct CellCopy pulseA;
  struct CellCopy pulseB;
  struct CellCopy opened;
  s32 flag;
  s32 facing;       ///< 1/65536 turns
  s32 destinationX; ///< pixels
  s32 destinationZ; ///< pixels
  struct CellCopy stairs[3];
  s32 localId;
  s32 objectEventX; ///< pixels
  s32 objectEventZ; ///< pixels
  s32 unused[3];    ///< never read or written; the bss keeps its 12 bytes
};

STATIC_ASSERT(offsetof(struct SealScene, unused) == 0xc4);

/// The placed object events, numbered in the order of
/// gSolSanctumStatue_ObjectEventTemplates: Kraden, who stays out of sight until
/// his two visits, the eight statues, and the centre statue in its two places;
/// Jenna and Garet, placed after them, keep their own slots. Of the statues,
/// four are pushed onto seals and two shine beams at the centre; the other two
/// never move.
enum SolSanctumStatueLocalId {
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  LOCALID_KRADEN = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_NORTHWEST_STATUE,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_LEFT_BEAM_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_NORTHEAST_STATUE,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_RIGHT_BEAM_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_SOUTHWEST_STATUE,
  /// South of the left beam statue; no event moves it.
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_LEFT_STILL_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_SOUTHEAST_STATUE,
  /// South of the right beam statue; no event moves it.
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_RIGHT_STILL_STATUE,
  /// ★ Named from the game: "and that other statue was here to... disarm the
  /// trap." (MsgSolSanctumKradenYouFoundIt)
  LOCALID_CENTER_STATUE,
  /// Stands in the hole in the floor once the centre statue has dropped in.
  /// ★ Named from the game: "And dropped the statue into it after that."
  /// (MsgSolSanctumKradenYouFoundIt)
  LOCALID_MOVED_CENTER_STATUE
};

/// The map's bss, in this order; as commons of one object the linker would
/// sort them itself.
static struct SealScene sSealScene;
/// Initialization writes each three-word OAM packet; drawing updates its
/// fields.
static union {
  struct AnimationSpritePart parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
  s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

const struct MapWarpDest *SolSanctumStatue_GetWarpDests(void) {
  return gSolSanctumStatue_WarpDests;
}

const struct MapWarpRegion *SolSanctumStatue_GetRegions(void) { return NULL; }

const u32 *SolSanctumStatue_GetWarps(void) { return gSolSanctumStatue_Warps; }

/// Returns Kraden, the eight statues, the centre statue and its moved copy
/// (LOCALID_KRADEN to LOCALID_MOVED_CENTER_STATUE), then Jenna and Garet.
const struct ObjectEventTemplate *
SolSanctumStatue_GetObjectEventTemplates(void) {
  return gSolSanctumStatue_ObjectEventTemplates;
}

/// Returns the statues' Move and push events, each row only until its seal or
/// beam is done.
const struct MapEvent *SolSanctumStatue_GetEvents(void) {
  return gSolSanctumStatue_Events;
}

/// Chooses the scripted scene in the Luna and Sol rooms that a newly opened
/// seal leads to, and requests its warp: once the centre statue has moved,
/// the quarter it lit flashing in the Luna and Sol rooms
/// (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED), until
/// FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING is set; before then, the
/// falling lights
/// (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED), until
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set. Returns 1 when it requests an
/// warp, otherwise -1.
static s32 SolSanctumStatue_SelectStoryWarp(void) {
  if (GameFlag_Test(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    if (GameFlag_Test(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
      Engine_ScriptRequestWarp(
          SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED);
      return 1;
    }
  } else if (GameFlag_Test(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
    Engine_ScriptRequestWarp(
        SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED);
    return 1;
  }
  return -1;
}

/// Animates the floor opening before the centre statue once both beams shine,
/// then lets the statue drop in if Move has already brought it there. Called
/// by whichever beam shines second. Sets FLAG_MAP_TEMP_07.
static void SolSanctumStatue_OpenCentralHole(void) {
  s32 flash;

  {
    MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
  }
  Sound_PlaySong(SE_200);
  for (flash = 0; flash != 22; flash++) {
    MapGrid_CopyCellsTo(10, 61, 17, 40, 2, 1);
    FieldScript_WaitFrames(4);
    MapGrid_CopyCellsTo(8, 61, 17, 40, 2, 1);
    FieldScript_WaitFrames(4);
  }
  {
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
  }
  MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
  {
    MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  }
  Engine_GameFlagSet(FLAG_MAP_TEMP_07);
  EventScript_CenterStatueMoved();
}

/// Kraden's second visit, a scripted scene: the first seal opened after the
/// centre statue dropped has set the quarter it lit flashing in the Luna and
/// Sol rooms, and Kraden comes to find the hole in the floor. "You found
/// it, Isaac!"
/// Garet and Jenna tell him how it opened; he decides the statue puzzle was
/// the trap and the centre statue there to disarm it, and leaves to observe
/// from the Luna room. Called by SolSanctumStatue_Initialize at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED
/// until FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED is set, which the caller
/// sets. Clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumStatue_ExplainTrapDisarmed(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(576), PIXELS(232));
  FieldScript_WaitFrames(1);
  // Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!"
  FieldScript_SetMessageId(MsgSolSanctumKradenYouFoundIt);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(576), PIXELS(280));
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 216);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(JENNA, 2, 0);
  FieldScript_WaitFrames(30);
  EventScript_SayThenWait(JENNA, 6);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(6);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  // The camera swings west to the hole at the centre and back.
  FieldCamera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  // Kraden hops up to the party: "Excellent work, Isaac!"
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3), PIXELS(2));
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 184);
  FieldScript_WaitFrames(40);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 0);
  FieldScript_WaitFrames(40);
  // The screen closes and opens on Kraden by the hole: "Where did that hole
  // come from?" Then back to the party.
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 2);
  gFieldWork->transitionFrames = 16;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(424), PIXELS(200));
  WaitFrames(1);
  EventScript_SayThenWait(LOCALID_KRADEN, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(576), PIXELS(184));
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(157), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  // Garet: two beams shot out of the statues Isaac moved, and the hole
  // appeared. Jenna: Isaac dropped the statue into it.
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  EventScript_SayThenWait(GARET, 20);
  ObjectMotion_SetAnimationAndWait(JENNA, ANIM_NOD);
  EventScript_SayThenWait(JENNA, 6);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(40);
  // Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_KRADEN, 80);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  EventScript_SayThenWait(LOCALID_KRADEN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  // Kraden: the other statue was there to disarm the trap, and now they
  // should be all right.
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, GARET, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, JENNA, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  FieldScript_WaitFrames(40);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(JENNA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  // Kraden: he will observe from the Luna room. He leaves south.
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 217);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 321);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(136), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  // Rejoin the companions to the leader and restore normal field control.
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ObjectEventSetAnimation(JENNA, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(JENNA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(JENNA);
  ObjectEvent_SetPosition(JENNA, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 16;
  FieldScript_End();
}

/// Kraden's first visit, a scripted scene: the party's first seal has sent
/// lights falling through the Luna room around him, and he runs
/// here to warn that the mechanism is a trap. "Wait, Isaac!" Moving one statue
/// springs it; whoever set it hid something very important, perhaps the key
/// to Alchemy itself, and his intuition says the light is the key to the
/// secret. He goes back to the Luna room and Jenna sets the party to work it
/// out. Called by SolSanctumStatue_Initialize at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED until
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED is set, which the caller sets.
/// Clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumStatue_ExplainStatuePuzzle(void) {
  struct EventWork *work;
  struct ObjectEvent *leader;

  FieldScript_Start();
  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetPosition(LOCALID_KRADEN, PIXELS(576), PIXELS(296));
  FieldScript_WaitFrames(1);
  // Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?"
  FieldScript_SetMessageId(MsgSolSanctumWait);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 216);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(JENNA, 2, 0);
  FieldScript_WaitFrames(30);
  EventScript_SayThenWait(JENNA, 6);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(6);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_WEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  FieldCamera_SetSpeedParameters(PIXELS(28) / 5, PIXELS(7) / 10);
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3), PIXELS(2));
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 184);
  FieldScript_WaitFrames(40);
  // Kraden: "This is a most interesting mechanism you've found, Isaac."
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(JENNA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  // Kraden: "But this is clearly... some kind of trap." All three start.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 3);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  ObjectMotion_Launch(GARET, 2, 0);
  ObjectMotion_Launch(JENNA, 2, 0);
  FieldScript_WaitFrames(30);
  // Garet: "This is...a trap?" Kraden: "Go look in the Luna room".
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  EventScript_SayThenWait(GARET, 6);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, JENNA, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAngleToward(LOCALID_KRADEN, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_KRADEN, JENNA, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 216);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_KRADEN, 432, 200);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(171), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(80);
  Engine_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 216);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(574), -1, PIXELS(171), 1);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3), PIXELS(2));
  ObjectMotion_SetPositionAndReset(LOCALID_KRADEN, 576, 184);
  FieldScript_WaitFrames(80);
  // Kraden: they are looking in the right place, but moving one statue
  // springs the trap. Jenna: why would anyone put a trap here?
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_KRADEN, 20);
  FieldEffect_SpawnLinked(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(JENNA, 2);
  EventScript_SayThenWait(JENNA, 6);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_WEST, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  // Kraden: they have hidden something very important, perhaps the key to
  // Alchemy itself.
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectEvent_LinkPair(GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_LinkPair(JENNA, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(40);
  // Kraden: "Should I tell you more?" Either answer, he thinks they would not
  // understand yet.
  FieldMessage_ShowAtObject(LOCALID_KRADEN, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_SetMessageId(MsgSolSanctumHonestlyDoubtUnderstand);
  } else {
    FieldScript_SetMessageId(MsgSolSanctumHmphWellTold);
  }
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  // Kraden: "We must try to find a solution to this mystery." Garet:
  // "Sheesh... Another mystery?" Kraden will keep checking the Luna room.
  FieldScript_SetMessageId(MsgSolSanctumTryFindSolution);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  FieldEffect_SpawnLinked(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  EventScript_SayThenWait(GARET, 6);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(LOCALID_KRADEN, 6, 0);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 576, 216);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(191), 1);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
  ObjectMotion_SetPositionAndReset(LOCALID_KRADEN, 576, 232);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(40);
  // Kraden, leaving: his intuition says the light is the key to the secret.
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectMotion_ArmCallback(LOCALID_KRADEN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  EventScript_SayThenWait(LOCALID_KRADEN, 6);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 574, 323);
  Engine_ObjectEventSetPosition(LOCALID_KRADEN, 0, 0);
  // Leave Isaac in control after Jenna and Garet rejoin him.
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  FieldCamera_MoveWithinMapBounds(PIXELS(576), -1, PIXELS(136), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(JENNA, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  // Jenna: "let's see if we can't figure this out ourselves."
  EventScript_SayThenWait(JENNA, 6);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(JENNA, ANIM_NOD);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(JENNA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(JENNA, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(JENNA);
  Engine_ObjectEventSetPosition(JENNA, 0, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(GARET, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  FieldScript_End();
}

/// Pans the camera away from the leader while a patch of the floor flashes
/// ever faster, redraws a larger block of the floor, then restores the camera.
/// Called only from the branch of SolSanctumStatue_OpenSeal that is marked
/// DEAD.
static void SolSanctumStatue_RunSealOpenedSequence(void) {
  struct MapGridScrollWork *grid;
  struct ObjectEvent *leader;
  s32 *savedOrigin;
  s32 cameraPosition[3];
  s32 approachSide;
  s32 frame;

  grid = gMapGridWork[0];
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  // Temporarily follow a local point, panning from the leader toward the seal.
  if (leader->z.fixed < PIXELS(179)) {
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 575, 132);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    FieldScript_WaitFrames(30);
    savedOrigin = grid->origin;
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      FieldScript_WaitFrames(1);
    }
    FieldScript_WaitFrames(40);
    approachSide = 1;
  } else {
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 577, 222);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      FieldScript_WaitFrames(1);
    }
    FieldScript_WaitFrames(40);
    approachSide = 2;
  }
  // Flash progressively faster before uncovering the stairway.
  for (frame = 0; frame != 6; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    FieldScript_WaitFrames(8);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    FieldScript_WaitFrames(8);
  }
  for (frame = 0; frame != 10; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    FieldScript_WaitFrames(4);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    FieldScript_WaitFrames(4);
  }
  for (frame = 0; frame != 12; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    FieldScript_WaitFrames(2);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    FieldScript_WaitFrames(2);
  }
  MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
  MapGrid_CopyCellsTo(8, 55, 32, 40, 8, 4);
  FieldScript_WaitFrames(60);
  // Reverse the pan before restoring the caller's camera target.
  if (approachSide == 1) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      FieldScript_WaitFrames(1);
    }
  } else if (approachSide == 2) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      FieldScript_WaitFrames(1);
    }
  }
  grid->origin = savedOrigin;
}

/// Draws the three rows of sky clouds while they overlap the screen. Run each
/// frame as a callback that SolSanctumStatue_InitializeScrollingSprites adds.
static void SolSanctumStatue_UpdateScrollingSprites(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct AnimationSpritePart *sprite = sScrollingSprites.parts;
#ifdef UBFIX
  s32 cameraX = grid->viewX / PIXELS(1);
  s32 screenY = 80 - grid->viewY / PIXELS(1);
#else
  // UB: viewX and viewY are separate scalar members, not an array. Reading
  // them by name adds four bytes in tbs-en.
  s32 *view = &grid->viewX;
  s32 cameraX = view[0] / PIXELS(1);
  s32 screenY = 80 - view[1] / PIXELS(1);
#endif
  s32 scrollX;
  u32 column;

  if ((u32)(screenY + 16) <= 175) {
    // Keep the first sprite just left of the screen for seamless tiling.
    scrollX = (gSolSanctumStatue_ScrollPhase >> 10) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY;
      Oam_AddSpriteAtDepth(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 9) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY;
      Oam_AddSpriteAtDepth(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 8) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY + SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET;
      Oam_AddSpriteAtDepth(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
  }
  // The three shifts turn this into 1/8, 1/4 and 1/2 pixel drift per frame.
  gSolSanctumStatue_ScrollPhase += 128;
}

/// Walking north over a jump spot jumps the leader two cells ahead
/// (Field_TryJumpForward).
static void EventScript_JumpNorth(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    Field_TryJumpForward();
  }
}

/// Walking south over a jump spot jumps the leader two cells ahead.
static void EventScript_JumpSouth(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    Field_TryJumpForward();
  }
}

/// Unused: the northwest statue slides by itself to (504, 152), where it
/// opens its seal, unless that seal is already open.
///
/// Nothing calls it; the player moves the statue there with Move instead.
UNUSED static void SolSanctumStatue_UnusedMoveNorthwestStatue(void) {
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0) {
    FieldScript_Start();
    ObjectEvent_GetById(LOCALID_NORTHWEST_STATUE);
    ObjectEvent_SetSpeed(LOCALID_NORTHWEST_STATUE, PIXELS(1) / 5,
                         PIXELS(1) / 10);
    ObjectEvent_WalkToAndWait(LOCALID_NORTHWEST_STATUE, 504, 152);
    FieldScript_End();
  }
}

/// Restores the room's statues, beams and seals from their flags and plays
/// Kraden's two visits. Called by Map_Run once the object events exist.
/// Sets FLAG_RETREAT_BLOCKED; starts the sky's cloud rows; at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED plays
/// SolSanctumStatue_ExplainTrapDisarmed and at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED
/// SolSanctumStatue_ExplainStatuePuzzle, each once (setting
/// FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED or
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED), and not on a return after a
/// battle.
s32 SolSanctumStatue_Initialize(void) {
  struct ObjectEvent *centerEffect;

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
  SolSanctumStatue_InitializeScrollingSprites();
  // Retreat does not work here (FLAG_RETREAT_BLOCKED).
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  centerEffect = ObjectEvent_GetById(LOCALID_MOVED_CENTER_STATUE);
  centerEffect->collisionFlags = 0;
  {
    u8 flags = centerEffect->priorityFlags | 2;

    centerEffect->priorityFlags = flags;
  }
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_MOVED_CENTER_STATUE),
                             0);
  ObjectEvent_GetById(LOCALID_MOVED_CENTER_STATUE)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_MOVED_CENTER_STATUE, 1);
  // Only warp destinations 3 and 4 keep Jenna and Garet staged for their
  // conversations.
  if ((u16)(gGameState.warpDest - 3) > 1) {
    ObjectEvent_SetPosition(JENNA, 0, 0);
    ObjectEvent_SetPosition(GARET, 0, 0);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    ObjectEvent_SetPosition(LOCALID_MOVED_CENTER_STATUE, PIXELS(288),
                            PIXELS(178));
    ObjectEvent_SetPosition(LOCALID_CENTER_STATUE, PIXELS(1608), PIXELS(1608));
    ObjectEvent_SetPosition(LOCALID_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    ObjectEvent_SetPosition(LOCALID_RIGHT_BEAM_STATUE, PIXELS(344),
                            PIXELS(120));
    ObjectEvent_SetPosition(LOCALID_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    ObjectEvent_SetPosition(LOCALID_RIGHT_BEAM_STATUE, PIXELS(344),
                            PIXELS(120));
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 7);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
    ObjectEvent_SetPosition(LOCALID_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    ObjectEvent_SetPosition(LOCALID_RIGHT_BEAM_STATUE, PIXELS(344),
                            PIXELS(120));
    MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
    ObjectEvent_SetPosition(LOCALID_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    ObjectEvent_SetPosition(LOCALID_RIGHT_BEAM_STATUE, PIXELS(344),
                            PIXELS(120));
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
      ObjectEvent_SetPosition(LOCALID_LEFT_BEAM_STATUE, PIXELS(232),
                              PIXELS(120));
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    }
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
      ObjectEvent_SetPosition(LOCALID_RIGHT_BEAM_STATUE, PIXELS(344),
                              PIXELS(120));
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_NORTHWEST_STATUE, PIXELS(504), PIXELS(152));
    MapGrid_CopyCellsTo(2, 28, 34, 10, 2, 1);
    MapGrid_CopyCellsTo(2, 30, 16, 10, 2, 1);
    MapGrid_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_NORTHEAST_STATUE, PIXELS(648), PIXELS(152));
    MapGrid_CopyCellsTo(4, 28, 36, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 30, 18, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_SOUTHWEST_STATUE, PIXELS(504), PIXELS(200));
    MapGrid_CopyCellsTo(2, 29, 34, 11, 2, 1);
    MapGrid_CopyCellsTo(2, 31, 16, 11, 2, 1);
    MapGrid_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    ObjectEvent_SetPosition(LOCALID_SOUTHEAST_STATUE, PIXELS(648), PIXELS(200));
    MapGrid_CopyCellsTo(4, 29, 36, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 31, 18, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 58, 36, 43, 4, 1);
  }
  if (gGameState.warpDest ==
      SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0) {
      ObjectEvent_SetPosition(GARET, 0, 0);
      ObjectEvent_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
    }
  }
  if (gGameState.warpDest ==
      SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED) != 0) {
      ObjectEvent_SetPosition(GARET, 0, 0);
      ObjectEvent_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainStatuePuzzle();
      GameFlag_Set(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED);
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Map_QueueMusic(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    FieldEffect_InitializeSparkles();
  }
  return 0;
}

/// Loads the three strips of graphics and fills in a row of sprites for each:
/// 32 by 8 pixels (attributes 0 and 1, 0x40004000: the wide shape, size 1),
/// at the lowest priority (3), in palettes 10, 13 and 0.
static void SolSanctumStatue_InitializeScrollingSprites(void) {
  u32 column;
  s32 tile;
  s32 *spriteWords;
  u8 *pixels;

  spriteWords = sScrollingSprites.words;
  pixels = AllocHeapSlotEwram(14, 1024);
  Decompress_ByteLz(gSolSanctumStatue_SpriteRowsGraphics, pixels);
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xac00;
  }
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels + 128);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xdc00;
  }
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels + 256);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xc00;
  }
  FreeHeapSlot(14);
  {
    s32 priority = 3200;

    FieldScheduler_AddCallback(SolSanctumStatue_UpdateScrollingSprites,
                               priority);
  }
}

/// Moving the northwest statue with Move: on its seal, the seal opens.
/// Reached by Move on LOCALID_NORTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void EventScript_NorthwestStatueMoved(void) {
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_NORTHWEST_STATUE, 31, 9) !=
      0) {
    SolSanctumStatue_OpenNorthwestSeal();
  }
}

/// Moving the northeast statue with Move: on its seal, the seal opens.
/// Reached by Move on LOCALID_NORTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void EventScript_NortheastStatueMoved(void) {
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_NORTHEAST_STATUE, 40, 9) !=
      0) {
    SolSanctumStatue_OpenNortheastSeal();
  }
}

/// Moving the southwest statue with Move: on its seal, the seal opens.
/// Reached by Move on LOCALID_SOUTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void EventScript_SouthwestStatueMoved(void) {
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_SOUTHWEST_STATUE, 31, 12) !=
      0) {
    SolSanctumStatue_OpenSouthwestSeal();
  }
}

/// Moving the southeast statue with Move: on its seal, the seal opens.
/// Reached by Move on LOCALID_SOUTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void EventScript_SoutheastStatueMoved(void) {
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_SOUTHEAST_STATUE, 40, 12) !=
      0) {
    SolSanctumStatue_OpenSoutheastSeal();
  }
}

/// Moving the left beam statue with Move: its cell is freed, and in front of
/// the centre it shines its beam. Reached by Move on LOCALID_LEFT_BEAM_STATUE
/// until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void EventScript_LeftBeamStatueMoved(void) {
  Field_SetMetatileBehaviorAt(2, PIXELS(208), PIXELS(112), 0);
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_LEFT_BEAM_STATUE, 14, 7) !=
      0) {
    SolSanctumStatue_ShineLeftBeam();
  }
}

/// Moving the right beam statue with Move: as the left one. Reached by Move on
/// LOCALID_RIGHT_BEAM_STATUE until FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void EventScript_RightBeamStatueMoved(void) {
  Field_SetMetatileBehaviorAt(2, PIXELS(352), PIXELS(112), 0);
  if (SolSanctumStatue_IsObjectEventAtCell(LOCALID_RIGHT_BEAM_STATUE, 21, 7) !=
      0) {
    SolSanctumStatue_ShineRightBeam();
  }
}

/// Starting Move beside the centre statue: its cells are blocked while the
/// leader stands in their row. Reached by the first phase of Move on
/// LOCALID_CENTER_STATUE until FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void EventScript_BlockCenterTriggerCells(void) {
  struct ObjectEvent *leaderPosition;
  s32 column;
  struct ObjectEvent *leader;

  leaderPosition = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  column = leaderPosition->x.fixed >> 20;
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((leader->z.fixed >> 20) == 8) {
    if ((u32)(column - 17) <= 1) {
      Field_SetMetatileBehaviorAt(2, PIXELS(272), PIXELS(128), 255);
      Field_SetMetatileBehaviorAt(2, PIXELS(288), PIXELS(128), 255);
    }
  }
}

/// Starting Move beside the left beam statue: its cell is blocked while the
/// leader stands beside it. Reached by the first phase of Move on
/// LOCALID_LEFT_BEAM_STATUE until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void EventScript_BlockLeftTriggerCell(void) {
  s32 column = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 13) <= 1) {
    Field_SetMetatileBehaviorAt(2, PIXELS(208), PIXELS(112), 255);
  }
}

/// Starting Move beside the right beam statue: as the left one, on
/// LOCALID_RIGHT_BEAM_STATUE.
static void EventScript_BlockRightTriggerCell(void) {
  s32 column = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed >> 20;
  s32 row = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 21) <= 1) {
    Field_SetMetatileBehaviorAt(2, PIXELS(352), PIXELS(112), 255);
  }
}

/// Moving the centre statue with Move, or a beam completing the pair: with
/// both beams shining the statue slides into the opened floor and the room
/// shakes. "Something clicked!" Reached by Move on LOCALID_CENTER_STATUE until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set, and called by
/// SolSanctumStatue_OpenCentralHole. Sets FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
/// and FLAG_SOL_SANCTUM_ALL_SEALS_OPEN when the four seals are open;
/// LOCALID_MOVED_CENTER_STATUE takes the statue's place in the hole.
static void EventScript_CenterStatueMoved(void) {
  struct ObjectEvent *statue;
  struct ObjectEvent *leader;
  s32 statueZ;

  statue = ObjectEvent_GetById(LOCALID_CENTER_STATUE);
  Field_SetMetatileBehaviorAt(2, PIXELS(272), PIXELS(128), 0);
  Field_SetMetatileBehaviorAt(2, PIXELS(288), PIXELS(128), 0);
  if (statue != NULL) {
    statueZ = statue->z.fixed;
    FieldScript_Start();
    if ((statueZ >> 20) == 8) {
      if (GameFlag_Test(FLAG_MAP_TEMP_07) == 0) {
        leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 289, 158);
          leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          leader->facing = FACING_NORTH;
        }
      }
      if (GameFlag_Test(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
          GameFlag_Test(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        Engine_GameFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
        // The statue slides south into the hole; LOCALID_MOVED_CENTER_STATUE
        // stands in for it there and the room shakes hard, then settles.
        FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
        FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
        FieldCamera_WaitForArrival();
        ObjectEvent_GetById(LOCALID_CENTER_STATUE)->steeringFlags &=
            ~OBJ_EVENT_FACES_MOTION;
        ObjectEvent_SetSpeed(LOCALID_CENTER_STATUE, PIXELS(3), PIXELS(1));
        statue->motionFlags = 0;
        ObjectMotion_SetActionVariant(LOCALID_CENTER_STATUE, 3);
        Sound_PlaySong(SE_189);
        ObjectMotion_ResetAndSetPosition(LOCALID_CENTER_STATUE, 288, 178);
        FieldScript_WaitFrames(8);
        ObjectEvent_SetPosition(LOCALID_MOVED_CENTER_STATUE, PIXELS(288),
                                PIXELS(178));
        statue->targetX = OBJ_EVENT_NO_TARGET;
        statue->targetY = OBJ_EVENT_NO_TARGET;
        statue->targetZ = OBJ_EVENT_NO_TARGET;
        statue->x.fixed = 0;
        statue->y.fixed = 0;
        statue->z.fixed = 0;
        SET_VELOCITY(statue, 0, 0, 0);
        Engine_ObjectEventSetPosition(LOCALID_CENTER_STATUE, 0, 0);
        MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        FieldScript_WaitFrames(10);
        Sound_PlaySong(SE_RUMBLE);
        MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        FieldScript_WaitFrames(10);
        MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
        FieldScript_WaitFrames(35);
        MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
        FieldScript_WaitFrames(20);
        MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        FieldScript_WaitFrames(30);
        MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
        FieldScript_WaitFrames(40);
        Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
        MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        FieldScript_WaitFrames(10);
        MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
        FieldScript_WaitFrames(60);
        Sound_PlaySong(SE_188);
        if (GameFlag_Test(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Test(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
            GameFlag_Test(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Test(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
          Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
        }
        FieldScript_WaitFrames(40);
        FieldMessage_Show(MsgSolSanctumSomethingClicked, 1);
        MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        MapGrid_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    FieldScript_End();
  }
}

/// Walking into the centre statue from the south: the leader pushes it north.
/// Reached by walking north into metatile behavior 32 until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void EventScript_PushCenterStatueNorth(void) {
  struct ObjectEvent *movingObjectEvent;

  movingObjectEvent = ObjectEvent_GetById(LOCALID_CENTER_STATUE);
  if (movingObjectEvent != 0) {
    if ((movingObjectEvent->z.fixed >> 20) == 8) {
      FieldScript_Start();
      Script_PlaySong(SE_PUSH);
      ObjectEvent_SetSpeed(LOCALID_CENTER_STATUE, PIXELS(1) / 5,
                           PIXELS(1) / 10);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      ObjectEvent_GetById(LOCALID_CENTER_STATUE)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
      movingObjectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      ObjectEvent_SetDestination(LOCALID_PARTY_LEADER,
                                 movingObjectEvent->x.part.pixel, 136);
      ObjectEvent_SetDestination(LOCALID_CENTER_STATUE, 288, 120);
      ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_CENTER_STATUE);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
      FieldScript_End();
    }
  }
}

/// Walking into the northwest statue: the leader pushes it onto its seal.
/// Reached by walking east into metatile behavior 5 until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void EventScript_PushNorthwestStatue(void) {
  SolSanctumStatue_PushStatue(LOCALID_NORTHWEST_STATUE, 31, 9, 30, 9);
  SolSanctumStatue_OpenNorthwestSeal();
}

/// Walking into the northeast statue: the leader pushes it onto its seal.
/// Reached by walking west into metatile behavior 6 until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void EventScript_PushNortheastStatue(void) {
  SolSanctumStatue_PushStatue(LOCALID_NORTHEAST_STATUE, 40, 9, 41, 9);
  SolSanctumStatue_OpenNortheastSeal();
}

/// Walking into the southwest statue: the leader pushes it onto its seal.
/// Reached by walking east into metatile behavior 7 until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void EventScript_PushSouthwestStatue(void) {
  SolSanctumStatue_PushStatue(LOCALID_SOUTHWEST_STATUE, 31, 12, 30, 12);
  SolSanctumStatue_OpenSouthwestSeal();
}

/// Walking into the southeast statue: the leader pushes it onto its seal.
/// Reached by walking west into metatile behavior 8 until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void EventScript_PushSoutheastStatue(void) {
  SolSanctumStatue_PushStatue(LOCALID_SOUTHEAST_STATUE, 40, 12, 41, 12);
  SolSanctumStatue_OpenSoutheastSeal();
}

/// Walking into the left beam statue from the west: it slides east in front
/// of the centre and shines its beam. Reached by walking east into metatile
/// behaviors 20 and 21 until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void EventScript_PushLeftBeamStatueEast(void) {
  SolSanctumStatue_PushStatue(LOCALID_LEFT_BEAM_STATUE, 14, 7, 13, 7);
  SolSanctumStatue_ShineLeftBeam();
}

/// Walking into the left beam statue from the north: it slides one cell south.
/// Reached by walking south into metatile behavior 21 until the beam shines.
static void EventScript_PushLeftBeamStatueSouth(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_LEFT_BEAM_STATUE);
  s32 row;

  if (statue == 0) {
    return;
  }

  row = statue->z.fixed >> 20;
  SolSanctumStatue_PushStatue(LOCALID_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

/// Walking into the left beam statue from the south: it slides one cell north.
/// Reached by walking north into metatile behaviors 23 and 24 until the beam
/// shines.
static void EventScript_PushLeftBeamStatueNorth(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_LEFT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(LOCALID_LEFT_BEAM_STATUE, 13, row - 1, 13, row);
  }
}

/// Walking into the right beam statue from the east: as the left one. Reached
/// by walking west into metatile behaviors 25 and 26 until
/// FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void EventScript_PushRightBeamStatueWest(void) {
  SolSanctumStatue_PushStatue(LOCALID_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
  SolSanctumStatue_ShineRightBeam();
}

/// Walking into the right beam statue from the north: it slides one cell south.
/// Reached by walking south into metatile behavior 26 until the beam shines.
static void EventScript_PushRightBeamStatueSouth(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(LOCALID_RIGHT_BEAM_STATUE, 22, row + 1, 22,
                                row);
  }
}

/// Walking into the right beam statue from the south: it slides one cell north.
/// Reached by walking north into metatile behaviors 28 and 29 until the beam
/// shines.
static void EventScript_PushRightBeamStatueNorth(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(LOCALID_RIGHT_BEAM_STATUE, 22, row - 1, 22,
                                row);
  }
}

/// Moves the leader and statue together to the centers of their target cells.
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn,
                                        s32 statueRow, s32 leaderColumn,
                                        s32 leaderRow) {
  s32 statueX;
  s32 statueZ;

  FieldScript_Start();
  Sound_PlaySong(SE_PUSH);
  ObjectEvent_SetSpeed(statueId, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectMotion_SetSpeedParameters(LOCALID_PARTY_LEADER, PIXELS(1) / 5,
                                  PIXELS(1) / 10);
  ObjectEvent_GetById(statueId)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
  ObjectMotion_ResetAndSetPosition(
      LOCALID_PARTY_LEADER, (leaderColumn << 4) + 8, (leaderRow << 4) + 8);
  statueX = statueColumn << 4;
  statueZ = statueRow << 4;
  ObjectMotion_ResetAndSetPosition(statueId, statueX + 8, statueZ + 8);
  ObjectMotion_CommitCurrentPositionAndActivate(statueId);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldScript_End();
}

/// Returns 1 when the object event stands in the given map cell.
static s32 SolSanctumStatue_IsObjectEventAtCell(s32 localId, s32 column,
                                                s32 row) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
  if (objectEvent == NULL || (objectEvent->x.fixed >> 20) != column) {
    return 0;
  }
  if ((objectEvent->z.fixed >> 20) != row) {
    return 0;
  }
  return 1;
}

/// Configures the northwest seal animation and statue return position.
static void SolSanctumStatue_OpenNorthwestSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.sourceX = 0;
  scene->open.sourceY = 55;
  scene->open.destinationX = 32;
  scene->open.destinationY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.sourceX = 2;
  scene->pulseA.sourceY = 30;
  scene->pulseA.destinationX = 34;
  scene->pulseA.destinationY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.sourceX = 2;
  scene->pulseB.sourceY = 28;
  scene->pulseB.destinationX = 34;
  scene->pulseB.destinationY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.sourceX = 2;
  scene->opened.sourceY = 30;
  scene->opened.destinationX = 16;
  scene->opened.destinationY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destinationX = 500;
  scene->destinationZ = 132;
  scene->stairs[0].sourceX = 8;
  scene->stairs[0].sourceY = 55;
  scene->stairs[0].destinationX = 32;
  scene->stairs[0].destinationY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].sourceX = 2;
  scene->stairs[1].sourceY = 30;
  scene->stairs[1].destinationX = 34;
  scene->stairs[1].destinationY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].sourceX = 2;
  scene->stairs[2].sourceY = 28;
  scene->stairs[2].destinationX = 16;
  scene->stairs[2].destinationY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->localId = LOCALID_NORTHWEST_STATUE;
  scene->objectEventX = 488;
  scene->objectEventZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the northeast seal animation and statue return position.
static void SolSanctumStatue_OpenNortheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.sourceX = 4;
  scene->open.sourceY = 55;
  scene->open.destinationX = 36;
  scene->open.destinationY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.sourceX = 4;
  scene->pulseA.sourceY = 30;
  scene->pulseA.destinationX = 36;
  scene->pulseA.destinationY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.sourceX = 4;
  scene->pulseB.sourceY = 28;
  scene->pulseB.destinationX = 36;
  scene->pulseB.destinationY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.sourceX = 4;
  scene->opened.sourceY = 30;
  scene->opened.destinationX = 18;
  scene->opened.destinationY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destinationX = 654;
  scene->destinationZ = 132;
  scene->stairs[0].sourceX = 12;
  scene->stairs[0].sourceY = 55;
  scene->stairs[0].destinationX = 36;
  scene->stairs[0].destinationY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].sourceX = 4;
  scene->stairs[1].sourceY = 30;
  scene->stairs[1].destinationX = 36;
  scene->stairs[1].destinationY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].sourceX = 4;
  scene->stairs[2].sourceY = 28;
  scene->stairs[2].destinationX = 18;
  scene->stairs[2].destinationY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->localId = LOCALID_NORTHEAST_STATUE;
  scene->objectEventX = 664;
  scene->objectEventZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southwest seal animation and statue return position.
static void SolSanctumStatue_OpenSouthwestSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.sourceX = 0;
  scene->open.sourceY = 58;
  scene->open.destinationX = 32;
  scene->open.destinationY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.sourceX = 2;
  scene->pulseA.sourceY = 31;
  scene->pulseA.destinationX = 34;
  scene->pulseA.destinationY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.sourceX = 2;
  scene->pulseB.sourceY = 29;
  scene->pulseB.destinationX = 34;
  scene->pulseB.destinationY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.sourceX = 2;
  scene->opened.sourceY = 31;
  scene->opened.destinationX = 16;
  scene->opened.destinationY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destinationX = 500;
  scene->destinationZ = 216;
  scene->stairs[0].sourceX = 8;
  scene->stairs[0].sourceY = 58;
  scene->stairs[0].destinationX = 32;
  scene->stairs[0].destinationY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].sourceX = 2;
  scene->stairs[1].sourceY = 31;
  scene->stairs[1].destinationX = 34;
  scene->stairs[1].destinationY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].sourceX = 2;
  scene->stairs[2].sourceY = 29;
  scene->stairs[2].destinationX = 16;
  scene->stairs[2].destinationY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->localId = LOCALID_SOUTHWEST_STATUE;
  scene->objectEventX = 488;
  scene->objectEventZ = 200;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southeast seal animation and statue return position.
static void SolSanctumStatue_OpenSoutheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.sourceX = 4;
  scene->open.sourceY = 58;
  scene->open.destinationX = 36;
  scene->open.destinationY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.sourceX = 4;
  scene->pulseA.sourceY = 31;
  scene->pulseA.destinationX = 36;
  scene->pulseA.destinationY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.sourceX = 4;
  scene->pulseB.sourceY = 29;
  scene->pulseB.destinationX = 36;
  scene->pulseB.destinationY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.sourceX = 4;
  scene->opened.sourceY = 31;
  scene->opened.destinationX = 18;
  scene->opened.destinationY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destinationX = 654;
  scene->destinationZ = 216;
  scene->stairs[0].sourceX = 12;
  scene->stairs[0].sourceY = 58;
  scene->stairs[0].destinationX = 36;
  scene->stairs[0].destinationY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].sourceX = 4;
  scene->stairs[1].sourceY = 31;
  scene->stairs[1].destinationX = 36;
  scene->stairs[1].destinationY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].sourceX = 4;
  scene->stairs[2].sourceY = 29;
  scene->stairs[2].destinationX = 18;
  scene->stairs[2].destinationY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->localId = LOCALID_SOUTHEAST_STATUE;
  scene->objectEventX = 664;
  scene->objectEventZ = 200;
  SolSanctumStatue_OpenSeal();
}

#define COPY_CELLS(copy)                                                       \
  MapGrid_CopyCellsTo((copy).sourceX, (copy).sourceY, (copy).destinationX,     \
                      (copy).destinationY, (copy).width, (copy).height)

/// Animates the seal and either advances the story or resets its statue.
/// Called by the four OpenXSeal functions with sSealScene filled in. Sets the
/// seal's flag; when no scripted scene follows and the centre statue has not
/// moved, the statue slides back, the leader jumps clear and the flag is
/// cleared again.
static void SolSanctumStatue_OpenSeal(void) {
  struct SealScene *scene;
  s32 warpResult;
  s32 pulse;

  warpResult = 0;
  FieldScript_Start();
  if (!GameFlag_Test(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
    // Show the configured seal and flash its two tile states.
    FieldCamera_SetSpeedParameters(PIXELS(2), PIXELS(1) / 4);
    FieldCamera_MoveWithinMapBounds(PIXELS(576), -1, PIXELS(172), 1);
    FieldCamera_WaitForArrival();
    Sound_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
    COPY_CELLS(sSealScene.open);
    for (pulse = 0; pulse != 20; pulse++) {
      Sound_PlaySong(SE_246);
      COPY_CELLS(sSealScene.pulseA);
      FieldScript_WaitFrames(4);
      Sound_PlaySong(SE_246);
      COPY_CELLS(sSealScene.pulseB);
      FieldScript_WaitFrames(4);
    }
    scene = &sSealScene;
    COPY_CELLS(scene->opened);
    Engine_GameFlagSet(scene->flag);
    warpResult = SolSanctumStatue_SelectStoryWarp();
    // Keep the statue reusable until the central statue has moved.
    if (warpResult == -1) {
      if (!GameFlag_Test(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
        FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 1);
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing = scene->facing;
        ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(2));
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
            ~OBJ_EVENT_FACES_MOTION;
        ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
        ObjectMotion_ResetAndSetPosition(
            LOCALID_PARTY_LEADER, scene->destinationX, scene->destinationZ);
        COPY_CELLS(scene->stairs[0]);
        COPY_CELLS(scene->stairs[1]);
        COPY_CELLS(scene->stairs[2]);
        ObjectEvent_GetById(scene->localId)->steeringFlags &=
            ~OBJ_EVENT_FACES_MOTION;
        ObjectMotion_SetPositionAndCommit(scene->localId, scene->objectEventX,
                                          scene->objectEventZ);
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
            OBJ_EVENT_FACES_MOTION;
        GameFlag_ClearBit(scene->flag);
      }
    } else if (warpResult == 0 &&
               GameFlag_Test(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
      // DEAD: SelectStoryWarp returns only 1 or -1, so this branch cannot run.
      if (GameFlag_Test(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) &&
          GameFlag_Test(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) &&
          GameFlag_Test(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN)) {
        if (!GameFlag_IsSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
          Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
          SolSanctumStatue_RunSealOpenedSequence();
        }
      } else if (GameFlag_Test(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN)) {
        Engine_ScriptRequestWarp(
            SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN);
        warpResult = 1;
      }
    }
  }
  if (warpResult == 1) {
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
  }
  FieldScript_End();
}

/// Draws the left beam and opens the floor when the right beam is also lit.
/// Called when the left beam statue reaches its place in front of the centre,
/// until the centre statue has moved. Sets FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING.
static void SolSanctumStatue_ShineLeftBeam(void) {
  FieldScript_Start();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      FieldCamera_WaitForArrival();
      Script_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
      FieldScript_WaitFrames(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  FieldScript_End();
}

/// Draws the right beam and opens the floor when the left beam is also lit.
/// As the left one; sets FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING.
static void SolSanctumStatue_ShineRightBeam(void) {
  FieldScript_Start();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      FieldCamera_WaitForArrival();
      Script_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
      FieldScript_WaitFrames(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  FieldScript_End();
}

/// Shows the selected message from one speaker and waits the requested frames.
static void EventScript_SayThenWait(s32 speaker, s32 frames) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(frames);
}

struct MapWarpDest gSolSanctumStatue_WarpDests[] = {
    {SOL_SANCTUM_STATUE_FROM_VALE_GATE, CONDITION_ALWAYS, 287, 0, 176,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS, 646,
     0, 329, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE, CONDITION_ALWAYS, 647,
     0, 470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
     CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
     CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
     CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
     CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gSolSanctumStatue_Warps[] = {
    MAP_WARPS(SolSanctumStatue),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
             SolSanctumLunaSolRooms,
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
             SolSanctumPsynergyStone,
             SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
             SolSanctumLunaSolRooms,
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
             SolSanctumLunaSolRooms,
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
             SolSanctumLunaSolRooms,
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN),
    MAP_WARP(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
             SolSanctumLunaSolRooms,
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED),
    WARP_TO_BATTLE(SOL_SANCTUM_STATUE_TO_BATTLE, 1),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gSolSanctumStatue_ObjectEventTemplates[] = {
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(216), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SUN_DISC_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(288), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SUN_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(560), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(592), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gSolSanctumStatue_Events[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
    {COORD_EVENT_WALKING, SOL_SANCTUM_STATUE_CELL_JUMP_NORTH, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_JumpNorth)},
    {COORD_EVENT_WALKING, SOL_SANCTUM_STATUE_CELL_JUMP_SOUTH, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_JumpSouth)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_NorthwestStatueMoved)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_NortheastStatueMoved)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_SouthwestStatueMoved)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SOUTHEAST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_SoutheastStatueMoved)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST),
     SOL_SANCTUM_STATUE_CELL_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_PushNorthwestStatue)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST),
     SOL_SANCTUM_STATUE_CELL_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_PushNortheastStatue)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST),
     SOL_SANCTUM_STATUE_CELL_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_PushSouthwestStatue)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST),
     SOL_SANCTUM_STATUE_CELL_SOUTHEAST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(EventScript_PushSoutheastStatue)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_LEFT_BEAM_STATUE, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_BlockLeftTriggerCell)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LEFT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_LeftBeamStatueMoved)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_RIGHT_BEAM_STATUE, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_BlockRightTriggerCell)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_RIGHT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_RightBeamStatueMoved)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 20,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushLeftBeamStatueEast)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 21,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushLeftBeamStatueEast)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 21,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushLeftBeamStatueSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 23,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushLeftBeamStatueNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 24,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushLeftBeamStatueNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 25,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushRightBeamStatueWest)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 26,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushRightBeamStatueWest)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 26,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushRightBeamStatueSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 28,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushRightBeamStatueNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 29,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(EventScript_PushRightBeamStatueNorth)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(EventScript_BlockCenterTriggerCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(EventScript_CenterStatueMoved)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH),
     SOL_SANCTUM_STATUE_CELL_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(EventScript_PushCenterStatueNorth)},
    {MAP_EVENTS_END},
};

s32 gSolSanctumStatue_ScrollPhase = 0;
