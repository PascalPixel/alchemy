// Near miss, differ 12 of 113 (was 20): the effect work comes from its heap
// slot and the angle is a halfword. What is left is scheduling: the work load
// against the argument copy, the three position loads against the two
// constants, and the state pointer copy.
#include "fixed_math.h"
#include "types.h"

struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct EffectTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EffectRuntime {
  u16 angle;
  u8 unk02[14];
  struct EffectTarget *target;
};

struct RadialBurstEffect {
  u8 unk00[12];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  u8 unk1c[4];
  s32 velocityX;
  s32 velocityZ;
  u8 unk28[10];
  u16 scale;
  u8 unk34[12];
  s8 state;
  u8 unk41;
  u8 enabled;
};

#include "ram_buffer.h"

s32 Random16(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectPosition *output);
void Camera_WorldToScreen(struct EffectPosition *value);
s32 BattleFx_HasReachedTarget(struct RadialBurstEffect *effect);
void BattleFx_ClearOwnedSlot(struct RadialBurstEffect *effect);

void BattleFx_UpdateRadialBurst(struct RadialBurstEffect *effect) {
  struct EffectRuntime *runtime = Ram_HeapSlots->effectWork;
  struct EffectPosition value;
  s32 state;

again:
  state = effect->state;
  if (state == 0) {
    value.x = effect->sourceX;
    value.z = effect->sourceZ;
    Vector_AddPolarOffset(0x190000, (u16)Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->velocityZ = 0x30000;
    effect->velocityX = 0x30000;
    effect->enabled = 0;
    effect->state++;
    return;
  }

  if (state == 1) {
    if (BattleFx_HasReachedTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    struct EffectTarget *target = runtime->target;

    value.x = target->x;
    value.y = target->y + 0x100000;
    value.z = target->z;
    Vector_AddPolarOffset(0x80000, runtime->angle, &value);
    Camera_WorldToScreen(&value);
    Vector_AddPolarOffset(0x40000, Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->scale = 0x800;
    effect->enabled = 1;
    effect->state++;
    return;
  }

  if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
    BattleFx_ClearOwnedSlot(effect);
}
