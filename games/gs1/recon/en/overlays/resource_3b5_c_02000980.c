#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001fb6[];
extern u8 Data_03001ebc[];
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

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
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

/* Newly named engine calls: the site's own calling form is kept; each name
 * states only what the engine function's own source shows it doing. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Object-id slot at 0x1f4 of the shared scene work record; set once below to
 * the id of the object created earlier in this function. */
#define SCENE_OBJECT_ID (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1f4))

/* Sets up objects 29/30/32, branches on a query result to run one of two
 * near-identical setup sequences for objects 29/30/20 (with different
 * positions, sizes and speeds), then finishes with shared placement calls
 * on 29/30/32. */
void Scene_RunPrimarySequence(void)
{
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
        bump_step(1);
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
