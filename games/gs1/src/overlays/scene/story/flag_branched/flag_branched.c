#include "types.h"
#include "scene.h"
#include "facing_object.h"

/* overlays/scene/story/flag_branched/conditional_scene_setup.c */

void Scene_RunStepWithValue174d(void)
{
    Story_unk21_4();
    Story_Apply(0x174D, 1);
    Story_unk22_4();
}

void Scene_RunStepWithValue174e(void)
{
    Story_unk23_4();
    Story_Apply2(0x174E, 1);
    Story_unk24_4();
}

void Scene_RunBranchedStep(void)
{
    if (Story_unk36() != 0) {
        Story_unk25_4();
    } else {
        Story_unk26_4();
    }
}

void Scene_RunStep210ByFlag84e(void)
{
    if (Story_unk37(0x84E) != 0) {
        Story_unk15_2(0x210);
    } else {
        Story_unk154_2(21, 182, 0x210);
    }
}

void Scene_RunStep211ByFlag84e(void)
{
    if (Story_unk38(0x84E) != 0) {
        Story_unk16_2(0x211);
    } else {
        Story_unk155_2(22, 183, 0x211);
    }
}

void Scene_RunStep212ByFlag84e(void)
{
    if (Story_unk39(0x84E) != 0) {
        Story_unk17_2(0x212);
    } else {
        Story_unk156_2(23, 186, 0x212);
    }
}

void Scene_RunStep213ByFlag84e(void)
{
    if (Story_unk40(0x84E) != 0) {
        Story_unk18_2(0x213);
    } else {
        Story_unk157_2(24, 189, 0x213);
    }
}

/* overlays/scene/story/flag_branched/overlay_object.c */

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/* overlays/scene/story/flag_branched/run_scene_long_branching_choreography.c */
extern u8 gVal[];
extern u8 gOv[];
extern u8 gWork[];
typedef void SceneEndFunc(void);
extern volatile SceneEndFunc Story_unk27_4;

void Scene_RunLongBranchingChoreography(void)
{
    s32 record;
    u8 *work;
    s32 base5_200a5c0;

    Story_unk28_4();
    Story_Run(-1, -1, -1, 0);
    Story_unk29_4(1);
    *(u8 *)(Story_unk41() + 85) = 0;
    Story_Place(0x37e0000, -1, 0x2980000);
    Story_unk30_4(1);
    Story_unk31_4(0, 0, 0);
    if (Story_Check(0x85f) != 0) {
        Story_unk2_4(0x37e0000, -1, 0x2ba0000, 0);
        Story_unk2_3(19, 0x36c0000, 0x27a0000);
        Story_unk3_3(19, 0x3000, 0);
        Story_unk4_3(0, 0x37e0000, 0x31e0000);
    }
    Story_unk32_4();
    Story_unk33_4(1);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 40;
    Story_unk34_4();
    Story_unk35_4();
    if (Story_unk2(0x85f) != 0) {
    } else {
        Story_unk36_4(80);
        Story_unk5_3(19, 0x37e0000, 0x31e0000);
        Story_unk3_4(0x9999, 0x1333);
        Story_unk4_4(0x37e0000, -1, 0x2ba0000, 1);
        Story_unk6_3(19, 0xcccc, 0x6666);
        Story_unk7_3(19, 0x37e, 0x2b8);
        Story_unk37_4(80);
        Story_unk5_4(0x37e0000, -1, 0x2980000, 1);
        Story_unk38_4(19);
        Story_unk8_3(19, 0x34a, 0x2b8);
        Story_unk9_3(19, 0x34a, 0x27c);
        Story_unk10_3(18, 0x7000, 20);
        Story_unk11_3(19, 0x36c, 0x27a);
        Story_unk39_4(19, 3);
        Story_unk40_4(20);
        Story_unk41_4(18, 3);
        Story_unk42_4(10);
        Story_Do(0x1437);
        Story_unk12_3(0x2012, 0, 10);
        Story_unk43_4(19, 2);
        Story_unk44_4(19, 0, 20);
        Story_unk45_4(18, 1);
        Story_unk13_3(0x2012, 0, 10);
        Story_unk46_4(19, 3);
        Story_unk47_4(40);
        Story_unk14_3(18, 0x105, 60);
        Story_unk6_4(0x2012, 0);
        Story_unk48_4(18, 1);
        Story_unk15_3(0x2012, 0, 10);
        Story_unk7_4(19, 0x102);
        Story_unk49_4(60);
        Story_unk16_3(19, 0x3000, 10);
        Story_unk17_3(18, 0x5000, 10);
        Story_unk8_4(0x37e0000, -1, 0x2ba0000, 1);
        Story_unk18_3(0, 0x37e0000, 0x31e0000);
        Story_unk19_3(0, 0x9999, 0x4ccc);
        Story_unk20_3(0, 0x37e, 0x2d6);
        Story_unk50_4(20);
        Story_unk51_4(0, 3);
        Story_unk52_3(10);
        Story_unk53_3(18, 1);
        Story_unk9_4(0x2012, 0);
        Story_unk10_4(0x37e0000, -1, 0x2980000, 1);
        Story_unk21_3(0, 0x37e, 0x2ac);
        record = Story_unk3(0);
        if (record != 0) {
            Story_unk54_3(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        record = Story_unk4(0);
        if (record != 0) {
            Story_unk55_3(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        if (Story_unk5(3) != 0) {
            record = Story_unk6(0);
            if (record != 0) {
                Story_unk56_3(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
        }
        Story_unk22_3(1, 0x9999, 0x4ccc);
        Story_unk23_3(2, 0x9999, 0x4ccc);
        Story_unk24_3(3, 0x10000, 0x8000);
        Story_unk57_3(1, 2);
        Story_unk58_3(2, 2);
        Story_unk59_3(3, 2);
        Story_unk25_3(1, -16, 16);
        Story_unk60_3(2, 16, 16);
        if (Story_unk7(3) != 0) {
            Story_unk61_3(3, 32, 16);
        }
        Story_unk62_3(2);
        Story_unk63_3(1, 1);
        Story_unk64_3(2, 1);
        Story_unk65_3(3, 1);
        Story_unk66_3(10);
        Story_unk26_3(3, 0xc000, 0);
        Story_unk27_3(1, 0xc000, 0);
        Story_unk28_3(2, 0xc000, 20);
        Story_unk67_3(18, 2, 20);
        Story_unk29_3(18, 0x7000, 10);
        Story_unk30_3(0x2012, 0, 10);
        Story_unk31_3(19, 0x1000, 10);
        ((void (*)())Story_unk42)(19, 3);
        Story_unk32_3(18, 0x5000, 40);
        Story_unk33_3(18, 0x7000, 10);
        Story_unk68_3(18, 4);
        Story_unk34_3(0x2012, 0, 10);
        Story_unk11_4(19, 0x102);
        Story_unk69_3(40);
        Story_unk35_3(18, 0x5000, 20);
        Story_unk36_3(18, 0x105, 40);
        Story_unk8(0x2012, 0);
        Story_unk37_3(1, 0xe000, 0);
        Story_unk38_3(2, 0xa000, 0);
        if (Story_unk9(0, 0) == 0) {
            goto L_02000f86;
        }
        L_02000cb6:;
        Story_unk70_3((s32)gVal);
        Story_unk39_3(0x2012, 0, 10);
        Story_unk40_3(19, 0x3000, 0);
        Story_unk71_3(18, 4);
        Story_unk41_3(0x2012, 0, 10);
        Story_unk42_3(1, 0xc000, 0);
        Story_unk43_3(2, 0xc000, 0);
        Story_unk72_3(0, 3);
        Story_unk73_3(1, 2);
        record = Story_unk10(0);
        if (record != 0) {
            Story_unk74_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Story_unk75_3(2, 2);
        record = Story_unk11(0);
        if (record != 0) {
            Story_unk76_3(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Story_unk12(3) != 0) {
            Story_unk77_3(3, 2);
            record = Story_unk13(0);
            if (record != 0) {
                Story_unk78_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Story_unk79_3(2);
        Story_unk80_3(1, 0, 0);
        Story_unk81_3(2, 0, 0);
        Story_unk82_3(3, 0, 0);
        Story_unk2_2(0x85f);
        Story_unk44_3(0, 0x10000, 0x8000);
        Story_unk45_3(0, 0x37e, 0x2f0);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
        Story_unk83_3();
        Story_unk84_3();
        Story_unk27_4();
    }
    Story_unk46_3(0, 0x9999, 0x4ccc);
    Story_unk47_3(0, 0x37e, 0x2ac);
    Story_unk85_3(80);
    Story_unk12_4(0x9999, 0x1333);
    Story_unk13_4(0x37e0000, -1, 0x2980000, 1);
    Story_unk86_3(0);
    Story_unk87_3(0, 1);
    record = Story_unk14(0);
    if (record != 0) {
        Story_unk88_3(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Story_unk15(0);
    if (record != 0) {
        Story_unk89_3(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    if (Story_unk16(3) != 0) {
        record = Story_unk17(0);
        if (record != 0) {
            Story_unk90_3(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
    }
    Story_unk48_3(1, 0x9999, 0x4ccc);
    Story_unk49_3(2, 0x9999, 0x4ccc);
    Story_unk50_3(3, 0x10000, 0x8000);
    Story_unk91_3(1, 2);
    Story_unk92_3(2, 2);
    Story_unk93_3(3, 2);
    Story_unk51_3(1, -16, 16);
    Story_unk94_3(2, 16, 16);
    if (Story_unk18(3) != 0) {
        Story_unk95_3(3, 32, 16);
    }
    Story_unk96_3(2);
    Story_unk97_3(1, 1);
    Story_unk98_3(2, 1);
    Story_unk99_3(3, 1);
    Story_unk100_3(10);
    Story_unk52_2(3, 0xc000, 0);
    Story_unk53_2(1, 0xc000, 0);
    Story_unk54_2(2, 0xc000, 20);
    Story_unk55_2(18, 0x101, 60);
    Story_unk3_2(0x1442);
    Story_unk19(0x2012, 0);
    if (Story_unk20(0, 0) == 1) {
        goto L_02000cb6;
    }
    L_02000f86:;
    Story_unk56_2(3, 0xc000, 0);
    Story_unk57_2(0, 0xc000, 0);
    Story_unk58_2(1, 0xc000, 0);
    Story_unk59_2(2, 0xc000, 20);
    Story_unk101_3(3, 3);
    Story_unk102_3(0, 3);
    Story_unk103_3(1, 3);
    Story_unk104_3(2, 3);
    Story_unk60_2(18, 0x105, 60);
    Story_unk4_2(0x1443);
    Story_unk14_4(0x2012, 0);
    record = Story_unk43(20);
    Story_unk105_3(record, 0);
    record = Story_unk44(20);
    *(volatile s32 *)(record + 24) = 0x8000;
    *(volatile s32 *)(record + 28) = 0x8000;
    record = Story_unk21(18);
    if (record != 0) {
        Story_unk106_3(20, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Story_unk107_3(1);
    Story_unk108_3(20, 6, 0);
    Story_unk61_2(20, 0x20000, 0x10000);
    Story_unk62_2(20, 0x37e, 0x29c);
    Story_unk109_3(40);
    Story_unk63_2(0x2012, 0, 10);
    Story_unk64_2(3, 0x101, 0);
    Story_unk65_2(0, 0x101, 0);
    Story_unk66_2(1, 0x101, 0);
    Story_unk67_2(2, 0x101, 60);
    Story_unk110_3(18, 4);
    Story_unk68_2(0x2012, 0, 10);
    Story_unk69_2(1, 0x103, 60);
    Story_unk70_2(1, 0xe000, 10);
    Story_unk22(0x4001, 0);
    Story_unk71_2(3, 0xa000, 0);
    Story_unk72_2(2, 0xa000, 0);
    Story_unk73_2(0, 0x6000, 0);
    if (Story_unk23(0, 0) == 1) {
        do {
            Story_unk111_3(1, 2);
            Story_unk112_3(2, 2);
            Story_unk5_2(0x1447);
            Story_unk24(0x4001, 0);
        } while (Story_unk25(0, 0) != 1);
    }
    Story_unk113_3(1, 3);
    Story_unk6_2(0x1448);
    Story_unk74_2(0x4001, 0, 10);
    Story_unk75_2(3, 0xc000, 0);
    Story_unk76_2(0, 0xc000, 0);
    Story_unk77_2(1, 0xc000, 0);
    Story_unk78_2(2, 0xc000, 10);
    Story_unk114_3(0, 3);
    Story_unk115_3(0, 3);
    Story_unk116_3(20);
    Story_unk79_2(18, 0x105, 60);
    Story_unk80_2(0x2012, 0, 20);
    Story_unk81_2(2, 0x102, 60);
    Story_unk82_2(18, 0x3000, 10);
    Story_unk83_2(18, 0x101, 60);
    Story_unk84_2(1, 0x101, 40);
    Story_unk117_3(1, 0, 20);
    Story_unk118_3(1, 1);
    Story_unk85_2(0x4001, 0, 10);
    Story_unk86_2(3, 0x8000, 0);
    Story_unk87_2(0, 0x2000, 20);
    Story_unk88_2(2, 0x102, 60);
    Story_unk89_2(2, 0x8000, 10);
    Story_unk90_2(0x4002, 0, 10);
    Story_unk119_3(1, 1);
    Story_unk120_3(40);
    Story_unk121_3(1, 3);
    Story_unk91_2(0x4001, 0, 10);
    Story_unk92_2(1, 0xe000, 10);
    Story_unk122_3(1, 1);
    Story_unk26(0x4001, 0);
    Story_unk93_2(3, 0xa000, 0);
    Story_unk94_2(2, 0xa000, 0);
    Story_unk95_2(0, 0x6000, 0);
    while (Story_unk27(0, 0) != 0) {
        Story_unk7_2(0x144e);
        Story_unk15_4(0x4001, 0);
    }
    Story_unk123_3(10);
    Story_unk96_2(3, 0x8000, 0);
    Story_unk124_3(1, 0, 10);
    Story_unk97_2(0, 0x2000, 10);
    Story_unk125_3(1, 3);
    Story_unk98_2(2, 0x105, 60);
    Story_unk99_2(2, 0x8000, 10);
    Story_unk126_3(2, 4);
    Story_unk8_2(0x144f);
    Story_unk100_2(0x4002, 0, 20);
    Story_unk127_3(18, 1);
    Story_unk101_2(18, 0x5000, 10);
    Story_unk128_3(18, 4);
    Story_unk102_2(0x2012, 0, 10);
    Story_unk103_2(3, 0xc000, 0);
    Story_unk104_2(0, 0xc000, 0);
    Story_unk105_2(1, 0xc000, 0);
    Story_unk106_2(2, 0xc000, 20);
    Story_unk129_3(3, 3);
    Story_unk130_3(0, 3);
    Story_unk131_3(1, 3);
    Story_unk132_3(2, 3);
    Story_unk133_3(40);
    Story_unk107_2(18, 0x105, 80);
    Story_unk108_2(19, 0x1000, 10);
    Story_unk16_4(19, 0x102);
    Story_unk134_3(40);
    Story_unk135_3(19, 0, 10);
    Story_unk136_3(18, 1);
    Story_unk137_3(40);
    Story_unk109_2(18, 0x7000, 20);
    Story_unk110_2(0x2012, 0, 10);
    Story_unk138_3(19, 3);
    Story_unk139_3(20);
    Story_unk140_3(18, 4);
    Story_unk141_3(18, 4);
    Story_unk111_2(0x2012, 0, 10);
    Story_unk142_3(20, 6, 0);
    record = Story_unk28(18);
    if (record != 0) {
        Story_unk143_3(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk144_3(20);
    Story_unk145_3(20, 0, 0);
    Story_unk146_3(20);
    Story_unk17_4(3, 0x102);
    Story_unk18_4(0, 0x102);
    Story_unk19_4(1, 0x102);
    Story_unk20_4(2, 0x102);
    Story_unk147_3(40);
    Story_unk148_3(19, 2);
    Story_unk149_3(19, 0, 10);
    Story_unk150_3(18, 3);
    Story_unk112_2(0x2012, 0, 20);
    Story_unk151_3(19, 3);
    Story_unk152_3(2, 2);
    Story_unk153_3(20);
    Story_unk113_2(0x4002, 0, 10);
    Story_unk114_2(19, 0x3000, 0);
    Story_unk115_2(18, 0x3000, 10);
    Story_unk154_3(18, 4);
    Story_unk116_2(0x2012, 0, 10);
    Story_unk117_2(1, 0x103, 60);
    Story_unk118_2(0x4001, 0, 10);
    Story_unk119_2(18, 0x5000, 10);
    Story_unk155_3(18, 4);
    Story_unk120_2(0x2012, 0, 10);
    Story_unk156_3(2, 4);
    Story_unk121_2(0x4002, 0, 10);
    Story_unk122_2(18, 0x3000, 10);
    Story_unk157_3(18, 3);
    Story_unk123_2(0x2012, 0, 10);
    Story_unk124_2(3, 0x107, 0);
    Story_unk125_2(0, 0x107, 0);
    Story_unk126_2(1, 0x107, 0);
    Story_unk127_2(2, 0x107, 60);
    Story_unk128_2(18, 0x7000, 10);
    Story_unk158_3(18, 3);
    Story_unk159_3(10);
    Story_unk129_2(0x2012, 0, 10);
    Story_unk160_3(19, 2);
    Story_unk130_2(19, 0x1000, 10);
    Story_unk161_3(19, 3);
    Story_unk162_3(20);
    base5_200a5c0 = (s32)gOv;
    Story_unk131_2(0, 0x10013, base5_200a5c0);
    Story_unk132_2(1, 0x10013, base5_200a5c0);
    Story_unk133_2(2, 0x10013, base5_200a5c0);
    Story_unk134_2(3, 0x10013, base5_200a5c0);
    Story_unk135_2(19, 0x9999, 0x4ccc);
    Story_unk136_2(19, 0x354, 0x286);
    Story_unk137_2(19, 0x354, 0x29a);
    Story_unk138_2(19, 0x360, 0x2a0);
    Story_unk139_2(19, 0x1000, 10);
    Story_unk140_2(0x4013, 0, 20);
    Story_unk163_3(0);
    Story_unk164_3(1);
    Story_unk165_3(2);
    Story_unk141_2(0, 0x105, 0);
    Story_unk142_2(1, 0x105, 0);
    Story_unk143_2(2, 0x105, 60);
    Story_unk166_3(0, 3);
    Story_unk167_3(1, 3);
    Story_unk168_3(2, 3);
    Story_unk144_2(19, 0x10000, base5_200a5c0);
    Story_unk169_3(1, 2);
    record = Story_unk29(0);
    if (record != 0) {
        Story_unk170_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk171_3(2, 2);
    record = Story_unk30(0);
    if (record != 0) {
        Story_unk172_3(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Story_unk31(3) != 0) {
        Story_unk173_3(3, 2);
        record = Story_unk32(0);
        if (record != 0) {
            Story_unk174_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Story_unk175_3(2);
    Story_unk176_3(1, 0, 0);
    Story_unk177_3(2, 0, 0);
    Story_unk178_3(3, 0, 0);
    Story_unk145_2(0, 0x10000, 0x8000);
    Story_unk146_2(0, 0x37e, 0x2f0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    ((s64 (*)())Story_unk179_3)();
    Story_unk180_3();
    Story_unk9_2(0x321);
    Story_unk181_3(29);
    Story_unk182_3();
}

/* overlays/scene/story/flag_branched/scene_channel.c */

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Story_unk158_2(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Story_unk159_2(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Story_unk160_2(channel, value, zero);
}

static __inline__ void ConfigureFirst2(s32 channel, s32 value, s32 zero)
{
    Story_unk161_2(channel, value, zero);
}

static __inline__ void ConfigureSecond2(s32 channel, s32 value, s32 zero)
{
    Story_unk162_2(channel, value, zero);
}

static __inline__ void ConfigureThird2(s32 channel, s32 value, s32 zero)
{
    Story_unk163_2(channel, value, zero);
}

void ConfigurePrimarySceneChannels(void)
{
    ConfigureFirst(1, 0xe000, 0);
    ConfigureSecond(2, 0xa000, 0);
    ConfigureThird(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    ConfigureFirst2(1, 0xc000, 0);
    ConfigureSecond2(2, 0xc000, 0);
    ConfigureThird2(3, 0xa000, 0);
}

/* overlays/scene/story/flag_branched/scene_data.c */
struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

/* Scene selector and table getters for resource_38d. The two records live in
 * the overlay image, which is writable, so the stores below land in it. */
extern s16 gCell[];     /* Shared scene workspace, not overlay data. */
extern u8 Value_00000021;
extern u8 gOv2[];      /* In-image record. */
extern u8 gOv3[];      /* In-image record. */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Story_unk183_3();           /* Record hand-off. */

s32 Story_unk184_3();            /* Story-flag test. */

/*
 * Returns the in-image table at 0x0200a638. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA638(void)
{
    return (u8 *)0x0200a638;
}

/*
 * Returns the in-image table at 0x0200a920. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA920(void)
{
    return (u8 *)0x0200a920;
}

/*
 * Picks one of two scene records by the current scene selector. For scene
 * 0x21 the record is handed to Story_unk183_3 and, when story flag 0x84e is
 * set, four of its bytes are stamped; otherwise the other record is returned
 * untouched. The four offsets are one field at a 24-byte stride over four
 * entries, left literal because the entry layout is not established.
 */
s32 SceneData_SelectRecordByScene21(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (gCell[224] == (s32)&Value_00000021) {
        p = gOv2;
        Story_unk183_3(p);

        if (Story_unk184_3(0x84e) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (s32)p;
    }
    return (s32)gOv3;
}

/* overlays/scene/story/flag_branched/scene_primary_script_head.c */

extern u8 gWork[];

void Story_unk185_3();           /* scripted-scene bracket: open */

s32 Story_unk186_3();            /* story-flag test; used in a condition */

void Story_unk187_3();           /* show a dialogue line by id */

void Story_unk188_3();           /* wait for the slot's action to finish */

void Story_unk189_3();           /* scripted-scene bracket: close */

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    Story_unk190_3();
    Story_unk147_2(14, 0x102, 0);
    Story_unk191_2(14, 2);
    Story_unk192_2(40);
    Story_unk10_2(0x1764);
    Story_unk193_2(14, 0, 20);
    Story_unk194_2(14, 0, 0);
    Story_unk195_2(20);
    Story_unk196_2(14, 0, 10);
    Story_unk148_2(14, 0xb000, 10);
    Story_unk197_2();
}

void Scene_RunCueSequenceA(void)
{
    u32 i;
    s32 record;

    Story_unk198_2();
    Story_unk11_2(0x1750);
    if (Story_unk33(0x302) != 0) {
        Story_unk12_2(0x1768);
    }
    Story_unk199_2(15, 0);
    Story_unk13_2(0x302);
    Story_unk200_2();
}

void Scene_RunActorSeventeenFlaggedDialogue(void)
{
    u8 *work;

    Story_unk185_3();

    if (Story_unk186_3(0x202) != 0) {
        Story_unk187_3(0x174b);
    } else if (Story_unk45(0x84e) != 0) {
        Story_unk201_2(0x176e);
    } else {
        Story_unk202_2(0x1432);
        if (Story_unk46(0x84d) != 0) {
            work = *(u8 **)0x03001ebc;
            *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        }
    }

    Story_unk188_3(17, 0);
    Story_unk189_3();
}

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    if (Story_unk34(0x84e) == 0) {
        if (Story_unk35(0x322) != 0) {
            Story_unk203_2();
            Story_unk149_2(19, 0x100, 0);
            Story_unk150_2(19, 0x7000, 10);
            Story_unk204_2(19, 2);
            Story_unk205_2(20);
            Story_unk14_2(0x1748);
            Story_unk206_2(19, 0);
            Story_unk151_2(0, 0x10000, 0x8000);
            Story_unk152_2(0, 0x268, 0x2fa);
            Story_unk153_2(19, 0xd000, 10);
            Story_unk207_2();
        }
    }
}

/* overlays/scene/story/flag_branched/scene_setup.c */
extern s16 gCell[];
extern u8 Value_00000021;
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 Value_00001420;

/* 0x02002996 serves two imports: the two-argument reader that guards the
 * branch and the one-argument flag clear inside its first arm. */

s32 SceneData_SelectTableac9cByState(void)
{
    if (gCell[224] == (s32)&Value_00000021) {
        return (s32)gOv4;
    }
    return (s32)gOv5;
}

void Dialogue_RunActor10Message1420(void)
{
    Story_unk208_2();
    Story_unk19_2((s32)&Value_00001420);
    Story_Apply3(10, 0);
    Story_unk209_2();
}

void Scene_RunSlotSubjectBranch(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Story_unk210_2();

    record = Story_unk164_2(0, slot, subject);

    if (Story_Apply4(subject, 0) != -1) {
        Story_Apply5(slot, 2);
        Story_unk20_2(0x84E);
        Story_unk21_2(flag);
        Story_unk22_2(0x322);
        Story_unk23_2(0x202);
    } else {
        Story_unk24_2(125);
        Story_Apply6(slot, 5);
    }

    Story_unk25_2(record);
    Story_unk211_2();
}

/* overlays/scene/story/flag_branched/scene_state_interaction.c */

void Story_unk212_2();           /* scripted-scene bracket: open */

s32 Story_unk213_2();            /* story-flag test; used in a condition */

void Story_unk214_2();           /* show a dialogue line by id */

void Story_unk215_2();           /* wait for the slot's action to finish */

void Story_unk216_2();           /* scripted-scene bracket: close */

void Scene_RunActorSeventeenFlagDialogue(void)
{
    Story_unk212_2();

    if (Story_unk213_2(0x202) != 0) {
        Story_unk214_2(0x174c);
    } else if (Story_unk47(0x845) == 0) {
        Story_unk217_2(0x1436);
    } else {
        Story_unk218_2(0x1434);
        if (Story_unk48(0x84e) != 0) {
            Story_unk219_2(0x176f);
        }
    }

    Story_unk215_2(17, 0);
    Story_unk216_2();
}
