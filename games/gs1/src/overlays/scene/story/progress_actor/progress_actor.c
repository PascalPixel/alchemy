#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/progress_actor/progress_actor.h"
#include "facing_object.h"

/* overlays/scene/story/progress_actor/progress_dependent_actor_scene_head.c */

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gWork[];

struct ObjectRuntime *Story_Run3(u32);

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
    if (Story_Check13(0x87A) != 0) {
        return gOv;
    }
    if (Story_Check14(0x834) != 0) {
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
        Story_Run4();
        Story_Place(0, 0x10000, 0x8000);
        Story_Do(0xfa6);
        Story_Run5(13, 1);
        Story_Place2(0, 232, 0x108);
        Story_Run6(0, 0);
        Story_Run7(0, 13, 20);
        Story_Run8(13, 2);
        Story_Run9(13, 0, 10);
        record = (u8 *)Story_Run3(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Story_Run10(5, x << 16, y << 16);
        Story_Run11(1, x << 16, y << 16);
        Story_Place3(5, 0x8000, 0x4000);
        Story_Place4(1, 0x8000, 0x4000);
        Story_Place5(5, 248, 0x108);
        Story_Place6(1, 216, 0x108);
        Story_Run12(0, 1);
        Story_Run13(5, 1);
        Story_Run14(1, 1);
        Story_Run15(4);
        Story_Place7(5, 0xb000, 0);
        Story_Place8(1, 0xd000, 20);
        Story_Run16(5, 4);
        Story_Run17(10);
        Story_Run18(5, 0, 20);
        Story_Run19(13, 1);
        Story_Run20(10);
        Story_Place9(13, 0x3000, 10);
        Story_Run21(13, 0, 10);
        Story_Place10(1, 0x3000, 40);
        Story_Run22(1, 2);
        Story_Run23(1, 0, 10);
        Story_Run24(13, 2);
        Story_Place11(13, 0x5000, 10);
        Story_Run25(13, 3);
        Story_Run26(13, 0, 8);
        Story_Place12(5, 0x3000, 20);
        Story_Run27(5, 0, 10);
        Story_Place13(13, 0x3000, 10);
        Story_Run28(13, 3);
        Story_Run29(10);
        Story_Run30(13, 0, 6);
        Story_Place14(0, 0x101, 0);
        Story_Place15(1, 0x101, 0);
        Story_Place16(5, 0x101, 60);
        Story_Place17(1, 0xd000, 0);
        Story_Place18(5, 0xb000, 20);
        Story_Run31(5, 2);
        Story_Run32(5, 0, 10);
        Story_Place19(13, 0x102, 60);
        Story_Run33(13, 4);
        Story_Run34(13, 0, 10);
        Story_Run35(1, 0, 0);
        Story_Place20(5, 0x8000, 20);
        Story_Place21(0, 0x100, 60);
        Story_Place22(1, 0xd000, 0);
        Story_Place23(5, 0xb000, 0);
        Story_Place24(13, 0x5000, 20);
        Story_Place25(13, 0x3000, 40);
        Story_Place26(13, 0x5000, 40);
        Story_Place27(13, 0x3000, 20);
        Story_Check2(13, 0);
        if (Story_Check3(0, 0) == 0) {
            Story_Run36((s32)gVal);
        } else {
            Story_Do2(0xfb1);
        }
        Story_Run37(20);
        Story_Run38(13, 3);
        Story_Run39(10);
        Story_Run40(13, 0, 10);
        Story_Do3(0xfb2);
        Story_Run41(5, 2);
        Story_Place28(5, 0x8000, 10);
        Story_Run42(5, 0, 6);
        Story_Place29(1, 0x103, 30);
        Story_Run43(1, 4, 30);
        Story_Run44(1, 0, 10);
        Story_Run45(1, 0, 6);
        Story_Run46(0, 1, 10);
        Story_Run47(0, 5, 0);
        Story_Run48(13, 1, 10);
        Story_Run49(13, 5, 10);
        Story_Run50(0, 3);
        Story_Run51(1, 3);
        Story_Run52(5, 3);
        Story_Run53(10);
        Story_Run54(5, 1);
        Story_Run55(1, 1);
        Story_Run56(0, 0);
        Story_Place30(0, 0x4000, 0);
        Story_Place31(1, 0x4000, 0);
        Story_Place32(5, 0x4000, 16);
        Story_Run(13, 0x102);
        Story_Run57(13, 3);
        Story_Run58(10);
        Story_Run59(13, 0, 6);
        Story_Place33(0, 0x100, 0);
        Story_Place34(1, 0x100, 0);
        Story_Place35(5, 0x100, 40);
        Story_Run60(13, 4);
        Story_Run61(10);
        Story_Run62(13, 0, 6);
        Story_Run63(13, 1);
        Story_Run64(13, 0, 6);
        Story_Place36(5, 0xb000, 10);
        Story_Run65(5, 0, 6);
        Story_Place37(13, 0x3000, 10);
        Story_Run66(13, 3);
        Story_Run67(6);
        Story_Run68(13, 0, 10);
        Story_Run69(0, 2, 0);
        Story_Run70(1, 2, 0);
        Story_Run71(5, 2, 10);
        Story_Place38(0, 0xc000, 0);
        Story_Place39(1, 0xd000, 20);
        Story_Run72(1, 0, 6);
        Story_Place40(13, 0x5000, 10);
        Story_Run73(13, 3);
        Story_Run74(16);
        Story_Run75(0, 5, 40);
        Story_Run76(5, 0, 10);
        Story_Run77(13, 2);
        Story_Place41(13, 0x3000, 10);
        Story_Run78(13, 0, 6);
        Story_Place42(0, 0xc000, 0);
        Story_Place43(5, 0xb000, 30);
        Story_Place44(0, 0x105, 0);
        Story_Place45(1, 0x105, 0);
        Story_Place46(5, 0x105, 80);
        Story_Run79(13, 4);
        Story_Check4(13, 0);
        if (Story_Check5(0, 0) == 0) {
            Story_Do4(0xfbd);
        } else {
            Story_Do5(0xfbe);
        }
        Story_Run80(13, 0, 20);
        base5_fbf = (s32)gVal2;
        Story_Run81(base5_fbf);
        Story_Run82(1, 0, 10);
        Story_Run83(1, 2);
        Story_Check6(1, 0);
        if (Story_Check7(0, 0) == 0) {
            Story_Run84((base5_fbf + 1));
        } else {
            Story_Run85((base5_fbf + 2));
        }
        Story_Run86(1, 0, 6);
        Story_Do6(0xfc2);
        Story_Place47(5, 0x8000, 10);
        Story_Run87(5, 1);
        Story_Run88(5, 0);
        Story_Run89(4);
        if (Story_Check8(0, 0) == 1) {
            Story_Run90(5, 2, 20);
            Story_Run91(5, 0, 10);
        } else {
            Story_Run92(0, 3);
            Story_Run93(1, 3);
            Story_Run94(5, 3);
            Story_Run95(8);
            Story_Run96(0, 0);
            bump_step(1);
        }
        Story_Run97(13, 3);
        Story_Run98(10);
        Story_Run99(13, 0, 10);
        Story_Place48(0, 0xc000, 0);
        Story_Place49(1, 0xd000, 0);
        Story_Place50(5, 0xb000, 10);
        Story_Run100(0, 3);
        Story_Run101(1, 3);
        Story_Run102(5, 3);
        Story_Run103(10);
        Story_Run104(0, 0);
        Story_Run105(20);
        Story_Run106(13, 2);
        Story_Run107(13, 0);
        Story_Run108(4);
        if (Story_Check9(0, 0) == 0) {
            Story_Do7(0xfc6);
        } else {
            Story_Do8(0xfc9);
        }
        ((void (*)())Story_Check15)(10);
        Story_Run109(1, 2);
        Story_Run110(1, 0, 10);
        Story_Run111(1, 0, 6);
        Story_Place51(5, 0x8000, 10);
        Story_Run112(5, 4);
        Story_Run113(5, 0, 6);
        Story_Run114(1, 2);
        Story_Place52(1, 0x8000, 10);
        Story_Place53(1, 0x103, 30);
        Story_Run115(1, 0, 10);
        Story_Place54(13, 0x100, 40);
        Story_Run116(13, 4, 40);
        Story_Place55(0, 0xc000, 0);
        Story_Place56(1, 0xc000, 0);
        Story_Place57(5, 0xb000, 20);
        Story_Place58(13, 0xb000, 10);
        Story_Run117(158);
        Story_Place59(0x200a0ac, 43, 8);
        Story_Place60(13, 0x10000, 0x8000);
        Story_Run118(13, 232, 218);
        Story_Run119(13, 0, 0);
        Story_Place61(0, 0x101, 0);
        Story_Place62(1, 0x101, 0);
        Story_Place63(5, 0x101, 60);
        *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x202;
        Story_Run120();
        Story_Run121();
        Story_Run122(13);
        Story_Run123();
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

    Story_Run124();
    Story_Run125(13, 0, 0);
    Story_Place64(1, 0xd80000, 0x1080000);
    Story_Place65(5, 0xf80000, 0x1080000);
    record = Story_Check16(1);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    record = Story_Check17(5);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Story_Place66(0x200a0ac, 43, 8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_Run126();
    Story_Run127();
    Story_Run128(40);
    Story_Place67(13, 0xcccc, 0x6666);
    Story_Place68(13, 0xe60000, 0xdc0000);
    Story_Run129(13, 230, 232);
    Story_Run130(20);
    Story_Run131(13, 3);
    Story_Do9(0xfcc);
    Story_Run132(13, 0, 10);
    Story_Run133(13, 2);
    Story_Place69(13, 0x3000, 10);
    Story_Run134(13, 0, 10);
    Story_Run135(1, 0, 0);
    Story_Place70(0, 0x8000, 10);
    Story_Run136(0, 3);
    Story_Run137(1, 3);
    Story_Run138(0, 0);
    Story_Run139(20);
    Story_Run140(0, 0, 0);
    Story_Place71(5, 0x8000, 10);
    Story_Run141(5, 3);
    Story_Run142(0, 3);
    Story_Run143(0, 0);
    Story_Place72(1, 0xcccc, 0x6666);
    Story_Place73(5, 0xcccc, 0x6666);
    Story_Run144(0, 0xc000, 0);
    Story_Run145(1, 2);
    record = Story_Check10(0);
    if (record != 0) {
        Story_Run146(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run147(5, 2);
    record = Story_Check11(0);
    if (record != 0) {
        Story_Run148(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run149(13, 2);
    record = Story_Check12(0);
    if (record != 0) {
        Story_Run150(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run151(1);
    Story_Run152(1, 0, 0);
    Story_Run153(5, 0, 0);
    Story_Run154(13);
    Story_Run155(13, 0, 0);
    Story_Run156(1, 1);
    Story_Run157(5, 1);
    Story_Run158(13, 1);
    Story_Run159(14, 0, 0);
    Story_Run160(15, 0, 0);
    Story_Do10(0x801);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_Run2(0x10000, 0);
    Story_Do11(0x242);
    Story_Run161();
}

void Scene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (Story_Check18(0x242) == 0) {
        Story_Do12(0x9E);
        Story_Place74(gOv4, 0x2B, 8);
    }
    Story_Place75(0, 0xE5, 0xD9);
    Story_Do13(3);
}

void State_SetWorkAndFlag87d(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_Run162();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_Do14(0x87D);
    Story_Apply(12, 0);
    Story_Do15(0x900);   /* 144 << 4 */
    Story_Run163();
}

void State_SetWorkAndFlag87e(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_Run164();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_Do16(0x87E);
    Story_Apply2(12, 1);
    Story_Do17(0x900);   /* 144 << 4 */
    Story_Run165();
}

void Dialogue_RunActorSixteenDialogue(void)
{
    Story_Run166();
    Story_Do18(0x1C96);
    Story_Apply3(16, 0);
    Story_Run167();
}
