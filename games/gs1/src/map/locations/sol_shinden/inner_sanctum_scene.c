#include "types.h"
#include "scene.h"

/* map/locations/sol_shinden/inner_sanctum_scene/actor_motion.c */
typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} EntA;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern u8 Value_00001000;
extern u16 gOv;
/* 手番カウンタ。他のオーバーレイからも書き換わるため volatile。
 * The reference reloads this cell on the path where the compiler can prove the
 * value is unchanged, which in ordinary C only a volatile object produces. */
extern volatile s32 gOv2;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;

EntA *Map_unk30_4(s32);

EntA *Map_unk31_4(s32);

EntB *Map_unk32_4(s32);

void UpdateStatueTrapActor(void)
{
    EntA *scene_actor;
    EntA *target_actor;
    EntB *target_position;
    s32 g1 = 0x810;
    s32 g2 = 0x810;
    s32 g3 = 0x810;
    s32 g4 = 0x810;
    s32 s1 = 0x10000;
    s32 s2 = 0x8000;
    s32 s3 = 0x20000;
    s32 s4 = 0x10000;
    s32 s5 = 0x4000;
    s32 d1 = 0x120;
    s32 d2 = 0x120;
    s32 d3 = 0x120;
    s32 d4 = 0x120;
    s32 d5 = 0xc000;
    s32 h1;
    s32 h2;

    scene_actor = Map_unk30_4(16);
    if (Map_unk41(0x809) == 0) {
        return;
    }
    if (Map_unk42(0x814) != 0) {
        Map_unk33_4();
        return;
    }
    if (Map_unk43(0x819) != 0) {
        return;
    }
    Map_unk44();
    Map_Apply(0, 0);
    Map_unk43_2((s32)&Value_00001000);
    if (Map_unk45(g1)!= 0 || Map_unk46(0x80a) == 0) {
        target_actor = Map_unk31_4(0);
        if (target_actor != 0) {
            Map_unk39_3(16, target_actor->unk8, target_actor->unk10);
        }
        Map_unk47(4);
        Map_unk40_3(16, s1, s2);
    } else {
        if (Map_unk48(g2)!= 0) goto do1;
        h1 = 0x1540000;
        if (scene_actor->unk8 > h1) {
do1:
            Map_unk41_3(16, 0x1880000, 0xa80000);
            Map_unk44_2(4);
            Map_unk42_3(16, s3, s4);
        }
    }
    if (Map_unk49(g3)!= 0) goto do2;
    h2 = 0x1540000;
    if (scene_actor->unk8 > h2) {
do2:
        Map_unk43_3(16, d1, 0xe8);
    } else {
        Map_unk45_2(0x80a);
    }
    Map_unk44_3(16, d2, 0xe8);
    Map_unk45_3(0, d5, 0);
    Map_unk34_4(16, s5, 10);
    Map_unk46_3(16, 0, 10);
    Map_Apply2(0, 3);
    if (Map_unk50(g4)!= 0 || Map_unk51(0x80a) == 0) {
        Map_Apply3(16, 2);
        target_position = Map_unk32_4(0);
        if (target_position != 0) {
            Map_unk47_3(16, target_position->unkA, target_position->unk12);
        }
        Map_unk46_2(16);
        Map_unk35_4(16, 0, 0);
        Map_unk48_3(0, d3, 0xe8);
    } else {
        Map_unk49_3(0, d4, 0xf8);
    }
    Map_unk36_4();
}

void UpdateStatueLight1(void)
{
    volatile s32 *st;
    s32 s;
    s32 t1 = 0x10000;
    s32 t2 = 0x10000;
    s32 t3 = 0x10000;
    s32 m1 = -1;
    s32 m2 = -1;
    s32 m3 = 0xe666;

    if ((Map_unk52() & 3) != 0) {
        u16 v = gOv;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_unk47_2(0xbb);
            a = 1;
            b = 5;
            Map_unk10_5(0x2e, 0x3b, 30, 0x21, a, b);
            break;
        }
        case 1:
            Map_unk11_5(0x2e, 0x3b, 30, 0x21, v, v);
            Map_unk12_5(0x2e, 0x3b, 30, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Map_unk13_5(0x2e, 0x3b, 30, 0x22, v, v);
            Map_unk12_5(0x2e, 0x3b, 30, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Map_unk14_5(0x2e, 0x3b, 30, 0x23, v, v);
            Map_unk12_5(0x2e, 0x3b, 30, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            volatile s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_unk15_5(0x2e, 0x3b, 30, 0x24, v, v);
            Map_unk12_5(0x2e, 0x3b, 30, 0x25, v, 5);
            break;
        case 0x50: {
            s32 a = 1;
            s32 b = 10;
            Map_unk16_5(0x2e, 0x31, 30, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_unk53() * 40) >> 16) + 90) {
                *c = 0;
            }
        }
    }
    st = &gOv2;
    s = *st;
    if (s != 0) {
        if (s == 2) {
            Map_unk50_3(t1, t2, t3);
        } else if (s == 1) {
            Map_unk51_3(m1, m2, m3);
        }
        *st = *st - 1;
    }
}

void UpdateStatueLight2(void)
{
    if ((Map_unk54() & 3) != 0) {
        u16 v = gOv3;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_unk48_2(0xbb);
            a = 1;
            b = 5;
            Map_unk17_5(0x2f, 0x3b, 42, 0x21, a, b);
            break;
        }
        case 1:
            Map_unk18_5(0x2f, 0x3b, 42, 0x21, v, v);
            Map_unk19_5(0x2f, 0x3b, 42, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Map_unk20_5(0x2f, 0x3b, 42, 0x22, v, v);
            Map_unk19_5(0x2f, 0x3b, 42, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Map_unk21_5(0x2f, 0x3b, 42, 0x23, v, v);
            Map_unk19_5(0x2f, 0x3b, 42, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_unk22_5(0x2f, 0x3b, 42, 0x24, v, v);
            Map_unk19_5(0x2f, 0x3b, 42, 0x25, v, 5);
            break;
        case 0x5a: {
            s32 a = 1;
            s32 b = 10;
            Map_unk23_5(0x2f, 0x31, 42, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv3;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_unk55() * 40) >> 16) + 100) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Map_unk56() & 3) != 0) {
        u16 v = gOv4;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_unk49_2(0xbb);
            a = 1;
            b = 5;
            Map_unk24_5(0x30, 0x3b, 31, 0x24, a, b);
            break;
        }
        case 1:
            Map_unk25_5(0x30, 0x3b, 31, 0x24, v, v);
            Map_unk26_5(0x30, 0x3b, 31, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Map_unk27_5(0x30, 0x3b, 31, 0x25, v, v);
            Map_unk26_5(0x30, 0x3b, 31, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Map_unk28_5(0x30, 0x3b, 31, 0x26, v, v);
            Map_unk26_5(0x30, 0x3b, 31, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_unk29_5(0x30, 0x3b, 31, 0x27, v, v);
            Map_unk26_5(0x30, 0x3b, 31, 0x28, v, 5);
            break;
        case 0x5f: {
            s32 a = 1;
            s32 b = 10;
            Map_unk30_5(0x30, 0x31, 31, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv4;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_unk57() * 40) >> 16) + 105) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Map_unk58() & 3) != 0) {
        u16 v = gOv5;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_unk50_2(0xbb);
            a = 1;
            b = 5;
            Map_unk31_5(0x2e, 0x3b, 41, 0x24, a, b);
            break;
        }
        case 1:
            Map_unk32_5(0x2e, 0x3b, 41, 0x24, v, v);
            Map_unk33_5(0x2e, 0x3b, 41, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Map_unk34_5(0x2e, 0x3b, 41, 0x25, v, v);
            Map_unk33_5(0x2e, 0x3b, 41, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Map_unk35_5(0x2e, 0x3b, 41, 0x26, v, v);
            Map_unk33_5(0x2e, 0x3b, 41, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_unk36_5(0x2e, 0x3b, 41, 0x27, v, v);
            Map_unk33_5(0x2e, 0x3b, 41, 0x28, v, 5);
            break;
        case 0x55: {
            s32 a = 1;
            s32 b = 10;
            Map_unk37_5(0x2e, 0x31, 41, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv5;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_unk59() * 40) >> 16) + 95) {
                *c = 0;
            }
        }
    }
}

/* map/locations/sol_shinden/inner_sanctum_scene/actor_position_transition.c */
#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE Value_000011b4
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE Value_0000100d
#else
#define SCENE_STEP_VALUE Value_0000102b
#endif

extern u8 Value_0000100d[];
extern u8 Value_0000102b[];
extern u8 Value_000011b4[];

extern u8 gWork[];

/*
 * Withdrawn on 2026-09-10 as not reproducible, and reinstated the same night
 * once the cause was found and removed.
 *
 * The pair of equal 0x6480000 arguments in the last call below made this
 * owner's compile split: thirty raw cc1 runs on one fixed preprocessed input
 * produced 27 of one output and 3 of another, the minority recomputing the
 * shift instead of sharing it through mov r2, r1. Three sessions measured it
 * independently and agreed.
 *
 * The cause was not the source. GCC 2.96 hashes LABEL_REF by rtx address and
 * SYMBOL_REF by the address of its name, so macOS ASLR reordered equal-constant
 * discovery in CSE between runs. The toolchain now spawns compilers with
 * _POSIX_SPAWN_DISABLE_ASLR and the split collapses to a single output.
 *
 * Kept as a marker: raw runs spawned outside the toolchain still split 27/3,
 * so anyone measuring this owner by hand must disable ASLR or they will
 * rediscover the artefact rather than a property of this C.
 */
void Scene_RunActorPositionTransition(void)
{
    u32 i;
    s32 record;

    Map_unk37_4(21);
    Map_Place(0, 0x178, 184);
    Map_unk38_4(0, 0);
    Map_unk2_3(16, 0x1780000, 0xb80000);
    Map_unk3_3(16, 0x10000, 0x8000);
    Map_unk4_3(16, 0x188, 168);
    Map_unk5_3(16, 0x8000, 30);
    Map_unk39_4(16, 1);
    Map_Do((s32)SCENE_STEP_VALUE);
    Map_unk40_4(16, 4, 30);
    Map_unk41_4(16, 6);
    Map_unk42_4(0, 2);
    Map_unk43_4(6);
    Map_unk44_4(16, 3);
    Map_unk45_4(16, 6);
    Map_unk6_3(16, 0x178, 184);
    Map_unk7_3(16, 0x6480000, 0x6480000);
    Map_unk46_4(4);
    Map_unk2_2(0x811);
}

/* map/locations/sol_shinden/inner_sanctum_scene/field_scene.c */
extern u8 gWork[];
extern s16 gOv, gOv3, gOv4, gOv5;
extern s32 gOv2;

void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

void Scene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;
    u8 *work;

    Map_Run(-1, -1, -1, 0);
    Map_SetRect(30, 43, 32, 40, 8, 3);
    Map_unk2_5(30, 43, 33, 39, 8, 1);
    Map_unk3_5(30, 43, 36, 38, 3, 3);
    Map_unk4_5(14, 41, 32, 41, 8, 4);
    Map_unk2_4(0x23e0000, -1, 0x9e0000, 0);
    Map_unk47_4();
    Map_unk8_3(16, 0x23e0000, 0x780000);
    Map_unk48_4(0, 0, 0);
    Map_unk49_4(1);
    Map_unk3_4(0x2051cc, 1);
    Map_unk50_4(20);
    Map_unk3_2(0x201);
    Map_unk4_2(0x200);
    Map_unk5_2(0x202);
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Map_unk51_4();
    ((void (*)())Map_unk60)();
    Map_unk52_4(40);
    Map_unk53_4(171);
    Map_unk4_4(0x10005, 1);
    Map_unk54_4(8);
    Map_unk55_4(32);
    Map_Check(0x2051cc, 1);
    Map_unk56_4(24);
}

void Scene_RunClosingSequence(void)
{
    s32 i;
    u8 *work;
    Map_unk57_4();
    Map_unk58_4();
    gOv = 0;
    gOv3 = 0;
    gOv4 = 0;
    gOv5 = 0;
    Map_unk59_4(4097);
    Map_unk9_3(16, 16384, 20);
    Map_unk10_3(16, 256, 0);
    Map_unk60_4(16, 6, 30);
    Map_unk61_4(37617664, -1, 11403264, 1);
    Map_unk62_4();
    Map_unk63_4(30);
    Map_unk64_4(32784, 20);
    for (i = 0; i != 4; i++) {
        Map_unk65_4(246);
        Map_unk66_4();
        Map_unk67_4(12);
        Map_unk68_4(246);
        Map_unk69_4();
        Map_unk70_4(12);
    }
    Map_unk71_4(32784, 6);
    gOv = ((Map_unk72_4() * 60) >> 16) + 20;
    gOv3 = ((Map_unk73_4() * 60) >> 16) + 20;
    gOv4 = ((Map_unk74_4() * 60) >> 16) + 20;
    gOv5 = ((Map_unk75_4() * 60) >> 16) + 20;
    gOv2 = 0;
    Map_unk2((s32)UpdateStatueLight1, 3200);
    Map_unk3((s32)UpdateStatueLight2, 3200);
    Map_unk4((s32)UpdateStatueLight3, 3200);
    Map_unk5((s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Map_unk76_4(246);
        Map_unk77_4();
        Map_unk78_4(5);
        Map_unk79_4(246);
        Map_unk80_4();
        Map_unk81_4(5);
    }
    for (i = 0; i != 8; i++) {
        Map_unk82_3(246);
        Map_unk83_3();
        Map_unk84_3(4);
        Map_unk85_3(246);
        Map_unk86_3();
        Map_unk87_3(4);
    }
    for (i = 0; i != 10; i++) {
        Map_unk88_3(246);
        Map_unk89_3();
        Map_unk90_3(3);
        Map_unk91_3(246);
        Map_unk92_3();
        Map_unk93_3(3);
    }
    for (i = 0; i != 12; i++) {
        Map_unk94_3(246);
        Map_unk95_3();
        Map_unk96_3(2);
        Map_unk97_3(246);
        Map_unk98_3();
        Map_unk99_3(2);
    }
    Map_unk5_5(45, 30, 34, 10, 4, 2);
    Map_unk100_3(16, 6, 40);
    Map_unk101_3(32784, 6);
    Map_unk11_3(16, 131072, 65536);
    Map_unk102_3(16, 576, 280);
    Map_unk103_3((s32)UpdateStatueLight1);
    Map_unk104_3((s32)UpdateStatueLight2);
    Map_unk105_3((s32)UpdateStatueLight3);
    Map_unk106_3((s32)UpdateStatueLight4);
    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Map_unk107_3();
    Map_unk108_3();
    Map_unk109_3(4);
}

void Scene_RunFlaggedSequence(void)
{
    s32 base;
    u8 *work;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Map_unk110_3();
    if (Map_unk6(2059) != 0 && Map_unk7(2086) != 0) {
        Map_unk6_2(2086);
        Map_unk6_5(45, 28, 34, 10, 2, 1);
    } else if (Map_unk8(2059) != 0 && Map_unk9(2086) == 0) {
        Map_unk7_2(2086);
    }
    if (Map_unk10(2060) != 0 && Map_unk11(2087) != 0) {
        Map_unk8_2(2087);
        Map_unk7_5(47, 28, 36, 10, 2, 1);
    } else if (Map_unk12(2060) != 0 && Map_unk13(2087) == 0) {
        Map_unk9_2(2087);
    }
    if (Map_unk14(2061) != 0 && Map_unk15(2088) != 0) {
        Map_unk10_2(2088);
        Map_unk8_5(45, 29, 34, 11, 2, 1);
    } else if (Map_unk16(2061) != 0 && Map_unk17(2088) == 0) {
        Map_unk11_2(2088);
    }
    if (Map_unk18(2062) != 0 && Map_unk19(2089) != 0) {
        Map_unk12_2(2089);
        Map_unk9_5(47, 29, 36, 11, 2, 1);
    } else if (Map_unk20(2062) != 0 && Map_unk21(2089) == 0) {
        Map_unk13_2(2089);
    }
    Map_unk111_3();
    Map_unk12_3(16, 16384, 20);
    Map_unk112_3(16, 6, 30);
    Map_unk113_3(37617664, -1, 11403264, 1);
    Map_unk114_3();
    Map_unk115_3(30);
    for (i6 = 0; i6 != 4; i6++) {
        Map_unk116_3(246);
        Map_unk117_3();
        Map_unk118_3(12);
        Map_unk119_3(246);
        Map_unk120_3();
        Map_unk121_3(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Map_unk122_3(246);
        Map_unk123_3();
        Map_unk124_3(8);
        Map_unk125_3(246);
        Map_unk126_3();
        Map_unk127_3(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Map_unk128_3(246);
        Map_unk129_3();
        Map_unk130_3(6);
        Map_unk131_3(246);
        Map_unk132_3();
        Map_unk133_3(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Map_unk134_3(246);
        Map_unk135_3();
        Map_unk136_3(4);
        Map_unk137_3(246);
        Map_unk138_3();
        Map_unk139_3(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Map_unk140_3(246);
        Map_unk141_3();
        Map_unk142_3(2);
        Map_unk143_3(246);
        Map_unk144_3();
        Map_unk145_3(2);
    }
    Map_unk146_3(246);
    Map_unk147_3();
    Map_unk148_3(6);
    if (Map_unk61(2082) == 0) {
        base = 32784;
        Map_unk149_3(4133);
        Map_unk150_3(base, 6);
        Map_unk151_3(16, 3);
        Map_unk152_3(base, 6);
    }
    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Map_unk153_3();
    Map_unk154_3();
    Map_unk155_3(5);
}

/* map/locations/sol_shinden/inner_sanctum_scene/scene_data.c */
/*
 * Inner sanctum data getters, published in the descriptor table at the head
 * of overlay resource_37a. Sibling overlays share this layout but not the
 * addresses returned.
 */

/*
 * Returns 0x0200aafc, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenScriptData(void)
{
    return (u8 *)0x0200aafc;
}

/*
 * Returns zero. The four-byte owner carries no pool word, since a constant
 * that fits in an immediate needs none, unlike its eight-byte neighbours in
 * the same table. Whether the caller reads the zero as a null pointer or as
 * an integer is not established.
 */
s32 GetSolShindenInitialState(void)
{
    return 0;
}

/*
 * Returns 0x0200abec, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenMessageData(void)
{
    return (u8 *)0x0200abec;
}

/*
 * Returns 0x0200ac14, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenActorData(void)
{
    return (u8 *)0x0200ac14;
}

/*
 * Returns 0x0200ad34, an address inside this overlay's image; its contents
 * are not established. The eight-byte owner includes its one pool word,
 * which sits past the bx lr. The address is published in the descriptor
 * table, not reached by any call.
 */
u8 *GetSolShindenEffectData(void)
{
    return (u8 *)0x0200ad34;
}

/* map/locations/sol_shinden/inner_sanctum_scene/scene_primary_script.c */
extern u8 gWork[];
extern u8 gVal[];
extern u8 gVal2[];

void Scene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (Map_unk5_4()!= 0) {
        record = Map_unk22(0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Map_unk156_3();
        Map_unk6_4(0x2051cc, 1);
        Map_unk157_3(20);
        Map_unk14_2(0x201);
        Map_unk15_2(0x200);
        Map_unk16_2(0x202);
        if (Map_unk23(0x80a) == 0) {
            Map_unk158_3();
        }
        if (Map_unk7_4()!= 0) {
            if (Map_unk24(0x811) == 0) {
                Map_unk159_3();
            }
        }
        Map_unk160_3();
    } else {
        if (Map_unk25(0x200) == 0) {
            Map_unk161_3();
            Map_unk8_4(0x10000, 1);
            Map_unk162_3(20);
            Map_unk17_2(0x200);
            Map_unk18_2(0x201);
            Map_unk19_2(0x202);
            Map_unk163_3();
        }
    }
    L_020000f0:;
}

void Scene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (Map_unk26(0x200) == 0) {
        Map_unk164_3();
        Map_unk9_4(0x10000, 1);
        Map_unk165_3(20);
        Map_unk20_2(0x200);
        Map_unk21_2(0x201);
        Map_unk22_2(0x202);
        Map_unk166_3();
    }
}

void Scene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Map_unk10_4()!= 0) {
        record = Map_unk27(0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Map_unk167_3();
        Map_unk11_4(0x10000, 1);
        Map_unk168_3(20);
        Map_unk23_2(0x200);
        Map_unk24_2(0x201);
        Map_unk25_2(0x202);
        Map_unk169_3();
    } else {
        if (Map_unk28(0x201) == 0) {
            Map_unk170_3();
            Map_unk12_4(0x2051cc, 1);
            Map_unk171_3(20);
            Map_unk26_2(0x201);
            Map_unk27_2(0x200);
            Map_unk28_2(0x202);
            if (Map_unk29(0x80a) == 0) {
                Map_unk172_3();
            }
            Map_unk173_3();
        }
    }
    L_020001d6:;
}

void Scene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (Map_unk30(0x202) == 0) {
        Map_unk13_4(0x202db1, 1);
        Map_unk174_3(20);
        Map_unk29_2(0x202);
        Map_unk30_2(0x200);
        Map_unk31_2(0x201);
    }
}

void Scene_RunScene37a(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Map_unk32_2(0xff6);
    Map_unk13_3(0, 0x10000, 0x8000);
    Map_unk14_3(0, 0x1e8, 176);
    Map_unk175_3(0, 0);
    record = Map_unk31(0);
    if (record != 0) {
        Map_unk176_3(16, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Map_unk177_3(0, 0, 1);
    Map_unk15_3(16, 0x10000, 0x8000);
    Map_unk16_3(16, 0x1d8, 168);
    Map_unk178_3(16, 0, 60);
    Map_unk179_3(16, 4, 40);
    Map_unk180_3(16, 6);
    Map_unk14_4(0x26666, 0x4ccc);
    Map_unk15_4(0x23f0000, -1, 0xb50000, 1);
    Map_unk181_3();
    Map_unk182_3(120);
    Map_unk16_4(0x1010, 80);
    Map_unk17_4(0x1ec0000, -1, 0xa80000, 1);
    Map_unk183_3();
    Map_unk184_3(20);
    base5_4010 = (s32)gVal2;
    Map_unk17_3(16, 0x3000, 20);
    Map_unk185_3(base5_4010, 6);
    Map_unk186_3(16, 0, 60);
    Map_unk187_3(16, 2);
    Map_unk18_3(16, 0x3000, 10);
    Map_unk32(base5_4010, 0);
    if (Map_unk33(0, 0) == 0) {
        Map_unk33_2(0xffa);
    } else {
        Map_unk34_2(0xffb);
    }
    base5_4010_2 = (s32)gVal2;
    Map_unk19_3(0, 0xa000, 10);
    Map_unk188_3(base5_4010_2, 10);
    base6_ffc = (s32)gVal;
    Map_unk189_3(base6_ffc);
    Map_unk190_3(16, 0, 40);
    Map_unk20_3(16, 0x105, 40);
    Map_unk191_3(16, 4);
    Map_unk21_3(16, 0x3000, 10);
    Map_unk192_3(16, 4);
    Map_unk34(base5_4010_2, 0);
    if (Map_unk35(0, 0) == 0) {
        Map_unk193_3((base6_ffc + 1));
        Map_unk35_2(0x896);
    } else {
        Map_unk194_3((base6_ffc + 2));
    }
    Map_unk18_4((s32)gVal2, 4);
    Map_unk195_3(16, 1);
    Map_unk22_3(16, 0x1e6, 131);
    Map_unk23_3(16, 0x240, 120);
    Map_unk24_3(16, 0xc000, 2);
    Map_unk19_4(0x40000, 0x8000);
    Map_unk36_2(0x80a);
}

void Scene_RunScene37aSequenceE(void)
{
    u32 i;
    s32 record;

    if (Map_unk36(0x810) != 0) {
    } else {
        if (Map_unk20_4() == 0) {
        } else {
            Map_unk196_3();
            Map_unk25_3(16, 0x2410000, 0x930000);
            Map_unk26_3(16, 0x4000, 1);
            Map_unk21_4(0x23e0000, -1, 0xb80000, 1);
            Map_unk37_2(0x1027);
            Map_unk27_3(0, 0x240, 232);
            Map_unk197_3(0, 0);
            Map_unk198_3();
            Map_unk199_3(10);
            Map_unk28_3(16, 0x10000, 0x8000);
            Map_unk29_3(16, 0x240, 152);
            Map_unk200_3(6);
            Map_unk201_3(16, 6, 30);
            Map_unk202_3(16, 6);
            Map_unk203_3(0, 3);
            Map_unk204_3(2);
            Map_unk205_3(16, 4);
            Map_unk206_2(16, 6);
            Map_unk22_4(0, 0x102);
            Map_unk207_2(40);
            Map_unk208_2(16, 2);
            Map_unk209_2(30);
            Map_unk210_2(16, 6);
            Map_unk211_2(0, 3);
            Map_unk30_3(16, 0x240, 184);
            Map_unk212_2(6);
            Map_unk213_2(16, 2);
            Map_unk214_2(40);
            Map_unk23_4(0x4010, 6);
            Map_unk31_3(16, 0x240, 208);
            Map_unk215_2(40);
            Map_unk216_2(0, 3);
            Map_unk217_2(6);
            Map_unk32_3(16, 0x8000, 0x4000);
            Map_unk218_2(16, 2);
            record = Map_unk37(0);
            if (record != 0) {
                Map_unk219_2(16, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Map_unk220_2(16);
            Map_unk221_2(16, 0, 0);
            Map_unk38_2(0x810);
            Map_unk222_2();
        }
    }
}

void Scene_RunScene37a(void)
{
    u32 i;
    s32 record;

    Map_unk223_2();
    if (Map_unk38(0x896) != 0) {
        Map_unk39_2(0xffd);
    } else {
        Map_unk40_2(0xfff);
    }
    Map_unk224_2(16, 0, 10);
    Map_unk33_3(16, 0xc000, 10);
    Map_unk225_2();
}

/* map/locations/sol_shinden/inner_sanctum_scene/scene_sequence_f.c */
/* Message ids handed to the Event_SetValue1d8 service; see the header note. */
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];

/*
 * resource_37a owner 0x02000488..0x020009f3 (0x56c = 1388 bytes).
 *
 * Reviewed scene script for the resource_37a sequence guarded by story flag
 * 0x809.  One straight command list: it opens the scene, places and drives
 * four actor slots (0, 1, 5 and 0x10) through a long beat-by-beat schedule of
 * moves and waits, plays two audio cues, branches once on a runtime query
 * near the end to choose between two message ids, then hands the three
 * principal slots back to their walking records, sets story flags 0x144 and
 * 0x809 and closes the scene.
 *
 * Complete owner.  Prologue `push {lr}` at 0x02000488; the single epilogue
 * `pop {r0} / bx r0` at 0x020009da pops the return address, so the owner is
 * void and takes no argument.  Code runs 0x02000488..0x020009d9 with two
 * interior literal pools (0x0200081c..0x0200084f and 0x020009de..0x020009f3);
 * the next prologue is at 0x020009f4.  All 140 `bl` sites are transcribed.
 *
 * Calls are spelled as the pre-relocation call word the image holds, the
 * convention the adopted sibling
 * games/gs1/src/overlays/scene/script/run_scene_37a_sequence_a.c uses,
 * so one target reached from several sites appears under several spellings.
 *
 * RESIDUAL (all 32 remaining differing halfwords).  Eight of those spellings
 * collide: two sites whose different call words happen to decode to the same
 * address (Map_unk227_2, Map_unk228_2, Map_unk229_2, Map_unk230_2,
 * Map_unk231_2, Map_unk232_2, Map_unk233_2 and Map_unk234_2).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Event_SetValue1d8 message service are spelled as
 * `Data_00000<id>` externs, the convention the adopted overlay scene scripts
 * already use for this family (see
 * games/gs1/src/overlays/scene/script/run_scene_372_sequence_a.c,
 * which declares `extern u8 gVal7[]` and casts it to s32 at the call).
 * The reference forces that reading for 0xff0: every other pooled constant in
 * this owner (0x101, 0x105, 0x2005, 0x6666 ...) is one GCC cannot synthesize,
 * while 0xff0 is 0xff << 4 and from a plain integer literal would come out as
 * `movs r0,#255 / lsls r0,r0,#4`.  The reference loads it from the pool
 * instead, so the operand was address-shaped in the original source.  The
 * other three ids are byte-identical either way; they are spelled the same
 * way because they reach the same service.
 *
 * Uncertain: none of the main-image services is established beyond its
 * argument count and access widths, so all are declared old-style and their
 * constants are transcribed rather than interpreted.  The record returned by
 * the Scene_GetRecord service is read at +8/+16 as words early in the scene and
 * at +10/+18 as signed halfwords at the end, so both stay raw offsets; no
 * shared type in games/gs1/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */

/* The sixteen colliding sites reach these main-image services directly. */

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
s32  Map_unk236_2();  /* GameFlag_IsSet */
s32  Map_unk238_2();  /* Scene_GetRecord */
s32  Map_unk239_2();  /* Map_unk240_2 */
s32  Map_unk241_2();  /* Battle_RunEvWait */
void Map_unk243_2();  /* Scene_RunScene37aSequenceA (0x02000054) */
void Map_unk244_2();  /* SetSolShindenActorStep (0x020025fc) */
void Map_unk245_2();  /* Map_unk246_2 */
void Map_unk247_2();  /* Map_unk248_2 */
void Map_unk249_2();  /* Motion_SetSpeed */
void Map_unk251_2();  /* Map_unk252_2 */
void Map_unk253_2();  /* Object_SetModeById */
void Map_unk255_2();  /* Event_SetValue1d8 */
void Map_unk256_2();  /* Motion_SetHPosTerrain */
void Map_unk258_2();  /* Motion_ArmCb */
void Map_unk260_2();  /* Motion_Launch */
void Map_unk262_2();  /* Map_unk263_2 */
void Map_unk264_2();  /* BattleFx_SpawnLinked */
void Map_unk266_2();  /* Audio_PlayCue */
void Map_unk268_2();  /* Motion_SetVarCbAndRefresh */
void Map_unk270_2();  /* Motion_SetModeAndWaitAnimation */
void Map_unk272_2();  /* ObjectTable_AllocateAndSetObjectSpeed */
void Map_unk180_3();  /* Motion_CamBounds */
void Map_unk275_2();  /* Map_unk276_2 */
void Map_unk277_2();  /* BattleEv_RunWait */
void Map_unk231_2();  /* main-image name; collision fallback, see the RESIDUAL note */
void Map_unk279_2();  /* Motion_CommitPos */
void Map_unk281_2();  /* GameFlag_Set */
void Map_unk283_2();  /* BattleFx_FinishAction */

void Scene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Map_unk39(0x814) != 0) {
        Map_unk243_2();
    }
    if (Map_unk39(0x809) == 0) {
        Map_unk245_2();
        Map_unk41_2((s32)gVal3);
        Map_unk266_2(17);
        Map_unk34_3(0, 0x10000, 0x8000);
        Map_unk35_3(0, 0x120, 232);
        Map_unk253_2(0, 0);
        Map_unk247_2(20);
        Map_unk266_2(21);
        Map_unk36_3(0, 0xc000, 0);
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk256_2(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Map_unk34_3(16, 0x16666, 0xb333);
        Map_unk35_3(16, 0x120, 206);
        Map_unk247_2(40);
        Map_unk37_3(16, 0x100, 0);
        Map_unk260_2(16, 4, 60);
        Map_unk244_2(16, 20);
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk256_2(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk256_2(5, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Map_unk34_3(1, 0x8000, 0x4000);
        Map_unk34_3(5, 0x8000, 0x4000);
        Map_unk38_3(1, 0x118, 248);
        Map_unk35_3(5, 0x128, 248);
        Map_unk253_2(1, 1);
        Map_unk36_3(1, 0xd000, 0);
        Map_unk36_3(5, 0xb000, 30);
        Map_unk24_4(0x9999, 0x1333);
        Map_unk25_4(0x1200000, -1, 0xd50000, 1);
        Map_unk34_3(16, 0x6666, 0x3333);
        Map_unk35_3(16, 0x120, 176);
        Map_unk247_2(40);
        Map_unk268_2(16, 2);
        Map_unk244_2(16, 6);
        Map_unk36_3(16, 0x4000, 60);
        Map_unk244_2(16, 20);
        Map_unk258_2(16, 0, 40);
        Map_unk270_2(16, 3);
        Map_unk247_2(10);
        Map_unk36_3(16, 0x8000, 40);
        Map_unk270_2(16, 3);
        Map_unk247_2(20);
        Map_unk268_2(5, 2);
        Map_unk36_3(5, 0x9000, 10);
        Map_unk244_2(5, 10);
        Map_unk268_2(1, 2);
        Map_unk36_3(1, 0xf000, 10);
        Map_unk244_2(1, 6);
        Map_unk26_4(5, 0x102);
        Map_unk247_2(40);
        Map_unk36_3(5, 0xa000, 10);
        Map_unk27_4(0x2005, 10);
        Map_unk268_2(16, 2);
        Map_unk247_2(10);
        Map_unk36_3(16, 0xa000, 20);
        Map_unk26_4(16, 0x102);
        Map_unk247_2(20);
        Map_unk36_3(0, 0x5000, 40);
        Map_unk36_3(0, 0xc000, 0);
        Map_unk36_3(1, 0xe000, 0);
        Map_unk36_3(5, 0xa000, 40);
        Map_unk37_3(1, 0x101, 20);
        Map_unk277_2(1, 0);
        Map_unk247_2(60);
        Map_unk270_2(16, 4);
        Map_unk247_2(40);
        Map_unk244_2(16, 20);
        Map_unk37_3(5, 0x101, 40);
        Map_unk244_2(5, 60);
        Map_unk270_2(16, 3);
        Map_unk244_2(16, 10);
        Map_unk37_3(0, 0x105, 0);
        Map_unk37_3(1, 0x105, 0);
        Map_unk37_3(5, 0x105, 60);
        Map_unk268_2(1, 2);
        Map_unk247_2(20);
        Map_unk244_2(1, 10);
        Map_unk270_2(16, 3);
        Map_unk247_2(20);
        Map_unk37_3(0, 0x102, 0);
        Map_unk37_3(1, 0x102, 0);
        Map_unk37_3(5, 0x102, 80);
        Map_unk37_3(16, 0x105, 80);
        Map_unk244_2(16, 6);
        Map_unk36_3(0, 0x4000, 0);
        Map_unk36_3(1, 0xf000, 0);
        Map_unk36_3(5, 0x9000, 60);
        Map_unk36_3(16, 0x4000, 10);
        Map_unk268_2(16, 3);
        Map_unk247_2(6);
        Map_unk28_4(16, 0);
        if (Map_unk29_4(0, 0) == 0) {
            Map_unk255_2((s32)gVal4);
        } else {
            Map_unk41_2((s32)gVal5);
            Map_unk37_3(16, 0x107, 20);
        }
        Map_unk260_2(16, 4, 20);
        Map_unk36_3(0, 0xc000, 0);
        Map_unk36_3(1, 0xe000, 0);
        Map_unk36_3(5, 0xa000, 0);
        Map_unk244_2(16, 6);
        Map_unk41_2((s32)gVal6);
        Map_unk247_2(30);
        Map_unk270_2(5, 4);
        Map_unk27_4(0x2005, 6);
        Map_unk270_2(1, 3);
        Map_unk244_2(1, 20);
        Map_unk260_2(16, 6, 20);
        Map_unk37_3(16, 0x104, 20);
        Map_unk244_2(16, 30);
        Map_unk253_2(0, 3);
        Map_unk253_2(1, 3);
        Map_unk270_2(5, 3);
        Map_unk247_2(20);
        Map_unk270_2(16, 3);
        Map_unk244_2(16, 6);
        Map_unk34_3(1, 0x10000, 0x8000);
        Map_unk34_3(5, 0x10000, 0x8000);
        Map_unk34_3(16, 0x20000, 0x10000);
        Map_unk253_2(16, 2);
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk231_2(16, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_unk279_2(16);
        Map_unk256_2(16, 0, 0);
        Map_unk253_2(1, 2);
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk231_2(1, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_unk279_2(1);
        Map_unk256_2(1, 0, 0);
        Map_unk253_2(5, 2);
        rec = (u8 *)Map_unk40(0);
        if (rec != 0) {
            Map_unk231_2(5, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_unk279_2(5);
        Map_unk256_2(5, 0, 0);
        Map_unk42_2(0x144);
        Map_unk42_2(0x809);
        Map_unk283_2();
    }
}

/* map/locations/sol_shinden/inner_sanctum_scene/scene_state.c */
/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

/* Declared old-style: this parameter list is not established here. */

/* Declared old-style: this parameter list is not established here. */

void ClearSolShindenBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}

void SetStatueLightGroup1(void)
{
    if (Map_unk62(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk38_5(0x2d, 28, 0x22, 10, a, b);
    }
    if (Map_unk63(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk39_5(0x2f, 28, 0x24, 10, a, b);
    }
    if (Map_unk64(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk40_5(0x2d, 29, 0x22, 11, a, b);
    }
    if (Map_unk65(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk41_5(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (Map_unk66(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk42_5(0x2d, 28, 0x22, 10, a, b);
    }
    if (Map_unk67(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk43_5(0x2f, 28, 0x24, 10, a, b);
    }
    if (Map_unk68(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk44_5(0x2d, 29, 0x22, 11, a, b);
    }
    if (Map_unk69(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk45_5(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (Map_unk70(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk46_5(0x2d, 30, 0x22, 10, a, b);
    }
    if (Map_unk71(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk47_5(0x2f, 30, 0x24, 10, a, b);
    }
    if (Map_unk72(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk48_5(0x2d, 31, 0x22, 11, a, b);
    }
    if (Map_unk73(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk49_5(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (Map_unk74(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk50_5(0x2d, 30, 0x22, 10, a, b);
    }
    if (Map_unk75(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk51_5(0x2f, 30, 0x24, 10, a, b);
    }
    if (Map_unk76(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk52_5(0x2d, 31, 0x22, 11, a, b);
    }
    if (Map_unk77(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_unk53_5(0x2f, 31, 0x24, 11, a, b);
    }
}

/*
 * Passes the step to one routine with a zero and the wait to another. Both
 * arguments are read before either incoming register is overwritten, so the
 * order of these two calls is what the reference requires. The zero is
 * transcribed, not interpreted.
 */
void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Map_unk285_2(actor_step, 0);
    Map_unk286_2(wait_frames);
}

/* map/locations/sol_shinden/inner_sanctum_scene/sol_shinden.c */
extern u32 gWork;

/* One import, four call sites, four distinct veneer addresses. */

static __inline__ void SetInitialScale(s32 actor, s32 scale, s32 duration)
{
    Map_unk52_3(actor, scale, duration);
}

static __inline__ void SetInitialDirection(s32 actor, s32 direction, s32 duration)
{
    Map_unk53_3(actor, direction, duration);
}

static __inline__ void SetFinalScale(s32 actor, s32 horizontal, s32 vertical)
{
    Map_unk54_3(actor, horizontal, vertical);
}

void StartSolShindenTrapEvent(void)
{
    u32 scene_state;
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Map_unk287_2();
    Map_unk288_2();
    Map_unk289_2(4120);
    SetInitialScale(16, 0x4000, 20);
    SetInitialDirection(16, 256, 0);
    Map_unk290_2(16, 6, 30);
    Map_unk291_2(37617664, -1, 11403264, 1);
    Map_unk292_2();
    Map_unk293_2(30);
    Map_unk294_2(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Map_unk295_2(246);
        Map_unk296_2();
        Map_unk297_2(12);
        Map_unk298_2(246);
        Map_unk299_2();
        Map_unk300_2(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Map_unk301_2(246);
        Map_unk302_2();
        Map_unk303_2(8);
        Map_unk304_2(246);
        Map_unk305_2();
        Map_unk306_2(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Map_unk307_2(246);
        Map_unk308_2();
        Map_unk309_2(6);
        Map_unk310_2(246);
        Map_unk311_2();
        Map_unk312_2(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Map_unk313_2(246);
        Map_unk314_2();
        Map_unk315_2(4);
        Map_unk316_2(246);
        Map_unk317_2();
        Map_unk318_2(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Map_unk319_2(246);
        Map_unk320_2();
        Map_unk321_2(2);
        Map_unk322_2(246);
        Map_unk323_2();
        Map_unk324_2(2);
    }
    Map_unk325_2();
    Map_unk326_2(6);
    Map_unk327_2(32784, 6);
    SetFinalScale(16, 0x20000, 0x10000);
    Map_unk328_2(16, 576, 280);
    scene_state = gWork;
    *(s32 *)(scene_state + 448) = 256;
    *(s32 *)(scene_state + 456) = 32;
    Map_unk329_2();
    Map_unk330_2();
    Map_unk331_2(2067);
    Map_unk332_2(3);
}

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (Map_unk78(0x80b) == 0)
        all_set = 0;
    if (Map_unk79(0x80c) == 0)
        all_set = 0;
    if (Map_unk80(0x80d) == 0)
        all_set = 0;
    if (Map_unk81(0x80e) == 0)
        all_set = 0;

    return all_set;
}
