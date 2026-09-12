#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_0200107c(args)
#define GameFlag_IsSet_1(a0) Value1(Func_02001062, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001114, a0)
#define BattleEventRuntime_ProcessAction_1(args...) Func_0200113c(args)
#define Object_SetModeById_1(args...) Func_020010f6(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_0200112c, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_0200114e(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02001094, a0)
#define Scene_GetRecord_1(args...) Func_020010e0(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_020011a6(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_0200119e, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(args...) Func_020011b2(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_0200115e, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020011aa, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020011d6(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020010fc(args)
#define Scene_GetRecord_2(args...) Func_0200112a(args)
#define ObjectMotion_PlaceWithinCameraBounds_2(args...) Func_020011e8(args)
#define Object_CommitPositionThenWaitIfModeZero_2(args...) Func_020011f4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001184(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001130(args)
#define SceneData_SelectTableByWord224 Func_02000030
#define SceneData_GetTable9390 Func_02000074
#define SceneData_SelectFlaggedTable Func_0200007c
#define SceneData_GetPrimaryTable Func_0200011c
#define FieldScene_RunActorEightPromptDialogue Func_0200015c
#define SceneDialogue_ShowLine1918 Func_020001b0
#define FieldScene_RunOpeningAuxiliarySequence Func_020001d0
#define SceneDialogue_RunActor17Message1924 Func_02000298
#define SceneDialogue_RunActor9Message1932 Func_020002b8
#define SceneDialogue_RunActor10Message18d9 Func_020002d8
#define SceneDialogue_RunActor14Message18e1 Func_020002f8
#define SceneDialogue_RunActor21Message194a Func_02000318
#define SceneActor_IsSlotZeroAngleInRange Func_02000338
#define FieldScene_RunActorFifteenFlagBranch Func_02000360
#define FieldScene_RunActorTwentyFlagBranch Func_020003d4
#define FieldScene_RunActorTwentyOneFlagBranch Func_02000448
#define FieldScene_RunFacingGatedDialogue18 Func_020004bc
#define FieldScene_RunEarlySequence Func_02000524
#define FieldScene_RunScene3a3SequenceB Func_020006a4
#define SceneMotion_UpdateTimedActor Func_020006f0
#define FieldScene_RunScene3a3SequenceC Func_020007b8
#define SceneState_SetFlag906ByActorNineteenX Func_0200084c
#define SceneState_SyncProgressFlagsAndDispatch Func_02000874
#define FieldScene_RunMiddleSequence Func_02000904
#define FieldScene_RunScene3a3SequenceD Func_02000b2c
#define SceneActor_ResetStateAndSpan Func_02000c0c
#define SceneEffect_UpdateDriftingParticle Func_02000c44
#define SceneEffect_SpawnDriftingParticle Func_02000d08
#define FieldScene_RunScene3a3_02000d58 Func_02000d58

struct Slot02000338 {
    u8 head[6];
    u16 angle;
};

struct SceneMotion {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[8];
    s32 velocity;
    u8 unk_2c[47];
    u8 state;
    u8 unk_5c[8];
    s16 timer;
    s16 delay;
    s32 active;
    void (*callback)(struct SceneMotion *);
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
};

struct Actor02000c0c {
    u8 head[18];
    u8 gap18[6];
    s32 span;
    s32 reach;
    u8 gap32[48];
    u8 *attached;
    u8 gap84;
    u8 state;
};

extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 Data_02009120[];
extern u8 Data_02009288[];
extern u8 Data_020090f0[];
extern u8 Data_0200940c[], Data_020095bc[], Data_020093f4[];
extern u8 Data_02009730[];
extern u8 Data_020099f4[];
extern u8 Data_02009724[];
extern u8 *Data_03001ebc;   /* pointer cell holding the overlay work */
extern u8 Data_02009e70[];
extern u32 Data_03001e40;
extern s16 Data_02000240_t[][1];

s32 Func_02000f1c(s32);
s32 Func_02000f3e(s32);
s32 Func_02000f50(s32);
s32 Func_02000f5a(s32);
void Func_02000f9e(void *);
s32 Func_020010a6();
void Func_020010c2();
void Func_020010ec();
void Func_0200105c(void);
void Func_020010ea(s32);
s32 Func_02001112(s32, s32);
void Func_02001076(void);
s32 Func_02001062();
void Func_0200107c();
s32 Func_02001094();
u8 *Func_020010e0();
void Func_020010f6();
void Func_020010fc();
void Func_02001114();
u8 *Func_0200112a();
void Func_0200112c();
void Func_02001130();
void Func_0200113c();
void Func_0200114e();
s32 Func_020011a6();
void Func_020011aa();
void Func_020011b2();
void Func_020011d6();
void Func_020011e8();
void Func_020011f4();
void Func_02001144(void);
void Func_020011d2(s32);
s32 Func_020011fa(s32, s32);
void Func_02001164(void);
void Func_020011f2(s32);
s32 Func_0200121a(s32, s32);
void Func_0200117e(void);
void Func_02001212(s32);
s32 Func_0200123a(s32, s32);
void Func_020011a4(void);
void Func_02001232(s32);
s32 Func_0200125a(s32, s32);
void Func_020011be(void);
void Func_020011c4(void);
void Func_02001252(s32);
s32 Func_0200127a(s32, s32);
void Func_020011de(void);
struct Slot02000338 *Func_02001206(s32);
s32 Func_020012cc();
s32 Func_020006be(void);
void Func_02001240();
void Func_020012ce();
s32 Func_0200122c();
void Func_020012f6();
void Func_0200126a();
s32 Func_02001262(s32);
void Func_0200128a(void);
void Func_02001318(s32);
void Func_02001330(s32, s32);
s32 Func_02000732(void);
void Func_020013b6(s32, s32);
void Func_020012b4(void);
s32 Func_020012a0(s32);
void Func_02001352(s32);
void Func_0200136a(s32, s32);
void Func_020013a6();
s32 Func_020007a8(void);
void Func_0200142c();
void Func_0200132a();
void Func_020013b8();
s32 Func_02001316();
void Func_020013c8();
void Func_020013e0();
void Func_020013e0_local();
void Func_02001354();
void Func_02001490();
void Func_0200142e();
void Func_02001436();
void Func_02001456();
void Func_020013f8();
s32 Func_0200140a();
void Func_0200140c();
void Func_02001422();
void Func_02001432();
void Func_0200144a();
void Func_0200145a();
void Func_0200145c();
void Func_02001470();
void Func_020014e2();
s32 Func_020014e8();
void Func_0200150e();
void Func_02001526();
void Func_0200152e();
void Func_02001538();
void Func_0200153a();
void Func_0200153e();
void Func_02001558();
void Func_02001598();
void Func_020015bc();
void Func_0200160c();
void Func_02001618();
void Func_02001624();
void Func_02001554();
s32 Func_0200157a();
void Func_02001594();
void Func_020015b2();
void Func_02001668();
void Func_02001674();
void Func_0200167e();
void Func_02001680();
void Func_0200159c();
void Func_0200156a(struct SceneMotion *, s32);
void Func_02001704(s32);
void Func_020015d8();
void Func_02001740(s32);
void Func_020015cc(struct SceneMotion *, s32);
u32 Func_02001a82(void);
u32 Func_02001ae4(void);
u32 Func_02001b0a(void);
void Func_02001bea(struct SceneMotion *, s32);
void Func_02001b40(struct SceneMotion *);
struct SceneMotion *Func_02001b6c(s32, s32, s32, s32);
void Func_02001948(struct SceneMotion *);
void Func_02001b88(struct SceneMotion *, s32);
s32 Func_02001686();
void Func_02001690();
void Func_020016a4();
void Func_020016b6_a();
void Func_020016b6_b();
void Func_020016ca_a();
void Func_020016ca_b();
void Func_020016d2();
void Func_020016d6();
void Func_02001732();
void Func_020017bc();
struct Actor *Func_0200171a(s32);
void Func_020016f0(s32);
void Func_02001700(s32);
void Func_02001792();
s32 Func_0200179c();
void Func_020017ac();
void Func_020017c4();
s32 Func_020017ce();
s32 Func_020017d4();
s32 Func_020017dc();
void Func_020017fa();
s32 Func_02001806();
s32 Func_02001812();
void Func_02001832();
void Func_02001836();
void Func_02001842();
s32 Func_02001866();
void Func_02001878();
void Func_0200187e();
void Func_02001884();
void Func_0200189c();
void Func_020018a2();
void Func_020018a4();
void Func_020018a8();
void Func_020018b8_a();
void Func_020018b8_b();
s32 Func_020018c2();
void Func_020018d0();
void Func_02001900();
void Func_02001906();
void Func_02001918();
u8 *Func_0200191a();
void Func_02001924();
s32 Func_0200193e();
void Func_02001944();
s32 Func_0200194e();
void Func_02001952();
u8 *Func_0200195c();
s32 Func_0200196a();
s32 Func_0200197a();
void Func_02001980();
u8 *Func_02001988_a();
u8 *Func_02001988_b();
s32 Func_02001990();
void Func_02001992();
void Func_02001998();
void Func_020019ae();
s32 Func_02001a30();
void Func_02001a38();
s32 Func_02001a40();
void Func_02001a6e();
s32 Func_020019bc();
s32 Func_020019e8();
u8 *Func_02001a12();
s32 Func_02001a16();
void Func_02001a3c();
u8 *Func_02001a3e();
void Func_02001a52();
void Func_02001a68();
u8 *Func_02001a6c();
s32 Func_02001a7a();
void Func_02001a80();
s32 Func_02001a8c();
void Func_02001a96();
s32 Func_02001aa2();
void Func_02001b52(struct Actor02000c0c *, s32);
void Func_020015a4();
void Func_02001bc6();
void Func_02001c04();
void Func_02001c20();
void Func_02001c3c();
void Func_02001c7c();
void Func_02001c88();
void Func_02001c96();
void Func_02001cba();
void Func_02001cbe();
void Func_02001cd4();
void Func_02001ce6();
void Func_02001cee();
void Func_02001cfa();
void Func_02001d08();
void Func_02001d0a();
void Func_02001d14();
void Func_02001d20();
void Func_02001d38();
void Func_02001d40();
void Func_02001d5c();
void Func_02001d78();
void Func_02001d8e();

/* Each Func_ symbol names the call word the overlay image holds before loader
 * relocation, not a runtime address. One word can serve two sites with
 * different targets, so the sites stay separate and must not be merged onto a
 * shared name. Names that describe what a target does are provisional. */

/* Calls spelled through these wrappers pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is then shared with later uses in the
 * same block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    f(a0, a1, a2);
}

/* Advance the scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3_020006f0(void (*func)(), s32 a, s32 b, s32 c)
{
    u32 Func_02001aaa(void);

    func(a, b, c);
}

static __inline__ struct SceneMotion *CreateActor(
    struct SceneMotion *(*func)(s32, s32, s32, s32),
    s32 kind, s32 x, s32 y, s32 z)
{
    u32 Func_02001aaa(void);

    return func(kind, x, y, z);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_02001008();       /* opens a scripted sequence */

void Func_02001096();       /* queues a cue / script id */

s32 Func_02001036();        /* result is compared against 1 */

void Func_02001050();       /* closes the scripted sequence */

s32 Func_020011ee();        /* tests a flag id; nonzero when set */

void Func_02001216();       /* opens a scripted sequence */

void Func_020012a4();       /* queues a cue / script id */

void Func_02001230();       /* closes the scripted sequence */

s32 Func_020012d8();        /* tests a flag id; nonzero when set */

void Func_02001300();       /* opens a scripted sequence */

void Func_0200138e();       /* queues a cue / script id */

void Func_0200131a();       /* closes the scripted sequence */

u8 *Func_0200138a();        /* scene/actor record accessor */

void Func_02001386();       /* opens a scripted sequence */

s32 Func_0200136c();        /* tests a flag id; nonzero when set */

void Func_0200141e();       /* queues a cue / script id */

void Func_020013ba();       /* closes the scripted sequence */

s32 Func_02001702();           /* tests a progress flag */

void Func_02001716();          /* sets a scene flag */

s32 Func_02001714();           /* tests a progress flag */

s32 Func_0200171e();           /* tests a progress flag */

void Func_02001730();          /* sets a scene flag */

s32 Func_0200172e();           /* tests a progress flag, second site */

s32 Func_02001738();           /* tests a progress flag, second site */

void Func_0200174a();          /* sets a scene flag */

void Func_020011d4(void);      /* scene continuation */

void Func_02001408(void);      /* scene continuation */

s32 SceneData_SelectTableByWord224(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009120;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_02009288;
    }
    return (s32)Data_020090f0;
}

s32 Func_02000070(void)
{
    return 0;
}

/*
 * The owner at 0x02000074 is eight bytes and includes its one pool word at
 * 0x02000078: the pc-relative load reads that word, so the word belongs to
 * this owner. The word is an address returned without being dereferenced.
 * Many getters share this body, but each returns a different address.
 */
u8 *SceneData_GetTable9390(void)
{
    return (u8 *)0x02009390;
}

u8 *SceneData_SelectFlaggedTable(void)
{
    extern s16 Data_02000240[];

    s32 id = Data_02000240[224];
    if (id == (s32)&Value_0000004b) {
        if (Func_02000f1c(0x909)) {
            Data_0200940c[142] = 0;
            Data_0200940c[166] = 0;
        }
        return Data_0200940c;
    }
    if (id == (s32)&Value_0000004c) {
        if (Func_02000f3e(0x8fd))
            Data_020095bc[46] = 1;
        if (Func_02000f50(0x8fe) || Func_02000f5a(0x907))
            Data_020095bc[94] = 1;
        Func_02000f9e(Data_020095bc);
        return Data_020095bc;
    }
    return Data_020093f4;
}

s32 SceneData_GetPrimaryTable(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009730;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_020099f4;
    }
    return (s32)Data_02009724;
}

void FieldScene_RunActorEightPromptDialogue(void)
{
    extern s16 Data_02000240[];

    u8 *work;

    Func_02001008();
    Func_02001096(0x18bd);
    /* r1 is set before r0; the argument order is unchanged. */
    Func_020010a6(8, 0);

    if (Func_02001036(0, 0) == 1) {
        Func_020010c2(8, 0);
    } else {
        work = Data_03001ebc;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Func_020010ec(8, 0);
    }

    Func_02001050();
}

void SceneDialogue_ShowLine1918(void)
{
    Func_0200105c();
    Func_020010ea(0x1918);
    Func_02001112(9, 0);
    Func_02001076();
}

/*
 * One of two branches of the opening setup: a short branch that only moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 of a
 * separately looked-up record on the way.
 */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    void Func_0200115e();
    void Func_02001184();
    void Func_0200119e();

    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    BattleRuntime_Reset_1();
    if (GameFlag_IsSet_1(0x909) != 0) {
        SceneWork_SetStepValue_1(0x191f);
        BattleEventRuntime_ProcessAction_1(14, 0); /* object 14, action 0 */
    } else {
        Object_SetModeById_1(14, 4); /* object 14, action 4 */
        SceneWork_SetStepValue_2(0x18c7);
        BattleRuntime_RunThenWaitIfModeZero_1(14, 0, 10);
        ready_flag = GameFlag_IsSet_2(0x8ff);
        if (ready_flag == 0) {
            rec18 = Scene_GetRecord_1(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = ready_flag;
            ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
            ObjectMotion_PlaceWithinCameraBounds_1(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            ObjectMotion_SetAngleToward_1(0, 0x4000, 0);
            ObjectMotion_ArmCallback_1(14, 0x3000, 0);
            Object_CommitPositionThenWaitIfModeZero_1();
            BattleRuntime_WaitIfModeZero_1(120); /* should_wait 120 */
            record = Scene_GetRecord_2(0);
            ObjectMotion_PlaceWithinCameraBounds_2(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Object_CommitPositionThenWaitIfModeZero_2();
        }
        ObjectMotion_CallThenWaitForAnimationChange_1(14, 4);
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void SceneDialogue_RunActor17Message1924(void)
{
    void Func_0200115e_local(void);
    void Func_02001184(void);
    void Func_0200119e(void);

    Func_02001144();
    Func_020011d2(0x1924);
    Func_020011fa(17, 0);
    Func_0200115e_local();
}

void SceneDialogue_RunActor9Message1932(void)
{
    void Func_0200115e(void);
    void Func_02001184(void);
    void Func_0200119e(void);

    Func_02001164();
    Func_020011f2(0x1932);
    Func_0200121a(9, 0);
    Func_0200117e();
}

void SceneDialogue_RunActor10Message18d9(void)
{
    void Func_0200115e(void);
    void Func_02001184_local(void);
    void Func_0200119e_local(void);

    Func_02001184_local();
    Func_02001212(0x18D9);
    Func_0200123a(10, 0);
    Func_0200119e_local();
}

void SceneDialogue_RunActor14Message18e1(void)
{
    void Func_0200115e(void);
    void Func_02001184(void);
    void Func_0200119e(void);

    Func_020011a4();
    Func_02001232(0x18E1);
    Func_0200125a(14, 0);
    Func_020011be();
}

void SceneDialogue_RunActor21Message194a(void)
{
    void Func_0200115e(void);
    void Func_02001184(void);
    void Func_0200119e(void);

    Func_020011c4();
    Func_02001252(0x194A);
    Func_0200127a(21, 0);
    Func_020011de();
}

s32 SceneActor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = Func_02001206(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorFifteenFlagBranch(void)
{
    void Func_020012de();
    void Func_02001342();

    if (Func_020011ee(0x242) == 0) {
        Func_02001216();
        Func_020012a4(0x18e7);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Func_020012cc(15, 0);
        Func_02001230();
        return;
    }

    if (Func_020006be() != 0) {
        Func_02001342(19, 15);
        return;
    }

    Func_02001240();
    Func_020012ce(0x18ea);
    if (Func_0200122c(0x909) != 0) {
        Func_020012de(0x1941);
    }
    Func_020012f6(15, 0);
    Func_0200126a();
}

void FieldScene_RunActorTwentyFlagBranch(void)
{
    void Func_020012a4_local(void);
    void Func_020012de_local(void);
    void Func_02001342_local(s32);

    if (Func_02001262(0x241) == 0) {
        Func_0200128a();
        Func_02001318(0x18ED);
        Func_02001330(20, 0);
        Func_020012a4_local();
        return;
    }

    if (Func_02000732() != 0) {
        Func_020013b6(20, 17);
        return;
    }

    Func_020012b4();
    Func_02001342_local(0x18EE);
    if (Func_020012a0(0x909) != 0) {
        Func_02001352(0x1943);
    }
    Func_0200136a(17, 0);
    Func_020012de_local();
}

void FieldScene_RunActorTwentyOneFlagBranch(void)
{
    if (Func_020012d8(0x240) == 0) {
        Func_02001300();
        Func_0200138e(0x18f1);
        Func_020013a6(21, 0);
        Func_0200131a();
        return;
    }

    if (Func_020007a8() != 0) {
        Func_0200142c(21, 16);
        return;
    }

    Func_0200132a();
    Func_020013b8(0x18f2);
    if (Func_02001316(0x909) != 0) {
        Func_020013c8(0x1945);
    }
    Func_020013e0(16, 0);
    Func_02001354();
}

void FieldScene_RunFacingGatedDialogue18(void)
{
    u8 *rec;

    rec = Func_0200138a(0);

    if ((u32)((*(u16 *)(rec + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02001490(6, 18);
        return;
    }

    Func_02001386();

    if (Func_0200136c(0x909) != 0) {
        Func_0200141e(0x1947);
        Func_02001436(18, 0);
    } else {
        Func_0200142e(0x18f5);
        Func_02001456(18, 0);
    }

    Func_020013ba();
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *work;
    u8 *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = *(u8 *volatile *)&Data_03001ebc;
    Func_020013e0_local();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = Data_02009e70;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Func_0200153e(188);
        Call6(Func_020013f8, 42, 33, a, b, 2, 2);
        c = a + 2;
        Call6(Func_0200140c, 42, 35, c, b, 2, 2);
        Func_0200144a(4);
        Call6(Func_02001422, 40, 33, a, b, 2, 2);
        Call6(Func_02001432, 40, 35, c, b, 2, 2);
        Func_02001470(4);
    } else {
        Func_02001598(158);
        if (idx == 3) {
            Call6(Func_0200145a, 33, 42, 8, 17, 1, 2);
        }
        Func_0200145c(*(s32 *)(tbl + off), a, b);
    }
    Call3(Func_020014e2, 0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)&Data_03001ebc + 0x1c0)) = 0x100;
    *(u8 *)(Func_020014e8(0) + 85) = 0;
    Func_0200152e(0, 2);
    if (idx == 6) {
        Func_02001526(0, 2, 0);
    } else {
        if (idx != 1) {
            Call3(Func_0200153a, 0, 2, -4);
        } else {
            Func_020015bc(0, 2);
            Call3(Func_02001558, 0, 0, -4);
        }
    }
    Func_0200150e(10);
    Func_0200160c(*(s16 *)(work + 0x16c));
    Func_02001618();
    Func_02001624();
    Func_02001538();
}

void FieldScene_RunScene3a3SequenceB(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    Func_02001554();
    *(u8 *)(Func_0200157a(0) + 85) = 0;
    Func_0200167e(123);
    Call3(Func_020015b2, 0, 2, -16);
    Func_02001668(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02001674();
    Func_02001680();
    Func_02001594();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    u32 Func_02001aaa(void);

    if (work->delay != 0) {
        if (--work->delay == 1)
            Call3_020006f0(Func_0200159c, -1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Func_0200156a(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Func_02001704(229);
                work->active = 0;
                work->delay = 4;
                Call3_020006f0(Func_020015d8, 0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Func_02001740(152);
        work->active = 1;
        Func_020015cc(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void FieldScene_RunScene3a3SequenceC(void)
{
    u32 i;
    s32 record;

    record = Value1(Func_02001686, 18);
    {
        s32 shown = 0;

        *(volatile u16 *)(record + 100) = shown;
    }
    {
        s32 shown = 0;

        *(volatile u16 *)((record + 100) + 2) = shown;
    }
    *(volatile s32 *)(record + 72) = 0x6666;
    *(volatile s32 *)(record + 108) = 0x20086f1;
    Call3(Func_020016b6_a, 18, 0x13333, 0x9999);
    Call3(Func_020016ca_a, 18, 28, 0x1cc);
    Call3(Func_020016d6, 18, 24, 0x1c0);
    Func_020017bc(229);
    Func_020016d2(18);
    Call3(Func_02001690, 0, 0x10000, 0x10000);
    Func_020016b6_b(4);
    Call3(Func_020016a4, -1, -1, 0xe666);
    Func_020016ca_b(40);
    Func_02001732(18, 1);
}

void SceneState_SetFlag906ByActorNineteenX(void)
{
    extern s16 Data_02000240[];

    struct Actor *p = Func_0200171a(19);

    if ((p->f08 >> 20) == 22) {
        Func_020016f0(0x906);
    } else {
        Func_02001700(0x906);
    }
}

/*
 * Mirror three progress flags into three scene flags, then let the scene id
 * at Data_02000240[224] select one of two continuations. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 SceneState_SyncProgressFlagsAndDispatch(void)
{
    extern s16 Data_02000240[];

    s16 scene;

    if (Func_02001702(0x8fd) != 0) {
        Func_02001716(0x240);
    }

    if (Func_02001714(0x8fe) != 0 || Func_0200171e(0x907) != 0) {
        Func_02001730(0x241);
    }

    if (Func_0200172e(0x8fe) != 0 && Func_02001738(0x907) != 0) {
        Func_0200174a(0x242);
    }

    scene = Data_02000240[224];
    if (scene == (s32)&Value_0000004b) {
        Func_020011d4();
    } else if (scene == (s32)&Value_0000004c) {
        Func_02001408();
    }

    return 0;
}

void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_02000240[];

    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;
    s32 bit;

    scene = Value1(Func_020017d4, 0);
    rec5 = Value1(Func_0200179c, 0x242);
    if (rec5 != 0) {
        Func_02001792(64, 32, 0, 32, 32, 32);
        Call6(Func_020017ac, 64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = Value1(Func_020017ce, 0x241);
        if (rec6 != 0) {
            Func_020017c4(64, 0, 0, 32, 32, 32);
            Value6(Func_020017dc, 64, 0, 32, 32, rec5, rec5);
            Func_02001842(17);
            kind = 20;
        } else {
            rec0 = Value1(Func_02001806, 0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Func_020017fa(0, 64, 0, 32, 32, 32);
            Value6(Func_02001812, 0, 64, 32, 32, rec6, rec6);
            Func_02001878(16);
            kind = 17;
        }
    }
    Func_0200187e(kind);
    Func_02001884(21);
    goto L_020009da;
    L_020009b8:;
    Call6(Func_02001836, 0, 32, 32, 32, rec0, rec0);
    Func_0200189c(15);
    Func_020018a2(16);
    Func_020018a8(17);
    L_020009da:;
    if (Value1(Func_02001866, 0x8ff) != 0) {
        Func_020018b8_a(18);
    } else {
        Func_02001998(170);
        Func_02001918(18, 2);
        Func_02001900(18, 3);
        Call2(Func_02001832, 0x2008d09, 0xc80);
    }
    if (Data_02000240_t[225][0] == 3) {
        Call1(Func_020018b8_b, 0x12f);
    }
    Call6(Func_020018a4, 0, 33, 4, 3, 20, 41);
    if (Value1(Func_020018c2, 0x906) != 0) {
        Call3(Func_02001944, 19, 0x1680000, 0xa80000);
    }
    Func_020018d0((s32)Func_0200191a(19), 0);
    Func_02001980(22, 15);
    Call2((void (*)())Func_02001988_a, 23, 15);
    bit = 8;
    Func_02001990(24, 15);
    *(u8 *)(Func_0200193e(22) + 89) |= bit;
    *(u8 *)(Func_0200194e(23) + 89) |= bit;
    {
        u8 *record = Func_0200195c(24);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | bit);
    }
    bit = 2;
    *(u8 *)(Func_0200196a(22) + 35) |= bit;
    *(u8 *)(Func_0200197a(23) + 35) |= bit;
    {
        u8 *record = Func_02001988_b(24);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | bit);
    }
    Func_02001a30(22, 1);
    Func_02001a38(23, 1);
    Func_02001a40(24, 1);
    Func_02001906(1);
    Func_02001992();
    Func_02001a6e(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Func_02001952();
    Func_020019ae();
    Func_02001924(1);
}

void FieldScene_RunScene3a3SequenceD(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_020019bc, 0x240) == 0) {
        Call3(Func_02001a3c, 8, 0x3280000, 0x2d70000);
        record = Func_02001a12(8);
        {
            s32 shown = 0x3000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call3(Func_02001a52, 9, 0x31a0000, 0x3390000);
    }
    if (Value1(Func_020019e8, 0x241) == 0) {
        Call3(Func_02001a68, 10, 0x2300000, 0x2c60000);
        record = Func_02001a3e(10);
        {
            s32 shown = 0x1000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call3(Func_02001a80, 11, 0x2400000, 0x2c60000);
    }
    if (Value1(Func_02001a16, 0x242) == 0) {
        Call3(Func_02001a96, 15, 0x1270000, 0x2e80000);
        record = Func_02001a6c(15);
        {
            s32 shown = 0xb000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
    } else {
        record = Value1(Func_02001a7a, 15);
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
    record = Value1(Func_02001a8c, 17);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
    record = Value1(Func_02001aa2, 16);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];

            record[89] = (u8)(value | 4);
        }
    }
}

void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor)
{
    void Func_02001aaa_local(struct Actor02000c0c *, s32);

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    Func_02001b52(actor, 3);
    Func_02001aaa_local(actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
    u32 Func_02001aaa(void);

    work->x += (work->timer << 12) +
        ((s16)((s32)((Func_02001a82() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((Func_02001aaa() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((Func_02001ae4() * work->timer) >> 16 == 0)
        Func_02001bea(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((Func_02001b0a() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Func_02001b40(work);
    }
}

void SceneEffect_SpawnDriftingParticle(void)
{
    u32 Func_02001aaa(void);

    struct SceneMotion *work;
    if ((Data_03001e40 & 3) == 0) {
        work = CreateActor(Func_02001b6c, 222, 0x400000, 0, 0x1900000);
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            Func_02001948(work);
            work->callback = (void (*)(struct SceneMotion *))0x02008c45;
            Func_02001b88(work, 1);
        }
    }
}

void FieldScene_RunScene3a3_02000d58(void)
{
    u32 i;
    s32 record;

    Func_02001c04();
    Call2(Func_02001cd4, 0x6666, 0xccc);
    Call4(Func_02001cee, 0x3f0000, -1, 0x1c20000, 1);
    Func_02001cfa();
    Func_02001c20(30);
    Func_02001c88(18, 1);
    Call1(Func_02001d38, -1);
    Call1(Func_02001bc6, 0x2008d09);
    Func_02001c3c(20);
    Func_02001cbe(0, 18, 0);
    Call3(Func_02001d0a, 0, 0x4000, 0);
    Func_02001d14(18, 0, 20);
    Call3(Func_02001d20, 18, 0xd000, 40);
    Func_02001d8e(147);
    Func_02001ce6(18, 2);
    Func_02001c7c(20);
    Call3(Func_02001d40, 18, 0xb000, 40);
    Func_020015a4();
    Func_02001d5c(0, 1);
    Func_02001d78();
    Func_02001d08(14, 4);
    Call1(Func_02001c96, 0x8ff);
    Func_02001cba();
}
