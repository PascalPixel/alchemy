@ tla-it's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x0003aab0, 0x00000534
	.section .rom.0003afe4, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003afe4, 0x000008a0
	.section .rom.0003b89c, "ax"
	.global UiText_GetResourceDimensions
	.type UiText_GetResourceDimensions, %function
	.thumb_func
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b89c, 0x0000004c
	.section .rom.0003b8e8, "ax"
	.global Func_0803b8cc
	.type Func_0803b8cc, %function
	.thumb_func
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8e8, 0x0000004c
	.section .rom.0003b934, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b934, 0x00000734
	.section .rom.0003c068, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c068, 0x00000108
	.section .rom.0003c170, "ax"
	.global Func_0803c170
	.type Func_0803c170, %function
	.thumb_func
Func_0803c170:
	.incbin "baserom.gba", 0x0003c170, 0x00000028
	.section .rom.0003c198, "ax"
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c198, 0x000001e0
	.section .rom.0003c378, "ax"
	.global UiWindow_SetTilemapEntry
	.type UiWindow_SetTilemapEntry, %function
	.thumb_func
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c378, 0x00000634
	.section .rom.0003c9bc, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9bc, 0x00000064
	.section .rom.0003ca20, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca20, 0x00000144
	.section .rom.0003cb72, "ax"
	.incbin "baserom.gba", 0x0003cb72, 0x00000002
	.section .rom.0003cb74, "ax"
	.global Func_0803cb1c
	.type Func_0803cb1c, %function
	.thumb_func
Func_0803cb1c:
	.incbin "baserom.gba", 0x0003cb74, 0x0000018c
	.section .rom.0003cd00, "ax"
	.global Func_0803cca8
	.type Func_0803cca8, %function
	.thumb_func
Func_0803cca8:
	.incbin "baserom.gba", 0x0003cd00, 0x00000028
	.section .rom.0003cd28, "ax"
	.global Func_0803ccd0
	.type Func_0803ccd0, %function
	.thumb_func
Func_0803ccd0:
	.incbin "baserom.gba", 0x0003cd28, 0x0000014c
	.section .rom.0003ce74, "ax"
	.global Func_0803ce1c
	.type Func_0803ce1c, %function
	.thumb_func
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce74, 0x00000048
	.section .rom.0003cebc, "ax"
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cebc, 0x000001bc
	.section .rom.0003d078, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d078, 0x000002d0
	.section .rom.0003d348, "ax"
	.global Localization_LookupEntryId
	.type Localization_LookupEntryId, %function
	.thumb_func
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d348, 0x000000d0
	.section .rom.0003d418, "ax"
	.global Func_0803d3c0
	.type Func_0803d3c0, %function
	.thumb_func
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d418, 0x00000090
	.section .rom.0003d4a8, "ax"
	.global Func_0803d450
	.type Func_0803d450, %function
	.thumb_func
Func_0803d450:
	.incbin "baserom.gba", 0x0003d4a8, 0x0000006c
	.section .rom.0003d53c, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d53c, 0x000000e0
	.section .rom.0003d61c, "ax"
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d61c, 0x000000bc
	.section .rom.0003d6d8, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6d8, 0x0000022c
	.section .rom.0003d982, "ax"
	.incbin "baserom.gba", 0x0003d982, 0x00000002
	.section .rom.0003d984, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d984, 0x00000060
	.section .rom.0003d9e4, "ax"
	.global Ability_LoadGlyph
	.type Ability_LoadGlyph, %function
	.thumb_func
Ability_LoadGlyph:
	.incbin "baserom.gba", 0x0003d9e4, 0x00000030
	.section .rom.0003da14, "ax"
	.global Func_0803d9bc
	.type Func_0803d9bc, %function
	.thumb_func
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da14, 0x000000bc
	.section .rom.0003dad0, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.type Ui_PrepareTransferFromTableEntry, %function
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dad0, 0x000001a4
	.section .rom.0003dc74, "ax"
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc74, 0x00000108
	.section .rom.0003dd7c, "ax"
	.global Func_0803dd24
	.type Func_0803dd24, %function
	.thumb_func
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd7c, 0x00000044
	.section .rom.0003ddc0, "ax"
	.global Func_0803dd68
	.type Func_0803dd68, %function
	.thumb_func
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddc0, 0x00000030
	.section .rom.0003ddf0, "ax"
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddf0, 0x00000110
	.section .rom.0003df00, "ax"
	.global Func_0803dea8
	.type Func_0803dea8, %function
	.thumb_func
Func_0803dea8:
	.incbin "baserom.gba", 0x0003df00, 0x00000058
	.section .rom.0003df58, "ax"
	.global Func_0803df00
	.type Func_0803df00, %function
	.thumb_func
Func_0803df00:
	.incbin "baserom.gba", 0x0003df58, 0x00000014
	.section .rom.0003df6c, "ax"
	.global Resource_ScheduleOwnerReset
	.type Resource_ScheduleOwnerReset, %function
	.thumb_func
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df6c, 0x00000694
	.section .rom.0003e730, "ax"
	.global Func_0803e6d8
	.type Func_0803e6d8, %function
	.thumb_func
Func_0803e6d8:
	.incbin "baserom.gba", 0x0003e730, 0x0000009c
	.section .rom.0003e7cc, "ax"
	.global Func_0803e774
	.type Func_0803e774, %function
	.thumb_func
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7cc, 0x00000038
	.section .rom.0003e804, "ax"
	.global Func_0803e7ac
	.type Func_0803e7ac, %function
	.thumb_func
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e804, 0x00000140
	.section .rom.0003e944, "ax"
	.global NodeChain_GetNodeAtCount
	.type NodeChain_GetNodeAtCount, %function
	.thumb_func
NodeChain_GetNodeAtCount:
	.incbin "baserom.gba", 0x0003e944, 0x0000002c
	.section .rom.0003e970, "ax"
	.global Func_0803e918
	.type Func_0803e918, %function
	.thumb_func
Func_0803e918:
	.incbin "baserom.gba", 0x0003e970, 0x00000080
	.section .rom.0003e9f0, "ax"
	.global Func_0803e998
	.type Func_0803e998, %function
	.thumb_func
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9f0, 0x0000063c
	.section .rom.0003f05c, "ax"
	.incbin "baserom.gba", 0x0003f05c, 0x000001d0
	.section .rom.0003f22c, "ax"
	.global Resource_LoadByMode
	.type Resource_LoadByMode, %function
	.thumb_func
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f22c, 0x00000060
	.section .rom.0003f28c, "ax"
	.global Func_0803f234
	.type Func_0803f234, %function
	.thumb_func
Func_0803f234:
	.incbin "baserom.gba", 0x0003f28c, 0x000003dc
	.section .rom.0003f668, "ax"
	.global Func_0803f610
	.type Func_0803f610, %function
	.thumb_func
Func_0803f610:
	.incbin "baserom.gba", 0x0003f668, 0x00000004
	.section .rom.0003f66c, "ax"
	.global Func_0803f614
	.type Func_0803f614, %function
	.thumb_func
Func_0803f614:
	.incbin "baserom.gba", 0x0003f66c, 0x0000000c
	.section .rom.0003f678, "ax"
	.global Func_0803f620
	.type Func_0803f620, %function
	.thumb_func
Func_0803f620:
	.incbin "baserom.gba", 0x0003f678, 0x00000004
	.section .rom.0003f67c, "ax"
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f67c, 0x00000074
	.section .rom.0003f6f0, "ax"
	.global UiTextResource_SetPosition
	.type UiTextResource_SetPosition, %function
	.thumb_func
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6f0, 0x00000028
	.section .rom.0003f718, "ax"
	.global UiTextResource_Release
	.type UiTextResource_Release, %function
	.thumb_func
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f718, 0x000000b8
	.section .rom.0003f7d0, "ax"
	.global Func_0803f778
	.type Func_0803f778, %function
	.thumb_func
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7d0, 0x000000f4
	.section .rom.0003f8c4, "ax"
	.global Func_0803f86c
	.type Func_0803f86c, %function
	.thumb_func
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8c4, 0x000000d0
	.section .rom.0003f994, "ax"
	.global Func_0803f93c
	.type Func_0803f93c, %function
	.thumb_func
Func_0803f93c:
	.incbin "baserom.gba", 0x0003f994, 0x0000002c
	.section .rom.0003f9c0, "ax"
	.global Func_0803f968
	.type Func_0803f968, %function
	.thumb_func
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9c0, 0x00000058
	.section .rom.0003fa18, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa18, 0x00000728
	.section .rom.00040140, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x00040140, 0x00001a8c
	.section .rom.00041bcc, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bcc, 0x000000a4
	.section .rom.00041c70, "ax"
	.global Func_08041c0c
	.type Func_08041c0c, %function
	.thumb_func
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c70, 0x00000048
	.section .rom.00041cb8, "ax"
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041cb8, 0x0000031c
	.section .rom.00041fd4, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x00041fd4, 0x00000020
	.section .rom.00041ff4, "ax"
	.global Func_08041f90
	.type Func_08041f90, %function
	.thumb_func
Func_08041f90:
	.incbin "baserom.gba", 0x00041ff4, 0x00000014
	.section .rom.00042008, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x00042008, 0x0000006c
	.section .rom.00042074, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042074, 0x00000098
	.section .rom.0004210c, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x0004210c, 0x00000054
	.section .rom.00042160, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042160, 0x0000008c
	.section .rom.000421ec, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421ec, 0x0000005c
	.section .rom.00042248, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042248, 0x00000030
	.section .rom.00042278, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042278, 0x00000030
	.section .rom.000422a8, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x000422a8, 0x000000d0
	.section .rom.00042378, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042378, 0x00000088
	.section .rom.000424b4, "ax"
	.global Func_08042450
	.type Func_08042450, %function
	.thumb_func
Func_08042450:
	.incbin "baserom.gba", 0x000424b4, 0x000000b8
	.section .rom.0004256c, "ax"
	.global Func_08042508
	.type Func_08042508, %function
	.thumb_func
Func_08042508:
	.incbin "baserom.gba", 0x0004256c, 0x00000064
	.section .rom.000425de, "ax"
	.incbin "baserom.gba", 0x000425de, 0x00000002
	.section .rom.000425e0, "ax"
	.global Func_0804257c
	.type Func_0804257c, %function
	.thumb_func
Func_0804257c:
	.incbin "baserom.gba", 0x000425e0, 0x0000000c
	.section .rom.000425ec, "ax"
	.global Func_08042588
	.type Func_08042588, %function
	.thumb_func
Func_08042588:
	.incbin "baserom.gba", 0x000425ec, 0x00000074
	.section .rom.00042694, "ax"
	.incbin "baserom.gba", 0x00042694, 0x00000060
	.section .rom.000426f4, "ax"
	.global Func_08042690
	.type Func_08042690, %function
	.thumb_func
Func_08042690:
	.incbin "baserom.gba", 0x000426f4, 0x000002ec
	.section .rom.000429e0, "ax"
	.global Func_0804297c
	.type Func_0804297c, %function
	.thumb_func
Func_0804297c:
	.incbin "baserom.gba", 0x000429e0, 0x00000430
	.section .rom.00042e10, "ax"
	.global Func_08042dac
	.type Func_08042dac, %function
	.thumb_func
Func_08042dac:
	.incbin "baserom.gba", 0x00042e10, 0x00000318
	.section .rom.00043128, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043128, 0x00000198
	.section .rom.000432c0, "ax"
	.global Func_0804325c
	.type Func_0804325c, %function
	.thumb_func
Func_0804325c:
	.incbin "baserom.gba", 0x000432c0, 0x00000048
	.section .rom.00043308, "ax"
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x00043308, 0x00000248
	.section .rom.00043550, "ax"
	.global Func_080434ec
	.type Func_080434ec, %function
	.thumb_func
Func_080434ec:
	.incbin "baserom.gba", 0x00043550, 0x00000048
	.section .rom.00043598, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043598, 0x000000c8
	.section .rom.00043660, "ax"
	.global Func_080435fc
	.type Func_080435fc, %function
	.thumb_func
Func_080435fc:
	.incbin "baserom.gba", 0x00043660, 0x00000094
	.section .rom.000436f4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436f4, 0x000000e4
	.section .rom.000437d8, "ax"
	.global Func_08043774
	.type Func_08043774, %function
	.thumb_func
Func_08043774:
	.incbin "baserom.gba", 0x000437d8, 0x00000098
	.section .rom.00043870, "ax"
	.global Func_0804380c
	.type Func_0804380c, %function
	.thumb_func
Func_0804380c:
	.incbin "baserom.gba", 0x00043870, 0x000000bc
	.section .rom.0004392c, "ax"
	.global Func_080438c8
	.type Func_080438c8, %function
	.thumb_func
Func_080438c8:
	.incbin "baserom.gba", 0x0004392c, 0x000000b0
	.section .rom.00043a28, "ax"
	.incbin "baserom.gba", 0x00043a28, 0x00000228
	.section .rom.00043c94, "ax"
	.incbin "baserom.gba", 0x00043c94, 0x00000830
	.section .rom.000444c4, "ax"
	.global Func_08044460
	.type Func_08044460, %function
	.thumb_func
Func_08044460:
	.incbin "baserom.gba", 0x000444c4, 0x00000018
	.section .rom.000444dc, "ax"
	.global Func_08044478
	.type Func_08044478, %function
	.thumb_func
Func_08044478:
	.incbin "baserom.gba", 0x000444dc, 0x00000010
	.section .rom.000444ec, "ax"
	.global Func_08044488
	.type Func_08044488, %function
	.thumb_func
Func_08044488:
	.incbin "baserom.gba", 0x000444ec, 0x000000d0
	.section .rom.000445bc, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445bc, 0x000004fc
	.section .rom.00044ab8, "ax"
	.global Func_08044a54
	.type Func_08044a54, %function
	.thumb_func
Func_08044a54:
	.incbin "baserom.gba", 0x00044ab8, 0x00000004
	.section .rom.00044abc, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044abc, 0x00000140
	.section .rom.00044bfc, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044bfc, 0x000000e8
	.section .rom.00044ce4, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044ce4, 0x00000148
	.section .rom.00044e2c, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e2c, 0x00000484
	.section .rom.0004531e, "ax"
	.incbin "baserom.gba", 0x0004531e, 0x00000076
	.section .rom.00045394, "ax"
	.global Func_08045330
	.type Func_08045330, %function
	.thumb_func
Func_08045330:
	.incbin "baserom.gba", 0x00045394, 0x000000a0
	.section .rom.00045434, "ax"
	.global Func_080453d0
	.type Func_080453d0, %function
	.thumb_func
Func_080453d0:
	.incbin "baserom.gba", 0x00045434, 0x00000094
	.section .rom.0004558a, "ax"
	.incbin "baserom.gba", 0x0004558a, 0x0000002a
	.section .rom.000455c8, "ax"
	.incbin "baserom.gba", 0x000455c8, 0x00000204
	.section .rom.000457e2, "ax"
	.incbin "baserom.gba", 0x000457e2, 0x00000032
	.section .rom.00045832, "ax"
	.incbin "baserom.gba", 0x00045832, 0x00000966
	.section .rom.00046198, "ax"
	.global Func_08046134
	.type Func_08046134, %function
	.thumb_func
Func_08046134:
	.incbin "baserom.gba", 0x00046198, 0x00000094
	.section .rom.0004622c, "ax"
	.global Func_080461c8
	.type Func_080461c8, %function
	.thumb_func
Func_080461c8:
	.incbin "baserom.gba", 0x0004622c, 0x00000098
	.section .rom.000462e8, "ax"
	.incbin "baserom.gba", 0x000462e8, 0x00000150
	.section .rom.00046476, "ax"
	.incbin "baserom.gba", 0x00046476, 0x0000364e
	.section .rom.00049b12, "ax"
	.incbin "baserom.gba", 0x00049b12, 0x00001f5e
	.section .rom.0004baaa, "ax"
	.incbin "baserom.gba", 0x0004baaa, 0x00000352
	.section .rom.0004bdfc, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004bdfc, 0x000014d4
	.section .rom.0004d2d0, "ax"
	.global AffineEffect_InitializeWork
	.type AffineEffect_InitializeWork, %function
	.thumb_func
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d2d0, 0x0000003c
	.section .rom.0004d30c, "ax"
	.global Menu_EndResourceSelection
	.type Menu_EndResourceSelection, %function
	.thumb_func
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d30c, 0x00000054
	.section .rom.0004d360, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d360, 0x00000220
	.section .rom.0004d580, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x0004d580, 0x0000005c
	.section .rom.0004d5dc, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5dc, 0x00000188
	.section .rom.0004d764, "ax"
	.global Menu_AnimateSelectionToEntry
	.type Menu_AnimateSelectionToEntry, %function
	.thumb_func
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d764, 0x0000003c
	.section .rom.0004d7a0, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7a0, 0x00000228
	.section .rom.0004d9c8, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9c8, 0x0000016c
	.section .rom.0004dc08, "ax"
	.global Func_0804da3c
	.type Func_0804da3c, %function
	.thumb_func
Func_0804da3c:
	.incbin "baserom.gba", 0x0004dc08, 0x00000050
	.section .rom.0004dc86, "ax"
	.incbin "baserom.gba", 0x0004dc86, 0x000000ba
	.section .rom.0004dd40, "ax"
	.global Func_0804db74
	.type Func_0804db74, %function
	.thumb_func
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd40, 0x00000254
	.section .rom.0004e06c, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e06c, 0x000000c4
	.section .rom.0004e130, "ax"
	.global Menu_HandleFlagGridInput
	.type Menu_HandleFlagGridInput, %function
	.thumb_func
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e130, 0x00000140
	.section .rom.0004e29a, "ax"
	.incbin "baserom.gba", 0x0004e29a, 0x00000002
	.section .rom.0004e29c, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e29c, 0x00000108
	.section .rom.0004e3a4, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3a4, 0x0000021c
	.section .rom.0004e5c0, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e5c0, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed24, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f2f0, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054be0, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00054ff0, 0x000056f4
	.section .rom.0005c2e4, "ax"
	.incbin "baserom.gba", 0x0005c2e4, 0x00003888
	.section .rom.000a3134, "ax"
	.incbin "baserom.gba", 0x000a3134, 0x00000410
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a3544, 0x00009abc
	.section .rom.000ad000, "ax"
	.global Trade_GetOfferStateFar
	.type Trade_GetOfferStateFar, %function
	.thumb_func
Trade_GetOfferStateFar:
	.global Resource_FarCall005
Resource_FarCall005:
	.incbin "baserom.gba", 0x000ad000, 0x00000008
	.section .rom.000ad008, "ax"
	.global Owner_RecalculateStatsFar
	.type Owner_RecalculateStatsFar, %function
	.thumb_func
Owner_RecalculateStatsFar:
	.incbin "baserom.gba", 0x000ad008, 0x00000008
	.section .rom.000ad010, "ax"
	.global Item_Get
	.type Item_Get, %function
	.thumb_func
Item_Get:
	.incbin "baserom.gba", 0x000ad010, 0x00000010
	.section .rom.000ad020, "ax"
	.global Inventory_AddItemFar
	.type Inventory_AddItemFar, %function
	.thumb_func
Inventory_AddItemFar:
	.incbin "baserom.gba", 0x000ad020, 0x00000030
	.section .rom.000ad050, "ax"
	.global Inventory_RemoveFar
	.type Inventory_RemoveFar, %function
	.thumb_func
Inventory_RemoveFar:
	.incbin "baserom.gba", 0x000ad050, 0x00000028
	.section .rom.000ad078, "ax"
	.global BattleAction_Get
	.type BattleAction_Get, %function
	.thumb_func
BattleAction_Get:
	.incbin "baserom.gba", 0x000ad078, 0x00000008
	.section .rom.000ad080, "ax"
	.global OwnerAction_AddFar
	.type OwnerAction_AddFar, %function
	.thumb_func
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000ad080, 0x00000008
	.section .rom.000ad088, "ax"
	.global Equipment_HasValueFar
	.type Equipment_HasValueFar, %function
	.thumb_func
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000ad088, 0x00000068
	.global Party_CountActiveOwnersFar
	.type Party_CountActiveOwnersFar, %function
	.thumb_func
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000ad0f0, 0x00000008
	.section .rom.000ad0f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000ad0f8, 0x00000018
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000ad110, 0x00000038
	.section .rom.000ad148, "ax"
	.global BattleRandom16Far
	.type BattleRandom16Far, %function
	.thumb_func
BattleRandom16Far:
	.incbin "baserom.gba", 0x000ad148, 0x00000008
	.section .rom.000ad150, "ax"
	.global Djinn_AddToOwnerFar
	.type Djinn_AddToOwnerFar, %function
	.thumb_func
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000ad150, 0x00000008
	.section .rom.000ad158, "ax"
	.global Djinn_ActivateFar
	.type Djinn_ActivateFar, %function
	.thumb_func
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000ad158, 0x00000010
	.section .rom.000ad168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000ad168, 0x00000058
	.section .rom.000ad1c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000ad1c0, 0x000000c0
	.section .rom.000ad280, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000ad280, 0x000000c8
	.section .rom.000ad348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000ad348, 0x0000007c
	.section .rom.000ad3f6, "ax"
	.incbin "baserom.gba", 0x000ad3f6, 0x00000002
	.section .rom.000ad3f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000ad3f8, 0x00001448
	.section .rom.000ae874, "ax"
	.incbin "baserom.gba", 0x000ae874, 0x000001c8
	.section .rom.000aec72, "ax"
	.incbin "baserom.gba", 0x000aec72, 0x00000002
	.section .rom.000aec74, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000aec74, 0x00000060
	.section .rom.000aee4c, "ax"
	.incbin "baserom.gba", 0x000aee4c, 0x00000058
	.section .rom.000aeea4, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000aeea4, 0x00000030
	.section .rom.000aef40, "ax"
	.incbin "baserom.gba", 0x000aef40, 0x000000d4
	.section .rom.000af0ee, "ax"
	.incbin "baserom.gba", 0x000af0ee, 0x00000066
	.section .rom.000af154, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000af154, 0x00000080
	.section .rom.000af206, "ax"
	.incbin "baserom.gba", 0x000af206, 0x0000009e
	.section .rom.000af2cc, "ax"
	.incbin "baserom.gba", 0x000af2cc, 0x00000028
	.section .rom.000af344, "ax"
	.incbin "baserom.gba", 0x000af344, 0x00000040
	.section .rom.000af384, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000af384, 0x00000028
	.section .rom.000af448, "ax"
	.incbin "baserom.gba", 0x000af448, 0x000004e0
	.section .rom.000af928, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000af928, 0x00000298
	.section .rom.000afbf8, "ax"
	.incbin "baserom.gba", 0x000afbf8, 0x000001d0
	.section .rom.000afde4, "ax"
	.incbin "baserom.gba", 0x000afde4, 0x000000a0
	.section .rom.000afebc, "ax"
	.incbin "baserom.gba", 0x000afebc, 0x00000024
	.section .rom.000aff34, "ax"
	.incbin "baserom.gba", 0x000aff34, 0x00000054
	.section .rom.000affa0, "ax"
	.incbin "baserom.gba", 0x000affa0, 0x000002f4
	.section .rom.000b02a4, "ax"
	.incbin "baserom.gba", 0x000b02a4, 0x000001c8
	.section .rom.000b04c6, "ax"
	.incbin "baserom.gba", 0x000b04c6, 0x000005da
	.section .rom.000b0b84, "ax"
	.global Djinn_AddToOwner
	.type Djinn_AddToOwner, %function
	.thumb_func
Djinn_AddToOwner:
	.incbin "baserom.gba", 0x000b0b84, 0x00000100
	.section .rom.000b0ca6, "ax"
	.incbin "baserom.gba", 0x000b0ca6, 0x00000002
	.section .rom.000b0ca8, "ax"
	.global Djinn_Activate
	.type Djinn_Activate, %function
	.thumb_func
Djinn_Activate:
	.incbin "baserom.gba", 0x000b0ca8, 0x00000068
	.section .rom.000b0d10, "ax"
	.global Djinn_Deactivate
	.type Djinn_Deactivate, %function
	.thumb_func
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000b0d10, 0x00000054
	.section .rom.000b0d64, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b0d64, 0x000000ac
	.section .rom.000b100e, "ax"
	.incbin "baserom.gba", 0x000b100e, 0x00001362
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000b2370, 0x0000f1a8
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000c1518, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000c1600, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000c6bbc, 0x00001444
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000c8000, 0x000002b8
	.section .rom.000c83b8, "ax"
	.incbin "baserom.gba", 0x000c83b8, 0x00000130
	.section .rom.000c8518, "ax"
	.incbin "baserom.gba", 0x000c8518, 0x00000018
	.section .rom.000c8598, "ax"
	.incbin "baserom.gba", 0x000c8598, 0x00000030
	.section .rom.000c8628, "ax"
	.incbin "baserom.gba", 0x000c8628, 0x00000010
	.section .rom.000c8658, "ax"
	.incbin "baserom.gba", 0x000c8658, 0x00000050
	.section .rom.000c88d8, "ax"
	.incbin "baserom.gba", 0x000c88d8, 0x00000080
	.section .rom.000c89cc, "ax"
	.incbin "baserom.gba", 0x000c89cc, 0x00000a00
	.section .rom.000c9694, "ax"
	.incbin "baserom.gba", 0x000c9694, 0x0000028c
	.section .rom.000c9934, "ax"
	.incbin "baserom.gba", 0x000c9934, 0x00000494
	.section .rom.000c9dc8, "ax"
	.global Func_080ca18c
	.type Func_080ca18c, %function
	.thumb_func
Func_080ca18c:
	.incbin "baserom.gba", 0x000c9dc8, 0x000003c4
	.section .rom.000ca18c, "ax"
	.global Func_080dc0b8
	.type Func_080dc0b8, %function
	.thumb_func
Func_080dc0b8:
	.incbin "baserom.gba", 0x000ca18c, 0x00000018
	.section .rom.000ca1a4, "ax"
	.global Func_080d295c
	.type Func_080d295c, %function
	.thumb_func
Func_080d295c:
	.incbin "baserom.gba", 0x000ca1a4, 0x00000018
	.section .rom.000ca1bc, "ax"
	.global Func_080ca1a4
	.type Func_080ca1a4, %function
	.thumb_func
Func_080ca1a4:
	.incbin "baserom.gba", 0x000ca1bc, 0x000000c4
	.section .rom.000ca280, "ax"
	.global Func_080d46a4
	.type Func_080d46a4, %function
	.thumb_func
Func_080d46a4:
	.incbin "baserom.gba", 0x000ca280, 0x00000424
	.section .rom.000ca6a4, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000ca6a4, 0x000006e0
	.section .rom.000cad84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000cad84, 0x000001ec
	.section .rom.000cafc4, "ax"
	.incbin "baserom.gba", 0x000cafc4, 0x00000704
	.section .rom.000cb6c8, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000cb6c8, 0x0000002c
	.section .rom.000cb6f4, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000cb6f4, 0x00001654
	.section .rom.000ccd76, "ax"
	.incbin "baserom.gba", 0x000ccd76, 0x00000002
	.section .rom.000ccd78, "ax"
	.global Func_080d25c8
	.type Func_080d25c8, %function
	.thumb_func
Func_080d25c8:
	.incbin "baserom.gba", 0x000ccd78, 0x00000150
	.section .rom.000ccec8, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000ccec8, 0x00000a54
	.section .rom.000cd91c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000cd91c, 0x000002dc
	.section .rom.000cdbf8, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000cdbf8, 0x00000048
	.section .rom.000cdc72, "ax"
	.incbin "baserom.gba", 0x000cdc72, 0x00000002
	.section .rom.000cdc74, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000cdc74, 0x0000010c
	.section .rom.000cdd80, "ax"
	.global Func_080eaf28
	.type Func_080eaf28, %function
	.thumb_func
Func_080eaf28:
	.incbin "baserom.gba", 0x000cdd80, 0x000001dc
	.section .rom.000cdf5c, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000cdf5c, 0x00000054
	.section .rom.000cdfb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000cdfb0, 0x00000ba8
	.section .rom.000ceb58, "ax"
	.global Func_080dca84
	.type Func_080dca84, %function
	.thumb_func
Func_080dca84:
	.incbin "baserom.gba", 0x000ceb58, 0x00000028
	.section .rom.000ceb80, "ax"
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.section .rom.000ceb94, "ax"
	.incbin "baserom.gba", 0x000ceb94, 0x000002ac
	.section .rom.000cee80, "ax"
	.incbin "baserom.gba", 0x000cee80, 0x000002fc
	.section .rom.000cf17c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000cf17c, 0x00000f7c
	.section .rom.000d00f8, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d00f8, 0x000000d4
	.section .rom.000d01cc, "ax"
	.global Func_080d00f8
	.type Func_080d00f8, %function
	.thumb_func
Func_080d00f8:
	.incbin "baserom.gba", 0x000d01cc, 0x00000354
	.section .rom.000d0520, "ax"
	.global Func_080d0bec
	.type Func_080d0bec, %function
	.thumb_func
Func_080d0bec:
	.incbin "baserom.gba", 0x000d0520, 0x00000228
	.section .rom.000d0748, "ax"
	.global Func_080d01cc
	.type Func_080d01cc, %function
	.thumb_func
Func_080d01cc:
	.incbin "baserom.gba", 0x000d0748, 0x000004a4
	.section .rom.000d0bec, "ax"
	.global Func_080d7240
	.type Func_080d7240, %function
	.thumb_func
Func_080d7240:
	.incbin "baserom.gba", 0x000d0bec, 0x00000064
	.section .rom.000d0c50, "ax"
	.global Func_080d170c
	.type Func_080d170c, %function
	.thumb_func
Func_080d170c:
	.incbin "baserom.gba", 0x000d0c50, 0x00000a34
	.section .rom.000d1684, "ax"
	.global Func_080d0748
	.type Func_080d0748, %function
	.thumb_func
Func_080d0748:
	.incbin "baserom.gba", 0x000d1684, 0x00000074
	.section .rom.000d16f8, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000d16f8, 0x00000014
	.section .rom.000d170c, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d170c, 0x00000020
	.section .rom.000d172c, "ax"
	.global Func_080d1684
	.type Func_080d1684, %function
	.thumb_func
Func_080d1684:
	.incbin "baserom.gba", 0x000d172c, 0x00000020
	.section .rom.000d174c, "ax"
	.global Func_080d17ac
	.type Func_080d17ac, %function
	.thumb_func
Func_080d17ac:
	.incbin "baserom.gba", 0x000d174c, 0x00000014
	.section .rom.000d1760, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1760, 0x0000004c
	.section .rom.000d17ac, "ax"
	.global Func_080d172c
	.type Func_080d172c, %function
	.thumb_func
Func_080d172c:
	.incbin "baserom.gba", 0x000d17ac, 0x0000003c
	.section .rom.000d17e8, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d17e8, 0x00000034
	.section .rom.000d183e, "ax"
	.incbin "baserom.gba", 0x000d183e, 0x00000516
	.section .rom.000d1d54, "ax"
	.global Func_080e70f8
	.type Func_080e70f8, %function
	.thumb_func
Func_080e70f8:
	.incbin "baserom.gba", 0x000d1d54, 0x00000058
	.section .rom.000d1dac, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d1dac, 0x00000044
	.section .rom.000d1df0, "ax"
	.global Func_080d1d54
	.type Func_080d1d54, %function
	.thumb_func
Func_080d1d54:
	.incbin "baserom.gba", 0x000d1df0, 0x00000028
	.section .rom.000d1e18, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d1e18, 0x00000094
	.section .rom.000d1eac, "ax"
	.global Func_080d1eac
	.type Func_080d1eac, %function
	.thumb_func
Func_080d1eac:
	.incbin "baserom.gba", 0x000d1eac, 0x0000002c
	.section .rom.000d1ed8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000d1ed8, 0x00000010
	.section .rom.000d1ee8, "ax"
	.global Func_080d4714
	.type Func_080d4714, %function
	.thumb_func
Func_080d4714:
	.incbin "baserom.gba", 0x000d1ee8, 0x00000358
	.section .rom.000d2240, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000d2240, 0x00000020
	.section .rom.000d2260, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000d2260, 0x00000178
	.section .rom.000d25c8, "ax"
	.global Func_080d072c
	.type Func_080d072c, %function
	.thumb_func
Func_080d072c:
	.incbin "baserom.gba", 0x000d25c8, 0x00000218
	.section .rom.000d27fe, "ax"
	.incbin "baserom.gba", 0x000d27fe, 0x00000152
	.section .rom.000d2950, "ax"
	.global Func_080cb6f4
	.type Func_080cb6f4, %function
	.thumb_func
Func_080cb6f4:
	.incbin "baserom.gba", 0x000d2950, 0x000000b0
	.section .rom.000d2a30, "ax"
	.global Func_080d16f8
	.type Func_080d16f8, %function
	.thumb_func
Func_080d16f8:
	.incbin "baserom.gba", 0x000d2a30, 0x00000028
	.section .rom.000d2a58, "ax"
	.global Func_080d174c
	.type Func_080d174c, %function
	.thumb_func
Func_080d174c:
	.incbin "baserom.gba", 0x000d2a58, 0x000001a0
	.section .rom.000d2c58, "ax"
	.incbin "baserom.gba", 0x000d2c58, 0x00000060
	.section .rom.000d2cb8, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000d2cb8, 0x00000044
	.section .rom.000d2cfc, "ax"
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000d2cfc, 0x0000007c
	.section .rom.000d2dd8, "ax"
	.incbin "baserom.gba", 0x000d2dd8, 0x00000038
	.section .rom.000d2f3c, "ax"
	.incbin "baserom.gba", 0x000d2f3c, 0x00000080
	.section .rom.000d3064, "ax"
	.global Func_080d3070
	.type Func_080d3070, %function
	.thumb_func
Func_080d3070:
	.incbin "baserom.gba", 0x000d3064, 0x0000008c
	.section .rom.000d310a, "ax"
	.incbin "baserom.gba", 0x000d310a, 0x0000012a
	.section .rom.000d325c, "ax"
	.incbin "baserom.gba", 0x000d325c, 0x00000334
	.section .rom.000d35f2, "ax"
	.incbin "baserom.gba", 0x000d35f2, 0x00000002
	.section .rom.000d35f4, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000d35f4, 0x000001d4
	.section .rom.000d3900, "ax"
	.incbin "baserom.gba", 0x000d3900, 0x0000001c
	.section .rom.000d391c, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000d391c, 0x00000018
	.section .rom.000d3934, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000d3934, 0x000000d0
	.section .rom.000d3a04, "ax"
	.global Func_080d3928
	.type Func_080d3928, %function
	.thumb_func
Func_080d3928:
	.incbin "baserom.gba", 0x000d3a04, 0x00000118
	.section .rom.000d3b1c, "ax"
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000d3b1c, 0x000000d0
	.section .rom.000d3bec, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000d3bec, 0x00000034
	.section .rom.000d3c20, "ax"
	.global Func_080d3c2c
	.type Func_080d3c2c, %function
	.thumb_func
Func_080d3c2c:
	.incbin "baserom.gba", 0x000d3c20, 0x000005bc
	.section .rom.000d41f0, "ax"
	.incbin "baserom.gba", 0x000d41f0, 0x000004a8
	.section .rom.000d4698, "ax"
	.global Func_080cad9c
	.type Func_080cad9c, %function
	.thumb_func
Func_080cad9c:
	.incbin "baserom.gba", 0x000d4698, 0x00000070
	.section .rom.000d4708, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000d4708, 0x00000278
	.section .rom.000d4a48, "ax"
	.incbin "baserom.gba", 0x000d4a48, 0x00000060
	.section .rom.000d4aa8, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000d4aa8, 0x00000058
	.section .rom.000d4b00, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000d4b00, 0x000001fc
	.section .rom.000d4cfc, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000d4cfc, 0x000003f0
	.section .rom.000d50ec, "ax"
	.global Func_080cdbf8
	.type Func_080cdbf8, %function
	.thumb_func
Func_080cdbf8:
	.incbin "baserom.gba", 0x000d50ec, 0x000002c0
	.section .rom.000d53ac, "ax"
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000d53ac, 0x000009b8
	.section .rom.000d5d64, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000d5d64, 0x00000070
	.section .rom.000d5dd4, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000d5dd4, 0x00000070
	.section .rom.000d5e56, "ax"
	.incbin "baserom.gba", 0x000d5e56, 0x00000aaa
	.section .rom.000d6900, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000d6900, 0x00000284
	.section .rom.000d6b84, "ax"
	.global Func_080d7408
	.type Func_080d7408, %function
	.thumb_func
Func_080d7408:
	.incbin "baserom.gba", 0x000d6b84, 0x000006b0
	.section .rom.000d7234, "ax"
	.global Func_080d7430
	.type Func_080d7430, %function
	.thumb_func
Func_080d7430:
	.incbin "baserom.gba", 0x000d7234, 0x000000c4
	.section .rom.000d72f8, "ax"
	.global Func_080d2260
	.type Func_080d2260, %function
	.thumb_func
Func_080d2260:
	.incbin "baserom.gba", 0x000d72f8, 0x000000b0
	.section .rom.000d73a8, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000d73a8, 0x0000002c
	.section .rom.000d73d4, "ax"
	.global Func_080d7304
	.type Func_080d7304, %function
	.thumb_func
Func_080d7304:
	.incbin "baserom.gba", 0x000d73d4, 0x00000028
	.section .rom.000d73fc, "ax"
	.global Func_080d73b4
	.type Func_080d73b4, %function
	.thumb_func
Func_080d73b4:
	.incbin "baserom.gba", 0x000d73fc, 0x00000028
	.section .rom.000d7424, "ax"
	.global Func_080d73e0
	.type Func_080d73e0, %function
	.thumb_func
Func_080d73e0:
	.incbin "baserom.gba", 0x000d7424, 0x000000c0
	.section .rom.000d7518, "ax"
	.incbin "baserom.gba", 0x000d7518, 0x00000264
	.section .rom.000d777c, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000d777c, 0x000002f0
	.section .rom.000d7a6c, "ax"
	.global BattleFx_InitializeSlots
	.type BattleFx_InitializeSlots, %function
	.thumb_func
BattleFx_InitializeSlots:
	.incbin "baserom.gba", 0x000d7a6c, 0x0000003c
	.section .rom.000d7aa8, "ax"
	.global Func_080d7ab4
	.type Func_080d7ab4, %function
	.thumb_func
Func_080d7ab4:
	.incbin "baserom.gba", 0x000d7aa8, 0x00000c58
	.section .rom.000d8734, "ax"
	.incbin "baserom.gba", 0x000d8734, 0x0000023c
	.section .rom.000d8970, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d8970, 0x000003ec
	.section .rom.000d8d5c, "ax"
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000d8d5c, 0x0000039c
	.section .rom.000d90f8, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000d90f8, 0x000001d0
	.section .rom.000d92c8, "ax"
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000d92c8, 0x0000043c
	.section .rom.000d9704, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000d9704, 0x000003a0
	.section .rom.000d9aa4, "ax"
	.global Func_080d9b08
	.type Func_080d9b08, %function
	.thumb_func
Func_080d9b08:
	.incbin "baserom.gba", 0x000d9aa4, 0x00000020
	.section .rom.000d9ac4, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000d9ac4, 0x00000038
	.section .rom.000d9afc, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000d9afc, 0x00000390
	.section .rom.000d9e8c, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000d9e8c, 0x00000a70
	.section .rom.000da8fc, "ax"
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000da8fc, 0x00000030
	.section .rom.000da92c, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000da92c, 0x00000594
	.section .rom.000daec0, "ax"
	.global Func_080cdf5c
	.type Func_080cdf5c, %function
	.thumb_func
Func_080cdf5c:
	.incbin "baserom.gba", 0x000daec0, 0x000001e4
	.section .rom.000db0a4, "ax"
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000db0a4, 0x00000394
	.section .rom.000db438, "ax"
	.global Func_080dbd48
	.type Func_080dbd48, %function
	.thumb_func
Func_080dbd48:
	.incbin "baserom.gba", 0x000db438, 0x00000048
	.section .rom.000db4aa, "ax"
	.incbin "baserom.gba", 0x000db4aa, 0x00000002
	.section .rom.000db4ac, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000db4ac, 0x000001b8
	.section .rom.000db664, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000db664, 0x000001d8
	.section .rom.000db83c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000db83c, 0x0000003c
	.section .rom.000db878, "ax"
	.global FieldEvent_RunTypeHandler
	.type FieldEvent_RunTypeHandler, %function
	.thumb_func
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000db878, 0x00000098
	.section .rom.000db910, "ax"
	.global Func_080d1df0
	.type Func_080d1df0, %function
	.thumb_func
Func_080d1df0:
	.incbin "baserom.gba", 0x000db910, 0x000000a4
	.section .rom.000db9b4, "ax"
	.global Func_080eb2c8
	.type Func_080eb2c8, %function
	.thumb_func
Func_080eb2c8:
	.incbin "baserom.gba", 0x000db9b4, 0x0000000c
	.section .rom.000db9c0, "ax"
	.global Func_080eb2d0
	.type Func_080eb2d0, %function
	.thumb_func
Func_080eb2d0:
	.incbin "baserom.gba", 0x000db9c0, 0x0000030c
	.section .rom.000dbccc, "ax"
	.global Object_Spawn
	.type Object_Spawn, %function
	.thumb_func
Object_Spawn:
	.incbin "baserom.gba", 0x000dbccc, 0x00000070
	.section .rom.000dbd3c, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000dbd3c, 0x00000080
	.section .rom.000dbdbc, "ax"
	.global Func_080e1420
	.type Func_080e1420, %function
	.thumb_func
Func_080e1420:
	.incbin "baserom.gba", 0x000dbdbc, 0x0000000c
	.section .rom.000dbdc8, "ax"
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000dbdc8, 0x00000014
	.section .rom.000dbddc, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000dbddc, 0x0000000c
	.section .rom.000dbde8, "ax"
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000dbde8, 0x00000014
	.section .rom.000dbdfc, "ax"
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000dbdfc, 0x0000003c
	.section .rom.000dbe38, "ax"
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000dbe38, 0x00000274
	.section .rom.000dc0ac, "ax"
	.global Func_080cb6c8
	.type Func_080cb6c8, %function
	.thumb_func
Func_080cb6c8:
	.incbin "baserom.gba", 0x000dc0ac, 0x00000054
	.section .rom.000dc100, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000dc100, 0x000000a4
	.section .rom.000dc1a4, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000dc1a4, 0x00000094
	.section .rom.000dc238, "ax"
	.global Func_080db9cc
	.type Func_080db9cc, %function
	.thumb_func
Func_080db9cc:
	.incbin "baserom.gba", 0x000dc238, 0x00000734
	.section .rom.000dc96c, "ax"
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000dc96c, 0x000000d8
	.section .rom.000dca44, "ax"
	.global Func_080cded4
	.type Func_080cded4, %function
	.thumb_func
Func_080cded4:
	.incbin "baserom.gba", 0x000dca44, 0x00000034
	.section .rom.000dca78, "ax"
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000dca78, 0x00000058
	.section .rom.000dcad0, "ax"
	.global Func_080dca50
	.type Func_080dca50, %function
	.thumb_func
Func_080dca50:
	.incbin "baserom.gba", 0x000dcad0, 0x00000578
	.section .rom.000dd048, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000dd048, 0x000005e8
	.section .rom.000dd630, "ax"
	.global Event_GetSpecialValue
	.type Event_GetSpecialValue, %function
	.thumb_func
Event_GetSpecialValue:
	.incbin "baserom.gba", 0x000dd630, 0x0000002c
	.section .rom.000dd65c, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000dd65c, 0x00000f10
	.section .rom.000de598, "ax"
	.incbin "baserom.gba", 0x000de598, 0x00000714
	.section .rom.000decac, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000decac, 0x0000101c
	.section .rom.000dfcec, "ax"
	.incbin "baserom.gba", 0x000dfcec, 0x000005f4
	.section .rom.000e02fc, "ax"
	.global Func_080e0308
	.type Func_080e0308, %function
	.thumb_func
Func_080e0308:
	.incbin "baserom.gba", 0x000e02fc, 0x00000054
	.section .rom.000e0350, "ax"
	.global Func_080e035c
	.type Func_080e035c, %function
	.thumb_func
Func_080e035c:
	.incbin "baserom.gba", 0x000e0350, 0x00000050
	.section .rom.000e03b6, "ax"
	.incbin "baserom.gba", 0x000e03b6, 0x00000fba
	.section .rom.000e1370, "ax"
	.global Func_080e15fc
	.type Func_080e15fc, %function
	.thumb_func
Func_080e15fc:
	.incbin "baserom.gba", 0x000e1370, 0x000000a4
	.section .rom.000e1414, "ax"
	.global Func_080e1650
	.type Func_080e1650, %function
	.thumb_func
Func_080e1650:
	.incbin "baserom.gba", 0x000e1414, 0x000001dc
	.section .rom.000e15f0, "ax"
	.global Func_080dc1b0
	.type Func_080dc1b0, %function
	.thumb_func
Func_080dc1b0:
	.incbin "baserom.gba", 0x000e15f0, 0x00000054
	.section .rom.000e1644, "ax"
	.global Field_BeginPaletteTransition
	.type Field_BeginPaletteTransition, %function
	.thumb_func
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000e1644, 0x00000f98
	.section .rom.000e25dc, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000e25dc, 0x0000018c
	.section .rom.000e2768, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.type ObjectGroup_ApplyRandomChildValues, %function
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000e2768, 0x00000f24
	.section .rom.000e368c, "ax"
	.global Func_080da938
	.type Func_080da938, %function
	.thumb_func
Func_080da938:
	.incbin "baserom.gba", 0x000e368c, 0x00000148
	.section .rom.000e37d4, "ax"
	.global Func_080d4ab4
	.type Func_080d4ab4, %function
	.thumb_func
Func_080d4ab4:
	.incbin "baserom.gba", 0x000e37d4, 0x00000a64
	.section .rom.000e4238, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000e4238, 0x00002eb4
	.section .rom.000e70ec, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000e70ec, 0x000036c8
	.section .rom.000ea7b4, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000ea7b4, 0x00000114
	.section .rom.000ea8c8, "ax"
	.global Func_080ed804
	.type Func_080ed804, %function
	.thumb_func
Func_080ed804:
	.incbin "baserom.gba", 0x000ea8c8, 0x00000140
	.section .rom.000eaa08, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000eaa08, 0x0000015c
	.section .rom.000eab64, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000eab64, 0x00000028
	.section .rom.000eab8c, "ax"
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000eab8c, 0x00000038
	.section .rom.000eabc4, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000eabc4, 0x00000118
	.section .rom.000eacdc, "ax"
	.global Func_080eab70
	.type Func_080eab70, %function
	.thumb_func
Func_080eab70:
	.incbin "baserom.gba", 0x000eacdc, 0x00000114
	.section .rom.000eadf0, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000eadf0, 0x000000b8
	.section .rom.000eaea8, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000eaea8, 0x00000074
	.section .rom.000eaf1c, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000eaf1c, 0x00000070
	.section .rom.000eaf8c, "ax"
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000eaf8c, 0x00000084
	.section .rom.000eb010, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000eb010, 0x0000027c
	.section .rom.000eb28c, "ax"
	.global Func_080eaf98
	.type Func_080eaf98, %function
	.thumb_func
Func_080eaf98:
	.incbin "baserom.gba", 0x000eb28c, 0x00000030
	.section .rom.000eb2bc, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000eb2bc, 0x00000008
	.section .rom.000eb2c4, "ax"
	.global Func_080eb298
	.type Func_080eb298, %function
	.thumb_func
Func_080eb298:
	.incbin "baserom.gba", 0x000eb2c4, 0x00000690
	.section .rom.000eb954, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000eb954, 0x000002d0
	.section .rom.000ebc24, "ax"
	.global Func_080ca1bc
	.type Func_080ca1bc, %function
	.thumb_func
Func_080ca1bc:
	.incbin "baserom.gba", 0x000ebc24, 0x00000240
	.section .rom.000ebe64, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000ebe64, 0x00000024
	.section .rom.000ebe9a, "ax"
	.incbin "baserom.gba", 0x000ebe9a, 0x0000195e
	.section .rom.000ed7f8, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000ed7f8, 0x00003650
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0e48, 0x000000d8
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0f20, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000f179c, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f32c4, 0x00000030
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f32f4, 0x00000480
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f3774, 0x0000488c
	.section .rom.000f80c4, "ax"
	.incbin "baserom.gba", 0x000f80c4, 0x00000058
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x000004a0
	.section .rom.000f8656, "ax"
	.incbin "baserom.gba", 0x000f8656, 0x000001ea
	.section .rom.000f8840, "ax"
	.global UiIcon_CreateWithResourceVariant
	.type UiIcon_CreateWithResourceVariant, %function
	.thumb_func
UiIcon_CreateWithResourceVariant:
	.incbin "baserom.gba", 0x000f8840, 0x00000048
	.section .rom.000f8888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8888, 0x00000390
	.section .rom.000f8c94, "ax"
	.incbin "baserom.gba", 0x000f8c94, 0x000002ac
	.section .rom.000f8f9c, "ax"
	.incbin "baserom.gba", 0x000f8f9c, 0x000001d4
	.section .rom.000f9222, "ax"
	.incbin "baserom.gba", 0x000f9222, 0x0000008a
	.section .rom.000f92ac, "ax"
	.global Palette_CopyObjectBankToBackground14
	.type Palette_CopyObjectBankToBackground14, %function
	.thumb_func
Palette_CopyObjectBankToBackground14:
	.incbin "baserom.gba", 0x000f92ac, 0x000000f8
	.section .rom.000f93b6, "ax"
	.incbin "baserom.gba", 0x000f93b6, 0x000000ee
	.section .rom.000f94a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f94a4, 0x00001844
	.section .rom.000facfa, "ax"
	.incbin "baserom.gba", 0x000facfa, 0x000003f2
	.section .rom.000fb14c, "ax"
	.incbin "baserom.gba", 0x000fb14c, 0x000007a8
	.section .rom.000fb8f4, "ax"
	.global ItemMenu_DrawItemDetails
	.type ItemMenu_DrawItemDetails, %function
	.thumb_func
ItemMenu_DrawItemDetails:
	.incbin "baserom.gba", 0x000fb8f4, 0x0000000c
	.section .rom.000fb900, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x000fb900, 0x0000047c
	.section .rom.000fbde4, "ax"
	.incbin "baserom.gba", 0x000fbde4, 0x00000d20
	.section .rom.000fcb04, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000fcb04, 0x0000177c
	.section .rom.000fe280, "ax"
	.global Func_080fe184
	.type Func_080fe184, %function
	.thumb_func
Func_080fe184:
	.incbin "baserom.gba", 0x000fe280, 0x000000f0
	.section .rom.000fe370, "ax"
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x000fe370, 0x000023d0
	.section .rom.0010077c, "ax"
	.incbin "baserom.gba", 0x0010077c, 0x0000081c
	.section .rom.00100f98, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00100f98, 0x00003f24
	.section .rom.00104ebc, "ax"
	.global Menu_UpdateEntryObjectTransforms
	.type Menu_UpdateEntryObjectTransforms, %function
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	.incbin "baserom.gba", 0x00104ebc, 0x00000150
	.section .rom.0010500c, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010500c, 0x000000e8
	.section .rom.001050f4, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x001050f4, 0x000000ac
	.section .rom.001051a0, "ax"
	.global Menu_ReleaseEntryObjects
	.type Menu_ReleaseEntryObjects, %function
	.thumb_func
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x001051a0, 0x0000002c
	.section .rom.001051cc, "ax"
	.global Func_081050b8
	.type Func_081050b8, %function
	.thumb_func
Func_081050b8:
	.incbin "baserom.gba", 0x001051cc, 0x000000f0
	.section .rom.001052bc, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x001052bc, 0x000000c4
	.section .rom.00105380, "ax"
	.global Func_0810526c
	.type Func_0810526c, %function
	.thumb_func
Func_0810526c:
	.incbin "baserom.gba", 0x00105380, 0x00000040
	.section .rom.001053c0, "ax"
	.global Func_081052ac
	.type Func_081052ac, %function
	.thumb_func
Func_081052ac:
	.incbin "baserom.gba", 0x001053c0, 0x00000054
	.section .rom.00105414, "ax"
	.global Func_08105300
	.type Func_08105300, %function
	.thumb_func
Func_08105300:
	.incbin "baserom.gba", 0x00105414, 0x0000002c
	.section .rom.00105440, "ax"
	.global Func_0810532c
	.type Func_0810532c, %function
	.thumb_func
Func_0810532c:
	.incbin "baserom.gba", 0x00105440, 0x00000024
	.section .rom.00105464, "ax"
	.global Func_08105350
	.type Func_08105350, %function
	.thumb_func
Func_08105350:
	.incbin "baserom.gba", 0x00105464, 0x00000230
	.section .rom.0010570a, "ax"
	.incbin "baserom.gba", 0x0010570a, 0x00000376
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00105a80, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00105a84, 0x0000007e
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x00105b02, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x00105b0f, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00105b1c, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00105b34, 0x000024cc
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
	.section .rom.0068a153, "ax"
	.incbin "baserom.gba", 0x0068a153, 0x00000001
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a154, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x0069284c, 0x0000519c
	.section .rom.006a457d, "ax"
	.incbin "baserom.gba", 0x006a457d, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a4580, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a4780, 0x00000808
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a4f88, 0x00000814
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a579c, 0x00000744
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a5ee0, 0x00000548
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a6428, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a6c50, 0x0000186c
	.section .rom.006a99e3, "ax"
	.incbin "baserom.gba", 0x006a99e3, 0x00000001
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006a99e4, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b1000, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006ccf8c, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd240, 0x00000f0c
	.section .rom.006d0b3a, "ax"
	.incbin "baserom.gba", 0x006d0b3a, 0x00000002
	.section .rom.006d4f7e, "ax"
	.incbin "baserom.gba", 0x006d4f7e, 0x00000002
	.section .rom.006e56ee, "ax"
	.incbin "baserom.gba", 0x006e56ee, 0x00000002
	.section .rom.006e8cde, "ax"
	.incbin "baserom.gba", 0x006e8cde, 0x00000002
	.section .rom.006f4782, "ax"
	.incbin "baserom.gba", 0x006f4782, 0x00000002
	.section .rom.00704e32, "ax"
	.incbin "baserom.gba", 0x00704e32, 0x00000002
	.section .rom.0070c8d2, "ax"
	.incbin "baserom.gba", 0x0070c8d2, 0x00000002
	.section .rom.00710112, "ax"
	.incbin "baserom.gba", 0x00710112, 0x00000002
	.section .rom.00719546, "ax"
	.incbin "baserom.gba", 0x00719546, 0x00000002
	.section .rom.007206f2, "ax"
	.incbin "baserom.gba", 0x007206f2, 0x00000002
	.section .rom.007285ae, "ax"
	.incbin "baserom.gba", 0x007285ae, 0x00000002
	.section .rom.0072d03e, "ax"
	.incbin "baserom.gba", 0x0072d03e, 0x00000002
	.section .rom.007312f2, "ax"
	.incbin "baserom.gba", 0x007312f2, 0x00000002
	.section .rom.00734c72, "ax"
	.incbin "baserom.gba", 0x00734c72, 0x00000002
	.section .rom.007414ae, "ax"
	.incbin "baserom.gba", 0x007414ae, 0x00000002
	.section .rom.0074cbee, "ax"
	.incbin "baserom.gba", 0x0074cbee, 0x00000002
	.section .rom.0074ffbe, "ax"
	.incbin "baserom.gba", 0x0074ffbe, 0x00000002
	.section .rom.00761eda, "ax"
	.incbin "baserom.gba", 0x00761eda, 0x00000002
	.section .rom.00765942, "ax"
	.incbin "baserom.gba", 0x00765942, 0x00000002
	.section .rom.00769332, "ax"
	.incbin "baserom.gba", 0x00769332, 0x00000002
	.section .rom.0077989a, "ax"
	.incbin "baserom.gba", 0x0077989a, 0x00000002
	.section .rom.0077d7ca, "ax"
	.incbin "baserom.gba", 0x0077d7ca, 0x00000002
	.section .rom.007811ee, "ax"
	.incbin "baserom.gba", 0x007811ee, 0x00000002
	.section .rom.0078950e, "ax"
	.incbin "baserom.gba", 0x0078950e, 0x00000002
	.section .rom.0079167a, "ax"
	.incbin "baserom.gba", 0x0079167a, 0x00000002
	.section .rom.0079e946, "ax"
	.incbin "baserom.gba", 0x0079e946, 0x00000002
	.section .rom.007a2802, "ax"
	.incbin "baserom.gba", 0x007a2802, 0x00000002
	.section .rom.007a65fe, "ax"
	.incbin "baserom.gba", 0x007a65fe, 0x00000002
	.section .rom.007aaace, "ax"
	.incbin "baserom.gba", 0x007aaace, 0x00000002
	.section .rom.007b24aa, "ax"
	.incbin "baserom.gba", 0x007b24aa, 0x00000002
	.section .rom.007b6ad6, "ax"
	.incbin "baserom.gba", 0x007b6ad6, 0x00000002
	.section .rom.007be752, "ax"
	.incbin "baserom.gba", 0x007be752, 0x00000002
	.section .rom.007c234e, "ax"
	.incbin "baserom.gba", 0x007c234e, 0x00000002
	.section .rom.007c5cfe, "ax"
	.incbin "baserom.gba", 0x007c5cfe, 0x00000002
	.section .rom.007ca14e, "ax"
	.incbin "baserom.gba", 0x007ca14e, 0x00000002
	.section .rom.007cdd46, "ax"
	.incbin "baserom.gba", 0x007cdd46, 0x00000002
	.section .rom.007d99a6, "ax"
	.incbin "baserom.gba", 0x007d99a6, 0x00000002
	.section .rom.007dd5f6, "ax"
	.incbin "baserom.gba", 0x007dd5f6, 0x00000002
	.section .rom.007e5b76, "ax"
	.incbin "baserom.gba", 0x007e5b76, 0x00000002
	.section .rom.007f21c6, "ax"
	.incbin "baserom.gba", 0x007f21c6, 0x00000002
	.section .rom.007f8c7a, "ax"
	.incbin "baserom.gba", 0x007f8c7a, 0x00000002
	.section .rom.00801f42, "ax"
	.incbin "baserom.gba", 0x00801f42, 0x00000002
	.section .rom.00809966, "ax"
	.incbin "baserom.gba", 0x00809966, 0x00000002
	.section .rom.0081f5e6, "ax"
	.incbin "baserom.gba", 0x0081f5e6, 0x00000002
	.section .rom.00828a2a, "ax"
	.incbin "baserom.gba", 0x00828a2a, 0x00000002
	.section .rom.00831c46, "ax"
	.incbin "baserom.gba", 0x00831c46, 0x00000002
	.section .rom.0083f56e, "ax"
	.incbin "baserom.gba", 0x0083f56e, 0x00000002
	.section .rom.008453c2, "ax"
	.incbin "baserom.gba", 0x008453c2, 0x00000002
	.section .rom.0084b106, "ax"
	.incbin "baserom.gba", 0x0084b106, 0x00000002
	.section .rom.0084b889, "ax"
	.incbin "baserom.gba", 0x0084b889, 0x00000003
	.section .rom.0084d1db, "ax"
	.incbin "baserom.gba", 0x0084d1db, 0x00000001
	.section .rom.0084fff9, "ax"
	.incbin "baserom.gba", 0x0084fff9, 0x00000003
	.section .rom.00853f0e, "ax"
	.incbin "baserom.gba", 0x00853f0e, 0x00000002
	.section .rom.00856e14, "ax"
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x00856e14, 0x000009bc
	.section .rom.00857d52, "ax"
	.incbin "baserom.gba", 0x00857d52, 0x00000002
	.section .rom.00858229, "ax"
	.incbin "baserom.gba", 0x00858229, 0x00000003
	.section .rom.00858773, "ax"
	.incbin "baserom.gba", 0x00858773, 0x00000001
	.section .rom.00858c03, "ax"
	.incbin "baserom.gba", 0x00858c03, 0x00000001
	.section .rom.008590ae, "ax"
	.incbin "baserom.gba", 0x008590ae, 0x00000002
	.section .rom.0085941d, "ax"
	.incbin "baserom.gba", 0x0085941d, 0x00000003
	.section .rom.00859fdb, "ax"
	.incbin "baserom.gba", 0x00859fdb, 0x00000001
	.section .rom.0085a09b, "ax"
	.incbin "baserom.gba", 0x0085a09b, 0x00000001
	.section .rom.0085ac2f, "ax"
	.incbin "baserom.gba", 0x0085ac2f, 0x00000001
	.section .rom.0085afc3, "ax"
	.incbin "baserom.gba", 0x0085afc3, 0x00000001
	.section .rom.0085b611, "ax"
	.incbin "baserom.gba", 0x0085b611, 0x00000003
	.section .rom.0085b853, "ax"
	.incbin "baserom.gba", 0x0085b853, 0x00000001
	.section .rom.0085def3, "ax"
	.incbin "baserom.gba", 0x0085def3, 0x00000001
	.section .rom.0085e66e, "ax"
	.incbin "baserom.gba", 0x0085e66e, 0x00000002
	.section .rom.0085fcb3, "ax"
	.incbin "baserom.gba", 0x0085fcb3, 0x00000001
	.section .rom.008601dd, "ax"
	.incbin "baserom.gba", 0x008601dd, 0x00000003
	.section .rom.008609e6, "ax"
	.incbin "baserom.gba", 0x008609e6, 0x00000002
	.section .rom.00860f5f, "ax"
	.incbin "baserom.gba", 0x00860f5f, 0x00000001
	.section .rom.0086269e, "ax"
	.incbin "baserom.gba", 0x0086269e, 0x00000002
	.section .rom.00865567, "ax"
	.incbin "baserom.gba", 0x00865567, 0x00000001
	.section .rom.00865819, "ax"
	.incbin "baserom.gba", 0x00865819, 0x00000003
	.section .rom.00866bfb, "ax"
	.incbin "baserom.gba", 0x00866bfb, 0x00000001
	.section .rom.0086b437, "ax"
	.incbin "baserom.gba", 0x0086b437, 0x00000001
	.section .rom.0086ebdd, "ax"
	.incbin "baserom.gba", 0x0086ebdd, 0x00000003
	.section .rom.00870902, "ax"
	.incbin "baserom.gba", 0x00870902, 0x00000002
	.section .rom.00872801, "ax"
	.incbin "baserom.gba", 0x00872801, 0x00000003
	.section .rom.008741af, "ax"
	.incbin "baserom.gba", 0x008741af, 0x00000001
	.section .rom.00875727, "ax"
	.incbin "baserom.gba", 0x00875727, 0x00000001
	.section .rom.0087935a, "ax"
	.incbin "baserom.gba", 0x0087935a, 0x00000002
	.section .rom.00879eb8, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x00879eb8, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087bb08, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087bf48, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c194, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c32c, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087cb58, 0x00000e98
	.section .rom.0087e027, "ax"
	.incbin "baserom.gba", 0x0087e027, 0x00000001
	.section .rom.00880191, "ax"
	.incbin "baserom.gba", 0x00880191, 0x00000003
	.section .rom.0088029c, "ax"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x0088029c, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x008804e8, 0x00000184
	.section .rom.00880f25, "ax"
	.incbin "baserom.gba", 0x00880f25, 0x00000003
	.section .rom.00882b09, "ax"
	.incbin "baserom.gba", 0x00882b09, 0x00000003
	.section .rom.00884666, "ax"
	.incbin "baserom.gba", 0x00884666, 0x00000002
	.section .rom.00884aa5, "ax"
	.incbin "baserom.gba", 0x00884aa5, 0x00000003
	.section .rom.00884eba, "ax"
	.incbin "baserom.gba", 0x00884eba, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00884ebc, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x008851f4, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x008862c0, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x008865a8, 0x00000154
	.section .rom.00888c07, "ax"
	.incbin "baserom.gba", 0x00888c07, 0x00000001
	.section .rom.008895c3, "ax"
	.incbin "baserom.gba", 0x008895c3, 0x00000001
	.section .rom.0088a706, "ax"
	.incbin "baserom.gba", 0x0088a706, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088a708, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b384, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b3b0, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b3dc, 0x000007c0
	.section .rom.0088d117, "ax"
	.incbin "baserom.gba", 0x0088d117, 0x00000001
	.section .rom.0088d441, "ax"
	.incbin "baserom.gba", 0x0088d441, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d444, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088d96c, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088de9c, 0x000005f0
	.section .rom.0088ec15, "ax"
	.incbin "baserom.gba", 0x0088ec15, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088ec18, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088ee18, 0x00000420
	.section .rom.0088f861, "ax"
	.incbin "baserom.gba", 0x0088f861, 0x00000003
	.section .rom.0088fc6b, "ax"
	.incbin "baserom.gba", 0x0088fc6b, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x0088fc6c, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x0088fec8, 0x0000057c
	.section .rom.008906ed, "ax"
	.incbin "baserom.gba", 0x008906ed, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x008906f0, 0x0000095c
	.section .rom.00891709, "ax"
	.incbin "baserom.gba", 0x00891709, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x0089170c, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x008941bc, 0x000011cc
	.section .rom.008959e7, "ax"
	.incbin "baserom.gba", 0x008959e7, 0x00000001
	.section .rom.00896a21, "ax"
	.incbin "baserom.gba", 0x00896a21, 0x00000003
	.section .rom.00897077, "ax"
	.incbin "baserom.gba", 0x00897077, 0x00000001
	.section .rom.008976f5, "ax"
	.incbin "baserom.gba", 0x008976f5, 0x00000003
	.section .rom.00897d15, "ax"
	.incbin "baserom.gba", 0x00897d15, 0x00000003
	.section .rom.008990e2, "ax"
	.incbin "baserom.gba", 0x008990e2, 0x00000002
	.section .rom.0089a126, "ax"
	.incbin "baserom.gba", 0x0089a126, 0x00000002
	.section .rom.0089abaf, "ax"
	.incbin "baserom.gba", 0x0089abaf, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089abb0, 0x000002cc
	.section .rom.0089bbb5, "ax"
	.incbin "baserom.gba", 0x0089bbb5, 0x00000003
	.section .rom.0089cdf1, "ax"
	.incbin "baserom.gba", 0x0089cdf1, 0x00000003
	.section .rom.0089d73b, "ax"
	.incbin "baserom.gba", 0x0089d73b, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089d73c, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089dd98, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e2c4, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a0580, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a1d14, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a23f8, 0x00001f4c
	.section .rom.008a4878, "ax"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4878, 0x000011d8
	.section .rom.008a5f3e, "ax"
	.incbin "baserom.gba", 0x008a5f3e, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a5f40, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6588, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a71ac, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a7570, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7738, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a7c84, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a7fd0, 0x0000076c
	.section .rom.008a8d7f, "ax"
	.incbin "baserom.gba", 0x008a8d7f, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a8d80, 0x000000a8
	.section .rom.008a913e, "ax"
	.incbin "baserom.gba", 0x008a913e, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a9140, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9ae8, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008a9de0, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aa910, 0x00000100
	.section .rom.008ab681, "ax"
	.incbin "baserom.gba", 0x008ab681, 0x00000003
	.section .rom.008abec9, "ax"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008abecc, 0x00001200
	.section .rom.008adf8b, "ax"
	.incbin "baserom.gba", 0x008adf8b, 0x00000001
	.section .rom.008aea67, "ax"
	.incbin "baserom.gba", 0x008aea67, 0x00000001
	.section .rom.008af12a, "ax"
	.incbin "baserom.gba", 0x008af12a, 0x00000002
	.section .rom.008af411, "ax"
	.incbin "baserom.gba", 0x008af411, 0x00000003
	.section .rom.008b0777, "ax"
	.incbin "baserom.gba", 0x008b0777, 0x00000001
	.section .rom.008b0b61, "ax"
	.incbin "baserom.gba", 0x008b0b61, 0x00000003
	.section .rom.008b0f33, "ax"
	.incbin "baserom.gba", 0x008b0f33, 0x00000001
	.section .rom.008b17d3, "ax"
	.incbin "baserom.gba", 0x008b17d3, 0x00000001
	.section .rom.008b1c76, "ax"
	.incbin "baserom.gba", 0x008b1c76, 0x00000002
	.section .rom.008b2e2f, "ax"
	.incbin "baserom.gba", 0x008b2e2f, 0x00000001
	.section .rom.008b3288, "ax"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3288, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b42f4, 0x00000fc8
	.section .rom.008b5516, "ax"
	.incbin "baserom.gba", 0x008b5516, 0x00000002
	.section .rom.008b6f71, "ax"
	.incbin "baserom.gba", 0x008b6f71, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b6f74, 0x000001b4
	.section .rom.008b74bd, "ax"
	.incbin "baserom.gba", 0x008b74bd, 0x00000003
	.section .rom.008b9246, "ax"
	.incbin "baserom.gba", 0x008b9246, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9248, 0x00000278
	.section .rom.008b9986, "ax"
	.incbin "baserom.gba", 0x008b9986, 0x00000002
	.section .rom.008bb55b, "ax"
	.incbin "baserom.gba", 0x008bb55b, 0x00000001
	.section .rom.008bd159, "ax"
	.incbin "baserom.gba", 0x008bd159, 0x00000003
	.section .rom.008bd37a, "ax"
	.incbin "baserom.gba", 0x008bd37a, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd37c, 0x0000043c
	.section .rom.008bd8c9, "ax"
	.incbin "baserom.gba", 0x008bd8c9, 0x00000003
	.section .rom.008bdeab, "ax"
	.incbin "baserom.gba", 0x008bdeab, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008bdeac, 0x000004a0
	.section .rom.008bf05e, "ax"
	.incbin "baserom.gba", 0x008bf05e, 0x00000002
	.section .rom.008bf5a0, "ax"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bf5a0, 0x00000840
	.section .rom.008c016f, "ax"
	.incbin "baserom.gba", 0x008c016f, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c0170, 0x00001258
	.section .rom.008c1eb6, "ax"
	.incbin "baserom.gba", 0x008c1eb6, 0x00000002
	.section .rom.008c2c8d, "ax"
	.incbin "baserom.gba", 0x008c2c8d, 0x00000003
	.section .rom.008c34ba, "ax"
	.incbin "baserom.gba", 0x008c34ba, 0x00000002
	.section .rom.008c38ac, "ax"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c38ac, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c3a68, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c3c24, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4564, 0x00000418
	.section .rom.008c52ed, "ax"
	.incbin "baserom.gba", 0x008c52ed, 0x00000003
	.section .rom.008c569b, "ax"
	.incbin "baserom.gba", 0x008c569b, 0x00000001
	.section .rom.008c763f, "ax"
	.incbin "baserom.gba", 0x008c763f, 0x00000001
	.section .rom.008c83db, "ax"
	.incbin "baserom.gba", 0x008c83db, 0x00000001
	.section .rom.008c85f7, "ax"
	.incbin "baserom.gba", 0x008c85f7, 0x00000001
	.section .rom.008c88f3, "ax"
	.incbin "baserom.gba", 0x008c88f3, 0x00000001
	.section .rom.008c8c94, "ax"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c8c94, 0x000016b0
	.section .rom.008caaa1, "ax"
	.incbin "baserom.gba", 0x008caaa1, 0x00000003
	.section .rom.008cb86f, "ax"
	.incbin "baserom.gba", 0x008cb86f, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cb870, 0x00000c74
	.section .rom.008ccad9, "ax"
	.incbin "baserom.gba", 0x008ccad9, 0x00000003
	.section .rom.008cd033, "ax"
	.incbin "baserom.gba", 0x008cd033, 0x00000001
	.section .rom.008ce91d, "ax"
	.incbin "baserom.gba", 0x008ce91d, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008ce920, 0x000008a0
	.section .rom.008cf59b, "ax"
	.incbin "baserom.gba", 0x008cf59b, 0x00000001
	.section .rom.008cf825, "ax"
	.incbin "baserom.gba", 0x008cf825, 0x00000003
	.section .rom.008cfbcb, "ax"
	.incbin "baserom.gba", 0x008cfbcb, 0x00000001
	.section .rom.008cfe26, "ax"
	.incbin "baserom.gba", 0x008cfe26, 0x00000002
	.section .rom.008d01de, "ax"
	.incbin "baserom.gba", 0x008d01de, 0x00000002
	.section .rom.008d16c7, "ax"
	.incbin "baserom.gba", 0x008d16c7, 0x00000001
	.section .rom.008d2c8a, "ax"
	.incbin "baserom.gba", 0x008d2c8a, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d2c8c, 0x00000a6c
	.section .rom.008d44ce, "ax"
	.incbin "baserom.gba", 0x008d44ce, 0x00000002
	.section .rom.008d4851, "ax"
	.incbin "baserom.gba", 0x008d4851, 0x00000003
	.section .rom.008d5501, "ax"
	.incbin "baserom.gba", 0x008d5501, 0x00000003
	.section .rom.008d6049, "ax"
	.incbin "baserom.gba", 0x008d6049, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d604c, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d61e4, 0x0000088c
	.section .rom.008d763e, "ax"
	.incbin "baserom.gba", 0x008d763e, 0x00000002
	.section .rom.008d7b56, "ax"
	.incbin "baserom.gba", 0x008d7b56, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d7b58, 0x00000624
	.section .rom.008d859d, "ax"
	.incbin "baserom.gba", 0x008d859d, 0x00000003
	.section .rom.008d8837, "ax"
	.incbin "baserom.gba", 0x008d8837, 0x00000001
	.section .rom.008d9d38, "ax"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008d9d38, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da1d4, 0x0000198c
	.section .rom.008dbf4a, "ax"
	.incbin "baserom.gba", 0x008dbf4a, 0x00000002
	.section .rom.008ddebf, "ax"
	.incbin "baserom.gba", 0x008ddebf, 0x00000001
	.section .rom.008de38f, "ax"
	.incbin "baserom.gba", 0x008de38f, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de390, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008dea24, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df458, 0x00000bfc
	.section .rom.008e0829, "ax"
	.incbin "baserom.gba", 0x008e0829, 0x00000003
	.section .rom.008e12fa, "ax"
	.incbin "baserom.gba", 0x008e12fa, 0x00000002
	.section .rom.008e1e37, "ax"
	.incbin "baserom.gba", 0x008e1e37, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e1e38, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2478, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3a00, 0x00000064
	.section .rom.008e3dcf, "ax"
	.incbin "baserom.gba", 0x008e3dcf, 0x00000001
	.section .rom.008e436d, "ax"
	.incbin "baserom.gba", 0x008e436d, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e4370, 0x00000204
	.section .rom.008e48ad, "ax"
	.incbin "baserom.gba", 0x008e48ad, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e48b0, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e58c8, 0x0000166c
	.section .rom.008e8ed2, "ax"
	.incbin "baserom.gba", 0x008e8ed2, 0x00000002
	.section .rom.008e9675, "ax"
	.incbin "baserom.gba", 0x008e9675, 0x00000003
	.section .rom.008ea68c, "ax"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008ea68c, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eaa08, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eae38, 0x000010cc
	.section .rom.008ebf88, "ax"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ebf88, 0x000004e8
	.section .rom.008ec578, "ax"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ec578, 0x000006b8
	.section .rom.008ed542, "ax"
	.incbin "baserom.gba", 0x008ed542, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008ed544, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef078, 0x00001050
	.section .rom.008f116d, "ax"
	.incbin "baserom.gba", 0x008f116d, 0x00000003
	.section .rom.008f136c, "ax"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f136c, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00932bd4, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093bf98, 0x00000028
	.section .rom.0093c1ad, "ax"
	.incbin "baserom.gba", 0x0093c1ad, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c1b0, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c304, 0x000004b8
	.section .rom.0093df26, "ax"
	.incbin "baserom.gba", 0x0093df26, 0x00000002
	.section .rom.0093f45d, "ax"
	.incbin "baserom.gba", 0x0093f45d, 0x00000003
	.section .rom.009412b3, "ax"
	.incbin "baserom.gba", 0x009412b3, 0x00000001
	.section .rom.009433c7, "ax"
	.incbin "baserom.gba", 0x009433c7, 0x00000001
	.section .rom.009435a0, "ax"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x009435a0, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943784, 0x000003c0
	.section .rom.009461bf, "ax"
	.incbin "baserom.gba", 0x009461bf, 0x00000001
	.section .rom.00946bc7, "ax"
	.incbin "baserom.gba", 0x00946bc7, 0x00000001
	.section .rom.009498e6, "ax"
	.incbin "baserom.gba", 0x009498e6, 0x00000002
	.section .rom.00949ab8, "ax"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00949ab8, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00949ac0, 0x00000460
	.section .rom.0094b512, "ax"
	.incbin "baserom.gba", 0x0094b512, 0x00000002
	.section .rom.0094c982, "ax"
	.incbin "baserom.gba", 0x0094c982, 0x00000002
	.section .rom.0094d276, "ax"
	.incbin "baserom.gba", 0x0094d276, 0x00000002
	.section .rom.0094db9d, "ax"
	.incbin "baserom.gba", 0x0094db9d, 0x00000003
	.section .rom.0094ea8e, "ax"
	.incbin "baserom.gba", 0x0094ea8e, 0x00000002
	.section .rom.0094f67b, "ax"
	.incbin "baserom.gba", 0x0094f67b, 0x00000001
	.section .rom.0094f856, "ax"
	.incbin "baserom.gba", 0x0094f856, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094f858, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094fa48, 0x000002d8
	.section .rom.009505b6, "ax"
	.incbin "baserom.gba", 0x009505b6, 0x00000002
	.section .rom.009529f1, "ax"
	.incbin "baserom.gba", 0x009529f1, 0x00000003
	.section .rom.00952fc3, "ax"
	.incbin "baserom.gba", 0x00952fc3, 0x00000001
	.section .rom.00953463, "ax"
	.incbin "baserom.gba", 0x00953463, 0x00000001
	.section .rom.009537b6, "ax"
	.incbin "baserom.gba", 0x009537b6, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x009537b8, 0x000002f8
	.section .rom.00953b89, "ax"
	.incbin "baserom.gba", 0x00953b89, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00953b8c, 0x000002f0
	.section .rom.00953f0e, "ax"
	.incbin "baserom.gba", 0x00953f0e, 0x00000002
	.section .rom.00954ac1, "ax"
	.incbin "baserom.gba", 0x00954ac1, 0x00000003
	.section .rom.0095576e, "ax"
	.incbin "baserom.gba", 0x0095576e, 0x00000002
	.section .rom.00956272, "ax"
	.incbin "baserom.gba", 0x00956272, 0x00000002
	.section .rom.009571b1, "ax"
	.incbin "baserom.gba", 0x009571b1, 0x00000003
	.section .rom.009579ed, "ax"
	.incbin "baserom.gba", 0x009579ed, 0x00000003
	.section .rom.00958e92, "ax"
	.incbin "baserom.gba", 0x00958e92, 0x00000002
	.section .rom.0095994a, "ax"
	.incbin "baserom.gba", 0x0095994a, 0x00000002
	.section .rom.00959c75, "ax"
	.incbin "baserom.gba", 0x00959c75, 0x00000003
	.section .rom.0095b0f0, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b0f0, 0x00000400
	.section .rom.009678aa, "ax"
	.incbin "baserom.gba", 0x009678aa, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x009678ac, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x009679ac, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x00967e74, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x009680dc, 0x000001c8
	.section .rom.0096870b, "ax"
	.incbin "baserom.gba", 0x0096870b, 0x00000001
	.section .rom.00968917, "ax"
	.incbin "baserom.gba", 0x00968917, 0x00000001
	.section .rom.00968b1b, "ax"
	.incbin "baserom.gba", 0x00968b1b, 0x00000001
	.section .rom.00968baa, "ax"
	.incbin "baserom.gba", 0x00968baa, 0x00000002
	.section .rom.0096908c, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x0096908c, 0x00000070
	.section .rom.00969157, "ax"
	.incbin "baserom.gba", 0x00969157, 0x00000001
	.section .rom.0096918a, "ax"
	.incbin "baserom.gba", 0x0096918a, 0x00000002
	.section .rom.00969356, "ax"
	.incbin "baserom.gba", 0x00969356, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969358, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x009695a4, 0x000000e4
	.section .rom.00969741, "ax"
	.incbin "baserom.gba", 0x00969741, 0x00000003
	.section .rom.00969916, "ax"
	.incbin "baserom.gba", 0x00969916, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x00969918, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x009699b0, 0x00000024
	.section .rom.00969b26, "ax"
	.incbin "baserom.gba", 0x00969b26, 0x00000002
	.section .rom.00969c09, "ax"
	.incbin "baserom.gba", 0x00969c09, 0x00000003
	.section .rom.00969cd9, "ax"
	.incbin "baserom.gba", 0x00969cd9, 0x00000003
	.section .rom.00969dff, "ax"
	.incbin "baserom.gba", 0x00969dff, 0x00000001
	.section .rom.00969e2e, "ax"
	.incbin "baserom.gba", 0x00969e2e, 0x00000002
	.section .rom.0096a092, "ax"
	.incbin "baserom.gba", 0x0096a092, 0x00000002
	.section .rom.0096a12f, "ax"
	.incbin "baserom.gba", 0x0096a12f, 0x00000001
	.section .rom.0096a194, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a194, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096a594, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096a644, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096aba4, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096ac18, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096ac60, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096acb0, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096ad00, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096ad58, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096adb0, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096adf0, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096ae34, 0x00000054
	.section .rom.009700bf, "ax"
	.incbin "baserom.gba", 0x009700bf, 0x00000001
	.section .rom.0097300a, "ax"
	.incbin "baserom.gba", 0x0097300a, 0x00000002
	.section .rom.00976c2d, "ax"
	.incbin "baserom.gba", 0x00976c2d, 0x00000003
	.section .rom.00978e79, "ax"
	.incbin "baserom.gba", 0x00978e79, 0x00000003
	.section .rom.0097e415, "ax"
	.incbin "baserom.gba", 0x0097e415, 0x00000003
	.section .rom.00981c8f, "ax"
	.incbin "baserom.gba", 0x00981c8f, 0x00000001
	.section .rom.00987af9, "ax"
	.incbin "baserom.gba", 0x00987af9, 0x00000003
	.section .rom.00988bcd, "ax"
	.incbin "baserom.gba", 0x00988bcd, 0x00000003
	.section .rom.00989c39, "ax"
	.incbin "baserom.gba", 0x00989c39, 0x00000003
	.section .rom.0098f293, "ax"
	.incbin "baserom.gba", 0x0098f293, 0x00000001
	.section .rom.009923d5, "ax"
	.incbin "baserom.gba", 0x009923d5, 0x00000003
	.section .rom.00994475, "ax"
	.incbin "baserom.gba", 0x00994475, 0x00000003
	.section .rom.00996795, "ax"
	.incbin "baserom.gba", 0x00996795, 0x00000003
	.section .rom.0099940e, "ax"
	.incbin "baserom.gba", 0x0099940e, 0x00000002
	.section .rom.0099b309, "ax"
	.incbin "baserom.gba", 0x0099b309, 0x00000003
	.section .rom.009a384d, "ax"
	.incbin "baserom.gba", 0x009a384d, 0x00000003
	.section .rom.009ac4cf, "ax"
	.incbin "baserom.gba", 0x009ac4cf, 0x00000001
	.section .rom.009af049, "ax"
	.incbin "baserom.gba", 0x009af049, 0x00000003
	.section .rom.009b5a86, "ax"
	.incbin "baserom.gba", 0x009b5a86, 0x00000002
	.section .rom.009b857a, "ax"
	.incbin "baserom.gba", 0x009b857a, 0x00000002
	.section .rom.009ba06a, "ax"
	.incbin "baserom.gba", 0x009ba06a, 0x00000002
	.section .rom.009bc980, "ax"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bc980, 0x00003440
	.section .rom.009c4766, "ax"
	.incbin "baserom.gba", 0x009c4766, 0x00000002
	.section .rom.009c5702, "ax"
	.incbin "baserom.gba", 0x009c5702, 0x00000002
	.section .rom.009c7e2d, "ax"
	.incbin "baserom.gba", 0x009c7e2d, 0x00000003
	.section .rom.009c9542, "ax"
	.incbin "baserom.gba", 0x009c9542, 0x00000002
	.section .rom.009d1d5b, "ax"
	.incbin "baserom.gba", 0x009d1d5b, 0x00000001
	.section .rom.009d6023, "ax"
	.incbin "baserom.gba", 0x009d6023, 0x00000001
	.section .rom.009dd85d, "ax"
	.incbin "baserom.gba", 0x009dd85d, 0x00000003
	.section .rom.009df042, "ax"
	.incbin "baserom.gba", 0x009df042, 0x00000002
	.section .rom.009e1dd9, "ax"
	.incbin "baserom.gba", 0x009e1dd9, 0x00000003
	.section .rom.009e2e23, "ax"
	.incbin "baserom.gba", 0x009e2e23, 0x00000001
	.section .rom.009eb3b3, "ax"
	.incbin "baserom.gba", 0x009eb3b3, 0x00000001
	.section .rom.009ed1ad, "ax"
	.incbin "baserom.gba", 0x009ed1ad, 0x00000003
	.section .rom.009ee573, "ax"
	.incbin "baserom.gba", 0x009ee573, 0x00000001
	.section .rom.009f5475, "ax"
	.incbin "baserom.gba", 0x009f5475, 0x00000003
	.section .rom.009f7776, "ax"
	.incbin "baserom.gba", 0x009f7776, 0x00000002
	.section .rom.009fc9c3, "ax"
	.incbin "baserom.gba", 0x009fc9c3, 0x00000001
	.section .rom.00a01dc2, "ax"
	.incbin "baserom.gba", 0x00a01dc2, 0x00000002
	.section .rom.00a03f76, "ax"
	.incbin "baserom.gba", 0x00a03f76, 0x00000002
	.section .rom.00a080dd, "ax"
	.incbin "baserom.gba", 0x00a080dd, 0x00000003
	.section .rom.00a0b302, "ax"
	.incbin "baserom.gba", 0x00a0b302, 0x00000002
	.section .rom.00a0ca06, "ax"
	.incbin "baserom.gba", 0x00a0ca06, 0x00000002
	.section .rom.00a135ee, "ax"
	.incbin "baserom.gba", 0x00a135ee, 0x00000002
	.section .rom.00a1fbce, "ax"
	.incbin "baserom.gba", 0x00a1fbce, 0x00000002
	.section .rom.00a22c62, "ax"
	.incbin "baserom.gba", 0x00a22c62, 0x00000002
	.section .rom.00a254cd, "ax"
	.incbin "baserom.gba", 0x00a254cd, 0x00000003
	.section .rom.00a26fbe, "ax"
	.incbin "baserom.gba", 0x00a26fbe, 0x00000002
	.section .rom.00a27dd6, "ax"
	.incbin "baserom.gba", 0x00a27dd6, 0x00000002
	.section .rom.00a28ac1, "ax"
	.incbin "baserom.gba", 0x00a28ac1, 0x00000003
	.section .rom.00a31f7e, "ax"
	.incbin "baserom.gba", 0x00a31f7e, 0x00000002
	.section .rom.00a32c89, "ax"
	.incbin "baserom.gba", 0x00a32c89, 0x00000003
	.section .rom.00a33eed, "ax"
	.incbin "baserom.gba", 0x00a33eed, 0x00000003
	.section .rom.00a34e1f, "ax"
	.incbin "baserom.gba", 0x00a34e1f, 0x00000001
	.section .rom.00a35a8f, "ax"
	.incbin "baserom.gba", 0x00a35a8f, 0x00000001
	.section .rom.00a366a7, "ax"
	.incbin "baserom.gba", 0x00a366a7, 0x00000001
	.section .rom.00a36e1b, "ax"
	.incbin "baserom.gba", 0x00a36e1b, 0x00000001
	.section .rom.00a386ab, "ax"
	.incbin "baserom.gba", 0x00a386ab, 0x00000001
	.section .rom.00a39079, "ax"
	.incbin "baserom.gba", 0x00a39079, 0x00000003
	.section .rom.00a39c97, "ax"
	.incbin "baserom.gba", 0x00a39c97, 0x00000001
	.section .rom.00a3c34b, "ax"
	.incbin "baserom.gba", 0x00a3c34b, 0x00000001
	.section .rom.00a3ea5e, "ax"
	.incbin "baserom.gba", 0x00a3ea5e, 0x00000002
	.section .rom.00a439b3, "ax"
	.incbin "baserom.gba", 0x00a439b3, 0x00000001
	.section .rom.00a47cdd, "ax"
	.incbin "baserom.gba", 0x00a47cdd, 0x00000003
	.section .rom.00a488ca, "ax"
	.incbin "baserom.gba", 0x00a488ca, 0x00000002
	.section .rom.00a4d47f, "ax"
	.incbin "baserom.gba", 0x00a4d47f, 0x00000001
	.section .rom.00a4f7e9, "ax"
	.incbin "baserom.gba", 0x00a4f7e9, 0x00000003
	.section .rom.00a5118b, "ax"
	.incbin "baserom.gba", 0x00a5118b, 0x00000001
	.section .rom.00a55a4d, "ax"
	.incbin "baserom.gba", 0x00a55a4d, 0x00000003
	.section .rom.00a58e5d, "ax"
	.incbin "baserom.gba", 0x00a58e5d, 0x00000003
	.section .rom.00a5cf25, "ax"
	.incbin "baserom.gba", 0x00a5cf25, 0x00000003
	.section .rom.00a605ee, "ax"
	.incbin "baserom.gba", 0x00a605ee, 0x00000002
	.section .rom.00a685bf, "ax"
	.incbin "baserom.gba", 0x00a685bf, 0x00000001
	.section .rom.00a6f8cf, "ax"
	.incbin "baserom.gba", 0x00a6f8cf, 0x00000001
	.section .rom.00a7484f, "ax"
	.incbin "baserom.gba", 0x00a7484f, 0x00000001
	.section .rom.00a792d1, "ax"
	.incbin "baserom.gba", 0x00a792d1, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a792d4, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a792e0, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79430, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79570, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a796b0, 0x00000140
	.section .rom.00a7aa4d, "ax"
	.incbin "baserom.gba", 0x00a7aa4d, 0x00000003
	.section .rom.00a7ac1e, "ax"
	.incbin "baserom.gba", 0x00a7ac1e, 0x00000002
	.section .rom.00a7ccbf, "ax"
	.incbin "baserom.gba", 0x00a7ccbf, 0x00000001
	.section .rom.00a7dc4c, "ax"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7dc4c, 0x000022d8
	.section .rom.00a81178, "ax"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a81178, 0x0000461c
	.section .rom.00a8589a, "ax"
	.incbin "baserom.gba", 0x00a8589a, 0x00000002
	.section .rom.00a86a85, "ax"
	.incbin "baserom.gba", 0x00a86a85, 0x00000003
	.section .rom.00a8870d, "ax"
	.incbin "baserom.gba", 0x00a8870d, 0x00000003
	.section .rom.00a88c1b, "ax"
	.incbin "baserom.gba", 0x00a88c1b, 0x00000001
	.section .rom.00a88d5b, "ax"
	.incbin "baserom.gba", 0x00a88d5b, 0x00000001
	.section .rom.00a8a9de, "ax"
	.incbin "baserom.gba", 0x00a8a9de, 0x00000002
	.section .rom.00a8d3b6, "ax"
	.incbin "baserom.gba", 0x00a8d3b6, 0x00000002
	.section .rom.00a8fb7f, "ax"
	.incbin "baserom.gba", 0x00a8fb7f, 0x00000001
	.section .rom.00a9109f, "ax"
	.incbin "baserom.gba", 0x00a9109f, 0x00000001
	.section .rom.00a929e3, "ax"
	.incbin "baserom.gba", 0x00a929e3, 0x00000001
	.section .rom.00a94491, "ax"
	.incbin "baserom.gba", 0x00a94491, 0x00000003
	.section .rom.00a94605, "ax"
	.incbin "baserom.gba", 0x00a94605, 0x00000003
	.section .rom.00a973e9, "ax"
	.incbin "baserom.gba", 0x00a973e9, 0x00000003
	.section .rom.00a99c93, "ax"
	.incbin "baserom.gba", 0x00a99c93, 0x00000001
	.section .rom.00a9e12a, "ax"
	.incbin "baserom.gba", 0x00a9e12a, 0x00000002
	.section .rom.00aa14a2, "ax"
	.incbin "baserom.gba", 0x00aa14a2, 0x00000002
	.section .rom.00aa3f9f, "ax"
	.incbin "baserom.gba", 0x00aa3f9f, 0x00000001
	.section .rom.00aa607d, "ax"
	.incbin "baserom.gba", 0x00aa607d, 0x00000003
	.section .rom.00aa82c9, "ax"
	.incbin "baserom.gba", 0x00aa82c9, 0x00000003
	.section .rom.00aa840b, "ax"
	.incbin "baserom.gba", 0x00aa840b, 0x00000001
	.section .rom.00aa9ade, "ax"
	.incbin "baserom.gba", 0x00aa9ade, 0x00000002
	.section .rom.00aa9bde, "ax"
	.incbin "baserom.gba", 0x00aa9bde, 0x00000002
	.section .rom.00aabad1, "ax"
	.incbin "baserom.gba", 0x00aabad1, 0x00000003
	.section .rom.00aad4a9, "ax"
	.incbin "baserom.gba", 0x00aad4a9, 0x00000003
	.section .rom.00aafeda, "ax"
	.incbin "baserom.gba", 0x00aafeda, 0x00000002
	.section .rom.00ab512b, "ax"
	.incbin "baserom.gba", 0x00ab512b, 0x00000001
	.section .rom.00ab7a6f, "ax"
	.incbin "baserom.gba", 0x00ab7a6f, 0x00000001
	.section .rom.00ab8f42, "ax"
	.incbin "baserom.gba", 0x00ab8f42, 0x00000002
	.section .rom.00abdd0d, "ax"
	.incbin "baserom.gba", 0x00abdd0d, 0x00000003
	.section .rom.00ac09b7, "ax"
	.incbin "baserom.gba", 0x00ac09b7, 0x00000001
	.section .rom.00ac3bef, "ax"
	.incbin "baserom.gba", 0x00ac3bef, 0x00000001
	.section .rom.00ac3d87, "ax"
	.incbin "baserom.gba", 0x00ac3d87, 0x00000001
	.section .rom.00ac6b87, "ax"
	.incbin "baserom.gba", 0x00ac6b87, 0x00000001
	.section .rom.00ac833b, "ax"
	.incbin "baserom.gba", 0x00ac833b, 0x00000001
	.section .rom.00ac92d6, "ax"
	.incbin "baserom.gba", 0x00ac92d6, 0x00000002
	.section .rom.00acb937, "ax"
	.incbin "baserom.gba", 0x00acb937, 0x00000001
	.section .rom.00acbade, "ax"
	.incbin "baserom.gba", 0x00acbade, 0x00000002
	.section .rom.00acea97, "ax"
	.incbin "baserom.gba", 0x00acea97, 0x00000001
	.section .rom.00acf73d, "ax"
	.incbin "baserom.gba", 0x00acf73d, 0x00000003
	.section .rom.00ad0d0d, "ax"
	.incbin "baserom.gba", 0x00ad0d0d, 0x00000003
	.section .rom.00ad8cd9, "ax"
	.incbin "baserom.gba", 0x00ad8cd9, 0x00000003
	.section .rom.00ada7bf, "ax"
	.incbin "baserom.gba", 0x00ada7bf, 0x00000001
	.section .rom.00adbca9, "ax"
	.incbin "baserom.gba", 0x00adbca9, 0x00000003
	.section .rom.00ae1ed3, "ax"
	.incbin "baserom.gba", 0x00ae1ed3, 0x00000001
	.section .rom.00ae23a5, "ax"
	.incbin "baserom.gba", 0x00ae23a5, 0x00000003
	.section .rom.00ae34ed, "ax"
	.incbin "baserom.gba", 0x00ae34ed, 0x00000003
	.section .rom.00ae369e, "ax"
	.incbin "baserom.gba", 0x00ae369e, 0x00000002
	.section .rom.00aef38b, "ax"
	.incbin "baserom.gba", 0x00aef38b, 0x00000001
	.section .rom.00aef4ab, "ax"
	.incbin "baserom.gba", 0x00aef4ab, 0x00000001
	.section .rom.00af184b, "ax"
	.incbin "baserom.gba", 0x00af184b, 0x00000001
	.section .rom.00af2a97, "ax"
	.incbin "baserom.gba", 0x00af2a97, 0x00000001
	.section .rom.00af4e27, "ax"
	.incbin "baserom.gba", 0x00af4e27, 0x00000001
	.section .rom.00af6725, "ax"
	.incbin "baserom.gba", 0x00af6725, 0x00000003
	.section .rom.00af7716, "ax"
	.incbin "baserom.gba", 0x00af7716, 0x00000002
	.section .rom.00af9cb2, "ax"
	.incbin "baserom.gba", 0x00af9cb2, 0x00000002
	.section .rom.00afe2da, "ax"
	.incbin "baserom.gba", 0x00afe2da, 0x00000002
	.section .rom.00afe99f, "ax"
	.incbin "baserom.gba", 0x00afe99f, 0x00000001
	.section .rom.00b01435, "ax"
	.incbin "baserom.gba", 0x00b01435, 0x00000003
	.section .rom.00b01586, "ax"
	.incbin "baserom.gba", 0x00b01586, 0x00000002
	.section .rom.00b03996, "ax"
	.incbin "baserom.gba", 0x00b03996, 0x00000002
	.section .rom.00b05b17, "ax"
	.incbin "baserom.gba", 0x00b05b17, 0x00000001
	.section .rom.00b05c57, "ax"
	.incbin "baserom.gba", 0x00b05c57, 0x00000001
	.section .rom.00b086f6, "ax"
	.incbin "baserom.gba", 0x00b086f6, 0x00000002
	.section .rom.00b08847, "ax"
	.incbin "baserom.gba", 0x00b08847, 0x00000001
	.section .rom.00b0ac56, "ax"
	.incbin "baserom.gba", 0x00b0ac56, 0x00000002
	.section .rom.00b0cdd7, "ax"
	.incbin "baserom.gba", 0x00b0cdd7, 0x00000001
	.section .rom.00b0cf17, "ax"
	.incbin "baserom.gba", 0x00b0cf17, 0x00000001
	.section .rom.00b10546, "ax"
	.incbin "baserom.gba", 0x00b10546, 0x00000002
	.section .rom.00b12aaa, "ax"
	.incbin "baserom.gba", 0x00b12aaa, 0x00000002
	.section .rom.00b14c2b, "ax"
	.incbin "baserom.gba", 0x00b14c2b, 0x00000001
	.section .rom.00b14d6b, "ax"
	.incbin "baserom.gba", 0x00b14d6b, 0x00000001
	.section .rom.00b1621b, "ax"
	.incbin "baserom.gba", 0x00b1621b, 0x00000001
	.section .rom.00b16326, "ax"
	.incbin "baserom.gba", 0x00b16326, 0x00000002
	.section .rom.00b17e7e, "ax"
	.incbin "baserom.gba", 0x00b17e7e, 0x00000002
	.section .rom.00b19521, "ax"
	.incbin "baserom.gba", 0x00b19521, 0x00000003
	.section .rom.00b196e2, "ax"
	.incbin "baserom.gba", 0x00b196e2, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b196e4, 0x00000d9c
	.section .rom.00b1c0d6, "ax"
	.incbin "baserom.gba", 0x00b1c0d6, 0x00000002
	.section .rom.00b1d92d, "ax"
	.incbin "baserom.gba", 0x00b1d92d, 0x00000003
	.section .rom.00b1daee, "ax"
	.incbin "baserom.gba", 0x00b1daee, 0x00000002
	.section .rom.00b21d69, "ax"
	.incbin "baserom.gba", 0x00b21d69, 0x00000003
	.section .rom.00b21f2a, "ax"
	.incbin "baserom.gba", 0x00b21f2a, 0x00000002
	.section .rom.00b229a1, "ax"
	.incbin "baserom.gba", 0x00b229a1, 0x00000003
	.section .rom.00b22aa9, "ax"
	.incbin "baserom.gba", 0x00b22aa9, 0x00000003
	.section .rom.00b2476a, "ax"
	.incbin "baserom.gba", 0x00b2476a, 0x00000002
	.section .rom.00b25ef7, "ax"
	.incbin "baserom.gba", 0x00b25ef7, 0x00000001
	.section .rom.00b261ad, "ax"
	.incbin "baserom.gba", 0x00b261ad, 0x00000003
	.section .rom.00b27cbd, "ax"
	.incbin "baserom.gba", 0x00b27cbd, 0x00000003
	.section .rom.00b2a54a, "ax"
	.incbin "baserom.gba", 0x00b2a54a, 0x00000002
	.section .rom.00b2cd3a, "ax"
	.incbin "baserom.gba", 0x00b2cd3a, 0x00000002
	.section .rom.00b2dcda, "ax"
	.incbin "baserom.gba", 0x00b2dcda, 0x00000002
	.section .rom.00b2fe2d, "ax"
	.incbin "baserom.gba", 0x00b2fe2d, 0x00000003
	.section .rom.00b334ba, "ax"
	.incbin "baserom.gba", 0x00b334ba, 0x00000002
	.section .rom.00b3524f, "ax"
	.incbin "baserom.gba", 0x00b3524f, 0x00000001
	.section .rom.00b3706d, "ax"
	.incbin "baserom.gba", 0x00b3706d, 0x00000003
	.section .rom.00b38b49, "ax"
	.incbin "baserom.gba", 0x00b38b49, 0x00000003
	.section .rom.00b3a132, "ax"
	.incbin "baserom.gba", 0x00b3a132, 0x00000002
	.section .rom.00b3c71a, "ax"
	.incbin "baserom.gba", 0x00b3c71a, 0x00000002
	.section .rom.00b3c893, "ax"
	.incbin "baserom.gba", 0x00b3c893, 0x00000001
	.section .rom.00b3dd52, "ax"
	.incbin "baserom.gba", 0x00b3dd52, 0x00000002
	.section .rom.00b3f556, "ax"
	.incbin "baserom.gba", 0x00b3f556, 0x00000002
	.section .rom.00b40ece, "ax"
	.incbin "baserom.gba", 0x00b40ece, 0x00000002
	.section .rom.00b41dee, "ax"
	.incbin "baserom.gba", 0x00b41dee, 0x00000002
	.section .rom.00b4389a, "ax"
	.incbin "baserom.gba", 0x00b4389a, 0x00000002
	.section .rom.00b439a7, "ax"
	.incbin "baserom.gba", 0x00b439a7, 0x00000001
	.section .rom.00b45ad7, "ax"
	.incbin "baserom.gba", 0x00b45ad7, 0x00000001
	.section .rom.00b476ad, "ax"
	.incbin "baserom.gba", 0x00b476ad, 0x00000003
	.section .rom.00b47cbf, "ax"
	.incbin "baserom.gba", 0x00b47cbf, 0x00000001
	.section .rom.00b47dff, "ax"
	.incbin "baserom.gba", 0x00b47dff, 0x00000001
	.section .rom.00b4a4ff, "ax"
	.incbin "baserom.gba", 0x00b4a4ff, 0x00000001
	.section .rom.00b4bd5e, "ax"
	.incbin "baserom.gba", 0x00b4bd5e, 0x00000002
	.section .rom.00b4c36f, "ax"
	.incbin "baserom.gba", 0x00b4c36f, 0x00000001
	.section .rom.00b4c4af, "ax"
	.incbin "baserom.gba", 0x00b4c4af, 0x00000001
	.section .rom.00b4d32a, "ax"
	.incbin "baserom.gba", 0x00b4d32a, 0x00000002
	.section .rom.00b4d441, "ax"
	.incbin "baserom.gba", 0x00b4d441, 0x00000003
	.section .rom.00b4f717, "ax"
	.incbin "baserom.gba", 0x00b4f717, 0x00000001
	.section .rom.00b5136a, "ax"
	.incbin "baserom.gba", 0x00b5136a, 0x00000002
	.section .rom.00b519af, "ax"
	.incbin "baserom.gba", 0x00b519af, 0x00000001
	.section .rom.00b51aef, "ax"
	.incbin "baserom.gba", 0x00b51aef, 0x00000001
	.section .rom.00b526f5, "ax"
	.incbin "baserom.gba", 0x00b526f5, 0x00000003
	.section .rom.00b54ca7, "ax"
	.incbin "baserom.gba", 0x00b54ca7, 0x00000001
	.section .rom.00b56966, "ax"
	.incbin "baserom.gba", 0x00b56966, 0x00000002
	.section .rom.00b58432, "ax"
	.incbin "baserom.gba", 0x00b58432, 0x00000002
	.section .rom.00b59506, "ax"
	.incbin "baserom.gba", 0x00b59506, 0x00000002
	.section .rom.00b5966d, "ax"
	.incbin "baserom.gba", 0x00b5966d, 0x00000003
	.section .rom.00b5a886, "ax"
	.incbin "baserom.gba", 0x00b5a886, 0x00000002
	.section .rom.00b5b465, "ax"
	.incbin "baserom.gba", 0x00b5b465, 0x00000003
	.section .rom.00b5b5d2, "ax"
	.incbin "baserom.gba", 0x00b5b5d2, 0x00000002
	.section .rom.00b5e423, "ax"
	.incbin "baserom.gba", 0x00b5e423, 0x00000001
	.section .rom.00b60be2, "ax"
	.incbin "baserom.gba", 0x00b60be2, 0x00000002
	.section .rom.00b66c93, "ax"
	.incbin "baserom.gba", 0x00b66c93, 0x00000001
	.section .rom.00b687c3, "ax"
	.incbin "baserom.gba", 0x00b687c3, 0x00000001
	.section .rom.00b6aafa, "ax"
	.incbin "baserom.gba", 0x00b6aafa, 0x00000002
	.section .rom.00b6bcab, "ax"
	.incbin "baserom.gba", 0x00b6bcab, 0x00000001
	.section .rom.00b6d5cd, "ax"
	.incbin "baserom.gba", 0x00b6d5cd, 0x00000003
	.section .rom.00b6f459, "ax"
	.incbin "baserom.gba", 0x00b6f459, 0x00000003
	.section .rom.00b7163a, "ax"
	.incbin "baserom.gba", 0x00b7163a, 0x00000002
	.section .rom.00b75001, "ax"
	.incbin "baserom.gba", 0x00b75001, 0x00000003
	.section .rom.00b750ed, "ax"
	.incbin "baserom.gba", 0x00b750ed, 0x00000003
	.section .rom.00b78326, "ax"
	.incbin "baserom.gba", 0x00b78326, 0x00000002
	.section .rom.00b7899d, "ax"
	.incbin "baserom.gba", 0x00b7899d, 0x00000003
	.section .rom.00b7c75f, "ax"
	.incbin "baserom.gba", 0x00b7c75f, 0x00000001
	.section .rom.00b7e9f1, "ax"
	.incbin "baserom.gba", 0x00b7e9f1, 0x00000003
	.section .rom.00b8040e, "ax"
	.incbin "baserom.gba", 0x00b8040e, 0x00000002
	.section .rom.00b8151b, "ax"
	.incbin "baserom.gba", 0x00b8151b, 0x00000001
	.section .rom.00b846ba, "ax"
	.incbin "baserom.gba", 0x00b846ba, 0x00000002
	.section .rom.00b858b2, "ax"
	.incbin "baserom.gba", 0x00b858b2, 0x00000002
	.section .rom.00b86775, "ax"
	.incbin "baserom.gba", 0x00b86775, 0x00000003
	.section .rom.00b88816, "ax"
	.incbin "baserom.gba", 0x00b88816, 0x00000002
	.section .rom.00b8a26b, "ax"
	.incbin "baserom.gba", 0x00b8a26b, 0x00000001
	.section .rom.00b8b736, "ax"
	.incbin "baserom.gba", 0x00b8b736, 0x00000002
	.section .rom.00b8d93a, "ax"
	.incbin "baserom.gba", 0x00b8d93a, 0x00000002
	.section .rom.00b8da2f, "ax"
	.incbin "baserom.gba", 0x00b8da2f, 0x00000001
	.section .rom.00b8f002, "ax"
	.incbin "baserom.gba", 0x00b8f002, 0x00000002
	.section .rom.00b8f221, "ax"
	.incbin "baserom.gba", 0x00b8f221, 0x00000003
	.section .rom.00b91305, "ax"
	.incbin "baserom.gba", 0x00b91305, 0x00000003
	.section .rom.00b914ca, "ax"
	.incbin "baserom.gba", 0x00b914ca, 0x00000002
	.section .rom.00b9358d, "ax"
	.incbin "baserom.gba", 0x00b9358d, 0x00000003
	.section .rom.00b936bd, "ax"
	.incbin "baserom.gba", 0x00b936bd, 0x00000003
	.section .rom.00b96176, "ax"
	.incbin "baserom.gba", 0x00b96176, 0x00000002
	.section .rom.00b97cce, "ax"
	.incbin "baserom.gba", 0x00b97cce, 0x00000002
	.section .rom.00b98c15, "ax"
	.incbin "baserom.gba", 0x00b98c15, 0x00000003
	.section .rom.00b9a61e, "ax"
	.incbin "baserom.gba", 0x00b9a61e, 0x00000002
	.section .rom.00baa373, "ax"
	.incbin "baserom.gba", 0x00baa373, 0x00000001
	.section .rom.00baa4c3, "ax"
	.incbin "baserom.gba", 0x00baa4c3, 0x00000001
	.section .rom.00bacfce, "ax"
	.incbin "baserom.gba", 0x00bacfce, 0x00000002
	.section .rom.00baebd7, "ax"
	.incbin "baserom.gba", 0x00baebd7, 0x00000001
	.section .rom.00bb05de, "ax"
	.incbin "baserom.gba", 0x00bb05de, 0x00000002
	.section .rom.00bb22f9, "ax"
	.incbin "baserom.gba", 0x00bb22f9, 0x00000003
	.section .rom.00bb244b, "ax"
	.incbin "baserom.gba", 0x00bb244b, 0x00000001
	.section .rom.00bb3e52, "ax"
	.incbin "baserom.gba", 0x00bb3e52, 0x00000002
	.section .rom.00bb7bda, "ax"
	.incbin "baserom.gba", 0x00bb7bda, 0x00000002
	.section .rom.00bb7d6d, "ax"
	.incbin "baserom.gba", 0x00bb7d6d, 0x00000003
	.section .rom.00bbcd77, "ax"
	.incbin "baserom.gba", 0x00bbcd77, 0x00000001
	.section .rom.00bbf3f9, "ax"
	.incbin "baserom.gba", 0x00bbf3f9, 0x00000003
	.section .rom.00bbf72f, "ax"
	.incbin "baserom.gba", 0x00bbf72f, 0x00000001
	.section .rom.00bc5b85, "ax"
	.incbin "baserom.gba", 0x00bc5b85, 0x00000003
	.section .rom.00bc5cd2, "ax"
	.incbin "baserom.gba", 0x00bc5cd2, 0x00000002
	.section .rom.00bc7b57, "ax"
	.incbin "baserom.gba", 0x00bc7b57, 0x00000001
	.section .rom.00bc90fe, "ax"
	.incbin "baserom.gba", 0x00bc90fe, 0x00000002
	.section .rom.00bcb0a9, "ax"
	.incbin "baserom.gba", 0x00bcb0a9, 0x00000003
	.section .rom.00bcc01a, "ax"
	.incbin "baserom.gba", 0x00bcc01a, 0x00000002
	.section .rom.00bd7623, "ax"
	.incbin "baserom.gba", 0x00bd7623, 0x00000001
	.section .rom.00bd76df, "ax"
	.incbin "baserom.gba", 0x00bd76df, 0x00000001
	.section .rom.00bd841e, "ax"
	.incbin "baserom.gba", 0x00bd841e, 0x00000002
	.section .rom.00bd8dfb, "ax"
	.incbin "baserom.gba", 0x00bd8dfb, 0x00000001
	.section .rom.00bd9a13, "ax"
	.incbin "baserom.gba", 0x00bd9a13, 0x00000001
	.section .rom.00bdb8ce, "ax"
	.incbin "baserom.gba", 0x00bdb8ce, 0x00000002
	.section .rom.00bdb9ea, "ax"
	.incbin "baserom.gba", 0x00bdb9ea, 0x00000002
	.section .rom.00bde26b, "ax"
	.incbin "baserom.gba", 0x00bde26b, 0x00000001
	.section .rom.00bdf9f6, "ax"
	.incbin "baserom.gba", 0x00bdf9f6, 0x00000002
	.section .rom.00be3926, "ax"
	.incbin "baserom.gba", 0x00be3926, 0x00000002
	.section .rom.00be3a6f, "ax"
	.incbin "baserom.gba", 0x00be3a6f, 0x00000001
	.section .rom.00be611f, "ax"
	.incbin "baserom.gba", 0x00be611f, 0x00000001
	.section .rom.00be88c2, "ax"
	.incbin "baserom.gba", 0x00be88c2, 0x00000002
	.section .rom.00be964d, "ax"
	.incbin "baserom.gba", 0x00be964d, 0x00000003
	.section .rom.00bec48e, "ax"
	.incbin "baserom.gba", 0x00bec48e, 0x00000002
	.section .rom.00beeab5, "ax"
	.incbin "baserom.gba", 0x00beeab5, 0x00000003
	.section .rom.00bf07a1, "ax"
	.incbin "baserom.gba", 0x00bf07a1, 0x00000003
	.section .rom.00bf1ea1, "ax"
	.incbin "baserom.gba", 0x00bf1ea1, 0x00000003
	.section .rom.00bf341d, "ax"
	.incbin "baserom.gba", 0x00bf341d, 0x00000003
	.section .rom.00bf4b8f, "ax"
	.incbin "baserom.gba", 0x00bf4b8f, 0x00000001
	.section .rom.00bf781a, "ax"
	.incbin "baserom.gba", 0x00bf781a, 0x00000002
	.section .rom.00bf8866, "ax"
	.incbin "baserom.gba", 0x00bf8866, 0x00000002
	.section .rom.00bf9963, "ax"
	.incbin "baserom.gba", 0x00bf9963, 0x00000001
	.section .rom.00bfb137, "ax"
	.incbin "baserom.gba", 0x00bfb137, 0x00000001
	.section .rom.00bfc5fb, "ax"
	.incbin "baserom.gba", 0x00bfc5fb, 0x00000001
	.section .rom.00bfeb2e, "ax"
	.incbin "baserom.gba", 0x00bfeb2e, 0x00000002
	.section .rom.00bfec62, "ax"
	.incbin "baserom.gba", 0x00bfec62, 0x00000002
	.section .rom.00c013ce, "ax"
	.incbin "baserom.gba", 0x00c013ce, 0x00000002
	.section .rom.00c031a1, "ax"
	.incbin "baserom.gba", 0x00c031a1, 0x00000003
	.section .rom.00c06a85, "ax"
	.incbin "baserom.gba", 0x00c06a85, 0x00000003
	.section .rom.00c091eb, "ax"
	.incbin "baserom.gba", 0x00c091eb, 0x00000001
	.section .rom.00c0ab55, "ax"
	.incbin "baserom.gba", 0x00c0ab55, 0x00000003
	.section .rom.00c0d37e, "ax"
	.incbin "baserom.gba", 0x00c0d37e, 0x00000002
	.section .rom.00c1175d, "ax"
	.incbin "baserom.gba", 0x00c1175d, 0x00000003
	.section .rom.00c14b07, "ax"
	.incbin "baserom.gba", 0x00c14b07, 0x00000001
	.section .rom.00c158fa, "ax"
	.incbin "baserom.gba", 0x00c158fa, 0x00000002
	.section .rom.00c16672, "ax"
	.incbin "baserom.gba", 0x00c16672, 0x00000002
	.section .rom.00c1e79a, "ax"
	.incbin "baserom.gba", 0x00c1e79a, 0x00000002
	.section .rom.00c24b19, "ax"
	.incbin "baserom.gba", 0x00c24b19, 0x00000003
	.section .rom.00c2551f, "ax"
	.incbin "baserom.gba", 0x00c2551f, 0x00000001
	.section .rom.00c26fcd, "ax"
	.incbin "baserom.gba", 0x00c26fcd, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c26fd0, 0x00001490
	.section .rom.00c288c7, "ax"
	.incbin "baserom.gba", 0x00c288c7, 0x00000001
	.section .rom.00c28a86, "ax"
	.incbin "baserom.gba", 0x00c28a86, 0x00000002
	.section .rom.00c2b28e, "ax"
	.incbin "baserom.gba", 0x00c2b28e, 0x00000002
	.section .rom.00c2b3e6, "ax"
	.incbin "baserom.gba", 0x00c2b3e6, 0x00000002
	.section .rom.00c2dd2d, "ax"
	.incbin "baserom.gba", 0x00c2dd2d, 0x00000003
	.section .rom.00c2ff51, "ax"
	.incbin "baserom.gba", 0x00c2ff51, 0x00000003
	.section .rom.00c31446, "ax"
	.incbin "baserom.gba", 0x00c31446, 0x00000002
	.section .rom.00c336a9, "ax"
	.incbin "baserom.gba", 0x00c336a9, 0x00000003
	.section .rom.00c35f57, "ax"
	.incbin "baserom.gba", 0x00c35f57, 0x00000001
	.section .rom.00c37eea, "ax"
	.incbin "baserom.gba", 0x00c37eea, 0x00000002
	.section .rom.00c38f0b, "ax"
	.incbin "baserom.gba", 0x00c38f0b, 0x00000001
	.section .rom.00c3904b, "ax"
	.incbin "baserom.gba", 0x00c3904b, 0x00000001
	.section .rom.00c3bc6a, "ax"
	.incbin "baserom.gba", 0x00c3bc6a, 0x00000002
	.section .rom.00c3e0ee, "ax"
	.incbin "baserom.gba", 0x00c3e0ee, 0x00000002
	.section .rom.00c401d5, "ax"
	.incbin "baserom.gba", 0x00c401d5, 0x00000003
	.section .rom.00c41a39, "ax"
	.incbin "baserom.gba", 0x00c41a39, 0x00000003
	.section .rom.00c43f9f, "ax"
	.incbin "baserom.gba", 0x00c43f9f, 0x00000001
	.section .rom.00c44105, "ax"
	.incbin "baserom.gba", 0x00c44105, 0x00000003
	.section .rom.00c468ff, "ax"
	.incbin "baserom.gba", 0x00c468ff, 0x00000001
	.section .rom.00c48933, "ax"
	.incbin "baserom.gba", 0x00c48933, 0x00000001
	.section .rom.00c4a55a, "ax"
	.incbin "baserom.gba", 0x00c4a55a, 0x00000002
	.section .rom.00c4cf7a, "ax"
	.incbin "baserom.gba", 0x00c4cf7a, 0x00000002
	.section .rom.00c4dae5, "ax"
	.incbin "baserom.gba", 0x00c4dae5, 0x00000003
	.section .rom.00c4dbcb, "ax"
	.incbin "baserom.gba", 0x00c4dbcb, 0x00000001
	.section .rom.00c4f515, "ax"
	.incbin "baserom.gba", 0x00c4f515, 0x00000003
	.section .rom.00c50e7b, "ax"
	.incbin "baserom.gba", 0x00c50e7b, 0x00000001
	.section .rom.00c52249, "ax"
	.incbin "baserom.gba", 0x00c52249, 0x00000003
	.section .rom.00c5232f, "ax"
	.incbin "baserom.gba", 0x00c5232f, 0x00000001
	.section .rom.00c52e9d, "ax"
	.incbin "baserom.gba", 0x00c52e9d, 0x00000003
	.section .rom.00c52f83, "ax"
	.incbin "baserom.gba", 0x00c52f83, 0x00000001
	.section .rom.00c53ce7, "ax"
	.incbin "baserom.gba", 0x00c53ce7, 0x00000001
	.section .rom.00c53dcb, "ax"
	.incbin "baserom.gba", 0x00c53dcb, 0x00000001
	.section .rom.00c545d1, "ax"
	.incbin "baserom.gba", 0x00c545d1, 0x00000003
	.section .rom.00c546b7, "ax"
	.incbin "baserom.gba", 0x00c546b7, 0x00000001
	.section .rom.00c55217, "ax"
	.incbin "baserom.gba", 0x00c55217, 0x00000001
	.section .rom.00c55a95, "ax"
	.incbin "baserom.gba", 0x00c55a95, 0x00000003
	.section .rom.00c55b92, "ax"
	.incbin "baserom.gba", 0x00c55b92, 0x00000002
	.section .rom.00c579eb, "ax"
	.incbin "baserom.gba", 0x00c579eb, 0x00000001
	.section .rom.00c587f9, "ax"
	.incbin "baserom.gba", 0x00c587f9, 0x00000003
	.section .rom.00c59a8d, "ax"
	.incbin "baserom.gba", 0x00c59a8d, 0x00000003
	.section .rom.00c5aa69, "ax"
	.incbin "baserom.gba", 0x00c5aa69, 0x00000003
	.section .rom.00c5ce6f, "ax"
	.incbin "baserom.gba", 0x00c5ce6f, 0x00000001
	.section .rom.00c5cfaf, "ax"
	.incbin "baserom.gba", 0x00c5cfaf, 0x00000001
	.section .rom.00c5d67d, "ax"
	.incbin "baserom.gba", 0x00c5d67d, 0x00000003
	.section .rom.00c5d753, "ax"
	.incbin "baserom.gba", 0x00c5d753, 0x00000001
	.section .rom.00c5dfc9, "ax"
	.incbin "baserom.gba", 0x00c5dfc9, 0x00000003
	.section .rom.00c5e47a, "ax"
	.incbin "baserom.gba", 0x00c5e47a, 0x00000002
	.section .rom.00c5f8f5, "ax"
	.incbin "baserom.gba", 0x00c5f8f5, 0x00000003
	.section .rom.00c6010b, "ax"
	.incbin "baserom.gba", 0x00c6010b, 0x00000001
	.section .rom.00c60da1, "ax"
	.incbin "baserom.gba", 0x00c60da1, 0x00000003
	.section .rom.00c60f46, "ax"
	.incbin "baserom.gba", 0x00c60f46, 0x00000002
	.section .rom.00c63e0a, "ax"
	.incbin "baserom.gba", 0x00c63e0a, 0x00000002
	.section .rom.00c65789, "ax"
	.incbin "baserom.gba", 0x00c65789, 0x00000003
	.section .rom.00c667d6, "ax"
	.incbin "baserom.gba", 0x00c667d6, 0x00000002
	.section .rom.00c6df1b, "ax"
	.incbin "baserom.gba", 0x00c6df1b, 0x00000001
	.section .rom.00c7570f, "ax"
	.incbin "baserom.gba", 0x00c7570f, 0x00000001
	.section .rom.00c7bd09, "ax"
	.incbin "baserom.gba", 0x00c7bd09, 0x00000003
	.section .rom.00c7e742, "ax"
	.incbin "baserom.gba", 0x00c7e742, 0x00000002
	.section .rom.00c81013, "ax"
	.incbin "baserom.gba", 0x00c81013, 0x00000001
	.section .rom.00c8585f, "ax"
	.incbin "baserom.gba", 0x00c8585f, 0x00000001
	.section .rom.00c8685d, "ax"
	.incbin "baserom.gba", 0x00c8685d, 0x00000003
	.section .rom.00c86976, "ax"
	.incbin "baserom.gba", 0x00c86976, 0x00000002
	.section .rom.00c87f1e, "ax"
	.incbin "baserom.gba", 0x00c87f1e, 0x00000002
	.section .rom.00c8997b, "ax"
	.incbin "baserom.gba", 0x00c8997b, 0x00000001
	.section .rom.00c89ab6, "ax"
	.incbin "baserom.gba", 0x00c89ab6, 0x00000002
	.section .rom.00c8afbd, "ax"
	.incbin "baserom.gba", 0x00c8afbd, 0x00000003
	.section .rom.00c8e246, "ax"
	.incbin "baserom.gba", 0x00c8e246, 0x00000002
	.section .rom.00c8f68d, "ax"
	.incbin "baserom.gba", 0x00c8f68d, 0x00000003
	.section .rom.00c92385, "ax"
	.incbin "baserom.gba", 0x00c92385, 0x00000003
	.section .rom.00c947bb, "ax"
	.incbin "baserom.gba", 0x00c947bb, 0x00000001
	.section .rom.00c94997, "ax"
	.incbin "baserom.gba", 0x00c94997, 0x00000001
	.section .rom.00c97aab, "ax"
	.incbin "baserom.gba", 0x00c97aab, 0x00000001
	.section .rom.00c98f2e, "ax"
	.incbin "baserom.gba", 0x00c98f2e, 0x00000002
	.section .rom.00c99f7d, "ax"
	.incbin "baserom.gba", 0x00c99f7d, 0x00000003
	.section .rom.00c9bde2, "ax"
	.incbin "baserom.gba", 0x00c9bde2, 0x00000002
	.section .rom.00c9bf7d, "ax"
	.incbin "baserom.gba", 0x00c9bf7d, 0x00000003
	.section .rom.00c9c0db, "ax"
	.incbin "baserom.gba", 0x00c9c0db, 0x00000001
	.section .rom.00c9d05a, "ax"
	.incbin "baserom.gba", 0x00c9d05a, 0x00000002
	.section .rom.00c9d172, "ax"
	.incbin "baserom.gba", 0x00c9d172, 0x00000002
	.section .rom.00c9f205, "ax"
	.incbin "baserom.gba", 0x00c9f205, 0x00000003
	.section .rom.00ca1087, "ax"
	.incbin "baserom.gba", 0x00ca1087, 0x00000001
	.section .rom.00ca35ad, "ax"
	.incbin "baserom.gba", 0x00ca35ad, 0x00000003
	.section .rom.00ca36ba, "ax"
	.incbin "baserom.gba", 0x00ca36ba, 0x00000002
	.section .rom.00ca574d, "ax"
	.incbin "baserom.gba", 0x00ca574d, 0x00000003
	.section .rom.00ca8c23, "ax"
	.incbin "baserom.gba", 0x00ca8c23, 0x00000001
	.section .rom.00ca970d, "ax"
	.incbin "baserom.gba", 0x00ca970d, 0x00000003
	.section .rom.00ca987d, "ax"
	.incbin "baserom.gba", 0x00ca987d, 0x00000003
	.section .rom.00cac797, "ax"
	.incbin "baserom.gba", 0x00cac797, 0x00000001
	.section .rom.00cae722, "ax"
	.incbin "baserom.gba", 0x00cae722, 0x00000002
	.section .rom.00cb008d, "ax"
	.incbin "baserom.gba", 0x00cb008d, 0x00000003
	.section .rom.00cb5b7a, "ax"
	.incbin "baserom.gba", 0x00cb5b7a, 0x00000002
	.section .rom.00cb5d2a, "ax"
	.incbin "baserom.gba", 0x00cb5d2a, 0x00000002
	.section .rom.00cba6a2, "ax"
	.incbin "baserom.gba", 0x00cba6a2, 0x00000002
	.section .rom.00cba7fa, "ax"
	.incbin "baserom.gba", 0x00cba7fa, 0x00000002
	.section .rom.00cbddc7, "ax"
	.incbin "baserom.gba", 0x00cbddc7, 0x00000001
	.section .rom.00cbf649, "ax"
	.incbin "baserom.gba", 0x00cbf649, 0x00000003
	.section .rom.00cc1371, "ax"
	.incbin "baserom.gba", 0x00cc1371, 0x00000003
	.section .rom.00cc4a92, "ax"
	.incbin "baserom.gba", 0x00cc4a92, 0x00000002
	.section .rom.00cc9c1b, "ax"
	.incbin "baserom.gba", 0x00cc9c1b, 0x00000001
	.section .rom.00ccc1e9, "ax"
	.incbin "baserom.gba", 0x00ccc1e9, 0x00000003
	.section .rom.00ccdb75, "ax"
	.incbin "baserom.gba", 0x00ccdb75, 0x00000003
	.section .rom.00cceca2, "ax"
	.incbin "baserom.gba", 0x00cceca2, 0x00000002
	.section .rom.00ccf852, "ax"
	.incbin "baserom.gba", 0x00ccf852, 0x00000002
	.section .rom.00cd121a, "ax"
	.incbin "baserom.gba", 0x00cd121a, 0x00000002
	.section .rom.00cd12fe, "ax"
	.incbin "baserom.gba", 0x00cd12fe, 0x00000002
	.section .rom.00cd375b, "ax"
	.incbin "baserom.gba", 0x00cd375b, 0x00000001
	.section .rom.00cd389b, "ax"
	.incbin "baserom.gba", 0x00cd389b, 0x00000001
	.section .rom.00cd6f11, "ax"
	.incbin "baserom.gba", 0x00cd6f11, 0x00000003
	.section .rom.00cd7075, "ax"
	.incbin "baserom.gba", 0x00cd7075, 0x00000003
	.section .rom.00cd9755, "ax"
	.incbin "baserom.gba", 0x00cd9755, 0x00000003
	.section .rom.00cdc605, "ax"
	.incbin "baserom.gba", 0x00cdc605, 0x00000003
	.section .rom.00cddb7a, "ax"
	.incbin "baserom.gba", 0x00cddb7a, 0x00000002
	.section .rom.00ce1c76, "ax"
	.incbin "baserom.gba", 0x00ce1c76, 0x00000002
	.section .rom.00ce3efb, "ax"
	.incbin "baserom.gba", 0x00ce3efb, 0x00000001
	.section .rom.00ce5adf, "ax"
	.incbin "baserom.gba", 0x00ce5adf, 0x00000001
	.section .rom.00ce77d5, "ax"
	.incbin "baserom.gba", 0x00ce77d5, 0x00000003
	.section .rom.00cf4df9, "ax"
	.incbin "baserom.gba", 0x00cf4df9, 0x00000003
	.section .rom.00cf4f21, "ax"
	.incbin "baserom.gba", 0x00cf4f21, 0x00000003
	.section .rom.00cf7132, "ax"
	.incbin "baserom.gba", 0x00cf7132, 0x00000002
	.section .rom.00cf72c3, "ax"
	.incbin "baserom.gba", 0x00cf72c3, 0x00000001
	.section .rom.00cfe775, "ax"
	.incbin "baserom.gba", 0x00cfe775, 0x00000003
	.section .rom.00d00f4b, "ax"
	.incbin "baserom.gba", 0x00d00f4b, 0x00000001
	.section .rom.00d036a6, "ax"
	.incbin "baserom.gba", 0x00d036a6, 0x00000002
	.section .rom.00d03859, "ax"
	.incbin "baserom.gba", 0x00d03859, 0x00000003
	.section .rom.00d04f5d, "ax"
	.incbin "baserom.gba", 0x00d04f5d, 0x00000003
	.section .rom.00d06fb7, "ax"
	.incbin "baserom.gba", 0x00d06fb7, 0x00000001
	.section .rom.00d08301, "ax"
	.incbin "baserom.gba", 0x00d08301, 0x00000003
	.section .rom.00d09d4b, "ax"
	.incbin "baserom.gba", 0x00d09d4b, 0x00000001
	.section .rom.00d09e6f, "ax"
	.incbin "baserom.gba", 0x00d09e6f, 0x00000001
	.section .rom.00d0a341, "ax"
	.incbin "baserom.gba", 0x00d0a341, 0x00000003
	.section .rom.00d0c525, "ax"
	.incbin "baserom.gba", 0x00d0c525, 0x00000003
	.section .rom.00d0c9f9, "ax"
	.incbin "baserom.gba", 0x00d0c9f9, 0x00000003
	.section .rom.00d0ef2f, "ax"
	.incbin "baserom.gba", 0x00d0ef2f, 0x00000001
	.section .rom.00d0f0b7, "ax"
	.incbin "baserom.gba", 0x00d0f0b7, 0x00000001
	.section .rom.00d13959, "ax"
	.incbin "baserom.gba", 0x00d13959, 0x00000003
	.section .rom.00d13a8f, "ax"
	.incbin "baserom.gba", 0x00d13a8f, 0x00000001
	.section .rom.00d156fb, "ax"
	.incbin "baserom.gba", 0x00d156fb, 0x00000001
	.section .rom.00d168b3, "ax"
	.incbin "baserom.gba", 0x00d168b3, 0x00000001
	.section .rom.00d17601, "ax"
	.incbin "baserom.gba", 0x00d17601, 0x00000003
	.section .rom.00d1878a, "ax"
	.incbin "baserom.gba", 0x00d1878a, 0x00000002
	.section .rom.00d1a283, "ax"
	.incbin "baserom.gba", 0x00d1a283, 0x00000001
	.section .rom.00d1b68f, "ax"
	.incbin "baserom.gba", 0x00d1b68f, 0x00000001
	.section .rom.00d1bbe3, "ax"
	.incbin "baserom.gba", 0x00d1bbe3, 0x00000001
	.section .rom.00d1c21d, "ax"
	.incbin "baserom.gba", 0x00d1c21d, 0x00000003
	.section .rom.00d2156d, "ax"
	.incbin "baserom.gba", 0x00d2156d, 0x00000003
	.section .rom.00d23bce, "ax"
	.incbin "baserom.gba", 0x00d23bce, 0x00000002
	.section .rom.00d23d63, "ax"
	.incbin "baserom.gba", 0x00d23d63, 0x00000001
	.section .rom.00d25917, "ax"
	.incbin "baserom.gba", 0x00d25917, 0x00000001
	.section .rom.00d25a77, "ax"
	.incbin "baserom.gba", 0x00d25a77, 0x00000001
	.section .rom.00d2851b, "ax"
	.incbin "baserom.gba", 0x00d2851b, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d2851c, 0x000021b0
	.section .rom.00d2c542, "ax"
	.incbin "baserom.gba", 0x00d2c542, 0x00000002
	.section .rom.00d2c683, "ax"
	.incbin "baserom.gba", 0x00d2c683, 0x00000001
	.section .rom.00d335fb, "ax"
	.incbin "baserom.gba", 0x00d335fb, 0x00000001
	.section .rom.00d33739, "ax"
	.incbin "baserom.gba", 0x00d33739, 0x00000003
	.section .rom.00d3570d, "ax"
	.incbin "baserom.gba", 0x00d3570d, 0x00000003
	.section .rom.00d35801, "ax"
	.incbin "baserom.gba", 0x00d35801, 0x00000003
	.section .rom.00d368e1, "ax"
	.incbin "baserom.gba", 0x00d368e1, 0x00000003
	.section .rom.00d387bb, "ax"
	.incbin "baserom.gba", 0x00d387bb, 0x00000001
	.section .rom.00d39773, "ax"
	.incbin "baserom.gba", 0x00d39773, 0x00000001
	.section .rom.00d3b6ba, "ax"
	.incbin "baserom.gba", 0x00d3b6ba, 0x00000002
	.section .rom.00d3d2c5, "ax"
	.incbin "baserom.gba", 0x00d3d2c5, 0x00000003
	.section .rom.00d3d411, "ax"
	.incbin "baserom.gba", 0x00d3d411, 0x00000003
	.section .rom.00d3f4cf, "ax"
	.incbin "baserom.gba", 0x00d3f4cf, 0x00000001
	.section .rom.00d435be, "ax"
	.incbin "baserom.gba", 0x00d435be, 0x00000002
	.section .rom.00d47370, "ax"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d47370, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d4927c, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4b5e0, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4d60c, 0x0000180c
	.section .rom.00d50565, "ax"
	.incbin "baserom.gba", 0x00d50565, 0x00000003
	.section .rom.00d50692, "ax"
	.incbin "baserom.gba", 0x00d50692, 0x00000002
	.section .rom.00d57605, "ax"
	.incbin "baserom.gba", 0x00d57605, 0x00000003
	.section .rom.00d5770a, "ax"
	.incbin "baserom.gba", 0x00d5770a, 0x00000002
	.section .rom.00d5784a, "ax"
	.incbin "baserom.gba", 0x00d5784a, 0x00000002
	.section .rom.00d59216, "ax"
	.incbin "baserom.gba", 0x00d59216, 0x00000002
	.section .rom.00d5930d, "ax"
	.incbin "baserom.gba", 0x00d5930d, 0x00000003
	.section .rom.00d5c161, "ax"
	.incbin "baserom.gba", 0x00d5c161, 0x00000003
	.section .rom.00d5c30a, "ax"
	.incbin "baserom.gba", 0x00d5c30a, 0x00000002
	.section .rom.00d5ef26, "ax"
	.incbin "baserom.gba", 0x00d5ef26, 0x00000002
	.section .rom.00d61c59, "ax"
	.incbin "baserom.gba", 0x00d61c59, 0x00000003
	.section .rom.00d63943, "ax"
	.incbin "baserom.gba", 0x00d63943, 0x00000001
	.section .rom.00d671bd, "ax"
	.incbin "baserom.gba", 0x00d671bd, 0x00000003
	.section .rom.00d67315, "ax"
	.incbin "baserom.gba", 0x00d67315, 0x00000003
	.section .rom.00d69859, "ax"
	.incbin "baserom.gba", 0x00d69859, 0x00000003
	.section .rom.00d6d71e, "ax"
	.incbin "baserom.gba", 0x00d6d71e, 0x00000002
	.section .rom.00d6f84e, "ax"
	.incbin "baserom.gba", 0x00d6f84e, 0x00000002
	.section .rom.00d73a5a, "ax"
	.incbin "baserom.gba", 0x00d73a5a, 0x00000002
	.section .rom.00d73c2e, "ax"
	.incbin "baserom.gba", 0x00d73c2e, 0x00000002
	.section .rom.00d75c6e, "ax"
	.incbin "baserom.gba", 0x00d75c6e, 0x00000002
	.section .rom.00d77065, "ax"
	.incbin "baserom.gba", 0x00d77065, 0x00000003
	.section .rom.00d77ba2, "ax"
	.incbin "baserom.gba", 0x00d77ba2, 0x00000002
	.section .rom.00d78d0d, "ax"
	.incbin "baserom.gba", 0x00d78d0d, 0x00000003
	.section .rom.00d79db9, "ax"
	.incbin "baserom.gba", 0x00d79db9, 0x00000003
	.section .rom.00d79f65, "ax"
	.incbin "baserom.gba", 0x00d79f65, 0x00000003
	.section .rom.00d7ba72, "ax"
	.incbin "baserom.gba", 0x00d7ba72, 0x00000002
	.section .rom.00d7d48f, "ax"
	.incbin "baserom.gba", 0x00d7d48f, 0x00000001
	.section .rom.00d7f961, "ax"
	.incbin "baserom.gba", 0x00d7f961, 0x00000003
	.section .rom.00d80cc3, "ax"
	.incbin "baserom.gba", 0x00d80cc3, 0x00000001
	.section .rom.00d81b77, "ax"
	.incbin "baserom.gba", 0x00d81b77, 0x00000001
	.section .rom.00d83165, "ax"
	.incbin "baserom.gba", 0x00d83165, 0x00000003
	.section .rom.00d846ab, "ax"
	.incbin "baserom.gba", 0x00d846ab, 0x00000001
	.section .rom.00d85475, "ax"
	.incbin "baserom.gba", 0x00d85475, 0x00000003
	.section .rom.00d855e3, "ax"
	.incbin "baserom.gba", 0x00d855e3, 0x00000001
	.section .rom.00d865a2, "ax"
	.incbin "baserom.gba", 0x00d865a2, 0x00000002
	.section .rom.00d8706b, "ax"
	.incbin "baserom.gba", 0x00d8706b, 0x00000001
	.section .rom.00d87c05, "ax"
	.incbin "baserom.gba", 0x00d87c05, 0x00000003
	.section .rom.00d87d55, "ax"
	.incbin "baserom.gba", 0x00d87d55, 0x00000003
	.section .rom.00d8c003, "ax"
	.incbin "baserom.gba", 0x00d8c003, 0x00000001
	.section .rom.00d8da9d, "ax"
	.incbin "baserom.gba", 0x00d8da9d, 0x00000003
	.section .rom.00d8f473, "ax"
	.incbin "baserom.gba", 0x00d8f473, 0x00000001
	.section .rom.00d8f5f2, "ax"
	.incbin "baserom.gba", 0x00d8f5f2, 0x00000002
	.section .rom.00d93222, "ax"
	.incbin "baserom.gba", 0x00d93222, 0x00000002
	.section .rom.00d94f7f, "ax"
	.incbin "baserom.gba", 0x00d94f7f, 0x00000001
	.section .rom.00d985a9, "ax"
	.incbin "baserom.gba", 0x00d985a9, 0x00000003
	.section .rom.00d986fa, "ax"
	.incbin "baserom.gba", 0x00d986fa, 0x00000002
	.section .rom.00d9e2f6, "ax"
	.incbin "baserom.gba", 0x00d9e2f6, 0x00000002
	.section .rom.00da0c33, "ax"
	.incbin "baserom.gba", 0x00da0c33, 0x00000001
	.section .rom.00da2575, "ax"
	.incbin "baserom.gba", 0x00da2575, 0x00000003
	.section .rom.00da26de, "ax"
	.incbin "baserom.gba", 0x00da26de, 0x00000002
	.section .rom.00da94dd, "ax"
	.incbin "baserom.gba", 0x00da94dd, 0x00000003
	.section .rom.00da9abb, "ax"
	.incbin "baserom.gba", 0x00da9abb, 0x00000001
	.section .rom.00daa9c3, "ax"
	.incbin "baserom.gba", 0x00daa9c3, 0x00000001
	.section .rom.00dad20b, "ax"
	.incbin "baserom.gba", 0x00dad20b, 0x00000001
	.section .rom.00dae82a, "ax"
	.incbin "baserom.gba", 0x00dae82a, 0x00000002
	.section .rom.00db018b, "ax"
	.incbin "baserom.gba", 0x00db018b, 0x00000001
	.section .rom.00db2f73, "ax"
	.incbin "baserom.gba", 0x00db2f73, 0x00000001
	.section .rom.00db30ee, "ax"
	.incbin "baserom.gba", 0x00db30ee, 0x00000002
	.section .rom.00dc22b7, "ax"
	.incbin "baserom.gba", 0x00dc22b7, 0x00000001
	.section .rom.00dc241f, "ax"
	.incbin "baserom.gba", 0x00dc241f, 0x00000001
	.section .rom.00dc4a42, "ax"
	.incbin "baserom.gba", 0x00dc4a42, 0x00000002
	.section .rom.00dc4b85, "ax"
	.incbin "baserom.gba", 0x00dc4b85, 0x00000003
	.section .rom.00dc6442, "ax"
	.incbin "baserom.gba", 0x00dc6442, 0x00000002
	.section .rom.00dc93a6, "ax"
	.incbin "baserom.gba", 0x00dc93a6, 0x00000002
	.section .rom.00dcbd3e, "ax"
	.incbin "baserom.gba", 0x00dcbd3e, 0x00000002
	.section .rom.00dd0696, "ax"
	.incbin "baserom.gba", 0x00dd0696, 0x00000002
	.section .rom.00dd2cbf, "ax"
	.incbin "baserom.gba", 0x00dd2cbf, 0x00000001
	.section .rom.00dd2e93, "ax"
	.incbin "baserom.gba", 0x00dd2e93, 0x00000001
	.section .rom.00dd61b2, "ax"
	.incbin "baserom.gba", 0x00dd61b2, 0x00000002
	.section .rom.00dd6389, "ax"
	.incbin "baserom.gba", 0x00dd6389, 0x00000003
	.section .rom.00dd96fd, "ax"
	.incbin "baserom.gba", 0x00dd96fd, 0x00000003
	.section .rom.00ddae51, "ax"
	.incbin "baserom.gba", 0x00ddae51, 0x00000003
	.section .rom.00ddbddf, "ax"
	.incbin "baserom.gba", 0x00ddbddf, 0x00000001
	.section .rom.00ddd755, "ax"
	.incbin "baserom.gba", 0x00ddd755, 0x00000003
	.section .rom.00ddd88f, "ax"
	.incbin "baserom.gba", 0x00ddd88f, 0x00000001
	.section .rom.00ddf3d1, "ax"
	.incbin "baserom.gba", 0x00ddf3d1, 0x00000003
	.section .rom.00de29a9, "ax"
	.incbin "baserom.gba", 0x00de29a9, 0x00000003
	.section .rom.00de3633, "ax"
	.incbin "baserom.gba", 0x00de3633, 0x00000001
	.section .rom.00de3773, "ax"
	.incbin "baserom.gba", 0x00de3773, 0x00000001
	.section .rom.00de5537, "ax"
	.incbin "baserom.gba", 0x00de5537, 0x00000001
	.section .rom.00de56aa, "ax"
	.incbin "baserom.gba", 0x00de56aa, 0x00000002
	.section .rom.00de7b12, "ax"
	.incbin "baserom.gba", 0x00de7b12, 0x00000002
	.section .rom.00de94ae, "ax"
	.incbin "baserom.gba", 0x00de94ae, 0x00000002
	.section .rom.00deb2e3, "ax"
	.incbin "baserom.gba", 0x00deb2e3, 0x00000001
	.section .rom.00debd9a, "ax"
	.incbin "baserom.gba", 0x00debd9a, 0x00000002
	.section .rom.00ded7b6, "ax"
	.incbin "baserom.gba", 0x00ded7b6, 0x00000002
	.section .rom.00df0482, "ax"
	.incbin "baserom.gba", 0x00df0482, 0x00000002
	.section .rom.00df065d, "ax"
	.incbin "baserom.gba", 0x00df065d, 0x00000003
	.section .rom.00df2193, "ax"
	.incbin "baserom.gba", 0x00df2193, 0x00000001
	.section .rom.00df525d, "ax"
	.incbin "baserom.gba", 0x00df525d, 0x00000003
	.section .rom.00df6419, "ax"
	.incbin "baserom.gba", 0x00df6419, 0x00000003
	.section .rom.00df6601, "ax"
	.incbin "baserom.gba", 0x00df6601, 0x00000003
	.section .rom.00df6793, "ax"
	.incbin "baserom.gba", 0x00df6793, 0x00000001
	.section .rom.00df751a, "ax"
	.incbin "baserom.gba", 0x00df751a, 0x00000002
	.section .rom.00df76af, "ax"
	.incbin "baserom.gba", 0x00df76af, 0x00000001
	.section .rom.00df9763, "ax"
	.incbin "baserom.gba", 0x00df9763, 0x00000001
	.section .rom.00e017e6, "ax"
	.incbin "baserom.gba", 0x00e017e6, 0x00000002
	.section .rom.00e01927, "ax"
	.incbin "baserom.gba", 0x00e01927, 0x00000001
	.section .rom.00e05caf, "ax"
	.incbin "baserom.gba", 0x00e05caf, 0x00000001
	.section .rom.00e063ce, "ax"
	.incbin "baserom.gba", 0x00e063ce, 0x00000002
	.section .rom.00e0778d, "ax"
	.incbin "baserom.gba", 0x00e0778d, 0x00000003
	.section .rom.00e0b8a7, "ax"
	.incbin "baserom.gba", 0x00e0b8a7, 0x00000001
	.section .rom.00e0c927, "ax"
	.incbin "baserom.gba", 0x00e0c927, 0x00000001
	.section .rom.00e0d9a9, "ax"
	.incbin "baserom.gba", 0x00e0d9a9, 0x00000003
	.section .rom.00e0e51a, "ax"
	.incbin "baserom.gba", 0x00e0e51a, 0x00000002
	.section .rom.00e0f46b, "ax"
	.incbin "baserom.gba", 0x00e0f46b, 0x00000001
	.section .rom.00e0f5e9, "ax"
	.incbin "baserom.gba", 0x00e0f5e9, 0x00000003
	.section .rom.00e12b43, "ax"
	.incbin "baserom.gba", 0x00e12b43, 0x00000001
	.section .rom.00e14176, "ax"
	.incbin "baserom.gba", 0x00e14176, 0x00000002
	.section .rom.00e142b7, "ax"
	.incbin "baserom.gba", 0x00e142b7, 0x00000001
	.section .rom.00e16c02, "ax"
	.incbin "baserom.gba", 0x00e16c02, 0x00000002
	.section .rom.00e1f5aa, "ax"
	.incbin "baserom.gba", 0x00e1f5aa, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1f5ac, 0x00000fdc
	.section .rom.00e22155, "ax"
	.incbin "baserom.gba", 0x00e22155, 0x00000003
	.section .rom.00e24095, "ax"
	.incbin "baserom.gba", 0x00e24095, 0x00000003
	.section .rom.00e25509, "ax"
	.incbin "baserom.gba", 0x00e25509, 0x00000003
	.section .rom.00e271c3, "ax"
	.incbin "baserom.gba", 0x00e271c3, 0x00000001
	.section .rom.00e34272, "ax"
	.incbin "baserom.gba", 0x00e34272, 0x00000002
	.section .rom.00e343b1, "ax"
	.incbin "baserom.gba", 0x00e343b1, 0x00000003
	.section .rom.00e362d6, "ax"
	.incbin "baserom.gba", 0x00e362d6, 0x00000002
	.section .rom.00e363ff, "ax"
	.incbin "baserom.gba", 0x00e363ff, 0x00000001
	.section .rom.00e385cf, "ax"
	.incbin "baserom.gba", 0x00e385cf, 0x00000001
	.section .rom.00e3ab9f, "ax"
	.incbin "baserom.gba", 0x00e3ab9f, 0x00000001
	.section .rom.00e3ceb1, "ax"
	.incbin "baserom.gba", 0x00e3ceb1, 0x00000003
	.section .rom.00e3d725, "ax"
	.incbin "baserom.gba", 0x00e3d725, 0x00000003
	.section .rom.00e3d867, "ax"
	.incbin "baserom.gba", 0x00e3d867, 0x00000001
	.section .rom.00e4091d, "ax"
	.incbin "baserom.gba", 0x00e4091d, 0x00000003
	.section .rom.00e42003, "ax"
	.incbin "baserom.gba", 0x00e42003, 0x00000001
	.section .rom.00e43bcb, "ax"
	.incbin "baserom.gba", 0x00e43bcb, 0x00000001
	.section .rom.00e45746, "ax"
	.incbin "baserom.gba", 0x00e45746, 0x00000002
	.section .rom.00e45b91, "ax"
	.incbin "baserom.gba", 0x00e45b91, 0x00000003
	.section .rom.00e47cf2, "ax"
	.incbin "baserom.gba", 0x00e47cf2, 0x00000002
	.section .rom.00e47e0b, "ax"
	.incbin "baserom.gba", 0x00e47e0b, 0x00000001
	.section .rom.00e48c35, "ax"
	.incbin "baserom.gba", 0x00e48c35, 0x00000003
	.section .rom.00e48d97, "ax"
	.incbin "baserom.gba", 0x00e48d97, 0x00000001
	.section .rom.00e4d5ff, "ax"
	.incbin "baserom.gba", 0x00e4d5ff, 0x00000001
	.section .rom.00e4fcbd, "ax"
	.incbin "baserom.gba", 0x00e4fcbd, 0x00000003
	.section .rom.00e5029b, "ax"
	.incbin "baserom.gba", 0x00e5029b, 0x00000001
	.section .rom.00e5222e, "ax"
	.incbin "baserom.gba", 0x00e5222e, 0x00000002
	.section .rom.00e53a0e, "ax"
	.incbin "baserom.gba", 0x00e53a0e, 0x00000002
	.section .rom.00e53b72, "ax"
	.incbin "baserom.gba", 0x00e53b72, 0x00000002
	.section .rom.00e5636a, "ax"
	.incbin "baserom.gba", 0x00e5636a, 0x00000002
	.section .rom.00e56523, "ax"
	.incbin "baserom.gba", 0x00e56523, 0x00000001
	.section .rom.00e566ab, "ax"
	.incbin "baserom.gba", 0x00e566ab, 0x00000001
	.section .rom.00e57ecd, "ax"
	.incbin "baserom.gba", 0x00e57ecd, 0x00000003
	.section .rom.00e58ab6, "ax"
	.incbin "baserom.gba", 0x00e58ab6, 0x00000002
	.section .rom.00e59fef, "ax"
	.incbin "baserom.gba", 0x00e59fef, 0x00000001
	.section .rom.00e5a18a, "ax"
	.incbin "baserom.gba", 0x00e5a18a, 0x00000002
	.section .rom.00e5c436, "ax"
	.incbin "baserom.gba", 0x00e5c436, 0x00000002
	.section .rom.00e5e5fd, "ax"
	.incbin "baserom.gba", 0x00e5e5fd, 0x00000003
	.section .rom.00e5fcfb, "ax"
	.incbin "baserom.gba", 0x00e5fcfb, 0x00000001
	.section .rom.00e6081f, "ax"
	.incbin "baserom.gba", 0x00e6081f, 0x00000001
	.section .rom.00e656c2, "ax"
	.incbin "baserom.gba", 0x00e656c2, 0x00000002
	.section .rom.00e67a69, "ax"
	.incbin "baserom.gba", 0x00e67a69, 0x00000003
	.section .rom.00e68152, "ax"
	.incbin "baserom.gba", 0x00e68152, 0x00000002
	.section .rom.00e68d3e, "ax"
	.incbin "baserom.gba", 0x00e68d3e, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e68d40, 0x00000ac4
	.section .rom.00e6adb3, "ax"
	.incbin "baserom.gba", 0x00e6adb3, 0x00000001
	.section .rom.00e6af66, "ax"
	.incbin "baserom.gba", 0x00e6af66, 0x00000002
	.section .rom.00e6d1a5, "ax"
	.incbin "baserom.gba", 0x00e6d1a5, 0x00000003
	.section .rom.00e6dccb, "ax"
	.incbin "baserom.gba", 0x00e6dccb, 0x00000001
	.section .rom.00e7875c, "ax"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e7875c, 0x000007d0
	.section .rom.00e7bbf5, "ax"
	.incbin "baserom.gba", 0x00e7bbf5, 0x00000003
	.section .rom.00e7bd6f, "ax"
	.incbin "baserom.gba", 0x00e7bd6f, 0x00000001
	.section .rom.00e7bee6, "ax"
	.incbin "baserom.gba", 0x00e7bee6, 0x00000002
	.section .rom.00e7c07e, "ax"
	.incbin "baserom.gba", 0x00e7c07e, 0x00000002
	.section .rom.00e7c20f, "ax"
	.incbin "baserom.gba", 0x00e7c20f, 0x00000001
	.section .rom.00e7e4bb, "ax"
	.incbin "baserom.gba", 0x00e7e4bb, 0x00000001
	.section .rom.00e7e5d3, "ax"
	.incbin "baserom.gba", 0x00e7e5d3, 0x00000001
	.section .rom.00e803d5, "ax"
	.incbin "baserom.gba", 0x00e803d5, 0x00000003
	.section .rom.00e81e9a, "ax"
	.incbin "baserom.gba", 0x00e81e9a, 0x00000002
	.section .rom.00e825c2, "ax"
	.incbin "baserom.gba", 0x00e825c2, 0x00000002
	.section .rom.00e839be, "ax"
	.incbin "baserom.gba", 0x00e839be, 0x00000002
	.section .rom.00e84892, "ax"
	.incbin "baserom.gba", 0x00e84892, 0x00000002
	.section .rom.00e849d6, "ax"
	.incbin "baserom.gba", 0x00e849d6, 0x00000002
	.section .rom.00e86d87, "ax"
	.incbin "baserom.gba", 0x00e86d87, 0x00000001
	.section .rom.00e895b5, "ax"
	.incbin "baserom.gba", 0x00e895b5, 0x00000003
	.section .rom.00e899b2, "ax"
	.incbin "baserom.gba", 0x00e899b2, 0x00000002
	.section .rom.00e8ae52, "ax"
	.incbin "baserom.gba", 0x00e8ae52, 0x00000002
	.section .rom.00e8afa6, "ax"
	.incbin "baserom.gba", 0x00e8afa6, 0x00000002
	.section .rom.00e8e8f3, "ax"
	.incbin "baserom.gba", 0x00e8e8f3, 0x00000001
	.section .rom.00e8fd5e, "ax"
	.incbin "baserom.gba", 0x00e8fd5e, 0x00000002
	.section .rom.00e91237, "ax"
	.incbin "baserom.gba", 0x00e91237, 0x00000001
	.section .rom.00e9138a, "ax"
	.incbin "baserom.gba", 0x00e9138a, 0x00000002
	.section .rom.00e94d97, "ax"
	.incbin "baserom.gba", 0x00e94d97, 0x00000001
	.section .rom.00e95f6f, "ax"
	.incbin "baserom.gba", 0x00e95f6f, 0x00000001
	.section .rom.00e9777a, "ax"
	.incbin "baserom.gba", 0x00e9777a, 0x00000002
	.section .rom.00e978ca, "ax"
	.incbin "baserom.gba", 0x00e978ca, 0x00000002
	.section .rom.00e99fd7, "ax"
	.incbin "baserom.gba", 0x00e99fd7, 0x00000001
	.section .rom.00e9a13e, "ax"
	.incbin "baserom.gba", 0x00e9a13e, 0x00000002
	.section .rom.00e9d0ff, "ax"
	.incbin "baserom.gba", 0x00e9d0ff, 0x00000001
	.section .rom.00e9e36d, "ax"
	.incbin "baserom.gba", 0x00e9e36d, 0x00000003
	.section .rom.00ea0efa, "ax"
	.incbin "baserom.gba", 0x00ea0efa, 0x00000002
	.section .rom.00ea18e1, "ax"
	.incbin "baserom.gba", 0x00ea18e1, 0x00000003
	.section .rom.00ea2282, "ax"
	.incbin "baserom.gba", 0x00ea2282, 0x00000002
	.section .rom.00ea23f6, "ax"
	.incbin "baserom.gba", 0x00ea23f6, 0x00000002
	.section .rom.00ea3f07, "ax"
	.incbin "baserom.gba", 0x00ea3f07, 0x00000001
	.section .rom.00ea4029, "ax"
	.incbin "baserom.gba", 0x00ea4029, 0x00000003
	.section .rom.00ea6b27, "ax"
	.incbin "baserom.gba", 0x00ea6b27, 0x00000001
	.section .rom.00ea791e, "ax"
	.incbin "baserom.gba", 0x00ea791e, 0x00000002
	.section .rom.00ea98bd, "ax"
	.incbin "baserom.gba", 0x00ea98bd, 0x00000003
	.section .rom.00eaabed, "ax"
	.incbin "baserom.gba", 0x00eaabed, 0x00000003
	.section .rom.00eaad41, "ax"
	.incbin "baserom.gba", 0x00eaad41, 0x00000003
	.section .rom.00eab8f3, "ax"
	.incbin "baserom.gba", 0x00eab8f3, 0x00000001
	.section .rom.00ead0cd, "ax"
	.incbin "baserom.gba", 0x00ead0cd, 0x00000003
	.section .rom.00eae22d, "ax"
	.incbin "baserom.gba", 0x00eae22d, 0x00000003
	.section .rom.00eaf34b, "ax"
	.incbin "baserom.gba", 0x00eaf34b, 0x00000001
	.section .rom.00eaf552, "ax"
	.incbin "baserom.gba", 0x00eaf552, 0x00000002
	.section .rom.00eb0247, "ax"
	.incbin "baserom.gba", 0x00eb0247, 0x00000001
	.section .rom.00eb29e9, "ax"
	.incbin "baserom.gba", 0x00eb29e9, 0x00000003
	.section .rom.00eb415e, "ax"
	.incbin "baserom.gba", 0x00eb415e, 0x00000002
	.section .rom.00eb541b, "ax"
	.incbin "baserom.gba", 0x00eb541b, 0x00000001
	.section .rom.00eb5c5e, "ax"
	.incbin "baserom.gba", 0x00eb5c5e, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb5c60, 0x0000060c
	.section .rom.00eb67e3, "ax"
	.incbin "baserom.gba", 0x00eb67e3, 0x00000001
	.section .rom.00eb6923, "ax"
	.incbin "baserom.gba", 0x00eb6923, 0x00000001
	.section .rom.00eb6a63, "ax"
	.incbin "baserom.gba", 0x00eb6a63, 0x00000001
	.section .rom.00eb75c5, "ax"
	.incbin "baserom.gba", 0x00eb75c5, 0x00000003
	.section .rom.00eb9d69, "ax"
	.incbin "baserom.gba", 0x00eb9d69, 0x00000003
	.section .rom.00ebb4de, "ax"
	.incbin "baserom.gba", 0x00ebb4de, 0x00000002
	.section .rom.00ebc79b, "ax"
	.incbin "baserom.gba", 0x00ebc79b, 0x00000001
	.section .rom.00ebcfde, "ax"
	.incbin "baserom.gba", 0x00ebcfde, 0x00000002
	.section .rom.00ebd57d, "ax"
	.incbin "baserom.gba", 0x00ebd57d, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebd580, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebd58c, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebd6dc, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebd81c, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebd95c, 0x00000140
	.section .rom.00ebe1b1, "ax"
	.incbin "baserom.gba", 0x00ebe1b1, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe1b4, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe1c0, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebe310, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebe450, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebe590, 0x00000140
	.section .rom.00ebf291, "ax"
	.incbin "baserom.gba", 0x00ebf291, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf294, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf2a0, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebf3f0, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebf530, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebf670, 0x00000140
	.section .rom.00ebfd7d, "ax"
	.incbin "baserom.gba", 0x00ebfd7d, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ebfd80, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ebfd8c, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ebfedc, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec001c, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec015c, 0x00000140
	.section .rom.00ec0c19, "ax"
	.incbin "baserom.gba", 0x00ec0c19, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec0c1c, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec0c28, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec0d78, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec0eb8, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec0ff8, 0x00000140
	.section .rom.00ec1561, "ax"
	.incbin "baserom.gba", 0x00ec1561, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1564, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1570, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec16c0, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1800, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec1940, 0x00000140
	.section .rom.00ec20a9, "ax"
	.incbin "baserom.gba", 0x00ec20a9, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec20ac, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec20b8, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2208, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec2348, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2488, 0x00000140
	.section .rom.00ec2cf1, "ax"
	.incbin "baserom.gba", 0x00ec2cf1, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec2cf4, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec2d00, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec2e50, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec2f90, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec30d0, 0x00000140
	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "baserom.gba", 0x00ec3210, 0x00000490
	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "baserom.gba", 0x00ec36a0, 0x00001d0c
	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "baserom.gba", 0x00ec53ac, 0x00003df0
	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "baserom.gba", 0x00ec919c, 0x00002df0
	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "baserom.gba", 0x00ecbf8c, 0x000019ec
	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "baserom.gba", 0x00ecd978, 0x000011b4
	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "baserom.gba", 0x00eceb2c, 0x000007c8
	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "baserom.gba", 0x00ecf2f4, 0x00001860
	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "baserom.gba", 0x00ed0b54, 0x000009d4
	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "baserom.gba", 0x00ed1528, 0x00000b34
	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "baserom.gba", 0x00ed205c, 0x00003234
	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "baserom.gba", 0x00ed5290, 0x000024c0
	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "baserom.gba", 0x00ed7750, 0x00002da8
	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "baserom.gba", 0x00eda4f8, 0x00001cf8
	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "baserom.gba", 0x00edc1f0, 0x0000114c
	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "baserom.gba", 0x00edd33c, 0x00001608
	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "baserom.gba", 0x00ede944, 0x000014b0
	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "baserom.gba", 0x00edfdf4, 0x00000e44
	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "baserom.gba", 0x00ee0c38, 0x00000274
	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "baserom.gba", 0x00ee0eac, 0x000001e4
	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "baserom.gba", 0x00ee1090, 0x000006e4
	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "baserom.gba", 0x00ee1774, 0x00000574
	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "baserom.gba", 0x00ee1ce8, 0x000025a4
	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "baserom.gba", 0x00ee428c, 0x00000714
	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "baserom.gba", 0x00ee49a0, 0x00003220
	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "baserom.gba", 0x00ee7bc0, 0x000014bc
	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "baserom.gba", 0x00ee907c, 0x0000294c
	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "baserom.gba", 0x00eeb9c8, 0x00004158
	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "baserom.gba", 0x00eefb20, 0x00001520
	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "baserom.gba", 0x00ef1040, 0x00000a9c
	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "baserom.gba", 0x00ef1adc, 0x00000bf8
	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "baserom.gba", 0x00ef26d4, 0x00002984
	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "baserom.gba", 0x00ef5058, 0x00001458
	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "baserom.gba", 0x00ef64b0, 0x00000dac
	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "baserom.gba", 0x00ef725c, 0x00001274
	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "baserom.gba", 0x00ef84d0, 0x000001d8
	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "baserom.gba", 0x00ef86a8, 0x000005a8
	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "baserom.gba", 0x00ef8c50, 0x00000a98
	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "baserom.gba", 0x00ef96e8, 0x00000de4
	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "baserom.gba", 0x00efa4cc, 0x0000374c
	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "baserom.gba", 0x00efdc18, 0x00004050
	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "baserom.gba", 0x00f01c68, 0x000012e8
	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "baserom.gba", 0x00f02f50, 0x00000a68
	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "baserom.gba", 0x00f039b8, 0x00002b2c
	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "baserom.gba", 0x00f064e4, 0x00000d94
	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "baserom.gba", 0x00f07278, 0x000015cc
	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "baserom.gba", 0x00f08844, 0x00000718
	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "baserom.gba", 0x00f08f5c, 0x00001eec
	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "baserom.gba", 0x00f0ae48, 0x000018e0
	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "baserom.gba", 0x00f0c728, 0x0000010c
	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "baserom.gba", 0x00f0c834, 0x00001fe0
	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "baserom.gba", 0x00f0e814, 0x0000336c
	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "baserom.gba", 0x00f11b80, 0x00001168
	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "baserom.gba", 0x00f12ce8, 0x00000a88
	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "baserom.gba", 0x00f13770, 0x00000fc0
	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "baserom.gba", 0x00f14730, 0x00000d10
	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "baserom.gba", 0x00f15440, 0x00003840
	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "baserom.gba", 0x00f18c80, 0x000014c8
	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "baserom.gba", 0x00f1a148, 0x0000035c
	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "baserom.gba", 0x00f1a4a4, 0x000000d0
	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "baserom.gba", 0x00f1a574, 0x00001ffc
	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "baserom.gba", 0x00f1c570, 0x0000219c
	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "baserom.gba", 0x00f1e70c, 0x000016e0
	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "baserom.gba", 0x00f1fdec, 0x000004a0
	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "baserom.gba", 0x00f2028c, 0x000003e8
	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "baserom.gba", 0x00f20674, 0x00000c10
	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "baserom.gba", 0x00f21284, 0x000012e8
	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "baserom.gba", 0x00f2256c, 0x00001228
	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "baserom.gba", 0x00f23794, 0x00001c28
	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "baserom.gba", 0x00f253bc, 0x00000df8
	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "baserom.gba", 0x00f261b4, 0x00002470
	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "baserom.gba", 0x00f28624, 0x00001910
	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "baserom.gba", 0x00f29f34, 0x00002cc4
	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "baserom.gba", 0x00f2cbf8, 0x000039e8
	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "baserom.gba", 0x00f305e0, 0x00000be4
	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "baserom.gba", 0x00f311c4, 0x00002e00
	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "baserom.gba", 0x00f33fc4, 0x00000b1c
	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "baserom.gba", 0x00f34ae0, 0x00003314
	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "baserom.gba", 0x00f37df4, 0x00004450
	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "baserom.gba", 0x00f3c244, 0x00001978
	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "baserom.gba", 0x00f3dbbc, 0x00001af8
	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "baserom.gba", 0x00f3f6b4, 0x000019ac
	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "baserom.gba", 0x00f41060, 0x00001f00
	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "baserom.gba", 0x00f42f60, 0x00002140
	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "baserom.gba", 0x00f450a0, 0x00001cb0
	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "baserom.gba", 0x00f46d50, 0x0000246c
	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "baserom.gba", 0x00f491bc, 0x00001a88
	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "baserom.gba", 0x00f4ac44, 0x000023b0
	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "baserom.gba", 0x00f4cff4, 0x0000205c
	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "baserom.gba", 0x00f4f050, 0x00001b4c
	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "baserom.gba", 0x00f50b9c, 0x000032b8
	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "baserom.gba", 0x00f53e54, 0x00002460
	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "baserom.gba", 0x00f562b4, 0x000037a4
	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "baserom.gba", 0x00f59a58, 0x00000fc8
	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "baserom.gba", 0x00f5aa20, 0x00000814
	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "baserom.gba", 0x00f5b234, 0x00002d5c
	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "baserom.gba", 0x00f5df90, 0x00000d70
	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "baserom.gba", 0x00f5ed00, 0x00003c08
	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "baserom.gba", 0x00f62908, 0x00002f0c
	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "baserom.gba", 0x00f65814, 0x00002274
	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "baserom.gba", 0x00f67a88, 0x000045b0
	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "baserom.gba", 0x00f6c038, 0x00003344
	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "baserom.gba", 0x00f6f37c, 0x00000d80
	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "baserom.gba", 0x00f700fc, 0x000028b4
	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "baserom.gba", 0x00f729b0, 0x000015ac
	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "baserom.gba", 0x00f73f5c, 0x00000430
	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "baserom.gba", 0x00f7438c, 0x00000554
	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "baserom.gba", 0x00f748e0, 0x00000e50
	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "baserom.gba", 0x00f75730, 0x000013f8
	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "baserom.gba", 0x00f76b28, 0x000000b0
	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "baserom.gba", 0x00f76bd8, 0x00000770
	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "baserom.gba", 0x00f77348, 0x00000cd8
	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "baserom.gba", 0x00f78020, 0x00000518
	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "baserom.gba", 0x00f78538, 0x00087ac8
