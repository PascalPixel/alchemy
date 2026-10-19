#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "object_runtime.h"
#include "ram_buffer.h"

#define ObjectDispatch_SetAnimationSpeedFar                                 \
  ((void (*)(struct ObjectRuntime *,                                           \
             s32))ObjectDispatch_SetAnimationSpeedFar)

struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 value);

struct SpawnScene {
  u8 unk00[0x10];
  struct ObjectRuntime *mainObject;
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct SpawnScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *object;
  u8 *part;
  s32 mask;

  object = Func_080200c0(kind, x, y, z);
  if (object != NULL) {
    if (object->animationKind == 0) {
      Object_Destroy((struct DispatchObject *)object);
      return NULL;
    }
    object->terrainHeight = scene->mainObject->terrainHeight;
    object->motionFlags = 4;
    object->priorityFlags = 4;
    mask = ~0xc;
    /* FAKEMATCH: ⚓️ builds the priority mask before loading the animation */
    asm volatile("" : "+l"(mask) : : "memory");
    part = object->animation;
    part[9] &= mask;
    ObjectDispatch_SetSingleChildFlagsFar(object, 0);
    Object_SetMode(object, 1);
  }
  return object;
}

void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excludedObject,
                                    s32 groupMode, s32 action) {
  s16 *activeObjectId;
  struct ObjectRuntime *object;
  s32 objectId;

  objectId = 0;
  activeObjectId = (s16 *)gPlayerObjectId;
  do {
    object = ObjectTable_Get(objectId);
    if (objectId != *activeObjectId && object != NULL &&
        object != excludedObject) {
      object->movementState = groupMode;
      ObjectDispatch_SetAnimationSpeedFar(object, action);
    }
    objectId++;
  } while (objectId <= 0x50);
}
