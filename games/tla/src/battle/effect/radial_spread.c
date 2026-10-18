#include "types.h"
#include "effect_slot.h"
#include "object_runtime.h"
#include "party_state.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

extern u32 gFrameTick;

void *Object_GetById(s32 id);
u32 Random16(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);
void Camera_WorldToScreen(struct EffectVector *position);
void Audio_PlayCue(s32 cue);
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot);

// Places a spark at random around the current owner and sends it 120 pixels
// along angle 0xc000. Each time EffectSlot_HasPendingTarget returns 0, state 1
// starts it over and state 2 clears its slot.
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect) {
  struct EffectVector position;
  struct ObjectRuntime *source;
  /* FAKEMATCH: the state is pinned to r8, the register the native gives
     it; free, global allocation trades it with the state's address in
     r10 once the two barriers below lengthen their lives (differ 14). */
  register s32 state asm("r8");
  u32 random;
  s32 x;
  s32 magnitude;
  s32 angle;

  source = Object_GetById(gPartyState.currentOwner);
  state = effect->state;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    x = position.x;
    magnitude = 240;
    /* FAKEMATCH: ⚓️ starts the distance (240 << 15) after it loads x
       and the angle (192 << 8) after it stores it, and shifts both after
       it loads z; as plain constants both halves follow the z load
       (differ 6). */
    asm volatile("" : "+l"(magnitude));
    effect->originX = x;
    angle = 192;
    asm volatile("" : "+l"(angle));
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(magnitude << 15, angle << 8, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x10000;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    if ((gFrameTick & 1) != 0)
      Audio_PlayCue(0x90);
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(effect) == 0)
      BattleFx_ClearOwnedSlot(effect);
  }
}
