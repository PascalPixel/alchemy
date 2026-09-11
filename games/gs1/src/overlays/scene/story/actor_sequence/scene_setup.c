#include "types.h"

#define FieldScene_DrawTilesAndResetActor8 Func_020003f4
#define FieldScene_RunLeaderMovementSequence Func_020015bc
#define FieldScene_InitializeStoryActors Func_02001668
extern u8 Data_03001ebc[];
extern u8 Data_00000068[];
extern u8 Data_0000009f[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

u8 *Func_02001dfc(s32);
u8 *Func_02001e04(s32);
void Func_02001da8(s32, s32, s32, s32, s32, s32);
void Func_02001dba(s32, s32, s32, s32, s32, s32);
void Func_02001dcc(s32, s32, s32, s32, s32, s32);
void Func_02001de0(u8 *, s32);
void Func_02001e26(s32);
void Func_02002fa0();
void Func_02002fb2();
void Func_02002fee();
void Func_0200300c();
void Func_02003022();
void Func_02003032();
void Func_02003040();
void Func_02003042();
void Func_02003048();
void Func_02003050();
void Func_02003052();
void Func_02003060();
void Func_02003064();
void Func_02003070();
void Func_02003076();
void Func_02003082();
void Func_0200308c();
void Func_02003094();
void Func_020030da();
void Func_020030e4();
void Func_020030ea();
void Func_020030fe();
void Func_02003100();
void Func_02001c7c_init();
void Func_02002a10_init();
void Func_02002cce_init();
void Func_02002edc_init();
void Func_02003044_init();
s32 Func_02003052_init();
void Func_02003058_init();
void Func_02003062_init();
void Func_0200307a_init();
s32 Func_020030a2_init();
s32 Func_020030ac_init();
s32 Func_020030ca_init();
s32 Func_020030d8_init();
s32 Func_020030e2_init();
void Func_02003110_init();
void Func_02003116_init();
void Func_0200311c_init();
void Func_02003122_init();
void Func_02003128_init();
void Func_0200312e_init();
void Func_02003134_init();
void Func_0200313a_init();
void Func_02003140_init();
void Func_02003146_init();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

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

void FieldScene_DrawTilesAndResetActor8(void)
{
    u8 *actor = Func_02001dfc(8);
    Func_02001e04(0);
    {
        s32 cell = 14;
        s32 row = 4;
        Func_02001da8(17, 4, 1, 1, cell, row);
        Func_02001dba(15, 3, 1, 1, 15, row);
        Func_02001dcc(15, 3, 1, 1, 13, row);
    }
    if (actor != 0) {
        Func_02001de0(actor, 0);
        actor[0x55] = 2;
        actor[0x23] = 1;
    }
    Func_02001e26(0x200);
}

void FieldScene_RunLeaderMovementSequence(void)
{
    Func_02002fa0();
    Func_0200308c();
    Func_02002fee(0, 152, 168);
    Func_0200300c(0);
    Func_02002fb2(20);
    Func_020030da(146, 1);
    Func_020030ea(0, 0);
    Func_020030fe();
    Func_020030e4(1);
    Func_02003100();
    Func_02003022(0, 144, 184);
    Func_02003040(0);
    Func_02003032(0, 88, 184);
    Func_02003050(0);
    Func_02003042(0, 88, 200);
    Func_02003060(0);
    Func_02003052(0, 72, 200);
    Func_02003070(0);
    Call3(Func_02003064, 0, 72, 0x120);
    Func_02003082(0);
    Call3(Func_02003076, 0, 88, 0x120);
    Func_02003094(0);
    Func_02003048();
}

s32 FieldScene_InitializeStoryActors(void)
{
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000068) {
        *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
        Func_02001c7c_init();
        if (Value1(Func_02003052_init, 0xfd1) == 0) {
            Func_02002edc_init(20);
        }
        record = Value1(Func_020030a2_init, 8);
        if (record != 0) {
            Func_02003044_init(record, 0);
        }
        Call1(Func_0200307a_init, 0x201);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009f) {
        *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
        Data_02000240_t[289][0] = 10;
        if (Data_02000240_t[225][0] == 4) {
            if (Value1(Func_020030ac_init, 0x109) == 0) {
                Func_02002a10_init();
            }
        }
        if (Data_02000240_t[225][0] == 3) {
            if (Value1(Func_020030ca_init, 0x109) == 0) {
                Func_02002cce_init();
            }
        }
        if (Value1(Func_020030d8_init, 0x941) != 0) {
            if (Value1(Func_020030e2_init, 0x94d) == 0) {
                Call2(Func_02003058_init, 0x2008ac5, 0xc80);
            }
        }
        Call2(Func_02003062_init, 0x2009241, 0xc80);
        Call1(Func_02003110_init, 0x944);
        Call1(Func_02003116_init, 0x945);
        Call1(Func_0200311c_init, 0x946);
        Call1(Func_02003122_init, 0x947);
        Call1(Func_02003128_init, 0x948);
        Call1(Func_0200312e_init, 0x943);
        Call1(Func_02003134_init, 0x949);
        Call1(Func_0200313a_init, 0x94a);
        Call1(Func_02003140_init, 0x94b);
        Call1(Func_02003146_init, 0x94c);
    }
    return 0;
}
