/* NONMATCHING: isolated axis helper trial, 336 of 340 bytes, 154 differing
 * halfwords, 117 aligned edits (2026-09-27). Not admitted; no new DONE.
 * The two reference loops share their waveform calculation and interleaved
 * halfword output; exact BABI_FUNE/HBLANK_SCROLL.C consumes those pages.
 * Hypothesis: one inline axis boundary owns the mask and loop locals,
 * lowering the cross-axis scroll lifetime below the mask's priority.
 * Admission: signed scroll load, shifted word spilled at sp+0, mask in fp
 * across each loop; adoption still requires the complete 340-byte owner,
 * literal pools, repeated exact compile and both ROM comparisons.
 * Baseline greg instead allocates shifted scroll pseudo 65 before masks
 * 81/137: four uses/70 instructions versus three/24 for each mask. The
 * helper retains scroll in fp, omits the stack word and reassigns the
 * state/line/counter roles. Pool membership and topology also change.
 * Full normalized difference read; reject this boundary after one trial,
 * with no parameter-order or declaration sweep. The 332-byte/52-edit
 * baseline remains in the preceding commit and is restored next.
 * Complete boundary 02000f80..020010d4: return at 020010ac,
 * alignment at 020010ae, nine pool words through 020010d0. Interleaved
 * halfword pages reproduce the second axis pointer. Staged phase arithmetic
 * restores state r6, line r5, accumulator r4 and counter r7. Remaining:
 * scroll_y stays in fp instead of spilling its shifted value; the first
 * loop rematerialises 255, multiply operands and second-axis setup differ.
 * Three structural trials: halfword pages alone 340 bytes/99 edits;
 * staged accumulator 332/52 (retained); axis-local masks 328/56 and changed
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

extern struct WaveState *Data_03001ed8;
extern struct BgScroll Data_03001ad0;
extern s16 Data_020094c8[];

static __inline__ void FillWaveAxis(u16 *line, s32 acc, s32 step,
                                  s32 amplitude, u16 base)
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

void Local_02000f80(void)
{
    struct WaveState *state;
    u16 *line;
    u16 scroll_y;
    u16 base;
    s32 acc;
    s32 step;
    s32 amplitude;

    state = Data_03001ed8;
    scroll_y = Data_03001ad0.y;
    line = state->pages[state->page ^ 1];
    step = state->step_x;
    acc = state->phase_x + scroll_y;
    acc *= state->frequency_x;
    amplitude = state->amplitude_x;
    base = Data_03001ad0.x;
    FillWaveAxis(line, acc, step, amplitude, base);
    line = state->pages[state->page ^ 1] + 1;
    step = state->step_y;
    acc = state->phase_y + scroll_y;
    acc *= state->frequency_y;
    amplitude = state->amplitude_y;
    base = scroll_y;
    FillWaveAxis(line, acc, step, amplitude, base);
    state->phase_y++;
    state->page ^= 1;
}
