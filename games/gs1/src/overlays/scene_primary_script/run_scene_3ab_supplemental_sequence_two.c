#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_020008ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000024db[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][2];
s32 Func_020022ca();
s32 Func_020022d4();
void Func_02002310();
void Func_02002372();
void Func_0200238c();
void Func_0200239a();
void Func_020023b2();
void Func_020023c0();
void Func_020023c2();
void Func_020023d8();

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

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 record;
    s32 base5_24db;

    if (Data_02000240_t[147][0] != 0) {
        Call1(Func_02002372, 0x2411);
    } else {
        if (Value1(Func_020022ca, 0x941) != 0) {
            record = Value1(Func_020022d4, 0x94d);
            if (record != 0) {
                goto L_0200094c;
            }
            Call3(Func_020023c2, 8, 0x101, 60);
            base5_24db = (s32)Data_000024db;
            Func_0200239a(base5_24db);
            Func_020023b2(8, 0);
            Func_0200238c(8, 1);
            Func_020023b2((base5_24db + 1));
            Call1(Func_02002310, 0x9af);
        } else {
            L_0200094c:;
            Call1(Func_020023c0, 0x1bb5);
        }
    }
    Func_020023d8(8, 0);
}
