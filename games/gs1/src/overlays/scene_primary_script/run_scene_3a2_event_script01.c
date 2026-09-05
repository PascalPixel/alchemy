#include "types.h"

#define RunEventScript01 Func_02000924

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001dfa();
void Func_02001e24();
void Func_02001e48();
u8 *Func_02001e6e();
s32 Func_02001e78();
void Func_02001e92();
void Func_02001ea2();
void Func_02001ea4();
u8 *Func_02001eb4();
void Func_02001ebc();
void Func_02001ec8();
void Func_02001ed2();
void Func_02001ede();
void Func_02001eea();
void Func_02001eee();
s32 Func_02001f14();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f3c();
void Func_02001f3e();
void Func_02001f48();
void Func_02001f4a();
void Func_02001f4e();
u8 *Func_02001f50();
void Func_02001f5c();
void Func_02001f6a();
void Func_02001f70();
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_02001dfa, 0x89a) == 0) {
    } else {
        Func_02001e24();
        Call3(Func_02001e92, 10, 0x2180000, 0xd80000);
        Call1(Func_02001ec8, 0x18b5);
        Func_02001eea(10, 0, 20);
        Func_02001ed2(0, 2);
        Func_02001e48(20);
        record = Func_02001e6e(0);
        *(s32 *)(record + 108) = 0x2008055;
        record = Value1(Func_02001e78, 0);
        if ((*(s32 *)(record + 16) >> 20) == 13) {
            Call3(Func_02001ebc, 0, 0x1b8, 200);
        }
        Call3(Func_02001ea2, 10, 0x20000, 0x10000);
        Func_02001f4a(10, 2);
        Call3(Func_02001ede, 10, 0x198, 216);
        {
            u8 *record = Func_02001eb4(10);
            u8 value = *(volatile u8 *)&record[35];
        
            record[35] = (u8)(value | 1);
        }
        Func_02001ea4(10);
        Call3(Func_02001f70, 10, 0x8000, 20);
        Func_02001f6a(10, 0, 20);
        Func_02001f4a(10, 2);
        Call2(Func_02001fa4, 10, 0x102);
        Func_02001ed2(60);
        Func_02001f8c(10, 0, 20);
        Value2(Func_02001f14, 10, 0x200962c);
        Call4(Func_02001fd6, 0x1280000, -1, 0x1580000, 1);
        Call1(Func_02001eee, 0x8b0);
        Func_02001f3c(10);
        Func_02001ff0();
        Call3(Func_02001f3e, 0, 0x10000, 0x8000);
        Call2(Func_02001f4e, 0, 0x20096b8);
        Func_02001f5c(0);
        Func_02001f2a(10);
        i = 0;
        record = Func_02001f50(0);
        *(s32 *)(record + 108) = i;
        Func_02001f3a(30);
        Func_02001fd2(10, 2);
        Func_02001f48(20);
        Call3(Func_02002014, 10, 0x5000, 120);
        Call3(Func_0200202e, 10, 0x105, 60);
        Call3(Func_02002038, 0, 0x101, 60);
        Func_02001fe8(10, 4);
        Func_02001f76(20);
        Func_02002030(10, 0, 20);
        Func_02001f94();
    }
}
