#include "EDITION.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "RUNTIME_INTERFACES.H"

s32 VramBlock_LoadCached(s32, s32, s32);
extern u8 Resource_FixedBlockBTiles[];

void Runtime_BumpFree(void *buffer);
extern u8 Data_03001e8c[];
u32 Resource_DecodeType01(const void *source, void *destination);
s32 UiText_MeasureEntryDimensions(s32 start, s32 *width, s32 *count, s32 mode);

/* ui/window/copy_tilemap_region.c */
extern u8 *gWindowWork;
extern u8 RomBytes_080310a4[];

/* resource/copy_fixed_block_a.c */
extern const u8 RomBytes_080317e4[];

/* ui/text/draw/draw_padded_label.c */
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, 0x80, Resource_FixedBlockBTiles);
    return slot;
}

#if EDITION_INTERNATIONAL
/* A routine that only reports success, after the fixed resource block
   loader; nothing in the image calls it by name. The Japanese edition checks
   the name entry's voiced marks here instead. */
s32 Resource_ReturnTrue(void)
{
    return 1;
}
#else
/* The letters that take the voiced and the semi-voiced mark, as pairs of
   first and last codes ending at zero. */
extern const u8 NameEntry_DakutenRanges[];
extern const u8 NameEntry_HandakutenRanges[];

/* Whether the name entry may put a mark after a letter: a voiced mark (0xde)
   or semi-voiced mark (0xdf) only follows the letters in its ranges; any
   other code follows anything. */
s32 NameEntry_AcceptsMark(s32 mark, s32 letter)
{
    const u8 *range;

    if ((u32)(mark - 0xde) > 1)
        return 1;
    if (mark == 0xde)
        range = NameEntry_DakutenRanges;
    else
        range = NameEntry_HandakutenRanges;
    for (; *range != 0; range += 2) {
        if (letter >= range[0] && letter <= range[1])
            return 1;
    }
    return 0;
}

/* The letters of a name, not counting its voiced and semi-voiced marks. */
s32 NameEntry_CountLetters(const u8 *name)
{
    s32 count;

    count = 0;
    for (; *name != 0; name++) {
        if ((u8)(*name - 0xde) > 1)
            count++;
    }
    return count;
}
#endif

/* ui/window/window_copy_tilemap_region.c */
void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source)
{
    s16 *mirror = (s16 *)gWindowWork;
    s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
    s16 *input = buffer;
    u32 cell;
    s16 *vram;
    s32 row;

    Resource_DecodeType01(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++) {
        s32 column;

        for (column = 0; column < window->width; column++) {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    Runtime_BumpFree(buffer);
}

/* ui/window/set_tile_attribute_rect.c */
void UiWindow_SetTileAttributeRect(const struct RenderInput *window,
    s32 x, s32 y, s32 width, s32 height, u32 field) {
    u8 *base = *(u8 **)((u32)&Data_03001e8c);

    x += window->x + 1;
    y += window->y + 1;
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
            u16 *cell = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        base[RENDER_DIRTY_OFS] = 1;
    }
}

/* resource/copy_fixed_block_b.c */
void Resource_CopyFixedBlockB(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, RomBytes_080310a4);
}

void Resource_CopyFixedBlockA(s32 arg0)
{
    VramBlock_LoadCached(arg0, 0x80, (s32)RomBytes_080317e4);
}

/* The Japanese edition has neither of the two text helpers that follow. */
#if EDITION_INTERNATIONAL
/* ui/text/text_set_render_string.c */
/* ui/text/misc/set_render_string.c */
s32 UiText_SetRenderString(const u8 *str)
{
    u8 *base;
    u16 *dst;
    s32 count;
    s32 offset;
    s32 count_out;
    s32 width_out;

    base = *(u8 **)((u32)&Data_03001e8c);
    count = 0;
    if (*str != 0) {
        dst = (u16 *)(base + RENDER_ENTRY_TBL_OFS);
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    offset = RENDER_ENTRY_TBL_OFS + count * 2;
    *(u16 *)(base + offset) = 0;
    UiText_MeasureEntryDimensions(0, &count_out, &width_out, 0);
    return count_out;
}

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}
#endif
