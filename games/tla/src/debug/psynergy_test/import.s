.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
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
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	overlay_veneer Func_080200c0
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	overlay_veneer Map_CopyCellsRectFar
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	overlay_veneer Func_08020300
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_02000dec
	.thumb_func
Func_02000dec:
	overlay_veneer Func_080380f8
	.global Func_02000df4
	.thumb_func
Func_02000df4:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	overlay_veneer Func_080382f8
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
	overlay_veneer Func_08038318
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global Unit_AddHp
	.thumb_func
Unit_AddHp:
	overlay_veneer Unit_AddHpFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global Djinn_Activate
	.thumb_func
Djinn_Activate:
	overlay_veneer Djinn_ActivateFar
	.global Unit_LevelUpTo
	.thumb_func
Unit_LevelUpTo:
	overlay_veneer Unit_LevelUpToFar
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	overlay_veneer Item_AdjustCounterFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Func_080c8078
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_02000e74
	.thumb_func
Func_02000e74:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02000e94
	.thumb_func
Func_02000e94:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_02000e9c
	.thumb_func
Func_02000e9c:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	overlay_veneer Func_080c8210
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	overlay_veneer Event_SetDestinationFar
	.global Func_02000ebc
	.thumb_func
Func_02000ebc:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8750
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8750
	.else
	overlay_veneer Func_080c8760
	.endif
	.endif
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87a0
	.else
	overlay_veneer Func_080c87b0
	.endif
	.endif
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87a8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87a8
	.else
	overlay_veneer Func_080c87b8
	.endif
	.endif
	.global Func_02000ee4
	.thumb_func
Func_02000ee4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b8
	.else
	overlay_veneer Func_080c87c8
	.endif
	.endif
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_02000efc
	.thumb_func
Func_02000efc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector3Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector3Far
	.else
	overlay_veneer Func_080c87f0
	.endif
	.endif
	.global Func_02000f04
	.thumb_func
Func_02000f04:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8870
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8870
	.else
	overlay_veneer Func_080c8880
	.endif
	.endif
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8878
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8878
	.else
	overlay_veneer Func_080c8888
	.endif
	.endif
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8938
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8938
	.else
	overlay_veneer Func_080c8948
	.endif
	.endif
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Func_08108008
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Func_08108010
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	overlay_veneer Func_08108018
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	overlay_veneer Func_08108068
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	overlay_veneer Audio_PlayCue
