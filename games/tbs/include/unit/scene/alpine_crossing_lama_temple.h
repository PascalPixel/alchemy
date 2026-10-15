#ifndef ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H
#define ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H

#include "types.h"
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
s32 AlpineCrossingLamaTemple_ApplyEntryState(void);
extern s32 AlpineCrossingLamaTemple_TemplePlacement1[];
extern s32 AlpineCrossingLamaTemple_HsuAction[];
extern s32 AlpineCrossingLamaTemple_LeaderAction[];
extern s32 AlpineCrossingLamaTemple_ActorNineAction[];
extern u16 AlpineCrossingLamaTemple_BoulderCells[];
extern u16 AlpineCrossingLamaTemple_BoulderCellsBack[];
extern struct SceneEntrance AlpineCrossingLamaTemple_Entrances[];
extern struct SceneEntrance AlpineCrossingLamaTemple_TempleEntrances[];
extern u32 AlpineCrossingLamaTemple_Exits[];
extern struct ScenePlacement AlpineCrossingLamaTemple_Placements[];
extern struct ScenePlacement AlpineCrossingLamaTemple_TemplePlacements[];
extern struct SceneEvent AlpineCrossingLamaTemple_Events[];
extern struct SceneEvent AlpineCrossingLamaTemple_TempleEvents[];

#endif
