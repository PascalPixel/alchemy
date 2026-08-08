#include "types.h"

/*
 * Resource 3bf camera/entry gate at 0x02001150 (two-actor variant).
 *
 * Third sibling of the already-converted 0x0200138c and 0x02001528: the same
 * IWRAM slots (0x03001e70 / 0x03001e40), the same +356 record, the same
 * workspace halfwords 382/384/386, the same Data_02000240[294] tail and the
 * same 0x2092 / small-constant stores.  What differs is that this one fetches
 * TWO actors, kinds 9 and 10, keeps them in r7 and sl for the whole body, and
 * clears/sets the byte at +91 on both of them.  The prologue therefore saves
 * r8 and sl explicitly (`mov r7,sl / mov r6,r8 / push {r6,r7}`) and the
 * epilogue restores them from `pop {r3,r5}` in the same order.
 *
 * The epilogue is `pop {r5,r6,r7} / pop {r0} / bx r0`, i.e. r0 holds the
 * popped return address, so this owner is VOID (HANDOVER section 0's
 * interworking-epilogue rule).
 *
 * Call targets were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_3bf 1150` (18 sites, 9 distinct targets), never from the
 * disassembler's pc-relative annotations, which are wrong for every overlay
 * `bl`.  The three import veneers resolve through their trailing word to real
 * main-image functions and are named accordingly:
 *   0x02005610 -> Func_080770c0   state query, one argument
 *   0x02005618 -> Func_080770c8   notifier, one argument
 *   0x02005650 -> Func_0808a080   actor fetch by kind
 * The remaining six targets are ordinary prologues of this same overlay
 * (0x02001108, 0x020018e4, 0x02001918, 0x02005a78, 0x02005af0, 0x02005b6c),
 * named by the tree's offset convention.
 *
 * The +32 field is computed in the overlay's soft-float library rather than in
 * fixed point, which is the one structural difference from the two siblings:
 *   Func_02005af0  s32 -> packed double (actor9->x)
 *   Func_02005a78  double -> s32 truncation
 *   Func_02005b6c  double subtraction, first operand the pooled constant
 * The pooled operand is r0 = 0x41610000, r1 = 0x00000000.  This library keeps
 * the HIGH word of a double in r0 (established by 0x02005af0's -2147483648.0
 * literal pair), so the constant is 8912896.0 == 8.5 * 0x100000 — the same
 * 20-bit tile scale the neighbouring 0x02001324 and 0x02001108 divide by.  It
 * is carried as SoftDouble (a u64 whose low half is the r0 word), matching the
 * three library sources, not as a C `double`.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * printed offsets (module linked at 0x02008000, dispatcher 0x02004638 being
 * the clearest witness).  0x03001e70, 0x03001e40 and 0x02000240 are all
 * outside that band and so are genuine RAM/ROM addresses, not in-image data.
 *
 * Uncertainties recorded rather than smoothed over:
 *  - Func_02001918's byte-exact source in assets/code takes no argument, yet
 *    both call sites here set r0 (9 and 10).  The argument is preserved as
 *    written at the call site; the declaration is old-style so the mismatch is
 *    legal, and this is the documented "one name, different arities" case.
 *  - the byte at actor9 + 91 is stored 0 and then immediately re-read and
 *    tested at 0x020011ec, so that test can never fail.  It is in the
 *    assembly and is kept.
 *  - Data_02000240[294] is re-loaded at each of its four tests rather than
 *    cached; that too is in the assembly.
 *  - the two actors are only touched at +8 and +91, so they are modelled as
 *    opaque byte blocks rather than given a speculative layout.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
s32 Func_020067ac();
s32 Func_020067b4();
s32 Func_020067a6();
s32 Func_020067de();
s32 Func_020067f6();
s32 Func_02006ce6();
s32 Func_02006c7a();
s32 Func_02006d72();
s32 Func_02002314();
s32 Func_02002b3e();
s32 Func_02002b5a();
void Func_02006898();
void Func_020068a0();
s32 Func_02002b72();
void Func_020068b0();
void Func_020068b8();
s32 Func_020068b8_b();

                    

                    

                    

void Func_02001150(void)
{
    u8 *actor9 = Func_020067ac(9);
    u8 *actor10 = Func_020067b4(10);
    u8 *base = *(u8 **)0x03001e70;
    s32 *record = (s32 *)(base + 356);
    u32 flags = *(u32 *)0x03001e40;
    u8 *workspace = *(u8 **)(base + 76);
    const s16 *table = (const s16 *)0x02000240;

    if ((flags & 1u) != 0u) {
        record[6] = 1;   /* +24 */
        record[7] = 1;   /* +28 */
    } else {
        record[6] = -1;
        record[7] = -1;
    }

    if (Func_020067a6(262) != 0
        || *(s16 *)(workspace + 382) != 0
        || *(s16 *)(workspace + 384) == 0) {
        actor10[91] = 1;
        actor9[91] = 1;
        return;
    }

    actor10[91] = 0;
    if (Func_020067de(532) != 0) {
        return;
    }
    actor9[91] = 0;

    if (Func_020067f6(532) == 0 && actor9[91] == 0) {
        u64 value = Func_02006ce6(*(s32 *)(actor9 + 8));

        /* r0/r1 = 0x41610000 / 0 == 8912896.0 in this library's word order. */
        record[8] = Func_02006d72(Func_02006c7a((u64)0x41610000u, value));
    }

    if (Func_02002314() != 0) {
        return;
    }

    if (table[294] == 0) {
        goto notify;
    }
    if (Func_02002b3e(9) != 0 && table[294] != 0) {
        *(s16 *)(workspace + 382) = 0x2092;
        return;
    }
    if (Func_02002b5a(10) != 0) {
        if (table[294] == 0) {
            goto notify;
        }
        *(s16 *)(workspace + 382) = 0x2092;
        return;
    }
    if (table[294] != 0) {
        goto lastGate;
    }

notify:
    if (Func_02002b5a(9) != 0) {
        Func_02006898(0x215);
        Func_020068a0(532);
    }
    if (Func_02002b72(10) != 0) {
        Func_020068b0(0x215);
        Func_020068b8(532);
    }

lastGate:
    if (Func_020068b8_b(532) != 0) {
        *(s16 *)(workspace + 386) = 91;
    }
}
