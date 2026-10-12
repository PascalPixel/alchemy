#ifndef ALCHEMY_UNIT_SCENE_YAMA_RAMA_H
#define ALCHEMY_UNIT_SCENE_YAMA_RAMA_H

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
s32 YamaRama_ApplyEntryState(void);
extern s32 YamaRama_TemplePlacement1[];
extern s32 YamaRama_HsuAction[];
extern s32 YamaRama_LeaderAction[];
extern s32 YamaRama_ActorNineAction[];
extern u16 YamaRama_BoulderCells[];
extern u16 YamaRama_BoulderCellsBack[];
extern struct SceneEntrance YamaRama_Entrances[];
extern struct SceneEntrance YamaRama_TempleEntrances[];
extern u32 YamaRama_Exits[];
extern struct ScenePlacement YamaRama_Placements[];
extern struct ScenePlacement YamaRama_TemplePlacements[];
extern struct SceneEvent YamaRama_Events[];
extern struct SceneEvent YamaRama_TempleEvents[];

#endif
