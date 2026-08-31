#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000f70

/* Audited retained field-scene body at 0x02000f70.
 * The complete production body preserves 50 decoded calls, 0 loop(s), and 10 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020011cc();
void Func_02001914();
void Func_0200191a();
void Func_020023cc();
void Func_0200265e();
void Func_020026be();
void *Func_020028b4();
void *Func_02003fe6();
void *Func_02003ff8();
s32 Func_02003ffe();
void Func_02004044();
s32 Func_02004054();
void Func_02004068();
void Func_02004078();
void Func_020040b4();
s32 Func_020040ba();
void Func_020040c0();
void *Func_020040d0();
void Func_020040e0();
void *Func_020040e6();
void Func_020040f0();
void *Func_020040f4();
s32 Func_020040f6();
void Func_020040fc();
void Func_0200411c();
void Func_0200412c();
void *Func_02004130();
s32 Func_02004132();
void Func_02004138();
void Func_02004158();
void *Func_0200416c();
s32 Func_0200416e();
void Func_02004174();
void Func_02004194();
void Func_020041a4();
void *Func_020041a8();
s32 Func_020041ba();
s32 Func_020041d2();
void Func_0200428e();
void Func_02004290();
void *Func_020042a4();
void Func_020042a8();
void Func_020042b0();
void Func_020042da();
void Func_02004300();
void *Func_02004318();
void Func_02004330();

void Func_02000f70(void)
{
    s32 base;
    void *p1;
    void *p11;
    void *p15;
    void *p20;
    void *p25;
    void *p3;
    void *p30;
    void *p4;
    void *p43;
    void *p5;
    void *p50;

    p1 = Func_020028b4();
    if (Func_02003ffe(3831) == 0) {
        p3 = Func_02003fe6(0, 3, 1, 1, 13, 40);
        p4 = Func_02003ff8(0, 2, 1, 1, 15, 40);
        p5 = Func_020040e6(101, 14155776, 42467328);
    }
    if (Func_02004054(2257) == 0) {
    } else {
        Func_02004068(2257);
        Func_02004044(0, 1, 1, 1, 13, 30);
        Func_02004132(100, 14155776, 31981568);
        Func_020026be();
        p11 = Func_020040d0(8);
        *(s32 *)(p11 + 56) = 8454144;
        Func_02001914(9);
        Func_0200191a(10);
        if (Func_020040ba(576) != 0) {
            p15 = Func_020040f4(11);
            *(u8 *)((u8 *)(p15) + 89) = 0;
            Func_02004078();
            Func_020040c0();
            Func_020040e0(0, 19922944, 24117248, 253);
        }
        if (Func_020040f6(577) != 0) {
            p20 = Func_02004130(12);
            *(u8 *)((u8 *)(p20) + 89) = 0;
            Func_020040b4();
            Func_020040fc();
            Func_0200411c(0, 5242880, 24117248, 253);
        }
        if (Func_02004132(578) != 0) {
            p25 = Func_0200416c(13);
            *(u8 *)((u8 *)(p25) + 89) = 0;
            Func_020040f0();
            Func_02004138();
            Func_02004158(0, 6291456, 22020096, 253);
        }
        if (Func_0200416e(579) != 0) {
            p30 = Func_020041a8(14);
            *(u8 *)((u8 *)(p30) + 89) = 0;
            Func_0200412c();
            Func_02004174();
            Func_02004194(0, 9437184, 20971520, 253);
            Func_020041a4(0, 49283072, 20971520, 253);
        }
        if (Func_020041ba(4055) == 0) {
            Func_02004044(8);
            base = 126;
            if (Func_020041d2(3828) == 0) {
                Func_020041ba(0, 0, 1, 1, 37, 10);
                Func_020042a8(100, 39321600, 11010048);
            }
            Func_020011cc();
            Func_020023cc();
            Func_0200265e();
        }
    }
    p43 = Func_020042a4(8);
    base = 126;
    Func_0200428e();
    Func_02004300(8, 42598400, 11010048);
    *(s32 *)(p43 + 12) = 0;
    Func_020042da(8);
    Func_02004290();
    Func_02004330(8, 3);
    *(u8 *)((u8 *)(p43) + 85) = 0;
    *(u8 *)((u8 *)(p43) + 35) |= 0x2;
    Func_020042b0(42, 10, 1, 1, 40, 10);
    p50 = Func_02004318(8);
    *(u8 *)((u8 *)(p50) + 85) = 0;
}
