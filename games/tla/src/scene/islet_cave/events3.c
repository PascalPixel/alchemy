#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020009f8();
void Func_02001234();
void GameFlag_SetBit();
struct Actor *Actor_GetById();
void wait();
void GameFlag_ClearBit();

void Func_020009f8();
void Func_02001234();
void GameFlag_ClearBit();
struct Actor *Actor_GetById();
void wait();
void GameFlag_SetBit();

void Func_02000aec(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(8);
  value1 = object0->x;
  object2 = Actor_GetById(8);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 8, 32, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(8);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_ClearBit, 0x280);
  return;
}

void Func_02000b4c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(8);
  value1 = object0->x;
  object2 = Actor_GetById(8);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 8, -32, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(8);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_SetBit, 0x280);
  return;
}

void Func_02000bb0(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(9);
  value1 = object0->x;
  object2 = Actor_GetById(9);
  value3 = object2->z;
  Value3((s32 (*)())Func_020009f8, 9, 112, 0);
  Value3((s32 (*)())Func_020009f8, 9, 96, 0);
  Value1((s32 (*)())wait, 2);
  object4 = Actor_GetById(9);
  ((void (*)())Func_02001234)((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                              (object4->x >> 20), ((value3 >> 20) - 1));
  Value6((s32 (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
         ((value3 >> 20) - 1));
  Value1((s32 (*)())GameFlag_ClearBit, 0x281);
  return;
}

void Func_02000c1c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(9);
  value1 = object0->x;
  object2 = Actor_GetById(9);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 9, -112, 0);
  Call3((void (*)())Func_020009f8, 9, -96, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(9);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_SetBit, 0x281);
  return;
}

void Func_02000c8c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(10);
  value1 = object0->x;
  object2 = Actor_GetById(10);
  value3 = object2->z;
  Value3((s32 (*)())Func_020009f8, 10, 80, 0);
  Value1((s32 (*)())wait, 2);
  object4 = Actor_GetById(10);
  ((void (*)())Func_02001234)((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                              (object4->x >> 20), ((value3 >> 20) - 1));
  Value6((s32 (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
         ((value3 >> 20) - 1));
  Value1((s32 (*)())GameFlag_SetBit, 0x282);
  return;
}

void Func_02000cf0(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(10);
  value1 = object0->x;
  object2 = Actor_GetById(10);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 10, -80, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(10);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_ClearBit, 0x282);
  return;
}

void Func_02000d54(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(11);
  value1 = object0->x;
  object2 = Actor_GetById(11);
  value3 = object2->z;
  Value3((s32 (*)())Func_020009f8, 11, 80, 0);
  Value1((s32 (*)())wait, 2);
  object4 = Actor_GetById(11);
  ((void (*)())Func_02001234)((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                              (object4->x >> 20), ((value3 >> 20) - 1));
  Value6((s32 (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
         ((value3 >> 20) - 1));
  Value1((s32 (*)())GameFlag_ClearBit, 0x283);
  return;
}

void Func_02000db8(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(11);
  value1 = object0->x;
  object2 = Actor_GetById(11);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 11, -80, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(11);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_ClearBit, 0x283);
  return;
}

void Func_02000e1c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(12);
  value1 = object0->x;
  object2 = Actor_GetById(12);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 12, 80, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(12);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_SetBit, 0x284);
  return;
}

void Func_02000e7c(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(12);
  value1 = object0->x;
  object2 = Actor_GetById(12);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 12, -80, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(12);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_ClearBit, 0x284);
  return;
}

void Func_02000ee0(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(13);
  value1 = object0->x;
  object2 = Actor_GetById(13);
  value3 = object2->z;
  Value3((s32 (*)())Func_020009f8, 13, 80, 0);
  Value1((s32 (*)())wait, 2);
  object4 = Actor_GetById(13);
  ((void (*)())Func_02001234)((value1 >> 20), ((value3 >> 20) - 1), 1, 3,
                              (object4->x >> 20), ((value3 >> 20) - 1));
  Value6((s32 (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
         ((value3 >> 20) - 1));
  Value1((s32 (*)())GameFlag_ClearBit, 0x285);
  return;
}

void Func_02000f44(void) {
  struct Actor *object0;
  s32 value1;
  struct Actor *object2;
  s32 value3;
  struct Actor *object4;

  object0 = Actor_GetById(13);
  value1 = object0->x;
  object2 = Actor_GetById(13);
  value3 = object2->z;
  Call3((void (*)())Func_020009f8, 13, -80, 0);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(13);
  Call6((void (*)())Func_02001234, (value1 >> 20), ((value3 >> 20) - 1), 1, 3,
        (object4->x >> 20), ((value3 >> 20) - 1));
  Call6((void (*)())Func_02001234, 0, 0, 1, 3, (value1 >> 20),
        ((value3 >> 20) - 1));
  Call1((void (*)())GameFlag_SetBit, 0x285);
  return;
}

void Func_02000fa8(void) {
  struct Actor *object0;
  s32 value1;
  s32 v5;
  struct Actor *object2;
  s32 v6;
  s32 v8;
  struct Actor *object3;

  object0 = Actor_GetById(14);
  value1 = object0->x;
  v5 = value1;
  object2 = Actor_GetById(14);
  v6 = 112;
  v8 = object2->z;
  v6 = -v6;
  v8 = (v8 >> 20);
  Call3((void (*)())Func_020009f8, 14, 0, v6);
  Call3((void (*)())Func_020009f8, 14, 0, v6);
  ((void (*)())Func_020009f8)(14, 0, -128);
  Call1((void (*)())wait, 2);
  object3 = Actor_GetById(14);
  v5 = (v5 >> 20);
  v5 = (v5 - 1);
  Call6((void (*)())Func_02001234, v5, v8, 3, 1, v5, (object3->z >> 20));
  Call6((void (*)())Func_02001234, 0, 0, 3, 1, v5, v8);
  Call1((void (*)())GameFlag_SetBit, 0x286);
  return;
}

void Func_02001034(void) {
  struct Actor *object0;
  s32 value1;
  s32 v5;
  struct Actor *object2;
  s32 value3;
  s32 v6;
  struct Actor *object4;

  object0 = Actor_GetById(14);
  value1 = object0->x;
  v5 = value1;
  object2 = Actor_GetById(14);
  value3 = object2->z;
  v6 = value3;
  Call3((void (*)())Func_020009f8, 14, 0, 112);
  Call3((void (*)())Func_020009f8, 14, 0, 112);
  Call3((void (*)())Func_020009f8, 14, 0, 128);
  Call1((void (*)())wait, 2);
  object4 = Actor_GetById(14);
  v5 = (v5 >> 20);
  v5 = (v5 - 1);
  v6 = (v6 >> 20);
  Call6((void (*)())Func_02001234, v5, v6, 3, 1, v5, (object4->z >> 20));
  Call6((void (*)())Func_02001234, 0, 0, 3, 1, v5, v6);
  Call1((void (*)())GameFlag_ClearBit, 0x286);
  return;
}
