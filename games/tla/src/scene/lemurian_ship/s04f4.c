#include "lemurian_ship.h"

extern s32 Data_020039c4[];
extern s32 Data_020039d0[];
extern s32 Data_02003a84[];
extern s32 Data_02003ae4[];
extern s32 Data_02003c10[];
extern s32 Data_02003c7c[];
extern s32 Data_02003d9c[];
extern s32 Data_02003dfc[];
extern s32 Data_02003ebc[];

s32 *Func_020004f4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov656Room1) {
        switch (gPartyState.entrance) {
        case 3:
        case 4:
        case 5:
            return Data_02003a84;
        case 25:
        case 26:
        case 27:
        case 28:
            return Data_02003ae4;
        case 15:
        case 16:
        case 17:
        case 18:
            return Data_02003c10;
        case 29:
        case 30:
            return Data_02003c7c;
        default:
            return Data_020039d0;
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
            return Data_02003d9c;
        case 6:
        case 10:
            return Data_02003dfc;
        default:
            return Data_02003ebc;
        }
    }
    return Data_020039c4;
}
