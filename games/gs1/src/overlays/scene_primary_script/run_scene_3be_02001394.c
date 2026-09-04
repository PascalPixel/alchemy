#include "types.h"

#define FieldScene_RunScene3be_02001394 Func_02001394

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000098[];
extern u8 Data_0000009e[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_020028b4();
void Func_020028c4();
void Func_020028f8();
s32 Func_020028fa();
void Func_0200292a();
s32 Func_0200292e();
void Func_0200293e();
void Func_02002974();
void Func_02002984();
s32 Func_02002990();
void Func_020029a4();
void Func_020029b2();
void Func_020029c2();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 FieldScene_RunScene3be_02001394(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        Call1(Func_020028b4, 0x144);
        if (Value1(Func_020028b4, 0x9a0) != 0) {
            Func_0200292a(11, 0, 0);
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
        if (Data_02000240_t[225][0] == 1) {
            Call6(Func_020028c4, 108, 17, 1, 1, 107, 17);
        }
        if (Value1(Func_020028fa, 0x9a2) != 0) {
            Call3(Func_02002974, 8, 0x1b80000, 0x1340000);
            Func_02002984(8, 2);
            Call6(Func_020028f8, 29, 19, 1, 1, 27, 19);
        }
        if (Value1(Func_0200292e, 0x9a5) != 0) {
            Func_020029a4(9, 0, 0);
            Call3(Func_020029b2, 10, 0x2b80000, 0x1200000);
            Func_020029c2(10, 2);
        }
        record = Func_02002990(12);
        Func_0200293e(record, 0);
    }
    return 0;
}
