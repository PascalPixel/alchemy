/* NONMATCHING: initialize the battle effect's 16-row tile map.
 * A Value_ mask restores the word load; a volatile cell keeps the VRAM
 * base in r8. Still missing the signed tile conversion; hoisted row
 * constants require an extra high register. Instruction topology is equal
 * (65 instructions), not a complete byte match. */
#include "TYPES.H"

struct CanvasMapCell {
    s32 tile : 16;
};

extern u8 Value_00006784[];
void BattleFx_BeginCanvasLayer(s32 mode);

void Func_080cdb24(u32 display_control)
{
    s32 row;
    s32 tile;
    s32 palette;
    s32 first_column;
    s32 second_column;
    s32 row_tile;
    s32 tile_index;
    u8 *offset;

    BattleFx_BeginCanvasLayer(display_control);
    *(volatile u16 *)0x0400000c = display_control | (u32)Value_00006784;

    offset = (u8 *)0;
    row = 0;
    row_tile = 0;
    tile_index = 0;
    do {
        tile = row_tile + 0x100;
        palette = tile_index << 1;
        first_column = 0;
        do {
            first_column++;
            ((volatile struct CanvasMapCell *)(0x06003800 + (s32)offset))->tile = tile | palette;
            tile += 0x200;
            palette += 2;
            offset += 2;
        } while (first_column != 8);

        second_column = 0;
        do {
            second_column++;
            *(u16 *)(0x06003800 + (s32)offset) = 0;
            offset += 2;
        } while (second_column != 8);

        row_tile += 0x1000;
        row++;
        tile_index += 8;
    } while (row != 16);
}
