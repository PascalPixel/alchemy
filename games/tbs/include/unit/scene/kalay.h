#ifndef ALCHEMY_UNIT_SCENE_KALAY_H
#define ALCHEMY_UNIT_SCENE_KALAY_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

typedef struct RenderData
{
	unsigned char pad00[0x1E];
	s16			  rotation;
} RenderData;

typedef struct LobeOrbitEffect
{
	unsigned char pad00[8];
	s32			  x;
	s32			  y;
	unsigned char pad10[0x20];
	s32			  angle;
	unsigned char pad34[4];
	s32			  baseX;
	s32			  baseY;
	unsigned char pad40[0x10];
	RenderData	 *render;
} LobeOrbitEffect;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *Kalay_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gKalay_Data01[];
extern s32 gSceneAction_GroupMotion[];
extern s32 gSceneAction_GroupOffsetMotion[];
extern s32 gKalay_Script01[];
extern s32 gKalay_DanceScriptA[];
extern s32 gKalay_DanceScriptB[];
extern s32 gKalay_DanceScriptC[];
extern s32 gKalay_DanceScriptD[];
extern s32 gKalay_DanceScriptE[];
extern s32 gKalay_DanceScriptF[];
extern s32 gKalay_Data02[];
extern s32 gKalay_Data03[];
extern s32 gKalay_ActionScript01[];
extern struct SceneEntrance gKalayEntrancesOther[];
extern struct SceneEntrance gKalayEntrances1[];
extern struct SceneEntrance gKalayEntrances2[];
extern struct SceneEntrance gKalayEntrances3[];
extern struct SceneEntrance gKalayEntrances4[];
extern struct SceneEntrance gKalayEntrances5[];
extern struct SceneEntrance gKalayEntrances6[];
extern u32 gKalay_SceneTable01[];
extern struct ScenePlacement gKalayPlacementsOther[];
extern struct ScenePlacement gKalayPlacements1[];
extern struct ScenePlacement gKalay_StagedGroupScript[];
extern struct ScenePlacement gKalayPlacements2[];
extern struct ScenePlacement gKalayPlacements3[];
extern struct ScenePlacement gKalayPlacements6[];
extern struct SceneEvent gKalayEventsOther[];
extern struct SceneEvent gKalayEvents1[];
extern struct SceneEvent gKalayEvents2[];
extern struct SceneEvent gKalayEvents3[];
extern struct SceneEvent gKalayEvents4[];
extern struct SceneEvent gKalayEvents5[];
extern struct SceneEvent gKalayEvents6[];
extern u16 gKalay_DoorCellData[];
extern struct MapCellAnim gKalay_DoorCells[];
extern s32 gKalay_Script02[];

#endif
