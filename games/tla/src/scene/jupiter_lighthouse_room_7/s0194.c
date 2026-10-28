#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void GameFlag_SetBit();
s32 GameFlag_Test();
s32 Actor_GetById();

s32 Func_02000194(void) {
  s32 t1, t2, t3, t5, t7, t9, t11, t13, v5, v6, v7, v8;

  t1 = Value1(Actor_GetById, 18);
  v5 = t1;
  t2 = Value1(Actor_GetById, 19);
  v7 = 1;
  v6 = (FIELD_AT(v5, s32, 8) >> 20);
  v8 = t2;
  v7 = -v7;
  v5 = (FIELD_AT(v5, s32, 16) >> 20);
  if (v6 != 35)
    goto L_020081d6;
  if (v5 != 15)
    goto L_020081d6;
  t3 = Value1(GameFlag_Test, 0xa11);
  if (t3 != 0)
    goto L_020081d6;
  Call1(GameFlag_SetBit, 0xa10);
  v7 = 0;
L_020081d6:
  if (v6 != 31)
    goto L_020081f8;
  if (v5 != 16)
    goto L_020081f8;
  t5 = Value1(GameFlag_Test, 0xa13);
  if (t5 != 0)
    goto L_020081f8;
  Call1(GameFlag_SetBit, 0xa12);
  v7 = 2;
L_020081f8:
  if (v6 != 33)
    goto L_0200821a;
  if (v5 != 10)
    goto L_0200821a;
  t7 = Value1(GameFlag_Test, 0xa15);
  if (t7 != 0)
    goto L_0200821a;
  Call1(GameFlag_SetBit, 0xa14);
  v7 = 4;
L_0200821a:
  v6 = (FIELD_AT(v8, s32, 8) >> 20);
  v5 = (FIELD_AT(v8, s32, 16) >> 20);
  if (v6 != 35)
    goto L_02008244;
  if (v5 != 15)
    goto L_02008244;
  t9 = Value1(GameFlag_Test, 0xa10);
  if (t9 != 0)
    goto L_02008244;
  Call1(GameFlag_SetBit, 0xa11);
  v7 = 1;
L_02008244:
  if (v6 != 31)
    goto L_02008266;
  if (v5 != 16)
    goto L_02008266;
  t11 = Value1(GameFlag_Test, 0xa12);
  if (t11 != 0)
    goto L_02008266;
  Call1(GameFlag_SetBit, 0xa13);
  v7 = 3;
L_02008266:
  if (v6 != 33)
    goto L_02008288;
  if (v5 != 10)
    goto L_02008288;
  t13 = Value1(GameFlag_Test, 0xa14);
  if (t13 != 0)
    goto L_02008288;
  Call1(GameFlag_SetBit, 0xa15);
  v7 = 5;
L_02008288:
  return v7;
}
