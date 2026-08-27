#include "types.h"
#include "render_input.h"
#include "global_cells.h"

/* Owner-local field access until this runtime workspace layout is recovered
 * elsewhere (shared with field_owner_54.c / runtime_owner_123.c / 0801db70.c,
 * which all reach the same 0x03001EA0 pointer). */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *Data_03001ea0;
extern s8 Data_080367c9[];
extern s8 Data_080367cc[];
extern s8 Data_080367ce[];
extern u8 Data_080367d0[];
extern u8 Data_080367d6[];

void Func_0801d014(void);
void *UiWindow_Create(s32, s32, s32, s32, s32);
struct RenderInput *Func_0801d108(void);
void *RenderResource_CreatePair(s32, struct RenderInput *, s32, s32);
s32 Modulo(s32, s32);
void Func_080a1038(void *);
void RenderResource_LoadFrame(s32, s32, s32);
s32 FixedPoint_Ratio(s32, s32);
void Func_080b0030(void *, s32, s32, s32);
void Func_080164d4(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawCharacter(s32, void *, u32, u32);
void Func_0801ccc0(s32, s32);
void Func_080216b4(void *);
void UiWork_Finalize(void *, s32);
void RenderOutput_ClearList(void *);
void UiText_DrawResource(s32, void *, s32, s32);
void Func_0801d0f0(void);

s32 Func_0801d4cc(void)
{
    u8 *base;
    struct RenderInput *icon;
    s32 page;
    s32 redraw;
    void *win;
    void *pair;
    s8 *pA;
    s8 *pB;
    s8 *p598;
    s32 i;
    s32 slot;
    s32 selected;
    s32 dest;
    s32 result;

    redraw = 1;
    page = 0;
    Func_0801d014();
    base = (u8 *)Data_03001ea0;
    win = UiWindow_Create(1, 2, 0x1C, 3, 2);
    icon = Func_0801d108();
    pair = RenderResource_CreatePair(7, icon, 0x40, -0x30);
    WaitFrames(1);
    pA = (s8 *)(base + 0x594);
    pB = (s8 *)(base + 0x595);

loop_1:
    if (redraw != 0) {
        redraw = 0;
        page = Modulo(page + 5, 5);
        FIELD(base, s8, 0x594 + page) =
            (s8)Modulo(FIELD(base, s8, 0x594 + page) + FIELD(base, s8, 0x599 + page), 5);
        FIELD(base, u16, 0x574) = (u16)page;
        if (*(u8 *)0x03001CA0 != 0) {
            FIELD(base, u8, 0x598) = 0;
        }

        slot = 0x5EC;
        i = 0;
        do {
            FIELD(*(void **)(base + slot), u8, 0xF) = 0xFB;
            Func_080a1038(*(void **)(base + slot));
            selected = 0;
            if (i != FIELD(base, s8, 0x596))
                selected = 1;
            RenderResource_LoadFrame(
                Data_080367c9[i], FIELD(*(void **)(base + slot), u8, 0xE), selected);
            slot += 4;
            i += 1;
        } while (i <= 2);

        slot = 0x5F8;
        i = 0;
        do {
            FIELD(*(void **)(base + slot), u8, 0xF) = 0xFB;
            Func_080a1038(*(void **)(base + slot));
            selected = 0;
            if (i != FIELD(base, s8, 0x597))
                selected = 1;
            RenderResource_LoadFrame(
                Data_080367cc[i], FIELD(*(void **)(base + slot), u8, 0xE), selected);
            slot += 4;
            i += 1;
        } while (i <= 1);

        slot = 0x604;
        i = 0;
        do {
            FIELD(*(void **)(base + slot), u8, 0xF) = 0xFB;
            Func_080a1038(*(void **)(base + slot));
            selected = 0;
            p598 = (s8 *)(base + 0x598);
            if (i != *p598)
                selected = 1;
            RenderResource_LoadFrame(
                Data_080367ce[i], FIELD(*(void **)(base + slot), u8, 0xE), selected);
            slot += 4;
            i += 1;
        } while (i <= 1);

        {
            s32 rowX = icon->x * 8;
            Func_080b0030(
                base + 0x5B4,
                rowX + 0x8C + FixedPoint_Ratio(*pA * 0x3C, FIELD(base, s8, 0x599)),
                icon->y * 8 + 4,
                1);
        }
        {
            s32 rowX = icon->x * 8;
            Func_080b0030(
                base + 0x5C4,
                rowX + 0x8C + FixedPoint_Ratio(*pB * 0x3C, FIELD(base, s8, 0x59A)),
                icon->y * 8 + 0x14,
                1);
        }

        dest = FIELD(base, s8, 0x596) + 0xC0A;
        Func_080164d4(icon, 0xA0, 0x28, 0xC8, 0x30);
        UiText_DrawCharacter(dest, icon, 0xA0, 0x28);

        dest = FIELD(base, s8, 0x597) + 0xC10;
        Func_080164d4(icon, 0xA0, 0x40, 0xB8, 0x48);
        UiText_DrawCharacter(dest, icon, 0xA0, 0x40);

        dest = *p598 + 0xC13;
        Func_080164d4(icon, 0xA0, 0x58, 0xB8, 0x60);
        UiText_DrawCharacter(dest, icon, 0xA0, 0x58);

        Func_0801ccc0(*pA, *pB);

        dest = ((page * 3 + icon->y) * 8) + 4;
        if (page == 0)
            dest += 8;
        Func_080b0030(base + 0x5A4, icon->x * 8, dest, 3);

        RenderOutput_ClearList(win);
        UiText_DrawResource(page + 0xC15, win, 0, 0);
    }

    Func_080216b4(pair);
    WaitFrames(1);

    if ((*(volatile u32 *)ADDR_03001C94 & 4) != 0) {
        Audio_PlayCue(0x70);
        FIELD(base, u16, 0x57E) = FIELD(base, u16, 0x57E) + 1;
        redraw = 1;
        if ((u32)(FIELD(base, u16, 0x57E) << 16) > 0x50000U) {
            FIELD(base, u16, 0x57E) = 0;
        }
        *pA = (s8)Data_080367d0[FIELD(base, u16, 0x57E)];
        *pB = (s8)Data_080367d6[FIELD(base, u16, 0x57E)];
        goto loop_1;
    }

    if (!(*(volatile u32 *)ADDR_03001C94 & 9)) {
        if (!(*(volatile u32 *)ADDR_03001C94 & 2)) {
            if (*(volatile u32 *)ADDR_03001B04 & 0x40) {
                Audio_PlayCue(0x6F);
                page -= 1;
                redraw = 1;
            } else if (*(volatile u32 *)ADDR_03001B04 & 0x80) {
                Audio_PlayCue(0x6F);
                page += 1;
                redraw = 1;
            } else {
                if (*(volatile u32 *)ADDR_03001B04 & 0x20) {
                    Audio_PlayCue(0x6F);
                    FIELD(base, s8, 0x594 + page) = FIELD(base, s8, 0x594 + page) - 1;
                    redraw = 1;
                }
                if (*(volatile u32 *)ADDR_03001B04 & 0x10) {
                    Audio_PlayCue(0x6F);
                    FIELD(base, s8, 0x594 + page) = FIELD(base, s8, 0x594 + page) + 1;
                    redraw = 1;
                }
            }
            goto loop_1;
        }
        result = -1;
        Audio_PlayCue(0x71);
    } else {
        result = 0;
        Audio_PlayCue(0x70);
    }

    UiWork_Finalize(win, 2);
    UiWork_Finalize(icon, 2);

    if (result == 0) {
        *(u8 *)0x02000445 = FIELD(base, u8, 0x594);
        *(u8 *)0x02000446 = FIELD(base, u8, 0x595);
        *(u8 *)0x0200044C = FIELD(base, u8, 0x596);
        *(u8 *)0x0200044A = FIELD(base, u8, 0x597);
        {
            u8 v = FIELD(base, u8, 0x598);
            *(u8 *)0x0200046A = v;
            *(u8 *)0x03001D08 = v;
        }
    } else {
        Func_0801ccc0(*(s8 *)0x02000445, *(s8 *)0x02000446);
    }

    Func_0801d0f0();
    WaitFrames(1);
    return result;
}
