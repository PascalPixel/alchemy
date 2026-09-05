#include "types.h"

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


void Func_02000aa0(void)
{
    s32 i;
    u8 *rec7;
    s32 record;
    s32 count;
    volatile s32 *tick;
    s32 base5_200a230;

    rec7 = (u8 *)Value1(Func_02001bd6, 10);
    if (rec7[91] == 0) {
        tick = (volatile s32 *)Data_0200a22c;
        count = *tick + 1;
        *tick = count;
        if ((63 & count) == 0) {
            base5_200a230 = (s32)Data_0200a230;
            record = Func_02001b48();
            record = Value2(Func_02001b36, record, 6);
            *(volatile s32 *)base5_200a230 = record;
            rec7 = Func_02001c06((record + 10));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = Func_02001c1a((i + 10));
            record = Value1(Func_02001bf6, (i + 0x200));
            if (record != 0) {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                *(s32 *)((s32)rec7 + 72) = 0;
                *(s32 *)((s32)rec7 + 40) = 0;
                Func_02001c66(106);
            } else {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)((s32)rec7 + 72) = record;
                *(s32 *)((s32)rec7 + 40) = record;
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                Func_02001c82(106);
            }
        }
    }
}
