/*
 * resource_3b7 owner at 0x02000d70, 96 bytes (0x02000d70-0x02000dcf).
 *
 * Picks a non-repeating variant line for a topic and returns its text id.
 *
 * Code runs 0x02000d70-0x02000dc7.  The last two words, 0x02000dc8 and
 * 0x02000dcc, are a literal pool (0x02000240 and 0x0200a018); both sit past the
 * `bx r1` return and are reached only by the two `ldr rN, [pc, ...]` loads at
 * 0x02000d8c and 0x02000db8, so they are data, not instructions.
 *
 * LINK BASE - this overlay is confirmed linked at 0x02008000.  The pool word
 * 0x0200a018 is an in-image address at file offset 0x2018, and the bytes there
 * are fifteen consecutive `.4byte` word ids - (0xfa, 0xfb, 0xfc), (0x100,
 * 0x101, 0x102), (0x106, 0x107, 0x108), (0xb7, 0xb6, 0xb5), (0xbd, 0xba, 0xbc)
 * - exactly five groups of three, which is exactly how the code indexes it
 * (`(topic * 3 + variant) * 4`).  `assets/code/resource_3b7_overlay.s` spells
 * the same fifteen words literally in its trailing data.  That is the witness
 * for the base; the other pool word, 0x02000240, is below the in-image band and
 * is the RAM workspace the byte-exact siblings
 * `assets/code/resource_3b7_c_02000044.c` and `..._020001a8.c` already name, so
 * it is used raw and unshifted, per the project convention.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x02000d82 -> veneer 0x02001874 -> Func_080000f8
 *   0x02000d9a -> veneer 0x02001874 -> Func_080000f8
 *   0x02000daa -> veneer 0x0200185c -> Func_030003ac
 * Completeness: 3 sites over 2 distinct targets, matching the tool's
 * `sites=3 distinct_targets=2` and the inventory row's calls=3.
 *
 * Func_080000f8 is the 16-bit random source: both uses are the standard
 * `(rand * N) >> 16` reduction (`* 5 >> 16` for a topic in 0..4, `* 2 >> 16`
 * for a coin flip).  Func_030003ac is the relocated IWRAM remainder helper -
 * the same identification `semantic/overlays/resource_3cb_c_02001294.c` makes,
 * where its result is used directly as a decimal digit, and the counterpart of
 * the quotient helper at 0x03000380 that `src/080f2ebc.c` calls through a
 * function pointer.
 *
 * The variant walk is the classic no-immediate-repeat pick: the previous
 * variant is kept as a signed byte at `Data_02000240[308 + topic]`, and the new
 * one is `(previous + 4 + coin) % 3`, i.e. previous+1 or previous+2 modulo 3,
 * which can never equal `previous`.  The new variant is stored back before it
 * is used.
 *
 * The epilogue is `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r1} /
 * bx r1`; the popped register is r1, not r0, so r0 survives and is the result.
 * The owner returns s32.
 *
 * Uncertainties: the `topic < 0` guard returns the r0 = 0 preset, so 0 is
 * returned rather than a table id - whether callers treat 0 as "no line" is not
 * established here.  The imports' interfaces are unknown, hence the old-style
 * declarations.  Topics above 5 would index past the fifteen-word table; no
 * call site in this owner bounds them.
 */

typedef signed int s32;
typedef signed char s8;

extern s8 Data_02000240[];      /* RAM workspace, also indexed as s16 elsewhere */
extern s32 Data_0200a018[];     /* in-image, file offset 0x2018: 5 topics x 3 ids */

s32 Func_080000f8();
s32 Func_030003ac();

s32 Func_02000d70(s32 topic)
{
    s32 cursor;
    s32 variant;

    if (topic < 0) {
        return 0;
    }

    /* Topic 5 means "any": reduce a 16-bit random to 0..4. */
    if (topic == 5) {
        topic = (s32)((unsigned int)(Func_080000f8() * 5) >> 16);
    }

    cursor = Data_02000240[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Func_030003ac(cursor + (s32)((unsigned int)(Func_080000f8() * 2) >> 16) + 4, 3);

    Data_02000240[308 + topic] = (s8)variant;

    return Data_0200a018[topic * 3 + variant];
}
