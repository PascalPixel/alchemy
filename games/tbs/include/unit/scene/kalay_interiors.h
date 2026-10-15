#ifndef ALCHEMY_UNIT_SCENE_KALAY_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_KALAY_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *KalayInteriors_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern struct SceneEntrance  gKalayInteriorsEntrancesOther[];
extern struct SceneEntrance  gKalayInteriorsEntrances1[];
extern struct SceneEntrance  gKalayInteriorsEntrances2[];
extern u32                   gKalayInteriors_Exits[];
extern struct ScenePlacement gKalayInteriorsPlacementsOther[];
extern struct ScenePlacement gKalayInteriorsPlacements1[];
extern struct ScenePlacement gKalayInteriorsPlacements1Entrance9[];
extern struct ScenePlacement gKalayInteriorsPlacements2[];
extern struct SceneEvent     gKalayInteriorsEventsOther[];
extern struct SceneEvent     gKalayInteriorsEvents1[];
extern struct SceneEvent     gKalayInteriorsEvents1Entrance9[];
extern struct SceneEvent     gKalayInteriorsEvents2[];
extern u16                   gKalayInteriors_ArrivalPlacementData[];
extern struct MapCellAnim    gKalayInteriors_ArrivalPlacements[];

#endif
