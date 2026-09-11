#include "types.h"
#include "scene.h"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "staged_actor.h"

/* overlays/scene/field/extended_presentation/actor_movement_effect.c */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Desc;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

extern s32 ActorSearchStep[];
extern s32 gOv[];

Ent *Field_unk_02000342(Desc *, Ent *);

s32 Actor_ResetMotionIfBlockedAhead(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = ActorSearchStep[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Field_unk_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv;

        do {
            if (v == *p++) {
                goto done;
            }
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = ActorSearchStep[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Field_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

/* overlays/scene/field/extended_presentation/actor_position.c */
u8 *Field_unk_02004bfa(s32);

void Actor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = Field_unk_02004bfa(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Field_unk_02004b68(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

/* overlays/scene/field/extended_presentation/actor_search.c */

s32 Actor_FindClearActorPosition(s32 *a)
{
}

/* overlays/scene/field/extended_presentation/display.c */

void Effect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Field_unk_0200474c(1);
    Field_unk_0200490a(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (Field_Check(0x201) != 0) {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x1d1d;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 0x3f;
            *slot = c;
        }
        Field_unk_020015ce();
        return;
    } else {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x3f3f;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 31;
            *slot = c;
        }
    }
    {
        s32 a = 0x3f42;
        *(u16 *)0x4000050 = a;
    }
    {
        s32 b = 0xc04;
        *(u16 *)0x4000052 = b;
    }
}

/* overlays/scene/field/extended_presentation/effect.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u16 *gOv2;
extern s8 gOv3;
extern u8 gOv4;

u8 *Field_unk_02004cea(s32);

/* Contiguous unnamed state-owner run for resource_3bd. */

void Actor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        Field_unk_02001e8e(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}

void Effect_UpdateStateMachine(void)
{
    u16 *p = gOv2;
    s32 flag = 1;
    s32 state = *(s16 *)p;

    if (state == 0) {
        s32 t = p[4] + 16;
        p[4] = t;
        if ((u16)t > 0xbff) {
            p[0] = p[0] + 1;
            p[1] = state;
        }
    } else if (state == 1) {
        if ((s16)p[1] == 30) {
            p[0] = p[0] + 1;
        }
    } else if (state == 2) {
        s32 t = p[4] + 0xfff8;
        p[4] = t;
        if ((u16)t <= 0x2ff) {
            p[0] = p[0] + 1;
        }
    } else if (state == 3) {
        s32 v = gOv3;
        s32 r = Field_unk_02004be0(v << 16, 5);
        if ((unsigned int)(((p[3] - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
            u8 *o;
            s32 nv = r + 0x4000;
            p[3] = nv;
            {
                s32 z = 0;
                s32 k = 0x63;
                p[0] = k;
                p[4] = z;
            }
            o = Field_unk_02004cea(v + 11);
            *(s32 *)(o + 0x6c) = (s32)&gOv4;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        gOv2[3] += gOv2[4];
        Field_unk_02001fb2(gOv2[3]);
        q2 = gOv2;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Field_unk_02004ed6(0x87);
            }
        }
    }
    {
        u16 *q = gOv2;
        q[1]++;
    }
}

void OvObj_UpdateThreeStateMotion(void *obj)
{
    s32 position[3];
    s32 x;
    s32 z;
    u8 *p;
    s32 state;

    p = (u8 *)obj + 0x40;
    state = *(s8 *)p;
    if (state == 0) {
        z = FIELD(obj, s32, 0x18);
        x = FIELD(obj, s32, 0x14);
        FIELD(obj, s32, 8) = z;
        position[2] = z;
        FIELD(obj, s32, 4) = x;
        position[0] = x;
        Field_unk_02007684(0x780000, Field_unk_02007670(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Field_unk_02007904(0x86);
    } else if (state == 1) {
        if (Field_unk_02007898(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (Field_unk_020078b0(obj) == 0)
            Field_unk_020078da(obj);
    }
}

/* overlays/scene/field/extended_presentation/move_and_redraw.c */

void Actor_MoveAndRedraw(StagedActorMovementRequest request)
{
}

/* overlays/scene/field/extended_presentation/scene_data.c */
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097

extern u8 gOv5[];
extern u8 gOv6[];
extern s16 RuntimeSelectorTable[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];

/* Contiguous unnamed leaf-owner run for resource_3bd. */

u8 *SceneData_GetTablebf70(void)
{
    return gOv5;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    return gOv6;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
}

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = RuntimeSelectorTable[224];

    if (v == (s32)&Value_00000093) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)gOv9;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)gOv10;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)gOv11;
    }
    return (s32)gOv12;
}

/* overlays/scene/field/extended_presentation/scene_primary_script.c */
#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18

u8 *Scene_GetRecord();

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_3();

/*
 * Primary cutscene script -- resource_3bd. A no-argument, void, almost-linear
 * sequence whose one branch rejoins after both arms.
 */

/*
 * Each alias is the per-site call word the overlay image holds -- one word can
 * serve two sites with different targets -- and the macro names the engine
 * function the site reaches, keeping the site's own calling form. Names
 * without a binding elsewhere in the tree are provisional.
 */

/* Signed coordinate halfwords in a placed-actor record, named by offset. */

void Scene_SetActor13Value41(void)
{
    Field_unk_02004794(13, 0x41);
}

/*
 * Every call site is written out separately and repeated calls must not be
 * folded: the sequence of distinct call words is what reproduces the
 * reference. The three record lookups near the end are null checked before
 * their stored coordinates are forwarded.
 */
void Scene_RunBranchingCutsceneSequence(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set_1(0x960);
    Audio_PlayCue_1(24);
    Battle_Reset_1();
    Field_unk_02006a42(); /* main:0808a460 */
    SceneWork_SetStepValue_1(0x214f);
    Motion_SetVarCbObj_1(8, 2);
    Battle_WaitMode0_1(20);
    BattleEv_RunWait_1(8, 0);
    Battle_WaitMode0_2(10);
    Motion_CamBounds_1(0xf80000, -1, 0xb80000, 1);
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetPosReset_1(0, 248, 192);
    Motion_ArmCb_1(0, 0xc000, 0);
    Motion_RealignToTrackedObjectAndArmCallback_1(1, -16, 16, 0xc000);
    Motion_RealignToTrackedObjectAndArmCallback_2(3, 0, 16, 0xc000);
    Motion_RealignToTrackedObjectAndArmCallback_3(2, 16, 16, 0xc000);
    Motion_CommitPos_1(1);
    Battle_WaitMode0_3(20);
    BattleFx_SpawnLinked_1(2, 0x102, 0);
    Battle_WaitMode0_4(40);
    BattleEv_RunWait_2(2, 0);
    Motion_SetVarCbObj_2(3, 2);
    Battle_WaitMode0_5(20);
    BattleEv_RunWait_3(3, 0);
    Battle_WaitMode0_6(10);
    BattleFx_SpawnLinked_2(8, 0x100, 40);
    BattleEv_RunWait_4(8, 0);
    Battle_WaitMode0_7(10);
    Object_LinkPair_1(1, 2, 50);
    Motion_ArmCb_2(1, 0xc000, 0);
    Motion_ArmCb_3(2, 0xc000, 0);
    Battle_WaitMode0_8(30);
    BattleEv_RunWait_5(1, 0);
    Battle_WaitMode0_9(10);
    Battle_WaitMode0_10(8, 0x102); /* main:0808a1f0 */
    Motion_SetVarCbObj_3(8, 2);
    Battle_WaitMode0_11(30);
    BattleEv_RunWait_6(8, 0);
    Battle_WaitMode0_12(20);
    Object_LinkPair_2(3, 2, 50);
    Motion_ArmCb_4(3, 0xc000, 0);
    Motion_ArmCb_5(2, 0xc000, 0);
    Battle_WaitMode0_13(30);
    BattleEv_RunWait_7(2, 0);
    Battle_WaitMode0_14(10);
    BattleFx_SpawnLinked_3(8, 0x100, 40);
    BattleEv_RunWait_8(8, 0);
    Battle_WaitMode0_15(10);
    Motion_CallWaitAnim_1(3, 4);
    Battle_WaitMode0_16(20);
    BattleEv_RunWait_9(3, 0);
    Battle_WaitMode0_17(10);
    BattleFx_SpawnLinked_4(8, 0x105, 40);
    BattleEv_RunWait_10(8, 0);
    Battle_WaitMode0_18(10);
    Motion_SetAngleToward_1(1, 0, 30);
    BattleFx_SpawnLinked_5(1, 0x101, 40);
    BattleEv_RunWait_11(1, 0);
    Motion_ArmCb_6(1, 0xc000, 0);
    Battle_WaitMode0_19(20);
    BattleEv_RunWait_12(1, 0);
    Battle_WaitMode0_20(10);
    BattleFx_SpawnLinked_6(8, 0x102, 40);
    BattleEv_RunWait_13(8, 0);
    Motion_SetVarCbObj_4(8, 2);
    Battle_WaitMode0_21(20);
    Motion_CallWaitAnim_2(3, 4);
    Battle_WaitMode0_22(20);
    BattleEv_RunWait_14(3, 0);
    Battle_WaitMode0_23(30);
    BattleFx_SpawnLinked_7(8, 0x106, 40);
    BattleEv_RunWait_15(8, 0);
    Battle_WaitMode0_24(10);
    BattleFx_SpawnLinked_8(0, 0x101, 0);
    BattleFx_SpawnLinked_9(1, 0x101, 0);
    BattleFx_SpawnLinked_10(3, 0x101, 0);
    BattleFx_SpawnLinked_11(2, 0x101, 0);
    Battle_WaitMode0_25(60);
    Motion_SetVarCbObj_5(2, 2);
    Battle_WaitMode0_26(20);
    BattleEv_RunWait_16(2, 0);
    Battle_WaitMode0_27(20);
    BattleEv_RunWait_17(8, 0);
    Battle_WaitMode0_28(10);
    BattleFx_SpawnLinked_12(1, 0x102, 40);
    BattleEv_RunWait_18(1, 0);
    Battle_WaitMode0_29(10);
    Motion_SetVarCbObj_6(8, 2);
    Battle_WaitMode0_30(20);
    BattleEv_RunWait_19(8, 0);
    Motion_SetVarCbObj_7(3, 2);
    Battle_WaitMode0_31(20);
    BattleEv_RunWait_20(3, 0);
    Battle_WaitMode0_32(10);
    BattleFx_SpawnLinked_13(8, 0x102, 40);
    Motion_SetSpeed_2(8, 0); /* main:0808a178 */
    Motion_SetAngleToward_2(1, 0, 0);
    Motion_SetAngleToward_3(2, 0, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        SceneWork_SetStepValue_2(0x2164);
        Battle_WaitMode0_33(20);
        BattleFx_SpawnLinked_14(1, 0x100, 40);
        Motion_SetSpeed_3(1, 0x20000, 0x10000);
        Motion_CommitPositionAndActivate_1(1, 0, -16);
        Battle_WaitMode0_34(10);
        Object_LinkPair_3(1, 0, 30);
        BattleEv_RunWait_21(1, 0);
    } else {
        SceneWork_SetStepValue_3(0x2168);
        Battle_WaitMode0_35(10);
        Motion_SetSpeed_4(1, 0x10000, 0x8000);
        Motion_CommitPositionAndActivate_2(1, 0, -16);
        Battle_WaitMode0_36(10);
        Object_LinkPair_4(1, 0, 30);
        Motion_CallWaitAnim_3(1, 3);
        Battle_WaitMode0_37(20);
        BattleEv_RunWait_22(1, 0);
    }

    line = 0x2165;
    SceneWork_SetStepValue_4(line);
    BattleFx_SpawnLinked_15(2, 0x103, 40);
    Motion_SetSpeed_5(2, 0x20000, 0x10000);
    Motion_CommitPositionAndActivate_3(2, 0, -16);
    Battle_WaitMode0_38(10);
    Object_LinkPair_5(2, 0, 30);
    BattleEv_RunWait_23(2, 0);
    Motion_CallWaitAnim_4(3, 3);
    Battle_WaitMode0_39(10);
    Motion_CallWaitAnim_5(3, 3);
    Battle_WaitMode0_40(20);
    Motion_SetAngleToward_4(0, 3, 0);
    Motion_SetAngleToward_5(1, 3, 0);
    Motion_SetAngleToward_6(2, 3, 0);
    Battle_WaitMode0_41(20);
    BattleEv_RunWait_24(3, 0);
    Battle_WaitMode0_42(20);
    Motion_SetSpeed_6(1, 0x10000, 0x8000);
    Motion_CommitPositionAndActivate_4(1, 0, 16);
    Battle_WaitMode0_43(30);
    Motion_CallWaitAnim_6(1, 4);
    Battle_WaitMode0_44(30);
    Motion_SetAngleToward_7(1, 0, 0);
    Battle_WaitMode0_45(20);
    BattleEv_RunWait_25(1, 0);
    Battle_WaitMode0_46(30);
    Object_LinkPair_6(3, 2, 30);
    Motion_CallWaitAnim_7(2, 3);
    Motion_CallWaitAnim_8(3, 3);
    Battle_WaitMode0_47(30);
    Motion_SetSpeed_7(2, 0x10000, 0x8000);
    Motion_CommitPositionAndActivate_5(2, 0, 16);
    Battle_WaitMode0_48(10);
    Motion_ArmCb_7(0, 0xc000, 0);
    Motion_ArmCb_8(1, 0xc000, 0);
    Motion_ArmCb_9(3, 0xc000, 0);
    Motion_ArmCb_10(2, 0xc000, 0);
    line += 7;
    SceneWork_SetStepValue_5(line);
    Battle_WaitMode0_49(30);
    BattleFx_SpawnLinked_16(8, 0x100, 40);
    BattleEv_RunWait_26(8, 0);
    Battle_WaitMode0_50(10);
    Motion_CallWaitAnim_9(2, 3);
    Battle_WaitMode0_51(30);
    BattleEv_RunWait_27(2, 0);
    Battle_WaitMode0_52(10);
    BattleFx_SpawnLinked_17(8, 0x108, 40);
    BattleEv_RunWait_28(8, 0);
    Battle_WaitMode0_53(10);
    Motion_CallWaitAnim_10(2, 3);
    Battle_WaitMode0_54(30);
    BattleEv_RunWait_29(8, 0);
    Battle_WaitMode0_55(10);
    Motion_CallWaitAnim_11(3, 3);
    Battle_WaitMode0_56(30);
    BattleEv_RunWait_30(8, 0);
    Battle_WaitMode0_57(10);
    Object_LinkPair_7(1, 0, 50);
    Motion_ArmCb_11(0, 0xc000, 0);
    Motion_ArmCb_12(1, 0xc000, 0);
    Battle_WaitMode0_58(30);
    BattleEv_RunWait_31(8, 0);
    Battle_WaitMode0_59(10);
    BattleFx_SpawnLinked_18(3, 0x101, 40);
    BattleEv_RunWait_32(3, 0);
    Battle_WaitMode0_60(30);
    BattleEv_RunWait_33(8, 0);
    Battle_WaitMode0_61(10);
    Object_LinkPair_8(1, 0, 50);
    Motion_ArmCb_13(0, 0xc000, 0);
    Motion_ArmCb_14(1, 0xc000, 0);
    Battle_WaitMode0_62(30);
    BattleEv_RunWait_34(8, 0);
    Battle_WaitMode0_63(10);
    Object_LinkPair_9(3, 2, 50);
    Motion_ArmCb_15(3, 0xc000, 0);
    Motion_ArmCb_16(2, 0xc000, 0);
    Battle_WaitMode0_64(30);
    BattleEv_RunWait_35(8, 0);
    Battle_WaitMode0_65(10);
    Motion_SetVarCbObj_8(1, 2);
    Battle_WaitMode0_66(20);
    BattleEv_RunWait_36(1, 0);
    Battle_WaitMode0_67(30);
    BattleEv_RunWait_37(8, 0);
    Battle_WaitMode0_68(10);
    Motion_SetVarCbObj_9(2, 2);
    Battle_WaitMode0_69(20);
    BattleEv_RunWait_38(2, 0);
    Battle_WaitMode0_70(20);
    BattleFx_SpawnLinked_19(8, 0x108, 50);
    BattleEv_RunWait_39(8, 0);
    Battle_WaitMode0_71(10);
    BattleFx_SpawnLinked_20(1, 0x101, 40);
    BattleEv_RunWait_40(1, 0);
    Battle_WaitMode0_72(10);
    BattleFx_SpawnLinked_21(8, 0x102, 40);
    BattleEv_RunWait_41(8, 0);
    Motion_SetVarCbObj_10(8, 2);
    Battle_WaitMode0_73(20);
    Battle_WaitMode0_74(10);
    Motion_CallWaitAnim_12(3, 4);
    Battle_WaitMode0_75(20);
    BattleEv_RunWait_42(3, 0);
    Battle_WaitMode0_76(10);
    Motion_SetAngleToward_8(0, 3, 40);
    Motion_CallWaitAnim_13(0, 3);
    Battle_WaitMode0_77(30);
    Motion_SetAngleToward_9(1, 3, 0);
    Motion_SetAngleToward_10(2, 3, 0);
    Battle_WaitMode0_78(20);
    Object_SetModeById_1(1, 3);
    Motion_CallWaitAnim_14(2, 3);
    Battle_WaitMode0_79(30);
    Audio_PlayCue_2(17);
    Motion_SetSpeed_8(1, 0x13333, 0x9999);
    Motion_SetSpeed_9(2, 0x13333, 0x9999);
    Motion_SetSpeed_10(3, 0x13333, 0x9999);
    Object_SetModeById_2(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Motion_CommitPos_2(1);
    Motion_SetHPosTerrain_1(1, 0, 0);
    Object_SetModeById_3(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Motion_CommitPos_3(2);
    Motion_SetHPosTerrain_2(2, 0, 0);
    Object_SetModeById_4(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Motion_CommitPos_4(3);
    Motion_SetHPosTerrain_3(3, 0, 0);
    Audio_PlayCueForPartyMember_1();
    Battle_SchedShoulder_1();
}

void Scene_RunFlagGatedActorEightDialogue(void)
{
    if (Field_TestFlag960(0x960) == 0)
        return;
    if (Field_TestFlag962(0x962) != 0)
        return;

    Field_TestFlag961(0x961);
    Field_unk_020071e8();
    Field_unk_0200729e(0x217d);
    Field_unk_020072b6(8, 0);
    Field_unk_020071f4(10);
    Field_unk_02007294(0, 2);
    Field_unk_02007202(30);
    Field_unk_020072ac(0, 8, 0);
    Field_unk_02007212(30);
    Field_unk_020072e2(8, 0);
    Field_unk_020072a2(0, 3);
    Field_unk_02007228(20);
    Field_unk_0200723c();
}

/* overlays/scene/field/extended_presentation/scene_setup.c */
typedef struct {
    s32 a, b, c, d, e, f;
} S6;

extern u8 Value_0000217f;

/* Contiguous unnamed leaf-owner run for resource_3bd. */

void Scene_RunIndexedStep0(void)
{
    Field_unk_020015aa(0);
}

void Scene_RunIndexedStep1(void)
{
    Field_unk_020015b6(1);
}

void Scene_RunIndexedStep2(void)
{
    Field_unk_020015c2(2);
}

void Scene_RunIndexedStep3(void)
{
    Field_unk_020015ce_a(3);
}

void Scene_RunIndexedStep4(void)
{
    Field_unk_020015da(4);
}

void Scene_RunIndexedStep5(void)
{
    Field_unk_020015e6(5);
}

void Scene_RunTwoCallSequence(void)
{
    Field_unk_02000e10();
    Field_unk_02001a48();
}

void Scene_RunGuardedSixWordStep(void)
{
    S6 s;

    Field_unk_02004b62();
    if (Field_unk_0200132e(&s) != 0) {
        Field_unk_020014da(s);
    }
    Field_unk_02004b8e();
}

void Scene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Field_unk_02007170();
    val = (s32)&Value_0000217f;
    Field_unk_02007228_a(val);
    Field_unk_02007238(8, 0);
    if (Field_unk_02007198(0, 0) == 0) {
        Field_unk_0200718a(20);
        Field_unk_02007248(val + 1);
        Field_unk_02007260(8, 0);
    } else {
        Field_unk_020071a0(20);
        Field_unk_0200725e(val + 2);
        Field_unk_02007276(8, 0);
    }
    Field_unk_020071c2();
}

/* overlays/scene/field/extended_presentation/scene_state_interaction.c */
extern s16 RuntimeSelectorTable;
extern u8 Value_00000097;

void State_RunFlag200SetupAndPlaceActors16To20(void)
{
    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (Field_TestFlag200(0x200) != 0) {
        Field_unk_02001770();
        work[0x34] = 1;
    }
    tbl = &RuntimeSelectorTable;
    if (tbl[0xe0] == (s32)&Value_00000097) {
        Field_unk_020049ba(16, 6);
        Field_unk_020049c2(17, 6);
        Field_unk_020049ca(18, 6);
        Field_unk_020049d2(19, 6);
        Field_unk_020049da(20, 6);
    }
}

/* overlays/scene/field/extended_presentation/staged_actor.c */
typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ent;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec;

Ent *Field_unk_02004a2c(s32);

u8 *Field_unk_02004a64(s32);

/* Contiguous unnamed state-owner run for resource_3bd. */

void State_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    Ent *p = Field_unk_02004a2c(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Field_EntOp2(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void Scene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Field_unk_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Field_SetRect(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        Field_TestFlag211(0x211);
    }
}

/* overlays/scene/field/extended_presentation/staged_actor_pair.c */

extern u32 ActorSearchStep[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void StagedActor_AdvancePair(void)
{
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    dir = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = rate;
    next_actor->move_rate_z = rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = rate;
    lead_actor->move_rate_z = rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

/* overlays/scene/field/extended_presentation/state_update.c */
extern u8 gOv13;

void State_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &gOv13;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Field_unk_02001528();
    }
}
