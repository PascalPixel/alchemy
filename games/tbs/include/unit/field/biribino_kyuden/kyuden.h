#ifndef ALCHEMY_UNIT_FIELD_BIRIBINO_KYUDEN_KYUDEN_H
#define ALCHEMY_UNIT_FIELD_BIRIBINO_KYUDEN_KYUDEN_H

#include "types.h"
#include "field_scene.h"

struct SceneEvent;
struct ScenePlacement;

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1);
extern s32 Kyuden_FacingActions[];
extern s32 Kyuden_PacingActions[];
extern struct SceneEntrance Placement_Scripts[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoKyudenPlacementsOther[];
extern struct ScenePlacement gBiribinoKyudenPlacements[];
extern struct SceneEvent gBiribinoKyudenEventsOther[];
extern struct SceneEvent gBiribinoKyudenEvents[];

#endif
