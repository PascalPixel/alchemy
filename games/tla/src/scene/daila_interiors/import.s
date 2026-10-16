.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global UnitGet
	.thumb_func
UnitGet:
	overlay_veneer UnitGetFar
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
	.global Func_02000c9c
	.thumb_func
Func_02000c9c:
	overlay_veneer Object_ResetMotion
	.global Func_02000ca4
	.thumb_func
Func_02000ca4:
	.ifdef TLA_EDITION_DE
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer MapLayer_SetField22Far
	.else
	overlay_veneer Func_080201a0
	.endif
	.endif
	.endif
	.endif
	.global Func_02000cac
	.thumb_func
Func_02000cac:
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
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Func_08020300
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02000ccc
	.thumb_func
Func_02000ccc:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global UnitCalc
	.thumb_func
UnitCalc:
	overlay_veneer UnitCalcFar
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	overlay_veneer Item_Get
	.global Inventory_GetQuantity
	.thumb_func
Inventory_GetQuantity:
	overlay_veneer Inventory_GetQuantityFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Inventory_Discard
	.thumb_func
Inventory_Discard:
	overlay_veneer Inventory_DiscardFar
	.global Unit_RefreshPsynergy
	.thumb_func
Unit_RefreshPsynergy:
	overlay_veneer Unit_RefreshPsynergyFar
	.global Inventory_CountItem
	.thumb_func
Inventory_CountItem:
	overlay_veneer Inventory_CountItemFar
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer EventRuntime_BeginFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer EventRuntime_EndFar
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
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
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
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02000d94
	.thumb_func
Func_02000d94:
	overlay_veneer Func_080c8210
	.global Func_02000d9c
	.thumb_func
Func_02000d9c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
	.endif
	.endif
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	overlay_veneer Func_080c83d8
	.endif
	.endif
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Engine_EventResolvePendingActions
	.thumb_func
Engine_EventResolvePendingActions:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02000dbc
	.thumb_func
Func_02000dbc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.else
	overlay_veneer Func_080c8558
	.endif
	.endif
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86e8
	.else
	overlay_veneer Func_080c86f8
	.endif
	.endif
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86f8
	.else
	overlay_veneer Func_080c8708
	.endif
	.endif
	.global Func_02000dd4
	.thumb_func
Func_02000dd4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02000ddc
	.thumb_func
Func_02000ddc:
	overlay_veneer Func_08108008
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	overlay_veneer Func_08108010
	.global Func_02000dec
	.thumb_func
Func_02000dec:
	overlay_veneer Func_08108018
	.global Func_02000df4
	.thumb_func
Func_02000df4:
	overlay_veneer Func_08108068
	.global Func_02000dfc
	.thumb_func
Func_02000dfc:
	overlay_veneer Audio_PlayCue
