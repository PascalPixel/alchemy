// __call_via_rN veneer site, resolved per-site against the ROM.
//
// The `bl Func_080072f4` at 0x080123c6 is `__call_via_r4` -- an indirect call
// through r4, not a call to a function at 0x080072f4.
//
// THE CALLEE IS THE BUFFER THIS FUNCTION JUST FILLED. Both ends check out:
//
//   caller side  -- `ldr r3, [pc, #56]` at 0x080123b2 loads 0x03001e50 from
//                   the pool word at 0x080123ec; `adds r3, #196` then
//                   `ldr r4, [r3, #0]` reads the word at 0x03001f14.
//   allocator side -- Func_080048b0 loads the SAME base 0x03001e50 (its pool
//                   word at 0x080048ec), computes `lsls r5, r0, #2` and stores
//                   the allocation with `str r0, [r4, r5]` at 0x080048de and
//                   0x080048e4. For id 0x31 that slot is
//                   0x03001e50 + 0x31 * 4 = 0x03001f14.
//
// So [0x03001f14] is exactly what Func_080048b0(0x31, 0x27c) returned, and the
// DMA immediately before the branch copies 0x9f words (= 0x27c bytes = 636)
// from 0x08009e7c into it. This is the heap-allocated variant of the
// `__call_via_sp` stack-kernel idiom: DMA a relocatable routine out of ROM,
// then call it. Two independent instruments, not two drafts.
//
// ARITY. The veneer register is r4, which is OUTSIDE the r0-r3 argument
// registers, so the "callee sits in the argument slot matching the register
// index" regularity does NOT apply here -- every argument the draft passed is
// real. r0 and r1 are the incoming arguments (parked in r8/sl across the
// allocation and restored at 0x080123c0/0x080123c2), r2 = 0x0203c000 from the
// pool, r3 = 0x0201c000 + 0x1000 = 0x0201d000. Four arguments.
#include "types.h"

struct DmaTransfer_08012388 {
    const void *source;
    void *destination;
    u32 control;
};

/// The 636-byte routine DMA'd out of ROM at 0x08009e7c and run from the
/// heap slot for allocation id 0x31.
typedef void (*RelocatedKernel)(s32, s32, void *, void *);

/// Slot table for Func_080048b0's allocations: entry n is at
/// 0x03001e50 + n * 4. Entry 0x31 is 0x03001f14.
extern void *Data_03001e50[];

void *Func_080048b0(s32 id, s32 size);
void Func_08002dd8(s32 id);

void Func_08012388(s32 arg0, s32 arg1)
{
    struct DmaTransfer_08012388 *dma =
        (struct DmaTransfer_08012388 *)0x040000d4;
    void *buffer = Func_080048b0(0x31, 0x27c);

    dma->source = (const void *)0x08009e7c;
    dma->destination = buffer;
    dma->control = 0x8400009f;
    ((RelocatedKernel)Data_03001e50[0x31])(
        arg0, arg1, (void *)0x0203c000, (void *)0x0201d000);
    Func_08002dd8(0x31);
}
