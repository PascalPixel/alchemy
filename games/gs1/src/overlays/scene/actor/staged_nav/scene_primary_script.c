#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_0200426e(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020042ea, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02004302, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_0200430e(args)
#define UiText_DrawMessage_1(a0, a1) Call2(Func_0200426e_a, a0, a1)
#define GameFlag_IsSet_1(a0) Value1(Func_0200427e, a0)
#define Audio_PlayCue_1(args...) Func_02004352(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020042b2(args)
#define Audio_PlayCue_2(args...) Func_02004368(args)
#define ObjectMotion_SetActionVariant_1(args...) Func_02004330(args)
#define Scene_GetRecord_1(args...) Func_020042de(args)
#define Scene_GetRecord_2(args...) Func_020042e8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200432a, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200433a(args)
#define GameFlag_Set_1(a0) Call1(Func_0200434c, a0)
#define Audio_PlayCue_3(args...) Func_02004414(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02004374(args)
#define Audio_PlayCue_4(args...) Func_0200442a(args)
#define Scene_GetRecord_3(args...) Func_02004398(args)
#define Scene_GetRecord_4(args...) Func_020043a4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020043e6, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_020043f6(args)
#define GameFlag_Clear_1(a0) Call1(Func_02004406, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02004422(args)
#define REC_ID 16
#define QUERY_FLAG 0x200
#define FieldScene_RunPrimarySequence Func_020011c4
#define FieldScene_RunScene3b3SequenceA Func_020015fc
#define FieldScene_RunFlaggedDisplayScene Func_0200175c
#define FieldScene_RunScene3b3_02001fd4 Func_02001fd4
#define FieldScene_RunScene3b3_020020f0 Func_020020f0
#define FieldScene_RunScene3b3_0200215c Func_0200215c
#define FieldScene_RunScene3b3_0200263c Func_0200263c

extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_02000240_t[][2];
extern u8 Data_0200b738[];

void Func_02001386();
s32 Func_02001630();
s32 Func_02001662();
s32 Func_02003c3e();
s32 Func_02003c52();
s32 Func_02003c5c();
s32 Func_02003c66();
s32 Func_02003cee();
void Func_02003d20();
void Func_02003d5e();
void Func_02003d68();
void Func_02003d92();
u8 *Func_02003d94();
s32 Func_02003da0();
void Func_02003da6();
void Func_02003de2();
s32 Func_02003dea();
s32 Func_02003e08();
s32 Func_02003e14();
s32 Func_02003e22();
void Func_02003e32();
void Func_02003e3c();
void Func_02003e5a();
void Func_02003ea4();
void Func_02003eaa();
void Func_02003eee();
void Func_020022e0();
void Func_020022f4();
void Func_02002308();
void Func_02002322();
void Func_02002336();
void Func_0200234a();
void Func_020040d4();
s32 Func_020040d6();
void Func_020040ec();
s32 Func_020040f2();
void Func_02004106();
void Func_02004154();
void Func_02004164();
void Func_02004174();
void Func_0200417a();
void Func_02004186();
void Func_0200418a();
void Func_0200419a();
void Func_020041ae();
s32 Func_020041bc();
void Func_020041be();
void Func_020041be_a();
void Func_02004264();
void Func_0200427c();
void Func_020026e0();
void Func_020026e6();
void Func_020026ec();
void Func_020026f2();
void Func_020026f8();
void Func_020026fe();
void Func_02002704();
void Func_02002822();
void Func_02002828();
void Func_0200282e();
void Func_02002834();
void Func_0200283a();
void Func_02002840();
void Func_02002846();
void Func_02002e0e();
void Func_02002ec0();
void Func_02004244();
void Func_0200426e();
void Func_0200426e_a();
s32 Func_0200427e();
void Func_0200428c();
void Func_02004296();
void Func_020042b2();
void Func_020042b2_a();
s32 Func_020042de();
u8 *Func_020042e8();
void Func_020042ea();
void Func_02004302();
void Func_02004306();
void Func_0200430e();
void Func_02004314();
void Func_0200432a();
void Func_02004330();
void Func_0200433a();
void Func_0200434c();
void Func_02004352();
void Func_0200435a();
void Func_0200435c();
void Func_02004368();
void Func_02004374();
s32 Func_02004398();
u8 *Func_020043a4();
void Func_020043c6();
void Func_020043e6();
void Func_020043f6();
void Func_02004406();
void Func_02004414();
void Func_02004422();
void Func_0200442a();
void Func_020023ac();
s32 Func_02003244();
void Func_02003d8c();
void Func_02004ae4();
s32 Func_02004b02();
s32 Func_02004b14();
s32 Func_02004b2a();
u8 *Func_02004b3a();
void Func_02004b3e();
void Func_02004bde();
void Func_02004c0a();
s32 Func_02004c14();
void Func_02004c16();
void Func_02004c42();
s32 Func_02004c56();
s32 Func_02004c7e();
void Func_02004c80();
s32 Func_02004c86();
void Func_02004c98();
void Func_020050be();
s32 Func_0200513a();
s32 Func_0200513a_a();
s32 Func_02005160();
void Func_020051aa();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    s32 Func_02001268();

    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001268();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02001268();

    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    s32 Func_02001268();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200263c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_RunPrimarySequence(void)
{
    s32 Func_02001268();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Func_02003cee(0);
    flag = *(volatile s32 *)0x03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Func_02003c3e();
        p6 = *(volatile s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Func_02003c52();
        p5 = *(volatile s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Func_02003c66();
        record = Func_02003c5c((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Call8(Func_02001386, p6, *(volatile s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 Func_02001268(void)
{
    s32 Func_02001268();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Func_02003d94(0);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(volatile u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (Data_02000240_t[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d20, 0x100000, mode, (s32)p);
    if (Value2(Func_02003da0, (s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Value2(Func_02001630, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d5e, 0x200000, mode, (s32)p);
    if (Value2(Func_02001662, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Value2(Func_02003dea, (s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Func_02003e32();
    Func_02003d92((s32)rec, 6);
    Func_02003d68(6);
    Func_02003eee(152);
    Func_02003da6((s32)rec, 7);
    *(volatile s32 *)(rec + 48) = 0x30000;
    *(volatile s32 *)(rec + 52) = 0x20000;
    *(volatile s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Func_02003e3c((s32)rec, 0);
    Func_02003eaa(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Func_02003de2((s32)rec, 6);
    Func_02003e5a((s32)rec, 1);
    *pflag = saved;
    Func_02003ea4();
    return 1;
reject:
    return 0;
}

void Func_020013b0(s32 a0)
{
    s32 Func_02001268();

    s32 v68;
    s32 v72;
    s32 v76;
    s32 step;
    s32 t;

    v68 = *(s32 *)(a0 + 68);
    t = *(s32 *)(a0 + 8);
    v72 = *(s32 *)(a0 + 72);
    *(s32 *)(a0 + 8) = t + v68;
    t = *(s32 *)(a0 + 12);
    v76 = *(s32 *)(a0 + 76);
    *(s32 *)(a0 + 12) = t + v72;
    *(s32 *)(a0 + 16) += v76;
    step = Value2(Func_02003e08, v68, 10);
    *(s32 *)(a0 + 68) = v68 - step;
    step = Func_02003e14(v72, 3);
    *(s32 *)(a0 + 72) = v72 - step;
    step = Func_02003e22(v76, 10);
    *(volatile s32 *)(a0 + 76) = v76 - step;
    *(volatile s32 *)(a0 + 24) += *(volatile s32 *)(a0 + 48);
    *(volatile s32 *)(a0 + 28) += *(s32 *)(a0 + 52);
    *(u16 *)(*(volatile s32 *)(a0 + 80) + 30) += *(u16 *)(a0 + 100);
}

void FieldScene_RunScene3b3SequenceA(void)
{
    s32 record;
    s32 count;

    record = Value1(Func_020040f2, 0x200);
    if (record == 0) {
        Func_020022e0(10, 19, 16, 5, record, 10, 31);
        Func_020022f4(10, 51, 16, 5, 1, 10, 31);
        Func_02002308(42, 51, 16, 5, 2, 10, 31);
    } else {
        Func_02002322(10, 19, 16, 5, 0, 10, 31);
        Func_02002336(10, 83, 16, 5, 1, 10, 31);
        Func_0200234a(42, 83, 16, 5, 2, 10, 31);
    }
    *(volatile s32 *)Data_0200b738 = 0;
    Func_020040d6(0x20095cd, 0xc80);
    Func_020040d4(1);
    Func_02004106(1, 0, 0x2009579);
    Func_02004264(231);
    *(volatile s32 *)Data_0200b738 = 0;
    do {
        Func_020040ec(1);
        count = *(volatile s32 *)Data_0200b738;
        count = count + 1;
        *(volatile s32 *)Data_0200b738 = count;
    } while (count <= 100);
    Call1(Func_0200427c, 0x121);
    if (Value1(Func_020041bc, 0x200) == 0) {
        Call6(Func_0200418a, 0, 32, 32, 0, 32, 32);
        Call6(Func_0200419a, 32, 32, 64, 0, 32, 32);
    } else {
        Call6(Func_020041ae, 0, 64, 32, 0, 32, 32);
        Call6(Func_020041be, 32, 64, 64, 0, 32, 32);
    }
    Func_02004154(1);
    Func_02004186(1, 0, 0);
    Func_02004164(1);
    Call1(Func_0200417a, 0x20095cd);
    Func_020041be_a();
    Func_02004174(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x1190000, -1, 0x1b00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    UiText_DrawMessage_1(0x1528, 1);
    queried = GameFlag_IsSet_1(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue_1(232);
        Call3(Func_02004244, 0x200ada8, 84, 24);
        BattleRuntime_WaitIfModeZero_1(30);
        Audio_PlayCue_2(240);
        ObjectMotion_SetActionVariant_1(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Scene_GetRecord_2(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        ObjectMotion_SetHorizontalPositionWithTerrain_1(REC_ID, 0x1100000, 0x1a00000);
        Object_SetModeById_1(REC_ID, 1);
        Call3(Func_0200428c, 0x200adfc, 80, 24);
        Call3(Func_02004296, 0x200ae50, 80, 28);
        Call6(Func_020042b2_a, 65, 40, 16, 27, 2, 4);
        Func_02002e0e();
        Func_020026e0(9);
        Func_020026e6(10);
        Func_020026ec(11);
        Func_020026f2(12);
        Func_020026f8(13);
        Func_020026fe(14);
        Func_02002704(15);
        Call6(Func_02004314, 24, 3, 1, 1, 24, 8);
        GameFlag_Set_1(QUERY_FLAG);
    } else {
        Audio_PlayCue_3(232);
        Call3(Func_02004306, 0x200add2, 84, 24);
        BattleRuntime_WaitIfModeZero_2(30);
        Audio_PlayCue_4(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Scene_GetRecord_4(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        ObjectMotion_SetHorizontalPositionWithTerrain_2(REC_ID, 0x1100000, 0x1b40000);
        Object_SetModeById_2(REC_ID, 2);
        Call6(Func_0200435a, 65, 45, 16, 27, 2, 4);
        Call3(Func_0200435c, 0x200ae26, 80, 24);
        Func_02002ec0();
        Func_02002822(9);
        Func_02002828(10);
        Func_0200282e(11);
        Func_02002834(12);
        Func_0200283a(13);
        Func_02002840(14);
        Func_02002846(15);
        Call6(Func_020043c6, 24, 4, 1, 1, 24, 8);
        GameFlag_Clear_1(QUERY_FLAG);
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void FieldScene_RunScene3b3_02001fd4(void)
{
    u32 i;
    u8 *record;

    Func_02004ae4();
    if (Value0(Func_02003244) == 0) {
        *(u8 *)(Func_02004b02(0) + 85) &= 254;
        *(u8 *)(Func_02004b14(0) + 35) &= 254;
        Func_020023ac();
        Func_02003d8c();
        *(u8 *)(Func_02004b2a(0) + 85) |= 1;
        {
            u8 *record = Func_02004b3a(0);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 1);
        }
    }
    Func_02004b3e();
}

void FieldScene_RunScene3b3_020020f0(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value0(Func_02004c14);
    Func_02004c0a();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Call6(Func_02004bde, 20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    Func_02004c16((a0 + 0x1f5));
    Value2(Func_02004c56, a0, 0x200ad64);
    Func_02004c42();
}

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Value1(Func_02004c7e, 0);
    record = Func_02004c86(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Call1(Func_02004c80, 0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Call1(Func_02004c98, 0x203);
    }
}

void FieldScene_RunScene3b3_0200263c(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Func_02005160();
    if (Value1_0200263c(Func_0200513a, (a0 + 0x1f5)) != 0) {
        Func_020050be(rec7, 5);
        *(s32 *)(rec7 + 108) = 0x200a0b9;
        Call6(Func_0200513a_a, 20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Func_020051aa(a0, 0x200ad64);
    }
}
