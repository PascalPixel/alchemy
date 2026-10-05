
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"

#include "RESOURCE_37C_RUNTIME.H"

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
    Resource37c_SetSetupFlag(0x144);
    if (Resource37c_TestSetupGate(0x814) != 0) {
        Resource37c_QueueSoundCue(0x8D);
        Resource37c_SetViewScale(scale_x, scale_y, scale_z);
        Resource37c_FinalizeSetup();
    }
    return 0;
}
