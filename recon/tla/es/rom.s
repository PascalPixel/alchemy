@ tla-es's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.section .rom.00024ec6, "ax"
	.incbin "baserom.gba", 0x00024ec6, 0x0000000e
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
	.section .rom.00025c8a, "ax"
	.incbin "baserom.gba", 0x00025c8a, 0x00000002
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
	.incbin "baserom.gba", 0x000396dc, 0x0000098c
	.section .rom.0003a068, "ax"
	.global Func_0803a084
	.type Func_0803a084, %function
	.thumb_func
Func_0803a084:
	.incbin "baserom.gba", 0x0003a068, 0x00000334
	.section .rom.0003a39c, "ax"
	.global UiWork_IsComplete
	.type UiWork_IsComplete, %function
	.thumb_func
UiWork_IsComplete:
	.incbin "baserom.gba", 0x0003a39c, 0x0000002c
	.section .rom.0003a3c8, "ax"
	.global UiWork_IsIdle
	.type UiWork_IsIdle, %function
	.thumb_func
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a3c8, 0x0000014c
	.section .rom.0003a514, "ax"
	.global Func_0803a530
	.type Func_0803a530, %function
	.thumb_func
Func_0803a530:
	.incbin "baserom.gba", 0x0003a514, 0x0000001c
	.section .rom.0003a530, "ax"
	.global Func_0803a54c
	.type Func_0803a54c, %function
	.thumb_func
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a530, 0x00000094
	.section .rom.0003a5c4, "ax"
	.global Func_0803a5e0
	.type Func_0803a5e0, %function
	.thumb_func
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a5c4, 0x0000002c
	.section .rom.0003a5f0, "ax"
	.global Func_0803a60c
	.type Func_0803a60c, %function
	.thumb_func
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5f0, 0x0000005c
	.section .rom.0003a680, "ax"
	.global UiText_OpenMessageWindow
	.type UiText_OpenMessageWindow, %function
	.thumb_func
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a680, 0x00000110
	.section .rom.0003a790, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.type UiText_ShowPositionedMessageAndWait, %function
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a790, 0x00000320
	.section .rom.0003aab0, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aab0, 0x00000594
	.section .rom.0003b044, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003b044, 0x0000083c
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
	.incbin "baserom.gba", 0x0003b930, 0x00000740
	.section .rom.0003c070, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c070, 0x00000108
	.section .rom.0003c178, "ax"
	.global Func_0803c170
	.type Func_0803c170, %function
	.thumb_func
Func_0803c170:
	.incbin "baserom.gba", 0x0003c178, 0x00000028
	.section .rom.0003c1a0, "ax"
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c1a0, 0x000001e0
	.section .rom.0003c380, "ax"
	.global UiWindow_SetTilemapEntry
	.type UiWindow_SetTilemapEntry, %function
	.thumb_func
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c380, 0x00000634
	.section .rom.0003c9c4, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9c4, 0x00000064
	.section .rom.0003ca28, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca28, 0x00000144
	.section .rom.0003cb7a, "ax"
	.incbin "baserom.gba", 0x0003cb7a, 0x00000002
	.section .rom.0003cb7c, "ax"
	.global Func_0803cb1c
	.type Func_0803cb1c, %function
	.thumb_func
Func_0803cb1c:
	.incbin "baserom.gba", 0x0003cb7c, 0x0000018c
	.section .rom.0003cd08, "ax"
	.global Func_0803cca8
	.type Func_0803cca8, %function
	.thumb_func
Func_0803cca8:
	.incbin "baserom.gba", 0x0003cd08, 0x00000028
	.section .rom.0003cd30, "ax"
	.global Func_0803ccd0
	.type Func_0803ccd0, %function
	.thumb_func
Func_0803ccd0:
	.incbin "baserom.gba", 0x0003cd30, 0x0000014c
	.section .rom.0003ce7c, "ax"
	.global Func_0803ce1c
	.type Func_0803ce1c, %function
	.thumb_func
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce7c, 0x00000048
	.section .rom.0003cec4, "ax"
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cec4, 0x000001bc
	.section .rom.0003d080, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d080, 0x000002d0
	.section .rom.0003d350, "ax"
	.global Localization_LookupEntryId
	.type Localization_LookupEntryId, %function
	.thumb_func
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d350, 0x000000d0
	.section .rom.0003d420, "ax"
	.global Func_0803d3c0
	.type Func_0803d3c0, %function
	.thumb_func
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d420, 0x00000090
	.section .rom.0003d4b0, "ax"
	.global Func_0803d450
	.type Func_0803d450, %function
	.thumb_func
Func_0803d450:
	.incbin "baserom.gba", 0x0003d4b0, 0x0000006c
	.section .rom.0003d544, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d544, 0x000000e0
	.section .rom.0003d624, "ax"
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d624, 0x000000bc
	.section .rom.0003d6e0, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6e0, 0x0000022c
	.section .rom.0003d98a, "ax"
	.incbin "baserom.gba", 0x0003d98a, 0x00000002
	.section .rom.0003d98c, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d98c, 0x00000060
	.section .rom.0003d9ec, "ax"
	.global Ability_LoadGlyph
	.type Ability_LoadGlyph, %function
	.thumb_func
Ability_LoadGlyph:
	.incbin "baserom.gba", 0x0003d9ec, 0x00000030
	.section .rom.0003da1c, "ax"
	.global Func_0803d9bc
	.type Func_0803d9bc, %function
	.thumb_func
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da1c, 0x000000bc
	.section .rom.0003dad8, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.type Ui_PrepareTransferFromTableEntry, %function
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dad8, 0x000001a4
	.section .rom.0003dc7c, "ax"
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc7c, 0x00000108
	.section .rom.0003dd84, "ax"
	.global Func_0803dd24
	.type Func_0803dd24, %function
	.thumb_func
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd84, 0x00000044
	.section .rom.0003ddc8, "ax"
	.global Func_0803dd68
	.type Func_0803dd68, %function
	.thumb_func
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddc8, 0x00000030
	.section .rom.0003ddf8, "ax"
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddf8, 0x00000110
	.section .rom.0003df08, "ax"
	.global Func_0803dea8
	.type Func_0803dea8, %function
	.thumb_func
Func_0803dea8:
	.incbin "baserom.gba", 0x0003df08, 0x00000058
	.section .rom.0003df60, "ax"
	.global Func_0803df00
	.type Func_0803df00, %function
	.thumb_func
Func_0803df00:
	.incbin "baserom.gba", 0x0003df60, 0x00000014
	.section .rom.0003df74, "ax"
	.global Resource_ScheduleOwnerReset
	.type Resource_ScheduleOwnerReset, %function
	.thumb_func
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df74, 0x00000694
	.section .rom.0003e7d4, "ax"
	.global Func_0803e774
	.type Func_0803e774, %function
	.thumb_func
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7d4, 0x00000038
	.section .rom.0003e80c, "ax"
	.global Func_0803e7ac
	.type Func_0803e7ac, %function
	.thumb_func
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e80c, 0x00000140
	.section .rom.0003e94c, "ax"
	.global NodeChain_GetNodeAtCount
	.type NodeChain_GetNodeAtCount, %function
	.thumb_func
NodeChain_GetNodeAtCount:
	.incbin "baserom.gba", 0x0003e94c, 0x0000002c
	.section .rom.0003e978, "ax"
	.global Func_0803e918
	.type Func_0803e918, %function
	.thumb_func
Func_0803e918:
	.incbin "baserom.gba", 0x0003e978, 0x00000080
	.section .rom.0003e9f8, "ax"
	.global Func_0803e998
	.type Func_0803e998, %function
	.thumb_func
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9f8, 0x0000063c
	.section .rom.0003f064, "ax"
	.incbin "baserom.gba", 0x0003f064, 0x000001d0
	.section .rom.0003f234, "ax"
	.global Resource_LoadByMode
	.type Resource_LoadByMode, %function
	.thumb_func
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f234, 0x00000060
	.section .rom.0003f294, "ax"
	.global Func_0803f234
	.type Func_0803f234, %function
	.thumb_func
Func_0803f234:
	.incbin "baserom.gba", 0x0003f294, 0x000003dc
	.section .rom.0003f670, "ax"
	.global Func_0803f610
	.type Func_0803f610, %function
	.thumb_func
Func_0803f610:
	.incbin "baserom.gba", 0x0003f670, 0x00000004
	.section .rom.0003f674, "ax"
	.global Func_0803f614
	.type Func_0803f614, %function
	.thumb_func
Func_0803f614:
	.incbin "baserom.gba", 0x0003f674, 0x0000000c
	.section .rom.0003f680, "ax"
	.global Func_0803f620
	.type Func_0803f620, %function
	.thumb_func
Func_0803f620:
	.incbin "baserom.gba", 0x0003f680, 0x00000004
	.section .rom.0003f684, "ax"
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f684, 0x00000074
	.section .rom.0003f6f8, "ax"
	.global UiTextResource_SetPosition
	.type UiTextResource_SetPosition, %function
	.thumb_func
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6f8, 0x00000028
	.section .rom.0003f720, "ax"
	.global UiTextResource_Release
	.type UiTextResource_Release, %function
	.thumb_func
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f720, 0x00000098
	.global Resource_ResetPendingTransfer
	.type Resource_ResetPendingTransfer, %function
	.thumb_func
Resource_ResetPendingTransfer:
	.incbin "baserom.gba", 0x0003f7b8, 0x00000020
	.section .rom.0003f7d8, "ax"
	.global Func_0803f778
	.type Func_0803f778, %function
	.thumb_func
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7d8, 0x000000f4
	.section .rom.0003f8cc, "ax"
	.global Func_0803f86c
	.type Func_0803f86c, %function
	.thumb_func
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8cc, 0x000000d0
	.section .rom.0003f99c, "ax"
	.global Func_0803f93c
	.type Func_0803f93c, %function
	.thumb_func
Func_0803f93c:
	.incbin "baserom.gba", 0x0003f99c, 0x0000002c
	.section .rom.0003f9c8, "ax"
	.global Func_0803f968
	.type Func_0803f968, %function
	.thumb_func
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9c8, 0x00000058
	.section .rom.0003fa20, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa20, 0x00000728
	.section .rom.00040148, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x00040148, 0x00001a8c
	.section .rom.00041bd4, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bd4, 0x000000a4
	.section .rom.00041c78, "ax"
	.global Func_08041c0c
	.type Func_08041c0c, %function
	.thumb_func
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c78, 0x00000048
	.section .rom.00041cc0, "ax"
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041cc0, 0x0000031c
	.section .rom.00041fdc, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x00041fdc, 0x00000020
	.section .rom.00041ffc, "ax"
	.global Func_08041f90
	.type Func_08041f90, %function
	.thumb_func
Func_08041f90:
	.incbin "baserom.gba", 0x00041ffc, 0x00000014
	.section .rom.00042010, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x00042010, 0x0000006c
	.section .rom.0004207c, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x0004207c, 0x00000098
	.section .rom.00042114, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x00042114, 0x00000054
	.section .rom.00042168, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042168, 0x0000008c
	.section .rom.000421f4, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421f4, 0x0000005c
	.section .rom.00042250, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042250, 0x00000030
	.section .rom.00042280, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042280, 0x00000030
	.section .rom.000422b0, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x000422b0, 0x000000d0
	.section .rom.00042380, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042380, 0x00000088
	.section .rom.000424bc, "ax"
	.global Func_08042450
	.type Func_08042450, %function
	.thumb_func
Func_08042450:
	.incbin "baserom.gba", 0x000424bc, 0x000000b8
	.section .rom.00042574, "ax"
	.global Func_08042508
	.type Func_08042508, %function
	.thumb_func
Func_08042508:
	.incbin "baserom.gba", 0x00042574, 0x00000064
	.section .rom.000425e6, "ax"
	.incbin "baserom.gba", 0x000425e6, 0x00000002
	.section .rom.000425e8, "ax"
	.global Func_0804257c
	.type Func_0804257c, %function
	.thumb_func
Func_0804257c:
	.incbin "baserom.gba", 0x000425e8, 0x0000000c
	.section .rom.000425f4, "ax"
	.global Func_08042588
	.type Func_08042588, %function
	.thumb_func
Func_08042588:
	.incbin "baserom.gba", 0x000425f4, 0x00000074
	.section .rom.0004269c, "ax"
	.incbin "baserom.gba", 0x0004269c, 0x00000060
	.section .rom.000426fc, "ax"
	.global Func_08042690
	.type Func_08042690, %function
	.thumb_func
Func_08042690:
	.incbin "baserom.gba", 0x000426fc, 0x000002ec
	.section .rom.000429e8, "ax"
	.global Func_0804297c
	.type Func_0804297c, %function
	.thumb_func
Func_0804297c:
	.incbin "baserom.gba", 0x000429e8, 0x00000430
	.section .rom.00042e18, "ax"
	.global Func_08042dac
	.type Func_08042dac, %function
	.thumb_func
Func_08042dac:
	.incbin "baserom.gba", 0x00042e18, 0x00000318
	.section .rom.00043130, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043130, 0x00000198
	.section .rom.000432c8, "ax"
	.global Func_0804325c
	.type Func_0804325c, %function
	.thumb_func
Func_0804325c:
	.incbin "baserom.gba", 0x000432c8, 0x00000048
	.section .rom.00043310, "ax"
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x00043310, 0x00000248
	.section .rom.00043558, "ax"
	.global Func_080434ec
	.type Func_080434ec, %function
	.thumb_func
Func_080434ec:
	.incbin "baserom.gba", 0x00043558, 0x00000048
	.section .rom.000435a0, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x000435a0, 0x000000c8
	.section .rom.00043668, "ax"
	.global Func_080435fc
	.type Func_080435fc, %function
	.thumb_func
Func_080435fc:
	.incbin "baserom.gba", 0x00043668, 0x00000094
	.section .rom.000436fc, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436fc, 0x000000e4
	.section .rom.000437e0, "ax"
	.global Func_08043774
	.type Func_08043774, %function
	.thumb_func
Func_08043774:
	.incbin "baserom.gba", 0x000437e0, 0x00000098
	.section .rom.00043878, "ax"
	.global Func_0804380c
	.type Func_0804380c, %function
	.thumb_func
Func_0804380c:
	.incbin "baserom.gba", 0x00043878, 0x000000bc
	.section .rom.00043934, "ax"
	.global Func_080438c8
	.type Func_080438c8, %function
	.thumb_func
Func_080438c8:
	.incbin "baserom.gba", 0x00043934, 0x000000b0
	.section .rom.00043a30, "ax"
	.incbin "baserom.gba", 0x00043a30, 0x00000228
	.section .rom.00043c9c, "ax"
	.incbin "baserom.gba", 0x00043c9c, 0x00000830
	.section .rom.000444cc, "ax"
	.global Func_08044460
	.type Func_08044460, %function
	.thumb_func
Func_08044460:
	.incbin "baserom.gba", 0x000444cc, 0x00000018
	.section .rom.000444e4, "ax"
	.global Func_08044478
	.type Func_08044478, %function
	.thumb_func
Func_08044478:
	.incbin "baserom.gba", 0x000444e4, 0x00000010
	.section .rom.000444f4, "ax"
	.global Func_08044488
	.type Func_08044488, %function
	.thumb_func
Func_08044488:
	.incbin "baserom.gba", 0x000444f4, 0x000000d0
	.section .rom.000445c4, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445c4, 0x0000051c
	.section .rom.00044ae0, "ax"
	.global Func_08044a54
	.type Func_08044a54, %function
	.thumb_func
Func_08044a54:
	.incbin "baserom.gba", 0x00044ae0, 0x00000004
	.section .rom.00044ae4, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae4, 0x00000140
	.section .rom.00044c24, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044c24, 0x000000e8
	.section .rom.00044d0c, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044d0c, 0x00000148
	.section .rom.00044e54, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e54, 0x00000484
	.section .rom.00045346, "ax"
	.incbin "baserom.gba", 0x00045346, 0x00000076
	.section .rom.000453bc, "ax"
	.global Func_08045330
	.type Func_08045330, %function
	.thumb_func
Func_08045330:
	.incbin "baserom.gba", 0x000453bc, 0x000000a0
	.section .rom.0004545c, "ax"
	.global Func_080453d0
	.type Func_080453d0, %function
	.thumb_func
Func_080453d0:
	.incbin "baserom.gba", 0x0004545c, 0x00000094
	.section .rom.000455b2, "ax"
	.incbin "baserom.gba", 0x000455b2, 0x0000002a
	.section .rom.000455f0, "ax"
	.incbin "baserom.gba", 0x000455f0, 0x00000204
	.section .rom.0004580a, "ax"
	.incbin "baserom.gba", 0x0004580a, 0x00000032
	.section .rom.0004585a, "ax"
	.incbin "baserom.gba", 0x0004585a, 0x00000966
	.section .rom.000461c0, "ax"
	.global Func_08046134
	.type Func_08046134, %function
	.thumb_func
Func_08046134:
	.incbin "baserom.gba", 0x000461c0, 0x00000094
	.section .rom.00046254, "ax"
	.global Func_080461c8
	.type Func_080461c8, %function
	.thumb_func
Func_080461c8:
	.incbin "baserom.gba", 0x00046254, 0x00000098
	.section .rom.00046310, "ax"
	.incbin "baserom.gba", 0x00046310, 0x00000150
	.section .rom.0004649e, "ax"
	.incbin "baserom.gba", 0x0004649e, 0x0000364e
	.section .rom.00049b3a, "ax"
	.incbin "baserom.gba", 0x00049b3a, 0x00001f5e
	.section .rom.0004bad2, "ax"
	.incbin "baserom.gba", 0x0004bad2, 0x00000352
	.section .rom.0004be24, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be24, 0x000014d4
	.section .rom.0004d2f8, "ax"
	.global AffineEffect_InitializeWork
	.type AffineEffect_InitializeWork, %function
	.thumb_func
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d2f8, 0x0000003c
	.section .rom.0004d334, "ax"
	.global Menu_EndResourceSelection
	.type Menu_EndResourceSelection, %function
	.thumb_func
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d334, 0x00000054
	.section .rom.0004d388, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d388, 0x00000220
	.section .rom.0004d5a8, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x0004d5a8, 0x0000005c
	.section .rom.0004d604, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d604, 0x00000188
	.section .rom.0004d78c, "ax"
	.global Menu_AnimateSelectionToEntry
	.type Menu_AnimateSelectionToEntry, %function
	.thumb_func
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d78c, 0x0000003c
	.section .rom.0004d7c8, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7c8, 0x00000228
	.section .rom.0004d9f0, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9f0, 0x0000016c
	.section .rom.0004dc30, "ax"
	.global Func_0804da3c
	.type Func_0804da3c, %function
	.thumb_func
Func_0804da3c:
	.incbin "baserom.gba", 0x0004dc30, 0x00000050
	.section .rom.0004dcae, "ax"
	.incbin "baserom.gba", 0x0004dcae, 0x000000ba
	.section .rom.0004dd68, "ax"
	.global Func_0804db74
	.type Func_0804db74, %function
	.thumb_func
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd68, 0x00000254
	.section .rom.0004e094, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e094, 0x000000c4
	.section .rom.0004e158, "ax"
	.global Menu_HandleFlagGridInput
	.type Menu_HandleFlagGridInput, %function
	.thumb_func
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e158, 0x00000140
	.section .rom.0004e2c2, "ax"
	.incbin "baserom.gba", 0x0004e2c2, 0x00000002
	.section .rom.0004e2c4, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2c4, 0x00000108
	.section .rom.0004e3cc, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3cc, 0x0000021c
	.section .rom.0004e5e8, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e5e8, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed4c, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f318, 0x000058f0
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x00054c08, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055018, 0x00005748
	.section .rom.0005c360, "ax"
	.incbin "baserom.gba", 0x0005c360, 0x0000387c
	.section .rom.000a8fd8, "ax"
	.incbin "baserom.gba", 0x000a8fd8, 0x0000030c
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a92e4, 0x0000cd1c
	.section .rom.000b6000, "ax"
	.global Trade_GetOfferStateFar
	.type Trade_GetOfferStateFar, %function
	.thumb_func
Trade_GetOfferStateFar:
	.global Resource_FarCall005
Resource_FarCall005:
	.incbin "baserom.gba", 0x000b6000, 0x00000008
	.section .rom.000b6008, "ax"
	.global Owner_RecalculateStatsFar
	.type Owner_RecalculateStatsFar, %function
	.thumb_func
Owner_RecalculateStatsFar:
	.incbin "baserom.gba", 0x000b6008, 0x00000008
	.section .rom.000b6010, "ax"
	.global Item_Get
	.type Item_Get, %function
	.thumb_func
Item_Get:
	.incbin "baserom.gba", 0x000b6010, 0x00000010
	.section .rom.000b6020, "ax"
	.global Inventory_AddItemFar
	.type Inventory_AddItemFar, %function
	.thumb_func
Inventory_AddItemFar:
	.incbin "baserom.gba", 0x000b6020, 0x00000030
	.section .rom.000b6050, "ax"
	.global Inventory_RemoveFar
	.type Inventory_RemoveFar, %function
	.thumb_func
Inventory_RemoveFar:
	.incbin "baserom.gba", 0x000b6050, 0x00000028
	.section .rom.000b6078, "ax"
	.global BattleAction_Get
	.type BattleAction_Get, %function
	.thumb_func
BattleAction_Get:
	.incbin "baserom.gba", 0x000b6078, 0x00000008
	.section .rom.000b6080, "ax"
	.global OwnerAction_AddFar
	.type OwnerAction_AddFar, %function
	.thumb_func
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000b6080, 0x00000008
	.section .rom.000b6088, "ax"
	.global Equipment_HasValueFar
	.type Equipment_HasValueFar, %function
	.thumb_func
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000b6088, 0x00000070
	.section .rom.000b60f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000b60f8, 0x00000050
	.section .rom.000b6148, "ax"
	.global BattleRandom16Far
	.type BattleRandom16Far, %function
	.thumb_func
BattleRandom16Far:
	.incbin "baserom.gba", 0x000b6148, 0x00000008
	.section .rom.000b6150, "ax"
	.global Djinn_AddToOwnerFar
	.type Djinn_AddToOwnerFar, %function
	.thumb_func
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000b6150, 0x00000008
	.section .rom.000b6158, "ax"
	.global Djinn_ActivateFar
	.type Djinn_ActivateFar, %function
	.thumb_func
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000b6158, 0x00000010
	.section .rom.000b6168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b6168, 0x00000058
	.section .rom.000b61c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b61c0, 0x000000c0
	.section .rom.000b6280, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000b6280, 0x000000c8
	.section .rom.000b6348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000b6348, 0x0000007c
	.section .rom.000b63f6, "ax"
	.incbin "baserom.gba", 0x000b63f6, 0x00000002
	.section .rom.000b63f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000b63f8, 0x00001448
	.section .rom.000b7874, "ax"
	.incbin "baserom.gba", 0x000b7874, 0x000001c8
	.section .rom.000b7c72, "ax"
	.incbin "baserom.gba", 0x000b7c72, 0x00000002
	.section .rom.000b7c74, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b7c74, 0x00000060
	.section .rom.000b7e4c, "ax"
	.incbin "baserom.gba", 0x000b7e4c, 0x00000058
	.section .rom.000b7ea4, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000b7ea4, 0x00000030
	.section .rom.000b7f40, "ax"
	.incbin "baserom.gba", 0x000b7f40, 0x000000d4
	.section .rom.000b80ee, "ax"
	.incbin "baserom.gba", 0x000b80ee, 0x00000066
	.section .rom.000b8154, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000b8154, 0x00000080
	.section .rom.000b8206, "ax"
	.incbin "baserom.gba", 0x000b8206, 0x0000009e
	.section .rom.000b8344, "ax"
	.incbin "baserom.gba", 0x000b8344, 0x00000040
	.section .rom.000b8384, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000b8384, 0x00000028
	.section .rom.000b8448, "ax"
	.incbin "baserom.gba", 0x000b8448, 0x000004e0
	.section .rom.000b8928, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b8928, 0x00000298
	.section .rom.000b8bf8, "ax"
	.incbin "baserom.gba", 0x000b8bf8, 0x000001d0
	.section .rom.000b8de4, "ax"
	.incbin "baserom.gba", 0x000b8de4, 0x000000a0
	.section .rom.000b8ebc, "ax"
	.incbin "baserom.gba", 0x000b8ebc, 0x00000024
	.section .rom.000b8f34, "ax"
	.incbin "baserom.gba", 0x000b8f34, 0x00000054
	.section .rom.000b8fa0, "ax"
	.incbin "baserom.gba", 0x000b8fa0, 0x000002f4
	.section .rom.000b92a4, "ax"
	.incbin "baserom.gba", 0x000b92a4, 0x000001c8
	.section .rom.000b94c6, "ax"
	.incbin "baserom.gba", 0x000b94c6, 0x000005da
	.section .rom.000b9bc2, "ax"
	.incbin "baserom.gba", 0x000b9bc2, 0x000000c2
	.section .rom.000b9ca6, "ax"
	.incbin "baserom.gba", 0x000b9ca6, 0x00000002
	.section .rom.000b9ca8, "ax"
	.global Djinn_Activate
	.type Djinn_Activate, %function
	.thumb_func
Djinn_Activate:
	.incbin "baserom.gba", 0x000b9ca8, 0x00000068
	.section .rom.000b9d10, "ax"
	.global Djinn_Deactivate
	.type Djinn_Deactivate, %function
	.thumb_func
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000b9d10, 0x00000054
	.section .rom.000b9d64, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b9d64, 0x000000ac
	.section .rom.000ba00e, "ax"
	.incbin "baserom.gba", 0x000ba00e, 0x00001362
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000bb370, 0x0000f1a8
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000ca518, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000ca600, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000cfbbc, 0x00001444
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000d1000, 0x000002b8
	.section .rom.000d13b8, "ax"
	.incbin "baserom.gba", 0x000d13b8, 0x00000130
	.section .rom.000d1518, "ax"
	.incbin "baserom.gba", 0x000d1518, 0x00000018
	.section .rom.000d1598, "ax"
	.incbin "baserom.gba", 0x000d1598, 0x00000030
	.section .rom.000d1628, "ax"
	.incbin "baserom.gba", 0x000d1628, 0x00000010
	.section .rom.000d1658, "ax"
	.incbin "baserom.gba", 0x000d1658, 0x00000050
	.section .rom.000d18d8, "ax"
	.incbin "baserom.gba", 0x000d18d8, 0x00000080
	.section .rom.000d19cc, "ax"
	.incbin "baserom.gba", 0x000d19cc, 0x00000a00
	.section .rom.000d25a8, "ax"
	.incbin "baserom.gba", 0x000d25a8, 0x00000378
	.section .rom.000d2934, "ax"
	.incbin "baserom.gba", 0x000d2934, 0x00000494
	.section .rom.000d2dc8, "ax"
	.global Func_080ca18c
	.type Func_080ca18c, %function
	.thumb_func
Func_080ca18c:
	.incbin "baserom.gba", 0x000d2dc8, 0x000003c4
	.section .rom.000d318c, "ax"
	.global Func_080dc0b8
	.type Func_080dc0b8, %function
	.thumb_func
Func_080dc0b8:
	.incbin "baserom.gba", 0x000d318c, 0x00000018
	.section .rom.000d31a4, "ax"
	.global Func_080d295c
	.type Func_080d295c, %function
	.thumb_func
Func_080d295c:
	.incbin "baserom.gba", 0x000d31a4, 0x00000018
	.section .rom.000d31bc, "ax"
	.global Func_080ca1a4
	.type Func_080ca1a4, %function
	.thumb_func
Func_080ca1a4:
	.incbin "baserom.gba", 0x000d31bc, 0x000000c4
	.section .rom.000d3280, "ax"
	.global Func_080d46a4
	.type Func_080d46a4, %function
	.thumb_func
Func_080d46a4:
	.incbin "baserom.gba", 0x000d3280, 0x00000424
	.section .rom.000d36a4, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000d36a4, 0x000006e0
	.section .rom.000d3d84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000d3d84, 0x000001ec
	.section .rom.000d3fc4, "ax"
	.incbin "baserom.gba", 0x000d3fc4, 0x00000704
	.section .rom.000d46c8, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000d46c8, 0x0000002c
	.section .rom.000d46f4, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000d46f4, 0x00001654
	.section .rom.000d5d76, "ax"
	.incbin "baserom.gba", 0x000d5d76, 0x00000002
	.section .rom.000d5d78, "ax"
	.global Func_080d25c8
	.type Func_080d25c8, %function
	.thumb_func
Func_080d25c8:
	.incbin "baserom.gba", 0x000d5d78, 0x00000150
	.section .rom.000d5ec8, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000d5ec8, 0x00000a54
	.section .rom.000d691c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d691c, 0x000002dc
	.section .rom.000d6bf8, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000d6bf8, 0x00000048
	.section .rom.000d6c72, "ax"
	.incbin "baserom.gba", 0x000d6c72, 0x00000002
	.section .rom.000d6c74, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000d6c74, 0x0000010c
	.section .rom.000d6d80, "ax"
	.global Func_080eaf28
	.type Func_080eaf28, %function
	.thumb_func
Func_080eaf28:
	.incbin "baserom.gba", 0x000d6d80, 0x000001dc
	.section .rom.000d6f5c, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000d6f5c, 0x00000054
	.section .rom.000d6fb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d6fb0, 0x00000ba8
	.section .rom.000d7b58, "ax"
	.global Func_080dca84
	.type Func_080dca84, %function
	.thumb_func
Func_080dca84:
	.incbin "baserom.gba", 0x000d7b58, 0x00000028
	.section .rom.000d7b80, "ax"
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.section .rom.000d7b94, "ax"
	.incbin "baserom.gba", 0x000d7b94, 0x000002ac
	.section .rom.000d7e80, "ax"
	.incbin "baserom.gba", 0x000d7e80, 0x000002fc
	.section .rom.000d817c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d817c, 0x00000f7c
	.section .rom.000d90f8, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d90f8, 0x000000d4
	.section .rom.000d91cc, "ax"
	.global Func_080d00f8
	.type Func_080d00f8, %function
	.thumb_func
Func_080d00f8:
	.incbin "baserom.gba", 0x000d91cc, 0x00000354
	.section .rom.000d9520, "ax"
	.global Func_080d0bec
	.type Func_080d0bec, %function
	.thumb_func
Func_080d0bec:
	.incbin "baserom.gba", 0x000d9520, 0x00000228
	.section .rom.000d9748, "ax"
	.global Func_080d01cc
	.type Func_080d01cc, %function
	.thumb_func
Func_080d01cc:
	.incbin "baserom.gba", 0x000d9748, 0x000004a4
	.section .rom.000d9bec, "ax"
	.global Func_080d7240
	.type Func_080d7240, %function
	.thumb_func
Func_080d7240:
	.incbin "baserom.gba", 0x000d9bec, 0x00000064
	.section .rom.000d9c50, "ax"
	.global Func_080d170c
	.type Func_080d170c, %function
	.thumb_func
Func_080d170c:
	.incbin "baserom.gba", 0x000d9c50, 0x00000a34
	.section .rom.000da684, "ax"
	.global Func_080d0748
	.type Func_080d0748, %function
	.thumb_func
Func_080d0748:
	.incbin "baserom.gba", 0x000da684, 0x00000074
	.section .rom.000da6f8, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000da6f8, 0x00000014
	.section .rom.000da70c, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000da70c, 0x00000020
	.section .rom.000da72c, "ax"
	.global Func_080d1684
	.type Func_080d1684, %function
	.thumb_func
Func_080d1684:
	.incbin "baserom.gba", 0x000da72c, 0x00000020
	.section .rom.000da74c, "ax"
	.global BattleFx_StartBufferInterpolation
	.type BattleFx_StartBufferInterpolation, %function
	.thumb_func
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000da74c, 0x00000014
	.section .rom.000da760, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000da760, 0x0000004c
	.section .rom.000da7ac, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000da7ac, 0x0000003c
	.section .rom.000da7e8, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000da7e8, 0x00000034
	.section .rom.000da83e, "ax"
	.incbin "baserom.gba", 0x000da83e, 0x00000516
	.section .rom.000dad54, "ax"
	.global Func_080e70f8
	.type Func_080e70f8, %function
	.thumb_func
Func_080e70f8:
	.incbin "baserom.gba", 0x000dad54, 0x00000058
	.section .rom.000dadac, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000dadac, 0x00000044
	.section .rom.000dadf0, "ax"
	.global Func_080d1d54
	.type Func_080d1d54, %function
	.thumb_func
Func_080d1d54:
	.incbin "baserom.gba", 0x000dadf0, 0x00000028
	.section .rom.000dae18, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000dae18, 0x00000094
	.section .rom.000daeac, "ax"
	.global Func_080d1eac
	.type Func_080d1eac, %function
	.thumb_func
Func_080d1eac:
	.incbin "baserom.gba", 0x000daeac, 0x0000002c
	.section .rom.000daed8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000daed8, 0x00000010
	.section .rom.000daee8, "ax"
	.global Func_080d4714
	.type Func_080d4714, %function
	.thumb_func
Func_080d4714:
	.incbin "baserom.gba", 0x000daee8, 0x00000358
	.section .rom.000db240, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000db240, 0x00000020
	.section .rom.000db260, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000db260, 0x00000178
	.section .rom.000db488, "ax"
	.incbin "baserom.gba", 0x000db488, 0x00000140
	.section .rom.000db5c8, "ax"
	.global Func_080d072c
	.type Func_080d072c, %function
	.thumb_func
Func_080d072c:
	.incbin "baserom.gba", 0x000db5c8, 0x00000218
	.section .rom.000db7fe, "ax"
	.incbin "baserom.gba", 0x000db7fe, 0x00000152
	.section .rom.000db950, "ax"
	.global Func_080cb6f4
	.type Func_080cb6f4, %function
	.thumb_func
Func_080cb6f4:
	.incbin "baserom.gba", 0x000db950, 0x000000b0
	.section .rom.000dba30, "ax"
	.global Func_080d16f8
	.type Func_080d16f8, %function
	.thumb_func
Func_080d16f8:
	.incbin "baserom.gba", 0x000dba30, 0x00000028
	.section .rom.000dba58, "ax"
	.global Func_080d174c
	.type Func_080d174c, %function
	.thumb_func
Func_080d174c:
	.incbin "baserom.gba", 0x000dba58, 0x000001a0
	.section .rom.000dbc58, "ax"
	.incbin "baserom.gba", 0x000dbc58, 0x00000060
	.section .rom.000dbcb8, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000dbcb8, 0x00000044
	.section .rom.000dbcfc, "ax"
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000dbcfc, 0x0000007c
	.section .rom.000dbdd8, "ax"
	.incbin "baserom.gba", 0x000dbdd8, 0x00000038
	.section .rom.000dbf3c, "ax"
	.incbin "baserom.gba", 0x000dbf3c, 0x00000080
	.section .rom.000dc064, "ax"
	.global Func_080d3070
	.type Func_080d3070, %function
	.thumb_func
Func_080d3070:
	.incbin "baserom.gba", 0x000dc064, 0x0000008c
	.section .rom.000dc10a, "ax"
	.incbin "baserom.gba", 0x000dc10a, 0x0000012a
	.section .rom.000dc25c, "ax"
	.incbin "baserom.gba", 0x000dc25c, 0x00000334
	.section .rom.000dc5f2, "ax"
	.incbin "baserom.gba", 0x000dc5f2, 0x00000002
	.section .rom.000dc5f4, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000dc5f4, 0x000002d4
	.section .rom.000dc900, "ax"
	.incbin "baserom.gba", 0x000dc900, 0x0000001c
	.section .rom.000dc91c, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000dc91c, 0x00000018
	.section .rom.000dc934, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000dc934, 0x000000d0
	.section .rom.000dca04, "ax"
	.global Func_080d3928
	.type Func_080d3928, %function
	.thumb_func
Func_080d3928:
	.incbin "baserom.gba", 0x000dca04, 0x00000118
	.section .rom.000dcb1c, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000dcb1c, 0x000000d0
	.section .rom.000dcbec, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000dcbec, 0x00000034
	.section .rom.000dcc20, "ax"
	.global Func_080d3c2c
	.type Func_080d3c2c, %function
	.thumb_func
Func_080d3c2c:
	.incbin "baserom.gba", 0x000dcc20, 0x000005bc
	.section .rom.000dd1f0, "ax"
	.incbin "baserom.gba", 0x000dd1f0, 0x000004a8
	.section .rom.000dd698, "ax"
	.global Func_080cad9c
	.type Func_080cad9c, %function
	.thumb_func
Func_080cad9c:
	.incbin "baserom.gba", 0x000dd698, 0x00000070
	.section .rom.000dd708, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000dd708, 0x00000278
	.section .rom.000dd994, "ax"
	.incbin "baserom.gba", 0x000dd994, 0x00000114
	.section .rom.000ddaa8, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000ddaa8, 0x00000058
	.section .rom.000ddb00, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000ddb00, 0x000001fc
	.section .rom.000ddcfc, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000ddcfc, 0x000003f0
	.section .rom.000de0ec, "ax"
	.global Func_080cdbf8
	.type Func_080cdbf8, %function
	.thumb_func
Func_080cdbf8:
	.incbin "baserom.gba", 0x000de0ec, 0x000002c0
	.section .rom.000de3ac, "ax"
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000de3ac, 0x000009b8
	.section .rom.000ded64, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000ded64, 0x00000070
	.section .rom.000dedd4, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000dedd4, 0x00000070
	.section .rom.000dee56, "ax"
	.incbin "baserom.gba", 0x000dee56, 0x00000aaa
	.section .rom.000df900, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000df900, 0x00000284
	.section .rom.000dfb84, "ax"
	.global Func_080d7408
	.type Func_080d7408, %function
	.thumb_func
Func_080d7408:
	.incbin "baserom.gba", 0x000dfb84, 0x000006b0
	.section .rom.000e0234, "ax"
	.global Func_080d7430
	.type Func_080d7430, %function
	.thumb_func
Func_080d7430:
	.incbin "baserom.gba", 0x000e0234, 0x000000c4
	.section .rom.000e02f8, "ax"
	.global Func_080d2260
	.type Func_080d2260, %function
	.thumb_func
Func_080d2260:
	.incbin "baserom.gba", 0x000e02f8, 0x000000b0
	.section .rom.000e03a8, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000e03a8, 0x0000002c
	.section .rom.000e03d4, "ax"
	.global Func_080d7304
	.type Func_080d7304, %function
	.thumb_func
Func_080d7304:
	.incbin "baserom.gba", 0x000e03d4, 0x00000028
	.section .rom.000e03fc, "ax"
	.global Func_080d73b4
	.type Func_080d73b4, %function
	.thumb_func
Func_080d73b4:
	.incbin "baserom.gba", 0x000e03fc, 0x00000028
	.section .rom.000e0424, "ax"
	.global Func_080d73e0
	.type Func_080d73e0, %function
	.thumb_func
Func_080d73e0:
	.incbin "baserom.gba", 0x000e0424, 0x000000c0
	.section .rom.000e0518, "ax"
	.incbin "baserom.gba", 0x000e0518, 0x00000264
	.section .rom.000e077c, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000e077c, 0x000002f0
	.section .rom.000e0a6c, "ax"
	.global BattleFx_InitializeSlots
	.type BattleFx_InitializeSlots, %function
	.thumb_func
BattleFx_InitializeSlots:
	.incbin "baserom.gba", 0x000e0a6c, 0x0000003c
	.section .rom.000e0aa8, "ax"
	.global Func_080d7ab4
	.type Func_080d7ab4, %function
	.thumb_func
Func_080d7ab4:
	.incbin "baserom.gba", 0x000e0aa8, 0x00000c58
	.section .rom.000e1734, "ax"
	.incbin "baserom.gba", 0x000e1734, 0x0000023c
	.section .rom.000e1970, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000e1970, 0x000003ec
	.section .rom.000e1d5c, "ax"
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000e1d5c, 0x0000039c
	.section .rom.000e20f8, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000e20f8, 0x000001d0
	.section .rom.000e22c8, "ax"
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000e22c8, 0x0000043c
	.section .rom.000e2704, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000e2704, 0x000003a0
	.section .rom.000e2aa4, "ax"
	.global Func_080d9b08
	.type Func_080d9b08, %function
	.thumb_func
Func_080d9b08:
	.incbin "baserom.gba", 0x000e2aa4, 0x00000020
	.section .rom.000e2ac4, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000e2ac4, 0x00000038
	.section .rom.000e2afc, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000e2afc, 0x00000390
	.section .rom.000e2e8c, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000e2e8c, 0x00000a70
	.section .rom.000e38fc, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000e38fc, 0x00000030
	.section .rom.000e392c, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000e392c, 0x00000594
	.section .rom.000e3ec0, "ax"
	.global Func_080cdf5c
	.type Func_080cdf5c, %function
	.thumb_func
Func_080cdf5c:
	.incbin "baserom.gba", 0x000e3ec0, 0x000001e4
	.section .rom.000e40a4, "ax"
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000e40a4, 0x00000394
	.section .rom.000e4438, "ax"
	.global Func_080dbd48
	.type Func_080dbd48, %function
	.thumb_func
Func_080dbd48:
	.incbin "baserom.gba", 0x000e4438, 0x00000048
	.section .rom.000e44aa, "ax"
	.incbin "baserom.gba", 0x000e44aa, 0x00000002
	.section .rom.000e44ac, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000e44ac, 0x000001b8
	.section .rom.000e4664, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e4664, 0x000001d8
	.section .rom.000e483c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000e483c, 0x0000003c
	.section .rom.000e4878, "ax"
	.global FieldEvent_RunTypeHandler
	.type FieldEvent_RunTypeHandler, %function
	.thumb_func
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000e4878, 0x00000098
	.section .rom.000e4910, "ax"
	.global Func_080d1df0
	.type Func_080d1df0, %function
	.thumb_func
Func_080d1df0:
	.incbin "baserom.gba", 0x000e4910, 0x000000a4
	.section .rom.000e49b4, "ax"
	.global Func_080eb2c8
	.type Func_080eb2c8, %function
	.thumb_func
Func_080eb2c8:
	.incbin "baserom.gba", 0x000e49b4, 0x0000000c
	.section .rom.000e49c0, "ax"
	.global Func_080eb2d0
	.type Func_080eb2d0, %function
	.thumb_func
Func_080eb2d0:
	.incbin "baserom.gba", 0x000e49c0, 0x0000030c
	.section .rom.000e4ccc, "ax"
	.global Object_Spawn
	.type Object_Spawn, %function
	.thumb_func
Object_Spawn:
	.incbin "baserom.gba", 0x000e4ccc, 0x00000070
	.section .rom.000e4d3c, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000e4d3c, 0x00000080
	.section .rom.000e4dbc, "ax"
	.global Func_080e1420
	.type Func_080e1420, %function
	.thumb_func
Func_080e1420:
	.incbin "baserom.gba", 0x000e4dbc, 0x0000000c
	.section .rom.000e4dc8, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e4dc8, 0x00000014
	.section .rom.000e4ddc, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000e4ddc, 0x0000000c
	.section .rom.000e4de8, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000e4de8, 0x00000014
	.section .rom.000e4dfc, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e4dfc, 0x0000003c
	.section .rom.000e4e38, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e4e38, 0x00000274
	.section .rom.000e50ac, "ax"
	.global Func_080cb6c8
	.type Func_080cb6c8, %function
	.thumb_func
Func_080cb6c8:
	.incbin "baserom.gba", 0x000e50ac, 0x00000054
	.section .rom.000e5100, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000e5100, 0x000000a4
	.section .rom.000e51a4, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000e51a4, 0x00000094
	.section .rom.000e5238, "ax"
	.global Func_080db9cc
	.type Func_080db9cc, %function
	.thumb_func
Func_080db9cc:
	.incbin "baserom.gba", 0x000e5238, 0x00000734
	.section .rom.000e596c, "ax"
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000e596c, 0x000000d8
	.section .rom.000e5a44, "ax"
	.global Func_080cded4
	.type Func_080cded4, %function
	.thumb_func
Func_080cded4:
	.incbin "baserom.gba", 0x000e5a44, 0x00000034
	.section .rom.000e5a78, "ax"
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000e5a78, 0x00000058
	.section .rom.000e5ad0, "ax"
	.global Func_080dca50
	.type Func_080dca50, %function
	.thumb_func
Func_080dca50:
	.incbin "baserom.gba", 0x000e5ad0, 0x00000578
	.section .rom.000e6048, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000e6048, 0x000005e8
	.section .rom.000e6630, "ax"
	.global Func_080ca494
	.type Func_080ca494, %function
	.thumb_func
Func_080ca494:
	.incbin "baserom.gba", 0x000e6630, 0x0000002c
	.section .rom.000e665c, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000e665c, 0x00000f10
	.section .rom.000e7598, "ax"
	.incbin "baserom.gba", 0x000e7598, 0x00000714
	.section .rom.000e7cac, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000e7cac, 0x0000101c
	.section .rom.000e8cec, "ax"
	.incbin "baserom.gba", 0x000e8cec, 0x000005f4
	.section .rom.000e92fc, "ax"
	.global Func_080e0308
	.type Func_080e0308, %function
	.thumb_func
Func_080e0308:
	.incbin "baserom.gba", 0x000e92fc, 0x00000054
	.section .rom.000e9350, "ax"
	.global Func_080e035c
	.type Func_080e035c, %function
	.thumb_func
Func_080e035c:
	.incbin "baserom.gba", 0x000e9350, 0x00000050
	.section .rom.000e93b6, "ax"
	.incbin "baserom.gba", 0x000e93b6, 0x00000fba
	.section .rom.000ea370, "ax"
	.global Func_080e15fc
	.type Func_080e15fc, %function
	.thumb_func
Func_080e15fc:
	.incbin "baserom.gba", 0x000ea370, 0x000000a4
	.section .rom.000ea414, "ax"
	.global Func_080e1650
	.type Func_080e1650, %function
	.thumb_func
Func_080e1650:
	.incbin "baserom.gba", 0x000ea414, 0x000001dc
	.section .rom.000ea5f0, "ax"
	.global Func_080dc1b0
	.type Func_080dc1b0, %function
	.thumb_func
Func_080dc1b0:
	.incbin "baserom.gba", 0x000ea5f0, 0x00000054
	.section .rom.000ea644, "ax"
	.global Field_BeginPaletteTransition
	.type Field_BeginPaletteTransition, %function
	.thumb_func
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000ea644, 0x00000f98
	.section .rom.000eb5dc, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000eb5dc, 0x0000018c
	.section .rom.000eb768, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.type ObjectGroup_ApplyRandomChildValues, %function
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000eb768, 0x00000f24
	.section .rom.000ec68c, "ax"
	.global Func_080da938
	.type Func_080da938, %function
	.thumb_func
Func_080da938:
	.incbin "baserom.gba", 0x000ec68c, 0x00000148
	.section .rom.000ec7d4, "ax"
	.global Func_080d4ab4
	.type Func_080d4ab4, %function
	.thumb_func
Func_080d4ab4:
	.incbin "baserom.gba", 0x000ec7d4, 0x00000a64
	.section .rom.000ed238, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000ed238, 0x00002eb4
	.section .rom.000f00ec, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000f00ec, 0x000036c8
	.section .rom.000f37b4, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000f37b4, 0x00000114
	.section .rom.000f38c8, "ax"
	.global Func_080ed804
	.type Func_080ed804, %function
	.thumb_func
Func_080ed804:
	.incbin "baserom.gba", 0x000f38c8, 0x00000140
	.section .rom.000f3a08, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f3a08, 0x0000015c
	.section .rom.000f3b64, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f3b64, 0x00000028
	.section .rom.000f3b8c, "ax"
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000f3b8c, 0x00000038
	.section .rom.000f3bc4, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000f3bc4, 0x00000118
	.section .rom.000f3cdc, "ax"
	.global Func_080eab70
	.type Func_080eab70, %function
	.thumb_func
Func_080eab70:
	.incbin "baserom.gba", 0x000f3cdc, 0x00000114
	.section .rom.000f3df0, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000f3df0, 0x000000b8
	.section .rom.000f3ea8, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000f3ea8, 0x00000074
	.section .rom.000f3f1c, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000f3f1c, 0x00000070
	.section .rom.000f3f8c, "ax"
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000f3f8c, 0x00000084
	.section .rom.000f4010, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000f4010, 0x0000027c
	.section .rom.000f428c, "ax"
	.global Func_080eaf98
	.type Func_080eaf98, %function
	.thumb_func
Func_080eaf98:
	.incbin "baserom.gba", 0x000f428c, 0x00000030
	.section .rom.000f42bc, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000f42bc, 0x00000008
	.section .rom.000f42c4, "ax"
	.global Func_080eb298
	.type Func_080eb298, %function
	.thumb_func
Func_080eb298:
	.incbin "baserom.gba", 0x000f42c4, 0x00000690
	.section .rom.000f4954, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000f4954, 0x000002d0
	.section .rom.000f4c24, "ax"
	.global Func_080ca1bc
	.type Func_080ca1bc, %function
	.thumb_func
Func_080ca1bc:
	.incbin "baserom.gba", 0x000f4c24, 0x00000240
	.section .rom.000f4e64, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000f4e64, 0x00000024
	.section .rom.000f4e9a, "ax"
	.incbin "baserom.gba", 0x000f4e9a, 0x0000195e
	.section .rom.000f67f8, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000f67f8, 0x00003650
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f9e48, 0x000000d8
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f9f20, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000fa79c, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000fc2c4, 0x000004b0
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000fc774, 0x0000188c
	.section .rom.000fe028, "ax"
	.incbin "baserom.gba", 0x000fe028, 0x00000048
	.section .rom.000fe0e0, "ax"
	.incbin "baserom.gba", 0x000fe0e0, 0x0000003c
	.section .rom.000fe170, "ax"
	.incbin "baserom.gba", 0x000fe170, 0x00000494
	.section .rom.000fe64a, "ax"
	.incbin "baserom.gba", 0x000fe64a, 0x000001ea
	.section .rom.000fe834, "ax"
	.global UiIcon_CreateWithResourceVariant
	.type UiIcon_CreateWithResourceVariant, %function
	.thumb_func
UiIcon_CreateWithResourceVariant:
	.incbin "baserom.gba", 0x000fe834, 0x00000048
	.section .rom.000fe87c, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fe87c, 0x00000390
	.section .rom.000fec88, "ax"
	.incbin "baserom.gba", 0x000fec88, 0x000002ac
	.section .rom.000fef90, "ax"
	.incbin "baserom.gba", 0x000fef90, 0x000001d4
	.section .rom.000ff216, "ax"
	.incbin "baserom.gba", 0x000ff216, 0x00000182
	.section .rom.000ff3aa, "ax"
	.incbin "baserom.gba", 0x000ff3aa, 0x000000ee
	.section .rom.000ff498, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000ff498, 0x00001844
	.section .rom.00100cee, "ax"
	.incbin "baserom.gba", 0x00100cee, 0x000003f2
	.section .rom.00101140, "ax"
	.incbin "baserom.gba", 0x00101140, 0x00000c40
	.section .rom.00101de8, "ax"
	.incbin "baserom.gba", 0x00101de8, 0x00000d54
	.section .rom.00102b3c, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00102b3c, 0x0000177c
	.section .rom.001042b8, "ax"
	.global Func_080fe184
	.type Func_080fe184, %function
	.thumb_func
Func_080fe184:
	.incbin "baserom.gba", 0x001042b8, 0x000000f0
	.section .rom.001043a8, "ax"
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x001043a8, 0x000023d0
	.section .rom.001067b4, "ax"
	.incbin "baserom.gba", 0x001067b4, 0x00004a50
	.section .rom.0010b204, "ax"
	.global Menu_ReleaseEntryObjects
	.type Menu_ReleaseEntryObjects, %function
	.thumb_func
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x0010b204, 0x000000f0
	.section .rom.0010b2f4, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010b2f4, 0x000000c4
	.section .rom.0010b3b8, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x0010b3b8, 0x00000040
	.section .rom.0010b3f8, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x0010b3f8, 0x00000054
	.section .rom.0010b44c, "ax"
	.global Func_0810526c
	.type Func_0810526c, %function
	.thumb_func
Func_0810526c:
	.incbin "baserom.gba", 0x0010b44c, 0x0000002c
	.section .rom.0010b478, "ax"
	.global Func_081050b8
	.type Func_081050b8, %function
	.thumb_func
Func_081050b8:
	.incbin "baserom.gba", 0x0010b478, 0x00000024
	.section .rom.0010b49c, "ax"
	.global Func_081052ac
	.type Func_081052ac, %function
	.thumb_func
Func_081052ac:
	.incbin "baserom.gba", 0x0010b49c, 0x00000230
	.section .rom.0010b742, "ax"
	.incbin "baserom.gba", 0x0010b742, 0x00000376
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x0010bab8, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x0010babc, 0x0000007e
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x0010bb3a, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x0010bb47, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x0010bb54, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x0010bb6c, 0x00000494
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x0010c000, 0x00000088
	.section .rom.0010c0a8, "ax"
	.incbin "baserom.gba", 0x0010c0a8, 0x00000438
	.section .rom.0010c4f4, "ax"
	.incbin "baserom.gba", 0x0010c4f4, 0x00000640
	.section .rom.0010cb70, "ax"
	.incbin "baserom.gba", 0x0010cb70, 0x00000f28
	.section .rom.0010dad8, "ax"
	.incbin "baserom.gba", 0x0010dad8, 0x00000d30
	.section .rom.0010e808, "ax"
	.global Func_0810a804
	.type Func_0810a804, %function
	.thumb_func
Func_0810a804:
	.incbin "baserom.gba", 0x0010e808, 0x00000030
	.section .rom.0010e838, "ax"
	.global Func_0810a834
	.type Func_0810a834, %function
	.thumb_func
Func_0810a834:
	.incbin "baserom.gba", 0x0010e838, 0x00000018
	.section .rom.0010e850, "ax"
	.global Func_0810a84c
	.type Func_0810a84c, %function
	.thumb_func
Func_0810a84c:
	.incbin "baserom.gba", 0x0010e850, 0x00000018
	.section .rom.0010e8f0, "ax"
	.incbin "baserom.gba", 0x0010e8f0, 0x00009710
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000070
	.section .rom.00118230, "ax"
	.incbin "baserom.gba", 0x00118230, 0x00000008
	.section .rom.00118244, "ax"
	.incbin "baserom.gba", 0x00118244, 0x00000040
	.section .rom.00118394, "ax"
	.incbin "baserom.gba", 0x00118394, 0x0000007c
	.section .rom.00118410, "ax"
	.global ColorBuffer_Scale
	.type ColorBuffer_Scale, %function
	.thumb_func
ColorBuffer_Scale:
	.incbin "baserom.gba", 0x00118410, 0x00000328
	.section .rom.00118738, "ax"
	.global DebugParty_LoadPreset
	.type DebugParty_LoadPreset, %function
	.thumb_func
DebugParty_LoadPreset:
	.incbin "baserom.gba", 0x00118738, 0x00000834
	.section .rom.00118f6c, "ax"
	.global Func_08118f6c
	.type Func_08118f6c, %function
	.thumb_func
Func_08118f6c:
	.incbin "baserom.gba", 0x00118f6c, 0x000007e0
	.section .rom.0011974c, "ax"
	.global Func_08119734
	.type Func_08119734, %function
	.thumb_func
Func_08119734:
	.incbin "baserom.gba", 0x0011974c, 0x000000bc
	.section .rom.00119808, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x00119808, 0x00000848
	.section .rom.0011a050, "ax"
	.global BattleParty_PrepareActiveOwners
	.type BattleParty_PrepareActiveOwners, %function
	.thumb_func
BattleParty_PrepareActiveOwners:
	.incbin "baserom.gba", 0x0011a050, 0x00000078
	.section .rom.0011a0c8, "ax"
	.global BattleParty_PrepareReserveOwners
	.type BattleParty_PrepareReserveOwners, %function
	.thumb_func
BattleParty_PrepareReserveOwners:
	.incbin "baserom.gba", 0x0011a0c8, 0x000000d8
	.section .rom.0011a264, "ax"
	.incbin "baserom.gba", 0x0011a264, 0x000000d0
	.section .rom.0011a334, "ax"
	.global BattleParty_ListActorIds
	.type BattleParty_ListActorIds, %function
	.thumb_func
BattleParty_ListActorIds:
	.incbin "baserom.gba", 0x0011a334, 0x00000080
	.section .rom.0011a3b4, "ax"
	.global Func_0811a39c
	.type Func_0811a39c, %function
	.thumb_func
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a3b4, 0x000000b0
	.section .rom.0011a49a, "ax"
	.incbin "baserom.gba", 0x0011a49a, 0x00000002
	.section .rom.0011a49c, "ax"
	.global Func_0811a484
	.type Func_0811a484, %function
	.thumb_func
Func_0811a484:
	.incbin "baserom.gba", 0x0011a49c, 0x0000000c
	.section .rom.0011a4f6, "ax"
	.incbin "baserom.gba", 0x0011a4f6, 0x00000c8e
	.section .rom.0011b198, "ax"
	.incbin "baserom.gba", 0x0011b198, 0x00000524
	.section .rom.0011b73c, "ax"
	.global ReleaseBattleObjectRecords
	.type ReleaseBattleObjectRecords, %function
	.thumb_func
ReleaseBattleObjectRecords:
	.incbin "baserom.gba", 0x0011b73c, 0x00000038
	.section .rom.0011b774, "ax"
	.global Func_0811b75c
	.type Func_0811b75c, %function
	.thumb_func
Func_0811b75c:
	.incbin "baserom.gba", 0x0011b774, 0x00000278
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
	.incbin "baserom.gba", 0x0011bc7c, 0x000000ec
	.section .rom.0011bd68, "ax"
	.global Func_0811bd50
	.type Func_0811bd50, %function
	.thumb_func
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd68, 0x00000060
	.section .rom.0011bdc8, "ax"
	.global GetMotionRecord
	.type GetMotionRecord, %function
	.thumb_func
GetMotionRecord:
	.incbin "baserom.gba", 0x0011bdc8, 0x0000008c
	.section .rom.0011be54, "ax"
	.global GetBattleObjectSlot
	.type GetBattleObjectSlot, %function
	.thumb_func
GetBattleObjectSlot:
	.incbin "baserom.gba", 0x0011be54, 0x0000002c
	.section .rom.0011bede, "ax"
	.incbin "baserom.gba", 0x0011bede, 0x000000da
	.section .rom.0011bfe8, "ax"
	.incbin "baserom.gba", 0x0011bfe8, 0x0000004c
	.section .rom.0011c034, "ax"
	.global Func_0811c01c
	.type Func_0811c01c, %function
	.thumb_func
Func_0811c01c:
	.incbin "baserom.gba", 0x0011c034, 0x00000058
	.section .rom.0011c08c, "ax"
	.global Func_0811c074
	.type Func_0811c074, %function
	.thumb_func
Func_0811c074:
	.incbin "baserom.gba", 0x0011c08c, 0x000000ac
	.section .rom.0011c138, "ax"
	.global Func_0811c120
	.type Func_0811c120, %function
	.thumb_func
Func_0811c120:
	.incbin "baserom.gba", 0x0011c138, 0x000000d8
	.section .rom.0011c228, "ax"
	.incbin "baserom.gba", 0x0011c228, 0x000000a4
	.section .rom.0011c2cc, "ax"
	.global Func_0811c2b4
	.type Func_0811c2b4, %function
	.thumb_func
Func_0811c2b4:
	.incbin "baserom.gba", 0x0011c2cc, 0x00000060
	.section .rom.0011c32c, "ax"
	.global Func_0811c314
	.type Func_0811c314, %function
	.thumb_func
Func_0811c314:
	.incbin "baserom.gba", 0x0011c32c, 0x00000068
	.section .rom.0011c394, "ax"
	.global Func_0811c37c
	.type Func_0811c37c, %function
	.thumb_func
Func_0811c37c:
	.incbin "baserom.gba", 0x0011c394, 0x000002d4
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x00000712
	.section .rom.0011cd94, "ax"
	.global Camera_ConfigureScene
	.type Camera_ConfigureScene, %function
	.thumb_func
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd94, 0x000009a4
	.section .rom.0011d760, "ax"
	.incbin "baserom.gba", 0x0011d760, 0x00000c24
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00001b5e
	.section .rom.0011ff20, "ax"
	.global BattlePresentation_WaitForAdvance
	.type BattlePresentation_WaitForAdvance, %function
	.thumb_func
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x0011ff20, 0x00000158
	.section .rom.00120078, "ax"
	.global Func_08120060
	.type Func_08120060, %function
	.thumb_func
Func_08120060:
	.incbin "baserom.gba", 0x00120078, 0x00000154
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
	.incbin "baserom.gba", 0x001224f0, 0x00000774
	.section .rom.00122c64, "ax"
	.global Func_08122c4c
	.type Func_08122c4c, %function
	.thumb_func
Func_08122c4c:
	.incbin "baserom.gba", 0x00122c64, 0x000008e8
	.section .rom.0012358c, "ax"
	.global Func_08123574
	.type Func_08123574, %function
	.thumb_func
Func_08123574:
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
	.incbin "baserom.gba", 0x001a6000, 0x00002264
	.section .rom.001a8286, "ax"
	.incbin "baserom.gba", 0x001a8286, 0x00003d7a
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
	.incbin "baserom.gba", 0x001b2008, 0x00001ee8
	.section .rom.001b3f18, "ax"
	.incbin "baserom.gba", 0x001b3f18, 0x000040e8
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
	.section .rom.0068a170, "ax"
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a170, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x00692868, 0x00005320
	.section .rom.006a471d, "ax"
	.incbin "baserom.gba", 0x006a471d, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a4720, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a4920, 0x00000880
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a51a0, 0x000008d0
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a5a70, 0x00000860
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a62d0, 0x000006a4
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a6974, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a719c, 0x0000186c
	.section .rom.006a9e30, "ax"
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006a9e30, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b144c, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006cd3d8, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd68c, 0x00000f0c
	.section .rom.006d0f86, "ax"
	.incbin "baserom.gba", 0x006d0f86, 0x00000002
	.section .rom.006d53ca, "ax"
	.incbin "baserom.gba", 0x006d53ca, 0x00000002
	.section .rom.006e5b3a, "ax"
	.incbin "baserom.gba", 0x006e5b3a, 0x00000002
	.section .rom.006e912a, "ax"
	.incbin "baserom.gba", 0x006e912a, 0x00000002
	.section .rom.006f4bce, "ax"
	.incbin "baserom.gba", 0x006f4bce, 0x00000002
	.section .rom.0070527e, "ax"
	.incbin "baserom.gba", 0x0070527e, 0x00000002
	.section .rom.0070cd1e, "ax"
	.incbin "baserom.gba", 0x0070cd1e, 0x00000002
	.section .rom.0071055e, "ax"
	.incbin "baserom.gba", 0x0071055e, 0x00000002
	.section .rom.00719992, "ax"
	.incbin "baserom.gba", 0x00719992, 0x00000002
	.section .rom.00720b3e, "ax"
	.incbin "baserom.gba", 0x00720b3e, 0x00000002
	.section .rom.007289fa, "ax"
	.incbin "baserom.gba", 0x007289fa, 0x00000002
	.section .rom.0072d48a, "ax"
	.incbin "baserom.gba", 0x0072d48a, 0x00000002
	.section .rom.0073173e, "ax"
	.incbin "baserom.gba", 0x0073173e, 0x00000002
	.section .rom.007350be, "ax"
	.incbin "baserom.gba", 0x007350be, 0x00000002
	.section .rom.007418fa, "ax"
	.incbin "baserom.gba", 0x007418fa, 0x00000002
	.section .rom.0074d03a, "ax"
	.incbin "baserom.gba", 0x0074d03a, 0x00000002
	.section .rom.0075040a, "ax"
	.incbin "baserom.gba", 0x0075040a, 0x00000002
	.section .rom.00762326, "ax"
	.incbin "baserom.gba", 0x00762326, 0x00000002
	.section .rom.00765d8e, "ax"
	.incbin "baserom.gba", 0x00765d8e, 0x00000002
	.section .rom.0076977e, "ax"
	.incbin "baserom.gba", 0x0076977e, 0x00000002
	.section .rom.00779ce6, "ax"
	.incbin "baserom.gba", 0x00779ce6, 0x00000002
	.section .rom.0077dc16, "ax"
	.incbin "baserom.gba", 0x0077dc16, 0x00000002
	.section .rom.0078163a, "ax"
	.incbin "baserom.gba", 0x0078163a, 0x00000002
	.section .rom.0078995a, "ax"
	.incbin "baserom.gba", 0x0078995a, 0x00000002
	.section .rom.00791ac6, "ax"
	.incbin "baserom.gba", 0x00791ac6, 0x00000002
	.section .rom.0079ed92, "ax"
	.incbin "baserom.gba", 0x0079ed92, 0x00000002
	.section .rom.007a2c4e, "ax"
	.incbin "baserom.gba", 0x007a2c4e, 0x00000002
	.section .rom.007a6a4a, "ax"
	.incbin "baserom.gba", 0x007a6a4a, 0x00000002
	.section .rom.007aaf1a, "ax"
	.incbin "baserom.gba", 0x007aaf1a, 0x00000002
	.section .rom.007b28f6, "ax"
	.incbin "baserom.gba", 0x007b28f6, 0x00000002
	.section .rom.007b6f22, "ax"
	.incbin "baserom.gba", 0x007b6f22, 0x00000002
	.section .rom.007beb9e, "ax"
	.incbin "baserom.gba", 0x007beb9e, 0x00000002
	.section .rom.007c279a, "ax"
	.incbin "baserom.gba", 0x007c279a, 0x00000002
	.section .rom.007c614a, "ax"
	.incbin "baserom.gba", 0x007c614a, 0x00000002
	.section .rom.007ca59a, "ax"
	.incbin "baserom.gba", 0x007ca59a, 0x00000002
	.section .rom.007ce192, "ax"
	.incbin "baserom.gba", 0x007ce192, 0x00000002
	.section .rom.007d9df2, "ax"
	.incbin "baserom.gba", 0x007d9df2, 0x00000002
	.section .rom.007dda42, "ax"
	.incbin "baserom.gba", 0x007dda42, 0x00000002
	.section .rom.007e5fc2, "ax"
	.incbin "baserom.gba", 0x007e5fc2, 0x00000002
	.section .rom.007f2612, "ax"
	.incbin "baserom.gba", 0x007f2612, 0x00000002
	.section .rom.007f90c6, "ax"
	.incbin "baserom.gba", 0x007f90c6, 0x00000002
	.section .rom.0080238e, "ax"
	.incbin "baserom.gba", 0x0080238e, 0x00000002
	.section .rom.00809db2, "ax"
	.incbin "baserom.gba", 0x00809db2, 0x00000002
	.section .rom.0081fa32, "ax"
	.incbin "baserom.gba", 0x0081fa32, 0x00000002
	.section .rom.00828e76, "ax"
	.incbin "baserom.gba", 0x00828e76, 0x00000002
	.section .rom.00832092, "ax"
	.incbin "baserom.gba", 0x00832092, 0x00000002
	.section .rom.0083f9ba, "ax"
	.incbin "baserom.gba", 0x0083f9ba, 0x00000002
	.section .rom.0084580e, "ax"
	.incbin "baserom.gba", 0x0084580e, 0x00000002
	.section .rom.0084bd89, "ax"
	.incbin "baserom.gba", 0x0084bd89, 0x00000003
	.section .rom.0084d70b, "ax"
	.incbin "baserom.gba", 0x0084d70b, 0x00000001
	.section .rom.00850529, "ax"
	.incbin "baserom.gba", 0x00850529, 0x00000003
	.section .rom.0085443e, "ax"
	.incbin "baserom.gba", 0x0085443e, 0x00000002
	.section .rom.008573ba, "ax"
	.incbin "baserom.gba", 0x008573ba, 0x00000002
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x008573bc, 0x000009bc
	.section .rom.00858396, "ax"
	.incbin "baserom.gba", 0x00858396, 0x00000002
	.section .rom.0085890f, "ax"
	.incbin "baserom.gba", 0x0085890f, 0x00000001
	.section .rom.00858ec1, "ax"
	.incbin "baserom.gba", 0x00858ec1, 0x00000003
	.section .rom.0085921b, "ax"
	.incbin "baserom.gba", 0x0085921b, 0x00000001
	.section .rom.00859579, "ax"
	.incbin "baserom.gba", 0x00859579, 0x00000003
	.section .rom.008598e9, "ax"
	.incbin "baserom.gba", 0x008598e9, 0x00000003
	.section .rom.00859d21, "ax"
	.incbin "baserom.gba", 0x00859d21, 0x00000003
	.section .rom.0085a457, "ax"
	.incbin "baserom.gba", 0x0085a457, 0x00000001
	.section .rom.0085a521, "ax"
	.incbin "baserom.gba", 0x0085a521, 0x00000003
	.section .rom.0085a686, "ax"
	.incbin "baserom.gba", 0x0085a686, 0x00000002
	.section .rom.0085b229, "ax"
	.incbin "baserom.gba", 0x0085b229, 0x00000003
	.section .rom.0085be62, "ax"
	.incbin "baserom.gba", 0x0085be62, 0x00000002
	.section .rom.0085c09b, "ax"
	.incbin "baserom.gba", 0x0085c09b, 0x00000001
	.section .rom.0085e73b, "ax"
	.incbin "baserom.gba", 0x0085e73b, 0x00000001
	.section .rom.0085eeb6, "ax"
	.incbin "baserom.gba", 0x0085eeb6, 0x00000002
	.section .rom.008604fb, "ax"
	.incbin "baserom.gba", 0x008604fb, 0x00000001
	.section .rom.00860a25, "ax"
	.incbin "baserom.gba", 0x00860a25, 0x00000003
	.section .rom.0086122e, "ax"
	.incbin "baserom.gba", 0x0086122e, 0x00000002
	.section .rom.008617a7, "ax"
	.incbin "baserom.gba", 0x008617a7, 0x00000001
	.section .rom.00862ee6, "ax"
	.incbin "baserom.gba", 0x00862ee6, 0x00000002
	.section .rom.00865daf, "ax"
	.incbin "baserom.gba", 0x00865daf, 0x00000001
	.section .rom.00866061, "ax"
	.incbin "baserom.gba", 0x00866061, 0x00000003
	.section .rom.00867443, "ax"
	.incbin "baserom.gba", 0x00867443, 0x00000001
	.section .rom.0086bc7f, "ax"
	.incbin "baserom.gba", 0x0086bc7f, 0x00000001
	.section .rom.0086f425, "ax"
	.incbin "baserom.gba", 0x0086f425, 0x00000003
	.section .rom.0087114a, "ax"
	.incbin "baserom.gba", 0x0087114a, 0x00000002
	.section .rom.00873049, "ax"
	.incbin "baserom.gba", 0x00873049, 0x00000003
	.section .rom.008749f7, "ax"
	.incbin "baserom.gba", 0x008749f7, 0x00000001
	.section .rom.00875f6f, "ax"
	.incbin "baserom.gba", 0x00875f6f, 0x00000001
	.section .rom.00879ba2, "ax"
	.incbin "baserom.gba", 0x00879ba2, 0x00000002
	.section .rom.0087a700, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a700, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087c350, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c790, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c9dc, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087cb74, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087d3a0, 0x00000e98
	.section .rom.0087e86f, "ax"
	.incbin "baserom.gba", 0x0087e86f, 0x00000001
	.section .rom.008809d9, "ax"
	.incbin "baserom.gba", 0x008809d9, 0x00000003
	.section .rom.00880ae4, "ax"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x00880ae4, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x00880d30, 0x00000184
	.section .rom.0088176d, "ax"
	.incbin "baserom.gba", 0x0088176d, 0x00000003
	.section .rom.00883351, "ax"
	.incbin "baserom.gba", 0x00883351, 0x00000003
	.section .rom.00884eae, "ax"
	.incbin "baserom.gba", 0x00884eae, 0x00000002
	.section .rom.008852ed, "ax"
	.incbin "baserom.gba", 0x008852ed, 0x00000003
	.section .rom.00885702, "ax"
	.incbin "baserom.gba", 0x00885702, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00885704, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x00885a3c, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x00886b08, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x00886df0, 0x00000154
	.section .rom.0088944f, "ax"
	.incbin "baserom.gba", 0x0088944f, 0x00000001
	.section .rom.00889e0b, "ax"
	.incbin "baserom.gba", 0x00889e0b, 0x00000001
	.section .rom.0088af4e, "ax"
	.incbin "baserom.gba", 0x0088af4e, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088af50, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088bbcc, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088bbf8, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088bc24, 0x000007c0
	.section .rom.0088d95f, "ax"
	.incbin "baserom.gba", 0x0088d95f, 0x00000001
	.section .rom.0088dc89, "ax"
	.incbin "baserom.gba", 0x0088dc89, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088dc8c, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088e1b4, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e6e4, 0x000005f0
	.section .rom.0088f45d, "ax"
	.incbin "baserom.gba", 0x0088f45d, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088f460, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f660, 0x00000420
	.section .rom.008900a9, "ax"
	.incbin "baserom.gba", 0x008900a9, 0x00000003
	.section .rom.008904b3, "ax"
	.incbin "baserom.gba", 0x008904b3, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x008904b4, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x00890710, 0x0000057c
	.section .rom.00890f35, "ax"
	.incbin "baserom.gba", 0x00890f35, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890f38, 0x0000095c
	.section .rom.00891f51, "ax"
	.incbin "baserom.gba", 0x00891f51, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891f54, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894a04, 0x000011cc
	.section .rom.0089622f, "ax"
	.incbin "baserom.gba", 0x0089622f, 0x00000001
	.section .rom.00897269, "ax"
	.incbin "baserom.gba", 0x00897269, 0x00000003
	.section .rom.008978bf, "ax"
	.incbin "baserom.gba", 0x008978bf, 0x00000001
	.section .rom.00897f3d, "ax"
	.incbin "baserom.gba", 0x00897f3d, 0x00000003
	.section .rom.0089855d, "ax"
	.incbin "baserom.gba", 0x0089855d, 0x00000003
	.section .rom.0089992a, "ax"
	.incbin "baserom.gba", 0x0089992a, 0x00000002
	.section .rom.0089a96e, "ax"
	.incbin "baserom.gba", 0x0089a96e, 0x00000002
	.section .rom.0089b3f7, "ax"
	.incbin "baserom.gba", 0x0089b3f7, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089b3f8, 0x000002cc
	.section .rom.0089c3fd, "ax"
	.incbin "baserom.gba", 0x0089c3fd, 0x00000003
	.section .rom.0089d639, "ax"
	.incbin "baserom.gba", 0x0089d639, 0x00000003
	.section .rom.0089df83, "ax"
	.incbin "baserom.gba", 0x0089df83, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089df84, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e5e0, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089eb0c, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a0dc8, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a255c, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2c40, 0x00001f4c
	.section .rom.008a50c0, "ax"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a50c0, 0x000011d8
	.section .rom.008a6786, "ax"
	.incbin "baserom.gba", 0x008a6786, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a6788, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6dd0, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a79f4, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a7db8, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7f80, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a84cc, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a8818, 0x0000076c
	.section .rom.008a95c7, "ax"
	.incbin "baserom.gba", 0x008a95c7, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a95c8, 0x000000a8
	.section .rom.008a9986, "ax"
	.incbin "baserom.gba", 0x008a9986, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a9988, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa330, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa628, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008ab158, 0x00000100
	.section .rom.008abec9, "ax"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
	.section .rom.008ac711, "ax"
	.incbin "baserom.gba", 0x008ac711, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac714, 0x00001200
	.section .rom.008ae7d3, "ax"
	.incbin "baserom.gba", 0x008ae7d3, 0x00000001
	.section .rom.008af2af, "ax"
	.incbin "baserom.gba", 0x008af2af, 0x00000001
	.section .rom.008af972, "ax"
	.incbin "baserom.gba", 0x008af972, 0x00000002
	.section .rom.008afc59, "ax"
	.incbin "baserom.gba", 0x008afc59, 0x00000003
	.section .rom.008b0fbf, "ax"
	.incbin "baserom.gba", 0x008b0fbf, 0x00000001
	.section .rom.008b13a9, "ax"
	.incbin "baserom.gba", 0x008b13a9, 0x00000003
	.section .rom.008b177b, "ax"
	.incbin "baserom.gba", 0x008b177b, 0x00000001
	.section .rom.008b201b, "ax"
	.incbin "baserom.gba", 0x008b201b, 0x00000001
	.section .rom.008b24be, "ax"
	.incbin "baserom.gba", 0x008b24be, 0x00000002
	.section .rom.008b3677, "ax"
	.incbin "baserom.gba", 0x008b3677, 0x00000001
	.section .rom.008b3ad0, "ax"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3ad0, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b4b3c, 0x00000fc8
	.section .rom.008b5d5e, "ax"
	.incbin "baserom.gba", 0x008b5d5e, 0x00000002
	.section .rom.008b77b9, "ax"
	.incbin "baserom.gba", 0x008b77b9, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b77bc, 0x000001b4
	.section .rom.008b7d05, "ax"
	.incbin "baserom.gba", 0x008b7d05, 0x00000003
	.section .rom.008b9a8e, "ax"
	.incbin "baserom.gba", 0x008b9a8e, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9a90, 0x00000278
	.section .rom.008ba1ce, "ax"
	.incbin "baserom.gba", 0x008ba1ce, 0x00000002
	.section .rom.008bbda3, "ax"
	.incbin "baserom.gba", 0x008bbda3, 0x00000001
	.section .rom.008bd9a1, "ax"
	.incbin "baserom.gba", 0x008bd9a1, 0x00000003
	.section .rom.008bdbc2, "ax"
	.incbin "baserom.gba", 0x008bdbc2, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bdbc4, 0x0000043c
	.section .rom.008be111, "ax"
	.incbin "baserom.gba", 0x008be111, 0x00000003
	.section .rom.008be6f3, "ax"
	.incbin "baserom.gba", 0x008be6f3, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be6f4, 0x000004a0
	.section .rom.008bf8a6, "ax"
	.incbin "baserom.gba", 0x008bf8a6, 0x00000002
	.section .rom.008bfde8, "ax"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bfde8, 0x00000840
	.section .rom.008c09b7, "ax"
	.incbin "baserom.gba", 0x008c09b7, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c09b8, 0x00001258
	.section .rom.008c26fe, "ax"
	.incbin "baserom.gba", 0x008c26fe, 0x00000002
	.section .rom.008c34d5, "ax"
	.incbin "baserom.gba", 0x008c34d5, 0x00000003
	.section .rom.008c3d02, "ax"
	.incbin "baserom.gba", 0x008c3d02, 0x00000002
	.section .rom.008c40f4, "ax"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c40f4, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c42b0, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c446c, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4dac, 0x00000418
	.section .rom.008c5b35, "ax"
	.incbin "baserom.gba", 0x008c5b35, 0x00000003
	.section .rom.008c5ee3, "ax"
	.incbin "baserom.gba", 0x008c5ee3, 0x00000001
	.section .rom.008c7e87, "ax"
	.incbin "baserom.gba", 0x008c7e87, 0x00000001
	.section .rom.008c8c23, "ax"
	.incbin "baserom.gba", 0x008c8c23, 0x00000001
	.section .rom.008c8e3f, "ax"
	.incbin "baserom.gba", 0x008c8e3f, 0x00000001
	.section .rom.008c913b, "ax"
	.incbin "baserom.gba", 0x008c913b, 0x00000001
	.section .rom.008c94dc, "ax"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c94dc, 0x000016b0
	.section .rom.008cb2e9, "ax"
	.incbin "baserom.gba", 0x008cb2e9, 0x00000003
	.section .rom.008cc0b7, "ax"
	.incbin "baserom.gba", 0x008cc0b7, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cc0b8, 0x00000c74
	.section .rom.008cd321, "ax"
	.incbin "baserom.gba", 0x008cd321, 0x00000003
	.section .rom.008cd87b, "ax"
	.incbin "baserom.gba", 0x008cd87b, 0x00000001
	.section .rom.008cf165, "ax"
	.incbin "baserom.gba", 0x008cf165, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008cf168, 0x000008a0
	.section .rom.008cfde3, "ax"
	.incbin "baserom.gba", 0x008cfde3, 0x00000001
	.section .rom.008d006d, "ax"
	.incbin "baserom.gba", 0x008d006d, 0x00000003
	.section .rom.008d0413, "ax"
	.incbin "baserom.gba", 0x008d0413, 0x00000001
	.section .rom.008d066e, "ax"
	.incbin "baserom.gba", 0x008d066e, 0x00000002
	.section .rom.008d0a26, "ax"
	.incbin "baserom.gba", 0x008d0a26, 0x00000002
	.section .rom.008d1f0f, "ax"
	.incbin "baserom.gba", 0x008d1f0f, 0x00000001
	.section .rom.008d34d2, "ax"
	.incbin "baserom.gba", 0x008d34d2, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d34d4, 0x00000a6c
	.section .rom.008d4d16, "ax"
	.incbin "baserom.gba", 0x008d4d16, 0x00000002
	.section .rom.008d5099, "ax"
	.incbin "baserom.gba", 0x008d5099, 0x00000003
	.section .rom.008d5d49, "ax"
	.incbin "baserom.gba", 0x008d5d49, 0x00000003
	.section .rom.008d6891, "ax"
	.incbin "baserom.gba", 0x008d6891, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6894, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d6a2c, 0x0000088c
	.section .rom.008d7e86, "ax"
	.incbin "baserom.gba", 0x008d7e86, 0x00000002
	.section .rom.008d839e, "ax"
	.incbin "baserom.gba", 0x008d839e, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d83a0, 0x00000624
	.section .rom.008d8de5, "ax"
	.incbin "baserom.gba", 0x008d8de5, 0x00000003
	.section .rom.008d907f, "ax"
	.incbin "baserom.gba", 0x008d907f, 0x00000001
	.section .rom.008da580, "ax"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da580, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008daa1c, 0x0000198c
	.section .rom.008dc792, "ax"
	.incbin "baserom.gba", 0x008dc792, 0x00000002
	.section .rom.008de707, "ax"
	.incbin "baserom.gba", 0x008de707, 0x00000001
	.section .rom.008debd7, "ax"
	.incbin "baserom.gba", 0x008debd7, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008debd8, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008df26c, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008dfca0, 0x00000bfc
	.section .rom.008e1071, "ax"
	.incbin "baserom.gba", 0x008e1071, 0x00000003
	.section .rom.008e1b42, "ax"
	.incbin "baserom.gba", 0x008e1b42, 0x00000002
	.section .rom.008e267f, "ax"
	.incbin "baserom.gba", 0x008e267f, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2680, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2cc0, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e4248, 0x00000064
	.section .rom.008e4617, "ax"
	.incbin "baserom.gba", 0x008e4617, 0x00000001
	.section .rom.008e4bb5, "ax"
	.incbin "baserom.gba", 0x008e4bb5, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e4bb8, 0x00000204
	.section .rom.008e50f5, "ax"
	.incbin "baserom.gba", 0x008e50f5, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e50f8, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e6110, 0x0000166c
	.section .rom.008e971a, "ax"
	.incbin "baserom.gba", 0x008e971a, 0x00000002
	.section .rom.008e9ebd, "ax"
	.incbin "baserom.gba", 0x008e9ebd, 0x00000003
	.section .rom.008eaed4, "ax"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eaed4, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eb250, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb680, 0x000010cc
	.section .rom.008ec7d0, "ax"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec7d0, 0x000004e8
	.section .rom.008ecdc0, "ax"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ecdc0, 0x000006b8
	.section .rom.008edd8a, "ax"
	.incbin "baserom.gba", 0x008edd8a, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008edd8c, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef8c0, 0x00001050
	.section .rom.008f19b5, "ax"
	.incbin "baserom.gba", 0x008f19b5, 0x00000003
	.section .rom.008f1bb4, "ax"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f1bb4, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x0093341c, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c7e0, 0x00000028
	.section .rom.0093c9f5, "ax"
	.incbin "baserom.gba", 0x0093c9f5, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c9f8, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093cb4c, 0x000004b8
	.section .rom.0093e76e, "ax"
	.incbin "baserom.gba", 0x0093e76e, 0x00000002
	.section .rom.0093fca5, "ax"
	.incbin "baserom.gba", 0x0093fca5, 0x00000003
	.section .rom.00941afb, "ax"
	.incbin "baserom.gba", 0x00941afb, 0x00000001
	.section .rom.00943c0f, "ax"
	.incbin "baserom.gba", 0x00943c0f, 0x00000001
	.section .rom.00943de8, "ax"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x00943de8, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943fcc, 0x000003c0
	.section .rom.00946a07, "ax"
	.incbin "baserom.gba", 0x00946a07, 0x00000001
	.section .rom.0094740f, "ax"
	.incbin "baserom.gba", 0x0094740f, 0x00000001
	.section .rom.0094a12e, "ax"
	.incbin "baserom.gba", 0x0094a12e, 0x00000002
	.section .rom.0094a300, "ax"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x0094a300, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x0094a308, 0x00000460
	.section .rom.0094bd5a, "ax"
	.incbin "baserom.gba", 0x0094bd5a, 0x00000002
	.section .rom.0094d1ca, "ax"
	.incbin "baserom.gba", 0x0094d1ca, 0x00000002
	.section .rom.0094dabe, "ax"
	.incbin "baserom.gba", 0x0094dabe, 0x00000002
	.section .rom.0094e3e5, "ax"
	.incbin "baserom.gba", 0x0094e3e5, 0x00000003
	.section .rom.0094f2d6, "ax"
	.incbin "baserom.gba", 0x0094f2d6, 0x00000002
	.section .rom.0094fec3, "ax"
	.incbin "baserom.gba", 0x0094fec3, 0x00000001
	.section .rom.0095009e, "ax"
	.incbin "baserom.gba", 0x0095009e, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x009500a0, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x00950290, 0x000002d8
	.section .rom.00950dfe, "ax"
	.incbin "baserom.gba", 0x00950dfe, 0x00000002
	.section .rom.00953239, "ax"
	.incbin "baserom.gba", 0x00953239, 0x00000003
	.section .rom.0095380b, "ax"
	.incbin "baserom.gba", 0x0095380b, 0x00000001
	.section .rom.00953cab, "ax"
	.incbin "baserom.gba", 0x00953cab, 0x00000001
	.section .rom.00953ffe, "ax"
	.incbin "baserom.gba", 0x00953ffe, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00954000, 0x000002f8
	.section .rom.009543d1, "ax"
	.incbin "baserom.gba", 0x009543d1, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x009543d4, 0x000002f0
	.section .rom.00954756, "ax"
	.incbin "baserom.gba", 0x00954756, 0x00000002
	.section .rom.00955309, "ax"
	.incbin "baserom.gba", 0x00955309, 0x00000003
	.section .rom.00955fb6, "ax"
	.incbin "baserom.gba", 0x00955fb6, 0x00000002
	.section .rom.00956aba, "ax"
	.incbin "baserom.gba", 0x00956aba, 0x00000002
	.section .rom.009579f9, "ax"
	.incbin "baserom.gba", 0x009579f9, 0x00000003
	.section .rom.00958235, "ax"
	.incbin "baserom.gba", 0x00958235, 0x00000003
	.section .rom.009596da, "ax"
	.incbin "baserom.gba", 0x009596da, 0x00000002
	.section .rom.0095a192, "ax"
	.incbin "baserom.gba", 0x0095a192, 0x00000002
	.section .rom.0095a4bd, "ax"
	.incbin "baserom.gba", 0x0095a4bd, 0x00000003
	.section .rom.0095bb28, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095bb28, 0x00000400
	.section .rom.009682e2, "ax"
	.incbin "baserom.gba", 0x009682e2, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x009682e4, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x009683e4, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009688ac, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x00968b14, 0x000001c8
	.section .rom.00969143, "ax"
	.incbin "baserom.gba", 0x00969143, 0x00000001
	.section .rom.0096934f, "ax"
	.incbin "baserom.gba", 0x0096934f, 0x00000001
	.section .rom.00969553, "ax"
	.incbin "baserom.gba", 0x00969553, 0x00000001
	.section .rom.009695e2, "ax"
	.incbin "baserom.gba", 0x009695e2, 0x00000002
	.section .rom.00969ac4, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x00969ac4, 0x00000070
	.section .rom.00969b8f, "ax"
	.incbin "baserom.gba", 0x00969b8f, 0x00000001
	.section .rom.00969bc2, "ax"
	.incbin "baserom.gba", 0x00969bc2, 0x00000002
	.section .rom.00969d8e, "ax"
	.incbin "baserom.gba", 0x00969d8e, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969d90, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969fdc, 0x000000e4
	.section .rom.0096a179, "ax"
	.incbin "baserom.gba", 0x0096a179, 0x00000003
	.section .rom.0096a34e, "ax"
	.incbin "baserom.gba", 0x0096a34e, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x0096a350, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a3e8, 0x00000024
	.section .rom.0096a55e, "ax"
	.incbin "baserom.gba", 0x0096a55e, 0x00000002
	.section .rom.0096a641, "ax"
	.incbin "baserom.gba", 0x0096a641, 0x00000003
	.section .rom.0096a711, "ax"
	.incbin "baserom.gba", 0x0096a711, 0x00000003
	.section .rom.0096a837, "ax"
	.incbin "baserom.gba", 0x0096a837, 0x00000001
	.section .rom.0096a866, "ax"
	.incbin "baserom.gba", 0x0096a866, 0x00000002
	.section .rom.0096aaca, "ax"
	.incbin "baserom.gba", 0x0096aaca, 0x00000002
	.section .rom.0096ab67, "ax"
	.incbin "baserom.gba", 0x0096ab67, 0x00000001
	.section .rom.0096abcc, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096abcc, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096afcc, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096b07c, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b5dc, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b650, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b698, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b6e8, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b738, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b790, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b7e8, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b828, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b86c, 0x00000054
	.section .rom.00970af7, "ax"
	.incbin "baserom.gba", 0x00970af7, 0x00000001
	.section .rom.00973a42, "ax"
	.incbin "baserom.gba", 0x00973a42, 0x00000002
	.section .rom.00977665, "ax"
	.incbin "baserom.gba", 0x00977665, 0x00000003
	.section .rom.009798b1, "ax"
	.incbin "baserom.gba", 0x009798b1, 0x00000003
	.section .rom.0097ee4d, "ax"
	.incbin "baserom.gba", 0x0097ee4d, 0x00000003
	.section .rom.009826c7, "ax"
	.incbin "baserom.gba", 0x009826c7, 0x00000001
	.section .rom.00988531, "ax"
	.incbin "baserom.gba", 0x00988531, 0x00000003
	.section .rom.00989605, "ax"
	.incbin "baserom.gba", 0x00989605, 0x00000003
	.section .rom.0098a671, "ax"
	.incbin "baserom.gba", 0x0098a671, 0x00000003
	.section .rom.0098fccb, "ax"
	.incbin "baserom.gba", 0x0098fccb, 0x00000001
	.section .rom.00992e0d, "ax"
	.incbin "baserom.gba", 0x00992e0d, 0x00000003
	.section .rom.00994ead, "ax"
	.incbin "baserom.gba", 0x00994ead, 0x00000003
	.section .rom.009971cd, "ax"
	.incbin "baserom.gba", 0x009971cd, 0x00000003
	.section .rom.00999e46, "ax"
	.incbin "baserom.gba", 0x00999e46, 0x00000002
	.section .rom.0099bd41, "ax"
	.incbin "baserom.gba", 0x0099bd41, 0x00000003
	.section .rom.009a4285, "ax"
	.incbin "baserom.gba", 0x009a4285, 0x00000003
	.section .rom.009acf07, "ax"
	.incbin "baserom.gba", 0x009acf07, 0x00000001
	.section .rom.009afa81, "ax"
	.incbin "baserom.gba", 0x009afa81, 0x00000003
	.section .rom.009b64be, "ax"
	.incbin "baserom.gba", 0x009b64be, 0x00000002
	.section .rom.009b8fb2, "ax"
	.incbin "baserom.gba", 0x009b8fb2, 0x00000002
	.section .rom.009baaa2, "ax"
	.incbin "baserom.gba", 0x009baaa2, 0x00000002
	.section .rom.009bd3b8, "ax"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bd3b8, 0x00003440
	.section .rom.009c519e, "ax"
	.incbin "baserom.gba", 0x009c519e, 0x00000002
	.section .rom.009c613a, "ax"
	.incbin "baserom.gba", 0x009c613a, 0x00000002
	.section .rom.009c8865, "ax"
	.incbin "baserom.gba", 0x009c8865, 0x00000003
	.section .rom.009c9f7a, "ax"
	.incbin "baserom.gba", 0x009c9f7a, 0x00000002
	.section .rom.009d2793, "ax"
	.incbin "baserom.gba", 0x009d2793, 0x00000001
	.section .rom.009d6a5b, "ax"
	.incbin "baserom.gba", 0x009d6a5b, 0x00000001
	.section .rom.009de295, "ax"
	.incbin "baserom.gba", 0x009de295, 0x00000003
	.section .rom.009dfa7a, "ax"
	.incbin "baserom.gba", 0x009dfa7a, 0x00000002
	.section .rom.009e2811, "ax"
	.incbin "baserom.gba", 0x009e2811, 0x00000003
	.section .rom.009e385b, "ax"
	.incbin "baserom.gba", 0x009e385b, 0x00000001
	.section .rom.009ebdeb, "ax"
	.incbin "baserom.gba", 0x009ebdeb, 0x00000001
	.section .rom.009edbe5, "ax"
	.incbin "baserom.gba", 0x009edbe5, 0x00000003
	.section .rom.009eefab, "ax"
	.incbin "baserom.gba", 0x009eefab, 0x00000001
	.section .rom.009f5ead, "ax"
	.incbin "baserom.gba", 0x009f5ead, 0x00000003
	.section .rom.009f81ae, "ax"
	.incbin "baserom.gba", 0x009f81ae, 0x00000002
	.section .rom.009fd3fb, "ax"
	.incbin "baserom.gba", 0x009fd3fb, 0x00000001
	.section .rom.00a027fa, "ax"
	.incbin "baserom.gba", 0x00a027fa, 0x00000002
	.section .rom.00a049ae, "ax"
	.incbin "baserom.gba", 0x00a049ae, 0x00000002
	.section .rom.00a08b15, "ax"
	.incbin "baserom.gba", 0x00a08b15, 0x00000003
	.section .rom.00a0bd3a, "ax"
	.incbin "baserom.gba", 0x00a0bd3a, 0x00000002
	.section .rom.00a0d43e, "ax"
	.incbin "baserom.gba", 0x00a0d43e, 0x00000002
	.section .rom.00a14026, "ax"
	.incbin "baserom.gba", 0x00a14026, 0x00000002
	.section .rom.00a20606, "ax"
	.incbin "baserom.gba", 0x00a20606, 0x00000002
	.section .rom.00a2369a, "ax"
	.incbin "baserom.gba", 0x00a2369a, 0x00000002
	.section .rom.00a25f05, "ax"
	.incbin "baserom.gba", 0x00a25f05, 0x00000003
	.section .rom.00a279f6, "ax"
	.incbin "baserom.gba", 0x00a279f6, 0x00000002
	.section .rom.00a2880e, "ax"
	.incbin "baserom.gba", 0x00a2880e, 0x00000002
	.section .rom.00a294f9, "ax"
	.incbin "baserom.gba", 0x00a294f9, 0x00000003
	.section .rom.00a329b6, "ax"
	.incbin "baserom.gba", 0x00a329b6, 0x00000002
	.section .rom.00a336c1, "ax"
	.incbin "baserom.gba", 0x00a336c1, 0x00000003
	.section .rom.00a34925, "ax"
	.incbin "baserom.gba", 0x00a34925, 0x00000003
	.section .rom.00a35857, "ax"
	.incbin "baserom.gba", 0x00a35857, 0x00000001
	.section .rom.00a364c7, "ax"
	.incbin "baserom.gba", 0x00a364c7, 0x00000001
	.section .rom.00a370df, "ax"
	.incbin "baserom.gba", 0x00a370df, 0x00000001
	.section .rom.00a37853, "ax"
	.incbin "baserom.gba", 0x00a37853, 0x00000001
	.section .rom.00a390e3, "ax"
	.incbin "baserom.gba", 0x00a390e3, 0x00000001
	.section .rom.00a39ab1, "ax"
	.incbin "baserom.gba", 0x00a39ab1, 0x00000003
	.section .rom.00a3a6cf, "ax"
	.incbin "baserom.gba", 0x00a3a6cf, 0x00000001
	.section .rom.00a3cd83, "ax"
	.incbin "baserom.gba", 0x00a3cd83, 0x00000001
	.section .rom.00a3f496, "ax"
	.incbin "baserom.gba", 0x00a3f496, 0x00000002
	.section .rom.00a443eb, "ax"
	.incbin "baserom.gba", 0x00a443eb, 0x00000001
	.section .rom.00a48715, "ax"
	.incbin "baserom.gba", 0x00a48715, 0x00000003
	.section .rom.00a49302, "ax"
	.incbin "baserom.gba", 0x00a49302, 0x00000002
	.section .rom.00a4deb7, "ax"
	.incbin "baserom.gba", 0x00a4deb7, 0x00000001
	.section .rom.00a50221, "ax"
	.incbin "baserom.gba", 0x00a50221, 0x00000003
	.section .rom.00a51bc3, "ax"
	.incbin "baserom.gba", 0x00a51bc3, 0x00000001
	.section .rom.00a56485, "ax"
	.incbin "baserom.gba", 0x00a56485, 0x00000003
	.section .rom.00a59895, "ax"
	.incbin "baserom.gba", 0x00a59895, 0x00000003
	.section .rom.00a5d95d, "ax"
	.incbin "baserom.gba", 0x00a5d95d, 0x00000003
	.section .rom.00a61026, "ax"
	.incbin "baserom.gba", 0x00a61026, 0x00000002
	.section .rom.00a68ff7, "ax"
	.incbin "baserom.gba", 0x00a68ff7, 0x00000001
	.section .rom.00a70307, "ax"
	.incbin "baserom.gba", 0x00a70307, 0x00000001
	.section .rom.00a75287, "ax"
	.incbin "baserom.gba", 0x00a75287, 0x00000001
	.section .rom.00a79d09, "ax"
	.incbin "baserom.gba", 0x00a79d09, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a79d0c, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a79d18, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79e68, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79fa8, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a7a0e8, 0x00000140
	.section .rom.00a7b485, "ax"
	.incbin "baserom.gba", 0x00a7b485, 0x00000003
	.section .rom.00a7b656, "ax"
	.incbin "baserom.gba", 0x00a7b656, 0x00000002
	.section .rom.00a7d6f7, "ax"
	.incbin "baserom.gba", 0x00a7d6f7, 0x00000001
	.section .rom.00a7e684, "ax"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e684, 0x000022d8
	.section .rom.00a81bb0, "ax"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a81bb0, 0x0000461c
	.section .rom.00a862d2, "ax"
	.incbin "baserom.gba", 0x00a862d2, 0x00000002
	.section .rom.00a874bd, "ax"
	.incbin "baserom.gba", 0x00a874bd, 0x00000003
	.section .rom.00a89145, "ax"
	.incbin "baserom.gba", 0x00a89145, 0x00000003
	.section .rom.00a89653, "ax"
	.incbin "baserom.gba", 0x00a89653, 0x00000001
	.section .rom.00a89793, "ax"
	.incbin "baserom.gba", 0x00a89793, 0x00000001
	.section .rom.00a8b416, "ax"
	.incbin "baserom.gba", 0x00a8b416, 0x00000002
	.section .rom.00a8ddee, "ax"
	.incbin "baserom.gba", 0x00a8ddee, 0x00000002
	.section .rom.00a905b7, "ax"
	.incbin "baserom.gba", 0x00a905b7, 0x00000001
	.section .rom.00a91ad7, "ax"
	.incbin "baserom.gba", 0x00a91ad7, 0x00000001
	.section .rom.00a9341b, "ax"
	.incbin "baserom.gba", 0x00a9341b, 0x00000001
	.section .rom.00a94ec9, "ax"
	.incbin "baserom.gba", 0x00a94ec9, 0x00000003
	.section .rom.00a9503d, "ax"
	.incbin "baserom.gba", 0x00a9503d, 0x00000003
	.section .rom.00a97e21, "ax"
	.incbin "baserom.gba", 0x00a97e21, 0x00000003
	.section .rom.00a9a6cb, "ax"
	.incbin "baserom.gba", 0x00a9a6cb, 0x00000001
	.section .rom.00a9eb62, "ax"
	.incbin "baserom.gba", 0x00a9eb62, 0x00000002
	.section .rom.00aa1eda, "ax"
	.incbin "baserom.gba", 0x00aa1eda, 0x00000002
	.section .rom.00aa49d7, "ax"
	.incbin "baserom.gba", 0x00aa49d7, 0x00000001
	.section .rom.00aa6ab5, "ax"
	.incbin "baserom.gba", 0x00aa6ab5, 0x00000003
	.section .rom.00aa8d01, "ax"
	.incbin "baserom.gba", 0x00aa8d01, 0x00000003
	.section .rom.00aa8e43, "ax"
	.incbin "baserom.gba", 0x00aa8e43, 0x00000001
	.section .rom.00aaa516, "ax"
	.incbin "baserom.gba", 0x00aaa516, 0x00000002
	.section .rom.00aaa616, "ax"
	.incbin "baserom.gba", 0x00aaa616, 0x00000002
	.section .rom.00aac509, "ax"
	.incbin "baserom.gba", 0x00aac509, 0x00000003
	.section .rom.00aadee1, "ax"
	.incbin "baserom.gba", 0x00aadee1, 0x00000003
	.section .rom.00ab0912, "ax"
	.incbin "baserom.gba", 0x00ab0912, 0x00000002
	.section .rom.00ab5b63, "ax"
	.incbin "baserom.gba", 0x00ab5b63, 0x00000001
	.section .rom.00ab84a7, "ax"
	.incbin "baserom.gba", 0x00ab84a7, 0x00000001
	.section .rom.00ab997a, "ax"
	.incbin "baserom.gba", 0x00ab997a, 0x00000002
	.section .rom.00abe745, "ax"
	.incbin "baserom.gba", 0x00abe745, 0x00000003
	.section .rom.00ac13ef, "ax"
	.incbin "baserom.gba", 0x00ac13ef, 0x00000001
	.section .rom.00ac4627, "ax"
	.incbin "baserom.gba", 0x00ac4627, 0x00000001
	.section .rom.00ac47bf, "ax"
	.incbin "baserom.gba", 0x00ac47bf, 0x00000001
	.section .rom.00ac75bf, "ax"
	.incbin "baserom.gba", 0x00ac75bf, 0x00000001
	.section .rom.00ac8d73, "ax"
	.incbin "baserom.gba", 0x00ac8d73, 0x00000001
	.section .rom.00ac9d0e, "ax"
	.incbin "baserom.gba", 0x00ac9d0e, 0x00000002
	.section .rom.00acc36f, "ax"
	.incbin "baserom.gba", 0x00acc36f, 0x00000001
	.section .rom.00acc516, "ax"
	.incbin "baserom.gba", 0x00acc516, 0x00000002
	.section .rom.00acf4cf, "ax"
	.incbin "baserom.gba", 0x00acf4cf, 0x00000001
	.section .rom.00ad0175, "ax"
	.incbin "baserom.gba", 0x00ad0175, 0x00000003
	.section .rom.00ad1745, "ax"
	.incbin "baserom.gba", 0x00ad1745, 0x00000003
	.section .rom.00ad9711, "ax"
	.incbin "baserom.gba", 0x00ad9711, 0x00000003
	.section .rom.00adb1f7, "ax"
	.incbin "baserom.gba", 0x00adb1f7, 0x00000001
	.section .rom.00adc6e1, "ax"
	.incbin "baserom.gba", 0x00adc6e1, 0x00000003
	.section .rom.00ae290b, "ax"
	.incbin "baserom.gba", 0x00ae290b, 0x00000001
	.section .rom.00ae2ddd, "ax"
	.incbin "baserom.gba", 0x00ae2ddd, 0x00000003
	.section .rom.00ae3f25, "ax"
	.incbin "baserom.gba", 0x00ae3f25, 0x00000003
	.section .rom.00ae40d6, "ax"
	.incbin "baserom.gba", 0x00ae40d6, 0x00000002
	.section .rom.00aefdc3, "ax"
	.incbin "baserom.gba", 0x00aefdc3, 0x00000001
	.section .rom.00aefee3, "ax"
	.incbin "baserom.gba", 0x00aefee3, 0x00000001
	.section .rom.00af2283, "ax"
	.incbin "baserom.gba", 0x00af2283, 0x00000001
	.section .rom.00af34cf, "ax"
	.incbin "baserom.gba", 0x00af34cf, 0x00000001
	.section .rom.00af585f, "ax"
	.incbin "baserom.gba", 0x00af585f, 0x00000001
	.section .rom.00af715d, "ax"
	.incbin "baserom.gba", 0x00af715d, 0x00000003
	.section .rom.00af814e, "ax"
	.incbin "baserom.gba", 0x00af814e, 0x00000002
	.section .rom.00afa6ea, "ax"
	.incbin "baserom.gba", 0x00afa6ea, 0x00000002
	.section .rom.00afed12, "ax"
	.incbin "baserom.gba", 0x00afed12, 0x00000002
	.section .rom.00aff3d7, "ax"
	.incbin "baserom.gba", 0x00aff3d7, 0x00000001
	.section .rom.00b01e6d, "ax"
	.incbin "baserom.gba", 0x00b01e6d, 0x00000003
	.section .rom.00b01fbe, "ax"
	.incbin "baserom.gba", 0x00b01fbe, 0x00000002
	.section .rom.00b043ce, "ax"
	.incbin "baserom.gba", 0x00b043ce, 0x00000002
	.section .rom.00b0654f, "ax"
	.incbin "baserom.gba", 0x00b0654f, 0x00000001
	.section .rom.00b0668f, "ax"
	.incbin "baserom.gba", 0x00b0668f, 0x00000001
	.section .rom.00b0912e, "ax"
	.incbin "baserom.gba", 0x00b0912e, 0x00000002
	.section .rom.00b0927f, "ax"
	.incbin "baserom.gba", 0x00b0927f, 0x00000001
	.section .rom.00b0b68e, "ax"
	.incbin "baserom.gba", 0x00b0b68e, 0x00000002
	.section .rom.00b0d80f, "ax"
	.incbin "baserom.gba", 0x00b0d80f, 0x00000001
	.section .rom.00b0d94f, "ax"
	.incbin "baserom.gba", 0x00b0d94f, 0x00000001
	.section .rom.00b10f7e, "ax"
	.incbin "baserom.gba", 0x00b10f7e, 0x00000002
	.section .rom.00b134e2, "ax"
	.incbin "baserom.gba", 0x00b134e2, 0x00000002
	.section .rom.00b15663, "ax"
	.incbin "baserom.gba", 0x00b15663, 0x00000001
	.section .rom.00b157a3, "ax"
	.incbin "baserom.gba", 0x00b157a3, 0x00000001
	.section .rom.00b16c53, "ax"
	.incbin "baserom.gba", 0x00b16c53, 0x00000001
	.section .rom.00b16d5e, "ax"
	.incbin "baserom.gba", 0x00b16d5e, 0x00000002
	.section .rom.00b188b6, "ax"
	.incbin "baserom.gba", 0x00b188b6, 0x00000002
	.section .rom.00b19f59, "ax"
	.incbin "baserom.gba", 0x00b19f59, 0x00000003
	.section .rom.00b1a11a, "ax"
	.incbin "baserom.gba", 0x00b1a11a, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b1a11c, 0x00000d9c
	.section .rom.00b1cb0e, "ax"
	.incbin "baserom.gba", 0x00b1cb0e, 0x00000002
	.section .rom.00b1e365, "ax"
	.incbin "baserom.gba", 0x00b1e365, 0x00000003
	.section .rom.00b1e526, "ax"
	.incbin "baserom.gba", 0x00b1e526, 0x00000002
	.section .rom.00b227a1, "ax"
	.incbin "baserom.gba", 0x00b227a1, 0x00000003
	.section .rom.00b22962, "ax"
	.incbin "baserom.gba", 0x00b22962, 0x00000002
	.section .rom.00b233d9, "ax"
	.incbin "baserom.gba", 0x00b233d9, 0x00000003
	.section .rom.00b234e1, "ax"
	.incbin "baserom.gba", 0x00b234e1, 0x00000003
	.section .rom.00b251a2, "ax"
	.incbin "baserom.gba", 0x00b251a2, 0x00000002
	.section .rom.00b2692f, "ax"
	.incbin "baserom.gba", 0x00b2692f, 0x00000001
	.section .rom.00b26be5, "ax"
	.incbin "baserom.gba", 0x00b26be5, 0x00000003
	.section .rom.00b286f5, "ax"
	.incbin "baserom.gba", 0x00b286f5, 0x00000003
	.section .rom.00b2af82, "ax"
	.incbin "baserom.gba", 0x00b2af82, 0x00000002
	.section .rom.00b2d772, "ax"
	.incbin "baserom.gba", 0x00b2d772, 0x00000002
	.section .rom.00b2e712, "ax"
	.incbin "baserom.gba", 0x00b2e712, 0x00000002
	.section .rom.00b30865, "ax"
	.incbin "baserom.gba", 0x00b30865, 0x00000003
	.section .rom.00b33ef2, "ax"
	.incbin "baserom.gba", 0x00b33ef2, 0x00000002
	.section .rom.00b35c87, "ax"
	.incbin "baserom.gba", 0x00b35c87, 0x00000001
	.section .rom.00b37aa5, "ax"
	.incbin "baserom.gba", 0x00b37aa5, 0x00000003
	.section .rom.00b39581, "ax"
	.incbin "baserom.gba", 0x00b39581, 0x00000003
	.section .rom.00b3ab6a, "ax"
	.incbin "baserom.gba", 0x00b3ab6a, 0x00000002
	.section .rom.00b3d152, "ax"
	.incbin "baserom.gba", 0x00b3d152, 0x00000002
	.section .rom.00b3d2cb, "ax"
	.incbin "baserom.gba", 0x00b3d2cb, 0x00000001
	.section .rom.00b3e78a, "ax"
	.incbin "baserom.gba", 0x00b3e78a, 0x00000002
	.section .rom.00b3ff8e, "ax"
	.incbin "baserom.gba", 0x00b3ff8e, 0x00000002
	.section .rom.00b41906, "ax"
	.incbin "baserom.gba", 0x00b41906, 0x00000002
	.section .rom.00b42826, "ax"
	.incbin "baserom.gba", 0x00b42826, 0x00000002
	.section .rom.00b442d2, "ax"
	.incbin "baserom.gba", 0x00b442d2, 0x00000002
	.section .rom.00b443df, "ax"
	.incbin "baserom.gba", 0x00b443df, 0x00000001
	.section .rom.00b4650f, "ax"
	.incbin "baserom.gba", 0x00b4650f, 0x00000001
	.section .rom.00b480e5, "ax"
	.incbin "baserom.gba", 0x00b480e5, 0x00000003
	.section .rom.00b486f7, "ax"
	.incbin "baserom.gba", 0x00b486f7, 0x00000001
	.section .rom.00b48837, "ax"
	.incbin "baserom.gba", 0x00b48837, 0x00000001
	.section .rom.00b4af37, "ax"
	.incbin "baserom.gba", 0x00b4af37, 0x00000001
	.section .rom.00b4c796, "ax"
	.incbin "baserom.gba", 0x00b4c796, 0x00000002
	.section .rom.00b4cda7, "ax"
	.incbin "baserom.gba", 0x00b4cda7, 0x00000001
	.section .rom.00b4cee7, "ax"
	.incbin "baserom.gba", 0x00b4cee7, 0x00000001
	.section .rom.00b4dd62, "ax"
	.incbin "baserom.gba", 0x00b4dd62, 0x00000002
	.section .rom.00b4de79, "ax"
	.incbin "baserom.gba", 0x00b4de79, 0x00000003
	.section .rom.00b5014f, "ax"
	.incbin "baserom.gba", 0x00b5014f, 0x00000001
	.section .rom.00b51da2, "ax"
	.incbin "baserom.gba", 0x00b51da2, 0x00000002
	.section .rom.00b523e7, "ax"
	.incbin "baserom.gba", 0x00b523e7, 0x00000001
	.section .rom.00b52527, "ax"
	.incbin "baserom.gba", 0x00b52527, 0x00000001
	.section .rom.00b5312d, "ax"
	.incbin "baserom.gba", 0x00b5312d, 0x00000003
	.section .rom.00b556df, "ax"
	.incbin "baserom.gba", 0x00b556df, 0x00000001
	.section .rom.00b5739e, "ax"
	.incbin "baserom.gba", 0x00b5739e, 0x00000002
	.section .rom.00b58e6a, "ax"
	.incbin "baserom.gba", 0x00b58e6a, 0x00000002
	.section .rom.00b59f3e, "ax"
	.incbin "baserom.gba", 0x00b59f3e, 0x00000002
	.section .rom.00b5a0a5, "ax"
	.incbin "baserom.gba", 0x00b5a0a5, 0x00000003
	.section .rom.00b5b2be, "ax"
	.incbin "baserom.gba", 0x00b5b2be, 0x00000002
	.section .rom.00b5be9d, "ax"
	.incbin "baserom.gba", 0x00b5be9d, 0x00000003
	.section .rom.00b5c00a, "ax"
	.incbin "baserom.gba", 0x00b5c00a, 0x00000002
	.section .rom.00b5ee5b, "ax"
	.incbin "baserom.gba", 0x00b5ee5b, 0x00000001
	.section .rom.00b6161a, "ax"
	.incbin "baserom.gba", 0x00b6161a, 0x00000002
	.section .rom.00b676cb, "ax"
	.incbin "baserom.gba", 0x00b676cb, 0x00000001
	.section .rom.00b691fb, "ax"
	.incbin "baserom.gba", 0x00b691fb, 0x00000001
	.section .rom.00b6b532, "ax"
	.incbin "baserom.gba", 0x00b6b532, 0x00000002
	.section .rom.00b6c6e3, "ax"
	.incbin "baserom.gba", 0x00b6c6e3, 0x00000001
	.section .rom.00b6e005, "ax"
	.incbin "baserom.gba", 0x00b6e005, 0x00000003
	.section .rom.00b6fe91, "ax"
	.incbin "baserom.gba", 0x00b6fe91, 0x00000003
	.section .rom.00b72072, "ax"
	.incbin "baserom.gba", 0x00b72072, 0x00000002
	.section .rom.00b75a39, "ax"
	.incbin "baserom.gba", 0x00b75a39, 0x00000003
	.section .rom.00b75b25, "ax"
	.incbin "baserom.gba", 0x00b75b25, 0x00000003
	.section .rom.00b78d5e, "ax"
	.incbin "baserom.gba", 0x00b78d5e, 0x00000002
	.section .rom.00b793d5, "ax"
	.incbin "baserom.gba", 0x00b793d5, 0x00000003
	.section .rom.00b7d197, "ax"
	.incbin "baserom.gba", 0x00b7d197, 0x00000001
	.section .rom.00b7f429, "ax"
	.incbin "baserom.gba", 0x00b7f429, 0x00000003
	.section .rom.00b80e46, "ax"
	.incbin "baserom.gba", 0x00b80e46, 0x00000002
	.section .rom.00b81f53, "ax"
	.incbin "baserom.gba", 0x00b81f53, 0x00000001
	.section .rom.00b850f2, "ax"
	.incbin "baserom.gba", 0x00b850f2, 0x00000002
	.section .rom.00b862ea, "ax"
	.incbin "baserom.gba", 0x00b862ea, 0x00000002
	.section .rom.00b871ad, "ax"
	.incbin "baserom.gba", 0x00b871ad, 0x00000003
	.section .rom.00b8924e, "ax"
	.incbin "baserom.gba", 0x00b8924e, 0x00000002
	.section .rom.00b8aca3, "ax"
	.incbin "baserom.gba", 0x00b8aca3, 0x00000001
	.section .rom.00b8c16e, "ax"
	.incbin "baserom.gba", 0x00b8c16e, 0x00000002
	.section .rom.00b8e372, "ax"
	.incbin "baserom.gba", 0x00b8e372, 0x00000002
	.section .rom.00b8e467, "ax"
	.incbin "baserom.gba", 0x00b8e467, 0x00000001
	.section .rom.00b8fa3a, "ax"
	.incbin "baserom.gba", 0x00b8fa3a, 0x00000002
	.section .rom.00b8fc59, "ax"
	.incbin "baserom.gba", 0x00b8fc59, 0x00000003
	.section .rom.00b91d3d, "ax"
	.incbin "baserom.gba", 0x00b91d3d, 0x00000003
	.section .rom.00b91f02, "ax"
	.incbin "baserom.gba", 0x00b91f02, 0x00000002
	.section .rom.00b93fc5, "ax"
	.incbin "baserom.gba", 0x00b93fc5, 0x00000003
	.section .rom.00b940f5, "ax"
	.incbin "baserom.gba", 0x00b940f5, 0x00000003
	.section .rom.00b96bae, "ax"
	.incbin "baserom.gba", 0x00b96bae, 0x00000002
	.section .rom.00b98706, "ax"
	.incbin "baserom.gba", 0x00b98706, 0x00000002
	.section .rom.00b9964d, "ax"
	.incbin "baserom.gba", 0x00b9964d, 0x00000003
	.section .rom.00b9b056, "ax"
	.incbin "baserom.gba", 0x00b9b056, 0x00000002
	.section .rom.00baadab, "ax"
	.incbin "baserom.gba", 0x00baadab, 0x00000001
	.section .rom.00baaefb, "ax"
	.incbin "baserom.gba", 0x00baaefb, 0x00000001
	.section .rom.00bada06, "ax"
	.incbin "baserom.gba", 0x00bada06, 0x00000002
	.section .rom.00baf60f, "ax"
	.incbin "baserom.gba", 0x00baf60f, 0x00000001
	.section .rom.00bb1016, "ax"
	.incbin "baserom.gba", 0x00bb1016, 0x00000002
	.section .rom.00bb2d31, "ax"
	.incbin "baserom.gba", 0x00bb2d31, 0x00000003
	.section .rom.00bb2e83, "ax"
	.incbin "baserom.gba", 0x00bb2e83, 0x00000001
	.section .rom.00bb488a, "ax"
	.incbin "baserom.gba", 0x00bb488a, 0x00000002
	.section .rom.00bb8612, "ax"
	.incbin "baserom.gba", 0x00bb8612, 0x00000002
	.section .rom.00bb87a5, "ax"
	.incbin "baserom.gba", 0x00bb87a5, 0x00000003
	.section .rom.00bbd7af, "ax"
	.incbin "baserom.gba", 0x00bbd7af, 0x00000001
	.section .rom.00bbfe31, "ax"
	.incbin "baserom.gba", 0x00bbfe31, 0x00000003
	.section .rom.00bc0167, "ax"
	.incbin "baserom.gba", 0x00bc0167, 0x00000001
	.section .rom.00bc65bd, "ax"
	.incbin "baserom.gba", 0x00bc65bd, 0x00000003
	.section .rom.00bc670a, "ax"
	.incbin "baserom.gba", 0x00bc670a, 0x00000002
	.section .rom.00bc858f, "ax"
	.incbin "baserom.gba", 0x00bc858f, 0x00000001
	.section .rom.00bc9b36, "ax"
	.incbin "baserom.gba", 0x00bc9b36, 0x00000002
	.section .rom.00bcbae1, "ax"
	.incbin "baserom.gba", 0x00bcbae1, 0x00000003
	.section .rom.00bcca52, "ax"
	.incbin "baserom.gba", 0x00bcca52, 0x00000002
	.section .rom.00bd805b, "ax"
	.incbin "baserom.gba", 0x00bd805b, 0x00000001
	.section .rom.00bd8117, "ax"
	.incbin "baserom.gba", 0x00bd8117, 0x00000001
	.section .rom.00bd8e56, "ax"
	.incbin "baserom.gba", 0x00bd8e56, 0x00000002
	.section .rom.00bd9833, "ax"
	.incbin "baserom.gba", 0x00bd9833, 0x00000001
	.section .rom.00bda44b, "ax"
	.incbin "baserom.gba", 0x00bda44b, 0x00000001
	.section .rom.00bdc306, "ax"
	.incbin "baserom.gba", 0x00bdc306, 0x00000002
	.section .rom.00bdc422, "ax"
	.incbin "baserom.gba", 0x00bdc422, 0x00000002
	.section .rom.00bdeca3, "ax"
	.incbin "baserom.gba", 0x00bdeca3, 0x00000001
	.section .rom.00be042e, "ax"
	.incbin "baserom.gba", 0x00be042e, 0x00000002
	.section .rom.00be435e, "ax"
	.incbin "baserom.gba", 0x00be435e, 0x00000002
	.section .rom.00be44a7, "ax"
	.incbin "baserom.gba", 0x00be44a7, 0x00000001
	.section .rom.00be6b57, "ax"
	.incbin "baserom.gba", 0x00be6b57, 0x00000001
	.section .rom.00be92fa, "ax"
	.incbin "baserom.gba", 0x00be92fa, 0x00000002
	.section .rom.00bea085, "ax"
	.incbin "baserom.gba", 0x00bea085, 0x00000003
	.section .rom.00becec6, "ax"
	.incbin "baserom.gba", 0x00becec6, 0x00000002
	.section .rom.00bef4ed, "ax"
	.incbin "baserom.gba", 0x00bef4ed, 0x00000003
	.section .rom.00bf11d9, "ax"
	.incbin "baserom.gba", 0x00bf11d9, 0x00000003
	.section .rom.00bf28d9, "ax"
	.incbin "baserom.gba", 0x00bf28d9, 0x00000003
	.section .rom.00bf3e55, "ax"
	.incbin "baserom.gba", 0x00bf3e55, 0x00000003
	.section .rom.00bf55c7, "ax"
	.incbin "baserom.gba", 0x00bf55c7, 0x00000001
	.section .rom.00bf8252, "ax"
	.incbin "baserom.gba", 0x00bf8252, 0x00000002
	.section .rom.00bf929e, "ax"
	.incbin "baserom.gba", 0x00bf929e, 0x00000002
	.section .rom.00bfa39b, "ax"
	.incbin "baserom.gba", 0x00bfa39b, 0x00000001
	.section .rom.00bfbb6f, "ax"
	.incbin "baserom.gba", 0x00bfbb6f, 0x00000001
	.section .rom.00bfd033, "ax"
	.incbin "baserom.gba", 0x00bfd033, 0x00000001
	.section .rom.00bff566, "ax"
	.incbin "baserom.gba", 0x00bff566, 0x00000002
	.section .rom.00bff69a, "ax"
	.incbin "baserom.gba", 0x00bff69a, 0x00000002
	.section .rom.00c01e06, "ax"
	.incbin "baserom.gba", 0x00c01e06, 0x00000002
	.section .rom.00c03bd9, "ax"
	.incbin "baserom.gba", 0x00c03bd9, 0x00000003
	.section .rom.00c074bd, "ax"
	.incbin "baserom.gba", 0x00c074bd, 0x00000003
	.section .rom.00c09c23, "ax"
	.incbin "baserom.gba", 0x00c09c23, 0x00000001
	.section .rom.00c0b58d, "ax"
	.incbin "baserom.gba", 0x00c0b58d, 0x00000003
	.section .rom.00c0ddb6, "ax"
	.incbin "baserom.gba", 0x00c0ddb6, 0x00000002
	.section .rom.00c12195, "ax"
	.incbin "baserom.gba", 0x00c12195, 0x00000003
	.section .rom.00c1553f, "ax"
	.incbin "baserom.gba", 0x00c1553f, 0x00000001
	.section .rom.00c16332, "ax"
	.incbin "baserom.gba", 0x00c16332, 0x00000002
	.section .rom.00c170aa, "ax"
	.incbin "baserom.gba", 0x00c170aa, 0x00000002
	.section .rom.00c1f1d2, "ax"
	.incbin "baserom.gba", 0x00c1f1d2, 0x00000002
	.section .rom.00c25551, "ax"
	.incbin "baserom.gba", 0x00c25551, 0x00000003
	.section .rom.00c25f57, "ax"
	.incbin "baserom.gba", 0x00c25f57, 0x00000001
	.section .rom.00c27a05, "ax"
	.incbin "baserom.gba", 0x00c27a05, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27a08, 0x00001490
	.section .rom.00c292ff, "ax"
	.incbin "baserom.gba", 0x00c292ff, 0x00000001
	.section .rom.00c294be, "ax"
	.incbin "baserom.gba", 0x00c294be, 0x00000002
	.section .rom.00c2bcc6, "ax"
	.incbin "baserom.gba", 0x00c2bcc6, 0x00000002
	.section .rom.00c2be1e, "ax"
	.incbin "baserom.gba", 0x00c2be1e, 0x00000002
	.section .rom.00c2e765, "ax"
	.incbin "baserom.gba", 0x00c2e765, 0x00000003
	.section .rom.00c30989, "ax"
	.incbin "baserom.gba", 0x00c30989, 0x00000003
	.section .rom.00c31e7e, "ax"
	.incbin "baserom.gba", 0x00c31e7e, 0x00000002
	.section .rom.00c340e1, "ax"
	.incbin "baserom.gba", 0x00c340e1, 0x00000003
	.section .rom.00c3698f, "ax"
	.incbin "baserom.gba", 0x00c3698f, 0x00000001
	.section .rom.00c38922, "ax"
	.incbin "baserom.gba", 0x00c38922, 0x00000002
	.section .rom.00c39943, "ax"
	.incbin "baserom.gba", 0x00c39943, 0x00000001
	.section .rom.00c39a83, "ax"
	.incbin "baserom.gba", 0x00c39a83, 0x00000001
	.section .rom.00c3c6a2, "ax"
	.incbin "baserom.gba", 0x00c3c6a2, 0x00000002
	.section .rom.00c3eb26, "ax"
	.incbin "baserom.gba", 0x00c3eb26, 0x00000002
	.section .rom.00c40c0d, "ax"
	.incbin "baserom.gba", 0x00c40c0d, 0x00000003
	.section .rom.00c42471, "ax"
	.incbin "baserom.gba", 0x00c42471, 0x00000003
	.section .rom.00c449d7, "ax"
	.incbin "baserom.gba", 0x00c449d7, 0x00000001
	.section .rom.00c44b3d, "ax"
	.incbin "baserom.gba", 0x00c44b3d, 0x00000003
	.section .rom.00c47337, "ax"
	.incbin "baserom.gba", 0x00c47337, 0x00000001
	.section .rom.00c4936b, "ax"
	.incbin "baserom.gba", 0x00c4936b, 0x00000001
	.section .rom.00c4af92, "ax"
	.incbin "baserom.gba", 0x00c4af92, 0x00000002
	.section .rom.00c4d9b2, "ax"
	.incbin "baserom.gba", 0x00c4d9b2, 0x00000002
	.section .rom.00c4e51d, "ax"
	.incbin "baserom.gba", 0x00c4e51d, 0x00000003
	.section .rom.00c4e603, "ax"
	.incbin "baserom.gba", 0x00c4e603, 0x00000001
	.section .rom.00c4ff4d, "ax"
	.incbin "baserom.gba", 0x00c4ff4d, 0x00000003
	.section .rom.00c518b3, "ax"
	.incbin "baserom.gba", 0x00c518b3, 0x00000001
	.section .rom.00c52c81, "ax"
	.incbin "baserom.gba", 0x00c52c81, 0x00000003
	.section .rom.00c52d67, "ax"
	.incbin "baserom.gba", 0x00c52d67, 0x00000001
	.section .rom.00c538d5, "ax"
	.incbin "baserom.gba", 0x00c538d5, 0x00000003
	.section .rom.00c539bb, "ax"
	.incbin "baserom.gba", 0x00c539bb, 0x00000001
	.section .rom.00c5471f, "ax"
	.incbin "baserom.gba", 0x00c5471f, 0x00000001
	.section .rom.00c54803, "ax"
	.incbin "baserom.gba", 0x00c54803, 0x00000001
	.section .rom.00c55009, "ax"
	.incbin "baserom.gba", 0x00c55009, 0x00000003
	.section .rom.00c550ef, "ax"
	.incbin "baserom.gba", 0x00c550ef, 0x00000001
	.section .rom.00c55c4f, "ax"
	.incbin "baserom.gba", 0x00c55c4f, 0x00000001
	.section .rom.00c564cd, "ax"
	.incbin "baserom.gba", 0x00c564cd, 0x00000003
	.section .rom.00c565ca, "ax"
	.incbin "baserom.gba", 0x00c565ca, 0x00000002
	.section .rom.00c58423, "ax"
	.incbin "baserom.gba", 0x00c58423, 0x00000001
	.section .rom.00c59231, "ax"
	.incbin "baserom.gba", 0x00c59231, 0x00000003
	.section .rom.00c5a4c5, "ax"
	.incbin "baserom.gba", 0x00c5a4c5, 0x00000003
	.section .rom.00c5b4a1, "ax"
	.incbin "baserom.gba", 0x00c5b4a1, 0x00000003
	.section .rom.00c5d8a7, "ax"
	.incbin "baserom.gba", 0x00c5d8a7, 0x00000001
	.section .rom.00c5d9e7, "ax"
	.incbin "baserom.gba", 0x00c5d9e7, 0x00000001
	.section .rom.00c5e0b5, "ax"
	.incbin "baserom.gba", 0x00c5e0b5, 0x00000003
	.section .rom.00c5e18b, "ax"
	.incbin "baserom.gba", 0x00c5e18b, 0x00000001
	.section .rom.00c5ea01, "ax"
	.incbin "baserom.gba", 0x00c5ea01, 0x00000003
	.section .rom.00c5eeb2, "ax"
	.incbin "baserom.gba", 0x00c5eeb2, 0x00000002
	.section .rom.00c6032d, "ax"
	.incbin "baserom.gba", 0x00c6032d, 0x00000003
	.section .rom.00c60b43, "ax"
	.incbin "baserom.gba", 0x00c60b43, 0x00000001
	.section .rom.00c617d9, "ax"
	.incbin "baserom.gba", 0x00c617d9, 0x00000003
	.section .rom.00c6197e, "ax"
	.incbin "baserom.gba", 0x00c6197e, 0x00000002
	.section .rom.00c64842, "ax"
	.incbin "baserom.gba", 0x00c64842, 0x00000002
	.section .rom.00c661c1, "ax"
	.incbin "baserom.gba", 0x00c661c1, 0x00000003
	.section .rom.00c6720e, "ax"
	.incbin "baserom.gba", 0x00c6720e, 0x00000002
	.section .rom.00c6e953, "ax"
	.incbin "baserom.gba", 0x00c6e953, 0x00000001
	.section .rom.00c76147, "ax"
	.incbin "baserom.gba", 0x00c76147, 0x00000001
	.section .rom.00c7c741, "ax"
	.incbin "baserom.gba", 0x00c7c741, 0x00000003
	.section .rom.00c7f17a, "ax"
	.incbin "baserom.gba", 0x00c7f17a, 0x00000002
	.section .rom.00c81a4b, "ax"
	.incbin "baserom.gba", 0x00c81a4b, 0x00000001
	.section .rom.00c86297, "ax"
	.incbin "baserom.gba", 0x00c86297, 0x00000001
	.section .rom.00c87295, "ax"
	.incbin "baserom.gba", 0x00c87295, 0x00000003
	.section .rom.00c873ae, "ax"
	.incbin "baserom.gba", 0x00c873ae, 0x00000002
	.section .rom.00c88956, "ax"
	.incbin "baserom.gba", 0x00c88956, 0x00000002
	.section .rom.00c8a3b3, "ax"
	.incbin "baserom.gba", 0x00c8a3b3, 0x00000001
	.section .rom.00c8a4ee, "ax"
	.incbin "baserom.gba", 0x00c8a4ee, 0x00000002
	.section .rom.00c8b9f5, "ax"
	.incbin "baserom.gba", 0x00c8b9f5, 0x00000003
	.section .rom.00c8ec7e, "ax"
	.incbin "baserom.gba", 0x00c8ec7e, 0x00000002
	.section .rom.00c900c5, "ax"
	.incbin "baserom.gba", 0x00c900c5, 0x00000003
	.section .rom.00c92dbd, "ax"
	.incbin "baserom.gba", 0x00c92dbd, 0x00000003
	.section .rom.00c951f3, "ax"
	.incbin "baserom.gba", 0x00c951f3, 0x00000001
	.section .rom.00c953cf, "ax"
	.incbin "baserom.gba", 0x00c953cf, 0x00000001
	.section .rom.00c984e3, "ax"
	.incbin "baserom.gba", 0x00c984e3, 0x00000001
	.section .rom.00c99966, "ax"
	.incbin "baserom.gba", 0x00c99966, 0x00000002
	.section .rom.00c9a9b5, "ax"
	.incbin "baserom.gba", 0x00c9a9b5, 0x00000003
	.section .rom.00c9c81a, "ax"
	.incbin "baserom.gba", 0x00c9c81a, 0x00000002
	.section .rom.00c9c9b5, "ax"
	.incbin "baserom.gba", 0x00c9c9b5, 0x00000003
	.section .rom.00c9cb13, "ax"
	.incbin "baserom.gba", 0x00c9cb13, 0x00000001
	.section .rom.00c9da92, "ax"
	.incbin "baserom.gba", 0x00c9da92, 0x00000002
	.section .rom.00c9dbaa, "ax"
	.incbin "baserom.gba", 0x00c9dbaa, 0x00000002
	.section .rom.00c9fc3d, "ax"
	.incbin "baserom.gba", 0x00c9fc3d, 0x00000003
	.section .rom.00ca1abf, "ax"
	.incbin "baserom.gba", 0x00ca1abf, 0x00000001
	.section .rom.00ca3fe5, "ax"
	.incbin "baserom.gba", 0x00ca3fe5, 0x00000003
	.section .rom.00ca40f2, "ax"
	.incbin "baserom.gba", 0x00ca40f2, 0x00000002
	.section .rom.00ca6185, "ax"
	.incbin "baserom.gba", 0x00ca6185, 0x00000003
	.section .rom.00ca965b, "ax"
	.incbin "baserom.gba", 0x00ca965b, 0x00000001
	.section .rom.00caa145, "ax"
	.incbin "baserom.gba", 0x00caa145, 0x00000003
	.section .rom.00caa2b5, "ax"
	.incbin "baserom.gba", 0x00caa2b5, 0x00000003
	.section .rom.00cad1cf, "ax"
	.incbin "baserom.gba", 0x00cad1cf, 0x00000001
	.section .rom.00caf15a, "ax"
	.incbin "baserom.gba", 0x00caf15a, 0x00000002
	.section .rom.00cb0ac5, "ax"
	.incbin "baserom.gba", 0x00cb0ac5, 0x00000003
	.section .rom.00cb65b2, "ax"
	.incbin "baserom.gba", 0x00cb65b2, 0x00000002
	.section .rom.00cb6762, "ax"
	.incbin "baserom.gba", 0x00cb6762, 0x00000002
	.section .rom.00cbb0da, "ax"
	.incbin "baserom.gba", 0x00cbb0da, 0x00000002
	.section .rom.00cbb232, "ax"
	.incbin "baserom.gba", 0x00cbb232, 0x00000002
	.section .rom.00cbe7ff, "ax"
	.incbin "baserom.gba", 0x00cbe7ff, 0x00000001
	.section .rom.00cc0081, "ax"
	.incbin "baserom.gba", 0x00cc0081, 0x00000003
	.section .rom.00cc1da9, "ax"
	.incbin "baserom.gba", 0x00cc1da9, 0x00000003
	.section .rom.00cc54ca, "ax"
	.incbin "baserom.gba", 0x00cc54ca, 0x00000002
	.section .rom.00cca653, "ax"
	.incbin "baserom.gba", 0x00cca653, 0x00000001
	.section .rom.00cccc21, "ax"
	.incbin "baserom.gba", 0x00cccc21, 0x00000003
	.section .rom.00cce5ad, "ax"
	.incbin "baserom.gba", 0x00cce5ad, 0x00000003
	.section .rom.00ccf6da, "ax"
	.incbin "baserom.gba", 0x00ccf6da, 0x00000002
	.section .rom.00cd028a, "ax"
	.incbin "baserom.gba", 0x00cd028a, 0x00000002
	.section .rom.00cd1c52, "ax"
	.incbin "baserom.gba", 0x00cd1c52, 0x00000002
	.section .rom.00cd1d36, "ax"
	.incbin "baserom.gba", 0x00cd1d36, 0x00000002
	.section .rom.00cd4193, "ax"
	.incbin "baserom.gba", 0x00cd4193, 0x00000001
	.section .rom.00cd42d3, "ax"
	.incbin "baserom.gba", 0x00cd42d3, 0x00000001
	.section .rom.00cd7949, "ax"
	.incbin "baserom.gba", 0x00cd7949, 0x00000003
	.section .rom.00cd7aad, "ax"
	.incbin "baserom.gba", 0x00cd7aad, 0x00000003
	.section .rom.00cda18d, "ax"
	.incbin "baserom.gba", 0x00cda18d, 0x00000003
	.section .rom.00cdd03d, "ax"
	.incbin "baserom.gba", 0x00cdd03d, 0x00000003
	.section .rom.00cde5b2, "ax"
	.incbin "baserom.gba", 0x00cde5b2, 0x00000002
	.section .rom.00ce26ae, "ax"
	.incbin "baserom.gba", 0x00ce26ae, 0x00000002
	.section .rom.00ce4933, "ax"
	.incbin "baserom.gba", 0x00ce4933, 0x00000001
	.section .rom.00ce6517, "ax"
	.incbin "baserom.gba", 0x00ce6517, 0x00000001
	.section .rom.00ce820d, "ax"
	.incbin "baserom.gba", 0x00ce820d, 0x00000003
	.section .rom.00cf5831, "ax"
	.incbin "baserom.gba", 0x00cf5831, 0x00000003
	.section .rom.00cf5959, "ax"
	.incbin "baserom.gba", 0x00cf5959, 0x00000003
	.section .rom.00cf7b6a, "ax"
	.incbin "baserom.gba", 0x00cf7b6a, 0x00000002
	.section .rom.00cf7cfb, "ax"
	.incbin "baserom.gba", 0x00cf7cfb, 0x00000001
	.section .rom.00cff1ad, "ax"
	.incbin "baserom.gba", 0x00cff1ad, 0x00000003
	.section .rom.00d01983, "ax"
	.incbin "baserom.gba", 0x00d01983, 0x00000001
	.section .rom.00d040de, "ax"
	.incbin "baserom.gba", 0x00d040de, 0x00000002
	.section .rom.00d04291, "ax"
	.incbin "baserom.gba", 0x00d04291, 0x00000003
	.section .rom.00d05995, "ax"
	.incbin "baserom.gba", 0x00d05995, 0x00000003
	.section .rom.00d079ef, "ax"
	.incbin "baserom.gba", 0x00d079ef, 0x00000001
	.section .rom.00d08d39, "ax"
	.incbin "baserom.gba", 0x00d08d39, 0x00000003
	.section .rom.00d0a783, "ax"
	.incbin "baserom.gba", 0x00d0a783, 0x00000001
	.section .rom.00d0a8a7, "ax"
	.incbin "baserom.gba", 0x00d0a8a7, 0x00000001
	.section .rom.00d0ad79, "ax"
	.incbin "baserom.gba", 0x00d0ad79, 0x00000003
	.section .rom.00d0cf5d, "ax"
	.incbin "baserom.gba", 0x00d0cf5d, 0x00000003
	.section .rom.00d0d431, "ax"
	.incbin "baserom.gba", 0x00d0d431, 0x00000003
	.section .rom.00d0f967, "ax"
	.incbin "baserom.gba", 0x00d0f967, 0x00000001
	.section .rom.00d0faef, "ax"
	.incbin "baserom.gba", 0x00d0faef, 0x00000001
	.section .rom.00d14391, "ax"
	.incbin "baserom.gba", 0x00d14391, 0x00000003
	.section .rom.00d144c7, "ax"
	.incbin "baserom.gba", 0x00d144c7, 0x00000001
	.section .rom.00d16133, "ax"
	.incbin "baserom.gba", 0x00d16133, 0x00000001
	.section .rom.00d172eb, "ax"
	.incbin "baserom.gba", 0x00d172eb, 0x00000001
	.section .rom.00d18039, "ax"
	.incbin "baserom.gba", 0x00d18039, 0x00000003
	.section .rom.00d191c2, "ax"
	.incbin "baserom.gba", 0x00d191c2, 0x00000002
	.section .rom.00d1acbb, "ax"
	.incbin "baserom.gba", 0x00d1acbb, 0x00000001
	.section .rom.00d1c0c7, "ax"
	.incbin "baserom.gba", 0x00d1c0c7, 0x00000001
	.section .rom.00d1c61b, "ax"
	.incbin "baserom.gba", 0x00d1c61b, 0x00000001
	.section .rom.00d1cc55, "ax"
	.incbin "baserom.gba", 0x00d1cc55, 0x00000003
	.section .rom.00d21fa5, "ax"
	.incbin "baserom.gba", 0x00d21fa5, 0x00000003
	.section .rom.00d24606, "ax"
	.incbin "baserom.gba", 0x00d24606, 0x00000002
	.section .rom.00d2479b, "ax"
	.incbin "baserom.gba", 0x00d2479b, 0x00000001
	.section .rom.00d2634f, "ax"
	.incbin "baserom.gba", 0x00d2634f, 0x00000001
	.section .rom.00d264af, "ax"
	.incbin "baserom.gba", 0x00d264af, 0x00000001
	.section .rom.00d28f53, "ax"
	.incbin "baserom.gba", 0x00d28f53, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28f54, 0x000021b0
	.section .rom.00d2cf7a, "ax"
	.incbin "baserom.gba", 0x00d2cf7a, 0x00000002
	.section .rom.00d2d0bb, "ax"
	.incbin "baserom.gba", 0x00d2d0bb, 0x00000001
	.section .rom.00d34033, "ax"
	.incbin "baserom.gba", 0x00d34033, 0x00000001
	.section .rom.00d34171, "ax"
	.incbin "baserom.gba", 0x00d34171, 0x00000003
	.section .rom.00d36145, "ax"
	.incbin "baserom.gba", 0x00d36145, 0x00000003
	.section .rom.00d36239, "ax"
	.incbin "baserom.gba", 0x00d36239, 0x00000003
	.section .rom.00d37319, "ax"
	.incbin "baserom.gba", 0x00d37319, 0x00000003
	.section .rom.00d391f3, "ax"
	.incbin "baserom.gba", 0x00d391f3, 0x00000001
	.section .rom.00d3a1ab, "ax"
	.incbin "baserom.gba", 0x00d3a1ab, 0x00000001
	.section .rom.00d3c0f2, "ax"
	.incbin "baserom.gba", 0x00d3c0f2, 0x00000002
	.section .rom.00d3dcfd, "ax"
	.incbin "baserom.gba", 0x00d3dcfd, 0x00000003
	.section .rom.00d3de49, "ax"
	.incbin "baserom.gba", 0x00d3de49, 0x00000003
	.section .rom.00d3ff07, "ax"
	.incbin "baserom.gba", 0x00d3ff07, 0x00000001
	.section .rom.00d43ff6, "ax"
	.incbin "baserom.gba", 0x00d43ff6, 0x00000002
	.section .rom.00d47da8, "ax"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d47da8, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d49cb4, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4c018, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4e044, 0x0000180c
	.section .rom.00d50f9d, "ax"
	.incbin "baserom.gba", 0x00d50f9d, 0x00000003
	.section .rom.00d510ca, "ax"
	.incbin "baserom.gba", 0x00d510ca, 0x00000002
	.section .rom.00d5803d, "ax"
	.incbin "baserom.gba", 0x00d5803d, 0x00000003
	.section .rom.00d58142, "ax"
	.incbin "baserom.gba", 0x00d58142, 0x00000002
	.section .rom.00d58282, "ax"
	.incbin "baserom.gba", 0x00d58282, 0x00000002
	.section .rom.00d59c4e, "ax"
	.incbin "baserom.gba", 0x00d59c4e, 0x00000002
	.section .rom.00d59d45, "ax"
	.incbin "baserom.gba", 0x00d59d45, 0x00000003
	.section .rom.00d5cb99, "ax"
	.incbin "baserom.gba", 0x00d5cb99, 0x00000003
	.section .rom.00d5cd42, "ax"
	.incbin "baserom.gba", 0x00d5cd42, 0x00000002
	.section .rom.00d5f95e, "ax"
	.incbin "baserom.gba", 0x00d5f95e, 0x00000002
	.section .rom.00d62691, "ax"
	.incbin "baserom.gba", 0x00d62691, 0x00000003
	.section .rom.00d6437b, "ax"
	.incbin "baserom.gba", 0x00d6437b, 0x00000001
	.section .rom.00d67bf5, "ax"
	.incbin "baserom.gba", 0x00d67bf5, 0x00000003
	.section .rom.00d67d4d, "ax"
	.incbin "baserom.gba", 0x00d67d4d, 0x00000003
	.section .rom.00d6a291, "ax"
	.incbin "baserom.gba", 0x00d6a291, 0x00000003
	.section .rom.00d6e156, "ax"
	.incbin "baserom.gba", 0x00d6e156, 0x00000002
	.section .rom.00d70286, "ax"
	.incbin "baserom.gba", 0x00d70286, 0x00000002
	.section .rom.00d74492, "ax"
	.incbin "baserom.gba", 0x00d74492, 0x00000002
	.section .rom.00d74666, "ax"
	.incbin "baserom.gba", 0x00d74666, 0x00000002
	.section .rom.00d766a6, "ax"
	.incbin "baserom.gba", 0x00d766a6, 0x00000002
	.section .rom.00d77a9d, "ax"
	.incbin "baserom.gba", 0x00d77a9d, 0x00000003
	.section .rom.00d785da, "ax"
	.incbin "baserom.gba", 0x00d785da, 0x00000002
	.section .rom.00d79745, "ax"
	.incbin "baserom.gba", 0x00d79745, 0x00000003
	.section .rom.00d7a7f1, "ax"
	.incbin "baserom.gba", 0x00d7a7f1, 0x00000003
	.section .rom.00d7a99d, "ax"
	.incbin "baserom.gba", 0x00d7a99d, 0x00000003
	.section .rom.00d7c4aa, "ax"
	.incbin "baserom.gba", 0x00d7c4aa, 0x00000002
	.section .rom.00d7dec7, "ax"
	.incbin "baserom.gba", 0x00d7dec7, 0x00000001
	.section .rom.00d80399, "ax"
	.incbin "baserom.gba", 0x00d80399, 0x00000003
	.section .rom.00d816fb, "ax"
	.incbin "baserom.gba", 0x00d816fb, 0x00000001
	.section .rom.00d825af, "ax"
	.incbin "baserom.gba", 0x00d825af, 0x00000001
	.section .rom.00d83b9d, "ax"
	.incbin "baserom.gba", 0x00d83b9d, 0x00000003
	.section .rom.00d850e3, "ax"
	.incbin "baserom.gba", 0x00d850e3, 0x00000001
	.section .rom.00d85ead, "ax"
	.incbin "baserom.gba", 0x00d85ead, 0x00000003
	.section .rom.00d8601b, "ax"
	.incbin "baserom.gba", 0x00d8601b, 0x00000001
	.section .rom.00d86fda, "ax"
	.incbin "baserom.gba", 0x00d86fda, 0x00000002
	.section .rom.00d87aa3, "ax"
	.incbin "baserom.gba", 0x00d87aa3, 0x00000001
	.section .rom.00d8863d, "ax"
	.incbin "baserom.gba", 0x00d8863d, 0x00000003
	.section .rom.00d8878d, "ax"
	.incbin "baserom.gba", 0x00d8878d, 0x00000003
	.section .rom.00d8ca3b, "ax"
	.incbin "baserom.gba", 0x00d8ca3b, 0x00000001
	.section .rom.00d8e4d5, "ax"
	.incbin "baserom.gba", 0x00d8e4d5, 0x00000003
	.section .rom.00d8feab, "ax"
	.incbin "baserom.gba", 0x00d8feab, 0x00000001
	.section .rom.00d9002a, "ax"
	.incbin "baserom.gba", 0x00d9002a, 0x00000002
	.section .rom.00d93c5a, "ax"
	.incbin "baserom.gba", 0x00d93c5a, 0x00000002
	.section .rom.00d959b7, "ax"
	.incbin "baserom.gba", 0x00d959b7, 0x00000001
	.section .rom.00d98fe1, "ax"
	.incbin "baserom.gba", 0x00d98fe1, 0x00000003
	.section .rom.00d99132, "ax"
	.incbin "baserom.gba", 0x00d99132, 0x00000002
	.section .rom.00d9ed2e, "ax"
	.incbin "baserom.gba", 0x00d9ed2e, 0x00000002
	.section .rom.00da166b, "ax"
	.incbin "baserom.gba", 0x00da166b, 0x00000001
	.section .rom.00da2fad, "ax"
	.incbin "baserom.gba", 0x00da2fad, 0x00000003
	.section .rom.00da3116, "ax"
	.incbin "baserom.gba", 0x00da3116, 0x00000002
	.section .rom.00da9f15, "ax"
	.incbin "baserom.gba", 0x00da9f15, 0x00000003
	.section .rom.00daa4f3, "ax"
	.incbin "baserom.gba", 0x00daa4f3, 0x00000001
	.section .rom.00dab3fb, "ax"
	.incbin "baserom.gba", 0x00dab3fb, 0x00000001
	.section .rom.00dadc43, "ax"
	.incbin "baserom.gba", 0x00dadc43, 0x00000001
	.section .rom.00daf262, "ax"
	.incbin "baserom.gba", 0x00daf262, 0x00000002
	.section .rom.00db0bc3, "ax"
	.incbin "baserom.gba", 0x00db0bc3, 0x00000001
	.section .rom.00db39ab, "ax"
	.incbin "baserom.gba", 0x00db39ab, 0x00000001
	.section .rom.00db3b26, "ax"
	.incbin "baserom.gba", 0x00db3b26, 0x00000002
	.section .rom.00dc2cef, "ax"
	.incbin "baserom.gba", 0x00dc2cef, 0x00000001
	.section .rom.00dc2e57, "ax"
	.incbin "baserom.gba", 0x00dc2e57, 0x00000001
	.section .rom.00dc547a, "ax"
	.incbin "baserom.gba", 0x00dc547a, 0x00000002
	.section .rom.00dc55bd, "ax"
	.incbin "baserom.gba", 0x00dc55bd, 0x00000003
	.section .rom.00dc6e7a, "ax"
	.incbin "baserom.gba", 0x00dc6e7a, 0x00000002
	.section .rom.00dc9dde, "ax"
	.incbin "baserom.gba", 0x00dc9dde, 0x00000002
	.section .rom.00dcc776, "ax"
	.incbin "baserom.gba", 0x00dcc776, 0x00000002
	.section .rom.00dd10ce, "ax"
	.incbin "baserom.gba", 0x00dd10ce, 0x00000002
	.section .rom.00dd36f7, "ax"
	.incbin "baserom.gba", 0x00dd36f7, 0x00000001
	.section .rom.00dd38cb, "ax"
	.incbin "baserom.gba", 0x00dd38cb, 0x00000001
	.section .rom.00dd6bea, "ax"
	.incbin "baserom.gba", 0x00dd6bea, 0x00000002
	.section .rom.00dd6dc1, "ax"
	.incbin "baserom.gba", 0x00dd6dc1, 0x00000003
	.section .rom.00dda135, "ax"
	.incbin "baserom.gba", 0x00dda135, 0x00000003
	.section .rom.00ddb889, "ax"
	.incbin "baserom.gba", 0x00ddb889, 0x00000003
	.section .rom.00ddc817, "ax"
	.incbin "baserom.gba", 0x00ddc817, 0x00000001
	.section .rom.00dde18d, "ax"
	.incbin "baserom.gba", 0x00dde18d, 0x00000003
	.section .rom.00dde2c7, "ax"
	.incbin "baserom.gba", 0x00dde2c7, 0x00000001
	.section .rom.00ddfe09, "ax"
	.incbin "baserom.gba", 0x00ddfe09, 0x00000003
	.section .rom.00de33e1, "ax"
	.incbin "baserom.gba", 0x00de33e1, 0x00000003
	.section .rom.00de406b, "ax"
	.incbin "baserom.gba", 0x00de406b, 0x00000001
	.section .rom.00de41ab, "ax"
	.incbin "baserom.gba", 0x00de41ab, 0x00000001
	.section .rom.00de5f6f, "ax"
	.incbin "baserom.gba", 0x00de5f6f, 0x00000001
	.section .rom.00de60e2, "ax"
	.incbin "baserom.gba", 0x00de60e2, 0x00000002
	.section .rom.00de854a, "ax"
	.incbin "baserom.gba", 0x00de854a, 0x00000002
	.section .rom.00de9ee6, "ax"
	.incbin "baserom.gba", 0x00de9ee6, 0x00000002
	.section .rom.00debd1b, "ax"
	.incbin "baserom.gba", 0x00debd1b, 0x00000001
	.section .rom.00dec7d2, "ax"
	.incbin "baserom.gba", 0x00dec7d2, 0x00000002
	.section .rom.00dee1ee, "ax"
	.incbin "baserom.gba", 0x00dee1ee, 0x00000002
	.section .rom.00df0eba, "ax"
	.incbin "baserom.gba", 0x00df0eba, 0x00000002
	.section .rom.00df1095, "ax"
	.incbin "baserom.gba", 0x00df1095, 0x00000003
	.section .rom.00df2bcb, "ax"
	.incbin "baserom.gba", 0x00df2bcb, 0x00000001
	.section .rom.00df5c95, "ax"
	.incbin "baserom.gba", 0x00df5c95, 0x00000003
	.section .rom.00df6e51, "ax"
	.incbin "baserom.gba", 0x00df6e51, 0x00000003
	.section .rom.00df7039, "ax"
	.incbin "baserom.gba", 0x00df7039, 0x00000003
	.section .rom.00df71cb, "ax"
	.incbin "baserom.gba", 0x00df71cb, 0x00000001
	.section .rom.00df7f52, "ax"
	.incbin "baserom.gba", 0x00df7f52, 0x00000002
	.section .rom.00df80e7, "ax"
	.incbin "baserom.gba", 0x00df80e7, 0x00000001
	.section .rom.00dfa19b, "ax"
	.incbin "baserom.gba", 0x00dfa19b, 0x00000001
	.section .rom.00e0221e, "ax"
	.incbin "baserom.gba", 0x00e0221e, 0x00000002
	.section .rom.00e0235f, "ax"
	.incbin "baserom.gba", 0x00e0235f, 0x00000001
	.section .rom.00e066e7, "ax"
	.incbin "baserom.gba", 0x00e066e7, 0x00000001
	.section .rom.00e06e06, "ax"
	.incbin "baserom.gba", 0x00e06e06, 0x00000002
	.section .rom.00e081c5, "ax"
	.incbin "baserom.gba", 0x00e081c5, 0x00000003
	.section .rom.00e0c2df, "ax"
	.incbin "baserom.gba", 0x00e0c2df, 0x00000001
	.section .rom.00e0d35f, "ax"
	.incbin "baserom.gba", 0x00e0d35f, 0x00000001
	.section .rom.00e0e3e1, "ax"
	.incbin "baserom.gba", 0x00e0e3e1, 0x00000003
	.section .rom.00e0ef52, "ax"
	.incbin "baserom.gba", 0x00e0ef52, 0x00000002
	.section .rom.00e0fea3, "ax"
	.incbin "baserom.gba", 0x00e0fea3, 0x00000001
	.section .rom.00e10021, "ax"
	.incbin "baserom.gba", 0x00e10021, 0x00000003
	.section .rom.00e1357b, "ax"
	.incbin "baserom.gba", 0x00e1357b, 0x00000001
	.section .rom.00e14bae, "ax"
	.incbin "baserom.gba", 0x00e14bae, 0x00000002
	.section .rom.00e14cef, "ax"
	.incbin "baserom.gba", 0x00e14cef, 0x00000001
	.section .rom.00e1763a, "ax"
	.incbin "baserom.gba", 0x00e1763a, 0x00000002
	.section .rom.00e1ffe2, "ax"
	.incbin "baserom.gba", 0x00e1ffe2, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1ffe4, 0x00000fdc
	.section .rom.00e22b8d, "ax"
	.incbin "baserom.gba", 0x00e22b8d, 0x00000003
	.section .rom.00e24acd, "ax"
	.incbin "baserom.gba", 0x00e24acd, 0x00000003
	.section .rom.00e25f41, "ax"
	.incbin "baserom.gba", 0x00e25f41, 0x00000003
	.section .rom.00e27bfb, "ax"
	.incbin "baserom.gba", 0x00e27bfb, 0x00000001
	.section .rom.00e34caa, "ax"
	.incbin "baserom.gba", 0x00e34caa, 0x00000002
	.section .rom.00e34de9, "ax"
	.incbin "baserom.gba", 0x00e34de9, 0x00000003
	.section .rom.00e36d0e, "ax"
	.incbin "baserom.gba", 0x00e36d0e, 0x00000002
	.section .rom.00e36e37, "ax"
	.incbin "baserom.gba", 0x00e36e37, 0x00000001
	.section .rom.00e39007, "ax"
	.incbin "baserom.gba", 0x00e39007, 0x00000001
	.section .rom.00e3b5d7, "ax"
	.incbin "baserom.gba", 0x00e3b5d7, 0x00000001
	.section .rom.00e3d8e9, "ax"
	.incbin "baserom.gba", 0x00e3d8e9, 0x00000003
	.section .rom.00e3e15d, "ax"
	.incbin "baserom.gba", 0x00e3e15d, 0x00000003
	.section .rom.00e3e29f, "ax"
	.incbin "baserom.gba", 0x00e3e29f, 0x00000001
	.section .rom.00e41355, "ax"
	.incbin "baserom.gba", 0x00e41355, 0x00000003
	.section .rom.00e42a3b, "ax"
	.incbin "baserom.gba", 0x00e42a3b, 0x00000001
	.section .rom.00e44603, "ax"
	.incbin "baserom.gba", 0x00e44603, 0x00000001
	.section .rom.00e4617e, "ax"
	.incbin "baserom.gba", 0x00e4617e, 0x00000002
	.section .rom.00e465c9, "ax"
	.incbin "baserom.gba", 0x00e465c9, 0x00000003
	.section .rom.00e4872a, "ax"
	.incbin "baserom.gba", 0x00e4872a, 0x00000002
	.section .rom.00e48843, "ax"
	.incbin "baserom.gba", 0x00e48843, 0x00000001
	.section .rom.00e4966d, "ax"
	.incbin "baserom.gba", 0x00e4966d, 0x00000003
	.section .rom.00e497cf, "ax"
	.incbin "baserom.gba", 0x00e497cf, 0x00000001
	.section .rom.00e4e037, "ax"
	.incbin "baserom.gba", 0x00e4e037, 0x00000001
	.section .rom.00e506f5, "ax"
	.incbin "baserom.gba", 0x00e506f5, 0x00000003
	.section .rom.00e50cd3, "ax"
	.incbin "baserom.gba", 0x00e50cd3, 0x00000001
	.section .rom.00e52c66, "ax"
	.incbin "baserom.gba", 0x00e52c66, 0x00000002
	.section .rom.00e54446, "ax"
	.incbin "baserom.gba", 0x00e54446, 0x00000002
	.section .rom.00e545aa, "ax"
	.incbin "baserom.gba", 0x00e545aa, 0x00000002
	.section .rom.00e56da2, "ax"
	.incbin "baserom.gba", 0x00e56da2, 0x00000002
	.section .rom.00e56f5b, "ax"
	.incbin "baserom.gba", 0x00e56f5b, 0x00000001
	.section .rom.00e570e3, "ax"
	.incbin "baserom.gba", 0x00e570e3, 0x00000001
	.section .rom.00e58905, "ax"
	.incbin "baserom.gba", 0x00e58905, 0x00000003
	.section .rom.00e594ee, "ax"
	.incbin "baserom.gba", 0x00e594ee, 0x00000002
	.section .rom.00e5aa27, "ax"
	.incbin "baserom.gba", 0x00e5aa27, 0x00000001
	.section .rom.00e5abc2, "ax"
	.incbin "baserom.gba", 0x00e5abc2, 0x00000002
	.section .rom.00e5ce6e, "ax"
	.incbin "baserom.gba", 0x00e5ce6e, 0x00000002
	.section .rom.00e5f035, "ax"
	.incbin "baserom.gba", 0x00e5f035, 0x00000003
	.section .rom.00e60733, "ax"
	.incbin "baserom.gba", 0x00e60733, 0x00000001
	.section .rom.00e61257, "ax"
	.incbin "baserom.gba", 0x00e61257, 0x00000001
	.section .rom.00e660fa, "ax"
	.incbin "baserom.gba", 0x00e660fa, 0x00000002
	.section .rom.00e684a1, "ax"
	.incbin "baserom.gba", 0x00e684a1, 0x00000003
	.section .rom.00e68b8a, "ax"
	.incbin "baserom.gba", 0x00e68b8a, 0x00000002
	.section .rom.00e69776, "ax"
	.incbin "baserom.gba", 0x00e69776, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e69778, 0x00000ac4
	.section .rom.00e6b7eb, "ax"
	.incbin "baserom.gba", 0x00e6b7eb, 0x00000001
	.section .rom.00e6b99e, "ax"
	.incbin "baserom.gba", 0x00e6b99e, 0x00000002
	.section .rom.00e6dbdd, "ax"
	.incbin "baserom.gba", 0x00e6dbdd, 0x00000003
	.section .rom.00e6e703, "ax"
	.incbin "baserom.gba", 0x00e6e703, 0x00000001
	.section .rom.00e79194, "ax"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e79194, 0x000007d0
	.section .rom.00e7c62d, "ax"
	.incbin "baserom.gba", 0x00e7c62d, 0x00000003
	.section .rom.00e7c7a7, "ax"
	.incbin "baserom.gba", 0x00e7c7a7, 0x00000001
	.section .rom.00e7c91e, "ax"
	.incbin "baserom.gba", 0x00e7c91e, 0x00000002
	.section .rom.00e7cab6, "ax"
	.incbin "baserom.gba", 0x00e7cab6, 0x00000002
	.section .rom.00e7cc47, "ax"
	.incbin "baserom.gba", 0x00e7cc47, 0x00000001
	.section .rom.00e7eef3, "ax"
	.incbin "baserom.gba", 0x00e7eef3, 0x00000001
	.section .rom.00e7f00b, "ax"
	.incbin "baserom.gba", 0x00e7f00b, 0x00000001
	.section .rom.00e80e0d, "ax"
	.incbin "baserom.gba", 0x00e80e0d, 0x00000003
	.section .rom.00e828d2, "ax"
	.incbin "baserom.gba", 0x00e828d2, 0x00000002
	.section .rom.00e82ffa, "ax"
	.incbin "baserom.gba", 0x00e82ffa, 0x00000002
	.section .rom.00e843f6, "ax"
	.incbin "baserom.gba", 0x00e843f6, 0x00000002
	.section .rom.00e852ca, "ax"
	.incbin "baserom.gba", 0x00e852ca, 0x00000002
	.section .rom.00e8540e, "ax"
	.incbin "baserom.gba", 0x00e8540e, 0x00000002
	.section .rom.00e877bf, "ax"
	.incbin "baserom.gba", 0x00e877bf, 0x00000001
	.section .rom.00e89fed, "ax"
	.incbin "baserom.gba", 0x00e89fed, 0x00000003
	.section .rom.00e8a3ea, "ax"
	.incbin "baserom.gba", 0x00e8a3ea, 0x00000002
	.section .rom.00e8b88a, "ax"
	.incbin "baserom.gba", 0x00e8b88a, 0x00000002
	.section .rom.00e8b9de, "ax"
	.incbin "baserom.gba", 0x00e8b9de, 0x00000002
	.section .rom.00e8f32b, "ax"
	.incbin "baserom.gba", 0x00e8f32b, 0x00000001
	.section .rom.00e90796, "ax"
	.incbin "baserom.gba", 0x00e90796, 0x00000002
	.section .rom.00e91c6f, "ax"
	.incbin "baserom.gba", 0x00e91c6f, 0x00000001
	.section .rom.00e91dc2, "ax"
	.incbin "baserom.gba", 0x00e91dc2, 0x00000002
	.section .rom.00e957cf, "ax"
	.incbin "baserom.gba", 0x00e957cf, 0x00000001
	.section .rom.00e969a7, "ax"
	.incbin "baserom.gba", 0x00e969a7, 0x00000001
	.section .rom.00e981b2, "ax"
	.incbin "baserom.gba", 0x00e981b2, 0x00000002
	.section .rom.00e98302, "ax"
	.incbin "baserom.gba", 0x00e98302, 0x00000002
	.section .rom.00e9aa0f, "ax"
	.incbin "baserom.gba", 0x00e9aa0f, 0x00000001
	.section .rom.00e9ab76, "ax"
	.incbin "baserom.gba", 0x00e9ab76, 0x00000002
	.section .rom.00e9db37, "ax"
	.incbin "baserom.gba", 0x00e9db37, 0x00000001
	.section .rom.00e9eda5, "ax"
	.incbin "baserom.gba", 0x00e9eda5, 0x00000003
	.section .rom.00ea1932, "ax"
	.incbin "baserom.gba", 0x00ea1932, 0x00000002
	.section .rom.00ea2319, "ax"
	.incbin "baserom.gba", 0x00ea2319, 0x00000003
	.section .rom.00ea2cba, "ax"
	.incbin "baserom.gba", 0x00ea2cba, 0x00000002
	.section .rom.00ea2e2e, "ax"
	.incbin "baserom.gba", 0x00ea2e2e, 0x00000002
	.section .rom.00ea493f, "ax"
	.incbin "baserom.gba", 0x00ea493f, 0x00000001
	.section .rom.00ea4a61, "ax"
	.incbin "baserom.gba", 0x00ea4a61, 0x00000003
	.section .rom.00ea755f, "ax"
	.incbin "baserom.gba", 0x00ea755f, 0x00000001
	.section .rom.00ea8356, "ax"
	.incbin "baserom.gba", 0x00ea8356, 0x00000002
	.section .rom.00eaa2f5, "ax"
	.incbin "baserom.gba", 0x00eaa2f5, 0x00000003
	.section .rom.00eab625, "ax"
	.incbin "baserom.gba", 0x00eab625, 0x00000003
	.section .rom.00eab779, "ax"
	.incbin "baserom.gba", 0x00eab779, 0x00000003
	.section .rom.00eac32b, "ax"
	.incbin "baserom.gba", 0x00eac32b, 0x00000001
	.section .rom.00eadb05, "ax"
	.incbin "baserom.gba", 0x00eadb05, 0x00000003
	.section .rom.00eaec65, "ax"
	.incbin "baserom.gba", 0x00eaec65, 0x00000003
	.section .rom.00eafd83, "ax"
	.incbin "baserom.gba", 0x00eafd83, 0x00000001
	.section .rom.00eaff8a, "ax"
	.incbin "baserom.gba", 0x00eaff8a, 0x00000002
	.section .rom.00eb0c7f, "ax"
	.incbin "baserom.gba", 0x00eb0c7f, 0x00000001
	.section .rom.00eb3421, "ax"
	.incbin "baserom.gba", 0x00eb3421, 0x00000003
	.section .rom.00eb4b96, "ax"
	.incbin "baserom.gba", 0x00eb4b96, 0x00000002
	.section .rom.00eb5e53, "ax"
	.incbin "baserom.gba", 0x00eb5e53, 0x00000001
	.section .rom.00eb6696, "ax"
	.incbin "baserom.gba", 0x00eb6696, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb6698, 0x0000060c
	.section .rom.00eb721b, "ax"
	.incbin "baserom.gba", 0x00eb721b, 0x00000001
	.section .rom.00eb735b, "ax"
	.incbin "baserom.gba", 0x00eb735b, 0x00000001
	.section .rom.00eb749b, "ax"
	.incbin "baserom.gba", 0x00eb749b, 0x00000001
	.section .rom.00eb7ffd, "ax"
	.incbin "baserom.gba", 0x00eb7ffd, 0x00000003
	.section .rom.00eba7a1, "ax"
	.incbin "baserom.gba", 0x00eba7a1, 0x00000003
	.section .rom.00ebbf16, "ax"
	.incbin "baserom.gba", 0x00ebbf16, 0x00000002
	.section .rom.00ebd1d3, "ax"
	.incbin "baserom.gba", 0x00ebd1d3, 0x00000001
	.section .rom.00ebda16, "ax"
	.incbin "baserom.gba", 0x00ebda16, 0x00000002
	.section .rom.00ebdfb5, "ax"
	.incbin "baserom.gba", 0x00ebdfb5, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdfb8, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdfc4, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebe114, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebe254, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebe394, 0x00000140
	.section .rom.00ebebe9, "ax"
	.incbin "baserom.gba", 0x00ebebe9, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebebec, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebebf8, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebed48, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebee88, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebefc8, 0x00000140
	.section .rom.00ebfcc9, "ax"
	.incbin "baserom.gba", 0x00ebfcc9, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebfccc, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebfcd8, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebfe28, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebff68, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ec00a8, 0x00000140
	.section .rom.00ec07b5, "ax"
	.incbin "baserom.gba", 0x00ec07b5, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec07b8, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec07c4, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec0914, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec0a54, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec0b94, 0x00000140
	.section .rom.00ec1651, "ax"
	.incbin "baserom.gba", 0x00ec1651, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec1654, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec1660, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec17b0, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec18f0, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1a30, 0x00000140
	.section .rom.00ec1f99, "ax"
	.incbin "baserom.gba", 0x00ec1f99, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1f9c, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1fa8, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec20f8, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec2238, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec2378, 0x00000140
	.section .rom.00ec2ae1, "ax"
	.incbin "baserom.gba", 0x00ec2ae1, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec2ae4, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec2af0, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2c40, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec2d80, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2ec0, 0x00000140
	.section .rom.00ec3729, "ax"
	.incbin "baserom.gba", 0x00ec3729, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec372c, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec3738, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec3888, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec39c8, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec3b08, 0x00000140
	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "baserom.gba", 0x00ec3c48, 0x00000490
	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "baserom.gba", 0x00ec40d8, 0x00001d08
	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "baserom.gba", 0x00ec5de0, 0x00003dec
	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "baserom.gba", 0x00ec9bcc, 0x00002df0
	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "baserom.gba", 0x00ecc9bc, 0x000019ec
	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "baserom.gba", 0x00ece3a8, 0x000011b4
	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "baserom.gba", 0x00ecf55c, 0x000007c8
	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "baserom.gba", 0x00ecfd24, 0x00001860
	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "baserom.gba", 0x00ed1584, 0x000009d4
	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "baserom.gba", 0x00ed1f58, 0x00000b34
	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "baserom.gba", 0x00ed2a8c, 0x00003238
	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "baserom.gba", 0x00ed5cc4, 0x000024c0
	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "baserom.gba", 0x00ed8184, 0x00002dac
	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "baserom.gba", 0x00edaf30, 0x00001cf8
	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "baserom.gba", 0x00edcc28, 0x0000114c
	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "baserom.gba", 0x00eddd74, 0x0000160c
	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "baserom.gba", 0x00edf380, 0x000014b0
	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "baserom.gba", 0x00ee0830, 0x00000e44
	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "baserom.gba", 0x00ee1674, 0x00000274
	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "baserom.gba", 0x00ee18e8, 0x000001e4
	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "baserom.gba", 0x00ee1acc, 0x000006e8
	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "baserom.gba", 0x00ee21b4, 0x00000574
	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "baserom.gba", 0x00ee2728, 0x000025a4
	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "baserom.gba", 0x00ee4ccc, 0x00000714
	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "baserom.gba", 0x00ee53e0, 0x00003224
	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "baserom.gba", 0x00ee8604, 0x000014bc
	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "baserom.gba", 0x00ee9ac0, 0x0000294c
	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "baserom.gba", 0x00eec40c, 0x00004158
	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "baserom.gba", 0x00ef0564, 0x00001524
	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "baserom.gba", 0x00ef1a88, 0x00000aa0
	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "baserom.gba", 0x00ef2528, 0x00000bf8
	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "baserom.gba", 0x00ef3120, 0x00002980
	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "baserom.gba", 0x00ef5aa0, 0x00001458
	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "baserom.gba", 0x00ef6ef8, 0x00000dac
	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "baserom.gba", 0x00ef7ca4, 0x00001274
	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "baserom.gba", 0x00ef8f18, 0x000001d8
	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "baserom.gba", 0x00ef90f0, 0x000005a8
	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "baserom.gba", 0x00ef9698, 0x00000a98
	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "baserom.gba", 0x00efa130, 0x00000de4
	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "baserom.gba", 0x00efaf14, 0x00003750
	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "baserom.gba", 0x00efe664, 0x00004050
	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "baserom.gba", 0x00f026b4, 0x000012e8
	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "baserom.gba", 0x00f0399c, 0x00000a68
	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "baserom.gba", 0x00f04404, 0x00002b30
	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "baserom.gba", 0x00f06f34, 0x00000d94
	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "baserom.gba", 0x00f07cc8, 0x000015c8
	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "baserom.gba", 0x00f09290, 0x00000714
	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "baserom.gba", 0x00f099a4, 0x00001eec
	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "baserom.gba", 0x00f0b890, 0x000018e0
	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "baserom.gba", 0x00f0d170, 0x0000010c
	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "baserom.gba", 0x00f0d27c, 0x00001fdc
	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "baserom.gba", 0x00f0f258, 0x00003368
	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "baserom.gba", 0x00f125c0, 0x00001168
	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "baserom.gba", 0x00f13728, 0x00000a84
	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "baserom.gba", 0x00f141ac, 0x00000fc0
	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "baserom.gba", 0x00f1516c, 0x00000d10
	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "baserom.gba", 0x00f15e7c, 0x0000383c
	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "baserom.gba", 0x00f196b8, 0x000014c8
	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "baserom.gba", 0x00f1ab80, 0x0000035c
	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "baserom.gba", 0x00f1aedc, 0x000000d0
	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "baserom.gba", 0x00f1afac, 0x00002000
	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "baserom.gba", 0x00f1cfac, 0x0000219c
	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "baserom.gba", 0x00f1f148, 0x000016e0
	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "baserom.gba", 0x00f20828, 0x000004a0
	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "baserom.gba", 0x00f20cc8, 0x000003e8
	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "baserom.gba", 0x00f210b0, 0x00000c10
	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "baserom.gba", 0x00f21cc0, 0x000012e8
	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "baserom.gba", 0x00f22fa8, 0x00001228
	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "baserom.gba", 0x00f241d0, 0x00001c28
	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "baserom.gba", 0x00f25df8, 0x00000df8
	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "baserom.gba", 0x00f26bf0, 0x0000246c
	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "baserom.gba", 0x00f2905c, 0x00001910
	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "baserom.gba", 0x00f2a96c, 0x00002cc0
	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "baserom.gba", 0x00f2d62c, 0x000039e8
	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "baserom.gba", 0x00f31014, 0x00000be4
	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "baserom.gba", 0x00f31bf8, 0x00002df8
	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "baserom.gba", 0x00f349f0, 0x00000b1c
	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "baserom.gba", 0x00f3550c, 0x00003310
	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "baserom.gba", 0x00f3881c, 0x00004450
	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "baserom.gba", 0x00f3cc6c, 0x00001978
	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "baserom.gba", 0x00f3e5e4, 0x00001af8
	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "baserom.gba", 0x00f400dc, 0x000019ac
	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "baserom.gba", 0x00f41a88, 0x00001efc
	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "baserom.gba", 0x00f43984, 0x00002140
	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "baserom.gba", 0x00f45ac4, 0x00001cb0
	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "baserom.gba", 0x00f47774, 0x00002468
	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "baserom.gba", 0x00f49bdc, 0x00001a84
	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "baserom.gba", 0x00f4b660, 0x000023b0
	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "baserom.gba", 0x00f4da10, 0x0000205c
	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "baserom.gba", 0x00f4fa6c, 0x00001b48
	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "baserom.gba", 0x00f515b4, 0x000032b8
	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "baserom.gba", 0x00f5486c, 0x0000245c
	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "baserom.gba", 0x00f56cc8, 0x000037a4
	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "baserom.gba", 0x00f5a46c, 0x00000fc4
	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "baserom.gba", 0x00f5b430, 0x00000810
	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "baserom.gba", 0x00f5bc40, 0x00002d64
	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "baserom.gba", 0x00f5e9a4, 0x00000d70
	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "baserom.gba", 0x00f5f714, 0x00003c08
	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "baserom.gba", 0x00f6331c, 0x00002f08
	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "baserom.gba", 0x00f66224, 0x00002274
	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "baserom.gba", 0x00f68498, 0x000045ac
	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "baserom.gba", 0x00f6ca44, 0x00003344
	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "baserom.gba", 0x00f6fd88, 0x00000d80
	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "baserom.gba", 0x00f70b08, 0x000028b4
	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "baserom.gba", 0x00f733bc, 0x000015a8
	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "baserom.gba", 0x00f74964, 0x00000430
	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "baserom.gba", 0x00f74d94, 0x00000554
	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "baserom.gba", 0x00f752e8, 0x00000e50
	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "baserom.gba", 0x00f76138, 0x000013f8
	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "baserom.gba", 0x00f77530, 0x000000b0
	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "baserom.gba", 0x00f775e0, 0x00000774
	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "baserom.gba", 0x00f77d54, 0x00000cd8
	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "baserom.gba", 0x00f78a2c, 0x00000518
	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "baserom.gba", 0x00f78f44, 0x000870bc
