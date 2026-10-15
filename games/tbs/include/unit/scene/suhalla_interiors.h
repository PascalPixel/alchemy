#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H

#include "types.h"
#include "field_scene.h"

u8 *SuhallaInteriors_GetEntrances(void);
u8 *SuhallaInteriors_GetRegions(void);
u8 *SuhallaInteriors_GetExits(void);
s32 SuhallaInteriors_SelectPlacements(void);
s32 SuhallaInteriors_SelectEvents(void);
s32 Scene_InitActorRecords(void);
extern struct SceneEntrance gSuhallaInteriorsEntrances[];
extern s16 gSuhallaInteriorsRegions[];
extern u32 gSuhallaInteriorsExits[];
extern struct ScenePlacement gSuhallaInteriorsPlacements[];
extern struct ScenePlacement gSuhallaInteriorsPlacements96f[];
extern struct SceneEvent gSuhallaInteriorsEvents[];
extern struct SceneEvent gSuhallaInteriorsEvents96f[];

#endif
