// __call_via_rN veneer site, resolved per-site against the ROM.
//
// The `bl Func_080072f0` at 0x08021c0e is `__call_via_r3` -- an indirect call
// through r3. The draft's fourth argument, `*(u32 *)(header + 196)`, is the
// callee load, not an argument.
//
// The callee is the buffer this function just filled. 0x03001e50 + 196 is the
// slot for allocation id 49 (0x31): Func_080048b0 stores its result with
// `str r0, [r4, r5]` (0x080048de, 0x080048e4) where r4 = 0x03001e50 and
// r5 = id * 4, and 0x31 * 4 = 196. The DMA immediately above copies 0x278
// bytes from 0x08015afc into that same buffer.
//
// The allocator arithmetic was read at 0x080048b0 itself, and it independently
// underwrites 0x08012388 and 0x080f02b0 -- which DMA *different* ROM routines
// into this one slot. Three call sites, three payloads, one relocation buffer;
// so the payload is a property of the site, never something to carry across.
//
// ARITY. Three arguments, callee in the r3 slot (r0-r3 are the argument
// registers, which is exactly why the draft mistook the callee for argument
// four). Register agreement at the branch: r0 = *slot, r1 = object, r2 = slot,
// r3 = callee at 0x08021c0a.
#include "types.h"

extern u8 Data_08015afc;
extern u8 Data_03001e50;
extern u8 Value_00000278;
extern u8 Value_00000604;

/// The 0x278-byte routine DMA'd out of ROM at 0x08015afc into slot 49 and
/// entered through the veneer.
typedef void (*RelocatedKernel)(u32 value, u8 *object, u32 *slot);

void *Func_080048b0(s32, u32);
void Func_08002dd8(s32);

void Func_08021be0(u8 *object)
{
    u32 size = (u32)&Value_00000278;
    void *buffer = Func_080048b0(49, size);
    u32 control = 0x84000000;
    u32 words = size >> 2;
    u32 *dma = (u32 *)0x040000d4;
    u32 source = (u32)&Data_08015afc;
    u32 destination = (u32)buffer;

    control |= words;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    {
        u8 *header = (u8 *)&Data_03001e50;
        u32 *slot = (u32 *)(object + (u32)&Value_00000604);

        ((RelocatedKernel)*(u32 *)(header + 196))(*slot, object, slot);
    }
    Func_08002dd8(49);
}
