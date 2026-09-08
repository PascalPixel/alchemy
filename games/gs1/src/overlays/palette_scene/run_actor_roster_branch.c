#include "types.h"

#define PaletteScene_RunActorRosterBranch Func_02000088

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001520[];
void Func_02001388();
void Func_020013c8();
void Func_020013d6();
void Func_020013fe();
void Func_02001416();
void Func_0200142e();
s32 Func_02001a1a();
void Func_02001a24();
s32 Func_02001a28();
void Func_02001a6a();
void Func_02001a8a();
void Func_02001aa0();
void Func_02001aba();
void Func_02001ad2();
void Func_02001ade();
void Func_02001ae2();
void Func_02001aea();
void Func_02001aec();
void Func_02001b04();
void Func_02001b24();
void Func_02001b32();
void Func_02001b52();
void Func_02001b64();

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

void PaletteScene_RunActorRosterBranch(void)
{
    s32 record;

    Func_02001a24();
    Func_02001388(11, 1);
    record = Value1(Func_02001a1a, 0x845);
    if (record != 0) {
        record = 0x151d;
    } else {
        record = Value1(Func_02001a28, 0x84c);
        if (record == 0) {
            goto L_020000be;
        }
        record = 0x1525;
    }
    Func_02001ad2(record);
    Func_02001aea(9, 0);
    goto L_02000134;
    L_020000be:;
    Func_02001ae2(Data_00001520);
    Func_02001b04(9, 0, 20);
    Func_020013c8(11, 0);
    Func_02001a6a(60);
    Func_020013d6(11, 1);
    Func_02001b24(9, 0, 10);
    Func_02001aec(0, 3);
    Func_02001a8a(40);
    Func_02001b32(9, 0);
    Func_020013fe(11, 0);
    Func_02001aa0(80);
    Func_02001b52(9, 0, 20);
    Func_02001416(11, 1);
    Func_02001b64(9, 0, 20);
    Call1(Func_02001aba, 0x84c);
    L_02000134:;
    Func_0200142e(11, 0);
    Func_02001ade();
}
