#include "types.h"

/*
 * resource_399 scene-script selector at 0x0200016c, 116 bytes.
 *
 * Complete owner: `push {r5, lr}` at 0x0200016c and the interworking return
 * `pop {r5} / pop {r1} / bx r1` at 0x020001c0.  The popped register is r1, so
 * r0 survives and IS the result — the owner returns the selected in-image data
 * pointer.  Bytes 0x020001c6-0x020001df are alignment plus the six-word
 * literal pool, reached only by `ldr rN, [pc, #imm]`.
 *
 * Same family as the byte-exact-backed `semantic/overlays/
 * resource_37f_c_0200007c.c`: read the scenario id from the shared
 * `Data_02000240` array (element 224, built as base + 448 with an `ldrsh` at
 * index 0), branch on it, and return one of several in-image script blocks,
 * announcing the chosen one with Func_0808a038 where it applies.
 *
 * LINK BASE.  All three returned pool words (0x0200aad0, 0x0200aa58,
 * 0x0200a9e0) are EVEN and lie in the 0x0200axxx band, so under the project's
 * proven 0x02008000 overlay link base they are in-image data at file offsets
 * 0x2ad0, 0x2a58 and 0x29e0 — data blocks, not callbacks.  (Odd would mean a
 * Thumb entry point.)  This owner is itself a witness for the base: the same
 * band holds this overlay's other script blocks.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 016c` (an overlay `bl` stores `target - 2`, so the disassembler's own
 * annotations are wrong).  Three sites, two distinct imports:
 *   0x02000184 -> Func_0808a038(script)     announce the chosen script
 *   0x0200018a -> Func_080770c0(0x881)      story-flag query, result tested
 *   0x020001b2 -> Func_080770c0(0x881)      the same query on the other arm
 * The two Func_080770c0 sites are on mutually exclusive arms of the scenario
 * test and rejoin only at the shared `pop`, so they are two real call sites,
 * not one shared site written twice.
 *
 * The overlay image is writable EWRAM, so patching the descriptor in place is
 * ordinary: when the flag is set, the block at 0x2ad0 has its byte at +262
 * cleared and its 16.16 coordinate pair replaced with (182.0, 564.0) at +0x50
 * and +0x58, plus the word 2 at +0x4c.  Both coordinates are built by shifts
 * (`movs r3,#182 ; lsls #16` and `movs r3,#141 ; lsls #18`), which is the usual
 * way this compiler spells a 16.16 whole number.
 *
 * Uncertainty: the meaning of +0x4c is unverified — the value 2 is set only on
 * this path, and nothing in this overlay reads the field back.
 */

extern s16 Data_02000240[];
extern u8 Data_0200a9e0[];      /* in-image script, file offset 0x29e0 */
extern u8 Data_0200aa58[];      /* in-image script, file offset 0x2a58 */
extern u8 Data_0200aad0[];      /* in-image script, file offset 0x2ad0 */

void Func_0808a038();
s32 Func_080770c0();

u8 *Func_0200016c(void)
{
    u8 *script;

    if (Data_02000240[224] == 0x33) {
        script = Data_0200aad0;
        Func_0808a038(script);
        if (Func_080770c0(0x881) != 0) {
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            script[262] = 0;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (Func_080770c0(0x881) != 0) {
        return Data_0200aa58;
    }
    return Data_0200a9e0;
}
