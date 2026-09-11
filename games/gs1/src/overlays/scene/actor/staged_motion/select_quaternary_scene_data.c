#include "types.h"
#include "scene.h"

extern s32 gOv[];
extern s32 gOv2[];
extern s32 gOv3[];

s32 MapStagedScene_SelectQuaternaryData(void)
{
    if (Actor_Check(0x87a) != 0)
        return (s32)gOv;
    if (Actor_unk2(0x815) != 0)
        return (s32)gOv2;
    return (s32)gOv3;
}
