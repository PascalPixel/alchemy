@ tla-fr's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.section .rom.000144ec, "ax"
	.global Func_080144c0
	.type Func_080144c0, %function
	.thumb_func
Func_080144c0:
	.incbin "baserom.gba", 0x000144ec, 0x00000044
	.section .rom.00014598, "ax"
	.global Func_0801456c
	.type Func_0801456c, %function
	.thumb_func
Func_0801456c:
	.incbin "baserom.gba", 0x00014598, 0x0000003c
	.section .rom.000145d4, "ax"
	.global Scheduler_AddOrUpdateCallback
	.type Scheduler_AddOrUpdateCallback, %function
	.thumb_func
Scheduler_AddOrUpdateCallback:
	.incbin "baserom.gba", 0x000145d4, 0x0000009c
	.section .rom.00014670, "ax"
	.global Scheduler_RemoveCallback
	.type Scheduler_RemoveCallback, %function
	.thumb_func
Scheduler_RemoveCallback:
	.incbin "baserom.gba", 0x00014670, 0x00000050
	.section .rom.000146c0, "ax"
	.global Func_08014694
	.type Func_08014694, %function
	.thumb_func
Func_08014694:
	.incbin "baserom.gba", 0x000146c0, 0x00000088
	.section .rom.00014748, "ax"
	.global Func_0801471c
	.type Func_0801471c, %function
	.thumb_func
Func_0801471c:
	.incbin "baserom.gba", 0x00014748, 0x00000040
	.section .rom.00014788, "ax"
	.global Func_0801475c
	.type Func_0801475c, %function
	.thumb_func
Func_0801475c:
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
	.global Resource_DecodeByteLzInRam
	.type Resource_DecodeByteLzInRam, %function
	.thumb_func
Resource_DecodeByteLzInRam:
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
	.global SerialRuntime_RemoveIrqHandlers
	.type SerialRuntime_RemoveIrqHandlers, %function
	.thumb_func
SerialRuntime_RemoveIrqHandlers:
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
	.section .rom.00022f64, "ax"
	.global Map_RenderAnimatedTileFrame
	.type Map_RenderAnimatedTileFrame, %function
	.thumb_func
Map_RenderAnimatedTileFrame:
	.incbin "baserom.gba", 0x00022f64, 0x00000084
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
	.incbin "baserom.gba", 0x0002de8c, 0x00000dbc
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
	.incbin "baserom.gba", 0x000396dc, 0x00000974
	.section .rom.0003a050, "ax"
	.global Func_0803a084
	.type Func_0803a084, %function
	.thumb_func
Func_0803a084:
	.incbin "baserom.gba", 0x0003a050, 0x00000334
	.section .rom.0003a384, "ax"
	.global UiWork_IsComplete
	.type UiWork_IsComplete, %function
	.thumb_func
UiWork_IsComplete:
	.incbin "baserom.gba", 0x0003a384, 0x0000002c
	.section .rom.0003a3b0, "ax"
	.global UiWork_IsIdle
	.type UiWork_IsIdle, %function
	.thumb_func
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a3b0, 0x0000014c
	.section .rom.0003a4fc, "ax"
	.global Func_0803a530
	.type Func_0803a530, %function
	.thumb_func
Func_0803a530:
	.incbin "baserom.gba", 0x0003a4fc, 0x0000001c
	.section .rom.0003a518, "ax"
	.global Func_0803a54c
	.type Func_0803a54c, %function
	.thumb_func
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a518, 0x00000094
	.section .rom.0003a5ac, "ax"
	.global Func_0803a5e0
	.type Func_0803a5e0, %function
	.thumb_func
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a5ac, 0x0000002c
	.section .rom.0003a5d8, "ax"
	.global Func_0803a60c
	.type Func_0803a60c, %function
	.thumb_func
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5d8, 0x0000005c
	.section .rom.0003a668, "ax"
	.global UiText_OpenMessageWindow
	.type UiText_OpenMessageWindow, %function
	.thumb_func
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a668, 0x00000110
	.section .rom.0003a778, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.type UiText_ShowPositionedMessageAndWait, %function
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a778, 0x00000320
	.section .rom.0003aa98, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa98, 0x0000058c
	.section .rom.0003b024, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003b024, 0x0000084c
	.section .rom.0003b888, "ax"
	.global UiText_GetResourceDimensions
	.type UiText_GetResourceDimensions, %function
	.thumb_func
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b888, 0x0000004c
	.section .rom.0003b8d4, "ax"
	.global Func_0803b8cc
	.type Func_0803b8cc, %function
	.thumb_func
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8d4, 0x0000004c
	.section .rom.0003b920, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b920, 0x00000734
	.section .rom.0003c054, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c054, 0x00000108
	.section .rom.0003c15c, "ax"
	.global Func_0803c170
	.type Func_0803c170, %function
	.thumb_func
Func_0803c170:
	.incbin "baserom.gba", 0x0003c15c, 0x00000028
	.section .rom.0003c184, "ax"
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c184, 0x000001e0
	.section .rom.0003c364, "ax"
	.global UiWindow_SetTilemapEntry
	.type UiWindow_SetTilemapEntry, %function
	.thumb_func
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c364, 0x00000634
	.section .rom.0003c9a8, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9a8, 0x00000064
	.section .rom.0003ca0c, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca0c, 0x00000144
	.section .rom.0003cb5e, "ax"
	.incbin "baserom.gba", 0x0003cb5e, 0x00000002
	.section .rom.0003cb60, "ax"
	.global Func_0803cb1c
	.type Func_0803cb1c, %function
	.thumb_func
Func_0803cb1c:
	.incbin "baserom.gba", 0x0003cb60, 0x0000018c
	.section .rom.0003ccec, "ax"
	.global Func_0803cca8
	.type Func_0803cca8, %function
	.thumb_func
Func_0803cca8:
	.incbin "baserom.gba", 0x0003ccec, 0x00000028
	.section .rom.0003cd14, "ax"
	.global Func_0803ccd0
	.type Func_0803ccd0, %function
	.thumb_func
Func_0803ccd0:
	.incbin "baserom.gba", 0x0003cd14, 0x0000014c
	.section .rom.0003ce60, "ax"
	.global Func_0803ce1c
	.type Func_0803ce1c, %function
	.thumb_func
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce60, 0x00000048
	.section .rom.0003cea8, "ax"
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cea8, 0x000001bc
	.section .rom.0003d064, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d064, 0x000002d0
	.section .rom.0003d334, "ax"
	.global Localization_LookupEntryId
	.type Localization_LookupEntryId, %function
	.thumb_func
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d334, 0x000000d0
	.section .rom.0003d404, "ax"
	.global Func_0803d3c0
	.type Func_0803d3c0, %function
	.thumb_func
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d404, 0x00000090
	.section .rom.0003d494, "ax"
	.global Func_0803d450
	.type Func_0803d450, %function
	.thumb_func
Func_0803d450:
	.incbin "baserom.gba", 0x0003d494, 0x0000006c
	.section .rom.0003d528, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d528, 0x000000e0
	.section .rom.0003d608, "ax"
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d608, 0x000000bc
	.section .rom.0003d6c4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6c4, 0x0000022c
	.section .rom.0003d96e, "ax"
	.incbin "baserom.gba", 0x0003d96e, 0x00000002
	.section .rom.0003d970, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d970, 0x00000060
	.section .rom.0003d9d0, "ax"
	.global Ability_LoadGlyph
	.type Ability_LoadGlyph, %function
	.thumb_func
Ability_LoadGlyph:
	.incbin "baserom.gba", 0x0003d9d0, 0x00000030
	.section .rom.0003da00, "ax"
	.global Func_0803d9bc
	.type Func_0803d9bc, %function
	.thumb_func
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da00, 0x000000bc
	.section .rom.0003dabc, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.type Ui_PrepareTransferFromTableEntry, %function
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dabc, 0x000001a4
	.section .rom.0003dc60, "ax"
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc60, 0x00000108
	.section .rom.0003dd68, "ax"
	.global Func_0803dd24
	.type Func_0803dd24, %function
	.thumb_func
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd68, 0x00000044
	.section .rom.0003ddac, "ax"
	.global Func_0803dd68
	.type Func_0803dd68, %function
	.thumb_func
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddac, 0x00000030
	.section .rom.0003dddc, "ax"
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003dddc, 0x00000110
	.section .rom.0003deec, "ax"
	.global Func_0803dea8
	.type Func_0803dea8, %function
	.thumb_func
Func_0803dea8:
	.incbin "baserom.gba", 0x0003deec, 0x00000058
	.section .rom.0003df44, "ax"
	.global Func_0803df00
	.type Func_0803df00, %function
	.thumb_func
Func_0803df00:
	.incbin "baserom.gba", 0x0003df44, 0x00000014
	.section .rom.0003df58, "ax"
	.global Resource_ScheduleOwnerReset
	.type Resource_ScheduleOwnerReset, %function
	.thumb_func
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df58, 0x00000694
	.section .rom.0003e71c, "ax"
	.global Func_0803e6d8
	.type Func_0803e6d8, %function
	.thumb_func
Func_0803e6d8:
	.incbin "baserom.gba", 0x0003e71c, 0x0000009c
	.section .rom.0003e7b8, "ax"
	.global Func_0803e774
	.type Func_0803e774, %function
	.thumb_func
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7b8, 0x00000038
	.section .rom.0003e7f0, "ax"
	.global Func_0803e7ac
	.type Func_0803e7ac, %function
	.thumb_func
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e7f0, 0x00000140
	.section .rom.0003e930, "ax"
	.global NodeChain_GetNodeAtCount
	.type NodeChain_GetNodeAtCount, %function
	.thumb_func
NodeChain_GetNodeAtCount:
	.incbin "baserom.gba", 0x0003e930, 0x0000002c
	.section .rom.0003e95c, "ax"
	.global Func_0803e918
	.type Func_0803e918, %function
	.thumb_func
Func_0803e918:
	.incbin "baserom.gba", 0x0003e95c, 0x00000080
	.section .rom.0003e9dc, "ax"
	.global Func_0803e998
	.type Func_0803e998, %function
	.thumb_func
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9dc, 0x0000063c
	.section .rom.0003f048, "ax"
	.incbin "baserom.gba", 0x0003f048, 0x000001d0
	.section .rom.0003f218, "ax"
	.global Resource_LoadByMode
	.type Resource_LoadByMode, %function
	.thumb_func
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f218, 0x00000060
	.section .rom.0003f278, "ax"
	.global Func_0803f234
	.type Func_0803f234, %function
	.thumb_func
Func_0803f234:
	.incbin "baserom.gba", 0x0003f278, 0x000003dc
	.section .rom.0003f654, "ax"
	.global Func_0803f610
	.type Func_0803f610, %function
	.thumb_func
Func_0803f610:
	.incbin "baserom.gba", 0x0003f654, 0x00000004
	.section .rom.0003f658, "ax"
	.global Func_0803f614
	.type Func_0803f614, %function
	.thumb_func
Func_0803f614:
	.incbin "baserom.gba", 0x0003f658, 0x0000000c
	.section .rom.0003f664, "ax"
	.global Func_0803f620
	.type Func_0803f620, %function
	.thumb_func
Func_0803f620:
	.incbin "baserom.gba", 0x0003f664, 0x00000004
	.section .rom.0003f668, "ax"
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f668, 0x00000074
	.section .rom.0003f6dc, "ax"
	.global UiTextResource_SetPosition
	.type UiTextResource_SetPosition, %function
	.thumb_func
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6dc, 0x00000028
	.section .rom.0003f704, "ax"
	.global UiTextResource_Release
	.type UiTextResource_Release, %function
	.thumb_func
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f704, 0x000000b8
	.section .rom.0003f7bc, "ax"
	.global Func_0803f778
	.type Func_0803f778, %function
	.thumb_func
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7bc, 0x000000f4
	.section .rom.0003f8b0, "ax"
	.global Func_0803f86c
	.type Func_0803f86c, %function
	.thumb_func
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8b0, 0x000000d0
	.section .rom.0003f980, "ax"
	.global Func_0803f93c
	.type Func_0803f93c, %function
	.thumb_func
Func_0803f93c:
	.incbin "baserom.gba", 0x0003f980, 0x0000002c
	.section .rom.0003f9ac, "ax"
	.global Func_0803f968
	.type Func_0803f968, %function
	.thumb_func
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9ac, 0x00000058
	.section .rom.0003fa04, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa04, 0x00000728
	.section .rom.0004012c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004012c, 0x00001a8c
	.section .rom.00041bb8, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bb8, 0x000000a4
	.section .rom.00041c5c, "ax"
	.global Func_08041c0c
	.type Func_08041c0c, %function
	.thumb_func
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c5c, 0x00000048
	.section .rom.00041ca4, "ax"
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041ca4, 0x0000031c
	.section .rom.00041fc0, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x00041fc0, 0x00000020
	.section .rom.00041fe0, "ax"
	.global Func_08041f90
	.type Func_08041f90, %function
	.thumb_func
Func_08041f90:
	.incbin "baserom.gba", 0x00041fe0, 0x00000014
	.section .rom.00041ff4, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x00041ff4, 0x0000006c
	.section .rom.00042060, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042060, 0x00000098
	.section .rom.000420f8, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x000420f8, 0x00000054
	.section .rom.0004214c, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x0004214c, 0x0000008c
	.section .rom.000421d8, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421d8, 0x0000005c
	.section .rom.00042234, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042234, 0x00000030
	.section .rom.00042264, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042264, 0x00000030
	.section .rom.00042294, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x00042294, 0x000000d0
	.section .rom.00042364, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042364, 0x00000088
	.section .rom.000424a0, "ax"
	.global Func_08042450
	.type Func_08042450, %function
	.thumb_func
Func_08042450:
	.incbin "baserom.gba", 0x000424a0, 0x000000b8
	.section .rom.00042558, "ax"
	.global Func_08042508
	.type Func_08042508, %function
	.thumb_func
Func_08042508:
	.incbin "baserom.gba", 0x00042558, 0x00000064
	.section .rom.000425ca, "ax"
	.incbin "baserom.gba", 0x000425ca, 0x00000002
	.section .rom.000425cc, "ax"
	.global Func_0804257c
	.type Func_0804257c, %function
	.thumb_func
Func_0804257c:
	.incbin "baserom.gba", 0x000425cc, 0x0000000c
	.section .rom.000425d8, "ax"
	.global Func_08042588
	.type Func_08042588, %function
	.thumb_func
Func_08042588:
	.incbin "baserom.gba", 0x000425d8, 0x00000074
	.section .rom.00042680, "ax"
	.incbin "baserom.gba", 0x00042680, 0x00000060
	.section .rom.000426e0, "ax"
	.global Func_08042690
	.type Func_08042690, %function
	.thumb_func
Func_08042690:
	.incbin "baserom.gba", 0x000426e0, 0x000002ec
	.section .rom.000429cc, "ax"
	.global Func_0804297c
	.type Func_0804297c, %function
	.thumb_func
Func_0804297c:
	.incbin "baserom.gba", 0x000429cc, 0x00000430
	.section .rom.00042dfc, "ax"
	.global Func_08042dac
	.type Func_08042dac, %function
	.thumb_func
Func_08042dac:
	.incbin "baserom.gba", 0x00042dfc, 0x00000318
	.section .rom.00043114, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043114, 0x00000198
	.section .rom.000432ac, "ax"
	.global Func_0804325c
	.type Func_0804325c, %function
	.thumb_func
Func_0804325c:
	.incbin "baserom.gba", 0x000432ac, 0x00000048
	.section .rom.000432f4, "ax"
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000432f4, 0x00000248
	.section .rom.0004353c, "ax"
	.global Func_080434ec
	.type Func_080434ec, %function
	.thumb_func
Func_080434ec:
	.incbin "baserom.gba", 0x0004353c, 0x00000048
	.section .rom.00043584, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043584, 0x000000c8
	.section .rom.0004364c, "ax"
	.global Func_080435fc
	.type Func_080435fc, %function
	.thumb_func
Func_080435fc:
	.incbin "baserom.gba", 0x0004364c, 0x00000094
	.section .rom.000436e0, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436e0, 0x000000e4
	.section .rom.000437c4, "ax"
	.global Func_08043774
	.type Func_08043774, %function
	.thumb_func
Func_08043774:
	.incbin "baserom.gba", 0x000437c4, 0x00000098
	.section .rom.0004385c, "ax"
	.global Func_0804380c
	.type Func_0804380c, %function
	.thumb_func
Func_0804380c:
	.incbin "baserom.gba", 0x0004385c, 0x000000bc
	.section .rom.00043918, "ax"
	.global Func_080438c8
	.type Func_080438c8, %function
	.thumb_func
Func_080438c8:
	.incbin "baserom.gba", 0x00043918, 0x000000b0
	.section .rom.00043a14, "ax"
	.incbin "baserom.gba", 0x00043a14, 0x00000228
	.section .rom.00043c80, "ax"
	.incbin "baserom.gba", 0x00043c80, 0x00000848
	.section .rom.000444c8, "ax"
	.global Func_08044460
	.type Func_08044460, %function
	.thumb_func
Func_08044460:
	.incbin "baserom.gba", 0x000444c8, 0x00000018
	.section .rom.000444e0, "ax"
	.global Func_08044478
	.type Func_08044478, %function
	.thumb_func
Func_08044478:
	.incbin "baserom.gba", 0x000444e0, 0x00000010
	.section .rom.000444f0, "ax"
	.global Func_08044488
	.type Func_08044488, %function
	.thumb_func
Func_08044488:
	.incbin "baserom.gba", 0x000444f0, 0x000000d0
	.section .rom.000445c0, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445c0, 0x0000051c
	.section .rom.00044adc, "ax"
	.global Func_08044a54
	.type Func_08044a54, %function
	.thumb_func
Func_08044a54:
	.incbin "baserom.gba", 0x00044adc, 0x00000004
	.section .rom.00044ae0, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae0, 0x00000140
	.section .rom.00044c20, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044c20, 0x000000e8
	.section .rom.00044d08, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044d08, 0x00000148
	.section .rom.00044e50, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e50, 0x00000484
	.section .rom.00045342, "ax"
	.incbin "baserom.gba", 0x00045342, 0x00000076
	.section .rom.000453b8, "ax"
	.global Func_08045330
	.type Func_08045330, %function
	.thumb_func
Func_08045330:
	.incbin "baserom.gba", 0x000453b8, 0x000000a0
	.section .rom.00045458, "ax"
	.global Func_080453d0
	.type Func_080453d0, %function
	.thumb_func
Func_080453d0:
	.incbin "baserom.gba", 0x00045458, 0x00000094
	.section .rom.000455ae, "ax"
	.incbin "baserom.gba", 0x000455ae, 0x0000002a
	.section .rom.000455ec, "ax"
	.incbin "baserom.gba", 0x000455ec, 0x00000204
	.section .rom.00045806, "ax"
	.incbin "baserom.gba", 0x00045806, 0x00000032
	.section .rom.00045856, "ax"
	.incbin "baserom.gba", 0x00045856, 0x00000966
	.section .rom.000461bc, "ax"
	.global Func_08046134
	.type Func_08046134, %function
	.thumb_func
Func_08046134:
	.incbin "baserom.gba", 0x000461bc, 0x00000094
	.section .rom.00046250, "ax"
	.global Func_080461c8
	.type Func_080461c8, %function
	.thumb_func
Func_080461c8:
	.incbin "baserom.gba", 0x00046250, 0x00000098
	.section .rom.0004630c, "ax"
	.incbin "baserom.gba", 0x0004630c, 0x00000150
	.section .rom.0004649a, "ax"
	.incbin "baserom.gba", 0x0004649a, 0x0000365e
	.section .rom.00049b46, "ax"
	.incbin "baserom.gba", 0x00049b46, 0x00001f5e
	.section .rom.0004bade, "ax"
	.incbin "baserom.gba", 0x0004bade, 0x00000352
	.section .rom.0004be30, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be30, 0x000014d4
	.section .rom.0004d304, "ax"
	.global AffineEffect_InitializeWork
	.type AffineEffect_InitializeWork, %function
	.thumb_func
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d304, 0x0000003c
	.section .rom.0004d340, "ax"
	.global Menu_EndResourceSelection
	.type Menu_EndResourceSelection, %function
	.thumb_func
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d340, 0x00000054
	.section .rom.0004d394, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d394, 0x00000220
	.section .rom.0004d5b4, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x0004d5b4, 0x0000005c
	.section .rom.0004d610, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d610, 0x00000188
	.section .rom.0004d798, "ax"
	.global Menu_AnimateSelectionToEntry
	.type Menu_AnimateSelectionToEntry, %function
	.thumb_func
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d798, 0x0000003c
	.section .rom.0004d7d4, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7d4, 0x0000023c
	.section .rom.0004da10, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da10, 0x0000017c
	.section .rom.0004dc60, "ax"
	.global Func_0804da3c
	.type Func_0804da3c, %function
	.thumb_func
Func_0804da3c:
	.incbin "baserom.gba", 0x0004dc60, 0x00000050
	.section .rom.0004dcb0, "ax"
	.global Menu_SelectEntry20To21
	.type Menu_SelectEntry20To21, %function
	.thumb_func
Menu_SelectEntry20To21:
	.incbin "baserom.gba", 0x0004dcb0, 0x000000e8
	.section .rom.0004dd98, "ax"
	.global Func_0804db74
	.type Func_0804db74, %function
	.thumb_func
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd98, 0x00000254
	.section .rom.0004e0c4, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0c4, 0x000000c4
	.section .rom.0004e188, "ax"
	.global Menu_HandleFlagGridInput
	.type Menu_HandleFlagGridInput, %function
	.thumb_func
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e188, 0x00000140
	.section .rom.0004e2f2, "ax"
	.incbin "baserom.gba", 0x0004e2f2, 0x00000002
	.section .rom.0004e2f4, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2f4, 0x00000108
	.section .rom.0004e3fc, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3fc, 0x0000021c
	.section .rom.0004e618, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e618, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed7c, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f348, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054c38, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055048, 0x000057a0
	.section .rom.0005c3e8, "ax"
	.incbin "baserom.gba", 0x0005c3e8, 0x00003868
	.section .rom.000a8df0, "ax"
	.incbin "baserom.gba", 0x000a8df0, 0x00000300
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a90f0, 0x00008f10
	.section .rom.000b2000, "ax"
	.global Trade_GetOfferStateFar
	.type Trade_GetOfferStateFar, %function
	.thumb_func
Trade_GetOfferStateFar:
	.global Resource_FarCall005
Resource_FarCall005:
	.incbin "baserom.gba", 0x000b2000, 0x00000008
	.section .rom.000b2008, "ax"
	.global Owner_RecalculateStatsFar
	.type Owner_RecalculateStatsFar, %function
	.thumb_func
Owner_RecalculateStatsFar:
	.incbin "baserom.gba", 0x000b2008, 0x00000008
	.section .rom.000b2010, "ax"
	.global Item_Get
	.type Item_Get, %function
	.thumb_func
Item_Get:
	.incbin "baserom.gba", 0x000b2010, 0x00000010
	.section .rom.000b2020, "ax"
	.global Inventory_AddItemFar
	.type Inventory_AddItemFar, %function
	.thumb_func
Inventory_AddItemFar:
	.incbin "baserom.gba", 0x000b2020, 0x00000030
	.section .rom.000b2050, "ax"
	.global Inventory_RemoveFar
	.type Inventory_RemoveFar, %function
	.thumb_func
Inventory_RemoveFar:
	.incbin "baserom.gba", 0x000b2050, 0x00000028
	.section .rom.000b2078, "ax"
	.global BattleAction_Get
	.type BattleAction_Get, %function
	.thumb_func
BattleAction_Get:
	.incbin "baserom.gba", 0x000b2078, 0x00000008
	.section .rom.000b2080, "ax"
	.global OwnerAction_AddFar
	.type OwnerAction_AddFar, %function
	.thumb_func
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000b2080, 0x00000008
	.section .rom.000b2088, "ax"
	.global Equipment_HasValueFar
	.type Equipment_HasValueFar, %function
	.thumb_func
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000b2088, 0x00000068
	.global Party_CountActiveOwnersFar
	.type Party_CountActiveOwnersFar, %function
	.thumb_func
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000b20f0, 0x00000008
	.section .rom.000b20f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000b20f8, 0x00000018
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000b2110, 0x00000038
	.section .rom.000b2148, "ax"
	.global BattleRandom16Far
	.type BattleRandom16Far, %function
	.thumb_func
BattleRandom16Far:
	.incbin "baserom.gba", 0x000b2148, 0x00000008
	.section .rom.000b2150, "ax"
	.global Djinn_AddToOwnerFar
	.type Djinn_AddToOwnerFar, %function
	.thumb_func
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000b2150, 0x00000008
	.section .rom.000b2158, "ax"
	.global Djinn_ActivateFar
	.type Djinn_ActivateFar, %function
	.thumb_func
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000b2158, 0x00000010
	.section .rom.000b2168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b2168, 0x00000058
	.section .rom.000b21c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b21c0, 0x000000c0
	.section .rom.000b2280, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000b2280, 0x000000c8
	.section .rom.000b2348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000b2348, 0x0000007c
	.section .rom.000b23f6, "ax"
	.incbin "baserom.gba", 0x000b23f6, 0x00000002
	.section .rom.000b23f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000b23f8, 0x0000143c
	.section .rom.000b3868, "ax"
	.incbin "baserom.gba", 0x000b3868, 0x000001c8
	.section .rom.000b3c66, "ax"
	.incbin "baserom.gba", 0x000b3c66, 0x00000002
	.section .rom.000b3c68, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b3c68, 0x00000060
	.section .rom.000b3e40, "ax"
	.incbin "baserom.gba", 0x000b3e40, 0x00000058
	.section .rom.000b3e98, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000b3e98, 0x00000030
	.section .rom.000b3f34, "ax"
	.incbin "baserom.gba", 0x000b3f34, 0x000000d4
	.section .rom.000b40e2, "ax"
	.incbin "baserom.gba", 0x000b40e2, 0x00000066
	.section .rom.000b4148, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000b4148, 0x00000080
	.section .rom.000b41fa, "ax"
	.incbin "baserom.gba", 0x000b41fa, 0x0000009e
	.section .rom.000b42c0, "ax"
	.incbin "baserom.gba", 0x000b42c0, 0x00000028
	.section .rom.000b4338, "ax"
	.incbin "baserom.gba", 0x000b4338, 0x00000040
	.section .rom.000b4378, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000b4378, 0x00000028
	.section .rom.000b443c, "ax"
	.incbin "baserom.gba", 0x000b443c, 0x000004e0
	.section .rom.000b491c, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b491c, 0x00000298
	.section .rom.000b4bec, "ax"
	.incbin "baserom.gba", 0x000b4bec, 0x000001d0
	.section .rom.000b4dd8, "ax"
	.incbin "baserom.gba", 0x000b4dd8, 0x000000a0
	.section .rom.000b4eb0, "ax"
	.incbin "baserom.gba", 0x000b4eb0, 0x00000024
	.section .rom.000b4f28, "ax"
	.incbin "baserom.gba", 0x000b4f28, 0x00000054
	.section .rom.000b4f94, "ax"
	.incbin "baserom.gba", 0x000b4f94, 0x000002f4
	.section .rom.000b5298, "ax"
	.incbin "baserom.gba", 0x000b5298, 0x000001c8
	.section .rom.000b54ba, "ax"
	.incbin "baserom.gba", 0x000b54ba, 0x000005da
	.section .rom.000b5b78, "ax"
	.global Djinn_AddToOwner
	.type Djinn_AddToOwner, %function
	.thumb_func
Djinn_AddToOwner:
	.incbin "baserom.gba", 0x000b5b78, 0x00000100
	.section .rom.000b5c9a, "ax"
	.incbin "baserom.gba", 0x000b5c9a, 0x00000002
	.section .rom.000b5c9c, "ax"
	.global Djinn_Activate
	.type Djinn_Activate, %function
	.thumb_func
Djinn_Activate:
	.incbin "baserom.gba", 0x000b5c9c, 0x00000068
	.section .rom.000b5d04, "ax"
	.global Djinn_Deactivate
	.type Djinn_Deactivate, %function
	.thumb_func
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000b5d04, 0x00000054
	.section .rom.000b5d58, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b5d58, 0x000000ac
	.section .rom.000b6002, "ax"
	.incbin "baserom.gba", 0x000b6002, 0x00001362
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000b7364, 0x0000f1a8
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000c650c, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000c65f4, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000cbbb0, 0x00001450
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000cd000, 0x00000008
	.section .rom.000cd5c0, "ax"
	.incbin "baserom.gba", 0x000cd5c0, 0x00000008
	.section .rom.000cd9cc, "ax"
	.incbin "baserom.gba", 0x000cd9cc, 0x00000a00
	.section .rom.000ce5a8, "ax"
	.incbin "baserom.gba", 0x000ce5a8, 0x00000378
	.section .rom.000ce934, "ax"
	.incbin "baserom.gba", 0x000ce934, 0x00000010
	.section .rom.000ce944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000ce944, 0x00000484
	.section .rom.000cedc8, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000cedc8, 0x000003c4
	.section .rom.000cf18c, "ax"
	.global Func_080ca18c
	.type Func_080ca18c, %function
	.thumb_func
Func_080ca18c:
	.incbin "baserom.gba", 0x000cf18c, 0x00000018
	.section .rom.000cf1a4, "ax"
	.global Func_080ca1a4
	.type Func_080ca1a4, %function
	.thumb_func
Func_080ca1a4:
	.incbin "baserom.gba", 0x000cf1a4, 0x00000018
	.section .rom.000cf1bc, "ax"
	.global Func_080ca1bc
	.type Func_080ca1bc, %function
	.thumb_func
Func_080ca1bc:
	.incbin "baserom.gba", 0x000cf1bc, 0x000000c4
	.section .rom.000cf280, "ax"
	.global Func_080ca280
	.type Func_080ca280, %function
	.thumb_func
Func_080ca280:
	.incbin "baserom.gba", 0x000cf280, 0x000000e8
	.section .rom.000cf368, "ax"
	.global Func_080ca368
	.type Func_080ca368, %function
	.thumb_func
Func_080ca368:
	.incbin "baserom.gba", 0x000cf368, 0x0000008c
	.section .rom.000cf3f4, "ax"
	.global Func_080ca3f4
	.type Func_080ca3f4, %function
	.thumb_func
Func_080ca3f4:
	.incbin "baserom.gba", 0x000cf3f4, 0x000000a0
	.section .rom.000cf494, "ax"
	.global Event_GetSpecialValue
	.type Event_GetSpecialValue, %function
	.thumb_func
Event_GetSpecialValue:
	.incbin "baserom.gba", 0x000cf494, 0x00000210
	.section .rom.000cf6a4, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000cf6a4, 0x00000040
	.section .rom.000cf6e4, "ax"
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000cf6e4, 0x00000004
	.section .rom.000cf6e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000cf6e8, 0x000002e4
	.section .rom.000cf9cc, "ax"
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000cf9cc, 0x00000060
	.section .rom.000cfa2c, "ax"
	.global Func_080caa2c
	.type Func_080caa2c, %function
	.thumb_func
Func_080caa2c:
	.incbin "baserom.gba", 0x000cfa2c, 0x00000020
	.section .rom.000cfa4c, "ax"
	.global Func_080caa4c
	.type Func_080caa4c, %function
	.thumb_func
Func_080caa4c:
	.incbin "baserom.gba", 0x000cfa4c, 0x00000274
	.section .rom.000cfcc0, "ax"
	.global ObjectTable_FindLastActiveId
	.type ObjectTable_FindLastActiveId, %function
	.thumb_func
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000cfcc0, 0x0000002c
	.section .rom.000cfcec, "ax"
	.global Scene_AssignViewFlags
	.type Scene_AssignViewFlags, %function
	.thumb_func
Scene_AssignViewFlags:
	.incbin "baserom.gba", 0x000cfcec, 0x00000098
	.section .rom.000cfd84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000cfd84, 0x00000018
	.section .rom.000cfd9c, "ax"
	.global Func_080cad9c
	.type Func_080cad9c, %function
	.thumb_func
Func_080cad9c:
	.incbin "baserom.gba", 0x000cfd9c, 0x000000c0
	.section .rom.000cfe5c, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000cfe5c, 0x00000114
	.section .rom.000cffc4, "ax"
	.incbin "baserom.gba", 0x000cffc4, 0x00000704
	.section .rom.000d06c8, "ax"
	.global Func_080cb6c8
	.type Func_080cb6c8, %function
	.thumb_func
Func_080cb6c8:
	.incbin "baserom.gba", 0x000d06c8, 0x0000002c
	.section .rom.000d06f4, "ax"
	.global BattleParty_ApplyHealthDelta
	.type BattleParty_ApplyHealthDelta, %function
	.thumb_func
BattleParty_ApplyHealthDelta:
	.incbin "baserom.gba", 0x000d06f4, 0x00000138
	.section .rom.000d082c, "ax"
	.global Func_080cb82c
	.type Func_080cb82c, %function
	.thumb_func
Func_080cb82c:
	.incbin "baserom.gba", 0x000d082c, 0x00000078
	.section .rom.000d08a4, "ax"
	.global Func_080cb8a4
	.type Func_080cb8a4, %function
	.thumb_func
Func_080cb8a4:
	.incbin "baserom.gba", 0x000d08a4, 0x00000ca8
	.section .rom.000d154c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d154c, 0x000007fc
	.section .rom.000d1d76, "ax"
	.incbin "baserom.gba", 0x000d1d76, 0x00000002
	.section .rom.000d1d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1d78, 0x00000150
	.section .rom.000d1ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d1ec8, 0x00000a54
	.section .rom.000d291c, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000d291c, 0x000002dc
	.section .rom.000d2bf8, "ax"
	.global Func_080cdbf8
	.type Func_080cdbf8, %function
	.thumb_func
Func_080cdbf8:
	.incbin "baserom.gba", 0x000d2bf8, 0x00000048
	.section .rom.000d2c72, "ax"
	.incbin "baserom.gba", 0x000d2c72, 0x00000002
	.section .rom.000d2c74, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000d2c74, 0x0000010c
	.section .rom.000d2d80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000d2d80, 0x00000148
	.section .rom.000d2ec8, "ax"
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000d2ec8, 0x0000000c
	.section .rom.000d2ed4, "ax"
	.global Func_080cded4
	.type Func_080cded4, %function
	.thumb_func
Func_080cded4:
	.incbin "baserom.gba", 0x000d2ed4, 0x00000088
	.section .rom.000d2f5c, "ax"
	.global Func_080cdf5c
	.type Func_080cdf5c, %function
	.thumb_func
Func_080cdf5c:
	.incbin "baserom.gba", 0x000d2f5c, 0x00000054
	.section .rom.000d2fb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d2fb0, 0x00000ba8
	.section .rom.000d3b58, "ax"
	.global Func_080ceb58
	.type Func_080ceb58, %function
	.thumb_func
Func_080ceb58:
	.incbin "baserom.gba", 0x000d3b58, 0x00000028
	.section .rom.000d3b94, "ax"
	.incbin "baserom.gba", 0x000d3b94, 0x000002ac
	.section .rom.000d3e80, "ax"
	.incbin "baserom.gba", 0x000d3e80, 0x00000048
	.section .rom.000d3ec8, "ax"
	.global Func_080ceec8
	.type Func_080ceec8, %function
	.thumb_func
Func_080ceec8:
	.incbin "baserom.gba", 0x000d3ec8, 0x000000a0
	.section .rom.000d3f68, "ax"
	.global Func_080cef68
	.type Func_080cef68, %function
	.thumb_func
Func_080cef68:
	.incbin "baserom.gba", 0x000d3f68, 0x0000004c
	.section .rom.000d3fb4, "ax"
	.global Func_080cefb4
	.type Func_080cefb4, %function
	.thumb_func
Func_080cefb4:
	.incbin "baserom.gba", 0x000d3fb4, 0x0000001c
	.section .rom.000d3fd0, "ax"
	.global Func_080cefd0
	.type Func_080cefd0, %function
	.thumb_func
Func_080cefd0:
	.incbin "baserom.gba", 0x000d3fd0, 0x0000002c
	.section .rom.000d3ffc, "ax"
	.global Func_080ceffc
	.type Func_080ceffc, %function
	.thumb_func
Func_080ceffc:
	.incbin "baserom.gba", 0x000d3ffc, 0x00000008
	.section .rom.000d4004, "ax"
	.global Func_080cf004
	.type Func_080cf004, %function
	.thumb_func
Func_080cf004:
	.incbin "baserom.gba", 0x000d4004, 0x00000008
	.section .rom.000d400c, "ax"
	.global Func_080cf00c
	.type Func_080cf00c, %function
	.thumb_func
Func_080cf00c:
	.incbin "baserom.gba", 0x000d400c, 0x00000044
	.section .rom.000d4050, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000d4050, 0x0000012c
	.section .rom.000d417c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d417c, 0x00000238
	.section .rom.000d43b4, "ax"
	.global Func_080cf3b4
	.type Func_080cf3b4, %function
	.thumb_func
Func_080cf3b4:
	.incbin "baserom.gba", 0x000d43b4, 0x00000070
	.section .rom.000d4424, "ax"
	.global Func_080cf424
	.type Func_080cf424, %function
	.thumb_func
Func_080cf424:
	.incbin "baserom.gba", 0x000d4424, 0x00000cd4
	.section .rom.000d50f8, "ax"
	.global Func_080d00f8
	.type Func_080d00f8, %function
	.thumb_func
Func_080d00f8:
	.incbin "baserom.gba", 0x000d50f8, 0x000000d4
	.section .rom.000d51cc, "ax"
	.global Func_080d01cc
	.type Func_080d01cc, %function
	.thumb_func
Func_080d01cc:
	.incbin "baserom.gba", 0x000d51cc, 0x00000354
	.section .rom.000d5520, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000d5520, 0x0000020c
	.section .rom.000d572c, "ax"
	.global Func_080d072c
	.type Func_080d072c, %function
	.thumb_func
Func_080d072c:
	.incbin "baserom.gba", 0x000d572c, 0x0000001c
	.section .rom.000d5748, "ax"
	.global Func_080d0748
	.type Func_080d0748, %function
	.thumb_func
Func_080d0748:
	.incbin "baserom.gba", 0x000d5748, 0x000004a4
	.section .rom.000d5bec, "ax"
	.global Func_080d0bec
	.type Func_080d0bec, %function
	.thumb_func
Func_080d0bec:
	.incbin "baserom.gba", 0x000d5bec, 0x00000064
	.section .rom.000d5c50, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000d5c50, 0x00000a34
	.section .rom.000d6684, "ax"
	.global Func_080d1684
	.type Func_080d1684, %function
	.thumb_func
Func_080d1684:
	.incbin "baserom.gba", 0x000d6684, 0x00000074
	.section .rom.000d66f8, "ax"
	.global Func_080d16f8
	.type Func_080d16f8, %function
	.thumb_func
Func_080d16f8:
	.incbin "baserom.gba", 0x000d66f8, 0x00000014
	.section .rom.000d670c, "ax"
	.global Func_080d170c
	.type Func_080d170c, %function
	.thumb_func
Func_080d170c:
	.incbin "baserom.gba", 0x000d670c, 0x00000020
	.section .rom.000d672c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000d672c, 0x00000020
	.section .rom.000d674c, "ax"
	.global Func_080d174c
	.type Func_080d174c, %function
	.thumb_func
Func_080d174c:
	.incbin "baserom.gba", 0x000d674c, 0x00000014
	.section .rom.000d6760, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d6760, 0x0000004c
	.section .rom.000d67ac, "ax"
	.global BattleFx_StartBufferInterpolation
	.type BattleFx_StartBufferInterpolation, %function
	.thumb_func
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000d67ac, 0x0000003c
	.section .rom.000d67e8, "ax"
	.global Func_080d17e8
	.type Func_080d17e8, %function
	.thumb_func
Func_080d17e8:
	.incbin "baserom.gba", 0x000d67e8, 0x00000034
	.section .rom.000d683e, "ax"
	.incbin "baserom.gba", 0x000d683e, 0x000001da
	.section .rom.000d6a18, "ax"
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000d6a18, 0x000000ac
	.section .rom.000d6ac4, "ax"
	.global Func_080d1ac4
	.type Func_080d1ac4, %function
	.thumb_func
Func_080d1ac4:
	.incbin "baserom.gba", 0x000d6ac4, 0x00000010
	.section .rom.000d6ad4, "ax"
	.global Func_080d1ad4
	.type Func_080d1ad4, %function
	.thumb_func
Func_080d1ad4:
	.incbin "baserom.gba", 0x000d6ad4, 0x000001ec
	.section .rom.000d6cc0, "ax"
	.global Func_080d1cc0
	.type Func_080d1cc0, %function
	.thumb_func
Func_080d1cc0:
	.incbin "baserom.gba", 0x000d6cc0, 0x00000074
	.section .rom.000d6d34, "ax"
	.global Func_080d1d34
	.type Func_080d1d34, %function
	.thumb_func
Func_080d1d34:
	.incbin "baserom.gba", 0x000d6d34, 0x00000010
	.section .rom.000d6d44, "ax"
	.global Func_080d1d44
	.type Func_080d1d44, %function
	.thumb_func
Func_080d1d44:
	.incbin "baserom.gba", 0x000d6d44, 0x00000010
	.section .rom.000d6d54, "ax"
	.global Func_080d1d54
	.type Func_080d1d54, %function
	.thumb_func
Func_080d1d54:
	.incbin "baserom.gba", 0x000d6d54, 0x00000058
	.section .rom.000d6dac, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d6dac, 0x00000044
	.section .rom.000d6df0, "ax"
	.global Func_080d1df0
	.type Func_080d1df0, %function
	.thumb_func
Func_080d1df0:
	.incbin "baserom.gba", 0x000d6df0, 0x00000028
	.section .rom.000d6e18, "ax"
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000d6e18, 0x00000094
	.section .rom.000d6eac, "ax"
	.global Func_080d1eac
	.type Func_080d1eac, %function
	.thumb_func
Func_080d1eac:
	.incbin "baserom.gba", 0x000d6eac, 0x0000002c
	.section .rom.000d6ed8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000d6ed8, 0x00000010
	.section .rom.000d6ee8, "ax"
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d6ee8, 0x00000024
	.section .rom.000d6f0c, "ax"
	.global Func_080d1f0c
	.type Func_080d1f0c, %function
	.thumb_func
Func_080d1f0c:
	.incbin "baserom.gba", 0x000d6f0c, 0x0000000c
	.section .rom.000d6f18, "ax"
	.global Func_080d1f18
	.type Func_080d1f18, %function
	.thumb_func
Func_080d1f18:
	.incbin "baserom.gba", 0x000d6f18, 0x00000008
	.section .rom.000d6f20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000d6f20, 0x00000164
	.section .rom.000d7084, "ax"
	.global Func_080d2084
	.type Func_080d2084, %function
	.thumb_func
Func_080d2084:
	.incbin "baserom.gba", 0x000d7084, 0x00000078
	.section .rom.000d70fc, "ax"
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000d70fc, 0x00000144
	.section .rom.000d7240, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000d7240, 0x00000020
	.section .rom.000d7260, "ax"
	.global Func_080d2260
	.type Func_080d2260, %function
	.thumb_func
Func_080d2260:
	.incbin "baserom.gba", 0x000d7260, 0x00000048
	.section .rom.000d72a8, "ax"
	.global Func_080d22a8
	.type Func_080d22a8, %function
	.thumb_func
Func_080d22a8:
	.incbin "baserom.gba", 0x000d72a8, 0x000000a8
	.section .rom.000d7350, "ax"
	.global Func_080d2350
	.type Func_080d2350, %function
	.thumb_func
Func_080d2350:
	.incbin "baserom.gba", 0x000d7350, 0x00000048
	.section .rom.000d7398, "ax"
	.global Event_RunObjectHookAndWait
	.type Event_RunObjectHookAndWait, %function
	.thumb_func
Event_RunObjectHookAndWait:
	.incbin "baserom.gba", 0x000d7398, 0x00000020
	.section .rom.000d73ca, "ax"
	.incbin "baserom.gba", 0x000d73ca, 0x00000002
	.section .rom.000d73cc, "ax"
	.global Event_SetWorkWord10
	.type Event_SetWorkWord10, %function
	.thumb_func
Event_SetWorkWord10:
	.incbin "baserom.gba", 0x000d73cc, 0x0000000c
	.section .rom.000d75c8, "ax"
	.global Func_080d25c8
	.type Func_080d25c8, %function
	.thumb_func
Func_080d25c8:
	.incbin "baserom.gba", 0x000d75c8, 0x00000044
	.section .rom.000d760c, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d760c, 0x000001d4
	.section .rom.000d77fe, "ax"
	.incbin "baserom.gba", 0x000d77fe, 0x00000036
	.section .rom.000d7834, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d7834, 0x0000011c
	.section .rom.000d7950, "ax"
	.global Func_080d295c
	.type Func_080d295c, %function
	.thumb_func
Func_080d295c:
	.incbin "baserom.gba", 0x000d7950, 0x00000010
	.section .rom.000d7960, "ax"
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000d7960, 0x000000a0
	.section .rom.000d7a30, "ax"
	.global Func_080d2a3c
	.type Func_080d2a3c, %function
	.thumb_func
Func_080d2a3c:
	.incbin "baserom.gba", 0x000d7a30, 0x00000028
	.section .rom.000d7a58, "ax"
	.global Func_080d2a64
	.type Func_080d2a64, %function
	.thumb_func
Func_080d2a64:
	.incbin "baserom.gba", 0x000d7a58, 0x00000028
	.section .rom.000d7a80, "ax"
	.global Func_080d2a8c
	.type Func_080d2a8c, %function
	.thumb_func
Func_080d2a8c:
	.incbin "baserom.gba", 0x000d7a80, 0x00000018
	.section .rom.000d7a98, "ax"
	.global Func_080d2aa4
	.type Func_080d2aa4, %function
	.thumb_func
Func_080d2aa4:
	.incbin "baserom.gba", 0x000d7a98, 0x0000002c
	.section .rom.000d7ac4, "ax"
	.global Func_080d2ad0
	.type Func_080d2ad0, %function
	.thumb_func
Func_080d2ad0:
	.incbin "baserom.gba", 0x000d7ac4, 0x0000002c
	.section .rom.000d7af0, "ax"
	.global Func_080d2afc
	.type Func_080d2afc, %function
	.thumb_func
Func_080d2afc:
	.incbin "baserom.gba", 0x000d7af0, 0x00000010
	.section .rom.000d7b00, "ax"
	.global Func_080d2b0c
	.type Func_080d2b0c, %function
	.thumb_func
Func_080d2b0c:
	.incbin "baserom.gba", 0x000d7b00, 0x00000040
	.section .rom.000d7b40, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d7b40, 0x000000b8
	.section .rom.000d7c58, "ax"
	.global Func_080d2c64
	.type Func_080d2c64, %function
	.thumb_func
Func_080d2c64:
	.incbin "baserom.gba", 0x000d7c58, 0x00000034
	.section .rom.000d7c8c, "ax"
	.global Func_080d2c98
	.type Func_080d2c98, %function
	.thumb_func
Func_080d2c98:
	.incbin "baserom.gba", 0x000d7c8c, 0x00000004
	.section .rom.000d7c90, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d7c90, 0x00000028
	.section .rom.000d7cb8, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d7cb8, 0x00000044
	.section .rom.000d7cfc, "ax"
	.global Func_080d2d08
	.type Func_080d2d08, %function
	.thumb_func
Func_080d2d08:
	.incbin "baserom.gba", 0x000d7cfc, 0x00000068
	.section .rom.000d7d64, "ax"
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000d7d64, 0x00000014
	.section .rom.000d7dd8, "ax"
	.global ObjectMotion_ResetTargetsAndVelocity
	.type ObjectMotion_ResetTargetsAndVelocity, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	.incbin "baserom.gba", 0x000d7dd8, 0x00000038
	.section .rom.000d7f3c, "ax"
	.global ObjectMotion_SnapHeadingAndOffset
	.type ObjectMotion_SnapHeadingAndOffset, %function
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	.incbin "baserom.gba", 0x000d7f3c, 0x00000080
	.section .rom.000d8064, "ax"
	.global Func_080d3070
	.type Func_080d3070, %function
	.thumb_func
Func_080d3070:
	.incbin "baserom.gba", 0x000d8064, 0x0000008c
	.section .rom.000d810a, "ax"
	.incbin "baserom.gba", 0x000d810a, 0x00000002
	.section .rom.000d810c, "ax"
	.global Func_080d3118
	.type Func_080d3118, %function
	.thumb_func
Func_080d3118:
	.incbin "baserom.gba", 0x000d810c, 0x0000004c
	.section .rom.000d8158, "ax"
	.global Func_080d3164
	.type Func_080d3164, %function
	.thumb_func
Func_080d3164:
	.incbin "baserom.gba", 0x000d8158, 0x0000005c
	.section .rom.000d81b4, "ax"
	.global Func_080d31c0
	.type Func_080d31c0, %function
	.thumb_func
Func_080d31c0:
	.incbin "baserom.gba", 0x000d81b4, 0x00000054
	.section .rom.000d8208, "ax"
	.global Func_080d3214
	.type Func_080d3214, %function
	.thumb_func
Func_080d3214:
	.incbin "baserom.gba", 0x000d8208, 0x0000002c
	.section .rom.000d825c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d825c, 0x00000050
	.section .rom.000d82f4, "ax"
	.global ObjectMotion_SetVariantCallback
	.type ObjectMotion_SetVariantCallback, %function
	.thumb_func
ObjectMotion_SetVariantCallback:
	.incbin "baserom.gba", 0x000d82f4, 0x0000002c
	.section .rom.000d8330, "ax"
	.incbin "baserom.gba", 0x000d8330, 0x00000124
	.section .rom.000d8454, "ax"
	.global Func_080d3460
	.type Func_080d3460, %function
	.thumb_func
Func_080d3460:
	.incbin "baserom.gba", 0x000d8454, 0x0000013c
	.section .rom.000d85f2, "ax"
	.incbin "baserom.gba", 0x000d85f2, 0x00000002
	.section .rom.000d85f4, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000d85f4, 0x000000a8
	.section .rom.000d869c, "ax"
	.global Func_080d36a8
	.type Func_080d36a8, %function
	.thumb_func
Func_080d36a8:
	.incbin "baserom.gba", 0x000d869c, 0x00000020
	.section .rom.000d86bc, "ax"
	.global ObjectGroup_ConfigureChildValue
	.type ObjectGroup_ConfigureChildValue, %function
	.thumb_func
ObjectGroup_ConfigureChildValue:
	.incbin "baserom.gba", 0x000d86bc, 0x0000007c
	.section .rom.000d8738, "ax"
	.global Object_SetPartAttribute
	.type Object_SetPartAttribute, %function
	.thumb_func
Object_SetPartAttribute:
	.incbin "baserom.gba", 0x000d8738, 0x0000003c
	.section .rom.000d8774, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000d8774, 0x00000054
	.section .rom.000d8900, "ax"
	.global Func_080d390c
	.type Func_080d390c, %function
	.thumb_func
Func_080d390c:
	.incbin "baserom.gba", 0x000d8900, 0x0000001c
	.section .rom.000d891c, "ax"
	.global Func_080d3928
	.type Func_080d3928, %function
	.thumb_func
Func_080d3928:
	.incbin "baserom.gba", 0x000d891c, 0x00000018
	.section .rom.000d8934, "ax"
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000d8934, 0x000000d0
	.section .rom.000d8a04, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000d8a04, 0x00000118
	.section .rom.000d8b1c, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000d8b1c, 0x000000c0
	.section .rom.000d8bdc, "ax"
	.global Func_080d3be8
	.type Func_080d3be8, %function
	.thumb_func
Func_080d3be8:
	.incbin "baserom.gba", 0x000d8bdc, 0x00000010
	.section .rom.000d8bec, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000d8bec, 0x00000034
	.section .rom.000d8c20, "ax"
	.global Func_080d3c2c
	.type Func_080d3c2c, %function
	.thumb_func
Func_080d3c2c:
	.incbin "baserom.gba", 0x000d8c20, 0x0000005c
	.section .rom.000d8c7c, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d8c7c, 0x00000328
	.section .rom.000d8fa4, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000d8fa4, 0x000000bc
	.section .rom.000d9060, "ax"
	.global Func_080d406c
	.type Func_080d406c, %function
	.thumb_func
Func_080d406c:
	.incbin "baserom.gba", 0x000d9060, 0x00000010
	.section .rom.000d9070, "ax"
	.global Func_080d407c
	.type Func_080d407c, %function
	.thumb_func
Func_080d407c:
	.incbin "baserom.gba", 0x000d9070, 0x00000008
	.section .rom.000d9078, "ax"
	.global Func_080d4084
	.type Func_080d4084, %function
	.thumb_func
Func_080d4084:
	.incbin "baserom.gba", 0x000d9078, 0x00000054
	.section .rom.000d90cc, "ax"
	.global Func_080d40d8
	.type Func_080d40d8, %function
	.thumb_func
Func_080d40d8:
	.incbin "baserom.gba", 0x000d90cc, 0x00000004
	.section .rom.000d90d0, "ax"
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000d90d0, 0x0000009c
	.section .rom.000d916c, "ax"
	.global Func_080d4178
	.type Func_080d4178, %function
	.thumb_func
Func_080d4178:
	.incbin "baserom.gba", 0x000d916c, 0x00000008
	.section .rom.000d9174, "ax"
	.global Func_080d4180
	.type Func_080d4180, %function
	.thumb_func
Func_080d4180:
	.incbin "baserom.gba", 0x000d9174, 0x00000068
	.section .rom.000d91f0, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000d91f0, 0x00000134
	.section .rom.000d9324, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000d9324, 0x00000054
	.section .rom.000d9378, "ax"
	.global Object_AttachWorkTargetToObject
	.type Object_AttachWorkTargetToObject, %function
	.thumb_func
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000d9378, 0x00000068
	.section .rom.000d93e0, "ax"
	.global Func_080d43ec
	.type Func_080d43ec, %function
	.thumb_func
Func_080d43ec:
	.incbin "baserom.gba", 0x000d93e0, 0x00000020
	.section .rom.000d9400, "ax"
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d9400, 0x00000100
	.section .rom.000d9500, "ax"
	.global Func_080d450c
	.type Func_080d450c, %function
	.thumb_func
Func_080d450c:
	.incbin "baserom.gba", 0x000d9500, 0x00000020
	.section .rom.000d9520, "ax"
	.global Func_080d452c
	.type Func_080d452c, %function
	.thumb_func
Func_080d452c:
	.incbin "baserom.gba", 0x000d9520, 0x0000001c
	.section .rom.000d953c, "ax"
	.global Func_080d4548
	.type Func_080d4548, %function
	.thumb_func
Func_080d4548:
	.incbin "baserom.gba", 0x000d953c, 0x00000020
	.section .rom.000d955c, "ax"
	.global Func_080d4568
	.type Func_080d4568, %function
	.thumb_func
Func_080d4568:
	.incbin "baserom.gba", 0x000d955c, 0x00000050
	.section .rom.000d95ac, "ax"
	.global Func_080d45b8
	.type Func_080d45b8, %function
	.thumb_func
Func_080d45b8:
	.incbin "baserom.gba", 0x000d95ac, 0x000000ec
	.section .rom.000d9698, "ax"
	.global Func_080d46a4
	.type Func_080d46a4, %function
	.thumb_func
Func_080d46a4:
	.incbin "baserom.gba", 0x000d9698, 0x00000070
	.section .rom.000d9708, "ax"
	.global Func_080d4714
	.type Func_080d4714, %function
	.thumb_func
Func_080d4714:
	.incbin "baserom.gba", 0x000d9708, 0x000000a0
	.section .rom.000d97a8, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000d97a8, 0x000000e8
	.section .rom.000d9890, "ax"
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000d9890, 0x000000f0
	.section .rom.000d9a48, "ax"
	.incbin "baserom.gba", 0x000d9a48, 0x00000054
	.section .rom.000d9a9c, "ax"
	.global Func_080d4aa8
	.type Func_080d4aa8, %function
	.thumb_func
Func_080d4aa8:
	.incbin "baserom.gba", 0x000d9a9c, 0x0000000c
	.section .rom.000d9aa8, "ax"
	.global Func_080d4ab4
	.type Func_080d4ab4, %function
	.thumb_func
Func_080d4ab4:
	.incbin "baserom.gba", 0x000d9aa8, 0x00000058
	.section .rom.000d9b00, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000d9b00, 0x000001fc
	.section .rom.000d9cfc, "ax"
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000d9cfc, 0x000003f0
	.section .rom.000da0ec, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000da0ec, 0x000002c0
	.section .rom.000da3ac, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000da3ac, 0x00000834
	.section .rom.000dabe0, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000dabe0, 0x00000184
	.section .rom.000dad64, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000dad64, 0x00000070
	.section .rom.000dadd4, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000dadd4, 0x00000070
	.section .rom.000dae56, "ax"
	.incbin "baserom.gba", 0x000dae56, 0x00000002
	.section .rom.000dae58, "ax"
	.global Func_080d5e64
	.type Func_080d5e64, %function
	.thumb_func
Func_080d5e64:
	.incbin "baserom.gba", 0x000dae58, 0x00000014
	.section .rom.000dae6c, "ax"
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000dae6c, 0x000000a0
	.section .rom.000dafc8, "ax"
	.incbin "baserom.gba", 0x000dafc8, 0x00000434
	.section .rom.000db3fc, "ax"
	.global Func_080d6408
	.type Func_080d6408, %function
	.thumb_func
Func_080d6408:
	.incbin "baserom.gba", 0x000db3fc, 0x000000b0
	.section .rom.000db4ac, "ax"
	.global Func_080d64b8
	.type Func_080d64b8, %function
	.thumb_func
Func_080d64b8:
	.incbin "baserom.gba", 0x000db4ac, 0x00000454
	.section .rom.000db900, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000db900, 0x00000284
	.section .rom.000dbb84, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000dbb84, 0x000002c8
	.section .rom.000dbe4c, "ax"
	.global Func_080d6e58
	.type Func_080d6e58, %function
	.thumb_func
Func_080d6e58:
	.incbin "baserom.gba", 0x000dbe4c, 0x000000dc
	.section .rom.000dbf28, "ax"
	.global Func_080d6f34
	.type Func_080d6f34, %function
	.thumb_func
Func_080d6f34:
	.incbin "baserom.gba", 0x000dbf28, 0x000000f0
	.section .rom.000dc018, "ax"
	.global Func_080d7024
	.type Func_080d7024, %function
	.thumb_func
Func_080d7024:
	.incbin "baserom.gba", 0x000dc018, 0x0000021c
	.section .rom.000dc234, "ax"
	.global Func_080d7240
	.type Func_080d7240, %function
	.thumb_func
Func_080d7240:
	.incbin "baserom.gba", 0x000dc234, 0x000000c4
	.section .rom.000dc2f8, "ax"
	.global Func_080d7304
	.type Func_080d7304, %function
	.thumb_func
Func_080d7304:
	.incbin "baserom.gba", 0x000dc2f8, 0x000000b0
	.section .rom.000dc3a8, "ax"
	.global Func_080d73b4
	.type Func_080d73b4, %function
	.thumb_func
Func_080d73b4:
	.incbin "baserom.gba", 0x000dc3a8, 0x0000002c
	.section .rom.000dc3d4, "ax"
	.global Func_080d73e0
	.type Func_080d73e0, %function
	.thumb_func
Func_080d73e0:
	.incbin "baserom.gba", 0x000dc3d4, 0x00000028
	.section .rom.000dc3fc, "ax"
	.global Func_080d7408
	.type Func_080d7408, %function
	.thumb_func
Func_080d7408:
	.incbin "baserom.gba", 0x000dc3fc, 0x00000028
	.section .rom.000dc424, "ax"
	.global Func_080d7430
	.type Func_080d7430, %function
	.thumb_func
Func_080d7430:
	.incbin "baserom.gba", 0x000dc424, 0x000000c0
	.section .rom.000dc518, "ax"
	.incbin "baserom.gba", 0x000dc518, 0x00000264
	.section .rom.000dc77c, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000dc77c, 0x000002f0
	.section .rom.000dca6c, "ax"
	.global BattleFx_InitializeSlots
	.type BattleFx_InitializeSlots, %function
	.thumb_func
BattleFx_InitializeSlots:
	.incbin "baserom.gba", 0x000dca6c, 0x0000003c
	.section .rom.000dcaa8, "ax"
	.global Func_080d7ab4
	.type Func_080d7ab4, %function
	.thumb_func
Func_080d7ab4:
	.incbin "baserom.gba", 0x000dcaa8, 0x00000c58
	.section .rom.000dd734, "ax"
	.incbin "baserom.gba", 0x000dd734, 0x0000023c
	.section .rom.000dd970, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000dd970, 0x000003ec
	.section .rom.000ddd5c, "ax"
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000ddd5c, 0x0000039c
	.section .rom.000de0f8, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000de0f8, 0x000001d0
	.section .rom.000de2c8, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000de2c8, 0x0000043c
	.section .rom.000de704, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000de704, 0x000003a0
	.section .rom.000deaa4, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000deaa4, 0x00000020
	.section .rom.000deac4, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000deac4, 0x00000038
	.section .rom.000deafc, "ax"
	.global Func_080d9b08
	.type Func_080d9b08, %function
	.thumb_func
Func_080d9b08:
	.incbin "baserom.gba", 0x000deafc, 0x00000390
	.section .rom.000dee8c, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000dee8c, 0x00000a70
	.section .rom.000df8fc, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000df8fc, 0x00000030
	.section .rom.000df92c, "ax"
	.global Func_080da938
	.type Func_080da938, %function
	.thumb_func
Func_080da938:
	.incbin "baserom.gba", 0x000df92c, 0x00000594
	.section .rom.000dfec0, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000dfec0, 0x000001e4
	.section .rom.000e00a4, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000e00a4, 0x00000394
	.section .rom.000e0438, "ax"
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000e0438, 0x00000048
	.section .rom.000e04aa, "ax"
	.incbin "baserom.gba", 0x000e04aa, 0x00000002
	.section .rom.000e04ac, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000e04ac, 0x000001b8
	.section .rom.000e0664, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e0664, 0x000001d8
	.section .rom.000e083c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000e083c, 0x0000003c
	.section .rom.000e0878, "ax"
	.global FieldEvent_RunTypeHandler
	.type FieldEvent_RunTypeHandler, %function
	.thumb_func
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000e0878, 0x00000098
	.section .rom.000e0910, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.type ObjectGroup_ApplyRandomChildValues, %function
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000e0910, 0x000000a4
	.section .rom.000e09b4, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000e09b4, 0x0000000c
	.section .rom.000e09c0, "ax"
	.global Func_080db9cc
	.type Func_080db9cc, %function
	.thumb_func
Func_080db9cc:
	.incbin "baserom.gba", 0x000e09c0, 0x0000030c
	.section .rom.000e0ccc, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000e0ccc, 0x00000070
	.section .rom.000e0d3c, "ax"
	.global Func_080dbd48
	.type Func_080dbd48, %function
	.thumb_func
Func_080dbd48:
	.incbin "baserom.gba", 0x000e0d3c, 0x00000080
	.section .rom.000e0dbc, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e0dbc, 0x0000000c
	.section .rom.000e0dc8, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e0dc8, 0x00000014
	.section .rom.000e0ddc, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000e0ddc, 0x0000000c
	.section .rom.000e0de8, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e0de8, 0x00000014
	.section .rom.000e0dfc, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000e0dfc, 0x0000003c
	.section .rom.000e0e38, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000e0e38, 0x00000274
	.section .rom.000e10ac, "ax"
	.global Func_080dc0b8
	.type Func_080dc0b8, %function
	.thumb_func
Func_080dc0b8:
	.incbin "baserom.gba", 0x000e10ac, 0x00000020
	.section .rom.000e10cc, "ax"
	.global Func_080dc0d8
	.type Func_080dc0d8, %function
	.thumb_func
Func_080dc0d8:
	.incbin "baserom.gba", 0x000e10cc, 0x00000034
	.section .rom.000e1100, "ax"
	.global Object_Spawn
	.type Object_Spawn, %function
	.thumb_func
Object_Spawn:
	.incbin "baserom.gba", 0x000e1100, 0x000000a4
	.section .rom.000e11a4, "ax"
	.global Func_080dc1b0
	.type Func_080dc1b0, %function
	.thumb_func
Func_080dc1b0:
	.incbin "baserom.gba", 0x000e11a4, 0x00000094
	.section .rom.000e1238, "ax"
	.global Field_BeginPaletteTransition
	.type Field_BeginPaletteTransition, %function
	.thumb_func
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000e1238, 0x00000050
	.section .rom.000e1288, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e1288, 0x000000f0
	.section .rom.000e1378, "ax"
	.global BattleFx_PrepareBufferInterpolation
	.type BattleFx_PrepareBufferInterpolation, %function
	.thumb_func
BattleFx_PrepareBufferInterpolation:
	.incbin "baserom.gba", 0x000e1378, 0x0000008c
	.section .rom.000e1404, "ax"
	.global Func_080dc410
	.type Func_080dc410, %function
	.thumb_func
Func_080dc410:
	.incbin "baserom.gba", 0x000e1404, 0x0000021c
	.section .rom.000e1620, "ax"
	.global Func_080dc62c
	.type Func_080dc62c, %function
	.thumb_func
Func_080dc62c:
	.incbin "baserom.gba", 0x000e1620, 0x000000ac
	.section .rom.000e16cc, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e16cc, 0x000000f4
	.section .rom.000e17c0, "ax"
	.global Func_080dc7cc
	.type Func_080dc7cc, %function
	.thumb_func
Func_080dc7cc:
	.incbin "baserom.gba", 0x000e17c0, 0x0000001c
	.section .rom.000e17dc, "ax"
	.global Func_080dc7e8
	.type Func_080dc7e8, %function
	.thumb_func
Func_080dc7e8:
	.incbin "baserom.gba", 0x000e17dc, 0x00000190
	.section .rom.000e196c, "ax"
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000e196c, 0x000000d8
	.section .rom.000e1a44, "ax"
	.global Func_080dca50
	.type Func_080dca50, %function
	.thumb_func
Func_080dca50:
	.incbin "baserom.gba", 0x000e1a44, 0x00000034
	.section .rom.000e1a78, "ax"
	.global Func_080dca84
	.type Func_080dca84, %function
	.thumb_func
Func_080dca84:
	.incbin "baserom.gba", 0x000e1a78, 0x00000058
	.section .rom.000e1ad0, "ax"
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000e1ad0, 0x00000578
	.section .rom.000e2048, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000e2048, 0x000004d4
	.section .rom.000e251c, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000e251c, 0x00000114
	.section .rom.000e2630, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000e2630, 0x0000002c
	.section .rom.000e265c, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000e265c, 0x00000f10
	.section .rom.000e3598, "ax"
	.incbin "baserom.gba", 0x000e3598, 0x00000714
	.section .rom.000e3cac, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000e3cac, 0x0000101c
	.section .rom.000e4cec, "ax"
	.incbin "baserom.gba", 0x000e4cec, 0x000005f4
	.section .rom.000e52fc, "ax"
	.global Func_080e0308
	.type Func_080e0308, %function
	.thumb_func
Func_080e0308:
	.incbin "baserom.gba", 0x000e52fc, 0x00000054
	.section .rom.000e5350, "ax"
	.global Func_080e035c
	.type Func_080e035c, %function
	.thumb_func
Func_080e035c:
	.incbin "baserom.gba", 0x000e5350, 0x00000050
	.section .rom.000e53b6, "ax"
	.incbin "baserom.gba", 0x000e53b6, 0x00000fba
	.section .rom.000e6370, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000e6370, 0x000000a4
	.section .rom.000e6414, "ax"
	.global Func_080e1420
	.type Func_080e1420, %function
	.thumb_func
Func_080e1420:
	.incbin "baserom.gba", 0x000e6414, 0x000001dc
	.section .rom.000e65f0, "ax"
	.global Func_080e15fc
	.type Func_080e15fc, %function
	.thumb_func
Func_080e15fc:
	.incbin "baserom.gba", 0x000e65f0, 0x00000054
	.section .rom.000e6644, "ax"
	.global Func_080e1650
	.type Func_080e1650, %function
	.thumb_func
Func_080e1650:
	.incbin "baserom.gba", 0x000e6644, 0x00000f98
	.section .rom.000e75dc, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000e75dc, 0x0000018c
	.section .rom.000e7768, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000e7768, 0x00000160
	.section .rom.000e78c8, "ax"
	.global Func_080e28d4
	.type Func_080e28d4, %function
	.thumb_func
Func_080e28d4:
	.incbin "baserom.gba", 0x000e78c8, 0x00000dc4
	.section .rom.000e868c, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000e868c, 0x00000148
	.section .rom.000e87d4, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000e87d4, 0x00000a64
	.section .rom.000e9238, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000e9238, 0x00002eb4
	.section .rom.000ec0ec, "ax"
	.global Func_080e70f8
	.type Func_080e70f8, %function
	.thumb_func
Func_080e70f8:
	.incbin "baserom.gba", 0x000ec0ec, 0x00000140
	.section .rom.000ec22c, "ax"
	.global Func_080e7238
	.type Func_080e7238, %function
	.thumb_func
Func_080e7238:
	.incbin "baserom.gba", 0x000ec22c, 0x000002a0
	.section .rom.000ec4cc, "ax"
	.global Func_080e74d8
	.type Func_080e74d8, %function
	.thumb_func
Func_080e74d8:
	.incbin "baserom.gba", 0x000ec4cc, 0x0000032c
	.section .rom.000ec7f8, "ax"
	.global Func_080e7804
	.type Func_080e7804, %function
	.thumb_func
Func_080e7804:
	.incbin "baserom.gba", 0x000ec7f8, 0x00002948
	.section .rom.000ef140, "ax"
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000ef140, 0x00000674
	.section .rom.000ef7b4, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000ef7b4, 0x00000114
	.section .rom.000ef8c8, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000ef8c8, 0x00000140
	.section .rom.000efa08, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000efa08, 0x0000015c
	.section .rom.000efb64, "ax"
	.global Func_080eab70
	.type Func_080eab70, %function
	.thumb_func
Func_080eab70:
	.incbin "baserom.gba", 0x000efb64, 0x00000028
	.section .rom.000efb8c, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000efb8c, 0x00000038
	.section .rom.000efbc4, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000efbc4, 0x00000118
	.section .rom.000efcdc, "ax"
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000efcdc, 0x00000114
	.section .rom.000efdf0, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000efdf0, 0x000000b8
	.section .rom.000efea8, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000efea8, 0x00000074
	.section .rom.000eff1c, "ax"
	.global Func_080eaf28
	.type Func_080eaf28, %function
	.thumb_func
Func_080eaf28:
	.incbin "baserom.gba", 0x000eff1c, 0x00000070
	.section .rom.000eff8c, "ax"
	.global Func_080eaf98
	.type Func_080eaf98, %function
	.thumb_func
Func_080eaf98:
	.incbin "baserom.gba", 0x000eff8c, 0x00000084
	.section .rom.000f0010, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000f0010, 0x0000027c
	.section .rom.000f028c, "ax"
	.global Func_080eb298
	.type Func_080eb298, %function
	.thumb_func
Func_080eb298:
	.incbin "baserom.gba", 0x000f028c, 0x00000030
	.section .rom.000f02bc, "ax"
	.global Func_080eb2c8
	.type Func_080eb2c8, %function
	.thumb_func
Func_080eb2c8:
	.incbin "baserom.gba", 0x000f02bc, 0x00000008
	.section .rom.000f02c4, "ax"
	.global Func_080eb2d0
	.type Func_080eb2d0, %function
	.thumb_func
Func_080eb2d0:
	.incbin "baserom.gba", 0x000f02c4, 0x00000690
	.section .rom.000f0954, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f0954, 0x000002d0
	.section .rom.000f0c24, "ax"
	.global Func_080ebc30
	.type Func_080ebc30, %function
	.thumb_func
Func_080ebc30:
	.incbin "baserom.gba", 0x000f0c24, 0x00000240
	.section .rom.000f0e64, "ax"
	.global BattleFx_HasReachedTarget
	.type BattleFx_HasReachedTarget, %function
	.thumb_func
BattleFx_HasReachedTarget:
	.incbin "baserom.gba", 0x000f0e64, 0x00000024
	.section .rom.000f0e9a, "ax"
	.incbin "baserom.gba", 0x000f0e9a, 0x00000002
	.section .rom.000f0e9c, "ax"
	.global Func_080ebea8
	.type Func_080ebea8, %function
	.thumb_func
Func_080ebea8:
	.incbin "baserom.gba", 0x000f0e9c, 0x0000000c
	.section .rom.000f0ea8, "ax"
	.global Func_080ebeb4
	.type Func_080ebeb4, %function
	.thumb_func
Func_080ebeb4:
	.incbin "baserom.gba", 0x000f0ea8, 0x00000014
	.section .rom.000f0ebc, "ax"
	.global Func_080ebec8
	.type Func_080ebec8, %function
	.thumb_func
Func_080ebec8:
	.incbin "baserom.gba", 0x000f0ebc, 0x000000a0
	.section .rom.000f0f5c, "ax"
	.global BattleFx_ClearOwnedSlot
	.type BattleFx_ClearOwnedSlot, %function
	.thumb_func
BattleFx_ClearOwnedSlot:
	.incbin "baserom.gba", 0x000f0f5c, 0x0000189c
	.section .rom.000f27f8, "ax"
	.global Func_080ed804
	.type Func_080ed804, %function
	.thumb_func
Func_080ed804:
	.incbin "baserom.gba", 0x000f27f8, 0x00003650
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f5e48, 0x000000d8
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f5f20, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000f679c, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f82c4, 0x00000030
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f82f4, 0x00000480
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f8774, 0x0000188c
	.section .rom.000fa028, "ax"
	.incbin "baserom.gba", 0x000fa028, 0x00000048
	.section .rom.000fa0c4, "ax"
	.incbin "baserom.gba", 0x000fa0c4, 0x00000058
	.section .rom.000fa170, "ax"
	.incbin "baserom.gba", 0x000fa170, 0x000004a0
	.section .rom.000fa656, "ax"
	.incbin "baserom.gba", 0x000fa656, 0x000001ea
	.section .rom.000fa840, "ax"
	.global UiIcon_CreateWithResourceVariant
	.type UiIcon_CreateWithResourceVariant, %function
	.thumb_func
UiIcon_CreateWithResourceVariant:
	.incbin "baserom.gba", 0x000fa840, 0x00000048
	.section .rom.000fa888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fa888, 0x00000390
	.section .rom.000fac94, "ax"
	.incbin "baserom.gba", 0x000fac94, 0x000002ac
	.section .rom.000faf9c, "ax"
	.incbin "baserom.gba", 0x000faf9c, 0x000001d4
	.section .rom.000fb222, "ax"
	.incbin "baserom.gba", 0x000fb222, 0x00000182
	.section .rom.000fb3b6, "ax"
	.incbin "baserom.gba", 0x000fb3b6, 0x000000ee
	.section .rom.000fb4a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000fb4a4, 0x00001854
	.section .rom.000fcd0a, "ax"
	.incbin "baserom.gba", 0x000fcd0a, 0x000003f2
	.section .rom.000fd15c, "ax"
	.incbin "baserom.gba", 0x000fd15c, 0x00000c40
	.section .rom.000fde04, "ax"
	.incbin "baserom.gba", 0x000fde04, 0x00000d54
	.section .rom.000feb58, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000feb58, 0x0000177c
	.section .rom.001002d4, "ax"
	.global Func_080fe184
	.type Func_080fe184, %function
	.thumb_func
Func_080fe184:
	.incbin "baserom.gba", 0x001002d4, 0x000000f0
	.section .rom.001003c4, "ax"
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x001003c4, 0x000023d0
	.section .rom.001027d0, "ax"
	.incbin "baserom.gba", 0x001027d0, 0x00004a40
	.section .rom.00107210, "ax"
	.global Menu_ReleaseEntryObjects
	.type Menu_ReleaseEntryObjects, %function
	.thumb_func
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x00107210, 0x000000f0
	.section .rom.00107300, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x00107300, 0x000000c4
	.section .rom.001073c4, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x001073c4, 0x00000040
	.section .rom.00107404, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x00107404, 0x00000054
	.section .rom.00107458, "ax"
	.global Func_0810526c
	.type Func_0810526c, %function
	.thumb_func
Func_0810526c:
	.incbin "baserom.gba", 0x00107458, 0x0000002c
	.section .rom.00107484, "ax"
	.global Func_081050b8
	.type Func_081050b8, %function
	.thumb_func
Func_081050b8:
	.incbin "baserom.gba", 0x00107484, 0x00000024
	.section .rom.001074a8, "ax"
	.global Func_081052ac
	.type Func_081052ac, %function
	.thumb_func
Func_081052ac:
	.incbin "baserom.gba", 0x001074a8, 0x00000230
	.section .rom.0010774e, "ax"
	.incbin "baserom.gba", 0x0010774e, 0x00000376
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00107ac4, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00107ac8, 0x0000007e
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x00107b46, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x00107b53, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00107b60, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00107b78, 0x00000488
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00108000, 0x00000088
	.section .rom.001080a8, "ax"
	.incbin "baserom.gba", 0x001080a8, 0x00000438
	.section .rom.001084f4, "ax"
	.incbin "baserom.gba", 0x001084f4, 0x00000640
	.section .rom.00108b70, "ax"
	.incbin "baserom.gba", 0x00108b70, 0x00000f28
	.section .rom.00109ad8, "ax"
	.incbin "baserom.gba", 0x00109ad8, 0x00000d30
	.section .rom.0010a808, "ax"
	.global Func_0810a804
	.type Func_0810a804, %function
	.thumb_func
Func_0810a804:
	.incbin "baserom.gba", 0x0010a808, 0x00000030
	.section .rom.0010a838, "ax"
	.global Func_0810a834
	.type Func_0810a834, %function
	.thumb_func
Func_0810a834:
	.incbin "baserom.gba", 0x0010a838, 0x00000018
	.section .rom.0010a850, "ax"
	.global Func_0810a84c
	.type Func_0810a84c, %function
	.thumb_func
Func_0810a84c:
	.incbin "baserom.gba", 0x0010a850, 0x00000018
	.section .rom.0010a8f0, "ax"
	.incbin "baserom.gba", 0x0010a8f0, 0x0000d710
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000060
	.section .rom.00118078, "ax"
	.global BattleMotion_ApproachTargetFar
	.type BattleMotion_ApproachTargetFar, %function
	.thumb_func
BattleMotion_ApproachTargetFar:
	.incbin "baserom.gba", 0x00118078, 0x00000010
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
	.global Func_0811a0b0
	.type Func_0811a0b0, %function
	.thumb_func
Func_0811a0b0:
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
	.incbin "baserom.gba", 0x0011bfe8, 0x000000a4
	.section .rom.0011c08c, "ax"
	.global Func_0811c074
	.type Func_0811c074, %function
	.thumb_func
Func_0811c074:
	.incbin "baserom.gba", 0x0011c08c, 0x00000184
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
	.global Func_081280fc
	.type Func_081280fc, %function
	.thumb_func
Func_081280fc:
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
	.incbin "baserom.gba", 0x00125bd0, 0x00000f2c
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
	.global Func_0812814c
	.type Func_0812814c, %function
	.thumb_func
Func_0812814c:
	.global Summon_IsEntryFlagged
	.type Summon_IsEntryFlagged, %function
	.thumb_func
Summon_IsEntryFlagged:
	.incbin "baserom.gba", 0x00128114, 0x00000050
	.section .rom.00128164, "ax"
	.global Func_08128174
	.type Func_08128174, %function
	.thumb_func
Func_08128174:
	.incbin "baserom.gba", 0x00128164, 0x00000064
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
	.incbin "baserom.gba", 0x00157636, 0x00002a62
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
	.section .rom.0068a0c0, "ax"
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a0c0, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x006927b8, 0x00005100
	.section .rom.006a444d, "ax"
	.incbin "baserom.gba", 0x006a444d, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a4450, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a4650, 0x00000828
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a4e78, 0x00000850
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a56c8, 0x000007ac
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a5e74, 0x0000056c
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a63e0, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a6c08, 0x0000186c
	.section .rom.006a99ab, "ax"
	.incbin "baserom.gba", 0x006a99ab, 0x00000001
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006a99ac, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b0fc8, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006ccf54, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd208, 0x00000f0c
	.section .rom.006d0b02, "ax"
	.incbin "baserom.gba", 0x006d0b02, 0x00000002
	.section .rom.006d4f46, "ax"
	.incbin "baserom.gba", 0x006d4f46, 0x00000002
	.section .rom.006e56b6, "ax"
	.incbin "baserom.gba", 0x006e56b6, 0x00000002
	.section .rom.006e8ca6, "ax"
	.incbin "baserom.gba", 0x006e8ca6, 0x00000002
	.section .rom.006f474a, "ax"
	.incbin "baserom.gba", 0x006f474a, 0x00000002
	.section .rom.00704dfa, "ax"
	.incbin "baserom.gba", 0x00704dfa, 0x00000002
	.section .rom.0070c89a, "ax"
	.incbin "baserom.gba", 0x0070c89a, 0x00000002
	.section .rom.007100da, "ax"
	.incbin "baserom.gba", 0x007100da, 0x00000002
	.section .rom.0071950e, "ax"
	.incbin "baserom.gba", 0x0071950e, 0x00000002
	.section .rom.007206ba, "ax"
	.incbin "baserom.gba", 0x007206ba, 0x00000002
	.section .rom.00728576, "ax"
	.incbin "baserom.gba", 0x00728576, 0x00000002
	.section .rom.0072d006, "ax"
	.incbin "baserom.gba", 0x0072d006, 0x00000002
	.section .rom.007312ba, "ax"
	.incbin "baserom.gba", 0x007312ba, 0x00000002
	.section .rom.00734c3a, "ax"
	.incbin "baserom.gba", 0x00734c3a, 0x00000002
	.section .rom.00741476, "ax"
	.incbin "baserom.gba", 0x00741476, 0x00000002
	.section .rom.0074cbb6, "ax"
	.incbin "baserom.gba", 0x0074cbb6, 0x00000002
	.section .rom.0074ff86, "ax"
	.incbin "baserom.gba", 0x0074ff86, 0x00000002
	.section .rom.00761ea2, "ax"
	.incbin "baserom.gba", 0x00761ea2, 0x00000002
	.section .rom.0076590a, "ax"
	.incbin "baserom.gba", 0x0076590a, 0x00000002
	.section .rom.007692fa, "ax"
	.incbin "baserom.gba", 0x007692fa, 0x00000002
	.section .rom.00779862, "ax"
	.incbin "baserom.gba", 0x00779862, 0x00000002
	.section .rom.0077d792, "ax"
	.incbin "baserom.gba", 0x0077d792, 0x00000002
	.section .rom.007811b6, "ax"
	.incbin "baserom.gba", 0x007811b6, 0x00000002
	.section .rom.007894d6, "ax"
	.incbin "baserom.gba", 0x007894d6, 0x00000002
	.section .rom.00791642, "ax"
	.incbin "baserom.gba", 0x00791642, 0x00000002
	.section .rom.0079e90e, "ax"
	.incbin "baserom.gba", 0x0079e90e, 0x00000002
	.section .rom.007a27ca, "ax"
	.incbin "baserom.gba", 0x007a27ca, 0x00000002
	.section .rom.007a65c6, "ax"
	.incbin "baserom.gba", 0x007a65c6, 0x00000002
	.section .rom.007aaa96, "ax"
	.incbin "baserom.gba", 0x007aaa96, 0x00000002
	.section .rom.007b2472, "ax"
	.incbin "baserom.gba", 0x007b2472, 0x00000002
	.section .rom.007b6a9e, "ax"
	.incbin "baserom.gba", 0x007b6a9e, 0x00000002
	.section .rom.007be71a, "ax"
	.incbin "baserom.gba", 0x007be71a, 0x00000002
	.section .rom.007c2316, "ax"
	.incbin "baserom.gba", 0x007c2316, 0x00000002
	.section .rom.007c5cc6, "ax"
	.incbin "baserom.gba", 0x007c5cc6, 0x00000002
	.section .rom.007ca116, "ax"
	.incbin "baserom.gba", 0x007ca116, 0x00000002
	.section .rom.007cdd0e, "ax"
	.incbin "baserom.gba", 0x007cdd0e, 0x00000002
	.section .rom.007d996e, "ax"
	.incbin "baserom.gba", 0x007d996e, 0x00000002
	.section .rom.007dd5be, "ax"
	.incbin "baserom.gba", 0x007dd5be, 0x00000002
	.section .rom.007e5b3e, "ax"
	.incbin "baserom.gba", 0x007e5b3e, 0x00000002
	.section .rom.007f218e, "ax"
	.incbin "baserom.gba", 0x007f218e, 0x00000002
	.section .rom.007f8c42, "ax"
	.incbin "baserom.gba", 0x007f8c42, 0x00000002
	.section .rom.00801f0a, "ax"
	.incbin "baserom.gba", 0x00801f0a, 0x00000002
	.section .rom.0080992e, "ax"
	.incbin "baserom.gba", 0x0080992e, 0x00000002
	.section .rom.0081f5ae, "ax"
	.incbin "baserom.gba", 0x0081f5ae, 0x00000002
	.section .rom.008289f2, "ax"
	.incbin "baserom.gba", 0x008289f2, 0x00000002
	.section .rom.00831c0e, "ax"
	.incbin "baserom.gba", 0x00831c0e, 0x00000002
	.section .rom.0083f536, "ax"
	.incbin "baserom.gba", 0x0083f536, 0x00000002
	.section .rom.0084538a, "ax"
	.incbin "baserom.gba", 0x0084538a, 0x00000002
	.section .rom.0084b905, "ax"
	.incbin "baserom.gba", 0x0084b905, 0x00000003
	.section .rom.0084d257, "ax"
	.incbin "baserom.gba", 0x0084d257, 0x00000001
	.section .rom.00850075, "ax"
	.incbin "baserom.gba", 0x00850075, 0x00000003
	.section .rom.00853f8a, "ax"
	.incbin "baserom.gba", 0x00853f8a, 0x00000002
	.section .rom.00857133, "ax"
	.incbin "baserom.gba", 0x00857133, 0x00000001
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x00857134, 0x000009bc
	.section .rom.008580d7, "ax"
	.incbin "baserom.gba", 0x008580d7, 0x00000001
	.section .rom.00858785, "ax"
	.incbin "baserom.gba", 0x00858785, 0x00000003
	.section .rom.00858a52, "ax"
	.incbin "baserom.gba", 0x00858a52, 0x00000002
	.section .rom.008595ad, "ax"
	.incbin "baserom.gba", 0x008595ad, 0x00000003
	.section .rom.0085a16b, "ax"
	.incbin "baserom.gba", 0x0085a16b, 0x00000001
	.section .rom.0085a25e, "ax"
	.incbin "baserom.gba", 0x0085a25e, 0x00000002
	.section .rom.0085a3ef, "ax"
	.incbin "baserom.gba", 0x0085a3ef, 0x00000001
	.section .rom.0085a8ce, "ax"
	.incbin "baserom.gba", 0x0085a8ce, 0x00000002
	.section .rom.0085bb6b, "ax"
	.incbin "baserom.gba", 0x0085bb6b, 0x00000001
	.section .rom.0085e43f, "ax"
	.incbin "baserom.gba", 0x0085e43f, 0x00000001
	.section .rom.0085ebba, "ax"
	.incbin "baserom.gba", 0x0085ebba, 0x00000002
	.section .rom.008601ff, "ax"
	.incbin "baserom.gba", 0x008601ff, 0x00000001
	.section .rom.00860729, "ax"
	.incbin "baserom.gba", 0x00860729, 0x00000003
	.section .rom.00860f32, "ax"
	.incbin "baserom.gba", 0x00860f32, 0x00000002
	.section .rom.008614ab, "ax"
	.incbin "baserom.gba", 0x008614ab, 0x00000001
	.section .rom.00862bea, "ax"
	.incbin "baserom.gba", 0x00862bea, 0x00000002
	.section .rom.00865ab3, "ax"
	.incbin "baserom.gba", 0x00865ab3, 0x00000001
	.section .rom.00865d65, "ax"
	.incbin "baserom.gba", 0x00865d65, 0x00000003
	.section .rom.00867147, "ax"
	.incbin "baserom.gba", 0x00867147, 0x00000001
	.section .rom.0086b983, "ax"
	.incbin "baserom.gba", 0x0086b983, 0x00000001
	.section .rom.0086f129, "ax"
	.incbin "baserom.gba", 0x0086f129, 0x00000003
	.section .rom.00870e4e, "ax"
	.incbin "baserom.gba", 0x00870e4e, 0x00000002
	.section .rom.00872d4d, "ax"
	.incbin "baserom.gba", 0x00872d4d, 0x00000003
	.section .rom.008746fb, "ax"
	.incbin "baserom.gba", 0x008746fb, 0x00000001
	.section .rom.00875c73, "ax"
	.incbin "baserom.gba", 0x00875c73, 0x00000001
	.section .rom.008798a6, "ax"
	.incbin "baserom.gba", 0x008798a6, 0x00000002
	.section .rom.0087a404, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a404, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087c054, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c494, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c6e0, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c878, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087d0a4, 0x00000e98
	.section .rom.0087e573, "ax"
	.incbin "baserom.gba", 0x0087e573, 0x00000001
	.section .rom.008806dd, "ax"
	.incbin "baserom.gba", 0x008806dd, 0x00000003
	.section .rom.008807e8, "ax"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x008807e8, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x00880a34, 0x00000184
	.section .rom.00881471, "ax"
	.incbin "baserom.gba", 0x00881471, 0x00000003
	.section .rom.00883055, "ax"
	.incbin "baserom.gba", 0x00883055, 0x00000003
	.section .rom.00884bb2, "ax"
	.incbin "baserom.gba", 0x00884bb2, 0x00000002
	.section .rom.00884ff1, "ax"
	.incbin "baserom.gba", 0x00884ff1, 0x00000003
	.section .rom.00885406, "ax"
	.incbin "baserom.gba", 0x00885406, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00885408, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x00885740, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088680c, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x00886af4, 0x00000154
	.section .rom.00889153, "ax"
	.incbin "baserom.gba", 0x00889153, 0x00000001
	.section .rom.00889b0f, "ax"
	.incbin "baserom.gba", 0x00889b0f, 0x00000001
	.section .rom.0088ac52, "ax"
	.incbin "baserom.gba", 0x0088ac52, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088ac54, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b8d0, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b8fc, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b928, 0x000007c0
	.section .rom.0088d663, "ax"
	.incbin "baserom.gba", 0x0088d663, 0x00000001
	.section .rom.0088d98d, "ax"
	.incbin "baserom.gba", 0x0088d98d, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d990, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088deb8, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e3e8, 0x000005f0
	.section .rom.0088f161, "ax"
	.incbin "baserom.gba", 0x0088f161, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088f164, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f364, 0x00000420
	.section .rom.0088fdad, "ax"
	.incbin "baserom.gba", 0x0088fdad, 0x00000003
	.section .rom.008901b7, "ax"
	.incbin "baserom.gba", 0x008901b7, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x008901b8, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x00890414, 0x0000057c
	.section .rom.00890c39, "ax"
	.incbin "baserom.gba", 0x00890c39, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890c3c, 0x0000095c
	.section .rom.00891c55, "ax"
	.incbin "baserom.gba", 0x00891c55, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891c58, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894708, 0x000011cc
	.section .rom.00895f33, "ax"
	.incbin "baserom.gba", 0x00895f33, 0x00000001
	.section .rom.00896f6d, "ax"
	.incbin "baserom.gba", 0x00896f6d, 0x00000003
	.section .rom.008975c3, "ax"
	.incbin "baserom.gba", 0x008975c3, 0x00000001
	.section .rom.00897c41, "ax"
	.incbin "baserom.gba", 0x00897c41, 0x00000003
	.section .rom.00898261, "ax"
	.incbin "baserom.gba", 0x00898261, 0x00000003
	.section .rom.0089962e, "ax"
	.incbin "baserom.gba", 0x0089962e, 0x00000002
	.section .rom.0089a672, "ax"
	.incbin "baserom.gba", 0x0089a672, 0x00000002
	.section .rom.0089b0fb, "ax"
	.incbin "baserom.gba", 0x0089b0fb, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089b0fc, 0x000002cc
	.section .rom.0089c101, "ax"
	.incbin "baserom.gba", 0x0089c101, 0x00000003
	.section .rom.0089d33d, "ax"
	.incbin "baserom.gba", 0x0089d33d, 0x00000003
	.section .rom.0089dc87, "ax"
	.incbin "baserom.gba", 0x0089dc87, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089dc88, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e2e4, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e810, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a0acc, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a2260, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2944, 0x00001f4c
	.section .rom.008a4dc4, "ax"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4dc4, 0x000011d8
	.section .rom.008a648a, "ax"
	.incbin "baserom.gba", 0x008a648a, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a648c, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6ad4, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a76f8, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a7abc, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7c84, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a81d0, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a851c, 0x0000076c
	.section .rom.008a92cb, "ax"
	.incbin "baserom.gba", 0x008a92cb, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a92cc, 0x000000a8
	.section .rom.008a968a, "ax"
	.incbin "baserom.gba", 0x008a968a, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a968c, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa034, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa32c, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aae5c, 0x00000100
	.section .rom.008abbcd, "ax"
	.incbin "baserom.gba", 0x008abbcd, 0x00000003
	.section .rom.008ac415, "ax"
	.incbin "baserom.gba", 0x008ac415, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac418, 0x00001200
	.section .rom.008ae4d7, "ax"
	.incbin "baserom.gba", 0x008ae4d7, 0x00000001
	.section .rom.008aefb3, "ax"
	.incbin "baserom.gba", 0x008aefb3, 0x00000001
	.section .rom.008af676, "ax"
	.incbin "baserom.gba", 0x008af676, 0x00000002
	.section .rom.008af95d, "ax"
	.incbin "baserom.gba", 0x008af95d, 0x00000003
	.section .rom.008b0cc3, "ax"
	.incbin "baserom.gba", 0x008b0cc3, 0x00000001
	.section .rom.008b10ad, "ax"
	.incbin "baserom.gba", 0x008b10ad, 0x00000003
	.section .rom.008b147f, "ax"
	.incbin "baserom.gba", 0x008b147f, 0x00000001
	.section .rom.008b1d1f, "ax"
	.incbin "baserom.gba", 0x008b1d1f, 0x00000001
	.section .rom.008b21c2, "ax"
	.incbin "baserom.gba", 0x008b21c2, 0x00000002
	.section .rom.008b337b, "ax"
	.incbin "baserom.gba", 0x008b337b, 0x00000001
	.section .rom.008b37d4, "ax"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b37d4, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b4840, 0x00000fc8
	.section .rom.008b5a62, "ax"
	.incbin "baserom.gba", 0x008b5a62, 0x00000002
	.section .rom.008b74bd, "ax"
	.incbin "baserom.gba", 0x008b74bd, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b74c0, 0x000001b4
	.section .rom.008b7a09, "ax"
	.incbin "baserom.gba", 0x008b7a09, 0x00000003
	.section .rom.008b9792, "ax"
	.incbin "baserom.gba", 0x008b9792, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9794, 0x00000278
	.section .rom.008b9ed2, "ax"
	.incbin "baserom.gba", 0x008b9ed2, 0x00000002
	.section .rom.008bbaa7, "ax"
	.incbin "baserom.gba", 0x008bbaa7, 0x00000001
	.section .rom.008bd6a5, "ax"
	.incbin "baserom.gba", 0x008bd6a5, 0x00000003
	.section .rom.008bd8c6, "ax"
	.incbin "baserom.gba", 0x008bd8c6, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd8c8, 0x0000043c
	.section .rom.008bde15, "ax"
	.incbin "baserom.gba", 0x008bde15, 0x00000003
	.section .rom.008be3f7, "ax"
	.incbin "baserom.gba", 0x008be3f7, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be3f8, 0x000004a0
	.section .rom.008bf5aa, "ax"
	.incbin "baserom.gba", 0x008bf5aa, 0x00000002
	.section .rom.008bfaec, "ax"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bfaec, 0x00000840
	.section .rom.008c06bb, "ax"
	.incbin "baserom.gba", 0x008c06bb, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c06bc, 0x00001258
	.section .rom.008c2402, "ax"
	.incbin "baserom.gba", 0x008c2402, 0x00000002
	.section .rom.008c31d9, "ax"
	.incbin "baserom.gba", 0x008c31d9, 0x00000003
	.section .rom.008c3a06, "ax"
	.incbin "baserom.gba", 0x008c3a06, 0x00000002
	.section .rom.008c3df8, "ax"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c3df8, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c3fb4, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c4170, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4ab0, 0x00000418
	.section .rom.008c5839, "ax"
	.incbin "baserom.gba", 0x008c5839, 0x00000003
	.section .rom.008c5be7, "ax"
	.incbin "baserom.gba", 0x008c5be7, 0x00000001
	.section .rom.008c7b8b, "ax"
	.incbin "baserom.gba", 0x008c7b8b, 0x00000001
	.section .rom.008c8927, "ax"
	.incbin "baserom.gba", 0x008c8927, 0x00000001
	.section .rom.008c8b43, "ax"
	.incbin "baserom.gba", 0x008c8b43, 0x00000001
	.section .rom.008c8e3f, "ax"
	.incbin "baserom.gba", 0x008c8e3f, 0x00000001
	.section .rom.008c91e0, "ax"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c91e0, 0x000016b0
	.section .rom.008cafed, "ax"
	.incbin "baserom.gba", 0x008cafed, 0x00000003
	.section .rom.008cbdbb, "ax"
	.incbin "baserom.gba", 0x008cbdbb, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbdbc, 0x00000c74
	.section .rom.008cd025, "ax"
	.incbin "baserom.gba", 0x008cd025, 0x00000003
	.section .rom.008cd57f, "ax"
	.incbin "baserom.gba", 0x008cd57f, 0x00000001
	.section .rom.008cee69, "ax"
	.incbin "baserom.gba", 0x008cee69, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008cee6c, 0x000008a0
	.section .rom.008cfae7, "ax"
	.incbin "baserom.gba", 0x008cfae7, 0x00000001
	.section .rom.008cfd71, "ax"
	.incbin "baserom.gba", 0x008cfd71, 0x00000003
	.section .rom.008d0117, "ax"
	.incbin "baserom.gba", 0x008d0117, 0x00000001
	.section .rom.008d0372, "ax"
	.incbin "baserom.gba", 0x008d0372, 0x00000002
	.section .rom.008d072a, "ax"
	.incbin "baserom.gba", 0x008d072a, 0x00000002
	.section .rom.008d1c13, "ax"
	.incbin "baserom.gba", 0x008d1c13, 0x00000001
	.section .rom.008d31d6, "ax"
	.incbin "baserom.gba", 0x008d31d6, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d31d8, 0x00000a6c
	.section .rom.008d4a1a, "ax"
	.incbin "baserom.gba", 0x008d4a1a, 0x00000002
	.section .rom.008d4d9d, "ax"
	.incbin "baserom.gba", 0x008d4d9d, 0x00000003
	.section .rom.008d5a4d, "ax"
	.incbin "baserom.gba", 0x008d5a4d, 0x00000003
	.section .rom.008d6595, "ax"
	.incbin "baserom.gba", 0x008d6595, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6598, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d6730, 0x0000088c
	.section .rom.008d7b8a, "ax"
	.incbin "baserom.gba", 0x008d7b8a, 0x00000002
	.section .rom.008d80a2, "ax"
	.incbin "baserom.gba", 0x008d80a2, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d80a4, 0x00000624
	.section .rom.008d8ae9, "ax"
	.incbin "baserom.gba", 0x008d8ae9, 0x00000003
	.section .rom.008d8d83, "ax"
	.incbin "baserom.gba", 0x008d8d83, 0x00000001
	.section .rom.008da284, "ax"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da284, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da720, 0x0000198c
	.section .rom.008dc496, "ax"
	.incbin "baserom.gba", 0x008dc496, 0x00000002
	.section .rom.008de40b, "ax"
	.incbin "baserom.gba", 0x008de40b, 0x00000001
	.section .rom.008de8db, "ax"
	.incbin "baserom.gba", 0x008de8db, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de8dc, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008def70, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df9a4, 0x00000bfc
	.section .rom.008e0d75, "ax"
	.incbin "baserom.gba", 0x008e0d75, 0x00000003
	.section .rom.008e1846, "ax"
	.incbin "baserom.gba", 0x008e1846, 0x00000002
	.section .rom.008e2383, "ax"
	.incbin "baserom.gba", 0x008e2383, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2384, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e29c4, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3f4c, 0x00000064
	.section .rom.008e431b, "ax"
	.incbin "baserom.gba", 0x008e431b, 0x00000001
	.section .rom.008e48b9, "ax"
	.incbin "baserom.gba", 0x008e48b9, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e48bc, 0x00000204
	.section .rom.008e4df9, "ax"
	.incbin "baserom.gba", 0x008e4df9, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4dfc, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5e14, 0x0000166c
	.section .rom.008e941e, "ax"
	.incbin "baserom.gba", 0x008e941e, 0x00000002
	.section .rom.008e9bc1, "ax"
	.incbin "baserom.gba", 0x008e9bc1, 0x00000003
	.section .rom.008eabd8, "ax"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eabd8, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eaf54, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb384, 0x000010cc
	.section .rom.008ec4d4, "ax"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec4d4, 0x000004e8
	.section .rom.008ecac4, "ax"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ecac4, 0x000006b8
	.section .rom.008eda8e, "ax"
	.incbin "baserom.gba", 0x008eda8e, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008eda90, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef5c4, 0x00001050
	.section .rom.008f16b9, "ax"
	.incbin "baserom.gba", 0x008f16b9, 0x00000003
	.section .rom.008f18b8, "ax"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f18b8, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00933120, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c4e4, 0x00000028
	.section .rom.0093c6f9, "ax"
	.incbin "baserom.gba", 0x0093c6f9, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c6fc, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c850, 0x000004b8
	.section .rom.0093e472, "ax"
	.incbin "baserom.gba", 0x0093e472, 0x00000002
	.section .rom.0093f9a9, "ax"
	.incbin "baserom.gba", 0x0093f9a9, 0x00000003
	.section .rom.009417ff, "ax"
	.incbin "baserom.gba", 0x009417ff, 0x00000001
	.section .rom.00943913, "ax"
	.incbin "baserom.gba", 0x00943913, 0x00000001
	.section .rom.00943aec, "ax"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x00943aec, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943cd0, 0x000003c0
	.section .rom.0094670b, "ax"
	.incbin "baserom.gba", 0x0094670b, 0x00000001
	.section .rom.00947113, "ax"
	.incbin "baserom.gba", 0x00947113, 0x00000001
	.section .rom.00949e32, "ax"
	.incbin "baserom.gba", 0x00949e32, 0x00000002
	.section .rom.0094a004, "ax"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x0094a004, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x0094a00c, 0x00000460
	.section .rom.0094ba5e, "ax"
	.incbin "baserom.gba", 0x0094ba5e, 0x00000002
	.section .rom.0094cece, "ax"
	.incbin "baserom.gba", 0x0094cece, 0x00000002
	.section .rom.0094d7c2, "ax"
	.incbin "baserom.gba", 0x0094d7c2, 0x00000002
	.section .rom.0094e0e9, "ax"
	.incbin "baserom.gba", 0x0094e0e9, 0x00000003
	.section .rom.0094efda, "ax"
	.incbin "baserom.gba", 0x0094efda, 0x00000002
	.section .rom.0094fbc7, "ax"
	.incbin "baserom.gba", 0x0094fbc7, 0x00000001
	.section .rom.0094fda2, "ax"
	.incbin "baserom.gba", 0x0094fda2, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fda4, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094ff94, 0x000002d8
	.section .rom.00950b02, "ax"
	.incbin "baserom.gba", 0x00950b02, 0x00000002
	.section .rom.00952f3d, "ax"
	.incbin "baserom.gba", 0x00952f3d, 0x00000003
	.section .rom.0095350f, "ax"
	.incbin "baserom.gba", 0x0095350f, 0x00000001
	.section .rom.009539af, "ax"
	.incbin "baserom.gba", 0x009539af, 0x00000001
	.section .rom.00953d02, "ax"
	.incbin "baserom.gba", 0x00953d02, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953d04, 0x000002f8
	.section .rom.009540d5, "ax"
	.incbin "baserom.gba", 0x009540d5, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x009540d8, 0x000002f0
	.section .rom.0095445a, "ax"
	.incbin "baserom.gba", 0x0095445a, 0x00000002
	.section .rom.0095500d, "ax"
	.incbin "baserom.gba", 0x0095500d, 0x00000003
	.section .rom.00955cba, "ax"
	.incbin "baserom.gba", 0x00955cba, 0x00000002
	.section .rom.009567be, "ax"
	.incbin "baserom.gba", 0x009567be, 0x00000002
	.section .rom.009576fd, "ax"
	.incbin "baserom.gba", 0x009576fd, 0x00000003
	.section .rom.00957f39, "ax"
	.incbin "baserom.gba", 0x00957f39, 0x00000003
	.section .rom.009593de, "ax"
	.incbin "baserom.gba", 0x009593de, 0x00000002
	.section .rom.00959e96, "ax"
	.incbin "baserom.gba", 0x00959e96, 0x00000002
	.section .rom.0095a1c1, "ax"
	.incbin "baserom.gba", 0x0095a1c1, 0x00000003
	.section .rom.0095af77, "ax"
	.incbin "baserom.gba", 0x0095af77, 0x00000001
	.section .rom.0095b778, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b778, 0x00000400
	.section .rom.00967f32, "ax"
	.incbin "baserom.gba", 0x00967f32, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f34, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968034, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009684fc, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x00968764, 0x000001c8
	.section .rom.00968d93, "ax"
	.incbin "baserom.gba", 0x00968d93, 0x00000001
	.section .rom.00968f9f, "ax"
	.incbin "baserom.gba", 0x00968f9f, 0x00000001
	.section .rom.009691a3, "ax"
	.incbin "baserom.gba", 0x009691a3, 0x00000001
	.section .rom.00969232, "ax"
	.incbin "baserom.gba", 0x00969232, 0x00000002
	.section .rom.00969714, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x00969714, 0x00000070
	.section .rom.009697df, "ax"
	.incbin "baserom.gba", 0x009697df, 0x00000001
	.section .rom.00969812, "ax"
	.incbin "baserom.gba", 0x00969812, 0x00000002
	.section .rom.009699de, "ax"
	.incbin "baserom.gba", 0x009699de, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x009699e0, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969c2c, 0x000000e4
	.section .rom.00969dc9, "ax"
	.incbin "baserom.gba", 0x00969dc9, 0x00000003
	.section .rom.00969f9e, "ax"
	.incbin "baserom.gba", 0x00969f9e, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x00969fa0, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a038, 0x00000024
	.section .rom.0096a1ae, "ax"
	.incbin "baserom.gba", 0x0096a1ae, 0x00000002
	.section .rom.0096a291, "ax"
	.incbin "baserom.gba", 0x0096a291, 0x00000003
	.section .rom.0096a361, "ax"
	.incbin "baserom.gba", 0x0096a361, 0x00000003
	.section .rom.0096a487, "ax"
	.incbin "baserom.gba", 0x0096a487, 0x00000001
	.section .rom.0096a4b6, "ax"
	.incbin "baserom.gba", 0x0096a4b6, 0x00000002
	.section .rom.0096a71a, "ax"
	.incbin "baserom.gba", 0x0096a71a, 0x00000002
	.section .rom.0096a7b7, "ax"
	.incbin "baserom.gba", 0x0096a7b7, 0x00000001
	.section .rom.0096a81c, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a81c, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096ac1c, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096accc, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b22c, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b2a0, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b2e8, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b338, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b388, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b3e0, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b438, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b478, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b4bc, 0x00000054
	.section .rom.00970747, "ax"
	.incbin "baserom.gba", 0x00970747, 0x00000001
	.section .rom.00973692, "ax"
	.incbin "baserom.gba", 0x00973692, 0x00000002
	.section .rom.009772b5, "ax"
	.incbin "baserom.gba", 0x009772b5, 0x00000003
	.section .rom.00979501, "ax"
	.incbin "baserom.gba", 0x00979501, 0x00000003
	.section .rom.0097ea9d, "ax"
	.incbin "baserom.gba", 0x0097ea9d, 0x00000003
	.section .rom.00982317, "ax"
	.incbin "baserom.gba", 0x00982317, 0x00000001
	.section .rom.00988181, "ax"
	.incbin "baserom.gba", 0x00988181, 0x00000003
	.section .rom.00989255, "ax"
	.incbin "baserom.gba", 0x00989255, 0x00000003
	.section .rom.0098a2c1, "ax"
	.incbin "baserom.gba", 0x0098a2c1, 0x00000003
	.section .rom.0098f91b, "ax"
	.incbin "baserom.gba", 0x0098f91b, 0x00000001
	.section .rom.00992a5d, "ax"
	.incbin "baserom.gba", 0x00992a5d, 0x00000003
	.section .rom.00994afd, "ax"
	.incbin "baserom.gba", 0x00994afd, 0x00000003
	.section .rom.00996e1d, "ax"
	.incbin "baserom.gba", 0x00996e1d, 0x00000003
	.section .rom.00999a96, "ax"
	.incbin "baserom.gba", 0x00999a96, 0x00000002
	.section .rom.0099b991, "ax"
	.incbin "baserom.gba", 0x0099b991, 0x00000003
	.section .rom.009a3ed5, "ax"
	.incbin "baserom.gba", 0x009a3ed5, 0x00000003
	.section .rom.009acb57, "ax"
	.incbin "baserom.gba", 0x009acb57, 0x00000001
	.section .rom.009af6d1, "ax"
	.incbin "baserom.gba", 0x009af6d1, 0x00000003
	.section .rom.009b610e, "ax"
	.incbin "baserom.gba", 0x009b610e, 0x00000002
	.section .rom.009b8c02, "ax"
	.incbin "baserom.gba", 0x009b8c02, 0x00000002
	.section .rom.009ba6f2, "ax"
	.incbin "baserom.gba", 0x009ba6f2, 0x00000002
	.section .rom.009bd008, "ax"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bd008, 0x00003440
	.section .rom.009c4dee, "ax"
	.incbin "baserom.gba", 0x009c4dee, 0x00000002
	.section .rom.009c5d8a, "ax"
	.incbin "baserom.gba", 0x009c5d8a, 0x00000002
	.section .rom.009c84b5, "ax"
	.incbin "baserom.gba", 0x009c84b5, 0x00000003
	.section .rom.009c9bca, "ax"
	.incbin "baserom.gba", 0x009c9bca, 0x00000002
	.section .rom.009d23e3, "ax"
	.incbin "baserom.gba", 0x009d23e3, 0x00000001
	.section .rom.009d66ab, "ax"
	.incbin "baserom.gba", 0x009d66ab, 0x00000001
	.section .rom.009ddee5, "ax"
	.incbin "baserom.gba", 0x009ddee5, 0x00000003
	.section .rom.009df6ca, "ax"
	.incbin "baserom.gba", 0x009df6ca, 0x00000002
	.section .rom.009e2461, "ax"
	.incbin "baserom.gba", 0x009e2461, 0x00000003
	.section .rom.009e34ab, "ax"
	.incbin "baserom.gba", 0x009e34ab, 0x00000001
	.section .rom.009eba3b, "ax"
	.incbin "baserom.gba", 0x009eba3b, 0x00000001
	.section .rom.009ed835, "ax"
	.incbin "baserom.gba", 0x009ed835, 0x00000003
	.section .rom.009eebfb, "ax"
	.incbin "baserom.gba", 0x009eebfb, 0x00000001
	.section .rom.009f5afd, "ax"
	.incbin "baserom.gba", 0x009f5afd, 0x00000003
	.section .rom.009f7dfe, "ax"
	.incbin "baserom.gba", 0x009f7dfe, 0x00000002
	.section .rom.009fd04b, "ax"
	.incbin "baserom.gba", 0x009fd04b, 0x00000001
	.section .rom.00a0244a, "ax"
	.incbin "baserom.gba", 0x00a0244a, 0x00000002
	.section .rom.00a045fe, "ax"
	.incbin "baserom.gba", 0x00a045fe, 0x00000002
	.section .rom.00a08765, "ax"
	.incbin "baserom.gba", 0x00a08765, 0x00000003
	.section .rom.00a0b98a, "ax"
	.incbin "baserom.gba", 0x00a0b98a, 0x00000002
	.section .rom.00a0d08e, "ax"
	.incbin "baserom.gba", 0x00a0d08e, 0x00000002
	.section .rom.00a13c76, "ax"
	.incbin "baserom.gba", 0x00a13c76, 0x00000002
	.section .rom.00a20256, "ax"
	.incbin "baserom.gba", 0x00a20256, 0x00000002
	.section .rom.00a232ea, "ax"
	.incbin "baserom.gba", 0x00a232ea, 0x00000002
	.section .rom.00a25b55, "ax"
	.incbin "baserom.gba", 0x00a25b55, 0x00000003
	.section .rom.00a27646, "ax"
	.incbin "baserom.gba", 0x00a27646, 0x00000002
	.section .rom.00a2845e, "ax"
	.incbin "baserom.gba", 0x00a2845e, 0x00000002
	.section .rom.00a29149, "ax"
	.incbin "baserom.gba", 0x00a29149, 0x00000003
	.section .rom.00a32606, "ax"
	.incbin "baserom.gba", 0x00a32606, 0x00000002
	.section .rom.00a33311, "ax"
	.incbin "baserom.gba", 0x00a33311, 0x00000003
	.section .rom.00a34575, "ax"
	.incbin "baserom.gba", 0x00a34575, 0x00000003
	.section .rom.00a354a7, "ax"
	.incbin "baserom.gba", 0x00a354a7, 0x00000001
	.section .rom.00a36117, "ax"
	.incbin "baserom.gba", 0x00a36117, 0x00000001
	.section .rom.00a36d2f, "ax"
	.incbin "baserom.gba", 0x00a36d2f, 0x00000001
	.section .rom.00a374a3, "ax"
	.incbin "baserom.gba", 0x00a374a3, 0x00000001
	.section .rom.00a38d33, "ax"
	.incbin "baserom.gba", 0x00a38d33, 0x00000001
	.section .rom.00a39701, "ax"
	.incbin "baserom.gba", 0x00a39701, 0x00000003
	.section .rom.00a3a31f, "ax"
	.incbin "baserom.gba", 0x00a3a31f, 0x00000001
	.section .rom.00a3c9d3, "ax"
	.incbin "baserom.gba", 0x00a3c9d3, 0x00000001
	.section .rom.00a3f0e6, "ax"
	.incbin "baserom.gba", 0x00a3f0e6, 0x00000002
	.section .rom.00a4403b, "ax"
	.incbin "baserom.gba", 0x00a4403b, 0x00000001
	.section .rom.00a48365, "ax"
	.incbin "baserom.gba", 0x00a48365, 0x00000003
	.section .rom.00a48f52, "ax"
	.incbin "baserom.gba", 0x00a48f52, 0x00000002
	.section .rom.00a4db07, "ax"
	.incbin "baserom.gba", 0x00a4db07, 0x00000001
	.section .rom.00a4fe71, "ax"
	.incbin "baserom.gba", 0x00a4fe71, 0x00000003
	.section .rom.00a51813, "ax"
	.incbin "baserom.gba", 0x00a51813, 0x00000001
	.section .rom.00a560d5, "ax"
	.incbin "baserom.gba", 0x00a560d5, 0x00000003
	.section .rom.00a594e5, "ax"
	.incbin "baserom.gba", 0x00a594e5, 0x00000003
	.section .rom.00a5d5ad, "ax"
	.incbin "baserom.gba", 0x00a5d5ad, 0x00000003
	.section .rom.00a60c76, "ax"
	.incbin "baserom.gba", 0x00a60c76, 0x00000002
	.section .rom.00a68c47, "ax"
	.incbin "baserom.gba", 0x00a68c47, 0x00000001
	.section .rom.00a6ff57, "ax"
	.incbin "baserom.gba", 0x00a6ff57, 0x00000001
	.section .rom.00a74ed7, "ax"
	.incbin "baserom.gba", 0x00a74ed7, 0x00000001
	.section .rom.00a79959, "ax"
	.incbin "baserom.gba", 0x00a79959, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a7995c, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a79968, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79ab8, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79bf8, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a79d38, 0x00000140
	.section .rom.00a7b0d5, "ax"
	.incbin "baserom.gba", 0x00a7b0d5, 0x00000003
	.section .rom.00a7b2a6, "ax"
	.incbin "baserom.gba", 0x00a7b2a6, 0x00000002
	.section .rom.00a7d347, "ax"
	.incbin "baserom.gba", 0x00a7d347, 0x00000001
	.section .rom.00a7e2d4, "ax"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e2d4, 0x000022d8
	.section .rom.00a81800, "ax"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a81800, 0x0000461c
	.section .rom.00a85f22, "ax"
	.incbin "baserom.gba", 0x00a85f22, 0x00000002
	.section .rom.00a8710d, "ax"
	.incbin "baserom.gba", 0x00a8710d, 0x00000003
	.section .rom.00a88d95, "ax"
	.incbin "baserom.gba", 0x00a88d95, 0x00000003
	.section .rom.00a892a3, "ax"
	.incbin "baserom.gba", 0x00a892a3, 0x00000001
	.section .rom.00a893e3, "ax"
	.incbin "baserom.gba", 0x00a893e3, 0x00000001
	.section .rom.00a8b066, "ax"
	.incbin "baserom.gba", 0x00a8b066, 0x00000002
	.section .rom.00a8da3e, "ax"
	.incbin "baserom.gba", 0x00a8da3e, 0x00000002
	.section .rom.00a90207, "ax"
	.incbin "baserom.gba", 0x00a90207, 0x00000001
	.section .rom.00a91727, "ax"
	.incbin "baserom.gba", 0x00a91727, 0x00000001
	.section .rom.00a9306b, "ax"
	.incbin "baserom.gba", 0x00a9306b, 0x00000001
	.section .rom.00a94b19, "ax"
	.incbin "baserom.gba", 0x00a94b19, 0x00000003
	.section .rom.00a94c8d, "ax"
	.incbin "baserom.gba", 0x00a94c8d, 0x00000003
	.section .rom.00a97a71, "ax"
	.incbin "baserom.gba", 0x00a97a71, 0x00000003
	.section .rom.00a9a31b, "ax"
	.incbin "baserom.gba", 0x00a9a31b, 0x00000001
	.section .rom.00a9e7b2, "ax"
	.incbin "baserom.gba", 0x00a9e7b2, 0x00000002
	.section .rom.00aa1b2a, "ax"
	.incbin "baserom.gba", 0x00aa1b2a, 0x00000002
	.section .rom.00aa4627, "ax"
	.incbin "baserom.gba", 0x00aa4627, 0x00000001
	.section .rom.00aa6705, "ax"
	.incbin "baserom.gba", 0x00aa6705, 0x00000003
	.section .rom.00aa8951, "ax"
	.incbin "baserom.gba", 0x00aa8951, 0x00000003
	.section .rom.00aa8a93, "ax"
	.incbin "baserom.gba", 0x00aa8a93, 0x00000001
	.section .rom.00aaa166, "ax"
	.incbin "baserom.gba", 0x00aaa166, 0x00000002
	.section .rom.00aaa266, "ax"
	.incbin "baserom.gba", 0x00aaa266, 0x00000002
	.section .rom.00aac159, "ax"
	.incbin "baserom.gba", 0x00aac159, 0x00000003
	.section .rom.00aadb31, "ax"
	.incbin "baserom.gba", 0x00aadb31, 0x00000003
	.section .rom.00ab0562, "ax"
	.incbin "baserom.gba", 0x00ab0562, 0x00000002
	.section .rom.00ab57b3, "ax"
	.incbin "baserom.gba", 0x00ab57b3, 0x00000001
	.section .rom.00ab80f7, "ax"
	.incbin "baserom.gba", 0x00ab80f7, 0x00000001
	.section .rom.00ab95ca, "ax"
	.incbin "baserom.gba", 0x00ab95ca, 0x00000002
	.section .rom.00abe395, "ax"
	.incbin "baserom.gba", 0x00abe395, 0x00000003
	.section .rom.00ac103f, "ax"
	.incbin "baserom.gba", 0x00ac103f, 0x00000001
	.section .rom.00ac4277, "ax"
	.incbin "baserom.gba", 0x00ac4277, 0x00000001
	.section .rom.00ac440f, "ax"
	.incbin "baserom.gba", 0x00ac440f, 0x00000001
	.section .rom.00ac720f, "ax"
	.incbin "baserom.gba", 0x00ac720f, 0x00000001
	.section .rom.00ac89c3, "ax"
	.incbin "baserom.gba", 0x00ac89c3, 0x00000001
	.section .rom.00ac995e, "ax"
	.incbin "baserom.gba", 0x00ac995e, 0x00000002
	.section .rom.00acbfbf, "ax"
	.incbin "baserom.gba", 0x00acbfbf, 0x00000001
	.section .rom.00acc166, "ax"
	.incbin "baserom.gba", 0x00acc166, 0x00000002
	.section .rom.00acf11f, "ax"
	.incbin "baserom.gba", 0x00acf11f, 0x00000001
	.section .rom.00acfdc5, "ax"
	.incbin "baserom.gba", 0x00acfdc5, 0x00000003
	.section .rom.00ad1395, "ax"
	.incbin "baserom.gba", 0x00ad1395, 0x00000003
	.section .rom.00ad9361, "ax"
	.incbin "baserom.gba", 0x00ad9361, 0x00000003
	.section .rom.00adae47, "ax"
	.incbin "baserom.gba", 0x00adae47, 0x00000001
	.section .rom.00adc331, "ax"
	.incbin "baserom.gba", 0x00adc331, 0x00000003
	.section .rom.00ae255b, "ax"
	.incbin "baserom.gba", 0x00ae255b, 0x00000001
	.section .rom.00ae2a2d, "ax"
	.incbin "baserom.gba", 0x00ae2a2d, 0x00000003
	.section .rom.00ae3b75, "ax"
	.incbin "baserom.gba", 0x00ae3b75, 0x00000003
	.section .rom.00ae3d26, "ax"
	.incbin "baserom.gba", 0x00ae3d26, 0x00000002
	.section .rom.00aefa13, "ax"
	.incbin "baserom.gba", 0x00aefa13, 0x00000001
	.section .rom.00aefb33, "ax"
	.incbin "baserom.gba", 0x00aefb33, 0x00000001
	.section .rom.00af1ed3, "ax"
	.incbin "baserom.gba", 0x00af1ed3, 0x00000001
	.section .rom.00af311f, "ax"
	.incbin "baserom.gba", 0x00af311f, 0x00000001
	.section .rom.00af54af, "ax"
	.incbin "baserom.gba", 0x00af54af, 0x00000001
	.section .rom.00af6dad, "ax"
	.incbin "baserom.gba", 0x00af6dad, 0x00000003
	.section .rom.00af7d9e, "ax"
	.incbin "baserom.gba", 0x00af7d9e, 0x00000002
	.section .rom.00afa33a, "ax"
	.incbin "baserom.gba", 0x00afa33a, 0x00000002
	.section .rom.00afe962, "ax"
	.incbin "baserom.gba", 0x00afe962, 0x00000002
	.section .rom.00aff027, "ax"
	.incbin "baserom.gba", 0x00aff027, 0x00000001
	.section .rom.00b01abd, "ax"
	.incbin "baserom.gba", 0x00b01abd, 0x00000003
	.section .rom.00b01c0e, "ax"
	.incbin "baserom.gba", 0x00b01c0e, 0x00000002
	.section .rom.00b0401e, "ax"
	.incbin "baserom.gba", 0x00b0401e, 0x00000002
	.section .rom.00b0619f, "ax"
	.incbin "baserom.gba", 0x00b0619f, 0x00000001
	.section .rom.00b062df, "ax"
	.incbin "baserom.gba", 0x00b062df, 0x00000001
	.section .rom.00b08d7e, "ax"
	.incbin "baserom.gba", 0x00b08d7e, 0x00000002
	.section .rom.00b08ecf, "ax"
	.incbin "baserom.gba", 0x00b08ecf, 0x00000001
	.section .rom.00b0b2de, "ax"
	.incbin "baserom.gba", 0x00b0b2de, 0x00000002
	.section .rom.00b0d45f, "ax"
	.incbin "baserom.gba", 0x00b0d45f, 0x00000001
	.section .rom.00b0d59f, "ax"
	.incbin "baserom.gba", 0x00b0d59f, 0x00000001
	.section .rom.00b10bce, "ax"
	.incbin "baserom.gba", 0x00b10bce, 0x00000002
	.section .rom.00b13132, "ax"
	.incbin "baserom.gba", 0x00b13132, 0x00000002
	.section .rom.00b152b3, "ax"
	.incbin "baserom.gba", 0x00b152b3, 0x00000001
	.section .rom.00b153f3, "ax"
	.incbin "baserom.gba", 0x00b153f3, 0x00000001
	.section .rom.00b168a3, "ax"
	.incbin "baserom.gba", 0x00b168a3, 0x00000001
	.section .rom.00b169ae, "ax"
	.incbin "baserom.gba", 0x00b169ae, 0x00000002
	.section .rom.00b18506, "ax"
	.incbin "baserom.gba", 0x00b18506, 0x00000002
	.section .rom.00b19ba9, "ax"
	.incbin "baserom.gba", 0x00b19ba9, 0x00000003
	.section .rom.00b19d6a, "ax"
	.incbin "baserom.gba", 0x00b19d6a, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19d6c, 0x00000d9c
	.section .rom.00b1c75e, "ax"
	.incbin "baserom.gba", 0x00b1c75e, 0x00000002
	.section .rom.00b1dfb5, "ax"
	.incbin "baserom.gba", 0x00b1dfb5, 0x00000003
	.section .rom.00b1e176, "ax"
	.incbin "baserom.gba", 0x00b1e176, 0x00000002
	.section .rom.00b223f1, "ax"
	.incbin "baserom.gba", 0x00b223f1, 0x00000003
	.section .rom.00b225b2, "ax"
	.incbin "baserom.gba", 0x00b225b2, 0x00000002
	.section .rom.00b23029, "ax"
	.incbin "baserom.gba", 0x00b23029, 0x00000003
	.section .rom.00b23131, "ax"
	.incbin "baserom.gba", 0x00b23131, 0x00000003
	.section .rom.00b24df2, "ax"
	.incbin "baserom.gba", 0x00b24df2, 0x00000002
	.section .rom.00b2657f, "ax"
	.incbin "baserom.gba", 0x00b2657f, 0x00000001
	.section .rom.00b26835, "ax"
	.incbin "baserom.gba", 0x00b26835, 0x00000003
	.section .rom.00b28345, "ax"
	.incbin "baserom.gba", 0x00b28345, 0x00000003
	.section .rom.00b2abd2, "ax"
	.incbin "baserom.gba", 0x00b2abd2, 0x00000002
	.section .rom.00b2d3c2, "ax"
	.incbin "baserom.gba", 0x00b2d3c2, 0x00000002
	.section .rom.00b2e362, "ax"
	.incbin "baserom.gba", 0x00b2e362, 0x00000002
	.section .rom.00b304b5, "ax"
	.incbin "baserom.gba", 0x00b304b5, 0x00000003
	.section .rom.00b33b42, "ax"
	.incbin "baserom.gba", 0x00b33b42, 0x00000002
	.section .rom.00b358d7, "ax"
	.incbin "baserom.gba", 0x00b358d7, 0x00000001
	.section .rom.00b376f5, "ax"
	.incbin "baserom.gba", 0x00b376f5, 0x00000003
	.section .rom.00b391d1, "ax"
	.incbin "baserom.gba", 0x00b391d1, 0x00000003
	.section .rom.00b3a7ba, "ax"
	.incbin "baserom.gba", 0x00b3a7ba, 0x00000002
	.section .rom.00b3cda2, "ax"
	.incbin "baserom.gba", 0x00b3cda2, 0x00000002
	.section .rom.00b3cf1b, "ax"
	.incbin "baserom.gba", 0x00b3cf1b, 0x00000001
	.section .rom.00b3e3da, "ax"
	.incbin "baserom.gba", 0x00b3e3da, 0x00000002
	.section .rom.00b3fbde, "ax"
	.incbin "baserom.gba", 0x00b3fbde, 0x00000002
	.section .rom.00b41556, "ax"
	.incbin "baserom.gba", 0x00b41556, 0x00000002
	.section .rom.00b42476, "ax"
	.incbin "baserom.gba", 0x00b42476, 0x00000002
	.section .rom.00b43f22, "ax"
	.incbin "baserom.gba", 0x00b43f22, 0x00000002
	.section .rom.00b4402f, "ax"
	.incbin "baserom.gba", 0x00b4402f, 0x00000001
	.section .rom.00b4615f, "ax"
	.incbin "baserom.gba", 0x00b4615f, 0x00000001
	.section .rom.00b47d35, "ax"
	.incbin "baserom.gba", 0x00b47d35, 0x00000003
	.section .rom.00b48347, "ax"
	.incbin "baserom.gba", 0x00b48347, 0x00000001
	.section .rom.00b48487, "ax"
	.incbin "baserom.gba", 0x00b48487, 0x00000001
	.section .rom.00b4ab87, "ax"
	.incbin "baserom.gba", 0x00b4ab87, 0x00000001
	.section .rom.00b4c3e6, "ax"
	.incbin "baserom.gba", 0x00b4c3e6, 0x00000002
	.section .rom.00b4c9f7, "ax"
	.incbin "baserom.gba", 0x00b4c9f7, 0x00000001
	.section .rom.00b4cb37, "ax"
	.incbin "baserom.gba", 0x00b4cb37, 0x00000001
	.section .rom.00b4d9b2, "ax"
	.incbin "baserom.gba", 0x00b4d9b2, 0x00000002
	.section .rom.00b4dac9, "ax"
	.incbin "baserom.gba", 0x00b4dac9, 0x00000003
	.section .rom.00b4fd9f, "ax"
	.incbin "baserom.gba", 0x00b4fd9f, 0x00000001
	.section .rom.00b519f2, "ax"
	.incbin "baserom.gba", 0x00b519f2, 0x00000002
	.section .rom.00b52037, "ax"
	.incbin "baserom.gba", 0x00b52037, 0x00000001
	.section .rom.00b52177, "ax"
	.incbin "baserom.gba", 0x00b52177, 0x00000001
	.section .rom.00b52d7d, "ax"
	.incbin "baserom.gba", 0x00b52d7d, 0x00000003
	.section .rom.00b5532f, "ax"
	.incbin "baserom.gba", 0x00b5532f, 0x00000001
	.section .rom.00b56fee, "ax"
	.incbin "baserom.gba", 0x00b56fee, 0x00000002
	.section .rom.00b58aba, "ax"
	.incbin "baserom.gba", 0x00b58aba, 0x00000002
	.section .rom.00b59b8e, "ax"
	.incbin "baserom.gba", 0x00b59b8e, 0x00000002
	.section .rom.00b59cf5, "ax"
	.incbin "baserom.gba", 0x00b59cf5, 0x00000003
	.section .rom.00b5af0e, "ax"
	.incbin "baserom.gba", 0x00b5af0e, 0x00000002
	.section .rom.00b5baed, "ax"
	.incbin "baserom.gba", 0x00b5baed, 0x00000003
	.section .rom.00b5bc5a, "ax"
	.incbin "baserom.gba", 0x00b5bc5a, 0x00000002
	.section .rom.00b5eaab, "ax"
	.incbin "baserom.gba", 0x00b5eaab, 0x00000001
	.section .rom.00b6126a, "ax"
	.incbin "baserom.gba", 0x00b6126a, 0x00000002
	.section .rom.00b6731b, "ax"
	.incbin "baserom.gba", 0x00b6731b, 0x00000001
	.section .rom.00b68e4b, "ax"
	.incbin "baserom.gba", 0x00b68e4b, 0x00000001
	.section .rom.00b6b182, "ax"
	.incbin "baserom.gba", 0x00b6b182, 0x00000002
	.section .rom.00b6c333, "ax"
	.incbin "baserom.gba", 0x00b6c333, 0x00000001
	.section .rom.00b6dc55, "ax"
	.incbin "baserom.gba", 0x00b6dc55, 0x00000003
	.section .rom.00b6fae1, "ax"
	.incbin "baserom.gba", 0x00b6fae1, 0x00000003
	.section .rom.00b71cc2, "ax"
	.incbin "baserom.gba", 0x00b71cc2, 0x00000002
	.section .rom.00b75689, "ax"
	.incbin "baserom.gba", 0x00b75689, 0x00000003
	.section .rom.00b75775, "ax"
	.incbin "baserom.gba", 0x00b75775, 0x00000003
	.section .rom.00b789ae, "ax"
	.incbin "baserom.gba", 0x00b789ae, 0x00000002
	.section .rom.00b79025, "ax"
	.incbin "baserom.gba", 0x00b79025, 0x00000003
	.section .rom.00b7cde7, "ax"
	.incbin "baserom.gba", 0x00b7cde7, 0x00000001
	.section .rom.00b7f079, "ax"
	.incbin "baserom.gba", 0x00b7f079, 0x00000003
	.section .rom.00b80a96, "ax"
	.incbin "baserom.gba", 0x00b80a96, 0x00000002
	.section .rom.00b81ba3, "ax"
	.incbin "baserom.gba", 0x00b81ba3, 0x00000001
	.section .rom.00b84d42, "ax"
	.incbin "baserom.gba", 0x00b84d42, 0x00000002
	.section .rom.00b85f3a, "ax"
	.incbin "baserom.gba", 0x00b85f3a, 0x00000002
	.section .rom.00b86dfd, "ax"
	.incbin "baserom.gba", 0x00b86dfd, 0x00000003
	.section .rom.00b88e9e, "ax"
	.incbin "baserom.gba", 0x00b88e9e, 0x00000002
	.section .rom.00b8a8f3, "ax"
	.incbin "baserom.gba", 0x00b8a8f3, 0x00000001
	.section .rom.00b8bdbe, "ax"
	.incbin "baserom.gba", 0x00b8bdbe, 0x00000002
	.section .rom.00b8dfc2, "ax"
	.incbin "baserom.gba", 0x00b8dfc2, 0x00000002
	.section .rom.00b8e0b7, "ax"
	.incbin "baserom.gba", 0x00b8e0b7, 0x00000001
	.section .rom.00b8f68a, "ax"
	.incbin "baserom.gba", 0x00b8f68a, 0x00000002
	.section .rom.00b8f8a9, "ax"
	.incbin "baserom.gba", 0x00b8f8a9, 0x00000003
	.section .rom.00b9198d, "ax"
	.incbin "baserom.gba", 0x00b9198d, 0x00000003
	.section .rom.00b91b52, "ax"
	.incbin "baserom.gba", 0x00b91b52, 0x00000002
	.section .rom.00b93c15, "ax"
	.incbin "baserom.gba", 0x00b93c15, 0x00000003
	.section .rom.00b93d45, "ax"
	.incbin "baserom.gba", 0x00b93d45, 0x00000003
	.section .rom.00b967fe, "ax"
	.incbin "baserom.gba", 0x00b967fe, 0x00000002
	.section .rom.00b98356, "ax"
	.incbin "baserom.gba", 0x00b98356, 0x00000002
	.section .rom.00b9929d, "ax"
	.incbin "baserom.gba", 0x00b9929d, 0x00000003
	.section .rom.00b9aca6, "ax"
	.incbin "baserom.gba", 0x00b9aca6, 0x00000002
	.section .rom.00baa9fb, "ax"
	.incbin "baserom.gba", 0x00baa9fb, 0x00000001
	.section .rom.00baab4b, "ax"
	.incbin "baserom.gba", 0x00baab4b, 0x00000001
	.section .rom.00bad656, "ax"
	.incbin "baserom.gba", 0x00bad656, 0x00000002
	.section .rom.00baf25f, "ax"
	.incbin "baserom.gba", 0x00baf25f, 0x00000001
	.section .rom.00bb0c66, "ax"
	.incbin "baserom.gba", 0x00bb0c66, 0x00000002
	.section .rom.00bb2981, "ax"
	.incbin "baserom.gba", 0x00bb2981, 0x00000003
	.section .rom.00bb2ad3, "ax"
	.incbin "baserom.gba", 0x00bb2ad3, 0x00000001
	.section .rom.00bb44da, "ax"
	.incbin "baserom.gba", 0x00bb44da, 0x00000002
	.section .rom.00bb8262, "ax"
	.incbin "baserom.gba", 0x00bb8262, 0x00000002
	.section .rom.00bb83f5, "ax"
	.incbin "baserom.gba", 0x00bb83f5, 0x00000003
	.section .rom.00bbd3ff, "ax"
	.incbin "baserom.gba", 0x00bbd3ff, 0x00000001
	.section .rom.00bbfa81, "ax"
	.incbin "baserom.gba", 0x00bbfa81, 0x00000003
	.section .rom.00bbfdb7, "ax"
	.incbin "baserom.gba", 0x00bbfdb7, 0x00000001
	.section .rom.00bc620d, "ax"
	.incbin "baserom.gba", 0x00bc620d, 0x00000003
	.section .rom.00bc635a, "ax"
	.incbin "baserom.gba", 0x00bc635a, 0x00000002
	.section .rom.00bc81df, "ax"
	.incbin "baserom.gba", 0x00bc81df, 0x00000001
	.section .rom.00bc9786, "ax"
	.incbin "baserom.gba", 0x00bc9786, 0x00000002
	.section .rom.00bcb731, "ax"
	.incbin "baserom.gba", 0x00bcb731, 0x00000003
	.section .rom.00bcc6a2, "ax"
	.incbin "baserom.gba", 0x00bcc6a2, 0x00000002
	.section .rom.00bd7cab, "ax"
	.incbin "baserom.gba", 0x00bd7cab, 0x00000001
	.section .rom.00bd7d67, "ax"
	.incbin "baserom.gba", 0x00bd7d67, 0x00000001
	.section .rom.00bd8aa6, "ax"
	.incbin "baserom.gba", 0x00bd8aa6, 0x00000002
	.section .rom.00bd9483, "ax"
	.incbin "baserom.gba", 0x00bd9483, 0x00000001
	.section .rom.00bda09b, "ax"
	.incbin "baserom.gba", 0x00bda09b, 0x00000001
	.section .rom.00bdbf56, "ax"
	.incbin "baserom.gba", 0x00bdbf56, 0x00000002
	.section .rom.00bdc072, "ax"
	.incbin "baserom.gba", 0x00bdc072, 0x00000002
	.section .rom.00bde8f3, "ax"
	.incbin "baserom.gba", 0x00bde8f3, 0x00000001
	.section .rom.00be007e, "ax"
	.incbin "baserom.gba", 0x00be007e, 0x00000002
	.section .rom.00be3fae, "ax"
	.incbin "baserom.gba", 0x00be3fae, 0x00000002
	.section .rom.00be40f7, "ax"
	.incbin "baserom.gba", 0x00be40f7, 0x00000001
	.section .rom.00be67a7, "ax"
	.incbin "baserom.gba", 0x00be67a7, 0x00000001
	.section .rom.00be8f4a, "ax"
	.incbin "baserom.gba", 0x00be8f4a, 0x00000002
	.section .rom.00be9cd5, "ax"
	.incbin "baserom.gba", 0x00be9cd5, 0x00000003
	.section .rom.00becb16, "ax"
	.incbin "baserom.gba", 0x00becb16, 0x00000002
	.section .rom.00bef13d, "ax"
	.incbin "baserom.gba", 0x00bef13d, 0x00000003
	.section .rom.00bf0e29, "ax"
	.incbin "baserom.gba", 0x00bf0e29, 0x00000003
	.section .rom.00bf2529, "ax"
	.incbin "baserom.gba", 0x00bf2529, 0x00000003
	.section .rom.00bf3aa5, "ax"
	.incbin "baserom.gba", 0x00bf3aa5, 0x00000003
	.section .rom.00bf5217, "ax"
	.incbin "baserom.gba", 0x00bf5217, 0x00000001
	.section .rom.00bf7ea2, "ax"
	.incbin "baserom.gba", 0x00bf7ea2, 0x00000002
	.section .rom.00bf8eee, "ax"
	.incbin "baserom.gba", 0x00bf8eee, 0x00000002
	.section .rom.00bf9feb, "ax"
	.incbin "baserom.gba", 0x00bf9feb, 0x00000001
	.section .rom.00bfb7bf, "ax"
	.incbin "baserom.gba", 0x00bfb7bf, 0x00000001
	.section .rom.00bfcc83, "ax"
	.incbin "baserom.gba", 0x00bfcc83, 0x00000001
	.section .rom.00bff1b6, "ax"
	.incbin "baserom.gba", 0x00bff1b6, 0x00000002
	.section .rom.00bff2ea, "ax"
	.incbin "baserom.gba", 0x00bff2ea, 0x00000002
	.section .rom.00c01a56, "ax"
	.incbin "baserom.gba", 0x00c01a56, 0x00000002
	.section .rom.00c03829, "ax"
	.incbin "baserom.gba", 0x00c03829, 0x00000003
	.section .rom.00c0710d, "ax"
	.incbin "baserom.gba", 0x00c0710d, 0x00000003
	.section .rom.00c09873, "ax"
	.incbin "baserom.gba", 0x00c09873, 0x00000001
	.section .rom.00c0b1dd, "ax"
	.incbin "baserom.gba", 0x00c0b1dd, 0x00000003
	.section .rom.00c0da06, "ax"
	.incbin "baserom.gba", 0x00c0da06, 0x00000002
	.section .rom.00c11de5, "ax"
	.incbin "baserom.gba", 0x00c11de5, 0x00000003
	.section .rom.00c1518f, "ax"
	.incbin "baserom.gba", 0x00c1518f, 0x00000001
	.section .rom.00c15f82, "ax"
	.incbin "baserom.gba", 0x00c15f82, 0x00000002
	.section .rom.00c16cfa, "ax"
	.incbin "baserom.gba", 0x00c16cfa, 0x00000002
	.section .rom.00c1ee22, "ax"
	.incbin "baserom.gba", 0x00c1ee22, 0x00000002
	.section .rom.00c251a1, "ax"
	.incbin "baserom.gba", 0x00c251a1, 0x00000003
	.section .rom.00c25ba7, "ax"
	.incbin "baserom.gba", 0x00c25ba7, 0x00000001
	.section .rom.00c27655, "ax"
	.incbin "baserom.gba", 0x00c27655, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27658, 0x00001490
	.section .rom.00c28f4f, "ax"
	.incbin "baserom.gba", 0x00c28f4f, 0x00000001
	.section .rom.00c2910e, "ax"
	.incbin "baserom.gba", 0x00c2910e, 0x00000002
	.section .rom.00c2b916, "ax"
	.incbin "baserom.gba", 0x00c2b916, 0x00000002
	.section .rom.00c2ba6e, "ax"
	.incbin "baserom.gba", 0x00c2ba6e, 0x00000002
	.section .rom.00c2e3b5, "ax"
	.incbin "baserom.gba", 0x00c2e3b5, 0x00000003
	.section .rom.00c305d9, "ax"
	.incbin "baserom.gba", 0x00c305d9, 0x00000003
	.section .rom.00c31ace, "ax"
	.incbin "baserom.gba", 0x00c31ace, 0x00000002
	.section .rom.00c33d31, "ax"
	.incbin "baserom.gba", 0x00c33d31, 0x00000003
	.section .rom.00c365df, "ax"
	.incbin "baserom.gba", 0x00c365df, 0x00000001
	.section .rom.00c38572, "ax"
	.incbin "baserom.gba", 0x00c38572, 0x00000002
	.section .rom.00c39593, "ax"
	.incbin "baserom.gba", 0x00c39593, 0x00000001
	.section .rom.00c396d3, "ax"
	.incbin "baserom.gba", 0x00c396d3, 0x00000001
	.section .rom.00c3c2f2, "ax"
	.incbin "baserom.gba", 0x00c3c2f2, 0x00000002
	.section .rom.00c3e776, "ax"
	.incbin "baserom.gba", 0x00c3e776, 0x00000002
	.section .rom.00c4085d, "ax"
	.incbin "baserom.gba", 0x00c4085d, 0x00000003
	.section .rom.00c420c1, "ax"
	.incbin "baserom.gba", 0x00c420c1, 0x00000003
	.section .rom.00c44627, "ax"
	.incbin "baserom.gba", 0x00c44627, 0x00000001
	.section .rom.00c4478d, "ax"
	.incbin "baserom.gba", 0x00c4478d, 0x00000003
	.section .rom.00c46f87, "ax"
	.incbin "baserom.gba", 0x00c46f87, 0x00000001
	.section .rom.00c48fbb, "ax"
	.incbin "baserom.gba", 0x00c48fbb, 0x00000001
	.section .rom.00c4abe2, "ax"
	.incbin "baserom.gba", 0x00c4abe2, 0x00000002
	.section .rom.00c4d602, "ax"
	.incbin "baserom.gba", 0x00c4d602, 0x00000002
	.section .rom.00c4e16d, "ax"
	.incbin "baserom.gba", 0x00c4e16d, 0x00000003
	.section .rom.00c4e253, "ax"
	.incbin "baserom.gba", 0x00c4e253, 0x00000001
	.section .rom.00c4fb9d, "ax"
	.incbin "baserom.gba", 0x00c4fb9d, 0x00000003
	.section .rom.00c51503, "ax"
	.incbin "baserom.gba", 0x00c51503, 0x00000001
	.section .rom.00c528d1, "ax"
	.incbin "baserom.gba", 0x00c528d1, 0x00000003
	.section .rom.00c529b7, "ax"
	.incbin "baserom.gba", 0x00c529b7, 0x00000001
	.section .rom.00c53525, "ax"
	.incbin "baserom.gba", 0x00c53525, 0x00000003
	.section .rom.00c5360b, "ax"
	.incbin "baserom.gba", 0x00c5360b, 0x00000001
	.section .rom.00c5436f, "ax"
	.incbin "baserom.gba", 0x00c5436f, 0x00000001
	.section .rom.00c54453, "ax"
	.incbin "baserom.gba", 0x00c54453, 0x00000001
	.section .rom.00c54c59, "ax"
	.incbin "baserom.gba", 0x00c54c59, 0x00000003
	.section .rom.00c54d3f, "ax"
	.incbin "baserom.gba", 0x00c54d3f, 0x00000001
	.section .rom.00c5589f, "ax"
	.incbin "baserom.gba", 0x00c5589f, 0x00000001
	.section .rom.00c5611d, "ax"
	.incbin "baserom.gba", 0x00c5611d, 0x00000003
	.section .rom.00c5621a, "ax"
	.incbin "baserom.gba", 0x00c5621a, 0x00000002
	.section .rom.00c58073, "ax"
	.incbin "baserom.gba", 0x00c58073, 0x00000001
	.section .rom.00c58e81, "ax"
	.incbin "baserom.gba", 0x00c58e81, 0x00000003
	.section .rom.00c5a115, "ax"
	.incbin "baserom.gba", 0x00c5a115, 0x00000003
	.section .rom.00c5b0f1, "ax"
	.incbin "baserom.gba", 0x00c5b0f1, 0x00000003
	.section .rom.00c5d4f7, "ax"
	.incbin "baserom.gba", 0x00c5d4f7, 0x00000001
	.section .rom.00c5d637, "ax"
	.incbin "baserom.gba", 0x00c5d637, 0x00000001
	.section .rom.00c5dd05, "ax"
	.incbin "baserom.gba", 0x00c5dd05, 0x00000003
	.section .rom.00c5dddb, "ax"
	.incbin "baserom.gba", 0x00c5dddb, 0x00000001
	.section .rom.00c5e651, "ax"
	.incbin "baserom.gba", 0x00c5e651, 0x00000003
	.section .rom.00c5eb02, "ax"
	.incbin "baserom.gba", 0x00c5eb02, 0x00000002
	.section .rom.00c5ff7d, "ax"
	.incbin "baserom.gba", 0x00c5ff7d, 0x00000003
	.section .rom.00c60793, "ax"
	.incbin "baserom.gba", 0x00c60793, 0x00000001
	.section .rom.00c61429, "ax"
	.incbin "baserom.gba", 0x00c61429, 0x00000003
	.section .rom.00c615ce, "ax"
	.incbin "baserom.gba", 0x00c615ce, 0x00000002
	.section .rom.00c64492, "ax"
	.incbin "baserom.gba", 0x00c64492, 0x00000002
	.section .rom.00c65e11, "ax"
	.incbin "baserom.gba", 0x00c65e11, 0x00000003
	.section .rom.00c66e5e, "ax"
	.incbin "baserom.gba", 0x00c66e5e, 0x00000002
	.section .rom.00c6e5a3, "ax"
	.incbin "baserom.gba", 0x00c6e5a3, 0x00000001
	.section .rom.00c75d97, "ax"
	.incbin "baserom.gba", 0x00c75d97, 0x00000001
	.section .rom.00c7c391, "ax"
	.incbin "baserom.gba", 0x00c7c391, 0x00000003
	.section .rom.00c7edca, "ax"
	.incbin "baserom.gba", 0x00c7edca, 0x00000002
	.section .rom.00c8169b, "ax"
	.incbin "baserom.gba", 0x00c8169b, 0x00000001
	.section .rom.00c85ee7, "ax"
	.incbin "baserom.gba", 0x00c85ee7, 0x00000001
	.section .rom.00c86ee5, "ax"
	.incbin "baserom.gba", 0x00c86ee5, 0x00000003
	.section .rom.00c86ffe, "ax"
	.incbin "baserom.gba", 0x00c86ffe, 0x00000002
	.section .rom.00c885a6, "ax"
	.incbin "baserom.gba", 0x00c885a6, 0x00000002
	.section .rom.00c8a003, "ax"
	.incbin "baserom.gba", 0x00c8a003, 0x00000001
	.section .rom.00c8a13e, "ax"
	.incbin "baserom.gba", 0x00c8a13e, 0x00000002
	.section .rom.00c8b645, "ax"
	.incbin "baserom.gba", 0x00c8b645, 0x00000003
	.section .rom.00c8e8ce, "ax"
	.incbin "baserom.gba", 0x00c8e8ce, 0x00000002
	.section .rom.00c8fd15, "ax"
	.incbin "baserom.gba", 0x00c8fd15, 0x00000003
	.section .rom.00c92a0d, "ax"
	.incbin "baserom.gba", 0x00c92a0d, 0x00000003
	.section .rom.00c94e43, "ax"
	.incbin "baserom.gba", 0x00c94e43, 0x00000001
	.section .rom.00c9501f, "ax"
	.incbin "baserom.gba", 0x00c9501f, 0x00000001
	.section .rom.00c98133, "ax"
	.incbin "baserom.gba", 0x00c98133, 0x00000001
	.section .rom.00c995b6, "ax"
	.incbin "baserom.gba", 0x00c995b6, 0x00000002
	.section .rom.00c9a605, "ax"
	.incbin "baserom.gba", 0x00c9a605, 0x00000003
	.section .rom.00c9c46a, "ax"
	.incbin "baserom.gba", 0x00c9c46a, 0x00000002
	.section .rom.00c9c605, "ax"
	.incbin "baserom.gba", 0x00c9c605, 0x00000003
	.section .rom.00c9c763, "ax"
	.incbin "baserom.gba", 0x00c9c763, 0x00000001
	.section .rom.00c9d6e2, "ax"
	.incbin "baserom.gba", 0x00c9d6e2, 0x00000002
	.section .rom.00c9d7fa, "ax"
	.incbin "baserom.gba", 0x00c9d7fa, 0x00000002
	.section .rom.00c9f88d, "ax"
	.incbin "baserom.gba", 0x00c9f88d, 0x00000003
	.section .rom.00ca170f, "ax"
	.incbin "baserom.gba", 0x00ca170f, 0x00000001
	.section .rom.00ca3c35, "ax"
	.incbin "baserom.gba", 0x00ca3c35, 0x00000003
	.section .rom.00ca3d42, "ax"
	.incbin "baserom.gba", 0x00ca3d42, 0x00000002
	.section .rom.00ca5dd5, "ax"
	.incbin "baserom.gba", 0x00ca5dd5, 0x00000003
	.section .rom.00ca92ab, "ax"
	.incbin "baserom.gba", 0x00ca92ab, 0x00000001
	.section .rom.00ca9d95, "ax"
	.incbin "baserom.gba", 0x00ca9d95, 0x00000003
	.section .rom.00ca9f05, "ax"
	.incbin "baserom.gba", 0x00ca9f05, 0x00000003
	.section .rom.00cace1f, "ax"
	.incbin "baserom.gba", 0x00cace1f, 0x00000001
	.section .rom.00caedaa, "ax"
	.incbin "baserom.gba", 0x00caedaa, 0x00000002
	.section .rom.00cb0715, "ax"
	.incbin "baserom.gba", 0x00cb0715, 0x00000003
	.section .rom.00cb6202, "ax"
	.incbin "baserom.gba", 0x00cb6202, 0x00000002
	.section .rom.00cb63b2, "ax"
	.incbin "baserom.gba", 0x00cb63b2, 0x00000002
	.section .rom.00cbad2a, "ax"
	.incbin "baserom.gba", 0x00cbad2a, 0x00000002
	.section .rom.00cbae82, "ax"
	.incbin "baserom.gba", 0x00cbae82, 0x00000002
	.section .rom.00cbe44f, "ax"
	.incbin "baserom.gba", 0x00cbe44f, 0x00000001
	.section .rom.00cbfcd1, "ax"
	.incbin "baserom.gba", 0x00cbfcd1, 0x00000003
	.section .rom.00cc19f9, "ax"
	.incbin "baserom.gba", 0x00cc19f9, 0x00000003
	.section .rom.00cc511a, "ax"
	.incbin "baserom.gba", 0x00cc511a, 0x00000002
	.section .rom.00cca2a3, "ax"
	.incbin "baserom.gba", 0x00cca2a3, 0x00000001
	.section .rom.00ccc871, "ax"
	.incbin "baserom.gba", 0x00ccc871, 0x00000003
	.section .rom.00cce1fd, "ax"
	.incbin "baserom.gba", 0x00cce1fd, 0x00000003
	.section .rom.00ccf32a, "ax"
	.incbin "baserom.gba", 0x00ccf32a, 0x00000002
	.section .rom.00ccfeda, "ax"
	.incbin "baserom.gba", 0x00ccfeda, 0x00000002
	.section .rom.00cd18a2, "ax"
	.incbin "baserom.gba", 0x00cd18a2, 0x00000002
	.section .rom.00cd1986, "ax"
	.incbin "baserom.gba", 0x00cd1986, 0x00000002
	.section .rom.00cd3de3, "ax"
	.incbin "baserom.gba", 0x00cd3de3, 0x00000001
	.section .rom.00cd3f23, "ax"
	.incbin "baserom.gba", 0x00cd3f23, 0x00000001
	.section .rom.00cd7599, "ax"
	.incbin "baserom.gba", 0x00cd7599, 0x00000003
	.section .rom.00cd76fd, "ax"
	.incbin "baserom.gba", 0x00cd76fd, 0x00000003
	.section .rom.00cd9ddd, "ax"
	.incbin "baserom.gba", 0x00cd9ddd, 0x00000003
	.section .rom.00cdcc8d, "ax"
	.incbin "baserom.gba", 0x00cdcc8d, 0x00000003
	.section .rom.00cde202, "ax"
	.incbin "baserom.gba", 0x00cde202, 0x00000002
	.section .rom.00ce22fe, "ax"
	.incbin "baserom.gba", 0x00ce22fe, 0x00000002
	.section .rom.00ce4583, "ax"
	.incbin "baserom.gba", 0x00ce4583, 0x00000001
	.section .rom.00ce6167, "ax"
	.incbin "baserom.gba", 0x00ce6167, 0x00000001
	.section .rom.00ce7e5d, "ax"
	.incbin "baserom.gba", 0x00ce7e5d, 0x00000003
	.section .rom.00cf5481, "ax"
	.incbin "baserom.gba", 0x00cf5481, 0x00000003
	.section .rom.00cf55a9, "ax"
	.incbin "baserom.gba", 0x00cf55a9, 0x00000003
	.section .rom.00cf77ba, "ax"
	.incbin "baserom.gba", 0x00cf77ba, 0x00000002
	.section .rom.00cf794b, "ax"
	.incbin "baserom.gba", 0x00cf794b, 0x00000001
	.section .rom.00cfedfd, "ax"
	.incbin "baserom.gba", 0x00cfedfd, 0x00000003
	.section .rom.00d015d3, "ax"
	.incbin "baserom.gba", 0x00d015d3, 0x00000001
	.section .rom.00d03d2e, "ax"
	.incbin "baserom.gba", 0x00d03d2e, 0x00000002
	.section .rom.00d03ee1, "ax"
	.incbin "baserom.gba", 0x00d03ee1, 0x00000003
	.section .rom.00d055e5, "ax"
	.incbin "baserom.gba", 0x00d055e5, 0x00000003
	.section .rom.00d0763f, "ax"
	.incbin "baserom.gba", 0x00d0763f, 0x00000001
	.section .rom.00d08989, "ax"
	.incbin "baserom.gba", 0x00d08989, 0x00000003
	.section .rom.00d0a3d3, "ax"
	.incbin "baserom.gba", 0x00d0a3d3, 0x00000001
	.section .rom.00d0a4f7, "ax"
	.incbin "baserom.gba", 0x00d0a4f7, 0x00000001
	.section .rom.00d0a9c9, "ax"
	.incbin "baserom.gba", 0x00d0a9c9, 0x00000003
	.section .rom.00d0cbad, "ax"
	.incbin "baserom.gba", 0x00d0cbad, 0x00000003
	.section .rom.00d0d081, "ax"
	.incbin "baserom.gba", 0x00d0d081, 0x00000003
	.section .rom.00d0f5b7, "ax"
	.incbin "baserom.gba", 0x00d0f5b7, 0x00000001
	.section .rom.00d0f73f, "ax"
	.incbin "baserom.gba", 0x00d0f73f, 0x00000001
	.section .rom.00d13fe1, "ax"
	.incbin "baserom.gba", 0x00d13fe1, 0x00000003
	.section .rom.00d14117, "ax"
	.incbin "baserom.gba", 0x00d14117, 0x00000001
	.section .rom.00d15d83, "ax"
	.incbin "baserom.gba", 0x00d15d83, 0x00000001
	.section .rom.00d16f3b, "ax"
	.incbin "baserom.gba", 0x00d16f3b, 0x00000001
	.section .rom.00d17c89, "ax"
	.incbin "baserom.gba", 0x00d17c89, 0x00000003
	.section .rom.00d18e12, "ax"
	.incbin "baserom.gba", 0x00d18e12, 0x00000002
	.section .rom.00d1a90b, "ax"
	.incbin "baserom.gba", 0x00d1a90b, 0x00000001
	.section .rom.00d1bd17, "ax"
	.incbin "baserom.gba", 0x00d1bd17, 0x00000001
	.section .rom.00d1c26b, "ax"
	.incbin "baserom.gba", 0x00d1c26b, 0x00000001
	.section .rom.00d1c8a5, "ax"
	.incbin "baserom.gba", 0x00d1c8a5, 0x00000003
	.section .rom.00d21bf5, "ax"
	.incbin "baserom.gba", 0x00d21bf5, 0x00000003
	.section .rom.00d24256, "ax"
	.incbin "baserom.gba", 0x00d24256, 0x00000002
	.section .rom.00d243eb, "ax"
	.incbin "baserom.gba", 0x00d243eb, 0x00000001
	.section .rom.00d25f9f, "ax"
	.incbin "baserom.gba", 0x00d25f9f, 0x00000001
	.section .rom.00d260ff, "ax"
	.incbin "baserom.gba", 0x00d260ff, 0x00000001
	.section .rom.00d28ba3, "ax"
	.incbin "baserom.gba", 0x00d28ba3, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28ba4, 0x000021b0
	.section .rom.00d2cbca, "ax"
	.incbin "baserom.gba", 0x00d2cbca, 0x00000002
	.section .rom.00d2cd0b, "ax"
	.incbin "baserom.gba", 0x00d2cd0b, 0x00000001
	.section .rom.00d33c83, "ax"
	.incbin "baserom.gba", 0x00d33c83, 0x00000001
	.section .rom.00d33dc1, "ax"
	.incbin "baserom.gba", 0x00d33dc1, 0x00000003
	.section .rom.00d35d95, "ax"
	.incbin "baserom.gba", 0x00d35d95, 0x00000003
	.section .rom.00d35e89, "ax"
	.incbin "baserom.gba", 0x00d35e89, 0x00000003
	.section .rom.00d36f69, "ax"
	.incbin "baserom.gba", 0x00d36f69, 0x00000003
	.section .rom.00d38e43, "ax"
	.incbin "baserom.gba", 0x00d38e43, 0x00000001
	.section .rom.00d39dfb, "ax"
	.incbin "baserom.gba", 0x00d39dfb, 0x00000001
	.section .rom.00d3bd42, "ax"
	.incbin "baserom.gba", 0x00d3bd42, 0x00000002
	.section .rom.00d3d94d, "ax"
	.incbin "baserom.gba", 0x00d3d94d, 0x00000003
	.section .rom.00d3da99, "ax"
	.incbin "baserom.gba", 0x00d3da99, 0x00000003
	.section .rom.00d3fb57, "ax"
	.incbin "baserom.gba", 0x00d3fb57, 0x00000001
	.section .rom.00d43c46, "ax"
	.incbin "baserom.gba", 0x00d43c46, 0x00000002
	.section .rom.00d479f8, "ax"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d479f8, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d49904, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4bc68, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4dc94, 0x0000180c
	.section .rom.00d50bed, "ax"
	.incbin "baserom.gba", 0x00d50bed, 0x00000003
	.section .rom.00d50d1a, "ax"
	.incbin "baserom.gba", 0x00d50d1a, 0x00000002
	.section .rom.00d57c8d, "ax"
	.incbin "baserom.gba", 0x00d57c8d, 0x00000003
	.section .rom.00d57d92, "ax"
	.incbin "baserom.gba", 0x00d57d92, 0x00000002
	.section .rom.00d57ed2, "ax"
	.incbin "baserom.gba", 0x00d57ed2, 0x00000002
	.section .rom.00d5989e, "ax"
	.incbin "baserom.gba", 0x00d5989e, 0x00000002
	.section .rom.00d59995, "ax"
	.incbin "baserom.gba", 0x00d59995, 0x00000003
	.section .rom.00d5c7e9, "ax"
	.incbin "baserom.gba", 0x00d5c7e9, 0x00000003
	.section .rom.00d5c992, "ax"
	.incbin "baserom.gba", 0x00d5c992, 0x00000002
	.section .rom.00d5f5ae, "ax"
	.incbin "baserom.gba", 0x00d5f5ae, 0x00000002
	.section .rom.00d622e1, "ax"
	.incbin "baserom.gba", 0x00d622e1, 0x00000003
	.section .rom.00d63fcb, "ax"
	.incbin "baserom.gba", 0x00d63fcb, 0x00000001
	.section .rom.00d67845, "ax"
	.incbin "baserom.gba", 0x00d67845, 0x00000003
	.section .rom.00d6799d, "ax"
	.incbin "baserom.gba", 0x00d6799d, 0x00000003
	.section .rom.00d69ee1, "ax"
	.incbin "baserom.gba", 0x00d69ee1, 0x00000003
	.section .rom.00d6dda6, "ax"
	.incbin "baserom.gba", 0x00d6dda6, 0x00000002
	.section .rom.00d6fed6, "ax"
	.incbin "baserom.gba", 0x00d6fed6, 0x00000002
	.section .rom.00d740e2, "ax"
	.incbin "baserom.gba", 0x00d740e2, 0x00000002
	.section .rom.00d742b6, "ax"
	.incbin "baserom.gba", 0x00d742b6, 0x00000002
	.section .rom.00d762f6, "ax"
	.incbin "baserom.gba", 0x00d762f6, 0x00000002
	.section .rom.00d776ed, "ax"
	.incbin "baserom.gba", 0x00d776ed, 0x00000003
	.section .rom.00d7822a, "ax"
	.incbin "baserom.gba", 0x00d7822a, 0x00000002
	.section .rom.00d79395, "ax"
	.incbin "baserom.gba", 0x00d79395, 0x00000003
	.section .rom.00d7a441, "ax"
	.incbin "baserom.gba", 0x00d7a441, 0x00000003
	.section .rom.00d7a5ed, "ax"
	.incbin "baserom.gba", 0x00d7a5ed, 0x00000003
	.section .rom.00d7c0fa, "ax"
	.incbin "baserom.gba", 0x00d7c0fa, 0x00000002
	.section .rom.00d7db17, "ax"
	.incbin "baserom.gba", 0x00d7db17, 0x00000001
	.section .rom.00d7ffe9, "ax"
	.incbin "baserom.gba", 0x00d7ffe9, 0x00000003
	.section .rom.00d8134b, "ax"
	.incbin "baserom.gba", 0x00d8134b, 0x00000001
	.section .rom.00d821ff, "ax"
	.incbin "baserom.gba", 0x00d821ff, 0x00000001
	.section .rom.00d837ed, "ax"
	.incbin "baserom.gba", 0x00d837ed, 0x00000003
	.section .rom.00d84d33, "ax"
	.incbin "baserom.gba", 0x00d84d33, 0x00000001
	.section .rom.00d85afd, "ax"
	.incbin "baserom.gba", 0x00d85afd, 0x00000003
	.section .rom.00d85c6b, "ax"
	.incbin "baserom.gba", 0x00d85c6b, 0x00000001
	.section .rom.00d86c2a, "ax"
	.incbin "baserom.gba", 0x00d86c2a, 0x00000002
	.section .rom.00d876f3, "ax"
	.incbin "baserom.gba", 0x00d876f3, 0x00000001
	.section .rom.00d8828d, "ax"
	.incbin "baserom.gba", 0x00d8828d, 0x00000003
	.section .rom.00d883dd, "ax"
	.incbin "baserom.gba", 0x00d883dd, 0x00000003
	.section .rom.00d8c68b, "ax"
	.incbin "baserom.gba", 0x00d8c68b, 0x00000001
	.section .rom.00d8e125, "ax"
	.incbin "baserom.gba", 0x00d8e125, 0x00000003
	.section .rom.00d8fafb, "ax"
	.incbin "baserom.gba", 0x00d8fafb, 0x00000001
	.section .rom.00d8fc7a, "ax"
	.incbin "baserom.gba", 0x00d8fc7a, 0x00000002
	.section .rom.00d938aa, "ax"
	.incbin "baserom.gba", 0x00d938aa, 0x00000002
	.section .rom.00d95607, "ax"
	.incbin "baserom.gba", 0x00d95607, 0x00000001
	.section .rom.00d98c31, "ax"
	.incbin "baserom.gba", 0x00d98c31, 0x00000003
	.section .rom.00d98d82, "ax"
	.incbin "baserom.gba", 0x00d98d82, 0x00000002
	.section .rom.00d9e97e, "ax"
	.incbin "baserom.gba", 0x00d9e97e, 0x00000002
	.section .rom.00da12bb, "ax"
	.incbin "baserom.gba", 0x00da12bb, 0x00000001
	.section .rom.00da2bfd, "ax"
	.incbin "baserom.gba", 0x00da2bfd, 0x00000003
	.section .rom.00da2d66, "ax"
	.incbin "baserom.gba", 0x00da2d66, 0x00000002
	.section .rom.00da9b65, "ax"
	.incbin "baserom.gba", 0x00da9b65, 0x00000003
	.section .rom.00daa143, "ax"
	.incbin "baserom.gba", 0x00daa143, 0x00000001
	.section .rom.00dab04b, "ax"
	.incbin "baserom.gba", 0x00dab04b, 0x00000001
	.section .rom.00dad893, "ax"
	.incbin "baserom.gba", 0x00dad893, 0x00000001
	.section .rom.00daeeb2, "ax"
	.incbin "baserom.gba", 0x00daeeb2, 0x00000002
	.section .rom.00db0813, "ax"
	.incbin "baserom.gba", 0x00db0813, 0x00000001
	.section .rom.00db35fb, "ax"
	.incbin "baserom.gba", 0x00db35fb, 0x00000001
	.section .rom.00db3776, "ax"
	.incbin "baserom.gba", 0x00db3776, 0x00000002
	.section .rom.00dc293f, "ax"
	.incbin "baserom.gba", 0x00dc293f, 0x00000001
	.section .rom.00dc2aa7, "ax"
	.incbin "baserom.gba", 0x00dc2aa7, 0x00000001
	.section .rom.00dc50ca, "ax"
	.incbin "baserom.gba", 0x00dc50ca, 0x00000002
	.section .rom.00dc520d, "ax"
	.incbin "baserom.gba", 0x00dc520d, 0x00000003
	.section .rom.00dc6aca, "ax"
	.incbin "baserom.gba", 0x00dc6aca, 0x00000002
	.section .rom.00dc9a2e, "ax"
	.incbin "baserom.gba", 0x00dc9a2e, 0x00000002
	.section .rom.00dcc3c6, "ax"
	.incbin "baserom.gba", 0x00dcc3c6, 0x00000002
	.section .rom.00dd0d1e, "ax"
	.incbin "baserom.gba", 0x00dd0d1e, 0x00000002
	.section .rom.00dd3347, "ax"
	.incbin "baserom.gba", 0x00dd3347, 0x00000001
	.section .rom.00dd351b, "ax"
	.incbin "baserom.gba", 0x00dd351b, 0x00000001
	.section .rom.00dd683a, "ax"
	.incbin "baserom.gba", 0x00dd683a, 0x00000002
	.section .rom.00dd6a11, "ax"
	.incbin "baserom.gba", 0x00dd6a11, 0x00000003
	.section .rom.00dd9d85, "ax"
	.incbin "baserom.gba", 0x00dd9d85, 0x00000003
	.section .rom.00ddb4d9, "ax"
	.incbin "baserom.gba", 0x00ddb4d9, 0x00000003
	.section .rom.00ddc467, "ax"
	.incbin "baserom.gba", 0x00ddc467, 0x00000001
	.section .rom.00dddddd, "ax"
	.incbin "baserom.gba", 0x00dddddd, 0x00000003
	.section .rom.00dddf17, "ax"
	.incbin "baserom.gba", 0x00dddf17, 0x00000001
	.section .rom.00ddfa59, "ax"
	.incbin "baserom.gba", 0x00ddfa59, 0x00000003
	.section .rom.00de3031, "ax"
	.incbin "baserom.gba", 0x00de3031, 0x00000003
	.section .rom.00de3cbb, "ax"
	.incbin "baserom.gba", 0x00de3cbb, 0x00000001
	.section .rom.00de3dfb, "ax"
	.incbin "baserom.gba", 0x00de3dfb, 0x00000001
	.section .rom.00de5bbf, "ax"
	.incbin "baserom.gba", 0x00de5bbf, 0x00000001
	.section .rom.00de5d32, "ax"
	.incbin "baserom.gba", 0x00de5d32, 0x00000002
	.section .rom.00de819a, "ax"
	.incbin "baserom.gba", 0x00de819a, 0x00000002
	.section .rom.00de9b36, "ax"
	.incbin "baserom.gba", 0x00de9b36, 0x00000002
	.section .rom.00deb96b, "ax"
	.incbin "baserom.gba", 0x00deb96b, 0x00000001
	.section .rom.00dec422, "ax"
	.incbin "baserom.gba", 0x00dec422, 0x00000002
	.section .rom.00dede3e, "ax"
	.incbin "baserom.gba", 0x00dede3e, 0x00000002
	.section .rom.00df0b0a, "ax"
	.incbin "baserom.gba", 0x00df0b0a, 0x00000002
	.section .rom.00df0ce5, "ax"
	.incbin "baserom.gba", 0x00df0ce5, 0x00000003
	.section .rom.00df281b, "ax"
	.incbin "baserom.gba", 0x00df281b, 0x00000001
	.section .rom.00df58e5, "ax"
	.incbin "baserom.gba", 0x00df58e5, 0x00000003
	.section .rom.00df6aa1, "ax"
	.incbin "baserom.gba", 0x00df6aa1, 0x00000003
	.section .rom.00df6c89, "ax"
	.incbin "baserom.gba", 0x00df6c89, 0x00000003
	.section .rom.00df6e1b, "ax"
	.incbin "baserom.gba", 0x00df6e1b, 0x00000001
	.section .rom.00df7ba2, "ax"
	.incbin "baserom.gba", 0x00df7ba2, 0x00000002
	.section .rom.00df7d37, "ax"
	.incbin "baserom.gba", 0x00df7d37, 0x00000001
	.section .rom.00df9deb, "ax"
	.incbin "baserom.gba", 0x00df9deb, 0x00000001
	.section .rom.00e01e6e, "ax"
	.incbin "baserom.gba", 0x00e01e6e, 0x00000002
	.section .rom.00e01faf, "ax"
	.incbin "baserom.gba", 0x00e01faf, 0x00000001
	.section .rom.00e06337, "ax"
	.incbin "baserom.gba", 0x00e06337, 0x00000001
	.section .rom.00e06a56, "ax"
	.incbin "baserom.gba", 0x00e06a56, 0x00000002
	.section .rom.00e07e15, "ax"
	.incbin "baserom.gba", 0x00e07e15, 0x00000003
	.section .rom.00e0bf2f, "ax"
	.incbin "baserom.gba", 0x00e0bf2f, 0x00000001
	.section .rom.00e0cfaf, "ax"
	.incbin "baserom.gba", 0x00e0cfaf, 0x00000001
	.section .rom.00e0e031, "ax"
	.incbin "baserom.gba", 0x00e0e031, 0x00000003
	.section .rom.00e0eba2, "ax"
	.incbin "baserom.gba", 0x00e0eba2, 0x00000002
	.section .rom.00e0faf3, "ax"
	.incbin "baserom.gba", 0x00e0faf3, 0x00000001
	.section .rom.00e0fc71, "ax"
	.incbin "baserom.gba", 0x00e0fc71, 0x00000003
	.section .rom.00e131cb, "ax"
	.incbin "baserom.gba", 0x00e131cb, 0x00000001
	.section .rom.00e147fe, "ax"
	.incbin "baserom.gba", 0x00e147fe, 0x00000002
	.section .rom.00e1493f, "ax"
	.incbin "baserom.gba", 0x00e1493f, 0x00000001
	.section .rom.00e1728a, "ax"
	.incbin "baserom.gba", 0x00e1728a, 0x00000002
	.section .rom.00e1fc32, "ax"
	.incbin "baserom.gba", 0x00e1fc32, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fc34, 0x00000fdc
	.section .rom.00e227dd, "ax"
	.incbin "baserom.gba", 0x00e227dd, 0x00000003
	.section .rom.00e2471d, "ax"
	.incbin "baserom.gba", 0x00e2471d, 0x00000003
	.section .rom.00e25b91, "ax"
	.incbin "baserom.gba", 0x00e25b91, 0x00000003
	.section .rom.00e2784b, "ax"
	.incbin "baserom.gba", 0x00e2784b, 0x00000001
	.section .rom.00e348fa, "ax"
	.incbin "baserom.gba", 0x00e348fa, 0x00000002
	.section .rom.00e34a39, "ax"
	.incbin "baserom.gba", 0x00e34a39, 0x00000003
	.section .rom.00e3695e, "ax"
	.incbin "baserom.gba", 0x00e3695e, 0x00000002
	.section .rom.00e36a87, "ax"
	.incbin "baserom.gba", 0x00e36a87, 0x00000001
	.section .rom.00e38c57, "ax"
	.incbin "baserom.gba", 0x00e38c57, 0x00000001
	.section .rom.00e3b227, "ax"
	.incbin "baserom.gba", 0x00e3b227, 0x00000001
	.section .rom.00e3d539, "ax"
	.incbin "baserom.gba", 0x00e3d539, 0x00000003
	.section .rom.00e3ddad, "ax"
	.incbin "baserom.gba", 0x00e3ddad, 0x00000003
	.section .rom.00e3deef, "ax"
	.incbin "baserom.gba", 0x00e3deef, 0x00000001
	.section .rom.00e40fa5, "ax"
	.incbin "baserom.gba", 0x00e40fa5, 0x00000003
	.section .rom.00e4268b, "ax"
	.incbin "baserom.gba", 0x00e4268b, 0x00000001
	.section .rom.00e44253, "ax"
	.incbin "baserom.gba", 0x00e44253, 0x00000001
	.section .rom.00e45dce, "ax"
	.incbin "baserom.gba", 0x00e45dce, 0x00000002
	.section .rom.00e46219, "ax"
	.incbin "baserom.gba", 0x00e46219, 0x00000003
	.section .rom.00e4837a, "ax"
	.incbin "baserom.gba", 0x00e4837a, 0x00000002
	.section .rom.00e48493, "ax"
	.incbin "baserom.gba", 0x00e48493, 0x00000001
	.section .rom.00e492bd, "ax"
	.incbin "baserom.gba", 0x00e492bd, 0x00000003
	.section .rom.00e4941f, "ax"
	.incbin "baserom.gba", 0x00e4941f, 0x00000001
	.section .rom.00e4dc87, "ax"
	.incbin "baserom.gba", 0x00e4dc87, 0x00000001
	.section .rom.00e50345, "ax"
	.incbin "baserom.gba", 0x00e50345, 0x00000003
	.section .rom.00e50923, "ax"
	.incbin "baserom.gba", 0x00e50923, 0x00000001
	.section .rom.00e528b6, "ax"
	.incbin "baserom.gba", 0x00e528b6, 0x00000002
	.section .rom.00e54096, "ax"
	.incbin "baserom.gba", 0x00e54096, 0x00000002
	.section .rom.00e541fa, "ax"
	.incbin "baserom.gba", 0x00e541fa, 0x00000002
	.section .rom.00e569f2, "ax"
	.incbin "baserom.gba", 0x00e569f2, 0x00000002
	.section .rom.00e56bab, "ax"
	.incbin "baserom.gba", 0x00e56bab, 0x00000001
	.section .rom.00e56d33, "ax"
	.incbin "baserom.gba", 0x00e56d33, 0x00000001
	.section .rom.00e58555, "ax"
	.incbin "baserom.gba", 0x00e58555, 0x00000003
	.section .rom.00e5913e, "ax"
	.incbin "baserom.gba", 0x00e5913e, 0x00000002
	.section .rom.00e5a677, "ax"
	.incbin "baserom.gba", 0x00e5a677, 0x00000001
	.section .rom.00e5a812, "ax"
	.incbin "baserom.gba", 0x00e5a812, 0x00000002
	.section .rom.00e5cabe, "ax"
	.incbin "baserom.gba", 0x00e5cabe, 0x00000002
	.section .rom.00e5ec85, "ax"
	.incbin "baserom.gba", 0x00e5ec85, 0x00000003
	.section .rom.00e60383, "ax"
	.incbin "baserom.gba", 0x00e60383, 0x00000001
	.section .rom.00e60ea7, "ax"
	.incbin "baserom.gba", 0x00e60ea7, 0x00000001
	.section .rom.00e65d4a, "ax"
	.incbin "baserom.gba", 0x00e65d4a, 0x00000002
	.section .rom.00e680f1, "ax"
	.incbin "baserom.gba", 0x00e680f1, 0x00000003
	.section .rom.00e687da, "ax"
	.incbin "baserom.gba", 0x00e687da, 0x00000002
	.section .rom.00e693c6, "ax"
	.incbin "baserom.gba", 0x00e693c6, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e693c8, 0x00000ac4
	.section .rom.00e6b43b, "ax"
	.incbin "baserom.gba", 0x00e6b43b, 0x00000001
	.section .rom.00e6b5ee, "ax"
	.incbin "baserom.gba", 0x00e6b5ee, 0x00000002
	.section .rom.00e6d82d, "ax"
	.incbin "baserom.gba", 0x00e6d82d, 0x00000003
	.section .rom.00e6e353, "ax"
	.incbin "baserom.gba", 0x00e6e353, 0x00000001
	.section .rom.00e78de4, "ax"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78de4, 0x000007d0
	.section .rom.00e7c27d, "ax"
	.incbin "baserom.gba", 0x00e7c27d, 0x00000003
	.section .rom.00e7c3f7, "ax"
	.incbin "baserom.gba", 0x00e7c3f7, 0x00000001
	.section .rom.00e7c56e, "ax"
	.incbin "baserom.gba", 0x00e7c56e, 0x00000002
	.section .rom.00e7c706, "ax"
	.incbin "baserom.gba", 0x00e7c706, 0x00000002
	.section .rom.00e7c897, "ax"
	.incbin "baserom.gba", 0x00e7c897, 0x00000001
	.section .rom.00e7eb43, "ax"
	.incbin "baserom.gba", 0x00e7eb43, 0x00000001
	.section .rom.00e7ec5b, "ax"
	.incbin "baserom.gba", 0x00e7ec5b, 0x00000001
	.section .rom.00e80a5d, "ax"
	.incbin "baserom.gba", 0x00e80a5d, 0x00000003
	.section .rom.00e82522, "ax"
	.incbin "baserom.gba", 0x00e82522, 0x00000002
	.section .rom.00e82c4a, "ax"
	.incbin "baserom.gba", 0x00e82c4a, 0x00000002
	.section .rom.00e84046, "ax"
	.incbin "baserom.gba", 0x00e84046, 0x00000002
	.section .rom.00e84f1a, "ax"
	.incbin "baserom.gba", 0x00e84f1a, 0x00000002
	.section .rom.00e8505e, "ax"
	.incbin "baserom.gba", 0x00e8505e, 0x00000002
	.section .rom.00e8740f, "ax"
	.incbin "baserom.gba", 0x00e8740f, 0x00000001
	.section .rom.00e89c3d, "ax"
	.incbin "baserom.gba", 0x00e89c3d, 0x00000003
	.section .rom.00e8a03a, "ax"
	.incbin "baserom.gba", 0x00e8a03a, 0x00000002
	.section .rom.00e8b4da, "ax"
	.incbin "baserom.gba", 0x00e8b4da, 0x00000002
	.section .rom.00e8b62e, "ax"
	.incbin "baserom.gba", 0x00e8b62e, 0x00000002
	.section .rom.00e8ef7b, "ax"
	.incbin "baserom.gba", 0x00e8ef7b, 0x00000001
	.section .rom.00e903e6, "ax"
	.incbin "baserom.gba", 0x00e903e6, 0x00000002
	.section .rom.00e918bf, "ax"
	.incbin "baserom.gba", 0x00e918bf, 0x00000001
	.section .rom.00e91a12, "ax"
	.incbin "baserom.gba", 0x00e91a12, 0x00000002
	.section .rom.00e9541f, "ax"
	.incbin "baserom.gba", 0x00e9541f, 0x00000001
	.section .rom.00e965f7, "ax"
	.incbin "baserom.gba", 0x00e965f7, 0x00000001
	.section .rom.00e97e02, "ax"
	.incbin "baserom.gba", 0x00e97e02, 0x00000002
	.section .rom.00e97f52, "ax"
	.incbin "baserom.gba", 0x00e97f52, 0x00000002
	.section .rom.00e9a65f, "ax"
	.incbin "baserom.gba", 0x00e9a65f, 0x00000001
	.section .rom.00e9a7c6, "ax"
	.incbin "baserom.gba", 0x00e9a7c6, 0x00000002
	.section .rom.00e9d787, "ax"
	.incbin "baserom.gba", 0x00e9d787, 0x00000001
	.section .rom.00e9e9f5, "ax"
	.incbin "baserom.gba", 0x00e9e9f5, 0x00000003
	.section .rom.00ea1582, "ax"
	.incbin "baserom.gba", 0x00ea1582, 0x00000002
	.section .rom.00ea1f69, "ax"
	.incbin "baserom.gba", 0x00ea1f69, 0x00000003
	.section .rom.00ea290a, "ax"
	.incbin "baserom.gba", 0x00ea290a, 0x00000002
	.section .rom.00ea2a7e, "ax"
	.incbin "baserom.gba", 0x00ea2a7e, 0x00000002
	.section .rom.00ea458f, "ax"
	.incbin "baserom.gba", 0x00ea458f, 0x00000001
	.section .rom.00ea46b1, "ax"
	.incbin "baserom.gba", 0x00ea46b1, 0x00000003
	.section .rom.00ea71af, "ax"
	.incbin "baserom.gba", 0x00ea71af, 0x00000001
	.section .rom.00ea7fa6, "ax"
	.incbin "baserom.gba", 0x00ea7fa6, 0x00000002
	.section .rom.00ea9f45, "ax"
	.incbin "baserom.gba", 0x00ea9f45, 0x00000003
	.section .rom.00eab275, "ax"
	.incbin "baserom.gba", 0x00eab275, 0x00000003
	.section .rom.00eab3c9, "ax"
	.incbin "baserom.gba", 0x00eab3c9, 0x00000003
	.section .rom.00eabf7b, "ax"
	.incbin "baserom.gba", 0x00eabf7b, 0x00000001
	.section .rom.00ead755, "ax"
	.incbin "baserom.gba", 0x00ead755, 0x00000003
	.section .rom.00eae8b5, "ax"
	.incbin "baserom.gba", 0x00eae8b5, 0x00000003
	.section .rom.00eaf9d3, "ax"
	.incbin "baserom.gba", 0x00eaf9d3, 0x00000001
	.section .rom.00eafbda, "ax"
	.incbin "baserom.gba", 0x00eafbda, 0x00000002
	.section .rom.00eb08cf, "ax"
	.incbin "baserom.gba", 0x00eb08cf, 0x00000001
	.section .rom.00eb3071, "ax"
	.incbin "baserom.gba", 0x00eb3071, 0x00000003
	.section .rom.00eb47e6, "ax"
	.incbin "baserom.gba", 0x00eb47e6, 0x00000002
	.section .rom.00eb5aa3, "ax"
	.incbin "baserom.gba", 0x00eb5aa3, 0x00000001
	.section .rom.00eb62e6, "ax"
	.incbin "baserom.gba", 0x00eb62e6, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb62e8, 0x0000060c
	.section .rom.00eb6e6b, "ax"
	.incbin "baserom.gba", 0x00eb6e6b, 0x00000001
	.section .rom.00eb6fab, "ax"
	.incbin "baserom.gba", 0x00eb6fab, 0x00000001
	.section .rom.00eb70eb, "ax"
	.incbin "baserom.gba", 0x00eb70eb, 0x00000001
	.section .rom.00eb7c4d, "ax"
	.incbin "baserom.gba", 0x00eb7c4d, 0x00000003
	.section .rom.00eba3f1, "ax"
	.incbin "baserom.gba", 0x00eba3f1, 0x00000003
	.section .rom.00ebbb66, "ax"
	.incbin "baserom.gba", 0x00ebbb66, 0x00000002
	.section .rom.00ebce23, "ax"
	.incbin "baserom.gba", 0x00ebce23, 0x00000001
	.section .rom.00ebd666, "ax"
	.incbin "baserom.gba", 0x00ebd666, 0x00000002
	.section .rom.00ebdc05, "ax"
	.incbin "baserom.gba", 0x00ebdc05, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdc08, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdc14, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebdd64, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebdea4, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebdfe4, 0x00000140
	.section .rom.00ebe839, "ax"
	.incbin "baserom.gba", 0x00ebe839, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe83c, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe848, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebe998, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebead8, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebec18, 0x00000140
	.section .rom.00ebf919, "ax"
	.incbin "baserom.gba", 0x00ebf919, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf91c, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf928, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebfa78, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebfbb8, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebfcf8, 0x00000140
	.section .rom.00ec0405, "ax"
	.incbin "baserom.gba", 0x00ec0405, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec0408, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec0414, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec0564, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec06a4, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec07e4, 0x00000140
	.section .rom.00ec12a1, "ax"
	.incbin "baserom.gba", 0x00ec12a1, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec12a4, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec12b0, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec1400, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec1540, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1680, 0x00000140
	.section .rom.00ec1be9, "ax"
	.incbin "baserom.gba", 0x00ec1be9, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1bec, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1bf8, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec1d48, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1e88, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec1fc8, 0x00000140
	.section .rom.00ec2731, "ax"
	.incbin "baserom.gba", 0x00ec2731, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec2734, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec2740, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2890, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec29d0, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2b10, 0x00000140
	.section .rom.00ec3379, "ax"
	.incbin "baserom.gba", 0x00ec3379, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec337c, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec3388, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec34d8, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec3618, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec3758, 0x00000140
	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "baserom.gba", 0x00ec3898, 0x00000490
	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "baserom.gba", 0x00ec3d28, 0x00001d0c
	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "baserom.gba", 0x00ec5a34, 0x00003df0
	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "baserom.gba", 0x00ec9824, 0x00002df0
	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "baserom.gba", 0x00ecc614, 0x000019ec
	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "baserom.gba", 0x00ece000, 0x000011b4
	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "baserom.gba", 0x00ecf1b4, 0x000007c4
	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "baserom.gba", 0x00ecf978, 0x00001860
	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "baserom.gba", 0x00ed11d8, 0x000009d4
	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "baserom.gba", 0x00ed1bac, 0x00000b34
	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "baserom.gba", 0x00ed26e0, 0x00003234
	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "baserom.gba", 0x00ed5914, 0x000024bc
	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "baserom.gba", 0x00ed7dd0, 0x00002da8
	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "baserom.gba", 0x00edab78, 0x00001cf8
	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "baserom.gba", 0x00edc870, 0x0000114c
	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "baserom.gba", 0x00edd9bc, 0x00001608
	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "baserom.gba", 0x00edefc4, 0x000014b0
	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "baserom.gba", 0x00ee0474, 0x00000e44
	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "baserom.gba", 0x00ee12b8, 0x00000274
	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "baserom.gba", 0x00ee152c, 0x000001e4
	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "baserom.gba", 0x00ee1710, 0x000006e4
	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "baserom.gba", 0x00ee1df4, 0x00000570
	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "baserom.gba", 0x00ee2364, 0x000025a4
	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "baserom.gba", 0x00ee4908, 0x00000710
	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "baserom.gba", 0x00ee5018, 0x00003220
	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "baserom.gba", 0x00ee8238, 0x000014bc
	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "baserom.gba", 0x00ee96f4, 0x00002950
	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "baserom.gba", 0x00eec044, 0x00004154
	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "baserom.gba", 0x00ef0198, 0x00001520
	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "baserom.gba", 0x00ef16b8, 0x00000a9c
	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "baserom.gba", 0x00ef2154, 0x00000bf8
	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "baserom.gba", 0x00ef2d4c, 0x00002984
	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "baserom.gba", 0x00ef56d0, 0x00001458
	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "baserom.gba", 0x00ef6b28, 0x00000dac
	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "baserom.gba", 0x00ef78d4, 0x00001274
	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "baserom.gba", 0x00ef8b48, 0x000001d8
	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "baserom.gba", 0x00ef8d20, 0x000005a8
	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "baserom.gba", 0x00ef92c8, 0x00000a98
	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "baserom.gba", 0x00ef9d60, 0x00000de4
	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "baserom.gba", 0x00efab44, 0x0000374c
	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "baserom.gba", 0x00efe290, 0x00004050
	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "baserom.gba", 0x00f022e0, 0x000012e4
	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "baserom.gba", 0x00f035c4, 0x00000a68
	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "baserom.gba", 0x00f0402c, 0x00002b2c
	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "baserom.gba", 0x00f06b58, 0x00000d94
	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "baserom.gba", 0x00f078ec, 0x000015c8
	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "baserom.gba", 0x00f08eb4, 0x00000714
	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "baserom.gba", 0x00f095c8, 0x00001eec
	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "baserom.gba", 0x00f0b4b4, 0x000018e0
	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "baserom.gba", 0x00f0cd94, 0x0000010c
	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "baserom.gba", 0x00f0cea0, 0x00001fe0
	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "baserom.gba", 0x00f0ee80, 0x00003368
	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "baserom.gba", 0x00f121e8, 0x00001168
	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "baserom.gba", 0x00f13350, 0x00000a88
	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "baserom.gba", 0x00f13dd8, 0x00000fc0
	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "baserom.gba", 0x00f14d98, 0x00000d10
	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "baserom.gba", 0x00f15aa8, 0x00003840
	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "baserom.gba", 0x00f192e8, 0x000014c8
	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "baserom.gba", 0x00f1a7b0, 0x0000035c
	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "baserom.gba", 0x00f1ab0c, 0x000000d0
	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "baserom.gba", 0x00f1abdc, 0x00001ffc
	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "baserom.gba", 0x00f1cbd8, 0x00002190
	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "baserom.gba", 0x00f1ed68, 0x000016e0
	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "baserom.gba", 0x00f20448, 0x000004a0
	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "baserom.gba", 0x00f208e8, 0x000003e8
	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "baserom.gba", 0x00f20cd0, 0x00000c10
	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "baserom.gba", 0x00f218e0, 0x000012e8
	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "baserom.gba", 0x00f22bc8, 0x00001228
	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "baserom.gba", 0x00f23df0, 0x00001c28
	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "baserom.gba", 0x00f25a18, 0x00000df8
	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "baserom.gba", 0x00f26810, 0x0000246c
	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "baserom.gba", 0x00f28c7c, 0x00001910
	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "baserom.gba", 0x00f2a58c, 0x00002cc0
	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "baserom.gba", 0x00f2d24c, 0x000039e8
	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "baserom.gba", 0x00f30c34, 0x00000be4
	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "baserom.gba", 0x00f31818, 0x00002e00
	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "baserom.gba", 0x00f34618, 0x00000b1c
	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "baserom.gba", 0x00f35134, 0x00003310
	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "baserom.gba", 0x00f38444, 0x00004450
	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "baserom.gba", 0x00f3c894, 0x00001978
	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "baserom.gba", 0x00f3e20c, 0x00001af8
	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "baserom.gba", 0x00f3fd04, 0x000019ac
	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "baserom.gba", 0x00f416b0, 0x00001efc
	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "baserom.gba", 0x00f435ac, 0x00002140
	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "baserom.gba", 0x00f456ec, 0x00001cb0
	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "baserom.gba", 0x00f4739c, 0x00002468
	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "baserom.gba", 0x00f49804, 0x00001a84
	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "baserom.gba", 0x00f4b288, 0x000023b0
	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "baserom.gba", 0x00f4d638, 0x00002058
	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "baserom.gba", 0x00f4f690, 0x00001b4c
	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "baserom.gba", 0x00f511dc, 0x000032b8
	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "baserom.gba", 0x00f54494, 0x0000245c
	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "baserom.gba", 0x00f568f0, 0x000037a4
	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "baserom.gba", 0x00f5a094, 0x00000fc4
	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "baserom.gba", 0x00f5b058, 0x00000810
	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "baserom.gba", 0x00f5b868, 0x00002d5c
	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "baserom.gba", 0x00f5e5c4, 0x00000d70
	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "baserom.gba", 0x00f5f334, 0x00003c08
	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "baserom.gba", 0x00f62f3c, 0x00002f0c
	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "baserom.gba", 0x00f65e48, 0x00002274
	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "baserom.gba", 0x00f680bc, 0x000045b0
	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "baserom.gba", 0x00f6c66c, 0x00003344
	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "baserom.gba", 0x00f6f9b0, 0x00000d80
	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "baserom.gba", 0x00f70730, 0x000028b4
	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "baserom.gba", 0x00f72fe4, 0x000015ac
	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "baserom.gba", 0x00f74590, 0x00000430
	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "baserom.gba", 0x00f749c0, 0x00000554
	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "baserom.gba", 0x00f74f14, 0x00000e50
	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "baserom.gba", 0x00f75d64, 0x000013f8
	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "baserom.gba", 0x00f7715c, 0x000000b0
	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "baserom.gba", 0x00f7720c, 0x00000774
	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "baserom.gba", 0x00f77980, 0x00000cd4
	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "baserom.gba", 0x00f78654, 0x00000518
	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "baserom.gba", 0x00f78b6c, 0x00087494
