#include "types.h"

/*
 * Fill the back page of a 160-row scroll table with a per-scanline sine wobble,
 * then flip the page.  The row is three 32-bit values; this owner writes the
 * low halfwords of all 160 rows first (starting at the page base) and the high
 * halfwords second (starting at page base + 2), which is why the two passes
 * step by 4 and are separated by the page-offset recomputation at 0x0809462c.
 *
 * Each pass has a constant arm and a modulated arm.  When the pass's step word
 * - base+0xf10 for the low pass, base+0xf14 for the high pass - is zero the
 * six base values read from Data_03001ad0 are stored unchanged into every row;
 * otherwise the accumulator base+0xf08 (respectively base+0xf0c) times the
 * frame counter at base+0xf02 plus the base value walks the 256-entry signed
 * table at 0x0809ed84, and the sampled amplitude is scaled by base+0xf18
 * (respectively base+0xf1c).
 *
 * The mov ip,pc ; bx sl at 0x08094600 and 0x080946b8 are ordinary indirect
 * calls to the pooled 0x03000118, the IWRAM-relocated ARM fixed-point
 * multiply, each landing site consuming r0 in the same frame, as recorded for
 * 0x0800ebec.  The movs r0,r0 at 0x080945fe is not padding: it is inside the
 * instruction stream, and it shifts the mov ip,pc so that the return address
 * it captures lands past the bx.  The second call site needs no such
 * adjustment and has none.
 *
 * cmp r0,#0 ; bge ; adds r0,#255 followed by lsls #8 ; lsrs #16 is the
 * truncating divide by 256 of a signed product, kept to sixteen bits.
 *
 * The epilogue at 0x080946fc unwinds the frame and ends pop {r0} ; bx r0, so
 * the owner is void.  Published as a callback at 0x080947dc and 0x08094810,
 * with no direct call site.
 */

struct ScrollRow_08094544 {
    u16 first_low;
    u16 first_high;
    u16 second_low;
    u16 second_high;
    u16 third_low;
    u16 third_high;
};

struct Wobble_08094544 {
    struct ScrollRow_08094544 pages[2][160];
    u8 page;
    u8 unknown_f01;
    u16 counter;
    u8 unknown_f04[4];
    s32 low_rate;
    s32 high_rate;
    s32 low_step;
    s32 high_step;
    s32 low_amplitude;
    s32 high_amplitude;
};

struct Base_08094544 {
    u8 unknown_00[4];
    s16 third_low;
    s16 third_high;
    s16 second_low;
    s16 second_high;
    s16 first_low;
    s16 first_high;
};

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08094544)(s32 a, s32 b);

extern struct Wobble_08094544 *Data_03001ed8;
extern struct Base_08094544 Data_03001ad0;

void Func_08094544(void)
{
    struct Wobble_08094544 *wobble = Data_03001ed8;
    s16 first_high = Data_03001ad0.first_high;
    s16 first_low = Data_03001ad0.first_low;
    s16 second_high = Data_03001ad0.second_high;
    s16 second_low = Data_03001ad0.second_low;
    s16 third_high = Data_03001ad0.third_high;
    s16 third_low = Data_03001ad0.third_low;
    const s16 *table = (const s16 *)0x0809ed84;
    Multiply_08094544 multiply;
    struct ScrollRow_08094544 *row;
    u16 *slot;
    s32 step;
    s32 amplitude;
    u32 phase;
    s32 count;

    row = wobble->pages[wobble->page ^ 1];
    step = wobble->low_step;
    phase = (u32)wobble->low_rate *
            (u32)(wobble->counter + (u16)first_high);

    if (step == 0) {
        for (count = 0; count < 160; count++) {
            row[count].first_low = (u16)first_low;
            row[count].second_low = (u16)second_low;
            row[count].third_low = (u16)third_low;
        }
    } else {
        amplitude = wobble->low_amplitude;
        multiply = (Multiply_08094544)0x03000118;
        slot = &row->first_low;

        for (count = 0; count < 160; count++) {
            s32 product = multiply(table[(phase >> 16) & 255], amplitude);
            u16 delta = (u16)((product < 0 ? product + 255 : product) >> 8);

            slot[0] = (u16)first_low + delta;
            slot[2] = (u16)second_low + delta;
            slot[4] = (u16)third_low + delta;
            slot += 6;
            phase += (u32)step;
        }
    }

    row = wobble->pages[wobble->page ^ 1];
    step = wobble->high_step;
    phase = (u32)wobble->high_rate *
            (u32)(wobble->counter + (u16)first_high);

    if (step == 0) {
        for (count = 0; count < 160; count++) {
            row[count].first_high = (u16)first_high;
            row[count].second_high = (u16)second_high;
            row[count].third_high = (u16)third_high;
        }
    } else {
        amplitude = wobble->high_amplitude;
        multiply = (Multiply_08094544)0x03000118;
        slot = &row->first_high;

        for (count = 0; count < 160; count++) {
            s32 product = multiply(table[(phase >> 16) & 255], amplitude);
            u16 delta = (u16)((product < 0 ? product + 255 : product) >> 8);

            slot[0] = (u16)first_high + delta;
            slot[2] = (u16)second_high + delta;
            slot[4] = (u16)third_high + delta;
            slot += 6;
            phase += (u32)step;
        }
    }

    wobble->counter++;
    wobble->page ^= 1;
}
