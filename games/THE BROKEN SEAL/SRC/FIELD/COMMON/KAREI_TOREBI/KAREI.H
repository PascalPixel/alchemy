#ifndef KAREI_H
#define KAREI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)


void BattleFx_RunPageEffectForSlot(s32, s32, s32);

#include "TYPES.H"

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"


#include "TYPES.H"

#include "TYPES.H"

void UiWork_PushValueSlot();
void UiText_DrawCharacterAtOffset();
void UiText_DrawNumberInWindow();
void UiWork_Finalize();
void UiWork_Finalize();
void UiWork_Finalize();
void Party_AdjustCoins();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void Battle_ResetEffectCounter();

#include "TYPES.H"

s32 PartyInventory_FindOwner();
s32 Inventory_Find();
void Inventory_Discard();
void Engine_EventShowMessage();

#include "TYPES.H"

void Engine_EventBegin(void);

#include "TYPES.H"

#include "TYPES.H"

#include "BATTLE_EFFECT_RUNTIME.H"
#include "WINDOW.H"
#define UiWindow_Create ((s32 (*)())UiWindow_Create)

void SceneState_ApplyValues14And0And5(void);
s32 SceneData_ReturnZero(void);

extern u8 KareiTorebi_SceneTable[];
u8 *KareiTorebi_GetExits(void);
void KareiTorebi_MoveGuardAside(void);
void KareiTorebi_AskGoToTolbi(void);
void KareiTorebi_AskCarryMessage(void);
void KareiTorebi_RemindBoardShip(void);
void SceneDialogue_RunActor12Event(void);
void FieldScene_RunActorThirteenFlagDialogue(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void KareiTorebi_TalkScaryTrip(void);
void KareiTorebi_TalkSeasickTourists(void);
void KareiTorebi_DeliverParentsMessage(void);
void KareiTorebi_PreparePort(void);
void KareiTorebi_PrepareWindowTransition(void);
void FieldScene_PlaceSlots14And15(void);
void SceneActor_SetMode3AndFlagBit1(s32 no);
void FieldScene_RunScene3aeSequenceA(void);
void FieldScene_RunScene3aeSequenceB(void);

#endif
