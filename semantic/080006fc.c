#include "types.h"

/*
 * Owner at 0x080006fc (16 bytes incl. pool, part of the core-band census).
 *
 * `ldr r0,[pc,#108]` through `beq.n 0x800070c / bx lr` -- a leaf, no
 * prologue/epilogue, no C-eligible pool word aside from two literal
 * addresses/constants.
 *
 * Loads the pointer stored at fixed RAM address 0x03007ff0, dereferences it,
 * and compares the first word there against the 32-bit magic constant
 * 0x68736d53 ("Smsh" read as bytes). If it matches, TAIL-BRANCHES (no `bl`,
 * no return here) into Func_0800070c, passing the same pointer through in
 * r0 -- that target's own manifest evidence says it expects exactly this
 * ("conditional_prologue_after_guard"). If it does not match, this function
 * just returns.
 *
 * Func_0800070c itself is retained assembly and explicitly marked
 * unconvertible (manifest evidence: "manual_high_register_and_stack_frame_
 * contract, tail_branch_to_iwram_without_link, approved_compiler_cannot_
 * express_contract") -- it stays an opaque extern. What THIS function does
 * is expressible: a magic-signature guard in front of an IWRAM trampoline.
 *
 * SmshContext is named here rather than promoted to include/ -- nothing
 * using it is exact-adopted yet (Func_0800070c stays opaque, and this file
 * and semantic/main/080f9c44.c are both semantic-tier drafts), so it
 * follows the same local-struct convention as
 * semantic/main/080fae58.c and semantic/main/080f9ef8.c rather than
 * touching the include/ registry ahead of an exact owner. Field layout
 * below is mirrored in 080f9c44.c until one of these goes exact and the
 * struct can be promoted for real.
 *
 * Fields 0x00/0x04/0x0b/0x2c/the 12x64-byte array at 0x50 come from the
 * exact src/080fa8d4.c (ground truth). Fields 0x06/0x10/0x14/0x18/0x1c/
 * 0x20/0x24/0x28 come from reading Func_0800070c's retained-asm body
 * directly (asm/0800070c.s) -- unconvertible, but its own field accesses
 * are ordinary reads and fair evidence for this struct's shape, same as
 * this project already reads other retained-asm bodies for structure
 * without claiming to convert them. Total size is at least 0x350 (848)
 * bytes -- 0x50 + 12*0x40 exactly matches Func_0800070c's own literal pool
 * constant 0x00000350, independent corroboration this is the real stride.
 */

struct SmshContext {
    u32 signature;      /* 0x00: 0x68736d53 ("Smsh") */
    u8 countdown;         /* 0x04 */
    u8 unknown_05;
    u8 audio_param;         /* 0x06: passed as r0 into the IWRAM
                              * trampoline, role not identified */
    u8 unknown_07[4];
    u8 countdown_reload;      /* 0x0b */
    u8 unknown_0c[4];
    u32 unknown_10;             /* 0x10: combined with a fixed pool base
                                  * and this struct's own address before
                                  * the trampoline call; role not
                                  * identified */
    u32 unknown_14;               /* 0x14: passed as r9 into the trampoline */
    u32 unknown_18;                 /* 0x18: passed as ip into the trampoline */
    void *unknown_1c;                 /* 0x1c: pointer to a separate
                                        * linked/array structure, walked in
                                        * 64-byte strides, up to 4 entries
                                        * (src/080fa8d4.c) */
    u32 unknown_20;                    /* 0x20: if nonzero, triggers an
                                         * extra Func_0800070a(field_0x24)
                                         * call before the unconditional
                                         * Func_0800070a(field_0x28) */
    u32 unknown_24;
    u32 unknown_28;
    void (*disable_callback)(u8);        /* 0x2c: exact, src/080fa8d4.c */
    u8 unknown_30[0x20];
    u8 channel_entries[12][0x40];          /* 0x50-0x34f: exact,
                                             * src/080fa8d4.c; only byte 0
                                             * of each entry established
                                             * (active/state flag) */
};

extern struct SmshContext *Data_03007ff0;

void Func_0800070c(void *signature_holder);

void Func_080006fc(void)
{
    struct SmshContext *context = Data_03007ff0;

    if (context->signature == 0x68736d53) {
        Func_0800070c(context);
        return;
    }
}
