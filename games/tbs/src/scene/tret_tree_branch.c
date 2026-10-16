#include "unit/scene/tret_tree_branch.h"
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
#include "scene.h"
#include "resource_397.h"
#include "io_reg.h"
#include "irq.h"
#include "game_state.h"
#include "ability_ids.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "display_transition.h"
#include "map_scroll.h"

// Starts the scanline-dependent BG3 scroll callbacks.
static void TretTreeBranch_StartBg3Split(void);

static u16 sTransferHeader[16];
static u8 sTransferData[96];
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

// Starts the battle for Djinni 61, using actor 8 for its opening hop.
static void TretTreeBranch_StartDjinniBattle(void) {
  Field_RequestDjinnBattle(8, 61);
}

// Returns the branch entrances from the adjoining Tret rooms.
const struct SceneEntrance *TretTreeBranch_GetEntrances(void) {
  return gTretTreeBranch_Entrances;
}

// Returns no region table for this overlay.
const struct SceneRegion *TretTreeBranch_GetRegions(void) { return NULL; }

// Returns the packed exits to the adjoining Tret scenes.
const u32 *TretTreeBranch_GetExits(void) { return gTretTreeBranch_Exits; }

// Returns the branch's actor placement table.
const struct ScenePlacement *TretTreeBranch_GetPlacements(void) {
  return gTretTreeBranch_Placements;
}

// Moves the leader and camera by signed 16-pixel cell offsets, placing each
// at the terrain height of its destination.
static void TretTreeBranch_ShiftLeaderAndCameraByCells(s32 cellOffsetX,
                                                       s32 cellOffsetZ) {
  struct EventWork *state = gEventWork;
  struct FieldActor *actor;
  s32 offsetX = cellOffsetX;
  s32 offsetZ = cellOffsetZ;
  s32 terrainHeight;

  actor = Object_GetById(gGameState.selectedActor);
  // Multiplication keeps negative cell offsets defined; every caller is within
  // six cells, so the fixed-point result fits in s32.
  offsetX *= PIXELS(16);
  offsetZ *= PIXELS(16);

  // Shift the selected actor before the separate camera object.
  if (actor != 0) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    terrainHeight =
        Map_GetTerrainHeight(actor->terrainId, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = terrainHeight;
    actor->terrainHeight = terrainHeight;
  }

  actor = state->viewCenter;
  if (actor != 0) {
    actor->x.fixed += offsetX;
    actor->z.fixed += offsetZ;
    terrainHeight =
        Map_GetTerrainHeight(actor->terrainId, actor->x.fixed, actor->z.fixed);
    actor->y.fixed = terrainHeight;
    actor->terrainHeight = terrainHeight;
  }
}

// Moves the leader and camera five cells along positive z.
static void TretTreeBranch_AdvanceFiveCellsFirst(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

// Moves the leader and camera five cells along negative z.
static void TretTreeBranch_ReturnFiveCellsFirst(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

// The second five-cell forward trigger uses the same displacement.
static void TretTreeBranch_AdvanceFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

// The second five-cell return trigger reverses that displacement.
static void TretTreeBranch_ReturnFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

// Moves the leader and camera six cells along positive z.
static void TretTreeBranch_AdvanceSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 6);
}

// Moves the leader and camera six cells along negative z.
static void TretTreeBranch_ReturnSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -6);
}

// Plays the exit cue and requests the exit number recorded by the trigger.
static void TretTreeBranch_PlayCueAndSelectTouchedExit(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedExit;

  Engine_AudioPlayCue(123);
  touchedExit = &state->touchedTrigger;
  Engine_EventRequestExit(*touchedExit);
}

// Selects the same mask and window parameters for Reveal and scene entry.
static inline void
TretTreeBranch_SetRevealWindow(struct DisplayTransitionState *state) {
  state->mask = 0x3f3f;
  state->active = 31;
  state->value = 10;
}

// Reveal selects window-transition mode 9 and its blend/window parameters.
static void TretTreeBranch_ConfigureReveal(void) {
  struct DisplayTransitionState *transition;

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x0c04;

  transition = gDisplayTransition;
  TretTreeBranch_SetRevealWindow(transition);
}

// Returns the touch, talk, exit and Reveal events for this branch.
const struct SceneEvent *TretTreeBranch_GetEvents(void) {
  return gTretTreeBranch_Events;
}

// Unused event hook: begins and immediately ends an event.
static void UNUSED TretTreeBranch_RunEmptyEvent(void) {
  Engine_EventBegin();
  Engine_EventEnd();
}

// Unused empty hook; its original role is not established.
static void UNUSED TretTreeBranch_UnusedHookFirst(void) {}

// Another unused empty hook with no established role.
static void UNUSED TretTreeBranch_UnusedHookSecond(void) {}

// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED TretTreeBranch_ExchangeInputPacket(void) {
  u16 *header = sTransferHeader;

  header[0] = gKeysHeld;
  header[1] = gKeysRepeat;
  SerialRuntime_PollAndTransfer(header, sTransferData);
}

// Initializes the backdrop transition and installs BG3's scanline scroll split.
// Returns 0 for the overlay initialization entry point.
s32 TretTreeBranch_Initialize(void) {
#ifndef UBFIX
  u8 **eventSlot = (u8 **)&gEventWork;
#endif
  struct DisplayTransitionState *transition;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x0c04;

  // UB: slot 31 lies past scalar gEventWork. Its named load adds six
  // bytes in the native EN trial (0x6a to 0x70).
#ifdef UBFIX
  transition = gDisplayTransition;
#else
  transition = (struct DisplayTransitionState *)eventSlot[4];
#endif
  TretTreeBranch_SetRevealWindow(transition);

  TretTreeBranch_StartBg3Split();
  return 0;
}

// HBlank selects BG3's horizontal offset for the next scanline.
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

// Updates the split from the first map layer's pixel position. Before the
// split, scroll subtracts one pixel for every four elapsed frames.
static void TretTreeBranch_UpdateBg3ScrollSplit(void) {
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[0];
  s32 horizontalOffset;

  sSplitLine = 192 - (s16)(layer->y >> 16);
  sHofsAtOrAfterSplit = horizontalOffset = (s16)(layer->x >> 16);
  sHofsBeforeSplit = horizontalOffset - (gFrameCount >> 2);
}

// Run the BG3 scroll selection at every HBlank, updating the split each frame.
static void TretTreeBranch_StartBg3Split(void) {
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, TretTreeBranch_ApplyBg3ScrollAtScanline);
  FieldScheduler_AddCallback(TretTreeBranch_UpdateBg3ScrollSplit, 0xc80);
}

struct SceneEntrance gTretTreeBranch_Entrances[] = {
    {0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 328, 0, 424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 424, 0, 440, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 424, 0, 328, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 376, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 328, 0, 248, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 328, 0, 152, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTretTreeBranch_Exits[] = {
    SCENE_EXITS(46),      SCENE_EXIT(1, 45, 2), SCENE_EXIT(2, 45, 3),
    SCENE_EXIT(3, 45, 4), SCENE_EXIT(4, 45, 5), SCENE_EXIT(5, 45, 7),
    SCENE_EXIT(6, 45, 6), SCENE_EXIT(7, 45, 8), SCENE_EXIT(8, 45, 9),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTretTreeBranch_Placements[] = {
    { 0x5d, 0x6d, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gTretTreeBranch_Events[] = {
    {0xa02, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 3, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 4, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_PlayCueAndSelectTouchedExit)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_AdvanceFiveCellsFirst)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_ReturnFiveCellsFirst)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_AdvanceFiveCellsSecond)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_ReturnFiveCellsSecond)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_AdvanceSixCells)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_ReturnSixCells)},
    {EVENT_TALK, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_StartDjinniBattle)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 2), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretTreeBranch_ConfigureReveal)},
    {SCENE_EVENTS_END},
};

u16 gTretTreeBranch_CellSteps[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
