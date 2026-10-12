#ifndef ALCHEMY_UNIT_SCENE_KORASHIAMU_IRIGUCHI_H
#define ALCHEMY_UNIT_SCENE_KORASHIAMU_IRIGUCHI_H

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
extern s32 KorashiamuIriguchi_ActionTable1[];
extern s32 KorashiamuIriguchi_ActionTable2[];
extern s32 KorashiamuIriguchi_ActionTable3[];
extern s32 KorashiamuIriguchi_ActionTable4[];
extern s32 KorashiamuIriguchi_ActionTable5[];
extern s32 KorashiamuIriguchi_ActionTable6[];
extern s32 KorashiamuIriguchi_ActionTable7[];
extern s32 KorashiamuIriguchi_ActionTable8[];
extern s32 KorashiamuIriguchi_ActionTable9[];
extern struct SceneEntrance gKorashiamuIriguchiEntrancesOther[];
extern struct SceneEntrance gKorashiamuIriguchiEntrances1[];
extern struct SceneEntrance gKorashiamuIriguchiEntrances3[];
extern u32 gKorashiamuIriguchiExits[];
extern struct ScenePlacement gKorashiamuIriguchiPlacementsOther[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance5[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance7[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance8[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance12[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance66[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3Flag962[];
extern struct ScenePlacement gKorashiamuIriguchiPlacements3Flag950[];
extern struct SceneEvent gKorashiamuIriguchiEventsOther[];
extern struct SceneEvent gKorashiamuIriguchiEvents2[];
extern struct SceneEvent gKorashiamuIriguchiEvents1[];
extern struct SceneEvent gKorashiamuIriguchiEvents3[];
extern struct SceneEvent gKorashiamuIriguchiEvents1Entrance12[];

#endif
