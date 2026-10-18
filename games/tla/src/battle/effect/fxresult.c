#include "types.h"
#include "ram_buffer.h"

struct EffectSceneState {
  u8 unk000[0xd31];
  u8 valueD31;
  u8 valueD32;
  u8 valueD33;
};

struct EffectSceneWork {
  u8 unk00[0x20];
  u8 failed;
};

s32 Func_080d9a74(s32 value, s32 *first, s32 *second);
void Func_080e09c0(void);

// Effect 17 on a target: asks Func_080d9a74 for two results; on failure
// marks the effect failed, else stores the target and both in the event work.
// Then runs Func_080e09c0.
void Func_080e0978(s32 value) {
  struct EffectSceneState *state = Ram_HeapSlots->eventWork;
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;
  s32 second;
  s32 first;

  if (Func_080d9a74(value, &first, &second)) {
    work->failed = 1;
  } else {
    state->valueD32 = value;
    /* FAKEMATCH: the ARM9 scheduling would load the first result before storing
     * the value. */
    __asm__("");
    state->valueD33 = first;
    /* FAKEMATCH: and would step the offset back before loading the second
     * result. */
    __asm__("");
    state->valueD31 = second;
  }
  Func_080e09c0();
}
