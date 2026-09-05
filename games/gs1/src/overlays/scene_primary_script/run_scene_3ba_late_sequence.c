#include "types.h"

#define FieldScene_RunLateSequence Func_02002844

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020049aa();
void Func_020049d6();
s32 Func_02004f36();
s32 Func_02004f62();
void Func_020063dc();
void Func_02006512();
void Func_02006532();
void Func_0200653c();
void Func_0200655a();
void Func_02006580();
void Func_02006600();
void Func_02006614();
void Func_0200662c();
void Func_02006644();
void Func_02006678();
void Func_020066a4();
void Func_020066aa();
void Func_020066b6();
s32 Func_020066d8();
void Func_020066da();
void Func_020066e6();

struct Work26 {
    u8 pad[26];
    u16 field;
};

extern struct Work26 Data_0200c628;
extern struct Work26 Data_0200be76;

void FieldScene_RunLateSequence(s32 a0)
{
    s32 scaled;
    s32 mag;
    s32 arg;
    struct Work26 *p1;
    struct Work26 *p2;

    Func_02006644(247);
    Func_02006600();
    Func_02006614();
    p1 = &Data_0200c628;
    p2 = &Data_0200be76;
    scaled = a0 * 60;
    p1->field = (u16)scaled;
    mag = a0;
    if (a0 < 0) {
        mag = -a0;
    }
    p2->field = (u16)(mag * 60);
    if (a0 < 0) {
        s32 n;

        Func_02006512(30);
        Func_02006678(86);
        Func_020049aa(8);
        Func_02004f36(3, 1);
        n = a0 + mag - mag;
        Func_02006532(((n - (n << 4)) << 2) + 60);
        arg = 0;
    } else {
        Func_0200653c(30);
        Func_020066a4(a0 + 90);
        Func_020049d6(4);
        Func_02004f62(3, 0);
        Func_0200655a(scaled + 60);
        arg = 8;
    }
    Func_0200662c(arg, 0x105, 0);
    while (Func_020066d8() != 0) {
        Func_020063dc(1);
    }
    Func_020066da(19);
    Func_02006580(30);
    Func_020066e6(0x121);
    Func_020066aa();
    Func_020066b6();
}
