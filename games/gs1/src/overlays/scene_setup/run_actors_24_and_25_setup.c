#include "types.h"

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee name refers
 * to its own call word rather than to a shared runtime address.
 */

extern u8 Value_00000928;

void Func_0200a6e4(void);
void Func_02008b0e();
void Func_02008b18();
void Func_02008b22();
void Func_02009532(void);
void Func_0200a6ec();
void Func_0200a718(void);

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void FieldScene_RunActors24And25Setup(void)
{
    Func_0200a6e4();
    Func_02008b0e(24, 1, 0);
    Func_02008b18(25, 0, 0);
    Func_02008b22(19, 11, 12);
    Func_02009532();
    Func_0200a6ec((s32)&Value_00000928);
    Func_0200a718();
}
