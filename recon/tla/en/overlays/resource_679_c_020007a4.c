// Draft, differ 41 of 188: an Apojii Islands scene inside an event: the actor
// takes Func_02000780 as its update, cues 78 and 21, a sparkle setup
// (Func_02001ad8), four flashes of random strength, then the screen blends
// in over six steps while the palette cycle gives way to Func_020006bc and
// the cell markers are rebuilt (Func_020017f8); flags 0x95f and 0x95e are
// set and the camera pans through three points before cue 77. Left: the native builds 0x7fff after the random call returns, builds the flash's 0x10000 around the second argument, and its literal pool holds one word fewer, which shifts every pc offset.
#include "types.h"
#include "object_runtime.h"
#include "io_write_queue.h"
#include "system/io_reg.h"

struct ObjectRuntime *Object_GetById(u32 id);
void Func_02001cc4(void);
void Func_02001dcc(s32 skip);
void Func_02000780(void);
void Func_02001e2c(s32 cue);
void WaitFrames(s32 frames);
void Func_02001ad8(void);
void Func_02001ba0(s32 a);
void Func_02001dec(s32 a);
u16 Random16Far(void);
void Func_02001da4(s32 a, s32 b);
void Func_02001dac(s32 frames);
void Func_02001de4(void);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02000540(void);
void Func_020006bc(void);
void Func_020017bc(void);
void Func_020017f8(u32 base, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
void Func_02001d7c(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001d6c(s32 a, s32 b, s32 c);
void Func_02001d8c(void);
void Func_02001d94(s32 cue);

void Func_020007a4(s32 unused, s32 id) {
  struct ObjectRuntime *actor = Object_GetById(id);
  s32 i;

  Func_02001cc4();
  Func_02001dcc(0);
  actor->update = Func_02000780;
  Func_02001e2c(78);
  WaitFrames(30);
  Func_02001e2c(21);
  Func_02001ad8();
  Func_02001ba0(17);
  Func_02001dec(170);
  i = 3;
flash:
  Func_02001da4(0x7fff & Random16Far(), 0);
  Func_02001dac(15);
  WaitFrames(10);
  Func_02001da4(0x10000, 0);
  Func_02001dac(10);
  i--;
  WaitFrames(10);
  if (i >= 0)
    goto flash;
  Func_02001de4();
  IO_WRITE_QUEUE_PUSH(0x04000050, 0xf00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000, 0x20000);
  WaitFrames(1);
  Scheduler_RemoveCallbackFar(Func_02000540);
  Scheduler_AddOrUpdateCallback(Func_020006bc, 0x480);
  Func_020017bc();
  Func_020017f8(33, 0, 0);
  i = 1;
blend:
  IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
  i++;
  WaitFrames(4);
  if (i <= 6)
    goto blend;
  GameFlag_SetBit(0x95f);
  GameFlag_SetBit(0x95e);
  Func_02001d7c(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0x1680000, -1, 0x1480000, 1);
  Func_02001d6c(16, 256, 20);
  Func_02001d8c();
  Camera_MoveWithinMapBounds(0x1b80000, -1, 0x1780000, 1);
  Func_02001d8c();
  Camera_MoveWithinMapBounds(0x1d80000, -1, 0x1980000, 1);
  Func_02001d94(77);
}
