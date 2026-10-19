#ifndef ALCHEMY_UNIT_SCENE_VALE_BABI_H
#define ALCHEMY_UNIT_SCENE_VALE_BABI_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *ValeBabi_GetEntrances(void);
struct SceneRegion *ValeBabi_GetRegions(void);
u32 *ValeBabi_GetExits(void);
struct ScenePlacement *ValeBabi_GetPlacements(void);
struct SceneEvent *ValeBabi_GetEvents(void);
s32 ValeBabi_Initialize(void);
extern s32 gValeBabi_LeaveHouseScript[];
extern s32 gValeBabi_DoraLeavesScript[];
extern s32 gValeBabi_IsaacLeavesScript[];
extern s32 gValeBabi_DoraRunsAheadScript[];
extern s32 gValeBabi_IsaacRunsAheadScript[];
extern s32 gValeBabi_DoraRunsBackScript[];
extern s32 gValeBabi_IsaacRunsBackScript[];
extern s32 gValeBabi_RunToPlazaScript[];
extern s32 gValeBabi_Actor10ShakesScript[];
extern s32 gValeBabi_Actor10FallsScript[];
extern s32 gValeBabi_DoraFacesIsaacScript[];
extern s32 gValeBabi_MythrilBagRisesScript[];
extern s32 gValeBabi_MythrilBagWobblesScript[];
extern struct SceneEntrance gValeBabi_Entrances[];
extern u32 gValeBabi_Exits[];
extern u32 gValeBabi_StormExits[];
extern struct ScenePlacement gValeBabi_Placements[];
extern struct ScenePlacement gValeBabi_PlacementsAfterLeaving[];
extern struct ScenePlacement gValeBabi_PlacementsAfterKalay[];
extern struct ScenePlacement gValeBabi_ScholarsPlacements[];
extern struct SceneEvent gValeBabi_StormEvents[];
extern struct SceneEvent gValeBabi_Events[];
extern struct SceneEvent gValeBabi_EventsAfterLeaving[];
extern struct SceneEvent gValeBabi_EventsAfterKalay[];
extern struct SceneEvent gValeBabi_ScholarsEvents[];
extern struct SceneEvent gValeBabi_ScholarsLaterEvents[];

#endif
