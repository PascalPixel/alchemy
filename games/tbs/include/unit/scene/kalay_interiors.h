#ifndef ALCHEMY_UNIT_SCENE_KALAY_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_KALAY_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *KalayInteriors_GetEntrances(void);
const struct SceneRegion *KalayInteriors_GetRegions(void);
u8 *KalayInteriors_GetExits(void);
const struct ScenePlacement *KalayInteriors_GetPlacements(void);
const struct SceneEvent *KalayInteriors_GetEvents(void);
s32 KalayInteriors_Initialize(void);
extern struct SceneEntrance gKalayInteriors_NoEntrances[];
extern struct SceneEntrance gKalayInteriors_HouseEntrances[];
extern struct SceneEntrance gKalayInteriors_SanctumEntrances[];
extern u32 gKalayInteriors_Exits[];
extern struct ScenePlacement gKalayInteriors_NoPlacements[];
extern struct ScenePlacement gKalayInteriors_HousePlacements[];
extern struct ScenePlacement gKalayInteriors_InnPlacements[];
extern struct ScenePlacement gKalayInteriors_SanctumPlacements[];
extern struct SceneEvent gKalayInteriors_NoEvents[];
extern struct SceneEvent gKalayInteriors_HouseEvents[];
extern struct SceneEvent gKalayInteriors_InnEvents[];
extern struct SceneEvent gKalayInteriors_SanctumEvents[];
extern u16 gKalayInteriors_DoorCellSteps[];
extern struct MapCellAnim gKalayInteriors_DoorCells[];

#endif
