#ifndef ALCHEMY_UNIT_FIELD_SHIAN_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_SHIAN_HEYA_HEYA_H

#include "types.h"
#include "field_scene.h"

s32 ShianHeya_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 ShianHeya_GetExits(void);
s32 ShianHeya_SelectPlacements(void);
s32 ShianHeya_SelectEvents(void);
s32 ShianHeya_PrepareWindowTransition(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance ShianHeya_SceneTable0[];
extern u32 ShianHeya_SceneTable1[];
extern struct ScenePlacement ShianHeya_SceneTable2[];
extern struct ScenePlacement ShianHeya_SceneTable2Entrance8[];
extern struct SceneEvent ShianHeya_SceneTable3[];
extern struct SceneEvent ShianHeya_SceneTable3Entrance8[];

#endif
