#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "version.h"
#include "event_inline.h"
s32 Func_02000a94();
s32 Func_02000b7c();
s32 Func_02000a9c();
extern u8 MsgAlhafraInteriors120[];
extern u8 gData_02000be4[];

void Func_02000738(void) {
  s32 t0;
  s32 t1;

  t0 = Event_GetFlag(0x903);
  if (t0 != 0) {
  } else {
#if TLA_PATCH_1 /* The Japanese edition does not check 0x200. */
    t1 = Event_GetFlag(0x200);
    if (t1 == 0) {
    } else
#endif
    {
      Event_SetFlag(0x903);
      Func_02000a94();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgAlhafraInteriors120);
      Actor_WalkToAndWait(4, 504, 328);
      Actor_FaceDirectionAndWait(4, FACING_WEST);
      FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(328), 1);
      Func_02000b7c();
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x100, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      {
        s32 b = 0x109, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Actor_ShowEmote(0x1f, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x102, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      FieldActor_Jump(32, 6, 15);
      FieldActor_Jump(32, 6, 23);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x109, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_FaceDirectionAndWait(31, 0xd000);
      FieldEvent_Wait(15);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_FaceDirectionAndWait(31, 0x3000);
      FieldEvent_Wait(15);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(20);
      Actor_SetSpeed(31, 0x10000, 0x8000);
      Actor_SetSpeed(32, 0x10000, 0x8000);
      FieldActor_SetSpritePriority(31, 2);
      FieldActor_SetSpritePriority(32, 2);
      FieldCamera_FollowActor(32, 1);
      Actor_WalkTo(31, 394, 342);
      FieldEvent_Wait(10);
      Actor_WalkTo(32, 450, 346);
      FieldActor_WaitForMoveById(31);
      Actor_WalkTo(31, 450, 346);
      FieldActor_WaitForMoveById(32);
      Actor_WalkTo(32, 470, 328);
      FieldActor_WaitForMoveById(31);
      Actor_WalkTo(31, 470, 328);
      FieldActor_WaitForMoveById(32);
      Actor_WalkTo(32, 486, 328);
      FieldActor_WaitForMoveById(32);
      Actor_SetAnimation(32, 1);
      Actor_FaceDirectionAndWait(32, FACING_EAST);
      FieldActor_WaitForMoveById(31);
      Actor_SetAnimation(31, 1);
      Actor_FaceDirectionAndWait(31, FACING_EAST);
      Actor_WalkByAndWait(4, 0, -16);
      Actor_FaceDirectionAndWait(4, FACING_SOUTH);
      FieldEvent_Wait(10);
      Actor_SetScript(32, (s32)gData_02000be4);
      FieldEvent_Wait(10);
      Actor_SetScript(31, (s32)gData_02000be4);
      FieldEvent_Wait(10);
      FieldActor_WatchAndSetScript(4, 32);
      FieldEvent_Wait(60);
      FieldActor_SetSpritePriority(31, 3);
      FieldActor_SetSpritePriority(32, 3);
      FieldActor_RefreshSelectorById(32);
      FieldActor_Stop(4);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Actor_SetPosition(32, 0, 0);
      Event_PlaySound(0x7b);
      FieldActor_RefreshSelectorById(31);
      Actor_SetPosition(31, 0, 0);
      Event_PlaySound(0x7b);
      FieldEvent_Wait(10);
      Func_02000a9c();
    }
  }
}
