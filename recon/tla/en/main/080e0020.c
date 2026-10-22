// Near miss, differ 6 of 117 (2026-10-15), ported from ☀️'s
// BattleEffect_UpdateRadialCamera. The listing loads the effect work before it
// copies the slot argument (ldr r3, [r3]; adds r7, r0, #0) and keeps two later
// loads one slot earlier; no single or paired barrier moved them.
#include "types.h"
#include "system.h"
#include "effect_slot.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *pos);
void Camera_WorldToScreen(struct EffectVector *pos);
void BattleEffect_ClearOwnedSlot(struct EffectSlot *slot);

void Func_080e0020(struct EffectSlot *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = Ram_HeapSlots->effectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->limitTurnStep = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (EffectSlot_HasPendingTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && EffectSlot_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}
