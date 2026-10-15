#include "fixed_math.h"
#include "types.h"
#include "scene.h"
#include "system.h"
void EffectSlot_SetPositionFar(struct Effect_080b2f4c *, s32, s32);
s32 BattleFx_HasReachedTargetFar(struct Effect_080b2f4c *);
void BattleFx_ClearOwnedSlotFar(struct Effect_080b2f4c *);

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

struct Effect_080b2f4c {
  u8 filler00[0xC];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  s32 filler1C;
  s32 velocity;
  s32 acceleration;
  u8 filler28[0x18];
  s8 state;
  u8 filler41;
  s8 flag;
};

void Vector_AddPolarOffset(s32, s32, struct Position *);

void BattleFx_UpdateRadialMotion(struct Effect_080b2f4c *effect) {
  struct Position position;
  s8 *statePointer;
  s32 state;
  s32 result;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == 0) {
    position.x = effect->sourceX;
    position.z = effect->sourceZ;
    Vector_AddPolarOffset(0x280000, Random16(), &position);
    EffectSlot_SetPositionFar(effect, position.x, position.z);
    position.x = effect->sourceX;
    position.z = effect->sourceZ;
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->x = position.x;
    effect->z = position.z;
    effect->velocity = 0x20000;
    effect->acceleration = 0x6666;
    effect->flag = state;
    *statePointer = (u8)*statePointer + 1;
  } else if (state == 1) {
    result = BattleFx_HasReachedTargetFar(effect);
    if (result == 0)
      *statePointer = result;
  } else if (state == 2) {
    if (BattleFx_HasReachedTargetFar(effect) == 0)
      BattleFx_ClearOwnedSlotFar(effect);
  }
}
