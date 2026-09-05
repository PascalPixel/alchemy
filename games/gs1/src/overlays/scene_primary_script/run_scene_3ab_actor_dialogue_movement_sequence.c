#include "types.h"

#define FieldScene_RunActorDialogueMovementSequence Func_0200131c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002cfe();
void Func_02002d00();
void Func_02002d58();
void Func_02002d6e();
void Func_02002d7c();
void Func_02002d8a();
void Func_02002d92();
void Func_02002d9c();
void Func_02002da6();
void Func_02002db4();
void Func_02002dc0();
void Func_02002dca();
void Func_02002dd6();
void Func_02002ddc();
void Func_02002de0();
void Func_02002de2();
void Func_02002de6();
void Func_02002df2();
void Func_02002dfe();
void Func_02002e06();
u8 *Func_02002e0a();
u8 *Func_02002e1a();
void Func_02002e26();
void Func_02002e32();
void Func_02002e42();
u8 *Func_02002e4e();
void Func_02002e56();
u8 *Func_02002e52();
u8 *Func_02002e62();
s32 Func_02002e6e();
void Func_02002e72();
s32 Func_02002e84();
void Func_02002e86();
void Func_02002e88();
void Func_02002e8e();
void Func_02002e90();
void Func_02002e96();
u8 *Func_02002ea8();
void Func_02002eb0();
u8 *Func_02002ec4();
void Func_02002ecc();
s32 Func_02002ede();
void Func_02002ee0();
void Func_02002ee6();
void Func_02002f12();
u8 *Func_02002f1c();
void Func_02002f26();
u8 *Func_02002f2c();
void Func_02002f2e();
void Func_02002f44();
void Func_02002f4a();
void Func_02002f56();
void Func_02002f58();
void Func_02002f5e();
void Func_02002f60();
void Func_02002f7a();
void Func_02002f7e();
void Func_02002f84();
void Func_02002f8a();
void Func_02002f96();
void Func_02002fa2();
void Func_02002fa8();
void Func_02002fc0();
void Func_02002fc8();
void Func_02002fd4();
void Func_02003022();
void Func_0200302e();
void Func_020092a5();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunActorDialogueMovementSequence(void)
{
    u32 i;
    u8 *record;
    s32 base6_20092a5;

    Func_02002d00();
    Call3(Func_02002d6e, 0, 0xa00000, 0x800000);
    Call3(Func_02002d7c, 8, 0x980000, 0x700000);
    Call3(Func_02002d8a, 9, 0xa80000, 0x700000);
    Call3(Func_02002de6, 0, 0x4000, 0);
    Call3(Func_02002df2, 17, 0x3000, 0);
    Call3(Func_02002dfe, 18, 0x5000, 0);
    Func_02002e26(0, 0);
    Func_02002e42();
    Func_02002d58(30);
    Call3(Func_02002d92, 0, 0x1cccc, 0xe666);
    Call3(Func_02002d9c, 8, 0x1cccc, 0xe666);
    Call3(Func_02002da6, 9, 0x1cccc, 0xe666);
    Call3(Func_02002dca, 8, 152, 0x120);
    Call3(Func_02002dd6, 9, 168, 0x120);
    Func_02002e06(0, 4);
    Call3(Func_02002de2, 0, 160, 0x128);
    base6_20092a5 = (s32)Func_020092a5;
    Call2(Func_02002cfe, base6_20092a5, 0xc80);
    Func_02002db4(1);
    Func_02002f12(121);
    Func_02002dc0(20);
    Func_02002e88(8, 3);
    Func_02002e90(9, 3);
    Func_02002f2e(121);
    Func_02002ddc(30);
    {
        u8 *record = Func_02002e0a(8);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    {
        u8 *record = Func_02002e1a(9);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Func_02002e72(0, 4);
    Func_02002f60(121);
    Call1((void (*)())Func_02002e6e, 0);
    Func_02002e86(8, 1);
    Func_02002e8e(9, 1);
    Func_02002d7c(base6_20092a5);
    {
        u8 *record = Func_02002e52(0);
        u8 value = *(volatile u8 *)&record[85];
    
        record[85] = (u8)(value | 2);
    }
    record = Func_02002e62(0);
    *(s32 *)(record + 40) = 0x60000;
    record = Value1(Func_02002e6e, 0);
    *(s32 *)(record + 44) = 0x60000;
    Func_02002e4e(1);
    goto wait_e84;
    do {
        Func_02002e56(1);
    wait_e84:
        record = (u8 *)Value1(Func_02002e84, 0);
    } while (*(s32 *)(record + 12) != 0);
    Call3(Func_02002f26, 0, 0xc000, 0);
    Func_02002ee6(0, 19);
    Func_02002fd4(127);
    Call2(Func_02002f56, 0, 0x102);
    Call2(Func_02002de0, (s32)Func_020092a5, 0xc80);
    Func_02002e96(2);
    record = Func_02002ec4(0);
    *(s32 *)(record + 40) = 0x30000;
    Func_02002ea8(1);
    goto wait_ede;
    do {
        Func_02002eb0(1);
    wait_ede:
        record = (u8 *)Value1(Func_02002ede, 0);
    } while (*(s32 *)(record + 12) != 0);
    Call2(Func_02002f96, 0, 0x102);
    Func_02002ecc(10);
    Func_02002f44(0, 1);
    Call1(Func_02002e32, (s32)Func_020092a5);
    Func_02002ee0(50);
    Call1(Func_02002f7e, 0x2410);
    Func_02002f96(8, 0);
    {
        u8 *record = Func_02002f1c(8);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    {
        u8 *record = Func_02002f2c(9);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Call3(Func_02002f4a, 8, 0x10000, 0x8000);
    Call3(Func_02002f58, 9, 0x10000, 0x8000);
    Func_02002f7a(8, 144, 200);
    Func_02002f84(9, 176, 200);
    Func_02002fa2(8);
    Func_02002fa8(9);
    Func_02002fc0(8, 1);
    Func_02002fc8(9, 1);
    Func_02002f5e(30);
    Call3(Func_02003022, 8, 0x3000, 0);
    Call3(Func_0200302e, 9, 0x5000, 0);
    Func_02002f8a();
}
