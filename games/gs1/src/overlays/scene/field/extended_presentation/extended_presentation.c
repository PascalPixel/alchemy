#include "types.h"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "staged_actor.h"

/* overlays/scene/field/extended_presentation/actor_movement_effect.c */
#define Actor_ResetMotionIfBlockedAhead Func_020002a8
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

extern s32 Data_0200beb4[];
extern s32 Data_0200bef4[];

Ent *Func_02000342(Desc *, Ent *);
s32 Func_02003f7a(Ent *, Desc *);

s32 Actor_ResetMotionIfBlockedAhead(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200beb4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200bef4;

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
    m = Data_0200beb4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_02003f7a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}
#undef Actor_ResetMotionIfBlockedAhead

/* overlays/scene/field/extended_presentation/actor_position.c */
#define Actor_SetPositionFromTransformedBase Func_02000f10
u8 *Func_02004bfa(s32);
void Func_02004b68(s32, s32, s32 *);

void Actor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = Func_02004bfa(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Func_02004b68(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}
#undef Actor_SetPositionFromTransformedBase

/* overlays/scene/field/extended_presentation/actor_search.c */
#define TestActorPosition Func_020041b4
#define ActorProbeOffsets Data_0200bf0c
#define ActorSearchStep Data_0200beb4
#define Actor_FindClearActorPosition Func_02000474


s32 Actor_FindClearActorPosition(s32 *a)
{
}
#undef TestActorPosition
#undef ActorProbeOffsets
#undef ActorSearchStep
#undef Actor_FindClearActorPosition

/* overlays/scene/field/extended_presentation/display.c */
#define Effect_SetupBlendByFlag201 Func_02000b30
void Func_0200474c(s32);
void Func_0200490a(s32);
s32 Func_020047ea(s32);
void Func_020015ce(void);

void Effect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Func_0200474c(1);
    Func_0200490a(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (Func_020047ea(0x201) != 0) {
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
        Func_020015ce();
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
#undef Effect_SetupBlendByFlag201

/* overlays/scene/field/extended_presentation/effect.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define Actor_PlaceFiveActorsInRow Func_02000f6c
#define Effect_UpdateStateMachine Func_02000f94
#define OvObj_UpdateThreeStateMotion Func_02003a30

extern u16 *Data_0200bf6c;
extern s8 Data_02001002;
extern u8 Data_02008ee1;

void Func_02001e8e(s32, s32, u8 *);
s32 Func_02004be0(s32, s32);
u8 *Func_02004cea(s32);
void Func_02001fb2(s32);
void Func_02004ed6(s32);
s32 Func_02007670(void);
void Func_02007684(s32 first, s32 second, s32 *position);
s32 Func_02007898(void *object);
s32 Func_020078b0(void *object);
void Func_020078da(void *object);
void Func_02007904(s32 value);

/* Contiguous unnamed state-owner run for resource_3bd. */

void Actor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        Func_02001e8e(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}

void Effect_UpdateStateMachine(void)
{
    u16 *p = Data_0200bf6c;
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
        s32 v = Data_02001002;
        s32 r = Func_02004be0(v << 16, 5);
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
            o = Func_02004cea(v + 11);
            *(s32 *)(o + 0x6c) = (s32)&Data_02008ee1;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        Data_0200bf6c[3] += Data_0200bf6c[4];
        Func_02001fb2(Data_0200bf6c[3]);
        q2 = Data_0200bf6c;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Func_02004ed6(0x87);
            }
        }
    }
    {
        u16 *q = Data_0200bf6c;
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
        Func_02007684(0x780000, Func_02007670(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Func_02007904(0x86);
    } else if (state == 1) {
        if (Func_02007898(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (Func_020078b0(obj) == 0)
            Func_020078da(obj);
    }
}
#undef FIELD
#undef Actor_PlaceFiveActorsInRow
#undef Effect_UpdateStateMachine
#undef OvObj_UpdateThreeStateMotion

/* overlays/scene/field/extended_presentation/move_and_redraw.c */
#define Data_0200e1e8 Data_0200bf0c
#define Data_0200e190 Data_0200beb4
#define Func_02006610 Func_020042f4
#define Func_0200661c Func_02004300
#define Func_020066b4 Func_02004398
#define Func_02006714 Func_020043f0
#define Func_0200668a Func_0200437e
#define Func_0200672c Func_02004408
#define Func_020066ea Func_020043ce
#define Func_020066c4 Func_020043b8
#define RefreshStagedActor Func_020043e0
#define Func_02006614 Func_02004348
#define Func_0200661e Func_02004352
#define Func_0200687c Func_020045a0
#define Func_02006658 Func_02004374
#define Func_02006776 Func_02004452
#define Func_0200678e Func_0200446a
#define Func_02006740 Func_02004424
#define Func_020067a4 Func_02004480
#define Func_020067bc Func_02004498
#define Func_020067d4 Func_020044b0
#define Func_0200677a Func_0200445e
#define Func_020068f4 Func_02004618
#define Func_020068fa Func_0200461e
#define Func_020066b8 Func_020043ec
#define Func_02006752 Func_0200445e_a
#define Func_020067ae Func_020044ba
#define Func_020069d0 Func_020046e4



void Func_02000608(StagedActorMovementRequest request)
{
}
#undef Data_0200e1e8
#undef Data_0200e190
#undef Func_02006610
#undef Func_0200661c
#undef Func_020066b4
#undef Func_02006714
#undef Func_0200668a
#undef Func_0200672c
#undef Func_020066ea
#undef Func_020066c4
#undef RefreshStagedActor
#undef Func_02006614
#undef Func_0200661e
#undef Func_0200687c
#undef Func_02006658
#undef Func_02006776
#undef Func_0200678e
#undef Func_02006740
#undef Func_020067a4
#undef Func_020067bc
#undef Func_020067d4
#undef Func_0200677a
#undef Func_020068f4
#undef Func_020068fa
#undef Func_020066b8
#undef Func_02006752
#undef Func_020067ae
#undef Func_020069d0

/* overlays/scene/field/extended_presentation/scene_data.c */
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097
#define PrimaryOverlayData Data_0200c1b0
#define SecondaryOverlayData Data_0200c270
#define TertiaryOverlayData Data_0200c318
#define DefaultOverlayData Data_0200c198
#define SceneData_GetTablebf70 Func_020009ec
#define SceneData_ReturnZero Func_020009f4
#define SceneData_GetTablec138 Func_020009f8
#define SceneData_SelectDataByRuntimeSelector Func_02000a00
#define SceneData_SelectTableBySceneId Func_02003598


extern u8 Data_0200bf70[];
extern u8 Data_0200c138[];
extern s16 Data_02000240[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 Data_0200c688[];
extern u8 Data_0200c724[];
extern u8 Data_0200c76c[];
extern u8 Data_0200c808[];
extern u8 Data_0200c850[];
extern u8 Data_0200c5e0[];

/* Contiguous unnamed leaf-owner run for resource_3bd. */

u8 *SceneData_GetTablebf70(void)
{
    return Data_0200bf70;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    return Data_0200c138;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
}

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}
#undef RuntimeSelectorTable
#undef PrimaryRuntimeSelector
#undef SecondaryRuntimeSelector
#undef TertiaryRuntimeSelector
#undef PrimaryOverlayData
#undef SecondaryOverlayData
#undef TertiaryOverlayData
#undef DefaultOverlayData
#undef SceneData_GetTablebf70
#undef SceneData_ReturnZero
#undef SceneData_GetTablec138
#undef SceneData_SelectDataByRuntimeSelector
#undef SceneData_SelectTableBySceneId

/* overlays/scene/field/extended_presentation/scene_primary_script.c */
#define GameFlag_Set_1(a0) Value1(Func_020068dc, a0)
#define Audio_PlayCue_1(a0) Value1(Func_02006ac2, a0)
#define Battle_Reset_1() Value0(Func_02006906)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020069c0, a0)
#define Motion_SetVarCbObj_1(a0, a1) Value2(Func_020069a8, a0, a1)
#define Battle_WaitMode0_1(a0) Value1(Func_02006916, a0)
#define BattleEv_RunWait_1(a0, a1) Value2(Func_020069e6, a0, a1)
#define Battle_WaitMode0_2(a0) Value1(Func_02006924, a0)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02006a26, a0, a1, a2, a3)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02006968, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_020069a2, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02006a26_a, a0, a1, a2)
#define Motion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02006af6, a0, a1, a2, a3)
#define Motion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02006b04, a0, a1, a2, a3)
#define Motion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Value4(Func_02006b12, a0, a1, a2, a3)
#define Motion_CommitPos_1(a0) Value1(Func_020069f0, a0)
#define Battle_WaitMode0_3(a0) Value1(Func_0200698e, a0)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Value3(Func_02006a72, a0, a1, a2)
#define Battle_WaitMode0_4(a0) Value1(Func_020069a0, a0)
#define BattleEv_RunWait_2(a0, a1) Call2(Func_02006a70, a0, a1)
#define Motion_SetVarCbObj_2(a0, a1) Value2(Func_02006a48, a0, a1)
#define Battle_WaitMode0_5(a0) Value1(Func_020069b6, a0)
#define BattleEv_RunWait_3(a0, a1) Value2(Func_02006a86, a0, a1)
#define Battle_WaitMode0_6(a0) Value1(Func_020069c4, a0)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02006aa8, a0, a1, a2)
#define BattleEv_RunWait_4(a0, a1) Value2(Func_02006aa0, a0, a1)
#define Battle_WaitMode0_7(a0) Value1(Func_020069de, a0)
#define Object_LinkPair_1(a0, a1, a2) Call3(Func_02006a90, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02006ac4, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Value3(Func_02006ad0, a0, a1, a2)
#define Battle_WaitMode0_8(a0) Value1(Func_02006a06, a0)
#define BattleEv_RunWait_5(a0, a1) Value2(Func_02006ad6, a0, a1)
#define Battle_WaitMode0_9(a0) Value1(Func_02006a14, a0)
#define Battle_WaitMode0_10(a0, a1) Call2(Func_02006afe, a0, a1)
#define Motion_SetVarCbObj_3(a0, a1) Value2(Func_02006abe, a0, a1)
#define Battle_WaitMode0_11(a0) Value1(Func_02006a2c, a0)
#define BattleEv_RunWait_6(a0, a1) Value2(Func_02006afc, a0, a1)
#define Battle_WaitMode0_12(a0) Value1(Func_02006a3a, a0)
#define Object_LinkPair_2(a0, a1, a2) Call3(Func_02006aec, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02006b20, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Value3(Func_02006b2c, a0, a1, a2)
#define Battle_WaitMode0_13(a0) Value1(Func_02006a62, a0)
#define BattleEv_RunWait_7(a0, a1) Value2(Func_02006b32, a0, a1)
#define Battle_WaitMode0_14(a0) Call1(Func_02006a70_a, a0)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02006b54, a0, a1, a2)
#define BattleEv_RunWait_8(a0, a1) Value2(Func_02006b4c, a0, a1)
#define Battle_WaitMode0_15(a0) Value1(Func_02006a8a, a0)
#define Motion_CallWaitAnim_1(a0, a1) Call2(Func_02006b12_a, a0, a1)
#define Battle_WaitMode0_16(a0) Value1(Func_02006a98, a0)
#define BattleEv_RunWait_9(a0, a1) Value2(Func_02006b68, a0, a1)
#define Battle_WaitMode0_17(a0) Value1(Func_02006aa6, a0)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02006b88, a0, a1, a2)
#define BattleEv_RunWait_10(a0, a1) Value2(Func_02006b80, a0, a1)
#define Battle_WaitMode0_18(a0) Call1(Func_02006abe_a, a0)
#define Motion_SetAngleToward_1(a0, a1, a2) Call3(Func_02006b68_a, a0, a1, a2)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_02006baa, a0, a1, a2)
#define BattleEv_RunWait_11(a0, a1) Call2(Func_02006ba2, a0, a1)
#define Motion_ArmCb_6(a0, a1, a2) Value3(Func_02006bb6, a0, a1, a2)
#define Battle_WaitMode0_19(a0) Call1(Func_02006aec_a, a0)
#define BattleEv_RunWait_12(a0, a1) Value2(Func_02006bbc, a0, a1)
#define Battle_WaitMode0_20(a0) Value1(Func_02006afa, a0)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_02006bde, a0, a1, a2)
#define BattleEv_RunWait_13(a0, a1) Call2(Func_02006bd6, a0, a1)
#define Motion_SetVarCbObj_4(a0, a1) Value2(Func_02006bae, a0, a1)
#define Battle_WaitMode0_21(a0) Value1(Func_02006b1c, a0)
#define Motion_CallWaitAnim_2(a0, a1) Value2(Func_02006ba4, a0, a1)
#define Battle_WaitMode0_22(a0) Value1(Func_02006b2a, a0)
#define BattleEv_RunWait_14(a0, a1) Value2(Func_02006bfa, a0, a1)
#define Battle_WaitMode0_23(a0) Value1(Func_02006b38, a0)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_02006c1c, a0, a1, a2)
#define BattleEv_RunWait_15(a0, a1) Value2(Func_02006c14, a0, a1)
#define Battle_WaitMode0_24(a0) Value1(Func_02006b52, a0)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Call3(Func_02006c34, a0, a1, a2)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Call3(Func_02006c3e, a0, a1, a2)
#define BattleFx_SpawnLinked_10(a0, a1, a2) Call3(Func_02006c48, a0, a1, a2)
#define BattleFx_SpawnLinked_11(a0, a1, a2) Value3(Func_02006c52, a0, a1, a2)
#define Battle_WaitMode0_25(a0) Call1(Func_02006b80_a, a0)
#define Motion_SetVarCbObj_5(a0, a1) Value2(Func_02006c20, a0, a1)
#define Battle_WaitMode0_26(a0) Value1(Func_02006b8e, a0)
#define BattleEv_RunWait_16(a0, a1) Value2(Func_02006c5e, a0, a1)
#define Battle_WaitMode0_27(a0) Value1(Func_02006b9c, a0)
#define BattleEv_RunWait_17(a0, a1) Value2(Func_02006c6c, a0, a1)
#define Battle_WaitMode0_28(a0) Call1(Func_02006baa_a, a0)
#define BattleFx_SpawnLinked_12(a0, a1, a2) Call3(Func_02006c8e, a0, a1, a2)
#define BattleEv_RunWait_18(a0, a1) Value2(Func_02006c86, a0, a1)
#define Battle_WaitMode0_29(a0) Value1(Func_02006bc4, a0)
#define Motion_SetVarCbObj_6(a0, a1) Value2(Func_02006c64, a0, a1)
#define Battle_WaitMode0_30(a0) Value1(Func_02006bd2, a0)
#define BattleEv_RunWait_19(a0, a1) Call2(Func_02006ca2, a0, a1)
#define Motion_SetVarCbObj_7(a0, a1) Value2(Func_02006c7a, a0, a1)
#define Battle_WaitMode0_31(a0) Value1(Func_02006be8, a0)
#define BattleEv_RunWait_20(a0, a1) Value2(Func_02006cb8, a0, a1)
#define Battle_WaitMode0_32(a0) Value1(Func_02006bf6, a0)
#define BattleFx_SpawnLinked_13(a0, a1, a2) Call3(Func_02006cda, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1) Value2(Func_02006cca, a0, a1)
#define Motion_SetAngleToward_2(a0, a1, a2) Call3(Func_02006cb4, a0, a1, a2)
#define Motion_SetAngleToward_3(a0, a1, a2) Call3(Func_02006cbe, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006c3e_a, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02006ce8, a0)
#define Battle_WaitMode0_33(a0) Value1(Func_02006c36, a0)
#define BattleFx_SpawnLinked_14(a0, a1, a2) Call3(Func_02006d1a, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02006c78, a0, a1, a2)
#define Motion_CommitPositionAndActivate_1(a0, a1, a2) Value3(Func_02006dfc, a0, a1, a2)
#define Battle_WaitMode0_34(a0) Value1(Func_02006c62, a0)
#define Object_LinkPair_3(a0, a1, a2) Call3(Func_02006d14, a0, a1, a2)
#define BattleEv_RunWait_21(a0, a1) Value2(Func_02006d3c, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Func_02006d4c, a0)
#define Battle_WaitMode0_35(a0) Value1(Func_02006c9a, a0)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02006cd0, a0, a1, a2)
#define Motion_CommitPositionAndActivate_2(a0, a1, a2) Value3(Func_02006e54, a0, a1, a2)
#define Battle_WaitMode0_36(a0) Value1(Func_02006cba, a0)
#define Object_LinkPair_4(a0, a1, a2) Call3(Func_02006d6c, a0, a1, a2)
#define Motion_CallWaitAnim_3(a0, a1) Call2(Func_02006d4c_a, a0, a1)
#define Battle_WaitMode0_37(a0) Value1(Func_02006cd2, a0)
#define BattleEv_RunWait_22(a0, a1) Value2(Func_02006da2, a0, a1)
#define SceneWork_SetStepValue_4(a0) Value1(Func_02006d9a, a0)
#define BattleFx_SpawnLinked_15(a0, a1, a2) Call3(Func_02006dc4, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_02006d22, a0, a1, a2)
#define Motion_CommitPositionAndActivate_3(a0, a1, a2) Value3(Func_02006ea6, a0, a1, a2)
#define Battle_WaitMode0_38(a0) Value1(Func_02006d0c, a0)
#define Object_LinkPair_5(a0, a1, a2) Call3(Func_02006dbe, a0, a1, a2)
#define BattleEv_RunWait_23(a0, a1) Call2(Func_02006de6, a0, a1)
#define Motion_CallWaitAnim_4(a0, a1) Value2(Func_02006da6, a0, a1)
#define Battle_WaitMode0_39(a0) Value1(Func_02006d2c, a0)
#define Motion_CallWaitAnim_5(a0, a1) Value2(Func_02006db4, a0, a1)
#define Battle_WaitMode0_40(a0) Value1(Func_02006d3a, a0)
#define Motion_SetAngleToward_4(a0, a1, a2) Call3(Func_02006de4, a0, a1, a2)
#define Motion_SetAngleToward_5(a0, a1, a2) Call3(Func_02006dee, a0, a1, a2)
#define Motion_SetAngleToward_6(a0, a1, a2) Value3(Func_02006df8, a0, a1, a2)
#define Battle_WaitMode0_41(a0) Value1(Func_02006d5e, a0)
#define BattleEv_RunWait_24(a0, a1) Value2(Func_02006e2e, a0, a1)
#define Battle_WaitMode0_42(a0) Call1(Func_02006d6c_a, a0)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_02006da2_a, a0, a1, a2)
#define Motion_CommitPositionAndActivate_4(a0, a1, a2) Value3(Func_02006f24, a0, a1, a2)
#define Battle_WaitMode0_43(a0) Value1(Func_02006d8a, a0)
#define Motion_CallWaitAnim_6(a0, a1) Value2(Func_02006e12, a0, a1)
#define Battle_WaitMode0_44(a0) Value1(Func_02006d98, a0)
#define Motion_SetAngleToward_7(a0, a1, a2) Value3(Func_02006e42, a0, a1, a2)
#define Battle_WaitMode0_45(a0) Value1(Func_02006da8, a0)
#define BattleEv_RunWait_25(a0, a1) Value2(Func_02006e78, a0, a1)
#define Battle_WaitMode0_46(a0) Value1(Func_02006db6, a0)
#define Object_LinkPair_6(a0, a1, a2) Call3(Func_02006e68, a0, a1, a2)
#define Motion_CallWaitAnim_7(a0, a1) Call2(Func_02006e48, a0, a1)
#define Motion_CallWaitAnim_8(a0, a1) Call2(Func_02006e50, a0, a1)
#define Battle_WaitMode0_47(a0) Value1(Func_02006dd6, a0)
#define Motion_SetSpeed_7(a0, a1, a2) Call3(Func_02006e0c, a0, a1, a2)
#define Motion_CommitPositionAndActivate_5(a0, a1, a2) Value3(Func_02006f8e, a0, a1, a2)
#define Battle_WaitMode0_48(a0) Value1(Func_02006df4, a0)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02006ed0, a0, a1, a2)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_02006edc, a0, a1, a2)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_02006ee8, a0, a1, a2)
#define Motion_ArmCb_10(a0, a1, a2) Value3(Func_02006ef6, a0, a1, a2)
#define SceneWork_SetStepValue_5(a0) Value1(Func_02006ee4, a0)
#define Battle_WaitMode0_49(a0) Value1(Func_02006e32, a0)
#define BattleFx_SpawnLinked_16(a0, a1, a2) Call3(Func_02006f16, a0, a1, a2)
#define BattleEv_RunWait_26(a0, a1) Value2(Func_02006f0e, a0, a1)
#define Battle_WaitMode0_50(a0) Value1(Func_02006e4c, a0)
#define Motion_CallWaitAnim_9(a0, a1) Value2(Func_02006ed4, a0, a1)
#define Battle_WaitMode0_51(a0) Value1(Func_02006e5a, a0)
#define BattleEv_RunWait_27(a0, a1) Value2(Func_02006f2a, a0, a1)
#define Battle_WaitMode0_52(a0) Call1(Func_02006e68_a, a0)
#define BattleFx_SpawnLinked_17(a0, a1, a2) Call3(Func_02006f4c, a0, a1, a2)
#define BattleEv_RunWait_28(a0, a1) Value2(Func_02006f44, a0, a1)
#define Battle_WaitMode0_53(a0) Value1(Func_02006e82, a0)
#define Motion_CallWaitAnim_10(a0, a1) Value2(Func_02006f0a, a0, a1)
#define Battle_WaitMode0_54(a0) Value1(Func_02006e90, a0)
#define BattleEv_RunWait_29(a0, a1) Value2(Func_02006f60, a0, a1)
#define Battle_WaitMode0_55(a0) Value1(Func_02006e9e, a0)
#define Motion_CallWaitAnim_11(a0, a1) Value2(Func_02006f26, a0, a1)
#define Battle_WaitMode0_56(a0) Value1(Func_02006eac, a0)
#define BattleEv_RunWait_30(a0, a1) Value2(Func_02006f7c, a0, a1)
#define Battle_WaitMode0_57(a0) Value1(Func_02006eba, a0)
#define Object_LinkPair_7(a0, a1, a2) Call3(Func_02006f6c, a0, a1, a2)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02006fa0, a0, a1, a2)
#define Motion_ArmCb_12(a0, a1, a2) Value3(Func_02006fac, a0, a1, a2)
#define Battle_WaitMode0_58(a0) Value1(Func_02006ee2, a0)
#define BattleEv_RunWait_31(a0, a1) Value2(Func_02006fb2, a0, a1)
#define Battle_WaitMode0_59(a0) Value1(Func_02006ef0, a0)
#define BattleFx_SpawnLinked_18(a0, a1, a2) Call3(Func_02006fd2, a0, a1, a2)
#define BattleEv_RunWait_32(a0, a1) Value2(Func_02006fca, a0, a1)
#define Battle_WaitMode0_60(a0) Value1(Func_02006f08, a0)
#define BattleEv_RunWait_33(a0, a1) Value2(Func_02006fd8, a0, a1)
#define Battle_WaitMode0_61(a0) Call1(Func_02006f16_a, a0)
#define Object_LinkPair_8(a0, a1, a2) Call3(Func_02006fc8, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02006ffc, a0, a1, a2)
#define Motion_ArmCb_14(a0, a1, a2) Value3(Func_02007008, a0, a1, a2)
#define Battle_WaitMode0_62(a0) Value1(Func_02006f3e, a0)
#define BattleEv_RunWait_34(a0, a1) Value2(Func_0200700e, a0, a1)
#define Battle_WaitMode0_63(a0) Call1(Func_02006f4c_a, a0)
#define Object_LinkPair_9(a0, a1, a2) Call3(Func_02006ffe, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_02007032, a0, a1, a2)
#define Motion_ArmCb_16(a0, a1, a2) Value3(Func_0200703e, a0, a1, a2)
#define Battle_WaitMode0_64(a0) Value1(Func_02006f74, a0)
#define BattleEv_RunWait_35(a0, a1) Value2(Func_02007044, a0, a1)
#define Battle_WaitMode0_65(a0) Value1(Func_02006f82, a0)
#define Motion_SetVarCbObj_8(a0, a1) Value2(Func_02007022, a0, a1)
#define Battle_WaitMode0_66(a0) Value1(Func_02006f90, a0)
#define BattleEv_RunWait_36(a0, a1) Value2(Func_02007060, a0, a1)
#define Battle_WaitMode0_67(a0) Value1(Func_02006f9e, a0)
#define BattleEv_RunWait_37(a0, a1) Value2(Func_0200706e, a0, a1)
#define Battle_WaitMode0_68(a0) Call1(Func_02006fac_a, a0)
#define Motion_SetVarCbObj_9(a0, a1) Value2(Func_0200704c, a0, a1)
#define Battle_WaitMode0_69(a0) Value1(Func_02006fba, a0)
#define BattleEv_RunWait_38(a0, a1) Value2(Func_0200708a, a0, a1)
#define Battle_WaitMode0_70(a0) Call1(Func_02006fc8_a, a0)
#define BattleFx_SpawnLinked_19(a0, a1, a2) Call3(Func_020070ac, a0, a1, a2)
#define BattleEv_RunWait_39(a0, a1) Value2(Func_020070a4, a0, a1)
#define Battle_WaitMode0_71(a0) Value1(Func_02006fe2, a0)
#define BattleFx_SpawnLinked_20(a0, a1, a2) Call3(Func_020070c4, a0, a1, a2)
#define BattleEv_RunWait_40(a0, a1) Value2(Func_020070bc, a0, a1)
#define Battle_WaitMode0_72(a0) Value1(Func_02006ffa, a0)
#define BattleFx_SpawnLinked_21(a0, a1, a2) Call3(Func_020070de, a0, a1, a2)
#define BattleEv_RunWait_41(a0, a1) Call2(Func_020070d6, a0, a1)
#define Motion_SetVarCbObj_10(a0, a1) Value2(Func_020070ae, a0, a1)
#define Battle_WaitMode0_73(a0) Value1(Func_0200701c, a0)
#define Battle_WaitMode0_74(a0) Call1(Func_02007022_a, a0)
#define Motion_CallWaitAnim_12(a0, a1) Value2(Func_020070aa, a0, a1)
#define Battle_WaitMode0_75(a0) Value1(Func_02007030, a0)
#define BattleEv_RunWait_42(a0, a1) Value2(Func_02007100, a0, a1)
#define Battle_WaitMode0_76(a0) Call1(Func_0200703e_a, a0)
#define Motion_SetAngleToward_8(a0, a1, a2) Call3(Func_020070e8, a0, a1, a2)
#define Motion_CallWaitAnim_13(a0, a1) Value2(Func_020070d0, a0, a1)
#define Battle_WaitMode0_77(a0) Value1(Func_02007056, a0)
#define Motion_SetAngleToward_9(a0, a1, a2) Call3(Func_02007100_a, a0, a1, a2)
#define Motion_SetAngleToward_10(a0, a1, a2) Value3(Func_0200710a, a0, a1, a2)
#define Battle_WaitMode0_78(a0) Value1(Func_02007070, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020070f0, a0, a1)
#define Motion_CallWaitAnim_14(a0, a1) Call2(Func_02007100_b, a0, a1)
#define Battle_WaitMode0_79(a0) Value1(Func_02007086, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02007266, a0)
#define Motion_SetSpeed_8(a0, a1, a2) Call3(Func_020070d0_a, a0, a1, a2)
#define Motion_SetSpeed_9(a0, a1, a2) Call3(Func_020070da, a0, a1, a2)
#define Motion_SetSpeed_10(a0, a1, a2) Call3(Func_020070e4, a0, a1, a2)
#define Object_SetModeById_2(a0, a1) Value2(Func_0200713c, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_020070ea, a0)
#define Motion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_02007124, a0, a1, a2)
#define Motion_CommitPos_2(a0) Value1(Func_0200714a, a0)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200715c, a0, a1, a2)
#define Object_SetModeById_3(a0, a1) Value2(Func_0200716c, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_0200711a, a0)
#define Motion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02007154, a0, a1, a2)
#define Motion_CommitPos_3(a0) Value1(Func_0200717a, a0)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_0200718c, a0, a1, a2)
#define Object_SetModeById_4(a0, a1) Value2(Func_0200719c, a0, a1)
#define Scene_GetRecord_3(a0) Value1(Func_0200714a_a, a0)
#define Motion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_02007184, a0, a1, a2)
#define Motion_CommitPos_4(a0) Value1(Func_020071aa, a0)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_020071bc, a0, a1, a2)
#define Audio_PlayCueForPartyMember_1() Call0(Func_02007298)
#define Battle_SchedShoulder_1() Call0(Func_02007164)
#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18
#define Scene_SetActor13Value41 Func_020009dc
#define Scene_RunBranchingCutsceneSequence Func_02002c44
#define Scene_RunFlagGatedActorEightDialogue Func_02003518

void Func_02004794(s32, s32);
void Func_080770c8();
void Func_080f9010();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a460();
void Func_0808a4f0();
void Func_0808a570();
void Func_0808a580();
void Func_020068dc();
void Func_02006ac2();
void Func_02006906();
void Func_02006a42();
void Func_020069c0();
void Func_020069a8();
void Func_02006916();
void Func_020069e6();
void Func_02006924();
void Func_02006a26();
void Func_02006a26_a();
void Func_02006968();
void Func_020069a2();
void Func_02006af6();
void Func_02006b04();
void Func_02006b12();
void Func_02006b12_a();
void Func_020069f0();
void Func_0200698e();
void Func_02006a72();
void Func_020069a0();
void Func_02006a70();
void Func_02006a70_a();
void Func_02006a48();
void Func_020069b6();
void Func_02006a86();
void Func_020069c4();
void Func_02006aa8();
void Func_02006aa0();
void Func_020069de();
void Func_02006a90();
void Func_02006ac4();
void Func_02006ad0();
void Func_02006a06();
void Func_02006ad6();
void Func_02006a14();
void Func_02006afe();
void Func_02006abe();
void Func_02006abe_a();
void Func_02006a2c();
void Func_02006afc();
void Func_02006a3a();
void Func_02006aec();
void Func_02006aec_a();
void Func_02006b20();
void Func_02006b2c();
void Func_02006a62();
void Func_02006b32();
void Func_02006b54();
void Func_02006b4c();
void Func_02006a8a();
void Func_02006a98();
void Func_02006b68();
void Func_02006b68_a();
void Func_02006aa6();
void Func_02006b88();
void Func_02006b80();
void Func_02006b80_a();
void Func_02006baa();
void Func_02006baa_a();
void Func_02006ba2();
void Func_02006bb6();
void Func_02006bbc();
void Func_02006afa();
void Func_02006bde();
void Func_02006bd6();
void Func_02006bae();
void Func_02006b1c();
void Func_02006ba4();
void Func_02006b2a();
void Func_02006bfa();
void Func_02006b38();
void Func_02006c1c();
void Func_02006c14();
void Func_02006b52();
void Func_02006c34();
s32 Func_02006c3e();
s32 Func_02006c3e_a();
void Func_02006c48();
void Func_02006c52();
void Func_02006c20();
void Func_02006b8e();
void Func_02006c5e();
void Func_02006b9c();
void Func_02006c6c();
void Func_02006c8e();
void Func_02006c86();
void Func_02006bc4();
void Func_02006c64();
void Func_02006bd2();
void Func_02006ca2();
void Func_02006c7a();
void Func_02006be8();
void Func_02006cb8();
void Func_02006bf6();
void Func_02006cda();
s32 Func_02006cca();
void Func_02006cb4();
void Func_02006cbe();
void Func_02006ce8();
void Func_02006c36();
void Func_02006d1a();
void Func_02006c78();
void Func_02006dfc();
void Func_02006c62();
void Func_02006d14();
void Func_02006d3c();
void Func_02006d4c();
void Func_02006d4c_a();
void Func_02006c9a();
void Func_02006cd0();
void Func_02006e54();
void Func_02006cba();
void Func_02006d6c();
void Func_02006d6c_a();
void Func_02006cd2();
void Func_02006da2();
void Func_02006da2_a();
void Func_02006d9a();
void Func_02006dc4();
void Func_02006d22();
void Func_02006ea6();
void Func_02006d0c();
void Func_02006dbe();
void Func_02006de6();
void Func_02006da6();
void Func_02006d2c();
void Func_02006db4();
void Func_02006d3a();
void Func_02006de4();
void Func_02006dee();
void Func_02006df8();
void Func_02006d5e();
void Func_02006e2e();
void Func_02006f24();
void Func_02006d8a();
void Func_02006e12();
void Func_02006d98();
void Func_02006e42();
void Func_02006da8();
void Func_02006e78();
void Func_02006db6();
void Func_02006e68();
void Func_02006e68_a();
void Func_02006e48();
void Func_02006e50();
void Func_02006dd6();
void Func_02006e0c();
void Func_02006f8e();
void Func_02006df4();
void Func_02006ed0();
void Func_02006edc();
void Func_02006ee8();
void Func_02006ef6();
void Func_02006ee4();
void Func_02006e32();
void Func_02006f16();
void Func_02006f16_a();
void Func_02006f0e();
void Func_02006e4c();
void Func_02006ed4();
void Func_02006e5a();
void Func_02006f2a();
void Func_02006f4c();
void Func_02006f4c_a();
void Func_02006f44();
void Func_02006e82();
void Func_02006f0a();
void Func_02006e90();
void Func_02006f60();
void Func_02006e9e();
void Func_02006f26();
void Func_02006eac();
void Func_02006f7c();
void Func_02006eba();
void Func_02006f6c();
void Func_02006fa0();
void Func_02006fac();
void Func_02006fac_a();
void Func_02006ee2();
void Func_02006fb2();
void Func_02006ef0();
void Func_02006fd2();
void Func_02006fca();
void Func_02006f08();
void Func_02006fd8();
void Func_02006fc8();
void Func_02006fc8_a();
void Func_02006ffc();
void Func_02007008();
void Func_02006f3e();
void Func_0200700e();
void Func_02006ffe();
void Func_02007032();
void Func_0200703e();
void Func_0200703e_a();
void Func_02006f74();
void Func_02007044();
void Func_02006f82();
void Func_02007022();
void Func_02007022_a();
void Func_02006f90();
void Func_02007060();
void Func_02006f9e();
void Func_0200706e();
void Func_0200704c();
void Func_02006fba();
void Func_0200708a();
void Func_020070ac();
void Func_020070a4();
void Func_02006fe2();
void Func_020070c4();
void Func_020070bc();
void Func_02006ffa();
void Func_020070de();
void Func_020070d6();
void Func_020070ae();
void Func_0200701c();
void Func_020070aa();
void Func_02007030();
void Func_02007100();
void Func_020070e8();
void Func_020070d0();
void Func_020070d0_a();
void Func_02007056();
void Func_02007100_a();
void Func_0200710a();
void Func_02007070();
void Func_020070f0();
void Func_02007100_b();
void Func_02007086();
void Func_02007266();
void Func_020070da();
void Func_020070e4();
void Func_0200713c();
u8 *Func_020070ea();
void Func_02007124();
s32 Func_0200714a();
void Func_0200715c();
void Func_0200716c();
u8 *Func_0200711a();
void Func_02007154();
void Func_0200717a();
void Func_0200718c();
void Func_0200719c();
u8 *Func_0200714a_a();
void Func_02007184();
void Func_020071aa();
void Func_020071bc();
void Func_02007298();
void Func_02007164();
s32 Func_020071a8(s32 value);
s32 Func_020071b2(s32 value);
void Func_020071c4(s32 value);
void Func_020071e8(void);
void Func_0200729e(s32 value);
void Func_020072b6(s32 first, s32 second);
void Func_020071f4(s32 value);
void Func_02007294(s32 first, s32 second);
void Func_02007202(s32 value);
void Func_020072ac(s32 first, s32 second, s32 third);
void Func_02007212(s32 value);
void Func_020072e2(s32 first, s32 second);
void Func_020072a2(s32 first, s32 second);
void Func_02007228(s32 value);
void Func_0200723c(void);

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

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call0(void (*f)())
{
    f();
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Scene_SetActor13Value41(void)
{
    Func_02004794(13, 0x41);
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
    Value0(Func_02006a42); /* main:0808a460 */
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
    if (Func_020071a8(0x960) == 0)
        return;
    if (Func_020071b2(0x962) != 0)
        return;

    Func_020071c4(0x961);
    Func_020071e8();
    Func_0200729e(0x217d);
    Func_020072b6(8, 0);
    Func_020071f4(10);
    Func_02007294(0, 2);
    Func_02007202(30);
    Func_020072ac(0, 8, 0);
    Func_02007212(30);
    Func_020072e2(8, 0);
    Func_020072a2(0, 3);
    Func_02007228(20);
    Func_0200723c();
}
#undef GameFlag_Set_1
#undef Audio_PlayCue_1
#undef Battle_Reset_1
#undef SceneWork_SetStepValue_1
#undef Motion_SetVarCbObj_1
#undef Battle_WaitMode0_1
#undef BattleEv_RunWait_1
#undef Battle_WaitMode0_2
#undef Motion_CamBounds_1
#undef Motion_SetSpeed_1
#undef Motion_SetPosReset_1
#undef Motion_ArmCb_1
#undef Motion_RealignToTrackedObjectAndArmCallback_1
#undef Motion_RealignToTrackedObjectAndArmCallback_2
#undef Motion_RealignToTrackedObjectAndArmCallback_3
#undef Motion_CommitPos_1
#undef Battle_WaitMode0_3
#undef BattleFx_SpawnLinked_1
#undef Battle_WaitMode0_4
#undef BattleEv_RunWait_2
#undef Motion_SetVarCbObj_2
#undef Battle_WaitMode0_5
#undef BattleEv_RunWait_3
#undef Battle_WaitMode0_6
#undef BattleFx_SpawnLinked_2
#undef BattleEv_RunWait_4
#undef Battle_WaitMode0_7
#undef Object_LinkPair_1
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Battle_WaitMode0_8
#undef BattleEv_RunWait_5
#undef Battle_WaitMode0_9
#undef Battle_WaitMode0_10
#undef Motion_SetVarCbObj_3
#undef Battle_WaitMode0_11
#undef BattleEv_RunWait_6
#undef Battle_WaitMode0_12
#undef Object_LinkPair_2
#undef Motion_ArmCb_4
#undef Motion_ArmCb_5
#undef Battle_WaitMode0_13
#undef BattleEv_RunWait_7
#undef Battle_WaitMode0_14
#undef BattleFx_SpawnLinked_3
#undef BattleEv_RunWait_8
#undef Battle_WaitMode0_15
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_16
#undef BattleEv_RunWait_9
#undef Battle_WaitMode0_17
#undef BattleFx_SpawnLinked_4
#undef BattleEv_RunWait_10
#undef Battle_WaitMode0_18
#undef Motion_SetAngleToward_1
#undef BattleFx_SpawnLinked_5
#undef BattleEv_RunWait_11
#undef Motion_ArmCb_6
#undef Battle_WaitMode0_19
#undef BattleEv_RunWait_12
#undef Battle_WaitMode0_20
#undef BattleFx_SpawnLinked_6
#undef BattleEv_RunWait_13
#undef Motion_SetVarCbObj_4
#undef Battle_WaitMode0_21
#undef Motion_CallWaitAnim_2
#undef Battle_WaitMode0_22
#undef BattleEv_RunWait_14
#undef Battle_WaitMode0_23
#undef BattleFx_SpawnLinked_7
#undef BattleEv_RunWait_15
#undef Battle_WaitMode0_24
#undef BattleFx_SpawnLinked_8
#undef BattleFx_SpawnLinked_9
#undef BattleFx_SpawnLinked_10
#undef BattleFx_SpawnLinked_11
#undef Battle_WaitMode0_25
#undef Motion_SetVarCbObj_5
#undef Battle_WaitMode0_26
#undef BattleEv_RunWait_16
#undef Battle_WaitMode0_27
#undef BattleEv_RunWait_17
#undef Battle_WaitMode0_28
#undef BattleFx_SpawnLinked_12
#undef BattleEv_RunWait_18
#undef Battle_WaitMode0_29
#undef Motion_SetVarCbObj_6
#undef Battle_WaitMode0_30
#undef BattleEv_RunWait_19
#undef Motion_SetVarCbObj_7
#undef Battle_WaitMode0_31
#undef BattleEv_RunWait_20
#undef Battle_WaitMode0_32
#undef BattleFx_SpawnLinked_13
#undef Motion_SetSpeed_2
#undef Motion_SetAngleToward_2
#undef Motion_SetAngleToward_3
#undef UiWork_WaitThenFinalizeCapacity_1
#undef SceneWork_SetStepValue_2
#undef Battle_WaitMode0_33
#undef BattleFx_SpawnLinked_14
#undef Motion_SetSpeed_3
#undef Motion_CommitPositionAndActivate_1
#undef Battle_WaitMode0_34
#undef Object_LinkPair_3
#undef BattleEv_RunWait_21
#undef SceneWork_SetStepValue_3
#undef Battle_WaitMode0_35
#undef Motion_SetSpeed_4
#undef Motion_CommitPositionAndActivate_2
#undef Battle_WaitMode0_36
#undef Object_LinkPair_4
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_37
#undef BattleEv_RunWait_22
#undef SceneWork_SetStepValue_4
#undef BattleFx_SpawnLinked_15
#undef Motion_SetSpeed_5
#undef Motion_CommitPositionAndActivate_3
#undef Battle_WaitMode0_38
#undef Object_LinkPair_5
#undef BattleEv_RunWait_23
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_39
#undef Motion_CallWaitAnim_5
#undef Battle_WaitMode0_40
#undef Motion_SetAngleToward_4
#undef Motion_SetAngleToward_5
#undef Motion_SetAngleToward_6
#undef Battle_WaitMode0_41
#undef BattleEv_RunWait_24
#undef Battle_WaitMode0_42
#undef Motion_SetSpeed_6
#undef Motion_CommitPositionAndActivate_4
#undef Battle_WaitMode0_43
#undef Motion_CallWaitAnim_6
#undef Battle_WaitMode0_44
#undef Motion_SetAngleToward_7
#undef Battle_WaitMode0_45
#undef BattleEv_RunWait_25
#undef Battle_WaitMode0_46
#undef Object_LinkPair_6
#undef Motion_CallWaitAnim_7
#undef Motion_CallWaitAnim_8
#undef Battle_WaitMode0_47
#undef Motion_SetSpeed_7
#undef Motion_CommitPositionAndActivate_5
#undef Battle_WaitMode0_48
#undef Motion_ArmCb_7
#undef Motion_ArmCb_8
#undef Motion_ArmCb_9
#undef Motion_ArmCb_10
#undef SceneWork_SetStepValue_5
#undef Battle_WaitMode0_49
#undef BattleFx_SpawnLinked_16
#undef BattleEv_RunWait_26
#undef Battle_WaitMode0_50
#undef Motion_CallWaitAnim_9
#undef Battle_WaitMode0_51
#undef BattleEv_RunWait_27
#undef Battle_WaitMode0_52
#undef BattleFx_SpawnLinked_17
#undef BattleEv_RunWait_28
#undef Battle_WaitMode0_53
#undef Motion_CallWaitAnim_10
#undef Battle_WaitMode0_54
#undef BattleEv_RunWait_29
#undef Battle_WaitMode0_55
#undef Motion_CallWaitAnim_11
#undef Battle_WaitMode0_56
#undef BattleEv_RunWait_30
#undef Battle_WaitMode0_57
#undef Object_LinkPair_7
#undef Motion_ArmCb_11
#undef Motion_ArmCb_12
#undef Battle_WaitMode0_58
#undef BattleEv_RunWait_31
#undef Battle_WaitMode0_59
#undef BattleFx_SpawnLinked_18
#undef BattleEv_RunWait_32
#undef Battle_WaitMode0_60
#undef BattleEv_RunWait_33
#undef Battle_WaitMode0_61
#undef Object_LinkPair_8
#undef Motion_ArmCb_13
#undef Motion_ArmCb_14
#undef Battle_WaitMode0_62
#undef BattleEv_RunWait_34
#undef Battle_WaitMode0_63
#undef Object_LinkPair_9
#undef Motion_ArmCb_15
#undef Motion_ArmCb_16
#undef Battle_WaitMode0_64
#undef BattleEv_RunWait_35
#undef Battle_WaitMode0_65
#undef Motion_SetVarCbObj_8
#undef Battle_WaitMode0_66
#undef BattleEv_RunWait_36
#undef Battle_WaitMode0_67
#undef BattleEv_RunWait_37
#undef Battle_WaitMode0_68
#undef Motion_SetVarCbObj_9
#undef Battle_WaitMode0_69
#undef BattleEv_RunWait_38
#undef Battle_WaitMode0_70
#undef BattleFx_SpawnLinked_19
#undef BattleEv_RunWait_39
#undef Battle_WaitMode0_71
#undef BattleFx_SpawnLinked_20
#undef BattleEv_RunWait_40
#undef Battle_WaitMode0_72
#undef BattleFx_SpawnLinked_21
#undef BattleEv_RunWait_41
#undef Motion_SetVarCbObj_10
#undef Battle_WaitMode0_73
#undef Battle_WaitMode0_74
#undef Motion_CallWaitAnim_12
#undef Battle_WaitMode0_75
#undef BattleEv_RunWait_42
#undef Battle_WaitMode0_76
#undef Motion_SetAngleToward_8
#undef Motion_CallWaitAnim_13
#undef Battle_WaitMode0_77
#undef Motion_SetAngleToward_9
#undef Motion_SetAngleToward_10
#undef Battle_WaitMode0_78
#undef Object_SetModeById_1
#undef Motion_CallWaitAnim_14
#undef Battle_WaitMode0_79
#undef Audio_PlayCue_2
#undef Motion_SetSpeed_8
#undef Motion_SetSpeed_9
#undef Motion_SetSpeed_10
#undef Object_SetModeById_2
#undef Scene_GetRecord_1
#undef Motion_ResetAndSetPosition_1
#undef Motion_CommitPos_2
#undef Motion_SetHPosTerrain_1
#undef Object_SetModeById_3
#undef Scene_GetRecord_2
#undef Motion_ResetAndSetPosition_2
#undef Motion_CommitPos_3
#undef Motion_SetHPosTerrain_2
#undef Object_SetModeById_4
#undef Scene_GetRecord_3
#undef Motion_ResetAndSetPosition_3
#undef Motion_CommitPos_4
#undef Motion_SetHPosTerrain_3
#undef Audio_PlayCueForPartyMember_1
#undef Battle_SchedShoulder_1
#undef RECORD_COORD_X_OFFSET
#undef RECORD_COORD_Y_OFFSET
#undef Scene_SetActor13Value41
#undef Scene_RunBranchingCutsceneSequence
#undef Scene_RunFlagGatedActorEightDialogue

/* overlays/scene/field/extended_presentation/scene_setup.c */
#define Scene_RunIndexedStep0 Func_02000ae8
#define Scene_RunIndexedStep1 Func_02000af4
#define Scene_RunIndexedStep2 Func_02000b00
#define Scene_RunIndexedStep3 Func_02000b0c
#define Scene_RunIndexedStep4 Func_02000b18
#define Scene_RunIndexedStep5 Func_02000b24
#define Scene_RunTwoCallSequence Func_02000d48
#define Scene_RunGuardedSixWordStep Func_02000eac
#define Scene_RunTwoArmSequenceWithValue217f Func_020034bc
typedef struct {
    s32 a, b, c, d, e, f;
} S6;

extern u8 Value_0000217f;

void Func_020015aa(s32);
void Func_020015b6(s32);
void Func_020015c2(s32);
void Func_020015ce_a(s32);
void Func_020015da(s32);
void Func_020015e6(s32);
void Func_02000e10(void);
void Func_02001a48(void);
void Func_02004b62(void);
s32 Func_0200132e(S6 *);
void Func_020014da(S6);
void Func_02004b8e(void);
void Func_02007170(void);
void Func_02007228_a(s32 arg0);
s32 Func_02007238(s32 arg0, s32 arg1);
s32 Func_02007198(s32 arg0, s32 arg1);
void Func_0200718a(s32 arg0);
void Func_02007248(s32 arg0);
void Func_02007260(s32 arg0, s32 arg1);
void Func_020071a0(s32 arg0);
void Func_0200725e(s32 arg0);
void Func_02007276(s32 arg0, s32 arg1);
void Func_020071c2(void);

/* Contiguous unnamed leaf-owner run for resource_3bd. */

void Scene_RunIndexedStep0(void)
{
    Func_020015aa(0);
}

void Scene_RunIndexedStep1(void)
{
    Func_020015b6(1);
}

void Scene_RunIndexedStep2(void)
{
    Func_020015c2(2);
}

void Scene_RunIndexedStep3(void)
{
    Func_020015ce_a(3);
}

void Scene_RunIndexedStep4(void)
{
    Func_020015da(4);
}

void Scene_RunIndexedStep5(void)
{
    Func_020015e6(5);
}

void Scene_RunTwoCallSequence(void)
{
    Func_02000e10();
    Func_02001a48();
}

void Scene_RunGuardedSixWordStep(void)
{
    S6 s;

    Func_02004b62();
    if (Func_0200132e(&s) != 0) {
        Func_020014da(s);
    }
    Func_02004b8e();
}

void Scene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Func_02007170();
    val = (s32)&Value_0000217f;
    Func_02007228_a(val);
    Func_02007238(8, 0);
    if (Func_02007198(0, 0) == 0) {
        Func_0200718a(20);
        Func_02007248(val + 1);
        Func_02007260(8, 0);
    } else {
        Func_020071a0(20);
        Func_0200725e(val + 2);
        Func_02007276(8, 0);
    }
    Func_020071c2();
}
#undef Scene_RunIndexedStep0
#undef Scene_RunIndexedStep1
#undef Scene_RunIndexedStep2
#undef Scene_RunIndexedStep3
#undef Scene_RunIndexedStep4
#undef Scene_RunIndexedStep5
#undef Scene_RunTwoCallSequence
#undef Scene_RunGuardedSixWordStep
#undef Scene_RunTwoArmSequenceWithValue217f

/* overlays/scene/field/extended_presentation/scene_state_interaction.c */
#define State_RunFlag200SetupAndPlaceActors16To20 Func_02000c2c
extern s16 Data_02000240;
extern u8 Value_00000097;

s32 Func_020048c0(s32);
void Func_02001770(void);
void Func_020049ba(s32, s32);
void Func_020049c2(s32, s32);
void Func_020049ca(s32, s32);
void Func_020049d2(s32, s32);
void Func_020049da(s32, s32);

void State_RunFlag200SetupAndPlaceActors16To20(void)
{
    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (Func_020048c0(0x200) != 0) {
        Func_02001770();
        work[0x34] = 1;
    }
    tbl = &Data_02000240;
    if (tbl[0xe0] == (s32)&Value_00000097) {
        Func_020049ba(16, 6);
        Func_020049c2(17, 6);
        Func_020049ca(18, 6);
        Func_020049d2(19, 6);
        Func_020049da(20, 6);
    }
}
#undef State_RunFlag200SetupAndPlaceActors16To20

/* overlays/scene/field/extended_presentation/staged_actor.c */
#define State_MarkObjectWhenActorElevenAhead Func_02000d58
#define Scene_RunActorElevenCellSetup Func_02000d90
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

Ent *Func_02004a2c(s32);
s32 Func_020049ce(Ent *, Vec *);
u8 *Func_02004a64(s32);
void Func_02004a06(s32, s32, s32, s32, s32, s32);
void Func_02004a66(s32);

/* Contiguous unnamed state-owner run for resource_3bd. */

void State_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    Ent *p = Func_02004a2c(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Func_020049ce(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void Scene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Func_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Func_02004a06(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        Func_02004a66(0x211);
    }
}
#undef State_MarkObjectWhenActorElevenAhead
#undef Scene_RunActorElevenCellSetup

/* overlays/scene/field/extended_presentation/staged_actor_pair.c */
#define StagedActorStepTable Data_0200beb4
#define GetStagedActor Func_02003d9c
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02003df8
#define SetStagedActorMode Func_02003de0
#define SelectStagedActorSlot Func_02003dc0
#define StartStagedActorEffect Func_02004036
#define StartNextStagedActorMove Func_02003e0e
#define StartLeadStagedActorMove Func_02003e1e
#define FinishStagedActorMove Func_02003e2c
#define FinishStagedActorEffect Func_02004048
#define SetStagedActorTransition Func_02003e4c
#define StagedActor_AdvancePair Func_020000c4


extern u32 Data_0200beb4[];

struct StagedActor *Func_02003d9c(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02003df8(struct StagedActor *arg0, s32 *arg1);
void Func_02003de0(struct StagedActor *arg0, s32 arg1);
void Func_02003dc0(s32 arg0);
void Func_02004036(s32 arg0);
void Func_02003e0e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003e1e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003e2c(struct StagedActor *arg0);
void Func_02004048(void);
void Func_02003e4c(struct StagedActor *arg0, s32 arg1);

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
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef StagedActor_AdvancePair

/* overlays/scene/field/extended_presentation/state_update.c */
#define State_SetByte1004AndRunWhenIdle Func_02000abc
extern u8 Data_02001004;

void Func_02001528(void);

void State_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Func_02001528();
    }
}
#undef State_SetByte1004AndRunWhenIdle
