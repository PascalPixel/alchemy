#include "types.h"
#include "object_runtime.h"
#include "mem.h"

s32 EventRuntime_GetControlledOwner(void);
void WaitFrames(s32);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const u8 *script);
void BattleFx_EmitRandomParticleFromEmitter(void);
void Func_080cf350(struct ObjectRuntime *object);
extern const u8 gData_080effd8[];

void Func_080cf3b4(struct ObjectRuntime *object, s32 flags) {
  struct ObjectRuntime *owner;

  if (object != 0) {
    owner = ObjectTable_Get(EventRuntime_GetControlledOwner());
    if (flags & 1) {
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_InitializeFar(object, gData_080effd8);
      object->velocityY = 0x20000;
      object->gravity = 0x4000;
      object->update = BattleFx_EmitRandomParticleFromEmitter;
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

void Func_080cf424(s32 item, s32 flags) {
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
      object->update = BattleFx_EmitRandomParticleFromEmitter;
    if (flags & 2)
      Func_080cf350(object);

    WaitFrames(80);
    Object_SetMode(owner, 1);
    Object_Destroy(object);
  }
}
