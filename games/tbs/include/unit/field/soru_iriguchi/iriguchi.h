#ifndef ALCHEMY_UNIT_FIELD_SORU_IRIGUCHI_IRIGUCHI_H
#define ALCHEMY_UNIT_FIELD_SORU_IRIGUCHI_IRIGUCHI_H

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
s32 Scene_Initialize(void);
extern struct SceneEntrance gSoruIriguchiEntrancesOther[];
extern struct SceneEntrance gSoruIriguchiEntrances2[];
extern struct SceneEntrance gSoruIriguchiEntrances1[];
extern u32 SoruIriguchi_Exits[];
extern struct ScenePlacement gSoruIriguchiPlacementsOther[];
extern struct ScenePlacement gSoruIriguchiPlacements1[];
extern struct ScenePlacement gSoruIriguchiPlacements1Entrances11To13[];
extern struct ScenePlacement gSoruIriguchiPlacements1Entrances14To16[];
extern struct ScenePlacement gSoruIriguchiPlacements2[];
extern struct SceneEvent gSoruIriguchiEventsOther[];
extern struct SceneEvent gSoruIriguchiEvents2[];
extern struct SceneEvent gSoruIriguchiEvents1[];
extern struct SceneEvent gSoruIriguchiEvents1Entrances11To13[];
extern struct SceneEvent gSoruIriguchiEvents1Entrances14To16[];
extern s32 gSoruPushSteps[];

#endif
