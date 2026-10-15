#include "lemurian_ship.h"

extern u32 gData_02003380[];
extern u32 gData_02003398[];
extern u32 gData_0200b410[];
extern u32 gData_02003560[];
extern u32 gData_02003590[];
extern u32 gData_02003710[];
extern u32 gData_020037d0[];
extern u32 gData_02003848[];

u32 *Func_020000b8(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov656Room1) {
        switch (gPartyState.entrance) {
        case 3:
        case 4:
        case 5:
            return gData_02003398;
        case 25:
        case 26:
        case 27:
        case 28:
            return gData_0200b410;
        case 15:
        case 16:
        case 17:
        case 18:
            return gData_02003560;
        case 29:
        case 30:
            return gData_02003590;
        }
    } else if (gPartyState.scene == (s32)&SceneId_Ov656Room2) {
        switch (gPartyState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 20:
            return gData_02003710;
        case 6:
        case 10:
            return gData_020037d0;
        default:
            return gData_02003848;
        }
    }
    return gData_02003380;
}
