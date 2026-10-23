#ifndef ALCHEMY_UNIT_SCENE_BABI_PALACE_H
#define ALCHEMY_UNIT_SCENE_BABI_PALACE_H

#include "types.h"
#include "map_tables.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *BabiPalace_GetRegions(void);
const u32 *BabiPalace_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct SceneEntrance gBabiPalace_Entrances[];
extern struct SceneEntrance gBabiPalace_OutsideEntrances[];
extern struct SceneRegion gBabiPalace_Regions[];
extern u32 gBabiPalace_Exits[];
extern struct ScenePlacement gBabiPalace_OutsidePlacements[];
extern struct ScenePlacement gBabiPalace_PlacementsBabiMissing[];
extern struct ScenePlacement gBabiPalace_PlacementsFinals[];
extern struct ScenePlacement gBabiPalace_PlacementsAfterColosso[];
extern struct SceneEvent gBabiPalace_EventsBabiMissing[];
extern struct SceneEvent gBabiPalace_EventsFinals[];
extern struct SceneEvent gBabiPalace_EventsAfterColosso[];
extern struct SceneEvent gBabiPalace_OutsideEventsBabiMissing[];
extern struct SceneEvent gBabiPalace_OutsideEventsFinals[];
extern struct SceneEvent gBabiPalace_OutsideEventsAfterColosso[];

#endif
