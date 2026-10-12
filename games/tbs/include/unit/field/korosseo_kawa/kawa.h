#ifndef ALCHEMY_UNIT_FIELD_KOROSSEO_KAWA_KAWA_H
#define ALCHEMY_UNIT_FIELD_KOROSSEO_KAWA_KAWA_H

#include "types.h"
#include "field_scene.h"

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

u8 *KorosseoKawa_GetEntrances(void);
s32 get_default_result(void);
u8 *KorosseoKawa_GetExits(void);
u8 *KorosseoKawa_GetPlacements(void);
u8 *KorosseoKawa_GetEvents(void);
s32 KorosseoKawa_ApplyEntryState(void);
/* FieldScene_RunMiddleSequence is called before its definition with no prototype in scope. */
extern const u8 Korosseo_PortraitPaletteOffsets[];
extern const s16 KorosseoKawa_ModeRecordTwo[];
extern const s16 KorosseoKawa_SpanB[];
extern char HexDigits[20];
extern s32 KorosseoKawa_ScriptA[];
extern s32 KorosseoKawa_ScriptB[];
extern u32 KorosseoKawa_DirectionSteps[];
extern struct SceneEntrance KorosseoKawa_SceneTableA[];
extern u32 KorosseoKawa_SceneTableB[];
extern struct ScenePlacement KorosseoKawa_SceneTableC[];
extern s32 KorosseoKawa_Countdown;
extern struct SceneEvent KorosseoKawa_SceneTableD[];
extern s16 Korosseo_PortraitSlot;
extern s16 KorosseoKawa_RoundSpans[];
extern s16 KorosseoKawa_ModeRecordFour[];
extern s16 KorosseoKawa_SpanA[];
extern s16 Korosseo_MarkerSlot;
extern s32 Korosseo_RivalFinishScript[];
extern s32 KorosseoKawa_ApproachScript[];

#endif
