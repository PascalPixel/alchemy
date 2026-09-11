#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/effect/table_audio.h"

#define PALETTE ((volatile u16 *)0x05000000)

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern u8 *gWork;
extern s32 gOv;
extern s32 gOv2;
extern s32 gOv3;
extern s32 gOv4;
extern s32 gOv5;
extern s32 gOv6;
extern u16 gOv7[];
extern u16 gOv8[];

T *Effect_Run(s32);
T *Effect_Run2(s32);
T *Effect_Run3(s32);
T *Effect_Run4(s32);

/* Return this overlay's state block. */

/*
 * Overlay resource_3ca. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable96c8(void)
{
    return (u8 *)0x020096c8;
}

/*
 * Overlay resource_3ca. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable9710(void)
{
    return (u8 *)0x02009710;
}

/*
 * Overlay resource_3ca. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable971c(void)
{
    return (u8 *)0x0200971c;
}

void PlayWorkspaceCueAndClearPaletteZero(void)
{
    Audio_PlayCue(*(s16 *)(gWork + 364));
    do {
        u16 color = PALETTE == 0;
        register volatile u16 *palette = PALETTE;

        palette[color] = color;
    } while (0);
}

void Scene_Forward(void)
{
    Effect_Run5();
}

void Scene_ConfigureFixedPointValues(void)
{
    Effect_SetRect(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *SceneData_GetTable97AC(void) { return (u8 *)0x020097ac; }

void State_CountDownEveryFortyTicks(void)
{
    s32 n = gOv + 1;

    gOv = n;
    if (n == 40) {
        if (gOv2 > 4) {
            gOv2 -= 1;
            gOv = 0;
        }
    }
}

s32 State_StoreSlotZeroField12(void)
{
    s32 *d;
    T *p;

    d = &gOv3;
    p = Effect_Run(0);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord1Field12(void)
{
    s32 *p;
    T *rec;

    p = &gOv4;
    rec = Effect_Run2(1);
    *p = rec->unk12;
    return 0;
}

s32 State_StoreSlotThreeField12(void)
{
    s32 *d;
    T *p;

    d = &gOv5;
    p = Effect_Run3(3);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord2Field12(void)
{
    s32 *d;
    T *p;

    d = &gOv6;
    p = Effect_Run4(2);
    *d = p->unk12;
    return 0;
}

void Effect_LoadTablesAndStopDma0(void)
{
    volatile u16 *reg;

    Effect_Do(gOv7);
    Effect_Do2(gOv8);
    reg = (volatile u16 *)0x040000B0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
