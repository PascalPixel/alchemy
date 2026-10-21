/// @file
/// Sol Sanctum (scene 12, a row of the scene table with no symbol), the room
/// of what Kraden calls the statue puzzle.
///
/// Isaac, Garet and Jenna have come into the sanctum with Kraden, the sage
/// who teaches in Vale, looking for the hidden passage that two strangers,
/// Saturos and Menardi, spoke of. Kraden has found the Luna and Sol rooms
/// (scene 11) and waits there, sending the party to look farther up the
/// passage; this room opens off them (its exit 1, their entrance 2), and
/// SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE leads into a room with a
/// Psynergy Stone (scene 13).
///
/// Eight statues stand here around a ninth in the centre. Four can be pushed,
/// or moved with Move, onto seals in the floor; two others, pushed in front
/// of the centre statue, shine a beam at it; the last two never move. Each
/// statue answers both ways: walking into it pushes it a cell
/// (SolSanctumStatue_PushStatue), and Move ends in a check of where it
/// stands. Every move's event row carries the flag of what it achieves (a
/// seal open, a beam shining, the centre statue moved), so a statue stops
/// answering once it has done its part; SolSanctumStatue_Initialize redraws
/// the room from those story flags on every entry, statues in their places
/// and seals and beams drawn in.
///
/// The puzzle is a trap, and the room's logic follows it in story order:
/// - Until the centre statue has moved (FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED),
///   the scene cuts away when a seal first opens (SolSanctumStatue_OpenSeal):
///   SolSanctumStatue_SelectStoryExit takes exit 4 to the Luna and Sol rooms,
///   where the quarter of the pictures the seal lights flashes and lights
///   fall through the Luna room around Kraden
///   (FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) and he cries `Stop!!! You're
///   about to make a big mistake!` He runs here after the party (entrance 4)
///   to warn that moving one statue springs the sanctum's trap, set by
///   someone who hid something very important here, perhaps the key to
///   Alchemy, and that the light must be the key to the secret
///   (SolSanctumStatue_ExplainStatuePuzzle, once,
///   FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED). After that a seal opened too
///   soon closes again: its statue slides back and the leader jumps clear.
/// - The beams are the way through. With both shining, the floor before the
///   centre statue opens (SolSanctumStatue_OpenCentralHole), and the centre
///   statue, moved south with Move, slides into the hole and the room shakes:
///   `Something clicked!` (Event_CenterStatueMoved).
/// - From then on the seals stay open. The first one opened cuts away by exit
///   3: in the Luna and Sol rooms the quarter it lights only flashes, and
///   Kraden runs off to see what Isaac has found
///   (FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG), and he comes here (entrance 3) to
///   find the hole. Garet and Jenna tell him how it opened; he decides that
///   the statue puzzle was the trap and the centre statue there to disarm it,
///   and goes back to watch from the Luna room
///   (SolSanctumStatue_ExplainTrapDisarmed, once,
///   FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED). Once all four seals are open,
///   the picture of Luna in the Luna and Sol rooms has changed to Sol, and
///   the way on is found there.
///
/// Both of Kraden's visits are scripted scenes that hold Jenna and Garet in
/// place beside the leader, then walk them back into the party; any other
/// arrival hides them. Retreat does not work here, a callback draws three
/// rows of sprites that drift sideways at different speeds
/// (SolSanctumStatue_UpdateScrollingSprites), and once Mt. Aleph has erupted
/// (FLAG_SOL_SANCTUM_ERUPTED) the room rumbles and shakes gently, as the rest
/// of the sanctum does.

#include "game_state.h"
#include "version.h"
#include "unit/scene/sol_sanctum_statue.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "map_scroll.h"
#include "fixed_math.h"
#include "resource_37de_scene.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "ability_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED 0x30a
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED 0x30b

#if TBS_PATCH_3
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 6
#else
#define SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET 8
#endif

#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH 32

// Scene flags, cleared on every ordinary entrance:
// - FLAG_SCENE_TEMP_07: both beams have opened the hole in the floor;
//   until then, Move on the centre statue first walks the leader clear of it.

static void SolSanctumStatue_InitializeScrollingSprites(void);
static void Event_CenterStatueMoved(void);
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
static void Event_SayThenWait(s32 speaker, s32 frames);

#define NULL ((void *)0)

/// A block of map cells copied from one place in the map to another.
struct CellCopy {
  s32 srcX;   ///< map cells
  s32 srcY;   ///< map cells
  s32 destX;  ///< map cells
  s32 destY;  ///< map cells
  s32 width;  ///< map cells
  s32 height; ///< map cells
};

/// The seal being opened: its cell animations, its flag, where the leader
/// jumps back to and where its statue returns if the story does not go on.
struct SealScene {
  struct CellCopy open;
  struct CellCopy pulseA;
  struct CellCopy pulseB;
  struct CellCopy opened;
  s32 flag;
  s32 facing; ///< 1/65536 turns
  s32 destX;  ///< pixels
  s32 destZ;  ///< pixels
  struct CellCopy stairs[3];
  s32 actor;
  s32 actorX;    ///< pixels
  s32 actorZ;    ///< pixels
  s32 unused[3]; ///< never read or written; the bss keeps its 12 bytes
};

STATIC_ASSERT(offsetof(struct SealScene, unused) == 0xc4);

/// The placed actors, numbered in the order of gSolSanctumStatue_Placements:
/// Kraden, who stays out of sight until his two visits, the eight statues, and
/// the centre statue in its two places; Jenna and Garet, placed after them,
/// keep their own slots. Of the statues, four are pushed onto seals and two
/// shine beams at the centre; the other two never move.
enum SolSanctumStatueActor {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_NORTHWEST_STATUE,
  ACTOR_LEFT_BEAM_STATUE,
  ACTOR_NORTHEAST_STATUE,
  ACTOR_RIGHT_BEAM_STATUE,
  ACTOR_SOUTHWEST_STATUE,
  /// South of the left beam statue; no event moves it.
  ACTOR_LEFT_STILL_STATUE,
  ACTOR_SOUTHEAST_STATUE,
  /// South of the right beam statue; no event moves it.
  ACTOR_RIGHT_STILL_STATUE,
  ACTOR_CENTER_STATUE,
  /// Stands in the hole in the floor once the centre statue has dropped in.
  ACTOR_MOVED_CENTER_STATUE
};

/// The scene's bss, in this order; as commons of one object the linker would
/// sort them itself.
static struct SealScene sSealScene;
/// Initialization writes each three-word OAM packet; drawing updates its
/// fields.
static union {
  struct AnimationSpritePart parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
  s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

const struct SceneEntrance *SolSanctumStatue_GetEntrances(void) {
  return gSolSanctumStatue_Entrances;
}

const struct SceneRegion *SolSanctumStatue_GetRegions(void) { return NULL; }

const u32 *SolSanctumStatue_GetExits(void) { return gSolSanctumStatue_Exits; }

/// Returns Kraden, the eight statues, the centre statue and its moved copy
/// (ACTOR_KRADEN to ACTOR_MOVED_CENTER_STATUE), then Jenna and Garet.
const struct ScenePlacement *SolSanctumStatue_GetPlacements(void) {
  return gSolSanctumStatue_Placements;
}

/// Returns the statues' Move and push events, each row only until its seal or
/// beam is done.
const struct SceneEvent *SolSanctumStatue_GetEvents(void) {
  return gSolSanctumStatue_Events;
}

/// Chooses the scripted scene in the Luna and Sol rooms that a newly opened
/// seal leads to, and requests its exit: once the centre statue has moved,
/// the quarter it lit flashing in the Luna and Sol rooms (exit 3), until
/// FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG is set; before then, the falling
/// lights (exit 4), until FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set.
/// Returns 1 when it requests an exit, otherwise -1.
static s32 SolSanctumStatue_SelectStoryExit(void) {
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG) == 0) {
      Engine_EventRequestExit(3);
      return 1;
    }
  } else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
    Engine_EventRequestExit(4);
    return 1;
  }
  return -1;
}

/// Animates the floor opening before the centre statue once both beams shine,
/// then lets the statue drop in if Move has already brought it there. Called
/// by whichever beam shines second. Sets FLAG_SCENE_TEMP_07.
static void SolSanctumStatue_OpenCentralHole(void) {
  s32 flash;

  {
    Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
  }
  Engine_AudioPlayCue(SE_200);
  for (flash = 0; flash != 22; flash++) {
    Map_CopyCellsTo(10, 61, 17, 40, 2, 1);
    Engine_EventWait(4);
    Map_CopyCellsTo(8, 61, 17, 40, 2, 1);
    Engine_EventWait(4);
  }
  {
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
  }
  Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
  {
    Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_07);
  Event_CenterStatueMoved();
}

/// Kraden's second visit, a scripted scene: the first seal opened after the
/// centre statue dropped has set the quarter it lit flashing in the Luna and
/// Sol rooms, and Kraden comes to find the hole in the floor. "You found
/// it, Isaac!"
/// Garet and Jenna tell him how it opened; he decides the statue puzzle was
/// the trap and the centre statue there to disarm it, and leaves to observe
/// from the Luna room. Called by SolSanctumStatue_Initialize at entrance 3
/// until FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED is set, which the caller
/// sets. Clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumStatue_ExplainTrapDisarmed(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(232));
  Engine_EventWait(1);
  // Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!"
  Engine_EventSetMessage(MsgSolSanctumKradenYouFoundIt);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(280));
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Engine_EventWait(20);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  // The camera swings west to the hole at the centre and back.
  Camera_SetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(10);
  // Kraden hops up to the party: "Excellent work, Isaac!"
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Engine_EventWait(40);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Engine_EventWait(40);
  // The screen closes and opens on Kraden by the hole: "Where did that hole
  // come from?" Then back to the party.
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(424), PIXELS(200));
  Engine_TaskWait(1);
  Event_SayThenWait(ACTOR_KRADEN, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(184));
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(157), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // Garet: two beams shot out of the statues Isaac moved, and the hole
  // appeared. Jenna: Isaac dropped the statue into it.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_SayThenWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(40);
  // Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_KRADEN, 80);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(ACTOR_KRADEN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  // Kraden: the other statue was there to disarm the trap, and now they
  // should be all right.
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // Kraden: he will observe from the Luna room. He leaves south.
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 217);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 321);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Camera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(136), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  // Rejoin the companions to the leader and restore normal field control.
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(JENNA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(JENNA);
  Actor_SetPosition(JENNA, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gEventWork->transitionFrames = 16;
  Engine_EventEnd();
}

/// Kraden's first visit, a scripted scene: the party's first seal has sent
/// lights falling through the Luna room around him, and he runs
/// here to warn that the mechanism is a trap. "Wait, Isaac!" Moving one statue
/// springs it; whoever set it hid something very important, perhaps the key
/// to Alchemy itself, and his intuition says the light is the key to the
/// secret. He goes back to the Luna room and Jenna sets the party to work it
/// out. Called by SolSanctumStatue_Initialize at entrance 4 until
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED is set, which the caller sets.
/// Clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumStatue_ExplainStatuePuzzle(void) {
  struct EventWork *work;
  struct FieldActor *leader;

  Engine_EventBegin();
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ActorSetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(296));
  Engine_EventWait(1);
  // Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?"
  Engine_EventSetMessage(MsgSolSanctumWait);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Engine_EventWait(20);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_EventWait(30);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(6);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_WEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Engine_CameraSetSpeed(PIXELS(28) / 5, PIXELS(7) / 10);
  Camera_MoveTo(PIXELS(287), -1, PIXELS(176), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
  Engine_EventWait(40);
  // Kraden: "This is a most interesting mechanism you've found, Isaac."
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  // Kraden: "But this is clearly... some kind of trap." All three start.
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_ActorJump(GARET, 2, 0);
  Engine_ActorJump(JENNA, 2, 0);
  Engine_EventWait(30);
  // Garet: "This is...a trap?" Kraden: "Go look in the Luna room".
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_SayThenWait(GARET, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorFaceActor(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_KRADEN, JENNA, 0);
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorWalkTo(ACTOR_KRADEN, 432, 200);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(288), -1, PIXELS(171), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(80);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(180), 1);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(574), -1, PIXELS(171), 1);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3), PIXELS(2));
  Engine_ActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
  Engine_EventWait(80);
  // Kraden: they are looking in the right place, but moving one statue
  // springs the trap. Jenna: why would anyone put a trap here?
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_SayThenWait(ACTOR_KRADEN, 20);
  Engine_ActorShowEmote(JENNA, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_SayThenWait(JENNA, 6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  // Kraden: they have hidden something very important, perhaps the key to
  // Alchemy itself.
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(JENNA, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(40);
  // Kraden: "Should I tell you more?" Either answer, he thinks they would not
  // understand yet.
  UiText_OpenMessageAtObject(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumHonestlyDoubtUnderstand);
  } else {
    Engine_EventSetMessage(MsgSolSanctumHmphWellTold);
  }
  Event_SayThenWait(ACTOR_KRADEN, 6);
  // Kraden: "We must try to find a solution to this mystery." Garet:
  // "Sheesh... Another mystery?" Kraden will keep checking the Luna room.
  Engine_EventSetMessage(MsgSolSanctumTryFindSolution);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_SayThenWait(GARET, 6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Engine_ActorJump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 216);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(191), 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkToAndWait(ACTOR_KRADEN, 576, 232);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(40);
  // Kraden, leaving: his intuition says the light is the key to the secret.
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Event_SayThenWait(ACTOR_KRADEN, 6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Camera_MoveTo(PIXELS(576), -1, PIXELS(215), 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 574, 323);
  Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
  // Leave Isaac in control after Jenna and Garet rejoin him.
  Camera_SetSpeed(PIXELS(18) / 5, PIXELS(9) / 20);
  Engine_CameraMoveTo(PIXELS(576), -1, PIXELS(136), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorFaceDirection(JENNA, FACING_EAST, 0);
  Engine_EventWait(10);
  // Jenna: "let's see if we can't figure this out ourselves."
  Event_SayThenWait(JENNA, 6);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(JENNA, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(JENNA);
  Engine_ActorSetPosition(JENNA, 0, 0);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetPosition(GARET, 0, 0);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  Engine_EventEnd();
}

/// Pans the camera away from the leader while a patch of the floor flashes
/// ever faster, redraws a larger block of the floor, then restores the camera.
/// Called only from the branch of SolSanctumStatue_OpenSeal that is marked
/// DEAD.
static void SolSanctumStatue_RunSealOpenedSequence(void) {
  struct MapScrollWork *map;
  struct FieldActor *leader;
  s32 *savedOrigin;
  s32 cameraPosition[3];
  s32 approachSide;
  s32 frame;

  map = gMapWork[0];
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Temporarily follow a local point, panning from the leader toward the seal.
  if (leader->z.fixed < PIXELS(179)) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 575, 132);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventWait(30);
    savedOrigin = map->origin;
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    map->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    approachSide = 1;
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 577, 222);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_EventWait(30);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = map->origin;
    map->origin = cameraPosition;
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Engine_EventWait(1);
    }
    Engine_EventWait(40);
    approachSide = 2;
  }
  // Flash progressively faster before uncovering the stairway.
  for (frame = 0; frame != 6; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(8);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(8);
  }
  for (frame = 0; frame != 10; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(4);
  }
  for (frame = 0; frame != 12; frame++) {
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_EventWait(2);
    Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
    Engine_EventWait(2);
  }
  Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
  Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
  Engine_EventWait(60);
  // Reverse the pan before restoring the caller's camera target.
  if (approachSide == 1) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] -= PIXELS(1);
      Engine_EventWait(1);
    }
  } else if (approachSide == 2) {
    for (frame = 0; frame != 30; frame++) {
      cameraPosition[2] += PIXELS(1);
      Engine_EventWait(1);
    }
  }
  map->origin = savedOrigin;
}

/// Draws the three parallax rows while they overlap the screen. Run each frame
/// as a callback that SolSanctumStatue_InitializeScrollingSprites adds.
static void SolSanctumStatue_UpdateScrollingSprites(void) {
  struct MapScrollWork *map = gMapWork[0];
  struct AnimationSpritePart *sprite = sScrollingSprites.parts;
#ifdef UBFIX
  s32 cameraX = map->viewX / PIXELS(1);
  s32 screenY = 80 - map->viewY / PIXELS(1);
#else
  // UB: viewX and viewY are separate scalar members, not an array. Reading
  // them by name adds four bytes in tbs-en.
  s32 *view = &map->viewX;
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
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 9) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY;
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
    scrollX = (gSolSanctumStatue_ScrollPhase >> 8) - cameraX;
    scrollX |= -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
    for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
      sprite->x = scrollX;
      sprite->y = screenY + SOL_SANCTUM_STATUE_BACK_SCROLL_ROW_Y_OFFSET;
      Runtime_PushSlotEntry(sprite, 0);
      scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
      sprite++;
    }
  }
  // The three shifts turn this into 1/8, 1/4 and 1/2 pixel drift per frame.
  gSolSanctumStatue_ScrollPhase += 128;
}

/// Walking north over cell code 10: the way ahead is checked as on any field.
static void Event_CheckAheadFacingNorth(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_NORTH) {
    Engine_LeaderCheckAhead();
  }
}

/// Walking south over cell code 11: the way ahead is checked as on any field.
static void Event_CheckAheadFacingSouth(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->facing == FACING_SOUTH) {
    Engine_LeaderCheckAhead();
  }
}

/// Unused: the northwest statue slides by itself to (504, 152), where it
/// opens its seal, unless that seal is already open.
///
/// Nothing calls it; the player moves the statue there with Move instead.
UNUSED static void SolSanctumStatue_UnusedMoveNorthwestStatue(void) {
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0) {
    Engine_EventBegin();
    Object_GetById(ACTOR_NORTHWEST_STATUE);
    Actor_SetSpeed(ACTOR_NORTHWEST_STATUE, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_WalkToAndWait(ACTOR_NORTHWEST_STATUE, 504, 152);
    Engine_EventEnd();
  }
}

/// Restores the room's statues, beams and seals from their flags and plays
/// Kraden's two visits. Called by Field_RunScene once the actors exist.
/// Sets FLAG_RETREAT_BLOCKED; starts the parallax rows; at entrance 3 plays
/// SolSanctumStatue_ExplainTrapDisarmed and at entrance 4
/// SolSanctumStatue_ExplainStatuePuzzle, each once (setting
/// FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED or
/// FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED), and not on a return after a
/// battle.
s32 SolSanctumStatue_Initialize(void) {
  struct FieldActor *centerEffect;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  SolSanctumStatue_InitializeScrollingSprites();
  // Retreat does not work here (flag 0x144).
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  centerEffect = Object_GetById(ACTOR_MOVED_CENTER_STATUE);
  centerEffect->collisionFlags = 0;
  {
    u8 flags = centerEffect->priorityFlags | 2;

    centerEffect->priorityFlags = flags;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_MOVED_CENTER_STATUE), 0);
  Object_GetById(ACTOR_MOVED_CENTER_STATUE)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_MOVED_CENTER_STATUE, 1);
  // Only arrivals 3 and 4 keep Jenna and Garet staged for their conversations.
  if ((u16)(gGameState.entrance - 3) > 1) {
    Actor_SetPosition(JENNA, 0, 0);
    Actor_SetPosition(GARET, 0, 0);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
    Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, PIXELS(288), PIXELS(178));
    Actor_SetPosition(ACTOR_CENTER_STATUE, PIXELS(1608), PIXELS(1608));
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    Map_CopyCellAttributes(0, 1, 2, 1, 17, 7);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(0, 28, 17, 8, 2, 1);
    Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
    Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
    Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    Map_CopyCellAttributes(0, 0, 2, 1, 17, 8);
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_LEFT_BEAM_STATUE, PIXELS(232), PIXELS(120));
      Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
    }
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
      Actor_SetPosition(ACTOR_RIGHT_BEAM_STATUE, PIXELS(344), PIXELS(120));
      Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHWEST_STATUE, PIXELS(504), PIXELS(152));
    Map_CopyCellsTo(2, 28, 34, 10, 2, 1);
    Map_CopyCellsTo(2, 30, 16, 10, 2, 1);
    Map_CopyCellsTo(0, 55, 32, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_NORTHEAST_STATUE, PIXELS(648), PIXELS(152));
    Map_CopyCellsTo(4, 28, 36, 10, 2, 1);
    Map_CopyCellsTo(4, 30, 18, 10, 2, 1);
    Map_CopyCellsTo(4, 55, 36, 40, 4, 3);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHWEST_STATUE, PIXELS(504), PIXELS(200));
    Map_CopyCellsTo(2, 29, 34, 11, 2, 1);
    Map_CopyCellsTo(2, 31, 16, 11, 2, 1);
    Map_CopyCellsTo(0, 58, 32, 43, 4, 1);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SOUTHEAST_STATUE, PIXELS(648), PIXELS(200));
    Map_CopyCellsTo(4, 29, 36, 11, 2, 1);
    Map_CopyCellsTo(4, 31, 18, 11, 2, 1);
    Map_CopyCellsTo(4, 58, 36, 43, 4, 1);
  }
  if (gGameState.entrance == 3) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0) {
      Actor_SetPosition(GARET, 0, 0);
      Actor_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainTrapDisarmed();
      GameFlag_Set(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
    }
  }
  if (gGameState.entrance == 4) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED) != 0) {
      Actor_SetPosition(GARET, 0, 0);
      Actor_SetPosition(JENNA, 0, 0);
    } else if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      SolSanctumStatue_ExplainStatuePuzzle();
      GameFlag_Set(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED);
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    BattleFx_SetQueuedSoundAndPlay(SE_141);
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    InitializeSceneRecordBuffer();
  }
  return 0;
}

/// Loads three graphic strips and initializes their scrolling sprite rows.
static void SolSanctumStatue_InitializeScrollingSprites(void) {
  u32 column;
  s32 tile;
  s32 *spriteWords;
  u8 *pixels;

  spriteWords = sScrollingSprites.words;
  pixels = Runtime_AllocateBlock(14, 1024);
  Resource_DecodeByteLz(gSolSanctumStatue_SpriteRowsGfx, pixels);
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xac00;
  }
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels + 128);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xdc00;
  }
  tile = Engine_VramLoad(Resource_FindFreeEntry(), 128, pixels + 256);
  for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
    s32 *attributeWords = spriteWords;

    *attributeWords++ = 0;
    *attributeWords++ = 0x40004000;
    spriteWords += 3;
    *attributeWords = tile | 0xc00;
  }
  Engine_HeapRelease(14);
  {
    s32 priority = 3200;

    FieldScheduler_AddCallback(SolSanctumStatue_UpdateScrollingSprites,
                               priority);
  }
}

/// Moving the northwest statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_NORTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void Event_NorthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHWEST_STATUE, 31, 9) != 0) {
    SolSanctumStatue_OpenNorthwestSeal();
  }
}

/// Moving the northeast statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_NORTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void Event_NortheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_NORTHEAST_STATUE, 40, 9) != 0) {
    SolSanctumStatue_OpenNortheastSeal();
  }
}

/// Moving the southwest statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_SOUTHWEST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void Event_SouthwestStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHWEST_STATUE, 31, 12) != 0) {
    SolSanctumStatue_OpenSouthwestSeal();
  }
}

/// Moving the southeast statue with Move: on its seal, the seal opens.
/// Reached by Move on ACTOR_SOUTHEAST_STATUE until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void Event_SoutheastStatueMoved(void) {
  if (SolSanctumStatue_IsActorAtCell(ACTOR_SOUTHEAST_STATUE, 40, 12) != 0) {
    SolSanctumStatue_OpenSoutheastSeal();
  }
}

/// Moving the left beam statue with Move: its cell is freed, and in front of
/// the centre it shines its beam. Reached by Move on ACTOR_LEFT_BEAM_STATUE
/// until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void Event_LeftBeamStatueMoved(void) {
  Field_SetMapCellCollision(2, PIXELS(208), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_LEFT_BEAM_STATUE, 14, 7) != 0) {
    SolSanctumStatue_ShineLeftBeam();
  }
}

/// Moving the right beam statue with Move: as the left one. Reached by Move on
/// ACTOR_RIGHT_BEAM_STATUE until FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void Event_RightBeamStatueMoved(void) {
  Field_SetMapCellCollision(2, PIXELS(352), PIXELS(112), 0);
  if (SolSanctumStatue_IsActorAtCell(ACTOR_RIGHT_BEAM_STATUE, 21, 7) != 0) {
    SolSanctumStatue_ShineRightBeam();
  }
}

/// Starting Move beside the centre statue: its cells are blocked while the
/// leader stands in their row. Reached by the first phase of Move on
/// ACTOR_CENTER_STATUE until FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void Event_BlockCenterTriggerCells(void) {
  struct FieldActor *leaderPosition;
  s32 column;
  struct FieldActor *leader;

  leaderPosition = Object_GetById(ACTOR_PARTY_LEADER);
  column = leaderPosition->x.fixed >> 20;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((leader->z.fixed >> 20) == 8) {
    if ((u32)(column - 17) <= 1) {
      Field_SetMapCellCollision(2, PIXELS(272), PIXELS(128), 255);
      Field_SetMapCellCollision(2, PIXELS(288), PIXELS(128), 255);
    }
  }
}

/// Starting Move beside the left beam statue: its cell is blocked while the
/// leader stands beside it. Reached by the first phase of Move on
/// ACTOR_LEFT_BEAM_STATUE until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void Event_BlockLeftTriggerCell(void) {
  s32 column = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 13) <= 1) {
    Field_SetMapCellCollision(2, PIXELS(208), PIXELS(112), 255);
  }
}

/// Starting Move beside the right beam statue: as the left one, on
/// ACTOR_RIGHT_BEAM_STATUE.
static void Event_BlockRightTriggerCell(void) {
  s32 column = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20;
  s32 row = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20;

  if (row == 7 && (u32)(column - 21) <= 1) {
    Field_SetMapCellCollision(2, PIXELS(352), PIXELS(112), 255);
  }
}

/// Moving the centre statue with Move, or a beam completing the pair: with
/// both beams shining the statue slides into the opened floor and the room
/// shakes. "Something clicked!" Reached by Move on ACTOR_CENTER_STATUE until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set, and called by
/// SolSanctumStatue_OpenCentralHole. Sets FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
/// and FLAG_SOL_SANCTUM_ALL_SEALS_OPEN when the four seals are open;
/// ACTOR_MOVED_CENTER_STATUE takes the statue's place in the hole.
static void Event_CenterStatueMoved(void) {
  struct FieldActor *statue;
  struct FieldActor *leader;
  s32 statueZ;

  statue = Object_GetById(ACTOR_CENTER_STATUE);
  Field_SetMapCellCollision(2, PIXELS(272), PIXELS(128), 0);
  Field_SetMapCellCollision(2, PIXELS(288), PIXELS(128), 0);
  if (statue != NULL) {
    statueZ = statue->z.fixed;
    Engine_EventBegin();
    if ((statueZ >> 20) == 8) {
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_07) == 0) {
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if ((u32)(leader->z.fixed >> 19) <= 17) {
          Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 289, 158);
          leader = Object_GetById(ACTOR_PARTY_LEADER);
          leader->facing = FACING_NORTH;
        }
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
          Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        Engine_GameFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
        // The statue slides south into the hole; ACTOR_MOVED_CENTER_STATUE
        // stands in for it there and the room shakes hard, then settles.
        Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
        Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
        Engine_CameraWaitForMove();
        Object_GetById(ACTOR_CENTER_STATUE)->steeringFlags &=
            ~ACTOR_FACES_MOTION;
        Actor_SetSpeed(ACTOR_CENTER_STATUE, PIXELS(3), PIXELS(1));
        statue->motionFlags = 0;
        Engine_ActorSetSpritePriority(ACTOR_CENTER_STATUE, 3);
        Engine_AudioPlayCue(SE_189);
        Engine_ActorSetDestination(ACTOR_CENTER_STATUE, 288, 178);
        Engine_EventWait(8);
        Actor_SetPosition(ACTOR_MOVED_CENTER_STATUE, PIXELS(288), PIXELS(178));
        statue->targetX = ACTOR_NO_TARGET;
        statue->targetY = ACTOR_NO_TARGET;
        statue->targetZ = ACTOR_NO_TARGET;
        statue->x.fixed = 0;
        statue->y.fixed = 0;
        statue->z.fixed = 0;
        SET_VELOCITY(statue, 0, 0, 0);
        Engine_ActorSetPosition(ACTOR_CENTER_STATUE, 0, 0);
        Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Engine_EventWait(10);
        Engine_AudioPlayCue(SE_141);
        Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Engine_EventWait(10);
        Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
        Engine_EventWait(35);
        Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
        Engine_EventWait(20);
        Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
        Engine_EventWait(30);
        Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
        Engine_EventWait(40);
        Engine_AudioPlayCue(SE_289);
        Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
        Engine_EventWait(10);
        Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
        Engine_EventWait(60);
        Engine_AudioPlayCue(SE_188);
        if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
            Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
          Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
        }
        Engine_EventWait(40);
        Engine_MessageShowCentered(MsgSolSanctumSomethingClicked, 1);
        Map_CopyCellAttributes(0, 1, 2, 1, 17, 8);
        Map_CopyCellAttributes(17, 9, 2, 1, 17, 7);
      }
    }
    Engine_EventEnd();
  }
}

/// Walking into the centre statue from the south: the leader pushes it north.
/// Reached by walking north into cell code 32 until
/// FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED is set.
static void Event_PushCenterStatueNorth(void) {
  struct FieldActor *movingActor;

  movingActor = Object_GetById(ACTOR_CENTER_STATUE);
  if (movingActor != 0) {
    if ((movingActor->z.fixed >> 20) == 8) {
      Engine_EventBegin();
      Event_PlayCue(SE_185);
      Actor_SetSpeed(ACTOR_CENTER_STATUE, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      Object_GetById(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
      movingActor = Object_GetById(ACTOR_PARTY_LEADER);
      Actor_SetDestination(ACTOR_PARTY_LEADER, movingActor->x.part.pixel, 136);
      Actor_SetDestination(ACTOR_CENTER_STATUE, 288, 120);
      Engine_ActorWaitForMove(ACTOR_CENTER_STATUE);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventEnd();
    }
  }
}

/// Walking into the northwest statue: the leader pushes it onto its seal.
/// Reached by walking east into cell code 5 until
/// FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN is set.
static void Event_PushNorthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHWEST_STATUE, 31, 9, 30, 9);
  SolSanctumStatue_OpenNorthwestSeal();
}

/// Walking into the northeast statue: the leader pushes it onto its seal.
/// Reached by walking west into cell code 6 until
/// FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN is set.
static void Event_PushNortheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_NORTHEAST_STATUE, 40, 9, 41, 9);
  SolSanctumStatue_OpenNortheastSeal();
}

/// Walking into the southwest statue: the leader pushes it onto its seal.
/// Reached by walking east into cell code 7 until
/// FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN is set.
static void Event_PushSouthwestStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHWEST_STATUE, 31, 12, 30, 12);
  SolSanctumStatue_OpenSouthwestSeal();
}

/// Walking into the southeast statue: the leader pushes it onto its seal.
/// Reached by walking west into cell code 8 until
/// FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN is set.
static void Event_PushSoutheastStatue(void) {
  SolSanctumStatue_PushStatue(ACTOR_SOUTHEAST_STATUE, 40, 12, 41, 12);
  SolSanctumStatue_OpenSoutheastSeal();
}

/// Walking into the left beam statue from the west: it slides east in front
/// of the centre and shines its beam. Reached by walking east into cell codes
/// 20 and 21 until FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING is set.
static void Event_PushLeftBeamStatueEast(void) {
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 14, 7, 13, 7);
  SolSanctumStatue_ShineLeftBeam();
}

/// Walking into the left beam statue from the north: it slides one cell south.
/// Reached by walking south into cell code 21 until the beam shines.
static void Event_PushLeftBeamStatueSouth(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_LEFT_BEAM_STATUE);
  s32 row;

  if (statue == 0) {
    return;
  }

  row = statue->z.fixed >> 20;
  SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

/// Walking into the left beam statue from the south: it slides one cell north.
/// Reached by walking north into cell codes 23 and 24 until the beam shines.
static void Event_PushLeftBeamStatueNorth(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_LEFT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row - 1, 13, row);
  }
}

/// Walking into the right beam statue from the east: as the left one. Reached
/// by walking west into cell codes 25 and 26 until
/// FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING is set.
static void Event_PushRightBeamStatueWest(void) {
  SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
  SolSanctumStatue_ShineRightBeam();
}

/// Walking into the right beam statue from the north: it slides one cell south.
/// Reached by walking south into cell code 26 until the beam shines.
static void Event_PushRightBeamStatueSouth(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_RIGHT_BEAM_STATUE);
  if (statue != NULL) {
    s32 row = statue->z.fixed >> 20;
    SolSanctumStatue_PushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row + 1, 22, row);
  }
}

/// Walking into the right beam statue from the south: it slides one cell north.
/// Reached by walking north into cell codes 28 and 29 until the beam shines.
static void Event_PushRightBeamStatueNorth(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_RIGHT_BEAM_STATUE);
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

  Engine_EventBegin();
  Engine_AudioPlayCue(SE_185);
  Actor_SetSpeed(statueId, PIXELS(1) / 5, PIXELS(1) / 10);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Object_GetById(statueId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Engine_ActorSetDestination(ACTOR_PARTY_LEADER, (leaderColumn << 4) + 8,
                             (leaderRow << 4) + 8);
  statueX = statueColumn << 4;
  statueZ = statueRow << 4;
  Engine_ActorSetDestination(statueId, statueX + 8, statueZ + 8);
  Engine_ActorWaitForMove(statueId);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventEnd();
}

/// Returns 1 when the actor stands in the given map cell.
static s32 SolSanctumStatue_IsActorAtCell(s32 actorId, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(actorId);
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
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 0;
  scene->open.srcY = 55;
  scene->open.destX = 32;
  scene->open.destY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.srcX = 2;
  scene->pulseA.srcY = 30;
  scene->pulseA.destX = 34;
  scene->pulseA.destY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 2;
  scene->pulseB.srcY = 28;
  scene->pulseB.destX = 34;
  scene->pulseB.destY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 2;
  scene->opened.srcY = 30;
  scene->opened.destX = 16;
  scene->opened.destY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destX = 500;
  scene->destZ = 132;
  scene->stairs[0].srcX = 8;
  scene->stairs[0].srcY = 55;
  scene->stairs[0].destX = 32;
  scene->stairs[0].destY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].srcX = 2;
  scene->stairs[1].srcY = 30;
  scene->stairs[1].destX = 34;
  scene->stairs[1].destY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 2;
  scene->stairs[2].srcY = 28;
  scene->stairs[2].destX = 16;
  scene->stairs[2].destY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = ACTOR_NORTHWEST_STATUE;
  scene->actorX = 488;
  scene->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the northeast seal animation and statue return position.
static void SolSanctumStatue_OpenNortheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 4;
  scene->open.srcY = 55;
  scene->open.destX = 36;
  scene->open.destY = 40;
  scene->open.width = 4;
  scene->open.height = 3;
  scene->pulseA.srcX = 4;
  scene->pulseA.srcY = 30;
  scene->pulseA.destX = 36;
  scene->pulseA.destY = 10;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 4;
  scene->pulseB.srcY = 28;
  scene->pulseB.destX = 36;
  scene->pulseB.destY = 10;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 4;
  scene->opened.srcY = 30;
  scene->opened.destX = 18;
  scene->opened.destY = 10;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
  scene->facing = FACING_SOUTH;
  scene->destX = 654;
  scene->destZ = 132;
  scene->stairs[0].srcX = 12;
  scene->stairs[0].srcY = 55;
  scene->stairs[0].destX = 36;
  scene->stairs[0].destY = 40;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 3;
  scene->stairs[1].srcX = 4;
  scene->stairs[1].srcY = 30;
  scene->stairs[1].destX = 36;
  scene->stairs[1].destY = 10;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 4;
  scene->stairs[2].srcY = 28;
  scene->stairs[2].destX = 18;
  scene->stairs[2].destY = 10;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = ACTOR_NORTHEAST_STATUE;
  scene->actorX = 664;
  scene->actorZ = 152;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southwest seal animation and statue return position.
static void SolSanctumStatue_OpenSouthwestSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 0;
  scene->open.srcY = 58;
  scene->open.destX = 32;
  scene->open.destY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.srcX = 2;
  scene->pulseA.srcY = 31;
  scene->pulseA.destX = 34;
  scene->pulseA.destY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 2;
  scene->pulseB.srcY = 29;
  scene->pulseB.destX = 34;
  scene->pulseB.destY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 2;
  scene->opened.srcY = 31;
  scene->opened.destX = 16;
  scene->opened.destY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destX = 500;
  scene->destZ = 216;
  scene->stairs[0].srcX = 8;
  scene->stairs[0].srcY = 58;
  scene->stairs[0].destX = 32;
  scene->stairs[0].destY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].srcX = 2;
  scene->stairs[1].srcY = 31;
  scene->stairs[1].destX = 34;
  scene->stairs[1].destY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 2;
  scene->stairs[2].srcY = 29;
  scene->stairs[2].destX = 16;
  scene->stairs[2].destY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = ACTOR_SOUTHWEST_STATUE;
  scene->actorX = 488;
  scene->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

/// Configures the southeast seal animation and statue return position.
static void SolSanctumStatue_OpenSoutheastSeal(void) {
  struct SealScene *scene = &sSealScene;
  scene->open.srcX = 4;
  scene->open.srcY = 58;
  scene->open.destX = 36;
  scene->open.destY = 43;
  scene->open.width = 4;
  scene->open.height = 1;
  scene->pulseA.srcX = 4;
  scene->pulseA.srcY = 31;
  scene->pulseA.destX = 36;
  scene->pulseA.destY = 11;
  scene->pulseA.width = 2;
  scene->pulseA.height = 1;
  scene->pulseB.srcX = 4;
  scene->pulseB.srcY = 29;
  scene->pulseB.destX = 36;
  scene->pulseB.destY = 11;
  scene->pulseB.width = 2;
  scene->pulseB.height = 1;
  scene->opened.srcX = 4;
  scene->opened.srcY = 31;
  scene->opened.destX = 18;
  scene->opened.destY = 11;
  scene->opened.width = 2;
  scene->opened.height = 1;
  scene->flag = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
  scene->facing = FACING_NORTH;
  scene->destX = 654;
  scene->destZ = 216;
  scene->stairs[0].srcX = 12;
  scene->stairs[0].srcY = 58;
  scene->stairs[0].destX = 36;
  scene->stairs[0].destY = 43;
  scene->stairs[0].width = 4;
  scene->stairs[0].height = 1;
  scene->stairs[1].srcX = 4;
  scene->stairs[1].srcY = 31;
  scene->stairs[1].destX = 36;
  scene->stairs[1].destY = 11;
  scene->stairs[1].width = 2;
  scene->stairs[1].height = 1;
  scene->stairs[2].srcX = 4;
  scene->stairs[2].srcY = 29;
  scene->stairs[2].destX = 18;
  scene->stairs[2].destY = 11;
  scene->stairs[2].width = 2;
  scene->stairs[2].height = 1;
  scene->actor = ACTOR_SOUTHEAST_STATUE;
  scene->actorX = 664;
  scene->actorZ = 200;
  SolSanctumStatue_OpenSeal();
}

#define COPY_CELLS(copy)                                                       \
  Map_CopyCellsTo((copy).srcX, (copy).srcY, (copy).destX, (copy).destY,        \
                  (copy).width, (copy).height)

/// Animates the seal and either advances the story or resets its statue.
/// Called by the four OpenXSeal functions with sSealScene filled in. Sets the
/// seal's flag; when no scripted scene follows and the centre statue has not
/// moved, the statue slides back, the leader jumps clear and the flag is
/// cleared again.
static void SolSanctumStatue_OpenSeal(void) {
  struct SealScene *scene;
  s32 exitResult;
  s32 pulse;

  exitResult = 0;
  Engine_EventBegin();
  if (!Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
    // Show the configured seal and flash its two tile states.
    Engine_CameraSetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveTo(PIXELS(576), -1, PIXELS(172), 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(SE_186);
    COPY_CELLS(sSealScene.open);
    for (pulse = 0; pulse != 20; pulse++) {
      Engine_AudioPlayCue(SE_246);
      COPY_CELLS(sSealScene.pulseA);
      Engine_EventWait(4);
      Engine_AudioPlayCue(SE_246);
      COPY_CELLS(sSealScene.pulseB);
      Engine_EventWait(4);
    }
    scene = &sSealScene;
    COPY_CELLS(scene->opened);
    Engine_GameFlagSet(scene->flag);
    exitResult = SolSanctumStatue_SelectStoryExit();
    // Keep the statue reusable until the central statue has moved.
    if (exitResult == -1) {
      if (!Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
        Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
        Object_GetById(ACTOR_PARTY_LEADER)->facing = scene->facing;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
        Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &=
            ~ACTOR_FACES_MOTION;
        Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
        Engine_ActorSetDestination(ACTOR_PARTY_LEADER, scene->destX,
                                   scene->destZ);
        COPY_CELLS(scene->stairs[0]);
        COPY_CELLS(scene->stairs[1]);
        COPY_CELLS(scene->stairs[2]);
        Object_GetById(scene->actor)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Engine_ObjectMotionSetPositionAndCommit(scene->actor, scene->actorX,
                                                scene->actorZ);
        Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
        Engine_GameFlagClear(scene->flag);
      }
    } else if (exitResult == 0 &&
               Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
      // DEAD: SelectStoryExit returns only 1 or -1, so this branch cannot run.
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) &&
          Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) &&
          Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN)) {
        if (!GameFlag_IsSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
          Engine_GameFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
          SolSanctumStatue_RunSealOpenedSequence();
        }
      } else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN)) {
        Engine_EventRequestExit(5);
        exitResult = 1;
      }
    }
  }
  if (exitResult == 1) {
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  }
  Engine_EventEnd();
}

/// Draws the left beam and opens the floor when the right beam is also lit.
/// Called when the left beam statue reaches its place in front of the centre,
/// until the centre statue has moved. Sets FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING.
static void SolSanctumStatue_ShineLeftBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      Engine_CameraWaitForMove();
      Event_PlayCue(SE_186);
      Map_CopyCellsTo(0, 59, 15, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
      Engine_EventWait(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Engine_EventEnd();
}

/// Draws the right beam and opens the floor when the left beam is also lit.
/// As the left one; sets FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING.
static void SolSanctumStatue_ShineRightBeam(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0) {
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Camera_MoveTo(PIXELS(286), -1, PIXELS(146), 1);
      Engine_CameraWaitForMove();
      Event_PlayCue(SE_186);
      Map_CopyCellsTo(4, 59, 17, 38, 4, 3);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
      }
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
      Engine_EventWait(30);
      GameFlag_Set(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
        SolSanctumStatue_OpenCentralHole();
      }
    }
  }
  Engine_EventEnd();
}

/// Shows the selected message from one speaker and waits the requested frames.
static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

struct SceneEntrance gSolSanctumStatue_Entrances[] = {
    {SOL_SANCTUM_STATUE_FROM_VALE_GATE, CONDITION_ALWAYS, 287, 0, 176,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 646, 0, 329, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE, CONDITION_ALWAYS, 647,
     0, 470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumStatue_Exits[] = {
    SCENE_EXITS(SolSanctumStatue),
    SCENE_EXIT(1, SolSanctumLunaSolRooms, 2),
    SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
               SolSanctumPsynergyStone,
               SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE),
    SCENE_EXIT(3, SolSanctumLunaSolRooms, 4),
    SCENE_EXIT(4, SolSanctumLunaSolRooms, 5),
    SCENE_EXIT(5, SolSanctumLunaSolRooms, 6),
    SCENE_EXIT(6, SolSanctumLunaSolRooms, 7),
    SCENE_EXIT_TO_BATTLE(SOL_SANCTUM_STATUE_TO_SCENE_510, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumStatue_Placements[] = {
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(216), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xca, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(288), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcc, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(560), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(592), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gSolSanctumStatue_Events[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
    {TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_CheckAheadFacingNorth)},
    {TOUCH_WALKING, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_CheckAheadFacingSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_STATUE,
     FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(Event_NorthwestStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHEAST_STATUE,
     FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(Event_NortheastStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHWEST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(Event_SouthwestStatueMoved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_STATUE,
     FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(Event_SoutheastStatueMoved)},
    {TOUCH_WALKING_INTO(FACING_EAST), 5, FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(Event_PushNorthwestStatue)},
    {TOUCH_WALKING_INTO(FACING_WEST), 6, FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(Event_PushNortheastStatue)},
    {TOUCH_WALKING_INTO(FACING_EAST), 7, FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     EVENT_SCRIPT(Event_PushSouthwestStatue)},
    {TOUCH_WALKING_INTO(FACING_WEST), 8, FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     EVENT_SCRIPT(Event_PushSoutheastStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), ACTOR_LEFT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_BlockLeftTriggerCell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_LeftBeamStatueMoved)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), ACTOR_RIGHT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_BlockRightTriggerCell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_BEAM_STATUE,
     FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_RightBeamStatueMoved)},
    {TOUCH_WALKING_INTO(FACING_EAST), 20, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushLeftBeamStatueEast)},
    {TOUCH_WALKING_INTO(FACING_EAST), 21, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushLeftBeamStatueEast)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 21, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushLeftBeamStatueSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 23, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushLeftBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 24, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushLeftBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_WEST), 25, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushRightBeamStatueWest)},
    {TOUCH_WALKING_INTO(FACING_WEST), 26, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushRightBeamStatueWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushRightBeamStatueSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 28, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushRightBeamStatueNorth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     EVENT_SCRIPT(Event_PushRightBeamStatueNorth)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), ACTOR_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(Event_BlockCenterTriggerCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CENTER_STATUE,
     FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(Event_CenterStatueMoved)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 32, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     EVENT_SCRIPT(Event_PushCenterStatueNorth)},
    {SCENE_EVENTS_END},
};

s32 gSolSanctumStatue_ScrollPhase = 0;
