#include "types.h"

/*
 * Resource 3b5, owner at 0x0200007c (244 bytes advertised; 236 bytes of code
 * plus an 8-byte literal pool at 0x02000168-0x0200016f).
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the high-register save at
 * 0x0200007c-0x02000088, and the matching
 * `pop {r3, r5, r6, r7} / ... / pop {r1} / bx r1` interworking return at
 * 0x02000158-0x02000166.  The epilogue pops into r1, not r0, so r0 survives
 * and IS the result (HANDOVER 0, epilogue rule).
 *
 * Link base: this overlay is linked at 0x02008000.  Eleven odd pool words in
 * `assets/code/resource_3b5_overlay.s` are the Thumb entries of functions that
 * already have byte-exact sources here (0x02008031 = Func_02000030 + 1,
 * 0x02008261 = Func_02000260 + 1, 0x0200828d, 0x020082b9, 0x020082d5,
 * 0x020084e9, 0x02008509, 0x0200859d, 0x020085bd, 0x02008955, 0x02008965),
 * so the base is proven against tracked material.
 *
 * Role, from the call graph: the overlay initialiser Func_02000728 installs
 * 0x02008171 (= Func_02000170 + 1) as the per-frame callback of actors 16 and
 * 17, and Func_02000170 is this owner's only caller.  This is the "has the
 * follower noticed the leader" proximity/facing test.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay `bl`
 * stores target_offset - 2, so the disassembler's own annotations are wrong).
 * The three sites the byte-exact assembly spells `sub_02000e24`,
 * `sub_02000e4a` and `sub_02000e60` are all the SAME import veneer at file
 * offset 0xd0c, which resolves to Func_08009080.
 *
 * Uncertainty: r4 is written at 0x020000e2 as call-clobbered scratch although
 * the prologue never saves it.  Nothing observable depends on it and the bytes
 * are not in doubt; it is a recognised idiom in this codebase, not a decoding
 * error, so it is reproduced rather than "fixed".
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
s32 Func_020000e6();
s32 Func_02000dc8();
void Func_02000e24();
u8 *Func_02000e9e();
void Func_02000e4a();
void Func_02000e60();
                       /* overlay-local, 0x02000040: separation of two points */
                       /* BIOS-style arc tangent, returns a 16-bit angle */

s32 Func_0200007c(u8 *self, u8 *other, s32 range, s32 force)
{
    s32 *self_pos = (s32 *)(self + 8);
    s32 *other_pos = (s32 *)(other + 8);
    s32 noticed = 0;
    s32 angle;
    s32 heading;
    s32 behind, ahead, left, right, front;

    if (Func_020000e6(other_pos, self_pos, 0) >= range && force == 0) {
        goto missed;
    }

    /* arc tangent of (dz, dx) between the two actors, as a 16-bit angle. */
    angle = Func_02000dc8(*(s32 *)(other + 0x10) - *(s32 *)(self + 0x10),
                          other_pos[0] - self_pos[0]);
    angle = (u16)angle;

    /*
     * Five 16-direction buckets around that angle, and the actor's own facing
     * quantised the same way.  The pooled bias words are 0xffffe000 and
     * 0xf8b6f001; only their low halves can affect a mask of 0xf000, so they
     * act as -0x2000 and -0x0fff.  The -0x0fff is genuinely one less than the
     * -0x1000 the pattern would suggest — it is what the pool holds.
     */
    ahead  = (angle + 0x00002000) & 0xf000;
    right  = (angle + 0xf8b6f001) & 0xf000;
    behind = (angle + 0xffffe000) & 0xf000;
    front  = angle & 0xf000;
    left   = (angle + 0x00001000) & 0xf000;

    heading = *(u16 *)(self + 6) & 0xf000;

    if (front == heading || left == heading || right == heading || force != 0) {
        self[91] = 1;
        Func_02000e24(self, 1);
        noticed = 1;
    }

    /* The wide cone only applies when the other actor is the player party. */
    if (other == Func_02000e9e(0) && (ahead == heading || behind == heading)) {
        noticed = 1;
        self[91] = 1;
        Func_02000e4a(self, 1);
    }

    return noticed;

missed:
    /* Out of range and not forced: clear the flag and report the miss. */
    self[91] = (u8)noticed;
    Func_02000e60(self, 2);
    return noticed;
}
