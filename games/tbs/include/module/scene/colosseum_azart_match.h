#ifndef ALCHEMY_MODULE_SCENE_COLOSSEUM_AZART_MATCH_H
#define ALCHEMY_MODULE_SCENE_COLOSSEUM_AZART_MATCH_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosso_LetteringDigit[];
extern const s16 gColosso_LetteringLose[];
extern char gHexDigits[20];
extern s32 gColosso_PrizeSparkleScript[];
extern s32 gColosso_PrizeScript[];
extern u32 gPushedActor_DirectionSteps[];
extern struct SceneEntrance gColosseumAzartMatch_Entrances[];
extern u32 gColosseumAzartMatch_Exits[];
extern struct ActorPlacement gColosseumAzartMatch_ActorPlacements[];
extern s32 gColosseumAzartMatch_Countdown;
extern struct Trigger gColosseumAzartMatch_Triggers[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosso_LetteringTitle[];
extern s16 gColosso_LetteringGo[];
extern s16 gColosso_LetteringWin[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosso_LeaderFinishScript[];

#endif
