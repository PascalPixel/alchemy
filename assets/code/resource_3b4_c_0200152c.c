#include "types.h"

extern s16 Data_02000240[];
extern s32 Data_0200af74[];

extern s32 *Func_02003a7a(s32);
extern void Func_02003ac8(s32, s32, s32);
extern void Func_02003b86(s32);
extern void Func_02003ae4(s32, s32, s32);
extern void Func_02003b10(s32, s32, s32);
extern void Func_02003b1e(s32);

void Func_0200152c(s32 which)
{
    s32 *origin = Func_02003a7a(0);
    s32 column = origin[2] / 0x100000;   /* +8  */
    s32 row = origin[4] / 0x100000;      /* +16 */
    s32 slot = which + 10;

    if (Data_02000240[293] == slot) return;
    if (column == Data_0200af74[which]) return;

    Func_02003ac8(slot, 0x48000, 0x24000);
    Func_02003b86(188);
    Func_02003ae4(slot, (column << 4) + 8, 360);

    Data_0200af74[which] = column;

    if (row <= 22) {
        Func_02003b10(0, 0, 8);
    }
    Func_02003b1e(0);
}
