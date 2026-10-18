// Draft of DisplayTransition_Finish: differ 194 of 208 (tla-en, 2026-10-18).
// The Broken Seal's function of the same name
// (games/tbs/src/object/object.c), ported unchanged: it fades the screen out
// over frames frames, by the kind in the mode's high byte.
//
// Left: other. The Lost Age's transition state is larger (its fields from
// 0x528, allocated by Func_080d019c), the window kind also sets a halfword
// from REG_WINOUT's high byte with 1, its scanline callbacks are
// Func_080cf78c and Func_080cf6fc at other keys (0x480, 0xc76), and its
// pool constants differ; the stores and calls shift with them.

#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "scene.h"
#include "ram_buffer.h"
#include "display_transition.h"
#define QUEUE_DISPLAY_CONTROL(value)

void DisplayTransition_Finish(s32 mode, s32 frames) {
  s32 value;
  s32 kind;

  kind = (mode >> 8) & 0xff;
  value = mode & 0xff;
  switch (kind) {
  case 0:
    Fade_FromBlack(0);
    Fade_ToBlack(frames);
    break;
  case 1:
    BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
    BattleFx_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateScanlineTable),
                                  0xc80);
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_StartWindowHBlankDma), 0x480);
    WaitFrames(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(0);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0xc80);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    if (value == 0) {
      Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_Update), 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFromCentre),
                                    0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
}
