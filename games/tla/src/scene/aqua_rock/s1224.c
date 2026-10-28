#include "local.h"

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void Func_02002a28(s32 frames);
void Func_02002968(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002a00(s32 a, s32 b);
void Func_02002a10(void);
void Func_02000b10(struct Actor *object);
void Func_02000b58(struct Actor *object);
void Func_02001d98(struct Actor *object);

/// Sets flag 0xa69 after four rounds of a random colour value, runs the
/// blend scripts, then moves the camera to object 15, starts its update and
/// plays cue 623 six times.
void Func_02001224(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
  Func_02002a48(0);
  object->update = (void (*)(void))Func_02000b10;
  WaitFrames(30);
  Func_02002834(8);
  Func_02002a70(220);
  for (i = 3; i >= 0; i--) {
    Func_02002a20(Random16Far() & 0x7fff, 0);
    Func_02002a28(15);
    WaitFrames(10);
    Func_02002a20(0x10000, 0);
    Func_02002a28(10);
    WaitFrames(10);
  }
  Func_02002a20(0x7fff, 0);
  Func_02002a28(20);
  FieldEvent_WaitFrames(20);
  GameFlag_SetBit(0xa69);
  Func_02002960(15, 48, 1, 1, 16, 48);
  Func_02002960(17, 50, 1, 1, 16, 50);
  object->update = (void (*)(void))Func_02000b58;
  Func_020025a8(8);
  Func_02002968(64, 68, 5, 13, 14, 94);
  Func_02002960(64, 68, 5, 8, 14, 34);
  FieldEvent_WaitFrames(30);
  Func_02002a20_(0x10000, 0);
  Func_02002a28(30);
  FieldEvent_WaitFrames(15);
  Func_02002a00(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(0xf80000, 0xffe00000, 0x2f80000, 1);
  Func_02002a10();
  Func_020029d8(15, 0x1080000, 0x3180000);
  Actor_SetSingleChildFlags(Actor_GetById(15), 0);
  Actor_GetById(15)->collisionFlags = 0;
  Actor_GetById(15)->priorityFlags |= 2;
  Actor_GetById(15)->update = (void (*)(void))Func_02001d98;
  FieldEvent_WaitFrames(30);
  for (i = 5; i >= 0; i--) {
    Func_02002a70(623);
    WaitFrames(10);
  }
  FieldEvent_WaitFrames(30);
  Func_020029a0();
}
