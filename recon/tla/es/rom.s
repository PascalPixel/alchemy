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
	.incbin "baserom.gba", 0x00024c6e, 0x0000003e
	.section .rom.00024ec6, "ax"
	.incbin "baserom.gba", 0x00024ec6, 0x00000002
	.section .rom.00024efe, "ax"
	.incbin "baserom.gba", 0x00024efe, 0x00000022
	.section .rom.00024fde, "ax"
	.incbin "baserom.gba", 0x00024fde, 0x00000002
	.section .rom.00025160, "ax"
	.incbin "baserom.gba", 0x00025160, 0x000009f8
	.section .rom.00025c8a, "ax"
	.incbin "baserom.gba", 0x00025c8a, 0x00000002
	.section .rom.0002632c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0002632c, 0x00000aa4
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
	.incbin "baserom.gba", 0x00026fc8, 0x00000e44
	.section .rom.00027e20, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00027e20, 0x000019f0
	.section .rom.00029838, "ax"
	.incbin "baserom.gba", 0x00029838, 0x00000cf4
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
	.section .rom.0002e91c, "ax"
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x0002e91c, 0x00000230
	.section .rom.0002eb4c, "ax"
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0002eb4c, 0x0000004c
	.section .rom.0002ec48, "ax"
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x0002ec48, 0x00000058
	.section .rom.0002ece0, "ax"
	.incbin "baserom.gba", 0x0002ece0, 0x000000e4
	.section .rom.0002efc4, "ax"
	.global Map_TerrainHeightHandlers
Map_TerrainHeightHandlers:
	.incbin "baserom.gba", 0x0002efc4, 0x00000040
	.section .rom.0002f004, "ax"
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x0002f004, 0x0000008c
	.section .rom.0002f090, "ax"
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x0002f090, 0x00000140
	.section .rom.0002f1d0, "ax"
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f1d0, 0x00000018
	.section .rom.0002f1e8, "ax"
	.global Data_0802f1e8
Data_0802f1e8:
	.incbin "baserom.gba", 0x0002f1e8, 0x00000018
	.section .rom.0002f200, "ax"
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f200, 0x000000dc
	.section .rom.0002f2dc, "ax"
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x0002f2dc, 0x000000a4
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
	.incbin "baserom.gba", 0x000397e0, 0x000006d4
	.section .rom.00039eb4, "ax"
	.global UiWork_ProcessDirectWork
	.type UiWork_ProcessDirectWork, %function
	.thumb_func
UiWork_ProcessDirectWork:
	.incbin "baserom.gba", 0x00039eb4, 0x00000184
	.section .rom.0003a294, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003a294, 0x00000108
	.section .rom.0003a42a, "ax"
	.incbin "baserom.gba", 0x0003a42a, 0x00000002
	.section .rom.0003a530, "ax"
	.global UiMessage_Show
	.type UiMessage_Show, %function
	.thumb_func
UiMessage_Show:
	.incbin "baserom.gba", 0x0003a530, 0x00000094
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
	.incbin "baserom.gba", 0x0003a790, 0x0000011c
	.section .rom.0003a8e0, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a8e0, 0x000001a8
	.section .rom.0003aaae, "ax"
	.incbin "baserom.gba", 0x0003aaae, 0x00000002
	.section .rom.0003aab0, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aab0, 0x0000020c
	.section .rom.0003acbc, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x0003acbc, 0x00000120
	.section .rom.0003addc, "ax"
	.global UiText_FormatNumber
	.type UiText_FormatNumber, %function
	.thumb_func
UiText_FormatNumber:
	.incbin "baserom.gba", 0x0003addc, 0x00000268
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
	.global UiText_GetResourceDimensionsAlt
	.type UiText_GetResourceDimensionsAlt, %function
	.thumb_func
UiText_GetResourceDimensionsAlt:
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
	.section .rom.0003c1a0, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c1a0, 0x000001e0
	.section .rom.0003c414, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c414, 0x0000013c
	.section .rom.0003c550, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c550, 0x00000464
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
	.section .rom.0003cc06, "ax"
	.incbin "baserom.gba", 0x0003cc06, 0x00000002
	.section .rom.0003cd68, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd68, 0x00000114
	.section .rom.0003cec4, "ax"
	.balign 4
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cec4, 0x000000fc
	.section .rom.0003cfcc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cfcc, 0x00000034
	.section .rom.0003d030, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003d030, 0x00000050
	.section .rom.0003d080, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d080, 0x00000158
	.section .rom.0003d224, "ax"
	.incbin "baserom.gba", 0x0003d224, 0x0000010c
	.section .rom.0003d330, "ax"
	.global UiWork_SetTwoEntriesTo999
	.type UiWork_SetTwoEntriesTo999, %function
	.thumb_func
UiWork_SetTwoEntriesTo999:
	.incbin "baserom.gba", 0x0003d330, 0x00000020
	.section .rom.0003d544, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d544, 0x000000b0
	.section .rom.0003d624, "ax"
	.balign 4
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
	.section .rom.0003da1a, "ax"
	.incbin "baserom.gba", 0x0003da1a, 0x00000002
	.section .rom.0003da1c, "ax"
	.global UiIcon_BuildAbilityIconTiles
	.type UiIcon_BuildAbilityIconTiles, %function
	.thumb_func
UiIcon_BuildAbilityIconTiles:
	.incbin "baserom.gba", 0x0003da1c, 0x000000bc
	.section .rom.0003dc7c, "ax"
	.balign 4
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc7c, 0x00000108
	.section .rom.0003ddf8, "ax"
	.balign 4
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddf8, 0x00000110
	.section .rom.0003df84, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003df84, 0x00000684
	.section .rom.0003e828, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e828, 0x00000124
	.section .rom.0003eab4, "ax"
	.incbin "baserom.gba", 0x0003eab4, 0x00000188
	.section .rom.0003ec3c, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec3c, 0x000001bc
	.section .rom.0003edf8, "ax"
	.global Menu_StepLeft
	.type Menu_StepLeft, %function
	.thumb_func
Menu_StepLeft:
	.incbin "baserom.gba", 0x0003edf8, 0x000001b0
	.section .rom.0003f064, "ax"
	.incbin "baserom.gba", 0x0003f064, 0x000001d0
	.section .rom.0003f344, "ax"
	.incbin "baserom.gba", 0x0003f344, 0x000000e4
	.section .rom.0003f428, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f428, 0x00000248
	.section .rom.0003f684, "ax"
	.balign 4
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f684, 0x00000074
	.section .rom.0003f7d8, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0003f7d8, 0x00000088
	.section .rom.0003f860, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0003f860, 0x00000010
	.section .rom.0003fa20, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa20, 0x000001a0
	.section .rom.0003fc98, "ax"
	.incbin "baserom.gba", 0x0003fc98, 0x000000dc
	.section .rom.0003fd88, "ax"
	.incbin "baserom.gba", 0x0003fd88, 0x000003c0
	.section .rom.00040148, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x00040148, 0x000004c4
	.section .rom.00040688, "ax"
	.incbin "baserom.gba", 0x00040688, 0x00000364
	.section .rom.00040a50, "ax"
	.incbin "baserom.gba", 0x00040a50, 0x00000270
	.section .rom.00040dd0, "ax"
	.incbin "baserom.gba", 0x00040dd0, 0x000000f4
	.section .rom.000410c8, "ax"
	.incbin "baserom.gba", 0x000410c8, 0x0000019c
	.section .rom.000412ec, "ax"
	.incbin "baserom.gba", 0x000412ec, 0x00000440
	.section .rom.0004172c, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0004172c, 0x000003fc
	.section .rom.00041b28, "ax"
	.global UiWindow_ClearTileAttributesInRect
	.type UiWindow_ClearTileAttributesInRect, %function
	.thumb_func
UiWindow_ClearTileAttributesInRect:
	.incbin "baserom.gba", 0x00041b28, 0x000000ac
	.section .rom.00041bd4, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bd4, 0x000000a4
	.section .rom.00041cc0, "ax"
	.balign 4
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
	.section .rom.00042010, "ax"
	.balign 4
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
	.section .rom.00042168, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042168, 0x0000008c
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
	.global CreateSideObject
	.type CreateSideObject, %function
	.thumb_func
CreateSideObject:
	.incbin "baserom.gba", 0x000424bc, 0x000000b8
	.section .rom.00042574, "ax"
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x00042574, 0x00000064
	.section .rom.000425e6, "ax"
	.incbin "baserom.gba", 0x000425e6, 0x00000002
	.section .rom.000425f4, "ax"
	.balign 4
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x000425f4, 0x00000074
	.section .rom.0004274c, "ax"
	.incbin "baserom.gba", 0x0004274c, 0x0000029c
	.section .rom.000429e8, "ax"
	.global PtyStDraw
	.type PtyStDraw, %function
	.thumb_func
PtyStDraw:
	.incbin "baserom.gba", 0x000429e8, 0x00000430
	.section .rom.00042e30, "ax"
	.incbin "baserom.gba", 0x00042e30, 0x00000064
	.section .rom.00042f7c, "ax"
	.global Func_08042f10
	.type Func_08042f10, %function
	.thumb_func
Func_08042f10:
	.incbin "baserom.gba", 0x00042f7c, 0x000000e0
	.section .rom.0004305c, "ax"
	.global Func_08042ff0
	.type Func_08042ff0, %function
	.thumb_func
Func_08042ff0:
	.incbin "baserom.gba", 0x0004305c, 0x000000d4
	.section .rom.00043130, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043130, 0x000000ec
	.section .rom.0004321c, "ax"
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x0004321c, 0x00000080
	.section .rom.00043310, "ax"
	.balign 4
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x00043310, 0x000000b4
	.section .rom.000433c4, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x000433c4, 0x00000194
	.section .rom.000435a0, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x000435a0, 0x000000c8
	.section .rom.000436fc, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436fc, 0x000000e4
	.section .rom.00043ad0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ad0, 0x000000d0
	.section .rom.00043bdc, "ax"
	.global Func_08043b70
	.type Func_08043b70, %function
	.thumb_func
Func_08043b70:
	.incbin "baserom.gba", 0x00043bdc, 0x0000007c
	.section .rom.00043d44, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d44, 0x0000066c
	.section .rom.000443b4, "ax"
	.incbin "baserom.gba", 0x000443b4, 0x00000090
	.section .rom.000445c4, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445c4, 0x0000051c
	.section .rom.00044ae4, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044ae4, 0x00000128
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
	.incbin "baserom.gba", 0x00044e54, 0x00000184
	.section .rom.000450a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000450a4, 0x00000030
	.section .rom.00045188, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x00045188, 0x000000a0
	.section .rom.00045248, "ax"
	.incbin "baserom.gba", 0x00045248, 0x00000090
	.section .rom.00045346, "ax"
	.incbin "baserom.gba", 0x00045346, 0x00000002
	.section .rom.00045348, "ax"
	.global Graphics_MergeShiftedTileRows
	.type Graphics_MergeShiftedTileRows, %function
	.thumb_func
Graphics_MergeShiftedTileRows:
	.incbin "baserom.gba", 0x00045348, 0x00000074
	.section .rom.000455b2, "ax"
	.incbin "baserom.gba", 0x000455b2, 0x00000002
	.section .rom.000455f0, "ax"
	.incbin "baserom.gba", 0x000455f0, 0x0000004c
	.section .rom.00045784, "ax"
	.incbin "baserom.gba", 0x00045784, 0x00000070
	.section .rom.0004580a, "ax"
	.incbin "baserom.gba", 0x0004580a, 0x00000002
	.section .rom.0004585a, "ax"
	.incbin "baserom.gba", 0x0004585a, 0x00000966
	.section .rom.00046310, "ax"
	.incbin "baserom.gba", 0x00046310, 0x00000150
	.section .rom.0004649e, "ax"
	.incbin "baserom.gba", 0x0004649e, 0x0000364e
	.section .rom.00049b3a, "ax"
	.incbin "baserom.gba", 0x00049b3a, 0x00000002
	.section .rom.00049b68, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00049b68, 0x00001d54
	.section .rom.0004b9dc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004b9dc, 0x000000bc
	.section .rom.0004bad2, "ax"
	.incbin "baserom.gba", 0x0004bad2, 0x000002f2
	.section .rom.0004bdec, "ax"
	.incbin "baserom.gba", 0x0004bdec, 0x00000038
	.section .rom.0004be24, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be24, 0x000011a0
	.section .rom.0004cfc4, "ax"
	.global Func_0804cda8
	.type Func_0804cda8, %function
	.thumb_func
Func_0804cda8:
	.incbin "baserom.gba", 0x0004cfc4, 0x00000334
	.section .rom.0004d388, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d388, 0x00000120
	.section .rom.0004d4a8, "ax"
	.global Menu_SelectResource
	.type Menu_SelectResource, %function
	.thumb_func
Menu_SelectResource:
	.incbin "baserom.gba", 0x0004d4a8, 0x000000b8
	.section .rom.0004d604, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d604, 0x0000009c
	.section .rom.0004d6a0, "ax"
	.global Menu_LayoutResourceEntries
	.type Menu_LayoutResourceEntries, %function
	.thumb_func
Menu_LayoutResourceEntries:
	.incbin "baserom.gba", 0x0004d6a0, 0x00000074
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
	.section .rom.0004dcae, "ax"
	.incbin "baserom.gba", 0x0004dcae, 0x00000002
	.section .rom.0004dd68, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x0004dd68, 0x00000254
	.section .rom.0004e094, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e094, 0x000000c4
	.section .rom.0004e2c2, "ax"
	.incbin "baserom.gba", 0x0004e2c2, 0x00000002
	.section .rom.0004e2c4, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2c4, 0x0000007c
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
	.incbin "baserom.gba", 0x0004e5e8, 0x00000190
	.section .rom.0004e878, "ax"
	.incbin "baserom.gba", 0x0004e878, 0x000004d4
	.section .rom.000599e4, "ax"
	.incbin "baserom.gba", 0x000599e4, 0x00000084
	.section .rom.00059b68, "ax"
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b68, 0x00000bf8
	.section .rom.0005c360, "ax"
	.incbin "baserom.gba", 0x0005c360, 0x0000007c
	.section .rom.0005c464, "ax"
	.incbin "baserom.gba", 0x0005c464, 0x00000010
	.section .rom.0005ec74, "ax"
	.incbin "baserom.gba", 0x0005ec74, 0x0000018c
	.section .rom.0005ee00, "ax"
	.global Data_0805eb50
Data_0805eb50:
	.incbin "baserom.gba", 0x0005ee00, 0x00000008
	.section .rom.0005ee08, "ax"
	.global Data_0805eb58
Data_0805eb58:
	.incbin "baserom.gba", 0x0005ee08, 0x00000024
	.section .rom.0005ee2c, "ax"
	.global Data_0805eb7c
Data_0805eb7c:
	.incbin "baserom.gba", 0x0005ee2c, 0x00000108
	.section .rom.0005f734, "ax"
	.incbin "baserom.gba", 0x0005f734, 0x00000101
	.section .rom.0005f835, "ax"
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x0005f835, 0x000000bb
	.section .rom.0005f8f0, "ax"
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f8f0, 0x00000030
	.section .rom.0005f920, "ax"
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x0005f920, 0x00000078
	.section .rom.0005fa68, "ax"
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fa68, 0x00000008
	.section .rom.0005fb10, "ax"
	.incbin "baserom.gba", 0x0005fb10, 0x0000005e
	.section .rom.0005fb6e, "ax"
	.global Menu_TopEntryCommandByPosition
	.type Menu_TopEntryCommandByPosition, %function
	.thumb_func
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fb6e, 0x0000000c
	.section .rom.0005fb7a, "ax"
	.global Menu_TopEntryPositionByCommand
	.type Menu_TopEntryPositionByCommand, %function
	.thumb_func
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fb7a, 0x00000026
	.section .rom.0005fba0, "ax"
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0005fba0, 0x0000003c
	.section .rom.000a8fd8, "ax"
	.incbin "baserom.gba", 0x000a8fd8, 0x0000000c
	.section .rom.000a9024, "ax"
	.incbin "baserom.gba", 0x000a9024, 0x000002c0
	.section .rom.000a92e4, "ax"
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a92e4, 0x00000004
	.section .rom.000b6070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000b6070, 0x00000008
	.section .rom.000b6078, "ax"
	.global BtlActGet
	.type BtlActGet, %function
	.thumb_func
BtlActGet:
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
	.incbin "baserom.gba", 0x000b6088, 0x00000008
	.section .rom.000b6090, "ax"
	.global Func_080ad090
	.type Func_080ad090, %function
	.thumb_func
Func_080ad090:
	.incbin "baserom.gba", 0x000b6090, 0x00000008
	.section .rom.000b6098, "ax"
	.global Func_080ad098
	.type Func_080ad098, %function
	.thumb_func
Func_080ad098:
	.incbin "baserom.gba", 0x000b6098, 0x00000008
	.section .rom.000b60a0, "ax"
	.global Func_080ad0a0
	.type Func_080ad0a0, %function
	.thumb_func
Func_080ad0a0:
	.incbin "baserom.gba", 0x000b60a0, 0x00000008
	.section .rom.000b60a8, "ax"
	.global Func_080ad0a8
	.type Func_080ad0a8, %function
	.thumb_func
Func_080ad0a8:
	.incbin "baserom.gba", 0x000b60a8, 0x00000008
	.section .rom.000b60c0, "ax"
	.global Owner_AdjustFirstValueFar
	.type Owner_AdjustFirstValueFar, %function
	.thumb_func
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000b60c0, 0x00000008
	.section .rom.000b60c8, "ax"
	.global UnitEpAddFar
	.type UnitEpAddFar, %function
	.thumb_func
UnitEpAddFar:
	.incbin "baserom.gba", 0x000b60c8, 0x00000008
	.section .rom.000b60d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000b60d0, 0x00000008
	.section .rom.000b60d8, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000b60d8, 0x00000008
	.section .rom.000b60e0, "ax"
	.global Owner_UpdateSecondInputAndRatiosFar
	.type Owner_UpdateSecondInputAndRatiosFar, %function
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000b60e0, 0x00000008
	.section .rom.000b60e8, "ax"
	.global BtlUnitSetFar
	.type BtlUnitSetFar, %function
	.thumb_func
BtlUnitSetFar:
	.incbin "baserom.gba", 0x000b60e8, 0x00000008
	.section .rom.000b60f0, "ax"
	.global Party_CountActiveOwnersFar
	.type Party_CountActiveOwnersFar, %function
	.thumb_func
Party_CountActiveOwnersFar:
	.incbin "baserom.gba", 0x000b60f0, 0x00000008
	.section .rom.000b60f8, "ax"
	.global Party_AddActiveOwnerFar
	.type Party_AddActiveOwnerFar, %function
	.thumb_func
Party_AddActiveOwnerFar:
	.incbin "baserom.gba", 0x000b60f8, 0x00000008
	.section .rom.000b6100, "ax"
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000b6100, 0x00000008
	.section .rom.000b6108, "ax"
	.global RollWeaponUnleashFar
	.type RollWeaponUnleashFar, %function
	.thumb_func
RollWeaponUnleashFar:
	.incbin "baserom.gba", 0x000b6108, 0x00000008
	.section .rom.000b6110, "ax"
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000b6110, 0x00000008
	.section .rom.000b6118, "ax"
	.global Item_GetEquippedElementFar
	.type Item_GetEquippedElementFar, %function
	.thumb_func
Item_GetEquippedElementFar:
	.incbin "baserom.gba", 0x000b6118, 0x00000008
	.section .rom.000b6120, "ax"
	.global BtlHitChk
	.type BtlHitChk, %function
	.thumb_func
BtlHitChk:
	.incbin "baserom.gba", 0x000b6120, 0x00000008
	.section .rom.000b6128, "ax"
	.global BtlAtkCalc
	.type BtlAtkCalc, %function
	.thumb_func
BtlAtkCalc:
	.incbin "baserom.gba", 0x000b6128, 0x00000008
	.section .rom.000b6130, "ax"
	.global BtlPowCalc
	.type BtlPowCalc, %function
	.thumb_func
BtlPowCalc:
	.incbin "baserom.gba", 0x000b6130, 0x00000008
	.section .rom.000b6138, "ax"
	.global BtlRcvCalc
	.type BtlRcvCalc, %function
	.thumb_func
BtlRcvCalc:
	.incbin "baserom.gba", 0x000b6138, 0x00000008
	.section .rom.000b6140, "ax"
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000b6140, 0x00000008
	.section .rom.000b6148, "ax"
	.global BtlRndFar
	.type BtlRndFar, %function
	.thumb_func
BtlRndFar:
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
	.incbin "baserom.gba", 0x000b6158, 0x00000008
	.section .rom.000b6160, "ax"
	.global Djinn_DeactivateFar
	.type Djinn_DeactivateFar, %function
	.thumb_func
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000b6160, 0x00000008
	.section .rom.000b6168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000b6168, 0x00000008
	.section .rom.000b6170, "ax"
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000b6170, 0x00000008
	.section .rom.000b6178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000b6178, 0x00000008
	.section .rom.000b6180, "ax"
	.global Trade_ListFlaggedEntriesFar
	.type Trade_ListFlaggedEntriesFar, %function
	.thumb_func
Trade_ListFlaggedEntriesFar:
	.incbin "baserom.gba", 0x000b6180, 0x00000008
	.section .rom.000b6188, "ax"
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000b6188, 0x00000008
	.section .rom.000b6190, "ax"
	.global Djinn_GetDefinitionHeaderFar
	.type Djinn_GetDefinitionHeaderFar, %function
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000b6190, 0x00000008
	.section .rom.000b6198, "ax"
	.global Party_AdvanceOwnerCountToTargetFar
	.type Party_AdvanceOwnerCountToTargetFar, %function
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000b6198, 0x00000008
	.section .rom.000b61a0, "ax"
	.global Owner_GetResistanceValueFar
	.type Owner_GetResistanceValueFar, %function
	.thumb_func
Owner_GetResistanceValueFar:
	.incbin "baserom.gba", 0x000b61a0, 0x00000008
	.section .rom.000b61a8, "ax"
	.global Trade_CountPendingOffersFar
	.type Trade_CountPendingOffersFar, %function
	.thumb_func
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000b61a8, 0x00000008
	.section .rom.000b61b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000b61b0, 0x00000008
	.section .rom.000b61b8, "ax"
	.global Trade_CanOfferDjinnFar
	.type Trade_CanOfferDjinnFar, %function
	.thumb_func
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000b61b8, 0x00000008
	.section .rom.000b61c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000b61c0, 0x00000008
	.section .rom.000b61c8, "ax"
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000b61c8, 0x00000008
	.section .rom.000b61d0, "ax"
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000b61d0, 0x00000008
	.section .rom.000b61d8, "ax"
	.global Party_AdjustCoinsFar
	.type Party_AdjustCoinsFar, %function
	.thumb_func
Party_AdjustCoinsFar:
	.incbin "baserom.gba", 0x000b61d8, 0x00000008
	.section .rom.000b61e0, "ax"
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000b61e0, 0x00000008
	.section .rom.000b61e8, "ax"
	.global Item_AdjustCounterFar
	.type Item_AdjustCounterFar, %function
	.thumb_func
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000b61e8, 0x00000008
	.section .rom.000b61f0, "ax"
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x000b61f0, 0x00000008
	.section .rom.000b61f8, "ax"
	.global PartyInventory_HasSpaceFar
	.type PartyInventory_HasSpaceFar, %function
	.thumb_func
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000b61f8, 0x00000008
	.section .rom.000b6200, "ax"
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000b6200, 0x00000008
	.section .rom.000b6208, "ax"
	.global Func_080ad208
	.type Func_080ad208, %function
	.thumb_func
Func_080ad208:
	.incbin "baserom.gba", 0x000b6208, 0x00000008
	.section .rom.000b6210, "ax"
	.global Func_080ad210
	.type Func_080ad210, %function
	.thumb_func
Func_080ad210:
	.incbin "baserom.gba", 0x000b6210, 0x00000008
	.section .rom.000b6218, "ax"
	.global Party_SetLeaderJasmineFar
	.type Party_SetLeaderJasmineFar, %function
	.thumb_func
Party_SetLeaderJasmineFar:
	.incbin "baserom.gba", 0x000b6218, 0x00000008
	.section .rom.000b6220, "ax"
	.global Party_SetLeaderGarciaFar
	.type Party_SetLeaderGarciaFar, %function
	.thumb_func
Party_SetLeaderGarciaFar:
	.incbin "baserom.gba", 0x000b6220, 0x00000008
	.section .rom.000b6228, "ax"
	.global Func_080ad228
	.type Func_080ad228, %function
	.thumb_func
Func_080ad228:
	.incbin "baserom.gba", 0x000b6228, 0x00000008
	.section .rom.000b6230, "ax"
	.global Func_080ad230
	.type Func_080ad230, %function
	.thumb_func
Func_080ad230:
	.incbin "baserom.gba", 0x000b6230, 0x00000008
	.section .rom.000b6238, "ax"
	.global Func_080ad238
	.type Func_080ad238, %function
	.thumb_func
Func_080ad238:
	.incbin "baserom.gba", 0x000b6238, 0x00000008
	.section .rom.000b6240, "ax"
	.global Func_080ad240
	.type Func_080ad240, %function
	.thumb_func
Func_080ad240:
	.incbin "baserom.gba", 0x000b6240, 0x00000008
	.section .rom.000b6248, "ax"
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000b6248, 0x00000008
	.section .rom.000b6250, "ax"
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000b6250, 0x00000008
	.section .rom.000b6258, "ax"
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000b6258, 0x00000008
	.section .rom.000b6260, "ax"
	.global Party_AdjustCounterCappedAt28Far
	.type Party_AdjustCounterCappedAt28Far, %function
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000b6260, 0x00000008
	.section .rom.000b6268, "ax"
	.global Func_080ad268
	.type Func_080ad268, %function
	.thumb_func
Func_080ad268:
	.incbin "baserom.gba", 0x000b6268, 0x00000008
	.section .rom.000b6270, "ax"
	.global Func_080ad270
	.type Func_080ad270, %function
	.thumb_func
Func_080ad270:
	.incbin "baserom.gba", 0x000b6270, 0x00000008
	.section .rom.000b6278, "ax"
	.global Inventory_DiscardFar
	.type Inventory_DiscardFar, %function
	.thumb_func
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000b6278, 0x00000008
	.section .rom.000b6280, "ax"
	.global BtlEfxRevChkFar
	.type BtlEfxRevChkFar, %function
	.thumb_func
BtlEfxRevChkFar:
	.incbin "baserom.gba", 0x000b6280, 0x00000008
	.section .rom.000b6288, "ax"
	.global Owner_RefreshClassActionsFar
	.type Owner_RefreshClassActionsFar, %function
	.thumb_func
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000b6288, 0x00000008
	.section .rom.000b6290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000b6290, 0x00000008
	.section .rom.000b6298, "ax"
	.global Owner_RefreshDerivedDataFar
	.type Owner_RefreshDerivedDataFar, %function
	.thumb_func
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000b6298, 0x00000008
	.section .rom.000b62a0, "ax"
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000b62a0, 0x00000008
	.section .rom.000b62a8, "ax"
	.global PartyInventory_CountItemFar
	.type PartyInventory_CountItemFar, %function
	.thumb_func
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000b62a8, 0x00000008
	.section .rom.000b62b0, "ax"
	.global PartyInventory_DiscardFar
	.type PartyInventory_DiscardFar, %function
	.thumb_func
PartyInventory_DiscardFar:
	.incbin "baserom.gba", 0x000b62b0, 0x00000008
	.section .rom.000b63f6, "ax"
	.incbin "baserom.gba", 0x000b63f6, 0x00000002
	.section .rom.000b63f8, "ax"
	.global UnitCalc
	.type UnitCalc, %function
	.thumb_func
UnitCalc:
	.incbin "baserom.gba", 0x000b63f8, 0x000007f4
	.section .rom.000b6bec, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000b6bec, 0x000000a4
	.section .rom.000b6df0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000b6df0, 0x000002ec
	.section .rom.000b7358, "ax"
	.global Func_080ae358
	.type Func_080ae358, %function
	.thumb_func
Func_080ae358:
	.incbin "baserom.gba", 0x000b7358, 0x000000a4
	.section .rom.000b7410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000b7410, 0x000001ec
	.section .rom.000b75fc, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000b75fc, 0x000000cc
	.section .rom.000b76e4, "ax"
	.global Func_080ae6e4
	.type Func_080ae6e4, %function
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000b76e4, 0x00000158
	.section .rom.000b7874, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000b7874, 0x000001c8
	.section .rom.000b7c72, "ax"
	.incbin "baserom.gba", 0x000b7c72, 0x00000002
	.section .rom.000b7c74, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000b7c74, 0x0000003c
	.section .rom.000b7f40, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x000b7f40, 0x000000d4
	.section .rom.000b80ee, "ax"
	.incbin "baserom.gba", 0x000b80ee, 0x00000002
	.section .rom.000b80f0, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.incbin "baserom.gba", 0x000b80f0, 0x00000064
	.section .rom.000b8154, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000b8154, 0x00000080
	.section .rom.000b84f0, "ax"
	.balign 4
	.global Owner_UpdateClassActions
	.type Owner_UpdateClassActions, %function
	.thumb_func
Owner_UpdateClassActions:
	.incbin "baserom.gba", 0x000b84f0, 0x00000210
	.section .rom.000b87b8, "ax"
	.incbin "baserom.gba", 0x000b87b8, 0x00000120
	.section .rom.000b8928, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000b8928, 0x00000264
	.section .rom.000b8b8c, "ax"
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000b8b8c, 0x00000034
	.section .rom.000b8bf8, "ax"
	.incbin "baserom.gba", 0x000b8bf8, 0x000001d0
	.section .rom.000b8f34, "ax"
	.incbin "baserom.gba", 0x000b8f34, 0x00000054
	.section .rom.000b8fb8, "ax"
	.balign 4
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000b8fb8, 0x0000007c
	.section .rom.000b9066, "ax"
	.incbin "baserom.gba", 0x000b9066, 0x00000002
	.section .rom.000b9068, "ax"
	.global Owner_GetDefaultElement
	.type Owner_GetDefaultElement, %function
	.thumb_func
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000b9068, 0x00000028
	.section .rom.000b9090, "ax"
	.global Owner_CalculateBaseElements
	.type Owner_CalculateBaseElements, %function
	.thumb_func
Owner_CalculateBaseElements:
	.incbin "baserom.gba", 0x000b9090, 0x000000c0
	.section .rom.000b9150, "ax"
	.global Owner_DetermineClass
	.type Owner_DetermineClass, %function
	.thumb_func
Owner_DetermineClass:
	.incbin "baserom.gba", 0x000b9150, 0x00000144
	.section .rom.000b9520, "ax"
	.incbin "baserom.gba", 0x000b9520, 0x00000194
	.section .rom.000b9768, "ax"
	.align 2, 0
	.incbin "baserom.gba", 0x000b9768, 0x00000338
	.section .rom.000b9bc2, "ax"
	.incbin "baserom.gba", 0x000b9bc2, 0x00000002
	.section .rom.000b9bc4, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000b9bc4, 0x000000c0
	.section .rom.000b9d64, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b9d64, 0x000000ac
	.section .rom.000ba094, "ax"
	.balign 4
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000ba094, 0x00000170
	.section .rom.000ba204, "ax"
	.global Func_080b11f8
	.type Func_080b11f8, %function
	.thumb_func
Func_080b11f8:
	.incbin "baserom.gba", 0x000ba204, 0x00000084
	.section .rom.000ba288, "ax"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000ba288, 0x0000004c
	.section .rom.000baf34, "ax"
	.incbin "baserom.gba", 0x000baf34, 0x00000004
	.section .rom.000cfb10, "ax"
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000cfb10, 0x00000014
	.section .rom.000cfb24, "ax"
	.global Battle_Effect90Classes
Battle_Effect90Classes:
	.incbin "baserom.gba", 0x000cfb24, 0x0000005e
	.section .rom.000cfb82, "ax"
	.global Battle_Effect91Classes
Battle_Effect91Classes:
	.incbin "baserom.gba", 0x000cfb82, 0x0000003a
	.section .rom.000d1000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000d1000, 0x00000008
	.section .rom.000d1008, "ax"
	.global Game_ResetForNewGameFar
	.type Game_ResetForNewGameFar, %function
	.thumb_func
Game_ResetForNewGameFar:
	.incbin "baserom.gba", 0x000d1008, 0x00000008
	.section .rom.000d1010, "ax"
	.global Battle_WaitMode0Far
	.type Battle_WaitMode0Far, %function
	.thumb_func
Battle_WaitMode0Far:
	.incbin "baserom.gba", 0x000d1010, 0x00000008
	.section .rom.000d1018, "ax"
	.global EventRuntime_BeginFar
	.type EventRuntime_BeginFar, %function
	.thumb_func
EventRuntime_BeginFar:
	.incbin "baserom.gba", 0x000d1018, 0x00000008
	.section .rom.000d1020, "ax"
	.global EventRuntime_EndFar
	.type EventRuntime_EndFar, %function
	.thumb_func
EventRuntime_EndFar:
	.incbin "baserom.gba", 0x000d1020, 0x00000008
	.section .rom.000d1028, "ax"
	.global Event_RunObjectHookAndWaitFar
	.type Event_RunObjectHookAndWaitFar, %function
	.thumb_func
Event_RunObjectHookAndWaitFar:
	.incbin "baserom.gba", 0x000d1028, 0x00000008
	.section .rom.000d1030, "ax"
	.global Event_CallWithLastActiveObjectIdFar
	.type Event_CallWithLastActiveObjectIdFar, %function
	.thumb_func
Event_CallWithLastActiveObjectIdFar:
	.incbin "baserom.gba", 0x000d1030, 0x00000008
	.section .rom.000d1038, "ax"
	.global Scene_AssignViewFlagsFar
	.type Scene_AssignViewFlagsFar, %function
	.thumb_func
Scene_AssignViewFlagsFar:
	.incbin "baserom.gba", 0x000d1038, 0x00000008
	.section .rom.000d1040, "ax"
	.global Event_SetSceneEventsFar
	.type Event_SetSceneEventsFar, %function
	.thumb_func
Event_SetSceneEventsFar:
	.incbin "baserom.gba", 0x000d1040, 0x00000008
	.section .rom.000d1048, "ax"
	.global Event_PrepareObjectAndApplyValueFar
	.type Event_PrepareObjectAndApplyValueFar, %function
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	.incbin "baserom.gba", 0x000d1048, 0x00000008
	.section .rom.000d1050, "ax"
	.global Event_PrepareTwoObjectsAndApplyFar
	.type Event_PrepareTwoObjectsAndApplyFar, %function
	.thumb_func
Event_PrepareTwoObjectsAndApplyFar:
	.incbin "baserom.gba", 0x000d1050, 0x00000008
	.section .rom.000d1058, "ax"
	.global Party_RemoveOwnerRestoredFar
	.type Party_RemoveOwnerRestoredFar, %function
	.thumb_func
Party_RemoveOwnerRestoredFar:
	.incbin "baserom.gba", 0x000d1058, 0x00000008
	.section .rom.000d1060, "ax"
	.global PartyInventory_GiveItemFar
	.type PartyInventory_GiveItemFar, %function
	.thumb_func
PartyInventory_GiveItemFar:
	.incbin "baserom.gba", 0x000d1060, 0x00000008
	.section .rom.000d1068, "ax"
	.global Inventory_TryAddAndReturnOwnerFar
	.type Inventory_TryAddAndReturnOwnerFar, %function
	.thumb_func
Inventory_TryAddAndReturnOwnerFar:
	.incbin "baserom.gba", 0x000d1068, 0x00000008
	.section .rom.000d1070, "ax"
	.global Inventory_PromptAndSetObjectModeFar
	.type Inventory_PromptAndSetObjectModeFar, %function
	.thumb_func
Inventory_PromptAndSetObjectModeFar:
	.incbin "baserom.gba", 0x000d1070, 0x00000008
	.section .rom.000d1078, "ax"
	.global Func_080c8078
	.type Func_080c8078, %function
	.thumb_func
Func_080c8078:
	.incbin "baserom.gba", 0x000d1078, 0x00000008
	.section .rom.000d1080, "ax"
	.global UiText_DrawQuantityPairWithCueFar
	.type UiText_DrawQuantityPairWithCueFar, %function
	.thumb_func
UiText_DrawQuantityPairWithCueFar:
	.incbin "baserom.gba", 0x000d1080, 0x00000008
	.section .rom.000d1088, "ax"
	.global Object_GetByIdFar
	.type Object_GetByIdFar, %function
	.thumb_func
Object_GetByIdFar:
	.incbin "baserom.gba", 0x000d1088, 0x00000008
	.section .rom.000d1090, "ax"
	.global ObjectTable_DestroyByIdFar
	.type ObjectTable_DestroyByIdFar, %function
	.thumb_func
ObjectTable_DestroyByIdFar:
	.incbin "baserom.gba", 0x000d1090, 0x00000008
	.section .rom.000d1098, "ax"
	.global ObjectMotion_SetSpeedParametersFar
	.type ObjectMotion_SetSpeedParametersFar, %function
	.thumb_func
ObjectMotion_SetSpeedParametersFar:
	.incbin "baserom.gba", 0x000d1098, 0x00000008
	.section .rom.000d10a0, "ax"
	.global ObjectMotion_EnableActionAndSetCallbackFar
	.type ObjectMotion_EnableActionAndSetCallbackFar, %function
	.thumb_func
ObjectMotion_EnableActionAndSetCallbackFar:
	.incbin "baserom.gba", 0x000d10a0, 0x00000008
	.section .rom.000d10a8, "ax"
	.global Object_RefreshSelectorByIdFar
	.type Object_RefreshSelectorByIdFar, %function
	.thumb_func
Object_RefreshSelectorByIdFar:
	.incbin "baserom.gba", 0x000d10a8, 0x00000008
	.section .rom.000d10b0, "ax"
	.global ObjectMotion_EnableActionAndResetMotionFar
	.type ObjectMotion_EnableActionAndResetMotionFar, %function
	.thumb_func
ObjectMotion_EnableActionAndResetMotionFar:
	.incbin "baserom.gba", 0x000d10b0, 0x00000008
	.section .rom.000d10b8, "ax"
	.global Object_SetActionCallbackAndRefreshByIdFar
	.type Object_SetActionCallbackAndRefreshByIdFar, %function
	.thumb_func
Object_SetActionCallbackAndRefreshByIdFar:
	.incbin "baserom.gba", 0x000d10b8, 0x00000008
	.section .rom.000d10c0, "ax"
	.global ObjectMotion_ResetAndSetPositionFar
	.type ObjectMotion_ResetAndSetPositionFar, %function
	.thumb_func
ObjectMotion_ResetAndSetPositionFar:
	.incbin "baserom.gba", 0x000d10c0, 0x00000008
	.section .rom.000d10c8, "ax"
	.global ObjectMotion_SetPositionAndCommitFar
	.type ObjectMotion_SetPositionAndCommitFar, %function
	.thumb_func
ObjectMotion_SetPositionAndCommitFar:
	.incbin "baserom.gba", 0x000d10c8, 0x00000008
	.section .rom.000d10d0, "ax"
	.global ObjectMotion_ResetAndSetPositionInMode2Far
	.type ObjectMotion_ResetAndSetPositionInMode2Far, %function
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2Far:
	.incbin "baserom.gba", 0x000d10d0, 0x00000008
	.section .rom.000d10d8, "ax"
	.global ObjectMotion_SetPositionAndResetFar
	.type ObjectMotion_SetPositionAndResetFar, %function
	.thumb_func
ObjectMotion_SetPositionAndResetFar:
	.incbin "baserom.gba", 0x000d10d8, 0x00000008
	.section .rom.000d10e0, "ax"
	.global ObjectMotion_CenterOnColumnFar
	.type ObjectMotion_CenterOnColumnFar, %function
	.thumb_func
ObjectMotion_CenterOnColumnFar:
	.incbin "baserom.gba", 0x000d10e0, 0x00000008
	.section .rom.000d10e8, "ax"
	.global ObjectMotion_OffsetPositionAndResetMotionFar
	.type ObjectMotion_OffsetPositionAndResetMotionFar, %function
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotionFar:
	.incbin "baserom.gba", 0x000d10e8, 0x00000008
	.section .rom.000d10f0, "ax"
	.global ObjectMotion_CommitCurrentPositionAndActivateFar
	.type ObjectMotion_CommitCurrentPositionAndActivateFar, %function
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivateFar:
	.incbin "baserom.gba", 0x000d10f0, 0x00000008
	.section .rom.000d10f8, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrainFar
	.type ObjectMotion_SetHorizontalPositionWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrainFar:
	.incbin "baserom.gba", 0x000d10f8, 0x00000008
	.section .rom.000d1100, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar:
	.incbin "baserom.gba", 0x000d1100, 0x00000008
	.section .rom.000d1108, "ax"
	.global ObjectMotion_SetPositionWithTerrainFar
	.type ObjectMotion_SetPositionWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrainFar:
	.incbin "baserom.gba", 0x000d1108, 0x00000008
	.section .rom.000d1110, "ax"
	.global ObjectMotion_MoveToObjectFar
	.type ObjectMotion_MoveToObjectFar, %function
	.thumb_func
ObjectMotion_MoveToObjectFar:
	.incbin "baserom.gba", 0x000d1110, 0x00000008
	.section .rom.000d1118, "ax"
	.global Object_SetModeByIdFar
	.type Object_SetModeByIdFar, %function
	.thumb_func
Object_SetModeByIdFar:
	.incbin "baserom.gba", 0x000d1118, 0x00000008
	.section .rom.000d1120, "ax"
	.global Object_SetActionByIdFar
	.type Object_SetActionByIdFar, %function
	.thumb_func
Object_SetActionByIdFar:
	.incbin "baserom.gba", 0x000d1120, 0x00000008
	.section .rom.000d1128, "ax"
	.global Motion_SetModeAndWaitAnimationFar
	.type Motion_SetModeAndWaitAnimationFar, %function
	.thumb_func
Motion_SetModeAndWaitAnimationFar:
	.incbin "baserom.gba", 0x000d1128, 0x00000008
	.section .rom.000d1130, "ax"
	.global ObjectMotion_WaitForAnimationChangeFar
	.type ObjectMotion_WaitForAnimationChangeFar, %function
	.thumb_func
ObjectMotion_WaitForAnimationChangeFar:
	.incbin "baserom.gba", 0x000d1130, 0x00000008
	.section .rom.000d1138, "ax"
	.global ObjectMotion_LaunchFar
	.type ObjectMotion_LaunchFar, %function
	.thumb_func
ObjectMotion_LaunchFar:
	.incbin "baserom.gba", 0x000d1138, 0x00000008
	.section .rom.000d1140, "ax"
	.global ObjectMotion_SetVariantCallbackFar
	.type ObjectMotion_SetVariantCallbackFar, %function
	.thumb_func
ObjectMotion_SetVariantCallbackFar:
	.incbin "baserom.gba", 0x000d1140, 0x00000008
	.section .rom.000d1148, "ax"
	.global Motion_SetVarCbAndRefreshFar
	.type Motion_SetVarCbAndRefreshFar, %function
	.thumb_func
Motion_SetVarCbAndRefreshFar:
	.incbin "baserom.gba", 0x000d1148, 0x00000008
	.section .rom.000d1150, "ax"
	.global BattleFx_RunRisingObjectSequenceFar
	.type BattleFx_RunRisingObjectSequenceFar, %function
	.thumb_func
BattleFx_RunRisingObjectSequenceFar:
	.incbin "baserom.gba", 0x000d1150, 0x00000008
	.section .rom.000d1158, "ax"
	.global ObjectMotion_SetAngleTowardFar
	.type ObjectMotion_SetAngleTowardFar, %function
	.thumb_func
ObjectMotion_SetAngleTowardFar:
	.incbin "baserom.gba", 0x000d1158, 0x00000008
	.section .rom.000d1160, "ax"
	.global Object_LinkPairFar
	.type Object_LinkPairFar, %function
	.thumb_func
Object_LinkPairFar:
	.incbin "baserom.gba", 0x000d1160, 0x00000008
	.section .rom.000d1168, "ax"
	.global ObjectGroup_ConfigureChildValueFar
	.type ObjectGroup_ConfigureChildValueFar, %function
	.thumb_func
ObjectGroup_ConfigureChildValueFar:
	.incbin "baserom.gba", 0x000d1168, 0x00000008
	.section .rom.000d1170, "ax"
	.global Object_SetPartAttributeFar
	.type Object_SetPartAttributeFar, %function
	.thumb_func
Object_SetPartAttributeFar:
	.incbin "baserom.gba", 0x000d1170, 0x00000008
	.section .rom.000d1178, "ax"
	.global Func_080c8178
	.type Func_080c8178, %function
	.thumb_func
Func_080c8178:
	.incbin "baserom.gba", 0x000d1178, 0x00000008
	.section .rom.000d1180, "ax"
	.global EventRuntime_SetMessageFar
	.type EventRuntime_SetMessageFar, %function
	.thumb_func
EventRuntime_SetMessageFar:
	.incbin "baserom.gba", 0x000d1180, 0x00000008
	.section .rom.000d1188, "ax"
	.global UiText_OpenMessageAtObjectFar
	.type UiText_OpenMessageAtObjectFar, %function
	.thumb_func
UiText_OpenMessageAtObjectFar:
	.incbin "baserom.gba", 0x000d1188, 0x00000008
	.section .rom.000d1190, "ax"
	.global Func_080c8190
	.type Func_080c8190, %function
	.thumb_func
Func_080c8190:
	.incbin "baserom.gba", 0x000d1190, 0x00000008
	.section .rom.000d1198, "ax"
	.global Func_080c8198
	.type Func_080c8198, %function
	.thumb_func
Func_080c8198:
	.incbin "baserom.gba", 0x000d1198, 0x00000008
	.section .rom.000d11a0, "ax"
	.global EventRuntime_ShowMessageAndWaitFar
	.type EventRuntime_ShowMessageAndWaitFar, %function
	.thumb_func
EventRuntime_ShowMessageAndWaitFar:
	.incbin "baserom.gba", 0x000d11a0, 0x00000008
	.section .rom.000d11a8, "ax"
	.global Func_080c81a8
	.type Func_080c81a8, %function
	.thumb_func
Func_080c81a8:
	.incbin "baserom.gba", 0x000d11a8, 0x00000008
	.section .rom.000d11b0, "ax"
	.global Func_080c81b0
	.type Func_080c81b0, %function
	.thumb_func
Func_080c81b0:
	.incbin "baserom.gba", 0x000d11b0, 0x00000008
	.section .rom.000d11b8, "ax"
	.global Func_080c81b8
	.type Func_080c81b8, %function
	.thumb_func
Func_080c81b8:
	.incbin "baserom.gba", 0x000d11b8, 0x00000008
	.section .rom.000d11c0, "ax"
	.global Func_080c81c0
	.type Func_080c81c0, %function
	.thumb_func
Func_080c81c0:
	.incbin "baserom.gba", 0x000d11c0, 0x00000008
	.section .rom.000d11c8, "ax"
	.global Event_ShowCounterAtPositionFar
	.type Event_ShowCounterAtPositionFar, %function
	.thumb_func
Event_ShowCounterAtPositionFar:
	.incbin "baserom.gba", 0x000d11c8, 0x00000008
	.section .rom.000d11d0, "ax"
	.global ObjectMotion_ArmCallbackFar
	.type ObjectMotion_ArmCallbackFar, %function
	.thumb_func
ObjectMotion_ArmCallbackFar:
	.incbin "baserom.gba", 0x000d11d0, 0x00000008
	.section .rom.000d11d8, "ax"
	.global ObjectMotion_ArmCallbackAndRefreshFar
	.type ObjectMotion_ArmCallbackAndRefreshFar, %function
	.thumb_func
ObjectMotion_ArmCallbackAndRefreshFar:
	.incbin "baserom.gba", 0x000d11d8, 0x00000008
	.section .rom.000d11e0, "ax"
	.global Object_ResetTargetAndSetMode1Far
	.type Object_ResetTargetAndSetMode1Far, %function
	.thumb_func
Object_ResetTargetAndSetMode1Far:
	.incbin "baserom.gba", 0x000d11e0, 0x00000008
	.section .rom.000d11e8, "ax"
	.global ObjectTable_RunIfActiveFar
	.type ObjectTable_RunIfActiveFar, %function
	.thumb_func
ObjectTable_RunIfActiveFar:
	.incbin "baserom.gba", 0x000d11e8, 0x00000008
	.section .rom.000d11f0, "ax"
	.global Battle_ShowPairedUnitWorkAndWaitFar
	.type Battle_ShowPairedUnitWorkAndWaitFar, %function
	.thumb_func
Battle_ShowPairedUnitWorkAndWaitFar:
	.incbin "baserom.gba", 0x000d11f0, 0x00000008
	.section .rom.000d11f8, "ax"
	.global Ui_SetRenderResultFromObjectFar
	.type Ui_SetRenderResultFromObjectFar, %function
	.thumb_func
Ui_SetRenderResultFromObjectFar:
	.incbin "baserom.gba", 0x000d11f8, 0x00000008
	.section .rom.000d1200, "ax"
	.global ObjectMotion_SetActionVariantFar
	.type ObjectMotion_SetActionVariantFar, %function
	.thumb_func
ObjectMotion_SetActionVariantFar:
	.incbin "baserom.gba", 0x000d1200, 0x00000008
	.section .rom.000d1208, "ax"
	.global Func_080c8208
	.type Func_080c8208, %function
	.thumb_func
Func_080c8208:
	.incbin "baserom.gba", 0x000d1208, 0x00000008
	.section .rom.000d1210, "ax"
	.global Func_080c8210
	.type Func_080c8210, %function
	.thumb_func
Func_080c8210:
	.incbin "baserom.gba", 0x000d1210, 0x00000008
	.section .rom.000d1218, "ax"
	.global Func_080c8218
	.type Func_080c8218, %function
	.thumb_func
Func_080c8218:
	.incbin "baserom.gba", 0x000d1218, 0x00000008
	.section .rom.000d1220, "ax"
	.global ObjectVisual_CopyAttributesFar
	.type ObjectVisual_CopyAttributesFar, %function
	.thumb_func
ObjectVisual_CopyAttributesFar:
	.incbin "baserom.gba", 0x000d1220, 0x00000008
	.section .rom.000d1228, "ax"
	.global Object_AttachWorkTargetToObjectFar
	.type Object_AttachWorkTargetToObjectFar, %function
	.thumb_func
Object_AttachWorkTargetToObjectFar:
	.incbin "baserom.gba", 0x000d1228, 0x00000008
	.section .rom.000d1230, "ax"
	.global ObjectTable_AllocateAndSetObjectSpeedFar
	.type ObjectTable_AllocateAndSetObjectSpeedFar, %function
	.thumb_func
ObjectTable_AllocateAndSetObjectSpeedFar:
	.incbin "baserom.gba", 0x000d1230, 0x00000008
	.section .rom.000d1238, "ax"
	.global Motion_CamBoundsFar
	.type Motion_CamBoundsFar, %function
	.thumb_func
Motion_CamBoundsFar:
	.incbin "baserom.gba", 0x000d1238, 0x00000008
	.section .rom.000d1240, "ax"
	.global Func_080c8240
	.type Func_080c8240, %function
	.thumb_func
Func_080c8240:
	.incbin "baserom.gba", 0x000d1240, 0x00000008
	.section .rom.000d1248, "ax"
	.global Func_080c8248
	.type Func_080c8248, %function
	.thumb_func
Func_080c8248:
	.incbin "baserom.gba", 0x000d1248, 0x00000008
	.section .rom.000d1250, "ax"
	.global Func_080c8250
	.type Func_080c8250, %function
	.thumb_func
Func_080c8250:
	.incbin "baserom.gba", 0x000d1250, 0x00000008
	.section .rom.000d1258, "ax"
	.global Func_080c8258
	.type Func_080c8258, %function
	.thumb_func
Func_080c8258:
	.incbin "baserom.gba", 0x000d1258, 0x00000008
	.section .rom.000d1260, "ax"
	.global Func_080c8260
	.type Func_080c8260, %function
	.thumb_func
Func_080c8260:
	.incbin "baserom.gba", 0x000d1260, 0x00000008
	.section .rom.000d12a8, "ax"
	.incbin "baserom.gba", 0x000d12a8, 0x00000010
	.section .rom.000d13b8, "ax"
	.global Event_DelayEffectFramesFar
	.type Event_DelayEffectFramesFar, %function
	.thumb_func
Event_DelayEffectFramesFar:
	.incbin "baserom.gba", 0x000d13b8, 0x00000008
	.section .rom.000d13c0, "ax"
	.global Event_FinishScreenTransitionFar
	.type Event_FinishScreenTransitionFar, %function
	.thumb_func
Event_FinishScreenTransitionFar:
	.incbin "baserom.gba", 0x000d13c0, 0x00000008
	.section .rom.000d13c8, "ax"
	.global Event_WaitTransitionFramesFar
	.type Event_WaitTransitionFramesFar, %function
	.thumb_func
Event_WaitTransitionFramesFar:
	.incbin "baserom.gba", 0x000d13c8, 0x00000008
	.section .rom.000d13d0, "ax"
	.global DisplayScroll_InitializeHBlankDmaFar
	.type DisplayScroll_InitializeHBlankDmaFar, %function
	.thumb_func
DisplayScroll_InitializeHBlankDmaFar:
	.incbin "baserom.gba", 0x000d13d0, 0x00000008
	.section .rom.000d13d8, "ax"
	.global DisplayScroll_DisarmHBlankDmaFar
	.type DisplayScroll_DisarmHBlankDmaFar, %function
	.thumb_func
DisplayScroll_DisarmHBlankDmaFar:
	.incbin "baserom.gba", 0x000d13d8, 0x00000008
	.section .rom.000d13e0, "ax"
	.global BattleFx_PlayCueAndStartEmitterOnTargetFar
	.type BattleFx_PlayCueAndStartEmitterOnTargetFar, %function
	.thumb_func
BattleFx_PlayCueAndStartEmitterOnTargetFar:
	.incbin "baserom.gba", 0x000d13e0, 0x00000008
	.section .rom.000d13e8, "ax"
	.global Func_080c83d8
	.type Func_080c83d8, %function
	.thumb_func
Func_080c83d8:
	.incbin "baserom.gba", 0x000d13e8, 0x00000008
	.section .rom.000d13f0, "ax"
	.global Func_080c83e0
	.type Func_080c83e0, %function
	.thumb_func
Func_080c83e0:
	.incbin "baserom.gba", 0x000d13f0, 0x00000008
	.section .rom.000d13f8, "ax"
	.global ObjectEvent_GetObjectFar
	.type ObjectEvent_GetObjectFar, %function
	.thumb_func
ObjectEvent_GetObjectFar:
	.incbin "baserom.gba", 0x000d13f8, 0x00000008
	.section .rom.000d1400, "ax"
	.global ObjectEvent_SetMode4WithCueFar
	.type ObjectEvent_SetMode4WithCueFar, %function
	.thumb_func
ObjectEvent_SetMode4WithCueFar:
	.incbin "baserom.gba", 0x000d1400, 0x00000008
	.section .rom.000d1408, "ax"
	.global ObjectEvent_SetMode5WithCueFar
	.type ObjectEvent_SetMode5WithCueFar, %function
	.thumb_func
ObjectEvent_SetMode5WithCueFar:
	.incbin "baserom.gba", 0x000d1408, 0x00000008
	.section .rom.000d1410, "ax"
	.global ObjectEvent_SetMode2Far
	.type ObjectEvent_SetMode2Far, %function
	.thumb_func
ObjectEvent_SetMode2Far:
	.incbin "baserom.gba", 0x000d1410, 0x00000008
	.section .rom.000d1418, "ax"
	.global ObjectEvent_PlaceOnGroundFar
	.type ObjectEvent_PlaceOnGroundFar, %function
	.thumb_func
ObjectEvent_PlaceOnGroundFar:
	.incbin "baserom.gba", 0x000d1418, 0x00000008
	.section .rom.000d1420, "ax"
	.global ObjectEvent_RemoveFar
	.type ObjectEvent_RemoveFar, %function
	.thumb_func
ObjectEvent_RemoveFar:
	.incbin "baserom.gba", 0x000d1420, 0x00000008
	.section .rom.000d1428, "ax"
	.global Func_080c8418
	.type Func_080c8418, %function
	.thumb_func
Func_080c8418:
	.incbin "baserom.gba", 0x000d1428, 0x00000008
	.section .rom.000d1430, "ax"
	.global Object_SetWideSpriteFar
	.type Object_SetWideSpriteFar, %function
	.thumb_func
Object_SetWideSpriteFar:
	.incbin "baserom.gba", 0x000d1430, 0x00000008
	.section .rom.000d1438, "ax"
	.global Object_ReturnHomeFar
	.type Object_ReturnHomeFar, %function
	.thumb_func
Object_ReturnHomeFar:
	.incbin "baserom.gba", 0x000d1438, 0x00000008
	.section .rom.000d1440, "ax"
	.global Func_080c8430
	.type Func_080c8430, %function
	.thumb_func
Func_080c8430:
	.incbin "baserom.gba", 0x000d1440, 0x00000008
	.section .rom.000d1448, "ax"
	.global Func_080c8438
	.type Func_080c8438, %function
	.thumb_func
Func_080c8438:
	.incbin "baserom.gba", 0x000d1448, 0x00000008
	.section .rom.000d1450, "ax"
	.global Func_080c8440
	.type Func_080c8440, %function
	.thumb_func
Func_080c8440:
	.incbin "baserom.gba", 0x000d1450, 0x00000008
	.section .rom.000d1458, "ax"
	.global Event_SpawnObjectTableFar
	.type Event_SpawnObjectTableFar, %function
	.thumb_func
Event_SpawnObjectTableFar:
	.incbin "baserom.gba", 0x000d1458, 0x00000008
	.section .rom.000d1460, "ax"
	.global ObjectTable_DestroyAtIndexFar
	.type ObjectTable_DestroyAtIndexFar, %function
	.thumb_func
ObjectTable_DestroyAtIndexFar:
	.incbin "baserom.gba", 0x000d1460, 0x00000008
	.section .rom.000d1468, "ax"
	.global ObjectTable_GetFar
	.type ObjectTable_GetFar, %function
	.thumb_func
ObjectTable_GetFar:
	.incbin "baserom.gba", 0x000d1468, 0x00000008
	.section .rom.000d1470, "ax"
	.global Func_080c8460
	.type Func_080c8460, %function
	.thumb_func
Func_080c8460:
	.incbin "baserom.gba", 0x000d1470, 0x00000008
	.section .rom.000d1478, "ax"
	.global Func_080c8468
	.type Func_080c8468, %function
	.thumb_func
Func_080c8468:
	.incbin "baserom.gba", 0x000d1478, 0x00000008
	.section .rom.000d1480, "ax"
	.global Func_080c8470
	.type Func_080c8470, %function
	.thumb_func
Func_080c8470:
	.incbin "baserom.gba", 0x000d1480, 0x00000008
	.section .rom.000d1488, "ax"
	.global Func_080c8478
	.type Func_080c8478, %function
	.thumb_func
Func_080c8478:
	.incbin "baserom.gba", 0x000d1488, 0x00000008
	.section .rom.000d1490, "ax"
	.global Func_080c8480
	.type Func_080c8480, %function
	.thumb_func
Func_080c8480:
	.incbin "baserom.gba", 0x000d1490, 0x00000008
	.section .rom.000d1498, "ax"
	.global Func_080c8488
	.type Func_080c8488, %function
	.thumb_func
Func_080c8488:
	.incbin "baserom.gba", 0x000d1498, 0x00000008
	.section .rom.000d14a0, "ax"
	.global Func_080c8490
	.type Func_080c8490, %function
	.thumb_func
Func_080c8490:
	.incbin "baserom.gba", 0x000d14a0, 0x00000008
	.section .rom.000d14a8, "ax"
	.global Field_DispatchTypeHandlerFar
	.type Field_DispatchTypeHandlerFar, %function
	.thumb_func
Field_DispatchTypeHandlerFar:
	.incbin "baserom.gba", 0x000d14a8, 0x00000008
	.section .rom.000d14b0, "ax"
	.global BattleFx_LoadActionEffectResourcesFar
	.type BattleFx_LoadActionEffectResourcesFar, %function
	.thumb_func
BattleFx_LoadActionEffectResourcesFar:
	.incbin "baserom.gba", 0x000d14b0, 0x00000008
	.section .rom.000d14b8, "ax"
	.global BattleFx_SetupObjectPairFar
	.type BattleFx_SetupObjectPairFar, %function
	.thumb_func
BattleFx_SetupObjectPairFar:
	.incbin "baserom.gba", 0x000d14b8, 0x00000008
	.section .rom.000d14c0, "ax"
	.global EffectRuntime_StopCurrentObjectFar
	.type EffectRuntime_StopCurrentObjectFar, %function
	.thumb_func
EffectRuntime_StopCurrentObjectFar:
	.incbin "baserom.gba", 0x000d14c0, 0x00000008
	.section .rom.000d14c8, "ax"
	.global BattleEffect_CleanupSceneObjectsFar
	.type BattleEffect_CleanupSceneObjectsFar, %function
	.thumb_func
BattleEffect_CleanupSceneObjectsFar:
	.incbin "baserom.gba", 0x000d14c8, 0x00000008
	.section .rom.000d14d0, "ax"
	.global EventObject_InitializeFar
	.type EventObject_InitializeFar, %function
	.thumb_func
EventObject_InitializeFar:
	.incbin "baserom.gba", 0x000d14d0, 0x00000008
	.section .rom.000d14d8, "ax"
	.global BattleEffect_InitializeSharedSceneFar
	.type BattleEffect_InitializeSharedSceneFar, %function
	.thumb_func
BattleEffect_InitializeSharedSceneFar:
	.incbin "baserom.gba", 0x000d14d8, 0x00000008
	.section .rom.000d14e0, "ax"
	.global BattleEffect_ResolvePendingActionsFar
	.type BattleEffect_ResolvePendingActionsFar, %function
	.thumb_func
BattleEffect_ResolvePendingActionsFar:
	.incbin "baserom.gba", 0x000d14e0, 0x00000008
	.section .rom.000d1518, "ax"
	.global BattleFx_HasMatchingEvent5Far
	.type BattleFx_HasMatchingEvent5Far, %function
	.thumb_func
BattleFx_HasMatchingEvent5Far:
	.incbin "baserom.gba", 0x000d1518, 0x00000008
	.section .rom.000d1520, "ax"
	.global BattleFx_HasTriggerFar
	.type BattleFx_HasTriggerFar, %function
	.thumb_func
BattleFx_HasTriggerFar:
	.incbin "baserom.gba", 0x000d1520, 0x00000008
	.section .rom.000d1528, "ax"
	.global Func_080c8518
	.type Func_080c8518, %function
	.thumb_func
Func_080c8518:
	.incbin "baserom.gba", 0x000d1528, 0x00000008
	.section .rom.000d1598, "ax"
	.global EffectSlot_UpdateFar
	.type EffectSlot_UpdateFar, %function
	.thumb_func
EffectSlot_UpdateFar:
	.incbin "baserom.gba", 0x000d1598, 0x00000008
	.section .rom.000d15a0, "ax"
	.global BattleFx_HasReachedTargetFar
	.type BattleFx_HasReachedTargetFar, %function
	.thumb_func
BattleFx_HasReachedTargetFar:
	.incbin "baserom.gba", 0x000d15a0, 0x00000008
	.section .rom.000d15a8, "ax"
	.global EffectSlot_SetPositionFar
	.type EffectSlot_SetPositionFar, %function
	.thumb_func
EffectSlot_SetPositionFar:
	.incbin "baserom.gba", 0x000d15a8, 0x00000008
	.section .rom.000d15b0, "ax"
	.global EffectSlot_SetObjectModeFar
	.type EffectSlot_SetObjectModeFar, %function
	.thumb_func
EffectSlot_SetObjectModeFar:
	.incbin "baserom.gba", 0x000d15b0, 0x00000008
	.section .rom.000d15b8, "ax"
	.global EffectSlot_SetCallbackFar
	.type EffectSlot_SetCallbackFar, %function
	.thumb_func
EffectSlot_SetCallbackFar:
	.incbin "baserom.gba", 0x000d15b8, 0x00000008
	.section .rom.000d15c0, "ax"
	.global Camera_WorldToScreenFar
	.type Camera_WorldToScreenFar, %function
	.thumb_func
Camera_WorldToScreenFar:
	.incbin "baserom.gba", 0x000d15c0, 0x00000008
	.section .rom.000d1628, "ax"
	.global Object_BuildRecordSnapshotFar
	.type Object_BuildRecordSnapshotFar, %function
	.thumb_func
Object_BuildRecordSnapshotFar:
	.incbin "baserom.gba", 0x000d1628, 0x00000010
	.section .rom.000d1658, "ax"
	.global Scene_GetAreaNameIndexFar
	.type Scene_GetAreaNameIndexFar, %function
	.thumb_func
Scene_GetAreaNameIndexFar:
	.incbin "baserom.gba", 0x000d1658, 0x00000008
	.section .rom.000d1660, "ax"
	.global Func_080c8650
	.type Func_080c8650, %function
	.thumb_func
Func_080c8650:
	.incbin "baserom.gba", 0x000d1660, 0x00000008
	.section .rom.000d1668, "ax"
	.global Func_080c8658
	.type Func_080c8658, %function
	.thumb_func
Func_080c8658:
	.incbin "baserom.gba", 0x000d1668, 0x00000008
	.section .rom.000d1670, "ax"
	.global Func_080c8660
	.type Func_080c8660, %function
	.thumb_func
Func_080c8660:
	.incbin "baserom.gba", 0x000d1670, 0x00000008
	.section .rom.000d1678, "ax"
	.global Func_080c8668
	.type Func_080c8668, %function
	.thumb_func
Func_080c8668:
	.incbin "baserom.gba", 0x000d1678, 0x00000008
	.section .rom.000d1680, "ax"
	.global Func_080c8670
	.type Func_080c8670, %function
	.thumb_func
Func_080c8670:
	.incbin "baserom.gba", 0x000d1680, 0x00000008
	.section .rom.000d1688, "ax"
	.global Func_080c8678
	.type Func_080c8678, %function
	.thumb_func
Func_080c8678:
	.incbin "baserom.gba", 0x000d1688, 0x00000008
	.section .rom.000d1690, "ax"
	.global Object_SetActionCallbackFar
	.type Object_SetActionCallbackFar, %function
	.thumb_func
Object_SetActionCallbackFar:
	.incbin "baserom.gba", 0x000d1690, 0x00000008
	.section .rom.000d1698, "ax"
	.global Func_080c8688
	.type Func_080c8688, %function
	.thumb_func
Func_080c8688:
	.incbin "baserom.gba", 0x000d1698, 0x00000008
	.section .rom.000d16a0, "ax"
	.global Event_NoOpFar
	.type Event_NoOpFar, %function
	.thumb_func
Event_NoOpFar:
	.incbin "baserom.gba", 0x000d16a0, 0x00000008
	.section .rom.000d18d8, "ax"
	.global Func_080c88c8
	.type Func_080c88c8, %function
	.thumb_func
Func_080c88c8:
	.incbin "baserom.gba", 0x000d18d8, 0x00000008
	.section .rom.000d18e0, "ax"
	.global Func_080c88d0
	.type Func_080c88d0, %function
	.thumb_func
Func_080c88d0:
	.incbin "baserom.gba", 0x000d18e0, 0x00000008
	.section .rom.000d18e8, "ax"
	.global Func_080c88d8
	.type Func_080c88d8, %function
	.thumb_func
Func_080c88d8:
	.incbin "baserom.gba", 0x000d18e8, 0x00000004
	.section .rom.000d18ec, "ax"
	.global BattleFx_DecodeFrameUnusedBase
BattleFx_DecodeFrameUnusedBase:
	.incbin "baserom.gba", 0x000d18ec, 0x00000004
	.section .rom.000d18f0, "ax"
	.global Func_080c88e0
	.type Func_080c88e0, %function
	.thumb_func
Func_080c88e0:
	.incbin "baserom.gba", 0x000d18f0, 0x00000008
	.section .rom.000d18f8, "ax"
	.global Func_080c88e8
	.type Func_080c88e8, %function
	.thumb_func
Func_080c88e8:
	.incbin "baserom.gba", 0x000d18f8, 0x00000008
	.section .rom.000d1900, "ax"
	.global Func_080c88f0
	.type Func_080c88f0, %function
	.thumb_func
Func_080c88f0:
	.incbin "baserom.gba", 0x000d1900, 0x00000008
	.section .rom.000d1908, "ax"
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.type ObjectMotion_ResetTargetsAndVelocityFar, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocityFar:
	.incbin "baserom.gba", 0x000d1908, 0x00000008
	.section .rom.000d1910, "ax"
	.global ObjectEffect_BeginContextEffect26Far
	.type ObjectEffect_BeginContextEffect26Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect26Far:
	.incbin "baserom.gba", 0x000d1910, 0x00000008
	.section .rom.000d1918, "ax"
	.global ObjectEffect_BeginContextEffect25Far
	.type ObjectEffect_BeginContextEffect25Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect25Far:
	.incbin "baserom.gba", 0x000d1918, 0x00000008
	.section .rom.000d1920, "ax"
	.global Func_080c8910
	.type Func_080c8910, %function
	.thumb_func
Func_080c8910:
	.incbin "baserom.gba", 0x000d1920, 0x00000008
	.section .rom.000d1928, "ax"
	.global Func_080c8918
	.type Func_080c8918, %function
	.thumb_func
Func_080c8918:
	.incbin "baserom.gba", 0x000d1928, 0x00000008
	.section .rom.000d1930, "ax"
	.global Func_080c8920
	.type Func_080c8920, %function
	.thumb_func
Func_080c8920:
	.incbin "baserom.gba", 0x000d1930, 0x00000008
	.section .rom.000d1938, "ax"
	.global Func_080c8928
	.type Func_080c8928, %function
	.thumb_func
Func_080c8928:
	.incbin "baserom.gba", 0x000d1938, 0x00000008
	.section .rom.000d1940, "ax"
	.global Func_080c8930
	.type Func_080c8930, %function
	.thumb_func
Func_080c8930:
	.incbin "baserom.gba", 0x000d1940, 0x00000008
	.section .rom.000d1948, "ax"
	.global Func_080c8938
	.type Func_080c8938, %function
	.thumb_func
Func_080c8938:
	.incbin "baserom.gba", 0x000d1948, 0x00000008
	.section .rom.000d1950, "ax"
	.global Func_080c8940
	.type Func_080c8940, %function
	.thumb_func
Func_080c8940:
	.incbin "baserom.gba", 0x000d1950, 0x00000008
	.section .rom.000d2694, "ax"
	.incbin "baserom.gba", 0x000d2694, 0x0000028c
	.section .rom.000d2944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000d2944, 0x000002cc
	.section .rom.000d2c38, "ax"
	.incbin "baserom.gba", 0x000d2c38, 0x00000190
	.section .rom.000d2e48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d2e48, 0x000000e4
	.section .rom.000d2f2c, "ax"
	.global BattleFx_LookupResult
	.type BattleFx_LookupResult, %function
	.thumb_func
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000d2f2c, 0x000000ac
	.section .rom.000d2fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000d2fd8, 0x0000018c
	.section .rom.000d32fc, "ax"
	.incbin "baserom.gba", 0x000d32fc, 0x00000198
	.section .rom.000d34a8, "ax"
	.incbin "baserom.gba", 0x000d34a8, 0x000000f8
	.section .rom.000d35d8, "ax"
	.global BattleFx_SelectBattleCue
	.type BattleFx_SelectBattleCue, %function
	.thumb_func
BattleFx_SelectBattleCue:
	.incbin "baserom.gba", 0x000d35d8, 0x000000cc
	.section .rom.000d36a4, "ax"
	.global Event_SelectSpawnObjectMode
	.type Event_SelectSpawnObjectMode, %function
	.thumb_func
Event_SelectSpawnObjectMode:
	.incbin "baserom.gba", 0x000d36a4, 0x00000044
	.section .rom.000d36e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000d36e8, 0x000002e4
	.section .rom.000d39cc, "ax"
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000d39cc, 0x00000060
	.section .rom.000d3a2c, "ax"
	.global Func_080caa2c
Func_080caa2c:
	.incbin "baserom.gba", 0x000d3a2c, 0x00000020
	.section .rom.000d3a4c, "ax"
	.global Func_080caa4c
Func_080caa4c:
	.incbin "baserom.gba", 0x000d3a4c, 0x00000274
	.section .rom.000d3e5c, "ax"
	.incbin "baserom.gba", 0x000d3e5c, 0x00000110
	.section .rom.000d409c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d409c, 0x0000062c
	.section .rom.000d491c, "ax"
	.incbin "baserom.gba", 0x000d491c, 0x00000d60
	.section .rom.000d567c, "ax"
	.global Func_080cc67c
Func_080cc67c:
	.incbin "baserom.gba", 0x000d567c, 0x00000148
	.section .rom.000d57c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d57c4, 0x000001d0
	.section .rom.000d59fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d59fc, 0x000002bc
	.section .rom.000d5cb8, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000d5cb8, 0x00000090
	.section .rom.000d5d76, "ax"
	.incbin "baserom.gba", 0x000d5d76, 0x00000002
	.section .rom.000d5d78, "ax"
	.global Func_080c8958_Target
	.type Func_080c8958_Target, %function
	.thumb_func
Func_080c8958_Target:
	.incbin "baserom.gba", 0x000d5d78, 0x0000010c
	.section .rom.000d5e94, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d5e94, 0x00000034
	.section .rom.000d5ec8, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000d5ec8, 0x000006bc
	.section .rom.000d6584, "ax"
	.global Func_080cd584
Func_080cd584:
	.incbin "baserom.gba", 0x000d6584, 0x00000398
	.section .rom.000d691c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d691c, 0x0000013c
	.section .rom.000d6a84, "ax"
	.balign 4
	.global Func_080cda84
Func_080cda84:
	.incbin "baserom.gba", 0x000d6a84, 0x0000003c
	.section .rom.000d6ac0, "ax"
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000d6ac0, 0x00000138
	.section .rom.000d6c72, "ax"
	.incbin "baserom.gba", 0x000d6c72, 0x00000002
	.section .rom.000d6c74, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000d6c74, 0x0000010c
	.section .rom.000d6d80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000d6d80, 0x00000128
	.section .rom.000d6fb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000d6fb0, 0x0000036c
	.section .rom.000d731c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000d731c, 0x0000013c
	.section .rom.000d7458, "ax"
	.global Func_080ce458
	.type Func_080ce458, %function
	.thumb_func
Func_080ce458:
	.incbin "baserom.gba", 0x000d7458, 0x0000011c
	.section .rom.000d761c, "ax"
	.incbin "baserom.gba", 0x000d761c, 0x000004e0
	.section .rom.000d7ba8, "ax"
	.incbin "baserom.gba", 0x000d7ba8, 0x0000007c
	.section .rom.000d7c34, "ax"
	.incbin "baserom.gba", 0x000d7c34, 0x0000020c
	.section .rom.000d7eac, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d7eac, 0x00000078
	.section .rom.000d8050, "ax"
	.incbin "baserom.gba", 0x000d8050, 0x00000080
	.section .rom.000d8158, "ax"
	.incbin "baserom.gba", 0x000d8158, 0x00000024
	.section .rom.000d817c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000d817c, 0x000001c4
	.section .rom.000d8350, "ax"
	.balign 4
	.global Func_080cf350
	.type Func_080cf350, %function
	.thumb_func
Func_080cf350:
	.incbin "baserom.gba", 0x000d8350, 0x00000064
	.section .rom.000d8554, "ax"
	.incbin "baserom.gba", 0x000d8554, 0x000001a8
	.section .rom.000d86fc, "ax"
	.global Func_080cf6fc
	.type Func_080cf6fc, %function
	.thumb_func
Func_080cf6fc:
	.incbin "baserom.gba", 0x000d86fc, 0x00000090
	.section .rom.000d878c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000d878c, 0x0000096c
	.section .rom.000d90f8, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d90f8, 0x0000008c
	.section .rom.000d91cc, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d91cc, 0x00000354
	.section .rom.000d9520, "ax"
	.global DisplayTransition_Finish
	.type DisplayTransition_Finish, %function
	.thumb_func
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d9520, 0x0000020c
	.section .rom.000d9788, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d9788, 0x000001cc
	.section .rom.000d9954, "ax"
	.global Func_080d0954
	.type Func_080d0954, %function
	.thumb_func
Func_080d0954:
	.incbin "baserom.gba", 0x000d9954, 0x000000d4
	.section .rom.000d9a28, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d9a28, 0x00000154
	.section .rom.000d9b7c, "ax"
	.global DisplayTransition_FillTilemapAndSolidTile
	.type DisplayTransition_FillTilemapAndSolidTile, %function
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d9b7c, 0x00000070
	.section .rom.000d9ca0, "ax"
	.incbin "baserom.gba", 0x000d9ca0, 0x0000017c
	.section .rom.000d9e1c, "ax"
	.global BattleFx_BuildColorBuffers
	.type BattleFx_BuildColorBuffers, %function
	.thumb_func
BattleFx_BuildColorBuffers:
	.incbin "baserom.gba", 0x000d9e1c, 0x00000868
	.section .rom.000da684, "ax"
	.global BattleEffect_InitializeBuffers
	.type BattleEffect_InitializeBuffers, %function
	.thumb_func
BattleEffect_InitializeBuffers:
	.incbin "baserom.gba", 0x000da684, 0x00000074
	.section .rom.000da6f8, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000da6f8, 0x00000014
	.section .rom.000da70c, "ax"
	.global BattleFx_ApplyColorToTargetBuffer
	.type BattleFx_ApplyColorToTargetBuffer, %function
	.thumb_func
BattleFx_ApplyColorToTargetBuffer:
	.incbin "baserom.gba", 0x000da70c, 0x00000020
	.section .rom.000da72c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000da72c, 0x00000020
	.section .rom.000da74c, "ax"
	.global BattleFx_SetBufferWord
	.type BattleFx_SetBufferWord, %function
	.thumb_func
BattleFx_SetBufferWord:
	.incbin "baserom.gba", 0x000da74c, 0x00000014
	.section .rom.000da760, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000da760, 0x0000004c
	.section .rom.000da7e8, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000da7e8, 0x00000034
	.section .rom.000da83e, "ax"
	.incbin "baserom.gba", 0x000da83e, 0x00000002
	.section .rom.000da840, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000da840, 0x00000284
	.section .rom.000daae4, "ax"
	.global Func_080d1ae4
	.type Func_080d1ae4, %function
	.thumb_func
Func_080d1ae4:
	.incbin "baserom.gba", 0x000daae4, 0x000001dc
	.section .rom.000dadac, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000dadac, 0x00000044
	.section .rom.000dae18, "ax"
	.balign 4
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000dae18, 0x00000048
	.section .rom.000dae60, "ax"
	.global Func_080d1e60
	.type Func_080d1e60, %function
	.thumb_func
Func_080d1e60:
	.incbin "baserom.gba", 0x000dae60, 0x00000024
	.section .rom.000daee8, "ax"
	.balign 4
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000daee8, 0x00000024
	.section .rom.000daf20, "ax"
	.incbin "baserom.gba", 0x000daf20, 0x00000164
	.section .rom.000db0fc, "ax"
	.incbin "baserom.gba", 0x000db0fc, 0x000000f8
	.section .rom.000db60c, "ax"
	.incbin "baserom.gba", 0x000db60c, 0x000001d4
	.section .rom.000db834, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000db834, 0x0000011c
	.section .rom.000db960, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000db960, 0x000000a0
	.section .rom.000dbb40, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
	.align 2, 0
Func_080d2b4c:
	.incbin "baserom.gba", 0x000dbb40, 0x000000b8
	.section .rom.000dbc90, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dbc90, 0x00000028
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
	.incbin "baserom.gba", 0x000dbcfc, 0x00000068
	.section .rom.000dbd64, "ax"
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000dbd64, 0x00000014
	.section .rom.000dc10a, "ax"
	.incbin "baserom.gba", 0x000dc10a, 0x00000002
	.section .rom.000dc10c, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.type ObjectMotion_SetHorizontalPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
	.incbin "baserom.gba", 0x000dc10c, 0x0000004c
	.section .rom.000dc158, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrain
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrain:
	.incbin "baserom.gba", 0x000dc158, 0x0000005c
	.section .rom.000dc1b4, "ax"
	.global ObjectMotion_SetPositionWithTerrain
	.type ObjectMotion_SetPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrain:
	.incbin "baserom.gba", 0x000dc1b4, 0x00000054
	.section .rom.000dc208, "ax"
	.global ObjectMotion_MoveToObject
	.type ObjectMotion_MoveToObject, %function
	.thumb_func
ObjectMotion_MoveToObject:
	.incbin "baserom.gba", 0x000dc208, 0x0000002c
	.section .rom.000dc25c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000dc25c, 0x00000050
	.section .rom.000dc36a, "ax"
	.incbin "baserom.gba", 0x000dc36a, 0x00000002
	.section .rom.000dc36c, "ax"
	.incbin "baserom.gba", 0x000dc36c, 0x000000e8
	.section .rom.000dc454, "ax"
	.global BattleFx_RunRisingObjectSequence
	.type BattleFx_RunRisingObjectSequence, %function
	.thumb_func
BattleFx_RunRisingObjectSequence:
	.incbin "baserom.gba", 0x000dc454, 0x0000013c
	.section .rom.000dc774, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000dc774, 0x00000054
	.section .rom.000dc91c, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000dcb1c, 0x000000c0
	.section .rom.000dcc7c, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000dcc7c, 0x00000328
	.section .rom.000dcfa4, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000dcfa4, 0x000000bc
	.section .rom.000dd0d0, "ax"
	.balign 4
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000dd0d0, 0x0000009c
	.section .rom.000dd1da, "ax"
	.incbin "baserom.gba", 0x000dd1da, 0x00000002
	.section .rom.000dd1f0, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000dd1f0, 0x00000134
	.section .rom.000dd324, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000dd324, 0x00000054
	.section .rom.000dd400, "ax"
	.balign 4
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000dd400, 0x00000100
	.section .rom.000dd7a8, "ax"
	.incbin "baserom.gba", 0x000dd7a8, 0x000000b4
	.section .rom.000dd890, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dd890, 0x000000f0
	.section .rom.000ddaa8, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000ddaa8, 0x00000058
	.section .rom.000ddbb8, "ax"
	.incbin "baserom.gba", 0x000ddbb8, 0x00000108
	.section .rom.000ddcfc, "ax"
	.balign 4
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000ddcfc, 0x000003f0
	.section .rom.000de0ec, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
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
	.section .rom.000dee6c, "ax"
	.balign 4
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000dee6c, 0x000000a0
	.section .rom.000df070, "ax"
	.global Func_080d607c
	.type Func_080d607c, %function
	.thumb_func
Func_080d607c:
	.incbin "baserom.gba", 0x000df070, 0x0000038c
	.section .rom.000df4e4, "ax"
	.incbin "baserom.gba", 0x000df4e4, 0x00000184
	.section .rom.000df668, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000df668, 0x00000174
	.section .rom.000df7dc, "ax"
	.global Func_080d67e8
	.type Func_080d67e8, %function
	.thumb_func
Func_080d67e8:
	.incbin "baserom.gba", 0x000df7dc, 0x00000124
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
	.incbin "baserom.gba", 0x000dfb84, 0x00000494
	.section .rom.000e0028, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000e0028, 0x0000020c
	.section .rom.000e0518, "ax"
	.incbin "baserom.gba", 0x000e0518, 0x00000264
	.section .rom.000e077c, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000e077c, 0x000002d0
	.section .rom.000e0af8, "ax"
	.incbin "baserom.gba", 0x000e0af8, 0x00000244
	.section .rom.000e0d84, "ax"
	.incbin "baserom.gba", 0x000e0d84, 0x0000097c
	.section .rom.000e1734, "ax"
	.incbin "baserom.gba", 0x000e1734, 0x0000023c
	.section .rom.000e1970, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000e1970, 0x00000084
	.section .rom.000e19f4, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000e19f4, 0x00000210
	.section .rom.000e1c04, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000e1c04, 0x00000130
	.section .rom.000e1d5c, "ax"
	.balign 4
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000e1d5c, 0x00000240
	.section .rom.000e1f9c, "ax"
	.global Func_080d8fa8
	.type Func_080d8fa8, %function
	.thumb_func
Func_080d8fa8:
	.incbin "baserom.gba", 0x000e1f9c, 0x0000015c
	.section .rom.000e20f8, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000e20f8, 0x000001a0
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
	.incbin "baserom.gba", 0x000e2704, 0x00000268
	.section .rom.000e2aa4, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000e2afc, 0x00000078
	.section .rom.000e2b74, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000e2b74, 0x000001c0
	.section .rom.000e2d34, "ax"
	.global Func_080d9d40
	.type Func_080d9d40, %function
	.thumb_func
Func_080d9d40:
	.incbin "baserom.gba", 0x000e2d34, 0x00000134
	.section .rom.000e2e8c, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000e2e8c, 0x000001c8
	.section .rom.000e3054, "ax"
	.global Func_080da060
	.type Func_080da060, %function
	.thumb_func
Func_080da060:
	.incbin "baserom.gba", 0x000e3054, 0x000008a8
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
	.incbin "baserom.gba", 0x000e392c, 0x00000030
	.section .rom.000e399c, "ax"
	.incbin "baserom.gba", 0x000e399c, 0x00000500
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
	.section .rom.000e499a, "ax"
	.incbin "baserom.gba", 0x000e499a, 0x00000002
	.section .rom.000e499c, "ax"
	.global Func_080db9a8
	.type Func_080db9a8, %function
	.thumb_func
Func_080db9a8:
	.incbin "baserom.gba", 0x000e499c, 0x00000018
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
	.incbin "baserom.gba", 0x000e49c0, 0x00000078
	.section .rom.000e4a50, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e4a50, 0x000000e4
	.section .rom.000e4b6c, "ax"
	.global Func_080dbb78
Func_080dbb78:
	.balign 4
	.global Map_GetCellAtPosition
	.type Map_GetCellAtPosition, %function
	.thumb_func
Map_GetCellAtPosition:
	.incbin "baserom.gba", 0x000e4b6c, 0x0000008c
	.section .rom.000e4bf8, "ax"
	.global Func_080dbc04
Func_080dbc04:
	.incbin "baserom.gba", 0x000e4bf8, 0x000000a4
	.section .rom.000e4c9c, "ax"
	.global Func_080dbca8
	.type Func_080dbca8, %function
	.thumb_func
Func_080dbca8:
	.incbin "baserom.gba", 0x000e4c9c, 0x00000018
	.section .rom.000e4cb4, "ax"
	.global Func_080dbcc0
Func_080dbcc0:
	.incbin "baserom.gba", 0x000e4cb4, 0x00000018
	.section .rom.000e4ccc, "ax"
	.global Func_080dbcd8
Func_080dbcd8:
	.global Map_TestSpecialTerrainPair
	.type Map_TestSpecialTerrainPair, %function
	.thumb_func
Map_TestSpecialTerrainPair:
	.incbin "baserom.gba", 0x000e4ccc, 0x00000070
	.section .rom.000e4d3c, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000e4d3c, 0x00000014
	.section .rom.000e4d50, "ax"
	.global Func_080dbd5c
Func_080dbd5c:
	.incbin "baserom.gba", 0x000e4d50, 0x0000004c
	.section .rom.000e4d9c, "ax"
	.global Func_080dbda8
	.type Func_080dbda8, %function
	.thumb_func
Func_080dbda8:
	.incbin "baserom.gba", 0x000e4d9c, 0x00000020
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
	.incbin "baserom.gba", 0x000e4e38, 0x0000003c
	.section .rom.000e4e74, "ax"
	.global Func_080dbe80
Func_080dbe80:
	.incbin "baserom.gba", 0x000e4e74, 0x000001c4
	.section .rom.000e51a4, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000e51a4, 0x00000094
	.section .rom.000e5288, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e5288, 0x000000e8
	.section .rom.000e5404, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x000e5404, 0x0000021c
	.section .rom.000e56cc, "ax"
	.incbin "baserom.gba", 0x000e56cc, 0x000000f4
	.section .rom.000e57dc, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000e57dc, 0x0000016c
	.section .rom.000e596c, "ax"
	.balign 4
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000e596c, 0x000000d8
	.section .rom.000e5ad0, "ax"
	.balign 4
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000e5ad0, 0x00000068
	.section .rom.000e5b38, "ax"
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000e5b38, 0x00000284
	.section .rom.000e6048, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000e6048, 0x000003e8
	.section .rom.000e651a, "ax"
	.incbin "baserom.gba", 0x000e651a, 0x00000002
	.section .rom.000e651c, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000e651c, 0x00000114
	.section .rom.000e6630, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000e6630, 0x0000002c
	.section .rom.000e665c, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000e665c, 0x000000e0
	.section .rom.000e6748, "ax"
	.incbin "baserom.gba", 0x000e6748, 0x000001fc
	.section .rom.000e6944, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000e6944, 0x000001ec
	.section .rom.000e6bcc, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000e6bcc, 0x0000063c
	.section .rom.000e7210, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000e7210, 0x00000334
	.section .rom.000e767c, "ax"
	.global BattleFx_RunOrbitingParticles
	.type BattleFx_RunOrbitingParticles, %function
	.thumb_func
BattleFx_RunOrbitingParticles:
	.incbin "baserom.gba", 0x000e767c, 0x00000190
	.section .rom.000e7858, "ax"
	.incbin "baserom.gba", 0x000e7858, 0x00000188
	.section .rom.000e79ec, "ax"
	.global Func_080de9f8
	.type Func_080de9f8, %function
	.thumb_func
Func_080de9f8:
	.incbin "baserom.gba", 0x000e79ec, 0x000001ac
	.section .rom.000e7bcc, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000e7bcc, 0x000000b4
	.section .rom.000e7cac, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000e7cac, 0x000000cc
	.section .rom.000e7ddc, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000e7ddc, 0x00000ac0
	.section .rom.000e88d2, "ax"
	.incbin "baserom.gba", 0x000e88d2, 0x00000002
	.section .rom.000e88d4, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000e88d4, 0x000001d4
	.section .rom.000e8c30, "ax"
	.incbin "baserom.gba", 0x000e8c30, 0x00000098
	.section .rom.000e8cf4, "ax"
	.global RunBattleEffect14
RunBattleEffect14:
	.incbin "baserom.gba", 0x000e8cf4, 0x000002fc
	.section .rom.000e9014, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e9014, 0x000000fc
	.section .rom.000e9128, "ax"
	.global Func_080e0134
	.type Func_080e0134, %function
	.thumb_func
Func_080e0134:
	.incbin "baserom.gba", 0x000e9128, 0x000001b8
	.section .rom.000e93b6, "ax"
	.incbin "baserom.gba", 0x000e93b6, 0x00000002
	.section .rom.000e93c0, "ax"
	.balign 4
	.global Func_080e03cc
	.type Func_080e03cc, %function
	.thumb_func
Func_080e03cc:
	.incbin "baserom.gba", 0x000e93c0, 0x0000024c
	.section .rom.000e960c, "ax"
	.incbin "baserom.gba", 0x000e960c, 0x00000360
	.section .rom.000e99b4, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000e99b4, 0x000002c4
	.section .rom.000e9ca0, "ax"
	.global Func_080e0cac
	.type Func_080e0cac, %function
	.thumb_func
Func_080e0cac:
	.incbin "baserom.gba", 0x000e9ca0, 0x00000040
	.section .rom.000e9ce0, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000e9ce0, 0x000000e8
	.section .rom.000e9f34, "ax"
	.incbin "baserom.gba", 0x000e9f34, 0x00000214
	.section .rom.000ea148, "ax"
	.global Func_080e1154
	.type Func_080e1154, %function
	.thumb_func
Func_080e1154:
	.incbin "baserom.gba", 0x000ea148, 0x00000068
	.section .rom.000ea43c, "ax"
	.global BattleFx_SpawnScaledArcObjects
	.type BattleFx_SpawnScaledArcObjects, %function
	.thumb_func
BattleFx_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000ea43c, 0x000000c4
	.section .rom.000ea500, "ax"
	.global BattleFx_UpdateArcEffect
	.type BattleFx_UpdateArcEffect, %function
	.thumb_func
BattleFx_UpdateArcEffect:
	.incbin "baserom.gba", 0x000ea500, 0x000000f0
	.section .rom.000ea66c, "ax"
	.incbin "baserom.gba", 0x000ea66c, 0x000008ac
	.section .rom.000eaf20, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000eaf20, 0x000006bc
	.section .rom.000eb5dc, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000eb5dc, 0x0000018c
	.section .rom.000eb768, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000eb768, 0x000004c4
	.section .rom.000ebc2c, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000ebc2c, 0x00000108
	.section .rom.000ebd58, "ax"
	.incbin "baserom.gba", 0x000ebd58, 0x000002fc
	.section .rom.000ec068, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000ec068, 0x00000624
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
	.incbin "baserom.gba", 0x000ed238, 0x00000090
	.section .rom.000ed2c8, "ax"
	.global Func_080e42d4
Func_080e42d4:
	.incbin "baserom.gba", 0x000ed2c8, 0x000002cc
	.section .rom.000ed59c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ed59c, 0x00000188
	.section .rom.000ed724, "ax"
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000ed724, 0x000015c0
	.section .rom.000eed48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000eed48, 0x00000634
	.section .rom.000ef3f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ef3f4, 0x00000cf8
	.section .rom.000f00ec, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000f00ec, 0x000016cc
	.section .rom.000f17b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000f17b8, 0x000005bc
	.section .rom.000f1da4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f1da4, 0x000004a0
	.section .rom.000f22a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f22a8, 0x00000b00
	.section .rom.000f2da8, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000f2da8, 0x00000130
	.section .rom.000f2f5c, "ax"
	.incbin "baserom.gba", 0x000f2f5c, 0x00000858
	.section .rom.000f37b4, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000f37b4, 0x000000e8
	.section .rom.000f389c, "ax"
	.global Func_080ea8a8
Func_080ea8a8:
	.incbin "baserom.gba", 0x000f389c, 0x0000002c
	.section .rom.000f38c8, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
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
	.global Func_080eb01c
Func_080eb01c:
	.incbin "baserom.gba", 0x000f4010, 0x0000027c
	.section .rom.000f430c, "ax"
	.incbin "baserom.gba", 0x000f430c, 0x00000204
	.section .rom.000f4588, "ax"
	.global Func_080eb594
	.type Func_080eb594, %function
	.thumb_func
Func_080eb594:
	.incbin "baserom.gba", 0x000f4588, 0x0000010c
	.section .rom.000f4694, "ax"
	.global Func_080eb6a0
	.type Func_080eb6a0, %function
	.thumb_func
Func_080eb6a0:
	.incbin "baserom.gba", 0x000f4694, 0x00000184
	.section .rom.000f4818, "ax"
	.incbin "baserom.gba", 0x000f4818, 0x0000010c
	.section .rom.000f4954, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000f4954, 0x000002d0
	.section .rom.000f4d18, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000f4d18, 0x0000014c
	.section .rom.000f4ebc, "ax"
	.global EffectSlot_Initialize
	.type EffectSlot_Initialize, %function
	.thumb_func
EffectSlot_Initialize:
	.incbin "baserom.gba", 0x000f4ebc, 0x000000a0
	.section .rom.000f4f88, "ax"
	.incbin "baserom.gba", 0x000f4f88, 0x000001b8
	.section .rom.000f51c4, "ax"
	.incbin "baserom.gba", 0x000f51c4, 0x000000c8
	.section .rom.000f52ac, "ax"
	.incbin "baserom.gba", 0x000f52ac, 0x000014d0
	.section .rom.000f6a00, "ax"
	.incbin "baserom.gba", 0x000f6a00, 0x000000c0
	.section .rom.000f8810, "ax"
	.incbin "baserom.gba", 0x000f8810, 0x00000008
	.section .rom.000f91ac, "ax"
	.incbin "baserom.gba", 0x000f91ac, 0x0000000c
	.section .rom.000f92fa, "ax"
	.incbin "baserom.gba", 0x000f92fa, 0x00000002
	.section .rom.000f9b44, "ax"
	.incbin "baserom.gba", 0x000f9b44, 0x0000002c
	.section .rom.000f9bf0, "ax"
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000f9bf0, 0x00000008
	.section .rom.000f9bf8, "ax"
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000f9bf8, 0x00000250
	.section .rom.000f9e48, "ax"
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f9e48, 0x00000024
	.section .rom.000f9e6c, "ax"
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000f9e6c, 0x00000024
	.section .rom.000f9e90, "ax"
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000f9e90, 0x00000008
	.section .rom.000f9eb8, "ax"
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f9eb8, 0x00000024
	.section .rom.000f9edc, "ax"
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000f9edc, 0x00000044
	.section .rom.000f9f20, "ax"
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f9f20, 0x000001d4
	.section .rom.000fa114, "ax"
	.incbin "baserom.gba", 0x000fa114, 0x00000020
	.section .rom.000fa134, "ax"
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000fa134, 0x00000668
	.section .rom.000fb1c4, "ax"
	.incbin "baserom.gba", 0x000fb1c4, 0x00000034
	.section .rom.000fc210, "ax"
	.incbin "baserom.gba", 0x000fc210, 0x000000b4
	.section .rom.000fc2c4, "ax"
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000fc2c4, 0x00000010
	.section .rom.000fc2d4, "ax"
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x000fc2d4, 0x00000020
	.section .rom.000fc2f4, "ax"
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000fc2f4, 0x00000128
	.section .rom.000fc41c, "ax"
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x000fc41c, 0x000000bc
	.section .rom.000fc4d8, "ax"
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x000fc4d8, 0x00000004
	.section .rom.000fc4dc, "ax"
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x000fc4dc, 0x0000000c
	.section .rom.000fc4e8, "ax"
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x000fc4e8, 0x0000000c
	.section .rom.000fc4f4, "ax"
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x000fc4f4, 0x0000000c
	.section .rom.000fc500, "ax"
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x000fc500, 0x000000bc
	.section .rom.000fc5bc, "ax"
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x000fc5bc, 0x0000004c
	.section .rom.000fc608, "ax"
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x000fc608, 0x00000128
	.section .rom.000fc730, "ax"
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x000fc730, 0x00000014
	.section .rom.000fc744, "ax"
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x000fc744, 0x00000014
	.section .rom.000fc758, "ax"
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x000fc758, 0x0000001c
	.section .rom.000fc774, "ax"
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000fc774, 0x00000018
	.section .rom.000fc852, "ax"
	.incbin "baserom.gba", 0x000fc852, 0x00000054
	.section .rom.000fc8f5, "ax"
	.incbin "baserom.gba", 0x000fc8f5, 0x00000043
	.section .rom.000fc938, "ax"
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x000fc938, 0x0000000c
	.section .rom.000fc944, "ax"
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x000fc944, 0x00000024
	.section .rom.000fc968, "ax"
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000fc968, 0x00000050
	.section .rom.000fe028, "ax"
	.global UiIcon_PrepareObjectFar
	.type UiIcon_PrepareObjectFar, %function
	.thumb_func
UiIcon_PrepareObjectFar:
	.incbin "baserom.gba", 0x000fe028, 0x00000008
	.section .rom.000fe030, "ax"
	.global ItemMenu_Open
	.type ItemMenu_Open, %function
	.thumb_func
ItemMenu_Open:
	.incbin "baserom.gba", 0x000fe030, 0x00000008
	.section .rom.000fe038, "ax"
	.global ItemMenu_DrawItemDetailsFar
	.type ItemMenu_DrawItemDetailsFar, %function
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	.incbin "baserom.gba", 0x000fe038, 0x00000008
	.section .rom.000fe040, "ax"
	.global Func_080f8040
	.type Func_080f8040, %function
	.thumb_func
Func_080f8040:
	.incbin "baserom.gba", 0x000fe040, 0x00000008
	.section .rom.000fe048, "ax"
	.global Palette_CopyObjectBankToBackground14Far
	.type Palette_CopyObjectBankToBackground14Far, %function
	.thumb_func
Palette_CopyObjectBankToBackground14Far:
	.incbin "baserom.gba", 0x000fe048, 0x00000008
	.section .rom.000fe050, "ax"
	.global Menu_UpdateEntryObjectTransformsFar
	.type Menu_UpdateEntryObjectTransformsFar, %function
	.thumb_func
Menu_UpdateEntryObjectTransformsFar:
	.incbin "baserom.gba", 0x000fe050, 0x00000020
	.section .rom.000fe170, "ax"
	.incbin "baserom.gba", 0x000fe170, 0x00000494
	.section .rom.000fe87c, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000fe87c, 0x0000003c
	.section .rom.000fe8c4, "ax"
	.global Func_080f88d0
	.type Func_080f88d0, %function
	.thumb_func
Func_080f88d0:
	.incbin "baserom.gba", 0x000fe8c4, 0x000001e4
	.section .rom.000feaa8, "ax"
	.global Func_080f8ab4
	.type Func_080f8ab4, %function
	.thumb_func
Func_080f8ab4:
	.incbin "baserom.gba", 0x000feaa8, 0x00000108
	.section .rom.000fecda, "ax"
	.incbin "baserom.gba", 0x000fecda, 0x00000002
	.section .rom.000fecdc, "ax"
	.global Func_080f8ce8
	.type Func_080f8ce8, %function
	.thumb_func
Func_080f8ce8:
	.incbin "baserom.gba", 0x000fecdc, 0x00000258
	.section .rom.000fef90, "ax"
	.incbin "baserom.gba", 0x000fef90, 0x0000016c
	.section .rom.000ff2d0, "ax"
	.incbin "baserom.gba", 0x000ff2d0, 0x00000098
	.section .rom.000ff3aa, "ax"
	.incbin "baserom.gba", 0x000ff3aa, 0x00000002
	.section .rom.000ff496, "ax"
	.incbin "baserom.gba", 0x000ff496, 0x00000002
	.section .rom.000ff498, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000ff498, 0x00000f3c
	.section .rom.00100478, "ax"
	.incbin "baserom.gba", 0x00100478, 0x0000003c
	.section .rom.001005e0, "ax"
	.global Func_080fa5a4
	.type Func_080fa5a4, %function
	.thumb_func
Func_080fa5a4:
	.incbin "baserom.gba", 0x001005e0, 0x000002a8
	.section .rom.001008ac, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001008ac, 0x00000370
	.section .rom.00100c94, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00100c94, 0x00000048
	.section .rom.00100d12, "ax"
	.incbin "baserom.gba", 0x00100d12, 0x00000002
	.section .rom.00100d14, "ax"
	.global Menu_HideEmptyEntryIcons
	.type Menu_HideEmptyEntryIcons, %function
	.thumb_func
Menu_HideEmptyEntryIcons:
	.incbin "baserom.gba", 0x00100d14, 0x00000044
	.section .rom.00100ec8, "ax"
	.incbin "baserom.gba", 0x00100ec8, 0x00000218
	.section .rom.00101140, "ax"
	.incbin "baserom.gba", 0x00101140, 0x00000544
	.section .rom.001017cc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001017cc, 0x0000012c
	.section .rom.00101904, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x00101904, 0x0000047c
	.section .rom.00101eb8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00101eb8, 0x00000618
	.section .rom.001025a8, "ax"
	.incbin "baserom.gba", 0x001025a8, 0x00000594
	.section .rom.00102b3c, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00102b3c, 0x00000854
	.section .rom.001033e4, "ax"
	.incbin "baserom.gba", 0x001033e4, 0x00000244
	.section .rom.00103658, "ax"
	.incbin "baserom.gba", 0x00103658, 0x00000188
	.section .rom.00103820, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x00103820, 0x00000a78
	.section .rom.001043a8, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x001043a8, 0x0000030c
	.section .rom.001046b4, "ax"
	.global Func_080fe580
	.type Func_080fe580, %function
	.thumb_func
Func_080fe580:
	.incbin "baserom.gba", 0x001046b4, 0x00000884
	.section .rom.00104f74, "ax"
	.incbin "baserom.gba", 0x00104f74, 0x000003b4
	.section .rom.001054a4, "ax"
	.incbin "baserom.gba", 0x001054a4, 0x00000570
	.section .rom.00105a80, "ax"
	.incbin "baserom.gba", 0x00105a80, 0x00000b8c
	.section .rom.00106694, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x00106694, 0x000000e4
	.section .rom.0010688c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010688c, 0x00000608
	.section .rom.00106fd0, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00106fd0, 0x00000734
	.section .rom.0010778c, "ax"
	.incbin "baserom.gba", 0x0010778c, 0x00000490
	.section .rom.00107c1c, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00107c1c, 0x00000148
	.section .rom.00107d98, "ax"
	.incbin "baserom.gba", 0x00107d98, 0x0000003c
	.section .rom.00107dd4, "ax"
	.global Func_08101c7c
	.type Func_08101c7c, %function
	.thumb_func
Func_08101c7c:
	.incbin "baserom.gba", 0x00107dd4, 0x000000b8
	.section .rom.00107eb4, "ax"
	.incbin "baserom.gba", 0x00107eb4, 0x00002cf0
	.section .rom.0010aca2, "ax"
	.incbin "baserom.gba", 0x0010aca2, 0x00000002
	.section .rom.0010aca4, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x0010aca4, 0x00000174
	.section .rom.0010aea8, "ax"
	.incbin "baserom.gba", 0x0010aea8, 0x0000004c
	.section .rom.0010aef4, "ax"
	.global Menu_UpdateEntryObjectTransforms
	.type Menu_UpdateEntryObjectTransforms, %function
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	.incbin "baserom.gba", 0x0010aef4, 0x00000238
	.section .rom.0010b12c, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x0010b12c, 0x000000ac
	.section .rom.0010b218, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x0010b218, 0x000000dc
	.section .rom.0010b2f4, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010b2f4, 0x000000c4
	.section .rom.0010b4f4, "ax"
	.incbin "baserom.gba", 0x0010b4f4, 0x000000c0
	.section .rom.0010b62c, "ax"
	.incbin "baserom.gba", 0x0010b62c, 0x000000a0
	.section .rom.0010b742, "ax"
	.incbin "baserom.gba", 0x0010b742, 0x00000346
	.section .rom.0010ba88, "ax"
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x0010ba88, 0x00000004
	.section .rom.0010ba8c, "ax"
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x0010ba8c, 0x00000004
	.section .rom.0010ba90, "ax"
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x0010ba90, 0x00000004
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
	.incbin "baserom.gba", 0x0010babc, 0x00000014
	.section .rom.0010bb00, "ax"
	.incbin "baserom.gba", 0x0010bb00, 0x0000003a
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
	.section .rom.0010bb9c, "ax"
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x0010bb9c, 0x00000008
	.section .rom.0010bba4, "ax"
	.global Data_08105a40
Data_08105a40:
	.incbin "baserom.gba", 0x0010bba4, 0x0000001c
	.section .rom.0010c148, "ax"
	.global Func_08108148
	.type Func_08108148, %function
	.thumb_func
Func_08108148:
	.incbin "baserom.gba", 0x0010c148, 0x00000104
	.section .rom.0010c24c, "ax"
	.global Func_0810824c
	.type Func_0810824c, %function
	.thumb_func
Func_0810824c:
	.incbin "baserom.gba", 0x0010c24c, 0x00000078
	.section .rom.0010c2c4, "ax"
	.global Func_081082c4
	.type Func_081082c4, %function
	.thumb_func
Func_081082c4:
	.incbin "baserom.gba", 0x0010c2c4, 0x000001a0
	.section .rom.0010c690, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010c690, 0x000000cc
	.section .rom.0010c948, "ax"
	.balign 4
	.global Func_08108948
	.type Func_08108948, %function
	.thumb_func
Func_08108948:
	.incbin "baserom.gba", 0x0010c948, 0x0000009c
	.section .rom.0010c9e4, "ax"
	.global Func_081089e4
	.type Func_081089e4, %function
	.thumb_func
Func_081089e4:
	.incbin "baserom.gba", 0x0010c9e4, 0x000000a4
	.section .rom.0010caa8, "ax"
	.balign 4
	.global Func_08108aa8
	.type Func_08108aa8, %function
	.thumb_func
Func_08108aa8:
	.incbin "baserom.gba", 0x0010caa8, 0x00000048
	.section .rom.0010cb70, "ax"
	.incbin "baserom.gba", 0x0010cb70, 0x00000618
	.section .rom.0010d1cc, "ax"
	.incbin "baserom.gba", 0x0010d1cc, 0x000000a4
	.section .rom.0010d28c, "ax"
	.incbin "baserom.gba", 0x0010d28c, 0x00000398
	.section .rom.0010d6f8, "ax"
	.global Func_081096f8
	.type Func_081096f8, %function
	.thumb_func
Func_081096f8:
	.incbin "baserom.gba", 0x0010d6f8, 0x000001c8
	.section .rom.0010dad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x0010dad8, 0x000001d4
	.section .rom.0010dcac, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x0010dcac, 0x0000035c
	.section .rom.0010e008, "ax"
	.global Shop_DrawItemCount
	.type Shop_DrawItemCount, %function
	.thumb_func
Shop_DrawItemCount:
	.incbin "baserom.gba", 0x0010e008, 0x00000104
	.section .rom.0010e10c, "ax"
	.global Func_0810a108
	.type Func_0810a108, %function
	.thumb_func
Func_0810a108:
	.incbin "baserom.gba", 0x0010e10c, 0x000001a4
	.section .rom.0010e2dc, "ax"
	.incbin "baserom.gba", 0x0010e2dc, 0x00000394
	.section .rom.0010e674, "ax"
	.incbin "baserom.gba", 0x0010e674, 0x00000088
	.section .rom.0010e710, "ax"
	.global Forge_FindRecipe
	.type Forge_FindRecipe, %function
	.thumb_func
Forge_FindRecipe:
	.incbin "baserom.gba", 0x0010e710, 0x0000003c
	.section .rom.0010e8f0, "ax"
	.incbin "baserom.gba", 0x0010e8f0, 0x00000074
	.section .rom.0010e9b0, "ax"
	.incbin "baserom.gba", 0x0010e9b0, 0x000000d4
	.section .rom.0010ea84, "ax"
	.global Func_0810aa80
	.type Func_0810aa80, %function
	.thumb_func
Func_0810aa80:
	.incbin "baserom.gba", 0x0010ea84, 0x00000424
	.section .rom.0010ef28, "ax"
	.incbin "baserom.gba", 0x0010ef28, 0x00000198
	.section .rom.0010f0c0, "ax"
	.global BattleFx_UpdateRadialMotion
BattleFx_UpdateRadialMotion:
	.incbin "baserom.gba", 0x0010f0c0, 0x000000ac
	.section .rom.0010f16c, "ax"
	.global Func_0810b168
Func_0810b168:
	.incbin "baserom.gba", 0x0010f16c, 0x00000210
	.section .rom.0010f37c, "ax"
	.global Func_0810b378
	.type Func_0810b378, %function
	.thumb_func
Func_0810b378:
	.incbin "baserom.gba", 0x0010f37c, 0x00000074
	.section .rom.0010f41c, "ax"
	.balign 4
	.global Func_0810b418
	.type Func_0810b418, %function
	.thumb_func
Func_0810b418:
	.incbin "baserom.gba", 0x0010f41c, 0x00000108
	.section .rom.0010f5c8, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x0010f5c8, 0x000000f8
	.section .rom.0010f6d0, "ax"
	.incbin "baserom.gba", 0x0010f6d0, 0x000000d0
	.section .rom.0010f7b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x0010f7b8, 0x0000060c
	.section .rom.0010fe08, "ax"
	.incbin "baserom.gba", 0x0010fe08, 0x00000048
	.section .rom.0010fed0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010fed0, 0x000000dc
	.section .rom.0011001c, "ax"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0011001c, 0x00000140
	.section .rom.0011015c, "ax"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0011015c, 0x00000200
	.section .rom.00110398, "ax"
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x00110398, 0x0000000a
	.section .rom.00118000, "ax"
	.global Resource_FarCall009
Resource_FarCall009:
	.incbin "baserom.gba", 0x00118000, 0x00000010
	.section .rom.00118058, "ax"
	.global Summon_IsEntrySecondaryFlaggedFar
	.type Summon_IsEntrySecondaryFlaggedFar, %function
	.thumb_func
Summon_IsEntrySecondaryFlaggedFar:
	.incbin "baserom.gba", 0x00118058, 0x00000008
	.section .rom.00118060, "ax"
	.global Battle_GetSpriteFlagHighBitsFar
	.type Battle_GetSpriteFlagHighBitsFar, %function
	.thumb_func
Battle_GetSpriteFlagHighBitsFar:
	.incbin "baserom.gba", 0x00118060, 0x00000008
	.section .rom.00118068, "ax"
	.global Summon_IsEntryFlaggedFar
	.type Summon_IsEntryFlaggedFar, %function
	.thumb_func
Summon_IsEntryFlaggedFar:
	.incbin "baserom.gba", 0x00118068, 0x00000008
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
	.incbin "baserom.gba", 0x0011c08c, 0x000000ac
	.section .rom.0011c138, "ax"
	.global Func_0811c120
	.type Func_0811c120, %function
	.thumb_func
Func_0811c120:
	.incbin "baserom.gba", 0x0011c138, 0x000000d8
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
	.incbin "baserom.gba", 0x001a1928, 0x0000085c
	.section .rom.001a2184, "ax"
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x001a2184, 0x000006a4
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
	.incbin "baserom.gba", 0x001a8286, 0x00000476
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
	.incbin "baserom.gba", 0x001b21ec, 0x00001c88
	.section .rom.001b3f18, "ax"
	.incbin "baserom.gba", 0x001b3f18, 0x000009f8
	.section .rom.001b8020, "ax"
	.incbin "baserom.gba", 0x001b8020, 0x00000088
	.section .rom.001b810c, "ax"
	.incbin "baserom.gba", 0x001b810c, 0x000002b8
	.section .rom.001b83c4, "ax"
	.global Func_081b83c4
	.type Func_081b83c4, %function
	.thumb_func
Func_081b83c4:
	.incbin "baserom.gba", 0x001b83c4, 0x00001f98
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
	.section .rom.00697b86, "ax"
	.incbin "baserom.gba", 0x00697b86, 0x00000002
	.section .rom.006a471d, "ax"
	.incbin "baserom.gba", 0x006a471d, 0x00000003
	.section .rom.006a5a6e, "ax"
	.incbin "baserom.gba", 0x006a5a6e, 0x00000002
	.section .rom.006b144b, "ax"
	.incbin "baserom.gba", 0x006b144b, 0x00000001
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
	.section .rom.0087c790, "ax"
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c790, 0x0000024c
	.section .rom.0087c9dc, "ax"
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c9dc, 0x00000198
	.section .rom.0087d39d, "ax"
	.incbin "baserom.gba", 0x0087d39d, 0x00000003
	.section .rom.0087e86f, "ax"
	.incbin "baserom.gba", 0x0087e86f, 0x00000001
	.section .rom.008809d9, "ax"
	.incbin "baserom.gba", 0x008809d9, 0x00000003
	.section .rom.00880d2f, "ax"
	.incbin "baserom.gba", 0x00880d2f, 0x00000001
	.section .rom.00880eb3, "ax"
	.incbin "baserom.gba", 0x00880eb3, 0x00000001
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
	.section .rom.00885a39, "ax"
	.incbin "baserom.gba", 0x00885a39, 0x00000003
	.section .rom.00886b06, "ax"
	.incbin "baserom.gba", 0x00886b06, 0x00000002
	.section .rom.00886b08, "ax"
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x00886b08, 0x000002e8
	.section .rom.00886f42, "ax"
	.incbin "baserom.gba", 0x00886f42, 0x00000002
	.section .rom.0088944f, "ax"
	.incbin "baserom.gba", 0x0088944f, 0x00000001
	.section .rom.00889e0b, "ax"
	.incbin "baserom.gba", 0x00889e0b, 0x00000001
	.section .rom.0088af4e, "ax"
	.incbin "baserom.gba", 0x0088af4e, 0x00000002
	.section .rom.0088bbcc, "ax"
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088bbcc, 0x0000002c
	.section .rom.0088bbf8, "ax"
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088bbf8, 0x0000002c
	.section .rom.0088c3e1, "ax"
	.incbin "baserom.gba", 0x0088c3e1, 0x00000003
	.section .rom.0088d95f, "ax"
	.incbin "baserom.gba", 0x0088d95f, 0x00000001
	.section .rom.0088dc89, "ax"
	.incbin "baserom.gba", 0x0088dc89, 0x00000003
	.section .rom.0088e1b2, "ax"
	.incbin "baserom.gba", 0x0088e1b2, 0x00000002
	.section .rom.0088e6e2, "ax"
	.incbin "baserom.gba", 0x0088e6e2, 0x00000002
	.section .rom.0088f45d, "ax"
	.incbin "baserom.gba", 0x0088f45d, 0x00000003
	.section .rom.0088fa7d, "ax"
	.incbin "baserom.gba", 0x0088fa7d, 0x00000003
	.section .rom.008900a9, "ax"
	.incbin "baserom.gba", 0x008900a9, 0x00000003
	.section .rom.008904b3, "ax"
	.incbin "baserom.gba", 0x008904b3, 0x00000001
	.section .rom.0089070d, "ax"
	.incbin "baserom.gba", 0x0089070d, 0x00000003
	.section .rom.00890c8b, "ax"
	.incbin "baserom.gba", 0x00890c8b, 0x00000001
	.section .rom.00890f35, "ax"
	.incbin "baserom.gba", 0x00890f35, 0x00000003
	.section .rom.00891f51, "ax"
	.incbin "baserom.gba", 0x00891f51, 0x00000003
	.section .rom.00894a03, "ax"
	.incbin "baserom.gba", 0x00894a03, 0x00000001
	.section .rom.00895bcf, "ax"
	.incbin "baserom.gba", 0x00895bcf, 0x00000001
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
	.section .rom.0089b6c2, "ax"
	.incbin "baserom.gba", 0x0089b6c2, 0x00000002
	.section .rom.0089c3fd, "ax"
	.incbin "baserom.gba", 0x0089c3fd, 0x00000003
	.section .rom.0089d639, "ax"
	.incbin "baserom.gba", 0x0089d639, 0x00000003
	.section .rom.0089df83, "ax"
	.incbin "baserom.gba", 0x0089df83, 0x00000001
	.section .rom.0089e5de, "ax"
	.incbin "baserom.gba", 0x0089e5de, 0x00000002
	.section .rom.0089eb0a, "ax"
	.incbin "baserom.gba", 0x0089eb0a, 0x00000002
	.section .rom.008a0dc5, "ax"
	.incbin "baserom.gba", 0x008a0dc5, 0x00000003
	.section .rom.008a2559, "ax"
	.incbin "baserom.gba", 0x008a2559, 0x00000003
	.section .rom.008a2c3d, "ax"
	.incbin "baserom.gba", 0x008a2c3d, 0x00000003
	.section .rom.008a2c40, "ax"
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2c40, 0x00001f4c
	.section .rom.008a6295, "ax"
	.incbin "baserom.gba", 0x008a6295, 0x00000003
	.section .rom.008a6786, "ax"
	.incbin "baserom.gba", 0x008a6786, 0x00000002
	.section .rom.008a6dce, "ax"
	.incbin "baserom.gba", 0x008a6dce, 0x00000002
	.section .rom.008a79f3, "ax"
	.incbin "baserom.gba", 0x008a79f3, 0x00000001
	.section .rom.008a7db5, "ax"
	.incbin "baserom.gba", 0x008a7db5, 0x00000003
	.section .rom.008a7f7f, "ax"
	.incbin "baserom.gba", 0x008a7f7f, 0x00000001
	.section .rom.008a84ca, "ax"
	.incbin "baserom.gba", 0x008a84ca, 0x00000002
	.section .rom.008a8816, "ax"
	.incbin "baserom.gba", 0x008a8816, 0x00000002
	.section .rom.008a8818, "ax"
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a8818, 0x0000076c
	.section .rom.008a95c7, "ax"
	.incbin "baserom.gba", 0x008a95c7, 0x00000001
	.section .rom.008a95c8, "ax"
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a95c8, 0x000000a8
	.section .rom.008a9986, "ax"
	.incbin "baserom.gba", 0x008a9986, 0x00000002
	.section .rom.008aa32d, "ax"
	.incbin "baserom.gba", 0x008aa32d, 0x00000003
	.section .rom.008aa330, "ax"
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa330, 0x000002f8
	.section .rom.008ab155, "ax"
	.incbin "baserom.gba", 0x008ab155, 0x00000003
	.section .rom.008ab158, "ax"
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008ab158, 0x00000100
	.section .rom.008abec9, "ax"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
	.section .rom.008ac711, "ax"
	.incbin "baserom.gba", 0x008ac711, 0x00000003
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
	.section .rom.008b4b3a, "ax"
	.incbin "baserom.gba", 0x008b4b3a, 0x00000002
	.section .rom.008b5b03, "ax"
	.incbin "baserom.gba", 0x008b5b03, 0x00000001
	.section .rom.008b5d5e, "ax"
	.incbin "baserom.gba", 0x008b5d5e, 0x00000002
	.section .rom.008b77b9, "ax"
	.incbin "baserom.gba", 0x008b77b9, 0x00000003
	.section .rom.008b7d05, "ax"
	.incbin "baserom.gba", 0x008b7d05, 0x00000003
	.section .rom.008b9a8e, "ax"
	.incbin "baserom.gba", 0x008b9a8e, 0x00000002
	.section .rom.008b9d07, "ax"
	.incbin "baserom.gba", 0x008b9d07, 0x00000001
	.section .rom.008ba1ce, "ax"
	.incbin "baserom.gba", 0x008ba1ce, 0x00000002
	.section .rom.008bbda3, "ax"
	.incbin "baserom.gba", 0x008bbda3, 0x00000001
	.section .rom.008bd9a1, "ax"
	.incbin "baserom.gba", 0x008bd9a1, 0x00000003
	.section .rom.008bdbc2, "ax"
	.incbin "baserom.gba", 0x008bdbc2, 0x00000002
	.section .rom.008bdfff, "ax"
	.incbin "baserom.gba", 0x008bdfff, 0x00000001
	.section .rom.008be111, "ax"
	.incbin "baserom.gba", 0x008be111, 0x00000003
	.section .rom.008be6f3, "ax"
	.incbin "baserom.gba", 0x008be6f3, 0x00000001
	.section .rom.008beb91, "ax"
	.incbin "baserom.gba", 0x008beb91, 0x00000003
	.section .rom.008bf8a6, "ax"
	.incbin "baserom.gba", 0x008bf8a6, 0x00000002
	.section .rom.008c0626, "ax"
	.incbin "baserom.gba", 0x008c0626, 0x00000002
	.section .rom.008c09b7, "ax"
	.incbin "baserom.gba", 0x008c09b7, 0x00000001
	.section .rom.008c26fe, "ax"
	.incbin "baserom.gba", 0x008c26fe, 0x00000002
	.section .rom.008c34d5, "ax"
	.incbin "baserom.gba", 0x008c34d5, 0x00000003
	.section .rom.008c3d02, "ax"
	.incbin "baserom.gba", 0x008c3d02, 0x00000002
	.section .rom.008c42ae, "ax"
	.incbin "baserom.gba", 0x008c42ae, 0x00000002
	.section .rom.008c446b, "ax"
	.incbin "baserom.gba", 0x008c446b, 0x00000001
	.section .rom.008c51c3, "ax"
	.incbin "baserom.gba", 0x008c51c3, 0x00000001
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
	.section .rom.008cb2e9, "ax"
	.incbin "baserom.gba", 0x008cb2e9, 0x00000003
	.section .rom.008cc0b7, "ax"
	.incbin "baserom.gba", 0x008cc0b7, 0x00000001
	.section .rom.008cd321, "ax"
	.incbin "baserom.gba", 0x008cd321, 0x00000003
	.section .rom.008cd87b, "ax"
	.incbin "baserom.gba", 0x008cd87b, 0x00000001
	.section .rom.008cf165, "ax"
	.incbin "baserom.gba", 0x008cf165, 0x00000003
	.section .rom.008cfa06, "ax"
	.incbin "baserom.gba", 0x008cfa06, 0x00000002
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
	.section .rom.008d4d16, "ax"
	.incbin "baserom.gba", 0x008d4d16, 0x00000002
	.section .rom.008d5099, "ax"
	.incbin "baserom.gba", 0x008d5099, 0x00000003
	.section .rom.008d5d49, "ax"
	.incbin "baserom.gba", 0x008d5d49, 0x00000003
	.section .rom.008d6891, "ax"
	.incbin "baserom.gba", 0x008d6891, 0x00000003
	.section .rom.008d6a2a, "ax"
	.incbin "baserom.gba", 0x008d6a2a, 0x00000002
	.section .rom.008d72b7, "ax"
	.incbin "baserom.gba", 0x008d72b7, 0x00000001
	.section .rom.008d7e86, "ax"
	.incbin "baserom.gba", 0x008d7e86, 0x00000002
	.section .rom.008d839e, "ax"
	.incbin "baserom.gba", 0x008d839e, 0x00000002
	.section .rom.008d89c2, "ax"
	.incbin "baserom.gba", 0x008d89c2, 0x00000002
	.section .rom.008d8de5, "ax"
	.incbin "baserom.gba", 0x008d8de5, 0x00000003
	.section .rom.008d907f, "ax"
	.incbin "baserom.gba", 0x008d907f, 0x00000001
	.section .rom.008daa1b, "ax"
	.incbin "baserom.gba", 0x008daa1b, 0x00000001
	.section .rom.008dc792, "ax"
	.incbin "baserom.gba", 0x008dc792, 0x00000002
	.section .rom.008de707, "ax"
	.incbin "baserom.gba", 0x008de707, 0x00000001
	.section .rom.008debd7, "ax"
	.incbin "baserom.gba", 0x008debd7, 0x00000001
	.section .rom.008df269, "ax"
	.incbin "baserom.gba", 0x008df269, 0x00000003
	.section .rom.008dfc9f, "ax"
	.incbin "baserom.gba", 0x008dfc9f, 0x00000001
	.section .rom.008e1071, "ax"
	.incbin "baserom.gba", 0x008e1071, 0x00000003
	.section .rom.008e1b42, "ax"
	.incbin "baserom.gba", 0x008e1b42, 0x00000002
	.section .rom.008e267f, "ax"
	.incbin "baserom.gba", 0x008e267f, 0x00000001
	.section .rom.008e2cbf, "ax"
	.incbin "baserom.gba", 0x008e2cbf, 0x00000001
	.section .rom.008e4246, "ax"
	.incbin "baserom.gba", 0x008e4246, 0x00000002
	.section .rom.008e42a9, "ax"
	.incbin "baserom.gba", 0x008e42a9, 0x00000003
	.section .rom.008e4617, "ax"
	.incbin "baserom.gba", 0x008e4617, 0x00000001
	.section .rom.008e4bb5, "ax"
	.incbin "baserom.gba", 0x008e4bb5, 0x00000003
	.section .rom.008e4bb8, "ax"
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e4bb8, 0x00000204
	.section .rom.008e50f5, "ax"
	.incbin "baserom.gba", 0x008e50f5, 0x00000003
	.section .rom.008e610e, "ax"
	.incbin "baserom.gba", 0x008e610e, 0x00000002
	.section .rom.008e971a, "ax"
	.incbin "baserom.gba", 0x008e971a, 0x00000002
	.section .rom.008e9ebd, "ax"
	.incbin "baserom.gba", 0x008e9ebd, 0x00000003
	.section .rom.008eb24f, "ax"
	.incbin "baserom.gba", 0x008eb24f, 0x00000001
	.section .rom.008eb67f, "ax"
	.incbin "baserom.gba", 0x008eb67f, 0x00000001
	.section .rom.008ed475, "ax"
	.incbin "baserom.gba", 0x008ed475, 0x00000003
	.section .rom.008edd8a, "ax"
	.incbin "baserom.gba", 0x008edd8a, 0x00000002
	.section .rom.008ef8be, "ax"
	.incbin "baserom.gba", 0x008ef8be, 0x00000002
	.section .rom.008f090d, "ax"
	.incbin "baserom.gba", 0x008f090d, 0x00000003
	.section .rom.008f19b5, "ax"
	.incbin "baserom.gba", 0x008f19b5, 0x00000003
	.section .rom.0093c7dd, "ax"
	.incbin "baserom.gba", 0x0093c7dd, 0x00000003
	.section .rom.0093c9f5, "ax"
	.incbin "baserom.gba", 0x0093c9f5, 0x00000003
	.section .rom.0093cb49, "ax"
	.incbin "baserom.gba", 0x0093cb49, 0x00000003
	.section .rom.0093d001, "ax"
	.incbin "baserom.gba", 0x0093d001, 0x00000003
	.section .rom.0093e76e, "ax"
	.incbin "baserom.gba", 0x0093e76e, 0x00000002
	.section .rom.0093fca5, "ax"
	.incbin "baserom.gba", 0x0093fca5, 0x00000003
	.section .rom.00941afb, "ax"
	.incbin "baserom.gba", 0x00941afb, 0x00000001
	.section .rom.00943c0f, "ax"
	.incbin "baserom.gba", 0x00943c0f, 0x00000001
	.section .rom.00944389, "ax"
	.incbin "baserom.gba", 0x00944389, 0x00000003
	.section .rom.00946a07, "ax"
	.incbin "baserom.gba", 0x00946a07, 0x00000001
	.section .rom.0094740f, "ax"
	.incbin "baserom.gba", 0x0094740f, 0x00000001
	.section .rom.0094a12e, "ax"
	.incbin "baserom.gba", 0x0094a12e, 0x00000002
	.section .rom.0094a306, "ax"
	.incbin "baserom.gba", 0x0094a306, 0x00000002
	.section .rom.0094a765, "ax"
	.incbin "baserom.gba", 0x0094a765, 0x00000003
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
	.section .rom.0095028e, "ax"
	.incbin "baserom.gba", 0x0095028e, 0x00000002
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
	.section .rom.009543d1, "ax"
	.incbin "baserom.gba", 0x009543d1, 0x00000003
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
	.section .rom.009682e2, "ax"
	.incbin "baserom.gba", 0x009682e2, 0x00000002
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
	.section .rom.0096a0be, "ax"
	.incbin "baserom.gba", 0x0096a0be, 0x00000002
	.section .rom.0096a179, "ax"
	.incbin "baserom.gba", 0x0096a179, 0x00000003
	.section .rom.0096a34e, "ax"
	.incbin "baserom.gba", 0x0096a34e, 0x00000002
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
	.section .rom.009c07f7, "ax"
	.incbin "baserom.gba", 0x009c07f7, 0x00000001
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
	.section .rom.00a79d17, "ax"
	.incbin "baserom.gba", 0x00a79d17, 0x00000001
	.section .rom.00a79e66, "ax"
	.incbin "baserom.gba", 0x00a79e66, 0x00000002
	.section .rom.00a79fa7, "ax"
	.incbin "baserom.gba", 0x00a79fa7, 0x00000001
	.section .rom.00a7a0e7, "ax"
	.incbin "baserom.gba", 0x00a7a0e7, 0x00000001
	.section .rom.00a7a227, "ax"
	.incbin "baserom.gba", 0x00a7a227, 0x00000001
	.section .rom.00a7b485, "ax"
	.incbin "baserom.gba", 0x00a7b485, 0x00000003
	.section .rom.00a7b656, "ax"
	.incbin "baserom.gba", 0x00a7b656, 0x00000002
	.section .rom.00a7d6f7, "ax"
	.incbin "baserom.gba", 0x00a7d6f7, 0x00000001
	.section .rom.00a80959, "ax"
	.incbin "baserom.gba", 0x00a80959, 0x00000003
	.section .rom.00a861cb, "ax"
	.incbin "baserom.gba", 0x00a861cb, 0x00000001
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
	.section .rom.00b1aeb6, "ax"
	.incbin "baserom.gba", 0x00b1aeb6, 0x00000002
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
	.section .rom.00c28e95, "ax"
	.incbin "baserom.gba", 0x00c28e95, 0x00000003
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
	.section .rom.00d2b101, "ax"
	.incbin "baserom.gba", 0x00d2b101, 0x00000003
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
	.section .rom.00d49cb2, "ax"
	.incbin "baserom.gba", 0x00d49cb2, 0x00000002
	.section .rom.00d4c017, "ax"
	.incbin "baserom.gba", 0x00d4c017, 0x00000001
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
	.section .rom.00e6a23a, "ax"
	.incbin "baserom.gba", 0x00e6a23a, 0x00000002
	.section .rom.00e6b7eb, "ax"
	.incbin "baserom.gba", 0x00e6b7eb, 0x00000001
	.section .rom.00e6b99e, "ax"
	.incbin "baserom.gba", 0x00e6b99e, 0x00000002
	.section .rom.00e6dbdd, "ax"
	.incbin "baserom.gba", 0x00e6dbdd, 0x00000003
	.section .rom.00e6e703, "ax"
	.incbin "baserom.gba", 0x00e6e703, 0x00000001
	.section .rom.00e79961, "ax"
	.incbin "baserom.gba", 0x00e79961, 0x00000003
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
	.section .rom.00eb6ca1, "ax"
	.incbin "baserom.gba", 0x00eb6ca1, 0x00000003
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
	.section .rom.00ebdfc3, "ax"
	.incbin "baserom.gba", 0x00ebdfc3, 0x00000001
	.section .rom.00ebe112, "ax"
	.incbin "baserom.gba", 0x00ebe112, 0x00000002
	.section .rom.00ebe253, "ax"
	.incbin "baserom.gba", 0x00ebe253, 0x00000001
	.section .rom.00ebe393, "ax"
	.incbin "baserom.gba", 0x00ebe393, 0x00000001
	.section .rom.00ebe4d3, "ax"
	.incbin "baserom.gba", 0x00ebe4d3, 0x00000001
	.section .rom.00ebebe9, "ax"
	.incbin "baserom.gba", 0x00ebebe9, 0x00000003
	.section .rom.00ebebf7, "ax"
	.incbin "baserom.gba", 0x00ebebf7, 0x00000001
	.section .rom.00ebed46, "ax"
	.incbin "baserom.gba", 0x00ebed46, 0x00000002
	.section .rom.00ebee87, "ax"
	.incbin "baserom.gba", 0x00ebee87, 0x00000001
	.section .rom.00ebefc7, "ax"
	.incbin "baserom.gba", 0x00ebefc7, 0x00000001
	.section .rom.00ebf107, "ax"
	.incbin "baserom.gba", 0x00ebf107, 0x00000001
	.section .rom.00ebfcc9, "ax"
	.incbin "baserom.gba", 0x00ebfcc9, 0x00000003
	.section .rom.00ebfcd7, "ax"
	.incbin "baserom.gba", 0x00ebfcd7, 0x00000001
	.section .rom.00ebfe26, "ax"
	.incbin "baserom.gba", 0x00ebfe26, 0x00000002
	.section .rom.00ebff67, "ax"
	.incbin "baserom.gba", 0x00ebff67, 0x00000001
	.section .rom.00ec00a7, "ax"
	.incbin "baserom.gba", 0x00ec00a7, 0x00000001
	.section .rom.00ec01e7, "ax"
	.incbin "baserom.gba", 0x00ec01e7, 0x00000001
	.section .rom.00ec07b5, "ax"
	.incbin "baserom.gba", 0x00ec07b5, 0x00000003
	.section .rom.00ec07c3, "ax"
	.incbin "baserom.gba", 0x00ec07c3, 0x00000001
	.section .rom.00ec0912, "ax"
	.incbin "baserom.gba", 0x00ec0912, 0x00000002
	.section .rom.00ec0a53, "ax"
	.incbin "baserom.gba", 0x00ec0a53, 0x00000001
	.section .rom.00ec0b93, "ax"
	.incbin "baserom.gba", 0x00ec0b93, 0x00000001
	.section .rom.00ec0cd3, "ax"
	.incbin "baserom.gba", 0x00ec0cd3, 0x00000001
	.section .rom.00ec1651, "ax"
	.incbin "baserom.gba", 0x00ec1651, 0x00000003
	.section .rom.00ec165f, "ax"
	.incbin "baserom.gba", 0x00ec165f, 0x00000001
	.section .rom.00ec17ae, "ax"
	.incbin "baserom.gba", 0x00ec17ae, 0x00000002
	.section .rom.00ec18ef, "ax"
	.incbin "baserom.gba", 0x00ec18ef, 0x00000001
	.section .rom.00ec1a2f, "ax"
	.incbin "baserom.gba", 0x00ec1a2f, 0x00000001
	.section .rom.00ec1b6f, "ax"
	.incbin "baserom.gba", 0x00ec1b6f, 0x00000001
	.section .rom.00ec1f99, "ax"
	.incbin "baserom.gba", 0x00ec1f99, 0x00000003
	.section .rom.00ec1fa7, "ax"
	.incbin "baserom.gba", 0x00ec1fa7, 0x00000001
	.section .rom.00ec20f6, "ax"
	.incbin "baserom.gba", 0x00ec20f6, 0x00000002
	.section .rom.00ec2237, "ax"
	.incbin "baserom.gba", 0x00ec2237, 0x00000001
	.section .rom.00ec2377, "ax"
	.incbin "baserom.gba", 0x00ec2377, 0x00000001
	.section .rom.00ec24b7, "ax"
	.incbin "baserom.gba", 0x00ec24b7, 0x00000001
	.section .rom.00ec2ae1, "ax"
	.incbin "baserom.gba", 0x00ec2ae1, 0x00000003
	.section .rom.00ec2aef, "ax"
	.incbin "baserom.gba", 0x00ec2aef, 0x00000001
	.section .rom.00ec2c3e, "ax"
	.incbin "baserom.gba", 0x00ec2c3e, 0x00000002
	.section .rom.00ec2d7f, "ax"
	.incbin "baserom.gba", 0x00ec2d7f, 0x00000001
	.section .rom.00ec2ebf, "ax"
	.incbin "baserom.gba", 0x00ec2ebf, 0x00000001
	.section .rom.00ec2fff, "ax"
	.incbin "baserom.gba", 0x00ec2fff, 0x00000001
	.section .rom.00ec3729, "ax"
	.incbin "baserom.gba", 0x00ec3729, 0x00000003
	.section .rom.00ec3737, "ax"
	.incbin "baserom.gba", 0x00ec3737, 0x00000001
	.section .rom.00ec3886, "ax"
	.incbin "baserom.gba", 0x00ec3886, 0x00000002
	.section .rom.00ec39c7, "ax"
	.incbin "baserom.gba", 0x00ec39c7, 0x00000001
	.section .rom.00ec3b07, "ax"
	.incbin "baserom.gba", 0x00ec3b07, 0x00000001
	.section .rom.00ec3c47, "ax"
	.incbin "baserom.gba", 0x00ec3c47, 0x00000001
