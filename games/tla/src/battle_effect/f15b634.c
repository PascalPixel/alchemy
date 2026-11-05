#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

// Mirrors the first 64 background colours, keeping the backdrop colour.
void Func_0815b634(void) {
  u16 saved[64];
  u16 flipped[64];
  s32 i;

  /* FAKEMATCH(wrapper): both copies through an inline forwarder, so each builds
   * its array's address anew; as direct calls, 19 of 39 instructions differ in
   * tla-en. */
  CopyWords_(saved, (void *)BG_PLTT, 128);
  for (i = 1; i != 64; i++)
    flipped[i] = saved[64 - i];
  flipped[0] = saved[0];
  CopyWords_((void *)BG_PLTT, flipped, 128);
  REG_BLDALPHA = 0x810;
}
