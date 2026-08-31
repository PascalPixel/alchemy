#include "types.h"

#define FieldScene_RunMiddleAuxiliarySequence Func_02000464

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 17 calls, 0 loop(s), and 6 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020016d4();
s32 Func_020016da();
void *Func_0200170c();
void Func_02001714();
void Func_0200171c();
s32 Func_0200173a();
void Func_02001762();
void Func_0200176c();
void Func_0200177c();
void Func_02001784();
void Func_0200178a();
void Func_0200178e();
void Func_020017aa();
void Func_020017ba();
void Func_020017c4();
void Func_02001840();

void Func_02000464(void)
{
    void *p1;

    p1 = Func_0200170c();
    Func_02001714();
    if (Func_020016da(527) == 0) {
        Func_02001714();
        Func_020017c4();
        Func_0200177c();
        if (Func_0200173a() != 0) {
            Func_0200176c();
        }
        Func_0200178a();
        Func_02001840(244);
        Func_020016d4(33588225, 3200);
        *(u8 *)((u8 *)(p1) + 85) = 0;
        Func_0200171c();
        Func_020017ba();
        *(s32 *)(p1 + 40) = 0;
        *(u8 *)((u8 *)(p1) + 85) = 4;
        Func_02001762(527);
        Func_02001784(536);
        Func_0200178e(528, 180);
        Func_020017aa();
    }
}
