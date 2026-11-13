.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_Remove
	.thumb_func
Task_Remove:
	overlay_veneer Task_RemoveFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Heap_ReleaseFar
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
	.global Window_LoadGraphics
	.thumb_func
Window_LoadGraphics:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global ScreenFade_ToBlack
	.thumb_func
ScreenFade_ToBlack:
	overlay_veneer ScreenFade_ToBlackFar
	.global Serial_Initialize
	.thumb_func
Serial_Initialize:
	overlay_veneer Serial_InitializeFar
	.global Serial_RemoveIrqHandlers
	.thumb_func
Serial_RemoveIrqHandlers:
	overlay_veneer Serial_RemoveIrqHandlersFar
	.global Serial_StartReceive
	.thumb_func
Serial_StartReceive:
	overlay_veneer Serial_StartReceiveFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
	.global GameFlag_Clear
	.thumb_func
GameFlag_Clear:
	overlay_veneer GameFlag_ClearFar
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
	.global WindowSprite_PrepareForRedraw
	.thumb_func
WindowSprite_PrepareForRedraw:
	overlay_veneer WindowSprite_PrepareForRedrawFar
	.global Window_DrawDividerLine
	.thumb_func
Window_DrawDividerLine:
	overlay_veneer Window_DrawDividerLineFar
	.global Text_DrawMessage
	.thumb_func
Text_DrawMessage:
	overlay_veneer Text_DrawMessageFar
	.global Text_DrawMessageAsTiles
	.thumb_func
Text_DrawMessageAsTiles:
	overlay_veneer Text_DrawMessageAsTilesFar
	.ifndef TLA_EDITION_JA
	.global Text_DrawStringInWindow
	.thumb_func
Text_DrawStringInWindow:
	overlay_veneer Text_DrawStringInWindowFar
	.endif
	.global WindowSprite_Create
	.thumb_func
WindowSprite_Create:
	overlay_veneer WindowSprite_CreateFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global FileList_SelectAction
	.thumb_func
FileList_SelectAction:
	overlay_veneer FileList_SelectActionFar
	.global IconMenu_ChooseSendOption
	.thumb_func
IconMenu_ChooseSendOption:
	overlay_veneer IconMenu_ChooseSendOptionFar
	.global FieldMessage_ShowCentered
	.thumb_func
FieldMessage_ShowCentered:
	overlay_veneer FieldMessage_ShowCenteredFar
	.global WindowSprite_RedrawSavedRect
	.thumb_func
WindowSprite_RedrawSavedRect:
	overlay_veneer WindowSprite_RedrawSavedRectFar
	.global Window_SetTilemapEntry
	.thumb_func
Window_SetTilemapEntry:
	overlay_veneer Window_SetTilemapEntryFar
	.global Import_SaveToChosenFile
	.thumb_func
Import_SaveToChosenFile:
	overlay_veneer Save_ToChosenFileFar
	.global Save_ChooseAndLoadFile
	.thumb_func
Save_ChooseAndLoadFile:
	overlay_veneer Save_ChooseAndLoadFileFar
	.global Save_CopyFile
	.thumb_func
Save_CopyFile:
	overlay_veneer Save_CopyFileFar
	.global Save_EraseSelectedFile
	.thumb_func
Save_EraseSelectedFile:
	overlay_veneer Save_EraseSelectedFileFar
	.global Import_SaveScanFileSummaries
	.thumb_func
Import_SaveScanFileSummaries:
	overlay_veneer Save_ScanFileSummariesFar
	.global NameEntry_EditMemberName
	.thumb_func
NameEntry_EditMemberName:
	overlay_veneer NameEntry_EditMemberNameFar
	.global Resource_CopyFixedBlockB
	.thumb_func
Resource_CopyFixedBlockB:
	overlay_veneer Resource_CopyFixedBlockBFar
	.global Resource_CopyFixedBlockA
	.thumb_func
Resource_CopyFixedBlockA:
	overlay_veneer Resource_CopyFixedBlockAFar
	.global WindowColor_Apply
	.thumb_func
WindowColor_Apply:
	overlay_veneer WindowColor_ApplyFar
	.global YesNoMenu_Run
	.thumb_func
YesNoMenu_Run:
	overlay_veneer YesNoMenu_RunFar
	.global Graphics_ExpandVramTilesByColorTable
	.thumb_func
Graphics_ExpandVramTilesByColorTable:
	overlay_veneer Graphics_ExpandVramTilesByColorTableFar
	.global Func_02002328
	.thumb_func
Func_02002328:
	overlay_veneer Glyph_DrawToTilesFar
	.global Func_02002330
	.thumb_func
Func_02002330:
	overlay_veneer Glyph_AddBlankSpriteFar
	.global Djinn_GetRecoveryTable
	.thumb_func
Djinn_GetRecoveryTable:
	overlay_veneer Djinn_GetRecoveryTableFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Import_MainRoutineWorkInitializeDefaults
	.thumb_func
Import_MainRoutineWorkInitializeDefaults:
	overlay_veneer MainRoutineWork_InitializeDefaultsFar
	.global Party_AddMember
	.thumb_func
Party_AddMember:
	overlay_veneer Party_AddMemberFar
	.global Party_RemoveMember
	.thumb_func
Party_RemoveMember:
	overlay_veneer Party_RemoveMemberFar
	.ifndef TLA_EDITION_JA
	.global Unit_GetExperienceForLevel
	.thumb_func
Unit_GetExperienceForLevel:
	overlay_veneer Unit_GetExperienceForLevelFar
	.endif
	.global Func_02002370
	.thumb_func
Func_02002370:
	overlay_veneer DataTransfer_ApplyFar
	.global Djinn_GiveToParty
	.thumb_func
Djinn_GiveToParty:
	overlay_veneer Djinn_GiveToPartyFar
	.global Unit_UpdateClass
	.thumb_func
Unit_UpdateClass:
	overlay_veneer Unit_UpdateClassFar
	.global BuildStamp_GetTime
	.thumb_func
BuildStamp_GetTime:
	overlay_veneer BuildStamp_GetTimeFar
	.global InventorySnapshot_Restore
	.thumb_func
InventorySnapshot_Restore:
	overlay_veneer InventorySnapshot_RestoreFar
	.global Func_02002398
	.thumb_func
Func_02002398:
	overlay_veneer DataTransfer_GiveDefaultDjinnFar
	.global Func_020023a0
	.thumb_func
Func_020023a0:
	overlay_veneer ClearData_ApplyFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global FieldYesNo_Ask
	.thumb_func
FieldYesNo_Ask:
	overlay_veneer FieldYesNo_AskFar
	.global Func_020023e0
	.thumb_func
Func_020023e0:
	overlay_veneer WorldMap_SetReturnLeavingSceneFar
	.global StatusScreen_Run
	.thumb_func
StatusScreen_Run:
	overlay_veneer StatusScreen_RunFar
	.global MenuCursor_Advance
	.thumb_func
MenuCursor_Advance:
	overlay_veneer MenuCursor_AdvanceFar
	.global MenuCursor_Set
	.thumb_func
MenuCursor_Set:
	overlay_veneer MenuCursor_SetFar
	.global MenuCursor_SetPositionImmediate
	.thumb_func
MenuCursor_SetPositionImmediate:
	overlay_veneer MenuCursor_SetPositionImmediateFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
