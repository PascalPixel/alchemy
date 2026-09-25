/* NONMATCHING: 316 bytes, candidate 312, 146 differing halfwords, 95
 * halfword edits (2026-09-25). KorosseoMaruta_Func0200391c, meant for
 * FIELD/KOROSSEO_MARUTA/F_0391C.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Structured interpolation and blinking recovered;
 * candidate is four bytes short and differs in allocation and scheduling
 * across the complete owner.
 * WALL: recover state layout and live ranges before
 * permutation */
#include "TYPES.H"

extern s16 Data_0200dace;
extern u16 Data_03001b10[];
extern s16 Data_0200dbcc;
extern u16 Data_0200db90;
extern s16 Data_0200dc34;
extern s16 Data_0200dbe4;
extern s16 Data_0200dba0;
extern s16 Data_0200dbc0;
extern s16 Data_0200dbfc;
extern s16 Data_0200dc40;
extern s16 Data_0200dbb4;
extern s16 Data_0200db98;
extern u32 Data_0200dbf0[3];

extern s32 Engine_MathDivide(s32 numerator, s32 denominator);
extern void Main_080001e8(void *record, s32 mask);

struct OamTile {
    u16 attr;
    u16 tile;
};

void KorosseoMaruta_Func0200391c(void)
{
    s32 tile;
    s32 duration;
    s32 step;
    s32 blink;
    u16 start;
    s32 diff;
    u32 *record;

    tile = ((struct OamTile *)Data_03001b10)[Data_0200dace].tile >> 5;
    duration = Data_0200dbcc;
    if (duration != 0) {
        step = ++Data_0200db90;
        step = (s16)step;

        start = Data_0200dbe4;
        diff = Data_0200dba0 - Data_0200dbe4;
        Data_0200dc34 = start + Engine_MathDivide(
            step * diff, duration);

        start = Data_0200dbfc;
        diff = Data_0200dc40 - Data_0200dbfc;
        Data_0200dbc0 = start + Engine_MathDivide(
            step * diff, duration);

        if (step >= duration)
            Data_0200dbcc = 0;
        Data_0200dbb4 = 0;
    }

    blink = ++Data_0200dbb4;
    blink = (s16)blink;
    if (blink <= 13) {
        record = Data_0200dbf0;
        *record++ = 0;
        *record++ = (Data_0200dbc0 - 8)
            | ((Data_0200dc34 - 8) << 16)
            | 0x40000000
            | (Data_0200db98 << 28);
        *record = tile | 0x400;
        Main_080001e8(Data_0200dbf0, 255);
    } else if (blink > 19) {
        Data_0200dbb4 = 0;
    }
}
