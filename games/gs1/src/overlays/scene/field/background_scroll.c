/*
 * Overlay resource_397: a field scene that shifts its two tracked objects by
 * whole blocks, blends the display for scene 9, and scrolls BG3 against the
 * vertical counter.
 */

#include "types.h"
#include "scene.h"

#include "resource_397.h"

/* The two tracked scene objects share this coordinate and terrain prefix. */
struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 settled_y;
    u8 unknown_18[10];
    u8 layer;
};

/* The split threshold and both scroll values share one work-record cell. */
union SceneCell {
    s32 w;
    s16 h[2];
};

extern s32 gCell[];
extern u8 *gCam;
extern u8 *gWork;
extern u32 gIw;
extern s32 gIw2;
extern s32 gIw3;
extern u16 gUnk;
extern s32 gOv;
extern u16 gOv2;
extern u16 gOv3;
extern u16 gUnk2;

u8 *Field_Run();

void State_SetActorEightValue3d(void)
{
    Field_Apply(8, 0x3D);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetTable835c(void)
{
    return (u8 *)0x0200835c;   /* image offset 0x35c */
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable844c(void)
{
    return (u8 *)0x0200844c;   /* image offset 0x44c */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable8474(void)
{
    return (u8 *)0x02008474;   /* image offset 0x474 */
}

void Actor_ShiftObjectsByBlock(s32 bx, s32 bz)
{
    u8 *work = *(u8 **)0x03001ebc;
    struct SceneObject *obj;
    s32 dx = bx;
    s32 dz = bz;
    s32 h;

    /* Block coordinates become 16.16 fixed-point shifts of sixteen tiles. */
    obj = (struct SceneObject *)Field_Run(gCell[125]);
    dx <<= 20;
    dz <<= 20;

    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Field_Check((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }

    /* Apply the same shift to the workspace's independently optional object. */
    obj = *(struct SceneObject **)(work + 480);
    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Field_unk2((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }
}

void Scene_ApplyOffset0Pos5(void)
{
    Field_Apply2(0, 5);
}

void Scene_ApplyOffset0Neg5(void)
{
    Field_Apply3(0, -5);
}

void Scene_ApplyOffset0Pos5Second(void)
{
    Actor_ShiftObjectsByBlock(0, 5);
}

void Scene_ApplyOffset0Neg5Second(void)
{
    Field_Apply4(0, -5);
}

void Scene_ApplyOffset0Pos6(void)
{
    Field_Apply5(0, 6);
}

void Scene_ApplyOffset0Neg6(void)
{
    Field_Apply6(0, -6);
}

void State_SetValue123ThenCounter16c(void)
{
    u8 *state = gWork;
    s16 *cnt;

    Field_Do(0x7B);
    cnt = (s16 *)(state + 0x16C);
    Field_unk2_2(*cnt);
}

void Effect_SetAlphaBlendForScene9(void)
{
    u8 *disp;

    /* Start the scene, then configure alpha blending for its display state. */
    Field_unk2_4(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    disp = *(u8 **)0x03001ecc;
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable84a4(void)
{
    return (u8 *)0x020084a4;   /* image offset 0x4a4 */
}

void Scene_RunTwoCallSequence(void)
{
    Field_unk3_4();
    Field_unk4_4();
}

/*
 * Scene hook that does nothing. The owner is the two-byte return alone; the
 * zero halfwords on either side align it and the entry that follows, and are
 * not part of it.
 */
void State_RunEmptyHook(void)
{
}

void SceneData_InitHeader8590(void)
{
    u16 *hdr = (u16 *)0x02008590;

    hdr[0] = gIw2;
    hdr[1] = gIw3;
    Field_Apply7(hdr, (u8 *)0x020085B0);
}

s32 State_SetRuntimeWord448To256(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *work;
    u8 *disp;
    s32 off = 224;
    s32 *scene;

    /* Reset the scene word at workspace + 448 before entering scene 9. */
    off <<= 1;
    work = *base;
    scene = (s32 *)(work + off);
    off -= 192;
    *scene = off;

    Field_unk5_4(9);

    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0c04;

    disp = base[4];
    {
        u16 *slot = (u16 *)(disp + 0x534);
        int value = 0x3f3f;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x536);
        int value = 31;
        *slot = value;
    }
    {
        u16 *slot = (u16 *)(disp + 0x52a);
        int value = 10;
        *slot = value;
    }

    Field_unk6_4();
    return 0;
}

void Effect_UpdateBg3HofsByVcount(void)
{
    u16 *src;
    u32 value;

    if (gUnk >= gOv) {
        src = &gOv2;
    } else {
        src = &gOv3;
    }
    value = *src;
    gUnk2 = value;
}

/*
 * The owner at 0x020002a0, 64 bytes: 42 bytes of code, the two-byte alignment
 * halfword, and a five-word literal pool at 0x020002cc holding 0x03001e70,
 * 0x02008610, 0x02008614, 0x03001e40 and 0x02008616.
 *
 * It prepares the three words that Effect_UpdateBg3HofsByVcount consumes:
 * a VCOUNT threshold at 0x02008610 and the two BG3HOFS values selected above
 * and below it. 192 is the screen height, so the threshold is a scanline
 * derived from a coordinate in the scene work record.
 *
 * The record cell is read as a union, not as bare halfwords. The word store to
 * 0x02008610 and the halfword reads are only ordered against each other when
 * they can alias, and the reference schedules the 0x02008614 address load ahead
 * of the halfword read on exactly that dependence. Reading the cell through a
 * halfword-only pointer disambiguates the two accesses and loses that order.
 */
void Effect_SetBg3HofsSplit(void)
{
    union SceneCell *work = (union SceneCell *)(gCam + 260);
    s32 hofs;

    gOv = 192 - work[1].h[1];
    gOv2 = hofs = work[0].h[1];
    gOv3 = hofs - (gIw >> 2);
}

void State_ApplyTables826dAnd82a1(void)
{
    Field_Place(1, 0, 0x0200826D);
    Field_Apply8(0x020082A1, 0xC80);
}
