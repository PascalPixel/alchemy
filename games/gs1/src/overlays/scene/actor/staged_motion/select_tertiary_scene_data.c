#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_motion/select_tertiary_scene_data.h"

extern u8 gCell[];
extern s32 gOv[];
extern s32 gOv2[];
extern s32 gOv3[];
extern s32 gOv4[];

s32 MapStagedScene_SelectTertiaryData(void)
{
    u8 *scene_state = gCell;
    if (*(s16 *)(scene_state + 0x1c2) == 16)
        return (s32)gOv;
    if (Actor_Check(0x87a) != 0)
        return (s32)gOv2;
    if (Actor_Check2(0x815) != 0)
        return (s32)gOv3;
    return (s32)gOv4;
}
