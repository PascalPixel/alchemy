@ Import veneers after the CLEAR overlay's code: fixed 8-byte veneers through
@ which the overlay calls main-image code, each loading its target into r4 and
@ branching, which the calling convention permits.
@ credit: reconstructed_veneer — CLEAR overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer Resource_DecodeType01Far
	.global Ui_LoadWindowGraphics
	.thumb_func
Ui_LoadWindowGraphics:
	overlay_veneer Ui_LoadWindowGraphicsFar
	.global Engine_ResourceGetTableEntry
	.thumb_func
Engine_ResourceGetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Engine_BlendSetDarkenTarget16
	.thumb_func
Engine_BlendSetDarkenTarget16:
	overlay_veneer Blend_SetDarkenTarget16Far
	.global SerialRuntime_Initialize
	.thumb_func
SerialRuntime_Initialize:
	overlay_veneer SerialRuntime_InitializeFar
	.global SerialRuntime_RemoveIrqHandlers
	.thumb_func
SerialRuntime_RemoveIrqHandlers:
	overlay_veneer SerialRuntime_RemoveIrqHandlersFar
	.global SerialRuntime_BeginTransferA
	.thumb_func
SerialRuntime_BeginTransferA:
	overlay_veneer SerialRuntime_BeginTransferAFar
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiWindow_DrawDividerLine
	.thumb_func
UiWindow_DrawDividerLine:
	overlay_veneer UiWindow_DrawDividerLineFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawString
	.thumb_func
UiText_DrawString:
	overlay_veneer UiText_DrawStringFar
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Menu_SelectSaveSlotAction
	.thumb_func
Menu_SelectSaveSlotAction:
	overlay_veneer Menu_SelectSaveSlotActionFar
	.global Menu_SelectResourceLayout
	.thumb_func
Menu_SelectResourceLayout:
	overlay_veneer Menu_SelectResourceLayoutFar
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer UiText_ShowCenteredMessageFar
	.global UiWindow_SetTilemapEntry
	.thumb_func
UiWindow_SetTilemapEntry:
	overlay_veneer UiWindow_SetTilemapEntryFar
	.global Save_WriteSelectedSlot
	.thumb_func
Save_WriteSelectedSlot:
	overlay_veneer Save_WriteSelectedSlotFar
	.global SaveState_LoadRecordIntoWork
	.thumb_func
SaveState_LoadRecordIntoWork:
	overlay_veneer SaveState_LoadRecordIntoWorkFar
	.global SaveState_CopySlotPair
	.thumb_func
SaveState_CopySlotPair:
	overlay_veneer SaveState_CopySlotPairFar
	.global SaveState_DeleteSelectedSlot
	.thumb_func
SaveState_DeleteSelectedSlot:
	overlay_veneer SaveState_DeleteSelectedSlotFar
	.global SaveState_ScanRecordFlags
	.thumb_func
SaveState_ScanRecordFlags:
	overlay_veneer SaveState_ScanRecordFlagsFar
	.global NameEntry_EditOwnerName
	.thumb_func
NameEntry_EditOwnerName:
	overlay_veneer NameEntry_EditOwnerNameFar
	.global PaletteGlow_Update
	.thumb_func
PaletteGlow_Update:
	overlay_veneer PaletteGlow_UpdateFar
	.global Menu_RunConfirmSelection
	.thumb_func
Menu_RunConfirmSelection:
	overlay_veneer Menu_RunConfirmSelectionFar
	.global Graphics_ExpandVramTilesByColorTable
	.thumb_func
Graphics_ExpandVramTilesByColorTable:
	overlay_veneer Graphics_ExpandVramTilesByColorTableFar
	.global Link_DrawShiftedTilePair
	.thumb_func
Link_DrawShiftedTilePair:
	overlay_veneer Link_DrawShiftedTilePairFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Func_02001444
	.thumb_func
Func_02001444:
	overlay_veneer Func_08077018
	.global GameState_InitDefaults
	.thumb_func
GameState_InitDefaults:
	overlay_veneer GameState_InitDefaultsFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Party_SetFlag32AndRefreshMembers
	.thumb_func
Party_SetFlag32AndRefreshMembers:
	overlay_veneer Party_SetFlag32AndRefreshMembersFar
	.global Runtime_GetBuildStampTime
	.thumb_func
Runtime_GetBuildStampTime:
	overlay_veneer Runtime_GetBuildStampTimeFar
	.global InventorySnapshot_Restore
	.thumb_func
InventorySnapshot_Restore:
	overlay_veneer InventorySnapshot_RestoreFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Event_SetPairWork1c0
	.thumb_func
Event_SetPairWork1c0:
	overlay_veneer Event_SetPairWork1c0Far
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Object_CallSpawnRoutineAtOrigin
	.thumb_func
Object_CallSpawnRoutineAtOrigin:
	overlay_veneer Object_CallSpawnRoutineAtOriginFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
