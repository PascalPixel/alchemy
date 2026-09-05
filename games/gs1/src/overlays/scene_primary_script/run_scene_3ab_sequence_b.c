#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
s32 Func_020023aa();
s32 Func_020023b4();
s32 Func_020023be();
void Func_020023fa();
void Func_0200247c();
void Func_0200249a();
void Func_0200249c();
void Func_020024aa();
void Func_020024c0();
void Func_020024c2();
void Func_020024c6();
void Func_020024c8();
void Func_020024e0();
void Func_0200252a();

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

void FieldScene_RunScene3abSequenceB(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[147][0] == 0) {
        record = Value1(Func_020023aa, 0x941);
        if (record == 0) {
            goto L_02000a54;
        }
        record = Value1(Func_020023b4, 0x94d);
        if (record != 0) {
            goto L_02000a54;
        }
        record = Value1(Func_020023be, 0x9af);
        if (record == 0) {
            *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17e)) = record;
            Func_0200252a();
            Func_0200247c(8, 0, 0);
            Call3(Func_020024c6, 8, 0x101, 60);
            Call1(Func_0200249c, 0x24db);
            Call1(Func_020023fa, 0x9af);
        } else {
            Call1(Func_020024aa, 0x24e7);
        }
        Func_020024c2(8, 0);
        Func_0200249a(8, 1);
        Call1(Func_020024c0, 0x24dc);
    } else {
        L_02000a54:;
        Call1(Func_020024c8, 0x1bbf);
    }
    Func_020024e0(8, 0);
}
