#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_BEND_H

#include "types.h"
#include "field_scene.h"

u8 *KolimaForestBend_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KolimaForestBend_GetExits(void);
u8 *KolimaForestBend_GetPlacements(void);
u8 *KolimaForestBend_GetEvents(void);
s32 KolimaForestBend_SetupBoard(void);
/* State_StampRecordCells is called before its definition with no prototype in scope. */
extern u32                   gKolimaForestBend_DefaultRecords[];
extern u8                    gKolimaForestBend_PushAnimations[];
extern s8                    gKolimaForestBend_PushStepX[];
extern s8                    gKolimaForestBend_PushStepZ[];
extern struct SceneEntrance  gKolimaForestBendEntrances[];
extern u32                   gKolimaForestBendExits[];
extern struct ScenePlacement gKolimaForestBendPlacements[];
extern struct SceneEvent     gKolimaForestBendEvents[];

#endif
