#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];
extern u16 gData_02006e08;

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004da8(void);
void *Heap_AllocEwram(s32 size);
s32 Func_02004aa8(const void *source, void *destination);
void Func_02003c50(void);
void Func_02004c20(void);
void Func_02004ac8(void);
void Heap_Release(void *pointer);

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02003d44(s32 mode) {
  u8 *buffer;
  u32 blend;

  wait(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    Map_SetScreenShake(0x20000, 0x30000, 0x10000);
    Sound_Play(141);
  }
  Func_02004da8();
  buffer = Heap_AllocEwram(0x4000);
  Func_02004aa8(gWorldMapPackedTiles, buffer);
  Func_02004aa8(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  Task_Add(Func_02003c50, 0x480);
  Func_02004c20();
  Event_ResolvePendingActions(0);
  Sound_Play(246);
  gData_02006e08 = 0xe00;
  PUSH_PICTURE(0x3480);
  Event_Wait(2);
  gData_02006e08 = 0xd00;
  PUSH_PICTURE(0x2e80);
  Event_Wait(2);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x2880);
  Event_Wait(2);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x2280);
  Event_Wait(2);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x1c80);
  Event_Wait(2);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x1680);
  Event_Wait(2);
  gData_02006e08 = 0x800;
  PUSH_PICTURE(0x1080);
  Event_Wait(140);
  PUSH_PICTURE(0x1680);
  Event_Wait(4);
  PUSH_PICTURE(0x1c80);
  Event_Wait(4);
  PUSH_PICTURE(0x2280);
  Event_Wait(4);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x2880);
  Event_Wait(4);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x2e80);
  Event_Wait(4);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x3480);
  Event_Wait(4);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    /* FAKEMATCH(wrapper): Map_SetScreenShake called through Call3; as
     * Map_SetScreenShake(0x20000, 0x20000, 0x10000), 19 of 593 instructions
     * differ in tla-en. */
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Sound_Play(141);
    gData_02006e08 = 0xd00;
    Event_Wait(4);
    gData_02006e08 = 0xe00;
    Event_Wait(4);
    gData_02006e08 = 0xf00;
    Event_Wait(4);
    gData_02006e08 = 0x1000;
    Event_Wait(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    /* FAKEMATCH(temporary): the blend read into blend before each of the two
     * QueueIoWriteDelay2 calls; as QueueIoWriteDelay2((u32)&REG_BLDALPHA,
     * gData_02006e08) at both, 2 of 593 instructions differ in tla-en. */
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  } else {
    gData_02006e08 = 0xd00;
    Event_Wait(4);
    gData_02006e08 = 0xe00;
    Event_Wait(4);
    gData_02006e08 = 0xf00;
    Event_Wait(4);
    gData_02006e08 = 0x1000;
    Event_Wait(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  GameFlag_SetBit(0x101);
}
