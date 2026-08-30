#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_ConfigureProgressDependentActors:
 * 46 calls, progress-gated actor setup, and three actor-state writes across
 * the complete callable owner. */

#define FieldScene_ConfigureProgressDependentActors Func_0200150c

void Func_02001ffa();
s32 Func_02002f98();
void *Func_02002fa2();
s32 Func_02002fa8();
void Func_02002fb2();
void Func_02002fda();
void *Func_0200302a();
s32 Func_02003032();
void Func_02003044();
void Func_0200304e();
void Func_02003050();
void *Func_02003058();
void Func_02003062();
void Func_0200306c();
void Func_02003074();
void Func_02003078();
void Func_02003082();
void Func_02003088();
void Func_0200308a();
void Func_0200309e();
void Func_020030a2();
void Func_020030ae();
void Func_020030c0();
s32 Func_020030ea();
void Func_020030f6();
s32 Func_02003100();
void Func_02003104();
s32 Func_0200312a();
void Func_02003142();
void Func_0200314c();
void Func_02003156();
void Func_02003160();
s32 Func_0200316a();
void Func_0200318e();
void Func_02003198();
void Func_020031a2();
void Func_020031a8();
void Func_020031bc();
void Func_020031ca();
void Func_020031fc();
void Func_02003218();
void Func_02003224();

extern s16 Data_02000240[];

void Func_0200150c(void)
{
    void *p13;
    void *p17;
    s16 *progress = &Data_02000240[225];

    if ((u16)(*progress - 5) <= 1) {
        Func_02002fa2(0x12f);
    }
    if (Func_02002f98(0x109) != 0) {
        Func_02002fa8(0x242);
    }
    if (Func_02002fa8(0x834) == 0) {
    } else {
        Func_020030ea();
        Func_020030f6();
        Func_02002fda();
        Func_02003044(12, 0, 0);
        Func_0200304e(13, 0, 0);
        Func_02003058(14, 0, 0);
        Func_02003062(15, 0, 0);
        Func_0200306c(5, 0, 0);
        p13 = Func_0200302a(8);
        *(u8 *)((u8 *)(p13) + 89) |= 0x8;
        Func_02003088(11, 5439488, 0x01090000);
        Func_02003082(11, 83, 273);
        Func_020030a2(11, 5);
        p17 = Func_02003058(11);
        *(u16 *)(p17 + 32) = 12;
        Func_02003074(11, 0x02009c34);
        if (Func_02003032(0x839) != 0) {
            Func_020030c0(11, 0, 0);
        }
        Func_0200306c();
        Func_02003050(9, 24, 1, 1, 14, 21);
        Func_02003062(9, 24, 1, 1, 15, 21);
        Func_02003078(9, 24, 1, 1, 23, 19);
        Func_0200308a(9, 24, 1, 1, 24, 19);
        Func_0200309e(9, 24, 1, 1, 23, 20);
        Func_020030ae(9, 24, 1, 1, 24, 20);
    }
    Func_02003142(9, 0, 0);
    Func_0200314c(10, 0, 0);
    Func_02003156(1, 0, 0);
    Func_02003160(11, 0, 0);
    Func_02003104();
    if (Func_020030ea(0x109) == 0 && *progress == 10) {
        Func_02001ffa();
    }
    if (Func_02003100(0x801) != 0) {
        Func_0200318e(13, 0, 0);
        Func_02003198(14, 0, 0);
        Func_020031a2(15, 0, 0);
    } else {
        if (Func_0200312a(0x808) != 0) {
            Func_020031bc(14, 25690112, 24641536);
            Func_020031ca(15, 24641536, 24641536);
            Func_02003218(14, 65536);
            Func_02003224(15, 65536);
        }
    }
    if (Func_0200316a(0x87a) != 0) {
        Func_020031fc(16, 8650752, 17301504);
    }
    Func_020031a8();
}
