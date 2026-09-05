#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02004c06();
void Func_02004c6a();
void Func_02004c8a();
void Func_02004cae();
s32 Func_02004cc2();
void Func_02004cfe();
void Func_02004d00();
void Func_02004d26();
s32 Func_02004d36();
s32 Func_02004d54();
void Func_02004d70();
s32 Func_02004d78();
void Func_02004d96();
s32 Func_02004d98();
void Func_02004e0c();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunClosingAuxiliarySequence(void)
{
    u32 i;
    u8 *p9;
    s32 rec;
    s32 rec7;
    u8 *record;
    u8 *p6;

    u8 *base = Data_02000240;

    p6 = *(u8 **)(base + 500);
    rec = Value1(Func_02004cc2, 0x362);
    if (rec == 0) {
        record = Value1(Func_02004d36, 10);
        if ((s32)record != 0) {
            Func_02004d70((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004d96((s32)p6);
        record = Value1(Func_02004d54, 11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c6a, (s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Value1(Func_02004d78, 10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c8a, (s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Func_02004d98((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Call4(Func_02004cae, rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Func_02004cfe(rec7, 1);
        Func_02004e0c((s32)p6);
        Call6(Func_02004d00, 0, 24, 1, 1, 9, 12);
        Func_02004c06(2);
        Func_02004d26(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
        Call1(Func_02004d96, 0x367);
    }
}
