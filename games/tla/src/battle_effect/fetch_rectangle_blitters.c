#include "types.h"
#include "ram_buffer.h"

s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH(barrier): an empty asm on heap and flags, so both are set
     * before the heap base is shifted and the slot offset built; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 3);
    out[1] = slots[47];
  } else {
    u32 heap = 0xc0;
    s32 flags = 23;
    void **slots;

    /* FAKEMATCH(barrier): the same asm for the mirrored blitters; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}
