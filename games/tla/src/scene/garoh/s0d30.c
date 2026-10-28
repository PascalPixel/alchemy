#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_WaitTransitionFrames();
void Func_02000280();
void Func_02000390();
void Func_020003d8();
void Func_02005184();
void Func_020052cc();
void Func_020052d4();
void Func_0200535c();
void Func_02005364();
void Func_0200538c();
s32 GameFlag_Test();
void Task_Add();
void Func_020002a4();
void Func_02000a68();

void Func_02000d30(void) {
  s32 t4;

  Value2((s32 (*)())Task_Add, (s32)Func_02000a68, 0x480);
  Func_0200535c();
  Call3(Func_02005364, 0, 10, 11);
  t4 = Value1(GameFlag_Test, 0x310);
  if (t4 == 0)
    goto L_02008da6;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 12)
    goto L_02008d72;
  Call2(Func_020052cc, 0x2048c9, 1);
  goto L_02008d7e;
L_02008d72:
  Call2(Func_020052cc, 0x10007, 1);
L_02008d7e:
  Call1(Func_020052d4, 1);
  Event_WaitTransitionFrames();
  Func_0200538c();
  Func_020003d8();
  Call2(Func_020052cc, 0x10000, 1);
  Call2(Task_Add, (s32)Func_020002a4, 0xc80);
  goto L_02008dea;
L_02008da6:
  Call6(Func_02005184, 20, 7, 9, 7, 5, 5);
  Call6(Func_02005184, 20, 71, 9, 71, 5, 5);
  Call6(Func_02005184, 84, 7, 73, 7, 5, 5);
  Func_02000390();
  Call2(Func_020052cc, 0x10000, 1);
  Func_02000280();
L_02008dea:
}
