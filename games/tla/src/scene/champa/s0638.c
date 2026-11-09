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
s32 Func_02002a44();
s32 Func_02002b3c();
void Func_0200060c(void);
s32 Func_02002a4c();
extern u8 MsgChampa52[];

void Func_02000638(void) {
  s32 t0;
#if TLA_PATCH_1
  s32 t1;
#endif
  s32 t17;
  s32 t106;

  t0 = Event_GetFlag(0x822);
  if (t0 == 0) {
  } else {
#if TLA_PATCH_1
    // The Japanese edition has no flag 0 test.
    t1 = Event_GetFlag(0);
    if (t1 != 0) {
    } else
#endif
    {
      GameFlag_SetBit(0xa75);
      Func_02002a44();
      Event_ResolvePendingActions(0);
      Event_SetMessageId((s32)MsgChampa52);
      {
        /* FAKEMATCH: the local starts the facing before the other arguments, as
         * the native has (one reordered row as a plain argument). */
        s32 angle = 0xc000;

        Actor_SetPositionAndFacing(29, PIXELS(314), PIXELS(679), angle);
      }
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_WalkToAndWait(4, 314, 580);
      Actor_ArmCallback(4, FACING_SOUTH, 0);
      Event_Wait(10);
      Camera_FollowActor(29, 1);
      Func_02002b3c();
      Event_Wait(40);
      Actor_WalkToAndWait(29, 314, 624);
      Func_080d47b4(0x1d, 0x101, 0x1e);
      FieldMessage_ShowAtActor(0x1d, 0);
      t17 = Event_AskYesNoWithActorAnimation(4, 0);
      if (t17 == 0) {
        Event_ShowMessageThenWait(0x1d, 0, 5);
        FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
            (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
      } else {
        FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
            (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
        Event_ShowMessageThenWait(0x1d, 0, 5);
      }
      Func_080d47b4(0x1d, 0x109, 0x1e);
      FieldActor_LaunchFromLeader(0x1e, 0x10, 0, 0x4000);
      Actor_WaitForMoveById(30);
      Event_Wait(20);
      {
        /* FAKEMATCH: the native loads the emote and the frames before the
         * actor; pinned to their argument registers they are loaded first (the
         * actor comes one row early through the inline). */
        register s32 emote asm("r1") = 0x101;
        /* FAKEMATCH: the frames, pinned with the emote above. */
        register s32 frames asm("r2") = 0x1e;

        Func_080d47b4(0x1e, emote, frames);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Motion_SetVarCbAndRefresh(29, 2);
      Event_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Motion_SetModeAndWaitAnimation(30, 3);
      Event_Wait(10);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      Event_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      Event_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      Event_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_TurnToFaceEachOther(30, 4, 0);
      Event_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      Event_Wait(20);
      Actor_SetAnimationAndWait(30, 4);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Func_080d47b4(0x1d, 0x107, 0x1e);
      FieldActor_Jump(29, 2, 0);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Func_080d47b4(0x1e, 0x101, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Motion_SetVarCbAndRefresh(29, 2);
      Event_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Func_080d47b4(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Func_080d47b4(0x1d, 0x102, 0x14);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Motion_SetModeAndWaitAnimation(30, 3);
      Event_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      Event_Wait(10);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Func_080d47b4(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_ArmCallbackAndRefresh(29, FACING_NORTH);
      Event_Wait(10);
      Func_080d47b4(0x1d, 0x108, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Motion_SetModeAndWaitAnimation(30, 3);
      Event_Wait(10);
      Motion_SetVarCbAndRefresh(29, 2);
      Event_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Func_0200060c();
      Actor_TurnToFaceEachOther(30, 4, 0);
      Event_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_ArmCallback(4, FACING_SOUTH, 0);
      Event_Wait(20);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Motion_SetModeAndWaitAnimation(29, 3);
      Event_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Func_080d47b4(0x1e, 0x100, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Motion_SetModeAndWaitAnimation(29, 3);
      Event_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimation(4, 3);
      Motion_SetModeAndWaitAnimation(30, 3);
      Event_Wait(10);
      Motion_SetModeAndWaitAnimation(29, 3);
      Event_Wait(10);
      FieldCamera_FollowActor(4, 1);
      Actor_SetSpeed(29, 0x13333, 0x9999);
      Actor_OffsetPositionAndReset(29, 0, 100);
      Event_Wait(10);
      Actor_TurnToFaceEachOther(4, 30, 0);
      Event_Wait(30);
      Actor_SetAnimation(30, 3);
      Motion_SetModeAndWaitAnimation(4, 3);
      Event_Wait(30);
      Actor_SetModeById(30, 2);
      t106 = ((s32)Actor_GetById(4));
      if (t106 != 0) {
        Actor_ResetAndSetPosition(30, FIELD_AT(t106, s16, 10),
                                  FIELD_AT(t106, s16, 18));
      }
      Actor_WaitForMoveById(30);
      Actor_SetPosition(30, 0, 0);
      Actor_SetPosition(29, 0, 0);
      Func_02002a4c();
    }
  }
}
