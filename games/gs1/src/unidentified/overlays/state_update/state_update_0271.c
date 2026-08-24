#include "resource_37d.h"
#include "resource_37de_scene.h"

#define Resource37d_Run Func_02000054

s32 Resource37d_Run(void) {
    struct Resource37deSceneRuntime *runtime;
    /* The three scale values are locals, not literals at the call, so that
       their materialisation sits in the entry block instead of the call's.
       Local CSE only unifies identical large constants inside one basic
       block; folding these back into the argument list collapses the three
       movs/lsls pairs to one pair plus two register copies. */
    s32 camera_zoom_x = 0x10000;
    s32 camera_zoom_y = 0x10000;
    s32 camera_zoom_z = 0x10000;

    runtime = RESOURCE37DE_SCENE_RUNTIME;
    runtime->scene_request_1c0 = 0x204;
    runtime->scene_setup_word_1c8 = 0x10;
    if (TestSceneFlag(0x814) != 0) {
        QueueSceneSound(0x8D);
        SetCameraZoom(camera_zoom_x, camera_zoom_y, camera_zoom_z);
        InitializeSceneRecordBuffer();
    }
    return 0;
}
