#include "types.h"

/*
 * Core-drive row 0x080f9c44, 76 bytes (kind shared_literal_function_module,
 * admitted per Vale's 2026-07-31 ruling). Its two far `ldr rN,[pc,#680]`
 * loads resolve past the end of its own compiled out/asm/080f9c44.bin;
 * read directly from the neighbouring out/asm/080f9c90.bin at the matching
 * file offset (0x260/0x264) rather than guessed at, giving 0x03007ff0 and
 * 0x68736d53 -- the SAME "Smsh"-tagged context pointer and magic constant
 * semantic/main/080006fc.c already established, reusing that file's
 * `Data_03007ff0` name and type.
 *
 * If the context's first word is not exactly the magic constant or one
 * past it, this owner does nothing. Otherwise it decrements a countdown
 * byte at context+4; once that countdown reaches zero or below, it is
 * reloaded from context+11, and two DMA-shaped hardware channel control
 * registers (base 0x040000bc, offsets +8 and +20) are each re-armed with
 * 0x84400004 if their own top bit (post `lsls #7`) reads clear, then the
 * paired length halfwords at +10 and +22 are stamped 0x0400 and
 * immediately overwritten with 0xb600 -- transcribed literally, including
 * the apparent redundancy, rather than collapsed to the final value.
 *
 * Complete owner: asm/080f9c44.s in full, entry to `bx lr`, plus its own
 * local two-word pool (0x040000bc, 0x84400004); no other exclusions.
 */

extern void *Data_03007ff0;

void Func_080f9c44(void)
{
    u32 *context = (u32 *)Data_03007ff0;
    u8 *countdown = (u8 *)context + 4;
    u8 *reload = (u8 *)context + 11;
    u32 *channel = (u32 *)0x040000bc;
    u16 *lengthA = (u16 *)((u8 *)channel + 10);
    u16 *lengthB = (u16 *)((u8 *)channel + 22);

    if (context[0] - 0x68736d53 > 1) {
        return;
    }

    {
        s32 decremented = (s32)*countdown - 1;
        *countdown = (u8)decremented;
        if (decremented > 0) {
            return;
        }
    }

    *countdown = *reload;

    if ((channel[2] << 7) == 0) {
        channel[2] = 0x84400004;
    }
    if ((channel[5] << 7) == 0) {
        channel[5] = 0x84400004;
    }

    *lengthA = 0x0400;
    *lengthB = 0x0400;
    *lengthA = 0xb600;
    *lengthB = 0xb600;
}
