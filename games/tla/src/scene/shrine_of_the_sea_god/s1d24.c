#include "shrine_of_the_sea_god.h"
#include "text/msg_ids.h"
#include "call.h"

/// A word of the object written through a one-member union (see the
/// FAKEMATCH(signature) tag in Func_02001d24).
union ObjectWord {
  s32 value;
};

extern const u8 gData_02003c58[];

void Func_02003990(struct ShrineOfTheSeaGodObject *object, s32 x, s32 y, s32 z);
void Func_02003a90(s32 actor, s32 angle);

static inline void ShrineOfTheSeaGod_ObjAngle(s32 actor, s32 angle) {
  Func_02003a90(actor, angle);
}
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);

void Func_02001d24(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *leader;
  struct ShrineOfTheSeaGodSprite *sprite;
  u8 *motionFlags;
  const void *callback;

  if (ShrineOfTheSeaGod_FlagCheck(0x873))
    return;
  Func_020039d8();
  Func_02003b08(0);
  Func_02003ad0()->motionFlags = 0;
  ShrineOfTheSeaGod_CamMove(170 << 17, -1, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 344, 184);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjPlace(14, 344 << 16, 184 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 356, 196);
  /* FAKEMATCH(wrapper): Func_02003a90 called through the inline
   * ShrineOfTheSeaGod_ObjAngle, here and below; as direct calls, 128 of 656
   * instructions differ in tla-en. */
  ShrineOfTheSeaGod_ObjAngle(14, 0xb000);
  Func_02003a70((s32)MsgFieldPraiseRopeWork);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_ObjPlace(13, 184 << 16, 168 << 16);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x8000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(232 << 16, 64 << 16, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(13, 232, 168);
  FieldEvent_Wait(20);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_CamSpeed(0x13333, 0x2666);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  object = Actor_GetById(13);
  sprite = object->sprite;
  Actor_SetSpritePriority(13, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 248, 168);
  Actor_SetModeById(13, 6);
  /* FAKEMATCH(temporary): the first motion flags store written through a byte
   * pointer, so it may alias the rotation and stays after it; as
   * object->motionFlags, which keeps the struct's alias set, 12 of 656
   * instructions differ in tla-en. */
  motionFlags = &object->motionFlags;
  /* FAKEMATCH(signature): field18 written through a one-member union, so both
   * its stores may alias the sprite's rotation and stay ahead of it; as
   * object->field18, 4 of 656 instructions differ in tla-en. */
  ((union ObjectWord *)&object->field18)->value = -0x10000;
  sprite->rotation = 0xd000;
  *motionFlags = 0;
  object->y += 0x60000;
  Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x6666, 0x3333);
  /* FAKEMATCH(wrapper): Func_02003990 called through Call4; as a direct call,
   * 2 of 656 instructions differ in tla-en. */
  Call4((void (*)())Func_02003990, (s32)object, 164 << 17, 152 << 14,
        168 << 16);
  // A once-only block: its end holds the order around it (as the plain
  // statement, 2 of 656 differ in tla-en).
  do {
    Actor_WaitForMoveById(13);
  } while (0);
  object->motionFlags = 3;
  object->height = 0x200000;
  ((union ObjectWord *)&object->field18)->value = 0x10000;
  sprite->rotation = 0;
  Actor_SetModeById(13, 1);
  Actor_SetSingleChildFlags(Actor_GetById(13), 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 328, 184);
  ShrineOfTheSeaGod_ObjTurn(13, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 256, 40);
  Actor_SetSpritePriority(13, 2);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(0x200e, 0);
  ShrineOfTheSeaGod_Jump(13, 257, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0xb000, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  Func_02003a78(0x200e, 0, 60);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(13, 3);
  ShrineOfTheSeaGod_ObjPlace(12, 394 << 16, 286 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 248);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Jump(12, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x3000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(188 << 17, -1, 264 << 16, 1);
  Func_02003ac8();
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_CamSpeed(0x26666, 0x4ccc);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(14, 12);
  Actor_WatchAndSetScript(13, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 220);
  ShrineOfTheSeaGod_ObjWalk(12, 370, 198);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjCommit(14, 368, 180);
  Actor_WatchAndSetScript(14, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 336, 198);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Stop(13);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_ArmCallback(12, 0xd000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjAngle(12, 0xb000);
  Actor_Jump(12, 4, 20);
  Func_02003a78(12, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 261, 40);
  ShrineOfTheSeaGod_Jump(12, 257, 60);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 20);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0x8000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 265, 40);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 40);
  Actor_Jump(13, 4, 40);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(12, 3);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x13333, 0x9999);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x13333, 0x9999);
  callback = gData_02003c58;
  Actor_SetMovementTypeById(12, callback);
  FieldEvent_Wait(10);
  Actor_SetMovementTypeAndWaitById(13, callback);
  FieldEvent_Wait(40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(14);
  wait(1);
  Func_02003a78(14, 0, 40);
  ShrineOfTheSeaGod_ObjPlace(13, 0, 0);
  ShrineOfTheSeaGod_ObjPlace(12, 0, 0);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_ResetAndSetPosition(14, leader->x >> 16, leader->z >> 16);
  Actor_WaitForMoveById(14);
  ShrineOfTheSeaGod_ObjPlace(14, 0, 0);
  GameFlag_SetBit(0x873);
  Func_020039e0();
}
