// Near miss, differ 6 of 593 (tla-en, 2026-10-30): instruction order only,
// in the second push. The native puts the queue count's increment and store
// after the entry address and the first entry word; sched2 ranks the
// increment higher here (its chain runs through r2's reuse for 0x06001000),
// and storing the entry first or reading the count as q->count changes
// nothing.
// The world map's fade, overlay 64b's Func_02003d44 (resource_64b_c_02003d44.c)
// instruction for instruction under this overlay's names: queues the world
// map palette for mode and its pictures, then steps the blend through 0xe00
// down to 0x800 and back. Written out here rather than included, as it now
// differs from that draft in three ways, together worth 8 instructions here
// (from 14): Func_02001f54, the veneer to Resource_DecodeType01, returns its
// s32 size as that function does (declared void, the pictures' address loads
// before their destination: 2 more differ); the pushes take IME through
// IO_WRITE_QUEUE_PUSH's own lock rather than a held ime pointer (which loaded
// IME's address before the queue's); and the blend level reaches
// QueueIoWriteDelay2 through a u32 local. Mode 1's Func_02001ff4 goes through
// Call3, as the native builds its two 0x20000 arguments apart.
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
extern u16 gData_02003fc0;

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ff4(s32 a, s32 b, s32 c);
void Func_020020b4(s32 cue);
void Func_02002074(void);
void *Heap_AllocEwram(s32 size);
s32 Func_02001f54(const void *source, void *destination);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_0200130c(void);
void Func_02002024(void);
void Func_02002084(s32 value);
void Func_02001f74(void);
void Heap_Release(void *pointer);

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02001400(s32 mode) {
  u8 *buffer;
  u32 level;

  /* FAKEMATCH(temporary): the blend level is read into level, a u32 local,
   * before each QueueIoWriteDelay2 call; as QueueIoWriteDelay2(0x04000052,
   * gData_02003fc0), 10 of 593 instructions differ in tla-en. */
  wait(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    Func_02001ff4(0x20000, 0x30000, 0x10000);
    Func_020020b4(141);
  }
  Func_02002074();
  buffer = Heap_AllocEwram(0x4000);
  Func_02001f54(gWorldMapPackedTiles, buffer);
  Func_02001f54(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  Task_Add(Func_0200130c, 0x480);
  Func_02002024();
  Func_02002084(0);
  Func_020020b4(246);
  gData_02003fc0 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    Call3((void (*)())Func_02001ff4, 0x20000, 0x20000, 0x10000);
    Func_020020b4(141);
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    Task_RemoveFar(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  } else {
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    Task_RemoveFar(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  Heap_Release(buffer);
  GameFlag_SetBit(0x101);
}
