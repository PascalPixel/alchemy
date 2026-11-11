#include "scene_tables.h"
#include "local.h"
#include "call.h"

void Func_02001230(const u16 *table);
void Func_02001520(s32 value);
void Func_02001568(void);
void Func_02001570(s32 a, s32 b, s32 c, s32 d);
void Func_020015e0(s32 value);
void Func_020015e8(const void *data, s32 a, s32 b, s32 c);
void Func_02001220(void);
s32 Func_02000258(void);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// The rooms' setup: sprite priorities, the scene callback, actors 12 to 14,
/// actors 15 to 24 by flags 0x8a7, 0x8ab and 0x8bd, and actor 8 stepping in
/// at entrance 9 until flag 0x109 is set.
void Func_0200051c(void) {
  /* FAKEMATCH(temporary): the data address held in a local, so it loads before
   * Func_020015e0 and is kept across it, as the ROM does; as loaded at its
   * call, 31 instructions differ. */
  const void *data;
  struct AlhafranCaveEventWork *work;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_Get(8)->priorityFlags |= 32;
  Func_02001520(0);
  data = gData_02001be0;
  Func_020015e0(0);
  Call4((void (*)())Func_020015e8, (s32)data, 8, 15, 0x40000000);
  FieldTask_AddFrame(Func_02000258, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 8) <= 1) {
    work = Ram_HeapSlots->eventWork;
    
    work->valueD4c = 3;
  }
  Func_02001568();
  Func_02001570(0, 0x200, 10, 11);
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  Func_02001230(gData_020016e4);
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSpriteRotationById(14, 0x1000);
  Actor_SetAnimation(12, 0);
  Actor_SetAnimation(13, 1);
  Actor_SetAnimation(14, 2);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_SetSpritePriority(13, 1);
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Actor_Get(14)->priorityFlags |= 2;
  Animation_ApplyChildValues(Actor_Get(12), 6);
  Animation_ApplyChildValues(Actor_Get(13), 6);
  Animation_ApplyChildValues(Actor_Get(14), 6);
  if (Event_GetFlag(0x8a7)) {
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = Func_02001220;
  } else if (Event_GetFlag(0x8ab)) {
    Actor_SetPosition(23, 0, 0);
    FieldActor_SetSpritePriority(18, 2);
    FieldActor_SetSpritePriority(16, 2);
    FieldActor_SetSpritePriority(17, 2);
    FieldActor_SetSpritePriority(19, 2);
    FieldActor_SetSpritePriority(20, 2);
    if (!Event_GetFlag(0x8bd)) {
      Actor_SetPosition(17, PIXELS(712), PIXELS(216));
      Actor_SetPosition(19, PIXELS(744), PIXELS(216));
    } else {
      Actor_SetPosition(22, 0, 0);
      Actor_SetPosition(21, 0, 0);
    }
  } else {
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_Get(23)->update = Func_02001220;
  }
  if (gMainRoutineWork.entrance == 9 && !Event_GetFlag(0x109)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetSpeed(8, 0x4ccc, 0x2666);
    Actor_SetDestinationOffset(8, 0, 16);
    FieldActor_WaitForMoveById(8);
  }
}

s32 Func_020007a4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66aRoom2 && Event_GetFlag(0x8a7)) {
    Func_02001408_(23, 46, 4, 5, 21, 14);
    Func_02001408_(86, 44, 5, 6, 84, 12);
    Func_02001400_(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

void Func_02000814(void) {}
