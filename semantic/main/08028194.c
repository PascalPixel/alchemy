#include "layout_guard.h"
#include "types.h"

#define NULL ((void *)0)

struct MenuItemSprite_08028194 {
    u32 oam_word0;
    u32 oam_word1;
    u32 oam_word2;
    s16 x;
    s16 y;
    u16 padding10;
    u16 shape;
};

struct MenuRenderState_08028194 {
    struct MenuItemSprite_08028194 items[7];
    s16 selected_item;
    s16 item_count;
    s16 padding90;
    s16 padding92;
    s16 border_side;
};

struct ShapeEntry_08028194 {
    u16 size;
    u16 tile_offset;
};

LAYOUT_SIZE_GUARD(
    MenuItemSprite08028194_Size,
    struct MenuItemSprite_08028194,
    0x14);
LAYOUT_OFFSET_GUARD(
    MenuItemSprite08028194_X,
    struct MenuItemSprite_08028194,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    MenuItemSprite08028194_Y,
    struct MenuItemSprite_08028194,
    y,
    0x0e);
LAYOUT_OFFSET_GUARD(
    MenuItemSprite08028194_Shape,
    struct MenuItemSprite_08028194,
    shape,
    0x12);
LAYOUT_OFFSET_GUARD(
    MenuRenderState08028194_SelectedItem,
    struct MenuRenderState_08028194,
    selected_item,
    0x8c);
LAYOUT_OFFSET_GUARD(
    MenuRenderState08028194_ItemCount,
    struct MenuRenderState_08028194,
    item_count,
    0x8e);
LAYOUT_OFFSET_GUARD(
    MenuRenderState08028194_BorderSide,
    struct MenuRenderState_08028194,
    border_side,
    0x94);
LAYOUT_SIZE_GUARD(
    ShapeEntry08028194_Size,
    struct ShapeEntry_08028194,
    4);

extern struct MenuRenderState_08028194 *Data_03001f38;
extern s32 Data_03001e40;
extern u8 *Data_03001ecc;
extern struct ShapeEntry_08028194 Data_03001b10[];

s32 Func_08003d28(const u32 *affine);
void Func_08003dec(void *sprite, s32 priority);

static s32 WaveCoordinate_08028194(void) {
    const u16 *wave = (const u16 *)0x080366F8;
    s32 sample = wave[(Data_03001e40 * 2) & 31];

    return (sample - 0x100) / 4 + 0x130;
}

static u16 ShapeTile_08028194(const struct MenuItemSprite_08028194 *item) {
    return Data_03001b10[item->shape].tile_offset >> 5;
}

static void DrawItemSprite_08028194(
    struct MenuItemSprite_08028194 *item,
    s32 selected,
    s32 coordinate,
    s32 affine_slot
) {
    s32 x = item->x;
    s32 y = item->y;
    s32 priority = 0xf5;

    if (selected) {
        x += coordinate * 7 / 512 - 20;
        if (y != 0) {
            y += coordinate * 3 / 256 - 20;
        } else {
            y = coordinate * 15 / 256 - 30;
        }
        priority = 0xf6;
    }

    item->oam_word0 = 0;
    item->oam_word1 =
        0x80002000 |
        ((u32)x << 16) |
        (u8)y |
        (selected ? 0x300 | ((u32)affine_slot << 25) : 0);
    item->oam_word2 = ShapeTile_08028194(item);
    Func_08003dec(item, priority);
}

static u8 *ActiveBorderPage_08028194(u8 *owner) {
    return owner + owner[0x539] * 0x284;
}

static u16 *BorderCell_08028194(u8 *page, s32 index) {
    return (u16 *)(page + 6 + index * 4);
}

static void SetBorderSpan_08028194(
    u8 *page,
    s32 index,
    s32 center,
    s32 radius
) {
    u16 *cell = BorderCell_08028194(page, index);

    *cell = (u16)((*cell & 0x00ff) |
        ((u32)(center - radius) << 8) + center + radius + 0x17);
}

static void SetBorderCenter_08028194(u8 *page, s32 index, s32 center) {
    u16 *cell = BorderCell_08028194(page, index);

    *cell = (u16)((*cell & 0x00ff) | ((u32)center << 8));
}

static void DrawUpperBorder_08028194(
    struct MenuRenderState_08028194 *state,
    u8 *page,
    s32 coordinate
) {
    const struct MenuItemSprite_08028194 *selected =
        &state->items[state->selected_item];
    s32 radius = (coordinate * 12 - 0x0b01) / 256;
    s32 end = selected->y + 25 + (coordinate * 32 - 0x1f01) / 512;
    s32 center = state->items[0].x;
    s32 index;

    for (index = 24; index < end; index++) {
        SetBorderSpan_08028194(page, index, selected->x, radius);
    }

    if (state->selected_item == 0) {
        center -= radius;
    }
    for (index = 0; index <= 23; index++) {
        SetBorderCenter_08028194(page, index, center);
    }
}

static void DrawLowerBorder_08028194(
    struct MenuRenderState_08028194 *state,
    u8 *page,
    s32 coordinate
) {
    const struct MenuItemSprite_08028194 *selected =
        &state->items[state->selected_item];
    s32 radius = (coordinate * 12 - 0x0b01) / 256;
    s32 start = selected->y - (coordinate * 32 - 0x1f01) / 512 - 1;
    s32 center = state->items[0].x;
    s32 index;

    for (index = start; (u32)index <= 135; index++) {
        SetBorderSpan_08028194(page, index, selected->x, radius);
    }

    if (state->selected_item == 0) {
        center -= radius;
    }
    for (index = 136; index <= 159; index++) {
        SetBorderCenter_08028194(page, index, center);
    }
}

/*
 * Animate the current menu item with an affine pulse, submit every visible
 * item to OAM, and reshape the active border page around the selection.
 */
void Func_08028194(void) {
    struct MenuRenderState_08028194 *state = Data_03001f38;
    s32 coordinate = WaveCoordinate_08028194();
    u32 affine[2] = {
        (u16)coordinate | ((u32)(u16)coordinate << 16),
        0,
    };
    s32 affine_slot = Func_08003d28(affine);
    s32 index;

    for (index = 0; index < state->item_count; index++) {
        struct MenuItemSprite_08028194 *item = &state->items[index];

        if (item->x != 0) {
            DrawItemSprite_08028194(
                item,
                index == state->selected_item,
                coordinate,
                affine_slot);
        }
    }

    if (Data_03001ecc != NULL && state->item_count != 0) {
        u8 *page = ActiveBorderPage_08028194(Data_03001ecc);

        if (state->border_side == 0) {
            DrawUpperBorder_08028194(state, page, coordinate);
        } else {
            DrawLowerBorder_08028194(state, page, coordinate);
        }
    }
}
