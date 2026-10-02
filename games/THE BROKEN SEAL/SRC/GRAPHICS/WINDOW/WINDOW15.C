#include "TYPES.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "IO_REG.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"

/* The eight-pixel row `up` rows above the bottom row of a background tile
   in the first character block. */
#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

s32 Runtime_GetLowTableAddress(void);
extern const u8 Tile_BuildMetatiles[];
extern const s8 UiWindow_PartyColumnOffsets[];
extern u8 Tile_BuildMetatilesCodeSize[];
extern void *Data_03001e8c;
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);
typedef void (*RamRoutine)(void *dst, void *scene);

extern u8 Data_03001e90[];

struct UiWindowBounds {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 height;
    u16 flags;
};

extern s32 BattleParty_PrepareActiveOwnersFar(s32);
extern s32 Party_CountActiveOwnersFar(void);
void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_DrawPartyStatusContents(s32);
void *Runtime_AllocateBlock(s32 flags, s32 arg1);
s32 UiWindow_Create(u16, u16, u16, u16, s32);

/* With no scene loaded, fills the 160-entry buffer with 0xe0e0; otherwise
   copies the metatile builder into RAM and runs it over the buffer and the
   scene record. */
void UiWindow_FillFromScene(void *dst)
{
    void *scene = Data_03001e8c;

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, (volatile u32 *)0x040000d4);
    } else {
        u32 size = (u32)Tile_BuildMetatilesCodeSize;
        RamRoutine code = (RamRoutine)Runtime_BumpAllocate(size);

        Dma_Set((void *)Tile_BuildMetatiles, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        code(dst, scene);
        Runtime_BumpFree(code);
    }
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)((u32)&Data_03001e90);
    struct UiWindowBounds *state = *slot;
    u8 *base = *(u8 **)(slot - 1);
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (base[RENDER_MENU_STATE_OFS] != 0) {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    } else {
        n = Party_CountActiveOwnersFar();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->left = left;
    state->top = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct UiWindowBounds *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)((u32)&Data_03001e8c) + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->height, 6);
    UiWindow_DrawPartyStatusContents(flags);
    *busy = zero;
}

/* Draws the dividers between the party columns of a status window: a
   joint on the top row, a foot on the bottom row and a bar between. The
   wide layout shifts every divider five tiles and adds the leftmost one.
   In the menu the bottom edge is then redrawn as a plain border. */
void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    u8 *base = Data_03001e8c;
    s32 first = 1;
    s32 bias = 0;
    u32 max = window->width - 1;
    s32 i;
    u32 rows = window->height;
    u32 col;
    u32 row;
    u16 *dest;

    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2) {
        bias = 5;
        first = 0;
    }
    i = first;
    while (UiWindow_PartyColumnOffsets[i] >= 0) {
        col = UiWindow_PartyColumnOffsets[i] + bias;
        if (col < max) {
            for (row = 0; row != rows; row++) {
                dest = (u16 *)(((window->y + row) * 32 + (window->x + col)) * 2 + (u32)base);
                if (row == 0)
                    *dest = 0xf018;
                else if (row == rows - 1)
                    *dest = 0xf019;
                else
                    *dest = 0xf00f;
            }
        }
        i++;
    }
    if (base[RENDER_MENU_STATE_OFS]) {
        dest = (u16 *)base + (window->y + window->height - 1) * 32 + window->x;
        col = 1;
        *dest++ = 0xf080;
        while (col < max) {
            col++;
            *dest++ = 0xf081;
        }
        *dest = 0xf082;
    }
    base[RENDER_DIRTY_OFS] = 1;
}

/* Recolours one five-tile status bar in place: value pixels of forty are
   filled. In each tile's bottom row, or its bottom three rows for a bar
   that is not empty, the bar colour 14 and its shadow 1 become 8 and 13
   where filled and 2 and 12 where empty. Outside the menu it first loads
   the bar palette. The epilogue keeps r0, so the function was declared
   with a result; it never returns one. */
s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value)
{
    s32 i;
    u8 *base = Data_03001e8c;
    s32 filled = value;
    s32 row;
    s32 col;
    u32 id;
    u32 light;
    u32 dark;
    u32 pixels;
    u32 result;

    if (base[RENDER_MENU_STATE_OFS] == 0) {
        Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(14, 0),
            0x80000010, REG_DMA3);
        BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
    }
    x += window->x;
    y += window->y;
    for (i = 0; i < 5; i++, x++) {
        id = *(u16 *)(base + ((y * 32 + x) << 1));
        light = 0x22222222;
        dark = 0xcccccccc;
        id &= 0x3ff;
        if (value > 7) {
            light = 0x88888888;
            dark = 0xdddddddd;
        } else if (value >= 0) {
            light <<= value * 4;
            light |= 0x88888888U >> (32 - value * 4);
            dark <<= value * 4;
            dark |= 0xddddddddU >> (32 - value * 4);
        }
        for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
            pixels = *TILE_ROW_FROM_BOTTOM(id, row);
            result = 0;
            for (col = 0; col <= 7; col++) {
                u32 color = pixels & 15;

                if (color == 14)
                    result |= light & (15U << (col * 4));
                else if (color == 1)
                    result |= dark & (15U << (col * 4));
                else
                    result |= color << (col * 4);
                pixels >>= 4;
            }
            *TILE_ROW_FROM_BOTTOM(id, row) = result;
        }
        value -= 8;
    }
}

/* The render work bytes this window reads: whether a battle is running,
   the busy mark while the window is redrawn, and the text palette. */
struct StatusRenderWork {
    u8 unknown_000[RENDER_MENU_STATE_OFS];
    u8 battle;
    u8 busy;
    u8 palette;
};

struct StatusParty {
    u8 unknown_00[0x58];
    u16 owners[4];
};

void UiWindow_EraseBorderRect(s32, s32, u32, u32);
void RenderOutput_RedrawSavedRect(struct RenderInput *);
void UiWindow_DrawFrame(s32, s32, u32, u32);
struct BattleUnit *Owner_GetStateFar(s32);
void UiWork_SetParamNibble(s32);
void UiText_DrawStringAtOffset(u8 *, struct RenderInput *, s32, s32);
s32 BattlePlacement_CountValidEntriesFar(s32, u8 *);
s32 __divsi3(s32, s32);
void UiWindow_SetTilemapEntry(struct RenderInput *, s32, s32, s32, u32);
/* Both are called as functions returning int, and the glyph call passes
   four arguments: the game's code had no prototype for either in scope. */
s32 UiText_DrawPrefixedNumberAtOffset();
s32 UiWindow_PutGlyph();

/* Draws the party status window: each active member's name, HP and its
   bar, PP and its bar on the tall layout, and the four Djinn counts in the
   wide battle layout. Flags of -1 keep the window's current layout; when
   the layout changes the window is erased, resized and framed again. */
void UiWindow_DrawPartyStatusContents(s32 flags)
{
    void **slot = (void **)Data_03001e90;
    struct UiWindowBounds *layout = slot[0];
    struct StatusParty *party = slot[-7];
    struct RenderInput *window = (struct RenderInput *)layout->handle;
    struct StatusRenderWork *work = slot[-1];
    s32 x = 0;
    u32 count;
    s32 y = 0;
    u32 i;
    u16 owners[5];
    u8 djinn[4];

    if (work->battle != 0) {
        count = BattleParty_PrepareActiveOwnersFar(0);
        y = -1;
        for (i = 0; i < count; i++) {
            owners[i] = party->owners[i];
            if (owners[i] == 255)
                break;
        }
    } else {
        count = Party_CountActiveOwnersFar();
        for (i = 0; i < count; i++)
            owners[i] = gGameState.active_owners[i];
        owners[i] = 255;
    }
    count = i;
    if (flags == -1)
        flags = layout->flags;
    if (!(flags & 1))
        flags &= ~2;
    if (work->battle == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
        flags &= ~2;
    if (flags == 9) {
        UiWindow_EraseBorderRect(layout->left, layout->top, layout->right, layout->height);
        return;
    }
    work->busy = 1;
    if (layout->flags == flags) {
        RenderOutput_RedrawSavedRect(window);
        UiWindow_DrawColumnBorders(window, flags);
    } else {
        UiWindow_EraseBorderRect(layout->left, layout->top, layout->right, layout->height);
        UiWindow_BuildLayoutBounds(flags);
        window->width = layout->right;
        window->height = layout->height;
        window->x = layout->left;
        UiWindow_DrawFrame(layout->left, layout->top, layout->right, layout->height);
        UiWindow_DrawColumnBorders(window, flags);
    }
    if (flags & 2)
        x = 5;
    for (i = 0; i != count; i++) {
        struct BattleUnit *unit = Owner_GetStateFar(owners[i]);
        s32 hp = unit->hp;
        s32 maximum = unit->max_hp;
        s32 value;
        s32 current;

        if (hp == 0)
            UiWork_SetParamNibble(2);
        else if (hp <= maximum / 4)
            UiWork_SetParamNibble(4);
        else
            UiWork_SetParamNibble(15);
        work->palette = 14;
        if (work->battle != 0)
            work->palette = 5;
        UiText_DrawPrefixedNumberAtOffset(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
        work->palette = 15;
        UiText_DrawStringAtOffset(unit->name, window, (x + i * 6) * 8, y * 8);
        UiWork_SetParamNibble(15);
        if (unit->max_hp != 0) {
            current = unit->hp;
            value = __divsi3(current * 40, unit->max_hp);
            if (value == 0 && current != 0)
                value = 1;
            UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 2, value);
        }
        if (flags & 1) {
            work->palette = 14;
            if (work->battle != 0)
                work->palette = 5;
            UiText_DrawPrefixedNumberAtOffset(unit->pp, window, (x + i * 6) * 8, y * 8 + 16, 1);
            if (unit->max_pp != 0) {
                current = unit->pp;
                value = __divsi3(current * 40, unit->max_pp);
                if (value == 0 && current != 0)
                    value = 1;
                UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 3, value);
            }
        }
    }
    work->palette = 15;
    if (work->battle != 0 && (flags & 2)) {
        s32 row = y;

        if (flags & 1)
            row++;
        BattlePlacement_CountValidEntriesFar(0, djinn);
        UiWindow_SetTilemapEntry(window, 0x5001, 0, row, 0);
        UiWindow_SetTilemapEntry(window, 0x5002, 2, row, 0);
        UiWindow_SetTilemapEntry(window, 0x5003, 0, row + 1, 0);
        UiWindow_SetTilemapEntry(window, 0x5004, 2, row + 1, 0);
        UiWindow_PutGlyph(window, djinn[0] + '0', 1, row);
        UiWindow_PutGlyph(window, djinn[1] + '0', 3, row);
        UiWindow_PutGlyph(window, djinn[2] + '0', 1, row + 1);
        UiWindow_PutGlyph(window, djinn[3] + '0', 3, row + 1);
    }
    work->busy = 0;
}
