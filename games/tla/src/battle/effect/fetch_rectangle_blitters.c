#include "types.h"
#include "ram_buffer.h"

void BattleEffect_LoadBlitter(s32 slot, s32 flags);

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  /* FAKEMATCH: never read: the native frame keeps 4 bytes below the saved
   * registers, which this 3-byte local explains (without it the frame
   * shrinks). */
  u8 unused[3];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH: the empty asm holds the heap base's first half and the
     * first call's flags, so both are started before the base is shifted
     * and the slot offset set, as the native does (differ 4 without it). */
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

    /* FAKEMATCH: as in the unmirrored branch. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}
