#ifndef ALCHEMY_UNIT_SCENE_SUHARA_HEYA_H
#define ALCHEMY_UNIT_SCENE_SUHARA_HEYA_H

#include "types.h"
#include "field_scene.h"

u8 *SuharaHeya_GetEntrances(void);
u8 *SuharaHeya_GetRegions(void);
u8 *SuharaHeya_GetExits(void);
s32 SuharaHeya_SelectPlacements(void);
s32 SuharaHeya_SelectEvents(void);
s32 Scene_InitActorRecords(void);
extern struct SceneEntrance gSuharaHeyaEntrances[];
extern s16 gSuharaHeyaRegions[];
extern u32 gSuharaHeyaExits[];
extern struct ScenePlacement gSuharaHeyaPlacements[];
extern struct ScenePlacement gSuharaHeyaPlacements96f[];
extern struct SceneEvent gSuharaHeyaEvents[];
extern struct SceneEvent gSuharaHeyaEvents96f[];

#endif
