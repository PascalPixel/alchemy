#ifndef ALCHEMY_UNIT_SCENE_GOMA_CAVE_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_GOMA_CAVE_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

void *GomaCaveEntrance_GetEntrances(void);
int SceneData_ReturnZero(void);
void *GomaCaveEntrance_GetExits(void);
void *GomaCaveEntrance_GetPlacements(void);
void *GomaCaveEntrance_GetEvents(void);
s32 GomaCaveEntrance_RestoreEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gGomaCaveEntrance_SceneTableCData[];
extern struct SceneEntrance gGomaCaveEntrance_SceneTableA[];
extern u32 gGomaCaveEntrance_SceneTableB[];
extern struct ScenePlacement gGomaCaveEntrance_SceneTableC[];
extern struct SceneEvent gGomaCaveEntrance_SceneTableD[];

#endif
