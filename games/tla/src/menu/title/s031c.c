#include "types.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "ram_buffer.h"

extern s16 gData_020006dc;

void Func_02000490(void);
void Func_02000278(void);
void Title_LoadSprites(s32 unused);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

// The title screen appears: the logo and its sprite row are loaded, the
// blend fades the logo in over 17 steps and the screen transition opens.
void Func_0200031c(void) {
  s32 i;

  Func_02000490();
  Battle_WaitMode0(30);
  gData_020006dc = 0;
  Title_LoadSprites(0);
  Scheduler_AddOrUpdateCallback(Func_02000278, 0x480);
  IO_WRITE_QUEUE_PUSH(0x4000000, 0x1540, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000050, 0x2fce, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000054, 16, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x4000052, 0x1010, 0x20000);
  Battle_WaitMode0(120);
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes reload the queue and the interrupt register addresses on every
   * pass; a for or do/while loop keeps them from the pushes above (differ 90).
   */
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH(0x4000054, 16 - i, 0x20000);
  WaitFrames(3);
  if (++i <= 16)
    goto fade;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 60;
}
