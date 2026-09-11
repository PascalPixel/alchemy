#include "types.h"
#include "scene.h"
#include "resource_3af_motion.h"

/* overlays/scene/actor/multi_encounter/multi_encounter.c */
/* overlays/scene/actor/multi_encounter/actor_facing.c */

s32 Actor_SetFacingFromSample(u8 *a)
{
    u32 v = ((u32)(Actor_unk23() << 5)) >> 16;

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
        val = *(s32 *)(obj + 0xc) - (((u32)(Actor_unk24() << 15)) >> 16) - 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = *(s32 *)(obj + 0xc) + (((u32)(Actor_unk25() << 15)) >> 16) + 0x8000;
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
    if ((((u32)(Actor_unk26() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}

/* overlays/scene/actor/multi_encounter/actor_position.c */

u8 *Actor_unk11_4(s32);

u8 *Actor_unk12_4(s32);

u8 *Actor_unk13_4(s32);

u8 *Actor_unk14_4(s32);

u8 *Actor_unk15_4(s32);

void Actor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_unk34_3(21, 0x1060000, 0x2C20000);
    *(u16 *)(Actor_unk11_4(21) + 6) = m;
    Actor_unk35_3(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Actor_unk12_4(24) + 6) = z;
    }
    Actor_Apply(24, 1);
    Actor_unk36_3(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Actor_unk13_4(25) + 6) = x << 8;
    }
    Actor_Apply2(25, 1);
    Actor_unk37_3(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Actor_unk14_4(26) + 6) = x << 8;
    }
    Actor_unk38_3(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Actor_unk15_4(27) + 6) = m;
    Actor_unk39_3(22, 0, 0);
    Actor_unk40_3(23, 0, 0);
    Actor_unk41_3(20, 0, 0);
}

/* overlays/scene/actor/multi_encounter/dialogue_layout.c */

void DialogueLayout_ConfigureTwoRegions(void)
{
    Actor_unk4_5(66, 61, 64, 40, 5, 4);
    Actor_unk5_5(0, 0, 5, 4, 5, 39);
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

u8 *Actor_unk16_4(s32);
u8 *Actor_unk17_4(s32);
u8 *Actor_unk18_4(s32);
u8 *Actor_unk19_4(s32);

s32 *Actor_unk20_4(s32);
s32 *Actor_unk21_4(s32);
s32 *Actor_unk22_4(s32);
s32 *Actor_unk23_4(s32);

s32 *Actor_unk24_4(s32);
s32 *Actor_unk25_4(s32);
s32 *Actor_unk26_4(s32);
s32 *Actor_unk27_4(s32);
s32 *Actor_unk28_4(s32);
s32 *Actor_unk29_4(s32);
s32 *Actor_unk30_4(s32);
s32 *Actor_unk31_4(s32);

void Effect_InitSlotsEightToNineteen(void)
{
    {
        s32 z = 0;
        *(Actor_unk16_4(8) + 89) = z;
        *(Actor_unk17_4(9) + 89) = z;
        *(Actor_unk18_4(10) + 89) = z;
        *(Actor_unk19_4(11) + 89) = z;
    }
    Actor_unk20_2(8);
    Actor_unk21_2(9);
    Actor_unk22_2(10);
    Actor_unk23_2(11);
    Actor_unk24_2(12);
    Actor_unk25_2(13);
    Actor_unk26_2(14);
    Actor_unk27_2(15);
    {
        Actor_Far[0] = Actor_unk20_4(12)[4];
        Actor_Far[1] = Actor_unk21_4(13)[4];
        Actor_Far[2] = Actor_unk22_4(14)[4];
        Actor_Far[3] = Actor_unk23_4(15)[4];
        Actor_unk28_2(16);
        Actor_unk29_2(17);
        Actor_unk30_2(18);
        Actor_unk31_2(19);
        {
            s32 v = 0xFFFF0000;

            Actor_unk24_4(16)[6] = v;
            Actor_unk25_4(17)[6] = v;
            Actor_unk26_4(18)[6] = v;
            Actor_unk27_4(19)[6] = v;
        }
        Actor_Far[4] = Actor_unk28_4(16)[4];
        Actor_Far[5] = Actor_unk29_4(17)[4];
        Actor_Far[6] = Actor_unk30_4(18)[4];
        Actor_Far[7] = Actor_unk31_4(19)[4];
    }
    Actor_unk32_4();
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
    s32 *p = (s32 *)Actor_unk27(a);
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
        t = Actor_unk28(gOv[b]);
        q[7].h[1] = Actor_unk29(gOv[b] + 0x8000) >> 5;
        p[4] = Actor_Far[b] - (t << 2) - (t << 1);
    } else {
        t = Actor_unk30(gOv[b] + 0x8000);
        q[7].h[1] = Actor_unk31(gOv[b]) >> 5;
        p[4] = Actor_Far[b] + (t << 2) + (t << 1);
    }
}

/* overlays/scene/actor/multi_encounter/field_scene_head.c */

extern u8 gWork[];
extern u8 gVal[];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Actor_unk33_4();

u8 *Actor_unk34_4();

u8 *Actor_unk35_4();

u8 *Actor_unk36_4();

u8 *Actor_unk37_4();
u8 *Actor_unk38_4();

void Scene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Actor_unk32)();
    if (Actor_Check(0x925) != 0) {
        Actor_Do(0x1e08);
        Actor_unk39_4(21, 0);
    } else {
        if (Actor_unk2(0x922) != 0) {
            Actor_unk40_4(21, 2);
            Actor_unk2_2(0x1d6f);
            Actor_unk41_4(21, 0);
            rec7 = Actor_unk3(21);
            record = Actor_unk33();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Actor_unk42_4(21, 0x200c4d8);
        } else {
            Actor_Place(21, 0x103, 0);
            Actor_unk43_4(21, 3);
            Actor_unk3_2(0x1d36);
            Actor_unk44_4(21, 0);
        }
    }
    Actor_unk45_4();
}

void Scene_RunScriptSequenceA(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Actor_unk34)();
    if (Actor_unk4(0x925) != 0) {
        Actor_unk4_2(0x1e09);
        Actor_unk47_4(24, 0);
    } else {
        if (Actor_unk5(0x922) != 0) {
            Actor_unk48_4(24, 2);
            Actor_unk5_2(0x1d70);
            Actor_unk49_4(24, 0);
            rec7 = Actor_unk6(24);
            record = Actor_unk35();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Actor_unk50_4(24, 0x200c4d8);
        } else {
            Actor_unk2_3(24, 0x103, 0);
            Actor_unk51_4(24, 3);
            Actor_unk6_2(0x1d37);
            Actor_unk52_4(24, 0);
        }
    }
    Actor_unk53_4();
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    if (Actor_unk7(0x271) == 0) {
        Actor_unk54_4(158);
        Actor_SetRect(30, 94, 13, 94, 1, 3);
        Actor_unk7_2(0x271);
    }
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    if (Actor_unk8(0x272) == 0) {
        Actor_unk55_4(158);
        Actor_unk2_5(30, 108, 13, 108, 1, 2);
        Actor_unk8_2(0x272);
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
        Actor_unk56_4();
        Motion_SetAngleToward_1(0, 20, 10);
        Motion_SetSpeedLim_1(0x19999, 0x3333);
        Motion_CamBounds_1(0xbe0000, -1, 0x2c40000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        Battle_WaitMode0_1(40);
        Motion_SetVarCbObj_1(22, 1);
        SceneWork_SetStepValue_1(0x1d26);
        Actor_unk57_4(0x4016);
        BattleFx_SpawnLinked_1(20, 0x102, 60);
        Motion_SetVarCb_1(20, 2);
        Actor_unk58_4(20);
        Motion_SetVarCbObj_2(22, 1);
        Motion_ArmCb_1(22, 0x5000, 0);
        Actor_unk59_4(0x4016);
        Motion_SetVarCbObj_3(20, 1);
        Actor_unk60_4(20, 0xb000);
        Actor_unk61_4(20);
        Actor_unk62_4(23, 0x3000);
        Object_SetModeById_1(23, 3);
        Actor_unk63_4(0x4017);
        BattleFx_SpawnLinked_2(22, 0x101, 40);
        Motion_ArmCb_2(22, 0x8000, 20);
        Actor_unk64_4(0x4016);
        Actor_unk65_4(23, 0);
        Motion_CallWaitAnim_1(23, 4);
        Actor_unk66_4(0x4017);
        BattleFx_SpawnLinked_3(20, 0x100, 40);
        Motion_SetVarCb_2(20, 2);
        Actor_unk67_4(20);
        Motion_CallWaitAnim_2(22, 3);
        Actor_unk68_4(0x4016);
        Actor_unk69_4(20, 0xd000);
        Object_SetModeById_2(23, 3);
        Motion_CallWaitAnim_3(20, 3);
        Battle_WaitMode0_2(60);
        BattleFx_SpawnLinked_4(22, 0x106, 40);
        Actor_unk70_4(22, 0x5000);
        SceneWork_SetStepValue_2((s32)gVal);
        Motion_SetVarCb_3(22, 1);
        Actor_unk71_4(0x4016);
        BattleFx_SpawnLinked_5(20, 0x101, 40);
        Motion_SetVarCb_4(20, 2);
        Actor_unk72_4(20);
        BattleFx_SpawnLinked_6(22, 0x108, 20);
        Battle_RunThenWaitIfModeZero_1(0x4016, 0, 20);
        BattleFx_SpawnLinked_7(23, 0x102, 60);
        Actor_unk73_4(0x4017);
        Actor_unk9(22, 0x8000);
        Motion_CallWaitAnim_4(22, 3);
        Battle_RunThenWaitIfModeZero_2(0x4016, 0, 20);
        BattleFx_SpawnLinked_8(20, 0x102, 40);
        Motion_SetVarCb_5(20, 2);
        Actor_unk74_4(20);
        Actor_unk75_4(22, 0x5000);
        Object_SetModeById_3(22, 4);
        Actor_unk76_4(22);
        Motion_ArmCb_3(20, 0xb000, 0);
        Motion_ArmCb_4(23, 0x3000, 40);
        Motion_ArmCb_5(23, 0, 0);
        Motion_ArmCb_6(20, 0xd000, 20);
        Motion_SetVarCbObj_4(22, 2);
        Battle_WaitMode0_3(20);
        Actor_unk77_4(0x4016);
        Battle_WaitMode0_4(23, 0x102);
        Battle_WaitMode0_5(20, 0x102);
        Battle_WaitMode0_6(40);
        Motion_CallWaitAnim_5(22, 3);
        Actor_unk78_4(0x4016);
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
        Actor_unk79_4(20, 0xd000);
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
        Actor_unk80_4(20, 0xd000);
        Motion_SetVarCbObj_5(20, 2);
        Actor_unk81_4();
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

    if (Actor_unk10(0x911) != 0) {
        if (Actor_unk11(0x922) == 0) {
            Actor_unk82_4();
            Actor_unk83_4();
            Actor_unk84_4();
            Actor_unk3_3(20, 0x6666, 0x3333);
            *(u8 *)(Actor_unk36(20) + 90) &= 254;
            Actor_unk4_3(20, 232, 0x330);
            Actor_unk85_4(1);
            *(u8 *)(Actor_unk37(20) + 90) |= 1;
            Actor_unk86_4(20);
            Actor_unk87_4(20, 2);
            Actor_unk88_4(20);
            Actor_unk5_3(20, 0x13333, 0x9999);
            *(u8 *)(Actor_unk38(20) + 90) &= 254;
            Actor_unk6_3(20, 244, 0x324);
            Actor_unk89_4(1);
            {
                u8 *record = Actor_unk33_4(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_unk90_4(20);
            Actor_unk7_3(20, 0x33333, 0x19999);
            Actor_unk8_3(20, 248, 0x30a);
            Actor_unk9_3(20, 248, 0x2bc);
            Actor_unk10_3(20, 0xf60000, 0x2000000);
            Actor_unk91_4(20, 0, 0);
            Actor_unk11_3(0, 0x101, 60);
            Actor_unk92_4();
        }
    }
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    if (Actor_unk12(0x911) != 0) {
        if (Actor_unk13(0x922) == 0) {
            Actor_unk93_4();
            Actor_unk94_4();
            Actor_unk95_4();
            Actor_unk12_3(20, 0x6666, 0x3333);
            *(u8 *)(Actor_unk39(20) + 90) &= 254;
            Actor_unk13_3(20, 202, 0x330);
            Actor_unk96_4(1);
            *(u8 *)(Actor_unk40(20) + 90) |= 1;
            Actor_unk97_4(20);
            Actor_unk98_4(20, 2);
            Actor_unk99_4(20);
            Actor_unk14_3(20, 0x13333, 0x9999);
            *(u8 *)(Actor_unk41(20) + 90) &= 254;
            Actor_unk15_3(20, 192, 0x324);
            Actor_unk100_4(1);
            {
                u8 *record = Actor_unk34_4(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_unk101_4(20);
            Actor_unk16_3(20, 0x33333, 0x19999);
            Actor_unk17_3(20, 180, 0x30a);
            Actor_unk18_3(20, 180, 0x2bc);
            Actor_unk19_3(20, 0xf60000, 0x2000000);
            Actor_unk102_4(20, 0, 0);
            Actor_unk20_3(0, 0x101, 60);
            Actor_unk103_4();
        }
    }
}

void Scene_RunScript(void)
{
    u32 i;
    s32 record;

    Actor_Run(0x19999, 0x3333);
    Actor_unk2_4(0xd80000, -1, 0x3380000, 1);
    Actor_unk104_4();
    Actor_unk105_4(20);
    Actor_unk106_4();
    Actor_unk3_5(30, 108, 13, 108, 1, 2);
    Actor_unk107_4(10);
    Actor_unk21_3(20, 0xd80000, 0x3200000);
    Actor_unk22_3(20, 0x13333, 0x9999);
    Actor_unk23_3(20, 216, 0x32e);
    Actor_unk108_4(0, 20, 10);
    Actor_unk109_4(20, 4);
    Actor_unk110_4(20, 2);
    Actor_unk24_3(20, 0x100, 20);
    Actor_unk111_4(20, 0, 20);
    Actor_unk112_4(20, 2);
    Actor_unk9_2(0x1d8d);
    Actor_unk113_4(20, 0, 20);
    Actor_unk25_3(20, 0x102, 0);
    Actor_unk10_2(0x923);
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    Actor_unk114_4();
    Actor_unk11_2(0x200d160);
    Actor_unk115_4(1);
    Actor_unk116_4(20, 0, 0);
    Actor_unk26_3(23, 0xee0000, 0x2720000);
    Actor_unk27_3(22, 0xcc0000, 0x2090000);
    record = Actor_unk35_4(22);
    *(volatile s32 *)((s32)record + 12) = 0x100000;
    *(u8 *)(Actor_unk42(22) + 89) |= 128;
    Actor_unk28_3(22, 0x9999, 0x4ccc);
    Actor_unk3_4(22, 0x200c58c);
    {
        u8 *record = Actor_unk36_4(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Actor_unk29_3(21, 0xcccc, 0x6666);
    Actor_unk4_4(21, 0x200c628);
    if (Actor_unk14(0x109) != 0) {
        Actor_unk117_4();
    }
    Actor_unk118_4();
}

void Scene_RunScript(void)
{
    u32 i;
    u8 *record;

    Actor_unk119_4();
    Actor_unk12_2(0x200d160);
    Actor_unk120_4(1);
    Actor_unk121_4(20, 0, 0);
    Actor_unk30_3(23, 0xee0000, 0x2720000);
    Actor_unk31_3(22, 0x10c0000, 0x2a60000);
    record = Actor_unk37_4(22);
    {
        s32 shown = 0;

        *(volatile u16 *)((s32)record + 6) = shown;
    }
    Actor_unk5_4(22, 0x200c980);
    {
        u8 *record = Actor_unk38_4(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Actor_unk32_3(21, 0xcccc, 0x6666);
    Actor_unk6_4(21, 0x200c628);
    if (Actor_unk15(0x109) != 0) {
        Actor_unk122_4();
    }
    Actor_unk123_4();
}

/* overlays/scene/actor/multi_encounter/overlay_object.c */
u8 *Actor_unk124_4(s32);

void OvObj_DecayFields24And28(u8 *o)
{
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}

void OvObj_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Actor_unk124_4(a);

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
    Actor_unk125_4(1);
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
    Actor_unk126_4(record, 0);
    Actor_unk127_4(1);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Motion_SetSpeed_2(21, 0xcccc, 0x6666);
    Motion_SetPosReset_1(21, 182, 0x214);
    Motion_ArmCb_1(21, 0xb000, 40);
    SceneWork_SetStepValue_1(SCENE_STEP_VALUE);
    Actor_unk128_4(21);
    base6_6014 = (s32)gVal2;
    Motion_SetVarCbObj_1(20, 2);
    Object_SetModeById_1(20, 4);
    Actor_unk129_4(base6_6014);
    Motion_ArmCb_2(21, 0xd000, 0);
    Motion_ArmCb_3(22, 0xd000, 0);
    BattleFx_SpawnLinked_1(21, 0x101, 0);
    BattleFx_SpawnLinked_2(22, 0x101, 60);
    Object_SetModeById_2(20, 3);
    Actor_unk130_4(base6_6014);
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
    Actor_unk16(20, 0xb000);
    Motion_SetVarCbObj_2(21, 1);
    Actor_unk13_2(0x5015);
    Motion_CallWaitAnim_1(20, 3);
    Actor_unk7_4(22, 0xd000);
    Actor_unk14_2(0x9016);
    Motion_ArmCb_6(20, 0x8000, 20);
    Object_SetModeById_3(20, 4);
    Actor_unk15_2(0xa014);
    Motion_SetPosReset_5(20, 204, 0x218);
    Motion_ArmCb_7(22, 0xb000, 0);
    Motion_SetPosReset_6(20, 182, 0x224);
    Motion_SetPosReset_7(20, 182, 0x250);
    Motion_ResetPosMode2_1(20, 182, 0x298);
    Battle_WaitMode0_5(40);
    Actor_unk131_3();
    Motion_SetSpeed_6();
    Actor_unk132_3(16);
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
    Actor_unk133_3(1);
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
    Actor_unk134_3(20, 0);
    Actor_unk17(0, 0x8000);
    Motion_SetVarCbObj_1(20, 1);
    SceneWork_SetStepValue_1(0x1ee1);
    ((void (*)())Actor_unk43)(20);
    Motion_CallWaitAnim_1(0, 3);
    Battle_WaitMode0_3(40);
    Motion_ArmCb_1(20, 0x5000, 20);
    BattleFx_SpawnLinked_1(20, 0x105, 60);
    Battle_RunThenWaitIfModeZero_1(20, 0, 40);
    Actor_unk135_3(20, 0);
    Actor_unk136_3(20);
    Motion_CallWaitAnim_2(0, 3);
    Motion_CallWaitAnim_3(20, 3);
    Motion_SetPosReset_2(20, 182, 0x258);
    Motion_SetPosReset_3(20, 216, 0x258);
    Actor_unk8_4(20, 0xc000);
    Actor_unk137_3();
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
    Actor_unk138_3(22, 0x5000);
    SceneWork_SetStepValue_1(0x1f69);
    Motion_SetSpeed_2(0x2016, 0);
    Motion_ArmCb_1(0, 0xe000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        Actor_unk16_2(0x2016);
        Battle_SchedShoulder_1();
    } else {
        bump_step(1);
        BattleEventRuntime_ProcessAction_1(0x2016, 0);
        Actor_unk139_3();
        Motion_SetHPosTerrain_1(26, 0xd80000, 0x24c0000);
        Motion_SetSpeed_3(26, 0x13333, 0x9999);
        Motion_SetPosReset_2(26, 216, 0x254);
        Motion_SetPosReset_3(26, 188, 0x268);
        Motion_ArmCb_2(0, 0xe000, 0);
        Motion_ArmCb_3(21, 0xd000, 0);
        Motion_ArmCb_4(22, 0xd000, 0);
        Actor_unk18(26, 0x5000);
        Motion_Launch_1(26, 2, 0);
        Object_SetModeById_1(26, 4);
        BattleEv_RunWait_1(26, 0);
        Motion_SetHPosTerrain_2(20, 0xb40000, 0x3090000);
        Motion_SetSpeed_4(20, 0x10000, 0x8000);
        Motion_SetPosReset_4(20, 180, 0x298);
        data_00002014_addr = (s32)gVal3;
        Motion_ArmCb_5(20, 0xd000, 0);
        Actor_unk140_3(data_00002014_addr);
        Motion_ArmCb_6(0, 0x2000, 0);
        Motion_ArmCb_7(22, 0x3000, 0);
        BattleFx_SpawnLinked_2(26, 0x101, 60);
        Motion_SetVarCbObj_2(20, 1);
        Actor_unk141_3(data_00002014_addr);
        Motion_SetVarCb_1(21, 2);
        Actor_unk142_3(21);
        Motion_ArmCb_8(20, 0x5000, 20);
        Object_SetModeById_2(20, 3);
        Actor_unk17_2(0x6014);
        Motion_Launch_2(26, 2, 20);
        Object_SetModeById_3(26, 4);
        Actor_unk143_3(26);
        Motion_SetPosReset_5(20, 182, 0x280);
        Motion_ArmCb_9(20, 0xd000, 0);
        Actor_unk18_2(0x8014);
        BattleFx_SpawnLinked_3(26, 0x100, 20);
        Motion_SetVarCb_2(26, 2);
        Actor_unk144_3(26);
        Motion_CallWaitAnim_1(20, 3);
        Actor_unk145_3(22, 0);
        Motion_SetVarCbObj_3(22, 1);
        Actor_unk146_3(22);
        Motion_SetSpeed_5(22, 0x19999, 0xcccc);
        data_0200c918_addr = (s32)gOv7;
        Motion_EnableActCb_1(22, data_0200c918_addr);
        Motion_SetSpeed_6(21, 0x19999, 0xcccc);
        Motion_SetPosReset_6(21, 168, 0x278);
        Motion_EnableActCb_2(21, data_0200c918_addr);
        Battle_WaitMode0_2(80);
        Motion_EnableActCb_3(26, data_0200c918_addr);
        Battle_WaitMode0_3(40);
        Actor_unk9_4(20, 0x8000);
        Actor_unk147_3(data_00002014_addr);
        Actor_unk33_3(0, 0xe000);
        Motion_CallWaitAnim_2(0, 3);
        Motion_CallWaitAnim_3(20, 3);
        SCENE_PHASE = 0x201;
        Actor_unk148_3();
        Motion_SetSpeed_7();
        Actor_unk149_3(17);
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
    Actor_unk19(3, 0x8000);
    Actor_unk150_3(22, 0);
    SceneWork_SetStepValue_1(0x1f55);
    Actor_unk19_2(22);
    Actor_unk20(21, 0xd000);
    Battle_RunThenWaitIfModeZero_1(21, 0, 40);
    BattleFx_SpawnLinked_1(22, 0x100, 20);
    Motion_SetVarCbObj_1(22, 1);
    Motion_SetSpeed_5(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        Motion_CallWaitAnim_1(2, 4);
        Actor_unk151_3(2);
        Actor_unk21(3, 0xa000);
        Object_SetModeById_3(3, 3);
        Actor_unk152_3(3);
        Actor_unk22(1, 0x6000);
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
    Actor_unk153_3(22);
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
    Actor_unk10_4(21, 0xd000);
    Actor_unk154_3(22);
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

    if (Actor_unk44((s32)&Value_0000093e))
        return gOv11;
    if (Actor_unk45((s32)&Value_00000927))
        return gOv12;
    v = Actor_unk46((s32)&Value_00000928);
    if (v != 0)
        return gOv13;
    if (Actor_unk47((s32)&Value_00000911)) {
        if (Actor_unk48((s32)&Value_00000925)) {
            gOv14[0x14E] = v;
            gOv14[0x1AE] = 2;
            gOv14[0x1C6] = 2;
        } else if (Actor_unk49((s32)&Value_00000922)) {
            gOv14[0x1AE] = 1;
            gOv14[0x1C6] = 1;
        }
        return gOv14;
    }
    return gOv15;
}

u8 *SceneData_SelectTableByThreeFlags(void)
{
    if (Actor_unk50((s32)&Value_0000093e))
        return gOv16;
    if (Actor_unk51(0x8A0))
        return gOv17;
    if (Actor_unk52((s32)&Value_00000928))
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
        if (Actor_unk53(v)!= 0) return id;
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
        if (Actor_unk54(base + i) != 0) return gOv20[i];
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
    Actor_unk155_2();
    Actor_unk32_2((s32)&Value_00001f00);
    Actor_Apply12(21, 0);
    Actor_unk156_2();
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

s32 *Actor_unk157_2();
s32 *Actor_unk158_2();

s32 *Actor_unk159_2(s32);
s32 *Actor_unk160_2(s32);
s32 *Actor_unk161_2(s32);

s32 *Actor_unk143_3(s32);
s32 *Actor_unk162_2(s32);
s32 *Actor_unk163_2(s32);
s32 *Actor_unk164_2(s32);
s32 *Actor_unk165_2(s32);
s32 *Actor_unk144_3(s32);
s32 *Actor_unk166_2(s32);
s32 *Actor_unk167_2();
s32 *Actor_unk168_2();
s32 *Actor_unk145_3(s32);

u8 *Actor_unk169_2(s32);
u8 *Actor_unk170_2(s32);
u8 *Actor_unk171_2(s32);
u8 *Actor_unk172_2(s32);
u8 *Actor_unk173_2(s32);
u8 *Actor_unk174_2(s32);
u8 *Actor_unk175_2(s32);
u8 *Actor_unk176_2(s32);

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#endif

s32 State_ResetWorkValuesAndStart(void)
{
    u8 **base = &gCam;
    u8 *p = base[0] + 0x104;

    Actor_unk33_2(0x11C);
    *(s32 *)(base[19] + 0x1C0) = 0x209;
    *(s32 *)(p + 0x1C) = 0;
    gOv21 = (u16)Actor_unk55();
    gOv22 = (u16)Actor_unk56();
    gOv4[0] = 0;
    gOv4[1] = 0;
    gOv5 = 0;
    Actor_unk177_2();
    Actor_unk34_2(1);
    Actor_unk178_2();
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
    Actor_unk42_3(12, 0, 0);
    Actor_unk43_3(13, 1, 0);
    Actor_unk44_3(14, 2, 0);
    Actor_unk45_3(15, 3, 0);
    Actor_unk46_3(16, 4, 1);
    Actor_unk47_3(17, 5, 1);
    Actor_unk48_3(18, 6, 1);
    Actor_unk49_3(19, 7, 1);
}

void State_InitActorSlots8To19(void)
{
    {
        u16 *p = gOv;
        u32 i;

        for (i = 0; i < 8; i++) p[i] = 0xC000;
    }
    Actor_unk35_2(8);
    ((void (*)())Actor_unk157_2)(9, 0, 0);
    Actor_unk50_3(10, 0, 0);
    Actor_unk51_3(11, 0, 0);
    Actor_unk52_3(12, 0, 0);
    Actor_unk36_2(13);
    Actor_unk37_2(14);
    Actor_unk38_2(15);
    {
        s32 *t = gOv3;
        s32 z = 0;
        t[0] = z;
        t[1] = z;
        t[2] = z;
        t[3] = z;
        Actor_Far[0] = Actor_unk159_2(8)[4];
        Actor_Far[1] = Actor_unk160_2(13)[4];
        Actor_Far[2] = Actor_unk158_2(14)[4];
        Actor_Far[3] = Actor_unk161_2(15)[4];
        Actor_unk39_2(16);
        Actor_unk40_2(17);
        Actor_unk41_2(18);
        Actor_unk42_2(19);
        {
            s32 v = 0xFFFF0000;

            Actor_unk143_3(16)[6] = v;
            Actor_unk162_2(17)[6] = v;
            Actor_unk163_2(18)[6] = v;
            Actor_unk164_2(19)[6] = v;
        }
        t[4] = z;
        t[5] = z;
        t[6] = z;
        t[7] = z;
        Actor_Far[4] = Actor_unk165_2(16)[4];
        Actor_Far[5] = Actor_unk144_3(17)[4];
        Actor_Far[6] = Actor_unk166_2(18)[4];
        Actor_Far[7] = Actor_unk167_2(19)[4];
    }
    {
        s32 *w = Actor_unk145_3(0);

        if (w != 0) Actor_unk53_3(8, w[2], w[4]);
    }
    Actor_unk43_2(1);
    Actor_Apply17(13, 8);
    Actor_Apply18(14, 8);
    Actor_Apply19(15, 8);
    Actor_Apply20(16, 8);
    Actor_Apply21(17, 8);
    Actor_Apply22(18, 8);
    Actor_Apply23(19, 8);
    {
        s32 o = 1;

        Actor_unk169_2(8)[92] = o;
        Actor_unk170_2(13)[92] = o;
        Actor_unk171_2(14)[92] = o;
        Actor_unk172_2(15)[92] = o;
        Actor_unk173_2(16)[92] = o;
        Actor_unk174_2(17)[92] = o;
        Actor_unk175_2(18)[92] = o;
        Actor_unk176_2(19)[92] = o;
    }
    Actor_unk168_2(1);
    Actor_unk54_3(8, 0x840000, 0x2780000);
    Actor_unk44_2(1);
    Actor_unk55_3(8, 0, 2);
    Actor_unk56_3(13, 1, 2);
    Actor_unk57_3(14, 2, 2);
    Actor_unk58_3(15, 3, 2);
    Actor_unk59_3(16, 4, 3);
    Actor_unk60_3(17, 5, 3);
    Actor_unk61_3(18, 6, 3);
    Actor_unk62_3(19, 7, 3);
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
            if (Actor_unk57(i)!= 0) p[i] = Actor_unk58(i);
            else p[i] = t;
        }
    }
    if (Actor_unk59(0) != 0) gOv3[0] = Actor_unk60(0);
    else gOv3[0] = 3;
    if (Actor_unk61(2) != 0) gOv3[1] = Actor_unk62(2);
    else gOv3[1] = 3;
    {
        s32 *q = gOv3;
        s32 t = 3;

        q[2] = t;
        q[3] = t;
        if (Actor_unk63(1) != 0) q[4] = Actor_unk64(1);
        else q[4] = t;
    }
    if (Actor_unk65(3) != 0) gOv3[5] = Actor_unk66(3);
    else gOv3[5] = 3;
    gOv3[6] = 3;
    gOv3[7] = 3;
}

/* overlays/scene/actor/multi_encounter/shared.c */

void Scene_RunStepThen10(s32 a)
{
    Actor_Apply24(a, 0);
    Actor_unk45_2(10);
}

void Scene_CallPairWith10(s32 a, s32 b)
{
    Actor_unk63_3(a, b, 10);
}

/* overlays/scene/actor/multi_encounter/state_update.c */

s32 State_ApplyArgMode1AndReturnZero(s32 a)
{
    Actor_Apply25(a, 1);
    return 0;
}

void State_ApplyFiveRectsAtColumn78(void)
{
    Actor_unk6_5(78, 39, 78, 40, 5, 1);
    Actor_unk7_5(78, 39, 78, 41, 5, 1);
    Actor_unk8_5(78, 39, 79, 42, 4, 1);
    Actor_unk9_5(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Actor_unk10_5(17, 38, 5, 2, x, y);
    }
}

/* overlays/scene/script/fx_entry_step.c */
extern u8 gWork[];

void Scene_RunScriptFxEntryStep(void)
{
    u32 i;
    s32 record;

    Script_unk3_4FxEntryStep();
    Script_Run(-1, -1, -1, 0);
    Script_unk4_4FxEntryStep(1);
    Script_unk5_3FxEntryStep(20, 0, 0);
    Script_unk6_3FxEntryStep(22, 0, 0);
    Script_unk7_3FxEntryStep(24, 0, 0);
    Script_unk8_2FxEntryStep(25, 0, 0);
    Script_unk9_2FxEntryStep(26, 0, 0);
    Script_unk10_2FxEntryStep(27, 0, 0);
    Script_unk11_2FxEntryStep(0, 0, 0);
    Script_unk12_2FxEntryStep(23, 0, 0);
    record = Script_CheckFxEntryStep(23);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_Place(21, 0xe80000, 0x28a0000);
    record = Script_unk2FxEntryStep(21);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk2_4(0xe80000, -1, 0x27c0000, 0);
    Script_unk13_2FxEntryStep();
    Script_unk14_2FxEntryStep(1);
    Script_unk15FxEntryStep(23, 21);
}

/* overlays/scene/script/fx_mid_step.c */
extern u8 gWork[];

void Scene_RunScriptFxMidStep(void)
{
    u32 i;
    s32 record;

    Script_Run();
    Script_Do(0x200d160);
    Script_unk2_4FxMidStep(1);
    Script_Place(0, 0xe80000, 0x27c0000);
    Script_unk3_4FxMidStep(0, 15);
    record = Script_CheckFxMidStep(0);
    Script_unk4_4FxMidStep(record, 0);
    Script_unk5_3FxMidStep(1);
    Script_unk6_3FxMidStep(0, 0);
    Script_unk7_3FxMidStep();
    Script_unk8_2FxMidStep(1);
    Script_unk9_2FxMidStep(22);
    Script_unk10_2FxMidStep(21);
    Script_unk11_2FxMidStep(1);
    Script_unk12_2FxMidStep(22, 0, 0);
    Script_unk13_2FxMidStep(21, 0, 0);
    Script_unk14_2FxMidStep(20, 0, 0);
    record = Script_unk2FxMidStep(20);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk2_3(23, 0xe80000, 0x28a0000);
    record = Script_unk3(23);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk15FxMidStep(1);
    Script_unk16(20, 23);
}

/* overlays/scene/script/fx_pair_step.c */
extern u8 gWork[];

void Scene_RunScriptFxPairStep(s32 a0, s32 a1)
{
    u32 i;
    s32 p10;
    s32 record;

    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x100;
    Script_unk2_4FxPairStep();
    Script_unk3_4FxPairStep();
    Script_unk4_4FxPairStep(20);
    Script_unk5_3FxPairStep();
    Script_Place(a0, 0xd80000, 0x24c0000);
    Script_unk2_3(a0, 0xcccc, 0x6666);
    Script_unk3_3(a0, 216, 0x258);
    Script_unk4_3(a0, 218, 0x25c);
    Script_unk5_2(a0, 234, 0x25c);
    Script_unk6_2(a0, 236, 0x26a);
    Script_unk7_2(a0, 0x5000, 20);
    Script_unk6_3FxPairStep(a0, 3);
    Script_unk7_3FxPairStep(20);
    Script_Run(a1, 0x5000);
    Script_unk8_2FxPairStep(a1, 4, 40);
    Script_unk9_2FxPairStep(a1, 2);
    Script_Do(0x1e39);
    Script_unk10_2FxPairStep(a1, 0, 20);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Script_unk11_2FxPairStep();
    Script_unk12_2FxPairStep();
    Script_unk13_2FxPairStep(10);
    p10 = a0;
}

/* overlays/scene/actor/multi_encounter/run_scene_actor_twenty_dialogue.c */
extern u8 gOvActorTwentyDialogue[];
extern s32 *gWork;

void Scene_RunActorTwentyDialogueSequence(void)
{
    Actor_RunActorTwentyDialogue();
    Actor_Do((s32)gOvActorTwentyDialogue);
    Actor_unk2_2(1);
    Actor_ApplyActorTwentyDialogue(0, 15);
    Actor_Apply2ActorTwentyDialogue(Actor_Check(0), 0);
    gWork[0x70] = 0x202;
    Actor_unk2_4ActorTwentyDialogue();
    Actor_unk3_4ActorTwentyDialogue();
    Actor_unk3_2(20);
    Actor_Apply3ActorTwentyDialogue(20, 1);
    Actor_unk4_2(0x1e41);
    Actor_Place(20, 0, 10);
    Actor_Apply4ActorTwentyDialogue(22, 0x5000);
    Actor_unk2_3(22, 4, 20);
    Actor_Apply5ActorTwentyDialogue(22, 2);
    Actor_unk3_3(0x6016, 0, 20);
    Actor_unk4_4ActorTwentyDialogue();
    Actor_unk5_4ActorTwentyDialogue();
    Actor_unk5_2(11);
}

/* overlays/scene/actor/multi_encounter/run_scene_configure_three_actors.c */
extern u8 gWork[];

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void Scene_ConfigureThreeActors(void)
{
    u32 i;
    s32 record;

    Battle_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Actor_RunConfigureThreeActors(record, 0);
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Actor_unk2_4ConfigureThreeActors(1);
    Motion_SetHPosTerrain_1(20, 0xc40000, 0x1f60000);
    record = Scene_GetRecord_2(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetHPosTerrain_2(22, 0xb80000, 0x20c0000);
    record = Scene_GetRecord_3(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetActionVariant_1(21, 1);
    Motion_SetHPosTerrain_3(21, 0xb80000, 0x2780000);
    record = Scene_GetRecord_4(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Battle_WaitMode0_2(20);
    Motion_Launch_1(22, 4, 10);
    Motion_Launch_2(22, 6, 20);
    SceneWork_SetStepValue_1(0x1ee5);
    Actor_unk3_4ConfigureThreeActors(22);
    Motion_CallWaitAnim_1(20, 3);
    Motion_SetSpeed_2(21, 0x30000, 0x18000);
    Motion_SetPosReset_1(21, 180, 0x222);
    Motion_ArmCb_1(21, 0xb000, 40);
    Motion_SetVarCbObj_1(21, 1);
    Actor_unk4_4ConfigureThreeActors(21);
    Actor_unk5_4ConfigureThreeActors(15);
}

/* overlays/scene/script/fx_tail_step.c */
extern u8 gWork[];

void Scene_RunScriptFxTailStep(void)
{
    u32 i;
    s32 record;

    Script_Run(0xe80000, -1, 0x2a40000, 0);
    Script_unk2_4FxTailStep();
    Script_Place(0, 0xe80000, 0x2a40000);
    record = Script_CheckFxTailStep(0);
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Script_unk3_4FxTailStep(1);
}
