/* NONMATCHING: 480/480 bytes, seven differing halfwords (2026-09-27).
 * Entry ownership H1: capture event = Data_03001ebc[0] before acquiring
 * the palette source, then inspect event+0x17e. This gives the reference
 * shared-base load order and changes reload ancestry through the loop:
 * the full entry, tint loads, nine attenuation sequences and color read
 * now match. All five pool words and frame4 remain exact. Only the final
 * color publication/index advancement sequence differs (seven halfwords).
 * H2 moved i++ before color construction: 480/12; it advances too early,
 * before both shifts, and changes the store/index scratch registers.
 * H3 first builds color, then increments i and stores color: 480/13;
 * it changes both shift registers and the packed-color register. Full
 * normalized diffs read; neither preserves the exact packing prefix.
 * Retain H1, freeze its complete prefix and pool. Three models complete;
 * no further publication spelling without new scheduling evidence. DONE +0.
 * Compiler follow-up: H3 replaces generated result227 with the input-color
 * user pseudo41. One distinct output-local trial tests that lifetime join;
 * it emits the same 480/13 binary as H3, including both packing-register
 * regressions (cmp checked). This publication-temporary axis is stopped.
 * H1 ordinary and diagnostic assembly are identical; canonical stays H1.
 *
 * Lifetime-transfer audit (2026-09-27): fresh complete score remains
 * 480/480, 30 halfwords / 29 aligned edits. Ordinary and -da assembly agree.
 * CSE owns src/dst/index as user pseudos 32/33/34; global allocation already
 * gives them reference r8/r9/sl, and tints 35/36/37 get r4/r7/r6 with frame4.
 * Unlike COND_ACT's late speaker lifetime or Vault's dead-leader-to-slot
 * transition, all three pointer/index values stay live together throughout
 * this loop. The entry event-pointer temporary dies while src remains live;
 * there is no demonstrated shared saved-register phase boundary to transfer.
 * Source load insn12 is reloaded into r2 then r8; event load17 reuses r3.
 * All nine attenuation sequences and five pool words remain admitted.
 * No new model tested: need evidence for the entry expression/reload or
 * iteration ownership, not another tint ordering or separate-global trial.
 * NONMATCHING: complete 480-byte owner, candidate 480, 30 differing
 * halfwords, 29 aligned edits (2026-09-27). Separate-global trial rejected;
 * the complete canonical body and original bindings are restored.
 * Reproducible single-overlay unit binds its imports and data symbols.
 * ENTER_ROOM installs
 * this palette callback at runtime address 0x02009245.
 * Prior baseline: hand-written from the disassembly; the goto loop keeps the clamp
 * zero and 31 unhoisted as the reference does. The reference allocates the
 * red tint to r4 with caller-saves around each divide and holds the red
 * half-sum (r5) across the first divide, i.e. evaluates red/2 before the
 * call; here preexpand_calls puts the divide first for all three and the
 * tints take r5-r7, so 16 bytes are missing. A half temporary before the call
 * reaches 480 bytes but takes a fourth high register.
 * Bounded negative result: sharing the red output local as the attenuation
 * accumulator gave 488 bytes, 239 differing halfwords and 180 edits. It
 * kept the partial sums in r5 but introduced fp, moved blue to r8 and kept
 * the shifts before the divide. That rejected model is not repeated.
 * Family H1: FIELD_EVENT.H Math_Divide boundary from exact KORIMA_KI and
 * KORIMA_HIROBA palette consumers, plus u32 color from exact ADJUST_BANK
 * and COPY_BANKS_WITH_BRIGHTNESS_OFFSET. The word local restores ldrh and
 * simple channel masks and moves all five literals to the final pool.
 * The inline call boundary still expands divide before the red numerator;
 * r5/r6/r7 tints and no caller-save frame remain. Whole diff inspected.
 * Baseline 464/222/175 is preserved before this commit. H1 is a better
 * typed RGB model, not an adoption: 0 new DONE bytes.
 * Family H2: ordinary C /3 and /5, following exact COMMON/EFFECT/SPAWN.C,
 * with __divsi3 bound to the existing 0x02009a10 veneer. All nine calls
 * resolve to resident signed division at 0x03000380. This restores the
 * reference four-byte caller-save frame and unshifted quarter numerator
 * live across every /5 call, without a new accumulator or high register.
 * Full diff: 472/221/122; red/blue take r6/r4 rather than r4/r6, red /2
 * still follows its /3 call, and output channels coalesce into saved tint
 * registers.
 * Family H3: transfer exact ADJUST_BANK's separate extraction of all three
 * components before adding tint channels. Whole 480-byte extent and all
 * pools now agree in layout: 30 halfwords / 29 edits. The complete nine
 * division/attenuation sequences match, including red's unshifted half
 * numerator, r4/r7/r6 tints and caller-saves, and no fp is introduced.
 * Residual: initial globals/palette-source load order, destination/index
 * setup, green/blue tint-load scheduling and index compare scratch, the
 * source palette-load scratch, and output-store/index-increment scheduling.
 * All three bounded family hypotheses are preserved in Git. No declaration
 * permutations or old accumulator trial. 0 new DONE; keep this canonical
 * typed model. Further work needs a supported palette-work/iteration
 * ownership boundary, preserving the now-exact attenuation body and pool.
 *
 * Global-ownership H4 (Sep 27): exact ARUTIN_YAMA/PALETTE_STEP.C declares
 * the field palette backing pointer at 03001ed0 as u16 *. Exact adjacent
 * TORETO_HEYA/PALETTE.C captures hardware palette RAM into that buffer.
 * FIELD_EVENT.H separately owns gEventWork at 03001ebc and the signed
 * psynergy_request field at +0x17e. They are distinct pointer cells, not
 * elements of an evidenced homogeneous event-work array.
 * The own-ROM entry nevertheless loads one 03001ebc literal, then the
 * event pointer from [r3] before the palette pointer from [r3,#20]. Its
 * final pool has five words. One model with the two independent externs
 * and canonical gEventWork->psynergy_request emits 488/480, 236 differing
 * halfwords, 49 aligned edits: an extra entry load, two-byte alignment pad
 * and sixth pool word. Whole normalized diff read. All nine attenuation
 * sequences retain their instruction/register structure; calls and branches
 * move with the two-byte entry growth. It does not repair the old loop
 * setup, tint-load, source-load or output/index scheduling residuals.
 * Reject as a match path; do not sweep tints/registers. This complete
 * trial is preserved at b056c2f0f; canonical restoration is checked against
 * the pre-trial candidate binary, including all attenuation code and pool.
 * The ownership fact is established, but the original source expression
 * that permits the shared-base loads in the ROM remains unproven. DONE +0. */
#include "TYPES.H"
#include "FIELD_EVENT.H"


/* FAKEMATCH: array view of distinct linker-adjacent pointer cells preserves
 * the ROM's shared address base; it does not establish one source array. */
extern u8 *Data_03001ebc[];
extern u32 Data_03001e40;
/* Index of the current tint in Data_02009f00. */
extern s32 Data_0200adb8;
/* Red, green and blue tints in threes; a red of 99 ends the list. */
extern s32 Data_02009f00[];

/* Every 32 frames, tints the background palette from the next random entry
 * of the tint list, weaker for the later colours. */
void ToretoPalette_ApplyTint(void)
{
    u16 *src;
    u16 *dst;
    u32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 red;
    s32 green;
    s32 blue;
    u32 color;
    u8 *event;

    event = Data_03001ebc[0];
    src = (u16 *)Data_03001ebc[5];
    if (*(s16 *)(event + 0x17e) != 0)
        return;
    if ((Data_03001e40 & 31) != 0)
        return;
    src += 16;
    dst = (u16 *)0x05000020;
    i = 0;
loop:
    {
        r = Data_02009f00[Data_0200adb8];
        g = Data_02009f00[Data_0200adb8 + 1];
        b = Data_02009f00[Data_0200adb8 + 2];
        if (i > 47) {
            r -= r / 2 + r / 3;
            g -= g / 2 + g / 3;
            b -= b / 2 + b / 3;
        } else if (i > 31) {
            r -= r / 3 + r / 4;
            g -= g / 3 + g / 4;
            b -= b / 3 + b / 4;
        } else if (i > 15) {
            r -= r / 4 + r / 5;
            g -= g / 4 + g / 5;
            b -= b / 4 + b / 5;
        }
        color = *src;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += r;
        green += g;
        blue += b;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *dst = (blue << 10) | (green << 5) | red;
        i++;
        dst++;
        src++;
    }
    if (i <= 62)
        goto loop;
    Data_0200adb8 += (Engine_RandomNext() & 7) * 3;
    if (Data_02009f00[Data_0200adb8] == 99)
        Data_0200adb8 = 0;
}
