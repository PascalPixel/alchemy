typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

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
 * `bun tools/overlay_call_targets.ts resource_3aa 0264` - 16 to import
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
void Func_08009190();
/* Used for its return value at every site: a zero/non-zero query. */
s32 Func_080770c0();
void Func_0808a0f0();
void Func_08009180();
void Func_080091c0();
void Func_08009128();
void Func_080000c0();

/* Used for its return value: the record whose halfword at +6 is written. */
u8 *Func_0808a080();

/* In-overlay scene bodies at file offsets 0x1494 and 0x0360. */
void Func_02001494();
void Func_02000360();

/* Signed halfword table in RAM; index 225 selects the scene. */
extern s16 Data_02000240[];

void Func_02000264(void)
{
    u8 *record;

    Func_08009190(1);
    Func_08009190(2);

    switch (Data_02000240[225]) {
    case 9:
        if (Func_080770c0(0x941) != 0) {
            record = Func_0808a080(8);
            *(u16 *)(record + 6) = 0x1000;

            if (Func_080770c0(0x914) == 0) {
                Func_02001494();
            }
        } else {
            Func_0808a0f0(9, 0, 0);
            if (Func_080770c0(0x321) != 0) {
                Func_0808a0f0(8, 0x038a0000, 0x01a60000);
                record = Func_0808a080(8);
                *(u16 *)(record + 6) = 0xd000;
            }
        }
        break;

    case 10:
    case 11:
        if (Func_080770c0(0x915) != 0) {
            Func_08009180(58, 70, 54, 70, 4, 3);
            Func_080091c0(55, 9, 2, 1, 55, 8);
            Func_08009128();
            Func_080000c0(1);
        }
        break;

    case 20:
        Func_0808a0f0(9, 0, 0);
        if (Func_080770c0(0x109) == 0) {
            Func_02000360();
        }
        break;

    default:
        break;
    }
}
