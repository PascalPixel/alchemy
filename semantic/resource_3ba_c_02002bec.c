#include "types.h"

/*
 * Resource 3ba owner at 0x02002bec (316 bytes), paired structurally with
 * resource_3bc:391c.  The high-register prologue has one matching unwind
 * return at 0x02002d14-0x02002d23; the zero pool word at 0x02002d24 closes
 * the owner immediately before 0x02002d28.
 *
 * When an interpolation duration is active, the routine advances two signed
 * halfword coordinates by frame/duration fractions, clears the duration on
 * completion, and restarts the 20-step display phase.  Phases 1..14 build a
 * three-word display command; phases 15..20 are quiet, after which the phase
 * wraps to zero.  Both division calls and the single display call resolve
 * through veneers and appear below in machine order.
 */

extern s16 Data_0200c6a6;
extern u8 Data_03001b10[];
extern s16 Data_0200c78c;
extern u32 Data_0200c7b0[];
extern u16 Data_0200c750;
extern u16 Data_0200c7f4;
extern s16 Data_0200c7a4;
extern s16 Data_0200c760;
extern u16 Data_0200c780;
extern s16 Data_0200c800;
extern u16 Data_0200c7bc;
extern s16 Data_0200c774;
extern s16 Data_0200c758;

s32 Func_03000380();
void Func_080001e8();

void Func_02002bec(void)
{
    s32 tile = *(u16 *)(Data_03001b10 + Data_0200c6a6 * 4 + 2) >> 5;
    s16 duration = Data_0200c78c;
    s16 phase;

    if (duration != 0) {
        s16 frame = (s16)(Data_0200c750 + 1);
        s32 delta;

        Data_0200c750 = (u16)frame;

        delta = Data_0200c760 - Data_0200c7a4;
        Data_0200c7f4 = (u16)(Data_0200c7f4 +
            Func_03000380(frame * delta, duration));

        delta = Data_0200c800 - (s16)Data_0200c7bc;
        Data_0200c780 = (u16)(Data_0200c7bc +
            Func_03000380(frame * delta, duration));

        if (frame >= duration) {
            Data_0200c78c = 0;
        }
        Data_0200c774 = 0;
    }

    phase = (s16)(Data_0200c774 + 1);
    Data_0200c774 = phase;

    if (phase <= 13) {
        u32 packed;

        Data_0200c7b0[0] = 0;
        packed = (u32)((s32)(s16)Data_0200c780 - 8);
        packed |= (u32)((s32)(s16)Data_0200c7f4 - 8) << 16;
        packed |= 0x40000000;
        packed |= (u32)Data_0200c758 << 28;
        Data_0200c7b0[1] = packed;
        Data_0200c7b0[2] = (u32)tile | 0x400;
        Func_080001e8(Data_0200c7b0, 255);
    } else if (phase > 19) {
        Data_0200c774 = 0;
    }
}
