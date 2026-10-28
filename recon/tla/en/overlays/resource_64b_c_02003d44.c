// Draft, differ 16 of 593 (was 35): the world map's fade: queues the world
// map palette for mode and its pictures, then steps the blend through 0xe00 down
// to 0x800 and back. Overlay 64c has the same code (its Func_02001400). Mode
// 1's Func_02004b78 goes through Call3, as the native builds its two 0x20000
// arguments apart. Left: instruction order only, all of it a load the native
// issues between the halves of a split constant or in another order: the
// event work's load inside 0x1b4, gData_02006e08's inside 0x04000052 at the
// two QueueIoWriteDelay2 calls, the two pool loads after the frames are
// unpacked, and the queue count's store in the first push.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "event_work.h"
#include "lib/call.h"

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];
extern u16 gData_02006e08;

void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004b78(s32 a, s32 b, s32 c);
void Func_02004e30(s32 cue);
void Func_02004da8(void);
void *Heap_AllocEwram(s32 size);
void Func_02004aa8(void *source, void *destination);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02003c50(void);
void Func_02004c20(void);
void Func_02004db8(s32 value);
void Func_02004ac8(void);
void Heap_Release(void *pointer);

#define PUSH_PICTURE(offset)                                                     \
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02003d44(s32 mode) {
  volatile u16 *ime;
  u8 *buffer;

  WaitFrames(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_GetById(gAdventure.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    Func_02004b78(0x20000, 0x30000, 0x10000);
    Func_02004e30(141);
  }
  Func_02004da8();
  buffer = Heap_AllocEwram(0x4000);
  ime = &REG_IME;
  Func_02004aa8(gWorldMapPackedTiles, buffer);
  Func_02004aa8(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                          0x80000010);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06001000, (u32)buffer, 0x84000400);
  Task_Add(Func_02003c50, 0x480);
  Func_02004c20();
  Func_02004db8(0);
  Func_02004e30(246);
  gData_02006e08 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_WaitFrames(2);
  gData_02006e08 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_WaitFrames(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_WaitFrames(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_WaitFrames(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_WaitFrames(4);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_WaitFrames(4);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_WaitFrames(4);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_WaitFrames(4);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    Call3((void (*)())Func_02004b78, 0x20000, 0x20000, 0x10000);
    Func_02004e30(141);
    gData_02006e08 = 0xd00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0xe00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0xf00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0x1000;
    FieldEvent_WaitFrames(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    QueueIoWriteDelay2(0x04000052, gData_02006e08);
    QueueIoWriteDelay2(0x04000050, 0);
  } else {
    gData_02006e08 = 0xd00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0xe00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0xf00;
    FieldEvent_WaitFrames(4);
    gData_02006e08 = 0x1000;
    FieldEvent_WaitFrames(45);
    Task_RemoveFar(Func_02003c50);
    Func_02004ac8();
    QueueIoWriteDelay2(0x04000052, gData_02006e08);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  Heap_Release(buffer);
  GameFlag_SetBit(0x101);
}
