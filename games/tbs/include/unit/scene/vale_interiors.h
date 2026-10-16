#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *ValeInteriors_GetEntrances(void);
struct SceneRegion *ValeInteriors_GetRegions(void);
u32 *ValeInteriors_SelectExits(void);
struct ScenePlacement *ValeInteriors_SelectPlacements(void);
struct SceneEvent *ValeInteriors_SelectEvents(void);
s32 ValeInteriors_Initialize(void);
extern s32 gValeFaceTargetScript[];
extern s32 gValeHouseholdPacingScript[];
extern s32 gValePartyDepartureScript[];
extern s32 gValeDriftingSpriteScript[];
extern struct SceneEntrance gValeHouseEntrances[];
extern u32 gValeHouseExits[];
extern u32 gValeHouseLateExits[];
extern struct ScenePlacement gValeHousePlacements[];
extern struct ScenePlacement gValeHousePlacementsAfterLeaving[];
extern struct ScenePlacement gValeHouseReturnPlacements[];
extern struct SceneEvent gValeHouseLateEvents[];

#endif
