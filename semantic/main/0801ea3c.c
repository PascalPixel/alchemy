#include "layout_guard.h"
#include "types.h"

typedef struct TextWindow_0801ea3c {
    u8 padding00[0x0c];
    u16 tile_x;
    u16 tile_y;
} TextWindow_0801ea3c;

typedef struct TextCommand_0801ea3c {
    u16 words[7];
} TextCommand_0801ea3c;

LAYOUT_OFFSET_GUARD(
    TextWindow0801ea3c_TileX,
    TextWindow_0801ea3c,
    tile_x,
    0x0c);

extern u8 *Data_03001e8c;

u8 *Func_08017dd4(u8 *buffer, s32 value, s32 digits);
void Func_0801de5c(
    const u16 *command,
    void *map_destination,
    void *vram_destination,
    s32 subpixel_x);

/* Build and place a four-digit tile command in the window's text map. */
void Func_0801ea3c(
    s32 value,
    const TextWindow_0801ea3c *window,
    u32 pixel_x,
    u32 pixel_y,
    s32 alternate_style)
{
    u8 converted[16];
    u8 *digits = Func_08017dd4(converted, value, 4);
    TextCommand_0801ea3c command;
    u32 column;
    u32 row;
    u32 map_index;
    s32 index;

    command.words[0] = alternate_style == 0 ? 0xf01d : 0xf01f;
    command.words[1] = 0xf01e;
    for (index = 0; index < 4; index++)
        command.words[index + 2] = digits[index];
    command.words[6] = 0;

    column = window->tile_x + (pixel_x >> 3);
    row = window->tile_y + (pixel_y >> 3) + 1;
    map_index = (row << 5) + column + 1;
    if (map_index < 0x280) {
        u32 byte_offset = map_index * 2;

        Func_0801de5c(
            command.words,
            Data_03001e8c + byte_offset,
            (u8 *)0x06002000 + byte_offset,
            pixel_x & 7);
    }
}
