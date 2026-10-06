#include "TYPES.H"
#include "FIELD_SCENE.H"

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
    return Field_SceneTable[effect_index].group;
}

s16 Field_GetSceneAttributes(s32 scene)
{
    return Field_SceneTable[scene].attributes;
}
