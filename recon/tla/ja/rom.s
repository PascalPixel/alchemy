@ tla-ja's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Resource_Data000
Resource_Data000:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00000630, "ax"
	.incbin "baserom.gba", 0x00000630, 0x00000088
	.section .rom.00000810, "ax"
	.incbin "baserom.gba", 0x00000810, 0x00000008
	.global IwramFillWords
IwramFillWords:
	.incbin "baserom.gba", 0x00000818, 0x000000f4
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
	.incbin "baserom.gba", 0x0001319c, 0x0000011c
	.section .rom.000132b8, "ax"
	.global Func_080132b8
	.type Func_080132b8, %function
	.thumb_func
Func_080132b8:
	.incbin "baserom.gba", 0x000132b8, 0x00000004
	.section .rom.000132bc, "ax"
	.global Func_080132bc
	.type Func_080132bc, %function
	.thumb_func
Func_080132bc:
	.incbin "baserom.gba", 0x000132bc, 0x00000004
	.section .rom.000132c0, "ax"
	.global Func_080132c0
	.type Func_080132c0, %function
	.thumb_func
Func_080132c0:
	.incbin "baserom.gba", 0x000132c0, 0x00000004
	.section .rom.000132c4, "ax"
	.global Func_080132c4
	.type Func_080132c4, %function
	.thumb_func
Func_080132c4:
	.incbin "baserom.gba", 0x000132c4, 0x00000004
	.section .rom.000132c8, "ax"
	.global Func_080132c8
	.type Func_080132c8, %function
	.thumb_func
Func_080132c8:
	.incbin "baserom.gba", 0x000132c8, 0x00000004
	.section .rom.000132d0, "ax"
	.global Func_080132d0
	.type Func_080132d0, %function
	.thumb_func
Func_080132d0:
	.incbin "baserom.gba", 0x000132d0, 0x0000002c
	.section .rom.000132fc, "ax"
	.global Func_080132fc
	.type Func_080132fc, %function
	.thumb_func
Func_080132fc:
	.incbin "baserom.gba", 0x000132fc, 0x00000004
	.section .rom.0001330c, "ax"
	.incbin "baserom.gba", 0x0001330c, 0x00000060
	.section .rom.0001336c, "ax"
	.global Resource_LoadCode
	.type Resource_LoadCode, %function
	.thumb_func
Resource_LoadCode:
	.incbin "baserom.gba", 0x0001336c, 0x000000cc
	.section .rom.00013438, "ax"
	.global Func_08013438
	.type Func_08013438, %function
	.thumb_func
Func_08013438:
	.incbin "baserom.gba", 0x00013438, 0x00000128
	.section .rom.00013560, "ax"
	.global WaitFrames
	.type WaitFrames, %function
	.thumb_func
WaitFrames:
	.incbin "baserom.gba", 0x00013560, 0x00000348
	.section .rom.000138a8, "ax"
	.global Runtime_SetMainState19
	.type Runtime_SetMainState19, %function
	.thumb_func
Runtime_SetMainState19:
	.incbin "baserom.gba", 0x000138a8, 0x00000288
	.section .rom.00013b30, "ax"
	.global Sound_LoadPresetParameters
	.type Sound_LoadPresetParameters, %function
	.thumb_func
Sound_LoadPresetParameters:
	.incbin "baserom.gba", 0x00013b30, 0x00000038
	.section .rom.00013b68, "ax"
	.global Func_08013b68
	.type Func_08013b68, %function
	.thumb_func
Func_08013b68:
	.incbin "baserom.gba", 0x00013b68, 0x0000003c
	.section .rom.00013ba4, "ax"
	.global QueueIoWriteDelay2
	.type QueueIoWriteDelay2, %function
	.thumb_func
QueueIoWriteDelay2:
	.incbin "baserom.gba", 0x00013ba4, 0x0000003c
	.section .rom.00013be0, "ax"
	.global Func_08013be0
	.type Func_08013be0, %function
	.thumb_func
Func_08013be0:
	.incbin "baserom.gba", 0x00013be0, 0x0000003c
	.section .rom.00013c1c, "ax"
	.global Func_08013c1c
	.type Func_08013c1c, %function
	.thumb_func
Func_08013c1c:
	.incbin "baserom.gba", 0x00013c1c, 0x0000003c
	.section .rom.00013c58, "ax"
	.global Func_08013c58
	.type Func_08013c58, %function
	.thumb_func
Func_08013c58:
	.incbin "baserom.gba", 0x00013c58, 0x0000003c
	.section .rom.00013c94, "ax"
	.global Func_08013c94
	.type Func_08013c94, %function
	.thumb_func
Func_08013c94:
	.incbin "baserom.gba", 0x00013c94, 0x0000003c
	.section .rom.00013cd0, "ax"
	.global Func_08013cd0
	.type Func_08013cd0, %function
	.thumb_func
Func_08013cd0:
	.incbin "baserom.gba", 0x00013cd0, 0x0000003c
	.section .rom.00013d0c, "ax"
	.global Func_08013d0c
	.type Func_08013d0c, %function
	.thumb_func
Func_08013d0c:
	.incbin "baserom.gba", 0x00013d0c, 0x0000003c
	.section .rom.00013d48, "ax"
	.global Func_08013d48
	.type Func_08013d48, %function
	.thumb_func
Func_08013d48:
	.incbin "baserom.gba", 0x00013d48, 0x00000128
	.section .rom.00013e70, "ax"
	.global Blend_SetDarkenTarget16
	.type Blend_SetDarkenTarget16, %function
	.thumb_func
Blend_SetDarkenTarget16:
	.incbin "baserom.gba", 0x00013e70, 0x00000044
	.section .rom.00013eb4, "ax"
	.global Func_08013eb4
	.type Func_08013eb4, %function
	.thumb_func
Func_08013eb4:
	.incbin "baserom.gba", 0x00013eb4, 0x00000044
	.section .rom.00013ef8, "ax"
	.global Func_08013ef8
	.type Func_08013ef8, %function
	.thumb_func
Func_08013ef8:
	.incbin "baserom.gba", 0x00013ef8, 0x00000044
	.section .rom.00013f3c, "ax"
	.global Func_08013f3c
	.type Func_08013f3c, %function
	.thumb_func
Func_08013f3c:
	.incbin "baserom.gba", 0x00013f3c, 0x00000044
	.section .rom.00013f80, "ax"
	.global Func_08013f80
	.type Func_08013f80, %function
	.thumb_func
Func_08013f80:
	.incbin "baserom.gba", 0x00013f80, 0x0000005c
	.section .rom.00013ffc, "ax"
	.incbin "baserom.gba", 0x00013ffc, 0x00000020
	.section .rom.0001401c, "ax"
	.global AffineMatrix_BuildForEffect
	.type AffineMatrix_BuildForEffect, %function
	.thumb_func
AffineMatrix_BuildForEffect:
	.incbin "baserom.gba", 0x0001401c, 0x000000bc
	.section .rom.000140d8, "ax"
	.global Func_080140d8
	.type Func_080140d8, %function
	.thumb_func
Func_080140d8:
	.incbin "baserom.gba", 0x000140d8, 0x00000148
	.section .rom.00014220, "ax"
	.global Func_08014220
	.type Func_08014220, %function
	.thumb_func
Func_08014220:
	.incbin "baserom.gba", 0x00014220, 0x00000020
	.section .rom.00014240, "ax"
	.global Resource_ClearSlotReferences
	.type Resource_ClearSlotReferences, %function
	.thumb_func
Resource_ClearSlotReferences:
	.incbin "baserom.gba", 0x00014240, 0x00000034
	.section .rom.00014274, "ax"
	.global Resource_ResetEntry
	.type Resource_ResetEntry, %function
	.thumb_func
Resource_ResetEntry:
	.incbin "baserom.gba", 0x00014274, 0x00000038
	.section .rom.000142d4, "ax"
	.global VramBlock_LoadCached
	.type VramBlock_LoadCached, %function
	.thumb_func
VramBlock_LoadCached:
	.incbin "baserom.gba", 0x000142d4, 0x00000094
	.section .rom.00014368, "ax"
	.global Func_08014368
	.type Func_08014368, %function
	.thumb_func
Func_08014368:
	.incbin "baserom.gba", 0x00014368, 0x00000044
	.section .rom.000143ac, "ax"
	.global Resource_FindFreeEntry
	.type Resource_FindFreeEntry, %function
	.thumb_func
Resource_FindFreeEntry:
	.incbin "baserom.gba", 0x000143ac, 0x00000034
	.section .rom.000143f6, "ax"
	.incbin "baserom.gba", 0x000143f6, 0x00000002
	.section .rom.000143f8, "ax"
	.global Resource_GetBuffer
	.type Resource_GetBuffer, %function
	.thumb_func
Resource_GetBuffer:
	.incbin "baserom.gba", 0x000143f8, 0x000000c8
	.section .rom.000144c0, "ax"
	.global Func_080144c0
	.type Func_080144c0, %function
	.thumb_func
Func_080144c0:
	.incbin "baserom.gba", 0x000144c0, 0x00000044
	.section .rom.0001451a, "ax"
	.incbin "baserom.gba", 0x0001451a, 0x00000052
	.section .rom.0001456c, "ax"
	.global Func_0801456c
	.type Func_0801456c, %function
	.thumb_func
Func_0801456c:
	.incbin "baserom.gba", 0x0001456c, 0x0000003c
	.section .rom.000145a8, "ax"
	.global Scheduler_AddOrUpdateCallback
	.type Scheduler_AddOrUpdateCallback, %function
	.thumb_func
Scheduler_AddOrUpdateCallback:
	.incbin "baserom.gba", 0x000145a8, 0x0000009c
	.section .rom.00014644, "ax"
	.global Scheduler_RemoveCallback
	.type Scheduler_RemoveCallback, %function
	.thumb_func
Scheduler_RemoveCallback:
	.incbin "baserom.gba", 0x00014644, 0x00000050
	.section .rom.00014694, "ax"
	.global Func_08014694
	.type Func_08014694, %function
	.thumb_func
Func_08014694:
	.incbin "baserom.gba", 0x00014694, 0x00000040
	.global Scheduler_EnableUnmaskedOverlayCallbacks
	.type Scheduler_EnableUnmaskedOverlayCallbacks, %function
	.thumb_func
Scheduler_EnableUnmaskedOverlayCallbacks:
	.incbin "baserom.gba", 0x000146d4, 0x00000048
	.section .rom.0001471c, "ax"
	.global Func_0801471c
	.type Func_0801471c, %function
	.thumb_func
Func_0801471c:
	.incbin "baserom.gba", 0x0001471c, 0x00000040
	.section .rom.0001475c, "ax"
	.global Func_0801475c
	.type Func_0801475c, %function
	.thumb_func
Func_0801475c:
	.incbin "baserom.gba", 0x0001475c, 0x00000040
	.section .rom.0001479c, "ax"
	.global Scheduler_DisableOverlayCallbacks
	.type Scheduler_DisableOverlayCallbacks, %function
	.thumb_func
Scheduler_DisableOverlayCallbacks:
	.incbin "baserom.gba", 0x0001479c, 0x0000003c
	.section .rom.000147d8, "ax"
	.global Func_080147d8
	.type Func_080147d8, %function
	.thumb_func
Func_080147d8:
	.incbin "baserom.gba", 0x000147d8, 0x000000a0
	.section .rom.00014878, "ax"
	.global Random16
	.type Random16, %function
	.thumb_func
Random16:
	.incbin "baserom.gba", 0x00014878, 0x00000024
	.section .rom.0001489c, "ax"
	.global Vector_AddPolarOffset
	.type Vector_AddPolarOffset, %function
	.thumb_func
Vector_AddPolarOffset:
	.incbin "baserom.gba", 0x0001489c, 0x0000004c
	.section .rom.000148e8, "ax"
	.global ArcTan2
	.type ArcTan2, %function
	.thumb_func
ArcTan2:
	.incbin "baserom.gba", 0x000148e8, 0x000000cc
	.section .rom.000149e0, "ax"
	.global Func_080149e0
	.type Func_080149e0, %function
	.thumb_func
Func_080149e0:
	.incbin "baserom.gba", 0x000149e0, 0x00000044
	.section .rom.00014a24, "ax"
	.global Text_FormatSignedDecimalToWork
	.type Text_FormatSignedDecimalToWork, %function
	.thumb_func
Text_FormatSignedDecimalToWork:
	.incbin "baserom.gba", 0x00014a24, 0x00000084
	.section .rom.00014aa8, "ax"
	.global Func_08014aa8
	.type Func_08014aa8, %function
	.thumb_func
Func_08014aa8:
	.incbin "baserom.gba", 0x00014aa8, 0x00000038
	.section .rom.00014ae0, "ax"
	.global Func_08014ae0
	.type Func_08014ae0, %function
	.thumb_func
Func_08014ae0:
	.incbin "baserom.gba", 0x00014ae0, 0x00000050
	.section .rom.00014b30, "ax"
	.global Func_08014b30
	.type Func_08014b30, %function
	.thumb_func
Func_08014b30:
	.incbin "baserom.gba", 0x00014b30, 0x00000020
	.section .rom.00014b50, "ax"
	.global Func_08014b50
	.type Func_08014b50, %function
	.thumb_func
Func_08014b50:
	.incbin "baserom.gba", 0x00014b50, 0x00000020
	.section .rom.00014b70, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b70, 0x0000003c
	.section .rom.00014bac, "ax"
	.global Func_08014bac
	.type Func_08014bac, %function
	.thumb_func
Func_08014bac:
	.incbin "baserom.gba", 0x00014bac, 0x000000a0
	.section .rom.00014c4c, "ax"
	.global Func_08014c4c
	.type Func_08014c4c, %function
	.thumb_func
Func_08014c4c:
	.incbin "baserom.gba", 0x00014c4c, 0x00000020
	.section .rom.00014c6c, "ax"
	.global Func_08014c6c
	.type Func_08014c6c, %function
	.thumb_func
Func_08014c6c:
	.incbin "baserom.gba", 0x00014c6c, 0x00000034
	.section .rom.00014ca0, "ax"
	.global Func_08014ca0
	.type Func_08014ca0, %function
	.thumb_func
Func_08014ca0:
	.incbin "baserom.gba", 0x00014ca0, 0x00000010
	.section .rom.00014cb0, "ax"
	.global Func_08014cb0
	.type Func_08014cb0, %function
	.thumb_func
Func_08014cb0:
	.incbin "baserom.gba", 0x00014cb0, 0x00000010
	.section .rom.00014cc0, "ax"
	.global Runtime_AllocateHeapBlock
	.type Runtime_AllocateHeapBlock, %function
	.thumb_func
Runtime_AllocateHeapBlock:
	.incbin "baserom.gba", 0x00014cc0, 0x00000040
	.section .rom.00014d00, "ax"
	.global Runtime_AllocateBlock
	.type Runtime_AllocateBlock, %function
	.thumb_func
Runtime_AllocateBlock:
	.incbin "baserom.gba", 0x00014d00, 0x00000078
	.section .rom.00014d78, "ax"
	.global Runtime_BumpAllocate
	.type Runtime_BumpAllocate, %function
	.thumb_func
Runtime_BumpAllocate:
	.incbin "baserom.gba", 0x00014d78, 0x00000034
	.section .rom.00014dac, "ax"
	.global Runtime_BumpAllocateAlternatePool
	.type Runtime_BumpAllocateAlternatePool, %function
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	.incbin "baserom.gba", 0x00014dac, 0x00000038
	.section .rom.00014de4, "ax"
	.global Func_08014de4
	.type Func_08014de4, %function
	.thumb_func
Func_08014de4:
	.incbin "baserom.gba", 0x00014de4, 0x00000054
	.section .rom.00014e38, "ax"
	.global Func_08014e38
	.type Func_08014e38, %function
	.thumb_func
Func_08014e38:
	.incbin "baserom.gba", 0x00014e38, 0x00000070
	.section .rom.00014ea8, "ax"
	.global Func_08014ea8
	.type Func_08014ea8, %function
	.thumb_func
Func_08014ea8:
	.incbin "baserom.gba", 0x00014ea8, 0x00000038
	.section .rom.00014ee0, "ax"
	.global Func_08014ee0
	.type Func_08014ee0, %function
	.thumb_func
Func_08014ee0:
	.incbin "baserom.gba", 0x00014ee0, 0x0000001c
	.section .rom.00014efc, "ax"
	.global Func_08014efc
	.type Func_08014efc, %function
	.thumb_func
Func_08014efc:
	.incbin "baserom.gba", 0x00014efc, 0x00000128
	.section .rom.00015024, "ax"
	.global SceneTransform_ApplyPitch
	.type SceneTransform_ApplyPitch, %function
	.thumb_func
SceneTransform_ApplyPitch:
	.incbin "baserom.gba", 0x00015024, 0x00000044
	.section .rom.00015068, "ax"
	.global Func_08015068
	.type Func_08015068, %function
	.thumb_func
Func_08015068:
	.incbin "baserom.gba", 0x00015068, 0x0000007c
	.section .rom.000150e4, "ax"
	.global Func_080150e4
	.type Func_080150e4, %function
	.thumb_func
Func_080150e4:
	.incbin "baserom.gba", 0x000150e4, 0x00000044
	.section .rom.00015128, "ax"
	.global SceneTransform_ApplyPosition
	.type SceneTransform_ApplyPosition, %function
	.thumb_func
SceneTransform_ApplyPosition:
	.incbin "baserom.gba", 0x00015128, 0x00000084
	.section .rom.000151ac, "ax"
	.global Func_080151ac
	.type Func_080151ac, %function
	.thumb_func
Func_080151ac:
	.incbin "baserom.gba", 0x000151ac, 0x000000a0
	.section .rom.0001524c, "ax"
	.global Func_0801524c
	.type Func_0801524c, %function
	.thumb_func
Func_0801524c:
	.incbin "baserom.gba", 0x0001524c, 0x00000138
	.section .rom.00015384, "ax"
	.global Func_08015384
	.type Func_08015384, %function
	.thumb_func
Func_08015384:
	.incbin "baserom.gba", 0x00015384, 0x00000364
	.section .rom.000156e8, "ax"
	.global Graphics_PrepareTransferInIwramWork
	.type Graphics_PrepareTransferInIwramWork, %function
	.thumb_func
Graphics_PrepareTransferInIwramWork:
	.incbin "baserom.gba", 0x000156e8, 0x00000010
	.section .rom.000156f8, "ax"
	.global Func_080156f8
	.type Func_080156f8, %function
	.thumb_func
Func_080156f8:
	.incbin "baserom.gba", 0x000156f8, 0x0000001c
	.section .rom.00015714, "ax"
	.global Func_08015714
	.type Func_08015714, %function
	.thumb_func
Func_08015714:
	.incbin "baserom.gba", 0x00015714, 0x00000054
	.section .rom.00015778, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x00015778, 0x00000104
	.section .rom.0001587c, "ax"
	.global Func_0801587c
	.type Func_0801587c, %function
	.thumb_func
Func_0801587c:
	.incbin "baserom.gba", 0x0001587c, 0x000000a0
	.section .rom.0001591c, "ax"
	.global Func_0801591c
	.type Func_0801591c, %function
	.thumb_func
Func_0801591c:
	.incbin "baserom.gba", 0x0001591c, 0x000006e0
	.section .rom.0001601c, "ax"
	.incbin "baserom.gba", 0x0001601c, 0x00000134
	.section .rom.00016180, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x00016180, 0x00000170
	.section .rom.00016346, "ax"
	.incbin "baserom.gba", 0x00016346, 0x00000002
	.section .rom.00016348, "ax"
	.global Func_08016348
	.type Func_08016348, %function
	.thumb_func
Func_08016348:
	.incbin "baserom.gba", 0x00016348, 0x00000464
	.section .rom.000167ac, "ax"
	.global Func_080167ac
	.type Func_080167ac, %function
	.thumb_func
Func_080167ac:
	.incbin "baserom.gba", 0x000167ac, 0x0000002c
	.section .rom.000167d8, "ax"
	.global Func_080167d8
	.type Func_080167d8, %function
	.thumb_func
Func_080167d8:
	.incbin "baserom.gba", 0x000167d8, 0x00000034
	.section .rom.0001680c, "ax"
	.global Func_0801680c
	.type Func_0801680c, %function
	.thumb_func
Func_0801680c:
	.incbin "baserom.gba", 0x0001680c, 0x00000048
	.section .rom.00016854, "ax"
	.global Party_Check
	.type Party_Check, %function
	.thumb_func
Party_Check:
	.incbin "baserom.gba", 0x00016854, 0x0000004c
	.section .rom.000168a0, "ax"
	.global Func_080168a0
	.type Func_080168a0, %function
	.thumb_func
Func_080168a0:
	.incbin "baserom.gba", 0x000168a0, 0x0000002c
	.section .rom.000168cc, "ax"
	.global SerialRuntime_WaitForTransferB
	.type SerialRuntime_WaitForTransferB, %function
	.thumb_func
SerialRuntime_WaitForTransferB:
	.incbin "baserom.gba", 0x000168cc, 0x0000002c
	.section .rom.000168f8, "ax"
	.global Func_080168f8
	.type Func_080168f8, %function
	.thumb_func
Func_080168f8:
	.incbin "baserom.gba", 0x000168f8, 0x00000034
	.section .rom.0001692c, "ax"
	.global Func_0801692c
	.type Func_0801692c, %function
	.thumb_func
Func_0801692c:
	.incbin "baserom.gba", 0x0001692c, 0x00000064
	.section .rom.00016990, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x00016990, 0x00000314
	.section .rom.00016ca4, "ax"
	.global Owner_GetState
	.type Owner_GetState, %function
	.thumb_func
Owner_GetState:
	.incbin "baserom.gba", 0x00016ca4, 0x00000040
	.section .rom.00016cfc, "ax"
	.global GameFlag_SetBit
	.type GameFlag_SetBit, %function
	.thumb_func
GameFlag_SetBit:
	.incbin "baserom.gba", 0x00016cfc, 0x0000001c
	.section .rom.00016d18, "ax"
	.global GameFlag_ClearBit
	.type GameFlag_ClearBit, %function
	.thumb_func
GameFlag_ClearBit:
	.incbin "baserom.gba", 0x00016d18, 0x0000001c
	.section .rom.00016d34, "ax"
	.global Func_08016d34
	.type Func_08016d34, %function
	.thumb_func
Func_08016d34:
	.incbin "baserom.gba", 0x00016d34, 0x00000028
	.section .rom.00016d7c, "ax"
	.global Func_08016d7c
	.type Func_08016d7c, %function
	.thumb_func
Func_08016d7c:
	.incbin "baserom.gba", 0x00016d7c, 0x0000001c
	.section .rom.00016d98, "ax"
	.global Func_08016d98
	.type Func_08016d98, %function
	.thumb_func
Func_08016d98:
	.incbin "baserom.gba", 0x00016d98, 0x0000001c
	.section .rom.00016dd0, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dd0, 0x00000028
	.section .rom.00016e02, "ax"
	.incbin "baserom.gba", 0x00016e02, 0x00000002
	.section .rom.000178b4, "ax"
	.incbin "baserom.gba", 0x000178b4, 0x00000434
	.section .rom.00017d08, "ax"
	.incbin "baserom.gba", 0x00017d08, 0x0000005c
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00017d64, 0x00000014
	.section .rom.00017dc0, "ax"
	.incbin "baserom.gba", 0x00017dc0, 0x00000024
	.section .rom.00017dfc, "ax"
	.incbin "baserom.gba", 0x00017dfc, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00017e14, 0x00000058
	.section .rom.00017e90, "ax"
	.incbin "baserom.gba", 0x00017e90, 0x0000008c
	.section .rom.00017f24, "ax"
	.incbin "baserom.gba", 0x00017f24, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00017f3c, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00017f68, 0x0000002c
	.section .rom.00017fbc, "ax"
	.incbin "baserom.gba", 0x00017fbc, 0x00008044
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
	.global Func_080219cc
	.type Func_080219cc, %function
	.thumb_func
Func_080219cc:
	.incbin "baserom.gba", 0x000219cc, 0x00000644
	.section .rom.00022010, "ax"
	.global Func_08022010
	.type Func_08022010, %function
	.thumb_func
Func_08022010:
	.incbin "baserom.gba", 0x00022010, 0x000000e0
	.section .rom.000220f0, "ax"
	.global Render_ApplyProjectedPlacement
	.type Render_ApplyProjectedPlacement, %function
	.thumb_func
Render_ApplyProjectedPlacement:
	.incbin "baserom.gba", 0x000220f0, 0x00000228
	.section .rom.00022318, "ax"
	.global Func_08022318
	.type Func_08022318, %function
	.thumb_func
Func_08022318:
	.incbin "baserom.gba", 0x00022318, 0x00000234
	.section .rom.0002254c, "ax"
	.global Func_0802254c
	.type Func_0802254c, %function
	.thumb_func
Func_0802254c:
	.incbin "baserom.gba", 0x0002254c, 0x00000254
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
	.incbin "baserom.gba", 0x000227e0, 0x00000100
	.section .rom.000228e0, "ax"
	.global InitializeAnimationObjects
	.type InitializeAnimationObjects, %function
	.thumb_func
InitializeAnimationObjects:
	.incbin "baserom.gba", 0x000228e0, 0x0000008c
	.section .rom.0002296c, "ax"
	.global Animation_InitWorkFromMetadata
	.type Animation_InitWorkFromMetadata, %function
	.thumb_func
Animation_InitWorkFromMetadata:
	.incbin "baserom.gba", 0x0002296c, 0x00000040
	.section .rom.000229ac, "ax"
	.global ResourceMetadata_Register
	.type ResourceMetadata_Register, %function
	.thumb_func
ResourceMetadata_Register:
	.incbin "baserom.gba", 0x000229ac, 0x00000078
	.section .rom.00022a24, "ax"
	.global Func_08022a24
	.type Func_08022a24, %function
	.thumb_func
Func_08022a24:
	.incbin "baserom.gba", 0x00022a24, 0x00000060
	.section .rom.00022a84, "ax"
	.global Func_08022a84
	.type Func_08022a84, %function
	.thumb_func
Func_08022a84:
	.incbin "baserom.gba", 0x00022a84, 0x00000048
	.section .rom.00022acc, "ax"
	.global Func_08022acc
	.type Func_08022acc, %function
	.thumb_func
Func_08022acc:
	.incbin "baserom.gba", 0x00022acc, 0x00000038
	.section .rom.00022b04, "ax"
	.global Animation_ApplyChildArgument
	.type Animation_ApplyChildArgument, %function
	.thumb_func
Animation_ApplyChildArgument:
	.incbin "baserom.gba", 0x00022b04, 0x0000007c
	.section .rom.00022b80, "ax"
	.global Func_08022b80
	.type Func_08022b80, %function
	.thumb_func
Func_08022b80:
	.incbin "baserom.gba", 0x00022b80, 0x0000002c
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
	.global Func_08022c78
	.type Func_08022c78, %function
	.thumb_func
Func_08022c78:
	.incbin "baserom.gba", 0x00022c78, 0x0000000c
	.section .rom.00022c84, "ax"
	.global Func_08022c84
	.type Func_08022c84, %function
	.thumb_func
Func_08022c84:
	.incbin "baserom.gba", 0x00022c84, 0x00000098
	.section .rom.00022d1c, "ax"
	.global ResourceMetadata_ClearRecord
	.type ResourceMetadata_ClearRecord, %function
	.thumb_func
ResourceMetadata_ClearRecord:
	.incbin "baserom.gba", 0x00022d1c, 0x00000024
	.section .rom.00022d40, "ax"
	.global Func_08022d40
	.type Func_08022d40, %function
	.thumb_func
Func_08022d40:
	.incbin "baserom.gba", 0x00022d40, 0x00000150
	.section .rom.00022e90, "ax"
	.global Func_08022e90
	.type Func_08022e90, %function
	.thumb_func
Func_08022e90:
	.incbin "baserom.gba", 0x00022e90, 0x00000048
	.section .rom.00022f22, "ax"
	.incbin "baserom.gba", 0x00022f22, 0x00000002
	.section .rom.00022f24, "ax"
	.global Func_08022f24
	.type Func_08022f24, %function
	.thumb_func
Func_08022f24:
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
	.incbin "baserom.gba", 0x000231a4, 0x00000024
	.section .rom.000231c8, "ax"
	.global Func_080231c8
	.type Func_080231c8, %function
	.thumb_func
Func_080231c8:
	.incbin "baserom.gba", 0x000231c8, 0x00000058
	.section .rom.00023220, "ax"
	.global Func_08023220
	.type Func_08023220, %function
	.thumb_func
Func_08023220:
	.incbin "baserom.gba", 0x00023220, 0x00000188
	.section .rom.000233a8, "ax"
	.global ObjectDispatch_Initialize
	.type ObjectDispatch_Initialize, %function
	.thumb_func
ObjectDispatch_Initialize:
	.incbin "baserom.gba", 0x000233a8, 0x00000028
	.section .rom.00023510, "ax"
	.global Func_08023510
	.type Func_08023510, %function
	.thumb_func
Func_08023510:
	.incbin "baserom.gba", 0x00023510, 0x00000014
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
	.incbin "baserom.gba", 0x00023680, 0x00000028
	.section .rom.000236a8, "ax"
	.global Animation_SetDisplayFlag
	.type Animation_SetDisplayFlag, %function
	.thumb_func
Animation_SetDisplayFlag:
	.incbin "baserom.gba", 0x000236a8, 0x00000028
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
	.incbin "baserom.gba", 0x0002372c, 0x0000009c
	.section .rom.000237c8, "ax"
	.global Func_080237c8
	.type Func_080237c8, %function
	.thumb_func
Func_080237c8:
	.incbin "baserom.gba", 0x000237c8, 0x00000020
	.section .rom.000237e8, "ax"
	.global Func_080237e8
	.type Func_080237e8, %function
	.thumb_func
Func_080237e8:
	.incbin "baserom.gba", 0x000237e8, 0x00000010
	.section .rom.000237f8, "ax"
	.global Func_080237f8
	.type Func_080237f8, %function
	.thumb_func
Func_080237f8:
	.incbin "baserom.gba", 0x000237f8, 0x00000048
	.section .rom.00023840, "ax"
	.global Func_08023840
	.type Func_08023840, %function
	.thumb_func
Func_08023840:
	.incbin "baserom.gba", 0x00023840, 0x000006d4
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
	.global Func_08026f80
	.type Func_08026f80, %function
	.thumb_func
Func_08026f80:
	.incbin "baserom.gba", 0x00026f80, 0x00000048
	.section .rom.00026fc8, "ax"
	.global Func_08026fc8
	.type Func_08026fc8, %function
	.thumb_func
Func_08026fc8:
	.incbin "baserom.gba", 0x00026fc8, 0x000034e0
	.section .rom.0002a4a8, "ax"
	.global Func_0802a52c
	.type Func_0802a52c, %function
	.thumb_func
Func_0802a52c:
	.incbin "baserom.gba", 0x0002a4a8, 0x00000024
	.section .rom.0002a560, "ax"
	.incbin "baserom.gba", 0x0002a560, 0x0000006c
	.section .rom.0002a5cc, "ax"
	.global Func_0802a650
	.type Func_0802a650, %function
	.thumb_func
Func_0802a650:
	.incbin "baserom.gba", 0x0002a5cc, 0x00000068
	.section .rom.0002a634, "ax"
	.global Func_0802a6b8
	.type Func_0802a6b8, %function
	.thumb_func
Func_0802a6b8:
	.incbin "baserom.gba", 0x0002a634, 0x000003bc
	.section .rom.0002a9f0, "ax"
	.global Func_0802aa74
	.type Func_0802aa74, %function
	.thumb_func
Func_0802aa74:
	.incbin "baserom.gba", 0x0002a9f0, 0x00000528
	.section .rom.0002af18, "ax"
	.global Func_0802af9c
	.type Func_0802af9c, %function
	.thumb_func
Func_0802af9c:
	.incbin "baserom.gba", 0x0002af18, 0x00000204
	.section .rom.0002b11c, "ax"
	.global Func_0802b1a0
	.type Func_0802b1a0, %function
	.thumb_func
Func_0802b1a0:
	.incbin "baserom.gba", 0x0002b11c, 0x00000134
	.section .rom.0002b250, "ax"
	.global Func_0802b2d4
	.type Func_0802b2d4, %function
	.thumb_func
Func_0802b2d4:
	.incbin "baserom.gba", 0x0002b250, 0x00000070
	.section .rom.0002b2c0, "ax"
	.global Func_0802b344
	.type Func_0802b344, %function
	.thumb_func
Func_0802b344:
	.incbin "baserom.gba", 0x0002b2c0, 0x00000048
	.section .rom.0002b308, "ax"
	.global Func_0802b38c
	.type Func_0802b38c, %function
	.thumb_func
Func_0802b38c:
	.incbin "baserom.gba", 0x0002b308, 0x000000c4
	.section .rom.0002b3cc, "ax"
	.global Func_0802b450
	.type Func_0802b450, %function
	.thumb_func
Func_0802b450:
	.incbin "baserom.gba", 0x0002b3cc, 0x00000140
	.section .rom.0002b50c, "ax"
	.global Func_0802b590
	.type Func_0802b590, %function
	.thumb_func
Func_0802b590:
	.incbin "baserom.gba", 0x0002b50c, 0x00000090
	.section .rom.0002b59c, "ax"
	.global Func_0802b620
	.type Func_0802b620, %function
	.thumb_func
Func_0802b620:
	.incbin "baserom.gba", 0x0002b59c, 0x0000001c
	.section .rom.0002b5b8, "ax"
	.global Func_0802b63c
	.type Func_0802b63c, %function
	.thumb_func
Func_0802b63c:
	.incbin "baserom.gba", 0x0002b5b8, 0x000000c4
	.section .rom.0002b67c, "ax"
	.global Func_0802b700
	.type Func_0802b700, %function
	.thumb_func
Func_0802b700:
	.incbin "baserom.gba", 0x0002b67c, 0x0000001c
	.section .rom.0002b698, "ax"
	.global Func_0802b71c
	.type Func_0802b71c, %function
	.thumb_func
Func_0802b71c:
	.incbin "baserom.gba", 0x0002b698, 0x000000c4
	.section .rom.0002b75c, "ax"
	.global Func_0802b7e0
	.type Func_0802b7e0, %function
	.thumb_func
Func_0802b7e0:
	.incbin "baserom.gba", 0x0002b75c, 0x00000048
	.section .rom.0002b7a4, "ax"
	.global Func_0802b828
	.type Func_0802b828, %function
	.thumb_func
Func_0802b828:
	.incbin "baserom.gba", 0x0002b7a4, 0x00000170
	.section .rom.0002b914, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b914, 0x00000370
	.section .rom.0002bc84, "ax"
	.global Func_0802bd08
	.type Func_0802bd08, %function
	.thumb_func
Func_0802bd08:
	.incbin "baserom.gba", 0x0002bc84, 0x000000c8
	.section .rom.0002bd4c, "ax"
	.global Func_0802bdd0
	.type Func_0802bdd0, %function
	.thumb_func
Func_0802bdd0:
	.incbin "baserom.gba", 0x0002bd4c, 0x0000007c
	.section .rom.0002bdc8, "ax"
	.global Func_0802be4c
	.type Func_0802be4c, %function
	.thumb_func
Func_0802be4c:
	.incbin "baserom.gba", 0x0002bdc8, 0x000003f4
	.section .rom.0002c1bc, "ax"
	.global Func_0802c240
	.type Func_0802c240, %function
	.thumb_func
Func_0802c240:
	.incbin "baserom.gba", 0x0002c1bc, 0x00000298
	.section .rom.0002c454, "ax"
	.global Func_0802c4d8
	.type Func_0802c4d8, %function
	.thumb_func
Func_0802c4d8:
	.incbin "baserom.gba", 0x0002c454, 0x000003c8
	.section .rom.0002c81c, "ax"
	.global Func_0802c8a0
	.type Func_0802c8a0, %function
	.thumb_func
Func_0802c8a0:
	.incbin "baserom.gba", 0x0002c81c, 0x00000128
	.section .rom.0002c944, "ax"
	.global Func_0802c9c8
	.type Func_0802c9c8, %function
	.thumb_func
Func_0802c9c8:
	.incbin "baserom.gba", 0x0002c944, 0x00000140
	.section .rom.0002ca84, "ax"
	.global Func_0802cb08
	.type Func_0802cb08, %function
	.thumb_func
Func_0802cb08:
	.incbin "baserom.gba", 0x0002ca84, 0x0000016c
	.section .rom.0002cbf0, "ax"
	.global Func_0802cc74
	.type Func_0802cc74, %function
	.thumb_func
Func_0802cc74:
	.incbin "baserom.gba", 0x0002cbf0, 0x00000014
	.section .rom.0002cc04, "ax"
	.global Func_0802cc88
	.type Func_0802cc88, %function
	.thumb_func
Func_0802cc88:
	.incbin "baserom.gba", 0x0002cc04, 0x000000c4
	.section .rom.0002ccc8, "ax"
	.global Func_0802cd4c
	.type Func_0802cd4c, %function
	.thumb_func
Func_0802cd4c:
	.incbin "baserom.gba", 0x0002ccc8, 0x00000024
	.section .rom.0002ccec, "ax"
	.global Func_0802cd70
	.type Func_0802cd70, %function
	.thumb_func
Func_0802cd70:
	.incbin "baserom.gba", 0x0002ccec, 0x000000dc
	.section .rom.0002cdc8, "ax"
	.global Func_0802ce4c
	.type Func_0802ce4c, %function
	.thumb_func
Func_0802ce4c:
	.incbin "baserom.gba", 0x0002cdc8, 0x00000054
	.section .rom.0002ce1c, "ax"
	.global Func_0802cea0
	.type Func_0802cea0, %function
	.thumb_func
Func_0802cea0:
	.incbin "baserom.gba", 0x0002ce1c, 0x00000010
	.section .rom.0002ce2c, "ax"
	.global Func_0802ceb0
	.type Func_0802ceb0, %function
	.thumb_func
Func_0802ceb0:
	.incbin "baserom.gba", 0x0002ce2c, 0x00000010
	.section .rom.0002ce8c, "ax"
	.incbin "baserom.gba", 0x0002ce8c, 0x0000032c
	.section .rom.0002d22c, "ax"
	.incbin "baserom.gba", 0x0002d22c, 0x00000074
	.section .rom.0002d2ea, "ax"
	.incbin "baserom.gba", 0x0002d2ea, 0x00000026
	.section .rom.0002d340, "ax"
	.incbin "baserom.gba", 0x0002d340, 0x00000028
	.section .rom.0002d37c, "ax"
	.incbin "baserom.gba", 0x0002d37c, 0x0000005c
	.section .rom.0002d3d8, "ax"
	.global Func_0802d45c
	.type Func_0802d45c, %function
	.thumb_func
Func_0802d45c:
	.incbin "baserom.gba", 0x0002d3d8, 0x0000007c
	.section .rom.0002d454, "ax"
	.global Func_0802d4d8
	.type Func_0802d4d8, %function
	.thumb_func
Func_0802d4d8:
	.incbin "baserom.gba", 0x0002d454, 0x00000058
	.section .rom.0002d4ac, "ax"
	.global Func_0802d530
	.type Func_0802d530, %function
	.thumb_func
Func_0802d530:
	.incbin "baserom.gba", 0x0002d4ac, 0x000000d0
	.section .rom.0002d57c, "ax"
	.global Func_0802d600
	.type Func_0802d600, %function
	.thumb_func
Func_0802d600:
	.incbin "baserom.gba", 0x0002d57c, 0x00000044
	.section .rom.0002d5c0, "ax"
	.global Func_0802d644
	.type Func_0802d644, %function
	.thumb_func
Func_0802d644:
	.incbin "baserom.gba", 0x0002d5c0, 0x00000014
	.section .rom.0002d5d4, "ax"
	.global Func_0802d658
	.type Func_0802d658, %function
	.thumb_func
Func_0802d658:
	.incbin "baserom.gba", 0x0002d5d4, 0x00000058
	.section .rom.0002d62c, "ax"
	.global Func_0802d6b0
	.type Func_0802d6b0, %function
	.thumb_func
Func_0802d6b0:
	.incbin "baserom.gba", 0x0002d62c, 0x00000038
	.section .rom.0002d664, "ax"
	.global Func_0802d6e8
	.type Func_0802d6e8, %function
	.thumb_func
Func_0802d6e8:
	.incbin "baserom.gba", 0x0002d664, 0x00000034
	.section .rom.0002d698, "ax"
	.global Func_0802d71c
	.type Func_0802d71c, %function
	.thumb_func
Func_0802d71c:
	.incbin "baserom.gba", 0x0002d698, 0x00000094
	.section .rom.0002d72c, "ax"
	.global Func_0802d7b0
	.type Func_0802d7b0, %function
	.thumb_func
Func_0802d7b0:
	.incbin "baserom.gba", 0x0002d72c, 0x000000cc
	.section .rom.0002d7f8, "ax"
	.global Func_0802d87c
	.type Func_0802d87c, %function
	.thumb_func
Func_0802d87c:
	.incbin "baserom.gba", 0x0002d7f8, 0x000001a8
	.section .rom.0002d9a0, "ax"
	.global Func_0802da24
	.type Func_0802da24, %function
	.thumb_func
Func_0802da24:
	.incbin "baserom.gba", 0x0002d9a0, 0x00000064
	.section .rom.0002da04, "ax"
	.global Func_0802da88
	.type Func_0802da88, %function
	.thumb_func
Func_0802da88:
	.incbin "baserom.gba", 0x0002da04, 0x00000038
	.section .rom.0002dae0, "ax"
	.global Func_0802db64
	.type Func_0802db64, %function
	.thumb_func
Func_0802db64:
	.incbin "baserom.gba", 0x0002dae0, 0x00000024
	.section .rom.0002db04, "ax"
	.global Func_0802db88
	.type Func_0802db88, %function
	.thumb_func
Func_0802db88:
	.incbin "baserom.gba", 0x0002db04, 0x000000c0
	.section .rom.0002dbc4, "ax"
	.global Func_0802dc48
	.type Func_0802dc48, %function
	.thumb_func
Func_0802dc48:
	.incbin "baserom.gba", 0x0002dbc4, 0x00000074
	.section .rom.0002dc38, "ax"
	.global Func_0802dcbc
	.type Func_0802dcbc, %function
	.thumb_func
Func_0802dcbc:
	.incbin "baserom.gba", 0x0002dc38, 0x0000001c
	.section .rom.0002dc54, "ax"
	.global Func_0802dcd8
	.type Func_0802dcd8, %function
	.thumb_func
Func_0802dcd8:
	.incbin "baserom.gba", 0x0002dc54, 0x000001b4
	.section .rom.0002de08, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de08, 0x00000dbc
	.global Data_0802ec48
Data_0802ec48:
	.incbin "baserom.gba", 0x0002ebc4, 0x0000027c
	.global Data_0802eec4
Data_0802eec4:
	.incbin "baserom.gba", 0x0002ee40, 0x0000030c
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f14c, 0x00000030
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f17c, 0x000000dc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x0002f258, 0x00008da8
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x00000528
	.section .rom.00038ea4, "ax"
	.incbin "baserom.gba", 0x00038ea4, 0x00000090
	.section .rom.00038f34, "ax"
	.global UiWork_InitializeWithResourceCounters
	.type UiWork_InitializeWithResourceCounters, %function
	.thumb_func
UiWork_InitializeWithResourceCounters:
	.incbin "baserom.gba", 0x00038f34, 0x000000c4
	.section .rom.00038ff8, "ax"
	.global UiWork_Initialize
	.type UiWork_Initialize, %function
	.thumb_func
UiWork_Initialize:
	.incbin "baserom.gba", 0x00038ff8, 0x00000118
	.section .rom.00039110, "ax"
	.global UiWindow_EraseBorderRect
	.type UiWindow_EraseBorderRect, %function
	.thumb_func
UiWindow_EraseBorderRect:
	.incbin "baserom.gba", 0x00039110, 0x00000144
	.section .rom.00039254, "ax"
	.global UiWindow_Create
	.type UiWindow_Create, %function
	.thumb_func
UiWindow_Create:
	.incbin "baserom.gba", 0x00039254, 0x00000114
	.section .rom.0003938e, "ax"
	.incbin "baserom.gba", 0x0003938e, 0x00000002
	.section .rom.00039390, "ax"
	.global UiWork_Finalize
	.type UiWork_Finalize, %function
	.thumb_func
UiWork_Finalize:
	.incbin "baserom.gba", 0x00039390, 0x00000060
	.section .rom.0003940a, "ax"
	.incbin "baserom.gba", 0x0003940a, 0x00000002
	.section .rom.0003940c, "ax"
	.global RenderOutput_RedrawSavedRect
	.type RenderOutput_RedrawSavedRect, %function
	.thumb_func
RenderOutput_RedrawSavedRect:
	.incbin "baserom.gba", 0x0003940c, 0x00000018
	.section .rom.00039446, "ax"
	.incbin "baserom.gba", 0x00039446, 0x00000002
	.section .rom.00039448, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039448, 0x00000094
	.section .rom.00039502, "ax"
	.incbin "baserom.gba", 0x00039502, 0x00000002
	.section .rom.00039504, "ax"
	.global RenderOutput_Release
	.type RenderOutput_Release, %function
	.thumb_func
RenderOutput_Release:
	.incbin "baserom.gba", 0x00039504, 0x000001cc
	.section .rom.000396d0, "ax"
	.global Func_080396dc
	.type Func_080396dc, %function
	.thumb_func
Func_080396dc:
	.incbin "baserom.gba", 0x000396d0, 0x00000948
	.section .rom.0003a018, "ax"
	.global Func_0803a084
	.type Func_0803a084, %function
	.thumb_func
Func_0803a084:
	.incbin "baserom.gba", 0x0003a018, 0x00000334
	.section .rom.0003a34c, "ax"
	.global UiWork_IsComplete
	.type UiWork_IsComplete, %function
	.thumb_func
UiWork_IsComplete:
	.incbin "baserom.gba", 0x0003a34c, 0x0000002c
	.section .rom.0003a378, "ax"
	.global UiWork_IsIdle
	.type UiWork_IsIdle, %function
	.thumb_func
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a378, 0x0000014c
	.section .rom.0003a4c4, "ax"
	.global Func_0803a530
	.type Func_0803a530, %function
	.thumb_func
Func_0803a530:
	.incbin "baserom.gba", 0x0003a4c4, 0x0000001c
	.section .rom.0003a4e0, "ax"
	.global Func_0803a54c
	.type Func_0803a54c, %function
	.thumb_func
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a4e0, 0x00000094
	.section .rom.0003a574, "ax"
	.global Func_0803a5e0
	.type Func_0803a5e0, %function
	.thumb_func
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a574, 0x0000002c
	.section .rom.0003a5a0, "ax"
	.global Func_0803a60c
	.type Func_0803a60c, %function
	.thumb_func
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5a0, 0x0000005c
	.section .rom.0003a5fc, "ax"
	.global UiWork_SetBusyFlags
	.type UiWork_SetBusyFlags, %function
	.thumb_func
UiWork_SetBusyFlags:
	.incbin "baserom.gba", 0x0003a5fc, 0x00000034
	.section .rom.0003a630, "ax"
	.global UiText_OpenMessageWindow
	.type UiText_OpenMessageWindow, %function
	.thumb_func
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a630, 0x00000110
	.section .rom.0003a740, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.type UiText_ShowPositionedMessageAndWait, %function
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a740, 0x00000344
	.section .rom.0003aa84, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa84, 0x000003b4
	.section .rom.0003ae38, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003ae38, 0x00000a3c
	.section .rom.0003b88c, "ax"
	.global UiText_GetResourceDimensions
	.type UiText_GetResourceDimensions, %function
	.thumb_func
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b88c, 0x0000004c
	.section .rom.0003b8d8, "ax"
	.global Func_0803b8cc
	.type Func_0803b8cc, %function
	.thumb_func
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8d8, 0x0000004c
	.section .rom.0003b924, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b924, 0x000007b8
	.section .rom.0003c0dc, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c0dc, 0x00000100
	.section .rom.0003c1dc, "ax"
	.global Func_0803c170
	.type Func_0803c170, %function
	.thumb_func
Func_0803c170:
	.incbin "baserom.gba", 0x0003c1dc, 0x00000028
	.section .rom.0003c204, "ax"
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c204, 0x0000022c
	.section .rom.0003c430, "ax"
	.global UiWindow_SetTilemapEntry
	.type UiWindow_SetTilemapEntry, %function
	.thumb_func
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c430, 0x00000634
	.section .rom.0003ca74, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003ca74, 0x00000064
	.section .rom.0003cad8, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003cad8, 0x000000ec
	.section .rom.0003cbd2, "ax"
	.incbin "baserom.gba", 0x0003cbd2, 0x00000002
	.section .rom.0003cbd4, "ax"
	.global Func_0803cb1c
	.type Func_0803cb1c, %function
	.thumb_func
Func_0803cb1c:
	.incbin "baserom.gba", 0x0003cbd4, 0x0000018c
	.section .rom.0003cd60, "ax"
	.global Func_0803cca8
	.type Func_0803cca8, %function
	.thumb_func
Func_0803cca8:
	.incbin "baserom.gba", 0x0003cd60, 0x00000028
	.section .rom.0003cd88, "ax"
	.global Func_0803ccd0
	.type Func_0803ccd0, %function
	.thumb_func
Func_0803ccd0:
	.incbin "baserom.gba", 0x0003cd88, 0x0000014c
	.section .rom.0003ced4, "ax"
	.global Func_0803ce1c
	.type Func_0803ce1c, %function
	.thumb_func
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ced4, 0x00000048
	.section .rom.0003cf1c, "ax"
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cf1c, 0x0000022c
	.section .rom.0003d148, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d148, 0x000002dc
	.section .rom.0003d424, "ax"
	.global Localization_LookupEntryId
	.type Localization_LookupEntryId, %function
	.thumb_func
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d424, 0x000000d0
	.section .rom.0003d4f4, "ax"
	.global Func_0803d3c0
	.type Func_0803d3c0, %function
	.thumb_func
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d4f4, 0x00000090
	.section .rom.0003d584, "ax"
	.global Func_0803d450
	.type Func_0803d450, %function
	.thumb_func
Func_0803d450:
	.incbin "baserom.gba", 0x0003d584, 0x0000006c
	.section .rom.0003d618, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d618, 0x000000e0
	.section .rom.0003d6f8, "ax"
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d6f8, 0x000000bc
	.section .rom.0003d7b4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d7b4, 0x0000022c
	.section .rom.0003da5e, "ax"
	.incbin "baserom.gba", 0x0003da5e, 0x00000002
	.section .rom.0003da60, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003da60, 0x00000060
	.section .rom.0003dac0, "ax"
	.global Ability_LoadGlyph
	.type Ability_LoadGlyph, %function
	.thumb_func
Ability_LoadGlyph:
	.incbin "baserom.gba", 0x0003dac0, 0x00000030
	.section .rom.0003daf0, "ax"
	.global Func_0803d9bc
	.type Func_0803d9bc, %function
	.thumb_func
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003daf0, 0x000000bc
	.section .rom.0003dbac, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.type Ui_PrepareTransferFromTableEntry, %function
	.thumb_func
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dbac, 0x000001a4
	.section .rom.0003dd50, "ax"
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dd50, 0x00000108
	.section .rom.0003de58, "ax"
	.global Func_0803dd24
	.type Func_0803dd24, %function
	.thumb_func
Func_0803dd24:
	.incbin "baserom.gba", 0x0003de58, 0x00000044
	.section .rom.0003de9c, "ax"
	.global Func_0803dd68
	.type Func_0803dd68, %function
	.thumb_func
Func_0803dd68:
	.incbin "baserom.gba", 0x0003de9c, 0x00000030
	.section .rom.0003decc, "ax"
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003decc, 0x00000110
	.section .rom.0003dfdc, "ax"
	.global Func_0803dea8
	.type Func_0803dea8, %function
	.thumb_func
Func_0803dea8:
	.incbin "baserom.gba", 0x0003dfdc, 0x00000058
	.section .rom.0003e034, "ax"
	.global Func_0803df00
	.type Func_0803df00, %function
	.thumb_func
Func_0803df00:
	.incbin "baserom.gba", 0x0003e034, 0x00000014
	.section .rom.0003e048, "ax"
	.global Resource_ScheduleOwnerReset
	.type Resource_ScheduleOwnerReset, %function
	.thumb_func
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003e048, 0x00000694
	.section .rom.0003e80c, "ax"
	.global Func_0803e6d8
	.type Func_0803e6d8, %function
	.thumb_func
Func_0803e6d8:
	.incbin "baserom.gba", 0x0003e80c, 0x0000009c
	.section .rom.0003e8a8, "ax"
	.global Func_0803e774
	.type Func_0803e774, %function
	.thumb_func
Func_0803e774:
	.incbin "baserom.gba", 0x0003e8a8, 0x00000038
	.section .rom.0003e8e0, "ax"
	.global Func_0803e7ac
	.type Func_0803e7ac, %function
	.thumb_func
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e8e0, 0x00000140
	.section .rom.0003ea20, "ax"
	.global NodeChain_GetNodeAtCount
	.type NodeChain_GetNodeAtCount, %function
	.thumb_func
NodeChain_GetNodeAtCount:
	.incbin "baserom.gba", 0x0003ea20, 0x0000002c
	.section .rom.0003ea4c, "ax"
	.global Func_0803e918
	.type Func_0803e918, %function
	.thumb_func
Func_0803e918:
	.incbin "baserom.gba", 0x0003ea4c, 0x00000080
	.section .rom.0003eacc, "ax"
	.global Func_0803e998
	.type Func_0803e998, %function
	.thumb_func
Func_0803e998:
	.incbin "baserom.gba", 0x0003eacc, 0x0000063c
	.section .rom.0003f138, "ax"
	.incbin "baserom.gba", 0x0003f138, 0x000001d0
	.section .rom.0003f308, "ax"
	.global Resource_LoadByMode
	.type Resource_LoadByMode, %function
	.thumb_func
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f308, 0x00000060
	.section .rom.0003f368, "ax"
	.global Func_0803f234
	.type Func_0803f234, %function
	.thumb_func
Func_0803f234:
	.incbin "baserom.gba", 0x0003f368, 0x000003dc
	.section .rom.0003f744, "ax"
	.global Func_0803f610
	.type Func_0803f610, %function
	.thumb_func
Func_0803f610:
	.incbin "baserom.gba", 0x0003f744, 0x00000004
	.section .rom.0003f748, "ax"
	.global Func_0803f614
	.type Func_0803f614, %function
	.thumb_func
Func_0803f614:
	.incbin "baserom.gba", 0x0003f748, 0x0000000c
	.section .rom.0003f754, "ax"
	.global Func_0803f620
	.type Func_0803f620, %function
	.thumb_func
Func_0803f620:
	.incbin "baserom.gba", 0x0003f754, 0x00000004
	.section .rom.0003f758, "ax"
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f758, 0x00000074
	.section .rom.0003f7cc, "ax"
	.global UiTextResource_SetPosition
	.type UiTextResource_SetPosition, %function
	.thumb_func
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f7cc, 0x00000028
	.section .rom.0003f7f4, "ax"
	.global UiTextResource_Release
	.type UiTextResource_Release, %function
	.thumb_func
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f7f4, 0x000000b8
	.section .rom.0003f8ac, "ax"
	.global Func_0803f778
	.type Func_0803f778, %function
	.thumb_func
Func_0803f778:
	.incbin "baserom.gba", 0x0003f8ac, 0x000000f4
	.section .rom.0003f9a0, "ax"
	.global Func_0803f86c
	.type Func_0803f86c, %function
	.thumb_func
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f9a0, 0x000000d0
	.section .rom.0003fa70, "ax"
	.global Func_0803f93c
	.type Func_0803f93c, %function
	.thumb_func
Func_0803f93c:
	.incbin "baserom.gba", 0x0003fa70, 0x0000002c
	.section .rom.0003fa9c, "ax"
	.global Func_0803f968
	.type Func_0803f968, %function
	.thumb_func
Func_0803f968:
	.incbin "baserom.gba", 0x0003fa9c, 0x00000058
	.section .rom.0003faf4, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003faf4, 0x00000718
	.section .rom.0004020c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004020c, 0x00001aa0
	.section .rom.00041cac, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041cac, 0x000000a4
	.section .rom.00041d50, "ax"
	.global Func_08041c0c
	.type Func_08041c0c, %function
	.thumb_func
Func_08041c0c:
	.incbin "baserom.gba", 0x00041d50, 0x00000048
	.section .rom.00041d98, "ax"
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041d98, 0x0000031c
	.section .rom.000420b4, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x000420b4, 0x00000020
	.section .rom.000420d4, "ax"
	.global Func_08041f90
	.type Func_08041f90, %function
	.thumb_func
Func_08041f90:
	.incbin "baserom.gba", 0x000420d4, 0x00000014
	.section .rom.000420e8, "ax"
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x000420e8, 0x0000006c
	.section .rom.00042154, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042154, 0x00000098
	.section .rom.000421ec, "ax"
	.global UiText_DrawString
	.type UiText_DrawString, %function
	.thumb_func
UiText_DrawString:
	.incbin "baserom.gba", 0x000421ec, 0x00000054
	.section .rom.00042240, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042240, 0x0000008c
	.section .rom.000422cc, "ax"
	.global UiText_DrawStringInWindow
	.type UiText_DrawStringInWindow, %function
	.thumb_func
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000422cc, 0x0000005c
	.section .rom.00042328, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042328, 0x00000030
	.section .rom.00042358, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042358, 0x00000030
	.section .rom.00042388, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x00042388, 0x000000d0
	.section .rom.00042458, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042458, 0x00000088
	.section .rom.00042594, "ax"
	.global Func_08042450
	.type Func_08042450, %function
	.thumb_func
Func_08042450:
	.incbin "baserom.gba", 0x00042594, 0x000000b8
	.section .rom.0004264c, "ax"
	.global Func_08042508
	.type Func_08042508, %function
	.thumb_func
Func_08042508:
	.incbin "baserom.gba", 0x0004264c, 0x00000064
	.section .rom.000426be, "ax"
	.incbin "baserom.gba", 0x000426be, 0x00000002
	.section .rom.000426c0, "ax"
	.global Func_0804257c
	.type Func_0804257c, %function
	.thumb_func
Func_0804257c:
	.incbin "baserom.gba", 0x000426c0, 0x0000000c
	.section .rom.000426cc, "ax"
	.global Func_08042588
	.type Func_08042588, %function
	.thumb_func
Func_08042588:
	.incbin "baserom.gba", 0x000426cc, 0x00000074
	.section .rom.00042774, "ax"
	.incbin "baserom.gba", 0x00042774, 0x00000060
	.section .rom.000427d4, "ax"
	.global Func_08042690
	.type Func_08042690, %function
	.thumb_func
Func_08042690:
	.incbin "baserom.gba", 0x000427d4, 0x000002ec
	.section .rom.00042ac0, "ax"
	.global Func_0804297c
	.type Func_0804297c, %function
	.thumb_func
Func_0804297c:
	.incbin "baserom.gba", 0x00042ac0, 0x00000430
	.section .rom.00042ef0, "ax"
	.global Func_08042dac
	.type Func_08042dac, %function
	.thumb_func
Func_08042dac:
	.incbin "baserom.gba", 0x00042ef0, 0x00000318
	.section .rom.00043208, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043208, 0x00000198
	.section .rom.000433a0, "ax"
	.global Func_0804325c
	.type Func_0804325c, %function
	.thumb_func
Func_0804325c:
	.incbin "baserom.gba", 0x000433a0, 0x00000048
	.section .rom.000433e8, "ax"
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000433e8, 0x00000248
	.section .rom.00043630, "ax"
	.global Func_080434ec
	.type Func_080434ec, %function
	.thumb_func
Func_080434ec:
	.incbin "baserom.gba", 0x00043630, 0x00000048
	.section .rom.00043678, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043678, 0x000000c8
	.section .rom.00043740, "ax"
	.global Func_080435fc
	.type Func_080435fc, %function
	.thumb_func
Func_080435fc:
	.incbin "baserom.gba", 0x00043740, 0x00000094
	.section .rom.000437d4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000437d4, 0x000000e4
	.section .rom.000438b8, "ax"
	.global Func_08043774
	.type Func_08043774, %function
	.thumb_func
Func_08043774:
	.incbin "baserom.gba", 0x000438b8, 0x00000098
	.section .rom.00043950, "ax"
	.global Func_0804380c
	.type Func_0804380c, %function
	.thumb_func
Func_0804380c:
	.incbin "baserom.gba", 0x00043950, 0x000000bc
	.section .rom.00043a0c, "ax"
	.global Func_080438c8
	.type Func_080438c8, %function
	.thumb_func
Func_080438c8:
	.incbin "baserom.gba", 0x00043a0c, 0x000000b0
	.section .rom.00043b08, "ax"
	.incbin "baserom.gba", 0x00043b08, 0x00000228
	.section .rom.00043d74, "ax"
	.incbin "baserom.gba", 0x00043d74, 0x00000894
	.section .rom.00044608, "ax"
	.global Func_08044460
	.type Func_08044460, %function
	.thumb_func
Func_08044460:
	.incbin "baserom.gba", 0x00044608, 0x00000018
	.section .rom.00044620, "ax"
	.global Func_08044478
	.type Func_08044478, %function
	.thumb_func
Func_08044478:
	.incbin "baserom.gba", 0x00044620, 0x00000010
	.section .rom.00044630, "ax"
	.global Func_08044488
	.type Func_08044488, %function
	.thumb_func
Func_08044488:
	.incbin "baserom.gba", 0x00044630, 0x00000010
	.section .rom.00044640, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x00044640, 0x00000610
	.section .rom.00044c50, "ax"
	.global Func_08044a54
	.type Func_08044a54, %function
	.thumb_func
Func_08044a54:
	.incbin "baserom.gba", 0x00044c50, 0x00000004
	.section .rom.00044c54, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044c54, 0x00000140
	.section .rom.00044d94, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044d94, 0x000000e8
	.section .rom.00044e7c, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044e7c, 0x00000148
	.section .rom.00044fc4, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044fc4, 0x00000484
	.section .rom.000454b6, "ax"
	.incbin "baserom.gba", 0x000454b6, 0x00000076
	.section .rom.0004552c, "ax"
	.global Func_08045330
	.type Func_08045330, %function
	.thumb_func
Func_08045330:
	.incbin "baserom.gba", 0x0004552c, 0x000000a0
	.section .rom.000455cc, "ax"
	.global Func_080453d0
	.type Func_080453d0, %function
	.thumb_func
Func_080453d0:
	.incbin "baserom.gba", 0x000455cc, 0x00000094
	.section .rom.00045722, "ax"
	.incbin "baserom.gba", 0x00045722, 0x0000002a
	.section .rom.00045760, "ax"
	.incbin "baserom.gba", 0x00045760, 0x0000004c
	.section .rom.000457d8, "ax"
	.incbin "baserom.gba", 0x000457d8, 0x0000018c
	.section .rom.0004597a, "ax"
	.incbin "baserom.gba", 0x0004597a, 0x00000032
	.section .rom.000459ca, "ax"
	.incbin "baserom.gba", 0x000459ca, 0x0000096a
	.section .rom.00046334, "ax"
	.global Func_08046134
	.type Func_08046134, %function
	.thumb_func
Func_08046134:
	.incbin "baserom.gba", 0x00046334, 0x00000094
	.section .rom.000463c8, "ax"
	.global Func_080461c8
	.type Func_080461c8, %function
	.thumb_func
Func_080461c8:
	.incbin "baserom.gba", 0x000463c8, 0x00000098
	.section .rom.00046484, "ax"
	.incbin "baserom.gba", 0x00046484, 0x00000150
	.section .rom.00046612, "ax"
	.incbin "baserom.gba", 0x00046612, 0x00003592
	.section .rom.00049bf2, "ax"
	.incbin "baserom.gba", 0x00049bf2, 0x00001e7a
	.section .rom.0004baa6, "ax"
	.incbin "baserom.gba", 0x0004baa6, 0x0000034e
	.section .rom.0004bdf4, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004bdf4, 0x000014dc
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
	.incbin "baserom.gba", 0x0004d764, 0x00000048
	.section .rom.0004d7ac, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7ac, 0x00000250
	.section .rom.0004d9fc, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9fc, 0x0000016c
	.section .rom.0004db68, "ax"
	.global Menu_SelectEntry11To14
	.type Menu_SelectEntry11To14, %function
	.thumb_func
Menu_SelectEntry11To14:
	.incbin "baserom.gba", 0x0004db68, 0x0000003c
	.section .rom.0004dba4, "ax"
	.global Menu_SelectEntry19To1c
	.type Menu_SelectEntry19To1c, %function
	.thumb_func
Menu_SelectEntry19To1c:
	.incbin "baserom.gba", 0x0004dba4, 0x0000003c
	.section .rom.0004dc3c, "ax"
	.global Func_0804da3c
	.type Func_0804da3c, %function
	.thumb_func
Func_0804da3c:
	.incbin "baserom.gba", 0x0004dc3c, 0x00000050
	.section .rom.0004dcba, "ax"
	.incbin "baserom.gba", 0x0004dcba, 0x000000ba
	.section .rom.0004dd74, "ax"
	.global Func_0804db74
	.type Func_0804db74, %function
	.thumb_func
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd74, 0x00000254
	.section .rom.0004e0a0, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0a0, 0x000000c4
	.section .rom.0004e164, "ax"
	.global Menu_HandleFlagGridInput
	.type Menu_HandleFlagGridInput, %function
	.thumb_func
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e164, 0x00000140
	.section .rom.0004e2ce, "ax"
	.incbin "baserom.gba", 0x0004e2ce, 0x00000002
	.section .rom.0004e2d0, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2d0, 0x00000108
	.section .rom.0004e3d8, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3d8, 0x0000021c
	.section .rom.0004e5f4, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e5f4, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed58, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f324, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054c14, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055024, 0x000058a8
	.section .rom.0005cdcc, "ax"
	.incbin "baserom.gba", 0x0005cdcc, 0x00003614
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x000603e0, 0x00000164
	.section .rom.0009d0d0, "ax"
	.incbin "baserom.gba", 0x0009d0d0, 0x00000594
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0009d664, 0x0000a99c
	.section .rom.000a8000, "ax"
	.global Trade_GetOfferStateFar
	.type Trade_GetOfferStateFar, %function
	.thumb_func
Trade_GetOfferStateFar:
	.global Resource_FarCall005
Resource_FarCall005:
	.incbin "baserom.gba", 0x000a8000, 0x00000008
	.section .rom.000a8008, "ax"
	.global Owner_RecalculateStatsFar
	.type Owner_RecalculateStatsFar, %function
	.thumb_func
Owner_RecalculateStatsFar:
	.incbin "baserom.gba", 0x000a8008, 0x00000008
	.section .rom.000a8010, "ax"
	.global Item_Get
	.type Item_Get, %function
	.thumb_func
Item_Get:
	.incbin "baserom.gba", 0x000a8010, 0x00000010
	.section .rom.000a8020, "ax"
	.global Inventory_AddItemFar
	.type Inventory_AddItemFar, %function
	.thumb_func
Inventory_AddItemFar:
	.incbin "baserom.gba", 0x000a8020, 0x00000030
	.section .rom.000a8050, "ax"
	.global Inventory_RemoveFar
	.type Inventory_RemoveFar, %function
	.thumb_func
Inventory_RemoveFar:
	.incbin "baserom.gba", 0x000a8050, 0x00000028
	.section .rom.000a8078, "ax"
	.global BattleAction_Get
	.type BattleAction_Get, %function
	.thumb_func
BattleAction_Get:
	.incbin "baserom.gba", 0x000a8078, 0x00000008
	.section .rom.000a8080, "ax"
	.global OwnerAction_AddFar
	.type OwnerAction_AddFar, %function
	.thumb_func
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000a8080, 0x00000008
	.section .rom.000a8088, "ax"
	.global Equipment_HasValueFar
	.type Equipment_HasValueFar, %function
	.thumb_func
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000a8088, 0x00000070
	.section .rom.000a80f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000a80f8, 0x00000050
	.section .rom.000a8148, "ax"
	.global BattleRandom16Far
	.type BattleRandom16Far, %function
	.thumb_func
BattleRandom16Far:
	.incbin "baserom.gba", 0x000a8148, 0x00000008
	.section .rom.000a8150, "ax"
	.global Djinn_AddToOwnerFar
	.type Djinn_AddToOwnerFar, %function
	.thumb_func
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000a8150, 0x00000008
	.section .rom.000a8158, "ax"
	.global Djinn_ActivateFar
	.type Djinn_ActivateFar, %function
	.thumb_func
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000a8158, 0x00000010
	.section .rom.000a8168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000a8168, 0x00000058
	.section .rom.000a81c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000a81c0, 0x000000c0
	.section .rom.000a8280, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000a8280, 0x000000c8
	.section .rom.000a8348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000a8348, 0x0000007c
	.section .rom.000a83f6, "ax"
	.incbin "baserom.gba", 0x000a83f6, 0x00000002
	.section .rom.000a83f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000a83f8, 0x00001438
	.section .rom.000a9864, "ax"
	.incbin "baserom.gba", 0x000a9864, 0x000001c8
	.section .rom.000a9c62, "ax"
	.incbin "baserom.gba", 0x000a9c62, 0x00000002
	.section .rom.000a9c64, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000a9c64, 0x00000060
	.section .rom.000a9e3c, "ax"
	.incbin "baserom.gba", 0x000a9e3c, 0x00000058
	.section .rom.000a9e94, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000a9e94, 0x00000030
	.section .rom.000a9f30, "ax"
	.incbin "baserom.gba", 0x000a9f30, 0x000000d4
	.section .rom.000aa0de, "ax"
	.incbin "baserom.gba", 0x000aa0de, 0x00000066
	.section .rom.000aa144, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000aa144, 0x00000080
	.section .rom.000aa1f6, "ax"
	.incbin "baserom.gba", 0x000aa1f6, 0x0000009e
	.section .rom.000aa334, "ax"
	.incbin "baserom.gba", 0x000aa334, 0x00000040
	.section .rom.000aa374, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000aa374, 0x00000028
	.section .rom.000aa438, "ax"
	.incbin "baserom.gba", 0x000aa438, 0x000004e0
	.section .rom.000aa918, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000aa918, 0x00000298
	.section .rom.000aabe8, "ax"
	.incbin "baserom.gba", 0x000aabe8, 0x000001d0
	.section .rom.000aadd4, "ax"
	.incbin "baserom.gba", 0x000aadd4, 0x000000a0
	.section .rom.000aaeac, "ax"
	.incbin "baserom.gba", 0x000aaeac, 0x00000024
	.section .rom.000aaf24, "ax"
	.incbin "baserom.gba", 0x000aaf24, 0x00000054
	.section .rom.000aaf90, "ax"
	.incbin "baserom.gba", 0x000aaf90, 0x000002f4
	.section .rom.000ab294, "ax"
	.incbin "baserom.gba", 0x000ab294, 0x000001c8
	.section .rom.000ab4b6, "ax"
	.incbin "baserom.gba", 0x000ab4b6, 0x000005da
	.section .rom.000abb74, "ax"
	.global Djinn_AddToOwner
	.type Djinn_AddToOwner, %function
	.thumb_func
Djinn_AddToOwner:
	.incbin "baserom.gba", 0x000abb74, 0x00000100
	.section .rom.000abc96, "ax"
	.incbin "baserom.gba", 0x000abc96, 0x00000002
	.section .rom.000abc98, "ax"
	.global Djinn_Activate
	.type Djinn_Activate, %function
	.thumb_func
Djinn_Activate:
	.incbin "baserom.gba", 0x000abc98, 0x00000068
	.section .rom.000abd00, "ax"
	.global Djinn_Deactivate
	.type Djinn_Deactivate, %function
	.thumb_func
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000abd00, 0x00000054
	.section .rom.000abd54, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000abd54, 0x000000ac
	.section .rom.000abffe, "ax"
	.incbin "baserom.gba", 0x000abffe, 0x00001336
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000ad334, 0x0000f1a8
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000bc4dc, 0x000000e8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000bc5c4, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000c1b80, 0x00006480
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000c8000, 0x000005d0
	.section .rom.000c85d0, "ax"
	.global Event_ClearInvalidPackedValuesFar
	.type Event_ClearInvalidPackedValuesFar, %function
	.thumb_func
Event_ClearInvalidPackedValuesFar:
	.incbin "baserom.gba", 0x000c85d0, 0x00000fd8
	.section .rom.000c9694, "ax"
	.incbin "baserom.gba", 0x000c9694, 0x0000028c
	.section .rom.000c9934, "ax"
	.incbin "baserom.gba", 0x000c9934, 0x00000db4
	.section .rom.000ca6e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000ca6e8, 0x000005d8
	.section .rom.000cacc0, "ax"
	.global ObjectTable_FindLastActiveId
	.type ObjectTable_FindLastActiveId, %function
	.thumb_func
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000cacc0, 0x000000c4
	.section .rom.000cad84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000cad84, 0x000001ec
	.section .rom.000cafc4, "ax"
	.incbin "baserom.gba", 0x000cafc4, 0x00001d7c
	.section .rom.000ccd6e, "ax"
	.incbin "baserom.gba", 0x000ccd6e, 0x00000eca
	.section .rom.000cdc6a, "ax"
	.incbin "baserom.gba", 0x000cdc6a, 0x00000326
	.section .rom.000cdf90, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000cdf90, 0x00000bd0
	.section .rom.000ceb88, "ax"
	.incbin "baserom.gba", 0x000ceb88, 0x00000298
	.section .rom.000cee60, "ax"
	.incbin "baserom.gba", 0x000cee60, 0x000002fc
	.section .rom.000cf15c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000cf15c, 0x000026a0
	.section .rom.000d181e, "ax"
	.incbin "baserom.gba", 0x000d181e, 0x0000069a
	.section .rom.000d1eb8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000d1eb8, 0x00000368
	.section .rom.000d2220, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000d2220, 0x00000178
	.section .rom.000d23aa, "ax"
	.incbin "baserom.gba", 0x000d23aa, 0x0000000e
	.section .rom.000d2468, "ax"
	.incbin "baserom.gba", 0x000d2468, 0x00000364
	.section .rom.000d27ea, "ax"
	.incbin "baserom.gba", 0x000d27ea, 0x00000202
	.section .rom.000d2a1c, "ax"
	.incbin "baserom.gba", 0x000d2a1c, 0x000001c8
	.section .rom.000d2c44, "ax"
	.incbin "baserom.gba", 0x000d2c44, 0x00000120
	.section .rom.000d2dc4, "ax"
	.incbin "baserom.gba", 0x000d2dc4, 0x00000038
	.section .rom.000d2f28, "ax"
	.incbin "baserom.gba", 0x000d2f28, 0x00000080
	.section .rom.000d3050, "ax"
	.incbin "baserom.gba", 0x000d3050, 0x0000008c
	.section .rom.000d30f6, "ax"
	.incbin "baserom.gba", 0x000d30f6, 0x0000012a
	.section .rom.000d3248, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d3248, 0x00000050
	.section .rom.000d32e0, "ax"
	.global ObjectMotion_SetVariantCallback
	.type ObjectMotion_SetVariantCallback, %function
	.thumb_func
ObjectMotion_SetVariantCallback:
	.incbin "baserom.gba", 0x000d32e0, 0x0000002c
	.section .rom.000d331c, "ax"
	.incbin "baserom.gba", 0x000d331c, 0x00000260
	.section .rom.000d35de, "ax"
	.incbin "baserom.gba", 0x000d35de, 0x00000002
	.section .rom.000d35e0, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000d35e0, 0x000002d4
	.section .rom.000d38ec, "ax"
	.incbin "baserom.gba", 0x000d38ec, 0x000002ec
	.section .rom.000d3bd8, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000d3bd8, 0x000005f0
	.section .rom.000d41dc, "ax"
	.incbin "baserom.gba", 0x000d41dc, 0x00000188
	.section .rom.000d4364, "ax"
	.global Object_AttachWorkTargetToObject
	.type Object_AttachWorkTargetToObject, %function
	.thumb_func
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000d4364, 0x00000088
	.section .rom.000d43ec, "ax"
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d43ec, 0x00000580
	.section .rom.000d4980, "ax"
	.incbin "baserom.gba", 0x000d4980, 0x0000016c
	.section .rom.000d4aec, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000d4aec, 0x000012d4
	.section .rom.000d5dc0, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000d5dc0, 0x00000070
	.section .rom.000d5e42, "ax"
	.incbin "baserom.gba", 0x000d5e42, 0x00000016
	.section .rom.000d5e58, "ax"
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000d5e58, 0x000000a0
	.section .rom.000d5fb4, "ax"
	.incbin "baserom.gba", 0x000d5fb4, 0x0000151c
	.section .rom.000d7504, "ax"
	.incbin "baserom.gba", 0x000d7504, 0x000011c4
	.section .rom.000d86fc, "ax"
	.incbin "baserom.gba", 0x000d86fc, 0x00002d4c
	.section .rom.000db472, "ax"
	.incbin "baserom.gba", 0x000db472, 0x00000002
	.section .rom.000db474, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000db474, 0x000001b8
	.section .rom.000db62c, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000db62c, 0x000001d8
	.section .rom.000db804, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000db804, 0x0000003c
	.section .rom.000db840, "ax"
	.global FieldEvent_RunTypeHandler
	.type FieldEvent_RunTypeHandler, %function
	.thumb_func
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000db840, 0x00002cf4
	.section .rom.000de560, "ax"
	.incbin "baserom.gba", 0x000de560, 0x00001730
	.section .rom.000dfcb4, "ax"
	.incbin "baserom.gba", 0x000dfcb4, 0x000005f4
	.section .rom.000e02c4, "ax"
	.incbin "baserom.gba", 0x000e02c4, 0x000000a4
	.section .rom.000e037e, "ax"
	.incbin "baserom.gba", 0x000e037e, 0x0000ba5a
	.section .rom.000ebdea, "ax"
	.incbin "baserom.gba", 0x000ebdea, 0x00004f76
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0d60, 0x000000d8
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0e38, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000f16b4, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f31dc, 0x000004b0
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f368c, 0x00004974
	.section .rom.000f80e0, "ax"
	.incbin "baserom.gba", 0x000f80e0, 0x0000003c
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x00000678
	.section .rom.000f8830, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8830, 0x00000390
	.section .rom.000f8c3c, "ax"
	.incbin "baserom.gba", 0x000f8c3c, 0x0000027c
	.section .rom.000f8f14, "ax"
	.incbin "baserom.gba", 0x000f8f14, 0x000001d4
	.section .rom.000f919a, "ax"
	.incbin "baserom.gba", 0x000f919a, 0x0000008a
	.section .rom.000f9224, "ax"
	.global Palette_CopyObjectBankToBackground14
	.type Palette_CopyObjectBankToBackground14, %function
	.thumb_func
Palette_CopyObjectBankToBackground14:
	.incbin "baserom.gba", 0x000f9224, 0x00000108
	.section .rom.000f933e, "ax"
	.incbin "baserom.gba", 0x000f933e, 0x000000ee
	.section .rom.000f942c, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f942c, 0x000017ac
	.section .rom.000fabea, "ax"
	.incbin "baserom.gba", 0x000fabea, 0x000003f2
	.section .rom.000fb03c, "ax"
	.incbin "baserom.gba", 0x000fb03c, 0x00000790
	.section .rom.000fb7cc, "ax"
	.global ItemMenu_DrawItemDetails
	.type ItemMenu_DrawItemDetails, %function
	.thumb_func
ItemMenu_DrawItemDetails:
	.incbin "baserom.gba", 0x000fb7cc, 0x0000000c
	.section .rom.000fb7d8, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x000fb7d8, 0x00000500
	.section .rom.000fbd40, "ax"
	.incbin "baserom.gba", 0x000fbd40, 0x00000d3c
	.section .rom.000fca7c, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000fca7c, 0x0000173c
	.section .rom.000fe1b8, "ax"
	.global Func_080fe184
	.type Func_080fe184, %function
	.thumb_func
Func_080fe184:
	.incbin "baserom.gba", 0x000fe1b8, 0x000000f0
	.section .rom.000fe2a8, "ax"
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x000fe2a8, 0x00002384
	.section .rom.00100668, "ax"
	.incbin "baserom.gba", 0x00100668, 0x0000081c
	.section .rom.00100e84, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00100e84, 0x00003efc
	.section .rom.00104d80, "ax"
	.global Menu_UpdateEntryObjectTransforms
	.type Menu_UpdateEntryObjectTransforms, %function
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	.incbin "baserom.gba", 0x00104d80, 0x00000150
	.section .rom.00104ed0, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x00104ed0, 0x000000e8
	.section .rom.00104fb8, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x00104fb8, 0x000000ac
	.section .rom.00105064, "ax"
	.global Menu_ReleaseEntryObjects
	.type Menu_ReleaseEntryObjects, %function
	.thumb_func
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x00105064, 0x0000002c
	.section .rom.00105090, "ax"
	.global Func_081050b8
	.type Func_081050b8, %function
	.thumb_func
Func_081050b8:
	.incbin "baserom.gba", 0x00105090, 0x000000f0
	.section .rom.00105180, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x00105180, 0x000000c4
	.section .rom.00105244, "ax"
	.global Func_0810526c
	.type Func_0810526c, %function
	.thumb_func
Func_0810526c:
	.incbin "baserom.gba", 0x00105244, 0x00000040
	.section .rom.00105284, "ax"
	.global Func_081052ac
	.type Func_081052ac, %function
	.thumb_func
Func_081052ac:
	.incbin "baserom.gba", 0x00105284, 0x00000054
	.section .rom.001052d8, "ax"
	.global Func_08105300
	.type Func_08105300, %function
	.thumb_func
Func_08105300:
	.incbin "baserom.gba", 0x001052d8, 0x0000002c
	.section .rom.00105304, "ax"
	.global Func_0810532c
	.type Func_0810532c, %function
	.thumb_func
Func_0810532c:
	.incbin "baserom.gba", 0x00105304, 0x00000024
	.section .rom.00105328, "ax"
	.global Func_08105350
	.type Func_08105350, %function
	.thumb_func
Func_08105350:
	.incbin "baserom.gba", 0x00105328, 0x00000230
	.section .rom.001055ce, "ax"
	.incbin "baserom.gba", 0x001055ce, 0x00000386
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00105954, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00105958, 0x0000008e
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x001059e6, 0x0000000d
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x001059f3, 0x0000000d
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00105a00, 0x00000018
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00105a18, 0x000025e8
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00108000, 0x00000088
	.section .rom.001080a8, "ax"
	.incbin "baserom.gba", 0x001080a8, 0x00000438
	.section .rom.001084f4, "ax"
	.incbin "baserom.gba", 0x001084f4, 0x00000640
	.section .rom.00108b70, "ax"
	.incbin "baserom.gba", 0x00108b70, 0x00000f38
	.section .rom.00109ae8, "ax"
	.incbin "baserom.gba", 0x00109ae8, 0x00000d2c
	.section .rom.0010a814, "ax"
	.global Func_0810a804
	.type Func_0810a804, %function
	.thumb_func
Func_0810a804:
	.incbin "baserom.gba", 0x0010a814, 0x00000030
	.section .rom.0010a844, "ax"
	.global Func_0810a834
	.type Func_0810a834, %function
	.thumb_func
Func_0810a834:
	.incbin "baserom.gba", 0x0010a844, 0x00000018
	.section .rom.0010a85c, "ax"
	.global Func_0810a84c
	.type Func_0810a84c, %function
	.thumb_func
Func_0810a84c:
	.incbin "baserom.gba", 0x0010a85c, 0x00000018
	.section .rom.0010a8fc, "ax"
	.incbin "baserom.gba", 0x0010a8fc, 0x0000d704
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000068
	.section .rom.00118080, "ax"
	.incbin "baserom.gba", 0x00118080, 0x00000008
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
	.incbin "baserom.gba", 0x00118f6c, 0x000007c8
	.section .rom.00119734, "ax"
	.global Func_08119734
	.type Func_08119734, %function
	.thumb_func
Func_08119734:
	.incbin "baserom.gba", 0x00119734, 0x000000bc
	.section .rom.001197f0, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x001197f0, 0x00000844
	.section .rom.0011a034, "ax"
	.global BattleParty_PrepareActiveOwners
	.type BattleParty_PrepareActiveOwners, %function
	.thumb_func
BattleParty_PrepareActiveOwners:
	.incbin "baserom.gba", 0x0011a034, 0x00000078
	.section .rom.0011a0ac, "ax"
	.global Func_0811a0b0
	.type Func_0811a0b0, %function
	.thumb_func
Func_0811a0b0:
	.incbin "baserom.gba", 0x0011a0ac, 0x000000d8
	.section .rom.0011a248, "ax"
	.incbin "baserom.gba", 0x0011a248, 0x000000d0
	.section .rom.0011a318, "ax"
	.global BattleParty_ListActorIds
	.type BattleParty_ListActorIds, %function
	.thumb_func
BattleParty_ListActorIds:
	.incbin "baserom.gba", 0x0011a318, 0x00000080
	.section .rom.0011a398, "ax"
	.global Func_0811a39c
	.type Func_0811a39c, %function
	.thumb_func
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a398, 0x000000b0
	.section .rom.0011a47e, "ax"
	.incbin "baserom.gba", 0x0011a47e, 0x00000002
	.section .rom.0011a480, "ax"
	.global Func_0811a484
	.type Func_0811a484, %function
	.thumb_func
Func_0811a484:
	.incbin "baserom.gba", 0x0011a480, 0x0000000c
	.section .rom.0011a4da, "ax"
	.incbin "baserom.gba", 0x0011a4da, 0x00000c8e
	.section .rom.0011b17c, "ax"
	.incbin "baserom.gba", 0x0011b17c, 0x00000524
	.section .rom.0011b756, "ax"
	.incbin "baserom.gba", 0x0011b756, 0x00000002
	.section .rom.0011b758, "ax"
	.global Func_0811b75c
	.type Func_0811b75c, %function
	.thumb_func
Func_0811b75c:
	.incbin "baserom.gba", 0x0011b758, 0x00000278
	.section .rom.0011b9d0, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x0011b9d0, 0x0000000c
	.section .rom.0011b9dc, "ax"
	.global ResetMotionRecordGroup
	.type ResetMotionRecordGroup, %function
	.thumb_func
ResetMotionRecordGroup:
	.incbin "baserom.gba", 0x0011b9dc, 0x0000024c
	.section .rom.0011bc60, "ax"
	.incbin "baserom.gba", 0x0011bc60, 0x000000ec
	.section .rom.0011bd4c, "ax"
	.global Func_0811bd50
	.type Func_0811bd50, %function
	.thumb_func
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd4c, 0x00000060
	.section .rom.0011bdac, "ax"
	.global GetMotionRecord
	.type GetMotionRecord, %function
	.thumb_func
GetMotionRecord:
	.incbin "baserom.gba", 0x0011bdac, 0x0000008c
	.section .rom.0011be38, "ax"
	.global GetBattleObjectSlot
	.type GetBattleObjectSlot, %function
	.thumb_func
GetBattleObjectSlot:
	.incbin "baserom.gba", 0x0011be38, 0x0000002c
	.section .rom.0011bec2, "ax"
	.incbin "baserom.gba", 0x0011bec2, 0x000000da
	.section .rom.0011bfcc, "ax"
	.incbin "baserom.gba", 0x0011bfcc, 0x000000a4
	.section .rom.0011c070, "ax"
	.global Func_0811c074
	.type Func_0811c074, %function
	.thumb_func
Func_0811c074:
	.incbin "baserom.gba", 0x0011c070, 0x000000ac
	.section .rom.0011c11c, "ax"
	.global Func_081280fc
	.type Func_081280fc, %function
	.thumb_func
Func_081280fc:
	.incbin "baserom.gba", 0x0011c11c, 0x000000d8
	.section .rom.0011c20c, "ax"
	.incbin "baserom.gba", 0x0011c20c, 0x000000a4
	.section .rom.0011c2b0, "ax"
	.global Func_0811c2b4
	.type Func_0811c2b4, %function
	.thumb_func
Func_0811c2b4:
	.incbin "baserom.gba", 0x0011c2b0, 0x00000060
	.section .rom.0011c310, "ax"
	.global Func_0811c314
	.type Func_0811c314, %function
	.thumb_func
Func_0811c314:
	.incbin "baserom.gba", 0x0011c310, 0x00000068
	.section .rom.0011c378, "ax"
	.global Func_0812814c
	.type Func_0812814c, %function
	.thumb_func
Func_0812814c:
	.incbin "baserom.gba", 0x0011c378, 0x000002d4
	.section .rom.0011c666, "ax"
	.incbin "baserom.gba", 0x0011c666, 0x00000712
	.section .rom.0011cd78, "ax"
	.global Camera_ConfigureScene
	.type Camera_ConfigureScene, %function
	.thumb_func
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd78, 0x000009a4
	.section .rom.0011d744, "ax"
	.incbin "baserom.gba", 0x0011d744, 0x00000c24
	.section .rom.0011e3a6, "ax"
	.incbin "baserom.gba", 0x0011e3a6, 0x00001b5e
	.section .rom.0011ff04, "ax"
	.global BattlePresentation_WaitForAdvance
	.type BattlePresentation_WaitForAdvance, %function
	.thumb_func
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x0011ff04, 0x00000158
	.section .rom.0012005c, "ax"
	.global Func_08120060
	.type Func_08120060, %function
	.thumb_func
Func_08120060:
	.incbin "baserom.gba", 0x0012005c, 0x00000154
	.section .rom.001201c0, "ax"
	.global BattleEv_DispatchQueued
	.type BattleEv_DispatchQueued, %function
	.thumb_func
BattleEv_DispatchQueued:
	.incbin "baserom.gba", 0x001201c0, 0x000001c4
	.section .rom.001203a4, "ax"
	.incbin "baserom.gba", 0x001203a4, 0x000000ac
	.section .rom.00120450, "ax"
	.global Battle_ResolveTargetAction
	.type Battle_ResolveTargetAction, %function
	.thumb_func
Battle_ResolveTargetAction:
	.incbin "baserom.gba", 0x00120450, 0x0000206c
	.section .rom.001224d4, "ax"
	.incbin "baserom.gba", 0x001224d4, 0x00000774
	.section .rom.00122c48, "ax"
	.global Func_08122c4c
	.type Func_08122c4c, %function
	.thumb_func
Func_08122c4c:
	.incbin "baserom.gba", 0x00122c48, 0x000008e8
	.section .rom.00123570, "ax"
	.global Func_08123574
	.type Func_08123574, %function
	.thumb_func
Func_08123574:
	.incbin "baserom.gba", 0x00123570, 0x0000129c
	.section .rom.00124af4, "ax"
	.incbin "baserom.gba", 0x00124af4, 0x000000b0
	.section .rom.00124cba, "ax"
	.incbin "baserom.gba", 0x00124cba, 0x00000eba
	.section .rom.00125bb4, "ax"
	.incbin "baserom.gba", 0x00125bb4, 0x00000f2c
	.section .rom.00126ae0, "ax"
	.global BattlePres_SetActorRecordMode
	.type BattlePres_SetActorRecordMode, %function
	.thumb_func
BattlePres_SetActorRecordMode:
	.incbin "baserom.gba", 0x00126ae0, 0x00000080
	.section .rom.00126bc8, "ax"
	.incbin "baserom.gba", 0x00126bc8, 0x00000130
	.section .rom.00126cf8, "ax"
	.global BattlePres_SetActorModes
	.type BattlePres_SetActorModes, %function
	.thumb_func
BattlePres_SetActorModes:
	.incbin "baserom.gba", 0x00126cf8, 0x000013a4
	.section .rom.001280b8, "ax"
	.incbin "baserom.gba", 0x001280b8, 0x00000040
	.section .rom.001280f8, "ax"
	.global Func_08128174
	.type Func_08128174, %function
	.thumb_func
Func_08128174:
	.global Summon_IsEntryFlagged
	.type Summon_IsEntryFlagged, %function
	.thumb_func
Summon_IsEntryFlagged:
	.incbin "baserom.gba", 0x001280f8, 0x000000b4
	.section .rom.001281e6, "ax"
	.incbin "baserom.gba", 0x001281e6, 0x0000065a
	.global Data_08128844
Data_08128844:
	.incbin "baserom.gba", 0x00128840, 0x000084c8
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x00130d08, 0x000072f8
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
	.incbin "baserom.gba", 0x00142944, 0x0000a038
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
	.incbin "baserom.gba", 0x0015e320, 0x000206f8
	.section .rom.0017ea5c, "ax"
	.incbin "baserom.gba", 0x0017ea5c, 0x00017994
	.section .rom.001963f0, "ax"
	.global Func_081963ec
	.type Func_081963ec, %function
	.thumb_func
Func_081963ec:
	.incbin "baserom.gba", 0x001963f0, 0x00000018
	.section .rom.00196408, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196408, 0x00000e2c
	.section .rom.001973f4, "ax"
	.incbin "baserom.gba", 0x001973f4, 0x00008c0c
	.global Resource_FarCall00A
Resource_FarCall00A:
	.incbin "baserom.gba", 0x001a0000, 0x00000030
	.section .rom.001a04d0, "ax"
	.incbin "baserom.gba", 0x001a04d0, 0x00000da4
	.section .rom.001a1280, "ax"
	.incbin "baserom.gba", 0x001a1280, 0x00004d80
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
	.incbin "baserom.gba", 0x001b2008, 0x00001d60
	.section .rom.001b3d90, "ax"
	.incbin "baserom.gba", 0x001b3d90, 0x00004270
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
	.section .rom.006322b0, "ax"
	.incbin "baserom.gba", 0x006322b0, 0x0004dd50
	.section .rom.00682000, "ax"
	.global Resource_Data002
Resource_Data002:
	.incbin "baserom.gba", 0x00682000, 0x00000010
	.section .rom.006848d0, "ax"
	.global Resource_Data015
Resource_Data015:
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .rom.0068a9c1, "ax"
	.incbin "baserom.gba", 0x0068a9c1, 0x00000003
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a9c4, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x006930bc, 0x000056b0
	.section .rom.006a5301, "ax"
	.incbin "baserom.gba", 0x006a5301, 0x00000003
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a5304, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a5504, 0x00000798
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a5c9c, 0x00000894
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a6530, 0x00000818
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a6d48, 0x000004dc
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a7224, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a7a4c, 0x0000186c
	.section .rom.006aa7f5, "ax"
	.incbin "baserom.gba", 0x006aa7f5, 0x00000003
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006aa7f8, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b1e14, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006cdda0, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006ce054, 0x00000f0c
	.section .rom.006d194e, "ax"
	.incbin "baserom.gba", 0x006d194e, 0x00000002
	.section .rom.006d5d92, "ax"
	.incbin "baserom.gba", 0x006d5d92, 0x00000002
	.section .rom.006e6502, "ax"
	.incbin "baserom.gba", 0x006e6502, 0x00000002
	.section .rom.006e9af2, "ax"
	.incbin "baserom.gba", 0x006e9af2, 0x00000002
	.section .rom.006f5596, "ax"
	.incbin "baserom.gba", 0x006f5596, 0x00000002
	.section .rom.00705c46, "ax"
	.incbin "baserom.gba", 0x00705c46, 0x00000002
	.section .rom.0070d6e6, "ax"
	.incbin "baserom.gba", 0x0070d6e6, 0x00000002
	.section .rom.00710f26, "ax"
	.incbin "baserom.gba", 0x00710f26, 0x00000002
	.section .rom.0071a35a, "ax"
	.incbin "baserom.gba", 0x0071a35a, 0x00000002
	.section .rom.00721506, "ax"
	.incbin "baserom.gba", 0x00721506, 0x00000002
	.section .rom.007293c2, "ax"
	.incbin "baserom.gba", 0x007293c2, 0x00000002
	.section .rom.0072de52, "ax"
	.incbin "baserom.gba", 0x0072de52, 0x00000002
	.section .rom.00732106, "ax"
	.incbin "baserom.gba", 0x00732106, 0x00000002
	.section .rom.00735a86, "ax"
	.incbin "baserom.gba", 0x00735a86, 0x00000002
	.section .rom.007422c2, "ax"
	.incbin "baserom.gba", 0x007422c2, 0x00000002
	.section .rom.0074da02, "ax"
	.incbin "baserom.gba", 0x0074da02, 0x00000002
	.section .rom.00750dd2, "ax"
	.incbin "baserom.gba", 0x00750dd2, 0x00000002
	.section .rom.00762cee, "ax"
	.incbin "baserom.gba", 0x00762cee, 0x00000002
	.section .rom.00766756, "ax"
	.incbin "baserom.gba", 0x00766756, 0x00000002
	.section .rom.0076a146, "ax"
	.incbin "baserom.gba", 0x0076a146, 0x00000002
	.section .rom.0077a6ae, "ax"
	.incbin "baserom.gba", 0x0077a6ae, 0x00000002
	.section .rom.0077e5de, "ax"
	.incbin "baserom.gba", 0x0077e5de, 0x00000002
	.section .rom.00782002, "ax"
	.incbin "baserom.gba", 0x00782002, 0x00000002
	.section .rom.0078a322, "ax"
	.incbin "baserom.gba", 0x0078a322, 0x00000002
	.section .rom.0079248e, "ax"
	.incbin "baserom.gba", 0x0079248e, 0x00000002
	.section .rom.0079f75a, "ax"
	.incbin "baserom.gba", 0x0079f75a, 0x00000002
	.section .rom.007a3616, "ax"
	.incbin "baserom.gba", 0x007a3616, 0x00000002
	.section .rom.007a7412, "ax"
	.incbin "baserom.gba", 0x007a7412, 0x00000002
	.section .rom.007ab8e2, "ax"
	.incbin "baserom.gba", 0x007ab8e2, 0x00000002
	.section .rom.007b32be, "ax"
	.incbin "baserom.gba", 0x007b32be, 0x00000002
	.section .rom.007b78ea, "ax"
	.incbin "baserom.gba", 0x007b78ea, 0x00000002
	.section .rom.007bf566, "ax"
	.incbin "baserom.gba", 0x007bf566, 0x00000002
	.section .rom.007c3162, "ax"
	.incbin "baserom.gba", 0x007c3162, 0x00000002
	.section .rom.007c6b12, "ax"
	.incbin "baserom.gba", 0x007c6b12, 0x00000002
	.section .rom.007caf62, "ax"
	.incbin "baserom.gba", 0x007caf62, 0x00000002
	.section .rom.007ceb5a, "ax"
	.incbin "baserom.gba", 0x007ceb5a, 0x00000002
	.section .rom.007da7ba, "ax"
	.incbin "baserom.gba", 0x007da7ba, 0x00000002
	.section .rom.007de40a, "ax"
	.incbin "baserom.gba", 0x007de40a, 0x00000002
	.section .rom.007e698a, "ax"
	.incbin "baserom.gba", 0x007e698a, 0x00000002
	.section .rom.007f2fda, "ax"
	.incbin "baserom.gba", 0x007f2fda, 0x00000002
	.section .rom.007f9a8e, "ax"
	.incbin "baserom.gba", 0x007f9a8e, 0x00000002
	.section .rom.00802d56, "ax"
	.incbin "baserom.gba", 0x00802d56, 0x00000002
	.section .rom.0080a77a, "ax"
	.incbin "baserom.gba", 0x0080a77a, 0x00000002
	.section .rom.008203fa, "ax"
	.incbin "baserom.gba", 0x008203fa, 0x00000002
	.section .rom.0082983e, "ax"
	.incbin "baserom.gba", 0x0082983e, 0x00000002
	.section .rom.00832a5a, "ax"
	.incbin "baserom.gba", 0x00832a5a, 0x00000002
	.section .rom.00840382, "ax"
	.incbin "baserom.gba", 0x00840382, 0x00000002
	.section .rom.008461d6, "ax"
	.incbin "baserom.gba", 0x008461d6, 0x00000002
	.section .rom.0084beaa, "ax"
	.incbin "baserom.gba", 0x0084beaa, 0x00000002
	.section .rom.0084c62d, "ax"
	.incbin "baserom.gba", 0x0084c62d, 0x00000003
	.section .rom.0084dfcf, "ax"
	.incbin "baserom.gba", 0x0084dfcf, 0x00000001
	.section .rom.00850ded, "ax"
	.incbin "baserom.gba", 0x00850ded, 0x00000003
	.section .rom.00854d02, "ax"
	.incbin "baserom.gba", 0x00854d02, 0x00000002
	.section .rom.00857c10, "ax"
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x00857c10, 0x000009bc
	.section .rom.00858a21, "ax"
	.incbin "baserom.gba", 0x00858a21, 0x00000003
	.section .rom.008590e1, "ax"
	.incbin "baserom.gba", 0x008590e1, 0x00000003
	.section .rom.0085931f, "ax"
	.incbin "baserom.gba", 0x0085931f, 0x00000001
	.section .rom.0085974f, "ax"
	.incbin "baserom.gba", 0x0085974f, 0x00000001
	.section .rom.00859a55, "ax"
	.incbin "baserom.gba", 0x00859a55, 0x00000003
	.section .rom.00859dc5, "ax"
	.incbin "baserom.gba", 0x00859dc5, 0x00000003
	.section .rom.0085a983, "ax"
	.incbin "baserom.gba", 0x0085a983, 0x00000001
	.section .rom.0085aa61, "ax"
	.incbin "baserom.gba", 0x0085aa61, 0x00000003
	.section .rom.0085af09, "ax"
	.incbin "baserom.gba", 0x0085af09, 0x00000003
	.section .rom.0085b233, "ax"
	.incbin "baserom.gba", 0x0085b233, 0x00000001
	.section .rom.0085b615, "ax"
	.incbin "baserom.gba", 0x0085b615, 0x00000003
	.section .rom.0085ba1a, "ax"
	.incbin "baserom.gba", 0x0085ba1a, 0x00000002
	.section .rom.0085bc8e, "ax"
	.incbin "baserom.gba", 0x0085bc8e, 0x00000002
	.section .rom.0085e32f, "ax"
	.incbin "baserom.gba", 0x0085e32f, 0x00000001
	.section .rom.0085eaaa, "ax"
	.incbin "baserom.gba", 0x0085eaaa, 0x00000002
	.section .rom.008600ef, "ax"
	.incbin "baserom.gba", 0x008600ef, 0x00000001
	.section .rom.00860619, "ax"
	.incbin "baserom.gba", 0x00860619, 0x00000003
	.section .rom.00860e22, "ax"
	.incbin "baserom.gba", 0x00860e22, 0x00000002
	.section .rom.0086139b, "ax"
	.incbin "baserom.gba", 0x0086139b, 0x00000001
	.section .rom.00862ada, "ax"
	.incbin "baserom.gba", 0x00862ada, 0x00000002
	.section .rom.008659a3, "ax"
	.incbin "baserom.gba", 0x008659a3, 0x00000001
	.section .rom.00865c55, "ax"
	.incbin "baserom.gba", 0x00865c55, 0x00000003
	.section .rom.00867037, "ax"
	.incbin "baserom.gba", 0x00867037, 0x00000001
	.section .rom.0086b873, "ax"
	.incbin "baserom.gba", 0x0086b873, 0x00000001
	.section .rom.0086f019, "ax"
	.incbin "baserom.gba", 0x0086f019, 0x00000003
	.section .rom.00870d3e, "ax"
	.incbin "baserom.gba", 0x00870d3e, 0x00000002
	.section .rom.00872c3d, "ax"
	.incbin "baserom.gba", 0x00872c3d, 0x00000003
	.section .rom.008745eb, "ax"
	.incbin "baserom.gba", 0x008745eb, 0x00000001
	.section .rom.00875b63, "ax"
	.incbin "baserom.gba", 0x00875b63, 0x00000001
	.section .rom.00879796, "ax"
	.incbin "baserom.gba", 0x00879796, 0x00000002
	.section .rom.0087a2f4, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a2f4, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087bf44, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c384, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c5d0, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c768, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087cf94, 0x00000e98
	.section .rom.0087e463, "ax"
	.incbin "baserom.gba", 0x0087e463, 0x00000001
	.section .rom.008805cd, "ax"
	.incbin "baserom.gba", 0x008805cd, 0x00000003
	.section .rom.008806d8, "ax"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x008806d8, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x00880924, 0x00000184
	.section .rom.00881361, "ax"
	.incbin "baserom.gba", 0x00881361, 0x00000003
	.section .rom.00882f45, "ax"
	.incbin "baserom.gba", 0x00882f45, 0x00000003
	.section .rom.00884aa2, "ax"
	.incbin "baserom.gba", 0x00884aa2, 0x00000002
	.section .rom.00884ee1, "ax"
	.incbin "baserom.gba", 0x00884ee1, 0x00000003
	.section .rom.008852f6, "ax"
	.incbin "baserom.gba", 0x008852f6, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x008852f8, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x00885630, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x008866fc, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x008869e4, 0x00000154
	.section .rom.00889043, "ax"
	.incbin "baserom.gba", 0x00889043, 0x00000001
	.section .rom.008899ff, "ax"
	.incbin "baserom.gba", 0x008899ff, 0x00000001
	.section .rom.0088ab42, "ax"
	.incbin "baserom.gba", 0x0088ab42, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088ab44, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b7c0, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b7ec, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b818, 0x000007c0
	.section .rom.0088d553, "ax"
	.incbin "baserom.gba", 0x0088d553, 0x00000001
	.section .rom.0088d87d, "ax"
	.incbin "baserom.gba", 0x0088d87d, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d880, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088dda8, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e2d8, 0x000005f0
	.section .rom.0088f051, "ax"
	.incbin "baserom.gba", 0x0088f051, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088f054, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f254, 0x00000420
	.section .rom.0088fc9d, "ax"
	.incbin "baserom.gba", 0x0088fc9d, 0x00000003
	.section .rom.008900a7, "ax"
	.incbin "baserom.gba", 0x008900a7, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x008900a8, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x00890304, 0x0000057c
	.section .rom.00890b29, "ax"
	.incbin "baserom.gba", 0x00890b29, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890b2c, 0x0000095c
	.section .rom.00891b45, "ax"
	.incbin "baserom.gba", 0x00891b45, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891b48, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x008945f8, 0x000011cc
	.section .rom.00895e23, "ax"
	.incbin "baserom.gba", 0x00895e23, 0x00000001
	.section .rom.00896e5d, "ax"
	.incbin "baserom.gba", 0x00896e5d, 0x00000003
	.section .rom.008974b3, "ax"
	.incbin "baserom.gba", 0x008974b3, 0x00000001
	.section .rom.00897b31, "ax"
	.incbin "baserom.gba", 0x00897b31, 0x00000003
	.section .rom.00898151, "ax"
	.incbin "baserom.gba", 0x00898151, 0x00000003
	.section .rom.0089951e, "ax"
	.incbin "baserom.gba", 0x0089951e, 0x00000002
	.section .rom.0089a562, "ax"
	.incbin "baserom.gba", 0x0089a562, 0x00000002
	.section .rom.0089afeb, "ax"
	.incbin "baserom.gba", 0x0089afeb, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089afec, 0x000002cc
	.section .rom.0089bff1, "ax"
	.incbin "baserom.gba", 0x0089bff1, 0x00000003
	.section .rom.0089d22d, "ax"
	.incbin "baserom.gba", 0x0089d22d, 0x00000003
	.section .rom.0089db77, "ax"
	.incbin "baserom.gba", 0x0089db77, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089db78, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e1d4, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e700, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a09bc, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a2150, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2834, 0x00001f4c
	.section .rom.008a4cb4, "ax"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4cb4, 0x000011d8
	.section .rom.008a637a, "ax"
	.incbin "baserom.gba", 0x008a637a, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a637c, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a69c4, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a75e8, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a79ac, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7b74, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a80c0, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a840c, 0x0000076c
	.section .rom.008a91bb, "ax"
	.incbin "baserom.gba", 0x008a91bb, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a91bc, 0x000000a8
	.section .rom.008a957a, "ax"
	.incbin "baserom.gba", 0x008a957a, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a957c, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9f24, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa21c, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aad4c, 0x00000100
	.section .rom.008ababd, "ax"
	.incbin "baserom.gba", 0x008ababd, 0x00000003
	.section .rom.008ac305, "ax"
	.incbin "baserom.gba", 0x008ac305, 0x00000003
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac308, 0x00001200
	.section .rom.008ae3c7, "ax"
	.incbin "baserom.gba", 0x008ae3c7, 0x00000001
	.section .rom.008aeea3, "ax"
	.incbin "baserom.gba", 0x008aeea3, 0x00000001
	.section .rom.008af566, "ax"
	.incbin "baserom.gba", 0x008af566, 0x00000002
	.section .rom.008af84d, "ax"
	.incbin "baserom.gba", 0x008af84d, 0x00000003
	.section .rom.008b0bb3, "ax"
	.incbin "baserom.gba", 0x008b0bb3, 0x00000001
	.section .rom.008b0f9d, "ax"
	.incbin "baserom.gba", 0x008b0f9d, 0x00000003
	.section .rom.008b136f, "ax"
	.incbin "baserom.gba", 0x008b136f, 0x00000001
	.section .rom.008b1c0f, "ax"
	.incbin "baserom.gba", 0x008b1c0f, 0x00000001
	.section .rom.008b20b2, "ax"
	.incbin "baserom.gba", 0x008b20b2, 0x00000002
	.section .rom.008b326b, "ax"
	.incbin "baserom.gba", 0x008b326b, 0x00000001
	.section .rom.008b36c4, "ax"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b36c4, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b4730, 0x00000fc8
	.section .rom.008b5952, "ax"
	.incbin "baserom.gba", 0x008b5952, 0x00000002
	.section .rom.008b73ad, "ax"
	.incbin "baserom.gba", 0x008b73ad, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b73b0, 0x000001b4
	.section .rom.008b78f9, "ax"
	.incbin "baserom.gba", 0x008b78f9, 0x00000003
	.section .rom.008b9682, "ax"
	.incbin "baserom.gba", 0x008b9682, 0x00000002
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9684, 0x00000278
	.section .rom.008b9dc2, "ax"
	.incbin "baserom.gba", 0x008b9dc2, 0x00000002
	.section .rom.008bb997, "ax"
	.incbin "baserom.gba", 0x008bb997, 0x00000001
	.section .rom.008bd595, "ax"
	.incbin "baserom.gba", 0x008bd595, 0x00000003
	.section .rom.008bd7b6, "ax"
	.incbin "baserom.gba", 0x008bd7b6, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd7b8, 0x0000043c
	.section .rom.008bdd05, "ax"
	.incbin "baserom.gba", 0x008bdd05, 0x00000003
	.section .rom.008be2e7, "ax"
	.incbin "baserom.gba", 0x008be2e7, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be2e8, 0x000004a0
	.section .rom.008bf49a, "ax"
	.incbin "baserom.gba", 0x008bf49a, 0x00000002
	.section .rom.008bf9dc, "ax"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bf9dc, 0x00000840
	.section .rom.008c05ab, "ax"
	.incbin "baserom.gba", 0x008c05ab, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c05ac, 0x00001258
	.section .rom.008c22f2, "ax"
	.incbin "baserom.gba", 0x008c22f2, 0x00000002
	.section .rom.008c30c9, "ax"
	.incbin "baserom.gba", 0x008c30c9, 0x00000003
	.section .rom.008c38f6, "ax"
	.incbin "baserom.gba", 0x008c38f6, 0x00000002
	.section .rom.008c3ce8, "ax"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c3ce8, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c3ea4, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c4060, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c49a0, 0x00000418
	.section .rom.008c5729, "ax"
	.incbin "baserom.gba", 0x008c5729, 0x00000003
	.section .rom.008c5ad7, "ax"
	.incbin "baserom.gba", 0x008c5ad7, 0x00000001
	.section .rom.008c7a7b, "ax"
	.incbin "baserom.gba", 0x008c7a7b, 0x00000001
	.section .rom.008c8817, "ax"
	.incbin "baserom.gba", 0x008c8817, 0x00000001
	.section .rom.008c8a33, "ax"
	.incbin "baserom.gba", 0x008c8a33, 0x00000001
	.section .rom.008c8d2f, "ax"
	.incbin "baserom.gba", 0x008c8d2f, 0x00000001
	.section .rom.008c90d0, "ax"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c90d0, 0x000016b0
	.section .rom.008caedd, "ax"
	.incbin "baserom.gba", 0x008caedd, 0x00000003
	.section .rom.008cbcab, "ax"
	.incbin "baserom.gba", 0x008cbcab, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbcac, 0x00000c74
	.section .rom.008ccf15, "ax"
	.incbin "baserom.gba", 0x008ccf15, 0x00000003
	.section .rom.008cd46f, "ax"
	.incbin "baserom.gba", 0x008cd46f, 0x00000001
	.section .rom.008ced59, "ax"
	.incbin "baserom.gba", 0x008ced59, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008ced5c, 0x000008a0
	.section .rom.008cf9d7, "ax"
	.incbin "baserom.gba", 0x008cf9d7, 0x00000001
	.section .rom.008cfc61, "ax"
	.incbin "baserom.gba", 0x008cfc61, 0x00000003
	.section .rom.008d0007, "ax"
	.incbin "baserom.gba", 0x008d0007, 0x00000001
	.section .rom.008d0262, "ax"
	.incbin "baserom.gba", 0x008d0262, 0x00000002
	.section .rom.008d061a, "ax"
	.incbin "baserom.gba", 0x008d061a, 0x00000002
	.section .rom.008d1b03, "ax"
	.incbin "baserom.gba", 0x008d1b03, 0x00000001
	.section .rom.008d30c6, "ax"
	.incbin "baserom.gba", 0x008d30c6, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d30c8, 0x00000a6c
	.section .rom.008d490a, "ax"
	.incbin "baserom.gba", 0x008d490a, 0x00000002
	.section .rom.008d4c8d, "ax"
	.incbin "baserom.gba", 0x008d4c8d, 0x00000003
	.section .rom.008d593d, "ax"
	.incbin "baserom.gba", 0x008d593d, 0x00000003
	.section .rom.008d6485, "ax"
	.incbin "baserom.gba", 0x008d6485, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6488, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d6620, 0x0000088c
	.section .rom.008d7a7a, "ax"
	.incbin "baserom.gba", 0x008d7a7a, 0x00000002
	.section .rom.008d7f92, "ax"
	.incbin "baserom.gba", 0x008d7f92, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d7f94, 0x00000624
	.section .rom.008d89d9, "ax"
	.incbin "baserom.gba", 0x008d89d9, 0x00000003
	.section .rom.008d8c73, "ax"
	.incbin "baserom.gba", 0x008d8c73, 0x00000001
	.section .rom.008da174, "ax"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da174, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da610, 0x0000198c
	.section .rom.008dc386, "ax"
	.incbin "baserom.gba", 0x008dc386, 0x00000002
	.section .rom.008de2fb, "ax"
	.incbin "baserom.gba", 0x008de2fb, 0x00000001
	.section .rom.008de7cb, "ax"
	.incbin "baserom.gba", 0x008de7cb, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de7cc, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008dee60, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df894, 0x00000bfc
	.section .rom.008e0c65, "ax"
	.incbin "baserom.gba", 0x008e0c65, 0x00000003
	.section .rom.008e1736, "ax"
	.incbin "baserom.gba", 0x008e1736, 0x00000002
	.section .rom.008e2273, "ax"
	.incbin "baserom.gba", 0x008e2273, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2274, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e28b4, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3e3c, 0x00000064
	.section .rom.008e420b, "ax"
	.incbin "baserom.gba", 0x008e420b, 0x00000001
	.section .rom.008e47a9, "ax"
	.incbin "baserom.gba", 0x008e47a9, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e47ac, 0x00000204
	.section .rom.008e4ce9, "ax"
	.incbin "baserom.gba", 0x008e4ce9, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4cec, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5d04, 0x0000166c
	.section .rom.008e930e, "ax"
	.incbin "baserom.gba", 0x008e930e, 0x00000002
	.section .rom.008e9ab1, "ax"
	.incbin "baserom.gba", 0x008e9ab1, 0x00000003
	.section .rom.008eaac8, "ax"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eaac8, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eae44, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb274, 0x000010cc
	.section .rom.008ec3c4, "ax"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec3c4, 0x000004e8
	.section .rom.008ec9b4, "ax"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ec9b4, 0x000006b8
	.section .rom.008ed97e, "ax"
	.incbin "baserom.gba", 0x008ed97e, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008ed980, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef4b4, 0x00001050
	.section .rom.008f15a9, "ax"
	.incbin "baserom.gba", 0x008f15a9, 0x00000003
	.section .rom.008f17a8, "ax"
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f17a8, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00933010, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c3d4, 0x00000028
	.section .rom.0093c5e9, "ax"
	.incbin "baserom.gba", 0x0093c5e9, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c5ec, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c740, 0x000004b8
	.section .rom.0093e362, "ax"
	.incbin "baserom.gba", 0x0093e362, 0x00000002
	.section .rom.0093f899, "ax"
	.incbin "baserom.gba", 0x0093f899, 0x00000003
	.section .rom.009416ef, "ax"
	.incbin "baserom.gba", 0x009416ef, 0x00000001
	.section .rom.00943803, "ax"
	.incbin "baserom.gba", 0x00943803, 0x00000001
	.section .rom.009439dc, "ax"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x009439dc, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943bc0, 0x000003c0
	.section .rom.009465fb, "ax"
	.incbin "baserom.gba", 0x009465fb, 0x00000001
	.section .rom.00947003, "ax"
	.incbin "baserom.gba", 0x00947003, 0x00000001
	.section .rom.00949d22, "ax"
	.incbin "baserom.gba", 0x00949d22, 0x00000002
	.section .rom.00949ef4, "ax"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00949ef4, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00949efc, 0x00000460
	.section .rom.0094b94e, "ax"
	.incbin "baserom.gba", 0x0094b94e, 0x00000002
	.section .rom.0094cdbe, "ax"
	.incbin "baserom.gba", 0x0094cdbe, 0x00000002
	.section .rom.0094d6b2, "ax"
	.incbin "baserom.gba", 0x0094d6b2, 0x00000002
	.section .rom.0094dfd9, "ax"
	.incbin "baserom.gba", 0x0094dfd9, 0x00000003
	.section .rom.0094eeca, "ax"
	.incbin "baserom.gba", 0x0094eeca, 0x00000002
	.section .rom.0094fab7, "ax"
	.incbin "baserom.gba", 0x0094fab7, 0x00000001
	.section .rom.0094fc92, "ax"
	.incbin "baserom.gba", 0x0094fc92, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fc94, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094fe84, 0x000002d8
	.section .rom.009509f2, "ax"
	.incbin "baserom.gba", 0x009509f2, 0x00000002
	.section .rom.00952e2d, "ax"
	.incbin "baserom.gba", 0x00952e2d, 0x00000003
	.section .rom.009533ff, "ax"
	.incbin "baserom.gba", 0x009533ff, 0x00000001
	.section .rom.0095389f, "ax"
	.incbin "baserom.gba", 0x0095389f, 0x00000001
	.section .rom.00953bf2, "ax"
	.incbin "baserom.gba", 0x00953bf2, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953bf4, 0x000002f8
	.section .rom.00953fc5, "ax"
	.incbin "baserom.gba", 0x00953fc5, 0x00000003
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00953fc8, 0x000002f0
	.section .rom.0095434a, "ax"
	.incbin "baserom.gba", 0x0095434a, 0x00000002
	.section .rom.00954efd, "ax"
	.incbin "baserom.gba", 0x00954efd, 0x00000003
	.section .rom.00955baa, "ax"
	.incbin "baserom.gba", 0x00955baa, 0x00000002
	.section .rom.009566ae, "ax"
	.incbin "baserom.gba", 0x009566ae, 0x00000002
	.section .rom.009575ed, "ax"
	.incbin "baserom.gba", 0x009575ed, 0x00000003
	.section .rom.00957e29, "ax"
	.incbin "baserom.gba", 0x00957e29, 0x00000003
	.section .rom.009592ce, "ax"
	.incbin "baserom.gba", 0x009592ce, 0x00000002
	.section .rom.00959d86, "ax"
	.incbin "baserom.gba", 0x00959d86, 0x00000002
	.section .rom.0095a0b1, "ax"
	.incbin "baserom.gba", 0x0095a0b1, 0x00000003
	.section .rom.0095afb6, "ax"
	.incbin "baserom.gba", 0x0095afb6, 0x00000002
	.section .rom.0095b7b8, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b7b8, 0x00000400
	.section .rom.00967f72, "ax"
	.incbin "baserom.gba", 0x00967f72, 0x00000002
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f74, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968074, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x0096853c, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x009687a4, 0x000001c8
	.section .rom.00968dd3, "ax"
	.incbin "baserom.gba", 0x00968dd3, 0x00000001
	.section .rom.00968fdf, "ax"
	.incbin "baserom.gba", 0x00968fdf, 0x00000001
	.section .rom.009691e3, "ax"
	.incbin "baserom.gba", 0x009691e3, 0x00000001
	.section .rom.00969272, "ax"
	.incbin "baserom.gba", 0x00969272, 0x00000002
	.section .rom.00969754, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x00969754, 0x00000070
	.section .rom.0096981f, "ax"
	.incbin "baserom.gba", 0x0096981f, 0x00000001
	.section .rom.00969852, "ax"
	.incbin "baserom.gba", 0x00969852, 0x00000002
	.section .rom.00969a1e, "ax"
	.incbin "baserom.gba", 0x00969a1e, 0x00000002
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969a20, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969c6c, 0x000000e4
	.section .rom.00969e09, "ax"
	.incbin "baserom.gba", 0x00969e09, 0x00000003
	.section .rom.00969fde, "ax"
	.incbin "baserom.gba", 0x00969fde, 0x00000002
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x00969fe0, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a078, 0x00000024
	.section .rom.0096a1ee, "ax"
	.incbin "baserom.gba", 0x0096a1ee, 0x00000002
	.section .rom.0096a2d1, "ax"
	.incbin "baserom.gba", 0x0096a2d1, 0x00000003
	.section .rom.0096a3a1, "ax"
	.incbin "baserom.gba", 0x0096a3a1, 0x00000003
	.section .rom.0096a4c7, "ax"
	.incbin "baserom.gba", 0x0096a4c7, 0x00000001
	.section .rom.0096a4f6, "ax"
	.incbin "baserom.gba", 0x0096a4f6, 0x00000002
	.section .rom.0096a75a, "ax"
	.incbin "baserom.gba", 0x0096a75a, 0x00000002
	.section .rom.0096a7f7, "ax"
	.incbin "baserom.gba", 0x0096a7f7, 0x00000001
	.section .rom.0096a85c, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a85c, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096ac5c, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096ad0c, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b26c, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b2e0, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b328, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b378, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b3c8, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b420, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b478, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b4b8, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b4fc, 0x00000054
	.section .rom.00970787, "ax"
	.incbin "baserom.gba", 0x00970787, 0x00000001
	.section .rom.009736d2, "ax"
	.incbin "baserom.gba", 0x009736d2, 0x00000002
	.section .rom.009772f5, "ax"
	.incbin "baserom.gba", 0x009772f5, 0x00000003
	.section .rom.00979541, "ax"
	.incbin "baserom.gba", 0x00979541, 0x00000003
	.section .rom.0097eadd, "ax"
	.incbin "baserom.gba", 0x0097eadd, 0x00000003
	.section .rom.00982357, "ax"
	.incbin "baserom.gba", 0x00982357, 0x00000001
	.section .rom.009881c1, "ax"
	.incbin "baserom.gba", 0x009881c1, 0x00000003
	.section .rom.00989295, "ax"
	.incbin "baserom.gba", 0x00989295, 0x00000003
	.section .rom.0098a301, "ax"
	.incbin "baserom.gba", 0x0098a301, 0x00000003
	.section .rom.0098f95b, "ax"
	.incbin "baserom.gba", 0x0098f95b, 0x00000001
	.section .rom.00992a9d, "ax"
	.incbin "baserom.gba", 0x00992a9d, 0x00000003
	.section .rom.00994b3d, "ax"
	.incbin "baserom.gba", 0x00994b3d, 0x00000003
	.section .rom.00996e5d, "ax"
	.incbin "baserom.gba", 0x00996e5d, 0x00000003
	.section .rom.00999ad6, "ax"
	.incbin "baserom.gba", 0x00999ad6, 0x00000002
	.section .rom.0099b9d1, "ax"
	.incbin "baserom.gba", 0x0099b9d1, 0x00000003
	.section .rom.009a3f15, "ax"
	.incbin "baserom.gba", 0x009a3f15, 0x00000003
	.section .rom.009acb97, "ax"
	.incbin "baserom.gba", 0x009acb97, 0x00000001
	.section .rom.009af711, "ax"
	.incbin "baserom.gba", 0x009af711, 0x00000003
	.section .rom.009b614e, "ax"
	.incbin "baserom.gba", 0x009b614e, 0x00000002
	.section .rom.009b8c42, "ax"
	.incbin "baserom.gba", 0x009b8c42, 0x00000002
	.section .rom.009ba732, "ax"
	.incbin "baserom.gba", 0x009ba732, 0x00000002
	.section .rom.009bd048, "ax"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bd048, 0x00003440
	.section .rom.009c4e2e, "ax"
	.incbin "baserom.gba", 0x009c4e2e, 0x00000002
	.section .rom.009c5dca, "ax"
	.incbin "baserom.gba", 0x009c5dca, 0x00000002
	.section .rom.009c84f5, "ax"
	.incbin "baserom.gba", 0x009c84f5, 0x00000003
	.section .rom.009c9c0a, "ax"
	.incbin "baserom.gba", 0x009c9c0a, 0x00000002
	.section .rom.009d2423, "ax"
	.incbin "baserom.gba", 0x009d2423, 0x00000001
	.section .rom.009d66eb, "ax"
	.incbin "baserom.gba", 0x009d66eb, 0x00000001
	.section .rom.009ddf25, "ax"
	.incbin "baserom.gba", 0x009ddf25, 0x00000003
	.section .rom.009df70a, "ax"
	.incbin "baserom.gba", 0x009df70a, 0x00000002
	.section .rom.009e24a1, "ax"
	.incbin "baserom.gba", 0x009e24a1, 0x00000003
	.section .rom.009e34eb, "ax"
	.incbin "baserom.gba", 0x009e34eb, 0x00000001
	.section .rom.009eba7b, "ax"
	.incbin "baserom.gba", 0x009eba7b, 0x00000001
	.section .rom.009ed875, "ax"
	.incbin "baserom.gba", 0x009ed875, 0x00000003
	.section .rom.009eec3b, "ax"
	.incbin "baserom.gba", 0x009eec3b, 0x00000001
	.section .rom.009f5b3d, "ax"
	.incbin "baserom.gba", 0x009f5b3d, 0x00000003
	.section .rom.009f7e3e, "ax"
	.incbin "baserom.gba", 0x009f7e3e, 0x00000002
	.section .rom.009fd08b, "ax"
	.incbin "baserom.gba", 0x009fd08b, 0x00000001
	.section .rom.00a0248a, "ax"
	.incbin "baserom.gba", 0x00a0248a, 0x00000002
	.section .rom.00a0463e, "ax"
	.incbin "baserom.gba", 0x00a0463e, 0x00000002
	.section .rom.00a087a5, "ax"
	.incbin "baserom.gba", 0x00a087a5, 0x00000003
	.section .rom.00a0b9ca, "ax"
	.incbin "baserom.gba", 0x00a0b9ca, 0x00000002
	.section .rom.00a0d0ce, "ax"
	.incbin "baserom.gba", 0x00a0d0ce, 0x00000002
	.section .rom.00a13cb6, "ax"
	.incbin "baserom.gba", 0x00a13cb6, 0x00000002
	.section .rom.00a20296, "ax"
	.incbin "baserom.gba", 0x00a20296, 0x00000002
	.section .rom.00a2332a, "ax"
	.incbin "baserom.gba", 0x00a2332a, 0x00000002
	.section .rom.00a25b95, "ax"
	.incbin "baserom.gba", 0x00a25b95, 0x00000003
	.section .rom.00a27686, "ax"
	.incbin "baserom.gba", 0x00a27686, 0x00000002
	.section .rom.00a2849e, "ax"
	.incbin "baserom.gba", 0x00a2849e, 0x00000002
	.section .rom.00a29189, "ax"
	.incbin "baserom.gba", 0x00a29189, 0x00000003
	.section .rom.00a32646, "ax"
	.incbin "baserom.gba", 0x00a32646, 0x00000002
	.section .rom.00a33351, "ax"
	.incbin "baserom.gba", 0x00a33351, 0x00000003
	.section .rom.00a345b5, "ax"
	.incbin "baserom.gba", 0x00a345b5, 0x00000003
	.section .rom.00a354e7, "ax"
	.incbin "baserom.gba", 0x00a354e7, 0x00000001
	.section .rom.00a36157, "ax"
	.incbin "baserom.gba", 0x00a36157, 0x00000001
	.section .rom.00a36d6f, "ax"
	.incbin "baserom.gba", 0x00a36d6f, 0x00000001
	.section .rom.00a374e3, "ax"
	.incbin "baserom.gba", 0x00a374e3, 0x00000001
	.section .rom.00a38d73, "ax"
	.incbin "baserom.gba", 0x00a38d73, 0x00000001
	.section .rom.00a39741, "ax"
	.incbin "baserom.gba", 0x00a39741, 0x00000003
	.section .rom.00a3a35f, "ax"
	.incbin "baserom.gba", 0x00a3a35f, 0x00000001
	.section .rom.00a3ca13, "ax"
	.incbin "baserom.gba", 0x00a3ca13, 0x00000001
	.section .rom.00a3f126, "ax"
	.incbin "baserom.gba", 0x00a3f126, 0x00000002
	.section .rom.00a4407b, "ax"
	.incbin "baserom.gba", 0x00a4407b, 0x00000001
	.section .rom.00a483a5, "ax"
	.incbin "baserom.gba", 0x00a483a5, 0x00000003
	.section .rom.00a48f92, "ax"
	.incbin "baserom.gba", 0x00a48f92, 0x00000002
	.section .rom.00a4db47, "ax"
	.incbin "baserom.gba", 0x00a4db47, 0x00000001
	.section .rom.00a4feb1, "ax"
	.incbin "baserom.gba", 0x00a4feb1, 0x00000003
	.section .rom.00a51853, "ax"
	.incbin "baserom.gba", 0x00a51853, 0x00000001
	.section .rom.00a56115, "ax"
	.incbin "baserom.gba", 0x00a56115, 0x00000003
	.section .rom.00a59525, "ax"
	.incbin "baserom.gba", 0x00a59525, 0x00000003
	.section .rom.00a5d5ed, "ax"
	.incbin "baserom.gba", 0x00a5d5ed, 0x00000003
	.section .rom.00a60cb6, "ax"
	.incbin "baserom.gba", 0x00a60cb6, 0x00000002
	.section .rom.00a68c87, "ax"
	.incbin "baserom.gba", 0x00a68c87, 0x00000001
	.section .rom.00a6ff97, "ax"
	.incbin "baserom.gba", 0x00a6ff97, 0x00000001
	.section .rom.00a74f17, "ax"
	.incbin "baserom.gba", 0x00a74f17, 0x00000001
	.section .rom.00a79999, "ax"
	.incbin "baserom.gba", 0x00a79999, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a7999c, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a799a8, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79af8, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79c38, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a79d78, 0x00000140
	.section .rom.00a7b115, "ax"
	.incbin "baserom.gba", 0x00a7b115, 0x00000003
	.section .rom.00a7b2e6, "ax"
	.incbin "baserom.gba", 0x00a7b2e6, 0x00000002
	.section .rom.00a7d387, "ax"
	.incbin "baserom.gba", 0x00a7d387, 0x00000001
	.section .rom.00a7e314, "ax"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e314, 0x000022d8
	.section .rom.00a81840, "ax"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a81840, 0x0000461c
	.section .rom.00a85f62, "ax"
	.incbin "baserom.gba", 0x00a85f62, 0x00000002
	.section .rom.00a8714d, "ax"
	.incbin "baserom.gba", 0x00a8714d, 0x00000003
	.section .rom.00a88dd5, "ax"
	.incbin "baserom.gba", 0x00a88dd5, 0x00000003
	.section .rom.00a892e3, "ax"
	.incbin "baserom.gba", 0x00a892e3, 0x00000001
	.section .rom.00a89423, "ax"
	.incbin "baserom.gba", 0x00a89423, 0x00000001
	.section .rom.00a8b0a6, "ax"
	.incbin "baserom.gba", 0x00a8b0a6, 0x00000002
	.section .rom.00a8da7e, "ax"
	.incbin "baserom.gba", 0x00a8da7e, 0x00000002
	.section .rom.00a90247, "ax"
	.incbin "baserom.gba", 0x00a90247, 0x00000001
	.section .rom.00a91767, "ax"
	.incbin "baserom.gba", 0x00a91767, 0x00000001
	.section .rom.00a930ab, "ax"
	.incbin "baserom.gba", 0x00a930ab, 0x00000001
	.section .rom.00a94b59, "ax"
	.incbin "baserom.gba", 0x00a94b59, 0x00000003
	.section .rom.00a94ccd, "ax"
	.incbin "baserom.gba", 0x00a94ccd, 0x00000003
	.section .rom.00a97ab1, "ax"
	.incbin "baserom.gba", 0x00a97ab1, 0x00000003
	.section .rom.00a9a35b, "ax"
	.incbin "baserom.gba", 0x00a9a35b, 0x00000001
	.section .rom.00a9e7f2, "ax"
	.incbin "baserom.gba", 0x00a9e7f2, 0x00000002
	.section .rom.00aa1b6a, "ax"
	.incbin "baserom.gba", 0x00aa1b6a, 0x00000002
	.section .rom.00aa4667, "ax"
	.incbin "baserom.gba", 0x00aa4667, 0x00000001
	.section .rom.00aa6745, "ax"
	.incbin "baserom.gba", 0x00aa6745, 0x00000003
	.section .rom.00aa8991, "ax"
	.incbin "baserom.gba", 0x00aa8991, 0x00000003
	.section .rom.00aa8ad3, "ax"
	.incbin "baserom.gba", 0x00aa8ad3, 0x00000001
	.section .rom.00aaa1a6, "ax"
	.incbin "baserom.gba", 0x00aaa1a6, 0x00000002
	.section .rom.00aaa2a6, "ax"
	.incbin "baserom.gba", 0x00aaa2a6, 0x00000002
	.section .rom.00aac199, "ax"
	.incbin "baserom.gba", 0x00aac199, 0x00000003
	.section .rom.00aadb71, "ax"
	.incbin "baserom.gba", 0x00aadb71, 0x00000003
	.section .rom.00ab05a2, "ax"
	.incbin "baserom.gba", 0x00ab05a2, 0x00000002
	.section .rom.00ab57f3, "ax"
	.incbin "baserom.gba", 0x00ab57f3, 0x00000001
	.section .rom.00ab8137, "ax"
	.incbin "baserom.gba", 0x00ab8137, 0x00000001
	.section .rom.00ab960a, "ax"
	.incbin "baserom.gba", 0x00ab960a, 0x00000002
	.section .rom.00abe3d5, "ax"
	.incbin "baserom.gba", 0x00abe3d5, 0x00000003
	.section .rom.00ac107f, "ax"
	.incbin "baserom.gba", 0x00ac107f, 0x00000001
	.section .rom.00ac42b7, "ax"
	.incbin "baserom.gba", 0x00ac42b7, 0x00000001
	.section .rom.00ac444f, "ax"
	.incbin "baserom.gba", 0x00ac444f, 0x00000001
	.section .rom.00ac724f, "ax"
	.incbin "baserom.gba", 0x00ac724f, 0x00000001
	.section .rom.00ac8a03, "ax"
	.incbin "baserom.gba", 0x00ac8a03, 0x00000001
	.section .rom.00ac999e, "ax"
	.incbin "baserom.gba", 0x00ac999e, 0x00000002
	.section .rom.00acbfff, "ax"
	.incbin "baserom.gba", 0x00acbfff, 0x00000001
	.section .rom.00acc1a6, "ax"
	.incbin "baserom.gba", 0x00acc1a6, 0x00000002
	.section .rom.00acf15f, "ax"
	.incbin "baserom.gba", 0x00acf15f, 0x00000001
	.section .rom.00acfe05, "ax"
	.incbin "baserom.gba", 0x00acfe05, 0x00000003
	.section .rom.00ad13d5, "ax"
	.incbin "baserom.gba", 0x00ad13d5, 0x00000003
	.section .rom.00ad93a1, "ax"
	.incbin "baserom.gba", 0x00ad93a1, 0x00000003
	.section .rom.00adae87, "ax"
	.incbin "baserom.gba", 0x00adae87, 0x00000001
	.section .rom.00adc371, "ax"
	.incbin "baserom.gba", 0x00adc371, 0x00000003
	.section .rom.00ae259b, "ax"
	.incbin "baserom.gba", 0x00ae259b, 0x00000001
	.section .rom.00ae2a6d, "ax"
	.incbin "baserom.gba", 0x00ae2a6d, 0x00000003
	.section .rom.00ae3bb5, "ax"
	.incbin "baserom.gba", 0x00ae3bb5, 0x00000003
	.section .rom.00ae3d66, "ax"
	.incbin "baserom.gba", 0x00ae3d66, 0x00000002
	.section .rom.00aefa53, "ax"
	.incbin "baserom.gba", 0x00aefa53, 0x00000001
	.section .rom.00aefb73, "ax"
	.incbin "baserom.gba", 0x00aefb73, 0x00000001
	.section .rom.00af1f13, "ax"
	.incbin "baserom.gba", 0x00af1f13, 0x00000001
	.section .rom.00af315f, "ax"
	.incbin "baserom.gba", 0x00af315f, 0x00000001
	.section .rom.00af54ef, "ax"
	.incbin "baserom.gba", 0x00af54ef, 0x00000001
	.section .rom.00af6ded, "ax"
	.incbin "baserom.gba", 0x00af6ded, 0x00000003
	.section .rom.00af7dde, "ax"
	.incbin "baserom.gba", 0x00af7dde, 0x00000002
	.section .rom.00afa37a, "ax"
	.incbin "baserom.gba", 0x00afa37a, 0x00000002
	.section .rom.00afe9a2, "ax"
	.incbin "baserom.gba", 0x00afe9a2, 0x00000002
	.section .rom.00aff067, "ax"
	.incbin "baserom.gba", 0x00aff067, 0x00000001
	.section .rom.00b01af5, "ax"
	.incbin "baserom.gba", 0x00b01af5, 0x00000003
	.section .rom.00b01c46, "ax"
	.incbin "baserom.gba", 0x00b01c46, 0x00000002
	.section .rom.00b04056, "ax"
	.incbin "baserom.gba", 0x00b04056, 0x00000002
	.section .rom.00b061d7, "ax"
	.incbin "baserom.gba", 0x00b061d7, 0x00000001
	.section .rom.00b06317, "ax"
	.incbin "baserom.gba", 0x00b06317, 0x00000001
	.section .rom.00b08db6, "ax"
	.incbin "baserom.gba", 0x00b08db6, 0x00000002
	.section .rom.00b08f07, "ax"
	.incbin "baserom.gba", 0x00b08f07, 0x00000001
	.section .rom.00b0b316, "ax"
	.incbin "baserom.gba", 0x00b0b316, 0x00000002
	.section .rom.00b0d497, "ax"
	.incbin "baserom.gba", 0x00b0d497, 0x00000001
	.section .rom.00b0d5d7, "ax"
	.incbin "baserom.gba", 0x00b0d5d7, 0x00000001
	.section .rom.00b10c06, "ax"
	.incbin "baserom.gba", 0x00b10c06, 0x00000002
	.section .rom.00b1316a, "ax"
	.incbin "baserom.gba", 0x00b1316a, 0x00000002
	.section .rom.00b152eb, "ax"
	.incbin "baserom.gba", 0x00b152eb, 0x00000001
	.section .rom.00b1542b, "ax"
	.incbin "baserom.gba", 0x00b1542b, 0x00000001
	.section .rom.00b168db, "ax"
	.incbin "baserom.gba", 0x00b168db, 0x00000001
	.section .rom.00b169e6, "ax"
	.incbin "baserom.gba", 0x00b169e6, 0x00000002
	.section .rom.00b1853e, "ax"
	.incbin "baserom.gba", 0x00b1853e, 0x00000002
	.section .rom.00b19be1, "ax"
	.incbin "baserom.gba", 0x00b19be1, 0x00000003
	.section .rom.00b19da2, "ax"
	.incbin "baserom.gba", 0x00b19da2, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19da4, 0x00000d9c
	.section .rom.00b1c796, "ax"
	.incbin "baserom.gba", 0x00b1c796, 0x00000002
	.section .rom.00b1dfed, "ax"
	.incbin "baserom.gba", 0x00b1dfed, 0x00000003
	.section .rom.00b1e1ae, "ax"
	.incbin "baserom.gba", 0x00b1e1ae, 0x00000002
	.section .rom.00b22429, "ax"
	.incbin "baserom.gba", 0x00b22429, 0x00000003
	.section .rom.00b225ea, "ax"
	.incbin "baserom.gba", 0x00b225ea, 0x00000002
	.section .rom.00b23061, "ax"
	.incbin "baserom.gba", 0x00b23061, 0x00000003
	.section .rom.00b23169, "ax"
	.incbin "baserom.gba", 0x00b23169, 0x00000003
	.section .rom.00b24e2a, "ax"
	.incbin "baserom.gba", 0x00b24e2a, 0x00000002
	.section .rom.00b265b7, "ax"
	.incbin "baserom.gba", 0x00b265b7, 0x00000001
	.section .rom.00b2686d, "ax"
	.incbin "baserom.gba", 0x00b2686d, 0x00000003
	.section .rom.00b2837d, "ax"
	.incbin "baserom.gba", 0x00b2837d, 0x00000003
	.section .rom.00b2ac0a, "ax"
	.incbin "baserom.gba", 0x00b2ac0a, 0x00000002
	.section .rom.00b2d3fa, "ax"
	.incbin "baserom.gba", 0x00b2d3fa, 0x00000002
	.section .rom.00b2e39a, "ax"
	.incbin "baserom.gba", 0x00b2e39a, 0x00000002
	.section .rom.00b304ed, "ax"
	.incbin "baserom.gba", 0x00b304ed, 0x00000003
	.section .rom.00b33b7a, "ax"
	.incbin "baserom.gba", 0x00b33b7a, 0x00000002
	.section .rom.00b3590f, "ax"
	.incbin "baserom.gba", 0x00b3590f, 0x00000001
	.section .rom.00b3772d, "ax"
	.incbin "baserom.gba", 0x00b3772d, 0x00000003
	.section .rom.00b39209, "ax"
	.incbin "baserom.gba", 0x00b39209, 0x00000003
	.section .rom.00b3a7f2, "ax"
	.incbin "baserom.gba", 0x00b3a7f2, 0x00000002
	.section .rom.00b3cdda, "ax"
	.incbin "baserom.gba", 0x00b3cdda, 0x00000002
	.section .rom.00b3cf53, "ax"
	.incbin "baserom.gba", 0x00b3cf53, 0x00000001
	.section .rom.00b3e412, "ax"
	.incbin "baserom.gba", 0x00b3e412, 0x00000002
	.section .rom.00b3fc16, "ax"
	.incbin "baserom.gba", 0x00b3fc16, 0x00000002
	.section .rom.00b4158e, "ax"
	.incbin "baserom.gba", 0x00b4158e, 0x00000002
	.section .rom.00b424ae, "ax"
	.incbin "baserom.gba", 0x00b424ae, 0x00000002
	.section .rom.00b43f5a, "ax"
	.incbin "baserom.gba", 0x00b43f5a, 0x00000002
	.section .rom.00b44067, "ax"
	.incbin "baserom.gba", 0x00b44067, 0x00000001
	.section .rom.00b46197, "ax"
	.incbin "baserom.gba", 0x00b46197, 0x00000001
	.section .rom.00b47d6d, "ax"
	.incbin "baserom.gba", 0x00b47d6d, 0x00000003
	.section .rom.00b4837f, "ax"
	.incbin "baserom.gba", 0x00b4837f, 0x00000001
	.section .rom.00b484bf, "ax"
	.incbin "baserom.gba", 0x00b484bf, 0x00000001
	.section .rom.00b4abbf, "ax"
	.incbin "baserom.gba", 0x00b4abbf, 0x00000001
	.section .rom.00b4c41e, "ax"
	.incbin "baserom.gba", 0x00b4c41e, 0x00000002
	.section .rom.00b4ca2f, "ax"
	.incbin "baserom.gba", 0x00b4ca2f, 0x00000001
	.section .rom.00b4cb6f, "ax"
	.incbin "baserom.gba", 0x00b4cb6f, 0x00000001
	.section .rom.00b4d9ea, "ax"
	.incbin "baserom.gba", 0x00b4d9ea, 0x00000002
	.section .rom.00b4db01, "ax"
	.incbin "baserom.gba", 0x00b4db01, 0x00000003
	.section .rom.00b4fdd7, "ax"
	.incbin "baserom.gba", 0x00b4fdd7, 0x00000001
	.section .rom.00b51a2a, "ax"
	.incbin "baserom.gba", 0x00b51a2a, 0x00000002
	.section .rom.00b5206f, "ax"
	.incbin "baserom.gba", 0x00b5206f, 0x00000001
	.section .rom.00b521af, "ax"
	.incbin "baserom.gba", 0x00b521af, 0x00000001
	.section .rom.00b52db5, "ax"
	.incbin "baserom.gba", 0x00b52db5, 0x00000003
	.section .rom.00b55367, "ax"
	.incbin "baserom.gba", 0x00b55367, 0x00000001
	.section .rom.00b57026, "ax"
	.incbin "baserom.gba", 0x00b57026, 0x00000002
	.section .rom.00b58af2, "ax"
	.incbin "baserom.gba", 0x00b58af2, 0x00000002
	.section .rom.00b59bc6, "ax"
	.incbin "baserom.gba", 0x00b59bc6, 0x00000002
	.section .rom.00b59d2d, "ax"
	.incbin "baserom.gba", 0x00b59d2d, 0x00000003
	.section .rom.00b5af46, "ax"
	.incbin "baserom.gba", 0x00b5af46, 0x00000002
	.section .rom.00b5bb25, "ax"
	.incbin "baserom.gba", 0x00b5bb25, 0x00000003
	.section .rom.00b5bc92, "ax"
	.incbin "baserom.gba", 0x00b5bc92, 0x00000002
	.section .rom.00b5eae3, "ax"
	.incbin "baserom.gba", 0x00b5eae3, 0x00000001
	.section .rom.00b612a2, "ax"
	.incbin "baserom.gba", 0x00b612a2, 0x00000002
	.section .rom.00b67353, "ax"
	.incbin "baserom.gba", 0x00b67353, 0x00000001
	.section .rom.00b68e83, "ax"
	.incbin "baserom.gba", 0x00b68e83, 0x00000001
	.section .rom.00b6b1ba, "ax"
	.incbin "baserom.gba", 0x00b6b1ba, 0x00000002
	.section .rom.00b6c36b, "ax"
	.incbin "baserom.gba", 0x00b6c36b, 0x00000001
	.section .rom.00b6dc8d, "ax"
	.incbin "baserom.gba", 0x00b6dc8d, 0x00000003
	.section .rom.00b6fb19, "ax"
	.incbin "baserom.gba", 0x00b6fb19, 0x00000003
	.section .rom.00b71cfa, "ax"
	.incbin "baserom.gba", 0x00b71cfa, 0x00000002
	.section .rom.00b756c1, "ax"
	.incbin "baserom.gba", 0x00b756c1, 0x00000003
	.section .rom.00b757ad, "ax"
	.incbin "baserom.gba", 0x00b757ad, 0x00000003
	.section .rom.00b789e6, "ax"
	.incbin "baserom.gba", 0x00b789e6, 0x00000002
	.section .rom.00b7905d, "ax"
	.incbin "baserom.gba", 0x00b7905d, 0x00000003
	.section .rom.00b7ce1f, "ax"
	.incbin "baserom.gba", 0x00b7ce1f, 0x00000001
	.section .rom.00b7f0b1, "ax"
	.incbin "baserom.gba", 0x00b7f0b1, 0x00000003
	.section .rom.00b80ace, "ax"
	.incbin "baserom.gba", 0x00b80ace, 0x00000002
	.section .rom.00b81bdb, "ax"
	.incbin "baserom.gba", 0x00b81bdb, 0x00000001
	.section .rom.00b84d7a, "ax"
	.incbin "baserom.gba", 0x00b84d7a, 0x00000002
	.section .rom.00b85f72, "ax"
	.incbin "baserom.gba", 0x00b85f72, 0x00000002
	.section .rom.00b86e35, "ax"
	.incbin "baserom.gba", 0x00b86e35, 0x00000003
	.section .rom.00b88ed6, "ax"
	.incbin "baserom.gba", 0x00b88ed6, 0x00000002
	.section .rom.00b8a92b, "ax"
	.incbin "baserom.gba", 0x00b8a92b, 0x00000001
	.section .rom.00b8bdf6, "ax"
	.incbin "baserom.gba", 0x00b8bdf6, 0x00000002
	.section .rom.00b8dffa, "ax"
	.incbin "baserom.gba", 0x00b8dffa, 0x00000002
	.section .rom.00b8e0ef, "ax"
	.incbin "baserom.gba", 0x00b8e0ef, 0x00000001
	.section .rom.00b8f6c2, "ax"
	.incbin "baserom.gba", 0x00b8f6c2, 0x00000002
	.section .rom.00b8f8e1, "ax"
	.incbin "baserom.gba", 0x00b8f8e1, 0x00000003
	.section .rom.00b919c5, "ax"
	.incbin "baserom.gba", 0x00b919c5, 0x00000003
	.section .rom.00b91b8a, "ax"
	.incbin "baserom.gba", 0x00b91b8a, 0x00000002
	.section .rom.00b93c4d, "ax"
	.incbin "baserom.gba", 0x00b93c4d, 0x00000003
	.section .rom.00b93d7d, "ax"
	.incbin "baserom.gba", 0x00b93d7d, 0x00000003
	.section .rom.00b96836, "ax"
	.incbin "baserom.gba", 0x00b96836, 0x00000002
	.section .rom.00b9838e, "ax"
	.incbin "baserom.gba", 0x00b9838e, 0x00000002
	.section .rom.00b992d5, "ax"
	.incbin "baserom.gba", 0x00b992d5, 0x00000003
	.section .rom.00b9acde, "ax"
	.incbin "baserom.gba", 0x00b9acde, 0x00000002
	.section .rom.00baaa33, "ax"
	.incbin "baserom.gba", 0x00baaa33, 0x00000001
	.section .rom.00baab83, "ax"
	.incbin "baserom.gba", 0x00baab83, 0x00000001
	.section .rom.00bad68e, "ax"
	.incbin "baserom.gba", 0x00bad68e, 0x00000002
	.section .rom.00baf297, "ax"
	.incbin "baserom.gba", 0x00baf297, 0x00000001
	.section .rom.00bb0c9e, "ax"
	.incbin "baserom.gba", 0x00bb0c9e, 0x00000002
	.section .rom.00bb29b9, "ax"
	.incbin "baserom.gba", 0x00bb29b9, 0x00000003
	.section .rom.00bb2b0b, "ax"
	.incbin "baserom.gba", 0x00bb2b0b, 0x00000001
	.section .rom.00bb4512, "ax"
	.incbin "baserom.gba", 0x00bb4512, 0x00000002
	.section .rom.00bb829a, "ax"
	.incbin "baserom.gba", 0x00bb829a, 0x00000002
	.section .rom.00bb842d, "ax"
	.incbin "baserom.gba", 0x00bb842d, 0x00000003
	.section .rom.00bbd437, "ax"
	.incbin "baserom.gba", 0x00bbd437, 0x00000001
	.section .rom.00bbfab9, "ax"
	.incbin "baserom.gba", 0x00bbfab9, 0x00000003
	.section .rom.00bbfdef, "ax"
	.incbin "baserom.gba", 0x00bbfdef, 0x00000001
	.section .rom.00bc6245, "ax"
	.incbin "baserom.gba", 0x00bc6245, 0x00000003
	.section .rom.00bc6392, "ax"
	.incbin "baserom.gba", 0x00bc6392, 0x00000002
	.section .rom.00bc8217, "ax"
	.incbin "baserom.gba", 0x00bc8217, 0x00000001
	.section .rom.00bc97be, "ax"
	.incbin "baserom.gba", 0x00bc97be, 0x00000002
	.section .rom.00bcb769, "ax"
	.incbin "baserom.gba", 0x00bcb769, 0x00000003
	.section .rom.00bcc6da, "ax"
	.incbin "baserom.gba", 0x00bcc6da, 0x00000002
	.section .rom.00bd7ce3, "ax"
	.incbin "baserom.gba", 0x00bd7ce3, 0x00000001
	.section .rom.00bd7d9f, "ax"
	.incbin "baserom.gba", 0x00bd7d9f, 0x00000001
	.section .rom.00bd8ade, "ax"
	.incbin "baserom.gba", 0x00bd8ade, 0x00000002
	.section .rom.00bd94bb, "ax"
	.incbin "baserom.gba", 0x00bd94bb, 0x00000001
	.section .rom.00bda0d3, "ax"
	.incbin "baserom.gba", 0x00bda0d3, 0x00000001
	.section .rom.00bdbf8e, "ax"
	.incbin "baserom.gba", 0x00bdbf8e, 0x00000002
	.section .rom.00bdc0aa, "ax"
	.incbin "baserom.gba", 0x00bdc0aa, 0x00000002
	.section .rom.00bde92b, "ax"
	.incbin "baserom.gba", 0x00bde92b, 0x00000001
	.section .rom.00be00b6, "ax"
	.incbin "baserom.gba", 0x00be00b6, 0x00000002
	.section .rom.00be3fe6, "ax"
	.incbin "baserom.gba", 0x00be3fe6, 0x00000002
	.section .rom.00be412f, "ax"
	.incbin "baserom.gba", 0x00be412f, 0x00000001
	.section .rom.00be67df, "ax"
	.incbin "baserom.gba", 0x00be67df, 0x00000001
	.section .rom.00be8f82, "ax"
	.incbin "baserom.gba", 0x00be8f82, 0x00000002
	.section .rom.00be9d0d, "ax"
	.incbin "baserom.gba", 0x00be9d0d, 0x00000003
	.section .rom.00becb4e, "ax"
	.incbin "baserom.gba", 0x00becb4e, 0x00000002
	.section .rom.00bef175, "ax"
	.incbin "baserom.gba", 0x00bef175, 0x00000003
	.section .rom.00bf0e61, "ax"
	.incbin "baserom.gba", 0x00bf0e61, 0x00000003
	.section .rom.00bf2561, "ax"
	.incbin "baserom.gba", 0x00bf2561, 0x00000003
	.section .rom.00bf3add, "ax"
	.incbin "baserom.gba", 0x00bf3add, 0x00000003
	.section .rom.00bf524f, "ax"
	.incbin "baserom.gba", 0x00bf524f, 0x00000001
	.section .rom.00bf7eda, "ax"
	.incbin "baserom.gba", 0x00bf7eda, 0x00000002
	.section .rom.00bf8f26, "ax"
	.incbin "baserom.gba", 0x00bf8f26, 0x00000002
	.section .rom.00bfa023, "ax"
	.incbin "baserom.gba", 0x00bfa023, 0x00000001
	.section .rom.00bfb7f7, "ax"
	.incbin "baserom.gba", 0x00bfb7f7, 0x00000001
	.section .rom.00bfccbb, "ax"
	.incbin "baserom.gba", 0x00bfccbb, 0x00000001
	.section .rom.00bff1ee, "ax"
	.incbin "baserom.gba", 0x00bff1ee, 0x00000002
	.section .rom.00bff322, "ax"
	.incbin "baserom.gba", 0x00bff322, 0x00000002
	.section .rom.00c01a8e, "ax"
	.incbin "baserom.gba", 0x00c01a8e, 0x00000002
	.section .rom.00c03861, "ax"
	.incbin "baserom.gba", 0x00c03861, 0x00000003
	.section .rom.00c04ba3, "ax"
	.incbin "baserom.gba", 0x00c04ba3, 0x00000001
	.section .rom.00c07115, "ax"
	.incbin "baserom.gba", 0x00c07115, 0x00000003
	.section .rom.00c0987b, "ax"
	.incbin "baserom.gba", 0x00c0987b, 0x00000001
	.section .rom.00c0b1e5, "ax"
	.incbin "baserom.gba", 0x00c0b1e5, 0x00000003
	.section .rom.00c0da0e, "ax"
	.incbin "baserom.gba", 0x00c0da0e, 0x00000002
	.section .rom.00c11ded, "ax"
	.incbin "baserom.gba", 0x00c11ded, 0x00000003
	.section .rom.00c15197, "ax"
	.incbin "baserom.gba", 0x00c15197, 0x00000001
	.section .rom.00c15f8a, "ax"
	.incbin "baserom.gba", 0x00c15f8a, 0x00000002
	.section .rom.00c16d02, "ax"
	.incbin "baserom.gba", 0x00c16d02, 0x00000002
	.section .rom.00c1ee2a, "ax"
	.incbin "baserom.gba", 0x00c1ee2a, 0x00000002
	.section .rom.00c251a9, "ax"
	.incbin "baserom.gba", 0x00c251a9, 0x00000003
	.section .rom.00c25baf, "ax"
	.incbin "baserom.gba", 0x00c25baf, 0x00000001
	.section .rom.00c2765d, "ax"
	.incbin "baserom.gba", 0x00c2765d, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27660, 0x00001490
	.section .rom.00c28f57, "ax"
	.incbin "baserom.gba", 0x00c28f57, 0x00000001
	.section .rom.00c29116, "ax"
	.incbin "baserom.gba", 0x00c29116, 0x00000002
	.section .rom.00c2b91e, "ax"
	.incbin "baserom.gba", 0x00c2b91e, 0x00000002
	.section .rom.00c2ba76, "ax"
	.incbin "baserom.gba", 0x00c2ba76, 0x00000002
	.section .rom.00c2e3bd, "ax"
	.incbin "baserom.gba", 0x00c2e3bd, 0x00000003
	.section .rom.00c305e1, "ax"
	.incbin "baserom.gba", 0x00c305e1, 0x00000003
	.section .rom.00c31ad6, "ax"
	.incbin "baserom.gba", 0x00c31ad6, 0x00000002
	.section .rom.00c33d39, "ax"
	.incbin "baserom.gba", 0x00c33d39, 0x00000003
	.section .rom.00c365e7, "ax"
	.incbin "baserom.gba", 0x00c365e7, 0x00000001
	.section .rom.00c3857a, "ax"
	.incbin "baserom.gba", 0x00c3857a, 0x00000002
	.section .rom.00c3959b, "ax"
	.incbin "baserom.gba", 0x00c3959b, 0x00000001
	.section .rom.00c396db, "ax"
	.incbin "baserom.gba", 0x00c396db, 0x00000001
	.section .rom.00c3c2fa, "ax"
	.incbin "baserom.gba", 0x00c3c2fa, 0x00000002
	.section .rom.00c3e77e, "ax"
	.incbin "baserom.gba", 0x00c3e77e, 0x00000002
	.section .rom.00c40865, "ax"
	.incbin "baserom.gba", 0x00c40865, 0x00000003
	.section .rom.00c420c9, "ax"
	.incbin "baserom.gba", 0x00c420c9, 0x00000003
	.section .rom.00c4461f, "ax"
	.incbin "baserom.gba", 0x00c4461f, 0x00000001
	.section .rom.00c44785, "ax"
	.incbin "baserom.gba", 0x00c44785, 0x00000003
	.section .rom.00c46f7f, "ax"
	.incbin "baserom.gba", 0x00c46f7f, 0x00000001
	.section .rom.00c48fb3, "ax"
	.incbin "baserom.gba", 0x00c48fb3, 0x00000001
	.section .rom.00c4abda, "ax"
	.incbin "baserom.gba", 0x00c4abda, 0x00000002
	.section .rom.00c4d5fa, "ax"
	.incbin "baserom.gba", 0x00c4d5fa, 0x00000002
	.section .rom.00c4e165, "ax"
	.incbin "baserom.gba", 0x00c4e165, 0x00000003
	.section .rom.00c4e24b, "ax"
	.incbin "baserom.gba", 0x00c4e24b, 0x00000001
	.section .rom.00c4fb95, "ax"
	.incbin "baserom.gba", 0x00c4fb95, 0x00000003
	.section .rom.00c514fb, "ax"
	.incbin "baserom.gba", 0x00c514fb, 0x00000001
	.section .rom.00c528c9, "ax"
	.incbin "baserom.gba", 0x00c528c9, 0x00000003
	.section .rom.00c529af, "ax"
	.incbin "baserom.gba", 0x00c529af, 0x00000001
	.section .rom.00c5351d, "ax"
	.incbin "baserom.gba", 0x00c5351d, 0x00000003
	.section .rom.00c53603, "ax"
	.incbin "baserom.gba", 0x00c53603, 0x00000001
	.section .rom.00c54367, "ax"
	.incbin "baserom.gba", 0x00c54367, 0x00000001
	.section .rom.00c5444b, "ax"
	.incbin "baserom.gba", 0x00c5444b, 0x00000001
	.section .rom.00c54c51, "ax"
	.incbin "baserom.gba", 0x00c54c51, 0x00000003
	.section .rom.00c54d37, "ax"
	.incbin "baserom.gba", 0x00c54d37, 0x00000001
	.section .rom.00c55897, "ax"
	.incbin "baserom.gba", 0x00c55897, 0x00000001
	.section .rom.00c56115, "ax"
	.incbin "baserom.gba", 0x00c56115, 0x00000003
	.section .rom.00c56212, "ax"
	.incbin "baserom.gba", 0x00c56212, 0x00000002
	.section .rom.00c5806b, "ax"
	.incbin "baserom.gba", 0x00c5806b, 0x00000001
	.section .rom.00c58e79, "ax"
	.incbin "baserom.gba", 0x00c58e79, 0x00000003
	.section .rom.00c5a10d, "ax"
	.incbin "baserom.gba", 0x00c5a10d, 0x00000003
	.section .rom.00c5b0e9, "ax"
	.incbin "baserom.gba", 0x00c5b0e9, 0x00000003
	.section .rom.00c5d4ef, "ax"
	.incbin "baserom.gba", 0x00c5d4ef, 0x00000001
	.section .rom.00c5d62f, "ax"
	.incbin "baserom.gba", 0x00c5d62f, 0x00000001
	.section .rom.00c5dcfd, "ax"
	.incbin "baserom.gba", 0x00c5dcfd, 0x00000003
	.section .rom.00c5ddd3, "ax"
	.incbin "baserom.gba", 0x00c5ddd3, 0x00000001
	.section .rom.00c5e649, "ax"
	.incbin "baserom.gba", 0x00c5e649, 0x00000003
	.section .rom.00c5eafa, "ax"
	.incbin "baserom.gba", 0x00c5eafa, 0x00000002
	.section .rom.00c5ff75, "ax"
	.incbin "baserom.gba", 0x00c5ff75, 0x00000003
	.section .rom.00c6078b, "ax"
	.incbin "baserom.gba", 0x00c6078b, 0x00000001
	.section .rom.00c61421, "ax"
	.incbin "baserom.gba", 0x00c61421, 0x00000003
	.section .rom.00c615c6, "ax"
	.incbin "baserom.gba", 0x00c615c6, 0x00000002
	.section .rom.00c6448a, "ax"
	.incbin "baserom.gba", 0x00c6448a, 0x00000002
	.section .rom.00c65e09, "ax"
	.incbin "baserom.gba", 0x00c65e09, 0x00000003
	.section .rom.00c66e56, "ax"
	.incbin "baserom.gba", 0x00c66e56, 0x00000002
	.section .rom.00c6e59b, "ax"
	.incbin "baserom.gba", 0x00c6e59b, 0x00000001
	.section .rom.00c75d8f, "ax"
	.incbin "baserom.gba", 0x00c75d8f, 0x00000001
	.section .rom.00c7c389, "ax"
	.incbin "baserom.gba", 0x00c7c389, 0x00000003
	.section .rom.00c7edc2, "ax"
	.incbin "baserom.gba", 0x00c7edc2, 0x00000002
	.section .rom.00c81693, "ax"
	.incbin "baserom.gba", 0x00c81693, 0x00000001
	.section .rom.00c85edf, "ax"
	.incbin "baserom.gba", 0x00c85edf, 0x00000001
	.section .rom.00c86edd, "ax"
	.incbin "baserom.gba", 0x00c86edd, 0x00000003
	.section .rom.00c86ff6, "ax"
	.incbin "baserom.gba", 0x00c86ff6, 0x00000002
	.section .rom.00c8855e, "ax"
	.incbin "baserom.gba", 0x00c8855e, 0x00000002
	.section .rom.00c89fbb, "ax"
	.incbin "baserom.gba", 0x00c89fbb, 0x00000001
	.section .rom.00c8a0f6, "ax"
	.incbin "baserom.gba", 0x00c8a0f6, 0x00000002
	.section .rom.00c8b5fd, "ax"
	.incbin "baserom.gba", 0x00c8b5fd, 0x00000003
	.section .rom.00c8e886, "ax"
	.incbin "baserom.gba", 0x00c8e886, 0x00000002
	.section .rom.00c8fccd, "ax"
	.incbin "baserom.gba", 0x00c8fccd, 0x00000003
	.section .rom.00c929c5, "ax"
	.incbin "baserom.gba", 0x00c929c5, 0x00000003
	.section .rom.00c94dfb, "ax"
	.incbin "baserom.gba", 0x00c94dfb, 0x00000001
	.section .rom.00c94fd7, "ax"
	.incbin "baserom.gba", 0x00c94fd7, 0x00000001
	.section .rom.00c980eb, "ax"
	.incbin "baserom.gba", 0x00c980eb, 0x00000001
	.section .rom.00c9956e, "ax"
	.incbin "baserom.gba", 0x00c9956e, 0x00000002
	.section .rom.00c9a5bd, "ax"
	.incbin "baserom.gba", 0x00c9a5bd, 0x00000003
	.section .rom.00c9c422, "ax"
	.incbin "baserom.gba", 0x00c9c422, 0x00000002
	.section .rom.00c9c5bd, "ax"
	.incbin "baserom.gba", 0x00c9c5bd, 0x00000003
	.section .rom.00c9c71b, "ax"
	.incbin "baserom.gba", 0x00c9c71b, 0x00000001
	.section .rom.00c9d69a, "ax"
	.incbin "baserom.gba", 0x00c9d69a, 0x00000002
	.section .rom.00c9d7b2, "ax"
	.incbin "baserom.gba", 0x00c9d7b2, 0x00000002
	.section .rom.00c9f845, "ax"
	.incbin "baserom.gba", 0x00c9f845, 0x00000003
	.section .rom.00ca16c7, "ax"
	.incbin "baserom.gba", 0x00ca16c7, 0x00000001
	.section .rom.00ca3bed, "ax"
	.incbin "baserom.gba", 0x00ca3bed, 0x00000003
	.section .rom.00ca3cfa, "ax"
	.incbin "baserom.gba", 0x00ca3cfa, 0x00000002
	.section .rom.00ca5d8d, "ax"
	.incbin "baserom.gba", 0x00ca5d8d, 0x00000003
	.section .rom.00ca9263, "ax"
	.incbin "baserom.gba", 0x00ca9263, 0x00000001
	.section .rom.00ca9d4d, "ax"
	.incbin "baserom.gba", 0x00ca9d4d, 0x00000003
	.section .rom.00ca9ebd, "ax"
	.incbin "baserom.gba", 0x00ca9ebd, 0x00000003
	.section .rom.00cacdd7, "ax"
	.incbin "baserom.gba", 0x00cacdd7, 0x00000001
	.section .rom.00caed62, "ax"
	.incbin "baserom.gba", 0x00caed62, 0x00000002
	.section .rom.00cb06cd, "ax"
	.incbin "baserom.gba", 0x00cb06cd, 0x00000003
	.section .rom.00cb61be, "ax"
	.incbin "baserom.gba", 0x00cb61be, 0x00000002
	.section .rom.00cb636e, "ax"
	.incbin "baserom.gba", 0x00cb636e, 0x00000002
	.section .rom.00cbacca, "ax"
	.incbin "baserom.gba", 0x00cbacca, 0x00000002
	.section .rom.00cbae22, "ax"
	.incbin "baserom.gba", 0x00cbae22, 0x00000002
	.section .rom.00cbe3ef, "ax"
	.incbin "baserom.gba", 0x00cbe3ef, 0x00000001
	.section .rom.00cbfc71, "ax"
	.incbin "baserom.gba", 0x00cbfc71, 0x00000003
	.section .rom.00cc1999, "ax"
	.incbin "baserom.gba", 0x00cc1999, 0x00000003
	.section .rom.00cc50ba, "ax"
	.incbin "baserom.gba", 0x00cc50ba, 0x00000002
	.section .rom.00cca243, "ax"
	.incbin "baserom.gba", 0x00cca243, 0x00000001
	.section .rom.00ccc811, "ax"
	.incbin "baserom.gba", 0x00ccc811, 0x00000003
	.section .rom.00cce19d, "ax"
	.incbin "baserom.gba", 0x00cce19d, 0x00000003
	.section .rom.00ccf2ca, "ax"
	.incbin "baserom.gba", 0x00ccf2ca, 0x00000002
	.section .rom.00ccfe7a, "ax"
	.incbin "baserom.gba", 0x00ccfe7a, 0x00000002
	.section .rom.00cd1842, "ax"
	.incbin "baserom.gba", 0x00cd1842, 0x00000002
	.section .rom.00cd1926, "ax"
	.incbin "baserom.gba", 0x00cd1926, 0x00000002
	.section .rom.00cd3d83, "ax"
	.incbin "baserom.gba", 0x00cd3d83, 0x00000001
	.section .rom.00cd3ec3, "ax"
	.incbin "baserom.gba", 0x00cd3ec3, 0x00000001
	.section .rom.00cd7539, "ax"
	.incbin "baserom.gba", 0x00cd7539, 0x00000003
	.section .rom.00cd769d, "ax"
	.incbin "baserom.gba", 0x00cd769d, 0x00000003
	.section .rom.00cd9d7d, "ax"
	.incbin "baserom.gba", 0x00cd9d7d, 0x00000003
	.section .rom.00cdcc2d, "ax"
	.incbin "baserom.gba", 0x00cdcc2d, 0x00000003
	.section .rom.00cde1a2, "ax"
	.incbin "baserom.gba", 0x00cde1a2, 0x00000002
	.section .rom.00ce229e, "ax"
	.incbin "baserom.gba", 0x00ce229e, 0x00000002
	.section .rom.00ce4523, "ax"
	.incbin "baserom.gba", 0x00ce4523, 0x00000001
	.section .rom.00ce6107, "ax"
	.incbin "baserom.gba", 0x00ce6107, 0x00000001
	.section .rom.00ce7dfd, "ax"
	.incbin "baserom.gba", 0x00ce7dfd, 0x00000003
	.section .rom.00cf5421, "ax"
	.incbin "baserom.gba", 0x00cf5421, 0x00000003
	.section .rom.00cf5549, "ax"
	.incbin "baserom.gba", 0x00cf5549, 0x00000003
	.section .rom.00cf775a, "ax"
	.incbin "baserom.gba", 0x00cf775a, 0x00000002
	.section .rom.00cf78eb, "ax"
	.incbin "baserom.gba", 0x00cf78eb, 0x00000001
	.section .rom.00cfed9d, "ax"
	.incbin "baserom.gba", 0x00cfed9d, 0x00000003
	.section .rom.00d01573, "ax"
	.incbin "baserom.gba", 0x00d01573, 0x00000001
	.section .rom.00d03cce, "ax"
	.incbin "baserom.gba", 0x00d03cce, 0x00000002
	.section .rom.00d03e81, "ax"
	.incbin "baserom.gba", 0x00d03e81, 0x00000003
	.section .rom.00d05585, "ax"
	.incbin "baserom.gba", 0x00d05585, 0x00000003
	.section .rom.00d075df, "ax"
	.incbin "baserom.gba", 0x00d075df, 0x00000001
	.section .rom.00d08929, "ax"
	.incbin "baserom.gba", 0x00d08929, 0x00000003
	.section .rom.00d0a373, "ax"
	.incbin "baserom.gba", 0x00d0a373, 0x00000001
	.section .rom.00d0a497, "ax"
	.incbin "baserom.gba", 0x00d0a497, 0x00000001
	.section .rom.00d0a969, "ax"
	.incbin "baserom.gba", 0x00d0a969, 0x00000003
	.section .rom.00d0cb4d, "ax"
	.incbin "baserom.gba", 0x00d0cb4d, 0x00000003
	.section .rom.00d0d021, "ax"
	.incbin "baserom.gba", 0x00d0d021, 0x00000003
	.section .rom.00d0f557, "ax"
	.incbin "baserom.gba", 0x00d0f557, 0x00000001
	.section .rom.00d0f6df, "ax"
	.incbin "baserom.gba", 0x00d0f6df, 0x00000001
	.section .rom.00d13f81, "ax"
	.incbin "baserom.gba", 0x00d13f81, 0x00000003
	.section .rom.00d140b7, "ax"
	.incbin "baserom.gba", 0x00d140b7, 0x00000001
	.section .rom.00d15d23, "ax"
	.incbin "baserom.gba", 0x00d15d23, 0x00000001
	.section .rom.00d16edb, "ax"
	.incbin "baserom.gba", 0x00d16edb, 0x00000001
	.section .rom.00d17c29, "ax"
	.incbin "baserom.gba", 0x00d17c29, 0x00000003
	.section .rom.00d18db2, "ax"
	.incbin "baserom.gba", 0x00d18db2, 0x00000002
	.section .rom.00d1a8ab, "ax"
	.incbin "baserom.gba", 0x00d1a8ab, 0x00000001
	.section .rom.00d1bcb7, "ax"
	.incbin "baserom.gba", 0x00d1bcb7, 0x00000001
	.section .rom.00d1c20b, "ax"
	.incbin "baserom.gba", 0x00d1c20b, 0x00000001
	.section .rom.00d1c845, "ax"
	.incbin "baserom.gba", 0x00d1c845, 0x00000003
	.section .rom.00d21b11, "ax"
	.incbin "baserom.gba", 0x00d21b11, 0x00000003
	.section .rom.00d24172, "ax"
	.incbin "baserom.gba", 0x00d24172, 0x00000002
	.section .rom.00d24307, "ax"
	.incbin "baserom.gba", 0x00d24307, 0x00000001
	.section .rom.00d25ebb, "ax"
	.incbin "baserom.gba", 0x00d25ebb, 0x00000001
	.section .rom.00d2601b, "ax"
	.incbin "baserom.gba", 0x00d2601b, 0x00000001
	.section .rom.00d28abf, "ax"
	.incbin "baserom.gba", 0x00d28abf, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28ac0, 0x000021b0
	.section .rom.00d2cae6, "ax"
	.incbin "baserom.gba", 0x00d2cae6, 0x00000002
	.section .rom.00d2cc27, "ax"
	.incbin "baserom.gba", 0x00d2cc27, 0x00000001
	.section .rom.00d33b9b, "ax"
	.incbin "baserom.gba", 0x00d33b9b, 0x00000001
	.section .rom.00d33cd9, "ax"
	.incbin "baserom.gba", 0x00d33cd9, 0x00000003
	.section .rom.00d35cad, "ax"
	.incbin "baserom.gba", 0x00d35cad, 0x00000003
	.section .rom.00d35da1, "ax"
	.incbin "baserom.gba", 0x00d35da1, 0x00000003
	.section .rom.00d36e81, "ax"
	.incbin "baserom.gba", 0x00d36e81, 0x00000003
	.section .rom.00d38d5b, "ax"
	.incbin "baserom.gba", 0x00d38d5b, 0x00000001
	.section .rom.00d39d13, "ax"
	.incbin "baserom.gba", 0x00d39d13, 0x00000001
	.section .rom.00d3bc5a, "ax"
	.incbin "baserom.gba", 0x00d3bc5a, 0x00000002
	.section .rom.00d3d865, "ax"
	.incbin "baserom.gba", 0x00d3d865, 0x00000003
	.section .rom.00d3d9b1, "ax"
	.incbin "baserom.gba", 0x00d3d9b1, 0x00000003
	.section .rom.00d3fa6f, "ax"
	.incbin "baserom.gba", 0x00d3fa6f, 0x00000001
	.section .rom.00d43b5e, "ax"
	.incbin "baserom.gba", 0x00d43b5e, 0x00000002
	.section .rom.00d47910, "ax"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d47910, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d4981c, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4bb80, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4dbac, 0x0000180c
	.section .rom.00d50b05, "ax"
	.incbin "baserom.gba", 0x00d50b05, 0x00000003
	.section .rom.00d50c32, "ax"
	.incbin "baserom.gba", 0x00d50c32, 0x00000002
	.section .rom.00d57ba5, "ax"
	.incbin "baserom.gba", 0x00d57ba5, 0x00000003
	.section .rom.00d57caa, "ax"
	.incbin "baserom.gba", 0x00d57caa, 0x00000002
	.section .rom.00d57dea, "ax"
	.incbin "baserom.gba", 0x00d57dea, 0x00000002
	.section .rom.00d597b6, "ax"
	.incbin "baserom.gba", 0x00d597b6, 0x00000002
	.section .rom.00d598ad, "ax"
	.incbin "baserom.gba", 0x00d598ad, 0x00000003
	.section .rom.00d5c701, "ax"
	.incbin "baserom.gba", 0x00d5c701, 0x00000003
	.section .rom.00d5c8aa, "ax"
	.incbin "baserom.gba", 0x00d5c8aa, 0x00000002
	.section .rom.00d5f4c6, "ax"
	.incbin "baserom.gba", 0x00d5f4c6, 0x00000002
	.section .rom.00d621f9, "ax"
	.incbin "baserom.gba", 0x00d621f9, 0x00000003
	.section .rom.00d63ee3, "ax"
	.incbin "baserom.gba", 0x00d63ee3, 0x00000001
	.section .rom.00d6775d, "ax"
	.incbin "baserom.gba", 0x00d6775d, 0x00000003
	.section .rom.00d678b5, "ax"
	.incbin "baserom.gba", 0x00d678b5, 0x00000003
	.section .rom.00d69df9, "ax"
	.incbin "baserom.gba", 0x00d69df9, 0x00000003
	.section .rom.00d6dcbe, "ax"
	.incbin "baserom.gba", 0x00d6dcbe, 0x00000002
	.section .rom.00d6fdee, "ax"
	.incbin "baserom.gba", 0x00d6fdee, 0x00000002
	.section .rom.00d73ffa, "ax"
	.incbin "baserom.gba", 0x00d73ffa, 0x00000002
	.section .rom.00d741ce, "ax"
	.incbin "baserom.gba", 0x00d741ce, 0x00000002
	.section .rom.00d7620e, "ax"
	.incbin "baserom.gba", 0x00d7620e, 0x00000002
	.section .rom.00d77605, "ax"
	.incbin "baserom.gba", 0x00d77605, 0x00000003
	.section .rom.00d78142, "ax"
	.incbin "baserom.gba", 0x00d78142, 0x00000002
	.section .rom.00d792ad, "ax"
	.incbin "baserom.gba", 0x00d792ad, 0x00000003
	.section .rom.00d7a359, "ax"
	.incbin "baserom.gba", 0x00d7a359, 0x00000003
	.section .rom.00d7a505, "ax"
	.incbin "baserom.gba", 0x00d7a505, 0x00000003
	.section .rom.00d7c012, "ax"
	.incbin "baserom.gba", 0x00d7c012, 0x00000002
	.section .rom.00d7da2f, "ax"
	.incbin "baserom.gba", 0x00d7da2f, 0x00000001
	.section .rom.00d7ff01, "ax"
	.incbin "baserom.gba", 0x00d7ff01, 0x00000003
	.section .rom.00d81263, "ax"
	.incbin "baserom.gba", 0x00d81263, 0x00000001
	.section .rom.00d82117, "ax"
	.incbin "baserom.gba", 0x00d82117, 0x00000001
	.section .rom.00d83705, "ax"
	.incbin "baserom.gba", 0x00d83705, 0x00000003
	.section .rom.00d84c4b, "ax"
	.incbin "baserom.gba", 0x00d84c4b, 0x00000001
	.section .rom.00d85a15, "ax"
	.incbin "baserom.gba", 0x00d85a15, 0x00000003
	.section .rom.00d85b83, "ax"
	.incbin "baserom.gba", 0x00d85b83, 0x00000001
	.section .rom.00d86b42, "ax"
	.incbin "baserom.gba", 0x00d86b42, 0x00000002
	.section .rom.00d8760b, "ax"
	.incbin "baserom.gba", 0x00d8760b, 0x00000001
	.section .rom.00d881a5, "ax"
	.incbin "baserom.gba", 0x00d881a5, 0x00000003
	.section .rom.00d882f5, "ax"
	.incbin "baserom.gba", 0x00d882f5, 0x00000003
	.section .rom.00d8c5a3, "ax"
	.incbin "baserom.gba", 0x00d8c5a3, 0x00000001
	.section .rom.00d8e03d, "ax"
	.incbin "baserom.gba", 0x00d8e03d, 0x00000003
	.section .rom.00d8fa13, "ax"
	.incbin "baserom.gba", 0x00d8fa13, 0x00000001
	.section .rom.00d8fb92, "ax"
	.incbin "baserom.gba", 0x00d8fb92, 0x00000002
	.section .rom.00d937c2, "ax"
	.incbin "baserom.gba", 0x00d937c2, 0x00000002
	.section .rom.00d9551f, "ax"
	.incbin "baserom.gba", 0x00d9551f, 0x00000001
	.section .rom.00d98b49, "ax"
	.incbin "baserom.gba", 0x00d98b49, 0x00000003
	.section .rom.00d98c9a, "ax"
	.incbin "baserom.gba", 0x00d98c9a, 0x00000002
	.section .rom.00d9e896, "ax"
	.incbin "baserom.gba", 0x00d9e896, 0x00000002
	.section .rom.00da11d3, "ax"
	.incbin "baserom.gba", 0x00da11d3, 0x00000001
	.section .rom.00da2b15, "ax"
	.incbin "baserom.gba", 0x00da2b15, 0x00000003
	.section .rom.00da2c7e, "ax"
	.incbin "baserom.gba", 0x00da2c7e, 0x00000002
	.section .rom.00da9a7d, "ax"
	.incbin "baserom.gba", 0x00da9a7d, 0x00000003
	.section .rom.00daa05b, "ax"
	.incbin "baserom.gba", 0x00daa05b, 0x00000001
	.section .rom.00daaf63, "ax"
	.incbin "baserom.gba", 0x00daaf63, 0x00000001
	.section .rom.00dad7ab, "ax"
	.incbin "baserom.gba", 0x00dad7ab, 0x00000001
	.section .rom.00daedca, "ax"
	.incbin "baserom.gba", 0x00daedca, 0x00000002
	.section .rom.00db072b, "ax"
	.incbin "baserom.gba", 0x00db072b, 0x00000001
	.section .rom.00db3513, "ax"
	.incbin "baserom.gba", 0x00db3513, 0x00000001
	.section .rom.00db368e, "ax"
	.incbin "baserom.gba", 0x00db368e, 0x00000002
	.section .rom.00dc2857, "ax"
	.incbin "baserom.gba", 0x00dc2857, 0x00000001
	.section .rom.00dc29bf, "ax"
	.incbin "baserom.gba", 0x00dc29bf, 0x00000001
	.section .rom.00dc4fe2, "ax"
	.incbin "baserom.gba", 0x00dc4fe2, 0x00000002
	.section .rom.00dc5125, "ax"
	.incbin "baserom.gba", 0x00dc5125, 0x00000003
	.section .rom.00dc69e2, "ax"
	.incbin "baserom.gba", 0x00dc69e2, 0x00000002
	.section .rom.00dc9946, "ax"
	.incbin "baserom.gba", 0x00dc9946, 0x00000002
	.section .rom.00dcc2de, "ax"
	.incbin "baserom.gba", 0x00dcc2de, 0x00000002
	.section .rom.00dd0c36, "ax"
	.incbin "baserom.gba", 0x00dd0c36, 0x00000002
	.section .rom.00dd325f, "ax"
	.incbin "baserom.gba", 0x00dd325f, 0x00000001
	.section .rom.00dd3433, "ax"
	.incbin "baserom.gba", 0x00dd3433, 0x00000001
	.section .rom.00dd6752, "ax"
	.incbin "baserom.gba", 0x00dd6752, 0x00000002
	.section .rom.00dd6929, "ax"
	.incbin "baserom.gba", 0x00dd6929, 0x00000003
	.section .rom.00dd9c9d, "ax"
	.incbin "baserom.gba", 0x00dd9c9d, 0x00000003
	.section .rom.00ddb3f1, "ax"
	.incbin "baserom.gba", 0x00ddb3f1, 0x00000003
	.section .rom.00ddc37f, "ax"
	.incbin "baserom.gba", 0x00ddc37f, 0x00000001
	.section .rom.00dddcf5, "ax"
	.incbin "baserom.gba", 0x00dddcf5, 0x00000003
	.section .rom.00ddde2f, "ax"
	.incbin "baserom.gba", 0x00ddde2f, 0x00000001
	.section .rom.00ddf971, "ax"
	.incbin "baserom.gba", 0x00ddf971, 0x00000003
	.section .rom.00de2f49, "ax"
	.incbin "baserom.gba", 0x00de2f49, 0x00000003
	.section .rom.00de3bd3, "ax"
	.incbin "baserom.gba", 0x00de3bd3, 0x00000001
	.section .rom.00de3d13, "ax"
	.incbin "baserom.gba", 0x00de3d13, 0x00000001
	.section .rom.00de5ad7, "ax"
	.incbin "baserom.gba", 0x00de5ad7, 0x00000001
	.section .rom.00de5c4a, "ax"
	.incbin "baserom.gba", 0x00de5c4a, 0x00000002
	.section .rom.00de80b2, "ax"
	.incbin "baserom.gba", 0x00de80b2, 0x00000002
	.section .rom.00de9a4e, "ax"
	.incbin "baserom.gba", 0x00de9a4e, 0x00000002
	.section .rom.00deb883, "ax"
	.incbin "baserom.gba", 0x00deb883, 0x00000001
	.section .rom.00dec33a, "ax"
	.incbin "baserom.gba", 0x00dec33a, 0x00000002
	.section .rom.00dedd56, "ax"
	.incbin "baserom.gba", 0x00dedd56, 0x00000002
	.section .rom.00df0a22, "ax"
	.incbin "baserom.gba", 0x00df0a22, 0x00000002
	.section .rom.00df0bfd, "ax"
	.incbin "baserom.gba", 0x00df0bfd, 0x00000003
	.section .rom.00df2733, "ax"
	.incbin "baserom.gba", 0x00df2733, 0x00000001
	.section .rom.00df57fd, "ax"
	.incbin "baserom.gba", 0x00df57fd, 0x00000003
	.section .rom.00df69b9, "ax"
	.incbin "baserom.gba", 0x00df69b9, 0x00000003
	.section .rom.00df6ba1, "ax"
	.incbin "baserom.gba", 0x00df6ba1, 0x00000003
	.section .rom.00df6d33, "ax"
	.incbin "baserom.gba", 0x00df6d33, 0x00000001
	.section .rom.00df7aba, "ax"
	.incbin "baserom.gba", 0x00df7aba, 0x00000002
	.section .rom.00df7c4f, "ax"
	.incbin "baserom.gba", 0x00df7c4f, 0x00000001
	.section .rom.00df9d03, "ax"
	.incbin "baserom.gba", 0x00df9d03, 0x00000001
	.section .rom.00e01d86, "ax"
	.incbin "baserom.gba", 0x00e01d86, 0x00000002
	.section .rom.00e01ec7, "ax"
	.incbin "baserom.gba", 0x00e01ec7, 0x00000001
	.section .rom.00e0624f, "ax"
	.incbin "baserom.gba", 0x00e0624f, 0x00000001
	.section .rom.00e0696e, "ax"
	.incbin "baserom.gba", 0x00e0696e, 0x00000002
	.section .rom.00e07d2d, "ax"
	.incbin "baserom.gba", 0x00e07d2d, 0x00000003
	.section .rom.00e0be47, "ax"
	.incbin "baserom.gba", 0x00e0be47, 0x00000001
	.section .rom.00e0cec7, "ax"
	.incbin "baserom.gba", 0x00e0cec7, 0x00000001
	.section .rom.00e0df49, "ax"
	.incbin "baserom.gba", 0x00e0df49, 0x00000003
	.section .rom.00e0eaba, "ax"
	.incbin "baserom.gba", 0x00e0eaba, 0x00000002
	.section .rom.00e0fa0b, "ax"
	.incbin "baserom.gba", 0x00e0fa0b, 0x00000001
	.section .rom.00e0fb89, "ax"
	.incbin "baserom.gba", 0x00e0fb89, 0x00000003
	.section .rom.00e130e3, "ax"
	.incbin "baserom.gba", 0x00e130e3, 0x00000001
	.section .rom.00e14716, "ax"
	.incbin "baserom.gba", 0x00e14716, 0x00000002
	.section .rom.00e14857, "ax"
	.incbin "baserom.gba", 0x00e14857, 0x00000001
	.section .rom.00e171a2, "ax"
	.incbin "baserom.gba", 0x00e171a2, 0x00000002
	.section .rom.00e1fb4a, "ax"
	.incbin "baserom.gba", 0x00e1fb4a, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fb4c, 0x00000fdc
	.section .rom.00e226f5, "ax"
	.incbin "baserom.gba", 0x00e226f5, 0x00000003
	.section .rom.00e24635, "ax"
	.incbin "baserom.gba", 0x00e24635, 0x00000003
	.section .rom.00e25aa9, "ax"
	.incbin "baserom.gba", 0x00e25aa9, 0x00000003
	.section .rom.00e27763, "ax"
	.incbin "baserom.gba", 0x00e27763, 0x00000001
	.section .rom.00e34812, "ax"
	.incbin "baserom.gba", 0x00e34812, 0x00000002
	.section .rom.00e34951, "ax"
	.incbin "baserom.gba", 0x00e34951, 0x00000003
	.section .rom.00e36876, "ax"
	.incbin "baserom.gba", 0x00e36876, 0x00000002
	.section .rom.00e3699f, "ax"
	.incbin "baserom.gba", 0x00e3699f, 0x00000001
	.section .rom.00e38b6f, "ax"
	.incbin "baserom.gba", 0x00e38b6f, 0x00000001
	.section .rom.00e3b13f, "ax"
	.incbin "baserom.gba", 0x00e3b13f, 0x00000001
	.section .rom.00e3d451, "ax"
	.incbin "baserom.gba", 0x00e3d451, 0x00000003
	.section .rom.00e3dcc5, "ax"
	.incbin "baserom.gba", 0x00e3dcc5, 0x00000003
	.section .rom.00e3de07, "ax"
	.incbin "baserom.gba", 0x00e3de07, 0x00000001
	.section .rom.00e40ebd, "ax"
	.incbin "baserom.gba", 0x00e40ebd, 0x00000003
	.section .rom.00e425a3, "ax"
	.incbin "baserom.gba", 0x00e425a3, 0x00000001
	.section .rom.00e4416b, "ax"
	.incbin "baserom.gba", 0x00e4416b, 0x00000001
	.section .rom.00e45ce6, "ax"
	.incbin "baserom.gba", 0x00e45ce6, 0x00000002
	.section .rom.00e46131, "ax"
	.incbin "baserom.gba", 0x00e46131, 0x00000003
	.section .rom.00e4828e, "ax"
	.incbin "baserom.gba", 0x00e4828e, 0x00000002
	.section .rom.00e483a7, "ax"
	.incbin "baserom.gba", 0x00e483a7, 0x00000001
	.section .rom.00e491d1, "ax"
	.incbin "baserom.gba", 0x00e491d1, 0x00000003
	.section .rom.00e49333, "ax"
	.incbin "baserom.gba", 0x00e49333, 0x00000001
	.section .rom.00e4db9b, "ax"
	.incbin "baserom.gba", 0x00e4db9b, 0x00000001
	.section .rom.00e50259, "ax"
	.incbin "baserom.gba", 0x00e50259, 0x00000003
	.section .rom.00e50837, "ax"
	.incbin "baserom.gba", 0x00e50837, 0x00000001
	.section .rom.00e527ca, "ax"
	.incbin "baserom.gba", 0x00e527ca, 0x00000002
	.section .rom.00e53fae, "ax"
	.incbin "baserom.gba", 0x00e53fae, 0x00000002
	.section .rom.00e54112, "ax"
	.incbin "baserom.gba", 0x00e54112, 0x00000002
	.section .rom.00e5690a, "ax"
	.incbin "baserom.gba", 0x00e5690a, 0x00000002
	.section .rom.00e56ac3, "ax"
	.incbin "baserom.gba", 0x00e56ac3, 0x00000001
	.section .rom.00e56c4b, "ax"
	.incbin "baserom.gba", 0x00e56c4b, 0x00000001
	.section .rom.00e5846d, "ax"
	.incbin "baserom.gba", 0x00e5846d, 0x00000003
	.section .rom.00e59056, "ax"
	.incbin "baserom.gba", 0x00e59056, 0x00000002
	.section .rom.00e5a58f, "ax"
	.incbin "baserom.gba", 0x00e5a58f, 0x00000001
	.section .rom.00e5a72a, "ax"
	.incbin "baserom.gba", 0x00e5a72a, 0x00000002
	.section .rom.00e5c9d6, "ax"
	.incbin "baserom.gba", 0x00e5c9d6, 0x00000002
	.section .rom.00e5eb9d, "ax"
	.incbin "baserom.gba", 0x00e5eb9d, 0x00000003
	.section .rom.00e6029b, "ax"
	.incbin "baserom.gba", 0x00e6029b, 0x00000001
	.section .rom.00e60dbf, "ax"
	.incbin "baserom.gba", 0x00e60dbf, 0x00000001
	.section .rom.00e65c46, "ax"
	.incbin "baserom.gba", 0x00e65c46, 0x00000002
	.section .rom.00e67fed, "ax"
	.incbin "baserom.gba", 0x00e67fed, 0x00000003
	.section .rom.00e686d6, "ax"
	.incbin "baserom.gba", 0x00e686d6, 0x00000002
	.section .rom.00e692c2, "ax"
	.incbin "baserom.gba", 0x00e692c2, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e692c4, 0x00000ac4
	.section .rom.00e6b337, "ax"
	.incbin "baserom.gba", 0x00e6b337, 0x00000001
	.section .rom.00e6b4ea, "ax"
	.incbin "baserom.gba", 0x00e6b4ea, 0x00000002
	.section .rom.00e6d729, "ax"
	.incbin "baserom.gba", 0x00e6d729, 0x00000003
	.section .rom.00e6e24f, "ax"
	.incbin "baserom.gba", 0x00e6e24f, 0x00000001
	.section .rom.00e78cd8, "ax"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78cd8, 0x000007d0
	.section .rom.00e7c171, "ax"
	.incbin "baserom.gba", 0x00e7c171, 0x00000003
	.section .rom.00e7c2eb, "ax"
	.incbin "baserom.gba", 0x00e7c2eb, 0x00000001
	.section .rom.00e7c462, "ax"
	.incbin "baserom.gba", 0x00e7c462, 0x00000002
	.section .rom.00e7c5fa, "ax"
	.incbin "baserom.gba", 0x00e7c5fa, 0x00000002
	.section .rom.00e7c78b, "ax"
	.incbin "baserom.gba", 0x00e7c78b, 0x00000001
	.section .rom.00e7ea37, "ax"
	.incbin "baserom.gba", 0x00e7ea37, 0x00000001
	.section .rom.00e7eb4f, "ax"
	.incbin "baserom.gba", 0x00e7eb4f, 0x00000001
	.section .rom.00e80951, "ax"
	.incbin "baserom.gba", 0x00e80951, 0x00000003
	.section .rom.00e82416, "ax"
	.incbin "baserom.gba", 0x00e82416, 0x00000002
	.section .rom.00e82b3e, "ax"
	.incbin "baserom.gba", 0x00e82b3e, 0x00000002
	.section .rom.00e83f3a, "ax"
	.incbin "baserom.gba", 0x00e83f3a, 0x00000002
	.section .rom.00e84e0e, "ax"
	.incbin "baserom.gba", 0x00e84e0e, 0x00000002
	.section .rom.00e84f52, "ax"
	.incbin "baserom.gba", 0x00e84f52, 0x00000002
	.section .rom.00e87303, "ax"
	.incbin "baserom.gba", 0x00e87303, 0x00000001
	.section .rom.00e89b31, "ax"
	.incbin "baserom.gba", 0x00e89b31, 0x00000003
	.section .rom.00e89f2e, "ax"
	.incbin "baserom.gba", 0x00e89f2e, 0x00000002
	.section .rom.00e8b3ce, "ax"
	.incbin "baserom.gba", 0x00e8b3ce, 0x00000002
	.section .rom.00e8b522, "ax"
	.incbin "baserom.gba", 0x00e8b522, 0x00000002
	.section .rom.00e8ee6f, "ax"
	.incbin "baserom.gba", 0x00e8ee6f, 0x00000001
	.section .rom.00e902da, "ax"
	.incbin "baserom.gba", 0x00e902da, 0x00000002
	.section .rom.00e917b3, "ax"
	.incbin "baserom.gba", 0x00e917b3, 0x00000001
	.section .rom.00e91906, "ax"
	.incbin "baserom.gba", 0x00e91906, 0x00000002
	.section .rom.00e95313, "ax"
	.incbin "baserom.gba", 0x00e95313, 0x00000001
	.section .rom.00e964eb, "ax"
	.incbin "baserom.gba", 0x00e964eb, 0x00000001
	.section .rom.00e97cf6, "ax"
	.incbin "baserom.gba", 0x00e97cf6, 0x00000002
	.section .rom.00e97e46, "ax"
	.incbin "baserom.gba", 0x00e97e46, 0x00000002
	.section .rom.00e9a553, "ax"
	.incbin "baserom.gba", 0x00e9a553, 0x00000001
	.section .rom.00e9a6ba, "ax"
	.incbin "baserom.gba", 0x00e9a6ba, 0x00000002
	.section .rom.00e9d67b, "ax"
	.incbin "baserom.gba", 0x00e9d67b, 0x00000001
	.section .rom.00e9e8e9, "ax"
	.incbin "baserom.gba", 0x00e9e8e9, 0x00000003
	.section .rom.00ea1476, "ax"
	.incbin "baserom.gba", 0x00ea1476, 0x00000002
	.section .rom.00ea1e5d, "ax"
	.incbin "baserom.gba", 0x00ea1e5d, 0x00000003
	.section .rom.00ea27fe, "ax"
	.incbin "baserom.gba", 0x00ea27fe, 0x00000002
	.section .rom.00ea2972, "ax"
	.incbin "baserom.gba", 0x00ea2972, 0x00000002
	.section .rom.00ea4483, "ax"
	.incbin "baserom.gba", 0x00ea4483, 0x00000001
	.section .rom.00ea45a5, "ax"
	.incbin "baserom.gba", 0x00ea45a5, 0x00000003
	.section .rom.00ea70a3, "ax"
	.incbin "baserom.gba", 0x00ea70a3, 0x00000001
	.section .rom.00ea7e9a, "ax"
	.incbin "baserom.gba", 0x00ea7e9a, 0x00000002
	.section .rom.00ea9e39, "ax"
	.incbin "baserom.gba", 0x00ea9e39, 0x00000003
	.section .rom.00eab169, "ax"
	.incbin "baserom.gba", 0x00eab169, 0x00000003
	.section .rom.00eab2bd, "ax"
	.incbin "baserom.gba", 0x00eab2bd, 0x00000003
	.section .rom.00eabe6f, "ax"
	.incbin "baserom.gba", 0x00eabe6f, 0x00000001
	.section .rom.00ead649, "ax"
	.incbin "baserom.gba", 0x00ead649, 0x00000003
	.section .rom.00eae7a9, "ax"
	.incbin "baserom.gba", 0x00eae7a9, 0x00000003
	.section .rom.00eaf8c7, "ax"
	.incbin "baserom.gba", 0x00eaf8c7, 0x00000001
	.section .rom.00eaface, "ax"
	.incbin "baserom.gba", 0x00eaface, 0x00000002
	.section .rom.00eb07c3, "ax"
	.incbin "baserom.gba", 0x00eb07c3, 0x00000001
	.section .rom.00eb2f65, "ax"
	.incbin "baserom.gba", 0x00eb2f65, 0x00000003
	.section .rom.00eb46da, "ax"
	.incbin "baserom.gba", 0x00eb46da, 0x00000002
	.section .rom.00eb5997, "ax"
	.incbin "baserom.gba", 0x00eb5997, 0x00000001
	.section .rom.00eb61da, "ax"
	.incbin "baserom.gba", 0x00eb61da, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb61dc, 0x0000060c
	.section .rom.00eb6d5f, "ax"
	.incbin "baserom.gba", 0x00eb6d5f, 0x00000001
	.section .rom.00eb6e9f, "ax"
	.incbin "baserom.gba", 0x00eb6e9f, 0x00000001
	.section .rom.00eb6fdf, "ax"
	.incbin "baserom.gba", 0x00eb6fdf, 0x00000001
	.section .rom.00eb7b41, "ax"
	.incbin "baserom.gba", 0x00eb7b41, 0x00000003
	.section .rom.00eba2e5, "ax"
	.incbin "baserom.gba", 0x00eba2e5, 0x00000003
	.section .rom.00ebba5a, "ax"
	.incbin "baserom.gba", 0x00ebba5a, 0x00000002
	.section .rom.00ebcd17, "ax"
	.incbin "baserom.gba", 0x00ebcd17, 0x00000001
	.section .rom.00ebd55a, "ax"
	.incbin "baserom.gba", 0x00ebd55a, 0x00000002
	.section .rom.00ebdaf9, "ax"
	.incbin "baserom.gba", 0x00ebdaf9, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdafc, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdb08, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebdc58, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebdd98, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebded8, 0x00000140
	.section .rom.00ebe72d, "ax"
	.incbin "baserom.gba", 0x00ebe72d, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe730, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe73c, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebe88c, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebe9cc, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebeb0c, 0x00000140
	.section .rom.00ebf80d, "ax"
	.incbin "baserom.gba", 0x00ebf80d, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf810, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf81c, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebf96c, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebfaac, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebfbec, 0x00000140
	.section .rom.00ec02f9, "ax"
	.incbin "baserom.gba", 0x00ec02f9, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec02fc, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec0308, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec0458, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec0598, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec06d8, 0x00000140
	.section .rom.00ec1195, "ax"
	.incbin "baserom.gba", 0x00ec1195, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec1198, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec11a4, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec12f4, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec1434, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1574, 0x00000140
	.section .rom.00ec1add, "ax"
	.incbin "baserom.gba", 0x00ec1add, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1ae0, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1aec, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec1c3c, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1d7c, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec1ebc, 0x00000140
	.section .rom.00ec2625, "ax"
	.incbin "baserom.gba", 0x00ec2625, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec2628, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec2634, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2784, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec28c4, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2a04, 0x00000140
	.section .rom.00ec326d, "ax"
	.incbin "baserom.gba", 0x00ec326d, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec3270, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec327c, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec33cc, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec350c, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec364c, 0x00000140
	.global Resource_Overlay649
Resource_Overlay649:
	.incbin "baserom.gba", 0x00ec378c, 0x00000490
	.global Resource_Overlay64A
Resource_Overlay64A:
	.incbin "baserom.gba", 0x00ec3c1c, 0x00001c88
	.global Resource_Overlay64B
Resource_Overlay64B:
	.incbin "baserom.gba", 0x00ec58a4, 0x00003dd4
	.global Resource_Overlay64C
Resource_Overlay64C:
	.incbin "baserom.gba", 0x00ec9678, 0x00002df0
	.global Resource_Overlay64D
Resource_Overlay64D:
	.incbin "baserom.gba", 0x00ecc468, 0x000019ec
	.global Resource_Overlay64E
Resource_Overlay64E:
	.incbin "baserom.gba", 0x00ecde54, 0x00001198
	.global Resource_Overlay64F
Resource_Overlay64F:
	.incbin "baserom.gba", 0x00ecefec, 0x000007c8
	.global Resource_Overlay650
Resource_Overlay650:
	.incbin "baserom.gba", 0x00ecf7b4, 0x0000185c
	.global Resource_Overlay651
Resource_Overlay651:
	.incbin "baserom.gba", 0x00ed1010, 0x000009d0
	.global Resource_Overlay652
Resource_Overlay652:
	.incbin "baserom.gba", 0x00ed19e0, 0x00000b40
	.global Resource_Overlay653
Resource_Overlay653:
	.incbin "baserom.gba", 0x00ed2520, 0x000031e4
	.global Resource_Overlay654
Resource_Overlay654:
	.incbin "baserom.gba", 0x00ed5704, 0x000024c0
	.global Resource_Overlay655
Resource_Overlay655:
	.incbin "baserom.gba", 0x00ed7bc4, 0x00002da8
	.global Resource_Overlay656
Resource_Overlay656:
	.incbin "baserom.gba", 0x00eda96c, 0x00001cf0
	.global Resource_Overlay657
Resource_Overlay657:
	.incbin "baserom.gba", 0x00edc65c, 0x00001144
	.global Resource_Overlay658
Resource_Overlay658:
	.incbin "baserom.gba", 0x00edd7a0, 0x00001608
	.global Resource_Overlay659
Resource_Overlay659:
	.incbin "baserom.gba", 0x00ededa8, 0x00001490
	.global Resource_Overlay65A
Resource_Overlay65A:
	.incbin "baserom.gba", 0x00ee0238, 0x00000e44
	.global Resource_Overlay65B
Resource_Overlay65B:
	.incbin "baserom.gba", 0x00ee107c, 0x00000274
	.global Resource_Overlay65C
Resource_Overlay65C:
	.incbin "baserom.gba", 0x00ee12f0, 0x000001e4
	.global Resource_Overlay65D
Resource_Overlay65D:
	.incbin "baserom.gba", 0x00ee14d4, 0x000006e4
	.global Resource_Overlay65E
Resource_Overlay65E:
	.incbin "baserom.gba", 0x00ee1bb8, 0x0000056c
	.global Resource_Overlay65F
Resource_Overlay65F:
	.incbin "baserom.gba", 0x00ee2124, 0x000025a8
	.global Resource_Overlay660
Resource_Overlay660:
	.incbin "baserom.gba", 0x00ee46cc, 0x00000724
	.global Resource_Overlay661
Resource_Overlay661:
	.incbin "baserom.gba", 0x00ee4df0, 0x000031d4
	.global Resource_Overlay662
Resource_Overlay662:
	.incbin "baserom.gba", 0x00ee7fc4, 0x000014ac
	.global Resource_Overlay663
Resource_Overlay663:
	.incbin "baserom.gba", 0x00ee9470, 0x0000294c
	.global Resource_Overlay664
Resource_Overlay664:
	.incbin "baserom.gba", 0x00eebdbc, 0x00004134
	.global Resource_Overlay665
Resource_Overlay665:
	.incbin "baserom.gba", 0x00eefef0, 0x000014fc
	.global Resource_Overlay666
Resource_Overlay666:
	.incbin "baserom.gba", 0x00ef13ec, 0x00000a58
	.global Resource_Overlay667
Resource_Overlay667:
	.incbin "baserom.gba", 0x00ef1e44, 0x00000bf8
	.global Resource_Overlay668
Resource_Overlay668:
	.incbin "baserom.gba", 0x00ef2a3c, 0x00002988
	.global Resource_Overlay669
Resource_Overlay669:
	.incbin "baserom.gba", 0x00ef53c4, 0x0000145c
	.global Resource_Overlay66A
Resource_Overlay66A:
	.incbin "baserom.gba", 0x00ef6820, 0x00000d94
	.global Resource_Overlay66B
Resource_Overlay66B:
	.incbin "baserom.gba", 0x00ef75b4, 0x00001274
	.global Resource_Overlay66C
Resource_Overlay66C:
	.incbin "baserom.gba", 0x00ef8828, 0x000001d8
	.global Resource_Overlay66D
Resource_Overlay66D:
	.incbin "baserom.gba", 0x00ef8a00, 0x000005b0
	.global Resource_Overlay66E
Resource_Overlay66E:
	.incbin "baserom.gba", 0x00ef8fb0, 0x00000a9c
	.global Resource_Overlay66F
Resource_Overlay66F:
	.incbin "baserom.gba", 0x00ef9a4c, 0x00000de4
	.global Resource_Overlay670
Resource_Overlay670:
	.incbin "baserom.gba", 0x00efa830, 0x00003754
	.global Resource_Overlay671
Resource_Overlay671:
	.incbin "baserom.gba", 0x00efdf84, 0x00004050
	.global Resource_Overlay672
Resource_Overlay672:
	.incbin "baserom.gba", 0x00f01fd4, 0x000012b4
	.global Resource_Overlay673
Resource_Overlay673:
	.incbin "baserom.gba", 0x00f03288, 0x00000a6c
	.global Resource_Overlay674
Resource_Overlay674:
	.incbin "baserom.gba", 0x00f03cf4, 0x00002aac
	.global Resource_Overlay675
Resource_Overlay675:
	.incbin "baserom.gba", 0x00f067a0, 0x00000d94
	.global Resource_Overlay676
Resource_Overlay676:
	.incbin "baserom.gba", 0x00f07534, 0x000015a4
	.global Resource_Overlay677
Resource_Overlay677:
	.incbin "baserom.gba", 0x00f08ad8, 0x0000070c
	.global Resource_Overlay678
Resource_Overlay678:
	.incbin "baserom.gba", 0x00f091e4, 0x00001eb4
	.global Resource_Overlay679
Resource_Overlay679:
	.incbin "baserom.gba", 0x00f0b098, 0x000018e4
	.global Resource_Overlay67A
Resource_Overlay67A:
	.incbin "baserom.gba", 0x00f0c97c, 0x0000010c
	.global Resource_Overlay67B
Resource_Overlay67B:
	.incbin "baserom.gba", 0x00f0ca88, 0x00001fe0
	.global Resource_Overlay67C
Resource_Overlay67C:
	.incbin "baserom.gba", 0x00f0ea68, 0x00003364
	.global Resource_Overlay67D
Resource_Overlay67D:
	.incbin "baserom.gba", 0x00f11dcc, 0x00001164
	.global Resource_Overlay67E
Resource_Overlay67E:
	.incbin "baserom.gba", 0x00f12f30, 0x00000a7c
	.global Resource_Overlay67F
Resource_Overlay67F:
	.incbin "baserom.gba", 0x00f139ac, 0x00000fc0
	.global Resource_Overlay680
Resource_Overlay680:
	.incbin "baserom.gba", 0x00f1496c, 0x00000cfc
	.global Resource_Overlay681
Resource_Overlay681:
	.incbin "baserom.gba", 0x00f15668, 0x000036bc
	.global Resource_Overlay682
Resource_Overlay682:
	.incbin "baserom.gba", 0x00f18d24, 0x000014bc
	.global Resource_Overlay683
Resource_Overlay683:
	.incbin "baserom.gba", 0x00f1a1e0, 0x0000035c
	.global Resource_Overlay684
Resource_Overlay684:
	.incbin "baserom.gba", 0x00f1a53c, 0x000000d0
	.global Resource_Overlay685
Resource_Overlay685:
	.incbin "baserom.gba", 0x00f1a60c, 0x00001ff8
	.global Resource_Overlay686
Resource_Overlay686:
	.incbin "baserom.gba", 0x00f1c604, 0x00001fe8
	.global Resource_Overlay687
Resource_Overlay687:
	.incbin "baserom.gba", 0x00f1e5ec, 0x000016e0
	.global Resource_Overlay688
Resource_Overlay688:
	.incbin "baserom.gba", 0x00f1fccc, 0x000004a0
	.global Resource_Overlay689
Resource_Overlay689:
	.incbin "baserom.gba", 0x00f2016c, 0x000003c0
	.global Resource_Overlay68A
Resource_Overlay68A:
	.incbin "baserom.gba", 0x00f2052c, 0x00000c10
	.global Resource_Overlay68B
Resource_Overlay68B:
	.incbin "baserom.gba", 0x00f2113c, 0x000012e0
	.global Resource_Overlay68C
Resource_Overlay68C:
	.incbin "baserom.gba", 0x00f2241c, 0x00001228
	.global Resource_Overlay68D
Resource_Overlay68D:
	.incbin "baserom.gba", 0x00f23644, 0x00001c24
	.global Resource_Overlay68E
Resource_Overlay68E:
	.incbin "baserom.gba", 0x00f25268, 0x00000dfc
	.global Resource_Overlay68F
Resource_Overlay68F:
	.incbin "baserom.gba", 0x00f26064, 0x0000244c
	.global Resource_Overlay690
Resource_Overlay690:
	.incbin "baserom.gba", 0x00f284b0, 0x000018f4
	.global Resource_Overlay691
Resource_Overlay691:
	.incbin "baserom.gba", 0x00f29da4, 0x00002cc4
	.global Resource_Overlay692
Resource_Overlay692:
	.incbin "baserom.gba", 0x00f2ca68, 0x000039e8
	.global Resource_Overlay693
Resource_Overlay693:
	.incbin "baserom.gba", 0x00f30450, 0x00000be8
	.global Resource_Overlay694
Resource_Overlay694:
	.incbin "baserom.gba", 0x00f31038, 0x00002e04
	.global Resource_Overlay695
Resource_Overlay695:
	.incbin "baserom.gba", 0x00f33e3c, 0x00000b1c
	.global Resource_Overlay696
Resource_Overlay696:
	.incbin "baserom.gba", 0x00f34958, 0x00003314
	.global Resource_Overlay697
Resource_Overlay697:
	.incbin "baserom.gba", 0x00f37c6c, 0x00004450
	.global Resource_Overlay698
Resource_Overlay698:
	.incbin "baserom.gba", 0x00f3c0bc, 0x00001978
	.global Resource_Overlay699
Resource_Overlay699:
	.incbin "baserom.gba", 0x00f3da34, 0x00001af8
	.global Resource_Overlay69A
Resource_Overlay69A:
	.incbin "baserom.gba", 0x00f3f52c, 0x000019ac
	.global Resource_Overlay69B
Resource_Overlay69B:
	.incbin "baserom.gba", 0x00f40ed8, 0x00001f00
	.global Resource_Overlay69C
Resource_Overlay69C:
	.incbin "baserom.gba", 0x00f42dd8, 0x00002140
	.global Resource_Overlay69D
Resource_Overlay69D:
	.incbin "baserom.gba", 0x00f44f18, 0x00001cb0
	.global Resource_Overlay69E
Resource_Overlay69E:
	.incbin "baserom.gba", 0x00f46bc8, 0x0000246c
	.global Resource_Overlay69F
Resource_Overlay69F:
	.incbin "baserom.gba", 0x00f49034, 0x00001a88
	.global Resource_Overlay6A0
Resource_Overlay6A0:
	.incbin "baserom.gba", 0x00f4aabc, 0x000023b0
	.global Resource_Overlay6A1
Resource_Overlay6A1:
	.incbin "baserom.gba", 0x00f4ce6c, 0x0000205c
	.global Resource_Overlay6A2
Resource_Overlay6A2:
	.incbin "baserom.gba", 0x00f4eec8, 0x00001b4c
	.global Resource_Overlay6A3
Resource_Overlay6A3:
	.incbin "baserom.gba", 0x00f50a14, 0x000032b4
	.global Resource_Overlay6A4
Resource_Overlay6A4:
	.incbin "baserom.gba", 0x00f53cc8, 0x00002460
	.global Resource_Overlay6A5
Resource_Overlay6A5:
	.incbin "baserom.gba", 0x00f56128, 0x000037a0
	.global Resource_Overlay6A6
Resource_Overlay6A6:
	.incbin "baserom.gba", 0x00f598c8, 0x00000fd0
	.global Resource_Overlay6A7
Resource_Overlay6A7:
	.incbin "baserom.gba", 0x00f5a898, 0x00000814
	.global Resource_Overlay6A8
Resource_Overlay6A8:
	.incbin "baserom.gba", 0x00f5b0ac, 0x00002d18
	.global Resource_Overlay6A9
Resource_Overlay6A9:
	.incbin "baserom.gba", 0x00f5ddc4, 0x00000d70
	.global Resource_Overlay6AA
Resource_Overlay6AA:
	.incbin "baserom.gba", 0x00f5eb34, 0x00003ba0
	.global Resource_Overlay6AB
Resource_Overlay6AB:
	.incbin "baserom.gba", 0x00f626d4, 0x00002de4
	.global Resource_Overlay6AC
Resource_Overlay6AC:
	.incbin "baserom.gba", 0x00f654b8, 0x00002210
	.global Resource_Overlay6AD
Resource_Overlay6AD:
	.incbin "baserom.gba", 0x00f676c8, 0x00004480
	.global Resource_Overlay6AE
Resource_Overlay6AE:
	.incbin "baserom.gba", 0x00f6bb48, 0x00003248
	.global Resource_Overlay6AF
Resource_Overlay6AF:
	.incbin "baserom.gba", 0x00f6ed90, 0x00000d80
	.global Resource_Overlay6B0
Resource_Overlay6B0:
	.incbin "baserom.gba", 0x00f6fb10, 0x000028b4
	.global Resource_Overlay6B1
Resource_Overlay6B1:
	.incbin "baserom.gba", 0x00f723c4, 0x00001594
	.global Resource_Overlay6B2
Resource_Overlay6B2:
	.incbin "baserom.gba", 0x00f73958, 0x00000430
	.global Resource_Overlay6B3
Resource_Overlay6B3:
	.incbin "baserom.gba", 0x00f73d88, 0x00000554
	.global Resource_Overlay6B4
Resource_Overlay6B4:
	.incbin "baserom.gba", 0x00f742dc, 0x00000e50
	.global Resource_Overlay6B5
Resource_Overlay6B5:
	.incbin "baserom.gba", 0x00f7512c, 0x000013e8
	.global Resource_Overlay6B6
Resource_Overlay6B6:
	.incbin "baserom.gba", 0x00f76514, 0x000000b0
	.global Resource_Overlay6B7
Resource_Overlay6B7:
	.incbin "baserom.gba", 0x00f765c4, 0x00000770
	.global Resource_Overlay6B8
Resource_Overlay6B8:
	.incbin "baserom.gba", 0x00f76d34, 0x00000ce0
	.global Resource_Overlay6B9
Resource_Overlay6B9:
	.incbin "baserom.gba", 0x00f77a14, 0x00000518
	.global Resource_Overlay6BA
Resource_Overlay6BA:
	.incbin "baserom.gba", 0x00f77f2c, 0x000880d4
