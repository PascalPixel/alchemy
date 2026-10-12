#ifndef ALCHEMY_UNIT_SCENE_GOMA_IRIGUCHI_H
#define ALCHEMY_UNIT_SCENE_GOMA_IRIGUCHI_H

#include "types.h"
#include "field_scene.h"

void *GomaIriguchi_GetEntrances(void);
int SceneData_ReturnZero(void);
void *GomaIriguchi_GetExits(void);
void *GomaIriguchi_GetPlacements(void);
void *GomaIriguchi_GetEvents(void);
s32 GomaIriguchi_RestoreEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 GomaIriguchi_SceneTableCData[];
extern struct SceneEntrance GomaIriguchi_SceneTableA[];
extern u32 GomaIriguchi_SceneTableB[];
extern struct ScenePlacement GomaIriguchi_SceneTableC[];
extern struct SceneEvent GomaIriguchi_SceneTableD[];

#endif
