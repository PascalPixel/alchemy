#include "types.h"

extern s16 Data_02001000;
extern void Func_02006fe8(s32 arg0);

void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02006fe8(1);
    }
}
