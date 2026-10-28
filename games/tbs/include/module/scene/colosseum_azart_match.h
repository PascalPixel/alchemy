#ifndef ALCHEMY_MODULE_MAP_COLOSSEUM_AZART_MATCH_H
#define ALCHEMY_MODULE_MAP_COLOSSEUM_AZART_MATCH_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *ColosseumAzartMatch_GetEntrances(void);
const struct SceneExitRegion *ColosseumAzartMatch_GetRegions(void);
const u32 *ColosseumAzartMatch_GetExits(void);
const struct ActorPlacement *ColosseumAzartMatch_GetActorPlacements(void);
const struct Trigger *ColosseumAzartMatch_GetTriggers(void);
s32 ColosseumAzartMatch_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosseumAzartMatch_LetteringDigit[];
extern const s16 gColosseumAzartMatch_LetteringLose[];
extern char gHexDigits[20];
extern s32 gColosseumAzartMatch_ScriptA[];
extern s32 gColosseumAzartMatch_ScriptB[];
extern u32 gColosseumAzartMatch_DirectionSteps[];
extern struct SceneEntrance gColosseumAzartMatch_Entrances[];
extern u32 gColosseumAzartMatch_Exits[];
extern struct ActorPlacement gColosseumAzartMatch_ActorPlacements[];
extern s32 gColosseumAzartMatch_Countdown;
extern struct Trigger gColosseumAzartMatch_Triggers[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosseumAzartMatch_LetteringTitle[];
extern s16 gColosseumAzartMatch_LetteringGo[];
extern s16 gColosseumAzartMatch_LetteringWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumAzartMatch_ApproachScript[];

#endif
