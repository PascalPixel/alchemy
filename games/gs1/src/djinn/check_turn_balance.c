#include "types.h"

void Func_080ae7fc(s8 *);

extern u8 *Data_03001f2c;

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
    u8 *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 difference;
    s32 balanced;

    work = Data_03001f2c;
    Djinn_CountTurns(counts);
    counts[from] -= 1;
    counts[to] += 1;
    balanced = 1;
    for (i = 0; i < work[0x219]; i++) {
        j = i;
        if (i < work[0x219]) {
            while (1) {
                j++;
                if (j >= work[0x219])
                    break;
                difference = counts[i] - counts[j];
                if (difference < -1 || difference > 1) {
                    balanced = 0;
                    break;
                }
            }
        }
    }
    return balanced;
}
