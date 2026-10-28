.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_020012e8
	.thumb_func
Func_020012e8:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_020012f8
	.thumb_func
Func_020012f8:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Window_Create
	.thumb_func
Window_Create:
	overlay_veneer Window_CreateFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Text_DrawCharacterAtOffset
	.thumb_func
Text_DrawCharacterAtOffset:
	overlay_veneer Text_DrawCharacterAtOffsetFar
	.global Text_DrawNumberInWindow
	.thumb_func
Text_DrawNumberInWindow:
	overlay_veneer Text_DrawNumberInWindowFar
	.global Func_02001328
	.thumb_func
Func_02001328:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global Func_02001338
	.thumb_func
Func_02001338:
	overlay_veneer Func_08038340
	.global Menu_SelectCoinsOrMedals
	.thumb_func
Menu_SelectCoinsOrMedals:
	overlay_veneer Menu_SelectCoinsOrMedalsFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Party_AdjustCoins
	.thumb_func
Party_AdjustCoins:
	overlay_veneer Party_AdjustCoinsFar
	.global Func_02001358
	.thumb_func
Func_02001358:
	overlay_veneer Func_080ad208
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global Func_02001370
	.thumb_func
Func_02001370:
	overlay_veneer Func_080ad2f8
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001380
	.thumb_func
Func_02001380:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001388
	.thumb_func
Func_02001388:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020013b0
	.thumb_func
Func_020013b0:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020013c8
	.thumb_func
Func_020013c8:
	overlay_veneer EventRuntime_SetMessageFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020013d8
	.thumb_func
Func_020013d8:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	overlay_veneer Func_080c83e0
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02001400
	.thumb_func
Func_02001400:
	overlay_veneer Func_080c8928
	.global Func_02001408
	.thumb_func
Func_02001408:
	overlay_veneer Sound_PlayFar
