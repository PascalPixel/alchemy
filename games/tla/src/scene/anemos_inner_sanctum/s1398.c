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

/// An event handler from the sanctum's event table (data.c): changes the
/// cells at the leader's place, spawns an effect of type 0x15d there and sets
/// the flag the event work's unk154 selects; once flags 0x200 to 0x206 are all
/// set, it moves object 8 in room 4 (else object 9) to mode 2 and sets flag
/// 0x207.
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
