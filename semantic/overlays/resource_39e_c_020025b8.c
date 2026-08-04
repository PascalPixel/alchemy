typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x020025b8, 164 bytes: a per-frame callback that
 * builds a randomised parameter block on the stack and hands it to this
 * overlay's own Func_0200013c.
 *
 * Complete owner: `push {r5, r6, r7, lr}` and `sub sp, #56` at
 * 0x020025b8 through `add sp, #56 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x02002648-0x0200264e, then the three-word literal pool
 * 0x02002650-0x0200265b (0xb333, 0xcccc, 0x03001e40). The next owner's
 * prologue is exactly at 0x0200265c. **164 bytes**, measured to the
 * epilogue.
 *
 * WHY IT HAS NO CALLER, ANSWERED RATHER THAN ASSERTED. This row is a
 * published owner reached by no `bl`, and the owner that publishes it is
 * its own neighbour 0x020026d8: the pool word at 0x02002754 is
 * **0x0200a5b9**, which is ODD. Under the published-pointer rule
 * `(0x0200a5b9 & ~1) - 0x8000 = 0x020025b8` -- this function. 0x020026d8
 * passes that word to Func_080000d0 with 200<<4 on the way in and to
 * Func_080000d8 on the way out, so it is a register/unregister pair
 * around a scene. That is the same mechanism recorded on
 * resource_3a4's 0x02002b58, and it is the concrete reason a call-graph
 * sweep cannot see this row: its address is a plain word in an argument,
 * never a branch target.
 *
 * All 6 `bl` targets resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 25b8 265c` under the
 * `+2` rule. Five are veneers; one is the overlay-internal Func_0200013c,
 * whose eight-argument signature is taken from the already-drafted
 * semantic/overlays/resource_39e_c_0200013c.c rather than guessed.
 *
 * THE ARGUMENT BLOCK IS FOUR STACK WORDS PLUS A STRUCT, and the split
 * matters. Thumb passes a1-a4 in r0-r3 and a5 onward on the stack, so
 * `str` to sp+0/+4/+8/+12 are arguments five to eight, while the struct
 * the eighth argument POINTS at lives at sp+16 (`add r6, sp, #16`). Both
 * are on the stack and only the displacement separates them; reading
 * sp+16 as another argument, or the sp+0..12 stores as struct fields,
 * produces a plausible wrong call.
 *
 * TWO INDEPENDENT DRAWS FROM Func_080000f8, NOT ONE REUSED. It is called
 * at 0x020025ca and again at 0x020025e6, and the results are consumed
 * differently: the first as `(x * 7) >> 16` masked to 3 bits and tested
 * for zero, the second as `(x * 8) >> 16` then multiplied up. Sharing one
 * local between them would be a silent behaviour change.
 *
 * The multiply chain at 0x020025ee is a constant fold spelled in shifts:
 * `r4 = r0*3`, then `r4 += r4<<4` makes r4*17, then `r4 += r4<<8` makes
 * r4*257 -- overall r0 * 3 * 17 * 257 = r0 * 13107, which is 0x3333, the
 * same constant 0x020026d8 passes literally from its own pool. Written as
 * the product below, with the shift spelling recorded here.
 *
 * The byte-count field is derived from a live global: `*(s32 *)0x03001e40
 * & 15` is subtracted from 8 and shifted left 16 before being added to
 * record+8. That global is read TWICE, at 0x020025f8 and again at
 * 0x0200262c for the low-bit test that picks the final Func_0808a158
 * argument, so it is re-read and not cached.
 *
 * Uncertainty: the four distinct callees are not identified beyond call
 * shape; the stack struct's field at +0 is never written here and is left
 * as it was. Every constant is recorded as a passed value.
 */

extern u8 *Func_020069c4(s32 arg0);
extern void Func_02006ac8(s32 id, s32 arg1);
extern void Func_02006ad2(s32 id, s32 arg1);
extern s32 Func_02006928(void);
extern s32 Func_02006944(void);
extern void Func_02002766(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

extern s32 Data_03001e40;

void Func_020025b8(void)
{
    u8 params[40];
    u8 *record;
    s32 draw;
    s32 offset;

    record = Func_020069c4(0);

    *(s32 *)(params + 4) = 7;
    draw = (Func_02006928() * 7) >> 16;
    if ((draw & 7) == 0)
        *(s32 *)(params + 4) = 5;

    *(s32 *)(params + 8) = 0xb333;
    *(s32 *)(params + 12) = 0xcccc;

    offset = ((Func_02006944() * 8) >> 16) * 13107;

    Func_02002766(*(s32 *)(record + 8) + ((8 - (Data_03001e40 & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  params);

    if ((Data_03001e40 & 1) != 0)
        Func_02006ac8(0, 15);
    else
        Func_02006ad2(0, 1);
}
