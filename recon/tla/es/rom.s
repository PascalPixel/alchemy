@ tla-es's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Cartridge_Restart
Cartridge_Restart:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00000630, "ax"
	.global SoundDriver_EnterFrameUpdate
	.thumb_func
	.type SoundDriver_EnterFrameUpdate, %function
SoundDriver_EnterFrameUpdate:
	.incbin "baserom.gba", 0x00000630, 0x00000088
	.section .rom.000019f4, "ax"
	.global Render_BuildOamList
	.thumb_func
	.type Render_BuildOamList, %function
Render_BuildOamList:
	.incbin "baserom.gba", 0x000019f4, 0x00000020
	.section .rom.00001c90, "ax"
	.global ColorBuffer_ScaleThreeQuarters
	.thumb_func
	.type ColorBuffer_ScaleThreeQuarters, %function
ColorBuffer_ScaleThreeQuarters:
	.incbin "baserom.gba", 0x00001c90, 0x00000020
	.size ColorBuffer_ScaleThreeQuarters, .-ColorBuffer_ScaleThreeQuarters
	.global ColorBuffer_Halve
	.thumb_func
	.type ColorBuffer_Halve, %function
ColorBuffer_Halve:
	.incbin "baserom.gba", 0x00001cb0, 0x00000020
	.size ColorBuffer_Halve, .-ColorBuffer_Halve
	.global ColorBuffer_Brighten
	.thumb_func
	.type ColorBuffer_Brighten, %function
ColorBuffer_Brighten:
	.incbin "baserom.gba", 0x00001cd0, 0x00000020
	.size ColorBuffer_Brighten, .-ColorBuffer_Brighten
	.global ColorBuffer_Darken
	.thumb_func
	.type ColorBuffer_Darken, %function
ColorBuffer_Darken:
	.incbin "baserom.gba", 0x00001cf0, 0x00000020
	.size ColorBuffer_Darken, .-ColorBuffer_Darken
	.section .rom.00013164, "ax"
	.global Sys_Free
	.thumb_func
	.type Sys_Free, %function
Sys_Free:
	.incbin "baserom.gba", 0x00013164, 0x00000038
	.section .rom.0001319c, "ax"
	.global Func_0801319c
	.thumb_func
	.type Func_0801319c, %function
Func_0801319c:
	.incbin "baserom.gba", 0x0001319c, 0x00000148
	.section .rom.000132e4, "ax"
	.global Func_080132b8
	.thumb_func
	.type Func_080132b8, %function
Func_080132b8:
	.incbin "baserom.gba", 0x000132e4, 0x00000004
	.section .rom.000132e8, "ax"
	.global Func_080132bc
	.thumb_func
	.type Func_080132bc, %function
Func_080132bc:
	.incbin "baserom.gba", 0x000132e8, 0x00000004
	.section .rom.000132ec, "ax"
	.global Func_080132c0
	.thumb_func
	.type Func_080132c0, %function
Func_080132c0:
	.incbin "baserom.gba", 0x000132ec, 0x00000004
	.section .rom.000132f0, "ax"
	.global Func_080132c4
	.thumb_func
	.type Func_080132c4, %function
Func_080132c4:
	.incbin "baserom.gba", 0x000132f0, 0x00000004
	.section .rom.000132f4, "ax"
	.global Func_080132c8
	.thumb_func
	.type Func_080132c8, %function
Func_080132c8:
	.incbin "baserom.gba", 0x000132f4, 0x00000004
	.section .rom.000132fc, "ax"
	.global Func_080132d0
	.thumb_func
	.type Func_080132d0, %function
Func_080132d0:
	.incbin "baserom.gba", 0x000132fc, 0x0000002c
	.section .rom.00013328, "ax"
	.global Func_080132fc
	.thumb_func
	.type Func_080132fc, %function
Func_080132fc:
	.incbin "baserom.gba", 0x00013328, 0x00000004
	.section .rom.000133f0, "ax"
	.balign 4
	.global Runtime_InstallIwramAndIrqs
	.thumb_func
	.type Runtime_InstallIwramAndIrqs, %function
Runtime_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133f0, 0x00000074
	.section .rom.00013464, "ax"
	.global Runtime_SetIrqHandler
	.thumb_func
	.type Runtime_SetIrqHandler, %function
Runtime_SetIrqHandler:
	.incbin "baserom.gba", 0x00013464, 0x00000078
	.global System_WaitForFrameInterrupt
	.thumb_func
	.type System_WaitForFrameInterrupt, %function
System_WaitForFrameInterrupt:
	.incbin "baserom.gba", 0x000134dc, 0x000000b0
	.section .rom.000138e0, "ax"
	.global Input_UpdateKeyRepeatAndDirection
	.thumb_func
	.type Input_UpdateKeyRepeatAndDirection, %function
Input_UpdateKeyRepeatAndDirection:
	.incbin "baserom.gba", 0x000138e0, 0x0000027c
	.section .rom.00013b5c, "ax"
	.global Sound_LoadPresetParameters
	.thumb_func
	.type Sound_LoadPresetParameters, %function
Sound_LoadPresetParameters:
	.incbin "baserom.gba", 0x00013b5c, 0x00000010
	.global Input_HandleKeyInterrupt
	.thumb_func
	.type Input_HandleKeyInterrupt, %function
Input_HandleKeyInterrupt:
	.incbin "baserom.gba", 0x00013b6c, 0x00000028
	.section .rom.00013db0, "ax"
	.incbin "baserom.gba", 0x00013db0, 0x000000ec
	.section .rom.00013e9c, "ax"
	.global Blend_SetDarkenTarget16
	.thumb_func
	.type Blend_SetDarkenTarget16, %function
Blend_SetDarkenTarget16:
	.incbin "baserom.gba", 0x00013e9c, 0x00000044
	.section .rom.00013ee0, "ax"
	.global Func_08013eb4
	.thumb_func
	.type Func_08013eb4, %function
Func_08013eb4:
	.incbin "baserom.gba", 0x00013ee0, 0x00000044
	.section .rom.00013f24, "ax"
	.global Func_08013ef8
	.thumb_func
	.type Func_08013ef8, %function
Func_08013ef8:
	.incbin "baserom.gba", 0x00013f24, 0x00000044
	.section .rom.00013f68, "ax"
	.global Func_08013f3c
	.thumb_func
	.type Func_08013f3c, %function
Func_08013f3c:
	.incbin "baserom.gba", 0x00013f68, 0x00000044
	.section .rom.00013fac, "ax"
	.global Func_08013f80
	.thumb_func
	.type Func_08013f80, %function
Func_08013f80:
	.incbin "baserom.gba", 0x00013fac, 0x0000005c
	.section .rom.00014028, "ax"
	.global Graphics_ResetFrameState
	.thumb_func
	.type Graphics_ResetFrameState, %function
Graphics_ResetFrameState:
	.incbin "baserom.gba", 0x00014028, 0x00000020
	.section .rom.00014048, "ax"
	.global AffineMatrix_BuildForEffect
	.thumb_func
	.type AffineMatrix_BuildForEffect, %function
AffineMatrix_BuildForEffect:
	.incbin "baserom.gba", 0x00014048, 0x000000bc
	.section .rom.00014104, "ax"
	.global Func_080140d8
	.thumb_func
	.type Func_080140d8, %function
Func_080140d8:
	.incbin "baserom.gba", 0x00014104, 0x0000009c
	.global ResourceTable_AllocateBlocks
	.thumb_func
	.type ResourceTable_AllocateBlocks, %function
ResourceTable_AllocateBlocks:
	.incbin "baserom.gba", 0x000141a0, 0x000000ac
	.section .rom.0001424c, "ax"
	.global Func_08014220
	.thumb_func
	.type Func_08014220, %function
Func_08014220:
	.incbin "baserom.gba", 0x0001424c, 0x00000020
	.section .rom.000142a0, "ax"
	.global Resource_ResetEntry
	.thumb_func
	.type Resource_ResetEntry, %function
Resource_ResetEntry:
	.incbin "baserom.gba", 0x000142a0, 0x00000038
	.section .rom.00014300, "ax"
	.global VramBlock_LoadCached
	.thumb_func
	.type VramBlock_LoadCached, %function
VramBlock_LoadCached:
	.incbin "baserom.gba", 0x00014300, 0x00000094
	.section .rom.00014394, "ax"
	.global Func_08014368
	.thumb_func
	.type Func_08014368, %function
Func_08014368:
	.incbin "baserom.gba", 0x00014394, 0x00000044
	.section .rom.000143d8, "ax"
	.global Resource_FindFreeEntry
	.thumb_func
	.type Resource_FindFreeEntry, %function
Resource_FindFreeEntry:
	.incbin "baserom.gba", 0x000143d8, 0x00000034
	.section .rom.00014422, "ax"
	.incbin "baserom.gba", 0x00014422, 0x00000002
	.section .rom.00014438, "ax"
	.incbin "baserom.gba", 0x00014438, 0x000000b4
	.section .rom.00014748, "ax"
	.global Scheduler_SetCallbackMask
	.thumb_func
	.type Scheduler_SetCallbackMask, %function
Scheduler_SetCallbackMask:
	.incbin "baserom.gba", 0x00014748, 0x00000040
	.section .rom.000148c8, "ax"
	.global Vector_AddPolarOffset
	.thumb_func
	.type Vector_AddPolarOffset, %function
Vector_AddPolarOffset:
	.incbin "baserom.gba", 0x000148c8, 0x0000004c
	.section .rom.00014914, "ax"
	.global ArcTan2
	.thumb_func
	.type ArcTan2, %function
ArcTan2:
	.incbin "baserom.gba", 0x00014914, 0x000000cc
	.section .rom.00014a0c, "ax"
	.global Func_080149e0
	.thumb_func
	.type Func_080149e0, %function
Func_080149e0:
	.incbin "baserom.gba", 0x00014a0c, 0x00000018
	.global Text_FormatHexToWork
	.thumb_func
	.type Text_FormatHexToWork, %function
Text_FormatHexToWork:
	.incbin "baserom.gba", 0x00014a24, 0x0000002c
	.section .rom.00014a50, "ax"
	.global Text_FormatSignedDecimalToWork
	.thumb_func
	.type Text_FormatSignedDecimalToWork, %function
Text_FormatSignedDecimalToWork:
	.incbin "baserom.gba", 0x00014a50, 0x00000084
	.section .rom.00014ad4, "ax"
	.global Func_08014aa8
	.thumb_func
	.type Func_08014aa8, %function
Func_08014aa8:
	.incbin "baserom.gba", 0x00014ad4, 0x00000038
	.section .rom.00014b0c, "ax"
	.global Runtime_WriteDebugTextTiles
	.thumb_func
	.type Runtime_WriteDebugTextTiles, %function
Runtime_WriteDebugTextTiles:
	.incbin "baserom.gba", 0x00014b0c, 0x00000050
	.section .rom.00014b9c, "ax"
	.global Func_08014b70
	.thumb_func
	.type Func_08014b70, %function
Func_08014b70:
	.incbin "baserom.gba", 0x00014b9c, 0x0000003c
	.section .rom.00014bd8, "ax"
	.global Func_08014bac
	.thumb_func
	.type Func_08014bac, %function
Func_08014bac:
	.incbin "baserom.gba", 0x00014bd8, 0x000000a0
	.section .rom.00014c78, "ax"
	.global Func_08014c4c
	.thumb_func
	.type Func_08014c4c, %function
Func_08014c4c:
	.incbin "baserom.gba", 0x00014c78, 0x00000020
	.section .rom.00014c98, "ax"
	.global Func_08014c6c
	.thumb_func
	.type Func_08014c6c, %function
Func_08014c6c:
	.incbin "baserom.gba", 0x00014c98, 0x00000034
	.section .rom.00014ccc, "ax"
	.global Func_08014ca0
	.thumb_func
	.type Func_08014ca0, %function
Func_08014ca0:
	.incbin "baserom.gba", 0x00014ccc, 0x00000010
	.section .rom.00014cdc, "ax"
	.global Func_08014cb0
	.thumb_func
	.type Func_08014cb0, %function
Func_08014cb0:
	.incbin "baserom.gba", 0x00014cdc, 0x00000010
	.section .rom.00014e0c, "ax"
	@ Uncredited four-byte tail; independent purpose is unproved.
	.incbin "baserom.gba", 0x00014e0c, 0x00000004
	.section .rom.00014e10, "ax"
	.global Func_08014de4
	.thumb_func
	.type Func_08014de4, %function
Func_08014de4:
	.incbin "baserom.gba", 0x00014e10, 0x00000038
	.section .rom.00014ed4, "ax"
	.global Func_08014ea8
	.thumb_func
	.type Func_08014ea8, %function
Func_08014ea8:
	.incbin "baserom.gba", 0x00014ed4, 0x00000038
	.section .rom.00014f0c, "ax"
	.global Func_08014ee0
	.thumb_func
	.type Func_08014ee0, %function
Func_08014ee0:
	.incbin "baserom.gba", 0x00014f0c, 0x0000001c
	.section .rom.00014f28, "ax"
	.global Func_08014efc
	.thumb_func
	.type Func_08014efc, %function
Func_08014efc:
	.incbin "baserom.gba", 0x00014f28, 0x00000128
	.section .rom.00015050, "ax"
	.global SceneTransform_ApplyPitch
	.thumb_func
	.type SceneTransform_ApplyPitch, %function
SceneTransform_ApplyPitch:
	.incbin "baserom.gba", 0x00015050, 0x00000044
	.section .rom.00015094, "ax"
	.global Func_08015068
	.thumb_func
	.type Func_08015068, %function
Func_08015068:
	.incbin "baserom.gba", 0x00015094, 0x0000007c
	.section .rom.00015110, "ax"
	.global Func_080150e4
	.thumb_func
	.type Func_080150e4, %function
Func_080150e4:
	.incbin "baserom.gba", 0x00015110, 0x00000044
	.section .rom.00015154, "ax"
	.global SceneTransform_ApplyPosition
	.thumb_func
	.type SceneTransform_ApplyPosition, %function
SceneTransform_ApplyPosition:
	.incbin "baserom.gba", 0x00015154, 0x00000084
	.section .rom.000151d8, "ax"
	.global Func_080151ac
	.thumb_func
	.type Func_080151ac, %function
Func_080151ac:
	.incbin "baserom.gba", 0x000151d8, 0x000000a0
	.section .rom.00015278, "ax"
	.global Func_0801524c
	.thumb_func
	.type Func_0801524c, %function
Func_0801524c:
	.incbin "baserom.gba", 0x00015278, 0x00000138
	.section .rom.000153b0, "ax"
	.global Func_08015384
	.thumb_func
	.type Func_08015384, %function
Func_08015384:
	.incbin "baserom.gba", 0x000153b0, 0x00000364
	.section .rom.00015714, "ax"
	.global Graphics_PrepareTransferInIwramWork
	.thumb_func
	.type Graphics_PrepareTransferInIwramWork, %function
Graphics_PrepareTransferInIwramWork:
	.incbin "baserom.gba", 0x00015714, 0x00000010
	.section .rom.00015724, "ax"
	.global Func_080156f8
	.thumb_func
	.type Func_080156f8, %function
Func_080156f8:
	.incbin "baserom.gba", 0x00015724, 0x0000001c
	.section .rom.00015740, "ax"
	.global Func_08015714
	.thumb_func
	.type Func_08015714, %function
Func_08015714:
	.incbin "baserom.gba", 0x00015740, 0x00000054
	.section .rom.000157a4, "ax"
	.global Render_ProjectPoint
	.thumb_func
	.type Render_ProjectPoint, %function
Render_ProjectPoint:
	.incbin "baserom.gba", 0x000157a4, 0x00000104
	.section .rom.000158a8, "ax"
	.global Resource_DecodeType01
	.thumb_func
	.type Resource_DecodeType01, %function
Resource_DecodeType01:
	.incbin "baserom.gba", 0x000158a8, 0x000000a0
	.section .rom.00015948, "ax"
	.global Resource_DecodeByteLzInRam
	.thumb_func
	.type Resource_DecodeByteLzInRam, %function
Resource_DecodeByteLzInRam:
	.incbin "baserom.gba", 0x00015948, 0x000006e0
	.section .rom.00016048, "ax"
	.incbin "baserom.gba", 0x00016048, 0x00000134
	.section .rom.000161ac, "ax"
	.global Func_08016180
	.thumb_func
	.type Func_08016180, %function
Func_08016180:
	.incbin "baserom.gba", 0x000161ac, 0x0000015c
	.section .rom.00016372, "ax"
	.incbin "baserom.gba", 0x00016372, 0x00000002
	.section .rom.00016374, "ax"
	.global Func_08016348
	.thumb_func
	.type Func_08016348, %function
Func_08016348:
	.incbin "baserom.gba", 0x00016374, 0x000000e8
	.section .rom.00016512, "ax"
	.incbin "baserom.gba", 0x00016512, 0x00000002
	.section .rom.00016570, "ax"
	.global SerialRuntime_PrepareSendPacket
	.thumb_func
	.type SerialRuntime_PrepareSendPacket, %function
SerialRuntime_PrepareSendPacket:
	.incbin "baserom.gba", 0x00016570, 0x00000070
	.global SerialRuntime_CollectReceivedPayloads
	.thumb_func
	.type SerialRuntime_CollectReceivedPayloads, %function
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x000165e0, 0x000000e0
	.global SerialRuntime_HandleTransferInterrupt
	.thumb_func
	.type SerialRuntime_HandleTransferInterrupt, %function
SerialRuntime_HandleTransferInterrupt:
	.incbin "baserom.gba", 0x000166c0, 0x00000118
	.section .rom.00016838, "ax"
	.global Func_0801680c
	.thumb_func
	.type Func_0801680c, %function
Func_0801680c:
	.incbin "baserom.gba", 0x00016838, 0x00000048
	.section .rom.00016880, "ax"
	.global SerialRuntime_BeginTransferB
	.thumb_func
	.type SerialRuntime_BeginTransferB, %function
SerialRuntime_BeginTransferB:
	.incbin "baserom.gba", 0x00016880, 0x0000004c
	.section .rom.000168cc, "ax"
	.global Func_080168a0
	.thumb_func
	.type Func_080168a0, %function
Func_080168a0:
	.incbin "baserom.gba", 0x000168cc, 0x0000002c
	.section .rom.000168f8, "ax"
	.global SerialRuntime_WaitForTransferB
	.thumb_func
	.type SerialRuntime_WaitForTransferB, %function
SerialRuntime_WaitForTransferB:
	.incbin "baserom.gba", 0x000168f8, 0x0000002c
	.section .rom.00016924, "ax"
	.global Func_080168f8
	.thumb_func
	.type Func_080168f8, %function
Func_080168f8:
	.incbin "baserom.gba", 0x00016924, 0x00000034
	.section .rom.00016958, "ax"
	.global Func_0801692c
	.thumb_func
	.type Func_0801692c, %function
Func_0801692c:
	.incbin "baserom.gba", 0x00016958, 0x00000024
	.global BattleLink_ResetTransferState
	.thumb_func
	.type BattleLink_ResetTransferState, %function
BattleLink_ResetTransferState:
	.incbin "baserom.gba", 0x0001697c, 0x00000040
	.section .rom.000169bc, "ax"
	.global Func_08016990
	.thumb_func
	.type Func_08016990, %function
Func_08016990:
	.incbin "baserom.gba", 0x000169bc, 0x00000314
	.section .rom.00016cd0, "ax"
	.global Owner_GetState
	.thumb_func
	.type Owner_GetState, %function
Owner_GetState:
	.incbin "baserom.gba", 0x00016cd0, 0x00000040
	.section .rom.00016d60, "ax"
	.global Func_08016d34
	.thumb_func
	.type Func_08016d34, %function
Func_08016d34:
	.incbin "baserom.gba", 0x00016d60, 0x00000028
	.section .rom.00016da8, "ax"
	.global Func_08016d7c
	.thumb_func
	.type Func_08016d7c, %function
Func_08016d7c:
	.incbin "baserom.gba", 0x00016da8, 0x0000001c
	.section .rom.00016dc4, "ax"
	.global Func_08016d98
	.thumb_func
	.type Func_08016d98, %function
Func_08016d98:
	.incbin "baserom.gba", 0x00016dc4, 0x0000001c
	.section .rom.00016dfc, "ax"
	.global Func_08016dd0
	.thumb_func
	.type Func_08016dd0, %function
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dfc, 0x00000028
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
	.section .rom.000178e0, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
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
	.incbin "baserom.gba", 0x00020010, 0x00000020
	.global Animation_ApplyChildArgumentFar
	.thumb_func
	.type Animation_ApplyChildArgumentFar, %function
Animation_ApplyChildArgumentFar:
	.incbin "baserom.gba", 0x00020030, 0x00000010
	.section .rom.00020060, "ax"
	.incbin "baserom.gba", 0x00020060, 0x00000008
	.global Func_08020038
	.thumb_func
	.type Func_08020038, %function
Func_08020038:
	.incbin "baserom.gba", 0x00020068, 0x00000008
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
	.type ResourceMetadata_ClearRecordFar, %function
ResourceMetadata_ClearRecordFar:
	.incbin "baserom.gba", 0x00020070, 0x00000008
	.global Animation_InitWorkFromMetadataFar
	.thumb_func
	.type Animation_InitWorkFromMetadataFar, %function
Animation_InitWorkFromMetadataFar:
	.incbin "baserom.gba", 0x00020078, 0x00000008
	.section .rom.00020180, "ax"
	.incbin "baserom.gba", 0x00020180, 0x00000018
	.global MapLayer_ClearField22Far
	.thumb_func
	.type MapLayer_ClearField22Far, %function
MapLayer_ClearField22Far:
	.incbin "baserom.gba", 0x00020198, 0x00000008
	.global MapLayer_SetField22Far
	.thumb_func
	.type MapLayer_SetField22Far, %function
MapLayer_SetField22Far:
	.incbin "baserom.gba", 0x000201a0, 0x00000008
	.global Map_DisableBlendScriptFar
	.thumb_func
	.type Map_DisableBlendScriptFar, %function
Map_DisableBlendScriptFar:
	.incbin "baserom.gba", 0x000201a8, 0x00000008
	.section .rom.000201f0, "ax"
	.global Map_GetTerrainHeightFar
	.thumb_func
	.type Map_GetTerrainHeightFar, %function
Map_GetTerrainHeightFar:
	.incbin "baserom.gba", 0x000201f0, 0x00000008
	.global Func_080201c8
	.thumb_func
	.type Func_080201c8, %function
Func_080201c8:
	.incbin "baserom.gba", 0x000201f8, 0x00000008
	.global Func_080201d0
	.thumb_func
	.type Func_080201d0, %function
Func_080201d0:
	.incbin "baserom.gba", 0x00020200, 0x00000008
	.global Func_080201d8
	.thumb_func
	.type Func_080201d8, %function
Func_080201d8:
	.incbin "baserom.gba", 0x00020208, 0x00000008
	.section .rom.00020238, "ax"
	.global Animation_SetIndexAndInitObjectsFar
	.thumb_func
	.type Animation_SetIndexAndInitObjectsFar, %function
Animation_SetIndexAndInitObjectsFar:
	.incbin "baserom.gba", 0x00020238, 0x00000008
	.global Func_08020240
	.thumb_func
	.type Func_08020240, %function
Func_08020240:
	.incbin "baserom.gba", 0x00020240, 0x00000008
	.global Func_08020248
	.thumb_func
	.type Func_08020248, %function
Func_08020248:
	.incbin "baserom.gba", 0x00020248, 0x00000008
	.global Animation_SetDisplayFlagFar
	.thumb_func
	.type Animation_SetDisplayFlagFar, %function
Animation_SetDisplayFlagFar:
	.incbin "baserom.gba", 0x00020250, 0x00000008
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
	.thumb_func
	.type Resource_GetMetadataRecordFar, %function
Resource_GetMetadataRecordFar:
	.incbin "baserom.gba", 0x00021918, 0x000000b4
	.section .rom.000219cc, "ax"
	.global Func_080237e8
	.thumb_func
	.type Func_080237e8, %function
Func_080237e8:
	.incbin "baserom.gba", 0x000219cc, 0x00000644
	.section .rom.00022010, "ax"
	.global Func_0802254c
	.thumb_func
	.type Func_0802254c, %function
Func_0802254c:
	.incbin "baserom.gba", 0x00022010, 0x00000790
	.section .rom.000227a0, "ax"
	.global Func_080227a0
	.thumb_func
	.type Func_080227a0, %function
Func_080227a0:
	.incbin "baserom.gba", 0x000227a0, 0x00000010
	.section .rom.000227b0, "ax"
	.global Animation_ApplyChildValuesToRecord
	.thumb_func
	.type Animation_ApplyChildValuesToRecord, %function
Animation_ApplyChildValuesToRecord:
	.incbin "baserom.gba", 0x000227b0, 0x00000030
	.section .rom.000227e0, "ax"
	.global Func_080227e0
	.thumb_func
	.type Func_080227e0, %function
Func_080227e0:
	.incbin "baserom.gba", 0x000227e0, 0x000000dc
	.global Animation_LookupValueByKey
	.thumb_func
	.type Animation_LookupValueByKey, %function
Animation_LookupValueByKey:
	.incbin "baserom.gba", 0x000228bc, 0x00000024
	.global InitializeAnimationObjects
	.thumb_func
	.type InitializeAnimationObjects, %function
InitializeAnimationObjects:
	.incbin "baserom.gba", 0x000228e0, 0x0000008c
	.section .rom.00022acc, "ax"
	.global Func_08022acc
	.thumb_func
	.type Func_08022acc, %function
Func_08022acc:
	.incbin "baserom.gba", 0x00022acc, 0x00000038
	.global Animation_ApplyChildArgument
	.thumb_func
	.type Animation_ApplyChildArgument, %function
Animation_ApplyChildArgument:
	.incbin "baserom.gba", 0x00022b04, 0x000000a8
	.section .rom.00022bac, "ax"
	.global Animation_ApplyChildValue
	.thumb_func
	.type Animation_ApplyChildValue, %function
Animation_ApplyChildValue:
	.incbin "baserom.gba", 0x00022bac, 0x0000002c
	.section .rom.00022bd8, "ax"
	.global Func_08022bd8
	.thumb_func
	.type Func_08022bd8, %function
Func_08022bd8:
	.incbin "baserom.gba", 0x00022bd8, 0x000000a0
	.section .rom.00022c78, "ax"
	.global Func_0802da88
	.thumb_func
	.type Func_0802da88, %function
Func_0802da88:
	.incbin "baserom.gba", 0x00022c78, 0x0000000c
	.section .rom.00022c84, "ax"
	.global Func_08022c78
	.thumb_func
	.type Func_08022c78, %function
Func_08022c78:
	.incbin "baserom.gba", 0x00022c84, 0x00000014
	.global Func_08022c98
	.thumb_func
	.type Func_08022c98, %function
Func_08022c98:
	.incbin "baserom.gba", 0x00022c98, 0x00000084
	.global ResourceMetadata_ClearRecord
	.thumb_func
	.type ResourceMetadata_ClearRecord, %function
ResourceMetadata_ClearRecord:
	.incbin "baserom.gba", 0x00022d1c, 0x00000024
	.section .rom.00022d40, "ax"
	.global ResourceObject_Create
	.thumb_func
	.type ResourceObject_Create, %function
ResourceObject_Create:
	.incbin "baserom.gba", 0x00022d40, 0x00000150
	.section .rom.00022f22, "ax"
	.incbin "baserom.gba", 0x00022f22, 0x00000002
	.section .rom.00022f24, "ax"
	.global Func_0802d7b0
	.thumb_func
	.type Func_0802d7b0, %function
Func_0802d7b0:
	.incbin "baserom.gba", 0x00022f24, 0x00000040
	.section .rom.00023088, "ax"
	.global Func_08023088
	.thumb_func
	.type Func_08023088, %function
Func_08023088:
	.incbin "baserom.gba", 0x00023088, 0x00000058
	.section .rom.000230e0, "ax"
	.global Func_080230e0
	.thumb_func
	.type Func_080230e0, %function
Func_080230e0:
	.incbin "baserom.gba", 0x000230e0, 0x000000c4
	.section .rom.00023220, "ax"
	.balign 4
	.global Func_08023220
	.thumb_func
	.type Func_08023220, %function
Func_08023220:
	.incbin "baserom.gba", 0x00023220, 0x00000188
	.section .rom.000233a8, "ax"
	.global ObjectDispatch_Initialize
	.thumb_func
	.type ObjectDispatch_Initialize, %function
ObjectDispatch_Initialize:
	.incbin "baserom.gba", 0x000233a8, 0x00000028
	.section .rom.00023510, "ax"
	.global Func_08023510
	.thumb_func
	.type Func_08023510, %function
Func_08023510:
	.incbin "baserom.gba", 0x00023510, 0x00000014
	.section .rom.00023524, "ax"
	.global Func_08023524
	.thumb_func
	.type Func_08023524, %function
Func_08023524:
	.incbin "baserom.gba", 0x00023524, 0x000000d4
	.section .rom.0002367e, "ax"
	.incbin "baserom.gba", 0x0002367e, 0x00000002
	.section .rom.00023680, "ax"
	.global Animation_SetStateFlags
	.thumb_func
	.type Animation_SetStateFlags, %function
Animation_SetStateFlags:
	.incbin "baserom.gba", 0x00023680, 0x00000028
	.global Animation_SetDisplayFlag
	.thumb_func
	.type Animation_SetDisplayFlag, %function
Animation_SetDisplayFlag:
	.incbin "baserom.gba", 0x000236a8, 0x00000028
	.section .rom.0002372a, "ax"
	.incbin "baserom.gba", 0x0002372a, 0x00000002
	.section .rom.0002372c, "ax"
	.global Func_0802372c
	.thumb_func
	.type Func_0802372c, %function
Func_0802372c:
	.incbin "baserom.gba", 0x0002372c, 0x000000bc
	.section .rom.000237e8, "ax"
	.global Func_0802d71c
	.thumb_func
	.type Func_0802d71c, %function
Func_0802d71c:
	.incbin "baserom.gba", 0x000237e8, 0x00000714
	.section .rom.00023f62, "ax"
	.incbin "baserom.gba", 0x00023f62, 0x00000002
	.section .rom.00023f64, "ax"
	.global Object_IsTargetUnset
	.thumb_func
	.type Object_IsTargetUnset, %function
Object_IsTargetUnset:
	.incbin "baserom.gba", 0x00023f64, 0x00000030
	.section .rom.00023f94, "ax"
	.global Func_08023f94
	.thumb_func
	.type Func_08023f94, %function
Func_08023f94:
	.incbin "baserom.gba", 0x00023f94, 0x00000788
	.section .rom.00024736, "ax"
	.incbin "baserom.gba", 0x00024736, 0x00000002
	.section .rom.00024738, "ax"
	.global Object_SetMoveTarget
	.thumb_func
	.type Object_SetMoveTarget, %function
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
	.thumb_func
	.type Func_08024f20, %function
Func_08024f20:
	.incbin "baserom.gba", 0x00024f20, 0x00000060
	.section .rom.00024f80, "ax"
	.global Func_08024f80
	.thumb_func
	.type Func_08024f80, %function
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
	.global Func_0802db64
	.thumb_func
	.type Func_0802db64, %function
Func_0802db64:
	.incbin "baserom.gba", 0x00026f80, 0x00000048
	.section .rom.00026fc8, "ax"
	.global Func_0802de8c
	.thumb_func
	.type Func_0802de8c, %function
Func_0802de8c:
	.incbin "baserom.gba", 0x00026fc8, 0x00003564
	.section .rom.0002a52c, "ax"
	.global Func_0802a52c
	.thumb_func
	.type Func_0802a52c, %function
Func_0802a52c:
	.incbin "baserom.gba", 0x0002a52c, 0x00000024
	.section .rom.0002a5e4, "ax"
	.global Func_0802a5e4
	.thumb_func
	.type Func_0802a5e4, %function
Func_0802a5e4:
	.incbin "baserom.gba", 0x0002a5e4, 0x0000006c
	.section .rom.0002a650, "ax"
	.global Func_08022c84
	.thumb_func
	.type Func_08022c84, %function
Func_08022c84:
	.incbin "baserom.gba", 0x0002a650, 0x00000068
	.section .rom.0002a6b8, "ax"
	.global Func_0802a6b8
	.thumb_func
	.type Func_0802a6b8, %function
Func_0802a6b8:
	.incbin "baserom.gba", 0x0002a6b8, 0x000003bc
	.section .rom.0002aa74, "ax"
	.global Func_0802aa74
	.thumb_func
	.type Func_0802aa74, %function
Func_0802aa74:
	.incbin "baserom.gba", 0x0002aa74, 0x00000528
	.section .rom.0002af9c, "ax"
	.global Func_0802af9c
	.thumb_func
	.type Func_0802af9c, %function
Func_0802af9c:
	.incbin "baserom.gba", 0x0002af9c, 0x00000204
	.section .rom.0002b1a0, "ax"
	.global Map_CopyMetatileIndicesRect
	.thumb_func
	.type Map_CopyMetatileIndicesRect, %function
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x0002b1a0, 0x00000134
	.section .rom.0002b2d4, "ax"
	.global Func_0802b2d4
	.thumb_func
	.type Func_0802b2d4, %function
Func_0802b2d4:
	.incbin "baserom.gba", 0x0002b2d4, 0x00000070
	.section .rom.0002b38c, "ax"
	.global Map_CopyCellAttributeRect
	.thumb_func
	.type Map_CopyCellAttributeRect, %function
Map_CopyCellAttributeRect:
	.incbin "baserom.gba", 0x0002b38c, 0x000000c4
	.global Func_0802b450
	.thumb_func
	.type Func_0802b450, %function
Func_0802b450:
	.incbin "baserom.gba", 0x0002b450, 0x00000140
	.global Func_0802b590
	.thumb_func
	.type Func_0802b590, %function
Func_0802b590:
	.incbin "baserom.gba", 0x0002b590, 0x00000090
	.section .rom.0002b620, "ax"
	.global Func_0802b620
	.thumb_func
	.type Func_0802b620, %function
Func_0802b620:
	.incbin "baserom.gba", 0x0002b620, 0x00000208
	.section .rom.0002b828, "ax"
	.global Func_08026f80
	.thumb_func
	.type Func_08026f80, %function
Func_08026f80:
	.incbin "baserom.gba", 0x0002b828, 0x00000050
	.global Map_WriteLayerCellTile
	.thumb_func
	.type Map_WriteLayerCellTile, %function
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x0002b878, 0x00000120
	.section .rom.0002b998, "ax"
	.global Func_0802b998
	.thumb_func
	.type Func_0802b998, %function
Func_0802b998:
	.incbin "baserom.gba", 0x0002b998, 0x000004b4
	.section .rom.0002be4c, "ax"
	.global Func_0802be4c
	.thumb_func
	.type Func_0802be4c, %function
Func_0802be4c:
	.incbin "baserom.gba", 0x0002be4c, 0x000003f4
	.section .rom.0002c240, "ax"
	.global Func_0802c240
	.thumb_func
	.type Func_0802c240, %function
Func_0802c240:
	.incbin "baserom.gba", 0x0002c240, 0x00000298
	.section .rom.0002c4d8, "ax"
	.global Func_0802c4d8
	.thumb_func
	.type Func_0802c4d8, %function
Func_0802c4d8:
	.incbin "baserom.gba", 0x0002c4d8, 0x000003c8
	.section .rom.0002c8a0, "ax"
	.global Func_0802c8a0
	.thumb_func
	.type Func_0802c8a0, %function
Func_0802c8a0:
	.incbin "baserom.gba", 0x0002c8a0, 0x00000128
	.section .rom.0002c9c8, "ax"
	.global Func_0802c9c8
	.thumb_func
	.type Func_0802c9c8, %function
Func_0802c9c8:
	.incbin "baserom.gba", 0x0002c9c8, 0x00000140
	.section .rom.0002cb08, "ax"
	.global Func_0802cb08
	.thumb_func
	.type Func_0802cb08, %function
Func_0802cb08:
	.incbin "baserom.gba", 0x0002cb08, 0x00000244
	.section .rom.0002cd4c, "ax"
	.global Func_0802cd4c
	.thumb_func
	.type Func_0802cd4c, %function
Func_0802cd4c:
	.incbin "baserom.gba", 0x0002cd4c, 0x00000024
	.section .rom.0002cd70, "ax"
	.global Func_0802cd70
	.thumb_func
	.type Func_0802cd70, %function
Func_0802cd70:
	.incbin "baserom.gba", 0x0002cd70, 0x000000dc
	.section .rom.0002ce4c, "ax"
	.global Func_0802b700
	.thumb_func
	.type Func_0802b700, %function
Func_0802b700:
	.incbin "baserom.gba", 0x0002ce4c, 0x00000054
	.section .rom.0002cea0, "ax"
	.global Func_0802b63c
	.thumb_func
	.type Func_0802b63c, %function
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
	.thumb_func
	.type Func_0802b71c, %function
Func_0802b71c:
	.incbin "baserom.gba", 0x0002d45c, 0x0000007c
	.section .rom.0002d4d8, "ax"
	.global Func_0802cc88
	.thumb_func
	.type Func_0802cc88, %function
Func_0802cc88:
	.incbin "baserom.gba", 0x0002d4d8, 0x00000058
	.section .rom.0002d530, "ax"
	.global Func_0802ceb0
	.thumb_func
	.type Func_0802ceb0, %function
Func_0802ceb0:
	.incbin "baserom.gba", 0x0002d530, 0x000000d0
	.section .rom.0002d600, "ax"
	.global Func_080237c8
	.thumb_func
	.type Func_080237c8, %function
Func_080237c8:
	.incbin "baserom.gba", 0x0002d600, 0x00000044
	.section .rom.0002d644, "ax"
	.global Func_08022f24
	.thumb_func
	.type Func_08022f24, %function
Func_08022f24:
	.incbin "baserom.gba", 0x0002d644, 0x00000014
	.section .rom.0002d658, "ax"
	.global Func_0802d658
	.thumb_func
	.type Func_0802d658, %function
Func_0802d658:
	.incbin "baserom.gba", 0x0002d658, 0x00000058
	.section .rom.0002d6b0, "ax"
	.global Func_0802cc74
	.thumb_func
	.type Func_0802cc74, %function
Func_0802cc74:
	.incbin "baserom.gba", 0x0002d6b0, 0x00000038
	.section .rom.0002d6e8, "ax"
	.global Func_0802d6e8
	.thumb_func
	.type Func_0802d6e8, %function
Func_0802d6e8:
	.incbin "baserom.gba", 0x0002d6e8, 0x00000194
	.section .rom.0002d87c, "ax"
	.global Func_0802d87c
	.thumb_func
	.type Func_0802d87c, %function
Func_0802d87c:
	.incbin "baserom.gba", 0x0002d87c, 0x000001a8
	.global Func_0802da24
	.thumb_func
	.type Func_0802da24, %function
Func_0802da24:
	.incbin "baserom.gba", 0x0002da24, 0x00000064
	.section .rom.0002da88, "ax"
	.global Func_0802db88
	.thumb_func
	.type Func_0802db88, %function
Func_0802db88:
	.incbin "baserom.gba", 0x0002da88, 0x00000038
	.global Func_0802b7e0
	.thumb_func
	.type Func_0802b7e0, %function
Func_0802b7e0:
	.section .rom.0002db64, "ax"
	.incbin "baserom.gba", 0x0002db64, 0x000000e4
	.section .rom.0002dc48, "ax"
	.global Func_0802dc48
	.thumb_func
	.type Func_0802dc48, %function
Func_0802dc48:
	.incbin "baserom.gba", 0x0002dc48, 0x00000074
	.global Func_0802dcbc
	.thumb_func
	.type Func_0802dcbc, %function
Func_0802dcbc:
	.incbin "baserom.gba", 0x0002dcbc, 0x0000001c
	.global Func_0802dcd8
	.thumb_func
	.type Func_0802dcd8, %function
Func_0802dcd8:
	.incbin "baserom.gba", 0x0002dcd8, 0x000001b4
	.section .rom.0002de8c, "ax"
	.global Func_0802a650
	.thumb_func
	.type Func_0802a650, %function
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
	.incbin "baserom.gba", 0x0002eec4, 0x00000100
	.global Map_TerrainHeightHandlers
Map_TerrainHeightHandlers:
	.incbin "baserom.gba", 0x0002efc4, 0x0000020c
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
	.section .rom.00038f40, "ax"
	.balign 4
	.global UiWork_InitializeWithResourceCounters
	.thumb_func
	.type UiWork_InitializeWithResourceCounters, %function
UiWork_InitializeWithResourceCounters:
	.incbin "baserom.gba", 0x00038f40, 0x000000c4
	.section .rom.00039004, "ax"
	.global UiWork_Initialize
	.thumb_func
	.type UiWork_Initialize, %function
UiWork_Initialize:
	.incbin "baserom.gba", 0x00039004, 0x00000118
	.section .rom.0003911c, "ax"
	.global UiWindow_EraseBorderRect
	.thumb_func
	.type UiWindow_EraseBorderRect, %function
UiWindow_EraseBorderRect:
	.incbin "baserom.gba", 0x0003911c, 0x00000144
	.section .rom.00039260, "ax"
	.global UiWindow_Create
	.thumb_func
	.type UiWindow_Create, %function
UiWindow_Create:
	.incbin "baserom.gba", 0x00039260, 0x00000114
	.section .rom.0003939a, "ax"
	.incbin "baserom.gba", 0x0003939a, 0x00000002
	.section .rom.00039416, "ax"
	.incbin "baserom.gba", 0x00039416, 0x00000002
	.section .rom.00039418, "ax"
	.global RenderOutput_RedrawSavedRect
	.thumb_func
	.type RenderOutput_RedrawSavedRect, %function
RenderOutput_RedrawSavedRect:
	.incbin "baserom.gba", 0x00039418, 0x00000018
	.section .rom.00039452, "ax"
	.incbin "baserom.gba", 0x00039452, 0x00000002
	.section .rom.00039454, "ax"
	.global UiWindow_ClearInteriorTiles
	.thumb_func
	.type UiWindow_ClearInteriorTiles, %function
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039454, 0x00000094
	.section .rom.0003950e, "ax"
	.incbin "baserom.gba", 0x0003950e, 0x00000002
	.section .rom.00039510, "ax"
	.global RenderOutput_Release
	.thumb_func
	.type RenderOutput_Release, %function
RenderOutput_Release:
	.incbin "baserom.gba", 0x00039510, 0x00000190
	.global Ui_ClearVramBlock
	.thumb_func
	.type Ui_ClearVramBlock, %function
Ui_ClearVramBlock:
	.incbin "baserom.gba", 0x000396a0, 0x0000003c
	.section .rom.0003972a, "ax"
	.incbin "baserom.gba", 0x0003972a, 0x0000090e
	.section .rom.0003a066, "ax"
	.incbin "baserom.gba", 0x0003a066, 0x00000002
	.section .rom.0003a068, "ax"
	.global Func_0803a084
	.thumb_func
	.type Func_0803a084, %function
Func_0803a084:
	.incbin "baserom.gba", 0x0003a068, 0x00000334
	.section .rom.0003a3c8, "ax"
	.balign 4
	.global UiWork_IsIdle
	.thumb_func
	.type UiWork_IsIdle, %function
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a3c8, 0x0000014c
	.section .rom.0003a514, "ax"
	.global Func_0803a530
	.thumb_func
	.type Func_0803a530, %function
Func_0803a530:
	.incbin "baserom.gba", 0x0003a514, 0x0000001c
	.section .rom.0003a530, "ax"
	.global Func_0803a54c
	.thumb_func
	.type Func_0803a54c, %function
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a530, 0x00000094
	.section .rom.0003a5c4, "ax"
	.global Func_0803a5e0
	.thumb_func
	.type Func_0803a5e0, %function
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a5c4, 0x0000002c
	.section .rom.0003a5f0, "ax"
	.global Func_0803a60c
	.thumb_func
	.type Func_0803a60c, %function
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5f0, 0x0000005c
	.section .rom.0003a680, "ax"
	.global UiText_OpenMessageWindow
	.thumb_func
	.type UiText_OpenMessageWindow, %function
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a680, 0x00000110
	.section .rom.0003a790, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
	.type UiText_ShowPositionedMessageAndWait, %function
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a790, 0x000002f8
	.section .rom.0003aaae, "ax"
	.incbin "baserom.gba", 0x0003aaae, 0x00000002
	.section .rom.0003aab0, "ax"
	.global UiText_RenderWideStringAtOffset
	.thumb_func
	.type UiText_RenderWideStringAtOffset, %function
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aab0, 0x0000032c
	.global UiText_FormatNumber
	.thumb_func
	.type UiText_FormatNumber, %function
UiText_FormatNumber:
	.incbin "baserom.gba", 0x0003addc, 0x00000268
	.section .rom.0003b044, "ax"
	.global UiText_BuildRenderEntries
	.thumb_func
	.type UiText_BuildRenderEntries, %function
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003b044, 0x0000083c
	.section .rom.0003b898, "ax"
	.global UiText_GetResourceDimensions
	.thumb_func
	.type UiText_GetResourceDimensions, %function
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b898, 0x0000004c
	.section .rom.0003b8e4, "ax"
	.global Func_0803b8cc
	.thumb_func
	.type Func_0803b8cc, %function
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8e4, 0x0000004c
	.section .rom.0003b930, "ax"
	.global UiText_MeasureEntryDimensions
	.thumb_func
	.type UiText_MeasureEntryDimensions, %function
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b930, 0x00000740
	.section .rom.0003c070, "ax"
	.global Func_0803c068
	.thumb_func
	.type Func_0803c068, %function
Func_0803c068:
	.incbin "baserom.gba", 0x0003c070, 0x00000108
	.section .rom.0003c178, "ax"
	.global Func_0803c170
	.thumb_func
	.type Func_0803c170, %function
Func_0803c170:
	.incbin "baserom.gba", 0x0003c178, 0x00000028
	.section .rom.0003c1a0, "ax"
	.global Func_0803c198
	.thumb_func
	.type Func_0803c198, %function
Func_0803c198:
	.incbin "baserom.gba", 0x0003c1a0, 0x000001e0
	.section .rom.0003c380, "ax"
	.global UiWindow_SetTilemapEntry
	.thumb_func
	.type UiWindow_SetTilemapEntry, %function
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c380, 0x00000634
	.section .rom.0003c9c4, "ax"
	.global UiText_CopyMessageString
	.thumb_func
	.type UiText_CopyMessageString, %function
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9c4, 0x00000064
	.section .rom.0003ca28, "ax"
	.global UiText_DecodeMessage
	.thumb_func
	.type UiText_DecodeMessage, %function
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca28, 0x00000144
	.section .rom.0003cb7a, "ax"
	.incbin "baserom.gba", 0x0003cb7a, 0x00000002
	.section .rom.0003cc06, "ax"
	.incbin "baserom.gba", 0x0003cc06, 0x00000102
	.section .rom.0003cd08, "ax"
	.global Func_0803cca8
	.thumb_func
	.type Func_0803cca8, %function
Func_0803cca8:
	.incbin "baserom.gba", 0x0003cd08, 0x00000028
	.section .rom.0003cd68, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd68, 0x00000114
	.section .rom.0003ce7c, "ax"
	.global Func_0803ce1c
	.thumb_func
	.type Func_0803ce1c, %function
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce7c, 0x00000048
	.section .rom.0003cec4, "ax"
	.global Func_0803ce64
	.thumb_func
	.type Func_0803ce64, %function
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cec4, 0x000001bc
	.section .rom.0003d080, "ax"
	.global Func_0803d020
	.thumb_func
	.type Func_0803d020, %function
Func_0803d020:
	.incbin "baserom.gba", 0x0003d080, 0x000002d0
	.section .rom.0003d350, "ax"
	.global Localization_LookupEntryId
	.thumb_func
	.type Localization_LookupEntryId, %function
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d350, 0x000000d0
	.section .rom.0003d420, "ax"
	.global Func_0803d3c0
	.thumb_func
	.type Func_0803d3c0, %function
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d420, 0x00000090
	.section .rom.0003d4b0, "ax"
	.global Func_0803d450
	.thumb_func
	.type Func_0803d450, %function
Func_0803d450:
	.incbin "baserom.gba", 0x0003d4b0, 0x0000006c
	.section .rom.0003d544, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.thumb_func
	.type Ui_BuildPairedPatternsToSlot, %function
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d544, 0x000000e0
	.section .rom.0003d624, "ax"
	.global Func_0803d5c4
	.thumb_func
	.type Func_0803d5c4, %function
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d624, 0x000000bc
	.section .rom.0003d6e0, "ax"
	.global ItemIcon_Compose
	.thumb_func
	.type ItemIcon_Compose, %function
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6e0, 0x0000022c
	.section .rom.0003d98a, "ax"
	.incbin "baserom.gba", 0x0003d98a, 0x00000002
	.section .rom.0003d98c, "ax"
	.global Func_0803d92c
	.thumb_func
	.type Func_0803d92c, %function
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d98c, 0x00000060
	.section .rom.0003da1a, "ax"
	.incbin "baserom.gba", 0x0003da1a, 0x00000002
	.section .rom.0003da1c, "ax"
	.global Func_0803d9bc
	.thumb_func
	.type Func_0803d9bc, %function
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da1c, 0x000000bc
	.section .rom.0003dad8, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.thumb_func
	.type Ui_PrepareTransferFromTableEntry, %function
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dad8, 0x000001a4
	.section .rom.0003dc7c, "ax"
	.global Func_0803dc1c
	.thumb_func
	.type Func_0803dc1c, %function
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc7c, 0x00000108
	.section .rom.0003dd84, "ax"
	.global Func_0803dd24
	.thumb_func
	.type Func_0803dd24, %function
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd84, 0x00000044
	.section .rom.0003ddc8, "ax"
	.global Func_0803dd68
	.thumb_func
	.type Func_0803dd68, %function
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddc8, 0x00000030
	.section .rom.0003ddf8, "ax"
	.global Func_0803dd98
	.thumb_func
	.type Func_0803dd98, %function
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddf8, 0x00000110
	.section .rom.0003df08, "ax"
	.global Func_0803dea8
	.thumb_func
	.type Func_0803dea8, %function
Func_0803dea8:
	.incbin "baserom.gba", 0x0003df08, 0x00000004
	.section .rom.0003df60, "ax"
	.global Func_0803df00
	.thumb_func
	.type Func_0803df00, %function
Func_0803df00:
	.incbin "baserom.gba", 0x0003df60, 0x00000014
	.section .rom.0003df74, "ax"
	.global Resource_ScheduleOwnerReset
	.thumb_func
	.type Resource_ScheduleOwnerReset, %function
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df74, 0x00000694
	.section .rom.0003e7d4, "ax"
	.global Func_0803e774
	.thumb_func
	.type Func_0803e774, %function
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7d4, 0x00000038
	.section .rom.0003e80c, "ax"
	.global Func_0803e7ac
	.thumb_func
	.type Func_0803e7ac, %function
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e80c, 0x00000140
	.section .rom.0003e976, "ax"
	.incbin "baserom.gba", 0x0003e976, 0x00000002
	.section .rom.0003e978, "ax"
	.global Func_0803e918
	.thumb_func
	.type Func_0803e918, %function
Func_0803e918:
	.incbin "baserom.gba", 0x0003e978, 0x00000080
	.section .rom.0003e9f8, "ax"
	.global Func_0803e998
	.thumb_func
	.type Func_0803e998, %function
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9f8, 0x0000063c
	.section .rom.0003f064, "ax"
	.incbin "baserom.gba", 0x0003f064, 0x000001d0
	.section .rom.0003f234, "ax"
	.global Resource_LoadByMode
	.thumb_func
	.type Resource_LoadByMode, %function
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f234, 0x00000060
	.section .rom.0003f294, "ax"
	.global Func_0803f234
	.thumb_func
	.type Func_0803f234, %function
Func_0803f234:
	.incbin "baserom.gba", 0x0003f294, 0x000003dc
	.section .rom.0003f670, "ax"
	.global Func_0803f610
	.thumb_func
	.type Func_0803f610, %function
Func_0803f610:
	.incbin "baserom.gba", 0x0003f670, 0x00000004
	.section .rom.0003f674, "ax"
	.global Func_0803f614
	.thumb_func
	.type Func_0803f614, %function
Func_0803f614:
	.incbin "baserom.gba", 0x0003f674, 0x0000000c
	.section .rom.0003f680, "ax"
	.global Func_0803f620
	.thumb_func
	.type Func_0803f620, %function
Func_0803f620:
	.incbin "baserom.gba", 0x0003f680, 0x00000004
	.section .rom.0003f684, "ax"
	.global UiTextResource_Initialize
	.thumb_func
	.type UiTextResource_Initialize, %function
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f684, 0x00000074
	.section .rom.0003f6f8, "ax"
	.global UiTextResource_SetPosition
	.thumb_func
	.type UiTextResource_SetPosition, %function
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6f8, 0x00000028
	.section .rom.0003f720, "ax"
	.global UiTextResource_Release
	.thumb_func
	.type UiTextResource_Release, %function
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f720, 0x00000098
	.global Resource_ResetPendingTransfer
	.thumb_func
	.type Resource_ResetPendingTransfer, %function
Resource_ResetPendingTransfer:
	.incbin "baserom.gba", 0x0003f7b8, 0x00000020
	.section .rom.0003f7d8, "ax"
	.global Func_0803f778
	.thumb_func
	.type Func_0803f778, %function
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7d8, 0x000000f4
	.section .rom.0003f8cc, "ax"
	.global Func_0803f86c
	.thumb_func
	.type Func_0803f86c, %function
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8cc, 0x000000d0
	.section .rom.0003f99c, "ax"
	.global UiTimedNotice_CloseIfActive
	.thumb_func
	.type UiTimedNotice_CloseIfActive, %function
UiTimedNotice_CloseIfActive:
	.incbin "baserom.gba", 0x0003f99c, 0x0000002c
	.section .rom.0003f9c8, "ax"
	.global Func_0803f968
	.thumb_func
	.type Func_0803f968, %function
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9c8, 0x00000058
	.section .rom.0003fa20, "ax"
	.global Func_0803f9c0
	.thumb_func
	.type Func_0803f9c0, %function
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa20, 0x00000728
	.section .rom.00040148, "ax"
	.global Func_080400e8
	.thumb_func
	.type Func_080400e8, %function
Func_080400e8:
	.incbin "baserom.gba", 0x00040148, 0x000015e4
	.global UiText_RenderStringTiles
	.thumb_func
	.type UiText_RenderStringTiles, %function
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0004172c, 0x000004a8
	.section .rom.00041bd4, "ax"
	.global Func_08041b68
	.thumb_func
	.type Func_08041b68, %function
Func_08041b68:
	.incbin "baserom.gba", 0x00041bd4, 0x000000a4
	.section .rom.00041c78, "ax"
	.global Func_08041c0c
	.thumb_func
	.type Func_08041c0c, %function
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c78, 0x00000048
	.section .rom.00041cc0, "ax"
	.global UiWindow_DrawDividerLine
	.thumb_func
	.type UiWindow_DrawDividerLine, %function
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041cc0, 0x0000031c
	.section .rom.00041fdc, "ax"
	.global Func_08041f70
	.thumb_func
	.type Func_08041f70, %function
Func_08041f70:
	.incbin "baserom.gba", 0x00041fdc, 0x00000020
	.section .rom.00041ffc, "ax"
	.global Func_08041f90
	.thumb_func
	.type Func_08041f90, %function
Func_08041f90:
	.incbin "baserom.gba", 0x00041ffc, 0x00000014
	.section .rom.00042010, "ax"
	.global UiText_DrawResource
	.thumb_func
	.type UiText_DrawResource, %function
UiText_DrawResource:
	.incbin "baserom.gba", 0x00042010, 0x0000006c
	.section .rom.0004207c, "ax"
	.global UiText_DrawCharacterAtOffset
	.thumb_func
	.type UiText_DrawCharacterAtOffset, %function
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x0004207c, 0x00000098
	.section .rom.00042114, "ax"
	.global UiText_DrawString
	.thumb_func
	.type UiText_DrawString, %function
UiText_DrawString:
	.incbin "baserom.gba", 0x00042114, 0x00000054
	.section .rom.00042168, "ax"
	.global UiText_DrawStringAtOffset
	.thumb_func
	.type UiText_DrawStringAtOffset, %function
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042168, 0x0000008c
	.section .rom.000421f4, "ax"
	.global UiText_DrawStringInWindow
	.thumb_func
	.type UiText_DrawStringInWindow, %function
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421f4, 0x0000005c
	.section .rom.00042250, "ax"
	.global UiText_DrawNumber
	.thumb_func
	.type UiText_DrawNumber, %function
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042250, 0x00000030
	.section .rom.00042280, "ax"
	.global UiText_DrawNumberAtOffset
	.thumb_func
	.type UiText_DrawNumberAtOffset, %function
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042280, 0x00000030
	.section .rom.000422b0, "ax"
	.global UiText_DrawNumberInWindow
	.thumb_func
	.type UiText_DrawNumberInWindow, %function
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x000422b0, 0x000000d0
	.section .rom.00042380, "ax"
	.global RenderOutput_Create
	.thumb_func
	.type RenderOutput_Create, %function
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042380, 0x00000088
	.section .rom.000424bc, "ax"
	.global Func_08042450
	.thumb_func
	.type Func_08042450, %function
Func_08042450:
	.incbin "baserom.gba", 0x000424bc, 0x000000b8
	.section .rom.00042574, "ax"
	.global Func_08042508
	.thumb_func
	.type Func_08042508, %function
Func_08042508:
	.incbin "baserom.gba", 0x00042574, 0x00000064
	.section .rom.000425e6, "ax"
	.incbin "baserom.gba", 0x000425e6, 0x00000002
	.section .rom.000425e8, "ax"
	.global Func_0804257c
	.thumb_func
	.type Func_0804257c, %function
Func_0804257c:
	.incbin "baserom.gba", 0x000425e8, 0x0000000c
	.section .rom.000425f4, "ax"
	.global Func_08042588
	.thumb_func
	.type Func_08042588, %function
Func_08042588:
	.incbin "baserom.gba", 0x000425f4, 0x00000074
	.section .rom.0004269c, "ax"
	.incbin "baserom.gba", 0x0004269c, 0x00000060
	.section .rom.000426fc, "ax"
	.global Func_08042690
	.thumb_func
	.type Func_08042690, %function
Func_08042690:
	.incbin "baserom.gba", 0x000426fc, 0x000002ec
	.section .rom.000429e8, "ax"
	.global Func_0804297c
	.thumb_func
	.type Func_0804297c, %function
Func_0804297c:
	.incbin "baserom.gba", 0x000429e8, 0x00000430
	.section .rom.00042e18, "ax"
	.global Func_08042dac
	.thumb_func
	.type Func_08042dac, %function
Func_08042dac:
	.incbin "baserom.gba", 0x00042e18, 0x00000318
	.section .rom.00043130, "ax"
	.global Func_080430c4
	.thumb_func
	.type Func_080430c4, %function
Func_080430c4:
	.incbin "baserom.gba", 0x00043130, 0x000000ec
	.global Text_FormatPlayTime
	.thumb_func
	.type Text_FormatPlayTime, %function
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x0004321c, 0x000000ac
	.section .rom.000432c8, "ax"
	.global Func_0804325c
	.thumb_func
	.type Func_0804325c, %function
Func_0804325c:
	.incbin "baserom.gba", 0x000432c8, 0x00000048
	.section .rom.00043310, "ax"
	.global Func_080432a4
	.thumb_func
	.type Func_080432a4, %function
Func_080432a4:
	.incbin "baserom.gba", 0x00043310, 0x00000248
	.section .rom.00043558, "ax"
	.global Func_080434ec
	.thumb_func
	.type Func_080434ec, %function
Func_080434ec:
	.incbin "baserom.gba", 0x00043558, 0x00000048
	.section .rom.000435a0, "ax"
	.global Func_08043534
	.thumb_func
	.type Func_08043534, %function
Func_08043534:
	.incbin "baserom.gba", 0x000435a0, 0x000000c8
	.section .rom.00043668, "ax"
	.global Func_080435fc
	.thumb_func
	.type Func_080435fc, %function
Func_080435fc:
	.incbin "baserom.gba", 0x00043668, 0x00000094
	.section .rom.000436fc, "ax"
	.global Func_08043690
	.thumb_func
	.type Func_08043690, %function
Func_08043690:
	.incbin "baserom.gba", 0x000436fc, 0x000000e4
	.section .rom.000437e0, "ax"
	.global Func_08043774
	.thumb_func
	.type Func_08043774, %function
Func_08043774:
	.incbin "baserom.gba", 0x000437e0, 0x00000098
	.section .rom.00043878, "ax"
	.global Func_0804380c
	.thumb_func
	.type Func_0804380c, %function
Func_0804380c:
	.incbin "baserom.gba", 0x00043878, 0x000000bc
	.section .rom.00043934, "ax"
	.global Func_080438c8
	.thumb_func
	.type Func_080438c8, %function
Func_080438c8:
	.incbin "baserom.gba", 0x00043934, 0x000000b0
	.section .rom.00043a30, "ax"
	.incbin "baserom.gba", 0x00043a30, 0x00000228
	.section .rom.00043d44, "ax"
	.incbin "baserom.gba", 0x00043d44, 0x00000788
	.section .rom.000444cc, "ax"
	.global Func_08044460
	.thumb_func
	.type Func_08044460, %function
Func_08044460:
	.incbin "baserom.gba", 0x000444cc, 0x00000018
	.section .rom.000444e4, "ax"
	.global Func_08044478
	.thumb_func
	.type Func_08044478, %function
Func_08044478:
	.incbin "baserom.gba", 0x000444e4, 0x00000010
	.section .rom.000444f4, "ax"
	.global Func_08044488
	.thumb_func
	.type Func_08044488, %function
Func_08044488:
	.incbin "baserom.gba", 0x000444f4, 0x000000d0
	.section .rom.000445c4, "ax"
	.global Func_08044558
	.thumb_func
	.type Func_08044558, %function
Func_08044558:
	.incbin "baserom.gba", 0x000445c4, 0x0000051c
	.section .rom.00044ae0, "ax"
	.global Func_08044a54
	.thumb_func
	.type Func_08044a54, %function
Func_08044a54:
	.incbin "baserom.gba", 0x00044ae0, 0x00000004
	.section .rom.00044ae4, "ax"
	.global Func_08044a58
	.thumb_func
	.type Func_08044a58, %function
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae4, 0x00000140
	.section .rom.00044c24, "ax"
	.global Func_08044b98
	.thumb_func
	.type Func_08044b98, %function
Func_08044b98:
	.incbin "baserom.gba", 0x00044c24, 0x000000e8
	.section .rom.00044d0c, "ax"
	.global Func_08044c80
	.thumb_func
	.type Func_08044c80, %function
Func_08044c80:
	.incbin "baserom.gba", 0x00044d0c, 0x00000148
	.section .rom.00044e54, "ax"
	.global Func_08044dc8
	.thumb_func
	.type Func_08044dc8, %function
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e54, 0x00000184
	.section .rom.00045014, "ax"
	.incbin "baserom.gba", 0x00045014, 0x000000c0
	.section .rom.000450d4, "ax"
	.global RenderResource_LoadFrame
	.thumb_func
	.type RenderResource_LoadFrame, %function
RenderResource_LoadFrame:
	.incbin "baserom.gba", 0x000450d4, 0x00000064
	.section .rom.00045188, "ax"
	.incbin "baserom.gba", 0x00045188, 0x00000150
	.section .rom.00045346, "ax"
	.incbin "baserom.gba", 0x00045346, 0x00000076
	.section .rom.000453bc, "ax"
	.global Func_08045330
	.thumb_func
	.type Func_08045330, %function
Func_08045330:
	.incbin "baserom.gba", 0x000453bc, 0x000000a0
	.section .rom.0004545c, "ax"
	.global Func_080453d0
	.thumb_func
	.type Func_080453d0, %function
Func_080453d0:
	.incbin "baserom.gba", 0x0004545c, 0x00000094
	.section .rom.000455b2, "ax"
	.incbin "baserom.gba", 0x000455b2, 0x0000002a
	.section .rom.000455f0, "ax"
	.incbin "baserom.gba", 0x000455f0, 0x0000004c
	.section .rom.00045668, "ax"
	.incbin "baserom.gba", 0x00045668, 0x0000018c
	.section .rom.0004580a, "ax"
	.incbin "baserom.gba", 0x0004580a, 0x00000032
	.section .rom.0004585a, "ax"
	.incbin "baserom.gba", 0x0004585a, 0x00000966
	.section .rom.000461c0, "ax"
	.global Func_08046134
	.thumb_func
	.type Func_08046134, %function
Func_08046134:
	.incbin "baserom.gba", 0x000461c0, 0x00000094
	.section .rom.00046254, "ax"
	.global BattleLayout_HighlightPartyPanels
	.thumb_func
	.type BattleLayout_HighlightPartyPanels, %function
BattleLayout_HighlightPartyPanels:
	.incbin "baserom.gba", 0x00046254, 0x00000098
	.section .rom.00046310, "ax"
	.incbin "baserom.gba", 0x00046310, 0x00000150
	.section .rom.0004649e, "ax"
	.incbin "baserom.gba", 0x0004649e, 0x0000364e
	.section .rom.00049b3a, "ax"
	.incbin "baserom.gba", 0x00049b3a, 0x00001f5e
	.section .rom.0004bad2, "ax"
	.incbin "baserom.gba", 0x0004bad2, 0x000002f2
	.section .rom.0004bdec, "ax"
	.incbin "baserom.gba", 0x0004bdec, 0x00000038
	.section .rom.0004be24, "ax"
	.global Func_0804bc08
	.thumb_func
	.type Func_0804bc08, %function
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be24, 0x000014d4
	.section .rom.0004d2f8, "ax"
	.global AffineEffect_InitializeWork
	.thumb_func
	.type AffineEffect_InitializeWork, %function
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d2f8, 0x0000003c
	.section .rom.0004d334, "ax"
	.global Menu_EndResourceSelection
	.thumb_func
	.type Menu_EndResourceSelection, %function
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d334, 0x00000054
	.section .rom.0004d388, "ax"
	.global Menu_RunResourceSelectionLoop
	.thumb_func
	.type Menu_RunResourceSelectionLoop, %function
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d388, 0x000001d8
	.section .rom.0004d78c, "ax"
	.global Menu_AnimateSelectionToEntry
	.thumb_func
	.type Menu_AnimateSelectionToEntry, %function
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d78c, 0x0000003c
	.section .rom.0004d7c8, "ax"
	.global Menu_SelectSaveSlotAction
	.thumb_func
	.type Menu_SelectSaveSlotAction, %function
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7c8, 0x00000228
	.section .rom.0004d9f0, "ax"
	.global Func_0804d7fc
	.thumb_func
	.type Func_0804d7fc, %function
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9f0, 0x0000016c
	.section .rom.0004dcae, "ax"
	.incbin "baserom.gba", 0x0004dcae, 0x000000ba
	.section .rom.0004dd68, "ax"
	.global Func_0804db74
	.thumb_func
	.type Func_0804db74, %function
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd68, 0x00000254
	.section .rom.0004e094, "ax"
	.global Menu_DrawFlagBitTable
	.thumb_func
	.type Menu_DrawFlagBitTable, %function
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e094, 0x000000c4
	.section .rom.0004e158, "ax"
	.global Menu_HandleFlagGridInput
	.thumb_func
	.type Menu_HandleFlagGridInput, %function
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e158, 0x00000140
	.section .rom.0004e2c2, "ax"
	.incbin "baserom.gba", 0x0004e2c2, 0x00000002
	.section .rom.0004e2c4, "ax"
	.global Func_0804e0d0
	.thumb_func
	.type Func_0804e0d0, %function
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2c4, 0x00000108
	.section .rom.0004e3cc, "ax"
	.global Func_0804e1d8
	.thumb_func
	.type Func_0804e1d8, %function
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3cc, 0x0000021c
	.section .rom.0004e5e8, "ax"
	.global Func_0804e3f4
	.thumb_func
	.type Func_0804e3f4, %function
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
	.incbin "baserom.gba", 0x00055018, 0x00004b50
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b68, 0x00000bf8
	.section .rom.0005c360, "ax"
	.incbin "baserom.gba", 0x0005c360, 0x00003590
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f8f0, 0x00000178
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fa68, 0x00000106
	.global Menu_TopEntryCommandByPosition
	.thumb_func
	.type Menu_TopEntryCommandByPosition, %function
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fb6e, 0x0000000c
	.global Menu_TopEntryPositionByCommand
	.thumb_func
	.type Menu_TopEntryPositionByCommand, %function
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fb7a, 0x00000062
	.section .rom.000a8fd8, "ax"
	.incbin "baserom.gba", 0x000a8fd8, 0x0000030c
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a92e4, 0x0000cd1c
	.section .rom.000b6070, "ax"
	.global Inventory_GetEquippedItemFar
	.thumb_func
	.type Inventory_GetEquippedItemFar, %function
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000b6070, 0x00000008
	.section .rom.000b6078, "ax"
	.global BattleAction_Get
	.thumb_func
	.type BattleAction_Get, %function
BattleAction_Get:
	.incbin "baserom.gba", 0x000b6078, 0x00000008
	.section .rom.000b6080, "ax"
	.global OwnerAction_AddFar
	.thumb_func
	.type OwnerAction_AddFar, %function
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000b6080, 0x00000008
	.section .rom.000b6088, "ax"
	.global Equipment_HasValueFar
	.thumb_func
	.type Equipment_HasValueFar, %function
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000b6088, 0x00000008
	.global Func_080ad090
	.thumb_func
	.type Func_080ad090, %function
Func_080ad090:
	.incbin "baserom.gba", 0x000b6090, 0x00000008
	.global Func_080ad098
	.thumb_func
	.type Func_080ad098, %function
Func_080ad098:
	.incbin "baserom.gba", 0x000b6098, 0x00000008
	.global Func_080ad0a0
	.thumb_func
	.type Func_080ad0a0, %function
Func_080ad0a0:
	.incbin "baserom.gba", 0x000b60a0, 0x00000008
	.global Func_080ad0a8
	.thumb_func
	.type Func_080ad0a8, %function
Func_080ad0a8:
	.incbin "baserom.gba", 0x000b60a8, 0x00000008
	.section .rom.000b60c0, "ax"
	.global Owner_AdjustFirstValueFar
	.thumb_func
	.type Owner_AdjustFirstValueFar, %function
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000b60c0, 0x00000008
	.global Owner_AdjustSecondValueFar
	.thumb_func
	.type Owner_AdjustSecondValueFar, %function
Owner_AdjustSecondValueFar:
	.incbin "baserom.gba", 0x000b60c8, 0x00000008
	.global Owner_RecalculateRatiosFar
	.thumb_func
	.type Owner_RecalculateRatiosFar, %function
Owner_RecalculateRatiosFar:
	.incbin "baserom.gba", 0x000b60d0, 0x00000008
	.section .rom.000b60d8, "ax"
	.global Owner_UpdateRatioPairFar
	.thumb_func
	.type Owner_UpdateRatioPairFar, %function
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000b60d8, 0x00000008
	.global Owner_UpdateSecondInputAndRatiosFar
	.thumb_func
	.type Owner_UpdateSecondInputAndRatiosFar, %function
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000b60e0, 0x00000008
	.global BattleUnit_AssignFar
	.thumb_func
	.type BattleUnit_AssignFar, %function
BattleUnit_AssignFar:
	.incbin "baserom.gba", 0x000b60e8, 0x00000008
	.section .rom.000b60f0, "ax"
	.global Party_CountActiveOwnersFar
	.thumb_func
	.type Party_CountActiveOwnersFar, %function
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000b60f0, 0x00000008
	.section .rom.000b60f8, "ax"
	.global Party_AddActiveOwnerFar
	.thumb_func
	.type Party_AddActiveOwnerFar, %function
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000b60f8, 0x00000008
	.global Party_ListActiveOwnersFar
	.thumb_func
	.type Party_ListActiveOwnersFar, %function
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000b6100, 0x00000008
	.global Func_080ad108
	.thumb_func
	.type Func_080ad108, %function
Func_080ad108:
	.incbin "baserom.gba", 0x000b6108, 0x00000008
	.global Party_RemoveActiveOwnerFar
	.thumb_func
	.type Party_RemoveActiveOwnerFar, %function
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000b6110, 0x00000008
	.global Func_080ad118
	.thumb_func
	.type Func_080ad118, %function
Func_080ad118:
	.incbin "baserom.gba", 0x000b6118, 0x00000008
	.global Battle_HitCheck
	.thumb_func
	.type Battle_HitCheck, %function
Battle_HitCheck:
	.incbin "baserom.gba", 0x000b6120, 0x00000008
	.global Battle_CalcAttack
	.thumb_func
	.type Battle_CalcAttack, %function
Battle_CalcAttack:
	.incbin "baserom.gba", 0x000b6128, 0x00000008
	.global Battle_CalcPower
	.thumb_func
	.type Battle_CalcPower, %function
Battle_CalcPower:
	.incbin "baserom.gba", 0x000b6130, 0x00000008
	.global Battle_CalcRestore
	.thumb_func
	.type Battle_CalcRestore, %function
Battle_CalcRestore:
	.incbin "baserom.gba", 0x000b6138, 0x00000008
	.global Owner_GetRecordFar
	.thumb_func
	.type Owner_GetRecordFar, %function
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000b6140, 0x00000008
	.section .rom.000b6148, "ax"
	.global BattleRandom16Far
	.thumb_func
	.type BattleRandom16Far, %function
BattleRandom16Far:
	.incbin "baserom.gba", 0x000b6148, 0x00000008
	.section .rom.000b6150, "ax"
	.global Djinn_AddToOwnerFar
	.thumb_func
	.type Djinn_AddToOwnerFar, %function
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000b6150, 0x00000008
	.section .rom.000b6158, "ax"
	.global Djinn_ActivateFar
	.thumb_func
	.type Djinn_ActivateFar, %function
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000b6158, 0x00000008
	.global Djinn_DeactivateFar
	.thumb_func
	.type Djinn_DeactivateFar, %function
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000b6160, 0x00000008
	.section .rom.000b6168, "ax"
	.global Trade_RemoveOfferFar
	.thumb_func
	.type Trade_RemoveOfferFar, %function
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b6168, 0x00000008
	.global Trade_AddOfferFar
	.thumb_func
	.type Trade_AddOfferFar, %function
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000b6170, 0x00000008
	.global Djinn_TransferFar
	.thumb_func
	.type Djinn_TransferFar, %function
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000b6178, 0x00000008
	.global Func_080ad180
	.thumb_func
	.type Func_080ad180, %function
Func_080ad180:
	.incbin "baserom.gba", 0x000b6180, 0x00000008
	.global SummonDefinition_Get
	.thumb_func
	.type SummonDefinition_Get, %function
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000b6188, 0x00000008
	.global Djinn_GetDefinitionHeaderFar
	.thumb_func
	.type Djinn_GetDefinitionHeaderFar, %function
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000b6190, 0x00000008
	.global Party_AdvanceOwnerCountToTargetFar
	.thumb_func
	.type Party_AdvanceOwnerCountToTargetFar, %function
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000b6198, 0x00000008
	.global Func_080ad1a0
	.thumb_func
	.type Func_080ad1a0, %function
Func_080ad1a0:
	.incbin "baserom.gba", 0x000b61a0, 0x00000008
	.global Trade_CountPendingOffersFar
	.thumb_func
	.type Trade_CountPendingOffersFar, %function
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000b61a8, 0x00000008
	.global Djinn_IsActiveFar
	.thumb_func
	.type Djinn_IsActiveFar, %function
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000b61b0, 0x00000008
	.global Trade_CanOfferDjinnFar
	.thumb_func
	.type Trade_CanOfferDjinnFar, %function
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000b61b8, 0x00000008
	.section .rom.000b61c0, "ax"
	.global Item_CanOwnerEquip
	.thumb_func
	.type Item_CanOwnerEquip, %function
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b61c0, 0x00000008
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
	.type Item_IsCompatibleWithOwnerFar, %function
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000b61c8, 0x00000008
	.global Inventory_FindEquippedFar
	.thumb_func
	.type Inventory_FindEquippedFar, %function
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000b61d0, 0x00000008
	.global Party_AdjustSixDigitCounterAFar
	.thumb_func
	.type Party_AdjustSixDigitCounterAFar, %function
Party_AdjustSixDigitCounterAFar:
	.incbin "baserom.gba", 0x000b61d8, 0x00000008
	.global Item_GetEquipmentGroupFar
	.thumb_func
	.type Item_GetEquipmentGroupFar, %function
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000b61e0, 0x00000008
	.global Item_AdjustCounterFar
	.thumb_func
	.type Item_AdjustCounterFar, %function
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000b61e8, 0x00000008
	.global Inventory_CountFar
	.thumb_func
	.type Inventory_CountFar, %function
Inventory_CountFar:
	.incbin "baserom.gba", 0x000b61f0, 0x00000008
	.global PartyInventory_HasSpaceFar
	.thumb_func
	.type PartyInventory_HasSpaceFar, %function
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000b61f8, 0x00000008
	.global Owner_GetLevelThresholdFar
	.thumb_func
	.type Owner_GetLevelThresholdFar, %function
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000b6200, 0x00000008
	.global Func_080ad208
	.thumb_func
	.type Func_080ad208, %function
Func_080ad208:
	.incbin "baserom.gba", 0x000b6208, 0x00000008
	.global Func_080ad210
	.thumb_func
	.type Func_080ad210, %function
Func_080ad210:
	.incbin "baserom.gba", 0x000b6210, 0x00000008
	.global Func_080ad218
	.thumb_func
	.type Func_080ad218, %function
Func_080ad218:
	.incbin "baserom.gba", 0x000b6218, 0x00000008
	.global Func_080ad220
	.thumb_func
	.type Func_080ad220, %function
Func_080ad220:
	.incbin "baserom.gba", 0x000b6220, 0x00000008
	.global Func_080ad228
	.thumb_func
	.type Func_080ad228, %function
Func_080ad228:
	.incbin "baserom.gba", 0x000b6228, 0x00000008
	.global Func_080ad230
	.thumb_func
	.type Func_080ad230, %function
Func_080ad230:
	.incbin "baserom.gba", 0x000b6230, 0x00000008
	.global Func_080ad238
	.thumb_func
	.type Func_080ad238, %function
Func_080ad238:
	.incbin "baserom.gba", 0x000b6238, 0x00000008
	.global Func_080ad240
	.thumb_func
	.type Func_080ad240, %function
Func_080ad240:
	.incbin "baserom.gba", 0x000b6240, 0x00000008
	.global Djinn_AddToLeastLoadedOwnerFar
	.thumb_func
	.type Djinn_AddToLeastLoadedOwnerFar, %function
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000b6248, 0x00000008
	.global Party_SumDjinnCountsFar
	.thumb_func
	.type Party_SumDjinnCountsFar, %function
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000b6250, 0x00000008
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
	.type Party_AdjustSixDigitCounterBFar, %function
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000b6258, 0x00000008
	.global Party_AdjustCounterCappedAt28Far
	.thumb_func
	.type Party_AdjustCounterCappedAt28Far, %function
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000b6260, 0x00000008
	.global Func_080ad268
	.thumb_func
	.type Func_080ad268, %function
Func_080ad268:
	.incbin "baserom.gba", 0x000b6268, 0x00000008
	.global Func_080ad270
	.thumb_func
	.type Func_080ad270, %function
Func_080ad270:
	.incbin "baserom.gba", 0x000b6270, 0x00000008
	.global Inventory_DiscardFar
	.thumb_func
	.type Inventory_DiscardFar, %function
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000b6278, 0x00000008
	.section .rom.000b6280, "ax"
	.global BattleFx_IsReviveFar
	.thumb_func
	.type BattleFx_IsReviveFar, %function
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000b6280, 0x00000008
	.global Owner_RefreshClassActionsFar
	.thumb_func
	.type Owner_RefreshClassActionsFar, %function
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000b6288, 0x00000008
	.global Func_080ad290
	.thumb_func
	.type Func_080ad290, %function
Func_080ad290:
	.incbin "baserom.gba", 0x000b6290, 0x00000008
	.global Owner_RefreshDerivedDataFar
	.thumb_func
	.type Owner_RefreshDerivedDataFar, %function
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000b6298, 0x00000008
	.global Inventory_CountItemFar
	.thumb_func
	.type Inventory_CountItemFar, %function
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000b62a0, 0x00000008
	.global PartyInventory_CountItemFar
	.thumb_func
	.type PartyInventory_CountItemFar, %function
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000b62a8, 0x00000008
	.global Func_080ad2b0
	.thumb_func
	.type Func_080ad2b0, %function
Func_080ad2b0:
	.incbin "baserom.gba", 0x000b62b0, 0x00000008
	.section .rom.000b6338, "ax"
	.global Func_080ad338
	.thumb_func
	.type Func_080ad338, %function
Func_080ad338:
	.incbin "baserom.gba", 0x000b6338, 0x00000010
	.section .rom.000b63f6, "ax"
	.incbin "baserom.gba", 0x000b63f6, 0x00000002
	.section .rom.000b63f8, "ax"
	.global Owner_RecalculateStats
	.thumb_func
	.type Owner_RecalculateStats, %function
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000b63f8, 0x000007f4
	.global GameFlag_RefreshLureCap
	.thumb_func
	.type GameFlag_RefreshLureCap, %function
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000b6bec, 0x000000a4
	.global Func_080adc90
	.thumb_func
	.type Func_080adc90, %function
Func_080adc90:
	.incbin "baserom.gba", 0x000b6c90, 0x000000cc
	.global Func_080add5c
	.thumb_func
	.type Func_080add5c, %function
Func_080add5c:
	.incbin "baserom.gba", 0x000b6d5c, 0x00000018
	.global Func_080add74
	.thumb_func
	.type Func_080add74, %function
Func_080add74:
	.incbin "baserom.gba", 0x000b6d74, 0x0000007c
	.global Func_080addf0
	.thumb_func
	.type Func_080addf0, %function
Func_080addf0:
	.incbin "baserom.gba", 0x000b6df0, 0x000002ec
	.global Func_080ae0dc
	.thumb_func
	.type Func_080ae0dc, %function
Func_080ae0dc:
	.incbin "baserom.gba", 0x000b70dc, 0x00000014
	.global Func_080ae0f0
	.thumb_func
	.type Func_080ae0f0, %function
Func_080ae0f0:
	.incbin "baserom.gba", 0x000b70f0, 0x00000028
	.global Func_080ae118
	.thumb_func
	.type Func_080ae118, %function
Func_080ae118:
	.incbin "baserom.gba", 0x000b7118, 0x00000054
	.global Func_080ae16c
	.thumb_func
	.type Func_080ae16c, %function
Func_080ae16c:
	.incbin "baserom.gba", 0x000b716c, 0x000000b4
	.global Func_080ae220
	.thumb_func
	.type Func_080ae220, %function
Func_080ae220:
	.incbin "baserom.gba", 0x000b7220, 0x000001f0
	.global Func_080ae410
	.thumb_func
	.type Func_080ae410, %function
Func_080ae410:
	.incbin "baserom.gba", 0x000b7410, 0x000001ec
	.global Func_080ae5fc
	.thumb_func
	.type Func_080ae5fc, %function
Func_080ae5fc:
	.incbin "baserom.gba", 0x000b75fc, 0x000000e8
	.global Func_080ae6e4
	.thumb_func
	.type Func_080ae6e4, %function
Func_080ae6e4:
	.incbin "baserom.gba", 0x000b76e4, 0x0000015c
	.section .rom.000b7874, "ax"
	.global Func_080ae868
	.thumb_func
	.type Func_080ae868, %function
Func_080ae868:
	.incbin "baserom.gba", 0x000b7874, 0x000001c8
	.section .rom.000b7c72, "ax"
	.incbin "baserom.gba", 0x000b7c72, 0x00000002
	.section .rom.000b7c74, "ax"
	.global Item_GetEquipmentGroup
	.thumb_func
	.type Item_GetEquipmentGroup, %function
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b7c74, 0x0000003c
	.global Inventory_GetQuantity
	.thumb_func
	.type Inventory_GetQuantity, %function
Inventory_GetQuantity:
	.incbin "baserom.gba", 0x000b7cb0, 0x00000024
	.section .rom.000b7e4c, "ax"
	.global Func_080aee40
	.thumb_func
	.type Func_080aee40, %function
Func_080aee40:
	.incbin "baserom.gba", 0x000b7e4c, 0x00000058
	.section .rom.000b7ea4, "ax"
	.global Inventory_Find
	.thumb_func
	.type Inventory_Find, %function
Inventory_Find:
	.incbin "baserom.gba", 0x000b7ea4, 0x00000030
	.section .rom.000b7f40, "ax"
	.global Inventory_Equip
	.thumb_func
	.type Inventory_Equip, %function
Inventory_Equip:
	.incbin "baserom.gba", 0x000b7f40, 0x000000d4
	.section .rom.000b80ee, "ax"
	.incbin "baserom.gba", 0x000b80ee, 0x00000002
	.global Func_080af0e4
	.thumb_func
	.type Func_080af0e4, %function
Func_080af0e4:
	.incbin "baserom.gba", 0x000b80f0, 0x00000064
	.section .rom.000b8154, "ax"
	.global Inventory_Remove
	.thumb_func
	.type Inventory_Remove, %function
Inventory_Remove:
	.incbin "baserom.gba", 0x000b8154, 0x00000080
	.section .rom.000b8206, "ax"
	.incbin "baserom.gba", 0x000b8206, 0x0000009e
	.section .rom.000b8344, "ax"
	.global Item_GetTargetMode
	.thumb_func
	.type Item_GetTargetMode, %function
Item_GetTargetMode:
	.incbin "baserom.gba", 0x000b8344, 0x00000040
	.section .rom.000b8384, "ax"
	.global Item_AdjustCounter
	.thumb_func
	.type Item_AdjustCounter, %function
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000b8384, 0x00000028
	.section .rom.000b8448, "ax"
	.global BattleAction_GetDirect
	.thumb_func
	.type BattleAction_GetDirect, %function
BattleAction_GetDirect:
	.incbin "baserom.gba", 0x000b8448, 0x00000028
	.global Func_080af464
	.thumb_func
	.type Func_080af464, %function
Func_080af464:
	.incbin "baserom.gba", 0x000b8470, 0x00000290
	.section .rom.000b879e, "ax"
	.incbin "baserom.gba", 0x000b879e, 0x0000000a
	.section .rom.000b87b8, "ax"
	.incbin "baserom.gba", 0x000b87b8, 0x00000124
	.global Owner_GetLevelThreshold
	.thumb_func
	.type Owner_GetLevelThreshold, %function
Owner_GetLevelThreshold:
	.incbin "baserom.gba", 0x000b88dc, 0x0000004c
	.section .rom.000b8928, "ax"
	.global Owner_LevelUp
	.thumb_func
	.type Owner_LevelUp, %function
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b8928, 0x00000264
	.global Func_080afb80
	.thumb_func
	.type Func_080afb80, %function
Func_080afb80:
	.incbin "baserom.gba", 0x000b8b8c, 0x00000034
	.section .rom.000b8bf8, "ax"
	.incbin "baserom.gba", 0x000b8bf8, 0x000001d0
	.section .rom.000b8de4, "ax"
	.incbin "baserom.gba", 0x000b8de4, 0x00000044
	.section .rom.000b8f34, "ax"
	.incbin "baserom.gba", 0x000b8f34, 0x00000054
	.section .rom.000b8fa0, "ax"
	.global Func_080aff94
	.thumb_func
	.type Func_080aff94, %function
Func_080aff94:
	.incbin "baserom.gba", 0x000b8fa0, 0x00000018
	.section .rom.000b8fb8, "ax"
	.global Owner_GetDigitValues
	.thumb_func
	.type Owner_GetDigitValues, %function
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000b8fb8, 0x0000007c
	.section .rom.000b9066, "ax"
	.incbin "baserom.gba", 0x000b9066, 0x00000002
	.global Owner_GetDefaultElement
	.thumb_func
	.type Owner_GetDefaultElement, %function
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000b9068, 0x0000022c
	.section .rom.000b92a4, "ax"
	.incbin "baserom.gba", 0x000b92a4, 0x0000003c
	.global Owner_RefreshClassActions
	.thumb_func
	.type Owner_RefreshClassActions, %function
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x000b92e0, 0x00000008
	.global Curve_LookupScaledValue
	.thumb_func
	.type Curve_LookupScaledValue, %function
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x000b92e8, 0x0000009c
	.section .rom.000b94c6, "ax"
	.incbin "baserom.gba", 0x000b94c6, 0x000005da
	.section .rom.000b9bc2, "ax"
	.incbin "baserom.gba", 0x000b9bc2, 0x000000c2
	.section .rom.000b9ca6, "ax"
	.incbin "baserom.gba", 0x000b9ca6, 0x00000002
	.section .rom.000b9ca8, "ax"
	.global Djinn_Activate
	.thumb_func
	.type Djinn_Activate, %function
Djinn_Activate:
	.incbin "baserom.gba", 0x000b9ca8, 0x00000068
	.section .rom.000b9d10, "ax"
	.global Djinn_Deactivate
	.thumb_func
	.type Djinn_Deactivate, %function
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000b9d10, 0x00000054
	.section .rom.000b9d64, "ax"
	.global Trade_RemoveOffer
	.thumb_func
	.type Trade_RemoveOffer, %function
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b9d64, 0x000000ac
	.section .rom.000ba00e, "ax"
	.incbin "baserom.gba", 0x000ba00e, 0x00000002
	.global Func_080b1004
	.thumb_func
	.type Func_080b1004, %function
Func_080b1004:
	.incbin "baserom.gba", 0x000ba010, 0x00000084
	.global Func_080b1088
	.thumb_func
	.type Func_080b1088, %function
Func_080b1088:
	.incbin "baserom.gba", 0x000ba094, 0x00000170
	.global Func_080b11f8
	.thumb_func
	.type Func_080b11f8, %function
Func_080b11f8:
	.incbin "baserom.gba", 0x000ba204, 0x000000d0
	.global Owner_ExperienceThresholds
Owner_ExperienceThresholds:
	.incbin "baserom.gba", 0x000ba2d4, 0x0000109c
	.section .rom.000bb370, "ax"
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000bb370, 0x000058b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x000c0c20, 0x00002268
	.global Enemy_DefinitionTable
Enemy_DefinitionTable:
	.incbin "baserom.gba", 0x000c2e88, 0x000070d0
	.global Owner_GrowthRecords
Owner_GrowthRecords:
	.incbin "baserom.gba", 0x000c9f58, 0x000005c0
	.section .rom.000ca518, "ax"
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x000ca518, 0x000000e8
	.section .rom.000ca600, "ax"
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000ca600, 0x000055bc
	.section .rom.000cfbbc, "ax"
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000cfbbc, 0x00001444
	.section .rom.000d1000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000d1000, 0x00000008
	.global Game_ResetForNewGameFar
	.thumb_func
	.type Game_ResetForNewGameFar, %function
Game_ResetForNewGameFar:
	.incbin "baserom.gba", 0x000d1008, 0x00000008
	.global Battle_WaitMode0Far
	.thumb_func
	.type Battle_WaitMode0Far, %function
Battle_WaitMode0Far:
	.incbin "baserom.gba", 0x000d1010, 0x00000008
	.global Func_080c8018
	.thumb_func
	.type Func_080c8018, %function
Func_080c8018:
	.incbin "baserom.gba", 0x000d1018, 0x00000008
	.global Func_080c8020
	.thumb_func
	.type Func_080c8020, %function
Func_080c8020:
	.incbin "baserom.gba", 0x000d1020, 0x00000008
	.global Event_RunObjectHookAndWaitFar
	.thumb_func
	.type Event_RunObjectHookAndWaitFar, %function
Event_RunObjectHookAndWaitFar:
	.incbin "baserom.gba", 0x000d1028, 0x00000008
	.global Event_CallWithLastActiveObjectIdFar
	.thumb_func
	.type Event_CallWithLastActiveObjectIdFar, %function
Event_CallWithLastActiveObjectIdFar:
	.incbin "baserom.gba", 0x000d1030, 0x00000008
	.global Scene_AssignViewFlagsFar
	.thumb_func
	.type Scene_AssignViewFlagsFar, %function
Scene_AssignViewFlagsFar:
	.incbin "baserom.gba", 0x000d1038, 0x00000008
	.global Event_SetWorkWord10Far
	.thumb_func
	.type Event_SetWorkWord10Far, %function
Event_SetWorkWord10Far:
	.incbin "baserom.gba", 0x000d1040, 0x00000008
	.global Event_PrepareObjectAndApplyValueFar
	.thumb_func
	.type Event_PrepareObjectAndApplyValueFar, %function
Event_PrepareObjectAndApplyValueFar:
	.incbin "baserom.gba", 0x000d1048, 0x00000008
	.global Event_PrepareTwoObjectsAndApplyFar
	.thumb_func
	.type Event_PrepareTwoObjectsAndApplyFar, %function
Event_PrepareTwoObjectsAndApplyFar:
	.incbin "baserom.gba", 0x000d1050, 0x00000008
	.global Party_RemoveOwnerRestoredFar
	.thumb_func
	.type Party_RemoveOwnerRestoredFar, %function
Party_RemoveOwnerRestoredFar:
	.incbin "baserom.gba", 0x000d1058, 0x00000008
	.global PartyInventory_GiveItemFar
	.thumb_func
	.type PartyInventory_GiveItemFar, %function
PartyInventory_GiveItemFar:
	.incbin "baserom.gba", 0x000d1060, 0x00000008
	.global Inventory_TryAddAndReturnOwnerFar
	.thumb_func
	.type Inventory_TryAddAndReturnOwnerFar, %function
Inventory_TryAddAndReturnOwnerFar:
	.incbin "baserom.gba", 0x000d1068, 0x00000008
	.global Inventory_PromptAndSetObjectModeFar
	.thumb_func
	.type Inventory_PromptAndSetObjectModeFar, %function
Inventory_PromptAndSetObjectModeFar:
	.incbin "baserom.gba", 0x000d1070, 0x00000008
	.global Func_080c8078
	.thumb_func
	.type Func_080c8078, %function
Func_080c8078:
	.incbin "baserom.gba", 0x000d1078, 0x00000008
	.global UiText_DrawQuantityPairWithCueFar
	.thumb_func
	.type UiText_DrawQuantityPairWithCueFar, %function
UiText_DrawQuantityPairWithCueFar:
	.incbin "baserom.gba", 0x000d1080, 0x00000008
	.global Object_GetByIdFar
	.thumb_func
	.type Object_GetByIdFar, %function
Object_GetByIdFar:
	.incbin "baserom.gba", 0x000d1088, 0x00000008
	.global Func_080c8090
	.thumb_func
	.type Func_080c8090, %function
Func_080c8090:
	.incbin "baserom.gba", 0x000d1090, 0x00000008
	.global ObjectMotion_SetSpeedParametersFar
	.thumb_func
	.type ObjectMotion_SetSpeedParametersFar, %function
ObjectMotion_SetSpeedParametersFar:
	.incbin "baserom.gba", 0x000d1098, 0x00000008
	.global ObjectMotion_EnableActionAndSetCallbackFar
	.thumb_func
	.type ObjectMotion_EnableActionAndSetCallbackFar, %function
ObjectMotion_EnableActionAndSetCallbackFar:
	.incbin "baserom.gba", 0x000d10a0, 0x00000008
	.global Object_RefreshSelectorByIdFar
	.thumb_func
	.type Object_RefreshSelectorByIdFar, %function
Object_RefreshSelectorByIdFar:
	.incbin "baserom.gba", 0x000d10a8, 0x00000008
	.global ObjectMotion_EnableActionAndResetMotionFar
	.thumb_func
	.type ObjectMotion_EnableActionAndResetMotionFar, %function
ObjectMotion_EnableActionAndResetMotionFar:
	.incbin "baserom.gba", 0x000d10b0, 0x00000008
	.global Object_SetActionCallbackAndRefreshByIdFar
	.thumb_func
	.type Object_SetActionCallbackAndRefreshByIdFar, %function
Object_SetActionCallbackAndRefreshByIdFar:
	.incbin "baserom.gba", 0x000d10b8, 0x00000008
	.global ObjectMotion_ResetAndSetPositionFar
	.thumb_func
	.type ObjectMotion_ResetAndSetPositionFar, %function
ObjectMotion_ResetAndSetPositionFar:
	.incbin "baserom.gba", 0x000d10c0, 0x00000008
	.global ObjectMotion_SetPositionAndCommitFar
	.thumb_func
	.type ObjectMotion_SetPositionAndCommitFar, %function
ObjectMotion_SetPositionAndCommitFar:
	.incbin "baserom.gba", 0x000d10c8, 0x00000008
	.global ObjectMotion_ResetAndSetPositionInMode2Far
	.thumb_func
	.type ObjectMotion_ResetAndSetPositionInMode2Far, %function
ObjectMotion_ResetAndSetPositionInMode2Far:
	.incbin "baserom.gba", 0x000d10d0, 0x00000008
	.global ObjectMotion_SetPositionAndResetFar
	.thumb_func
	.type ObjectMotion_SetPositionAndResetFar, %function
ObjectMotion_SetPositionAndResetFar:
	.incbin "baserom.gba", 0x000d10d8, 0x00000008
	.global ObjectMotion_SnapHeadingAndOffsetFar
	.thumb_func
	.type ObjectMotion_SnapHeadingAndOffsetFar, %function
ObjectMotion_SnapHeadingAndOffsetFar:
	.incbin "baserom.gba", 0x000d10e0, 0x00000008
	.global ObjectMotion_OffsetPositionAndResetMotionFar
	.thumb_func
	.type ObjectMotion_OffsetPositionAndResetMotionFar, %function
ObjectMotion_OffsetPositionAndResetMotionFar:
	.incbin "baserom.gba", 0x000d10e8, 0x00000008
	.global ObjectMotion_CommitCurrentPositionAndActivateFar
	.thumb_func
	.type ObjectMotion_CommitCurrentPositionAndActivateFar, %function
ObjectMotion_CommitCurrentPositionAndActivateFar:
	.incbin "baserom.gba", 0x000d10f0, 0x00000008
	.global Func_080c80f8
	.thumb_func
	.type Func_080c80f8, %function
Func_080c80f8:
	.incbin "baserom.gba", 0x000d10f8, 0x00000008
	.global Func_080c8100
	.thumb_func
	.type Func_080c8100, %function
Func_080c8100:
	.incbin "baserom.gba", 0x000d1100, 0x00000008
	.global Func_080c8108
	.thumb_func
	.type Func_080c8108, %function
Func_080c8108:
	.incbin "baserom.gba", 0x000d1108, 0x00000008
	.global Func_080c8110
	.thumb_func
	.type Func_080c8110, %function
Func_080c8110:
	.incbin "baserom.gba", 0x000d1110, 0x00000008
	.global Object_SetModeByIdFar
	.thumb_func
	.type Object_SetModeByIdFar, %function
Object_SetModeByIdFar:
	.incbin "baserom.gba", 0x000d1118, 0x00000008
	.global Object_SetActionByIdFar
	.thumb_func
	.type Object_SetActionByIdFar, %function
Object_SetActionByIdFar:
	.incbin "baserom.gba", 0x000d1120, 0x00000008
	.global Motion_SetModeAndWaitAnimationFar
	.thumb_func
	.type Motion_SetModeAndWaitAnimationFar, %function
Motion_SetModeAndWaitAnimationFar:
	.incbin "baserom.gba", 0x000d1128, 0x00000008
	.global ObjectMotion_WaitForAnimationChangeFar
	.thumb_func
	.type ObjectMotion_WaitForAnimationChangeFar, %function
ObjectMotion_WaitForAnimationChangeFar:
	.incbin "baserom.gba", 0x000d1130, 0x00000008
	.global ObjectMotion_LaunchFar
	.thumb_func
	.type ObjectMotion_LaunchFar, %function
ObjectMotion_LaunchFar:
	.incbin "baserom.gba", 0x000d1138, 0x00000008
	.global ObjectMotion_SetVariantCallbackFar
	.thumb_func
	.type ObjectMotion_SetVariantCallbackFar, %function
ObjectMotion_SetVariantCallbackFar:
	.incbin "baserom.gba", 0x000d1140, 0x00000008
	.global Motion_SetVarCbAndRefreshFar
	.thumb_func
	.type Motion_SetVarCbAndRefreshFar, %function
Motion_SetVarCbAndRefreshFar:
	.incbin "baserom.gba", 0x000d1148, 0x00000008
	.global Func_080c8150
	.thumb_func
	.type Func_080c8150, %function
Func_080c8150:
	.incbin "baserom.gba", 0x000d1150, 0x00000008
	.global ObjectMotion_SetAngleTowardFar
	.thumb_func
	.type ObjectMotion_SetAngleTowardFar, %function
ObjectMotion_SetAngleTowardFar:
	.incbin "baserom.gba", 0x000d1158, 0x00000008
	.global Object_LinkPairFar
	.thumb_func
	.type Object_LinkPairFar, %function
Object_LinkPairFar:
	.incbin "baserom.gba", 0x000d1160, 0x00000008
	.global Func_080c8168
	.thumb_func
	.type Func_080c8168, %function
Func_080c8168:
	.incbin "baserom.gba", 0x000d1168, 0x00000008
	.global Object_SetPartAttributeFar
	.thumb_func
	.type Object_SetPartAttributeFar, %function
Object_SetPartAttributeFar:
	.incbin "baserom.gba", 0x000d1170, 0x00000008
	.global Func_080c8178
	.thumb_func
	.type Func_080c8178, %function
Func_080c8178:
	.incbin "baserom.gba", 0x000d1178, 0x00000008
	.global Func_080c8180
	.thumb_func
	.type Func_080c8180, %function
Func_080c8180:
	.incbin "baserom.gba", 0x000d1180, 0x00000008
	.global UiText_OpenMessageAtObjectFar
	.thumb_func
	.type UiText_OpenMessageAtObjectFar, %function
UiText_OpenMessageAtObjectFar:
	.incbin "baserom.gba", 0x000d1188, 0x00000008
	.global Func_080c8190
	.thumb_func
	.type Func_080c8190, %function
Func_080c8190:
	.incbin "baserom.gba", 0x000d1190, 0x00000008
	.global Func_080c8198
	.thumb_func
	.type Func_080c8198, %function
Func_080c8198:
	.incbin "baserom.gba", 0x000d1198, 0x00000008
	.global Func_080c81a0
	.thumb_func
	.type Func_080c81a0, %function
Func_080c81a0:
	.incbin "baserom.gba", 0x000d11a0, 0x00000008
	.global Func_080c81a8
	.thumb_func
	.type Func_080c81a8, %function
Func_080c81a8:
	.incbin "baserom.gba", 0x000d11a8, 0x00000008
	.global Func_080c81b0
	.thumb_func
	.type Func_080c81b0, %function
Func_080c81b0:
	.incbin "baserom.gba", 0x000d11b0, 0x00000008
	.global Func_080c81b8
	.thumb_func
	.type Func_080c81b8, %function
Func_080c81b8:
	.incbin "baserom.gba", 0x000d11b8, 0x00000008
	.global Func_080c81c0
	.thumb_func
	.type Func_080c81c0, %function
Func_080c81c0:
	.incbin "baserom.gba", 0x000d11c0, 0x00000008
	.global Event_ShowCounterAtPositionFar
	.thumb_func
	.type Event_ShowCounterAtPositionFar, %function
Event_ShowCounterAtPositionFar:
	.incbin "baserom.gba", 0x000d11c8, 0x00000008
	.global ObjectMotion_ArmCallbackFar
	.thumb_func
	.type ObjectMotion_ArmCallbackFar, %function
ObjectMotion_ArmCallbackFar:
	.incbin "baserom.gba", 0x000d11d0, 0x00000008
	.global Func_080c81d8
	.thumb_func
	.type Func_080c81d8, %function
Func_080c81d8:
	.incbin "baserom.gba", 0x000d11d8, 0x00000008
	.global Func_080c81e0
	.thumb_func
	.type Func_080c81e0, %function
Func_080c81e0:
	.incbin "baserom.gba", 0x000d11e0, 0x00000008
	.global ObjectTable_RunIfActiveFar
	.thumb_func
	.type ObjectTable_RunIfActiveFar, %function
ObjectTable_RunIfActiveFar:
	.incbin "baserom.gba", 0x000d11e8, 0x00000008
	.global Func_080c81f0
	.thumb_func
	.type Func_080c81f0, %function
Func_080c81f0:
	.incbin "baserom.gba", 0x000d11f0, 0x00000008
	.global Func_080c81f8
	.thumb_func
	.type Func_080c81f8, %function
Func_080c81f8:
	.incbin "baserom.gba", 0x000d11f8, 0x00000008
	.global ObjectMotion_SetActionVariantFar
	.thumb_func
	.type ObjectMotion_SetActionVariantFar, %function
ObjectMotion_SetActionVariantFar:
	.incbin "baserom.gba", 0x000d1200, 0x00000008
	.global Func_080c8208
	.thumb_func
	.type Func_080c8208, %function
Func_080c8208:
	.incbin "baserom.gba", 0x000d1208, 0x00000008
	.global Func_080c8210
	.thumb_func
	.type Func_080c8210, %function
Func_080c8210:
	.incbin "baserom.gba", 0x000d1210, 0x00000008
	.global Func_080c8218
	.thumb_func
	.type Func_080c8218, %function
Func_080c8218:
	.incbin "baserom.gba", 0x000d1218, 0x00000008
	.global ObjectVisual_CopyAttributesFar
	.thumb_func
	.type ObjectVisual_CopyAttributesFar, %function
ObjectVisual_CopyAttributesFar:
	.incbin "baserom.gba", 0x000d1220, 0x00000008
	.global Object_AttachWorkTargetToObjectFar
	.thumb_func
	.type Object_AttachWorkTargetToObjectFar, %function
Object_AttachWorkTargetToObjectFar:
	.incbin "baserom.gba", 0x000d1228, 0x00000008
	.global Func_080c8230
	.thumb_func
	.type Func_080c8230, %function
Func_080c8230:
	.incbin "baserom.gba", 0x000d1230, 0x00000008
	.global Motion_CamBoundsFar
	.thumb_func
	.type Motion_CamBoundsFar, %function
Motion_CamBoundsFar:
	.incbin "baserom.gba", 0x000d1238, 0x00000008
	.global Func_080c8240
	.thumb_func
	.type Func_080c8240, %function
Func_080c8240:
	.incbin "baserom.gba", 0x000d1240, 0x00000008
	.global Func_080c8248
	.thumb_func
	.type Func_080c8248, %function
Func_080c8248:
	.incbin "baserom.gba", 0x000d1248, 0x00000008
	.global Func_080c8250
	.thumb_func
	.type Func_080c8250, %function
Func_080c8250:
	.incbin "baserom.gba", 0x000d1250, 0x00000008
	.global Func_080c8258
	.thumb_func
	.type Func_080c8258, %function
Func_080c8258:
	.incbin "baserom.gba", 0x000d1258, 0x00000008
	.global Func_080c8260
	.thumb_func
	.type Func_080c8260, %function
Func_080c8260:
	.incbin "baserom.gba", 0x000d1260, 0x00000008
	.section .rom.000d12a8, "ax"
	.incbin "baserom.gba", 0x000d12a8, 0x00000010
	.section .rom.000d13b8, "ax"
	.global Event_DelayEffectFramesFar
	.thumb_func
	.type Event_DelayEffectFramesFar, %function
Event_DelayEffectFramesFar:
	.incbin "baserom.gba", 0x000d13b8, 0x00000008
	.global Event_ClearStatus1c6Far
	.thumb_func
	.type Event_ClearStatus1c6Far, %function
Event_ClearStatus1c6Far:
	.incbin "baserom.gba", 0x000d13c0, 0x00000008
	.global Event_WaitValue1c8FramesFar
	.thumb_func
	.type Event_WaitValue1c8FramesFar, %function
Event_WaitValue1c8FramesFar:
	.incbin "baserom.gba", 0x000d13c8, 0x00000008
	.global Func_080c83c0
	.thumb_func
	.type Func_080c83c0, %function
Func_080c83c0:
	.incbin "baserom.gba", 0x000d13d0, 0x00000008
	.global Func_080c83c8
	.thumb_func
	.type Func_080c83c8, %function
Func_080c83c8:
	.incbin "baserom.gba", 0x000d13d8, 0x00000008
	.global BattleFx_PlayCueAndStartEmitterOnTargetFar
	.thumb_func
	.type BattleFx_PlayCueAndStartEmitterOnTargetFar, %function
BattleFx_PlayCueAndStartEmitterOnTargetFar:
	.incbin "baserom.gba", 0x000d13e0, 0x00000008
	.global Func_080c83d8
	.thumb_func
	.type Func_080c83d8, %function
Func_080c83d8:
	.incbin "baserom.gba", 0x000d13e8, 0x00000008
	.global Func_080c83e0
	.thumb_func
	.type Func_080c83e0, %function
Func_080c83e0:
	.incbin "baserom.gba", 0x000d13f0, 0x00000008
	.global Func_080c83e8
	.thumb_func
	.type Func_080c83e8, %function
Func_080c83e8:
	.incbin "baserom.gba", 0x000d13f8, 0x00000008
	.global Func_080c83f0
	.thumb_func
	.type Func_080c83f0, %function
Func_080c83f0:
	.incbin "baserom.gba", 0x000d1400, 0x00000008
	.global Func_080c83f8
	.thumb_func
	.type Func_080c83f8, %function
Func_080c83f8:
	.incbin "baserom.gba", 0x000d1408, 0x00000008
	.global Func_080c8400
	.thumb_func
	.type Func_080c8400, %function
Func_080c8400:
	.incbin "baserom.gba", 0x000d1410, 0x00000008
	.global Func_080c8408
	.thumb_func
	.type Func_080c8408, %function
Func_080c8408:
	.incbin "baserom.gba", 0x000d1418, 0x00000008
	.global Func_080c8410
	.thumb_func
	.type Func_080c8410, %function
Func_080c8410:
	.incbin "baserom.gba", 0x000d1420, 0x00000008
	.global Func_080c8418
	.thumb_func
	.type Func_080c8418, %function
Func_080c8418:
	.incbin "baserom.gba", 0x000d1428, 0x00000008
	.global Object_SetWideSpriteFar
	.thumb_func
	.type Object_SetWideSpriteFar, %function
Object_SetWideSpriteFar:
	.incbin "baserom.gba", 0x000d1430, 0x00000008
	.global Func_080c8428
	.thumb_func
	.type Func_080c8428, %function
Func_080c8428:
	.incbin "baserom.gba", 0x000d1438, 0x00000008
	.global Func_080c8430
	.thumb_func
	.type Func_080c8430, %function
Func_080c8430:
	.incbin "baserom.gba", 0x000d1440, 0x00000008
	.global Func_080c8438
	.thumb_func
	.type Func_080c8438, %function
Func_080c8438:
	.incbin "baserom.gba", 0x000d1448, 0x00000008
	.global Func_080c8440
	.thumb_func
	.type Func_080c8440, %function
Func_080c8440:
	.incbin "baserom.gba", 0x000d1450, 0x00000008
	.global Event_SpawnObjectTableFar
	.thumb_func
	.type Event_SpawnObjectTableFar, %function
Event_SpawnObjectTableFar:
	.incbin "baserom.gba", 0x000d1458, 0x00000008
	.global Func_080c8450
	.thumb_func
	.type Func_080c8450, %function
Func_080c8450:
	.incbin "baserom.gba", 0x000d1460, 0x00000008
	.global ObjectTable_GetFar
	.thumb_func
	.type ObjectTable_GetFar, %function
ObjectTable_GetFar:
	.incbin "baserom.gba", 0x000d1468, 0x00000008
	.global Func_080c8460
	.thumb_func
	.type Func_080c8460, %function
Func_080c8460:
	.incbin "baserom.gba", 0x000d1470, 0x00000008
	.global Func_080c8468
	.thumb_func
	.type Func_080c8468, %function
Func_080c8468:
	.incbin "baserom.gba", 0x000d1478, 0x00000008
	.global Func_080c8470
	.thumb_func
	.type Func_080c8470, %function
Func_080c8470:
	.incbin "baserom.gba", 0x000d1480, 0x00000008
	.global Func_080c8478
	.thumb_func
	.type Func_080c8478, %function
Func_080c8478:
	.incbin "baserom.gba", 0x000d1488, 0x00000008
	.global Func_080c8480
	.thumb_func
	.type Func_080c8480, %function
Func_080c8480:
	.incbin "baserom.gba", 0x000d1490, 0x00000008
	.global Func_080c8488
	.thumb_func
	.type Func_080c8488, %function
Func_080c8488:
	.incbin "baserom.gba", 0x000d1498, 0x00000008
	.global Func_080c8490
	.thumb_func
	.type Func_080c8490, %function
Func_080c8490:
	.incbin "baserom.gba", 0x000d14a0, 0x00000008
	.global Field_DispatchTypeHandlerFar
	.thumb_func
	.type Field_DispatchTypeHandlerFar, %function
Field_DispatchTypeHandlerFar:
	.incbin "baserom.gba", 0x000d14a8, 0x00000008
	.global Func_080c84a0
	.thumb_func
	.type Func_080c84a0, %function
Func_080c84a0:
	.incbin "baserom.gba", 0x000d14b0, 0x00000008
	.global Func_080c84a8
	.thumb_func
	.type Func_080c84a8, %function
Func_080c84a8:
	.incbin "baserom.gba", 0x000d14b8, 0x00000008
	.global Func_080c84b0
	.thumb_func
	.type Func_080c84b0, %function
Func_080c84b0:
	.incbin "baserom.gba", 0x000d14c0, 0x00000008
	.global Func_080c84b8
	.thumb_func
	.type Func_080c84b8, %function
Func_080c84b8:
	.incbin "baserom.gba", 0x000d14c8, 0x00000008
	.global Func_080c84c0
	.thumb_func
	.type Func_080c84c0, %function
Func_080c84c0:
	.incbin "baserom.gba", 0x000d14d0, 0x00000008
	.global BattleEffect_InitializeSharedSceneFar
	.thumb_func
	.type BattleEffect_InitializeSharedSceneFar, %function
BattleEffect_InitializeSharedSceneFar:
	.incbin "baserom.gba", 0x000d14d8, 0x00000008
	.global BattleEffect_ResolvePendingActionsFar
	.thumb_func
	.type BattleEffect_ResolvePendingActionsFar, %function
BattleEffect_ResolvePendingActionsFar:
	.incbin "baserom.gba", 0x000d14e0, 0x00000008
	.section .rom.000d1518, "ax"
	.global Func_080c8508
	.thumb_func
	.type Func_080c8508, %function
Func_080c8508:
	.incbin "baserom.gba", 0x000d1518, 0x00000008
	.global BattleFx_HasTriggerFar
	.thumb_func
	.type BattleFx_HasTriggerFar, %function
BattleFx_HasTriggerFar:
	.incbin "baserom.gba", 0x000d1520, 0x00000008
	.global Func_080c8518
	.thumb_func
	.type Func_080c8518, %function
Func_080c8518:
	.incbin "baserom.gba", 0x000d1528, 0x00000008
	.section .rom.000d1598, "ax"
	.global EffectSlot_UpdateFar
	.thumb_func
	.type EffectSlot_UpdateFar, %function
EffectSlot_UpdateFar:
	.incbin "baserom.gba", 0x000d1598, 0x00000008
	.global BattleFx_HasReachedTargetFar
	.thumb_func
	.type BattleFx_HasReachedTargetFar, %function
BattleFx_HasReachedTargetFar:
	.incbin "baserom.gba", 0x000d15a0, 0x00000008
	.global EffectSlot_SetPositionFar
	.thumb_func
	.type EffectSlot_SetPositionFar, %function
EffectSlot_SetPositionFar:
	.incbin "baserom.gba", 0x000d15a8, 0x00000008
	.global Func_080c85a0
	.thumb_func
	.type Func_080c85a0, %function
Func_080c85a0:
	.incbin "baserom.gba", 0x000d15b0, 0x00000008
	.global Func_080c85a8
	.thumb_func
	.type Func_080c85a8, %function
Func_080c85a8:
	.incbin "baserom.gba", 0x000d15b8, 0x00000008
	.global Camera_WorldToScreenFar
	.thumb_func
	.type Camera_WorldToScreenFar, %function
@ Complete8-byte veneer reaches the verified96-byte world-to-screen camera routine.
Camera_WorldToScreenFar:
	.incbin "baserom.gba", 0x000d15c0, 0x00000008
	.section .rom.000d1628, "ax"
	.global Object_BuildRecordSnapshotFar
	.thumb_func
	.type Object_BuildRecordSnapshotFar, %function
@ Complete8-byte far entry reaches the verified192-byte record snapshot builder.
Object_BuildRecordSnapshotFar:
	.incbin "baserom.gba", 0x000d1628, 0x00000010
	.section .rom.000d1658, "ax"
	.global Func_080c8648
	.thumb_func
	.type Func_080c8648, %function
Func_080c8648:
	.incbin "baserom.gba", 0x000d1658, 0x00000008
	.global Func_080c8650
	.thumb_func
	.type Func_080c8650, %function
Func_080c8650:
	.incbin "baserom.gba", 0x000d1660, 0x00000008
	.global Func_080c8658
	.thumb_func
	.type Func_080c8658, %function
Func_080c8658:
	.incbin "baserom.gba", 0x000d1668, 0x00000008
	.global Func_080c8660
	.thumb_func
	.type Func_080c8660, %function
Func_080c8660:
	.incbin "baserom.gba", 0x000d1670, 0x00000008
	.global Func_080c8668
	.thumb_func
	.type Func_080c8668, %function
Func_080c8668:
	.incbin "baserom.gba", 0x000d1678, 0x00000008
	.global Func_080c8670
	.thumb_func
	.type Func_080c8670, %function
Func_080c8670:
	.incbin "baserom.gba", 0x000d1680, 0x00000008
	.global Func_080c8678
	.thumb_func
	.type Func_080c8678, %function
Func_080c8678:
	.incbin "baserom.gba", 0x000d1688, 0x00000008
	.global Object_SetActionCallbackFar
	.thumb_func
	.type Object_SetActionCallbackFar, %function
Object_SetActionCallbackFar:
	.incbin "baserom.gba", 0x000d1690, 0x00000008
	.global Func_080c8688
	.thumb_func
	.type Func_080c8688, %function
Func_080c8688:
	.incbin "baserom.gba", 0x000d1698, 0x00000008
	.global Func_080c8690
	.thumb_func
	.type Func_080c8690, %function
Func_080c8690:
	.incbin "baserom.gba", 0x000d16a0, 0x00000008
	.section .rom.000d18d8, "ax"
	.global Func_080c88c8
	.thumb_func
	.type Func_080c88c8, %function
Func_080c88c8:
	.incbin "baserom.gba", 0x000d18d8, 0x00000008
	.global Func_080c88d0
	.thumb_func
	.type Func_080c88d0, %function
Func_080c88d0:
	.incbin "baserom.gba", 0x000d18e0, 0x00000008
	.global Func_080c88d8
	.thumb_func
	.type Func_080c88d8, %function
Func_080c88d8:
	.incbin "baserom.gba", 0x000d18e8, 0x00000004
	.global BattleFx_DecodeFrameUnusedBase
BattleFx_DecodeFrameUnusedBase:
	.incbin "baserom.gba", 0x000d18ec, 0x00000004
	.global Func_080c88e0
	.thumb_func
	.type Func_080c88e0, %function
Func_080c88e0:
	.incbin "baserom.gba", 0x000d18f0, 0x00000008
	.global Func_080c88e8
	.thumb_func
	.type Func_080c88e8, %function
Func_080c88e8:
	.incbin "baserom.gba", 0x000d18f8, 0x00000008
	.global Func_080c88f0
	.thumb_func
	.type Func_080c88f0, %function
Func_080c88f0:
	.incbin "baserom.gba", 0x000d1900, 0x00000008
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.thumb_func
	.type ObjectMotion_ResetTargetsAndVelocityFar, %function
ObjectMotion_ResetTargetsAndVelocityFar:
	.incbin "baserom.gba", 0x000d1908, 0x00000008
	.global ObjectEffect_BeginContextEffect26Far
	.thumb_func
	.type ObjectEffect_BeginContextEffect26Far, %function
ObjectEffect_BeginContextEffect26Far:
	.incbin "baserom.gba", 0x000d1910, 0x00000008
	.global Func_080c8908
	.thumb_func
	.type Func_080c8908, %function
Func_080c8908:
	.incbin "baserom.gba", 0x000d1918, 0x00000008
	.global Func_080c8910
	.thumb_func
	.type Func_080c8910, %function
Func_080c8910:
	.incbin "baserom.gba", 0x000d1920, 0x00000008
	.global Func_080c8918
	.thumb_func
	.type Func_080c8918, %function
Func_080c8918:
	.incbin "baserom.gba", 0x000d1928, 0x00000008
	.global Func_080c8920
	.thumb_func
	.type Func_080c8920, %function
Func_080c8920:
	.incbin "baserom.gba", 0x000d1930, 0x00000008
	.global Func_080c8928
	.thumb_func
	.type Func_080c8928, %function
Func_080c8928:
	.incbin "baserom.gba", 0x000d1938, 0x00000008
	.global Func_080c8930
	.thumb_func
	.type Func_080c8930, %function
Func_080c8930:
	.incbin "baserom.gba", 0x000d1940, 0x00000008
	.global Func_080c8938
	.thumb_func
	.type Func_080c8938, %function
Func_080c8938:
	.incbin "baserom.gba", 0x000d1948, 0x00000008
	.global Func_080c8940
	.thumb_func
	.type Func_080c8940, %function
Func_080c8940:
	.incbin "baserom.gba", 0x000d1950, 0x00000008
	.section .rom.000d19cc, "ax"
	.incbin "baserom.gba", 0x000d19cc, 0x00000a00
	.section .rom.000d2694, "ax"
	.incbin "baserom.gba", 0x000d2694, 0x0000028c
	.section .rom.000d2934, "ax"
	.incbin "baserom.gba", 0x000d2934, 0x00000010
	.global Game_ResetForNewGame
	.thumb_func
	.type Game_ResetForNewGame, %function
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000d2944, 0x000002cc
	.section .rom.000d2c38, "ax"
	.incbin "baserom.gba", 0x000d2c38, 0x00000190
	.section .rom.000d2dc8, "ax"
	.global Audio_PlayPartyCue
	.thumb_func
	.type Audio_PlayPartyCue, %function
Audio_PlayPartyCue:
	.incbin "baserom.gba", 0x000d2dc8, 0x00000164
	.global BattleFx_LookupResult
	.thumb_func
	.type BattleFx_LookupResult, %function
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000d2f2c, 0x000000ac
	.global Encounter_SelectEnemyGroup
	.thumb_func
	.type Encounter_SelectEnemyGroup, %function
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000d2fd8, 0x0000018c
	.section .rom.000d31bc, "ax"
	.global Audio_SelectPartyCueForArea
	.thumb_func
	.type Audio_SelectPartyCueForArea, %function
Audio_SelectPartyCueForArea:
	.incbin "baserom.gba", 0x000d31bc, 0x000000c4
	.section .rom.000d3280, "ax"
	.global Func_080d46a4
	.thumb_func
	.type Func_080d46a4, %function
Func_080d46a4:
	.incbin "baserom.gba", 0x000d3280, 0x00000214
	.section .rom.000d34a8, "ax"
	.incbin "baserom.gba", 0x000d34a8, 0x000001fc
	.section .rom.000d36a4, "ax"
	.global Event_SelectSpawnObjectMode
	.thumb_func
	.type Event_SelectSpawnObjectMode, %function
Event_SelectSpawnObjectMode:
	.incbin "baserom.gba", 0x000d36a4, 0x00000044
	.global Event_SpawnObjectTable
	.thumb_func
	.type Event_SpawnObjectTable, %function
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000d36e8, 0x000002e4
	.global Event_ReadSpawnCoordinates
	.thumb_func
	.type Event_ReadSpawnCoordinates, %function
Event_ReadSpawnCoordinates:
	.incbin "baserom.gba", 0x000d39cc, 0x000002f4
	.global ObjectTable_FindLastActiveId
	.thumb_func
	.type ObjectTable_FindLastActiveId, %function
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000d3cc0, 0x000000c4
	.section .rom.000d3d84, "ax"
	.global ObjectTable_Get
	.thumb_func
	.type ObjectTable_Get, %function
ObjectTable_Get:
	.incbin "baserom.gba", 0x000d3d84, 0x000001ec
	.section .rom.000d3fc4, "ax"
	.incbin "baserom.gba", 0x000d3fc4, 0x00000704
	.section .rom.000d482c, "ax"
	.global Func_080cb82c
	.thumb_func
	.type Func_080cb82c, %function
Func_080cb82c:
	.incbin "baserom.gba", 0x000d482c, 0x00000078
	.global Func_080cb8a4
	.thumb_func
	.type Func_080cb8a4, %function
Func_080cb8a4:
	.incbin "baserom.gba", 0x000d48a4, 0x000010f0
	.global Stream_ReadLittleEndianHalfword
	.thumb_func
	.type Stream_ReadLittleEndianHalfword, %function
Stream_ReadLittleEndianHalfword:
	.incbin "baserom.gba", 0x000d5994, 0x00000324
	.global BattleAction_FindDescriptor
	.thumb_func
	.type BattleAction_FindDescriptor, %function
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000d5cb8, 0x00000090
	.section .rom.000d5d76, "ax"
	.incbin "baserom.gba", 0x000d5d76, 0x00000002
	.section .rom.000d5d78, "ax"
	.global Func_080d25c8
	.thumb_func
	.type Func_080d25c8, %function
Func_080d25c8:
	.incbin "baserom.gba", 0x000d5d78, 0x00000150
	.section .rom.000d5ec8, "ax"
	.global Func_080d0520
	.thumb_func
	.type Func_080d0520, %function
Func_080d0520:
	.incbin "baserom.gba", 0x000d5ec8, 0x00000a54
	.section .rom.000d691c, "ax"
	.global Func_080cc54c
	.thumb_func
	.type Func_080cc54c, %function
Func_080cc54c:
	.incbin "baserom.gba", 0x000d691c, 0x000002dc
	.section .rom.000d6bf8, "ax"
	.global Func_080dd054
	.thumb_func
	.type Func_080dd054, %function
Func_080dd054:
	.incbin "baserom.gba", 0x000d6bf8, 0x00000048
	.section .rom.000d6c72, "ax"
	.incbin "baserom.gba", 0x000d6c72, 0x00000002
	.section .rom.000d6c74, "ax"
	.global Func_080eab98
	.thumb_func
	.type Func_080eab98, %function
Func_080eab98:
	.incbin "baserom.gba", 0x000d6c74, 0x0000010c
	.section .rom.000d6d80, "ax"
	.global Func_080eaf28
	.thumb_func
	.type Func_080eaf28, %function
Func_080eaf28:
	.incbin "baserom.gba", 0x000d6d80, 0x00000128
	.section .rom.000d6f80, "ax"
	.incbin "baserom.gba", 0x000d6f80, 0x00000030
	.section .rom.000d6fb0, "ax"
	.global Event_FindFacingTrigger
	.thumb_func
	.type Event_FindFacingTrigger, %function
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d6fb0, 0x000005c4
	.global EventRuntime_ExecutePackedAction
	.thumb_func
	.type EventRuntime_ExecutePackedAction, %function
EventRuntime_ExecutePackedAction:
	.incbin "baserom.gba", 0x000d7574, 0x000005e4
	.section .rom.000d7b58, "ax"
	.global Func_080dca84
	.thumb_func
	.type Func_080dca84, %function
Func_080dca84:
	.incbin "baserom.gba", 0x000d7b58, 0x00000028
	.global Func_080dcadc
	.thumb_func
	.type Func_080dcadc, %function
Func_080dcadc:
	.section .rom.000d7ba8, "ax"
	.incbin "baserom.gba", 0x000d7ba8, 0x00000298
	.section .rom.000d7e80, "ax"
	.incbin "baserom.gba", 0x000d7e80, 0x00000250
	.section .rom.000d8158, "ax"
	.incbin "baserom.gba", 0x000d8158, 0x00000024
	.section .rom.000d817c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.thumb_func
	.type BattleFx_StartRandomParticleEmitter, %function
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d817c, 0x00000364
	.section .rom.000d8554, "ax"
	.incbin "baserom.gba", 0x000d8554, 0x00000ba4
	.section .rom.000d90f8, "ax"
	.global Func_080d6b90
	.thumb_func
	.type Func_080d6b90, %function
Func_080d6b90:
	.incbin "baserom.gba", 0x000d90f8, 0x000000d4
	.section .rom.000d91cc, "ax"
	.global DisplayTransition_Start
	.thumb_func
	.type DisplayTransition_Start, %function
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d91cc, 0x00000354
	.section .rom.000d9520, "ax"
	.global DisplayTransition_Finish
	.thumb_func
	.type DisplayTransition_Finish, %function
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d9520, 0x00000228
	.section .rom.000d9748, "ax"
	.global Func_080d01cc
	.thumb_func
	.type Func_080d01cc, %function
Func_080d01cc:
	.incbin "baserom.gba", 0x000d9748, 0x000002e0
	.global DisplayTransition_UpdateFrame
	.thumb_func
	.type DisplayTransition_UpdateFrame, %function
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d9a28, 0x00000154
	.global DisplayTransition_FillTilemapAndSolidTile
	.thumb_func
	.type DisplayTransition_FillTilemapAndSolidTile, %function
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d9b7c, 0x00000070
	.section .rom.000d9ca0, "ax"
	.incbin "baserom.gba", 0x000d9ca0, 0x0000017c
	.global BattleFx_BuildColorBuffers
	.thumb_func
	.type BattleFx_BuildColorBuffers, %function
BattleFx_BuildColorBuffers:
	.incbin "baserom.gba", 0x000d9e1c, 0x00000868
	.section .rom.000da684, "ax"
	.global Func_080d0748
	.thumb_func
	.type Func_080d0748, %function
Func_080d0748:
	.incbin "baserom.gba", 0x000da684, 0x00000074
	.section .rom.000da6f8, "ax"
	.global Func_080d0c50
	.thumb_func
	.type Func_080d0c50, %function
Func_080d0c50:
	.incbin "baserom.gba", 0x000da6f8, 0x00000014
	.section .rom.000da70c, "ax"
	.global Func_080ccec8
	.thumb_func
	.type Func_080ccec8, %function
Func_080ccec8:
	.incbin "baserom.gba", 0x000da70c, 0x00000020
	.section .rom.000da72c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.thumb_func
	.type BattleFx_ApplyColorToSourceBuffer, %function
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000da72c, 0x00000020
	.section .rom.000da74c, "ax"
	.global BattleFx_SetBufferWord
	.thumb_func
	.type BattleFx_SetBufferWord, %function
BattleFx_SetBufferWord:
	.incbin "baserom.gba", 0x000da74c, 0x00000014
	.section .rom.000da760, "ax"
	.global Func_080ccd78
	.thumb_func
	.type Func_080ccd78, %function
Func_080ccd78:
	.incbin "baserom.gba", 0x000da760, 0x0000004c
	.section .rom.000da7ac, "ax"
	.global BattleFx_StartBufferInterpolation
	.thumb_func
	.type BattleFx_StartBufferInterpolation, %function
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000da7ac, 0x0000003c
	.section .rom.000da7e8, "ax"
	.global Func_080d1760
	.thumb_func
	.type Func_080d1760, %function
Func_080d1760:
	.incbin "baserom.gba", 0x000da7e8, 0x00000034
	.section .rom.000da83e, "ax"
	.incbin "baserom.gba", 0x000da83e, 0x00000516
	.section .rom.000dad54, "ax"
	.global Func_080e70f8
	.thumb_func
	.type Func_080e70f8, %function
Func_080e70f8:
	.incbin "baserom.gba", 0x000dad54, 0x00000058
	.section .rom.000dadac, "ax"
	.global Func_080d2cc4
	.thumb_func
	.type Func_080d2cc4, %function
Func_080d2cc4:
	.incbin "baserom.gba", 0x000dadac, 0x00000044
	.section .rom.000dadf0, "ax"
	.global Func_080d1d54
	.thumb_func
	.type Func_080d1d54, %function
Func_080d1d54:
	.incbin "baserom.gba", 0x000dadf0, 0x00000028
	.section .rom.000dae18, "ax"
	.global Func_080d1dac
	.thumb_func
	.type Func_080d1dac, %function
Func_080d1dac:
	.incbin "baserom.gba", 0x000dae18, 0x0000006c
	.section .rom.000daeaa, "ax"
	.incbin "baserom.gba", 0x000daeaa, 0x00000002
	.section .rom.000daeac, "ax"
	.global Func_080d1eac
	.thumb_func
	.type Func_080d1eac, %function
Func_080d1eac:
	.incbin "baserom.gba", 0x000daeac, 0x0000002c
	.section .rom.000daed8, "ax"
	.global BattleFx_GetFlags
	.thumb_func
	.type BattleFx_GetFlags, %function
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000daed8, 0x00000010
	.section .rom.000daee8, "ax"
	.global Func_080d4714
	.thumb_func
	.type Func_080d4714, %function
Func_080d4714:
	.incbin "baserom.gba", 0x000daee8, 0x0000030c
	.global EventRuntime_UpdateWaitMode
	.thumb_func
	.type EventRuntime_UpdateWaitMode, %function
EventRuntime_UpdateWaitMode:
	.incbin "baserom.gba", 0x000db1f4, 0x0000004c
	.section .rom.000db394, "ax"
	.incbin "baserom.gba", 0x000db394, 0x00000024
	.section .rom.000db3ca, "ax"
	.incbin "baserom.gba", 0x000db3ca, 0x0000000e
	.section .rom.000db5c8, "ax"
	.global Func_080d072c
	.thumb_func
	.type Func_080d072c, %function
Func_080d072c:
	.incbin "baserom.gba", 0x000db5c8, 0x00000218
	.section .rom.000db7fe, "ax"
	.incbin "baserom.gba", 0x000db7fe, 0x00000036
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
	.type Inventory_PromptAndSetObjectMode, %function
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000db834, 0x0000011c
	.section .rom.000db950, "ax"
	.global Menu_RunDefaultConfirmation
	.thumb_func
	.type Menu_RunDefaultConfirmation, %function
Menu_RunDefaultConfirmation:
	.incbin "baserom.gba", 0x000db950, 0x000000b0
	.section .rom.000dbb00, "ax"
	.global Func_080d2b0c
	.thumb_func
	.type Func_080d2b0c, %function
Func_080d2b0c:
	.incbin "baserom.gba", 0x000dbb00, 0x00000040
	.global Func_080d2b4c
	.thumb_func
	.type Func_080d2b4c, %function
Func_080d2b4c:
	.incbin "baserom.gba", 0x000dbb40, 0x000000b8
	.section .rom.000dbc58, "ax"
	.global Func_080d2c64
	.thumb_func
	.type Func_080d2c64, %function
Func_080d2c64:
	.incbin "baserom.gba", 0x000dbc58, 0x00000034
	.global Func_080d2c98
	.thumb_func
	.type Func_080d2c98, %function
Func_080d2c98:
	.incbin "baserom.gba", 0x000dbc8c, 0x0000002c
	.section .rom.000dbcb8, "ax"
	.global Func_080e37e0
	.thumb_func
	.type Func_080e37e0, %function
Func_080e37e0:
	.incbin "baserom.gba", 0x000dbcb8, 0x00000044
	.section .rom.000dbcfc, "ax"
	.global Func_080d1e18
	.thumb_func
	.type Func_080d1e18, %function
Func_080d1e18:
	.incbin "baserom.gba", 0x000dbcfc, 0x00000068
	.global Func_080d2d70
	.thumb_func
	.type Func_080d2d70, %function
Func_080d2d70:
	.incbin "baserom.gba", 0x000dbd64, 0x00000014
	.section .rom.000dbdd8, "ax"
	.global ObjectMotion_ResetTargetsAndVelocity
	.thumb_func
	.type ObjectMotion_ResetTargetsAndVelocity, %function
ObjectMotion_ResetTargetsAndVelocity:
	.incbin "baserom.gba", 0x000dbdd8, 0x00000038
	.section .rom.000dbf3c, "ax"
	.incbin "baserom.gba", 0x000dbf3c, 0x00000080
	.section .rom.000dc064, "ax"
	.global Func_080d3070
	.thumb_func
	.type Func_080d3070, %function
Func_080d3070:
	.incbin "baserom.gba", 0x000dc064, 0x0000008c
	.section .rom.000dc10a, "ax"
	.incbin "baserom.gba", 0x000dc10a, 0x00000002
	.global Func_080d3118
	.thumb_func
	.type Func_080d3118, %function
Func_080d3118:
	.incbin "baserom.gba", 0x000dc10c, 0x0000004c
	.global Func_080d3164
	.thumb_func
	.type Func_080d3164, %function
Func_080d3164:
	.incbin "baserom.gba", 0x000dc158, 0x0000005c
	.global Func_080d31c0
	.thumb_func
	.type Func_080d31c0, %function
Func_080d31c0:
	.incbin "baserom.gba", 0x000dc1b4, 0x00000054
	.global Func_080d3214
	.thumb_func
	.type Func_080d3214, %function
Func_080d3214:
	.incbin "baserom.gba", 0x000dc208, 0x0000002c
	.section .rom.000dc25c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
	.type ObjectMotion_WaitForAnimationChange, %function
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000dc25c, 0x00000050
	.section .rom.000dc330, "ax"
	.incbin "baserom.gba", 0x000dc330, 0x00000124
	.section .rom.000dc454, "ax"
	.global Func_080d3460
	.thumb_func
	.type Func_080d3460, %function
Func_080d3460:
	.incbin "baserom.gba", 0x000dc454, 0x0000013c
	.section .rom.000dc5f2, "ax"
	.incbin "baserom.gba", 0x000dc5f2, 0x00000002
	.section .rom.000dc5f4, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.thumb_func
	.type FacingObject_TurnPairToFaceEachOther, %function
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000dc5f4, 0x000000a8
	.section .rom.000dc69c, "ax"
	.global Func_080d36a8
	.thumb_func
	.type Func_080d36a8, %function
Func_080d36a8:
	.incbin "baserom.gba", 0x000dc69c, 0x00000020
	.section .rom.000dc774, "ax"
	.global Func_080d3780
	.thumb_func
	.type Func_080d3780, %function
Func_080d3780:
	.incbin "baserom.gba", 0x000dc774, 0x00000054
	.section .rom.000dc900, "ax"
	.global Func_080d390c
	.thumb_func
	.type Func_080d390c, %function
Func_080d390c:
	.incbin "baserom.gba", 0x000dc900, 0x0000001c
	.section .rom.000dc91c, "ax"
	.global Func_080d5d70
	.thumb_func
	.type Func_080d5d70, %function
Func_080d5d70:
	.incbin "baserom.gba", 0x000dc91c, 0x00000018
	.section .rom.000dc934, "ax"
	.global Func_080eb960
	.thumb_func
	.type Func_080eb960, %function
Func_080eb960:
	.incbin "baserom.gba", 0x000dc934, 0x000000d0
	.section .rom.000dca04, "ax"
	.global Func_080d3928
	.thumb_func
	.type Func_080d3928, %function
Func_080d3928:
	.incbin "baserom.gba", 0x000dca04, 0x00000118
	.section .rom.000dcb1c, "ax"
	.global Func_080dbe08
	.thumb_func
	.type Func_080dbe08, %function
Func_080dbe08:
	.incbin "baserom.gba", 0x000dcb1c, 0x000000c0
	.section .rom.000dcc20, "ax"
	.global Func_080d3c2c
	.thumb_func
	.type Func_080d3c2c, %function
Func_080d3c2c:
	.incbin "baserom.gba", 0x000dcc20, 0x0000005c
	.global UiText_OpenMessageAtObject
	.thumb_func
	.type UiText_OpenMessageAtObject, %function
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000dcc7c, 0x00000328
	.global Func_080d3fb0
	.thumb_func
	.type Func_080d3fb0, %function
Func_080d3fb0:
	.incbin "baserom.gba", 0x000dcfa4, 0x000000bc
	.global Func_080d406c
	.thumb_func
	.type Func_080d406c, %function
Func_080d406c:
	.incbin "baserom.gba", 0x000dd060, 0x00000010
	.global EventRuntime_ShowMessageAndWait
	.thumb_func
	.type EventRuntime_ShowMessageAndWait, %function
EventRuntime_ShowMessageAndWait:
	.incbin "baserom.gba", 0x000dd070, 0x00000008
	.global Func_080d4084
	.thumb_func
	.type Func_080d4084, %function
Func_080d4084:
	.incbin "baserom.gba", 0x000dd078, 0x00000054
	.global Func_080d40d8
	.thumb_func
	.type Func_080d40d8, %function
Func_080d40d8:
	.incbin "baserom.gba", 0x000dd0cc, 0x00000004
	.global Func_080d40dc
	.thumb_func
	.type Func_080d40dc, %function
Func_080d40dc:
	.incbin "baserom.gba", 0x000dd0d0, 0x0000009c
	.global Func_080d4178
	.thumb_func
	.type Func_080d4178, %function
Func_080d4178:
	.incbin "baserom.gba", 0x000dd16c, 0x00000008
	.section .rom.000dd1da, "ax"
	.incbin "baserom.gba", 0x000dd1da, 0x00000002
	.section .rom.000dd1f0, "ax"
	.global Func_080d41fc
	.thumb_func
	.type Func_080d41fc, %function
Func_080d41fc:
	.incbin "baserom.gba", 0x000dd1f0, 0x00000134
	.global Func_080d4330
	.thumb_func
	.type Func_080d4330, %function
Func_080d4330:
	.incbin "baserom.gba", 0x000dd324, 0x00000054
	.global Object_AttachWorkTargetToObject
	.thumb_func
	.type Object_AttachWorkTargetToObject, %function
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000dd378, 0x00000068
	.global Func_080d43ec
	.thumb_func
	.type Func_080d43ec, %function
Func_080d43ec:
	.incbin "baserom.gba", 0x000dd3e0, 0x00000020
	.global Motion_CamBounds
	.thumb_func
	.type Motion_CamBounds, %function
Motion_CamBounds:
	.incbin "baserom.gba", 0x000dd400, 0x00000100
	.global Func_080d450c
	.thumb_func
	.type Func_080d450c, %function
Func_080d450c:
	.incbin "baserom.gba", 0x000dd500, 0x00000020
	.global Func_080d452c
	.thumb_func
	.type Func_080d452c, %function
Func_080d452c:
	.incbin "baserom.gba", 0x000dd520, 0x0000001c
	.global Func_080d4548
	.thumb_func
	.type Func_080d4548, %function
Func_080d4548:
	.incbin "baserom.gba", 0x000dd53c, 0x00000020
	.global Func_080d4568
	.thumb_func
	.type Func_080d4568, %function
Func_080d4568:
	.incbin "baserom.gba", 0x000dd55c, 0x00000050
	.global Func_080d45b8
	.thumb_func
	.type Func_080d45b8, %function
Func_080d45b8:
	.incbin "baserom.gba", 0x000dd5ac, 0x000000ec
	.section .rom.000dd698, "ax"
	.global Func_080cad9c
	.thumb_func
	.type Func_080cad9c, %function
Func_080cad9c:
	.incbin "baserom.gba", 0x000dd698, 0x00000070
	.section .rom.000dd708, "ax"
	.global Func_080cae5c
	.thumb_func
	.type Func_080cae5c, %function
Func_080cae5c:
	.incbin "baserom.gba", 0x000dd708, 0x00000058
	.section .rom.000dd7a8, "ax"
	.incbin "baserom.gba", 0x000dd7a8, 0x000001d8
	.section .rom.000dda9c, "ax"
	.incbin "baserom.gba", 0x000dda9c, 0x0000000c
	.section .rom.000ddaa8, "ax"
	.global Func_080da908
	.thumb_func
	.type Func_080da908, %function
Func_080da908:
	.incbin "baserom.gba", 0x000ddaa8, 0x00000058
	.section .rom.000ddb00, "ax"
	.global Object_SetActionCallback
	.thumb_func
	.type Object_SetActionCallback, %function
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000ddb00, 0x000001fc
	.section .rom.000ddcfc, "ax"
	.global Func_080decb8
	.thumb_func
	.type Func_080decb8, %function
Func_080decb8:
	.incbin "baserom.gba", 0x000ddcfc, 0x000003f0
	.section .rom.000de0ec, "ax"
	.global Func_080cdbf8
	.thumb_func
	.type Func_080cdbf8, %function
Func_080cdbf8:
	.incbin "baserom.gba", 0x000de0ec, 0x000002c0
	.section .rom.000de3ac, "ax"
	.global Func_080d4d08
	.thumb_func
	.type Func_080d4d08, %function
Func_080d4d08:
	.incbin "baserom.gba", 0x000de3ac, 0x000009b8
	.section .rom.000ded64, "ax"
	.global Func_080daecc
	.thumb_func
	.type Func_080daecc, %function
Func_080daecc:
	.incbin "baserom.gba", 0x000ded64, 0x00000070
	.section .rom.000dedd4, "ax"
	.global ObjectEffect_PrepareContextEffect
	.thumb_func
	.type ObjectEffect_PrepareContextEffect, %function
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000dedd4, 0x00000070
	.section .rom.000dee56, "ax"
	.incbin "baserom.gba", 0x000dee56, 0x00000016
	.section .rom.000dee6c, "ax"
	.global ObjectEffect_EndContextEffect
	.thumb_func
	.type ObjectEffect_EndContextEffect, %function
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000dee6c, 0x000000a0
	.section .rom.000defc8, "ax"
	.incbin "baserom.gba", 0x000defc8, 0x00000938
	.section .rom.000df900, "ax"
	.global Func_080d53b8
	.thumb_func
	.type Func_080d53b8, %function
Func_080d53b8:
	.incbin "baserom.gba", 0x000df900, 0x00000284
	.section .rom.000dfb84, "ax"
	.global Func_080d7408
	.thumb_func
	.type Func_080d7408, %function
Func_080d7408:
	.incbin "baserom.gba", 0x000dfb84, 0x000006b0
	.section .rom.000e0234, "ax"
	.global Func_080d7430
	.thumb_func
	.type Func_080d7430, %function
Func_080d7430:
	.incbin "baserom.gba", 0x000e0234, 0x000000c4
	.section .rom.000e02f8, "ax"
	.global Func_080d2260
	.thumb_func
	.type Func_080d2260, %function
Func_080d2260:
	.incbin "baserom.gba", 0x000e02f8, 0x000000b0
	.section .rom.000e03a8, "ax"
	.global Func_080d690c
	.thumb_func
	.type Func_080d690c, %function
Func_080d690c:
	.incbin "baserom.gba", 0x000e03a8, 0x0000002c
	.section .rom.000e03d4, "ax"
	.global Func_080d7304
	.thumb_func
	.type Func_080d7304, %function
Func_080d7304:
	.incbin "baserom.gba", 0x000e03d4, 0x00000028
	.section .rom.000e03fc, "ax"
	.global Func_080d73b4
	.thumb_func
	.type Func_080d73b4, %function
Func_080d73b4:
	.incbin "baserom.gba", 0x000e03fc, 0x00000028
	.section .rom.000e0424, "ax"
	.global Func_080d73e0
	.thumb_func
	.type Func_080d73e0, %function
Func_080d73e0:
	.incbin "baserom.gba", 0x000e0424, 0x000000c0
	.section .rom.000e0518, "ax"
	.incbin "baserom.gba", 0x000e0518, 0x00000264
	.section .rom.000e077c, "ax"
	.global Func_080c9dc8
	.thumb_func
	.type Func_080c9dc8, %function
Func_080c9dc8:
	.incbin "baserom.gba", 0x000e077c, 0x000002d0
	.section .rom.000e0aa8, "ax"
	.global Func_080d7ab4
	.thumb_func
	.type Func_080d7ab4, %function
Func_080d7ab4:
	.incbin "baserom.gba", 0x000e0aa8, 0x00000c58
	.section .rom.000e1734, "ax"
	.incbin "baserom.gba", 0x000e1734, 0x0000023c
	.section .rom.000e1970, "ax"
	.global Func_080d2c9c
	.thumb_func
	.type Func_080d2c9c, %function
Func_080d2c9c:
	.incbin "baserom.gba", 0x000e1970, 0x000003ec
	.section .rom.000e1d5c, "ax"
	.global Func_080ca6e4
	.thumb_func
	.type Func_080ca6e4, %function
Func_080ca6e4:
	.incbin "baserom.gba", 0x000e1d5c, 0x0000039c
	.section .rom.000e20f8, "ax"
	.global Func_080d897c
	.thumb_func
	.type Func_080d897c, %function
Func_080d897c:
	.incbin "baserom.gba", 0x000e20f8, 0x000001d0
	.section .rom.000e22c8, "ax"
	.global Func_080d8d68
	.thumb_func
	.type Func_080d8d68, %function
Func_080d8d68:
	.incbin "baserom.gba", 0x000e22c8, 0x0000043c
	.section .rom.000e2704, "ax"
	.global Func_080d9104
	.thumb_func
	.type Func_080d9104, %function
Func_080d9104:
	.incbin "baserom.gba", 0x000e2704, 0x000003a0
	.section .rom.000e2aa4, "ax"
	.global Func_080d9b08
	.thumb_func
	.type Func_080d9b08, %function
Func_080d9b08:
	.incbin "baserom.gba", 0x000e2aa4, 0x00000020
	.section .rom.000e2ac4, "ax"
	.global Func_080d9e98
	.thumb_func
	.type Func_080d9e98, %function
Func_080d9e98:
	.incbin "baserom.gba", 0x000e2ac4, 0x00000038
	.section .rom.000e2afc, "ax"
	.global Func_080d92d4
	.thumb_func
	.type Func_080d92d4, %function
Func_080d92d4:
	.incbin "baserom.gba", 0x000e2afc, 0x00000390
	.section .rom.000e2e8c, "ax"
	.global Func_080d9710
	.thumb_func
	.type Func_080d9710, %function
Func_080d9710:
	.incbin "baserom.gba", 0x000e2e8c, 0x00000a70
	.section .rom.000e38fc, "ax"
	.global Func_080dbe44
	.thumb_func
	.type Func_080dbe44, %function
Func_080dbe44:
	.incbin "baserom.gba", 0x000e38fc, 0x00000030
	.section .rom.000e392c, "ax"
	.global Func_080d3b28
	.thumb_func
	.type Func_080d3b28, %function
Func_080d3b28:
	.incbin "baserom.gba", 0x000e392c, 0x00000594
	.section .rom.000e3ec0, "ax"
	.global Func_080cdf5c
	.thumb_func
	.type Func_080cdf5c, %function
Func_080cdf5c:
	.incbin "baserom.gba", 0x000e3ec0, 0x000001e4
	.section .rom.000e40a4, "ax"
	.global Func_080db444
	.thumb_func
	.type Func_080db444, %function
Func_080db444:
	.incbin "baserom.gba", 0x000e40a4, 0x00000394
	.section .rom.000e4438, "ax"
	.global Func_080dbd48
	.thumb_func
	.type Func_080dbd48, %function
Func_080dbd48:
	.incbin "baserom.gba", 0x000e4438, 0x00000048
	.section .rom.000e44aa, "ax"
	.incbin "baserom.gba", 0x000e44aa, 0x00000002
	.section .rom.000e44ac, "ax"
	.global BattleFx_Run
	.thumb_func
	.type BattleFx_Run, %function
BattleFx_Run:
	.incbin "baserom.gba", 0x000e44ac, 0x000001b8
	.section .rom.000e4664, "ax"
	.global BattleFx_DispatchRequestKind
	.thumb_func
	.type BattleFx_DispatchRequestKind, %function
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e4664, 0x000001d8
	.section .rom.000e483c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.thumb_func
	.type BattleFx_ClearChildValueOnMismatch, %function
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000e483c, 0x0000003c
	.section .rom.000e4878, "ax"
	.global FieldEvent_RunTypeHandler
	.thumb_func
	.type FieldEvent_RunTypeHandler, %function
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000e4878, 0x00000098
	.section .rom.000e4910, "ax"
	.global Func_080d1df0
	.thumb_func
	.type Func_080d1df0, %function
Func_080d1df0:
	.incbin "baserom.gba", 0x000e4910, 0x000000a4
	.section .rom.000e49b4, "ax"
	.global Func_080eb2c8
	.thumb_func
	.type Func_080eb2c8, %function
Func_080eb2c8:
	.incbin "baserom.gba", 0x000e49b4, 0x0000000c
	.section .rom.000e49c0, "ax"
	.global Func_080eb2d0
	.thumb_func
	.type Func_080eb2d0, %function
Func_080eb2d0:
	.incbin "baserom.gba", 0x000e49c0, 0x000001ac
	.global Map_GetCellAtPosition
	.thumb_func
	.type Map_GetCellAtPosition, %function
Map_GetCellAtPosition:
	.incbin "baserom.gba", 0x000e4b6c, 0x00000160
	.section .rom.000e4ccc, "ax"
	.global Map_TestSpecialTerrainPair
	.thumb_func
	.type Map_TestSpecialTerrainPair, %function
Map_TestSpecialTerrainPair:
	.incbin "baserom.gba", 0x000e4ccc, 0x00000070
	.section .rom.000e4d3c, "ax"
	.global Func_080e25e8
	.thumb_func
	.type Func_080e25e8, %function
Func_080e25e8:
	.incbin "baserom.gba", 0x000e4d3c, 0x00000080
	.section .rom.000e4dbc, "ax"
	.global Func_080e1420
	.thumb_func
	.type Func_080e1420, %function
Func_080e1420:
	.incbin "baserom.gba", 0x000e4dbc, 0x0000000c
	.section .rom.000e4dc8, "ax"
	.global Func_080dbdc8
	.thumb_func
	.type Func_080dbdc8, %function
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e4dc8, 0x00000014
	.section .rom.000e4ddc, "ax"
	.global Func_080e4244
	.thumb_func
	.type Func_080e4244, %function
Func_080e4244:
	.incbin "baserom.gba", 0x000e4ddc, 0x0000000c
	.section .rom.000e4de8, "ax"
	.global Func_080dbde8
	.thumb_func
	.type Func_080dbde8, %function
Func_080dbde8:
	.incbin "baserom.gba", 0x000e4de8, 0x00000014
	.section .rom.000e4dfc, "ax"
	.global Func_080dbdd4
	.thumb_func
	.type Func_080dbdd4, %function
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e4dfc, 0x0000003c
	.section .rom.000e4e38, "ax"
	.global Func_080dbdf4
	.thumb_func
	.type Func_080dbdf4, %function
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e4e38, 0x00000274
	.section .rom.000e50ac, "ax"
	.global Func_080cb6c8
	.thumb_func
	.type Func_080cb6c8, %function
Func_080cb6c8:
	.incbin "baserom.gba", 0x000e50ac, 0x00000020
	.global Func_080dc0d8
	.thumb_func
	.type Func_080dc0d8, %function
Func_080dc0d8:
	.incbin "baserom.gba", 0x000e50cc, 0x00000034
	.section .rom.000e5100, "ax"
	.global Object_Spawn
	.thumb_func
	.type Object_Spawn, %function
Object_Spawn:
	.incbin "baserom.gba", 0x000e5100, 0x00000058
	.section .rom.000e51a4, "ax"
	.global Func_080db9c0
	.thumb_func
	.type Func_080db9c0, %function
Func_080db9c0:
	.incbin "baserom.gba", 0x000e51a4, 0x00000094
	.section .rom.000e5238, "ax"
	.global Field_BeginPaletteTransition
	.thumb_func
	.type Field_BeginPaletteTransition, %function
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000e5238, 0x000003e8
	.global Func_080dc62c
	.thumb_func
	.type Func_080dc62c, %function
Func_080dc62c:
	.incbin "baserom.gba", 0x000e5620, 0x0000034c
	.section .rom.000e596c, "ax"
	.global Func_080cdec8
	.thumb_func
	.type Func_080cdec8, %function
Func_080cdec8:
	.incbin "baserom.gba", 0x000e596c, 0x000000d8
	.section .rom.000e5a44, "ax"
	.global Func_080cded4
	.thumb_func
	.type Func_080cded4, %function
Func_080cded4:
	.incbin "baserom.gba", 0x000e5a44, 0x00000034
	.section .rom.000e5a78, "ax"
	.global Func_080dc978
	.thumb_func
	.type Func_080dc978, %function
Func_080dc978:
	.incbin "baserom.gba", 0x000e5a78, 0x00000058
	.section .rom.000e5ad0, "ax"
	.global Func_080dca50
	.thumb_func
	.type Func_080dca50, %function
Func_080dca50:
	.incbin "baserom.gba", 0x000e5ad0, 0x00000478
	.section .rom.000e6048, "ax"
	.global Func_080ca6a4
	.thumb_func
	.type Func_080ca6a4, %function
Func_080ca6a4:
	.incbin "baserom.gba", 0x000e6048, 0x000005e8
	.section .rom.000e6630, "ax"
	.global BattleFx_SnapScaleToFull
	.thumb_func
	.type BattleFx_SnapScaleToFull, %function
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000e6630, 0x0000002c
	.section .rom.000e665c, "ax"
	.global BattleFx_StartItemBreak
	.thumb_func
	.type BattleFx_StartItemBreak, %function
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000e665c, 0x00000ee8
	.section .rom.000e7598, "ax"
	.incbin "baserom.gba", 0x000e7598, 0x00000714
	.section .rom.000e7cac, "ax"
	.global Func_080cd91c
	.thumb_func
	.type Func_080cd91c, %function
Func_080cd91c:
	.incbin "baserom.gba", 0x000e7cac, 0x000000f0
	.section .rom.000e7dd4, "ax"
	.incbin "baserom.gba", 0x000e7dd4, 0x00000ac8
	.section .rom.000e88d2, "ax"
	.incbin "baserom.gba", 0x000e88d2, 0x000001d6
	.section .rom.000e8afe, "ax"
	.incbin "baserom.gba", 0x000e8afe, 0x000001ca
	.section .rom.000e8cec, "ax"
	.incbin "baserom.gba", 0x000e8cec, 0x000005f4
	.section .rom.000e9350, "ax"
	.global Func_080e035c
	.thumb_func
	.type Func_080e035c, %function
Func_080e035c:
	.incbin "baserom.gba", 0x000e9350, 0x00000050
	.section .rom.000e93b6, "ax"
	.incbin "baserom.gba", 0x000e93b6, 0x00000fba
	.section .rom.000ea370, "ax"
	.global Func_080e15fc
	.thumb_func
	.type Func_080e15fc, %function
Func_080e15fc:
	.incbin "baserom.gba", 0x000ea370, 0x000000a4
	.section .rom.000ea414, "ax"
	.global Func_080e1650
	.thumb_func
	.type Func_080e1650, %function
Func_080e1650:
	.incbin "baserom.gba", 0x000ea414, 0x000001dc
	.section .rom.000ea5f0, "ax"
	.global Func_080dc1b0
	.thumb_func
	.type Func_080dc1b0, %function
Func_080dc1b0:
	.incbin "baserom.gba", 0x000ea5f0, 0x00000054
	.section .rom.000ea644, "ax"
	.global Field_EndActorSpriteEffect
	.thumb_func
	.type Field_EndActorSpriteEffect, %function
Field_EndActorSpriteEffect:
	.incbin "baserom.gba", 0x000ea644, 0x00000f98
	.section .rom.000eb5dc, "ax"
	.global Func_080eaeb4
	.thumb_func
	.type Func_080eaeb4, %function
Func_080eaeb4:
	.incbin "baserom.gba", 0x000eb5dc, 0x0000018c
	.section .rom.000eb768, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.thumb_func
	.type ObjectGroup_ApplyRandomChildValues, %function
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000eb768, 0x00000f24
	.section .rom.000ec68c, "ax"
	.global Func_080da938
	.thumb_func
	.type Func_080da938, %function
Func_080da938:
	.incbin "baserom.gba", 0x000ec68c, 0x00000148
	.section .rom.000ec7d4, "ax"
	.global Func_080d4ab4
	.thumb_func
	.type Func_080d4ab4, %function
Func_080d4ab4:
	.incbin "baserom.gba", 0x000ec7d4, 0x00000a64
	.section .rom.000ed238, "ax"
	.global Func_080e137c
	.thumb_func
	.type Func_080e137c, %function
Func_080e137c:
	.incbin "baserom.gba", 0x000ed238, 0x00002eb4
	.section .rom.000f00ec, "ax"
	.global Func_080e3698
	.thumb_func
	.type Func_080e3698, %function
Func_080e3698:
	.incbin "baserom.gba", 0x000f00ec, 0x000036c8
	.section .rom.000f37b4, "ax"
	.global Func_080d9ad0
	.thumb_func
	.type Func_080d9ad0, %function
Func_080d9ad0:
	.incbin "baserom.gba", 0x000f37b4, 0x00000114
	.section .rom.000f38c8, "ax"
	.global Func_080ed804
	.thumb_func
	.type Func_080ed804, %function
Func_080ed804:
	.incbin "baserom.gba", 0x000f38c8, 0x00000140
	.section .rom.000f3a08, "ax"
	.global Func_080ea7c0
	.thumb_func
	.type Func_080ea7c0, %function
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f3a08, 0x0000015c
	.section .rom.000f3b64, "ax"
	.global Func_080ea8d4
	.thumb_func
	.type Func_080ea8d4, %function
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f3b64, 0x00000028
	.section .rom.000f3b8c, "ax"
	.global Func_080eace8
	.thumb_func
	.type Func_080eace8, %function
Func_080eace8:
	.incbin "baserom.gba", 0x000f3b8c, 0x00000038
	.section .rom.000f3bc4, "ax"
	.global Func_080eaa14
	.thumb_func
	.type Func_080eaa14, %function
Func_080eaa14:
	.incbin "baserom.gba", 0x000f3bc4, 0x00000118
	.section .rom.000f3cdc, "ax"
	.global Func_080eab70
	.thumb_func
	.type Func_080eab70, %function
Func_080eab70:
	.incbin "baserom.gba", 0x000f3cdc, 0x00000114
	.section .rom.000f3df0, "ax"
	.global Func_080eabd0
	.thumb_func
	.type Func_080eabd0, %function
Func_080eabd0:
	.incbin "baserom.gba", 0x000f3df0, 0x000000b8
	.section .rom.000f3ea8, "ax"
	.global Func_080cdc74
	.thumb_func
	.type Func_080cdc74, %function
Func_080cdc74:
	.incbin "baserom.gba", 0x000f3ea8, 0x00000074
	.section .rom.000f3f1c, "ax"
	.global Func_080eadfc
	.thumb_func
	.type Func_080eadfc, %function
Func_080eadfc:
	.incbin "baserom.gba", 0x000f3f1c, 0x00000070
	.section .rom.000f3f8c, "ax"
	.global Func_080d3940
	.thumb_func
	.type Func_080d3940, %function
Func_080d3940:
	.incbin "baserom.gba", 0x000f3f8c, 0x00000084
	.section .rom.000f4010, "ax"
	.global Func_080d3a10
	.thumb_func
	.type Func_080d3a10, %function
Func_080d3a10:
	.incbin "baserom.gba", 0x000f4010, 0x0000027c
	.section .rom.000f428c, "ax"
	.global Func_080eaf98
	.thumb_func
	.type Func_080eaf98, %function
Func_080eaf98:
	.incbin "baserom.gba", 0x000f428c, 0x00000030
	.section .rom.000f42bc, "ax"
	.global Func_080eb01c
	.thumb_func
	.type Func_080eb01c, %function
Func_080eb01c:
	.incbin "baserom.gba", 0x000f42bc, 0x00000008
	.section .rom.000f42c4, "ax"
	.global Func_080eb298
	.thumb_func
	.type Func_080eb298, %function
Func_080eb298:
	.incbin "baserom.gba", 0x000f42c4, 0x00000690
	.section .rom.000f4954, "ax"
	.global Func_080db0b0
	.thumb_func
	.type Func_080db0b0, %function
Func_080db0b0:
	.incbin "baserom.gba", 0x000f4954, 0x000002d0
	.section .rom.000f4c24, "ax"
	.global EffectSlot_Update
	.global Func_080ca1bc
	.thumb_func
	.type EffectSlot_Update, %function
	.type Func_080ca1bc, %function
EffectSlot_Update:
Func_080ca1bc:
	.incbin "baserom.gba", 0x000f4c24, 0x00000240
	.section .rom.000f4e9a, "ax"
	.incbin "baserom.gba", 0x000f4e9a, 0x000002a6
	.section .rom.000f5160, "ax"
	.incbin "baserom.gba", 0x000f5160, 0x00001698
	.section .rom.000f67f8, "ax"
	.global Func_080d9ab0
	.thumb_func
	.type Func_080d9ab0, %function
Func_080d9ab0:
	.incbin "baserom.gba", 0x000f67f8, 0x000002c8
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x000f6ac0, 0x00001488
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000f7f48, 0x00000140
	.global Audio_AreaCueTable
Audio_AreaCueTable:
	.incbin "baserom.gba", 0x000f8088, 0x00001100
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000f9188, 0x000000b2
	.global Graphics_ColorCurveA
Graphics_ColorCurveA:
	.incbin "baserom.gba", 0x000f923a, 0x00000040
	.global Graphics_ColorCurveB
Graphics_ColorCurveB:
	.incbin "baserom.gba", 0x000f927a, 0x00000040
	.global Graphics_ColorCurveC
Graphics_ColorCurveC:
	.incbin "baserom.gba", 0x000f92ba, 0x00000412
	.global Object_VariantMotionScripts
Object_VariantMotionScripts:
	.incbin "baserom.gba", 0x000f96cc, 0x000001b4
	.global Data_080f088c
Data_080f088c:
	.incbin "baserom.gba", 0x000f9880, 0x000005c8
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f9e48, 0x00000050
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000f9e98, 0x00000020
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f9eb8, 0x00000068
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f9f20, 0x0000087c
	.global Field_SceneTable
Field_SceneTable:
	.incbin "baserom.gba", 0x000fa79c, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000fc2c4, 0x00000030
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000fc2f4, 0x00000480
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000fc774, 0x0000188c
	.section .rom.000fe028, "ax"
	.global UiIcon_PrepareObjectFar
	.thumb_func
	.type UiIcon_PrepareObjectFar, %function
UiIcon_PrepareObjectFar:
	.incbin "baserom.gba", 0x000fe028, 0x00000008
	.global ItemMenu_Open
	.thumb_func
	.type ItemMenu_Open, %function
ItemMenu_Open:
	.incbin "baserom.gba", 0x000fe030, 0x00000008
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
	.type ItemMenu_DrawItemDetailsFar, %function
ItemMenu_DrawItemDetailsFar:
	.incbin "baserom.gba", 0x000fe038, 0x00000008
	.global Func_080f8040
	.thumb_func
	.type Func_080f8040, %function
Func_080f8040:
	.incbin "baserom.gba", 0x000fe040, 0x00000008
	.global Palette_CopyObjectBankToBackground14Far
	.thumb_func
	.type Palette_CopyObjectBankToBackground14Far, %function
Palette_CopyObjectBankToBackground14Far:
	.incbin "baserom.gba", 0x000fe048, 0x00000008
	.global Menu_UpdateEntryObjectTransformsFar
	.thumb_func
	.type Menu_UpdateEntryObjectTransformsFar, %function
Menu_UpdateEntryObjectTransformsFar:
	.incbin "baserom.gba", 0x000fe050, 0x00000020
	.section .rom.000fe0e0, "ax"
	.incbin "baserom.gba", 0x000fe0e0, 0x0000003c
	.section .rom.000fe170, "ax"
	.incbin "baserom.gba", 0x000fe170, 0x00000494
	.section .rom.000fe64a, "ax"
	.incbin "baserom.gba", 0x000fe64a, 0x000000b2
	.section .rom.000fe87c, "ax"
	.global UiIcon_PrepareObject
	.thumb_func
	.type UiIcon_PrepareObject, %function
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fe87c, 0x00000390
	.section .rom.000fecda, "ax"
	.incbin "baserom.gba", 0x000fecda, 0x0000025a
	.section .rom.000fef90, "ax"
	.incbin "baserom.gba", 0x000fef90, 0x000001d4
	.section .rom.000ff216, "ax"
	.incbin "baserom.gba", 0x000ff216, 0x00000002
	.section .rom.000ff218, "ax"
	.global Render_SetTilemapFlagRect
	.thumb_func
	.type Render_SetTilemapFlagRect, %function
Render_SetTilemapFlagRect:
	.incbin "baserom.gba", 0x000ff218, 0x00000088
	.section .rom.000ff2a0, "ax"
	.global Palette_CopyObjectBankToBackground14
	.thumb_func
	.type Palette_CopyObjectBankToBackground14, %function
Palette_CopyObjectBankToBackground14:
	.incbin "baserom.gba", 0x000ff2a0, 0x000000c8
	.section .rom.000ff3aa, "ax"
	.incbin "baserom.gba", 0x000ff3aa, 0x00000002
	.section .rom.000ff496, "ax"
	.incbin "baserom.gba", 0x000ff496, 0x00000002
	.section .rom.000ff498, "ax"
	.global Func_080f94a4
	.thumb_func
	.type Func_080f94a4, %function
Func_080f94a4:
	.incbin "baserom.gba", 0x000ff498, 0x00000f3c
	.section .rom.00100456, "ax"
	.incbin "baserom.gba", 0x00100456, 0x00000002
	.section .rom.00100458, "ax"
	.global ItemMenu_HideAllIcons
	.thumb_func
	.type ItemMenu_HideAllIcons, %function
ItemMenu_HideAllIcons:
	.incbin "baserom.gba", 0x00100458, 0x00000884
	.section .rom.00100cee, "ax"
	.incbin "baserom.gba", 0x00100cee, 0x0000011e
	.global ItemMenu_DrawIcons
	.thumb_func
	.type ItemMenu_DrawIcons, %function
ItemMenu_DrawIcons:
	.incbin "baserom.gba", 0x00100e0c, 0x000002d4
	.section .rom.00101140, "ax"
	.incbin "baserom.gba", 0x00101140, 0x00000c40
	.section .rom.00101de8, "ax"
	.incbin "baserom.gba", 0x00101de8, 0x00000d54
	.section .rom.00102b3c, "ax"
	.global Func_080fcab8
	.thumb_func
	.type Func_080fcab8, %function
Func_080fcab8:
	.incbin "baserom.gba", 0x00102b3c, 0x0000175c
	.section .rom.001042b8, "ax"
	.global Func_080fe184
	.thumb_func
	.type Func_080fe184, %function
Func_080fe184:
	.incbin "baserom.gba", 0x001042b8, 0x000000f0
	.section .rom.001043a8, "ax"
	.global Func_080fe274
	.thumb_func
	.type Func_080fe274, %function
Func_080fe274:
	.incbin "baserom.gba", 0x001043a8, 0x00000f80
	.section .rom.0010541c, "ax"
	.incbin "baserom.gba", 0x0010541c, 0x000005f8
	.section .rom.00105a80, "ax"
	.incbin "baserom.gba", 0x00105a80, 0x00000b8c
	.section .rom.00106694, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.thumb_func
	.type ItemMenu_DrawEquippedItemNames, %function
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x00106694, 0x000000e4
	.section .rom.001067b4, "ax"
	.global ItemMenu_ArrangeCategoryItemIcons
	.thumb_func
	.type ItemMenu_ArrangeCategoryItemIcons, %function
ItemMenu_ArrangeCategoryItemIcons:
	.incbin "baserom.gba", 0x001067b4, 0x000000a0
	.section .rom.00106854, "ax"
	.global ItemMenu_PosCategory
	.thumb_func
	.type ItemMenu_PosCategory, %function
ItemMenu_PosCategory:
	.incbin "baserom.gba", 0x00106854, 0x0000077c
	.section .rom.00106fd0, "ax"
	.global Func_08100e7c
	.thumb_func
	.type Func_08100e7c, %function
Func_08100e7c:
	.incbin "baserom.gba", 0x00106fd0, 0x00003c48
	.section .rom.0010aca2, "ax"
	.incbin "baserom.gba", 0x0010aca2, 0x00000002
	.section .rom.0010aca4, "ax"
	.global UiIcon_CreateStatChangeArrow
	.thumb_func
	.type UiIcon_CreateStatChangeArrow, %function
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x0010aca4, 0x00000560
	.section .rom.0010b204, "ax"
	.global Menu_ReleaseEntryObjects
	.thumb_func
	.type Menu_ReleaseEntryObjects, %function
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x0010b204, 0x000000f0
	.section .rom.0010b2f4, "ax"
	.global Func_08104ef8
	.thumb_func
	.type Func_08104ef8, %function
Func_08104ef8:
	.incbin "baserom.gba", 0x0010b2f4, 0x000000c4
	.section .rom.0010b3b8, "ax"
	.global Func_08104fe0
	.thumb_func
	.type Func_08104fe0, %function
Func_08104fe0:
	.incbin "baserom.gba", 0x0010b3b8, 0x00000040
	.section .rom.0010b3f8, "ax"
	.global Func_081051a8
	.thumb_func
	.type Func_081051a8, %function
Func_081051a8:
	.incbin "baserom.gba", 0x0010b3f8, 0x00000054
	.section .rom.0010b44c, "ax"
	.global Func_0810526c
	.thumb_func
	.type Func_0810526c, %function
Func_0810526c:
	.incbin "baserom.gba", 0x0010b44c, 0x0000002c
	.section .rom.0010b478, "ax"
	.global Func_081050b8
	.thumb_func
	.type Func_081050b8, %function
Func_081050b8:
	.incbin "baserom.gba", 0x0010b478, 0x00000024
	.section .rom.0010b49c, "ax"
	.global Func_081052ac
	.thumb_func
	.type Func_081052ac, %function
Func_081052ac:
	.incbin "baserom.gba", 0x0010b49c, 0x00000230
	.section .rom.0010b742, "ax"
	.incbin "baserom.gba", 0x0010b742, 0x00000352
	.section .rom.0010ba94, "ax"
	.global Data_08105948
Data_08105948:
	.incbin "baserom.gba", 0x0010ba94, 0x00000024
	.section .rom.0010bab8, "ax"
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x0010bab8, 0x00000004
	.section .rom.0010babc, "ax"
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x0010babc, 0x0000007e
	.section .rom.0010bb3a, "ax"
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x0010bb3a, 0x0000000d
	.section .rom.0010bb47, "ax"
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x0010bb47, 0x0000000d
	.section .rom.0010bb54, "ax"
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x0010bb54, 0x00000018
	.section .rom.0010bb6c, "ax"
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x0010bb6c, 0x00000030
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x0010bb9c, 0x00000464
	.section .rom.0010c000, "ax"
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x0010c000, 0x00000008
	.global Func_08108008
	.thumb_func
	.type Func_08108008, %function
Func_08108008:
	.incbin "baserom.gba", 0x0010c008, 0x00000008
	.global Func_08108010
	.thumb_func
	.type Func_08108010, %function
Func_08108010:
	.incbin "baserom.gba", 0x0010c010, 0x00000008
	.global Func_08108018
	.thumb_func
	.type Func_08108018, %function
Func_08108018:
	.incbin "baserom.gba", 0x0010c018, 0x00000008
	.global Func_08108020
	.thumb_func
	.type Func_08108020, %function
Func_08108020:
	.incbin "baserom.gba", 0x0010c020, 0x00000008
	.global Func_08108028
	.thumb_func
	.type Func_08108028, %function
Func_08108028:
	.incbin "baserom.gba", 0x0010c028, 0x00000008
	.section .rom.0010c0a8, "ax"
	.incbin "baserom.gba", 0x0010c0a8, 0x00000438
	.section .rom.0010c4f4, "ax"
	.incbin "baserom.gba", 0x0010c4f4, 0x00000268
	.section .rom.0010c8d8, "ax"
	.global Func_081088d8
	.thumb_func
	.type Func_081088d8, %function
Func_081088d8:
	.incbin "baserom.gba", 0x0010c8d8, 0x00000050
	.section .rom.0010c928, "ax"
	.global Func_08108928
	.thumb_func
	.type Func_08108928, %function
Func_08108928:
	.incbin "baserom.gba", 0x0010c928, 0x00000020
	.section .rom.0010c948, "ax"
	.global Func_08108948
	.thumb_func
	.type Func_08108948, %function
Func_08108948:
	.incbin "baserom.gba", 0x0010c948, 0x0000009c
	.section .rom.0010c9e4, "ax"
	.global Func_081089e4
	.thumb_func
	.type Func_081089e4, %function
Func_081089e4:
	.incbin "baserom.gba", 0x0010c9e4, 0x000000a4
	.section .rom.0010ca88, "ax"
	.global Func_08108a88
	.thumb_func
	.type Func_08108a88, %function
Func_08108a88:
	.incbin "baserom.gba", 0x0010ca88, 0x00000020
	.section .rom.0010caa8, "ax"
	.global Func_08108aa8
	.thumb_func
	.type Func_08108aa8, %function
Func_08108aa8:
	.incbin "baserom.gba", 0x0010caa8, 0x0000008c
	.section .rom.0010cb70, "ax"
	.incbin "baserom.gba", 0x0010cb70, 0x00000f28
	.section .rom.0010dad8, "ax"
	.incbin "baserom.gba", 0x0010dad8, 0x00000d30
	.section .rom.0010e808, "ax"
	.global Func_0810a804
	.thumb_func
	.type Func_0810a804, %function
Func_0810a804:
	.incbin "baserom.gba", 0x0010e808, 0x00000030
	.section .rom.0010e838, "ax"
	.global Func_0810a834
	.thumb_func
	.type Func_0810a834, %function
Func_0810a834:
	.incbin "baserom.gba", 0x0010e838, 0x00000018
	.section .rom.0010e850, "ax"
	.global Func_0810a84c
	.thumb_func
	.type Func_0810a84c, %function
Func_0810a84c:
	.incbin "baserom.gba", 0x0010e850, 0x00000018
	.section .rom.0010e8f0, "ax"
	.incbin "baserom.gba", 0x0010e8f0, 0x000005b8
	.section .rom.0010ef28, "ax"
	.incbin "baserom.gba", 0x0010ef28, 0x000005fc
	.section .rom.0010f5c8, "ax"
	.incbin "baserom.gba", 0x0010f5c8, 0x000001d8
	.section .rom.0010f7a0, "ax"
	.global Func_0810b79c
	.thumb_func
	.type Func_0810b79c, %function
Func_0810b79c:
	.incbin "baserom.gba", 0x0010f7a0, 0x0000000c
	.section .rom.0010f7ac, "ax"
	.global Func_0810b7a8
	.thumb_func
	.type Func_0810b7a8, %function
Func_0810b7a8:
	.incbin "baserom.gba", 0x0010f7ac, 0x00000870
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0011001c, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0011015c, 0x00000200
	.global Shop_SelectorOffsets
Shop_SelectorOffsets:
	.incbin "baserom.gba", 0x0011035c, 0x0000003c
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x00110398, 0x00007c68
	.section .rom.00118000, "ax"
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000010
	.section .rom.00118058, "ax"
	.global Func_08118058
	.thumb_func
	.type Func_08118058, %function
Func_08118058:
	.incbin "baserom.gba", 0x00118058, 0x00000008
	.global Func_08118060
	.thumb_func
	.type Func_08118060, %function
Func_08118060:
	.incbin "baserom.gba", 0x00118060, 0x00000008
	.global Func_08118068
	.thumb_func
	.type Func_08118068, %function
Func_08118068:
	.incbin "baserom.gba", 0x00118068, 0x00000008
	.section .rom.00118230, "ax"
	.incbin "baserom.gba", 0x00118230, 0x00000008
	.section .rom.00118244, "ax"
	.incbin "baserom.gba", 0x00118244, 0x00000040
	.section .rom.00118394, "ax"
	.incbin "baserom.gba", 0x00118394, 0x0000007c
	.section .rom.00118410, "ax"
	.global ColorBuffer_Scale
	.thumb_func
	.type ColorBuffer_Scale, %function
ColorBuffer_Scale:
	.incbin "baserom.gba", 0x00118410, 0x00000328
	.section .rom.00118738, "ax"
	.global DebugParty_LoadPreset
	.thumb_func
	.type DebugParty_LoadPreset, %function
DebugParty_LoadPreset:
	.incbin "baserom.gba", 0x00118738, 0x00000638
	.section .rom.00118e64, "ax"
	.incbin "baserom.gba", 0x00118e64, 0x00000108
	.section .rom.00118f6c, "ax"
	.global Func_08118f6c
	.thumb_func
	.type Func_08118f6c, %function
Func_08118f6c:
	.incbin "baserom.gba", 0x00118f6c, 0x000000e8
	.section .rom.0011915e, "ax"
	.incbin "baserom.gba", 0x0011915e, 0x0000015a
	.section .rom.001192e6, "ax"
	.incbin "baserom.gba", 0x001192e6, 0x00000002
	.global BattlePres_WaitSync
	.thumb_func
	.type BattlePres_WaitSync, %function
BattlePres_WaitSync:
	.incbin "baserom.gba", 0x001192e8, 0x00000464
	.section .rom.0011974c, "ax"
	.global Func_08119734
	.thumb_func
	.type Func_08119734, %function
Func_08119734:
	.incbin "baserom.gba", 0x0011974c, 0x000000bc
	.section .rom.00119808, "ax"
	.global Func_081197f0
	.thumb_func
	.type Func_081197f0, %function
Func_081197f0:
	.incbin "baserom.gba", 0x00119808, 0x00000848
	.section .rom.0011a050, "ax"
	.global BattleParty_PrepareActiveOwners
	.thumb_func
	.type BattleParty_PrepareActiveOwners, %function
BattleParty_PrepareActiveOwners:
	.incbin "baserom.gba", 0x0011a050, 0x00000078
	.section .rom.0011a0c8, "ax"
	.global BattleParty_PrepareReserveOwners
	.thumb_func
	.type BattleParty_PrepareReserveOwners, %function
BattleParty_PrepareReserveOwners:
	.incbin "baserom.gba", 0x0011a0c8, 0x000000d8
	.section .rom.0011a264, "ax"
	.incbin "baserom.gba", 0x0011a264, 0x000000d0
	.section .rom.0011a3b4, "ax"
	.global Func_0811a39c
	.thumb_func
	.type Func_0811a39c, %function
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a3b4, 0x000000b0
	.section .rom.0011a49a, "ax"
	.incbin "baserom.gba", 0x0011a49a, 0x00000002
	.section .rom.0011a49c, "ax"
	.global BattleMotion_GetSlotField14
	.thumb_func
	.type BattleMotion_GetSlotField14, %function
BattleMotion_GetSlotField14:
	.incbin "baserom.gba", 0x0011a49c, 0x0000000c
	.section .rom.0011a4f6, "ax"
	.incbin "baserom.gba", 0x0011a4f6, 0x0000011e
	.global Func_0811a5fc
	.thumb_func
	.type Func_0811a5fc, %function
Func_0811a5fc:
	.incbin "baserom.gba", 0x0011a614, 0x00000044
	.section .rom.0011a68c, "ax"
	.incbin "baserom.gba", 0x0011a68c, 0x00000af8
	.section .rom.0011b198, "ax"
	.incbin "baserom.gba", 0x0011b198, 0x00000114
	.section .rom.0011b2dc, "ax"
	.incbin "baserom.gba", 0x0011b2dc, 0x000001e8
	.section .rom.0011b4f0, "ax"
	.balign 4
	.global BattleUnit_BuildStatusFlags
	.thumb_func
	.type BattleUnit_BuildStatusFlags, %function
BattleUnit_BuildStatusFlags:
	.incbin "baserom.gba", 0x0011b4f0, 0x000001cc
	.section .rom.0011b772, "ax"
	.incbin "baserom.gba", 0x0011b772, 0x00000002
	.section .rom.0011b774, "ax"
	.global Func_0811b75c
	.thumb_func
	.type Func_0811b75c, %function
Func_0811b75c:
	.incbin "baserom.gba", 0x0011b774, 0x00000278
	.section .rom.0011ba14, "ax"
	.incbin "baserom.gba", 0x0011ba14, 0x00000230
	.section .rom.0011bcb0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bcb0, 0x000000b8
	.section .rom.0011bd68, "ax"
	.global Func_0811bd50
	.thumb_func
	.type Func_0811bd50, %function
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd68, 0x00000060
	.section .rom.0011bdf4, "ax"
	.global Camera_InitDefaultTransform
	.thumb_func
	.type Camera_InitDefaultTransform, %function
Camera_InitDefaultTransform:
	.incbin "baserom.gba", 0x0011bdf4, 0x00000060
	.section .rom.0011be54, "ax"
	.global GetBattleObjectSlot
	.thumb_func
	.type GetBattleObjectSlot, %function
GetBattleObjectSlot:
	.incbin "baserom.gba", 0x0011be54, 0x0000002c
	.section .rom.0011bede, "ax"
	.incbin "baserom.gba", 0x0011bede, 0x00000002
	.global BattleMotion_SetupEscapeObject
	.thumb_func
	.type BattleMotion_SetupEscapeObject, %function
BattleMotion_SetupEscapeObject:
	.incbin "baserom.gba", 0x0011bee0, 0x000000d8
	.section .rom.0011bfe8, "ax"
	.incbin "baserom.gba", 0x0011bfe8, 0x0000004c
	.section .rom.0011c034, "ax"
	.global Func_0811c01c
	.thumb_func
	.type Func_0811c01c, %function
Func_0811c01c:
	.incbin "baserom.gba", 0x0011c034, 0x00000058
	.section .rom.0011c08c, "ax"
	.global Func_0811c074
	.thumb_func
	.type Func_0811c074, %function
Func_0811c074:
	.incbin "baserom.gba", 0x0011c08c, 0x000000ac
	.section .rom.0011c138, "ax"
	.global Func_0811c120
	.thumb_func
	.type Func_0811c120, %function
Func_0811c120:
	.incbin "baserom.gba", 0x0011c138, 0x000000d8
	.section .rom.0011c228, "ax"
	.incbin "baserom.gba", 0x0011c228, 0x000000a4
	.section .rom.0011c2cc, "ax"
	.global Func_0811c2b4
	.thumb_func
	.type Func_0811c2b4, %function
Func_0811c2b4:
	.incbin "baserom.gba", 0x0011c2cc, 0x00000060
	.section .rom.0011c32c, "ax"
	.global Func_0811c314
	.thumb_func
	.type Func_0811c314, %function
Func_0811c314:
	.incbin "baserom.gba", 0x0011c32c, 0x00000068
	.section .rom.0011c394, "ax"
	.global Func_0811c37c
	.thumb_func
	.type Func_0811c37c, %function
Func_0811c37c:
	.incbin "baserom.gba", 0x0011c394, 0x000002d4
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x00000002
	.section .rom.0011c728, "ax"
	.incbin "baserom.gba", 0x0011c728, 0x000004d8
	.section .rom.0011cd08, "ax"
	.global BattleMotion_SetMode5AndActivateSlot
	.thumb_func
	.type BattleMotion_SetMode5AndActivateSlot, %function
BattleMotion_SetMode5AndActivateSlot:
	.incbin "baserom.gba", 0x0011cd08, 0x0000008c
	.section .rom.0011cd94, "ax"
	.global Camera_ConfigureScene
	.thumb_func
	.type Camera_ConfigureScene, %function
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd94, 0x000000d4
	.section .rom.0011ceac, "ax"
	.global BattleEscape_CheckSuccess
	.thumb_func
	.type BattleEscape_CheckSuccess, %function
BattleEscape_CheckSuccess:
	.incbin "baserom.gba", 0x0011ceac, 0x00000110
	.global BattlePres_BuildUnitEntries
	.thumb_func
	.type BattlePres_BuildUnitEntries, %function
BattlePres_BuildUnitEntries:
	.incbin "baserom.gba", 0x0011cfbc, 0x0000002c
	.global BattlePres_BuildOpponentEntries
	.thumb_func
	.type BattlePres_BuildOpponentEntries, %function
BattlePres_BuildOpponentEntries:
	.incbin "baserom.gba", 0x0011cfe8, 0x00000190
	.global BattleQueue_SortByPriority
	.thumb_func
	.type BattleQueue_SortByPriority, %function
BattleQueue_SortByPriority:
	.incbin "baserom.gba", 0x0011d178, 0x000002b4
	.global BattlePresentation_AppendLinkedActions
	.thumb_func
	.type BattlePresentation_AppendLinkedActions, %function
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x0011d42c, 0x0000018c
	.section .rom.0011d7b4, "ax"
	.incbin "baserom.gba", 0x0011d7b4, 0x00000bd0
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00000c86
	.section .rom.0011f048, "ax"
	.global BattleMotion_SetRecordChildValues
	.thumb_func
	.type BattleMotion_SetRecordChildValues, %function
BattleMotion_SetRecordChildValues:
	.incbin "baserom.gba", 0x0011f048, 0x00000300
	.section .rom.0011f4ec, "ax"
	.incbin "baserom.gba", 0x0011f4ec, 0x00000968
	.section .rom.0011ff1e, "ax"
	.incbin "baserom.gba", 0x0011ff1e, 0x00000002
	.section .rom.0011ff20, "ax"
	.global BattlePresentation_WaitForAdvance
	.thumb_func
	.type BattlePresentation_WaitForAdvance, %function
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x0011ff20, 0x00000158
	.section .rom.00120078, "ax"
	.global Func_08120060
	.thumb_func
	.type Func_08120060, %function
Func_08120060:
	.incbin "baserom.gba", 0x00120078, 0x00000118
	.section .rom.001201dc, "ax"
	.global BattleEv_DispatchQueued
	.thumb_func
	.type BattleEv_DispatchQueued, %function
BattleEv_DispatchQueued:
	.incbin "baserom.gba", 0x001201dc, 0x000001c4
	.section .rom.001203c0, "ax"
	.incbin "baserom.gba", 0x001203c0, 0x000000ac
	.section .rom.0012046c, "ax"
	.global Battle_ResolveTargetAction
	.thumb_func
	.type Battle_ResolveTargetAction, %function
Battle_ResolveTargetAction:
	.incbin "baserom.gba", 0x0012046c, 0x0000206c
	.section .rom.001224f0, "ax"
	.incbin "baserom.gba", 0x001224f0, 0x00000774
	.section .rom.00122c64, "ax"
	.global Func_08122c4c
	.thumb_func
	.type Func_08122c4c, %function
Func_08122c4c:
	.incbin "baserom.gba", 0x00122c64, 0x000008e8
	.section .rom.0012358c, "ax"
	.global Func_08123574
	.thumb_func
	.type Func_08123574, %function
Func_08123574:
	.incbin "baserom.gba", 0x0012358c, 0x0000129c
	.section .rom.00124b10, "ax"
	.incbin "baserom.gba", 0x00124b10, 0x000000b0
	.section .rom.00124cd6, "ax"
	.incbin "baserom.gba", 0x00124cd6, 0x00000eba
	.section .rom.00125bd0, "ax"
	.incbin "baserom.gba", 0x00125bd0, 0x00000660
	.global Func_08126218
	.thumb_func
	.type Func_08126218, %function
Func_08126218:
	.incbin "baserom.gba", 0x00126230, 0x00000074
	.global Func_0812628c
	.thumb_func
	.type Func_0812628c, %function
Func_0812628c:
	.incbin "baserom.gba", 0x001262a4, 0x00000170
	.global Func_081263fc
	.thumb_func
	.type Func_081263fc, %function
Func_081263fc:
	.incbin "baserom.gba", 0x00126414, 0x00000408
	.section .rom.0012681c, "ax"
	.global BattlePres_SetupTransitionScene
	.thumb_func
	.type BattlePres_SetupTransitionScene, %function
BattlePres_SetupTransitionScene:
	.incbin "baserom.gba", 0x0012681c, 0x00000100
	.section .rom.0012695a, "ax"
	.incbin "baserom.gba", 0x0012695a, 0x00000002
	.global Func_08126944
	.thumb_func
	.type Func_08126944, %function
Func_08126944:
	.incbin "baserom.gba", 0x0012695c, 0x00000034
	.global Func_08126978
	.thumb_func
	.type Func_08126978, %function
Func_08126978:
	.incbin "baserom.gba", 0x00126990, 0x00000054
	.section .rom.00126a04, "ax"
	.incbin "baserom.gba", 0x00126a04, 0x000000f8
	.section .rom.00126afc, "ax"
	.global BattlePres_SetActorRecordMode
	.thumb_func
	.type BattlePres_SetActorRecordMode, %function
BattlePres_SetActorRecordMode:
	.incbin "baserom.gba", 0x00126afc, 0x00000080
	.section .rom.00126be4, "ax"
	.incbin "baserom.gba", 0x00126be4, 0x00000130
	.section .rom.00126d14, "ax"
	.global BattlePres_SetActorModes
	.thumb_func
	.type BattlePres_SetActorModes, %function
BattlePres_SetActorModes:
	.incbin "baserom.gba", 0x00126d14, 0x00000ea4
	.section .rom.00127c96, "ax"
	.incbin "baserom.gba", 0x00127c96, 0x00000422
	.section .rom.001281ac, "ax"
	.global Summon_GetEntryByte4
	.thumb_func
	.type Summon_GetEntryByte4, %function
Summon_GetEntryByte4:
	.incbin "baserom.gba", 0x001281ac, 0x0000001c
	.section .rom.00128202, "ax"
	.incbin "baserom.gba", 0x00128202, 0x0000065a
	.global Data_08128844
Data_08128844:
	.incbin "baserom.gba", 0x0012885c, 0x0000428c
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x0012cae8, 0x0000423c
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x00130d24, 0x000072dc
	.section .rom.001380ac, "ax"
	.incbin "baserom.gba", 0x001380ac, 0x00000f1c
	.section .rom.0013ba50, "ax"
	.incbin "baserom.gba", 0x0013ba50, 0x00002980
	.section .rom.0013e3d0, "ax"
	.global Func_0813e3d0
	.thumb_func
	.type Func_0813e3d0, %function
Func_0813e3d0:
	.incbin "baserom.gba", 0x0013e3d0, 0x000007a0
	.section .rom.0013eb70, "ax"
	.global Func_0813eb70
	.thumb_func
	.type Func_0813eb70, %function
Func_0813eb70:
	.incbin "baserom.gba", 0x0013eb70, 0x000009a8
	.section .rom.0013f518, "ax"
	.global Func_0813f518
	.thumb_func
	.type Func_0813f518, %function
Func_0813f518:
	.incbin "baserom.gba", 0x0013f518, 0x00000570
	.section .rom.0013fa88, "ax"
	.global Func_0813fa88
	.thumb_func
	.type Func_0813fa88, %function
Func_0813fa88:
	.incbin "baserom.gba", 0x0013fa88, 0x000002b8
	.section .rom.0013fd40, "ax"
	.global Func_0813fd40
	.thumb_func
	.type Func_0813fd40, %function
Func_0813fd40:
	.incbin "baserom.gba", 0x0013fd40, 0x00002c04
	.section .rom.00142944, "ax"
	.global Func_08142944
	.thumb_func
	.type Func_08142944, %function
Func_08142944:
	.incbin "baserom.gba", 0x00142944, 0x00000c9c
	.section .rom.001435e0, "ax"
	.global BattleFx_BeginCanvasLayer
	.thumb_func
	.type BattleFx_BeginCanvasLayer, %function
BattleFx_BeginCanvasLayer:
	.incbin "baserom.gba", 0x001435e0, 0x000004a8
	.section .rom.00143bb6, "ax"
	.incbin "baserom.gba", 0x00143bb6, 0x00005ff6
	.section .rom.00149bac, "ax"
	.global BattleFx_RunSparkGroups
	.thumb_func
	.type BattleFx_RunSparkGroups, %function
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x00149bac, 0x00000bfc
	.section .rom.0014a812, "ax"
	.incbin "baserom.gba", 0x0014a812, 0x00000002
	.global BattleFx_RenderMode
	.thumb_func
	.type BattleFx_RenderMode, %function
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x0014a814, 0x00002168
	.section .rom.0014c992, "ax"
	.incbin "baserom.gba", 0x0014c992, 0x00000002
	.section .rom.0014c994, "ax"
	.global Func_0814c994
	.thumb_func
	.type Func_0814c994, %function
Func_0814c994:
	.incbin "baserom.gba", 0x0014c994, 0x000001cc
	.section .rom.0014cb60, "ax"
	.global Func_0814cb60
	.thumb_func
	.type Func_0814cb60, %function
Func_0814cb60:
	.incbin "baserom.gba", 0x0014cb60, 0x00002328
	.section .rom.0014ef44, "ax"
	.balign 4
	.global BattleFx_RunSeriesA
	.thumb_func
	.type BattleFx_RunSeriesA, %function
BattleFx_RunSeriesA:
	.incbin "baserom.gba", 0x0014ef44, 0x0000352c
	.section .rom.00152470, "ax"
	.global BattleFx_RunNoEffect
	.thumb_func
	.type BattleFx_RunNoEffect, %function
BattleFx_RunNoEffect:
	.incbin "baserom.gba", 0x00152470, 0x00000084
	.section .rom.0015265a, "ax"
	.incbin "baserom.gba", 0x0015265a, 0x00000002
	.section .rom.0015265c, "ax"
	.global BattleFx_RunProjectileVolleyB
	.thumb_func
	.type BattleFx_RunProjectileVolleyB, %function
BattleFx_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x00003934
	.section .rom.0015613e, "ax"
	.incbin "baserom.gba", 0x0015613e, 0x00000002
	.section .rom.00156140, "ax"
	.global BattleFx_RunProjectileVolley
	.thumb_func
	.type BattleFx_RunProjectileVolley, %function
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
	.thumb_func
	.type BattlePres_RunBurstScene, %function
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x00004210
	.section .rom.0015e320, "ax"
	.global Func_0815e320
	.thumb_func
	.type Func_0815e320, %function
Func_0815e320:
	.incbin "baserom.gba", 0x0015e320, 0x0000bef8
	.section .rom.0016a29c, "ax"
	.incbin "baserom.gba", 0x0016a29c, 0x00014778
	.section .rom.0017ea58, "ax"
	.incbin "baserom.gba", 0x0017ea58, 0x00010b74
	.section .rom.0018f620, "ax"
	.balign 4
	.global Func_0818f620
	.thumb_func
	.type Func_0818f620, %function
Func_0818f620:
	.incbin "baserom.gba", 0x0018f620, 0x00006dcc
	.section .rom.001963ec, "ax"
	.global Func_081963ec
	.thumb_func
	.type Func_081963ec, %function
Func_081963ec:
	.incbin "baserom.gba", 0x001963ec, 0x00000018
	.section .rom.00196404, "ax"
	.global Func_08196404
	.thumb_func
	.type Func_08196404, %function
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000e2c
	.section .rom.001973f0, "ax"
	.incbin "baserom.gba", 0x001973f0, 0x00008c10
	.global Resource_FarCall00A
Resource_FarCall00A:
	.incbin "baserom.gba", 0x001a0000, 0x00000008
	.global Func_081a0008
	.thumb_func
	.type Func_081a0008, %function
Func_081a0008:
	.incbin "baserom.gba", 0x001a0008, 0x00000008
	.global Func_081a0010
	.thumb_func
	.type Func_081a0010, %function
Func_081a0010:
	.incbin "baserom.gba", 0x001a0010, 0x00000008
	.global Func_081a0018
	.thumb_func
	.type Func_081a0018, %function
Func_081a0018:
	.incbin "baserom.gba", 0x001a0018, 0x00000008
	.global Func_081a0020
	.thumb_func
	.type Func_081a0020, %function
Func_081a0020:
	.incbin "baserom.gba", 0x001a0020, 0x00000008
	.global Func_081a0028
	.thumb_func
	.type Func_081a0028, %function
Func_081a0028:
	.incbin "baserom.gba", 0x001a0028, 0x00000008
	.section .rom.001a04d0, "ax"
	.incbin "baserom.gba", 0x001a04d0, 0x00000db8
	.section .rom.001a1294, "ax"
	.incbin "baserom.gba", 0x001a1294, 0x00004d6c
	.global Resource_FarCall00B
Resource_FarCall00B:
	.incbin "baserom.gba", 0x001a6000, 0x00000018
	.global Func_081a6018
	.thumb_func
	.type Func_081a6018, %function
Func_081a6018:
	.incbin "baserom.gba", 0x001a6018, 0x00000008
	.global Func_081a6020
	.thumb_func
	.type Func_081a6020, %function
Func_081a6020:
	.incbin "baserom.gba", 0x001a6020, 0x00000008
	.global Func_081a6028
	.thumb_func
	.type Func_081a6028, %function
Func_081a6028:
	.incbin "baserom.gba", 0x001a6028, 0x0000223c
	.section .rom.001a8286, "ax"
	.incbin "baserom.gba", 0x001a8286, 0x00003d7a
	.section .rom.001ac026, "ax"
	.incbin "baserom.gba", 0x001ac026, 0x0000008a
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.thumb_func
	.type LuckyDice_Run, %function
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x00005ea4
	.section .rom.001b2008, "ax"
	.global Func_081b2008
	.thumb_func
	.type Func_081b2008, %function
Func_081b2008:
	.incbin "baserom.gba", 0x001b2008, 0x00000034
	.section .rom.001b20a0, "ax"
	.incbin "baserom.gba", 0x001b20a0, 0x00001dd4
	.section .rom.001b3eb0, "ax"
	.incbin "baserom.gba", 0x001b3eb0, 0x00000040
	.section .rom.001b3f18, "ax"
	.incbin "baserom.gba", 0x001b3f18, 0x000040e8
	.section .rom.001b8008, "ax"
	.global Func_081b8008
	.thumb_func
	.type Func_081b8008, %function
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
	.thumb_func
	.type Func_081c0168, %function
Func_081c0168:
	.incbin "baserom.gba", 0x001c0168, 0x00000030
	.section .rom.001c03d8, "ax"
	.global Func_081c03d8
	.thumb_func
	.type Func_081c03d8, %function
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
	.thumb_func
	.type MusicTrack_HandleNote, %function
MusicTrack_HandleNote:
	.incbin "baserom.gba", 0x001c07a2, 0x00000102
	.section .rom.001c08a4, "ax"
	.global Func_081c08a4
	.thumb_func
	.type Func_081c08a4, %function
Func_081c08a4:
	.incbin "baserom.gba", 0x001c08a4, 0x0000012c
	.section .rom.001c09d0, "ax"
	.global MusicPlayer_Tick
	.thumb_func
	.type MusicPlayer_Tick, %function
MusicPlayer_Tick:
	.incbin "baserom.gba", 0x001c09d0, 0x00000248
	.section .rom.001c0c18, "ax"
	.global Func_081c0c18
	.thumb_func
	.type Func_081c0c18, %function
Func_081c0c18:
	.incbin "baserom.gba", 0x001c0c18, 0x00000004
	.section .rom.001c0c1c, "ax"
	.global Func_081c0c1c
	.thumb_func
	.type Func_081c0c1c, %function
Func_081c0c1c:
	.incbin "baserom.gba", 0x001c0c1c, 0x00000094
	.section .rom.001c0cb0, "ax"
	.global Func_081c0cb0
	.thumb_func
	.type Func_081c0cb0, %function
Func_081c0cb0:
	.incbin "baserom.gba", 0x001c0cb0, 0x00000180
	.section .rom.001c0e30, "ax"
	.global Func_081c0e30
	.thumb_func
	.type Func_081c0e30, %function
Func_081c0e30:
	.incbin "baserom.gba", 0x001c0e30, 0x00000040
	.section .rom.001c0e70, "ax"
	.global Func_081c0e70
	.thumb_func
	.type Func_081c0e70, %function
Func_081c0e70:
	.incbin "baserom.gba", 0x001c0e70, 0x000000cc
	.section .rom.001c0f3c, "ax"
	.global Func_081c0f3c
	.thumb_func
	.type Func_081c0f3c, %function
Func_081c0f3c:
	.incbin "baserom.gba", 0x001c0f3c, 0x00000008
	.section .rom.001c0f44, "ax"
	.global Func_081c0f44
	.thumb_func
	.type Func_081c0f44, %function
Func_081c0f44:
	.incbin "baserom.gba", 0x001c0f44, 0x00000014
	.section .rom.001c0f70, "ax"
	.global Func_081c0f70
	.thumb_func
	.type Func_081c0f70, %function
Func_081c0f70:
	.incbin "baserom.gba", 0x001c0f70, 0x00000014
	.section .rom.001c0fac, "ax"
	.global Func_081c0fac
	.thumb_func
	.type Func_081c0fac, %function
Func_081c0fac:
	.incbin "baserom.gba", 0x001c0fac, 0x00000014
	.section .rom.001c0fc0, "ax"
	.global Func_081c0fc0
	.thumb_func
	.type Func_081c0fc0, %function
Func_081c0fc0:
	.incbin "baserom.gba", 0x001c0fc0, 0x00000008
	.section .rom.001c0fc8, "ax"
	.global Func_081c0fc8
	.thumb_func
	.type Func_081c0fc8, %function
Func_081c0fc8:
	.incbin "baserom.gba", 0x001c0fc8, 0x00000008
	.section .rom.001c0fd0, "ax"
	.global Func_081c0fd0
	.thumb_func
	.type Func_081c0fd0, %function
Func_081c0fd0:
	.incbin "baserom.gba", 0x001c0fd0, 0x0000000c
	.section .rom.001c1000, "ax"
	.global AudioEngine_SuspendDirectSoundWrapper
	.thumb_func
	.type AudioEngine_SuspendDirectSoundWrapper, %function
AudioEngine_SuspendDirectSoundWrapper:
	.incbin "baserom.gba", 0x001c1000, 0x00000014
	.section .rom.001c1014, "ax"
	.global Func_081c1014
	.thumb_func
	.type Func_081c1014, %function
Func_081c1014:
	.incbin "baserom.gba", 0x001c1014, 0x00000144
	.section .rom.001c117c, "ax"
	.global Func_081c117c
	.thumb_func
	.type Func_081c117c, %function
Func_081c117c:
	.incbin "baserom.gba", 0x001c117c, 0x0000000c
	.section .rom.001c1188, "ax"
	.global Func_081c1188
	.thumb_func
	.type Func_081c1188, %function
Func_081c1188:
	.incbin "baserom.gba", 0x001c1188, 0x00000024
	.section .rom.001c11ac, "ax"
	.global Func_081c11ac
	.thumb_func
	.type Func_081c11ac, %function
Func_081c11ac:
	.incbin "baserom.gba", 0x001c11ac, 0x00000020
	.section .rom.001c11cc, "ax"
	.global Func_081c11cc
	.thumb_func
	.type Func_081c11cc, %function
Func_081c11cc:
	.incbin "baserom.gba", 0x001c11cc, 0x000000b0
	.section .rom.001c16ca, "ax"
	.incbin "baserom.gba", 0x001c16ca, 0x00000022
	.global MusicTrack_FinishSequence
	.thumb_func
	.type MusicTrack_FinishSequence, %function
MusicTrack_FinishSequence:
	.incbin "baserom.gba", 0x001c16ec, 0x00000030
	.section .rom.001c1e48, "ax"
	.global Func_081c1e48
	.thumb_func
	.type Func_081c1e48, %function
Func_081c1e48:
	.incbin "baserom.gba", 0x001c1e48, 0x0000001c
	.section .rom.001c1e6e, "ax"
	.incbin "baserom.gba", 0x001c1e6e, 0x00000002
	.section .rom.001c2314, "ax"
	.global Func_081c2314
	.thumb_func
	.type Func_081c2314, %function
Func_081c2314:
	.incbin "baserom.gba", 0x001c2314, 0x00000014
	.global AudioCommand_InvokeSlot35
	.global Func_081c2328
	.thumb_func
	.type AudioCommand_InvokeSlot35, %function
AudioCommand_InvokeSlot35:
Func_081c2328:
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
	.thumb_func
	.type Audio_EmptyCallback, %function
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
