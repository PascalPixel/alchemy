#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unk00[0x10];
  struct LaunchObject *mainObject;
};

extern u32 gFrameCount;

void Camera_WorldToScreen(struct LaunchVector *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, void *position);
u32 Random16(void);
void Sound_PlayFar(s32 cue);
void BattleEffect_ClearOwnedSlot(struct EffectSlot *slot);

void Func_080e0cec(struct EffectSlot *slot) {
  struct LaunchVector pos;
  struct LaunchObject *src;
  s32 state;
  u32 r;
  struct LaunchVector *arg;
  s32 lift;

  src = ((struct LaunchScene *)Ram_HeapSlots->effectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x;
    arg = &pos;
    pos.y = src->y;
    pos.z = src->z;
    Camera_WorldToScreen(arg);

    slot->x = slot->originX = pos.x;
    slot->z = slot->originZ = pos.z + (8 << 16);

    r = Random16();
    Vector_AddPolarOffset(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(246);
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(slot) == 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
}
