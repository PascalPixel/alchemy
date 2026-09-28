@ Far-call stub table heading the window and text code at 0x08015000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08015000
	.thumb_func
Func_08015000:
	overlay_veneer 0x08015f31
	overlay_veneer 0x08016019
	.global UiWindow_CreateFar
	.thumb_func
UiWindow_CreateFar:
	overlay_veneer 0x080162d5
	.global UiWork_FinalizeFar
	.thumb_func
UiWork_FinalizeFar:
	overlay_veneer 0x08016419
	.global Party_Apply
	.thumb_func
Party_Apply:
	.global Ui_AdjustValueWithoutLimitFar
	.thumb_func
Ui_AdjustValueWithoutLimitFar:
	overlay_veneer 0x0801964d
	overlay_veneer 0x080196c5
	.global UiText_CopyMessageStringFar
	.thumb_func
UiText_CopyMessageStringFar:
	overlay_veneer 0x0801965d
	.global UiText_OpenMessageWindowFar
	.thumb_func
UiText_OpenMessageWindowFar:
	overlay_veneer 0x08017659
	.global UiText_DrawMessage
	.thumb_func
UiText_DrawMessage:
	.global UiText_ShowPositionedMessageAndWaitFar
	.thumb_func
UiText_ShowPositionedMessageAndWaitFar:
	overlay_veneer 0x0801776d
	.global UiWork_IsCompleteFar
	.thumb_func
UiWork_IsCompleteFar:
	overlay_veneer 0x08017365
	.global UiWork_IsIdleFar
	.thumb_func
UiWork_IsIdleFar:
	overlay_veneer 0x08017395
	.global UiWork_SetBusyFlagsFar
	.thumb_func
UiWork_SetBusyFlagsFar:
	overlay_veneer 0x08017621
	.global UiWindow_Clear
	.thumb_func
UiWindow_Clear:
	overlay_veneer 0x08016479
	.global UiWindow_ClearInteriorTilesFar
	.thumb_func
UiWindow_ClearInteriorTilesFar:
	overlay_veneer 0x080164d5
	.global UiWindow_DrawDividerLineFar
	.thumb_func
UiWindow_DrawDividerLineFar:
	overlay_veneer 0x0801e41d
	.global Func_08015078
	.thumb_func
Func_08015078:
	.global UiText_DrawMessageAt
	.thumb_func
UiText_DrawMessageAt:
	overlay_veneer 0x0801e74d
	.global UiText_DrawAt
	.thumb_func
UiText_DrawAt:
	.global UiText_DrawCharacterAtOffsetFar
	.thumb_func
UiText_DrawCharacterAtOffsetFar:
	overlay_veneer 0x0801e7c1
	overlay_veneer 0x0801e859
	.global UiText_DrawStringAtOffsetFar
	.global Func_08015090
	.thumb_func
UiText_DrawStringAtOffsetFar:
Func_08015090:
	overlay_veneer 0x0801e8b1
	.global UiText_DrawStringInWindowFar
	.thumb_func
UiText_DrawStringInWindowFar:
	overlay_veneer 0x0801e941
	overlay_veneer 0x0801e9a1
	.global UiText_DrawNumberAtOffsetFar
	.global Func_080150a8
	.thumb_func
UiText_DrawNumberAtOffsetFar:
Func_080150a8:
	overlay_veneer 0x0801e9d5
	.global UiNumber_DrawAt
	.thumb_func
UiNumber_DrawAt:
	.global UiText_DrawNumberInWindowFar
	.thumb_func
UiText_DrawNumberInWindowFar:
	overlay_veneer 0x0801ea09
	.global UiWork_SetParamNibbleFar
	.thumb_func
UiWork_SetParamNibbleFar:
	overlay_veneer 0x0801e71d
	overlay_veneer 0x0801e739
	.global RenderOutput_CreateFar
	.global Func_080150c8
	.thumb_func
RenderOutput_CreateFar:
Func_080150c8:
	overlay_veneer 0x0801eadd
	.global RenderOutput_CreateFromResourceFar
	.thumb_func
RenderOutput_CreateFromResourceFar:
	overlay_veneer 0x0801eb65
	.global SideObject_CreateFar
	.thumb_func
SideObject_CreateFar:
	overlay_veneer 0x0801ec6d
	.global Ui_LoadCharacterEntryForSlotFar
	.thumb_func
Ui_LoadCharacterEntryForSlotFar:
	overlay_veneer 0x0801ed41
	overlay_veneer 0x0801edcd
	overlay_veneer 0x0801eddd
	.global UiWindow_CreateWithSideObjectFar
	.global Func_080150f8
	.thumb_func
UiWindow_CreateWithSideObjectFar:
Func_080150f8:
	overlay_veneer 0x08019da9
	.global UiWork_FinalizeEntityMatchingLocalizedIdFar
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedIdFar:
	overlay_veneer 0x08019e49
	.global Func_08015108
	.thumb_func
Func_08015108:
	overlay_veneer 0x080187ad
	overlay_veneer 0x080187fd
	.global UiWork_ClearValueNameTablesFar
	.thumb_func
UiWork_ClearValueNameTablesFar:
	overlay_veneer 0x080198dd
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	.global UiWork_PushValueSlotFar
	.thumb_func
UiWork_PushValueSlotFar:
	overlay_veneer 0x08019909
	overlay_veneer 0x0801ef09
	.global UiWindow_DrawPartyStatusContentsFar
	.thumb_func
UiWindow_DrawPartyStatusContentsFar:
	overlay_veneer 0x0801f201
	overlay_veneer 0x0801f5d5
	.global UiWork_FinalizePendingCoreFar
	.thumb_func
UiWork_FinalizePendingCoreFar:
	overlay_veneer 0x08019a55
	overlay_veneer 0x080197c5
	overlay_veneer 0x0801a66d
	overlay_veneer 0x0801a779
	overlay_veneer 0x0801a7c1
	overlay_veneer 0x0801a969
	overlay_veneer 0x0801a97d
	overlay_veneer 0x0801b011
	overlay_veneer 0x0801b149
	overlay_veneer 0x0801b229
	overlay_veneer 0x0801b399
	overlay_veneer 0x0801b425
	overlay_veneer 0x0801c0c9
	overlay_veneer 0x0801c0cd
	overlay_veneer 0x0801a7f5
	overlay_veneer 0x0801a90d
	.global Battle_CollectPartyCommandsFar
	.global Func_080151c0
	.thumb_func
Battle_CollectPartyCommandsFar:
Func_080151c0:
	overlay_veneer 0x08027115
	.global UiText_ShowMessageAndWaitCoreFar
	.thumb_func
UiText_ShowMessageAndWaitCoreFar:
	overlay_veneer 0x080175a1
	.global UiText_PrepareMessageWorkFar
	.thumb_func
UiText_PrepareMessageWorkFar:
	overlay_veneer 0x080174f9
	overlay_veneer 0x0801c0d9
	.global Func_080151e0
	.thumb_func
Func_080151e0:
	overlay_veneer 0x08019d2d
	overlay_veneer 0x0801c245
	overlay_veneer 0x080289e9
	overlay_veneer 0x08028c05
	.global Func_08015200
	.thumb_func
Func_08015200:
	overlay_veneer 0x0801c34d
	.global UiTimedNotice_CloseIfActiveFar
	.thumb_func
UiTimedNotice_CloseIfActiveFar:
	overlay_veneer 0x0801c429
	overlay_veneer 0x08019aa1
	.global UiWork_FinalizeSharedSlotFar
	.global Func_08015218
	.thumb_func
UiWork_FinalizeSharedSlotFar:
Func_08015218:
	overlay_veneer 0x080174d9
	.global UiWork_ResetFreeChannelFar
	.thumb_func
UiWork_ResetFreeChannelFar:
	overlay_veneer 0x08016759
	overlay_veneer 0x0801c0dd
	overlay_veneer 0x0801c155
	overlay_veneer 0x0801c17d
	overlay_veneer 0x0801c925
	overlay_veneer 0x0801c459
	.global ItemIcon_LoadTilesFar
	.global Func_08015250
	.thumb_func
ItemIcon_LoadTilesFar:
Func_08015250:
	overlay_veneer 0x0801a371
	.global UiText_RenderWideStringAtOffsetFar
	.thumb_func
UiText_RenderWideStringAtOffsetFar:
	overlay_veneer 0x08017aa5
	overlay_veneer 0x0801cf45
	.global Func_08015268
	.thumb_func
Func_08015268:
	overlay_veneer 0x0801eded
	.global RenderOutput_RedrawSavedRectFar
	.thumb_func
RenderOutput_RedrawSavedRectFar:
	.global UiWindow_Commit
	.thumb_func
UiWindow_Commit:
	overlay_veneer 0x08016499
	.global RenderOutput_ClearListFar
	.thumb_func
RenderOutput_ClearListFar:
	overlay_veneer 0x080164ad
	.global UiWindow_SetTilemapEntryFar
	.thumb_func
UiWindow_SetTilemapEntryFar:
	overlay_veneer 0x08019001
	overlay_veneer 0x08028f99
	overlay_veneer 0x080291e5
	.global Func_08015298
	.thumb_func
Func_08015298:
	.global Resource_LoadByModeIntoSlotFar
	.thumb_func
Resource_LoadByModeIntoSlotFar:
	overlay_veneer 0x0801bcd5
	overlay_veneer 0x0801c49d
	.global Func_080152a8
	.thumb_func
Func_080152a8:
	overlay_veneer 0x0801e319
	overlay_veneer 0x0801a089
	.global BattleLayout_HighlightPartyPanelsFar
	.global Func_080152b8
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
Func_080152b8:
	overlay_veneer 0x0802281d
	overlay_veneer 0x08019fcd
	overlay_veneer 0x0801a405
	.global Func_080152d0
	.thumb_func
Func_080152d0:
	.global UiIcon_Draw
	.thumb_func
UiIcon_Draw:
	overlay_veneer 0x0801eb91
	overlay_veneer 0x0801ec25
	overlay_veneer 0x0801f9b5
	overlay_veneer 0x080207c5
	overlay_veneer 0x080208e5
	overlay_veneer 0x0801fba9
	overlay_veneer 0x0801fc85
	overlay_veneer 0x0801faa9
	overlay_veneer 0x0801fa3d
	overlay_veneer 0x0801f77d
	overlay_veneer 0x08020bd9
	overlay_veneer 0x08020aed
	overlay_veneer 0x08020b01
	overlay_veneer 0x0801b1ed
	overlay_veneer 0x08029555
	overlay_veneer 0x0802977d
	overlay_veneer 0x0801c469
	.global Djinn_ShowJoinedMessageFar
	.global Func_08015358
	.thumb_func
Djinn_ShowJoinedMessageFar:
Func_08015358:
	overlay_veneer 0x08021229
	.global PaletteGlow_UpdateFar
	.thumb_func
PaletteGlow_UpdateFar:
	overlay_veneer 0x0801ccc1
	overlay_veneer 0x0801d4cd
	.global Func_08015370
	.thumb_func
Func_08015370:
	overlay_veneer 0x08029505
	overlay_veneer 0x0801ee69
	.global Func_08015380
	.thumb_func
Func_08015380:
	overlay_veneer 0x08028d75
	.global Menu_SelectEntry19To1cFar
	.thumb_func
Menu_SelectEntry19To1cFar:
	overlay_veneer 0x08028db5
	.global Inventory_RequestMode
	.thumb_func
Inventory_RequestMode:
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer 0x08028df5
	overlay_veneer 0x08028ea9
	overlay_veneer 0x0802106d
	overlay_veneer 0x0801a32d
	.global Menu_RunConfirmSelectionAtFar
	.global Func_080153b0
	.thumb_func
Menu_RunConfirmSelectionAtFar:
Func_080153b0:
	overlay_veneer 0x08028e55
	.global Func_080153b8
	.thumb_func
Func_080153b8:
	overlay_veneer 0x080209b1
	overlay_veneer 0x08018791
	.global UiWindow_SetPaletteBitRectFar
	.thumb_func
UiWindow_SetPaletteBitRectFar:
	overlay_veneer 0x08022769
	.global Func_080153d0
	.thumb_func
Func_080153d0:
	overlay_veneer 0x0801a3d1
	.global Func_080153d8
	.thumb_func
Func_080153d8:
	overlay_veneer 0x08021a19
	.global Func_080153e0
	.thumb_func
Func_080153e0:
	overlay_veneer 0x0801e3c9
	overlay_veneer 0x0802899d
	.global Func_080153f0
	.thumb_func
Func_080153f0:
	overlay_veneer 0x08021bc9
	overlay_veneer 0x080175c1
	overlay_veneer 0x0801f731
	.global UiWindow_DrawFrameFar
	.global Func_08015408
	.thumb_func
UiWindow_DrawFrameFar:
Func_08015408:
	overlay_veneer 0x080170f9
	.global UiWindow_EraseBorderRectFar
	.thumb_func
UiWindow_EraseBorderRectFar:
	overlay_veneer 0x08016179
	.global Link_DrawShiftedTilePairFar
	.thumb_func
Link_DrawShiftedTilePairFar:
	overlay_veneer 0x080219c9
	.global Party_ShowJoinedMessageFar
	.thumb_func
Party_ShowJoinedMessageFar:
	overlay_veneer 0x08021391
	.global Party_ShowPairJoinedMessageFar
	.thumb_func
Party_ShowPairJoinedMessageFar:
	overlay_veneer 0x08021489
