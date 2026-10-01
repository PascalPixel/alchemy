@ tla-de's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Resource_Data000
Resource_Data000:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00000630, "ax"
	.incbin "baserom.gba", 0x00000630, 0x00000088
	.section .rom.000019f4, "ax"
	.incbin "baserom.gba", 0x000019f4, 0x00000020
	.section .rom.00001c90, "ax"
	.incbin "baserom.gba", 0x00001c90, 0x00000080
	.section .rom.00013164, "ax"
	.global Sys_Free
	.type Sys_Free, %function
	.thumb_func
Sys_Free:
	.incbin "baserom.gba", 0x00013164, 0x00000038
	.section .rom.0001319c, "ax"
	.global Func_0801319c
	.type Func_0801319c, %function
	.thumb_func
Func_0801319c:
	.incbin "baserom.gba", 0x0001319c, 0x00000148
	.section .rom.000132e4, "ax"
	.global Func_080132b8
	.type Func_080132b8, %function
	.thumb_func
Func_080132b8:
	.incbin "baserom.gba", 0x000132e4, 0x00000004
	.section .rom.000132e8, "ax"
	.global Func_080132bc
	.type Func_080132bc, %function
	.thumb_func
Func_080132bc:
	.incbin "baserom.gba", 0x000132e8, 0x00000004
	.section .rom.000132ec, "ax"
	.global Func_080132c0
	.type Func_080132c0, %function
	.thumb_func
Func_080132c0:
	.incbin "baserom.gba", 0x000132ec, 0x00000004
	.section .rom.000132f0, "ax"
	.global Func_080132c4
	.type Func_080132c4, %function
	.thumb_func
Func_080132c4:
	.incbin "baserom.gba", 0x000132f0, 0x00000004
	.section .rom.000132f4, "ax"
	.global Func_080132c8
	.type Func_080132c8, %function
	.thumb_func
Func_080132c8:
	.incbin "baserom.gba", 0x000132f4, 0x00000004
	.section .rom.000132fc, "ax"
	.global Func_080132d0
	.type Func_080132d0, %function
	.thumb_func
Func_080132d0:
	.incbin "baserom.gba", 0x000132fc, 0x0000002c
	.section .rom.00013328, "ax"
	.global Func_080132fc
	.type Func_080132fc, %function
	.thumb_func
Func_080132fc:
	.incbin "baserom.gba", 0x00013328, 0x00000004
	.section .rom.00013338, "ax"
	.incbin "baserom.gba", 0x00013338, 0x00000060
	.section .rom.00013398, "ax"
	.global Resource_LoadCode
	.type Resource_LoadCode, %function
	.thumb_func
Resource_LoadCode:
	.incbin "baserom.gba", 0x00013398, 0x000000cc
	.section .rom.00013464, "ax"
	.global Func_08013438
	.type Func_08013438, %function
	.thumb_func
Func_08013438:
	.incbin "baserom.gba", 0x00013464, 0x00000128
	.section .rom.0001358c, "ax"
	.global WaitFrames
	.type WaitFrames, %function
	.thumb_func
WaitFrames:
	.incbin "baserom.gba", 0x0001358c, 0x00000348
	.section .rom.000138d4, "ax"
	.global Runtime_SetMainState19
	.type Runtime_SetMainState19, %function
	.thumb_func
Runtime_SetMainState19:
	.incbin "baserom.gba", 0x000138d4, 0x00000288
	.section .rom.00013b5c, "ax"
	.global Sound_LoadPresetParameters
	.type Sound_LoadPresetParameters, %function
	.thumb_func
Sound_LoadPresetParameters:
	.incbin "baserom.gba", 0x00013b5c, 0x00000038
	.section .rom.00013b94, "ax"
	.global Func_08013b68
	.type Func_08013b68, %function
	.thumb_func
Func_08013b68:
	.incbin "baserom.gba", 0x00013b94, 0x0000003c
	.section .rom.00013bd0, "ax"
	.global QueueIoWriteDelay2
	.type QueueIoWriteDelay2, %function
	.thumb_func
QueueIoWriteDelay2:
	.incbin "baserom.gba", 0x00013bd0, 0x0000003c
	.section .rom.00013c0c, "ax"
	.global Func_08013be0
	.type Func_08013be0, %function
	.thumb_func
Func_08013be0:
	.incbin "baserom.gba", 0x00013c0c, 0x0000003c
	.section .rom.00013c48, "ax"
	.global Func_08013c1c
	.type Func_08013c1c, %function
	.thumb_func
Func_08013c1c:
	.incbin "baserom.gba", 0x00013c48, 0x0000003c
	.section .rom.00013c84, "ax"
	.global Func_08013c58
	.type Func_08013c58, %function
	.thumb_func
Func_08013c58:
	.incbin "baserom.gba", 0x00013c84, 0x0000003c
	.section .rom.00013cc0, "ax"
	.global Func_08013c94
	.type Func_08013c94, %function
	.thumb_func
Func_08013c94:
	.incbin "baserom.gba", 0x00013cc0, 0x0000003c
	.section .rom.00013cfc, "ax"
	.global Func_08013cd0
	.type Func_08013cd0, %function
	.thumb_func
Func_08013cd0:
	.incbin "baserom.gba", 0x00013cfc, 0x0000003c
	.section .rom.00013d38, "ax"
	.global Func_08013d0c
	.type Func_08013d0c, %function
	.thumb_func
Func_08013d0c:
	.incbin "baserom.gba", 0x00013d38, 0x0000003c
	.section .rom.00013d74, "ax"
	.global Func_08013d48
	.type Func_08013d48, %function
	.thumb_func
Func_08013d48:
	.incbin "baserom.gba", 0x00013d74, 0x00000128
	.section .rom.00013e9c, "ax"
	.global Blend_SetDarkenTarget16
	.type Blend_SetDarkenTarget16, %function
	.thumb_func
Blend_SetDarkenTarget16:
	.incbin "baserom.gba", 0x00013e9c, 0x00000044
	.section .rom.00013ee0, "ax"
	.global Func_08013eb4
	.type Func_08013eb4, %function
	.thumb_func
Func_08013eb4:
	.incbin "baserom.gba", 0x00013ee0, 0x00000044
	.section .rom.00013f24, "ax"
	.global Func_08013ef8
	.type Func_08013ef8, %function
	.thumb_func
Func_08013ef8:
	.incbin "baserom.gba", 0x00013f24, 0x00000044
	.section .rom.00013f68, "ax"
	.global Func_08013f3c
	.type Func_08013f3c, %function
	.thumb_func
Func_08013f3c:
	.incbin "baserom.gba", 0x00013f68, 0x00000044
	.section .rom.00013fac, "ax"
	.global Func_08013f80
	.type Func_08013f80, %function
	.thumb_func
Func_08013f80:
	.incbin "baserom.gba", 0x00013fac, 0x0000005c
	.section .rom.00014028, "ax"
	.incbin "baserom.gba", 0x00014028, 0x00000020
	.section .rom.00014048, "ax"
	.global AffineMatrix_BuildForEffect
	.type AffineMatrix_BuildForEffect, %function
	.thumb_func
AffineMatrix_BuildForEffect:
	.incbin "baserom.gba", 0x00014048, 0x000000bc
	.section .rom.00014104, "ax"
	.global Func_080140d8
	.type Func_080140d8, %function
	.thumb_func
Func_080140d8:
	.incbin "baserom.gba", 0x00014104, 0x00000148
	.section .rom.0001424c, "ax"
	.global Func_08014220
	.type Func_08014220, %function
	.thumb_func
Func_08014220:
	.incbin "baserom.gba", 0x0001424c, 0x00000020
	.section .rom.0001426c, "ax"
	.global Resource_ClearSlotReferences
	.type Resource_ClearSlotReferences, %function
	.thumb_func
Resource_ClearSlotReferences:
	.incbin "baserom.gba", 0x0001426c, 0x00000034
	.section .rom.000142a0, "ax"
	.global Resource_ResetEntry
	.type Resource_ResetEntry, %function
	.thumb_func
Resource_ResetEntry:
	.incbin "baserom.gba", 0x000142a0, 0x00000038
	.section .rom.00014300, "ax"
	.global VramBlock_LoadCached
	.type VramBlock_LoadCached, %function
	.thumb_func
VramBlock_LoadCached:
	.incbin "baserom.gba", 0x00014300, 0x00000094
	.section .rom.00014394, "ax"
	.global Func_08014368
	.type Func_08014368, %function
	.thumb_func
Func_08014368:
	.incbin "baserom.gba", 0x00014394, 0x00000044
	.section .rom.000143d8, "ax"
	.global Resource_FindFreeEntry
	.type Resource_FindFreeEntry, %function
	.thumb_func
Resource_FindFreeEntry:
	.incbin "baserom.gba", 0x000143d8, 0x00000034
	.section .rom.00014422, "ax"
	.incbin "baserom.gba", 0x00014422, 0x00000002
	.section .rom.00014424, "ax"
	.global Resource_GetBuffer
	.type Resource_GetBuffer, %function
	.thumb_func
Resource_GetBuffer:
	.incbin "baserom.gba", 0x00014424, 0x000000c8
	.section .rom.00014748, "ax"
	.global Scheduler_SetCallbackMask
	.type Scheduler_SetCallbackMask, %function
	.thumb_func
Scheduler_SetCallbackMask:
	.incbin "baserom.gba", 0x00014748, 0x00000040
	.section .rom.00014788, "ax"
	.global Scheduler_DisableCallbacks
	.type Scheduler_DisableCallbacks, %function
	.thumb_func
Scheduler_DisableCallbacks:
	.incbin "baserom.gba", 0x00014788, 0x00000040
	.section .rom.000147c8, "ax"
	.global Scheduler_DisableOverlayCallbacks
	.type Scheduler_DisableOverlayCallbacks, %function
	.thumb_func
Scheduler_DisableOverlayCallbacks:
	.incbin "baserom.gba", 0x000147c8, 0x0000003c
	.section .rom.00014804, "ax"
	.global Func_080147d8
	.type Func_080147d8, %function
	.thumb_func
Func_080147d8:
	.incbin "baserom.gba", 0x00014804, 0x000000a0
	.section .rom.000148a4, "ax"
	.global Random16
	.type Random16, %function
	.thumb_func
Random16:
	.incbin "baserom.gba", 0x000148a4, 0x00000024
	.section .rom.000148c8, "ax"
	.global Vector_AddPolarOffset
	.type Vector_AddPolarOffset, %function
	.thumb_func
Vector_AddPolarOffset:
	.incbin "baserom.gba", 0x000148c8, 0x0000004c
	.section .rom.00014914, "ax"
	.global ArcTan2
	.type ArcTan2, %function
	.thumb_func
ArcTan2:
	.incbin "baserom.gba", 0x00014914, 0x000000cc
	.section .rom.00014a0c, "ax"
	.global Func_080149e0
	.type Func_080149e0, %function
	.thumb_func
Func_080149e0:
	.incbin "baserom.gba", 0x00014a0c, 0x00000044
	.section .rom.00014a50, "ax"
	.global Text_FormatSignedDecimalToWork
	.type Text_FormatSignedDecimalToWork, %function
	.thumb_func
Text_FormatSignedDecimalToWork:
	.incbin "baserom.gba", 0x00014a50, 0x00000084
	.section .rom.00014ad4, "ax"
	.global Func_08014aa8
	.type Func_08014aa8, %function
	.thumb_func
Func_08014aa8:
	.incbin "baserom.gba", 0x00014ad4, 0x00000038
	.section .rom.00014b0c, "ax"
	.global Func_08014ae0
	.type Func_08014ae0, %function
	.thumb_func
Func_08014ae0:
	.incbin "baserom.gba", 0x00014b0c, 0x00000050
	.section .rom.00014b5c, "ax"
	.global Func_08014b30
	.type Func_08014b30, %function
	.thumb_func
Func_08014b30:
	.incbin "baserom.gba", 0x00014b5c, 0x00000020
	.section .rom.00014b7c, "ax"
	.global Func_08014b50
	.type Func_08014b50, %function
	.thumb_func
Func_08014b50:
	.incbin "baserom.gba", 0x00014b7c, 0x00000020
	.section .rom.00014b9c, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b9c, 0x0000003c
	.section .rom.00014bd8, "ax"
	.global Func_08014bac
	.type Func_08014bac, %function
	.thumb_func
Func_08014bac:
	.incbin "baserom.gba", 0x00014bd8, 0x000000a0
	.section .rom.00014c78, "ax"
	.global Func_08014c4c
	.type Func_08014c4c, %function
	.thumb_func
Func_08014c4c:
	.incbin "baserom.gba", 0x00014c78, 0x00000020
	.section .rom.00014c98, "ax"
	.global Func_08014c6c
	.type Func_08014c6c, %function
	.thumb_func
Func_08014c6c:
	.incbin "baserom.gba", 0x00014c98, 0x00000034
	.section .rom.00014ccc, "ax"
	.global Func_08014ca0
	.type Func_08014ca0, %function
	.thumb_func
Func_08014ca0:
	.incbin "baserom.gba", 0x00014ccc, 0x00000010
	.section .rom.00014cdc, "ax"
	.global Func_08014cb0
	.type Func_08014cb0, %function
	.thumb_func
Func_08014cb0:
	.incbin "baserom.gba", 0x00014cdc, 0x00000010
	.section .rom.00014cec, "ax"
	.global Runtime_AllocateHeapBlock
	.type Runtime_AllocateHeapBlock, %function
	.thumb_func
Runtime_AllocateHeapBlock:
	.incbin "baserom.gba", 0x00014cec, 0x00000040
	.section .rom.00014d2c, "ax"
	.global Runtime_AllocateBlock
	.type Runtime_AllocateBlock, %function
	.thumb_func
Runtime_AllocateBlock:
	.incbin "baserom.gba", 0x00014d2c, 0x00000078
	.section .rom.00014da4, "ax"
	.global Runtime_BumpAllocate
	.type Runtime_BumpAllocate, %function
	.thumb_func
Runtime_BumpAllocate:
	.incbin "baserom.gba", 0x00014da4, 0x00000034
	.section .rom.00014dd8, "ax"
	.global Runtime_BumpAllocateAlternatePool
	.type Runtime_BumpAllocateAlternatePool, %function
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	.incbin "baserom.gba", 0x00014dd8, 0x00000038
	.section .rom.00014e10, "ax"
	.global Func_08014de4
	.type Func_08014de4, %function
	.thumb_func
Func_08014de4:
	.incbin "baserom.gba", 0x00014e10, 0x00000054
	.section .rom.00014e64, "ax"
	.global Func_08014e38
	.type Func_08014e38, %function
	.thumb_func
Func_08014e38:
	.incbin "baserom.gba", 0x00014e64, 0x00000070
	.section .rom.00014ed4, "ax"
	.global Func_08014ea8
	.type Func_08014ea8, %function
	.thumb_func
Func_08014ea8:
	.incbin "baserom.gba", 0x00014ed4, 0x00000038
	.section .rom.00014f0c, "ax"
	.global Func_08014ee0
	.type Func_08014ee0, %function
	.thumb_func
Func_08014ee0:
	.incbin "baserom.gba", 0x00014f0c, 0x0000001c
	.section .rom.00014f28, "ax"
	.global Func_08014efc
	.type Func_08014efc, %function
	.thumb_func
Func_08014efc:
	.incbin "baserom.gba", 0x00014f28, 0x00000128
	.section .rom.00015050, "ax"
	.global SceneTransform_ApplyPitch
	.type SceneTransform_ApplyPitch, %function
	.thumb_func
SceneTransform_ApplyPitch:
	.incbin "baserom.gba", 0x00015050, 0x00000044
	.section .rom.00015094, "ax"
	.global Func_08015068
	.type Func_08015068, %function
	.thumb_func
Func_08015068:
	.incbin "baserom.gba", 0x00015094, 0x0000007c
	.section .rom.00015110, "ax"
	.global Func_080150e4
	.type Func_080150e4, %function
	.thumb_func
Func_080150e4:
	.incbin "baserom.gba", 0x00015110, 0x00000044
	.section .rom.00015154, "ax"
	.global SceneTransform_ApplyPosition
	.type SceneTransform_ApplyPosition, %function
	.thumb_func
SceneTransform_ApplyPosition:
	.incbin "baserom.gba", 0x00015154, 0x00000084
	.section .rom.000151d8, "ax"
	.global Func_080151ac
	.type Func_080151ac, %function
	.thumb_func
Func_080151ac:
	.incbin "baserom.gba", 0x000151d8, 0x000000a0
	.section .rom.00015278, "ax"
	.global Func_0801524c
	.type Func_0801524c, %function
	.thumb_func
Func_0801524c:
	.incbin "baserom.gba", 0x00015278, 0x00000138
	.section .rom.000153b0, "ax"
	.global Func_08015384
	.type Func_08015384, %function
	.thumb_func
Func_08015384:
	.incbin "baserom.gba", 0x000153b0, 0x00000364
	.section .rom.00015714, "ax"
	.global Graphics_PrepareTransferInIwramWork
	.type Graphics_PrepareTransferInIwramWork, %function
	.thumb_func
Graphics_PrepareTransferInIwramWork:
	.incbin "baserom.gba", 0x00015714, 0x00000010
	.section .rom.00015724, "ax"
	.global Func_080156f8
	.type Func_080156f8, %function
	.thumb_func
Func_080156f8:
	.incbin "baserom.gba", 0x00015724, 0x0000001c
	.section .rom.00015740, "ax"
	.global Func_08015714
	.type Func_08015714, %function
	.thumb_func
Func_08015714:
	.incbin "baserom.gba", 0x00015740, 0x00000054
	.section .rom.000157a4, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x000157a4, 0x00000104
	.section .rom.000158a8, "ax"
	.global Func_0801587c
	.type Func_0801587c, %function
	.thumb_func
Func_0801587c:
	.incbin "baserom.gba", 0x000158a8, 0x000000a0
	.section .rom.00015948, "ax"
	.global Func_0801591c
	.type Func_0801591c, %function
	.thumb_func
Func_0801591c:
	.incbin "baserom.gba", 0x00015948, 0x000006e0
	.section .rom.00016048, "ax"
	.incbin "baserom.gba", 0x00016048, 0x00000134
	.section .rom.000161ac, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x000161ac, 0x00000170
	.section .rom.00016372, "ax"
	.incbin "baserom.gba", 0x00016372, 0x00000002
	.section .rom.00016374, "ax"
	.global Func_08016348
	.type Func_08016348, %function
	.thumb_func
Func_08016348:
	.incbin "baserom.gba", 0x00016374, 0x00000464
	.section .rom.000167d8, "ax"
	.global Func_080167ac
	.type Func_080167ac, %function
	.thumb_func
Func_080167ac:
	.incbin "baserom.gba", 0x000167d8, 0x0000002c
	.section .rom.00016804, "ax"
	.global Func_080167d8
	.type Func_080167d8, %function
	.thumb_func
Func_080167d8:
	.incbin "baserom.gba", 0x00016804, 0x00000034
	.section .rom.00016838, "ax"
	.global Func_0801680c
	.type Func_0801680c, %function
	.thumb_func
Func_0801680c:
	.incbin "baserom.gba", 0x00016838, 0x00000048
	.section .rom.00016880, "ax"
	.global Party_Check
	.type Party_Check, %function
	.thumb_func
Party_Check:
	.incbin "baserom.gba", 0x00016880, 0x0000004c
	.section .rom.000168cc, "ax"
	.global Func_080168a0
	.type Func_080168a0, %function
	.thumb_func
Func_080168a0:
	.incbin "baserom.gba", 0x000168cc, 0x0000002c
	.section .rom.000168f8, "ax"
	.global SerialRuntime_WaitForTransferB
	.type SerialRuntime_WaitForTransferB, %function
	.thumb_func
SerialRuntime_WaitForTransferB:
	.incbin "baserom.gba", 0x000168f8, 0x0000002c
	.section .rom.00016924, "ax"
	.global Func_080168f8
	.type Func_080168f8, %function
	.thumb_func
Func_080168f8:
	.incbin "baserom.gba", 0x00016924, 0x00000034
	.section .rom.00016958, "ax"
	.global Func_0801692c
	.type Func_0801692c, %function
	.thumb_func
Func_0801692c:
	.incbin "baserom.gba", 0x00016958, 0x00000064
	.section .rom.000169bc, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x000169bc, 0x00000314
	.section .rom.00016cd0, "ax"
	.global Owner_GetState
	.type Owner_GetState, %function
	.thumb_func
Owner_GetState:
	.incbin "baserom.gba", 0x00016cd0, 0x00000040
	.section .rom.00016d28, "ax"
	.global GameFlag_SetBit
	.type GameFlag_SetBit, %function
	.thumb_func
GameFlag_SetBit:
	.incbin "baserom.gba", 0x00016d28, 0x0000001c
	.section .rom.00016d44, "ax"
	.global GameFlag_ClearBit
	.type GameFlag_ClearBit, %function
	.thumb_func
GameFlag_ClearBit:
	.incbin "baserom.gba", 0x00016d44, 0x0000001c
	.section .rom.00016d60, "ax"
	.global Func_08016d34
	.type Func_08016d34, %function
	.thumb_func
Func_08016d34:
	.incbin "baserom.gba", 0x00016d60, 0x00000028
	.section .rom.00016da8, "ax"
	.global Func_08016d7c
	.type Func_08016d7c, %function
	.thumb_func
Func_08016d7c:
	.incbin "baserom.gba", 0x00016da8, 0x0000001c
	.section .rom.00016dc4, "ax"
	.global Func_08016d98
	.type Func_08016d98, %function
	.thumb_func
Func_08016d98:
	.incbin "baserom.gba", 0x00016dc4, 0x0000001c
	.section .rom.00016dfc, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dfc, 0x00000028
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
	.section .rom.000178e0, "ax"
	.incbin "baserom.gba", 0x000178e0, 0x00000434
	.section .rom.00017d34, "ax"
	.incbin "baserom.gba", 0x00017d34, 0x0000005c
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00017d90, 0x00000014
	.section .rom.00017dec, "ax"
	.incbin "baserom.gba", 0x00017dec, 0x00000024
	.section .rom.00017e28, "ax"
	.incbin "baserom.gba", 0x00017e28, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00017e40, 0x00000058
	.section .rom.00017ebc, "ax"
	.incbin "baserom.gba", 0x00017ebc, 0x0000008c
	.section .rom.00017f50, "ax"
	.incbin "baserom.gba", 0x00017f50, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00017f68, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00017f94, 0x0000002c
	.section .rom.00017fe8, "ax"
	.incbin "baserom.gba", 0x00017fe8, 0x00008018
	.section .rom.00020010, "ax"
	.incbin "baserom.gba", 0x00020010, 0x00000030
	.section .rom.00020060, "ax"
	.incbin "baserom.gba", 0x00020060, 0x00000020
	.section .rom.000200c0, "ax"
	.incbin "baserom.gba", 0x000200c0, 0x00000028
	.section .rom.00020180, "ax"
	.incbin "baserom.gba", 0x00020180, 0x00000030
	.section .rom.000201f0, "ax"
	.incbin "baserom.gba", 0x000201f0, 0x00000020
	.section .rom.00020228, "ax"
	.incbin "baserom.gba", 0x00020228, 0x00000040
	.section .rom.00020308, "ax"
	.incbin "baserom.gba", 0x00020308, 0x00000030
	.section .rom.000203a8, "ax"
	.incbin "baserom.gba", 0x000203a8, 0x00000a00
	.section .rom.000212d8, "ax"
	.incbin "baserom.gba", 0x000212d8, 0x0000042c
	.section .rom.00021824, "ax"
	.incbin "baserom.gba", 0x00021824, 0x000000f4
	.section .rom.00021918, "ax"
	.global Resource_GetMetadataRecordFar
	.type Resource_GetMetadataRecordFar, %function
	.thumb_func
Resource_GetMetadataRecordFar:
	.incbin "baserom.gba", 0x00021918, 0x000000b4
	.section .rom.000219cc, "ax"
	.global Func_080237e8
	.type Func_080237e8, %function
	.thumb_func
Func_080237e8:
	.incbin "baserom.gba", 0x000219cc, 0x00000644
	.section .rom.00022010, "ax"
	.global Func_0802254c
	.type Func_0802254c, %function
	.thumb_func
Func_0802254c:
	.incbin "baserom.gba", 0x00022010, 0x00000790
	.section .rom.000227a0, "ax"
	.global Func_080227a0
	.type Func_080227a0, %function
	.thumb_func
Func_080227a0:
	.incbin "baserom.gba", 0x000227a0, 0x00000010
	.section .rom.000227b0, "ax"
	.global Animation_ApplyChildValuesToRecord
	.type Animation_ApplyChildValuesToRecord, %function
	.thumb_func
Animation_ApplyChildValuesToRecord:
	.incbin "baserom.gba", 0x000227b0, 0x00000030
	.section .rom.000227e0, "ax"
	.global Func_080227e0
	.type Func_080227e0, %function
	.thumb_func
Func_080227e0:
	.incbin "baserom.gba", 0x000227e0, 0x000001cc
	.section .rom.000229ac, "ax"
	.global InitializeAnimationObjects
	.type InitializeAnimationObjects, %function
	.thumb_func
InitializeAnimationObjects:
	.incbin "baserom.gba", 0x000229ac, 0x00000120
	.section .rom.00022acc, "ax"
	.global Func_08022acc
	.type Func_08022acc, %function
	.thumb_func
Func_08022acc:
	.incbin "baserom.gba", 0x00022acc, 0x000000e0
	.section .rom.00022bac, "ax"
	.global Animation_ApplyChildValue
	.type Animation_ApplyChildValue, %function
	.thumb_func
Animation_ApplyChildValue:
	.incbin "baserom.gba", 0x00022bac, 0x0000002c
	.section .rom.00022bd8, "ax"
	.global Func_08022bd8
	.type Func_08022bd8, %function
	.thumb_func
Func_08022bd8:
	.incbin "baserom.gba", 0x00022bd8, 0x000000a0
	.section .rom.00022c78, "ax"
	.global Func_0802da88
	.type Func_0802da88, %function
	.thumb_func
Func_0802da88:
	.incbin "baserom.gba", 0x00022c78, 0x0000000c
	.section .rom.00022c84, "ax"
	.global Func_08022c78
	.type Func_08022c78, %function
	.thumb_func
Func_08022c78:
	.incbin "baserom.gba", 0x00022c84, 0x000000bc
	.section .rom.00022d40, "ax"
	.global Render_ApplyProjectedPlacement
	.type Render_ApplyProjectedPlacement, %function
	.thumb_func
Render_ApplyProjectedPlacement:
	.incbin "baserom.gba", 0x00022d40, 0x00000150
	.section .rom.00022e90, "ax"
	.global Func_08022318
	.type Func_08022318, %function
	.thumb_func
Func_08022318:
	.incbin "baserom.gba", 0x00022e90, 0x00000048
	.section .rom.00022f22, "ax"
	.incbin "baserom.gba", 0x00022f22, 0x00000002
	.section .rom.00022f24, "ax"
	.global Func_0802d7b0
	.type Func_0802d7b0, %function
	.thumb_func
Func_0802d7b0:
	.incbin "baserom.gba", 0x00022f24, 0x00000040
	.section .rom.0002301c, "ax"
	.global Func_0802301c
	.type Func_0802301c, %function
	.thumb_func
Func_0802301c:
	.incbin "baserom.gba", 0x0002301c, 0x0000006c
	.section .rom.00023088, "ax"
	.global Func_08023088
	.type Func_08023088, %function
	.thumb_func
Func_08023088:
	.incbin "baserom.gba", 0x00023088, 0x00000058
	.section .rom.000230e0, "ax"
	.global Func_080230e0
	.type Func_080230e0, %function
	.thumb_func
Func_080230e0:
	.incbin "baserom.gba", 0x000230e0, 0x000000c4
	.section .rom.000231a4, "ax"
	.global Func_080231a4
	.type Func_080231a4, %function
	.thumb_func
Func_080231a4:
	.incbin "baserom.gba", 0x000231a4, 0x00000204
	.section .rom.000233a8, "ax"
	.global ObjectDispatch_Initialize
	.type ObjectDispatch_Initialize, %function
	.thumb_func
ObjectDispatch_Initialize:
	.incbin "baserom.gba", 0x000233a8, 0x00000028
	.section .rom.000233d0, "ax"
	.global ObjectDispatch_ApplyArgumentToChildren
	.type ObjectDispatch_ApplyArgumentToChildren, %function
	.thumb_func
ObjectDispatch_ApplyArgumentToChildren:
	.incbin "baserom.gba", 0x000233d0, 0x00000040
	.section .rom.00023410, "ax"
	.global ObjectDispatch_ApplyValueToChildren
	.type ObjectDispatch_ApplyValueToChildren, %function
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	.incbin "baserom.gba", 0x00023410, 0x00000040
	.section .rom.00023450, "ax"
	.global ObjectDispatch_ApplyPairToChildren
	.type ObjectDispatch_ApplyPairToChildren, %function
	.thumb_func
ObjectDispatch_ApplyPairToChildren:
	.incbin "baserom.gba", 0x00023450, 0x000000d4
	.section .rom.00023524, "ax"
	.global Func_08023524
	.type Func_08023524, %function
	.thumb_func
Func_08023524:
	.incbin "baserom.gba", 0x00023524, 0x000000d4
	.section .rom.0002367e, "ax"
	.incbin "baserom.gba", 0x0002367e, 0x00000002
	.section .rom.00023680, "ax"
	.global Animation_SetStateFlags
	.type Animation_SetStateFlags, %function
	.thumb_func
Animation_SetStateFlags:
	.incbin "baserom.gba", 0x00023680, 0x00000050
	.section .rom.000236e8, "ax"
	.global Func_080236e8
	.type Func_080236e8, %function
	.thumb_func
Func_080236e8:
	.incbin "baserom.gba", 0x000236e8, 0x00000044
	.section .rom.0002372c, "ax"
	.global Func_0802372c
	.type Func_0802372c, %function
	.thumb_func
Func_0802372c:
	.incbin "baserom.gba", 0x0002372c, 0x000000bc
	.section .rom.000237e8, "ax"
	.global Func_0802d71c
	.type Func_0802d71c, %function
	.thumb_func
Func_0802d71c:
	.incbin "baserom.gba", 0x000237e8, 0x0000072c
	.section .rom.00023f62, "ax"
	.incbin "baserom.gba", 0x00023f62, 0x00000002
	.section .rom.00023f64, "ax"
	.global Object_IsTargetUnset
	.type Object_IsTargetUnset, %function
	.thumb_func
Object_IsTargetUnset:
	.incbin "baserom.gba", 0x00023f64, 0x00000030
	.section .rom.00023f94, "ax"
	.global Func_08023f94
	.type Func_08023f94, %function
	.thumb_func
Func_08023f94:
	.incbin "baserom.gba", 0x00023f94, 0x00000788
	.section .rom.00024736, "ax"
	.incbin "baserom.gba", 0x00024736, 0x00000002
	.section .rom.00024738, "ax"
	.global Object_SetMoveTarget
	.type Object_SetMoveTarget, %function
	.thumb_func
Object_SetMoveTarget:
	.incbin "baserom.gba", 0x00024738, 0x00000518
	.section .rom.00024c6e, "ax"
	.incbin "baserom.gba", 0x00024c6e, 0x0000003e
	.section .rom.00024d14, "ax"
	.incbin "baserom.gba", 0x00024d14, 0x0000004c
	.section .rom.00024e3c, "ax"
	.incbin "baserom.gba", 0x00024e3c, 0x0000002c
	.section .rom.00024ea6, "ax"
	.incbin "baserom.gba", 0x00024ea6, 0x0000002e
	.section .rom.00024efe, "ax"
	.incbin "baserom.gba", 0x00024efe, 0x00000022
	.section .rom.00024f20, "ax"
	.global Func_08024f20
	.type Func_08024f20, %function
	.thumb_func
Func_08024f20:
	.incbin "baserom.gba", 0x00024f20, 0x00000060
	.section .rom.00024f80, "ax"
	.global Func_08024f80
	.type Func_08024f80, %function
	.thumb_func
Func_08024f80:
	.incbin "baserom.gba", 0x00024f80, 0x000001b4
	.section .rom.00025160, "ax"
	.incbin "baserom.gba", 0x00025160, 0x000009f8
	.section .rom.00025bb4, "ax"
	.incbin "baserom.gba", 0x00025bb4, 0x00000030
	.section .rom.00025c5c, "ax"
	.incbin "baserom.gba", 0x00025c5c, 0x00000030
	.section .rom.00025f9a, "ax"
	.incbin "baserom.gba", 0x00025f9a, 0x00000036
	.section .rom.00026320, "ax"
	.incbin "baserom.gba", 0x00026320, 0x00000c60
	.section .rom.00026f80, "ax"
	.global Func_0802db64
	.type Func_0802db64, %function
	.thumb_func
Func_0802db64:
	.incbin "baserom.gba", 0x00026f80, 0x00000048
	.section .rom.00026fc8, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x00026fc8, 0x00003564
	.section .rom.0002a52c, "ax"
	.global Func_0802a52c
	.type Func_0802a52c, %function
	.thumb_func
Func_0802a52c:
	.incbin "baserom.gba", 0x0002a52c, 0x00000024
	.section .rom.0002a5e4, "ax"
	.incbin "baserom.gba", 0x0002a5e4, 0x0000006c
	.section .rom.0002a650, "ax"
	.global Func_08022c84
	.type Func_08022c84, %function
	.thumb_func
Func_08022c84:
	.incbin "baserom.gba", 0x0002a650, 0x00000068
	.section .rom.0002a6b8, "ax"
	.global Func_0802a6b8
	.type Func_0802a6b8, %function
	.thumb_func
Func_0802a6b8:
	.incbin "baserom.gba", 0x0002a6b8, 0x000003bc
	.section .rom.0002aa74, "ax"
	.global Func_0802aa74
	.type Func_0802aa74, %function
	.thumb_func
Func_0802aa74:
	.incbin "baserom.gba", 0x0002aa74, 0x00000528
	.section .rom.0002af9c, "ax"
	.global Func_0802af9c
	.type Func_0802af9c, %function
	.thumb_func
Func_0802af9c:
	.incbin "baserom.gba", 0x0002af9c, 0x00000204
	.section .rom.0002b1a0, "ax"
	.global Func_0802b1a0
	.type Func_0802b1a0, %function
	.thumb_func
Func_0802b1a0:
	.incbin "baserom.gba", 0x0002b1a0, 0x00000134
	.section .rom.0002b2d4, "ax"
	.global Func_0802b2d4
	.type Func_0802b2d4, %function
	.thumb_func
Func_0802b2d4:
	.incbin "baserom.gba", 0x0002b2d4, 0x00000070
	.section .rom.0002b344, "ax"
	.global Func_0802cea0
	.type Func_0802cea0, %function
	.thumb_func
Func_0802cea0:
	.incbin "baserom.gba", 0x0002b344, 0x00000048
	.section .rom.0002b38c, "ax"
	.global Func_0802ce4c
	.type Func_0802ce4c, %function
	.thumb_func
Func_0802ce4c:
	.incbin "baserom.gba", 0x0002b38c, 0x00000294
	.section .rom.0002b620, "ax"
	.global Func_0802b620
	.type Func_0802b620, %function
	.thumb_func
Func_0802b620:
	.incbin "baserom.gba", 0x0002b620, 0x00000208
	.section .rom.0002b828, "ax"
	.global Func_08026f80
	.type Func_08026f80, %function
	.thumb_func
Func_08026f80:
	.incbin "baserom.gba", 0x0002b828, 0x00000170
	.section .rom.0002b998, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b998, 0x000004b4
	.section .rom.0002be4c, "ax"
	.global Func_0802be4c
	.type Func_0802be4c, %function
	.thumb_func
Func_0802be4c:
	.incbin "baserom.gba", 0x0002be4c, 0x000003f4
	.section .rom.0002c240, "ax"
	.global Func_0802c240
	.type Func_0802c240, %function
	.thumb_func
Func_0802c240:
	.incbin "baserom.gba", 0x0002c240, 0x00000298
	.section .rom.0002c4d8, "ax"
	.global Func_0802c4d8
	.type Func_0802c4d8, %function
	.thumb_func
Func_0802c4d8:
	.incbin "baserom.gba", 0x0002c4d8, 0x000003c8
	.section .rom.0002c8a0, "ax"
	.global Func_0802c8a0
	.type Func_0802c8a0, %function
	.thumb_func
Func_0802c8a0:
	.incbin "baserom.gba", 0x0002c8a0, 0x00000128
	.section .rom.0002c9c8, "ax"
	.global Func_0802c9c8
	.type Func_0802c9c8, %function
	.thumb_func
Func_0802c9c8:
	.incbin "baserom.gba", 0x0002c9c8, 0x00000140
	.section .rom.0002cb08, "ax"
	.global Func_0802cb08
	.type Func_0802cb08, %function
	.thumb_func
Func_0802cb08:
	.incbin "baserom.gba", 0x0002cb08, 0x00000244
	.section .rom.0002cd4c, "ax"
	.global Func_0802cd4c
	.type Func_0802cd4c, %function
	.thumb_func
Func_0802cd4c:
	.incbin "baserom.gba", 0x0002cd4c, 0x00000024
	.section .rom.0002cd70, "ax"
	.global Func_0802cd70
	.type Func_0802cd70, %function
	.thumb_func
Func_0802cd70:
	.incbin "baserom.gba", 0x0002cd70, 0x000000dc
	.section .rom.0002ce4c, "ax"
	.global Func_0802b700
	.type Func_0802b700, %function
	.thumb_func
Func_0802b700:
	.incbin "baserom.gba", 0x0002ce4c, 0x00000054
	.section .rom.0002cea0, "ax"
	.global Func_0802b63c
	.type Func_0802b63c, %function
	.thumb_func
Func_0802b63c:
	.incbin "baserom.gba", 0x0002cea0, 0x00000020
	.section .rom.0002cf10, "ax"
	.incbin "baserom.gba", 0x0002cf10, 0x0000032c
	.section .rom.0002d2b0, "ax"
	.incbin "baserom.gba", 0x0002d2b0, 0x00000074
	.section .rom.0002d36e, "ax"
	.incbin "baserom.gba", 0x0002d36e, 0x00000026
	.section .rom.0002d3c4, "ax"
	.incbin "baserom.gba", 0x0002d3c4, 0x00000028
	.section .rom.0002d400, "ax"
	.incbin "baserom.gba", 0x0002d400, 0x0000005c
	.section .rom.0002d45c, "ax"
	.global Func_0802b71c
	.type Func_0802b71c, %function
	.thumb_func
Func_0802b71c:
	.incbin "baserom.gba", 0x0002d45c, 0x0000007c
	.section .rom.0002d4d8, "ax"
	.global Func_0802cc88
	.type Func_0802cc88, %function
	.thumb_func
Func_0802cc88:
	.incbin "baserom.gba", 0x0002d4d8, 0x00000058
	.section .rom.0002d530, "ax"
	.global Func_0802ceb0
	.type Func_0802ceb0, %function
	.thumb_func
Func_0802ceb0:
	.incbin "baserom.gba", 0x0002d530, 0x000000d0
	.section .rom.0002d600, "ax"
	.global Func_080237c8
	.type Func_080237c8, %function
	.thumb_func
Func_080237c8:
	.incbin "baserom.gba", 0x0002d600, 0x00000044
	.section .rom.0002d644, "ax"
	.global Func_08022f24
	.type Func_08022f24, %function
	.thumb_func
Func_08022f24:
	.incbin "baserom.gba", 0x0002d644, 0x00000014
	.section .rom.0002d658, "ax"
	.global Func_0802d658
	.type Func_0802d658, %function
	.thumb_func
Func_0802d658:
	.incbin "baserom.gba", 0x0002d658, 0x00000058
	.section .rom.0002d6b0, "ax"
	.global Func_0802cc74
	.type Func_0802cc74, %function
	.thumb_func
Func_0802cc74:
	.incbin "baserom.gba", 0x0002d6b0, 0x00000038
	.section .rom.0002d6e8, "ax"
	.global Func_0802d6e8
	.type Func_0802d6e8, %function
	.thumb_func
Func_0802d6e8:
	.incbin "baserom.gba", 0x0002d6e8, 0x00000194
	.section .rom.0002d87c, "ax"
	.global Func_0802d87c
	.type Func_0802d87c, %function
	.thumb_func
Func_0802d87c:
	.incbin "baserom.gba", 0x0002d87c, 0x0000020c
	.section .rom.0002da88, "ax"
	.global Func_0802db88
	.type Func_0802db88, %function
	.thumb_func
Func_0802db88:
	.incbin "baserom.gba", 0x0002da88, 0x00000038
	.section .rom.0002dac0, "ax"
	.global Func_0802b7e0
	.type Func_0802b7e0, %function
	.thumb_func
Func_0802b7e0:
	.section .rom.0002db64, "ax"
	.incbin "baserom.gba", 0x0002db64, 0x000000e4
	.section .rom.0002dc48, "ax"
	.global Func_0802dc48
	.type Func_0802dc48, %function
	.thumb_func
Func_0802dc48:
	.incbin "baserom.gba", 0x0002dc48, 0x00000244
	.section .rom.0002de8c, "ax"
	.global Func_0802a650
	.type Func_0802a650, %function
	.thumb_func
Func_0802a650:
	.incbin "baserom.gba", 0x0002de8c, 0x00000cc0
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0002eb4c, 0x000000fc
	.global Data_0802ec48
Data_0802ec48:
	.incbin "baserom.gba", 0x0002ec48, 0x0000027c
	.global Data_0802eec4
Data_0802eec4:
	.incbin "baserom.gba", 0x0002eec4, 0x0000030c
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f1d0, 0x00000030
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f200, 0x000000dc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x0002f2dc, 0x00008d24
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x00000534
	.section .rom.00038eb0, "ax"
	.incbin "baserom.gba", 0x00038eb0, 0x00000090
	.section .rom.00038f40, "ax"
	.global UiWork_InitializeWithResourceCounters
	.type UiWork_InitializeWithResourceCounters, %function
	.thumb_func
UiWork_InitializeWithResourceCounters:
	.incbin "baserom.gba", 0x00038f40, 0x000000c4
	.section .rom.00039004, "ax"
	.global UiWork_Initialize
	.type UiWork_Initialize, %function
	.thumb_func
UiWork_Initialize:
	.incbin "baserom.gba", 0x00039004, 0x00000118
	.section .rom.0003911c, "ax"
	.global UiWindow_EraseBorderRect
	.type UiWindow_EraseBorderRect, %function
	.thumb_func
UiWindow_EraseBorderRect:
	.incbin "baserom.gba", 0x0003911c, 0x00000144
	.section .rom.00039260, "ax"
	.global UiWindow_Create
	.type UiWindow_Create, %function
	.thumb_func
UiWindow_Create:
	.incbin "baserom.gba", 0x00039260, 0x00000114
	.section .rom.0003939a, "ax"
	.incbin "baserom.gba", 0x0003939a, 0x00000002
	.section .rom.0003939c, "ax"
	.global UiWork_Finalize
	.type UiWork_Finalize, %function
	.thumb_func
UiWork_Finalize:
	.incbin "baserom.gba", 0x0003939c, 0x00000060
	.section .rom.00039416, "ax"
	.incbin "baserom.gba", 0x00039416, 0x00000002
	.section .rom.00039418, "ax"
	.global RenderOutput_RedrawSavedRect
	.type RenderOutput_RedrawSavedRect, %function
	.thumb_func
RenderOutput_RedrawSavedRect:
	.incbin "baserom.gba", 0x00039418, 0x00000018
	.section .rom.00039452, "ax"
	.incbin "baserom.gba", 0x00039452, 0x00000002
	.section .rom.00039454, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039454, 0x00000094
	.section .rom.0003950e, "ax"
	.incbin "baserom.gba", 0x0003950e, 0x00000002
	.section .rom.00039510, "ax"
	.global RenderOutput_Release
	.type RenderOutput_Release, %function
	.thumb_func
RenderOutput_Release:
	.incbin "baserom.gba", 0x00039510, 0x000001cc
	.section .rom.000396dc, "ax"
	.global Func_080396dc
	.type Func_080396dc, %function
	.thumb_func
Func_080396dc:
	.incbin "baserom.gba", 0x000396dc, 0x00000968
	.section .rom.0003a044, "ax"
	.global Func_0803a084
	.type Func_0803a084, %function
	.thumb_func
Func_0803a084:
	.incbin "baserom.gba", 0x0003a044, 0x00000334
	.section .rom.0003a378, "ax"
	.global UiWork_IsComplete
	.type UiWork_IsComplete, %function
	.thumb_func
UiWork_IsComplete:
	.incbin "baserom.gba", 0x0003a378, 0x0000002c
	.section .rom.0003a3a4, "ax"
	.global UiWork_IsIdle
	.type UiWork_IsIdle, %function
	.thumb_func
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a3a4, 0x0000014c
	.section .rom.0003a4f0, "ax"
	.global Func_0803a530
	.type Func_0803a530, %function
	.thumb_func
Func_0803a530:
	.incbin "baserom.gba", 0x0003a4f0, 0x0000001c
	.section .rom.0003a50c, "ax"
	.global Func_0803a54c
	.type Func_0803a54c, %function
	.thumb_func
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a50c, 0x00000094
	.section .rom.0003a5a0, "ax"
	.global Func_0803a5e0
	.type Func_0803a5e0, %function
	.thumb_func
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a5a0, 0x0000002c
	.section .rom.0003a5cc, "ax"
	.global Func_0803a60c
	.type Func_0803a60c, %function
	.thumb_func
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5cc, 0x0000005c
	.section .rom.0003a65c, "ax"
	.global UiText_OpenMessageWindow
	.type UiText_OpenMessageWindow, %function
	.thumb_func
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a65c, 0x00000110
	.section .rom.0003a76c, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.type UiText_ShowPositionedMessageAndWait, %function
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a76c, 0x00000320
	.section .rom.0003aa8c, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa8c, 0x00000540
	.section .rom.0003afcc, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003afcc, 0x000008b4
	.section .rom.0003b898, "ax"
	.global UiText_GetResourceDimensions
	.type UiText_GetResourceDimensions, %function
	.thumb_func
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b898, 0x0000004c
	.section .rom.0003b8e4, "ax"
	.global Func_0803b8cc
	.type Func_0803b8cc, %function
	.thumb_func
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8e4, 0x0000004c
	.section .rom.0003b930, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b930, 0x00000734
	.section .rom.0003c064, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c064, 0x00000108
	.section .rom.0003c16c, "ax"
	.global Func_0803c170
	.type Func_0803c170, %function
	.thumb_func
Func_0803c170:
	.incbin "baserom.gba", 0x0003c16c, 0x00000028
	.section .rom.0003c194, "ax"
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c194, 0x000001e0
	.section .rom.0003c374, "ax"
	.global UiWindow_SetTilemapEntry
	.type UiWindow_SetTilemapEntry, %function
	.thumb_func
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c374, 0x00000634
	.section .rom.0003c9b8, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9b8, 0x00000064
	.section .rom.0003ca1c, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca1c, 0x00000144
	.section .rom.0003cb6e, "ax"
	.incbin "baserom.gba", 0x0003cb6e, 0x00000002
	.section .rom.0003cb70, "ax"
	.global Func_0803cb1c
	.type Func_0803cb1c, %function
	.thumb_func
Func_0803cb1c:
	.incbin "baserom.gba", 0x0003cb70, 0x0000018c
	.section .rom.0003ccfc, "ax"
	.global Func_0803cca8
	.type Func_0803cca8, %function
	.thumb_func
Func_0803cca8:
	.incbin "baserom.gba", 0x0003ccfc, 0x00000028
	.section .rom.0003cd24, "ax"
	.global Func_0803ccd0
	.type Func_0803ccd0, %function
	.thumb_func
Func_0803ccd0:
	.incbin "baserom.gba", 0x0003cd24, 0x0000014c
	.section .rom.0003ce70, "ax"
	.global Func_0803ce1c
	.type Func_0803ce1c, %function
	.thumb_func
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce70, 0x00000048
	.section .rom.0003ceb8, "ax"
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003ceb8, 0x000001bc
	.section .rom.0003d074, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d074, 0x000002d0
	.section .rom.0003d344, "ax"
	.global Localization_LookupEntryId
	.type Localization_LookupEntryId, %function
	.thumb_func
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d344, 0x000000d0
	.section .rom.0003d414, "ax"
	.global Func_0803d3c0
	.type Func_0803d3c0, %function
	.thumb_func
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d414, 0x00000090
	.section .rom.0003d4a4, "ax"
	.global Func_0803d450
	.type Func_0803d450, %function
	.thumb_func
Func_0803d450:
	.incbin "baserom.gba", 0x0003d4a4, 0x0000006c
	.section .rom.0003d538, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d538, 0x000000e0
	.section .rom.0003d618, "ax"
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d618, 0x000000bc
	.section .rom.0003d6d4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6d4, 0x0000022c
	.section .rom.0003d97e, "ax"
	.incbin "baserom.gba", 0x0003d97e, 0x00000002
	.section .rom.0003d980, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d980, 0x00000060
	.section .rom.0003d9e0, "ax"
	.global Ability_LoadGlyph
	.type Ability_LoadGlyph, %function
	.thumb_func
Ability_LoadGlyph:
	.incbin "baserom.gba", 0x0003d9e0, 0x00000030
	.section .rom.0003da10, "ax"
	.global Func_0803d9bc
	.type Func_0803d9bc, %function
	.thumb_func
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da10, 0x000000bc
	.section .rom.0003dacc, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.type Ui_PrepareTransferFromTableEntry, %function
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dacc, 0x000001a4
	.section .rom.0003dc70, "ax"
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc70, 0x00000108
	.section .rom.0003dd78, "ax"
	.global Func_0803dd24
	.type Func_0803dd24, %function
	.thumb_func
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd78, 0x00000044
	.section .rom.0003ddbc, "ax"
	.global Func_0803dd68
	.type Func_0803dd68, %function
	.thumb_func
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddbc, 0x00000030
	.section .rom.0003ddec, "ax"
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddec, 0x00000110
	.section .rom.0003defc, "ax"
	.global Func_0803dea8
	.type Func_0803dea8, %function
	.thumb_func
Func_0803dea8:
	.incbin "baserom.gba", 0x0003defc, 0x00000058
	.section .rom.0003df54, "ax"
	.global Func_0803df00
	.type Func_0803df00, %function
	.thumb_func
Func_0803df00:
	.incbin "baserom.gba", 0x0003df54, 0x00000014
	.section .rom.0003df68, "ax"
	.global Resource_ScheduleOwnerReset
	.type Resource_ScheduleOwnerReset, %function
	.thumb_func
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df68, 0x00000694
	.section .rom.0003e7c8, "ax"
	.global Func_0803e774
	.type Func_0803e774, %function
	.thumb_func
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7c8, 0x00000038
	.section .rom.0003e800, "ax"
	.global Func_0803e7ac
	.type Func_0803e7ac, %function
	.thumb_func
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e800, 0x00000140
	.section .rom.0003e940, "ax"
	.global NodeChain_GetNodeAtCount
	.type NodeChain_GetNodeAtCount, %function
	.thumb_func
NodeChain_GetNodeAtCount:
	.incbin "baserom.gba", 0x0003e940, 0x0000002c
	.section .rom.0003e96c, "ax"
	.global Func_0803e918
	.type Func_0803e918, %function
	.thumb_func
Func_0803e918:
	.incbin "baserom.gba", 0x0003e96c, 0x00000080
	.section .rom.0003e9ec, "ax"
	.global Func_0803e998
	.type Func_0803e998, %function
	.thumb_func
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9ec, 0x0000063c
	.section .rom.0003f058, "ax"
	.incbin "baserom.gba", 0x0003f058, 0x000001d0
	.section .rom.0003f228, "ax"
	.global Resource_LoadByMode
	.type Resource_LoadByMode, %function
	.thumb_func
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f228, 0x00000060
	.section .rom.0003f288, "ax"
	.global Func_0803f234
	.type Func_0803f234, %function
	.thumb_func
Func_0803f234:
	.incbin "baserom.gba", 0x0003f288, 0x000003dc
	.section .rom.0003f664, "ax"
	.global Func_0803f610
	.type Func_0803f610, %function
	.thumb_func
Func_0803f610:
	.incbin "baserom.gba", 0x0003f664, 0x00000004
	.section .rom.0003f668, "ax"
	.global Func_0803f614
	.type Func_0803f614, %function
	.thumb_func
Func_0803f614:
	.incbin "baserom.gba", 0x0003f668, 0x0000000c
	.section .rom.0003f674, "ax"
	.global Func_0803f620
	.type Func_0803f620, %function
	.thumb_func
Func_0803f620:
	.incbin "baserom.gba", 0x0003f674, 0x00000004
	.section .rom.0003f678, "ax"
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f678, 0x00000074
	.section .rom.0003f6ec, "ax"
	.global UiTextResource_SetPosition
	.type UiTextResource_SetPosition, %function
	.thumb_func
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6ec, 0x00000028
	.section .rom.0003f714, "ax"
	.global UiTextResource_Release
	.type UiTextResource_Release, %function
	.thumb_func
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f714, 0x00000098
	.global Resource_ResetPendingTransfer
	.type Resource_ResetPendingTransfer, %function
	.thumb_func
Resource_ResetPendingTransfer:
	.incbin "baserom.gba", 0x0003f7ac, 0x00000020
	.section .rom.0003f7cc, "ax"
	.global Func_0803f778
	.type Func_0803f778, %function
	.thumb_func
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7cc, 0x000000f4
	.section .rom.0003f8c0, "ax"
	.global Func_0803f86c
	.type Func_0803f86c, %function
	.thumb_func
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8c0, 0x000000d0
	.section .rom.0003f990, "ax"
	.global Func_0803f93c
	.type Func_0803f93c, %function
	.thumb_func
Func_0803f93c:
	.incbin "baserom.gba", 0x0003f990, 0x0000002c
	.section .rom.0003f9bc, "ax"
	.global Func_0803f968
	.type Func_0803f968, %function
	.thumb_func
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9bc, 0x00000058
	.section .rom.0003fa14, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa14, 0x00000728
	.section .rom.0004013c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004013c, 0x00001a80
	.section .rom.00041bbc, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bbc, 0x000000a4
	.section .rom.00041c60, "ax"
	.global Func_08041c0c
	.type Func_08041c0c, %function
	.thumb_func
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c60, 0x00000048
	.section .rom.00041ca8, "ax"
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041ca8, 0x0000031c
	.section .rom.00041fc4, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x00041fc4, 0x00000020
	.section .rom.00041fe4, "ax"
	.global Func_08041f90
	.type Func_08041f90, %function
	.thumb_func
Func_08041f90:
	.incbin "baserom.gba", 0x00041fe4, 0x00000014
	.section .rom.00041ff8, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x00041ff8, 0x0000006c
	.section .rom.00042064, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042064, 0x00000098
	.section .rom.000420fc, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x000420fc, 0x00000054
	.section .rom.00042150, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042150, 0x0000008c
	.section .rom.000421dc, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421dc, 0x0000005c
	.section .rom.00042238, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042238, 0x00000030
	.section .rom.00042268, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042268, 0x00000030
	.section .rom.00042298, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x00042298, 0x000000d0
	.section .rom.00042368, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042368, 0x00000088
	.section .rom.000424a4, "ax"
	.global Func_08042450
	.type Func_08042450, %function
	.thumb_func
Func_08042450:
	.incbin "baserom.gba", 0x000424a4, 0x000000b8
	.section .rom.0004255c, "ax"
	.global Func_08042508
	.type Func_08042508, %function
	.thumb_func
Func_08042508:
	.incbin "baserom.gba", 0x0004255c, 0x00000064
	.section .rom.000425ce, "ax"
	.incbin "baserom.gba", 0x000425ce, 0x00000002
	.section .rom.000425d0, "ax"
	.global Func_0804257c
	.type Func_0804257c, %function
	.thumb_func
Func_0804257c:
	.incbin "baserom.gba", 0x000425d0, 0x0000000c
	.section .rom.000425dc, "ax"
	.global Func_08042588
	.type Func_08042588, %function
	.thumb_func
Func_08042588:
	.incbin "baserom.gba", 0x000425dc, 0x00000074
	.section .rom.00042684, "ax"
	.incbin "baserom.gba", 0x00042684, 0x00000060
	.section .rom.000426e4, "ax"
	.global Func_08042690
	.type Func_08042690, %function
	.thumb_func
Func_08042690:
	.incbin "baserom.gba", 0x000426e4, 0x000002ec
	.section .rom.000429d0, "ax"
	.global Func_0804297c
	.type Func_0804297c, %function
	.thumb_func
Func_0804297c:
	.incbin "baserom.gba", 0x000429d0, 0x00000430
	.section .rom.00042e00, "ax"
	.global Func_08042dac
	.type Func_08042dac, %function
	.thumb_func
Func_08042dac:
	.incbin "baserom.gba", 0x00042e00, 0x00000318
	.section .rom.00043118, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043118, 0x00000198
	.section .rom.000432b0, "ax"
	.global Func_0804325c
	.type Func_0804325c, %function
	.thumb_func
Func_0804325c:
	.incbin "baserom.gba", 0x000432b0, 0x00000048
	.section .rom.000432f8, "ax"
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000432f8, 0x00000248
	.section .rom.00043540, "ax"
	.global Func_080434ec
	.type Func_080434ec, %function
	.thumb_func
Func_080434ec:
	.incbin "baserom.gba", 0x00043540, 0x00000048
	.section .rom.00043588, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043588, 0x000000c8
	.section .rom.00043650, "ax"
	.global Func_080435fc
	.type Func_080435fc, %function
	.thumb_func
Func_080435fc:
	.incbin "baserom.gba", 0x00043650, 0x00000094
	.section .rom.000436e4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436e4, 0x000000e4
	.section .rom.000437c8, "ax"
	.global Func_08043774
	.type Func_08043774, %function
	.thumb_func
Func_08043774:
	.incbin "baserom.gba", 0x000437c8, 0x00000098
	.section .rom.00043860, "ax"
	.global Func_0804380c
	.type Func_0804380c, %function
	.thumb_func
Func_0804380c:
	.incbin "baserom.gba", 0x00043860, 0x000000bc
	.section .rom.0004391c, "ax"
	.global Func_080438c8
	.type Func_080438c8, %function
	.thumb_func
Func_080438c8:
	.incbin "baserom.gba", 0x0004391c, 0x000000b0
	.section .rom.00043a18, "ax"
	.incbin "baserom.gba", 0x00043a18, 0x00000228
	.section .rom.00043c84, "ax"
	.incbin "baserom.gba", 0x00043c84, 0x00000830
	.section .rom.000444b4, "ax"
	.global Func_08044460
	.type Func_08044460, %function
	.thumb_func
Func_08044460:
	.incbin "baserom.gba", 0x000444b4, 0x00000018
	.section .rom.000444cc, "ax"
	.global Func_08044478
	.type Func_08044478, %function
	.thumb_func
Func_08044478:
	.incbin "baserom.gba", 0x000444cc, 0x00000010
	.section .rom.000444dc, "ax"
	.global Func_08044488
	.type Func_08044488, %function
	.thumb_func
Func_08044488:
	.incbin "baserom.gba", 0x000444dc, 0x000000d0
	.section .rom.000445ac, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445ac, 0x0000051c
	.section .rom.00044ac8, "ax"
	.global Func_08044a54
	.type Func_08044a54, %function
	.thumb_func
Func_08044a54:
	.incbin "baserom.gba", 0x00044ac8, 0x00000004
	.section .rom.00044acc, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044acc, 0x00000140
	.section .rom.00044c0c, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044c0c, 0x000000e8
	.section .rom.00044cf4, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044cf4, 0x00000148
	.section .rom.00044e3c, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e3c, 0x00000484
	.section .rom.0004532e, "ax"
	.incbin "baserom.gba", 0x0004532e, 0x00000076
	.section .rom.000453a4, "ax"
	.global Func_08045330
	.type Func_08045330, %function
	.thumb_func
Func_08045330:
	.incbin "baserom.gba", 0x000453a4, 0x000000a0
	.section .rom.00045444, "ax"
	.global Func_080453d0
	.type Func_080453d0, %function
	.thumb_func
Func_080453d0:
	.incbin "baserom.gba", 0x00045444, 0x00000094
	.section .rom.0004559a, "ax"
	.incbin "baserom.gba", 0x0004559a, 0x0000002a
	.section .rom.000455d8, "ax"
	.incbin "baserom.gba", 0x000455d8, 0x0000004c
	.section .rom.00045650, "ax"
	.incbin "baserom.gba", 0x00045650, 0x0000018c
	.section .rom.000457f2, "ax"
	.incbin "baserom.gba", 0x000457f2, 0x00000032
	.section .rom.00045842, "ax"
	.incbin "baserom.gba", 0x00045842, 0x00000966
	.section .rom.000461a8, "ax"
	.global Func_08046134
	.type Func_08046134, %function
	.thumb_func
Func_08046134:
	.incbin "baserom.gba", 0x000461a8, 0x00000094
	.section .rom.0004623c, "ax"
	.global Func_080461c8
	.type Func_080461c8, %function
	.thumb_func
Func_080461c8:
	.incbin "baserom.gba", 0x0004623c, 0x00000098
	.section .rom.000462f8, "ax"
	.incbin "baserom.gba", 0x000462f8, 0x00000150
	.section .rom.00046486, "ax"
	.incbin "baserom.gba", 0x00046486, 0x00003656
	.section .rom.00049b2a, "ax"
	.incbin "baserom.gba", 0x00049b2a, 0x00001f5e
	.section .rom.0004bac2, "ax"
	.incbin "baserom.gba", 0x0004bac2, 0x00000352
	.section .rom.0004be14, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be14, 0x000014d4
	.section .rom.0004d2e8, "ax"
	.global AffineEffect_InitializeWork
	.type AffineEffect_InitializeWork, %function
	.thumb_func
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d2e8, 0x0000003c
	.section .rom.0004d324, "ax"
	.global Menu_EndResourceSelection
	.type Menu_EndResourceSelection, %function
	.thumb_func
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d324, 0x00000054
	.section .rom.0004d378, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d378, 0x00000220
	.section .rom.0004d598, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x0004d598, 0x0000005c
	.section .rom.0004d5f4, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5f4, 0x00000188
	.section .rom.0004d77c, "ax"
	.global Menu_AnimateSelectionToEntry
	.type Menu_AnimateSelectionToEntry, %function
	.thumb_func
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d77c, 0x0000003c
	.section .rom.0004d7b8, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7b8, 0x00000250
	.section .rom.0004da08, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da08, 0x0000016c
	.section .rom.0004db74, "ax"
	.global Menu_SelectEntry11To14
	.type Menu_SelectEntry11To14, %function
	.thumb_func
Menu_SelectEntry11To14:
	.incbin "baserom.gba", 0x0004db74, 0x0000003c
	.section .rom.0004dc48, "ax"
	.global Func_0804da3c
	.type Func_0804da3c, %function
	.thumb_func
Func_0804da3c:
	.incbin "baserom.gba", 0x0004dc48, 0x00000050
	.section .rom.0004dcc6, "ax"
	.incbin "baserom.gba", 0x0004dcc6, 0x000000ba
	.section .rom.0004dd80, "ax"
	.global Func_0804db74
	.type Func_0804db74, %function
	.thumb_func
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd80, 0x00000254
	.section .rom.0004e0ac, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0ac, 0x000000c4
	.section .rom.0004e170, "ax"
	.global Menu_HandleFlagGridInput
	.type Menu_HandleFlagGridInput, %function
	.thumb_func
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e170, 0x00000140
	.section .rom.0004e2da, "ax"
	.incbin "baserom.gba", 0x0004e2da, 0x00000002
	.section .rom.0004e2dc, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2dc, 0x00000108
	.section .rom.0004e3e4, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3e4, 0x0000021c
	.section .rom.0004e600, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e600, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed64, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f330, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054c20, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055030, 0x000057a4
	.section .rom.0005c3d4, "ax"
	.incbin "baserom.gba", 0x0005c3d4, 0x0000374c
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fb20, 0x00000174
	.section .rom.000b7264, "ax"
	.incbin "baserom.gba", 0x000b7264, 0x00000510
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000b7774, 0x0000288c
	.section .rom.000ba0b0, "ax"
	.incbin "baserom.gba", 0x000ba0b0, 0x00000010
	.section .rom.000ba200, "ax"
	.incbin "baserom.gba", 0x000ba200, 0x00000048
	.section .rom.000ba2b8, "ax"
	.incbin "baserom.gba", 0x000ba2b8, 0x00000050
	.section .rom.000ba338, "ax"
	.incbin "baserom.gba", 0x000ba338, 0x00000010
	.section .rom.000ba348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000ba348, 0x00000018
	.section .rom.000ba360, "ax"
	.global Func_080ad360
	.type Func_080ad360, %function
	.thumb_func
Func_080ad360:
	.incbin "baserom.gba", 0x000ba360, 0x00000048
	.section .rom.000ba3a8, "ax"
	.global Owner_GetRecord
	.type Owner_GetRecord, %function
	.thumb_func
Owner_GetRecord:
	.incbin "baserom.gba", 0x000ba3a8, 0x0000001c
	.section .rom.000ba3f6, "ax"
	.incbin "baserom.gba", 0x000ba3f6, 0x00000002
	.section .rom.000ba3f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000ba3f8, 0x00000898
	.section .rom.000bac90, "ax"
	.global Func_080aff94
	.type Func_080aff94, %function
	.thumb_func
Func_080aff94:
	.incbin "baserom.gba", 0x000bac90, 0x000000cc
	.section .rom.000bad5c, "ax"
	.global Func_080aee40
	.type Func_080aee40, %function
	.thumb_func
Func_080aee40:
	.incbin "baserom.gba", 0x000bad5c, 0x00000094
	.section .rom.000badf0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000badf0, 0x0000080c
	.section .rom.000bb5fc, "ax"
	.global Func_080add5c
	.type Func_080add5c, %function
	.thumb_func
Func_080add5c:
	.incbin "baserom.gba", 0x000bb5fc, 0x00000238
	.section .rom.000bb868, "ax"
	.global Func_080b1004
	.type Func_080b1004, %function
	.thumb_func
Func_080b1004:
	.incbin "baserom.gba", 0x000bb868, 0x000001c8
	.section .rom.000bbc66, "ax"
	.incbin "baserom.gba", 0x000bbc66, 0x00000002
	.section .rom.000bbc68, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000bbc68, 0x0000003c
	.section .rom.000bbca4, "ax"
	.global Inventory_GetQuantity
	.type Inventory_GetQuantity, %function
	.thumb_func
Inventory_GetQuantity:
	.incbin "baserom.gba", 0x000bbca4, 0x00000024
	.section .rom.000bbe40, "ax"
	.incbin "baserom.gba", 0x000bbe40, 0x00000058
	.section .rom.000bbe98, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000bbe98, 0x00000030
	.section .rom.000bbf34, "ax"
	.global Func_080aef34
	.type Func_080aef34, %function
	.thumb_func
Func_080aef34:
	.incbin "baserom.gba", 0x000bbf34, 0x000000d4
	.section .rom.000bc0e2, "ax"
	.incbin "baserom.gba", 0x000bc0e2, 0x00000002
	.section .rom.000bc0e4, "ax"
	.global Func_080af464
	.type Func_080af464, %function
	.thumb_func
Func_080af464:
	.incbin "baserom.gba", 0x000bc0e4, 0x00000064
	.section .rom.000bc148, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000bc148, 0x00000080
	.section .rom.000bc1fa, "ax"
	.incbin "baserom.gba", 0x000bc1fa, 0x00000002
	.section .rom.000bc1fc, "ax"
	.global Func_080af1fc
	.type Func_080af1fc, %function
	.thumb_func
Func_080af1fc:
	.incbin "baserom.gba", 0x000bc1fc, 0x00000048
	.section .rom.000bc244, "ax"
	.global Func_080af244
	.type Func_080af244, %function
	.thumb_func
Func_080af244:
	.incbin "baserom.gba", 0x000bc244, 0x00000054
	.section .rom.000bc338, "ax"
	.global Func_080af338
	.type Func_080af338, %function
	.thumb_func
Func_080af338:
	.incbin "baserom.gba", 0x000bc338, 0x00000040
	.section .rom.000bc378, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000bc378, 0x00000028
	.section .rom.000bc43c, "ax"
	.global Func_080af43c
	.type Func_080af43c, %function
	.thumb_func
Func_080af43c:
	.incbin "baserom.gba", 0x000bc43c, 0x0000007c
	.section .rom.000bc4b8, "ax"
	.global Func_080af4b8
	.type Func_080af4b8, %function
	.thumb_func
Func_080af4b8:
	.incbin "baserom.gba", 0x000bc4b8, 0x0000023c
	.section .rom.000bc6f4, "ax"
	.global OwnerAction_Add
	.type OwnerAction_Add, %function
	.thumb_func
OwnerAction_Add:
	.incbin "baserom.gba", 0x000bc6f4, 0x000000a0
	.section .rom.000bc794, "ax"
	.global Func_080af794
	.type Func_080af794, %function
	.thumb_func
Func_080af794:
	.incbin "baserom.gba", 0x000bc794, 0x00000018
	.section .rom.000bc7ac, "ax"
	.global Func_080af7ac
	.type Func_080af7ac, %function
	.thumb_func
Func_080af7ac:
	.incbin "baserom.gba", 0x000bc7ac, 0x00000120
	.section .rom.000bc8cc, "ax"
	.global Func_080af8cc
	.type Func_080af8cc, %function
	.thumb_func
Func_080af8cc:
	.incbin "baserom.gba", 0x000bc8cc, 0x00000050
	.section .rom.000bc91c, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000bc91c, 0x00000298
	.section .rom.000bcbec, "ax"
	.global Func_080afbec
	.type Func_080afbec, %function
	.thumb_func
Func_080afbec:
	.incbin "baserom.gba", 0x000bcbec, 0x000001d0
	.section .rom.000bcdd8, "ax"
	.global Func_080afdd8
	.type Func_080afdd8, %function
	.thumb_func
Func_080afdd8:
	.incbin "baserom.gba", 0x000bcdd8, 0x00000044
	.section .rom.000bce1c, "ax"
	.global Func_080afe1c
	.type Func_080afe1c, %function
	.thumb_func
Func_080afe1c:
	.incbin "baserom.gba", 0x000bce1c, 0x0000005c
	.section .rom.000bceb0, "ax"
	.global Func_080afeb0
	.type Func_080afeb0, %function
	.thumb_func
Func_080afeb0:
	.incbin "baserom.gba", 0x000bceb0, 0x00000024
	.section .rom.000bcf28, "ax"
	.global Func_080aff28
	.type Func_080aff28, %function
	.thumb_func
Func_080aff28:
	.incbin "baserom.gba", 0x000bcf28, 0x00000054
	.section .rom.000bcf94, "ax"
	.incbin "baserom.gba", 0x000bcf94, 0x00000094
	.section .rom.000bd028, "ax"
	.global Func_080b0028
	.type Func_080b0028, %function
	.thumb_func
Func_080b0028:
	.incbin "baserom.gba", 0x000bd028, 0x00000260
	.section .rom.000bd298, "ax"
	.global Owner_RefreshDerivedData
	.type Owner_RefreshDerivedData, %function
	.thumb_func
Owner_RefreshDerivedData:
	.incbin "baserom.gba", 0x000bd298, 0x0000003c
	.section .rom.000bd2d4, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x000bd2d4, 0x000000a4
	.section .rom.000bd378, "ax"
	.global BattleRandom16
	.type BattleRandom16, %function
	.thumb_func
BattleRandom16:
	.incbin "baserom.gba", 0x000bd378, 0x00000034
	.section .rom.000bd3ac, "ax"
	.global Func_080b03ac
	.type Func_080b03ac, %function
	.thumb_func
Func_080b03ac:
	.incbin "baserom.gba", 0x000bd3ac, 0x00000034
	.section .rom.000bd3e0, "ax"
	.global Func_080b03e0
	.type Func_080b03e0, %function
	.thumb_func
Func_080b03e0:
	.incbin "baserom.gba", 0x000bd3e0, 0x00000028
	.section .rom.000bd408, "ax"
	.global Func_080b0408
	.type Func_080b0408, %function
	.thumb_func
Func_080b0408:
	.incbin "baserom.gba", 0x000bd408, 0x0000002c
	.section .rom.000bd434, "ax"
	.global Func_080b0434
	.type Func_080b0434, %function
	.thumb_func
Func_080b0434:
	.incbin "baserom.gba", 0x000bd434, 0x0000002c
	.section .rom.000bd4ba, "ax"
	.incbin "baserom.gba", 0x000bd4ba, 0x00000002
	.section .rom.000bd4bc, "ax"
	.global Func_080b04bc
	.type Func_080b04bc, %function
	.thumb_func
Func_080b04bc:
	.incbin "baserom.gba", 0x000bd4bc, 0x00000284
	.section .rom.000bd740, "ax"
	.global Func_080b0740
	.type Func_080b0740, %function
	.thumb_func
Func_080b0740:
	.incbin "baserom.gba", 0x000bd740, 0x0000001c
	.section .rom.000bd75c, "ax"
	.global Func_080b075c
	.type Func_080b075c, %function
	.thumb_func
Func_080b075c:
	.incbin "baserom.gba", 0x000bd75c, 0x00000338
	.section .rom.000bdbb6, "ax"
	.incbin "baserom.gba", 0x000bdbb6, 0x00000002
	.section .rom.000bdbb8, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000bdbb8, 0x000000c0
	.section .rom.000bdc9a, "ax"
	.incbin "baserom.gba", 0x000bdc9a, 0x00000002
	.section .rom.000bdc9c, "ax"
	.global Djinn_Activate
	.type Djinn_Activate, %function
	.thumb_func
Djinn_Activate:
	.incbin "baserom.gba", 0x000bdc9c, 0x00000068
	.section .rom.000bdd04, "ax"
	.global Djinn_Deactivate
	.type Djinn_Deactivate, %function
	.thumb_func
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000bdd04, 0x00000054
	.section .rom.000bdd58, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000bdd58, 0x000000ac
	.section .rom.000bdf5a, "ax"
	.incbin "baserom.gba", 0x000bdf5a, 0x00000002
	.section .rom.000bdf5c, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.section .rom.000be002, "ax"
	.incbin "baserom.gba", 0x000be002, 0x00001362
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000bf364, 0x0000f1a8
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000ce50c, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000ce5f4, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000d3bb0, 0x00001450
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000d5000, 0x00000008
	.section .rom.000d55c0, "ax"
	.incbin "baserom.gba", 0x000d55c0, 0x00000008
	.section .rom.000d59cc, "ax"
	.incbin "baserom.gba", 0x000d59cc, 0x00000a00
	.section .rom.000d65a8, "ax"
	.incbin "baserom.gba", 0x000d65a8, 0x00000378
	.section .rom.000d6934, "ax"
	.incbin "baserom.gba", 0x000d6934, 0x00000010
	.section .rom.000d6944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000d6944, 0x00000484
	.section .rom.000d6dc8, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000d6dc8, 0x000003c4
	.section .rom.000d718c, "ax"
	.global Func_080ca18c
	.type Func_080ca18c, %function
	.thumb_func
Func_080ca18c:
	.incbin "baserom.gba", 0x000d718c, 0x00000018
	.section .rom.000d71a4, "ax"
	.global Func_080ca1a4
	.type Func_080ca1a4, %function
	.thumb_func
Func_080ca1a4:
	.incbin "baserom.gba", 0x000d71a4, 0x00000018
	.section .rom.000d71bc, "ax"
	.global Func_080ca1bc
	.type Func_080ca1bc, %function
	.thumb_func
Func_080ca1bc:
	.incbin "baserom.gba", 0x000d71bc, 0x000000c4
	.section .rom.000d7280, "ax"
	.global Func_080ca280
	.type Func_080ca280, %function
	.thumb_func
Func_080ca280:
	.incbin "baserom.gba", 0x000d7280, 0x000000e8
	.section .rom.000d7368, "ax"
	.global Func_080ca368
	.type Func_080ca368, %function
	.thumb_func
Func_080ca368:
	.incbin "baserom.gba", 0x000d7368, 0x0000008c
	.section .rom.000d73f4, "ax"
	.global Func_080ca3f4
	.type Func_080ca3f4, %function
	.thumb_func
Func_080ca3f4:
	.incbin "baserom.gba", 0x000d73f4, 0x000000a0
	.section .rom.000d7494, "ax"
	.global Func_080ca494
	.type Func_080ca494, %function
	.thumb_func
Func_080ca494:
	.incbin "baserom.gba", 0x000d7494, 0x00000210
	.section .rom.000d76a4, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000d76a4, 0x00000040
	.section .rom.000d76e4, "ax"
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000d76e4, 0x00000004
	.section .rom.000d76e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000d76e8, 0x000002e4
	.section .rom.000d79cc, "ax"
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000d79cc, 0x00000060
	.section .rom.000d7a2c, "ax"
	.global Func_080caa2c
	.type Func_080caa2c, %function
	.thumb_func
Func_080caa2c:
	.incbin "baserom.gba", 0x000d7a2c, 0x00000020
	.section .rom.000d7a4c, "ax"
	.global Func_080caa4c
	.type Func_080caa4c, %function
	.thumb_func
Func_080caa4c:
	.incbin "baserom.gba", 0x000d7a4c, 0x00000274
	.section .rom.000d7cc0, "ax"
	.global ObjectTable_FindLastActiveId
	.type ObjectTable_FindLastActiveId, %function
	.thumb_func
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000d7cc0, 0x0000002c
	.section .rom.000d7cec, "ax"
	.global Scene_AssignViewFlags
	.type Scene_AssignViewFlags, %function
	.thumb_func
Scene_AssignViewFlags:
	.incbin "baserom.gba", 0x000d7cec, 0x00000098
	.section .rom.000d7d84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000d7d84, 0x00000018
	.section .rom.000d7d9c, "ax"
	.global Func_080cad9c
	.type Func_080cad9c, %function
	.thumb_func
Func_080cad9c:
	.incbin "baserom.gba", 0x000d7d9c, 0x000000c0
	.section .rom.000d7e5c, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000d7e5c, 0x00000114
	.section .rom.000d7fc4, "ax"
	.incbin "baserom.gba", 0x000d7fc4, 0x00000704
	.section .rom.000d86c8, "ax"
	.global Func_080cb6c8
	.type Func_080cb6c8, %function
	.thumb_func
Func_080cb6c8:
	.incbin "baserom.gba", 0x000d86c8, 0x0000002c
	.section .rom.000d86f4, "ax"
	.global Func_080cb6f4
	.type Func_080cb6f4, %function
	.thumb_func
Func_080cb6f4:
	.incbin "baserom.gba", 0x000d86f4, 0x00000138
	.section .rom.000d882c, "ax"
	.global Func_080cb82c
	.type Func_080cb82c, %function
	.thumb_func
Func_080cb82c:
	.incbin "baserom.gba", 0x000d882c, 0x00000078
	.section .rom.000d88a4, "ax"
	.global Func_080cb8a4
	.type Func_080cb8a4, %function
	.thumb_func
Func_080cb8a4:
	.incbin "baserom.gba", 0x000d88a4, 0x00000ca8
	.section .rom.000d954c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d954c, 0x000007fc
	.section .rom.000d9d76, "ax"
	.incbin "baserom.gba", 0x000d9d76, 0x00000002
	.section .rom.000d9d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d9d78, 0x00000150
	.section .rom.000d9ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d9ec8, 0x00000a54
	.section .rom.000da91c, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000da91c, 0x000002dc
	.section .rom.000dabf8, "ax"
	.global Func_080cdbf8
	.type Func_080cdbf8, %function
	.thumb_func
Func_080cdbf8:
	.incbin "baserom.gba", 0x000dabf8, 0x00000048
	.section .rom.000dac72, "ax"
	.incbin "baserom.gba", 0x000dac72, 0x00000002
	.section .rom.000dac74, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000dac74, 0x0000010c
	.section .rom.000dad80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000dad80, 0x00000148
	.section .rom.000daec8, "ax"
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000daec8, 0x0000000c
	.section .rom.000daed4, "ax"
	.global Func_080cded4
	.type Func_080cded4, %function
	.thumb_func
Func_080cded4:
	.incbin "baserom.gba", 0x000daed4, 0x00000088
	.section .rom.000daf5c, "ax"
	.global Func_080cdf5c
	.type Func_080cdf5c, %function
	.thumb_func
Func_080cdf5c:
	.incbin "baserom.gba", 0x000daf5c, 0x00000054
	.section .rom.000dafb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000dafb0, 0x00000ba8
	.section .rom.000dbb58, "ax"
	.global Func_080ceb58
	.type Func_080ceb58, %function
	.thumb_func
Func_080ceb58:
	.incbin "baserom.gba", 0x000dbb58, 0x00000028
	.section .rom.000dbb94, "ax"
	.incbin "baserom.gba", 0x000dbb94, 0x000002ac
	.section .rom.000dbe80, "ax"
	.incbin "baserom.gba", 0x000dbe80, 0x00000048
	.section .rom.000dbec8, "ax"
	.global Func_080ceec8
	.type Func_080ceec8, %function
	.thumb_func
Func_080ceec8:
	.incbin "baserom.gba", 0x000dbec8, 0x000000a0
	.section .rom.000dbf68, "ax"
	.global Func_080cef68
	.type Func_080cef68, %function
	.thumb_func
Func_080cef68:
	.incbin "baserom.gba", 0x000dbf68, 0x0000004c
	.section .rom.000dbfb4, "ax"
	.global Func_080cefb4
	.type Func_080cefb4, %function
	.thumb_func
Func_080cefb4:
	.incbin "baserom.gba", 0x000dbfb4, 0x0000001c
	.section .rom.000dbfd0, "ax"
	.global Func_080cefd0
	.type Func_080cefd0, %function
	.thumb_func
Func_080cefd0:
	.incbin "baserom.gba", 0x000dbfd0, 0x0000002c
	.section .rom.000dbffc, "ax"
	.global Func_080ceffc
	.type Func_080ceffc, %function
	.thumb_func
Func_080ceffc:
	.incbin "baserom.gba", 0x000dbffc, 0x00000008
	.section .rom.000dc004, "ax"
	.global Func_080cf004
	.type Func_080cf004, %function
	.thumb_func
Func_080cf004:
	.incbin "baserom.gba", 0x000dc004, 0x00000008
	.section .rom.000dc00c, "ax"
	.global Func_080cf00c
	.type Func_080cf00c, %function
	.thumb_func
Func_080cf00c:
	.incbin "baserom.gba", 0x000dc00c, 0x00000044
	.section .rom.000dc050, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000dc050, 0x0000012c
	.section .rom.000dc17c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000dc17c, 0x00000238
	.section .rom.000dc3b4, "ax"
	.global Func_080cf3b4
	.type Func_080cf3b4, %function
	.thumb_func
Func_080cf3b4:
	.incbin "baserom.gba", 0x000dc3b4, 0x00000070
	.section .rom.000dc424, "ax"
	.global Func_080cf424
	.type Func_080cf424, %function
	.thumb_func
Func_080cf424:
	.incbin "baserom.gba", 0x000dc424, 0x00000cd4
	.section .rom.000dd0f8, "ax"
	.global Func_080d00f8
	.type Func_080d00f8, %function
	.thumb_func
Func_080d00f8:
	.incbin "baserom.gba", 0x000dd0f8, 0x000000d4
	.section .rom.000dd1cc, "ax"
	.global Func_080d01cc
	.type Func_080d01cc, %function
	.thumb_func
Func_080d01cc:
	.incbin "baserom.gba", 0x000dd1cc, 0x00000354
	.section .rom.000dd520, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000dd520, 0x0000020c
	.section .rom.000dd72c, "ax"
	.global Func_080d072c
	.type Func_080d072c, %function
	.thumb_func
Func_080d072c:
	.incbin "baserom.gba", 0x000dd72c, 0x0000001c
	.section .rom.000dd748, "ax"
	.global Func_080d0748
	.type Func_080d0748, %function
	.thumb_func
Func_080d0748:
	.incbin "baserom.gba", 0x000dd748, 0x000004a4
	.section .rom.000ddbec, "ax"
	.global Func_080d0bec
	.type Func_080d0bec, %function
	.thumb_func
Func_080d0bec:
	.incbin "baserom.gba", 0x000ddbec, 0x00000064
	.section .rom.000ddc50, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000ddc50, 0x00000a34
	.section .rom.000de684, "ax"
	.global Func_080d1684
	.type Func_080d1684, %function
	.thumb_func
Func_080d1684:
	.incbin "baserom.gba", 0x000de684, 0x00000074
	.section .rom.000de6f8, "ax"
	.global Func_080d16f8
	.type Func_080d16f8, %function
	.thumb_func
Func_080d16f8:
	.incbin "baserom.gba", 0x000de6f8, 0x00000014
	.section .rom.000de70c, "ax"
	.global Func_080d170c
	.type Func_080d170c, %function
	.thumb_func
Func_080d170c:
	.incbin "baserom.gba", 0x000de70c, 0x00000020
	.section .rom.000de72c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000de72c, 0x00000020
	.section .rom.000de74c, "ax"
	.global Func_080d174c
	.type Func_080d174c, %function
	.thumb_func
Func_080d174c:
	.incbin "baserom.gba", 0x000de74c, 0x00000014
	.section .rom.000de760, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000de760, 0x0000004c
	.section .rom.000de7ac, "ax"
	.global BattleFx_StartBufferInterpolation
	.type BattleFx_StartBufferInterpolation, %function
	.thumb_func
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000de7ac, 0x0000003c
	.section .rom.000de7e8, "ax"
	.global Func_080d17e8
	.type Func_080d17e8, %function
	.thumb_func
Func_080d17e8:
	.incbin "baserom.gba", 0x000de7e8, 0x00000034
	.section .rom.000de83e, "ax"
	.incbin "baserom.gba", 0x000de83e, 0x000001da
	.section .rom.000dea18, "ax"
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000dea18, 0x000000ac
	.section .rom.000deac4, "ax"
	.global Func_080d1ac4
	.type Func_080d1ac4, %function
	.thumb_func
Func_080d1ac4:
	.incbin "baserom.gba", 0x000deac4, 0x00000010
	.section .rom.000dead4, "ax"
	.global Func_080d1ad4
	.type Func_080d1ad4, %function
	.thumb_func
Func_080d1ad4:
	.incbin "baserom.gba", 0x000dead4, 0x000001ec
	.section .rom.000decc0, "ax"
	.global Func_080d1cc0
	.type Func_080d1cc0, %function
	.thumb_func
Func_080d1cc0:
	.incbin "baserom.gba", 0x000decc0, 0x00000074
	.section .rom.000ded34, "ax"
	.global Func_080d1d34
	.type Func_080d1d34, %function
	.thumb_func
Func_080d1d34:
	.incbin "baserom.gba", 0x000ded34, 0x00000010
	.section .rom.000ded44, "ax"
	.global Func_080d1d44
	.type Func_080d1d44, %function
	.thumb_func
Func_080d1d44:
	.incbin "baserom.gba", 0x000ded44, 0x00000010
	.section .rom.000ded54, "ax"
	.global Func_080d1d54
	.type Func_080d1d54, %function
	.thumb_func
Func_080d1d54:
	.incbin "baserom.gba", 0x000ded54, 0x00000058
	.section .rom.000dedac, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000dedac, 0x00000044
	.section .rom.000dedf0, "ax"
	.global Func_080d1df0
	.type Func_080d1df0, %function
	.thumb_func
Func_080d1df0:
	.incbin "baserom.gba", 0x000dedf0, 0x00000028
	.section .rom.000dee18, "ax"
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000dee18, 0x00000094
	.section .rom.000deeac, "ax"
	.global Func_080d1eac
	.type Func_080d1eac, %function
	.thumb_func
Func_080d1eac:
	.incbin "baserom.gba", 0x000deeac, 0x0000002c
	.section .rom.000deed8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000deed8, 0x00000010
	.section .rom.000deee8, "ax"
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000deee8, 0x00000024
	.section .rom.000def0c, "ax"
	.global Func_080d1f0c
	.type Func_080d1f0c, %function
	.thumb_func
Func_080d1f0c:
	.incbin "baserom.gba", 0x000def0c, 0x0000000c
	.section .rom.000def18, "ax"
	.global Func_080d1f18
	.type Func_080d1f18, %function
	.thumb_func
Func_080d1f18:
	.incbin "baserom.gba", 0x000def18, 0x00000008
	.section .rom.000def20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000def20, 0x00000164
	.section .rom.000df084, "ax"
	.global Func_080d2084
	.type Func_080d2084, %function
	.thumb_func
Func_080d2084:
	.incbin "baserom.gba", 0x000df084, 0x00000078
	.section .rom.000df0fc, "ax"
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000df0fc, 0x00000144
	.section .rom.000df240, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000df240, 0x00000020
	.section .rom.000df260, "ax"
	.global Func_080d2260
	.type Func_080d2260, %function
	.thumb_func
Func_080d2260:
	.incbin "baserom.gba", 0x000df260, 0x00000048
	.section .rom.000df2a8, "ax"
	.global Func_080d22a8
	.type Func_080d22a8, %function
	.thumb_func
Func_080d22a8:
	.incbin "baserom.gba", 0x000df2a8, 0x000000a8
	.section .rom.000df350, "ax"
	.global Func_080d2350
	.type Func_080d2350, %function
	.thumb_func
Func_080d2350:
	.incbin "baserom.gba", 0x000df350, 0x00000048
	.section .rom.000df398, "ax"
	.global Event_RunObjectHookAndWait
	.type Event_RunObjectHookAndWait, %function
	.thumb_func
Event_RunObjectHookAndWait:
	.incbin "baserom.gba", 0x000df398, 0x00000020
	.section .rom.000df3ca, "ax"
	.incbin "baserom.gba", 0x000df3ca, 0x00000002
	.section .rom.000df3cc, "ax"
	.global Event_SetWorkWord10
	.type Event_SetWorkWord10, %function
	.thumb_func
Event_SetWorkWord10:
	.incbin "baserom.gba", 0x000df3cc, 0x0000000c
	.section .rom.000df488, "ax"
	.global Party_RemoveOwnerRestored
	.type Party_RemoveOwnerRestored, %function
	.thumb_func
Party_RemoveOwnerRestored:
	.incbin "baserom.gba", 0x000df488, 0x00000140
	.section .rom.000df5c8, "ax"
	.global Func_080d25c8
	.type Func_080d25c8, %function
	.thumb_func
Func_080d25c8:
	.incbin "baserom.gba", 0x000df5c8, 0x00000044
	.section .rom.000df60c, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000df60c, 0x000001e0
	.section .rom.000df80a, "ax"
	.incbin "baserom.gba", 0x000df80a, 0x00000036
	.section .rom.000df840, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000df840, 0x0000011c
	.section .rom.000df95c, "ax"
	.global Func_080d295c
	.type Func_080d295c, %function
	.thumb_func
Func_080d295c:
	.incbin "baserom.gba", 0x000df95c, 0x00000010
	.section .rom.000df96c, "ax"
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000df96c, 0x000000a0
	.section .rom.000dfa3c, "ax"
	.global Func_080d2a3c
	.type Func_080d2a3c, %function
	.thumb_func
Func_080d2a3c:
	.incbin "baserom.gba", 0x000dfa3c, 0x00000028
	.section .rom.000dfa64, "ax"
	.global Func_080d2a64
	.type Func_080d2a64, %function
	.thumb_func
Func_080d2a64:
	.incbin "baserom.gba", 0x000dfa64, 0x00000028
	.section .rom.000dfa8c, "ax"
	.global Func_080d2a8c
	.type Func_080d2a8c, %function
	.thumb_func
Func_080d2a8c:
	.incbin "baserom.gba", 0x000dfa8c, 0x00000018
	.section .rom.000dfaa4, "ax"
	.global Func_080d2aa4
	.type Func_080d2aa4, %function
	.thumb_func
Func_080d2aa4:
	.incbin "baserom.gba", 0x000dfaa4, 0x0000002c
	.section .rom.000dfad0, "ax"
	.global Func_080d2ad0
	.type Func_080d2ad0, %function
	.thumb_func
Func_080d2ad0:
	.incbin "baserom.gba", 0x000dfad0, 0x0000002c
	.section .rom.000dfafc, "ax"
	.global Func_080d2afc
	.type Func_080d2afc, %function
	.thumb_func
Func_080d2afc:
	.incbin "baserom.gba", 0x000dfafc, 0x00000010
	.section .rom.000dfb0c, "ax"
	.global Func_080d2b0c
	.type Func_080d2b0c, %function
	.thumb_func
Func_080d2b0c:
	.incbin "baserom.gba", 0x000dfb0c, 0x00000040
	.section .rom.000dfb4c, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
Func_080d2b4c:
	.incbin "baserom.gba", 0x000dfb4c, 0x000000b8
	.section .rom.000dfc64, "ax"
	.global Func_080d2c64
	.type Func_080d2c64, %function
	.thumb_func
Func_080d2c64:
	.incbin "baserom.gba", 0x000dfc64, 0x00000034
	.section .rom.000dfc98, "ax"
	.global Func_080d2c98
	.type Func_080d2c98, %function
	.thumb_func
Func_080d2c98:
	.incbin "baserom.gba", 0x000dfc98, 0x00000004
	.section .rom.000dfc9c, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000dfc9c, 0x00000028
	.section .rom.000dfcc4, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000dfcc4, 0x00000044
	.section .rom.000dfd08, "ax"
	.global Func_080d2d08
	.type Func_080d2d08, %function
	.thumb_func
Func_080d2d08:
	.incbin "baserom.gba", 0x000dfd08, 0x00000068
	.section .rom.000dfd70, "ax"
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000dfd70, 0x00000014
	.section .rom.000dfde4, "ax"
	.global ObjectMotion_ResetTargetsAndVelocity
	.type ObjectMotion_ResetTargetsAndVelocity, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	.incbin "baserom.gba", 0x000dfde4, 0x00000038
	.section .rom.000dff48, "ax"
	.global ObjectMotion_SnapHeadingAndOffset
	.type ObjectMotion_SnapHeadingAndOffset, %function
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	.incbin "baserom.gba", 0x000dff48, 0x00000080
	.section .rom.000e0070, "ax"
	.global Func_080d3070
	.type Func_080d3070, %function
	.thumb_func
Func_080d3070:
	.incbin "baserom.gba", 0x000e0070, 0x0000008c
	.section .rom.000e0116, "ax"
	.incbin "baserom.gba", 0x000e0116, 0x00000002
	.section .rom.000e0118, "ax"
	.global Func_080d3118
	.type Func_080d3118, %function
	.thumb_func
Func_080d3118:
	.incbin "baserom.gba", 0x000e0118, 0x0000004c
	.section .rom.000e0164, "ax"
	.global Func_080d3164
	.type Func_080d3164, %function
	.thumb_func
Func_080d3164:
	.incbin "baserom.gba", 0x000e0164, 0x0000005c
	.section .rom.000e01c0, "ax"
	.global Func_080d31c0
	.type Func_080d31c0, %function
	.thumb_func
Func_080d31c0:
	.incbin "baserom.gba", 0x000e01c0, 0x00000054
	.section .rom.000e0214, "ax"
	.global Func_080d3214
	.type Func_080d3214, %function
	.thumb_func
Func_080d3214:
	.incbin "baserom.gba", 0x000e0214, 0x0000002c
	.section .rom.000e0268, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000e0268, 0x00000050
	.section .rom.000e0300, "ax"
	.global ObjectMotion_SetVariantCallback
	.type ObjectMotion_SetVariantCallback, %function
	.thumb_func
ObjectMotion_SetVariantCallback:
	.incbin "baserom.gba", 0x000e0300, 0x0000002c
	.section .rom.000e033c, "ax"
	.incbin "baserom.gba", 0x000e033c, 0x00000124
	.section .rom.000e0460, "ax"
	.global Func_080d3460
	.type Func_080d3460, %function
	.thumb_func
Func_080d3460:
	.incbin "baserom.gba", 0x000e0460, 0x0000013c
	.section .rom.000e05fe, "ax"
	.incbin "baserom.gba", 0x000e05fe, 0x00000002
	.section .rom.000e0600, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000e0600, 0x000000a8
	.section .rom.000e06a8, "ax"
	.global Func_080d36a8
	.type Func_080d36a8, %function
	.thumb_func
Func_080d36a8:
	.incbin "baserom.gba", 0x000e06a8, 0x00000020
	.section .rom.000e06c8, "ax"
	.global Func_080d36c8
	.type Func_080d36c8, %function
	.thumb_func
Func_080d36c8:
	.incbin "baserom.gba", 0x000e06c8, 0x0000007c
	.section .rom.000e0744, "ax"
	.global Object_SetPartAttribute
	.type Object_SetPartAttribute, %function
	.thumb_func
Object_SetPartAttribute:
	.incbin "baserom.gba", 0x000e0744, 0x0000003c
	.section .rom.000e0780, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000e0780, 0x00000094
	.section .rom.000e0814, "ax"
	.global Func_080d3814
	.type Func_080d3814, %function
	.thumb_func
Func_080d3814:
	.incbin "baserom.gba", 0x000e0814, 0x00000024
	.section .rom.000e0838, "ax"
	.global ObjectMotion_ArmCallback
	.type ObjectMotion_ArmCallback, %function
	.thumb_func
ObjectMotion_ArmCallback:
	.incbin "baserom.gba", 0x000e0838, 0x00000028
	.section .rom.000e0860, "ax"
	.global Func_080d3860
	.type Func_080d3860, %function
	.thumb_func
Func_080d3860:
	.incbin "baserom.gba", 0x000e0860, 0x00000028
	.section .rom.000e0888, "ax"
	.global ObjectMotion_SetActionVariant
	.type ObjectMotion_SetActionVariant, %function
	.thumb_func
ObjectMotion_SetActionVariant:
	.incbin "baserom.gba", 0x000e0888, 0x0000004c
	.section .rom.000e090c, "ax"
	.global Func_080d390c
	.type Func_080d390c, %function
	.thumb_func
Func_080d390c:
	.incbin "baserom.gba", 0x000e090c, 0x0000001c
	.section .rom.000e0928, "ax"
	.global Func_080d3928
	.type Func_080d3928, %function
	.thumb_func
Func_080d3928:
	.incbin "baserom.gba", 0x000e0928, 0x00000018
	.section .rom.000e0940, "ax"
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000e0940, 0x000000d0
	.section .rom.000e0a10, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000e0a10, 0x00000118
	.section .rom.000e0b28, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000e0b28, 0x000000c0
	.section .rom.000e0be8, "ax"
	.global Func_080d3be8
	.type Func_080d3be8, %function
	.thumb_func
Func_080d3be8:
	.incbin "baserom.gba", 0x000e0be8, 0x00000010
	.section .rom.000e0bf8, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000e0bf8, 0x00000034
	.section .rom.000e0c2c, "ax"
	.global Func_080d3c2c
	.type Func_080d3c2c, %function
	.thumb_func
Func_080d3c2c:
	.incbin "baserom.gba", 0x000e0c2c, 0x0000005c
	.section .rom.000e0c88, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000e0c88, 0x00000328
	.section .rom.000e0fb0, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000e0fb0, 0x000000bc
	.section .rom.000e106c, "ax"
	.global Func_080d406c
	.type Func_080d406c, %function
	.thumb_func
Func_080d406c:
	.incbin "baserom.gba", 0x000e106c, 0x00000010
	.section .rom.000e107c, "ax"
	.global Func_080d407c
	.type Func_080d407c, %function
	.thumb_func
Func_080d407c:
	.incbin "baserom.gba", 0x000e107c, 0x00000008
	.section .rom.000e1084, "ax"
	.global Func_080d4084
	.type Func_080d4084, %function
	.thumb_func
Func_080d4084:
	.incbin "baserom.gba", 0x000e1084, 0x00000054
	.section .rom.000e10d8, "ax"
	.global Func_080d40d8
	.type Func_080d40d8, %function
	.thumb_func
Func_080d40d8:
	.incbin "baserom.gba", 0x000e10d8, 0x00000004
	.section .rom.000e10dc, "ax"
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000e10dc, 0x0000009c
	.section .rom.000e1178, "ax"
	.global Func_080d4178
	.type Func_080d4178, %function
	.thumb_func
Func_080d4178:
	.incbin "baserom.gba", 0x000e1178, 0x00000008
	.section .rom.000e1180, "ax"
	.global Event_ShowCounterAtPosition
	.type Event_ShowCounterAtPosition, %function
	.thumb_func
Event_ShowCounterAtPosition:
	.incbin "baserom.gba", 0x000e1180, 0x00000068
	.section .rom.000e11fc, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000e11fc, 0x00000134
	.section .rom.000e1330, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000e1330, 0x00000054
	.section .rom.000e1384, "ax"
	.global Object_AttachWorkTargetToObject
	.type Object_AttachWorkTargetToObject, %function
	.thumb_func
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000e1384, 0x00000068
	.section .rom.000e13ec, "ax"
	.global Func_080d43ec
	.type Func_080d43ec, %function
	.thumb_func
Func_080d43ec:
	.incbin "baserom.gba", 0x000e13ec, 0x00000020
	.section .rom.000e140c, "ax"
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000e140c, 0x00000100
	.section .rom.000e150c, "ax"
	.global Func_080d450c
	.type Func_080d450c, %function
	.thumb_func
Func_080d450c:
	.incbin "baserom.gba", 0x000e150c, 0x00000020
	.section .rom.000e152c, "ax"
	.global Func_080d452c
	.type Func_080d452c, %function
	.thumb_func
Func_080d452c:
	.incbin "baserom.gba", 0x000e152c, 0x0000001c
	.section .rom.000e1548, "ax"
	.global Func_080d4548
	.type Func_080d4548, %function
	.thumb_func
Func_080d4548:
	.incbin "baserom.gba", 0x000e1548, 0x00000020
	.section .rom.000e1568, "ax"
	.global Func_080d4568
	.type Func_080d4568, %function
	.thumb_func
Func_080d4568:
	.incbin "baserom.gba", 0x000e1568, 0x00000050
	.section .rom.000e15b8, "ax"
	.global Func_080d45b8
	.type Func_080d45b8, %function
	.thumb_func
Func_080d45b8:
	.incbin "baserom.gba", 0x000e15b8, 0x000000ec
	.section .rom.000e16a4, "ax"
	.global Func_080d46a4
	.type Func_080d46a4, %function
	.thumb_func
Func_080d46a4:
	.incbin "baserom.gba", 0x000e16a4, 0x00000070
	.section .rom.000e1714, "ax"
	.global Func_080d4714
	.type Func_080d4714, %function
	.thumb_func
Func_080d4714:
	.incbin "baserom.gba", 0x000e1714, 0x000000a0
	.section .rom.000e17b4, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000e17b4, 0x000000e8
	.section .rom.000e189c, "ax"
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000e189c, 0x000000f0
	.section .rom.000e19a0, "ax"
	.incbin "baserom.gba", 0x000e19a0, 0x00000108
	.section .rom.000e1aa8, "ax"
	.global Func_080d4aa8
	.type Func_080d4aa8, %function
	.thumb_func
Func_080d4aa8:
	.incbin "baserom.gba", 0x000e1aa8, 0x0000000c
	.section .rom.000e1ab4, "ax"
	.global Func_080d4ab4
	.type Func_080d4ab4, %function
	.thumb_func
Func_080d4ab4:
	.incbin "baserom.gba", 0x000e1ab4, 0x00000058
	.section .rom.000e1b0c, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000e1b0c, 0x000001fc
	.section .rom.000e1d08, "ax"
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000e1d08, 0x000003f0
	.section .rom.000e20f8, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000e20f8, 0x000002c0
	.section .rom.000e23b8, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000e23b8, 0x00000834
	.section .rom.000e2bec, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000e2bec, 0x00000184
	.section .rom.000e2d70, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000e2d70, 0x00000070
	.section .rom.000e2de0, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000e2de0, 0x00000070
	.section .rom.000e2e62, "ax"
	.incbin "baserom.gba", 0x000e2e62, 0x00000002
	.section .rom.000e2e64, "ax"
	.global Func_080d5e64
	.type Func_080d5e64, %function
	.thumb_func
Func_080d5e64:
	.incbin "baserom.gba", 0x000e2e64, 0x00000014
	.section .rom.000e2e78, "ax"
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000e2e78, 0x000000a0
	.section .rom.000e2fd4, "ax"
	.incbin "baserom.gba", 0x000e2fd4, 0x00000434
	.section .rom.000e3408, "ax"
	.global Func_080d6408
	.type Func_080d6408, %function
	.thumb_func
Func_080d6408:
	.incbin "baserom.gba", 0x000e3408, 0x000000b0
	.section .rom.000e34b8, "ax"
	.global Func_080d64b8
	.type Func_080d64b8, %function
	.thumb_func
Func_080d64b8:
	.incbin "baserom.gba", 0x000e34b8, 0x00000454
	.section .rom.000e390c, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000e390c, 0x00000284
	.section .rom.000e3b90, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000e3b90, 0x000002c8
	.section .rom.000e3e58, "ax"
	.global Func_080d6e58
	.type Func_080d6e58, %function
	.thumb_func
Func_080d6e58:
	.incbin "baserom.gba", 0x000e3e58, 0x000000dc
	.section .rom.000e3f34, "ax"
	.global Func_080d6f34
	.type Func_080d6f34, %function
	.thumb_func
Func_080d6f34:
	.incbin "baserom.gba", 0x000e3f34, 0x000000f0
	.section .rom.000e4024, "ax"
	.global Func_080d7024
	.type Func_080d7024, %function
	.thumb_func
Func_080d7024:
	.incbin "baserom.gba", 0x000e4024, 0x0000021c
	.section .rom.000e4240, "ax"
	.global Func_080d7240
	.type Func_080d7240, %function
	.thumb_func
Func_080d7240:
	.incbin "baserom.gba", 0x000e4240, 0x000000c4
	.section .rom.000e4304, "ax"
	.global Func_080d7304
	.type Func_080d7304, %function
	.thumb_func
Func_080d7304:
	.incbin "baserom.gba", 0x000e4304, 0x000000b0
	.section .rom.000e43b4, "ax"
	.global Func_080d73b4
	.type Func_080d73b4, %function
	.thumb_func
Func_080d73b4:
	.incbin "baserom.gba", 0x000e43b4, 0x0000002c
	.section .rom.000e43e0, "ax"
	.global Func_080d73e0
	.type Func_080d73e0, %function
	.thumb_func
Func_080d73e0:
	.incbin "baserom.gba", 0x000e43e0, 0x00000028
	.section .rom.000e4408, "ax"
	.global Func_080d7408
	.type Func_080d7408, %function
	.thumb_func
Func_080d7408:
	.incbin "baserom.gba", 0x000e4408, 0x00000028
	.section .rom.000e4430, "ax"
	.global Func_080d7430
	.type Func_080d7430, %function
	.thumb_func
Func_080d7430:
	.incbin "baserom.gba", 0x000e4430, 0x000000c0
	.section .rom.000e4524, "ax"
	.incbin "baserom.gba", 0x000e4524, 0x00000264
	.section .rom.000e4788, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000e4788, 0x000002f0
	.section .rom.000e4a78, "ax"
	.global BattleFx_InitializeSlots
	.type BattleFx_InitializeSlots, %function
	.thumb_func
BattleFx_InitializeSlots:
	.incbin "baserom.gba", 0x000e4a78, 0x0000003c
	.section .rom.000e4ab4, "ax"
	.global Func_080d7ab4
	.type Func_080d7ab4, %function
	.thumb_func
Func_080d7ab4:
	.incbin "baserom.gba", 0x000e4ab4, 0x00000c58
	.section .rom.000e5740, "ax"
	.incbin "baserom.gba", 0x000e5740, 0x0000023c
	.section .rom.000e597c, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000e597c, 0x000003ec
	.section .rom.000e5d68, "ax"
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000e5d68, 0x0000039c
	.section .rom.000e6104, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000e6104, 0x000001d0
	.section .rom.000e62d4, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000e62d4, 0x0000043c
	.section .rom.000e6710, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000e6710, 0x000003a0
	.section .rom.000e6ab0, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000e6ab0, 0x00000020
	.section .rom.000e6ad0, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000e6ad0, 0x00000038
	.section .rom.000e6b08, "ax"
	.global Func_080d9b08
	.type Func_080d9b08, %function
	.thumb_func
Func_080d9b08:
	.incbin "baserom.gba", 0x000e6b08, 0x00000390
	.section .rom.000e6e98, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000e6e98, 0x00000a70
	.section .rom.000e7908, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000e7908, 0x00000030
	.section .rom.000e7938, "ax"
	.global Func_080da938
	.type Func_080da938, %function
	.thumb_func
Func_080da938:
	.incbin "baserom.gba", 0x000e7938, 0x00000594
	.section .rom.000e7ecc, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000e7ecc, 0x000001e4
	.section .rom.000e80b0, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000e80b0, 0x00000394
	.section .rom.000e8444, "ax"
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000e8444, 0x00000048
	.section .rom.000e84b6, "ax"
	.incbin "baserom.gba", 0x000e84b6, 0x00000002
	.section .rom.000e84b8, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000e84b8, 0x000001b8
	.section .rom.000e8670, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e8670, 0x000001d8
	.section .rom.000e8848, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000e8848, 0x0000003c
	.section .rom.000e8884, "ax"
	.global FieldEvent_RunTypeHandler
	.type FieldEvent_RunTypeHandler, %function
	.thumb_func
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000e8884, 0x00000098
	.section .rom.000e891c, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.type ObjectGroup_ApplyRandomChildValues, %function
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000e891c, 0x000000a4
	.section .rom.000e89c0, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000e89c0, 0x0000000c
	.section .rom.000e89cc, "ax"
	.global Func_080db9cc
	.type Func_080db9cc, %function
	.thumb_func
Func_080db9cc:
	.incbin "baserom.gba", 0x000e89cc, 0x0000030c
	.section .rom.000e8cd8, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000e8cd8, 0x00000070
	.section .rom.000e8d48, "ax"
	.global Func_080dbd48
	.type Func_080dbd48, %function
	.thumb_func
Func_080dbd48:
	.incbin "baserom.gba", 0x000e8d48, 0x00000080
	.section .rom.000e8dc8, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e8dc8, 0x0000000c
	.section .rom.000e8dd4, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e8dd4, 0x00000014
	.section .rom.000e8de8, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000e8de8, 0x0000000c
	.section .rom.000e8df4, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e8df4, 0x00000014
	.section .rom.000e8e08, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000e8e08, 0x0000003c
	.section .rom.000e8e44, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000e8e44, 0x00000274
	.section .rom.000e90b8, "ax"
	.global Func_080dc0b8
	.type Func_080dc0b8, %function
	.thumb_func
Func_080dc0b8:
	.incbin "baserom.gba", 0x000e90b8, 0x00000020
	.section .rom.000e90d8, "ax"
	.global Func_080dc0d8
	.type Func_080dc0d8, %function
	.thumb_func
Func_080dc0d8:
	.incbin "baserom.gba", 0x000e90d8, 0x00000034
	.section .rom.000e910c, "ax"
	.global Object_Spawn
	.type Object_Spawn, %function
	.thumb_func
Object_Spawn:
	.incbin "baserom.gba", 0x000e910c, 0x000000a4
	.section .rom.000e91b0, "ax"
	.global Func_080dc1b0
	.type Func_080dc1b0, %function
	.thumb_func
Func_080dc1b0:
	.incbin "baserom.gba", 0x000e91b0, 0x00000094
	.section .rom.000e9244, "ax"
	.global Field_BeginPaletteTransition
	.type Field_BeginPaletteTransition, %function
	.thumb_func
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000e9244, 0x00000050
	.section .rom.000e9294, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e9294, 0x000000f0
	.section .rom.000e9384, "ax"
	.global BattleFx_PrepareBufferInterpolation
	.type BattleFx_PrepareBufferInterpolation, %function
	.thumb_func
BattleFx_PrepareBufferInterpolation:
	.incbin "baserom.gba", 0x000e9384, 0x0000008c
	.section .rom.000e9410, "ax"
	.global Func_080dc410
	.type Func_080dc410, %function
	.thumb_func
Func_080dc410:
	.incbin "baserom.gba", 0x000e9410, 0x0000021c
	.section .rom.000e962c, "ax"
	.global Func_080dc62c
	.type Func_080dc62c, %function
	.thumb_func
Func_080dc62c:
	.incbin "baserom.gba", 0x000e962c, 0x000000ac
	.section .rom.000e96d8, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e96d8, 0x000000f4
	.section .rom.000e97cc, "ax"
	.global Func_080dc7cc
	.type Func_080dc7cc, %function
	.thumb_func
Func_080dc7cc:
	.incbin "baserom.gba", 0x000e97cc, 0x0000001c
	.section .rom.000e97e8, "ax"
	.global Func_080dc7e8
	.type Func_080dc7e8, %function
	.thumb_func
Func_080dc7e8:
	.incbin "baserom.gba", 0x000e97e8, 0x00000190
	.section .rom.000e9978, "ax"
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000e9978, 0x000000d8
	.section .rom.000e9a50, "ax"
	.global Func_080dca50
	.type Func_080dca50, %function
	.thumb_func
Func_080dca50:
	.incbin "baserom.gba", 0x000e9a50, 0x00000034
	.section .rom.000e9a84, "ax"
	.global Func_080dca84
	.type Func_080dca84, %function
	.thumb_func
Func_080dca84:
	.incbin "baserom.gba", 0x000e9a84, 0x00000058
	.section .rom.000e9adc, "ax"
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000e9adc, 0x00000578
	.section .rom.000ea054, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000ea054, 0x000004d4
	.section .rom.000ea528, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000ea528, 0x00000114
	.section .rom.000ea63c, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000ea63c, 0x0000002c
	.section .rom.000ea668, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000ea668, 0x00000f10
	.section .rom.000eb5a4, "ax"
	.incbin "baserom.gba", 0x000eb5a4, 0x00000714
	.section .rom.000ebcb8, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000ebcb8, 0x0000101c
	.section .rom.000eccf8, "ax"
	.incbin "baserom.gba", 0x000eccf8, 0x000005f4
	.section .rom.000ed308, "ax"
	.global Func_080e0308
	.type Func_080e0308, %function
	.thumb_func
Func_080e0308:
	.incbin "baserom.gba", 0x000ed308, 0x00000054
	.section .rom.000ed35c, "ax"
	.global Func_080e035c
	.type Func_080e035c, %function
	.thumb_func
Func_080e035c:
	.incbin "baserom.gba", 0x000ed35c, 0x00000050
	.section .rom.000ed3c2, "ax"
	.incbin "baserom.gba", 0x000ed3c2, 0x00000fba
	.section .rom.000ee37c, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000ee37c, 0x000000a4
	.section .rom.000ee420, "ax"
	.global Func_080e1420
	.type Func_080e1420, %function
	.thumb_func
Func_080e1420:
	.incbin "baserom.gba", 0x000ee420, 0x000001dc
	.section .rom.000ee5fc, "ax"
	.global Func_080e15fc
	.type Func_080e15fc, %function
	.thumb_func
Func_080e15fc:
	.incbin "baserom.gba", 0x000ee5fc, 0x00000054
	.section .rom.000ee650, "ax"
	.global Func_080e1650
	.type Func_080e1650, %function
	.thumb_func
Func_080e1650:
	.incbin "baserom.gba", 0x000ee650, 0x00000f98
	.section .rom.000ef5e8, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000ef5e8, 0x0000018c
	.section .rom.000ef774, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000ef774, 0x00000160
	.section .rom.000ef8d4, "ax"
	.global Func_080e28d4
	.type Func_080e28d4, %function
	.thumb_func
Func_080e28d4:
	.incbin "baserom.gba", 0x000ef8d4, 0x00000dc4
	.section .rom.000f0698, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000f0698, 0x00000148
	.section .rom.000f07e0, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000f07e0, 0x00000a64
	.section .rom.000f1244, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000f1244, 0x00002eb4
	.section .rom.000f40f8, "ax"
	.global Func_080e70f8
	.type Func_080e70f8, %function
	.thumb_func
Func_080e70f8:
	.incbin "baserom.gba", 0x000f40f8, 0x00000140
	.section .rom.000f4238, "ax"
	.global Func_080e7238
	.type Func_080e7238, %function
	.thumb_func
Func_080e7238:
	.incbin "baserom.gba", 0x000f4238, 0x000002a0
	.section .rom.000f44d8, "ax"
	.global Func_080e74d8
	.type Func_080e74d8, %function
	.thumb_func
Func_080e74d8:
	.incbin "baserom.gba", 0x000f44d8, 0x0000032c
	.section .rom.000f4804, "ax"
	.global Func_080e7804
	.type Func_080e7804, %function
	.thumb_func
Func_080e7804:
	.incbin "baserom.gba", 0x000f4804, 0x00002948
	.section .rom.000f714c, "ax"
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000f714c, 0x00000674
	.section .rom.000f77c0, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f77c0, 0x00000114
	.section .rom.000f78d4, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f78d4, 0x00000140
	.section .rom.000f7a14, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000f7a14, 0x0000015c
	.section .rom.000f7b70, "ax"
	.global Func_080eab70
	.type Func_080eab70, %function
	.thumb_func
Func_080eab70:
	.incbin "baserom.gba", 0x000f7b70, 0x00000028
	.section .rom.000f7b98, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000f7b98, 0x00000038
	.section .rom.000f7bd0, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000f7bd0, 0x00000118
	.section .rom.000f7ce8, "ax"
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000f7ce8, 0x00000114
	.section .rom.000f7dfc, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000f7dfc, 0x000000b8
	.section .rom.000f7eb4, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000f7eb4, 0x00000074
	.section .rom.000f7f28, "ax"
	.global Func_080eaf28
	.type Func_080eaf28, %function
	.thumb_func
Func_080eaf28:
	.incbin "baserom.gba", 0x000f7f28, 0x00000070
	.section .rom.000f7f98, "ax"
	.global Func_080eaf98
	.type Func_080eaf98, %function
	.thumb_func
Func_080eaf98:
	.incbin "baserom.gba", 0x000f7f98, 0x00000084
	.section .rom.000f801c, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000f801c, 0x0000027c
	.section .rom.000f8298, "ax"
	.global Func_080eb298
	.type Func_080eb298, %function
	.thumb_func
Func_080eb298:
	.incbin "baserom.gba", 0x000f8298, 0x00000030
	.section .rom.000f82c8, "ax"
	.global Func_080eb2c8
	.type Func_080eb2c8, %function
	.thumb_func
Func_080eb2c8:
	.incbin "baserom.gba", 0x000f82c8, 0x00000008
	.section .rom.000f82d0, "ax"
	.global Func_080eb2d0
	.type Func_080eb2d0, %function
	.thumb_func
Func_080eb2d0:
	.incbin "baserom.gba", 0x000f82d0, 0x00000690
	.section .rom.000f8960, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f8960, 0x000002d0
	.section .rom.000f8c30, "ax"
	.global Func_080ebc30
	.type Func_080ebc30, %function
	.thumb_func
Func_080ebc30:
	.incbin "baserom.gba", 0x000f8c30, 0x00000240
	.section .rom.000f8e70, "ax"
	.global BattleFx_HasReachedTarget
	.type BattleFx_HasReachedTarget, %function
	.thumb_func
BattleFx_HasReachedTarget:
	.incbin "baserom.gba", 0x000f8e70, 0x00000024
	.section .rom.000f8ea6, "ax"
	.incbin "baserom.gba", 0x000f8ea6, 0x00000002
	.section .rom.000f8ea8, "ax"
	.global Func_080ebea8
	.type Func_080ebea8, %function
	.thumb_func
Func_080ebea8:
	.incbin "baserom.gba", 0x000f8ea8, 0x0000000c
	.section .rom.000f8eb4, "ax"
	.global Func_080ebeb4
	.type Func_080ebeb4, %function
	.thumb_func
Func_080ebeb4:
	.incbin "baserom.gba", 0x000f8eb4, 0x00000014
	.section .rom.000f8ec8, "ax"
	.global Func_080ebec8
	.type Func_080ebec8, %function
	.thumb_func
Func_080ebec8:
	.incbin "baserom.gba", 0x000f8ec8, 0x000000a0
	.section .rom.000f8f68, "ax"
	.global BattleFx_ClearOwnedSlot
	.type BattleFx_ClearOwnedSlot, %function
	.thumb_func
BattleFx_ClearOwnedSlot:
	.incbin "baserom.gba", 0x000f8f68, 0x0000189c
	.section .rom.000fa804, "ax"
	.global Func_080ed804
	.type Func_080ed804, %function
	.thumb_func
Func_080ed804:
	.incbin "baserom.gba", 0x000fa804, 0x00003650
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000fde54, 0x000000d8
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000fdf2c, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000fe7a8, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x001002d0, 0x000004b0
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x00100780, 0x00001880
	.section .rom.00102028, "ax"
	.incbin "baserom.gba", 0x00102028, 0x00000048
	.section .rom.001020e0, "ax"
	.incbin "baserom.gba", 0x001020e0, 0x0000003c
	.section .rom.00102170, "ax"
	.incbin "baserom.gba", 0x00102170, 0x000004a0
	.section .rom.00102656, "ax"
	.incbin "baserom.gba", 0x00102656, 0x000001ea
	.section .rom.00102840, "ax"
	.global UiIcon_CreateWithResourceVariant
	.type UiIcon_CreateWithResourceVariant, %function
	.thumb_func
UiIcon_CreateWithResourceVariant:
	.incbin "baserom.gba", 0x00102840, 0x00000048
	.section .rom.00102888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x00102888, 0x00000390
	.section .rom.00102c94, "ax"
	.incbin "baserom.gba", 0x00102c94, 0x000002ac
	.section .rom.00102f9c, "ax"
	.incbin "baserom.gba", 0x00102f9c, 0x000001d4
	.section .rom.00103222, "ax"
	.incbin "baserom.gba", 0x00103222, 0x00000182
	.section .rom.001033b6, "ax"
	.incbin "baserom.gba", 0x001033b6, 0x000000ee
	.section .rom.001034a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x001034a4, 0x00001844
	.section .rom.00104cfa, "ax"
	.incbin "baserom.gba", 0x00104cfa, 0x000003f2
	.section .rom.0010514c, "ax"
	.incbin "baserom.gba", 0x0010514c, 0x00000cac
	.section .rom.00105e60, "ax"
	.incbin "baserom.gba", 0x00105e60, 0x00000d54
	.section .rom.00106bb4, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00106bb4, 0x0000177c
	.section .rom.00108330, "ax"
	.global Func_080fe184
	.type Func_080fe184, %function
	.thumb_func
Func_080fe184:
	.incbin "baserom.gba", 0x00108330, 0x000000f0
	.section .rom.00108420, "ax"
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x00108420, 0x000023d0
	.section .rom.0010a82c, "ax"
	.incbin "baserom.gba", 0x0010a82c, 0x00004a58
	.section .rom.0010f284, "ax"
	.global Menu_ReleaseEntryObjects
	.type Menu_ReleaseEntryObjects, %function
	.thumb_func
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x0010f284, 0x000000f0
	.section .rom.0010f374, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010f374, 0x000000c4
	.section .rom.0010f438, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x0010f438, 0x00000040
	.section .rom.0010f478, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x0010f478, 0x00000054
	.section .rom.0010f4cc, "ax"
	.global Func_0810526c
	.type Func_0810526c, %function
	.thumb_func
Func_0810526c:
	.incbin "baserom.gba", 0x0010f4cc, 0x0000002c
	.section .rom.0010f4f8, "ax"
	.global Func_081050b8
	.type Func_081050b8, %function
	.thumb_func
Func_081050b8:
	.incbin "baserom.gba", 0x0010f4f8, 0x00000024
	.section .rom.0010f51c, "ax"
	.global Func_081052ac
	.type Func_081052ac, %function
	.thumb_func
Func_081052ac:
	.incbin "baserom.gba", 0x0010f51c, 0x00000230
	.section .rom.0010f7c2, "ax"
	.incbin "baserom.gba", 0x0010f7c2, 0x00000376
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x0010fb38, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x0010fb3c, 0x0000007e
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x0010fbba, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x0010fbc7, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x0010fbd4, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x0010fbec, 0x00000414
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00110000, 0x00000088
	.section .rom.001100a8, "ax"
	.incbin "baserom.gba", 0x001100a8, 0x00000438
	.section .rom.001104f4, "ax"
	.incbin "baserom.gba", 0x001104f4, 0x00000640
	.section .rom.00110b70, "ax"
	.incbin "baserom.gba", 0x00110b70, 0x00000f28
	.section .rom.00111ad8, "ax"
	.incbin "baserom.gba", 0x00111ad8, 0x00000d30
	.section .rom.00112808, "ax"
	.global Func_0810a804
	.type Func_0810a804, %function
	.thumb_func
Func_0810a804:
	.incbin "baserom.gba", 0x00112808, 0x00000030
	.section .rom.00112838, "ax"
	.global Func_0810a834
	.type Func_0810a834, %function
	.thumb_func
Func_0810a834:
	.incbin "baserom.gba", 0x00112838, 0x00000018
	.section .rom.00112850, "ax"
	.global Func_0810a84c
	.type Func_0810a84c, %function
	.thumb_func
Func_0810a84c:
	.incbin "baserom.gba", 0x00112850, 0x00000018
	.section .rom.001128f0, "ax"
	.incbin "baserom.gba", 0x001128f0, 0x00005710
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000078
	.section .rom.00118078, "ax"
	.global BattleMotion_ApproachTargetFar
	.type BattleMotion_ApproachTargetFar, %function
	.thumb_func
BattleMotion_ApproachTargetFar:
	.incbin "baserom.gba", 0x00118078, 0x00000068
	.section .rom.001180e0, "ax"
	.global BattlePres_SetActorModesFar
	.type BattlePres_SetActorModesFar, %function
	.thumb_func
BattlePres_SetActorModesFar:
	.incbin "baserom.gba", 0x001180e0, 0x00000080
	.section .rom.00118230, "ax"
	.incbin "baserom.gba", 0x00118230, 0x00000008
	.section .rom.00118244, "ax"
	.incbin "baserom.gba", 0x00118244, 0x00000040
	.section .rom.00118394, "ax"
	.incbin "baserom.gba", 0x00118394, 0x00001cbc
	.section .rom.0011a050, "ax"
	.global BattleParty_PrepareActiveOwners
	.type BattleParty_PrepareActiveOwners, %function
	.thumb_func
BattleParty_PrepareActiveOwners:
	.incbin "baserom.gba", 0x0011a050, 0x00000150
	.section .rom.0011a264, "ax"
	.incbin "baserom.gba", 0x0011a264, 0x000000d0
	.section .rom.0011a334, "ax"
	.global BattleParty_ListActorIds
	.type BattleParty_ListActorIds, %function
	.thumb_func
BattleParty_ListActorIds:
	.incbin "baserom.gba", 0x0011a334, 0x00000130
	.section .rom.0011a49a, "ax"
	.incbin "baserom.gba", 0x0011a49a, 0x0000000e
	.section .rom.0011a4f6, "ax"
	.incbin "baserom.gba", 0x0011a4f6, 0x00000c8e
	.section .rom.0011b198, "ax"
	.incbin "baserom.gba", 0x0011b198, 0x00000524
	.section .rom.0011b73c, "ax"
	.incbin "baserom.gba", 0x0011b73c, 0x000002b0
	.section .rom.0011b9ec, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x0011b9ec, 0x0000000c
	.section .rom.0011b9f8, "ax"
	.global ResetMotionRecordGroup
	.type ResetMotionRecordGroup, %function
	.thumb_func
ResetMotionRecordGroup:
	.incbin "baserom.gba", 0x0011b9f8, 0x0000024c
	.section .rom.0011bc7c, "ax"
	.incbin "baserom.gba", 0x0011bc7c, 0x000001d8
	.section .rom.0011be54, "ax"
	.global GetBattleObjectSlot
	.type GetBattleObjectSlot, %function
	.thumb_func
GetBattleObjectSlot:
	.incbin "baserom.gba", 0x0011be54, 0x0000002c
	.section .rom.0011bede, "ax"
	.incbin "baserom.gba", 0x0011bede, 0x000000da
	.section .rom.0011bfe8, "ax"
	.incbin "baserom.gba", 0x0011bfe8, 0x00000228
	.section .rom.0011c228, "ax"
	.incbin "baserom.gba", 0x0011c228, 0x00000440
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x000010b6
	.section .rom.0011d760, "ax"
	.incbin "baserom.gba", 0x0011d760, 0x00000c24
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00001b5e
	.section .rom.0011ff20, "ax"
	.global BattlePresentation_WaitForAdvance
	.type BattlePresentation_WaitForAdvance, %function
	.thumb_func
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x0011ff20, 0x000002ac
	.section .rom.001201dc, "ax"
	.global BattleEv_DispatchQueued
	.type BattleEv_DispatchQueued, %function
	.thumb_func
BattleEv_DispatchQueued:
	.incbin "baserom.gba", 0x001201dc, 0x000001c4
	.section .rom.001203c0, "ax"
	.incbin "baserom.gba", 0x001203c0, 0x000000ac
	.section .rom.0012046c, "ax"
	.global Battle_ResolveTargetAction
	.type Battle_ResolveTargetAction, %function
	.thumb_func
Battle_ResolveTargetAction:
	.incbin "baserom.gba", 0x0012046c, 0x0000206c
	.section .rom.001224f0, "ax"
	.incbin "baserom.gba", 0x001224f0, 0x0000105c
	.section .rom.0012358c, "ax"
	.incbin "baserom.gba", 0x0012358c, 0x0000129c
	.section .rom.00124b10, "ax"
	.incbin "baserom.gba", 0x00124b10, 0x000000b0
	.section .rom.00124cd6, "ax"
	.incbin "baserom.gba", 0x00124cd6, 0x00000eba
	.section .rom.00125bd0, "ax"
	.incbin "baserom.gba", 0x00125bd0, 0x00000c4c
	.section .rom.0012681c, "ax"
	.global BattlePres_SetupTransitionScene
	.type BattlePres_SetupTransitionScene, %function
	.thumb_func
BattlePres_SetupTransitionScene:
	.incbin "baserom.gba", 0x0012681c, 0x00000100
	.section .rom.0012695a, "ax"
	.incbin "baserom.gba", 0x0012695a, 0x000001a2
	.section .rom.00126afc, "ax"
	.global BattlePres_SetActorRecordMode
	.type BattlePres_SetActorRecordMode, %function
	.thumb_func
BattlePres_SetActorRecordMode:
	.incbin "baserom.gba", 0x00126afc, 0x00000080
	.section .rom.00126be4, "ax"
	.incbin "baserom.gba", 0x00126be4, 0x00000130
	.section .rom.00126d14, "ax"
	.global BattlePres_SetActorModes
	.type BattlePres_SetActorModes, %function
	.thumb_func
BattlePres_SetActorModes:
	.incbin "baserom.gba", 0x00126d14, 0x000013a4
	.section .rom.001280d4, "ax"
	.incbin "baserom.gba", 0x001280d4, 0x00000040
	.section .rom.00128114, "ax"
	.global Summon_IsEntryFlagged
	.type Summon_IsEntryFlagged, %function
	.thumb_func
Summon_IsEntryFlagged:
	.incbin "baserom.gba", 0x00128114, 0x000000b4
	.section .rom.00128202, "ax"
	.incbin "baserom.gba", 0x00128202, 0x0000065a
	.global Data_08128844
Data_08128844:
	.incbin "baserom.gba", 0x0012885c, 0x000084c8
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x00130d24, 0x000072dc
	.section .rom.001380ac, "ax"
	.incbin "baserom.gba", 0x001380ac, 0x00000f1c
	.section .rom.0013ba50, "ax"
	.incbin "baserom.gba", 0x0013ba50, 0x00002980
	.section .rom.0013e3d0, "ax"
	.global Func_0813e3d0
	.type Func_0813e3d0, %function
	.thumb_func
Func_0813e3d0:
	.incbin "baserom.gba", 0x0013e3d0, 0x000007a0
	.section .rom.0013eb70, "ax"
	.global Func_0813eb70
	.type Func_0813eb70, %function
	.thumb_func
Func_0813eb70:
	.incbin "baserom.gba", 0x0013eb70, 0x000009a8
	.section .rom.0013f518, "ax"
	.global Func_0813f518
	.type Func_0813f518, %function
	.thumb_func
Func_0813f518:
	.incbin "baserom.gba", 0x0013f518, 0x00000570
	.section .rom.0013fa88, "ax"
	.global Func_0813fa88
	.type Func_0813fa88, %function
	.thumb_func
Func_0813fa88:
	.incbin "baserom.gba", 0x0013fa88, 0x000002b8
	.section .rom.0013fd40, "ax"
	.global Func_0813fd40
	.type Func_0813fd40, %function
	.thumb_func
Func_0813fd40:
	.incbin "baserom.gba", 0x0013fd40, 0x00002c04
	.section .rom.00142944, "ax"
	.global Func_08142944
	.type Func_08142944, %function
	.thumb_func
Func_08142944:
	.incbin "baserom.gba", 0x00142944, 0x00007268
	.section .rom.00149bac, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x00149bac, 0x00002dd0
	.section .rom.0014c992, "ax"
	.incbin "baserom.gba", 0x0014c992, 0x00000002
	.section .rom.0014c994, "ax"
	.global Func_0814c994
	.type Func_0814c994, %function
	.thumb_func
Func_0814c994:
	.incbin "baserom.gba", 0x0014c994, 0x000001cc
	.section .rom.0014cb60, "ax"
	.global Func_0814cb60
	.type Func_0814cb60, %function
	.thumb_func
Func_0814cb60:
	.incbin "baserom.gba", 0x0014cb60, 0x00005910
	.section .rom.00152470, "ax"
	.global BattleFx_RunNoEffect
	.type BattleFx_RunNoEffect, %function
	.thumb_func
BattleFx_RunNoEffect:
	.incbin "baserom.gba", 0x00152470, 0x0000517c
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x00002a0e
	.section .rom.0015a0bc, "ax"
	.incbin "baserom.gba", 0x0015a0bc, 0x00000030
	.section .rom.0015a110, "ax"
	.global BattlePres_RunBurstScene
	.type BattlePres_RunBurstScene, %function
	.thumb_func
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x00004210
	.section .rom.0015e320, "ax"
	.global Func_0815e320
	.type Func_0815e320, %function
	.thumb_func
Func_0815e320:
	.incbin "baserom.gba", 0x0015e320, 0x0000bef8
	.section .rom.0016a244, "ax"
	.incbin "baserom.gba", 0x0016a244, 0x0000002c
	.section .rom.0016a29c, "ax"
	.incbin "baserom.gba", 0x0016a29c, 0x00014778
	.section .rom.0017ea58, "ax"
	.incbin "baserom.gba", 0x0017ea58, 0x00017994
	.section .rom.001963ec, "ax"
	.global Func_081963ec
	.type Func_081963ec, %function
	.thumb_func
Func_081963ec:
	.incbin "baserom.gba", 0x001963ec, 0x00000018
	.section .rom.00196404, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000e2c
	.section .rom.001973f0, "ax"
	.incbin "baserom.gba", 0x001973f0, 0x00008c10
	.global Resource_FarCall00A
Resource_FarCall00A:
	.incbin "baserom.gba", 0x001a0000, 0x00000030
	.section .rom.001a04d0, "ax"
	.incbin "baserom.gba", 0x001a04d0, 0x00000db8
	.section .rom.001a1294, "ax"
	.incbin "baserom.gba", 0x001a1294, 0x00004d6c
	.global Resource_FarCall00B
Resource_FarCall00B:
	.incbin "baserom.gba", 0x001a6000, 0x00002284
	.section .rom.001a82a6, "ax"
	.incbin "baserom.gba", 0x001a82a6, 0x00003d5a
	.section .rom.001ac026, "ax"
	.incbin "baserom.gba", 0x001ac026, 0x0000008a
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x00005ea4
	.section .rom.001b2008, "ax"
	.global Func_081b2008
	.type Func_081b2008, %function
	.thumb_func
Func_081b2008:
	.incbin "baserom.gba", 0x001b2008, 0x00001ea8
	.section .rom.001b3ed8, "ax"
	.incbin "baserom.gba", 0x001b3ed8, 0x00004128
	.section .rom.001b8008, "ax"
	.global Func_081b8008
	.type Func_081b8008, %function
	.thumb_func
Func_081b8008:
	.incbin "baserom.gba", 0x001b8008, 0x00007ff8
	.section .rom.001c00b0, "ax"
	.incbin "baserom.gba", 0x001c00b0, 0x00000064
	.global LeftoverSoundDriver_StaleCallTarget
LeftoverSoundDriver_StaleCallTarget:
	.incbin "baserom.gba", 0x001c0114, 0x00000054
	.section .rom.001c0168, "ax"
	.global Func_081c0168
	.type Func_081c0168, %function
	.thumb_func
Func_081c0168:
	.incbin "baserom.gba", 0x001c0168, 0x00000030
	.section .rom.001c03d8, "ax"
	.global Func_081c03d8
	.type Func_081c03d8, %function
	.thumb_func
Func_081c03d8:
	.incbin "baserom.gba", 0x001c03d8, 0x0000001c
	.section .rom.001c03fe, "ax"
	.incbin "baserom.gba", 0x001c03fe, 0x00000002
	.section .rom.001c0428, "ax"
	.global Func_081c0428
Func_081c0428:
	.incbin "baserom.gba", 0x001c0428, 0x000001c0
	.section .rom.001c075a, "ax"
	.incbin "baserom.gba", 0x001c075a, 0x00000048
	.section .rom.001c07a2, "ax"
	.global MusicTrack_HandleNote
	.type MusicTrack_HandleNote, %function
	.thumb_func
MusicTrack_HandleNote:
	.incbin "baserom.gba", 0x001c07a2, 0x00000102
	.section .rom.001c08a4, "ax"
	.global Func_081c08a4
	.type Func_081c08a4, %function
	.thumb_func
Func_081c08a4:
	.incbin "baserom.gba", 0x001c08a4, 0x0000012c
	.section .rom.001c09d0, "ax"
	.global MusicPlayer_Tick
	.type MusicPlayer_Tick, %function
	.thumb_func
MusicPlayer_Tick:
	.incbin "baserom.gba", 0x001c09d0, 0x00000248
	.section .rom.001c0c18, "ax"
	.global Func_081c0c18
	.type Func_081c0c18, %function
	.thumb_func
Func_081c0c18:
	.incbin "baserom.gba", 0x001c0c18, 0x00000004
	.section .rom.001c0c1c, "ax"
	.global Func_081c0c1c
	.type Func_081c0c1c, %function
	.thumb_func
Func_081c0c1c:
	.incbin "baserom.gba", 0x001c0c1c, 0x00000094
	.section .rom.001c0cb0, "ax"
	.global Func_081c0cb0
	.type Func_081c0cb0, %function
	.thumb_func
Func_081c0cb0:
	.incbin "baserom.gba", 0x001c0cb0, 0x00000180
	.section .rom.001c0e30, "ax"
	.global Func_081c0e30
	.type Func_081c0e30, %function
	.thumb_func
Func_081c0e30:
	.incbin "baserom.gba", 0x001c0e30, 0x00000040
	.section .rom.001c0e70, "ax"
	.global Func_081c0e70
	.type Func_081c0e70, %function
	.thumb_func
Func_081c0e70:
	.incbin "baserom.gba", 0x001c0e70, 0x000000cc
	.section .rom.001c0f3c, "ax"
	.global Func_081c0f3c
	.type Func_081c0f3c, %function
	.thumb_func
Func_081c0f3c:
	.incbin "baserom.gba", 0x001c0f3c, 0x00000008
	.section .rom.001c0f44, "ax"
	.global Func_081c0f44
	.type Func_081c0f44, %function
	.thumb_func
Func_081c0f44:
	.incbin "baserom.gba", 0x001c0f44, 0x00000014
	.section .rom.001c0f70, "ax"
	.global Func_081c0f70
	.type Func_081c0f70, %function
	.thumb_func
Func_081c0f70:
	.incbin "baserom.gba", 0x001c0f70, 0x00000014
	.section .rom.001c0fac, "ax"
	.global Func_081c0fac
	.type Func_081c0fac, %function
	.thumb_func
Func_081c0fac:
	.incbin "baserom.gba", 0x001c0fac, 0x00000014
	.section .rom.001c0fc0, "ax"
	.global Func_081c0fc0
	.type Func_081c0fc0, %function
	.thumb_func
Func_081c0fc0:
	.incbin "baserom.gba", 0x001c0fc0, 0x00000008
	.section .rom.001c0fc8, "ax"
	.global Func_081c0fc8
	.type Func_081c0fc8, %function
	.thumb_func
Func_081c0fc8:
	.incbin "baserom.gba", 0x001c0fc8, 0x00000008
	.section .rom.001c0fd0, "ax"
	.global Func_081c0fd0
	.type Func_081c0fd0, %function
	.thumb_func
Func_081c0fd0:
	.incbin "baserom.gba", 0x001c0fd0, 0x0000000c
	.section .rom.001c1000, "ax"
	.global Func_081c1000
	.type Func_081c1000, %function
	.thumb_func
Func_081c1000:
	.incbin "baserom.gba", 0x001c1000, 0x00000014
	.section .rom.001c1014, "ax"
	.global Func_081c1014
	.type Func_081c1014, %function
	.thumb_func
Func_081c1014:
	.incbin "baserom.gba", 0x001c1014, 0x00000144
	.section .rom.001c117c, "ax"
	.global Func_081c117c
	.type Func_081c117c, %function
	.thumb_func
Func_081c117c:
	.incbin "baserom.gba", 0x001c117c, 0x0000000c
	.section .rom.001c1188, "ax"
	.global Func_081c1188
	.type Func_081c1188, %function
	.thumb_func
Func_081c1188:
	.incbin "baserom.gba", 0x001c1188, 0x00000024
	.section .rom.001c11ac, "ax"
	.global Func_081c11ac
	.type Func_081c11ac, %function
	.thumb_func
Func_081c11ac:
	.incbin "baserom.gba", 0x001c11ac, 0x00000020
	.section .rom.001c11cc, "ax"
	.global Func_081c11cc
	.type Func_081c11cc, %function
	.thumb_func
Func_081c11cc:
	.incbin "baserom.gba", 0x001c11cc, 0x000000b0
	.section .rom.001c16ca, "ax"
	.incbin "baserom.gba", 0x001c16ca, 0x00000052
	.section .rom.001c171c, "ax"
	.global Sound_LoadCommandTable
	.type Sound_LoadCommandTable, %function
	.thumb_func
Sound_LoadCommandTable:
	.incbin "baserom.gba", 0x001c171c, 0x000001c4
	.section .rom.001c1e48, "ax"
	.global Func_081c1e48
	.type Func_081c1e48, %function
	.thumb_func
Func_081c1e48:
	.incbin "baserom.gba", 0x001c1e48, 0x0000001c
	.section .rom.001c1e6e, "ax"
	.incbin "baserom.gba", 0x001c1e6e, 0x00000002
	.section .rom.001c2314, "ax"
	.global Func_081c2314
	.type Func_081c2314, %function
	.thumb_func
Func_081c2314:
	.incbin "baserom.gba", 0x001c2314, 0x00000014
	.global Func_081c2328
Func_081c2328:
	.global AudioCommand_InvokeSlot35
	.type AudioCommand_InvokeSlot35, %function
	.thumb_func
AudioCommand_InvokeSlot35:
	.incbin "baserom.gba", 0x001c2328, 0x00000014
	.section .rom.001c332c, "ax"
	.incbin "baserom.gba", 0x001c332c, 0x00000014
	.section .rom.001c3388, "ax"
	.incbin "baserom.gba", 0x001c3388, 0x00000028
	.section .rom.001c3404, "ax"
	.incbin "baserom.gba", 0x001c3404, 0x00000028
	.section .rom.001c342c, "ax"
	.global Audio_EmptyCallback
	.type Audio_EmptyCallback, %function
	.thumb_func
Audio_EmptyCallback:
	.incbin "baserom.gba", 0x001c342c, 0x000000a0
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x001c34cc, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x001c3580, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x001c35b0, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x001c35c8, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x001c364c, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x001c3664, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x001c36a0, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x001c36b0, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x001c36e4, 0x00000030
	.section .rom.001c43b0, "ax"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .rom.001c44d0, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .rom.002f9030, "ax"
	.incbin "baserom.gba", 0x002f9030, 0x00006fd0
	.section .rom.006322b2, "ax"
	.incbin "baserom.gba", 0x006322b2, 0x0004dd4e
	.section .rom.00682000, "ax"
	.global Resource_Data002
Resource_Data002:
	.incbin "baserom.gba", 0x00682000, 0x00000010
	.section .rom.006848d0, "ax"
	.global Resource_Data015
Resource_Data015:
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .rom.0068a0ca, "ax"
	.incbin "baserom.gba", 0x0068a0ca, 0x00000002
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a0cc, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x006927c4, 0x00005274
	.section .rom.006a45cd, "ax"
	.incbin "baserom.gba", 0x006a45cd, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a45d0, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a47d0, 0x00000884
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a5054, 0x000008fc
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a5950, 0x000008f8
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a6248, 0x00000758
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a69a0, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a71c8, 0x0000186c
	.section .rom.006a9fa2, "ax"
	.incbin "baserom.gba", 0x006a9fa2, 0x00000002
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006a9fa4, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b15c0, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006cd54c, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd800, 0x00000f0c
	.section .rom.006d10fa, "ax"
	.incbin "baserom.gba", 0x006d10fa, 0x00000002
	.section .rom.006d553e, "ax"
	.incbin "baserom.gba", 0x006d553e, 0x00000002
	.section .rom.006e5cae, "ax"
	.incbin "baserom.gba", 0x006e5cae, 0x00000002
	.section .rom.006e929e, "ax"
	.incbin "baserom.gba", 0x006e929e, 0x00000002
	.section .rom.006f4d42, "ax"
	.incbin "baserom.gba", 0x006f4d42, 0x00000002
	.section .rom.007053f2, "ax"
	.incbin "baserom.gba", 0x007053f2, 0x00000002
	.section .rom.0070ce92, "ax"
	.incbin "baserom.gba", 0x0070ce92, 0x00000002
	.section .rom.007106d2, "ax"
	.incbin "baserom.gba", 0x007106d2, 0x00000002
	.section .rom.00719b06, "ax"
	.incbin "baserom.gba", 0x00719b06, 0x00000002
	.section .rom.00720cb2, "ax"
	.incbin "baserom.gba", 0x00720cb2, 0x00000002
	.section .rom.00728b6e, "ax"
	.incbin "baserom.gba", 0x00728b6e, 0x00000002
	.section .rom.0072d5fe, "ax"
	.incbin "baserom.gba", 0x0072d5fe, 0x00000002
	.section .rom.007318b2, "ax"
	.incbin "baserom.gba", 0x007318b2, 0x00000002
	.section .rom.00735232, "ax"
	.incbin "baserom.gba", 0x00735232, 0x00000002
	.section .rom.00741a6e, "ax"
	.incbin "baserom.gba", 0x00741a6e, 0x00000002
	.section .rom.0074d1ae, "ax"
	.incbin "baserom.gba", 0x0074d1ae, 0x00000002
	.section .rom.0075057e, "ax"
	.incbin "baserom.gba", 0x0075057e, 0x00000002
	.section .rom.0076249a, "ax"
	.incbin "baserom.gba", 0x0076249a, 0x00000002
	.section .rom.00765f02, "ax"
	.incbin "baserom.gba", 0x00765f02, 0x00000002
	.section .rom.007698f2, "ax"
	.incbin "baserom.gba", 0x007698f2, 0x00000002
	.section .rom.00779e5a, "ax"
	.incbin "baserom.gba", 0x00779e5a, 0x00000002
	.section .rom.0077dd8a, "ax"
	.incbin "baserom.gba", 0x0077dd8a, 0x00000002
	.section .rom.007817ae, "ax"
	.incbin "baserom.gba", 0x007817ae, 0x00000002
	.section .rom.00789ace, "ax"
	.incbin "baserom.gba", 0x00789ace, 0x00000002
	.section .rom.00791c3a, "ax"
	.incbin "baserom.gba", 0x00791c3a, 0x00000002
	.section .rom.0079ef06, "ax"
	.incbin "baserom.gba", 0x0079ef06, 0x00000002
	.section .rom.007a2dc2, "ax"
	.incbin "baserom.gba", 0x007a2dc2, 0x00000002
	.section .rom.007a6bbe, "ax"
	.incbin "baserom.gba", 0x007a6bbe, 0x00000002
	.section .rom.007ab08e, "ax"
	.incbin "baserom.gba", 0x007ab08e, 0x00000002
	.section .rom.007b2a6a, "ax"
	.incbin "baserom.gba", 0x007b2a6a, 0x00000002
	.section .rom.007b7096, "ax"
	.incbin "baserom.gba", 0x007b7096, 0x00000002
	.section .rom.007bed12, "ax"
	.incbin "baserom.gba", 0x007bed12, 0x00000002
	.section .rom.007c290e, "ax"
	.incbin "baserom.gba", 0x007c290e, 0x00000002
	.section .rom.007c62be, "ax"
	.incbin "baserom.gba", 0x007c62be, 0x00000002
	.section .rom.007ca70e, "ax"
	.incbin "baserom.gba", 0x007ca70e, 0x00000002
	.section .rom.007ce306, "ax"
	.incbin "baserom.gba", 0x007ce306, 0x00000002
	.section .rom.007d9f66, "ax"
	.incbin "baserom.gba", 0x007d9f66, 0x00000002
	.section .rom.007ddbb6, "ax"
	.incbin "baserom.gba", 0x007ddbb6, 0x00000002
	.section .rom.007e6136, "ax"
	.incbin "baserom.gba", 0x007e6136, 0x00000002
	.section .rom.007f2786, "ax"
	.incbin "baserom.gba", 0x007f2786, 0x00000002
	.section .rom.007f923a, "ax"
	.incbin "baserom.gba", 0x007f923a, 0x00000002
	.section .rom.00802502, "ax"
	.incbin "baserom.gba", 0x00802502, 0x00000002
	.section .rom.00809f26, "ax"
	.incbin "baserom.gba", 0x00809f26, 0x00000002
	.section .rom.0081fba6, "ax"
	.incbin "baserom.gba", 0x0081fba6, 0x00000002
	.section .rom.00828fea, "ax"
	.incbin "baserom.gba", 0x00828fea, 0x00000002
	.section .rom.00832206, "ax"
	.incbin "baserom.gba", 0x00832206, 0x00000002
	.section .rom.0083fb2e, "ax"
	.incbin "baserom.gba", 0x0083fb2e, 0x00000002
	.section .rom.00845982, "ax"
	.incbin "baserom.gba", 0x00845982, 0x00000002
	.section .rom.0084b33a, "ax"
	.incbin "baserom.gba", 0x0084b33a, 0x00000002
	.section .rom.0084babd, "ax"
	.incbin "baserom.gba", 0x0084babd, 0x00000003
	.section .rom.0084d40f, "ax"
	.incbin "baserom.gba", 0x0084d40f, 0x00000001
	.section .rom.0085022d, "ax"
	.incbin "baserom.gba", 0x0085022d, 0x00000003
	.section .rom.00854142, "ax"
	.incbin "baserom.gba", 0x00854142, 0x00000002
	.section .rom.008573f8, "ax"
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x008573f8, 0x000009bc
	.section .rom.008583c7, "ax"
	.incbin "baserom.gba", 0x008583c7, 0x00000001
	.section .rom.008587e7, "ax"
	.incbin "baserom.gba", 0x008587e7, 0x00000001
	.section .rom.00858a66, "ax"
	.incbin "baserom.gba", 0x00858a66, 0x00000002
	.section .rom.00858d86, "ax"
	.incbin "baserom.gba", 0x00858d86, 0x00000002
	.section .rom.008591a9, "ax"
	.incbin "baserom.gba", 0x008591a9, 0x00000003
	.section .rom.0085954e, "ax"
	.incbin "baserom.gba", 0x0085954e, 0x00000002
	.section .rom.008598bd, "ax"
	.incbin "baserom.gba", 0x008598bd, 0x00000003
	.section .rom.0085a47b, "ax"
	.incbin "baserom.gba", 0x0085a47b, 0x00000001
	.section .rom.0085a587, "ax"
	.incbin "baserom.gba", 0x0085a587, 0x00000001
	.section .rom.0085a725, "ax"
	.incbin "baserom.gba", 0x0085a725, 0x00000003
	.section .rom.0085ab92, "ax"
	.incbin "baserom.gba", 0x0085ab92, 0x00000002
	.section .rom.0085b10d, "ax"
	.incbin "baserom.gba", 0x0085b10d, 0x00000003
	.section .rom.0085bc0b, "ax"
	.incbin "baserom.gba", 0x0085bc0b, 0x00000001
	.section .rom.0085be0d, "ax"
	.incbin "baserom.gba", 0x0085be0d, 0x00000003
	.section .rom.0085e4af, "ax"
	.incbin "baserom.gba", 0x0085e4af, 0x00000001
	.section .rom.0085ec2a, "ax"
	.incbin "baserom.gba", 0x0085ec2a, 0x00000002
	.section .rom.0086026f, "ax"
	.incbin "baserom.gba", 0x0086026f, 0x00000001
	.section .rom.00860799, "ax"
	.incbin "baserom.gba", 0x00860799, 0x00000003
	.section .rom.00860fa2, "ax"
	.incbin "baserom.gba", 0x00860fa2, 0x00000002
	.section .rom.0086151b, "ax"
	.incbin "baserom.gba", 0x0086151b, 0x00000001
	.section .rom.00862c5a, "ax"
	.incbin "baserom.gba", 0x00862c5a, 0x00000002
	.section .rom.00865b23, "ax"
	.incbin "baserom.gba", 0x00865b23, 0x00000001
	.section .rom.00865dd5, "ax"
	.incbin "baserom.gba", 0x00865dd5, 0x00000003
	.section .rom.008671b7, "ax"
	.incbin "baserom.gba", 0x008671b7, 0x00000001
	.section .rom.0086b9f3, "ax"
	.incbin "baserom.gba", 0x0086b9f3, 0x00000001
	.section .rom.0086f199, "ax"
	.incbin "baserom.gba", 0x0086f199, 0x00000003
	.section .rom.00870ebe, "ax"
	.incbin "baserom.gba", 0x00870ebe, 0x00000002
	.section .rom.00872dbd, "ax"
	.incbin "baserom.gba", 0x00872dbd, 0x00000003
	.section .rom.0087476b, "ax"
	.incbin "baserom.gba", 0x0087476b, 0x00000001
	.section .rom.00875ce3, "ax"
	.incbin "baserom.gba", 0x00875ce3, 0x00000001
	.section .rom.00879916, "ax"
	.incbin "baserom.gba", 0x00879916, 0x00000002
	.section .rom.0087a474, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a474, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087c0c4, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c504, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c750, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c8e8, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087d114, 0x00000e98
	.section .rom.0087e5e3, "ax"
	.incbin "baserom.gba", 0x0087e5e3, 0x00000001
	.section .rom.0088074d, "ax"
	.incbin "baserom.gba", 0x0088074d, 0x00000003
	.section .rom.00880858, "ax"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x00880858, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x00880aa4, 0x00000184
	.section .rom.008814e1, "ax"
	.incbin "baserom.gba", 0x008814e1, 0x00000003
	.section .rom.008830c5, "ax"
	.incbin "baserom.gba", 0x008830c5, 0x00000003
	.section .rom.00884c22, "ax"
	.incbin "baserom.gba", 0x00884c22, 0x00000002
	.section .rom.00885061, "ax"
	.incbin "baserom.gba", 0x00885061, 0x00000003
	.section .rom.00885476, "ax"
	.incbin "baserom.gba", 0x00885476, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00885478, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x008857b0, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088687c, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x00886b64, 0x00000154
	.section .rom.008891c3, "ax"
	.incbin "baserom.gba", 0x008891c3, 0x00000001
	.section .rom.00889b7f, "ax"
	.incbin "baserom.gba", 0x00889b7f, 0x00000001
	.section .rom.0088acc2, "ax"
	.incbin "baserom.gba", 0x0088acc2, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088acc4, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b940, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b96c, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b998, 0x000007c0
	.section .rom.0088d6d3, "ax"
	.incbin "baserom.gba", 0x0088d6d3, 0x00000001
	.section .rom.0088d9fd, "ax"
	.incbin "baserom.gba", 0x0088d9fd, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088da00, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088df28, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e458, 0x000005f0
	.section .rom.0088f1d1, "ax"
	.incbin "baserom.gba", 0x0088f1d1, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088f1d4, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f3d4, 0x00000420
	.section .rom.0088fe1d, "ax"
	.incbin "baserom.gba", 0x0088fe1d, 0x00000003
	.section .rom.00890227, "ax"
	.incbin "baserom.gba", 0x00890227, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x00890228, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x00890484, 0x0000057c
	.section .rom.00890ca9, "ax"
	.incbin "baserom.gba", 0x00890ca9, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890cac, 0x0000095c
	.section .rom.00891cc5, "ax"
	.incbin "baserom.gba", 0x00891cc5, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891cc8, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894778, 0x000011cc
	.section .rom.00895fa3, "ax"
	.incbin "baserom.gba", 0x00895fa3, 0x00000001
	.section .rom.00896fdd, "ax"
	.incbin "baserom.gba", 0x00896fdd, 0x00000003
	.section .rom.00897633, "ax"
	.incbin "baserom.gba", 0x00897633, 0x00000001
	.section .rom.00897cb1, "ax"
	.incbin "baserom.gba", 0x00897cb1, 0x00000003
	.section .rom.008982d1, "ax"
	.incbin "baserom.gba", 0x008982d1, 0x00000003
	.section .rom.0089969e, "ax"
	.incbin "baserom.gba", 0x0089969e, 0x00000002
	.section .rom.0089a6e2, "ax"
	.incbin "baserom.gba", 0x0089a6e2, 0x00000002
	.section .rom.0089b16b, "ax"
	.incbin "baserom.gba", 0x0089b16b, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089b16c, 0x000002cc
	.section .rom.0089c171, "ax"
	.incbin "baserom.gba", 0x0089c171, 0x00000003
	.section .rom.0089d3ad, "ax"
	.incbin "baserom.gba", 0x0089d3ad, 0x00000003
	.section .rom.0089dcf7, "ax"
	.incbin "baserom.gba", 0x0089dcf7, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089dcf8, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e354, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e880, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a0b3c, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a22d0, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a29b4, 0x00001f4c
	.section .rom.008a4e34, "ax"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4e34, 0x000011d8
	.section .rom.008a64fa, "ax"
	.incbin "baserom.gba", 0x008a64fa, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a64fc, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6b44, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a7768, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a7b2c, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7cf4, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a8240, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a858c, 0x0000076c
	.section .rom.008a933b, "ax"
	.incbin "baserom.gba", 0x008a933b, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a933c, 0x000000a8
	.section .rom.008a96fa, "ax"
	.incbin "baserom.gba", 0x008a96fa, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a96fc, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa0a4, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa39c, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aaecc, 0x00000100
	.section .rom.008abc3d, "ax"
	.incbin "baserom.gba", 0x008abc3d, 0x00000003
	.section .rom.008ac485, "ax"
	.incbin "baserom.gba", 0x008ac485, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac488, 0x00001200
	.section .rom.008ae547, "ax"
	.incbin "baserom.gba", 0x008ae547, 0x00000001
	.section .rom.008af023, "ax"
	.incbin "baserom.gba", 0x008af023, 0x00000001
	.section .rom.008af6e6, "ax"
	.incbin "baserom.gba", 0x008af6e6, 0x00000002
	.section .rom.008af9cd, "ax"
	.incbin "baserom.gba", 0x008af9cd, 0x00000003
	.section .rom.008b0d33, "ax"
	.incbin "baserom.gba", 0x008b0d33, 0x00000001
	.section .rom.008b111d, "ax"
	.incbin "baserom.gba", 0x008b111d, 0x00000003
	.section .rom.008b14ef, "ax"
	.incbin "baserom.gba", 0x008b14ef, 0x00000001
	.section .rom.008b1d8f, "ax"
	.incbin "baserom.gba", 0x008b1d8f, 0x00000001
	.section .rom.008b2232, "ax"
	.incbin "baserom.gba", 0x008b2232, 0x00000002
	.section .rom.008b33eb, "ax"
	.incbin "baserom.gba", 0x008b33eb, 0x00000001
	.section .rom.008b3844, "ax"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3844, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b48b0, 0x00000fc8
	.section .rom.008b5ad2, "ax"
	.incbin "baserom.gba", 0x008b5ad2, 0x00000002
	.section .rom.008b752d, "ax"
	.incbin "baserom.gba", 0x008b752d, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b7530, 0x000001b4
	.section .rom.008b7a79, "ax"
	.incbin "baserom.gba", 0x008b7a79, 0x00000003
	.section .rom.008b9802, "ax"
	.incbin "baserom.gba", 0x008b9802, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9804, 0x00000278
	.section .rom.008b9f42, "ax"
	.incbin "baserom.gba", 0x008b9f42, 0x00000002
	.section .rom.008bbb17, "ax"
	.incbin "baserom.gba", 0x008bbb17, 0x00000001
	.section .rom.008bd715, "ax"
	.incbin "baserom.gba", 0x008bd715, 0x00000003
	.section .rom.008bd936, "ax"
	.incbin "baserom.gba", 0x008bd936, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd938, 0x0000043c
	.section .rom.008bde85, "ax"
	.incbin "baserom.gba", 0x008bde85, 0x00000003
	.section .rom.008be467, "ax"
	.incbin "baserom.gba", 0x008be467, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be468, 0x000004a0
	.section .rom.008bf61a, "ax"
	.incbin "baserom.gba", 0x008bf61a, 0x00000002
	.section .rom.008bfb5c, "ax"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bfb5c, 0x00000840
	.section .rom.008c072b, "ax"
	.incbin "baserom.gba", 0x008c072b, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c072c, 0x00001258
	.section .rom.008c2472, "ax"
	.incbin "baserom.gba", 0x008c2472, 0x00000002
	.section .rom.008c3249, "ax"
	.incbin "baserom.gba", 0x008c3249, 0x00000003
	.section .rom.008c3a76, "ax"
	.incbin "baserom.gba", 0x008c3a76, 0x00000002
	.section .rom.008c3e68, "ax"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c3e68, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c4024, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c41e0, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4b20, 0x00000418
	.section .rom.008c58a9, "ax"
	.incbin "baserom.gba", 0x008c58a9, 0x00000003
	.section .rom.008c5c57, "ax"
	.incbin "baserom.gba", 0x008c5c57, 0x00000001
	.section .rom.008c7bfb, "ax"
	.incbin "baserom.gba", 0x008c7bfb, 0x00000001
	.section .rom.008c8997, "ax"
	.incbin "baserom.gba", 0x008c8997, 0x00000001
	.section .rom.008c8bb3, "ax"
	.incbin "baserom.gba", 0x008c8bb3, 0x00000001
	.section .rom.008c8eaf, "ax"
	.incbin "baserom.gba", 0x008c8eaf, 0x00000001
	.section .rom.008c9250, "ax"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c9250, 0x000016b0
	.section .rom.008cb05d, "ax"
	.incbin "baserom.gba", 0x008cb05d, 0x00000003
	.section .rom.008cbe2b, "ax"
	.incbin "baserom.gba", 0x008cbe2b, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbe2c, 0x00000c74
	.section .rom.008cd095, "ax"
	.incbin "baserom.gba", 0x008cd095, 0x00000003
	.section .rom.008cd5ef, "ax"
	.incbin "baserom.gba", 0x008cd5ef, 0x00000001
	.section .rom.008ceed9, "ax"
	.incbin "baserom.gba", 0x008ceed9, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008ceedc, 0x000008a0
	.section .rom.008cfb57, "ax"
	.incbin "baserom.gba", 0x008cfb57, 0x00000001
	.section .rom.008cfde1, "ax"
	.incbin "baserom.gba", 0x008cfde1, 0x00000003
	.section .rom.008d0187, "ax"
	.incbin "baserom.gba", 0x008d0187, 0x00000001
	.section .rom.008d03e2, "ax"
	.incbin "baserom.gba", 0x008d03e2, 0x00000002
	.section .rom.008d079a, "ax"
	.incbin "baserom.gba", 0x008d079a, 0x00000002
	.section .rom.008d1c83, "ax"
	.incbin "baserom.gba", 0x008d1c83, 0x00000001
	.section .rom.008d3246, "ax"
	.incbin "baserom.gba", 0x008d3246, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d3248, 0x00000a6c
	.section .rom.008d4a8a, "ax"
	.incbin "baserom.gba", 0x008d4a8a, 0x00000002
	.section .rom.008d4e0d, "ax"
	.incbin "baserom.gba", 0x008d4e0d, 0x00000003
	.section .rom.008d5abd, "ax"
	.incbin "baserom.gba", 0x008d5abd, 0x00000003
	.section .rom.008d6605, "ax"
	.incbin "baserom.gba", 0x008d6605, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6608, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d67a0, 0x0000088c
	.section .rom.008d7bfa, "ax"
	.incbin "baserom.gba", 0x008d7bfa, 0x00000002
	.section .rom.008d8112, "ax"
	.incbin "baserom.gba", 0x008d8112, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d8114, 0x00000624
	.section .rom.008d8b59, "ax"
	.incbin "baserom.gba", 0x008d8b59, 0x00000003
	.section .rom.008d8df3, "ax"
	.incbin "baserom.gba", 0x008d8df3, 0x00000001
	.section .rom.008da2f4, "ax"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da2f4, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da790, 0x0000198c
	.section .rom.008dc506, "ax"
	.incbin "baserom.gba", 0x008dc506, 0x00000002
	.section .rom.008de47b, "ax"
	.incbin "baserom.gba", 0x008de47b, 0x00000001
	.section .rom.008de94b, "ax"
	.incbin "baserom.gba", 0x008de94b, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de94c, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008defe0, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008dfa14, 0x00000bfc
	.section .rom.008e0de5, "ax"
	.incbin "baserom.gba", 0x008e0de5, 0x00000003
	.section .rom.008e18b6, "ax"
	.incbin "baserom.gba", 0x008e18b6, 0x00000002
	.section .rom.008e23f3, "ax"
	.incbin "baserom.gba", 0x008e23f3, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e23f4, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2a34, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3fbc, 0x00000064
	.section .rom.008e438b, "ax"
	.incbin "baserom.gba", 0x008e438b, 0x00000001
	.section .rom.008e4929, "ax"
	.incbin "baserom.gba", 0x008e4929, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e492c, 0x00000204
	.section .rom.008e4e69, "ax"
	.incbin "baserom.gba", 0x008e4e69, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4e6c, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5e84, 0x0000166c
	.section .rom.008e948e, "ax"
	.incbin "baserom.gba", 0x008e948e, 0x00000002
	.section .rom.008e9c31, "ax"
	.incbin "baserom.gba", 0x008e9c31, 0x00000003
	.section .rom.008eac48, "ax"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eac48, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eafc4, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb3f4, 0x000010cc
	.section .rom.008ec544, "ax"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec544, 0x000004e8
	.section .rom.008ecb34, "ax"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ecb34, 0x000006b8
	.section .rom.008edafe, "ax"
	.incbin "baserom.gba", 0x008edafe, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008edb00, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef634, 0x00001050
	.section .rom.008f1729, "ax"
	.incbin "baserom.gba", 0x008f1729, 0x00000003
	.section .rom.008f1928, "ax"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f1928, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00933190, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c554, 0x00000028
	.section .rom.0093c769, "ax"
	.incbin "baserom.gba", 0x0093c769, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c76c, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c8c0, 0x000004b8
	.section .rom.0093e4e2, "ax"
	.incbin "baserom.gba", 0x0093e4e2, 0x00000002
	.section .rom.0093fa19, "ax"
	.incbin "baserom.gba", 0x0093fa19, 0x00000003
	.section .rom.0094186f, "ax"
	.incbin "baserom.gba", 0x0094186f, 0x00000001
	.section .rom.00943983, "ax"
	.incbin "baserom.gba", 0x00943983, 0x00000001
	.section .rom.00943b5c, "ax"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x00943b5c, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943d40, 0x000003c0
	.section .rom.0094677b, "ax"
	.incbin "baserom.gba", 0x0094677b, 0x00000001
	.section .rom.00947183, "ax"
	.incbin "baserom.gba", 0x00947183, 0x00000001
	.section .rom.00949ea2, "ax"
	.incbin "baserom.gba", 0x00949ea2, 0x00000002
	.section .rom.0094a074, "ax"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x0094a074, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x0094a07c, 0x00000460
	.section .rom.0094bace, "ax"
	.incbin "baserom.gba", 0x0094bace, 0x00000002
	.section .rom.0094cf3e, "ax"
	.incbin "baserom.gba", 0x0094cf3e, 0x00000002
	.section .rom.0094d832, "ax"
	.incbin "baserom.gba", 0x0094d832, 0x00000002
	.section .rom.0094e159, "ax"
	.incbin "baserom.gba", 0x0094e159, 0x00000003
	.section .rom.0094f04a, "ax"
	.incbin "baserom.gba", 0x0094f04a, 0x00000002
	.section .rom.0094fc37, "ax"
	.incbin "baserom.gba", 0x0094fc37, 0x00000001
	.section .rom.0094fe12, "ax"
	.incbin "baserom.gba", 0x0094fe12, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fe14, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x00950004, 0x000002d8
	.section .rom.00950b72, "ax"
	.incbin "baserom.gba", 0x00950b72, 0x00000002
	.section .rom.00952fad, "ax"
	.incbin "baserom.gba", 0x00952fad, 0x00000003
	.section .rom.0095357f, "ax"
	.incbin "baserom.gba", 0x0095357f, 0x00000001
	.section .rom.00953a1f, "ax"
	.incbin "baserom.gba", 0x00953a1f, 0x00000001
	.section .rom.00953d72, "ax"
	.incbin "baserom.gba", 0x00953d72, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953d74, 0x000002f8
	.section .rom.00954145, "ax"
	.incbin "baserom.gba", 0x00954145, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00954148, 0x000002f0
	.section .rom.009544ca, "ax"
	.incbin "baserom.gba", 0x009544ca, 0x00000002
	.section .rom.0095507d, "ax"
	.incbin "baserom.gba", 0x0095507d, 0x00000003
	.section .rom.00955d2a, "ax"
	.incbin "baserom.gba", 0x00955d2a, 0x00000002
	.section .rom.0095682e, "ax"
	.incbin "baserom.gba", 0x0095682e, 0x00000002
	.section .rom.0095776d, "ax"
	.incbin "baserom.gba", 0x0095776d, 0x00000003
	.section .rom.00957fa9, "ax"
	.incbin "baserom.gba", 0x00957fa9, 0x00000003
	.section .rom.0095944e, "ax"
	.incbin "baserom.gba", 0x0095944e, 0x00000002
	.section .rom.00959f06, "ax"
	.incbin "baserom.gba", 0x00959f06, 0x00000002
	.section .rom.0095a231, "ax"
	.incbin "baserom.gba", 0x0095a231, 0x00000003
	.section .rom.0095b02e, "ax"
	.incbin "baserom.gba", 0x0095b02e, 0x00000002
	.section .rom.0095b830, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b830, 0x00000400
	.section .rom.00967fea, "ax"
	.incbin "baserom.gba", 0x00967fea, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967fec, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x009680ec, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009685b4, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x0096881c, 0x000001c8
	.section .rom.00968e4b, "ax"
	.incbin "baserom.gba", 0x00968e4b, 0x00000001
	.section .rom.00969057, "ax"
	.incbin "baserom.gba", 0x00969057, 0x00000001
	.section .rom.0096925b, "ax"
	.incbin "baserom.gba", 0x0096925b, 0x00000001
	.section .rom.009692ea, "ax"
	.incbin "baserom.gba", 0x009692ea, 0x00000002
	.section .rom.009697cc, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x009697cc, 0x00000070
	.section .rom.00969897, "ax"
	.incbin "baserom.gba", 0x00969897, 0x00000001
	.section .rom.009698ca, "ax"
	.incbin "baserom.gba", 0x009698ca, 0x00000002
	.section .rom.00969a96, "ax"
	.incbin "baserom.gba", 0x00969a96, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969a98, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969ce4, 0x000000e4
	.section .rom.00969e81, "ax"
	.incbin "baserom.gba", 0x00969e81, 0x00000003
	.section .rom.0096a056, "ax"
	.incbin "baserom.gba", 0x0096a056, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x0096a058, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a0f0, 0x00000024
	.section .rom.0096a266, "ax"
	.incbin "baserom.gba", 0x0096a266, 0x00000002
	.section .rom.0096a349, "ax"
	.incbin "baserom.gba", 0x0096a349, 0x00000003
	.section .rom.0096a419, "ax"
	.incbin "baserom.gba", 0x0096a419, 0x00000003
	.section .rom.0096a53f, "ax"
	.incbin "baserom.gba", 0x0096a53f, 0x00000001
	.section .rom.0096a56e, "ax"
	.incbin "baserom.gba", 0x0096a56e, 0x00000002
	.section .rom.0096a7d2, "ax"
	.incbin "baserom.gba", 0x0096a7d2, 0x00000002
	.section .rom.0096a86f, "ax"
	.incbin "baserom.gba", 0x0096a86f, 0x00000001
	.section .rom.0096a8d4, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a8d4, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096acd4, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096ad84, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b2e4, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b358, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b3a0, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b3f0, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b440, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b498, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b4f0, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b530, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b574, 0x00000054
	.section .rom.009707ff, "ax"
	.incbin "baserom.gba", 0x009707ff, 0x00000001
	.section .rom.0097374a, "ax"
	.incbin "baserom.gba", 0x0097374a, 0x00000002
	.section .rom.0097736d, "ax"
	.incbin "baserom.gba", 0x0097736d, 0x00000003
	.section .rom.009795b9, "ax"
	.incbin "baserom.gba", 0x009795b9, 0x00000003
	.section .rom.0097eb55, "ax"
	.incbin "baserom.gba", 0x0097eb55, 0x00000003
	.section .rom.009823cf, "ax"
	.incbin "baserom.gba", 0x009823cf, 0x00000001
	.section .rom.00988239, "ax"
	.incbin "baserom.gba", 0x00988239, 0x00000003
	.section .rom.0098930d, "ax"
	.incbin "baserom.gba", 0x0098930d, 0x00000003
	.section .rom.0098a379, "ax"
	.incbin "baserom.gba", 0x0098a379, 0x00000003
	.section .rom.0098f9d3, "ax"
	.incbin "baserom.gba", 0x0098f9d3, 0x00000001
	.section .rom.00992b15, "ax"
	.incbin "baserom.gba", 0x00992b15, 0x00000003
	.section .rom.00994bb5, "ax"
	.incbin "baserom.gba", 0x00994bb5, 0x00000003
	.section .rom.00996ed5, "ax"
	.incbin "baserom.gba", 0x00996ed5, 0x00000003
	.section .rom.00999b4e, "ax"
	.incbin "baserom.gba", 0x00999b4e, 0x00000002
	.section .rom.0099ba49, "ax"
	.incbin "baserom.gba", 0x0099ba49, 0x00000003
	.section .rom.009a3f8d, "ax"
	.incbin "baserom.gba", 0x009a3f8d, 0x00000003
	.section .rom.009acc0f, "ax"
	.incbin "baserom.gba", 0x009acc0f, 0x00000001
	.section .rom.009af789, "ax"
	.incbin "baserom.gba", 0x009af789, 0x00000003
	.section .rom.009b61c6, "ax"
	.incbin "baserom.gba", 0x009b61c6, 0x00000002
	.section .rom.009b8cba, "ax"
	.incbin "baserom.gba", 0x009b8cba, 0x00000002
	.section .rom.009ba7aa, "ax"
	.incbin "baserom.gba", 0x009ba7aa, 0x00000002
	.section .rom.009bd0c0, "ax"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bd0c0, 0x00003440
	.section .rom.009c4ea6, "ax"
	.incbin "baserom.gba", 0x009c4ea6, 0x00000002
	.section .rom.009c5e42, "ax"
	.incbin "baserom.gba", 0x009c5e42, 0x00000002
	.section .rom.009c856d, "ax"
	.incbin "baserom.gba", 0x009c856d, 0x00000003
	.section .rom.009c9c82, "ax"
	.incbin "baserom.gba", 0x009c9c82, 0x00000002
	.section .rom.009d249b, "ax"
	.incbin "baserom.gba", 0x009d249b, 0x00000001
	.section .rom.009d6763, "ax"
	.incbin "baserom.gba", 0x009d6763, 0x00000001
	.section .rom.009ddf9d, "ax"
	.incbin "baserom.gba", 0x009ddf9d, 0x00000003
	.section .rom.009df782, "ax"
	.incbin "baserom.gba", 0x009df782, 0x00000002
	.section .rom.009e2519, "ax"
	.incbin "baserom.gba", 0x009e2519, 0x00000003
	.section .rom.009e3563, "ax"
	.incbin "baserom.gba", 0x009e3563, 0x00000001
	.section .rom.009ebaf3, "ax"
	.incbin "baserom.gba", 0x009ebaf3, 0x00000001
	.section .rom.009ed8ed, "ax"
	.incbin "baserom.gba", 0x009ed8ed, 0x00000003
	.section .rom.009eecb3, "ax"
	.incbin "baserom.gba", 0x009eecb3, 0x00000001
	.section .rom.009f5bb5, "ax"
	.incbin "baserom.gba", 0x009f5bb5, 0x00000003
	.section .rom.009f7eb6, "ax"
	.incbin "baserom.gba", 0x009f7eb6, 0x00000002
	.section .rom.009fd103, "ax"
	.incbin "baserom.gba", 0x009fd103, 0x00000001
	.section .rom.00a02502, "ax"
	.incbin "baserom.gba", 0x00a02502, 0x00000002
	.section .rom.00a046b6, "ax"
	.incbin "baserom.gba", 0x00a046b6, 0x00000002
	.section .rom.00a0881d, "ax"
	.incbin "baserom.gba", 0x00a0881d, 0x00000003
	.section .rom.00a0ba42, "ax"
	.incbin "baserom.gba", 0x00a0ba42, 0x00000002
	.section .rom.00a0d146, "ax"
	.incbin "baserom.gba", 0x00a0d146, 0x00000002
	.section .rom.00a13d2e, "ax"
	.incbin "baserom.gba", 0x00a13d2e, 0x00000002
	.section .rom.00a2030e, "ax"
	.incbin "baserom.gba", 0x00a2030e, 0x00000002
	.section .rom.00a233a2, "ax"
	.incbin "baserom.gba", 0x00a233a2, 0x00000002
	.section .rom.00a25c0d, "ax"
	.incbin "baserom.gba", 0x00a25c0d, 0x00000003
	.section .rom.00a276fe, "ax"
	.incbin "baserom.gba", 0x00a276fe, 0x00000002
	.section .rom.00a28516, "ax"
	.incbin "baserom.gba", 0x00a28516, 0x00000002
	.section .rom.00a29201, "ax"
	.incbin "baserom.gba", 0x00a29201, 0x00000003
	.section .rom.00a326be, "ax"
	.incbin "baserom.gba", 0x00a326be, 0x00000002
	.section .rom.00a333c9, "ax"
	.incbin "baserom.gba", 0x00a333c9, 0x00000003
	.section .rom.00a3462d, "ax"
	.incbin "baserom.gba", 0x00a3462d, 0x00000003
	.section .rom.00a3555f, "ax"
	.incbin "baserom.gba", 0x00a3555f, 0x00000001
	.section .rom.00a361cf, "ax"
	.incbin "baserom.gba", 0x00a361cf, 0x00000001
	.section .rom.00a36de7, "ax"
	.incbin "baserom.gba", 0x00a36de7, 0x00000001
	.section .rom.00a3755b, "ax"
	.incbin "baserom.gba", 0x00a3755b, 0x00000001
	.section .rom.00a38deb, "ax"
	.incbin "baserom.gba", 0x00a38deb, 0x00000001
	.section .rom.00a397b9, "ax"
	.incbin "baserom.gba", 0x00a397b9, 0x00000003
	.section .rom.00a3a3d7, "ax"
	.incbin "baserom.gba", 0x00a3a3d7, 0x00000001
	.section .rom.00a3ca8b, "ax"
	.incbin "baserom.gba", 0x00a3ca8b, 0x00000001
	.section .rom.00a3f19e, "ax"
	.incbin "baserom.gba", 0x00a3f19e, 0x00000002
	.section .rom.00a440f3, "ax"
	.incbin "baserom.gba", 0x00a440f3, 0x00000001
	.section .rom.00a4841d, "ax"
	.incbin "baserom.gba", 0x00a4841d, 0x00000003
	.section .rom.00a4900a, "ax"
	.incbin "baserom.gba", 0x00a4900a, 0x00000002
	.section .rom.00a4dbbf, "ax"
	.incbin "baserom.gba", 0x00a4dbbf, 0x00000001
	.section .rom.00a4ff29, "ax"
	.incbin "baserom.gba", 0x00a4ff29, 0x00000003
	.section .rom.00a518cb, "ax"
	.incbin "baserom.gba", 0x00a518cb, 0x00000001
	.section .rom.00a5618d, "ax"
	.incbin "baserom.gba", 0x00a5618d, 0x00000003
	.section .rom.00a5959d, "ax"
	.incbin "baserom.gba", 0x00a5959d, 0x00000003
	.section .rom.00a5d665, "ax"
	.incbin "baserom.gba", 0x00a5d665, 0x00000003
	.section .rom.00a60d2e, "ax"
	.incbin "baserom.gba", 0x00a60d2e, 0x00000002
	.section .rom.00a68cff, "ax"
	.incbin "baserom.gba", 0x00a68cff, 0x00000001
	.section .rom.00a7000f, "ax"
	.incbin "baserom.gba", 0x00a7000f, 0x00000001
	.section .rom.00a74f8f, "ax"
	.incbin "baserom.gba", 0x00a74f8f, 0x00000001
	.section .rom.00a79a11, "ax"
	.incbin "baserom.gba", 0x00a79a11, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a79a14, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a79a20, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79b70, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79cb0, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a79df0, 0x00000140
	.section .rom.00a7b18d, "ax"
	.incbin "baserom.gba", 0x00a7b18d, 0x00000003
	.section .rom.00a7b35e, "ax"
	.incbin "baserom.gba", 0x00a7b35e, 0x00000002
	.section .rom.00a7d3ff, "ax"
	.incbin "baserom.gba", 0x00a7d3ff, 0x00000001
	.section .rom.00a7e38c, "ax"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e38c, 0x000022d8
	.section .rom.00a818b8, "ax"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a818b8, 0x0000461c
	.section .rom.00a85fda, "ax"
	.incbin "baserom.gba", 0x00a85fda, 0x00000002
	.section .rom.00a871c5, "ax"
	.incbin "baserom.gba", 0x00a871c5, 0x00000003
	.section .rom.00a88e4d, "ax"
	.incbin "baserom.gba", 0x00a88e4d, 0x00000003
	.section .rom.00a8935b, "ax"
	.incbin "baserom.gba", 0x00a8935b, 0x00000001
	.section .rom.00a8949b, "ax"
	.incbin "baserom.gba", 0x00a8949b, 0x00000001
	.section .rom.00a8b11e, "ax"
	.incbin "baserom.gba", 0x00a8b11e, 0x00000002
	.section .rom.00a8daf6, "ax"
	.incbin "baserom.gba", 0x00a8daf6, 0x00000002
	.section .rom.00a902bf, "ax"
	.incbin "baserom.gba", 0x00a902bf, 0x00000001
	.section .rom.00a917df, "ax"
	.incbin "baserom.gba", 0x00a917df, 0x00000001
	.section .rom.00a93123, "ax"
	.incbin "baserom.gba", 0x00a93123, 0x00000001
	.section .rom.00a94bd1, "ax"
	.incbin "baserom.gba", 0x00a94bd1, 0x00000003
	.section .rom.00a94d45, "ax"
	.incbin "baserom.gba", 0x00a94d45, 0x00000003
	.section .rom.00a97b29, "ax"
	.incbin "baserom.gba", 0x00a97b29, 0x00000003
	.section .rom.00a9a3d3, "ax"
	.incbin "baserom.gba", 0x00a9a3d3, 0x00000001
	.section .rom.00a9e86a, "ax"
	.incbin "baserom.gba", 0x00a9e86a, 0x00000002
	.section .rom.00aa1be2, "ax"
	.incbin "baserom.gba", 0x00aa1be2, 0x00000002
	.section .rom.00aa46df, "ax"
	.incbin "baserom.gba", 0x00aa46df, 0x00000001
	.section .rom.00aa67bd, "ax"
	.incbin "baserom.gba", 0x00aa67bd, 0x00000003
	.section .rom.00aa8a09, "ax"
	.incbin "baserom.gba", 0x00aa8a09, 0x00000003
	.section .rom.00aa8b4b, "ax"
	.incbin "baserom.gba", 0x00aa8b4b, 0x00000001
	.section .rom.00aaa21e, "ax"
	.incbin "baserom.gba", 0x00aaa21e, 0x00000002
	.section .rom.00aaa31e, "ax"
	.incbin "baserom.gba", 0x00aaa31e, 0x00000002
	.section .rom.00aac211, "ax"
	.incbin "baserom.gba", 0x00aac211, 0x00000003
	.section .rom.00aadbe9, "ax"
	.incbin "baserom.gba", 0x00aadbe9, 0x00000003
	.section .rom.00ab061a, "ax"
	.incbin "baserom.gba", 0x00ab061a, 0x00000002
	.section .rom.00ab586b, "ax"
	.incbin "baserom.gba", 0x00ab586b, 0x00000001
	.section .rom.00ab81af, "ax"
	.incbin "baserom.gba", 0x00ab81af, 0x00000001
	.section .rom.00ab9682, "ax"
	.incbin "baserom.gba", 0x00ab9682, 0x00000002
	.section .rom.00abe44d, "ax"
	.incbin "baserom.gba", 0x00abe44d, 0x00000003
	.section .rom.00ac10f7, "ax"
	.incbin "baserom.gba", 0x00ac10f7, 0x00000001
	.section .rom.00ac432f, "ax"
	.incbin "baserom.gba", 0x00ac432f, 0x00000001
	.section .rom.00ac44c7, "ax"
	.incbin "baserom.gba", 0x00ac44c7, 0x00000001
	.section .rom.00ac72c7, "ax"
	.incbin "baserom.gba", 0x00ac72c7, 0x00000001
	.section .rom.00ac8a7b, "ax"
	.incbin "baserom.gba", 0x00ac8a7b, 0x00000001
	.section .rom.00ac9a16, "ax"
	.incbin "baserom.gba", 0x00ac9a16, 0x00000002
	.section .rom.00acc077, "ax"
	.incbin "baserom.gba", 0x00acc077, 0x00000001
	.section .rom.00acc21e, "ax"
	.incbin "baserom.gba", 0x00acc21e, 0x00000002
	.section .rom.00acf1d7, "ax"
	.incbin "baserom.gba", 0x00acf1d7, 0x00000001
	.section .rom.00acfe7d, "ax"
	.incbin "baserom.gba", 0x00acfe7d, 0x00000003
	.section .rom.00ad144d, "ax"
	.incbin "baserom.gba", 0x00ad144d, 0x00000003
	.section .rom.00ad9419, "ax"
	.incbin "baserom.gba", 0x00ad9419, 0x00000003
	.section .rom.00adaeff, "ax"
	.incbin "baserom.gba", 0x00adaeff, 0x00000001
	.section .rom.00adc3e9, "ax"
	.incbin "baserom.gba", 0x00adc3e9, 0x00000003
	.section .rom.00ae2613, "ax"
	.incbin "baserom.gba", 0x00ae2613, 0x00000001
	.section .rom.00ae2ae5, "ax"
	.incbin "baserom.gba", 0x00ae2ae5, 0x00000003
	.section .rom.00ae3c2d, "ax"
	.incbin "baserom.gba", 0x00ae3c2d, 0x00000003
	.section .rom.00ae3dde, "ax"
	.incbin "baserom.gba", 0x00ae3dde, 0x00000002
	.section .rom.00aefacb, "ax"
	.incbin "baserom.gba", 0x00aefacb, 0x00000001
	.section .rom.00aefbeb, "ax"
	.incbin "baserom.gba", 0x00aefbeb, 0x00000001
	.section .rom.00af1f8b, "ax"
	.incbin "baserom.gba", 0x00af1f8b, 0x00000001
	.section .rom.00af31d7, "ax"
	.incbin "baserom.gba", 0x00af31d7, 0x00000001
	.section .rom.00af5567, "ax"
	.incbin "baserom.gba", 0x00af5567, 0x00000001
	.section .rom.00af6e65, "ax"
	.incbin "baserom.gba", 0x00af6e65, 0x00000003
	.section .rom.00af7e56, "ax"
	.incbin "baserom.gba", 0x00af7e56, 0x00000002
	.section .rom.00afa3f2, "ax"
	.incbin "baserom.gba", 0x00afa3f2, 0x00000002
	.section .rom.00afea1a, "ax"
	.incbin "baserom.gba", 0x00afea1a, 0x00000002
	.section .rom.00aff0df, "ax"
	.incbin "baserom.gba", 0x00aff0df, 0x00000001
	.section .rom.00b01b75, "ax"
	.incbin "baserom.gba", 0x00b01b75, 0x00000003
	.section .rom.00b01cc6, "ax"
	.incbin "baserom.gba", 0x00b01cc6, 0x00000002
	.section .rom.00b040d6, "ax"
	.incbin "baserom.gba", 0x00b040d6, 0x00000002
	.section .rom.00b06257, "ax"
	.incbin "baserom.gba", 0x00b06257, 0x00000001
	.section .rom.00b06397, "ax"
	.incbin "baserom.gba", 0x00b06397, 0x00000001
	.section .rom.00b08e36, "ax"
	.incbin "baserom.gba", 0x00b08e36, 0x00000002
	.section .rom.00b08f87, "ax"
	.incbin "baserom.gba", 0x00b08f87, 0x00000001
	.section .rom.00b0b396, "ax"
	.incbin "baserom.gba", 0x00b0b396, 0x00000002
	.section .rom.00b0d517, "ax"
	.incbin "baserom.gba", 0x00b0d517, 0x00000001
	.section .rom.00b0d657, "ax"
	.incbin "baserom.gba", 0x00b0d657, 0x00000001
	.section .rom.00b10c86, "ax"
	.incbin "baserom.gba", 0x00b10c86, 0x00000002
	.section .rom.00b131ea, "ax"
	.incbin "baserom.gba", 0x00b131ea, 0x00000002
	.section .rom.00b1536b, "ax"
	.incbin "baserom.gba", 0x00b1536b, 0x00000001
	.section .rom.00b154ab, "ax"
	.incbin "baserom.gba", 0x00b154ab, 0x00000001
	.section .rom.00b1695b, "ax"
	.incbin "baserom.gba", 0x00b1695b, 0x00000001
	.section .rom.00b16a66, "ax"
	.incbin "baserom.gba", 0x00b16a66, 0x00000002
	.section .rom.00b185be, "ax"
	.incbin "baserom.gba", 0x00b185be, 0x00000002
	.section .rom.00b19c61, "ax"
	.incbin "baserom.gba", 0x00b19c61, 0x00000003
	.section .rom.00b19e22, "ax"
	.incbin "baserom.gba", 0x00b19e22, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19e24, 0x00000d9c
	.section .rom.00b1c816, "ax"
	.incbin "baserom.gba", 0x00b1c816, 0x00000002
	.section .rom.00b1e06d, "ax"
	.incbin "baserom.gba", 0x00b1e06d, 0x00000003
	.section .rom.00b1e22e, "ax"
	.incbin "baserom.gba", 0x00b1e22e, 0x00000002
	.section .rom.00b224a9, "ax"
	.incbin "baserom.gba", 0x00b224a9, 0x00000003
	.section .rom.00b2266a, "ax"
	.incbin "baserom.gba", 0x00b2266a, 0x00000002
	.section .rom.00b230e1, "ax"
	.incbin "baserom.gba", 0x00b230e1, 0x00000003
	.section .rom.00b231e9, "ax"
	.incbin "baserom.gba", 0x00b231e9, 0x00000003
	.section .rom.00b24eaa, "ax"
	.incbin "baserom.gba", 0x00b24eaa, 0x00000002
	.section .rom.00b26637, "ax"
	.incbin "baserom.gba", 0x00b26637, 0x00000001
	.section .rom.00b268ed, "ax"
	.incbin "baserom.gba", 0x00b268ed, 0x00000003
	.section .rom.00b283fd, "ax"
	.incbin "baserom.gba", 0x00b283fd, 0x00000003
	.section .rom.00b2ac8a, "ax"
	.incbin "baserom.gba", 0x00b2ac8a, 0x00000002
	.section .rom.00b2d47a, "ax"
	.incbin "baserom.gba", 0x00b2d47a, 0x00000002
	.section .rom.00b2e41a, "ax"
	.incbin "baserom.gba", 0x00b2e41a, 0x00000002
	.section .rom.00b3056d, "ax"
	.incbin "baserom.gba", 0x00b3056d, 0x00000003
	.section .rom.00b33bfa, "ax"
	.incbin "baserom.gba", 0x00b33bfa, 0x00000002
	.section .rom.00b3598f, "ax"
	.incbin "baserom.gba", 0x00b3598f, 0x00000001
	.section .rom.00b377ad, "ax"
	.incbin "baserom.gba", 0x00b377ad, 0x00000003
	.section .rom.00b39289, "ax"
	.incbin "baserom.gba", 0x00b39289, 0x00000003
	.section .rom.00b3a872, "ax"
	.incbin "baserom.gba", 0x00b3a872, 0x00000002
	.section .rom.00b3ce5a, "ax"
	.incbin "baserom.gba", 0x00b3ce5a, 0x00000002
	.section .rom.00b3cfd3, "ax"
	.incbin "baserom.gba", 0x00b3cfd3, 0x00000001
	.section .rom.00b3e492, "ax"
	.incbin "baserom.gba", 0x00b3e492, 0x00000002
	.section .rom.00b3fc96, "ax"
	.incbin "baserom.gba", 0x00b3fc96, 0x00000002
	.section .rom.00b4160e, "ax"
	.incbin "baserom.gba", 0x00b4160e, 0x00000002
	.section .rom.00b4252e, "ax"
	.incbin "baserom.gba", 0x00b4252e, 0x00000002
	.section .rom.00b43fda, "ax"
	.incbin "baserom.gba", 0x00b43fda, 0x00000002
	.section .rom.00b440e7, "ax"
	.incbin "baserom.gba", 0x00b440e7, 0x00000001
	.section .rom.00b46217, "ax"
	.incbin "baserom.gba", 0x00b46217, 0x00000001
	.section .rom.00b47ded, "ax"
	.incbin "baserom.gba", 0x00b47ded, 0x00000003
	.section .rom.00b483ff, "ax"
	.incbin "baserom.gba", 0x00b483ff, 0x00000001
	.section .rom.00b4853f, "ax"
	.incbin "baserom.gba", 0x00b4853f, 0x00000001
	.section .rom.00b4ac3f, "ax"
	.incbin "baserom.gba", 0x00b4ac3f, 0x00000001
	.section .rom.00b4c49e, "ax"
	.incbin "baserom.gba", 0x00b4c49e, 0x00000002
	.section .rom.00b4caaf, "ax"
	.incbin "baserom.gba", 0x00b4caaf, 0x00000001
	.section .rom.00b4cbef, "ax"
	.incbin "baserom.gba", 0x00b4cbef, 0x00000001
	.section .rom.00b4da6a, "ax"
	.incbin "baserom.gba", 0x00b4da6a, 0x00000002
	.section .rom.00b4db81, "ax"
	.incbin "baserom.gba", 0x00b4db81, 0x00000003
	.section .rom.00b4fe57, "ax"
	.incbin "baserom.gba", 0x00b4fe57, 0x00000001
	.section .rom.00b51aaa, "ax"
	.incbin "baserom.gba", 0x00b51aaa, 0x00000002
	.section .rom.00b520ef, "ax"
	.incbin "baserom.gba", 0x00b520ef, 0x00000001
	.section .rom.00b5222f, "ax"
	.incbin "baserom.gba", 0x00b5222f, 0x00000001
	.section .rom.00b52e35, "ax"
	.incbin "baserom.gba", 0x00b52e35, 0x00000003
	.section .rom.00b553e7, "ax"
	.incbin "baserom.gba", 0x00b553e7, 0x00000001
	.section .rom.00b570a6, "ax"
	.incbin "baserom.gba", 0x00b570a6, 0x00000002
	.section .rom.00b58b72, "ax"
	.incbin "baserom.gba", 0x00b58b72, 0x00000002
	.section .rom.00b59c46, "ax"
	.incbin "baserom.gba", 0x00b59c46, 0x00000002
	.section .rom.00b59dad, "ax"
	.incbin "baserom.gba", 0x00b59dad, 0x00000003
	.section .rom.00b5afc6, "ax"
	.incbin "baserom.gba", 0x00b5afc6, 0x00000002
	.section .rom.00b5bba5, "ax"
	.incbin "baserom.gba", 0x00b5bba5, 0x00000003
	.section .rom.00b5bd12, "ax"
	.incbin "baserom.gba", 0x00b5bd12, 0x00000002
	.section .rom.00b5eb63, "ax"
	.incbin "baserom.gba", 0x00b5eb63, 0x00000001
	.section .rom.00b61322, "ax"
	.incbin "baserom.gba", 0x00b61322, 0x00000002
	.section .rom.00b673d3, "ax"
	.incbin "baserom.gba", 0x00b673d3, 0x00000001
	.section .rom.00b68f03, "ax"
	.incbin "baserom.gba", 0x00b68f03, 0x00000001
	.section .rom.00b6b23a, "ax"
	.incbin "baserom.gba", 0x00b6b23a, 0x00000002
	.section .rom.00b6c3eb, "ax"
	.incbin "baserom.gba", 0x00b6c3eb, 0x00000001
	.section .rom.00b6dd0d, "ax"
	.incbin "baserom.gba", 0x00b6dd0d, 0x00000003
	.section .rom.00b6fb99, "ax"
	.incbin "baserom.gba", 0x00b6fb99, 0x00000003
	.section .rom.00b71d7a, "ax"
	.incbin "baserom.gba", 0x00b71d7a, 0x00000002
	.section .rom.00b75741, "ax"
	.incbin "baserom.gba", 0x00b75741, 0x00000003
	.section .rom.00b7582d, "ax"
	.incbin "baserom.gba", 0x00b7582d, 0x00000003
	.section .rom.00b78a66, "ax"
	.incbin "baserom.gba", 0x00b78a66, 0x00000002
	.section .rom.00b790dd, "ax"
	.incbin "baserom.gba", 0x00b790dd, 0x00000003
	.section .rom.00b7ce9f, "ax"
	.incbin "baserom.gba", 0x00b7ce9f, 0x00000001
	.section .rom.00b7f131, "ax"
	.incbin "baserom.gba", 0x00b7f131, 0x00000003
	.section .rom.00b80b4e, "ax"
	.incbin "baserom.gba", 0x00b80b4e, 0x00000002
	.section .rom.00b81c5b, "ax"
	.incbin "baserom.gba", 0x00b81c5b, 0x00000001
	.section .rom.00b84dfa, "ax"
	.incbin "baserom.gba", 0x00b84dfa, 0x00000002
	.section .rom.00b85ff2, "ax"
	.incbin "baserom.gba", 0x00b85ff2, 0x00000002
	.section .rom.00b86eb5, "ax"
	.incbin "baserom.gba", 0x00b86eb5, 0x00000003
	.section .rom.00b88f56, "ax"
	.incbin "baserom.gba", 0x00b88f56, 0x00000002
	.section .rom.00b8a9ab, "ax"
	.incbin "baserom.gba", 0x00b8a9ab, 0x00000001
	.section .rom.00b8be76, "ax"
	.incbin "baserom.gba", 0x00b8be76, 0x00000002
	.section .rom.00b8e07a, "ax"
	.incbin "baserom.gba", 0x00b8e07a, 0x00000002
	.section .rom.00b8e16f, "ax"
	.incbin "baserom.gba", 0x00b8e16f, 0x00000001
	.section .rom.00b8f742, "ax"
	.incbin "baserom.gba", 0x00b8f742, 0x00000002
	.section .rom.00b8f961, "ax"
	.incbin "baserom.gba", 0x00b8f961, 0x00000003
	.section .rom.00b91a45, "ax"
	.incbin "baserom.gba", 0x00b91a45, 0x00000003
	.section .rom.00b91c0a, "ax"
	.incbin "baserom.gba", 0x00b91c0a, 0x00000002
	.section .rom.00b93ccd, "ax"
	.incbin "baserom.gba", 0x00b93ccd, 0x00000003
	.section .rom.00b93dfd, "ax"
	.incbin "baserom.gba", 0x00b93dfd, 0x00000003
	.section .rom.00b968b6, "ax"
	.incbin "baserom.gba", 0x00b968b6, 0x00000002
	.section .rom.00b9840e, "ax"
	.incbin "baserom.gba", 0x00b9840e, 0x00000002
	.section .rom.00b99355, "ax"
	.incbin "baserom.gba", 0x00b99355, 0x00000003
	.section .rom.00b9ad5e, "ax"
	.incbin "baserom.gba", 0x00b9ad5e, 0x00000002
	.section .rom.00baaab3, "ax"
	.incbin "baserom.gba", 0x00baaab3, 0x00000001
	.section .rom.00baac03, "ax"
	.incbin "baserom.gba", 0x00baac03, 0x00000001
	.section .rom.00bad70e, "ax"
	.incbin "baserom.gba", 0x00bad70e, 0x00000002
	.section .rom.00baf317, "ax"
	.incbin "baserom.gba", 0x00baf317, 0x00000001
	.section .rom.00bb0d1e, "ax"
	.incbin "baserom.gba", 0x00bb0d1e, 0x00000002
	.section .rom.00bb2a39, "ax"
	.incbin "baserom.gba", 0x00bb2a39, 0x00000003
	.section .rom.00bb2b8b, "ax"
	.incbin "baserom.gba", 0x00bb2b8b, 0x00000001
	.section .rom.00bb4592, "ax"
	.incbin "baserom.gba", 0x00bb4592, 0x00000002
	.section .rom.00bb831a, "ax"
	.incbin "baserom.gba", 0x00bb831a, 0x00000002
	.section .rom.00bb84ad, "ax"
	.incbin "baserom.gba", 0x00bb84ad, 0x00000003
	.section .rom.00bbd4b7, "ax"
	.incbin "baserom.gba", 0x00bbd4b7, 0x00000001
	.section .rom.00bbfb39, "ax"
	.incbin "baserom.gba", 0x00bbfb39, 0x00000003
	.section .rom.00bbfe6f, "ax"
	.incbin "baserom.gba", 0x00bbfe6f, 0x00000001
	.section .rom.00bc62c5, "ax"
	.incbin "baserom.gba", 0x00bc62c5, 0x00000003
	.section .rom.00bc6412, "ax"
	.incbin "baserom.gba", 0x00bc6412, 0x00000002
	.section .rom.00bc8297, "ax"
	.incbin "baserom.gba", 0x00bc8297, 0x00000001
	.section .rom.00bc983e, "ax"
	.incbin "baserom.gba", 0x00bc983e, 0x00000002
	.section .rom.00bcb7e9, "ax"
	.incbin "baserom.gba", 0x00bcb7e9, 0x00000003
	.section .rom.00bcc75a, "ax"
	.incbin "baserom.gba", 0x00bcc75a, 0x00000002
	.section .rom.00bd7d63, "ax"
	.incbin "baserom.gba", 0x00bd7d63, 0x00000001
	.section .rom.00bd7e1f, "ax"
	.incbin "baserom.gba", 0x00bd7e1f, 0x00000001
	.section .rom.00bd8b5e, "ax"
	.incbin "baserom.gba", 0x00bd8b5e, 0x00000002
	.section .rom.00bd953b, "ax"
	.incbin "baserom.gba", 0x00bd953b, 0x00000001
	.section .rom.00bda153, "ax"
	.incbin "baserom.gba", 0x00bda153, 0x00000001
	.section .rom.00bdc00e, "ax"
	.incbin "baserom.gba", 0x00bdc00e, 0x00000002
	.section .rom.00bdc12a, "ax"
	.incbin "baserom.gba", 0x00bdc12a, 0x00000002
	.section .rom.00bde9ab, "ax"
	.incbin "baserom.gba", 0x00bde9ab, 0x00000001
	.section .rom.00be0136, "ax"
	.incbin "baserom.gba", 0x00be0136, 0x00000002
	.section .rom.00be4066, "ax"
	.incbin "baserom.gba", 0x00be4066, 0x00000002
	.section .rom.00be41af, "ax"
	.incbin "baserom.gba", 0x00be41af, 0x00000001
	.section .rom.00be685f, "ax"
	.incbin "baserom.gba", 0x00be685f, 0x00000001
	.section .rom.00be9002, "ax"
	.incbin "baserom.gba", 0x00be9002, 0x00000002
	.section .rom.00be9d8d, "ax"
	.incbin "baserom.gba", 0x00be9d8d, 0x00000003
	.section .rom.00becbce, "ax"
	.incbin "baserom.gba", 0x00becbce, 0x00000002
	.section .rom.00bef1f5, "ax"
	.incbin "baserom.gba", 0x00bef1f5, 0x00000003
	.section .rom.00bf0ee1, "ax"
	.incbin "baserom.gba", 0x00bf0ee1, 0x00000003
	.section .rom.00bf25e1, "ax"
	.incbin "baserom.gba", 0x00bf25e1, 0x00000003
	.section .rom.00bf3b5d, "ax"
	.incbin "baserom.gba", 0x00bf3b5d, 0x00000003
	.section .rom.00bf52cf, "ax"
	.incbin "baserom.gba", 0x00bf52cf, 0x00000001
	.section .rom.00bf7f5a, "ax"
	.incbin "baserom.gba", 0x00bf7f5a, 0x00000002
	.section .rom.00bf8fa6, "ax"
	.incbin "baserom.gba", 0x00bf8fa6, 0x00000002
	.section .rom.00bfa0a3, "ax"
	.incbin "baserom.gba", 0x00bfa0a3, 0x00000001
	.section .rom.00bfb877, "ax"
	.incbin "baserom.gba", 0x00bfb877, 0x00000001
	.section .rom.00bfcd3b, "ax"
	.incbin "baserom.gba", 0x00bfcd3b, 0x00000001
	.section .rom.00bff26e, "ax"
	.incbin "baserom.gba", 0x00bff26e, 0x00000002
	.section .rom.00bff3a2, "ax"
	.incbin "baserom.gba", 0x00bff3a2, 0x00000002
	.section .rom.00c01b0e, "ax"
	.incbin "baserom.gba", 0x00c01b0e, 0x00000002
	.section .rom.00c038e1, "ax"
	.incbin "baserom.gba", 0x00c038e1, 0x00000003
	.section .rom.00c071c5, "ax"
	.incbin "baserom.gba", 0x00c071c5, 0x00000003
	.section .rom.00c0992b, "ax"
	.incbin "baserom.gba", 0x00c0992b, 0x00000001
	.section .rom.00c0b295, "ax"
	.incbin "baserom.gba", 0x00c0b295, 0x00000003
	.section .rom.00c0dabe, "ax"
	.incbin "baserom.gba", 0x00c0dabe, 0x00000002
	.section .rom.00c11e9d, "ax"
	.incbin "baserom.gba", 0x00c11e9d, 0x00000003
	.section .rom.00c15247, "ax"
	.incbin "baserom.gba", 0x00c15247, 0x00000001
	.section .rom.00c1603a, "ax"
	.incbin "baserom.gba", 0x00c1603a, 0x00000002
	.section .rom.00c16db2, "ax"
	.incbin "baserom.gba", 0x00c16db2, 0x00000002
	.section .rom.00c1eeda, "ax"
	.incbin "baserom.gba", 0x00c1eeda, 0x00000002
	.section .rom.00c25259, "ax"
	.incbin "baserom.gba", 0x00c25259, 0x00000003
	.section .rom.00c25c5f, "ax"
	.incbin "baserom.gba", 0x00c25c5f, 0x00000001
	.section .rom.00c2770d, "ax"
	.incbin "baserom.gba", 0x00c2770d, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27710, 0x00001490
	.section .rom.00c29007, "ax"
	.incbin "baserom.gba", 0x00c29007, 0x00000001
	.section .rom.00c291c6, "ax"
	.incbin "baserom.gba", 0x00c291c6, 0x00000002
	.section .rom.00c2b9ce, "ax"
	.incbin "baserom.gba", 0x00c2b9ce, 0x00000002
	.section .rom.00c2bb26, "ax"
	.incbin "baserom.gba", 0x00c2bb26, 0x00000002
	.section .rom.00c2e46d, "ax"
	.incbin "baserom.gba", 0x00c2e46d, 0x00000003
	.section .rom.00c30691, "ax"
	.incbin "baserom.gba", 0x00c30691, 0x00000003
	.section .rom.00c31b86, "ax"
	.incbin "baserom.gba", 0x00c31b86, 0x00000002
	.section .rom.00c33de9, "ax"
	.incbin "baserom.gba", 0x00c33de9, 0x00000003
	.section .rom.00c36697, "ax"
	.incbin "baserom.gba", 0x00c36697, 0x00000001
	.section .rom.00c3862a, "ax"
	.incbin "baserom.gba", 0x00c3862a, 0x00000002
	.section .rom.00c3964b, "ax"
	.incbin "baserom.gba", 0x00c3964b, 0x00000001
	.section .rom.00c3978b, "ax"
	.incbin "baserom.gba", 0x00c3978b, 0x00000001
	.section .rom.00c3c3aa, "ax"
	.incbin "baserom.gba", 0x00c3c3aa, 0x00000002
	.section .rom.00c3e82e, "ax"
	.incbin "baserom.gba", 0x00c3e82e, 0x00000002
	.section .rom.00c40915, "ax"
	.incbin "baserom.gba", 0x00c40915, 0x00000003
	.section .rom.00c42179, "ax"
	.incbin "baserom.gba", 0x00c42179, 0x00000003
	.section .rom.00c446df, "ax"
	.incbin "baserom.gba", 0x00c446df, 0x00000001
	.section .rom.00c44845, "ax"
	.incbin "baserom.gba", 0x00c44845, 0x00000003
	.section .rom.00c4703f, "ax"
	.incbin "baserom.gba", 0x00c4703f, 0x00000001
	.section .rom.00c49073, "ax"
	.incbin "baserom.gba", 0x00c49073, 0x00000001
	.section .rom.00c4ac9a, "ax"
	.incbin "baserom.gba", 0x00c4ac9a, 0x00000002
	.section .rom.00c4d6ba, "ax"
	.incbin "baserom.gba", 0x00c4d6ba, 0x00000002
	.section .rom.00c4e225, "ax"
	.incbin "baserom.gba", 0x00c4e225, 0x00000003
	.section .rom.00c4e30b, "ax"
	.incbin "baserom.gba", 0x00c4e30b, 0x00000001
	.section .rom.00c4fc55, "ax"
	.incbin "baserom.gba", 0x00c4fc55, 0x00000003
	.section .rom.00c515bb, "ax"
	.incbin "baserom.gba", 0x00c515bb, 0x00000001
	.section .rom.00c52989, "ax"
	.incbin "baserom.gba", 0x00c52989, 0x00000003
	.section .rom.00c52a6f, "ax"
	.incbin "baserom.gba", 0x00c52a6f, 0x00000001
	.section .rom.00c535dd, "ax"
	.incbin "baserom.gba", 0x00c535dd, 0x00000003
	.section .rom.00c536c3, "ax"
	.incbin "baserom.gba", 0x00c536c3, 0x00000001
	.section .rom.00c54427, "ax"
	.incbin "baserom.gba", 0x00c54427, 0x00000001
	.section .rom.00c5450b, "ax"
	.incbin "baserom.gba", 0x00c5450b, 0x00000001
	.section .rom.00c54d11, "ax"
	.incbin "baserom.gba", 0x00c54d11, 0x00000003
	.section .rom.00c54df7, "ax"
	.incbin "baserom.gba", 0x00c54df7, 0x00000001
	.section .rom.00c55957, "ax"
	.incbin "baserom.gba", 0x00c55957, 0x00000001
	.section .rom.00c561d5, "ax"
	.incbin "baserom.gba", 0x00c561d5, 0x00000003
	.section .rom.00c562d2, "ax"
	.incbin "baserom.gba", 0x00c562d2, 0x00000002
	.section .rom.00c5812b, "ax"
	.incbin "baserom.gba", 0x00c5812b, 0x00000001
	.section .rom.00c58f39, "ax"
	.incbin "baserom.gba", 0x00c58f39, 0x00000003
	.section .rom.00c5a1cd, "ax"
	.incbin "baserom.gba", 0x00c5a1cd, 0x00000003
	.section .rom.00c5b1a9, "ax"
	.incbin "baserom.gba", 0x00c5b1a9, 0x00000003
	.section .rom.00c5d5af, "ax"
	.incbin "baserom.gba", 0x00c5d5af, 0x00000001
	.section .rom.00c5d6ef, "ax"
	.incbin "baserom.gba", 0x00c5d6ef, 0x00000001
	.section .rom.00c5ddbd, "ax"
	.incbin "baserom.gba", 0x00c5ddbd, 0x00000003
	.section .rom.00c5de93, "ax"
	.incbin "baserom.gba", 0x00c5de93, 0x00000001
	.section .rom.00c5e709, "ax"
	.incbin "baserom.gba", 0x00c5e709, 0x00000003
	.section .rom.00c5ebba, "ax"
	.incbin "baserom.gba", 0x00c5ebba, 0x00000002
	.section .rom.00c60035, "ax"
	.incbin "baserom.gba", 0x00c60035, 0x00000003
	.section .rom.00c6084b, "ax"
	.incbin "baserom.gba", 0x00c6084b, 0x00000001
	.section .rom.00c614e1, "ax"
	.incbin "baserom.gba", 0x00c614e1, 0x00000003
	.section .rom.00c61686, "ax"
	.incbin "baserom.gba", 0x00c61686, 0x00000002
	.section .rom.00c6454a, "ax"
	.incbin "baserom.gba", 0x00c6454a, 0x00000002
	.section .rom.00c65ec9, "ax"
	.incbin "baserom.gba", 0x00c65ec9, 0x00000003
	.section .rom.00c66f16, "ax"
	.incbin "baserom.gba", 0x00c66f16, 0x00000002
	.section .rom.00c6e65b, "ax"
	.incbin "baserom.gba", 0x00c6e65b, 0x00000001
	.section .rom.00c75e4f, "ax"
	.incbin "baserom.gba", 0x00c75e4f, 0x00000001
	.section .rom.00c7c449, "ax"
	.incbin "baserom.gba", 0x00c7c449, 0x00000003
	.section .rom.00c7ee82, "ax"
	.incbin "baserom.gba", 0x00c7ee82, 0x00000002
	.section .rom.00c81753, "ax"
	.incbin "baserom.gba", 0x00c81753, 0x00000001
	.section .rom.00c85f9f, "ax"
	.incbin "baserom.gba", 0x00c85f9f, 0x00000001
	.section .rom.00c86f9d, "ax"
	.incbin "baserom.gba", 0x00c86f9d, 0x00000003
	.section .rom.00c870b6, "ax"
	.incbin "baserom.gba", 0x00c870b6, 0x00000002
	.section .rom.00c8865e, "ax"
	.incbin "baserom.gba", 0x00c8865e, 0x00000002
	.section .rom.00c8a0bb, "ax"
	.incbin "baserom.gba", 0x00c8a0bb, 0x00000001
	.section .rom.00c8a1f6, "ax"
	.incbin "baserom.gba", 0x00c8a1f6, 0x00000002
	.section .rom.00c8b6fd, "ax"
	.incbin "baserom.gba", 0x00c8b6fd, 0x00000003
	.section .rom.00c8e986, "ax"
	.incbin "baserom.gba", 0x00c8e986, 0x00000002
	.section .rom.00c8fdcd, "ax"
	.incbin "baserom.gba", 0x00c8fdcd, 0x00000003
	.section .rom.00c92ac5, "ax"
	.incbin "baserom.gba", 0x00c92ac5, 0x00000003
	.section .rom.00c94efb, "ax"
	.incbin "baserom.gba", 0x00c94efb, 0x00000001
	.section .rom.00c950d7, "ax"
	.incbin "baserom.gba", 0x00c950d7, 0x00000001
	.section .rom.00c981eb, "ax"
	.incbin "baserom.gba", 0x00c981eb, 0x00000001
	.section .rom.00c9966e, "ax"
	.incbin "baserom.gba", 0x00c9966e, 0x00000002
	.section .rom.00c9a6bd, "ax"
	.incbin "baserom.gba", 0x00c9a6bd, 0x00000003
	.section .rom.00c9c522, "ax"
	.incbin "baserom.gba", 0x00c9c522, 0x00000002
	.section .rom.00c9c6bd, "ax"
	.incbin "baserom.gba", 0x00c9c6bd, 0x00000003
	.section .rom.00c9c81b, "ax"
	.incbin "baserom.gba", 0x00c9c81b, 0x00000001
	.section .rom.00c9d79a, "ax"
	.incbin "baserom.gba", 0x00c9d79a, 0x00000002
	.section .rom.00c9d8b2, "ax"
	.incbin "baserom.gba", 0x00c9d8b2, 0x00000002
	.section .rom.00c9f945, "ax"
	.incbin "baserom.gba", 0x00c9f945, 0x00000003
	.section .rom.00ca17c7, "ax"
	.incbin "baserom.gba", 0x00ca17c7, 0x00000001
	.section .rom.00ca3ced, "ax"
	.incbin "baserom.gba", 0x00ca3ced, 0x00000003
	.section .rom.00ca3dfa, "ax"
	.incbin "baserom.gba", 0x00ca3dfa, 0x00000002
	.section .rom.00ca5e8d, "ax"
	.incbin "baserom.gba", 0x00ca5e8d, 0x00000003
	.section .rom.00ca9363, "ax"
	.incbin "baserom.gba", 0x00ca9363, 0x00000001
	.section .rom.00ca9e4d, "ax"
	.incbin "baserom.gba", 0x00ca9e4d, 0x00000003
	.section .rom.00ca9fbd, "ax"
	.incbin "baserom.gba", 0x00ca9fbd, 0x00000003
	.section .rom.00caced7, "ax"
	.incbin "baserom.gba", 0x00caced7, 0x00000001
	.section .rom.00caee62, "ax"
	.incbin "baserom.gba", 0x00caee62, 0x00000002
	.section .rom.00cb07cd, "ax"
	.incbin "baserom.gba", 0x00cb07cd, 0x00000003
	.section .rom.00cb62ba, "ax"
	.incbin "baserom.gba", 0x00cb62ba, 0x00000002
	.section .rom.00cb646a, "ax"
	.incbin "baserom.gba", 0x00cb646a, 0x00000002
	.section .rom.00cbade2, "ax"
	.incbin "baserom.gba", 0x00cbade2, 0x00000002
	.section .rom.00cbaf3a, "ax"
	.incbin "baserom.gba", 0x00cbaf3a, 0x00000002
	.section .rom.00cbe507, "ax"
	.incbin "baserom.gba", 0x00cbe507, 0x00000001
	.section .rom.00cbfd89, "ax"
	.incbin "baserom.gba", 0x00cbfd89, 0x00000003
	.section .rom.00cc1ab1, "ax"
	.incbin "baserom.gba", 0x00cc1ab1, 0x00000003
	.section .rom.00cc51d2, "ax"
	.incbin "baserom.gba", 0x00cc51d2, 0x00000002
	.section .rom.00cca35b, "ax"
	.incbin "baserom.gba", 0x00cca35b, 0x00000001
	.section .rom.00ccc929, "ax"
	.incbin "baserom.gba", 0x00ccc929, 0x00000003
	.section .rom.00cce2b5, "ax"
	.incbin "baserom.gba", 0x00cce2b5, 0x00000003
	.section .rom.00ccf3e2, "ax"
	.incbin "baserom.gba", 0x00ccf3e2, 0x00000002
	.section .rom.00ccff92, "ax"
	.incbin "baserom.gba", 0x00ccff92, 0x00000002
	.section .rom.00cd195a, "ax"
	.incbin "baserom.gba", 0x00cd195a, 0x00000002
	.section .rom.00cd1a3e, "ax"
	.incbin "baserom.gba", 0x00cd1a3e, 0x00000002
	.section .rom.00cd3e9b, "ax"
	.incbin "baserom.gba", 0x00cd3e9b, 0x00000001
	.section .rom.00cd3fdb, "ax"
	.incbin "baserom.gba", 0x00cd3fdb, 0x00000001
	.section .rom.00cd7651, "ax"
	.incbin "baserom.gba", 0x00cd7651, 0x00000003
	.section .rom.00cd77b5, "ax"
	.incbin "baserom.gba", 0x00cd77b5, 0x00000003
	.section .rom.00cd9e95, "ax"
	.incbin "baserom.gba", 0x00cd9e95, 0x00000003
	.section .rom.00cdcd45, "ax"
	.incbin "baserom.gba", 0x00cdcd45, 0x00000003
	.section .rom.00cde2ba, "ax"
	.incbin "baserom.gba", 0x00cde2ba, 0x00000002
	.section .rom.00ce23b6, "ax"
	.incbin "baserom.gba", 0x00ce23b6, 0x00000002
	.section .rom.00ce463b, "ax"
	.incbin "baserom.gba", 0x00ce463b, 0x00000001
	.section .rom.00ce621f, "ax"
	.incbin "baserom.gba", 0x00ce621f, 0x00000001
	.section .rom.00ce7f15, "ax"
	.incbin "baserom.gba", 0x00ce7f15, 0x00000003
	.section .rom.00cf5539, "ax"
	.incbin "baserom.gba", 0x00cf5539, 0x00000003
	.section .rom.00cf5661, "ax"
	.incbin "baserom.gba", 0x00cf5661, 0x00000003
	.section .rom.00cf7872, "ax"
	.incbin "baserom.gba", 0x00cf7872, 0x00000002
	.section .rom.00cf7a03, "ax"
	.incbin "baserom.gba", 0x00cf7a03, 0x00000001
	.section .rom.00cfeeb5, "ax"
	.incbin "baserom.gba", 0x00cfeeb5, 0x00000003
	.section .rom.00d0168b, "ax"
	.incbin "baserom.gba", 0x00d0168b, 0x00000001
	.section .rom.00d03de6, "ax"
	.incbin "baserom.gba", 0x00d03de6, 0x00000002
	.section .rom.00d03f99, "ax"
	.incbin "baserom.gba", 0x00d03f99, 0x00000003
	.section .rom.00d0569d, "ax"
	.incbin "baserom.gba", 0x00d0569d, 0x00000003
	.section .rom.00d076f7, "ax"
	.incbin "baserom.gba", 0x00d076f7, 0x00000001
	.section .rom.00d08a41, "ax"
	.incbin "baserom.gba", 0x00d08a41, 0x00000003
	.section .rom.00d0a48b, "ax"
	.incbin "baserom.gba", 0x00d0a48b, 0x00000001
	.section .rom.00d0a5af, "ax"
	.incbin "baserom.gba", 0x00d0a5af, 0x00000001
	.section .rom.00d0aa81, "ax"
	.incbin "baserom.gba", 0x00d0aa81, 0x00000003
	.section .rom.00d0cc65, "ax"
	.incbin "baserom.gba", 0x00d0cc65, 0x00000003
	.section .rom.00d0d139, "ax"
	.incbin "baserom.gba", 0x00d0d139, 0x00000003
	.section .rom.00d0f66f, "ax"
	.incbin "baserom.gba", 0x00d0f66f, 0x00000001
	.section .rom.00d0f7f7, "ax"
	.incbin "baserom.gba", 0x00d0f7f7, 0x00000001
	.section .rom.00d14099, "ax"
	.incbin "baserom.gba", 0x00d14099, 0x00000003
	.section .rom.00d141cf, "ax"
	.incbin "baserom.gba", 0x00d141cf, 0x00000001
	.section .rom.00d15e3b, "ax"
	.incbin "baserom.gba", 0x00d15e3b, 0x00000001
	.section .rom.00d16ff3, "ax"
	.incbin "baserom.gba", 0x00d16ff3, 0x00000001
	.section .rom.00d17d41, "ax"
	.incbin "baserom.gba", 0x00d17d41, 0x00000003
	.section .rom.00d18eca, "ax"
	.incbin "baserom.gba", 0x00d18eca, 0x00000002
	.section .rom.00d1a9c3, "ax"
	.incbin "baserom.gba", 0x00d1a9c3, 0x00000001
	.section .rom.00d1bdcf, "ax"
	.incbin "baserom.gba", 0x00d1bdcf, 0x00000001
	.section .rom.00d1c323, "ax"
	.incbin "baserom.gba", 0x00d1c323, 0x00000001
	.section .rom.00d1c95d, "ax"
	.incbin "baserom.gba", 0x00d1c95d, 0x00000003
	.section .rom.00d21cad, "ax"
	.incbin "baserom.gba", 0x00d21cad, 0x00000003
	.section .rom.00d2430e, "ax"
	.incbin "baserom.gba", 0x00d2430e, 0x00000002
	.section .rom.00d244a3, "ax"
	.incbin "baserom.gba", 0x00d244a3, 0x00000001
	.section .rom.00d26057, "ax"
	.incbin "baserom.gba", 0x00d26057, 0x00000001
	.section .rom.00d261b7, "ax"
	.incbin "baserom.gba", 0x00d261b7, 0x00000001
	.section .rom.00d28c5b, "ax"
	.incbin "baserom.gba", 0x00d28c5b, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28c5c, 0x000021b0
	.section .rom.00d2cc82, "ax"
	.incbin "baserom.gba", 0x00d2cc82, 0x00000002
	.section .rom.00d2cdc3, "ax"
	.incbin "baserom.gba", 0x00d2cdc3, 0x00000001
	.section .rom.00d33d3b, "ax"
	.incbin "baserom.gba", 0x00d33d3b, 0x00000001
	.section .rom.00d33e79, "ax"
	.incbin "baserom.gba", 0x00d33e79, 0x00000003
	.section .rom.00d35e4d, "ax"
	.incbin "baserom.gba", 0x00d35e4d, 0x00000003
	.section .rom.00d35f41, "ax"
	.incbin "baserom.gba", 0x00d35f41, 0x00000003
	.section .rom.00d37021, "ax"
	.incbin "baserom.gba", 0x00d37021, 0x00000003
	.section .rom.00d38efb, "ax"
	.incbin "baserom.gba", 0x00d38efb, 0x00000001
	.section .rom.00d39eb3, "ax"
	.incbin "baserom.gba", 0x00d39eb3, 0x00000001
	.section .rom.00d3bdfa, "ax"
	.incbin "baserom.gba", 0x00d3bdfa, 0x00000002
	.section .rom.00d3da05, "ax"
	.incbin "baserom.gba", 0x00d3da05, 0x00000003
	.section .rom.00d3db51, "ax"
	.incbin "baserom.gba", 0x00d3db51, 0x00000003
	.section .rom.00d3fc0f, "ax"
	.incbin "baserom.gba", 0x00d3fc0f, 0x00000001
	.section .rom.00d43cfe, "ax"
	.incbin "baserom.gba", 0x00d43cfe, 0x00000002
	.section .rom.00d47ab0, "ax"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d47ab0, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d499bc, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4bd20, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4dd4c, 0x0000180c
	.section .rom.00d50ca5, "ax"
	.incbin "baserom.gba", 0x00d50ca5, 0x00000003
	.section .rom.00d50dd2, "ax"
	.incbin "baserom.gba", 0x00d50dd2, 0x00000002
	.section .rom.00d57d45, "ax"
	.incbin "baserom.gba", 0x00d57d45, 0x00000003
	.section .rom.00d57e4a, "ax"
	.incbin "baserom.gba", 0x00d57e4a, 0x00000002
	.section .rom.00d57f8a, "ax"
	.incbin "baserom.gba", 0x00d57f8a, 0x00000002
	.section .rom.00d59956, "ax"
	.incbin "baserom.gba", 0x00d59956, 0x00000002
	.section .rom.00d59a4d, "ax"
	.incbin "baserom.gba", 0x00d59a4d, 0x00000003
	.section .rom.00d5c8a1, "ax"
	.incbin "baserom.gba", 0x00d5c8a1, 0x00000003
	.section .rom.00d5ca4a, "ax"
	.incbin "baserom.gba", 0x00d5ca4a, 0x00000002
	.section .rom.00d5f666, "ax"
	.incbin "baserom.gba", 0x00d5f666, 0x00000002
	.section .rom.00d62399, "ax"
	.incbin "baserom.gba", 0x00d62399, 0x00000003
	.section .rom.00d64083, "ax"
	.incbin "baserom.gba", 0x00d64083, 0x00000001
	.section .rom.00d678fd, "ax"
	.incbin "baserom.gba", 0x00d678fd, 0x00000003
	.section .rom.00d67a55, "ax"
	.incbin "baserom.gba", 0x00d67a55, 0x00000003
	.section .rom.00d69f99, "ax"
	.incbin "baserom.gba", 0x00d69f99, 0x00000003
	.section .rom.00d6de5e, "ax"
	.incbin "baserom.gba", 0x00d6de5e, 0x00000002
	.section .rom.00d6ff8e, "ax"
	.incbin "baserom.gba", 0x00d6ff8e, 0x00000002
	.section .rom.00d7419a, "ax"
	.incbin "baserom.gba", 0x00d7419a, 0x00000002
	.section .rom.00d7436e, "ax"
	.incbin "baserom.gba", 0x00d7436e, 0x00000002
	.section .rom.00d763ae, "ax"
	.incbin "baserom.gba", 0x00d763ae, 0x00000002
	.section .rom.00d777a5, "ax"
	.incbin "baserom.gba", 0x00d777a5, 0x00000003
	.section .rom.00d782e2, "ax"
	.incbin "baserom.gba", 0x00d782e2, 0x00000002
	.section .rom.00d7944d, "ax"
	.incbin "baserom.gba", 0x00d7944d, 0x00000003
	.section .rom.00d7a4f9, "ax"
	.incbin "baserom.gba", 0x00d7a4f9, 0x00000003
	.section .rom.00d7a6a5, "ax"
	.incbin "baserom.gba", 0x00d7a6a5, 0x00000003
	.section .rom.00d7c1b2, "ax"
	.incbin "baserom.gba", 0x00d7c1b2, 0x00000002
	.section .rom.00d7dbcf, "ax"
	.incbin "baserom.gba", 0x00d7dbcf, 0x00000001
	.section .rom.00d800a1, "ax"
	.incbin "baserom.gba", 0x00d800a1, 0x00000003
	.section .rom.00d81403, "ax"
	.incbin "baserom.gba", 0x00d81403, 0x00000001
	.section .rom.00d822b7, "ax"
	.incbin "baserom.gba", 0x00d822b7, 0x00000001
	.section .rom.00d838a5, "ax"
	.incbin "baserom.gba", 0x00d838a5, 0x00000003
	.section .rom.00d84deb, "ax"
	.incbin "baserom.gba", 0x00d84deb, 0x00000001
	.section .rom.00d85bb5, "ax"
	.incbin "baserom.gba", 0x00d85bb5, 0x00000003
	.section .rom.00d85d23, "ax"
	.incbin "baserom.gba", 0x00d85d23, 0x00000001
	.section .rom.00d86ce2, "ax"
	.incbin "baserom.gba", 0x00d86ce2, 0x00000002
	.section .rom.00d877ab, "ax"
	.incbin "baserom.gba", 0x00d877ab, 0x00000001
	.section .rom.00d88345, "ax"
	.incbin "baserom.gba", 0x00d88345, 0x00000003
	.section .rom.00d88495, "ax"
	.incbin "baserom.gba", 0x00d88495, 0x00000003
	.section .rom.00d8c743, "ax"
	.incbin "baserom.gba", 0x00d8c743, 0x00000001
	.section .rom.00d8e1dd, "ax"
	.incbin "baserom.gba", 0x00d8e1dd, 0x00000003
	.section .rom.00d8fbb3, "ax"
	.incbin "baserom.gba", 0x00d8fbb3, 0x00000001
	.section .rom.00d8fd32, "ax"
	.incbin "baserom.gba", 0x00d8fd32, 0x00000002
	.section .rom.00d93962, "ax"
	.incbin "baserom.gba", 0x00d93962, 0x00000002
	.section .rom.00d956bf, "ax"
	.incbin "baserom.gba", 0x00d956bf, 0x00000001
	.section .rom.00d98ce9, "ax"
	.incbin "baserom.gba", 0x00d98ce9, 0x00000003
	.section .rom.00d98e3a, "ax"
	.incbin "baserom.gba", 0x00d98e3a, 0x00000002
	.section .rom.00d9ea36, "ax"
	.incbin "baserom.gba", 0x00d9ea36, 0x00000002
	.section .rom.00da1373, "ax"
	.incbin "baserom.gba", 0x00da1373, 0x00000001
	.section .rom.00da2cb5, "ax"
	.incbin "baserom.gba", 0x00da2cb5, 0x00000003
	.section .rom.00da2e1e, "ax"
	.incbin "baserom.gba", 0x00da2e1e, 0x00000002
	.section .rom.00da9c1d, "ax"
	.incbin "baserom.gba", 0x00da9c1d, 0x00000003
	.section .rom.00daa1fb, "ax"
	.incbin "baserom.gba", 0x00daa1fb, 0x00000001
	.section .rom.00dab103, "ax"
	.incbin "baserom.gba", 0x00dab103, 0x00000001
	.section .rom.00dad94b, "ax"
	.incbin "baserom.gba", 0x00dad94b, 0x00000001
	.section .rom.00daef6a, "ax"
	.incbin "baserom.gba", 0x00daef6a, 0x00000002
	.section .rom.00db08cb, "ax"
	.incbin "baserom.gba", 0x00db08cb, 0x00000001
	.section .rom.00db36b3, "ax"
	.incbin "baserom.gba", 0x00db36b3, 0x00000001
	.section .rom.00db382e, "ax"
	.incbin "baserom.gba", 0x00db382e, 0x00000002
	.section .rom.00dc29f7, "ax"
	.incbin "baserom.gba", 0x00dc29f7, 0x00000001
	.section .rom.00dc2b5f, "ax"
	.incbin "baserom.gba", 0x00dc2b5f, 0x00000001
	.section .rom.00dc5182, "ax"
	.incbin "baserom.gba", 0x00dc5182, 0x00000002
	.section .rom.00dc52c5, "ax"
	.incbin "baserom.gba", 0x00dc52c5, 0x00000003
	.section .rom.00dc6b82, "ax"
	.incbin "baserom.gba", 0x00dc6b82, 0x00000002
	.section .rom.00dc9ae6, "ax"
	.incbin "baserom.gba", 0x00dc9ae6, 0x00000002
	.section .rom.00dcc47e, "ax"
	.incbin "baserom.gba", 0x00dcc47e, 0x00000002
	.section .rom.00dd0dd6, "ax"
	.incbin "baserom.gba", 0x00dd0dd6, 0x00000002
	.section .rom.00dd33ff, "ax"
	.incbin "baserom.gba", 0x00dd33ff, 0x00000001
	.section .rom.00dd35d3, "ax"
	.incbin "baserom.gba", 0x00dd35d3, 0x00000001
	.section .rom.00dd68f2, "ax"
	.incbin "baserom.gba", 0x00dd68f2, 0x00000002
	.section .rom.00dd6ac9, "ax"
	.incbin "baserom.gba", 0x00dd6ac9, 0x00000003
	.section .rom.00dd9e3d, "ax"
	.incbin "baserom.gba", 0x00dd9e3d, 0x00000003
	.section .rom.00ddb591, "ax"
	.incbin "baserom.gba", 0x00ddb591, 0x00000003
	.section .rom.00ddc51f, "ax"
	.incbin "baserom.gba", 0x00ddc51f, 0x00000001
	.section .rom.00ddde95, "ax"
	.incbin "baserom.gba", 0x00ddde95, 0x00000003
	.section .rom.00dddfcf, "ax"
	.incbin "baserom.gba", 0x00dddfcf, 0x00000001
	.section .rom.00ddfb11, "ax"
	.incbin "baserom.gba", 0x00ddfb11, 0x00000003
	.section .rom.00de30e9, "ax"
	.incbin "baserom.gba", 0x00de30e9, 0x00000003
	.section .rom.00de3d73, "ax"
	.incbin "baserom.gba", 0x00de3d73, 0x00000001
	.section .rom.00de3eb3, "ax"
	.incbin "baserom.gba", 0x00de3eb3, 0x00000001
	.section .rom.00de5c77, "ax"
	.incbin "baserom.gba", 0x00de5c77, 0x00000001
	.section .rom.00de5dea, "ax"
	.incbin "baserom.gba", 0x00de5dea, 0x00000002
	.section .rom.00de8252, "ax"
	.incbin "baserom.gba", 0x00de8252, 0x00000002
	.section .rom.00de9bee, "ax"
	.incbin "baserom.gba", 0x00de9bee, 0x00000002
	.section .rom.00deba23, "ax"
	.incbin "baserom.gba", 0x00deba23, 0x00000001
	.section .rom.00dec4da, "ax"
	.incbin "baserom.gba", 0x00dec4da, 0x00000002
	.section .rom.00dedef6, "ax"
	.incbin "baserom.gba", 0x00dedef6, 0x00000002
	.section .rom.00df0bc2, "ax"
	.incbin "baserom.gba", 0x00df0bc2, 0x00000002
	.section .rom.00df0d9d, "ax"
	.incbin "baserom.gba", 0x00df0d9d, 0x00000003
	.section .rom.00df28d3, "ax"
	.incbin "baserom.gba", 0x00df28d3, 0x00000001
	.section .rom.00df599d, "ax"
	.incbin "baserom.gba", 0x00df599d, 0x00000003
	.section .rom.00df6b59, "ax"
	.incbin "baserom.gba", 0x00df6b59, 0x00000003
	.section .rom.00df6d41, "ax"
	.incbin "baserom.gba", 0x00df6d41, 0x00000003
	.section .rom.00df6ed3, "ax"
	.incbin "baserom.gba", 0x00df6ed3, 0x00000001
	.section .rom.00df7c5a, "ax"
	.incbin "baserom.gba", 0x00df7c5a, 0x00000002
	.section .rom.00df7def, "ax"
	.incbin "baserom.gba", 0x00df7def, 0x00000001
	.section .rom.00df9ea3, "ax"
	.incbin "baserom.gba", 0x00df9ea3, 0x00000001
	.section .rom.00e01f26, "ax"
	.incbin "baserom.gba", 0x00e01f26, 0x00000002
	.section .rom.00e02067, "ax"
	.incbin "baserom.gba", 0x00e02067, 0x00000001
	.section .rom.00e063ef, "ax"
	.incbin "baserom.gba", 0x00e063ef, 0x00000001
	.section .rom.00e06b0e, "ax"
	.incbin "baserom.gba", 0x00e06b0e, 0x00000002
	.section .rom.00e07ecd, "ax"
	.incbin "baserom.gba", 0x00e07ecd, 0x00000003
	.section .rom.00e0bfe7, "ax"
	.incbin "baserom.gba", 0x00e0bfe7, 0x00000001
	.section .rom.00e0d067, "ax"
	.incbin "baserom.gba", 0x00e0d067, 0x00000001
	.section .rom.00e0e0e9, "ax"
	.incbin "baserom.gba", 0x00e0e0e9, 0x00000003
	.section .rom.00e0ec5a, "ax"
	.incbin "baserom.gba", 0x00e0ec5a, 0x00000002
	.section .rom.00e0fbab, "ax"
	.incbin "baserom.gba", 0x00e0fbab, 0x00000001
	.section .rom.00e0fd29, "ax"
	.incbin "baserom.gba", 0x00e0fd29, 0x00000003
	.section .rom.00e13283, "ax"
	.incbin "baserom.gba", 0x00e13283, 0x00000001
	.section .rom.00e148b6, "ax"
	.incbin "baserom.gba", 0x00e148b6, 0x00000002
	.section .rom.00e149f7, "ax"
	.incbin "baserom.gba", 0x00e149f7, 0x00000001
	.section .rom.00e17342, "ax"
	.incbin "baserom.gba", 0x00e17342, 0x00000002
	.section .rom.00e1fcea, "ax"
	.incbin "baserom.gba", 0x00e1fcea, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fcec, 0x00000fdc
	.section .rom.00e22895, "ax"
	.incbin "baserom.gba", 0x00e22895, 0x00000003
	.section .rom.00e247d5, "ax"
	.incbin "baserom.gba", 0x00e247d5, 0x00000003
	.section .rom.00e25c49, "ax"
	.incbin "baserom.gba", 0x00e25c49, 0x00000003
	.section .rom.00e27903, "ax"
	.incbin "baserom.gba", 0x00e27903, 0x00000001
	.section .rom.00e349b2, "ax"
	.incbin "baserom.gba", 0x00e349b2, 0x00000002
	.section .rom.00e34af1, "ax"
	.incbin "baserom.gba", 0x00e34af1, 0x00000003
	.section .rom.00e36a16, "ax"
	.incbin "baserom.gba", 0x00e36a16, 0x00000002
	.section .rom.00e36b3f, "ax"
	.incbin "baserom.gba", 0x00e36b3f, 0x00000001
	.section .rom.00e38d0f, "ax"
	.incbin "baserom.gba", 0x00e38d0f, 0x00000001
	.section .rom.00e3b2df, "ax"
	.incbin "baserom.gba", 0x00e3b2df, 0x00000001
	.section .rom.00e3d5f1, "ax"
	.incbin "baserom.gba", 0x00e3d5f1, 0x00000003
	.section .rom.00e3de65, "ax"
	.incbin "baserom.gba", 0x00e3de65, 0x00000003
	.section .rom.00e3dfa7, "ax"
	.incbin "baserom.gba", 0x00e3dfa7, 0x00000001
	.section .rom.00e4105d, "ax"
	.incbin "baserom.gba", 0x00e4105d, 0x00000003
	.section .rom.00e42743, "ax"
	.incbin "baserom.gba", 0x00e42743, 0x00000001
	.section .rom.00e4430b, "ax"
	.incbin "baserom.gba", 0x00e4430b, 0x00000001
	.section .rom.00e45e86, "ax"
	.incbin "baserom.gba", 0x00e45e86, 0x00000002
	.section .rom.00e462d1, "ax"
	.incbin "baserom.gba", 0x00e462d1, 0x00000003
	.section .rom.00e48432, "ax"
	.incbin "baserom.gba", 0x00e48432, 0x00000002
	.section .rom.00e4854b, "ax"
	.incbin "baserom.gba", 0x00e4854b, 0x00000001
	.section .rom.00e49375, "ax"
	.incbin "baserom.gba", 0x00e49375, 0x00000003
	.section .rom.00e494d7, "ax"
	.incbin "baserom.gba", 0x00e494d7, 0x00000001
	.section .rom.00e4dd3f, "ax"
	.incbin "baserom.gba", 0x00e4dd3f, 0x00000001
	.section .rom.00e503fd, "ax"
	.incbin "baserom.gba", 0x00e503fd, 0x00000003
	.section .rom.00e509db, "ax"
	.incbin "baserom.gba", 0x00e509db, 0x00000001
	.section .rom.00e5296e, "ax"
	.incbin "baserom.gba", 0x00e5296e, 0x00000002
	.section .rom.00e5414e, "ax"
	.incbin "baserom.gba", 0x00e5414e, 0x00000002
	.section .rom.00e542b2, "ax"
	.incbin "baserom.gba", 0x00e542b2, 0x00000002
	.section .rom.00e56aaa, "ax"
	.incbin "baserom.gba", 0x00e56aaa, 0x00000002
	.section .rom.00e56c63, "ax"
	.incbin "baserom.gba", 0x00e56c63, 0x00000001
	.section .rom.00e56deb, "ax"
	.incbin "baserom.gba", 0x00e56deb, 0x00000001
	.section .rom.00e5860d, "ax"
	.incbin "baserom.gba", 0x00e5860d, 0x00000003
	.section .rom.00e591f6, "ax"
	.incbin "baserom.gba", 0x00e591f6, 0x00000002
	.section .rom.00e5a72f, "ax"
	.incbin "baserom.gba", 0x00e5a72f, 0x00000001
	.section .rom.00e5a8ca, "ax"
	.incbin "baserom.gba", 0x00e5a8ca, 0x00000002
	.section .rom.00e5cb76, "ax"
	.incbin "baserom.gba", 0x00e5cb76, 0x00000002
	.section .rom.00e5ed3d, "ax"
	.incbin "baserom.gba", 0x00e5ed3d, 0x00000003
	.section .rom.00e6043b, "ax"
	.incbin "baserom.gba", 0x00e6043b, 0x00000001
	.section .rom.00e60f5f, "ax"
	.incbin "baserom.gba", 0x00e60f5f, 0x00000001
	.section .rom.00e65e02, "ax"
	.incbin "baserom.gba", 0x00e65e02, 0x00000002
	.section .rom.00e681a9, "ax"
	.incbin "baserom.gba", 0x00e681a9, 0x00000003
	.section .rom.00e68892, "ax"
	.incbin "baserom.gba", 0x00e68892, 0x00000002
	.section .rom.00e6947e, "ax"
	.incbin "baserom.gba", 0x00e6947e, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e69480, 0x00000ac4
	.section .rom.00e6b4f3, "ax"
	.incbin "baserom.gba", 0x00e6b4f3, 0x00000001
	.section .rom.00e6b6a6, "ax"
	.incbin "baserom.gba", 0x00e6b6a6, 0x00000002
	.section .rom.00e6d8e5, "ax"
	.incbin "baserom.gba", 0x00e6d8e5, 0x00000003
	.section .rom.00e6e40b, "ax"
	.incbin "baserom.gba", 0x00e6e40b, 0x00000001
	.section .rom.00e78e9c, "ax"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78e9c, 0x000007d0
	.section .rom.00e7c335, "ax"
	.incbin "baserom.gba", 0x00e7c335, 0x00000003
	.section .rom.00e7c4af, "ax"
	.incbin "baserom.gba", 0x00e7c4af, 0x00000001
	.section .rom.00e7c626, "ax"
	.incbin "baserom.gba", 0x00e7c626, 0x00000002
	.section .rom.00e7c7be, "ax"
	.incbin "baserom.gba", 0x00e7c7be, 0x00000002
	.section .rom.00e7c94f, "ax"
	.incbin "baserom.gba", 0x00e7c94f, 0x00000001
	.section .rom.00e7ebfb, "ax"
	.incbin "baserom.gba", 0x00e7ebfb, 0x00000001
	.section .rom.00e7ed13, "ax"
	.incbin "baserom.gba", 0x00e7ed13, 0x00000001
	.section .rom.00e80b15, "ax"
	.incbin "baserom.gba", 0x00e80b15, 0x00000003
	.section .rom.00e825da, "ax"
	.incbin "baserom.gba", 0x00e825da, 0x00000002
	.section .rom.00e82d02, "ax"
	.incbin "baserom.gba", 0x00e82d02, 0x00000002
	.section .rom.00e840fe, "ax"
	.incbin "baserom.gba", 0x00e840fe, 0x00000002
	.section .rom.00e84fd2, "ax"
	.incbin "baserom.gba", 0x00e84fd2, 0x00000002
	.section .rom.00e85116, "ax"
	.incbin "baserom.gba", 0x00e85116, 0x00000002
	.section .rom.00e874c7, "ax"
	.incbin "baserom.gba", 0x00e874c7, 0x00000001
	.section .rom.00e89cf5, "ax"
	.incbin "baserom.gba", 0x00e89cf5, 0x00000003
	.section .rom.00e8a0f2, "ax"
	.incbin "baserom.gba", 0x00e8a0f2, 0x00000002
	.section .rom.00e8b592, "ax"
	.incbin "baserom.gba", 0x00e8b592, 0x00000002
	.section .rom.00e8b6e6, "ax"
	.incbin "baserom.gba", 0x00e8b6e6, 0x00000002
	.section .rom.00e8f033, "ax"
	.incbin "baserom.gba", 0x00e8f033, 0x00000001
	.section .rom.00e9049e, "ax"
	.incbin "baserom.gba", 0x00e9049e, 0x00000002
	.section .rom.00e91977, "ax"
	.incbin "baserom.gba", 0x00e91977, 0x00000001
	.section .rom.00e91aca, "ax"
	.incbin "baserom.gba", 0x00e91aca, 0x00000002
	.section .rom.00e954d7, "ax"
	.incbin "baserom.gba", 0x00e954d7, 0x00000001
	.section .rom.00e966af, "ax"
	.incbin "baserom.gba", 0x00e966af, 0x00000001
	.section .rom.00e97eba, "ax"
	.incbin "baserom.gba", 0x00e97eba, 0x00000002
	.section .rom.00e9800a, "ax"
	.incbin "baserom.gba", 0x00e9800a, 0x00000002
	.section .rom.00e9a717, "ax"
	.incbin "baserom.gba", 0x00e9a717, 0x00000001
	.section .rom.00e9a87e, "ax"
	.incbin "baserom.gba", 0x00e9a87e, 0x00000002
	.section .rom.00e9d83f, "ax"
	.incbin "baserom.gba", 0x00e9d83f, 0x00000001
	.section .rom.00e9eaad, "ax"
	.incbin "baserom.gba", 0x00e9eaad, 0x00000003
	.section .rom.00ea163a, "ax"
	.incbin "baserom.gba", 0x00ea163a, 0x00000002
	.section .rom.00ea2021, "ax"
	.incbin "baserom.gba", 0x00ea2021, 0x00000003
	.section .rom.00ea29c2, "ax"
	.incbin "baserom.gba", 0x00ea29c2, 0x00000002
	.section .rom.00ea2b36, "ax"
	.incbin "baserom.gba", 0x00ea2b36, 0x00000002
	.section .rom.00ea4647, "ax"
	.incbin "baserom.gba", 0x00ea4647, 0x00000001
	.section .rom.00ea4769, "ax"
	.incbin "baserom.gba", 0x00ea4769, 0x00000003
	.section .rom.00ea7267, "ax"
	.incbin "baserom.gba", 0x00ea7267, 0x00000001
	.section .rom.00ea805e, "ax"
	.incbin "baserom.gba", 0x00ea805e, 0x00000002
	.section .rom.00ea9ffd, "ax"
	.incbin "baserom.gba", 0x00ea9ffd, 0x00000003
	.section .rom.00eab32d, "ax"
	.incbin "baserom.gba", 0x00eab32d, 0x00000003
	.section .rom.00eab481, "ax"
	.incbin "baserom.gba", 0x00eab481, 0x00000003
	.section .rom.00eac033, "ax"
	.incbin "baserom.gba", 0x00eac033, 0x00000001
	.section .rom.00ead80d, "ax"
	.incbin "baserom.gba", 0x00ead80d, 0x00000003
	.section .rom.00eae96d, "ax"
	.incbin "baserom.gba", 0x00eae96d, 0x00000003
	.section .rom.00eafa8b, "ax"
	.incbin "baserom.gba", 0x00eafa8b, 0x00000001
	.section .rom.00eafc92, "ax"
	.incbin "baserom.gba", 0x00eafc92, 0x00000002
	.section .rom.00eb0987, "ax"
	.incbin "baserom.gba", 0x00eb0987, 0x00000001
	.section .rom.00eb3129, "ax"
	.incbin "baserom.gba", 0x00eb3129, 0x00000003
	.section .rom.00eb489e, "ax"
	.incbin "baserom.gba", 0x00eb489e, 0x00000002
	.section .rom.00eb5b5b, "ax"
	.incbin "baserom.gba", 0x00eb5b5b, 0x00000001
	.section .rom.00eb639e, "ax"
	.incbin "baserom.gba", 0x00eb639e, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb63a0, 0x0000060c
	.section .rom.00eb6f23, "ax"
	.incbin "baserom.gba", 0x00eb6f23, 0x00000001
	.section .rom.00eb7063, "ax"
	.incbin "baserom.gba", 0x00eb7063, 0x00000001
	.section .rom.00eb71a3, "ax"
	.incbin "baserom.gba", 0x00eb71a3, 0x00000001
	.section .rom.00eb7d05, "ax"
	.incbin "baserom.gba", 0x00eb7d05, 0x00000003
	.section .rom.00eba4a9, "ax"
	.incbin "baserom.gba", 0x00eba4a9, 0x00000003
	.section .rom.00ebbc1e, "ax"
	.incbin "baserom.gba", 0x00ebbc1e, 0x00000002
	.section .rom.00ebcedb, "ax"
	.incbin "baserom.gba", 0x00ebcedb, 0x00000001
	.section .rom.00ebd71e, "ax"
	.incbin "baserom.gba", 0x00ebd71e, 0x00000002
	.section .rom.00ebdcbd, "ax"
	.incbin "baserom.gba", 0x00ebdcbd, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdcc0, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdccc, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebde1c, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebdf5c, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebe09c, 0x00000140
	.section .rom.00ebe8f1, "ax"
	.incbin "baserom.gba", 0x00ebe8f1, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe8f4, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe900, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebea50, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebeb90, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebecd0, 0x00000140
	.section .rom.00ebf9d1, "ax"
	.incbin "baserom.gba", 0x00ebf9d1, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf9d4, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf9e0, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebfb30, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebfc70, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebfdb0, 0x00000140
	.section .rom.00ec04bd, "ax"
	.incbin "baserom.gba", 0x00ec04bd, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec04c0, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec04cc, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec061c, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec075c, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec089c, 0x00000140
	.section .rom.00ec1359, "ax"
	.incbin "baserom.gba", 0x00ec1359, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec135c, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec1368, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec14b8, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec15f8, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1738, 0x00000140
	.section .rom.00ec1ca1, "ax"
	.incbin "baserom.gba", 0x00ec1ca1, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1ca4, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1cb0, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec1e00, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1f40, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec2080, 0x00000140
	.section .rom.00ec27e9, "ax"
	.incbin "baserom.gba", 0x00ec27e9, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec27ec, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec27f8, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2948, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec2a88, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2bc8, 0x00000140
	.section .rom.00ec3431, "ax"
	.incbin "baserom.gba", 0x00ec3431, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec3434, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec3440, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec3590, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec36d0, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec3810, 0x00000140
	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "baserom.gba", 0x00ec3950, 0x00000490
	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "baserom.gba", 0x00ec3de0, 0x00001d08
	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "baserom.gba", 0x00ec5ae8, 0x00003df0
	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "baserom.gba", 0x00ec98d8, 0x00002df0
	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "baserom.gba", 0x00ecc6c8, 0x000019ec
	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "baserom.gba", 0x00ece0b4, 0x000011b8
	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "baserom.gba", 0x00ecf26c, 0x000007c4
	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "baserom.gba", 0x00ecfa30, 0x00001860
	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "baserom.gba", 0x00ed1290, 0x000009c8
	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "baserom.gba", 0x00ed1c58, 0x00000b38
	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "baserom.gba", 0x00ed2790, 0x00003238
	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "baserom.gba", 0x00ed59c8, 0x000024bc
	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "baserom.gba", 0x00ed7e84, 0x00002da8
	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "baserom.gba", 0x00edac2c, 0x00001cfc
	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "baserom.gba", 0x00edc928, 0x0000114c
	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "baserom.gba", 0x00edda74, 0x00001608
	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "baserom.gba", 0x00edf07c, 0x000014b0
	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "baserom.gba", 0x00ee052c, 0x00000e48
	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "baserom.gba", 0x00ee1374, 0x00000274
	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "baserom.gba", 0x00ee15e8, 0x000001e4
	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "baserom.gba", 0x00ee17cc, 0x000006e0
	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "baserom.gba", 0x00ee1eac, 0x0000056c
	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "baserom.gba", 0x00ee2418, 0x000025a4
	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "baserom.gba", 0x00ee49bc, 0x00000720
	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "baserom.gba", 0x00ee50dc, 0x00003220
	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "baserom.gba", 0x00ee82fc, 0x000014bc
	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "baserom.gba", 0x00ee97b8, 0x00002950
	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "baserom.gba", 0x00eec108, 0x00004154
	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "baserom.gba", 0x00ef025c, 0x000014fc
	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "baserom.gba", 0x00ef1758, 0x00000a9c
	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "baserom.gba", 0x00ef21f4, 0x00000bf8
	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "baserom.gba", 0x00ef2dec, 0x00002984
	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "baserom.gba", 0x00ef5770, 0x00001458
	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "baserom.gba", 0x00ef6bc8, 0x00000da8
	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "baserom.gba", 0x00ef7970, 0x00001274
	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "baserom.gba", 0x00ef8be4, 0x000001d8
	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "baserom.gba", 0x00ef8dbc, 0x000005b0
	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "baserom.gba", 0x00ef936c, 0x00000aa4
	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "baserom.gba", 0x00ef9e10, 0x00000de0
	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "baserom.gba", 0x00efabf0, 0x0000375c
	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "baserom.gba", 0x00efe34c, 0x0000404c
	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "baserom.gba", 0x00f02398, 0x000012e8
	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "baserom.gba", 0x00f03680, 0x00000a68
	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "baserom.gba", 0x00f040e8, 0x00002b2c
	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "baserom.gba", 0x00f06c14, 0x00000d94
	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "baserom.gba", 0x00f079a8, 0x000015c8
	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "baserom.gba", 0x00f08f70, 0x00000720
	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "baserom.gba", 0x00f09690, 0x00001eec
	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "baserom.gba", 0x00f0b57c, 0x000018e4
	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "baserom.gba", 0x00f0ce60, 0x0000010c
	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "baserom.gba", 0x00f0cf6c, 0x00001fdc
	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "baserom.gba", 0x00f0ef48, 0x00003368
	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "baserom.gba", 0x00f122b0, 0x00001164
	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "baserom.gba", 0x00f13414, 0x00000a88
	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "baserom.gba", 0x00f13e9c, 0x00000fc0
	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "baserom.gba", 0x00f14e5c, 0x00000d0c
	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "baserom.gba", 0x00f15b68, 0x00003844
	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "baserom.gba", 0x00f193ac, 0x000014c8
	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "baserom.gba", 0x00f1a874, 0x0000035c
	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "baserom.gba", 0x00f1abd0, 0x000000d0
	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "baserom.gba", 0x00f1aca0, 0x00001ff8
	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "baserom.gba", 0x00f1cc98, 0x00002190
	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "baserom.gba", 0x00f1ee28, 0x000016e0
	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "baserom.gba", 0x00f20508, 0x000004a0
	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "baserom.gba", 0x00f209a8, 0x000003e8
	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "baserom.gba", 0x00f20d90, 0x00000c10
	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "baserom.gba", 0x00f219a0, 0x000012e0
	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "baserom.gba", 0x00f22c80, 0x00001228
	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "baserom.gba", 0x00f23ea8, 0x00001c28
	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "baserom.gba", 0x00f25ad0, 0x00000df4
	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "baserom.gba", 0x00f268c4, 0x0000246c
	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "baserom.gba", 0x00f28d30, 0x00001910
	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "baserom.gba", 0x00f2a640, 0x00002cc0
	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "baserom.gba", 0x00f2d300, 0x000039e8
	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "baserom.gba", 0x00f30ce8, 0x00000be4
	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "baserom.gba", 0x00f318cc, 0x00002e18
	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "baserom.gba", 0x00f346e4, 0x00000b1c
	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "baserom.gba", 0x00f35200, 0x00003314
	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "baserom.gba", 0x00f38514, 0x00004450
	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "baserom.gba", 0x00f3c964, 0x00001978
	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "baserom.gba", 0x00f3e2dc, 0x00001af8
	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "baserom.gba", 0x00f3fdd4, 0x000019ac
	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "baserom.gba", 0x00f41780, 0x00001efc
	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "baserom.gba", 0x00f4367c, 0x00002140
	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "baserom.gba", 0x00f457bc, 0x00001cb0
	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "baserom.gba", 0x00f4746c, 0x00002468
	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "baserom.gba", 0x00f498d4, 0x00001a84
	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "baserom.gba", 0x00f4b358, 0x000023b0
	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "baserom.gba", 0x00f4d708, 0x0000205c
	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "baserom.gba", 0x00f4f764, 0x00001b4c
	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "baserom.gba", 0x00f512b0, 0x000032b8
	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "baserom.gba", 0x00f54568, 0x0000245c
	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "baserom.gba", 0x00f569c4, 0x000037a8
	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "baserom.gba", 0x00f5a16c, 0x00000fc4
	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "baserom.gba", 0x00f5b130, 0x00000810
	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "baserom.gba", 0x00f5b940, 0x00002d6c
	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "baserom.gba", 0x00f5e6ac, 0x00000d70
	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "baserom.gba", 0x00f5f41c, 0x00003c08
	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "baserom.gba", 0x00f63024, 0x00002f08
	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "baserom.gba", 0x00f65f2c, 0x00002274
	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "baserom.gba", 0x00f681a0, 0x000045b0
	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "baserom.gba", 0x00f6c750, 0x00003344
	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "baserom.gba", 0x00f6fa94, 0x00000d80
	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "baserom.gba", 0x00f70814, 0x000028b4
	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "baserom.gba", 0x00f730c8, 0x000015ac
	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "baserom.gba", 0x00f74674, 0x00000430
	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "baserom.gba", 0x00f74aa4, 0x00000564
	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "baserom.gba", 0x00f75008, 0x00000e50
	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "baserom.gba", 0x00f75e58, 0x000013f8
	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "baserom.gba", 0x00f77250, 0x000000b0
	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "baserom.gba", 0x00f77300, 0x00000774
	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "baserom.gba", 0x00f77a74, 0x00000cd4
	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "baserom.gba", 0x00f78748, 0x00000518
	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "baserom.gba", 0x00f78c60, 0x000873a0
