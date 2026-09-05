#include "types.h"

extern u8 Data_02001004;
extern void Func_02001528(void);

void SceneState_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Func_02001528();
    }
}
