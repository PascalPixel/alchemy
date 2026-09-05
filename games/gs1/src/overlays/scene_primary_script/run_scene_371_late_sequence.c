#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000800[];
extern u8 Data_00000c00[];
extern u8 Data_00000d00[];
extern u8 Data_00000e00[];
extern u8 Data_00000f00[];
extern u8 Data_00001000[];
extern u8 Data_02000434[];
extern s32 Data_02000240_t[][1];
extern u8 Data_02002090[];
extern u8 Data_04000208[];
extern u8 Data_0200e7a0[];
extern u8 Data_03001ebc[];
s32 Func_02007af8();
s32 Func_02007b20();
void Func_02007b9e();
s32 Func_02007bd6();
s32 Func_02007c00();
void Func_02007c0c();
void Func_02007c26();
s32 Func_02007c66();
void Func_02007c70();
void Func_02007cbc();
void Func_02007d7a();
void Func_02007dc4();
s32 Func_02007dd4();
void Func_02007dda();
void Func_02007e52();
void Func_02007e9a();
void Func_02007ee2();
void Func_02007f20();
void Func_02007f2c();
void Func_02007f40();
void Func_02007f5e();
void Func_02007f9c();
void Func_02007f9e();
void Func_02007fe2();
void Func_02008014();
void Func_02008026();
void Func_02008046();
void Func_02008078();
void Func_020080b0();
void Func_020080c0();
void Func_020080e4();
void Func_020080e8();
void Func_02008120();
void Func_02008180();
void Func_0200818c();
void Func_0200818e();
void Func_02008198();
void Func_020081a4();
void Func_02008318();
void Func_0200833a();
void Func_02008344();

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

void FieldScene_RunLateSequence(void)
{
    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Func_02007c66(Data_02000240_t[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Value2(Func_02007af8, *(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Func_02007b20() << 2) >> 16;
        switch (mode) {
        case 0:
            Call4(Func_02007dc4, (sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Func_02007dc4((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Call4(Func_02007dc4, (sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Call4(Func_02007dda, (sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

