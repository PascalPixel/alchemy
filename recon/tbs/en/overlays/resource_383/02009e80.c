/* Draft of FieldScene_ConfigurePairedActors, resource_383 at 0x02009e80 (split from FIELD/KUUPUAPPU_HEYA/PROMPT.C).
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Func_02006498,
 * Func_020064bc, Func_020064d2, Func_02006ca4, Func_02006c92, Func_02006c9e). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuTheyreBack[];

enum {
    /* Message 0x182 + 189. */
    ITEM_WATER_OF_LIFE = 189,
    /* Message 0x182 + 231. */
    ITEM_BONE = 231
};


#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define Audio_PlayCue_1_020019a4(a0) Value1(Engine_AudioPlayCue, a0)
#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define SceneWork_SetStepValue_1_020019e4(a0) Value1(Engine_EventSetMessage, a0)
#define Audio_PlayCue_1_020019e4(a0) Value1(Engine_AudioPlayCue, a0)
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define Scene_GetRecord_1_02001a4c(args...) Func_020067d0(args)
#define Scene_GetRecord_2_02001a4c(args...) Func_020067d8(args)
#define SceneWork_SetStepValue_1_02001a4c(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1(a0) Call1(Func_020068d0, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c() Value0(Engine_EventEnd)
#define RATIO_HI 52428
#define RATIO_LO 26214
#define Scene_GetRecord_1_02001ba0(a0) Value1(Func_02006924, a0)
#define Scene_GetRecord_2_02001ba0(a0) Value1(Func_0200692c, a0)
#define ObjectMotion_EnableActionAndSetCallback_1_02001e80(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1_02001ba0(args...) Func_02006ca4(args)
#define Scene_GetRecord_1_02001e80(args...) Func_02006c92(args)
#define Scene_GetRecord_2_02001e80(a0) Value1(Func_02006c9e, a0)
#define ACTOR_SHOWN_OFFSET 100
#define Scene_GetRecord_1_02002fd4(a0) Value1(Func_02007d56, a0)
#define Scene_GetRecord_2_02002fd4(args...) Func_02007d8c(args)
#define Scene_GetRecord_3(args...) Func_02007d98(args)
#define Scene_GetRecord_4(args...) Func_02007da4(args)
#define Scene_GetRecord_5(args...) Func_02007dc8_a(args)

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern u8 Data_0200cf2c[];
extern u8 MsgNoEffect;
extern s32 Data_0200e4a8[];
extern s32 Data_0200e4c0[];
extern u8 Data_0200d17c[];
extern u8 Data_0200d354[];
extern u8 Data_0200d4c8[];
extern u16 Data_0200e4f8;

void Func_02004dbc(u8 *);
u8 *Func_02004e04(s32);
u8 *Func_02004e14(s32);
u8 *Func_02004e3e(s32);
u8 *Func_02004e4e(s32);
struct FieldActor *Func_02004e84(s32);
u8 *Func_02004ebe(s32);
u8 *Func_02004f1e(s32);
u8 *Func_02004f7e(s32);
s32 Func_020050fe(void);
s32 Func_02004f98(void);
void Func_0200513c(void);
u8 *Func_02007472(s32 actor);
u8 *Func_0200749e(s32 actor);
u8 *Func_020074a6(s32 actor);
void Func_0200957e(void);
void Func_02004e3e_a();
void Func_02004e50();
void Func_02004e90();
void Func_02004ea8();
void Func_02004ea8_a();
s32 Func_020055d6();
s32 Func_020056b8();
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005232();
void Func_0200525c();
void Func_0200526c();
void Func_02005290();
void Func_020052c6();
void Func_020052d0();
void Func_020052fe();
void Func_02005346();
void Func_02005cca();
u8 *Func_02005b1c();
void Func_020067dc();
void Func_020068d0();
void *Func_020067d0();
u8 *Func_020051d6(s32);
struct FieldActor *Func_0200533e(s32);
void Func_02005418(s32);
struct FieldActor *Func_02007926();
struct FieldActor *Func_0200792e();
struct FieldActor *Func_02007936();
s32 Func_02007858();
s32 Func_0200786c();
void Func_02006290();
void Func_020062bc();
void Func_020062ec();
void Func_020062f0();
void Func_020062fe();
void Func_0200631e();
void Func_02006348();
void Func_0200634e();
void Func_02006352();
void Func_02006378();
void Func_02006388();
void Func_020063b8();
void Func_020063fa();
void Func_02006838();
s32 Func_02006924();
s32 Func_0200692c();
void Func_02006498();
void Func_020064bc();
void Func_020064d2();
u8 *Func_02006c92();
s32 Func_02006c9e();
void Func_02006ca4();
void Func_0200731c();
void Func_02007324();
void Func_02007378();
void Func_0200738a();
void Func_0200739a();
void Func_020073b8();
s32 Func_02007a1a();
s32 Func_02007a26();
s32 Func_02007a30_a();
s32 Func_02007a3a();
s32 Func_02007a46();
s32 Func_02007a52();
void Func_020076ba();
void Func_0200770c();
void Func_02007726();
void Func_020077a2();
void Func_020077e2();
void Func_020077e6();
void Func_020077f6();
void Func_02007824();
void Func_02007848();
void Func_02007860();
void Func_02007870();
void Func_02007870_a();
void Func_02007890();
void Func_020078b2();
void Func_020078b6();
void Func_020078bc();
void Func_020078d2();
void Func_020078d8();
void Func_020078ec();
void Func_020078f6();
void Func_02007912();
void Func_02007936_a();
void Func_02007950();
void Func_02007960();
void Func_02007980();
void Func_02007984();
void Func_020079ae();
void Func_020079b8();
void Func_020079fe();
void Func_02007a2a();
void Func_02007a2a_a();
void Func_02007a56_a();
void Func_02007aa6();
void Func_02007ab0();
void Func_02007abc();
void Func_02007abe();
void Func_02007b18();
void Func_02007b78();
void Func_02007b90();
void Func_02007b96();
void Func_02007b98_a();
void Func_02007ba0();
void Func_02007bc0();
void Func_02007bd0();
void Func_02007bda();
void Func_02007bf0();
void Func_02007c00();
void Func_02007c04();
void Func_02007c12();
void Func_02007c26();
void Func_02007c3c();
void Func_02007c4a();
void Func_02007c66();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c7c();
void Func_02007c8e();
void Func_02007cb6();
void Func_02007cbc();
void Func_02007cf0();
s32 Func_02007d56();
s32 Func_02007d8c();
s32 Func_02007d98();
s32 Func_02007da4();
s32 Func_02007dc8_a();
void Func_0200822c();
void Func_02007ea2();
void Func_02007ec2();
void Func_02007f02();
void Func_02007f1c();
void Func_02007fbe();
void Func_02007fc4_a();
void Func_02007fdc();
void Func_02007ffe();
void Func_02008010_a();
void Func_02008016();
void Func_02008020();
void Func_02008026();
void Func_02008044_a();
void Func_02008070_a();
void Func_02008084();
void Func_020080e4();
void Func_020080ee_a();
void Func_02008128();
void Func_020081b8();
void Func_020081de();
void Func_020081e4();
void Func_020081f2();
void Func_0200820a();
void Func_0200821e_a();
void Func_020082b6_b();
void Func_020082f0_a();
void Func_02008300_a();
void Func_02008326();
void Func_0200833a();
void Func_0200834a();
void Func_02008378();
void Func_02008382();
void Func_020083aa_a();
void Func_020083b0();
void Func_020083bc();
void Func_020083d6();
void Func_020083ea();
void Func_020083fa_a();
void Func_02008418();
void Func_02008422();
void Func_02008430();
void Func_0200843a();
void Func_02008442();
void Func_0200847c();
u8 *Func_02008492();
void Func_02008494();
void Func_020084b6();
void Func_020084bc();
void Func_020084e0();
void Func_020084ea();
void Func_020084f0();
void Func_02008516();
u8 *Func_02008526();
void Func_0200852a();
void Func_0200852c_a();
u8 *Func_02008532();
u8 *Func_0200853e();
void Func_02008562();
void Func_02008592();
void Func_020085a2_a();
void Func_020085c2();
void Func_020085d2_a();
void Func_020085e6();
void Func_020085ea();
void Func_020085fc();
void Func_0200860e_a();
void Func_02008614();
void Func_02008620();
void Func_02008648();
void Func_0200864e();
void Func_020086fe();
void Func_02008734();
void Func_02008738();
void Func_0200876e();
void Func_0200879c();
void Func_020087a6();
void Func_020087a8();
void Func_020087ea();
void Func_020087f6_a();
void Func_020087f8();
void Func_020087f8_a();
void Func_0200880c_a();
void Func_02008814_a();
void Func_02008830();
void Func_0200885c();
void Func_02008866();
void Func_02008870_a();
void Func_0200888c();
void Func_0200889c();
void Func_020088bc();
void Func_020088cc();
void Func_020088ce();
void Func_020088fc();
void Func_020088fe();
void Func_0200890e();
void Func_02008910();
void Func_02008950();
void Func_02008978();
s32 Func_020089a6();
void Func_02008a30();
void Func_02008a66_a();
void Func_02008a68_a();
void Func_02008aee();
void Func_02008afc();
void Func_02008b16();
void Func_02008b4e();
s32 Func_02008e26();
void Func_02008e2e();
void Func_0200920a();
s32 Func_02009278();
s32 Func_0200929c();
void Func_020098a8();
void Func_02009a54();
u8 *Func_02007c8e_a(s32);
void Func_02007eb8(void);
u8 *Func_02007ca6(s32);
void Func_02007ce6(s32, s32, s32, s32);
void Func_02007cd2(s32);
s32 Func_02007cd8(s32);
u8 *Func_02009356(s32);
u8 *Func_02009370(s32, s32);
void Func_02009418_a(s32, s32, s32);
void Func_0200942e(s32);
s32 Func_02009062(s32, s32, s32);
s32 Func_02009030_a(s32, s32, s32);
s32 Func_0200903e_a(s32, s32, s32);
s32 Func_0200904c(s32, s32, s32);
void Func_0200958c(s32, s32, s32, s32);

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

/* Word at +456 of the shared scene work record. */

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_02001a4c(s32 (*f)())
{
    void *Func_02005ae2();
    void *Func_020067d8();

    return f();
}

static __inline__ void Call1_02001a4c(void (*f)(), s32 a0)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0, a1, a2, a3, a4, a5);
}

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

static __inline__ void Call6_02001ba0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Configures actor records 24 and 25 (their +100 "shown" half words end up
 * set to 1 and 3 respectively) and finishes with a six-argument call that
 * repeats actor id 14. */
void FieldScene_ConfigurePairedActors(void)
{
    u32 i;
    u8 *record;

    Event_Wait(30);
    Actor_RunRepeatedMotion(24, 1);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKuupuappuTheyreBack);
    Func_02006498(24, 20);
    Actor_FaceDirection(25, 0, 20);
    Actor_SetAttachedEffect(25, 0x102);
    Actor_RunRepeatedMotion(25, 2);
    Func_020064bc(25, 20);
    Actor_SetAnimationAndWait(24, 4);
    Event_Wait(20);
    Func_020064d2(24, 20);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    Actor_SetSpeed(25, 0x38000, 0x1c000);
    ObjectMotion_EnableActionAndSetCallback_1_02001e80(25, 0x200d830);
    Actor_EnableActionCallback(24, 0x200d560);
    Object_LookupAndStep_1_02001ba0(24);
    {
        u8 *record = Scene_GetRecord_1_02001e80(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = Scene_GetRecord_2_02001e80(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}
