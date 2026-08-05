#include "types.h"

/*
 * Resource 3ae, owner at 0x02000ba0 (672 bytes including its six-word pool at
 * 0x02000e28..0x02000e3e).
 *
 * Role known in advance from the call graph: `Func_020008cc`, the room-0x6b
 * setup hook, calls this owner with no arguments exactly once, guarded by the
 * read-then-set of event flag 0x8ac — the "play this scene once" idiom.  So it
 * is a one-shot cutscene script and takes no arguments.
 *
 * Prologue `push {lr}` at 0x02000ba0; epilogue `pop {r0} / bx r0` at
 * 0x02000e24.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.  There is no frame: every call's arguments fit in
 * r0-r3, which is what makes the body a flat instruction-per-beat script.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  Every
 * site is an import veneer except none — all 68 `bl`s in the span resolve to
 * the Func_0808axxx scene-director bank, which is more than the inventory's
 * calls=60; the inventory count is documented as systematically low and every
 * `bl` in the 672-byte span is transcribed below in order, so the set is
 * complete.
 *
 * The bracket Func_0808a018 ... Func_0808a020 is the same scene open/close pair
 * the siblings at 0x020002dc and 0x020004b8 use.  Func_0808a010(n) is the
 * beat delay (its argument is always a small frame count and its result is
 * never read).  Func_0808a1b8(slot, angle, 0) sets a facing, as recorded on
 * 0x020002dc.  Func_0808a0f0(slot, x, y) places, with 16.16 fixed-point
 * coordinates: `movs rN,#k / lsls rN,rN,#17` is 2k.0, `#19` is 8k.0, `#16` is
 * k.0.
 *
 * Repeated beats are NOT merged: the two Func_0808a128 calls, the four
 * Func_0808a180(x, 0) calls and the several identical Func_0808a010(20) waits
 * are distinct script sites with distinct timing, and collapsing them would
 * change the scene.
 *
 * The tail (0x02000df8..0x02000e1e) converts two 16.16 fields of slot 8's
 * object record into whole units and stores them as halfwords at +100 and
 * +102.  `cmp r2,#0 / bge / adds r2,r2,#0xffff / asrs r2,r2,#16` is the
 * standard truncate-toward-zero division by 65536; it is written out rather
 * than as `/ 65536` so the rounding is visible.
 *
 * Uncertainties: 0x1f89 is read as a cue id from its use as the sole argument
 * of Func_0808a170 (the same position 0x1d23 occupies on 0x020002dc).  The
 * mixed units in the Func_0808a0d0 calls (a small raw r1 alongside an r2 that
 * has been shifted by 3) are preserved as computed; no witness in this overlay
 * settles what either argument scales.  Field +0x5b of the Func_0808a080
 * record is set to 1 at the top of the scene and back to 0 near the end, so it
 * reads as a "scene owns this slot" latch.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();
void Func_0808a460();
void Func_0808a570();

void Func_02000ba0(void)
{
    u8 *object;
    s32 value;

    Func_0808a018();
    Func_0808a460();

    Func_0808a0f0(8, 328 << 16, 1424 << 16);
    object = Func_0808a080(8);
    object[0x5b] = 1;

    Func_0808a360();
    Func_0808a370();

    Func_0808a010(20);
    Func_0808a570(1, -16, 0, 0x8000);
    Func_0808a0e8(1);
    Func_0808a1b8(0, 0xa000, 0);

    Func_0808a010(20);
    Func_0808a170(0x1f89);
    Func_0808a1b8(0, 0xa000, 0);

    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0d0(1, 232, 1424);
    Func_0808a1b8(1, 0x8000, 0);

    Func_0808a210(184 << 16, -1, 1440 << 16, 1);
    Func_0808a218();

    Func_0808a010(10);
    Func_0808a128(1, 6, 15);
    Func_0808a128(1, 6, 40);
    Func_0808a180(1, 0);

    Func_0808a010(20);
    Func_0808a210(264 << 16, -1, 1448 << 16, 1);
    Func_0808a218();

    Func_0808a010(20);
    Func_0808a1e8(8, 256, 50);
    Func_0808a090(8, 0x13333, 0x9999);
    Func_0808a0d0(8, 264, 1424);
    Func_0808a1b8(8, 0x8000, 0);

    Func_0808a010(10);
    Func_0808a1b8(1, 0, 0);

    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a110(8, 4);
    Func_0808a010(10);
    Func_0808a180(8, 0);

    Func_0808a010(20);
    Func_0808a1e8(1, 258, 40);

    Func_0808a010(30);
    Func_0808a1b8(1, 0x8000, 0);

    Func_0808a010(50);
    Func_0808a180(1, 0);

    Func_0808a010(20);
    Func_0808a1b8(1, 0x2000, 0);

    Func_0808a010(30);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a0d0(1, 264, 1464);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(8, 0x4000, 0);

    Func_0808a010(30);
    Func_0808a180(1, 0);

    Func_0808a010(10);
    Func_0808a110(0, 3);

    Func_0808a010(30);
    Func_0808a110(1, 3);

    Func_0808a010(30);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a100(1, 2);

    object = Func_0808a080(0);
    if (object != 0) {
        Func_0808a0b8(1, *(s16 *)(object + 10), *(s16 *)(object + 18));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a010(20);
    object = Func_0808a080(8);
    object[0x5b] = 0;
    Func_0808a098(8, 2);

    object = Func_0808a080(8);

    value = *(s32 *)(object + 8);
    if (value < 0) {
        value += 0xffff;
    }
    *(s16 *)(object + 100) = (s16)(value >> 16);

    *(s16 *)(object + 102) = (s16)(value >> 16);
    value = *(s32 *)(object + 16);
    if (value < 0) {
        value += 0xffff;
    }

    Func_0808a020();
}
