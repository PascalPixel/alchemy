/*
 * Overlay resource_397: a field scene that shifts its two tracked objects by
 * whole blocks, blends the display for scene 9, and scrolls BG3 against the
 * vertical counter.
 */

#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_SetActorEightValue3d Func_02000030
#define SceneData_GetTable835c Func_02000040
#define SceneData_ReturnZero Func_02000048
#define SceneData_GetTable844c Func_0200004c
#define SceneData_GetTable8474 Func_02000054
#define SceneActor_ShiftObjectsByBlock Func_0200005c
#define FieldScene_ApplyOffset0Pos5 Func_020000d8
#define FieldScene_ApplyOffset0Neg5 Func_020000e8
#define FieldScene_ApplyOffset0Pos5Second Func_020000f8
#define FieldScene_ApplyOffset0Neg5Second Func_02000108
#define FieldScene_ApplyOffset0Pos6 Func_02000118
#define FieldScene_ApplyOffset0Neg6 Func_02000128
#define SceneState_SetValue123ThenCounter16c Func_02000138
#define SceneEffect_SetAlphaBlendForScene9 Func_0200015c
#define SceneData_GetTable84a4 Func_020001b4
#define FieldScene_RunTwoCallSequence Func_020001bc
#define SceneState_RunEmptyHook Func_020001d0
#define SceneData_InitHeader8590 Func_020001d4
#define SceneState_SetRuntimeWord448To256 Func_02000200
#define SceneEffect_UpdateBg3HofsByVcount Func_0200026c
#define SceneEffect_SetBg3HofsSplit Func_020002a0
#define SceneState_ApplyTables826dAnd82a1 Func_020002e0

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

extern s32 Data_02000240[];
extern u8 *Data_03001e70;
extern u8 *Data_03001ebc;
extern u32 Data_03001e40;
extern s32 Data_03001ae8;
extern s32 Data_03001b04;
extern u16 Data_04000006;
extern s32 Data_02008610;
extern u16 Data_02008614;
extern u16 Data_02008616;
extern u16 Data_0400001c;

void Func_0200037c(s32, s32);
u8 *Func_020003ac();
s32 Func_020003b4();
s32 Func_020003da();
void Func_0200013c(s32, s32);
s32 Func_0200014e(s32, s32);
s32 Func_0200016e(s32, s32);
void Func_0200017c(s32, s32);
s32 Func_0200018e(s32, s32);
void Func_02000496(s32);
void Func_0200048c(s32);
void Func_020004ae();
void Func_020004e4(void);
void Func_020004f0(void);
void Func_020004fc(u16 *, u8 *);
void Func_02000560();
void Func_0200051e();
void Func_020005f6(s32, s32, s32);
s32 Func_020005f8(s32, s32);

void SceneState_SetActorEightValue3d(void)
{
    Func_0200037c(8, 0x3D);
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

void SceneActor_ShiftObjectsByBlock(s32 bx, s32 bz)
{
    u8 *work = *(u8 **)0x03001ebc;
    struct SceneObject *obj;
    s32 dx = bx;
    s32 dz = bz;
    s32 h;

    /* Block coordinates become 16.16 fixed-point shifts of sixteen tiles. */
    obj = (struct SceneObject *)Func_020003ac(Data_02000240[125]);
    dx <<= 20;
    dz <<= 20;

    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Func_020003b4((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }

    /* Apply the same shift to the workspace's independently optional object. */
    obj = *(struct SceneObject **)(work + 480);
    if (obj != 0) {
        obj->x += dx;
        obj->z += dz;
        h = Func_020003da((s32)obj->layer, obj->x, obj->z);
        obj->y = h;
        obj->settled_y = h;
    }
}

void FieldScene_ApplyOffset0Pos5(void)
{
    Func_0200013c(0, 5);
}

void FieldScene_ApplyOffset0Neg5(void)
{
    Func_0200014e(0, -5);
}

void FieldScene_ApplyOffset0Pos5Second(void)
{
    SceneActor_ShiftObjectsByBlock(0, 5);
}

void FieldScene_ApplyOffset0Neg5Second(void)
{
    Func_0200016e(0, -5);
}

void FieldScene_ApplyOffset0Pos6(void)
{
    Func_0200017c(0, 6);
}

void FieldScene_ApplyOffset0Neg6(void)
{
    Func_0200018e(0, -6);
}

void SceneState_SetValue123ThenCounter16c(void)
{
    u8 *state = Data_03001ebc;
    s16 *cnt;

    Func_02000496(0x7B);
    cnt = (s16 *)(state + 0x16C);
    Func_0200048c(*cnt);
}

void SceneEffect_SetAlphaBlendForScene9(void)
{
    u8 *disp;

    /* Start the scene, then configure alpha blending for its display state. */
    Func_020004ae(9);

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

void FieldScene_RunTwoCallSequence(void)
{
    Func_020004e4();
    Func_020004f0();
}

/*
 * Scene hook that does nothing. The owner is the two-byte return alone; the
 * zero halfwords on either side align it and the entry that follows, and are
 * not part of it.
 */
void SceneState_RunEmptyHook(void)
{
}

void SceneData_InitHeader8590(void)
{
    u16 *hdr = (u16 *)0x02008590;

    hdr[0] = Data_03001ae8;
    hdr[1] = Data_03001b04;
    Func_020004fc(hdr, (u8 *)0x020085B0);
}

s32 SceneState_SetRuntimeWord448To256(void)
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

    Func_02000560(9);

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

    Func_0200051e();
    return 0;
}

void SceneEffect_UpdateBg3HofsByVcount(void)
{
    u16 *src;
    u32 value;

    if (Data_04000006 >= Data_02008610) {
        src = &Data_02008614;
    } else {
        src = &Data_02008616;
    }
    value = *src;
    Data_0400001c = value;
}

/*
 * The owner at 0x020002a0, 64 bytes: 42 bytes of code, the two-byte alignment
 * halfword, and a five-word literal pool at 0x020002cc holding 0x03001e70,
 * 0x02008610, 0x02008614, 0x03001e40 and 0x02008616.
 *
 * It prepares the three words that SceneEffect_UpdateBg3HofsByVcount consumes:
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
void SceneEffect_SetBg3HofsSplit(void)
{
    union SceneCell *work = (union SceneCell *)(Data_03001e70 + 260);
    s32 hofs;

    Data_02008610 = 192 - work[1].h[1];
    Data_02008614 = hofs = work[0].h[1];
    Data_02008616 = hofs - (Data_03001e40 >> 2);
}

void SceneState_ApplyTables826dAnd82a1(void)
{
    Func_020005f6(1, 0, 0x0200826D);
    Func_020005f8(0x020082A1, 0xC80);
}
