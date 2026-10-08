#include "types.h"
#include "field_scene.h"

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
    return Field_SceneTable[effect_index].group;
}

s16 Field_GetSceneAttributes(s32 scene)
{
    return Field_SceneTable[scene].attributes;
}
