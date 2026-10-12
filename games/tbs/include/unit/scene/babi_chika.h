#ifndef ALCHEMY_UNIT_SCENE_BABI_CHIKA_H
#define ALCHEMY_UNIT_SCENE_BABI_CHIKA_H

#include "types.h"
#include "field_scene.h"
#include "fieldcoo.h"

struct SceneEntrance;
struct SceneRegion;

typedef struct Target_02000cd0
{
	union FieldCoordinate x;
	s32					  pad4;
	union FieldCoordinate z;
} Target_02000cd0;

typedef struct Record
{
	s32 pad0;
	s32 pad4;
	s32 w8;
	s32 w12;
	s32 w16;
} Record;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *BabiChika_GetExits(void);
u8 *SceneData_SelectAndApplyTableBySceneId(void);
s32 BabiChika_SelectEvents(void);
s32 FieldScene_InitializeActorGroups(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 Data_0200b350[];
extern const s32 *gEffectScripts[];
extern s32 Data_0200b8f4_1[];
extern s32 Data_0200b8f4_0[];
extern s32 BabiChika_FlickerScript[];
extern u16 Data_0200b3ec[];
extern u16 Data_0200b40c[];
extern struct SceneEntrance gBabiChikaEntrancesOther[];
extern struct SceneEntrance gBabiChikaEntrances1[];
extern struct SceneEntrance gBabiChikaEntrances2[];
extern s16 gBabiChikaRegions2[];
extern u32 Data_0200b85c[];
extern struct ScenePlacement Data_0200b8f4[];
extern u32 Data_0200ba74[];
extern struct SceneEvent Data_0200bc0c[];
extern struct SceneEvent Data_0200bef4[];

#endif
