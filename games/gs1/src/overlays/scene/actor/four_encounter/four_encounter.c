#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/four_encounter/four_encounter.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"

/* overlays/scene/actor/four_encounter/field_scene.c */

/* Contiguous unnamed leaf-owner run for resource_396. */

void Scene_RunStep200(void) { Actor_SetMode(0x200, 64, 35, 21); }

void Scene_RunStep201(void) { Actor_SetMode2(0x201, 65, 35, 22); }

void Scene_RunStep202(void) { Actor_SetMode3(0x202, 66, 35, 23); }

void Scene_RunStep203(void) { Actor_SetMode4(0x203, 67, 35, 24); }

void Scene_RunStep204(void) { Actor_SetMode5(0x204, 68, 35, 25); }

void Scene_RunStep205(void) { Actor_SetMode6(0x205, 69, 35, 26); }

void Scene_RunStep206(void) { Actor_SetMode7(0x206, 70, 35, 27); }

void Scene_RunStep207(void) { Actor_SetMode8(0x207, 71, 35, 28); }

void Scene_RunStep208(void) { Actor_SetMode9(0x208, 72, 35, 29); }

void Scene_RunStep209(void) { Actor_SetMode10(0x209, 73, 35, 31); }

void Scene_RunStep20a(void) { Actor_SetMode11(0x20a, 74, 35, 32); }

void Scene_RunStep20b(void) { Actor_SetMode12(0x20b, 79, 35, 50); }

void Scene_RunStep20c(void) { Actor_SetMode13(0x20c, 75, 35, 51); }

void Scene_RunStep20d(void) { Actor_SetMode14(0x20d, 76, 35, 52); }

void Scene_RunStep20e(void) { Actor_SetMode15(0x20e, 77, 35, 53); }

void Scene_RunStep20f(void) { Actor_SetMode16(0x20f, 78, 35, 54); }

void Scene_RunStep210(void) { Actor_SetMode17(0x210, 80, 35, 55); }

void Scene_RunStep211(void) { Actor_SetMode18(0x211, 81, 35, 56); }

void Scene_RunStep212(void) { Actor_SetMode19(0x212, 82, 35, 57); }

void Scene_RunStep213(void) { Actor_SetMode20(0x213, 83, 35, 58); }

void Scene_RunStep214(void) { Actor_SetMode21(0x214, 84, 35, 59); }

/* overlays/scene/actor/four_encounter/overlay_object.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *AcquireOverlayObject(s32, s32, s32, s32);

void *OvObj_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(kind, x, y, z);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* overlays/scene/actor/four_encounter/palette_scene.c */

/*
 * Register this overlay's palette-fade callback.
 *
 * Both addresses are named at their decoded sites: the reference bl's raw
 * decode is 0x02003282 (the registration import as this stream reaches it,
 * semantically Actor_Run19), and the pool word is 0x02009245 -- the
 * callback lives at 0x02009244 in a later-loaded RAM segment, not at this
 * stream's own same-shaped 0x02001244.
 */

void Effect_RegisterPaletteFadeCallback(void)
{
    Actor_Do5(Actor_Run20);
}

/* overlays/scene/actor/four_encounter/scene_data.c */
/* Contiguous unnamed leaf-owner run for resource_396. */

/* Return the overlay's first persistent workspace block. */
void *SceneData_GetTablea5cc(void)
{
    return (void *)0x0200a5cc;
}

/* Return the overlay's second persistent workspace block. */
void *SceneData_GetTablea9d4(void)
{
    return (void *)0x0200a9d4;
}

/* Return the overlay's third persistent workspace block. */
void *SceneData_GetTableaa14(void)
{
    return (void *)0x0200aa14;
}

/* Return the overlay's fourth persistent workspace block. */
void *SceneData_GetTableaa58(void)
{
    return (void *)0x0200aa58;
}

/* Return the overlay's presentation workspace block. */
void *SceneData_GetTableAae8(void)
{
    return (void *)0x0200aae8;
}

/* overlays/scene/actor/four_encounter/scene_primary_script.c */
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gWork[];

void Scene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    u8 *work;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = Actor_Check(3);
    *(u8 *)(Actor_Check7(3) + 35) &= 254;
    Actor_Run21(3, 2);
    *(u8 *)(Actor_Check8(0) + 35) &= 254;
    Actor_Run22(0, 2);
    Actor_Run23(184);
    Actor_Run24(17);
    Actor_Run25();
    Actor_Place(0, 0xcccc, 0x6666);
    Actor_Place2(1, 0xcccc, 0x6666);
    Actor_Place3(2, 0xcccc, 0x6666);
    Actor_Place4(3, 0xcccc, 0x6666);
    Actor_Place5(0, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Check9(0);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Actor_Place6(1, 0x940000, 0x5a0000);
    record = Actor_Check10(1);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Actor_Place7(2, 0xb60000, 0x5a0000);
    record = Actor_Check2(2);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_Place8(3, 0xa60000, 0x680000);
        record = Actor_Check3(3);
        *(volatile u16 *)(record + 6) = (v6 << 8);
    }
    Actor_Run26(0);
    Actor_Run27(10);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 48;
    Actor_Run28();
    Actor_Run29();
    Actor_Run30(20);
    Actor_Run(0x13333, 0x2666);
    Actor_Run2(0xa80000, -1, 0x980000, 1);
    Actor_Run31();
    Actor_Run32(10);
    v5 = 10;
    Actor_Run33(123);
    Actor_SetRect(26, 3, 1, 2, v5, 8);
    Actor_SetRect2(26, 38, 1, 1, v5, 43);
    Actor_Run34(4);
    Actor_SetRect3(26, 37, 1, 2, v5, 42);
    Actor_Run35(4);
    Actor_SetRect4(26, 36, 1, 3, v5, 41);
    Actor_Run36(4);
    Actor_SetRect5(26, 35, 1, 4, v5, 40);
    Actor_Run37(80);
    Actor_Do(0x14d3);
    Actor_Place9(0x8009, 0, 20);
    Actor_Run38(0, 2);
    Actor_Run39(1, 2);
    Actor_Run40(3, 2);
    Actor_Run41(2, 2);
    Actor_Run42(20);
    Actor_Run3(0xa80000, -1, 0x5a0000, 1);
    Actor_Run43();
    Actor_Run44(40);
    Actor_Run45(1);
    Actor_Run46(60);
    Actor_Run47(21);
    Actor_Run48(4);
    Actor_Place10(0x8009, 0, 20);
    Actor_Place11(0, 0x101, 0);
    Actor_Place12(1, 0x101, 0);
    Actor_Place13(3, 0x101, 0);
    Actor_Place14(2, 0x101, 80);
    Actor_Run4(0x8009, 0);
    Actor_Run49(40);
    Actor_Place15(0x8009, 0, 20);
    Actor_Run50(0, 3);
    Actor_Run51(1, 3);
    Actor_Run52(3, 3);
    Actor_Run53(2, 3);
    Actor_Run54(20);
    Actor_Place16(0x8009, 0, 20);
    Actor_Run55(0);
    Actor_Run56(40);
    Actor_Place17(0x8009, 0, 20);
    Actor_Run5(0, 0x102);
    Actor_Run6(1, 0x102);
    Actor_Run7(3, 0x102);
    Actor_Run8(2, 0x102);
    Actor_Run57(60);
    Actor_Run58(1, 2);
    Actor_Place18(1, 0xe000, 10);
    Actor_Place19(0, 0x6000, 10);
    Actor_Place20(0x8001, 0, 10);
    Actor_Run59(2, 4);
    Actor_Place21(0, 0x2000, 0);
    Actor_Place22(2, 0xa000, 0);
    Actor_Place23(0x8002, 0, 20);
    Actor_Run60(0);
    Actor_Run61(40);
    Actor_Place24(0x8009, 0, 10);
    Actor_Run62(0, 2);
    Actor_Run63(1, 2);
    Actor_Run64(3, 2);
    Actor_Run65(2, 2);
    Actor_Run66(0, (v6 << 8), 0);
    ((void (*)())Actor_Check11)(1, (v6 << 8), 0);
    Actor_Run67(2, (v6 << 8), 40);
    Actor_Run68(4);
    Actor_Check4(0x8009, 0);
    Actor_Place25(1, 0xe000, 0);
    Actor_Place26(2, 0xa000, 0);
    if (Actor_Check5(0, 0) != 0) {
        Actor_Place27(1, 0x103, 20);
        Actor_Run69(1, 4);
        Actor_Do2(0x14dd);
        Actor_Run9(0x8001, 0);
        Actor_Place28(2, 0x103, 10);
        Actor_Run70(2, 3);
        Actor_Run10(0x8002, 0);
    }
    Actor_Run71(20);
    Actor_Run72(4);
    Actor_Do3(0x14df);
    Actor_Place29(0x8009, 0, 20);
    Actor_Place30(0x8009, 0, 10);
    Actor_Run73(0);
    Actor_Run74(20);
    Actor_Run11(0x10000, 0);
    Actor_Run12(0x406218, 1);
    Actor_Run75(20);
    Actor_Run76(40);
    Actor_Run77(0, 2);
    Actor_Run78(1, 2);
    Actor_Run79(3, 2);
    Actor_Run80(2, 2);
    Actor_Run81(1, (v6 << 8), 0);
    Actor_Run82(2, (v6 << 8), 20);
    Actor_Run83(20);
    *(volatile s32 *)gOv5 = 0;
    {
        volatile s32 *bank = (volatile s32 *)gOv2;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)Actor_Run84;
        bank[2] = 0x340000;
    }
    Actor_Run13(base5_200962d, 0xc80);
    Actor_Run85(220);
    Actor_Run86(base5_200962d);
    Actor_Run14(0x10000, 1);
    Actor_Run87(20);
    Actor_Run88(40);
    Actor_Run89(4);
    Actor_Run90(20);
    Actor_Place31(0x8009, 0, 10);
    Actor_Run91(0);
    Actor_Run15(0x8009, 0);
    Actor_Run16(8, 0x2009e54);
    Actor_Run92(40);
    Actor_Place32(1, 0x102, 60);
    Actor_Run17(0x8001, 0);
    Actor_Place33(2, 0x102, 10);
    Actor_Run18(0x8002, 0);
    Actor_Place34(1, 0xe000, 0);
    Actor_Place35(2, 0xa000, 10);
    Actor_Place36(0, 0x4000, 10);
    Actor_Run93(1, 1);
    Actor_Place37(0x8001, 0, 10);
    Actor_Run94(2, 1);
    Actor_Place38(0x8002, 0, 10);
    if (rec != 0) {
        Actor_Run95(3, 1);
        Actor_Place39(0x8003, 0, 10);
    }
    Actor_Run96(0, 3);
    Actor_Run97(1, 3);
    Actor_Run98(3, 3);
    Actor_Run99(2, 3);
    base5_2009ec8 = (s32)gOv;
    Actor_Run100(1, base5_2009ec8);
    if (rec != 0) {
        Actor_Run101(3, base5_2009ec8);
    }
    Actor_Run102(2, base5_2009ec8);
    Actor_Run103(20);
    *(u8 *)(Actor_Check12(0) + 35) |= 1;
    Actor_Do4(0x844);
    Actor_Check6(0x2009245, 0xc80);
    Actor_Run104();
}

/* overlays/scene/actor/four_encounter/state_update.c */
extern s16 *gOv6;

/* Clear the active story variant once the scene controller is idle. */
void State_ClearStoryVariantWhenIdle(void)
{
    if (Actor_Check13() == 0)
        *gOv6 = -1;
}

void State_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && Actor_Check14(0x109) == 0)
        Actor_Run105();

    Actor_Do6(1);
    if (Actor_Check15(0x844) != 0) {
        s32 w1 = 10;
        Actor_SetRect6(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Actor_SetRect7(46, 38, 1, 1, w2, 43);
            Actor_SetRect8(0, 0, 1, 2, w2, 9);
        }
        Actor_SetRect9(26, 3, 1, 2, w1, 8);
        Actor_SetRect10(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Actor_SetRect11(11, 8, 1, 2, w1, w2);
    }
}
