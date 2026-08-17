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
 * WHY THIS ROW DOES NOT CLOSE: it was never C.
 *
 * The reference merges the three DMA writes into `stmia r3!, {r0, r1, r2}` plus a
 * `subs r3, #12`, where this emits three separate `str`.  An earlier version of
 * this note called that a peephole nobody had worked out yet and said whoever did
 * would unlock 82 owners.  That was wrong, and wrong in the expensive direction:
 * `bef5cad7c` settled it from arm.md, which gates both store-multiple peepholes
 * on TARGET_ARM.  Stock gcc 2.96 cannot emit a Thumb multiple-transfer from ANY
 * source, so there is no reading of this function that produces the reference.
 * The region is hand-written assembly or a library object.
 *
 * `residual_class` now returns `unemittable` for a reference-side stmia or ldmia,
 * so the ranking says this rather than leaving it to be rediscovered.  Only the
 * reference side counts: the same mnemonic on our side is an ordinary defect in
 * our source.
 *
 * The readings tried here are kept because they are still true about the source,
 * just not the route to a match: the control word computed inside the block and
 * outside it, first and last; no temporaries at all; a struct assigned field by
 * field; a struct initialised at its declaration; either stored through a cast or
 * a pointer variable (all three struct forms spill and cost 8 bytes); a
 * post-increment walk; indexed stores; the pointer declared first, last and
 * assigned separately; constants in both declaration orders.
 *
 * The related constant-sharing question is settled too, and separately: two
 * identical (const_int N) trees are unified by cse and reload gives the result a
 * hard register instead of rematerialising it.  Identical trees, so no spelling
 * reaches that either.
 *
 * Its header also claimed "This file is byte-exact, so nothing below is
 * rewritten", while sitting in semantic/ at 12 differing halfwords.  Corrected in
 * place rather than deleted: a stale byte-exact claim reads as a reason not to
 * look, and the veneer content the note exists for is still sound.
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
