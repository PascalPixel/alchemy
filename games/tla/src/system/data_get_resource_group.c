#include "types.h"
#include "scene_tables.h"

s8 BattleEffect_GetResourceGroup(s32 effectIndex) {
  return gScenes[effectIndex].group;
}

s16 Field_GetSceneAttributes(s32 scene) {
  return gScenes[scene].attributes;
}
