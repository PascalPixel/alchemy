#ifndef ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_SCENE_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KareiTorebi_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 KareiTorebi_LeaveCells1[];
extern u16 KareiTorebi_LeaveCells3[];
extern s32 KareiTorebi_ActorSixteenScript[];
extern struct SceneEntrance gKareiTorebiEntrancesOther[];
extern struct SceneEntrance gKareiTorebiEntrances1[];
extern struct SceneEntrance gKareiTorebiEntrances3[];
extern struct SceneEntrance gKareiTorebiEntrances2[];
extern u32 KareiTorebi_SceneTable[];
extern struct ScenePlacement gKareiTorebiPlacementsOther[];
extern struct ScenePlacement gKareiTorebiPlacements3[];
extern struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern struct ScenePlacement gKareiTorebiPlacements1[];
extern struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern struct ScenePlacement gKareiTorebiPlacements2[];
extern struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern struct SceneEvent gKareiTorebiEventsOther[];
extern struct SceneEvent gKareiTorebiEvents1[];
extern struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern struct SceneEvent gKareiTorebiEvents3[];
extern struct SceneEvent gKareiTorebiEvents3Flag950[];
extern struct SceneEvent gKareiTorebiEvents2[];
extern struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern struct SceneEvent gKareiTorebiEvents2Flag950[];

#endif
