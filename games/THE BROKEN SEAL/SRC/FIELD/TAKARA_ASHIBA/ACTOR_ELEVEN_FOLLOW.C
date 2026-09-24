#include "TYPES.H"

#define TakaraAshiba_RunActorElevenFollowScene Func_02001694

extern u8 Data_02000240[];
void Func_02003bce();
void Func_02003bf8();
void Func_02003c02();
s32 Func_02003c06();
s32 Func_02003c18();
s32 Func_02003c38();
s32 Func_02003c4a();
void Func_02003c5a();
s32 Func_02003c72();
void Func_02003c7a();
void Func_02003c8e();
void Func_02003c94();
void Func_02003c9c();
void Func_02003caa();
void Func_02003cba();
void Func_02003cbc();
void Func_02003cbe();
void Func_02003cc0();
void Func_02003cce();
void Func_02003cd2();
void Func_02003ce0();
void Func_02003cf4();
void Func_02003d60();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Unless the scene is 11, bring actor 11 level with the leader (0.85
 * speed), play cue 188 twice around a facing beat and walk it to (0x158,
 * 0x168). */
void TakaraAshiba_RunActorElevenFollowScene(void)
{
    s32 record;
    s32 base3_2000240;
    s32 v3;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 11) {
        Func_02003bce();
        Call3(Func_02003bf8, 0, 0x1b333, 0xd999);
        Call3(Func_02003c02, 11, 0x1b333, 0xd999);
        Func_02003cc0(188);
        v3 = *(s32 *)(Value1(Func_02003c06, 0) + 8) / 0x100000;
        if (v3 > *(s32 *)(Value1(Func_02003c18, 11) + 8) / 0x100000) {
            Func_02003c5a(11, 8, 0);
        }
        v3 = *(s32 *)(Value1(Func_02003c38, 0) + 8) / 0x100000;
        if (v3 < *(s32 *)(Value1(Func_02003c4a, 11) + 8) / 0x100000) {
            Call3(Func_02003c8e, 11, -8, 0);
        }
        Func_02003c9c(11);
        record = Value1(Func_02003c72, 0);
        if (record != 0) {
            Func_02003c94(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02003cba(11);
        Func_02003cbc(0, 0, 24);
        Func_02003c7a(4);
        Func_02003d60(188);
        Func_02003cd2(11, 0, 16);
        Func_02003ce0(0);
        Call3(Func_02003cce, 11, 0x158, 0x168);
        Func_02003cf4(11);
        Func_02003caa(10);
        Func_02003cbe();
    }
}
