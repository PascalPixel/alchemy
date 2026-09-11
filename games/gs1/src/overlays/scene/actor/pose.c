#include "types.h"
#include "scene.h"

extern s16 gCell[];
extern u8 Value_000000b3;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 Value_000000b4;
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 *gWork;
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 Value_00002880;

u8 *Actor_unk107_4(s32);

u8 *Actor_unk108_4(s32);

u8 *Actor_unk109_4(s32);

/*
 * The import's main-image target is Actor_unk110_4; its second argument is a
 * pose id.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x02009438.
 */

/*
 * The eight-byte owner at 0x0200007c includes its one pool word, which holds
 * the returned table address 0x020096d0.
 */

/*
 * Select the scene script at 0x020004b0.  The owner runs to 0x020004e6 and
 * also owns the alignment halfword at 0x020004ea and the literal pool at
 * 0x020004ec-0x02000507.  The 0x9a7 predicate is evaluated on both sides of
 * the scene-id test, so the two calls stay separate -- they are two sites on
 * disjoint paths.  Index 224 of the scene-state block at 0x02000240 is the
 * signed scene id.
 */

/* Four scripts embedded in this overlay's own data. */

/*
 * Import veneer; used in a condition, so the return type is stated and the
 * arity left open.
 */

/*
 * The calls appear in machine order; that order is what reproduces the
 * reference.
 */

/*
 * Loader-relocated ROM calls: each site names the pre-relocation call word the
 * image holds, not a runtime address.
 */

/*
 * Import veneers, declared old-style: one import name takes different argument
 * counts at different sites in this overlay, so no arity is asserted and none
 * of these results is used.
 */
static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    SetActorPose(actor, pose);
}

s32 Actor_SetActor14Pose258(void)
{
    ConfigureActorPose(14, 258);
    return 0;
}

u8 *SceneData_GetTable9438(void)
{
    return (u8 *)0x02009438;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_000000b3) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

u8 *SceneData_GetTable96d0(void)
{
    return (u8 *)0x020096d0;
}

u8 *SceneData_SelectTableBySceneIdAndFlag9a7(void)
{
    if (gCell[224] == (s32)&Value_000000b4) {
        if (Actor_unk128(0x9A7) != 0) {
            return gOv4;
        }
        return gOv5;
    }
    return gOv3;
}

void Scene_RunSequenceA(void)
{
    s32 Actor_unk111_4(s32, s32);

    Actor_unk7_2(0x9BC);
    Actor_unk112_4();
    Actor_unk113_4();
    Actor_unk8_2(0xA);
    Actor_SetMode(0x780000, -1, 0x600000, 1);
    Actor_unk114_4();
    Actor_unk9_2(0x1E);
    Actor_unk10_2(0x288B);
    Actor_Apply2(0xC, 0);
    Actor_unk11_2(0xA);
    Actor_unk67_3(0, 0xC, 0);
    Actor_unk12_2(0x1E);
    Actor_unk111_4(0, 3);
    Actor_unk13_2(0x1E);
    Actor_unk115_4();
}

void Scene_RunThreeCallSequence(void)
{
    void Actor_unk116_4(s32, s32);

    Actor_unk14_2(0x9BC);
    Actor_unk15_2(0x288B);
    Actor_unk116_4(0xC, 0);
}

void State_ForwardWord16cAndApply7b(void)
{
    u8 *work = gWork;
    s16 *p = (s16 *)(work + 0x16C);

    Actor_unk16_2(*p);
    Actor_unk17_2(0x7B);
}

u8 *SceneData_SelectScriptBySceneIdAndFlag9a7(void)
{
    s32 Actor_unk117_4();

    if (gCell[224] == (s32)&Value_000000b4) {
        if (Actor_unk117_4(0x9a7) != 0) {
            return gOv6;
        }
        return gOv7;
    }
    if (Actor_unk129(0x9a7) != 0) {
        return gOv8;
    }
    return gOv9;
}

void Scene_RunPrimaryScript(void)
{
    void Actor_unk118_4();

    Actor_Run();
    Actor_Check(1);
    Actor_Place(0, 6291456, 12058624);
    Actor_unk2_3(0, 49152, 0);
    Actor_unk3_3(11, 0, 0);
    Actor_unk4_3(12, 32768, 0);
    Actor_Do(10320);
    Actor_unk2_4();
    Actor_unk3_4();
    Actor_unk2(10);
    Actor_unk5_3(0, 65536, 32768);
    Actor_unk6_3(0, 0, -16);
    Actor_unk7_3(0, 104, 136);
    Actor_unk3(10);
    Actor_unk4_4(1, -16, 16, 49152);
    Actor_unk4(3, 0, 24, 49152);
    Actor_unk5_4(2, 16, 16, 49152);
    Actor_unk5(1);
    Actor_unk6(20);
    Actor_unk7(10);
    Actor_unk6_4(2, 2);
    Actor_unk8(20);
    Actor_unk7_4(2, 0);
    Actor_unk9(10);
    Actor_unk8_3(11, 16384, 0);
    Actor_unk9_3(12, 16384, 0);
    Actor_unk10(30);
    Actor_unk10_3(11, 261, 70);
    Actor_unk8_4(11, 0);
    Actor_unk11(10);
    Actor_unk9_4(11, 4);
    Actor_unk12(20);
    Actor_unk10_4(11, 0);
    Actor_unk13(20);
    Actor_unk11_4(11, 2);
    Actor_unk14(20);
    Actor_unk12_4(11, 0);
    Actor_unk15(20);
    Actor_unk11_3(12, 258, 40);
    Actor_unk13_4(12, 0);
    Actor_unk16(10);
    Actor_unk14_4(12, 4);
    Actor_unk17(20);
    Actor_unk15_4(12, 0);
    Actor_unk18(20);
    Actor_unk2_2(10);
    Actor_unk12_3(11, 261, 50);
    Actor_unk16_4(11, 0);
    if (Actor_unk19(2495) == 0) {
        Actor_unk17_4();
    } else {
    Actor_unk3_2(20);
    Actor_unk18_4(11, 2);
    Actor_unk20(20);
    Actor_unk19_4(11, 0);
    Actor_unk21(10);
    Actor_unk13_3(1, 0, 50);
    Actor_unk14_3(0, 261, 60);
    Actor_unk22(10);
    Actor_unk15_3(1, 49152, 0);
    Actor_unk23(20);
    Actor_unk20_4(1, 0);
    Actor_unk24(10);
    Actor_unk21_4(3, 4);
    Actor_unk25(20);
    Actor_unk22_4(3, 0);
    Actor_unk26(10);
    Actor_unk23_4(2, 2);
    Actor_unk27(20);
    Actor_unk24_4(2, 0);
    Actor_unk28(20);
    Actor_unk16_3(12, 257, 40);
    Actor_unk25_4(12, 0);
    Actor_unk26_4(0, 0);
    Actor_unk29(20);
    Actor_unk27_4(12, 2);
    Actor_unk30(20);
    Actor_unk28_4(12, 0);
    Actor_unk31(20);
    Actor_unk29_4(0, 3);
    Actor_unk32(30);
    Actor_unk33(10);
    Actor_unk30_4(1, 2);
    Actor_unk34(20);
    Actor_unk31_4(1, 0);
    Actor_unk35(10);
    Actor_unk32_4(3, 4);
    Actor_unk36(20);
    Actor_unk33_4(3, 0);
    Actor_unk37(10);
    Actor_unk34_4(2, 3);
    Actor_unk38(30);
    Actor_unk35_4(2, 0);
    Actor_unk39(10);
    Actor_unk17_3(2, 0, 30);
    Actor_unk36_4(8194, 0);
    }
    Actor_unk4_2(10337);
    Actor_unk40(10);
    Actor_unk18_3(0, 16384, 0);
    Actor_unk19_3(1, 0, 20);
    if (Actor_Apply(0, 0) == 0) {
    Actor_unk41(20);
    Actor_unk20_3(0, 49152, 0);
    Actor_unk21_3(1, 49152, 0);
    Actor_unk22_3(2, 49152, 0);
    Actor_unk42(30);
    Actor_unk43(10);
    Actor_unk37_4(11, 3);
    Actor_unk44(30);
    Actor_unk38_4(11, 0);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    } else {
    Actor_unk5_2(20);
    Actor_unk23_3(0, 49152, 0);
    Actor_unk24_3(1, 49152, 0);
    Actor_unk25_3(2, 49152, 0);
    Actor_unk45(30);
    Actor_unk46(10);
    Actor_unk39_4(11, 4);
    Actor_unk47(20);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    Actor_unk40_4(11, 0);
    }
    Actor_unk48(10);
    Actor_unk41_4(11, 2);
    Actor_unk49(20);
    Actor_unk42_4(11, 0);
    Actor_unk50(10);
    Actor_unk26_3(1, 0, 0);
    Actor_unk27_3(3, 2, 0);
    Actor_unk51(40);
    Actor_unk52(10);
    Actor_unk28_3(11, 258, 40);
    Actor_unk43_4(11, 0);
    Actor_unk53(10);
    Actor_unk29_3(0, 49152, 0);
    Actor_unk30_3(1, 49152, 0);
    Actor_unk31_3(2, 49152, 0);
    Actor_unk32_3(3, 49152, 0);
    Actor_unk54(50);
    Actor_unk55(10);
    Actor_unk44_4(11, 3);
    Actor_unk56(30);
    Actor_unk45_4(11, 0);
    Actor_unk57(10);
    Actor_unk46_4(11, 2);
    Actor_unk58(20);
    Actor_unk47_4(11, 0);
    Actor_unk59(10);
    Actor_unk33_3(3, 256, 40);
    Actor_unk48_4(3, 0);
    Actor_unk60(20);
    Actor_unk34_3(11, 0, 0);
    Actor_unk61(20);
    Actor_unk49_4(11, 0);
    Actor_unk62(10);
    Actor_unk50_4(12, 2);
    Actor_unk63(20);
    Actor_unk35_3(12, 32768, 0);
    Actor_unk64(20);
    Actor_unk51_4(12, 0);
    Actor_unk65(10);
    Actor_unk52_4(11, 4);
    Actor_unk66(20);
    Actor_unk53_4(11, 0);
    Actor_unk67(10);
    Actor_unk54_4(2, 2);
    Actor_unk68(20);
    Actor_unk55_4(2, 0);
    Actor_unk69(10);
    Actor_unk36_3(11, 16384, 0);
    Actor_unk70(30);
    Actor_unk56_4(11, 0);
    Actor_unk71(10);
    Actor_unk37_3(12, 258, 50);
    Actor_unk57_4(12, 0);
    Actor_unk72(20);
    Actor_unk38_3(11, 257, 65);
    Actor_unk39_3(11, 0, 0);
    Actor_unk73(20);
    Actor_unk58_4(11, 0);
    Actor_unk74(20);
    Actor_unk40_3(12, 256, 40);
    Actor_unk59_4(12, 0);
    Actor_unk75(20);
    Actor_unk41_3(12, 16384, 0);
    Actor_unk6_2(40);
    Actor_unk60_4(12, 0);
    Actor_unk76(10);
    Actor_unk42_3(11, 16384, 0);
    Actor_unk77(50);
    Actor_unk43_3(11, 262, 60);
    Actor_unk61_4(11, 0);
    Actor_unk78(10);
    Actor_unk62_4(11, 3);
    Actor_unk79(30);
    Actor_unk63_4(11, 0);
    Actor_unk80(10);
    Actor_unk64_4(0, 2);
    Actor_unk65_4(1, 2);
    Actor_unk66_4(3, 2);
    Actor_unk67_4(2, 2);
    Actor_unk81(30);
    Actor_unk82(10);
    Actor_unk44_3(11, 0, 0);
    Actor_unk83(20);
    Actor_unk68_4(11, 0);
    Actor_unk84(10);
    Actor_unk69_4(12, 2);
    Actor_unk85(30);
    Actor_unk45_3(12, 32768, 0);
    Actor_unk86(20);
    Actor_unk70_4(12, 0);
    Actor_unk87(10);
    Actor_unk46_3(11, 257, 0);
    Actor_unk88(10);
    Actor_unk47_3(1, 57344, 0);
    Actor_unk89(30);
    Actor_unk71_4(1, 4);
    Actor_unk90(20);
    Actor_unk72_4(1, 0);
    Actor_unk91(10);
    Actor_unk48_3(12, 16384, 0);
    Actor_unk92(30);
    Actor_unk49_3(12, 261, 60);
    Actor_unk73_4(12, 0);
    Actor_unk93(10);
    Actor_unk74_4(1, 2);
    Actor_unk94(20);
    Actor_unk75_4(1, 0);
    Actor_unk95(10);
    Actor_unk50_3(11, 16384, 0);
    Actor_unk96(40);
    Actor_unk51_3(11, 258, 40);
    Actor_unk76_4(11, 0);
    Actor_unk97(10);
    Actor_unk52_3(2, 32768, 0);
    Actor_unk98(30);
    Actor_unk53_3(2, 263, 60);
    Actor_unk54_3(2, 49152, 0);
    Actor_unk99(30);
    Actor_unk77_4(2, 4);
    Actor_unk100(20);
    Actor_unk78_4(2, 0);
    Actor_unk101(10);
    Actor_unk79_4(11, 4);
    Actor_unk102(20);
    Actor_unk80_4(11, 0);
    Actor_unk103(10);
    Actor_unk81_4(0, 2);
    Actor_unk82_4(1, 2);
    Actor_unk83_4(3, 2);
    Actor_unk84_4(2, 2);
    Actor_unk104(30);
    Actor_unk105(10);
    Actor_unk85_4(11, 3);
    Actor_unk106(30);
    Actor_unk86_4(11, 0);
    Actor_unk107(20);
    Actor_unk87_4(0, 3);
    Actor_unk88_4(1, 3);
    Actor_unk89_4(3, 3);
    Actor_unk90_4(2, 3);
    Actor_unk108(30);
    Actor_unk109(10);
    Actor_unk91_4(11, 2);
    Actor_unk110(20);
    Actor_unk92_4(11, 0);
    Actor_unk111(10);
    Actor_unk93_4(1, 2);
    Actor_unk112(20);
    Actor_unk94_4(1, 0);
    Actor_unk113(10);
    Actor_unk95_4(11, 3);
    Actor_unk114(30);
    Actor_unk96_4(11, 0);
    Actor_unk115(20);
    Actor_unk97_4(3, 3);
    Actor_unk116(30);
    Actor_unk98_4(3, 0);
    Actor_unk117(20);
    Actor_unk55_3(0, 16384, 0);
    Actor_unk56_3(1, 57344, 0);
    Actor_unk57_3(2, 40960, 0);
    Actor_unk118(30);
    Actor_unk99_4(0, 3);
    Actor_unk119(10);
    Actor_unk100_4(1, 3);
    Actor_unk101_4(2, 3);
    Actor_unk102_4(3, 3);
    Actor_unk120(30);
    Actor_unk58_3(1, 78643, 39321);
    Actor_unk59_3(3, 78643, 39321);
    Actor_unk60_3(2, 78643, 39321);
    Actor_unk103_4(1, 2);
    {
        u8 *rec = Actor_unk121(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_unk61_3(1, y, x);
        }
    }
    Actor_unk122(1);
    Actor_unk62_3(1, 0, 0);
    Actor_unk104_4(3, 2);
    {
        u8 *rec = Actor_unk123(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_unk63_3(3, y, x);
        }
    }
    Actor_unk124(3);
    Actor_unk64_3(3, 0, 0);
    Actor_unk105_4(2, 2);
    {
        u8 *rec = Actor_unk125(0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Actor_unk65_3(2, y, x);
        }
    }
    Actor_unk126(2);
    Actor_unk66_3(2, 0, 0);
    Actor_unk127(10);
    Actor_unk106_4();
}

/*
 * Cutscene script at 0x02000eec.  The owner runs to 0x0200103a and also owns
 * the alignment halfword at 0x0200103e and the literal pool at
 * 0x02001040-0x0200104b; the body is straight-line, with no branch.  The
 * script is a sequence of "act on channel N, then wait k frames" beats.  The
 * channel ids and beat constants are transcribed literally: what each channel
 * drives is not established, and the middle argument 0x105 is unidentified.
 */
void Scene_RunSecondaryScript(void)
{
    void Actor_unk119_4(s32);
    void Actor_unk120_4(s32);
    void Actor_unk121_4(s32);
    void Actor_unk122_4(s32);

    Actor_unk123_4(((s32)&Value_00002880));
    Actor_unk124_4(20);

    Actor_unk125_4(11, 2);
    Actor_unk126_4(20);
    Actor_unk127_4(11, 0);
    Actor_unk128_4(10);

    Actor_unk129_4(1, 0, 50);
    Actor_unk130_4(0, 0x105, 60);
    Actor_unk131_3(10);
    Actor_unk132_3(1, 0xc000, 0);
    Actor_unk133_3(20);
    Actor_unk134_3(1, 0);
    Actor_unk135_3(10);

    Actor_unk136_3(3, 4);
    Actor_unk137_3(20);
    Actor_unk138_3(3, 0);
    Actor_unk139_3(10);

    Actor_unk140_3(2, 2);
    Actor_unk141_3(20);
    Actor_unk142_3(2, 0);
    Actor_unk143_3(10);

    Actor_unk144_3(12, 2);
    Actor_unk145_3(20);
    Actor_unk146_3(12, 0);
    Actor_unk147_3(20);

    Actor_unk148_3(0, 2);
    Actor_unk149_3(20);
    Actor_unk119_4(25);

    Actor_unk150_3(1, 2);
    Actor_unk151_3(30);
    Actor_unk152_3(1, 0);
    Actor_unk153_3(30);

    Actor_unk154_3(1, 4);
    Actor_unk120_4(20);
    Actor_unk155_2(1, 0);
    Actor_unk156_2(10);

    Actor_unk157_2(3, 4);
    Actor_unk121_4(20);
    Actor_unk158_2(3, 0);
    Actor_unk159_2(10);

    Actor_unk160_2(2, 3);
    Actor_unk122_4(30);
    Actor_unk161_2(2, 0);
    Actor_unk162_2(10);

    Actor_unk163_2(2, 0, 30);
    Actor_unk130(0x2002, 0);
}
