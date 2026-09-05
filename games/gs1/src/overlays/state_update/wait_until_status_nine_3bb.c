#include "types.h"

#define SceneState_WaitUntilStatusNine Func_02003238

extern s16 Data_02001000;
extern void Func_02006fe8(s32 arg0);

void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02006fe8(1);
    }
}
