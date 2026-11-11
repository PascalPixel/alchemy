#include "types.h"
#include "ram_buffer.h"
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
      Event_SetFlag(0xa75);
      Func_02002a44();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgChampa52);
      {
        /* FAKEMATCH(temporary): the facing in a local, which starts it before
         * the other arguments as the native has; as the plain argument 0xc000,
         * 2 of 414 instructions differ in tla-en. */
        s32 angle = 0xc000;

        Actor_SetPositionAndFacing(29, PIXELS(314), PIXELS(679), angle);
      }
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_WalkToAndWait(4, 314, 580);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(10);
      FieldCamera_FollowActor(29, 1);
      Func_02002b3c();
      FieldEvent_Wait(40);
      Actor_WalkToAndWait(29, 314, 624);
      Actor_ShowEmote(0x1d, 0x101, 0x1e);
      Event_OpenMessage(0x1d, 0);
      t17 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
      if (t17 == 0) {
        FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
        FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) =
            (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
      } else {
        FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) =
            (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
        FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      }
      Actor_ShowEmote(0x1d, 0x109, 0x1e);
      FieldActor_LaunchFromLeader(0x1e, 0x10, 0, 0x4000);
      FieldActor_WaitForMoveById(30);
      FieldEvent_Wait(20);
      {
        /* FAKEMATCH(pin): the emote pinned to r1, so that it loads before the
         * actor as the native has; as the plain argument 0x101 through the
         * inline, 2 of 414 instructions differ in tla-en. */
        register s32 emote asm("r1") = 0x101;

        Actor_ShowEmote(0x1e, emote, 0x1e);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_FaceEachOther(30, 4, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(30, 4);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_ShowEmote(0x1d, 0x107, 0x1e);
      FieldActor_Jump(29, 2, 0);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_ShowEmote(0x1e, 0x101, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH(barrier): the emote and the frames held at an empty asm, so
         * the first argument is set last, as the native schedule has it; as no
         * asm, 18 of 414 instructions differ in tla-en. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Actor_ShowEmote(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_ShowEmote(0x1d, 0x102, 0x14);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(10);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH(barrier): the emote and the frames held at an empty asm, so
         * the first argument is set last, as the native schedule has it; as no
         * asm, 12 of 414 instructions differ in tla-en. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Actor_ShowEmote(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(0x1d, 0x108, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Func_0200060c();
      Actor_FaceEachOther(30, 4, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_ShowEmote(0x1e, 0x100, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimation(4, 3);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      FieldCamera_FollowActor(4, 1);
      Actor_SetSpeed(29, 0x13333, 0x9999);
      Actor_WalkBy(29, 0, 100);
      FieldEvent_Wait(10);
      Actor_FaceEachOther(4, 30, 0);
      FieldEvent_Wait(30);
      Actor_SetAnimation(30, 3);
      Actor_SetAnimationAndWait(4, 3);
      FieldEvent_Wait(30);
      Actor_SetAnimation(30, 2);
      t106 = ((s32)Actor_Get(4));
      if (t106 != 0) {
        Actor_SetDestination(30, FIELD_AT(t106, s16, 10),
                                  FIELD_AT(t106, s16, 18));
      }
      FieldActor_WaitForMoveById(30);
      Actor_SetPosition(30, 0, 0);
      Actor_SetPosition(29, 0, 0);
      Func_02002a4c();
    }
  }
}
