@ Import veneers after the OVERLAY_692 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_692 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02004c18
	.thumb_func
Func_02004c18:
	overlay_veneer Func_080000d0
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
	overlay_veneer Func_080001a8
	.global Func_02004c60
	.thumb_func
Func_02004c60:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_02004c80
	.thumb_func
Func_02004c80:
	overlay_veneer Func_080003c8
	.global Func_02004c88
	.thumb_func
Func_02004c88:
	overlay_veneer Func_080003d0
	.global Func_02004c90
	.thumb_func
Func_02004c90:
	overlay_veneer Func_080003d8
	.global Func_02004c98
	.thumb_func
Func_02004c98:
	overlay_veneer Object_SetMode
	.global Func_02004ca0
	.thumb_func
Func_02004ca0:
	overlay_veneer Object_SetCallback
	.global Func_02004ca8
	.thumb_func
Func_02004ca8:
	overlay_veneer Func_080200c0
	.global Func_02004cb0
	.thumb_func
Func_02004cb0:
	overlay_veneer Func_080200c8
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02004cc0
	.thumb_func
Func_02004cc0:
	overlay_veneer Func_08020120
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
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Func_080201e8
	.global Func_02004cf0
	.thumb_func
Func_02004cf0:
	overlay_veneer Func_080201f0
	.global Func_02004cf8
	.thumb_func
Func_02004cf8:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Func_08020228
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer Func_080c8018
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer Func_080c8020
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
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
	overlay_veneer Func_080c80f8
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	overlay_veneer Func_080c8110
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
	overlay_veneer Func_080c8168
	.global Func_02004dc8
	.thumb_func
Func_02004dc8:
	overlay_veneer Func_080c8170
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer Func_080c8180
	.global Func_02004dd8
	.thumb_func
Func_02004dd8:
	overlay_veneer Func_080c8188
	.global Func_02004de0
	.thumb_func
Func_02004de0:
	overlay_veneer Func_080c81a0
	.global Func_02004de8
	.thumb_func
Func_02004de8:
	overlay_veneer Func_080c81d0
	.global Func_02004df0
	.thumb_func
Func_02004df0:
	overlay_veneer Func_080c8200
	.global Func_02004df8
	.thumb_func
Func_02004df8:
	overlay_veneer Func_080c8208
	.global Func_02004e00
	.thumb_func
Func_02004e00:
	overlay_veneer Func_080c8220
	.global Func_02004e08
	.thumb_func
Func_02004e08:
	overlay_veneer Func_080c8228
	.global Func_02004e10
	.thumb_func
Func_02004e10:
	overlay_veneer Func_080c8230
	.global Func_02004e18
	.thumb_func
Func_02004e18:
	overlay_veneer Func_080c8238
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	overlay_veneer Func_080c8240
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer Func_080c8278
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
	overlay_veneer Func_080c8378
	.global Func_02004e48
	.thumb_func
Func_02004e48:
	overlay_veneer Func_080c8380
	.global Func_02004e50
	.thumb_func
Func_02004e50:
	overlay_veneer Func_080c8390
	.global Func_02004e58
	.thumb_func
Func_02004e58:
	overlay_veneer Func_080c83e0
	.global Func_02004e60
	.thumb_func
Func_02004e60:
	overlay_veneer Func_080c8480
	.global Func_02004e68
	.thumb_func
Func_02004e68:
	overlay_veneer Func_080c8498
	.global Func_02004e70
	.thumb_func
Func_02004e70:
	overlay_veneer Func_080c84a0
	.global Func_02004e78
	.thumb_func
Func_02004e78:
	overlay_veneer Func_080c84a8
	.global Func_02004e80
	.thumb_func
Func_02004e80:
	overlay_veneer Func_080c84b0
	.global Func_02004e88
	.thumb_func
Func_02004e88:
	overlay_veneer Func_080c84b8
	.global Func_02004e90
	.thumb_func
Func_02004e90:
	overlay_veneer Func_080c84e0
	.global Func_02004e98
	.thumb_func
Func_02004e98:
	overlay_veneer Func_080c8518
	.global Func_02004ea0
	.thumb_func
Func_02004ea0:
	overlay_veneer Func_080c8570
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
	overlay_veneer Func_080c8690
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
	overlay_veneer Func_080c8778
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
	overlay_veneer Func_080c87e0
	.global Func_02004f28
	.thumb_func
Func_02004f28:
	overlay_veneer Func_080c88c8
	.global Func_02004f30
	.thumb_func
Func_02004f30:
	overlay_veneer Audio_PlayCue
