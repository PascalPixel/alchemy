// Draft, differ 22 of 208 (2026-10-18), rewritten from ☀️'s
// DisplayTransition_Finish with this game's callbacks, priorities and
// active word. Mode 4's zero is one variable shared by both branches (the
// native keeps it in r7 rather than reusing the variant known to be 0;
// differ 132 without it). Left: the literal-pool loads of the byte and
// halfword constants (1, 0 and 32) issue one instruction later than the
// native's, which moves the pools and every branch past them by 2 or 4.
#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "irq.h"
#include "display_transition.h"

void WaitFrames(s32 frames);
void Fade_FromBlack(s32 frames);
void Fade_ToBlack(s32 frames);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 index);
void BattleFx_StartBufferInterpolation(s32 frames);
struct DisplayTransitionState *Func_080d019c(void);
void Func_080cf78c(void);

// Fades the screen out over frames frames, as ☀️'s DisplayTransition_Finish.
void DisplayTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_FromBlack(0);
    Fade_ToBlack(frames);
    break;
  case 1:
    BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
    BattleFx_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct DisplayTransitionState *state = Func_080d019c();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Scheduler_AddOrUpdateCallback(Func_080cf78c, 0x480);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0xc76);
    WaitFrames(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct DisplayTransitionState *state = Func_080d019c();
    state->mode = variant;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(0);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0x480);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct DisplayTransitionState *state = Func_080d019c();
    s32 zero = 0;

    ((u8 *)state)[0x53f] = 1;
    if (variant == 0) {
      Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0x480);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    } else {
      Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0x480);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    }
    break;
  }
  }
}
