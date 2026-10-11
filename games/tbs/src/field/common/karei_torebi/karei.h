#ifndef KAREI_H
#define KAREI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

#include "types.h"

#include "select_overlay_data_by_runtime_selector.h"

#include "types.h"

#include "types.h"

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

#include "types.h"

s32	 PartyInventory_FindOwner();
s32	 Inventory_Find();
void Inventory_Discard();
void Engine_EventShowMessage();

#include "types.h"

void Engine_EventBegin(void);

#include "types.h"

#include "types.h"

#include "battle_effect_runtime.h"
#include "window.h"
#define UiWindow_Create ((s32 (*)())UiWindow_Create)

void SceneState_ApplyValues14And0And5(void);
s32	 SceneData_ReturnZero(void);

extern u8 KareiTorebi_SceneTable[];
u8		 *KareiTorebi_GetExits(void);
void	  KareiTorebi_MoveGuardAside(void);
void	  KareiTorebi_AskGoToTolbi(void);
void	  KareiTorebi_AskCarryMessage(void);
void	  KareiTorebi_RemindBoardShip(void);
void	  SceneDialogue_RunActor12Event(void);
void	  FieldScene_RunActorThirteenFlagDialogue(void);
void	  FieldScene_RunOpeningAuxiliarySequence(void);
void	  KareiTorebi_TalkScaryTrip(void);
void	  KareiTorebi_TalkSeasickTourists(void);
void	  KareiTorebi_DeliverParentsMessage(void);
void	  KareiTorebi_PreparePort(void);
void	  KareiTorebi_PrepareWindowTransition(void);
void	  FieldScene_PlaceSlots14And15(void);
void	  SceneActor_SetMode3AndFlagBit1(s32 no);
void	  FieldScene_RunScene3aeSequenceA(void);
void	  FieldScene_RunScene3aeSequenceB(void);

#endif
