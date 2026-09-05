#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020009ae();
void Func_020009d8();
s32 Func_02001e32();
s32 Func_02001e3c();
void Func_02001e86();
void Func_02001e8a();
s32 Func_02001eb4();
void Func_02001ed4();
void Func_02001ef2();
void Func_02001efa();
void Func_02001f10();
void Func_02001f36();
void Func_02001f50();
void Func_02001f5c();
void Func_02001f68();
void Func_02001f7c();
void Func_02001f82();
void Func_02001f98();
void Func_02001f9a();
void Func_02001fb4();
void Func_02001fb8();
void Func_02001fba();
void Func_02001fca();
void Func_02001fd0();
void Func_02002030();
void Func_02002044();
void Func_02002050();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene382_020004a0(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02001e32, 0x855) == 0) {
        record = Value1(Func_02001e3c, 0x856);
        if (record != 0) {
            goto L_020004cc;
        }
    }
    Func_02001f68((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    goto L_0200059e;
    L_020004cc:;
    Func_02001e86();
    record = Value1(Func_02001eb4, 0);
    if (record != 0) {
        Func_02001efa(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02001ed4, 2, 0xcccc, 0x6666);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 20) {
        Call3(Func_02001f10, 2, 0x190, 0x1c0);
    } else {
        Call2(Func_02001f9a, 0xcccc, 0x1999);
        Call4(Func_02001fb4, 0xe00000, -1, 0xa20000, 1);
        Func_02001f36(2, 224, 162);
        Func_02001fca();
    }
    Func_02001f7c(0, 2, 0);
    Func_02001ef2(20);
    Call1(Func_02001f98, 0x1327);
    Call3(Func_02001fba, 0x9002, 0, 20);
    Func_02001f82(0, 3);
    if (Value0(Func_020009ae) != 0) {
        Call1(Func_02001fb8, 0x132a);
        Func_02001fd0(2, 0);
        Func_020009d8();
        Func_02001e8a(20);
    }
    Func_02001f50(2);
    Func_02002030((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    Func_02002044();
    Func_02002050();
    Func_02001f5c();
    L_0200059e:;
}
