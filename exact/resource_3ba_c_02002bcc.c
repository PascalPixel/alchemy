#include "types.h"

extern s16 Data_0200c6a6;

extern s16 Func_02006816(void);

void Func_02002bcc(void)
{
    if (Data_0200c6a6 == -1) {
        Data_0200c6a6 = Func_02006816();
    }
}
