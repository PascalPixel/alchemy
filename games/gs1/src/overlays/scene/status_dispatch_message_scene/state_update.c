#include "types.h"

#define SceneState_ApplyFlags565And564 Func_020011ac
#define SceneState_SetRuntimeWord448To513 Func_0200160c
#define SceneState_DispatchByStatus448 Func_02001a14
#define SceneState_ForwardMaskedHalfwordWith10 Func_02001c5c
extern u8 *Data_03001ebc;
extern u8 Data_02000240[];
extern u8 Value_0000008c;
extern u8 Value_0000008e;

void Func_02003d7e_a();
void Func_02003d7e_b();
void Func_02004204();
void Func_0200433e();
void Func_02004352();
void Func_02004218();
void Func_02003292();
void Func_020042d0();
s32 Func_020041fe();
void Func_020032a2();
void Func_020032c0();
void Func_0200422e();
void Func_0200426a();
void Func_02003476(void);
void Func_020036a2(void);
void Func_02004922(s32 arg0, u16 arg1, s32 arg2);

/*
 * Story-flag hand-off in overlay resource_3b9: clear one flag, set its
 * immediate neighbour.  It is installed into a script record rather than
 * called, and each Func_ name spells a relocated call word.
 */

/*
 * resource_3b9 owner at 0x02001c5c, 12 bytes with no literal pool: a
 * single-call trampoline forwarding `arg0' unchanged, `arg1' masked to its
 * low 16 bits, and a fixed third argument.  `arg0' is never freshly loaded
 * before the call, so it is a pass-through parameter.  The callee is not
 * identified beyond call shape.
 */

/*
 * The two flag ids are adjacent but spelled differently, and the spellings
 * are load-bearing.  565 is 0x235 and comes from the owner's single pool
 * word; 564 is built as movs #141 / lsls #2, so it must stay a plain decimal
 * value rather than another pool constant.  The 24-byte owner covers that
 * pool word and the alignment halfword after it.  What the pair gates is not
 * established.
 */
void SceneState_ApplyFlags565And564(void)
{
    Func_02003d7e_a(0x235);
    Func_02003d7e_b(564);
}

void SceneState_SetRuntimeWord448To513(void)
{
    Func_02004204();
    *(s32 *)((u8 *)Data_03001ebc + 448) = 513;

    Func_0200433e();
    Func_02004352();

    Func_02004218(20);
    Func_02003292(17, 160 << 7);
    Func_020042d0(0x206e);

    if (Func_020041fe(0x8a4) != 0) {
        *(u16 *)((u8 *)Data_03001ebc + 472) =
            (u16)(*(u16 *)((u8 *)Data_03001ebc + 472) + 1);
    }

    Func_020032a2(17);
    Func_020032c0(17, 192 << 6);
    Func_0200422e(0x8a3);

    Func_0200426a();
}

s32 SceneState_DispatchByStatus448(void)
{
    s32 off = 448;
    s16 status = *(s16 *)(Data_02000240 + off);

    if (status == (s32)&Value_0000008c) {
        Func_02003476();
    } else if (status == (s32)&Value_0000008e) {
        Func_020036a2();
    }
    return 0;
}

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    Func_02004922(arg0, (u16)arg1, 10);
}
