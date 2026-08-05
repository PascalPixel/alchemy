#include "types.h"

/*
 * resource_3b2 owner at 0x02000f70, 676 bytes.  Executable range
 * 0x02000f70-0x020011d9, a two-byte alignment `movs r0, r0` at 0x020011da, and
 * the 14-word literal pool 0x020011dc-0x02001213.  The pool map was derived by
 * walking the owner's control flow from its prologue; nothing between
 * 0x02000f70 and 0x020011d9 is unreached, and nothing at or after 0x020011dc
 * is reached as an instruction.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r7}` (saving r8), an 8-byte
 * outgoing-argument frame, and the single epilogue at 0x020011cc.  That
 * epilogue ends `pop {r1} / bx r1` — N != 0, so r0 SURVIVES and is the result.
 * `movs r0, #0` immediately precedes it, so the owner returns the constant 0
 * on every path.  Signature: `s32 Func_02000f70(void)`.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 0f70 --json` (41 sites, 14 distinct targets: 7 prologue, 34 veneer).  The
 * seven in-overlay callees are Func_0200191c (0x02000f96), Func_0200167c
 * (0x02001040), Func_020008c0 (0x02001052 and 0x02001058), Func_02002ed8
 * (0x0200116a), Func_02001214 (0x020011b6) and Func_02001494 (0x020011c8).
 * The imports are Func_080770c0 x8, Func_0808a080 x5, Func_08009278 x5,
 * Func_08009080 x4, Func_080091c0 x4, Func_080091e0 x4, Func_0808a3c0 x3 and
 * Func_080770c8 x1 — 34, matching the tool's veneer count exactly.
 *
 * Behaviour: the overlay's per-scene entry hook.  It first stamps the shared
 * workspace word at +448 with 516, then branches on the scene id in
 * `Data_02000240[224]`:
 *
 *   0x7b            - hand off to the slot-placement table at 0x0200191c.
 *   0x7d            - two collision strips and a camera move if the 0x0ef7
 *                     companion is absent; then, if the scene id has not moved
 *                     underneath, either the 0x08d1 companion must be present
 *                     (or the sub-state at [225] must be 5) or the hook stops.
 *                     Otherwise it notifies 0x08d1, repaints and moves the
 *                     camera again.  If the id DID move, control falls into the
 *                     general block below, exactly as the assembly branches.
 *   0x71            - the party set-up scene: place the slots, lift slot 8 to
 *                     y = 129.0, run the two 0x020008c0 beats for slots 9 and
 *                     10, then for each of the four optional companions
 *                     (0x0240, 0x0241, 0x0242, 0x0243 -> slots 11..14) reset
 *                     the slot's byte at +89, put it in presentation mode 0 and
 *                     drop a marker through Func_08009278.  Slot 14 gets two
 *                     markers.  Finally, if the 0x0fd7 companion is absent, run
 *                     the 0x02002ed8 idle beat for slot 8.
 *   0x7e            - one collision strip and a camera move if 0x0ef4 is
 *                     absent, then fall into the range check below.
 *   0x7e .. 0x86    - run the slot-8 beat at 0x02001214, and when the sub-state
 *                     at [225] is 5 also the cutscene at 0x02001494.
 *
 * `Data_03001ebc` is the shared workspace POINTER (a `u8 *` variable at IWRAM
 * 0x03001ebc), the same one the byte-exact sibling
 * assets/code/resource_3b2_c_0200006c.c dereferences; the store here is
 * `*(s32 *)(Data_03001ebc + 448) = 516`.
 *
 * UNCERTAINTY: Func_0808a3c0's two trailing arguments are 16.16 coordinates
 * (this is consistent with resource_3a2's converted sites, which pass 0 and
 * -1), but the meaning of its first argument (100/101 here) is not
 * established.  Func_08009278's fourth argument is 253 at every site in this
 * owner; other overlays pass 0 and 255, so it reads as an intensity or alpha.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */
extern u8 *Data_03001ebc;               /* shared workspace pointer */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_080770c0();                    /* companion entity by selector, or 0 */
void Func_080770c8();                   /* companion notification by selector */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080091c0();                   /* collision repaint, six arguments */
void Func_080091e0();                   /* set presentation mode */
void Func_08009080();                   /* select presentation mode */
void Func_08009278();                   /* place a marker (kind, x, z, level) */
void Func_0808a3c0();                   /* move the camera to (x, z) */

/* This overlay's own routines. */
void Func_0200191c();                   /* slot placement table */
void Func_0200167c();                   /* party placement pass */
void Func_020008c0();                   /* per-slot scene beat */
void Func_02002ed8();                   /* idle presentation beat */
void Func_02001214();                   /* slot-8 beat */
void Func_02001494();                   /* the 0x7e..0x86 cutscene */

s32 Func_02000f70(void)
{
    s32 scene;
    s32 current;
    u8 *slot;

    *(s32 *)(Data_03001ebc + 448) = 516;
    scene = Data_02000240[224];

    if (scene == 0x7b) {
        Func_0200191c();
        return 0;
    }

    if (scene == 0x7d) {
        if (Func_080770c0(0xef7) == 0) {
            Func_080091c0(0, 3, 1, 1, 13, 40);
            Func_080091c0(0, 2, 1, 1, 15, 40);
            Func_0808a3c0(101, 0x00d80000, 0x02880000);
        }

        /* Re-read: the calls above can advance the scene. */
        if (Data_02000240[224] == scene) {
            if (Data_02000240[225] != 5 && Func_080770c0(0x8d1) == 0) {
                return 0;
            }
            Func_080770c8(0x8d1);
            Func_080091c0(0, 1, 1, 1, 13, 30);
            Func_0808a3c0(100, 0x00d80000, 0x01e80000);
            return 0;
        }
        /* Otherwise fall into the general block, as the assembly branches. */
    }

    current = Data_02000240[224];

    if (current == 0x71) {
        Func_0200167c();
        *(s32 *)(Func_0808a080(8) + 56) = 0x00810000;   /* y = 129.0 */

        Func_020008c0(9);
        Func_020008c0(10);

        if (Func_080770c0(576) != 0) {              /* 144 << 2 == 0x240 */
            slot = Func_0808a080(11);
        }
            if (slot != 0) {
                slot[89] = 0;
                Func_08009080(slot, 4);
                Func_080091e0(slot, 0);
            }
            Func_08009278(0, 0x01300000, 0x01700000, 253);

        if (Func_080770c0(0x241) != 0) {
            slot = Func_0808a080(12);
            if (slot != 0) {
                slot[89] = 0;
                Func_08009080(slot, 4);
                Func_080091e0(slot, 0);
            }
            Func_08009278(0, 0x00500000, 0x01700000, 253);
        }

        if (Func_080770c0(0x242) != 0) {
            slot = Func_0808a080(13);
            if (slot != 0) {
                Func_08009080(slot, 4);
                slot[89] = 0;
                Func_080091e0(slot, 0);
            }
            Func_08009278(0, 0x00600000, 0x01500000, 253);
        }

        if (Func_080770c0(0x243) != 0) {
            slot = Func_0808a080(14);
            if (slot != 0) {
                slot[89] = 0;
                Func_08009080(slot, 4);
                Func_080091e0(slot, 0);
            }
            Func_08009278(0, 0x00900000, 0x01400000, 253);
            Func_08009278(0, 0x02f00000, 0x01400000, 253);
        }

        if (Func_080770c0(0xfd7) == 0) {
            Func_02002ed8(8);
        }
        return 0;
    }

    if (current == 0x7e && Func_080770c0(0xef4) == 0) {
        Func_080091c0(0, 0, 1, 1, 37, 10);
        Func_0808a3c0(100, 0x02580000, 0x00a80000);
    }

    /* Re-read again; the block above can advance the scene. */
    current = Data_02000240[224];
    if (current < 0x7e || current > 0x86) {
        return 0;
    }

    Func_02001214();
    if (Data_02000240[225] == 5) {
        Func_02001494();
    }
    return 0;
}
