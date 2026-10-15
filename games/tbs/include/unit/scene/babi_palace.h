#ifndef ALCHEMY_UNIT_SCENE_BABI_PALACE_H
#define ALCHEMY_UNIT_SCENE_BABI_PALACE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 TorebiKyuden_GetRegions(void);
s32 TorebiKyuden_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TorebiKyuden_ApplyEntryState(void);
extern u16 TorebiKyuden_CellSteps[];
extern s32 TorebiKyuden_MiddleActionScript[];
extern struct SceneEntrance gTorebiKyudenEntrancesOther[];
extern struct SceneEntrance gTorebiKyudenEntrances2[];
extern s16 TorebiKyuden_EmptyTable[];
extern u32 TorebiKyuden_MessageTable[];
extern struct ScenePlacement gTorebiKyudenPlacements2[];
extern struct ScenePlacement gTorebiKyudenPlacementsOther[];
extern struct ScenePlacement gTorebiKyudenPlacementsColosso[];
extern struct ScenePlacement gTorebiKyudenPlacementsAfterColosso[];
extern struct SceneEvent gTorebiKyudenEventsOther[];
extern struct SceneEvent gTorebiKyudenEventsColosso[];
extern struct SceneEvent gTorebiKyudenEventsAfterColosso[];
extern struct SceneEvent gTorebiKyudenEvents2[];
extern struct SceneEvent gTorebiKyudenEvents2Colosso[];

#endif
