#include "types.h"
#include "scene.h"
#include "facing_object.h"

/* overlays/scene/story/progress_actor/progress_dependent_actor_scene_head.c */

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gWork[];

struct ObjectRuntime *Story_unk3_4(u32);

/* Each Func_ symbol names the loader-relocated call word the image holds for
 * one call site, not a runtime address, so several names can reach the same
 * target. Declarations are old-style where the arity varies between sites. */

struct ObjectRuntime;

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        ang = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Returns the in-image table at 0x02009cf4. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9CF4(void)
{
    return (u8 *)0x02009cf4;
}

/*
 * Returns the in-image table at 0x02009db4. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9db4(void)
{
    return (u8 *)0x02009db4;
}

/*
 * Returns the in-image table at 0x02009ddc. The eight-byte owner at
 * 0x0200009c includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9ddc(void)
{
    return (u8 *)0x02009ddc;
}

/* Picks one of three tables by story progress, testing flags 0x87a then
 * 0x834. */
u8 *SceneData_SelectTable9efcByFlags(void)
{
    if (Story_unk13(0x87A) != 0) {
        return gOv;
    }
    if (Story_unk14(0x834) != 0) {
        return gOv2;
    }
    return gOv3;
}

/*
 * Runs the scene's fixed position and pose sequence for actors 0, 1, 5 and
 * 13, skipped entirely when flag 0x801 is set. Two of the record fields feed
 * the placement calls as s16 values shifted into fixed point, and base5_fbf
 * holds a base that later sites use as base + 1 and base + 2.
 */
void Scene_RunActorPositionSequence(void)
{
    u8 *record;
    s32 x, y;
    s32 base5_fbf;

    if (Story_Check(0x801) != 0) {
    } else {
        Story_unk4_4();
        Story_Place(0, 0x10000, 0x8000);
        Story_Do(0xfa6);
        Story_unk5_4(13, 1);
        Story_unk2_3(0, 232, 0x108);
        Story_unk6_4(0, 0);
        Story_unk7_4(0, 13, 20);
        Story_unk8_4(13, 2);
        Story_unk9_4(13, 0, 10);
        record = (u8 *)Story_unk3_4(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Story_unk10_4(5, x << 16, y << 16);
        Story_unk11_4(1, x << 16, y << 16);
        Story_unk3_3(5, 0x8000, 0x4000);
        Story_unk4_3(1, 0x8000, 0x4000);
        Story_unk5_3(5, 248, 0x108);
        Story_unk6_3(1, 216, 0x108);
        Story_unk12_4(0, 1);
        Story_unk13_4(5, 1);
        Story_unk14_4(1, 1);
        Story_unk15_4(4);
        Story_unk7_3(5, 0xb000, 0);
        Story_unk8_3(1, 0xd000, 20);
        Story_unk16_4(5, 4);
        Story_unk17_4(10);
        Story_unk18_4(5, 0, 20);
        Story_unk19_4(13, 1);
        Story_unk20_4(10);
        Story_unk9_3(13, 0x3000, 10);
        Story_unk21_4(13, 0, 10);
        Story_unk10_3(1, 0x3000, 40);
        Story_unk22_4(1, 2);
        Story_unk23_4(1, 0, 10);
        Story_unk24_4(13, 2);
        Story_unk11_3(13, 0x5000, 10);
        Story_unk25_4(13, 3);
        Story_unk26_4(13, 0, 8);
        Story_unk12_3(5, 0x3000, 20);
        Story_unk27_4(5, 0, 10);
        Story_unk13_3(13, 0x3000, 10);
        Story_unk28_4(13, 3);
        Story_unk29_4(10);
        Story_unk30_4(13, 0, 6);
        Story_unk14_3(0, 0x101, 0);
        Story_unk15_3(1, 0x101, 0);
        Story_unk16_3(5, 0x101, 60);
        Story_unk17_3(1, 0xd000, 0);
        Story_unk18_3(5, 0xb000, 20);
        Story_unk31_4(5, 2);
        Story_unk32_4(5, 0, 10);
        Story_unk19_3(13, 0x102, 60);
        Story_unk33_4(13, 4);
        Story_unk34_4(13, 0, 10);
        Story_unk35_4(1, 0, 0);
        Story_unk20_3(5, 0x8000, 20);
        Story_unk21_3(0, 0x100, 60);
        Story_unk22_3(1, 0xd000, 0);
        Story_unk23_3(5, 0xb000, 0);
        Story_unk24_3(13, 0x5000, 20);
        Story_unk25_3(13, 0x3000, 40);
        Story_unk26_3(13, 0x5000, 40);
        Story_unk27_3(13, 0x3000, 20);
        Story_unk2(13, 0);
        if (Story_unk3(0, 0) == 0) {
            Story_unk36_4((s32)gVal);
        } else {
            Story_unk2_2(0xfb1);
        }
        Story_unk37_4(20);
        Story_unk38_4(13, 3);
        Story_unk39_4(10);
        Story_unk40_4(13, 0, 10);
        Story_unk3_2(0xfb2);
        Story_unk41_4(5, 2);
        Story_unk28_3(5, 0x8000, 10);
        Story_unk42_4(5, 0, 6);
        Story_unk29_3(1, 0x103, 30);
        Story_unk43_4(1, 4, 30);
        Story_unk44_4(1, 0, 10);
        Story_unk45_4(1, 0, 6);
        Story_unk46_4(0, 1, 10);
        Story_unk47_4(0, 5, 0);
        Story_unk48_4(13, 1, 10);
        Story_unk49_4(13, 5, 10);
        Story_unk50_4(0, 3);
        Story_unk51_4(1, 3);
        Story_unk52_3(5, 3);
        Story_unk53_3(10);
        Story_unk54_3(5, 1);
        Story_unk55_3(1, 1);
        Story_unk56_3(0, 0);
        Story_unk30_3(0, 0x4000, 0);
        Story_unk31_3(1, 0x4000, 0);
        Story_unk32_3(5, 0x4000, 16);
        Story_Run(13, 0x102);
        Story_unk57_3(13, 3);
        Story_unk58_3(10);
        Story_unk59_3(13, 0, 6);
        Story_unk33_3(0, 0x100, 0);
        Story_unk34_3(1, 0x100, 0);
        Story_unk35_3(5, 0x100, 40);
        Story_unk60_3(13, 4);
        Story_unk61_3(10);
        Story_unk62_3(13, 0, 6);
        Story_unk63_3(13, 1);
        Story_unk64_3(13, 0, 6);
        Story_unk36_3(5, 0xb000, 10);
        Story_unk65_3(5, 0, 6);
        Story_unk37_3(13, 0x3000, 10);
        Story_unk66_3(13, 3);
        Story_unk67_3(6);
        Story_unk68_3(13, 0, 10);
        Story_unk69_3(0, 2, 0);
        Story_unk70_3(1, 2, 0);
        Story_unk71_3(5, 2, 10);
        Story_unk38_3(0, 0xc000, 0);
        Story_unk39_3(1, 0xd000, 20);
        Story_unk72_3(1, 0, 6);
        Story_unk40_3(13, 0x5000, 10);
        Story_unk73_3(13, 3);
        Story_unk74_3(16);
        Story_unk75_3(0, 5, 40);
        Story_unk76_3(5, 0, 10);
        Story_unk77_3(13, 2);
        Story_unk41_3(13, 0x3000, 10);
        Story_unk78_3(13, 0, 6);
        Story_unk42_3(0, 0xc000, 0);
        Story_unk43_3(5, 0xb000, 30);
        Story_unk44_3(0, 0x105, 0);
        Story_unk45_3(1, 0x105, 0);
        Story_unk46_3(5, 0x105, 80);
        Story_unk79_3(13, 4);
        Story_unk4(13, 0);
        if (Story_unk5(0, 0) == 0) {
            Story_unk4_2(0xfbd);
        } else {
            Story_unk5_2(0xfbe);
        }
        Story_unk80_3(13, 0, 20);
        base5_fbf = (s32)gVal2;
        Story_unk81_3(base5_fbf);
        Story_unk82_3(1, 0, 10);
        Story_unk83_3(1, 2);
        Story_unk6(1, 0);
        if (Story_unk7(0, 0) == 0) {
            Story_unk84_3((base5_fbf + 1));
        } else {
            Story_unk85_3((base5_fbf + 2));
        }
        Story_unk86_3(1, 0, 6);
        Story_unk6_2(0xfc2);
        Story_unk47_3(5, 0x8000, 10);
        Story_unk87_3(5, 1);
        Story_unk88_3(5, 0);
        Story_unk89_3(4);
        if (Story_unk8(0, 0) == 1) {
            Story_unk90_3(5, 2, 20);
            Story_unk91_3(5, 0, 10);
        } else {
            Story_unk92_3(0, 3);
            Story_unk93_3(1, 3);
            Story_unk94_3(5, 3);
            Story_unk95_3(8);
            Story_unk96_3(0, 0);
            bump_step(1);
        }
        Story_unk97_3(13, 3);
        Story_unk98_3(10);
        Story_unk99_3(13, 0, 10);
        Story_unk48_3(0, 0xc000, 0);
        Story_unk49_3(1, 0xd000, 0);
        Story_unk50_3(5, 0xb000, 10);
        Story_unk100_3(0, 3);
        Story_unk101_3(1, 3);
        Story_unk102_3(5, 3);
        Story_unk103_3(10);
        Story_unk104_3(0, 0);
        Story_unk105_3(20);
        Story_unk106_3(13, 2);
        Story_unk107_3(13, 0);
        Story_unk108_3(4);
        if (Story_unk9(0, 0) == 0) {
            Story_unk7_2(0xfc6);
        } else {
            Story_unk8_2(0xfc9);
        }
        ((void (*)())Story_unk15)(10);
        Story_unk109_3(1, 2);
        Story_unk110_3(1, 0, 10);
        Story_unk111_3(1, 0, 6);
        Story_unk51_3(5, 0x8000, 10);
        Story_unk112_3(5, 4);
        Story_unk113_3(5, 0, 6);
        Story_unk114_3(1, 2);
        Story_unk52_2(1, 0x8000, 10);
        Story_unk53_2(1, 0x103, 30);
        Story_unk115_3(1, 0, 10);
        Story_unk54_2(13, 0x100, 40);
        Story_unk116_3(13, 4, 40);
        Story_unk55_2(0, 0xc000, 0);
        Story_unk56_2(1, 0xc000, 0);
        Story_unk57_2(5, 0xb000, 20);
        Story_unk58_2(13, 0xb000, 10);
        Story_unk117_3(158);
        Story_unk59_2(0x200a0ac, 43, 8);
        Story_unk60_2(13, 0x10000, 0x8000);
        Story_unk118_3(13, 232, 218);
        Story_unk119_3(13, 0, 0);
        Story_unk61_2(0, 0x101, 0);
        Story_unk62_2(1, 0x101, 0);
        Story_unk63_2(5, 0x101, 60);
        *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x202;
        Story_unk120_3();
        Story_unk121_3();
        Story_unk122_3(13);
        Story_unk123_3();
    }
}

/* overlays/scene/story/progress_actor/progress_dependent_actor_scene_tail.c */

extern u8 gOv4[];

/* Contiguous unnamed state-owner run for resource_375. */

/* The same import at two sites, so two names: the flag set at +0x1e and again
 * at +0x2e. */

void Scene_RunScene375(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_unk124_3();
    Story_unk125_3(13, 0, 0);
    Story_unk64_2(1, 0xd80000, 0x1080000);
    Story_unk65_2(5, 0xf80000, 0x1080000);
    record = Story_unk16(1);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    record = Story_unk17(5);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Story_unk66_2(0x200a0ac, 43, 8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_unk126_3();
    Story_unk127_3();
    Story_unk128_3(40);
    Story_unk67_2(13, 0xcccc, 0x6666);
    Story_unk68_2(13, 0xe60000, 0xdc0000);
    Story_unk129_3(13, 230, 232);
    Story_unk130_3(20);
    Story_unk131_3(13, 3);
    Story_unk9_2(0xfcc);
    Story_unk132_3(13, 0, 10);
    Story_unk133_3(13, 2);
    Story_unk69_2(13, 0x3000, 10);
    Story_unk134_3(13, 0, 10);
    Story_unk135_3(1, 0, 0);
    Story_unk70_2(0, 0x8000, 10);
    Story_unk136_3(0, 3);
    Story_unk137_3(1, 3);
    Story_unk138_3(0, 0);
    Story_unk139_3(20);
    Story_unk140_3(0, 0, 0);
    Story_unk71_2(5, 0x8000, 10);
    Story_unk141_3(5, 3);
    Story_unk142_3(0, 3);
    Story_unk143_3(0, 0);
    Story_unk72_2(1, 0xcccc, 0x6666);
    Story_unk73_2(5, 0xcccc, 0x6666);
    Story_unk144_3(0, 0xc000, 0);
    Story_unk145_3(1, 2);
    record = Story_unk10(0);
    if (record != 0) {
        Story_unk146_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk147_3(5, 2);
    record = Story_unk11(0);
    if (record != 0) {
        Story_unk148_3(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk149_3(13, 2);
    record = Story_unk12(0);
    if (record != 0) {
        Story_unk150_3(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk151_3(1);
    Story_unk152_3(1, 0, 0);
    Story_unk153_3(5, 0, 0);
    Story_unk154_3(13);
    Story_unk155_3(13, 0, 0);
    Story_unk156_3(1, 1);
    Story_unk157_3(5, 1);
    Story_unk158_3(13, 1);
    Story_unk159_3(14, 0, 0);
    Story_unk160_3(15, 0, 0);
    Story_unk10_2(0x801);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_unk2_4(0x10000, 0);
    Story_unk11_2(0x242);
    Story_unk161_3();
}

void Scene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (Story_unk18(0x242) == 0) {
        Story_unk12_2(0x9E);
        Story_unk74_2(gOv4, 0x2B, 8);
    }
    Story_unk75_2(0, 0xE5, 0xD9);
    Story_unk13_2(3);
}

void State_SetWorkAndFlag87d(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_unk162_3();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_unk14_2(0x87D);
    Story_Apply(12, 0);
    Story_unk15_2(0x900);   /* 144 << 4 */
    Story_unk163_3();
}

void State_SetWorkAndFlag87e(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_unk164_3();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_unk16_2(0x87E);
    Story_Apply2(12, 1);
    Story_unk17_2(0x900);   /* 144 << 4 */
    Story_unk165_3();
}

void Dialogue_RunActorSixteenDialogue(void)
{
    Story_unk166_3();
    Story_unk18_2(0x1C96);
    Story_Apply3(16, 0);
    Story_unk167_3();
}
