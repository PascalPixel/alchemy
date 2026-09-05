#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020009c0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200a224[];
extern u8 Data_0200a228[];
extern u8 Data_0200a22c[];
extern u8 Data_0200a230[];
extern u8 Data_03001ebc[];
s32 Func_0200138a();
s32 Func_020013ba();
void Func_020015b0();
s32 Func_02001af6();
u8 *Func_02001b22();
s32 Func_02001b36();
s32 Func_02001b40();
s32 Func_02001b48();
s32 Func_02001b58();
void Func_02001b6a();
void Func_02001b8a();
void Func_02001b92();
void Func_02001bb8();
void Func_02001bc0();
void Func_02001bd0();
s32 Func_02001bd6();
s32 Func_02001bf6();
u8 *Func_02001c06();
u8 *Func_02001c1a();
void Func_02001c66();
void Func_02001c82();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 count;
    s32 index;
    volatile s32 *tick;
    volatile s32 *slot;
    u8 *table;

    flag = *(u8 *)(Func_02001af6(10) + 91);
    if (flag == 0) {
        tick = (volatile s32 *)Data_0200a224;
        count = *tick + 1;
        *tick = count;
        if (count > 190) {
            *tick = flag;
        }
        slot = (volatile s32 *)Data_0200a228;
        index = *slot;
        table = (u8 *)0x0200a214;
        if (*(s32 *)(table + (index << 2)) == *tick) {
            rec7 = Func_02001b22((index + 11));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
            count = *slot + 1;
            *slot = count;
            if (count > 3) {
                *slot = flag;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = Value1(Func_02001b40, (i + 11));
            if (*(volatile s32 *)((s32)rec7 + 40) >= 0) {
                if (*(volatile s32 *)((s32)rec7 + 12) <= 0xffff) {
                    Func_020015b0();
                    *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                    *(s32 *)((s32)rec7 + 72) = 0;
                    *(s32 *)((s32)rec7 + 40) = 0;
                    rec7[91] = 0;
                    Func_02001b8a(106);
                }
            }
        }
        if (Value1(Func_0200138a, 10) != 0) {
            Func_02001b92(10, 1);
            if (Value1(Func_02001b58, 0x207) == 0) {
                Call1(Func_02001b6a, 0x207);
                Func_02001bb8(204);
            } else {
                Func_02001bc0(106);
            }
        }
        if (Value1(Func_020013ba, 9) != 0) {
            Func_02001bd0(106);
        }
    }
}

