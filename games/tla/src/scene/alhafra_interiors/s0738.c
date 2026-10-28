#include "types.h"
#include "scene_tables.h"
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
s32 Actor_SetPositionAndReset();
s32 Func_02000b4c();
s32 Camera_MoveWithinMapBounds();
s32 Func_02000b7c();
s32 FieldEvent_Wait();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000b34();
s32 Func_02000b5c();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_Jump();
s32 Actor_SetSpeedParameters();
s32 Actor_SetSpritePriority();
s32 Camera_FollowActor();
s32 Actor_ResetAndSetPositionInMode2();
s32 Actor_WaitForMoveById();
s32 Actor_SetModeById();
s32 Actor_CommitPositionAndActivate();
s32 Actor_SetMovementTypeById();
s32 Actor_WatchAndSetScript();
s32 Actor_RefreshSelectorById();
s32 Actor_Stop();
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
      Call3(Actor_SetPositionAndReset, 4, 504, 328);
      Call2(Func_02000b4c, 4, FACING_WEST);
      Call4(Camera_MoveWithinMapBounds, PIXELS(408), -1, PIXELS(328), 1);
      Func_02000b7c();
      Call1(FieldEvent_Wait, 10);
      Call2(Motion_SetVarCbAndRefresh, 31, 2);
      Call1(FieldEvent_Wait, 5);
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
      Call2(Motion_SetModeAndWaitAnimation, 31, 3);
      Call1(FieldEvent_Wait, 10);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call3(Actor_Jump, 32, 6, 15);
      Call3(Actor_Jump, 32, 6, 23);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 31, 3);
      Call1(FieldEvent_Wait, 10);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Func_02000b5c(0x20, 0x109, 0x1e);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Motion_SetVarCbAndRefresh, 31, 2);
      Call1(FieldEvent_Wait, 5);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 32, 3);
      Call1(FieldEvent_Wait, 10);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Func_02000b4c, 31, 0xd000);
      Call1(FieldEvent_Wait, 15);
      Call3(Func_02000b34, 0x1f, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 32, 3);
      Call1(FieldEvent_Wait, 10);
      Call3(Func_02000b34, 0x20, 0, 5);
      Call2(Func_02000b4c, 31, 0x3000);
      Call1(FieldEvent_Wait, 15);
      Call2(Motion_SetModeAndWaitAnimation, 31, 3);
      Call1(FieldEvent_Wait, 20);
      Call2(Motion_SetModeAndWaitAnimation, 32, 3);
      Call1(FieldEvent_Wait, 20);
      Call3(Actor_SetSpeedParameters, 31, 0x10000, 0x8000);
      Call3(Actor_SetSpeedParameters, 32, 0x10000, 0x8000);
      Call2(Actor_SetSpritePriority, 31, 2);
      Call2(Actor_SetSpritePriority, 32, 2);
      Call2(Camera_FollowActor, 32, 1);
      Call3(Actor_ResetAndSetPositionInMode2, 31, 394, 342);
      Call1(FieldEvent_Wait, 10);
      Call3(Actor_ResetAndSetPositionInMode2, 32, 450, 346);
      Call1(Actor_WaitForMoveById, 31);
      Call3(Actor_ResetAndSetPositionInMode2, 31, 450, 346);
      Call1(Actor_WaitForMoveById, 32);
      Call3(Actor_ResetAndSetPositionInMode2, 32, 470, 328);
      Call1(Actor_WaitForMoveById, 31);
      Call3(Actor_ResetAndSetPositionInMode2, 31, 470, 328);
      Call1(Actor_WaitForMoveById, 32);
      Call3(Actor_ResetAndSetPositionInMode2, 32, 486, 328);
      Call1(Actor_WaitForMoveById, 32);
      Call2(Actor_SetModeById, 32, 1);
      Call2(Func_02000b4c, 32, FACING_EAST);
      Call1(Actor_WaitForMoveById, 31);
      Call2(Actor_SetModeById, 31, 1);
      Call2(Func_02000b4c, 31, FACING_EAST);
      Call3(Actor_CommitPositionAndActivate, 4, 0, -16);
      Call2(Func_02000b4c, 4, FACING_SOUTH);
      Call1(FieldEvent_Wait, 10);
      Call2(Actor_SetMovementTypeById, 32, (s32)gData_02000be4);
      Call1(FieldEvent_Wait, 10);
      Call2(Actor_SetMovementTypeById, 31, (s32)gData_02000be4);
      Call1(FieldEvent_Wait, 10);
      Call2(Actor_WatchAndSetScript, 4, 32);
      Call1(FieldEvent_Wait, 60);
      Call2(Actor_SetSpritePriority, 31, 3);
      Call2(Actor_SetSpritePriority, 32, 3);
      Call1(Actor_RefreshSelectorById, 32);
      Call1(Actor_Stop, 4);
      Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
      Call3(Func_02000aec, 32, 0, 0);
      Call1(Func_02000bdc, 0x7b);
      Call1(Actor_RefreshSelectorById, 31);
      Call3(Func_02000aec, 31, 0, 0);
      Call1(Func_02000bdc, 0x7b);
      Call1(FieldEvent_Wait, 10);
      Func_02000a9c();
    }
  }
}
