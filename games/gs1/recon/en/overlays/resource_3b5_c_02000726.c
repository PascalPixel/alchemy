#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000726

/* Audited retained supplemental scene body.
 * The complete production span preserves 42 calls, 0 loop(s), and 5 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020009e8();
void Func_0200145c();
void *Func_020014ce();
s32 Func_020014d8();
s32 Func_020014f0();
void *Func_020014f6();
void Func_020014f8();
s32 Func_02001500();
void *Func_02001506();
s32 Func_02001518();
void Func_02001520();
void Func_0200155a();
s32 Func_02001586();
void Func_02001588();
void Func_0200159a();
void Func_020015a8();
void Func_020015b6();
void Func_020015c4();
void Func_020015d2();
void Func_020015e0();
void Func_020015ee();
void Func_020015f4();
void Func_02001608();
void Func_02001618();
void Func_02001624();
void Func_02001642();
s32 Func_0200164e();
void Func_0200167c();
void Func_0200167e();
void Func_02001688();
void Func_020016a4();
void Func_020016ae();
void Func_020016bc();
void Func_020016be();
void Func_020016ce();
void Func_020016da();
void Func_020016fa();
void Func_02001740();
void Func_02001748();

void Func_02000726(void)
{
    void *p3;
    void *p6;
    void *p7;

    Func_020014f8(16, 23068672, 23068672, 50339964);
    Func_020014d8(16, 33590976);
    p3 = Func_020014ce(16);
    *(u16 *)((u8 *)(p3) + 100) = 1;
    *(s32 *)(p3 + 108) = 33587569;
    Func_02001520(17, 24117248, 20971520, 1);
    Func_02001500(17, 33591184);
    p6 = Func_020014f6(17);
    *(u16 *)((u8 *)(p6) + 100) = 0;
    *(s32 *)(p6 + 108) = 33587569;
    p7 = Func_02001506(14);
    *(s32 *)(p7 + 108) = 33588969;
    if (Func_020014d8(2241) != 0) {
        Func_0200155a(28, 20709376, 21495808);
    }
    if (Func_020014f0(513) != 0) {
        Func_0200145c();
    }
    if (Func_02001500(512) != 0) {
        Func_020009e8();
        Func_02001588(8, 4);
    }
    if (Func_02001518(2384) != 0) {
        Func_0200159a(20, 34078720, 36700160);
        Func_020015a8(21, 34078720, 36700160);
        Func_020015b6(22, 34078720, 36700160);
        Func_020015c4(24, 34078720, 36700160);
        Func_020015d2(25, 34078720, 36700160);
        Func_020015e0(26, 34078720, 36700160);
        Func_020015ee(27, 34078720, 36700160);
    } else {
        if (Func_02001586(2402) != 0) {
            Func_02001608(27, 18350080, 5242880);
            Func_0200167c(27, 8192, 0);
            Func_02001624(27, 1);
        }
    }
    Func_020015f4(0);
    Func_02001642(0, 304, 312);
    Func_020016ce(0, 49152, 0);
    Func_020016da(28, 16384, 0);
    Func_02001618(20);
    Func_020016be(3645);
    Func_020016ce(28, 0);
    if (Func_0200164e(0, 0) == 0) {
        Func_020016fa(28, 0);
        Func_02001688(28, 65536, 32768);
        Func_020016ae(28, 320, 304);
        Func_020016bc(28, 316, 328);
        Func_02001748(28, 40960, 0);
        Func_0200167e();
    } else {
        Func_02001740(28, 0);
    }
    Func_020016a4();
}
