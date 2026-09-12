#include "types.h"

#define Audio_PlayCue Func_020014c2
#define PALETTE ((volatile u16 *)0x05000000)
#define SceneData_GetTable96c8 Func_02000030
#define SceneData_GetTable9710 Func_0200003c
#define SceneData_GetTable971c Func_02000044
#define PlayWorkspaceCueAndClearPaletteZero Func_0200004c
#define FieldScene_Forward11fc Func_02000070
#define FieldScene_ConfigureFixedPointValues Func_0200007c
#define SceneData_GetTable97AC Func_020000a8
#define SceneState_CountDownEveryFortyTicks Func_02000168
#define SceneState_StoreSlotZeroField12 Func_020003c0
#define SceneData_StoreRecord1Field12 Func_020003dc
#define SceneState_StoreSlotThreeField12 Func_020003f8
#define SceneData_StoreRecord2Field12 Func_02000414
#define SceneEffect_LoadTablesAndStopDma0 Func_02001188

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern u8 *Data_03001ebc;
extern s32 Data_020097f4;
extern s32 Data_020097f0;
extern s32 Data_0200980c;
extern s32 Data_02009810;
extern s32 Data_02009814;
extern s32 Data_02009818;
extern u16 Data_02008f31[];
extern u16 Data_02008f81[];

void Func_020014c2(s32 cue);
void Func_020011fc(void);
void Func_02001172(s32, s32, s32, s32, s32, s32, s32);
T *Func_0200179c(s32);
T *Func_020017b8(s32);
T *Func_020017d4(s32);
T *Func_020017f0(s32);
void Func_020024ba(void *);
void Func_020024c0(void *);

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
    Audio_PlayCue(*(s16 *)(Data_03001ebc + 364));
    do {
        u16 color = PALETTE == 0;
        register volatile u16 *palette = PALETTE;

        palette[color] = color;
    } while (0);
}

void FieldScene_Forward11fc(void)
{
    Func_020011fc();
}

void FieldScene_ConfigureFixedPointValues(void)
{
    Func_02001172(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *SceneData_GetTable97AC(void) { return (u8 *)0x020097ac; }

void SceneState_CountDownEveryFortyTicks(void)
{
    s32 n = Data_020097f4 + 1;

    Data_020097f4 = n;
    if (n == 40) {
        if (Data_020097f0 > 4) {
            Data_020097f0 -= 1;
            Data_020097f4 = 0;
        }
    }
}

s32 SceneState_StoreSlotZeroField12(void)
{
    s32 *d;
    T *p;

    d = &Data_0200980c;
    p = Func_0200179c(0);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord1Field12(void)
{
    s32 *p;
    T *rec;

    p = &Data_02009810;
    rec = Func_020017b8(1);
    *p = rec->unk12;
    return 0;
}

s32 SceneState_StoreSlotThreeField12(void)
{
    s32 *d;
    T *p;

    d = &Data_02009814;
    p = Func_020017d4(3);
    *d = p->unk12;
    return 0;
}

s32 SceneData_StoreRecord2Field12(void)
{
    s32 *d;
    T *p;

    d = &Data_02009818;
    p = Func_020017f0(2);
    *d = p->unk12;
    return 0;
}

void SceneEffect_LoadTablesAndStopDma0(void)
{
    volatile u16 *reg;

    Func_020024ba(Data_02008f31);
    Func_020024c0(Data_02008f81);
    reg = (volatile u16 *)0x040000B0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
