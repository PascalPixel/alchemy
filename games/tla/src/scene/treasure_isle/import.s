.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Func_02000f7c
	.thumb_func
Func_02000f7c:
	overlay_veneer Object_SetMode
	.global Func_02000f84
	.thumb_func
Func_02000f84:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	overlay_veneer Map_CopyCellAttributeRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02000fb4
	.thumb_func
Func_02000fb4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	overlay_veneer EventRuntime_StartFar
	.global Func_02000fd4
	.thumb_func
Func_02000fd4:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001004
	.thumb_func
Func_02001004:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001014
	.thumb_func
Func_02001014:
	overlay_veneer Func_080c8240
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02001024
	.thumb_func
Func_02001024:
	overlay_veneer Func_080c8288
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
	.endif
	.endif
	.global Func_02001034
	.thumb_func
Func_02001034:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_02001044
	.thumb_func
Func_02001044:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.endif
	.endif
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02001054
	.thumb_func
Func_02001054:
	.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusicFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusicFar
	.else
	overlay_veneer Func_080c8580
	.endif
	.endif
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02001064
	.thumb_func
Func_02001064:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Audio_PlayCue
