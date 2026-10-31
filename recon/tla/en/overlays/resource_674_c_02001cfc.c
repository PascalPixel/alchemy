// Near miss, differ 2 of 65 (tla-en, 2026-10-31): the counter's 0 is stored
// after the reload copies of the layer and line pointers to r10 and r8, not
// before them; sched2 ties all three at priority 1 and reload emits each copy
// right after the add that forms its pointer, so insn order puts them first.
// The wave scanline table of map layer 2, from its sibling in overlay 68b. The
// sine and the counter live on the stack as an array (sp and sp + 4), the
// counter through a pointer, as the native reloads it after each call; a
// one-pass block stores the sine before the layer pointer is reloaded (alchemy
// permute found the same block). No single barrier or one-pass block after any
// statement changes the order. Worse: the counter set before the pointers are
// formed (differ 14), between them (14), the pointers copied in the loop's
// init (8) or through an operand asm (52); while, do-while and goto loops stay
// at 2, and four more minutes of alchemy permute (9,934 candidates) found
// nothing better.
#include "../../../../games/tla/src/scene/islets/islets.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

s32 Math_Sine(s32 angle);

void Func_02001cfc(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[2];
  s16 *lines = gData_02004f10[gData_02004f04 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(one-pass): the one-pass block kept; as plain statements, 4 of
     * 65 instructions differ in tla-en. */
    do {
      sine = Math_Sine((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
  }
  gData_02004f04 ^= 1;
}
