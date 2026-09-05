#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_02002f96();
s32 Func_02002fe8();
s32 Func_02002ff0();
s32 Func_02002ffa();
s32 Func_02003002();
void Func_02003030();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a6SequenceC(void)
{
    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02002f96, 0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 8) {
            idx = *(s16 *)((s32)p5 + 0x16c);
            rec8 = Value1(Func_02002fe8, 8);
            record = Value1(Func_02002ff0, 0);
            *(volatile s32 *)(rec8 + 48) = *(volatile s32 *)(record + 48);
            rec8 = Value1(Func_02002ffa, 8);
            record = Value1(Func_02003002, 0);
            *(volatile s32 *)(rec8 + 52) = *(volatile s32 *)(record + 52);
            idx -= 45;
            tbl = 0x02009f00;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003030(8, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}
