#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

void Func_02001158(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

s32 Func_02000f28(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Func_02001158(17);
  Func_02001158(18);
  FieldActor_SetSpritePriority(10, 2);
  Actor_Get(12)->collisionFlags |= 4;
  FieldActor_SetSpritePriority(16, 2);
  Actor_Get(16)->collisionFlags |= 4;

  object = Actor_Get(18);
  object->motionFlags = 0;
  object->y = -0x40000;
  object->terrainHeight = -0x40000;
  Actor_Get(18)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(18, 1);
  Actor_SetPosition(18, 0, 0);

  object = Actor_Get(19);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_Get(19)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(19, 1);
  Actor_SetPosition(19, 0, 0);

  object = Actor_Get(20);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_Get(20)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(20, 1);
  Actor_SetPosition(20, 0, 0);

  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  Actor_SetSingleChildFlags(Actor_Get(20), 0);
  Actor_SetSpriteColor(Actor_Get(20), 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
