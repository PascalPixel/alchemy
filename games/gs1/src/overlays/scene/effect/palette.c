#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/effect/palette.h"


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

u8 *Effect_Run15();

struct PaletteEffect *Effect_Run16(s32, s32, s32, s32);

struct PaletteSceneRecord *Effect_Run17();

u8 *Effect_Run18();

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
    Effect_Run19();
    Effect_Place11(0, 0, 0);
    Effect_Do7(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Effect_Run20();
}

/* Returns this overlay's state block. */
u8 *PaletteScene_GetState(void) { return (u8 *)0x02009d3c; }

void Scene_RunScene395(void)
{
    u32 i;
    s32 record;

    Effect_Run21();
    if (Effect_Check(0x845) != 0) {
        Effect_Run22(10, 1);
        Effect_Do(0x151c);
        Effect_Run23(8, 0);
        Effect_Run24(10, 0);
    } else {
        if (Effect_Check2(0x844) != 0) {
            Effect_Run25(10, 1);
            Effect_Do2(0x14eb);
            Effect_Run26(8, 0);
            Effect_Check3(10, 0);
            record = Effect_Check9(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)(*(u8 *volatile *)gWork + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Effect_Do3(0x14c9);
            Effect_Run27(8, 0);
            Effect_Run(0x406218, 1);
            Effect_Run28(20);
            Effect_Run29(40);
            Effect_Place(0x200e, 0, 10);
            Effect_Run30(0, 2);
            Effect_Run2(0x200e, 0);
            Effect_Run3(0x10000, 1);
            Effect_Run31(20);
            Effect_Run32(40);
        }
    }
    L_02000220:;
    Effect_Run33();
}

void PaletteScene_RunActorNineBranch(void)
{
    extern s32 gOv5;

    Effect_Run34();
    if (Effect_Check10(0x845) != 0) {
        Effect_Do8(0x151F);
    } else {
        Effect_Do9(0x14C8);
    }
    Effect_Apply(9, 0);
    Effect_Run35();
}

void PaletteScene_RunActorEightBranch(void)
{
    extern s32 gOv5;

    Effect_Run36();
    if (Effect_Check11(0x845) != 0) {
        Effect_Do10(0x151E);
    } else {
        Effect_Do11(0x14EC);
    }
    Effect_Apply2(8, 0);
    Effect_Run37();
}

void PaletteScene_RunFlaggedBranch(void)
{
    extern s32 gOv5;

    Effect_Run38();
    Effect_Run39();
    if (Effect_Check12(0x844) == 0) {
        Effect_Run40();
    } else {
        Effect_Run41();
    }
    Effect_Run42();
}

void RunEventScript01(void)
{
    extern u8 gOv5[];

    u32 i;
    s32 rec8;
    u8 *record;
    s32 base5_2009dd0;

    rec8 = Effect_Check13(0);
    Effect_Place2(0, 0xc000, 0);
    Effect_Run4(0x406218, 1);
    Effect_Run43(20);
    Effect_Run44(40);
    Effect_Run45(17);
    base5_2009dd0 = (s32)gOv5;
    *(volatile s32 *)base5_2009dd0 = 1;
    Effect_Run5(0x2009219, 0xc80);
    Effect_Run46(30);
    *(volatile s32 *)base5_2009dd0 = 0;
    Effect_Run6(0x1480000, -1, 0xeb0000, 1);
    Effect_Run47(0, 1);
    *(u8 *)(Effect_Check14(0) + 90) &= 254;
    Effect_Run48(0, 16);
    Effect_Place3(0, 0x20000, 0x20000);
    Effect_Run49(133);
    *(volatile s32 *)(rec8 + 40) = 0x50000;
    *(volatile s32 *)(rec8 + 72) = 0x4000;
    *(volatile s32 *)(rec8 + 68) = 0xa000;
    Effect_Place4(0, 0x14f, 0x102);
    if (*(volatile s32 *)(rec8 + 40) >= 0) {
        do {
            Effect_Run50(1);
        } while (*(volatile s32 *)(rec8 + 40) >= 0);
    }
    do {
        Effect_Run51(1);
    } while (*(volatile s32 *)(rec8 + 40) <= 0);
    Effect_Run52(161);
    Effect_Run53(0, 19);
    Effect_Run54(120);
    Effect_Do4(0x2009219);
    Effect_Run55(40);
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Effect_Run18(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Effect_Run56(80);
    Effect_Do5(0x14cc);
    Effect_Place5(0x200e, 0, 20);
    Effect_Run57(0, 2);
    Effect_Run58(20);
    Effect_Run7(0x200e, 0);
    Effect_Run59();
    Effect_Run8(0x10000, 1);
    Effect_Run60(20);
    Effect_Run61(40);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(rec8 + 6) = shown;
    }
    *(volatile s32 *)(rec8 + 72) = 0x10000;
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    Effect_Run62(0, 2);
    Effect_Run63(40);
    Effect_Run64(0, 4, 0);
    Effect_Run65(0, 1);
    Effect_Run66(20);
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

    actorThreeEnabled = Effect_Check15(3);
    Effect_Place6(0, 0x148, 212);
    Effect_Place7(0, 0xc000, 20);
    Effect_Run67(17);
    Effect_Run9(0x14ed, 1);
    Effect_Place8(1, 0x10000, 0x8000);
    Effect_Place8(2, 0x10000, 0x8000);
    object = Effect_Run15(0);
    if (object != 0) {
        Effect_Run68(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Effect_Run15(0);
    if (object != 0) {
        Effect_Run68(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Effect_Run69(1, SceneAction_ActorOneEntry);
    Effect_Run69(2, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Effect_Place8(3, 0x10000, 0x8000);
        object = Effect_Run15(0);
        if (object != 0) {
            Effect_Run68(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Effect_Run69(3, SceneAction_ActorThreeEntry);
    }
    Effect_Run70(2);
    Effect_Run71(40);
    Effect_Run72(0);
    Effect_Run73(32);
    Effect_Run74(40);
    transitionState = &gOv;
    *transitionState = 0;
    Effect_Check4((s32)PaletteScene_AdvanceTransition, 0xc80);
    Effect_Run71(40);
    Effect_Place7(1, 0x6000, 20);
    Effect_Run10(0x33333, 0x6666);
    Effect_Run11(0x1000000, -1, 0xfe0000, 1);
    Effect_Run75();
    Effect_Run67(246);
    Effect_Run71(40);
    Effect_Place7(2, 0x2000, 20);
    Effect_Run11(0x19d0000, -1, 0x1050000, 1);
    Effect_Run75();
    Effect_Run67(246);
    Effect_Run71(40);
    Effect_Place7(0, 0x4000, 0);
    Effect_Place7(3, 0x4000, 20);
    Effect_Run11(0x1460000, -1, 0x1800000, 1);
    Effect_Run75();
    Effect_Run67(246);
    if (*transitionState != 24) {
        do {
            Effect_Run74(1);
        } while (*transitionState != 24);
    }
    Effect_Check5((s32)PaletteScene_AdvanceTransition);
    Effect_Run74(10);
    cycle = 0;
    do {
        Effect_Run72(0);
        Effect_Run73(6);
        Effect_Run74(6);
        Effect_Run72(1);
        Effect_Run73(6);
        cycle = (cycle + 1);
        Effect_Run74(6);
    } while ((u32)cycle <= 3);
    Effect_Run72(0);
    Effect_Run73(40);
    Effect_Run74(80);
    Effect_Run11(0x1480000, 0x80000, 0xd40000, 1);
    Effect_Run75();
    Effect_Run71(40);
    Effect_Run76(10, 1);
    Effect_Run71(40);
    Effect_Run67(7);
    Effect_Do6(0x14ee);
    Effect_Run77(8, 0);
    Effect_Run78(0, 2);
    Effect_Run78(1, 2);
    Effect_Run78(3, 2);
    Effect_Run79(2, 2);
    Effect_Place7(0, 0xc000, 0);
    Effect_Place7(1, 0xc000, 0);
    Effect_Place7(3, 0xc000, 0);
    Effect_Place7(2, 0xc000, 20);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Run76(10, 3);
    Effect_Run71(40);
    Effect_Run76(10, 1);
    Effect_Run71(20);
    Effect_Run77(8, 0);
    Effect_Place9(0, 0x105, 0);
    Effect_Place9(1, 0x105, 0);
    Effect_Place9(3, 0x105, 0);
    Effect_Place9(2, 0x105, 40);
    Effect_Run11(0xea0000, 0, 0xe80000, 1);
    Effect_Run75();
    Effect_Run71(40);
    Effect_Run76(11, 1);
    Effect_Run71(40);
    Effect_Run76(11, 3);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 20);
    Effect_Run76(11, 2);
    Effect_Run71(10);
    Effect_Place7(0, 0x6000, 0);
    Effect_Place7(1, 0x6000, 0);
    Effect_Place7(2, 0x6000, 0);
    Effect_Place7(3, 0x6000, 20);
    Effect_Run76(11, 3);
    Effect_Run71(20);
    Effect_Run76(11, 2);
    Effect_Run71(20);
    Effect_Run76(11, 3);
    Effect_Place10(0x4009, 0, 10);
    Effect_Run76(10, 0);
    Effect_Run71(20);
    Effect_Run12(0x8008, 0);
    Effect_Run76(10, 1);
    Effect_Run71(20);
    Effect_Check6(0x8008, 0);
    Effect_Place7(0, 0xc000, 0);
    Effect_Place7(1, 0xe000, 0);
    Effect_Place7(2, 0xa000, 0);
    Effect_Place7(3, 0xc000, 0);
    if (Effect_Check7(0, 0) == 0) {
        Effect_Run12(0x4009, 0);
        Effect_Run12(0x8008, 0);
    } else {
        sceneWorkSlot = 0x3001ebc;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Effect_Place9(3, 0x103, 0);
        Effect_Place9(1, 0x103, 0);
        Effect_Place9(2, 0x103, 40);
        Effect_Run80(1, 4);
        Effect_Run77(1, 0);
        if (actorThreeEnabled != 0) {
            Effect_Run79(3, 2);
            Effect_Run77(3, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Effect_Run81(2, 3);
        Effect_Run77(2, 0);
        Effect_Run12(0x4009, 0);
        Effect_Run12(0x8008, 0);
    }
    Effect_Run80(0, 3);
    Effect_Run80(1, 3);
    Effect_Run80(3, 3);
    Effect_Run81(2, 3);
    Effect_Run11(0x1480000, 0x80000, 0xd40000, 1);
    Effect_Run75();
    Effect_Run71(20);
    Effect_Run76(10, 0);
    Effect_Run71(20);
    Effect_Run72(0);
    Effect_Run73(1);
    Effect_Run74(1);
    Effect_Run13(0x406218, 1);
    Effect_Run73(40);
    Effect_Run71(60);
    gOv2 = 0;
    gOv3[0] = 0x1480000;
    gOv3[1] = 0x300000;
    effectCallback = (s32)Effect_Run82;
    gOv3[2] = 0xcd0000;
    Effect_Check4(effectCallback, 0xc80);
    Effect_Run71(100);
    Effect_Check16(effectCallback);
    Effect_Run13(0x7fff, 0);
    Effect_Run73(60);
    Effect_Run71(100);
    Effect_Run72(0);
    Effect_Run73(20);
    Effect_Run71(40);
    Effect_Run76(10, 1);
    Effect_Run71(10);
    Effect_Do6(0x14fb);
    Effect_Run12(0x8008, 0);
    Effect_Run80(0, 3);
    Effect_Run80(1, 3);
    Effect_Run80(3, 3);
    Effect_Run81(2, 3);
    Effect_Run11(0xea0000, 0, 0xe80000, 1);
    Effect_Run75();
    Effect_Run71(20);
    Effect_Run12(0x4009, 0);
    Effect_Place10(0x8008, 0, 10);
    Effect_Run79(1, 2);
    Effect_Place7(0, 0x6000, 0);
    Effect_Place7(1, 0xe000, 10);
    Effect_Check6(1, 0);
    if (Effect_Check7(0, 0) == 0) {
        Effect_Place9(1, 0x102, 40);
    } else {
        Effect_Run81(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_Run77(1, 0);
    Effect_Run76(10, 4);
    Effect_Run71(20);
    Effect_Do6(0x1501);
    Effect_Run12(0x8008, 0);
    Effect_Place7(0, 0xc000, 0);
    Effect_Place7(1, 0xc000, 0);
    Effect_Run12(0x8008, 0);
    Effect_Run80(0, 3);
    Effect_Run80(1, 3);
    Effect_Run80(3, 3);
    Effect_Run81(2, 3);
    Effect_Run76(10, 4);
    Effect_Run71(20);
    Effect_Place10(0x8008, 0, 20);
    Effect_Run76(11, 0);
    Effect_Place10(0x4009, 0, 20);
    Effect_Run76(11, 3);
    Effect_Run71(40);
    Effect_Run76(11, 1);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 20);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Run12(0x8008, 0);
    Effect_Place9(0, 0x102, 0);
    Effect_Place9(1, 0x102, 0);
    Effect_Place9(3, 0x102, 0);
    Effect_Place9(2, 0x102, 80);
    Effect_Run76(11, 5);
    Effect_Run71(60);
    Effect_Run76(11, 3);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 20);
    Effect_Run76(10, 5);
    Effect_Run71(40);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Place10(0x4008, 0, 20);
    Effect_Run79(1, 2);
    Effect_Place7(1, 0x8000, 10);
    Effect_Run77(1, 0);
    Effect_Place7(2, 0x8000, 20);
    Effect_Run12(0x8002, 0);
    Effect_Run76(11, 4);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 20);
    Effect_Place7(1, 0xe000, 0);
    Effect_Place7(0, 0x6000, 10);
    Effect_Run83(1, 0, 20);
    Effect_Run76(10, 1);
    Effect_Place10(0x8008, 0, 10);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Run76(11, 3);
    Effect_Run71(40);
    Effect_Run76(11, 0);
    Effect_Run71(20);
    Effect_Run72(0);
    Effect_Run73(1);
    Effect_Run74(1);
    Effect_Run13(0x406218, 1);
    Effect_Run73(40);
    Effect_Run71(60);
    gOv2 = 0;
    gOv3[0] = 0x880000;
    gOv3[1] = 0x140000;
    effectCallback = (s32)Effect_Run82;
    gOv3[2] = 0x1020000;
    Effect_Check4(effectCallback, 0xc80);
    Effect_Run71(100);
    Effect_Place7(0, 0x6000, 0);
    Effect_Place7(1, 0x6000, 0);
    Effect_Place7(3, 0x6000, 0);
    Effect_Place7(2, 0x6000, 40);
    Effect_Run78(2, 1);
    Effect_Place9(2, 0x100, 20);
    Effect_Place10(0x8002, 0, 10);
    Effect_Run79(0, 2);
    Effect_Place7(0, 0x2000, 10);
    Effect_Run81(0, 3);
    Effect_Run76(10, 4);
    Effect_Run71(20);
    Effect_Run12(0x8008, 0);
    Effect_Place9(2, 0x101, 60);
    Effect_Place7(2, 0xc000, 10);
    Effect_Place10(0x8002, 0, 10);
    Effect_Place7(0, 0xc000, 0);
    Effect_Place7(3, 0xc000, 0);
    Effect_Place7(1, 0xc000, 20);
    Effect_Place10(0x8008, 0, 10);
    Effect_Run80(0, 3);
    Effect_Run80(1, 3);
    Effect_Run80(3, 3);
    Effect_Run81(2, 3);
    Effect_Run71(10);
    Effect_Place7(0, 0x6000, 0);
    Effect_Place7(1, 0x6000, 0);
    Effect_Place7(3, 0x6000, 0);
    Effect_Place7(2, 0x6000, 120);
    Effect_Check16(effectCallback);
    Effect_Run71(60);
    Effect_Run72(0);
    Effect_Run73(40);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Place10(0x8008, 0, 20);
    Effect_Run76(11, 3);
    Effect_Run12(0x4009, 0);
    Effect_Run12(0x8008, 0);
    Effect_Run76(11, 4);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 10);
    Effect_Run78(0, 2);
    Effect_Run78(1, 2);
    Effect_Run78(3, 2);
    Effect_Run79(2, 2);
    Effect_Run76(10, 1);
    Effect_Check6(0x8008, 0);
    Effect_Place7(0, 0xc000, 0);
    Effect_Place7(1, 0xe000, 0);
    Effect_Place7(3, 0xc000, 0);
    Effect_Place7(2, 0xa000, 0);
    if (Effect_Check7(0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Effect_Run71(10);
    Effect_Run76(10, 2);
    Effect_Run71(20);
    Effect_Run76(11, 3);
    Effect_Run71(40);
    Effect_Run76(10, 1);
    Effect_Run71(20);
    Effect_Place10(0x8008, 0, 10);
    Effect_Place7(0, 0x4000, 0);
    Effect_Run84(1, 0, 0);
    Effect_Place7(3, 0xc000, 0);
    Effect_Place7(2, 0x8000, 10);
    Effect_Run80(0, 3);
    Effect_Run80(1, 3);
    Effect_Run80(3, 3);
    Effect_Run81(2, 3);
    Effect_Run67(17);
    finalActions = SceneAction_GroupFinish;
    Effect_Run69(1, finalActions);
    if (actorThreeEnabled != 0) {
        Effect_Run69(3, finalActions);
    }
    Effect_Run14(2, (s32)finalActions);
    Effect_Run76(10, 4);
    Effect_Run76(10, 4);
    Effect_Run71(20);
    Effect_Do6(0x1519);
    Effect_Run12(0x8008, 0);
    Effect_Run76(11, 4);
    Effect_Run76(11, 4);
    Effect_Run71(20);
    Effect_Place10(0x4009, 0, 10);
    Effect_Run81(0, 3);
    Effect_Check8(0x845);
    Effect_Run67(1);
    PaletteScene_SetRecordValue(184, 185);
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    extern s32 gOv5;

    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Effect_Do12(frame);
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
    if (gOv5 != 0) Effect_Do13(200);
    effect = Effect_Run16(26, spawn_x, spawn_y, spawn_z);
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
        Effect_Do14(0);
        Effect_Do15(20);
    } else if (step == 20) {
        Effect_Do16(1);
        Effect_Do17(8);
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
        Effect_Run85(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Effect_Run86(effect->owner[0x1c]);
        Effect_Run87(effect);
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    extern s32 gOv5;

    s32 slot = Effect_Check17(key);

    if (slot != -1) {
        s32 index = Effect_Check18(slot, key);

        if (index != -1) {
            Effect_Run17(slot)->values[index] = value;
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
    Effect_Run88();
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
    Effect_Run89(); Effect_Run90(); Effect_Run91(0x10000, 0);
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

    red = (s16)(red + Effect_Check19(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Effect_Check20(green, adjustment));
    blue = (s16)(blue - Effect_Check21(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
