#include "tundaria_tower.h"
#include "call.h"

void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetActionById(u32 id, s32 action);
void Motion_SetVarCbAndRefresh(u32 id, s32 variant);
void Func_080d489c(s32 id, s32 emote);
void Func_02003030(s32 x, s32 z, s32 a, s32 b);

/// The leader turns to angle 0x4000 and takes mode 22, shows emote 0x102, then
/// turns to 0xc000 and takes mode 5 and action 24.
void Func_02000b3c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02003048();
  Event_ResolvePendingActions(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  leader->angle = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Event_Wait(30);
  Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
  Motion_SetVarCbAndRefresh(gMainRoutineWork.currentOwner, 2);
  Event_Wait(20);
  leader->angle = 0xc000;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 5);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 24);
  Event_Wait(40);
  Func_02003030(0x1300000, 0x3900000, 0, -12);
  Actor_GetById(gMainRoutineWork.currentOwner)->bounce = 0;
  Import_EventSetExitRequest(11);
  Func_02003050();
}

void Func_02000bfc(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov68fRoom6) {
    struct Actor *object;

    Func_02003170(gData_02003264);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->action = ((object->x >> 20) << 8) | ((object->z >> 20) & 0xff);
  } else if (party->scene == (s32)&SceneId_Ov68fRoom7) {
    Func_02003170(gData_0200326e);
  }
}
