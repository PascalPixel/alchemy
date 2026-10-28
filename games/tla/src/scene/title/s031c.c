#include "types.h"
#include "task.h"
#include "io_write_queue.h"
#include "ram_buffer.h"

extern s16 gData_020006dc;

void Func_02000490(void);
void Func_02000278(void);
void Title_LoadSprites(s32 unused);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// The title screen appears: the logo and its sprite row are loaded, the
/// blend fades the logo in over 17 steps and the screen transition opens.
void Func_0200031c(void) {
  s32 i;

  Func_02000490();
  FieldEvent_Wait(30);
  gData_020006dc = 0;
  Title_LoadSprites(0);
  Task_Add(Func_02000278, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                          BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 16), IO_WRITE_16);
  FieldEvent_Wait(120);
  /* FAKEMATCH: a goto loop carries no loop notes, so each pass reloads the
   * queue and interrupt register addresses; as a for or do/while loop, 90
   * instructions differ in tla-en. */
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - i, IO_WRITE_16);
  wait(3);
  if (++i <= 16)
    goto fade;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 60;
}
