/// @file
/// Scene code of overlay 0x681, in ROM order, from s2020's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02004e3c(struct Actor *object, s32 value);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004ec4(s32 id, s32 target);
extern u8 MsgGaiaRockInside01[];

void Func_02002020(void) {
  s32 t1, t2, t26, t27, t28, t34, t35, t37, t38, t43, t44, t50, v_r5, v_r6, v_r7,
      v_r8;
  struct Actor *v_r10;

  t1 = ((s32)Actor_Get(8));
  v_r7 = t1;
  t2 = ((s32)Actor_Get(9));
  v_r8 = t2;
  v_r10 = Actor_Get(13);
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgGaiaRockInside01, 1);
  Event_PlaySound(193);
  Func_02004e3c(v_r10, 1);
  v_r5 = (s32)&gMainRoutineWork;
  v_r5 = (v_r5 + 0x214);
  Func_02004ec4(13, FIELD_AT(v_r5, s32, 0));
  Actor_SetDestination(13, 496, 712);
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  FieldActor_WaitForMoveById(13);
  FieldEvent_Wait(45);
  Actor_FaceActor(8, FIELD_AT(v_r5, s32, 0), 0);
  Actor_FaceActor(9, FIELD_AT(v_r5, s32, 0), 0);
  Event_PlaySound(0x240);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 5);
  System_Wait(30);
  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimation(8, 3);
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(40);
  Event_PlaySound(191);
  t26 = Event_GetFlag(0xa62);
  v_r5 = 119;
  if (t26 != 0)
    goto L_0200a1d8;
  v_r5 = 19;
L_0200a10a:
  t27 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t27;
  t28 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t28;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a10a;
  Actor_SetSpeed(8, 0x4000, 0xccc);
  Actor_SetSpeed(9, 0x4000, 0xccc);
  Actor_SetDestinationOffset(8, 24, 0);
  Actor_SetDestinationOffset(9, -24, 0);
  v_r6 = v_r7;
  v_r6 = (v_r6 + 86);
  v_r5 = 0;
  for (; v_r5 < 300 && FIELD_AT(v_r6, u8, 0) != 0; v_r5++) {
    t34 = Random16Far();
    FIELD_AT(v_r7, s32, 40) = t34;
    t35 = Random16Far();
    FIELD_AT(v_r8, s32, 40) = t35;
    System_Wait(1);
  }
  v_r5 = 9;
L_0200a18c:
  t37 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t37;
  t38 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t38;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a18c;
  v_r5 = 40;
  MapGrid_CopyCells(119, 31, 1, 2, 89, v_r5);
  MapGrid_CopyCells(119, 31, 1, 2, 100, v_r5);
  Event_SetFlag(0xa62);
  goto L_0200a1f8;
L_0200a1d8:
  t43 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t43;
  t44 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t44;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a1d8;
  FieldEvent_Wait(30);
L_0200a1f8:
  Actor_SetAnimation(8, 4);
  Actor_SetAnimation(9, 4);
  FieldEvent_Wait(30);
  t50 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t50 == 0)
    goto L_0200a22e;
  Actor_SetDestination(13, FIELD_AT(t50, s16, 10), FIELD_AT(t50, s16, 18));
L_0200a22e:
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  FieldActor_WaitForMoveById(13);
  Actor_SetPosition(13, 0, 0);
  Func_02004e64();
}

#include "actor.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "ram_buffer.h"

void Func_02004fdc(void);

/// Three scene events of one shape, for objects 8 and 12, 9 and 12, and 8 and
/// 14: the first object shakes with random vertical speeds, and while the flag
/// of its event (0x9bd, 0x9be, 0x9bc) is clear it moves, two map copies open
/// the way and the partner returns to the leader.
void Func_0200225c(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(12);
  Func_02004e5c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(12, gMainRoutineWork.currentOwner);
  Actor_SetDestination(12, 528, 372);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(12);
  Event_Wait(45);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  Event_Wait(30);
  Actor_SetModeById(8, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Test(0x9bd)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Actor_SetSpeed(8, 0x4000, 0xccc);
    Actor_OffsetPositionAndResetMotion(8, 24, 0);
    MapGrid_CopyCellAttributes(27, 23, 1, 1, 28, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 29, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 28, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x308);
      Event_Wait(30);
      Func_02004fdc();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_SetBit(0x9bd);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Func_02004e64();
  }
}

void Func_020024b8(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(9);
  partner = Actor_GetById(12);
  Func_02004e5c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(12, gMainRoutineWork.currentOwner);
  Actor_SetDestination(12, 672, 372);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(12);
  Event_Wait(45);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(9, 5);
  wait(30);
  Actor_SetModeById(9, 2);
  Event_Wait(30);
  Actor_SetModeById(9, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Test(0x9be)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Actor_SetSpeed(9, 0x4000, 0xccc);
    Actor_SetDestination(9, 608, 352);
    MapGrid_CopyCellAttributes(35, 23, 1, 1, 36, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 37, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 36, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(9, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x309);
      Event_Wait(30);
      Func_02004fdc();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_SetBit(0x9be);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(9, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Func_02004e64();
  }
}

void Func_02002718(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(14);
  Func_02004e5c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Func_02004e3c(partner, 1);
  Func_02004ec4(14, gMainRoutineWork.currentOwner);
  Actor_SetDestination(14, 336, 396);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(14);
  Event_Wait(45);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  Event_Wait(30);
  Actor_SetModeById(8, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Test(0x9bc)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Actor_SetSpeed(8, 0x4000, 0xccc);
    Actor_SetDestinationOffset(8, -16, 0);
    Map_CopyCellAttributeRect(27, 15, 2, 1, 23, 23);
    Map_CopyCellAttributeRect(25, 25, 1, 1, 25, 23);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16Far();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(1, 85, 1, 4, 25, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(14, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x307);
      Event_Wait(30);
      Func_02004fdc();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_SetBit(0x9bc);
      Func_02004e64();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16Far();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_ResetAndSetPosition(14, *(s16 *)((u8 *)leader + 10),
                                *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Func_02004e64();
  }
}

// Records which of eleven listed cells persons 10 and 11 stand on, as the two halves of the byte flag 0x210.

extern const s16 gData_0200530c[][2];
s32 GameFlag_SetByte(s32 flag, s32 value);

/// Records which of eleven listed cells persons 10 and 11 stand on, as the two
/// halves of the byte flag 0x210.
s32 Func_02002974(void) {
  struct Actor *first = Actor_Get(10);
  s8 firstIndex = 0;
  s32 firstX = first->x >> 19;
  s8 secondIndex = 0;
  s32 firstZ = first->z >> 19;
  struct Actor *second = Actor_Get(11);
  s32 secondX = second->x >> 19;
  s32 secondZ = second->z >> 19;
  s8 i;

  for (i = 1; i <= 11; i++) {
    if (firstX == gData_0200530c[i][0] && firstZ == gData_0200530c[i][1])
      firstIndex = i;
    if (secondX == gData_0200530c[i][0] && secondZ == gData_0200530c[i][1])
      secondIndex = i;
  }
  {
    u32 high = secondIndex << 28;
    return GameFlag_SetByte(0x210, (s32)((firstIndex << 24) | high) >> 24);
  }
}

s32 Func_02004af0();

void Func_02002a14(void) { Func_02004af0(0xb, 0x303); }

void Func_02002a24(void) { Func_02004af0(0xc, 0x304); }

void Func_02002a34(void) { Func_02004af0(0xd, 0x305); }

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
extern const u8 gData_02005ee8[];
extern const u8 gData_02005f30[];
extern const u8 gData_02005fcc[];
extern const u8 gData_02006038[];
extern const u8 gData_020060f8[];
extern const u8 gData_02006164[];
extern const u8 gData_020061a0[];

/// Returns the current room's table.
const u8 *Func_02002a44(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2)
    return gData_02006038;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room3)
    return gData_020060f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room4)
    return gData_02006164;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_020061a0;
  if (gMainRoutineWork.entrance == 9)
    return gData_02005f30;
  if (gMainRoutineWork.entrance > 9)
    return gData_02005fcc;
  return gData_02005ee8;
}
