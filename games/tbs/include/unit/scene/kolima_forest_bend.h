#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *KolimaForestBend_GetEntrances(void);
const struct SceneRegion *KolimaForestBend_GetRegions(void);
const u32 *KolimaForestBend_GetExits(void);
const struct ScenePlacement *KolimaForestBend_GetPlacements(void);
const struct SceneEvent *KolimaForestBend_GetEvents(void);
s32 KolimaForestBend_Initialize(void);
// State_StampRecordCells is called before its definition with no prototype in
// scope.
extern u32 gKolimaForestBend_DefaultRecords[];
extern u8 gKolimaForestBend_PushAnimations[];
extern s8 gKolimaForestBend_PushStepX[];
extern s8 gKolimaForestBend_PushStepZ[];
extern struct SceneEntrance gKolimaForestBendEntrances[];
extern u32 gKolimaForestBendExits[];
extern struct ScenePlacement gKolimaForestBendPlacements[];
extern struct SceneEvent gKolimaForestBendEvents[];

#endif
