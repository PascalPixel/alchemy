#include "types.h"

/*
 * Set up actors 24 and 25 -- resource_3b1. A flat setter sequence with no
 * branches; the owner includes its one literal pool word.
 */

/* The pool word, referenced by address so that it is emitted. */
extern u8 Value_00000929;

/*
 * The aliases name the call words encoded in the overlay image, and the
 * declarations are old-style because the call sites vary in arity.
 */
s32 Func_0200a634();
void Func_0200bb5a(void);
void Func_02009f84();
void Func_02009f8e();
void Func_02008e84();
void Func_02009f9e();
void Func_0200bc08();
void Func_0200a9b8(void);
void Func_0200bb72();
void Func_0200bb9e(void);

void FieldScene_RunActors24And25SetupWithValue929(void)
{
    s32 handle = Func_0200a634(0, 0);

    Func_0200bb5a();
    Func_02009f84(24, 1, 0);
    Func_02009f8e(25, 0, 0);
    Func_02008e84(0);
    Func_02009f9e(19, handle, 12);
    Func_0200bc08(11, 0, 0);
    Func_0200a9b8();
    Func_0200bb72((s32)&Value_00000929);
    Func_0200bb9e();
}
