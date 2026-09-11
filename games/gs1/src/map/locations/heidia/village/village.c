#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/village.h"
#include "object_runtime.h"

/* map/locations/heidia/village/actor_pair_sequence.c */
extern u8 gVal[];
extern u8 gWork[];

void Scene_RunScene3bf(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Map_Run2();
    Map_Run3(0, 0, 0);
    Map_Run4(0, 1);
    Map_Run5(0, 1);
    Map_Run6(113);
    Map_Place(21, 0x100, 0);
    Map_Place2(13, 0x100, 60);
    Map_Run7(21, 0, 0);
    Map_Run8(13, 0, 0);
    base5_240d = (s32)gVal;
    Map_Run9(base5_240d);
    Map_Run10(13, 0);
    Map_Place3(0, 0x102, 30);
    Map_Run11((base5_240d + 1));
    Map_Run12(13, 0);
    Map_Run13();
    Map_Run14(60);
    Map_Run15(60);
    Map_Run16();
    Map_Do(0x225);
}

/* map/locations/heidia/village/actor_scenes.c */
/* External Func aliases name loader-relocated call words, not runtime addresses. */

u8 *Map_Run17(s32);

extern u8 gWork[];

extern s32 gOv[];

extern s32 gOv2[];

extern s32 gOv3[];

extern u32 gOv4;

extern u8 gVal2[];

extern u8 gCam[];
extern u32 gIw;
extern s16 gCell[];
extern u8 Value_00002092;
struct ObjectRuntime *Map_Run18();
struct ObjectRuntime *Map_Run19();

struct ObjectRuntime *Map_Run20();

struct ObjectRuntime *Map_Run21();

void ConfigureSceneActor12(void)
{
    s32 actor_slot = 15;
    u8 *actor;

    Map_SetRect6(15, 20, 1, 1, actor_slot, 22);
    Map_SetRect7(17, 23, 1, 3, actor_slot, 23);
    actor = Map_Run17(12);
    if (actor != 0) {
        Map_Apply(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{
    Map_Run22();
}

void Scene_StartActorTwelveTransition(void)
{
    Map_Place4(12, 0x10000, 0x8000);
    Map_Place5(12, 248, 0x178);
    Map_Run23(12);
    Map_Run24(215);
    Map_Run25(60);
    Map_Run26();
    Map_Do2(0x943);
}

void Scene_UpdateActorTwelveTransition(void)
{
    u8 *rec7;
    s32 record;

    record = Map_Check(12);
    if ((*(volatile s32 *)(record + 16) >> 20) > 22) {
        Map_Place6(0x40000, 0x40000, 0x10000);
        Map_Place7(-1, -1, 0xe666);
        Map_Run27(144);
        Map_SetRect(15, 20, 1, 1, 15, 22);
        Map_SetRect2(17, 23, 1, 3, 15, 23);
        rec7 = Map_Check2(12);
        if ((s32)rec7 != 0) {
            Map_Do12((s32)rec7, 0);
            rec7[35] = 2;
        }
        Map_Do3(0x943);
    }
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    s32 position_x = gOv[table_index * 2];
    s32 position_z = gOv[table_index * 2 + 1];

    Map_SetRect8(0, 0x4d, 1, 3, position_x, position_z);
    Map_SetRect9(1, 0x4d, 1, 1, position_x + 1, position_z);
    Map_SetRect10(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void Scene_UpdateObjectPairA(void)
{
    s32 v6;
    s32 raw;
    s32 d;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    v6 = 1;
    if (Map_Do13(234) != -v6) {
        raw = *(s16 *)(((s32)p5 + 0x16c));
        d = raw - 40;
        if (Map_Check3(0x941) != 0) {
            if (d == 4) {
                goto done;
            }
        }
        Map_Run28(d);
        Map_Run29(157);
        Map_Place8(0x30000, 0x30000, 0x10000);
        Map_Place9(-v6, -v6, 0xe666);
        Map_Run30(raw + 0x328);
    }
done:;
}

void Scene_UpdateObjectPairB(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)gWork;
    if (Map_Check23(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Map_Run31(field - 40);
        Map_Run32(157);
        Map_Place10(0x30000, 0x30000, 0x10000);
        Map_Place11(-1, -1, 0xe666);
        Map_Run33(field + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    s32 position_x = gOv2[table_index * 2];
    s32 position_z = gOv2[table_index * 2 + 1];

    Map_SetRect11(0x37, 0x79, 1, 3, position_x, position_z);
    Map_SetRect12(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Map_SetRect13(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void Scene_UpdateTableBObjectPair(void)
{
    u8 *work;
    s16 field;

    work = *(volatile s32 *)gWork;
    if (Map_Check24(234) != -1) {
        field = *(s16 *)((s32)work + 0x16c);
        Map_Run34(field - 40);
        Map_Run35(157);
        Map_Place12(0x30000, 0x30000, 0x10000);
        Map_Place13(-1, -1, 0xe666);
        Map_Run36(field + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    s32 position_x = gOv3[table_index * 2];
    s32 position_z = gOv3[table_index * 2 + 1];

    Map_SetRect14(1, 0x50, 1, 3, position_x, position_z);
    Map_SetRect15(2, 0x50, 1, 1, position_x + 1, position_z);
    Map_SetRect16(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void Scene_UpdateObjectPairC(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)gWork;
    if (Map_Check25(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Map_Run37(field - 40);
        Map_Run38(157);
        Map_Place14(0x30000, 0x30000, 0x10000);
        Map_Place15(-1, -1, 0xe666);
        Map_Run39(field + 0x332);
    }
}

static __inline__ void SetSceneValue(s16 *field, s32 value)
{
    *field = value;
}

void Scene_UpdateActorPairInteraction(void)
{
    struct ObjectRuntime *actor = Map_Run18(9);
    struct ObjectRuntime *other = Map_Run19(10);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_Check26(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!Map_Check4(0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!Map_Check4(0x214) && actor->movement_state == 0) {
            work[8] = Map_Check27(Map_Apply2(8912896.0, Map_Do14(actor->x)));
        }
        if (!Map_Check28()) {
            if (gCell[294] != 0) {
                if (Map_Check29(9) && gCell[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
                if (Map_Check29(10) && gCell[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
            }
            if (gCell[294] == 0) {
                if (Map_Check30(9)) {
                    Map_Run40(0x215);
                    Map_Run40(0x214);
                }
                if (Map_Check30(10)) {
                    Map_Run40(0x215);
                    Map_Run40(0x214);
                }
            }
            if (Map_Check4(0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

void Scene_UpdateActorSeventeenInteraction(void)
{
    struct ObjectRuntime *actor = Map_Run20(17);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    Map_Run41(actor);
    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_Check31(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Map_Check5(0x214)) {
        actor->movement_state = 0;
        if (!Map_Check5(0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!Map_Check32()) {
            Map_Check33(17);
            if (Map_Check34(17) && gCell[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gCell[294] == 0) {
                if (Map_Check35(17)) {
                    Map_Run42(0x215);
                    Map_Run42(0x214);
                }
            }
            if (Map_Check5(0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void Scene_UpdateActorEighteenInteraction(void)
{
    struct ObjectRuntime *actor = Map_Run21(18);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gIw & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Map_Check36(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Map_Check6(0x214)) {
        actor->movement_state = 0;
        if (!Map_Check6(0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!Map_Check37()) {
            if (Map_Check38(18) && gCell[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gCell[294] == 0) {
                if (Map_Check39(18)) {
                    Map_Run43(0x215);
                    Map_Run43(0x214);
                }
            }
            if (Map_Check6(0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void Scene_SelectActorTwentyOneMessage(void)
{
    switch (gOv4) {
    case 0:
        Map_Run44(0x2414);
        Map_Run45(21, 0);
        break;
    case 1:
        Map_Run44(0x2415);
        Map_Run45(21, 0);
        break;
    case 2:
        Map_Run44(0x2416);
        Map_Run45(21, 0);
        break;
    case 3:
        Map_Run44(0x2417);
        Map_Run45(21, 0);
        break;
    case 4:
        Map_Run44(0x2418);
        Map_Run45(21, 0);
        break;
    case 6:
        Map_Run44(0x241a);
        Map_Run45(21, 0);
        break;
    case 7:
        Map_Run44(0x241b);
        Map_Run45(21, 0);
        break;
    case 5:
        Map_Place16(21, 0xd000, 0);
        Map_Run46(50);
        Map_Place17(21, 0xb000, 0);
        Map_Run47(50);
        Map_Place18(21, 0x5000, 0);
        Map_Run48(50);
        Map_Run49(0x2419);
        Map_Run50(21, 0);
        break;
    }
}

void Scene_RunActorTwentyOneSequence(void)
{
    s32 base5_2411;

    Map_Place19(21, 0x101, 30);
    Map_Place20(21, 0xd000, 0);
    Map_Run51(50);
    Map_Place21(21, 0xb000, 0);
    Map_Run52(50);
    Map_Place22(21, 0x5000, 0);
    Map_Run53(50);
    base5_2411 = (s32)gVal2;
    Map_Run54(base5_2411);
    Map_Run55(21, 0);
    Map_Run56(21, 4);
    Map_Run57(60);
    Map_Place23(21, 0xb000, 0);
    Map_Run58(40);
    Map_Run59((base5_2411 + 1));
    Map_Run60(21, 0);
}

/* map/locations/heidia/village/actor_setup.c */
extern u8 gWork[];

struct ObjectRuntime *Map_Run61();
struct ObjectRuntime *Map_Run62();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Map_Run63();

struct ObjectRuntime *Map_Run64();
struct ObjectRuntime *Map_Run65();

void Village_FinishActorRestore();

extern s16 gCell[];
extern u8 gOv5[], gOv6[], gOv7[];

struct ObjectRuntime *Map_Run66();

struct ObjectRuntime *Map_Run67();
struct ObjectRuntime *Map_Run68();

extern u8 gOv8[], gOv9[], gOv10[];
extern u8 gOv11[], gOv12[], gOv13[];

/* Scheduler_AddOrUpdateCallback returns an index even when it is ignored. */

void Scene_InstallSceneTasks(void)
{
    Map_Run69();
    switch (gCell[225]) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Check7((s32)gOv8, 3200);
        Map_Check8((s32)gOv9, 3200);
        Map_Check9((s32)gOv10, 3200);
        Map_Run70(0xe00);
        break;
    case 12:
    case 19:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x209;
        Map_Run70(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Check10((s32)gOv11, 3200);
        Map_Check11((s32)gOv12, 3200);
        Map_Run71(1);
        Map_Run72();
        Map_Run73(1);
        Map_SetRect3(101, 9, 10, 8, 110, 9);
        Map_Run70(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Check12((s32)gOv13, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Run74(0xe00);
        break;
    }
    Map_Run75(18, 1);
    Map_Run76(17, 1);
    Map_Run77(21, 1);
    Map_Run78(12, 1);
    Map_Run79(13, 1);
    Map_Run80(1);
}

void Scene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    Map_Run81();
    Map_Run82(9, 1);
    Map_Run83(10, 1);
    Map_Run84(17, 1);
    if (Map_Check40(0x94c)) {
        Map_Run85(15, 0, 0);
    }
    if (Map_Check41(0x949)) {
        Map_Run86(11, 0, 0);
    }
    if (Map_Check42(0x94b)) {
        Map_Run87(16, 0, 0);
    }
    if (Map_Check43(0xf2e)) {
        Map_Run88(8, 0, 0);
    }
    switch (gCell[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Run89(0xe00);
        Map_Run90(gOv5, 3200);
        Map_Run91(1);
        Map_Run92();
        Map_Run93(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x209;
        Map_Run94(0xc00);
        Map_Run95(Map_Run66(24), 0);
        if (Map_Check44(0x314)) {
            Map_Place24(25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Run96(0xe00);
        Map_Run90(gOv6, 3200);
        Map_Run91(1);
        Map_Run92();
        Map_Run93(1);
        break;
    case 11:
    case 12:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        if (Map_Check45(0x94a)) {
            Map_Run97();
        }
        break;
    case 31:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Run98();
        break;
    case 14:
    case 15:
    case 16:
        Map_Run99(gOv7, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
        Map_Run100(0xe00);
        break;
    }
    actor = Map_Run67(8);
    Map_Run101(Map_Run68(8), 0);
    Map_Run102(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void Scene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x200;
    Map_Run103();
    if (Map_Check46(0x943)) {
        Map_Run104();
    }
    Map_Do4(0x217);
    Map_Do5(0x218);
    if (Map_Check47(0x944)) {
        Map_Run105(8, 0, 0);
        Map_Do6(0x217);
    }
    if (Map_Check48(0x945)) {
        Map_Run106(9, 0, 0);
        Map_Run107();
    }
    if (Map_Check49(0x946)) {
        Map_Run108(10, 0, 0);
        Map_Do7(0x218);
    }
    if (Map_Check50(0x947)) {
        Map_Run109();
    }
    if (Map_Check51(0x948)) {
        Map_Run110();
    }
    Map_Run111();
    actor = Map_Run61(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Map_Run62(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Map_Run63(11);
    if (actor != 0) {
        Map_Run112(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Map_Run64(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Map_Run113(Map_Run65(11), 0);
    Map_Run114();
    Village_FinishActorRestore(0xe00);
}

void Scene_ActivateThreeActorGroup(void)
{
    if (Map_Check52(0x35a)) {
        Map_Do15(0);
    }
    if (Map_Check53(0x35b)) {
        Map_Do16(1);
    }
    if (Map_Check54(0x35c)) {
        Map_Do17(2);
    }
}

void Scene_ActivateTwoActorGroup(void)
{
    if (Map_Check55(0x358)) {
        Map_Do13(0);
    }
    if (Map_Check56(0x359)) {
        Map_Do18(1);
    }
}

void Scene_ActivateAlternateActorGroup(void)
{
    if (Map_Check57(0x355)) {
        Map_Do12(0);
    }
    if (Map_Check58(0x356)) {
        Map_Do19(1);
    }
    if (Map_Check59(0x357)) {
        Map_Do20(2);
    }
}

/* map/locations/heidia/village/actor_steps.c */
extern u8 gVal3[];
extern u8 gWork[];

void Scene_RunScene3bf2(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)gVal3;
    Map_Run115(base6_2424);
    Map_Run116(a0, 0);
    Map_Run117(120);
    Map_Place25(a0, 0x101, 60);
    Map_Run118((base6_2424 + 1));
    Map_Run119(a0, 0);
    Map_Run120(a0, 1);
    Map_Run121((base6_2424 + 2));
    Map_Run122(a0, 0);
    Map_Run123(a0, 4);
    Map_Run124((base6_2424 + 3));
    Map_Run125(a0, 0);
}

/* map/locations/heidia/village/conditional_scene.c */
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gWork[];

void Scene_RunScene3bf3(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Map_Run126();
    if (Map_Check13(0x941) != 0) {
        Map_Do8(0x2566);
        Map_Run127(18, 0);
        Map_Run128();
    } else {
        if (Map_Check14(0x313) != 0) {
            Map_Do9(0x2457);
            Map_Check15(25, 0);
            Map_Run129();
        } else {
            Map_Place26(25, 0x102, 30);
            Map_Run130(25, 0, 0);
            base5_244f = (s32)gVal4;
            Map_Run131(base5_244f);
            Map_Run132(25, 0);
            Map_Run133(25, 24, 0);
            Map_Run134(24, 1);
            Map_Run135();
            Map_Run136(60);
            Map_Run137(0, 1);
            Map_Run55(20);
            Map_Place27(25, 0x105, 60);
            Map_Run138((base5_244f + 1));
            Map_Run139(25, 0);
            Map_Place28(25, 0x107, 60);
            Map_Run140((base5_244f + 2));
            Map_Run141(25, 0);
            Map_Run142(70);
            Map_Place29(25, 0x100, 60);
            Map_Run143(25, 0, 0);
            Map_Run144((base5_244f + 3));
            Map_Check16(25, 0);
            if (Map_Check17(0, 0) == 0) {
                Map_Run145((base5_244f + 4));
                Map_Run146(25, 0);
            } else {
                Map_Run147((base5_244f + 5));
                Map_Run148(25, 0);
            }
            Map_Run149(60);
            Map_Place30(25, 0x105, 60);
            base5_2455 = (s32)gVal5;
            Map_Run150(base5_2455);
            Map_Check18(25, 0);
            Map_Run151(25, 1);
            Map_Run152((base5_2455 + 1));
            Map_Check19(25, 0);
            Map_Run153(25, 3);
            Map_Run154((base5_2455 + 2));
            Map_Run(25, 0);
            Map_Do10(0x313);
            Map_Run155();
        }
    }
}

/* map/locations/heidia/village/main_scene.c */
extern u8 gVal6[];
extern u8 gVal7[];
extern u8 gVal8[];

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_3();

u8 *Motion_CommitPos_22();

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();
u8 *Motion_CommitPos_22();
u8 *Scene_GetRecord_3();
u8 *Scene_GetRecord_4();
u8 *Battle_WaitMode0_92();

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void Scene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set_1(0x301);
    GameFlag_Set_2(0x941);
    Map_SetRect4(16, 75, 7, 4, 26, 55);
    Map_Run156(4);
    Battle_Reset_1();
    record = Scene_GetRecord_1(12);
    Map_Run157(record, 1);
    Motion_SetHPosTerrain_1(0, 0x1c80000, 0xb80000);
    Motion_ArmCb_1(0, 0x4000, 0);
    Motion_SetHPosTerrain_2(1, 0x1b80000, 0xc00000);
    Motion_ArmCb_2(1, 0x4000, 0);
    Motion_SetHPosTerrain_3(3, 0x1e80000, 0xb80000);
    Motion_ArmCb_3(3, 0x4000, 0);
    Motion_SetHPosTerrain_4(2, 0x1d80000, 0xb80000);
    Motion_ArmCb_4(2, 0x4000, 0);
    Motion_SetHPosTerrain_5(12, 0x2080000, 0xe00000);
    Motion_ArmCb_5(12, 0xb000, 0);
    Motion_SetHPosTerrain_6(11, 0x1c00000, 0xed0000);
    Motion_ArmCb_6(11, 0x8000, 0);
    Motion_SetHPosTerrain_7(15, 0x1c00000, 0xee0000);
    Motion_SetActionVariant_1(15, 3);
    Object_SetModeById_1(11, 3);
    Motion_SetHPosTerrain_8(13, 0x1ca0000, 0xf30000);
    Motion_ArmCb_7(13, 0x4000, 0);
    Object_SetModeById_2(13, 5);
    Battle_WaitMode0_1();
    Battle_WaitMode0_2(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)gVal6;
    SceneWork_SetStepValue_1(script_a);
    BattleEv_RunWait_1(13, 0);
    BattleFx_SpawnLinked_1(1, 0x101, 60);
    SceneWork_SetStepValue_2((script_a + 1));
    BattleEv_RunWait_2(1, 0);
    BattleFx_SpawnLinked_2(0, 0x101, 1);
    BattleFx_SpawnLinked_3(2, 0x101, 1);
    BattleFx_SpawnLinked_4(3, 0x101, 1);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_1(12, 13, 0);
    Battle_WaitMode0_4(60);
    SceneWork_SetStepValue_3((script_a + 2));
    BattleEv_RunWait_3(12, 0);
    Motion_SetAngleToward_2(0, 12, 0);
    Motion_SetAngleToward_3(1, 12, 0);
    Motion_SetAngleToward_4(2, 12, 0);
    Motion_SetAngleToward_5(3, 12, 0);
    Battle_WaitMode0_5(60);
    Motion_ResetPosMode2_1(12, 0x200, 232);
    Object_SetModeById_3(12, 4);
    Battle_WaitMode0_6(60);
    SceneWork_SetStepValue_4((script_a + 3));
    BattleEv_RunWait_4(12, 0);
    Battle_WaitMode0_7(15);
    Motion_SetVarCb_1(13, 2);
    SceneWork_SetStepValue_5((script_a + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_SetAngleToward_6(0, 13, 0);
    Motion_SetAngleToward_7(2, 13, 0);
    Motion_SetAngleToward_8(3, 13, 0);
    Motion_SetAngleToward_9(1, 13, 0);
    Motion_SetVarCb_2(2, 1);
    SceneWork_SetStepValue_6((script_a + 5));
    BattleEv_RunWait_6(2, 0);
    BattleFx_SpawnLinked_5(3, 0x105, 0);
    Object_SetModeById_4(3, 4);
    Battle_WaitMode0_8(80);
    Object_SetModeById_5(3, 1);
    SceneWork_SetStepValue_7((script_a + 6));
    BattleEv_RunWait_7(3, 0);
    BattleFx_SpawnLinked_6(12, 0x102, 65);
    Motion_SetVarCb_3(12, 2);
    Battle_WaitMode0_9(100);
    SceneWork_SetStepValue_8((script_a + 7));
    BattleEv_RunWait_8(13, 0);
    Motion_SetAngleToward_10(1, 0, 0);
    Battle_WaitMode0_10(10);
    SceneWork_SetStepValue_9((script_a + 8));
    BattleEv_RunWait_9(1, 0);
    Motion_SetVarCb_4(2, 1);
    Battle_WaitMode0_11(30);
    SceneWork_SetStepValue_10((script_a + 9));
    BattleEv_RunWait_10(2, 0);
    Motion_SetAngleToward_11(3, 0, 0);
    SceneWork_SetStepValue_11((script_a + 10));
    Motion_SetSpeed_1(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Object_SetModeById_6(2, 3);
        Object_SetModeById_7(1, 3);
        Object_SetModeById_8(3, 3);
        Motion_SetAngleToward_12(12, 13, 0);
        Battle_WaitMode0_12(60);
    } else {
        SceneWork_SetStepValue_12((script_a + 11));
        BattleEv_RunWait_11(3, 0);
        Motion_SetAngleToward_13(0, 12, 0);
        Motion_SetAngleToward_14(1, 12, 0);
        Motion_SetAngleToward_15(2, 12, 0);
        Motion_SetAngleToward_16(3, 12, 0);
        Battle_WaitMode0_13(20);
        Motion_SetAngleToward_17(12, 0, 0);
        Battle_WaitMode0_14(60);
        Motion_SetAngleToward_18(12, 13, 0);
        Battle_WaitMode0_15(80);
        Motion_CallWaitAnim_1(12, 3);
        Battle_WaitMode0_16(30);
        Motion_SetAngleToward_19(12, 0, 0);
        SceneWork_SetStepValue_13((script_a + 12));
        BattleEv_RunWait_12(12, 0);
        Battle_WaitMode0_17(60);
        Motion_SetAngleToward_20(2, 12, 0);
        Motion_SetAngleToward_21(1, 12, 0);
        Motion_SetAngleToward_22(3, 12, 0);
        Motion_SetAngleToward_23(0, 12, 0);
        Object_SetModeById_9(2, 3);
        Object_SetModeById_10(1, 3);
        Object_SetModeById_11(3, 3);
        Object_SetModeById_12(0, 3);
        Battle_WaitMode0_18(80);
        Motion_SetAngleToward_24(12, 13, 0);
        Motion_SetAngleToward_25(2, 13, 0);
        Motion_SetAngleToward_26(1, 13, 0);
        Motion_SetAngleToward_27(3, 13, 0);
        Motion_SetAngleToward_28(0, 13, 0);
    }
    Motion_SetSpeed_2(0, 0x8000, 0x4000);
    Motion_SetSpeed_3(1, 0x8000, 0x4000);
    Motion_SetSpeed_4(3, 0x8000, 0x4000);
    Motion_SetSpeed_5(2, 0x8000, 0x4000);
    Motion_ResetPosMode2_2(1, 0x1a0, 216);
    Motion_CommitPos_1(1);
    Motion_ResetPosMode2_3(1, 0x1a0, 248);
    Motion_CommitPos_2(1);
    Motion_ResetPosMode2_4(1, 0x1b8, 248);
    Motion_CommitPos_3(1);
    Motion_ArmCb_8(1, 0xc000, 0);
    Motion_ResetPosMode2_5(0, 0x1b8, 216);
    Motion_CommitPos_4(0);
    Motion_ArmCb_9(0, 0, 0);
    Motion_ResetPosMode2_6(3, 0x1e8, 248);
    Motion_CommitPos_5(3);
    Motion_ResetPosMode2_7(3, 0x1c8, 248);
    Motion_CommitPos_6(3);
    Motion_ArmCb_10(3, 0xc000, 0);
    Motion_ResetPosMode2_8(2, 0x1c8, 216);
    Motion_CommitPos_7(2);
    Motion_ArmCb_11(2, 0x8000, 0);
    Object_LinkPair_1(1, 0, 0);
    Object_LinkPair_2(3, 2, 0);
    Battle_WaitMode0_19(60);
    Motion_CallWaitAnim_2(0, 3);
    Object_SetModeById_13(2, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(3, 3);
    Battle_WaitMode0_20(100);
    Audio_PlayCue_1(226);
    Object_SetModeById_16(13, 7);
    Motion_SetSpeed_6(0, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_1(0, -24, 0);
    Motion_SetSpeed_7(1, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_2(1, -24, 0);
    Motion_SetSpeed_8(3, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_3(3, -24, 0);
    Motion_SetSpeed_9(2, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_4(2, -24, 0);
    Motion_SetSpeed_10(11, 0x1999, 0xccc);
    Motion_SetSpeed_11(15, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_5(11, -24, 0);
    Motion_OffsetPositionAndResetMotion_6(15, -24, 0);
    Motion_CommitPos_8(0);
    Audio_PlayCue_2(0x120);
    Battle_WaitMode0_21(60);
    Motion_ArmCb_12(0, 0x4000, 0);
    Motion_ArmCb_13(1, 0xc000, 0);
    Motion_ArmCb_14(2, 0x4000, 0);
    Motion_ArmCb_15(3, 0xc000, 0);
    Battle_WaitMode0_22(60);
    Object_SetModeById_17(0, 3);
    Object_SetModeById_18(1, 3);
    Object_SetModeById_19(2, 3);
    Object_SetModeById_20(3, 3);
    Battle_WaitMode0_23(100);
    Motion_SetSpeed_12(0, 0xb333, 0x5999);
    Motion_SetSpeed_13(1, 0xb333, 0x5999);
    Motion_SetSpeed_14(2, 0xb333, 0x5999);
    Motion_SetSpeed_15(3, 0xb333, 0x5999);
    Motion_ResetPosMode2_9(0, 0x1c8, 184);
    Motion_CommitPos_9(0);
    Object_SetModeById_21(0, 1);
    Motion_ArmCb_16(0, 0x4000, 0);
    Motion_ResetPosMode2_10(1, 0x1d0, 0x100);
    Motion_CommitPos_10(1);
    Motion_ResetPosMode2_11(1, 0x1e0, 248);
    Motion_CommitPos_11(1);
    Motion_ResetPosMode2_12(1, 0x1b8, 192);
    Motion_CommitPos_12(1);
    Object_SetModeById_22(1, 1);
    Motion_ArmCb_17(1, 0x4000, 0);
    Motion_ResetPosMode2_13(3, 0x1e8, 248);
    Motion_CommitPos_13(3);
    Motion_ResetPosMode2_14(3, 0x1e8, 184);
    Motion_ResetPosMode2_15(2, 0x1d8, 184);
    Motion_CommitPos_14(2);
    Object_SetModeById_23(2, 1);
    Motion_ArmCb_18(2, 0x4000, 0);
    Motion_CommitPos_15(3);
    Object_SetModeById_24(3, 1);
    Motion_ArmCb_19(3, 0x4000, 0);
    Battle_WaitMode0_24(30);
    Battle_WaitMode0_25(60);
    script_b = (s32)gVal7;
    SceneWork_SetStepValue_14(script_b);
    BattleEv_RunWait_13(1, 0);
    Object_SetModeById_25(13, 6);
    Battle_WaitMode0_26(120);
    Motion_SetVarCb_5(13, 2);
    Battle_WaitMode0_27(60);
    Object_SetModeById_26(13, 7);
    SceneWork_SetStepValue_15((script_b + 1));
    BattleEv_RunWait_14(13, 0);
    Battle_WaitMode0_28(20);
    Object_LinkPair_3(3, 2, 0);
    Battle_WaitMode0_29(10);
    BattleFx_SpawnLinked_7(3, 0x101, 80);
    Motion_SetAngleToward_29(3, 13, 0);
    Motion_SetAngleToward_30(2, 13, 0);
    SceneWork_SetStepValue_16((script_b + 2));
    BattleEv_RunWait_15(3, 0);
    BattleFx_SpawnLinked_8(13, 0x102, 70);
    Motion_SetVarCb_6(13, 2);
    Battle_WaitMode0_30(60);
    Object_SetModeById_27(13, 5);
    Battle_WaitMode0_31(70);
    SceneWork_SetStepValue_17((script_b + 3));
    BattleEv_RunWait_16(13, 0);
    Object_SetModeById_28(13, 7);
    BattleFx_SpawnLinked_9(2, 0x108, 40);
    SceneWork_SetStepValue_18((script_b + 4));
    BattleEv_RunWait_17(2, 0);
    Motion_CallWaitAnim_3(12, 3);
    BattleFx_SpawnLinked_10(1, 0x103, 60);
    Motion_ResetPosMode2_16(1, 0x1b8, 208);
    Motion_CommitPos_16(1);
    Motion_SetAngleToward_31(1, 0, 0);
    SceneWork_SetStepValue_19((script_b + 5));
    BattleEv_RunWait_18(1, 0);
    Battle_WaitMode0_32(20);
    BattleFx_SpawnLinked_11(0, 0x101, 0);
    BattleFx_SpawnLinked_12(2, 0x101, 0);
    BattleFx_SpawnLinked_13(3, 0x101, 0);
    BattleFx_SpawnLinked_14(12, 0x101, 0);
    Battle_WaitMode0_33(70);
    Motion_SetAngleToward_32(1, 2, 0);
    BattleFx_SpawnLinked_15(1, 0x102, 75);
    Motion_SetAngleToward_33(1, 0, 0);
    Object_SetModeById_29(1, 4);
    SceneWork_SetStepValue_20((script_b + 6));
    BattleEv_RunWait_19(1, 0);
    BattleFx_SpawnLinked_16(12, 0x101, 0);
    Battle_WaitMode0_34(60);
    SceneWork_SetStepValue_21((script_b + 7));
    BattleEv_RunWait_20(12, 0);
    Motion_ResetPosMode2_17(1, 0x1c0, 208);
    Motion_CommitPos_17(1);
    Object_SetModeById_30(1, 1);
    Motion_SetAngleToward_34(1, 12, 0);
    SceneWork_SetStepValue_22((script_b + 8));
    BattleEv_RunWait_21(1, 0);
    Motion_SetVarCb_7(12, 1);
    Battle_WaitMode0_35(60);
    Motion_SetAngleToward_35(1, 12, 0);
    Battle_WaitMode0_36(60);
    SceneWork_SetStepValue_23((script_b + 9));
    BattleEv_RunWait_22(12, 0);
    Object_SetModeById_31(1, 4);
    Battle_WaitMode0_37(60);
    SceneWork_SetStepValue_24((script_b + 10));
    BattleEv_RunWait_23(1, 0);
    Motion_SetVarCb_8(2, 1);
    Battle_WaitMode0_38(30);
    SceneWork_SetStepValue_25((script_b + 11));
    BattleEv_RunWait_24(2, 0);
    Motion_SetAngleToward_36(1, 3, 0);
    Battle_WaitMode0_39(20);
    SceneWork_SetStepValue_26((script_b + 12));
    BattleEv_RunWait_25(1, 0);
    Battle_WaitMode0_40(30);
    BattleFx_SpawnLinked_17(3, 0x100, 80);
    Motion_SetAngleToward_37(3, 1, 0);
    SceneWork_SetStepValue_27((script_b + 13));
    BattleEv_RunWait_26(3, 0);
    Motion_SetAngleToward_38(3, 13, 0);
    SceneWork_SetStepValue_28((script_b + 14));
    BattleEv_RunWait_27(3, 0);
    Motion_SetAngleToward_39(0, 13, 0);
    Motion_SetAngleToward_40(2, 13, 0);
    Motion_SetAngleToward_41(1, 13, 0);
    Motion_SetAngleToward_42(3, 13, 0);
    Battle_WaitMode0_41(120);
    BattleFx_SpawnLinked_18(13, 0x102, 30);
    Motion_SetVarCb_9(13, 1);
    Battle_WaitMode0_42(120);
    Motion_SetVarCb_10(12, 1);
    Battle_WaitMode0_43(60);
    SceneWork_SetStepValue_29((script_b + 15));
    BattleEv_RunWait_28(12, 0);
    BattleFx_SpawnLinked_19(1, 0x107, 110);
    Motion_ResetPosMode2_18(1, 0x1c8, 212);
    Motion_CommitPos_18(1);
    Object_SetModeById_32(1, 1);
    SceneWork_SetStepValue_30((script_b + 16));
    BattleEv_RunWait_29(1, 0);
    Motion_SetAngleToward_43(2, 1, 0);
    Battle_WaitMode0_44(60);
    Motion_SetVarCb_11(2, 1);
    SceneWork_SetStepValue_31((script_b + 17));
    BattleEv_RunWait_30(2, 0);
    Motion_SetAngleToward_44(1, 13, 0);
    Battle_WaitMode0_45(80);
    Motion_SetAngleToward_45(1, 2, 0);
    Battle_WaitMode0_46(60);
    Motion_SetAngleToward_46(1, 13, 0);
    Battle_WaitMode0_47(30);
    Motion_SetAngleToward_47(0, 13, 0);
    Motion_SetAngleToward_48(3, 13, 0);
    Motion_SetAngleToward_49(13, 13, 0);
    Motion_SetAngleToward_50(12, 13, 0);
    Battle_WaitMode0_48(80);
    Motion_CallWaitAnim_4(1, 3);
    Battle_WaitMode0_49(30);
    SceneWork_SetStepValue_32((script_b + 18));
    BattleEv_RunWait_31(1, 0);
    Motion_SetVarCb_12(13, 2);
    Battle_WaitMode0_50(70);
    SceneWork_SetStepValue_33((script_b + 19));
    BattleEv_RunWait_32(13, 0);
    BattleFx_SpawnLinked_20(3, 0x100, 60);
    SceneWork_SetStepValue_34((script_b + 20));
    BattleEv_RunWait_33(3, 0);
    Motion_SetAngleToward_51(2, 0, 0);
    Battle_WaitMode0_51(80);
    SceneWork_SetStepValue_35((script_b + 21));
    Motion_SetSpeed_16(2, 0);
    Motion_SetAngleToward_52(1, 0, 0);
    Motion_SetAngleToward_53(3, 0, 0);
    Motion_SetAngleToward_54(2, 0, 0);
    Motion_SetAngleToward_55(12, 0, 0);
    Motion_SetHPosTerrain_9(14, 0x1c80000, 0x1300000);
    Motion_SetSpeed_17(14, 0x8000, 0x4000);
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        Audio_PlayCue_3(19);
        SceneWork_SetStepValue_36((script_b + 22));
        BattleEv_RunWait_34(14, 0);
    } else {
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_37((script_b + 23));
        BattleEv_RunWait_35(14, 0);
    }
    Motion_SetAngleToward_56(0, 14, 0);
    Motion_SetAngleToward_57(1, 14, 0);
    Motion_SetAngleToward_58(3, 14, 0);
    Motion_SetAngleToward_59(2, 14, 0);
    Motion_SetAngleToward_60(12, 14, 0);
    Motion_SetAngleToward_61(13, 14, 0);
    Motion_CamBounds_1(0x1c80000, -1, 0xf00000, 1);
    Motion_SetVarCb_13(13, 1);
    Battle_WaitMode0_52(60);
    Audio_PlayCue_5(8);
    script_c = (s32)gVal8;
    SceneWork_SetStepValue_38(script_c);
    BattleEv_RunWait_36(13, 0);
    Motion_ResetPosMode2_19(14, 0x1c8, 0x118);
    Motion_CommitPos_19(14);
    Motion_ResetPosMode2_20(14, 0x1b8, 0x100);
    Motion_CamBounds_2(0x1c80000, -1, 0xe00000, 1);
    SceneWork_SetStepValue_39((script_c + 1));
    BattleEv_RunWait_37(14, 0);
    Motion_CommitPos_20(14);
    Motion_ArmCb_20(14, 0xd000, 0);
    BattleFx_SpawnLinked_21(13, 0x102, 80);
    Object_SetModeById_33(14, 4);
    Battle_WaitMode0_53(89);
    SceneWork_SetStepValue_40((script_c + 2));
    BattleEv_RunWait_38(14, 0);
    Motion_SetVarCbObj_1(13, 2);
    Motion_CallWaitAnim_5(14, 3);
    SceneWork_SetStepValue_41((script_c + 3));
    BattleEv_RunWait_39(14, 0);
    BattleFx_SpawnLinked_22(13, 0x100, 80);
    Object_SetModeById_34(14, 4);
    Battle_WaitMode0_54(80);
    Motion_ArmCb_21(14, 0x3000, 0);
    Battle_WaitMode0_55(20);
    SceneWork_SetStepValue_42((script_c + 4));
    BattleEv_RunWait_40(14, 0);
    Motion_SetVarCb_14(13, 1);
    SceneWork_SetStepValue_43((script_c + 5));
    BattleEv_RunWait_41(13, 0);
    Battle_WaitMode0_56(30);
    Motion_CallWaitAnim_6(14, 3);
    Motion_SetAngleToward_62(14, 13, 0);
    Battle_WaitMode0_57(20);
    SceneWork_SetStepValue_44((script_c + 6));
    BattleEv_RunWait_42(14, 0);
    Motion_SetVarCb_15(13, 1);
    BattleFx_SpawnLinked_23(13, 0x102, 80);
    SceneWork_SetStepValue_45((script_c + 7));
    BattleEv_RunWait_43(13, 0);
    BattleFx_SpawnLinked_24(14, 0x103, 60);
    SceneWork_SetStepValue_46((script_c + 8));
    BattleEv_RunWait_44(14, 0);
    Motion_CallWaitAnim_7(14, 3);
    SceneWork_SetStepValue_47((script_c + 9));
    BattleEv_RunWait_45(14, 0);
    Battle_WaitMode0_58(20);
    Battle_WaitMode0_59(13, 0x101);
    Battle_WaitMode0_60(80);
    Battle_WaitMode0_61(13, 0);
    Motion_ResetPosMode2_21(14, 0x1f0, 240);
    Motion_CommitPos_21(14);
    Motion_SetAngleToward_63(14, 12, 0);
    Battle_WaitMode0_62(20);
    Motion_SetAngleToward_64(12, 14, 0);
    SceneWork_SetStepValue_48((script_c + 10));
    BattleEv_RunWait_46(14, 0);
    Motion_SetVarCb_16(12, 1);
    SceneWork_SetStepValue_49((script_c + 11));
    BattleEv_RunWait_47(12, 0);
    Battle_WaitMode0_63(40);
    Motion_CallWaitAnim_8(14, 3);
    Battle_WaitMode0_64(20);
    SceneWork_SetStepValue_50((script_c + 12));
    BattleEv_RunWait_48(14, 0);
    Object_SetModeById_35(14, 3);
    BattleFx_SpawnLinked_25(12, 0x102, 60);
    SceneWork_SetStepValue_51((script_c + 13));
    BattleEv_RunWait_49(12, 0);
    BattleFx_SpawnLinked_26(14, 0x100, 70);
    SceneWork_SetStepValue_52((script_c + 14));
    BattleEv_RunWait_50(14, 0);
    Object_SetModeById_36(12, 3);
    Battle_WaitMode0_65(140);
    Object_SetModeById_37(14, 3);
    Battle_WaitMode0_66(120);
    Object_LinkPair_4(0, 14, 0);
    Object_SetModeById_38(0, 3);
    Object_SetModeById_39(2, 3);
    Object_SetModeById_40(1, 3);
    Object_SetModeById_41(3, 3);
    Battle_WaitMode0_67(120);
    BattleFx_SpawnLinked_27(14, 0x108, 180);
    SceneWork_SetStepValue_53((script_c + 15));
    BattleEv_RunWait_51(14, 0);
    BattleFx_SpawnLinked_28(12, 0x101, 80);
    SceneWork_SetStepValue_54((script_c + 16));
    BattleEv_RunWait_52(12, 0);
    Object_LinkPair_5(0, 1, 0);
    Object_LinkPair_6(3, 2, 0);
    Battle_WaitMode0_68(80);
    Motion_SetAngleToward_65(14, 0, 0);
    Battle_WaitMode0_69(60);
    Motion_SetAngleToward_66(0, 14, 0);
    Motion_SetAngleToward_67(1, 14, 0);
    Motion_SetAngleToward_68(2, 14, 0);
    Motion_SetAngleToward_69(3, 14, 0);
    Battle_WaitMode0_70(60);
    SceneWork_SetStepValue_55((script_c + 17));
    BattleEv_RunWait_53(14, 0);
    BattleFx_SpawnLinked_29(0, 0x101, 0);
    BattleFx_SpawnLinked_30(1, 0x101, 0);
    BattleFx_SpawnLinked_31(2, 0x101, 0);
    BattleFx_SpawnLinked_32(3, 0x101, 0);
    Battle_WaitMode0_71(100);
    SceneWork_SetStepValue_56((script_c + 18));
    BattleEv_RunWait_54(1, 0);
    Motion_SetVarCb_17(2, 1);
    Battle_WaitMode0_72(60);
    SceneWork_SetStepValue_57((script_c + 19));
    BattleEv_RunWait_55(2, 0);
    Motion_SetAngleToward_70(14, 2, 0);
    Battle_WaitMode0_73(20);
    SceneWork_SetStepValue_58((script_c + 20));
    BattleEv_RunWait_56(14, 0);
    BattleFx_SpawnLinked_33(0, 0x101, 0);
    BattleFx_SpawnLinked_34(1, 0x101, 0);
    BattleFx_SpawnLinked_35(2, 0x101, 0);
    BattleFx_SpawnLinked_36(3, 0x101, 0);
    Battle_WaitMode0_74(120);
    Object_SetModeById_42(14, 4);
    Battle_WaitMode0_75(120);
    SceneWork_SetStepValue_59((script_c + 21));
    BattleEv_RunWait_57(14, 0);
    BattleFx_SpawnLinked_37(14, 0x102, 90);
    SceneWork_SetStepValue_60((script_c + 22));
    BattleEv_RunWait_58(14, 0);
    Object_SetModeById_43(0, 3);
    Object_SetModeById_44(2, 3);
    Object_SetModeById_45(3, 3);
    Object_SetModeById_46(1, 3);
    Object_SetModeById_47(12, 3);
    Battle_WaitMode0_76(80);
    Object_SetModeById_48(14, 4);
    Battle_WaitMode0_77(120);
    SceneWork_SetStepValue_61((script_c + 23));
    BattleEv_RunWait_59(14, 0);
    BattleFx_SpawnLinked_38(12, 0x100, 60);
    SceneWork_SetStepValue_62((script_c + 24));
    BattleEv_RunWait_60(12, 0);
    Motion_SetAngleToward_71(14, 12, 0);
    Battle_WaitMode0_78(20);
    Motion_SetVarCb_18(14, 1);
    Battle_WaitMode0_79(50);
    SceneWork_SetStepValue_63((script_c + 25));
    BattleEv_RunWait_61(14, 0);
    Object_SetModeById_49(1, 3);
    Battle_WaitMode0_80(70);
    SceneWork_SetStepValue_64((script_c + 26));
    BattleEv_RunWait_62(1, 0);
    Object_SetModeById_50(3, 3);
    SceneWork_SetStepValue_65((script_c + 27));
    BattleEv_RunWait_63(3, 0);
    Motion_SetAngleToward_72(14, 0, 0);
    Motion_CallWaitAnim_9(14, 3);
    Battle_WaitMode0_81(20);
    SceneWork_SetStepValue_66((script_c + 28));
    BattleEv_RunWait_64(14, 0);
    BattleFx_SpawnLinked_39(2, 0x101, 90);
    SceneWork_SetStepValue_67((script_c + 29));
    BattleEv_RunWait_65(2, 0);
    Motion_ArmCb_22(14, 0x5000, 0);
    Battle_WaitMode0_82(20);
    SceneWork_SetStepValue_68((script_c + 30));
    BattleEv_RunWait_66(14, 0);
    Motion_SetVarCb_19(12, 1);
    Battle_WaitMode0_83(60);
    SceneWork_SetStepValue_69((script_c + 31));
    BattleEv_RunWait_67(12, 0);
    Motion_SetAngleToward_73(14, 12, 0);
    Battle_WaitMode0_84(20);
    Motion_CallWaitAnim_10(14, 3);
    Battle_WaitMode0_85(20);
    SceneWork_SetStepValue_70((script_c + 32));
    BattleEv_RunWait_68(14, 0);
    BattleFx_SpawnLinked_40(1, 0x100, 70);
    SceneWork_SetStepValue_71((script_c + 33));
    BattleEv_RunWait_69(1, 0);
    Motion_SetAngleToward_74(14, 0, 0);
    Battle_WaitMode0_86(30);
    SceneWork_SetStepValue_72((script_c + 34));
    BattleEv_RunWait_70(14, 0);
    BattleFx_SpawnLinked_41(3, 0x100, 80);
    SceneWork_SetStepValue_73((script_c + 35));
    BattleEv_RunWait_71(3, 0);
    Motion_CallWaitAnim_11(14, 3);
    SceneWork_SetStepValue_74((script_c + 36));
    BattleEv_RunWait_72(14, 0);
    Motion_SetAngleToward_75(2, 0, 0);
    Battle_WaitMode0_87(30);
    Object_SetModeById_51(2, 3);
    Battle_WaitMode0_88(100);
    SceneWork_SetStepValue_75((script_c + 37));
    Motion_SetSpeed_18(2, 0);
    if (UiWork_WaitThenFinalizeCapacity_3(0, 0) != 0) {
        SceneWork_SetStepValue_76((script_c + 38));
        BattleEv_RunWait_73(14, 0);
    }
    Battle_WaitMode0_89(30);
    Object_SetModeById_52(1, 3);
    Object_SetModeById_53(2, 3);
    Object_SetModeById_54(3, 3);
    Battle_WaitMode0_90(100);
    Object_LinkPair_7(0, 12, 0);
    Object_SetModeById_55(0, 3);
    Object_SetModeById_56(12, 3);
    Battle_WaitMode0_91(100);
    Object_SetModeById_57(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_22(1);
    Motion_SetHPosTerrain_10(1, 0, 0);
    Object_SetModeById_58(2, 2);
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_23(2);
    Motion_SetHPosTerrain_11(2, 0, 0);
    Object_SetModeById_59(3, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_24(3);
    Motion_SetHPosTerrain_12(3, 0, 0);
    Battle_WaitMode0_92(30);
    Motion_SetSpeed_19(12, 0x6666, 0x3333);
    Motion_ResetPosMode2_22(12, 0x1d8, 184);
    Motion_CommitPos_25(12);
    Object_SetModeById_60(12, 1);
    Motion_ArmCb_23(12, 0x3000, 0);
    Motion_SetAngleToward_76(14, 0, 0);
    Object_SetModeById_61(14, 3);
    Battle_WaitMode0_93(20);
    Motion_SetAngleToward_77(14, 13, 0);
    Battle_WaitMode0_94(20);
    SceneWork_SetStepValue_77((script_c + 39));
    BattleEv_RunWait_74(14, 0);
    Motion_SetVarCbObj_2(13, 2);
    SceneWork_SetStepValue_78((script_c + 40));
    BattleEv_RunWait_75(13, 0);
    Object_SetModeById_62(0, 3);
    Battle_WaitMode0_95(30);
    Motion_SetSpeed_20(0, 0x10000, 0x8000);
    Motion_SetSpeed_21(12, 0x10000, 0x8000);
    Motion_SetPosReset_1(0, 1);
    Motion_ResetPosMode2_23(0, 0x1e0, 248);
    Battle_WaitMode0_96(40);
    Motion_SetActionVariant_2(12, 0);
    Motion_SetActionVariant_3(0, 0);
    Motion_ResetPosMode2_24(12, 0x1e0, 216);
    Motion_CommitPos_26(0);
    Motion_SetAngleToward_78(0, 14, 0);
    Motion_CommitPos_27(12);
    Motion_SetAngleToward_79(12, 14, 0);
    Motion_ArmCb_24(12, 0x3000, 0);
    Object_SetModeById_63(0, 3);
    Battle_WaitMode0_97(5);
    Object_SetModeById_64(12, 3);
    Battle_WaitMode0_98(100);
    Object_SetModeById_65(14, 3);
    Battle_WaitMode0_99(100);
    Motion_ResetPosMode2_25(12, 0x1e0, 248);
    Motion_ResetPosMode2_26(0, 0x1c8, 248);
    Motion_CommitPos_28(12);
    Motion_ResetPosMode2_27(12, 0x1c8, 248);
    Motion_CommitPos_29(0);
    Motion_ResetPosMode2_28(0, 0x1c8, 0x168);
    Motion_CommitPos_30(12);
    Motion_ResetPosMode2_29(12, 0x1c8, 0x168);
    Motion_CommitPos_31(0);
    Motion_ResetPosMode2_30(0, 0x160, 0x168);
    Motion_CommitPos_32(12);
    Motion_ResetPosMode2_31(12, 0x160, 0x168);
    Battle_WaitMode0_100(20);
    Audio_PlayCue_6(17);
    SCENE_PHASE = 0x203;
    Map_Run158();
    Battle_WaitMode0_101(1);
    Battle_WaitMode0_102(210);
    Map_Run159(4);
    Battle_SchedShoulder_1();
}

/* map/locations/heidia/village/party_animation.c */
extern u8 gWork[];

void Map_Run160(); void Map_Run161();

void Scene_RunScene3bf4(void)
{
    u32 i;
    s32 record;

    Map_Place31(2, 0x10000, 0x8000);
    Map_Place32(2, 0x1f8, 216);
    Map_Place33(3, 0x10000, 0x8000);
    Map_Place34(3, 0x1b8, 232);
    Map_Place35(1, 0x10000, 0x8000);
    Map_Place36(1, 0x1e0, 224);
    Map_Run162(1);
    Map_Place37(1, 0xc000, 0);
    Map_Run163(2);
    Map_Place38(2, 0xc000, 0);
    Map_Run164(3);
    Map_Place39(3, 0xc000, 0);
    Map_Place40(0, 0xc000, 0);
}

/* map/locations/heidia/village/scene_party_placement.c */
extern u8 gWork[];

void Scene_RunScene3bf5(void)
{
    u32 i;
    s32 record;

    record = Map_Check20(0);
    if (record != 0) {
        Map_Run165(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Map_Check21(0);
    if (record != 0) {
        Map_Run166(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Map_Check22(0);
    if (record != 0) {
        Map_Run167(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Map_Run168(0, 0, 0);
    Map_Place41(2, 0xb333, 0x5999);
    Map_Place42(2, 0x1c8, 192);
    Map_Place43(3, 0xb333, 0x5999);
    Map_Place44(3, 0x1b8, 184);
    Map_Place45(1, 0xb333, 0x5999);
    Map_Place46(1, 0x1c0, 240);
    Map_Run169(2);
    Map_Run170(2, 12, 0);
    Map_Run171(1);
    Map_Run172(3);
    Map_Run173(1, 12, 0);
    Map_Run174(3, 12, 0);
    Map_Run175(15);
}

/* map/locations/heidia/village/scene_sequence.c */
/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after Motion_SetAngleToward_2(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */
#define PRIMARY_ID 24
#define DERIVED_ID 25

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void Scene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet_1(2369) != 0) {
        if (GameFlag_IsSet_2(2382) == 0 && GameFlag_IsSet_3(788) == 0) {
        sequence_id = 9569;
        SceneWork_SetStepValue_1(sequence_id);
        BattleEv_RunWait_1(PRIMARY_ID, 0);
        Motion_SetVarCbObj_1(PRIMARY_ID, 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeed_1(PRIMARY_ID, 6553, 3276);
        Motion_OffsetPositionAndResetMotion_1(PRIMARY_ID, -4, 0);
        Motion_CommitPos_1(PRIMARY_ID);
        Object_SetModeById_1(PRIMARY_ID, 3);
        Battle_WaitMode0_2(60);
        Motion_SetSpeed_2(PRIMARY_ID, 13107, 6553);
        Motion_OffsetPositionAndResetMotion_2(PRIMARY_ID, -6, 0);
        Motion_SetAngleToward_1(PRIMARY_ID, 0, 0);
        Motion_CommitPos_2(PRIMARY_ID);
        SceneWork_SetStepValue_2(sequence_id + 1);
        BattleEv_RunWait_2(PRIMARY_ID, 0);
        Motion_SetVarCbObj_2(PRIMARY_ID, 1);
        Motion_SetAngleToward_2(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3(sequence_id + 2);
        BattleEv_RunWait_3(PRIMARY_ID, 0);
        Battle_WaitMode0_3(70);
        Object_SetModeById_2(DERIVED_ID, 3);
        Battle_WaitMode0_4(60);
        Motion_SetSpeed_3(DERIVED_ID, 26214, 13107);
        Motion_ResetPosMode2_1(DERIVED_ID, 880, 112);
        Motion_CommitPos_3(DERIVED_ID);
        Motion_ArmCb_1(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4(sequence_id + 3);
        BattleEv_RunWait_4(PRIMARY_ID, 0);
        Object_SetModeById_3(PRIMARY_ID, 3);
        Battle_WaitMode0_5(70);
        Motion_OffsetPositionAndResetMotion_3(PRIMARY_ID, 8, 0);
        Motion_CommitPos_4(PRIMARY_ID);
        Object_SetModeById_4(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5(sequence_id + 4);
        BattleEv_RunWait_5(PRIMARY_ID, 0);
        Motion_ResetPosMode2_2(0, 896, 120);
        Motion_CommitPos_5(0);
        Object_LinkPair_1(0, DERIVED_ID, 0);
        Battle_WaitMode0_6(60);
        Object_SetModeById_5(DERIVED_ID, 3);
        Battle_WaitMode0_7(30);
        GameFlag_Set_1(788);
        } else {
            SceneWork_SetStepValue_6(9575);
            BattleEv_RunWait_6(PRIMARY_ID, 0);
        }
    } else {
        SceneWork_SetStepValue_7(0x244d);
        BattleEv_RunWait_7(PRIMARY_ID, 0);
    }
}

/* map/locations/heidia/village/scene_transition.c */
extern u8 gWork[];

void Scene_RunScene3bf6(void)
{
    u32 i;
    s32 record;

    Map_Place47(0x40000, 0x40000, 0x10000);
    Map_Run176(141);
    Map_Run177(80);
    Map_Do11(0x120);
    Map_Run178(5);
    Map_Run179(145);
    Map_SetRect5(16, 75, 7, 4, 26, 55);
    Map_Place48(-1, -1, 0xe666);
    Map_Place49(0, 0x100, 0);
    Map_Place50(1, 0x100, 0);
    Map_Place51(2, 0x100, 0);
    Map_Place52(3, 0x100, 0);
    Map_Place53(12, 0x100, 0);
    Map_Run180(60);
}

/* map/locations/heidia/village/supplemental_scene.c */
extern u8 gWork[];
extern u8 gVal9[];
extern u8 gVal10[];
extern u8 gCell[];

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at gVal10 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at gCell
 * before handing off to the next step. */
void Scene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;
    s32 sequence_2438;
    s32 status_record_2000240;

    Object_SetModeById_1(0, 1);
    Object_SetModeById_2(12, 1);
    Object_SetModeById_3(13, 1);
    Object_SetModeById_4(14, 1);
    Audio_PlayCue_1(113);
    BattleFx_SpawnLinked_1(12, 0x100, 0);
    Battle_WaitMode0_1(30);
    Motion_SetAngleToward_1(12, 0, 0);
    sequence_2438 = (s32)gVal10;
    SceneWork_SetStepValue_1(sequence_2438);
    BattleEv_RunWait_1(12, 0);
    BattleFx_SpawnLinked_2(13, 0x100, 0);
    BattleFx_SpawnLinked_3(14, 0x100, 0);
    Motion_SetAngleToward_2(13, 0, 0);
    Motion_SetAngleToward_3(14, 0, 0);
    Motion_SetAngleToward_4(0, 13, 0);
    Battle_WaitMode0_2(65);
    Motion_ArmCb_1(13, 0x5000, 0);
    Motion_ArmCb_2(14, 0xd000, 0);
    SceneWork_SetStepValue_2((sequence_2438 + 1));
    BattleEv_RunWait_2(13, 0);
    Motion_CallWaitAnim_1(14, 3);
    SceneWork_SetStepValue_3((sequence_2438 + 2));
    BattleEv_RunWait_3(14, 0);
    SceneWork_SetStepValue_4((sequence_2438 + 3));
    BattleEv_RunWait_4(12, 0);
    Motion_SetVarCbObj_1(13, 1);
    SceneWork_SetStepValue_5((sequence_2438 + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_CallWaitAnim_2(14, 3);
    SceneWork_SetStepValue_6((sequence_2438 + 5));
    BattleEv_RunWait_6(14, 0);
    Motion_CallWaitAnim_3(14, 3);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_5(13, 0, 0);
    Motion_SetAngleToward_6(14, 0, 0);
    Battle_WaitMode0_4(70);
    Motion_ResetPosMode2_1(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    Motion_CommitPos_1(12);
    Motion_SetAngleToward_7(12, 0, 0);
    Motion_CallWaitAnim_4(12, 3);
    Battle_WaitMode0_5(30);
    SceneWork_SetStepValue_7((sequence_2438 + 6));
    BattleEv_RunWait_7(12, 0);
    SCENE_PHASE = 0x200;
    SharedWorkData_SetFirstAndSecondFields_1((s32)gVal9, 31);
    status_record_2000240 = (s32)gCell;
    /* Status byte at +0x22b of the record. */
    *(u8 *)((status_record_2000240 + 0x22b)) = 3;
    BattleFx_ComputeWeightedResultAndDispatch_1(98, 3);
    Motion_SetHPosTerrain_1(12, 0, 0);
    Motion_SetHPosTerrain_2(13, 0, 0);
    Motion_SetHPosTerrain_3(14, 0, 0);
    Battle_SchedShoulder_1();
    GameFlag_Set_1(0x94a); /* main:080770c8 */
}
