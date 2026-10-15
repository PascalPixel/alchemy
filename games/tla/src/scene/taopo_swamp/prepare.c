#include "local.h"

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Func_02001eb8(void)
{
    s32 frame = -1;

    if (GameFlag_Test(0x348)) {
        frame = 62;
    } else {
        switch (Data_020035f0) {
        case 0:
            frame = 59;
            break;
        case 10:
        case 50:
            frame = 56;
            break;
        case 20:
        case 40:
            frame = 53;
            break;
        case 30:
            frame = 50;
            break;
        }
        if (++Data_020035f0 > 59)
            Data_020035f0 = 0;
    }
    if (frame != -1)
        Func_02002848_(45, frame, 3, 3, 76, 61);
}
