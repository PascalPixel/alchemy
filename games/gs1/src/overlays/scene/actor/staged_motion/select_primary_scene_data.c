#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_motion/select_primary_scene_data.h"
extern s32 gOv[];
s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)gOv;
}
