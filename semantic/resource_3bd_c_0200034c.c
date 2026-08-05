#include "types.h"

/*
 * Locate the world object occupying the square selected by the current
 * direction.  This is the complete owner beginning at 0x0200034c: the
 * high-register save and 24-byte frame run through the single epilogue at
 * 0x02000452..0x02000462.  Its four literal words follow at 0x02000464,
 * and the next owner's prologue is 0x02000474, so the recorded span is
 * 0x0200034c..0x02000473 (296 bytes).
 *
 * The first table is the 58-entry world/object pointer array at workspace
 * +0x34.  The second table is consumed continuously: each of the 58 * 6
 * candidates consumes one key word and one four-word limit record, and those
 * cursors are not rewound at each outer iteration.  That detail is visible in
 * the machine code because both cursors are initialized only once, before the
 * outer loop, and advanced before the inner-loop branch.
 *
 * The eight stores to [sp,#0], [sp,#4], [sp,#8], [sp,#12], [sp,#16] and
 * [sp,#20] are compiler spills.  The four observable stores are expressed
 * below: directionOut, kindIndexOut, and the two parity arms of slotOut.
 */

extern u8 *Data_03001ebc;
extern const s32 Data_0200bef4[];
extern const s32 Data_0200bf0c[];
extern const s32 Data_0200beb4[];

extern u8 *Func_0808a080(s32 id);

static s32 add_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left + (u32)right);
}

u8 *Func_0200034c(s32 *directionOut, s32 *slotOut, s32 *kindIndexOut)
{
    u8 *workspace = Data_03001ebc;
    u8 **worldTable = (u8 **)(workspace + 0x34);
    u8 *record;
    const s32 *keys = Data_0200bef4;
    const s32 *limits = Data_0200bf0c;
    s32 slot;

    record = Func_0808a080(0);
    *directionOut = (*(u16 *)(record + 6)) >> 12;

    for (slot = 8; slot <= 65; slot++, worldTable++) {
        u8 *world = *worldTable;
        u8 *worldInfo = *(u8 **)(world + 0x50);
        s32 worldKey = *(s16 *)(worldInfo + 0x28);
        s32 index;

        for (index = 0; index <= 5; index++, limits += 4) {
            s32 key = *keys++;

            if (worldKey != key) {
                continue;
            }

            *kindIndexOut = index;

            {
                s32 packed;
                s32 recordX;
                s32 recordY;
                s32 baseX;
                s32 baseY;
                s32 probeX;
                s32 probeY;
                s32 minX;
                s32 minY;
                s32 maxX;
                s32 maxY;

                packed = Data_0200beb4[*directionOut];
                recordX = *(s32 *)(record + 8);
                probeX = add_wrapped(recordX >> 16, packed >> 16) >> 4;
                recordY = *(s32 *)(record + 16);
                probeY = add_wrapped(recordY >> 16, (s16)packed) >> 4;
                baseX = *(s16 *)(world + 0x0a);
                baseY = *(s16 *)(world + 0x12);
                minX = add_wrapped(baseX, limits[0]) >> 4;
                minY = add_wrapped(baseY, limits[1]) >> 4;
                maxX = add_wrapped(baseX, limits[2]) >> 4;
                maxY = add_wrapped(baseY, limits[3]) >> 4;

                if (minX > probeX || probeX >= maxX ||
                    minY > probeY || probeY >= maxY) {
                    continue;
                }

                if ((index & 1) != 0) {
                    if (minX == (recordX >> 20)) {
                        continue;
                    }
                } else {
                    if (minY == (recordY >> 20)) {
                        continue;
                    }
                }

                return world;
                *slotOut = slot;
            }
        }
    }

    return 0;
}
