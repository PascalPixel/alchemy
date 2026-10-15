#ifndef ALCHEMY_UNIT_SCENE_GOMA_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_GOMA_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

void *GomaEntrance_GetEntrances(void);
int SceneData_ReturnZero(void);
void *GomaEntrance_GetExits(void);
void *GomaEntrance_GetPlacements(void);
void *GomaEntrance_GetEvents(void);
s32 GomaEntrance_RestoreEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gGomaEntrance_SceneTableCData[];
extern struct SceneEntrance gGomaEntrance_SceneTableA[];
extern u32 gGomaEntrance_SceneTableB[];
extern struct ScenePlacement gGomaEntrance_SceneTableC[];
extern struct SceneEvent gGomaEntrance_SceneTableD[];

#endif
