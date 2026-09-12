#include "types.h"
#include "scene.h"

#include "palette_scene.h"

struct PaletteEffectFrame {
    s32 pad00[6];
    s32 progress;
    s32 pad1c[7];
    s32 limit;
    s32 second_limit;
    s32 third_limit;
};

struct EffectSprite { u8 pad00[9]; u8 flags; u8 pad0a[28]; u8 state; };

struct PaletteEffect {
    u8 pad00[0x18];
    s32 progress;
    u8 pad1c[7];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[24];
    struct EffectSprite *sprite;
    u8 pad54[1];
    u8 mode;
};

struct OrbitingPaletteEffect {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle_x;
    s32 angle_y;
    s32 pad20[6];
    s32 anchor_x;
    s32 anchor_y;
    s32 anchor_z;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

struct PaletteSceneRecord {
    u8 pad00[216];
    u16 values[1];                 /* +216 */
};

extern s32 gOv;
extern s32 gOv2;
extern s32 gOv3[3];
extern const s32 SceneAction_ActorOneEntry[];
extern const s32 SceneAction_ActorTwoEntry[];
extern const s32 SceneAction_ActorThreeEntry[];
extern const s32 SceneAction_GroupFinish[];
extern s32 gIw;
extern u8 gOv4[];
extern u8 gWork[];

u8 *Effect_unk15_4();

struct PaletteEffect *Effect_unk16_4(s32, s32, s32, s32);

struct PaletteSceneRecord *Effect_unk17_4();

u8 *Effect_unk18_4();

/* One symbol per call site, named at the site's decoded address. All three
 * reach the same helper, which scales one channel by the adjustment, and each
 * site still needs its own name. */

/*
 * Returns the in-image table at 0x02009ba4. The eight-byte owner at 0x02000030
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetScriptData(void)
{
    extern s32 gOv5;

    return (u8 *)0x02009ba4;
}

/*
 * Returns the in-image table at 0x02009c04. The eight-byte owner at 0x02000038
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetMessageData(void)
{
    extern s32 gOv5;

    return (u8 *)0x02009c04;
}

/*
 * Returns the in-image table at 0x02009c24. The eight-byte owner at 0x02000040
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetActorData(void)
{
    extern s32 gOv5;

    return (u8 *)0x02009c24;
}

/*
 * Returns the in-image table at 0x02009c34. The eight-byte owner at 0x02000048
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetEffectData(void)
{
    extern s32 gOv5;

    return (u8 *)0x02009c34;
}

void PaletteScene_Initialize(void)
{
    extern s32 gOv5;

    void *scene;

    scene = *(void **)0x03001EBC;
    Effect_unk19_4();
    Effect_unk11_3(0, 0, 0);
    Effect_unk7_2(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Effect_unk20_4();
}

/* Returns this overlay's state block. */
u8 *PaletteScene_GetState(void) { return (u8 *)0x02009d3c; }

void Scene_RunScene395(void)
{
    u32 i;
    s32 record;

    Effect_unk21_4();
    if (Effect_Check(0x845) != 0) {
        Effect_unk22_4(10, 1);
        Effect_Do(0x151c);
        Effect_unk23_4(8, 0);
        Effect_unk24_4(10, 0);
    } else {
        if (Effect_unk2(0x844) != 0) {
            Effect_unk25_3(10, 1);
            Effect_unk2_2(0x14eb);
            Effect_unk26_3(8, 0);
            Effect_unk3(10, 0);
            record = Effect_unk9(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)(*(u8 *volatile *)gWork + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Effect_unk3_2(0x14c9);
            Effect_unk27_3(8, 0);
            Effect_Run(0x406218, 1);
            Effect_unk28_3(20);
            Effect_unk29_3(40);
            Effect_Place(0x200e, 0, 10);
            Effect_unk30_2(0, 2);
            Effect_unk2_4(0x200e, 0);
            Effect_unk3_4(0x10000, 1);
            Effect_unk31_2(20);
            Effect_unk32_2(40);
        }
    }
    L_02000220:;
    Effect_unk33_2();
}

void PaletteScene_RunActorNineBranch(void)
{
    extern s32 gOv5;

    Effect_unk34_2();
    if (Effect_unk10(0x845) != 0) {
        Effect_unk8_2(0x151F);
    } else {
        Effect_unk9_2(0x14C8);
    }
    Effect_Apply(9, 0);
    Effect_unk35_2();
}

void PaletteScene_RunActorEightBranch(void)
{
    extern s32 gOv5;

    Effect_unk36_2();
    if (Effect_unk11(0x845) != 0) {
        Effect_unk10_2(0x151E);
    } else {
        Effect_unk11_2(0x14EC);
    }
    Effect_Apply2(8, 0);
    Effect_unk37_2();
}

void PaletteScene_RunFlaggedBranch(void)
{
    extern s32 gOv5;

    Effect_unk38_2();
    Effect_unk39_2();
    if (Effect_unk12(0x844) == 0) {
        Effect_unk40_2();
    } else {
        Effect_unk41_2();
    }
    Effect_unk42();
}

void RunEventScript01(void)
{
    extern u8 gOv5[];

    u32 i;
    s32 rec8;
    u8 *record;
    s32 base5_2009dd0;

    rec8 = Effect_unk13(0);
    Effect_unk2_3(0, 0xc000, 0);
    Effect_unk4_4(0x406218, 1);
    Effect_unk43(20);
    Effect_unk44(40);
    Effect_unk45(17);
    base5_2009dd0 = (s32)gOv5;
    *(volatile s32 *)base5_2009dd0 = 1;
    Effect_unk5_4(0x2009219, 0xc80);
    Effect_unk46(30);
    *(volatile s32 *)base5_2009dd0 = 0;
    Effect_unk6_4(0x1480000, -1, 0xeb0000, 1);
    Effect_unk47(0, 1);
    *(u8 *)(Effect_unk14(0) + 90) &= 254;
    Effect_unk48(0, 16);
    Effect_unk3_3(0, 0x20000, 0x20000);
    Effect_unk49(133);
    *(volatile s32 *)(rec8 + 40) = 0x50000;
    *(volatile s32 *)(rec8 + 72) = 0x4000;
    *(volatile s32 *)(rec8 + 68) = 0xa000;
    Effect_unk4_3(0, 0x14f, 0x102);
    if (*(volatile s32 *)(rec8 + 40) >= 0) {
        do {
            Effect_unk50(1);
        } while (*(volatile s32 *)(rec8 + 40) >= 0);
    }
    do {
        Effect_unk51(1);
    } while (*(volatile s32 *)(rec8 + 40) <= 0);
    Effect_unk52(161);
    Effect_unk53(0, 19);
    Effect_unk54(120);
    Effect_unk4_2(0x2009219);
    Effect_unk55(40);
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Effect_unk18_4(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Effect_unk56(80);
    Effect_unk5_2(0x14cc);
    Effect_unk5_3(0x200e, 0, 20);
    Effect_unk57(0, 2);
    Effect_unk58(20);
    Effect_unk7_4(0x200e, 0);
    Effect_unk59();
    Effect_unk8_4(0x10000, 1);
    Effect_unk60(20);
    Effect_unk61(40);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(rec8 + 6) = shown;
    }
    *(volatile s32 *)(rec8 + 72) = 0x10000;
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    Effect_unk62(0, 2);
    Effect_unk63(40);
    Effect_unk64(0, 4, 0);
    Effect_unk65(0, 1);
    Effect_unk66(20);
}

/* The scene's actor transition sequence. Actor three takes part only when the
 * saved flag reports it enabled, and the arms that skip it bump the step
 * counter instead. */
void PaletteScene_RunActorTransitionSequence(void)
{
    extern s32 gOv5;

    s32 actorThreeEnabled;
    u8 *object;
    s32 *transitionState;
    s32 cycle;
    s32 sceneWorkSlot;
    s32 effectCallback;
    const s32 *finalActions;

    actorThreeEnabled = Effect_unk15(3);
    Effect_unk6_3(0, 0x148, 212);
    Effect_unk7_3(0, 0xc000, 20);
    Effect_unk67(17);
    Effect_unk9_4(0x14ed, 1);
    Effect_unk8_3(1, 0x10000, 0x8000);
    Effect_unk8_3(2, 0x10000, 0x8000);
    object = Effect_unk15_4(0);
    if (object != 0) {
        Effect_unk68(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Effect_unk15_4(0);
    if (object != 0) {
        Effect_unk68(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Effect_unk69(1, SceneAction_ActorOneEntry);
    Effect_unk69(2, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Effect_unk8_3(3, 0x10000, 0x8000);
        object = Effect_unk15_4(0);
        if (object != 0) {
            Effect_unk68(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Effect_unk69(3, SceneAction_ActorThreeEntry);
    }
    Effect_unk70(2);
    Effect_unk71(40);
    Effect_unk72(0);
    Effect_unk73(32);
    Effect_unk74(40);
    transitionState = &gOv;
    *transitionState = 0;
    Effect_unk4((s32)PaletteScene_AdvanceTransition, 0xc80);
    Effect_unk71(40);
    Effect_unk7_3(1, 0x6000, 20);
    Effect_unk10_4(0x33333, 0x6666);
    Effect_unk11_4(0x1000000, -1, 0xfe0000, 1);
    Effect_unk75();
    Effect_unk67(246);
    Effect_unk71(40);
    Effect_unk7_3(2, 0x2000, 20);
    Effect_unk11_4(0x19d0000, -1, 0x1050000, 1);
    Effect_unk75();
    Effect_unk67(246);
    Effect_unk71(40);
    Effect_unk7_3(0, 0x4000, 0);
    Effect_unk7_3(3, 0x4000, 20);
    Effect_unk11_4(0x1460000, -1, 0x1800000, 1);
    Effect_unk75();
    Effect_unk67(246);
    if (*transitionState != 24) {
        do {
            Effect_unk74(1);
        } while (*transitionState != 24);
    }
    Effect_unk5((s32)PaletteScene_AdvanceTransition);
    Effect_unk74(10);
    cycle = 0;
    do {
        Effect_unk72(0);
        Effect_unk73(6);
        Effect_unk74(6);
        Effect_unk72(1);
        Effect_unk73(6);
        cycle = (cycle + 1);
        Effect_unk74(6);
    } while ((u32)cycle <= 3);
    Effect_unk72(0);
    Effect_unk73(40);
    Effect_unk74(80);
    Effect_unk11_4(0x1480000, 0x80000, 0xd40000, 1);
    Effect_unk75();
    Effect_unk71(40);
    Effect_unk76(10, 1);
    Effect_unk71(40);
    Effect_unk67(7);
    Effect_unk6_2(0x14ee);
    Effect_unk77(8, 0);
    Effect_unk78(0, 2);
    Effect_unk78(1, 2);
    Effect_unk78(3, 2);
    Effect_unk79(2, 2);
    Effect_unk7_3(0, 0xc000, 0);
    Effect_unk7_3(1, 0xc000, 0);
    Effect_unk7_3(3, 0xc000, 0);
    Effect_unk7_3(2, 0xc000, 20);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk76(10, 3);
    Effect_unk71(40);
    Effect_unk76(10, 1);
    Effect_unk71(20);
    Effect_unk77(8, 0);
    Effect_unk9_3(0, 0x105, 0);
    Effect_unk9_3(1, 0x105, 0);
    Effect_unk9_3(3, 0x105, 0);
    Effect_unk9_3(2, 0x105, 40);
    Effect_unk11_4(0xea0000, 0, 0xe80000, 1);
    Effect_unk75();
    Effect_unk71(40);
    Effect_unk76(11, 1);
    Effect_unk71(40);
    Effect_unk76(11, 3);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 20);
    Effect_unk76(11, 2);
    Effect_unk71(10);
    Effect_unk7_3(0, 0x6000, 0);
    Effect_unk7_3(1, 0x6000, 0);
    Effect_unk7_3(2, 0x6000, 0);
    Effect_unk7_3(3, 0x6000, 20);
    Effect_unk76(11, 3);
    Effect_unk71(20);
    Effect_unk76(11, 2);
    Effect_unk71(20);
    Effect_unk76(11, 3);
    Effect_unk10_3(0x4009, 0, 10);
    Effect_unk76(10, 0);
    Effect_unk71(20);
    Effect_unk12_4(0x8008, 0);
    Effect_unk76(10, 1);
    Effect_unk71(20);
    Effect_unk6(0x8008, 0);
    Effect_unk7_3(0, 0xc000, 0);
    Effect_unk7_3(1, 0xe000, 0);
    Effect_unk7_3(2, 0xa000, 0);
    Effect_unk7_3(3, 0xc000, 0);
    if (Effect_unk7(0, 0) == 0) {
        Effect_unk12_4(0x4009, 0);
        Effect_unk12_4(0x8008, 0);
    } else {
        sceneWorkSlot = 0x3001ebc;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Effect_unk9_3(3, 0x103, 0);
        Effect_unk9_3(1, 0x103, 0);
        Effect_unk9_3(2, 0x103, 40);
        Effect_unk80(1, 4);
        Effect_unk77(1, 0);
        if (actorThreeEnabled != 0) {
            Effect_unk79(3, 2);
            Effect_unk77(3, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Effect_unk81(2, 3);
        Effect_unk77(2, 0);
        Effect_unk12_4(0x4009, 0);
        Effect_unk12_4(0x8008, 0);
    }
    Effect_unk80(0, 3);
    Effect_unk80(1, 3);
    Effect_unk80(3, 3);
    Effect_unk81(2, 3);
    Effect_unk11_4(0x1480000, 0x80000, 0xd40000, 1);
    Effect_unk75();
    Effect_unk71(20);
    Effect_unk76(10, 0);
    Effect_unk71(20);
    Effect_unk72(0);
    Effect_unk73(1);
    Effect_unk74(1);
    Effect_unk13_4(0x406218, 1);
    Effect_unk73(40);
    Effect_unk71(60);
    gOv2 = 0;
    gOv3[0] = 0x1480000;
    gOv3[1] = 0x300000;
    effectCallback = (s32)Effect_unk82;
    gOv3[2] = 0xcd0000;
    Effect_unk4(effectCallback, 0xc80);
    Effect_unk71(100);
    Effect_unk16(effectCallback);
    Effect_unk13_4(0x7fff, 0);
    Effect_unk73(60);
    Effect_unk71(100);
    Effect_unk72(0);
    Effect_unk73(20);
    Effect_unk71(40);
    Effect_unk76(10, 1);
    Effect_unk71(10);
    Effect_unk6_2(0x14fb);
    Effect_unk12_4(0x8008, 0);
    Effect_unk80(0, 3);
    Effect_unk80(1, 3);
    Effect_unk80(3, 3);
    Effect_unk81(2, 3);
    Effect_unk11_4(0xea0000, 0, 0xe80000, 1);
    Effect_unk75();
    Effect_unk71(20);
    Effect_unk12_4(0x4009, 0);
    Effect_unk10_3(0x8008, 0, 10);
    Effect_unk79(1, 2);
    Effect_unk7_3(0, 0x6000, 0);
    Effect_unk7_3(1, 0xe000, 10);
    Effect_unk6(1, 0);
    if (Effect_unk7(0, 0) == 0) {
        Effect_unk9_3(1, 0x102, 40);
    } else {
        Effect_unk81(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_unk77(1, 0);
    Effect_unk76(10, 4);
    Effect_unk71(20);
    Effect_unk6_2(0x1501);
    Effect_unk12_4(0x8008, 0);
    Effect_unk7_3(0, 0xc000, 0);
    Effect_unk7_3(1, 0xc000, 0);
    Effect_unk12_4(0x8008, 0);
    Effect_unk80(0, 3);
    Effect_unk80(1, 3);
    Effect_unk80(3, 3);
    Effect_unk81(2, 3);
    Effect_unk76(10, 4);
    Effect_unk71(20);
    Effect_unk10_3(0x8008, 0, 20);
    Effect_unk76(11, 0);
    Effect_unk10_3(0x4009, 0, 20);
    Effect_unk76(11, 3);
    Effect_unk71(40);
    Effect_unk76(11, 1);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 20);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk12_4(0x8008, 0);
    Effect_unk9_3(0, 0x102, 0);
    Effect_unk9_3(1, 0x102, 0);
    Effect_unk9_3(3, 0x102, 0);
    Effect_unk9_3(2, 0x102, 80);
    Effect_unk76(11, 5);
    Effect_unk71(60);
    Effect_unk76(11, 3);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 20);
    Effect_unk76(10, 5);
    Effect_unk71(40);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk10_3(0x4008, 0, 20);
    Effect_unk79(1, 2);
    Effect_unk7_3(1, 0x8000, 10);
    Effect_unk77(1, 0);
    Effect_unk7_3(2, 0x8000, 20);
    Effect_unk12_4(0x8002, 0);
    Effect_unk76(11, 4);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 20);
    Effect_unk7_3(1, 0xe000, 0);
    Effect_unk7_3(0, 0x6000, 10);
    Effect_unk83(1, 0, 20);
    Effect_unk76(10, 1);
    Effect_unk10_3(0x8008, 0, 10);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk76(11, 3);
    Effect_unk71(40);
    Effect_unk76(11, 0);
    Effect_unk71(20);
    Effect_unk72(0);
    Effect_unk73(1);
    Effect_unk74(1);
    Effect_unk13_4(0x406218, 1);
    Effect_unk73(40);
    Effect_unk71(60);
    gOv2 = 0;
    gOv3[0] = 0x880000;
    gOv3[1] = 0x140000;
    effectCallback = (s32)Effect_unk82;
    gOv3[2] = 0x1020000;
    Effect_unk4(effectCallback, 0xc80);
    Effect_unk71(100);
    Effect_unk7_3(0, 0x6000, 0);
    Effect_unk7_3(1, 0x6000, 0);
    Effect_unk7_3(3, 0x6000, 0);
    Effect_unk7_3(2, 0x6000, 40);
    Effect_unk78(2, 1);
    Effect_unk9_3(2, 0x100, 20);
    Effect_unk10_3(0x8002, 0, 10);
    Effect_unk79(0, 2);
    Effect_unk7_3(0, 0x2000, 10);
    Effect_unk81(0, 3);
    Effect_unk76(10, 4);
    Effect_unk71(20);
    Effect_unk12_4(0x8008, 0);
    Effect_unk9_3(2, 0x101, 60);
    Effect_unk7_3(2, 0xc000, 10);
    Effect_unk10_3(0x8002, 0, 10);
    Effect_unk7_3(0, 0xc000, 0);
    Effect_unk7_3(3, 0xc000, 0);
    Effect_unk7_3(1, 0xc000, 20);
    Effect_unk10_3(0x8008, 0, 10);
    Effect_unk80(0, 3);
    Effect_unk80(1, 3);
    Effect_unk80(3, 3);
    Effect_unk81(2, 3);
    Effect_unk71(10);
    Effect_unk7_3(0, 0x6000, 0);
    Effect_unk7_3(1, 0x6000, 0);
    Effect_unk7_3(3, 0x6000, 0);
    Effect_unk7_3(2, 0x6000, 120);
    Effect_unk16(effectCallback);
    Effect_unk71(60);
    Effect_unk72(0);
    Effect_unk73(40);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk10_3(0x8008, 0, 20);
    Effect_unk76(11, 3);
    Effect_unk12_4(0x4009, 0);
    Effect_unk12_4(0x8008, 0);
    Effect_unk76(11, 4);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 10);
    Effect_unk78(0, 2);
    Effect_unk78(1, 2);
    Effect_unk78(3, 2);
    Effect_unk79(2, 2);
    Effect_unk76(10, 1);
    Effect_unk6(0x8008, 0);
    Effect_unk7_3(0, 0xc000, 0);
    Effect_unk7_3(1, 0xe000, 0);
    Effect_unk7_3(3, 0xc000, 0);
    Effect_unk7_3(2, 0xa000, 0);
    if (Effect_unk7(0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_unk71(10);
    Effect_unk76(10, 2);
    Effect_unk71(20);
    Effect_unk76(11, 3);
    Effect_unk71(40);
    Effect_unk76(10, 1);
    Effect_unk71(20);
    Effect_unk10_3(0x8008, 0, 10);
    Effect_unk7_3(0, 0x4000, 0);
    Effect_unk84(1, 0, 0);
    Effect_unk7_3(3, 0xc000, 0);
    Effect_unk7_3(2, 0x8000, 10);
    Effect_unk80(0, 3);
    Effect_unk80(1, 3);
    Effect_unk80(3, 3);
    Effect_unk81(2, 3);
    Effect_unk67(17);
    finalActions = SceneAction_GroupFinish;
    Effect_unk69(1, finalActions);
    if (actorThreeEnabled != 0) {
        Effect_unk69(3, finalActions);
    }
    Effect_unk14_4(2, (s32)finalActions);
    Effect_unk76(10, 4);
    Effect_unk76(10, 4);
    Effect_unk71(20);
    Effect_unk6_2(0x1519);
    Effect_unk12_4(0x8008, 0);
    Effect_unk76(11, 4);
    Effect_unk76(11, 4);
    Effect_unk71(20);
    Effect_unk10_3(0x4009, 0, 10);
    Effect_unk81(0, 3);
    Effect_unk8(0x845);
    Effect_unk67(1);
    PaletteScene_SetRecordValue(184, 185);
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    extern s32 gOv5;

    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Effect_unk12_2(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{
    extern s32 gOv5;

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = gIw & 3;
    if (phase != 0) return;
    if (gOv5 != 0) Effect_unk13_2(200);
    effect = Effect_unk16_4(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Effect_Apply3(effect, 2);
    Effect_SetMode(effect, target_x, 0, target_z);
    Effect_Apply4(effect, gOv4);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    extern s32 gOv5;

    s32 step = gOv;

    if (step == 0) {
        Effect_unk14_2(0);
        Effect_unk15_2(20);
    } else if (step == 20) {
        Effect_unk16_2(1);
        Effect_unk17_2(8);
    }
    step = gOv + 1;
    gOv = step;
    if (step == 30) {
        gOv = 0;
    }
}

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    extern s32 gOv5;

    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Effect_unk85(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Effect_unk86(effect->owner[0x1c]);
        Effect_unk87(effect);
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    extern s32 gOv5;

    s32 slot = Effect_unk17(key);

    if (slot != -1) {
        s32 index = Effect_unk18(slot, key);

        if (index != -1) {
            Effect_unk17_4(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    extern s32 gOv5;

    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    Effect_unk88();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = Effect_Apply5(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    Effect_unk89(); Effect_unk90(); Effect_unk91(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    extern s32 gOv5;

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Effect_unk19(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Effect_unk20(green, adjustment));
    blue = (s16)(blue - Effect_unk21(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
