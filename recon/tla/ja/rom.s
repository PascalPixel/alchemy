@ tla-ja's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.section .rom.0001471c, "ax"
	.global Scheduler_SetCallbackMask
	.type Scheduler_SetCallbackMask, %function
	.thumb_func
Scheduler_SetCallbackMask:
	.incbin "baserom.gba", 0x0001471c, 0x00000040
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
	.section .rom.00014de0, "ax"
	@ Uncredited four-byte tail; independent purpose is unproved.
	.incbin "baserom.gba", 0x00014de0, 0x00000004
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
	.global Resource_DecodeType01
	.type Resource_DecodeType01, %function
	.thumb_func
Resource_DecodeType01:
	.incbin "baserom.gba", 0x0001587c, 0x000000a0
	.section .rom.0001591c, "ax"
	.global Resource_DecodeByteLzInRam
	.type Resource_DecodeByteLzInRam, %function
	.thumb_func
Resource_DecodeByteLzInRam:
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
	.global SerialRuntime_RemoveIrqHandlers
	.type SerialRuntime_RemoveIrqHandlers, %function
	.thumb_func
SerialRuntime_RemoveIrqHandlers:
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
	.incbin "baserom.gba", 0x00024f80, 0x00000140
	.section .rom.000250f2, "ax"
	.incbin "baserom.gba", 0x000250f2, 0x00000042
	.section .rom.00025160, "ax"
	.incbin "baserom.gba", 0x00025160, 0x000009f8
	.section .rom.00025bb4, "ax"
	.incbin "baserom.gba", 0x00025bb4, 0x00000030
	.section .rom.00025c8a, "ax"
	.incbin "baserom.gba", 0x00025c8a, 0x00000002
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
	.incbin "baserom.gba", 0x0002de08, 0x00000cc0
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0002eac8, 0x000000fc
	.global Data_0802ec48
Data_0802ec48:
	.incbin "baserom.gba", 0x0002ebc4, 0x0000027c
	.global Data_0802eec4
Data_0802eec4:
	.incbin "baserom.gba", 0x0002ee40, 0x00000100
	.global Map_TerrainHeightHandlers
Map_TerrainHeightHandlers:
	.incbin "baserom.gba", 0x0002ef40, 0x00000040
	.incbin "baserom.gba", 0x0002ef80, 0x000001cc
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
	.incbin "baserom.gba", 0x00039504, 0x00000190
	.global Ui_ClearVramBlock
	.type Ui_ClearVramBlock, %function
	.thumb_func
Ui_ClearVramBlock:
	.incbin "baserom.gba", 0x00039694, 0x0000003c
	.section .rom.0003971e, "ax"
	.incbin "baserom.gba", 0x0003971e, 0x000008fa
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
	.incbin "baserom.gba", 0x0003a740, 0x00000304
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
	.section .rom.0003cc5e, "ax"
	.incbin "baserom.gba", 0x0003cc5e, 0x00000102
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
	.incbin "baserom.gba", 0x0003dfdc, 0x00000004
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
	.incbin "baserom.gba", 0x0003f7f4, 0x00000098
	.global Resource_ResetPendingTransfer
	.type Resource_ResetPendingTransfer, %function
	.thumb_func
Resource_ResetPendingTransfer:
	.incbin "baserom.gba", 0x0003f88c, 0x00000020
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
	.incbin "baserom.gba", 0x00043208, 0x000000ec
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x000432f4, 0x000000ac
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
	.section .rom.00043e1c, "ax"
	.incbin "baserom.gba", 0x00043e1c, 0x000007ec
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
	.incbin "baserom.gba", 0x00044fc4, 0x00000184
	.section .rom.00045184, "ax"
	.incbin "baserom.gba", 0x00045184, 0x000000c0
	.section .rom.00045244, "ax"
	.global RenderResource_LoadFrame
	.type RenderResource_LoadFrame, %function
	.thumb_func
RenderResource_LoadFrame:
	.incbin "baserom.gba", 0x00045244, 0x00000064
	.section .rom.000452f8, "ax"
	.incbin "baserom.gba", 0x000452f8, 0x00000150
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
	.incbin "baserom.gba", 0x0004d360, 0x000001d8
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
	.incbin "baserom.gba", 0x0004d5dc, 0x0000009c
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
	.section .rom.0004dba2, "ax"
	.incbin "baserom.gba", 0x0004dba2, 0x00000002
	.section .rom.0004dbde, "ax"
	.incbin "baserom.gba", 0x0004dbde, 0x00000002
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
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x00054c14, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055024, 0x00004b50
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b74, 0x00000d58
	.section .rom.0005cdcc, "ax"
	.incbin "baserom.gba", 0x0005cdcc, 0x000034a4
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00060270, 0x00000170
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x000603e0, 0x000000f7
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000604d7, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000604e3, 0x0000000c
	.incbin "baserom.gba", 0x000604ef, 0x00000055
	.section .rom.0009d0d0, "ax"
	.incbin "baserom.gba", 0x0009d0d0, 0x00000594
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0009d664, 0x0000a99c
	.section .rom.000a8070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000a8070, 0x00000008
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
	.incbin "baserom.gba", 0x000a8088, 0x00000008
	.global Func_080ad090
	.type Func_080ad090, %function
	.thumb_func
Func_080ad090:
	.incbin "baserom.gba", 0x000a8090, 0x00000008
	.global Func_080ad098
	.type Func_080ad098, %function
	.thumb_func
Func_080ad098:
	.incbin "baserom.gba", 0x000a8098, 0x00000008
	.global Func_080ad0a0
	.type Func_080ad0a0, %function
	.thumb_func
Func_080ad0a0:
	.incbin "baserom.gba", 0x000a80a0, 0x00000008
	.global Func_080ad0a8
	.type Func_080ad0a8, %function
	.thumb_func
Func_080ad0a8:
	.incbin "baserom.gba", 0x000a80a8, 0x00000008
	.section .rom.000a80c0, "ax"
	.global Owner_AdjustFirstValueFar
	.type Owner_AdjustFirstValueFar, %function
	.thumb_func
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000a80c0, 0x00000008
	.global Owner_AdjustSecondValueFar
	.type Owner_AdjustSecondValueFar, %function
	.thumb_func
Owner_AdjustSecondValueFar:
	.incbin "baserom.gba", 0x000a80c8, 0x00000008
	.global Owner_RecalculateRatiosFar
	.type Owner_RecalculateRatiosFar, %function
	.thumb_func
Owner_RecalculateRatiosFar:
	.incbin "baserom.gba", 0x000a80d0, 0x00000008
	.section .rom.000a80d8, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000a80d8, 0x00000008
	.global Owner_UpdateSecondInputAndRatiosFar
	.type Owner_UpdateSecondInputAndRatiosFar, %function
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000a80e0, 0x00000008
	.global BattleUnit_AssignFar
	.type BattleUnit_AssignFar, %function
	.thumb_func
BattleUnit_AssignFar:
	.incbin "baserom.gba", 0x000a80e8, 0x00000008
	.section .rom.000a80f0, "ax"
	.global Party_CountActiveOwnersFar
	.type Party_CountActiveOwnersFar, %function
	.thumb_func
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000a80f0, 0x00000008
	.section .rom.000a80f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000a80f8, 0x00000008
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000a8100, 0x00000008
	.global Func_080ad108
	.type Func_080ad108, %function
	.thumb_func
Func_080ad108:
	.incbin "baserom.gba", 0x000a8108, 0x00000008
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000a8110, 0x00000008
	.global Func_080ad118
	.type Func_080ad118, %function
	.thumb_func
Func_080ad118:
	.incbin "baserom.gba", 0x000a8118, 0x00000008
	.global Battle_HitCheck
	.type Battle_HitCheck, %function
	.thumb_func
Battle_HitCheck:
	.incbin "baserom.gba", 0x000a8120, 0x00000008
	.global Battle_CalcAttack
	.type Battle_CalcAttack, %function
	.thumb_func
Battle_CalcAttack:
	.incbin "baserom.gba", 0x000a8128, 0x00000008
	.global Battle_CalcPower
	.type Battle_CalcPower, %function
	.thumb_func
Battle_CalcPower:
	.incbin "baserom.gba", 0x000a8130, 0x00000008
	.global Battle_CalcRestore
	.type Battle_CalcRestore, %function
	.thumb_func
Battle_CalcRestore:
	.incbin "baserom.gba", 0x000a8138, 0x00000008
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000a8140, 0x00000008
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
	.incbin "baserom.gba", 0x000a8158, 0x00000008
	.global Djinn_DeactivateFar
	.type Djinn_DeactivateFar, %function
	.thumb_func
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000a8160, 0x00000008
	.section .rom.000a8168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000a8168, 0x00000008
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000a8170, 0x00000008
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000a8178, 0x00000008
	.global Func_080ad180
	.type Func_080ad180, %function
	.thumb_func
Func_080ad180:
	.incbin "baserom.gba", 0x000a8180, 0x00000008
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000a8188, 0x00000008
	.global Djinn_GetDefinitionHeaderFar
	.type Djinn_GetDefinitionHeaderFar, %function
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000a8190, 0x00000008
	.global Party_AdvanceOwnerCountToTargetFar
	.type Party_AdvanceOwnerCountToTargetFar, %function
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000a8198, 0x00000008
	.global Func_080ad1a0
	.type Func_080ad1a0, %function
	.thumb_func
Func_080ad1a0:
	.incbin "baserom.gba", 0x000a81a0, 0x00000008
	.global Trade_CountPendingOffersFar
	.type Trade_CountPendingOffersFar, %function
	.thumb_func
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000a81a8, 0x00000008
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000a81b0, 0x00000008
	.global Trade_CanOfferDjinnFar
	.type Trade_CanOfferDjinnFar, %function
	.thumb_func
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000a81b8, 0x00000008
	.section .rom.000a81c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000a81c0, 0x00000008
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000a81c8, 0x00000008
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000a81d0, 0x00000008
	.global Party_AdjustSixDigitCounterAFar
	.type Party_AdjustSixDigitCounterAFar, %function
	.thumb_func
Party_AdjustSixDigitCounterAFar:
	.incbin "baserom.gba", 0x000a81d8, 0x00000008
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000a81e0, 0x00000008
	.global Item_AdjustCounterFar
	.type Item_AdjustCounterFar, %function
	.thumb_func
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000a81e8, 0x00000008
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x000a81f0, 0x00000008
	.global PartyInventory_HasSpaceFar
	.type PartyInventory_HasSpaceFar, %function
	.thumb_func
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000a81f8, 0x00000008
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000a8200, 0x00000008
	.global Func_080ad208
	.type Func_080ad208, %function
	.thumb_func
Func_080ad208:
	.incbin "baserom.gba", 0x000a8208, 0x00000008
	.global Func_080ad210
	.type Func_080ad210, %function
	.thumb_func
Func_080ad210:
	.incbin "baserom.gba", 0x000a8210, 0x00000008
	.global Func_080ad218
	.type Func_080ad218, %function
	.thumb_func
Func_080ad218:
	.incbin "baserom.gba", 0x000a8218, 0x00000008
	.global Func_080ad220
	.type Func_080ad220, %function
	.thumb_func
Func_080ad220:
	.incbin "baserom.gba", 0x000a8220, 0x00000008
	.global Func_080ad228
	.type Func_080ad228, %function
	.thumb_func
Func_080ad228:
	.incbin "baserom.gba", 0x000a8228, 0x00000008
	.global Func_080ad230
	.type Func_080ad230, %function
	.thumb_func
Func_080ad230:
	.incbin "baserom.gba", 0x000a8230, 0x00000008
	.global Func_080ad238
	.type Func_080ad238, %function
	.thumb_func
Func_080ad238:
	.incbin "baserom.gba", 0x000a8238, 0x00000008
	.global Func_080ad240
	.type Func_080ad240, %function
	.thumb_func
Func_080ad240:
	.incbin "baserom.gba", 0x000a8240, 0x00000008
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000a8248, 0x00000008
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000a8250, 0x00000008
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000a8258, 0x00000008
	.global Party_AdjustCounterCappedAt28Far
	.type Party_AdjustCounterCappedAt28Far, %function
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000a8260, 0x00000008
	.global Func_080ad268
	.type Func_080ad268, %function
	.thumb_func
Func_080ad268:
	.incbin "baserom.gba", 0x000a8268, 0x00000008
	.global Func_080ad270
	.type Func_080ad270, %function
	.thumb_func
Func_080ad270:
	.incbin "baserom.gba", 0x000a8270, 0x00000008
	.global Inventory_DiscardFar
	.type Inventory_DiscardFar, %function
	.thumb_func
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000a8278, 0x00000008
	.section .rom.000a8280, "ax"
	.global BattleFx_IsReviveFar
	.type BattleFx_IsReviveFar, %function
	.thumb_func
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000a8280, 0x00000008
	.global Owner_RefreshClassActionsFar
	.type Owner_RefreshClassActionsFar, %function
	.thumb_func
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000a8288, 0x00000008
	.global Func_080ad290
	.type Func_080ad290, %function
	.thumb_func
Func_080ad290:
	.incbin "baserom.gba", 0x000a8290, 0x00000008
	.global Owner_RefreshDerivedDataFar
	.type Owner_RefreshDerivedDataFar, %function
	.thumb_func
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000a8298, 0x00000008
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000a82a0, 0x00000008
	.global PartyInventory_CountItemFar
	.type PartyInventory_CountItemFar, %function
	.thumb_func
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000a82a8, 0x00000008
	.global Func_080ad2b0
	.type Func_080ad2b0, %function
	.thumb_func
Func_080ad2b0:
	.incbin "baserom.gba", 0x000a82b0, 0x00000008
	.global Func_080ad2b8
	.type Func_080ad2b8, %function
	.thumb_func
Func_080ad2b8:
	.incbin "baserom.gba", 0x000a82b8, 0x00000008
	.global Equipment_GetUnleashRateBonusFar
	.type Equipment_GetUnleashRateBonusFar, %function
	.thumb_func
Equipment_GetUnleashRateBonusFar:
	.incbin "baserom.gba", 0x000a82c0, 0x00000008
	.global Func_080ad2c8
	.type Func_080ad2c8, %function
	.thumb_func
Func_080ad2c8:
	.incbin "baserom.gba", 0x000a82c8, 0x00000008
	.global PartyInventory_CountFreeSlotsFar
	.type PartyInventory_CountFreeSlotsFar, %function
	.thumb_func
PartyInventory_CountFreeSlotsFar:
	.incbin "baserom.gba", 0x000a82d0, 0x00000008
	.global Func_080ad2d8
	.type Func_080ad2d8, %function
	.thumb_func
Func_080ad2d8:
	.incbin "baserom.gba", 0x000a82d8, 0x00000008
	.global Func_080ad2e0
	.type Func_080ad2e0, %function
	.thumb_func
Func_080ad2e0:
	.incbin "baserom.gba", 0x000a82e0, 0x00000008
	.global Func_080ad2e8
	.type Func_080ad2e8, %function
	.thumb_func
Func_080ad2e8:
	.incbin "baserom.gba", 0x000a82e8, 0x00000008
	.global Func_080ad2f0
	.type Func_080ad2f0, %function
	.thumb_func
Func_080ad2f0:
	.incbin "baserom.gba", 0x000a82f0, 0x00000008
	.global Func_080ad2f8
	.type Func_080ad2f8, %function
	.thumb_func
Func_080ad2f8:
	.incbin "baserom.gba", 0x000a82f8, 0x00000008
	.global Func_080ad300
	.type Func_080ad300, %function
	.thumb_func
Func_080ad300:
	.incbin "baserom.gba", 0x000a8300, 0x00000008
	.global Func_080ad308
	.type Func_080ad308, %function
	.thumb_func
Func_080ad308:
	.incbin "baserom.gba", 0x000a8308, 0x00000008
	.global Func_080ad310
	.type Func_080ad310, %function
	.thumb_func
Func_080ad310:
	.incbin "baserom.gba", 0x000a8310, 0x00000008
	.global Func_080ad318
	.type Func_080ad318, %function
	.thumb_func
Func_080ad318:
	.incbin "baserom.gba", 0x000a8318, 0x00000008
	.global Func_080ad320
	.type Func_080ad320, %function
	.thumb_func
Func_080ad320:
	.incbin "baserom.gba", 0x000a8320, 0x00000008
	.global Owner_SumDjinnCountsFar
	.type Owner_SumDjinnCountsFar, %function
	.thumb_func
Owner_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000a8328, 0x00000008
	.global Func_080ad330
	.type Func_080ad330, %function
	.thumb_func
Func_080ad330:
	.incbin "baserom.gba", 0x000a8330, 0x00000008
	.global Func_080ad338
	.type Func_080ad338, %function
	.thumb_func
Func_080ad338:
	.incbin "baserom.gba", 0x000a8338, 0x00000010
	.section .rom.000a8348, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x000a8348, 0x00000018
	.section .rom.000a83a8, "ax"
	.incbin "baserom.gba", 0x000a83a8, 0x0000001c
	.section .rom.000a83f6, "ax"
	.incbin "baserom.gba", 0x000a83f6, 0x00000002
	.section .rom.000a83f8, "ax"
	.global Owner_RecalculateStats
	.type Owner_RecalculateStats, %function
	.thumb_func
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000a83f8, 0x000007f4
	.global Func_080adbec
	.type Func_080adbec, %function
	.thumb_func
Func_080adbec:
	.incbin "baserom.gba", 0x000a8bec, 0x000000a4
	.global Func_080adc90
	.type Func_080adc90, %function
	.thumb_func
Func_080adc90:
	.incbin "baserom.gba", 0x000a8c90, 0x000000cc
	.global Func_080add5c
	.type Func_080add5c, %function
	.thumb_func
Func_080add5c:
	.incbin "baserom.gba", 0x000a8d5c, 0x00000018
	.global Func_080add74
	.type Func_080add74, %function
	.thumb_func
Func_080add74:
	.incbin "baserom.gba", 0x000a8d74, 0x0000007c
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000a8df0, 0x00000a40
	.section .rom.000a9864, "ax"
	.incbin "baserom.gba", 0x000a9864, 0x000001c8
	.section .rom.000a9c62, "ax"
	.incbin "baserom.gba", 0x000a9c62, 0x00000002
	.section .rom.000a9c64, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000a9c64, 0x0000003c
	.global Inventory_GetQuantity
	.type Inventory_GetQuantity, %function
	.thumb_func
Inventory_GetQuantity:
	.incbin "baserom.gba", 0x000a9ca0, 0x00000024
	.section .rom.000a9e3c, "ax"
	.incbin "baserom.gba", 0x000a9e3c, 0x00000058
	.section .rom.000a9e94, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x000a9e94, 0x00000030
	.section .rom.000a9f30, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
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
	.global Item_GetTargetMode
	.type Item_GetTargetMode, %function
	.thumb_func
Item_GetTargetMode:
	.incbin "baserom.gba", 0x000aa334, 0x00000040
	.section .rom.000aa374, "ax"
	.global Item_AdjustCounter
	.type Item_AdjustCounter, %function
	.thumb_func
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000aa374, 0x00000028
	.section .rom.000aa438, "ax"
	.global BattleAction_GetDirect
	.type BattleAction_GetDirect, %function
	.thumb_func
BattleAction_GetDirect:
	.incbin "baserom.gba", 0x000aa438, 0x000002b8
	.section .rom.000aa78e, "ax"
	.incbin "baserom.gba", 0x000aa78e, 0x0000000a
	.section .rom.000aa7a8, "ax"
	.incbin "baserom.gba", 0x000aa7a8, 0x00000124
	.global Owner_GetLevelThreshold
	.type Owner_GetLevelThreshold, %function
	.thumb_func
Owner_GetLevelThreshold:
	.incbin "baserom.gba", 0x000aa8cc, 0x0000004c
	.section .rom.000aa918, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000aa918, 0x00000264
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000aab7c, 0x00000034
	.section .rom.000aabe8, "ax"
	.incbin "baserom.gba", 0x000aabe8, 0x000001d0
	.section .rom.000aadd4, "ax"
	.incbin "baserom.gba", 0x000aadd4, 0x00000044
	.section .rom.000aaeac, "ax"
	.incbin "baserom.gba", 0x000aaeac, 0x00000024
	.section .rom.000aaf24, "ax"
	.incbin "baserom.gba", 0x000aaf24, 0x00000054
	.section .rom.000aaf90, "ax"
	.incbin "baserom.gba", 0x000aaf90, 0x00000018
	.section .rom.000aafa8, "ax"
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000aafa8, 0x0000007c
	.section .rom.000ab056, "ax"
	.incbin "baserom.gba", 0x000ab056, 0x0000022e
	.section .rom.000ab294, "ax"
	.incbin "baserom.gba", 0x000ab294, 0x0000003c
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x000ab2d0, 0x0000018c
	.section .rom.000ab4b6, "ax"
	.incbin "baserom.gba", 0x000ab4b6, 0x000005da
	.section .rom.000abbb2, "ax"
	.incbin "baserom.gba", 0x000abbb2, 0x000000c2
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
	.incbin "baserom.gba", 0x000abffe, 0x0000029a
	.global Owner_ExperienceThresholds
Owner_ExperienceThresholds:
	.incbin "baserom.gba", 0x000ac298, 0x0000109c
	.section .rom.000ad334, "ax"
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000ad334, 0x000058b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x000b2be4, 0x00009338
	.global Owner_GrowthRecords
Owner_GrowthRecords:
	.incbin "baserom.gba", 0x000bbf1c, 0x000005c0
	.section .rom.000bc4dc, "ax"
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000bc4dc, 0x000000e8
	.section .rom.000bc5c4, "ax"
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000bc5c4, 0x000055bc
	.section .rom.000c1b80, "ax"
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000c1b80, 0x00006480
	.section .rom.000c8000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000c8000, 0x00000008
	.global Game_ResetForNewGameFar
	.type Game_ResetForNewGameFar, %function
	.thumb_func
Game_ResetForNewGameFar:
	.incbin "baserom.gba", 0x000c8008, 0x00000008
	.global Battle_WaitMode0Far
	.type Battle_WaitMode0Far, %function
	.thumb_func
Battle_WaitMode0Far:
	.incbin "baserom.gba", 0x000c8010, 0x00000008
	.global Func_080c8018
	.type Func_080c8018, %function
	.thumb_func
Func_080c8018:
	.incbin "baserom.gba", 0x000c8018, 0x00000008
	.global Func_080c8020
	.type Func_080c8020, %function
	.thumb_func
Func_080c8020:
	.incbin "baserom.gba", 0x000c8020, 0x00000008
	.global Event_RunObjectHookAndWaitFar
	.type Event_RunObjectHookAndWaitFar, %function
	.thumb_func
Event_RunObjectHookAndWaitFar:
	.incbin "baserom.gba", 0x000c8028, 0x00000008
	.global Event_CallWithLastActiveObjectIdFar
	.type Event_CallWithLastActiveObjectIdFar, %function
	.thumb_func
Event_CallWithLastActiveObjectIdFar:
	.incbin "baserom.gba", 0x000c8030, 0x00000008
	.global Scene_AssignViewFlagsFar
	.type Scene_AssignViewFlagsFar, %function
	.thumb_func
Scene_AssignViewFlagsFar:
	.incbin "baserom.gba", 0x000c8038, 0x00000008
	.global Event_SetWorkWord10Far
	.type Event_SetWorkWord10Far, %function
	.thumb_func
Event_SetWorkWord10Far:
	.incbin "baserom.gba", 0x000c8040, 0x00000008
	.global Event_PrepareObjectAndApplyValueFar
	.type Event_PrepareObjectAndApplyValueFar, %function
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	.incbin "baserom.gba", 0x000c8048, 0x00000008
	.global Event_PrepareTwoObjectsAndApplyFar
	.type Event_PrepareTwoObjectsAndApplyFar, %function
	.thumb_func
Event_PrepareTwoObjectsAndApplyFar:
	.incbin "baserom.gba", 0x000c8050, 0x00000008
	.global Party_RemoveOwnerRestoredFar
	.type Party_RemoveOwnerRestoredFar, %function
	.thumb_func
Party_RemoveOwnerRestoredFar:
	.incbin "baserom.gba", 0x000c8058, 0x00000008
	.global PartyInventory_GiveItemFar
	.type PartyInventory_GiveItemFar, %function
	.thumb_func
PartyInventory_GiveItemFar:
	.incbin "baserom.gba", 0x000c8060, 0x00000008
	.global Inventory_TryAddAndReturnOwnerFar
	.type Inventory_TryAddAndReturnOwnerFar, %function
	.thumb_func
Inventory_TryAddAndReturnOwnerFar:
	.incbin "baserom.gba", 0x000c8068, 0x00000008
	.global Inventory_PromptAndSetObjectModeFar
	.type Inventory_PromptAndSetObjectModeFar, %function
	.thumb_func
Inventory_PromptAndSetObjectModeFar:
	.incbin "baserom.gba", 0x000c8070, 0x00000008
	.global Func_080c8078
	.type Func_080c8078, %function
	.thumb_func
Func_080c8078:
	.incbin "baserom.gba", 0x000c8078, 0x00000008
	.global UiText_DrawQuantityPairWithCueFar
	.type UiText_DrawQuantityPairWithCueFar, %function
	.thumb_func
UiText_DrawQuantityPairWithCueFar:
	.incbin "baserom.gba", 0x000c8080, 0x00000008
	.section .rom.000c82a8, "ax"
	.incbin "baserom.gba", 0x000c82a8, 0x00000018
	.global Func_080c82c0
	.type Func_080c82c0, %function
	.thumb_func
Func_080c82c0:
	.incbin "baserom.gba", 0x000c82c0, 0x00000018
	.global Func_080c82d8
	.type Func_080c82d8, %function
	.thumb_func
Func_080c82d8:
	.incbin "baserom.gba", 0x000c82d8, 0x00000008
	.global Func_080c82e0
	.type Func_080c82e0, %function
	.thumb_func
Func_080c82e0:
	.incbin "baserom.gba", 0x000c82e0, 0x00000008
	.global Func_080c82e8
	.type Func_080c82e8, %function
	.thumb_func
Func_080c82e8:
	.incbin "baserom.gba", 0x000c82e8, 0x00000010
	.global Func_080c82f8
	.type Func_080c82f8, %function
	.thumb_func
Func_080c82f8:
	.incbin "baserom.gba", 0x000c82f8, 0x00000038
	.global Func_080c8330
	.type Func_080c8330, %function
	.thumb_func
Func_080c8330:
	.incbin "baserom.gba", 0x000c8330, 0x00000020
	.global Func_080c8350
	.type Func_080c8350, %function
	.thumb_func
Func_080c8350:
	.incbin "baserom.gba", 0x000c8350, 0x00000018
	.global Func_080c8368
	.type Func_080c8368, %function
	.thumb_func
Func_080c8368:
	.incbin "baserom.gba", 0x000c8368, 0x00000010
	.section .rom.000c83a8, "ax"
	.global Event_SetStatus1c6Far
	.type Event_SetStatus1c6Far, %function
	.thumb_func
Event_SetStatus1c6Far:
	.incbin "baserom.gba", 0x000c83a8, 0x00000008
	.global Event_ClearStatus1c6Far
	.type Event_ClearStatus1c6Far, %function
	.thumb_func
Event_ClearStatus1c6Far:
	.incbin "baserom.gba", 0x000c83b0, 0x00000008
	.global Event_WaitValue1c8FramesFar
	.type Event_WaitValue1c8FramesFar, %function
	.thumb_func
Event_WaitValue1c8FramesFar:
	.incbin "baserom.gba", 0x000c83b8, 0x00000020
	.global Func_080c83d8
	.type Func_080c83d8, %function
	.thumb_func
Func_080c83d8:
	.incbin "baserom.gba", 0x000c83d8, 0x00000008
	.global Func_080c83e0
	.type Func_080c83e0, %function
	.thumb_func
Func_080c83e0:
	.incbin "baserom.gba", 0x000c83e0, 0x00000028
	.global Func_080c8408
	.type Func_080c8408, %function
	.thumb_func
Func_080c8408:
	.incbin "baserom.gba", 0x000c8408, 0x00000018
	.global Object_SetWideSpriteFar
	.type Object_SetWideSpriteFar, %function
	.thumb_func
Object_SetWideSpriteFar:
	.incbin "baserom.gba", 0x000c8420, 0x00000008
	.global Func_080c8428
	.type Func_080c8428, %function
	.thumb_func
Func_080c8428:
	.incbin "baserom.gba", 0x000c8428, 0x00000008
	.global Func_080c8430
	.type Func_080c8430, %function
	.thumb_func
Func_080c8430:
	.incbin "baserom.gba", 0x000c8430, 0x00000008
	.global Func_080c8438
	.type Func_080c8438, %function
	.thumb_func
Func_080c8438:
	.incbin "baserom.gba", 0x000c8438, 0x00000020
	.global ObjectTable_GetFar
	.type ObjectTable_GetFar, %function
	.thumb_func
ObjectTable_GetFar:
	.incbin "baserom.gba", 0x000c8458, 0x00000028
	.global Func_080c8480
	.type Func_080c8480, %function
	.thumb_func
Func_080c8480:
	.incbin "baserom.gba", 0x000c8480, 0x00000018
	.global Field_DispatchTypeHandlerFar
	.type Field_DispatchTypeHandlerFar, %function
	.thumb_func
Field_DispatchTypeHandlerFar:
	.incbin "baserom.gba", 0x000c8498, 0x00000008
	.global Func_080c84a0
	.type Func_080c84a0, %function
	.thumb_func
Func_080c84a0:
	.incbin "baserom.gba", 0x000c84a0, 0x00000008
	.global Func_080c84a8
	.type Func_080c84a8, %function
	.thumb_func
Func_080c84a8:
	.incbin "baserom.gba", 0x000c84a8, 0x00000008
	.global Func_080c84b0
	.type Func_080c84b0, %function
	.thumb_func
Func_080c84b0:
	.incbin "baserom.gba", 0x000c84b0, 0x00000008
	.global Func_080c84b8
	.type Func_080c84b8, %function
	.thumb_func
Func_080c84b8:
	.incbin "baserom.gba", 0x000c84b8, 0x00000008
	.global Func_080c84c0
	.type Func_080c84c0, %function
	.thumb_func
Func_080c84c0:
	.incbin "baserom.gba", 0x000c84c0, 0x00000010
	.global BattleFx_PrepareBufferInterpolationFar
	.type BattleFx_PrepareBufferInterpolationFar, %function
	.thumb_func
BattleFx_PrepareBufferInterpolationFar:
	.incbin "baserom.gba", 0x000c84d0, 0x00000008
	.global Func_080c84d8
	.type Func_080c84d8, %function
	.thumb_func
Func_080c84d8:
	.incbin "baserom.gba", 0x000c84d8, 0x00000008
	.global Func_080c84e0
	.type Func_080c84e0, %function
	.thumb_func
Func_080c84e0:
	.incbin "baserom.gba", 0x000c84e0, 0x00000008
	.global Func_080c84e8
	.type Func_080c84e8, %function
	.thumb_func
Func_080c84e8:
	.incbin "baserom.gba", 0x000c84e8, 0x00000008
	.global Func_080c84f0
	.type Func_080c84f0, %function
	.thumb_func
Func_080c84f0:
	.incbin "baserom.gba", 0x000c84f0, 0x00000018
	.section .rom.000c8520, "ax"
	.incbin "baserom.gba", 0x000c8520, 0x00000008
	.global BattleFx_StartItemBreakFar
	.type BattleFx_StartItemBreakFar, %function
	.thumb_func
BattleFx_StartItemBreakFar:
	.incbin "baserom.gba", 0x000c8528, 0x00000008
	.global BattleFx_SnapScaleToFullFar
	.type BattleFx_SnapScaleToFullFar, %function
	.thumb_func
BattleFx_SnapScaleToFullFar:
	.incbin "baserom.gba", 0x000c8530, 0x00000008
	.global UpdateRisingParticleBurstFar
	.type UpdateRisingParticleBurstFar, %function
	.thumb_func
UpdateRisingParticleBurstFar:
	.incbin "baserom.gba", 0x000c8538, 0x00000008
	.global Func_080c8540
	.type Func_080c8540, %function
	.thumb_func
Func_080c8540:
	.incbin "baserom.gba", 0x000c8540, 0x00000008
	.global Func_080c8548
	.type Func_080c8548, %function
	.thumb_func
Func_080c8548:
	.incbin "baserom.gba", 0x000c8548, 0x00000010
	.global Func_080c8558
	.type Func_080c8558, %function
	.thumb_func
Func_080c8558:
	.incbin "baserom.gba", 0x000c8558, 0x00000018
	.global Func_080c8570
	.type Func_080c8570, %function
	.thumb_func
Func_080c8570:
	.incbin "baserom.gba", 0x000c8570, 0x00000008
	.global Func_080c8578
	.type Func_080c8578, %function
	.thumb_func
Func_080c8578:
	.incbin "baserom.gba", 0x000c8578, 0x00000008
	.global Func_080c8580
	.type Func_080c8580, %function
	.thumb_func
Func_080c8580:
	.incbin "baserom.gba", 0x000c8580, 0x00000040
	.global Func_080c85c0
	.type Func_080c85c0, %function
	.thumb_func
Func_080c85c0:
	.incbin "baserom.gba", 0x000c85c0, 0x00000008
	.global BattleFx_GetResourceIdFar
	.type BattleFx_GetResourceIdFar, %function
	.thumb_func
BattleFx_GetResourceIdFar:
	.incbin "baserom.gba", 0x000c85c8, 0x00000008
	.section .rom.000c85d0, "ax"
	.global Event_ClearInvalidPackedValuesFar
	.type Event_ClearInvalidPackedValuesFar, %function
	.thumb_func
Event_ClearInvalidPackedValuesFar:
	.incbin "baserom.gba", 0x000c85d0, 0x00000018
	.global Func_080c85e8
	.type Func_080c85e8, %function
	.thumb_func
Func_080c85e8:
	.incbin "baserom.gba", 0x000c85e8, 0x00000008
	.global ObjectMotion_OffsetPositionAndResetFar
	.type ObjectMotion_OffsetPositionAndResetFar, %function
	.thumb_func
ObjectMotion_OffsetPositionAndResetFar:
	.incbin "baserom.gba", 0x000c85f0, 0x00000008
	.global ObjectMotion_CommitPositionAndActivateFar
	.type ObjectMotion_CommitPositionAndActivateFar, %function
	.thumb_func
ObjectMotion_CommitPositionAndActivateFar:
	.incbin "baserom.gba", 0x000c85f8, 0x00000008
	.global Object_LinkObjectAndSetCallbackFar
	.type Object_LinkObjectAndSetCallbackFar, %function
	.thumb_func
Object_LinkObjectAndSetCallbackFar:
	.incbin "baserom.gba", 0x000c8600, 0x00000020
	.global ObjectTable_ReadActiveValueFar
	.type ObjectTable_ReadActiveValueFar, %function
	.thumb_func
ObjectTable_ReadActiveValueFar:
	.incbin "baserom.gba", 0x000c8620, 0x00000008
	.section .rom.000c8648, "ax"
	.incbin "baserom.gba", 0x000c8648, 0x00000038
	.global Object_SetActionCallbackFar
	.type Object_SetActionCallbackFar, %function
	.thumb_func
Object_SetActionCallbackFar:
	.incbin "baserom.gba", 0x000c8680, 0x00000008
	.global Func_080c8688
	.type Func_080c8688, %function
	.thumb_func
Func_080c8688:
	.incbin "baserom.gba", 0x000c8688, 0x00000008
	.global Func_080c8690
	.type Func_080c8690, %function
	.thumb_func
Func_080c8690:
	.incbin "baserom.gba", 0x000c8690, 0x00000018
	.global Func_080c86a8
	.type Func_080c86a8, %function
	.thumb_func
Func_080c86a8:
	.incbin "baserom.gba", 0x000c86a8, 0x00000008
	.global Func_080c86b0
	.type Func_080c86b0, %function
	.thumb_func
Func_080c86b0:
	.incbin "baserom.gba", 0x000c86b0, 0x00000008
	.global Func_080c86b8
	.type Func_080c86b8, %function
	.thumb_func
Func_080c86b8:
	.incbin "baserom.gba", 0x000c86b8, 0x00000008
	.global Func_080c86c0
	.type Func_080c86c0, %function
	.thumb_func
Func_080c86c0:
	.incbin "baserom.gba", 0x000c86c0, 0x00000010
	.global Func_080c86d0
	.type Func_080c86d0, %function
	.thumb_func
Func_080c86d0:
	.incbin "baserom.gba", 0x000c86d0, 0x00000008
	.global Func_080c86d8
	.type Func_080c86d8, %function
	.thumb_func
Func_080c86d8:
	.incbin "baserom.gba", 0x000c86d8, 0x00000008
	.global Func_080c86e0
	.type Func_080c86e0, %function
	.thumb_func
Func_080c86e0:
	.incbin "baserom.gba", 0x000c86e0, 0x00000008
	.global Func_080c86e8
	.type Func_080c86e8, %function
	.thumb_func
Func_080c86e8:
	.incbin "baserom.gba", 0x000c86e8, 0x00000008
	.global Func_080c86f0
	.type Func_080c86f0, %function
	.thumb_func
Func_080c86f0:
	.incbin "baserom.gba", 0x000c86f0, 0x00000008
	.global Func_080c86f8
	.type Func_080c86f8, %function
	.thumb_func
Func_080c86f8:
	.incbin "baserom.gba", 0x000c86f8, 0x00000008
	.global Func_080c8700
	.type Func_080c8700, %function
	.thumb_func
Func_080c8700:
	.incbin "baserom.gba", 0x000c8700, 0x00000008
	.global Func_080c8708
	.type Func_080c8708, %function
	.thumb_func
Func_080c8708:
	.incbin "baserom.gba", 0x000c8708, 0x00000008
	.global Func_080c8710
	.type Func_080c8710, %function
	.thumb_func
Func_080c8710:
	.incbin "baserom.gba", 0x000c8710, 0x00000008
	.global Func_080c8718
	.type Func_080c8718, %function
	.thumb_func
Func_080c8718:
	.incbin "baserom.gba", 0x000c8718, 0x00000008
	.global Func_080c8720
	.type Func_080c8720, %function
	.thumb_func
Func_080c8720:
	.incbin "baserom.gba", 0x000c8720, 0x00000008
	.global Func_080c8728
	.type Func_080c8728, %function
	.thumb_func
Func_080c8728:
	.incbin "baserom.gba", 0x000c8728, 0x00000008
	.global Func_080c8730
	.type Func_080c8730, %function
	.thumb_func
Func_080c8730:
	.incbin "baserom.gba", 0x000c8730, 0x00000008
	.global Func_080c8738
	.type Func_080c8738, %function
	.thumb_func
Func_080c8738:
	.incbin "baserom.gba", 0x000c8738, 0x00000008
	.global Func_080c8740
	.type Func_080c8740, %function
	.thumb_func
Func_080c8740:
	.incbin "baserom.gba", 0x000c8740, 0x00000008
	.global Func_080c8748
	.type Func_080c8748, %function
	.thumb_func
Func_080c8748:
	.incbin "baserom.gba", 0x000c8748, 0x00000008
	.global Func_080c8750
	.type Func_080c8750, %function
	.thumb_func
Func_080c8750:
	.incbin "baserom.gba", 0x000c8750, 0x00000008
	.global Object_SpawnFar
	.type Object_SpawnFar, %function
	.thumb_func
Object_SpawnFar:
	.incbin "baserom.gba", 0x000c8758, 0x00000008
	.global Func_080c8760
	.type Func_080c8760, %function
	.thumb_func
Func_080c8760:
	.incbin "baserom.gba", 0x000c8760, 0x00000018
	.global Func_080c8778
	.type Func_080c8778, %function
	.thumb_func
Func_080c8778:
	.incbin "baserom.gba", 0x000c8778, 0x00000008
	.global Func_080c8780
	.type Func_080c8780, %function
	.thumb_func
Func_080c8780:
	.incbin "baserom.gba", 0x000c8780, 0x00000018
	.global Func_080c8798
	.type Func_080c8798, %function
	.thumb_func
Func_080c8798:
	.incbin "baserom.gba", 0x000c8798, 0x00000008
	.global Func_080c87a0
	.type Func_080c87a0, %function
	.thumb_func
Func_080c87a0:
	.incbin "baserom.gba", 0x000c87a0, 0x00000008
	.global Func_080c87a8
	.type Func_080c87a8, %function
	.thumb_func
Func_080c87a8:
	.incbin "baserom.gba", 0x000c87a8, 0x00000008
	.global Func_080c87b0
	.type Func_080c87b0, %function
	.thumb_func
Func_080c87b0:
	.incbin "baserom.gba", 0x000c87b0, 0x00000008
	.global Func_080c87b8
	.type Func_080c87b8, %function
	.thumb_func
Func_080c87b8:
	.incbin "baserom.gba", 0x000c87b8, 0x00000008
	.global Func_080c87c0
	.type Func_080c87c0, %function
	.thumb_func
Func_080c87c0:
	.incbin "baserom.gba", 0x000c87c0, 0x00000008
	.global Func_080c87c8
	.type Func_080c87c8, %function
	.thumb_func
Func_080c87c8:
	.incbin "baserom.gba", 0x000c87c8, 0x00000008
	.global Func_080c87d0
	.type Func_080c87d0, %function
	.thumb_func
Func_080c87d0:
	.incbin "baserom.gba", 0x000c87d0, 0x00000010
	.global Func_080c87e0
	.type Func_080c87e0, %function
	.thumb_func
Func_080c87e0:
	.incbin "baserom.gba", 0x000c87e0, 0x00000008
	.global Func_080c87e8
	.type Func_080c87e8, %function
	.thumb_func
Func_080c87e8:
	.incbin "baserom.gba", 0x000c87e8, 0x00000008
	.global Func_080c87f0
	.type Func_080c87f0, %function
	.thumb_func
Func_080c87f0:
	.incbin "baserom.gba", 0x000c87f0, 0x00000008
	.global Func_080c87f8
	.type Func_080c87f8, %function
	.thumb_func
Func_080c87f8:
	.incbin "baserom.gba", 0x000c87f8, 0x00000008
	.global Field_BeginPaletteTransitionFar
	.type Field_BeginPaletteTransitionFar, %function
	.thumb_func
Field_BeginPaletteTransitionFar:
	.incbin "baserom.gba", 0x000c8800, 0x00000008
	.global Func_080c8808
	.type Func_080c8808, %function
	.thumb_func
Func_080c8808:
	.incbin "baserom.gba", 0x000c8808, 0x00000008
	.global Func_080c8810
	.type Func_080c8810, %function
	.thumb_func
Func_080c8810:
	.incbin "baserom.gba", 0x000c8810, 0x00000008
	.global Func_080c8818
	.type Func_080c8818, %function
	.thumb_func
Func_080c8818:
	.incbin "baserom.gba", 0x000c8818, 0x00000008
	.global Func_080c8820
	.type Func_080c8820, %function
	.thumb_func
Func_080c8820:
	.incbin "baserom.gba", 0x000c8820, 0x00000008
	.section .rom.000c88c8, "ax"
	.global Func_080c88c8
	.type Func_080c88c8, %function
	.thumb_func
Func_080c88c8:
	.incbin "baserom.gba", 0x000c88c8, 0x00000004
	.global BattleFx_DecodeFrameTranslationBase
BattleFx_DecodeFrameTranslationBase:
	.incbin "baserom.gba", 0x000c88cc, 0x00000004
	.global Func_080c88d0
	.type Func_080c88d0, %function
	.thumb_func
Func_080c88d0:
	.incbin "baserom.gba", 0x000c88d0, 0x00000008
	.global Func_080c88d8
	.type Func_080c88d8, %function
	.thumb_func
Func_080c88d8:
	.incbin "baserom.gba", 0x000c88d8, 0x00000008
	.global Func_080c88e0
	.type Func_080c88e0, %function
	.thumb_func
Func_080c88e0:
	.incbin "baserom.gba", 0x000c88e0, 0x00000008
	.global Func_080c88e8
	.type Func_080c88e8, %function
	.thumb_func
Func_080c88e8:
	.incbin "baserom.gba", 0x000c88e8, 0x00000004
	.global BattleFx_DecodeFrameUnusedBase
BattleFx_DecodeFrameUnusedBase:
	.incbin "baserom.gba", 0x000c88ec, 0x00000004
	.global Func_080c88f0
	.type Func_080c88f0, %function
	.thumb_func
Func_080c88f0:
	.incbin "baserom.gba", 0x000c88f0, 0x00000008
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.type ObjectMotion_ResetTargetsAndVelocityFar, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocityFar:
	.incbin "baserom.gba", 0x000c88f8, 0x00000008
	.global ObjectEffect_BeginContextEffect26Far
	.type ObjectEffect_BeginContextEffect26Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect26Far:
	.incbin "baserom.gba", 0x000c8900, 0x00000018
	.global Func_080c8918
	.type Func_080c8918, %function
	.thumb_func
Func_080c8918:
	.incbin "baserom.gba", 0x000c8918, 0x00000010
	.global Func_080c8928
	.type Func_080c8928, %function
	.thumb_func
Func_080c8928:
	.incbin "baserom.gba", 0x000c8928, 0x00000008
	.global Func_080c8930
	.type Func_080c8930, %function
	.thumb_func
Func_080c8930:
	.incbin "baserom.gba", 0x000c8930, 0x00000018
	.global Func_080c8948
	.type Func_080c8948, %function
	.thumb_func
Func_080c8948:
	.incbin "baserom.gba", 0x000c8948, 0x00000008
	.section .rom.000c89cc, "ax"
	.incbin "baserom.gba", 0x000c89cc, 0x00000a00
	.section .rom.000c9694, "ax"
	.incbin "baserom.gba", 0x000c9694, 0x0000028c
	.section .rom.000c9934, "ax"
	.incbin "baserom.gba", 0x000c9934, 0x000005f8
	.section .rom.000c9f2c, "ax"
	.global BattleFx_LookupResult
	.type BattleFx_LookupResult, %function
	.thumb_func
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000c9f2c, 0x000000ac
	.section .rom.000c9fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000c9fd8, 0x0000018c
	.section .rom.000ca1bc, "ax"
	.incbin "baserom.gba", 0x000ca1bc, 0x000002d8
	.section .rom.000ca4a8, "ax"
	.incbin "baserom.gba", 0x000ca4a8, 0x00000240
	.section .rom.000ca6e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000ca6e8, 0x000002e4
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000ca9cc, 0x00000060
	.global Func_080caa2c
	.type Func_080caa2c, %function
	.thumb_func
Func_080caa2c:
	.incbin "baserom.gba", 0x000caa2c, 0x00000020
	.global Func_080caa4c
	.type Func_080caa4c, %function
	.thumb_func
Func_080caa4c:
	.incbin "baserom.gba", 0x000caa4c, 0x00000274
	.section .rom.000cacc0, "ax"
	.global ObjectTable_FindLastActiveId
	.type ObjectTable_FindLastActiveId, %function
	.thumb_func
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000cacc0, 0x0000002c
	.global Scene_AssignViewFlags
	.type Scene_AssignViewFlags, %function
	.thumb_func
Scene_AssignViewFlags:
	.incbin "baserom.gba", 0x000cacec, 0x00000098
	.section .rom.000cad84, "ax"
	.global ObjectTable_Get
	.type ObjectTable_Get, %function
	.thumb_func
ObjectTable_Get:
	.incbin "baserom.gba", 0x000cad84, 0x00000018
	.global Func_080cad9c
	.type Func_080cad9c, %function
	.thumb_func
Func_080cad9c:
	.incbin "baserom.gba", 0x000cad9c, 0x000000c0
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000cae5c, 0x00000114
	.section .rom.000cafc4, "ax"
	.incbin "baserom.gba", 0x000cafc4, 0x00000730
	.section .rom.000cb82c, "ax"
	.incbin "baserom.gba", 0x000cb82c, 0x00001484
	.section .rom.000cccb0, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000cccb0, 0x00000090
	.section .rom.000ccd6e, "ax"
	.incbin "baserom.gba", 0x000ccd6e, 0x00000002
	.global SceneEvent_FindActiveRecord
	.type SceneEvent_FindActiveRecord, %function
	.thumb_func
SceneEvent_FindActiveRecord:
	.incbin "baserom.gba", 0x000ccd70, 0x0000010c
	.incbin "baserom.gba", 0x000cce7c, 0x00000a98
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000cd914, 0x000002dc
	.global Func_080cdbf8
	.type Func_080cdbf8, %function
	.thumb_func
Func_080cdbf8:
	.incbin "baserom.gba", 0x000cdbf0, 0x00000048
	.section .rom.000cdc6a, "ax"
	.incbin "baserom.gba", 0x000cdc6a, 0x00000002
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000cdc6c, 0x00000324
	.section .rom.000cdf90, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000cdf90, 0x00000ba8
	.global Func_080ceb58
	.type Func_080ceb58, %function
	.thumb_func
Func_080ceb58:
	.incbin "baserom.gba", 0x000ceb38, 0x00000028
	.section .rom.000ceb88, "ax"
	.incbin "baserom.gba", 0x000ceb88, 0x00000298
	.section .rom.000cee60, "ax"
	.incbin "baserom.gba", 0x000cee60, 0x00000048
	.section .rom.000ceea8, "ax"
	.global Func_080ceec8
	.type Func_080ceec8, %function
	.thumb_func
Func_080ceec8:
	.incbin "baserom.gba", 0x000ceea8, 0x000000a0
	.section .rom.000cef48, "ax"
	.global Func_080cef68
	.type Func_080cef68, %function
	.thumb_func
Func_080cef68:
	.incbin "baserom.gba", 0x000cef48, 0x0000004c
	.section .rom.000cef94, "ax"
	.global Func_080cefb4
	.type Func_080cefb4, %function
	.thumb_func
Func_080cefb4:
	.incbin "baserom.gba", 0x000cef94, 0x0000001c
	.section .rom.000cefb0, "ax"
	.global Func_080cefd0
	.type Func_080cefd0, %function
	.thumb_func
Func_080cefd0:
	.incbin "baserom.gba", 0x000cefb0, 0x0000002c
	.section .rom.000cefdc, "ax"
	.global Func_080ceffc
	.type Func_080ceffc, %function
	.thumb_func
Func_080ceffc:
	.incbin "baserom.gba", 0x000cefdc, 0x00000008
	.section .rom.000cefe4, "ax"
	.global Func_080cf004
	.type Func_080cf004, %function
	.thumb_func
Func_080cf004:
	.incbin "baserom.gba", 0x000cefe4, 0x00000008
	.section .rom.000cefec, "ax"
	.global Func_080cf00c
	.type Func_080cf00c, %function
	.thumb_func
Func_080cf00c:
	.incbin "baserom.gba", 0x000cefec, 0x00000044
	.section .rom.000cf030, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000cf030, 0x00000080
	.section .rom.000cf138, "ax"
	.incbin "baserom.gba", 0x000cf138, 0x00000024
	.section .rom.000cf15c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000cf15c, 0x00000238
	.section .rom.000cf394, "ax"
	.global Func_080cf3b4
	.type Func_080cf3b4, %function
	.thumb_func
Func_080cf3b4:
	.incbin "baserom.gba", 0x000cf394, 0x00000070
	.section .rom.000cf404, "ax"
	.global Func_080cf424
	.type Func_080cf424, %function
	.thumb_func
Func_080cf424:
	.incbin "baserom.gba", 0x000cf404, 0x000000bc
	.section .rom.000cf534, "ax"
	.incbin "baserom.gba", 0x000cf534, 0x00000ba4
	.section .rom.000d00d8, "ax"
	.global Func_080d00f8
	.type Func_080d00f8, %function
	.thumb_func
Func_080d00f8:
	.incbin "baserom.gba", 0x000d00d8, 0x000000d4
	.section .rom.000d01ac, "ax"
	.global Func_080d01cc
	.type Func_080d01cc, %function
	.thumb_func
Func_080d01cc:
	.incbin "baserom.gba", 0x000d01ac, 0x00000354
	.section .rom.000d0500, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000d0500, 0x0000020c
	.section .rom.000d070c, "ax"
	.global Func_080d072c
	.type Func_080d072c, %function
	.thumb_func
Func_080d072c:
	.incbin "baserom.gba", 0x000d070c, 0x0000001c
	.section .rom.000d0728, "ax"
	.global Func_080d0748
	.type Func_080d0748, %function
	.thumb_func
Func_080d0748:
	.incbin "baserom.gba", 0x000d0728, 0x000004a4
	.section .rom.000d0bcc, "ax"
	.global Func_080d0bec
	.type Func_080d0bec, %function
	.thumb_func
Func_080d0bec:
	.incbin "baserom.gba", 0x000d0bcc, 0x00000064
	.section .rom.000d0c30, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000d0c30, 0x00000a34
	.section .rom.000d1664, "ax"
	.global Func_080d1684
	.type Func_080d1684, %function
	.thumb_func
Func_080d1684:
	.incbin "baserom.gba", 0x000d1664, 0x00000074
	.section .rom.000d16d8, "ax"
	.global Func_080d16f8
	.type Func_080d16f8, %function
	.thumb_func
Func_080d16f8:
	.incbin "baserom.gba", 0x000d16d8, 0x00000014
	.section .rom.000d16ec, "ax"
	.global Func_080d170c
	.type Func_080d170c, %function
	.thumb_func
Func_080d170c:
	.incbin "baserom.gba", 0x000d16ec, 0x00000020
	.section .rom.000d170c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000d170c, 0x00000020
	.section .rom.000d172c, "ax"
	.global Func_080d174c
	.type Func_080d174c, %function
	.thumb_func
Func_080d174c:
	.incbin "baserom.gba", 0x000d172c, 0x00000014
	.section .rom.000d1740, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d1740, 0x0000004c
	.section .rom.000d178c, "ax"
	.global BattleFx_StartBufferInterpolation
	.type BattleFx_StartBufferInterpolation, %function
	.thumb_func
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000d178c, 0x0000003c
	.section .rom.000d17c8, "ax"
	.global Func_080d17e8
	.type Func_080d17e8, %function
	.thumb_func
Func_080d17e8:
	.incbin "baserom.gba", 0x000d17c8, 0x00000034
	.section .rom.000d181e, "ax"
	.incbin "baserom.gba", 0x000d181e, 0x000001da
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000d19f8, 0x000000ac
	.global Func_080d1ac4
	.type Func_080d1ac4, %function
	.thumb_func
Func_080d1ac4:
	.incbin "baserom.gba", 0x000d1aa4, 0x00000010
	.global Func_080d1ad4
	.type Func_080d1ad4, %function
	.thumb_func
Func_080d1ad4:
	.incbin "baserom.gba", 0x000d1ab4, 0x000001ec
	.global Func_080d1cc0
	.type Func_080d1cc0, %function
	.thumb_func
Func_080d1cc0:
	.incbin "baserom.gba", 0x000d1ca0, 0x00000074
	.global Func_080d1d34
	.type Func_080d1d34, %function
	.thumb_func
Func_080d1d34:
	.incbin "baserom.gba", 0x000d1d14, 0x00000010
	.global Func_080d1d44
	.type Func_080d1d44, %function
	.thumb_func
Func_080d1d44:
	.incbin "baserom.gba", 0x000d1d24, 0x00000010
	.global Func_080d1d54
	.type Func_080d1d54, %function
	.thumb_func
Func_080d1d54:
	.incbin "baserom.gba", 0x000d1d34, 0x00000058
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d1d8c, 0x00000044
	.global Func_080d1df0
	.type Func_080d1df0, %function
	.thumb_func
Func_080d1df0:
	.incbin "baserom.gba", 0x000d1dd0, 0x00000028
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000d1df8, 0x0000006c
	.section .rom.000d1e8a, "ax"
	.incbin "baserom.gba", 0x000d1e8a, 0x00000002
	.global Func_080d1eac
	.type Func_080d1eac, %function
	.thumb_func
Func_080d1eac:
	.incbin "baserom.gba", 0x000d1e8c, 0x0000002c
	.section .rom.000d1eb8, "ax"
	.global BattleFx_GetFlags
	.type BattleFx_GetFlags, %function
	.thumb_func
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000d1eb8, 0x00000010
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d1ec8, 0x00000024
	.global Func_080d1f0c
	.type Func_080d1f0c, %function
	.thumb_func
Func_080d1f0c:
	.incbin "baserom.gba", 0x000d1eec, 0x0000000c
	.global Func_080d1f18
	.type Func_080d1f18, %function
	.thumb_func
Func_080d1f18:
	.incbin "baserom.gba", 0x000d1ef8, 0x00000008
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000d1f00, 0x00000164
	.global Func_080d2084
	.type Func_080d2084, %function
	.thumb_func
Func_080d2084:
	.incbin "baserom.gba", 0x000d2064, 0x00000078
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000d20dc, 0x00000144
	.section .rom.000d2220, "ax"
	.global Battle_WaitMode0
	.type Battle_WaitMode0, %function
	.thumb_func
Battle_WaitMode0:
	.incbin "baserom.gba", 0x000d2220, 0x00000020
	.global Func_080d2260
	.type Func_080d2260, %function
	.thumb_func
Func_080d2260:
	.incbin "baserom.gba", 0x000d2240, 0x00000048
	.global Func_080d22a8
	.type Func_080d22a8, %function
	.thumb_func
Func_080d22a8:
	.incbin "baserom.gba", 0x000d2288, 0x000000a8
	.global Func_080d2350
	.type Func_080d2350, %function
	.thumb_func
Func_080d2350:
	.incbin "baserom.gba", 0x000d2330, 0x00000048
	.global Event_RunObjectHookAndWait
	.type Event_RunObjectHookAndWait, %function
	.thumb_func
Event_RunObjectHookAndWait:
	.incbin "baserom.gba", 0x000d2378, 0x00000020
	.section .rom.000d23aa, "ax"
	.incbin "baserom.gba", 0x000d23aa, 0x00000002
	.global Event_SetWorkWord10
	.type Event_SetWorkWord10, %function
	.thumb_func
Event_SetWorkWord10:
	.incbin "baserom.gba", 0x000d23ac, 0x0000000c
	.section .rom.000d25a8, "ax"
	.global Func_080d25c8
	.type Func_080d25c8, %function
	.thumb_func
Func_080d25c8:
	.incbin "baserom.gba", 0x000d25a8, 0x00000044
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d25ec, 0x000001e0
	.section .rom.000d27ea, "ax"
	.incbin "baserom.gba", 0x000d27ea, 0x00000036
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d2820, 0x0000011c
	.global Func_080d295c
	.type Func_080d295c, %function
	.thumb_func
Func_080d295c:
	.incbin "baserom.gba", 0x000d293c, 0x00000010
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000d294c, 0x000000a0
	.section .rom.000d2a1c, "ax"
	.global Func_080d2a3c
	.type Func_080d2a3c, %function
	.thumb_func
Func_080d2a3c:
	.incbin "baserom.gba", 0x000d2a1c, 0x00000028
	.global Func_080d2a64
	.type Func_080d2a64, %function
	.thumb_func
Func_080d2a64:
	.incbin "baserom.gba", 0x000d2a44, 0x00000028
	.global Func_080d2a8c
	.type Func_080d2a8c, %function
	.thumb_func
Func_080d2a8c:
	.incbin "baserom.gba", 0x000d2a6c, 0x00000018
	.global Func_080d2aa4
	.type Func_080d2aa4, %function
	.thumb_func
Func_080d2aa4:
	.incbin "baserom.gba", 0x000d2a84, 0x0000002c
	.global Func_080d2ad0
	.type Func_080d2ad0, %function
	.thumb_func
Func_080d2ad0:
	.incbin "baserom.gba", 0x000d2ab0, 0x0000002c
	.global Func_080d2afc
	.type Func_080d2afc, %function
	.thumb_func
Func_080d2afc:
	.incbin "baserom.gba", 0x000d2adc, 0x00000010
	.global Func_080d2b0c
	.type Func_080d2b0c, %function
	.thumb_func
Func_080d2b0c:
	.incbin "baserom.gba", 0x000d2aec, 0x00000040
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d2b2c, 0x000000b8
	.section .rom.000d2c44, "ax"
	.global Func_080d2c64
	.type Func_080d2c64, %function
	.thumb_func
Func_080d2c64:
	.incbin "baserom.gba", 0x000d2c44, 0x00000034
	.global Func_080d2c98
	.type Func_080d2c98, %function
	.thumb_func
Func_080d2c98:
	.incbin "baserom.gba", 0x000d2c78, 0x00000004
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d2c7c, 0x00000028
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d2ca4, 0x00000044
	.global Func_080d2d08
	.type Func_080d2d08, %function
	.thumb_func
Func_080d2d08:
	.incbin "baserom.gba", 0x000d2ce8, 0x00000068
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000d2d50, 0x00000014
	.section .rom.000d2dc4, "ax"
	.global ObjectMotion_ResetTargetsAndVelocity
	.type ObjectMotion_ResetTargetsAndVelocity, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	.incbin "baserom.gba", 0x000d2dc4, 0x00000038
	.section .rom.000d2f28, "ax"
	.global ObjectMotion_SnapHeadingAndOffset
	.type ObjectMotion_SnapHeadingAndOffset, %function
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	.incbin "baserom.gba", 0x000d2f28, 0x00000080
	.section .rom.000d3050, "ax"
	.global Func_080d3070
	.type Func_080d3070, %function
	.thumb_func
Func_080d3070:
	.incbin "baserom.gba", 0x000d3050, 0x0000008c
	.section .rom.000d30f6, "ax"
	.incbin "baserom.gba", 0x000d30f6, 0x00000002
	.global Func_080d3118
	.type Func_080d3118, %function
	.thumb_func
Func_080d3118:
	.incbin "baserom.gba", 0x000d30f8, 0x0000004c
	.global Func_080d3164
	.type Func_080d3164, %function
	.thumb_func
Func_080d3164:
	.incbin "baserom.gba", 0x000d3144, 0x0000005c
	.global Func_080d31c0
	.type Func_080d31c0, %function
	.thumb_func
Func_080d31c0:
	.incbin "baserom.gba", 0x000d31a0, 0x00000054
	.global Func_080d3214
	.type Func_080d3214, %function
	.thumb_func
Func_080d3214:
	.incbin "baserom.gba", 0x000d31f4, 0x0000002c
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
	.incbin "baserom.gba", 0x000d331c, 0x00000124
	.global Func_080d3460
	.type Func_080d3460, %function
	.thumb_func
Func_080d3460:
	.incbin "baserom.gba", 0x000d3440, 0x0000013c
	.section .rom.000d35de, "ax"
	.incbin "baserom.gba", 0x000d35de, 0x00000002
	.section .rom.000d35e0, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.type FacingObject_TurnPairToFaceEachOther, %function
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000d35e0, 0x000000a8
	.section .rom.000d3688, "ax"
	.global Func_080d36a8
	.type Func_080d36a8, %function
	.thumb_func
Func_080d36a8:
	.incbin "baserom.gba", 0x000d3688, 0x00000020
	.section .rom.000d36d4, "ax"
	.global ObjectGroup_ApplyIndexedChildValue
	.type ObjectGroup_ApplyIndexedChildValue, %function
	.thumb_func
ObjectGroup_ApplyIndexedChildValue:
	.incbin "baserom.gba", 0x000d36d4, 0x00000050
	.section .rom.000d3724, "ax"
	.global Object_SetPartAttribute
	.type Object_SetPartAttribute, %function
	.thumb_func
Object_SetPartAttribute:
	.incbin "baserom.gba", 0x000d3724, 0x0000003c
	.section .rom.000d3760, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000d3760, 0x00000054
	.section .rom.000d38ec, "ax"
	.global Func_080d390c
	.type Func_080d390c, %function
	.thumb_func
Func_080d390c:
	.incbin "baserom.gba", 0x000d38ec, 0x0000001c
	.global Func_080d3928
	.type Func_080d3928, %function
	.thumb_func
Func_080d3928:
	.incbin "baserom.gba", 0x000d3908, 0x00000018
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000d3920, 0x000000d0
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000d39f0, 0x00000118
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000d3b08, 0x000000c0
	.global Func_080d3be8
	.type Func_080d3be8, %function
	.thumb_func
Func_080d3be8:
	.incbin "baserom.gba", 0x000d3bc8, 0x00000010
	.section .rom.000d3bd8, "ax"
	.global ObjectTable_ReadActiveValue
	.type ObjectTable_ReadActiveValue, %function
	.thumb_func
ObjectTable_ReadActiveValue:
	.incbin "baserom.gba", 0x000d3bd8, 0x00000034
	.global Func_080d3c2c
	.type Func_080d3c2c, %function
	.thumb_func
Func_080d3c2c:
	.incbin "baserom.gba", 0x000d3c0c, 0x0000005c
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d3c68, 0x00000328
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000d3f90, 0x000000bc
	.global Func_080d406c
	.type Func_080d406c, %function
	.thumb_func
Func_080d406c:
	.incbin "baserom.gba", 0x000d404c, 0x00000010
	.global Func_080d407c
	.type Func_080d407c, %function
	.thumb_func
Func_080d407c:
	.incbin "baserom.gba", 0x000d405c, 0x00000008
	.global Func_080d4084
	.type Func_080d4084, %function
	.thumb_func
Func_080d4084:
	.incbin "baserom.gba", 0x000d4064, 0x00000054
	.global Func_080d40d8
	.type Func_080d40d8, %function
	.thumb_func
Func_080d40d8:
	.incbin "baserom.gba", 0x000d40b8, 0x00000004
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000d40bc, 0x0000009c
	.global Func_080d4178
	.type Func_080d4178, %function
	.thumb_func
Func_080d4178:
	.incbin "baserom.gba", 0x000d4158, 0x00000008
	.section .rom.000d41c6, "ax"
	.incbin "baserom.gba", 0x000d41c6, 0x00000002
	.section .rom.000d41dc, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000d41dc, 0x00000134
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000d4310, 0x00000054
	.section .rom.000d4364, "ax"
	.global Object_AttachWorkTargetToObject
	.type Object_AttachWorkTargetToObject, %function
	.thumb_func
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000d4364, 0x00000068
	.global Func_080d43ec
	.type Func_080d43ec, %function
	.thumb_func
Func_080d43ec:
	.incbin "baserom.gba", 0x000d43cc, 0x00000020
	.section .rom.000d43ec, "ax"
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d43ec, 0x00000100
	.section .rom.000d44ec, "ax"
	.global Func_080d450c
	.type Func_080d450c, %function
	.thumb_func
Func_080d450c:
	.incbin "baserom.gba", 0x000d44ec, 0x00000020
	.section .rom.000d450c, "ax"
	.global Func_080d452c
	.type Func_080d452c, %function
	.thumb_func
Func_080d452c:
	.incbin "baserom.gba", 0x000d450c, 0x0000001c
	.section .rom.000d4528, "ax"
	.global Func_080d4548
	.type Func_080d4548, %function
	.thumb_func
Func_080d4548:
	.incbin "baserom.gba", 0x000d4528, 0x00000020
	.section .rom.000d4548, "ax"
	.global Func_080d4568
	.type Func_080d4568, %function
	.thumb_func
Func_080d4568:
	.incbin "baserom.gba", 0x000d4548, 0x00000050
	.section .rom.000d4598, "ax"
	.global Func_080d45b8
	.type Func_080d45b8, %function
	.thumb_func
Func_080d45b8:
	.incbin "baserom.gba", 0x000d4598, 0x000000ec
	.section .rom.000d4684, "ax"
	.global Func_080d46a4
	.type Func_080d46a4, %function
	.thumb_func
Func_080d46a4:
	.incbin "baserom.gba", 0x000d4684, 0x00000070
	.section .rom.000d46f4, "ax"
	.global Func_080d4714
	.type Func_080d4714, %function
	.thumb_func
Func_080d4714:
	.incbin "baserom.gba", 0x000d46f4, 0x00000058
	.section .rom.000d4794, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000d4794, 0x000000e8
	.section .rom.000d487c, "ax"
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000d487c, 0x000000f0
	.section .rom.000d4a34, "ax"
	.incbin "baserom.gba", 0x000d4a34, 0x00000054
	.global Func_080d4aa8
	.type Func_080d4aa8, %function
	.thumb_func
Func_080d4aa8:
	.incbin "baserom.gba", 0x000d4a88, 0x0000000c
	.global Func_080d4ab4
	.type Func_080d4ab4, %function
	.thumb_func
Func_080d4ab4:
	.incbin "baserom.gba", 0x000d4a94, 0x00000058
	.section .rom.000d4aec, "ax"
	.global Object_SetActionCallback
	.type Object_SetActionCallback, %function
	.thumb_func
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000d4aec, 0x000001fc
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000d4ce8, 0x000003f0
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000d50d8, 0x000002c0
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000d5398, 0x00000834
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000d5bcc, 0x00000184
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000d5d50, 0x00000070
	.section .rom.000d5dc0, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000d5dc0, 0x00000070
	.section .rom.000d5e42, "ax"
	.incbin "baserom.gba", 0x000d5e42, 0x00000002
	.global Func_080d5e64
	.type Func_080d5e64, %function
	.thumb_func
Func_080d5e64:
	.incbin "baserom.gba", 0x000d5e44, 0x00000014
	.section .rom.000d5e58, "ax"
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000d5e58, 0x000000a0
	.section .rom.000d5fb4, "ax"
	.incbin "baserom.gba", 0x000d5fb4, 0x00000434
	.global Func_080d6408
	.type Func_080d6408, %function
	.thumb_func
Func_080d6408:
	.incbin "baserom.gba", 0x000d63e8, 0x000000b0
	.global Func_080d64b8
	.type Func_080d64b8, %function
	.thumb_func
Func_080d64b8:
	.incbin "baserom.gba", 0x000d6498, 0x00000454
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000d68ec, 0x00000284
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d6b70, 0x000002c8
	.global Func_080d6e58
	.type Func_080d6e58, %function
	.thumb_func
Func_080d6e58:
	.incbin "baserom.gba", 0x000d6e38, 0x000000dc
	.global Func_080d6f34
	.type Func_080d6f34, %function
	.thumb_func
Func_080d6f34:
	.incbin "baserom.gba", 0x000d6f14, 0x000000f0
	.global Func_080d7024
	.type Func_080d7024, %function
	.thumb_func
Func_080d7024:
	.incbin "baserom.gba", 0x000d7004, 0x0000021c
	.global Func_080d7240
	.type Func_080d7240, %function
	.thumb_func
Func_080d7240:
	.incbin "baserom.gba", 0x000d7220, 0x000000c4
	.global Func_080d7304
	.type Func_080d7304, %function
	.thumb_func
Func_080d7304:
	.incbin "baserom.gba", 0x000d72e4, 0x000000b0
	.global Func_080d73b4
	.type Func_080d73b4, %function
	.thumb_func
Func_080d73b4:
	.incbin "baserom.gba", 0x000d7394, 0x0000002c
	.global Func_080d73e0
	.type Func_080d73e0, %function
	.thumb_func
Func_080d73e0:
	.incbin "baserom.gba", 0x000d73c0, 0x00000028
	.global Func_080d7408
	.type Func_080d7408, %function
	.thumb_func
Func_080d7408:
	.incbin "baserom.gba", 0x000d73e8, 0x00000028
	.global Func_080d7430
	.type Func_080d7430, %function
	.thumb_func
Func_080d7430:
	.incbin "baserom.gba", 0x000d7410, 0x000000c0
	.section .rom.000d7504, "ax"
	.incbin "baserom.gba", 0x000d7504, 0x000011c4
	.section .rom.000d86fc, "ax"
	.incbin "baserom.gba", 0x000d86fc, 0x0000023c
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000d8938, 0x000003ec
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000d8d24, 0x0000039c
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000d90c0, 0x000001d0
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000d9290, 0x0000043c
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000d96cc, 0x000003a0
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000d9a6c, 0x00000020
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000d9a8c, 0x00000038
	.global Func_080d9b08
	.type Func_080d9b08, %function
	.thumb_func
Func_080d9b08:
	.incbin "baserom.gba", 0x000d9ac4, 0x00000390
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000d9e54, 0x00000a70
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000da8c4, 0x00000030
	.global Func_080da938
	.type Func_080da938, %function
	.thumb_func
Func_080da938:
	.incbin "baserom.gba", 0x000da8f4, 0x00000594
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000dae88, 0x000001e4
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000db06c, 0x00000394
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000db400, 0x00000048
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
	.incbin "baserom.gba", 0x000db840, 0x00000098
	.global ObjectGroup_ApplyRandomChildValues
	.type ObjectGroup_ApplyRandomChildValues, %function
	.thumb_func
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000db8d8, 0x000000a4
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000db97c, 0x0000000c
	.global Func_080db9cc
	.type Func_080db9cc, %function
	.thumb_func
Func_080db9cc:
	.incbin "baserom.gba", 0x000db988, 0x0000030c
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000dbc94, 0x00000070
	.global Func_080dbd48
	.type Func_080dbd48, %function
	.thumb_func
Func_080dbd48:
	.incbin "baserom.gba", 0x000dbd04, 0x00000080
	.global Func_080dbdc8
	.type Func_080dbdc8, %function
	.thumb_func
Func_080dbdc8:
	.incbin "baserom.gba", 0x000dbd84, 0x0000000c
	.global Func_080dbdd4
	.type Func_080dbdd4, %function
	.thumb_func
Func_080dbdd4:
	.incbin "baserom.gba", 0x000dbd90, 0x00000014
	.global Func_080dbde8
	.type Func_080dbde8, %function
	.thumb_func
Func_080dbde8:
	.incbin "baserom.gba", 0x000dbda4, 0x0000000c
	.global Func_080dbdf4
	.type Func_080dbdf4, %function
	.thumb_func
Func_080dbdf4:
	.incbin "baserom.gba", 0x000dbdb0, 0x00000014
	.global Func_080dbe08
	.type Func_080dbe08, %function
	.thumb_func
Func_080dbe08:
	.incbin "baserom.gba", 0x000dbdc4, 0x0000003c
	.global Func_080dbe44
	.type Func_080dbe44, %function
	.thumb_func
Func_080dbe44:
	.incbin "baserom.gba", 0x000dbe00, 0x00000274
	.global Func_080dc0b8
	.type Func_080dc0b8, %function
	.thumb_func
Func_080dc0b8:
	.incbin "baserom.gba", 0x000dc074, 0x00000020
	.global Func_080dc0d8
	.type Func_080dc0d8, %function
	.thumb_func
Func_080dc0d8:
	.incbin "baserom.gba", 0x000dc094, 0x00000034
	.global Object_Spawn
	.type Object_Spawn, %function
	.thumb_func
Object_Spawn:
	.incbin "baserom.gba", 0x000dc0c8, 0x00000058
	.section .rom.000dc16c, "ax"
	.global Func_080dc1b0
	.type Func_080dc1b0, %function
	.thumb_func
Func_080dc1b0:
	.incbin "baserom.gba", 0x000dc16c, 0x00000094
	.section .rom.000dc200, "ax"
	.global Field_BeginPaletteTransition
	.type Field_BeginPaletteTransition, %function
	.thumb_func
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000dc200, 0x00000050
	.section .rom.000dc250, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000dc250, 0x000000f0
	.section .rom.000dc340, "ax"
	.global BattleFx_PrepareBufferInterpolation
	.type BattleFx_PrepareBufferInterpolation, %function
	.thumb_func
BattleFx_PrepareBufferInterpolation:
	.incbin "baserom.gba", 0x000dc340, 0x0000008c
	.section .rom.000dc3cc, "ax"
	.global Func_080dc410
	.type Func_080dc410, %function
	.thumb_func
Func_080dc410:
	.incbin "baserom.gba", 0x000dc3cc, 0x0000021c
	.section .rom.000dc5e8, "ax"
	.global Func_080dc62c
	.type Func_080dc62c, %function
	.thumb_func
Func_080dc62c:
	.incbin "baserom.gba", 0x000dc5e8, 0x000000ac
	.section .rom.000dc694, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000dc694, 0x000000f4
	.section .rom.000dc788, "ax"
	.global Func_080dc7cc
	.type Func_080dc7cc, %function
	.thumb_func
Func_080dc7cc:
	.incbin "baserom.gba", 0x000dc788, 0x0000001c
	.section .rom.000dc7a4, "ax"
	.global Func_080dc7e8
	.type Func_080dc7e8, %function
	.thumb_func
Func_080dc7e8:
	.incbin "baserom.gba", 0x000dc7a4, 0x00000190
	.section .rom.000dc934, "ax"
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000dc934, 0x000000d8
	.section .rom.000dca0c, "ax"
	.global Func_080dca50
	.type Func_080dca50, %function
	.thumb_func
Func_080dca50:
	.incbin "baserom.gba", 0x000dca0c, 0x00000034
	.section .rom.000dca40, "ax"
	.global Func_080dca84
	.type Func_080dca84, %function
	.thumb_func
Func_080dca84:
	.incbin "baserom.gba", 0x000dca40, 0x00000058
	.section .rom.000dca98, "ax"
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000dca98, 0x00000478
	.section .rom.000dcf2c, "ax"
	.incbin "baserom.gba", 0x000dcf2c, 0x0000001c
	.section .rom.000dd010, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000dd010, 0x000004d4
	.section .rom.000dd4e4, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000dd4e4, 0x00000114
	.section .rom.000dd5f8, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000dd5f8, 0x0000002c
	.section .rom.000dd624, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000dd624, 0x00000f10
	.section .rom.000de560, "ax"
	.incbin "baserom.gba", 0x000de560, 0x00000714
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000dec74, 0x000000f0
	.section .rom.000ded9c, "ax"
	.incbin "baserom.gba", 0x000ded9c, 0x00000ef4
	.section .rom.000dfcb4, "ax"
	.incbin "baserom.gba", 0x000dfcb4, 0x000005f4
	.section .rom.000e0318, "ax"
	.global Func_080e035c
	.type Func_080e035c, %function
	.thumb_func
Func_080e035c:
	.incbin "baserom.gba", 0x000e0318, 0x00000050
	.section .rom.000e037e, "ax"
	.incbin "baserom.gba", 0x000e037e, 0x00000fba
	.section .rom.000e1338, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000e1338, 0x000000a4
	.section .rom.000e13dc, "ax"
	.global Func_080e1420
	.type Func_080e1420, %function
	.thumb_func
Func_080e1420:
	.incbin "baserom.gba", 0x000e13dc, 0x000001dc
	.section .rom.000e15b8, "ax"
	.global Func_080e15fc
	.type Func_080e15fc, %function
	.thumb_func
Func_080e15fc:
	.incbin "baserom.gba", 0x000e15b8, 0x00000054
	.section .rom.000e160c, "ax"
	.global Func_080e1650
	.type Func_080e1650, %function
	.thumb_func
Func_080e1650:
	.incbin "baserom.gba", 0x000e160c, 0x00000f98
	.section .rom.000e25a4, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000e25a4, 0x0000018c
	.section .rom.000e2730, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000e2730, 0x00000160
	.section .rom.000e2890, "ax"
	.global Func_080e28d4
	.type Func_080e28d4, %function
	.thumb_func
Func_080e28d4:
	.incbin "baserom.gba", 0x000e2890, 0x00000dc4
	.section .rom.000e3654, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000e3654, 0x00000148
	.section .rom.000e379c, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000e379c, 0x00000a64
	.section .rom.000e4200, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000e4200, 0x00002eb4
	.section .rom.000e70b4, "ax"
	.global Func_080e70f8
	.type Func_080e70f8, %function
	.thumb_func
Func_080e70f8:
	.incbin "baserom.gba", 0x000e70b4, 0x00000140
	.section .rom.000e71f4, "ax"
	.global Func_080e7238
	.type Func_080e7238, %function
	.thumb_func
Func_080e7238:
	.incbin "baserom.gba", 0x000e71f4, 0x000002a0
	.section .rom.000e7494, "ax"
	.global Func_080e74d8
	.type Func_080e74d8, %function
	.thumb_func
Func_080e74d8:
	.incbin "baserom.gba", 0x000e7494, 0x0000032c
	.section .rom.000e77c0, "ax"
	.global Func_080e7804
	.type Func_080e7804, %function
	.thumb_func
Func_080e7804:
	.incbin "baserom.gba", 0x000e77c0, 0x000045f4
	.section .rom.000ebdea, "ax"
	.incbin "baserom.gba", 0x000ebdea, 0x00000002
	.global Func_080ebea8
	.type Func_080ebea8, %function
	.thumb_func
Func_080ebea8:
	.incbin "baserom.gba", 0x000ebdec, 0x0000000c
	.global Func_080ebeb4
	.type Func_080ebeb4, %function
	.thumb_func
Func_080ebeb4:
	.incbin "baserom.gba", 0x000ebdf8, 0x00000014
	.global Func_080ebec8
	.type Func_080ebec8, %function
	.thumb_func
Func_080ebec8:
	.incbin "baserom.gba", 0x000ebe0c, 0x000000a0
	.global BattleFx_ClearOwnedSlot
	.type BattleFx_ClearOwnedSlot, %function
	.thumb_func
BattleFx_ClearOwnedSlot:
	.incbin "baserom.gba", 0x000ebeac, 0x00001b2c
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x000ed9d8, 0x00001488
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000eee60, 0x00001240
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000f00a0, 0x00000cc0
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0d60, 0x00000050
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000f0db0, 0x00000088
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0e38, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000f16b4, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f31dc, 0x00000030
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f320c, 0x00000480
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f368c, 0x00004974
	.section .rom.000f80e0, "ax"
	.incbin "baserom.gba", 0x000f80e0, 0x0000003c
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x00000540
	.section .rom.000f8830, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8830, 0x00000390
	.section .rom.000f8c8e, "ax"
	.incbin "baserom.gba", 0x000f8c8e, 0x0000022a
	.section .rom.000f8f14, "ax"
	.incbin "baserom.gba", 0x000f8f14, 0x000001d4
	.section .rom.000f919a, "ax"
	.incbin "baserom.gba", 0x000f919a, 0x00000002
	.global Render_SetTilemapFlagRect
	.type Render_SetTilemapFlagRect, %function
	.thumb_func
Render_SetTilemapFlagRect:
	.incbin "baserom.gba", 0x000f919c, 0x00000088
	.section .rom.000f9224, "ax"
	.global Palette_CopyObjectBankToBackground14
	.type Palette_CopyObjectBankToBackground14, %function
	.thumb_func
Palette_CopyObjectBankToBackground14:
	.incbin "baserom.gba", 0x000f9224, 0x000000c8
	.section .rom.000f933e, "ax"
	.incbin "baserom.gba", 0x000f933e, 0x00000002
	.section .rom.000f942a, "ax"
	.incbin "baserom.gba", 0x000f942a, 0x00000002
	.section .rom.000f942c, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f942c, 0x00000ebc
	.section .rom.000fa36a, "ax"
	.incbin "baserom.gba", 0x000fa36a, 0x00000002
	.section .rom.000fa36c, "ax"
	.global ItemMenu_HideAllIcons
	.type ItemMenu_HideAllIcons, %function
	.thumb_func
ItemMenu_HideAllIcons:
	.incbin "baserom.gba", 0x000fa36c, 0x0000086c
	.section .rom.000fabea, "ax"
	.incbin "baserom.gba", 0x000fabea, 0x0000011e
	.global ItemMenu_DrawIcons
	.type ItemMenu_DrawIcons, %function
	.thumb_func
ItemMenu_DrawIcons:
	.incbin "baserom.gba", 0x000fad08, 0x000002d4
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
	.incbin "baserom.gba", 0x000fca7c, 0x0000171c
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
	.incbin "baserom.gba", 0x000fe2a8, 0x00000f80
	.section .rom.000ff2b0, "ax"
	.incbin "baserom.gba", 0x000ff2b0, 0x00001210
	.section .rom.00100548, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x00100548, 0x000000e4
	.section .rom.00100668, "ax"
	.global ItemMenu_ArrangeCategoryItemIcons
	.type ItemMenu_ArrangeCategoryItemIcons, %function
	.thumb_func
ItemMenu_ArrangeCategoryItemIcons:
	.incbin "baserom.gba", 0x00100668, 0x000000a0
	.global ItemMenu_PosCategory
	.type ItemMenu_PosCategory, %function
	.thumb_func
ItemMenu_PosCategory:
	.incbin "baserom.gba", 0x00100708, 0x0000077c
	.section .rom.00100e84, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00100e84, 0x00003c20
	.section .rom.00104b2e, "ax"
	.incbin "baserom.gba", 0x00104b2e, 0x00000002
	.section .rom.00104b30, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x00104b30, 0x00000250
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
	.incbin "baserom.gba", 0x001055ce, 0x00000356
	.global Data_08105948
Data_08105948:
	.incbin "baserom.gba", 0x00105924, 0x00000030
	.section .rom.00105954, "ax"
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00105954, 0x00000004
	.section .rom.00105958, "ax"
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00105958, 0x0000008e
	.section .rom.001059e6, "ax"
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x001059e6, 0x0000000d
	.section .rom.001059f3, "ax"
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x001059f3, 0x0000000d
	.section .rom.00105a00, "ax"
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00105a00, 0x00000018
	.section .rom.00105a18, "ax"
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00105a18, 0x00000018
	.section .rom.00105a30, "ax"
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x00105a30, 0x000025d0
	.section .rom.00108000, "ax"
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00108000, 0x00000008
	.global Func_08108008
	.type Func_08108008, %function
	.thumb_func
Func_08108008:
	.incbin "baserom.gba", 0x00108008, 0x00000008
	.global Func_08108010
	.type Func_08108010, %function
	.thumb_func
Func_08108010:
	.incbin "baserom.gba", 0x00108010, 0x00000008
	.global Func_08108018
	.type Func_08108018, %function
	.thumb_func
Func_08108018:
	.incbin "baserom.gba", 0x00108018, 0x00000008
	.global Func_08108020
	.type Func_08108020, %function
	.thumb_func
Func_08108020:
	.incbin "baserom.gba", 0x00108020, 0x00000008
	.global Func_08108028
	.type Func_08108028, %function
	.thumb_func
Func_08108028:
	.incbin "baserom.gba", 0x00108028, 0x00000008
	.section .rom.001080a8, "ax"
	.incbin "baserom.gba", 0x001080a8, 0x00000438
	.section .rom.001084f4, "ax"
	.incbin "baserom.gba", 0x001084f4, 0x00000268
	.section .rom.001087e0, "ax"
	.incbin "baserom.gba", 0x001087e0, 0x000000f8
	.section .rom.001088d8, "ax"
	.global Func_081088d8
	.type Func_081088d8, %function
	.thumb_func
Func_081088d8:
	.incbin "baserom.gba", 0x001088d8, 0x00000050
	.section .rom.00108928, "ax"
	.global Func_08108928
	.type Func_08108928, %function
	.thumb_func
Func_08108928:
	.incbin "baserom.gba", 0x00108928, 0x00000020
	.section .rom.00108948, "ax"
	.global Func_08108948
	.type Func_08108948, %function
	.thumb_func
Func_08108948:
	.incbin "baserom.gba", 0x00108948, 0x0000009c
	.section .rom.001089e4, "ax"
	.global Func_081089e4
	.type Func_081089e4, %function
	.thumb_func
Func_081089e4:
	.incbin "baserom.gba", 0x001089e4, 0x000000a4
	.section .rom.00108a88, "ax"
	.global Func_08108a88
	.type Func_08108a88, %function
	.thumb_func
Func_08108a88:
	.incbin "baserom.gba", 0x00108a88, 0x00000020
	.section .rom.00108aa8, "ax"
	.global Func_08108aa8
	.type Func_08108aa8, %function
	.thumb_func
Func_08108aa8:
	.incbin "baserom.gba", 0x00108aa8, 0x0000008c
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
	.incbin "baserom.gba", 0x0010a8fc, 0x000005b8
	.section .rom.0010af34, "ax"
	.incbin "baserom.gba", 0x0010af34, 0x000005fc
	.section .rom.0010b5d4, "ax"
	.incbin "baserom.gba", 0x0010b5d4, 0x000001d8
	.section .rom.0010b7ac, "ax"
	.global Func_0810b79c
	.type Func_0810b79c, %function
	.thumb_func
Func_0810b79c:
	.incbin "baserom.gba", 0x0010b7ac, 0x0000000c
	.section .rom.0010b7b8, "ax"
	.global Func_0810b7a8
	.type Func_0810b7a8, %function
	.thumb_func
Func_0810b7a8:
	.incbin "baserom.gba", 0x0010b7b8, 0x00000864
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0010c01c, 0x00000340
	.global Shop_SelectorOffsets
Shop_SelectorOffsets:
	.incbin "baserom.gba", 0x0010c35c, 0x0000003c
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x0010c398, 0x0000bc68
	.section .rom.00118000, "ax"
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000010
	.section .rom.00118058, "ax"
	.incbin "baserom.gba", 0x00118058, 0x00000010
	.section .rom.00118080, "ax"
	.global Battle_GetObjectTableValueFar
	.type Battle_GetObjectTableValueFar, %function
	.thumb_func
Battle_GetObjectTableValueFar:
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
	.incbin "baserom.gba", 0x00118738, 0x00000638
	.section .rom.00118e64, "ax"
	.incbin "baserom.gba", 0x00118e64, 0x00000108
	.section .rom.00118f6c, "ax"
	.global Func_08118f6c
	.type Func_08118f6c, %function
	.thumb_func
Func_08118f6c:
	.incbin "baserom.gba", 0x00118f6c, 0x000000e8
	.section .rom.00119148, "ax"
	.incbin "baserom.gba", 0x00119148, 0x00000158
	.section .rom.001192ce, "ax"
	.incbin "baserom.gba", 0x001192ce, 0x00000466
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
	.global BattleParty_PrepareReserveOwners
	.type BattleParty_PrepareReserveOwners, %function
	.thumb_func
BattleParty_PrepareReserveOwners:
	.incbin "baserom.gba", 0x0011a0ac, 0x000000d8
	.section .rom.0011a248, "ax"
	.incbin "baserom.gba", 0x0011a248, 0x000000d0
	.section .rom.0011a398, "ax"
	.global Func_0811a39c
	.type Func_0811a39c, %function
	.thumb_func
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a398, 0x000000b0
	.section .rom.0011a47e, "ax"
	.incbin "baserom.gba", 0x0011a47e, 0x00000002
	.section .rom.0011a480, "ax"
	.global BattleMotion_GetSlotField14
	.type BattleMotion_GetSlotField14, %function
	.thumb_func
BattleMotion_GetSlotField14:
	.incbin "baserom.gba", 0x0011a480, 0x0000000c
	.section .rom.0011a4da, "ax"
	.incbin "baserom.gba", 0x0011a4da, 0x00000c8e
	.section .rom.0011b17c, "ax"
	.incbin "baserom.gba", 0x0011b17c, 0x00000114
	.section .rom.0011b2c0, "ax"
	.incbin "baserom.gba", 0x0011b2c0, 0x00000214
	.global BattleUnit_BuildStatusFlags
	.type BattleUnit_BuildStatusFlags, %function
	.thumb_func
BattleUnit_BuildStatusFlags:
	.incbin "baserom.gba", 0x0011b4d4, 0x000001cc
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
	.section .rom.0011b9f8, "ax"
	.incbin "baserom.gba", 0x0011b9f8, 0x00000230
	.section .rom.0011bc60, "ax"
	.global ActivateBattleObjectSlot
	.type ActivateBattleObjectSlot, %function
	.thumb_func
ActivateBattleObjectSlot:
	.incbin "baserom.gba", 0x0011bc60, 0x000000ec
	.section .rom.0011bd4c, "ax"
	.global Func_0811bd50
	.type Func_0811bd50, %function
	.thumb_func
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd4c, 0x00000060
	.section .rom.0011bdd8, "ax"
	.incbin "baserom.gba", 0x0011bdd8, 0x00000060
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
	.incbin "baserom.gba", 0x0011c666, 0x00000062
	.section .rom.0011c70c, "ax"
	.incbin "baserom.gba", 0x0011c70c, 0x000004d8
	.section .rom.0011ccec, "ax"
	.global BattleMotion_SetMode5AndActivateSlot
	.type BattleMotion_SetMode5AndActivateSlot, %function
	.thumb_func
BattleMotion_SetMode5AndActivateSlot:
	.incbin "baserom.gba", 0x0011ccec, 0x0000008c
	.section .rom.0011cd78, "ax"
	.global Camera_ConfigureScene
	.type Camera_ConfigureScene, %function
	.thumb_func
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd78, 0x000000d4
	.section .rom.0011ce90, "ax"
	.incbin "baserom.gba", 0x0011ce90, 0x0000088c
	.section .rom.0011d744, "ax"
	.incbin "baserom.gba", 0x0011d744, 0x00000c24
	.section .rom.0011e3a6, "ax"
	.incbin "baserom.gba", 0x0011e3a6, 0x00000c86
	.section .rom.0011f02c, "ax"
	.global BattleMotion_SetRecordChildValues
	.type BattleMotion_SetRecordChildValues, %function
	.thumb_func
BattleMotion_SetRecordChildValues:
	.incbin "baserom.gba", 0x0011f02c, 0x00000300
	.section .rom.0011f3b4, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x0011f3b4, 0x00000a84
	.section .rom.0011ff02, "ax"
	.incbin "baserom.gba", 0x0011ff02, 0x00000002
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
	.incbin "baserom.gba", 0x0012005c, 0x00000118
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
	.incbin "baserom.gba", 0x00125bb4, 0x00000660
	.section .rom.00126214, "ax"
	.global Func_08126218
	.type Func_08126218, %function
	.thumb_func
Func_08126218:
	.incbin "baserom.gba", 0x00126214, 0x00000074
	.section .rom.00126288, "ax"
	.global Func_0812628c
	.type Func_0812628c, %function
	.thumb_func
Func_0812628c:
	.incbin "baserom.gba", 0x00126288, 0x00000170
	.section .rom.001263f8, "ax"
	.global Func_081263fc
	.type Func_081263fc, %function
	.thumb_func
Func_081263fc:
	.incbin "baserom.gba", 0x001263f8, 0x00000408
	.section .rom.00126800, "ax"
	.global BattlePres_SetupTransitionScene
	.type BattlePres_SetupTransitionScene, %function
	.thumb_func
BattlePres_SetupTransitionScene:
	.incbin "baserom.gba", 0x00126800, 0x00000100
	.section .rom.0012693e, "ax"
	.incbin "baserom.gba", 0x0012693e, 0x00000002
	.section .rom.00126940, "ax"
	.global Func_08126944
	.type Func_08126944, %function
	.thumb_func
Func_08126944:
	.incbin "baserom.gba", 0x00126940, 0x00000034
	.section .rom.00126974, "ax"
	.global Func_08126978
	.type Func_08126978, %function
	.thumb_func
Func_08126978:
	.incbin "baserom.gba", 0x00126974, 0x00000054
	.section .rom.001269e8, "ax"
	.incbin "baserom.gba", 0x001269e8, 0x000000f8
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
	.incbin "baserom.gba", 0x00126cf8, 0x00000ea4
	.section .rom.00127c7a, "ax"
	.incbin "baserom.gba", 0x00127c7a, 0x00000422
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
	.incbin "baserom.gba", 0x001280f8, 0x00000098
	.global Summon_GetEntryByte4
	.type Summon_GetEntryByte4, %function
	.thumb_func
Summon_GetEntryByte4:
	.incbin "baserom.gba", 0x00128190, 0x0000001c
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
	.incbin "baserom.gba", 0x00142944, 0x00000c9c
	.section .rom.001435e0, "ax"
	.global BattleFx_BeginCanvasLayer
	.type BattleFx_BeginCanvasLayer, %function
	.thumb_func
BattleFx_BeginCanvasLayer:
	.incbin "baserom.gba", 0x001435e0, 0x000004a8
	.section .rom.00143bb6, "ax"
	.incbin "baserom.gba", 0x00143bb6, 0x00005ff6
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
	.incbin "baserom.gba", 0x00152470, 0x00000084
	.section .rom.0015265a, "ax"
	.incbin "baserom.gba", 0x0015265a, 0x00000002
	.section .rom.0015265c, "ax"
	.global BattleFx_RunProjectileVolleyB
	.type BattleFx_RunProjectileVolleyB, %function
	.thumb_func
BattleFx_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x00003934
	.section .rom.0015613e, "ax"
	.incbin "baserom.gba", 0x0015613e, 0x00000002
	.section .rom.00156140, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x00156140, 0x000013a0
	.section .rom.00157530, "ax"
	.incbin "baserom.gba", 0x00157530, 0x000000bc
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x000006be
	.section .rom.00157d30, "ax"
	.incbin "baserom.gba", 0x00157d30, 0x00002314
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
	.section .rom.0016a29c, "ax"
	.incbin "baserom.gba", 0x0016a29c, 0x0001477c
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
	.incbin "baserom.gba", 0x001a0000, 0x00000008
	.global Func_081a0008
	.type Func_081a0008, %function
	.thumb_func
Func_081a0008:
	.incbin "baserom.gba", 0x001a0008, 0x00000008
	.global Func_081a0010
	.type Func_081a0010, %function
	.thumb_func
Func_081a0010:
	.incbin "baserom.gba", 0x001a0010, 0x00000008
	.global Func_081a0018
	.type Func_081a0018, %function
	.thumb_func
Func_081a0018:
	.incbin "baserom.gba", 0x001a0018, 0x00000008
	.global Func_081a0020
	.type Func_081a0020, %function
	.thumb_func
Func_081a0020:
	.incbin "baserom.gba", 0x001a0020, 0x00000008
	.global Func_081a0028
	.type Func_081a0028, %function
	.thumb_func
Func_081a0028:
	.incbin "baserom.gba", 0x001a0028, 0x00000008
	.section .rom.001a04d0, "ax"
	.incbin "baserom.gba", 0x001a04d0, 0x00000da4
	.section .rom.001a1280, "ax"
	.incbin "baserom.gba", 0x001a1280, 0x00004d80
	.global Resource_FarCall00B
Resource_FarCall00B:
	.incbin "baserom.gba", 0x001a6000, 0x00000018
	.global Func_081a6018
	.type Func_081a6018, %function
	.thumb_func
Func_081a6018:
	.incbin "baserom.gba", 0x001a6018, 0x00000008
	.global Func_081a6020
	.type Func_081a6020, %function
	.thumb_func
Func_081a6020:
	.incbin "baserom.gba", 0x001a6020, 0x00000008
	.global Func_081a6028
	.type Func_081a6028, %function
	.thumb_func
Func_081a6028:
	.incbin "baserom.gba", 0x001a6028, 0x0000223c
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
	.incbin "baserom.gba", 0x001b2008, 0x00000034
	.section .rom.001b20a0, "ax"
	.incbin "baserom.gba", 0x001b20a0, 0x00001c4c
	.section .rom.001b3d28, "ax"
	.incbin "baserom.gba", 0x001b3d28, 0x00000040
	.section .rom.001b3d90, "ax"
	.incbin "baserom.gba", 0x001b3d90, 0x00004270
	.section .rom.001b8008, "ax"
	.global Func_081b8008
	.type Func_081b8008, %function
	.thumb_func
Func_081b8008:
	.incbin "baserom.gba", 0x001b8008, 0x000000a0
	.section .rom.001b810c, "ax"
	.incbin "baserom.gba", 0x001b810c, 0x00007ef4
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
	.incbin "baserom.gba", 0x001c16ca, 0x00000022
	.global MusicTrack_FinishSequence
	.type MusicTrack_FinishSequence, %function
	.thumb_func
MusicTrack_FinishSequence:
	.incbin "baserom.gba", 0x001c16ec, 0x00000030
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
	.section .rom.001c339a, "ax"
	.incbin "baserom.gba", 0x001c339a, 0x00000002
	.section .rom.001c33ae, "ax"
	.incbin "baserom.gba", 0x001c33ae, 0x00000002
	.section .rom.001c3416, "ax"
	.incbin "baserom.gba", 0x001c3416, 0x00000002
	.section .rom.001c342a, "ax"
	.incbin "baserom.gba", 0x001c342a, 0x00000002
	.section .rom.001c342c, "ax"
	.global Audio_EmptyCallback
	.type Audio_EmptyCallback, %function
	.thumb_func
Audio_EmptyCallback:
	.incbin "baserom.gba", 0x001c342c, 0x00000010
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x001c343c, 0x00000090
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
