/*
 * Overlay resource_37c: in-image table getters and the scene entry that posts
 * the setup request and sets the view scale.
 */

#include "TYPES.H"
#include "SCENE.H"

/* The scene's four tables, in the overlay's read-only data. */
extern u8 SoruDou_SceneTable0[];
extern u8 SoruDou_SceneTable1[];
extern u8 SoruDou_SceneTable2[];
extern u8 SoruDou_SceneTable3[];
#include "RESOURCE_37C_RUNTIME.H"

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *State_Run(void)
{
    return SoruDou_SceneTable0;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 state_update_0432(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetPrimaryTable37c(void)
{
    return SoruDou_SceneTable1;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetSecondaryTable37c(void)
{
    return SoruDou_SceneTable2;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTertiaryTable37c(void)
{
    return SoruDou_SceneTable3;
}

s32 Resource37c_Run(void)
{
    /*
     * The three 16.16 scale values must stay separate locals built in the
     * entry block; sharing them turns two of the three into register copies.
     */
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
