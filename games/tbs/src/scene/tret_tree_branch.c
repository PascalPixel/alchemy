/// @file
/// A branch of Tret, the great tree, where Breeze, a Jupiter Djinni, waits;
/// touch triggers move the leader and the camera along the branch.

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
#include "djinn_ids.h"
#include "display_transition.h"
#include "map_scroll.h"
#include "flags.h"
#include "songs.h"
#include "event.h"

/// Breeze, the Jupiter Djinni on the branch: Djinni 61 is the name message
/// "Breeze", and its placement is gone once its flag (FLAG_DJINNI_0 + 61) is
/// set.
enum { ACTOR_BREEZE = ACTOR_FIRST_PLACED, SPRITE_JUPITER_DJINNI = 0x5d };

/// Starts the scanline-dependent BG3 scroll callbacks.
static void TretTreeBranch_StartBg3Split(void);

static u16 sTransferHeader[16];
static u8 sTransferData[96];
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

/// Talking to Breeze: the Djinni hops and its battle starts.
static void Event_TalkToBreeze(void) {
  Field_RequestDjinnBattle(ACTOR_BREEZE, DJINNI_BREEZE);
}

/// Returns the branch entrances from the adjoining Tret rooms.
const struct SceneEntrance *TretTreeBranch_GetEntrances(void) {
  return gTretTreeBranch_Entrances;
}

/// Returns no region table for this overlay.
const struct SceneRegion *TretTreeBranch_GetRegions(void) { return NULL; }

/// Returns the packed exits to the adjoining Tret scenes.
const u32 *TretTreeBranch_GetExits(void) { return gTretTreeBranch_Exits; }

/// Returns the branch's actor placement table.
const struct ScenePlacement *TretTreeBranch_GetPlacements(void) {
  return gTretTreeBranch_Placements;
}

/// Moves the leader and camera by signed 16-pixel cell offsets, placing each
/// at the terrain height of its destination.
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

/// Touch trigger 10: the leader and camera move 80 pixels south at once.
static void Event_ShiftSouthFiveCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touch trigger 11: the leader and camera move 80 pixels north at once.
static void Event_ShiftNorthFiveCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touch trigger 12: the same 80-pixel move south as trigger 10.
static void Event_ShiftSouthFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 5);
}

/// Touch trigger 13: the same 80-pixel move north as trigger 11.
static void Event_ShiftNorthFiveCellsSecond(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -5);
}

/// Touch trigger 14: the leader and camera move 96 pixels south at once.
static void Event_ShiftSouthSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, 6);
}

/// Touch trigger 15: the leader and camera move 96 pixels north at once.
static void Event_ShiftNorthSixCells(void) {
  TretTreeBranch_ShiftLeaderAndCameraByCells(0, -6);
}

/// Walking into one of exits 1 to 8: the exit sound plays and the party
/// leaves by the exit touched.
static void Event_TakeTouchedExit(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedExit;

  SE(SE_MAP_EXIT);
  touchedExit = &state->touchedTrigger;
  Engine_EventRequestExit(*touchedExit);
}

/// Selects the same mask and window parameters for Reveal and scene entry.
static inline void
TretTreeBranch_SetRevealWindow(struct DisplayTransitionState *state) {
  state->mask = 0x3f3f;
  state->active = 31;
  state->value = 10;
}

/// Casting Reveal on the branch (its phase 2): the window transition and BG1's
/// blend over the rest of the screen are set again, as when the scene opened.
static void Event_CastReveal(void) {
  struct DisplayTransitionState *transition;

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

  transition = gDisplayTransition;
  TretTreeBranch_SetRevealWindow(transition);
}

/// Returns the touch, talk, exit and Reveal events for this branch.
const struct SceneEvent *TretTreeBranch_GetEvents(void) {
  return gTretTreeBranch_Events;
}

/// Unused event hook: begins and immediately ends an event.
static void UNUSED TretTreeBranch_RunEmptyEvent(void) {
  EV_BEGIN();
  EV_END();
}

/// Unused empty hook; its original role is not established.
static void UNUSED TretTreeBranch_UnusedHookFirst(void) {}

/// Another unused empty hook with no established role.
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
  struct DisplayTransitionState *transition;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(4, 12);

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

/// Updates the split from the first map layer's pixel position. Before the
/// split, scroll subtracts one pixel for every four elapsed frames.
static void TretTreeBranch_UpdateBg3ScrollSplit(void) {
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[0];
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
                             TASK_PRIORITY_SCENE);
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
    { SPRITE_JUPITER_DJINNI, FLAG_DJINNI_0 + DJINNI_BREEZE, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gTretTreeBranch_Events[] = {
    {0xa02, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {0xa02, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeTouchedExit)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ShiftSouthFiveCells)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ShiftNorthFiveCells)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ShiftSouthFiveCellsSecond)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ShiftNorthFiveCellsSecond)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShiftSouthSixCells)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShiftNorthSixCells)},
    {EVENT_TALK, ACTOR_BREEZE, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToBreeze)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 2), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_CastReveal)},
    {SCENE_EVENTS_END},
};

u16 gTretTreeBranch_CellSteps[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
