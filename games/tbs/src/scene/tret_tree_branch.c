/// @file
/// Tret Tree (SceneId_TretTreeBranch), the second of Tret Tree's two scenes:
/// out on Tret's branches, under open sky
/// (places/tret-tree-branch/map-0.png), between the rooms of
/// SceneId_TreeRooms (tree_rooms.c), where the story is told.
/// This scene has no story event and tests no story flag.
///
/// Its eight exits (TRET_TREE_BRANCH_TO_TREE_ROOMS_SOUTH_SOUTHWEST and
/// the seven after it) lead back into SceneId_TreeRooms; walking into
/// one for two frames (STEP_TRIGGER_FLAG_QUICK) plays the exit sound and leaves
/// at once, the cells' cell behaviors being the exits' own numbers.
/// Step triggers on cell behaviors 10 to 15 lie in pairs at the top and
/// bottom of the vines that hang from one branch to the one below (cells
/// (7, 12) and (8, 17), (37, 11) and (36, 16), (41, 20) and (40, 26) in
/// map_047's map.txt): each moves the leader and the camera five
/// or six cells north or south in one step, up or down its vine, with no walk.
/// Each vine has cell behaviors of its own, so 12 and 13 repeat 10's and
/// 11's events for the second vine. Breeze, a Jupiter Djinni, stands here until
/// it has joined the party: its own flag (FLAG_DJINNI(DJINNI_BREEZE))
/// removes its actor placement. Talking to it starts a battle with it.
///
/// The setup opens the map with a backdrop fade, blends BG1 over the rest of
/// the screen and splits BG3's scroll at a scanline, 192 less the ground
/// layer's vertical scroll (TretTreeBranch_UpdateBg3ScrollSplit). BG3 follows
/// the map's horizontal scroll on both sides of the split, and above it its
/// offset also falls by one every four frames, so the clouds there drift
/// right while the band below moves only with the map. Casting Reveal sets
/// the blend and the window again as at entry. The overlay also keeps three
/// unused hooks and a link-cable input exchange no table reaches.

#include "module/scene/tret_tree_branch.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "irq.h"
#include "adventure.h"
#include "ability_ids.h"
#include "event.h"
#include "system_call.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "display_transition.h"
#include "map_grid_scroll.h"
#include "flags.h"
#include "sounds.h"

/// Breeze, a Jupiter Djinni (DJINNI_BREEZE), the only placed actor; its
/// actor placement is gone once its flag (FLAG_DJINNI(DJINNI_BREEZE))
/// is set.
enum {
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  ACTOR_BREEZE = ACTOR_FIRST_PLACED
};

/// Starts the scanline-dependent BG3 scroll tasks.
static void TretTreeBranch_StartBg3Split(void);

static u16 sTransferHeader[16];
static u8 sTransferData[96];
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

/// Talking to Breeze in Tret Tree: the Djinni hops and the battle with it
/// starts.
static void TretTreeBranch_TalkToBreeze(void) {
  Field_RequestDjinnBattle(ACTOR_BREEZE, DJINNI_BREEZE);
}

const struct SceneEntrance *TretTreeBranch_GetEntrances(void) {
  return gTretTreeBranch_Entrances;
}

const struct SceneExitRegion *TretTreeBranch_GetRegions(void) { return NULL; }

const u32 *TretTreeBranch_GetExits(void) { return gTretTreeBranch_Exits; }

const struct ActorPlacement *TretTreeBranch_GetActorPlacements(void) {
  return gTretTreeBranch_ActorPlacements;
}

/// Moves the leader and camera by signed 16-pixel cell offsets, placing each
/// at the ground height of its destination.
static void TretTreeBranch_ShiftLeaderAndCameraByCells(s32 cellOffsetX,
                                                       s32 cellOffsetZ) {
  struct FieldWork *state = gFieldWork;
  struct Actor *actor;
  s32 offsetX = cellOffsetX;
  s32 offsetZ = cellOffsetZ;
  s32 groundHeight;

  actor = Actor_GetById(gAdventure.leaderActorId);
  // Multiplication keeps negative cell offsets defined; every caller is within
  // six cells, so the fixed-point result fits in s32.
  offsetX *= PIXELS(16);
  offsetZ *= PIXELS(16);

  // Shift the selected actor before the separate camera actor.
  if (actor != 0) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    groundHeight =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = groundHeight;
    actor->groundHeight = groundHeight;
  }

  actor = state->camera;
  if (actor != 0) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    groundHeight =
        MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = groundHeight;
    actor->groundHeight = groundHeight;
  }
}

/// Touching cell behavior 10 in Tret Tree: the leader and camera move 80
/// pixels south at once.
static void TretTreeBranch_GoDownWestVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touching cell behavior 11 in Tret Tree: the leader and camera move 80
/// pixels north at once.
static void TretTreeBranch_GoUpWestVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touching cell behavior 12 in Tret Tree: the same 80-pixel move south as
/// cell behavior 10.
static void TretTreeBranch_GoDownNortheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touching cell behavior 13 in Tret Tree: the same 80-pixel move north as
/// cell behavior 11.
static void TretTreeBranch_GoUpNortheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touching cell behavior 14 in Tret Tree: the leader and camera move 96
/// pixels south at once.
static void TretTreeBranch_GoDownSoutheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 6);
}

/// Touching cell behavior 15 in Tret Tree: the leader and camera move 96
/// pixels north at once.
static void TretTreeBranch_GoUpSoutheastVine(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -6);
}

/// Walking into one of the eight exits in Tret Tree, whose cells' cell
/// behaviors are the exits' own numbers: the exit sound plays and the party
/// leaves by the exit touched, into Tret Tree's other scene
/// (SceneId_TreeRooms).
static void TretTreeBranch_EnterTretTreeRooms(void) {
  struct FieldWork *state = gFieldWork;
  s16 *touchedExit;

  Event_PlaySound(SE_MAP_EXIT);
  touchedExit = &state->stepTrigger;
  Import_EventSetExitRequest(*touchedExit);
}

/// Selects the same mask and window parameters for Reveal and scene entry.
static inline void
TretTreeBranch_SetRevealWindow(struct ScreenTransitionState *state) {
  state->mask = 0x3f3f;
  state->active = 31;
  state->value = 10;
}

/// Casting Reveal in Tret Tree: the window transition and BG1's blend over the
/// rest of the screen are set again, as when the map opened.
static void TretTreeBranch_CastReveal(void) {
  struct ScreenTransitionState *state;

  ScreenTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  state = gScreenTransitionWork;
  TretTreeBranch_SetRevealWindow(state);
}

/// Returns the scene's exit, coord, talk and Reveal events.
const struct Trigger *TretTreeBranch_GetTriggers(void) {
  return gTretTreeBranch_Triggers;
}

/// Unused event hook: begins and immediately ends an event.
static void UNUSED TretTreeBranch_RunEmptyEvent(void) {
  Event_Start();
  Event_End();
}

static void UNUSED TretTreeBranch_UnusedHookFirst(void) {}

static void UNUSED TretTreeBranch_UnusedHookSecond(void) {}

/// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED TretTreeBranch_ExchangeInputPacket(void) {
  u16 *header = sTransferHeader;

  header[0] = gKeysHeld;
  header[1] = gKeysRepeat;
  Serial_PollAndTransfer(header, sTransferData);
}

/// Initializes the backdrop transition and installs BG3's line scroll
/// split. Returns 0 for the overlay initialization entry point.
s32 TretTreeBranch_Initialize(void) {
#ifndef UBFIX
  u8 **eventSlot = (u8 **)&gFieldWork;
#endif
  struct ScreenTransitionState *state;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  ScreenTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  // UB: HEAP_SLOT_SCREEN_TRANSITION lies past scalar gFieldWork. Its named load
  // adds six bytes in the native EN trial (0x6a to 0x70).
#ifdef UBFIX
  state = gScreenTransitionWork;
#else
  state = (struct ScreenTransitionState *)eventSlot[4];
#endif
  TretTreeBranch_SetRevealWindow(state);

  TretTreeBranch_StartBg3Split();
  return 0;
}

/// HBlank selects BG3's horizontal offset for the next scanline.
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

/// Updates the split from the pixel position of the map grid's first layer.
/// Before the split, scroll subtracts one pixel for every four elapsed frames.
static void TretTreeBranch_UpdateBg3ScrollSplit(void) {
  struct MapGridLayerScroll *layer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_GROUND];
  s32 horizontalOffset;

  sSplitLine = 192 - (s16)(layer->y >> 16);
  sHofsAtOrAfterSplit = horizontalOffset = (s16)(layer->x >> 16);
  sHofsBeforeSplit = horizontalOffset - (gFrameCount >> 2);
}

/// Runs the BG3 scroll selection at every HBlank, updating the split each
/// frame.
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
    { JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREEZE), MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gTretTreeBranch_Triggers[] = {
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 1, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 2, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 3, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 4, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 5, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 6, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 7, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, 8, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_EnterTretTreeRooms)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TretTreeBranch_GoDownWestVine)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TretTreeBranch_GoUpWestVine)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_GoDownNortheastVine)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_GoUpNortheastVine)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_GoDownSoutheastVine)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_GoUpSoutheastVine)},
    {TRIGGER_TALK, ACTOR_BREEZE, CONDITION_ALWAYS,
     EVENT(TretTreeBranch_TalkToBreeze)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_AFTER_END), 0,
     CONDITION_ALWAYS, EVENT(TretTreeBranch_CastReveal)},
    {TRIGGERS_END},
};

/// Unused: the two steps of a door opening, as Vale's doors have them
/// (gValeStormyNight_SanctumDoorOpens); nothing on the branch plays them.
u16 gTretTreeBranch_UnusedCellSteps[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
