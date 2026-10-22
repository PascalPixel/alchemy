#ifndef ALCHEMY_UNIT_SCENE_ALTIN_H
#define ALCHEMY_UNIT_SCENE_ALTIN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gAltinHouses_CookScript[];
extern struct SceneEntrance gAltinOther_Entrances[];
extern struct SceneEntrance gAltinVillage_Entrances[];
extern struct SceneEntrance gAltinHouses_Entrances[];
extern u32 gAltin_Exits[];
extern struct ScenePlacement gAltinOther_Placements[];
extern struct ScenePlacement gAltinVillage_Placements[];
extern struct ScenePlacement gAltinHouses_Placements[];
extern struct SceneEvent gAltinOther_Events[];
extern struct SceneEvent gAltinVillage_Events[];
extern struct SceneEvent gAltinHouses_Events[];
extern u16 gAltinVillage_SanctumDoorCells[];
extern u16 gAltinVillage_SouthwestDoorCells[];
extern u16 gAltinVillage_SoutheastDoorCells[];
extern u16 gAltinVillage_EastDoorCells[];
extern u16 gAltinVillage_NorthwestDoorCells[];
extern u16 gAltinVillage_NorthAndWestDoorCells[];
extern u16 gAltinVillage_NortheastDoorCells[];
extern struct MapCellAnimation gAltinVillage_Doors[];

#endif
