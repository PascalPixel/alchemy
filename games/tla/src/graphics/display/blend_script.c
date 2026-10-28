#include "types.h"
#include "dma.h"
#include "task.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_reg.h"

void DisplayBlend_RunScript(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct BlendScriptState *state;
  u16 *cursor;
  u32 command;
  u32 value;
  u16 test;

  state = &work->blend;
  if (state->script == 0)
    return;
  if (state->paused != 0)
    return;

again:
  value = state->delay;
  test = value;
  if (test != 0)
    goto count_down;

  cursor = state->cursor;
  command = *cursor;
  cursor++;
  if (command == 0xffff) {
    state->cursor = state->script;
    goto again;
  }

  if ((command & 0xff00) == 0xfe00) {
    value = command & 0xff;
    if (value == 0xff)
      return;
    state->cursor = state->script + value * 2;
    goto again;
  }

  if ((command & 0xf000) == 0x3000) {
    REG_BLDCNT = command;
    work->blendControl = command;
    state->cursor++;
    goto again;
  }

  if ((work->blendControl & 0xc0) == 0x40)
    REG_BLDALPHA = command;
  else
    REG_BLDY = command;
  state->delay = cursor[0];
  state->cursor += 2;
  goto again;

count_down:
  state->delay = value + 0xffff;
}

void DisplayBlend_StartScript(u16 *script) {
  struct BlendScriptState *state;
  s32 started;
  volatile u32 zero;
  u32 first;

  started = 0;
  zero = 0;
  state = &Ram_HeapSlots->mapWork->blend;
  Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
  first = *script;
  /* FAKEMATCH: the plain order builds 0xffff before loading the first command.
   */
  __asm__("");
  if (first != 0xffff) {
    state->script = script;
    state->cursor = script;
    /* FAKEMATCH: the ARM9 scheduling would clear the delay and pause before
     * storing the pointers. */
    __asm__("");
    state->delay = 0;
    state->paused = 0;
    started = 1;
  }
  if (started)
    Task_Add((s32)(DisplayBlend_RunScript), TASK_PASS_VBLANK);
}

void DisplayBlend_EnableRunScript(void) {
  Task_SuspendAll((u32)DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void) {
  Task_ResumeAll((u32)DisplayBlend_RunScript);
}
