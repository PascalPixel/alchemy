#include "types.h"

/*
 * Resource 3bc owner at 0x0200391c (316 bytes), the independently addressed
 * twin of resource_3ba:2bec.  Its high-register unwind returns at
 * 0x02003a44-0x02003a53 and the trailing zero pool word ends at 0x02003a57,
 * immediately before the next owner.
 *
 * The routine advances two interpolated halfword coordinates while a duration
 * is active, then emits a three-word display command during phases 1..14 of a
 * 20-step cycle.  The two Func_03000380 calls and the Func_080001e8 call are
 * the complete three-site, two-target call inventory.
 */

extern s16 Data_0200dace;
extern u8 Data_03001b10[];
extern s16 Data_0200dbcc;
extern u32 Data_0200dbf0[];
extern u16 Data_0200db90;
extern u16 Data_0200dc34;
extern s16 Data_0200dbe4;
extern s16 Data_0200dba0;
extern u16 Data_0200dbc0;
extern s16 Data_0200dc40;
extern u16 Data_0200dbfc;
extern s16 Data_0200dbb4;
extern s16 Data_0200db98;

s32 Func_03000380();
void Func_080001e8();

void Func_0200391c(void)
{
    s32 tile = *(u16 *)(Data_03001b10 + Data_0200dace * 4 + 2) >> 5;
    s16 duration = Data_0200dbcc;
    s16 phase;

    if (duration != 0) {
        s16 frame = (s16)(Data_0200db90 + 1);
        s32 delta;

        Data_0200db90 = (u16)frame;

        delta = Data_0200dba0 - Data_0200dbe4;
        Data_0200dc34 = (u16)(Data_0200dc34 +
            Func_03000380(frame * delta, duration));

        delta = Data_0200dc40 - (s16)Data_0200dbfc;
        Data_0200dbc0 = (u16)(Data_0200dbfc +
            Func_03000380(frame * delta, duration));

        if (frame >= duration) {
            Data_0200dbcc = 0;
        }
        Data_0200dbb4 = 0;
    }

    phase = (s16)(Data_0200dbb4 + 1);
    Data_0200dbb4 = phase;

    if (phase <= 13) {
        u32 packed;

        Data_0200dbf0[0] = 0;
        packed = (u32)((s32)(s16)Data_0200dbc0 - 8);
        packed |= (u32)((s32)(s16)Data_0200dc34 - 8) << 16;
        packed |= 0x40000000;
        packed |= (u32)Data_0200db98 << 28;
        Data_0200dbf0[1] = packed;
        Data_0200dbf0[2] = (u32)tile | 0x400;
        Func_080001e8(Data_0200dbf0, 255);
    } else if (phase > 19) {
        Data_0200dbb4 = 0;
    }
}
