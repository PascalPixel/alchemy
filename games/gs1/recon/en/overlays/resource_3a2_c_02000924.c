#include "types.h"

#define RunEventScript01 Func_02000924

/* Audited 41-call script for the complete 0x02000924 owner.
 * Recovered from the bounded canonical owner. */

s32 Func_02001dfa();
void Func_02001e24();
void Func_02001e48();
void *Func_02001e6e();
void *Func_02001e78();
void *Func_02001e92();
void *Func_02001ea2();
void Func_02001ea4();
void *Func_02001eb4();
void *Func_02001ebc();
void Func_02001ec8();
void Func_02001ed2_a();
void Func_02001ed2_b();
void Func_02001ede();
void Func_02001eea();
void Func_02001eee();
void Func_02001f14();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f3c();
void Func_02001f3e();
void Func_02001f48();
void Func_02001f4a_a();
void Func_02001f4a_b();
void Func_02001f4e();
void *Func_02001f50();
void Func_02001f5c();
void Func_02001f6a();
void *Func_02001f70();
void Func_02001f76();
void Func_02001f8c();
void Func_02001f94();
void Func_02001fa4();
void Func_02001fd2();
void Func_02001fd6();
void Func_02001fe8();
void Func_02001ff0();
void Func_02002014();
void Func_0200202e();
void Func_02002030();
void Func_02002038();

void RunEventScript01(void)
{
    void *p10;
    void *p14;
    void *p31;
    void *p8;

    if (Func_02001dfa(2202) == 0) {
    } else {
        Func_02001e24();
        Func_02001e92(10, 35127296, 14155776);
        Func_02001ec8(6325);
        Func_02001eea(10, 0, 20);
        Func_02001ed2_a(0, 2);
        Func_02001e48(20);
        p8 = Func_02001e6e(0);
        *(s32 *)(p8 + 108) = 33587285;
        p10 = Func_02001e78(0);
        if ((*(s32 *)(p10 + 16) >> 20) == 13) {
            Func_02001ebc(0, 440, 200);
        }
        Func_02001ea2(10, 131072, 65536);
        Func_02001f4a_a(10, 2);
        Func_02001ede(10, 408, 216);
        p14 = Func_02001eb4(10);
        *(u8 *)((u8 *)(p14) + 35) |= 0x1;
        Func_02001ea4(10);
        Func_02001f70(10, 32768, 20);
        Func_02001f6a(10, 0, 20);
        Func_02001f4a_b(10, 2);
        Func_02001fa4(10, 258);
        Func_02001ed2_b(60);
        Func_02001f8c(10, 0, 20);
        Func_02001f14(10, 33592876);
        Func_02001fd6(19398656, -1, 22544384, 1);
        Func_02001eee(2224);
        Func_02001f3c(10);
        Func_02001ff0();
        Func_02001f3e(0, 65536, 32768);
        Func_02001f4e(0, 33593016);
        Func_02001f5c(0);
        Func_02001f2a(10);
        p31 = Func_02001f50(0);
        *(s32 *)(p31 + 108) = 0;
        Func_02001f3a(30);
        Func_02001fd2(10, 2);
        Func_02001f48(20);
        Func_02002014(10, 20480, 120);
        Func_0200202e(10, 261, 60);
        Func_02002038(0, 257, 60);
        Func_02001fe8(10, 4);
        Func_02001f76(20);
        Func_02002030(10, 0, 20);
        Func_02001f94();
    }
}
