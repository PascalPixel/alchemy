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
 * The magic constant and the structure at *(u32*)0x03007ff0 are otherwise
 * unidentified; not guessed at here.
 */

extern void *Data_03007ff0;

/* Retained asm, "approved_compiler_cannot_express_contract" -- called only
 * as a tail branch, opaque from this caller's side. */
void Func_0800070c(void *signature_holder);

void Func_080006fc(void)
{
    void *signature_holder = Data_03007ff0;

    if (*(u32 *)signature_holder == 0x68736d53) {
        Func_0800070c(signature_holder);
        return;
    }
}
