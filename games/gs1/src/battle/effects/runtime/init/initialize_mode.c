#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/init/initialize_mode.h"

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

void BattleFx_RenderAnimationMode0(s32 arg0);

void BattleFx_InitializeMode(s32 *arg0)
{
    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    if (*arg0 == 0) {
        Battle_Do(arg0);
    } else {
        switch (*arg0) {
        case 1:
            Battle_Do2(arg0);
            break;
        case 2:
            Battle_Do3(arg0);
            break;
        case 3:
            Battle_Do4(arg0);
            break;
        case 4:
            Battle_Do5(arg0);
            break;
        case 5:
            Battle_Do6(arg0);
            break;
        case 6:
            Battle_Do7(arg0);
            break;
        case 7:
            BattleFx_RenderAnimationMode0(arg0);
            break;
        case 8:
            Battle_Do8(arg0);
            break;
        case 9:
            Battle_Do9(arg0);
            break;
        case 10:
            Battle_Do10(arg0);
            break;
        case 11:
            Battle_Do(arg0);
            break;
        case 12:
            Battle_Do11(arg0);
            break;
        }
    }

    Battle_Do12(40);
    Battle_Do12(39);
    Battle_Do12(41);
}
