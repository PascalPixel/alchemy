#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001d90(void) {
  Task_AddFrame(Func_02001cfc, 0x480);
  Task_AddFrame(Func_02001cac, 0xc80);
}

void Func_02003fb4();
void Func_02003fbc();
void Func_02003fc4();

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
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(19, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(9, 2, 10);
  FieldActor_Jump(9, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 10);
  Event_ShowMessage(9, 0);
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  Actor_SetItemIcon(item, 0x1c4);
  FieldActor_SetPositionAndResetMotion(item, leader->x, leader->y + PIXELS(24),
                                  leader->z);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Party_GiveItem(0x1c4, 0);
  Func_02003fbc();
  Actor_SetPosition(20, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_SetFlag(0xaa2);
  Func_02003e6c();
}
