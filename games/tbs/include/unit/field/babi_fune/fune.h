#ifndef ALCHEMY_UNIT_FIELD_BABI_FUNE_FUNE_H
#define ALCHEMY_UNIT_FIELD_BABI_FUNE_FUNE_H

#include "types.h"

u8 *BabiFune_GetEntrances(void);
s32 BabiFune_GetDefaultResult(void);
u8 *BabiFune_GetExits(void);
u8 *BabiFune_GetPlacements(void);
void PlayWorkspaceCueAndClearPaletteZero(void);
void BabiFune_LoadTablesAndStopDma(void);
void FieldScene_ConfigureFixedPointValues(void);
u8 *BabiFune_GetEvents(void);
void BabiFune_CyclePalette(void);
void BabiFune_UpdateDriftingObject(u8 *obj);
void SceneState_CountDownEveryFortyTicks(void);
void BabiFune_UpdateWaves(void);
s32 BabiFune_StoreLeaderHeight(void);
s32 BabiFune_StoreActorOneHeight(void);
s32 BabiFune_StoreActorThreeHeight(void);
s32 BabiFune_StoreActorTwoHeight(void);
void Scene_RunExtendedPresentationSequence(void);
void BabiFune_RunDeparture(void);
s32 BabiFune_SetupScene(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
void SceneEffect_LoadTablesAndStopDma0(void);
void BabiFune_StepFade(void);
void BabiFune_ScheduleFade(void);

#endif
