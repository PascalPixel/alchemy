#include "types.h"

extern s16 Data_0200cb46;
extern s32 Func_02006d3e(void);

void Func_02002e64(void)
{
    s16 *cursor = &Data_0200cb46;

    if (*cursor == -1) {
        *cursor = Func_02006d3e();
    }
}
