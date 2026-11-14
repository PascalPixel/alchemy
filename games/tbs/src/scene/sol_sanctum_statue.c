/// @file
/// Sol Sanctum (SceneId_SolSanctumStatue), the room of what Kraden calls the
/// statue puzzle, off the Luna and Sol rooms (SceneId_SolSanctumLunaSolRooms)
/// and leading on to a room with a Psynergy Stone
/// (SceneId_SolSanctumPsynergyStone).
///
/// Eight winged statues stand around a ninth, the centre statue, which bears a
/// sun disc. Four can be pushed, or moved with Move, onto seals in the floor.
/// Two others, pushed in front of the centre statue, shine a beam at it. The
/// last two never move.
///
/// Walking into a statue pushes it a cell (SolSanctumStatue_PushStatue), and
/// Move ends in a check of where it stands.
///
/// Each move's trigger carries the flag of what it achieves (a seal open, a
/// beam shining, the centre statue moved), so a statue stops answering once it
/// has done its part. Scene_Initialize redraws the room from those
/// story flags on every entry.
///
/// The puzzle is a trap, and the room follows the story flags in order:
/// - Until the centre statue has moved (FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED),
///   the first seal opened cuts away to the Luna and Sol rooms, where the trap
///   is sprung
///   (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
///   FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN). Kraden then comes here to explain
///   that moving one statue springs the sanctum's trap
///   (SolSanctumStatue_ExplainStatuePuzzle,
///   FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED). From then on a seal opened
///   too soon closes again. Its statue slides back and the leader jumps clear.
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
/// then walk them back into the party. Any other arrival hides them.
///
/// Retreat does not work here.
///
/// A task draws three rows of sprites that drift sideways at different speeds
/// (SolSanctumStatue_UpdateScrollingSprites). They are the clouds of the sky
/// seen between the pillars of the colonnade along the room's north wall.
/// They sit 80 pixels down the map at the lowest priority, so they pass in
/// front of the sky and behind the pillars. The rows move an eighth, a
/// quarter and a half of a pixel a frame, the fastest drawn lowest
/// (https://alchemy.pascalpixel.com/places/sol-sanctum-statue/sky-rows.gif: the
/// top of the screen twice its size, above the sprites alone and the
/// backgrounds alone, the clouds drifting behind the pillars).
///
/// Once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED) the room rumbles and
/// shakes, as the rest of the sanctum does.

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/battle_sprite_id.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_release.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumStatue_Entrances[];
extern u32 gSolSanctumStatue_Exits[];
extern struct ActorPlacement gSolSanctumStatue_ActorPlacements[];
extern struct Trigger gSolSanctumStatue_Triggers[];
extern s32 gSolSanctumStatue_ScrollPhase;

/// Both beams have opened the hole in the floor before the centre statue.
#define FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN FLAG_SCENE_TEMP_07

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// Set once Kraden has found the hole and explained the disarmed trap.
#define FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED FLAG_AREA_TEMP_0A
/// Set once Kraden has explained the statue puzzle's trap.
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED FLAG_AREA_TEMP_0B

// The third patch draws the back row of drifting sprites 6 pixels below the
// other two rather than 8.
#if TBS_PATCH_3
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 6
#else
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 8
#endif

#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH 32

/// The cell behaviors the step triggers answer.
///
/// They are the jump spots, the four seal statues' and the centre statue's
/// cells. The beam statues' rows answer the behaviors of the cells each can
/// stand in, 20 to 29.
enum SolSanctumStatueCellBehavior {
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE = 5,
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE = 6,
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE = 7,
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE = 8,
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH =
      10, ///< a jump spot, crossed northward
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH =
      11, ///< a jump spot, crossed southward
  CELL_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE = 32
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

/// A block of cells copied from one place in the map grid to another.
struct CellCopy {
  s32 sourceX;      ///< map cells
  s32 sourceY;      ///< map cells
  s32 destinationX; ///< map cells
  s32 destinationY; ///< map cells
  s32 width;        ///< map cells
  s32 height;       ///< map cells
};

/// Sets a cell copy: columns by rows cells from (x, y) to (toX, toY).
#define CELL_COPY_SET(copy, x, y, toX, toY, columns, rows)                     \
  ((copy).sourceX = (x), (copy).sourceY = (y), (copy).destinationX = (toX),    \
   (copy).destinationY = (toY), (copy).width = (columns),                      \
   (copy).height = (rows))

/// The seal being opened.
///
/// It holds the seal's cell animations, its flag, where the leader jumps back
/// to, the cells that close it again (`closed`) and where its statue returns
/// if the story does not go on.
struct SealOpening {
  struct CellCopy open;
  struct CellCopy pulseA;
  struct CellCopy pulseB;
  struct CellCopy opened;
  s32 flag;
  s32 facing;       ///< 1/65536 turns
  s32 destinationX; ///< pixels
  s32 destinationZ; ///< pixels
  struct CellCopy closed[3];
  s32 actorId;
  s32 actorX;    ///< pixels
  s32 actorZ;    ///< pixels
  s32 unused[3]; ///< never read or written; the bss keeps its 12 bytes
};

STATIC_ASSERT(offsetof(struct SealOpening, unused) == 0xc4);

/// The placed actors, numbered in the order of
/// gSolSanctumStatue_ActorPlacements.
///
/// They are Kraden, who stays out of sight until his two visits, the eight
/// statues, and the centre statue in its two places. Jenna and Garet, placed
/// after them, keep their own slots.
///
/// Of the statues, four are pushed onto seals and two shine beams at the
/// centre. The other two never move.
enum SolSanctumStatueActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_NORTHWEST_STATUE,
  ACTOR_LEFT_BEAM_STATUE,
  ACTOR_NORTHEAST_STATUE,
  ACTOR_RIGHT_BEAM_STATUE,
  ACTOR_SOUTHWEST_STATUE,
  /// South of the left beam statue, and moved by no event.
  ACTOR_LEFT_STILL_STATUE,
  ACTOR_SOUTHEAST_STATUE,
  /// South of the right beam statue, and moved by no event.
  ACTOR_RIGHT_STILL_STATUE,
  ACTOR_CENTER_STATUE,
  /// Stands in the hole in the floor once the centre statue has dropped in.
  ACTOR_MOVED_CENTER_STATUE
};

/// The scene's bss, in this order.
///
/// As commons of one object the linker would sort them itself.
static struct SealOpening sSealOpening ALIGNED(16);
/// Initialization writes each three-word OAM packet, and drawing updates its
/// fields.
static union {
  struct AnimationSpritePart parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
  s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSolSanctumStatue_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSolSanctumStatue_Exits; }

/// Returns Kraden, the eight statues, the centre statue and its moved copy
/// (ACTOR_KRADEN to ACTOR_MOVED_CENTER_STATUE), then Jenna and Garet.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gSolSanctumStatue_ActorPlacements;
}

/// Returns the statues' Move and push events, each row only until its seal or
/// beam is done.
const struct Trigger *Scene_GetTriggers(void) {
  return gSolSanctumStatue_Triggers;
}

/// Chooses the event in the Luna and Sol rooms that a newly opened
/// seal leads to, and requests its exit.
///
/// Returns 1 when it requests an exit, otherwise -1.
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
/// then lets the statue drop in if Move has already brought it there.
static void SolSanctumStatue_OpenCentralHole(void) {
  s32 flash;

  MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
  Sound_Play(SE_SPARKLE_FALL);
  for (flash = 0; flash != 22; flash++) {
    MapGrid_CopyCellsTo(10, 61, 17, 40, 2, 1);
    Event_Wait(4);
    MapGrid_CopyCellsTo(8, 61, 17, 40, 2, 1);
    Event_Wait(4);
  }
  MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
  MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
  MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
  MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  GameFlag_Set(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN);
  SolSanctumStatue_CenterStatueMoved();
}

/// Kraden's second visit, an event in which he comes to find the hole in the
/// floor.
static void SolSanctumStatue_ExplainTrapDisarmed(void) {
  Event_Start();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetPosition(ACTOR_KRADEN, 576, 232);
  Event_Wait(1);
  // Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!"
  Event_SetMessageId(MsgSolSanctumKradenYouFoundIt);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetPosition(ACTOR_KRADEN, 576, 280);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_Wait(20);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_Wait(30);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
  Event_Wait(10);
  // The camera swings west to the hole at the centre and back.
  FieldCamera_SetSpeed(5.6);
  FieldCamera_MoveTo(287, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForMove();
  Event_Wait(60);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(10);
  // Kraden hops up to the party: "Excellent work, Isaac!"
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Event_Wait(40);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(40);
  // The screen closes and opens on Kraden by the hole: "Where did that hole
  // come from?" Then back to the party.
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(287, CAMERA_KEEP, 176, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_SetPosition(ACTOR_KRADEN, 424, 200);
  wait(1);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPosition(ACTOR_KRADEN, 576, 184);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 157, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  // Garet: two beams shot out of the statues Isaac moved, and the hole
  // appeared. Jenna: Isaac dropped the statue into it.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(40);
  // Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(30);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(30);
  // Kraden: the other statue was there to disarm the trap, and now they
  // should be all right.
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_Wait(40);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // Kraden: he will observe from the Luna room. He leaves south.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(576, CAMERA_KEEP, 215, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 217);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 321);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldCamera_SetSpeed(3.6);
  FieldCamera_MoveTo(576, CAMERA_KEEP, 136, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_JENNA);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gFieldWork->transitionFrames = 16;
  Event_End();
}

/// Kraden's first visit, an event in which he runs here to warn that the
/// mechanism is a trap.
static void SolSanctumStatue_ExplainStatuePuzzle(void) {
  struct FieldWork *work;

  Event_Start();
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetPosition(ACTOR_KRADEN, 576, 296);
  Event_Wait(1);
  // Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?"
  Event_SetMessageId(MsgSolSanctumWait);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_Wait(20);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_Wait(30);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
  Event_Wait(10);
  FieldCamera_SetSpeed(5.6);
  FieldCamera_MoveTo(287, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForMove();
  Event_Wait(60);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Event_Wait(40);
  // Kraden: "This is a most interesting mechanism you've found, Isaac."
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(40);
  // Kraden: "But this is clearly... some kind of trap." All three start.
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 3);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Actor_Jump(ACTOR_JENNA, 2, 0);
  Event_Wait(30);
  // Garet: "This is...a trap?" Kraden: "Go look in the Luna room".
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 6);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_JENNA, 0);
  Event_Wait(40);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_WalkTo(ACTOR_KRADEN, 432, 200);
  Event_Wait(20);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 171, 1);
  FieldCamera_WaitForMove();
  Event_Wait(80);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 180, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 171, 1);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Event_Wait(80);
  // Kraden: they are looking in the right place, but moving one statue
  // springs the trap. Jenna: why would anyone put a trap here?
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(30);
  // Kraden: they have hidden something very important, perhaps the key to
  // Alchemy itself.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(40);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(40);
  // Kraden: "Should I tell you more?" Either answer, he thinks they would not
  // understand yet.
  Event_OpenMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
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
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  SolSanctumStatue_SayThenWait(ACTOR_GARET, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Event_Wait(40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(574, CAMERA_KEEP, 191, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 232);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(40);
  // Kraden, leaving: his intuition says the light is the key to the secret.
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(30);
  SolSanctumStatue_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldCamera_MoveTo(576, CAMERA_KEEP, 215, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 574, 323);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldCamera_SetSpeed(3.6);
  FieldCamera_MoveTo(576, CAMERA_KEEP, 136, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(10);
  // Jenna: "let's see if we can't figure this out ourselves."
  SolSanctumStatue_SayThenWait(ACTOR_JENNA, 6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Scene_WalkIntoLeader(ACTOR_JENNA);
  Scene_WalkIntoLeader(ACTOR_GARET);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  Event_End();
}

/// Unused: called only from the branch of SolSanctumStatue_OpenSeal that
/// cannot run, since SolSanctumStatue_SelectStoryExit returns only 1 or -1.
static void SolSanctumStatue_UnusedPutOutSeals(void) {
  struct MapGridScrollWork *grid;
  struct Actor *leader;
  s32 *savedOrigin;
  s32 cameraPosition[3];
  s32 approachSide;
  s32 frame;

  grid = gMapGridWork[0];
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  // Temporarily follow a local point, panning from the leader toward the seal.
  if (leader->z.fixed < px(179)) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 575, 132);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_Wait(30);
    savedOrigin = grid->origin;
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += px(1);
      Event_Wait(1);
    }
    Event_Wait(40);
    approachSide = 1;
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 577, 222);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(30);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = grid->origin;
    grid->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= px(1);
      Event_Wait(1);
    }
    Event_Wait(40);
    approachSide = 2;
  }
  for (frame = 0; frame != 6; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_Wait(8);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_Wait(8);
  }
  for (frame = 0; frame != 10; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_Wait(4);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_Wait(4);
  }
  for (frame = 0; frame != 12; frame++) {
    MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Event_Wait(2);
    MapGrid_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Event_Wait(2);
  }
  MapGrid_CopyCellsTo(2, 28, 34, 10, 4, 2);
  MapGrid_CopyCellsTo(8, 55, 32, 40, 8, 4);
  Event_Wait(60);
  // Reverse the pan before restoring the caller's camera target.
  if (approachSide == 1) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= px(1);
      Event_Wait(1);
    }
  } else if (approachSide == 2) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += px(1);
      Event_Wait(1);
    }
  }
  grid->origin = savedOrigin;
}

/// Draws the three rows of sky clouds while they overlap the screen.
static void SolSanctumStatue_UpdateScrollingSprites(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct AnimationSpritePart *sprite = sScrollingSprites.parts;
  s32 *view = grid->view;
  s32 cameraX = view[0] / px(1);
  s32 screenY = 80 - view[1] / px(1);
  s32 scrollX;
  u32 column;

  if ((u32)(screenY + 16) <= 175) {
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
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    Field_TryJumpForward();
  }
}

/// Walking south over a jump spot jumps the leader two cells ahead.
static void SolSanctumStatue_JumpSouth(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    Field_TryJumpForward();
  }
}

/// Unused: the eastern hall's northwest statue slides by itself to (504, 152),
/// the cell where Move opens its seal, unless that seal is already open.
///
/// This function opens nothing.
///
/// Nothing calls it. The player moves the statue there with Move instead.
UNUSED static void SolSanctumStatue_UnusedMoveNorthwestStatue(void) {
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0) {
    Event_Start();
    Actor_Get(ACTOR_NORTHWEST_STATUE);
    Actor_SetSpeed(ACTOR_NORTHWEST_STATUE, 0.2);
    Actor_WalkToAndWait(ACTOR_NORTHWEST_STATUE, 504, 152);
    Event_End();
  }
}

/// Restores the room's statues, beams and seals from their flags and plays
/// Kraden's two visits.
s32 Scene_Initialize(void) {
  struct Actor *centerEffect;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  SolSanctumStatue_InitializeScrollingSprites();
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  centerEffect = Actor_Get(ACTOR_MOVED_CENTER_STATUE);
  centerEffect->collisionFlags = 0;
  {
    u8 flags = centerEffect->priorityFlags | 2;

    centerEffect->priorityFlags = flags;
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MOVED_CENTER_STATUE), 0);
  Actor_Get(ACTOR_MOVED_CENTER_STATUE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MOVED_CENTER_STATUE, 1);
  // Only entrances 3 and 4 keep Jenna and Garet staged for their
  // conversations.
  if ((u16)(gMainRoutineWork.entrance - 3) > 1) {
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, 288, 178);
    Actor_SetPosition(ACTOR_CENTER_STATUE, 1608, 1608);
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, 232, 120);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, 232, 120);
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 7);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
             GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, 232, 120);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
    MapGrid_CopyCellsTo(0, 28, 17, 8, 2, 1);
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, 232, 120);
    MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
    MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
    MapGrid_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  } else {
    // The cells SolSanctumStatue_ShineLeftBeam copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, 232, 120);
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
    }
    // The cells SolSanctumStatue_ShineRightBeam copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHWEST_STATUE, 504, 152);
    MapGrid_CopyCellsTo(2, 28, 34, 10, 2, 1);
    MapGrid_CopyCellsTo(2, 30, 16, 10, 2, 1);
    MapGrid_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHEAST_STATUE, 648, 152);
    MapGrid_CopyCellsTo(4, 28, 36, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 30, 18, 10, 2, 1);
    MapGrid_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHWEST_STATUE, 504, 200);
    MapGrid_CopyCellsTo(2, 29, 34, 11, 2, 1);
    MapGrid_CopyCellsTo(2, 31, 16, 11, 2, 1);
    MapGrid_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHEAST_STATUE, 648, 200);
    MapGrid_CopyCellsTo(4, 29, 36, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 31, 18, 11, 2, 1);
    MapGrid_CopyCellsTo(4, 58, 36, 43, 4, 1);
  }
  if (gMainRoutineWork.entrance ==
      SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0) {
      Actor_SetPosition(ACTOR_GARET, 0, 0);
      Actor_SetPosition(ACTOR_JENNA, 0, 0);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
    }
  }
  if (gMainRoutineWork.entrance ==
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
    MapGrid_SetShake(1, 1, 1);
    FieldEffect_InitializeSparkles();
  }
  return 0;
}

/// Loads the three strips of graphics and fills in a row of sprites for each.
static void SolSanctumStatue_InitializeScrollingSprites(void) {
  u32 column;
  s32 tile;
  s32 *spriteWords;
  u8 *pixels;

  spriteWords = sScrollingSprites.words;
  pixels = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gSolSanctumStatue_SpriteRowsGraphics, pixels);
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = OBJ_SIZE_32X8;
    spriteWords += 3;
    *attributeWords = tile | OBJ_PRIORITY(3) | OBJ_PALETTE(10);
  }
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels + 128);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = OBJ_SIZE_32X8;
    spriteWords += 3;
    *attributeWords = tile | OBJ_PRIORITY(3) | OBJ_PALETTE(13);
  }
  tile = VramCache_Load(VramCache_FindFreeEntry(), 128, pixels + 256);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = OBJ_SIZE_32X8;
    spriteWords += 3;
    *attributeWords = tile | OBJ_PRIORITY(3);
  }
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  Task_AddFrame(SolSanctumStatue_UpdateScrollingSprites, TASK_PASS_FRAME);
}

/// Moving the northwest statue with Move opens its seal when it stands on it.
static void SolSanctumStatue_NorthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHWEST_STATUE, 31, 9) != 0) {
    SolSanctumStatue_OpenNorthwestSeal();
  }
}

/// Moving the northeast statue with Move opens its seal when it stands on it.
static void SolSanctumStatue_NortheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHEAST_STATUE, 40, 9) != 0) {
    SolSanctumStatue_OpenNortheastSeal();
  }
}

/// Moving the southwest statue with Move opens its seal when it stands on it.
static void SolSanctumStatue_SouthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHWEST_STATUE, 31, 12) != 0) {
    SolSanctumStatue_OpenSouthwestSeal();
  }
}

/// Moving the southeast statue with Move opens its seal when it stands on it.
static void SolSanctumStatue_SoutheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHEAST_STATUE, 40, 12) != 0) {
    SolSanctumStatue_OpenSoutheastSeal();
  }
}

/// Moving the left beam statue with Move frees its cell, and in front of the
/// centre it shines its beam.
static void SolSanctumStatue_LeftBeamStatueMoved(void) {
  Field_SetCellBehaviorAt(2, 208, 112, 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_LEFT_BEAM_STATUE, 14, 7) != 0) {
    SolSanctumStatue_ShineLeftBeam();
  }
}

/// Moving the right beam statue with Move does as the left one.
static void SolSanctumStatue_RightBeamStatueMoved(void) {
  Field_SetCellBehaviorAt(2, 352, 112, 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_RIGHT_BEAM_STATUE, 21, 7) != 0) {
    SolSanctumStatue_ShineRightBeam();
  }
}

/// Starting Move beside the centre statue blocks its cells while the leader
/// stands in their row.
static void SolSanctumStatue_PrepareMoveOnCenterStatue(void) {
  struct Actor *leaderPosition;
  s32 column;
  struct Actor *leader;

  leaderPosition = Actor_Get(ACTOR_PARTY_LEADER);
  column = FIELD_CELL(leaderPosition->x.fixed);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FIELD_CELL(leader->z.fixed) == 8) {
    if (column >= 17 && column <= 18) {
      Field_SetCellBehaviorAt(2, 272, 128, CELL_BEHAVIOR_WALL);
      Field_SetCellBehaviorAt(2, 288, 128, CELL_BEHAVIOR_WALL);
    }
  }
}

/// Starting Move beside the left beam statue blocks its cell while the leader
/// stands beside it.
static void SolSanctumStatue_PrepareMoveOnLeftBeamStatue(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->z.fixed);

  if (row == 7 && (u32)(column - 13) <= 1) {
    Field_SetCellBehaviorAt(2, 208, 112, CELL_BEHAVIOR_WALL);
  }
}

/// Starting Move beside the right beam statue does as the left one, on
/// ACTOR_RIGHT_BEAM_STATUE.
static void SolSanctumStatue_PrepareMoveOnRightBeamStatue(void) {
  s32 column = FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed);
  s32 row = FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->z.fixed);

  if (row == 7 && (u32)(column - 21) <= 1) {
    Field_SetCellBehaviorAt(2, 352, 112, CELL_BEHAVIOR_WALL);
  }
}

/// Moving the centre statue with Move, or a beam completing the pair, drops
/// the statue into the floor.
static void SolSanctumStatue_CenterStatueMoved(void) {
  struct Actor *statue;
  struct Actor *leader;
  s32 statueZ;

  statue = Actor_Get(ACTOR_CENTER_STATUE);
  Field_SetCellBehaviorAt(2, 272, 128, 0);
  Field_SetCellBehaviorAt(2, 288, 128, 0);
  if (statue != NULL) {
    statueZ = statue->z.fixed;
    Event_Start();
    if ((statueZ >> 20) == 8) {
      if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN) == 0) {
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 289, 158);
          leader = Actor_Get(ACTOR_PARTY_LEADER);
          leader->facing = FACING_NORTH;
        }
      }
      if (GameFlag_Get(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
          GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        GameFlag_Set(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
        // The statue slides south into the hole; ACTOR_MOVED_CENTER_STATUE
        // stands in for it there and the room shakes hard, then settles.
        FieldCamera_SetSpeed(2);
        FieldCamera_MoveTo(286, CAMERA_KEEP, 146, 1);
        FieldCamera_WaitForMove();
        Actor_Get(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_SetSpeedAndAcceleration(ACTOR_CENTER_STATUE, 3, 1);
        statue->motionFlags = 0;
        Actor_SetSpritePriority(ACTOR_CENTER_STATUE, 3);
        Sound_Play(SE_STONE_GRIND);
        Actor_SetDestination(ACTOR_CENTER_STATUE, 288, 178);
        Event_Wait(8);
        Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, 288, 178);
        statue->targetX = ACTOR_NO_TARGET;
        statue->targetY = ACTOR_NO_TARGET;
        statue->targetZ = ACTOR_NO_TARGET;
        statue->x.fixed = 0;
        statue->y.fixed = 0;
        statue->z.fixed = 0;
        SET_VELOCITY(statue, 0, 0, 0);
        Actor_SetPosition(ACTOR_CENTER_STATUE, 0, 0);
        MapGrid_SetShake(1, 1, 1);
        Event_Wait(10);
        Sound_Play(SE_RUMBLE);
        MapGrid_SetShake(3, 3, 1);
        Event_Wait(10);
        MapGrid_SetShake(5, 5, 1);
        Event_Wait(35);
        MapGrid_SetShake(4, 4, 1);
        Event_Wait(20);
        MapGrid_SetShake(3, 3, 1);
        Event_Wait(30);
        MapGrid_SetShake(2, 2, 1);
        Event_Wait(40);
        Sound_Play(SOUND_FADE_OUT_ACTION_SE);
        MapGrid_SetShake(1, 1, 1);
        Event_Wait(10);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        Event_Wait(60);
        Sound_Play(SE_THUMP);
        if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
            GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
          GameFlag_Set(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
        }
        Event_Wait(40);
        FieldMessage_Show(MsgSolSanctumSomethingClicked, 1);
        MapGrid_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        MapGrid_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    Event_End();
  }
}

/// Walking into the centre statue from the south makes the leader push it
/// north.
static void SolSanctumStatue_PushCenterStatueNorth(void) {
  struct Actor *movingActor;

  movingActor = Actor_Get(ACTOR_CENTER_STATUE);
  if (movingActor != NULL) {
    if (FIELD_CELL(movingActor->z.fixed) == 8) {
      Event_Start();
      Sound_Play(SE_PUSH);
      Actor_SetSpeed(ACTOR_CENTER_STATUE, 0.2);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
      Actor_Get(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
      movingActor = Actor_Get(ACTOR_PARTY_LEADER);
      Actor_SetDestination(ACTOR_PARTY_LEADER, movingActor->x.part.pixel, 136);
      Actor_SetDestination(ACTOR_CENTER_STATUE, 288, 120);
      Actor_WaitForMoveById(ACTOR_CENTER_STATUE);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_End();
    }
  }
}

/// Walking into the northwest statue makes the leader push it onto its seal.
static void SolSanctumStatue_PushNorthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHWEST_STATUE, 31, 9, 30, 9);
  SolSanctumStatue_OpenNorthwestSeal();
}

/// Walking into the northeast statue makes the leader push it onto its seal.
static void SolSanctumStatue_PushNortheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHEAST_STATUE, 40, 9, 41, 9);
  SolSanctumStatue_OpenNortheastSeal();
}

/// Walking into the southwest statue makes the leader push it onto its seal.
static void SolSanctumStatue_PushSouthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHWEST_STATUE, 31, 12, 30, 12);
  SolSanctumStatue_OpenSouthwestSeal();
}

/// Walking into the southeast statue makes the leader push it onto its seal.
static void SolSanctumStatue_PushSoutheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHEAST_STATUE, 40, 12, 41, 12);
  SolSanctumStatue_OpenSoutheastSeal();
}

/// Walking into the left beam statue from the west slides it east in front
/// of the centre, where it shines its beam.
static void SolSanctumStatue_PushLeftBeamStatueEast(void) {
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 14, 7, 13, 7);
  SolSanctumStatue_ShineLeftBeam();
}

/// Walking into the left beam statue from the north slides it one cell south.
static void SolSanctumStatue_PushLeftBeamStatueSouth(void) {
  struct Actor *statue = Actor_Get(ACTOR_LEFT_BEAM_STATUE);
  s32 row;

  if (statue == NULL) {
    return;
  }

  row = FIELD_CELL(statue->z.fixed);
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

/// Walking into the left beam statue from the south slides it one cell north.
static void SolSanctumStatue_PushLeftBeamStatueNorth(void) {
  struct Actor *statue = Actor_Get(ACTOR_LEFT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = FIELD_CELL(statue->z.fixed);
    SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row - 1, 13, row);
  }
}

/// Walking into the right beam statue from the east does as the left one.
static void SolSanctumStatue_PushRightBeamStatueWest(void) {
  SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
  SolSanctumStatue_ShineRightBeam();
}

/// Walking into the right beam statue from the north slides it one cell south.
static void SolSanctumStatue_PushRightBeamStatueSouth(void) {
  struct Actor *statue = Actor_Get(ACTOR_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = FIELD_CELL(statue->z.fixed);
    SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row + 1, 22, row);
  }
}

/// Walking into the right beam statue from the south slides it one cell north.
static void SolSanctumStatue_PushRightBeamStatueNorth(void) {
  struct Actor *statue = Actor_Get(ACTOR_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = FIELD_CELL(statue->z.fixed);
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
  Sound_Play(SE_PUSH);
  Actor_SetSpeed(statueId, 0.2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  Actor_Get(statueId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Actor_SetDestination(ACTOR_PARTY_LEADER, (leaderColumn << 4) + 8,
                       (leaderRow << 4) + 8);
  statueX = statueColumn << 4;
  statueZ = statueRow << 4;
  Actor_SetDestination(statueId, statueX + 8, statueZ + 8);
  Actor_WaitForMoveById(statueId);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_End();
}

/// Returns 1 when the actor stands in the given map cell.
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row) {
  struct Actor *actor = Actor_Get(actorId);
  if (actor == NULL || FIELD_CELL(actor->x.fixed) != column) {
    return 0;
  }
  if (FIELD_CELL(actor->z.fixed) != row) {
    return 0;
  }
  return 1;
}

/// Configures the northwest seal animation and statue return position.
static void SolSanctumStatue_OpenNorthwestSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  CELL_COPY_SET(seal->open, 0, 55, 32, 40, 4, 3);
  CELL_COPY_SET(seal->pulseA, 2, 30, 34, 10, 2, 1);
  CELL_COPY_SET(seal->pulseB, 2, 28, 34, 10, 2, 1);
  CELL_COPY_SET(seal->opened, 2, 30, 16, 10, 2, 1);
  seal->flag = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
  seal->facing = FACING_SOUTH;
  seal->destinationX = 500;
  seal->destinationZ = 132;
  CELL_COPY_SET(seal->closed[0], 8, 55, 32, 40, 4, 3);
  CELL_COPY_SET(seal->closed[1], 2, 30, 34, 10, 2, 1);
  CELL_COPY_SET(seal->closed[2], 2, 28, 16, 10, 2, 1);
  seal->actorId = ACTOR_NORTHWEST_STATUE;
  seal->actorX = 488;
  seal->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the northeast seal animation and statue return position.
static void SolSanctumStatue_OpenNortheastSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  CELL_COPY_SET(seal->open, 4, 55, 36, 40, 4, 3);
  CELL_COPY_SET(seal->pulseA, 4, 30, 36, 10, 2, 1);
  CELL_COPY_SET(seal->pulseB, 4, 28, 36, 10, 2, 1);
  CELL_COPY_SET(seal->opened, 4, 30, 18, 10, 2, 1);
  seal->flag = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
  seal->facing = FACING_SOUTH;
  seal->destinationX = 654;
  seal->destinationZ = 132;
  CELL_COPY_SET(seal->closed[0], 12, 55, 36, 40, 4, 3);
  CELL_COPY_SET(seal->closed[1], 4, 30, 36, 10, 2, 1);
  CELL_COPY_SET(seal->closed[2], 4, 28, 18, 10, 2, 1);
  seal->actorId = ACTOR_NORTHEAST_STATUE;
  seal->actorX = 664;
  seal->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southwest seal animation and statue return position.
static void SolSanctumStatue_OpenSouthwestSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  CELL_COPY_SET(seal->open, 0, 58, 32, 43, 4, 1);
  CELL_COPY_SET(seal->pulseA, 2, 31, 34, 11, 2, 1);
  CELL_COPY_SET(seal->pulseB, 2, 29, 34, 11, 2, 1);
  CELL_COPY_SET(seal->opened, 2, 31, 16, 11, 2, 1);
  seal->flag = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
  seal->facing = FACING_NORTH;
  seal->destinationX = 500;
  seal->destinationZ = 216;
  CELL_COPY_SET(seal->closed[0], 8, 58, 32, 43, 4, 1);
  CELL_COPY_SET(seal->closed[1], 2, 31, 34, 11, 2, 1);
  CELL_COPY_SET(seal->closed[2], 2, 29, 16, 11, 2, 1);
  seal->actorId = ACTOR_SOUTHWEST_STATUE;
  seal->actorX = 488;
  seal->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southeast seal animation and statue return position.
static void SolSanctumStatue_OpenSoutheastSeal(void) {
  struct SealOpening *seal = &sSealOpening;
  CELL_COPY_SET(seal->open, 4, 58, 36, 43, 4, 1);
  CELL_COPY_SET(seal->pulseA, 4, 31, 36, 11, 2, 1);
  CELL_COPY_SET(seal->pulseB, 4, 29, 36, 11, 2, 1);
  CELL_COPY_SET(seal->opened, 4, 31, 18, 11, 2, 1);
  seal->flag = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
  seal->facing = FACING_NORTH;
  seal->destinationX = 654;
  seal->destinationZ = 216;
  CELL_COPY_SET(seal->closed[0], 12, 58, 36, 43, 4, 1);
  CELL_COPY_SET(seal->closed[1], 4, 31, 36, 11, 2, 1);
  CELL_COPY_SET(seal->closed[2], 4, 29, 18, 11, 2, 1);
  seal->actorId = ACTOR_SOUTHEAST_STATUE;
  seal->actorX = 664;
  seal->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

#define COPY_CELLS(copy)                                                       \
  MapGrid_CopyCellsTo((copy).sourceX, (copy).sourceY, (copy).destinationX,     \
                      (copy).destinationY, (copy).width, (copy).height)

/// Animates the seal and either advances the story or resets its statue.
static void SolSanctumStatue_OpenSeal(void) {
  struct SealOpening *seal;
  s32 exitResult;
  s32 pulse;

  exitResult = 0;
  Event_Start();
  if (!GameFlag_Get(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(576, CAMERA_KEEP, 172, 1);
    FieldCamera_WaitForMove();
    Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
    COPY_CELLS(sSealOpening.open);
    for (pulse = 0; pulse != 20; pulse++) {
      Sound_Play(SE_GLITTER);
      COPY_CELLS(sSealOpening.pulseA);
      Event_Wait(4);
      Sound_Play(SE_GLITTER);
      COPY_CELLS(sSealOpening.pulseB);
      Event_Wait(4);
    }
    seal = &sSealOpening;
    COPY_CELLS(seal->opened);
    GameFlag_Set(seal->flag);
    exitResult = SolSanctumStatue_SelectStoryExit();
    if (exitResult == -1) {
      if (!GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
        FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
        Actor_Get(ACTOR_PARTY_LEADER)->facing = seal->facing;
        Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
        Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
        Actor_SetDestination(ACTOR_PARTY_LEADER, seal->destinationX,
                             seal->destinationZ);
        COPY_CELLS(seal->closed[0]);
        COPY_CELLS(seal->closed[1]);
        COPY_CELLS(seal->closed[2]);
        Actor_Get(seal->actorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_MoveToAndWait(seal->actorId, seal->actorX, seal->actorZ);
        Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
        GameFlag_Clear(seal->flag);
      }
    } else if (exitResult == 0 &&
               GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
      // Unused: SolSanctumStatue_SelectStoryExit returns only 1 or -1, so
      // this branch cannot run.
      if (GameFlag_Get(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) &&
          GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) &&
          GameFlag_Get(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN)) {
        if (!Event_GetFlag(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
          GameFlag_Set(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
          SolSanctumStatue_UnusedPutOutSeals();
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
static void SolSanctumStatue_ShineLeftBeam(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveTo(286, CAMERA_KEEP, 146, 1);
      FieldCamera_WaitForMove();
      Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(0, 59, 15, 38, 4, 3);
      // The cells SolSanctumStatue_ShineRightBeam copies as it sets the flag,
      // redrawn on arrival.
      if (Event_GetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
      Event_Wait(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
      if (GameFlag_Get(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Event_End();
}

/// Draws the right beam and opens the floor when the left beam is also lit.
static void SolSanctumStatue_ShineRightBeam(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0) {
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveTo(286, CAMERA_KEEP, 146, 1);
      FieldCamera_WaitForMove();
      Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
      MapGrid_CopyCellsTo(4, 59, 17, 38, 4, 3);
      // The cells SolSanctumStatue_ShineLeftBeam copies as it sets the flag,
      // redrawn on arrival.
      if (Event_GetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        MapGrid_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
      Event_Wait(30);
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
  Event_ShowMessage(speaker, 0);
  Event_Wait(frames);
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
struct ActorPlacement gSolSanctumStatue_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        488, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        488, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 216, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        288, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        560, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        592, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gSolSanctumStatue_Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH,
     CONDITION_ALWAYS, EVENT(SolSanctumStatue_JumpNorth)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH,
     CONDITION_ALWAYS, EVENT(SolSanctumStatue_JumpSouth)},
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
     CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushNorthwestStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushNortheastStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT(SolSanctumStatue_PushSouthwestStatue)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE,
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
     CELL_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT(SolSanctumStatue_PushCenterStatueNorth)},
    {TRIGGER_TABLE_END},
};

s32 gSolSanctumStatue_ScrollPhase = 0;
