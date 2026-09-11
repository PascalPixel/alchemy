#include "types.h"

/* overlays/scene/actor/proximity/actor_proximity.c */
#define Actor_UpdatePlayerProximity Func_0200007c
struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

s32 Actor_GetPositionDistance(s32 *, s32 *);
u32 Actor_GetAngle(s32, s32);
void Actor_SetMode(struct SceneActor *, s32);
struct SceneActor *Actor_Find(s32);

s32 Actor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Actor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Actor_GetAngle(target->z - actor->z,
                                            *targetPos - *actorPos);
        u32 farLeft = (angle - 0x2000) & 0xf000;
        u32 farRight = (angle + 0x2000) & 0xf000;
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Actor_SetMode(actor, 1);
            result = 1;
        }
        if (target == Actor_Find(0) && (farRight == facing || farLeft == facing)) {
            actor->active = 1;
            Actor_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Actor_SetMode(actor, 2);
    }
    return result;
}
#undef Actor_UpdatePlayerProximity

/* overlays/scene/actor/proximity/actor_setup_and_events.c */
/*
 * Overlay resource_3b5: shared scene work plus the actor setup, dialogue and
 * event sequences that drive this scene, in address order.
 */


#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

/*
 * Each macro below stands for one per-site call word held in the overlay
 * image and names the engine function that site reaches through the veneers,
 * keeping the site's own calling form. One word can serve two sites with
 * different targets, so the bindings are per call and not per name. Names
 * that have no binding in this tree are provisional.
 */
#define Battle_Reset_1(args...) Func_020016e0(args)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_020017c2, a0, a1, a2, a3)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02001728, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02001736, a0, a1, a2)
#define SceneWork_SetStepValue_1(args...) Func_020017ae(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200177c, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_0200178a, a0, a1, a2)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_020017ca(args)
#define Scene_GetRecord_1(args...) Func_02001760(args)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_020017ac, a0, a1, a2)
#define Motion_ResetPosMode2_1(args...) Func_02001796(args)
#define Motion_ResetPosMode2_2(args...) Func_020017a0(args)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_020017b4, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02001840_a, a0, a1, a2)
#define Motion_CommitPos_1(args...) Func_020017d6(args)
#define Object_SetModeById_1(args...) Func_020017ee(args)
#define Object_SetModeById_2(args...) Func_020017f6(args)
#define Object_SetModeById_3(args...) Func_020017fe(args)
#define Motion_SetAngleToward_1(args...) Func_02001828(args)
#define Motion_SetAngleToward_2(args...) Func_02001832(args)
#define Battle_WaitMode0_1(args...) Func_020017b0(args)
#define Battle_WaitMode0_2(a0, a1) Call2(Func_0200188a_a, a0, a1)
#define Battle_WaitMode0_3(a0, a1) Call2(Func_02001894, a0, a1)
#define Motion_SetVarCb_1(args...) Func_02001844(args)
#define Motion_SetVarCbObj_1(args...) Func_02001854_a(args)
#define Battle_WaitMode0_4(args...) Func_020017da(args)
#define Motion_SetSpeed_3(args...) Func_0200188a_b(args)
#define Battle_WaitMode0_5(args...) Func_020017e8(args)
#define UiWork_Create_1(args...) Func_020017e2(args)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02001838, a0, a1)
#define Battle_WaitMode0_6(args...) Func_02001822(args)
#define Motion_SetVarCbObj_2(args...) Func_020018aa(args)
#define Battle_WaitMode0_7(args...) Func_02001830(args)
#define Motion_ArmCb_2(args...) Func_02001902(args)
#define Battle_WaitMode0_8(args...) Func_02001840_b(args)
#define Battle_WaitMode0_9(args...) Func_02001846(args)
#define Motion_CallWaitAnim_1(args...) Func_020018be(args)
#define Battle_WaitMode0_10(args...) Func_02001854_b(args)
#define Motion_ArmCb_3(args...) Func_02001926(args)
#define Battle_WaitMode0_11(args...) Func_02001864(args)
#define BattleEv_RunWait_1(args...) Func_0200191c(args)
#define Battle_WaitMode0_12(args...) Func_02001872(args)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02001946, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02001952, a0, a1, a2)
#define Battle_WaitMode0_13(args...) Func_02001890(args)
#define Object_SetModeById_4(args...) Func_02001900(args)
#define Motion_CallWaitAnim_2(args...) Func_02001910(args)
#define Battle_WaitMode0_14(args...) Func_020018a6(args)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_020018e0, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_020018ea, a0, a1, a2)
#define Motion_ResetPosMode2_3(args...) Func_02001904_a(args)
#define Battle_WaitMode0_15(args...) Func_020018ca(args)
#define Motion_ResetPosMode2_4(args...) Func_02001914_a(args)
#define Motion_CommitPos_2(args...) Func_02001932(args)
#define Motion_ResetPosMode2_5(args...) Func_02001924(args)
#define Motion_SetPosReset_2(args...) Func_02001936(args)
#define Battle_WaitMode0_16(args...) Func_020018f6(args)
#define Motion_SetVarCbObj_3(args...) Func_0200197e(args)
#define Battle_WaitMode0_17(args...) Func_02001904_b(args)
#define Motion_ArmCb_6(args...) Func_020019d6(args)
#define Battle_WaitMode0_18(args...) Func_02001914_b(args)
#define Battle_WaitMode0_19(args...) Func_0200191a(args)
#define Motion_CallWaitAnim_3(args...) Func_02001992(args)
#define Battle_WaitMode0_20(args...) Func_02001928(args)
#define Motion_ArmCb_7(args...) Func_020019fa(args)
#define Battle_WaitMode0_21(args...) Func_02001938(args)
#define BattleEv_RunWait_2(args...) Func_020019fe(args)
#define Battle_WaitMode0_22(args...) Func_02001954(args)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_02001a28, a0, a1, a2)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_02001a34, a0, a1, a2)
#define Battle_WaitMode0_23(args...) Func_02001972(args)
#define Object_SetModeById_5(args...) Func_020019e2(args)
#define Motion_CallWaitAnim_4(args...) Func_020019f2(args)
#define Battle_WaitMode0_24(args...) Func_02001988(args)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_020019c2, a0, a1, a2)
#define Motion_SetSpeed_7(a0, a1, a2) Call3(Func_020019cc, a0, a1, a2)
#define Motion_ResetPosMode2_6(args...) Func_020019e6(args)
#define Motion_SetPosReset_3(args...) Func_020019f8(args)
#define Motion_SetHPosTerrain_4(args...) Func_02001a1a(args)
#define Motion_SetHPosTerrain_5(args...) Func_02001a24(args)
#define Motion_SetHPosTerrain_6(args...) Func_02001a2e(args)
#define GameFlag_Set_1(a0) Call1(Func_020019ce, a0)
#define Battle_SchedShoulder_1(args...) Func_020019ea(args)

/*
 * Object-id slot at 0x1f4 of the shared scene work record, set once below to
 * the id of the object created earlier in the same function.
 */
#define SCENE_OBJECT_ID (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1f4))
#define State_SetValues31_2_4 Func_02000030
#define Actor_GetPositionDistance Func_02000040
#define Actor_UpdatePartnerProximity Func_02000170
#define SceneData_GetTable9060 Func_02000208
#define SceneData_ReturnZero Func_02000210
#define SceneData_GetTable91f8 Func_02000214
#define SceneData_GetTable9238 Func_0200021c
#define FieldScene_RunScene3b5_02000224 Func_02000224
#define ConfigureAndPlaceActorOneHundredTwo Func_02000260
#define Dialogue_RunMessage0e36 Func_020002b8
#define Dialogue_RunMessage0e37 Func_020002d4
#define Scene_RunSupplementalSequenceTwo Func_020002f0
#define SceneData_SelectTable94a8ByFlags Func_020004b0
#define Dialogue_RunActor15Message1f92 Func_020004e8
#define Dialogue_RunActor24Message1f9d Func_02000508
#define FieldScene_RunScene3b5_02000528 Func_02000528
#define FieldScene_RunScene3b5_02000568 Func_02000568
#define Dialogue_RunActor27Message1fa3 Func_0200059c
#define Dialogue_RunActor24Message235f Func_020005bc
#define FieldScene_RunScene3b5_020005dc Func_020005dc
#define SceneScript_SetupActors Func_02000644
#define Actor_CopyPlayerModeToActor Func_020006e8
#define Scene_RunScene3b5SequenceA Func_02000894
#define State_SetValue30ThenCall Func_02000954
#define State_PassWorkHalfword16C Func_02000964
#define Scene_RunPrimarySequence Func_02000980
#define Scene_ResetActor9AndDrawTiles Func_02000ca8

struct SceneSetup {
    u16 *commands;
    u16 first;
    u16 second;
};

struct Rec_3b5 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b5 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b5 *volatile f80;   /* +80, re-read for the second store */
};

struct Actor {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};

extern u8 Data_00001f95[];
extern u8 Data_00001f96[];
extern u8 Data_00001faa[];
extern u8 Data_00001fab[];
extern u8 Data_000021e2[];
extern u8 Data_000021e3[];
extern u8 Data_000021f5[];
extern u8 Data_000021f6[];
extern u8 Data_00002365[];
extern u8 Data_00002366[];
extern u8 Data_00002371[];
extern u8 Data_00002372[];
extern u8 Data_02009a9c[];
extern u8 Data_020097a8[];
extern u8 Data_020094a8[];
extern u8 Data_00001fa0[];
extern struct SceneSetup Data_02009d00[];
extern u8 Data_00001fb6[];

void Func_02000e96(s32, s32, s32);
typedef s32(*IwramSqrt02000040)(s32);
u8 *Func_02000f1e();
s32 Func_0200022a();
u8 *Func_02000f34();
void Func_0200026a();
void Func_02000f66();
void Func_02000f7e();
s32 Func_02000fa6_a();
s32 Func_02000fa6_b();
u8 *Func_02000fb8();
s32 Func_0200132a();
void Func_0200133c_a();
void Func_0200133c_b();
void Func_0200138a();
void Func_02001396();
void Func_020013ea();
void Func_02001402();
void Func_02001412();
void Func_02001432();
void Func_02001452();
void Func_020015f4();
void Func_02001618();
void Func_02001642();
s32 Func_0200164e();
void Func_0200167e();
void Func_02001688();
void Func_020016a4();
void Func_020016ae();
void Func_020016bc();
void Func_020016be();
s32 Func_020016ce_a();
s32 Func_020016ce_b();
void Func_020016da();
void Func_020016fa();
void Func_02001740();
void Func_02001748();
void Func_02000f92();
void Func_020010d8(s32, s32, s32);
void Func_02000fbe();
void Func_02001104(s32, s32, s32);
void Func_020010b2(s32);
void Func_020010cc(s32, s32);
void Func_020010ce(s32);
void Func_020010e8(s32, s32);
void Func_0200106a();
s32 Func_02001070();
s32 Func_0200107a();
s32 Func_0200108c();
void Func_0200109a();
s32 Func_020010a0();
s32 Func_020010ae();
void Func_0200110e();
void Func_0200113c();
void Func_0200114a();
s32 Func_02001150();
s32 Func_0200115a();
s32 Dialogue_ShowMessage();
s32 SceneFlag_Test();
void Func_0200116e();
void Func_0200117a();
s32 Func_02001180();
s32 Func_0200118e();
void Func_020011a0();
void Func_020011ee();
void Func_0200121c();
void Func_0200124c();
void Func_0200124e();
void Func_02001280();
s32 Func_020011fc(s32);
s32 Func_0200120a(s32);
void Func_02001248(void);
void Func_020012e6(s32);
s32 Func_0200130e(s32, s32);
void Func_02001262(void);
void Func_02001268(void);
void Func_02001306(s32);
s32 Func_0200132e(s32, s32);
void Func_02001282(void);
void Func_02001288();
void Func_020012c2();
void Func_02001326();
void Func_0200134a();
void Func_0200135a();
void Func_0200135e();
void Func_0200136e();
void Func_020012c8();
void Func_020012f6();
void Func_0200134c();
void Func_0200137a();
void Func_02001392();
void Func_02001394();
void Func_020012fc(void);
void Func_0200139a(s32);
void Func_020013b2(s32, s32);
void Func_02001316(void);
void Func_0200131c(void);
void Func_020013ba(s32);
s32 Func_020013e2(s32, s32);
void Func_02001336(void);
void Func_020013a8(void);
void Func_020014dc(s32);
void Func_0200139e();
void Func_0200141c(s32, s32, s32);
void Func_02001468(s32, s32);
void Func_02001460(s32, s32, s32);
void Func_02001416(s32);
void Func_020014fe(s32);
void Func_0200150a(void);
void Func_02001516(void);
void Func_0200143a(void);
struct Work_3b5 *Func_02001470();
void Func_020009e8();
void Func_0200145c();
s32 Func_020014ce();
s32 Func_020014d8_a();
s32 Func_020014d8_b();
s32 Func_020014f0();
s32 Func_020014f6();
void Func_020014f8();
s32 Func_02001500_a();
s32 Func_02001500_b();
s32 Func_02001506();
s32 Func_02001518();
s32 Func_02001520();
void Func_0200155a();
s32 Func_02001586();
void Func_02001588();
void Func_0200159a();
void Func_020015a8();
void Func_020015b6();
void Func_020015c4();
void Func_020015d2();
void Func_020015e0();
void Func_020015ee();
void Func_02001608();
void Func_02001624();
void Func_0200167c();
void Func_0200178e(s32);
void Func_020016c2(void);
void Func_020017aa(s32);
void Func_020016e0();
void Func_0200170e();
void Func_02001728();
void Func_02001736();
s32 Func_02001760();
void Func_0200177c();
void Func_0200178a();
void Func_02001796();
void Func_020017a0();
void Func_020017ac();
void Func_020017ae();
void Func_020017b0();
void Func_020017b4();
void Func_020017c2();
void Func_020017ca();
void Func_020017d6();
void Func_020017da();
s32 Func_020017de();
void Func_020017e2();
void Func_020017e8();
void Func_020017ee();
void Func_020017f6();
void Func_020017fe();
void Func_02001822();
void Func_02001828();
void Func_02001830();
void Func_02001832();
s32 Func_02001838();
void Func_02001840_a();
void Func_02001840_b();
void Func_02001844();
void Func_02001846();
void Func_02001854_a();
void Func_02001854_b();
void Func_02001864();
void Func_02001872();
void Func_0200188a_a();
void Func_0200188a_b();
void Func_02001890();
void Func_02001894();
void Func_020018a6();
void Func_020018aa();
void Func_020018be();
void Func_020018ca();
void Func_020018e0();
void Func_020018ea();
void Func_020018f6();
void Func_02001900();
void Func_02001902();
void Func_02001904_a();
void Func_02001904_b();
void Func_02001910();
void Func_02001914_a();
void Func_02001914_b();
void Func_0200191a();
void Func_0200191c();
void Func_02001924();
void Func_02001926();
void Func_02001928();
void Func_02001932();
void Func_02001936();
void Func_02001938();
void Func_02001946();
void Func_02001952();
void Func_02001954();
void Func_02001972();
void Func_0200197e();
void Func_02001988();
void Func_02001992();
void Func_020019c2();
void Func_020019cc();
void Func_020019ce();
void Func_020019d6();
void Func_020019e2();
void Func_020019e6();
void Func_020019ea();
void Func_020019f2();
void Func_020019f8();
void Func_020019fa();
void Func_020019fe();
void Func_02001a1a();
void Func_02001a24();
void Func_02001a28();
void Func_02001a2e();
void Func_02001a34();
struct Actor *Func_02001a2c(s32);
void Func_020019e0(struct Actor *, s32);
void Func_02001a90(s32, s32);
void Func_02001a04(s32, s32, s32, s32, s32, s32);
void Func_02001a3a(s32);

/*
 * These declarations are deliberately old-style: overlay import arities are
 * not fixed per name, and each call site names the veneer it reaches rather
 * than the import behind it, so one import reached from several sites carries
 * several names.
 *
 * Func_0200026a is void because its result is discarded.
 *
 * Func_0200022a returns s32 because callers test the result whole and never
 * truncate it; a u8 return would add a widening shift that is not wanted.
 */

/*
 * Each Func_ name is a loader-relocated call word, not a runtime address.
 *
 * Call sites spelled through the wrappers below pass their constants straight
 * into the argument registers. A direct call instead precomputes a costly
 * constant into a temporary that is then shared with later uses in the same
 * block. A value-returning call sets r0 last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    /* The scene step counter is at 0x1d8 of the shared scene work record. */
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    f(a0, a1, a2);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_020010d8(actor, x, y);
}

static __inline__ void PlaceActor_02001104(s32 actor, s32 x, s32 y)
{
    Func_02001104(actor, x, y);
}

static __inline__ void bump_step_02000528(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void bump_step_02000568(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_02000644(void (*f)(), s32 a)
{
    extern u8 *Data_03001ebc;
    u8 *Func_020013d2(s32);
    u8 *Func_0200141a(s32);

    f(a);
}

static __inline__ void Call3_02000644(void (*f)(), s32 a, s32 b, s32 c)
{
    extern u8 *Data_03001ebc;
    u8 *Func_020013d2(s32);
    u8 *Func_0200141a(s32);

    f(a, b, c);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void bump_step_02000728(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2, a3);
}

static __inline__ void bump_step_02000980(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void State_SetValues31_2_4(void)
{
    Func_02000e96(0x1F, 2, 4);
}

s32 Actor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt02000040) 0x030001D8)(dxsq + dysq + dzsq);
}

/*
 * Per-frame actor callback, whose argument is the owning actor record. The
 * initialiser stores this address plus the Thumb bit into field +0x6c of
 * actors 16 and 17. It returns a constant zero.
 *
 * The owner's eight-byte literal pool follows its return and belongs to it.
 * 0x03001e8c is a table of pointers whose entry 12 is 0x03001ebc, the overlay
 * work pointer the rest of this overlay loads directly.
 */
s32 Actor_UpdatePartnerProximity(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * The branch must stay two calls: as a conditional expression the
     * selector folds into arithmetic on the bit instead.
     */
    if ((*flags & 1) != 0) {
        partner = Func_02000f1e(17);
    } else {
        partner = Func_02000f1e(16);
    }
    if (Func_0200022a(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Func_02000f34(0);

    /*
     * Widen the test when the scene counter at work + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Func_0200026a(self, player, range, force);
    return 0;
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetTable9060(void)
{
    return (u8 *)0x02009060;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable91f8(void)
{
    return (u8 *)0x020091f8;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable9238(void)
{
    return (u8 *)0x02009238;
}

void FieldScene_RunScene3b5_02000224(void)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    u32 i;
    u8 *record;

    record = Value1(Func_02000fa6_a, 8);
    if ((s32)record != 0) {
        record[89] = 0;
    }
    record = Func_02000fb8(8);
    Func_02000f66((s32)record, 0);
    Call4(Func_02000f7e, 0, 0x2200000, 0x1200000, 253);
    Call1(Func_02000fa6_b, 0x200);
}

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Func_02000f92(3, 32, 1, 1, a, b);
    PlaceActor(102, 0x00380000, 0x01a80000);
}

void Func_0200028c(void)
{
    s32 a = 3, b = 26;
    Func_02000fbe(2, 25, 1, 1, a, b);
    PlaceActor_02001104(102, -1, -1);
}

void Dialogue_RunMessage0e36(void)
{
    Func_020010b2(0x0E36);
    Func_020010cc(-1, 0);
}

void Dialogue_RunMessage0e37(void)
{
    Func_020010ce(0x0E37);
    Func_020010e8(-1, 0);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    extern u8 Data_03001ebc[];

    u8 *__restrict p5;
    s32 rec;
    s32 p8;
    s32 msg;

    p5 = *(u8 **)Data_03001ebc;
    rec = Func_0200107a(16);
    p8 = *(s16 *)(rec + 6);
    Func_0200106a();
    {
        volatile u16 *flags = (volatile u16 *)(rec + 100);
        u16 value = *flags;

        *flags = (u16)(value | 2);
    }
    if (*(s16 *)(p5 + 0x17e) == 0) {
        if (Func_02001070(0x950) != 0) {
            msg = (s32)Data_00002365;
        } else if (Func_0200108c(0x962) != 0) {
            msg = (s32)Data_000021e2;
        } else {
            msg = (s32)Data_00001f95;
        }
    } else {
        if (Func_020010a0(0x950) != 0) {
            msg = (s32)Data_00002371;
        } else if (Func_020010ae(0x962) != 0) {
            msg = (s32)Data_000021f5;
        } else {
            msg = (s32)Data_00001faa;
        }
    }
    Dialogue_ShowMessage(msg);
    Func_0200113c(16, 0);
    Func_0200116e(16, 0, 2);
    Func_020011a0(16, 0, 10);
    *(volatile u16 *)(rec + 6) = p8;
    Func_0200109a(1);
    *(volatile u16 *)(rec + 100) &= 1;
    Func_0200110e();
}

void Func_020003d0(void)
{
    extern u8 Data_03001ebc[];

    u8 *__restrict p5;
    s32 rec;
    s32 p8;
    s32 msg;

    p5 = *(u8 **)Data_03001ebc;
    rec = Func_0200115a(17);
    p8 = *(s16 *)(rec + 6);
    Func_0200114a();
    {
        volatile u16 *flags = (volatile u16 *)(rec + 100);
        u16 value = *flags;

        *flags = (u16)(value | 2);
    }
    if (*(s16 *)(p5 + 0x17e) == 0) {
        if (Func_02001150(0x950) != 0) {
            msg = (s32)Data_00002366;
        } else if (SceneFlag_Test(0x962) != 0) {
            msg = (s32)Data_000021e3;
        } else {
            msg = (s32)Data_00001f96;
        }
    } else {
        if (Func_02001180(0x950) != 0) {
            msg = (s32)Data_00002372;
        } else if (Func_0200118e(0x962) != 0) {
            msg = (s32)Data_000021f6;
        } else {
            msg = (s32)Data_00001fab;
        }
    }
    Func_0200124c(msg);
    Func_0200121c(17, 0);
    Func_0200124e(17, 0, 2);
    Func_02001280(17, 0, 10);
    *(volatile u16 *)(rec + 6) = p8;
    Func_0200117a(1);
    *(volatile u16 *)(rec + 100) &= 1;
    Func_020011ee();
}

u8 *SceneData_SelectTable94a8ByFlags(void)
{
    if (Func_020011fc(0x950) != 0) {
        return Data_02009a9c;
    }
    if (Func_0200120a(0x962) != 0) {
        return Data_020097a8;
    }
    return Data_020094a8;
}

void Dialogue_RunActor15Message1f92(void)
{
    Func_02001248();
    Func_020012e6(0x1F92);
    Func_0200130e(15, 0);
    Func_02001262();
}

void Dialogue_RunActor24Message1f9d(void)
{
    Func_02001268();
    Func_02001306(0x1F9D);
    Func_0200132e(24, 0);
    Func_02001282();
}

void FieldScene_RunScene3b5_02000528(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001288();
    Func_02001326((s32)Data_00001fa0);
    Call3(Func_0200135a, 25, 0xc000, 0);
    Func_0200134a(25, 0);
    Call3(Func_0200136e, 25, 0x8000, 0);
    Func_0200135e(25, 0);
    Func_020012c2();
}

void FieldScene_RunScene3b5_02000568(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020012c8();
    Call3(Func_02001394, 26, 0x4000, 0);
    Func_0200134c(26, 2);
    Call1(Func_0200137a, 0x1fa2);
    Func_02001392(26, 0);
    Func_020012f6();
}

void Dialogue_RunActor27Message1fa3(void)
{
    Func_020012fc();
    Func_0200139a(0x1FA3);
    Func_020013b2(0x1B, 0);
    Func_02001316();
}

void Dialogue_RunActor24Message235f(void)
{
    Func_0200131c();
    Func_020013ba(0x235F);
    Func_020013e2(24, 0);
    Func_02001336();
}

void FieldScene_RunScene3b5_020005dc(void)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    u32 i;
    s32 record;

    Func_0200133c_a();
    if (Value1(Func_0200132a, 0x8bf) == 0) {
        Call1(Func_0200133c_b, 0x8bf);
        Call1(Func_020013ea, 0x2368);
        Func_02001402(19, 0);
        Func_02001452(233, 3);
        Func_02001412(19, 0);
        Func_020013d2(0, 1);
        Func_0200138a(233, 0);
    } else {
        Call1(Func_0200141a, 0x236a);
        Func_02001432(19, 0);
    }
    Func_02001396();
}

void SceneScript_SetupActors(void)
{
    extern u8 *Data_03001ebc;
    u8 *Actor_Find(s32);

    u8 *work = Data_03001ebc;
    u32 no;
    s32 index;

    Func_020013a8();
    for (no = 8; no <= 65; no++) {
        u8 *actor = Actor_Find(no);
        if (actor != NULL) {
            actor[85] = 0;
        }
    }
    index = *(s16 *)(work + 0x16c) - 1;
    Call1_02000644(Func_020014dc, 158);
    Call3_02000644(Func_0200139e, (s32)Data_02009d00[index].commands,
                 Data_02009d00[index].first,
                 Data_02009d00[index].second);
    Call3_02000644(Func_0200141c, 0, 0x8000, 0x4000);
    Actor_Find(0)[85] = 0;
    Func_02001468(0, 2);
    if (index != 6) {
        Call3_02000644(Func_02001460, 0, 2, -8);
        Func_02001416(10);
    }
    Func_020014fe(*(s16 *)(work + 0x16c));
    Func_0200150a();
    Func_02001516();
    Func_0200143a();
}

/*
 * Copy the player's two-bit mode into both mode fields of the given actor's
 * record and clear its flag byte at +35.
 *
 * f80 is volatile so that the pointer is loaded again for the second store;
 * without that the first load is reused and the second one disappears. The
 * two mode writes must stay bitfields so they share one 32-bit mask, which
 * explicit mask-and-or arithmetic does not produce.
 */
void Actor_CopyPlayerModeToActor(struct Work_3b5 *work)
{
    s32 bits;

    if (work != 0) {
        bits = Func_02001470(0)->f80->mode9;
        work->f35 = 0;
        work->f80->mode9 = bits;
        work->f80->mode15 = bits;
    }
}

s32 Func_02000728(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 handler;
    s32 hidden;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call3(Func_020014f8, 16, 0x1600000, 0x1600000);
    Call2(Func_020014d8_a, 16, 0x2008ec0);
    record = Value1(Func_020014ce, 16);
    handler = 0x2008171;
    {
        volatile u16 *target = (volatile u16 *)(record + 100);
        s32 shown = 1;

        *target = shown;
    }
    *(volatile s32 *)(record + 108) = handler;
    hidden = 0;
    Value3(Func_02001520, 17, 0x1700000, 0x1400000);
    Call2(Func_02001500_a, 17, 0x2008f90);
    record = Value1(Func_020014f6, 17);
    *(volatile u16 *)(record + 100) = hidden;
    *(volatile s32 *)(record + 108) = handler;
    record = Func_02001506(14);
    *(volatile s32 *)(record + 108) = 0x20086e9;
    if (Value1(Func_020014d8_b, 0x8c1) != 0) {
        Call3(Func_0200155a, 28, 0x13c0000, 0x1480000);
    }
    if (Value1(Func_020014f0, 0x201) != 0) {
        Func_0200145c();
    }
    if (Value1(Func_02001500_b, 0x200) != 0) {
        Func_020009e8();
        Func_02001588(8, 4);
    }
    if (Value1(Func_02001518, 0x950) != 0) {
        Call3(Func_0200159a, 20, 0x2080000, 0x2300000);
        Call3(Func_020015a8, 21, 0x2080000, 0x2300000);
        Call3(Func_020015b6, 22, 0x2080000, 0x2300000);
        Call3(Func_020015c4, 24, 0x2080000, 0x2300000);
        Call3(Func_020015d2, 25, 0x2080000, 0x2300000);
        Call3(Func_020015e0, 26, 0x2080000, 0x2300000);
        Call3(Func_020015ee, 27, 0x2080000, 0x2300000);
    } else {
        if (Value1(Func_02001586, 0x962) != 0) {
            Call3(Func_02001608, 27, 0x1180000, 0x500000);
            Call3(Func_0200167c, 27, 0x2000, 0);
            Func_02001624(27, 1);
        }
    }
    return 0;
}

void Scene_RunScene3b5SequenceA(void)
{
    extern u8 Data_03001ebc[];
    void Func_020013d2();
    void Func_0200141a();

    u32 i;
    s32 record;

    Func_020015f4();
    Call3(Func_02001642, 0, 0x130, 0x138);
    Call3(Func_020016ce_a, 0, 0xc000, 0);
    Call3(Func_020016da, 28, 0x4000, 0);
    Func_02001618(20);
    Call1(Func_020016be, 0xe3d);
    Value2(Func_020016ce_b, 28, 0);
    if (Value2(Func_0200164e, 0, 0) == 0) {
        bump_step(1);
        Func_020016fa(28, 0);
        Call3(Func_02001688, 28, 0x10000, 0x8000);
        Call3(Func_020016ae, 28, 0x140, 0x130);
        Call3(Func_020016bc, 28, 0x13c, 0x148);
        Call3(Func_02001748, 28, 0xa000, 0);
        Call1(Func_0200167e, 0x8c1);
    } else {
        Func_02001740(28, 0);
    }
    Func_020016a4();
}

void State_SetValue30ThenCall(void)
{
    Func_0200178e(30);
    Func_020016c2();
}

void State_PassWorkHalfword16C(void)
{
    extern u8 *Data_03001ebc;

    s16 *cnt = (s16 *)(Data_03001ebc + 0x16C);

    Func_020017aa(*cnt);
}

/*
 * Set up objects 29, 30 and 32, branch on a query result to run one of two
 * near-identical sequences for objects 29, 30 and 20 with different
 * positions, sizes and speeds, then finish with shared placement calls.
 */
void Scene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 object32_id;
    s32 list_00001fb6;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Motion_SetSpeed_1(29, 0x10000, 0x8000);
    Motion_SetSpeed_2(30, 0x10000, 0x8000);
    list_00001fb6 = (s32)Data_00001fb6;
    SceneWork_SetStepValue_1(list_00001fb6);
    Motion_SetHPosTerrain_1(29, 0x480000, 0xd00000);
    Motion_SetHPosTerrain_2(30, 0x380000, 0xd00000);
    ObjectGroup_ConfigureChildValue_1(32, 15);
    object32_id = Scene_GetRecord_1(32);
    Func_0200170e(object32_id, 0); /* main:080091e0 */
    Motion_SetHPosTerrain_3(32, 0x5f0000, 0x280000);
    Motion_ResetPosMode2_1(29, 72, 248);
    Motion_ResetPosMode2_2(30, 56, 248);
    Motion_SetPosReset_1(0, 64, 0x108);
    Motion_ArmCb_1(0, 0xc000, 0);
    Motion_CommitPos_1(29);
    Object_SetModeById_1(29, 1);
    Object_SetModeById_2(30, 1);
    Object_SetModeById_3(0, 1);
    Motion_SetAngleToward_1(29, 0, 0);
    Motion_SetAngleToward_2(30, 0, 0);
    Battle_WaitMode0_1(20);
    Battle_WaitMode0_2(29, 0x102); /* main:0808a1f0 */
    Battle_WaitMode0_3(30, 0x102); /* main:0808a1f0 */
    Motion_SetVarCb_1(29, 2);
    Motion_SetVarCbObj_1(30, 2);
    Battle_WaitMode0_4(20);
    Motion_SetSpeed_3(29, 0); /* main:0808a178 */
    Battle_WaitMode0_5(25);
    Value4(Func_020017de, 52, 0, 12, 7); /* main:080150f8 */
    UiWork_Create_1((list_00001fb6 + 3), 11, 12, 2);
    SCENE_OBJECT_ID = 32;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) { /* object_id 0, force 0 */
        Battle_WaitMode0_6(20);
        Motion_SetVarCbObj_2(30, 2);
        Battle_WaitMode0_7(30);
        Motion_ArmCb_2(30, 0, 0);
        Battle_WaitMode0_8(30);
        Battle_WaitMode0_9(10);
        Motion_CallWaitAnim_1(29, 3);
        Battle_WaitMode0_10(20);
        Motion_ArmCb_3(29, 0, 0);
        Battle_WaitMode0_11(30);
        BattleEv_RunWait_1(29, 0);
        Battle_WaitMode0_12(20);
        Motion_ArmCb_4(29, 0x4000, 0);
        Motion_ArmCb_5(30, 0x4000, 0);
        Battle_WaitMode0_13(30);
        Object_SetModeById_4(29, 3);
        Motion_CallWaitAnim_2(30, 3);
        Battle_WaitMode0_14(20);
        Motion_SetSpeed_4(29, 0x1cccc, 0xe666);
        Motion_SetSpeed_5(30, 0x1cccc, 0xe666);
        Motion_ResetPosMode2_3(29, 232, 248);
        Battle_WaitMode0_15(2);
        Motion_ResetPosMode2_4(30, 232, 248);
        Motion_CommitPos_2(29);
        Motion_ResetPosMode2_5(29, 248, 248);
        Motion_SetPosReset_2(30, 248, 248);
    } else {
        Battle_WaitMode0_16(20);
        Motion_SetVarCbObj_3(30, 2);
        Battle_WaitMode0_17(30);
        Motion_ArmCb_6(30, 0, 0);
        Battle_WaitMode0_18(30);
        Battle_WaitMode0_19(10);
        Motion_CallWaitAnim_3(29, 4);
        Battle_WaitMode0_20(20);
        Motion_ArmCb_7(29, 0, 0);
        Battle_WaitMode0_21(30);
        bump_step_02000980(1);
        BattleEv_RunWait_2(29, 0);
        Battle_WaitMode0_22(20);
        Motion_ArmCb_8(29, 0x4000, 0);
        Motion_ArmCb_9(30, 0x4000, 0);
        Battle_WaitMode0_23(30);
        Object_SetModeById_5(29, 3);
        Motion_CallWaitAnim_4(30, 3);
        Battle_WaitMode0_24(20);
        Motion_SetSpeed_6(29, 0x19999, 0xcccc);
        Motion_SetSpeed_7(30, 0x19999, 0xcccc);
        Motion_ResetPosMode2_6(29, 72, 184);
        Motion_SetPosReset_3(30, 56, 184);
    }
    Motion_SetHPosTerrain_4(29, 0, 0);
    Motion_SetHPosTerrain_5(30, 0, 0);
    Motion_SetHPosTerrain_6(32, 0, 0);
    GameFlag_Set_1(0x8c0);
    Battle_SchedShoulder_1();
}

void Scene_ResetActor9AndDrawTiles(void)
{
    struct Actor *actor = Func_02001a2c(9);
    if (actor != 0) {
        Func_020019e0(actor, 0);
        actor->field23 = 2;
        actor->field55 = 0;
    }
    Func_02001a90(9, 5);
    {
        s32 v5 = 34;
        s32 v6 = 16;
        Func_02001a04(36, 16, 1, 1, v5, v6);
    }
    Func_02001a3a(0x201);
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef Battle_Reset_1
#undef Motion_CamBounds_1
#undef Motion_SetSpeed_1
#undef Motion_SetSpeed_2
#undef SceneWork_SetStepValue_1
#undef Motion_SetHPosTerrain_1
#undef Motion_SetHPosTerrain_2
#undef ObjectGroup_ConfigureChildValue_1
#undef Scene_GetRecord_1
#undef Motion_SetHPosTerrain_3
#undef Motion_ResetPosMode2_1
#undef Motion_ResetPosMode2_2
#undef Motion_SetPosReset_1
#undef Motion_ArmCb_1
#undef Motion_CommitPos_1
#undef Object_SetModeById_1
#undef Object_SetModeById_2
#undef Object_SetModeById_3
#undef Motion_SetAngleToward_1
#undef Motion_SetAngleToward_2
#undef Battle_WaitMode0_1
#undef Battle_WaitMode0_2
#undef Battle_WaitMode0_3
#undef Motion_SetVarCb_1
#undef Motion_SetVarCbObj_1
#undef Battle_WaitMode0_4
#undef Motion_SetSpeed_3
#undef Battle_WaitMode0_5
#undef UiWork_Create_1
#undef UiWork_WaitThenFinalizeCapacity_1
#undef Battle_WaitMode0_6
#undef Motion_SetVarCbObj_2
#undef Battle_WaitMode0_7
#undef Motion_ArmCb_2
#undef Battle_WaitMode0_8
#undef Battle_WaitMode0_9
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_10
#undef Motion_ArmCb_3
#undef Battle_WaitMode0_11
#undef BattleEv_RunWait_1
#undef Battle_WaitMode0_12
#undef Motion_ArmCb_4
#undef Motion_ArmCb_5
#undef Battle_WaitMode0_13
#undef Object_SetModeById_4
#undef Motion_CallWaitAnim_2
#undef Battle_WaitMode0_14
#undef Motion_SetSpeed_4
#undef Motion_SetSpeed_5
#undef Motion_ResetPosMode2_3
#undef Battle_WaitMode0_15
#undef Motion_ResetPosMode2_4
#undef Motion_CommitPos_2
#undef Motion_ResetPosMode2_5
#undef Motion_SetPosReset_2
#undef Battle_WaitMode0_16
#undef Motion_SetVarCbObj_3
#undef Battle_WaitMode0_17
#undef Motion_ArmCb_6
#undef Battle_WaitMode0_18
#undef Battle_WaitMode0_19
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_20
#undef Motion_ArmCb_7
#undef Battle_WaitMode0_21
#undef BattleEv_RunWait_2
#undef Battle_WaitMode0_22
#undef Motion_ArmCb_8
#undef Motion_ArmCb_9
#undef Battle_WaitMode0_23
#undef Object_SetModeById_5
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_24
#undef Motion_SetSpeed_6
#undef Motion_SetSpeed_7
#undef Motion_ResetPosMode2_6
#undef Motion_SetPosReset_3
#undef Motion_SetHPosTerrain_4
#undef Motion_SetHPosTerrain_5
#undef Motion_SetHPosTerrain_6
#undef GameFlag_Set_1
#undef Battle_SchedShoulder_1
#undef SCENE_OBJECT_ID
#undef State_SetValues31_2_4
#undef Actor_GetPositionDistance
#undef Actor_UpdatePartnerProximity
#undef SceneData_GetTable9060
#undef SceneData_ReturnZero
#undef SceneData_GetTable91f8
#undef SceneData_GetTable9238
#undef FieldScene_RunScene3b5_02000224
#undef ConfigureAndPlaceActorOneHundredTwo
#undef Dialogue_RunMessage0e36
#undef Dialogue_RunMessage0e37
#undef Scene_RunSupplementalSequenceTwo
#undef SceneData_SelectTable94a8ByFlags
#undef Dialogue_RunActor15Message1f92
#undef Dialogue_RunActor24Message1f9d
#undef FieldScene_RunScene3b5_02000528
#undef FieldScene_RunScene3b5_02000568
#undef Dialogue_RunActor27Message1fa3
#undef Dialogue_RunActor24Message235f
#undef FieldScene_RunScene3b5_020005dc
#undef SceneScript_SetupActors
#undef Actor_CopyPlayerModeToActor
#undef Scene_RunScene3b5SequenceA
#undef State_SetValue30ThenCall
#undef State_PassWorkHalfword16C
#undef Scene_RunPrimarySequence
#undef Scene_ResetActor9AndDrawTiles
