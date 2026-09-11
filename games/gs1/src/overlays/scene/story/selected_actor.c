#include "types.h"
#include "scene.h"

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

extern u8 Value_00000059;
extern u8 Value_0000005a;
extern u8 Value_0000005b;
extern u8 Value_0000005c;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gWork[];
extern u8 gVal[];

void *Story_unk3_4(s32, s32, s32, s32);

typedef s32(*IwramSqrt)(s32);

/*
 * Waits for an object field to fall to a limit -- resource_3a5.
 */

/* Contiguous unnamed leaf-owner run for resource_3a5. */

void *OvObj_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Story_unk3_4(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Story_Apply(obj, 0);
        Story_Apply2(obj, 14);
        Story_Apply3(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void Scene_ApplyActor13Values3And3(void) { Story_unk4_4(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OvObj_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Story_unk5_4(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}

s32 SceneData_SelectTableByScene59To5c(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_00000059) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_0000005a) {
        return (s32)gOv2;
    }
    if (v == (s32)&Value_0000005b) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_0000005c) {
        return (s32)gOv4;
    }
    return (s32)gOv5;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTablea36c(void)
{
    return (void *)0x0200a36c;
}

void Scene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Story_unk6_4();
    if (Story_Check(0x8b2) == 0) {
        if (Story_unk2(0x8b3) == 0) {
            Story_Do(0x8b3);
            Story_unk2_2(0x8b2);
        }
    }
    Story_unk7_4(123);
    Story_unk8_4(3);
    Story_unk9_4();
}

void *SceneData_GetTableA500(void)
{
    return (void *)0x0200a500;
}

void Scene_RunScene3a5(void)
{
    extern u8 gCell[];

    Story_unk10_4(8, 2);
    Story_unk11_4((s32)gVal, 5);
    do {
        gCell[0x22b] = 3;
    } while (0);
    Story_unk12_4(53, 5);
}

void Scene_RunScene3a5(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Story_Run();
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = Story_unk3(v5);
        if (record == 0) {
            Story_unk13_4(v5);
            break;
        }
    }
    Story_unk14_4();
    Story_unk15_4((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Story_unk16_4((v6 + 1), 0);
    Story_unk17_4();
}

s32 CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz *dz;
    s32 dx_squared = dx *dx;

    return ((IwramSqrt)0x030001d8)(dx_squared + dz_squared);
}

void Scene_RunScene3a5(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Story_unk18_4();
    if (Story_unk4(0x90a) != 0) {
    } else {
        rec8 = Story_unk5(0x200);
        if (rec8 != 0) {
        } else {
            Story_unk3_2(0x200);
            Story_unk19_4(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Story_unk6(0);
            *(s32 *)(record + 36) = rec8;
            record = Story_unk7(0);
            *(s32 *)(record + 44) = rec8;
            record = Story_unk8(0);
            *(s32 *)(record + 56) = -0x80000000;
            record = Story_unk9(0);
            *(s32 *)(record + 64) = -0x80000000;
            Story_unk20_4(0, 1);
            Story_unk21_4(0, 8, 0);
            Story_unk22_4(40);
            Story_unk23_4(0, 2);
            Story_unk2_4(0, 0x102);
            Story_unk24_4(40);
            *(u8 *)(Story_unk10(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Story_unk11(0);
            Story_Place(-0x100000, *(u16 *)(record + 6), (s32)rect);
            Story_unk2_3(0, 0x20000, 0x10000);
            Story_unk25_4(0, 2);
            Story_unk26_4(0, rect[0] / 0x10000, rect[2] / 0x10000);
            Story_unk27_4(0);
            Story_unk28_4(2);
            *(u8 *)(Story_unk12(0) + 90) |= 1;
            Story_unk29_4(30);
            Story_unk30_4(148);
            Story_unk31_4(8, 2);
            Story_unk32_4(20);
            Story_unk3_3(8, 0x28000, 0x14000);
            Story_unk33_4(8, 168, 104);
            Story_unk4_3(8, 0x8000, 0x4000);
            Story_unk34_4(8, 168, 92);
            *shown_addr = shown;
            Story_unk35_4(0);
        }
    }
    /* unlifted: 0x02001590..0x02001594 (2), 0x0200159c..0x020015a0 (2) */
}

void Scene_RunScene3a5(void)
{
    u32 i;
    s32 record;

    Story_unk5_3(8, 0x8000, 0x4000);
    Story_unk36_4(8, 1);
    Story_unk37_4(8, 168, 96);
    Story_unk38_4(8, 2);
}

void State_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
