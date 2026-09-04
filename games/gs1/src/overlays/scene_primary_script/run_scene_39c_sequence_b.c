#include "types.h"

#define FieldScene_RunScene39cSequenceB Func_020036ac

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000036[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02003756();
void Func_02009126();
void Func_02009128();
void Func_02009166();
s32 Func_020091ca();
s32 Func_020091d6();
void Func_020091de();
s32 Func_020091fc();
void Func_02009202();
void Func_0200922a();
void Func_02009272();
void Func_02009296();
void Func_020092a4();
void Func_020092be();
void Func_020092d2();
void Func_02009328();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

void FieldScene_RunScene39cSequenceB(void)
{
    u32 i;
    s32 record;

    Func_02009166();
    if (Data_02000240_t[224][0] == (s32)Data_00000036) {
        Call3(Func_020091de, 0, 0x1d8, 0x258);
        Call3(Func_02009272, 0, 0x4000, 10);
        Call4(Func_020092a4, 0x1d00000, -1, 0x2900000, 1);
        record = Func_020091ca(0);
        Func_02009128(record, 0);
        record = Func_020091d6(0);
        (void)Func_02003756(*(volatile s32 *)(record + 8), 0, 0x2be0000, 223);
        Call6(Func_02009126, 92, 46, 92, 40, 3, 2);
        record = Func_020091fc(0);
        *(volatile s32 *)(record + 72) = 0x8000;
        Func_020092d2(0, 2);
        Call3(Func_02009296, 0, 6, -1);
        *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x203;
        Func_02009202(60);
        Func_02009328(8);
    } else {
        Call3(Func_020092be, 0, 6, -1);
    }
    Func_0200922a();
}
