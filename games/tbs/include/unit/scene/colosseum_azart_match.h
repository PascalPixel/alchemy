#ifndef ALCHEMY_UNIT_MAP_COLOSSEUM_AZART_MATCH_H
#define ALCHEMY_UNIT_MAP_COLOSSEUM_AZART_MATCH_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ColosseumAzartMatch_GetWarpDests(void);
const struct MapWarpRegion *ColosseumAzartMatch_GetRegions(void);
const u32 *ColosseumAzartMatch_GetWarps(void);
const struct ObjectEventTemplate *
ColosseumAzartMatch_GetObjectEventTemplates(void);
const struct MapEvent *ColosseumAzartMatch_GetEvents(void);
s32 ColosseumAzartMatch_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosseumAzartMatch_LetteringDigit[];
extern const s16 gColosseumAzartMatch_LetteringLose[];
extern char gHexDigits[20];
extern s32 gColosseumAzartMatch_ScriptA[];
extern s32 gColosseumAzartMatch_ScriptB[];
extern u32 gColosseumAzartMatch_DirectionSteps[];
extern struct MapWarpDest gColosseumAzartMatch_WarpDests[];
extern u32 gColosseumAzartMatch_Warps[];
extern struct ObjectEventTemplate gColosseumAzartMatch_ObjectEventTemplates[];
extern s32 gColosseumAzartMatch_Countdown;
extern struct MapEvent gColosseumAzartMatch_Events[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumAzartMatch_LetteringTitle[];
extern s16 gColosseumAzartMatch_LetteringGo[];
extern s16 gColosseumAzartMatch_LetteringWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumAzartMatch_ApproachScript[];

#endif
