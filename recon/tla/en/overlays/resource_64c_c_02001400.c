// Draft, differ 41 of 593: the staff credits fade: queues the world map
// palette and frames, then steps the blend through 0xe00 down to 0x800 and
// back. Left: a few literal-pool loads the native issues one instruction
// earlier, and mode 1 building its first two 0x20000 arguments apart.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "event_work.h"

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedFrames[];
extern u8 gWorldMapPalettes[];
extern u16 gData_02003fc0;

void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ff4(s32 a, s32 b, s32 c);
void Func_020020b4(s32 cue);
void Func_02002074(void);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Func_02001f54(void *source, void *destination);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200130c(void);
void Func_02002024(void);
void Func_02002084(s32 value);
void Func_02001f74(void);
void MemFree(void *pointer);

#define PUSH_FRAME(offset)                                                     \
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02001400(s32 mode) {
  volatile u16 *ime;
  u8 *buffer;

  WaitFrames(1);
  GameFlag_ClearBit(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Object_GetById(gPartyState.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    Func_02001ff4(0x20000, 0x30000, 0x10000);
    Func_020020b4(141);
  }
  Func_02002074();
  buffer = Runtime_BumpAllocateAlternatePool(0x4000);
  ime = &REG_IME;
  Func_02001f54(gWorldMapPackedTiles, buffer);
  Func_02001f54(gWorldMapPackedFrames, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                          0x80000010);
  IO_WRITE_QUEUE_PUSH_IME(ime, 0x06001000, (u32)buffer, 0x84000400);
  Scheduler_AddOrUpdateCallback(Func_0200130c, 0x480);
  Func_02002024();
  Func_02002084(0);
  Func_020020b4(246);
  gData_02003fc0 = 0xe00;
  PUSH_FRAME(0x3480);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0xd00;
  PUSH_FRAME(0x2e80);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0xc00;
  PUSH_FRAME(0x2880);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0xb00;
  PUSH_FRAME(0x2280);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0xa00;
  PUSH_FRAME(0x1c80);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0x900;
  PUSH_FRAME(0x1680);
  FieldEvent_WaitFrames(2);
  gData_02003fc0 = 0x800;
  PUSH_FRAME(0x1080);
  FieldEvent_WaitFrames(140);
  PUSH_FRAME(0x1680);
  FieldEvent_WaitFrames(4);
  PUSH_FRAME(0x1c80);
  FieldEvent_WaitFrames(4);
  PUSH_FRAME(0x2280);
  FieldEvent_WaitFrames(4);
  gData_02003fc0 = 0x900;
  PUSH_FRAME(0x2880);
  FieldEvent_WaitFrames(4);
  gData_02003fc0 = 0xa00;
  PUSH_FRAME(0x2e80);
  FieldEvent_WaitFrames(4);
  gData_02003fc0 = 0xb00;
  PUSH_FRAME(0x3480);
  FieldEvent_WaitFrames(4);
  gData_02003fc0 = 0xc00;
  PUSH_FRAME(0x3a80);
  if (mode == 1) {
    Func_02001ff4(0x20000, 0x20000, 0x10000);
    Func_020020b4(141);
    gData_02003fc0 = 0xd00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_WaitFrames(45);
    Scheduler_RemoveCallbackFar(Func_0200130c);
    Func_02001f74();
    QueueIoWriteDelay2(0x04000052, gData_02003fc0);
    QueueIoWriteDelay2(0x04000050, 0);
  } else {
    gData_02003fc0 = 0xd00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_WaitFrames(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_WaitFrames(45);
    Scheduler_RemoveCallbackFar(Func_0200130c);
    Func_02001f74();
    QueueIoWriteDelay2(0x04000052, gData_02003fc0);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  MemFree(buffer);
  GameFlag_SetBit(0x101);
}
