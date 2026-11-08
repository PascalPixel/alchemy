#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "call.h"

extern u8 MsgKandoreanTemple08;

void Func_02005330(struct Actor *object, s32 mode);
void Func_02005148(struct Actor *object);
void PartyInventory_GiveItem(s32 item, s32 count);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_WatchAndSetScript(s32 id, s32 target);

void Func_020023e8(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 180 of 260 instructions differ in
   * tla-en. */
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->angle;
  struct Actor *object;
  struct FieldSprite *sprite;

  Func_020051c0();
  Call1((void (*)())Func_02005370, 0);
  Func_020052f0()->motionFlags = 0;
  ((void (*)())Actor_ArmCallback)(12, 0);
  Func_02005280((s32)&MsgKandoreanTemple08);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Call3((void (*)())Actor_SetAngleToward, gMainRoutineWork.currentOwner, 12, 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Call2((void (*)())Actor_SetModeById, 12, 4);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation,
        gMainRoutineWork.currentOwner, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 12, 3);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, (u16)angle, 10);
  if (gData_020062ac != 0)
    Func_02005330(gData_020062ac, 3);
  Call2((void (*)())PartyInventory_GiveItem, 198, 0);
  if (gData_020062ac != 0)
    Func_02005148(gData_020062ac);
  Call2((void (*)())Actor_SetModeById, 12, 4);
  Call2((void (*)())Func_02005298, SPLIT_CONSTANT(0x8000, 12), 0);
  Call2((void (*)())Func_020052d8, SPLIT_CONSTANT(0x6600, 102), SPLIT_CONSTANT(0xc00, 204));
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1a80000, 0, 0x1900000, 1);
  object = Actor_GetById(12);
  sprite = object->animation;
  Call3((void (*)())Actor_SetSpeedParameters, 12, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
  Call3((void (*)())Actor_SetPositionAndReset, 12, 424, 376);
  Call2((void (*)())Actor_SetSpritePriority, 12, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, SPLIT_CONSTANT(0xcc00, 204));
  Call3((void (*)())Actor_Jump, 12, 4, 0);
  Call3((void (*)())Actor_SetPositionAndCommit, 12, 444, 376);
  Call2((void (*)())Actor_SetModeById, 12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  object->motionFlags = 0;
  object->y += 0x40000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x6666, SPLIT_CONSTANT(0x3300, 51));
  Call4((void (*)())Func_02005160, (s32)object, 0x1ec0000, 0x80000, 0x1780000);
  Call1((void (*)())Actor_WaitForMoveById, 12);
  object->motionFlags = 3;
  object->terrainHeight = 0;
  object->scaleX = 0x10000;
  sprite->rotation = 0;
  Call2((void (*)())Actor_SetModeById, 12, 1);
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, SPLIT_CONSTANT(0xcc00, 204));
  Call3((void (*)())Actor_Jump, 12, 4, 0);
  Call3((void (*)())Actor_SetPositionAndCommit, 12, 504, 376);
  Call3((void (*)())Actor_SetSpeedParameters, 12, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
  Call3((void (*)())Actor_SetPositionAndReset, 12, 496, 394);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 432, 394);
  Call2((void (*)())Actor_WatchAndSetScript, 12, gMainRoutineWork.currentOwner);
  Call2((void (*)())Actor_SetSpritePriority, 12, 2);
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x800, 74));
  Func_020051c8();
}
