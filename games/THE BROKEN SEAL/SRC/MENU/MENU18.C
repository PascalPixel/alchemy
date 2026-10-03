#include "CHARACTER_MENU.H"
#include "INVENTORY_MENU.H"
#include "WINDOW.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "MENU_RESULT.H"
#include "FIXED_MATH.H"
#include "DMA.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

/* Move a row or page using repeat keys. Returns -1 for no input, zero for
 * a row change and one for a page change. Keep result lifetimes local to
 * each decision and join row changes before the common result exit. */
extern volatile u32 gKeysRepeat;
void Link_DrawShiftedTilePairFar(s32 addr);
void Audio_PlayCue(s32 cue);
void Runtime_SetMainState19(void);

void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 style);
#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

void UiIcon_PrepareObject(struct RenderOutput *icon);

s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor, s32 *page)
{
    s32 pages;
    s32 next;
    s32 previous;
    s32 page_forward;
    s32 result;

    result = -1;
    if (count == 0)
        goto done;
    Link_DrawShiftedTilePairFar(0x06002500);
    pages = count / per_page;
    if (count % per_page != 0)
        pages++;
    if (horizontal) {
        next = gKeysRepeat & KEY_RIGHT;
        previous = gKeysRepeat & KEY_LEFT;
        horizontal = gKeysRepeat & KEY_UP;
        page_forward = gKeysRepeat & KEY_DOWN;
    } else {
        next = gKeysRepeat & KEY_DOWN;
        previous = gKeysRepeat & KEY_UP;
        horizontal = gKeysRepeat & KEY_LEFT;
        page_forward = gKeysRepeat & KEY_RIGHT;
    }
    if (horizontal) {
        Audio_PlayCue(111);
        if (--*page < 0)
            *page = pages - 1;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (page_forward) {
        Audio_PlayCue(111);
        if (++*page > pages - 1)
            *page = 0;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (previous) {
        Audio_PlayCue(111);
        if (--*cursor < 0) {
            *cursor = per_page - 1;
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        goto row_changed;
    }
    result = -1;
    if (next == 0)
        goto done;
    {
        Audio_PlayCue(111);
        result = 0;
        if (++*cursor == count - *page * per_page)
            *cursor = result;
        if (*cursor > per_page - 1)
            *cursor = result;
    }
row_changed:
    result = 0;
done:
    return result;
}

/* Copies palette bank 15 over the given object bank, then lightens its
   colour 4 by 9 in each channel, saturating at 31. */
void Palette_LightenBankHighlight(s32 bank)
{
    u16 *palette = (u16 *)(0x05000000 + (bank << 5));
    u32 color;
    u32 red;
    u32 green;
    u32 blue;

    Dma_Set((const void *)0x050001e0, palette, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, (volatile u32 *)0x040000d4);
    color = palette[4];
    /* FAKEMATCH: the (u16) casts on an already 16-bit colour produce the
       reference's shift pair. 2026-10-02: a u16 color local without these
       casts changes the load to ldrsh and shifts the red mask before use. */
    blue = (u16)color >> 10;
    green = ((u16)color >> 5) & 31;
    red = color & 31;
    blue += 9;
    if (blue > 31)
        blue = 31;
    green += 9;
    if (green > 31)
        green = 31;
    red += 9;
    if (red > 31)
        red = 31;
    palette[4] = (blue << 10) | (green << 5) | red;
}

void Menu_DrawPageIndicator(
    s32 window,
    s32 item_count,
    s32 page_size,
    s32 selected_page,
    s32 right_edge
)
{
    s32 page_count;
    s32 page;
    s32 x;
    s32 tile;

    x = right_edge;
    tile = PAGE_LABEL_FIRST;
    page_count = item_count / page_size;
    if (item_count % page_size != 0)
        page_count++;

    x -= page_count;
    if (page_count > 1) {
        UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

        for (page = 0; page < page_count; page++) {
            if (page == selected_page)
                UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2);
            else
                UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3);
            tile++;
            x++;
        }

        UiWindow_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
    }
}

void Render_SetTilemapFlagRect(
    const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u8 *base = (u8 *)work->tilemap;

    x += object->x + 1;
    y += object->y + 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *pixel = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *pixel;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *pixel = value;
                pixel++;
            }
            height--;
            x += 64;
        } while (height != 0);
        work->dirty = 1;
    }
}

/* Copies object palette bank 0 over background bank 14, then copies object
   colour 4 of bank 0 into background bank 14's entry 14. */
void Palette_CopyObjectBankToBackground14(void)
{
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, (volatile u32 *)0x040000d4);
}

/*
 * Hides every entry icon, then shows one page of them: up to page_size
 * icons from the first entry, placed in a column at (x, y), 16 pixels
 * apart, stopping at a missing icon or the end of the list.
 */
void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y)
{
    struct InventoryMenuState *menu = gMenuWork;
    struct RenderOutput *icon;
    s32 i;

    for (i = 0; i < 32; i++) {
        icon = menu->entry_icons[i];
        if (icon != NULL)
            icon->active = 13;
    }
    for (i = first; i < page_size + first && (icon = menu->entry_icons[i]) != NULL
         && i <= menu->item_count - 1; i++) {
        icon->x = x;
        icon->y = y + (i - first) * 16;
        UiIcon_PrepareObject(icon);
        icon->active = 1;
    }
}
