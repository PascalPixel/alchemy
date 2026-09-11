#include "types.h"
#include "scene.h"
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
 * semantically Actor_unk19_4), and the pool word is 0x02009245 -- the
 * callback lives at 0x02009244 in a later-loaded RAM segment, not at this
 * stream's own same-shaped 0x02001244.
 */

void Effect_RegisterPaletteFadeCallback(void)
{
    Actor_unk5_2(Actor_unk20_4);
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
    *(u8 *)(Actor_unk7(3) + 35) &= 254;
    Actor_unk21_4(3, 2);
    *(u8 *)(Actor_unk8(0) + 35) &= 254;
    Actor_unk22_4(0, 2);
    Actor_unk23_4(184);
    Actor_unk24_4(17);
    Actor_unk25_4();
    Actor_Place(0, 0xcccc, 0x6666);
    Actor_unk2_3(1, 0xcccc, 0x6666);
    Actor_unk3_3(2, 0xcccc, 0x6666);
    Actor_unk4_3(3, 0xcccc, 0x6666);
    Actor_unk5_3(0, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_unk9(0);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Actor_unk6_3(1, 0x940000, 0x5a0000);
    record = Actor_unk10(1);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Actor_unk7_3(2, 0xb60000, 0x5a0000);
    record = Actor_unk2(2);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_unk8_3(3, 0xa60000, 0x680000);
        record = Actor_unk3(3);
        *(volatile u16 *)(record + 6) = (v6 << 8);
    }
    Actor_unk26_4(0);
    Actor_unk27_4(10);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 48;
    Actor_unk28_4();
    Actor_unk29_4();
    Actor_unk30_4(20);
    Actor_Run(0x13333, 0x2666);
    Actor_unk2_4(0xa80000, -1, 0x980000, 1);
    Actor_unk31_4();
    Actor_unk32_4(10);
    v5 = 10;
    Actor_unk33_4(123);
    Actor_SetRect(26, 3, 1, 2, v5, 8);
    Actor_unk2_5(26, 38, 1, 1, v5, 43);
    Actor_unk34_4(4);
    Actor_unk3_5(26, 37, 1, 2, v5, 42);
    Actor_unk35_4(4);
    Actor_unk4_5(26, 36, 1, 3, v5, 41);
    Actor_unk36_4(4);
    Actor_unk5_5(26, 35, 1, 4, v5, 40);
    Actor_unk37_4(80);
    Actor_Do(0x14d3);
    Actor_unk9_3(0x8009, 0, 20);
    Actor_unk38_4(0, 2);
    Actor_unk39_4(1, 2);
    Actor_unk40_4(3, 2);
    Actor_unk41_4(2, 2);
    Actor_unk42_4(20);
    Actor_unk3_4(0xa80000, -1, 0x5a0000, 1);
    Actor_unk43_4();
    Actor_unk44_4(40);
    Actor_unk45_4(1);
    Actor_unk46_4(60);
    Actor_unk47_4(21);
    Actor_unk48_4(4);
    Actor_unk10_3(0x8009, 0, 20);
    Actor_unk11_3(0, 0x101, 0);
    Actor_unk12_3(1, 0x101, 0);
    Actor_unk13_3(3, 0x101, 0);
    Actor_unk14_3(2, 0x101, 80);
    Actor_unk4_4(0x8009, 0);
    Actor_unk49_4(40);
    Actor_unk15_3(0x8009, 0, 20);
    Actor_unk50_4(0, 3);
    Actor_unk51_4(1, 3);
    Actor_unk52_4(3, 3);
    Actor_unk53_4(2, 3);
    Actor_unk54_4(20);
    Actor_unk16_3(0x8009, 0, 20);
    Actor_unk55_4(0);
    Actor_unk56_4(40);
    Actor_unk17_3(0x8009, 0, 20);
    Actor_unk5_4(0, 0x102);
    Actor_unk6_4(1, 0x102);
    Actor_unk7_4(3, 0x102);
    Actor_unk8_4(2, 0x102);
    Actor_unk57_4(60);
    Actor_unk58_4(1, 2);
    Actor_unk18_3(1, 0xe000, 10);
    Actor_unk19_3(0, 0x6000, 10);
    Actor_unk20_3(0x8001, 0, 10);
    Actor_unk59_4(2, 4);
    Actor_unk21_3(0, 0x2000, 0);
    Actor_unk22_3(2, 0xa000, 0);
    Actor_unk23_3(0x8002, 0, 20);
    Actor_unk60_4(0);
    Actor_unk61_4(40);
    Actor_unk24_3(0x8009, 0, 10);
    Actor_unk62_4(0, 2);
    Actor_unk63_4(1, 2);
    Actor_unk64_4(3, 2);
    Actor_unk65_4(2, 2);
    Actor_unk66_4(0, (v6 << 8), 0);
    ((void (*)())Actor_unk11)(1, (v6 << 8), 0);
    Actor_unk67_4(2, (v6 << 8), 40);
    Actor_unk68_4(4);
    Actor_unk4(0x8009, 0);
    Actor_unk25_3(1, 0xe000, 0);
    Actor_unk26_3(2, 0xa000, 0);
    if (Actor_unk5(0, 0) != 0) {
        Actor_unk27_3(1, 0x103, 20);
        Actor_unk69_4(1, 4);
        Actor_unk2_2(0x14dd);
        Actor_unk9_4(0x8001, 0);
        Actor_unk28_3(2, 0x103, 10);
        Actor_unk70_4(2, 3);
        Actor_unk10_4(0x8002, 0);
    }
    Actor_unk71_4(20);
    Actor_unk72_4(4);
    Actor_unk3_2(0x14df);
    Actor_unk29_3(0x8009, 0, 20);
    Actor_unk30_3(0x8009, 0, 10);
    Actor_unk73_4(0);
    Actor_unk74_4(20);
    Actor_unk11_4(0x10000, 0);
    Actor_unk12_4(0x406218, 1);
    Actor_unk75_4(20);
    Actor_unk76_4(40);
    Actor_unk77_4(0, 2);
    Actor_unk78_4(1, 2);
    Actor_unk79_4(3, 2);
    Actor_unk80_4(2, 2);
    Actor_unk81_4(1, (v6 << 8), 0);
    Actor_unk82_4(2, (v6 << 8), 20);
    Actor_unk83_4(20);
    *(volatile s32 *)gOv5 = 0;
    {
        volatile s32 *bank = (volatile s32 *)gOv2;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)Actor_unk84_4;
        bank[2] = 0x340000;
    }
    Actor_unk13_4(base5_200962d, 0xc80);
    Actor_unk85_4(220);
    Actor_unk86_4(base5_200962d);
    Actor_unk14_4(0x10000, 1);
    Actor_unk87_4(20);
    Actor_unk88_4(40);
    Actor_unk89_4(4);
    Actor_unk90_4(20);
    Actor_unk31_3(0x8009, 0, 10);
    Actor_unk91_4(0);
    Actor_unk15_4(0x8009, 0);
    Actor_unk16_4(8, 0x2009e54);
    Actor_unk92_4(40);
    Actor_unk32_3(1, 0x102, 60);
    Actor_unk17_4(0x8001, 0);
    Actor_unk33_3(2, 0x102, 10);
    Actor_unk18_4(0x8002, 0);
    Actor_unk34_3(1, 0xe000, 0);
    Actor_unk35_3(2, 0xa000, 10);
    Actor_unk36_3(0, 0x4000, 10);
    Actor_unk93_4(1, 1);
    Actor_unk37_3(0x8001, 0, 10);
    Actor_unk94_4(2, 1);
    Actor_unk38_3(0x8002, 0, 10);
    if (rec != 0) {
        Actor_unk95_4(3, 1);
        Actor_unk39_3(0x8003, 0, 10);
    }
    Actor_unk96_4(0, 3);
    Actor_unk97_4(1, 3);
    Actor_unk98_4(3, 3);
    Actor_unk99_4(2, 3);
    base5_2009ec8 = (s32)gOv;
    Actor_unk100_4(1, base5_2009ec8);
    if (rec != 0) {
        Actor_unk101_4(3, base5_2009ec8);
    }
    Actor_unk102_4(2, base5_2009ec8);
    Actor_unk103_4(20);
    *(u8 *)(Actor_unk12(0) + 35) |= 1;
    Actor_unk4_2(0x844);
    Actor_unk6(0x2009245, 0xc80);
    Actor_unk104_4();
}

/* overlays/scene/actor/four_encounter/state_update.c */
extern s16 *gOv6;

/* Clear the active story variant once the scene controller is idle. */
void State_ClearStoryVariantWhenIdle(void)
{
    if (Actor_unk13() == 0)
        *gOv6 = -1;
}

void State_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && Actor_unk14(0x109) == 0)
        Actor_unk105_4();

    Actor_unk6_2(1);
    if (Actor_unk15(0x844) != 0) {
        s32 w1 = 10;
        Actor_unk6_5(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Actor_unk7_5(46, 38, 1, 1, w2, 43);
            Actor_unk8_5(0, 0, 1, 2, w2, 9);
        }
        Actor_unk9_5(26, 3, 1, 2, w1, 8);
        Actor_unk10_5(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Actor_unk11_5(11, 8, 1, 2, w1, w2);
    }
}
