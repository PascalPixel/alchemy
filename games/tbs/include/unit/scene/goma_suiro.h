#ifndef ALCHEMY_UNIT_SCENE_GOMA_SUIRO_H
#define ALCHEMY_UNIT_SCENE_GOMA_SUIRO_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

typedef struct
{
	u8	pad_to_angle[30];
	u16 angle;
} EffectRecord_020004c4;

typedef struct
{
	u8					   pad_to_record[80];
	EffectRecord_020004c4 *record;
} EffectWork_020004c4;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_PlaceActor8OnEntry(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gGomaSuiroEntrancesOther[];
extern struct SceneEntrance gGomaSuiroEntrances2[];
extern u32 GomaSuiro_Exits[];
extern struct ScenePlacement gGomaSuiroPlacementsOther[];
extern struct ScenePlacement gGomaSuiroPlacements2[];
extern struct SceneEvent gGomaSuiroEventsOther[];
extern struct SceneEvent gGomaSuiroEvents2[];

#endif
