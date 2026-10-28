#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Task_Add();
s32 Func_020029fc();
s32 Actor_GetById();
s32 Actor_SetSingleChildFlags();
s32 Actor_SetSpritePriority();
s32 Func_02002ba4();
s32 Func_02002bac();
s32 GameFlag_Test();
s32 Actor_SetActionCallback();
s32 Func_02002a9c();
s32 Func_0200060c();
extern u8 Func_02000a80[];

s32 Func_02000b18(void) {
  s32 t7;
  s32 t8;
  s32 t12;
  s32 t19;
  s32 t23;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
  Call2(Task_Add, (s32)Func_02000a80, 0xc80);
  Call1(Func_020029fc, 8);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 10), 0);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 11), 0);
  Call2(Actor_SetSpritePriority, 13, 3);
  t7 = Value1(Actor_GetById, 23);
  FIELD_AT(t7, s32, 24) = 0x8000;
  t8 = Value1(Actor_GetById, 23);
  FIELD_AT(t8, s32, 28) = 0x8000;
  Func_02002ba4();
  {
    s32 first = 8;
    s32 second = 9;
    /* FAKEMATCH: ⚓️ loads the second and third arguments before the first; the
     * plain call loads them in order (one instruction moved). */
    __asm__ volatile("" : "+l"(first), "+l"(second));
    Call3(Func_02002bac, 0, first, second);
  }
  Call1(Func_020029fc, 3);
  t12 = Value1(GameFlag_Test, 0x97f);
  if (t12 != 0) {
    Call2(Actor_SetSpritePriority, 20, 2);
    Call2(Actor_SetSpritePriority, 21, 3);
    Call2(Actor_SetSpritePriority, 22, 3);
    Call2(Actor_SetSpritePriority, 19, 3);
    Call2(Actor_SetActionCallback, Value1(Actor_GetById, 18), 9);
    t19 = Value1(GameFlag_Test, 0x973);
    if (t19 != 0) {
      Call3(Func_02002a9c, 32, 0, 0);
      Call3(Func_02002a9c, 33, 0, 0);
      Call3(Func_02002a9c, 34, 0, 0);
    }
  }
  t23 = Value1(GameFlag_Test, 0xa74);
  if (t23 != 0) {
    Func_0200060c();
  }
  return 0;
}
