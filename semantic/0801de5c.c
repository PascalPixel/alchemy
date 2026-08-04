/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 *
 * 0x03000164 is NOT established -- see the audit note. Typed by arity only.
 */
#include "types.h"

typedef void (*ArmFill)(void *destination, u32 size, u32 value);
typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

struct TextTileState_0801de5c {
    u8 unknown_000[0xda0];
    u8 tile_in_use[0x100];
    u16 next_tile;
    u8 expanded_tiles;
    u8 unknown_ea3[4];
    u8 palette_bank;
    u8 unknown_ea8[6];
    u16 glyph_style;
};

extern struct TextTileState_0801de5c *Data_03001e8c;

void *Func_08004938(u32 size);
const u8 *Func_08002f40(s32 resource_id);
void Func_08002df0(void *allocation);

/*
 * Render an encoded text run into an eight-row scratch raster, pack it into
 * 4bpp tiles, allocate missing tile indices, and upload the completed tiles.
 */
u32 Func_0801de5c(
    const u16 *tokens,
    u16 *tilemap_a,
    u16 *tilemap_b,
    s32 pixel_offset)
{
    struct TextTileState_0801de5c *state = Data_03001e8c;
    u8 *scratch = Func_08004938(0x800);
    const u8 *font = Func_08002f40(0x13);
    const u8 *width_table = (const u8 *)0x080370d4;
    const u8 *palette_table = (const u8 *)0x080371b4;
    u8 translation[16];
    u32 palette_bits = (u32)state->palette_bank << 12;
    u32 columns;
    u32 row;

    /*
     * The relocated copy routine receives the allocation in r2.  The
     * allocator leaves the successful allocation in both r0 and r2; spelling
     * the source explicitly preserves that behavior without relying on a
     * volatile-register accident.
     */
    ((Resident_03000164)0x03000164)(
        translation,
        sizeof(translation),
        (u32)scratch);

    if (palette_bits == 0xf000) {
        translation[1] =
            palette_table[state->glyph_style & 0x0f];
        translation[3] = 3;
        ((ArmFill)0x03000168)(
            scratch,
            0x800,
            0x04040404);
    } else {
        translation[1] = state->glyph_style & 0x0f;
        translation[3] = 1;
        ((ArmFill)0x03000168)(
            scratch,
            0x800,
            0x0e0e0e0e);
    }

    if (tokens != 0) {
        u16 token;

        while ((token = *tokens++) != 0) {
            if (token <= 30) {
                switch (token) {
                case 3:
                    pixel_offset += width_table[0];
                    break;

                case 7:
                case 9:
                case 10:
                    state->glyph_style = 15;
                    translation[1] = palette_table[15];
                    break;

                case 8: {
                    u16 style = *tokens++;

                    state->glyph_style = style;
                    translation[1] =
                        palette_table[style & 0x0f];
                    break;
                }

                case 11:
                case 12:
                case 17:
                case 29:
                    tokens++;
                    break;

                case 14:
                case 15:
                case 28:
                    tokens += 2;
                    break;

                default:
                    break;
                }
            } else {
                const u32 *glyph =
                    (const u32 *)(font + (u8)token * 32);
                u8 *destination = scratch + pixel_offset;

                for (row = 0; row < 8; row++) {
                    u32 pixels = glyph[row];
                    u32 pair;

                    for (pair = 0; pair < 4; pair++) {
                        u8 color =
                            translation[pixels & 0x0f];

                        if (color != 0)
                            destination[0] = color;
                        pixels >>= 4;
                        color = translation[pixels & 0x0f];
                        if (color != 0)
                            destination[1] = color;
                        pixels >>= 4;
                        destination += 2;
                    }
                    destination += 0xf8;
                }

                if (token == 0xf01d || token == 0xf01f)
                    pixel_offset += 8;
                else if (token == 0xf01e)
                    pixel_offset += 3;
                else if ((u8)token > 31)
                    pixel_offset +=
                        width_table[(u8)token - 32];
                else
                    pixel_offset++;
            }
        }
    }

    columns = ((u32)pixel_offset + 7) >> 3;

    /*
     * Collapse each row's eight one-byte pixels into one 4bpp tile word.
     * Source and destination both retain a 0x100-byte row stride.
     */
    for (row = 0; row < 8; row++) {
        u8 *source = scratch + row * 0x100;
        u32 *destination =
            (u32 *)(scratch + row * 0x100);
        u32 column;

        for (column = 0; column < columns; column++) {
            u32 low = *(u32 *)source;
            u32 high = *(u32 *)(source + 4);
            u32 low_pairs =
                (low | (low << 4)) & 0x0ff00ff0;
            u32 high_pairs =
                (high | (high >> 4)) & 0x00ff00ff;

            low_pairs |= low_pairs << 8;
            high_pairs |= high_pairs >> 8;
            *destination++ =
                ((low_pairs << 4) >> 16) |
                (high_pairs << 16);
            source += 8;
        }
    }

    for (row = 0; row < columns; row++) {
        u32 tile = *tilemap_a & 0x03ff;

        if ((u32)(tile - 0x80) > 0x7f &&
            (state->expanded_tiles == 0 ||
             tile < 0x200 ||
             tile >= 0x280)) {
            u32 mask =
                state->expanded_tiles != 0 ? 0xff : 0x7f;
            u32 slot = state->next_tile & mask;

            if (state->tile_in_use[slot] != 0) {
                u32 probes = 0;

                do {
                    probes++;
                    slot = (slot + 1) & mask;
                } while (
                    probes <= mask &&
                    state->tile_in_use[slot] != 0);
            }

            state->next_tile = (slot + 1) & mask;
            state->tile_in_use[slot] = 1;
            if (slot > 0x7f)
                tile = slot + 0x180;
            else
                tile = slot | 0x80;

            *tilemap_a =
                (u16)(palette_bits | tile);
            *tilemap_b =
                (u16)(palette_bits | tile);
        }

        {
            u32 *packed = (u32 *)(scratch + row * 4);
            volatile u32 *vram =
                (volatile u32 *)(0x06000000 + tile * 32);
            u32 tile_row;

            for (tile_row = 0; tile_row < 8; tile_row++)
                vram[tile_row] =
                    packed[tile_row * 0x40];
        }

        tilemap_a++;
        tilemap_b++;
    }

    Func_08002df0(scratch);
    return columns;
}
