/// @file
/// The scene code of overlay 0x67e, in ROM order.

// field.h declares this on struct ActorView, as the routine behind it is
// defined; this scene passes the same actors as struct Actor and declares it
// so below.
#define OWN_Actor_SetSingleChildFlags
#include "izumo/izumo.h"
#include "call.h"
#include "flag_call.h"
#include "scene_tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "version.h"
#include "frame.h"

void Func_02000038(void) { Func_02000de8(gData_02000e10); }

void Func_02000048(void) {
  struct FieldActor *actor = Actor_GetById(12);
  s32 x = actor->x >> 20;
  s32 z = actor->z >> 20;

  /* FAKEMATCH(wrapper): the flag commands go through flag_call.h's inlines,
   * which build each flag id again, and the block copies through call.h's
   * Call6; as direct calls to GameFlag_Test, GameFlag_SetBit,
   * GameFlag_ClearBit and Func_02000ce8, 27 of 135 instructions differ in
   * tla-en. */
  Func_02000df0();
  Func_02000e08(132);
  if (x == 20 && z == 9) {
    if (Flag_IsSet(0x301)) {
      Func_02000ce0(36, 12, 22, 8, 3, 3);
      Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    }
    if (!Flag_IsSet(0x924)) {
      Func_02000d00();
      Func_02000dd0(0);
      Camera_FollowActor(22, 1);
      Func_02000da0();
      Func_02000d88(22, 257, 0);
      FieldEvent_Wait(60);
      Actor_ArmCallback(22, FACING_WEST, 0);
      FieldEvent_Wait(60);
      Func_02000d88(22, 256, 0);
      FieldEvent_Wait(60);
      Func_02000d60(MsgTheMisalignedStone);
      Func_02000d70(22, 0);
      FieldEvent_Wait(30);
      Func_02000d88(22, 262, 0);
      FieldEvent_Wait(30);
      Func_02000d70(22, 0);
      Actor_ArmCallback(22, FACING_SOUTH, 0);
      Func_02000d08();
      Flag_Set(0x924);
    }
    Flag_Set(0x250);
  } else if (x == 19 && z == 9) {
    Flag_Clear(0x250);
    if (Flag_IsSet(0x301)) {
      Func_02000ce0(36, 8, 22, 8, 3, 3);
      Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Func_020001ac(void) {
  if (GameFlag_Test(2343))
    return gData_02001644;
  return gData_02001494;
}

void Func_020001cc(void) {
  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
}

void Func_020001ec(void) {
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Test(3999)) {
    Call3((void (*)())Func_02000d40, 64, PIXELS(456), PIXELS(584));
    if (object->x >> 20 == 28 && object->z >> 20 == 36) {
      Func_02000d00();
      Func_02000dd0(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, -16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02000d08();
    }
  }
}

void Func_0200027c(void) {
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Test(4000)) {
    Call3((void (*)())Func_02000d40, 65, PIXELS(472), PIXELS(552));
    if (object->x >> 20 == 29 && object->z >> 20 == 34) {
      Func_02000d00();
      Func_02000dd0(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Call3((void (*)())Actor_OffsetPositionAndResetMotion,
            gMainRoutineWork.currentOwner, 0, -16);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02000d08();
    }
  }
}

void Func_0200030c(void) {
  Call7((void (*)())Func_02000df8, 44, 5, 8, 5, 7, 32, 0);
}

void Func_0200032c(void) {
  Call7((void (*)())Func_02000df8, 55, 0, 4, 2, 16, 23, 0);
}

void Func_0200034c(void) {
  Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
}

void Func_0200036c(void) {
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
  if (!GameFlag_Test(4001)) {
    Call3((void (*)())Func_02000d40, 66, PIXELS(168), PIXELS(328));
    if (object->x >> 20 == 10 && object->z >> 20 == 20) {
      Func_02000d00();
      Func_02000dd0(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02000d08();
    }
  }
}

void Func_020003f8(void) {
  Call7((void (*)())Func_02000df8, 49, 0, 5, 4, 24, 16, 0);
}

void Func_02000418(void) {
  Call7((void (*)())Func_02000df8, 36, 0, 5, 3, 16, 9, 0);
}

void Func_02000438(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02000d00();
  Func_02000dd0(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Func_02000e08(123);
  Func_02000db0(*(s16 *)(work + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000d08();
}

const u8 *Func_0200049c(void) {
  if (GameFlag_Test(2343))
    return gData_02001b00;
  return gData_020018b4;
}

static inline s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): the flag test forwarded through the inline, as overlay
   * 679's GameFlag_Test_ does, keeps the actor in r5 and the message in r6; as
   * a direct call to GameFlag_Test the two swap; as that direct call, 16 of 42
   * instructions of Func_020004bc differ in tla-en, as do 16 of 42 of
   * Func_02000538. */
  return GameFlag_Test(flag);
}

/// The ring of stones: one line after the flag is set, otherwise the full talk
/// and its answer.
void Func_020004bc(s32 actor) {
  Func_02000d00();
  Func_02000dd0(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgCanYouImagine);
    Func_02000d70(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRing;

    Func_02000d60(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
  }
  Func_02000d08();
}

void Func_02000538(s32 actor) {
  Func_02000d00();
  Func_02000dd0(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgButItS);
    Func_02000d70(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRingOf;

    Func_02000d60(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
  }
  Func_02000d08();
}

void Func_020005b4(s32 actor) {
  const u8 *message;

  Func_02000d00();
  Func_02000dd0(0);
  message = MsgLadyUzumeAnd;
  Func_02000d60(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02000e00(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02000d60(message + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02000d60(message + 2);
  }
  Func_02000d70(actor, 0);
  Func_02000d08();
}

/// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Func_0200060c(void) {
  volatile u16 *palette;
  s16 saved;
  s32 index;

  // The three rotations are goto loops, which loop.c leaves alone, as the
  // native's are; as do-while, while or for loops, 90 instructions differ in
  // tla-en.
  if ((gFrameCount & 3) == 0) {
    saved = *(volatile u16 *)&BG_PLTT_COLOR(3, 14);
    index = 14;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(3, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)&BG_PLTT_COLOR(3, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(4, 14);
    index = 14;
  second:
    palette = (u16 *)&BG_PLTT_COLOR(4, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(4, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(5, 14);
    index = 14;
  third:
    palette = (u16 *)&BG_PLTT_COLOR(5, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)&BG_PLTT_COLOR(5, 1) = saved;
  }
}

void Func_020006a4(void) {
  /* FAKEMATCH(signature): six routines called through call.h's Call3 and Value
   * with function types they do not have (Value3 calls the void Func_02000d88
   * as returning a value), so each call's arguments are set up in the native's
   * order; as all six direct calls, 12 of 140 instructions differ in tla-en. */
  GameFlag_SetBit(0x929);
  Func_02000d00();
  Func_02000dd0(0);
  Func_02000d60(MsgWait2);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 200, 264);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_02000d70(32, 0);
  FieldEvent_Wait(15);
  Value3((s32 (*)())Func_02000d88, gMainRoutineWork.currentOwner, 256, 30);
  Call3((void (*)())Func_02000d40, 32, PIXELS(200), PIXELS(216));
  Actor_ResetAndSetPositionInMode2(32, 200, 248);
  Actor_WaitForMoveById(32);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 32, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Value3((s32 (*)())Func_02000d88, gMainRoutineWork.currentOwner, 261, 60);
  Func_02000d88(32, 262, 30);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 32, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    FieldEvent_Wait(90);
    Call3((void (*)())Actor_SetSpeedParameters, 32, 0x6666, 0x3333);
    Actor_ResetAndSetPositionInMode2(32, 200, 216);
    Actor_WaitForMoveById(32);
    Func_02000da8(32, 1);
    FieldEvent_Wait(60);
    Call3((void (*)())Actor_ArmCallback, 32, FACING_SOUTH, 0);
    Func_02000d88(32, 265, 30);
    Func_02000d70(32, 0);
    Actor_SetSpritePriority(32, 3);
    Actor_GetById(32)->motionFlags |= 2;
    GameFlag_SetBit(0x302);
  } else {
    Func_02000814();
  }
  Func_02000d08();
}

void Func_02000814(void) {
  Func_02000d60(MsgThisTokenIsn);
  Func_02000d88(32, 264, 30);
  Func_02000d70(32, 0);
  Actor_ArmCallback(32, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(32, gMainRoutineWork.currentOwner, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Actor_StartRepeatedMotion(32, 1);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Func_02000d70(32, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(15);
  Func_02000d88(32, 265, 30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(32, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(15);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(32, 0x3333, 0x1999);
  Actor_GetById(32)->motionFlags = 0;
  Actor_SetSpritePriority(32, 3);
  Actor_ResetAndSetPositionInMode2(32, 200, 208);
  Actor_WaitForMoveById(32);
  Func_02000d40(32, 0, 0);
  GameFlag_SetBit(0x92a);
  GameFlag_ClearBit(0x302);
}

void Func_02000954(void) {
  if (!GameFlag_Test(2346)) {
    const u8 *message;

    Func_02000d00();
    Func_02000dd0(0);
    Func_02000d88(32, 256, 60);
    message = MsgWouldYouBe;
    Func_02000d60(message);
    /* FAKEMATCH(signature): FieldMessage_ShowAtActor called as returning a
     * value, so the flags load before the actor as the native schedules it;
     * as FieldMessage_ShowAtActor(32, 0) through the header's void prototype,
     * 2 of 43 instructions differ in tla-en. */
    ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(32, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Func_02000814();
    } else {
      Func_02000d60(message - 1);
      Func_02000d70(32, 0);
    }
    Func_02000d08();
  } else {
    Func_02000d70((s32)MsgTheLegendSays, 0);
  }
}

s32 Func_020009d0(void) {
  /* FAKEMATCH(wrapper): Call3 sets Func_02000d40's arguments up in the native's
   * order, as in Func_020006a4; as a direct call, 2 of 123 instructions differ
   * in tla-en. */
  GameFlag_ClearBit(0x302);
  WORD_AT(Ram_HeapSlots->eventWork, 428) = 0x100;
  Task_Add(Func_0200060c, TASK_PASS_FRAME);
  Func_02000de0(gData_02000e10);
  if (GameFlag_Test(0x927)) {
    BYTE_AT(Actor_GetById(15), 90) = 2;
    Actor_SetModeById(25, 5);
    Actor_SetModeById(26, 5);
    Actor_SetModeById(27, 5);
    Actor_SetModeById(28, 5);
    Actor_SetModeById(29, 6);
    Actor_SetModeById(30, 6);
    Actor_SetModeById(31, 5);
    if (gMainRoutineWork.entrance == 8 && GameFlag_Test(0x928) &&
        !GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Event_StartScreenTransition();
      Func_020006a4();
    }
    if (GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Call3((void (*)())Func_02000d40, 32, PIXELS(200), PIXELS(216));
      Actor_SetSpritePriority(32, 3);
      Actor_GetById(32)->motionFlags |= 2;
      GameFlag_SetBit(0x302);
    }
  }
  Actor_SetSpritePriority(24, 3);
  Actor_GetById(24)->motionFlags |= 2;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000b08(void) {
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_SetBit(769);
  if (GameFlag_Test(592)) {
    Func_02000ce0(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Func_02000d40(gMainRoutineWork.currentOwner, PIXELS(376), PIXELS(136));
  } else {
    Func_02000ce0(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
  }
}

void Func_02000ba4(void) {
  GameFlag_ClearBit(769);
  Func_02000ce0(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Func_02000ce8, 40, 8, 3, 3, 22, 8);
}

void Func_02000bdc(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 340);

  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
#if TLA_PATCH_1 /* From patch 1. */
  Func_02000e08(129);
#endif
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  FieldEvent_Wait(14);
  Func_02000db0(value);
}

void Func_02000c58(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Actor_CommitPositionAndActivate(4, 16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000c7c(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Actor_CommitPositionAndActivate(4, -16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000ca4(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}
