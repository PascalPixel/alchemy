#include "types.h"

/*
 * Resource 39c owner at 0x02003948 (284 bytes): a three-group formation
 * builder derived independently from the bytes, using the eight-argument
 * spawn ABI already established by this overlay's owner at 0x0200013c and
 * the related formation routine at 0x020017a8.
 *
 * Complete owner: high-register/frame prologue at 0x02003948, matching
 * unwind and interworking return through 0x02003a55, one alignment halfword,
 * and three referenced literal words at 0x02003a58-0x02003a63.  The next
 * owner starts at 0x02003a64.
 *
 * The outer loop builds three groups.  Members 1..7 advance a shared Z base;
 * only odd members spawn.  Bit 1 selects between two geometries, each with
 * its own random sample and one call to Func_0200013c, followed by a one-frame
 * wait.  The shared Z base deliberately continues across group boundaries.
 *
 * Call completeness: Func_08009180 x3, Func_080000f8 x2,
 * Func_0200013c x2 and Func_0808a010 x1.  The two local calls resolve to the
 * real 0x0200013c prologue; the other six resolve through veneers.
 */

extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern u32 Func_080000f8(void);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, s32 *);
extern void Func_0808a010(s32);

void Func_02003948(void)
{
    s32 extra[4];
    s32 group;
    s32 baseZ = (s32)0xfffe0000;

    Func_08009180(76, 61, 74, 38, 1, 1);

    extra[1] = 5;
    extra[2] = 0x8000;
    extra[3] = 0x8000;

    for (group = 0; group <= 2; group++) {
        s32 member;

        for (member = 1; member <= 7; member++) {
            if ((member & 1) != 0) {
                if ((member & 2) != 0) {
                    s32 sample = (s32)((Func_080000f8() * 5) >> 16);
                    s32 x = (105 - sample) << 16;
                    s32 z = baseZ - group * 0x80000 + 0x022e0000;

                    Func_0200013c(x, 0, z, 0, 0, -0x4000,
                                  0x90000, extra);
                } else {
                    s32 sample = (s32)((Func_080000f8() * 5) >> 16);
                    s32 x = ((group * 4 + member) << 17) + (183 << 16);
                    s32 z = (620 - sample) << 16;

                    Func_0200013c(x, 0, z, 0x4000, 0, 0,
                                  0x90000, extra);
                }

                Func_0808a010(1);
            }

            baseZ -= 0x20000;
        }

        Func_08009180(71, 59, 70, 34 - group, 1, 1);
        Func_08009180(71, 59, 75 + group, 38, 1, 1);
    }
}
