#include "types.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void Func_02001bcc();

s32 Func_02000a18(void) {
  if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171))) {
    u16 *src = (u16 *)0x5000116;

    u32 c;

    /* FAKEMATCH(one-pass): the four copies in a once-only block, whose end holds
     * the order around it; as plain statements, 4 of 57 instructions differ in
     * tla-en. */
    do {
      c = *src;
      *(u16 *)0x50000f6 = c;
      src++;
      c = *src;
      *(u16 *)0x50000f8 = c;
      src++;
      c = *src;
      *(u16 *)0x50000fa = c;
      {
        u32 last = src[1];

        *(u16 *)SPLIT_CONSTANT(0x5000000, 252) = last;
      }
    } while (0);
    Call6(Func_02001bcc, 37, 18, 2, 2, 37, 33);
    Call6(Func_02001bcc, 37, 18, 2, 3, 39, 31);
    Call6(Func_02001bcc, 50, 80, 7, 5, 35, 95);
  }
  return 0;
}
