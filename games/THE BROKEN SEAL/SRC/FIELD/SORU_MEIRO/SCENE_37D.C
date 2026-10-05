

/* Scene table getters and the entry that posts the scene request and sets the camera zoom. */
#include "TYPES.H"
#include "SCENE.H"
extern struct Resource37deSceneRuntime *gEventWork;

/* The scene's four tables, in the overlay's read-only data. */
extern u8 SoruMeiro_SceneTable0[];
extern u8 SoruMeiro_SceneTable1[];
extern u8 SoruMeiro_SceneTable2[];
extern u8 SoruMeiro_SceneTable3[];
#include "RESOURCE_37DE_SCENE.H"

u8 *SceneData_GetScriptTable(void)
{
    return SoruMeiro_SceneTable0;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return SoruMeiro_SceneTable1;
}

u8 *SceneData_GetActorTable(void)
{
    return SoruMeiro_SceneTable2;
}

u8 *SceneData_GetEffectTable(void)
{
    return SoruMeiro_SceneTable3;
}

s32 Scene_InitSceneRequestAndCameraZoom(void)
{
    struct Resource37deSceneRuntime *runtime;

    s32 zoom_x = 0x10000;
    s32 zoom_y = 0x10000;
    s32 zoom_z = 0x10000;

    runtime = gEventWork;
    runtime->scene_request_1c0 = 0x204;
    runtime->scene_setup_word_1c8 = 0x10;
    if (TestSceneFlag(0x814) != 0) {
        QueueSceneSound(0x8D);
        SetCameraZoom(zoom_x, zoom_y, zoom_z);
        InitializeSceneRecordBuffer();
    }
    return 0;
}
