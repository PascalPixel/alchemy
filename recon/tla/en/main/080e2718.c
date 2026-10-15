/* Near miss of Func_080e2718 (listing 42 instructions, draft 41, 15 differ): the native keeps the
 * work base in r1 and the count address in r2 until the first test, copies it to r7 after, and
 * counts before the second stack argument; a struct with an indexed array gives 37 differ. */
#include "types.h"

extern u8 gData_02001000[];
void Func_080201f0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u8 *Func_080dbb78(s32 x, s32 z, s32 mode);

void Func_080e2718(void)
{
    u8 *base = gData_02001000;
    s8 *count = (s8 *)(base + 0x194c);
    s32 i = 0;

    if (i < *count) {
        u8 *entry = base + 0x174c;

        do {
            Func_080201f0(entry[0], entry[1], 1, 1, entry[2], entry[3]);
            i++;
            Func_080dbb78(entry[4] << 20, entry[5] << 20, 2)[2] = 0;
            entry += 8;
        } while (i < *count);
    }
}
