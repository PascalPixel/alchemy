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
	.incbin "baserom.gba", 0x0003addc, 0x00000208
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
	.global UiText_GetResourceDimensionsAlt
	.type UiText_GetResourceDimensionsAlt, %function
	.thumb_func
UiText_GetResourceDimensionsAlt:
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
	.section .rom.0003c198, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c198, 0x000001e0
	.section .rom.0003c40c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c40c, 0x0000013c
	.section .rom.0003c548, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c548, 0x00000464
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
	.section .rom.0003cbfe, "ax"
	.incbin "baserom.gba", 0x0003cbfe, 0x00000002
	.section .rom.0003cd60, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd60, 0x00000114
	.section .rom.0003cebc, "ax"
	.balign 4
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cebc, 0x000000fc
	.section .rom.0003cfc4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cfc4, 0x00000034
	.section .rom.0003d028, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003d028, 0x00000050
	.section .rom.0003d078, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d078, 0x00000158
	.section .rom.0003d21c, "ax"
	.incbin "baserom.gba", 0x0003d21c, 0x0000010c
	.section .rom.0003d328, "ax"
	.global UiWork_SetTwoEntriesTo999
	.type UiWork_SetTwoEntriesTo999, %function
	.thumb_func
UiWork_SetTwoEntriesTo999:
	.incbin "baserom.gba", 0x0003d328, 0x00000020
	.section .rom.0003d53c, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d53c, 0x000000b0
	.section .rom.0003d61c, "ax"
	.balign 4
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
	.section .rom.0003da12, "ax"
	.incbin "baserom.gba", 0x0003da12, 0x00000002
	.section .rom.0003da14, "ax"
	.global UiIcon_BuildAbilityIconTiles
	.type UiIcon_BuildAbilityIconTiles, %function
	.thumb_func
UiIcon_BuildAbilityIconTiles:
	.incbin "baserom.gba", 0x0003da14, 0x000000bc
	.section .rom.0003dc74, "ax"
	.balign 4
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc74, 0x00000108
	.section .rom.0003ddf0, "ax"
	.balign 4
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddf0, 0x00000110
	.section .rom.0003df7c, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003df7c, 0x00000684
	.section .rom.0003e820, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e820, 0x00000124
	.section .rom.0003eaac, "ax"
	.incbin "baserom.gba", 0x0003eaac, 0x00000188
	.section .rom.0003ec34, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec34, 0x000001bc
	.section .rom.0003edf0, "ax"
	.global Menu_StepLeft
	.type Menu_StepLeft, %function
	.thumb_func
Menu_StepLeft:
	.incbin "baserom.gba", 0x0003edf0, 0x000001b0
	.section .rom.0003f05c, "ax"
	.incbin "baserom.gba", 0x0003f05c, 0x000001d0
	.section .rom.0003f33c, "ax"
	.incbin "baserom.gba", 0x0003f33c, 0x000000e4
	.section .rom.0003f420, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f420, 0x00000248
	.section .rom.0003f67c, "ax"
	.balign 4
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f67c, 0x00000074
	.section .rom.0003f7d0, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0003f7d0, 0x00000088
	.section .rom.0003f858, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0003f858, 0x00000010
	.section .rom.0003fa18, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa18, 0x000001a0
	.section .rom.0003fc90, "ax"
	.incbin "baserom.gba", 0x0003fc90, 0x000000dc
	.section .rom.0003fd80, "ax"
	.incbin "baserom.gba", 0x0003fd80, 0x000003c0
	.section .rom.00040140, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x00040140, 0x000004c4
	.section .rom.00040680, "ax"
	.incbin "baserom.gba", 0x00040680, 0x00000364
	.section .rom.00040a48, "ax"
	.incbin "baserom.gba", 0x00040a48, 0x00000270
	.section .rom.00040dc8, "ax"
	.incbin "baserom.gba", 0x00040dc8, 0x000000f4
	.section .rom.000410c0, "ax"
	.incbin "baserom.gba", 0x000410c0, 0x0000019c
	.section .rom.000412e4, "ax"
	.incbin "baserom.gba", 0x000412e4, 0x00000440
	.section .rom.00041724, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x00041724, 0x000003fc
	.section .rom.00041b20, "ax"
	.global UiWindow_ClearTileAttributesInRect
	.type UiWindow_ClearTileAttributesInRect, %function
	.thumb_func
UiWindow_ClearTileAttributesInRect:
	.incbin "baserom.gba", 0x00041b20, 0x000000ac
	.section .rom.00041bcc, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bcc, 0x000000a4
	.section .rom.00041cb8, "ax"
	.balign 4
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
	.section .rom.00042008, "ax"
	.balign 4
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
	.section .rom.00042160, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042160, 0x0000008c
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
	.global CreateSideObject
	.type CreateSideObject, %function
	.thumb_func
CreateSideObject:
	.incbin "baserom.gba", 0x000424b4, 0x000000b8
	.section .rom.0004256c, "ax"
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x0004256c, 0x00000064
	.section .rom.000425de, "ax"
	.incbin "baserom.gba", 0x000425de, 0x00000002
	.section .rom.000425ec, "ax"
	.balign 4
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x000425ec, 0x00000074
	.section .rom.00042744, "ax"
	.incbin "baserom.gba", 0x00042744, 0x0000029c
	.section .rom.000429e0, "ax"
	.global PtyStDraw
	.type PtyStDraw, %function
	.thumb_func
PtyStDraw:
	.incbin "baserom.gba", 0x000429e0, 0x00000430
	.section .rom.00042e28, "ax"
	.incbin "baserom.gba", 0x00042e28, 0x00000064
	.section .rom.00042f74, "ax"
	.global Func_08042f10
	.type Func_08042f10, %function
	.thumb_func
Func_08042f10:
	.incbin "baserom.gba", 0x00042f74, 0x000000e0
	.section .rom.00043054, "ax"
	.global Func_08042ff0
	.type Func_08042ff0, %function
	.thumb_func
Func_08042ff0:
	.incbin "baserom.gba", 0x00043054, 0x000000d4
	.section .rom.00043128, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043128, 0x000000ec
	.section .rom.00043214, "ax"
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x00043214, 0x00000080
	.section .rom.00043308, "ax"
	.balign 4
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x00043308, 0x000000b4
	.section .rom.000433bc, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x000433bc, 0x00000194
	.section .rom.00043598, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043598, 0x000000c8
	.section .rom.000436f4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436f4, 0x000000e4
	.section .rom.00043ac8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ac8, 0x000000d0
	.section .rom.00043bd4, "ax"
	.global Func_08043b70
	.type Func_08043b70, %function
	.thumb_func
Func_08043b70:
	.incbin "baserom.gba", 0x00043bd4, 0x0000007c
	.section .rom.00043d3c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d3c, 0x0000066c
	.section .rom.000443ac, "ax"
	.incbin "baserom.gba", 0x000443ac, 0x00000090
	.section .rom.000445bc, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445bc, 0x000004fc
	.section .rom.00044abc, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044abc, 0x00000128
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
	.incbin "baserom.gba", 0x00044e2c, 0x00000184
	.section .rom.0004507c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004507c, 0x00000030
	.section .rom.00045160, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x00045160, 0x000000a0
	.section .rom.00045220, "ax"
	.incbin "baserom.gba", 0x00045220, 0x00000090
	.section .rom.0004531e, "ax"
	.incbin "baserom.gba", 0x0004531e, 0x00000002
	.section .rom.00045320, "ax"
	.global Graphics_MergeShiftedTileRows
	.type Graphics_MergeShiftedTileRows, %function
	.thumb_func
Graphics_MergeShiftedTileRows:
	.incbin "baserom.gba", 0x00045320, 0x00000074
	.section .rom.0004558a, "ax"
	.incbin "baserom.gba", 0x0004558a, 0x00000002
	.section .rom.000455c8, "ax"
	.incbin "baserom.gba", 0x000455c8, 0x0000004c
	.section .rom.0004575c, "ax"
	.incbin "baserom.gba", 0x0004575c, 0x00000070
	.section .rom.000457e2, "ax"
	.incbin "baserom.gba", 0x000457e2, 0x00000002
	.section .rom.00045832, "ax"
	.incbin "baserom.gba", 0x00045832, 0x00000966
	.section .rom.000462e8, "ax"
	.incbin "baserom.gba", 0x000462e8, 0x00000150
	.section .rom.00046476, "ax"
	.incbin "baserom.gba", 0x00046476, 0x0000364e
	.section .rom.00049b12, "ax"
	.incbin "baserom.gba", 0x00049b12, 0x00000002
	.section .rom.00049b40, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00049b40, 0x00001d54
	.section .rom.0004b9b4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004b9b4, 0x000000bc
	.section .rom.0004baaa, "ax"
	.incbin "baserom.gba", 0x0004baaa, 0x000002f2
	.section .rom.0004bdc4, "ax"
	.incbin "baserom.gba", 0x0004bdc4, 0x00000038
	.section .rom.0004bdfc, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004bdfc, 0x000011a0
	.section .rom.0004cf9c, "ax"
	.global Func_0804cda8
	.type Func_0804cda8, %function
	.thumb_func
Func_0804cda8:
	.incbin "baserom.gba", 0x0004cf9c, 0x00000334
	.section .rom.0004d360, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d360, 0x00000120
	.section .rom.0004d480, "ax"
	.global Menu_SelectResource
	.type Menu_SelectResource, %function
	.thumb_func
Menu_SelectResource:
	.incbin "baserom.gba", 0x0004d480, 0x000000b8
	.section .rom.0004d5dc, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5dc, 0x0000009c
	.section .rom.0004d678, "ax"
	.global Menu_LayoutResourceEntries
	.type Menu_LayoutResourceEntries, %function
	.thumb_func
Menu_LayoutResourceEntries:
	.incbin "baserom.gba", 0x0004d678, 0x00000074
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
	.section .rom.0004dc86, "ax"
	.incbin "baserom.gba", 0x0004dc86, 0x00000002
	.section .rom.0004dd40, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x0004dd40, 0x00000254
	.section .rom.0004e06c, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e06c, 0x000000c4
	.section .rom.0004e29a, "ax"
	.incbin "baserom.gba", 0x0004e29a, 0x00000002
	.section .rom.0004e29c, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e29c, 0x0000007c
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
	.incbin "baserom.gba", 0x0004e5c0, 0x00000190
	.section .rom.0004e850, "ax"
	.incbin "baserom.gba", 0x0004e850, 0x000004d4
	.section .rom.000599bc, "ax"
	.incbin "baserom.gba", 0x000599bc, 0x00000084
	.section .rom.00059b40, "ax"
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b40, 0x00000ba4
	.section .rom.0005c2e4, "ax"
	.incbin "baserom.gba", 0x0005c2e4, 0x00000090
	.section .rom.0005c3fc, "ax"
	.incbin "baserom.gba", 0x0005c3fc, 0x00000010
	.section .rom.0005ec0c, "ax"
	.incbin "baserom.gba", 0x0005ec0c, 0x0000018c
	.section .rom.0005ed98, "ax"
	.global Data_0805eb50
Data_0805eb50:
	.incbin "baserom.gba", 0x0005ed98, 0x00000008
	.section .rom.0005eda0, "ax"
	.global Data_0805eb58
Data_0805eb58:
	.incbin "baserom.gba", 0x0005eda0, 0x00000024
	.section .rom.0005edc4, "ax"
	.global Data_0805eb7c
Data_0805eb7c:
	.incbin "baserom.gba", 0x0005edc4, 0x00000108
	.section .rom.0005f6cc, "ax"
	.incbin "baserom.gba", 0x0005f6cc, 0x00000101
	.section .rom.0005f7cd, "ax"
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x0005f7cd, 0x000000bb
	.section .rom.0005f888, "ax"
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f888, 0x00000026
	.section .rom.0005f8ae, "ax"
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x0005f8ae, 0x0000007a
	.section .rom.0005f9f8, "ax"
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005f9f8, 0x00000008
	.section .rom.0005faa0, "ax"
	.incbin "baserom.gba", 0x0005faa0, 0x0000005e
	.section .rom.0005fafe, "ax"
	.global Menu_TopEntryCommandByPosition
	.type Menu_TopEntryCommandByPosition, %function
	.thumb_func
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fafe, 0x0000000c
	.section .rom.0005fb0a, "ax"
	.global Menu_TopEntryPositionByCommand
	.type Menu_TopEntryPositionByCommand, %function
	.thumb_func
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fb0a, 0x00000026
	.section .rom.0005fb30, "ax"
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0005fb30, 0x0000003c
	.section .rom.000a3134, "ax"
	.incbin "baserom.gba", 0x000a3134, 0x0000011a
	.section .rom.000a328e, "ax"
	.incbin "baserom.gba", 0x000a328e, 0x000002b6
	.section .rom.000a3544, "ax"
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000a3544, 0x00000004
	.section .rom.000ad070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000ad070, 0x00000008
	.section .rom.000ad078, "ax"
	.global BtlActGet
	.type BtlActGet, %function
	.thumb_func
BtlActGet:
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
	.incbin "baserom.gba", 0x000ad088, 0x00000008
	.section .rom.000ad090, "ax"
	.global Func_080ad090
	.type Func_080ad090, %function
	.thumb_func
Func_080ad090:
	.incbin "baserom.gba", 0x000ad090, 0x00000008
	.section .rom.000ad098, "ax"
	.global Func_080ad098
	.type Func_080ad098, %function
	.thumb_func
Func_080ad098:
	.incbin "baserom.gba", 0x000ad098, 0x00000008
	.section .rom.000ad0a0, "ax"
	.global Func_080ad0a0
	.type Func_080ad0a0, %function
	.thumb_func
Func_080ad0a0:
	.incbin "baserom.gba", 0x000ad0a0, 0x00000008
	.section .rom.000ad0a8, "ax"
	.global Func_080ad0a8
	.type Func_080ad0a8, %function
	.thumb_func
Func_080ad0a8:
	.incbin "baserom.gba", 0x000ad0a8, 0x00000008
	.section .rom.000ad0c0, "ax"
	.global Owner_AdjustFirstValueFar
	.type Owner_AdjustFirstValueFar, %function
	.thumb_func
Owner_AdjustFirstValueFar:
	.incbin "baserom.gba", 0x000ad0c0, 0x00000008
	.section .rom.000ad0c8, "ax"
	.global UnitEpAddFar
	.type UnitEpAddFar, %function
	.thumb_func
UnitEpAddFar:
	.incbin "baserom.gba", 0x000ad0c8, 0x00000008
	.section .rom.000ad0d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000ad0d0, 0x00000008
	.section .rom.000ad0d8, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000ad0d8, 0x00000008
	.section .rom.000ad0e0, "ax"
	.global Owner_UpdateSecondInputAndRatiosFar
	.type Owner_UpdateSecondInputAndRatiosFar, %function
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000ad0e0, 0x00000008
	.section .rom.000ad0e8, "ax"
	.global BtlUnitSetFar
	.type BtlUnitSetFar, %function
	.thumb_func
BtlUnitSetFar:
	.incbin "baserom.gba", 0x000ad0e8, 0x00000008
	.section .rom.000ad0f0, "ax"
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
	.incbin "baserom.gba", 0x000ad0f8, 0x00000008
	.section .rom.000ad100, "ax"
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000ad100, 0x00000008
	.section .rom.000ad108, "ax"
	.global RollWeaponUnleashFar
	.type RollWeaponUnleashFar, %function
	.thumb_func
RollWeaponUnleashFar:
	.incbin "baserom.gba", 0x000ad108, 0x00000008
	.section .rom.000ad110, "ax"
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000ad110, 0x00000008
	.section .rom.000ad118, "ax"
	.global Item_GetEquippedElementFar
	.type Item_GetEquippedElementFar, %function
	.thumb_func
Item_GetEquippedElementFar:
	.incbin "baserom.gba", 0x000ad118, 0x00000008
	.section .rom.000ad120, "ax"
	.global BtlHitChk
	.type BtlHitChk, %function
	.thumb_func
BtlHitChk:
	.incbin "baserom.gba", 0x000ad120, 0x00000008
	.section .rom.000ad128, "ax"
	.global BtlAtkCalc
	.type BtlAtkCalc, %function
	.thumb_func
BtlAtkCalc:
	.incbin "baserom.gba", 0x000ad128, 0x00000008
	.section .rom.000ad130, "ax"
	.global BtlPowCalc
	.type BtlPowCalc, %function
	.thumb_func
BtlPowCalc:
	.incbin "baserom.gba", 0x000ad130, 0x00000008
	.section .rom.000ad138, "ax"
	.global BtlRcvCalc
	.type BtlRcvCalc, %function
	.thumb_func
BtlRcvCalc:
	.incbin "baserom.gba", 0x000ad138, 0x00000008
	.section .rom.000ad140, "ax"
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000ad140, 0x00000008
	.section .rom.000ad148, "ax"
	.global BtlRndFar
	.type BtlRndFar, %function
	.thumb_func
BtlRndFar:
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
	.incbin "baserom.gba", 0x000ad158, 0x00000008
	.section .rom.000ad160, "ax"
	.global Djinn_DeactivateFar
	.type Djinn_DeactivateFar, %function
	.thumb_func
Djinn_DeactivateFar:
	.incbin "baserom.gba", 0x000ad160, 0x00000008
	.section .rom.000ad168, "ax"
	.global Trade_RemoveOfferFar
	.type Trade_RemoveOfferFar, %function
	.thumb_func
Trade_RemoveOfferFar:
	.incbin "baserom.gba", 0x000ad168, 0x00000008
	.section .rom.000ad170, "ax"
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000ad170, 0x00000008
	.section .rom.000ad178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000ad178, 0x00000008
	.section .rom.000ad180, "ax"
	.global Trade_ListFlaggedEntriesFar
	.type Trade_ListFlaggedEntriesFar, %function
	.thumb_func
Trade_ListFlaggedEntriesFar:
	.incbin "baserom.gba", 0x000ad180, 0x00000008
	.section .rom.000ad188, "ax"
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000ad188, 0x00000008
	.section .rom.000ad190, "ax"
	.global Djinn_GetDefinitionHeaderFar
	.type Djinn_GetDefinitionHeaderFar, %function
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000ad190, 0x00000008
	.section .rom.000ad198, "ax"
	.global Party_AdvanceOwnerCountToTargetFar
	.type Party_AdvanceOwnerCountToTargetFar, %function
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000ad198, 0x00000008
	.section .rom.000ad1a0, "ax"
	.global Owner_GetResistanceValueFar
	.type Owner_GetResistanceValueFar, %function
	.thumb_func
Owner_GetResistanceValueFar:
	.incbin "baserom.gba", 0x000ad1a0, 0x00000008
	.section .rom.000ad1a8, "ax"
	.global Trade_CountPendingOffersFar
	.type Trade_CountPendingOffersFar, %function
	.thumb_func
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000ad1a8, 0x00000008
	.section .rom.000ad1b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000ad1b0, 0x00000008
	.section .rom.000ad1b8, "ax"
	.global Trade_CanOfferDjinnFar
	.type Trade_CanOfferDjinnFar, %function
	.thumb_func
Trade_CanOfferDjinnFar:
	.incbin "baserom.gba", 0x000ad1b8, 0x00000008
	.section .rom.000ad1c0, "ax"
	.global Item_CanOwnerEquip
	.type Item_CanOwnerEquip, %function
	.thumb_func
Item_CanOwnerEquip:
	.incbin "baserom.gba", 0x000ad1c0, 0x00000008
	.section .rom.000ad1c8, "ax"
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000ad1c8, 0x00000008
	.section .rom.000ad1d0, "ax"
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000ad1d0, 0x00000008
	.section .rom.000ad1d8, "ax"
	.global Party_AdjustCoinsFar
	.type Party_AdjustCoinsFar, %function
	.thumb_func
Party_AdjustCoinsFar:
	.incbin "baserom.gba", 0x000ad1d8, 0x00000008
	.section .rom.000ad1e0, "ax"
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000ad1e0, 0x00000008
	.section .rom.000ad1e8, "ax"
	.global Item_AdjustCounterFar
	.type Item_AdjustCounterFar, %function
	.thumb_func
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000ad1e8, 0x00000008
	.section .rom.000ad1f0, "ax"
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x000ad1f0, 0x00000008
	.section .rom.000ad1f8, "ax"
	.global PartyInventory_HasSpaceFar
	.type PartyInventory_HasSpaceFar, %function
	.thumb_func
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000ad1f8, 0x00000008
	.section .rom.000ad200, "ax"
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000ad200, 0x00000008
	.section .rom.000ad208, "ax"
	.global Func_080ad208
	.type Func_080ad208, %function
	.thumb_func
Func_080ad208:
	.incbin "baserom.gba", 0x000ad208, 0x00000008
	.section .rom.000ad210, "ax"
	.global Func_080ad210
	.type Func_080ad210, %function
	.thumb_func
Func_080ad210:
	.incbin "baserom.gba", 0x000ad210, 0x00000008
	.section .rom.000ad218, "ax"
	.global Party_SetLeaderJasmineFar
	.type Party_SetLeaderJasmineFar, %function
	.thumb_func
Party_SetLeaderJasmineFar:
	.incbin "baserom.gba", 0x000ad218, 0x00000008
	.section .rom.000ad220, "ax"
	.global Party_SetLeaderGarciaFar
	.type Party_SetLeaderGarciaFar, %function
	.thumb_func
Party_SetLeaderGarciaFar:
	.incbin "baserom.gba", 0x000ad220, 0x00000008
	.section .rom.000ad228, "ax"
	.global Func_080ad228
	.type Func_080ad228, %function
	.thumb_func
Func_080ad228:
	.incbin "baserom.gba", 0x000ad228, 0x00000008
	.section .rom.000ad230, "ax"
	.global Func_080ad230
	.type Func_080ad230, %function
	.thumb_func
Func_080ad230:
	.incbin "baserom.gba", 0x000ad230, 0x00000008
	.section .rom.000ad238, "ax"
	.global Func_080ad238
	.type Func_080ad238, %function
	.thumb_func
Func_080ad238:
	.incbin "baserom.gba", 0x000ad238, 0x00000008
	.section .rom.000ad240, "ax"
	.global Func_080ad240
	.type Func_080ad240, %function
	.thumb_func
Func_080ad240:
	.incbin "baserom.gba", 0x000ad240, 0x00000008
	.section .rom.000ad248, "ax"
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000ad248, 0x00000008
	.section .rom.000ad250, "ax"
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000ad250, 0x00000008
	.section .rom.000ad258, "ax"
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000ad258, 0x00000008
	.section .rom.000ad260, "ax"
	.global Party_AdjustCounterCappedAt28Far
	.type Party_AdjustCounterCappedAt28Far, %function
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000ad260, 0x00000008
	.section .rom.000ad268, "ax"
	.global Func_080ad268
	.type Func_080ad268, %function
	.thumb_func
Func_080ad268:
	.incbin "baserom.gba", 0x000ad268, 0x00000008
	.section .rom.000ad270, "ax"
	.global Func_080ad270
	.type Func_080ad270, %function
	.thumb_func
Func_080ad270:
	.incbin "baserom.gba", 0x000ad270, 0x00000008
	.section .rom.000ad278, "ax"
	.global Inventory_DiscardFar
	.type Inventory_DiscardFar, %function
	.thumb_func
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000ad278, 0x00000008
	.section .rom.000ad280, "ax"
	.global BtlEfxRevChkFar
	.type BtlEfxRevChkFar, %function
	.thumb_func
BtlEfxRevChkFar:
	.incbin "baserom.gba", 0x000ad280, 0x00000008
	.section .rom.000ad288, "ax"
	.global Owner_RefreshClassActionsFar
	.type Owner_RefreshClassActionsFar, %function
	.thumb_func
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000ad288, 0x00000008
	.section .rom.000ad290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000ad290, 0x00000008
	.section .rom.000ad298, "ax"
	.global Owner_RefreshDerivedDataFar
	.type Owner_RefreshDerivedDataFar, %function
	.thumb_func
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000ad298, 0x00000008
	.section .rom.000ad2a0, "ax"
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000ad2a0, 0x00000008
	.section .rom.000ad2a8, "ax"
	.global PartyInventory_CountItemFar
	.type PartyInventory_CountItemFar, %function
	.thumb_func
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000ad2a8, 0x00000008
	.section .rom.000ad2b0, "ax"
	.global PartyInventory_DiscardFar
	.type PartyInventory_DiscardFar, %function
	.thumb_func
PartyInventory_DiscardFar:
	.incbin "baserom.gba", 0x000ad2b0, 0x00000008
	.section .rom.000ad3f6, "ax"
	.incbin "baserom.gba", 0x000ad3f6, 0x00000002
	.section .rom.000ad3f8, "ax"
	.global UnitCalc
	.type UnitCalc, %function
	.thumb_func
UnitCalc:
	.incbin "baserom.gba", 0x000ad3f8, 0x000007f4
	.section .rom.000adbec, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000adbec, 0x000000a4
	.section .rom.000addf0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000addf0, 0x000002ec
	.section .rom.000ae358, "ax"
	.global Func_080ae358
	.type Func_080ae358, %function
	.thumb_func
Func_080ae358:
	.incbin "baserom.gba", 0x000ae358, 0x000000a4
	.section .rom.000ae410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000ae410, 0x000001ec
	.section .rom.000ae5fc, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000ae5fc, 0x000000cc
	.section .rom.000ae6e4, "ax"
	.global Func_080ae6e4
	.type Func_080ae6e4, %function
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000ae6e4, 0x00000158
	.section .rom.000ae874, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000ae874, 0x000001c8
	.section .rom.000aec72, "ax"
	.incbin "baserom.gba", 0x000aec72, 0x00000002
	.section .rom.000aec74, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000aec74, 0x0000003c
	.section .rom.000aef40, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x000aef40, 0x000000d4
	.section .rom.000af0ee, "ax"
	.incbin "baserom.gba", 0x000af0ee, 0x00000002
	.section .rom.000af0f0, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.incbin "baserom.gba", 0x000af0f0, 0x00000064
	.section .rom.000af154, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000af154, 0x00000080
	.section .rom.000af4f0, "ax"
	.balign 4
	.global Owner_UpdateClassActions
	.type Owner_UpdateClassActions, %function
	.thumb_func
Owner_UpdateClassActions:
	.incbin "baserom.gba", 0x000af4f0, 0x00000210
	.section .rom.000af7b8, "ax"
	.incbin "baserom.gba", 0x000af7b8, 0x00000120
	.section .rom.000af928, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000af928, 0x00000264
	.section .rom.000afb8c, "ax"
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000afb8c, 0x00000034
	.section .rom.000afbf8, "ax"
	.incbin "baserom.gba", 0x000afbf8, 0x000001d0
	.section .rom.000aff34, "ax"
	.incbin "baserom.gba", 0x000aff34, 0x00000054
	.section .rom.000affb8, "ax"
	.balign 4
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000affb8, 0x0000007c
	.section .rom.000b0066, "ax"
	.incbin "baserom.gba", 0x000b0066, 0x00000002
	.section .rom.000b0068, "ax"
	.global Owner_GetDefaultElement
	.type Owner_GetDefaultElement, %function
	.thumb_func
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000b0068, 0x00000028
	.section .rom.000b0090, "ax"
	.global Owner_CalculateBaseElements
	.type Owner_CalculateBaseElements, %function
	.thumb_func
Owner_CalculateBaseElements:
	.incbin "baserom.gba", 0x000b0090, 0x000000c0
	.section .rom.000b0150, "ax"
	.global Owner_DetermineClass
	.type Owner_DetermineClass, %function
	.thumb_func
Owner_DetermineClass:
	.incbin "baserom.gba", 0x000b0150, 0x00000144
	.section .rom.000b0520, "ax"
	.incbin "baserom.gba", 0x000b0520, 0x00000194
	.section .rom.000b0768, "ax"
	.align 2, 0
	.incbin "baserom.gba", 0x000b0768, 0x00000338
	.section .rom.000b0bc2, "ax"
	.incbin "baserom.gba", 0x000b0bc2, 0x00000002
	.section .rom.000b0bc4, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000b0bc4, 0x000000c0
	.section .rom.000b0d64, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000b0d64, 0x000000ac
	.section .rom.000b1094, "ax"
	.balign 4
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000b1094, 0x00000170
	.section .rom.000b1204, "ax"
	.global Func_080b11f8
	.type Func_080b11f8, %function
	.thumb_func
Func_080b11f8:
	.incbin "baserom.gba", 0x000b1204, 0x00000084
	.section .rom.000b1288, "ax"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000b1288, 0x0000004c
	.section .rom.000b1f34, "ax"
	.incbin "baserom.gba", 0x000b1f34, 0x00000004
	.section .rom.000c6b10, "ax"
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000c6b10, 0x00000014
	.section .rom.000c6b24, "ax"
	.global Battle_Effect90Classes
Battle_Effect90Classes:
	.incbin "baserom.gba", 0x000c6b24, 0x0000005e
	.section .rom.000c6b82, "ax"
	.global Battle_Effect91Classes
Battle_Effect91Classes:
	.incbin "baserom.gba", 0x000c6b82, 0x0000003a
	.section .rom.000c8000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000c8000, 0x00000008
	.section .rom.000c8008, "ax"
	.global Game_ResetForNewGameFar
	.type Game_ResetForNewGameFar, %function
	.thumb_func
Game_ResetForNewGameFar:
	.incbin "baserom.gba", 0x000c8008, 0x00000008
	.section .rom.000c8010, "ax"
	.global Battle_WaitMode0Far
	.type Battle_WaitMode0Far, %function
	.thumb_func
Battle_WaitMode0Far:
	.incbin "baserom.gba", 0x000c8010, 0x00000008
	.section .rom.000c8018, "ax"
	.global EventRuntime_BeginFar
	.type EventRuntime_BeginFar, %function
	.thumb_func
EventRuntime_BeginFar:
	.incbin "baserom.gba", 0x000c8018, 0x00000008
	.section .rom.000c8020, "ax"
	.global EventRuntime_EndFar
	.type EventRuntime_EndFar, %function
	.thumb_func
EventRuntime_EndFar:
	.incbin "baserom.gba", 0x000c8020, 0x00000008
	.section .rom.000c8028, "ax"
	.global Event_RunObjectHookAndWaitFar
	.type Event_RunObjectHookAndWaitFar, %function
	.thumb_func
Event_RunObjectHookAndWaitFar:
	.incbin "baserom.gba", 0x000c8028, 0x00000008
	.section .rom.000c8030, "ax"
	.global Event_CallWithLastActiveObjectIdFar
	.type Event_CallWithLastActiveObjectIdFar, %function
	.thumb_func
Event_CallWithLastActiveObjectIdFar:
	.incbin "baserom.gba", 0x000c8030, 0x00000008
	.section .rom.000c8038, "ax"
	.global Scene_AssignViewFlagsFar
	.type Scene_AssignViewFlagsFar, %function
	.thumb_func
Scene_AssignViewFlagsFar:
	.incbin "baserom.gba", 0x000c8038, 0x00000008
	.section .rom.000c8040, "ax"
	.global Event_SetSceneEventsFar
	.type Event_SetSceneEventsFar, %function
	.thumb_func
Event_SetSceneEventsFar:
	.incbin "baserom.gba", 0x000c8040, 0x00000008
	.section .rom.000c8048, "ax"
	.global Event_PrepareObjectAndApplyValueFar
	.type Event_PrepareObjectAndApplyValueFar, %function
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	.incbin "baserom.gba", 0x000c8048, 0x00000008
	.section .rom.000c8050, "ax"
	.global Event_PrepareTwoObjectsAndApplyFar
	.type Event_PrepareTwoObjectsAndApplyFar, %function
	.thumb_func
Event_PrepareTwoObjectsAndApplyFar:
	.incbin "baserom.gba", 0x000c8050, 0x00000008
	.section .rom.000c8058, "ax"
	.global Party_RemoveOwnerRestoredFar
	.type Party_RemoveOwnerRestoredFar, %function
	.thumb_func
Party_RemoveOwnerRestoredFar:
	.incbin "baserom.gba", 0x000c8058, 0x00000008
	.section .rom.000c8060, "ax"
	.global PartyInventory_GiveItemFar
	.type PartyInventory_GiveItemFar, %function
	.thumb_func
PartyInventory_GiveItemFar:
	.incbin "baserom.gba", 0x000c8060, 0x00000008
	.section .rom.000c8068, "ax"
	.global Inventory_TryAddAndReturnOwnerFar
	.type Inventory_TryAddAndReturnOwnerFar, %function
	.thumb_func
Inventory_TryAddAndReturnOwnerFar:
	.incbin "baserom.gba", 0x000c8068, 0x00000008
	.section .rom.000c8070, "ax"
	.global Inventory_PromptAndSetObjectModeFar
	.type Inventory_PromptAndSetObjectModeFar, %function
	.thumb_func
Inventory_PromptAndSetObjectModeFar:
	.incbin "baserom.gba", 0x000c8070, 0x00000008
	.section .rom.000c8078, "ax"
	.global Func_080c8078
	.type Func_080c8078, %function
	.thumb_func
Func_080c8078:
	.incbin "baserom.gba", 0x000c8078, 0x00000008
	.section .rom.000c8080, "ax"
	.global UiText_DrawQuantityPairWithCueFar
	.type UiText_DrawQuantityPairWithCueFar, %function
	.thumb_func
UiText_DrawQuantityPairWithCueFar:
	.incbin "baserom.gba", 0x000c8080, 0x00000008
	.section .rom.000c8088, "ax"
	.global Object_GetByIdFar
	.type Object_GetByIdFar, %function
	.thumb_func
Object_GetByIdFar:
	.incbin "baserom.gba", 0x000c8088, 0x00000008
	.section .rom.000c8090, "ax"
	.global ObjectTable_DestroyByIdFar
	.type ObjectTable_DestroyByIdFar, %function
	.thumb_func
ObjectTable_DestroyByIdFar:
	.incbin "baserom.gba", 0x000c8090, 0x00000008
	.section .rom.000c8098, "ax"
	.global ObjectMotion_SetSpeedParametersFar
	.type ObjectMotion_SetSpeedParametersFar, %function
	.thumb_func
ObjectMotion_SetSpeedParametersFar:
	.incbin "baserom.gba", 0x000c8098, 0x00000008
	.section .rom.000c80a0, "ax"
	.global ObjectMotion_EnableActionAndSetCallbackFar
	.type ObjectMotion_EnableActionAndSetCallbackFar, %function
	.thumb_func
ObjectMotion_EnableActionAndSetCallbackFar:
	.incbin "baserom.gba", 0x000c80a0, 0x00000008
	.section .rom.000c80a8, "ax"
	.global Object_RefreshSelectorByIdFar
	.type Object_RefreshSelectorByIdFar, %function
	.thumb_func
Object_RefreshSelectorByIdFar:
	.incbin "baserom.gba", 0x000c80a8, 0x00000008
	.section .rom.000c80b0, "ax"
	.global ObjectMotion_EnableActionAndResetMotionFar
	.type ObjectMotion_EnableActionAndResetMotionFar, %function
	.thumb_func
ObjectMotion_EnableActionAndResetMotionFar:
	.incbin "baserom.gba", 0x000c80b0, 0x00000008
	.section .rom.000c80b8, "ax"
	.global Object_SetActionCallbackAndRefreshByIdFar
	.type Object_SetActionCallbackAndRefreshByIdFar, %function
	.thumb_func
Object_SetActionCallbackAndRefreshByIdFar:
	.incbin "baserom.gba", 0x000c80b8, 0x00000008
	.section .rom.000c80c0, "ax"
	.global ObjectMotion_ResetAndSetPositionFar
	.type ObjectMotion_ResetAndSetPositionFar, %function
	.thumb_func
ObjectMotion_ResetAndSetPositionFar:
	.incbin "baserom.gba", 0x000c80c0, 0x00000008
	.section .rom.000c80c8, "ax"
	.global ObjectMotion_SetPositionAndCommitFar
	.type ObjectMotion_SetPositionAndCommitFar, %function
	.thumb_func
ObjectMotion_SetPositionAndCommitFar:
	.incbin "baserom.gba", 0x000c80c8, 0x00000008
	.section .rom.000c80d0, "ax"
	.global ObjectMotion_ResetAndSetPositionInMode2Far
	.type ObjectMotion_ResetAndSetPositionInMode2Far, %function
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2Far:
	.incbin "baserom.gba", 0x000c80d0, 0x00000008
	.section .rom.000c80d8, "ax"
	.global ObjectMotion_SetPositionAndResetFar
	.type ObjectMotion_SetPositionAndResetFar, %function
	.thumb_func
ObjectMotion_SetPositionAndResetFar:
	.incbin "baserom.gba", 0x000c80d8, 0x00000008
	.section .rom.000c80e0, "ax"
	.global ObjectMotion_CenterOnColumnFar
	.type ObjectMotion_CenterOnColumnFar, %function
	.thumb_func
ObjectMotion_CenterOnColumnFar:
	.incbin "baserom.gba", 0x000c80e0, 0x00000008
	.section .rom.000c80e8, "ax"
	.global ObjectMotion_OffsetPositionAndResetMotionFar
	.type ObjectMotion_OffsetPositionAndResetMotionFar, %function
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotionFar:
	.incbin "baserom.gba", 0x000c80e8, 0x00000008
	.section .rom.000c80f0, "ax"
	.global ObjectMotion_CommitCurrentPositionAndActivateFar
	.type ObjectMotion_CommitCurrentPositionAndActivateFar, %function
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivateFar:
	.incbin "baserom.gba", 0x000c80f0, 0x00000008
	.section .rom.000c80f8, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrainFar
	.type ObjectMotion_SetHorizontalPositionWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrainFar:
	.incbin "baserom.gba", 0x000c80f8, 0x00000008
	.section .rom.000c8100, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar:
	.incbin "baserom.gba", 0x000c8100, 0x00000008
	.section .rom.000c8108, "ax"
	.global ObjectMotion_SetPositionWithTerrainFar
	.type ObjectMotion_SetPositionWithTerrainFar, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrainFar:
	.incbin "baserom.gba", 0x000c8108, 0x00000008
	.section .rom.000c8110, "ax"
	.global ObjectMotion_MoveToObjectFar
	.type ObjectMotion_MoveToObjectFar, %function
	.thumb_func
ObjectMotion_MoveToObjectFar:
	.incbin "baserom.gba", 0x000c8110, 0x00000008
	.section .rom.000c8118, "ax"
	.global Object_SetModeByIdFar
	.type Object_SetModeByIdFar, %function
	.thumb_func
Object_SetModeByIdFar:
	.incbin "baserom.gba", 0x000c8118, 0x00000008
	.section .rom.000c8120, "ax"
	.global Object_SetActionByIdFar
	.type Object_SetActionByIdFar, %function
	.thumb_func
Object_SetActionByIdFar:
	.incbin "baserom.gba", 0x000c8120, 0x00000008
	.section .rom.000c8128, "ax"
	.global Motion_SetModeAndWaitAnimationFar
	.type Motion_SetModeAndWaitAnimationFar, %function
	.thumb_func
Motion_SetModeAndWaitAnimationFar:
	.incbin "baserom.gba", 0x000c8128, 0x00000008
	.section .rom.000c8130, "ax"
	.global ObjectMotion_WaitForAnimationChangeFar
	.type ObjectMotion_WaitForAnimationChangeFar, %function
	.thumb_func
ObjectMotion_WaitForAnimationChangeFar:
	.incbin "baserom.gba", 0x000c8130, 0x00000008
	.section .rom.000c8138, "ax"
	.global ObjectMotion_LaunchFar
	.type ObjectMotion_LaunchFar, %function
	.thumb_func
ObjectMotion_LaunchFar:
	.incbin "baserom.gba", 0x000c8138, 0x00000008
	.section .rom.000c8140, "ax"
	.global ObjectMotion_SetVariantCallbackFar
	.type ObjectMotion_SetVariantCallbackFar, %function
	.thumb_func
ObjectMotion_SetVariantCallbackFar:
	.incbin "baserom.gba", 0x000c8140, 0x00000008
	.section .rom.000c8148, "ax"
	.global Motion_SetVarCbAndRefreshFar
	.type Motion_SetVarCbAndRefreshFar, %function
	.thumb_func
Motion_SetVarCbAndRefreshFar:
	.incbin "baserom.gba", 0x000c8148, 0x00000008
	.section .rom.000c8150, "ax"
	.global BattleFx_RunRisingObjectSequenceFar
	.type BattleFx_RunRisingObjectSequenceFar, %function
	.thumb_func
BattleFx_RunRisingObjectSequenceFar:
	.incbin "baserom.gba", 0x000c8150, 0x00000008
	.section .rom.000c8158, "ax"
	.global ObjectMotion_SetAngleTowardFar
	.type ObjectMotion_SetAngleTowardFar, %function
	.thumb_func
ObjectMotion_SetAngleTowardFar:
	.incbin "baserom.gba", 0x000c8158, 0x00000008
	.section .rom.000c8160, "ax"
	.global Object_LinkPairFar
	.type Object_LinkPairFar, %function
	.thumb_func
Object_LinkPairFar:
	.incbin "baserom.gba", 0x000c8160, 0x00000008
	.section .rom.000c8168, "ax"
	.global ObjectGroup_ConfigureChildValueFar
	.type ObjectGroup_ConfigureChildValueFar, %function
	.thumb_func
ObjectGroup_ConfigureChildValueFar:
	.incbin "baserom.gba", 0x000c8168, 0x00000008
	.section .rom.000c8170, "ax"
	.global Object_SetPartAttributeFar
	.type Object_SetPartAttributeFar, %function
	.thumb_func
Object_SetPartAttributeFar:
	.incbin "baserom.gba", 0x000c8170, 0x00000008
	.section .rom.000c8178, "ax"
	.global Func_080c8178
	.type Func_080c8178, %function
	.thumb_func
Func_080c8178:
	.incbin "baserom.gba", 0x000c8178, 0x00000008
	.section .rom.000c8180, "ax"
	.global EventRuntime_SetMessageFar
	.type EventRuntime_SetMessageFar, %function
	.thumb_func
EventRuntime_SetMessageFar:
	.incbin "baserom.gba", 0x000c8180, 0x00000008
	.section .rom.000c8188, "ax"
	.global UiText_OpenMessageAtObjectFar
	.type UiText_OpenMessageAtObjectFar, %function
	.thumb_func
UiText_OpenMessageAtObjectFar:
	.incbin "baserom.gba", 0x000c8188, 0x00000008
	.section .rom.000c8190, "ax"
	.global Func_080c8190
	.type Func_080c8190, %function
	.thumb_func
Func_080c8190:
	.incbin "baserom.gba", 0x000c8190, 0x00000008
	.section .rom.000c8198, "ax"
	.global Func_080c8198
	.type Func_080c8198, %function
	.thumb_func
Func_080c8198:
	.incbin "baserom.gba", 0x000c8198, 0x00000008
	.section .rom.000c81a0, "ax"
	.global EventRuntime_ShowMessageAndWaitFar
	.type EventRuntime_ShowMessageAndWaitFar, %function
	.thumb_func
EventRuntime_ShowMessageAndWaitFar:
	.incbin "baserom.gba", 0x000c81a0, 0x00000008
	.section .rom.000c81a8, "ax"
	.global Func_080c81a8
	.type Func_080c81a8, %function
	.thumb_func
Func_080c81a8:
	.incbin "baserom.gba", 0x000c81a8, 0x00000008
	.section .rom.000c81b0, "ax"
	.global Func_080c81b0
	.type Func_080c81b0, %function
	.thumb_func
Func_080c81b0:
	.incbin "baserom.gba", 0x000c81b0, 0x00000008
	.section .rom.000c81b8, "ax"
	.global Func_080c81b8
	.type Func_080c81b8, %function
	.thumb_func
Func_080c81b8:
	.incbin "baserom.gba", 0x000c81b8, 0x00000008
	.section .rom.000c81c0, "ax"
	.global Func_080c81c0
	.type Func_080c81c0, %function
	.thumb_func
Func_080c81c0:
	.incbin "baserom.gba", 0x000c81c0, 0x00000008
	.section .rom.000c81c8, "ax"
	.global Event_ShowCounterAtPositionFar
	.type Event_ShowCounterAtPositionFar, %function
	.thumb_func
Event_ShowCounterAtPositionFar:
	.incbin "baserom.gba", 0x000c81c8, 0x00000008
	.section .rom.000c81d0, "ax"
	.global ObjectMotion_ArmCallbackFar
	.type ObjectMotion_ArmCallbackFar, %function
	.thumb_func
ObjectMotion_ArmCallbackFar:
	.incbin "baserom.gba", 0x000c81d0, 0x00000008
	.section .rom.000c81d8, "ax"
	.global ObjectMotion_ArmCallbackAndRefreshFar
	.type ObjectMotion_ArmCallbackAndRefreshFar, %function
	.thumb_func
ObjectMotion_ArmCallbackAndRefreshFar:
	.incbin "baserom.gba", 0x000c81d8, 0x00000008
	.section .rom.000c81e0, "ax"
	.global Object_ResetTargetAndSetMode1Far
	.type Object_ResetTargetAndSetMode1Far, %function
	.thumb_func
Object_ResetTargetAndSetMode1Far:
	.incbin "baserom.gba", 0x000c81e0, 0x00000008
	.section .rom.000c81e8, "ax"
	.global ObjectTable_RunIfActiveFar
	.type ObjectTable_RunIfActiveFar, %function
	.thumb_func
ObjectTable_RunIfActiveFar:
	.incbin "baserom.gba", 0x000c81e8, 0x00000008
	.section .rom.000c81f0, "ax"
	.global Battle_ShowPairedUnitWorkAndWaitFar
	.type Battle_ShowPairedUnitWorkAndWaitFar, %function
	.thumb_func
Battle_ShowPairedUnitWorkAndWaitFar:
	.incbin "baserom.gba", 0x000c81f0, 0x00000008
	.section .rom.000c81f8, "ax"
	.global Ui_SetRenderResultFromObjectFar
	.type Ui_SetRenderResultFromObjectFar, %function
	.thumb_func
Ui_SetRenderResultFromObjectFar:
	.incbin "baserom.gba", 0x000c81f8, 0x00000008
	.section .rom.000c8200, "ax"
	.global ObjectMotion_SetActionVariantFar
	.type ObjectMotion_SetActionVariantFar, %function
	.thumb_func
ObjectMotion_SetActionVariantFar:
	.incbin "baserom.gba", 0x000c8200, 0x00000008
	.section .rom.000c8208, "ax"
	.global Func_080c8208
	.type Func_080c8208, %function
	.thumb_func
Func_080c8208:
	.incbin "baserom.gba", 0x000c8208, 0x00000008
	.section .rom.000c8210, "ax"
	.global Func_080c8210
	.type Func_080c8210, %function
	.thumb_func
Func_080c8210:
	.incbin "baserom.gba", 0x000c8210, 0x00000008
	.section .rom.000c8218, "ax"
	.global Func_080c8218
	.type Func_080c8218, %function
	.thumb_func
Func_080c8218:
	.incbin "baserom.gba", 0x000c8218, 0x00000008
	.section .rom.000c8220, "ax"
	.global ObjectVisual_CopyAttributesFar
	.type ObjectVisual_CopyAttributesFar, %function
	.thumb_func
ObjectVisual_CopyAttributesFar:
	.incbin "baserom.gba", 0x000c8220, 0x00000008
	.section .rom.000c8228, "ax"
	.global Object_AttachWorkTargetToObjectFar
	.type Object_AttachWorkTargetToObjectFar, %function
	.thumb_func
Object_AttachWorkTargetToObjectFar:
	.incbin "baserom.gba", 0x000c8228, 0x00000008
	.section .rom.000c8230, "ax"
	.global ObjectTable_AllocateAndSetObjectSpeedFar
	.type ObjectTable_AllocateAndSetObjectSpeedFar, %function
	.thumb_func
ObjectTable_AllocateAndSetObjectSpeedFar:
	.incbin "baserom.gba", 0x000c8230, 0x00000008
	.section .rom.000c8238, "ax"
	.global Motion_CamBoundsFar
	.type Motion_CamBoundsFar, %function
	.thumb_func
Motion_CamBoundsFar:
	.incbin "baserom.gba", 0x000c8238, 0x00000008
	.section .rom.000c8240, "ax"
	.global Func_080c8240
	.type Func_080c8240, %function
	.thumb_func
Func_080c8240:
	.incbin "baserom.gba", 0x000c8240, 0x00000008
	.section .rom.000c8248, "ax"
	.global Func_080c8248
	.type Func_080c8248, %function
	.thumb_func
Func_080c8248:
	.incbin "baserom.gba", 0x000c8248, 0x00000008
	.section .rom.000c8250, "ax"
	.global Func_080c8250
	.type Func_080c8250, %function
	.thumb_func
Func_080c8250:
	.incbin "baserom.gba", 0x000c8250, 0x00000008
	.section .rom.000c8258, "ax"
	.global Func_080c8258
	.type Func_080c8258, %function
	.thumb_func
Func_080c8258:
	.incbin "baserom.gba", 0x000c8258, 0x00000008
	.section .rom.000c8260, "ax"
	.global Func_080c8260
	.type Func_080c8260, %function
	.thumb_func
Func_080c8260:
	.incbin "baserom.gba", 0x000c8260, 0x00000008
	.section .rom.000c82a8, "ax"
	.incbin "baserom.gba", 0x000c82a8, 0x00000010
	.section .rom.000c83b8, "ax"
	.global Event_DelayEffectFramesFar
	.type Event_DelayEffectFramesFar, %function
	.thumb_func
Event_DelayEffectFramesFar:
	.incbin "baserom.gba", 0x000c83b8, 0x00000008
	.section .rom.000c83c0, "ax"
	.global Event_FinishScreenTransitionFar
	.type Event_FinishScreenTransitionFar, %function
	.thumb_func
Event_FinishScreenTransitionFar:
	.incbin "baserom.gba", 0x000c83c0, 0x00000008
	.section .rom.000c83c8, "ax"
	.global Event_WaitTransitionFramesFar
	.type Event_WaitTransitionFramesFar, %function
	.thumb_func
Event_WaitTransitionFramesFar:
	.incbin "baserom.gba", 0x000c83c8, 0x00000008
	.section .rom.000c83d0, "ax"
	.global DisplayScroll_InitializeHBlankDmaFar
	.type DisplayScroll_InitializeHBlankDmaFar, %function
	.thumb_func
DisplayScroll_InitializeHBlankDmaFar:
	.incbin "baserom.gba", 0x000c83d0, 0x00000008
	.section .rom.000c83d8, "ax"
	.global DisplayScroll_DisarmHBlankDmaFar
	.type DisplayScroll_DisarmHBlankDmaFar, %function
	.thumb_func
DisplayScroll_DisarmHBlankDmaFar:
	.incbin "baserom.gba", 0x000c83d8, 0x00000008
	.section .rom.000c83e0, "ax"
	.global BattleFx_PlayCueAndStartEmitterOnTargetFar
	.type BattleFx_PlayCueAndStartEmitterOnTargetFar, %function
	.thumb_func
BattleFx_PlayCueAndStartEmitterOnTargetFar:
	.incbin "baserom.gba", 0x000c83e0, 0x00000008
	.section .rom.000c83e8, "ax"
	.global Func_080c83d8
	.type Func_080c83d8, %function
	.thumb_func
Func_080c83d8:
	.incbin "baserom.gba", 0x000c83e8, 0x00000008
	.section .rom.000c83f0, "ax"
	.global Func_080c83e0
	.type Func_080c83e0, %function
	.thumb_func
Func_080c83e0:
	.incbin "baserom.gba", 0x000c83f0, 0x00000008
	.section .rom.000c83f8, "ax"
	.global ObjectEvent_GetObjectFar
	.type ObjectEvent_GetObjectFar, %function
	.thumb_func
ObjectEvent_GetObjectFar:
	.incbin "baserom.gba", 0x000c83f8, 0x00000008
	.section .rom.000c8400, "ax"
	.global ObjectEvent_SetMode4WithCueFar
	.type ObjectEvent_SetMode4WithCueFar, %function
	.thumb_func
ObjectEvent_SetMode4WithCueFar:
	.incbin "baserom.gba", 0x000c8400, 0x00000008
	.section .rom.000c8408, "ax"
	.global ObjectEvent_SetMode5WithCueFar
	.type ObjectEvent_SetMode5WithCueFar, %function
	.thumb_func
ObjectEvent_SetMode5WithCueFar:
	.incbin "baserom.gba", 0x000c8408, 0x00000008
	.section .rom.000c8410, "ax"
	.global ObjectEvent_SetMode2Far
	.type ObjectEvent_SetMode2Far, %function
	.thumb_func
ObjectEvent_SetMode2Far:
	.incbin "baserom.gba", 0x000c8410, 0x00000008
	.section .rom.000c8418, "ax"
	.global ObjectEvent_PlaceOnGroundFar
	.type ObjectEvent_PlaceOnGroundFar, %function
	.thumb_func
ObjectEvent_PlaceOnGroundFar:
	.incbin "baserom.gba", 0x000c8418, 0x00000008
	.section .rom.000c8420, "ax"
	.global ObjectEvent_RemoveFar
	.type ObjectEvent_RemoveFar, %function
	.thumb_func
ObjectEvent_RemoveFar:
	.incbin "baserom.gba", 0x000c8420, 0x00000008
	.section .rom.000c8428, "ax"
	.global Func_080c8418
	.type Func_080c8418, %function
	.thumb_func
Func_080c8418:
	.incbin "baserom.gba", 0x000c8428, 0x00000008
	.section .rom.000c8430, "ax"
	.global Object_SetWideSpriteFar
	.type Object_SetWideSpriteFar, %function
	.thumb_func
Object_SetWideSpriteFar:
	.incbin "baserom.gba", 0x000c8430, 0x00000008
	.section .rom.000c8438, "ax"
	.global Object_ReturnHomeFar
	.type Object_ReturnHomeFar, %function
	.thumb_func
Object_ReturnHomeFar:
	.incbin "baserom.gba", 0x000c8438, 0x00000008
	.section .rom.000c8440, "ax"
	.global Func_080c8430
	.type Func_080c8430, %function
	.thumb_func
Func_080c8430:
	.incbin "baserom.gba", 0x000c8440, 0x00000008
	.section .rom.000c8448, "ax"
	.global Func_080c8438
	.type Func_080c8438, %function
	.thumb_func
Func_080c8438:
	.incbin "baserom.gba", 0x000c8448, 0x00000008
	.section .rom.000c8450, "ax"
	.global Func_080c8440
	.type Func_080c8440, %function
	.thumb_func
Func_080c8440:
	.incbin "baserom.gba", 0x000c8450, 0x00000008
	.section .rom.000c8458, "ax"
	.global Event_SpawnObjectTableFar
	.type Event_SpawnObjectTableFar, %function
	.thumb_func
Event_SpawnObjectTableFar:
	.incbin "baserom.gba", 0x000c8458, 0x00000008
	.section .rom.000c8460, "ax"
	.global ObjectTable_DestroyAtIndexFar
	.type ObjectTable_DestroyAtIndexFar, %function
	.thumb_func
ObjectTable_DestroyAtIndexFar:
	.incbin "baserom.gba", 0x000c8460, 0x00000008
	.section .rom.000c8468, "ax"
	.global ObjectTable_GetFar
	.type ObjectTable_GetFar, %function
	.thumb_func
ObjectTable_GetFar:
	.incbin "baserom.gba", 0x000c8468, 0x00000008
	.section .rom.000c8470, "ax"
	.global Func_080c8460
	.type Func_080c8460, %function
	.thumb_func
Func_080c8460:
	.incbin "baserom.gba", 0x000c8470, 0x00000008
	.section .rom.000c8478, "ax"
	.global Func_080c8468
	.type Func_080c8468, %function
	.thumb_func
Func_080c8468:
	.incbin "baserom.gba", 0x000c8478, 0x00000008
	.section .rom.000c8480, "ax"
	.global Func_080c8470
	.type Func_080c8470, %function
	.thumb_func
Func_080c8470:
	.incbin "baserom.gba", 0x000c8480, 0x00000008
	.section .rom.000c8488, "ax"
	.global Func_080c8478
	.type Func_080c8478, %function
	.thumb_func
Func_080c8478:
	.incbin "baserom.gba", 0x000c8488, 0x00000008
	.section .rom.000c8490, "ax"
	.global Func_080c8480
	.type Func_080c8480, %function
	.thumb_func
Func_080c8480:
	.incbin "baserom.gba", 0x000c8490, 0x00000008
	.section .rom.000c8498, "ax"
	.global Func_080c8488
	.type Func_080c8488, %function
	.thumb_func
Func_080c8488:
	.incbin "baserom.gba", 0x000c8498, 0x00000008
	.section .rom.000c84a0, "ax"
	.global Func_080c8490
	.type Func_080c8490, %function
	.thumb_func
Func_080c8490:
	.incbin "baserom.gba", 0x000c84a0, 0x00000008
	.section .rom.000c84a8, "ax"
	.global Field_DispatchTypeHandlerFar
	.type Field_DispatchTypeHandlerFar, %function
	.thumb_func
Field_DispatchTypeHandlerFar:
	.incbin "baserom.gba", 0x000c84a8, 0x00000008
	.section .rom.000c84b0, "ax"
	.global BattleFx_LoadActionEffectResourcesFar
	.type BattleFx_LoadActionEffectResourcesFar, %function
	.thumb_func
BattleFx_LoadActionEffectResourcesFar:
	.incbin "baserom.gba", 0x000c84b0, 0x00000008
	.section .rom.000c84b8, "ax"
	.global BattleFx_SetupObjectPairFar
	.type BattleFx_SetupObjectPairFar, %function
	.thumb_func
BattleFx_SetupObjectPairFar:
	.incbin "baserom.gba", 0x000c84b8, 0x00000008
	.section .rom.000c84c0, "ax"
	.global EffectRuntime_StopCurrentObjectFar
	.type EffectRuntime_StopCurrentObjectFar, %function
	.thumb_func
EffectRuntime_StopCurrentObjectFar:
	.incbin "baserom.gba", 0x000c84c0, 0x00000008
	.section .rom.000c84c8, "ax"
	.global BattleEffect_CleanupSceneObjectsFar
	.type BattleEffect_CleanupSceneObjectsFar, %function
	.thumb_func
BattleEffect_CleanupSceneObjectsFar:
	.incbin "baserom.gba", 0x000c84c8, 0x00000008
	.section .rom.000c84d0, "ax"
	.global EventObject_InitializeFar
	.type EventObject_InitializeFar, %function
	.thumb_func
EventObject_InitializeFar:
	.incbin "baserom.gba", 0x000c84d0, 0x00000008
	.section .rom.000c84d8, "ax"
	.global BattleEffect_InitializeSharedSceneFar
	.type BattleEffect_InitializeSharedSceneFar, %function
	.thumb_func
BattleEffect_InitializeSharedSceneFar:
	.incbin "baserom.gba", 0x000c84d8, 0x00000008
	.section .rom.000c84e0, "ax"
	.global BattleEffect_ResolvePendingActionsFar
	.type BattleEffect_ResolvePendingActionsFar, %function
	.thumb_func
BattleEffect_ResolvePendingActionsFar:
	.incbin "baserom.gba", 0x000c84e0, 0x00000008
	.section .rom.000c8518, "ax"
	.global BattleFx_HasMatchingEvent5Far
	.type BattleFx_HasMatchingEvent5Far, %function
	.thumb_func
BattleFx_HasMatchingEvent5Far:
	.incbin "baserom.gba", 0x000c8518, 0x00000008
	.section .rom.000c8520, "ax"
	.global BattleFx_HasTriggerFar
	.type BattleFx_HasTriggerFar, %function
	.thumb_func
BattleFx_HasTriggerFar:
	.incbin "baserom.gba", 0x000c8520, 0x00000008
	.section .rom.000c8528, "ax"
	.global Func_080c8518
	.type Func_080c8518, %function
	.thumb_func
Func_080c8518:
	.incbin "baserom.gba", 0x000c8528, 0x00000008
	.section .rom.000c8598, "ax"
	.global EffectSlot_UpdateFar
	.type EffectSlot_UpdateFar, %function
	.thumb_func
EffectSlot_UpdateFar:
	.incbin "baserom.gba", 0x000c8598, 0x00000008
	.section .rom.000c85a0, "ax"
	.global BattleFx_HasReachedTargetFar
	.type BattleFx_HasReachedTargetFar, %function
	.thumb_func
BattleFx_HasReachedTargetFar:
	.incbin "baserom.gba", 0x000c85a0, 0x00000008
	.section .rom.000c85a8, "ax"
	.global EffectSlot_SetPositionFar
	.type EffectSlot_SetPositionFar, %function
	.thumb_func
EffectSlot_SetPositionFar:
	.incbin "baserom.gba", 0x000c85a8, 0x00000008
	.section .rom.000c85b0, "ax"
	.global EffectSlot_SetObjectModeFar
	.type EffectSlot_SetObjectModeFar, %function
	.thumb_func
EffectSlot_SetObjectModeFar:
	.incbin "baserom.gba", 0x000c85b0, 0x00000008
	.section .rom.000c85b8, "ax"
	.global EffectSlot_SetCallbackFar
	.type EffectSlot_SetCallbackFar, %function
	.thumb_func
EffectSlot_SetCallbackFar:
	.incbin "baserom.gba", 0x000c85b8, 0x00000008
	.section .rom.000c85c0, "ax"
	.global Camera_WorldToScreenFar
	.type Camera_WorldToScreenFar, %function
	.thumb_func
Camera_WorldToScreenFar:
	.incbin "baserom.gba", 0x000c85c0, 0x00000008
	.section .rom.000c8628, "ax"
	.global Object_BuildRecordSnapshotFar
	.type Object_BuildRecordSnapshotFar, %function
	.thumb_func
Object_BuildRecordSnapshotFar:
	.incbin "baserom.gba", 0x000c8628, 0x00000010
	.section .rom.000c8658, "ax"
	.global Scene_GetAreaNameIndexFar
	.type Scene_GetAreaNameIndexFar, %function
	.thumb_func
Scene_GetAreaNameIndexFar:
	.incbin "baserom.gba", 0x000c8658, 0x00000008
	.section .rom.000c8660, "ax"
	.global Func_080c8650
	.type Func_080c8650, %function
	.thumb_func
Func_080c8650:
	.incbin "baserom.gba", 0x000c8660, 0x00000008
	.section .rom.000c8668, "ax"
	.global Func_080c8658
	.type Func_080c8658, %function
	.thumb_func
Func_080c8658:
	.incbin "baserom.gba", 0x000c8668, 0x00000008
	.section .rom.000c8670, "ax"
	.global Func_080c8660
	.type Func_080c8660, %function
	.thumb_func
Func_080c8660:
	.incbin "baserom.gba", 0x000c8670, 0x00000008
	.section .rom.000c8678, "ax"
	.global Func_080c8668
	.type Func_080c8668, %function
	.thumb_func
Func_080c8668:
	.incbin "baserom.gba", 0x000c8678, 0x00000008
	.section .rom.000c8680, "ax"
	.global Func_080c8670
	.type Func_080c8670, %function
	.thumb_func
Func_080c8670:
	.incbin "baserom.gba", 0x000c8680, 0x00000008
	.section .rom.000c8688, "ax"
	.global Func_080c8678
	.type Func_080c8678, %function
	.thumb_func
Func_080c8678:
	.incbin "baserom.gba", 0x000c8688, 0x00000008
	.section .rom.000c8690, "ax"
	.global Object_SetActionCallbackFar
	.type Object_SetActionCallbackFar, %function
	.thumb_func
Object_SetActionCallbackFar:
	.incbin "baserom.gba", 0x000c8690, 0x00000008
	.section .rom.000c8698, "ax"
	.global Func_080c8688
	.type Func_080c8688, %function
	.thumb_func
Func_080c8688:
	.incbin "baserom.gba", 0x000c8698, 0x00000008
	.section .rom.000c86a0, "ax"
	.global Event_NoOpFar
	.type Event_NoOpFar, %function
	.thumb_func
Event_NoOpFar:
	.incbin "baserom.gba", 0x000c86a0, 0x00000008
	.section .rom.000c88d8, "ax"
	.global Func_080c88c8
	.type Func_080c88c8, %function
	.thumb_func
Func_080c88c8:
	.incbin "baserom.gba", 0x000c88d8, 0x00000008
	.section .rom.000c88e0, "ax"
	.global Func_080c88d0
	.type Func_080c88d0, %function
	.thumb_func
Func_080c88d0:
	.incbin "baserom.gba", 0x000c88e0, 0x00000008
	.section .rom.000c88e8, "ax"
	.global Func_080c88d8
	.type Func_080c88d8, %function
	.thumb_func
Func_080c88d8:
	.incbin "baserom.gba", 0x000c88e8, 0x00000004
	.section .rom.000c88ec, "ax"
	.global BattleFx_DecodeFrameUnusedBase
BattleFx_DecodeFrameUnusedBase:
	.incbin "baserom.gba", 0x000c88ec, 0x00000004
	.section .rom.000c88f0, "ax"
	.global Func_080c88e0
	.type Func_080c88e0, %function
	.thumb_func
Func_080c88e0:
	.incbin "baserom.gba", 0x000c88f0, 0x00000008
	.section .rom.000c88f8, "ax"
	.global Func_080c88e8
	.type Func_080c88e8, %function
	.thumb_func
Func_080c88e8:
	.incbin "baserom.gba", 0x000c88f8, 0x00000008
	.section .rom.000c8900, "ax"
	.global Func_080c88f0
	.type Func_080c88f0, %function
	.thumb_func
Func_080c88f0:
	.incbin "baserom.gba", 0x000c8900, 0x00000008
	.section .rom.000c8908, "ax"
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.type ObjectMotion_ResetTargetsAndVelocityFar, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocityFar:
	.incbin "baserom.gba", 0x000c8908, 0x00000008
	.section .rom.000c8910, "ax"
	.global ObjectEffect_BeginContextEffect26Far
	.type ObjectEffect_BeginContextEffect26Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect26Far:
	.incbin "baserom.gba", 0x000c8910, 0x00000008
	.section .rom.000c8918, "ax"
	.global ObjectEffect_BeginContextEffect25Far
	.type ObjectEffect_BeginContextEffect25Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect25Far:
	.incbin "baserom.gba", 0x000c8918, 0x00000008
	.section .rom.000c8920, "ax"
	.global Func_080c8910
	.type Func_080c8910, %function
	.thumb_func
Func_080c8910:
	.incbin "baserom.gba", 0x000c8920, 0x00000008
	.section .rom.000c8928, "ax"
	.global Func_080c8918
	.type Func_080c8918, %function
	.thumb_func
Func_080c8918:
	.incbin "baserom.gba", 0x000c8928, 0x00000008
	.section .rom.000c8930, "ax"
	.global Func_080c8920
	.type Func_080c8920, %function
	.thumb_func
Func_080c8920:
	.incbin "baserom.gba", 0x000c8930, 0x00000008
	.section .rom.000c8938, "ax"
	.global Func_080c8928
	.type Func_080c8928, %function
	.thumb_func
Func_080c8928:
	.incbin "baserom.gba", 0x000c8938, 0x00000008
	.section .rom.000c8940, "ax"
	.global Func_080c8930
	.type Func_080c8930, %function
	.thumb_func
Func_080c8930:
	.incbin "baserom.gba", 0x000c8940, 0x00000008
	.section .rom.000c8948, "ax"
	.global Func_080c8938
	.type Func_080c8938, %function
	.thumb_func
Func_080c8938:
	.incbin "baserom.gba", 0x000c8948, 0x00000008
	.section .rom.000c8950, "ax"
	.global Func_080c8940
	.type Func_080c8940, %function
	.thumb_func
Func_080c8940:
	.incbin "baserom.gba", 0x000c8950, 0x00000008
	.section .rom.000c9694, "ax"
	.incbin "baserom.gba", 0x000c9694, 0x0000028c
	.section .rom.000c9944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000c9944, 0x000002cc
	.section .rom.000c9c38, "ax"
	.incbin "baserom.gba", 0x000c9c38, 0x00000190
	.section .rom.000c9e48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000c9e48, 0x000000e4
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
	.section .rom.000ca2fc, "ax"
	.incbin "baserom.gba", 0x000ca2fc, 0x00000198
	.section .rom.000ca4a8, "ax"
	.incbin "baserom.gba", 0x000ca4a8, 0x000000f8
	.section .rom.000ca5d8, "ax"
	.global BattleFx_SelectBattleCue
	.type BattleFx_SelectBattleCue, %function
	.thumb_func
BattleFx_SelectBattleCue:
	.incbin "baserom.gba", 0x000ca5d8, 0x000000cc
	.section .rom.000ca6a4, "ax"
	.global Event_SelectSpawnObjectMode
	.type Event_SelectSpawnObjectMode, %function
	.thumb_func
Event_SelectSpawnObjectMode:
	.incbin "baserom.gba", 0x000ca6a4, 0x00000044
	.section .rom.000ca6e8, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000ca6e8, 0x000002e4
	.section .rom.000ca9cc, "ax"
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000ca9cc, 0x00000060
	.section .rom.000caa2c, "ax"
	.global Func_080caa2c
Func_080caa2c:
	.incbin "baserom.gba", 0x000caa2c, 0x00000020
	.section .rom.000caa4c, "ax"
	.global Func_080caa4c
Func_080caa4c:
	.incbin "baserom.gba", 0x000caa4c, 0x00000274
	.section .rom.000cae5c, "ax"
	.incbin "baserom.gba", 0x000cae5c, 0x00000110
	.section .rom.000cb09c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cb09c, 0x0000062c
	.section .rom.000cb91c, "ax"
	.incbin "baserom.gba", 0x000cb91c, 0x00000d60
	.section .rom.000cc67c, "ax"
	.global Func_080cc67c
Func_080cc67c:
	.incbin "baserom.gba", 0x000cc67c, 0x00000148
	.section .rom.000cc7c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000cc7c4, 0x000001d0
	.section .rom.000cc9fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cc9fc, 0x000002bc
	.section .rom.000cccb8, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000cccb8, 0x00000090
	.section .rom.000ccd76, "ax"
	.incbin "baserom.gba", 0x000ccd76, 0x00000002
	.section .rom.000ccd78, "ax"
	.global Func_080c8958_Target
	.type Func_080c8958_Target, %function
	.thumb_func
Func_080c8958_Target:
	.incbin "baserom.gba", 0x000ccd78, 0x0000010c
	.section .rom.000cce94, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cce94, 0x00000034
	.section .rom.000ccec8, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000ccec8, 0x000006bc
	.section .rom.000cd584, "ax"
	.global Func_080cd584
Func_080cd584:
	.incbin "baserom.gba", 0x000cd584, 0x00000398
	.section .rom.000cd91c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000cd91c, 0x0000013c
	.section .rom.000cda84, "ax"
	.balign 4
	.global Func_080cda84
Func_080cda84:
	.incbin "baserom.gba", 0x000cda84, 0x0000003c
	.section .rom.000cdac0, "ax"
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000cdac0, 0x00000138
	.section .rom.000cdc72, "ax"
	.incbin "baserom.gba", 0x000cdc72, 0x00000002
	.section .rom.000cdc74, "ax"
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000cdc74, 0x0000010c
	.section .rom.000cdd80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000cdd80, 0x00000128
	.section .rom.000cdfb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000cdfb0, 0x0000036c
	.section .rom.000ce31c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000ce31c, 0x0000013c
	.section .rom.000ce458, "ax"
	.global Func_080ce458
	.type Func_080ce458, %function
	.thumb_func
Func_080ce458:
	.incbin "baserom.gba", 0x000ce458, 0x0000011c
	.section .rom.000ce61c, "ax"
	.incbin "baserom.gba", 0x000ce61c, 0x000004e0
	.section .rom.000ceba8, "ax"
	.incbin "baserom.gba", 0x000ceba8, 0x0000007c
	.section .rom.000cec34, "ax"
	.incbin "baserom.gba", 0x000cec34, 0x0000020c
	.section .rom.000ceeac, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ceeac, 0x00000078
	.section .rom.000cf050, "ax"
	.incbin "baserom.gba", 0x000cf050, 0x00000080
	.section .rom.000cf158, "ax"
	.incbin "baserom.gba", 0x000cf158, 0x00000024
	.section .rom.000cf17c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000cf17c, 0x000001c4
	.section .rom.000cf350, "ax"
	.balign 4
	.global Func_080cf350
	.type Func_080cf350, %function
	.thumb_func
Func_080cf350:
	.incbin "baserom.gba", 0x000cf350, 0x00000064
	.section .rom.000cf554, "ax"
	.incbin "baserom.gba", 0x000cf554, 0x000001a8
	.section .rom.000cf6fc, "ax"
	.global Func_080cf6fc
	.type Func_080cf6fc, %function
	.thumb_func
Func_080cf6fc:
	.incbin "baserom.gba", 0x000cf6fc, 0x00000090
	.section .rom.000cf78c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000cf78c, 0x0000096c
	.section .rom.000d00f8, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d00f8, 0x0000008c
	.section .rom.000d01cc, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d01cc, 0x00000354
	.section .rom.000d0520, "ax"
	.global DisplayTransition_Finish
	.type DisplayTransition_Finish, %function
	.thumb_func
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d0520, 0x0000020c
	.section .rom.000d0788, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d0788, 0x000001cc
	.section .rom.000d0954, "ax"
	.global Func_080d0954
	.type Func_080d0954, %function
	.thumb_func
Func_080d0954:
	.incbin "baserom.gba", 0x000d0954, 0x000000d4
	.section .rom.000d0a28, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d0a28, 0x00000154
	.section .rom.000d0b7c, "ax"
	.global DisplayTransition_FillTilemapAndSolidTile
	.type DisplayTransition_FillTilemapAndSolidTile, %function
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d0b7c, 0x00000070
	.section .rom.000d0ca0, "ax"
	.incbin "baserom.gba", 0x000d0ca0, 0x0000017c
	.section .rom.000d0e1c, "ax"
	.global BattleFx_BuildColorBuffers
	.type BattleFx_BuildColorBuffers, %function
	.thumb_func
BattleFx_BuildColorBuffers:
	.incbin "baserom.gba", 0x000d0e1c, 0x00000868
	.section .rom.000d1684, "ax"
	.global BattleEffect_InitializeBuffers
	.type BattleEffect_InitializeBuffers, %function
	.thumb_func
BattleEffect_InitializeBuffers:
	.incbin "baserom.gba", 0x000d1684, 0x00000074
	.section .rom.000d16f8, "ax"
	.global Func_080d0c50
	.type Func_080d0c50, %function
	.thumb_func
Func_080d0c50:
	.incbin "baserom.gba", 0x000d16f8, 0x00000014
	.section .rom.000d170c, "ax"
	.global BattleFx_ApplyColorToTargetBuffer
	.type BattleFx_ApplyColorToTargetBuffer, %function
	.thumb_func
BattleFx_ApplyColorToTargetBuffer:
	.incbin "baserom.gba", 0x000d170c, 0x00000020
	.section .rom.000d172c, "ax"
	.global BattleFx_ApplyColorToSourceBuffer
	.type BattleFx_ApplyColorToSourceBuffer, %function
	.thumb_func
BattleFx_ApplyColorToSourceBuffer:
	.incbin "baserom.gba", 0x000d172c, 0x00000020
	.section .rom.000d174c, "ax"
	.global BattleFx_SetBufferWord
	.type BattleFx_SetBufferWord, %function
	.thumb_func
BattleFx_SetBufferWord:
	.incbin "baserom.gba", 0x000d174c, 0x00000014
	.section .rom.000d1760, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1760, 0x0000004c
	.section .rom.000d17e8, "ax"
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d17e8, 0x00000034
	.section .rom.000d183e, "ax"
	.incbin "baserom.gba", 0x000d183e, 0x00000002
	.section .rom.000d1840, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000d1840, 0x00000284
	.section .rom.000d1ae4, "ax"
	.global Func_080d1ae4
	.type Func_080d1ae4, %function
	.thumb_func
Func_080d1ae4:
	.incbin "baserom.gba", 0x000d1ae4, 0x000001dc
	.section .rom.000d1dac, "ax"
	.global Func_080d2cc4
	.type Func_080d2cc4, %function
	.thumb_func
Func_080d2cc4:
	.incbin "baserom.gba", 0x000d1dac, 0x00000044
	.section .rom.000d1e18, "ax"
	.balign 4
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d1e18, 0x00000048
	.section .rom.000d1e60, "ax"
	.global Func_080d1e60
	.type Func_080d1e60, %function
	.thumb_func
Func_080d1e60:
	.incbin "baserom.gba", 0x000d1e60, 0x00000024
	.section .rom.000d1ee8, "ax"
	.balign 4
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d1ee8, 0x00000024
	.section .rom.000d1f20, "ax"
	.incbin "baserom.gba", 0x000d1f20, 0x00000164
	.section .rom.000d20fc, "ax"
	.incbin "baserom.gba", 0x000d20fc, 0x000000f8
	.section .rom.000d260c, "ax"
	.incbin "baserom.gba", 0x000d260c, 0x000001d4
	.section .rom.000d2834, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d2834, 0x0000011c
	.section .rom.000d2960, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d2960, 0x000000a0
	.section .rom.000d2b40, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
	.align 2, 0
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d2b40, 0x000000b8
	.section .rom.000d2c90, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d2c90, 0x00000028
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
	.incbin "baserom.gba", 0x000d2cfc, 0x00000068
	.section .rom.000d2d64, "ax"
	.global Func_080d2d70
	.type Func_080d2d70, %function
	.thumb_func
Func_080d2d70:
	.incbin "baserom.gba", 0x000d2d64, 0x00000014
	.section .rom.000d310a, "ax"
	.incbin "baserom.gba", 0x000d310a, 0x00000002
	.section .rom.000d310c, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.type ObjectMotion_SetHorizontalPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
	.incbin "baserom.gba", 0x000d310c, 0x0000004c
	.section .rom.000d3158, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrain
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrain:
	.incbin "baserom.gba", 0x000d3158, 0x0000005c
	.section .rom.000d31b4, "ax"
	.global ObjectMotion_SetPositionWithTerrain
	.type ObjectMotion_SetPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrain:
	.incbin "baserom.gba", 0x000d31b4, 0x00000054
	.section .rom.000d3208, "ax"
	.global ObjectMotion_MoveToObject
	.type ObjectMotion_MoveToObject, %function
	.thumb_func
ObjectMotion_MoveToObject:
	.incbin "baserom.gba", 0x000d3208, 0x0000002c
	.section .rom.000d325c, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d325c, 0x00000050
	.section .rom.000d336a, "ax"
	.incbin "baserom.gba", 0x000d336a, 0x00000002
	.section .rom.000d336c, "ax"
	.incbin "baserom.gba", 0x000d336c, 0x000000e8
	.section .rom.000d3454, "ax"
	.global BattleFx_RunRisingObjectSequence
	.type BattleFx_RunRisingObjectSequence, %function
	.thumb_func
BattleFx_RunRisingObjectSequence:
	.incbin "baserom.gba", 0x000d3454, 0x0000013c
	.section .rom.000d3774, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000d3774, 0x00000054
	.section .rom.000d391c, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000d3b1c, 0x000000c0
	.section .rom.000d3c7c, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d3c7c, 0x00000328
	.section .rom.000d3fa4, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000d3fa4, 0x000000bc
	.section .rom.000d40d0, "ax"
	.balign 4
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000d40d0, 0x0000009c
	.section .rom.000d41da, "ax"
	.incbin "baserom.gba", 0x000d41da, 0x00000002
	.section .rom.000d41f0, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000d41f0, 0x00000134
	.section .rom.000d4324, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000d4324, 0x00000054
	.section .rom.000d4400, "ax"
	.balign 4
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d4400, 0x00000100
	.section .rom.000d47a8, "ax"
	.incbin "baserom.gba", 0x000d47a8, 0x000000b4
	.section .rom.000d4890, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d4890, 0x000000f0
	.section .rom.000d4aa8, "ax"
	.global Func_080da908
	.type Func_080da908, %function
	.thumb_func
Func_080da908:
	.incbin "baserom.gba", 0x000d4aa8, 0x00000058
	.section .rom.000d4bb8, "ax"
	.incbin "baserom.gba", 0x000d4bb8, 0x00000108
	.section .rom.000d4cfc, "ax"
	.balign 4
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000d4cfc, 0x000003f0
	.section .rom.000d50ec, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
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
	.section .rom.000d5e6c, "ax"
	.balign 4
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000d5e6c, 0x000000a0
	.section .rom.000d6070, "ax"
	.global Func_080d607c
	.type Func_080d607c, %function
	.thumb_func
Func_080d607c:
	.incbin "baserom.gba", 0x000d6070, 0x0000038c
	.section .rom.000d64e4, "ax"
	.incbin "baserom.gba", 0x000d64e4, 0x00000184
	.section .rom.000d6668, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000d6668, 0x00000174
	.section .rom.000d67dc, "ax"
	.global Func_080d67e8
	.type Func_080d67e8, %function
	.thumb_func
Func_080d67e8:
	.incbin "baserom.gba", 0x000d67dc, 0x00000124
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
	.incbin "baserom.gba", 0x000d6b84, 0x00000494
	.section .rom.000d7028, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000d7028, 0x0000020c
	.section .rom.000d7518, "ax"
	.incbin "baserom.gba", 0x000d7518, 0x00000264
	.section .rom.000d777c, "ax"
	.global Func_080c9dc8
	.type Func_080c9dc8, %function
	.thumb_func
Func_080c9dc8:
	.incbin "baserom.gba", 0x000d777c, 0x000002d0
	.section .rom.000d7af8, "ax"
	.incbin "baserom.gba", 0x000d7af8, 0x00000244
	.section .rom.000d7d84, "ax"
	.incbin "baserom.gba", 0x000d7d84, 0x0000097c
	.section .rom.000d8734, "ax"
	.incbin "baserom.gba", 0x000d8734, 0x0000023c
	.section .rom.000d8970, "ax"
	.global Func_080d2c9c
	.type Func_080d2c9c, %function
	.thumb_func
Func_080d2c9c:
	.incbin "baserom.gba", 0x000d8970, 0x00000084
	.section .rom.000d89f4, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000d89f4, 0x00000210
	.section .rom.000d8c04, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000d8c04, 0x00000130
	.section .rom.000d8d5c, "ax"
	.balign 4
	.global Func_080ca6e4
	.type Func_080ca6e4, %function
	.thumb_func
Func_080ca6e4:
	.incbin "baserom.gba", 0x000d8d5c, 0x00000240
	.section .rom.000d8f9c, "ax"
	.global Func_080d8fa8
	.type Func_080d8fa8, %function
	.thumb_func
Func_080d8fa8:
	.incbin "baserom.gba", 0x000d8f9c, 0x0000015c
	.section .rom.000d90f8, "ax"
	.global Func_080d897c
	.type Func_080d897c, %function
	.thumb_func
Func_080d897c:
	.incbin "baserom.gba", 0x000d90f8, 0x000001a0
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
	.incbin "baserom.gba", 0x000d9704, 0x00000268
	.section .rom.000d9aa4, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000d9afc, 0x00000078
	.section .rom.000d9b74, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000d9b74, 0x000001c0
	.section .rom.000d9d34, "ax"
	.global Func_080d9d40
	.type Func_080d9d40, %function
	.thumb_func
Func_080d9d40:
	.incbin "baserom.gba", 0x000d9d34, 0x00000134
	.section .rom.000d9e8c, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000d9e8c, 0x000001c8
	.section .rom.000da054, "ax"
	.global Func_080da060
	.type Func_080da060, %function
	.thumb_func
Func_080da060:
	.incbin "baserom.gba", 0x000da054, 0x000008a8
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
	.incbin "baserom.gba", 0x000da92c, 0x00000030
	.section .rom.000da99c, "ax"
	.incbin "baserom.gba", 0x000da99c, 0x00000500
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
	.section .rom.000db99a, "ax"
	.incbin "baserom.gba", 0x000db99a, 0x00000002
	.section .rom.000db99c, "ax"
	.global Func_080db9a8
	.type Func_080db9a8, %function
	.thumb_func
Func_080db9a8:
	.incbin "baserom.gba", 0x000db99c, 0x00000018
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
	.incbin "baserom.gba", 0x000db9c0, 0x00000078
	.section .rom.000dba50, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dba50, 0x000000e4
	.section .rom.000dbb6c, "ax"
	.global Func_080dbb78
Func_080dbb78:
	.balign 4
	.global Map_GetCellAtPosition
	.type Map_GetCellAtPosition, %function
	.thumb_func
Map_GetCellAtPosition:
	.incbin "baserom.gba", 0x000dbb6c, 0x0000008c
	.section .rom.000dbbf8, "ax"
	.global Func_080dbc04
Func_080dbc04:
	.incbin "baserom.gba", 0x000dbbf8, 0x000000a4
	.section .rom.000dbc9c, "ax"
	.global Func_080dbca8
	.type Func_080dbca8, %function
	.thumb_func
Func_080dbca8:
	.incbin "baserom.gba", 0x000dbc9c, 0x00000018
	.section .rom.000dbcb4, "ax"
	.global Func_080dbcc0
Func_080dbcc0:
	.incbin "baserom.gba", 0x000dbcb4, 0x00000018
	.section .rom.000dbccc, "ax"
	.global Func_080dbcd8
Func_080dbcd8:
	.global Map_TestSpecialTerrainPair
	.type Map_TestSpecialTerrainPair, %function
	.thumb_func
Map_TestSpecialTerrainPair:
	.incbin "baserom.gba", 0x000dbccc, 0x00000070
	.section .rom.000dbd3c, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000dbd3c, 0x00000014
	.section .rom.000dbd50, "ax"
	.global Func_080dbd5c
Func_080dbd5c:
	.incbin "baserom.gba", 0x000dbd50, 0x0000004c
	.section .rom.000dbd9c, "ax"
	.global Func_080dbda8
	.type Func_080dbda8, %function
	.thumb_func
Func_080dbda8:
	.incbin "baserom.gba", 0x000dbd9c, 0x00000020
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
	.incbin "baserom.gba", 0x000dbe38, 0x0000003c
	.section .rom.000dbe74, "ax"
	.global Func_080dbe80
Func_080dbe80:
	.incbin "baserom.gba", 0x000dbe74, 0x000001c4
	.section .rom.000dc1a4, "ax"
	.global Func_080db9c0
	.type Func_080db9c0, %function
	.thumb_func
Func_080db9c0:
	.incbin "baserom.gba", 0x000dc1a4, 0x00000094
	.section .rom.000dc288, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000dc288, 0x000000e8
	.section .rom.000dc404, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x000dc404, 0x0000021c
	.section .rom.000dc6cc, "ax"
	.incbin "baserom.gba", 0x000dc6cc, 0x000000f4
	.section .rom.000dc7dc, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000dc7dc, 0x0000016c
	.section .rom.000dc96c, "ax"
	.balign 4
	.global Func_080cdec8
	.type Func_080cdec8, %function
	.thumb_func
Func_080cdec8:
	.incbin "baserom.gba", 0x000dc96c, 0x000000d8
	.section .rom.000dcad0, "ax"
	.balign 4
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000dcad0, 0x00000068
	.section .rom.000dcb38, "ax"
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000dcb38, 0x00000284
	.section .rom.000dd048, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000dd048, 0x000003e8
	.section .rom.000dd51a, "ax"
	.incbin "baserom.gba", 0x000dd51a, 0x00000002
	.section .rom.000dd51c, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000dd51c, 0x00000114
	.section .rom.000dd630, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000dd630, 0x0000002c
	.section .rom.000dd65c, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000dd65c, 0x000000e0
	.section .rom.000dd748, "ax"
	.incbin "baserom.gba", 0x000dd748, 0x000001fc
	.section .rom.000dd944, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000dd944, 0x000001ec
	.section .rom.000ddbcc, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000ddbcc, 0x0000063c
	.section .rom.000de210, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000de210, 0x00000334
	.section .rom.000de67c, "ax"
	.global BattleFx_RunOrbitingParticles
	.type BattleFx_RunOrbitingParticles, %function
	.thumb_func
BattleFx_RunOrbitingParticles:
	.incbin "baserom.gba", 0x000de67c, 0x00000190
	.section .rom.000de858, "ax"
	.incbin "baserom.gba", 0x000de858, 0x00000188
	.section .rom.000de9ec, "ax"
	.global Func_080de9f8
	.type Func_080de9f8, %function
	.thumb_func
Func_080de9f8:
	.incbin "baserom.gba", 0x000de9ec, 0x000001ac
	.section .rom.000debcc, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000debcc, 0x000000b4
	.section .rom.000decac, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000decac, 0x000000cc
	.section .rom.000deddc, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000deddc, 0x00000ac0
	.section .rom.000df8d2, "ax"
	.incbin "baserom.gba", 0x000df8d2, 0x00000002
	.section .rom.000df8d4, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000df8d4, 0x000001d4
	.section .rom.000dfc30, "ax"
	.incbin "baserom.gba", 0x000dfc30, 0x00000098
	.section .rom.000dfcf4, "ax"
	.global RunBattleEffect14
RunBattleEffect14:
	.incbin "baserom.gba", 0x000dfcf4, 0x000002fc
	.section .rom.000e0014, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e0014, 0x000000fc
	.section .rom.000e0128, "ax"
	.global Func_080e0134
	.type Func_080e0134, %function
	.thumb_func
Func_080e0134:
	.incbin "baserom.gba", 0x000e0128, 0x000001b8
	.section .rom.000e03b6, "ax"
	.incbin "baserom.gba", 0x000e03b6, 0x00000002
	.section .rom.000e03c0, "ax"
	.balign 4
	.global Func_080e03cc
	.type Func_080e03cc, %function
	.thumb_func
Func_080e03cc:
	.incbin "baserom.gba", 0x000e03c0, 0x0000024c
	.section .rom.000e060c, "ax"
	.incbin "baserom.gba", 0x000e060c, 0x00000360
	.section .rom.000e09b4, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000e09b4, 0x000002c4
	.section .rom.000e0ca0, "ax"
	.global Func_080e0cac
	.type Func_080e0cac, %function
	.thumb_func
Func_080e0cac:
	.incbin "baserom.gba", 0x000e0ca0, 0x00000040
	.section .rom.000e0ce0, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000e0ce0, 0x000000e8
	.section .rom.000e0f34, "ax"
	.incbin "baserom.gba", 0x000e0f34, 0x00000214
	.section .rom.000e1148, "ax"
	.global Func_080e1154
	.type Func_080e1154, %function
	.thumb_func
Func_080e1154:
	.incbin "baserom.gba", 0x000e1148, 0x00000068
	.section .rom.000e143c, "ax"
	.global BattleFx_SpawnScaledArcObjects
	.type BattleFx_SpawnScaledArcObjects, %function
	.thumb_func
BattleFx_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000e143c, 0x000000c4
	.section .rom.000e1500, "ax"
	.global BattleFx_UpdateArcEffect
	.type BattleFx_UpdateArcEffect, %function
	.thumb_func
BattleFx_UpdateArcEffect:
	.incbin "baserom.gba", 0x000e1500, 0x000000f0
	.section .rom.000e166c, "ax"
	.incbin "baserom.gba", 0x000e166c, 0x000008ac
	.section .rom.000e1f20, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000e1f20, 0x000006bc
	.section .rom.000e25dc, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000e25dc, 0x0000018c
	.section .rom.000e2768, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000e2768, 0x000004c4
	.section .rom.000e2c2c, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000e2c2c, 0x00000108
	.section .rom.000e2d58, "ax"
	.incbin "baserom.gba", 0x000e2d58, 0x000002fc
	.section .rom.000e3068, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000e3068, 0x00000624
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
	.incbin "baserom.gba", 0x000e37d4, 0x0000051c
	.section .rom.000e3cf8, "ax"
	.balign 4
	.global Func_080e3d04
	.type Func_080e3d04, %function
	.thumb_func
Func_080e3d04:
	.incbin "baserom.gba", 0x000e3cf8, 0x00000540
	.section .rom.000e4238, "ax"
	.global Func_080e137c
	.type Func_080e137c, %function
	.thumb_func
Func_080e137c:
	.incbin "baserom.gba", 0x000e4238, 0x00000090
	.section .rom.000e42c8, "ax"
	.global Func_080e42d4
Func_080e42d4:
	.incbin "baserom.gba", 0x000e42c8, 0x000002cc
	.section .rom.000e459c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e459c, 0x00000188
	.section .rom.000e4724, "ax"
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000e4724, 0x000002fc
	.section .rom.000e4a28, "ax"
	.balign 4
	.global Func_080e4a34
	.type Func_080e4a34, %function
	.thumb_func
Func_080e4a34:
	.incbin "baserom.gba", 0x000e4a28, 0x00000cf0
	.section .rom.000e5720, "ax"
	.balign 4
	.global Func_080e572c
	.type Func_080e572c, %function
	.thumb_func
Func_080e572c:
	.incbin "baserom.gba", 0x000e5720, 0x000005c4
	.section .rom.000e5d50, "ax"
	.balign 4
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000e5d50, 0x0000062c
	.section .rom.000e63f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e63f4, 0x000004c8
	.section .rom.000e68c4, "ax"
	.balign 4
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000e68c4, 0x00000828
	.section .rom.000e70ec, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000e70ec, 0x00000720
	.section .rom.000e780c, "ax"
	.global Func_080e7818
Func_080e7818:
	.incbin "baserom.gba", 0x000e780c, 0x00000024
	.section .rom.000e7830, "ax"
	.global Func_080e783c
Func_080e783c:
	.incbin "baserom.gba", 0x000e7830, 0x00000f88
	.section .rom.000e87b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e87b8, 0x000005bc
	.section .rom.000e8da4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8da4, 0x000004a0
	.section .rom.000e92a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e92a8, 0x00000838
	.section .rom.000e9ae8, "ax"
	.balign 4
	.global Func_080e9af4
	.type Func_080e9af4, %function
	.thumb_func
Func_080e9af4:
	.incbin "baserom.gba", 0x000e9ae8, 0x000002c0
	.section .rom.000e9da8, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000e9da8, 0x00000130
	.section .rom.000e9f5c, "ax"
	.incbin "baserom.gba", 0x000e9f5c, 0x00000858
	.section .rom.000ea7b4, "ax"
	.global Func_080d9ad0
	.type Func_080d9ad0, %function
	.thumb_func
Func_080d9ad0:
	.incbin "baserom.gba", 0x000ea7b4, 0x000000e8
	.section .rom.000ea89c, "ax"
	.global Func_080ea8a8
Func_080ea8a8:
	.incbin "baserom.gba", 0x000ea89c, 0x0000002c
	.section .rom.000ea8c8, "ax"
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
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
	.global Func_080eb01c
Func_080eb01c:
	.incbin "baserom.gba", 0x000eb010, 0x0000027c
	.section .rom.000eb30c, "ax"
	.incbin "baserom.gba", 0x000eb30c, 0x00000204
	.section .rom.000eb588, "ax"
	.global Func_080eb594
	.type Func_080eb594, %function
	.thumb_func
Func_080eb594:
	.incbin "baserom.gba", 0x000eb588, 0x0000010c
	.section .rom.000eb694, "ax"
	.global Func_080eb6a0
	.type Func_080eb6a0, %function
	.thumb_func
Func_080eb6a0:
	.incbin "baserom.gba", 0x000eb694, 0x00000184
	.section .rom.000eb818, "ax"
	.incbin "baserom.gba", 0x000eb818, 0x0000010c
	.section .rom.000eb954, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000eb954, 0x000002d0
	.section .rom.000ebd18, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000ebd18, 0x0000014c
	.section .rom.000ebebc, "ax"
	.global EffectSlot_Initialize
	.type EffectSlot_Initialize, %function
	.thumb_func
EffectSlot_Initialize:
	.incbin "baserom.gba", 0x000ebebc, 0x000000a0
	.section .rom.000ebf88, "ax"
	.incbin "baserom.gba", 0x000ebf88, 0x000001b8
	.section .rom.000ec1c4, "ax"
	.incbin "baserom.gba", 0x000ec1c4, 0x000000c8
	.section .rom.000ec2ac, "ax"
	.incbin "baserom.gba", 0x000ec2ac, 0x000014d0
	.section .rom.000eda00, "ax"
	.incbin "baserom.gba", 0x000eda00, 0x000000c0
	.section .rom.000ef810, "ax"
	.incbin "baserom.gba", 0x000ef810, 0x00000008
	.section .rom.000f01ac, "ax"
	.incbin "baserom.gba", 0x000f01ac, 0x0000000c
	.section .rom.000f02fa, "ax"
	.incbin "baserom.gba", 0x000f02fa, 0x00000002
	.section .rom.000f0b44, "ax"
	.incbin "baserom.gba", 0x000f0b44, 0x0000002c
	.section .rom.000f0bf0, "ax"
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000f0bf0, 0x00000008
	.section .rom.000f0bf8, "ax"
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000f0bf8, 0x00000250
	.section .rom.000f0e48, "ax"
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0e48, 0x00000024
	.section .rom.000f0e6c, "ax"
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000f0e6c, 0x00000024
	.section .rom.000f0e90, "ax"
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000f0e90, 0x00000008
	.section .rom.000f0eb8, "ax"
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f0eb8, 0x00000024
	.section .rom.000f0edc, "ax"
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000f0edc, 0x00000044
	.section .rom.000f0f20, "ax"
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0f20, 0x000001d4
	.section .rom.000f1114, "ax"
	.incbin "baserom.gba", 0x000f1114, 0x00000020
	.section .rom.000f1134, "ax"
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000f1134, 0x00000668
	.section .rom.000f21c4, "ax"
	.incbin "baserom.gba", 0x000f21c4, 0x00000034
	.section .rom.000f3210, "ax"
	.incbin "baserom.gba", 0x000f3210, 0x000000b4
	.section .rom.000f32c4, "ax"
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f32c4, 0x00000010
	.section .rom.000f32d4, "ax"
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x000f32d4, 0x00000020
	.section .rom.000f32f4, "ax"
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f32f4, 0x00000128
	.section .rom.000f341c, "ax"
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x000f341c, 0x000000bc
	.section .rom.000f34d8, "ax"
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x000f34d8, 0x00000004
	.section .rom.000f34dc, "ax"
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x000f34dc, 0x0000000c
	.section .rom.000f34e8, "ax"
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x000f34e8, 0x0000000c
	.section .rom.000f34f4, "ax"
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x000f34f4, 0x0000000c
	.section .rom.000f3500, "ax"
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x000f3500, 0x000000bc
	.section .rom.000f35bc, "ax"
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x000f35bc, 0x0000004c
	.section .rom.000f3608, "ax"
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x000f3608, 0x00000128
	.section .rom.000f3730, "ax"
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x000f3730, 0x00000014
	.section .rom.000f3744, "ax"
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x000f3744, 0x00000014
	.section .rom.000f3758, "ax"
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x000f3758, 0x0000001c
	.section .rom.000f3774, "ax"
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f3774, 0x00000018
	.section .rom.000f3852, "ax"
	.incbin "baserom.gba", 0x000f3852, 0x00000054
	.section .rom.000f38f5, "ax"
	.incbin "baserom.gba", 0x000f38f5, 0x00000043
	.section .rom.000f3938, "ax"
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x000f3938, 0x0000000c
	.section .rom.000f3944, "ax"
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x000f3944, 0x00000024
	.section .rom.000f3968, "ax"
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000f3968, 0x00000050
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x000004a0
	.section .rom.000f8888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8888, 0x0000003c
	.section .rom.000f88d0, "ax"
	.global Func_080f88d0
	.type Func_080f88d0, %function
	.thumb_func
Func_080f88d0:
	.incbin "baserom.gba", 0x000f88d0, 0x000001e4
	.section .rom.000f8ab4, "ax"
	.global Func_080f8ab4
	.type Func_080f8ab4, %function
	.thumb_func
Func_080f8ab4:
	.incbin "baserom.gba", 0x000f8ab4, 0x00000108
	.section .rom.000f8ce6, "ax"
	.incbin "baserom.gba", 0x000f8ce6, 0x00000002
	.section .rom.000f8ce8, "ax"
	.global Func_080f8ce8
	.type Func_080f8ce8, %function
	.thumb_func
Func_080f8ce8:
	.incbin "baserom.gba", 0x000f8ce8, 0x00000258
	.section .rom.000f8f9c, "ax"
	.incbin "baserom.gba", 0x000f8f9c, 0x0000016c
	.section .rom.000f92dc, "ax"
	.incbin "baserom.gba", 0x000f92dc, 0x00000098
	.section .rom.000f93b6, "ax"
	.incbin "baserom.gba", 0x000f93b6, 0x00000002
	.section .rom.000f94a2, "ax"
	.incbin "baserom.gba", 0x000f94a2, 0x00000002
	.section .rom.000f94a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f94a4, 0x00000f3c
	.section .rom.000fa484, "ax"
	.incbin "baserom.gba", 0x000fa484, 0x0000003c
	.section .rom.000fa5ec, "ax"
	.global Func_080fa5a4
	.type Func_080fa5a4, %function
	.thumb_func
Func_080fa5a4:
	.incbin "baserom.gba", 0x000fa5ec, 0x000002a8
	.section .rom.000fa8b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fa8b8, 0x00000370
	.section .rom.000faca0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000faca0, 0x00000048
	.section .rom.000fad1e, "ax"
	.incbin "baserom.gba", 0x000fad1e, 0x00000002
	.section .rom.000fad20, "ax"
	.global Menu_HideEmptyEntryIcons
	.type Menu_HideEmptyEntryIcons, %function
	.thumb_func
Menu_HideEmptyEntryIcons:
	.incbin "baserom.gba", 0x000fad20, 0x00000044
	.section .rom.000faed4, "ax"
	.incbin "baserom.gba", 0x000faed4, 0x00000218
	.section .rom.000fb14c, "ax"
	.incbin "baserom.gba", 0x000fb14c, 0x00000534
	.section .rom.000fb7c8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fb7c8, 0x0000012c
	.section .rom.000fb900, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x000fb900, 0x0000047c
	.section .rom.000fbeb4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fbeb4, 0x00000618
	.section .rom.000fc5a4, "ax"
	.incbin "baserom.gba", 0x000fc5a4, 0x00000560
	.section .rom.000fcb04, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000fcb04, 0x00000854
	.section .rom.000fd3ac, "ax"
	.incbin "baserom.gba", 0x000fd3ac, 0x00000244
	.section .rom.000fd620, "ax"
	.incbin "baserom.gba", 0x000fd620, 0x00000188
	.section .rom.000fd7e8, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x000fd7e8, 0x00000a78
	.section .rom.000fe370, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x000fe370, 0x0000030c
	.section .rom.000fe67c, "ax"
	.global Func_080fe580
	.type Func_080fe580, %function
	.thumb_func
Func_080fe580:
	.incbin "baserom.gba", 0x000fe67c, 0x00000884
	.section .rom.000fef3c, "ax"
	.incbin "baserom.gba", 0x000fef3c, 0x000003b4
	.section .rom.000ff46c, "ax"
	.incbin "baserom.gba", 0x000ff46c, 0x00000570
	.section .rom.000ffa48, "ax"
	.incbin "baserom.gba", 0x000ffa48, 0x00000b8c
	.section .rom.0010065c, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x0010065c, 0x000000e4
	.section .rom.00100854, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00100854, 0x00000608
	.section .rom.00100f98, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00100f98, 0x00000734
	.section .rom.00101754, "ax"
	.incbin "baserom.gba", 0x00101754, 0x00000490
	.section .rom.00101be4, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00101be4, 0x00000148
	.section .rom.00101d60, "ax"
	.incbin "baserom.gba", 0x00101d60, 0x0000003c
	.section .rom.00101d9c, "ax"
	.global Func_08101c7c
	.type Func_08101c7c, %function
	.thumb_func
Func_08101c7c:
	.incbin "baserom.gba", 0x00101d9c, 0x000000b8
	.section .rom.00101e7c, "ax"
	.incbin "baserom.gba", 0x00101e7c, 0x00002cf0
	.section .rom.00104c6a, "ax"
	.incbin "baserom.gba", 0x00104c6a, 0x00000002
	.section .rom.00104c6c, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x00104c6c, 0x00000174
	.section .rom.00104e70, "ax"
	.incbin "baserom.gba", 0x00104e70, 0x0000004c
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
	.section .rom.001051e0, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x001051e0, 0x000000dc
	.section .rom.001052bc, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x001052bc, 0x000000c4
	.section .rom.001054bc, "ax"
	.incbin "baserom.gba", 0x001054bc, 0x000000c0
	.section .rom.001055f4, "ax"
	.incbin "baserom.gba", 0x001055f4, 0x000000a0
	.section .rom.0010570a, "ax"
	.incbin "baserom.gba", 0x0010570a, 0x00000346
	.section .rom.00105a50, "ax"
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x00105a50, 0x00000004
	.section .rom.00105a54, "ax"
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x00105a54, 0x00000004
	.section .rom.00105a58, "ax"
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x00105a58, 0x00000004
	.section .rom.00105a5c, "ax"
	.global Data_08105948
Data_08105948:
	.incbin "baserom.gba", 0x00105a5c, 0x00000024
	.section .rom.00105a80, "ax"
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x00105a80, 0x00000004
	.section .rom.00105a84, "ax"
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x00105a84, 0x00000014
	.section .rom.00105ac8, "ax"
	.incbin "baserom.gba", 0x00105ac8, 0x0000003a
	.section .rom.00105b02, "ax"
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x00105b02, 0x0000000d
	.section .rom.00105b0f, "ax"
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x00105b0f, 0x0000000d
	.section .rom.00105b1c, "ax"
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x00105b1c, 0x00000018
	.section .rom.00105b34, "ax"
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x00105b34, 0x00000018
	.section .rom.00105b4c, "ax"
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x00105b4c, 0x00000008
	.section .rom.00105b54, "ax"
	.global Data_08105a40
Data_08105a40:
	.incbin "baserom.gba", 0x00105b54, 0x0000001c
	.section .rom.00108000, "ax"
	.global Resource_FarCall008
Resource_FarCall008:
	.incbin "baserom.gba", 0x00108000, 0x00000008
	.section .rom.00108008, "ax"
	.global Func_08108008
	.type Func_08108008, %function
	.thumb_func
Func_08108008:
	.incbin "baserom.gba", 0x00108008, 0x00000008
	.section .rom.00108010, "ax"
	.global Func_08108010
	.type Func_08108010, %function
	.thumb_func
Func_08108010:
	.incbin "baserom.gba", 0x00108010, 0x00000008
	.section .rom.00108018, "ax"
	.global Func_08108018
	.type Func_08108018, %function
	.thumb_func
Func_08108018:
	.incbin "baserom.gba", 0x00108018, 0x00000008
	.section .rom.00108020, "ax"
	.global Func_08108020
	.type Func_08108020, %function
	.thumb_func
Func_08108020:
	.incbin "baserom.gba", 0x00108020, 0x00000008
	.section .rom.00108028, "ax"
	.global Func_08108028
	.type Func_08108028, %function
	.thumb_func
Func_08108028:
	.incbin "baserom.gba", 0x00108028, 0x00000008
	.section .rom.00108030, "ax"
	.global Func_08108030
	.type Func_08108030, %function
	.thumb_func
Func_08108030:
	.incbin "baserom.gba", 0x00108030, 0x00000008
	.section .rom.00108038, "ax"
	.global Func_08108038
	.type Func_08108038, %function
	.thumb_func
Func_08108038:
	.incbin "baserom.gba", 0x00108038, 0x00000008
	.section .rom.00108040, "ax"
	.global Func_08108040
	.type Func_08108040, %function
	.thumb_func
Func_08108040:
	.incbin "baserom.gba", 0x00108040, 0x00000008
	.section .rom.00108048, "ax"
	.global Func_08108048
	.type Func_08108048, %function
	.thumb_func
Func_08108048:
	.incbin "baserom.gba", 0x00108048, 0x00000008
	.section .rom.00108050, "ax"
	.global Shop_FillSelectorFar
	.type Shop_FillSelectorFar, %function
	.thumb_func
Shop_FillSelectorFar:
	.incbin "baserom.gba", 0x00108050, 0x00000008
	.section .rom.00108058, "ax"
	.global Shop_RestoreSceneTilesFar
	.type Shop_RestoreSceneTilesFar, %function
	.thumb_func
Shop_RestoreSceneTilesFar:
	.incbin "baserom.gba", 0x00108058, 0x00000008
	.section .rom.00108060, "ax"
	.global Shop_InitEffectFar
	.type Shop_InitEffectFar, %function
	.thumb_func
Shop_InitEffectFar:
	.incbin "baserom.gba", 0x00108060, 0x00000008
	.section .rom.00108068, "ax"
	.global Func_08108068
	.type Func_08108068, %function
	.thumb_func
Func_08108068:
	.incbin "baserom.gba", 0x00108068, 0x00000008
	.section .rom.00108070, "ax"
	.global Func_08108070
	.type Func_08108070, %function
	.thumb_func
Func_08108070:
	.incbin "baserom.gba", 0x00108070, 0x00000008
	.section .rom.00108078, "ax"
	.global AudioCommand_WaitForStateByteClearFar
	.type AudioCommand_WaitForStateByteClearFar, %function
	.thumb_func
AudioCommand_WaitForStateByteClearFar:
	.incbin "baserom.gba", 0x00108078, 0x00000008
	.section .rom.00108080, "ax"
	.global Inn_PlaySleepFar
	.type Inn_PlaySleepFar, %function
	.thumb_func
Inn_PlaySleepFar:
	.incbin "baserom.gba", 0x00108080, 0x00000008
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
	.incbin "baserom.gba", 0x0010a9b0, 0x000004d8
	.section .rom.0010af08, "ax"
	.incbin "baserom.gba", 0x0010af08, 0x00000198
	.section .rom.0010b0a0, "ax"
	.global BattleFx_UpdateRadialMotion
BattleFx_UpdateRadialMotion:
	.incbin "baserom.gba", 0x0010b0a0, 0x000000ac
	.section .rom.0010b14c, "ax"
	.global Func_0810b168
Func_0810b168:
	.incbin "baserom.gba", 0x0010b14c, 0x00000210
	.section .rom.0010b35c, "ax"
	.global Func_0810b378
	.type Func_0810b378, %function
	.thumb_func
Func_0810b378:
	.incbin "baserom.gba", 0x0010b35c, 0x00000074
	.section .rom.0010b3fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010b3fc, 0x00000108
	.section .rom.0010b5a8, "ax"
	.incbin "baserom.gba", 0x0010b5a8, 0x000000f8
	.section .rom.0010b6b0, "ax"
	.incbin "baserom.gba", 0x0010b6b0, 0x000000d0
	.section .rom.0010b798, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x0010b798, 0x0000060c
	.section .rom.0010bde8, "ax"
	.incbin "baserom.gba", 0x0010bde8, 0x00000048
	.section .rom.0010beb0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010beb0, 0x000000dc
	.section .rom.0010bffc, "ax"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0010bffc, 0x00000140
	.section .rom.0010c13c, "ax"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0010c13c, 0x00000200
	.section .rom.0010c378, "ax"
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x0010c378, 0x0000000a
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
	.incbin "baserom.gba", 0x00163c2c, 0x0000015c
	.section .rom.00163e14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163e14, 0x00000de0
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
	.incbin "baserom.gba", 0x00166b10, 0x000028a0
	.section .rom.001693d0, "ax"
	.incbin "baserom.gba", 0x001693d0, 0x00000e48
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
	.incbin "baserom.gba", 0x001a2184, 0x000006b0
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
	.incbin "baserom.gba", 0x001a8286, 0x00000490
	.section .rom.001a8812, "ax"
	.incbin "baserom.gba", 0x001a8812, 0x00000002
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
	.incbin "baserom.gba", 0x001b83c4, 0x00001f58
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
	.section .rom.0068a153, "ax"
	.incbin "baserom.gba", 0x0068a153, 0x00000001
	.section .rom.006979e6, "ax"
	.incbin "baserom.gba", 0x006979e6, 0x00000002
	.section .rom.006a457d, "ax"
	.incbin "baserom.gba", 0x006a457d, 0x00000003
	.section .rom.006a5799, "ax"
	.incbin "baserom.gba", 0x006a5799, 0x00000003
	.section .rom.006a99e3, "ax"
	.incbin "baserom.gba", 0x006a99e3, 0x00000001
	.section .rom.006b0fff, "ax"
	.incbin "baserom.gba", 0x006b0fff, 0x00000001
	.section .rom.006cd240, "ax"
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
	.section .rom.0087bf48, "ax"
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087bf48, 0x0000024c
	.section .rom.0087c194, "ax"
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c194, 0x00000198
	.section .rom.0087cb55, "ax"
	.incbin "baserom.gba", 0x0087cb55, 0x00000003
	.section .rom.0087e027, "ax"
	.incbin "baserom.gba", 0x0087e027, 0x00000001
	.section .rom.00880191, "ax"
	.incbin "baserom.gba", 0x00880191, 0x00000003
	.section .rom.008804e7, "ax"
	.incbin "baserom.gba", 0x008804e7, 0x00000001
	.section .rom.0088066b, "ax"
	.incbin "baserom.gba", 0x0088066b, 0x00000001
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
	.section .rom.008851f1, "ax"
	.incbin "baserom.gba", 0x008851f1, 0x00000003
	.section .rom.008862be, "ax"
	.incbin "baserom.gba", 0x008862be, 0x00000002
	.section .rom.008862c0, "ax"
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x008862c0, 0x000002e8
	.section .rom.008866fa, "ax"
	.incbin "baserom.gba", 0x008866fa, 0x00000002
	.section .rom.00888c07, "ax"
	.incbin "baserom.gba", 0x00888c07, 0x00000001
	.section .rom.008895c3, "ax"
	.incbin "baserom.gba", 0x008895c3, 0x00000001
	.section .rom.0088a706, "ax"
	.incbin "baserom.gba", 0x0088a706, 0x00000002
	.section .rom.0088b384, "ax"
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b384, 0x0000002c
	.section .rom.0088b3b0, "ax"
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b3b0, 0x0000002c
	.section .rom.0088bb99, "ax"
	.incbin "baserom.gba", 0x0088bb99, 0x00000003
	.section .rom.0088d117, "ax"
	.incbin "baserom.gba", 0x0088d117, 0x00000001
	.section .rom.0088d441, "ax"
	.incbin "baserom.gba", 0x0088d441, 0x00000003
	.section .rom.0088d96a, "ax"
	.incbin "baserom.gba", 0x0088d96a, 0x00000002
	.section .rom.0088de9a, "ax"
	.incbin "baserom.gba", 0x0088de9a, 0x00000002
	.section .rom.0088ec15, "ax"
	.incbin "baserom.gba", 0x0088ec15, 0x00000003
	.section .rom.0088f235, "ax"
	.incbin "baserom.gba", 0x0088f235, 0x00000003
	.section .rom.0088f861, "ax"
	.incbin "baserom.gba", 0x0088f861, 0x00000003
	.section .rom.0088fc6b, "ax"
	.incbin "baserom.gba", 0x0088fc6b, 0x00000001
	.section .rom.0088fec5, "ax"
	.incbin "baserom.gba", 0x0088fec5, 0x00000003
	.section .rom.00890443, "ax"
	.incbin "baserom.gba", 0x00890443, 0x00000001
	.section .rom.008906ed, "ax"
	.incbin "baserom.gba", 0x008906ed, 0x00000003
	.section .rom.00891709, "ax"
	.incbin "baserom.gba", 0x00891709, 0x00000003
	.section .rom.008941bb, "ax"
	.incbin "baserom.gba", 0x008941bb, 0x00000001
	.section .rom.00895387, "ax"
	.incbin "baserom.gba", 0x00895387, 0x00000001
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
	.section .rom.0089ae7a, "ax"
	.incbin "baserom.gba", 0x0089ae7a, 0x00000002
	.section .rom.0089bbb5, "ax"
	.incbin "baserom.gba", 0x0089bbb5, 0x00000003
	.section .rom.0089cdf1, "ax"
	.incbin "baserom.gba", 0x0089cdf1, 0x00000003
	.section .rom.0089d73b, "ax"
	.incbin "baserom.gba", 0x0089d73b, 0x00000001
	.section .rom.0089dd96, "ax"
	.incbin "baserom.gba", 0x0089dd96, 0x00000002
	.section .rom.0089e2c2, "ax"
	.incbin "baserom.gba", 0x0089e2c2, 0x00000002
	.section .rom.008a057d, "ax"
	.incbin "baserom.gba", 0x008a057d, 0x00000003
	.section .rom.008a1d11, "ax"
	.incbin "baserom.gba", 0x008a1d11, 0x00000003
	.section .rom.008a23f5, "ax"
	.incbin "baserom.gba", 0x008a23f5, 0x00000003
	.section .rom.008a23f8, "ax"
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a23f8, 0x00001f4c
	.section .rom.008a5a4d, "ax"
	.incbin "baserom.gba", 0x008a5a4d, 0x00000003
	.section .rom.008a5f3e, "ax"
	.incbin "baserom.gba", 0x008a5f3e, 0x00000002
	.section .rom.008a6586, "ax"
	.incbin "baserom.gba", 0x008a6586, 0x00000002
	.section .rom.008a71ab, "ax"
	.incbin "baserom.gba", 0x008a71ab, 0x00000001
	.section .rom.008a756d, "ax"
	.incbin "baserom.gba", 0x008a756d, 0x00000003
	.section .rom.008a7737, "ax"
	.incbin "baserom.gba", 0x008a7737, 0x00000001
	.section .rom.008a7c82, "ax"
	.incbin "baserom.gba", 0x008a7c82, 0x00000002
	.section .rom.008a7fce, "ax"
	.incbin "baserom.gba", 0x008a7fce, 0x00000002
	.section .rom.008a7fd0, "ax"
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a7fd0, 0x0000076c
	.section .rom.008a8d7f, "ax"
	.incbin "baserom.gba", 0x008a8d7f, 0x00000001
	.section .rom.008a8d80, "ax"
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a8d80, 0x000000a8
	.section .rom.008a913e, "ax"
	.incbin "baserom.gba", 0x008a913e, 0x00000002
	.section .rom.008a9ae5, "ax"
	.incbin "baserom.gba", 0x008a9ae5, 0x00000003
	.section .rom.008a9ae8, "ax"
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9ae8, 0x000002f8
	.section .rom.008aa90d, "ax"
	.incbin "baserom.gba", 0x008aa90d, 0x00000003
	.section .rom.008aa910, "ax"
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aa910, 0x00000100
	.section .rom.008ab681, "ax"
	.incbin "baserom.gba", 0x008ab681, 0x00000003
	.section .rom.008abec9, "ax"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
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
	.section .rom.008b42f2, "ax"
	.incbin "baserom.gba", 0x008b42f2, 0x00000002
	.section .rom.008b52bb, "ax"
	.incbin "baserom.gba", 0x008b52bb, 0x00000001
	.section .rom.008b5516, "ax"
	.incbin "baserom.gba", 0x008b5516, 0x00000002
	.section .rom.008b6f71, "ax"
	.incbin "baserom.gba", 0x008b6f71, 0x00000003
	.section .rom.008b74bd, "ax"
	.incbin "baserom.gba", 0x008b74bd, 0x00000003
	.section .rom.008b9246, "ax"
	.incbin "baserom.gba", 0x008b9246, 0x00000002
	.section .rom.008b94bf, "ax"
	.incbin "baserom.gba", 0x008b94bf, 0x00000001
	.section .rom.008b9986, "ax"
	.incbin "baserom.gba", 0x008b9986, 0x00000002
	.section .rom.008bb55b, "ax"
	.incbin "baserom.gba", 0x008bb55b, 0x00000001
	.section .rom.008bd159, "ax"
	.incbin "baserom.gba", 0x008bd159, 0x00000003
	.section .rom.008bd37a, "ax"
	.incbin "baserom.gba", 0x008bd37a, 0x00000002
	.section .rom.008bd7b7, "ax"
	.incbin "baserom.gba", 0x008bd7b7, 0x00000001
	.section .rom.008bd8c9, "ax"
	.incbin "baserom.gba", 0x008bd8c9, 0x00000003
	.section .rom.008bdeab, "ax"
	.incbin "baserom.gba", 0x008bdeab, 0x00000001
	.section .rom.008be349, "ax"
	.incbin "baserom.gba", 0x008be349, 0x00000003
	.section .rom.008bf05e, "ax"
	.incbin "baserom.gba", 0x008bf05e, 0x00000002
	.section .rom.008bfdde, "ax"
	.incbin "baserom.gba", 0x008bfdde, 0x00000002
	.section .rom.008c016f, "ax"
	.incbin "baserom.gba", 0x008c016f, 0x00000001
	.section .rom.008c1eb6, "ax"
	.incbin "baserom.gba", 0x008c1eb6, 0x00000002
	.section .rom.008c2c8d, "ax"
	.incbin "baserom.gba", 0x008c2c8d, 0x00000003
	.section .rom.008c34ba, "ax"
	.incbin "baserom.gba", 0x008c34ba, 0x00000002
	.section .rom.008c3a66, "ax"
	.incbin "baserom.gba", 0x008c3a66, 0x00000002
	.section .rom.008c3c23, "ax"
	.incbin "baserom.gba", 0x008c3c23, 0x00000001
	.section .rom.008c497b, "ax"
	.incbin "baserom.gba", 0x008c497b, 0x00000001
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
	.section .rom.008caaa1, "ax"
	.incbin "baserom.gba", 0x008caaa1, 0x00000003
	.section .rom.008cb86f, "ax"
	.incbin "baserom.gba", 0x008cb86f, 0x00000001
	.section .rom.008ccad9, "ax"
	.incbin "baserom.gba", 0x008ccad9, 0x00000003
	.section .rom.008cd033, "ax"
	.incbin "baserom.gba", 0x008cd033, 0x00000001
	.section .rom.008ce91d, "ax"
	.incbin "baserom.gba", 0x008ce91d, 0x00000003
	.section .rom.008cf1be, "ax"
	.incbin "baserom.gba", 0x008cf1be, 0x00000002
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
	.section .rom.008d44ce, "ax"
	.incbin "baserom.gba", 0x008d44ce, 0x00000002
	.section .rom.008d4851, "ax"
	.incbin "baserom.gba", 0x008d4851, 0x00000003
	.section .rom.008d5501, "ax"
	.incbin "baserom.gba", 0x008d5501, 0x00000003
	.section .rom.008d6049, "ax"
	.incbin "baserom.gba", 0x008d6049, 0x00000003
	.section .rom.008d61e2, "ax"
	.incbin "baserom.gba", 0x008d61e2, 0x00000002
	.section .rom.008d6a6f, "ax"
	.incbin "baserom.gba", 0x008d6a6f, 0x00000001
	.section .rom.008d763e, "ax"
	.incbin "baserom.gba", 0x008d763e, 0x00000002
	.section .rom.008d7b56, "ax"
	.incbin "baserom.gba", 0x008d7b56, 0x00000002
	.section .rom.008d817a, "ax"
	.incbin "baserom.gba", 0x008d817a, 0x00000002
	.section .rom.008d859d, "ax"
	.incbin "baserom.gba", 0x008d859d, 0x00000003
	.section .rom.008d8837, "ax"
	.incbin "baserom.gba", 0x008d8837, 0x00000001
	.section .rom.008da1d3, "ax"
	.incbin "baserom.gba", 0x008da1d3, 0x00000001
	.section .rom.008dbf4a, "ax"
	.incbin "baserom.gba", 0x008dbf4a, 0x00000002
	.section .rom.008ddebf, "ax"
	.incbin "baserom.gba", 0x008ddebf, 0x00000001
	.section .rom.008de38f, "ax"
	.incbin "baserom.gba", 0x008de38f, 0x00000001
	.section .rom.008dea21, "ax"
	.incbin "baserom.gba", 0x008dea21, 0x00000003
	.section .rom.008df457, "ax"
	.incbin "baserom.gba", 0x008df457, 0x00000001
	.section .rom.008e0829, "ax"
	.incbin "baserom.gba", 0x008e0829, 0x00000003
	.section .rom.008e12fa, "ax"
	.incbin "baserom.gba", 0x008e12fa, 0x00000002
	.section .rom.008e1e37, "ax"
	.incbin "baserom.gba", 0x008e1e37, 0x00000001
	.section .rom.008e2477, "ax"
	.incbin "baserom.gba", 0x008e2477, 0x00000001
	.section .rom.008e39fe, "ax"
	.incbin "baserom.gba", 0x008e39fe, 0x00000002
	.section .rom.008e3a61, "ax"
	.incbin "baserom.gba", 0x008e3a61, 0x00000003
	.section .rom.008e3dcf, "ax"
	.incbin "baserom.gba", 0x008e3dcf, 0x00000001
	.section .rom.008e436d, "ax"
	.incbin "baserom.gba", 0x008e436d, 0x00000003
	.section .rom.008e4370, "ax"
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e4370, 0x00000204
	.section .rom.008e48ad, "ax"
	.incbin "baserom.gba", 0x008e48ad, 0x00000003
	.section .rom.008e58c6, "ax"
	.incbin "baserom.gba", 0x008e58c6, 0x00000002
	.section .rom.008e8ed2, "ax"
	.incbin "baserom.gba", 0x008e8ed2, 0x00000002
	.section .rom.008e9675, "ax"
	.incbin "baserom.gba", 0x008e9675, 0x00000003
	.section .rom.008eaa07, "ax"
	.incbin "baserom.gba", 0x008eaa07, 0x00000001
	.section .rom.008eae37, "ax"
	.incbin "baserom.gba", 0x008eae37, 0x00000001
	.section .rom.008ecc2d, "ax"
	.incbin "baserom.gba", 0x008ecc2d, 0x00000003
	.section .rom.008ed542, "ax"
	.incbin "baserom.gba", 0x008ed542, 0x00000002
	.section .rom.008ef076, "ax"
	.incbin "baserom.gba", 0x008ef076, 0x00000002
	.section .rom.008f00c5, "ax"
	.incbin "baserom.gba", 0x008f00c5, 0x00000003
	.section .rom.008f116d, "ax"
	.incbin "baserom.gba", 0x008f116d, 0x00000003
	.section .rom.0093bf95, "ax"
	.incbin "baserom.gba", 0x0093bf95, 0x00000003
	.section .rom.0093c1ad, "ax"
	.incbin "baserom.gba", 0x0093c1ad, 0x00000003
	.section .rom.0093c301, "ax"
	.incbin "baserom.gba", 0x0093c301, 0x00000003
	.section .rom.0093c7b9, "ax"
	.incbin "baserom.gba", 0x0093c7b9, 0x00000003
	.section .rom.0093df26, "ax"
	.incbin "baserom.gba", 0x0093df26, 0x00000002
	.section .rom.0093f45d, "ax"
	.incbin "baserom.gba", 0x0093f45d, 0x00000003
	.section .rom.009412b3, "ax"
	.incbin "baserom.gba", 0x009412b3, 0x00000001
	.section .rom.009433c7, "ax"
	.incbin "baserom.gba", 0x009433c7, 0x00000001
	.section .rom.00943b41, "ax"
	.incbin "baserom.gba", 0x00943b41, 0x00000003
	.section .rom.009461bf, "ax"
	.incbin "baserom.gba", 0x009461bf, 0x00000001
	.section .rom.00946bc7, "ax"
	.incbin "baserom.gba", 0x00946bc7, 0x00000001
	.section .rom.009498e6, "ax"
	.incbin "baserom.gba", 0x009498e6, 0x00000002
	.section .rom.00949abe, "ax"
	.incbin "baserom.gba", 0x00949abe, 0x00000002
	.section .rom.00949f1d, "ax"
	.incbin "baserom.gba", 0x00949f1d, 0x00000003
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
	.section .rom.0094fa46, "ax"
	.incbin "baserom.gba", 0x0094fa46, 0x00000002
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
	.section .rom.009537b8, "ax"
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x009537b8, 0x000002f8
	.section .rom.00953b89, "ax"
	.incbin "baserom.gba", 0x00953b89, 0x00000003
	.section .rom.00953b8c, "ax"
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
	.section .rom.009678ac, "ax"
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x009678ac, 0x00000100
	.section .rom.009679ac, "ax"
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x009679ac, 0x000004c8
	.section .rom.00967e74, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x00967e74, 0x00000268
	.section .rom.009680dc, "ax"
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
	.section .rom.00969358, "ax"
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969358, 0x0000024c
	.section .rom.00969686, "ax"
	.incbin "baserom.gba", 0x00969686, 0x00000002
	.section .rom.00969741, "ax"
	.incbin "baserom.gba", 0x00969741, 0x00000003
	.section .rom.00969916, "ax"
	.incbin "baserom.gba", 0x00969916, 0x00000002
	.section .rom.009699b0, "ax"
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
	.section .rom.0096a594, "ax"
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096a594, 0x000000b0
	.section .rom.0096a644, "ax"
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096a644, 0x00000560
	.section .rom.0096aba4, "ax"
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096aba4, 0x00000074
	.section .rom.0096ac18, "ax"
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096ac18, 0x00000048
	.section .rom.0096ac60, "ax"
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096ac60, 0x00000050
	.section .rom.0096acb0, "ax"
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096acb0, 0x00000050
	.section .rom.0096ad00, "ax"
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096ad00, 0x00000058
	.section .rom.0096ad58, "ax"
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096ad58, 0x00000058
	.section .rom.0096adb0, "ax"
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096adb0, 0x00000040
	.section .rom.0096adf0, "ax"
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096adf0, 0x00000044
	.section .rom.0096ae34, "ax"
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
	.section .rom.009bfdbf, "ax"
	.incbin "baserom.gba", 0x009bfdbf, 0x00000001
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
	.section .rom.00a792df, "ax"
	.incbin "baserom.gba", 0x00a792df, 0x00000001
	.section .rom.00a7942e, "ax"
	.incbin "baserom.gba", 0x00a7942e, 0x00000002
	.section .rom.00a7956f, "ax"
	.incbin "baserom.gba", 0x00a7956f, 0x00000001
	.section .rom.00a796af, "ax"
	.incbin "baserom.gba", 0x00a796af, 0x00000001
	.section .rom.00a797ef, "ax"
	.incbin "baserom.gba", 0x00a797ef, 0x00000001
	.section .rom.00a7aa4d, "ax"
	.incbin "baserom.gba", 0x00a7aa4d, 0x00000003
	.section .rom.00a7ac1e, "ax"
	.incbin "baserom.gba", 0x00a7ac1e, 0x00000002
	.section .rom.00a7ccbf, "ax"
	.incbin "baserom.gba", 0x00a7ccbf, 0x00000001
	.section .rom.00a7ff21, "ax"
	.incbin "baserom.gba", 0x00a7ff21, 0x00000003
	.section .rom.00a85793, "ax"
	.incbin "baserom.gba", 0x00a85793, 0x00000001
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
	.section .rom.00b1a47e, "ax"
	.incbin "baserom.gba", 0x00b1a47e, 0x00000002
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
	.section .rom.00c2845d, "ax"
	.incbin "baserom.gba", 0x00c2845d, 0x00000003
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
	.section .rom.00d2a6c9, "ax"
	.incbin "baserom.gba", 0x00d2a6c9, 0x00000003
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
	.section .rom.00d4927a, "ax"
	.incbin "baserom.gba", 0x00d4927a, 0x00000002
	.section .rom.00d4b5df, "ax"
	.incbin "baserom.gba", 0x00d4b5df, 0x00000001
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
	.section .rom.00e69802, "ax"
	.incbin "baserom.gba", 0x00e69802, 0x00000002
	.section .rom.00e6adb3, "ax"
	.incbin "baserom.gba", 0x00e6adb3, 0x00000001
	.section .rom.00e6af66, "ax"
	.incbin "baserom.gba", 0x00e6af66, 0x00000002
	.section .rom.00e6d1a5, "ax"
	.incbin "baserom.gba", 0x00e6d1a5, 0x00000003
	.section .rom.00e6dccb, "ax"
	.incbin "baserom.gba", 0x00e6dccb, 0x00000001
	.section .rom.00e78f29, "ax"
	.incbin "baserom.gba", 0x00e78f29, 0x00000003
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
	.section .rom.00eb6269, "ax"
	.incbin "baserom.gba", 0x00eb6269, 0x00000003
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
	.section .rom.00ebd58b, "ax"
	.incbin "baserom.gba", 0x00ebd58b, 0x00000001
	.section .rom.00ebd6da, "ax"
	.incbin "baserom.gba", 0x00ebd6da, 0x00000002
	.section .rom.00ebd81b, "ax"
	.incbin "baserom.gba", 0x00ebd81b, 0x00000001
	.section .rom.00ebd95b, "ax"
	.incbin "baserom.gba", 0x00ebd95b, 0x00000001
	.section .rom.00ebda9b, "ax"
	.incbin "baserom.gba", 0x00ebda9b, 0x00000001
	.section .rom.00ebe1b1, "ax"
	.incbin "baserom.gba", 0x00ebe1b1, 0x00000003
	.section .rom.00ebe1bf, "ax"
	.incbin "baserom.gba", 0x00ebe1bf, 0x00000001
	.section .rom.00ebe30e, "ax"
	.incbin "baserom.gba", 0x00ebe30e, 0x00000002
	.section .rom.00ebe44f, "ax"
	.incbin "baserom.gba", 0x00ebe44f, 0x00000001
	.section .rom.00ebe58f, "ax"
	.incbin "baserom.gba", 0x00ebe58f, 0x00000001
	.section .rom.00ebe6cf, "ax"
	.incbin "baserom.gba", 0x00ebe6cf, 0x00000001
	.section .rom.00ebf291, "ax"
	.incbin "baserom.gba", 0x00ebf291, 0x00000003
	.section .rom.00ebf29f, "ax"
	.incbin "baserom.gba", 0x00ebf29f, 0x00000001
	.section .rom.00ebf3ee, "ax"
	.incbin "baserom.gba", 0x00ebf3ee, 0x00000002
	.section .rom.00ebf52f, "ax"
	.incbin "baserom.gba", 0x00ebf52f, 0x00000001
	.section .rom.00ebf66f, "ax"
	.incbin "baserom.gba", 0x00ebf66f, 0x00000001
	.section .rom.00ebf7af, "ax"
	.incbin "baserom.gba", 0x00ebf7af, 0x00000001
	.section .rom.00ebfd7d, "ax"
	.incbin "baserom.gba", 0x00ebfd7d, 0x00000003
	.section .rom.00ebfd8b, "ax"
	.incbin "baserom.gba", 0x00ebfd8b, 0x00000001
	.section .rom.00ebfeda, "ax"
	.incbin "baserom.gba", 0x00ebfeda, 0x00000002
	.section .rom.00ec001b, "ax"
	.incbin "baserom.gba", 0x00ec001b, 0x00000001
	.section .rom.00ec015b, "ax"
	.incbin "baserom.gba", 0x00ec015b, 0x00000001
	.section .rom.00ec029b, "ax"
	.incbin "baserom.gba", 0x00ec029b, 0x00000001
	.section .rom.00ec0c19, "ax"
	.incbin "baserom.gba", 0x00ec0c19, 0x00000003
	.section .rom.00ec0c27, "ax"
	.incbin "baserom.gba", 0x00ec0c27, 0x00000001
	.section .rom.00ec0d76, "ax"
	.incbin "baserom.gba", 0x00ec0d76, 0x00000002
	.section .rom.00ec0eb7, "ax"
	.incbin "baserom.gba", 0x00ec0eb7, 0x00000001
	.section .rom.00ec0ff7, "ax"
	.incbin "baserom.gba", 0x00ec0ff7, 0x00000001
	.section .rom.00ec1137, "ax"
	.incbin "baserom.gba", 0x00ec1137, 0x00000001
	.section .rom.00ec1561, "ax"
	.incbin "baserom.gba", 0x00ec1561, 0x00000003
	.section .rom.00ec156f, "ax"
	.incbin "baserom.gba", 0x00ec156f, 0x00000001
	.section .rom.00ec16be, "ax"
	.incbin "baserom.gba", 0x00ec16be, 0x00000002
	.section .rom.00ec17ff, "ax"
	.incbin "baserom.gba", 0x00ec17ff, 0x00000001
	.section .rom.00ec193f, "ax"
	.incbin "baserom.gba", 0x00ec193f, 0x00000001
	.section .rom.00ec1a7f, "ax"
	.incbin "baserom.gba", 0x00ec1a7f, 0x00000001
	.section .rom.00ec20a9, "ax"
	.incbin "baserom.gba", 0x00ec20a9, 0x00000003
	.section .rom.00ec20b7, "ax"
	.incbin "baserom.gba", 0x00ec20b7, 0x00000001
	.section .rom.00ec2206, "ax"
	.incbin "baserom.gba", 0x00ec2206, 0x00000002
	.section .rom.00ec2347, "ax"
	.incbin "baserom.gba", 0x00ec2347, 0x00000001
	.section .rom.00ec2487, "ax"
	.incbin "baserom.gba", 0x00ec2487, 0x00000001
	.section .rom.00ec25c7, "ax"
	.incbin "baserom.gba", 0x00ec25c7, 0x00000001
	.section .rom.00ec2cf1, "ax"
	.incbin "baserom.gba", 0x00ec2cf1, 0x00000003
	.section .rom.00ec2cff, "ax"
	.incbin "baserom.gba", 0x00ec2cff, 0x00000001
	.section .rom.00ec2e4e, "ax"
	.incbin "baserom.gba", 0x00ec2e4e, 0x00000002
	.section .rom.00ec2f8f, "ax"
	.incbin "baserom.gba", 0x00ec2f8f, 0x00000001
	.section .rom.00ec30cf, "ax"
	.incbin "baserom.gba", 0x00ec30cf, 0x00000001
	.section .rom.00ec320f, "ax"
	.incbin "baserom.gba", 0x00ec320f, 0x00000001
