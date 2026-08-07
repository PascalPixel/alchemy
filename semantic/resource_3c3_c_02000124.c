#include "types.h"

/*
 * Resource 3c3, overlay entry / scene installer at 0x02000124
 * (356 bytes, 17 call sites).
 *
 * This is a ROOT.  The overlay's image offset 0 is an exported-entry veneer
 * table of `ldr r4,[pc,#0] / bx r4 / .word` triples; its six words are
 * 0x02008125, 0x02008041, 0x02008085, 0x0200808d, 0x020080e5 and 0x02008081.
 * Under the proven 0x02008000 link base those are offsets 0x125, 0x41, 0x85,
 * 0x8d, 0xe5 and 0x81 — all odd, i.e. Thumb entry points at 0x124, 0x40, 0x84,
 * 0x8c, 0xe4 and 0x80.  Entry 0 is this owner, entries 1/3/4 are the three
 * table selectors, entry 2 is the two-instruction `ldr r0,[pc,#0] / bx lr`
 * constant at 0x84 and entry 5 the `movs r0,#0 / bx lr` leaf at 0x80.
 *
 * Complete owner: `push {r5, r6, lr} / sub sp, #8` at 0x02000124 and
 * `add sp, #8 / pop {r5, r6} / pop {r1} / bx r1` at 0x0200024e.  The popped
 * branch register is r1, so r0 survives and is the result; the only value that
 * reaches it is the `movs r0, #0` at 0x0200024c, so this returns 0 on every
 * path.  Bytes 0x02000258-0x02000287 are the literal pool.
 *
 * `Data_02000240` is the cross-overlay RAM global block, indexed here as
 * signed halfwords: element 224 is the scene id, 225 the scene phase, 226/227
 * a pair the 0x89f arm seeds, and 288/289 a (scene, variant) pair written when
 * either of the two 0x8fb/0x8fc flags is up.  Element 289 is reached through
 * the pooled byte offset 0x242, which is 289*2 — the displacement/value trap
 * documented in HANDOVER: 0x242 is a byte displacement, not a value.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; all 17 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080770c0 x5, Func_0808a100 x5, Func_080091c0 x2, Func_080770c8 x2,
 *   Func_080770d0 x1, Func_0808a080 x1, Func_0808a0f0 x1.
 * (The inventory row says calls=17, which agrees here; it is a floor in
 * general and not used as an equality test.)
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_020009a8();
s32 Func_020009da();
void Func_02000a58();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
void Func_02000a4e();
s32 Func_02000a4c();
void Func_02000a54();
void Func_02000a80();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b20();
u8 *Func_02000ae6();
void Func_02000ab6();
                        /* test a story flag (used in a condition) */
                        /* set a story flag */
                        /* clear a story flag */
                        /* six-argument presentation request, last two on the stack */
                        /* scene entity record by selector */
                        /* place a slot at a 16.16 position */
                        /* set a slot's presentation mode */

extern s16 Data_02000240[];

s32 Func_02000124(void)
{
    s16 scene;
    u8 *record;

    if (Func_020009a8(0x89f) != 0) {
        Data_02000240[226] = 0x69;
        Data_02000240[227] = 10;
    }

    if (scene == 0xa9) {
        if (Func_020009da(0x897) != 0) {
            Func_02000a58(10, 0, 0);
        }

        if (Data_02000240[225] == 3) {
            if (Func_020009fc(0x8fb) != 0) {
                Data_02000240[288] = scene;
                Data_02000240[289] = 1;
            }
            if (Func_02000a16(0x8fc) != 0) {
                Data_02000240[288] = scene;
                Data_02000240[289] = 5;
            }
            Func_02000a40(0x12f);
        }

        if (Data_02000240[225] == 1) {
            Func_02000a4e(0x8fb);
            if (Func_02000a4c(0x96f) == 0) {
                Func_02000a54(6, 0, 2, 1, 8, 27);
            }
        }

        if (Data_02000240[225] == 5) {
            Func_02000a80(0x8fc);
        }
    } else if (scene == 0xaa) {
        Func_02000b00(8, 4);
        Func_02000b08(9, 4);
        Func_02000b10(10, 3);
        Func_02000b18(11, 4);
        Func_02000b20(12, 3);

        record = Func_02000ae6(15);
        *(s32 *)(record + 28) = 0x19999;
        Func_02000ab6(108, 38, 1, 1, 102, 56);
    }
    scene = Data_02000240[224];

    return 0;
}
