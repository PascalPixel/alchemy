#include "types.h"
#include "scene.h"

/* overlays/scene/actor/staged_motion/select_primary_scene_data.c */
extern s32 gOv[];
s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)gOv;
}

/* overlays/scene/actor/staged_motion/get_empty_scene_data.c */
s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

/* overlays/scene/actor/staged_motion/select_secondary_scene_data.c */
extern s32 gOv[];
s32 MapStagedScene_SelectSecondaryData(void)
{
    return (s32)gOv;
}

/* overlays/scene/actor/staged_motion/select_tertiary_scene_data.c */
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
    if (Actor_unk2(0x815) != 0)
        return (s32)gOv3;
    return (s32)gOv4;
}
