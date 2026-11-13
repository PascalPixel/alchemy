.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
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
	.global Random16
	.thumb_func
Random16:
	overlay_veneer Random16Far
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.global HeapSlot_Alloc
	.thumb_func
HeapSlot_Alloc:
	overlay_veneer HeapSlot_AllocFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02000560
	.thumb_func
Func_02000560:
	overlay_veneer Decompress_TaggedLzFar
	.global VramCache_FreeEntry
	.thumb_func
VramCache_FreeEntry:
	overlay_veneer VramCache_FreeEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Map_CopyCellsRect
	.thumb_func
Map_CopyCellsRect:
	overlay_veneer Map_CopyCellsRectFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global MapGrid_CopyCellPictureCellsRect
	.thumb_func
MapGrid_CopyCellPictureCellsRect:
	overlay_veneer MapGrid_CopyCellPictureCellsRectFar
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Rope_Initialize
	.thumb_func
Rope_Initialize:
	overlay_veneer Rope_InitializeFar
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	overlay_veneer Rope_CoilFar
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	overlay_veneer EffectSprite_InitFar
	.global EffectSprite_DrawAtWorldPosition
	.thumb_func
EffectSprite_DrawAtWorldPosition:
	overlay_veneer EffectSprite_DrawAtWorldPositionFar
	.global BattleEffect_StepVector3
	.thumb_func
BattleEffect_StepVector3:
	overlay_veneer BattleEffect_StepVector3Far
