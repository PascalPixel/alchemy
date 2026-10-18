// Draft of DisplayTransition_Start: differ 280 of 352 (tla-en, 2026-10-18).
// The Broken Seal's function of the same name
// (games/tbs/src/object/object.c), ported: it fades the screen in over frames
// frames by the kind in the mode's high byte, and queues the map's display
// control. QUEUE_DISPLAY_CONTROL is IO_WRITE_QUEUE_PUSH on REG_DISPCNT again
// (an empty stand-in had left the draft at 369).
//
// Left: other. The Lost Age's transition state is larger (its fields from
// 0x528, allocated by Func_080d019c), the window kind also sets a halfword
// from REG_WINOUT's high byte, and its scanline callbacks and keys differ, as
// in DisplayTransition_Finish (0x080d0520).

#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "scene.h"
#include "ram_buffer.h"
#include "display_transition.h"
#define QUEUE_DISPLAY_CONTROL(value)                                           \
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, value, 0x20000)

void DisplayTransition_Start(s32 mode, s32 frames) {
  void **work;
  struct DisplayTransitionWindow *display;
  s32 value;
  s32 kind;

  /* FAKEMATCH: the one-pass IME scopes and count-store spelling in
     QUEUE_DISPLAY_CONTROL keep the saved IME move before disabling IME
     and the final queue/IME loads in native order. The ordinary block
     and member store keep 708 bytes but change 24 instruction bytes in EN;
     both spellings store the count as a halfword. The word prefix of
     the map-work view preserves its existing load/address ordering. */
  kind = (mode >> 8) & 0xff;
  work = (void **)Ram_HeapSlots->mapWork;
  display = *(struct DisplayTransitionWindow **)work;
  value = mode & 0xff;
  switch (kind) {
  case 0:
    Fade_ToBlack(0);
    Fade_FromBlack(frames);
    WaitFrames(1);
    break;
  case 1:
    BattleFx_ApplyColorToSourceBuffer(0x8000, *(u16 *)0x05000000);
    BattleFx_StartBufferInterpolation(frames);
    WaitFrames(1);
    QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
    BattleFx_SetPrimaryBufferValue(0);
    return;
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 0;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateScanlineTable),
                                  0xc80);
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_StartWindowHBlankDma), 0x480);
    WaitFrames(1);
    QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = value;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(15);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0xc80);
    QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct DisplayTransitionState *state;

    display = (struct DisplayTransitionWindow *)*work;
    state = DisplayTransition_AllocateAndClearState();
    display->firstLine = 80;
    display->secondLine = 80;
    WaitFrames(1);
    if (value == 0) {
      Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_Update), 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFromCentre),
                                    0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
}
