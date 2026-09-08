#include "types.h"

extern u8 Data_00000002[];
extern u8 Data_000000a5[];
extern s16 Data_02000240[];
s32 Func_02001e70();
void Func_02001e74();
void Func_02001e86();
void Func_02001ea6();
s32 Func_02001ea8();
s32 Func_02001eb0();
void Func_02001eec();
void Func_02001f14();
s32 Func_02001f16();
void Func_02001f68();
void Func_02001fb6();
void Func_02001fd4();
void Func_02001fe0();
void Func_02001ff2();
void Func_02001ffc();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_02000c00(void)
{
    u8 *rec;
    u8 *rec2;
    s32 rec8;
    s32 record;
    s32 count;

    rec8 = Value1(Func_02001e70, 0x218);
    {
        u8 *base = (u8 *)Data_02000240;

        rec = (u8 *)Value1(Func_02001ea8, *(s32 *)(base + 500));
        rec2 = (u8 *)Value1(Func_02001eb0, rec8);
        Func_02001ea6();
        Call4(Func_02001f68, -1, -1, -1, 0);
        Func_02001fb6(219);
        Func_02001e86(*(s32 *)(base + 500), 0);
    }
    {
        s32 zero = 0;

        rec2[85] = zero;
        rec[85] = zero;
        *(s32 *)(rec + 40) = zero;
    }
    *(u8 *)(rec + 85 + 12) = 1;
    rec2[97] = 1;
    count = 59;
    do {
        *(volatile s32 *)(rec + 40) += 0x3333;
        *(volatile s32 *)(rec2 + 40) += 0x3333;
        Func_02001e74(1);
        count = count - 1;
    } while (count >= 0);
    Func_02001fd4();
    Func_02001fe0();
    Func_02001f14();
    Call1(Func_02001eec, 0x122);
    if (Data_02000240[224] == (s32)Data_000000a5) {
        record = Value1(Func_02001f16, 0x218);
        if (record != 11) {
            goto tail;
        }
        Func_02001ff2((s32)Data_00000002, 77);
        return;
    }
tail:;
    Func_02001ffc((s32)Data_00000002, 27);
}
