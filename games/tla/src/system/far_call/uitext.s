.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall004
Resource_FarCall004:
	.global WindowSystem_InitFieldFar
	.thumb_func
WindowSystem_InitFieldFar:
	overlay_veneer WindowSystem_InitField
	.global WindowSystem_InitBattleFar
	.thumb_func
WindowSystem_InitBattleFar:
	overlay_veneer WindowSystem_InitBattle
	.global Window_CreateFar
	.thumb_func
Window_CreateFar:
	overlay_veneer Window_Create
	.global Window_CloseFar
	.thumb_func
Window_CloseFar:
	overlay_veneer Window_Close
	.global Message_DecodeRawUnboundedFar
	.thumb_func
Message_DecodeRawUnboundedFar:
	overlay_veneer Message_DecodeRawUnbounded
	.global Message_DecodeRawFar
	.thumb_func
Message_DecodeRawFar:
	overlay_veneer Message_DecodeRaw
	.global Message_CopyExpandedFar
	.thumb_func
Message_CopyExpandedFar:
	overlay_veneer Message_CopyExpanded
	.global Message_OpenWindowFar
	.thumb_func
Message_OpenWindowFar:
	overlay_veneer Message_OpenWindow
	.global FieldMessage_ShowFar
	.thumb_func
FieldMessage_ShowFar:
	overlay_veneer FieldMessage_Show
	.global Message_AllTypedFar
	.thumb_func
Message_AllTypedFar:
	overlay_veneer Message_AllTyped
	.global Window_IsClosedFar
	.thumb_func
Window_IsClosedFar:
	overlay_veneer Window_IsClosed
	.global Window_SetBusyFlagsFar
	.thumb_func
Window_SetBusyFlagsFar:
	overlay_veneer Window_SetBusyFlags
	.global WindowSprite_PrepareForRedrawFar
	.thumb_func
WindowSprite_PrepareForRedrawFar:
	overlay_veneer WindowSprite_PrepareForRedraw
	.global Window_ClearInteriorTilesFar
	.thumb_func
Window_ClearInteriorTilesFar:
	overlay_veneer Window_ClearInteriorTiles
	.global Window_DrawDividerLineFar
	.thumb_func
Window_DrawDividerLineFar:
	overlay_veneer Window_DrawDividerLine
	.global Text_DrawMessageFar
	.thumb_func
Text_DrawMessageFar:
	overlay_veneer Text_DrawMessage
	.global Text_DrawCharacterAtOffsetFar
	.thumb_func
Text_DrawCharacterAtOffsetFar:
	overlay_veneer Text_DrawCharacterAtOffset
	.global Text_DrawStringFar
	.thumb_func
Text_DrawStringFar:
	overlay_veneer Text_DrawString
