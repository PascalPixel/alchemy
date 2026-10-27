#include "types.h"
#include "object_runtime.h"
#include "mem.h"

s32 EventRuntime_GetControlledOwner(void);
void WaitFrames(s32);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const u8 *script);
void Marker_MoveLeaderAwayFromEmitter(void);
void ObjectDispatch_SetSpritePriorityFar(struct ObjectRuntime *object,
                                         s32 priority);
extern const u8 gData_080effd8[];
extern const u8 gData_080f0074[];

/// Places the object 36 pixels above the controlled owner, gives it its
/// sprite priority and, three frames on, the descent script, and turns the
/// owner to face a quarter turn.
void Func_080cf350(struct ObjectRuntime *object) {
  struct ObjectRuntime *owner;

  if (object == 0)
    return;

  {
    /* FAKEMATCH(pin): the owner kept in r0, where the call returns it, until
     * after the motion byte's store; as an ordinary local, 2 of 40
     * instructions differ in tla-en. */
    register struct ObjectRuntime *found asm("r0") =
        ObjectTable_Get(EventRuntime_GetControlledOwner());
    object->acceleration = 0x10000;
    object->speedLimit = 0x20000;
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): the owner's copy out of r0 held after the stores,
     * as the native copies it; without the barrier, 2 of 40 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+r"(found));
    owner = found;
  }
  Object_SetPosition(object, owner->x, owner->y + 0x240000, owner->z);
  ObjectDispatch_SetSpritePriorityFar(object, 1);
  WaitFrames(3);
  Object_SetMode(owner, 28);
  ObjectDispatch_InitializeFar(object, gData_080f0074);
  owner->angle = 0x4000;
}

void EffectRuntime_RunObjectRiseAndDescent(struct ObjectRuntime *object, s32 flags) {
  struct ObjectRuntime *owner;

  if (object != 0) {
    owner = ObjectTable_Get(EventRuntime_GetControlledOwner());
    if (flags & 1) {
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_InitializeFar(object, gData_080effd8);
      object->velocityY = 0x20000;
      object->gravity = 0x4000;
      object->update = Marker_MoveLeaderAwayFromEmitter;
    }
    if (flags == 3)
      WaitFrames(60);
    if (flags & 2)
      Func_080cf350(object);
    if (flags == 3)
      WaitFrames(80);
    Object_SetMode(owner, 1);
  }
}

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[3];
  u8 flags09;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Object_Destroy(struct ObjectRuntime *object);

void BattleEffect_StartEffectObject22(s32 item, s32 flags) {
  struct ObjectRuntime *owner =
      ObjectTable_Get(EventRuntime_GetControlledOwner());
  u8 *buffer = Runtime_AllocateHeapBlock(68, 0x608);
  struct ObjectRuntime *object =
      Func_080200c0(489, owner->x, owner->y + 0x240000, owner->z);

  if (object != 0) {
    struct EffectVisual *visual = object->animation;

    s32 mask;
    s32 zero = 0;

    visual->value1a = zero;
    visual->value1b = zero;
    visual->flags05 &= zero - 33;

    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;

    Func_08038248(item);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    Runtime_ReleaseHeapBlock(68);

    if (flags & 1)
      object->update = Marker_MoveLeaderAwayFromEmitter;
    if (flags & 2)
      Func_080cf350(object);

    WaitFrames(80);
    Object_SetMode(owner, 1);
    Object_Destroy(object);
  }
}
