.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
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
	.global Func_02001fa8
	.thumb_func
Func_02001fa8:
	overlay_veneer Irq_SetHandlerFar
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Func_02001fe0
	.thumb_func
Func_02001fe0:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02001ff0
	.thumb_func
Func_02001ff0:
	overlay_veneer Actor_StartScriptFar
	.global Func_02001ff8
	.thumb_func
Func_02001ff8:
	overlay_veneer Func_080200c0
	.global Func_02002000
	.thumb_func
Func_02002000:
	overlay_veneer Actor_Destroy
	.global Func_02002008
	.thumb_func
Func_02002008:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002010
	.thumb_func
Func_02002010:
	overlay_veneer Actor_CommitPosition
	.global Func_02002018
	.thumb_func
Func_02002018:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002028
	.thumb_func
Func_02002028:
	overlay_veneer GetMapCellCollisionFar
	.global Func_02002030
	.thumb_func
Func_02002030:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02002048
	.thumb_func
Func_02002048:
	overlay_veneer Func_080202f8
	.global Func_02002050
	.thumb_func
Func_02002050:
	overlay_veneer Field_CheckSpheresOverlapFar
	.global Func_02002058
	.thumb_func
Func_02002058:
	overlay_veneer Func_08038248
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02002068
	.thumb_func
Func_02002068:
	overlay_veneer Event_StartFar
	.global Func_02002070
	.thumb_func
Func_02002070:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020020c0
	.thumb_func
Func_020020c0:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_020020e0
	.thumb_func
Func_020020e0:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020020f0
	.thumb_func
Func_020020f0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002110
	.thumb_func
Func_02002110:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002118
	.thumb_func
Func_02002118:
	overlay_veneer Leader_StopMovingFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002130
	.thumb_func
Func_02002130:
	overlay_veneer Func_080c83e0
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_02002140
	.thumb_func
Func_02002140:
	overlay_veneer Func_080c8480
	.global Func_02002148
	.thumb_func
Func_02002148:
	overlay_veneer Event_ResolveAllPendingActionsFar
	.global Func_02002150
	.thumb_func
Func_02002150:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002158
	.thumb_func
Func_02002158:
	overlay_veneer Func_080c8580
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02002168
	.thumb_func
Func_02002168:
	overlay_veneer Func_080c8688
	.global Func_02002170
	.thumb_func
Func_02002170:
	overlay_veneer Func_080c86a8
	.global Func_02002178
	.thumb_func
Func_02002178:
	overlay_veneer Func_080c86c0
	.global Func_02002180
	.thumb_func
Func_02002180:
	overlay_veneer Func_080c86e8
	.global Func_02002188
	.thumb_func
Func_02002188:
	overlay_veneer Func_080c8718
	.global Func_02002190
	.thumb_func
Func_02002190:
	overlay_veneer Func_080c8720
	.global Func_02002198
	.thumb_func
Func_02002198:
	overlay_veneer Func_080c8728
	.global Func_020021a0
	.thumb_func
Func_020021a0:
	overlay_veneer Func_080c8760
	.global Func_020021a8
	.thumb_func
Func_020021a8:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020021b0
	.thumb_func
Func_020021b0:
	overlay_veneer Sound_PlayFar
