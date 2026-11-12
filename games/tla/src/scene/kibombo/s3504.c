/// @file
/// Scene code of overlay 0x670, in ROM order, from s3504's offset up to the next object the overlay links.

#include "kibombo.h"

s32 Func_02005c44(struct FieldSprite *sprite, s32 previous);
void Func_02003308(struct KibomboObject *object);

void Func_02003504(void) {
  struct KibomboObject *center = Actor_GetById(17);
  struct KibomboObject *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;
  s32 x;
  s32 y;
  s32 z;

  Sound_Play(154);
  for (i = 0; i <= 7; i++) {
    Actor_SetPartAttribute(Actor_GetById(17), 7);
    wait(2);
    Actor_SetPartAttribute(Actor_GetById(17), 0);
    wait(2);
  }
  Sound_Play(209);
  i = 0;
  angle = 0;
  do {
    x = center->x;
    y = center->y + 0x180000;
    z = center->z;
    spark = Import_ActorCreate(673, x, y, z);
    if (spark != 0) {
      previous = Func_02005c44(spark->sprite, previous);
      spark->motionFlags = 0;
      spark->sprite->priority = 1;
      Actor_SetSingleChildFlags(spark, 0);
      Func_02005a24(spark, 1);
      spark->field64 = 0;
      spark->field4c = 0;
      spark->field66 = angle / 360;
      spark->timer = 0;
      spark->targetX = center->x;
      spark->targetY = center->y + 0x180000;
      spark->targetZ = center->z;
      spark->update = Func_02003308;
    }
    angle += 0x160000;
    i++;
  } while (i <= 15);
}

#include "scene_tables.h"

void Func_02003610(void) {
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  Func_02005bb4();
  Event_Wait(40);
  if (gData_02007950 != 0)
    Func_02003504();
}

void Func_02003640(s32 open, s32 hide) {
  if (open != 0) {
    Kibombo_MapCopy(49, 37, 79, 15, 3, 2);
    Kibombo_ObjSetPos(Actor_GetById(12), PIXELS(253), PIXELS(32), PIXELS(289));
    Actor_SetPositionAndResetMotion(Actor_GetById(13), PIXELS(275), PIXELS(32),
                                    PIXELS(289));
    if (hide != 0) {
      Actor_SetModeById(12, 0);
      Actor_SetModeById(13, 0);
    } else {
      Actor_SetModeById(12, 1);
      Actor_SetModeById(13, 1);
    }
  } else {
    Kibombo_MapCopy(45, 37, 79, 15, 3, 2);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
}

void Func_020036e0(s32 open, s32 mode) {
  if (open != 0) {
    Map_CopyCellPictureIndicesRect(41, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Actor_SetPositionAndResetMotion(Actor_GetById(14), PIXELS(264),
                                      PIXELS(32), PIXELS(308));
    switch (mode) {
    case 0:
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      break;
    case 1:
      Actor_SetModeById(14, 0);
      break;
    case 2:
      Actor_SetModeById(14, 1);
      break;
    case 3:
      Actor_SetModeById(14, 2);
      break;
    case 4:
      Actor_SetModeById(14, 3);
      break;
    case 5:
      Actor_SetModeById(14, 6);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    case 6:
      Actor_SetModeById(14, 7);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    }
  } else {
    Map_CopyCellPictureIndicesRect(45, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(17, 22, 1, 1, 16, 22);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  }
}

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

void Func_020037e4(void) {
  struct KibomboEffectWork *work = Ram_HeapSlots->effectWork;

  
  if (work->field7c4 != 0) {
    if (GameFlag_Test(0x220))
      Func_02000624();
    else if (GameFlag_Test(0x221))
      Func_02000668();
    else if (GameFlag_Test(0x222))
      Func_020006ac();
    else if (GameFlag_Test(0x223))
      Func_020006f0();
  }
}

void Func_0200384c(void) {
  Func_02005c14(136, 1);
  Task_Add(Func_020037e4, 0x480);
  Func_02005c1c(gMainRoutineWork.currentOwner, -1);
  Func_02005c34();
  Field_DispatchTypeHandler(1);
  Func_02005c24();
  Task_RemoveFar(Func_020037e4);
  Func_02005c2c();
}

void Func_02003898(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo4)
    Task_RemoveFar(Func_02002c34);
  else if (scene == (s32)&SceneId_Kibombo5)
    Task_RemoveFar(Func_02002ccc);
  else if (scene == (s32)&SceneId_Kibombo6)
    Task_RemoveFar(Func_02002d64);
}

void Func_020038ec(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x202))
    Func_02003898();
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003930(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  Func_020038ec();
}

void Func_02003948(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader;
  s32 x;
  s32 px;

  
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  x = leader->x;
  px = x >> 16;
  Actor_SetMoveTarget(leader, ((8 - px % 16) << 16) + x, leader->y, leader->z);
  Script_WaitForEventTimeout(leader);
  leader->facing = 0xc000;
  if (GameFlag_Test(0x202))
    Func_02003898();
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02005cd0);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003a08(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader = Actor_GetById(party->currentOwner);

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(leader, x << 16, PIXELS(-12), z << 16);
  Script_WaitForEventTimeout(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

#include "input.h"

void Func_02003ac8(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 23 && (gInput.held & 64) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & 32) == 0)
    return;
  Func_02003a08(680, 352);
}

void Func_02003b20(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 38) {
    if (x == 23 && (gInput.held & 16) == 0)
      return;
    if (x == 25 && (gInput.held & 32) == 0)
      return;
  } else if (x == 24 && z == 39 && (gInput.held & 64) == 0) {
    return;
  }
  Func_02003a08(392, 608);
}

void Func_02003b88(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 54) {
    if (x == 42 && (gInput.held & 16) == 0)
      return;
    if (x == 44 && (gInput.held & 32) == 0)
      return;
  } else if (x == 43 && z == 55 && (gInput.held & 64) == 0) {
    return;
  }
  Func_02003a08(696, 864);
}

void Func_02003bf0(void) {
  if (GameFlag_Test(0x202))
    Func_02003898();
  Func_02003a08(376, 288);
}

/// Fades the screen in and runs Func_02002d64 each frame for up to 160 frames,
/// or until A, B or Start is held, then fades out.
void Func_02003c14(void) {
  u32 i;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002d64, 0x480);
  Event_Wait(20);
  // As a for loop, loop.c keeps gInput's address and the mask in registers
  // across the wait; the native builds both again each frame (differ 18).
  i = 0;
  goto test;
next:
  i++;
test:
  if (i > 159)
    goto done;
  Event_Wait(1);
  if ((gInput.held & 11) == 0)
    goto next;
done:
  Task_RemoveFar(Func_02002d64);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void Func_02005a4c(void);
void BattleEffect_StartBufferInterpolation(s32 frames);

void Func_02003c84(void) {
  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_CamMove(146 << 18, -1, PIXELS(248), 0);
  Func_02005a4c();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Camera_MoveWithinMapBounds(PIXELS(248), -1, 134 << 18, 0);
  Func_02005a4c();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(180);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(10);
}

void Func_020032b0(void);

void Func_02003dec(void) {
  struct KibomboObject *leader;

  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Event_SetMessageId((s32)MsgAtLastThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(78);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 86, 455);
  Actor_SetHorizontalPositionWithTerrain(26, PIXELS(86), PIXELS(455));
  wait(1);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjWalk(26, 72, 455);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Func_02005bb4();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(320), 1);
  Func_02005bb4();
  Event_Wait(80);
  Actor_Jump(17, 4, 40);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_CamSpeed(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(400), 1);
  Func_02005bb4();
  Event_Wait(20);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(11);
  GameFlag_SetBit(0x8f2);
  Sound_Play(148);
  Actor_GetById(18)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(19)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(20)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(21)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(22)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(23)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(24)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(25)->velocityY = PIXELS(4);
  wait(20);
  Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 20);
  Func_020032b0();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x3333, 0x666);
  Camera_MoveWithinMapBounds(PIXELS(264), 0, PIXELS(400), 1);
  Func_02005bb4();
  Event_Wait(200);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(111), PIXELS(32), PIXELS(456), 1);
  Func_02005bb4();
  Actor_ArmCallback(26, FACING_EAST, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_SetModeById(26, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(26, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(26);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  GameFlag_SetBit(0x8f1);
}

void Func_02002eb0(void);
void Func_020032e0(void);
void Func_02003610(void);

void Func_020040cc(void) {
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(78);
  Kibombo_CamSpeed(0xcccc, 0x1999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  Func_02005bb4();
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_SetMessageId((s32)MsgLetUsNow);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Actor_SetPositionAndReset(17, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Event_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(11);
  Func_02003610();
  Event_Wait(160);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(0x240);
  Func_02003640(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 40);
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x202;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(10);
}

void Func_02004294(void) {
  struct KibomboObject **slot;
  struct KibomboObject *object;
  s32 y;

  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Func_02003640(1, 1);
  wait(1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(161);
  Func_02003640(0, 1);
  Event_Wait(20);
  Func_080d489c(17, 0x102);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStill);
  Event_ShowMessageAndWait(17, 0);
  slot = &gData_02007950;
  object = *slot;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000; the loop is
    // entered at its test, which reuses the pointer read for the test above.
    object->motionFlags = 0;
    goto test;
    do {
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      /* FAKEMATCH(temporary): the slot's address reloaded each pass, as the
       * native does; as gData_02007950->y, 4 of 151 instructions differ in
       * tla-en. */
      slot = &gData_02007950;
      (*slot)->y += 0xffffc000;
      wait(1);
      object = *slot;
    test:
      y = object->y;
      /* FAKEMATCH(barrier): an empty asm on the height, so the limit's first
       * half is built before it loads; as object->y > 0x240000, 2 of 151
       * instructions differ in tla-en. */
      __asm__("" : "+l"(y));
    } while (y > 0x240000);
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  GameFlag_ClearBit(0x8f7);
  Sound_Play(11);
  Event_Wait(80);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);

void Func_02004424(void) {
  struct KibomboEventWork *work;
  s32 effect;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, FACING_SOUTH + FACING_STEP);
  Event_ShowMessageAndWait(0x4011, 0);
  Task_RemoveFar(Func_02002ccc);
  Kibombo_ObjTurn(17, FACING_NORTH + FACING_STEP);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(0x109);
  Import_EventSetExitRequest(12);
}

extern const u8 gData_0200613c[];
extern const u8 gData_0200614c[];
extern const u8 gData_02006324[];

void Func_020000a0(struct KibomboObject *object);
void Func_02002f34(void);
void Func_02003030(void);
void Func_02003184(void);
void Func_02003640(s32 open, s32 hide);
void Func_020036e0(s32 open, s32 mode);
void Func_02004524(void);
void Func_020045d0(void);
void Func_020050ac(void);
void Func_02005114(void);
void Func_0200516c(void);
void Func_020054e0(void);
void Func_02005594(void);
void Func_020057e0(void);
void Func_02005898(void);
void Func_02005bcc(void);
void Func_02005c04(s32 a);
void Func_02005c54(void);
void Func_02005c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02005c64(const void *script);

void Func_02004524(void) {
  struct KibomboEventWork *work;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02003640(1, 1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}

extern const u8 gData_020061a8[];

void Actor_Stop(s32 actor);

void Func_020045d0(void) {
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Kibombo_CamMove(132 << 17, 0, 200 << 17, 1);
  Func_02005bb4();
  Event_Wait(10);
  Actor_Jump(18, 6, 0);
  Actor_Jump(19, 6, 0);
  Actor_Jump(20, 6, 0);
  Actor_Jump(21, 6, 0);
  Actor_Jump(22, 6, 0);
  Actor_Jump(23, 6, 0);
  Actor_Jump(24, 6, 0);
  Actor_Jump(25, 6, 40);
  Kibombo_ObjTurn(20, FACING_SOUTH + FACING_STEP);
  Actor_Jump(20, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 20, 0);
  Kibombo_ObjTurn(23, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(23, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 23, 0);
  Actor_SetMovementTypeById(18, gData_020061a8);
  Actor_SetMovementTypeById(19, gData_020061a8);
  Actor_SetMovementTypeById(20, gData_020061a8);
  Actor_SetMovementTypeById(21, gData_020061a8);
  Actor_SetMovementTypeById(22, gData_020061a8);
  Actor_SetMovementTypeById(23, gData_020061a8);
  Actor_SetMovementTypeById(24, gData_020061a8);
  Actor_SetMovementTypeById(25, gData_020061a8);
  Event_Wait(40);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(17, 2, 10);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(17, 0);
  Actor_Stop(18);
  Actor_Stop(19);
  Actor_Stop(20);
  Actor_Stop(21);
  Actor_Stop(22);
  Actor_Stop(23);
  Actor_Stop(24);
  Actor_Stop(25);
  Event_Wait(40);
  Kibombo_ObjArm(18, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(19, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(20, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(21, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(22, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(23, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(24, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(25, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(17, 0x107, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(0x8000 + 17, 0);
}

void Func_020047e4(void) {
  struct KibomboObject **slot;
  struct KibomboObject *object;

  Func_02004524();
  Sound_Play(161);
  Func_02003640(0, 1);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStillNot);
  Event_ShowMessageAndWait(17, 0);
  object = gData_02007950;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000.
    object->motionFlags = 0;
    if (object->y > 144 << 14) {
      do {
        Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 0);
        slot = &gData_02007950;
        object = *slot;
        object->y += 0xffffc000;
        wait(1);
      } while ((*slot)->y > 0x240000);
    }
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x4011, 0);
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_0200489c(void) {
  Func_02004524();
  Sound_Play(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(0x241);
  Actor_SetMovementTypeById(12, gData_0200614c);
  Actor_SetMovementTypeAndWaitById(13, gData_0200614c);
  Actor_SetMovementTypeById(12, gData_0200613c);
  Actor_SetMovementTypeById(13, gData_0200613c);
  Actor_SetPartAttribute(Actor_GetById(12), 12);
  Actor_SetPartAttribute(Actor_GetById(13), 12);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_02004978(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_GetById(12)->update = Func_020000a0;
  Actor_GetById(13)->update = Func_020000a0;
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  Event_ShowMessageThenWait(17, 0, 20);
  Sound_Play(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(162);
  Task_Add(Func_02002f34, 0x480);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_02004a28(void) {
  struct KibomboObject *object;
  u32 i;

  object = Actor_GetById(14);
  Func_02004524();
  Actor_SetSpritePriority(17, 1);
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookTheGreatGabomba);
  Event_ShowMessageThenWait(17, 0, 20);
  Kibombo_SePlay(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(233);
  object->scaleY = 0x1999;
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  Actor_SetModeById(14, 3);
  for (i = 0; i < 64; i++) {
    object->scaleY += 0x400;
    object->y += 0x2000;
    wait(1);
  }
  Sound_Play(0x12a);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a while loop, which moves the global's address out of the loop, 24
    // of 288 instructions differ in tla-en.
    if (gData_02007950->y > 0x240000) {
    lower:
      gData_02007950->y -= 0x8000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto lower;
    }
  }
  Sound_Play(232);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a for loop, which moves the global's address out of the loop, 25 of
    // 288 instructions differ in tla-en.
    i = 0;
  rise:
    gData_02007950->y += 0x60000;
    object->scaleY -= 0x2000;
    object->y -= 0x8000;
    wait(1);
    i++;
    if (i < 4)
      goto rise;
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      object->scaleY -= 0x2000;
      object->y -= 0x8000;
      wait(1);
    }
  }
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Func_020036e0(0, 0);
  Func_02003640(1, 1);
  Event_Wait(10);
  Func_02003640(0, 1);
  Event_Wait(40);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Func_020036e0(1, 0);
  Sound_Play(132);
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  for (i = 0; i < 8; i++) {
    object->scaleY += 0x2000;
    object->y += 0x8000;
    wait(1);
  }
  Func_080d489c(17, 0x102);
  Event_Wait(40);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjEmote(17, 0x108, 20);
  Kibombo_MessagePut(0x4011, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjSpeed(17, 0xcccc, 0x6666);
  Kibombo_ObjWalk(17, 0x108, 0x140);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fb);
  Import_EventSetExitRequest(13);
}

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);

void Func_02004cf4(void) {
  struct KibomboEventWork *work;
  struct KibomboObject *leader;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Camera_MoveWithinMapBounds(PIXELS(384), -1, PIXELS(601), 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(370), PIXELS(636), FACING_NORTH);
  Func_02005a4c();
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  BattleEffect_ApplyColorToSourceBuffer(0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Actor_GetById(10)->velocityY = PIXELS(3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(60);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_SetMessageId((s32)MsgRightWellI);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 10);
  Actor_SetModeById(10, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_Jump(8, 4, 10);
  Kibombo_MessagePut(0x2008, 0);
  Actor_ArmCallbackAndRefresh(9, FACING_EAST);
  Func_080d47b4(8, 259, 20);
  Kibombo_MessagePut(0x2008, 0);
  Func_080d47b4(9, 256, 20);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(8, FACING_WEST);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(8, FACING_SOUTH + FACING_STEP);
  Kibombo_MessagePut(0x2008, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(0x2008, 0);
  Kibombo_ObjArm(9, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjSpeed(8, 0x19999, 0xcccc);
  Kibombo_ObjWalk(8, 364, 576);
  Kibombo_ObjArm(8, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(9, 261, 80);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_WEST);
  Actor_SetModeById(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Kibombo_ObjSpeed(10, 0xcccc, 0x6666);
  Actor_SetModeById(10, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(10, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  work = Ram_HeapSlots->eventWork;
  work->field1b4 = 16;
  work->screenEffect = 0x209;
  GameFlag_SetBit(0x8ef);
  Func_02005aa4();
}

s32 Func_02005034(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    Func_02005114();
  else if (scene == (s32)&SceneId_Kibombo4)
    Func_0200516c();
  else if (scene == (s32)&SceneId_Kibombo2)
    Func_020054e0();
  else if (scene == (s32)&SceneId_Kibombo5)
    Func_02005594();
  else if (scene == (s32)&SceneId_Kibombo6)
    Func_020057e0();
  else if (scene == (s32)&SceneId_Kibombo3)
    Func_02005898();
  return 0;
}

#include "types.h"

s32 Scene_PrepareMap(void) { return 0; }

void Func_020050ac(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Func_02005c04(0);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 3);
  Func_02005c54();
  Func_02005c5c(0, 0x9e9, 12, 13);
  Actor_GetById(22)->collisionFlags |= 0x80;
  Actor_GetById(23)->collisionFlags |= 0x80;
}

void Func_02005114(void) {
  Actor_SetHorizontalPositionWithTerrain(11, PIXELS(88), PIXELS(314));
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->scaleY = 0x8000;
  Func_020050ac();
  GameFlag_ClearBit(0x86b);
  Func_02005b8c(25);
  Actor_SetModeById(25, 2);
  KIBOMBO_TASK_ADD(Func_02003030, 0x480)
  Func_02003184();
}

