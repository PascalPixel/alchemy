#include "types.h"
#include "scene.h"

struct Ent { s32 a; u16 b; u16 c; };

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern struct Ent gOv4[];
extern u8 gCell[];
extern s16 gCell2[][1];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 Value_0000239e;
extern u8 Value_00001fbb;
extern u8 gVal3[];
extern u8 Value_000023ac;

u8 *Talk_Run();

u8 *Talk_unk2_4();

u8 *Talk_unk3_4(s32 no);

u8 *Talk_unk4_4(s32 no);

u8 *Talk_unk5_4(s32 no);

u8 *Talk_unk6_4(s32 no);

/* Contiguous unnamed leaf-owner run for resource_3b6. */

/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */

/* Old-style declarations: interfaces vary by call site across this overlay. */

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 gWork[];

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void SceneInit_Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 SceneInit_Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void SceneInit_Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void SceneInit_Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Scene_Call1(void (*f)(s32), s32 a) { f(a); }

static __inline__ s32 Scene_Value2(s32 (*f)(s32, s32), s32 a, s32 b) { return f(a, b); }

static __inline__ void Scene_Call3(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c) { f(a, b, c); }

void Talk_unk7_4(); /* begin step */

void Talk_unk8_4(); /* present message by id */

s32 Talk_unk9_4();  /* act on the subject, variant used before the branch */

s32 Talk_unk10_4();  /* branch predicate; nonzero selects the second arm */

void Talk_unk11_4(); /* wait / delay by count */

void Talk_unk12_4(); /* present message, first arm */

void Talk_unk13_4(); /* present message, second arm */

void Talk_unk14_4(); /* act on the subject */

void Talk_unk15_4(); /* end step */

void Talk_unk16_4(); /* begin step */

void Talk_unk17_4(); /* present message by id */

s32 Talk_unk18_4();  /* act on the subject, variant used before the branch */

s32 Talk_unk19_4();  /* branch predicate; nonzero selects the second arm */

void Talk_unk20_4(); /* wait / delay by count */

void Talk_unk21_4(); /* present message, first arm */

void Talk_unk22_4(); /* present message, second arm */

void Talk_unk23_4(); /* act on the subject */

void Talk_unk24_4(); /* end step */

void Talk_unk25_4(); /* begin step */

void Talk_unk26_4(); /* present message by id */

s32 Talk_unk27_4();  /* act on the subject, variant used before the branch */

s32 Talk_unk28_4();  /* branch predicate; nonzero selects the second arm */

void Talk_unk29_4(); /* wait / delay by count */

void Talk_unk30_4(); /* present message, first arm */

void Talk_unk31_4(); /* present message, second arm */

void Talk_unk32_4(); /* act on the subject */

void Talk_unk33_4(); /* end step */

u8 *SceneData_GetTable8BB4(void)
{
    return (u8 *)0x02008bb4;
}

s32 get_default_result(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return (u8 *)0x02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void)
{
    if (Talk_unk7(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (Talk_unk8(0x950) != 0) {
        return gOv;
    }
    if (Talk_unk9(0x962) != 0) {
        return gOv2;
    }
    return gOv3;
}

void State_SetWork1c0AndRun(void)
{
    extern u8 *gWork;

    u8 *state = gWork;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    Talk_unk35_4();
}

void Scene_RunPrimarySequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)gWork;
    for (i = 8; i < 66; i++) {
        record = Talk_Check(i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Talk_unk36_4(158);
    Talk_Place(gOv4[n].a, gOv4[n].b, gOv4[n].c);
    Talk_unk2_3(0, 0x8000, 0x4000);
    *(u8 *)(Talk_unk10(0) + 85) = 0;
    Talk_unk37_4(0, 2);
    Talk_unk38_4(*(s16 *)p6);
}

void Scene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Talk_unk39_4();
    Talk_Do(0x2394);
    Talk_unk40_4(40);
    rec7 = Talk_unk2(0x11c, 0x2580000, 0, 0x3380000);
    Talk_unk41_4(rec7, 0);
    Talk_unk42_4(rec7, 6);
    Talk_unk43_4(10);
    Talk_unk44_4(rec7, 1);
    Talk_unk45_4(40);
    Talk_unk46_4(rec7);
    Talk_unk47_4(2);
    Talk_unk3_3(25, 0x100, 50);
    Talk_unk4_3(25, 0x10000, 0x8000);
    Talk_unk5_3(25, 0x258, 0x350);
    Talk_unk6_3(25, 0xc000, 0);
    Talk_unk48_4(40);
    Talk_unk49_4(25, 0);
    Talk_unk50_4(25, 2);
    Talk_unk51_4(30);
    Talk_unk7_3(25, 0x238, 0x350);
    Talk_unk8_3(25, 0xc000, 0);
    Talk_unk52_4(30);
    Talk_unk9_3(25, 0x108, 50);
    Talk_unk53_4(20);
    Talk_unk10_3(0, 0, -16);
    Talk_unk54_4(20);
    Talk_unk11_3(25, 0x3000, 0);
    Talk_unk55_4(30);
    Talk_unk56_4(25, 2);
    Talk_unk57_4(20);
    Talk_unk58_4(25, 0);
    Talk_unk59_4(20);
    Talk_unk12_3(0, 0x101, 50);
    Talk_unk60_4(20);
    Talk_unk61_4(25, 4);
    Talk_unk62_4(20);
    Talk_unk63_4(25, 0);
    Talk_unk64_4(30);
    Talk_unk13_3(25, 0x102, 50);
    Talk_unk65_4(25, 0);
    Talk_unk14_3(25, 0x16666, 0xb333);
    Talk_unk66_4(25, 16, 0);
    Talk_unk67_4(25, 0, 32);
    Talk_unk68_4(20);
    Talk_unk69_4(25, 3);
    Talk_unk70_4(20);
    Talk_unk71_4(25, 0);
    Talk_unk72_4(0, 16, 0);
    Talk_unk15_3(0, 0x8000, 0);
    Talk_unk73_4(20);
    Talk_unk16_3(25, 0x1cccc, 0xe666);
    Talk_unk74_4(25, 0, 48);
    Talk_unk75_4(25, 0, 0);
    Talk_unk76_4();
}

void Scene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Talk_unk77_3(s32);
    void Talk_unk78_3(s32);

    Talk_unk79_3();
    Talk_unk78_3(0x23A4);
    Talk_unk6_2(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Talk_unk18_3(31, 4, 13);
    Talk_unk19_3(31, 4, 30);
    Talk_Apply(31, 0);
    Talk_unk7_2(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Talk_unk20_3(32, 0x102, 50);
    Talk_unk77_3(10);
    Talk_Apply2(32, 3);
    Talk_unk8_2(30);
    Talk_Apply3(32, 0);
    Talk_unk9_2(10);
    Talk_Apply4(33, 4);
    Talk_unk10_2(20);
    Talk_Apply5(33, 0);
    Talk_unk11_2(10);
    Talk_Apply6(31, 2);
    Talk_unk12_2(20);
    Talk_Apply7(31, 0);
    Talk_unk13_2(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Talk_Apply8(32, 3);
    Talk_unk14_2(30);
    Talk_unk80_3();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    extern u8 gWork[];

    u8 *record;
    s32 none;

    *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x209;
    if (SceneInit_Value1(Talk_unk11, 0x950) != 0) {
        SceneInit_Call6(Talk_unk81_3, 51, 47, 3, 1, 51, 45);
        record = Talk_Run(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Talk_unk2_4(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (SceneInit_Value1(Talk_unk12, 0x8bc) != 0) {
            SceneInit_Call3(Talk_unk82_3, 25, 0x2300000, 0x2a80000);
            SceneInit_Call3(Talk_unk83_3, 25, 0x8000, 0);
        }
        if (gCell2[225][0] == 19) {
            if (SceneInit_Value1(Talk_unk13, 0x8bc) == 0) {
                SceneInit_Call1(Talk_unk84_3, 0x8bc);
                Talk_unk85_3();
                Talk_unk86_3();
            }
        }
        if (gCell2[225][0] == 16) {
            if (SceneInit_Value1(Talk_unk14, 0x300) == 0) {
                SceneInit_Call1(Talk_unk87_3, 0x300);
                Talk_unk88_3();
                Talk_unk89_3();
            }
        }
        if (SceneInit_Value1(Talk_unk15, 0x8ab) != 0) {
            Talk_unk90_3(35, 0, 0);
            Talk_unk91_3(36, 0, 0);
        }
    }
    return 0;
}

void Dialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Talk_unk3_4(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Talk_Apply9(28, no);
    } else if (Talk_unk16(0x950) != 0) {
        Scene_Call1(Talk_unk15_2, 0x238d);
        Talk_Apply10(no, 0);
    } else if (Talk_unk17(0x962) != 0) {
        Scene_Call1(Talk_unk15_2, 0x221b);
        Talk_Apply10(no, 0);
    } else {
        msg = (s32)gVal;
        Talk_unk16_2(msg);
        Scene_Value2(Talk_Apply11, no, 0);
        if (Scene_Value2(Talk_Apply12, 0, 0) == 0) {
            Talk_unk17_2(10);
            Talk_unk18_2(msg + 1);
        } else {
            Talk_unk19_2(msg + 2);
        }
        Talk_Apply13(no, 0);
    }
}

void Dialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Talk_unk4_4(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_Apply14(26, no);
    } else if (Talk_unk18(0x950) != 0) {
        msg = (s32)gVal2;
        Talk_unk20_2(msg);
        Scene_Value2(Talk_Apply15, no, 0);
        if (Scene_Value2(Talk_Apply16, 0, 0) == 0) {
            Talk_unk21_2(10);
            Talk_unk22_2(msg + 1);
        } else {
            Talk_unk23_2(msg + 2);
        }
        Talk_Apply17(no, 0);
    } else if (Talk_unk19(0x962) != 0) {
        Scene_Call1(Talk_unk22_2, 0x2219);
        Talk_Apply17(no, 0);
    } else {
        Scene_Call1(Talk_unk24_2, 0x1fd2);
        Talk_Apply18(no, 0);
        Scene_Call3(Talk_unk21_3, no, 0x106, 0);
        Talk_unk25_2(40);
        Talk_Apply19(no, 0);
    }
}

void Dialogue_RunFacingAction(s32 no)
{
    u8 *actor = Talk_unk5_4(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_Apply20(27, no);
    } else {
        if (Talk_unk20(0x950) != 0) {
            Scene_Call1(Talk_unk26_2, 0x238f);
            Talk_Apply21(no, 0);
        } else if (Talk_unk21(0x962) != 0) {
            Scene_Call1(Talk_unk26_2, 0x221d);
            Talk_Apply21(no, 0);
        } else {
            Talk_unk27_2(0x1fd9);
            Talk_Apply22(no, 0);
        }
    }
}

void Dialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Talk_unk7_4();

    message = (s32)&Value_0000239e;
    Talk_unk8_4(message);
    Talk_unk9_4(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_unk10_4(0, 0) == 0) {
        Talk_unk11_4(10);
        Talk_unk12_4(message + 1);
    } else {
        Talk_unk13_4(message + 2);
    }

    Talk_unk14_4(subject, 0);
    Talk_unk15_4();
}

void Dialogue_RunActorLine23a1(s32 no)
{
    Talk_unk28_2(no);
    Talk_unk29_2(0x23A1);
    Talk_Apply23(no, 0);
    Talk_unk92_3();
}

void Dialogue_RunMessage1FBBStep(s32 subject)
{
    s32 msg;

    Talk_unk16_4();

    msg = (s32)&Value_00001fbb;
    Talk_unk17_4(msg);
    Talk_unk18_4(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_unk19_4(0, 0) == 0) {
        Talk_unk20_4(10);
        Talk_unk21_4(msg + 1);
    } else {
        Talk_unk22_4(msg + 2);
    }

    Talk_unk23_4(subject, 0);
    Talk_unk24_4();
}

void Scene_RunScene3b6(s32 a0)
{
    void Talk_unk93_3();

    u32 i;
    s32 record;
    s32 base5_2399;

    Talk_unk94_3();
    if (Talk_unk3(0x8bd) == 0) {
        base5_2399 = (s32)gVal3;
        Talk_unk95_3(base5_2399);
        Talk_unk4(a0, 0);
        if (Talk_unk5(0, 0) == 0) {
            Talk_unk96_3(10);
            Talk_unk97_3((base5_2399 + 1));
        } else {
            Talk_unk98_3((base5_2399 + 2));
        }
        Talk_unk93_3(a0, 0);
    } else {
        if (Talk_unk6(0x8be) == 0) {
            Talk_unk2_2(0x8be);
            Talk_unk3_2(0x239c);
            Talk_unk99_3(a0, 0);
            Talk_unk100_3(10);
            Talk_unk101_3(a0, 2);
            Talk_unk102_3(20);
        }
        Talk_unk4_2(0x239d);
        Talk_unk103_3(a0, 0);
    }
    Talk_unk104_3();
}

void Dialogue_RunActor25FlaggedLine(void)
{
    void Talk_unk105_3(void);

    Talk_unk105_3();
    if (Talk_unk22(0x8BE) == 0) {
        Talk_unk30_2(0x23B3);
    } else {
        Talk_unk31_2(0x23B4);
    }
    Talk_Apply24(25, 0);
    Talk_unk106_3();
}

void Scene_RunScene3b6(s32 a0)
{
    u32 i;
    s32 record;

    Talk_unk107_3();
    Talk_unk5_2(0x23a8);
    Talk_unk17_3(31, 0x103, 40);
    Talk_unk108_3(a0, 0);
    Talk_unk109_3();
}

void Dialogue_RunMessage23acStep(s32 subject)
{
    s32 message;

    Talk_unk25_4();

    message = (s32)&Value_000023ac;
    Talk_unk26_4(message);
    Talk_unk27_4(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_unk28_4(0, 0) == 0) {
        Talk_unk29_4(10);
        Talk_unk30_4(message + 1);
    } else {
        Talk_unk31_4(message + 2);
    }

    Talk_unk32_4(subject, 0);
    Talk_unk33_4();
}

void Dialogue_RunFacingMessage(s32 no)
{
    s32 Talk_unk110_3(s32 flag);

    u8 *actor = Talk_unk6_4(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_unk32_2(no);
    } else {
        if (Talk_unk110_3(0x950) != 0) {
            Scene_Call1(Talk_unk33_2, 0x23bf);
            Talk_Apply25(no, 0);
        } else if (Talk_unk23(0x962) != 0) {
            Scene_Call1(Talk_unk33_2, 0x2231);
            Talk_Apply25(no, 0);
        } else {
            Talk_unk34_2(0x1feb);
            Talk_Apply26(no, 0);
        }
    }
}
