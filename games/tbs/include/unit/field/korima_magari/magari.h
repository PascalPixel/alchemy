#ifndef ALCHEMY_UNIT_FIELD_KORIMA_MAGARI_MAGARI_H
#define ALCHEMY_UNIT_FIELD_KORIMA_MAGARI_MAGARI_H

#include "types.h"
#include "field_scene.h"

struct IcePlacement;
struct TileRun2;
struct TileRun;

u8 *KorimaMagari_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *KorimaMagari_GetExits(void);
u8 *KorimaMagari_GetPlacements(void);
u8 *KorimaMagari_GetEvents(void);
void KorimaMagari_RedrawBoard(void);
void KorimaMagari_TryJumpForward(void);
void State_ApplyRectByLayoutSelector(void);
void Scene_RepaintBoardRecords(void);
void State_UpdateScrollRegistersWithPreset(void);
void KorimaMagari_CaptureShakeScroll(void);
void Scene_RunKorimaMagariSequence(void);
void KorimaMagari_RunReturnSequence(void);
void KorimaMagari_RunDepartSequence(void);
s32 KorimaMagari_SetupBoard(void);
void KorimaMagari_PlaceObjects(struct IcePlacement *entry);
void State_StampRecordCells(s16 *records, s32 value);
const struct TileRun *SceneData_FindTileRunAt(const struct TileRun *run, s32 x, s32 y);
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode);
void Scene_PushBlockAlongRun(struct TileRun2 *runs);
extern u32 KorimaMagari_DefaultRecords[];
extern u8 KorimaMagari_PushAnimations[];
extern s8 KorimaMagari_PushStepX[];
extern s8 KorimaMagari_PushStepZ[];
extern struct SceneEntrance gKorimaMagariEntrances[];
extern u32 gKorimaMagariExits[];
extern struct ScenePlacement gKorimaMagariPlacements[];
extern struct SceneEvent gKorimaMagariEvents[];

#endif
