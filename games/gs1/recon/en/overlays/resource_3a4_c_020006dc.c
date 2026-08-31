#include "types.h"

#define FieldScene_RunEarlySequence Func_020006dc

/* Audited retained field-scene body at 0x020006dc.
 * The complete production body preserves 50 decoded calls, 3 loop(s), and 2 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200075a();
void Func_0200084e();
void Func_02000962();
void Func_0200418e();
void Func_020041d6();
void Func_020041ec();
void Func_02004208();
void Func_02004230();
void Func_02004236();
void Func_02004266();
void Func_020042ae();
void *Func_020042b8();
void *Func_020042c0();
void Func_020042ca();
void Func_020042e0();
void Func_020042e8();
void Func_020042ec();
void Func_020042f6();
void Func_020042fc();
void Func_02004320();
void Func_02004322();
void Func_0200432a();
void Func_0200432c();
void Func_02004350();
void Func_02004386();
void Func_020043c2();
void Func_020043d8();
void Func_020043da();
void Func_020043de();
void Func_020043f4();
void Func_020043fa();
void Func_02004408();
void Func_0200440e();
void Func_02004410();
void Func_02004418();
void Func_0200441e();
void Func_0200443e();
void Func_02004464();
void Func_0200449a();
void Func_020044a4();
void Func_020044d6();
void Func_020044ec();
void Func_02004522();
void Func_02004528();
void Func_0200452c();
void Func_02004598();
void Func_0200463c();
void Func_020046ac();

void Func_020006dc(void)
{
    s32 base;
    void *p5;
    void *p6;
    s32 i1;
    s32 i2;
    s32 i3;

    Func_02004408(230);
    Func_02004230(131072, 131072, 65536);
    Func_02004266(10);
    base = 6553;
    for (i1 = 0; i1 <= 23; i1++) {
        Func_0200418e(4);
        p5 = Func_020042b8(8);
        *(s32 *)(p5 + 24) = base;
        p6 = Func_020042c0(8);
        *(s32 *)(p6 + 28) = base;
        Func_02004320(8, 9961472, 14155776);
        Func_020042e8(8, 33602888);
    }
    Func_02004208(1, 0, 33588897);
    Func_020041d6(1);
    Func_0200075a();
    Func_020041ec(1);
    Func_02004236(1, 0, 0);
    Func_020044a4(289);
    Func_020042ec(-1, -1, 58982);
    Func_02004322(30);
    Func_020042f6(0, 0, 1, 2, 3, 14);
    Func_0200432c(2301);
    Func_02004322(93, 41, 16, 4, 77, 28);
    Func_02004528(230);
    Func_02004350(131072, 131072, 65536);
    Func_02004386(10);
    for (i2 = 23; i2 >= 0; i2--) {
        Func_020042ae(4);
    }
    Func_020042fc(1, 0, 33588897);
    Func_020042ca(1);
    Func_0200084e();
    Func_020042e0(1);
    Func_0200432a(1, 0, 0);
    Func_02004598(289);
    Func_020043d8(-1, -1, 58982);
    Func_0200440e(30);
    Func_020043da(77, 41, 16, 4, 77, 28);
    Func_02004418(2302);
    Func_020043fa(113, 31, 103, 17, 1, 1);
    Func_0200440e(111, 32, 104, 18, 3, 2);
    Func_0200441e(64, 32, 103, 18, 1, 2);
    Func_0200463c(230);
    Func_02004464(131072, 131072, 65536);
    Func_0200449a(10);
    for (i3 = 23; i3 >= 0; i3--) {
        Func_020043c2(4);
    }
    Func_02004410(1, 0, 33588897);
    Func_020043de(1);
    Func_02000962();
    Func_020043f4(1);
    Func_0200443e(1, 0, 0);
    Func_020046ac(289);
    Func_020044ec(-1, -1, 58982);
    Func_02004522(30);
    Func_020044d6(103, 14, 103, 17, 4, 3);
    Func_0200452c();
}
