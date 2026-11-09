#include "islets.h"
#include "scene_tables.h"

void Func_02001d90(void) {
  Value2((s32 (*)())Task_Add, (s32)Func_02001cfc, 0x480);
  Value2((s32 (*)())Task_Add, (s32)Func_02001cac, 0xc80);
}

void Func_080dc978();
void Func_02003fb4();
void Func_02003fbc();
void Func_02003fc4();
void Actor_SetHorizontalPositionWithTerrain();
void Actor_Jump();
void PartyInventory_GiveItem();

/// When object 19 stands in its target area, actor 9 and the leader react and
/// the party is given item 0x1c4.
void Func_02001db0(void) {
  struct Actor *object = Actor_GetById(19);
  struct Actor *leader;
  struct Actor *item;
  s32 z;

  if (object->x < 0xe40000 || object->x > 0xec0000)
    return;
  z = object->z;
  /* FAKEMATCH: the one-pass block loads z before 0xa40000 is built; plain, the
   * ARM9TDMI schedule starts the constant first (differ 2), and z's two tests
   * stay apart as the native's are (one range test differs by 71). */
  do {
  } while (0);
  if (z < 0xa40000)
    return;
  if (z > 0xac0000)
    return;
  Func_02003e64();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(19, 1);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 216, 184);
  Func_080dc978(gMainRoutineWork.currentOwner, 9);
  Event_SetMessageId((s32)MsgThankYouSo);
  Actor_Jump(9, 2, 10);
  Actor_Jump(9, 4, 10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 9, FACING_WEST, 10);
  Event_ShowMessageAndWait(9, 0);
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_SetPositionAndReset(9, 222, 186);
  Event_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_SetPositionAndReset(9, 232, 186);
  Event_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  Call2((void (*)())Func_0802372c, (s32)item, 0x1c4);
  Actor_SetPositionAndResetMotion(item, leader->x, leader->y + PIXELS(24),
                                  leader->z);
  Event_Wait(20);
  Func_02003fc4();
  Value2((s32 (*)())PartyInventory_GiveItem, 0x1c4, 0);
  Func_02003fbc();
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02003fb4();
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 9,
        FACING_NORTHWEST + FACING_STEP);
  GameFlag_SetBit(0xaa2);
  Func_02003e6c();
}
