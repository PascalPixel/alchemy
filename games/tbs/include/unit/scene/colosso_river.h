#ifndef ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H
#define ALCHEMY_UNIT_SCENE_COLOSSO_RIVER_H

#include "types.h"
#include "field_scene.h"

typedef struct Position3
{
	s32 x;
	s32 y;
	s32 z;
} Position3;

u8 *ColossoRiver_GetEntrances(void);
s32 get_default_result(void);
u8 *ColossoRiver_GetExits(void);
u8 *ColossoRiver_GetPlacements(void);
u8 *ColossoRiver_GetEvents(void);
s32 ColossoRiver_ApplyEntryState(void);
extern const u8 Colosso_PortraitPaletteOffsets[];
extern const s16 ColossoRiver_ModeRecordTwo[];
extern const s16 ColossoRiver_SpanB[];
extern char HexDigits[20];
extern s32 ColossoRiver_ScriptA[];
extern s32 ColossoRiver_ScriptB[];
extern u32 ColossoRiver_DirectionSteps[];
extern struct SceneEntrance ColossoRiver_SceneTableA[];
extern u32 ColossoRiver_SceneTableB[];
extern struct ScenePlacement ColossoRiver_SceneTableC[];
extern s32 ColossoRiver_Countdown;
extern struct SceneEvent ColossoRiver_SceneTableD[];
extern s16 Colosso_PortraitSlot;
extern s16 ColossoRiver_RoundSpans[];
extern s16 ColossoRiver_ModeRecordFour[];
extern s16 ColossoRiver_SpanA[];
extern s16 Colosso_MarkerSlot;
extern s32 Colosso_RivalFinishScript[];
extern s32 ColossoRiver_ApproachScript[];

#endif
