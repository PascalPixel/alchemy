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
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
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
void Func_02002124();
void Func_02002230();
void Func_0200218e();
u8 *Func_0200215c();
void Func_0200223e();
void Func_0200224a();
void Func_02002148();
void Func_02002278();
void Func_020021b6();
void Func_02002232();
void Func_02002170();
void Func_02002216();
void Func_020021bc();
void Func_020021e8();
void Func_0200226c();
void Func_0200229e();
void Func_020022aa();
void Func_020021c0();
void Func_02002242();
void Func_0200224c();
void Func_0200228c();
void Func_020021e2();
void Func_020022dc();
void Func_020022e8();
void Func_020021fe();
void Func_020022e2();
void Func_02002244();
void Func_02002272();
void Func_020022f6();
void Func_02002234();
void Func_02002306();
void Func_020022c2();
void Func_02002258();
void Func_02002310();
void Func_02002266();
void Func_0200234a();
void Func_0200234c();
void Func_0200228a();
void Func_02002342();
void Func_02002298();
void Func_0200236c();
void Func_02002316();
void Func_02002398();
void Func_020023a4();
void Func_020023b0();
void Func_020022ee();
void Func_020023a6();
void Func_020022fc();
void Func_02002374();
void Func_0200230a();
void Func_02002382();
void Func_02002318();
void Func_02002352();
void Func_02002392();
u8 *Func_02002358();
void Func_020023a0();
void Func_020023b2();
void Func_02002358_b();
u8 *Func_02002386();
u8 *Func_0200239a();
void Func_020023aa();

                     

                     

                     

                     

                     

                     

                     

void Func_02000ba0(void)
{
    u8 *object;
    s32 value;

    Func_02002124();
    Func_02002230();

    Func_0200218e(8, 328 << 16, 1424 << 16);
    object = Func_0200215c(8);
    object[0x5b] = 1;

    Func_0200223e();
    Func_0200224a();

    Func_02002148(20);
    Func_02002278(1, -16, 0, 0x8000);
    Func_020021b6(1);
    Func_02002232(0, 0xa000, 0);

    Func_02002170(20);
    Func_02002216(0x1f89);
    Func_0200224a(0, 0xa000, 0);

    Func_020021bc(1, 0x19999, 0xcccc);
    Func_020021e8(1, 232, 1424);
    Func_0200226c(1, 0x8000, 0);

    Func_0200229e(184 << 16, -1, 1440 << 16, 1);
    Func_020022aa();

    Func_020021c0(10);
    Func_02002242(1, 6, 15);
    Func_0200224c(1, 6, 40);
    Func_0200228c(1, 0);

    Func_020021e2(20);
    Func_020022dc(264 << 16, -1, 1448 << 16, 1);
    Func_020022e8();

    Func_020021fe(20);
    Func_020022e2(8, 256, 50);
    Func_02002244(8, 0x13333, 0x9999);
    Func_02002272(8, 264, 1424);
    Func_020022f6(8, 0x8000, 0);

    Func_02002234(10);
    Func_02002306(1, 0, 0);

    Func_02002244(20);
    Func_0200224a(10);
    Func_020022c2(8, 4);
    Func_02002258(10);
    Func_02002310(8, 0);

    Func_02002266(20);
    Func_0200234a(1, 258, 40);

    Func_02002278(30);
    Func_0200234c(1, 0x8000, 0);

    Func_0200228a(50);
    Func_02002342(1, 0);

    Func_02002298(20);
    Func_0200236c(1, 0x2000, 0);

    Func_020022aa(30);
    Func_020022e8(1, 0x10000, 0x8000);
    Func_02002316(1, 264, 1464);
    Func_02002398(1, 0, 0);
    Func_020023a4(0, 0x8000, 0);
    Func_020023b0(8, 0x4000, 0);

    Func_020022ee(30);
    Func_020023a6(1, 0);

    Func_020022fc(10);
    Func_02002374(0, 3);

    Func_0200230a(30);
    Func_02002382(1, 3);

    Func_02002318(30);
    Func_02002352(1, 0x13333, 0x9999);
    Func_02002392(1, 2);

    object = Func_02002358(0);
    if (object != 0) {
        Func_02002382(1, *(s16 *)(object + 10), *(s16 *)(object + 18));
    }

    Func_020023a0(1);
    Func_020023b2(1, 0, 0);

    Func_02002358_b(20);
    object = Func_02002386(8);
    object[0x5b] = 0;
    Func_020023a4(8, 2);

    object = Func_0200239a(8);

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

    Func_020023aa();
}
