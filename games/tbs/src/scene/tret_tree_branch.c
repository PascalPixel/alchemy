/// @file
/// Tret Tree's branches (SceneId_TretTreeBranch): the outside of Tret, the holy
/// tree of Kolima Forest, high up under open sky, with clouds drifting past
/// above and the green of the forest far below
/// (places/tret-tree-branch/map-0.png). Isaac climbs Tret from the inside
/// (SceneId_TreeRooms, tree_rooms.c), where the story is told; out here there
/// is no story event, and nothing tests a story flag.
///
/// Arched doors in Tret's trunk lead out onto the branches, two from each of
/// the second to fifth floors. Walking into one goes back inside:
/// - the second floor's west door (cell (20, 26)) opens onto a branch that
///   leads nowhere, and its east door (cell (26, 27)) onto the branch below
///   the southeast vine;
/// - the third floor's south door (cell (23, 22)) opens onto a branch that
///   runs west, and its east door (cell (26, 20)) onto the branch above the
///   southeast vine;
/// - the fourth floor's south door (cell (23, 16)) opens onto the branch below
///   the northeast vine, and its west door (cell (20, 15)) onto the branch
///   below the west vine;
/// - the fifth floor's west door (cell (20, 9)) opens onto the branch above the
///   west vine, and its east door (cell (26, 8)) onto the branch above the
///   northeast vine.
///
/// So the way up Tret goes out of the second floor's east door, up the
/// southeast vine and in at the third floor's east door; then out of the
/// fourth floor's south door, east and up the northeast vine to the fifth
/// floor's east door, where the Nut's chest is; and out of the fourth floor's
/// west door, along the branch and up the west vine to the fifth floor's west
/// door, which leads to the leaf that drops Isaac to Tret.
///
/// Breeze, a Jupiter Djinni, waits at the east end of the highest branch, above
/// the fifth floor's east door, until it has joined the party.
///
/// The three vines climb like any vine: Isaac stands on the vine and holds Up
/// or Down. Nothing visible happens at the vines' tops, but the game has work
/// to do there. The stretch of upper branch at each vine's top is stored twice
/// in the map: once as the branch Isaac walks on, and once as a hidden copy, 5
/// or 6 cells further south and as much higher, so that both are drawn on the
/// same pixels. Only the hidden copy joins the vine. One cell beside each vine
/// moves Isaac onto the copy, and one on the copy moves him back
/// (TretTreeBranch_JoinWestVine and the others).
///
/// Leaves hang in front of the branches, so the scene keeps an oval round Isaac
/// in which they are see-through, from when he steps out until he goes back
/// in. Reveal's own ripple takes its place while Reveal lasts, and the scene
/// puts it back afterwards.
///
/// The clouds above the forest drift slowly right while the forest below moves
/// only with the map (TretTreeBranch_UpdateBg3ScrollSplit).
///
/// The overlay also keeps three unused hooks and a link-cable input exchange no
/// table reaches.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"

extern struct SceneEntrance gTretTreeBranch_Entrances[];
extern u32 gTretTreeBranch_Exits[];
extern struct ActorPlacement gTretTreeBranch_ActorPlacements[];
extern struct Trigger gTretTreeBranch_Triggers[];

/// Breeze, a Jupiter Djinni (DJINNI_BREEZE), is the only placed actor. Its
/// actor placement is gone once its flag (FLAG_DJINNI(DJINNI_BREEZE)) is set.
enum { ACTOR_BREEZE = ACTOR_FIRST_PLACED };

/// The cells at the vines' tops that move Isaac between the branch he walks on
/// and the hidden copy of it that joins the vine. A door's cells carry its exit
/// number (TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST and the others).
enum TretTreeBranchCellBehavior {
  /// Beside the west vine's top, on the branch Isaac walks on: cell (7, 12).
  CELL_BEHAVIOR_TRET_JOIN_WEST_VINE = 10,
  /// On the hidden copy, east of the west vine's top: cell (8, 17).
  CELL_BEHAVIOR_TRET_LEAVE_WEST_VINE = 11,
  /// Above the northeast vine's top, on the branch: cell (37, 11).
  CELL_BEHAVIOR_TRET_JOIN_NORTHEAST_VINE = 12,
  /// On the hidden copy, west of the northeast vine's top: cell (36, 16).
  CELL_BEHAVIOR_TRET_LEAVE_NORTHEAST_VINE = 13,
  /// West of the southeast vine's top, on the branch: cell (41, 20).
  CELL_BEHAVIOR_TRET_JOIN_SOUTHEAST_VINE = 14,
  /// On the hidden copy, west of the southeast vine's top: cell (40, 26).
  CELL_BEHAVIOR_TRET_LEAVE_SOUTHEAST_VINE = 15,
};

/// Starts the clouds drifting above the forest.
static void TretTreeBranch_StartBg3Split(void);

/// The header and data of the unused input exchange
/// (TretTreeBranch_ExchangeInputPacket).
static u16 sTransferHeader[16] ALIGNED(16);
static u8 sTransferData[96];
/// The map height at which the background splits: the clouds drift above it,
/// and the forest far below lies under it.
#define BG3_SPLIT_Y 192

/// The screen line where BG3 splits, and its horizontal scroll below the split
/// (the forest) and above it (the drifting clouds).
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

/// Talking to Breeze, the Jupiter Djinni at the east end of Tret's highest
/// branch, makes it hop and starts the battle with it.
static void TretTreeBranch_TalkToBreeze(void) {
  Field_RequestDjinnBattle(ACTOR_BREEZE, DJINNI_BREEZE);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTretTreeBranch_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gTretTreeBranch_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gTretTreeBranch_ActorPlacements;
}

/// Moves the leader and the camera the same number of cells, each set on the
/// ground of the cell it lands on. Between a branch and its hidden copy the
/// cells south are as much higher, so the picture does not change.
static void TretTreeBranch_ShiftLeaderAndCameraByCells(s32 cellOffsetX,
                                                       s32 cellOffsetZ) {
  struct FieldState *state = gFieldState;
  struct Actor *actor;
  s32 offsetX = cellOffsetX;
  s32 offsetZ = cellOffsetZ;
  FIXED groundHeight;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  offsetX *= toFIXED(16);
  offsetZ *= toFIXED(16);

  if (actor != NULL) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    groundHeight =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = groundHeight;
    actor->groundHeight = groundHeight;
  }

  actor = state->camera;
  if (actor != NULL) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    groundHeight =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = groundHeight;
    actor->groundHeight = groundHeight;
  }
}

/// On the upper branch beside the west vine's top (cell (7, 12)), moves Isaac
/// onto the hidden copy that joins the vine, so that Down climbs him down it.
static void TretTreeBranch_JoinWestVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Stepping east off the west vine's top (cell (8, 17)) moves Isaac back onto
/// the drawn branch, the one to the fifth floor's west door.
static void TretTreeBranch_LeaveWestVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// On the northeast vine's top (cell (37, 11)), on the branch to the fifth
/// floor's east door, moves Isaac onto the hidden copy that joins the vine.
static void TretTreeBranch_JoinNortheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Stepping west off the northeast vine's top (cell (36, 16)) moves Isaac back
/// onto the drawn branch, the one to the fifth floor's east door and Breeze.
static void TretTreeBranch_LeaveNortheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// At the southeast vine's top (cell (41, 20)), on the branch from the third
/// floor's east door, moves Isaac onto the hidden copy that joins the vine,
/// down which he climbs to the second floor's east door.
static void TretTreeBranch_JoinSoutheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 6);
}

/// Stepping west off the southeast vine's top (cell (40, 26)) moves Isaac back
/// onto the drawn branch to the third floor's east door.
static void TretTreeBranch_LeaveSoutheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -6);
}

/// Isaac walks into one of the arched doors in Tret's trunk, such as the second
/// floor's west door at the end of the lowest branch (cell (20, 26)), and goes
/// back inside Tret Tree to the floor that door belongs to.
///
/// A moment's push against the doorway is enough (STEP_TRIGGER_FLAG_QUICK).
/// The exit sound plays, and the door's cell carries its own exit number,
/// which the step trigger hands on as the exit to take.
static void TretTreeBranch_EnterTretTreeRooms(void) {
  struct FieldState *state = gFieldState;

  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(state->stepTrigger);
}

/// The oval round Isaac (WINDOW_TRANSITION_LEADER) in which the leaves in front
/// of the branches are see-through: inside it BG1, the leaves, is blended over
/// the rest, and outside it every layer shows without the colour effect.
/// REVEAL_WINDOW_STEPS is how far it is open.
#define REVEAL_WININ (WININ_WIN0_ALL | WININ_WIN1_ALL)
#define REVEAL_WINOUT (WINOUT_WIN01_ALL & ~WINOUT_WIN01_CLR)
#define REVEAL_WINDOW_STEPS 10

/// Opens the see-through oval round Isaac, as the scene starts and again after
/// Reveal.
static inline void
TretTreeBranch_SetRevealWindow(struct ScreenTransitionState *state) {
  state->mask = REVEAL_WININ;
  state->active = REVEAL_WINOUT;
  state->value = REVEAL_WINDOW_STEPS;
}

/// When Reveal wears off, the leaves round Isaac turn see-through again: Reveal
/// drew its own rippling oval in place of the scene's, so the scene's oval and
/// the leaves' blend are set back as Scene_Initialize set them.
static void TretTreeBranch_RevealFades(void) {
  ScreenTransition_StartHeldWindow(WINDOW_TRANSITION_LEADER);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  TretTreeBranch_SetRevealWindow(gScreenTransitionState);
}

/// Returns the scene's events: its eight doors, the six cells at the vines'
/// tops, Breeze and Reveal.
const struct Trigger *Scene_GetTriggers(void) {
  return gTretTreeBranch_Triggers;
}

/// Unused event hook: begins and immediately ends an event.
static void UNUSED TretTreeBranch_RunEmptyEvent(void) {
  Event_Start();
  Event_End();
}

/// Unused: an empty function nothing refers to, the first of two here.
static void UNUSED TretTreeBranch_UnusedHookFirst(void) {}

/// Unused: an empty function nothing refers to, the second of two here.
static void UNUSED TretTreeBranch_UnusedHookSecond(void) {}

/// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED TretTreeBranch_ExchangeInputPacket(void) {
  u16 *header = sTransferHeader;

  header[0] = gKeysHeld;
  header[1] = gKeysRepeat;
  Serial_PollAndTransfer(header, sTransferData);
}

/// Opens Tret's branches with a backdrop fade, opens the see-through oval in
/// the leaves round Isaac and starts the clouds drifting.
s32 Scene_Initialize(void) {
  struct ScreenTransitionState *state;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  ScreenTransition_StartHeldWindow(WINDOW_TRANSITION_LEADER);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  state = gScreenTransitionState;
  TretTreeBranch_SetRevealWindow(state);

  TretTreeBranch_StartBg3Split();
  return 0;
}

/// At each scanline, picks the background's horizontal scroll: the drifting
/// clouds' above the split, the map's below it.
static void TretTreeBranch_ApplyBg3ScrollAtScanline(void) {
  u16 *scrollOffset;
  u32 horizontalScroll;

  if (REG_VCOUNT >= sSplitLine) {
    scrollOffset = &sHofsAtOrAfterSplit;
  } else {
    scrollOffset = &sHofsBeforeSplit;
  }
  horizontalScroll = *scrollOffset;
  REG_BG3HOFS = horizontalScroll;
}

/// Each frame, moves the split with the map's vertical scroll, so that it stays
/// at BG3_SPLIT_Y on the map. The clouds above it lag the map by one more pixel
/// every four frames, so they drift slowly right.
static void TretTreeBranch_UpdateBg3ScrollSplit(void) {
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollState *)gMapGridState[0])
           ->layers[MAPGRID_LAYER_GROUND];
  s32 horizontalOffset;

  sSplitLine = BG3_SPLIT_Y - (s16)(FIXEDtoINT(layer->y));
  sHofsAtOrAfterSplit = horizontalOffset = (s16)(FIXEDtoINT(layer->x));
  sHofsBeforeSplit = horizontalOffset - (gFrameCount >> 2);
}

/// Starts the clouds drifting: the split is chosen at every scanline and moved
/// every frame.
static void TretTreeBranch_StartBg3Split(void) {
  Irq_SetHandler(IRQ_HBLANK, 0, TretTreeBranch_ApplyBg3ScrollAtScanline);
  Task_AddFrame(TretTreeBranch_UpdateBg3ScrollSplit, TASK_PASS_FRAME);
}

struct SceneEntrance gTretTreeBranch_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHWEST, CONDITION_ALWAYS, 328, 0,
     424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHEAST, CONDITION_ALWAYS, 424, 0,
     440, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH, CONDITION_ALWAYS, 376, 0, 360,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTHEAST, CONDITION_ALWAYS, 424, 0, 328,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH, CONDITION_ALWAYS, 376, 0, 264,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTHWEST, CONDITION_ALWAYS, 328, 0, 248,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHWEST, CONDITION_ALWAYS, 328, 0,
     152, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHEAST, CONDITION_ALWAYS, 424, 0,
     136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTretTreeBranch_Exits[] = {
    SCENE_EXITS(TretTreeBranch),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHEAST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTHEAST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTHEAST),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTHWEST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTHWEST),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHWEST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST),
    SCENE_EXIT(TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHEAST, TreeRooms,
               TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gTretTreeBranch_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREEZE), MOVEMENT_TYPE_STAND,
        648, 0, 88, FACING_WEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gTretTreeBranch_Triggers[] = {
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTHWEST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHWEST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK,
     TRET_TREE_BRANCH_TO_TREE_ROOMS_NORTH_NORTHEAST, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_JOIN_WEST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_JoinWestVine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_LEAVE_WEST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_LeaveWestVine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_JOIN_NORTHEAST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_JoinNortheastVine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_LEAVE_NORTHEAST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_LeaveNortheastVine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_JOIN_SOUTHEAST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_JoinSoutheastVine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TRET_LEAVE_SOUTHEAST_VINE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_LeaveSoutheastVine)},
    {TRIGGER_TALK, ACTOR_BREEZE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_TalkToBreeze)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_AFTER_END), 0,
     CONDITION_ALWAYS, EVENT(TretTreeBranch_RevealFades)},
    {TRIGGER_TABLE_END},
};

/// Unused: the two steps of a door opening, as Vale's doors have them
/// (gValePrologue_SanctumDoorOpens). Nothing on the branch plays them.
u16 gTretTreeBranch_UnusedCellSteps[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
