#ifndef ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H
#define ALCHEMY_UNIT_SCENE_ALPINE_CROSSING_LAMA_TEMPLE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
const u32                   *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          AlpineCrossingLamaTemple_ApplyEntryState(void);
extern s32                   gAlpineCrossingLamaTemple_TemplePlacement1[];
extern s32                   gAlpineCrossingLamaTemple_HsuAction[];
extern s32                   gAlpineCrossingLamaTemple_LeaderAction[];
extern s32                   gAlpineCrossingLamaTemple_ActorNineAction[];
extern u16                   gAlpineCrossingLamaTemple_BoulderCells[];
extern u16                   gAlpineCrossingLamaTemple_BoulderCellsBack[];
extern struct SceneEntrance  gAlpineCrossingLamaTemple_Entrances[];
extern struct SceneEntrance  gAlpineCrossingLamaTemple_TempleEntrances[];
extern u32                   gAlpineCrossingLamaTemple_Exits[];
extern struct ScenePlacement gAlpineCrossingLamaTemple_Placements[];
extern struct ScenePlacement gAlpineCrossingLamaTemple_TemplePlacements[];
extern struct SceneEvent     gAlpineCrossingLamaTemple_Events[];
extern struct SceneEvent     gAlpineCrossingLamaTemple_TempleEvents[];

#endif
