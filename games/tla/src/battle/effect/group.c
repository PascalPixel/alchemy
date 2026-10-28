#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "actor.h"
#include "ram_buffer.h"

#define Actor_SetAnimationSpeedFar                                 \
  ((void (*)(struct Actor *,                                           \
             s32))Actor_SetAnimationSpeedFar)

struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 value);

struct SpawnScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct SpawnScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *object;
  u8 *part;
  s32 mask;

  object = Func_080200c0(kind, x, y, z);
  if (object != NULL) {
    if (object->animationKind == 0) {
      Actor_Destroy((struct DispatchObject *)object);
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
    Actor_SetSingleChildFlagsFar(object, 0);
    Actor_SetMode(object, 1);
  }
  return object;
}

void Actor_SetOthersPaused(struct Actor *excludedObject,
                                    s32 groupMode, s32 action) {
  s16 *activeObjectId;
  struct Actor *object;
  s32 objectId;

  objectId = 0;
  activeObjectId = (s16 *)gPlayerObjectId;
  do {
    object = ObjectTable_Get(objectId);
    if (objectId != *activeObjectId && object != NULL &&
        object != excludedObject) {
      object->movementState = groupMode;
      Actor_SetAnimationSpeedFar(object, action);
    }
    objectId++;
  } while (objectId <= 0x50);
}
