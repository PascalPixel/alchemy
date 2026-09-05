#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000069[];
extern u8 Data_000000a9[];
extern u8 Data_000000aa[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_020009a8();
s32 Func_020009da();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
s32 Func_02000a4c();
void Func_02000a4e();
void Func_02000a54();
void Func_02000a58();
void Func_02000a80();
void Func_02000ab6();
s32 Func_02000ae6();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b1e();
void Func_02000b20();
void Func_02000b34();
void Func_02000b36();
s32 Func_02000b52();
s32 Func_02000bc0();
void Func_02000c16();
void Func_02000c46();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

s32 Func_02000124(void)
{
    s16 *q;
    s16 *p;
    s16 *r;
    s32 v;

    if (Value1(Func_020009a8, 0x89f) != 0) {
        s32 a = (s32)Data_00000069;

        q = (s16 *)Data_02000240;
        q[226] = a;
        {
            s16 *t = q + 227;
            s32 shown = 10;

            *t = shown;
        }
    }
    p = (s16 *)Data_02000240;
    v = p[224];
    if (v == (s32)Data_000000a9) {
        if (Value1(Func_020009da, 0x897) != 0) {
            Func_02000a58(10, 0, 0);
        }
        if (p[225] == 3) {
            if (Value1(Func_020009fc, 0x8fb) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 1;

                    *t = shown;
                }
            }
            if (Value1(Func_02000a16, 0x8fc) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 5;

                    *t = shown;
                }
            }
            Call1(Func_02000a40, 0x12f);
        }
        r = (s16 *)Data_02000240;
        if (r[225] == 1) {
            Call1(Func_02000a4e, 0x8fb);
            if (Value1(Func_02000a4c, 0x96f) == 0) {
                Call6(Func_02000a54, 6, 0, 2, 1, 8, 27);
            }
        }
        if (r[225] != 5) {
            goto L_0200024c;
        }
        Call1(Func_02000a80, 0x8fc);
    } else {
        if (v == (s32)Data_000000aa) {
            Func_02000b00(8, 4);
            Func_02000b08(9, 4);
            Func_02000b10(10, 3);
            Func_02000b18(11, 4);
            Func_02000b20(12, 3);
            *(volatile s32 *)(Func_02000ae6(15) + 28) = 0x19999;
            Call6(Func_02000ab6, 108, 38, 1, 1, 102, 56);
        }
    }
    L_0200024c:;
    return 0;
}

