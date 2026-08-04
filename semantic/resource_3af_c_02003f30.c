#include "types.h"

/*
 * resource_3af owner at 0x02003f30, 744 bytes: a four-actor dialogue
 * scene fronted by the skip-beat gate. It is one of the two rows the
 * 0x02001444 driver does NOT dispatch directly, so its caller is
 * elsewhere in the overlay.
 *
 * The gate is the idiom named in resource_3c9's 0x3e9c/0x4bec, but in
 * its EARLY-EXIT form rather than the converging form:
 *
 *     if (Func_0808a070(0, 0) == 1) {
 *         Func_020039ec(0x2016);
 *         Func_0808a020();
 *         return;
 *     }
 *     workspace = Data_03001ebc;
 *     *(u16 *)(workspace + 472) += 1;
 *
 * In the 3c9 copies both arms do work and converge, and the counter
 * bump is duplicated so it happens exactly once either way. Here the
 * ==1 arm closes the scripted-scene bracket and leaves through the
 * epilogue (`b.n 0x020041d4`, a 526-byte forward jump straight to the
 * unwind), so the counter advances ONLY on the not-taken path. Same
 * gate, different control shape -- worth having both spellings on
 * record before anyone assumes the converging form is the rule.
 *
 * MORE EVIDENCE FOR THE PACKED ACTOR-ID FORM (first argued in
 * resource_3af_c_02003a0c.c). Four pool constants are passed as ids
 * here: 0x2016, 0x2014, 0x6014 and 0x8014. Low byte 0x16 = 22 and
 * 0x14 = 20 -- both actors this scene drives -- with the high nibble
 * varying, so `(mode << 12) | actorId` holds again, now across three
 * different callees (Func_020039ec, Func_0808a178, Func_0808a190).
 * Note 0x2016 goes to Func_0808a178 and Func_0808a190 as well as
 * Func_020039ec, which is what rules out the packing being one
 * helper's private convention. Still transcribed as constants.
 *
 * The workspace pointer's ADDRESS is cached in r8 across the whole body
 * (that is what the `mov r6,r8 / push {r6}` spill in the prologue is
 * for), and it is dereferenced twice: once for the counter bump above
 * and once for the closing mode write. That closing write is
 * `movs r2,#224 / lsls r2,#1` for the displacement 448 and
 * `adds r2,#65` for the value -- 513, the CANONICAL value from
 * resource_377_c_02000a0c.c, and the first time it appears anywhere in
 * resource_3af. Every other owner here uses 256, 514 or 521. There is
 * no opening mode write at all: Func_0808a360 is called bare at
 * 0x02003f46 before any workspace access.
 *
 * Func_02000bb8, the once-only flag-0x271 reward grant, is called at
 * 0x02003fd0 -- the SIXTH caller, and the only one behind a skip-beat
 * gate, so on a repeat visit the grant is skipped along with the rest
 * of the scene.
 *
 * The body between the gate and the close is an ordinary run of scene
 * calls on actors 0, 20, 21, 22 and 26, with one dialogue line (0x1f69)
 * and one shared data pointer (0x0200c918) handed to actors 22, 21 and
 * 26 in turn.
 *
 * Complete owner: `push {r5, r6, lr}` plus the r8 spill (`mov r6,r8 /
 * push {r6}`) at 0x02003f30 through the matching unwind `pop {r3} /
 * mov r8,r3 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x020041d4-0x020041dc; an alignment halfword and fourteen pool words
 * (0x0200d1d8, 0x00001f69, 0x00002016, 0x03001ebc, 0x00013333,
 * 0x00009999, 0x03090000, 0x00002014, 0x00000101, 0x00006014,
 * 0x00008014, 0x0000cccc, 0x00019999, 0x0200c918) end at 0x02004217,
 * and the next owner (0x02004218, already tracked byte-exact) begins
 * there.
 *
 * Second read done (over 400 bytes): the correction was the gate's
 * shape. A first pass read the `bne` at 0x02003fa8 as skipping only
 * the two calls that follow and rejoining at 0x02003fb6 -- but the
 * taken side's own `b.n` at 0x02003fb4 goes to 0x020041d4, the
 * epilogue, so it is an early return over the whole rest of the
 * function, not a two-call skip. Also note the pool word 0x0200d1d8 at
 * 0x020041e0 disassembles as `bne.n 0x02004194`, a plausible-looking
 * branch into the body; it is data.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 3f30 4218, the +2
 * rule against the raw image).
 *
 * Uncertainty: actor ids and argument roles by shape; coordinates,
 * fixed-point pairs, the data pointer and the packed constants are
 * transcribed, meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a190(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_02000bb8(void);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 packedId);

void Func_02003f30(void)
{
    u8 *workspace;

    Func_0808a018();
    Func_0808a030(0x0200d1d8);
    Func_080000c0(1);
    Func_0808a360();

    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 148, 164 << 2);
    Func_0808a1e8(22, 128 << 1, 0);
    Func_0808a138(22, 1);
    Func_02003a00(22, 160 << 7);
    Func_0808a170(0x1f69);
    Func_0808a178(0x2016, 0);
    Func_0808a1b8(0, 224 << 8, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_020039ec(0x2016);
        Func_0808a020();
        return;
    }

    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;
    Func_0808a190(0x2016, 0);

    Func_02000bb8();

    Func_0808a0f0(26, 216 << 16, 147 << 18);
    Func_0808a090(26, 0x13333, 0x9999);
    Func_0808a0d0(26, 216, 149 << 2);
    Func_0808a0d0(26, 188, 154 << 2);
    Func_0808a1b8(0, 224 << 8, 0);
    Func_0808a1b8(21, 208 << 8, 0);
    Func_0808a1b8(22, 208 << 8, 0);
    Func_02003a00(26, 160 << 7);
    Func_0808a128(26, 2, 0);
    Func_0808a100(26, 4);
    Func_0808a180(26, 0);

    Func_0808a0f0(20, 180 << 16, 0x03090000);
    Func_0808a090(20, 128 << 9, 128 << 8);
    Func_0808a0d0(20, 180, 166 << 2);
    Func_0808a1b8(20, 208 << 8, 0);
    Func_020039ec(0x2014);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a1b8(22, 192 << 6, 0);
    Func_0808a1e8(26, 0x101, 60);
    Func_0808a138(20, 1);
    Func_020039ec(0x2014);
    Func_0808a130(21, 2);
    Func_020039ec(21);
    Func_0808a1b8(20, 160 << 7, 20);
    Func_0808a100(20, 3);
    Func_020039ec(0x6014);
    Func_0808a128(26, 2, 20);
    Func_0808a100(26, 4);
    Func_020039ec(26);
    Func_0808a0d0(20, 182, 160 << 2);
    Func_0808a1b8(20, 208 << 8, 0);
    Func_020039ec(0x8014);
    Func_0808a1e8(26, 128 << 1, 20);
    Func_0808a130(26, 2);
    Func_020039ec(26);
    Func_0808a110(20, 3);
    Func_02003a00(22, 0);
    Func_0808a138(22, 1);
    Func_020039ec(22);

    Func_0808a090(22, 0x19999, 0xcccc);
    Func_0808a098(22, 0x0200c918);
    Func_0808a090(21, 0x19999, 0xcccc);
    Func_0808a0d0(21, 168, 158 << 2);
    Func_0808a098(21, 0x0200c918);
    Func_0808a010(80);
    Func_0808a098(26, 0x0200c918);
    Func_0808a010(40);

    Func_02003a00(20, 128 << 8);
    Func_020039ec(0x2014);
    Func_02003a00(0, 224 << 8);
    Func_0808a110(0, 3);
    Func_0808a110(20, 3);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 + 65;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(17);
}
