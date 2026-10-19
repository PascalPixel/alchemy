#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_INTERIORS_H

#include "types.h"
#include "field_scene.h"

u8 *Scene_GetEntrances(void);
u8 *Scene_GetRegions(void);
u8 *Scene_GetExits(void);
s32 Scene_GetPlacements(void);
s32 Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSuhallaInteriorsEntrances[];
extern s16 gSuhallaInteriorsRegions[];
extern u32 gSuhallaInteriorsExits[];
extern struct ScenePlacement gSuhallaInteriorsPlacements[];
extern struct ScenePlacement gSuhallaInteriorsPlacementsAfterAudience[];
extern struct SceneEvent gSuhallaInteriorsEvents[];
extern struct SceneEvent gSuhallaInteriorsEventsAfterAudience[];

#endif
