#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "irq.h"
#include "display_transition.h"

void WaitFrames(s32 frames);
void Fade_FromBlack(s32 vblanks);
void Fade_ToBlack(s32 vblanks);
void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 index);
void BattleEffect_StartBufferInterpolation(s32 frames);
struct DisplayTransitionState *Func_080d019c(void);
void Func_080cf78c(void);

/// Fades the screen out over frames frames, as The Broken Seal's
/// DisplayTransition_Finish does, with this game's callbacks and priorities.
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
    BattleEffect_ApplyColorToTargetBuffer(0x8000, 0);
    BattleEffect_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct DisplayTransitionState *state = Func_080d019c();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
    Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
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
    Task_Add(DisplayTransition_UpdateDissolve, TASK_PASS_VBLANK);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct DisplayTransitionState *state = Func_080d019c();
    s32 zero = 0;

    /* FAKEMATCH(temporary): one zero both branches store, kept in r7; as the
     * constant 0 in each store, 126 of 208 instructions differ in tla-en. */
    ((u8 *)state)[0x53f] = 1;
    if (variant == 0) {
      Task_Add(DisplayTransition_Update, TASK_PASS_VBLANK);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    } else {
      Task_Add(DisplayTransition_UpdateFromCentre, TASK_PASS_VBLANK);
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
