#include "types.h"

#define EntryDriver_02000414 Func_02000414

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000001c[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
void Func_02000b5e();
void Func_02000b64();
s32 Func_02000b66();
u8 *Func_02000b6e();
void Func_02000b88();
u8 *Func_02000b96();
s32 Func_02000ba2();
void Func_02000bb0();
void Func_02000bdc();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 EntryDriver_02000414(void)
{
    u8 *record;
    u8 *base;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    base = Data_02000240;
    if (*(s16 *)(base + 0x1c0) == (s32)Data_0000001c) {
        if (*(s16 *)(base + 0x1c2) == 5) {
            Call1(Func_02000b5e, 0x12f);
        } else {
            {
                u8 *record = Func_02000b6e(8);
                u8 value = *(volatile u8 *)&record[89];
            
                record[89] = (u8)(value | 16);
            }
            if (Value1(Func_02000b66, 0x864) != 0) {
                Call3(Func_02000bb0, 8, 0x15a0000, 0x1240000);
                record = Func_02000b96(8);
                Func_02000b64((s32)record, 0);
                *(u8 *)(Func_02000ba2(8) + 35) |= 2;
                Func_02000bdc(8, 2);
                Call6(Func_02000b88, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}
