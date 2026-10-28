.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02000560
	.thumb_func
Func_02000560:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000598
	.thumb_func
Func_02000598:
	overlay_veneer Func_080200c0
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020005c8
	.thumb_func
Func_020005c8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020005d8
	.thumb_func
Func_020005d8:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_020005e0
	.thumb_func
Func_020005e0:
	overlay_veneer Func_080c86a8
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	overlay_veneer Func_080c86b8
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	overlay_veneer Func_080c87c0
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	overlay_veneer Func_080c87d0
	.global Func_02000600
	.thumb_func
Func_02000600:
	overlay_veneer BattleEffect_StepVector3Far
