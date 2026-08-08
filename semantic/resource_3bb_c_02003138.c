#include "types.h"

/*
 * resource_3bb owner at 0x02003138, 196 bytes: if the looked-up object's
 * state byte (offset 0x54) is 1, DMA a small block from the owner
 * record, run a short setup sequence on the owner record, then clear
 * and remask several of the owner record's fields.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl spill at
 * 0x02003138 through `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov
 * sl,r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020031ee-0x020031fa;
 * a four-word literal pool sits mid-body at 0x020031dc-0x020031eb,
 * reached only by branches that skip it (the same inline-pool shape
 * resource_39c_c_02002844.c/02002e08.c and
 * resource_3bb_c_02001ba8.c document); the next owner's prologue
 * follows immediately at 0x020031fc, no trailing pool. Two arguments
 * (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * The DMA3 register block (0x040000d4 = REG_DMA3SAD, control word
 * 0x85000020: enable + 32 words) is the same block
 * resource_39c_c_02004fcc.c and resource_3bb_c_020023bc.c already
 * document.
 *
 * Uncertainty: none of the six callees are identified beyond call
 * shape; `object`'s field 0x54 (state byte, gates the whole body) and
 * 0x5c, and `owner`'s many touched fields (0x1c, 0x28, 5, 8, 0x25/0x26,
 * 0x27) are inferred only from this call shape.
 */

extern u8 *Func_0808a080(s32 arg0);
extern u8 *Func_08000140(s32 arg0, s32 arg1);
extern void Func_08015250(s32 arg0);
extern s32 Func_080001c8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08000150(s32 arg0);
extern void Func_08009060(s32 arg0);

void Func_02003138(s32 arg0, s32 arg1)
{
    u8 *object = Func_0808a080(arg0);
    u8 state;
    u8 *owner;
    u8 *dest;
    s32 result;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u32 stackWord = 0;

    if (object == 0) {
        return;
    }

    if (state != 1) {
        return;
    }
    state = object[0x54];

    owner = *(u8 **)(object + 0x50);
    dest = Func_08000140(17, 0x608) + 0x400;

    dma3[0] = (u32)&stackWord;
    dma3[1] = (u32)dest;
    dma3[2] = 0x85000020;

    Func_08015250(arg1);

    result = Func_080001c8(owner[28], 128, (s32)dest);
    Func_08000150(17);

    object[0x5c] = state;

    *(s32 *)(owner + 0x28) = 0;
    Func_08009060(*(s32 *)(owner + 0x28));
    owner[5] &= (u8)~0x20;
    *(u16 *)(owner + 8) = (*(u16 *)(owner + 8) & 0xfc00) | (result & 0x3ff);
    owner[0x25] = 0;
    owner[0x26] = 0;
    owner[0x27] = 0;
}
