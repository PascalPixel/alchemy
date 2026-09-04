#include "types.h"

#define SceneState_InitHalfwordC6a6Once Func_02002bcc

extern s16 Data_0200c6a6;

extern s16 Func_02006816(void);

void SceneState_InitHalfwordC6a6Once(void)
{
    if (Data_0200c6a6 == -1) {
        Data_0200c6a6 = Func_02006816();
    }
}
