#include "types.h"

/*
 * resource_3bb owner at 0x020023bc, 148 bytes: reserve a resource,
 * refresh a cached cursor value if unset, then DMA a small block into
 * palette RAM and wait for the transfer to finish.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8 spill at 0x020023bc
 * through `pop {r3} / mov r8,r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x0200242e-0x02002436, followed by the six-word literal pool
 * 0x02002438-0x0200244f; the next owner's prologue is at 0x02002450. One
 * argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The DMA3 register block (0x040000d4 = REG_DMA3SAD, +4 = REG_DMA3DAD,
 * +8 = REG_DMA3CNT) is public GBA hardware, spelled out by address per
 * this project's convention for hardware registers -- the same block
 * resource_39c_c_02004fcc.c already uses. The destination
 * (0x050003e0, inside palette RAM) and control word (0x84000008: enable
 * + 16-bit transfer + count 8) are recorded as read, not decoded
 * further; the trailing loop re-reads REG_DMA3CNT and spins while bit 31
 * (enable) is still set, i.e. waits for the transfer to complete.
 *
 * `Func_080001a8`'s first argument is the un-moved return value of the
 * immediately preceding `Func_08000290(0xe7)` call -- kept as a direct
 * chained call rather than introducing an intermediate named variable,
 * since the compiled code never gives it one either.
 *
 * Uncertainty: none of the seven callees are identified beyond call
 * shape; the byte table at 0x0200c0c4 (indexed by `arg0`) and the cached
 * cursor cell at 0x0200ca1c are not identified.
 */

extern s32 Func_08000170(s32 arg0);
extern s32 Func_080001d0(void);
extern s32 Func_08000290(s32 arg0);
extern void Func_080001a8(s32 arg0, s32 arg1);
extern void Func_080001c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_08000178(s32 arg0);

void Func_020023bc(s32 arg0)
{
    volatile u16 *cursor = (volatile u16 *)0x0200ca1c;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u8 *table = (u8 *)0x0200c0c4;
    s32 result = Func_08000170(0x1ca0);
    s32 tableByte;

    if ((short)*cursor == -1) {
        *cursor = (u16)Func_080001d0();
    }

    tableByte = table[arg0];
    if (arg0 == 8) {
        arg0 = 4;
    }

    Func_080001a8(Func_08000290(0xe7), result);

    dma3[0] = (u32)(result + tableByte);
    dma3[1] = 0x050003e0;
    dma3[2] = 0x84000008;

    Func_080001c8((short)*cursor, 0x400, arg0 * 1024 + result + 160, 0);

    while ((dma3[2] & 0x80000000) != 0) {
        /* wait for the DMA3 transfer to finish */
    }

    Func_08000178(result);
}
