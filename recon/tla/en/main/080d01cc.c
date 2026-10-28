// Under -mthumb-slow-loads (2026-10-26): differ 32 of 352 in tla-en,
// instruction order and register choice. No empty asm before any one statement
// lowers it.
// Draft, differ 42 of 352 (2026-10-18), rewritten from ☀️'s
// ScreenTransition_Start in place of a draft whose headers no longer exist:
// this game's map work (dispcnt at 0x14, the centre lines at 0x104), its
// callbacks and priorities, and the IO_WRITE_QUEUE_PUSH of REG_DISPCNT after
// each mode. Left: in each of the five queue pushes the native starts
// REG_DISPCNT's address after the count's store and adds the entry's base
// after the map's dispcnt load (two reordered rows each), and the switch's
// opening schedules its mask and the frames copy in another order. Tried:
// the value's operands swapped (50), the push written out with q->count
// (42) and with the entry taken after the count's store (106).
#include "types.h"
#include "task.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "irq.h"
#include "ram_buffer.h"
#include "display_transition.h"

// The map work as the window transitions see it.
struct TransitionMapWork {
  u8 unk000[0x14];
  u16 dispcnt;
  u8 unk016[0x104 - 0x16];
  u16 firstLine;
  u16 secondLine;
};

void WaitFrames(s32 frames);
void Fade_FromBlack(s32 vblanks);
void Fade_ToBlack(s32 vblanks);
void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void BattleEffect_SetPrimaryBufferValue(s32 value);
struct ScreenTransitionState *Func_080d019c(void);
void Func_080cf78c(void);

// Fades the screen in over frames frames, as ☀️'s ScreenTransition_Start.
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct TransitionMapWork *display;
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  display = (struct TransitionMapWork *)Ram_HeapSlots->mapWork;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_ToBlack(0);
    Fade_FromBlack(frames);
    WaitFrames(1);
    break;
  case 1:
    BattleEffect_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
    BattleEffect_StartBufferInterpolation(frames);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    BattleEffect_SetPrimaryBufferValue(0);
    return;
  case 2: {
    struct ScreenTransitionState *state = Func_080d019c();
    state->mode = variant;
    state->value = 0;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
    Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct ScreenTransitionState *state = Func_080d019c();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(15);
    WaitFrames(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct ScreenTransitionState *state;

    display = (struct TransitionMapWork *)Ram_HeapSlots->mapWork;
    state = Func_080d019c();
    display->firstLine = 80;
    display->secondLine = 80;
    WaitFrames(1);
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      0x20000);
}
