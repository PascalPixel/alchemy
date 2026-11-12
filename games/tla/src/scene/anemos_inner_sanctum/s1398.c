#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x154];
  s16 unk154;
};

extern char SceneId_Ov6B0Room4;
extern const s32 gData_02003574[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_020032e0(struct Actor *object, s32 value);
void Func_020032e8(struct Actor *object, const s32 *script);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003430(void);
void Func_02003438(s32 id, s32 a);

static __inline__ struct Actor *Create(s32 type, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 4
   * of 159 instructions differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

/// An event handler from the sanctum's event table (in s2f48.c):
/// changes the cells at the leader's place, spawns an effect of type 0x15d
/// there and sets the flag the event work's unk154 selects; once flags 0x200 to
/// 0x206 are all set, it moves object 8 in room 4 (else object 9) to mode 2 and
/// sets flag 0x207.
void Func_02001398(void) {
  struct AnemosInnerSanctumEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 all;
  s32 i;
  s32 id;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    MapGrid_CopyCellsTo(17, 71, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  else
    MapGrid_CopyCellsTo(54, 87, leader->x >> 20, (leader->z >> 20) + 64, 1, 1);
  Event_PlaySound(106);
  object = Create(0x15d, ((leader->x >> 20) << 20) + 0x80000, 0,
                  ((leader->z >> 20) << 20) + 0x80000);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 4);
  object->scaleY = 0x16000;
  object->scaleX = 0x16000;
  Scene_SetActorSpritePriority(object, 2);
  Func_020032e0(object, 3);
  Actor_SetSingleChildFlags(object, 0);
  Func_020032e8(object, gData_02003574);
  Event_SetFlag(work->unk154 + 0x1ce);
  all = Event_GetFlag(0x200);
  for (i = 1; i != 7; i++)
    all &= Event_GetFlag(0x200 + i);
  if (all) {
    Func_02003398();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_Wait(10);
    FieldCamera_SetSpeed(0xcccc, 0x1999);
    id = 9;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
      id = 8;
    Func_02003438(id, 1);
    Func_02003430();
    FieldEvent_Wait(10);
    Event_PlaySound(0x246);
    Actor_SetAnimation(id, 2);
    Event_SetFlag(0x207);
    Actor_Get(id)->motionFlags = 4;
    FieldEvent_Wait(80);
    Func_020033a0();
  }
}

#include "scene_tables.h"

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Centres the party leader on its cell and raises actor 8 (in Ov6B0Room4) or
/// actor 9 with cue 198, 4 pixels at once and then 128 more, before the screen
/// transition finishes.
void Func_02001518(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object;

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    object = Actor_Get(8);
    FieldEvent_SetExitRequest(13);
  } else {
    object = Actor_Get(9);
    FieldEvent_SetExitRequest(11);
  }
  {
    s32 x = ((leader->x >> 20) << 4) + 8;
    s32 z = ((leader->z >> 20) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x, z);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  /* FAKEMATCH(wrapper): the camera move forwarded through FieldCamera_MoveTo,
   * so each -1 is built on its own, as in the ROM; as a direct call, the three
   * share one register, two instructions shorter, 12 of 100 instructions
   * differ in tla-en. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object->speedLimit = 0x20000;
  object->acceleration = 0x800;
  Event_PlaySound(198);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x40000, object->z);
  Actor_WaitForMove(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x800000, object->z);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001614(void) {
  s32 t0;
  s32 t1;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  t1 =
      MapGrid_GetCellBehaviorAt(2, FIELD_AT(t0, s32, 8), FIELD_AT(t0, s32, 16));
  if (t1 == 0xe8) {
    FIELD_AT((t0 + 0x22), u8, 0) = 2;
  }
}

void Func_02001644(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  FIELD_AT((t0 + 0x22), u8, 0) = 0;
}

void Func_02001660(void) {
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(14, 0, 0);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(0x200);
  Func_020033a0();
}

void Func_020016fc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(14));
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  FIELD_AT(t0, s32, 8) = 0x2180000;
  FIELD_AT(t0, s32, 16) = 0x1680000;
  FIELD_AT(t0, s32, 20) = -917504;
  FIELD_AT(t0, s32, 12) = -917504;
  MapGrid_CopyCellsTo(51, 22, 33, 22, 1, 1);
  Event_PlaySound(0x248);
  FieldEvent_Wait(30);
  Event_PlaySound(0xbc);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(52, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_ClearFlag(0x200);
  Func_020033a0();
}

s32 Func_020030e0();

void Func_0200178c(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    FieldEvent_SetExitRequest(0xb);
  }
}

void Func_020017a0(void) {
  s32 t0;

  t0 = Func_020030e0();
  if (t0 != 0) {
    FieldEvent_SetExitRequest(0xc);
  }
}
