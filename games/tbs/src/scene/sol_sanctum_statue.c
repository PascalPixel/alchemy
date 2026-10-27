/// @file
/// Sol Sanctum (SceneId_SolSanctumStatue), the room of what Kraden calls the
/// statue puzzle, off the Luna and Sol rooms (SceneId_SolSanctumLunaSolRooms)
/// and leading on to a room with a Psynergy Stone
/// (SceneId_SolSanctumPsynergyStone).
///
/// Eight winged statues stand around a ninth, the centre statue, which bears a
/// sun disc. Four can be pushed, or moved with Move, onto seals in the floor;
/// two others, pushed in front of the centre statue, shine a beam at it; the
/// last two never move. Walking into a statue pushes it a cell
/// (SolSanctumStatue_PushStatue), and Move ends in a check of where it stands.
/// Each move's trigger carries the flag of what it achieves (a seal open, a
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
///   with Move, drops into it (SolSanctumStatue_CenterStatueMoved).
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
/// does not work here, a task draws three rows of sprites that drift
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

#include "common.h"
#include "version.h"
#include "unit/scene/sol_sanctum_statue.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/runtime/runtime.h"
#include "task.h"
#include "resource.h"
#include "mem.h"
#include "heap_slots.h"
#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "event_inline.h"
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
#include "heap.h"
#include "audio_engine_symbols.h"
#include "ability_ids.h"

/// Both beams have opened the hole in the floor before the centre statue.
/// ★ Named from what the player sees: the statue holding a bowl at the head of
/// the hall, where the beams from the statues beside it meet on the floor
/// before it (its render)
#define FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN FLAG_SCENE_TEMP_07

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
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

/// The metatile behaviors the step triggers answer: the jump spots,
/// the four seal statues' and the centre statue's cells. The beam statues'
/// rows answer the behaviors of the cells each can stand in, 20 to 29.
enum SolSanctumStatueMetatileBehavior {
  MB_SOL_SANCTUM_STATUE_NORTHWEST_STATUE = 5,
  MB_SOL_SANCTUM_STATUE_NORTHEAST_STATUE = 6,
  MB_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE = 7,
  MB_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE = 8,
  MB_SOL_SANCTUM_STATUE_JUMP_NORTH = 10, ///< a jump spot, crossed northward
  MB_SOL_SANCTUM_STATUE_JUMP_SOUTH = 11, ///< a jump spot, crossed southward
  MB_SOL_SANCTUM_STATUE_CENTER_STATUE = 32
};

// Scene flags, cleared on every ordinary arrival:
// - FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN: both beams have opened the hole in the
//   floor; until then, Move on the centre statue first walks the leader clear
//   of it.

static void SolSanctumStatue_InitializeScrollingSprites(void);
static void SolSanctumStatue_CenterStatueMoved(void);
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn,
                                        s32 statueRow, s32 leaderColumn,
                                        s32 leaderRow);
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row);
static void SolSanctumStatue_OpenNorthwestSeal(void);
static void SolSanctumStatue_OpenNortheastSeal(void);
static void SolSanctumStatue_OpenSouthwestSeal(void);
static void SolSanctumStatue_OpenSoutheastSeal(void);
static void SolSanctumStatue_OpenSeal(void);
static void SolSanctumStatue_ShineLeftBeam(void);
static void SolSanctumStatue_ShineRightBeam(void);
static void SolSanctumStatue_SayThenWait(s32 speaker, s32 frames);

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
struct SealOpening {
  struct CellCopy open;
  struct CellCopy pulseA;
  struct CellCopy pulseB;
  struct CellCopy opened;
  s32 flag;
  s32 facing;       ///< 1/65536 turns
  s32 destinationX; ///< pixels
  s32 destinationZ; ///< pixels
  struct CellCopy stairs[3];
  s32 actorId;
  s32 actorX;    ///< pixels
  s32 actorZ;    ///< pixels
  s32 unused[3]; ///< never read or written; the bss keeps its 12 bytes
};

STATIC_ASSERT(offsetof(struct SealOpening, unused) == 0xc4);

/// The placed actors, numbered in the order of
/// gSolSanctumStatue_ActorTemplates: Kraden, who stays out of sight until
/// his two visits, the eight statues, and the centre statue in its two places;
/// Jenna and Garet, placed after them, keep their own slots. Of the statues,
/// four are pushed onto seals and two shine beams at the centre; the other two
/// never move.
enum SolSanctumStatueActorId {
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_NORTHWEST_STATUE,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_LEFT_BEAM_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_NORTHEAST_STATUE,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_RIGHT_BEAM_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_SOUTHWEST_STATUE,
  /// South of the left beam statue; no event moves it.
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_LEFT_STILL_STATUE,
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_SOUTHEAST_STATUE,
  /// South of the right beam statue; no event moves it.
  /// ★ Named from the game: "So the statue puzzle was the trap..."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_RIGHT_STILL_STATUE,
  /// ★ Named from the game: "and that other statue was here to... disarm the
  /// trap." (MsgSolSanctumKradenYouFoundIt)
  ACTOR_CENTER_STATUE,
  /// Stands in the hole in the floor once the centre statue has dropped in.
  /// ★ Named from the game: "And dropped the statue into it after that."
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_MOVED_CENTER_STATUE
};

/// The scene's bss, in this order; as commons of one object the linker would
/// sort them itself.
static struct SealOpening sSealOpening;
/// Initialization writes each three-word OAM packet; drawing updates its
/// fields.
static union {
  struct AnimationSpritePart parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
  s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

const struct SceneEntrance *SolSanctumStatue_GetEntrances(void) {
  return gSolSanctumStatue_Entrances;
}

const struct SceneExitRegion *SolSanctumStatue_GetRegions(void) { return NULL; }

const u32 *SolSanctumStatue_GetExits(void) { return gSolSanctumStatue_Exits; }

/// Returns Kraden, the eight statues, the centre statue and its moved copy
/// (ACTOR_KRADEN to ACTOR_MOVED_CENTER_STATUE), then Jenna and Garet.
const struct ActorTemplate *SolSanctumStatue_GetActorTemplates(void) {
  return gSolSanctumStatue_ActorTemplates;
}

/// Returns the statues' Move and push events, each row only until its seal or
/// beam is done.
const struct Trigger *SolSanctumStatue_GetTriggers(void) {
  return gSolSanctumStatue_Triggers;
}

/// Chooses the event in the Luna and Sol rooms that a newly opened
/// seal leads to, and requests its exit: once the centre statue has moved,
/// the quarter it lit flashing in the Luna and Sol rooms
/// (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED), until
/// FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING is set; before then, the
/// falling lights
/// (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED), until
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set. Returns 1 when it requests an
/// exit, otherwise -1.
static s32 SolSanctumStatue_SelectStoryExit(void) {
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
      Import_EventSetExitRequest(
          SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED);
      return 1;
    }
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
    Import_EventSetExitRequest(
        SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED);
    return 1;
  }
  return -1;
}

/// Animates the floor opening before the centre statue once both beams shine,
/// then lets the statue drop in if Move has already brought it there. Called by
/// whichever beam shines second. Sets FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN.
static void SolSanctumStatue_OpenCentralHole(void) {
  s32 flash;

  {
    MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
  }
  Sound_PlaySong(SE_SPARKLE_FALL);
  for (flash = 0; flash != 22; flash++) {
    MapGrid_CopyCellsTo(10, 61, 17, 40, 2, 1);
    Event_WaitFrames(4);
    MapGrid_CopyCellsTo(8, 61, 17, 40, 2, 1);
    Event_WaitFrames(4);
  }
  {
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
  }
  MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
  {
    MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  }
  GameFlag_Set(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN);
  SolSanctumStatue_CenterStatueMoved();
}

/// Kraden's second visit, an event: the first seal opened after the
/// centre statue dropped has set the quarter it lit flashing in the Luna and
/// Sol rooms, and Kraden comes to find the hole in the floor. "You found
/// it, Isaac!"
/// Garet and Jenna tell him how it opened; he decides the statue puzzle was
/// the trap and the centre statue there to disarm it, and leaves to observe
/// from the Luna room. Called by SolSanctumStatue_Initialize at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED
/// until FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED is set, which the caller
/// sets. Clears FLAG_SHOW_AREA_NAME.
static void SolSanctumStatue_ExplainTrapDisarmed(void) {
  struct Actor *leader;

  Event_Start();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(232));
  Event_WaitFrames(1);
  // Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!"
  Event_SetMessageId(MsgSolSanctumKradenYouFoundIt);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(280));
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_WaitFrames(20);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_WaitFrames(30);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_WaitFrames(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  // The camera swings west to the hole at the centre and back.
  FieldCamera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(10);
  // Kraden hops up to the party: "Excellent work, Isaac!"
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Event_WaitFrames(40);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_WaitFrames(40);
  // The screen closes and opens on Kraden by the hole: "Where did that hole
  // come from?" Then back to the party.
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(424), PIXELS(200));
  WaitFrames(1);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(184));
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(157), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  // Garet: two beams shot out of the statues Isaac moved, and the hole
  // appeared. Jenna: Isaac dropped the statue into it.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_WaitFrames(40);
  // Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  // Kraden: the other statue was there to disarm the trap, and now they
  // should be all right.
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_WaitFrames(40);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_WaitFrames(40);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // Kraden: he will observe from the Luna room. He leaves south.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_WaitFrames(10);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 217);
  Event_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 321);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(136), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  // Rejoin the companions to the leader and restore normal field control.
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 16;
  Event_End();
}

/// Kraden's first visit, an event: the party's first seal has sent
/// lights falling through the Luna room around him, and he runs
/// here to warn that the mechanism is a trap. "Wait, Isaac!" Moving one statue
/// springs it; whoever set it hid something very important, perhaps the key
/// to Alchemy itself, and his intuition says the light is the key to the
/// secret. He goes back to the Luna room and Jenna sets the party to work it
/// out. Called by SolSanctumStatue_Initialize at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED until
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED is set, which the caller sets.
/// Clears FLAG_SHOW_AREA_NAME.
static void SolSanctumStatue_ExplainStatuePuzzle(void) {
  struct FieldWork *work;
  struct Actor *leader;

  Event_Start();
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  Import_ActorSetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(296));
  Event_WaitFrames(1);
  // Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?"
  Event_SetMessageId(MsgSolSanctumWait);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_WaitFrames(20);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_WaitFrames(30);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_WaitFrames(6);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  FieldCamera_SetSpeedParameters(PIXELS(28) / 5, PIXELS(7) / 10);
  FieldCamera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(10);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Event_WaitFrames(40);
  // Kraden: "This is a most interesting mechanism you've found, Isaac."
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(40);
  // Kraden: "But this is clearly... some kind of trap." All three start.
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_WaitFrames(30);
  // Garet: "This is...a trap?" Kraden: "Go look in the Luna room".
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_WaitFrames(40);
  Actor_SetAngleToward(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_JENNA, 0);
  Event_WaitFrames(40);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_ResetAndSetPositionInMode2(ACTOR_KRADEN, 432, 200);
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(171), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(80);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Event_WaitFrames(20);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_EAST, 0);
  Event_WaitFrames(30);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(574), -1, PIXELS(171), 1);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_SetPositionAndReset(ACTOR_KRADEN, 576, 184);
  Event_WaitFrames(80);
  // Kraden: they are looking in the right place, but moving one statue
  // springs the trap. Jenna: why would anyone put a trap here?
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  FieldEffect_SpawnAttached(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_WEST, 0);
  Event_WaitFrames(40);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  // Kraden: they have hidden something very important, perhaps the key to
  // Alchemy itself.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_WaitFrames(60);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_LinkPair(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(40);
  Actor_LinkPair(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(40);
  // Kraden: "Should I tell you more?" Either answer, he thinks they would not
  // understand yet.
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId(MsgSolSanctumHonestlyDoubtUnderstand);
  } else {
    Event_SetMessageId(MsgSolSanctumHmphWellTold);
  }
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  // Kraden: "We must try to find a solution to this mystery." Garet:
  // "Sheesh... Another mystery?" Kraden will keep checking the Luna room.
  Event_SetMessageId(MsgSolSanctumTryFindSolution);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_WaitFrames(20);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_WaitFrames(40);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(574), -1, PIXELS(191), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetPositionAndReset(ACTOR_KRADEN, 576, 232);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_WaitFrames(40);
  // Kraden, leaving: his intuition says the light is the key to the secret.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldCamera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 574, 323);
  Import_ActorSetPosition(ACTOR_KRADEN, 0, 0);
  // Leave Isaac in control after Jenna and Garet rejoin him.
  FieldCamera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  FieldCamera_MoveWithinMapBounds(PIXELS(576), -1, PIXELS(136), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_ArmCallback(ACTOR_JENNA, FACING_EAST, 0);
  Event_WaitFrames(10);
  // Jenna: "let's see if we can't figure this out ourselves."
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_JENNA, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Import_ActorSetPosition(ACTOR_JENNA, 0, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  Event_End();
}

/// Pans the camera away from the leader while a patch of the floor flashes
/// ever faster, redraws a larger block of the floor, then restores the camera.
/// Called only from the branch of SolSanctumStatue_OpenSeal that is marked
/// DEAD.
static void SolSanctumStatue_RunSealOpenedSequence(void) {
  struct MapGridScrollWork *grid;
  struct Actor *leader;
  s32 *savedOrigin;
  s32 cameraPosition[3];
  s32 approachSide;
  s32 frame;

  grid = gMapGridWork[0];
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  // Temporarily follow a local point, panning from the leader toward the seal.
  if (leader->z.fixed < PIXELS(179)) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 575, 132);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_WaitFrames(30);
    savedOrigin = grid->origin;
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Event_WaitFrames(1);
    }
    Event_WaitFrames(40);
    approachSide = 1;
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 577, 222);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_WaitFrames(30);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Event_WaitFrames(1);
    }
    Event_WaitFrames(40);
    approachSide = 2;
  }
  // Flash progressively faster before uncovering the stairway.
  for (frame = 0; frame != 6; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_WaitFrames(8);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_WaitFrames(8);
  }
  for (frame = 0; frame != 10; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_WaitFrames(4);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_WaitFrames(4);
  }
  for (frame = 0; frame != 12; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_WaitFrames(2);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_WaitFrames(2);
  }
  MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
  MapGrid_CopyCellsTo(8, 55, 32, 40, 8, 4);
  Event_WaitFrames(60);
  // Reverse the pan before restoring the caller's camera target.
  if (approachSide == 1) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Event_WaitFrames(1);
    }
  } else if (approachSide == 2) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Event_WaitFrames(1);
    }
  }
  grid->origin = savedOrigin;
}

/// Draws the three rows of sky clouds while they overlap the screen. Run each
/// frame as a task that SolSanctumStatue_InitializeScrollingSprites adds.
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
static void SolSanctumStatue_JumpNorth(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    Field_TryJumpForward();
  }
}

/// Walking south over a jump spot jumps the leader two cells ahead.
static void SolSanctumStatue_JumpSouth(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    Field_TryJumpForward();
  }
}

/// Unused: the northwest statue slides by itself to (504, 152), where it
/// opens its seal, unless that seal is already open.
///
/// Nothing calls it; the player moves the statue there with Move instead.
UNUSED static void SolSanctumStatue_UnusedMoveNorthwestStatue(void) {
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0) {
    Event_Start();
    Actor_GetById(ACTOR_NORTHWEST_STATUE);
    Actor_SetSpeed(ACTOR_NORTHWEST_STATUE, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_WalkToAndWait(ACTOR_NORTHWEST_STATUE, 504, 152);
    Event_End();
  }
}

/// Restores the room's statues, beams and seals from their flags and plays
/// Kraden's two visits. Called by FieldLoop_Run once the actors
/// exist. Sets FLAG_RETREAT_BLOCKED; starts the sky's cloud rows; at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED plays
/// SolSanctumStatue_ExplainTrapDisarmed and at
/// SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED
/// SolSanctumStatue_ExplainStatuePuzzle, each once (setting
/// FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED or
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED), and not on a return after a
/// battle.
s32 SolSanctumStatue_Initialize(void) {
  struct Actor *centerEffect;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  SolSanctumStatue_InitializeScrollingSprites();
  // Retreat does not work here (FLAG_RETREAT_BLOCKED).
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  centerEffect = Actor_GetById(ACTOR_MOVED_CENTER_STATUE);
  centerEffect->collisionFlags = 0;
  {
    u8 flags = centerEffect->priorityFlags | 2;

    centerEffect->priorityFlags = flags;
  }
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MOVED_CENTER_STATUE), 0);
  Actor_GetById(ACTOR_MOVED_CENTER_STATUE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MOVED_CENTER_STATUE, 1);
  // Only entrances 3 and 4 keep Jenna and Garet staged for their
  // conversations.
  if ((u16)(gCommon.entrance - 3) > 1) {
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, PIXELS(288), PIXELS(178));
    Actor_SetPosition(ACTOR_CENTER_STATUE, PIXELS(1608), PIXELS(1608));
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 7);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
             GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    }
    if (GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHWEST_STATUE, PIXELS(504), PIXELS(152));
    MapGrid_CopyCellsTo(2, 28, 34, 10, 2, 1);
    MapGrid_CopyCellsTo(2, 30, 16, 10, 2, 1);
    MapGrid_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHEAST_STATUE, PIXELS(648), PIXELS(152));
    MapGrid_CopyCellsTo(4, 28, 36, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 30, 18, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHWEST_STATUE, PIXELS(504), PIXELS(200));
    MapGrid_CopyCellsTo(2, 29, 34, 11, 2, 1);
    MapGrid_CopyCellsTo(2, 31, 16, 11, 2, 1);
    MapGrid_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHEAST_STATUE, PIXELS(648), PIXELS(200));
    MapGrid_CopyCellsTo(4, 29, 36, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 31, 18, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 58, 36, 43, 4, 1);
  }
  if (gCommon.entrance ==
      SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0) {
      Actor_SetPosition(ACTOR_GARET, 0, 0);
      Actor_SetPosition(ACTOR_JENNA, 0, 0);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
    }
  }
  if (gCommon.entrance ==
      SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED) != 0) {
      Actor_SetPosition(ACTOR_GARET, 0, 0);
      Actor_SetPosition(ACTOR_JENNA, 0, 0);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainStatuePuzzle();
      GameFlag_Set(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED);
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
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
  pixels = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
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
  FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  {
    s32 priority = 3200;

    Task_AddFrame(SolSanctumStatue_UpdateScrollingSprites, priority);
  }
}

/// Moving the northwest statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_NORTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void SolSanctumStatue_NorthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHWEST_STATUE, 31, 9) != 0) {
    SolSanctumStatue_OpenNorthwestSeal();
  }
}

/// Moving the northeast statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_NORTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void SolSanctumStatue_NortheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHEAST_STATUE, 40, 9) != 0) {
    SolSanctumStatue_OpenNortheastSeal();
  }
}

/// Moving the southwest statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_SOUTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void SolSanctumStatue_SouthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHWEST_STATUE, 31, 12) != 0) {
    SolSanctumStatue_OpenSouthwestSeal();
  }
}

/// Moving the southeast statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_SOUTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void SolSanctumStatue_SoutheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHEAST_STATUE, 40, 12) != 0) {
    SolSanctumStatue_OpenSoutheastSeal();
  }
}

/// Moving the left beam statue with Move: its cell is freed, and in front of
/// the centre it shines its beam. Reached by Move on ACTOR_LEFT_BEAM_STATUE
/// until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void SolSanctumStatue_LeftBeamStatueMoved(void) {
  Field_SetMetatileBehaviorAt(2, PIXELS(208), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_LEFT_BEAM_STATUE, 14, 7) != 0) {
    SolSanctumStatue_ShineLeftBeam();
  }
}

/// Moving the right beam statue with Move: as the left one. Reached by Move on
/// ACTOR_RIGHT_BEAM_STATUE until FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void SolSanctumStatue_RightBeamStatueMoved(void) {
  Field_SetMetatileBehaviorAt(2, PIXELS(352), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_RIGHT_BEAM_STATUE, 21, 7) != 0) {
    SolSanctumStatue_ShineRightBeam();
  }
}

/// Starting Move beside the centre statue: its cells are blocked while the
/// leader stands in their row. Reached by the first phase of Move on
/// ACTOR_CENTER_STATUE until FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void SolSanctumStatue_PrepareMoveOnCenterStatue(void) {
  struct Actor *leaderPosition;
  s32 column;
  struct Actor *leader;

  leaderPosition = Actor_GetById(ACTOR_PARTY_LEADER);
  column = leaderPosition->x.fixed >> 20;
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if ((leader->z.fixed >> 20) == 8) {
    if ((u32)(column - 17) <= 1) {
      Field_SetMetatileBehaviorAt(2, PIXELS(272), PIXELS(128), MB_WALL);
      Field_SetMetatileBehaviorAt(2, PIXELS(288), PIXELS(128), MB_WALL);
    }
  }
}

/// Starting Move beside the left beam statue: its cell is blocked while the
/// leader stands beside it. Reached by the first phase of Move on
/// ACTOR_LEFT_BEAM_STATUE until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void SolSanctumStatue_PrepareMoveOnLeftBeamStatue(void) {
  s32 column = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20;
  s32 row = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 13) <= 1) {
    Field_SetMetatileBehaviorAt(2, PIXELS(208), PIXELS(112), MB_WALL);
  }
}

/// Starting Move beside the right beam statue: as the left one, on
/// ACTOR_RIGHT_BEAM_STATUE.
static void SolSanctumStatue_PrepareMoveOnRightBeamStatue(void) {
  s32 column = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20;
  s32 row = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 21) <= 1) {
    Field_SetMetatileBehaviorAt(2, PIXELS(352), PIXELS(112), MB_WALL);
  }
}

/// Moving the centre statue with Move, or a beam completing the pair: with
/// both beams shining the statue slides into the opened floor and the room
/// shakes. "Something clicked!" Reached by Move on ACTOR_CENTER_STATUE until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set, and called by
/// SolSanctumStatue_OpenCentralHole. Sets FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
/// and FLAG_SOL_SANCTUM_ALL_SEALS_OPEN when the four seals are open;
/// ACTOR_MOVED_CENTER_STATUE takes the statue's place in the hole.
static void SolSanctumStatue_CenterStatueMoved(void) {
  struct Actor *statue;
  struct Actor *leader;
  s32 statueZ;

  statue = Actor_GetById(ACTOR_CENTER_STATUE);
  Field_SetMetatileBehaviorAt(2, PIXELS(272), PIXELS(128), 0);
  Field_SetMetatileBehaviorAt(2, PIXELS(288), PIXELS(128), 0);
  if (statue != NULL) {
    statueZ = statue->z.fixed;
    Event_Start();
    if ((statueZ >> 20) == 8) {
      if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN) == 0) {
        leader = Actor_GetById(ACTOR_PARTY_LEADER);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 289, 158);
          leader = Actor_GetById(ACTOR_PARTY_LEADER);
          leader->facing = FACING_NORTH;
        }
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
          GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        GameFlag_Set(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
        // The statue slides south into the hole; ACTOR_MOVED_CENTER_STATUE
        // stands in for it there and the room shakes hard, then settles.
        FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
        FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
        FieldCamera_WaitForArrival();
        Actor_GetById(ACTOR_CENTER_STATUE)->steeringFlags &=
            ~ACTOR_FACES_MOTION;
        Actor_SetSpeed(ACTOR_CENTER_STATUE, PIXELS(3), PIXELS(1));
        statue->motionFlags = 0;
        Actor_SetSpritePriority(ACTOR_CENTER_STATUE, 3);
        Sound_PlaySong(SE_STONE_GRIND);
        Actor_ResetAndSetPosition(ACTOR_CENTER_STATUE, 288, 178);
        Event_WaitFrames(8);
        Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, PIXELS(288), PIXELS(178));
        statue->targetX = ACTOR_NO_TARGET;
        statue->targetY = ACTOR_NO_TARGET;
        statue->targetZ = ACTOR_NO_TARGET;
        statue->x.fixed = 0;
        statue->y.fixed = 0;
        statue->z.fixed = 0;
        SET_VELOCITY(statue, 0, 0, 0);
        Import_ActorSetPosition(ACTOR_CENTER_STATUE, 0, 0);
        MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Event_WaitFrames(10);
        Sound_PlaySong(SE_RUMBLE);
        MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Event_WaitFrames(10);
        MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
        Event_WaitFrames(35);
        MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
        Event_WaitFrames(20);
        MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Event_WaitFrames(30);
        MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
        Event_WaitFrames(40);
        Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
        MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Event_WaitFrames(10);
        MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
        Event_WaitFrames(60);
        Sound_PlaySong(SE_THUMP);
        if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
          GameFlag_Set(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
        }
        Event_WaitFrames(40);
        FieldMessage_Show(MsgSolSanctumSomethingClicked, 1);
        MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        MapGrid_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    Event_End();
  }
}

/// Walking into the centre statue from the south: the leader pushes it north.
/// Reached by walking north into metatile behavior 32 until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void SolSanctumStatue_PushCenterStatueNorth(void) {
  struct Actor *movingActor;

  movingActor = Actor_GetById(ACTOR_CENTER_STATUE);
  if (movingActor != 0) {
    if ((movingActor->z.fixed >> 20) == 8) {
      Event_Start();
      Event_PlaySong(SE_PUSH);
      Actor_SetSpeed(ACTOR_CENTER_STATUE, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_GetById(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
      movingActor = Actor_GetById(ACTOR_PARTY_LEADER);
      Actor_SetDestination(ACTOR_PARTY_LEADER, movingActor->x.part.pixel, 136);
      Actor_SetDestination(ACTOR_CENTER_STATUE, 288, 120);
      Actor_WaitForMoveById(ACTOR_CENTER_STATUE);
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_End();
    }
  }
}

/// Walking into the northwest statue: the leader pushes it onto its seal.
/// Reached by walking east into metatile behavior 5 until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void SolSanctumStatue_PushNorthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHWEST_STATUE, 31, 9, 30, 9);
  SolSanctumStatue_OpenNorthwestSeal();
}

/// Walking into the northeast statue: the leader pushes it onto its seal.
/// Reached by walking west into metatile behavior 6 until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void SolSanctumStatue_PushNortheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHEAST_STATUE, 40, 9, 41, 9);
  SolSanctumStatue_OpenNortheastSeal();
}

/// Walking into the southwest statue: the leader pushes it onto its seal.
/// Reached by walking east into metatile behavior 7 until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void SolSanctumStatue_PushSouthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHWEST_STATUE, 31, 12, 30, 12);
  SolSanctumStatue_OpenSouthwestSeal();
}

/// Walking into the southeast statue: the leader pushes it onto its seal.
/// Reached by walking west into metatile behavior 8 until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void SolSanctumStatue_PushSoutheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHEAST_STATUE, 40, 12, 41, 12);
  SolSanctumStatue_OpenSoutheastSeal();
}

/// Walking into the left beam statue from the west: it slides east in front
/// of the centre and shines its beam. Reached by walking east into metatile
/// behaviors 20 and 21 until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void SolSanctumStatue_PushLeftBeamStatueEast(void) {
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 14, 7, 13, 7);
  SolSanctumStatue_ShineLeftBeam();
}

/// Walking into the left beam statue from the north: it slides one cell south.
/// Reached by walking south into metatile behavior 21 until the beam shines.
static void SolSanctumStatue_PushLeftBeamStatueSouth(void) {
  struct Actor *statue = Actor_GetById(ACTOR_LEFT_BEAM_STATUE);
  s32 row;

  if (statue == 0) {
    return;
  }

  row = statue->z.fixed >> 20;
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

/// Walking into the left beam statue from the south: it slides one cell north.
/// Reached by walking north into metatile behaviors 23 and 24 until the beam
/// shines.
static void SolSanctumStatue_PushLeftBeamStatueNorth(void) {
  struct Actor *statue = Actor_GetById(ACTOR_LEFT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row - 1, 13, row);
  }
}

/// Walking into the right beam statue from the east: as the left one. Reached
/// by walking west into metatile behaviors 25 and 26 until
/// FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void SolSanctumStatue_PushRightBeamStatueWest(void) {
  SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
  SolSanctumStatue_ShineRightBeam();
}

/// Walking into the right beam statue from the north: it slides one cell south.
/// Reached by walking south into metatile behavior 26 until the beam shines.
static void SolSanctumStatue_PushRightBeamStatueSouth(void) {
  struct Actor *statue = Actor_GetById(ACTOR_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row + 1, 22, row);
  }
}

/// Walking into the right beam statue from the south: it slides one cell north.
/// Reached by walking north into metatile behaviors 28 and 29 until the beam
/// shines.
static void SolSanctumStatue_PushRightBeamStatueNorth(void) {
  struct Actor *statue = Actor_GetById(ACTOR_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row - 1, 22, row);
  }
}

/// Moves the leader and statue together to the centers of their target cells.
static void SolSanctumStatue_PushStatue(s32 statueId, s32 statueColumn,
                                        s32 statueRow, s32 leaderColumn,
                                        s32 leaderRow) {
  s32 statueX;
  s32 statueZ;

  Event_Start();
  Sound_PlaySong(SE_PUSH);
  Actor_SetSpeed(statueId, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_GetById(statueId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Actor_ResetAndSetPosition(ACTOR_PARTY_LEADER, (leaderColumn << 4) + 8,
                            (leaderRow << 4) + 8);
  statueX = statueColumn << 4;
  statueZ = statueRow << 4;
  Actor_ResetAndSetPosition(statueId, statueX + 8, statueZ + 8);
  Actor_WaitForMoveById(statueId);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_End();
}

/// Returns 1 when the actor stands in the given map cell.
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row) {
  struct Actor *actor = Actor_GetById(actorId);
  if (actor == NULL || (actor->x.fixed >> 20) != column) {
    return 0;
  }
  if ((actor->z.fixed >> 20) != row) {
    return 0;
  }
  return 1;
}

/// Configures the northwest seal animation and statue return position.
static void SolSanctumStatue_OpenNorthwestSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  seal->open.sourceX = 0;
  seal->open.sourceY = 55;
  seal->open.destinationX = 32;
  seal->open.destinationY = 40;
  seal->open.width = 4;
  seal->open.height = 3;
  seal->pulseA.sourceX = 2;
  seal->pulseA.sourceY = 30;
  seal->pulseA.destinationX = 34;
  seal->pulseA.destinationY = 10;
  seal->pulseA.width = 2;
  seal->pulseA.height = 1;
  seal->pulseB.sourceX = 2;
  seal->pulseB.sourceY = 28;
  seal->pulseB.destinationX = 34;
  seal->pulseB.destinationY = 10;
  seal->pulseB.width = 2;
  seal->pulseB.height = 1;
  seal->opened.sourceX = 2;
  seal->opened.sourceY = 30;
  seal->opened.destinationX = 16;
  seal->opened.destinationY = 10;
  seal->opened.width = 2;
  seal->opened.height = 1;
  seal->flag = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
  seal->facing = FACING_SOUTH;
  seal->destinationX = 500;
  seal->destinationZ = 132;
  seal->stairs[0].sourceX = 8;
  seal->stairs[0].sourceY = 55;
  seal->stairs[0].destinationX = 32;
  seal->stairs[0].destinationY = 40;
  seal->stairs[0].width = 4;
  seal->stairs[0].height = 3;
  seal->stairs[1].sourceX = 2;
  seal->stairs[1].sourceY = 30;
  seal->stairs[1].destinationX = 34;
  seal->stairs[1].destinationY = 10;
  seal->stairs[1].width = 2;
  seal->stairs[1].height = 1;
  seal->stairs[2].sourceX = 2;
  seal->stairs[2].sourceY = 28;
  seal->stairs[2].destinationX = 16;
  seal->stairs[2].destinationY = 10;
  seal->stairs[2].width = 2;
  seal->stairs[2].height = 1;
  seal->actorId = ACTOR_NORTHWEST_STATUE;
  seal->actorX = 488;
  seal->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the northeast seal animation and statue return position.
static void SolSanctumStatue_OpenNortheastSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  seal->open.sourceX = 4;
  seal->open.sourceY = 55;
  seal->open.destinationX = 36;
  seal->open.destinationY = 40;
  seal->open.width = 4;
  seal->open.height = 3;
  seal->pulseA.sourceX = 4;
  seal->pulseA.sourceY = 30;
  seal->pulseA.destinationX = 36;
  seal->pulseA.destinationY = 10;
  seal->pulseA.width = 2;
  seal->pulseA.height = 1;
  seal->pulseB.sourceX = 4;
  seal->pulseB.sourceY = 28;
  seal->pulseB.destinationX = 36;
  seal->pulseB.destinationY = 10;
  seal->pulseB.width = 2;
  seal->pulseB.height = 1;
  seal->opened.sourceX = 4;
  seal->opened.sourceY = 30;
  seal->opened.destinationX = 18;
  seal->opened.destinationY = 10;
  seal->opened.width = 2;
  seal->opened.height = 1;
  seal->flag = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
  seal->facing = FACING_SOUTH;
  seal->destinationX = 654;
  seal->destinationZ = 132;
  seal->stairs[0].sourceX = 12;
  seal->stairs[0].sourceY = 55;
  seal->stairs[0].destinationX = 36;
  seal->stairs[0].destinationY = 40;
  seal->stairs[0].width = 4;
  seal->stairs[0].height = 3;
  seal->stairs[1].sourceX = 4;
  seal->stairs[1].sourceY = 30;
  seal->stairs[1].destinationX = 36;
  seal->stairs[1].destinationY = 10;
  seal->stairs[1].width = 2;
  seal->stairs[1].height = 1;
  seal->stairs[2].sourceX = 4;
  seal->stairs[2].sourceY = 28;
  seal->stairs[2].destinationX = 18;
  seal->stairs[2].destinationY = 10;
  seal->stairs[2].width = 2;
  seal->stairs[2].height = 1;
  seal->actorId = ACTOR_NORTHEAST_STATUE;
  seal->actorX = 664;
  seal->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southwest seal animation and statue return position.
static void SolSanctumStatue_OpenSouthwestSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  seal->open.sourceX = 0;
  seal->open.sourceY = 58;
  seal->open.destinationX = 32;
  seal->open.destinationY = 43;
  seal->open.width = 4;
  seal->open.height = 1;
  seal->pulseA.sourceX = 2;
  seal->pulseA.sourceY = 31;
  seal->pulseA.destinationX = 34;
  seal->pulseA.destinationY = 11;
  seal->pulseA.width = 2;
  seal->pulseA.height = 1;
  seal->pulseB.sourceX = 2;
  seal->pulseB.sourceY = 29;
  seal->pulseB.destinationX = 34;
  seal->pulseB.destinationY = 11;
  seal->pulseB.width = 2;
  seal->pulseB.height = 1;
  seal->opened.sourceX = 2;
  seal->opened.sourceY = 31;
  seal->opened.destinationX = 16;
  seal->opened.destinationY = 11;
  seal->opened.width = 2;
  seal->opened.height = 1;
  seal->flag = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
  seal->facing = FACING_NORTH;
  seal->destinationX = 500;
  seal->destinationZ = 216;
  seal->stairs[0].sourceX = 8;
  seal->stairs[0].sourceY = 58;
  seal->stairs[0].destinationX = 32;
  seal->stairs[0].destinationY = 43;
  seal->stairs[0].width = 4;
  seal->stairs[0].height = 1;
  seal->stairs[1].sourceX = 2;
  seal->stairs[1].sourceY = 31;
  seal->stairs[1].destinationX = 34;
  seal->stairs[1].destinationY = 11;
  seal->stairs[1].width = 2;
  seal->stairs[1].height = 1;
  seal->stairs[2].sourceX = 2;
  seal->stairs[2].sourceY = 29;
  seal->stairs[2].destinationX = 16;
  seal->stairs[2].destinationY = 11;
  seal->stairs[2].width = 2;
  seal->stairs[2].height = 1;
  seal->actorId = ACTOR_SOUTHWEST_STATUE;
  seal->actorX = 488;
  seal->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southeast seal animation and statue return position.
static void SolSanctumStatue_OpenSoutheastSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  seal->open.sourceX = 4;
  seal->open.sourceY = 58;
  seal->open.destinationX = 36;
  seal->open.destinationY = 43;
  seal->open.width = 4;
  seal->open.height = 1;
  seal->pulseA.sourceX = 4;
  seal->pulseA.sourceY = 31;
  seal->pulseA.destinationX = 36;
  seal->pulseA.destinationY = 11;
  seal->pulseA.width = 2;
  seal->pulseA.height = 1;
  seal->pulseB.sourceX = 4;
  seal->pulseB.sourceY = 29;
  seal->pulseB.destinationX = 36;
  seal->pulseB.destinationY = 11;
  seal->pulseB.width = 2;
  seal->pulseB.height = 1;
  seal->opened.sourceX = 4;
  seal->opened.sourceY = 31;
  seal->opened.destinationX = 18;
  seal->opened.destinationY = 11;
  seal->opened.width = 2;
  seal->opened.height = 1;
  seal->flag = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
  seal->facing = FACING_NORTH;
  seal->destinationX = 654;
  seal->destinationZ = 216;
  seal->stairs[0].sourceX = 12;
  seal->stairs[0].sourceY = 58;
  seal->stairs[0].destinationX = 36;
  seal->stairs[0].destinationY = 43;
  seal->stairs[0].width = 4;
  seal->stairs[0].height = 1;
  seal->stairs[1].sourceX = 4;
  seal->stairs[1].sourceY = 31;
  seal->stairs[1].destinationX = 36;
  seal->stairs[1].destinationY = 11;
  seal->stairs[1].width = 2;
  seal->stairs[1].height = 1;
  seal->stairs[2].sourceX = 4;
  seal->stairs[2].sourceY = 29;
  seal->stairs[2].destinationX = 18;
  seal->stairs[2].destinationY = 11;
  seal->stairs[2].width = 2;
  seal->stairs[2].height = 1;
  seal->actorId = ACTOR_SOUTHEAST_STATUE;
  seal->actorX = 664;
  seal->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

#define COPY_CELLS(copy)                                                       \
  MapGrid_CopyCellsTo((copy).sourceX, (copy).sourceY, (copy).destinationX,     \
                      (copy).destinationY, (copy).width, (copy).height)

/// Animates the seal and either advances the story or resets its statue.
/// Called by the four OpenXSeal functions with sSealOpening filled in. Sets the
/// seal's flag; when no event follows and the centre statue has not
/// moved, the statue slides back, the leader jumps clear and the flag is
/// cleared again.
static void SolSanctumStatue_OpenSeal(void) {
  struct SealOpening *seal;
  s32 exitResult;
  s32 pulse;

  exitResult = 0;
  Event_Start();
  if (!GameFlag_Get(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
    // Show the configured seal and flash its two tile states.
    FieldCamera_SetSpeedParameters(PIXELS(2), PIXELS(1) / 4);
    FieldCamera_MoveWithinMapBounds(PIXELS(576), -1, PIXELS(172), 1);
    FieldCamera_WaitForArrival();
    Sound_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
    COPY_CELLS(sSealOpening.open);
    for (pulse = 0; pulse != 20; pulse++) {
      Sound_PlaySong(SE_GLITTER);
      COPY_CELLS(sSealOpening.pulseA);
      Event_WaitFrames(4);
      Sound_PlaySong(SE_GLITTER);
      COPY_CELLS(sSealOpening.pulseB);
      Event_WaitFrames(4);
    }
    seal = &sSealOpening;
    COPY_CELLS(seal->opened);
    GameFlag_Set(seal->flag);
    exitResult = SolSanctumStatue_SelectStoryExit();
    // Keep the statue reusable until the central statue has moved.
    if (exitResult == -1) {
      if (!GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
        FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
        Actor_GetById(ACTOR_PARTY_LEADER)->facing = seal->facing;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
        Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
        Actor_ResetAndSetPosition(ACTOR_PARTY_LEADER, seal->destinationX,
                                  seal->destinationZ);
        COPY_CELLS(seal->stairs[0]);
        COPY_CELLS(seal->stairs[1]);
        COPY_CELLS(seal->stairs[2]);
        Actor_GetById(seal->actorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_SetPositionAndCommit(seal->actorId, seal->actorX, seal->actorZ);
        Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
        GameFlag_Clear(seal->flag);
      }
    } else if (exitResult == 0 &&
               GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
      // DEAD: SelectStoryExit returns only 1 or -1, so this branch cannot run.
      if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) &&
          GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) &&
          GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN)) {
        if (!Event_GetFlag(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
          GameFlag_Set(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
          SolSanctumStatue_RunSealOpenedSequence();
        }
      } else if (GameFlag_Get(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN)) {
        Import_EventSetExitRequest(
            SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN);
        exitResult = 1;
      }
    }
  }
  if (exitResult == 1) {
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
  }
  Event_End();
}

/// Draws the left beam and opens the floor when the right beam is also lit.
/// Called when the left beam statue reaches its place in front of the centre,
/// until the centre statue has moved. Sets FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING.
static void SolSanctumStatue_ShineLeftBeam(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      FieldCamera_WaitForArrival();
      Event_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
      if (Event_GetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
      Event_WaitFrames(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Event_End();
}

/// Draws the right beam and opens the floor when the left beam is also lit.
/// As the left one; sets FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING.
static void SolSanctumStatue_ShineRightBeam(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      FieldCamera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      FieldCamera_WaitForArrival();
      Event_PlaySong(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
      if (Event_GetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
      Event_WaitFrames(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Event_End();
}

/// Shows the selected message from one speaker and waits the requested frames.
static void SolSanctumStatue_SayThenWait(s32 speaker, s32 frames) {
  Event_ShowMessageAndWait(speaker, 0);
  Event_WaitFrames(frames);
}

struct SceneEntrance gSolSanctumStatue_Entrances[] = {
    {SOL_SANCTUM_STATUE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 287, 0, 176,
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
    {SCENE_TABLE_END},
};

u32 gSolSanctumStatue_Exits[] = {
    SCENE_EXITS(SolSanctumStatue),
    SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
               SolSanctumLunaSolRooms,
               SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE),
    SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
               SolSanctumPsynergyStone,
               SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE),
    SCENE_EXIT(
        SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
        SolSanctumLunaSolRooms,
        SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED),
    SCENE_EXIT(
        SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
        SolSanctumLunaSolRooms,
        SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED),
    SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
               SolSanctumLunaSolRooms,
               SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN),
    SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
               SolSanctumLunaSolRooms,
               SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED),
    EXIT_TO_BATTLE(SOL_SANCTUM_STATUE_TO_BATTLE, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gSolSanctumStatue_ActorTemplates[] = {
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(216), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SUN_DISC_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(288), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SUN_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(560), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(592), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gSolSanctumStatue_Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
    {STEP_TRIGGER_WALKING, MB_SOL_SANCTUM_STATUE_JUMP_NORTH, CONDITION_ALWAYS,
     EVENT(SolSanctumStatue_JumpNorth)},
    {STEP_TRIGGER_WALKING, MB_SOL_SANCTUM_STATUE_JUMP_SOUTH, CONDITION_ALWAYS,
     EVENT(SolSanctumStatue_JumpSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_NorthwestStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT(SolSanctumStatue_NortheastStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_SouthwestStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT(SolSanctumStatue_SoutheastStatueMoved)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     MB_SOL_SANCTUM_STATUE_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushNorthwestStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     MB_SOL_SANCTUM_STATUE_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushNortheastStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     MB_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushSouthwestStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     MB_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushSoutheastStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LEFT_BEAM_STATUE, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PrepareMoveOnLeftBeamStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_LeftBeamStatueMoved)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RIGHT_BEAM_STATUE, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PrepareMoveOnRightBeamStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_RightBeamStatueMoved)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushLeftBeamStatueEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushLeftBeamStatueEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushLeftBeamStatueSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 23,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushLeftBeamStatueNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 24,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushLeftBeamStatueNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 25,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushRightBeamStatueWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 26,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushRightBeamStatueWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushRightBeamStatueSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 28,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushRightBeamStatueNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT(SolSanctumStatue_PushRightBeamStatueNorth)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT(SolSanctumStatue_PrepareMoveOnCenterStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT(SolSanctumStatue_CenterStatueMoved)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     MB_SOL_SANCTUM_STATUE_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT(SolSanctumStatue_PushCenterStatueNorth)},
    {TRIGGERS_END},
};

s32 gSolSanctumStatue_ScrollPhase = 0;
