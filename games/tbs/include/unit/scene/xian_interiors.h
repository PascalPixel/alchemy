#ifndef ALCHEMY_UNIT_SCENE_XIAN_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_XIAN_INTERIORS_H

#include "types.h"
#include "field_scene.h"

s32 XianInteriors_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 XianInteriors_GetExits(void);
s32 XianInteriors_SelectPlacements(void);
s32 XianInteriors_SelectEvents(void);
s32 XianInteriors_PrepareWindowTransition(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance XianInteriors_SceneTable0[];
extern u32 XianInteriors_SceneTable1[];
extern struct ScenePlacement XianInteriors_SceneTable2[];
extern struct ScenePlacement XianInteriors_SceneTable2Entrance8[];
extern struct SceneEvent XianInteriors_SceneTable3[];
extern struct SceneEvent XianInteriors_SceneTable3Entrance8[];

#endif
