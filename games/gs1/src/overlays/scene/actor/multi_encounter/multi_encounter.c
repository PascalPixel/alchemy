#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/multi_encounter/multi_encounter.h"
#include "resource_3af_motion.h"

/* overlays/scene/actor/multi_encounter/actor_facing.c */

s32 Actor_SetFacingFromSample(u8 *a)
{
    u32 v = ((u32)(Actor_Check23() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        *(u16 *)(a + 6) = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        *(u16 *)(a + 6) = t << 8;
    }
    return 1;
}

/* overlays/scene/actor/multi_encounter/actor_motion.c */

s32 Actor_OscillateHeightBetweenLimits(u8 *obj)
{
    s16 *flag = (s16 *)(obj + 0x66);
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = *(s32 *)(obj + 0xc) - (((u32)(Actor_Check24() << 15)) >> 16) - 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = *(s32 *)(obj + 0xc) + (((u32)(Actor_Check25() << 15)) >> 16) + 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 Actor_SetWord28RandomlyOneIn40(u8 *obj)
{
    if ((((u32)(Actor_Check26() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}

/* overlays/scene/actor/multi_encounter/actor_position.c */

u8 *Actor_Run11(s32);

u8 *Actor_Run12(s32);

u8 *Actor_Run13(s32);

u8 *Actor_Run14(s32);

u8 *Actor_Run15(s32);

void Actor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_Place34(21, 0x1060000, 0x2C20000);
    *(u16 *)(Actor_Run11(21) + 6) = m;
    Actor_Place35(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Actor_Run12(24) + 6) = z;
    }
    Actor_Apply(24, 1);
    Actor_Place36(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Actor_Run13(25) + 6) = x << 8;
    }
    Actor_Apply2(25, 1);
    Actor_Place37(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Actor_Run14(26) + 6) = x << 8;
    }
    Actor_Place38(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Actor_Run15(27) + 6) = m;
    Actor_Place39(22, 0, 0);
    Actor_Place40(23, 0, 0);
    Actor_Place41(20, 0, 0);
}

/* overlays/scene/actor/multi_encounter/dialogue_layout.c */

void DialogueLayout_ConfigureTwoRegions(void)
{
    Actor_SetRect4(66, 61, 64, 40, 5, 4);
    Actor_SetRect5(0, 0, 5, 4, 5, 39);
}

/* overlays/scene/actor/multi_encounter/effect.c */
union Slot {
    s32 w;
    s16 h[2];
};

extern s32 Actor_Far[];
extern u16 gOv[];
extern u16 gOv2[];
extern u32 gOv3[];

u8 *Actor_Run16(s32);
u8 *Actor_Run17(s32);
u8 *Actor_Run18(s32);
u8 *Actor_Run19(s32);

s32 *Actor_Run20(s32);
s32 *Actor_Run21(s32);
s32 *Actor_Run22(s32);
s32 *Actor_Run23(s32);

s32 *Actor_Run24(s32);
s32 *Actor_Run25(s32);
s32 *Actor_Run26(s32);
s32 *Actor_Run27(s32);
s32 *Actor_Run28(s32);
s32 *Actor_Run29(s32);
s32 *Actor_Run30(s32);
s32 *Actor_Run31(s32);

void Effect_InitSlotsEightToNineteen(void)
{
    {
        s32 z = 0;
        *(Actor_Run16(8) + 89) = z;
        *(Actor_Run17(9) + 89) = z;
        *(Actor_Run18(10) + 89) = z;
        *(Actor_Run19(11) + 89) = z;
    }
    Actor_Do20(8);
    Actor_Do21(9);
    Actor_Do22(10);
    Actor_Do23(11);
    Actor_Do24(12);
    Actor_Do25(13);
    Actor_Do26(14);
    Actor_Do27(15);
    {
        Actor_Far[0] = Actor_Run20(12)[4];
        Actor_Far[1] = Actor_Run21(13)[4];
        Actor_Far[2] = Actor_Run22(14)[4];
        Actor_Far[3] = Actor_Run23(15)[4];
        Actor_Do28(16);
        Actor_Do29(17);
        Actor_Do30(18);
        Actor_Do31(19);
        {
            s32 v = 0xFFFF0000;

            Actor_Run24(16)[6] = v;
            Actor_Run25(17)[6] = v;
            Actor_Run26(18)[6] = v;
            Actor_Run27(19)[6] = v;
        }
        Actor_Far[4] = Actor_Run28(16)[4];
        Actor_Far[5] = Actor_Run29(17)[4];
        Actor_Far[6] = Actor_Run30(18)[4];
        Actor_Far[7] = Actor_Run31(19)[4];
    }
    Actor_Run32();
}

void Effect_AdvanceSlotByValueBand(s32 a, s32 i)
{
    u16 v = gOv[i];

    if (v >= 0x6801 && v <= 0x6FFF) {
        gOv2[i] += 0x70;
        Actor_Apply3(a, 3);
    } else if (v >= 0xE801 && v <= 0xEFFF) {
        gOv2[i] += 0xE0;
        Actor_Apply3(a, 3);
    } else if (v >= 0x7001 && v <= 0xEFFF) {
        gOv2[i] += 0x1C0;
        Actor_Apply4(a, 2);
    } else {
        gOv2[i] += 0x300;
        Actor_Apply5(a, 1);
    }
}

void Effect_SelectSlotValueAndPosition(s32 a, s32 b, s32 c)
{
    s32 *p = (s32 *)Actor_Check27(a);
    union Slot *q = (union Slot *)p[20];
    s32 t;

    if ((c & 2) == 0) {
        switch (gOv3[b]) {
        case 1:
            gOv[b] = gOv2[0];
            Actor_Apply6(a, 8);
            break;
        case 2:
            gOv[b] = gOv2[1];
            Actor_Apply7(a, 9);
            break;
        case 3:
            gOv[b] = gOv2[2];
            Actor_Apply8(a, 10);
            break;
        case 4:
            gOv[b] = gOv2[3];
            Actor_Apply9(a, 11);
            break;
        }
    }
    if ((c & 1) != 0) {
        t = Actor_Check28(gOv[b]);
        q[7].h[1] = Actor_Check29(gOv[b] + 0x8000) >> 5;
        p[4] = Actor_Far[b] - (t << 2) - (t << 1);
    } else {
        t = Actor_Check30(gOv[b] + 0x8000);
        q[7].h[1] = Actor_Check31(gOv[b]) >> 5;
        p[4] = Actor_Far[b] + (t << 2) + (t << 1);
    }
}

/* overlays/scene/actor/multi_encounter/field_scene_head.c */

extern u8 gWork[];
extern u8 gVal[];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Actor_Run33();

u8 *Actor_Run34();

u8 *Actor_Run35();

u8 *Actor_Run36();

u8 *Actor_Run37();
u8 *Actor_Run38();

void Scene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Actor_Check32)();
    if (Actor_Check(0x925) != 0) {
        Actor_Do(0x1e08);
        Actor_Run39(21, 0);
    } else {
        if (Actor_Check2(0x922) != 0) {
            Actor_Run40(21, 2);
            Actor_Do2(0x1d6f);
            Actor_Run41(21, 0);
            rec7 = Actor_Check3(21);
            record = Actor_Check33();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Actor_Run42(21, 0x200c4d8);
        } else {
            Actor_Place(21, 0x103, 0);
            Actor_Run43(21, 3);
            Actor_Do3(0x1d36);
            Actor_Run44(21, 0);
        }
    }
    Actor_Run45();
}

void Actor_Run46(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Actor_Check34)();
    if (Actor_Check4(0x925) != 0) {
        Actor_Do4(0x1e09);
        Actor_Run47(24, 0);
    } else {
        if (Actor_Check5(0x922) != 0) {
            Actor_Run48(24, 2);
            Actor_Do5(0x1d70);
            Actor_Run49(24, 0);
            rec7 = Actor_Check6(24);
            record = Actor_Check35();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Actor_Run50(24, 0x200c4d8);
        } else {
            Actor_Place2(24, 0x103, 0);
            Actor_Run51(24, 3);
            Actor_Do6(0x1d37);
            Actor_Run52(24, 0);
        }
    }
    Actor_Run53();
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    if (Actor_Check7(0x271) == 0) {
        Actor_Run54(158);
        Actor_SetRect(30, 94, 13, 94, 1, 3);
        Actor_Do7(0x271);
    }
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    if (Actor_Check8(0x272) == 0) {
        Actor_Run55(158);
        Actor_SetRect2(30, 108, 13, 108, 1, 2);
        Actor_Do8(0x272);
    }
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void Scene_RunActorAndEffectPresentationSetup(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x911) == 0) {
    } else {
        Battle_Reset_1();
        Actor_Run56();
        Motion_SetAngleToward_1(0, 20, 10);
        Motion_SetSpeedLim_1(0x19999, 0x3333);
        Motion_CamBounds_1(0xbe0000, -1, 0x2c40000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        Battle_WaitMode0_1(40);
        Motion_SetVarCbObj_1(22, 1);
        SceneWork_SetStepValue_1(0x1d26);
        Actor_Run57(0x4016);
        BattleFx_SpawnLinked_1(20, 0x102, 60);
        Motion_SetVarCb_1(20, 2);
        Actor_Run58(20);
        Motion_SetVarCbObj_2(22, 1);
        Motion_ArmCb_1(22, 0x5000, 0);
        Actor_Run59(0x4016);
        Motion_SetVarCbObj_3(20, 1);
        Actor_Run60(20, 0xb000);
        Actor_Run61(20);
        Actor_Run62(23, 0x3000);
        Object_SetModeById_1(23, 3);
        Actor_Run63(0x4017);
        BattleFx_SpawnLinked_2(22, 0x101, 40);
        Motion_ArmCb_2(22, 0x8000, 20);
        Actor_Run64(0x4016);
        Actor_Run65(23, 0);
        Motion_CallWaitAnim_1(23, 4);
        Actor_Run66(0x4017);
        BattleFx_SpawnLinked_3(20, 0x100, 40);
        Motion_SetVarCb_2(20, 2);
        Actor_Run67(20);
        Motion_CallWaitAnim_2(22, 3);
        Actor_Run68(0x4016);
        Actor_Run69(20, 0xd000);
        Object_SetModeById_2(23, 3);
        Motion_CallWaitAnim_3(20, 3);
        Battle_WaitMode0_2(60);
        BattleFx_SpawnLinked_4(22, 0x106, 40);
        Actor_Run70(22, 0x5000);
        SceneWork_SetStepValue_2((s32)gVal);
        Motion_SetVarCb_3(22, 1);
        Actor_Run71(0x4016);
        BattleFx_SpawnLinked_5(20, 0x101, 40);
        Motion_SetVarCb_4(20, 2);
        Actor_Run72(20);
        BattleFx_SpawnLinked_6(22, 0x108, 20);
        Battle_RunThenWaitIfModeZero_1(0x4016, 0, 20);
        BattleFx_SpawnLinked_7(23, 0x102, 60);
        Actor_Run73(0x4017);
        Actor_Check9(22, 0x8000);
        Motion_CallWaitAnim_4(22, 3);
        Battle_RunThenWaitIfModeZero_2(0x4016, 0, 20);
        BattleFx_SpawnLinked_8(20, 0x102, 40);
        Motion_SetVarCb_5(20, 2);
        Actor_Run74(20);
        Actor_Run75(22, 0x5000);
        Object_SetModeById_3(22, 4);
        Actor_Run76(22);
        Motion_ArmCb_3(20, 0xb000, 0);
        Motion_ArmCb_4(23, 0x3000, 40);
        Motion_ArmCb_5(23, 0, 0);
        Motion_ArmCb_6(20, 0xd000, 20);
        Motion_SetVarCbObj_4(22, 2);
        Battle_WaitMode0_3(20);
        Actor_Run77(0x4016);
        Battle_WaitMode0_4(23, 0x102);
        Battle_WaitMode0_5(20, 0x102);
        Battle_WaitMode0_6(40);
        Motion_CallWaitAnim_5(22, 3);
        Actor_Run78(0x4016);
        Motion_SetSpeedLim_2(0xcccc, 0x1999);
        Motion_CamBounds_2(0xb60000, -1, 0x2f80000, 1);
        Motion_SetSpeed_1(23, 0xcccc, 0x6666);
        Motion_EnableActCb_1(23, 0x200c464);
        Motion_SetSpeed_2(22, 0xcccc, 0x6666);
        Motion_EnableActCb_2(22, 0x200c49c);
        Motion_SetSpeed_3(20, 0xcccc, 0x6666);
        Motion_SetPosReset_1(20, 182, 0x2f8);
        Motion_SetVarCb_6(20, 2);
        BattleFx_SpawnLinked_9(20, 0x100, 60);
        Actor_Run79(20, 0xd000);
        Battle_RunThenWaitIfModeZero_3(20, 0, 20);
        Motion_CallWaitAnim_6(20, 3);
        Motion_Launch_1(20, 4, 0);
        Motion_ArmCb_7(20, 0x3000, 40);
        Motion_SetSpeedLim_3(0x10000, 0x2000);
        Motion_CamBounds_3(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = Scene_GetRecord_1(20);
            u8 flags = *(volatile u8 *)&record[35];

            record[35] = (u8)(flags | 1);
        }
        Motion_SetSpeed_4(20, 0x13333, 0x9999);
        Motion_SetPosReset_2(20, 182, 0x30e);
        Motion_SetPosReset_3(20, 192, 0x328);
        Motion_SetPosReset_4(20, 216, 0x328);
        Actor_Run80(20, 0xd000);
        Motion_SetVarCbObj_5(20, 2);
        Actor_Run81();
        Motion_SetPosReset_5(20, 216, 0x31e);
        Motion_SetHPosTerrain_1(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = Scene_GetRecord_2(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = Scene_GetRecord_3(20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set_1(0x920);
        Battle_SchedShoulder_1();
    }
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    if (Actor_Check10(0x911) != 0) {
        if (Actor_Check11(0x922) == 0) {
            Actor_Run82();
            Actor_Run83();
            Actor_Run84();
            Actor_Place3(20, 0x6666, 0x3333);
            *(u8 *)(Actor_Check36(20) + 90) &= 254;
            Actor_Place4(20, 232, 0x330);
            Actor_Run85(1);
            *(u8 *)(Actor_Check37(20) + 90) |= 1;
            Actor_Run86(20);
            Actor_Run87(20, 2);
            Actor_Run88(20);
            Actor_Place5(20, 0x13333, 0x9999);
            *(u8 *)(Actor_Check38(20) + 90) &= 254;
            Actor_Place6(20, 244, 0x324);
            Actor_Run89(1);
            {
                u8 *record = Actor_Run33(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_Run90(20);
            Actor_Place7(20, 0x33333, 0x19999);
            Actor_Place8(20, 248, 0x30a);
            Actor_Place9(20, 248, 0x2bc);
            Actor_Place10(20, 0xf60000, 0x2000000);
            Actor_Run91(20, 0, 0);
            Actor_Place11(0, 0x101, 60);
            Actor_Run92();
        }
    }
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    if (Actor_Check12(0x911) != 0) {
        if (Actor_Check13(0x922) == 0) {
            Actor_Run93();
            Actor_Run94();
            Actor_Run95();
            Actor_Place12(20, 0x6666, 0x3333);
            *(u8 *)(Actor_Check39(20) + 90) &= 254;
            Actor_Place13(20, 202, 0x330);
            Actor_Run96(1);
            *(u8 *)(Actor_Check40(20) + 90) |= 1;
            Actor_Run97(20);
            Actor_Run98(20, 2);
            Actor_Run99(20);
            Actor_Place14(20, 0x13333, 0x9999);
            *(u8 *)(Actor_Check41(20) + 90) &= 254;
            Actor_Place15(20, 192, 0x324);
            Actor_Run100(1);
            {
                u8 *record = Actor_Run34(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_Run101(20);
            Actor_Place16(20, 0x33333, 0x19999);
            Actor_Place17(20, 180, 0x30a);
            Actor_Place18(20, 180, 0x2bc);
            Actor_Place19(20, 0xf60000, 0x2000000);
            Actor_Run102(20, 0, 0);
            Actor_Place20(0, 0x101, 60);
            Actor_Run103();
        }
    }
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Actor_Run(0x19999, 0x3333);
    Actor_Run2(0xd80000, -1, 0x3380000, 1);
    Actor_Run104();
    Actor_Run105(20);
    Actor_Run106();
    Actor_SetRect3(30, 108, 13, 108, 1, 2);
    Actor_Run107(10);
    Actor_Place21(20, 0xd80000, 0x3200000);
    Actor_Place22(20, 0x13333, 0x9999);
    Actor_Place23(20, 216, 0x32e);
    Actor_Run108(0, 20, 10);
    Actor_Run109(20, 4);
    Actor_Run110(20, 2);
    Actor_Place24(20, 0x100, 20);
    Actor_Run111(20, 0, 20);
    Actor_Run112(20, 2);
    Actor_Do9(0x1d8d);
    Actor_Run113(20, 0, 20);
    Actor_Place25(20, 0x102, 0);
    Actor_Do10(0x923);
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    Actor_Run114();
    Actor_Do11(0x200d160);
    Actor_Run115(1);
    Actor_Run116(20, 0, 0);
    Actor_Place26(23, 0xee0000, 0x2720000);
    Actor_Place27(22, 0xcc0000, 0x2090000);
    record = Actor_Run35(22);
    *(volatile s32 *)((s32)record + 12) = 0x100000;
    *(u8 *)(Actor_Check42(22) + 89) |= 128;
    Actor_Place28(22, 0x9999, 0x4ccc);
    Actor_Run3(22, 0x200c58c);
    {
        u8 *record = Actor_Run36(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Actor_Place29(21, 0xcccc, 0x6666);
    Actor_Run4(21, 0x200c628);
    if (Actor_Check14(0x109) != 0) {
        Actor_Run117();
    }
    Actor_Run118();
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    Actor_Run119();
    Actor_Do12(0x200d160);
    Actor_Run120(1);
    Actor_Run121(20, 0, 0);
    Actor_Place30(23, 0xee0000, 0x2720000);
    Actor_Place31(22, 0x10c0000, 0x2a60000);
    record = Actor_Run37(22);
    {
        s32 shown = 0;

        *(volatile u16 *)((s32)record + 6) = shown;
    }
    Actor_Run5(22, 0x200c980);
    {
        u8 *record = Actor_Run38(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Actor_Place32(21, 0xcccc, 0x6666);
    Actor_Run6(21, 0x200c628);
    if (Actor_Check15(0x109) != 0) {
        Actor_Run122();
    }
    Actor_Run123();
}

/* overlays/scene/actor/multi_encounter/overlay_object.c */
u8 *Actor_Run124(s32);

void OvObj_DecayFields24And28(u8 *o)
{
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}

void OvObj_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Actor_Run124(a);

    if (p != 0) {
        Actor_Apply10(a, 3);
        Actor_Apply11(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}

/* overlays/scene/actor/multi_encounter/run_scene_configure_four_actor_presentation.c */
/* The step value differs in the localized scene data. */
#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE 0x20ac
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE 0x1f05
#else
#define SCENE_STEP_VALUE 0x1f23
#endif

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#endif
#if defined(GS1_EDITION_DE)
#endif

extern u8 gVal2[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gWork[];

void ObjectGroup_ConfigureChildValue_1(); void Battle_WaitMode0_4();

void Motion_SetVarCbObj_1(); void Motion_SetSpeed_3();

void Scene_ConfigureFourActorPresentation(void)
{
    u32 i;
    s32 record;
    s32 base6_6014;

    Battle_Reset_1();
    *(volatile s32 *)gOv4 = 0x40000;
    *(volatile s32 *)gOv5 = -0x8000;
    Object_NotifyLastActiveOfEvent_1((s32)gOv6);
    Actor_Run125(1);
    Motion_SetHPosTerrain_1(21, 0xb60000, 0x26a0000);
    record = Scene_GetRecord_1(21);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetHPosTerrain_2(20, 0xda0000, 0x2040000);
    record = Scene_GetRecord_2(20);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetHPosTerrain_3(22, 0xcc0000, 0x20e0000);
    record = Scene_GetRecord_3(22);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetHPosTerrain_4(23, 0, 0);
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_4(0);
    Actor_Run126(record, 0);
    Actor_Run127(1);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Motion_SetSpeed_2(21, 0xcccc, 0x6666);
    Motion_SetPosReset_1(21, 182, 0x214);
    Motion_ArmCb_1(21, 0xb000, 40);
    SceneWork_SetStepValue_1(SCENE_STEP_VALUE);
    Actor_Run128(21);
    base6_6014 = (s32)gVal2;
    Motion_SetVarCbObj_1(20, 2);
    Object_SetModeById_1(20, 4);
    Actor_Run129(base6_6014);
    Motion_ArmCb_2(21, 0xd000, 0);
    Motion_ArmCb_3(22, 0xd000, 0);
    BattleFx_SpawnLinked_1(21, 0x101, 0);
    BattleFx_SpawnLinked_2(22, 0x101, 60);
    Object_SetModeById_2(20, 3);
    Actor_Run130(base6_6014);
    Battle_WaitMode0_2(21, 0x102);
    Battle_WaitMode0_3(22, 0x102);
    Battle_WaitMode0_4(80);
    BattleFx_SpawnLinked_3(21, 0x100, 20);
    Motion_SetSpeed_3(21, 0x19999, 0xcccc);
    Motion_SetPosReset_2(21, 194, 0x1f4);
    Motion_ArmCb_4(21, 0xb000, 20);
    Motion_SetSpeed_4(22, 0xcccc, 0x6666);
    Motion_SetPosReset_3(22, 192, 0x206);
    Motion_ArmCb_5(22, 0xb000, 0);
    Motion_SetSpeed_5(20, 0x10000, 0x8000);
    Motion_SetPosReset_4(20, 210, 0x1fc);
    Actor_Check16(20, 0xb000);
    Motion_SetVarCbObj_2(21, 1);
    Actor_Do13(0x5015);
    Motion_CallWaitAnim_1(20, 3);
    Actor_Run7(22, 0xd000);
    Actor_Do14(0x9016);
    Motion_ArmCb_6(20, 0x8000, 20);
    Object_SetModeById_3(20, 4);
    Actor_Do15(0xa014);
    Motion_SetPosReset_5(20, 204, 0x218);
    Motion_ArmCb_7(22, 0xb000, 0);
    Motion_SetPosReset_6(20, 182, 0x224);
    Motion_SetPosReset_7(20, 182, 0x250);
    Motion_ResetPosMode2_1(20, 182, 0x298);
    Battle_WaitMode0_5(40);
    Actor_Run131();
    Motion_SetSpeed_6();
    Actor_Run132(16);
}

/* overlays/scene/actor/multi_encounter/run_scene_configure_lead_actors.c */
extern u8 gWork[];

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void Scene_ConfigureLeadActors(void)
{
    u8 *record;

    Battle_Reset_1();
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Actor_Run133(1);
    Motion_SetHPosTerrain_1(20, 0xb60000, 0x26a0000);
    Motion_SetHPosTerrain_2(23, 0xee0000, 0x2720000);
    Motion_SetHPosTerrain_3(22, 0x10c0000, 0x2a60000);
    record = Scene_GetRecord_1(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_EnableActCb_1(22, 0x200c980);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = Scene_GetRecord_2(21);
        u8 flags = *(volatile u8 *)&record[89];

        record[89] = (u8)(flags | 128);
    }
    Motion_SetSpeed_1(21, 0xcccc, 0x6666);
    Motion_EnableActCb_2(21, 0x200c628);
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1();
    Motion_SetSpeed_2();
    Battle_WaitMode0_2(20);
    Motion_SetSpeed_3(20, 0x19999, 0xcccc);
    Motion_SetPosReset_1(20, 182, 0x224);
    Actor_Run134(20, 0);
    Actor_Check17(0, 0x8000);
    Motion_SetVarCbObj_1(20, 1);
    SceneWork_SetStepValue_1(0x1ee1);
    ((void (*)())Actor_Check43)(20);
    Motion_CallWaitAnim_1(0, 3);
    Battle_WaitMode0_3(40);
    Motion_ArmCb_1(20, 0x5000, 20);
    BattleFx_SpawnLinked_1(20, 0x105, 60);
    Battle_RunThenWaitIfModeZero_1(20, 0, 40);
    Actor_Run135(20, 0);
    Actor_Run136(20);
    Motion_CallWaitAnim_2(0, 3);
    Motion_CallWaitAnim_3(20, 3);
    Motion_SetPosReset_2(20, 182, 0x258);
    Motion_SetPosReset_3(20, 216, 0x258);
    Actor_Run8(20, 0xc000);
    Actor_Run137();
    Battle_WaitMode0_4(10);
    Motion_SetPosReset_4(20, 216, 0x244);
    Motion_SetHPosTerrain_4(20, 0, 0);
    SCENE_PHASE = 0x209;
    GameFlag_Set_1(0x92b);
    GameFlag_Clear_1(0x302);
    Battle_SchedShoulder_1();
}

/* overlays/scene/actor/multi_encounter/run_scene_encounter_closing_sequence.c */
extern u8 gVal3[];
extern u8 gOv7[];

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE (*(s32 *)(SCENE_WORK + 0x1c0))

/* Sets up background/camera parameters and either takes the short branch
 * (branch value 1 from UiWork_WaitThenFinalizeCapacity_1) or runs a longer sequence that moves
 * and reconfigures actors 20, 21, 22 and 26 before advancing the scene
 * phase to 0x201 and finishing the scene. */
void Scene_RunEncounterClosingSequence(void)
{
    u32 i;
    s32 record;
    s32 data_00002014_addr;
    s32 data_0200c918_addr;

    Battle_Reset_1();
    Object_NotifyLastActiveOfEvent_1(0x200d1d8);
    Motion_SetVarCbObj_3(1);
    Battle_WaitMode0_1();
    Motion_SetSpeed_1(0, 0x10000, 0x8000);
    Motion_SetPosReset_1(0, 148, 0x290);
    BattleFx_SpawnLinked_1(22, 0x100, 0);
    Motion_SetVarCbObj_1(22, 1);
    Actor_Run138(22, 0x5000);
    SceneWork_SetStepValue_1(0x1f69);
    Motion_SetSpeed_2(0x2016, 0);
    Motion_ArmCb_1(0, 0xe000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        Actor_Do16(0x2016);
        Battle_SchedShoulder_1();
    } else {
        bump_step(1);
        BattleEventRuntime_ProcessAction_1(0x2016, 0);
        Actor_Run139();
        Motion_SetHPosTerrain_1(26, 0xd80000, 0x24c0000);
        Motion_SetSpeed_3(26, 0x13333, 0x9999);
        Motion_SetPosReset_2(26, 216, 0x254);
        Motion_SetPosReset_3(26, 188, 0x268);
        Motion_ArmCb_2(0, 0xe000, 0);
        Motion_ArmCb_3(21, 0xd000, 0);
        Motion_ArmCb_4(22, 0xd000, 0);
        Actor_Check18(26, 0x5000);
        Motion_Launch_1(26, 2, 0);
        Object_SetModeById_1(26, 4);
        BattleEv_RunWait_1(26, 0);
        Motion_SetHPosTerrain_2(20, 0xb40000, 0x3090000);
        Motion_SetSpeed_4(20, 0x10000, 0x8000);
        Motion_SetPosReset_4(20, 180, 0x298);
        data_00002014_addr = (s32)gVal3;
        Motion_ArmCb_5(20, 0xd000, 0);
        Actor_Run140(data_00002014_addr);
        Motion_ArmCb_6(0, 0x2000, 0);
        Motion_ArmCb_7(22, 0x3000, 0);
        BattleFx_SpawnLinked_2(26, 0x101, 60);
        Motion_SetVarCbObj_2(20, 1);
        Actor_Run141(data_00002014_addr);
        Motion_SetVarCb_1(21, 2);
        Actor_Run142(21);
        Motion_ArmCb_8(20, 0x5000, 20);
        Object_SetModeById_2(20, 3);
        Actor_Do17(0x6014);
        Motion_Launch_2(26, 2, 20);
        Object_SetModeById_3(26, 4);
        Actor_Run143(26);
        Motion_SetPosReset_5(20, 182, 0x280);
        Motion_ArmCb_9(20, 0xd000, 0);
        Actor_Do18(0x8014);
        BattleFx_SpawnLinked_3(26, 0x100, 20);
        Motion_SetVarCb_2(26, 2);
        Actor_Run144(26);
        Motion_CallWaitAnim_1(20, 3);
        Actor_Run145(22, 0);
        Motion_SetVarCbObj_3(22, 1);
        Actor_Run146(22);
        Motion_SetSpeed_5(22, 0x19999, 0xcccc);
        data_0200c918_addr = (s32)gOv7;
        Motion_EnableActCb_1(22, data_0200c918_addr);
        Motion_SetSpeed_6(21, 0x19999, 0xcccc);
        Motion_SetPosReset_6(21, 168, 0x278);
        Motion_EnableActCb_2(21, data_0200c918_addr);
        Battle_WaitMode0_2(80);
        Motion_EnableActCb_3(26, data_0200c918_addr);
        Battle_WaitMode0_3(40);
        Actor_Run9(20, 0x8000);
        Actor_Run147(data_00002014_addr);
        Actor_Place33(0, 0xe000);
        Motion_CallWaitAnim_2(0, 3);
        Motion_CallWaitAnim_3(20, 3);
        SCENE_PHASE = 0x201;
        Actor_Run148();
        Motion_SetSpeed_7();
        Actor_Run149(17);
    }
}

/* overlays/scene/actor/multi_encounter/run_scene_three_actor_encounter.c */

u8 *Scene_GetRecord_5();

u8 *Scene_GetRecord_7();

u8 *Scene_GetRecord_5();
u8 *Scene_GetRecord_7();

/* Offset of a flag byte on an actor record, cleared and set below. */
#define ACTOR_FLAGS_OFFSET 90

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void Scene_RunThreeActorEncounter(void)
{
    u8 *record;

    Battle_Reset_1();
    Motion_SetSpeed_1(0, 0x10000, 0x8000);
    Motion_SetPosReset_1(0, 180, 0x28e);
    Motion_ArmCb_1(0, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_SetHPosTerrain_3(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetSpeed_2(1, 0x13333, 0x9999);
    Motion_SetSpeed_3(2, 0x10000, 0x8000);
    Motion_SetSpeed_4(3, 0x13333, 0x9999);
    Motion_ResetPosMode2_1(1, 194, 0x280);
    Motion_ResetPosMode2_2(2, 198, 0x28e);
    Motion_SetPosReset_2(3, 194, 0x2a0);
    Object_SetModeById_1(1, 1);
    Object_SetModeById_2(2, 1);
    Battle_WaitMode0_1(10);
    Motion_ArmCb_2(1, 0x8000, 0);
    Motion_ArmCb_3(2, 0x8000, 0);
    Actor_Check19(3, 0x8000);
    Actor_Run150(22, 0);
    SceneWork_SetStepValue_1(0x1f55);
    Actor_Do19(22);
    Actor_Check20(21, 0xd000);
    Battle_RunThenWaitIfModeZero_1(21, 0, 40);
    BattleFx_SpawnLinked_1(22, 0x100, 20);
    Motion_SetVarCbObj_1(22, 1);
    Motion_SetSpeed_5(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        Motion_CallWaitAnim_1(2, 4);
        Actor_Run151(2);
        Actor_Check21(3, 0xa000);
        Object_SetModeById_3(3, 3);
        Actor_Run152(3);
        Actor_Check22(1, 0x6000);
        Motion_SetVarCbObj_2(1, 1);
        Motion_SetSpeed_6(1, 0);
        L_02003dfa:;
        if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 1) {
            Motion_SetVarCbObj_3(2, 1);
            SceneWork_SetStepValue_2(0x1f53);
            Motion_SetSpeed_7(2, 0);
            goto L_02003dfa;
        }
    }
    Battle_WaitMode0_2(20);
    Motion_CallWaitAnim_2(22, 3);
    SceneWork_SetStepValue_3(0x1f5b);
    Actor_Run153(22);
    Motion_SetSpeed_8(22, 0x10000, 0x8000);
    Motion_SetSpeed_9(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)(Scene_GetRecord_4(22) + ACTOR_FLAGS_OFFSET) &= 254;
    Motion_SetPosReset_3(22, 162, 0x27a);
    Battle_WaitMode0_3(1);
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = Scene_GetRecord_5(22);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)(Scene_GetRecord_6(21) + ACTOR_FLAGS_OFFSET) &= 254;
    Motion_SetPosReset_4(21, 162, 0x2a4);
    Battle_WaitMode0_4(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = Scene_GetRecord_7(21);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    Motion_ArmCb_4(22, 0x3000, 0);
    Actor_Run10(21, 0xd000);
    Actor_Run154(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    Motion_ResetPosMode2_3(1, 180, 0x28e);
    Motion_ResetPosMode2_4(2, 180, 0x28e);
    Motion_SetPosReset_5(3, 180, 0x28e);
    ObjectTable_DestroyById_1(1);
    ObjectTable_DestroyById_2(2);
    ObjectTable_DestroyById_3(3);
    GameFlag_Set_1(0x903);
    Battle_SchedShoulder_1();
}

/* overlays/scene/actor/multi_encounter/scene_data.c */
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 Value_0000093e;
extern u8 Value_00000927;
extern u8 Value_00000928;
extern u8 Value_00000911;
extern u8 Value_00000925;
extern u8 Value_00000922;
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern s32 gOv20[];

/* Contiguous unnamed leaf-owner run for resource_3af. */

u8 *SceneData_GetTablec994(void)
{
    return gOv8;
}

u8 *SceneData_GetTablecb44(void)
{
    return gOv9;
}

u8 *SceneData_GetTablecb64(void)
{
    return gOv10;
}

u8 *SceneData_SelectTableByFlags(void)
{
    s32 v;

    if (Actor_Check44((s32)&Value_0000093e))
        return gOv11;
    if (Actor_Check45((s32)&Value_00000927))
        return gOv12;
    v = Actor_Check46((s32)&Value_00000928);
    if (v != 0)
        return gOv13;
    if (Actor_Check47((s32)&Value_00000911)) {
        if (Actor_Check48((s32)&Value_00000925)) {
            gOv14[0x14E] = v;
            gOv14[0x1AE] = 2;
            gOv14[0x1C6] = 2;
        } else if (Actor_Check49((s32)&Value_00000922)) {
            gOv14[0x1AE] = 1;
            gOv14[0x1C6] = 1;
        }
        return gOv14;
    }
    return gOv15;
}

u8 *SceneData_SelectTableByThreeFlags(void)
{
    if (Actor_Check50((s32)&Value_0000093e))
        return gOv16;
    if (Actor_Check51(0x8A0))
        return gOv17;
    if (Actor_Check52((s32)&Value_00000928))
        return gOv18;
    return gOv19;
}

s32 State_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Actor_Check53(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Actor_Check54(base + i) != 0) return gOv20[i];
    }
    return 0;
}

/* overlays/scene/actor/multi_encounter/scene_dialogue.c */
extern unsigned char Value_00001f00;

/*
 * One-line dialogue callback in overlay resource_3af.  It is installed into
 * the script-record table rather than called.
 */

/*
 * Open the scripted-scene bracket, speak the line, hand actor 21 back, close
 * the bracket.  The 32-byte owner ends with its one pool word, 0x1f00, the
 * dialogue line id: it is passed as the address of Value_00001f00 so the id
 * stays a pool word and is not folded into an immediate.  The actor and line
 * ids are transcribed only, and what the installing record represents is not
 * established.
 */
void Dialogue_RunActor21Line(void)
{
    Actor_Run155();
    Actor_Do32((s32)&Value_00001f00);
    Actor_Apply12(21, 0);
    Actor_Run156();
}

/* overlays/scene/actor/multi_encounter/scene_motion.c */

s32 Effect_UpdateCountdownMotion(struct Resource3afMotion *motion)
{
    switch (motion->countdown) {
    case 6:
        motion->component_a += (s32) 0xFFFFC000;
        motion->component_b += 0x2000;
        break;
    case 4:
        motion->component_a += 0x2000;
        /* The loader relocates the stored pool word to -0x1000. */
        motion->component_b -= 0x1000;
        break;
    case 2:
        motion->component_a += 0x1000;
        motion->component_b += (s32) 0xFFFFF800;
        break;
    case 0:
        motion->component_a = 0x10000;
        motion->component_b = 0x10000;
        motion->countdown =
            (s16)(BuildMotionCountdown(GetMotionCountdownInput(), 90) + 60);
        break;
    }
    motion->countdown--;
    return 1;
}

/* overlays/scene/actor/multi_encounter/scene_setup.c */
extern u8 *gCam;
extern u32 gOv21;
extern u32 gOv22;
extern u32 gOv4[];
extern u32 gOv5;
extern u16 gOv2[];
extern u16 gOv[];
extern s32 gOv3[];
extern s32 Actor_Far[];

s32 *Actor_Run157();
s32 *Actor_Run158();

s32 *Actor_Run159(s32);
s32 *Actor_Run160(s32);
s32 *Actor_Run161(s32);

s32 *Actor_Run143(s32);
s32 *Actor_Run162(s32);
s32 *Actor_Run163(s32);
s32 *Actor_Run164(s32);
s32 *Actor_Run165(s32);
s32 *Actor_Run144(s32);
s32 *Actor_Run166(s32);
s32 *Actor_Run167();
s32 *Actor_Run168();
s32 *Actor_Run145(s32);

u8 *Actor_Run169(s32);
u8 *Actor_Run170(s32);
u8 *Actor_Run171(s32);
u8 *Actor_Run172(s32);
u8 *Actor_Run173(s32);
u8 *Actor_Run174(s32);
u8 *Actor_Run175(s32);
u8 *Actor_Run176(s32);

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#endif

s32 State_ResetWorkValuesAndStart(void)
{
    u8 **base = &gCam;
    u8 *p = base[0] + 0x104;

    Actor_Do33(0x11C);
    *(s32 *)(base[19] + 0x1C0) = 0x209;
    *(s32 *)(p + 0x1C) = 0;
    gOv21 = (u16)Actor_Check55();
    gOv22 = (u16)Actor_Check56();
    gOv4[0] = 0;
    gOv4[1] = 0;
    gOv5 = 0;
    Actor_Run177();
    Actor_Do34(1);
    Actor_Run178();
    return 0;
}

void State_ConfigureEntries8Through19(void)
{
    u16 *p;

    Actor_Apply13(8, 0);
    p = gOv2;
    {
        s32 z = 0;

        p[0] = z;
    }
    Actor_Apply14(9, 1);
    p[1] += 0x80;
    Actor_Apply15(10, 2);
    p[2] += 0x100;
    Actor_Apply16(11, 3);
    p[3] += 0x200;
    Actor_Place42(12, 0, 0);
    Actor_Place43(13, 1, 0);
    Actor_Place44(14, 2, 0);
    Actor_Place45(15, 3, 0);
    Actor_Place46(16, 4, 1);
    Actor_Place47(17, 5, 1);
    Actor_Place48(18, 6, 1);
    Actor_Place49(19, 7, 1);
}

void State_InitActorSlots8To19(void)
{
    {
        u16 *p = gOv;
        u32 i;

        for (i = 0; i < 8; i++) p[i] = 0xC000;
    }
    Actor_Do35(8);
    ((void (*)())Actor_Run157)(9, 0, 0);
    Actor_Place50(10, 0, 0);
    Actor_Place51(11, 0, 0);
    Actor_Place52(12, 0, 0);
    Actor_Do36(13);
    Actor_Do37(14);
    Actor_Do38(15);
    {
        s32 *t = gOv3;
        s32 z = 0;
        t[0] = z;
        t[1] = z;
        t[2] = z;
        t[3] = z;
        Actor_Far[0] = Actor_Run159(8)[4];
        Actor_Far[1] = Actor_Run160(13)[4];
        Actor_Far[2] = Actor_Run158(14)[4];
        Actor_Far[3] = Actor_Run161(15)[4];
        Actor_Do39(16);
        Actor_Do40(17);
        Actor_Do41(18);
        Actor_Do42(19);
        {
            s32 v = 0xFFFF0000;

            Actor_Run143(16)[6] = v;
            Actor_Run162(17)[6] = v;
            Actor_Run163(18)[6] = v;
            Actor_Run164(19)[6] = v;
        }
        t[4] = z;
        t[5] = z;
        t[6] = z;
        t[7] = z;
        Actor_Far[4] = Actor_Run165(16)[4];
        Actor_Far[5] = Actor_Run144(17)[4];
        Actor_Far[6] = Actor_Run166(18)[4];
        Actor_Far[7] = Actor_Run167(19)[4];
    }
    {
        s32 *w = Actor_Run145(0);

        if (w != 0) Actor_Place53(8, w[2], w[4]);
    }
    Actor_Do43(1);
    Actor_Apply17(13, 8);
    Actor_Apply18(14, 8);
    Actor_Apply19(15, 8);
    Actor_Apply20(16, 8);
    Actor_Apply21(17, 8);
    Actor_Apply22(18, 8);
    Actor_Apply23(19, 8);
    {
        s32 o = 1;

        Actor_Run169(8)[92] = o;
        Actor_Run170(13)[92] = o;
        Actor_Run171(14)[92] = o;
        Actor_Run172(15)[92] = o;
        Actor_Run173(16)[92] = o;
        Actor_Run174(17)[92] = o;
        Actor_Run175(18)[92] = o;
        Actor_Run176(19)[92] = o;
    }
    Actor_Run168(1);
    Actor_Place54(8, 0x840000, 0x2780000);
    Actor_Do44(1);
    Actor_Place55(8, 0, 2);
    Actor_Place56(13, 1, 2);
    Actor_Place57(14, 2, 2);
    Actor_Place58(15, 3, 2);
    Actor_Place59(16, 4, 3);
    Actor_Place60(17, 5, 3);
    Actor_Place61(18, 6, 3);
    Actor_Place62(19, 7, 3);
}

/* overlays/scene/actor/multi_encounter/scene_state_interaction.c */
extern s32 gOv3[];

void State_FillTableDb70ByChecks(void)
{
    {
        s32 *p = gOv3;
        u32 i = 0;
        s32 t = 3;

        for (i = 0; i < 4; i++) {
            if (Actor_Check57(i)!= 0) p[i] = Actor_Check58(i);
            else p[i] = t;
        }
    }
    if (Actor_Check59(0) != 0) gOv3[0] = Actor_Check60(0);
    else gOv3[0] = 3;
    if (Actor_Check61(2) != 0) gOv3[1] = Actor_Check62(2);
    else gOv3[1] = 3;
    {
        s32 *q = gOv3;
        s32 t = 3;

        q[2] = t;
        q[3] = t;
        if (Actor_Check63(1) != 0) q[4] = Actor_Check64(1);
        else q[4] = t;
    }
    if (Actor_Check65(3) != 0) gOv3[5] = Actor_Check66(3);
    else gOv3[5] = 3;
    gOv3[6] = 3;
    gOv3[7] = 3;
}

/* overlays/scene/actor/multi_encounter/shared.c */

void Scene_RunStepThen10(s32 a)
{
    Actor_Apply24(a, 0);
    Actor_Do45(10);
}

void Scene_CallPairWith10(s32 a, s32 b)
{
    Actor_Place63(a, b, 10);
}

/* overlays/scene/actor/multi_encounter/state_update.c */

s32 State_ApplyArgMode1AndReturnZero(s32 a)
{
    Actor_Apply25(a, 1);
    return 0;
}

void State_ApplyFiveRectsAtColumn78(void)
{
    Actor_SetRect6(78, 39, 78, 40, 5, 1);
    Actor_SetRect7(78, 39, 78, 41, 5, 1);
    Actor_SetRect8(78, 39, 79, 42, 4, 1);
    Actor_SetRect9(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Actor_SetRect10(17, 38, 5, 2, x, y);
    }
}
