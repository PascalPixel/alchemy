/* Near miss, differ 17: the listing builds the mask 3 inside the loop in the
 * register it ANDs into (movs r3, #3; ands r3, r2) and keeps the table in r4;
 * this draft hoists the 3 into r4 and pushes r5 for the table. A descending
 * and an ascending loop give the same code. */
#include "types.h"
#include "frame.h"

extern const u16 Data_02001ae0[];

void Func_02000738(void)
{
    u32 phase = gFrameCount >> 4;
    u16 *color = (u16 *)0x05000018;
    s32 i;

    for (i = 3; i >= 0; i--) {
        *color++ = Data_02001ae0[phase & 3];
        phase++;
    }
}
