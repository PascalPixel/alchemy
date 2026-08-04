#include "types.h"

/*
 * Append one encoded text fragment to a 512-code-unit circular buffer.
 *
 * Mode 1, and mode 3's first half, can prepend one of eight short templates.
 * Mode 2, and mode 3's second half, add an "es" suffix, omitting the "e"
 * when the copied fragment did not end in S/s.  A nonzero `framed` argument
 * adds the control-code framing used by the caller.
 */
s32 Func_08017e88(
    s32 framed,
    u16 *text,
    s32 write_index,
    u16 *output,
    s32 mode,
    s32 second_half,
    s32 *ends_in_s)
{
    const s8 *const *templates =
        (const s8 *const *)0x08033E40;
    u16 code;

    if (framed != 0) {
        output[write_index] = 0x20;
        write_index = (write_index + 1) & 0x1FF;
        output[write_index] = 0x0A;
        write_index = (write_index + 1) & 0x1FF;
        output[write_index] = 0x0A;
        write_index = (write_index + 1) & 0x1FF;
    }

    if (mode == 1 || (mode == 3 && second_half == 0)) {
        s32 template_index = 0;
        u16 first_code = text[0];
        const s8 *prefix;
        s32 index;

        if (first_code == 0x1D) {
            template_index = text[1] - 1;
            text += 2;
        }

        if (template_index == 0) {
            switch (first_code) {
            case 0x41:
            case 0x45:
            case 0x49:
            case 0x4F:
            case 0x55:
                template_index = 2;
                break;

            default:
                template_index = 1;
                break;
            }
        }

        prefix = templates[template_index & 7];
        for (index = 0; index < 8 && prefix[index] != 0; index++) {
            output[write_index] = (s16)prefix[index];
            write_index = (write_index + 1) & 0x1FF;
        }
    } else if (text[0] == 0x1D) {
        text += 2;
    }

    code = *text;
    while (code != 0) {
        output[write_index] = code;
        write_index = (write_index + 1) & 0x1FF;
        *ends_in_s = code == 0x53 || code == 0x73;
        text++;
        code = *text;
    }

    if (mode == 2 || (mode == 3 && second_half != 0)) {
        if (*ends_in_s != 0) {
            output[write_index] = 0x65;
            write_index = (write_index + 1) & 0x1FF;
        }
        output[write_index] = 0x73;
        write_index = (write_index + 1) & 0x1FF;
    }

    if (framed != 0) {
        output[write_index] = 0x0A;
        write_index = (write_index + 1) & 0x1FF;
        output[write_index] = 8;
        write_index = (write_index + 1) & 0x1FF;
        output[write_index] = 0x20;
        write_index = (write_index + 1) & 0x1FF;
    }

    return write_index;
}
