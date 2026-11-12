/// @file
/// The scene code of overlay 0x672 that its scene tables belong to, in ROM order; the rest of the overlay is in gabomba_statue_chamber/.

#include "text/msg_ids.h"

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "gabomba_statue_chamber/gabomba_statue_chamber.h"

u8 *Func_02000040(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov672Room1)
    return gData_0200a08c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

u8 *Func_02000070(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov672Room1)
    return gData_020020f8;
  if (scene == (s32)&SceneId_Ov672Room3)
    return gData_020021e8;
  return gData_02002458;
}

u8 *Func_020000b0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov672Room1)
    return gData_02002470;
  if (scene == (s32)&SceneId_Ov672Room2)
    return gData_020024a0;
  if (scene == (s32)&SceneId_Ov672Room3)
    return gData_020024dc;
  return gData_02002530;
}

#include "io_reg.h"
#include "event_inline.h"

void Func_02000100(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);
  struct GabombaStatueChamberEventWork *work;

  Actor_SetModeById(8, 2);
  PaletteFade_ApplySource(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 2);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  work = Ram_HeapSlots->eventWork;
  Func_0803ce64((s16)work->field1c4++, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 2);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Actor_SetModeById(8, 1);
}

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern struct Actor *gData_02002058;
extern const u8 gData_02001f78[];
extern const u8 gData_02001f9c[];
extern const u8 gData_02001fe4[];
extern const u8 gData_0200200c[];
#define OBJECT(id) (Actor_Get((id)))
void Func_02001094(void);
void Func_02001d78(void);
void Func_02001d80(void);

void Func_02000184(void) {
  struct Actor *object, *leader;
  u32 i;
  volatile u16 *color;

  Func_02001da0();
  Event_ResolvePendingActions(0);
  Sound_Play(26);
  Sound_Play(212);
  Actor_SetModeById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 1);
  BattleEffect_ApplyColorToTargetBuffer(0x403108, 1);
  BattleEffect_StartBufferInterpolation(40);
  wait(40);
  Sound_Play(197);
  wait(1);
  if (gData_02002058 != 0) {
    i = 0;
  rise:
    if (i == 20)
      Actor_ShowEmote(10, 0x100, 0);
    gData_02002058->y += 0x8000;
    wait(1);
    if (++i <= 79)
      goto rise;
    gData_02002058->motionFlags = 4;
  }
  Actor_SetModeById(8, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 358, 108);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Func_02001e20(9, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkToAndWait(9, 364, 124);
  Actor_FaceDirection(9, 0xd000, 0);
  Event_SetMessageId(MsgOhYoungWitch);
  Func_02000100();
  Actor_ArmCallback(10, 0, 20);
  Actor_FaceDirection(10, 0x8000, 40);
  Actor_ArmCallback(10, 0, 20);
  Event_ShowMessage(0x200a, 0);
  Actor_SetAttachedEffect(9, 258);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(10, 0xb000);
  Actor_Jump(10, 4, 20);
  Event_ShowMessageAndWait(0x200a, 0);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
  Actor_FaceDirection(9, 0xb000, 40);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(9, 0x100, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x2000, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_SetModeById(8, 3);
  Actor_ArmCallback(10, 0x3000, 20);
  object = OBJECT(14);
  object->angle = OBJECT(10)->angle;
  wait(1);
  Func_02001e20(14, 10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Sound_Play(110);
  Actor_SetModeById(14, 5);
  Event_Wait(60);
  Event_Wait(20);
  Actor_FaceDirection(14, 0xb000, 0);
  Actor_SetModeById(8, 1);
  Event_Wait(40);
  Actor_SetModeById(8, 3);
  Sound_Play(110);
  Actor_SetModeById(14, 6);
  Event_Wait(60);
  Actor_SetModeById(14, 1);
  Event_Wait(20);
  Actor_SetModeById(8, 1);
  Event_Wait(40);
  Func_02000100();
  Actor_Jump(14, 2, 10);
  Actor_Jump(14, 4, 10);
  Event_ShowMessage(0x200e, 0);
  Actor_SetMovementTypeAndWaitById(14, gData_02001f78);
  Event_ShowMessageAndWait(0x200e, 0);
  Actor_SetSpeed(14, 0x19999, 0xcccc);
  Actor_WalkToAndWait(14, 392, 164);
  Func_080d47b4(14, 264, 0);
  Actor_Jump(14, 6, 20);
  Event_ShowMessageAndWait(0x600e, 0);
  Actor_WalkToAndWait(14, 392, 220);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0, 0);
  OBJECT(12)->motionFlags = 0;
  Actor_SetMovementTypeAndWaitById(12, gData_02001f9c);
  Event_Wait(20);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(9, 257, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_FaceDirection(9, 0xd000, 0x50);
  Func_02000100();
  Event_Wait(20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x2000, 20);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x8000, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x2000, 20);
  Actor_SetModeById(8, 3);
  PaletteFade_ApplySource(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 2);
  /* FAKEMATCH(temporary): the palette entry's address taken before the fade's
   * wait; as OBJ_PLTT_COLOR(0, 15) = 0x7fff after it, 4 of 895 instructions
   * differ in tla-en. */
  color = &OBJ_PLTT_COLOR(0, 15);
  BattleEffect_StartBufferInterpolation(10);
  wait(10);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s16)(*(u16 *)(EVENT_WORK + 0x1c4))++, 0, 2);
  PaletteFade_ApplyTarget(0x10000, 2);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Actor_SetModeById(8, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x100, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
  Actor_FaceDirection(9, 0xb000, 20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_FaceDirection(9, 0xd000, 0);
  Func_02000100();
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(9, 257, 40);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0, 0);
  Actor_SetMovementTypeAndWaitById(12, gData_02001fe4);
  Func_02000100();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  Actor_SetAttachedEffect(9, 258);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_FaceDirection(9, 0xd000, 20);
  Func_02000100();
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
  Actor_FaceDirection(9, 0xb000, 20);
  Func_02000100();
  Actor_FaceDirectionAndWait(9, 0xd000);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Func_080d47b4(13, 257, 40);
  Func_02000100();
  Event_ShowMessageThenWait(9, 0, 40);
  Func_02000100();
  Actor_Jump(9, 4, 10);
  Event_ShowMessageAndWait(9, 0);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  Actor_FaceDirectionAndWait(9, 0xb000);
  Motion_SetModeAndWaitAnimation(9, 3);
  Func_02000100();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_FaceDirectionAndWait(9, 0xd000);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Func_02000100();
  Sound_Play(0);
  Sound_Play(220);
  Actor_SetModeById(8, 3);
  Actor_SetPartAttribute(((s32)Actor_GetById(13)), 7);
  Actor_SetPosition(13, 0x1a80000, 0x680000);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(40);
  Func_02001094();
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Event_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x403108, 1);
  BattleEffect_StartBufferInterpolation(60);
  wait(60);
  Func_02000100();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  gData_02002058->motionFlags = 3;
  Event_Wait(10);
  Sound_Play(309);
  Event_Wait(40);
  Sound_Play(25);
  Event_Wait(40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x2000, 0);
  Actor_FaceDirectionAndWait(9, 0xb000);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 392, 104);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0xc000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  if (gData_02002058 != 0) {
    leader = OBJECT(gMainRoutineWork.currentOwner);
    object = gData_02002058;
    object->motionFlags = 0;
    Actor_SetMoveTarget((s32)object, leader->x, leader->y + 0x240000,
                        leader->z);
    Script_WaitForEventTimeout((s32)gData_02002058);
  }
  PartyInventory_GiveItem(242, 0);
  if (gData_02002058 != 0)
    Func_02001d30((s32)gData_02002058);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x6000, 0);
  Actor_FaceDirection(9, 0xd000, 40);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(9, 4);
    Event_ShowMessageAndWait(9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Actor_SetModeById(9, 3);
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_SetPositionAndReset(9, 376, 124);
  Event_Wait(20);
  Event_ShowMessageAndWait(9, 0);
  Actor_FaceDirectionAndWait(9, 0xd000);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessageAndWait(9, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Event_ShowMessageAndWait(9, 0);
  }
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetMovementTypeById(9, gData_0200200c);
  Event_Wait(20);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_0200200c);
  Event_Wait(40);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  BattleEffect_StartBufferInterpolation(60);
  wait(0x40);
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
  *(s32 *)(EVENT_WORK + 0x1b4) = 10;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  *(s32 *)(EVENT_WORK + 0x1b4) = 1;
  BattleEffect_StartBufferInterpolation(1);
  Func_02001d78();
  Func_0803ce64(MsgCarriedTheMessage, 0, 3);
  Func_02001d80();
  GameFlag_SetBit(0x8ff);
  GameFlag_ClearBit(0x86b);
  GameFlag_ClearBit(0x8f2);
  gMainRoutineWork.retreatScene = (s32)&SceneId_Kibombo1;
  gMainRoutineWork.retreatEntrance = 1;
  gMainRoutineWork.savedScene = (s32)&SceneId_Kibombo3;
  gMainRoutineWork.savedEntrance = 6;
  Import_EventSetExitRequest(3);
}

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.currentOwner));

  Func_02001da0();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(((struct GabombaStatueChamberObject *)Actor_GetById(
                                gMainRoutineWork.currentOwner)),
                            0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(object, x << 16, PIXELS(-12), z << 16);
  Script_WaitForEventTimeout(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02000b60(void) { Func_02000a98(312, 144); }

#include "input.h"

void Func_02000b70(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(
          gMainRoutineWork.currentOwner));
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 26) {
    if (z == 5 && (gInput.held & 128) == 0)
      return;
    if (z == 7 && (gInput.held & 64) == 0)
      return;
  } else if (z == 6) {
    if (x == 25 && (gInput.held & 16) == 0)
      return;
    if (x == 27 && (gInput.held & 32) == 0)
      return;
  }
  Func_02000a98(424, 100);
}

#include "system/io_reg.h"

void Func_02001d60(void);
void Func_02001d40(void);

void Func_02000be8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Func_02001da0();
  Event_ResolvePendingActions(0);
  Func_02001d60();
  step = 1;
  for (i = 0; i < 352; i++) {
    if (i >= 17 && i < 80) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
  }
  Func_02001d40();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 328, 184);
  Event_Wait(20);
  Sound_Play(308);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (; i != 0; i--) {
    if (i >= 33 && i < 96) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Sound_Play(298);
  wait(1);
  Sound_Play(125);
  Event_Wait(20);
  GameFlag_SetBit(0x206);
  Func_02001da8();
}

void Func_02000d3c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Func_02001da0();
  Event_ResolvePendingActions(0);
  Func_02001d60();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0x4000);
  Sound_Play(308);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (i = 0; i <= 351; i++) {
    if ((u32)(i - 17) <= 62) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Sound_Play(298);
  wait(1);
  Sound_Play(125);
  Event_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
  GameFlag_SetBit(0x206);
  GameFlag_SetBit(0x207);
  Func_02001da8();
}

void Func_02000e50(void) { GameFlag_ClearBit(518); }

void Func_02000e60(void) {
  Func_02001da0();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(392), -1, PIXELS(120), 1);
  Func_02001ea8();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgIHaveFollowed);
  Actor_SetAttachedEffect(10, 0x102);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionAndWait(10, FACING_NORTH + FACING_STEP);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageAndWait(10, 0);
  GameFlag_SetBit(0x8fe);
  Func_02001da8();
}

void Func_02000ef0(struct Actor *object) {
  object->x = Actor_GetById(8)->x;
}
