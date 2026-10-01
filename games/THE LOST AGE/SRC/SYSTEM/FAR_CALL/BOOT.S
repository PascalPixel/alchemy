@ Far-call stub table after the cartridge header at 0x080000c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global WaitFramesFar
	.thumb_func
WaitFramesFar:
	overlay_veneer WaitFrames
	.global Func_080000c8
	.thumb_func
Func_080000c8:
	overlay_veneer Scheduler_ResetTaskTable
	.global Scheduler_AddOrUpdateCallbackFar
	.thumb_func
Scheduler_AddOrUpdateCallbackFar:
	overlay_veneer Scheduler_AddOrUpdateCallback
	.global Func_080000d8
	.thumb_func
Func_080000d8:
	overlay_veneer Scheduler_RemoveCallback
	.global Func_080000e0
	.thumb_func
Func_080000e0:
	overlay_veneer Scheduler_EnableCallbacks
	.global Func_080000e8
	.thumb_func
Func_080000e8:
	overlay_veneer Scheduler_DisableCallbacks
	.global Func_080000f0
	.thumb_func
Func_080000f0:
	overlay_veneer Func_080147d8
	.global Func_080000f8
	.thumb_func
Func_080000f8:
	overlay_veneer Random16
	.global ArcTan2Far
	.thumb_func
ArcTan2Far:
	overlay_veneer ArcTan2
	.global Math_IntegerSqrtFar
	.thumb_func
Math_IntegerSqrtFar:
	overlay_veneer Math_IntegerSqrt
	.global Func_08000110
	.thumb_func
Func_08000110:
	overlay_veneer Func_080149e0
	.global Math_SineFar
	.thumb_func
Math_SineFar:
	overlay_veneer Trig_Sin
	.global Math_CosineFar
	.thumb_func
Math_CosineFar:
	overlay_veneer Trig_Cos
	.global Func_08000128
	.thumb_func
Func_08000128:
	overlay_veneer Vector_AddPolarOffset
	.global Func_08000130
	.thumb_func
Func_08000130:
	overlay_veneer Func_08013438
	.global Func_08000138
	.thumb_func
Func_08000138:
	overlay_veneer Func_08014c6c
	.global Func_08000140
	.thumb_func
Func_08000140:
	overlay_veneer Runtime_AllocateHeapBlock
	.global Runtime_AllocateBlockFar
	.thumb_func
Runtime_AllocateBlockFar:
	overlay_veneer Runtime_AllocateBlock
	.global Runtime_ReleaseHeapBlockFar
	.thumb_func
Runtime_ReleaseHeapBlockFar:
	overlay_veneer Runtime_ReleaseHeapBlock
	.global Func_08000158
	.thumb_func
Func_08000158:
	overlay_veneer Func_08014ca0
	.global Func_08000160
	.thumb_func
Func_08000160:
	overlay_veneer Func_08014cb0
	.global Runtime_BumpAllocateFar
	.thumb_func
Runtime_BumpAllocateFar:
	overlay_veneer Runtime_BumpAllocate
	.global Runtime_BumpAllocateAlternatePoolFar
	.thumb_func
Runtime_BumpAllocateAlternatePoolFar:
	overlay_veneer Runtime_BumpAllocateAlternatePool
	.global Sys_FreeFar
	.thumb_func
Sys_FreeFar:
	overlay_veneer Sys_Free
	.global Func_08000180
	.thumb_func
Func_08000180:
	overlay_veneer Func_08014ae0
	.global Func_08000188
	.thumb_func
Func_08000188:
	overlay_veneer Func_08014b30
	.global Func_08000190
	.thumb_func
Func_08000190:
	overlay_veneer Func_08014b50
	.global Func_08000198
	.thumb_func
Func_08000198:
	overlay_veneer Func_08014aa8
	.global Func_080001a0
	.thumb_func
Func_080001a0:
	overlay_veneer Func_0801591c
	.global Func_080001a8
	.thumb_func
Func_080001a8:
	overlay_veneer Resource_DecodeType01
	.global Func_080001b0
	.thumb_func
Func_080001b0:
	overlay_veneer Func_08014368
	.global Resource_ResetEntryFar
	.thumb_func
Resource_ResetEntryFar:
	overlay_veneer Resource_ResetEntry
	.global Resource_ActivateEntryFar
	.thumb_func
Resource_ActivateEntryFar:
	overlay_veneer Resource_ActivateEntry
	.global VramBlock_LoadCachedFar
	.thumb_func
VramBlock_LoadCachedFar:
	overlay_veneer VramBlock_LoadCached
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	overlay_veneer Resource_FindFreeEntry
	.global Func_080001d8
	.thumb_func
Func_080001d8:
	overlay_veneer Func_08014b70
	.global AffineMatrix_BuildForEffectFar
	.thumb_func
AffineMatrix_BuildForEffectFar:
	overlay_veneer AffineMatrix_BuildForEffect
	.global Func_080001e8
	.thumb_func
Func_080001e8:
	overlay_veneer Func_080140d8
	.global Func_080001f0
	.thumb_func
Func_080001f0:
	overlay_veneer Func_08014bac
	.global Func_080001f8
	.thumb_func
Func_080001f8:
	overlay_veneer Func_08014c4c
	.global Func_08000200
	.thumb_func
Func_08000200:
	overlay_veneer Func_08014de4
	.global Func_08000208
	.thumb_func
Func_08000208:
	overlay_veneer Func_08014ee0
	.global Func_08000210
	.thumb_func
Func_08000210:
	overlay_veneer Func_08014e38
	.global Func_08000218
	.thumb_func
Func_08000218:
	overlay_veneer Func_08014ea8
	.global Func_08000220
	.thumb_func
Func_08000220:
	overlay_veneer Func_08014efc
	.global SceneTransform_ApplyPitchFar
	.thumb_func
SceneTransform_ApplyPitchFar:
	overlay_veneer SceneTransform_ApplyPitch
	.global Func_08000230
	.thumb_func
Func_08000230:
	overlay_veneer Func_08015068
	.global Func_08000238
	.thumb_func
Func_08000238:
	overlay_veneer Func_080150e4
	.global SceneTransform_ApplyPositionFar
	.thumb_func
SceneTransform_ApplyPositionFar:
	overlay_veneer SceneTransform_ApplyPosition
	.global Func_08000248
	.thumb_func
Func_08000248:
	overlay_veneer Func_080151ac
	.global Func_08000250
	.thumb_func
Func_08000250:
	overlay_veneer Func_0801524c
	.global Func_08000258
	.thumb_func
Func_08000258:
	overlay_veneer Func_08015384
	.global Graphics_PrepareTransferInIwramWorkFar
	.thumb_func
Graphics_PrepareTransferInIwramWorkFar:
	overlay_veneer Graphics_PrepareTransferInIwramWork
	.global Render_ProjectPointFar
	.thumb_func
Render_ProjectPointFar:
	overlay_veneer Render_ProjectPoint
	.global Func_08000270
	.thumb_func
Func_08000270:
	overlay_veneer Func_08015714
	.global Camera_StoreSceneParametersFar
	.thumb_func
Camera_StoreSceneParametersFar:
	overlay_veneer Camera_StoreSceneParameters
	.global Func_08000280
	.thumb_func
Func_08000280:
	overlay_veneer Func_080132d0
	.global Runtime_ConstantZeroResultFar
	.thumb_func
Runtime_ConstantZeroResultFar:
	overlay_veneer Runtime_ConstantZeroResult
	.global Resource_GetTableEntryFar
	.thumb_func
Resource_GetTableEntryFar:
	overlay_veneer Resource_GetTableEntry
	.global Func_08000298
	.thumb_func
Func_08000298:
	overlay_veneer Func_080132fc
	.global Resource_LoadCodeFar
	.thumb_func
Resource_LoadCodeFar:
	overlay_veneer Resource_LoadCode
	.global Blend_SetDarkenTarget16Far
	.thumb_func
Blend_SetDarkenTarget16Far:
	overlay_veneer Blend_SetDarkenTarget16
	.global Func_080002b0
	.thumb_func
Func_080002b0:
	overlay_veneer Func_08013eb4
	.global Func_080002b8
	.thumb_func
Func_080002b8:
	overlay_veneer Func_08013ef8
	.global Func_080002c0
	.thumb_func
Func_080002c0:
	overlay_veneer Func_08013f3c
	.global Func_080002c8
	.thumb_func
Func_080002c8:
	overlay_veneer Func_08013f80
	.global Blend_WaitForTransitionFar
	.thumb_func
Blend_WaitForTransitionFar:
	overlay_veneer Blend_WaitForTransition
	.global Func_080002d8
	.thumb_func
Func_080002d8:
	overlay_veneer Func_080132b8
	.global Func_080002e0
	.thumb_func
Func_080002e0:
	overlay_veneer Func_080132bc
	.global Func_080002e8
	.thumb_func
Func_080002e8:
	overlay_veneer Func_080132c0
	.global Func_080002f0
	.thumb_func
Func_080002f0:
	overlay_veneer Func_080132c4
	.global Func_080002f8
	.thumb_func
Func_080002f8:
	overlay_veneer Func_080132c8
	.global Func_08000300
	.thumb_func
Func_08000300:
	overlay_veneer Func_08016180
	.global Func_08000308
	.thumb_func
Func_08000308:
	overlay_veneer Func_08016348
	.global Func_08000310
	.thumb_func
Func_08000310:
	overlay_veneer Func_08013b68
	.global QueueIoWriteDelay2Far
	.thumb_func
QueueIoWriteDelay2Far:
	overlay_veneer QueueIoWriteDelay2
	.global Func_08000320
	.thumb_func
Func_08000320:
	overlay_veneer Func_08013be0
	.global Func_08000328
	.thumb_func
Func_08000328:
	overlay_veneer Func_08013c1c
	.global Func_08000330
	.thumb_func
Func_08000330:
	overlay_veneer Func_08013c58
	.global Func_08000338
	.thumb_func
Func_08000338:
	overlay_veneer Func_08013c94
	.global Func_08000340
	.thumb_func
Func_08000340:
	overlay_veneer Func_08013cd0
	.global Func_08000348
	.thumb_func
Func_08000348:
	overlay_veneer Func_08013d0c
	.global Func_08000350
	.thumb_func
Func_08000350:
	overlay_veneer Func_08013d48
	.global Runtime_SetMainState19Far
	.thumb_func
Runtime_SetMainState19Far:
	overlay_veneer Runtime_SetMainState19
	.global Func_08000360
	.thumb_func
Func_08000360:
	overlay_veneer Func_080156f8
	.global Func_08000368
	.thumb_func
Func_08000368:
	overlay_veneer Func_080167d8
	.global Func_08000370
	.thumb_func
Func_08000370:
	overlay_veneer Func_080167ac
	.global Sound_LoadPresetParametersFar
	.thumb_func
Sound_LoadPresetParametersFar:
	overlay_veneer Sound_LoadPresetParameters
	.global Func_08000380
	.thumb_func
Func_08000380:
	overlay_veneer Func_0801680c
	.global Party_CheckFar
	.thumb_func
Party_CheckFar:
	overlay_veneer Party_Check
	.global Func_08000390
	.thumb_func
Func_08000390:
	overlay_veneer Func_080168a0
	.global SerialRuntime_WaitForTransferBFar
	.thumb_func
SerialRuntime_WaitForTransferBFar:
	overlay_veneer SerialRuntime_WaitForTransferB
	.global Func_080003a0
	.thumb_func
Func_080003a0:
	overlay_veneer Func_080168f8
	.global Func_080003a8
	.thumb_func
Func_080003a8:
	overlay_veneer Func_0801692c
	.global Func_080003b0
	.thumb_func
Func_080003b0:
	overlay_veneer Func_08016990
	.global Func_080003b8
	.thumb_func
Func_080003b8:
	overlay_veneer Scheduler_SetCallbackMask
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	overlay_veneer Owner_GetState
	.global GameFlag_TestFar
	.thumb_func
GameFlag_TestFar:
	overlay_veneer GameFlag_Test
	.global GameFlag_SetBitFar
	.thumb_func
GameFlag_SetBitFar:
	overlay_veneer GameFlag_SetBit
	.global GameFlag_ClearBitFar
	.thumb_func
GameFlag_ClearBitFar:
	overlay_veneer GameFlag_ClearBit
	.global Func_080003e0
	.thumb_func
Func_080003e0:
	overlay_veneer Func_08016d34
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer GameFlag_GetByte
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer GameFlag_SetByte
	.global Func_080003f8
	.thumb_func
Func_080003f8:
	overlay_veneer Func_08016d7c
	.global Func_08000400
	.thumb_func
Func_08000400:
	overlay_veneer Func_08016d98
	.global GameFlag_GetNibbleFar
	.thumb_func
GameFlag_GetNibbleFar:
	overlay_veneer GameFlag_GetNibble
	.global Func_08000410
	.thumb_func
Func_08000410:
	overlay_veneer Func_08016dd0
	.global Func_08000418
	.thumb_func
Func_08000418:
	overlay_veneer Scheduler_FindCallback
	.global Func_08000420
	.thumb_func
Func_08000420:
	overlay_veneer Func_08014220
