/* Scene table getters and the entry that posts the setup request and sets the view scale. */

#include "TYPES.H"
#include "SCENE.H"

/* The scene's four tables, in the overlay's read-only data. */
extern u8 SoruDou_SceneTable0[];
extern u8 SoruDou_SceneTable1[];
extern u8 SoruDou_SceneTable2[];
extern u8 SoruDou_SceneTable3[];
#include "RESOURCE_37C_RUNTIME.H"

u8 *State_Run(void)
{
    return SoruDou_SceneTable0;
}

/* Table slot with no data: reads nothing and returns zero. */
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
