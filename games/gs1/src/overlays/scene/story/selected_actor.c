#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/selected_actor.h"

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

void *Story_Run3(s32, s32, s32, s32);

typedef s32(*IwramSqrt)(s32);

/*
 * Waits for an object field to fall to a limit -- resource_3a5.
 */

/* Contiguous unnamed leaf-owner run for resource_3a5. */

void *OvObj_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Story_Run3(arg3, arg0, arg1, arg2);

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

void Scene_ApplyActor13Values3And3(void) { Story_Run4(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OvObj_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Story_Run5(1);
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

    Story_Run6();
    if (Story_Check(0x8b2) == 0) {
        if (Story_Check2(0x8b3) == 0) {
            Story_Do(0x8b3);
            Story_Do2(0x8b2);
        }
    }
    Story_Run7(123);
    Story_Run8(3);
    Story_Run9();
}

void *SceneData_GetTableA500(void)
{
    return (void *)0x0200a500;
}

void Scene_RunScene3a5(void)
{
    extern u8 gCell[];

    Story_Run10(8, 2);
    Story_Run11((s32)gVal, 5);
    do {
        gCell[0x22b] = 3;
    } while (0);
    Story_Run12(53, 5);
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
        record = Story_Check3(v5);
        if (record == 0) {
            Story_Run13(v5);
            break;
        }
    }
    Story_Run14();
    Story_Run15((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Story_Run16((v6 + 1), 0);
    Story_Run17();
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
    Story_Run18();
    if (Story_Check4(0x90a) != 0) {
    } else {
        rec8 = Story_Check5(0x200);
        if (rec8 != 0) {
        } else {
            Story_Do3(0x200);
            Story_Run19(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Story_Check6(0);
            *(s32 *)(record + 36) = rec8;
            record = Story_Check7(0);
            *(s32 *)(record + 44) = rec8;
            record = Story_Check8(0);
            *(s32 *)(record + 56) = -0x80000000;
            record = Story_Check9(0);
            *(s32 *)(record + 64) = -0x80000000;
            Story_Run20(0, 1);
            Story_Run21(0, 8, 0);
            Story_Run22(40);
            Story_Run23(0, 2);
            Story_Run2(0, 0x102);
            Story_Run24(40);
            *(u8 *)(Story_Check10(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Story_Check11(0);
            Story_Place(-0x100000, *(u16 *)(record + 6), (s32)rect);
            Story_Place2(0, 0x20000, 0x10000);
            Story_Run25(0, 2);
            Story_Run26(0, rect[0] / 0x10000, rect[2] / 0x10000);
            Story_Run27(0);
            Story_Run28(2);
            *(u8 *)(Story_Check12(0) + 90) |= 1;
            Story_Run29(30);
            Story_Run30(148);
            Story_Run31(8, 2);
            Story_Run32(20);
            Story_Place3(8, 0x28000, 0x14000);
            Story_Run33(8, 168, 104);
            Story_Place4(8, 0x8000, 0x4000);
            Story_Run34(8, 168, 92);
            *shown_addr = shown;
            Story_Run35(0);
        }
    }
    /* unlifted: 0x02001590..0x02001594 (2), 0x0200159c..0x020015a0 (2) */
}

void Scene_RunScene3a5(void)
{
    u32 i;
    s32 record;

    Story_Place5(8, 0x8000, 0x4000);
    Story_Run36(8, 1);
    Story_Run37(8, 168, 96);
    Story_Run38(8, 2);
}

void State_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}
