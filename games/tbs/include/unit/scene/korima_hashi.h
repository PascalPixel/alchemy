#ifndef ALCHEMY_UNIT_SCENE_KORIMA_HASHI_H
#define ALCHEMY_UNIT_SCENE_KORIMA_HASHI_H

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
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 KorimaHashi_GeraldAction[];
extern s32 KorimaHashi_IvanAction[];
extern s32 KorimaHashi_MiaAction[];
extern s32 KorimaHashi_PartyAction[];
extern s32 KorimaHashi_ResetAction[];
extern s32 KorimaHashi_FormationAction[];
extern s32 KorimaHashi_FinishAction[];
extern s32 KorimaHashi_EntryAction[];
extern struct SceneEntrance KorimaHashi_Entrances[];
extern struct SceneEntrance KorimaHashi_Exits[];
extern struct SceneEntrance KorimaHashi_Placements[];
extern struct SceneEvent KorimaHashi_Events[];
extern s32 KorimaHashi_Object26Script[];
extern s32 KorimaHashi_TriggerScript[];
extern s32 gFallingEffectScript[];

#endif
