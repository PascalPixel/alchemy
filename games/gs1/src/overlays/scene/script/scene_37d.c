/*
 * Overlay resource_37d: in-image table getters and the scene entry that
 * posts the scene request and sets the camera zoom.
 */

#include "types.h"
#include "scene.h"

#include "resource_37d.h"
#include "resource_37de_scene.h"

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    return RESOURCE37D_PRIMARY_TABLE;   /* image offset 0xc0 */
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetMessageTable(void)
{
    return RESOURCE37D_SECONDARY_TABLE;   /* image offset 0x120 */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetActorTable(void)
{
    return RESOURCE37D_TERTIARY_TABLE;   /* image offset 0x130 */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    return RESOURCE37D_QUATERNARY_TABLE;   /* image offset 0x148 */
}

s32 Scene_InitSceneRequestAndCameraZoom(void)
{
    struct Resource37deSceneRuntime *runtime;
    /*
     * The three scale values must stay locals rather than literals at the
     * call, so each one is built in the entry block. Folded back into the
     * argument list they collapse into one built value and two copies.
     */
    s32 zoom_x = 0x10000;
    s32 zoom_y = 0x10000;
    s32 zoom_z = 0x10000;

    runtime = RESOURCE37DE_SCENE_RUNTIME;
    runtime->scene_request_1c0 = 0x204;
    runtime->scene_setup_word_1c8 = 0x10;
    if (TestSceneFlag(0x814) != 0) {
        QueueSceneSound(0x8D);
        SetCameraZoom(zoom_x, zoom_y, zoom_z);
        InitializeSceneRecordBuffer();
    }
    return 0;
}
