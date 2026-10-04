@ tla-fr's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.section .rom.00024f1e, "ax"
	.incbin "baserom.gba", 0x00024f1e, 0x00000002
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
	.incbin "baserom.gba", 0x0003972a, 0x000008f6
	.section .rom.0003a04e, "ax"
	.incbin "baserom.gba", 0x0003a04e, 0x00000002
	.section .rom.0003a050, "ax"
	.global Func_0803a084
	.thumb_func
	.type Func_0803a084, %function
Func_0803a084:
	.incbin "baserom.gba", 0x0003a050, 0x00000334
	.section .rom.0003a3b0, "ax"
	.balign 4
	.global UiWork_IsIdle
	.thumb_func
	.type UiWork_IsIdle, %function
UiWork_IsIdle:
	.incbin "baserom.gba", 0x0003a3b0, 0x0000014c
	.section .rom.0003a4fc, "ax"
	.global Func_0803a530
	.thumb_func
	.type Func_0803a530, %function
Func_0803a530:
	.incbin "baserom.gba", 0x0003a4fc, 0x0000001c
	.section .rom.0003a518, "ax"
	.global Func_0803a54c
	.thumb_func
	.type Func_0803a54c, %function
Func_0803a54c:
	.incbin "baserom.gba", 0x0003a518, 0x00000094
	.section .rom.0003a5ac, "ax"
	.global Func_0803a5e0
	.thumb_func
	.type Func_0803a5e0, %function
Func_0803a5e0:
	.incbin "baserom.gba", 0x0003a5ac, 0x0000002c
	.section .rom.0003a5d8, "ax"
	.global Func_0803a60c
	.thumb_func
	.type Func_0803a60c, %function
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5d8, 0x0000005c
	.section .rom.0003a668, "ax"
	.global UiText_OpenMessageWindow
	.thumb_func
	.type UiText_OpenMessageWindow, %function
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a668, 0x00000110
	.section .rom.0003a778, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
	.type UiText_ShowPositionedMessageAndWait, %function
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0003a778, 0x000002f8
	.section .rom.0003aa96, "ax"
	.incbin "baserom.gba", 0x0003aa96, 0x00000002
	.section .rom.0003aa98, "ax"
	.global UiText_RenderWideStringAtOffset
	.thumb_func
	.type UiText_RenderWideStringAtOffset, %function
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa98, 0x00000310
	.global UiText_FormatNumber
	.thumb_func
	.type UiText_FormatNumber, %function
UiText_FormatNumber:
	.incbin "baserom.gba", 0x0003ada8, 0x0000027c
	.section .rom.0003b024, "ax"
	.global UiText_BuildRenderEntries
	.thumb_func
	.type UiText_BuildRenderEntries, %function
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003b024, 0x0000084c
	.section .rom.0003b888, "ax"
	.global UiText_GetResourceDimensions
	.thumb_func
	.type UiText_GetResourceDimensions, %function
UiText_GetResourceDimensions:
	.incbin "baserom.gba", 0x0003b888, 0x0000004c
	.section .rom.0003b8d4, "ax"
	.global Func_0803b8cc
	.thumb_func
	.type Func_0803b8cc, %function
Func_0803b8cc:
	.incbin "baserom.gba", 0x0003b8d4, 0x0000004c
	.section .rom.0003b920, "ax"
	.global UiText_MeasureEntryDimensions
	.thumb_func
	.type UiText_MeasureEntryDimensions, %function
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b920, 0x00000734
	.section .rom.0003c054, "ax"
	.global Func_0803c068
	.thumb_func
	.type Func_0803c068, %function
Func_0803c068:
	.incbin "baserom.gba", 0x0003c054, 0x00000108
	.section .rom.0003c15c, "ax"
	.global Func_0803c170
	.thumb_func
	.type Func_0803c170, %function
Func_0803c170:
	.incbin "baserom.gba", 0x0003c15c, 0x00000028
	.section .rom.0003c184, "ax"
	.global Func_0803c198
	.thumb_func
	.type Func_0803c198, %function
Func_0803c198:
	.incbin "baserom.gba", 0x0003c184, 0x000001e0
	.section .rom.0003c364, "ax"
	.global UiWindow_SetTilemapEntry
	.thumb_func
	.type UiWindow_SetTilemapEntry, %function
UiWindow_SetTilemapEntry:
	.incbin "baserom.gba", 0x0003c364, 0x00000634
	.section .rom.0003c9a8, "ax"
	.global UiText_CopyMessageString
	.thumb_func
	.type UiText_CopyMessageString, %function
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9a8, 0x00000064
	.section .rom.0003ca0c, "ax"
	.global UiText_DecodeMessage
	.thumb_func
	.type UiText_DecodeMessage, %function
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca0c, 0x00000144
	.section .rom.0003cb5e, "ax"
	.incbin "baserom.gba", 0x0003cb5e, 0x00000002
	.section .rom.0003cbea, "ax"
	.incbin "baserom.gba", 0x0003cbea, 0x00000102
	.section .rom.0003ccec, "ax"
	.global Func_0803cca8
	.thumb_func
	.type Func_0803cca8, %function
Func_0803cca8:
	.incbin "baserom.gba", 0x0003ccec, 0x00000028
	.section .rom.0003cd4c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd4c, 0x00000114
	.section .rom.0003ce60, "ax"
	.global Func_0803ce1c
	.thumb_func
	.type Func_0803ce1c, %function
Func_0803ce1c:
	.incbin "baserom.gba", 0x0003ce60, 0x00000048
	.section .rom.0003cea8, "ax"
	.global Func_0803ce64
	.thumb_func
	.type Func_0803ce64, %function
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cea8, 0x000001bc
	.section .rom.0003d064, "ax"
	.global Func_0803d020
	.thumb_func
	.type Func_0803d020, %function
Func_0803d020:
	.incbin "baserom.gba", 0x0003d064, 0x000002d0
	.section .rom.0003d334, "ax"
	.global Localization_LookupEntryId
	.thumb_func
	.type Localization_LookupEntryId, %function
Localization_LookupEntryId:
	.incbin "baserom.gba", 0x0003d334, 0x000000d0
	.section .rom.0003d404, "ax"
	.global Func_0803d3c0
	.thumb_func
	.type Func_0803d3c0, %function
Func_0803d3c0:
	.incbin "baserom.gba", 0x0003d404, 0x00000090
	.section .rom.0003d494, "ax"
	.global Func_0803d450
	.thumb_func
	.type Func_0803d450, %function
Func_0803d450:
	.incbin "baserom.gba", 0x0003d494, 0x0000006c
	.section .rom.0003d528, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.thumb_func
	.type Ui_BuildPairedPatternsToSlot, %function
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d528, 0x000000e0
	.section .rom.0003d608, "ax"
	.global Func_0803d5c4
	.thumb_func
	.type Func_0803d5c4, %function
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d608, 0x000000bc
	.section .rom.0003d6c4, "ax"
	.global ItemIcon_Compose
	.thumb_func
	.type ItemIcon_Compose, %function
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6c4, 0x0000022c
	.section .rom.0003d96e, "ax"
	.incbin "baserom.gba", 0x0003d96e, 0x00000002
	.section .rom.0003d970, "ax"
	.global Func_0803d92c
	.thumb_func
	.type Func_0803d92c, %function
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d970, 0x00000060
	.section .rom.0003d9fe, "ax"
	.incbin "baserom.gba", 0x0003d9fe, 0x00000002
	.section .rom.0003da00, "ax"
	.global Func_0803d9bc
	.thumb_func
	.type Func_0803d9bc, %function
Func_0803d9bc:
	.incbin "baserom.gba", 0x0003da00, 0x000000bc
	.section .rom.0003dabc, "ax"
	.global Ui_PrepareTransferFromTableEntry
	.thumb_func
	.type Ui_PrepareTransferFromTableEntry, %function
Ui_PrepareTransferFromTableEntry:
	.incbin "baserom.gba", 0x0003dabc, 0x000001a4
	.section .rom.0003dc60, "ax"
	.global Func_0803dc1c
	.thumb_func
	.type Func_0803dc1c, %function
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc60, 0x00000108
	.section .rom.0003dd68, "ax"
	.global Func_0803dd24
	.thumb_func
	.type Func_0803dd24, %function
Func_0803dd24:
	.incbin "baserom.gba", 0x0003dd68, 0x00000044
	.section .rom.0003ddac, "ax"
	.global Func_0803dd68
	.thumb_func
	.type Func_0803dd68, %function
Func_0803dd68:
	.incbin "baserom.gba", 0x0003ddac, 0x00000030
	.section .rom.0003dddc, "ax"
	.global Func_0803dd98
	.thumb_func
	.type Func_0803dd98, %function
Func_0803dd98:
	.incbin "baserom.gba", 0x0003dddc, 0x00000110
	.section .rom.0003deec, "ax"
	.global Func_0803dea8
	.thumb_func
	.type Func_0803dea8, %function
Func_0803dea8:
	.incbin "baserom.gba", 0x0003deec, 0x00000004
	.section .rom.0003df44, "ax"
	.global Func_0803df00
	.thumb_func
	.type Func_0803df00, %function
Func_0803df00:
	.incbin "baserom.gba", 0x0003df44, 0x00000014
	.section .rom.0003df58, "ax"
	.global Resource_ScheduleOwnerReset
	.thumb_func
	.type Resource_ScheduleOwnerReset, %function
Resource_ScheduleOwnerReset:
	.incbin "baserom.gba", 0x0003df58, 0x00000694
	.section .rom.0003e7b8, "ax"
	.global Func_0803e774
	.thumb_func
	.type Func_0803e774, %function
Func_0803e774:
	.incbin "baserom.gba", 0x0003e7b8, 0x00000038
	.section .rom.0003e7f0, "ax"
	.global Func_0803e7ac
	.thumb_func
	.type Func_0803e7ac, %function
Func_0803e7ac:
	.incbin "baserom.gba", 0x0003e7f0, 0x00000140
	.section .rom.0003e95a, "ax"
	.incbin "baserom.gba", 0x0003e95a, 0x00000002
	.section .rom.0003e95c, "ax"
	.global Func_0803e918
	.thumb_func
	.type Func_0803e918, %function
Func_0803e918:
	.incbin "baserom.gba", 0x0003e95c, 0x00000080
	.section .rom.0003e9dc, "ax"
	.global Func_0803e998
	.thumb_func
	.type Func_0803e998, %function
Func_0803e998:
	.incbin "baserom.gba", 0x0003e9dc, 0x0000063c
	.section .rom.0003f048, "ax"
	.incbin "baserom.gba", 0x0003f048, 0x000001d0
	.section .rom.0003f218, "ax"
	.global Resource_LoadByMode
	.thumb_func
	.type Resource_LoadByMode, %function
Resource_LoadByMode:
	.incbin "baserom.gba", 0x0003f218, 0x00000060
	.section .rom.0003f278, "ax"
	.global Func_0803f234
	.thumb_func
	.type Func_0803f234, %function
Func_0803f234:
	.incbin "baserom.gba", 0x0003f278, 0x000003dc
	.section .rom.0003f654, "ax"
	.global Func_0803f610
	.thumb_func
	.type Func_0803f610, %function
Func_0803f610:
	.incbin "baserom.gba", 0x0003f654, 0x00000004
	.section .rom.0003f658, "ax"
	.global Func_0803f614
	.thumb_func
	.type Func_0803f614, %function
Func_0803f614:
	.incbin "baserom.gba", 0x0003f658, 0x0000000c
	.section .rom.0003f664, "ax"
	.global Func_0803f620
	.thumb_func
	.type Func_0803f620, %function
Func_0803f620:
	.incbin "baserom.gba", 0x0003f664, 0x00000004
	.section .rom.0003f668, "ax"
	.global UiTextResource_Initialize
	.thumb_func
	.type UiTextResource_Initialize, %function
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f668, 0x00000074
	.section .rom.0003f6dc, "ax"
	.global UiTextResource_SetPosition
	.thumb_func
	.type UiTextResource_SetPosition, %function
UiTextResource_SetPosition:
	.incbin "baserom.gba", 0x0003f6dc, 0x00000028
	.section .rom.0003f704, "ax"
	.global UiTextResource_Release
	.thumb_func
	.type UiTextResource_Release, %function
UiTextResource_Release:
	.incbin "baserom.gba", 0x0003f704, 0x00000098
	.global Resource_ResetPendingTransfer
	.thumb_func
	.type Resource_ResetPendingTransfer, %function
Resource_ResetPendingTransfer:
	.incbin "baserom.gba", 0x0003f79c, 0x00000020
	.section .rom.0003f7bc, "ax"
	.global Func_0803f778
	.thumb_func
	.type Func_0803f778, %function
Func_0803f778:
	.incbin "baserom.gba", 0x0003f7bc, 0x000000f4
	.section .rom.0003f8b0, "ax"
	.global Func_0803f86c
	.thumb_func
	.type Func_0803f86c, %function
Func_0803f86c:
	.incbin "baserom.gba", 0x0003f8b0, 0x000000d0
	.section .rom.0003f980, "ax"
	.global UiTimedNotice_CloseIfActive
	.thumb_func
	.type UiTimedNotice_CloseIfActive, %function
UiTimedNotice_CloseIfActive:
	.incbin "baserom.gba", 0x0003f980, 0x0000002c
	.section .rom.0003f9ac, "ax"
	.global Func_0803f968
	.thumb_func
	.type Func_0803f968, %function
Func_0803f968:
	.incbin "baserom.gba", 0x0003f9ac, 0x00000058
	.section .rom.0003fa04, "ax"
	.global Func_0803f9c0
	.thumb_func
	.type Func_0803f9c0, %function
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa04, 0x00000728
	.section .rom.0004012c, "ax"
	.global Func_080400e8
	.thumb_func
	.type Func_080400e8, %function
Func_080400e8:
	.incbin "baserom.gba", 0x0004012c, 0x000015e4
	.global UiText_RenderStringTiles
	.thumb_func
	.type UiText_RenderStringTiles, %function
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x00041710, 0x000004a8
	.section .rom.00041bb8, "ax"
	.global Func_08041b68
	.thumb_func
	.type Func_08041b68, %function
Func_08041b68:
	.incbin "baserom.gba", 0x00041bb8, 0x000000a4
	.section .rom.00041c5c, "ax"
	.global Func_08041c0c
	.thumb_func
	.type Func_08041c0c, %function
Func_08041c0c:
	.incbin "baserom.gba", 0x00041c5c, 0x00000048
	.section .rom.00041ca4, "ax"
	.global UiWindow_DrawDividerLine
	.thumb_func
	.type UiWindow_DrawDividerLine, %function
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041ca4, 0x0000031c
	.section .rom.00041fc0, "ax"
	.global Func_08041f70
	.thumb_func
	.type Func_08041f70, %function
Func_08041f70:
	.incbin "baserom.gba", 0x00041fc0, 0x00000020
	.section .rom.00041fe0, "ax"
	.global Func_08041f90
	.thumb_func
	.type Func_08041f90, %function
Func_08041f90:
	.incbin "baserom.gba", 0x00041fe0, 0x00000014
	.section .rom.00041ff4, "ax"
	.global UiText_DrawResource
	.thumb_func
	.type UiText_DrawResource, %function
UiText_DrawResource:
	.incbin "baserom.gba", 0x00041ff4, 0x0000006c
	.section .rom.00042060, "ax"
	.global UiText_DrawCharacterAtOffset
	.thumb_func
	.type UiText_DrawCharacterAtOffset, %function
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042060, 0x00000098
	.section .rom.000420f8, "ax"
	.global UiText_DrawString
	.thumb_func
	.type UiText_DrawString, %function
UiText_DrawString:
	.incbin "baserom.gba", 0x000420f8, 0x00000054
	.section .rom.0004214c, "ax"
	.global UiText_DrawStringAtOffset
	.thumb_func
	.type UiText_DrawStringAtOffset, %function
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x0004214c, 0x0000008c
	.section .rom.000421d8, "ax"
	.global UiText_DrawStringInWindow
	.thumb_func
	.type UiText_DrawStringInWindow, %function
UiText_DrawStringInWindow:
	.incbin "baserom.gba", 0x000421d8, 0x0000005c
	.section .rom.00042234, "ax"
	.global UiText_DrawNumber
	.thumb_func
	.type UiText_DrawNumber, %function
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042234, 0x00000030
	.section .rom.00042264, "ax"
	.global UiText_DrawNumberAtOffset
	.thumb_func
	.type UiText_DrawNumberAtOffset, %function
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042264, 0x00000030
	.section .rom.00042294, "ax"
	.global UiText_DrawNumberInWindow
	.thumb_func
	.type UiText_DrawNumberInWindow, %function
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x00042294, 0x000000d0
	.section .rom.00042364, "ax"
	.global RenderOutput_Create
	.thumb_func
	.type RenderOutput_Create, %function
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042364, 0x00000088
	.section .rom.000424a0, "ax"
	.global Func_08042450
	.thumb_func
	.type Func_08042450, %function
Func_08042450:
	.incbin "baserom.gba", 0x000424a0, 0x000000b8
	.section .rom.00042558, "ax"
	.global Func_08042508
	.thumb_func
	.type Func_08042508, %function
Func_08042508:
	.incbin "baserom.gba", 0x00042558, 0x00000064
	.section .rom.000425ca, "ax"
	.incbin "baserom.gba", 0x000425ca, 0x00000002
	.section .rom.000425cc, "ax"
	.global Func_0804257c
	.thumb_func
	.type Func_0804257c, %function
Func_0804257c:
	.incbin "baserom.gba", 0x000425cc, 0x0000000c
	.section .rom.000425d8, "ax"
	.global Func_08042588
	.thumb_func
	.type Func_08042588, %function
Func_08042588:
	.incbin "baserom.gba", 0x000425d8, 0x00000074
	.section .rom.00042680, "ax"
	.incbin "baserom.gba", 0x00042680, 0x00000060
	.section .rom.000426e0, "ax"
	.global Func_08042690
	.thumb_func
	.type Func_08042690, %function
Func_08042690:
	.incbin "baserom.gba", 0x000426e0, 0x000002ec
	.section .rom.000429cc, "ax"
	.global Func_0804297c
	.thumb_func
	.type Func_0804297c, %function
Func_0804297c:
	.incbin "baserom.gba", 0x000429cc, 0x00000430
	.section .rom.00042dfc, "ax"
	.global Func_08042dac
	.thumb_func
	.type Func_08042dac, %function
Func_08042dac:
	.incbin "baserom.gba", 0x00042dfc, 0x00000318
	.section .rom.00043114, "ax"
	.global Func_080430c4
	.thumb_func
	.type Func_080430c4, %function
Func_080430c4:
	.incbin "baserom.gba", 0x00043114, 0x000000ec
	.global Text_FormatPlayTime
	.thumb_func
	.type Text_FormatPlayTime, %function
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x00043200, 0x000000ac
	.section .rom.000432ac, "ax"
	.global Func_0804325c
	.thumb_func
	.type Func_0804325c, %function
Func_0804325c:
	.incbin "baserom.gba", 0x000432ac, 0x00000048
	.section .rom.000432f4, "ax"
	.global Func_080432a4
	.thumb_func
	.type Func_080432a4, %function
Func_080432a4:
	.incbin "baserom.gba", 0x000432f4, 0x00000248
	.section .rom.0004353c, "ax"
	.global Func_080434ec
	.thumb_func
	.type Func_080434ec, %function
Func_080434ec:
	.incbin "baserom.gba", 0x0004353c, 0x00000048
	.section .rom.00043584, "ax"
	.global Func_08043534
	.thumb_func
	.type Func_08043534, %function
Func_08043534:
	.incbin "baserom.gba", 0x00043584, 0x000000c8
	.section .rom.0004364c, "ax"
	.global Func_080435fc
	.thumb_func
	.type Func_080435fc, %function
Func_080435fc:
	.incbin "baserom.gba", 0x0004364c, 0x00000094
	.section .rom.000436e0, "ax"
	.global Func_08043690
	.thumb_func
	.type Func_08043690, %function
Func_08043690:
	.incbin "baserom.gba", 0x000436e0, 0x000000e4
	.section .rom.000437c4, "ax"
	.global Func_08043774
	.thumb_func
	.type Func_08043774, %function
Func_08043774:
	.incbin "baserom.gba", 0x000437c4, 0x00000098
	.section .rom.0004385c, "ax"
	.global Func_0804380c
	.thumb_func
	.type Func_0804380c, %function
Func_0804380c:
	.incbin "baserom.gba", 0x0004385c, 0x000000bc
	.section .rom.00043918, "ax"
	.global Func_080438c8
	.thumb_func
	.type Func_080438c8, %function
Func_080438c8:
	.incbin "baserom.gba", 0x00043918, 0x000000b0
	.section .rom.00043a14, "ax"
	.incbin "baserom.gba", 0x00043a14, 0x00000228
	.section .rom.00043d40, "ax"
	.incbin "baserom.gba", 0x00043d40, 0x00000788
	.section .rom.000444c8, "ax"
	.global Func_08044460
	.thumb_func
	.type Func_08044460, %function
Func_08044460:
	.incbin "baserom.gba", 0x000444c8, 0x00000018
	.section .rom.000444e0, "ax"
	.global Func_08044478
	.thumb_func
	.type Func_08044478, %function
Func_08044478:
	.incbin "baserom.gba", 0x000444e0, 0x00000010
	.section .rom.000444f0, "ax"
	.global Func_08044488
	.thumb_func
	.type Func_08044488, %function
Func_08044488:
	.incbin "baserom.gba", 0x000444f0, 0x000000d0
	.section .rom.000445c0, "ax"
	.global Func_08044558
	.thumb_func
	.type Func_08044558, %function
Func_08044558:
	.incbin "baserom.gba", 0x000445c0, 0x0000051c
	.section .rom.00044adc, "ax"
	.global Func_08044a54
	.thumb_func
	.type Func_08044a54, %function
Func_08044a54:
	.incbin "baserom.gba", 0x00044adc, 0x00000004
	.section .rom.00044ae0, "ax"
	.global Func_08044a58
	.thumb_func
	.type Func_08044a58, %function
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae0, 0x00000140
	.section .rom.00044c20, "ax"
	.global Func_08044b98
	.thumb_func
	.type Func_08044b98, %function
Func_08044b98:
	.incbin "baserom.gba", 0x00044c20, 0x000000e8
	.section .rom.00044d08, "ax"
	.global Func_08044c80
	.thumb_func
	.type Func_08044c80, %function
Func_08044c80:
	.incbin "baserom.gba", 0x00044d08, 0x00000148
	.section .rom.00044e50, "ax"
	.global Func_08044dc8
	.thumb_func
	.type Func_08044dc8, %function
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e50, 0x00000184
	.section .rom.00045010, "ax"
	.incbin "baserom.gba", 0x00045010, 0x000000c0
	.section .rom.000450d0, "ax"
	.global RenderResource_LoadFrame
	.thumb_func
	.type RenderResource_LoadFrame, %function
RenderResource_LoadFrame:
	.incbin "baserom.gba", 0x000450d0, 0x00000064
	.section .rom.00045184, "ax"
	.incbin "baserom.gba", 0x00045184, 0x00000150
	.section .rom.00045342, "ax"
	.incbin "baserom.gba", 0x00045342, 0x00000076
	.section .rom.000453b8, "ax"
	.global Func_08045330
	.thumb_func
	.type Func_08045330, %function
Func_08045330:
	.incbin "baserom.gba", 0x000453b8, 0x000000a0
	.section .rom.00045458, "ax"
	.global Func_080453d0
	.thumb_func
	.type Func_080453d0, %function
Func_080453d0:
	.incbin "baserom.gba", 0x00045458, 0x00000094
	.section .rom.000455ae, "ax"
	.incbin "baserom.gba", 0x000455ae, 0x0000002a
	.section .rom.000455ec, "ax"
	.incbin "baserom.gba", 0x000455ec, 0x0000004c
	.section .rom.00045664, "ax"
	.incbin "baserom.gba", 0x00045664, 0x0000018c
	.section .rom.00045806, "ax"
	.incbin "baserom.gba", 0x00045806, 0x00000032
	.section .rom.00045856, "ax"
	.incbin "baserom.gba", 0x00045856, 0x00000966
	.section .rom.000461bc, "ax"
	.global Func_08046134
	.thumb_func
	.type Func_08046134, %function
Func_08046134:
	.incbin "baserom.gba", 0x000461bc, 0x00000094
	.section .rom.00046250, "ax"
	.global BattleLayout_HighlightPartyPanels
	.thumb_func
	.type BattleLayout_HighlightPartyPanels, %function
BattleLayout_HighlightPartyPanels:
	.incbin "baserom.gba", 0x00046250, 0x00000098
	.section .rom.0004630c, "ax"
	.incbin "baserom.gba", 0x0004630c, 0x00000150
	.section .rom.0004649a, "ax"
	.incbin "baserom.gba", 0x0004649a, 0x0000365e
	.section .rom.00049b46, "ax"
	.incbin "baserom.gba", 0x00049b46, 0x00001f5e
	.section .rom.0004bade, "ax"
	.incbin "baserom.gba", 0x0004bade, 0x000002f2
	.section .rom.0004bdf8, "ax"
	.incbin "baserom.gba", 0x0004bdf8, 0x00000038
	.section .rom.0004be30, "ax"
	.global Func_0804bc08
	.thumb_func
	.type Func_0804bc08, %function
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be30, 0x000014d4
	.section .rom.0004d304, "ax"
	.global AffineEffect_InitializeWork
	.thumb_func
	.type AffineEffect_InitializeWork, %function
AffineEffect_InitializeWork:
	.incbin "baserom.gba", 0x0004d304, 0x0000003c
	.section .rom.0004d340, "ax"
	.global Menu_EndResourceSelection
	.thumb_func
	.type Menu_EndResourceSelection, %function
Menu_EndResourceSelection:
	.incbin "baserom.gba", 0x0004d340, 0x00000054
	.section .rom.0004d394, "ax"
	.global Menu_RunResourceSelectionLoop
	.thumb_func
	.type Menu_RunResourceSelectionLoop, %function
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d394, 0x000001d8
	.section .rom.0004d798, "ax"
	.global Menu_AnimateSelectionToEntry
	.thumb_func
	.type Menu_AnimateSelectionToEntry, %function
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x0004d798, 0x0000003c
	.section .rom.0004d7d4, "ax"
	.global Menu_SelectSaveSlotAction
	.thumb_func
	.type Menu_SelectSaveSlotAction, %function
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7d4, 0x0000023c
	.section .rom.0004da10, "ax"
	.global Func_0804d7fc
	.thumb_func
	.type Func_0804d7fc, %function
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da10, 0x0000017c
	.section .rom.0004dcde, "ax"
	.incbin "baserom.gba", 0x0004dcde, 0x000000ba
	.section .rom.0004dd98, "ax"
	.global Func_0804db74
	.thumb_func
	.type Func_0804db74, %function
Func_0804db74:
	.incbin "baserom.gba", 0x0004dd98, 0x00000254
	.section .rom.0004e0c4, "ax"
	.global Menu_DrawFlagBitTable
	.thumb_func
	.type Menu_DrawFlagBitTable, %function
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0c4, 0x000000c4
	.section .rom.0004e188, "ax"
	.global Menu_HandleFlagGridInput
	.thumb_func
	.type Menu_HandleFlagGridInput, %function
Menu_HandleFlagGridInput:
	.incbin "baserom.gba", 0x0004e188, 0x00000140
	.section .rom.0004e2f2, "ax"
	.incbin "baserom.gba", 0x0004e2f2, 0x00000002
	.section .rom.0004e2f4, "ax"
	.global Func_0804e0d0
	.thumb_func
	.type Func_0804e0d0, %function
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2f4, 0x00000108
	.section .rom.0004e3fc, "ax"
	.global Func_0804e1d8
	.thumb_func
	.type Func_0804e1d8, %function
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3fc, 0x0000021c
	.section .rom.0004e618, "ax"
	.global Func_0804e3f4
	.thumb_func
	.type Func_0804e3f4, %function
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e618, 0x00000764
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004ed7c, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f348, 0x000058f0
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x00054c38, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00055048, 0x00004b50
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b98, 0x00000c50
	.section .rom.0005c3e8, "ax"
	.incbin "baserom.gba", 0x0005c3e8, 0x0000357c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f964, 0x00000178
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fadc, 0x00000106
	.global Menu_TopEntryCommandByPosition
	.thumb_func
	.type Menu_TopEntryCommandByPosition, %function
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fbe2, 0x0000000c
	.global Menu_TopEntryPositionByCommand
	.thumb_func
	.type Menu_TopEntryPositionByCommand, %function
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fbee, 0x00000062
	.section .rom.000a8df0, "ax"
	.incbin "baserom.gba", 0x000a8df0, 0x00000300
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a90f0, 0x00008f10
	.section .rom.000b2070, "ax"
	.global Inventory_GetEquippedItemFar
	.thumb_func
	.type Inventory_GetEquippedItemFar, %function
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000b2070, 0x00000008
	.section .rom.000b2078, "ax"
	.global BattleAction_Get
	.thumb_func
	.type BattleAction_Get, %function
BattleAction_Get:
	.incbin "baserom.gba", 0x000b2078, 0x00000008
	.section .rom.000b2080, "ax"
	.global OwnerAction_AddFar
	.thumb_func
	.type OwnerAction_AddFar, %function
OwnerAction_AddFar:
	.incbin "baserom.gba", 0x000b2080, 0x00000008
	.section .rom.000b2088, "ax"
	.global Equipment_HasValueFar
	.thumb_func
	.type Equipment_HasValueFar, %function
Equipment_HasValueFar:
	.incbin "baserom.gba", 0x000b2088, 0x00000008
	.global Func_080ad090
	.thumb_func
	.type Func_080ad090, %function
Func_080ad090:
	.incbin "baserom.gba", 0x000b2090, 0x00000008
	.global Func_080ad098
	.thumb_func
	.type Func_080ad098, %function
Func_080ad098:
	.incbin "baserom.gba", 0x000b2098, 0x00000008
	.global Func_080ad0a0
	.thumb_func
	.type Func_080ad0a0, %function
Func_080ad0a0:
	.incbin "baserom.gba", 0x000b20a0, 0x00000008
	.global Func_080ad0a8
	.thumb_func
	.type Func_080ad0a8, %function
Func_080ad0a8:
	.incbin "baserom.gba", 0x000b20a8, 0x00000008
	.section .rom.000b20c0, "ax"
	.global Owner_AdjustFirstValueFar
	.thumb_func
	.type Owner_AdjustFirstValueFar, %function
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000b20c0, 0x00000008
	.global Owner_AdjustSecondValueFar
	.thumb_func
	.type Owner_AdjustSecondValueFar, %function
Owner_AdjustSecondValueFar:
	.incbin "baserom.gba", 0x000b20c8, 0x00000008
	.global Owner_RecalculateRatiosFar
	.thumb_func
	.type Owner_RecalculateRatiosFar, %function
Owner_RecalculateRatiosFar:
	.incbin "baserom.gba", 0x000b20d0, 0x00000008
	.section .rom.000b20d8, "ax"
	.global Owner_UpdateRatioPairFar
	.thumb_func
	.type Owner_UpdateRatioPairFar, %function
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000b20d8, 0x00000008
	.global Owner_UpdateSecondInputAndRatiosFar
	.thumb_func
	.type Owner_UpdateSecondInputAndRatiosFar, %function
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000b20e0, 0x00000008
	.global BattleUnit_AssignFar
	.thumb_func
	.type BattleUnit_AssignFar, %function
BattleUnit_AssignFar:
	.incbin "baserom.gba", 0x000b20e8, 0x00000008
	.section .rom.000b20f0, "ax"
	.global Party_CountActiveOwnersFar
	.thumb_func
	.type Party_CountActiveOwnersFar, %function
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000b20f0, 0x00000008
	.section .rom.000b20f8, "ax"
	.global Party_AddActiveOwnerFar
	.thumb_func
	.type Party_AddActiveOwnerFar, %function
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000b20f8, 0x00000008
	.global Party_ListActiveOwnersFar
	.thumb_func
	.type Party_ListActiveOwnersFar, %function
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000b2100, 0x00000008
	.global Func_080ad108
	.thumb_func
	.type Func_080ad108, %function
Func_080ad108:
	.incbin "baserom.gba", 0x000b2108, 0x00000008
	.global Party_RemoveActiveOwnerFar
	.thumb_func
	.type Party_RemoveActiveOwnerFar, %function
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000b2110, 0x00000008
	.global Func_080ad118
	.thumb_func
	.type Func_080ad118, %function
Func_080ad118:
	.incbin "baserom.gba", 0x000b2118, 0x00000008
	.global Battle_HitCheck
	.thumb_func
	.type Battle_HitCheck, %function
Battle_HitCheck:
	.incbin "baserom.gba", 0x000b2120, 0x00000008
	.global Battle_CalcAttack
	.thumb_func
	.type Battle_CalcAttack, %function
Battle_CalcAttack:
	.incbin "baserom.gba", 0x000b2128, 0x00000008
	.global Battle_CalcPower
	.thumb_func
	.type Battle_CalcPower, %function
Battle_CalcPower:
	.incbin "baserom.gba", 0x000b2130, 0x00000008
	.global Battle_CalcRestore
	.thumb_func
	.type Battle_CalcRestore, %function
Battle_CalcRestore:
	.incbin "baserom.gba", 0x000b2138, 0x00000008
	.global Owner_GetRecordFar
	.thumb_func
	.type Owner_GetRecordFar, %function
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000b2140, 0x00000008
	.section .rom.000b2148, "ax"
	.global BattleRandom16Far
	.thumb_func
	.type BattleRandom16Far, %function
BattleRandom16Far:
	.incbin "baserom.gba", 0x000b2148, 0x00000008
	.section .rom.000b2150, "ax"
	.global Djinn_AddToOwnerFar
	.thumb_func
	.type Djinn_AddToOwnerFar, %function
Djinn_AddToOwnerFar:
	.incbin "baserom.gba", 0x000b2150, 0x00000008
	.section .rom.000b2158, "ax"
	.global Djinn_ActivateFar
	.thumb_func
	.type Djinn_ActivateFar, %function
Djinn_ActivateFar:
	.incbin "baserom.gba", 0x000b2158, 0x00000008
	.global Djinn_DeactivateFar
	.thumb_func
	.type Djinn_DeactivateFar, %function
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000b2160, 0x00000008
	.section .rom.000b2168, "ax"
	.global Trade_RemoveOfferFar
	.thumb_func
	.type Trade_RemoveOfferFar, %function
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b2168, 0x00000008
	.global Trade_AddOfferFar
	.thumb_func
	.type Trade_AddOfferFar, %function
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000b2170, 0x00000008
	.global Djinn_TransferFar
	.thumb_func
	.type Djinn_TransferFar, %function
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000b2178, 0x00000008
	.global Func_080ad180
	.thumb_func
	.type Func_080ad180, %function
Func_080ad180:
	.incbin "baserom.gba", 0x000b2180, 0x00000008
	.global SummonDefinition_Get
	.thumb_func
	.type SummonDefinition_Get, %function
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000b2188, 0x00000008
	.global Djinn_GetDefinitionHeaderFar
	.thumb_func
	.type Djinn_GetDefinitionHeaderFar, %function
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000b2190, 0x00000008
	.global Party_AdvanceOwnerCountToTargetFar
	.thumb_func
	.type Party_AdvanceOwnerCountToTargetFar, %function
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000b2198, 0x00000008
	.global Func_080ad1a0
	.thumb_func
	.type Func_080ad1a0, %function
Func_080ad1a0:
	.incbin "baserom.gba", 0x000b21a0, 0x00000008
	.global Trade_CountPendingOffersFar
	.thumb_func
	.type Trade_CountPendingOffersFar, %function
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000b21a8, 0x00000008
	.global Djinn_IsActiveFar
	.thumb_func
	.type Djinn_IsActiveFar, %function
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000b21b0, 0x00000008
	.global Trade_CanOfferDjinnFar
	.thumb_func
	.type Trade_CanOfferDjinnFar, %function
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000b21b8, 0x00000008
	.section .rom.000b21c0, "ax"
	.global Item_CanOwnerEquip
	.thumb_func
	.type Item_CanOwnerEquip, %function
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b21c0, 0x00000008
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
	.type Item_IsCompatibleWithOwnerFar, %function
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000b21c8, 0x00000008
	.global Inventory_FindEquippedFar
	.thumb_func
	.type Inventory_FindEquippedFar, %function
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000b21d0, 0x00000008
	.global Party_AdjustSixDigitCounterAFar
	.thumb_func
	.type Party_AdjustSixDigitCounterAFar, %function
Party_AdjustSixDigitCounterAFar:
	.incbin "baserom.gba", 0x000b21d8, 0x00000008
	.global Item_GetEquipmentGroupFar
	.thumb_func
	.type Item_GetEquipmentGroupFar, %function
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000b21e0, 0x00000008
	.global Item_AdjustCounterFar
	.thumb_func
	.type Item_AdjustCounterFar, %function
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000b21e8, 0x00000008
	.global Inventory_CountFar
	.thumb_func
	.type Inventory_CountFar, %function
Inventory_CountFar:
	.incbin "baserom.gba", 0x000b21f0, 0x00000008
	.global PartyInventory_HasSpaceFar
	.thumb_func
	.type PartyInventory_HasSpaceFar, %function
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000b21f8, 0x00000008
	.global Owner_GetLevelThresholdFar
	.thumb_func
	.type Owner_GetLevelThresholdFar, %function
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000b2200, 0x00000008
	.global Func_080ad208
	.thumb_func
	.type Func_080ad208, %function
Func_080ad208:
	.incbin "baserom.gba", 0x000b2208, 0x00000008
	.global Func_080ad210
	.thumb_func
	.type Func_080ad210, %function
Func_080ad210:
	.incbin "baserom.gba", 0x000b2210, 0x00000008
	.global Func_080ad218
	.thumb_func
	.type Func_080ad218, %function
Func_080ad218:
	.incbin "baserom.gba", 0x000b2218, 0x00000008
	.global Func_080ad220
	.thumb_func
	.type Func_080ad220, %function
Func_080ad220:
	.incbin "baserom.gba", 0x000b2220, 0x00000008
	.global Func_080ad228
	.thumb_func
	.type Func_080ad228, %function
Func_080ad228:
	.incbin "baserom.gba", 0x000b2228, 0x00000008
	.global Func_080ad230
	.thumb_func
	.type Func_080ad230, %function
Func_080ad230:
	.incbin "baserom.gba", 0x000b2230, 0x00000008
	.global Func_080ad238
	.thumb_func
	.type Func_080ad238, %function
Func_080ad238:
	.incbin "baserom.gba", 0x000b2238, 0x00000008
	.global Func_080ad240
	.thumb_func
	.type Func_080ad240, %function
Func_080ad240:
	.incbin "baserom.gba", 0x000b2240, 0x00000008
	.global Djinn_AddToLeastLoadedOwnerFar
	.thumb_func
	.type Djinn_AddToLeastLoadedOwnerFar, %function
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000b2248, 0x00000008
	.global Party_SumDjinnCountsFar
	.thumb_func
	.type Party_SumDjinnCountsFar, %function
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000b2250, 0x00000008
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
	.type Party_AdjustSixDigitCounterBFar, %function
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000b2258, 0x00000008
	.global Party_AdjustCounterCappedAt28Far
	.thumb_func
	.type Party_AdjustCounterCappedAt28Far, %function
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000b2260, 0x00000008
	.global Func_080ad268
	.thumb_func
	.type Func_080ad268, %function
Func_080ad268:
	.incbin "baserom.gba", 0x000b2268, 0x00000008
	.global Func_080ad270
	.thumb_func
	.type Func_080ad270, %function
Func_080ad270:
	.incbin "baserom.gba", 0x000b2270, 0x00000008
	.global Inventory_DiscardFar
	.thumb_func
	.type Inventory_DiscardFar, %function
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000b2278, 0x00000008
	.section .rom.000b2280, "ax"
	.global BattleFx_IsReviveFar
	.thumb_func
	.type BattleFx_IsReviveFar, %function
BattleFx_IsReviveFar:
	.incbin "baserom.gba", 0x000b2280, 0x00000008
	.global Owner_RefreshClassActionsFar
	.thumb_func
	.type Owner_RefreshClassActionsFar, %function
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000b2288, 0x00000008
	.global Func_080ad290
	.thumb_func
	.type Func_080ad290, %function
Func_080ad290:
	.incbin "baserom.gba", 0x000b2290, 0x00000008
	.global Owner_RefreshDerivedDataFar
	.thumb_func
	.type Owner_RefreshDerivedDataFar, %function
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000b2298, 0x00000008
	.global Inventory_CountItemFar
	.thumb_func
	.type Inventory_CountItemFar, %function
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000b22a0, 0x00000008
	.global PartyInventory_CountItemFar
	.thumb_func
	.type PartyInventory_CountItemFar, %function
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000b22a8, 0x00000008
	.global Func_080ad2b0
	.thumb_func
	.type Func_080ad2b0, %function
Func_080ad2b0:
	.incbin "baserom.gba", 0x000b22b0, 0x00000008
	.section .rom.000b2338, "ax"
	.global Func_080ad338
	.thumb_func
	.type Func_080ad338, %function
Func_080ad338:
	.incbin "baserom.gba", 0x000b2338, 0x00000010
	.section .rom.000b23f6, "ax"
	.incbin "baserom.gba", 0x000b23f6, 0x00000002
	.section .rom.000b23f8, "ax"
	.global Owner_RecalculateStats
	.thumb_func
	.type Owner_RecalculateStats, %function
Owner_RecalculateStats:
	.incbin "baserom.gba", 0x000b23f8, 0x000007f4
	.global GameFlag_RefreshLureCap
	.thumb_func
	.type GameFlag_RefreshLureCap, %function
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000b2bec, 0x000000a4
	.global Func_080adc90
	.thumb_func
	.type Func_080adc90, %function
Func_080adc90:
	.incbin "baserom.gba", 0x000b2c90, 0x000000cc
	.global Func_080add5c
	.thumb_func
	.type Func_080add5c, %function
Func_080add5c:
	.incbin "baserom.gba", 0x000b2d5c, 0x00000018
	.global Func_080add74
	.thumb_func
	.type Func_080add74, %function
Func_080add74:
	.incbin "baserom.gba", 0x000b2d74, 0x0000007c
	.global Func_080addf0
	.thumb_func
	.type Func_080addf0, %function
Func_080addf0:
	.incbin "baserom.gba", 0x000b2df0, 0x000002ec
	.global Func_080ae0dc
	.thumb_func
	.type Func_080ae0dc, %function
Func_080ae0dc:
	.incbin "baserom.gba", 0x000b30dc, 0x00000014
	.global Func_080ae0f0
	.thumb_func
	.type Func_080ae0f0, %function
Func_080ae0f0:
	.incbin "baserom.gba", 0x000b30f0, 0x00000028
	.global Func_080ae118
	.thumb_func
	.type Func_080ae118, %function
Func_080ae118:
	.incbin "baserom.gba", 0x000b3118, 0x00000054
	.global Func_080ae16c
	.thumb_func
	.type Func_080ae16c, %function
Func_080ae16c:
	.incbin "baserom.gba", 0x000b316c, 0x000000b4
	.global Func_080ae220
	.thumb_func
	.type Func_080ae220, %function
Func_080ae220:
	.incbin "baserom.gba", 0x000b3220, 0x000001f0
	.global Func_080ae410
	.thumb_func
	.type Func_080ae410, %function
Func_080ae410:
	.incbin "baserom.gba", 0x000b3410, 0x000001ec
	.global Func_080ae5fc
	.thumb_func
	.type Func_080ae5fc, %function
Func_080ae5fc:
	.incbin "baserom.gba", 0x000b35fc, 0x000000e8
	.global Func_080ae6e4
	.thumb_func
	.type Func_080ae6e4, %function
Func_080ae6e4:
	.incbin "baserom.gba", 0x000b36e4, 0x0000014c
	.global Func_080ae830
	.thumb_func
	.type Func_080ae830, %function
Func_080ae830:
	.incbin "baserom.gba", 0x000b3830, 0x00000004
	.section .rom.000b3868, "ax"
	.global Func_080ae868
	.thumb_func
	.type Func_080ae868, %function
Func_080ae868:
	.incbin "baserom.gba", 0x000b3868, 0x000001c8
	.section .rom.000b3c66, "ax"
	.incbin "baserom.gba", 0x000b3c66, 0x00000002
	.section .rom.000b3c68, "ax"
	.global Item_GetEquipmentGroup
	.thumb_func
	.type Item_GetEquipmentGroup, %function
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b3c68, 0x0000003c
	.global Inventory_GetQuantity
	.thumb_func
	.type Inventory_GetQuantity, %function
Inventory_GetQuantity:
	.incbin "baserom.gba", 0x000b3ca4, 0x00000024
	.section .rom.000b3e40, "ax"
	.global Func_080aee40
	.thumb_func
	.type Func_080aee40, %function
Func_080aee40:
	.incbin "baserom.gba", 0x000b3e40, 0x00000058
	.section .rom.000b3e98, "ax"
	.global Inventory_Find
	.thumb_func
	.type Inventory_Find, %function
Inventory_Find:
	.incbin "baserom.gba", 0x000b3e98, 0x00000030
	.section .rom.000b3f34, "ax"
	.global Inventory_Equip
	.thumb_func
	.type Inventory_Equip, %function
Inventory_Equip:
	.incbin "baserom.gba", 0x000b3f34, 0x000000d4
	.section .rom.000b40e2, "ax"
	.incbin "baserom.gba", 0x000b40e2, 0x00000002
	.global Func_080af0e4
	.thumb_func
	.type Func_080af0e4, %function
Func_080af0e4:
	.incbin "baserom.gba", 0x000b40e4, 0x00000064
	.section .rom.000b4148, "ax"
	.global Inventory_Remove
	.thumb_func
	.type Inventory_Remove, %function
Inventory_Remove:
	.incbin "baserom.gba", 0x000b4148, 0x00000080
	.section .rom.000b41fa, "ax"
	.incbin "baserom.gba", 0x000b41fa, 0x0000009e
	.section .rom.000b4338, "ax"
	.global Item_GetTargetMode
	.thumb_func
	.type Item_GetTargetMode, %function
Item_GetTargetMode:
	.incbin "baserom.gba", 0x000b4338, 0x00000040
	.section .rom.000b4378, "ax"
	.global Item_AdjustCounter
	.thumb_func
	.type Item_AdjustCounter, %function
Item_AdjustCounter:
	.incbin "baserom.gba", 0x000b4378, 0x00000028
	.section .rom.000b443c, "ax"
	.global BattleAction_GetDirect
	.thumb_func
	.type BattleAction_GetDirect, %function
BattleAction_GetDirect:
	.incbin "baserom.gba", 0x000b443c, 0x00000028
	.global Func_080af464
	.thumb_func
	.type Func_080af464, %function
Func_080af464:
	.incbin "baserom.gba", 0x000b4464, 0x00000290
	.section .rom.000b4792, "ax"
	.incbin "baserom.gba", 0x000b4792, 0x0000000a
	.section .rom.000b47ac, "ax"
	.incbin "baserom.gba", 0x000b47ac, 0x00000124
	.global Owner_GetLevelThreshold
	.thumb_func
	.type Owner_GetLevelThreshold, %function
Owner_GetLevelThreshold:
	.incbin "baserom.gba", 0x000b48d0, 0x0000004c
	.section .rom.000b491c, "ax"
	.global Owner_LevelUp
	.thumb_func
	.type Owner_LevelUp, %function
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b491c, 0x00000264
	.global Func_080afb80
	.thumb_func
	.type Func_080afb80, %function
Func_080afb80:
	.incbin "baserom.gba", 0x000b4b80, 0x00000034
	.section .rom.000b4bec, "ax"
	.incbin "baserom.gba", 0x000b4bec, 0x000001d0
	.section .rom.000b4dd8, "ax"
	.incbin "baserom.gba", 0x000b4dd8, 0x00000044
	.section .rom.000b4f28, "ax"
	.incbin "baserom.gba", 0x000b4f28, 0x00000054
	.section .rom.000b4f94, "ax"
	.global Func_080aff94
	.thumb_func
	.type Func_080aff94, %function
Func_080aff94:
	.incbin "baserom.gba", 0x000b4f94, 0x00000018
	.section .rom.000b4fac, "ax"
	.global Owner_GetDigitValues
	.thumb_func
	.type Owner_GetDigitValues, %function
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000b4fac, 0x0000007c
	.section .rom.000b505a, "ax"
	.incbin "baserom.gba", 0x000b505a, 0x00000002
	.global Owner_GetDefaultElement
	.thumb_func
	.type Owner_GetDefaultElement, %function
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000b505c, 0x0000022c
	.section .rom.000b5298, "ax"
	.incbin "baserom.gba", 0x000b5298, 0x0000003c
	.global Owner_RefreshClassActions
	.thumb_func
	.type Owner_RefreshClassActions, %function
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x000b52d4, 0x00000008
	.global Curve_LookupScaledValue
	.thumb_func
	.type Curve_LookupScaledValue, %function
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x000b52dc, 0x0000009c
	.section .rom.000b54ba, "ax"
	.incbin "baserom.gba", 0x000b54ba, 0x000005da
	.section .rom.000b5bb6, "ax"
	.incbin "baserom.gba", 0x000b5bb6, 0x000000c2
	.section .rom.000b5c9a, "ax"
	.incbin "baserom.gba", 0x000b5c9a, 0x00000002
	.section .rom.000b5c9c, "ax"
	.global Djinn_Activate
	.thumb_func
	.type Djinn_Activate, %function
Djinn_Activate:
	.incbin "baserom.gba", 0x000b5c9c, 0x00000068
	.section .rom.000b5d04, "ax"
	.global Djinn_Deactivate
	.thumb_func
	.type Djinn_Deactivate, %function
Djinn_Deactivate:
	.incbin "baserom.gba", 0x000b5d04, 0x00000054
	.section .rom.000b5d58, "ax"
	.global Trade_RemoveOffer
	.thumb_func
	.type Trade_RemoveOffer, %function
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b5d58, 0x000000ac
	.section .rom.000b6002, "ax"
	.incbin "baserom.gba", 0x000b6002, 0x00000002
	.global Func_080b1004
	.thumb_func
	.type Func_080b1004, %function
Func_080b1004:
	.incbin "baserom.gba", 0x000b6004, 0x00000084
	.global Func_080b1088
	.thumb_func
	.type Func_080b1088, %function
Func_080b1088:
	.incbin "baserom.gba", 0x000b6088, 0x00000170
	.global Func_080b11f8
	.thumb_func
	.type Func_080b11f8, %function
Func_080b11f8:
	.incbin "baserom.gba", 0x000b61f8, 0x000000d0
	.global Owner_ExperienceThresholds
Owner_ExperienceThresholds:
	.incbin "baserom.gba", 0x000b62c8, 0x0000109c
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000b7364, 0x000058b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x000bcc14, 0x00002268
	.global Enemy_DefinitionTable
Enemy_DefinitionTable:
	.incbin "baserom.gba", 0x000bee7c, 0x000070d0
	.global Owner_GrowthRecords
Owner_GrowthRecords:
	.incbin "baserom.gba", 0x000c5f4c, 0x000005c0
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
	.global Func_080c85c0
	.thumb_func
	.type Func_080c85c0, %function
Func_080c85c0:
	.incbin "baserom.gba", 0x000cd5c0, 0x00000008
	.section .rom.000cd9cc, "ax"
	.incbin "baserom.gba", 0x000cd9cc, 0x00000a00
	.section .rom.000ce694, "ax"
	.incbin "baserom.gba", 0x000ce694, 0x0000028c
	.section .rom.000ce934, "ax"
	.incbin "baserom.gba", 0x000ce934, 0x00000010
	.section .rom.000ce944, "ax"
	.global Game_ResetForNewGame
	.thumb_func
	.type Game_ResetForNewGame, %function
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000ce944, 0x000002cc
	.section .rom.000cec38, "ax"
	.incbin "baserom.gba", 0x000cec38, 0x00000190
	.section .rom.000cedc8, "ax"
	.global Func_080c9dc8
	.thumb_func
	.type Func_080c9dc8, %function
Func_080c9dc8:
	.incbin "baserom.gba", 0x000cedc8, 0x00000164
	.section .rom.000cef2c, "ax"
	.global BattleFx_LookupResult
	.thumb_func
	.type BattleFx_LookupResult, %function
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000cef2c, 0x000000ac
	.section .rom.000cefd8, "ax"
	.global Encounter_SelectEnemyGroup
	.thumb_func
	.type Encounter_SelectEnemyGroup, %function
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000cefd8, 0x0000018c
	.section .rom.000cf1bc, "ax"
	.global Func_080ca1bc
	.thumb_func
	.type Func_080ca1bc, %function
Func_080ca1bc:
	.incbin "baserom.gba", 0x000cf1bc, 0x000000c4
	.section .rom.000cf280, "ax"
	.global Func_080ca280
	.thumb_func
	.type Func_080ca280, %function
Func_080ca280:
	.incbin "baserom.gba", 0x000cf280, 0x000000e8
	.section .rom.000cf368, "ax"
	.global Func_080ca368
	.thumb_func
	.type Func_080ca368, %function
Func_080ca368:
	.incbin "baserom.gba", 0x000cf368, 0x0000008c
	.section .rom.000cf3f4, "ax"
	.global Func_080ca3f4
	.thumb_func
	.type Func_080ca3f4, %function
Func_080ca3f4:
	.incbin "baserom.gba", 0x000cf3f4, 0x000000a0
	.section .rom.000cf4a8, "ax"
	.incbin "baserom.gba", 0x000cf4a8, 0x000001fc
	.section .rom.000cf6a4, "ax"
	.global Func_080ca6a4
	.thumb_func
	.type Func_080ca6a4, %function
Func_080ca6a4:
	.incbin "baserom.gba", 0x000cf6a4, 0x00000040
	.section .rom.000cf6e4, "ax"
	.global Func_080ca6e4
	.thumb_func
	.type Func_080ca6e4, %function
Func_080ca6e4:
	.incbin "baserom.gba", 0x000cf6e4, 0x00000004
	.section .rom.000cf6e8, "ax"
	.global Event_SpawnObjectTable
	.thumb_func
	.type Event_SpawnObjectTable, %function
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000cf6e8, 0x000002e4
	.section .rom.000cf9cc, "ax"
	.global Func_080ca9cc
	.thumb_func
	.type Func_080ca9cc, %function
Func_080ca9cc:
	.incbin "baserom.gba", 0x000cf9cc, 0x00000060
	.section .rom.000cfa2c, "ax"
	.global Func_080caa2c
	.thumb_func
	.type Func_080caa2c, %function
Func_080caa2c:
	.incbin "baserom.gba", 0x000cfa2c, 0x00000020
	.section .rom.000cfa4c, "ax"
	.global Func_080caa4c
	.thumb_func
	.type Func_080caa4c, %function
Func_080caa4c:
	.incbin "baserom.gba", 0x000cfa4c, 0x00000274
	.section .rom.000cfcc0, "ax"
	.global ObjectTable_FindLastActiveId
	.thumb_func
	.type ObjectTable_FindLastActiveId, %function
ObjectTable_FindLastActiveId:
	.incbin "baserom.gba", 0x000cfcc0, 0x0000002c
	.section .rom.000cfcec, "ax"
	.global Scene_AssignViewFlags
	.thumb_func
	.type Scene_AssignViewFlags, %function
Scene_AssignViewFlags:
	.incbin "baserom.gba", 0x000cfcec, 0x00000098
	.section .rom.000cfd84, "ax"
	.global ObjectTable_Get
	.thumb_func
	.type ObjectTable_Get, %function
ObjectTable_Get:
	.incbin "baserom.gba", 0x000cfd84, 0x00000018
	.section .rom.000cfd9c, "ax"
	.global Func_080cad9c
	.thumb_func
	.type Func_080cad9c, %function
Func_080cad9c:
	.incbin "baserom.gba", 0x000cfd9c, 0x000000c0
	.section .rom.000cfe5c, "ax"
	.global Func_080cae5c
	.thumb_func
	.type Func_080cae5c, %function
Func_080cae5c:
	.incbin "baserom.gba", 0x000cfe5c, 0x00000114
	.section .rom.000cffc4, "ax"
	.incbin "baserom.gba", 0x000cffc4, 0x00000704
	.section .rom.000d082c, "ax"
	.global Func_080cb82c
	.thumb_func
	.type Func_080cb82c, %function
Func_080cb82c:
	.incbin "baserom.gba", 0x000d082c, 0x00000078
	.section .rom.000d08a4, "ax"
	.global Func_080cb8a4
	.thumb_func
	.type Func_080cb8a4, %function
Func_080cb8a4:
	.incbin "baserom.gba", 0x000d08a4, 0x00000ca8
	.section .rom.000d154c, "ax"
	.global Func_080cc54c
	.thumb_func
	.type Func_080cc54c, %function
Func_080cc54c:
	.incbin "baserom.gba", 0x000d154c, 0x0000076c
	.global BattleAction_FindDescriptor
	.thumb_func
	.type BattleAction_FindDescriptor, %function
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000d1cb8, 0x00000090
	.section .rom.000d1d76, "ax"
	.incbin "baserom.gba", 0x000d1d76, 0x00000002
	.section .rom.000d1d78, "ax"
	.global Func_080ccd78
	.thumb_func
	.type Func_080ccd78, %function
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1d78, 0x00000150
	.section .rom.000d1ec8, "ax"
	.global Func_080ccec8
	.thumb_func
	.type Func_080ccec8, %function
Func_080ccec8:
	.incbin "baserom.gba", 0x000d1ec8, 0x00000a54
	.section .rom.000d291c, "ax"
	.global Func_080cd91c
	.thumb_func
	.type Func_080cd91c, %function
Func_080cd91c:
	.incbin "baserom.gba", 0x000d291c, 0x000002dc
	.section .rom.000d2bf8, "ax"
	.global Func_080cdbf8
	.thumb_func
	.type Func_080cdbf8, %function
Func_080cdbf8:
	.incbin "baserom.gba", 0x000d2bf8, 0x00000048
	.section .rom.000d2c72, "ax"
	.incbin "baserom.gba", 0x000d2c72, 0x00000002
	.section .rom.000d2c74, "ax"
	.global Func_080cdc74
	.thumb_func
	.type Func_080cdc74, %function
Func_080cdc74:
	.incbin "baserom.gba", 0x000d2c74, 0x0000010c
	.section .rom.000d2d80, "ax"
	.global Func_080cdd80
	.thumb_func
	.type Func_080cdd80, %function
Func_080cdd80:
	.incbin "baserom.gba", 0x000d2d80, 0x00000128
	.section .rom.000d2f80, "ax"
	.incbin "baserom.gba", 0x000d2f80, 0x00000030
	.section .rom.000d2fb0, "ax"
	.global Event_FindFacingTrigger
	.thumb_func
	.type Event_FindFacingTrigger, %function
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d2fb0, 0x000005c4
	.global EventRuntime_ExecutePackedAction
	.thumb_func
	.type EventRuntime_ExecutePackedAction, %function
EventRuntime_ExecutePackedAction:
	.incbin "baserom.gba", 0x000d3574, 0x000005e4
	.section .rom.000d3b58, "ax"
	.global Func_080ceb58
	.thumb_func
	.type Func_080ceb58, %function
Func_080ceb58:
	.incbin "baserom.gba", 0x000d3b58, 0x00000028
	.section .rom.000d3ba8, "ax"
	.incbin "baserom.gba", 0x000d3ba8, 0x00000298
	.section .rom.000d3e80, "ax"
	.incbin "baserom.gba", 0x000d3e80, 0x00000048
	.section .rom.000d3ec8, "ax"
	.global Func_080ceec8
	.thumb_func
	.type Func_080ceec8, %function
Func_080ceec8:
	.incbin "baserom.gba", 0x000d3ec8, 0x000000a0
	.section .rom.000d3f68, "ax"
	.global Func_080cef68
	.thumb_func
	.type Func_080cef68, %function
Func_080cef68:
	.incbin "baserom.gba", 0x000d3f68, 0x0000004c
	.section .rom.000d3fb4, "ax"
	.global Func_080cefb4
	.thumb_func
	.type Func_080cefb4, %function
Func_080cefb4:
	.incbin "baserom.gba", 0x000d3fb4, 0x0000001c
	.section .rom.000d3fd0, "ax"
	.global Func_080cefd0
	.thumb_func
	.type Func_080cefd0, %function
Func_080cefd0:
	.incbin "baserom.gba", 0x000d3fd0, 0x0000002c
	.section .rom.000d3ffc, "ax"
	.global Func_080ceffc
	.thumb_func
	.type Func_080ceffc, %function
Func_080ceffc:
	.incbin "baserom.gba", 0x000d3ffc, 0x00000008
	.section .rom.000d4004, "ax"
	.global Func_080cf004
	.thumb_func
	.type Func_080cf004, %function
Func_080cf004:
	.incbin "baserom.gba", 0x000d4004, 0x00000008
	.section .rom.000d400c, "ax"
	.global Func_080cf00c
	.thumb_func
	.type Func_080cf00c, %function
Func_080cf00c:
	.incbin "baserom.gba", 0x000d400c, 0x00000044
	.section .rom.000d4050, "ax"
	.global Func_080cf050
	.thumb_func
	.type Func_080cf050, %function
Func_080cf050:
	.incbin "baserom.gba", 0x000d4050, 0x00000080
	.section .rom.000d4158, "ax"
	.incbin "baserom.gba", 0x000d4158, 0x00000024
	.section .rom.000d417c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.thumb_func
	.type BattleFx_StartRandomParticleEmitter, %function
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d417c, 0x00000238
	.section .rom.000d43b4, "ax"
	.global Func_080cf3b4
	.thumb_func
	.type Func_080cf3b4, %function
Func_080cf3b4:
	.incbin "baserom.gba", 0x000d43b4, 0x00000070
	.section .rom.000d4424, "ax"
	.global Func_080cf424
	.thumb_func
	.type Func_080cf424, %function
Func_080cf424:
	.incbin "baserom.gba", 0x000d4424, 0x000000bc
	.section .rom.000d4554, "ax"
	.incbin "baserom.gba", 0x000d4554, 0x00000ba4
	.section .rom.000d50f8, "ax"
	.global Func_080d00f8
	.thumb_func
	.type Func_080d00f8, %function
Func_080d00f8:
	.incbin "baserom.gba", 0x000d50f8, 0x000000d4
	.section .rom.000d51cc, "ax"
	.global DisplayTransition_Start
	.thumb_func
	.type DisplayTransition_Start, %function
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d51cc, 0x00000354
	.section .rom.000d5520, "ax"
	.global DisplayTransition_Finish
	.thumb_func
	.type DisplayTransition_Finish, %function
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d5520, 0x0000020c
	.section .rom.000d572c, "ax"
	.global Func_080d072c
	.thumb_func
	.type Func_080d072c, %function
Func_080d072c:
	.incbin "baserom.gba", 0x000d572c, 0x0000001c
	.section .rom.000d5748, "ax"
	.global Func_080d0748
	.thumb_func
	.type Func_080d0748, %function
Func_080d0748:
	.incbin "baserom.gba", 0x000d5748, 0x000002e0
	.global DisplayTransition_UpdateFrame
	.thumb_func
	.type DisplayTransition_UpdateFrame, %function
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d5a28, 0x00000154
	.global DisplayTransition_FillTilemapAndSolidTile
	.thumb_func
	.type DisplayTransition_FillTilemapAndSolidTile, %function
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d5b7c, 0x00000070
	.section .rom.000d5ca0, "ax"
	.incbin "baserom.gba", 0x000d5ca0, 0x000009e4
	.section .rom.000d6684, "ax"
	.global Func_080d1684
	.thumb_func
	.type Func_080d1684, %function
Func_080d1684:
	.incbin "baserom.gba", 0x000d6684, 0x00000074
	.section .rom.000d66f8, "ax"
	.global Func_080d16f8
	.thumb_func
	.type Func_080d16f8, %function
Func_080d16f8:
	.incbin "baserom.gba", 0x000d66f8, 0x00000014
	.section .rom.000d670c, "ax"
	.global Func_080d170c
	.thumb_func
	.type Func_080d170c, %function
Func_080d170c:
	.incbin "baserom.gba", 0x000d670c, 0x00000020
	.section .rom.000d672c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.thumb_func
	.type BattleFx_ApplyColorToSourceBuffer, %function
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000d672c, 0x00000020
	.section .rom.000d674c, "ax"
	.global Func_080d174c
	.thumb_func
	.type Func_080d174c, %function
Func_080d174c:
	.incbin "baserom.gba", 0x000d674c, 0x00000014
	.section .rom.000d6760, "ax"
	.global Func_080d1760
	.thumb_func
	.type Func_080d1760, %function
Func_080d1760:
	.incbin "baserom.gba", 0x000d6760, 0x0000004c
	.section .rom.000d67ac, "ax"
	.global BattleFx_StartBufferInterpolation
	.thumb_func
	.type BattleFx_StartBufferInterpolation, %function
BattleFx_StartBufferInterpolation:
	.incbin "baserom.gba", 0x000d67ac, 0x0000003c
	.section .rom.000d67e8, "ax"
	.global Func_080d17e8
	.thumb_func
	.type Func_080d17e8, %function
Func_080d17e8:
	.incbin "baserom.gba", 0x000d67e8, 0x00000034
	.section .rom.000d683e, "ax"
	.incbin "baserom.gba", 0x000d683e, 0x000001da
	.section .rom.000d6a18, "ax"
	.global Func_080d1a18
	.thumb_func
	.type Func_080d1a18, %function
Func_080d1a18:
	.incbin "baserom.gba", 0x000d6a18, 0x000000ac
	.section .rom.000d6ac4, "ax"
	.global Func_080d1ac4
	.thumb_func
	.type Func_080d1ac4, %function
Func_080d1ac4:
	.incbin "baserom.gba", 0x000d6ac4, 0x00000010
	.section .rom.000d6ad4, "ax"
	.global Func_080d1ad4
	.thumb_func
	.type Func_080d1ad4, %function
Func_080d1ad4:
	.incbin "baserom.gba", 0x000d6ad4, 0x000001ec
	.section .rom.000d6cc0, "ax"
	.global Func_080d1cc0
	.thumb_func
	.type Func_080d1cc0, %function
Func_080d1cc0:
	.incbin "baserom.gba", 0x000d6cc0, 0x00000074
	.section .rom.000d6d34, "ax"
	.global Func_080d1d34
	.thumb_func
	.type Func_080d1d34, %function
Func_080d1d34:
	.incbin "baserom.gba", 0x000d6d34, 0x00000010
	.section .rom.000d6d44, "ax"
	.global Func_080d1d44
	.thumb_func
	.type Func_080d1d44, %function
Func_080d1d44:
	.incbin "baserom.gba", 0x000d6d44, 0x00000010
	.section .rom.000d6d54, "ax"
	.global Func_080d1d54
	.thumb_func
	.type Func_080d1d54, %function
Func_080d1d54:
	.incbin "baserom.gba", 0x000d6d54, 0x00000058
	.section .rom.000d6dac, "ax"
	.global Func_080d1dac
	.thumb_func
	.type Func_080d1dac, %function
Func_080d1dac:
	.incbin "baserom.gba", 0x000d6dac, 0x00000044
	.section .rom.000d6df0, "ax"
	.global Func_080d1df0
	.thumb_func
	.type Func_080d1df0, %function
Func_080d1df0:
	.incbin "baserom.gba", 0x000d6df0, 0x00000028
	.section .rom.000d6e18, "ax"
	.global Func_080d1e18
	.thumb_func
	.type Func_080d1e18, %function
Func_080d1e18:
	.incbin "baserom.gba", 0x000d6e18, 0x0000006c
	.section .rom.000d6eaa, "ax"
	.incbin "baserom.gba", 0x000d6eaa, 0x00000002
	.section .rom.000d6eac, "ax"
	.global Func_080d1eac
	.thumb_func
	.type Func_080d1eac, %function
Func_080d1eac:
	.incbin "baserom.gba", 0x000d6eac, 0x0000002c
	.section .rom.000d6ed8, "ax"
	.global BattleFx_GetFlags
	.thumb_func
	.type BattleFx_GetFlags, %function
BattleFx_GetFlags:
	.incbin "baserom.gba", 0x000d6ed8, 0x00000010
	.section .rom.000d6ee8, "ax"
	.global Func_080d1ee8
	.thumb_func
	.type Func_080d1ee8, %function
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d6ee8, 0x00000024
	.section .rom.000d6f0c, "ax"
	.global Func_080d1f0c
	.thumb_func
	.type Func_080d1f0c, %function
Func_080d1f0c:
	.incbin "baserom.gba", 0x000d6f0c, 0x0000000c
	.section .rom.000d6f18, "ax"
	.global Func_080d1f18
	.thumb_func
	.type Func_080d1f18, %function
Func_080d1f18:
	.incbin "baserom.gba", 0x000d6f18, 0x00000008
	.section .rom.000d6f20, "ax"
	.global Func_080d1f20
	.thumb_func
	.type Func_080d1f20, %function
Func_080d1f20:
	.incbin "baserom.gba", 0x000d6f20, 0x00000164
	.section .rom.000d7084, "ax"
	.global Func_080d2084
	.thumb_func
	.type Func_080d2084, %function
Func_080d2084:
	.incbin "baserom.gba", 0x000d7084, 0x00000078
	.section .rom.000d70fc, "ax"
	.global Func_080d20fc
	.thumb_func
	.type Func_080d20fc, %function
Func_080d20fc:
	.incbin "baserom.gba", 0x000d70fc, 0x000000f8
	.global EventRuntime_UpdateWaitMode
	.thumb_func
	.type EventRuntime_UpdateWaitMode, %function
EventRuntime_UpdateWaitMode:
	.incbin "baserom.gba", 0x000d71f4, 0x0000004c
	.section .rom.000d7394, "ax"
	.incbin "baserom.gba", 0x000d7394, 0x00000004
	.section .rom.000d7398, "ax"
	.global Event_RunObjectHookAndWait
	.thumb_func
	.type Event_RunObjectHookAndWait, %function
Event_RunObjectHookAndWait:
	.incbin "baserom.gba", 0x000d7398, 0x00000020
	.section .rom.000d73ca, "ax"
	.incbin "baserom.gba", 0x000d73ca, 0x00000002
	.section .rom.000d73cc, "ax"
	.global Event_SetWorkWord10
	.thumb_func
	.type Event_SetWorkWord10, %function
Event_SetWorkWord10:
	.incbin "baserom.gba", 0x000d73cc, 0x0000000c
	.section .rom.000d75c8, "ax"
	.global Func_080d25c8
	.thumb_func
	.type Func_080d25c8, %function
Func_080d25c8:
	.incbin "baserom.gba", 0x000d75c8, 0x00000044
	.section .rom.000d760c, "ax"
	.global PartyInventory_GiveItem
	.thumb_func
	.type PartyInventory_GiveItem, %function
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d760c, 0x000001d4
	.section .rom.000d77fe, "ax"
	.incbin "baserom.gba", 0x000d77fe, 0x00000036
	.section .rom.000d7834, "ax"
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
	.type Inventory_PromptAndSetObjectMode, %function
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d7834, 0x0000011c
	.section .rom.000d7950, "ax"
	.global Func_080d295c
	.thumb_func
	.type Func_080d295c, %function
Func_080d295c:
	.incbin "baserom.gba", 0x000d7950, 0x00000010
	.section .rom.000d7960, "ax"
	.global Func_080d296c
	.thumb_func
	.type Func_080d296c, %function
Func_080d296c:
	.incbin "baserom.gba", 0x000d7960, 0x000000a0
	.section .rom.000d7b00, "ax"
	.global Func_080d2b0c
	.thumb_func
	.type Func_080d2b0c, %function
Func_080d2b0c:
	.incbin "baserom.gba", 0x000d7b00, 0x00000040
	.section .rom.000d7b40, "ax"
	.global Func_080d2b4c
	.thumb_func
	.type Func_080d2b4c, %function
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d7b40, 0x000000b8
	.section .rom.000d7c58, "ax"
	.global Func_080d2c64
	.thumb_func
	.type Func_080d2c64, %function
Func_080d2c64:
	.incbin "baserom.gba", 0x000d7c58, 0x00000034
	.section .rom.000d7c8c, "ax"
	.global Func_080d2c98
	.thumb_func
	.type Func_080d2c98, %function
Func_080d2c98:
	.incbin "baserom.gba", 0x000d7c8c, 0x00000004
	.section .rom.000d7c90, "ax"
	.global Func_080d2c9c
	.thumb_func
	.type Func_080d2c9c, %function
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d7c90, 0x00000028
	.section .rom.000d7cb8, "ax"
	.global Func_080d2cc4
	.thumb_func
	.type Func_080d2cc4, %function
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d7cb8, 0x00000044
	.section .rom.000d7cfc, "ax"
	.global Func_080d2d08
	.thumb_func
	.type Func_080d2d08, %function
Func_080d2d08:
	.incbin "baserom.gba", 0x000d7cfc, 0x00000068
	.section .rom.000d7d64, "ax"
	.global Func_080d2d70
	.thumb_func
	.type Func_080d2d70, %function
Func_080d2d70:
	.incbin "baserom.gba", 0x000d7d64, 0x00000014
	.section .rom.000d7dd8, "ax"
	.global ObjectMotion_ResetTargetsAndVelocity
	.thumb_func
	.type ObjectMotion_ResetTargetsAndVelocity, %function
ObjectMotion_ResetTargetsAndVelocity:
	.incbin "baserom.gba", 0x000d7dd8, 0x00000038
	.section .rom.000d7f3c, "ax"
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
	.type ObjectMotion_SnapHeadingAndOffset, %function
ObjectMotion_SnapHeadingAndOffset:
	.incbin "baserom.gba", 0x000d7f3c, 0x00000080
	.section .rom.000d8064, "ax"
	.global Func_080d3070
	.thumb_func
	.type Func_080d3070, %function
Func_080d3070:
	.incbin "baserom.gba", 0x000d8064, 0x0000008c
	.section .rom.000d810a, "ax"
	.incbin "baserom.gba", 0x000d810a, 0x00000002
	.section .rom.000d810c, "ax"
	.global Func_080d3118
	.thumb_func
	.type Func_080d3118, %function
Func_080d3118:
	.incbin "baserom.gba", 0x000d810c, 0x0000004c
	.section .rom.000d8158, "ax"
	.global Func_080d3164
	.thumb_func
	.type Func_080d3164, %function
Func_080d3164:
	.incbin "baserom.gba", 0x000d8158, 0x0000005c
	.section .rom.000d81b4, "ax"
	.global Func_080d31c0
	.thumb_func
	.type Func_080d31c0, %function
Func_080d31c0:
	.incbin "baserom.gba", 0x000d81b4, 0x00000054
	.section .rom.000d8208, "ax"
	.global Func_080d3214
	.thumb_func
	.type Func_080d3214, %function
Func_080d3214:
	.incbin "baserom.gba", 0x000d8208, 0x0000002c
	.section .rom.000d825c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
	.type ObjectMotion_WaitForAnimationChange, %function
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d825c, 0x00000050
	.section .rom.000d8330, "ax"
	.incbin "baserom.gba", 0x000d8330, 0x00000124
	.section .rom.000d8454, "ax"
	.global Func_080d3460
	.thumb_func
	.type Func_080d3460, %function
Func_080d3460:
	.incbin "baserom.gba", 0x000d8454, 0x0000013c
	.section .rom.000d85f2, "ax"
	.incbin "baserom.gba", 0x000d85f2, 0x00000002
	.section .rom.000d85f4, "ax"
	.global FacingObject_TurnPairToFaceEachOther
	.thumb_func
	.type FacingObject_TurnPairToFaceEachOther, %function
FacingObject_TurnPairToFaceEachOther:
	.incbin "baserom.gba", 0x000d85f4, 0x000000a8
	.section .rom.000d869c, "ax"
	.global Func_080d36a8
	.thumb_func
	.type Func_080d36a8, %function
Func_080d36a8:
	.incbin "baserom.gba", 0x000d869c, 0x00000020
	.section .rom.000d8774, "ax"
	.global Func_080d3780
	.thumb_func
	.type Func_080d3780, %function
Func_080d3780:
	.incbin "baserom.gba", 0x000d8774, 0x00000054
	.section .rom.000d8900, "ax"
	.global Func_080d390c
	.thumb_func
	.type Func_080d390c, %function
Func_080d390c:
	.incbin "baserom.gba", 0x000d8900, 0x0000001c
	.section .rom.000d891c, "ax"
	.global Func_080d3928
	.thumb_func
	.type Func_080d3928, %function
Func_080d3928:
	.incbin "baserom.gba", 0x000d891c, 0x00000018
	.section .rom.000d8934, "ax"
	.global Func_080d3940
	.thumb_func
	.type Func_080d3940, %function
Func_080d3940:
	.incbin "baserom.gba", 0x000d8934, 0x000000d0
	.section .rom.000d8a04, "ax"
	.global Func_080d3a10
	.thumb_func
	.type Func_080d3a10, %function
Func_080d3a10:
	.incbin "baserom.gba", 0x000d8a04, 0x00000118
	.section .rom.000d8b1c, "ax"
	.global Func_080d3b28
	.thumb_func
	.type Func_080d3b28, %function
Func_080d3b28:
	.incbin "baserom.gba", 0x000d8b1c, 0x000000c0
	.section .rom.000d8c20, "ax"
	.global Func_080d3c2c
	.thumb_func
	.type Func_080d3c2c, %function
Func_080d3c2c:
	.incbin "baserom.gba", 0x000d8c20, 0x0000005c
	.section .rom.000d8c7c, "ax"
	.global UiText_OpenMessageAtObject
	.thumb_func
	.type UiText_OpenMessageAtObject, %function
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d8c7c, 0x00000328
	.section .rom.000d8fa4, "ax"
	.global Func_080d3fb0
	.thumb_func
	.type Func_080d3fb0, %function
Func_080d3fb0:
	.incbin "baserom.gba", 0x000d8fa4, 0x000000bc
	.section .rom.000d9060, "ax"
	.global Func_080d406c
	.thumb_func
	.type Func_080d406c, %function
Func_080d406c:
	.incbin "baserom.gba", 0x000d9060, 0x00000010
	.section .rom.000d9070, "ax"
	.global EventRuntime_ShowMessageAndWait
	.thumb_func
	.type EventRuntime_ShowMessageAndWait, %function
EventRuntime_ShowMessageAndWait:
	.incbin "baserom.gba", 0x000d9070, 0x00000008
	.section .rom.000d9078, "ax"
	.global Func_080d4084
	.thumb_func
	.type Func_080d4084, %function
Func_080d4084:
	.incbin "baserom.gba", 0x000d9078, 0x00000054
	.section .rom.000d90cc, "ax"
	.global Func_080d40d8
	.thumb_func
	.type Func_080d40d8, %function
Func_080d40d8:
	.incbin "baserom.gba", 0x000d90cc, 0x00000004
	.section .rom.000d90d0, "ax"
	.global Func_080d40dc
	.thumb_func
	.type Func_080d40dc, %function
Func_080d40dc:
	.incbin "baserom.gba", 0x000d90d0, 0x0000009c
	.section .rom.000d916c, "ax"
	.global Func_080d4178
	.thumb_func
	.type Func_080d4178, %function
Func_080d4178:
	.incbin "baserom.gba", 0x000d916c, 0x00000008
	.section .rom.000d91da, "ax"
	.incbin "baserom.gba", 0x000d91da, 0x00000002
	.section .rom.000d91f0, "ax"
	.global Func_080d41fc
	.thumb_func
	.type Func_080d41fc, %function
Func_080d41fc:
	.incbin "baserom.gba", 0x000d91f0, 0x00000134
	.section .rom.000d9324, "ax"
	.global Func_080d4330
	.thumb_func
	.type Func_080d4330, %function
Func_080d4330:
	.incbin "baserom.gba", 0x000d9324, 0x00000054
	.section .rom.000d9378, "ax"
	.global Object_AttachWorkTargetToObject
	.thumb_func
	.type Object_AttachWorkTargetToObject, %function
Object_AttachWorkTargetToObject:
	.incbin "baserom.gba", 0x000d9378, 0x00000068
	.section .rom.000d93e0, "ax"
	.global Func_080d43ec
	.thumb_func
	.type Func_080d43ec, %function
Func_080d43ec:
	.incbin "baserom.gba", 0x000d93e0, 0x00000020
	.section .rom.000d9400, "ax"
	.global Motion_CamBounds
	.thumb_func
	.type Motion_CamBounds, %function
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d9400, 0x00000100
	.section .rom.000d9500, "ax"
	.global Func_080d450c
	.thumb_func
	.type Func_080d450c, %function
Func_080d450c:
	.incbin "baserom.gba", 0x000d9500, 0x00000020
	.section .rom.000d9520, "ax"
	.global Func_080d452c
	.thumb_func
	.type Func_080d452c, %function
Func_080d452c:
	.incbin "baserom.gba", 0x000d9520, 0x0000001c
	.section .rom.000d953c, "ax"
	.global Func_080d4548
	.thumb_func
	.type Func_080d4548, %function
Func_080d4548:
	.incbin "baserom.gba", 0x000d953c, 0x00000020
	.section .rom.000d955c, "ax"
	.global Func_080d4568
	.thumb_func
	.type Func_080d4568, %function
Func_080d4568:
	.incbin "baserom.gba", 0x000d955c, 0x00000050
	.section .rom.000d95ac, "ax"
	.global Func_080d45b8
	.thumb_func
	.type Func_080d45b8, %function
Func_080d45b8:
	.incbin "baserom.gba", 0x000d95ac, 0x000000ec
	.section .rom.000d9698, "ax"
	.global Func_080d46a4
	.thumb_func
	.type Func_080d46a4, %function
Func_080d46a4:
	.incbin "baserom.gba", 0x000d9698, 0x00000070
	.section .rom.000d9708, "ax"
	.global Func_080d4714
	.thumb_func
	.type Func_080d4714, %function
Func_080d4714:
	.incbin "baserom.gba", 0x000d9708, 0x00000058
	.section .rom.000d97a8, "ax"
	.global Func_080d47b4
	.thumb_func
	.type Func_080d47b4, %function
Func_080d47b4:
	.incbin "baserom.gba", 0x000d97a8, 0x000000e8
	.section .rom.000d9890, "ax"
	.global Func_080d489c
	.thumb_func
	.type Func_080d489c, %function
Func_080d489c:
	.incbin "baserom.gba", 0x000d9890, 0x000000f0
	.section .rom.000d9a9c, "ax"
	.global Func_080d4aa8
	.thumb_func
	.type Func_080d4aa8, %function
Func_080d4aa8:
	.incbin "baserom.gba", 0x000d9a9c, 0x0000000c
	.section .rom.000d9aa8, "ax"
	.global Func_080d4ab4
	.thumb_func
	.type Func_080d4ab4, %function
Func_080d4ab4:
	.incbin "baserom.gba", 0x000d9aa8, 0x00000058
	.section .rom.000d9b00, "ax"
	.global Object_SetActionCallback
	.thumb_func
	.type Object_SetActionCallback, %function
Object_SetActionCallback:
	.incbin "baserom.gba", 0x000d9b00, 0x000001fc
	.section .rom.000d9cfc, "ax"
	.global Func_080d4d08
	.thumb_func
	.type Func_080d4d08, %function
Func_080d4d08:
	.incbin "baserom.gba", 0x000d9cfc, 0x000003f0
	.section .rom.000da0ec, "ax"
	.global Func_080d50f8
	.thumb_func
	.type Func_080d50f8, %function
Func_080d50f8:
	.incbin "baserom.gba", 0x000da0ec, 0x000002c0
	.section .rom.000da3ac, "ax"
	.global Func_080d53b8
	.thumb_func
	.type Func_080d53b8, %function
Func_080d53b8:
	.incbin "baserom.gba", 0x000da3ac, 0x00000834
	.section .rom.000dabe0, "ax"
	.global Func_080d5bec
	.thumb_func
	.type Func_080d5bec, %function
Func_080d5bec:
	.incbin "baserom.gba", 0x000dabe0, 0x00000184
	.section .rom.000dad64, "ax"
	.global Func_080d5d70
	.thumb_func
	.type Func_080d5d70, %function
Func_080d5d70:
	.incbin "baserom.gba", 0x000dad64, 0x00000070
	.section .rom.000dadd4, "ax"
	.global ObjectEffect_PrepareContextEffect
	.thumb_func
	.type ObjectEffect_PrepareContextEffect, %function
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000dadd4, 0x00000070
	.section .rom.000dae56, "ax"
	.incbin "baserom.gba", 0x000dae56, 0x00000002
	.section .rom.000dae58, "ax"
	.global Func_080d5e64
	.thumb_func
	.type Func_080d5e64, %function
Func_080d5e64:
	.incbin "baserom.gba", 0x000dae58, 0x00000014
	.section .rom.000dae6c, "ax"
	.global ObjectEffect_EndContextEffect
	.thumb_func
	.type ObjectEffect_EndContextEffect, %function
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000dae6c, 0x000000a0
	.section .rom.000dafc8, "ax"
	.incbin "baserom.gba", 0x000dafc8, 0x00000434
	.section .rom.000db3fc, "ax"
	.global Func_080d6408
	.thumb_func
	.type Func_080d6408, %function
Func_080d6408:
	.incbin "baserom.gba", 0x000db3fc, 0x000000b0
	.section .rom.000db4ac, "ax"
	.global Func_080d64b8
	.thumb_func
	.type Func_080d64b8, %function
Func_080d64b8:
	.incbin "baserom.gba", 0x000db4ac, 0x00000454
	.section .rom.000db900, "ax"
	.global Func_080d690c
	.thumb_func
	.type Func_080d690c, %function
Func_080d690c:
	.incbin "baserom.gba", 0x000db900, 0x00000284
	.section .rom.000dbb84, "ax"
	.global Func_080d6b90
	.thumb_func
	.type Func_080d6b90, %function
Func_080d6b90:
	.incbin "baserom.gba", 0x000dbb84, 0x000002c8
	.section .rom.000dbe4c, "ax"
	.global Func_080d6e58
	.thumb_func
	.type Func_080d6e58, %function
Func_080d6e58:
	.incbin "baserom.gba", 0x000dbe4c, 0x000000dc
	.section .rom.000dbf28, "ax"
	.global Func_080d6f34
	.thumb_func
	.type Func_080d6f34, %function
Func_080d6f34:
	.incbin "baserom.gba", 0x000dbf28, 0x000000f0
	.section .rom.000dc018, "ax"
	.global Func_080d7024
	.thumb_func
	.type Func_080d7024, %function
Func_080d7024:
	.incbin "baserom.gba", 0x000dc018, 0x0000021c
	.section .rom.000dc234, "ax"
	.global Func_080d7240
	.thumb_func
	.type Func_080d7240, %function
Func_080d7240:
	.incbin "baserom.gba", 0x000dc234, 0x000000c4
	.section .rom.000dc2f8, "ax"
	.global Func_080d7304
	.thumb_func
	.type Func_080d7304, %function
Func_080d7304:
	.incbin "baserom.gba", 0x000dc2f8, 0x000000b0
	.section .rom.000dc3a8, "ax"
	.global Func_080d73b4
	.thumb_func
	.type Func_080d73b4, %function
Func_080d73b4:
	.incbin "baserom.gba", 0x000dc3a8, 0x0000002c
	.section .rom.000dc3d4, "ax"
	.global Func_080d73e0
	.thumb_func
	.type Func_080d73e0, %function
Func_080d73e0:
	.incbin "baserom.gba", 0x000dc3d4, 0x00000028
	.section .rom.000dc3fc, "ax"
	.global Func_080d7408
	.thumb_func
	.type Func_080d7408, %function
Func_080d7408:
	.incbin "baserom.gba", 0x000dc3fc, 0x00000028
	.section .rom.000dc424, "ax"
	.global Func_080d7430
	.thumb_func
	.type Func_080d7430, %function
Func_080d7430:
	.incbin "baserom.gba", 0x000dc424, 0x000000c0
	.section .rom.000dc518, "ax"
	.incbin "baserom.gba", 0x000dc518, 0x00000264
	.section .rom.000dc77c, "ax"
	.global Func_080d7788
	.thumb_func
	.type Func_080d7788, %function
Func_080d7788:
	.incbin "baserom.gba", 0x000dc77c, 0x000002d0
	.section .rom.000dcaa8, "ax"
	.global Func_080d7ab4
	.thumb_func
	.type Func_080d7ab4, %function
Func_080d7ab4:
	.incbin "baserom.gba", 0x000dcaa8, 0x00000c58
	.section .rom.000dd734, "ax"
	.incbin "baserom.gba", 0x000dd734, 0x0000023c
	.section .rom.000dd970, "ax"
	.global Func_080d897c
	.thumb_func
	.type Func_080d897c, %function
Func_080d897c:
	.incbin "baserom.gba", 0x000dd970, 0x000003ec
	.section .rom.000ddd5c, "ax"
	.global Func_080d8d68
	.thumb_func
	.type Func_080d8d68, %function
Func_080d8d68:
	.incbin "baserom.gba", 0x000ddd5c, 0x0000039c
	.section .rom.000de0f8, "ax"
	.global Func_080d9104
	.thumb_func
	.type Func_080d9104, %function
Func_080d9104:
	.incbin "baserom.gba", 0x000de0f8, 0x000001d0
	.section .rom.000de2c8, "ax"
	.global Func_080d92d4
	.thumb_func
	.type Func_080d92d4, %function
Func_080d92d4:
	.incbin "baserom.gba", 0x000de2c8, 0x0000043c
	.section .rom.000de704, "ax"
	.global Func_080d9710
	.thumb_func
	.type Func_080d9710, %function
Func_080d9710:
	.incbin "baserom.gba", 0x000de704, 0x000003a0
	.section .rom.000deaa4, "ax"
	.global Func_080d9ab0
	.thumb_func
	.type Func_080d9ab0, %function
Func_080d9ab0:
	.incbin "baserom.gba", 0x000deaa4, 0x00000020
	.section .rom.000deac4, "ax"
	.global Func_080d9ad0
	.thumb_func
	.type Func_080d9ad0, %function
Func_080d9ad0:
	.incbin "baserom.gba", 0x000deac4, 0x00000038
	.section .rom.000deafc, "ax"
	.global Func_080d9b08
	.thumb_func
	.type Func_080d9b08, %function
Func_080d9b08:
	.incbin "baserom.gba", 0x000deafc, 0x00000390
	.section .rom.000dee8c, "ax"
	.global Func_080d9e98
	.thumb_func
	.type Func_080d9e98, %function
Func_080d9e98:
	.incbin "baserom.gba", 0x000dee8c, 0x00000a70
	.section .rom.000df8fc, "ax"
	.global Func_080da908
	.thumb_func
	.type Func_080da908, %function
Func_080da908:
	.incbin "baserom.gba", 0x000df8fc, 0x00000030
	.section .rom.000df92c, "ax"
	.global Func_080da938
	.thumb_func
	.type Func_080da938, %function
Func_080da938:
	.incbin "baserom.gba", 0x000df92c, 0x00000594
	.section .rom.000dfec0, "ax"
	.global Func_080daecc
	.thumb_func
	.type Func_080daecc, %function
Func_080daecc:
	.incbin "baserom.gba", 0x000dfec0, 0x000001e4
	.section .rom.000e00a4, "ax"
	.global Func_080db0b0
	.thumb_func
	.type Func_080db0b0, %function
Func_080db0b0:
	.incbin "baserom.gba", 0x000e00a4, 0x00000394
	.section .rom.000e0438, "ax"
	.global Func_080db444
	.thumb_func
	.type Func_080db444, %function
Func_080db444:
	.incbin "baserom.gba", 0x000e0438, 0x00000048
	.section .rom.000e04aa, "ax"
	.incbin "baserom.gba", 0x000e04aa, 0x00000002
	.section .rom.000e04ac, "ax"
	.global BattleFx_Run
	.thumb_func
	.type BattleFx_Run, %function
BattleFx_Run:
	.incbin "baserom.gba", 0x000e04ac, 0x000001b8
	.section .rom.000e0664, "ax"
	.global BattleFx_DispatchRequestKind
	.thumb_func
	.type BattleFx_DispatchRequestKind, %function
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e0664, 0x000001d8
	.section .rom.000e083c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.thumb_func
	.type BattleFx_ClearChildValueOnMismatch, %function
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x000e083c, 0x0000003c
	.section .rom.000e0878, "ax"
	.global FieldEvent_RunTypeHandler
	.thumb_func
	.type FieldEvent_RunTypeHandler, %function
FieldEvent_RunTypeHandler:
	.incbin "baserom.gba", 0x000e0878, 0x00000098
	.section .rom.000e0910, "ax"
	.global ObjectGroup_ApplyRandomChildValues
	.thumb_func
	.type ObjectGroup_ApplyRandomChildValues, %function
ObjectGroup_ApplyRandomChildValues:
	.incbin "baserom.gba", 0x000e0910, 0x000000a4
	.section .rom.000e09b4, "ax"
	.global Func_080db9c0
	.thumb_func
	.type Func_080db9c0, %function
Func_080db9c0:
	.incbin "baserom.gba", 0x000e09b4, 0x0000000c
	.section .rom.000e09c0, "ax"
	.global Func_080db9cc
	.thumb_func
	.type Func_080db9cc, %function
Func_080db9cc:
	.incbin "baserom.gba", 0x000e09c0, 0x0000030c
	.section .rom.000e0ccc, "ax"
	.global Func_080dbcd8
	.thumb_func
	.type Func_080dbcd8, %function
Func_080dbcd8:
	.incbin "baserom.gba", 0x000e0ccc, 0x00000070
	.section .rom.000e0d3c, "ax"
	.global Func_080dbd48
	.thumb_func
	.type Func_080dbd48, %function
Func_080dbd48:
	.incbin "baserom.gba", 0x000e0d3c, 0x00000080
	.section .rom.000e0dbc, "ax"
	.global Func_080dbdc8
	.thumb_func
	.type Func_080dbdc8, %function
Func_080dbdc8:
	.incbin "baserom.gba", 0x000e0dbc, 0x0000000c
	.section .rom.000e0dc8, "ax"
	.global Func_080dbdd4
	.thumb_func
	.type Func_080dbdd4, %function
Func_080dbdd4:
	.incbin "baserom.gba", 0x000e0dc8, 0x00000014
	.section .rom.000e0ddc, "ax"
	.global Func_080dbde8
	.thumb_func
	.type Func_080dbde8, %function
Func_080dbde8:
	.incbin "baserom.gba", 0x000e0ddc, 0x0000000c
	.section .rom.000e0de8, "ax"
	.global Func_080dbdf4
	.thumb_func
	.type Func_080dbdf4, %function
Func_080dbdf4:
	.incbin "baserom.gba", 0x000e0de8, 0x00000014
	.section .rom.000e0dfc, "ax"
	.global Func_080dbe08
	.thumb_func
	.type Func_080dbe08, %function
Func_080dbe08:
	.incbin "baserom.gba", 0x000e0dfc, 0x0000003c
	.section .rom.000e0e38, "ax"
	.global Func_080dbe44
	.thumb_func
	.type Func_080dbe44, %function
Func_080dbe44:
	.incbin "baserom.gba", 0x000e0e38, 0x00000274
	.section .rom.000e10ac, "ax"
	.global Func_080dc0b8
	.thumb_func
	.type Func_080dc0b8, %function
Func_080dc0b8:
	.incbin "baserom.gba", 0x000e10ac, 0x00000020
	.section .rom.000e10cc, "ax"
	.global Func_080dc0d8
	.thumb_func
	.type Func_080dc0d8, %function
Func_080dc0d8:
	.incbin "baserom.gba", 0x000e10cc, 0x00000034
	.section .rom.000e1100, "ax"
	.global Object_Spawn
	.thumb_func
	.type Object_Spawn, %function
Object_Spawn:
	.incbin "baserom.gba", 0x000e1100, 0x00000058
	.section .rom.000e11a4, "ax"
	.global Func_080dc1b0
	.thumb_func
	.type Func_080dc1b0, %function
Func_080dc1b0:
	.incbin "baserom.gba", 0x000e11a4, 0x00000094
	.section .rom.000e1238, "ax"
	.global Field_BeginPaletteTransition
	.thumb_func
	.type Field_BeginPaletteTransition, %function
Field_BeginPaletteTransition:
	.incbin "baserom.gba", 0x000e1238, 0x00000050
	.section .rom.000e1288, "ax"
	.global BattleEffect_InitializeSharedScene
	.thumb_func
	.type BattleEffect_InitializeSharedScene, %function
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e1288, 0x000000f0
	.section .rom.000e1378, "ax"
	.global BattleFx_PrepareBufferInterpolation
	.thumb_func
	.type BattleFx_PrepareBufferInterpolation, %function
BattleFx_PrepareBufferInterpolation:
	.incbin "baserom.gba", 0x000e1378, 0x0000008c
	.section .rom.000e1404, "ax"
	.global Func_080dc410
	.thumb_func
	.type Func_080dc410, %function
Func_080dc410:
	.incbin "baserom.gba", 0x000e1404, 0x0000021c
	.section .rom.000e1620, "ax"
	.global Func_080dc62c
	.thumb_func
	.type Func_080dc62c, %function
Func_080dc62c:
	.incbin "baserom.gba", 0x000e1620, 0x000000ac
	.section .rom.000e16cc, "ax"
	.global Func_080dc6d8
	.thumb_func
	.type Func_080dc6d8, %function
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e16cc, 0x000000f4
	.section .rom.000e17c0, "ax"
	.global Func_080dc7cc
	.thumb_func
	.type Func_080dc7cc, %function
Func_080dc7cc:
	.incbin "baserom.gba", 0x000e17c0, 0x0000001c
	.section .rom.000e17dc, "ax"
	.global Func_080dc7e8
	.thumb_func
	.type Func_080dc7e8, %function
Func_080dc7e8:
	.incbin "baserom.gba", 0x000e17dc, 0x00000190
	.section .rom.000e196c, "ax"
	.global Func_080dc978
	.thumb_func
	.type Func_080dc978, %function
Func_080dc978:
	.incbin "baserom.gba", 0x000e196c, 0x000000d8
	.section .rom.000e1a44, "ax"
	.global Func_080dca50
	.thumb_func
	.type Func_080dca50, %function
Func_080dca50:
	.incbin "baserom.gba", 0x000e1a44, 0x00000034
	.section .rom.000e1a78, "ax"
	.global Func_080dca84
	.thumb_func
	.type Func_080dca84, %function
Func_080dca84:
	.incbin "baserom.gba", 0x000e1a78, 0x00000058
	.section .rom.000e1ad0, "ax"
	.global Func_080dcadc
	.thumb_func
	.type Func_080dcadc, %function
Func_080dcadc:
	.incbin "baserom.gba", 0x000e1ad0, 0x00000478
	.section .rom.000e2048, "ax"
	.global Func_080dd054
	.thumb_func
	.type Func_080dd054, %function
Func_080dd054:
	.incbin "baserom.gba", 0x000e2048, 0x000004d4
	.section .rom.000e251c, "ax"
	.global BattleFx_StartItemBreak
	.thumb_func
	.type BattleFx_StartItemBreak, %function
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000e251c, 0x00000114
	.section .rom.000e2630, "ax"
	.global BattleFx_SnapScaleToFull
	.thumb_func
	.type BattleFx_SnapScaleToFull, %function
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000e2630, 0x0000002c
	.section .rom.000e265c, "ax"
	.global UpdateRisingParticleBurst
	.thumb_func
	.type UpdateRisingParticleBurst, %function
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000e265c, 0x00000ee8
	.section .rom.000e3598, "ax"
	.incbin "baserom.gba", 0x000e3598, 0x00000714
	.section .rom.000e3cac, "ax"
	.global Func_080decb8
	.thumb_func
	.type Func_080decb8, %function
Func_080decb8:
	.incbin "baserom.gba", 0x000e3cac, 0x000000f0
	.section .rom.000e3dd4, "ax"
	.incbin "baserom.gba", 0x000e3dd4, 0x00000ac8
	.section .rom.000e48d2, "ax"
	.incbin "baserom.gba", 0x000e48d2, 0x000001d6
	.section .rom.000e4afe, "ax"
	.incbin "baserom.gba", 0x000e4afe, 0x000001ca
	.section .rom.000e4cec, "ax"
	.incbin "baserom.gba", 0x000e4cec, 0x000005f4
	.section .rom.000e5350, "ax"
	.global Func_080e035c
	.thumb_func
	.type Func_080e035c, %function
Func_080e035c:
	.incbin "baserom.gba", 0x000e5350, 0x00000050
	.section .rom.000e53b6, "ax"
	.incbin "baserom.gba", 0x000e53b6, 0x00000fba
	.section .rom.000e6370, "ax"
	.global Func_080e137c
	.thumb_func
	.type Func_080e137c, %function
Func_080e137c:
	.incbin "baserom.gba", 0x000e6370, 0x000000a4
	.section .rom.000e6414, "ax"
	.global Func_080e1420
	.thumb_func
	.type Func_080e1420, %function
Func_080e1420:
	.incbin "baserom.gba", 0x000e6414, 0x000001dc
	.section .rom.000e65f0, "ax"
	.global Func_080e15fc
	.thumb_func
	.type Func_080e15fc, %function
Func_080e15fc:
	.incbin "baserom.gba", 0x000e65f0, 0x00000054
	.section .rom.000e6644, "ax"
	.global Func_080e1650
	.thumb_func
	.type Func_080e1650, %function
Func_080e1650:
	.incbin "baserom.gba", 0x000e6644, 0x00000f98
	.section .rom.000e75dc, "ax"
	.global Func_080e25e8
	.thumb_func
	.type Func_080e25e8, %function
Func_080e25e8:
	.incbin "baserom.gba", 0x000e75dc, 0x0000018c
	.section .rom.000e7768, "ax"
	.global Func_080e2774
	.thumb_func
	.type Func_080e2774, %function
Func_080e2774:
	.incbin "baserom.gba", 0x000e7768, 0x00000160
	.section .rom.000e78c8, "ax"
	.global Func_080e28d4
	.thumb_func
	.type Func_080e28d4, %function
Func_080e28d4:
	.incbin "baserom.gba", 0x000e78c8, 0x00000dc4
	.section .rom.000e868c, "ax"
	.global Func_080e3698
	.thumb_func
	.type Func_080e3698, %function
Func_080e3698:
	.incbin "baserom.gba", 0x000e868c, 0x00000148
	.section .rom.000e87d4, "ax"
	.global Func_080e37e0
	.thumb_func
	.type Func_080e37e0, %function
Func_080e37e0:
	.incbin "baserom.gba", 0x000e87d4, 0x00000a64
	.section .rom.000e9238, "ax"
	.global Func_080e4244
	.thumb_func
	.type Func_080e4244, %function
Func_080e4244:
	.incbin "baserom.gba", 0x000e9238, 0x00002eb4
	.section .rom.000ec0ec, "ax"
	.global Func_080e70f8
	.thumb_func
	.type Func_080e70f8, %function
Func_080e70f8:
	.incbin "baserom.gba", 0x000ec0ec, 0x00000140
	.section .rom.000ec22c, "ax"
	.global Func_080e7238
	.thumb_func
	.type Func_080e7238, %function
Func_080e7238:
	.incbin "baserom.gba", 0x000ec22c, 0x000002a0
	.section .rom.000ec4cc, "ax"
	.global Func_080e74d8
	.thumb_func
	.type Func_080e74d8, %function
Func_080e74d8:
	.incbin "baserom.gba", 0x000ec4cc, 0x0000032c
	.section .rom.000ec7f8, "ax"
	.global Func_080e7804
	.thumb_func
	.type Func_080e7804, %function
Func_080e7804:
	.incbin "baserom.gba", 0x000ec7f8, 0x00002948
	.section .rom.000ef140, "ax"
	.global Func_080ea14c
	.thumb_func
	.type Func_080ea14c, %function
Func_080ea14c:
	.incbin "baserom.gba", 0x000ef140, 0x00000674
	.section .rom.000ef7b4, "ax"
	.global Func_080ea7c0
	.thumb_func
	.type Func_080ea7c0, %function
Func_080ea7c0:
	.incbin "baserom.gba", 0x000ef7b4, 0x00000114
	.section .rom.000ef8c8, "ax"
	.global Func_080ea8d4
	.thumb_func
	.type Func_080ea8d4, %function
Func_080ea8d4:
	.incbin "baserom.gba", 0x000ef8c8, 0x00000140
	.section .rom.000efa08, "ax"
	.global Func_080eaa14
	.thumb_func
	.type Func_080eaa14, %function
Func_080eaa14:
	.incbin "baserom.gba", 0x000efa08, 0x0000015c
	.section .rom.000efb64, "ax"
	.global Func_080eab70
	.thumb_func
	.type Func_080eab70, %function
Func_080eab70:
	.incbin "baserom.gba", 0x000efb64, 0x00000028
	.section .rom.000efb8c, "ax"
	.global Func_080eab98
	.thumb_func
	.type Func_080eab98, %function
Func_080eab98:
	.incbin "baserom.gba", 0x000efb8c, 0x00000038
	.section .rom.000efbc4, "ax"
	.global Func_080eabd0
	.thumb_func
	.type Func_080eabd0, %function
Func_080eabd0:
	.incbin "baserom.gba", 0x000efbc4, 0x00000118
	.section .rom.000efcdc, "ax"
	.global Func_080eace8
	.thumb_func
	.type Func_080eace8, %function
Func_080eace8:
	.incbin "baserom.gba", 0x000efcdc, 0x00000114
	.section .rom.000efdf0, "ax"
	.global Func_080eadfc
	.thumb_func
	.type Func_080eadfc, %function
Func_080eadfc:
	.incbin "baserom.gba", 0x000efdf0, 0x000000b8
	.section .rom.000efea8, "ax"
	.global Func_080eaeb4
	.thumb_func
	.type Func_080eaeb4, %function
Func_080eaeb4:
	.incbin "baserom.gba", 0x000efea8, 0x00000074
	.section .rom.000eff1c, "ax"
	.global Func_080eaf28
	.thumb_func
	.type Func_080eaf28, %function
Func_080eaf28:
	.incbin "baserom.gba", 0x000eff1c, 0x00000070
	.section .rom.000eff8c, "ax"
	.global Func_080eaf98
	.thumb_func
	.type Func_080eaf98, %function
Func_080eaf98:
	.incbin "baserom.gba", 0x000eff8c, 0x00000084
	.section .rom.000f0010, "ax"
	.global Func_080eb01c
	.thumb_func
	.type Func_080eb01c, %function
Func_080eb01c:
	.incbin "baserom.gba", 0x000f0010, 0x0000027c
	.section .rom.000f028c, "ax"
	.global Func_080eb298
	.thumb_func
	.type Func_080eb298, %function
Func_080eb298:
	.incbin "baserom.gba", 0x000f028c, 0x00000030
	.section .rom.000f02bc, "ax"
	.global Func_080eb2c8
	.thumb_func
	.type Func_080eb2c8, %function
Func_080eb2c8:
	.incbin "baserom.gba", 0x000f02bc, 0x00000008
	.section .rom.000f02c4, "ax"
	.global Func_080eb2d0
	.thumb_func
	.type Func_080eb2d0, %function
Func_080eb2d0:
	.incbin "baserom.gba", 0x000f02c4, 0x00000690
	.section .rom.000f0954, "ax"
	.global Func_080eb960
	.thumb_func
	.type Func_080eb960, %function
Func_080eb960:
	.incbin "baserom.gba", 0x000f0954, 0x000002d0
	.section .rom.000f0c24, "ax"
	.global EffectSlot_Update
	.thumb_func
	.type EffectSlot_Update, %function
EffectSlot_Update:
	.incbin "baserom.gba", 0x000f0c24, 0x00000240
	.section .rom.000f0e9a, "ax"
	.incbin "baserom.gba", 0x000f0e9a, 0x00000002
	.section .rom.000f0e9c, "ax"
	.global Func_080ebea8
	.thumb_func
	.type Func_080ebea8, %function
Func_080ebea8:
	.incbin "baserom.gba", 0x000f0e9c, 0x0000000c
	.section .rom.000f0ea8, "ax"
	.global Func_080ebeb4
	.thumb_func
	.type Func_080ebeb4, %function
Func_080ebeb4:
	.incbin "baserom.gba", 0x000f0ea8, 0x00000014
	.section .rom.000f0ebc, "ax"
	.global Func_080ebec8
	.thumb_func
	.type Func_080ebec8, %function
Func_080ebec8:
	.incbin "baserom.gba", 0x000f0ebc, 0x000000a0
	.section .rom.000f0f5c, "ax"
	.global BattleFx_ClearOwnedSlot
	.thumb_func
	.type BattleFx_ClearOwnedSlot, %function
BattleFx_ClearOwnedSlot:
	.incbin "baserom.gba", 0x000f0f5c, 0x000001e4
	.section .rom.000f1160, "ax"
	.incbin "baserom.gba", 0x000f1160, 0x00001698
	.section .rom.000f27f8, "ax"
	.global Func_080ed804
	.thumb_func
	.type Func_080ed804, %function
Func_080ed804:
	.incbin "baserom.gba", 0x000f27f8, 0x000002c8
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x000f2ac0, 0x00001488
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000f3f48, 0x00001240
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000f5188, 0x00000544
	.global Object_VariantMotionScripts
Object_VariantMotionScripts:
	.incbin "baserom.gba", 0x000f56cc, 0x000001b4
	.global Data_080f088c
Data_080f088c:
	.incbin "baserom.gba", 0x000f5880, 0x000005c8
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f5e48, 0x00000050
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000f5e98, 0x00000020
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f5eb8, 0x00000068
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
	.global UiIcon_PrepareObjectFar
	.thumb_func
	.type UiIcon_PrepareObjectFar, %function
UiIcon_PrepareObjectFar:
	.incbin "baserom.gba", 0x000fa028, 0x00000008
	.global ItemMenu_Open
	.thumb_func
	.type ItemMenu_Open, %function
ItemMenu_Open:
	.incbin "baserom.gba", 0x000fa030, 0x00000008
	.global ItemMenu_DrawItemDetailsFar
	.thumb_func
	.type ItemMenu_DrawItemDetailsFar, %function
ItemMenu_DrawItemDetailsFar:
	.incbin "baserom.gba", 0x000fa038, 0x00000008
	.global Func_080f8040
	.thumb_func
	.type Func_080f8040, %function
Func_080f8040:
	.incbin "baserom.gba", 0x000fa040, 0x00000008
	.global Palette_CopyObjectBankToBackground14Far
	.thumb_func
	.type Palette_CopyObjectBankToBackground14Far, %function
Palette_CopyObjectBankToBackground14Far:
	.incbin "baserom.gba", 0x000fa048, 0x00000008
	.global Menu_UpdateEntryObjectTransformsFar
	.thumb_func
	.type Menu_UpdateEntryObjectTransformsFar, %function
Menu_UpdateEntryObjectTransformsFar:
	.incbin "baserom.gba", 0x000fa050, 0x00000020
	.section .rom.000fa0e0, "ax"
	.incbin "baserom.gba", 0x000fa0e0, 0x0000003c
	.section .rom.000fa170, "ax"
	.incbin "baserom.gba", 0x000fa170, 0x000004a0
	.section .rom.000fa656, "ax"
	.incbin "baserom.gba", 0x000fa656, 0x000000b2
	.section .rom.000fa888, "ax"
	.global UiIcon_PrepareObject
	.thumb_func
	.type UiIcon_PrepareObject, %function
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fa888, 0x00000390
	.section .rom.000face6, "ax"
	.incbin "baserom.gba", 0x000face6, 0x0000025a
	.section .rom.000faf9c, "ax"
	.incbin "baserom.gba", 0x000faf9c, 0x000001d4
	.section .rom.000fb222, "ax"
	.incbin "baserom.gba", 0x000fb222, 0x00000002
	.section .rom.000fb224, "ax"
	.global Render_SetTilemapFlagRect
	.thumb_func
	.type Render_SetTilemapFlagRect, %function
Render_SetTilemapFlagRect:
	.incbin "baserom.gba", 0x000fb224, 0x00000088
	.section .rom.000fb2ac, "ax"
	.global Palette_CopyObjectBankToBackground14
	.thumb_func
	.type Palette_CopyObjectBankToBackground14, %function
Palette_CopyObjectBankToBackground14:
	.incbin "baserom.gba", 0x000fb2ac, 0x000000c8
	.section .rom.000fb3b6, "ax"
	.incbin "baserom.gba", 0x000fb3b6, 0x00000002
	.section .rom.000fb4a2, "ax"
	.incbin "baserom.gba", 0x000fb4a2, 0x00000002
	.section .rom.000fb4a4, "ax"
	.global Func_080f94a4
	.thumb_func
	.type Func_080f94a4, %function
Func_080f94a4:
	.incbin "baserom.gba", 0x000fb4a4, 0x00000f4c
	.section .rom.000fc472, "ax"
	.incbin "baserom.gba", 0x000fc472, 0x00000002
	.section .rom.000fc474, "ax"
	.global ItemMenu_HideAllIcons
	.thumb_func
	.type ItemMenu_HideAllIcons, %function
ItemMenu_HideAllIcons:
	.incbin "baserom.gba", 0x000fc474, 0x00000884
	.section .rom.000fcd0a, "ax"
	.incbin "baserom.gba", 0x000fcd0a, 0x0000011e
	.global ItemMenu_DrawIcons
	.thumb_func
	.type ItemMenu_DrawIcons, %function
ItemMenu_DrawIcons:
	.incbin "baserom.gba", 0x000fce28, 0x000002d4
	.section .rom.000fd15c, "ax"
	.incbin "baserom.gba", 0x000fd15c, 0x00000c40
	.section .rom.000fde04, "ax"
	.incbin "baserom.gba", 0x000fde04, 0x00000d54
	.section .rom.000feb58, "ax"
	.global Func_080fcab8
	.thumb_func
	.type Func_080fcab8, %function
Func_080fcab8:
	.incbin "baserom.gba", 0x000feb58, 0x0000175c
	.section .rom.001002d4, "ax"
	.global Func_080fe184
	.thumb_func
	.type Func_080fe184, %function
Func_080fe184:
	.incbin "baserom.gba", 0x001002d4, 0x000000f0
	.section .rom.001003c4, "ax"
	.global Func_080fe274
	.thumb_func
	.type Func_080fe274, %function
Func_080fe274:
	.incbin "baserom.gba", 0x001003c4, 0x00000f80
	.section .rom.00101438, "ax"
	.incbin "baserom.gba", 0x00101438, 0x000005f8
	.section .rom.00101a9c, "ax"
	.incbin "baserom.gba", 0x00101a9c, 0x00000b8c
	.section .rom.001026b0, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.thumb_func
	.type ItemMenu_DrawEquippedItemNames, %function
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x001026b0, 0x000000e4
	.section .rom.001027d0, "ax"
	.global ItemMenu_ArrangeCategoryItemIcons
	.thumb_func
	.type ItemMenu_ArrangeCategoryItemIcons, %function
ItemMenu_ArrangeCategoryItemIcons:
	.incbin "baserom.gba", 0x001027d0, 0x000000a0
	.section .rom.00102870, "ax"
	.global ItemMenu_PosCategory
	.thumb_func
	.type ItemMenu_PosCategory, %function
ItemMenu_PosCategory:
	.incbin "baserom.gba", 0x00102870, 0x0000077c
	.section .rom.00102fec, "ax"
	.global Func_08100e7c
	.thumb_func
	.type Func_08100e7c, %function
Func_08100e7c:
	.incbin "baserom.gba", 0x00102fec, 0x00003c38
	.section .rom.00106cae, "ax"
	.incbin "baserom.gba", 0x00106cae, 0x00000002
	.section .rom.00106cb0, "ax"
	.global UiIcon_CreateStatChangeArrow
	.thumb_func
	.type UiIcon_CreateStatChangeArrow, %function
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x00106cb0, 0x00000560
	.section .rom.00107210, "ax"
	.global Menu_ReleaseEntryObjects
	.thumb_func
	.type Menu_ReleaseEntryObjects, %function
Menu_ReleaseEntryObjects:
	.incbin "baserom.gba", 0x00107210, 0x000000f0
	.section .rom.00107300, "ax"
	.global Func_08104ef8
	.thumb_func
	.type Func_08104ef8, %function
Func_08104ef8:
	.incbin "baserom.gba", 0x00107300, 0x000000c4
	.section .rom.001073c4, "ax"
	.global Func_08104fe0
	.thumb_func
	.type Func_08104fe0, %function
Func_08104fe0:
	.incbin "baserom.gba", 0x001073c4, 0x00000040
	.section .rom.00107404, "ax"
	.global Func_081051a8
	.thumb_func
	.type Func_081051a8, %function
Func_081051a8:
	.incbin "baserom.gba", 0x00107404, 0x00000054
	.section .rom.00107458, "ax"
	.global Func_0810526c
	.thumb_func
	.type Func_0810526c, %function
Func_0810526c:
	.incbin "baserom.gba", 0x00107458, 0x0000002c
	.section .rom.00107484, "ax"
	.global Func_081050b8
	.thumb_func
	.type Func_081050b8, %function
Func_081050b8:
	.incbin "baserom.gba", 0x00107484, 0x00000024
	.section .rom.001074a8, "ax"
	.global Func_081052ac
	.thumb_func
	.type Func_081052ac, %function
Func_081052ac:
	.incbin "baserom.gba", 0x001074a8, 0x00000230
	.section .rom.0010774e, "ax"
	.incbin "baserom.gba", 0x0010774e, 0x00000352
	.section .rom.00107aa0, "ax"
	.global Data_08105948
Data_08105948:
	.incbin "baserom.gba", 0x00107aa0, 0x00000024
	.section .rom.00107ac4, "ax"
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00107ac4, 0x00000004
	.section .rom.00107ac8, "ax"
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00107ac8, 0x0000007e
	.section .rom.00107b46, "ax"
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x00107b46, 0x0000000d
	.section .rom.00107b53, "ax"
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x00107b53, 0x0000000d
	.section .rom.00107b60, "ax"
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00107b60, 0x00000018
	.section .rom.00107b78, "ax"
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00107b78, 0x00000030
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x00107ba8, 0x00000458
	.section .rom.00108000, "ax"
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00108000, 0x00000008
	.global Func_08108008
	.thumb_func
	.type Func_08108008, %function
Func_08108008:
	.incbin "baserom.gba", 0x00108008, 0x00000008
	.global Func_08108010
	.thumb_func
	.type Func_08108010, %function
Func_08108010:
	.incbin "baserom.gba", 0x00108010, 0x00000008
	.global Func_08108018
	.thumb_func
	.type Func_08108018, %function
Func_08108018:
	.incbin "baserom.gba", 0x00108018, 0x00000008
	.global Func_08108020
	.thumb_func
	.type Func_08108020, %function
Func_08108020:
	.incbin "baserom.gba", 0x00108020, 0x00000008
	.global Func_08108028
	.thumb_func
	.type Func_08108028, %function
Func_08108028:
	.incbin "baserom.gba", 0x00108028, 0x00000008
	.section .rom.001080a8, "ax"
	.incbin "baserom.gba", 0x001080a8, 0x00000438
	.section .rom.001084f4, "ax"
	.incbin "baserom.gba", 0x001084f4, 0x00000268
	.section .rom.001088d8, "ax"
	.global Func_081088d8
	.thumb_func
	.type Func_081088d8, %function
Func_081088d8:
	.incbin "baserom.gba", 0x001088d8, 0x00000050
	.section .rom.00108928, "ax"
	.global Func_08108928
	.thumb_func
	.type Func_08108928, %function
Func_08108928:
	.incbin "baserom.gba", 0x00108928, 0x00000020
	.section .rom.00108948, "ax"
	.global Func_08108948
	.thumb_func
	.type Func_08108948, %function
Func_08108948:
	.incbin "baserom.gba", 0x00108948, 0x0000009c
	.section .rom.001089e4, "ax"
	.global Func_081089e4
	.thumb_func
	.type Func_081089e4, %function
Func_081089e4:
	.incbin "baserom.gba", 0x001089e4, 0x000000a4
	.section .rom.00108a88, "ax"
	.global Func_08108a88
	.thumb_func
	.type Func_08108a88, %function
Func_08108a88:
	.incbin "baserom.gba", 0x00108a88, 0x00000020
	.section .rom.00108aa8, "ax"
	.global Func_08108aa8
	.thumb_func
	.type Func_08108aa8, %function
Func_08108aa8:
	.incbin "baserom.gba", 0x00108aa8, 0x0000008c
	.section .rom.00108b70, "ax"
	.incbin "baserom.gba", 0x00108b70, 0x00000f28
	.section .rom.00109ad8, "ax"
	.incbin "baserom.gba", 0x00109ad8, 0x00000d30
	.section .rom.0010a808, "ax"
	.global Func_0810a804
	.thumb_func
	.type Func_0810a804, %function
Func_0810a804:
	.incbin "baserom.gba", 0x0010a808, 0x00000030
	.section .rom.0010a838, "ax"
	.global Func_0810a834
	.thumb_func
	.type Func_0810a834, %function
Func_0810a834:
	.incbin "baserom.gba", 0x0010a838, 0x00000018
	.section .rom.0010a850, "ax"
	.global Func_0810a84c
	.thumb_func
	.type Func_0810a84c, %function
Func_0810a84c:
	.incbin "baserom.gba", 0x0010a850, 0x00000018
	.section .rom.0010a8f0, "ax"
	.incbin "baserom.gba", 0x0010a8f0, 0x000005b8
	.section .rom.0010af28, "ax"
	.incbin "baserom.gba", 0x0010af28, 0x000005fc
	.section .rom.0010b5c8, "ax"
	.incbin "baserom.gba", 0x0010b5c8, 0x000001d8
	.section .rom.0010b7a0, "ax"
	.global Func_0810b79c
	.thumb_func
	.type Func_0810b79c, %function
Func_0810b79c:
	.incbin "baserom.gba", 0x0010b7a0, 0x0000000c
	.section .rom.0010b7ac, "ax"
	.global Func_0810b7a8
	.thumb_func
	.type Func_0810b7a8, %function
Func_0810b7a8:
	.incbin "baserom.gba", 0x0010b7ac, 0x00000860
	.section .rom.0010c00c, "ax"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0010c00c, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0010c14c, 0x00000200
	.section .rom.0010c34c, "ax"
	.global Shop_SelectorOffsets
Shop_SelectorOffsets:
	.incbin "baserom.gba", 0x0010c34c, 0x0000003c
	.section .rom.0010c388, "ax"
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x0010c388, 0x0000bc78
	.section .rom.00118000, "ax"
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000010
	.section .rom.00118070, "ax"
	.incbin "baserom.gba", 0x00118070, 0x00000008
	.section .rom.00118078, "ax"
	.global BattleMotion_ApproachTargetFar
	.thumb_func
	.type BattleMotion_ApproachTargetFar, %function
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
	.incbin "baserom.gba", 0x0011bfe8, 0x000000a4
	.section .rom.0011c08c, "ax"
	.global Func_0811c074
	.thumb_func
	.type Func_0811c074, %function
Func_0811c074:
	.incbin "baserom.gba", 0x0011c08c, 0x00000184
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
	.section .rom.00126230, "ax"
	.global Func_08126218
	.thumb_func
	.type Func_08126218, %function
Func_08126218:
	.incbin "baserom.gba", 0x00126230, 0x00000074
	.section .rom.001262a4, "ax"
	.global Func_0812628c
	.thumb_func
	.type Func_0812628c, %function
Func_0812628c:
	.incbin "baserom.gba", 0x001262a4, 0x00000170
	.section .rom.00126414, "ax"
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
	.section .rom.0012695c, "ax"
	.global Func_08126944
	.thumb_func
	.type Func_08126944, %function
Func_08126944:
	.incbin "baserom.gba", 0x0012695c, 0x00000034
	.section .rom.00126990, "ax"
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
	.incbin "baserom.gba", 0x001b20a0, 0x00001d94
	.section .rom.001b3e70, "ax"
	.incbin "baserom.gba", 0x001b3e70, 0x00000040
	.section .rom.001b3ed8, "ax"
	.incbin "baserom.gba", 0x001b3ed8, 0x00004128
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
