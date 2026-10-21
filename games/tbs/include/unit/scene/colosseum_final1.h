#ifndef ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_1_H
#define ALCHEMY_UNIT_SCENE_COLOSSEUM_FINAL_1_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *ColosseumFinal1_GetEntrances(void);
const struct SceneRegion *ColosseumFinal1_GetRegions(void);
const u32 *ColosseumFinal1_GetExits(void);
const struct ScenePlacement *ColosseumFinal1_GetPlacements(void);
const struct SceneEvent *ColosseumFinal1_GetEvents(void);
s32 ColosseumFinal1_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosseumFinal1_ModeRecordTwo[];
extern const s16 gColosseumFinal1_SpanB[];
extern char gHexDigits[20];
extern s32 gColosseumFinal1_ScriptA[];
extern s32 gColosseumFinal1_ScriptB[];
extern u32 gColosseumFinal1_DirectionSteps[];
extern struct SceneEntrance gColosseumFinal1_Entrances[];
extern u32 gColosseumFinal1_Exits[];
extern struct ScenePlacement gColosseumFinal1_Placements[];
extern s32 gColosseumFinal1_Countdown;
extern struct SceneEvent gColosseumFinal1_Events[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumFinal1_RoundSpans[];
extern s16 gColosseumFinal1_ModeRecordFour[];
extern s16 gColosseumFinal1_SpanA[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_RivalFinishScript[];
extern s32 gColosseumFinal1_ApproachScript[];

#endif
