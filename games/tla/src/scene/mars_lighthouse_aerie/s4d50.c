#include "version.h"

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

#include "mars_lighthouse_aerie.h"

/// A halfword held in its own local: its load, the pool's mask and its store
/// keep their halfword forms.
struct HeldHalfword {
  u16 value;
};

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsLighthouseAerieParty *party =
      (struct MarsLighthouseAerieParty *)&gMainRoutineWork;
  s32 held = party->held;
  /* FAKEMATCH(temporary): the word held in a halfword struct; as a plain store
   * of -1, 42 of 32 instructions differ in tla-en. */
  struct HeldHalfword raw = *(struct HeldHalfword *)&party->held;
  struct HeldHalfword ones;

  if (held == id) {
    s32 zero = 0;

    work[0x138a] = zero;
    ones.value = 0xffff;
    party->held = raw.value | ones.value;
    Actor_Get(held)->update = (void *)zero;
    Actor_SetSpriteColor(Actor_Get(held), 0);
  }
}

#endif
