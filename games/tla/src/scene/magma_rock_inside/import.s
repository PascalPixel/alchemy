.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02004c58
	.thumb_func
Func_02004c58:
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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02004c98
	.thumb_func
Func_02004c98:
	overlay_veneer Object_SetMode
	.global Func_02004ca0
	.thumb_func
Func_02004ca0:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02004cb0
	.thumb_func
Func_02004cb0:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02004cc0
	.thumb_func
Func_02004cc0:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004cc8
	.thumb_func
Func_02004cc8:
	overlay_veneer Object_SetPosition
	.global Func_02004cd0
	.thumb_func
Func_02004cd0:
	overlay_veneer Object_CommitPosition
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02004cf0
	.thumb_func
Func_02004cf0:
	overlay_veneer Func_080201f0
	.global Func_02004cf8
	.thumb_func
Func_02004cf8:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02004d18
	.thumb_func
Func_02004d18:
	overlay_veneer Func_08020230
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Func_080202f0
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer EventRuntime_StartFar
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global UiText_DrawQuantityPairWithCue
	.thumb_func
UiText_DrawQuantityPairWithCue:
	overlay_veneer UiText_DrawQuantityPairWithCueFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02004de0
	.thumb_func
Func_02004de0:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004df8
	.thumb_func
Func_02004df8:
	overlay_veneer Func_080c8208
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004e10
	.thumb_func
Func_02004e10:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	overlay_veneer Func_080c8240
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004e30
	.thumb_func
Func_02004e30:
	overlay_veneer Func_080c8288
	.global Func_02004e38
	.thumb_func
Func_02004e38:
	overlay_veneer Func_080c82e0
	.global Func_02004e40
	.thumb_func
Func_02004e40:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004e48
	.thumb_func
Func_02004e48:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02004e50
	.thumb_func
Func_02004e50:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Func_02004e58
	.thumb_func
Func_02004e58:
	overlay_veneer Func_080c83e0
	.global Func_02004e60
	.thumb_func
Func_02004e60:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02004e70
	.thumb_func
Func_02004e70:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02004e78
	.thumb_func
Func_02004e78:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02004e80
	.thumb_func
Func_02004e80:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_02004e88
	.thumb_func
Func_02004e88:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02004e90
	.thumb_func
Func_02004e90:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02004e98
	.thumb_func
Func_02004e98:
	overlay_veneer Func_080c8518
	.global Func_02004ea0
	.thumb_func
Func_02004ea0:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004ea8
	.thumb_func
Func_02004ea8:
	overlay_veneer Func_080c8580
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_02004eb8
	.thumb_func
Func_02004eb8:
	overlay_veneer Event_NoOpFar
	.global Func_02004ec0
	.thumb_func
Func_02004ec0:
	overlay_veneer Func_080c86d0
	.global Func_02004ec8
	.thumb_func
Func_02004ec8:
	overlay_veneer Func_080c86d8
	.global Func_02004ed0
	.thumb_func
Func_02004ed0:
	overlay_veneer Func_080c8710
	.global Func_02004ed8
	.thumb_func
Func_02004ed8:
	overlay_veneer Func_080c8718
	.global Func_02004ee0
	.thumb_func
Func_02004ee0:
	overlay_veneer Func_080c8720
	.global Func_02004ee8
	.thumb_func
Func_02004ee8:
	overlay_veneer Func_080c8728
	.global Func_02004ef0
	.thumb_func
Func_02004ef0:
	overlay_veneer Func_080c8730
	.global Func_02004ef8
	.thumb_func
Func_02004ef8:
	overlay_veneer Func_080c8738
	.global Func_02004f00
	.thumb_func
Func_02004f00:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_02004f08
	.thumb_func
Func_02004f08:
	overlay_veneer Func_080c8780
	.global Func_02004f10
	.thumb_func
Func_02004f10:
	overlay_veneer Func_080c87c0
	.global Func_02004f18
	.thumb_func
Func_02004f18:
	overlay_veneer Func_080c87d0
	.global Func_02004f20
	.thumb_func
Func_02004f20:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02004f28
	.thumb_func
Func_02004f28:
	overlay_veneer Func_080c88c8
	.global Func_02004f30
	.thumb_func
Func_02004f30:
	overlay_veneer Audio_PlayCue
