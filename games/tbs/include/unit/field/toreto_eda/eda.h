#ifndef ALCHEMY_UNIT_FIELD_TORETO_EDA_EDA_H
#define ALCHEMY_UNIT_FIELD_TORETO_EDA_EDA_H

#include "types.h"

void State_SetActorEightValue3d(void);
u8 *ToretoEda_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ToretoEda_GetExits(void);
u8 *ToretoEda_GetPlacements(void);
void Actor_ShiftObjectsByBlock(s32 bx, s32 bz);
void Scene_ApplyOffset0Pos5(void);
void Scene_ApplyOffset0Neg5(void);
void Scene_ApplyOffset0Pos5Second(void);
void Scene_ApplyOffset0Neg5Second(void);
void Scene_ApplyOffset0Pos6(void);
void Scene_ApplyOffset0Neg6(void);
void ToretoEda_PlayCueAndSelectTouchedExit(void);
void Effect_SetAlphaBlendForScene9(void);
u8 *ToretoEda_GetEvents(void);
void Scene_RunTwoCallSequence(void);
void State_RunEmptyHookFirst(void);
void State_RunEmptyHook(void);
void ToretoEda_ExchangeInputPacket(void);
s32 ToretoEda_InitializeBackdropTransition(void);
void Effect_UpdateBg3HofsByVcount(void);
void Effect_SetBg3HofsSplit(void);
void ToretoEda_StartBg3Split(void);

#endif
