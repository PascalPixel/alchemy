#include "types.h"
/* resource_382 owner at 0x02000938, 68 bytes.
 * Raises a busy flag at +91 for the duration of the sequence and clears it
 * afterwards; the clearing zero is held in a high register across the calls. */
u8 *Func_02002322();
void Func_0200230a();
void Func_0200237a();
void Func_02002310();
void Func_020023c8();
void Func_0200232c();
void Func_02000938(s32 arg0)
{
    u8 *rec = Func_02002322();

    rec[91] = 1;
    Func_0200230a();
    Func_0200237a(arg0, 1);
    Func_02002310(2);
    Func_020023c8(arg0, 0);
    Func_0200232c();
    rec[91] = 0;
}
