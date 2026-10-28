.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_020027f8
	.thumb_func
Func_020027f8:
	overlay_veneer Object_SetMode
	.global Func_02002800
	.thumb_func
Func_02002800:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002808
	.thumb_func
Func_02002808:
	overlay_veneer Func_080200c0
	.global Func_02002810
	.thumb_func
Func_02002810:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002818
	.thumb_func
Func_02002818:
	overlay_veneer Object_SetPosition
	.global Func_02002820
	.thumb_func
Func_02002820:
	overlay_veneer Object_CommitPosition
	.global Func_02002828
	.thumb_func
Func_02002828:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002838
	.thumb_func
Func_02002838:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002840
	.thumb_func
Func_02002840:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002848
	.thumb_func
Func_02002848:
	overlay_veneer Func_080201f0
	.global Func_02002850
	.thumb_func
Func_02002850:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02002868
	.thumb_func
Func_02002868:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02002870
	.thumb_func
Func_02002870:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValuesToRecord
	.thumb_func
Animation_ApplyChildValuesToRecord:
	overlay_veneer Animation_ApplyChildValuesToRecordFar
	.global Func_02002880
	.thumb_func
Func_02002880:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global Animation_ApplyChildValue
	.thumb_func
Animation_ApplyChildValue:
	overlay_veneer Animation_ApplyChildValueFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002898
	.thumb_func
Func_02002898:
	overlay_veneer Event_StartFar
	.global Func_020028a0
	.thumb_func
Func_020028a0:
	overlay_veneer Event_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020028d0
	.thumb_func
Func_020028d0:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020028e0
	.thumb_func
Func_020028e0:
	overlay_veneer Object_JumpDownLedgeFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02002908
	.thumb_func
Func_02002908:
	overlay_veneer Func_080c8210
	.global Func_02002910
	.thumb_func
Func_02002910:
	overlay_veneer Func_080c8218
	.global Func_02002918
	.thumb_func
Func_02002918:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002928
	.thumb_func
Func_02002928:
	overlay_veneer Func_080c8240
	.global Func_02002930
	.thumb_func
Func_02002930:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002938
	.thumb_func
Func_02002938:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02002940
	.thumb_func
Func_02002940:
	overlay_veneer Func_080c8288
	.ifndef TLA_EDITION_JA
	.global Func_02002948
	.thumb_func
Func_02002948:
	overlay_veneer Func_080c82e0
	.endif
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Func_02002958
	.thumb_func
Func_02002958:
	overlay_veneer Object_ReturnHomeFar
	.global Func_02002960
	.thumb_func
Func_02002960:
	overlay_veneer Func_080c8480
	.global Func_02002968
	.thumb_func
Func_02002968:
	overlay_veneer Event_ResolvePendingActionsFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02002978
	.thumb_func
Func_02002978:
	overlay_veneer Func_080c8688
	.global Func_02002980
	.thumb_func
Func_02002980:
	overlay_veneer Func_080c86a8
	.global Func_02002988
	.thumb_func
Func_02002988:
	overlay_veneer Func_080c86e8
	.global Func_02002990
	.thumb_func
Func_02002990:
	overlay_veneer Func_080c8760
	.global Func_02002998
	.thumb_func
Func_02002998:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	overlay_veneer Func_080c8898
	.global Func_020029a8
	.thumb_func
Func_020029a8:
	overlay_veneer Func_080c8948
	.global Func_020029b0
	.thumb_func
Func_020029b0:
	overlay_veneer Sound_PlayFar
