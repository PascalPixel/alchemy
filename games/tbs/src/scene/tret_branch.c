#include "unit/scene/tret_branch.h"
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

// Starts the scanline-dependent BG3 scroll callbacks.
static void TretBranch_StartBg3Split(void);

struct SceneObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 settledY;
  u8 unk18[10];
  u8 layer;
};

union SceneCell {
  s32 w;
  s16 h[2];
};

static u16 sTransferHeader[16];
static u8 sTransferData[0x60];
static s32 sSplitLine;
static u16 sHofsAtOrAfterSplit;
static u16 sHofsBeforeSplit;

// Starts the battle for Djinni 0x3d, using actor 8 for its opening hop.
static void EV08_TALK(void) { BattleFx_SetPhaseRequest(8, 0x3D); }

// Returns the entrance table through the overlay's encoded table interface.
u8 *TretBranch_GetEntrances(void) { return (u8 *)gTretBranch_SceneTable0; }

// Returns no region table for this overlay.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the packed exits to the adjoining Tret scenes.
u8 *TretBranch_GetExits(void) { return (u8 *)gTretBranch_SceneTable1; }

// Returns the branch's actor placement table.
u8 *TretBranch_GetPlacements(void) { return (u8 *)gTretBranch_SceneTable2; }

// Moves the leader and camera by signed 16-pixel cell offsets, placing each
// at the terrain height of its destination.
static void TretBranch_ShiftLeaderAndCameraByCells(s32 cellOffsetX,
                                                   s32 cellOffsetZ) {
  u8 *work = *(u8 **)&gEventWork;
  struct SceneObject *obj;
  s32 dx = cellOffsetX;
  s32 dz = cellOffsetZ;
  s32 terrainHeight;

  obj = (struct SceneObject *)Object_GetById(gGameState.selectedActor);
  // Multiplication keeps negative cell offsets defined; every caller is within
  // six cells, so the fixed-point result fits in s32.
  dx *= 16 * 0x10000;
  dz *= 16 * 0x10000;

  // Shift the selected actor before the separate camera object.
  if (obj != 0) {
    obj->x += dx;
    obj->z += dz;
    terrainHeight = Map_GetTerrainHeight(obj->layer, obj->x, obj->z);
    obj->y = terrainHeight;
    obj->settledY = terrainHeight;
  }

  obj = *(struct SceneObject **)(work + 480);
  if (obj != 0) {
    obj->x += dx;
    obj->z += dz;
    terrainHeight = Map_GetTerrainHeight(obj->layer, obj->x, obj->z);
    obj->y = terrainHeight;
    obj->settledY = terrainHeight;
  }
}

// Moves the leader and camera five cells along positive z.
static void EV10_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, 5); }

// Moves the leader and camera five cells along negative z.
static void EV11_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, -5); }

// The second five-cell forward trigger uses the same displacement.
static void EV12_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, 5); }

// The second five-cell return trigger reverses that displacement.
static void EV13_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, -5); }

// Moves the leader and camera six cells along positive z.
static void EV14_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, 6); }

// Moves the leader and camera six cells along negative z.
static void EV15_TOUCH(void) { TretBranch_ShiftLeaderAndCameraByCells(0, -6); }

// Plays the exit cue and requests the exit number recorded by the trigger.
static void TretBranch_PlayCueAndSelectTouchedExit(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedExit;

  Engine_AudioPlayCue(0x7B);
  touchedExit = &state->touchedTrigger;
  Engine_EventRequestExit(*touchedExit);
}

// Reveal selects window-transition mode 9 and its blend/window parameters.
static void EV00_PSYNERGY(void) {
  struct DisplayTransitionState *transition;

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x0c04;

  transition = gDisplayTransition;
  {
    s16 *slot = &transition->mask;
    int value = 0x3f3f;
    *slot = value;
  }
  {
    s16 *slot = &transition->active;
    int value = 31;
    *slot = value;
  }
  {
    u16 *slot = &transition->value;
    int value = 10;
    *slot = value;
  }
}

// Returns the touch, talk, exit and Reveal events for this branch.
u8 *TretBranch_GetEvents(void) { return (u8 *)gTretBranch_SceneTable3; }

// Unused event hook: begins and immediately ends an event.
static void UNUSED TretBranch_RunEmptyEvent(void) {
  Engine_EventBegin();
  Engine_EventEnd();
}

// Unused empty hook; its original role is not established.
static void UNUSED State_RunEmptyHookFirst(void) {}

// Another unused empty hook with no established role.
static void UNUSED State_RunEmptyHook(void) {}

// Unused exchange callback: puts held/repeated keys in the transfer header.
static void UNUSED TretBranch_ExchangeInputPacket(void) {
  u16 *header = sTransferHeader;

  header[0] = gKeysHeld;
  header[1] = gKeysRepeat;
  SerialRuntime_PollAndTransfer(header, sTransferData);
}

// Initializes the backdrop transition and installs BG3's scanline scroll split.
// Returns 0 for the overlay initialization entry point.
s32 TretBranch_InitializeBackdropTransition(void) {
  u8 **base = (u8 **)&gEventWork;
  u8 *work;
  struct DisplayTransitionState *disp;
  s32 transitionOffset = 224;
  s32 *scene;

  // The event's transition request occupies the word at offset 0x1c0.
  transitionOffset <<= 1;
  work = *base;
  scene = (s32 *)(work + transitionOffset);
  transitionOffset -= 192;
  *scene = transitionOffset;

  DisplayTransition_InitializeBattleEffectState(9);

  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x0c04;

  // UB: slot 31 lies past scalar gEventWork. Its named load adds six
  // bytes in the native EN trial (0x6a to 0x70).
#ifdef UBFIX
  disp = gDisplayTransition;
#else
  disp = (struct DisplayTransitionState *)base[4];
#endif
  {
    s16 *slot = &disp->mask;
    int value = 0x3f3f;
    *slot = value;
  }
  {
    s16 *slot = &disp->active;
    int value = 31;
    *slot = value;
  }
  {
    u16 *slot = &disp->value;
    int value = 10;
    *slot = value;
  }

  TretBranch_StartBg3Split();
  return 0;
}

// Interrupt handler: selects BG3's horizontal offset for this scanline.
static void TretBranch_ApplyBg3ScrollAtScanline(void) {
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
static void TretBranch_UpdateBg3ScrollSplit(void) {
  union SceneCell *work = (union SceneCell *)(((u8 *)gMapWork[0]) + 260);
  s32 hofs;

  sSplitLine = 192 - work[1].h[1];
  sHofsAtOrAfterSplit = hofs = work[0].h[1];
  sHofsBeforeSplit = hofs - (gFrameCount >> 2);
}

// Installs the scroll interrupt and its per-frame split-position update.
static void TretBranch_StartBg3Split(void) {
  Runtime_SetIrqHandler(1, 0, TretBranch_ApplyBg3ScrollAtScanline);
  FieldScheduler_AddCallback(TretBranch_UpdateBg3ScrollSplit, 0xc80);
}

struct SceneEntrance gTretBranch_SceneTable0[] = {
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

u32 gTretBranch_SceneTable1[] = {
    SCENE_EXITS(46),      SCENE_EXIT(1, 45, 2), SCENE_EXIT(2, 45, 3),
    SCENE_EXIT(3, 45, 4), SCENE_EXIT(4, 45, 5), SCENE_EXIT(5, 45, 7),
    SCENE_EXIT(6, 45, 6), SCENE_EXIT(7, 45, 8), SCENE_EXIT(8, 45, 9),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTretBranch_SceneTable2[] = {
    { 0x5d, 0x6d, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gTretBranch_SceneTable3[] = {
    {0xa02, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 3, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 4, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 5, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 6, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {0xa02, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(TretBranch_PlayCueAndSelectTouchedExit)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 2), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV00_PSYNERGY)},
    {SCENE_EVENTS_END},
};

u16 gTretBranch_CellSteps[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
