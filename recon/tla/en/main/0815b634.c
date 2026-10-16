// Near miss of Func_0815b634 (listing 39 instructions, draft 38, differ 21):
// CSE keeps the address of saved (sp + 128) from the first copy's argument
// in r5 across the call and reads saved[0] through it; the native
// rematerialises the address before the flip loop (add r4, sp, #128) and
// saves no register. Tried: the arrays as one u16[2][64], as structs, in
// the other declaration order (differ 33), &saved[0], pointer loops, the
// final read as saved[64 - i], a one-pass block after or around the first
// copy (differ 21 to 23).
#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"

// Mirrors the first 64 background colours, keeping the backdrop colour.
void Func_0815b634(void) {
  u16 saved[64];
  u16 flipped[64];
  s32 i;

  Iwram_CopyWords(saved, (void *)BG_PLTT, 128);
  for (i = 1; i != 64; i++)
    flipped[i] = saved[64 - i];
  flipped[0] = saved[0];
  Iwram_CopyWords((void *)BG_PLTT, flipped, 128);
  REG_BLDALPHA = 0x810;
}
