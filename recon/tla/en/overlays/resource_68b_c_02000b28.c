// Near miss, differ 2 of 65 (tla-en, 2026-10-31): scheduler order. The native
// stores the counter's 0 before it copies the layer and line pointers to r10
// and r8; here the copies go first. In sched2 all three tie at priority 1 and
// fall back to insn order, and the copies are reload outputs placed right
// after the adds that form the pointers, ahead of the store; the native order
// needs real copies after the store, which CSE and combine fold away (temporary
// pointers copied after the store: differ 8). alchemy permute (4 minutes,
// 15,000 candidates) found nothing better.
// Under -mthumb-slow-loads (2026-10-26): differ 2 of 65; left: as overlay 674's
// Func_02001cfc; no single barrier or one-pass block changes it.
// The wave scanline table of map layer 1, from its sibling in overlay 674. The
// sine and the counter live on the stack as an array (sp and sp + 4), the
// counter through a pointer, as the native reloads it after each call; a
// one-pass block stores the sine before the layer pointer is reloaded (alchemy
// permute found the same block). The layer read through its struct and the
// frame count as (u8)gFrameCount, as eastern_alhafra/wave.c does, compile the
// same as halfword casts. The counter set before the pointers are formed
// (differ 16), set between the two pointers (14) and the pointers copied
// through an operand asm after it (52) were worse.
#include "types.h"
#include "frame.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gData_02002590;
extern s16 gData_020025a0[2][160];

s32 Math_Sine(s32 angle);

void Func_02000b28(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  s16 *lines = gData_020025a0[gData_02002590 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(one-pass): the block stores the sine before the layer pointer
     * is reloaded; as plain statements, 4 of 65 instructions differ in tla-en.
     */
    do {
      sine = Math_Sine((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
      v[0] = sine;
    } while (0);
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gData_02002590 ^= 1;
}
