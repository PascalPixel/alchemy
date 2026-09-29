/* NONMATCHING: canonical ship H2 retained, 336/340 bytes, 111 differing
 * halfwords, 62 aligned edits. The three product trials are closed;
 * all attempts are preserved. Frequency owns the accumulator, signed
 * scroll spills at sp+0, both masks occupy fp and the pool has nine words.
 * Remaining: accumulator copy, high-register ownership and second-axis
 * address reuse. The complete extent/pool is not exact; no DONE.
 * Ship H3 separate frequency input, 340/340 bytes,
 * 56 differing halfwords, 51 aligned edits (2026-09-27). A distinct product
 * destination restores extent and native loop-alignment placement, but
 * copies the phase into r4, not frequency, losing the admitted H2 product
 * ownership. Signed scroll, sp+0 spill and both fp masks still survive.
 * All nine pool values survive, with the last two in reversed order;
 * high-register roles and second-axis address reuse remain different.
 * Full normalized diff read. Reject this ownership model despite its
 * smaller edit count; retain H2 as canonical successor and close the
 * three-attempt product axis. No exact function or alignment credit.
 * Ship H2 separate phase, 336/340 bytes, 111 differing
 * halfwords, 62 aligned edits (2026-09-27). Computing phase first restores
 * its first-axis load order and the nine-word pool. Frequency owns the r4
 * product, but loads directly there instead of the reference r3-to-r4 copy.
 * Signed scroll, sp+0 spill, fp masks and state/line/counter roles survive.
 * Step/amplitude/base/routine high-register ownership and second-axis
 * address reuse remain different. Full normalized difference read.
 * Next bounded hypothesis: a separate frequency input and fresh product
 * destination preserve phase order but emit the reference accumulator copy.
 * Ship H1 frequency staging, 332/340 bytes, 122 differing
 * halfwords, 70 aligned edits (2026-09-27). Assigning frequency to acc
 * before the phase product gives r4 the reference multiply ownership,
 * unlike the earlier fused expression. It loads frequency before phase,
 * though, omits the reference's separate r3-to-r4 copy and changes pool
 * membership. Signed scroll load, sp+0 spill and both fp masks survive.
 * Full normalized diff read; this checkpoint earns no exact bytes.
 * Next bounded hypothesis: a separate phase local computed first restores
 * phase/frequency load order while leaving frequency as accumulator.
 * Previous word-sized output base, 336 of 340 bytes, 112 differing
 * halfwords, 64 aligned edits (2026-09-27). Local invariant admitted;
 * the complete owner is not exact and earns no new DONE.
 * The two reference loops share their waveform calculation and interleaved
 * halfword output; exact BABI_FUNE/HBLANK_SCROLL.C consumes those pages.
 * Hypothesis: one inline axis boundary owns the mask and loop locals,
 * lowering the cross-axis scroll lifetime below the mask's priority.
 * Admission: signed scroll load, shifted word spilled at sp+0, mask in fp
 * across each loop; adoption still requires the complete 340-byte owner,
 * literal pools, repeated exact compile and both ROM comparisons.
 * Baseline greg instead allocates shifted scroll pseudo 65 before masks
 * 81/137: lreg reports four uses/54 instructions versus three/36 per mask.
 * The earlier life pass reported four/70 versus three/24. The
 * helper retains scroll in fp, omits the stack word and reassigns the
 * state/line/counter roles. Pool membership and topology also change.
 * Full normalized difference read; reject this boundary after one trial,
 * with no parameter-order or declaration sweep; preserved at 4cbb52b73.
 * H2 restores the original loops and widens only their output base to u32.
 * Baseline RTL widens the second scroll twice: once before its phase sum,
 * then again after loop hoisting the u16 base. Sharing that word conversion
 * was the prediction, not a spelling sweep. The signed ldrsh remains,
 * shifted scroll spills at sp+0 and both masks occupy fp, as in the ROM.
 * Freeze those three admitted facts even though the old baseline had only
 * 52 aligned edits. Full difference read: step/base/amplitude/routine roles,
 * initial multiply operands and second-axis address reuse remain wrong;
 * the candidate has eight pool words instead of nine and differs in layout.
 * Repair outward only while preserving the admitted load/spill/mask shape.
 * One outward product trial replaced each axis's two acc assignments with
 * acc = state->frequency_x * (state->phase_x + scroll_y), and the analogous
 * y expression. Prediction: frequency would become the multiply accumulator
 * as in ROM. Result 344/340 bytes, 97 halfwords / 91 aligned edits: spill
 * and fp masks survive, but multiplication still accumulates the phase sum,
 * and state/line/counter move to r7/r6/r4. It also adds a tenth pool word.
 * Full diff rejects this product model; retain H2, byte-identically, and
 * stop the expression axis rather than permuting operands or declarations.
 * Output-view follow-up: keep u32 base but spell both stores as
 * *line = off + (u16)base. Prediction: a distinct narrow output view might
 * lower the shared base priority without losing the scroll spill or masks.
 * The complete result is byte-identical to H2 (336/112/64); the conversion
 * is absorbed by the halfword store. Close this view axis, with H2 retained.
 * Complete boundary 02000f80..020010d4: return at 020010ac,
 * alignment at 020010ae, nine pool words through 020010d0. Interleaved
 * halfword pages reproduce the second axis pointer. Historical 332-byte
 * baseline: staged phase arithmetic restored state r6, line r5, accumulator
 * r4 and counter r7, but retained these now-superseded differences:
 * scroll_y stays in fp instead of spilling its shifted value; the first
 * loop rematerialises 255, multiply operands and second-axis setup differ.
 * Three structural trials: halfword pages alone 340 bytes/99 edits;
 * staged accumulator 332/52; axis-local masks 328/56 and changed
 * topology. Earlier: shared mask 336; separate loop counters unchanged;
 * packed scroll 316/64 lost the required signed load. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct WaveState {
    u16 pages[2][960];
    u8 page;
    u8 unknown_f01;
    u16 phase_x;
    u16 phase_y;
    u8 unknown_f06[2];
    s32 frequency_x;
    s32 frequency_y;
    s32 step_x;
    s32 step_y;
    s32 amplitude_x;
    s32 amplitude_y;
};

struct BgScroll {
    u16 unknown_00[6];
    u16 x;
    s16 y;
};

extern struct WaveState *gHBlankScrollWork;
extern struct BgScroll gBgScroll;
extern s16 Data_020094c8[];

void Local_02000f80(void)
{
    struct WaveState *state;
    u16 *line;
    u16 scroll_y;
    u32 base;
    s32 acc;
    s32 step;
    s32 amplitude;
    s32 phase;

    state = gHBlankScrollWork;
    scroll_y = gBgScroll.y;
    line = state->pages[state->page ^ 1];
    step = state->step_x;
    phase = state->phase_x + scroll_y;
    acc = state->frequency_x;
    acc *= phase;
    amplitude = state->amplitude_x;
    base = gBgScroll.x;
    {
        s32 i;
        u16 off;

        for (i = 0; i != 160; i++) {
            off = Iwram_MulQ16(Data_020094c8[(acc >> 16) & 0xff], amplitude) / 256;
            *line = off + base;
            acc += step;
            line += 2;
        }
    }
    line = state->pages[state->page ^ 1] + 1;
    step = state->step_y;
    phase = state->phase_y + scroll_y;
    acc = state->frequency_y;
    acc *= phase;
    amplitude = state->amplitude_y;
    base = scroll_y;
    {
        s32 i;
        u16 off;

        for (i = 0; i != 160; i++) {
            off = Iwram_MulQ16(Data_020094c8[(acc >> 16) & 0xff], amplitude) / 256;
            *line = off + base;
            acc += step;
            line += 2;
        }
    }
    state->phase_y++;
    state->page ^= 1;
}
