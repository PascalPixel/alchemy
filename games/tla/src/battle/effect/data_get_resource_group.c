#include "types.h"
#include "field_scene.h"

s8 BattleEffect_GetResourceGroup(s32 effectIndex) {
  return gField_SceneTable[effectIndex].group;
}

s16 Field_GetSceneAttributes(s32 scene) {
  return gField_SceneTable[scene].attributes;
}
