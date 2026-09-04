#include "types.h"

#define FieldScene_RunScene3ae_020008cc Func_020008cc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02001490();
s32 Func_02001e32();
void Func_02001e44();
s32 Func_02001e54();
s32 Func_02001e64();
s32 Func_02001e6e();
void Func_02001f08();
void Func_02001f7a();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ae_020008cc(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 1) {
        if (Value1(Func_02001e32, 0x8ac) == 0) {
            Call1(Func_02001e44, 0x8ac);
            Func_02001490();
        }
    }
    if (Data_02000240_t[225][0] == 2) {
        if (Value1(Func_02001e54, 0x109) == 0) {
            Call1(Func_02001e6e, 0x8a9);
        }
    }
    if (Value1(Func_02001e64, 0x911) != 0) {
        if (Value1(Func_02001e6e, 0x8a9) == 0) {
            Call3(Func_02001f08, 12, 0x580000, 0x5180000);
            Func_02001f7a(12, 0, 0);
        }
    }
}
