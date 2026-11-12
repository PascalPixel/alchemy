/// @file
/// Scene code of overlay 0x682, in ROM order, from s05dc's offset up to the next object the overlay links.

#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02002a44();
s32 Func_02002a5c();
s32 Func_02002a4c();

void Func_020005dc(s32 a0) {
  s32 t2;

  Func_02002a44();
  FieldEvent_ResolvePendingActions(0);
  t2 = Value3(Func_02002a5c, a0, 0x19f, -1);
  if (t2 == 0) {
    Event_ClearFlag(0xa74);
  }
  Func_02002a4c();
}

void Func_0200060c(void) {
  s32 angle;

  Actor_ShowItemIconById(31, 415);
  /* FAKEMATCH(temporary): the facing in a local, which starts it before the
   * other arguments as the native has; as the plain argument 0xc000, 4 of 17
   * instructions differ in tla-en. */
  angle = 0xc000;
  Actor_SetPositionAndFacing(31, PIXELS(312), PIXELS(593), angle);
  Event_SetFlag(0xa74);
}

#include "version.h"
#include "frame.h"
s32 Func_02002b3c();
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

extern s32 gData_0200320c[];
extern s32 gData_0200b410[];

const s32 *Func_02000a60(void) {
  if (Event_GetFlag(0x97f))
    return gData_0200b410;
  return gData_0200320c;
}

void Func_02000a80(void) {
  if ((gFrameCount & 1) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500007c;
    index = 14;
  first:
    palette = (volatile u16 *)0x05000060;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x05000062 = saved;
    saved = *(volatile u16 *)0x0500009c;
    index = 14;
  second:
    palette = (volatile u16 *)0x05000080;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000082 = saved;
    saved = *(volatile u16 *)0x050000bc;
    index = 14;
  third:
    palette = (volatile u16 *)0x050000a0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)0x050000a2 = saved;
  }
}
