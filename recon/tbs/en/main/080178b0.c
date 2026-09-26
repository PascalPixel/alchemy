/* NONMATCHING: complete 436-byte glyph-pair renderer [080178b0,08017a64),
   including its literal pool. Current candidate is 436 bytes, 5 differing
   halfwords / 5 aligned edits, improved from 424 bytes and 128 aligned edits.
   Corrected the stack-relative buffer offsets (buffer starts at sp+4), the
   second glyph's expansion address, and the signed width increment. Separate
   index and width locals recover sl; assigning transparency before colour in
   both branches shares the fp assignment. Value-returning clear and expansion
   prototypes reproduce their argument schedules; frame size is exactly 388.
   Residual: the last non-outline pair advances dst in r5 before its first
   call; the reference keeps r5 intact and computes dst+32 in r1 afterwards.
   Wrapping the second call in a one-pass loop only delays the r5 update and
   changes the first call's schedule. A void expansion prototype also changes
   that schedule; indexing buf[length+offset] additionally swaps r5/r6.
   u16 colour adds a spill and grows the frame; keep the s32 colour local. */
#include "TYPES.H"

struct GlyphRuntime {
    u8 unknown_000[0xea4];
    u8 fixed_colour;
    u8 unknown_ea5[7];
    u16 outlined;
    u16 colour;
};

extern struct GlyphRuntime *Data_03001e8c;
extern u8 Data_08032224[];
extern u8 Data_08031e24[];

typedef s32 (*ExpandGlyphFn)(const void *, void *, s32);

s32 Func_080178b0(s32 code, u32 *out)
{
    u8 buf[384];
    struct GlyphRuntime *work;
    const u8 *input;
    s32 length;
    s32 colour;
    s32 transparent;
    s32 second_index;
    s32 second_width;
    u8 *digits;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    work = Data_03001e8c;
    second_index = (s32)((u32)code << 8) >> 16;
    code &= 255;
    ((s32 (*)(void *, s32))0x03000164)(buf, 384);
    if (work->fixed_colour != 0) {
        transparent = 0;
        colour = 8;
    } else {
        transparent = 1;
        colour = work->colour;
    }
    input = Data_08032224 + ((code - 32) << 5);
    length = *(const u16 *)input;
    input += 2;
    if (work->outlined == 1) {
        ExpandGlyphFn expand = (ExpandGlyphFn)0x03000214;
        expand(input, buf + 49, transparent);
        expand(input, buf + 50, transparent);
        expand(input, buf + 32, colour);
        expand(input, buf + 33, colour);
        length++;
    } else {
        ExpandGlyphFn expand = (ExpandGlyphFn)0x03000214;
        expand(input, buf + 49, transparent);
        expand(input, buf + 32, colour);
    }
    if ((u16)second_index != 0) {
        input = Data_08031e24 + ((u16)second_index << 5);
        second_width = *(const s16 *)input;
        input += 2;
        if (work->outlined == 1) {
            u8 *dst = buf + length;
            ExpandGlyphFn expand = (ExpandGlyphFn)0x03000214;
            expand(input, dst + 49, transparent);
            expand(input, dst + 50, transparent);
            expand(input, dst + 32, colour);
            expand(input, dst + 33, colour);
            /* FAKEMATCH: retain the shifted signed-halfword increment. */
            second_width = (s32)(((u32)second_width << 16) + 0x10000) >> 16;
        } else {
            u8 *dst = buf + length;
            ExpandGlyphFn expand = (ExpandGlyphFn)0x03000214;
            expand(input, dst + 49, transparent);
            expand(input, dst + 32, colour);
        }
        length += (u16)second_width;
    }

    digits = buf + 7;
    group = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                value = 0;
                nibble = 7;
                do {
                    value = (value << 4) + *digits--;
                    nibble--;
                } while (nibble >= 0);
                *out++ = value;
                column++;
                digits += 24;
            } while (column <= 7);
            row++;
            digits -= 120;
        } while (row <= 1);
        group++;
        digits += 112;
    } while (group <= 1);
    return length;
}
