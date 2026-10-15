	.section .rom.00000630, "ax"
	.global SoundDriver_EnterFrameUpdate
	.type SoundDriver_EnterFrameUpdate, %function
	.thumb_func
SoundDriver_EnterFrameUpdate:
	.incbin "baserom.gba", 0x00000630, 0x00000088
	.section .rom.000019f4, "ax"
	.global Render_BuildOamList
	.type Render_BuildOamList, %function
	.thumb_func
Render_BuildOamList:
	.incbin "baserom.gba", 0x000019f4, 0x00000020
	.section .rom.00001c90, "ax"
	.global ColorBuffer_ScaleThreeQuarters
	.type ColorBuffer_ScaleThreeQuarters, %function
	.thumb_func
ColorBuffer_ScaleThreeQuarters:
	.incbin "baserom.gba", 0x00001c90, 0x00000020
	.size ColorBuffer_ScaleThreeQuarters, .-ColorBuffer_ScaleThreeQuarters
	.section .rom.00001cb0, "ax"
	.global ColorBuffer_Halve
	.type ColorBuffer_Halve, %function
	.thumb_func
ColorBuffer_Halve:
	.incbin "baserom.gba", 0x00001cb0, 0x00000020
	.size ColorBuffer_Halve, .-ColorBuffer_Halve
	.section .rom.00001cd0, "ax"
	.global ColorBuffer_Brighten
	.type ColorBuffer_Brighten, %function
	.thumb_func
ColorBuffer_Brighten:
	.incbin "baserom.gba", 0x00001cd0, 0x00000020
	.size ColorBuffer_Brighten, .-ColorBuffer_Brighten
	.section .rom.00001cf0, "ax"
	.global ColorBuffer_Darken
	.type ColorBuffer_Darken, %function
	.thumb_func
ColorBuffer_Darken:
	.incbin "baserom.gba", 0x00001cf0, 0x00000020
	.size ColorBuffer_Darken, .-ColorBuffer_Darken
	.section .rom.00013164, "ax"
	.global MemFree
	.type MemFree, %function
	.thumb_func
MemFree:
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
	.section .rom.000133f0, "ax"
	.balign 4
	.global Runtime_InstallIwramAndIrqs
	.type Runtime_InstallIwramAndIrqs, %function
	.thumb_func
Runtime_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133f0, 0x00000074
	.section .rom.000134dc, "ax"
	.global System_WaitForFrameInterrupt
	.type System_WaitForFrameInterrupt, %function
	.thumb_func
System_WaitForFrameInterrupt:
	.incbin "baserom.gba", 0x000134dc, 0x000000b0
	.section .rom.000138e0, "ax"
	.global Input_UpdateKeyRepeatAndDirection
	.type Input_UpdateKeyRepeatAndDirection, %function
	.thumb_func
Input_UpdateKeyRepeatAndDirection:
	.incbin "baserom.gba", 0x000138e0, 0x0000027c
	.section .rom.00013db0, "ax"
	.incbin "baserom.gba", 0x00013db0, 0x000000ec
	.section .rom.00014048, "ax"
	.global AffineMatrix_BuildForEffect
	.type AffineMatrix_BuildForEffect, %function
	.thumb_func
AffineMatrix_BuildForEffect:
	.incbin "baserom.gba", 0x00014048, 0x000000bc
	.section .rom.000141a0, "ax"
	.global ResourceTable_AllocateBlocks
	.type ResourceTable_AllocateBlocks, %function
	.thumb_func
ResourceTable_AllocateBlocks:
	.incbin "baserom.gba", 0x000141a0, 0x000000ac
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
	.section .rom.00014438, "ax"
	.incbin "baserom.gba", 0x00014438, 0x000000b4
	.section .rom.00014b9c, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b9c, 0x0000003c
	.section .rom.00015050, "ax"
	.global SceneTransform_ApplyPitch
	.type SceneTransform_ApplyPitch, %function
	.thumb_func
SceneTransform_ApplyPitch:
	.incbin "baserom.gba", 0x00015050, 0x00000044
	.section .rom.00015094, "ax"
	.global SceneTransform_ApplyYaw
	.type SceneTransform_ApplyYaw, %function
	.thumb_func
SceneTransform_ApplyYaw:
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
	.incbin "baserom.gba", 0x00015154, 0x00000070
	.section .rom.000151d8, "ax"
	.global Func_080151ac
	.type Func_080151ac, %function
	.thumb_func
Func_080151ac:
	.incbin "baserom.gba", 0x000151d8, 0x000000a0
	.section .rom.0001553c, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x0001553c, 0x000001d8
	.section .rom.000157a4, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x000157a4, 0x000000b8
	.section .rom.00015998, "ax"
	.global Func_0801596c
	.type Func_0801596c, %function
	.thumb_func
Func_0801596c:
	.incbin "baserom.gba", 0x00015998, 0x000001b8
	.section .rom.00015cd0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00015cd0, 0x000001a0
	.section .rom.00016080, "ax"
	.balign 4
	.global Func_08016054
	.type Func_08016054, %function
	.thumb_func
Func_08016054:
	.incbin "baserom.gba", 0x00016080, 0x00000080
	.section .rom.000161ac, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x000161ac, 0x0000015c
	.section .rom.00016372, "ax"
	.incbin "baserom.gba", 0x00016372, 0x00000002
	.section .rom.00016512, "ax"
	.incbin "baserom.gba", 0x00016512, 0x00000002
	.section .rom.000165e0, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x000165e0, 0x000000e0
	.section .rom.000166c0, "ax"
	.global SerialRuntime_HandleTransferInterrupt
	.type SerialRuntime_HandleTransferInterrupt, %function
	.thumb_func
SerialRuntime_HandleTransferInterrupt:
	.incbin "baserom.gba", 0x000166c0, 0x00000118
	.section .rom.000169bc, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x000169bc, 0x00000248
	.section .rom.00016c08, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00016c08, 0x000000c8
	.section .rom.00016dfc, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dfc, 0x00000028
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
	.section .rom.000178e0, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x000178e0, 0x00000038
	.section .rom.00017cfc, "ax"
	.global Data_08017cd0
Data_08017cd0:
	.incbin "baserom.gba", 0x00017cfc, 0x00000018
	.section .rom.00017d34, "ax"
	.incbin "baserom.gba", 0x00017d34, 0x00000008
	.section .rom.00017d3c, "ax"
	.global Data_08017d10
Data_08017d10:
	.incbin "baserom.gba", 0x00017d3c, 0x00000054
	.section .rom.00017d90, "ax"
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00017d90, 0x00000014
	.section .rom.00017e28, "ax"
	.incbin "baserom.gba", 0x00017e28, 0x00000018
	.section .rom.00017e40, "ax"
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00017e40, 0x00000058
	.section .rom.00017ec0, "ax"
	.incbin "baserom.gba", 0x00017ec0, 0x00000088
	.section .rom.00017f50, "ax"
	.incbin "baserom.gba", 0x00017f50, 0x00000018
	.section .rom.00017f68, "ax"
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00017f68, 0x0000002c
	.section .rom.00017f94, "ax"
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00017f94, 0x0000002c
	.section .rom.00020010, "ax"
	.global Render_ApplyProjectedPlacementFar
Render_ApplyProjectedPlacementFar:
	.incbin "baserom.gba", 0x00020010, 0x00000020
	.section .rom.00020030, "ax"
	.global Animation_ApplyChildArgumentFar
	.type Animation_ApplyChildArgumentFar, %function
	.thumb_func
Animation_ApplyChildArgumentFar:
	.incbin "baserom.gba", 0x00020030, 0x00000008
	.section .rom.00020038, "ax"
	.global Func_08022b80Far
	.type Func_08022b80Far, %function
	.thumb_func
Func_08022b80Far:
	.incbin "baserom.gba", 0x00020038, 0x00000008
	.section .rom.00020060, "ax"
	.incbin "baserom.gba", 0x00020060, 0x00000008
	.section .rom.00020068, "ax"
	.global ResourceMetadata_ReleaseSlotFar
	.type ResourceMetadata_ReleaseSlotFar, %function
	.thumb_func
ResourceMetadata_ReleaseSlotFar:
	.incbin "baserom.gba", 0x00020068, 0x00000008
	.section .rom.00020070, "ax"
	.global ResourceMetadata_ClearRecordFar
	.type ResourceMetadata_ClearRecordFar, %function
	.thumb_func
ResourceMetadata_ClearRecordFar:
	.incbin "baserom.gba", 0x00020070, 0x00000008
	.section .rom.00020078, "ax"
	.global Animation_InitWorkFromMetadataFar
	.type Animation_InitWorkFromMetadataFar, %function
	.thumb_func
Animation_InitWorkFromMetadataFar:
	.incbin "baserom.gba", 0x00020078, 0x00000008
	.section .rom.00020180, "ax"
	.incbin "baserom.gba", 0x00020180, 0x00000018
	.section .rom.00020198, "ax"
	.global MapLayer_ClearField22Far
	.type MapLayer_ClearField22Far, %function
	.thumb_func
MapLayer_ClearField22Far:
	.incbin "baserom.gba", 0x00020198, 0x00000008
	.section .rom.000201a0, "ax"
	.global MapLayer_SetField22Far
	.type MapLayer_SetField22Far, %function
	.thumb_func
MapLayer_SetField22Far:
	.incbin "baserom.gba", 0x000201a0, 0x00000008
	.section .rom.000201a8, "ax"
	.global Map_DisableBlendScriptFar
	.type Map_DisableBlendScriptFar, %function
	.thumb_func
Map_DisableBlendScriptFar:
	.incbin "baserom.gba", 0x000201a8, 0x00000008
	.section .rom.000201f0, "ax"
	.global Map_GetTerrainHeightFar
	.type Map_GetTerrainHeightFar, %function
	.thumb_func
Map_GetTerrainHeightFar:
	.incbin "baserom.gba", 0x000201f0, 0x00000008
	.section .rom.000201f8, "ax"
	.global GetMapCellCollisionFar
	.type GetMapCellCollisionFar, %function
	.thumb_func
GetMapCellCollisionFar:
	.incbin "baserom.gba", 0x000201f8, 0x00000008
	.section .rom.00020200, "ax"
	.global Func_080201d0
	.type Func_080201d0, %function
	.thumb_func
Func_080201d0:
	.incbin "baserom.gba", 0x00020200, 0x00000008
	.section .rom.00020208, "ax"
	.global Func_080201d8
	.type Func_080201d8, %function
	.thumb_func
Func_080201d8:
	.incbin "baserom.gba", 0x00020208, 0x00000008
	.section .rom.00020238, "ax"
	.global Animation_SetIndexAndInitObjectsFar
	.type Animation_SetIndexAndInitObjectsFar, %function
	.thumb_func
Animation_SetIndexAndInitObjectsFar:
	.incbin "baserom.gba", 0x00020238, 0x00000008
	.section .rom.00020240, "ax"
	.global Func_08020240
	.type Func_08020240, %function
	.thumb_func
Func_08020240:
	.incbin "baserom.gba", 0x00020240, 0x00000008
	.section .rom.00020248, "ax"
	.global Func_08020248
	.type Func_08020248, %function
	.thumb_func
Func_08020248:
	.incbin "baserom.gba", 0x00020248, 0x00000008
	.section .rom.00020250, "ax"
	.global Animation_SetDisplayFlagFar
	.type Animation_SetDisplayFlagFar, %function
	.thumb_func
Animation_SetDisplayFlagFar:
	.incbin "baserom.gba", 0x00020250, 0x00000008
	.section .rom.00020308, "ax"
	.incbin "baserom.gba", 0x00020308, 0x00000010
	.section .rom.00020318, "ax"
	.global Func_0802db64Far
	.type Func_0802db64Far, %function
	.thumb_func
Func_0802db64Far:
	.incbin "baserom.gba", 0x00020318, 0x00000020
	.section .rom.000212d8, "ax"
	.incbin "baserom.gba", 0x000212d8, 0x0000042c
	.section .rom.00021824, "ax"
	.incbin "baserom.gba", 0x00021824, 0x000000f4
	.section .rom.000219cc, "ax"
	.balign 4
	.global Func_080219cc
	.type Func_080219cc, %function
	.thumb_func
Func_080219cc:
	.incbin "baserom.gba", 0x000219cc, 0x00000644
	.section .rom.00022010, "ax"
	.global Func_0802254c
	.type Func_0802254c, %function
	.thumb_func
Func_0802254c:
	.incbin "baserom.gba", 0x00022010, 0x000000e0
	.section .rom.000220f0, "ax"
	.global Render_ApplyProjectedPlacement
	.type Render_ApplyProjectedPlacement, %function
	.thumb_func
Render_ApplyProjectedPlacement:
	.incbin "baserom.gba", 0x000220f0, 0x000006b0
	.section .rom.000228e0, "ax"
	.global InitializeAnimationObjects
	.type InitializeAnimationObjects, %function
	.thumb_func
InitializeAnimationObjects:
	.incbin "baserom.gba", 0x000228e0, 0x0000008c
	.section .rom.00022bd8, "ax"
	.balign 4
	.global Func_08022bd8
	.type Func_08022bd8, %function
	.thumb_func
Func_08022bd8:
	.incbin "baserom.gba", 0x00022bd8, 0x000000a0
	.section .rom.00022c98, "ax"
	.global AnimationObject_Allocate
	.type AnimationObject_Allocate, %function
	.thumb_func
AnimationObject_Allocate:
	.incbin "baserom.gba", 0x00022c98, 0x00000084
	.section .rom.00022d40, "ax"
	.global ResourceObject_Create
	.type ResourceObject_Create, %function
	.thumb_func
ResourceObject_Create:
	.incbin "baserom.gba", 0x00022d40, 0x00000150
	.section .rom.00022f22, "ax"
	.incbin "baserom.gba", 0x00022f22, 0x00000002
	.section .rom.000230e0, "ax"
	.global Func_080230e0
	.type Func_080230e0, %function
	.thumb_func
Func_080230e0:
	.incbin "baserom.gba", 0x000230e0, 0x000000c4
	.section .rom.00023220, "ax"
	.balign 4
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
	.section .rom.0002372a, "ax"
	.incbin "baserom.gba", 0x0002372a, 0x00000002
	.section .rom.0002372c, "ax"
	.global Func_0802372c
	.type Func_0802372c, %function
	.thumb_func
Func_0802372c:
	.incbin "baserom.gba", 0x0002372c, 0x0000009c
	.section .rom.0002386c, "ax"
	.global Func_0802386c
	.type Func_0802386c, %function
	.thumb_func
Func_0802386c:
	.incbin "baserom.gba", 0x0002386c, 0x00000510
	.section .rom.00023d80, "ax"
	.incbin "baserom.gba", 0x00023d80, 0x00000098
	.section .rom.00023e18, "ax"
	.global Func_08023e18
	.type Func_08023e18, %function
	.thumb_func
Func_08023e18:
	.incbin "baserom.gba", 0x00023e18, 0x000000e4
	.section .rom.00023f9c, "ax"
	.incbin "baserom.gba", 0x00023f9c, 0x0000071c
	.section .rom.00024736, "ax"
	.incbin "baserom.gba", 0x00024736, 0x00000002
	.section .rom.00024738, "ax"
	.global Object_SetMoveTarget
	.type Object_SetMoveTarget, %function
	.thumb_func
Object_SetMoveTarget:
	.incbin "baserom.gba", 0x00024738, 0x000001e8
	.section .rom.0002493c, "ax"
	.incbin "baserom.gba", 0x0002493c, 0x00000314
	.section .rom.00024c6e, "ax"
	.incbin "baserom.gba", 0x00024c6e, 0x00000002
	.global Func_08024c70
	.type Func_08024c70, %function
	.thumb_func
Func_08024c70:
	.incbin "baserom.gba", 0x00024c70, 0x0000003c
	.section .rom.00024ec6, "ax"
	.incbin "baserom.gba", 0x00024ec6, 0x00000002
	.section .rom.00024fde, "ax"
	.incbin "baserom.gba", 0x00024fde, 0x00000002
	.section .rom.00025160, "ax"
	.global Func_08025160
	.type Func_08025160, %function
	.thumb_func
Func_08025160:
	.incbin "baserom.gba", 0x00025160, 0x000002c8
	.global Func_08025428
	.type Func_08025428, %function
	.thumb_func
Func_08025428:
	.incbin "baserom.gba", 0x00025428, 0x000001e0
	.global Func_08025608
	.type Func_08025608, %function
	.thumb_func
Func_08025608:
	.incbin "baserom.gba", 0x00025608, 0x00000090
	.global Func_08025698
	.type Func_08025698, %function
	.thumb_func
Func_08025698:
	.incbin "baserom.gba", 0x00025698, 0x00000198
	.global Func_08025830
	.type Func_08025830, %function
	.thumb_func
Func_08025830:
	.incbin "baserom.gba", 0x00025830, 0x00000328
	.section .rom.00025c8a, "ax"
	.incbin "baserom.gba", 0x00025c8a, 0x00000002
	.section .rom.0002632c, "ax"
	.balign 4
	.global Func_0802632c
	.type Func_0802632c, %function
	.thumb_func
Func_0802632c:
	.incbin "baserom.gba", 0x0002632c, 0x00000490
	.global Func_080267bc
	.type Func_080267bc, %function
	.thumb_func
Func_080267bc:
	.incbin "baserom.gba", 0x000267bc, 0x00000614
	.section .rom.00026e60, "ax"
	.global Func_08026e60
	.type Func_08026e60, %function
	.thumb_func
Func_08026e60:
	.incbin "baserom.gba", 0x00026e60, 0x00000120
	.section .rom.00026fc8, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x00026fc8, 0x0000009c
	.global Func_08027064
	.type Func_08027064, %function
	.thumb_func
Func_08027064:
	.incbin "baserom.gba", 0x00027064, 0x0000094c
	.global Func_080279b0
	.type Func_080279b0, %function
	.thumb_func
Func_080279b0:
	.incbin "baserom.gba", 0x000279b0, 0x0000045c
	.section .rom.00027e20, "ax"
	.balign 4
	.global Func_08027e20
	.type Func_08027e20, %function
	.thumb_func
Func_08027e20:
	.incbin "baserom.gba", 0x00027e20, 0x00000714
	.global Func_08028534
	.type Func_08028534, %function
	.thumb_func
Func_08028534:
	.incbin "baserom.gba", 0x00028534, 0x00000578
	.global Func_08028aac
	.type Func_08028aac, %function
	.thumb_func
Func_08028aac:
	.incbin "baserom.gba", 0x00028aac, 0x00000648
	.global Func_080290f4
	.type Func_080290f4, %function
	.thumb_func
Func_080290f4:
	.incbin "baserom.gba", 0x000290f4, 0x00000208
	.global Func_080292fc
	.type Func_080292fc, %function
	.thumb_func
Func_080292fc:
	.incbin "baserom.gba", 0x000292fc, 0x00000274
	.global Func_08029570
	.type Func_08029570, %function
	.thumb_func
Func_08029570:
	.incbin "baserom.gba", 0x00029570, 0x000002a0
	.section .rom.00029838, "ax"
	.incbin "baserom.gba", 0x00029838, 0x00000088
	.global Func_080298c0
	.type Func_080298c0, %function
	.thumb_func
Func_080298c0:
	.incbin "baserom.gba", 0x000298c0, 0x00000864
	.global Func_0802a124
	.type Func_0802a124, %function
	.thumb_func
Func_0802a124:
	.incbin "baserom.gba", 0x0002a124, 0x00000408
	.section .rom.0002a5e4, "ax"
	.global Func_0802a5e4
	.type Func_0802a5e4, %function
	.thumb_func
Func_0802a5e4:
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
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x0002b1a0, 0x00000134
	.section .rom.0002b38c, "ax"
	.global Map_CopyCellAttributeRect
	.type Map_CopyCellAttributeRect, %function
	.thumb_func
Map_CopyCellAttributeRect:
	.incbin "baserom.gba", 0x0002b38c, 0x000000c4
	.section .rom.0002b450, "ax"
	.global Func_0802b450
	.type Func_0802b450, %function
	.thumb_func
Func_0802b450:
	.incbin "baserom.gba", 0x0002b450, 0x00000140
	.section .rom.0002b590, "ax"
	.global Func_0802b590
	.type Func_0802b590, %function
	.thumb_func
Func_0802b590:
	.incbin "baserom.gba", 0x0002b590, 0x00000090
	.section .rom.0002b878, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x0002b878, 0x00000120
	.section .rom.0002b998, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b998, 0x00000370
	.section .rom.0002bdd0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0002bdd0, 0x0000007c
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
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x0002c4d8, 0x0000038c
	.section .rom.0002c9c8, "ax"
	.global Func_0802c9c8
	.type Func_0802c9c8, %function
	.thumb_func
Func_0802c9c8:
	.incbin "baserom.gba", 0x0002c9c8, 0x00000140
	.section .rom.0002cb64, "ax"
	.global MapAnimation_Update
	.type MapAnimation_Update, %function
	.thumb_func
MapAnimation_Update:
	.incbin "baserom.gba", 0x0002cb64, 0x00000110
	.section .rom.0002cc9c, "ax"
	.incbin "baserom.gba", 0x0002cc9c, 0x000000b0
	.section .rom.0002d088, "ax"
	.incbin "baserom.gba", 0x0002d088, 0x000001b4
	.section .rom.0002d36e, "ax"
	.incbin "baserom.gba", 0x0002d36e, 0x00000002
	.section .rom.0002d45c, "ax"
	.global Func_0802d45c
	.type Func_0802d45c, %function
	.thumb_func
Func_0802d45c:
	.incbin "baserom.gba", 0x0002d45c, 0x0000007c
	.section .rom.0002d71c, "ax"
	.global Func_0802d71c
	.type Func_0802d71c, %function
	.thumb_func
Func_0802d71c:
	.incbin "baserom.gba", 0x0002d71c, 0x00000160
	.section .rom.0002d87c, "ax"
	.global Func_0802d87c
	.type Func_0802d87c, %function
	.thumb_func
Func_0802d87c:
	.incbin "baserom.gba", 0x0002d87c, 0x000001a8
	.section .rom.0002da88, "ax"
	.global Func_0802da88
	.type Func_0802da88, %function
	.thumb_func
Func_0802da88:
	.incbin "baserom.gba", 0x0002da88, 0x00000038
	.global Func_0802b7e0
	.type Func_0802b7e0, %function
	.thumb_func
Func_0802b7e0:
	.section .rom.0002dd70, "ax"
	.incbin "baserom.gba", 0x0002dd70, 0x0000011c
	.section .rom.0002de8c, "ax"
	.global Func_0802a650
	.type Func_0802a650, %function
	.thumb_func
Func_0802a650:
	.incbin "baserom.gba", 0x0002de8c, 0x00000828
	.section .rom.00030310, "ax"
	.incbin "baserom.gba", 0x00030310, 0x00000010
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x0000031c
	.section .rom.000391c4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000391c4, 0x0000009c
	.section .rom.00039260, "ax"
	.global UiWindow_Create
	.type UiWindow_Create, %function
	.thumb_func
UiWindow_Create:
	.incbin "baserom.gba", 0x00039260, 0x00000114
	.section .rom.0003939a, "ax"
	.incbin "baserom.gba", 0x0003939a, 0x00000002
	.section .rom.00039452, "ax"
	.incbin "baserom.gba", 0x00039452, 0x00000002
	.section .rom.00039454, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039454, 0x00000094
	.section .rom.0003954c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003954c, 0x00000154
	.section .rom.0003972a, "ax"
	.incbin "baserom.gba", 0x0003972a, 0x0000002a
	.section .rom.0003975c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003975c, 0x00000084
	.section .rom.000397e0, "ax"
	.global UiWork_ProcessRenderChannels
	.type UiWork_ProcessRenderChannels, %function
	.thumb_func
UiWork_ProcessRenderChannels:
	.incbin "baserom.gba", 0x000397e0, 0x000006bc
	.section .rom.00039e9c, "ax"
	.global UiWork_ProcessDirectWork
	.type UiWork_ProcessDirectWork, %function
	.thumb_func
UiWork_ProcessDirectWork:
	.incbin "baserom.gba", 0x00039e9c, 0x00000184
	.section .rom.0003a27c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003a27c, 0x00000108
	.section .rom.0003a412, "ax"
	.incbin "baserom.gba", 0x0003a412, 0x00000002
	.section .rom.0003a518, "ax"
	.global UiMessage_Show
	.type UiMessage_Show, %function
	.thumb_func
UiMessage_Show:
	.incbin "baserom.gba", 0x0003a518, 0x00000094
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
	.section .rom.0003a8c8, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a8c8, 0x000001a8
	.section .rom.0003aa96, "ax"
	.incbin "baserom.gba", 0x0003aa96, 0x00000002
	.section .rom.0003aa98, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa98, 0x000001f0
	.section .rom.0003ac88, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x0003ac88, 0x00000120
	.section .rom.0003ada8, "ax"
	.global UiText_FormatNumber
	.type UiText_FormatNumber, %function
	.thumb_func
UiText_FormatNumber:
	.incbin "baserom.gba", 0x0003ada8, 0x0000027c
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
	.global UiText_GetResourceDimensionsAlt
	.type UiText_GetResourceDimensionsAlt, %function
	.thumb_func
UiText_GetResourceDimensionsAlt:
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
	.section .rom.0003c184, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c184, 0x000001e0
	.section .rom.0003c3f8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c3f8, 0x0000013c
	.section .rom.0003c534, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c534, 0x00000464
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
	.section .rom.0003cbea, "ax"
	.incbin "baserom.gba", 0x0003cbea, 0x00000002
	.section .rom.0003cd4c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd4c, 0x00000114
	.section .rom.0003cea8, "ax"
	.balign 4
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cea8, 0x000000fc
	.section .rom.0003cfb0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cfb0, 0x00000034
	.section .rom.0003d014, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003d014, 0x00000050
	.section .rom.0003d064, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d064, 0x00000158
	.section .rom.0003d208, "ax"
	.incbin "baserom.gba", 0x0003d208, 0x0000010c
	.section .rom.0003d314, "ax"
	.global UiWork_SetTwoEntriesTo999
	.type UiWork_SetTwoEntriesTo999, %function
	.thumb_func
UiWork_SetTwoEntriesTo999:
	.incbin "baserom.gba", 0x0003d314, 0x00000020
	.section .rom.0003d528, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d528, 0x000000b0
	.section .rom.0003d608, "ax"
	.balign 4
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
	.section .rom.0003d9fe, "ax"
	.incbin "baserom.gba", 0x0003d9fe, 0x00000002
	.section .rom.0003da00, "ax"
	.global UiIcon_BuildAbilityIconTiles
	.type UiIcon_BuildAbilityIconTiles, %function
	.thumb_func
UiIcon_BuildAbilityIconTiles:
	.incbin "baserom.gba", 0x0003da00, 0x000000bc
	.section .rom.0003dc60, "ax"
	.balign 4
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc60, 0x00000108
	.section .rom.0003dddc, "ax"
	.balign 4
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003dddc, 0x00000110
	.section .rom.0003df68, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003df68, 0x00000684
	.section .rom.0003e80c, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e80c, 0x00000124
	.section .rom.0003ea98, "ax"
	.incbin "baserom.gba", 0x0003ea98, 0x00000188
	.section .rom.0003ec20, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec20, 0x000001bc
	.section .rom.0003eddc, "ax"
	.global Menu_StepLeft
	.type Menu_StepLeft, %function
	.thumb_func
Menu_StepLeft:
	.incbin "baserom.gba", 0x0003eddc, 0x000001b0
	.section .rom.0003f048, "ax"
	.incbin "baserom.gba", 0x0003f048, 0x000001d0
	.section .rom.0003f328, "ax"
	.incbin "baserom.gba", 0x0003f328, 0x000000e4
	.section .rom.0003f40c, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f40c, 0x00000248
	.section .rom.0003f668, "ax"
	.balign 4
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f668, 0x00000074
	.section .rom.0003f7bc, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0003f7bc, 0x00000088
	.section .rom.0003f844, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0003f844, 0x00000010
	.section .rom.0003fa04, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa04, 0x000001a0
	.section .rom.0003fc7c, "ax"
	.incbin "baserom.gba", 0x0003fc7c, 0x000000dc
	.section .rom.0003fd6c, "ax"
	.incbin "baserom.gba", 0x0003fd6c, 0x000003c0
	.section .rom.0004012c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004012c, 0x000004c4
	.section .rom.0004066c, "ax"
	.incbin "baserom.gba", 0x0004066c, 0x00000364
	.section .rom.00040a34, "ax"
	.incbin "baserom.gba", 0x00040a34, 0x00000270
	.section .rom.00040db4, "ax"
	.incbin "baserom.gba", 0x00040db4, 0x000000f4
	.section .rom.000410ac, "ax"
	.incbin "baserom.gba", 0x000410ac, 0x0000019c
	.section .rom.000412d0, "ax"
	.incbin "baserom.gba", 0x000412d0, 0x00000440
	.section .rom.00041710, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x00041710, 0x000003fc
	.section .rom.00041b0c, "ax"
	.global UiWindow_ClearTileAttributesInRect
	.type UiWindow_ClearTileAttributesInRect, %function
	.thumb_func
UiWindow_ClearTileAttributesInRect:
	.incbin "baserom.gba", 0x00041b0c, 0x000000ac
	.section .rom.00041bb8, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bb8, 0x000000a4
	.section .rom.00041ca4, "ax"
	.balign 4
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
	.section .rom.00041ff4, "ax"
	.balign 4
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
	.section .rom.0004214c, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x0004214c, 0x0000008c
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
	.global CreateSideObject
	.type CreateSideObject, %function
	.thumb_func
CreateSideObject:
	.incbin "baserom.gba", 0x000424a0, 0x000000b8
	.section .rom.00042558, "ax"
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x00042558, 0x00000064
	.section .rom.000425ca, "ax"
	.incbin "baserom.gba", 0x000425ca, 0x00000002
	.section .rom.000425d8, "ax"
	.balign 4
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x000425d8, 0x00000074
	.section .rom.00042730, "ax"
	.incbin "baserom.gba", 0x00042730, 0x0000029c
	.section .rom.000429cc, "ax"
	.global PtyStDraw
	.type PtyStDraw, %function
	.thumb_func
PtyStDraw:
	.incbin "baserom.gba", 0x000429cc, 0x00000430
	.section .rom.00042e14, "ax"
	.incbin "baserom.gba", 0x00042e14, 0x00000064
	.section .rom.00042f60, "ax"
	.global Func_08042f10
	.type Func_08042f10, %function
	.thumb_func
Func_08042f10:
	.incbin "baserom.gba", 0x00042f60, 0x000000e0
	.section .rom.00043040, "ax"
	.global Func_08042ff0
	.type Func_08042ff0, %function
	.thumb_func
Func_08042ff0:
	.incbin "baserom.gba", 0x00043040, 0x000000d4
	.section .rom.00043114, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043114, 0x000000ec
	.section .rom.00043200, "ax"
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x00043200, 0x00000080
	.section .rom.000432f4, "ax"
	.balign 4
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000432f4, 0x000000b4
	.section .rom.000433a8, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x000433a8, 0x00000194
	.section .rom.00043584, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043584, 0x000000c8
	.section .rom.000436e0, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436e0, 0x000000e4
	.section .rom.00043ab4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ab4, 0x000000d0
	.section .rom.00043bc0, "ax"
	.global Func_08043b70
	.type Func_08043b70, %function
	.thumb_func
Func_08043b70:
	.incbin "baserom.gba", 0x00043bc0, 0x0000007c
	.section .rom.00043d40, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d40, 0x0000066c
	.section .rom.000443b0, "ax"
	.incbin "baserom.gba", 0x000443b0, 0x00000090
	.section .rom.000445c0, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445c0, 0x0000051c
	.section .rom.00044ae0, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae0, 0x00000128
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
	.incbin "baserom.gba", 0x00044e50, 0x00000184
	.section .rom.000450a0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000450a0, 0x00000030
	.section .rom.00045184, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x00045184, 0x000000a0
	.section .rom.00045244, "ax"
	.incbin "baserom.gba", 0x00045244, 0x00000090
	.section .rom.00045342, "ax"
	.incbin "baserom.gba", 0x00045342, 0x00000002
	.section .rom.00045344, "ax"
	.global Graphics_MergeShiftedTileRows
	.type Graphics_MergeShiftedTileRows, %function
	.thumb_func
Graphics_MergeShiftedTileRows:
	.incbin "baserom.gba", 0x00045344, 0x00000074
	.section .rom.000455ae, "ax"
	.incbin "baserom.gba", 0x000455ae, 0x00000002
	.section .rom.000455ec, "ax"
	.incbin "baserom.gba", 0x000455ec, 0x0000004c
	.section .rom.00045780, "ax"
	.incbin "baserom.gba", 0x00045780, 0x00000070
	.section .rom.00045806, "ax"
	.incbin "baserom.gba", 0x00045806, 0x00000002
	.section .rom.00045856, "ax"
	.incbin "baserom.gba", 0x00045856, 0x00000966
	.section .rom.0004630c, "ax"
	.incbin "baserom.gba", 0x0004630c, 0x00000150
	.section .rom.0004649a, "ax"
	.incbin "baserom.gba", 0x0004649a, 0x0000365e
	.section .rom.00049b46, "ax"
	.incbin "baserom.gba", 0x00049b46, 0x00000002
	.section .rom.00049b74, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00049b74, 0x00001d54
	.section .rom.0004b9e8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004b9e8, 0x000000bc
	.section .rom.0004bade, "ax"
	.incbin "baserom.gba", 0x0004bade, 0x000002f2
	.section .rom.0004bdf8, "ax"
	.incbin "baserom.gba", 0x0004bdf8, 0x00000038
	.section .rom.0004be30, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be30, 0x000011a0
	.section .rom.0004cfd0, "ax"
	.global Func_0804cda8
	.type Func_0804cda8, %function
	.thumb_func
Func_0804cda8:
	.incbin "baserom.gba", 0x0004cfd0, 0x00000334
	.section .rom.0004d394, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d394, 0x00000120
	.section .rom.0004d4b4, "ax"
	.global Menu_SelectResource
	.type Menu_SelectResource, %function
	.thumb_func
Menu_SelectResource:
	.incbin "baserom.gba", 0x0004d4b4, 0x000000b8
	.section .rom.0004d610, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d610, 0x0000009c
	.section .rom.0004d6ac, "ax"
	.global Menu_LayoutResourceEntries
	.type Menu_LayoutResourceEntries, %function
	.thumb_func
Menu_LayoutResourceEntries:
	.incbin "baserom.gba", 0x0004d6ac, 0x00000074
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
	.section .rom.0004dcde, "ax"
	.incbin "baserom.gba", 0x0004dcde, 0x00000002
	.section .rom.0004dd98, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x0004dd98, 0x00000254
	.section .rom.0004e0c4, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0c4, 0x000000c4
	.section .rom.0004e2f2, "ax"
	.incbin "baserom.gba", 0x0004e2f2, 0x00000002
	.section .rom.0004e2f4, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2f4, 0x0000007c
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
	.incbin "baserom.gba", 0x0004e618, 0x00000190
	.section .rom.0004e8a8, "ax"
	.incbin "baserom.gba", 0x0004e8a8, 0x000004d4
	.section .rom.00059a14, "ax"
	.incbin "baserom.gba", 0x00059a14, 0x00000084
	.section .rom.00059b98, "ax"
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b98, 0x00000c50
	.section .rom.0005c3e8, "ax"
	.incbin "baserom.gba", 0x0005c3e8, 0x0000006a
	.section .rom.0005c4da, "ax"
	.incbin "baserom.gba", 0x0005c4da, 0x00000012
	.section .rom.0005ecec, "ax"
	.incbin "baserom.gba", 0x0005ecec, 0x0000018c
	.section .rom.0005ee78, "ax"
	.global Data_0805eb50
Data_0805eb50:
	.incbin "baserom.gba", 0x0005ee78, 0x00000008
	.section .rom.0005ee80, "ax"
	.global Data_0805eb58
Data_0805eb58:
	.incbin "baserom.gba", 0x0005ee80, 0x00000024
	.section .rom.0005eea4, "ax"
	.global Data_0805eb7c
Data_0805eb7c:
	.incbin "baserom.gba", 0x0005eea4, 0x00000108
	.section .rom.0005f7ac, "ax"
	.incbin "baserom.gba", 0x0005f7ac, 0x000000fb
	.section .rom.0005f8a7, "ax"
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x0005f8a7, 0x000000bd
	.section .rom.0005f964, "ax"
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f964, 0x00000030
	.section .rom.0005f994, "ax"
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x0005f994, 0x00000078
	.section .rom.0005fadc, "ax"
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fadc, 0x00000008
	.section .rom.0005fb84, "ax"
	.incbin "baserom.gba", 0x0005fb84, 0x0000005e
	.section .rom.0005fbe2, "ax"
	.global Menu_TopEntryCommandByPosition
	.type Menu_TopEntryCommandByPosition, %function
	.thumb_func
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fbe2, 0x0000000c
	.section .rom.0005fbee, "ax"
	.global Menu_TopEntryPositionByCommand
	.type Menu_TopEntryPositionByCommand, %function
	.thumb_func
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fbee, 0x00000026
	.section .rom.0005fc14, "ax"
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0005fc14, 0x0000003c
	.section .rom.000a8df0, "ax"
	.incbin "baserom.gba", 0x000a8df0, 0x0000000c
	.section .rom.000a8e3c, "ax"
	.incbin "baserom.gba", 0x000a8e3c, 0x000002b4
	.section .rom.000a90f0, "ax"
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a90f0, 0x00000004
	.section .rom.000b2070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000b2070, 0x00000008
	.section .rom.000b2078, "ax"
	.global BtlActGet
	.type BtlActGet, %function
	.thumb_func
BtlActGet:
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
	.incbin "baserom.gba", 0x000b2088, 0x00000008
	.section .rom.000b2090, "ax"
	.global Func_080ad090
	.type Func_080ad090, %function
	.thumb_func
Func_080ad090:
	.incbin "baserom.gba", 0x000b2090, 0x00000008
	.section .rom.000b2098, "ax"
	.global Func_080ad098
	.type Func_080ad098, %function
	.thumb_func
Func_080ad098:
	.incbin "baserom.gba", 0x000b2098, 0x00000008
	.section .rom.000b20a0, "ax"
	.global Func_080ad0a0
	.type Func_080ad0a0, %function
	.thumb_func
Func_080ad0a0:
	.incbin "baserom.gba", 0x000b20a0, 0x00000008
	.section .rom.000b20a8, "ax"
	.global Func_080ad0a8
	.type Func_080ad0a8, %function
	.thumb_func
Func_080ad0a8:
	.incbin "baserom.gba", 0x000b20a8, 0x00000008
	.section .rom.000b20c0, "ax"
	.global Owner_AdjustFirstValueFar
	.type Owner_AdjustFirstValueFar, %function
	.thumb_func
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000b20c0, 0x00000008
	.section .rom.000b20c8, "ax"
	.global UnitEpAddFar
	.type UnitEpAddFar, %function
	.thumb_func
UnitEpAddFar:
	.incbin "baserom.gba", 0x000b20c8, 0x00000008
	.section .rom.000b20d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000b20d0, 0x00000008
	.section .rom.000b20d8, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000b20d8, 0x00000008
	.section .rom.000b20e0, "ax"
	.global Owner_UpdateSecondInputAndRatiosFar
	.type Owner_UpdateSecondInputAndRatiosFar, %function
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000b20e0, 0x00000008
	.section .rom.000b20e8, "ax"
	.global BtlUnitSetFar
	.type BtlUnitSetFar, %function
	.thumb_func
BtlUnitSetFar:
	.incbin "baserom.gba", 0x000b20e8, 0x00000008
	.section .rom.000b20f0, "ax"
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
	.incbin "baserom.gba", 0x000b20f8, 0x00000008
	.section .rom.000b2100, "ax"
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000b2100, 0x00000008
	.section .rom.000b2108, "ax"
	.global RollWeaponUnleashFar
	.type RollWeaponUnleashFar, %function
	.thumb_func
RollWeaponUnleashFar:
	.incbin "baserom.gba", 0x000b2108, 0x00000008
	.section .rom.000b2110, "ax"
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000b2110, 0x00000008
	.section .rom.000b2118, "ax"
	.global Item_GetEquippedElementFar
	.type Item_GetEquippedElementFar, %function
	.thumb_func
Item_GetEquippedElementFar:
	.incbin "baserom.gba", 0x000b2118, 0x00000008
	.section .rom.000b2120, "ax"
	.global BtlHitChk
	.type BtlHitChk, %function
	.thumb_func
BtlHitChk:
	.incbin "baserom.gba", 0x000b2120, 0x00000008
	.section .rom.000b2128, "ax"
	.global BtlAtkCalc
	.type BtlAtkCalc, %function
	.thumb_func
BtlAtkCalc:
	.incbin "baserom.gba", 0x000b2128, 0x00000008
	.section .rom.000b2130, "ax"
	.global BtlPowCalc
	.type BtlPowCalc, %function
	.thumb_func
BtlPowCalc:
	.incbin "baserom.gba", 0x000b2130, 0x00000008
	.section .rom.000b2138, "ax"
	.global BtlRcvCalc
	.type BtlRcvCalc, %function
	.thumb_func
BtlRcvCalc:
	.incbin "baserom.gba", 0x000b2138, 0x00000008
	.section .rom.000b2140, "ax"
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000b2140, 0x00000008
	.section .rom.000b2148, "ax"
	.global BtlRndFar
	.type BtlRndFar, %function
	.thumb_func
BtlRndFar:
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
	.incbin "baserom.gba", 0x000b2158, 0x00000008
	.section .rom.000b2160, "ax"
	.global Djinn_DeactivateFar
	.type Djinn_DeactivateFar, %function
	.thumb_func
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000b2160, 0x00000008
	.section .rom.000b2168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b2168, 0x00000008
	.section .rom.000b2170, "ax"
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000b2170, 0x00000008
	.section .rom.000b2178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000b2178, 0x00000008
	.section .rom.000b2180, "ax"
	.global Trade_ListFlaggedEntriesFar
	.type Trade_ListFlaggedEntriesFar, %function
	.thumb_func
Trade_ListFlaggedEntriesFar:
	.incbin "baserom.gba", 0x000b2180, 0x00000008
	.section .rom.000b2188, "ax"
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000b2188, 0x00000008
	.section .rom.000b2190, "ax"
	.global Djinn_GetDefinitionHeaderFar
	.type Djinn_GetDefinitionHeaderFar, %function
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000b2190, 0x00000008
	.section .rom.000b2198, "ax"
	.global Party_AdvanceOwnerCountToTargetFar
	.type Party_AdvanceOwnerCountToTargetFar, %function
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000b2198, 0x00000008
	.section .rom.000b21a0, "ax"
	.global Owner_GetResistanceValueFar
	.type Owner_GetResistanceValueFar, %function
	.thumb_func
Owner_GetResistanceValueFar:
	.incbin "baserom.gba", 0x000b21a0, 0x00000008
	.section .rom.000b21a8, "ax"
	.global Trade_CountPendingOffersFar
	.type Trade_CountPendingOffersFar, %function
	.thumb_func
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000b21a8, 0x00000008
	.section .rom.000b21b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000b21b0, 0x00000008
	.section .rom.000b21b8, "ax"
	.global Trade_CanOfferDjinnFar
	.type Trade_CanOfferDjinnFar, %function
	.thumb_func
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000b21b8, 0x00000008
	.section .rom.000b21c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b21c0, 0x00000008
	.section .rom.000b21c8, "ax"
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000b21c8, 0x00000008
	.section .rom.000b21d0, "ax"
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000b21d0, 0x00000008
	.section .rom.000b21d8, "ax"
	.global Party_AdjustCoinsFar
	.type Party_AdjustCoinsFar, %function
	.thumb_func
Party_AdjustCoinsFar:
	.incbin "baserom.gba", 0x000b21d8, 0x00000008
	.section .rom.000b21e0, "ax"
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000b21e0, 0x00000008
	.section .rom.000b21e8, "ax"
	.global Item_AdjustCounterFar
	.type Item_AdjustCounterFar, %function
	.thumb_func
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000b21e8, 0x00000008
	.section .rom.000b21f0, "ax"
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x000b21f0, 0x00000008
	.section .rom.000b21f8, "ax"
	.global PartyInventory_HasSpaceFar
	.type PartyInventory_HasSpaceFar, %function
	.thumb_func
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000b21f8, 0x00000008
	.section .rom.000b2200, "ax"
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000b2200, 0x00000008
	.section .rom.000b2208, "ax"
	.global Func_080ad208
	.type Func_080ad208, %function
	.thumb_func
Func_080ad208:
	.incbin "baserom.gba", 0x000b2208, 0x00000008
	.section .rom.000b2210, "ax"
	.global Func_080ad210
	.type Func_080ad210, %function
	.thumb_func
Func_080ad210:
	.incbin "baserom.gba", 0x000b2210, 0x00000008
	.section .rom.000b2218, "ax"
	.global Party_SetLeaderJasmineFar
	.type Party_SetLeaderJasmineFar, %function
	.thumb_func
Party_SetLeaderJasmineFar:
	.incbin "baserom.gba", 0x000b2218, 0x00000008
	.section .rom.000b2220, "ax"
	.global Party_SetLeaderGarciaFar
	.type Party_SetLeaderGarciaFar, %function
	.thumb_func
Party_SetLeaderGarciaFar:
	.incbin "baserom.gba", 0x000b2220, 0x00000008
	.section .rom.000b2228, "ax"
	.global Func_080ad228
	.type Func_080ad228, %function
	.thumb_func
Func_080ad228:
	.incbin "baserom.gba", 0x000b2228, 0x00000008
	.section .rom.000b2230, "ax"
	.global Func_080ad230
	.type Func_080ad230, %function
	.thumb_func
Func_080ad230:
	.incbin "baserom.gba", 0x000b2230, 0x00000008
	.section .rom.000b2238, "ax"
	.global Func_080ad238
	.type Func_080ad238, %function
	.thumb_func
Func_080ad238:
	.incbin "baserom.gba", 0x000b2238, 0x00000008
	.section .rom.000b2240, "ax"
	.global Func_080ad240
	.type Func_080ad240, %function
	.thumb_func
Func_080ad240:
	.incbin "baserom.gba", 0x000b2240, 0x00000008
	.section .rom.000b2248, "ax"
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000b2248, 0x00000008
	.section .rom.000b2250, "ax"
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000b2250, 0x00000008
	.section .rom.000b2258, "ax"
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000b2258, 0x00000008
	.section .rom.000b2260, "ax"
	.global Party_AdjustCounterCappedAt28Far
	.type Party_AdjustCounterCappedAt28Far, %function
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000b2260, 0x00000008
	.section .rom.000b2268, "ax"
	.global Func_080ad268
	.type Func_080ad268, %function
	.thumb_func
Func_080ad268:
	.incbin "baserom.gba", 0x000b2268, 0x00000008
	.section .rom.000b2270, "ax"
	.global Func_080ad270
	.type Func_080ad270, %function
	.thumb_func
Func_080ad270:
	.incbin "baserom.gba", 0x000b2270, 0x00000008
	.section .rom.000b2278, "ax"
	.global Inventory_DiscardFar
	.type Inventory_DiscardFar, %function
	.thumb_func
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000b2278, 0x00000008
	.section .rom.000b2280, "ax"
	.global BtlEfxRevChkFar
	.type BtlEfxRevChkFar, %function
	.thumb_func
BtlEfxRevChkFar:
	.incbin "baserom.gba", 0x000b2280, 0x00000008
	.section .rom.000b2288, "ax"
	.global Owner_RefreshClassActionsFar
	.type Owner_RefreshClassActionsFar, %function
	.thumb_func
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000b2288, 0x00000008
	.section .rom.000b2290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000b2290, 0x00000008
	.section .rom.000b2298, "ax"
	.global Owner_RefreshDerivedDataFar
	.type Owner_RefreshDerivedDataFar, %function
	.thumb_func
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000b2298, 0x00000008
	.section .rom.000b22a0, "ax"
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000b22a0, 0x00000008
	.section .rom.000b22a8, "ax"
	.global PartyInventory_CountItemFar
	.type PartyInventory_CountItemFar, %function
	.thumb_func
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000b22a8, 0x00000008
	.section .rom.000b22b0, "ax"
	.global PartyInventory_DiscardFar
	.type PartyInventory_DiscardFar, %function
	.thumb_func
PartyInventory_DiscardFar:
	.incbin "baserom.gba", 0x000b22b0, 0x00000008
	.section .rom.000b23f6, "ax"
	.incbin "baserom.gba", 0x000b23f6, 0x00000002
	.section .rom.000b23f8, "ax"
	.global UnitCalc
	.type UnitCalc, %function
	.thumb_func
UnitCalc:
	.incbin "baserom.gba", 0x000b23f8, 0x000007f4
	.section .rom.000b2bec, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000b2bec, 0x000000a4
	.section .rom.000b2df0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000b2df0, 0x000002ec
	.section .rom.000b3358, "ax"
	.global Func_080ae358
	.type Func_080ae358, %function
	.thumb_func
Func_080ae358:
	.incbin "baserom.gba", 0x000b3358, 0x000000a4
	.section .rom.000b3410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000b3410, 0x000001ec
	.section .rom.000b35fc, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000b35fc, 0x000000cc
	.section .rom.000b36e4, "ax"
	.global Func_080ae6e4
	.type Func_080ae6e4, %function
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000b36e4, 0x0000014c
	.section .rom.000b3868, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000b3868, 0x000001c8
	.section .rom.000b3c66, "ax"
	.incbin "baserom.gba", 0x000b3c66, 0x00000002
	.section .rom.000b3c68, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b3c68, 0x0000003c
	.section .rom.000b3f34, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x000b3f34, 0x000000d4
	.section .rom.000b40e2, "ax"
	.incbin "baserom.gba", 0x000b40e2, 0x00000002
	.section .rom.000b40e4, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.incbin "baserom.gba", 0x000b40e4, 0x00000064
	.section .rom.000b4148, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000b4148, 0x00000080
	.section .rom.000b44e4, "ax"
	.balign 4
	.global Owner_UpdateClassActions
	.type Owner_UpdateClassActions, %function
	.thumb_func
Owner_UpdateClassActions:
	.incbin "baserom.gba", 0x000b44e4, 0x00000210
	.section .rom.000b47ac, "ax"
	.incbin "baserom.gba", 0x000b47ac, 0x00000120
	.section .rom.000b491c, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b491c, 0x00000264
	.section .rom.000b4b80, "ax"
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000b4b80, 0x00000034
	.section .rom.000b4bec, "ax"
	.incbin "baserom.gba", 0x000b4bec, 0x000001d0
	.section .rom.000b4f28, "ax"
	.incbin "baserom.gba", 0x000b4f28, 0x00000054
	.section .rom.000b4fac, "ax"
	.balign 4
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000b4fac, 0x0000007c
	.section .rom.000b505a, "ax"
	.incbin "baserom.gba", 0x000b505a, 0x00000002
	.section .rom.000b505c, "ax"
	.global Owner_GetDefaultElement
	.type Owner_GetDefaultElement, %function
	.thumb_func
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000b505c, 0x00000028
	.section .rom.000b5084, "ax"
	.global Owner_CalculateBaseElements
	.type Owner_CalculateBaseElements, %function
	.thumb_func
Owner_CalculateBaseElements:
	.incbin "baserom.gba", 0x000b5084, 0x000000c0
	.section .rom.000b5144, "ax"
	.global Owner_DetermineClass
	.type Owner_DetermineClass, %function
	.thumb_func
Owner_DetermineClass:
	.incbin "baserom.gba", 0x000b5144, 0x00000144
	.section .rom.000b5514, "ax"
	.incbin "baserom.gba", 0x000b5514, 0x00000194
	.section .rom.000b575c, "ax"
	.align 2, 0
	.incbin "baserom.gba", 0x000b575c, 0x00000338
	.section .rom.000b5bb6, "ax"
	.incbin "baserom.gba", 0x000b5bb6, 0x00000002
	.section .rom.000b5bb8, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000b5bb8, 0x000000c0
	.section .rom.000b5d58, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b5d58, 0x000000ac
	.section .rom.000b6088, "ax"
	.balign 4
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000b6088, 0x00000170
	.section .rom.000b61f8, "ax"
	.global Func_080b11f8
	.type Func_080b11f8, %function
	.thumb_func
Func_080b11f8:
	.incbin "baserom.gba", 0x000b61f8, 0x00000084
	.section .rom.000b627c, "ax"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000b627c, 0x0000004c
	.section .rom.000b6f28, "ax"
	.incbin "baserom.gba", 0x000b6f28, 0x00000004
	.section .rom.000cbb04, "ax"
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000cbb04, 0x00000014
	.section .rom.000cbb18, "ax"
	.global Battle_Effect90Classes
Battle_Effect90Classes:
	.incbin "baserom.gba", 0x000cbb18, 0x0000005e
	.section .rom.000cbb76, "ax"
	.global Battle_Effect91Classes
Battle_Effect91Classes:
	.incbin "baserom.gba", 0x000cbb76, 0x0000003a
	.section .rom.000cd000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000cd000, 0x00000008
	.section .rom.000cd5c0, "ax"
	.global Func_080c85c0
	.type Func_080c85c0, %function
	.thumb_func
Func_080c85c0:
	.incbin "baserom.gba", 0x000cd5c0, 0x00000008
	.section .rom.000ce694, "ax"
	.incbin "baserom.gba", 0x000ce694, 0x0000028c
	.section .rom.000ce944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000ce944, 0x000002cc
	.section .rom.000cec38, "ax"
	.incbin "baserom.gba", 0x000cec38, 0x00000190
	.section .rom.000cee48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cee48, 0x000000e4
	.section .rom.000cef2c, "ax"
	.global BattleFx_LookupResult
	.type BattleFx_LookupResult, %function
	.thumb_func
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000cef2c, 0x000000ac
	.section .rom.000cefd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000cefd8, 0x0000018c
	.section .rom.000cf2fc, "ax"
	.incbin "baserom.gba", 0x000cf2fc, 0x0000006c
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
	.section .rom.000cf4a8, "ax"
	.incbin "baserom.gba", 0x000cf4a8, 0x000000f8
	.section .rom.000cf5d8, "ax"
	.global BattleFx_SelectBattleCue
	.type BattleFx_SelectBattleCue, %function
	.thumb_func
BattleFx_SelectBattleCue:
	.incbin "baserom.gba", 0x000cf5d8, 0x000000cc
	.section .rom.000cf6a4, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000cf6a4, 0x00000040
	.section .rom.000cf6e8, "ax"
	.balign 4
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
	.section .rom.000cfe5c, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000cfe5c, 0x00000110
	.section .rom.000d009c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d009c, 0x0000062c
	.section .rom.000d091c, "ax"
	.incbin "baserom.gba", 0x000d091c, 0x00000c30
	.section .rom.000d154c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d154c, 0x00000130
	.section .rom.000d167c, "ax"
	.global Func_080cc67c
Func_080cc67c:
	.incbin "baserom.gba", 0x000d167c, 0x00000148
	.section .rom.000d17c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d17c4, 0x000001d0
	.section .rom.000d19fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d19fc, 0x000002bc
	.section .rom.000d1cb8, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000d1cb8, 0x00000090
	.section .rom.000d1d76, "ax"
	.incbin "baserom.gba", 0x000d1d76, 0x00000002
	.section .rom.000d1d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1d78, 0x0000010c
	.section .rom.000d1e94, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d1e94, 0x00000034
	.section .rom.000d1ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d1ec8, 0x000006bc
	.section .rom.000d25bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d25bc, 0x00000360
	.section .rom.000d291c, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000d291c, 0x0000013c
	.section .rom.000d2a84, "ax"
	.balign 4
	.global Func_080cda84
Func_080cda84:
	.incbin "baserom.gba", 0x000d2a84, 0x0000003c
	.section .rom.000d2ac0, "ax"
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000d2ac0, 0x00000138
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
	.incbin "baserom.gba", 0x000d2d80, 0x00000128
	.section .rom.000d2fb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d2fb0, 0x0000036c
	.section .rom.000d331c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000d331c, 0x0000013c
	.section .rom.000d3458, "ax"
	.global Func_080ce458
	.type Func_080ce458, %function
	.thumb_func
Func_080ce458:
	.incbin "baserom.gba", 0x000d3458, 0x0000011c
	.section .rom.000d361c, "ax"
	.incbin "baserom.gba", 0x000d361c, 0x000004e0
	.section .rom.000d3ba8, "ax"
	.incbin "baserom.gba", 0x000d3ba8, 0x0000007c
	.section .rom.000d3c34, "ax"
	.incbin "baserom.gba", 0x000d3c34, 0x0000020c
	.section .rom.000d4050, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000d4050, 0x00000080
	.section .rom.000d4158, "ax"
	.incbin "baserom.gba", 0x000d4158, 0x00000024
	.section .rom.000d417c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d417c, 0x000001c4
	.section .rom.000d4350, "ax"
	.balign 4
	.global Func_080cf350
	.type Func_080cf350, %function
	.thumb_func
Func_080cf350:
	.incbin "baserom.gba", 0x000d4350, 0x00000064
	.section .rom.000d4554, "ax"
	.incbin "baserom.gba", 0x000d4554, 0x000001a8
	.section .rom.000d46fc, "ax"
	.global Func_080cf6fc
	.type Func_080cf6fc, %function
	.thumb_func
Func_080cf6fc:
	.incbin "baserom.gba", 0x000d46fc, 0x00000090
	.section .rom.000d478c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000d478c, 0x0000096c
	.section .rom.000d51cc, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d51cc, 0x00000354
	.section .rom.000d5520, "ax"
	.global DisplayTransition_Finish
	.type DisplayTransition_Finish, %function
	.thumb_func
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d5520, 0x0000020c
	.section .rom.000d5788, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d5788, 0x000001cc
	.section .rom.000d5954, "ax"
	.global Func_080d0954
	.type Func_080d0954, %function
	.thumb_func
Func_080d0954:
	.incbin "baserom.gba", 0x000d5954, 0x000000d4
	.section .rom.000d5a28, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d5a28, 0x00000154
	.section .rom.000d5b7c, "ax"
	.global DisplayTransition_FillTilemapAndSolidTile
	.type DisplayTransition_FillTilemapAndSolidTile, %function
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d5b7c, 0x00000070
	.section .rom.000d5ca0, "ax"
	.global BattlePalette_UpdateBlend
	.type BattlePalette_UpdateBlend, %function
	.thumb_func
BattlePalette_UpdateBlend:
	.incbin "baserom.gba", 0x000d5ca0, 0x0000017c
	.section .rom.000d5e1c, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x000d5e1c, 0x00000868
	.section .rom.000d6760, "ax"
	.balign 4
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d6760, 0x0000004c
	.section .rom.000d683e, "ax"
	.incbin "baserom.gba", 0x000d683e, 0x00000002
	.section .rom.000d6840, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000d6840, 0x000001d8
	.section .rom.000d6a18, "ax"
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000d6a18, 0x000000ac
	.section .rom.000d6ae4, "ax"
	.global Func_080d1ae4
	.type Func_080d1ae4, %function
	.thumb_func
Func_080d1ae4:
	.incbin "baserom.gba", 0x000d6ae4, 0x000001dc
	.section .rom.000d6dac, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d6dac, 0x00000044
	.section .rom.000d6e18, "ax"
	.balign 4
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000d6e18, 0x00000048
	.section .rom.000d6e60, "ax"
	.global Func_080d1e60
	.type Func_080d1e60, %function
	.thumb_func
Func_080d1e60:
	.incbin "baserom.gba", 0x000d6e60, 0x00000024
	.section .rom.000d6ee8, "ax"
	.balign 4
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d6ee8, 0x00000024
	.section .rom.000d6f20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000d6f20, 0x00000164
	.section .rom.000d70fc, "ax"
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000d70fc, 0x000000f8
	.section .rom.000d760c, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d760c, 0x000001d4
	.section .rom.000d7834, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d7834, 0x0000011c
	.section .rom.000d7960, "ax"
	.balign 4
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000d7960, 0x000000a0
	.section .rom.000d7b40, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
	.align 2, 0
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d7b40, 0x000000b8
	.section .rom.000d810a, "ax"
	.incbin "baserom.gba", 0x000d810a, 0x00000002
	.section .rom.000d810c, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.type ObjectMotion_SetHorizontalPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
	.incbin "baserom.gba", 0x000d810c, 0x0000004c
	.section .rom.000d8158, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrain
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrain:
	.incbin "baserom.gba", 0x000d8158, 0x0000005c
	.section .rom.000d81b4, "ax"
	.global ObjectMotion_SetPositionWithTerrain
	.type ObjectMotion_SetPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrain:
	.incbin "baserom.gba", 0x000d81b4, 0x00000054
	.section .rom.000d8208, "ax"
	.global ObjectMotion_MoveToObject
	.type ObjectMotion_MoveToObject, %function
	.thumb_func
ObjectMotion_MoveToObject:
	.incbin "baserom.gba", 0x000d8208, 0x0000002c
	.section .rom.000d825c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d825c, 0x00000050
	.section .rom.000d836a, "ax"
	.incbin "baserom.gba", 0x000d836a, 0x00000002
	.section .rom.000d836c, "ax"
	.incbin "baserom.gba", 0x000d836c, 0x000000e8
	.section .rom.000d8454, "ax"
	.global BattleFx_RunRisingObjectSequence
	.type BattleFx_RunRisingObjectSequence, %function
	.thumb_func
BattleFx_RunRisingObjectSequence:
	.incbin "baserom.gba", 0x000d8454, 0x0000013c
	.section .rom.000d8774, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000d8774, 0x00000054
	.section .rom.000d8934, "ax"
	.balign 4
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
	.section .rom.000d8c7c, "ax"
	.balign 4
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
	.section .rom.000d90d0, "ax"
	.balign 4
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000d90d0, 0x0000009c
	.section .rom.000d91da, "ax"
	.incbin "baserom.gba", 0x000d91da, 0x00000002
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
	.section .rom.000d9400, "ax"
	.balign 4
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d9400, 0x00000100
	.section .rom.000d97a8, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000d97a8, 0x000000b4
	.section .rom.000d9890, "ax"
	.balign 4
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000d9890, 0x000000f0
	.section .rom.000d9bb8, "ax"
	.incbin "baserom.gba", 0x000d9bb8, 0x00000108
	.section .rom.000d9cfc, "ax"
	.balign 4
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
	.section .rom.000dae6c, "ax"
	.balign 4
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000dae6c, 0x000000a0
	.section .rom.000db070, "ax"
	.global Func_080d607c
	.type Func_080d607c, %function
	.thumb_func
Func_080d607c:
	.incbin "baserom.gba", 0x000db070, 0x0000038c
	.section .rom.000db4e4, "ax"
	.incbin "baserom.gba", 0x000db4e4, 0x00000184
	.section .rom.000db668, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000db668, 0x00000174
	.section .rom.000db7dc, "ax"
	.global Func_080d67e8
	.type Func_080d67e8, %function
	.thumb_func
Func_080d67e8:
	.incbin "baserom.gba", 0x000db7dc, 0x00000124
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
	.section .rom.000dc028, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000dc028, 0x0000020c
	.section .rom.000dc518, "ax"
	.incbin "baserom.gba", 0x000dc518, 0x00000264
	.section .rom.000dc77c, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000dc77c, 0x000002d0
	.section .rom.000dcaf8, "ax"
	.incbin "baserom.gba", 0x000dcaf8, 0x00000244
	.section .rom.000dcd84, "ax"
	.incbin "baserom.gba", 0x000dcd84, 0x0000097c
	.section .rom.000dd734, "ax"
	.incbin "baserom.gba", 0x000dd734, 0x0000023c
	.section .rom.000dd9f4, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000dd9f4, 0x00000210
	.section .rom.000ddc04, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000ddc04, 0x00000130
	.section .rom.000ddd5c, "ax"
	.balign 4
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000ddd5c, 0x00000240
	.section .rom.000ddf9c, "ax"
	.global Func_080d8fa8
	.type Func_080d8fa8, %function
	.thumb_func
Func_080d8fa8:
	.incbin "baserom.gba", 0x000ddf9c, 0x0000015c
	.section .rom.000de0f8, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000de0f8, 0x000001a0
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
	.incbin "baserom.gba", 0x000de704, 0x00000268
	.section .rom.000deaa4, "ax"
	.balign 4
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000deaa4, 0x00000020
	.section .rom.000deb74, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000deb74, 0x000001c0
	.section .rom.000ded34, "ax"
	.global Func_080d9d40
	.type Func_080d9d40, %function
	.thumb_func
Func_080d9d40:
	.incbin "baserom.gba", 0x000ded34, 0x00000134
	.section .rom.000dee8c, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000dee8c, 0x000001c8
	.section .rom.000df054, "ax"
	.global Func_080da060
	.type Func_080da060, %function
	.thumb_func
Func_080da060:
	.incbin "baserom.gba", 0x000df054, 0x000008a8
	.section .rom.000df99c, "ax"
	.incbin "baserom.gba", 0x000df99c, 0x00000500
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
	.section .rom.000e099a, "ax"
	.incbin "baserom.gba", 0x000e099a, 0x00000002
	.section .rom.000e0a50, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e0a50, 0x000000e4
	.section .rom.000e0b6c, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.incbin "baserom.gba", 0x000e0b6c, 0x0000008c
	.section .rom.000e0bf8, "ax"
	.global Func_080dbc04
Func_080dbc04:
	.incbin "baserom.gba", 0x000e0bf8, 0x000000a4
	.section .rom.000e0c9c, "ax"
	.global Func_080dbca8
	.type Func_080dbca8, %function
	.thumb_func
Func_080dbca8:
	.incbin "baserom.gba", 0x000e0c9c, 0x00000018
	.section .rom.000e0cb4, "ax"
	.global Func_080dbcc0
Func_080dbcc0:
	.incbin "baserom.gba", 0x000e0cb4, 0x00000018
	.section .rom.000e0ccc, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000e0ccc, 0x00000070
	.section .rom.000e0d50, "ax"
	.balign 4
	.global Func_080dbd5c
Func_080dbd5c:
	.incbin "baserom.gba", 0x000e0d50, 0x0000004c
	.section .rom.000e0ec4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e0ec4, 0x00000174
	.section .rom.000e1288, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e1288, 0x000000e8
	.section .rom.000e1404, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x000e1404, 0x0000021c
	.section .rom.000e16cc, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e16cc, 0x000000f4
	.section .rom.000e17dc, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000e17dc, 0x0000016c
	.section .rom.000e196c, "ax"
	.balign 4
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000e196c, 0x000000d8
	.section .rom.000e1ad0, "ax"
	.balign 4
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000e1ad0, 0x00000068
	.section .rom.000e1b38, "ax"
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000e1b38, 0x00000284
	.section .rom.000e2048, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000e2048, 0x000003e8
	.section .rom.000e251a, "ax"
	.incbin "baserom.gba", 0x000e251a, 0x00000002
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
	.incbin "baserom.gba", 0x000e265c, 0x000000e0
	.section .rom.000e2748, "ax"
	.incbin "baserom.gba", 0x000e2748, 0x000001fc
	.section .rom.000e2944, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000e2944, 0x000001ec
	.section .rom.000e2bcc, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000e2bcc, 0x0000063c
	.section .rom.000e3210, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000e3210, 0x00000334
	.section .rom.000e367c, "ax"
	.global BattleFx_RunOrbitingParticles
	.type BattleFx_RunOrbitingParticles, %function
	.thumb_func
BattleFx_RunOrbitingParticles:
	.incbin "baserom.gba", 0x000e367c, 0x00000190
	.section .rom.000e3858, "ax"
	.incbin "baserom.gba", 0x000e3858, 0x00000188
	.section .rom.000e39ec, "ax"
	.global Func_080de9f8
	.type Func_080de9f8, %function
	.thumb_func
Func_080de9f8:
	.incbin "baserom.gba", 0x000e39ec, 0x000001ac
	.section .rom.000e3bcc, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000e3bcc, 0x000000b4
	.section .rom.000e3cac, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000e3cac, 0x000000cc
	.section .rom.000e3ddc, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000e3ddc, 0x00000ac0
	.section .rom.000e48d2, "ax"
	.incbin "baserom.gba", 0x000e48d2, 0x00000002
	.section .rom.000e48d4, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000e48d4, 0x000001d4
	.section .rom.000e4c30, "ax"
	.incbin "baserom.gba", 0x000e4c30, 0x00000098
	.section .rom.000e4cf4, "ax"
	.global RunBattleEffect14
RunBattleEffect14:
	.incbin "baserom.gba", 0x000e4cf4, 0x000002fc
	.section .rom.000e5014, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e5014, 0x000000fc
	.section .rom.000e5128, "ax"
	.global Func_080e0134
	.type Func_080e0134, %function
	.thumb_func
Func_080e0134:
	.incbin "baserom.gba", 0x000e5128, 0x000001b8
	.section .rom.000e53b6, "ax"
	.incbin "baserom.gba", 0x000e53b6, 0x00000002
	.section .rom.000e53c0, "ax"
	.balign 4
	.global Func_080e03cc
	.type Func_080e03cc, %function
	.thumb_func
Func_080e03cc:
	.incbin "baserom.gba", 0x000e53c0, 0x0000024c
	.section .rom.000e560c, "ax"
	.incbin "baserom.gba", 0x000e560c, 0x00000360
	.section .rom.000e59b4, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000e59b4, 0x000002c4
	.section .rom.000e5ca0, "ax"
	.global Func_080e0cac
	.type Func_080e0cac, %function
	.thumb_func
Func_080e0cac:
	.incbin "baserom.gba", 0x000e5ca0, 0x00000040
	.section .rom.000e5ce0, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000e5ce0, 0x000000e8
	.section .rom.000e5f34, "ax"
	.incbin "baserom.gba", 0x000e5f34, 0x00000214
	.section .rom.000e6148, "ax"
	.global Func_080e1154
	.type Func_080e1154, %function
	.thumb_func
Func_080e1154:
	.incbin "baserom.gba", 0x000e6148, 0x00000068
	.section .rom.000e643c, "ax"
	.global BattleFx_SpawnScaledArcObjects
	.type BattleFx_SpawnScaledArcObjects, %function
	.thumb_func
BattleFx_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000e643c, 0x000000c4
	.section .rom.000e6500, "ax"
	.global BattleFx_UpdateArcEffect
	.type BattleFx_UpdateArcEffect, %function
	.thumb_func
BattleFx_UpdateArcEffect:
	.incbin "baserom.gba", 0x000e6500, 0x000000f0
	.section .rom.000e666c, "ax"
	.incbin "baserom.gba", 0x000e666c, 0x000008ac
	.section .rom.000e6f20, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000e6f20, 0x000006bc
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
	.incbin "baserom.gba", 0x000e78c8, 0x00000364
	.section .rom.000e7c2c, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000e7c2c, 0x00000108
	.section .rom.000e7d58, "ax"
	.incbin "baserom.gba", 0x000e7d58, 0x000002fc
	.section .rom.000e8068, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000e8068, 0x00000624
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
	.incbin "baserom.gba", 0x000e9238, 0x00000090
	.section .rom.000e9398, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e9398, 0x000001fc
	.section .rom.000e959c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e959c, 0x00000188
	.section .rom.000e9724, "ax"
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000e9724, 0x000002fc
	.section .rom.000e9a28, "ax"
	.balign 4
	.global Func_080e9af4
	.type Func_080e9af4, %function
	.thumb_func
Func_080e9af4:
	.incbin "baserom.gba", 0x000e9a28, 0x000012bc
	.section .rom.000eace4, "ax"
	.global Func_080e5cf0
Func_080e5cf0:
	.incbin "baserom.gba", 0x000eace4, 0x00000698
	.section .rom.000eb37c, "ax"
	.global Func_080e6388
Func_080e6388:
	.incbin "baserom.gba", 0x000eb37c, 0x00000d70
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
	.incbin "baserom.gba", 0x000ec7f8, 0x00000fc0
	.section .rom.000ed7b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000ed7b8, 0x000005bc
	.section .rom.000edd74, "ax"
	.global Func_080e8d80
Func_080e8d80:
	.incbin "baserom.gba", 0x000edd74, 0x000004d0
	.section .rom.000ee244, "ax"
	.global Func_080e9250
Func_080e9250:
	.incbin "baserom.gba", 0x000ee244, 0x00000b64
	.section .rom.000eeda8, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000eeda8, 0x00000130
	.section .rom.000eef5c, "ax"
	.incbin "baserom.gba", 0x000eef5c, 0x000001e4
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
	.incbin "baserom.gba", 0x000ef7b4, 0x000000e8
	.section .rom.000ef89c, "ax"
	.global Func_080ea8a8
Func_080ea8a8:
	.incbin "baserom.gba", 0x000ef89c, 0x0000002c
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
	.section .rom.000efb8c, "ax"
	.balign 4
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
	.section .rom.000f030c, "ax"
	.incbin "baserom.gba", 0x000f030c, 0x00000204
	.section .rom.000f0588, "ax"
	.global Func_080eb594
	.type Func_080eb594, %function
	.thumb_func
Func_080eb594:
	.incbin "baserom.gba", 0x000f0588, 0x0000010c
	.section .rom.000f0694, "ax"
	.global Func_080eb6a0
	.type Func_080eb6a0, %function
	.thumb_func
Func_080eb6a0:
	.incbin "baserom.gba", 0x000f0694, 0x00000184
	.section .rom.000f0818, "ax"
	.incbin "baserom.gba", 0x000f0818, 0x0000010c
	.section .rom.000f0954, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f0954, 0x000002d0
	.section .rom.000f0d18, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000f0d18, 0x0000014c
	.section .rom.000f0ebc, "ax"
	.global EffectSlot_Initialize
	.type EffectSlot_Initialize, %function
	.thumb_func
EffectSlot_Initialize:
	.incbin "baserom.gba", 0x000f0ebc, 0x000000a0
	.section .rom.000f0f88, "ax"
	.incbin "baserom.gba", 0x000f0f88, 0x000001b8
	.section .rom.000f11c4, "ax"
	.incbin "baserom.gba", 0x000f11c4, 0x000000c8
	.section .rom.000f12ac, "ax"
	.incbin "baserom.gba", 0x000f12ac, 0x000014d0
	.section .rom.000f2a00, "ax"
	.incbin "baserom.gba", 0x000f2a00, 0x000000c0
	.section .rom.000f4810, "ax"
	.incbin "baserom.gba", 0x000f4810, 0x00000008
	.section .rom.000f51ac, "ax"
	.incbin "baserom.gba", 0x000f51ac, 0x0000000c
	.section .rom.000f52fa, "ax"
	.incbin "baserom.gba", 0x000f52fa, 0x00000002
	.section .rom.000f5b44, "ax"
	.incbin "baserom.gba", 0x000f5b44, 0x0000002c
	.section .rom.000f5bf0, "ax"
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000f5bf0, 0x00000008
	.section .rom.000f5bf8, "ax"
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000f5bf8, 0x00000250
	.section .rom.000f5e48, "ax"
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f5e48, 0x00000024
	.section .rom.000f5e6c, "ax"
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000f5e6c, 0x00000024
	.section .rom.000f5e90, "ax"
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000f5e90, 0x00000008
	.section .rom.000f5eb8, "ax"
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f5eb8, 0x00000024
	.section .rom.000f5edc, "ax"
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000f5edc, 0x00000044
	.section .rom.000f5f20, "ax"
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f5f20, 0x000001d4
	.section .rom.000f6114, "ax"
	.incbin "baserom.gba", 0x000f6114, 0x00000020
	.section .rom.000f6134, "ax"
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000f6134, 0x00000668
	.section .rom.000f71c4, "ax"
	.incbin "baserom.gba", 0x000f71c4, 0x00000034
	.section .rom.000f8210, "ax"
	.incbin "baserom.gba", 0x000f8210, 0x000000b4
	.section .rom.000f82c4, "ax"
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f82c4, 0x00000010
	.section .rom.000f82d4, "ax"
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x000f82d4, 0x00000020
	.section .rom.000f82f4, "ax"
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f82f4, 0x00000128
	.section .rom.000f841c, "ax"
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x000f841c, 0x000000bc
	.section .rom.000f84d8, "ax"
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x000f84d8, 0x00000004
	.section .rom.000f84dc, "ax"
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x000f84dc, 0x0000000c
	.section .rom.000f84e8, "ax"
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x000f84e8, 0x0000000c
	.section .rom.000f84f4, "ax"
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x000f84f4, 0x0000000c
	.section .rom.000f8500, "ax"
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x000f8500, 0x000000bc
	.section .rom.000f85bc, "ax"
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x000f85bc, 0x0000004c
	.section .rom.000f8608, "ax"
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x000f8608, 0x00000128
	.section .rom.000f8730, "ax"
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x000f8730, 0x00000014
	.section .rom.000f8744, "ax"
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x000f8744, 0x00000014
	.section .rom.000f8758, "ax"
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x000f8758, 0x0000001c
	.section .rom.000f8774, "ax"
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f8774, 0x00000018
	.section .rom.000f8852, "ax"
	.incbin "baserom.gba", 0x000f8852, 0x00000054
	.section .rom.000f88f5, "ax"
	.incbin "baserom.gba", 0x000f88f5, 0x00000043
	.section .rom.000f8938, "ax"
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x000f8938, 0x0000000c
	.section .rom.000f8944, "ax"
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x000f8944, 0x00000024
	.section .rom.000f8968, "ax"
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000f8968, 0x00000050
	.section .rom.000fa028, "ax"
	.global UiIcon_PrepareObjectFar
	.type UiIcon_PrepareObjectFar, %function
	.thumb_func
UiIcon_PrepareObjectFar:
	.incbin "baserom.gba", 0x000fa028, 0x00000008
	.section .rom.000fa030, "ax"
	.global ItemMenu_Open
	.type ItemMenu_Open, %function
	.thumb_func
ItemMenu_Open:
	.incbin "baserom.gba", 0x000fa030, 0x00000008
	.section .rom.000fa038, "ax"
	.global ItemMenu_DrawItemDetailsFar
	.type ItemMenu_DrawItemDetailsFar, %function
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	.incbin "baserom.gba", 0x000fa038, 0x00000008
	.section .rom.000fa040, "ax"
	.global Func_080f8040
	.type Func_080f8040, %function
	.thumb_func
Func_080f8040:
	.incbin "baserom.gba", 0x000fa040, 0x00000008
	.section .rom.000fa048, "ax"
	.global Palette_CopyObjectBankToBackground14Far
	.type Palette_CopyObjectBankToBackground14Far, %function
	.thumb_func
Palette_CopyObjectBankToBackground14Far:
	.incbin "baserom.gba", 0x000fa048, 0x00000008
	.section .rom.000fa050, "ax"
	.global Menu_UpdateEntryObjectTransformsFar
	.type Menu_UpdateEntryObjectTransformsFar, %function
	.thumb_func
Menu_UpdateEntryObjectTransformsFar:
	.incbin "baserom.gba", 0x000fa050, 0x00000020
	.section .rom.000fa170, "ax"
	.incbin "baserom.gba", 0x000fa170, 0x000004a0
	.section .rom.000fa888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fa888, 0x0000003c
	.section .rom.000fa8d0, "ax"
	.global Func_080f88d0
	.type Func_080f88d0, %function
	.thumb_func
Func_080f88d0:
	.incbin "baserom.gba", 0x000fa8d0, 0x000001e4
	.section .rom.000faab4, "ax"
	.global Func_080f8ab4
	.type Func_080f8ab4, %function
	.thumb_func
Func_080f8ab4:
	.incbin "baserom.gba", 0x000faab4, 0x00000108
	.section .rom.000face6, "ax"
	.incbin "baserom.gba", 0x000face6, 0x00000002
	.section .rom.000face8, "ax"
	.global Func_080f8ce8
	.type Func_080f8ce8, %function
	.thumb_func
Func_080f8ce8:
	.incbin "baserom.gba", 0x000face8, 0x00000258
	.section .rom.000faf9c, "ax"
	.incbin "baserom.gba", 0x000faf9c, 0x0000016c
	.section .rom.000fb2dc, "ax"
	.incbin "baserom.gba", 0x000fb2dc, 0x00000098
	.section .rom.000fb3b6, "ax"
	.incbin "baserom.gba", 0x000fb3b6, 0x00000002
	.section .rom.000fb4a2, "ax"
	.incbin "baserom.gba", 0x000fb4a2, 0x00000002
	.section .rom.000fb4a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000fb4a4, 0x00000f4c
	.section .rom.000fc494, "ax"
	.incbin "baserom.gba", 0x000fc494, 0x0000003c
	.section .rom.000fc5fc, "ax"
	.global Func_080fa5a4
	.type Func_080fa5a4, %function
	.thumb_func
Func_080fa5a4:
	.incbin "baserom.gba", 0x000fc5fc, 0x000002a8
	.section .rom.000fc8c8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fc8c8, 0x00000370
	.section .rom.000fccb0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fccb0, 0x00000048
	.section .rom.000fcd2e, "ax"
	.incbin "baserom.gba", 0x000fcd2e, 0x00000002
	.section .rom.000fcd30, "ax"
	.global Menu_HideEmptyEntryIcons
	.type Menu_HideEmptyEntryIcons, %function
	.thumb_func
Menu_HideEmptyEntryIcons:
	.incbin "baserom.gba", 0x000fcd30, 0x00000044
	.section .rom.000fcee4, "ax"
	.incbin "baserom.gba", 0x000fcee4, 0x00000218
	.section .rom.000fd15c, "ax"
	.incbin "baserom.gba", 0x000fd15c, 0x00000544
	.section .rom.000fd7e8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fd7e8, 0x0000012c
	.section .rom.000fd920, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x000fd920, 0x0000047c
	.section .rom.000fded4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fded4, 0x00000618
	.section .rom.000fe5c4, "ax"
	.incbin "baserom.gba", 0x000fe5c4, 0x00000594
	.section .rom.000feb58, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000feb58, 0x00000854
	.section .rom.000ff400, "ax"
	.incbin "baserom.gba", 0x000ff400, 0x00000244
	.section .rom.000ff674, "ax"
	.incbin "baserom.gba", 0x000ff674, 0x00000188
	.section .rom.000ff83c, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x000ff83c, 0x00000a78
	.section .rom.001003c4, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x001003c4, 0x0000030c
	.section .rom.001006d0, "ax"
	.global Func_080fe580
	.type Func_080fe580, %function
	.thumb_func
Func_080fe580:
	.incbin "baserom.gba", 0x001006d0, 0x00000884
	.section .rom.00100f90, "ax"
	.incbin "baserom.gba", 0x00100f90, 0x000003b4
	.section .rom.001014c0, "ax"
	.incbin "baserom.gba", 0x001014c0, 0x00000570
	.section .rom.00101a9c, "ax"
	.incbin "baserom.gba", 0x00101a9c, 0x00000b8c
	.section .rom.001026b0, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x001026b0, 0x000000e4
	.section .rom.001028a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001028a8, 0x00000608
	.section .rom.00102fec, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00102fec, 0x00000734
	.section .rom.001037a8, "ax"
	.incbin "baserom.gba", 0x001037a8, 0x00000490
	.section .rom.00103c38, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00103c38, 0x00000144
	.section .rom.00103db0, "ax"
	.incbin "baserom.gba", 0x00103db0, 0x0000003c
	.section .rom.00103dec, "ax"
	.global Func_08101c7c
	.type Func_08101c7c, %function
	.thumb_func
Func_08101c7c:
	.incbin "baserom.gba", 0x00103dec, 0x000000b8
	.section .rom.00103ecc, "ax"
	.incbin "baserom.gba", 0x00103ecc, 0x00002ce4
	.section .rom.00106cae, "ax"
	.incbin "baserom.gba", 0x00106cae, 0x00000002
	.section .rom.00106cb0, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x00106cb0, 0x00000174
	.section .rom.00106eb4, "ax"
	.incbin "baserom.gba", 0x00106eb4, 0x0000004c
	.section .rom.00106f00, "ax"
	.global Menu_UpdateEntryObjectTransforms
	.type Menu_UpdateEntryObjectTransforms, %function
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	.incbin "baserom.gba", 0x00106f00, 0x00000238
	.section .rom.00107138, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x00107138, 0x000000ac
	.section .rom.00107224, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x00107224, 0x000000dc
	.section .rom.00107300, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x00107300, 0x000000c4
	.section .rom.00107500, "ax"
	.incbin "baserom.gba", 0x00107500, 0x000000c0
	.section .rom.00107638, "ax"
	.incbin "baserom.gba", 0x00107638, 0x000000a0
	.section .rom.0010774e, "ax"
	.incbin "baserom.gba", 0x0010774e, 0x00000346
	.section .rom.00107a94, "ax"
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x00107a94, 0x00000004
	.section .rom.00107a98, "ax"
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x00107a98, 0x00000004
	.section .rom.00107a9c, "ax"
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x00107a9c, 0x00000004
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
	.incbin "baserom.gba", 0x00107ac8, 0x00000014
	.section .rom.00107b0c, "ax"
	.incbin "baserom.gba", 0x00107b0c, 0x0000003a
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
	.section .rom.00107ba8, "ax"
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x00107ba8, 0x00000008
	.section .rom.00107bb0, "ax"
	.global Data_08105a40
Data_08105a40:
	.incbin "baserom.gba", 0x00107bb0, 0x0000001c
	.section .rom.00108148, "ax"
	.global Func_08108148
	.type Func_08108148, %function
	.thumb_func
Func_08108148:
	.incbin "baserom.gba", 0x00108148, 0x00000104
	.section .rom.0010824c, "ax"
	.global Func_0810824c
	.type Func_0810824c, %function
	.thumb_func
Func_0810824c:
	.incbin "baserom.gba", 0x0010824c, 0x00000078
	.section .rom.001082c4, "ax"
	.global Func_081082c4
	.type Func_081082c4, %function
	.thumb_func
Func_081082c4:
	.incbin "baserom.gba", 0x001082c4, 0x000001a0
	.section .rom.00108690, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00108690, 0x000000cc
	.section .rom.00108948, "ax"
	.balign 4
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
	.section .rom.00108aa8, "ax"
	.balign 4
	.global Func_08108aa8
	.type Func_08108aa8, %function
	.thumb_func
Func_08108aa8:
	.incbin "baserom.gba", 0x00108aa8, 0x00000048
	.section .rom.00108b70, "ax"
	.incbin "baserom.gba", 0x00108b70, 0x00000618
	.section .rom.001091cc, "ax"
	.incbin "baserom.gba", 0x001091cc, 0x000000a4
	.section .rom.0010928c, "ax"
	.incbin "baserom.gba", 0x0010928c, 0x00000398
	.section .rom.001096f8, "ax"
	.global Func_081096f8
	.type Func_081096f8, %function
	.thumb_func
Func_081096f8:
	.incbin "baserom.gba", 0x001096f8, 0x000001c8
	.section .rom.00109ad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00109ad8, 0x000001d4
	.section .rom.00109cac, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x00109cac, 0x0000035c
	.section .rom.0010a040, "ax"
	.incbin "baserom.gba", 0x0010a040, 0x000000cc
	.section .rom.0010a10c, "ax"
	.global Func_0810a108
	.type Func_0810a108, %function
	.thumb_func
Func_0810a108:
	.incbin "baserom.gba", 0x0010a10c, 0x000001a4
	.section .rom.0010a2dc, "ax"
	.incbin "baserom.gba", 0x0010a2dc, 0x00000394
	.section .rom.0010a674, "ax"
	.incbin "baserom.gba", 0x0010a674, 0x00000088
	.section .rom.0010a710, "ax"
	.global Forge_FindRecipe
	.type Forge_FindRecipe, %function
	.thumb_func
Forge_FindRecipe:
	.incbin "baserom.gba", 0x0010a710, 0x0000003c
	.section .rom.0010a8f0, "ax"
	.incbin "baserom.gba", 0x0010a8f0, 0x00000074
	.section .rom.0010a9b0, "ax"
	.incbin "baserom.gba", 0x0010a9b0, 0x000000d4
	.section .rom.0010aa84, "ax"
	.global Func_0810aa80
	.type Func_0810aa80, %function
	.thumb_func
Func_0810aa80:
	.incbin "baserom.gba", 0x0010aa84, 0x00000424
	.section .rom.0010af28, "ax"
	.incbin "baserom.gba", 0x0010af28, 0x00000198
	.section .rom.0010b1b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010b1b8, 0x000001c4
	.section .rom.0010b37c, "ax"
	.global Func_0810b378
	.type Func_0810b378, %function
	.thumb_func
Func_0810b378:
	.incbin "baserom.gba", 0x0010b37c, 0x00000074
	.section .rom.0010b41c, "ax"
	.balign 4
	.global Func_0810b418
	.type Func_0810b418, %function
	.thumb_func
Func_0810b418:
	.incbin "baserom.gba", 0x0010b41c, 0x00000108
	.section .rom.0010b5c8, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x0010b5c8, 0x000000f8
	.section .rom.0010b6d0, "ax"
	.incbin "baserom.gba", 0x0010b6d0, 0x000000d0
	.section .rom.0010b7b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x0010b7b8, 0x000005fc
	.section .rom.0010bdf8, "ax"
	.incbin "baserom.gba", 0x0010bdf8, 0x00000048
	.section .rom.0010bec0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010bec0, 0x000000dc
	.section .rom.0010c00c, "ax"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0010c00c, 0x00000140
	.section .rom.0010c14c, "ax"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0010c14c, 0x00000200
	.section .rom.0010c388, "ax"
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x0010c388, 0x0000000a
	.section .rom.00118000, "ax"
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000010
	.section .rom.00118070, "ax"
	.incbin "baserom.gba", 0x00118070, 0x00000008
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
	.incbin "baserom.gba", 0x00118410, 0x00000020
	.section .rom.0011843c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011843c, 0x000002fc
	.section .rom.00118738, "ax"
	.global DebugParty_LoadPreset
	.type DebugParty_LoadPreset, %function
	.thumb_func
DebugParty_LoadPreset:
	.incbin "baserom.gba", 0x00118738, 0x0000021c
	.section .rom.00118958, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00118958, 0x000003d4
	.section .rom.00118eb0, "ax"
	.incbin "baserom.gba", 0x00118eb0, 0x000000bc
	.section .rom.00118f6c, "ax"
	.global Func_08118f6c
	.type Func_08118f6c, %function
	.thumb_func
Func_08118f6c:
	.incbin "baserom.gba", 0x00118f6c, 0x000000e8
	.section .rom.00119604, "ax"
	.incbin "baserom.gba", 0x00119604, 0x00000148
	.section .rom.00119808, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x00119808, 0x00000848
	.section .rom.0011a264, "ax"
	.incbin "baserom.gba", 0x0011a264, 0x000000d0
	.section .rom.0011a3b4, "ax"
	.global Func_0811a39c
	.type Func_0811a39c, %function
	.thumb_func
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a3b4, 0x000000b0
	.section .rom.0011a4f6, "ax"
	.incbin "baserom.gba", 0x0011a4f6, 0x0000011e
	.section .rom.0011a68c, "ax"
	.incbin "baserom.gba", 0x0011a68c, 0x00000138
	.section .rom.0011a7c4, "ax"
	.global BtlObjMake
	.type BtlObjMake, %function
	.thumb_func
BtlObjMake:
	.incbin "baserom.gba", 0x0011a7c4, 0x000009c0
	.section .rom.0011b198, "ax"
	.incbin "baserom.gba", 0x0011b198, 0x00000114
	.section .rom.0011b2dc, "ax"
	.global EnmUpd
	.type EnmUpd, %function
	.thumb_func
EnmUpd:
	.incbin "baserom.gba", 0x0011b2dc, 0x000001e8
	.section .rom.0011b4f0, "ax"
	.balign 4
	.global BattleUnit_BuildStatusFlags
	.type BattleUnit_BuildStatusFlags, %function
	.thumb_func
BattleUnit_BuildStatusFlags:
	.incbin "baserom.gba", 0x0011b4f0, 0x000001cc
	.section .rom.0011b772, "ax"
	.incbin "baserom.gba", 0x0011b772, 0x00000002
	.section .rom.0011b774, "ax"
	.global Func_0811b75c
	.type Func_0811b75c, %function
	.thumb_func
Func_0811b75c:
	.incbin "baserom.gba", 0x0011b774, 0x00000278
	.section .rom.0011ba14, "ax"
	.incbin "baserom.gba", 0x0011ba14, 0x00000230
	.section .rom.0011bd04, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bd04, 0x00000024
	.section .rom.0011bd68, "ax"
	.global Func_0811bd50
	.type Func_0811bd50, %function
	.thumb_func
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd68, 0x00000060
	.section .rom.0011bf30, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bf30, 0x00000088
	.section .rom.0011c08c, "ax"
	.balign 4
	.global Func_0811c074
	.type Func_0811c074, %function
	.thumb_func
Func_0811c074:
	.incbin "baserom.gba", 0x0011c08c, 0x00000184
	.section .rom.0011c3d4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011c3d4, 0x000001d8
	.section .rom.0011c682, "ax"
	.incbin "baserom.gba", 0x0011c682, 0x00000002
	.section .rom.0011c728, "ax"
	.global Func_0811c710
	.type Func_0811c710, %function
	.thumb_func
Func_0811c710:
	.incbin "baserom.gba", 0x0011c728, 0x00000344
	.section .rom.0011ca6c, "ax"
	.global Func_0811ca54
	.type Func_0811ca54, %function
	.thumb_func
Func_0811ca54:
	.incbin "baserom.gba", 0x0011ca6c, 0x00000194
	.section .rom.0011cd94, "ax"
	.balign 4
	.global Camera_ConfigureScene
	.type Camera_ConfigureScene, %function
	.thumb_func
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd94, 0x000000d4
	.section .rom.0011ceac, "ax"
	.global BattleEscape_CheckSuccess
	.type BattleEscape_CheckSuccess, %function
	.thumb_func
BattleEscape_CheckSuccess:
	.incbin "baserom.gba", 0x0011ceac, 0x00000110
	.section .rom.0011cfe8, "ax"
	.balign 4
	.global BattlePres_BuildOpponentEntries
	.type BattlePres_BuildOpponentEntries, %function
	.thumb_func
BattlePres_BuildOpponentEntries:
	.incbin "baserom.gba", 0x0011cfe8, 0x00000190
	.section .rom.0011d264, "ax"
	.incbin "baserom.gba", 0x0011d264, 0x000001c8
	.section .rom.0011d42c, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x0011d42c, 0x0000018c
	.section .rom.0011d9e4, "ax"
	.incbin "baserom.gba", 0x0011d9e4, 0x000009a0
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00000432
	.section .rom.0011e848, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011e848, 0x00000800
	.section .rom.0011f048, "ax"
	.global BattleMotion_SetRecordChildValues
	.type BattleMotion_SetRecordChildValues, %function
	.thumb_func
BattleMotion_SetRecordChildValues:
	.incbin "baserom.gba", 0x0011f048, 0x00000300
	.section .rom.0011f4ec, "ax"
	.global Func_0811f4d4
	.type Func_0811f4d4, %function
	.thumb_func
Func_0811f4d4:
	.incbin "baserom.gba", 0x0011f4ec, 0x00000968
	.section .rom.0011ff1e, "ax"
	.incbin "baserom.gba", 0x0011ff1e, 0x00000002
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
	.incbin "baserom.gba", 0x00120078, 0x000000f8
	.section .rom.001201dc, "ax"
	.global BattleEv_DispatchQueued
	.type BattleEv_DispatchQueued, %function
	.thumb_func
BattleEv_DispatchQueued:
	.incbin "baserom.gba", 0x001201dc, 0x0000019c
	.section .rom.0012046c, "ax"
	.global BtlTgtExec
	.type BtlTgtExec, %function
	.thumb_func
BtlTgtExec:
	.incbin "baserom.gba", 0x0012046c, 0x0000206c
	.section .rom.001224f0, "ax"
	.incbin "baserom.gba", 0x001224f0, 0x00000740
	.section .rom.00122ce8, "ax"
	.incbin "baserom.gba", 0x00122ce8, 0x00000040
	.section .rom.00122d28, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x00122d28, 0x00000794
	.section .rom.0012358c, "ax"
	.global Func_08123574
	.type Func_08123574, %function
	.thumb_func
Func_08123574:
	.incbin "baserom.gba", 0x0012358c, 0x0000129c
	.section .rom.00124cdc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00124cdc, 0x00000eb4
	.section .rom.00125bd0, "ax"
	.incbin "baserom.gba", 0x00125bd0, 0x000000a4
	.section .rom.00125d14, "ax"
	.incbin "baserom.gba", 0x00125d14, 0x0000006c
	.section .rom.00125d8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00125d8c, 0x000004a4
	.section .rom.001262a4, "ax"
	.global Func_0812628c
	.type Func_0812628c, %function
	.thumb_func
Func_0812628c:
	.incbin "baserom.gba", 0x001262a4, 0x00000138
	.section .rom.00126414, "ax"
	.balign 4
	.global Func_081263fc
	.type Func_081263fc, %function
	.thumb_func
Func_081263fc:
	.incbin "baserom.gba", 0x00126414, 0x00000408
	.section .rom.0012681c, "ax"
	.global BattlePres_SetupTransitionScene
	.type BattlePres_SetupTransitionScene, %function
	.thumb_func
BattlePres_SetupTransitionScene:
	.incbin "baserom.gba", 0x0012681c, 0x00000100
	.section .rom.0012695a, "ax"
	.incbin "baserom.gba", 0x0012695a, 0x00000002
	.section .rom.0012695c, "ax"
	.global Func_08126944
	.type Func_08126944, %function
	.thumb_func
Func_08126944:
	.incbin "baserom.gba", 0x0012695c, 0x00000034
	.section .rom.00126990, "ax"
	.global Func_08126978
	.type Func_08126978, %function
	.thumb_func
Func_08126978:
	.incbin "baserom.gba", 0x00126990, 0x00000044
	.section .rom.00126a10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00126a10, 0x000000ec
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
	.incbin "baserom.gba", 0x00126d14, 0x00000104
	.section .rom.00126e18, "ax"
	.global Func_08126e00
	.type Func_08126e00, %function
	.thumb_func
Func_08126e00:
	.incbin "baserom.gba", 0x00126e18, 0x00000238
	.section .rom.00127080, "ax"
	.incbin "baserom.gba", 0x00127080, 0x00000248
	.section .rom.00127320, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00127320, 0x00000264
	.section .rom.001275a0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001275a0, 0x00000554
	.section .rom.00127af4, "ax"
	.global EnmCntDec
	.type EnmCntDec, %function
	.thumb_func
EnmCntDec:
	.incbin "baserom.gba", 0x00127af4, 0x000000c4
	.section .rom.00127c96, "ax"
	.incbin "baserom.gba", 0x00127c96, 0x00000002
	.section .rom.00127cec, "ax"
	.incbin "baserom.gba", 0x00127cec, 0x000003cc
	.section .rom.00128202, "ax"
	.incbin "baserom.gba", 0x00128202, 0x0000003e
	.section .rom.00128264, "ax"
	.incbin "baserom.gba", 0x00128264, 0x000005f8
	.section .rom.0012885c, "ax"
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x0012885c, 0x00000024
	.section .rom.001288b0, "ax"
	.incbin "baserom.gba", 0x001288b0, 0x000000da
	.section .rom.0012ce6c, "ax"
	.incbin "baserom.gba", 0x0012ce6c, 0x00000028
	.section .rom.001380ac, "ax"
	.incbin "baserom.gba", 0x001380ac, 0x00000f1c
	.section .rom.0013ba50, "ax"
	.incbin "baserom.gba", 0x0013ba50, 0x000000e8
	.section .rom.0013bbd8, "ax"
	.incbin "baserom.gba", 0x0013bbd8, 0x000009d4
	.section .rom.0013c5f4, "ax"
	.balign 4
	.global Func_0813c5f4
	.type Func_0813c5f4, %function
	.thumb_func
Func_0813c5f4:
	.incbin "baserom.gba", 0x0013c5f4, 0x0000051c
	.section .rom.0013cb34, "ax"
	.balign 4
	.global Func_0813cb34
	.type Func_0813cb34, %function
	.thumb_func
Func_0813cb34:
	.incbin "baserom.gba", 0x0013cb34, 0x0000044c
	.section .rom.0013d098, "ax"
	.balign 4
	.global Func_0813d098
	.type Func_0813d098, %function
	.thumb_func
Func_0813d098:
	.incbin "baserom.gba", 0x0013d098, 0x0000107c
	.section .rom.0013e114, "ax"
	.incbin "baserom.gba", 0x0013e114, 0x000002bc
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
	.incbin "baserom.gba", 0x0013f518, 0x00000384
	.section .rom.0013f8d4, "ax"
	.global Func_0813f8d4
	.type Func_0813f8d4, %function
	.thumb_func
Func_0813f8d4:
	.incbin "baserom.gba", 0x0013f8d4, 0x00000170
	.section .rom.0013fa88, "ax"
	.balign 4
	.global Func_0813fa88
	.type Func_0813fa88, %function
	.thumb_func
Func_0813fa88:
	.incbin "baserom.gba", 0x0013fa88, 0x000002b8
	.section .rom.0013fd84, "ax"
	.incbin "baserom.gba", 0x0013fd84, 0x00002bc0
	.section .rom.00142944, "ax"
	.global Func_08142944
	.type Func_08142944, %function
	.thumb_func
Func_08142944:
	.incbin "baserom.gba", 0x00142944, 0x000007d0
	.section .rom.00143174, "ax"
	.incbin "baserom.gba", 0x00143174, 0x00000314
	.section .rom.0014355c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0014355c, 0x00000084
	.section .rom.001435e0, "ax"
	.global BattleFx_BeginCanvasLayer
	.type BattleFx_BeginCanvasLayer, %function
	.thumb_func
BattleFx_BeginCanvasLayer:
	.incbin "baserom.gba", 0x001435e0, 0x000004a8
	.section .rom.00143bb6, "ax"
	.incbin "baserom.gba", 0x00143bb6, 0x00000002
	.section .rom.00143bb8, "ax"
	.global Func_08143bb8
	.type Func_08143bb8, %function
	.thumb_func
Func_08143bb8:
	.incbin "baserom.gba", 0x00143bb8, 0x000002fc
	.section .rom.00143eb4, "ax"
	.global Func_08143eb4
	.type Func_08143eb4, %function
	.thumb_func
Func_08143eb4:
	.incbin "baserom.gba", 0x00143eb4, 0x00000744
	.section .rom.00144628, "ax"
	.balign 4
	.global Func_08144628
	.type Func_08144628, %function
	.thumb_func
Func_08144628:
	.incbin "baserom.gba", 0x00144628, 0x000004c8
	.section .rom.00144b14, "ax"
	.balign 4
	.global Func_08144b14
	.type Func_08144b14, %function
	.thumb_func
Func_08144b14:
	.incbin "baserom.gba", 0x00144b14, 0x000005a0
	.section .rom.0014512c, "ax"
	.balign 4
	.global Func_0814512c
	.type Func_0814512c, %function
	.thumb_func
Func_0814512c:
	.incbin "baserom.gba", 0x0014512c, 0x000029a8
	.section .rom.00147aec, "ax"
	.balign 4
	.global Func_08147aec
	.type Func_08147aec, %function
	.thumb_func
Func_08147aec:
	.incbin "baserom.gba", 0x00147aec, 0x000020a8
	.section .rom.00149bac, "ax"
	.balign 4
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x00149bac, 0x00000bfc
	.section .rom.0014a812, "ax"
	.incbin "baserom.gba", 0x0014a812, 0x00000002
	.section .rom.0014a814, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x0014a814, 0x000011b0
	.section .rom.0014b9e8, "ax"
	.balign 4
	.global Func_0814b9e8
	.type Func_0814b9e8, %function
	.thumb_func
Func_0814b9e8:
	.incbin "baserom.gba", 0x0014b9e8, 0x00000f40
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
	.incbin "baserom.gba", 0x0014cb60, 0x0000006c
	.section .rom.0014cca8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0014cca8, 0x000000a0
	.section .rom.0014cd48, "ax"
	.global Func_0814cd48
	.type Func_0814cd48, %function
	.thumb_func
Func_0814cd48:
	.incbin "baserom.gba", 0x0014cd48, 0x000000d8
	.section .rom.0014ce30, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0014ce30, 0x00001c0c
	.section .rom.0014ea58, "ax"
	.balign 4
	.global Func_0814ea58
	.type Func_0814ea58, %function
	.thumb_func
Func_0814ea58:
	.incbin "baserom.gba", 0x0014ea58, 0x00000430
	.section .rom.0014ef44, "ax"
	.balign 4
	.global BattleFx_RunSeriesA
	.type BattleFx_RunSeriesA, %function
	.thumb_func
BattleFx_RunSeriesA:
	.incbin "baserom.gba", 0x0014ef44, 0x00000ac4
	.section .rom.0014fab8, "ax"
	.balign 4
	.global Func_0814fab8
	.type Func_0814fab8, %function
	.thumb_func
Func_0814fab8:
	.incbin "baserom.gba", 0x0014fab8, 0x000009f0
	.section .rom.001504d6, "ax"
	.incbin "baserom.gba", 0x001504d6, 0x00000002
	.section .rom.001504d8, "ax"
	.global SummonFx_RunCanvasEffect
SummonFx_RunCanvasEffect:
	.incbin "baserom.gba", 0x001504d8, 0x00000740
	.section .rom.00150c74, "ax"
	.incbin "baserom.gba", 0x00150c74, 0x00001364
	.section .rom.00151ff0, "ax"
	.balign 4
	.global Func_08151ff0
	.type Func_08151ff0, %function
	.thumb_func
Func_08151ff0:
	.incbin "baserom.gba", 0x00151ff0, 0x00000480
	.section .rom.00152474, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00152474, 0x00000080
	.section .rom.0015265a, "ax"
	.incbin "baserom.gba", 0x0015265a, 0x00000002
	.section .rom.0015265c, "ax"
	.global BattleFx_RunProjectileVolleyB
	.type BattleFx_RunProjectileVolleyB, %function
	.thumb_func
BattleFx_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x0000225c
	.section .rom.001548d0, "ax"
	.balign 4
	.global Func_081548d0
	.type Func_081548d0, %function
	.thumb_func
Func_081548d0:
	.incbin "baserom.gba", 0x001548d0, 0x0000048c
	.section .rom.00154d5c, "ax"
	.incbin "baserom.gba", 0x00154d5c, 0x00000b00
	.section .rom.0015585c, "ax"
	.global Func_0815585c
	.type Func_0815585c, %function
	.thumb_func
Func_0815585c:
	.incbin "baserom.gba", 0x0015585c, 0x00000734
	.section .rom.0015613e, "ax"
	.incbin "baserom.gba", 0x0015613e, 0x00000002
	.section .rom.00156140, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x00156140, 0x000013a0
	.section .rom.00157530, "ax"
	.global Func_08157530
	.type Func_08157530, %function
	.thumb_func
Func_08157530:
	.incbin "baserom.gba", 0x00157530, 0x000000bc
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x000006be
	.section .rom.00157d30, "ax"
	.incbin "baserom.gba", 0x00157d30, 0x00001038
	.section .rom.00158dc8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00158dc8, 0x0000127c
	.section .rom.0015a110, "ax"
	.global BattlePres_RunBurstScene
	.type BattlePres_RunBurstScene, %function
	.thumb_func
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x00001300
	.section .rom.0015b434, "ax"
	.incbin "baserom.gba", 0x0015b434, 0x00001524
	.section .rom.0015c970, "ax"
	.balign 4
	.global Func_0815c970
	.type Func_0815c970, %function
	.thumb_func
Func_0815c970:
	.incbin "baserom.gba", 0x0015c970, 0x00000260
	.section .rom.0015cbd0, "ax"
	.incbin "baserom.gba", 0x0015cbd0, 0x000015a0
	.section .rom.0015e22c, "ax"
	.incbin "baserom.gba", 0x0015e22c, 0x0000005c
	.section .rom.0015e288, "ax"
	.global BattleUnit_ProjectToScreen
BattleUnit_ProjectToScreen:
	.incbin "baserom.gba", 0x0015e288, 0x00000074
	.section .rom.0015e3ac, "ax"
	.balign 4
	.global Func_0815e3ac
	.type Func_0815e3ac, %function
	.thumb_func
Func_0815e3ac:
	.incbin "baserom.gba", 0x0015e3ac, 0x00000648
	.section .rom.0015e9f4, "ax"
	.global Func_0815e9f4
	.type Func_0815e9f4, %function
	.thumb_func
Func_0815e9f4:
	.incbin "baserom.gba", 0x0015e9f4, 0x0000060c
	.section .rom.0015f0a0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0015f0a0, 0x000000cc
	.section .rom.0015f16c, "ax"
	.global Func_0815f16c
	.type Func_0815f16c, %function
	.thumb_func
Func_0815f16c:
	.incbin "baserom.gba", 0x0015f16c, 0x00004a94
	.section .rom.00163c2c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163c2c, 0x00000fc8
	.section .rom.00164c0a, "ax"
	.incbin "baserom.gba", 0x00164c0a, 0x000000ba
	.section .rom.00164cc4, "ax"
	.global BattleEffect_RunParticleStreams
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x00164cc4, 0x00001e34
	.section .rom.00166b10, "ax"
	.balign 4
	.global Func_08166b10
	.type Func_08166b10, %function
	.thumb_func
Func_08166b10:
	.incbin "baserom.gba", 0x00166b10, 0x00003708
	.section .rom.0016a2d8, "ax"
	.balign 4
	.global Func_0816a2d8
Func_0816a2d8:
	.incbin "baserom.gba", 0x0016a2d8, 0x00000bfc
	.section .rom.0016aeec, "ax"
	.balign 4
	.global Func_0816aeec
Func_0816aeec:
	.incbin "baserom.gba", 0x0016aeec, 0x000007e8
	.section .rom.0016b6ec, "ax"
	.balign 4
	.global Func_0816b6ec
Func_0816b6ec:
	.incbin "baserom.gba", 0x0016b6ec, 0x00002f58
	.section .rom.0016e65c, "ax"
	.balign 4
	.global Func_0816e65c
Func_0816e65c:
	.incbin "baserom.gba", 0x0016e65c, 0x00001634
	.section .rom.0016fca8, "ax"
	.balign 4
	.global Func_0816fca8
Func_0816fca8:
	.incbin "baserom.gba", 0x0016fca8, 0x00001ecc
	.section .rom.00171bb0, "ax"
	.balign 4
	.global Func_08171bb0
	.thumb_func
Func_08171bb0:
	.incbin "baserom.gba", 0x00171bb0, 0x00000f38
	.section .rom.00172b00, "ax"
	.balign 4
	.global Func_08172b00
Func_08172b00:
	.incbin "baserom.gba", 0x00172b00, 0x00000a70
	.section .rom.00173588, "ax"
	.balign 4
	.global Func_08173588
Func_08173588:
	.incbin "baserom.gba", 0x00173588, 0x00000464
	.section .rom.00173aac, "ax"
	.balign 4
	.global Func_08173aac
Func_08173aac:
	.incbin "baserom.gba", 0x00173aac, 0x00007ec4
	.section .rom.0017b9b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0017b9b8, 0x000026c0
	.section .rom.0017e0c0, "ax"
	.balign 4
	.global Func_0817e0c0
Func_0817e0c0:
	.incbin "baserom.gba", 0x0017e0c0, 0x000005a8
	.section .rom.0017e698, "ax"
	.balign 4
	.global Func_0817e698
Func_0817e698:
	.incbin "baserom.gba", 0x0017e698, 0x0000037c
	.section .rom.0017ea58, "ax"
	.incbin "baserom.gba", 0x0017ea58, 0x00003458
	.section .rom.00181ed4, "ax"
	.balign 4
	.global Func_08181ed4
Func_08181ed4:
	.incbin "baserom.gba", 0x00181ed4, 0x000004bc
	.section .rom.001823a8, "ax"
	.balign 4
	.global Func_081823a8
Func_081823a8:
	.incbin "baserom.gba", 0x001823a8, 0x000004a4
	.section .rom.00182898, "ax"
	.balign 4
	.global Func_08182898
	.type Func_08182898, %function
	.thumb_func
Func_08182898:
	.incbin "baserom.gba", 0x00182898, 0x00007958
	.section .rom.0018a220, "ax"
	.balign 4
	.global Func_0818a220
Func_0818a220:
	.incbin "baserom.gba", 0x0018a220, 0x00000af0
	.section .rom.0018ad40, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad40, 0x00000f20
	.section .rom.0018bc98, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc98, 0x00000b08
	.section .rom.0018c7b8, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7b8, 0x00000dd8
	.section .rom.0018d5a8, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5a8, 0x00002024
	.section .rom.0018f620, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f620, 0x000024c0
	.section .rom.00191cc4, "ax"
	.incbin "baserom.gba", 0x00191cc4, 0x00000b64
	.section .rom.0019284c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0019284c, 0x00003ba0
	.section .rom.00196404, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000554
	.section .rom.00196a7c, "ax"
	.incbin "baserom.gba", 0x00196a7c, 0x00000560
	.section .rom.001973f0, "ax"
	.incbin "baserom.gba", 0x001973f0, 0x0000025c
	.section .rom.00197690, "ax"
	.incbin "baserom.gba", 0x00197690, 0x000001a4
	.section .rom.00197864, "ax"
	.incbin "baserom.gba", 0x00197864, 0x00000d02
	.section .rom.001985a6, "ax"
	.incbin "baserom.gba", 0x001985a6, 0x00001c02
	.section .rom.001a052c, "ax"
	.incbin "baserom.gba", 0x001a052c, 0x00000188
	.section .rom.001a06e0, "ax"
	.global Func_081a06e0
	.type Func_081a06e0, %function
	.thumb_func
Func_081a06e0:
	.incbin "baserom.gba", 0x001a06e0, 0x00000320
	.section .rom.001a0a70, "ax"
	.incbin "baserom.gba", 0x001a0a70, 0x00000178
	.section .rom.001a0be8, "ax"
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0be8, 0x000000c4
	.section .rom.001a0d78, "ax"
	.global Func_081a0d78
	.type Func_081a0d78, %function
	.thumb_func
Func_081a0d78:
	.incbin "baserom.gba", 0x001a0d78, 0x00000240
	.section .rom.001a101c, "ax"
	.global DisplayScroll_DrawLine
	.type DisplayScroll_DrawLine, %function
	.thumb_func
DisplayScroll_DrawLine:
	.incbin "baserom.gba", 0x001a101c, 0x0000026c
	.section .rom.001a1294, "ax"
	.incbin "baserom.gba", 0x001a1294, 0x00000278
	.section .rom.001a150c, "ax"
	.global Func_081a150c
	.type Func_081a150c, %function
	.thumb_func
Func_081a150c:
	.incbin "baserom.gba", 0x001a150c, 0x000000d0
	.section .rom.001a15dc, "ax"
	.global Func_081a15dc
	.type Func_081a15dc, %function
	.thumb_func
Func_081a15dc:
	.incbin "baserom.gba", 0x001a15dc, 0x000001f4
	.section .rom.001a1830, "ax"
	.incbin "baserom.gba", 0x001a1830, 0x000000c8
	.section .rom.001a1928, "ax"
	.incbin "baserom.gba", 0x001a1928, 0x00000858
	.section .rom.001a2180, "ax"
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x001a2180, 0x000006b0
	.section .rom.001a6030, "ax"
	.incbin "baserom.gba", 0x001a6030, 0x00000064
	.section .rom.001a6154, "ax"
	.incbin "baserom.gba", 0x001a6154, 0x00000960
	.section .rom.001a6b50, "ax"
	.incbin "baserom.gba", 0x001a6b50, 0x00000678
	.section .rom.001a71c8, "ax"
	.global Func_081a71c8
	.type Func_081a71c8, %function
	.thumb_func
Func_081a71c8:
	.incbin "baserom.gba", 0x001a71c8, 0x000000d4
	.section .rom.001a729c, "ax"
	.global Func_081a729c
	.type Func_081a729c, %function
	.thumb_func
Func_081a729c:
	.incbin "baserom.gba", 0x001a729c, 0x00000278
	.section .rom.001a7514, "ax"
	.global Data_081a7514
	.type Data_081a7514, %function
	.thumb_func
Data_081a7514:
	.incbin "baserom.gba", 0x001a7514, 0x00000004
	.section .rom.001a7518, "ax"
	.global Func_081a7518
	.type Func_081a7518, %function
	.thumb_func
Func_081a7518:
	.incbin "baserom.gba", 0x001a7518, 0x000001e0
	.section .rom.001a76f8, "ax"
	.global Func_081a76f8
	.type Func_081a76f8, %function
	.thumb_func
Func_081a76f8:
	.incbin "baserom.gba", 0x001a76f8, 0x00000174
	.section .rom.001a786c, "ax"
	.global Data_081a786c
	.type Data_081a786c, %function
	.thumb_func
Data_081a786c:
	.incbin "baserom.gba", 0x001a786c, 0x00000004
	.section .rom.001a78c0, "ax"
	.global TitlePalette_UpdateFade
	.type TitlePalette_UpdateFade, %function
	.thumb_func
TitlePalette_UpdateFade:
	.incbin "baserom.gba", 0x001a78c0, 0x00000168
	.section .rom.001a7a28, "ax"
	.global Graphics_TransformPaletteBuffer
	.type Graphics_TransformPaletteBuffer, %function
	.thumb_func
Graphics_TransformPaletteBuffer:
	.incbin "baserom.gba", 0x001a7a28, 0x00000724
	.section .rom.001a8286, "ax"
	.incbin "baserom.gba", 0x001a8286, 0x000004ae
	.section .rom.001ac026, "ax"
	.incbin "baserom.gba", 0x001ac026, 0x0000008a
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x00001238
	.section .rom.001b2008, "ax"
	.global Func_081b2008
	.type Func_081b2008, %function
	.thumb_func
Func_081b2008:
	.incbin "baserom.gba", 0x001b2008, 0x00000034
	.section .rom.001b20a0, "ax"
	.incbin "baserom.gba", 0x001b20a0, 0x000000b0
	.section .rom.001b21ec, "ax"
	.incbin "baserom.gba", 0x001b21ec, 0x00001c48
	.section .rom.001b3ed8, "ax"
	.incbin "baserom.gba", 0x001b3ed8, 0x000009f8
	.section .rom.001b8020, "ax"
	.incbin "baserom.gba", 0x001b8020, 0x00000088
	.section .rom.001b810c, "ax"
	.incbin "baserom.gba", 0x001b810c, 0x000002b8
	.section .rom.001b83c4, "ax"
	.global Func_081b83c4
	.type Func_081b83c4, %function
	.thumb_func
Func_081b83c4:
	.incbin "baserom.gba", 0x001b83c4, 0x00001f54
	.section .rom.001c00b0, "ax"
	.incbin "baserom.gba", 0x001c00b0, 0x00000064
	.section .rom.001c0114, "ax"
	.global LeftoverSoundDriver_StaleCallTarget
LeftoverSoundDriver_StaleCallTarget:
	.incbin "baserom.gba", 0x001c0114, 0x00000054
	.section .rom.001c0168, "ax"
	.global Func_081c0168
	.type Func_081c0168, %function
	.thumb_func
Func_081c0168:
	.incbin "baserom.gba", 0x001c0168, 0x00000030
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
	.section .rom.001c0c1c, "ax"
	.balign 4
	.global Func_081c0c1c
	.type Func_081c0c1c, %function
	.thumb_func
Func_081c0c1c:
	.incbin "baserom.gba", 0x001c0c1c, 0x00000090
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
	.section .rom.001c1008, "ax"
	.incbin "baserom.gba", 0x001c1008, 0x0000000c
	.section .rom.001c1014, "ax"
	.global Func_081c1014
	.type Func_081c1014, %function
	.thumb_func
Func_081c1014:
	.incbin "baserom.gba", 0x001c1014, 0x00000144
	.section .rom.001c11ec, "ax"
	.incbin "baserom.gba", 0x001c11ec, 0x00000090
	.section .rom.001c16ca, "ax"
	.incbin "baserom.gba", 0x001c16ca, 0x00000002
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
AudioCommand_InvokeSlot35:
	.section .rom.001c2328, "ax"
	.global AudioCommand_InvokeSlot35
	.global Func_081c2328
	.type AudioCommand_InvokeSlot35, %function
	.thumb_func
Func_081c2328:
	.incbin "baserom.gba", 0x001c2328, 0x00000014
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
	.section .rom.001c343c, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x001c343c, 0x00000090
	.section .rom.001c34cc, "ax"
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x001c34cc, 0x000000b4
	.section .rom.001c3580, "ax"
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x001c3580, 0x00000030
	.section .rom.001c35b0, "ax"
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x001c35b0, 0x00000018
	.section .rom.001c35c8, "ax"
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x001c35c8, 0x00000084
	.section .rom.001c364c, "ax"
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x001c364c, 0x00000018
	.section .rom.001c3664, "ax"
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x001c3664, 0x0000003c
	.section .rom.001c36a0, "ax"
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x001c36a0, 0x00000010
	.section .rom.001c36b0, "ax"
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x001c36b0, 0x00000034
	.section .rom.001c36e4, "ax"
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x001c36e4, 0x00000030
	.section .rom.001c43b0, "ax"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .rom.001c44d0, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .rom.00682000, "ax"
	.global Resource_Data002
Resource_Data002:
	.incbin "baserom.gba", 0x00682000, 0x00000010
	.section .rom.006848d0, "ax"
	.global Resource_Data015
Resource_Data015:
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .rom.006978b5, "ax"
	.incbin "baserom.gba", 0x006978b5, 0x00000003
	.section .rom.006a444d, "ax"
	.incbin "baserom.gba", 0x006a444d, 0x00000003
	.section .rom.006a56c6, "ax"
	.incbin "baserom.gba", 0x006a56c6, 0x00000002
	.section .rom.006a99ab, "ax"
	.incbin "baserom.gba", 0x006a99ab, 0x00000001
	.section .rom.006b0fc7, "ax"
	.incbin "baserom.gba", 0x006b0fc7, 0x00000001
	.section .rom.006cd208, "ax"
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
	.section .rom.0087c494, "ax"
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c494, 0x0000024c
	.section .rom.0087c6e0, "ax"
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c6e0, 0x00000198
	.section .rom.0087d0a1, "ax"
	.incbin "baserom.gba", 0x0087d0a1, 0x00000003
	.section .rom.0087e573, "ax"
	.incbin "baserom.gba", 0x0087e573, 0x00000001
	.section .rom.008806dd, "ax"
	.incbin "baserom.gba", 0x008806dd, 0x00000003
	.section .rom.00880a33, "ax"
	.incbin "baserom.gba", 0x00880a33, 0x00000001
	.section .rom.00880bb7, "ax"
	.incbin "baserom.gba", 0x00880bb7, 0x00000001
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
	.section .rom.0088573d, "ax"
	.incbin "baserom.gba", 0x0088573d, 0x00000003
	.section .rom.0088680a, "ax"
	.incbin "baserom.gba", 0x0088680a, 0x00000002
	.section .rom.0088680c, "ax"
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088680c, 0x000002e8
	.section .rom.00886c46, "ax"
	.incbin "baserom.gba", 0x00886c46, 0x00000002
	.section .rom.00889153, "ax"
	.incbin "baserom.gba", 0x00889153, 0x00000001
	.section .rom.00889b0f, "ax"
	.incbin "baserom.gba", 0x00889b0f, 0x00000001
	.section .rom.0088ac52, "ax"
	.incbin "baserom.gba", 0x0088ac52, 0x00000002
	.section .rom.0088b8d0, "ax"
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b8d0, 0x0000002c
	.section .rom.0088b8fc, "ax"
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b8fc, 0x0000002c
	.section .rom.0088c0e5, "ax"
	.incbin "baserom.gba", 0x0088c0e5, 0x00000003
	.section .rom.0088d663, "ax"
	.incbin "baserom.gba", 0x0088d663, 0x00000001
	.section .rom.0088d98d, "ax"
	.incbin "baserom.gba", 0x0088d98d, 0x00000003
	.section .rom.0088deb6, "ax"
	.incbin "baserom.gba", 0x0088deb6, 0x00000002
	.section .rom.0088e3e6, "ax"
	.incbin "baserom.gba", 0x0088e3e6, 0x00000002
	.section .rom.0088f161, "ax"
	.incbin "baserom.gba", 0x0088f161, 0x00000003
	.section .rom.0088f781, "ax"
	.incbin "baserom.gba", 0x0088f781, 0x00000003
	.section .rom.0088fdad, "ax"
	.incbin "baserom.gba", 0x0088fdad, 0x00000003
	.section .rom.008901b7, "ax"
	.incbin "baserom.gba", 0x008901b7, 0x00000001
	.section .rom.00890411, "ax"
	.incbin "baserom.gba", 0x00890411, 0x00000003
	.section .rom.0089098f, "ax"
	.incbin "baserom.gba", 0x0089098f, 0x00000001
	.section .rom.00890c39, "ax"
	.incbin "baserom.gba", 0x00890c39, 0x00000003
	.section .rom.00891c55, "ax"
	.incbin "baserom.gba", 0x00891c55, 0x00000003
	.section .rom.00894707, "ax"
	.incbin "baserom.gba", 0x00894707, 0x00000001
	.section .rom.008958d3, "ax"
	.incbin "baserom.gba", 0x008958d3, 0x00000001
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
	.section .rom.0089b3c6, "ax"
	.incbin "baserom.gba", 0x0089b3c6, 0x00000002
	.section .rom.0089c101, "ax"
	.incbin "baserom.gba", 0x0089c101, 0x00000003
	.section .rom.0089d33d, "ax"
	.incbin "baserom.gba", 0x0089d33d, 0x00000003
	.section .rom.0089dc87, "ax"
	.incbin "baserom.gba", 0x0089dc87, 0x00000001
	.section .rom.0089e2e2, "ax"
	.incbin "baserom.gba", 0x0089e2e2, 0x00000002
	.section .rom.0089e80e, "ax"
	.incbin "baserom.gba", 0x0089e80e, 0x00000002
	.section .rom.008a0ac9, "ax"
	.incbin "baserom.gba", 0x008a0ac9, 0x00000003
	.section .rom.008a225d, "ax"
	.incbin "baserom.gba", 0x008a225d, 0x00000003
	.section .rom.008a2941, "ax"
	.incbin "baserom.gba", 0x008a2941, 0x00000003
	.section .rom.008a2944, "ax"
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2944, 0x00001f4c
	.section .rom.008a5f99, "ax"
	.incbin "baserom.gba", 0x008a5f99, 0x00000003
	.section .rom.008a648a, "ax"
	.incbin "baserom.gba", 0x008a648a, 0x00000002
	.section .rom.008a6ad2, "ax"
	.incbin "baserom.gba", 0x008a6ad2, 0x00000002
	.section .rom.008a76f7, "ax"
	.incbin "baserom.gba", 0x008a76f7, 0x00000001
	.section .rom.008a7ab9, "ax"
	.incbin "baserom.gba", 0x008a7ab9, 0x00000003
	.section .rom.008a7c83, "ax"
	.incbin "baserom.gba", 0x008a7c83, 0x00000001
	.section .rom.008a81ce, "ax"
	.incbin "baserom.gba", 0x008a81ce, 0x00000002
	.section .rom.008a851a, "ax"
	.incbin "baserom.gba", 0x008a851a, 0x00000002
	.section .rom.008a851c, "ax"
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a851c, 0x0000076c
	.section .rom.008a92cb, "ax"
	.incbin "baserom.gba", 0x008a92cb, 0x00000001
	.section .rom.008a92cc, "ax"
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a92cc, 0x000000a8
	.section .rom.008a968a, "ax"
	.incbin "baserom.gba", 0x008a968a, 0x00000002
	.section .rom.008aa031, "ax"
	.incbin "baserom.gba", 0x008aa031, 0x00000003
	.section .rom.008aa034, "ax"
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa034, 0x000002f8
	.section .rom.008aae59, "ax"
	.incbin "baserom.gba", 0x008aae59, 0x00000003
	.section .rom.008aae5c, "ax"
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aae5c, 0x00000100
	.section .rom.008abbcd, "ax"
	.incbin "baserom.gba", 0x008abbcd, 0x00000003
	.section .rom.008ac415, "ax"
	.incbin "baserom.gba", 0x008ac415, 0x00000003
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
	.section .rom.008b483e, "ax"
	.incbin "baserom.gba", 0x008b483e, 0x00000002
	.section .rom.008b5807, "ax"
	.incbin "baserom.gba", 0x008b5807, 0x00000001
	.section .rom.008b5a62, "ax"
	.incbin "baserom.gba", 0x008b5a62, 0x00000002
	.section .rom.008b74bd, "ax"
	.incbin "baserom.gba", 0x008b74bd, 0x00000003
	.section .rom.008b7a09, "ax"
	.incbin "baserom.gba", 0x008b7a09, 0x00000003
	.section .rom.008b9792, "ax"
	.incbin "baserom.gba", 0x008b9792, 0x00000002
	.section .rom.008b9a0b, "ax"
	.incbin "baserom.gba", 0x008b9a0b, 0x00000001
	.section .rom.008b9ed2, "ax"
	.incbin "baserom.gba", 0x008b9ed2, 0x00000002
	.section .rom.008bbaa7, "ax"
	.incbin "baserom.gba", 0x008bbaa7, 0x00000001
	.section .rom.008bd6a5, "ax"
	.incbin "baserom.gba", 0x008bd6a5, 0x00000003
	.section .rom.008bd8c6, "ax"
	.incbin "baserom.gba", 0x008bd8c6, 0x00000002
	.section .rom.008bdd03, "ax"
	.incbin "baserom.gba", 0x008bdd03, 0x00000001
	.section .rom.008bde15, "ax"
	.incbin "baserom.gba", 0x008bde15, 0x00000003
	.section .rom.008be3f7, "ax"
	.incbin "baserom.gba", 0x008be3f7, 0x00000001
	.section .rom.008be895, "ax"
	.incbin "baserom.gba", 0x008be895, 0x00000003
	.section .rom.008bf5aa, "ax"
	.incbin "baserom.gba", 0x008bf5aa, 0x00000002
	.section .rom.008c032a, "ax"
	.incbin "baserom.gba", 0x008c032a, 0x00000002
	.section .rom.008c06bb, "ax"
	.incbin "baserom.gba", 0x008c06bb, 0x00000001
	.section .rom.008c2402, "ax"
	.incbin "baserom.gba", 0x008c2402, 0x00000002
	.section .rom.008c31d9, "ax"
	.incbin "baserom.gba", 0x008c31d9, 0x00000003
	.section .rom.008c3a06, "ax"
	.incbin "baserom.gba", 0x008c3a06, 0x00000002
	.section .rom.008c3fb2, "ax"
	.incbin "baserom.gba", 0x008c3fb2, 0x00000002
	.section .rom.008c416f, "ax"
	.incbin "baserom.gba", 0x008c416f, 0x00000001
	.section .rom.008c4ec7, "ax"
	.incbin "baserom.gba", 0x008c4ec7, 0x00000001
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
	.section .rom.008cafed, "ax"
	.incbin "baserom.gba", 0x008cafed, 0x00000003
	.section .rom.008cbdbb, "ax"
	.incbin "baserom.gba", 0x008cbdbb, 0x00000001
	.section .rom.008cd025, "ax"
	.incbin "baserom.gba", 0x008cd025, 0x00000003
	.section .rom.008cd57f, "ax"
	.incbin "baserom.gba", 0x008cd57f, 0x00000001
	.section .rom.008cee69, "ax"
	.incbin "baserom.gba", 0x008cee69, 0x00000003
	.section .rom.008cf70a, "ax"
	.incbin "baserom.gba", 0x008cf70a, 0x00000002
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
	.section .rom.008d4a1a, "ax"
	.incbin "baserom.gba", 0x008d4a1a, 0x00000002
	.section .rom.008d4d9d, "ax"
	.incbin "baserom.gba", 0x008d4d9d, 0x00000003
	.section .rom.008d5a4d, "ax"
	.incbin "baserom.gba", 0x008d5a4d, 0x00000003
	.section .rom.008d6595, "ax"
	.incbin "baserom.gba", 0x008d6595, 0x00000003
	.section .rom.008d672e, "ax"
	.incbin "baserom.gba", 0x008d672e, 0x00000002
	.section .rom.008d6fbb, "ax"
	.incbin "baserom.gba", 0x008d6fbb, 0x00000001
	.section .rom.008d7b8a, "ax"
	.incbin "baserom.gba", 0x008d7b8a, 0x00000002
	.section .rom.008d80a2, "ax"
	.incbin "baserom.gba", 0x008d80a2, 0x00000002
	.section .rom.008d86c6, "ax"
	.incbin "baserom.gba", 0x008d86c6, 0x00000002
	.section .rom.008d8ae9, "ax"
	.incbin "baserom.gba", 0x008d8ae9, 0x00000003
	.section .rom.008d8d83, "ax"
	.incbin "baserom.gba", 0x008d8d83, 0x00000001
	.section .rom.008da71f, "ax"
	.incbin "baserom.gba", 0x008da71f, 0x00000001
	.section .rom.008dc496, "ax"
	.incbin "baserom.gba", 0x008dc496, 0x00000002
	.section .rom.008de40b, "ax"
	.incbin "baserom.gba", 0x008de40b, 0x00000001
	.section .rom.008de8db, "ax"
	.incbin "baserom.gba", 0x008de8db, 0x00000001
	.section .rom.008def6d, "ax"
	.incbin "baserom.gba", 0x008def6d, 0x00000003
	.section .rom.008df9a3, "ax"
	.incbin "baserom.gba", 0x008df9a3, 0x00000001
	.section .rom.008e0d75, "ax"
	.incbin "baserom.gba", 0x008e0d75, 0x00000003
	.section .rom.008e1846, "ax"
	.incbin "baserom.gba", 0x008e1846, 0x00000002
	.section .rom.008e2383, "ax"
	.incbin "baserom.gba", 0x008e2383, 0x00000001
	.section .rom.008e29c3, "ax"
	.incbin "baserom.gba", 0x008e29c3, 0x00000001
	.section .rom.008e3f4a, "ax"
	.incbin "baserom.gba", 0x008e3f4a, 0x00000002
	.section .rom.008e3fad, "ax"
	.incbin "baserom.gba", 0x008e3fad, 0x00000003
	.section .rom.008e431b, "ax"
	.incbin "baserom.gba", 0x008e431b, 0x00000001
	.section .rom.008e48b9, "ax"
	.incbin "baserom.gba", 0x008e48b9, 0x00000003
	.section .rom.008e48bc, "ax"
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e48bc, 0x00000204
	.section .rom.008e4df9, "ax"
	.incbin "baserom.gba", 0x008e4df9, 0x00000003
	.section .rom.008e5e12, "ax"
	.incbin "baserom.gba", 0x008e5e12, 0x00000002
	.section .rom.008e941e, "ax"
	.incbin "baserom.gba", 0x008e941e, 0x00000002
	.section .rom.008e9bc1, "ax"
	.incbin "baserom.gba", 0x008e9bc1, 0x00000003
	.section .rom.008eaf53, "ax"
	.incbin "baserom.gba", 0x008eaf53, 0x00000001
	.section .rom.008eb383, "ax"
	.incbin "baserom.gba", 0x008eb383, 0x00000001
	.section .rom.008ed179, "ax"
	.incbin "baserom.gba", 0x008ed179, 0x00000003
	.section .rom.008eda8e, "ax"
	.incbin "baserom.gba", 0x008eda8e, 0x00000002
	.section .rom.008ef5c2, "ax"
	.incbin "baserom.gba", 0x008ef5c2, 0x00000002
	.section .rom.008f0611, "ax"
	.incbin "baserom.gba", 0x008f0611, 0x00000003
	.section .rom.008f16b9, "ax"
	.incbin "baserom.gba", 0x008f16b9, 0x00000003
	.section .rom.0093c4e1, "ax"
	.incbin "baserom.gba", 0x0093c4e1, 0x00000003
	.section .rom.0093c6f9, "ax"
	.incbin "baserom.gba", 0x0093c6f9, 0x00000003
	.section .rom.0093c84d, "ax"
	.incbin "baserom.gba", 0x0093c84d, 0x00000003
	.section .rom.0093cd05, "ax"
	.incbin "baserom.gba", 0x0093cd05, 0x00000003
	.section .rom.0093e472, "ax"
	.incbin "baserom.gba", 0x0093e472, 0x00000002
	.section .rom.0093f9a9, "ax"
	.incbin "baserom.gba", 0x0093f9a9, 0x00000003
	.section .rom.009417ff, "ax"
	.incbin "baserom.gba", 0x009417ff, 0x00000001
	.section .rom.00943913, "ax"
	.incbin "baserom.gba", 0x00943913, 0x00000001
	.section .rom.0094408d, "ax"
	.incbin "baserom.gba", 0x0094408d, 0x00000003
	.section .rom.0094670b, "ax"
	.incbin "baserom.gba", 0x0094670b, 0x00000001
	.section .rom.00947113, "ax"
	.incbin "baserom.gba", 0x00947113, 0x00000001
	.section .rom.00949e32, "ax"
	.incbin "baserom.gba", 0x00949e32, 0x00000002
	.section .rom.0094a00a, "ax"
	.incbin "baserom.gba", 0x0094a00a, 0x00000002
	.section .rom.0094a469, "ax"
	.incbin "baserom.gba", 0x0094a469, 0x00000003
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
	.section .rom.0094ff92, "ax"
	.incbin "baserom.gba", 0x0094ff92, 0x00000002
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
	.section .rom.00953d04, "ax"
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953d04, 0x000002f8
	.section .rom.009540d5, "ax"
	.incbin "baserom.gba", 0x009540d5, 0x00000003
	.section .rom.009540d8, "ax"
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
	.section .rom.00967f34, "ax"
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f34, 0x00000100
	.section .rom.00968034, "ax"
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968034, 0x000004c8
	.section .rom.009684fc, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009684fc, 0x00000268
	.section .rom.00968764, "ax"
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
	.section .rom.009699e0, "ax"
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x009699e0, 0x0000024c
	.section .rom.00969d0e, "ax"
	.incbin "baserom.gba", 0x00969d0e, 0x00000002
	.section .rom.00969dc9, "ax"
	.incbin "baserom.gba", 0x00969dc9, 0x00000003
	.section .rom.00969f9e, "ax"
	.incbin "baserom.gba", 0x00969f9e, 0x00000002
	.section .rom.0096a038, "ax"
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
	.section .rom.0096ac1c, "ax"
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096ac1c, 0x000000b0
	.section .rom.0096accc, "ax"
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096accc, 0x00000560
	.section .rom.0096b22c, "ax"
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b22c, 0x00000074
	.section .rom.0096b2a0, "ax"
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b2a0, 0x00000048
	.section .rom.0096b2e8, "ax"
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b2e8, 0x00000050
	.section .rom.0096b338, "ax"
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b338, 0x00000050
	.section .rom.0096b388, "ax"
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b388, 0x00000058
	.section .rom.0096b3e0, "ax"
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b3e0, 0x00000058
	.section .rom.0096b438, "ax"
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b438, 0x00000040
	.section .rom.0096b478, "ax"
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b478, 0x00000044
	.section .rom.0096b4bc, "ax"
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
	.section .rom.009c0447, "ax"
	.incbin "baserom.gba", 0x009c0447, 0x00000001
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
	.section .rom.00a79967, "ax"
	.incbin "baserom.gba", 0x00a79967, 0x00000001
	.section .rom.00a79ab6, "ax"
	.incbin "baserom.gba", 0x00a79ab6, 0x00000002
	.section .rom.00a79bf7, "ax"
	.incbin "baserom.gba", 0x00a79bf7, 0x00000001
	.section .rom.00a79d37, "ax"
	.incbin "baserom.gba", 0x00a79d37, 0x00000001
	.section .rom.00a79e77, "ax"
	.incbin "baserom.gba", 0x00a79e77, 0x00000001
	.section .rom.00a7b0d5, "ax"
	.incbin "baserom.gba", 0x00a7b0d5, 0x00000003
	.section .rom.00a7b2a6, "ax"
	.incbin "baserom.gba", 0x00a7b2a6, 0x00000002
	.section .rom.00a7d347, "ax"
	.incbin "baserom.gba", 0x00a7d347, 0x00000001
	.section .rom.00a805a9, "ax"
	.incbin "baserom.gba", 0x00a805a9, 0x00000003
	.section .rom.00a85e1b, "ax"
	.incbin "baserom.gba", 0x00a85e1b, 0x00000001
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
	.section .rom.00b1ab06, "ax"
	.incbin "baserom.gba", 0x00b1ab06, 0x00000002
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
	.section .rom.00c28ae5, "ax"
	.incbin "baserom.gba", 0x00c28ae5, 0x00000003
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
	.section .rom.00d2ad51, "ax"
	.incbin "baserom.gba", 0x00d2ad51, 0x00000003
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
	.section .rom.00d49902, "ax"
	.incbin "baserom.gba", 0x00d49902, 0x00000002
	.section .rom.00d4bc67, "ax"
	.incbin "baserom.gba", 0x00d4bc67, 0x00000001
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
	.section .rom.00e69e8a, "ax"
	.incbin "baserom.gba", 0x00e69e8a, 0x00000002
	.section .rom.00e6b43b, "ax"
	.incbin "baserom.gba", 0x00e6b43b, 0x00000001
	.section .rom.00e6b5ee, "ax"
	.incbin "baserom.gba", 0x00e6b5ee, 0x00000002
	.section .rom.00e6d82d, "ax"
	.incbin "baserom.gba", 0x00e6d82d, 0x00000003
	.section .rom.00e6e353, "ax"
	.incbin "baserom.gba", 0x00e6e353, 0x00000001
	.section .rom.00e795b1, "ax"
	.incbin "baserom.gba", 0x00e795b1, 0x00000003
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
	.section .rom.00eb68f1, "ax"
	.incbin "baserom.gba", 0x00eb68f1, 0x00000003
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
	.section .rom.00ebdc13, "ax"
	.incbin "baserom.gba", 0x00ebdc13, 0x00000001
	.section .rom.00ebdd62, "ax"
	.incbin "baserom.gba", 0x00ebdd62, 0x00000002
	.section .rom.00ebdea3, "ax"
	.incbin "baserom.gba", 0x00ebdea3, 0x00000001
	.section .rom.00ebdfe3, "ax"
	.incbin "baserom.gba", 0x00ebdfe3, 0x00000001
	.section .rom.00ebe123, "ax"
	.incbin "baserom.gba", 0x00ebe123, 0x00000001
	.section .rom.00ebe839, "ax"
	.incbin "baserom.gba", 0x00ebe839, 0x00000003
	.section .rom.00ebe847, "ax"
	.incbin "baserom.gba", 0x00ebe847, 0x00000001
	.section .rom.00ebe996, "ax"
	.incbin "baserom.gba", 0x00ebe996, 0x00000002
	.section .rom.00ebead7, "ax"
	.incbin "baserom.gba", 0x00ebead7, 0x00000001
	.section .rom.00ebec17, "ax"
	.incbin "baserom.gba", 0x00ebec17, 0x00000001
	.section .rom.00ebed57, "ax"
	.incbin "baserom.gba", 0x00ebed57, 0x00000001
	.section .rom.00ebf919, "ax"
	.incbin "baserom.gba", 0x00ebf919, 0x00000003
	.section .rom.00ebf927, "ax"
	.incbin "baserom.gba", 0x00ebf927, 0x00000001
	.section .rom.00ebfa76, "ax"
	.incbin "baserom.gba", 0x00ebfa76, 0x00000002
	.section .rom.00ebfbb7, "ax"
	.incbin "baserom.gba", 0x00ebfbb7, 0x00000001
	.section .rom.00ebfcf7, "ax"
	.incbin "baserom.gba", 0x00ebfcf7, 0x00000001
	.section .rom.00ebfe37, "ax"
	.incbin "baserom.gba", 0x00ebfe37, 0x00000001
	.section .rom.00ec0405, "ax"
	.incbin "baserom.gba", 0x00ec0405, 0x00000003
	.section .rom.00ec0413, "ax"
	.incbin "baserom.gba", 0x00ec0413, 0x00000001
	.section .rom.00ec0562, "ax"
	.incbin "baserom.gba", 0x00ec0562, 0x00000002
	.section .rom.00ec06a3, "ax"
	.incbin "baserom.gba", 0x00ec06a3, 0x00000001
	.section .rom.00ec07e3, "ax"
	.incbin "baserom.gba", 0x00ec07e3, 0x00000001
	.section .rom.00ec0923, "ax"
	.incbin "baserom.gba", 0x00ec0923, 0x00000001
	.section .rom.00ec12a1, "ax"
	.incbin "baserom.gba", 0x00ec12a1, 0x00000003
	.section .rom.00ec12af, "ax"
	.incbin "baserom.gba", 0x00ec12af, 0x00000001
	.section .rom.00ec13fe, "ax"
	.incbin "baserom.gba", 0x00ec13fe, 0x00000002
	.section .rom.00ec153f, "ax"
	.incbin "baserom.gba", 0x00ec153f, 0x00000001
	.section .rom.00ec167f, "ax"
	.incbin "baserom.gba", 0x00ec167f, 0x00000001
	.section .rom.00ec17bf, "ax"
	.incbin "baserom.gba", 0x00ec17bf, 0x00000001
	.section .rom.00ec1be9, "ax"
	.incbin "baserom.gba", 0x00ec1be9, 0x00000003
	.section .rom.00ec1bf7, "ax"
	.incbin "baserom.gba", 0x00ec1bf7, 0x00000001
	.section .rom.00ec1d46, "ax"
	.incbin "baserom.gba", 0x00ec1d46, 0x00000002
	.section .rom.00ec1e87, "ax"
	.incbin "baserom.gba", 0x00ec1e87, 0x00000001
	.section .rom.00ec1fc7, "ax"
	.incbin "baserom.gba", 0x00ec1fc7, 0x00000001
	.section .rom.00ec2107, "ax"
	.incbin "baserom.gba", 0x00ec2107, 0x00000001
	.section .rom.00ec2731, "ax"
	.incbin "baserom.gba", 0x00ec2731, 0x00000003
	.section .rom.00ec273f, "ax"
	.incbin "baserom.gba", 0x00ec273f, 0x00000001
	.section .rom.00ec288e, "ax"
	.incbin "baserom.gba", 0x00ec288e, 0x00000002
	.section .rom.00ec29cf, "ax"
	.incbin "baserom.gba", 0x00ec29cf, 0x00000001
	.section .rom.00ec2b0f, "ax"
	.incbin "baserom.gba", 0x00ec2b0f, 0x00000001
	.section .rom.00ec2c4f, "ax"
	.incbin "baserom.gba", 0x00ec2c4f, 0x00000001
	.section .rom.00ec3379, "ax"
	.incbin "baserom.gba", 0x00ec3379, 0x00000003
	.section .rom.00ec3387, "ax"
	.incbin "baserom.gba", 0x00ec3387, 0x00000001
	.section .rom.00ec34d6, "ax"
	.incbin "baserom.gba", 0x00ec34d6, 0x00000002
	.section .rom.00ec3617, "ax"
	.incbin "baserom.gba", 0x00ec3617, 0x00000001
	.section .rom.00ec3757, "ax"
	.incbin "baserom.gba", 0x00ec3757, 0x00000001
	.section .rom.00ec3897, "ax"
	.incbin "baserom.gba", 0x00ec3897, 0x00000001
