#ifndef ALCHEMY_UNIT_FIELD_KORIMA_MAGARI_MAGARI_H
#define ALCHEMY_UNIT_FIELD_KORIMA_MAGARI_MAGARI_H

#include "types.h"
#include "field_scene.h"

u8 *KorimaMagari_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KorimaMagari_GetExits(void);
u8 *KorimaMagari_GetPlacements(void);
u8 *KorimaMagari_GetEvents(void);
s32 KorimaMagari_SetupBoard(void);
/* State_StampRecordCells is called before its definition with no prototype in scope. */
extern u32 KorimaMagari_DefaultRecords[];
extern u8 KorimaMagari_PushAnimations[];
extern s8 KorimaMagari_PushStepX[];
extern s8 KorimaMagari_PushStepZ[];
extern struct SceneEntrance gKorimaMagariEntrances[];
extern u32 gKorimaMagariExits[];
extern struct ScenePlacement gKorimaMagariPlacements[];
extern struct SceneEvent gKorimaMagariEvents[];

#endif
