#ifndef ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H
#define ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H

#include "types.h"
#include "map_tables.h"

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
extern s32 gAlpineCrossing_WorkerPlacementScript[];
extern s32 gAlpineCrossing_FeizhiPath[];
extern s32 gAlpineCrossing_LeaderPath[];
extern s32 gAlpineCrossing_HsuStruggle[];
extern u16 gLamaTemple_DoorOpenCells[];
extern u16 gLamaTemple_DoorCloseCells[];
extern struct SceneEntrance gLamaTemple_Entrances[];
extern struct SceneEntrance gAlpineCrossing_Entrances[];
extern u32 gAlpineCrossingLamaTemple_Exits[];
extern struct ScenePlacement gLamaTemple_Placements[];
extern struct ScenePlacement gAlpineCrossing_Placements[];
extern struct SceneEvent gLamaTemple_Events[];
extern struct SceneEvent gAlpineCrossing_Events[];

#endif
