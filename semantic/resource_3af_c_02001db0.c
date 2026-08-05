#include "types.h"

/*
 * resource_3af owner at 0x02001db0, 480 bytes: a scripted scene whose
 * middle stages three actors as an ensemble and then WAITS ON THEM --
 * the one real control-flow feature in the body.
 *
 * Opening: Func_0808a018, then TWO data blocks are registered, each
 * followed by its own frame advance -- Func_0808a030(0x0200d160) /
 * Func_080000c0(1), then Func_0808a030(0x0200d208) / Func_080000c0(1).
 * Every other scene entry in this overlay registers 0x0200d160 alone,
 * so the second block is this owner's own.
 *
 * The transition-mode write is the additive arm: displacement 448 from
 * `movs r2,#224 / lsls r2,#1`, value 514 from `adds r2,#66`. It is
 * followed by Func_0808a360 ALONE -- no Func_0808a370. The a360/a370
 * pairing that holds in 0x02001c14, 0x02001d0c and 0x020029d4 is not a
 * rule; the close here does use the full Func_0808a368/Func_0808a370
 * pair.
 *
 * Actor 21 is dressed first (Func_0808a090 with 0x19999/0xcccc, three
 * Func_0808a0d0 triples, Func_0808a138(21, 2)), a dialogue line 0x1e44
 * plays, and then Func_020039ec is called with 0xa015 -- a PACKED
 * CONSTANT where every other site in this overlay passes a small actor
 * id. That helper (tracked byte-exact at 0x020039ec) forwards its
 * argument straight to Func_0808a180, so the packing is the callee's
 * business; it is transcribed, not decomposed.
 *
 * Then the ensemble: sound 146, the u16 at record +100 cleared on
 * actors 24, 25 and 26, each placed, each given the same 0x26666/
 * 0x13333 pair and the same data pointer 0x0200c4ec, and each set
 * running with Func_0808a158(id, 3).
 *
 * The wait is a do-while poll, the body's only branch (`beq` at
 * 0x02001f08 back to 0x02001ef4):
 *
 *     do { Func_080000c0(1); }
 *     while (*(s16 *)(Func_0808a080(24) + 100) == 0);
 *
 * It advances a frame and re-reads actor 24's signed halfword at +100 --
 * the same field the scene zeroed on all three actors before starting
 * them -- until it goes nonzero. Actor 24 alone is polled; 25 and 26 are
 * started and not waited on.
 *
 * The poll clears straight into Func_02000bb8, this overlay's once-only
 * flag-0x271 reward grant. That is the THIRD caller found for 0xbb8
 * (after 0x02001c14 and 0x020029d4), and it is the most explicit: the
 * grant fires the instant the ensemble animation reports done.
 *
 * The close: Func_0808a0c8(21, 196, 612), Func_0808a0a0(24), a 10-frame
 * wait, the Func_0808a368/Func_0808a370 pair, another 10-frame wait,
 * then the window-engine bracket first named for resource_3c9's 0x4bec
 * -- Func_08015210(0x1e45, 1, 0) sitting between Func_08009208 and
 * Func_08009210 -- and Func_0808a248(12).
 *
 * Complete owner: `push {r5, lr}` at 0x02001db0 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02001f52-0x02001f56; fourteen trailing pool
 * words (0x0200d160, 0x0200d208, 0x03001ebc, 0x00019999, 0x0000cccc,
 * 0x000002a6, 0x0000028e, 0x00001e44, 0x0000a015, 0x00026666,
 * 0x00013333, 0x00000261, 0x0200c4ec, 0x00001e45) end at 0x02001f8f,
 * and the next owner (0x02001f90, already tracked byte-exact) begins
 * there.
 *
 * Second read done (over 400 bytes): the loop bound was the correction.
 * A first pass read the `beq` as an `if` skipping the reward call --
 * wrong; the target 0x02001ef4 is BEHIND the compare, so it is a
 * backward branch and the construct is a poll, not a guard. The
 * `ldrsh` also matters: the field is signed, so the loop exits on any
 * nonzero value including a negative one.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 1db0 1f90, the +2
 * rule against the raw image).
 *
 * Uncertainty: actor ids and argument roles by shape; coordinates,
 * fixed-point pairs, data pointers and the two dialogue ids are
 * transcribed, meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern void Func_0808a018(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a0(s32 id);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a1d8(s32 id);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_08009208(void);
extern void Func_08009210(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 sound_id);
extern void Func_02000bb8(void);
extern void Func_020039ec(s32 id);

void Func_02001db0(void)
{
    u8 *workspace;

    Func_0808a018();
    Func_0808a030(0x0200d160);
    Func_080000c0(1);
    Func_0808a030(0x0200d208);
    Func_080000c0(1);

    Func_0808a0f0(21, 248 << 16, 182 << 18);
    Func_0808a158(0, 15);
    Func_080091e0(Func_0808a080(0), 0);

    *(s32 *)(workspace + 448) = 448 + 66;
    workspace = Data_03001ebc;
    Func_0808a360();

    Func_0808a090(21, 0x19999, 0xcccc);
    Func_0808a0d0(21, 242, 173 << 2);
    Func_0808a0d0(21, 196, 0x2a6);
    Func_0808a0d0(21, 182, 0x28e);
    Func_0808a138(21, 2);
    Func_0808a170(0x1e44);
    Func_020039ec(0xa015);

    Func_0808a090(0, 0x26666, 0x13333);
    Func_0808a0b8(0, 154, 0x261);
    Func_080f9010(146);

    *(u16 *)(Func_0808a080(24) + 100) = 0;
    *(u16 *)(Func_0808a080(25) + 100) = 0;
    *(u16 *)(Func_0808a080(26) + 100) = 0;

    Func_0808a0f0(24, 128 << 14, 242 << 17);
    Func_0808a0f0(25, 168 << 15, 248 << 17);
    Func_0808a0f0(26, 128 << 13, 149 << 18);
    Func_0808a090(24, 0x26666, 0x13333);
    Func_0808a090(25, 0x26666, 0x13333);
    Func_0808a090(26, 0x26666, 0x13333);
    Func_0808a098(24, 0x0200c4ec);
    Func_0808a098(25, 0x0200c4ec);
    Func_0808a098(26, 0x0200c4ec);
    Func_0808a158(24, 3);
    Func_0808a158(25, 3);
    Func_0808a158(26, 3);

    do {
        Func_080000c0(1);
    } while (*(s16 *)(Func_0808a080(24) + 100) == 0);

    Func_02000bb8();

    Func_0808a0c8(21, 196, 153 << 2);
    Func_0808a0a0(24);
    Func_0808a010(10);
    Func_0808a368();
    Func_0808a370();
    Func_0808a010(10);

    Func_08009208();
    Func_0808a1d8(21);
    Func_08015210(0x1e45, 1, 0);
    Func_08009210();
    Func_0808a248(12);
}
