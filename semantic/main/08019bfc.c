#include "types.h"

struct DecoderState_08019bfc {
    u32 symbol;
    const u8 *input;
    s32 input_bits;
};

struct DecoderTable_08019bfc {
    const u8 *tree;
    const u16 *offsets;
};

extern const struct DecoderTable_08019bfc Data_0803842c[];

u32 Func_08019bfc(struct DecoderState_08019bfc *state)
{
    const struct DecoderTable_08019bfc *table;
    const u8 *tree;
    const u8 *packed_end;
    const u8 *input;
    s32 input_bits;
    s32 tree_bits;
    s32 leaf;
    s32 bit;
    u32 symbol;

    symbol = state->symbol;
    table = &Data_0803842c[symbol >> 8];
    tree = table->tree + table->offsets[(u8)symbol];
    packed_end = tree - 1;
    input = state->input;
    input_bits = state->input_bits;
    tree_bits = 1;
    leaf = 0;

    for (;;) {
        bit = tree_bits & 1;
        tree_bits >>= 1;
        if (bit == 0) {
read_input:
            bit = input_bits & 1;
            input_bits >>= 1;
            if (bit != 0) {
                if (input_bits == 0) {
                    s32 byte = *input++;

                    bit = byte & 1;
                    input_bits = (byte >> 1) | 0x80;
                }
                if (bit != 0) {
                    s32 depth = 0;

                    do {
                        bit = tree_bits & 1;
                        tree_bits >>= 1;
                        if (bit != 0) {
                            if (tree_bits == 0) {
                                s32 byte = *tree++;

                                bit = byte & 1;
                                tree_bits = (byte >> 1) | 0x80;
                            }
                            if (bit != 0) {
                                leaf++;
                                depth--;
                            } else {
                                depth++;
                            }
                        } else {
                            depth++;
                        }
                    } while (depth >= 0);
                }
            }
            continue;
        }

        if (tree_bits == 0) {
            s32 byte = *tree++;

            bit = byte & 1;
            tree_bits = (byte >> 1) | 0x80;
        }
        if (bit == 0)
            goto read_input;
        break;
    }

    {
        u32 packed_index = leaf * 3;
        const u8 *packed = packed_end - (packed_index >> 1);

        if (((leaf * 12) & 7) == 0)
            symbol = ((u32)packed[0] << 4) | ((u32)packed[-1] >> 4);
        else
            symbol = packed[-1] | ((u32)(packed[0] & 0x0f) << 8);
    }

    state->input_bits = input_bits;
    state->symbol = symbol;
    state->input = input;
    return symbol;
}
