/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is NOT byte-exact and the note above once said it was.  It sits in
 * semantic/ and scores 12 differing halfwords at the right size, 72 bytes, so
 * whatever was true when the veneer audit was written stopped being true --
 * most likely at the routing cut that parked several rows in this overlay.  The
 * claim is corrected here rather than deleted, because a stale "byte-exact" in
 * a source comment is read as a reason not to look.
 *
 * The residual is the store-merge shape described below, not the veneer
 * question the rest of this note is about.  The veneer content still stands.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x08003e36  __call_via_r6  ->  the RETURN VALUE of the call at 0x08004938
 *     The callee is whatever that call produced, not a fixed address.
 */
#include "types.h"

u32 Func_08004938(s32 arg0);
s32 Func_080072fc(s32);
void Func_08002df0(void *);

extern u8 Data_08001dc8;
extern u8 Value_000000e0;

/*
 * WHY THIS ROW DOES NOT CLOSE.  The reference merges the three DMA writes into
 * `stmia r3!, {r0, r1, r2}` followed by `subs r3, #12`, where this emits three
 * separate `str`.  That peephole only fires when the three source registers
 * ascend with the offsets they are stored to -- the reference has source in r0,
 * destination in r1 and control in r2 -- and which register holds which value is
 * the allocator's decision.
 *
 * Readings tried here and on 08004838, none of which fired it: the control word
 * computed inside the block and outside it, first and last; no temporaries at
 * all; a struct assigned field by field; a struct initialised at its
 * declaration; either of those stored through a cast or through a pointer
 * variable (all three struct forms spill to the stack and cost 8 bytes); a
 * post-increment walk; indexed stores; the pointer declared first, last, and
 * assigned separately; and the constants declared in both orders.
 *
 * On 08004838, the smallest instance, the residual reduces to the numbering
 * ALONE and nothing else: base pointer r3, the same three pool words loaded in
 * the same order, and the three stores at ascending offsets 0, 4, 8 in the same
 * order -- all identical to the reference.  The only difference left is that gcc
 * numbers the values r2, r1, r0 where the reference has r0, r1, r2.  Assignment
 * order does control the store offset order (assigning in reverse scrambles the
 * stores to +4, +0, +8 and loses even that), but the descending numbering is
 * invariant across every reading that keeps the offsets ascending.  So the source
 * reaches the whole shape except which register number each value gets, which is
 * the one thing it cannot name.
 *
 * WHERE THE DECISION IS MADE.  Compiling the smallest instance with RTL dumps
 * puts it in the global allocator, not in anything the source says.  The three
 * values become pseudos 32, 33 and 34 -- stored at offsets 0, 4 and 8
 * respectively -- and the base becomes 35.  After local allocation all four are
 * still pseudos with IDENTICAL cost vectors and all four prefer LO_REGS; the
 * global pass then assigns 32 -> r2, 33 -> r1, 34 -> r0 and 35 -> r3, i.e.
 * descending for three equal-priority pseudos, where the ROM has ascending.
 *
 * It is not a compiler-family question either: `candidate-show --family` gives 6
 * differing halfwords for both `routed` and `gcc296` and 8 for `old-agbcc`, so no
 * approved compiler here produces the merge from this source.
 *
 * That leaves two possibilities worth separating before anyone spends more on it.
 * Either the original source has something that changes the pseudos' priorities
 * -- a live range that is not identical, which every reading tried above failed
 * to produce -- or this fork's allocator orders equal-priority pseudos
 * differently from the compiler Camelot used.  The second would make these 82
 * owners a compiler discrepancy rather than 82 source problems, and it is
 * checkable against a stock gcc 2.96 ARM build, which this tree does not carry.
 *
 * This is not a local problem.  82 candidates across the main image differ from
 * their reference by exactly this merge, 15,438 bytes in all, and 35 of them are
 * DMA kicks like this one.  No byte-exact owner in the corpus writes 0x040000d4
 * at all, and the retained-assembly classes already carry
 * `deliberate_dma_kick_macro`, so the shape has never been reproduced from C
 * here.  Whoever works out what makes the merge fire unlocks all 82 at once;
 * until then this row is waiting on that, not on a better reading of its own
 * behaviour.
 */
void Func_08003e10(s32 request)
{
    u32 size = (u32)&Value_000000e0;
    void *buffer = Func_08004938(size);
    u32 words = size >> 2;
    u32 control = 0x84000000;

    control |= words;
    {
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001dc8;
        u32 destination = (u32)buffer;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
    Func_080072fc(request);
    Func_08002df0(buffer);
}
