#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "inventory_menu.h"
#include "window.h"
#include "types.h"
#include "io_reg.h"
#include "menu_result.h"
#include "fixed_math.h"
#include "dma.h"
#include "scene.h"
#include "global_cells.h"
#include "tbs_edition.h"


void AudioCommand_PlayFar(s32 cue);
void Runtime_SetMainState19(void);

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

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
        AudioCommand_PlayFar(111);
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
        AudioCommand_PlayFar(111);
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
        AudioCommand_PlayFar(111);
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
        AudioCommand_PlayFar(111);
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

void Palette_LightenBankHighlight(s32 bank)
{
    u16 *palette = (u16 *)(0x05000000 + (bank << 5));
    u32 red;
    u32 green;
    u32 blue;

    Dma_Set((const void *)0x050001e0, palette, 0x80000010, REG_DMA3);
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
    blue = palette[4] >> 10;
    green = (palette[4] >> 5) & 31;
    red = palette[4] & 31;
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
            u16 *pixel = (u16 *)(x + (u32)base);
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

void Palette_CopyObjectBankToBackground14(void)
{
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
}

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
