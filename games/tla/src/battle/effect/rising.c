#include "types.h"
#include "actor.h"
#include "mem.h"

s32 Party_GetLeaderActor(void);
void WaitFrames(s32);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
void Actor_StartScriptFar(struct Actor *object,
                                  const u8 *script);
void Marker_MoveLeaderAwayFromEmitter(void);
void Actor_SetSpritePriorityDirectFar(struct Actor *object,
                                         s32 priority);
extern const u8 gData_080effd8[];
extern const u8 gData_080f0074[];

/// Places the object 36 pixels above the controlled owner, gives it its
/// sprite priority and, three frames on, the descent script, and turns the
/// owner to face a quarter turn.
void Func_080cf350(struct Actor *object) {
  struct Actor *owner;

  if (object == 0)
    return;

  {
    /* FAKEMATCH(pin): the owner kept in r0, where the call returns it, until
     * after the motion byte's store; as an ordinary local, 2 of 40
     * instructions differ in tla-en. */
    register struct Actor *found asm("r0") =
        ObjectTable_Get(Party_GetLeaderActor());
    object->acceleration = 0x10000;
    object->speedLimit = 0x20000;
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): the owner's copy out of r0 held after the stores,
     * as the native copies it; without the barrier, 2 of 40 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+r"(found));
    owner = found;
  }
  Actor_SetTargetPosition(object, owner->x, owner->y + 0x240000, owner->z);
  Actor_SetSpritePriorityDirectFar(object, 1);
  WaitFrames(3);
  Actor_SetMode(owner, 28);
  Actor_StartScriptFar(object, gData_080f0074);
  owner->angle = 0x4000;
}

void EffectRuntime_RunObjectRiseAndDescent(struct Actor *object, s32 flags) {
  struct Actor *owner;

  if (object != 0) {
    owner = ObjectTable_Get(Party_GetLeaderActor());
    if (flags & 1) {
      Actor_SetSingleChildFlagsFar(object, 0);
      Actor_StartScriptFar(object, gData_080effd8);
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
    Actor_SetMode(owner, 1);
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
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Actor_Destroy(struct Actor *object);

void BattleEffect_StartEffectObject22(s32 item, s32 flags) {
  struct Actor *owner =
      ObjectTable_Get(Party_GetLeaderActor());
  u8 *buffer = HeapSlot_Alloc(68, 0x608);
  struct Actor *object =
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
    HeapSlot_Release(68);

    if (flags & 1)
      object->update = Marker_MoveLeaderAwayFromEmitter;
    if (flags & 2)
      Func_080cf350(object);

    WaitFrames(80);
    Actor_SetMode(owner, 1);
    Actor_Destroy(object);
  }
}
