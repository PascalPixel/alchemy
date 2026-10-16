#ifndef ALCHEMY_UNIT_SCENE_ALTIN_H
#define ALCHEMY_UNIT_SCENE_ALTIN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
// Each village door the party can walk through: its opening picture and the
// cell it is drawn at.
struct DoorAnim {
  const u16 *steps;
  s16 x;
  s16 y;
};

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
extern u16 gAltinVillage_MineDoorCells[];
extern u16 gAltinVillage_DoorCellsA[];
extern u16 gAltinVillage_DoorCellsF[];
extern u16 gAltinVillage_DoorCellsG[];
extern u16 gAltinVillage_DoorCellsC[];
extern u16 gAltinVillage_DoorCellsD[];
extern u16 gAltinVillage_DoorCellsE[];
extern struct DoorAnim gAltinVillage_Doors[];

#endif
