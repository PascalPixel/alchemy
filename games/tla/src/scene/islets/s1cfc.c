#include "islets.h"
#include "event_inline.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];


void Func_02001cfc(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[2];
  s16 *lines = gData_02004f10[gData_02004f04 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(one-pass): the one-pass block kept; as plain statements, 2 of
     * 65 instructions differ in tla-en. */
    do {
      sine = Math_Sin((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
  }
  gData_02004f04 ^= 1;
}
