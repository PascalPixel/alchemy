#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "edition.h"
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02000a94();
s32 Func_02000b8c();
s32 Func_02000b24();
s32 ObjectMotion_SetPositionAndReset();
s32 Func_02000b4c();
s32 Camera_MoveWithinMapBounds();
s32 Func_02000b7c();
s32 FieldEvent_WaitTicks();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000b34();
s32 Func_02000b5c();
s32 Motion_SetModeAndWaitAnimation();
s32 ObjectMotion_Launch();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_SetActionVariant();
s32 Camera_FollowActor();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Object_SetModeById();
s32 ObjectMotion_CommitPositionAndActivate();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 Object_LinkObjectAndSetCallback();
s32 Object_RefreshSelectorById();
s32 ObjectMotion_EnableActionAndResetMotion();
s32 Func_02000aec();
s32 Func_02000bdc();
s32 Func_02000a9c();
extern u8 MsgAlhafraInteriors120[];
extern u8 gData_02000be4[];

void Func_02000738(void) {
  s32 t0;
  s32 t1;

  t0 = Value1(GameFlag_Test, 0x903);
  if (t0 != 0) {
  } else {
#if (LANGUAGE != LANGUAGE_JAPANESE) /* The Japanese edition does not check 0x200. */
    t1 = Value1(GameFlag_Test, 0x200);
    if (t1 == 0) {
    } else
#endif
    {
      Call1(GameFlag_SetBit, 0x903);
      Func_02000a94();
      Call1(Func_02000b8c, 0);
      Call1(Func_02000b24, (s32)MsgAlhafraInteriors120);
      Call3(ObjectMotion_SetPositionAndReset, 4, 0x1f8, 0x148);
      Call2(Func_02000b4c, 4, 0x8000);
      Call4(Camera_MoveWithinMapBounds, 0x1980000, -1, 0x1480000, 1);
      Func_02000b7c();
      Call1(FieldEvent_WaitTicks, 0xa);
      Call2(Motion_SetVarCbAndRefresh, 0x1f, 2);
      Call1(FieldEvent_WaitTicks, 5);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Func_02000b5c(0x20, 0x100, 0x1e);
      Call3(Func_02000b34, 0x20, 0, 5);
      {
        s32 b = 0x109, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Call3(Func_02000b5c, 0x1f, b, c);
      }
      Call3(Func_02000b34, 0x1f, 0, 5);
      Func_02000b5c(0x20, 0x102, 0x1e);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1f, 3);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call3(ObjectMotion_Launch, 0x20, 6, 0xf);
      Call3(ObjectMotion_Launch, 0x20, 6, 0x17);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1f, 3);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Func_02000b5c(0x20, 0x109, 0x1e);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Motion_SetVarCbAndRefresh, 0x1f, 2);
      Call1(FieldEvent_WaitTicks, 5);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x20, 3);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Func_02000b4c, 0x1f, 0xd000);
      Call1(FieldEvent_WaitTicks, 0xf);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x20, 3);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Func_02000b4c, 0x1f, 0x3000);
      Call1(FieldEvent_WaitTicks, 0xf);
      Call2(Motion_SetModeAndWaitAnimation, 0x1f, 3);
      Call1(FieldEvent_WaitTicks, 0x14);
      Call2(Motion_SetModeAndWaitAnimation, 0x20, 3);
      Call1(FieldEvent_WaitTicks, 0x14);
      Call3(ObjectMotion_SetSpeedParameters, 0x1f, 0x10000, 0x8000);
      Call3(ObjectMotion_SetSpeedParameters, 0x20, 0x10000, 0x8000);
      Call2(ObjectMotion_SetActionVariant, 0x1f, 2);
      Call2(ObjectMotion_SetActionVariant, 0x20, 2);
      Call2(Camera_FollowActor, 0x20, 1);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x1f, 0x18a, 0x156);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x20, 0x1c2, 0x15a);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x1f);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x1f, 0x1c2, 0x15a);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x20);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x20, 0x1d6, 0x148);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x1f);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x1f, 0x1d6, 0x148);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x20);
      Call3(ObjectMotion_ResetAndSetPositionInMode2, 0x20, 0x1e6, 0x148);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x20);
      Call2(Object_SetModeById, 0x20, 1);
      Call2(Func_02000b4c, 0x20, 0);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x1f);
      Call2(Object_SetModeById, 0x1f, 1);
      Call2(Func_02000b4c, 0x1f, 0);
      Call3(ObjectMotion_CommitPositionAndActivate, 4, 0, -16);
      Call2(Func_02000b4c, 4, 0x4000);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call2(ObjectMotion_EnableActionAndSetCallback, 0x20, (s32)gData_02000be4);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call2(ObjectMotion_EnableActionAndSetCallback, 0x1f, (s32)gData_02000be4);
      Call1(FieldEvent_WaitTicks, 0xa);
      Call2(Object_LinkObjectAndSetCallback, 4, 0x20);
      Call1(FieldEvent_WaitTicks, 0x3c);
      Call2(ObjectMotion_SetActionVariant, 0x1f, 3);
      Call2(ObjectMotion_SetActionVariant, 0x20, 3);
      Call1(Object_RefreshSelectorById, 0x20);
      Call1(ObjectMotion_EnableActionAndResetMotion, 4);
      Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      Call3(Func_02000aec, 0x20, 0, 0);
      Call1(Func_02000bdc, 0x7b);
      Call1(Object_RefreshSelectorById, 0x1f);
      Call3(Func_02000aec, 0x1f, 0, 0);
      Call1(Func_02000bdc, 0x7b);
      Call1(FieldEvent_WaitTicks, 0xa);
      Func_02000a9c();
    }
  }
}
