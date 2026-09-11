#include "types.h"
#include "scene.h"
#include "abi/map/locations/sol_shinden/inner_sanctum_scene/inner_sanctum_scene.h"

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

EntA *Map_Run30(s32);

EntA *Map_Run31(s32);

EntB *Map_Run32(s32);

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

    scene_actor = Map_Run30(16);
    if (Map_Check41(0x809) == 0) {
        return;
    }
    if (Map_Check42(0x814) != 0) {
        Map_Run33();
        return;
    }
    if (Map_Check43(0x819) != 0) {
        return;
    }
    Map_Check44();
    Map_Apply(0, 0);
    Map_Do43((s32)&Value_00001000);
    if (Map_Check45(g1)!= 0 || Map_Check46(0x80a) == 0) {
        target_actor = Map_Run31(0);
        if (target_actor != 0) {
            Map_Place39(16, target_actor->unk8, target_actor->unk10);
        }
        Map_Check47(4);
        Map_Place40(16, s1, s2);
    } else {
        if (Map_Check48(g2)!= 0) goto do1;
        h1 = 0x1540000;
        if (scene_actor->unk8 > h1) {
do1:
            Map_Place41(16, 0x1880000, 0xa80000);
            Map_Do44(4);
            Map_Place42(16, s3, s4);
        }
    }
    if (Map_Check49(g3)!= 0) goto do2;
    h2 = 0x1540000;
    if (scene_actor->unk8 > h2) {
do2:
        Map_Place43(16, d1, 0xe8);
    } else {
        Map_Do45(0x80a);
    }
    Map_Place44(16, d2, 0xe8);
    Map_Place45(0, d5, 0);
    Map_Run34(16, s5, 10);
    Map_Place46(16, 0, 10);
    Map_Apply2(0, 3);
    if (Map_Check50(g4)!= 0 || Map_Check51(0x80a) == 0) {
        Map_Apply3(16, 2);
        target_position = Map_Run32(0);
        if (target_position != 0) {
            Map_Place47(16, target_position->unkA, target_position->unk12);
        }
        Map_Do46(16);
        Map_Run35(16, 0, 0);
        Map_Place48(0, d3, 0xe8);
    } else {
        Map_Place49(0, d4, 0xf8);
    }
    Map_Run36();
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

    if ((Map_Check52() & 3) != 0) {
        u16 v = gOv;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_Do47(0xbb);
            a = 1;
            b = 5;
            Map_SetRect10(0x2e, 0x3b, 30, 0x21, a, b);
            break;
        }
        case 1:
            Map_SetRect11(0x2e, 0x3b, 30, 0x21, v, v);
            Map_SetRect12(0x2e, 0x3b, 30, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Map_SetRect13(0x2e, 0x3b, 30, 0x22, v, v);
            Map_SetRect12(0x2e, 0x3b, 30, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Map_SetRect14(0x2e, 0x3b, 30, 0x23, v, v);
            Map_SetRect12(0x2e, 0x3b, 30, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            volatile s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_SetRect15(0x2e, 0x3b, 30, 0x24, v, v);
            Map_SetRect12(0x2e, 0x3b, 30, 0x25, v, 5);
            break;
        case 0x50: {
            s32 a = 1;
            s32 b = 10;
            Map_SetRect16(0x2e, 0x31, 30, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_Check53() * 40) >> 16) + 90) {
                *c = 0;
            }
        }
    }
    st = &gOv2;
    s = *st;
    if (s != 0) {
        if (s == 2) {
            Map_Place50(t1, t2, t3);
        } else if (s == 1) {
            Map_Place51(m1, m2, m3);
        }
        *st = *st - 1;
    }
}

void UpdateStatueLight2(void)
{
    if ((Map_Check54() & 3) != 0) {
        u16 v = gOv3;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_Do48(0xbb);
            a = 1;
            b = 5;
            Map_SetRect17(0x2f, 0x3b, 42, 0x21, a, b);
            break;
        }
        case 1:
            Map_SetRect18(0x2f, 0x3b, 42, 0x21, v, v);
            Map_SetRect19(0x2f, 0x3b, 42, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Map_SetRect20(0x2f, 0x3b, 42, 0x22, v, v);
            Map_SetRect19(0x2f, 0x3b, 42, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Map_SetRect21(0x2f, 0x3b, 42, 0x23, v, v);
            Map_SetRect19(0x2f, 0x3b, 42, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_SetRect22(0x2f, 0x3b, 42, 0x24, v, v);
            Map_SetRect19(0x2f, 0x3b, 42, 0x25, v, 5);
            break;
        case 0x5a: {
            s32 a = 1;
            s32 b = 10;
            Map_SetRect23(0x2f, 0x31, 42, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv3;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_Check55() * 40) >> 16) + 100) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight3(void)
{
    if ((Map_Check56() & 3) != 0) {
        u16 v = gOv4;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_Do49(0xbb);
            a = 1;
            b = 5;
            Map_SetRect24(0x30, 0x3b, 31, 0x24, a, b);
            break;
        }
        case 1:
            Map_SetRect25(0x30, 0x3b, 31, 0x24, v, v);
            Map_SetRect26(0x30, 0x3b, 31, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Map_SetRect27(0x30, 0x3b, 31, 0x25, v, v);
            Map_SetRect26(0x30, 0x3b, 31, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Map_SetRect28(0x30, 0x3b, 31, 0x26, v, v);
            Map_SetRect26(0x30, 0x3b, 31, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_SetRect29(0x30, 0x3b, 31, 0x27, v, v);
            Map_SetRect26(0x30, 0x3b, 31, 0x28, v, 5);
            break;
        case 0x5f: {
            s32 a = 1;
            s32 b = 10;
            Map_SetRect30(0x30, 0x31, 31, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv4;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_Check57() * 40) >> 16) + 105) {
                *c = 0;
            }
        }
    }
}

void UpdateStatueLight4(void)
{
    if ((Map_Check58() & 3) != 0) {
        u16 v = gOv5;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Map_Do50(0xbb);
            a = 1;
            b = 5;
            Map_SetRect31(0x2e, 0x3b, 41, 0x24, a, b);
            break;
        }
        case 1:
            Map_SetRect32(0x2e, 0x3b, 41, 0x24, v, v);
            Map_SetRect33(0x2e, 0x3b, 41, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Map_SetRect34(0x2e, 0x3b, 41, 0x25, v, v);
            Map_SetRect33(0x2e, 0x3b, 41, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Map_SetRect35(0x2e, 0x3b, 41, 0x26, v, v);
            Map_SetRect33(0x2e, 0x3b, 41, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &gOv2;
            *p = val;
        }
            v = 1;
            Map_SetRect36(0x2e, 0x3b, 41, 0x27, v, v);
            Map_SetRect33(0x2e, 0x3b, 41, 0x28, v, 5);
            break;
        case 0x55: {
            s32 a = 1;
            s32 b = 10;
            Map_SetRect37(0x2e, 0x31, 41, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &gOv5;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Map_Check59() * 40) >> 16) + 95) {
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

    Map_Run37(21);
    Map_Place(0, 0x178, 184);
    Map_Run38(0, 0);
    Map_Place2(16, 0x1780000, 0xb80000);
    Map_Place3(16, 0x10000, 0x8000);
    Map_Place4(16, 0x188, 168);
    Map_Place5(16, 0x8000, 30);
    Map_Run39(16, 1);
    Map_Do((s32)SCENE_STEP_VALUE);
    Map_Run40(16, 4, 30);
    Map_Run41(16, 6);
    Map_Run42(0, 2);
    Map_Run43(6);
    Map_Run44(16, 3);
    Map_Run45(16, 6);
    Map_Place6(16, 0x178, 184);
    Map_Place7(16, 0x6480000, 0x6480000);
    Map_Run46(4);
    Map_Do2(0x811);
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
    Map_SetRect2(30, 43, 33, 39, 8, 1);
    Map_SetRect3(30, 43, 36, 38, 3, 3);
    Map_SetRect4(14, 41, 32, 41, 8, 4);
    Map_Run2(0x23e0000, -1, 0x9e0000, 0);
    Map_Run47();
    Map_Place8(16, 0x23e0000, 0x780000);
    Map_Run48(0, 0, 0);
    Map_Run49(1);
    Map_Run3(0x2051cc, 1);
    Map_Run50(20);
    Map_Do3(0x201);
    Map_Do4(0x200);
    Map_Do5(0x202);
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Map_Run51();
    ((void (*)())Map_Check60)();
    Map_Run52(40);
    Map_Run53(171);
    Map_Run4(0x10005, 1);
    Map_Run54(8);
    Map_Run55(32);
    Map_Check(0x2051cc, 1);
    Map_Run56(24);
}

void Scene_RunClosingSequence(void)
{
    s32 i;
    u8 *work;
    Map_Run57();
    Map_Run58();
    gOv = 0;
    gOv3 = 0;
    gOv4 = 0;
    gOv5 = 0;
    Map_Run59(4097);
    Map_Place9(16, 16384, 20);
    Map_Place10(16, 256, 0);
    Map_Run60(16, 6, 30);
    Map_Run61(37617664, -1, 11403264, 1);
    Map_Run62();
    Map_Run63(30);
    Map_Run64(32784, 20);
    for (i = 0; i != 4; i++) {
        Map_Run65(246);
        Map_Run66();
        Map_Run67(12);
        Map_Run68(246);
        Map_Run69();
        Map_Run70(12);
    }
    Map_Run71(32784, 6);
    gOv = ((Map_Run72() * 60) >> 16) + 20;
    gOv3 = ((Map_Run73() * 60) >> 16) + 20;
    gOv4 = ((Map_Run74() * 60) >> 16) + 20;
    gOv5 = ((Map_Run75() * 60) >> 16) + 20;
    gOv2 = 0;
    Map_Check2((s32)UpdateStatueLight1, 3200);
    Map_Check3((s32)UpdateStatueLight2, 3200);
    Map_Check4((s32)UpdateStatueLight3, 3200);
    Map_Check5((s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Map_Run76(246);
        Map_Run77();
        Map_Run78(5);
        Map_Run79(246);
        Map_Run80();
        Map_Run81(5);
    }
    for (i = 0; i != 8; i++) {
        Map_Run82(246);
        Map_Run83();
        Map_Run84(4);
        Map_Run85(246);
        Map_Run86();
        Map_Run87(4);
    }
    for (i = 0; i != 10; i++) {
        Map_Run88(246);
        Map_Run89();
        Map_Run90(3);
        Map_Run91(246);
        Map_Run92();
        Map_Run93(3);
    }
    for (i = 0; i != 12; i++) {
        Map_Run94(246);
        Map_Run95();
        Map_Run96(2);
        Map_Run97(246);
        Map_Run98();
        Map_Run99(2);
    }
    Map_SetRect5(45, 30, 34, 10, 4, 2);
    Map_Run100(16, 6, 40);
    Map_Run101(32784, 6);
    Map_Place11(16, 131072, 65536);
    Map_Run102(16, 576, 280);
    Map_Run103((s32)UpdateStatueLight1);
    Map_Run104((s32)UpdateStatueLight2);
    Map_Run105((s32)UpdateStatueLight3);
    Map_Run106((s32)UpdateStatueLight4);
    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Map_Run107();
    Map_Run108();
    Map_Run109(4);
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

    Map_Run110();
    if (Map_Check6(2059) != 0 && Map_Check7(2086) != 0) {
        Map_Do6(2086);
        Map_SetRect6(45, 28, 34, 10, 2, 1);
    } else if (Map_Check8(2059) != 0 && Map_Check9(2086) == 0) {
        Map_Do7(2086);
    }
    if (Map_Check10(2060) != 0 && Map_Check11(2087) != 0) {
        Map_Do8(2087);
        Map_SetRect7(47, 28, 36, 10, 2, 1);
    } else if (Map_Check12(2060) != 0 && Map_Check13(2087) == 0) {
        Map_Do9(2087);
    }
    if (Map_Check14(2061) != 0 && Map_Check15(2088) != 0) {
        Map_Do10(2088);
        Map_SetRect8(45, 29, 34, 11, 2, 1);
    } else if (Map_Check16(2061) != 0 && Map_Check17(2088) == 0) {
        Map_Do11(2088);
    }
    if (Map_Check18(2062) != 0 && Map_Check19(2089) != 0) {
        Map_Do12(2089);
        Map_SetRect9(47, 29, 36, 11, 2, 1);
    } else if (Map_Check20(2062) != 0 && Map_Check21(2089) == 0) {
        Map_Do13(2089);
    }
    Map_Run111();
    Map_Place12(16, 16384, 20);
    Map_Run112(16, 6, 30);
    Map_Run113(37617664, -1, 11403264, 1);
    Map_Run114();
    Map_Run115(30);
    for (i6 = 0; i6 != 4; i6++) {
        Map_Run116(246);
        Map_Run117();
        Map_Run118(12);
        Map_Run119(246);
        Map_Run120();
        Map_Run121(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Map_Run122(246);
        Map_Run123();
        Map_Run124(8);
        Map_Run125(246);
        Map_Run126();
        Map_Run127(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Map_Run128(246);
        Map_Run129();
        Map_Run130(6);
        Map_Run131(246);
        Map_Run132();
        Map_Run133(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Map_Run134(246);
        Map_Run135();
        Map_Run136(4);
        Map_Run137(246);
        Map_Run138();
        Map_Run139(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Map_Run140(246);
        Map_Run141();
        Map_Run142(2);
        Map_Run143(246);
        Map_Run144();
        Map_Run145(2);
    }
    Map_Run146(246);
    Map_Run147();
    Map_Run148(6);
    if (Map_Check61(2082) == 0) {
        base = 32784;
        Map_Run149(4133);
        Map_Run150(base, 6);
        Map_Run151(16, 3);
        Map_Run152(base, 6);
    }
    work = *(u8 **)gWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Map_Run153();
    Map_Run154();
    Map_Run155(5);
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

    if (Map_Run5()!= 0) {
        record = Map_Check22(0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Map_Run156();
        Map_Run6(0x2051cc, 1);
        Map_Run157(20);
        Map_Do14(0x201);
        Map_Do15(0x200);
        Map_Do16(0x202);
        if (Map_Check23(0x80a) == 0) {
            Map_Run158();
        }
        if (Map_Run7()!= 0) {
            if (Map_Check24(0x811) == 0) {
                Map_Run159();
            }
        }
        Map_Run160();
    } else {
        if (Map_Check25(0x200) == 0) {
            Map_Run161();
            Map_Run8(0x10000, 1);
            Map_Run162(20);
            Map_Do17(0x200);
            Map_Do18(0x201);
            Map_Do19(0x202);
            Map_Run163();
        }
    }
    L_020000f0:;
}

void Scene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (Map_Check26(0x200) == 0) {
        Map_Run164();
        Map_Run9(0x10000, 1);
        Map_Run165(20);
        Map_Do20(0x200);
        Map_Do21(0x201);
        Map_Do22(0x202);
        Map_Run166();
    }
}

void Scene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Map_Run10()!= 0) {
        record = Map_Check27(0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Map_Run167();
        Map_Run11(0x10000, 1);
        Map_Run168(20);
        Map_Do23(0x200);
        Map_Do24(0x201);
        Map_Do25(0x202);
        Map_Run169();
    } else {
        if (Map_Check28(0x201) == 0) {
            Map_Run170();
            Map_Run12(0x2051cc, 1);
            Map_Run171(20);
            Map_Do26(0x201);
            Map_Do27(0x200);
            Map_Do28(0x202);
            if (Map_Check29(0x80a) == 0) {
                Map_Run172();
            }
            Map_Run173();
        }
    }
    L_020001d6:;
}

void Scene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (Map_Check30(0x202) == 0) {
        Map_Run13(0x202db1, 1);
        Map_Run174(20);
        Map_Do29(0x202);
        Map_Do30(0x200);
        Map_Do31(0x201);
    }
}

void Scene_RunScene37a(void)
{
    u32 i;
    s32 record;
    s32 base6_ffc;
    s32 base5_4010;
    s32 base5_4010_2;

    Map_Do32(0xff6);
    Map_Place13(0, 0x10000, 0x8000);
    Map_Place14(0, 0x1e8, 176);
    Map_Run175(0, 0);
    record = Map_Check31(0);
    if (record != 0) {
        Map_Run176(16, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Map_Run177(0, 0, 1);
    Map_Place15(16, 0x10000, 0x8000);
    Map_Place16(16, 0x1d8, 168);
    Map_Run178(16, 0, 60);
    Map_Run179(16, 4, 40);
    Map_Run180(16, 6);
    Map_Run14(0x26666, 0x4ccc);
    Map_Run15(0x23f0000, -1, 0xb50000, 1);
    Map_Run181();
    Map_Run182(120);
    Map_Run16(0x1010, 80);
    Map_Run17(0x1ec0000, -1, 0xa80000, 1);
    Map_Run183();
    Map_Run184(20);
    base5_4010 = (s32)gVal2;
    Map_Place17(16, 0x3000, 20);
    Map_Run185(base5_4010, 6);
    Map_Run186(16, 0, 60);
    Map_Run187(16, 2);
    Map_Place18(16, 0x3000, 10);
    Map_Check32(base5_4010, 0);
    if (Map_Check33(0, 0) == 0) {
        Map_Do33(0xffa);
    } else {
        Map_Do34(0xffb);
    }
    base5_4010_2 = (s32)gVal2;
    Map_Place19(0, 0xa000, 10);
    Map_Run188(base5_4010_2, 10);
    base6_ffc = (s32)gVal;
    Map_Run189(base6_ffc);
    Map_Run190(16, 0, 40);
    Map_Place20(16, 0x105, 40);
    Map_Run191(16, 4);
    Map_Place21(16, 0x3000, 10);
    Map_Run192(16, 4);
    Map_Check34(base5_4010_2, 0);
    if (Map_Check35(0, 0) == 0) {
        Map_Run193((base6_ffc + 1));
        Map_Do35(0x896);
    } else {
        Map_Run194((base6_ffc + 2));
    }
    Map_Run18((s32)gVal2, 4);
    Map_Run195(16, 1);
    Map_Place22(16, 0x1e6, 131);
    Map_Place23(16, 0x240, 120);
    Map_Place24(16, 0xc000, 2);
    Map_Run19(0x40000, 0x8000);
    Map_Do36(0x80a);
}

void Scene_RunScene37aSequenceE(void)
{
    u32 i;
    s32 record;

    if (Map_Check36(0x810) != 0) {
    } else {
        if (Map_Run20() == 0) {
        } else {
            Map_Run196();
            Map_Place25(16, 0x2410000, 0x930000);
            Map_Place26(16, 0x4000, 1);
            Map_Run21(0x23e0000, -1, 0xb80000, 1);
            Map_Do37(0x1027);
            Map_Place27(0, 0x240, 232);
            Map_Run197(0, 0);
            Map_Run198();
            Map_Run199(10);
            Map_Place28(16, 0x10000, 0x8000);
            Map_Place29(16, 0x240, 152);
            Map_Run200(6);
            Map_Run201(16, 6, 30);
            Map_Run202(16, 6);
            Map_Run203(0, 3);
            Map_Run204(2);
            Map_Run205(16, 4);
            Map_Run206(16, 6);
            Map_Run22(0, 0x102);
            Map_Run207(40);
            Map_Run208(16, 2);
            Map_Run209(30);
            Map_Run210(16, 6);
            Map_Run211(0, 3);
            Map_Place30(16, 0x240, 184);
            Map_Run212(6);
            Map_Run213(16, 2);
            Map_Run214(40);
            Map_Run23(0x4010, 6);
            Map_Place31(16, 0x240, 208);
            Map_Run215(40);
            Map_Run216(0, 3);
            Map_Run217(6);
            Map_Place32(16, 0x8000, 0x4000);
            Map_Run218(16, 2);
            record = Map_Check37(0);
            if (record != 0) {
                Map_Run219(16, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Map_Run220(16);
            Map_Run221(16, 0, 0);
            Map_Do38(0x810);
            Map_Run222();
        }
    }
}

void Scene_RunScene37a(void)
{
    u32 i;
    s32 record;

    Map_Run223();
    if (Map_Check38(0x896) != 0) {
        Map_Do39(0xffd);
    } else {
        Map_Do40(0xfff);
    }
    Map_Run224(16, 0, 10);
    Map_Place33(16, 0xc000, 10);
    Map_Run225();
}

/* map/locations/sol_shinden/inner_sanctum_scene/scene_sequence_f.c */
/* Message ids handed to the Map_Run226 service; see the header note. */
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
 * address (Map_Run227, Map_Run228, Map_Run229, Map_Run230,
 * Map_Run231, Map_Run232, Map_Run233 and Map_Run234).
 * Separating them needs an `absolute_symbols` entry in the translation-unit
 * manifest, which a single-file draft may not add, so all sixteen of those
 * sites are spelled with the main-image name of the service they reach.
 * Those calls are semantically right; they emit the direct long call instead
 * of this overlay's veneer word, and nothing else about them differs.
 *
 * The four ids handed to the Map_Run226 message service are spelled as
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
 * the Map_Run235 service is read at +8/+16 as words early in the scene and
 * at +10/+18 as signed halfwords at the end, so both stay raw offsets; no
 * shared type in games/gs1/include covers it.
 */

/* Each declaration's comment names the owner or main-image service the call
 * word reaches. */

/* The sixteen colliding sites reach these main-image services directly. */

/* Ordinary C field access retained from the reviewed disassembly dataflow. */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
s32  Map_Run236();  /* Map_Run237 */
s32  Map_Run238();  /* Map_Run235 */
s32  Map_Run239();  /* Map_Run240 */
s32  Map_Run241();  /* Map_Run242 */
void Map_Run243();  /* Scene_RunScene37aSequenceA (0x02000054) */
void Map_Run244();  /* SetSolShindenActorStep (0x020025fc) */
void Map_Run245();  /* Map_Run246 */
void Map_Run247();  /* Map_Run248 */
void Map_Run249();  /* Map_Run250 */
void Map_Run251();  /* Map_Run252 */
void Map_Run253();  /* Map_Run254 */
void Map_Run255();  /* Map_Run226 */
void Map_Run256();  /* Map_Run257 */
void Map_Run258();  /* Map_Run259 */
void Map_Run260();  /* Map_Run261 */
void Map_Run262();  /* Map_Run263 */
void Map_Run264();  /* Map_Run265 */
void Map_Run266();  /* Map_Run267 */
void Map_Run268();  /* Map_Run269 */
void Map_Run270();  /* Map_Run271 */
void Map_Run272();  /* Map_Run273 */
void Map_Run180();  /* Map_Run274 */
void Map_Run275();  /* Map_Run276 */
void Map_Run277();  /* Map_Run278 */
void Map_Run231();  /* main-image name; collision fallback, see the RESIDUAL note */
void Map_Run279();  /* Map_Run280 */
void Map_Run281();  /* Map_Run282 */
void Map_Run283();  /* Map_Run284 */

void Scene_RunScene37aSequenceF(void)
{
    u8 *rec;

    if (Map_Check39(0x814) != 0) {
        Map_Run243();
    }
    if (Map_Check39(0x809) == 0) {
        Map_Run245();
        Map_Do41((s32)gVal3);
        Map_Run266(17);
        Map_Place34(0, 0x10000, 0x8000);
        Map_Place35(0, 0x120, 232);
        Map_Run253(0, 0);
        Map_Run247(20);
        Map_Run266(21);
        Map_Place36(0, 0xc000, 0);
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run256(16, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Map_Place34(16, 0x16666, 0xb333);
        Map_Place35(16, 0x120, 206);
        Map_Run247(40);
        Map_Place37(16, 0x100, 0);
        Map_Run260(16, 4, 60);
        Map_Run244(16, 20);
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run256(1, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run256(5, FIELD(rec, s32 *, 8), FIELD(rec, s32 *, 16));
        }
        Map_Place34(1, 0x8000, 0x4000);
        Map_Place34(5, 0x8000, 0x4000);
        Map_Place38(1, 0x118, 248);
        Map_Place35(5, 0x128, 248);
        Map_Run253(1, 1);
        Map_Place36(1, 0xd000, 0);
        Map_Place36(5, 0xb000, 30);
        Map_Run24(0x9999, 0x1333);
        Map_Run25(0x1200000, -1, 0xd50000, 1);
        Map_Place34(16, 0x6666, 0x3333);
        Map_Place35(16, 0x120, 176);
        Map_Run247(40);
        Map_Run268(16, 2);
        Map_Run244(16, 6);
        Map_Place36(16, 0x4000, 60);
        Map_Run244(16, 20);
        Map_Run258(16, 0, 40);
        Map_Run270(16, 3);
        Map_Run247(10);
        Map_Place36(16, 0x8000, 40);
        Map_Run270(16, 3);
        Map_Run247(20);
        Map_Run268(5, 2);
        Map_Place36(5, 0x9000, 10);
        Map_Run244(5, 10);
        Map_Run268(1, 2);
        Map_Place36(1, 0xf000, 10);
        Map_Run244(1, 6);
        Map_Run26(5, 0x102);
        Map_Run247(40);
        Map_Place36(5, 0xa000, 10);
        Map_Run27(0x2005, 10);
        Map_Run268(16, 2);
        Map_Run247(10);
        Map_Place36(16, 0xa000, 20);
        Map_Run26(16, 0x102);
        Map_Run247(20);
        Map_Place36(0, 0x5000, 40);
        Map_Place36(0, 0xc000, 0);
        Map_Place36(1, 0xe000, 0);
        Map_Place36(5, 0xa000, 40);
        Map_Place37(1, 0x101, 20);
        Map_Run277(1, 0);
        Map_Run247(60);
        Map_Run270(16, 4);
        Map_Run247(40);
        Map_Run244(16, 20);
        Map_Place37(5, 0x101, 40);
        Map_Run244(5, 60);
        Map_Run270(16, 3);
        Map_Run244(16, 10);
        Map_Place37(0, 0x105, 0);
        Map_Place37(1, 0x105, 0);
        Map_Place37(5, 0x105, 60);
        Map_Run268(1, 2);
        Map_Run247(20);
        Map_Run244(1, 10);
        Map_Run270(16, 3);
        Map_Run247(20);
        Map_Place37(0, 0x102, 0);
        Map_Place37(1, 0x102, 0);
        Map_Place37(5, 0x102, 80);
        Map_Place37(16, 0x105, 80);
        Map_Run244(16, 6);
        Map_Place36(0, 0x4000, 0);
        Map_Place36(1, 0xf000, 0);
        Map_Place36(5, 0x9000, 60);
        Map_Place36(16, 0x4000, 10);
        Map_Run268(16, 3);
        Map_Run247(6);
        Map_Run28(16, 0);
        if (Map_Run29(0, 0) == 0) {
            Map_Run255((s32)gVal4);
        } else {
            Map_Do41((s32)gVal5);
            Map_Place37(16, 0x107, 20);
        }
        Map_Run260(16, 4, 20);
        Map_Place36(0, 0xc000, 0);
        Map_Place36(1, 0xe000, 0);
        Map_Place36(5, 0xa000, 0);
        Map_Run244(16, 6);
        Map_Do41((s32)gVal6);
        Map_Run247(30);
        Map_Run270(5, 4);
        Map_Run27(0x2005, 6);
        Map_Run270(1, 3);
        Map_Run244(1, 20);
        Map_Run260(16, 6, 20);
        Map_Place37(16, 0x104, 20);
        Map_Run244(16, 30);
        Map_Run253(0, 3);
        Map_Run253(1, 3);
        Map_Run270(5, 3);
        Map_Run247(20);
        Map_Run270(16, 3);
        Map_Run244(16, 6);
        Map_Place34(1, 0x10000, 0x8000);
        Map_Place34(5, 0x10000, 0x8000);
        Map_Place34(16, 0x20000, 0x10000);
        Map_Run253(16, 2);
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run231(16, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_Run279(16);
        Map_Run256(16, 0, 0);
        Map_Run253(1, 2);
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run231(1, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_Run279(1);
        Map_Run256(1, 0, 0);
        Map_Run253(5, 2);
        rec = (u8 *)Map_Check40(0);
        if (rec != 0) {
            Map_Run231(5, FIELD(rec, s16 *, 10), FIELD(rec, s16 *, 18));
        }
        Map_Run279(5);
        Map_Run256(5, 0, 0);
        Map_Do42(0x144);
        Map_Do42(0x809);
        Map_Run283();
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
    if (Map_Check62(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect38(0x2d, 28, 0x22, 10, a, b);
    }
    if (Map_Check63(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect39(0x2f, 28, 0x24, 10, a, b);
    }
    if (Map_Check64(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect40(0x2d, 29, 0x22, 11, a, b);
    }
    if (Map_Check65(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect41(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (Map_Check66(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect42(0x2d, 28, 0x22, 10, a, b);
    }
    if (Map_Check67(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect43(0x2f, 28, 0x24, 10, a, b);
    }
    if (Map_Check68(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect44(0x2d, 29, 0x22, 11, a, b);
    }
    if (Map_Check69(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect45(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (Map_Check70(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect46(0x2d, 30, 0x22, 10, a, b);
    }
    if (Map_Check71(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect47(0x2f, 30, 0x24, 10, a, b);
    }
    if (Map_Check72(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect48(0x2d, 31, 0x22, 11, a, b);
    }
    if (Map_Check73(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect49(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (Map_Check74(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect50(0x2d, 30, 0x22, 10, a, b);
    }
    if (Map_Check75(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect51(0x2f, 30, 0x24, 10, a, b);
    }
    if (Map_Check76(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect52(0x2d, 31, 0x22, 11, a, b);
    }
    if (Map_Check77(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_SetRect53(0x2f, 31, 0x24, 11, a, b);
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
    Map_Run285(actor_step, 0);
    Map_Run286(wait_frames);
}

/* map/locations/sol_shinden/inner_sanctum_scene/sol_shinden.c */
extern u32 gWork;

/* One import, four call sites, four distinct veneer addresses. */

static __inline__ void SetInitialScale(s32 actor, s32 scale, s32 duration)
{
    Map_Place52(actor, scale, duration);
}

static __inline__ void SetInitialDirection(s32 actor, s32 direction, s32 duration)
{
    Map_Place53(actor, direction, duration);
}

static __inline__ void SetFinalScale(s32 actor, s32 horizontal, s32 vertical)
{
    Map_Place54(actor, horizontal, vertical);
}

void StartSolShindenTrapEvent(void)
{
    u32 scene_state;
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Map_Run287();
    Map_Run288();
    Map_Run289(4120);
    SetInitialScale(16, 0x4000, 20);
    SetInitialDirection(16, 256, 0);
    Map_Run290(16, 6, 30);
    Map_Run291(37617664, -1, 11403264, 1);
    Map_Run292();
    Map_Run293(30);
    Map_Run294(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Map_Run295(246);
        Map_Run296();
        Map_Run297(12);
        Map_Run298(246);
        Map_Run299();
        Map_Run300(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Map_Run301(246);
        Map_Run302();
        Map_Run303(8);
        Map_Run304(246);
        Map_Run305();
        Map_Run306(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Map_Run307(246);
        Map_Run308();
        Map_Run309(6);
        Map_Run310(246);
        Map_Run311();
        Map_Run312(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Map_Run313(246);
        Map_Run314();
        Map_Run315(4);
        Map_Run316(246);
        Map_Run317();
        Map_Run318(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Map_Run319(246);
        Map_Run320();
        Map_Run321(2);
        Map_Run322(246);
        Map_Run323();
        Map_Run324(2);
    }
    Map_Run325();
    Map_Run326(6);
    Map_Run327(32784, 6);
    SetFinalScale(16, 0x20000, 0x10000);
    Map_Run328(16, 576, 280);
    scene_state = gWork;
    *(s32 *)(scene_state + 448) = 256;
    *(s32 *)(scene_state + 456) = 32;
    Map_Run329();
    Map_Run330();
    Map_Run331(2067);
    Map_Run332(3);
}

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (Map_Check78(0x80b) == 0)
        all_set = 0;
    if (Map_Check79(0x80c) == 0)
        all_set = 0;
    if (Map_Check80(0x80d) == 0)
        all_set = 0;
    if (Map_Check81(0x80e) == 0)
        all_set = 0;

    return all_set;
}
