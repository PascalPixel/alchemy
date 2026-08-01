#include "types.h"

/*
 * Core-drive row 0x080f9c44, 76 bytes (kind shared_literal_function_module,
 * admitted by the 2026-07-31 classification rule). Its two far `ldr rN,[pc,#680]`
 * loads resolve past the end of its own compiled out/asm/080f9c44.bin;
 * read directly from the neighbouring out/asm/080f9c90.bin at the matching
 * file offset (0x260/0x264) rather than guessed at, giving 0x03007ff0 and
 * 0x68736d53 -- the SAME "Smsh"-tagged context pointer and magic constant
 * semantic/main/080006fc.c established. Mirrors that file's `SmshContext`
 * local struct (using the settled field layout) rather than raw byte
 * casts, now that it is settled: `countdown` at +0x04 and
 * `countdown_reload` at +0x0b are exactly the two fields this owner touches.
 *
 * If the context's signature is not exactly the magic constant or one past
 * it, this owner does nothing. Otherwise it decrements `countdown`; once
 * that reaches zero or below (the branch tests the zero-extended 32-bit
 * `subs` result, not a re-read-as-signed-byte comparison -- verified
 * against the reference's actual register-level semantics), `countdown` is
 * reloaded from `countdown_reload`, and two DMA-shaped hardware channel
 * control registers (base 0x040000bc, offsets +8 and +20) are each
 * re-armed with 0x84400004 if their own top bit (post `lsls #7`) reads
 * clear, then the paired length halfwords at +10 and +22 are stamped
 * 0x0400 and immediately overwritten with 0xb600 -- transcribed literally,
 * including the apparent redundancy, rather than collapsed to the final
 * value.
 *
 * Complete owner: asm/080f9c44.s in full, entry to `bx lr`, plus its own
 * local two-word pool (0x040000bc, 0x84400004); no other exclusions.
 */

struct SmshContext {
    u32 signature;
    u8 countdown;
    u8 unknown_05;
    u8 audio_param;
    u8 unknown_07[4];
    u8 countdown_reload;
    u8 unknown_0c[4];
    u32 unknown_10;
    u32 unknown_14;
    u32 unknown_18;
    void *unknown_1c;
    u32 unknown_20;
    u32 unknown_24;
    u32 unknown_28;
    void (*disable_callback)(u8);
    u8 unknown_30[0x20];
    u8 channel_entries[12][0x40];
};

extern struct SmshContext *Data_03007ff0;

void Func_080f9c44(void)
{
    struct SmshContext *context = Data_03007ff0;
    u32 *channel = (u32 *)0x040000bc;
    u16 *lengthA = (u16 *)((u8 *)channel + 10);
    u16 *lengthB = (u16 *)((u8 *)channel + 22);
    s32 decremented;

    if (context->signature - 0x68736d53 > 1) {
        return;
    }

    decremented = (s32)context->countdown - 1;
    context->countdown = (u8)decremented;
    if (decremented > 0) {
        return;
    }

    context->countdown = context->countdown_reload;

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
