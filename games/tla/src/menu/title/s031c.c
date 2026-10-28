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
  IO_WRITE_QUEUE_PUSH(0x4000000, 0x1540, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000050, 0x2fce, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000054, 16, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000052, 0x1010, 0x20000);
  FieldEvent_Wait(120);
  /* FAKEMATCH: a goto loop carries no loop notes, so each pass reloads the
   * queue and interrupt register addresses; as a for or do/while loop, 90
   * instructions differ in tla-en. */
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH(0x4000054, 16 - i, 0x20000);
  wait(3);
  if (++i <= 16)
    goto fade;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 60;
}
