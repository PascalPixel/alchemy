#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/facing_events.h"


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

u8 *Talk_Run2();

u8 *Talk_Run3(s32 no);

u8 *Talk_Run4(s32 no);

u8 *Talk_Run5(s32 no);

u8 *Talk_Run6(s32 no);

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

void Talk_Run7(); /* begin step */

void Talk_Run8(); /* present message by id */

s32 Talk_Run9();  /* act on the subject, variant used before the branch */

s32 Talk_Run10();  /* branch predicate; nonzero selects the second arm */

void Talk_Run11(); /* wait / delay by count */

void Talk_Run12(); /* present message, first arm */

void Talk_Run13(); /* present message, second arm */

void Talk_Run14(); /* act on the subject */

void Talk_Run15(); /* end step */

void Talk_Run16(); /* begin step */

void Talk_Run17(); /* present message by id */

s32 Talk_Run18();  /* act on the subject, variant used before the branch */

s32 Talk_Run19();  /* branch predicate; nonzero selects the second arm */

void Talk_Run20(); /* wait / delay by count */

void Talk_Run21(); /* present message, first arm */

void Talk_Run22(); /* present message, second arm */

void Talk_Run23(); /* act on the subject */

void Talk_Run24(); /* end step */

void Talk_Run25(); /* begin step */

void Talk_Run26(); /* present message by id */

s32 Talk_Run27();  /* act on the subject, variant used before the branch */

s32 Talk_Run28();  /* branch predicate; nonzero selects the second arm */

void Talk_Run29(); /* wait / delay by count */

void Talk_Run30(); /* present message, first arm */

void Talk_Run31(); /* present message, second arm */

void Talk_Run32(); /* act on the subject */

void Talk_Run33(); /* end step */

u8 *SceneData_GetTable8BB4(void)
{
    return (u8 *)0x02008bb4;
}

s32 Talk_Run34(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return (u8 *)0x02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void)
{
    if (Talk_Check7(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (Talk_Check8(0x950) != 0) {
        return gOv;
    }
    if (Talk_Check9(0x962) != 0) {
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
    Talk_Run35();
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
    Talk_Run36(158);
    Talk_Place(gOv4[n].a, gOv4[n].b, gOv4[n].c);
    Talk_Place2(0, 0x8000, 0x4000);
    *(u8 *)(Talk_Check10(0) + 85) = 0;
    Talk_Run37(0, 2);
    Talk_Run38(*(s16 *)p6);
}

void Scene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Talk_Run39();
    Talk_Do(0x2394);
    Talk_Run40(40);
    rec7 = Talk_Check2(0x11c, 0x2580000, 0, 0x3380000);
    Talk_Run41(rec7, 0);
    Talk_Run42(rec7, 6);
    Talk_Run43(10);
    Talk_Run44(rec7, 1);
    Talk_Run45(40);
    Talk_Run46(rec7);
    Talk_Run47(2);
    Talk_Place3(25, 0x100, 50);
    Talk_Place4(25, 0x10000, 0x8000);
    Talk_Place5(25, 0x258, 0x350);
    Talk_Place6(25, 0xc000, 0);
    Talk_Run48(40);
    Talk_Run49(25, 0);
    Talk_Run50(25, 2);
    Talk_Run51(30);
    Talk_Place7(25, 0x238, 0x350);
    Talk_Place8(25, 0xc000, 0);
    Talk_Run52(30);
    Talk_Place9(25, 0x108, 50);
    Talk_Run53(20);
    Talk_Place10(0, 0, -16);
    Talk_Run54(20);
    Talk_Place11(25, 0x3000, 0);
    Talk_Run55(30);
    Talk_Run56(25, 2);
    Talk_Run57(20);
    Talk_Run58(25, 0);
    Talk_Run59(20);
    Talk_Place12(0, 0x101, 50);
    Talk_Run60(20);
    Talk_Run61(25, 4);
    Talk_Run62(20);
    Talk_Run63(25, 0);
    Talk_Run64(30);
    Talk_Place13(25, 0x102, 50);
    Talk_Run65(25, 0);
    Talk_Place14(25, 0x16666, 0xb333);
    Talk_Run66(25, 16, 0);
    Talk_Run67(25, 0, 32);
    Talk_Run68(20);
    Talk_Run69(25, 3);
    Talk_Run70(20);
    Talk_Run71(25, 0);
    Talk_Run72(0, 16, 0);
    Talk_Place15(0, 0x8000, 0);
    Talk_Run73(20);
    Talk_Place16(25, 0x1cccc, 0xe666);
    Talk_Run74(25, 0, 48);
    Talk_Run75(25, 0, 0);
    Talk_Run76();
}

void Scene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Talk_Run77(s32);
    void Talk_Run78(s32);

    Talk_Run79();
    Talk_Run78(0x23A4);
    Talk_Do6(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Talk_Place18(31, 4, 13);
    Talk_Place19(31, 4, 30);
    Talk_Apply(31, 0);
    Talk_Do7(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Talk_Place20(32, 0x102, 50);
    Talk_Run77(10);
    Talk_Apply2(32, 3);
    Talk_Do8(30);
    Talk_Apply3(32, 0);
    Talk_Do9(10);
    Talk_Apply4(33, 4);
    Talk_Do10(20);
    Talk_Apply5(33, 0);
    Talk_Do11(10);
    Talk_Apply6(31, 2);
    Talk_Do12(20);
    Talk_Apply7(31, 0);
    Talk_Do13(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Talk_Apply8(32, 3);
    Talk_Do14(30);
    Talk_Run80();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    extern u8 gWork[];

    u8 *record;
    s32 none;

    *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x209;
    if (SceneInit_Value1(Talk_Check11, 0x950) != 0) {
        SceneInit_Call6(Talk_Run81, 51, 47, 3, 1, 51, 45);
        record = Talk_Run(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Talk_Run2(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (SceneInit_Value1(Talk_Check12, 0x8bc) != 0) {
            SceneInit_Call3(Talk_Run82, 25, 0x2300000, 0x2a80000);
            SceneInit_Call3(Talk_Run83, 25, 0x8000, 0);
        }
        if (gCell2[225][0] == 19) {
            if (SceneInit_Value1(Talk_Check13, 0x8bc) == 0) {
                SceneInit_Call1(Talk_Run84, 0x8bc);
                Talk_Run85();
                Talk_Run86();
            }
        }
        if (gCell2[225][0] == 16) {
            if (SceneInit_Value1(Talk_Check14, 0x300) == 0) {
                SceneInit_Call1(Talk_Run87, 0x300);
                Talk_Run88();
                Talk_Run89();
            }
        }
        if (SceneInit_Value1(Talk_Check15, 0x8ab) != 0) {
            Talk_Run90(35, 0, 0);
            Talk_Run91(36, 0, 0);
        }
    }
    return 0;
}

void Dialogue_RunFacingPrompt(s32 no)
{
    u8 *actor = Talk_Run3(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Talk_Apply9(28, no);
    } else if (Talk_Check16(0x950) != 0) {
        Scene_Call1(Talk_Do15, 0x238d);
        Talk_Apply10(no, 0);
    } else if (Talk_Check17(0x962) != 0) {
        Scene_Call1(Talk_Do15, 0x221b);
        Talk_Apply10(no, 0);
    } else {
        msg = (s32)gVal;
        Talk_Do16(msg);
        Scene_Value2(Talk_Apply11, no, 0);
        if (Scene_Value2(Talk_Apply12, 0, 0) == 0) {
            Talk_Do17(10);
            Talk_Do18(msg + 1);
        } else {
            Talk_Do19(msg + 2);
        }
        Talk_Apply13(no, 0);
    }
}

void Dialogue_RunFacingActionPrompt(s32 no)
{
    u8 *actor = Talk_Run4(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_Apply14(26, no);
    } else if (Talk_Check18(0x950) != 0) {
        msg = (s32)gVal2;
        Talk_Do20(msg);
        Scene_Value2(Talk_Apply15, no, 0);
        if (Scene_Value2(Talk_Apply16, 0, 0) == 0) {
            Talk_Do21(10);
            Talk_Do22(msg + 1);
        } else {
            Talk_Do23(msg + 2);
        }
        Talk_Apply17(no, 0);
    } else if (Talk_Check19(0x962) != 0) {
        Scene_Call1(Talk_Do22, 0x2219);
        Talk_Apply17(no, 0);
    } else {
        Scene_Call1(Talk_Do24, 0x1fd2);
        Talk_Apply18(no, 0);
        Scene_Call3(Talk_Place21, no, 0x106, 0);
        Talk_Do25(40);
        Talk_Apply19(no, 0);
    }
}

void Dialogue_RunFacingAction(s32 no)
{
    u8 *actor = Talk_Run5(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_Apply20(27, no);
    } else {
        if (Talk_Check20(0x950) != 0) {
            Scene_Call1(Talk_Do26, 0x238f);
            Talk_Apply21(no, 0);
        } else if (Talk_Check21(0x962) != 0) {
            Scene_Call1(Talk_Do26, 0x221d);
            Talk_Apply21(no, 0);
        } else {
            Talk_Do27(0x1fd9);
            Talk_Apply22(no, 0);
        }
    }
}

void Dialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Talk_Run7();

    message = (s32)&Value_0000239e;
    Talk_Run8(message);
    Talk_Run9(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_Run10(0, 0) == 0) {
        Talk_Run11(10);
        Talk_Run12(message + 1);
    } else {
        Talk_Run13(message + 2);
    }

    Talk_Run14(subject, 0);
    Talk_Run15();
}

void Dialogue_RunActorLine23a1(s32 no)
{
    Talk_Do28(no);
    Talk_Do29(0x23A1);
    Talk_Apply23(no, 0);
    Talk_Run92();
}

void Dialogue_RunMessage1FBBStep(s32 subject)
{
    s32 msg;

    Talk_Run16();

    msg = (s32)&Value_00001fbb;
    Talk_Run17(msg);
    Talk_Run18(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_Run19(0, 0) == 0) {
        Talk_Run20(10);
        Talk_Run21(msg + 1);
    } else {
        Talk_Run22(msg + 2);
    }

    Talk_Run23(subject, 0);
    Talk_Run24();
}

void Scene_RunScene3b6(s32 a0)
{
    void Talk_Run93();

    u32 i;
    s32 record;
    s32 base5_2399;

    Talk_Run94();
    if (Talk_Check3(0x8bd) == 0) {
        base5_2399 = (s32)gVal3;
        Talk_Run95(base5_2399);
        Talk_Check4(a0, 0);
        if (Talk_Check5(0, 0) == 0) {
            Talk_Run96(10);
            Talk_Run97((base5_2399 + 1));
        } else {
            Talk_Run98((base5_2399 + 2));
        }
        Talk_Run93(a0, 0);
    } else {
        if (Talk_Check6(0x8be) == 0) {
            Talk_Do2(0x8be);
            Talk_Do3(0x239c);
            Talk_Run99(a0, 0);
            Talk_Run100(10);
            Talk_Run101(a0, 2);
            Talk_Run102(20);
        }
        Talk_Do4(0x239d);
        Talk_Run103(a0, 0);
    }
    Talk_Run104();
}

void Dialogue_RunActor25FlaggedLine(void)
{
    void Talk_Run105(void);

    Talk_Run105();
    if (Talk_Check22(0x8BE) == 0) {
        Talk_Do30(0x23B3);
    } else {
        Talk_Do31(0x23B4);
    }
    Talk_Apply24(25, 0);
    Talk_Run106();
}

void Scene_RunScene3b6(s32 a0)
{
    u32 i;
    s32 record;

    Talk_Run107();
    Talk_Do5(0x23a8);
    Talk_Place17(31, 0x103, 40);
    Talk_Run108(a0, 0);
    Talk_Run109();
}

void Dialogue_RunMessage23acStep(s32 subject)
{
    s32 message;

    Talk_Run25();

    message = (s32)&Value_000023ac;
    Talk_Run26(message);
    Talk_Run27(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Talk_Run28(0, 0) == 0) {
        Talk_Run29(10);
        Talk_Run30(message + 1);
    } else {
        Talk_Run31(message + 2);
    }

    Talk_Run32(subject, 0);
    Talk_Run33();
}

void Dialogue_RunFacingMessage(s32 no)
{
    s32 Talk_Run110(s32 flag);

    u8 *actor = Talk_Run6(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Talk_Do32(no);
    } else {
        if (Talk_Run110(0x950) != 0) {
            Scene_Call1(Talk_Do33, 0x23bf);
            Talk_Apply25(no, 0);
        } else if (Talk_Check23(0x962) != 0) {
            Scene_Call1(Talk_Do33, 0x2231);
            Talk_Apply25(no, 0);
        } else {
            Talk_Do34(0x1feb);
            Talk_Apply26(no, 0);
        }
    }
}
