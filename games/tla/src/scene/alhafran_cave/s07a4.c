#include "local.h"
#include "call.h"

void Func_02001230(const u16 *table);
void Func_02001488(s32 actor, s32 x, s32 z);
void Func_02001520(s32 value);
void Func_02001568(void);
void Func_02001570(s32 a, s32 b, s32 c, s32 d);
void Func_02001598(s32 actor, s32 value);
void Func_020015e0(s32 value);
void Func_020015e8(const void *data, s32 a, s32 b, s32 c);
void Func_02001220(void);
s32 Func_02000258(void);
void Scheduler_AddOrUpdateCallback(s32 (*callback)(void), s32 priority);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);

/// The rooms' setup: sprite priorities, the scene callback, actors 12 to 14,
/// actors 15 to 24 by flags 0x8a7, 0x8ab and 0x8bd, and actor 8 stepping in
/// at entrance 9 until flag 0x109 is set.
void Func_0200051c(void) {
  /* FAKEMATCH: the data address is loaded before Func_020015e0 and kept across
   * it, as the native does (differ 31 loaded at its call). */
  const void *data;
  struct AlhafranCaveEventWork *work;

  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  Object_GetById(8)->priorityFlags |= 32;
  Func_02001520(0);
  data = gData_02001be0;
  Func_020015e0(0);
  Call4((void (*)())Func_020015e8, (s32)data, 8, 15, 0x40000000);
  Scheduler_AddOrUpdateCallback(Func_02000258, 0x480);
  if ((u16)(gPartyState.entrance - 8) <= 1) {
    work = Ram_HeapSlots->eventWork;
    /* FAKEMATCH: a one-pass block loads the event work before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    work->valueD4c = 3;
  }
  Func_02001568();
  Func_02001570(0, 0x200, 10, 11);
  Object_GetById(10)->priorityFlags |= 32;
  Object_GetById(11)->priorityFlags |= 32;
  Func_02001230(gData_020016e4);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
  Call2((void (*)())Func_02001598, 14, 0x1000);
  Object_SetModeById(12, 0);
  Object_SetModeById(13, 1);
  Object_SetModeById(14, 2);
  ObjectMotion_SetActionVariant(12, 1);
  ObjectMotion_SetActionVariant(13, 1);
  Object_GetById(12)->priorityFlags |= 2;
  Object_GetById(13)->priorityFlags |= 2;
  Object_GetById(14)->priorityFlags |= 2;
  Animation_ApplyChildValues(Object_GetById(12), 6);
  Animation_ApplyChildValues(Object_GetById(13), 6);
  Animation_ApplyChildValues(Object_GetById(14), 6);
  if (GameFlag_Test(0x8a7)) {
    Func_02001488(15, 0, 0);
    Func_02001488(18, 0, 0);
    Func_02001488(16, 0, 0);
    Func_02001488(17, 0, 0);
    Func_02001488(19, 0, 0);
    Func_02001488(20, 0, 0);
    Func_02001488(21, 0, 0);
    Func_02001488(22, 0, 0);
    Func_02001488(24, 0, 0);
    Object_GetById(23)->update = Func_02001220;
  } else if (GameFlag_Test(0x8ab)) {
    Func_02001488(23, 0, 0);
    ObjectMotion_SetActionVariant(18, 2);
    ObjectMotion_SetActionVariant(16, 2);
    ObjectMotion_SetActionVariant(17, 2);
    ObjectMotion_SetActionVariant(19, 2);
    ObjectMotion_SetActionVariant(20, 2);
    if (!GameFlag_Test(0x8bd)) {
      Call3((void (*)())Func_02001488, 17, 0x2c80000, 0xd80000);
      Call3((void (*)())Func_02001488, 19, 0x2e80000, 0xd80000);
    } else {
      Func_02001488(22, 0, 0);
      Func_02001488(21, 0, 0);
    }
  } else {
    Func_02001488(18, 0, 0);
    Func_02001488(16, 0, 0);
    Func_02001488(17, 0, 0);
    Func_02001488(19, 0, 0);
    Func_02001488(20, 0, 0);
    Func_02001488(21, 0, 0);
    Func_02001488(22, 0, 0);
    Func_02001488(24, 0, 0);
    Object_GetById(23)->update = Func_02001220;
  }
  if (gPartyState.entrance == 9 && !GameFlag_Test(0x109)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x4ccc, 0x2666);
    ObjectMotion_OffsetPositionAndResetMotion(8, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(8);
  }
}

s32 Func_020007a4(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov66aRoom2 && GameFlag_Test(0x8a7)) {
    Func_02001408_(23, 46, 4, 5, 21, 14);
    Func_02001408_(86, 44, 5, 6, 84, 12);
    Func_02001400_(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

void Func_02000814(void) {}
