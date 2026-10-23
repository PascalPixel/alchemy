/// @file
/// Tret Tree (MapId_TretTreeBranch), the second of Tret Tree's two maps:
/// out on Tret's branches, under open sky
/// (places/tret-tree-branch/map-0.png), between the rooms of
/// MapId_TretTreeInteriors (tret_tree_interiors.c), where the story is told.
/// This map has no story event and tests no story flag.
///
/// Its eight warps (TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST and
/// the seven after it) lead back into MapId_TretTreeInteriors; walking into
/// one for two frames (COORD_EVENT_FLAG_QUICK) plays the warp sound and leaves
/// at once, the cells' metatile behaviors being the warps' own numbers.
/// Coord events on metatile behaviors 10 to 15 lie in pairs at the top and
/// bottom of the vines that hang from one branch to the one below (cells
/// (7, 12) and (8, 17), (37, 11) and (36, 16), (41, 20) and (40, 26) in
/// layout_047's grid_collision.tsv): each moves the leader and the camera five
/// or six cells north or south in one step, up or down its vine, with no walk.
/// Each vine has metatile behaviors of its own, so 12 and 13 repeat 10's and
/// 11's events for the second vine. Breeze, a Jupiter Djinni, stands here until
/// it has joined the party: its own flag (FLAG_DJINNI_0 + DJINNI_BREEZE)
/// removes its object event template. Talking to it starts a battle with it.
///
/// The setup opens the map with a backdrop fade, blends BG1 over the rest of
/// the screen and splits BG3's scroll at a scanline, 192 less the ground
/// layer's vertical scroll (TretTreeBranch_UpdateBg3ScrollSplit). BG3 follows
/// the map's horizontal scroll on both sides of the split, and above it its
/// offset also falls by one every four frames, so the clouds there drift
/// right while the band below moves only with the map
/// (https://alchemy.pascalpixel.com/places/tret-tree-branch/bg3-split.gif:
/// the screen beside BG3 alone, the split marked in red). Casting Reveal sets
/// the blend and the window again as at entry. The overlay also keeps three
/// unused hooks and a link-cable input exchange no table reaches.
///
/// Open questions: none.

#include "unit/map/tret_tree_branch.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
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
#include "global_cells.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_offset.h"
#include "resource_397.h"
#include "io_reg.h"
#include "irq.h"
#include "game_state.h"
#include "ability_ids.h"
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "display_transition.h"
#include "map_grid_scroll.h"
#include "flags.h"
#include "songs.h"
#include "script.h"

/// Breeze, a Jupiter Djinni (DJINNI_BREEZE), the only placed object event; its
/// object event template is gone once its flag (FLAG_DJINNI_0 + DJINNI_BREEZE)
/// is set.
enum {
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  LOCALID_BREEZE = LOCALID_FIRST_PLACED
};

/// Starts the scanline-dependent BG3 scroll callbacks.
static void TretTreeBranch_StartBg3Split(void);

static u16 sTransferHeader[16];
static u8 sTransferData[96];
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

/// Talking to Breeze in Tret Tree: the Djinni hops and the battle with it
/// starts.
static void EventScript_TalkToBreeze(void) {
  Field_RequestDjinnBattle(LOCALID_BREEZE, DJINNI_BREEZE);
}

const struct MapWarpDest *TretTreeBranch_GetWarpDests(void) {
  return gTretTreeBranch_WarpDests;
}

const struct MapWarpRegion *TretTreeBranch_GetRegions(void) { return NULL; }

const u32 *TretTreeBranch_GetWarps(void) { return gTretTreeBranch_Warps; }

const struct ObjectEventTemplate *TretTreeBranch_GetObjectEventTemplates(void) {
  return gTretTreeBranch_ObjectEventTemplates;
}

/// Moves the leader and camera by signed 16-pixel cell offsets, placing each
/// at the terrain height of its destination.
static void TretTreeBranch_ShiftLeaderAndCameraByCells(s32 cellOffsetX,
                                                       s32 cellOffsetZ) {
  struct EventWork *state = gEventWork;
  struct ObjectEvent *objectEvent;
  s32 offsetX = cellOffsetX;
  s32 offsetZ = cellOffsetZ;
  s32 terrainHeight;

  objectEvent = ObjectEvent_GetById(gGameState.selectedObjectEvent);
  // Multiplication keeps negative cell offsets defined; every caller is within
  // six cells, so the fixed-point result fits in s32.
  offsetX *= PIXELS(16);
  offsetZ *= PIXELS(16);

  // Shift the selected object event before the separate camera object.
  if (objectEvent != 0) {
    objectEvent->x.fixed += offsetX;
    objectEvent->z.fixed += offsetZ;
    terrainHeight = MapGrid_GetTerrainHeight(
        objectEvent->terrainId, objectEvent->x.fixed, objectEvent->z.fixed);
    objectEvent->y.fixed = terrainHeight;
    objectEvent->terrainHeight = terrainHeight;
  }

  objectEvent = state->viewCenter;
  if (objectEvent != 0) {
    objectEvent->x.fixed += offsetX;
    objectEvent->z.fixed += offsetZ;
    terrainHeight = MapGrid_GetTerrainHeight(
        objectEvent->terrainId, objectEvent->x.fixed, objectEvent->z.fixed);
    objectEvent->y.fixed = terrainHeight;
    objectEvent->terrainHeight = terrainHeight;
  }
}

/// Touching metatile behavior 10 in Tret Tree: the leader and camera move 80
/// pixels south at once.
static void EventScript_ShiftSouthFiveCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touching metatile behavior 11 in Tret Tree: the leader and camera move 80
/// pixels north at once.
static void EventScript_ShiftNorthFiveCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touching metatile behavior 12 in Tret Tree: the same 80-pixel move south as
/// metatile behavior 10.
static void EventScript_ShiftSouthFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touching metatile behavior 13 in Tret Tree: the same 80-pixel move north as
/// metatile behavior 11.
static void EventScript_ShiftNorthFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touching metatile behavior 14 in Tret Tree: the leader and camera move 96
/// pixels south at once.
static void EventScript_ShiftSouthSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 6);
}

/// Touching metatile behavior 15 in Tret Tree: the leader and camera move 96
/// pixels north at once.
static void EventScript_ShiftNorthSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -6);
}

/// Walking into one of the eight warps in Tret Tree, whose cells' metatile
/// behaviors are the warps' own numbers: the warp sound plays and the party
/// leaves by the warp touched, into Tret Tree's other map
/// (MapId_TretTreeInteriors).
static void EventScript_TakeTouchedWarp(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedWarp;

  SE(SE_MAP_EXIT);
  touchedWarp = &state->touchedTrigger;
  Engine_ScriptRequestWarp(*touchedWarp);
}

/// Selects the same mask and window parameters for Reveal and map entry.
static inline void
TretTreeBranch_SetRevealWindow(struct DisplayTransitionState *state) {
  state->mask = 0x3f3f;
  state->active = 31;
  state->value = 10;
}

/// Casting Reveal in Tret Tree: the window transition and BG1's blend over the
/// rest of the screen are set again, as when the map opened.
static void EventScript_CastReveal(void) {
  struct DisplayTransitionState *state;

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  state = gDisplayTransition;
  TretTreeBranch_SetRevealWindow(state);
}

/// Returns the map's warp, coord, talk and Reveal events.
const struct MapEvent *TretTreeBranch_GetEvents(void) {
  return gTretTreeBranch_Events;
}

/// Unused script hook: begins and immediately ends a script.
static void UNUSED TretTreeBranch_RunEmptyEvent(void) {
  EV_START();
  EV_END();
}

static void UNUSED TretTreeBranch_UnusedHookFirst(void) {}

static void UNUSED TretTreeBranch_UnusedHookSecond(void) {}

/// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED TretTreeBranch_ExchangeInputPacket(void) {
  u16 *header = sTransferHeader;

  header[0] = gKeysHeld;
  header[1] = gKeysRepeat;
  SerialRuntime_PollAndTransfer(header, sTransferData);
}

/// Initializes the backdrop transition and installs BG3's scanline scroll
/// split. Returns 0 for the overlay initialization entry point.
s32 TretTreeBranch_Initialize(void) {
#ifndef UBFIX
  u8 **eventSlot = (u8 **)&gEventWork;
#endif
  struct DisplayTransitionState *state;

  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  // UB: HEAP_SLOT_TRANSITION lies past scalar gEventWork. Its named load adds
  // six bytes in the native EN trial (0x6a to 0x70).
#ifdef UBFIX
  state = gDisplayTransition;
#else
  state = (struct DisplayTransitionState *)eventSlot[4];
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
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, TretTreeBranch_ApplyBg3ScrollAtScanline);
  FieldScheduler_AddCallback(TretTreeBranch_UpdateBg3ScrollSplit,
                             TASK_PRIORITY_MAP);
}

struct MapWarpDest gTretTreeBranch_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST,
     CONDITION_ALWAYS, 328, 0, 424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHEAST,
     CONDITION_ALWAYS, 424, 0, 440, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH, CONDITION_ALWAYS, 376, 0,
     360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTHEAST, CONDITION_ALWAYS, 424,
     0, 328, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH, CONDITION_ALWAYS, 376, 0,
     264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTHWEST, CONDITION_ALWAYS, 328,
     0, 248, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHWEST,
     CONDITION_ALWAYS, 328, 0, 152, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHEAST,
     CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gTretTreeBranch_Warps[] = {
    MAP_WARPS(TretTreeBranch),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH_SOUTHEAST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTH, TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_SOUTHEAST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTHEAST),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH, TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTHWEST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTHWEST),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH_NORTHWEST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST),
    MAP_WARP(TRET_TREE_BRANCH_TO_TRET_TREE_INTERIORS_NORTH_NORTHEAST,
             TretTreeInteriors,
             TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gTretTreeBranch_ObjectEventTemplates[] = {
    { JUPITER_DJINNI, FLAG_DJINNI_0 + DJINNI_BREEZE, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gTretTreeBranch_Events[] = {
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 3, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 4, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TakeTouchedWarp)},
    {MAP_EVENT_COORD, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftSouthFiveCells)},
    {MAP_EVENT_COORD, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftNorthFiveCells)},
    {MAP_EVENT_COORD, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftSouthFiveCellsSecond)},
    {MAP_EVENT_COORD, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftNorthFiveCellsSecond)},
    {MAP_EVENT_COORD, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftSouthSixCells)},
    {MAP_EVENT_COORD, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ShiftNorthSixCells)},
    {MAP_EVENT_TALK, LOCALID_BREEZE, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToBreeze)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_AFTER_END), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CastReveal)},
    {MAP_EVENTS_END},
};

/// Unused: the two steps of a door opening, as Vale's doors have them
/// (gValeStorm_SanctumDoorOpens); nothing on the branch plays them.
u16 gTretTreeBranch_UnusedCellSteps[] = {
    MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
    MAPGRID_ANIMATION_STEPS_END,
};
