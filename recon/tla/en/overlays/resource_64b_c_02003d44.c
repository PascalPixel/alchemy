// Near miss, differ 6 of 593 (tla-en, 2026-10-30): the world map's fade:
// queues the world map palette for mode and its pictures, then steps the blend
// through 0xe00 down to 0x800 and back. Overlay 64c has the same code (its
// Func_02001400, which includes this file). Mode 1's Func_02004b78 goes
// through Call3, as the native builds its two 0x20000 arguments apart.
// Measured steps (was 35, then 16, 14 at the start of this pass): the plain
// IO_WRITE_QUEUE_PUSH in place of IO_WRITE_QUEUE_PUSH_IME with an ime local
// loads the queue's address before IME's, as the native does (14 to 12);
// gData_02006e08 read into blend before each QueueIoWriteDelay2 puts its load
// between 0x04000052's halves (12 to 8, tagged); Func_02004aa8 typed s32, as
// its veneer's target Resource_DecodeType01 returns (graphics/resource.h),
// puts the pool load after the pictures' add (8 to 6).
// Left: the second push (the frames' tiles, value buffer itself): the native
// computes the entry's address before the count's increment and stores the
// count after the buffer word. Here the scheduler gives the increment priority
// 8 (the count store frees r2 for 0x06001000) over the address's 7, and the
// count and entry stores do not alias, so no statement order reaches it:
// measured at 8, before the s32 return, the count stored after the value
// changed nothing and after all three words gave 22; at 6, the macro with
// q->count, a u16 or u32 count, the entry taken after the increment, or
// indexed stores gave 34 to 134 (all pushes). Scheduler order. The permuter
// (4 minutes, at 8) found only a volatile return type on Func_02004aa8.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "call.h"

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];
extern u16 gData_02006e08;

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004b78(s32 a, s32 b, s32 c);
void Func_02004e30(s32 cue);
void Func_02004da8(void);
void *Heap_AllocEwram(s32 size);
s32 Func_02004aa8(const void *source, void *destination);
s32 Task_Add(void (*func)(void), s32 order);
void Task_RemoveFar(void (*func)(void));
void Func_02003c50(void);
void Func_02004c20(void);
void Func_02004db8(s32 value);
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
    Func_02004b78(0x20000, 0x30000, 0x10000);
    Func_02004e30(141);
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
  Func_02004db8(0);
  Func_02004e30(246);
  gData_02006e08 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    /* FAKEMATCH(wrapper): Func_02004b78 called through Call3; as
     * Func_02004b78(0x20000, 0x20000, 0x10000), 25 of 593 instructions
     * differ in tla-en. */
    Call3((void (*)())Func_02004b78, 0x20000, 0x20000, 0x10000);
    Func_02004e30(141);
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    /* FAKEMATCH(temporary): the blend read into blend before each of the two
     * QueueIoWriteDelay2 calls; as QueueIoWriteDelay2((u32)&REG_BLDALPHA,
     * gData_02006e08) at both, 10 of 593 instructions differ in tla-en. */
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  } else {
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  GameFlag_SetBit(0x101);
}
