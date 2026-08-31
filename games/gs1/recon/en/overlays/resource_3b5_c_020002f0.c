#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_020002f0

/* Audited retained supplemental scene body.
 * The complete production span preserves 24 calls, 0 loop(s), and 12 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200106a();
s32 Func_02001070();
void *Func_0200107a();
s32 Func_0200108c();
void Func_0200109a();
s32 Func_020010a0();
s32 Func_020010ae();
void Func_0200110e();
void Func_0200113c();
void Func_0200114a();
s32 Func_02001150();
void *Func_0200115a();
s32 Func_0200116c();
void Func_0200116e();
void Func_0200117a();
s32 Func_02001180();
s32 Func_0200118e();
void Func_020011a0();
void Func_020011ee();
void Func_0200121c();
void Func_0200124c();
void Func_0200124e();
void Func_02001280();

void Func_020002f0(void)
{
    void *p1;
    void *p13;

    p1 = Func_0200107a(16);
    Func_0200106a();
    *(u16 *)((u8 *)(p1) + 100) |= 0x2;
    if (Func_02001070(2384) != 0) {
    }
    if (Func_0200108c(2402) != 0) {
    } else {
        if (Func_020010a0(2384) != 0) {
        } else {
            if (Func_020010ae(2402) != 0) {
            } else {
            }
        }
    }
    Func_0200116c(8106);
    Func_0200113c(16, 0);
    Func_0200116e(16, 0, 2);
    Func_020011a0(16, 0, 10);
    *(u16 *)(p1 + 6) = 0;
    Func_0200109a(1);
    *(u16 *)((u8 *)(p1) + 100) &= 0x1;
    Func_0200110e();
    p13 = Func_0200115a(17);
    Func_0200114a();
    *(u16 *)((u8 *)(p13) + 100) |= 0x2;
    if (Func_02001150(2384) != 0) {
    }
    if (Func_0200116c(2402) != 0) {
    } else {
        if (Func_02001180(2384) != 0) {
        } else {
            if (Func_0200118e(2402) != 0) {
            } else {
            }
        }
    }
    Func_0200124c();
    Func_0200121c(17, 0);
    Func_0200124e(17, 0, 2);
    Func_02001280(17, 0, 10);
    *(u16 *)(p13 + 6) = 0;
    Func_0200117a(1);
    *(u16 *)((u8 *)(p13) + 100) &= 0x1;
    Func_020011ee();
}
