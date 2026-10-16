// Draft, differ 26 of 57: the listing copies four palette colours with the
// source walking from one pool address (adds r3, #2) and each destination an
// explicit constant that reload turns into adds r2, #2, the last one rebuilt;
// it keeps the colour in r1 and stores the second stack argument from r2. Tried
// a struct copy (ldmia/stmia), dst/src pointers, explicit addresses, direct
// calls.
#include "types.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void Func_02001bcc();

s32 Func_02000a18(void) {
  if (GameFlag_Test(0x8ab)) {
    u16 *src = (u16 *)0x5000116;

    *(u16 *)0x50000f6 = src[0];
    *(u16 *)0x50000f8 = src[1];
    *(u16 *)0x50000fa = src[2];
    *(u16 *)0x50000fc = src[3];
    Call6(Func_02001bcc, 37, 18, 2, 2, 37, 33);
    Call6(Func_02001bcc, 37, 18, 2, 3, 39, 31);
    Call6(Func_02001bcc, 50, 80, 7, 5, 35, 95);
  }
  return 0;
}
