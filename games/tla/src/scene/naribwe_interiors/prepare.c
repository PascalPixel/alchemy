#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct Actor *Actor_GetById(s32 id);
void Func_02001158(s32 id);
void Func_02001108(s32 id, s32 x, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);

s32 Func_02000f28(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Func_02001158(17);
  Func_02001158(18);
  Actor_SetSpritePriority(10, 2);
  Actor_GetById(12)->collisionFlags |= 4;
  Actor_SetSpritePriority(16, 2);
  Actor_GetById(16)->collisionFlags |= 4;

  object = Actor_GetById(18);
  object->motionFlags = 0;
  object->y = -0x40000;
  object->terrainHeight = -0x40000;
  Actor_GetById(18)->priorityFlags |= 0x20;
  Actor_SetSpritePriority(18, 1);
  Func_02001108(18, 0, 0);

  object = Actor_GetById(19);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_GetById(19)->priorityFlags |= 0x20;
  Actor_SetSpritePriority(19, 1);
  Func_02001108(19, 0, 0);

  object = Actor_GetById(20);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_GetById(20)->priorityFlags |= 0x20;
  Actor_SetSpritePriority(20, 1);
  Func_02001108(20, 0, 0);

  Actor_SetSingleChildFlags(Actor_GetById(19), 0);
  Actor_SetSingleChildFlags(Actor_GetById(20), 0);
  Actor_SetPartAttribute(Actor_GetById(20), 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
