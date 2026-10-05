/* Declarations shared by Kalay's Tolbi-road sources (resource_3ae). */
#ifndef KAREI_H
#define KAREI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

extern u8 *Data_03001ebc;

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

#include "TYPES.H"

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

extern s16 Data_02000240[];

#include "TYPES.H"

#include "TYPES.H"

s32 UiWindow_Create();
void UiWork_PushValueSlot();
void UiText_DrawCharacterAtOffset();
void UiText_DrawNumberInWindow();
void UiWork_Finalize();
void UiWork_Finalize();
void UiWork_Finalize();
void Party_AdjustSixDigitCounterA();
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

void SceneState_ApplyValues14And0And5(void);
s32 SceneData_ReturnZero(void);

/* The scene table in the overlay's data. */
extern u8 KareiTorebi_SceneTable[];
u8 *SceneData_GetTable98a0(void);
void FieldScene_RunScene3ae_02000144(void);
void KareiTorebi_AskGoToTolbi(void);
void FieldScene_RunScene3ae_02000260(void);
void FieldScene_RunScene3ae_020002dc(void);
void SceneDialogue_RunActor12Event(void);
void FieldScene_RunActorThirteenFlagDialogue(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void KareiTorebi_TalkScaryTrip(void);
void KareiTorebi_TalkSeasickTourists(void);
void FieldScene_RunScene3ae_020006c8(void);
s32 FieldScene_RunScene3ae_0200086c(void);
void FieldScene_RunScene3ae_020008cc(void);
void SceneState_SetRuntimeWord448To521AndSend303(void);
void FieldScene_PlaceSlots14And15(void);
void SceneActor_SetMode3AndFlagBit1(s32 no);
void FieldScene_RunScene3aeSequenceA(void);
void FieldScene_RunScene3aeSequenceB(void);

#endif
