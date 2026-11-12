/// @file
/// Scene code of overlay 0x681, in ROM order, from s225c's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "iwram_call.h"
#include "ram_buffer.h"

void Func_02004e3c(struct Actor *object, s32 value);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004ec4(s32 id, s32 target);
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
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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
