#include "types.h"

/*
 * resource_3aa owner at 0x02000264, 252 bytes: code 0x02000264-0x02000343 and
 * the seven-word literal pool at 0x02000344-0x0200035f.  The next inventory
 * row starts at 0x02000360, so the owner is exactly its advertised span; the
 * 8-byte outgoing-argument frame it opens with `sub sp, #8` is released by
 * `add sp, #8` before the return, so nothing live escapes.
 *
 * This is the overlay's scene dispatcher.  It re-enables two channels, then
 * switches on the signed halfword `Data_02000240[225]` - the neighbour of the
 * edition selector at index 224 that the byte-exact siblings
 * `assets/code/resource_3aa_c_02000030.c` and `_020000a4.c` test - and runs
 * one of three scene bodies.  Values other than 9, 10, 11 and 20 do nothing.
 *
 * The comparison chain is a compiled `switch`: `cmp #11 / bgt`, then
 * `cmp #10 / bge` for the 10-11 pair, then `cmp #9 / beq`, with 20 tested on
 * the greater-than side.  10 and 11 share one body.
 *
 * Return type, by the interworking-epilogue rule: `pop {r0} / bx r0` pops the
 * return address into r0, so the owner is void.  r0 is written with 1 before
 * the first branch, so the owner takes no arguments either.
 *
 * Call accounting: 18 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3aa 0264` - 16 to import
 * veneers (10 distinct) and 2 to in-overlay prologues, at file offsets 0x1494
 * and 0x0360.  Every site appears below exactly once.  The disassembler's own
 * annotations are wrong in the usual overlay way and were not used.
 *
 * Import shapes, consistent with the already-converted overlays:
 *   Func_080770c0(id)              -> queried predicate; its result is tested
 *                                     against zero at all five sites, so it is
 *                                     used purely as a condition here.
 *   Func_0808a080(selector)        -> record pointer; the halfword at +6 of
 *                                     the returned record is written straight
 *                                     after both calls (resource_373 and
 *                                     resource_39f fix the same shape).
 *   Func_0808a0f0(selector, x, z)  -> 16.16 position setter.
 *   Func_080091c0(a, b, c, d, e, f)-> the six-argument service whose spelling
 *                                     semantic/overlays/resource_39a already
 *                                     carries; the two stack words this owner
 *                                     stores at [sp,#0] and [sp,#4] before the
 *                                     0x02000302 and 0x02000316 branches are
 *                                     arguments five and six, so Func_08009180
 *                                     is called with the same six-argument
 *                                     shape.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The pooled ids 0x941, 0x914, 0x321, 0x915 and 0x109 passed to
 *    Func_080770c0 are opaque here; only the sense of each test is recovered.
 *  - The halfword written at +6 of the Func_0808a080 record is 0x1000 in one
 *    arm and 0xd000 in the other, both built by shifting 0x80 / 0xd0.  Whether
 *    that field is an angle or a flag word is not established.
 *  - Func_0808a0f0(8, 0x038a0000, 0x01a60000) uses a pooled first coordinate
 *    and a shifted second (211 << 17); as 16.16 these are 906.0 and 211.0.
 *  - Both in-overlay branches (0x02001494 and 0x02000360) are taken with r0
 *    holding the zero result of the preceding Func_080770c0, i.e. no argument
 *    is deliberately set, so they are called with none here.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations are mandatory in this overlay. */
void Func_02001cc0();
void Func_02001cc6();
s32 Func_02001cfc();
u8 *Func_02001d46();
s32 Func_02001d12();
void Func_0200174a();
void Func_02001da6();
s32 Func_02001d2c();
void Func_02001dbc(s32, s32, s32);
u8 *Func_02001d82();
s32 Func_02001d50();
void Func_02001d48(s32, s32, s32, s32, s32, s32);
void Func_02001d74(s32, s32, s32, s32, s32, s32);
void Func_02001d58();
void Func_02001d56();
void Func_02001e12();
s32 Func_02001d98();
void Func_0200069c();

/* Signed halfword table in RAM; index 225 selects the scene. */
extern s16 Data_02000240[];

void Func_02000264(void)
{
    u8 *record;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Func_02001cc0(1);
    Func_02001cc6(2);

    switch (Data_02000240[225]) {
    case 9:
        if (Func_02001cfc(0x941) != 0) {
            record = Func_02001d46(8);
            h = 0x1000;
            *(u16 *)(record + 6) = h;

            if (Func_02001d12(0x914) == 0) {
                Func_0200174a();
            }
        } else {
            Func_02001da6(9, 0, 0);
            if (Func_02001d2c(0x321) != 0) {
                Func_02001dbc(8, x1, z1);
                record = Func_02001d82(8);
                h = 0xd000;
                *(u16 *)(record + 6) = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Func_02001d50(0x915) != 0) {
            s32 fifth = 4;
            s32 sixth = 3;
            Func_02001d48(58, 70, 54, 70, fifth, sixth);
            {
                s32 fifth2 = 55;
                s32 sixth2 = 8;
                Func_02001d74(55, 9, 2, 1, fifth2, sixth2);
            }
            Func_02001d58();
            Func_02001d56(1);
        }
        break;

    case 20:
        Func_02001e12(9, 0, 0);
        if (Func_02001d98(0x109) == 0) {
            Func_0200069c();
        }
        break;

    default:
        break;
    }
}
