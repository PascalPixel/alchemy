#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/flag_branched/flag_branched.h"
#include "facing_object.h"

/* overlays/scene/story/flag_branched/conditional_scene_setup.c */

void Scene_RunStepWithValue174d(void)
{
    Story_Run21();
    Story_Apply(0x174D, 1);
    Story_Run22();
}

void Scene_RunStepWithValue174e(void)
{
    Story_Run23();
    Story_Apply2(0x174E, 1);
    Story_Run24();
}

void Scene_RunBranchedStep(void)
{
    if (Story_Check36() != 0) {
        Story_Run25();
    } else {
        Story_Run26();
    }
}

void Scene_RunStep210ByFlag84e(void)
{
    if (Story_Check37(0x84E) != 0) {
        Story_Do15(0x210);
    } else {
        Story_Place154(21, 182, 0x210);
    }
}

void Scene_RunStep211ByFlag84e(void)
{
    if (Story_Check38(0x84E) != 0) {
        Story_Do16(0x211);
    } else {
        Story_Place155(22, 183, 0x211);
    }
}

void Scene_RunStep212ByFlag84e(void)
{
    if (Story_Check39(0x84E) != 0) {
        Story_Do17(0x212);
    } else {
        Story_Place156(23, 186, 0x212);
    }
}

void Scene_RunStep213ByFlag84e(void)
{
    if (Story_Check40(0x84E) != 0) {
        Story_Do18(0x213);
    } else {
        Story_Place157(24, 189, 0x213);
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
extern volatile SceneEndFunc Story_Run27;

void Scene_RunLongBranchingChoreography(void)
{
    s32 record;
    u8 *work;
    s32 base5_200a5c0;

    Story_Run28();
    Story_Run(-1, -1, -1, 0);
    Story_Run29(1);
    *(u8 *)(Story_Check41() + 85) = 0;
    Story_Place(0x37e0000, -1, 0x2980000);
    Story_Run30(1);
    Story_Run31(0, 0, 0);
    if (Story_Check(0x85f) != 0) {
        Story_Run2(0x37e0000, -1, 0x2ba0000, 0);
        Story_Place2(19, 0x36c0000, 0x27a0000);
        Story_Place3(19, 0x3000, 0);
        Story_Place4(0, 0x37e0000, 0x31e0000);
    }
    Story_Run32();
    Story_Run33(1);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 40;
    Story_Run34();
    Story_Run35();
    if (Story_Check2(0x85f) != 0) {
    } else {
        Story_Run36(80);
        Story_Place5(19, 0x37e0000, 0x31e0000);
        Story_Run3(0x9999, 0x1333);
        Story_Run4(0x37e0000, -1, 0x2ba0000, 1);
        Story_Place6(19, 0xcccc, 0x6666);
        Story_Place7(19, 0x37e, 0x2b8);
        Story_Run37(80);
        Story_Run5(0x37e0000, -1, 0x2980000, 1);
        Story_Run38(19);
        Story_Place8(19, 0x34a, 0x2b8);
        Story_Place9(19, 0x34a, 0x27c);
        Story_Place10(18, 0x7000, 20);
        Story_Place11(19, 0x36c, 0x27a);
        Story_Run39(19, 3);
        Story_Run40(20);
        Story_Run41(18, 3);
        Story_Run42(10);
        Story_Do(0x1437);
        Story_Place12(0x2012, 0, 10);
        Story_Run43(19, 2);
        Story_Run44(19, 0, 20);
        Story_Run45(18, 1);
        Story_Place13(0x2012, 0, 10);
        Story_Run46(19, 3);
        Story_Run47(40);
        Story_Place14(18, 0x105, 60);
        Story_Run6(0x2012, 0);
        Story_Run48(18, 1);
        Story_Place15(0x2012, 0, 10);
        Story_Run7(19, 0x102);
        Story_Run49(60);
        Story_Place16(19, 0x3000, 10);
        Story_Place17(18, 0x5000, 10);
        Story_Run8(0x37e0000, -1, 0x2ba0000, 1);
        Story_Place18(0, 0x37e0000, 0x31e0000);
        Story_Place19(0, 0x9999, 0x4ccc);
        Story_Place20(0, 0x37e, 0x2d6);
        Story_Run50(20);
        Story_Run51(0, 3);
        Story_Run52(10);
        Story_Run53(18, 1);
        Story_Run9(0x2012, 0);
        Story_Run10(0x37e0000, -1, 0x2980000, 1);
        Story_Place21(0, 0x37e, 0x2ac);
        record = Story_Check3(0);
        if (record != 0) {
            Story_Run54(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        record = Story_Check4(0);
        if (record != 0) {
            Story_Run55(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        if (Story_Check5(3) != 0) {
            record = Story_Check6(0);
            if (record != 0) {
                Story_Run56(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
        }
        Story_Place22(1, 0x9999, 0x4ccc);
        Story_Place23(2, 0x9999, 0x4ccc);
        Story_Place24(3, 0x10000, 0x8000);
        Story_Run57(1, 2);
        Story_Run58(2, 2);
        Story_Run59(3, 2);
        Story_Place25(1, -16, 16);
        Story_Run60(2, 16, 16);
        if (Story_Check7(3) != 0) {
            Story_Run61(3, 32, 16);
        }
        Story_Run62(2);
        Story_Run63(1, 1);
        Story_Run64(2, 1);
        Story_Run65(3, 1);
        Story_Run66(10);
        Story_Place26(3, 0xc000, 0);
        Story_Place27(1, 0xc000, 0);
        Story_Place28(2, 0xc000, 20);
        Story_Run67(18, 2, 20);
        Story_Place29(18, 0x7000, 10);
        Story_Place30(0x2012, 0, 10);
        Story_Place31(19, 0x1000, 10);
        ((void (*)())Story_Check42)(19, 3);
        Story_Place32(18, 0x5000, 40);
        Story_Place33(18, 0x7000, 10);
        Story_Run68(18, 4);
        Story_Place34(0x2012, 0, 10);
        Story_Run11(19, 0x102);
        Story_Run69(40);
        Story_Place35(18, 0x5000, 20);
        Story_Place36(18, 0x105, 40);
        Story_Check8(0x2012, 0);
        Story_Place37(1, 0xe000, 0);
        Story_Place38(2, 0xa000, 0);
        if (Story_Check9(0, 0) == 0) {
            goto L_02000f86;
        }
        L_02000cb6:;
        Story_Run70((s32)gVal);
        Story_Place39(0x2012, 0, 10);
        Story_Place40(19, 0x3000, 0);
        Story_Run71(18, 4);
        Story_Place41(0x2012, 0, 10);
        Story_Place42(1, 0xc000, 0);
        Story_Place43(2, 0xc000, 0);
        Story_Run72(0, 3);
        Story_Run73(1, 2);
        record = Story_Check10(0);
        if (record != 0) {
            Story_Run74(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Story_Run75(2, 2);
        record = Story_Check11(0);
        if (record != 0) {
            Story_Run76(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Story_Check12(3) != 0) {
            Story_Run77(3, 2);
            record = Story_Check13(0);
            if (record != 0) {
                Story_Run78(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Story_Run79(2);
        Story_Run80(1, 0, 0);
        Story_Run81(2, 0, 0);
        Story_Run82(3, 0, 0);
        Story_Do2(0x85f);
        Story_Place44(0, 0x10000, 0x8000);
        Story_Place45(0, 0x37e, 0x2f0);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
        Story_Run83();
        Story_Run84();
        Story_Run27();
    }
    Story_Place46(0, 0x9999, 0x4ccc);
    Story_Place47(0, 0x37e, 0x2ac);
    Story_Run85(80);
    Story_Run12(0x9999, 0x1333);
    Story_Run13(0x37e0000, -1, 0x2980000, 1);
    Story_Run86(0);
    Story_Run87(0, 1);
    record = Story_Check14(0);
    if (record != 0) {
        Story_Run88(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Story_Check15(0);
    if (record != 0) {
        Story_Run89(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    if (Story_Check16(3) != 0) {
        record = Story_Check17(0);
        if (record != 0) {
            Story_Run90(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
    }
    Story_Place48(1, 0x9999, 0x4ccc);
    Story_Place49(2, 0x9999, 0x4ccc);
    Story_Place50(3, 0x10000, 0x8000);
    Story_Run91(1, 2);
    Story_Run92(2, 2);
    Story_Run93(3, 2);
    Story_Place51(1, -16, 16);
    Story_Run94(2, 16, 16);
    if (Story_Check18(3) != 0) {
        Story_Run95(3, 32, 16);
    }
    Story_Run96(2);
    Story_Run97(1, 1);
    Story_Run98(2, 1);
    Story_Run99(3, 1);
    Story_Run100(10);
    Story_Place52(3, 0xc000, 0);
    Story_Place53(1, 0xc000, 0);
    Story_Place54(2, 0xc000, 20);
    Story_Place55(18, 0x101, 60);
    Story_Do3(0x1442);
    Story_Check19(0x2012, 0);
    if (Story_Check20(0, 0) == 1) {
        goto L_02000cb6;
    }
    L_02000f86:;
    Story_Place56(3, 0xc000, 0);
    Story_Place57(0, 0xc000, 0);
    Story_Place58(1, 0xc000, 0);
    Story_Place59(2, 0xc000, 20);
    Story_Run101(3, 3);
    Story_Run102(0, 3);
    Story_Run103(1, 3);
    Story_Run104(2, 3);
    Story_Place60(18, 0x105, 60);
    Story_Do4(0x1443);
    Story_Run14(0x2012, 0);
    record = Story_Check43(20);
    Story_Run105(record, 0);
    record = Story_Check44(20);
    *(volatile s32 *)(record + 24) = 0x8000;
    *(volatile s32 *)(record + 28) = 0x8000;
    record = Story_Check21(18);
    if (record != 0) {
        Story_Run106(20, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Story_Run107(1);
    Story_Run108(20, 6, 0);
    Story_Place61(20, 0x20000, 0x10000);
    Story_Place62(20, 0x37e, 0x29c);
    Story_Run109(40);
    Story_Place63(0x2012, 0, 10);
    Story_Place64(3, 0x101, 0);
    Story_Place65(0, 0x101, 0);
    Story_Place66(1, 0x101, 0);
    Story_Place67(2, 0x101, 60);
    Story_Run110(18, 4);
    Story_Place68(0x2012, 0, 10);
    Story_Place69(1, 0x103, 60);
    Story_Place70(1, 0xe000, 10);
    Story_Check22(0x4001, 0);
    Story_Place71(3, 0xa000, 0);
    Story_Place72(2, 0xa000, 0);
    Story_Place73(0, 0x6000, 0);
    if (Story_Check23(0, 0) == 1) {
        do {
            Story_Run111(1, 2);
            Story_Run112(2, 2);
            Story_Do5(0x1447);
            Story_Check24(0x4001, 0);
        } while (Story_Check25(0, 0) != 1);
    }
    Story_Run113(1, 3);
    Story_Do6(0x1448);
    Story_Place74(0x4001, 0, 10);
    Story_Place75(3, 0xc000, 0);
    Story_Place76(0, 0xc000, 0);
    Story_Place77(1, 0xc000, 0);
    Story_Place78(2, 0xc000, 10);
    Story_Run114(0, 3);
    Story_Run115(0, 3);
    Story_Run116(20);
    Story_Place79(18, 0x105, 60);
    Story_Place80(0x2012, 0, 20);
    Story_Place81(2, 0x102, 60);
    Story_Place82(18, 0x3000, 10);
    Story_Place83(18, 0x101, 60);
    Story_Place84(1, 0x101, 40);
    Story_Run117(1, 0, 20);
    Story_Run118(1, 1);
    Story_Place85(0x4001, 0, 10);
    Story_Place86(3, 0x8000, 0);
    Story_Place87(0, 0x2000, 20);
    Story_Place88(2, 0x102, 60);
    Story_Place89(2, 0x8000, 10);
    Story_Place90(0x4002, 0, 10);
    Story_Run119(1, 1);
    Story_Run120(40);
    Story_Run121(1, 3);
    Story_Place91(0x4001, 0, 10);
    Story_Place92(1, 0xe000, 10);
    Story_Run122(1, 1);
    Story_Check26(0x4001, 0);
    Story_Place93(3, 0xa000, 0);
    Story_Place94(2, 0xa000, 0);
    Story_Place95(0, 0x6000, 0);
    while (Story_Check27(0, 0) != 0) {
        Story_Do7(0x144e);
        Story_Run15(0x4001, 0);
    }
    Story_Run123(10);
    Story_Place96(3, 0x8000, 0);
    Story_Run124(1, 0, 10);
    Story_Place97(0, 0x2000, 10);
    Story_Run125(1, 3);
    Story_Place98(2, 0x105, 60);
    Story_Place99(2, 0x8000, 10);
    Story_Run126(2, 4);
    Story_Do8(0x144f);
    Story_Place100(0x4002, 0, 20);
    Story_Run127(18, 1);
    Story_Place101(18, 0x5000, 10);
    Story_Run128(18, 4);
    Story_Place102(0x2012, 0, 10);
    Story_Place103(3, 0xc000, 0);
    Story_Place104(0, 0xc000, 0);
    Story_Place105(1, 0xc000, 0);
    Story_Place106(2, 0xc000, 20);
    Story_Run129(3, 3);
    Story_Run130(0, 3);
    Story_Run131(1, 3);
    Story_Run132(2, 3);
    Story_Run133(40);
    Story_Place107(18, 0x105, 80);
    Story_Place108(19, 0x1000, 10);
    Story_Run16(19, 0x102);
    Story_Run134(40);
    Story_Run135(19, 0, 10);
    Story_Run136(18, 1);
    Story_Run137(40);
    Story_Place109(18, 0x7000, 20);
    Story_Place110(0x2012, 0, 10);
    Story_Run138(19, 3);
    Story_Run139(20);
    Story_Run140(18, 4);
    Story_Run141(18, 4);
    Story_Place111(0x2012, 0, 10);
    Story_Run142(20, 6, 0);
    record = Story_Check28(18);
    if (record != 0) {
        Story_Run143(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run144(20);
    Story_Run145(20, 0, 0);
    Story_Run146(20);
    Story_Run17(3, 0x102);
    Story_Run18(0, 0x102);
    Story_Run19(1, 0x102);
    Story_Run20(2, 0x102);
    Story_Run147(40);
    Story_Run148(19, 2);
    Story_Run149(19, 0, 10);
    Story_Run150(18, 3);
    Story_Place112(0x2012, 0, 20);
    Story_Run151(19, 3);
    Story_Run152(2, 2);
    Story_Run153(20);
    Story_Place113(0x4002, 0, 10);
    Story_Place114(19, 0x3000, 0);
    Story_Place115(18, 0x3000, 10);
    Story_Run154(18, 4);
    Story_Place116(0x2012, 0, 10);
    Story_Place117(1, 0x103, 60);
    Story_Place118(0x4001, 0, 10);
    Story_Place119(18, 0x5000, 10);
    Story_Run155(18, 4);
    Story_Place120(0x2012, 0, 10);
    Story_Run156(2, 4);
    Story_Place121(0x4002, 0, 10);
    Story_Place122(18, 0x3000, 10);
    Story_Run157(18, 3);
    Story_Place123(0x2012, 0, 10);
    Story_Place124(3, 0x107, 0);
    Story_Place125(0, 0x107, 0);
    Story_Place126(1, 0x107, 0);
    Story_Place127(2, 0x107, 60);
    Story_Place128(18, 0x7000, 10);
    Story_Run158(18, 3);
    Story_Run159(10);
    Story_Place129(0x2012, 0, 10);
    Story_Run160(19, 2);
    Story_Place130(19, 0x1000, 10);
    Story_Run161(19, 3);
    Story_Run162(20);
    base5_200a5c0 = (s32)gOv;
    Story_Place131(0, 0x10013, base5_200a5c0);
    Story_Place132(1, 0x10013, base5_200a5c0);
    Story_Place133(2, 0x10013, base5_200a5c0);
    Story_Place134(3, 0x10013, base5_200a5c0);
    Story_Place135(19, 0x9999, 0x4ccc);
    Story_Place136(19, 0x354, 0x286);
    Story_Place137(19, 0x354, 0x29a);
    Story_Place138(19, 0x360, 0x2a0);
    Story_Place139(19, 0x1000, 10);
    Story_Place140(0x4013, 0, 20);
    Story_Run163(0);
    Story_Run164(1);
    Story_Run165(2);
    Story_Place141(0, 0x105, 0);
    Story_Place142(1, 0x105, 0);
    Story_Place143(2, 0x105, 60);
    Story_Run166(0, 3);
    Story_Run167(1, 3);
    Story_Run168(2, 3);
    Story_Place144(19, 0x10000, base5_200a5c0);
    Story_Run169(1, 2);
    record = Story_Check29(0);
    if (record != 0) {
        Story_Run170(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run171(2, 2);
    record = Story_Check30(0);
    if (record != 0) {
        Story_Run172(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Story_Check31(3) != 0) {
        Story_Run173(3, 2);
        record = Story_Check32(0);
        if (record != 0) {
            Story_Run174(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Story_Run175(2);
    Story_Run176(1, 0, 0);
    Story_Run177(2, 0, 0);
    Story_Run178(3, 0, 0);
    Story_Place145(0, 0x10000, 0x8000);
    Story_Place146(0, 0x37e, 0x2f0);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    ((s64 (*)())Story_Run179)();
    Story_Run180();
    Story_Do9(0x321);
    Story_Run181(29);
    Story_Run182();
}

/* overlays/scene/story/flag_branched/scene_channel.c */

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Story_Place158(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Story_Place159(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Story_Place160(channel, value, zero);
}

static __inline__ void ConfigureFirst2(s32 channel, s32 value, s32 zero)
{
    Story_Place161(channel, value, zero);
}

static __inline__ void ConfigureSecond2(s32 channel, s32 value, s32 zero)
{
    Story_Place162(channel, value, zero);
}

static __inline__ void ConfigureThird2(s32 channel, s32 value, s32 zero)
{
    Story_Place163(channel, value, zero);
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
void Story_Run183();           /* Record hand-off. */

s32 Story_Run184();            /* Story-flag test. */

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
 * 0x21 the record is handed to Story_Run183 and, when story flag 0x84e is
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
        Story_Run183(p);

        if (Story_Run184(0x84e) != 0) {
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

void Story_Run185();           /* scripted-scene bracket: open */

s32 Story_Run186();            /* story-flag test; used in a condition */

void Story_Run187();           /* show a dialogue line by id */

void Story_Run188();           /* wait for the slot's action to finish */

void Story_Run189();           /* scripted-scene bracket: close */

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    Story_Run190();
    Story_Place147(14, 0x102, 0);
    Story_Run191(14, 2);
    Story_Run192(40);
    Story_Do10(0x1764);
    Story_Run193(14, 0, 20);
    Story_Run194(14, 0, 0);
    Story_Run195(20);
    Story_Run196(14, 0, 10);
    Story_Place148(14, 0xb000, 10);
    Story_Run197();
}

void Scene_RunCueSequenceA(void)
{
    u32 i;
    s32 record;

    Story_Run198();
    Story_Do11(0x1750);
    if (Story_Check33(0x302) != 0) {
        Story_Do12(0x1768);
    }
    Story_Run199(15, 0);
    Story_Do13(0x302);
    Story_Run200();
}

void Scene_RunActorSeventeenFlaggedDialogue(void)
{
    u8 *work;

    Story_Run185();

    if (Story_Run186(0x202) != 0) {
        Story_Run187(0x174b);
    } else if (Story_Check45(0x84e) != 0) {
        Story_Run201(0x176e);
    } else {
        Story_Run202(0x1432);
        if (Story_Check46(0x84d) != 0) {
            work = *(u8 **)0x03001ebc;
            *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        }
    }

    Story_Run188(17, 0);
    Story_Run189();
}

void Scene_RunCue(void)
{
    u32 i;
    s32 record;

    if (Story_Check34(0x84e) == 0) {
        if (Story_Check35(0x322) != 0) {
            Story_Run203();
            Story_Place149(19, 0x100, 0);
            Story_Place150(19, 0x7000, 10);
            Story_Run204(19, 2);
            Story_Run205(20);
            Story_Do14(0x1748);
            Story_Run206(19, 0);
            Story_Place151(0, 0x10000, 0x8000);
            Story_Place152(0, 0x268, 0x2fa);
            Story_Place153(19, 0xd000, 10);
            Story_Run207();
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
    Story_Run208();
    Story_Do19((s32)&Value_00001420);
    Story_Apply3(10, 0);
    Story_Run209();
}

void Scene_RunSlotSubjectBranch(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Story_Run210();

    record = Story_Place164(0, slot, subject);

    if (Story_Apply4(subject, 0) != -1) {
        Story_Apply5(slot, 2);
        Story_Do20(0x84E);
        Story_Do21(flag);
        Story_Do22(0x322);
        Story_Do23(0x202);
    } else {
        Story_Do24(125);
        Story_Apply6(slot, 5);
    }

    Story_Do25(record);
    Story_Run211();
}

/* overlays/scene/story/flag_branched/scene_state_interaction.c */

void Story_Run212();           /* scripted-scene bracket: open */

s32 Story_Run213();            /* story-flag test; used in a condition */

void Story_Run214();           /* show a dialogue line by id */

void Story_Run215();           /* wait for the slot's action to finish */

void Story_Run216();           /* scripted-scene bracket: close */

void Scene_RunActorSeventeenFlagDialogue(void)
{
    Story_Run212();

    if (Story_Run213(0x202) != 0) {
        Story_Run214(0x174c);
    } else if (Story_Check47(0x845) == 0) {
        Story_Run217(0x1436);
    } else {
        Story_Run218(0x1434);
        if (Story_Check48(0x84e) != 0) {
            Story_Run219(0x176f);
        }
    }

    Story_Run215(17, 0);
    Story_Run216();
}
