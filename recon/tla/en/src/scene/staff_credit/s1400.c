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
   * gData_02003fc0), 4 of 593 instructions differ in tla-en. */
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
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)buffer;
      q->count = count + 1;
      *entry++ = 0x06001000;
      *entry = 0x84000400;
    }
    IRQ_UNLOCK(saved);
  }
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
    QueueIoWriteDelay2(SPLIT_CONSTANT(0x4000052), level);
    QueueIoWriteDelay2(SPLIT_CONSTANT(0x4000050), 0);
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
    QueueIoWriteDelay2(SPLIT_CONSTANT(0x4000052), level);
    QueueIoWriteDelay2(SPLIT_CONSTANT(0x4000050), 0);
  }
  Heap_Release(buffer);
  GameFlag_SetBit(0x101);
}
