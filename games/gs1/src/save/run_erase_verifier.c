/* Flash erase verification for the save driver. */
#include "flash.h"

s32 Func_080072e8();

/*
 * Func_080072e8 is the `bx r1` slot of the __call_via_rN veneer bank at
 * 0x080072e4, so this call is an indirect call through r1 rather than a call
 * to a function at that address.  r1 is never written here: the callee is
 * the pointer the caller left in it, and r0 reaches that callee untouched.
 * The empty parameter list is unverified -- the real arity is not known.
 */
s32 RunFlashEraseVerifier(void)
{
    if (Func_080072e8() == 0) {
        return 0;
    }
    return 0x8004;
}
