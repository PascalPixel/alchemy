
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"

#include "resource_37c_runtime.h"

s32 Engine_GameFlagIsSet(s32);

void Engine_WorkSetValuesIfNonNegative(s32, s32, s32);

void Engine_GameFlagSet(s32);

u8 *State_Run(void)
{
    return SoruDou_SceneTable0;
}

s32 SoruDou_GetView(void)
{
    return 0;
}

u8 *SceneData_GetPrimaryTable37c(void)
{
    return SoruDou_SceneTable1;
}

u8 *SceneData_GetSecondaryTable37c(void)
{
    return SoruDou_SceneTable2;
}

u8 *SceneData_GetTertiaryTable37c(void)
{
    return SoruDou_SceneTable3;
}

s32 Resource37c_Run(void)
{

    s32 scale_x = 0x10000;
    s32 scale_y = 0x10000;
    s32 scale_z = 0x10000;

    RESOURCE37C_RUNTIME->setup_request_1c0 = 0x204;
    Engine_GameFlagSet(0x144);
    if (Engine_GameFlagIsSet(0x814) != 0) {
        Resource37c_QueueSoundCue(0x8D);
        Engine_WorkSetValuesIfNonNegative(scale_x, scale_y, scale_z);
        Resource37c_FinalizeSetup();
    }
    return 0;
}
