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
	.incbin "baserom.gba", 0x000397e0, 0x000006b0
	.section .rom.00039e90, "ax"
	.global UiWork_ProcessDirectWork
	.type UiWork_ProcessDirectWork, %function
	.thumb_func
UiWork_ProcessDirectWork:
	.incbin "baserom.gba", 0x00039e90, 0x00000184
	.section .rom.0003a270, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003a270, 0x00000108
	.section .rom.0003a406, "ax"
	.incbin "baserom.gba", 0x0003a406, 0x00000002
	.section .rom.0003a50c, "ax"
	.global UiMessage_Show
	.type UiMessage_Show, %function
	.thumb_func
UiMessage_Show:
	.incbin "baserom.gba", 0x0003a50c, 0x00000094
	.section .rom.0003a5cc, "ax"
	.global Func_0803a60c
	.type Func_0803a60c, %function
	.thumb_func
Func_0803a60c:
	.incbin "baserom.gba", 0x0003a5cc, 0x0000005c
	.section .rom.0003a65c, "ax"
	.global UiText_OpenMessageWindow
	.type UiText_OpenMessageWindow, %function
	.thumb_func
UiText_OpenMessageWindow:
	.incbin "baserom.gba", 0x0003a65c, 0x00000110
	.section .rom.0003a8bc, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a8bc, 0x000001a8
	.section .rom.0003aa8a, "ax"
	.incbin "baserom.gba", 0x0003aa8a, 0x00000002
	.section .rom.0003aa8c, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa8c, 0x000001f0
	.section .rom.0003ac7c, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x0003ac7c, 0x00000120
	.section .rom.0003ad9c, "ax"
	.global UiText_FormatNumber
	.type UiText_FormatNumber, %function
	.thumb_func
UiText_FormatNumber:
	.incbin "baserom.gba", 0x0003ad9c, 0x00000230
	.section .rom.0003afcc, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003afcc, 0x000008b4
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
	.incbin "baserom.gba", 0x0003b930, 0x00000734
	.section .rom.0003c064, "ax"
	.global Func_0803c068
	.type Func_0803c068, %function
	.thumb_func
Func_0803c068:
	.incbin "baserom.gba", 0x0003c064, 0x00000108
	.section .rom.0003c194, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c194, 0x000001e0
	.section .rom.0003c408, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c408, 0x0000013c
	.section .rom.0003c544, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c544, 0x00000464
	.section .rom.0003c9b8, "ax"
	.global UiText_CopyMessageString
	.type UiText_CopyMessageString, %function
	.thumb_func
UiText_CopyMessageString:
	.incbin "baserom.gba", 0x0003c9b8, 0x00000064
	.section .rom.0003ca1c, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x0003ca1c, 0x00000144
	.section .rom.0003cb6e, "ax"
	.incbin "baserom.gba", 0x0003cb6e, 0x00000002
	.section .rom.0003cbfa, "ax"
	.incbin "baserom.gba", 0x0003cbfa, 0x00000002
	.section .rom.0003cd5c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cd5c, 0x00000114
	.section .rom.0003ceb8, "ax"
	.balign 4
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003ceb8, 0x000000fc
	.section .rom.0003cfc0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cfc0, 0x00000034
	.section .rom.0003d024, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003d024, 0x00000050
	.section .rom.0003d074, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d074, 0x00000158
	.section .rom.0003d218, "ax"
	.incbin "baserom.gba", 0x0003d218, 0x0000010c
	.section .rom.0003d324, "ax"
	.global UiWork_SetTwoEntriesTo999
	.type UiWork_SetTwoEntriesTo999, %function
	.thumb_func
UiWork_SetTwoEntriesTo999:
	.incbin "baserom.gba", 0x0003d324, 0x00000020
	.section .rom.0003d538, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d538, 0x000000b0
	.section .rom.0003d618, "ax"
	.balign 4
	.global Func_0803d5c4
	.type Func_0803d5c4, %function
	.thumb_func
Func_0803d5c4:
	.incbin "baserom.gba", 0x0003d618, 0x000000bc
	.section .rom.0003d6d4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6d4, 0x0000022c
	.section .rom.0003d97e, "ax"
	.incbin "baserom.gba", 0x0003d97e, 0x00000002
	.section .rom.0003d980, "ax"
	.global Func_0803d92c
	.type Func_0803d92c, %function
	.thumb_func
Func_0803d92c:
	.incbin "baserom.gba", 0x0003d980, 0x00000060
	.section .rom.0003da0e, "ax"
	.incbin "baserom.gba", 0x0003da0e, 0x00000002
	.section .rom.0003da10, "ax"
	.global UiIcon_BuildAbilityIconTiles
	.type UiIcon_BuildAbilityIconTiles, %function
	.thumb_func
UiIcon_BuildAbilityIconTiles:
	.incbin "baserom.gba", 0x0003da10, 0x000000bc
	.section .rom.0003dc70, "ax"
	.balign 4
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dc70, 0x00000108
	.section .rom.0003ddec, "ax"
	.balign 4
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003ddec, 0x00000110
	.section .rom.0003df78, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003df78, 0x00000684
	.section .rom.0003e81c, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e81c, 0x00000124
	.section .rom.0003eaa8, "ax"
	.incbin "baserom.gba", 0x0003eaa8, 0x00000188
	.section .rom.0003ec30, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ec30, 0x000001bc
	.section .rom.0003edec, "ax"
	.global Menu_StepLeft
	.type Menu_StepLeft, %function
	.thumb_func
Menu_StepLeft:
	.incbin "baserom.gba", 0x0003edec, 0x000001b0
	.section .rom.0003f058, "ax"
	.incbin "baserom.gba", 0x0003f058, 0x000001d0
	.section .rom.0003f338, "ax"
	.incbin "baserom.gba", 0x0003f338, 0x000000e4
	.section .rom.0003f41c, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f41c, 0x00000248
	.section .rom.0003f678, "ax"
	.balign 4
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f678, 0x00000074
	.section .rom.0003f7cc, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0003f7cc, 0x00000088
	.section .rom.0003f854, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0003f854, 0x00000010
	.section .rom.0003fa14, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003fa14, 0x000001a0
	.section .rom.0003fc8c, "ax"
	.incbin "baserom.gba", 0x0003fc8c, 0x000000dc
	.section .rom.0003fd7c, "ax"
	.incbin "baserom.gba", 0x0003fd7c, 0x000003c0
	.section .rom.0004013c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004013c, 0x000004c4
	.section .rom.0004067c, "ax"
	.incbin "baserom.gba", 0x0004067c, 0x00000364
	.section .rom.00040a44, "ax"
	.incbin "baserom.gba", 0x00040a44, 0x00000270
	.section .rom.00040dc4, "ax"
	.incbin "baserom.gba", 0x00040dc4, 0x000000f4
	.section .rom.000410bc, "ax"
	.incbin "baserom.gba", 0x000410bc, 0x0000019c
	.section .rom.000412e0, "ax"
	.incbin "baserom.gba", 0x000412e0, 0x00000440
	.section .rom.00041720, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x00041720, 0x000003f0
	.section .rom.00041b10, "ax"
	.global UiWindow_ClearTileAttributesInRect
	.type UiWindow_ClearTileAttributesInRect, %function
	.thumb_func
UiWindow_ClearTileAttributesInRect:
	.incbin "baserom.gba", 0x00041b10, 0x000000ac
	.section .rom.00041bbc, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041bbc, 0x000000a4
	.section .rom.00041ca8, "ax"
	.balign 4
	.global UiWindow_DrawDividerLine
	.type UiWindow_DrawDividerLine, %function
	.thumb_func
UiWindow_DrawDividerLine:
	.incbin "baserom.gba", 0x00041ca8, 0x0000031c
	.section .rom.00041fc4, "ax"
	.global Func_08041f70
	.type Func_08041f70, %function
	.thumb_func
Func_08041f70:
	.incbin "baserom.gba", 0x00041fc4, 0x00000020
	.section .rom.00041ff8, "ax"
	.balign 4
	.global UiText_DrawResource
	.type UiText_DrawResource, %function
	.thumb_func
UiText_DrawResource:
	.incbin "baserom.gba", 0x00041ff8, 0x0000006c
	.section .rom.00042064, "ax"
	.global UiText_DrawCharacterAtOffset
	.type UiText_DrawCharacterAtOffset, %function
	.thumb_func
UiText_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042064, 0x00000098
	.section .rom.00042150, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042150, 0x0000008c
	.section .rom.00042238, "ax"
	.global UiText_DrawNumber
	.type UiText_DrawNumber, %function
	.thumb_func
UiText_DrawNumber:
	.incbin "baserom.gba", 0x00042238, 0x00000030
	.section .rom.00042268, "ax"
	.global UiText_DrawNumberAtOffset
	.type UiText_DrawNumberAtOffset, %function
	.thumb_func
UiText_DrawNumberAtOffset:
	.incbin "baserom.gba", 0x00042268, 0x00000030
	.section .rom.00042298, "ax"
	.global UiText_DrawNumberInWindow
	.type UiText_DrawNumberInWindow, %function
	.thumb_func
UiText_DrawNumberInWindow:
	.incbin "baserom.gba", 0x00042298, 0x000000d0
	.section .rom.00042368, "ax"
	.global RenderOutput_Create
	.type RenderOutput_Create, %function
	.thumb_func
RenderOutput_Create:
	.incbin "baserom.gba", 0x00042368, 0x00000088
	.section .rom.000424a4, "ax"
	.global CreateSideObject
	.type CreateSideObject, %function
	.thumb_func
CreateSideObject:
	.incbin "baserom.gba", 0x000424a4, 0x000000b8
	.section .rom.0004255c, "ax"
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x0004255c, 0x00000064
	.section .rom.000425ce, "ax"
	.incbin "baserom.gba", 0x000425ce, 0x00000002
	.section .rom.000425dc, "ax"
	.balign 4
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x000425dc, 0x00000074
	.section .rom.00042734, "ax"
	.incbin "baserom.gba", 0x00042734, 0x0000029c
	.section .rom.000429d0, "ax"
	.global PtyStDraw
	.type PtyStDraw, %function
	.thumb_func
PtyStDraw:
	.incbin "baserom.gba", 0x000429d0, 0x00000430
	.section .rom.00042e18, "ax"
	.incbin "baserom.gba", 0x00042e18, 0x00000064
	.section .rom.00042f64, "ax"
	.global Func_08042f10
	.type Func_08042f10, %function
	.thumb_func
Func_08042f10:
	.incbin "baserom.gba", 0x00042f64, 0x000000e0
	.section .rom.00043044, "ax"
	.global Func_08042ff0
	.type Func_08042ff0, %function
	.thumb_func
Func_08042ff0:
	.incbin "baserom.gba", 0x00043044, 0x000000d4
	.section .rom.00043118, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043118, 0x000000ec
	.section .rom.00043204, "ax"
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x00043204, 0x00000080
	.section .rom.000432f8, "ax"
	.balign 4
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000432f8, 0x000000b4
	.section .rom.000433ac, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x000433ac, 0x00000194
	.section .rom.00043588, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043588, 0x000000c8
	.section .rom.000436e4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000436e4, 0x000000e4
	.section .rom.00043ab8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ab8, 0x000000d0
	.section .rom.00043bc4, "ax"
	.global Func_08043b70
	.type Func_08043b70, %function
	.thumb_func
Func_08043b70:
	.incbin "baserom.gba", 0x00043bc4, 0x0000007c
	.section .rom.00043d2c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043d2c, 0x0000066c
	.section .rom.0004439c, "ax"
	.incbin "baserom.gba", 0x0004439c, 0x00000090
	.section .rom.000445ac, "ax"
	.balign 4
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x000445ac, 0x0000051c
	.section .rom.00044acc, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044acc, 0x00000128
	.section .rom.00044c0c, "ax"
	.global Func_08044b98
	.type Func_08044b98, %function
	.thumb_func
Func_08044b98:
	.incbin "baserom.gba", 0x00044c0c, 0x000000e8
	.section .rom.00044cf4, "ax"
	.global Func_08044c80
	.type Func_08044c80, %function
	.thumb_func
Func_08044c80:
	.incbin "baserom.gba", 0x00044cf4, 0x00000148
	.section .rom.00044e3c, "ax"
	.global Func_08044dc8
	.type Func_08044dc8, %function
	.thumb_func
Func_08044dc8:
	.incbin "baserom.gba", 0x00044e3c, 0x00000184
	.section .rom.0004508c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004508c, 0x00000030
	.section .rom.00045170, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x00045170, 0x000000a0
	.section .rom.00045230, "ax"
	.incbin "baserom.gba", 0x00045230, 0x00000090
	.section .rom.0004532e, "ax"
	.incbin "baserom.gba", 0x0004532e, 0x00000002
	.section .rom.00045330, "ax"
	.global Graphics_MergeShiftedTileRows
	.type Graphics_MergeShiftedTileRows, %function
	.thumb_func
Graphics_MergeShiftedTileRows:
	.incbin "baserom.gba", 0x00045330, 0x00000074
	.section .rom.0004559a, "ax"
	.incbin "baserom.gba", 0x0004559a, 0x00000002
	.section .rom.000455d8, "ax"
	.incbin "baserom.gba", 0x000455d8, 0x0000004c
	.section .rom.0004576c, "ax"
	.incbin "baserom.gba", 0x0004576c, 0x00000070
	.section .rom.000457f2, "ax"
	.incbin "baserom.gba", 0x000457f2, 0x00000002
	.section .rom.00045842, "ax"
	.incbin "baserom.gba", 0x00045842, 0x00000966
	.section .rom.000462f8, "ax"
	.incbin "baserom.gba", 0x000462f8, 0x00000150
	.section .rom.00046486, "ax"
	.incbin "baserom.gba", 0x00046486, 0x00003656
	.section .rom.00049b2a, "ax"
	.incbin "baserom.gba", 0x00049b2a, 0x00000002
	.section .rom.00049b58, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00049b58, 0x00001d54
	.section .rom.0004b9cc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004b9cc, 0x000000bc
	.section .rom.0004bac2, "ax"
	.incbin "baserom.gba", 0x0004bac2, 0x000002f2
	.section .rom.0004bddc, "ax"
	.incbin "baserom.gba", 0x0004bddc, 0x00000038
	.section .rom.0004be14, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004be14, 0x000011a0
	.section .rom.0004cfb4, "ax"
	.global Func_0804cda8
	.type Func_0804cda8, %function
	.thumb_func
Func_0804cda8:
	.incbin "baserom.gba", 0x0004cfb4, 0x00000334
	.section .rom.0004d378, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d378, 0x00000120
	.section .rom.0004d498, "ax"
	.global Menu_SelectResource
	.type Menu_SelectResource, %function
	.thumb_func
Menu_SelectResource:
	.incbin "baserom.gba", 0x0004d498, 0x000000b8
	.section .rom.0004d5f4, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5f4, 0x0000009c
	.section .rom.0004d690, "ax"
	.global Menu_LayoutResourceEntries
	.type Menu_LayoutResourceEntries, %function
	.thumb_func
Menu_LayoutResourceEntries:
	.incbin "baserom.gba", 0x0004d690, 0x00000074
	.section .rom.0004d7b8, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x0004d7b8, 0x000001a8
	.section .rom.0004da08, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da08, 0x0000016c
	.section .rom.0004dcc6, "ax"
	.incbin "baserom.gba", 0x0004dcc6, 0x00000002
	.section .rom.0004dd80, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x0004dd80, 0x00000254
	.section .rom.0004e0ac, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0ac, 0x000000c4
	.section .rom.0004e2da, "ax"
	.incbin "baserom.gba", 0x0004e2da, 0x00000002
	.section .rom.0004e2dc, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2dc, 0x0000007c
	.section .rom.0004e3e4, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3e4, 0x0000021c
	.section .rom.0004e600, "ax"
	.global Func_0804e3f4
	.type Func_0804e3f4, %function
	.thumb_func
Func_0804e3f4:
	.incbin "baserom.gba", 0x0004e600, 0x00000190
	.section .rom.0005c3d4, "ax"
	.incbin "baserom.gba", 0x0005c3d4, 0x000000bc
	.section .rom.0005c518, "ax"
	.incbin "baserom.gba", 0x0005c518, 0x00000010
	.section .rom.0005f7e8, "ax"
	.incbin "baserom.gba", 0x0005f7e8, 0x00000101
	.section .rom.0005f8e9, "ax"
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x0005f8e9, 0x000000bb
	.section .rom.0005f9a4, "ax"
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x0005f9a4, 0x00000032
	.section .rom.0005f9d6, "ax"
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x0005f9d6, 0x0000007a
	.section .rom.0005fb20, "ax"
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x0005fb20, 0x00000008
	.section .rom.0005fbc8, "ax"
	.incbin "baserom.gba", 0x0005fbc8, 0x0000005e
	.section .rom.0005fc26, "ax"
	.global Menu_TopEntryCommandByPosition
	.type Menu_TopEntryCommandByPosition, %function
	.thumb_func
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0005fc26, 0x0000000c
	.section .rom.0005fc32, "ax"
	.global Menu_TopEntryPositionByCommand
	.type Menu_TopEntryPositionByCommand, %function
	.thumb_func
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x0005fc32, 0x00000026
	.section .rom.0005fc58, "ax"
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0005fc58, 0x0000003c
	.section .rom.000b7264, "ax"
	.incbin "baserom.gba", 0x000b7264, 0x0000021c
	.section .rom.000b74c0, "ax"
	.incbin "baserom.gba", 0x000b74c0, 0x000002b4
	.section .rom.000b7774, "ax"
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000b7774, 0x00000004
	.section .rom.000ba0b0, "ax"
	.global Func_080ad0b0
	.type Func_080ad0b0, %function
	.thumb_func
Func_080ad0b0:
	.incbin "baserom.gba", 0x000ba0b0, 0x00000008
	.section .rom.000ba0b8, "ax"
	.global Func_080ad0b8
	.type Func_080ad0b8, %function
	.thumb_func
Func_080ad0b8:
	.incbin "baserom.gba", 0x000ba0b8, 0x00000008
	.section .rom.000ba3f6, "ax"
	.incbin "baserom.gba", 0x000ba3f6, 0x00000002
	.section .rom.000ba3f8, "ax"
	.global UnitCalc
	.type UnitCalc, %function
	.thumb_func
UnitCalc:
	.incbin "baserom.gba", 0x000ba3f8, 0x000007f4
	.section .rom.000babec, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000babec, 0x000000a4
	.section .rom.000badf0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000badf0, 0x000002ec
	.section .rom.000bb358, "ax"
	.global Func_080ae358
	.type Func_080ae358, %function
	.thumb_func
Func_080ae358:
	.incbin "baserom.gba", 0x000bb358, 0x000000a4
	.section .rom.000bb410, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000bb410, 0x000001ec
	.section .rom.000bb5fc, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000bb5fc, 0x000000cc
	.section .rom.000bb6e4, "ax"
	.global Func_080ae6e4
	.type Func_080ae6e4, %function
	.thumb_func
Func_080ae6e4:
	.incbin "baserom.gba", 0x000bb6e4, 0x0000014c
	.section .rom.000bb868, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000bb868, 0x000001c8
	.section .rom.000bbc66, "ax"
	.incbin "baserom.gba", 0x000bbc66, 0x00000002
	.section .rom.000bbc68, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000bbc68, 0x0000003c
	.section .rom.000bbf34, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x000bbf34, 0x000000d4
	.section .rom.000bc0e2, "ax"
	.incbin "baserom.gba", 0x000bc0e2, 0x00000002
	.section .rom.000bc0e4, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.incbin "baserom.gba", 0x000bc0e4, 0x00000064
	.section .rom.000bc148, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000bc148, 0x00000080
	.section .rom.000bc4e4, "ax"
	.balign 4
	.global Owner_UpdateClassActions
	.type Owner_UpdateClassActions, %function
	.thumb_func
Owner_UpdateClassActions:
	.incbin "baserom.gba", 0x000bc4e4, 0x00000210
	.section .rom.000bc7ac, "ax"
	.global Func_080af7ac
	.type Func_080af7ac, %function
	.thumb_func
Func_080af7ac:
	.incbin "baserom.gba", 0x000bc7ac, 0x00000120
	.section .rom.000bc91c, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000bc91c, 0x00000264
	.section .rom.000bcb80, "ax"
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000bcb80, 0x00000034
	.section .rom.000bcbec, "ax"
	.global Func_080afbec
	.type Func_080afbec, %function
	.thumb_func
Func_080afbec:
	.incbin "baserom.gba", 0x000bcbec, 0x000001d0
	.section .rom.000bcf28, "ax"
	.global Trade_ListFlaggedEntries
	.type Trade_ListFlaggedEntries, %function
	.thumb_func
Trade_ListFlaggedEntries:
	.incbin "baserom.gba", 0x000bcf28, 0x00000054
	.section .rom.000bcfac, "ax"
	.balign 4
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000bcfac, 0x0000007c
	.section .rom.000bd05a, "ax"
	.incbin "baserom.gba", 0x000bd05a, 0x00000002
	.section .rom.000bd05c, "ax"
	.global Owner_GetDefaultElement
	.type Owner_GetDefaultElement, %function
	.thumb_func
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000bd05c, 0x00000028
	.section .rom.000bd084, "ax"
	.global Owner_CalculateBaseElements
	.type Owner_CalculateBaseElements, %function
	.thumb_func
Owner_CalculateBaseElements:
	.incbin "baserom.gba", 0x000bd084, 0x000000c0
	.section .rom.000bd144, "ax"
	.global Owner_DetermineClass
	.type Owner_DetermineClass, %function
	.thumb_func
Owner_DetermineClass:
	.incbin "baserom.gba", 0x000bd144, 0x00000144
	.section .rom.000bd514, "ax"
	.incbin "baserom.gba", 0x000bd514, 0x00000194
	.section .rom.000bd75c, "ax"
	.global Func_080b075c
	.type Func_080b075c, %function
	.thumb_func
	.align 2, 0
Func_080b075c:
	.incbin "baserom.gba", 0x000bd75c, 0x00000338
	.section .rom.000bdbb6, "ax"
	.incbin "baserom.gba", 0x000bdbb6, 0x00000002
	.section .rom.000bdbb8, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000bdbb8, 0x000000c0
	.section .rom.000bdd58, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000bdd58, 0x000000ac
	.section .rom.000bdf5a, "ax"
	.incbin "baserom.gba", 0x000bdf5a, 0x00000002
	.section .rom.000be088, "ax"
	.balign 4
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000be088, 0x00000170
	.section .rom.000be1f8, "ax"
	.global Func_080b11f8
	.type Func_080b11f8, %function
	.thumb_func
Func_080b11f8:
	.incbin "baserom.gba", 0x000be1f8, 0x00000084
	.section .rom.000be27c, "ax"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000be27c, 0x0000004c
	.section .rom.000bef28, "ax"
	.incbin "baserom.gba", 0x000bef28, 0x00000004
	.section .rom.000d3b04, "ax"
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000d3b04, 0x00000014
	.section .rom.000d3b18, "ax"
	.global Battle_Effect90Classes
Battle_Effect90Classes:
	.incbin "baserom.gba", 0x000d3b18, 0x0000005e
	.section .rom.000d3b76, "ax"
	.global Battle_Effect91Classes
Battle_Effect91Classes:
	.incbin "baserom.gba", 0x000d3b76, 0x0000003a
	.section .rom.000d5000, "ax"
	.global Resource_FarCall006
Resource_FarCall006:
	.incbin "baserom.gba", 0x000d5000, 0x00000008
	.section .rom.000d55c0, "ax"
	.global Func_080c85c0
	.type Func_080c85c0, %function
	.thumb_func
Func_080c85c0:
	.incbin "baserom.gba", 0x000d55c0, 0x00000008
	.section .rom.000d6694, "ax"
	.incbin "baserom.gba", 0x000d6694, 0x0000028c
	.section .rom.000d6944, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000d6944, 0x000002cc
	.section .rom.000d6c38, "ax"
	.incbin "baserom.gba", 0x000d6c38, 0x00000190
	.section .rom.000d6e48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d6e48, 0x000000e4
	.section .rom.000d6f2c, "ax"
	.global BattleFx_LookupResult
	.type BattleFx_LookupResult, %function
	.thumb_func
BattleFx_LookupResult:
	.incbin "baserom.gba", 0x000d6f2c, 0x000000ac
	.section .rom.000d6fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000d6fd8, 0x0000018c
	.section .rom.000d72fc, "ax"
	.incbin "baserom.gba", 0x000d72fc, 0x0000006c
	.section .rom.000d7368, "ax"
	.global Func_080ca368
	.type Func_080ca368, %function
	.thumb_func
Func_080ca368:
	.incbin "baserom.gba", 0x000d7368, 0x0000008c
	.section .rom.000d73f4, "ax"
	.global Func_080ca3f4
	.type Func_080ca3f4, %function
	.thumb_func
Func_080ca3f4:
	.incbin "baserom.gba", 0x000d73f4, 0x000000a0
	.section .rom.000d74a8, "ax"
	.incbin "baserom.gba", 0x000d74a8, 0x000000f8
	.section .rom.000d75d8, "ax"
	.global BattleFx_SelectBattleCue
	.type BattleFx_SelectBattleCue, %function
	.thumb_func
BattleFx_SelectBattleCue:
	.incbin "baserom.gba", 0x000d75d8, 0x000000cc
	.section .rom.000d76a4, "ax"
	.global Func_080ca6a4
	.type Func_080ca6a4, %function
	.thumb_func
Func_080ca6a4:
	.incbin "baserom.gba", 0x000d76a4, 0x00000040
	.section .rom.000d76e8, "ax"
	.balign 4
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000d76e8, 0x000002e4
	.section .rom.000d79cc, "ax"
	.global Func_080ca9cc
	.type Func_080ca9cc, %function
	.thumb_func
Func_080ca9cc:
	.incbin "baserom.gba", 0x000d79cc, 0x00000060
	.section .rom.000d7a2c, "ax"
	.global Func_080caa2c
	.type Func_080caa2c, %function
	.thumb_func
Func_080caa2c:
	.incbin "baserom.gba", 0x000d7a2c, 0x00000020
	.section .rom.000d7a4c, "ax"
	.global Func_080caa4c
	.type Func_080caa4c, %function
	.thumb_func
Func_080caa4c:
	.incbin "baserom.gba", 0x000d7a4c, 0x00000274
	.section .rom.000d7e5c, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000d7e5c, 0x00000110
	.section .rom.000d809c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d809c, 0x0000062c
	.section .rom.000d891c, "ax"
	.incbin "baserom.gba", 0x000d891c, 0x00000c30
	.section .rom.000d954c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d954c, 0x00000130
	.section .rom.000d967c, "ax"
	.global Func_080cc67c
Func_080cc67c:
	.incbin "baserom.gba", 0x000d967c, 0x00000148
	.section .rom.000d97c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d97c4, 0x000001d0
	.section .rom.000d99fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d99fc, 0x000002bc
	.section .rom.000d9cb8, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000d9cb8, 0x00000090
	.section .rom.000d9d76, "ax"
	.incbin "baserom.gba", 0x000d9d76, 0x00000002
	.section .rom.000d9d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d9d78, 0x0000010c
	.section .rom.000d9e94, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d9e94, 0x00000034
	.section .rom.000d9ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d9ec8, 0x000006bc
	.section .rom.000da5bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000da5bc, 0x00000360
	.section .rom.000da91c, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000da91c, 0x0000013c
	.section .rom.000daa84, "ax"
	.balign 4
	.global Func_080cda84
Func_080cda84:
	.incbin "baserom.gba", 0x000daa84, 0x0000003c
	.section .rom.000daac0, "ax"
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000daac0, 0x00000138
	.section .rom.000dac72, "ax"
	.incbin "baserom.gba", 0x000dac72, 0x00000002
	.section .rom.000dac74, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000dac74, 0x0000010c
	.section .rom.000dad80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000dad80, 0x00000128
	.section .rom.000dafb0, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000dafb0, 0x0000036c
	.section .rom.000db31c, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000db31c, 0x0000013c
	.section .rom.000db458, "ax"
	.global Func_080ce458
	.type Func_080ce458, %function
	.thumb_func
Func_080ce458:
	.incbin "baserom.gba", 0x000db458, 0x0000011c
	.section .rom.000db61c, "ax"
	.incbin "baserom.gba", 0x000db61c, 0x000004e0
	.section .rom.000dbba8, "ax"
	.incbin "baserom.gba", 0x000dbba8, 0x0000007c
	.section .rom.000dbc34, "ax"
	.incbin "baserom.gba", 0x000dbc34, 0x0000020c
	.section .rom.000dc050, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000dc050, 0x00000080
	.section .rom.000dc158, "ax"
	.incbin "baserom.gba", 0x000dc158, 0x00000024
	.section .rom.000dc17c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000dc17c, 0x000001c4
	.section .rom.000dc350, "ax"
	.balign 4
	.global Func_080cf350
	.type Func_080cf350, %function
	.thumb_func
Func_080cf350:
	.incbin "baserom.gba", 0x000dc350, 0x00000064
	.section .rom.000dc554, "ax"
	.incbin "baserom.gba", 0x000dc554, 0x000001a8
	.section .rom.000dc6fc, "ax"
	.global Func_080cf6fc
	.type Func_080cf6fc, %function
	.thumb_func
Func_080cf6fc:
	.incbin "baserom.gba", 0x000dc6fc, 0x00000090
	.section .rom.000dc78c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000dc78c, 0x0000096c
	.section .rom.000dd1cc, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000dd1cc, 0x00000354
	.section .rom.000dd520, "ax"
	.global DisplayTransition_Finish
	.type DisplayTransition_Finish, %function
	.thumb_func
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000dd520, 0x0000020c
	.section .rom.000dd788, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dd788, 0x000001cc
	.section .rom.000dd954, "ax"
	.global Func_080d0954
	.type Func_080d0954, %function
	.thumb_func
Func_080d0954:
	.incbin "baserom.gba", 0x000dd954, 0x000000d4
	.section .rom.000dda28, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000dda28, 0x00000154
	.section .rom.000ddb7c, "ax"
	.global DisplayTransition_FillTilemapAndSolidTile
	.type DisplayTransition_FillTilemapAndSolidTile, %function
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000ddb7c, 0x00000070
	.section .rom.000ddca0, "ax"
	.global BattlePalette_UpdateBlend
	.type BattlePalette_UpdateBlend, %function
	.thumb_func
BattlePalette_UpdateBlend:
	.incbin "baserom.gba", 0x000ddca0, 0x0000017c
	.section .rom.000dde1c, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x000dde1c, 0x00000868
	.section .rom.000de760, "ax"
	.balign 4
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000de760, 0x0000004c
	.section .rom.000de83e, "ax"
	.incbin "baserom.gba", 0x000de83e, 0x00000002
	.section .rom.000de840, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000de840, 0x000001d8
	.section .rom.000dea18, "ax"
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000dea18, 0x000000ac
	.section .rom.000deae4, "ax"
	.global Func_080d1ae4
	.type Func_080d1ae4, %function
	.thumb_func
Func_080d1ae4:
	.incbin "baserom.gba", 0x000deae4, 0x000001dc
	.section .rom.000dedac, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000dedac, 0x00000044
	.section .rom.000dee18, "ax"
	.balign 4
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000dee18, 0x00000048
	.section .rom.000dee60, "ax"
	.global Func_080d1e60
	.type Func_080d1e60, %function
	.thumb_func
Func_080d1e60:
	.incbin "baserom.gba", 0x000dee60, 0x00000024
	.section .rom.000deee8, "ax"
	.balign 4
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000deee8, 0x00000024
	.section .rom.000def20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000def20, 0x00000164
	.section .rom.000df0fc, "ax"
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000df0fc, 0x000000f8
	.section .rom.000df60c, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000df60c, 0x000001e0
	.section .rom.000df840, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000df840, 0x0000011c
	.section .rom.000df96c, "ax"
	.balign 4
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000df96c, 0x000000a0
	.section .rom.000dfb4c, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
	.align 2, 0
Func_080d2b4c:
	.incbin "baserom.gba", 0x000dfb4c, 0x000000b8
	.section .rom.000e0116, "ax"
	.incbin "baserom.gba", 0x000e0116, 0x00000002
	.section .rom.000e0118, "ax"
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.type ObjectMotion_SetHorizontalPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
	.incbin "baserom.gba", 0x000e0118, 0x0000004c
	.section .rom.000e0164, "ax"
	.global ObjectMotion_SetHorizontalPositionAndAngleWithTerrain
	.type ObjectMotion_SetHorizontalPositionAndAngleWithTerrain, %function
	.thumb_func
ObjectMotion_SetHorizontalPositionAndAngleWithTerrain:
	.incbin "baserom.gba", 0x000e0164, 0x0000005c
	.section .rom.000e01c0, "ax"
	.global ObjectMotion_SetPositionWithTerrain
	.type ObjectMotion_SetPositionWithTerrain, %function
	.thumb_func
ObjectMotion_SetPositionWithTerrain:
	.incbin "baserom.gba", 0x000e01c0, 0x00000054
	.section .rom.000e0214, "ax"
	.global ObjectMotion_MoveToObject
	.type ObjectMotion_MoveToObject, %function
	.thumb_func
ObjectMotion_MoveToObject:
	.incbin "baserom.gba", 0x000e0214, 0x0000002c
	.section .rom.000e0268, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000e0268, 0x00000050
	.section .rom.000e0376, "ax"
	.incbin "baserom.gba", 0x000e0376, 0x00000002
	.section .rom.000e0378, "ax"
	.incbin "baserom.gba", 0x000e0378, 0x000000e8
	.section .rom.000e0460, "ax"
	.global BattleFx_RunRisingObjectSequence
	.type BattleFx_RunRisingObjectSequence, %function
	.thumb_func
BattleFx_RunRisingObjectSequence:
	.incbin "baserom.gba", 0x000e0460, 0x0000013c
	.section .rom.000e0780, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000e0780, 0x00000054
	.section .rom.000e0940, "ax"
	.balign 4
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000e0940, 0x000000d0
	.section .rom.000e0a10, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000e0a10, 0x00000118
	.section .rom.000e0b28, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000e0b28, 0x000000c0
	.section .rom.000e0c88, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000e0c88, 0x00000328
	.section .rom.000e0fb0, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000e0fb0, 0x000000bc
	.section .rom.000e10dc, "ax"
	.balign 4
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000e10dc, 0x0000009c
	.section .rom.000e11e6, "ax"
	.incbin "baserom.gba", 0x000e11e6, 0x00000002
	.section .rom.000e11fc, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000e11fc, 0x00000134
	.section .rom.000e1330, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000e1330, 0x00000054
	.section .rom.000e140c, "ax"
	.balign 4
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000e140c, 0x00000100
	.section .rom.000e17b4, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000e17b4, 0x000000b4
	.section .rom.000e189c, "ax"
	.balign 4
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000e189c, 0x000000f0
	.section .rom.000e1bc4, "ax"
	.incbin "baserom.gba", 0x000e1bc4, 0x00000108
	.section .rom.000e1d08, "ax"
	.balign 4
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000e1d08, 0x000003f0
	.section .rom.000e20f8, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000e20f8, 0x000002c0
	.section .rom.000e23b8, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000e23b8, 0x00000834
	.section .rom.000e2bec, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000e2bec, 0x00000184
	.section .rom.000e2d70, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000e2d70, 0x00000070
	.section .rom.000e2de0, "ax"
	.global ObjectEffect_PrepareContextEffect
	.type ObjectEffect_PrepareContextEffect, %function
	.thumb_func
ObjectEffect_PrepareContextEffect:
	.incbin "baserom.gba", 0x000e2de0, 0x00000070
	.section .rom.000e2e78, "ax"
	.balign 4
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000e2e78, 0x000000a0
	.section .rom.000e307c, "ax"
	.global Func_080d607c
	.type Func_080d607c, %function
	.thumb_func
Func_080d607c:
	.incbin "baserom.gba", 0x000e307c, 0x0000038c
	.section .rom.000e34f0, "ax"
	.incbin "baserom.gba", 0x000e34f0, 0x00000184
	.section .rom.000e3674, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000e3674, 0x00000174
	.section .rom.000e37e8, "ax"
	.global Func_080d67e8
	.type Func_080d67e8, %function
	.thumb_func
Func_080d67e8:
	.incbin "baserom.gba", 0x000e37e8, 0x00000124
	.section .rom.000e390c, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000e390c, 0x00000284
	.section .rom.000e3b90, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000e3b90, 0x000002c8
	.section .rom.000e3e58, "ax"
	.global Func_080d6e58
	.type Func_080d6e58, %function
	.thumb_func
Func_080d6e58:
	.incbin "baserom.gba", 0x000e3e58, 0x000000dc
	.section .rom.000e3f34, "ax"
	.global Func_080d6f34
	.type Func_080d6f34, %function
	.thumb_func
Func_080d6f34:
	.incbin "baserom.gba", 0x000e3f34, 0x000000f0
	.section .rom.000e4034, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000e4034, 0x0000020c
	.section .rom.000e4524, "ax"
	.incbin "baserom.gba", 0x000e4524, 0x00000264
	.section .rom.000e4788, "ax"
	.global Func_080d7788
	.type Func_080d7788, %function
	.thumb_func
Func_080d7788:
	.incbin "baserom.gba", 0x000e4788, 0x000002d0
	.section .rom.000e4b04, "ax"
	.incbin "baserom.gba", 0x000e4b04, 0x00000244
	.section .rom.000e4d90, "ax"
	.incbin "baserom.gba", 0x000e4d90, 0x0000097c
	.section .rom.000e5740, "ax"
	.incbin "baserom.gba", 0x000e5740, 0x0000023c
	.section .rom.000e5a00, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000e5a00, 0x00000210
	.section .rom.000e5c10, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000e5c10, 0x00000130
	.section .rom.000e5d68, "ax"
	.balign 4
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000e5d68, 0x00000240
	.section .rom.000e5fa8, "ax"
	.global Func_080d8fa8
	.type Func_080d8fa8, %function
	.thumb_func
Func_080d8fa8:
	.incbin "baserom.gba", 0x000e5fa8, 0x0000015c
	.section .rom.000e6104, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000e6104, 0x000001a0
	.section .rom.000e62d4, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000e62d4, 0x0000043c
	.section .rom.000e6710, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000e6710, 0x00000264
	.section .rom.000e6974, "ax"
	.global Func_080e7818
Func_080e7818:
	.incbin "baserom.gba", 0x000e6974, 0x00000004
	.section .rom.000e6ab0, "ax"
	.balign 4
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000e6ab0, 0x00000020
	.section .rom.000e6b80, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000e6b80, 0x000001c0
	.section .rom.000e6d40, "ax"
	.global Func_080d9d40
	.type Func_080d9d40, %function
	.thumb_func
Func_080d9d40:
	.incbin "baserom.gba", 0x000e6d40, 0x00000134
	.section .rom.000e6e98, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000e6e98, 0x000001c8
	.section .rom.000e7060, "ax"
	.global Func_080da060
	.type Func_080da060, %function
	.thumb_func
Func_080da060:
	.incbin "baserom.gba", 0x000e7060, 0x000008a8
	.section .rom.000e79a8, "ax"
	.incbin "baserom.gba", 0x000e79a8, 0x00000500
	.section .rom.000e7ecc, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000e7ecc, 0x000001e4
	.section .rom.000e80b0, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000e80b0, 0x00000394
	.section .rom.000e8444, "ax"
	.global Func_080db444
	.type Func_080db444, %function
	.thumb_func
Func_080db444:
	.incbin "baserom.gba", 0x000e8444, 0x00000048
	.section .rom.000e84b6, "ax"
	.incbin "baserom.gba", 0x000e84b6, 0x00000002
	.section .rom.000e84b8, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x000e84b8, 0x000001b8
	.section .rom.000e8670, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x000e8670, 0x000001d8
	.section .rom.000e89a6, "ax"
	.incbin "baserom.gba", 0x000e89a6, 0x00000002
	.section .rom.000e8a5c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8a5c, 0x000000e4
	.section .rom.000e8b78, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.incbin "baserom.gba", 0x000e8b78, 0x0000008c
	.section .rom.000e8c04, "ax"
	.global Func_080dbc04
Func_080dbc04:
	.incbin "baserom.gba", 0x000e8c04, 0x000000a4
	.section .rom.000e8ca8, "ax"
	.global Func_080dbca8
	.type Func_080dbca8, %function
	.thumb_func
Func_080dbca8:
	.incbin "baserom.gba", 0x000e8ca8, 0x00000018
	.section .rom.000e8cc0, "ax"
	.global Func_080dbcc0
Func_080dbcc0:
	.incbin "baserom.gba", 0x000e8cc0, 0x00000018
	.section .rom.000e8cd8, "ax"
	.global Func_080dbcd8
	.type Func_080dbcd8, %function
	.thumb_func
Func_080dbcd8:
	.incbin "baserom.gba", 0x000e8cd8, 0x00000070
	.section .rom.000e8d5c, "ax"
	.balign 4
	.global Func_080dbd5c
Func_080dbd5c:
	.incbin "baserom.gba", 0x000e8d5c, 0x0000004c
	.section .rom.000e8ed0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8ed0, 0x00000174
	.section .rom.000e9294, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000e9294, 0x000000e8
	.section .rom.000e9410, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x000e9410, 0x0000021c
	.section .rom.000e96d8, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000e96d8, 0x000000f4
	.section .rom.000e97e8, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000e97e8, 0x0000016c
	.section .rom.000e9978, "ax"
	.balign 4
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000e9978, 0x000000d8
	.section .rom.000e9adc, "ax"
	.balign 4
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000e9adc, 0x00000068
	.section .rom.000e9b44, "ax"
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000e9b44, 0x00000284
	.section .rom.000ea054, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000ea054, 0x000003e8
	.section .rom.000ea526, "ax"
	.incbin "baserom.gba", 0x000ea526, 0x00000002
	.section .rom.000ea528, "ax"
	.global BattleFx_StartItemBreak
	.type BattleFx_StartItemBreak, %function
	.thumb_func
BattleFx_StartItemBreak:
	.incbin "baserom.gba", 0x000ea528, 0x00000114
	.section .rom.000ea63c, "ax"
	.global BattleFx_SnapScaleToFull
	.type BattleFx_SnapScaleToFull, %function
	.thumb_func
BattleFx_SnapScaleToFull:
	.incbin "baserom.gba", 0x000ea63c, 0x0000002c
	.section .rom.000ea668, "ax"
	.global UpdateRisingParticleBurst
	.type UpdateRisingParticleBurst, %function
	.thumb_func
UpdateRisingParticleBurst:
	.incbin "baserom.gba", 0x000ea668, 0x000000e0
	.section .rom.000ea754, "ax"
	.incbin "baserom.gba", 0x000ea754, 0x000001fc
	.section .rom.000ea950, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000ea950, 0x000001ec
	.section .rom.000eabd8, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000eabd8, 0x0000063c
	.section .rom.000eb21c, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000eb21c, 0x00000334
	.section .rom.000eb688, "ax"
	.global BattleFx_RunOrbitingParticles
	.type BattleFx_RunOrbitingParticles, %function
	.thumb_func
BattleFx_RunOrbitingParticles:
	.incbin "baserom.gba", 0x000eb688, 0x00000190
	.section .rom.000eb864, "ax"
	.incbin "baserom.gba", 0x000eb864, 0x00000188
	.section .rom.000eb9f8, "ax"
	.global Func_080de9f8
	.type Func_080de9f8, %function
	.thumb_func
Func_080de9f8:
	.incbin "baserom.gba", 0x000eb9f8, 0x000001ac
	.section .rom.000ebbd8, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000ebbd8, 0x000000b4
	.section .rom.000ebcb8, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000ebcb8, 0x000000cc
	.section .rom.000ebde8, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000ebde8, 0x00000ac0
	.section .rom.000ec8de, "ax"
	.incbin "baserom.gba", 0x000ec8de, 0x00000002
	.section .rom.000ec8e0, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000ec8e0, 0x000001d4
	.section .rom.000ecc3c, "ax"
	.incbin "baserom.gba", 0x000ecc3c, 0x00000098
	.section .rom.000ecd00, "ax"
	.global RunBattleEffect14
RunBattleEffect14:
	.incbin "baserom.gba", 0x000ecd00, 0x000002fc
	.section .rom.000ed020, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ed020, 0x000000fc
	.section .rom.000ed134, "ax"
	.global Func_080e0134
	.type Func_080e0134, %function
	.thumb_func
Func_080e0134:
	.incbin "baserom.gba", 0x000ed134, 0x000001b8
	.section .rom.000ed3c2, "ax"
	.incbin "baserom.gba", 0x000ed3c2, 0x00000002
	.section .rom.000ed3cc, "ax"
	.balign 4
	.global Func_080e03cc
	.type Func_080e03cc, %function
	.thumb_func
Func_080e03cc:
	.incbin "baserom.gba", 0x000ed3cc, 0x0000024c
	.section .rom.000ed618, "ax"
	.incbin "baserom.gba", 0x000ed618, 0x00000360
	.section .rom.000ed9c0, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000ed9c0, 0x000002c4
	.section .rom.000edcac, "ax"
	.global Func_080e0cac
	.type Func_080e0cac, %function
	.thumb_func
Func_080e0cac:
	.incbin "baserom.gba", 0x000edcac, 0x00000040
	.section .rom.000edcec, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000edcec, 0x000000e8
	.section .rom.000edf40, "ax"
	.incbin "baserom.gba", 0x000edf40, 0x00000214
	.section .rom.000ee154, "ax"
	.global Func_080e1154
	.type Func_080e1154, %function
	.thumb_func
Func_080e1154:
	.incbin "baserom.gba", 0x000ee154, 0x00000068
	.section .rom.000ee448, "ax"
	.global BattleFx_SpawnScaledArcObjects
	.type BattleFx_SpawnScaledArcObjects, %function
	.thumb_func
BattleFx_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000ee448, 0x000000c4
	.section .rom.000ee50c, "ax"
	.global BattleFx_UpdateArcEffect
	.type BattleFx_UpdateArcEffect, %function
	.thumb_func
BattleFx_UpdateArcEffect:
	.incbin "baserom.gba", 0x000ee50c, 0x000000f0
	.section .rom.000ee678, "ax"
	.incbin "baserom.gba", 0x000ee678, 0x000008ac
	.section .rom.000eef2c, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000eef2c, 0x000006bc
	.section .rom.000ef5e8, "ax"
	.global Func_080e25e8
	.type Func_080e25e8, %function
	.thumb_func
Func_080e25e8:
	.incbin "baserom.gba", 0x000ef5e8, 0x0000018c
	.section .rom.000ef774, "ax"
	.global Func_080e2774
	.type Func_080e2774, %function
	.thumb_func
Func_080e2774:
	.incbin "baserom.gba", 0x000ef774, 0x00000160
	.section .rom.000ef8d4, "ax"
	.global Func_080e28d4
	.type Func_080e28d4, %function
	.thumb_func
Func_080e28d4:
	.incbin "baserom.gba", 0x000ef8d4, 0x00000364
	.section .rom.000efc38, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000efc38, 0x00000108
	.section .rom.000efd64, "ax"
	.incbin "baserom.gba", 0x000efd64, 0x000002fc
	.section .rom.000f0074, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000f0074, 0x00000624
	.section .rom.000f0698, "ax"
	.global Func_080e3698
	.type Func_080e3698, %function
	.thumb_func
Func_080e3698:
	.incbin "baserom.gba", 0x000f0698, 0x00000148
	.section .rom.000f07e0, "ax"
	.global Func_080e37e0
	.type Func_080e37e0, %function
	.thumb_func
Func_080e37e0:
	.incbin "baserom.gba", 0x000f07e0, 0x00000a64
	.section .rom.000f1244, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000f1244, 0x00000090
	.section .rom.000f13a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f13a4, 0x000001fc
	.section .rom.000f15a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f15a8, 0x00000188
	.section .rom.000f1730, "ax"
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000f1730, 0x000015c0
	.section .rom.000f2d54, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f2d54, 0x00000634
	.section .rom.000f3400, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f3400, 0x00000cf8
	.section .rom.000f416c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f416c, 0x000000cc
	.section .rom.000f4238, "ax"
	.global Func_080e7238
	.type Func_080e7238, %function
	.thumb_func
Func_080e7238:
	.incbin "baserom.gba", 0x000f4238, 0x000002a0
	.section .rom.000f44d8, "ax"
	.global Func_080e74d8
	.type Func_080e74d8, %function
	.thumb_func
Func_080e74d8:
	.incbin "baserom.gba", 0x000f44d8, 0x0000032c
	.section .rom.000f4804, "ax"
	.global Func_080e7804
	.type Func_080e7804, %function
	.thumb_func
Func_080e7804:
	.incbin "baserom.gba", 0x000f4804, 0x00000018
	.section .rom.000f481c, "ax"
	.global Func_080e781c
Func_080e781c:
	.incbin "baserom.gba", 0x000f481c, 0x00000fa8
	.section .rom.000f57c4, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000f57c4, 0x000005bc
	.section .rom.000f5db0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f5db0, 0x000004a0
	.section .rom.000f62b4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f62b4, 0x00000b00
	.section .rom.000f6db4, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000f6db4, 0x00000130
	.section .rom.000f6f68, "ax"
	.incbin "baserom.gba", 0x000f6f68, 0x000001e4
	.section .rom.000f714c, "ax"
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000f714c, 0x00000674
	.section .rom.000f77c0, "ax"
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000f77c0, 0x000000e8
	.section .rom.000f78a8, "ax"
	.global Func_080ea8a8
Func_080ea8a8:
	.incbin "baserom.gba", 0x000f78a8, 0x0000002c
	.section .rom.000f78d4, "ax"
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000f78d4, 0x00000140
	.section .rom.000f7a14, "ax"
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000f7a14, 0x0000015c
	.section .rom.000f7b98, "ax"
	.balign 4
	.global Func_080eab98
	.type Func_080eab98, %function
	.thumb_func
Func_080eab98:
	.incbin "baserom.gba", 0x000f7b98, 0x00000038
	.section .rom.000f7bd0, "ax"
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000f7bd0, 0x00000118
	.section .rom.000f7ce8, "ax"
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000f7ce8, 0x00000114
	.section .rom.000f7dfc, "ax"
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000f7dfc, 0x000000b8
	.section .rom.000f7eb4, "ax"
	.global Func_080eaeb4
	.type Func_080eaeb4, %function
	.thumb_func
Func_080eaeb4:
	.incbin "baserom.gba", 0x000f7eb4, 0x00000074
	.section .rom.000f7f98, "ax"
	.global Func_080eaf98
	.type Func_080eaf98, %function
	.thumb_func
Func_080eaf98:
	.incbin "baserom.gba", 0x000f7f98, 0x00000084
	.section .rom.000f801c, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000f801c, 0x0000027c
	.section .rom.000f8318, "ax"
	.incbin "baserom.gba", 0x000f8318, 0x00000204
	.section .rom.000f8594, "ax"
	.global Func_080eb594
	.type Func_080eb594, %function
	.thumb_func
Func_080eb594:
	.incbin "baserom.gba", 0x000f8594, 0x0000010c
	.section .rom.000f86a0, "ax"
	.global Func_080eb6a0
	.type Func_080eb6a0, %function
	.thumb_func
Func_080eb6a0:
	.incbin "baserom.gba", 0x000f86a0, 0x00000184
	.section .rom.000f8824, "ax"
	.incbin "baserom.gba", 0x000f8824, 0x0000010c
	.section .rom.000f8960, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f8960, 0x000002d0
	.section .rom.000f8d24, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000f8d24, 0x0000014c
	.section .rom.000f8ec8, "ax"
	.global EffectSlot_Initialize
	.type EffectSlot_Initialize, %function
	.thumb_func
EffectSlot_Initialize:
	.incbin "baserom.gba", 0x000f8ec8, 0x000000a0
	.section .rom.000f8f94, "ax"
	.incbin "baserom.gba", 0x000f8f94, 0x000001b8
	.section .rom.000f91d0, "ax"
	.incbin "baserom.gba", 0x000f91d0, 0x000000c8
	.section .rom.000f92b8, "ax"
	.incbin "baserom.gba", 0x000f92b8, 0x000014d0
	.section .rom.000faa0c, "ax"
	.incbin "baserom.gba", 0x000faa0c, 0x000000c0
	.section .rom.000fc81c, "ax"
	.incbin "baserom.gba", 0x000fc81c, 0x00000008
	.section .rom.000fd1b8, "ax"
	.incbin "baserom.gba", 0x000fd1b8, 0x0000000c
	.section .rom.000fd306, "ax"
	.incbin "baserom.gba", 0x000fd306, 0x00000002
	.section .rom.000fdb50, "ax"
	.incbin "baserom.gba", 0x000fdb50, 0x0000002c
	.section .rom.000fdbfc, "ax"
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000fdbfc, 0x00000008
	.section .rom.000fdc04, "ax"
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000fdc04, 0x00000250
	.section .rom.000fde54, "ax"
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000fde54, 0x00000024
	.section .rom.000fde78, "ax"
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000fde78, 0x00000024
	.section .rom.000fde9c, "ax"
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000fde9c, 0x00000008
	.section .rom.000fdec4, "ax"
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000fdec4, 0x00000024
	.section .rom.000fdee8, "ax"
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000fdee8, 0x00000044
	.section .rom.000fdf2c, "ax"
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000fdf2c, 0x000001d4
	.section .rom.000fe120, "ax"
	.incbin "baserom.gba", 0x000fe120, 0x00000020
	.section .rom.000fe140, "ax"
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000fe140, 0x00000668
	.section .rom.000ff1d0, "ax"
	.incbin "baserom.gba", 0x000ff1d0, 0x00000034
	.section .rom.0010021c, "ax"
	.incbin "baserom.gba", 0x0010021c, 0x000000b4
	.section .rom.001002d0, "ax"
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x001002d0, 0x00000010
	.section .rom.001002e0, "ax"
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x001002e0, 0x00000020
	.section .rom.00100300, "ax"
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x00100300, 0x00000128
	.section .rom.00100428, "ax"
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x00100428, 0x000000bc
	.section .rom.001004e4, "ax"
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x001004e4, 0x00000004
	.section .rom.001004e8, "ax"
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x001004e8, 0x0000000c
	.section .rom.001004f4, "ax"
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x001004f4, 0x0000000c
	.section .rom.00100500, "ax"
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x00100500, 0x0000000c
	.section .rom.0010050c, "ax"
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x0010050c, 0x000000bc
	.section .rom.001005c8, "ax"
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x001005c8, 0x0000004c
	.section .rom.00100614, "ax"
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x00100614, 0x00000128
	.section .rom.0010073c, "ax"
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x0010073c, 0x00000014
	.section .rom.00100750, "ax"
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x00100750, 0x00000014
	.section .rom.00100764, "ax"
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x00100764, 0x0000001c
	.section .rom.00100780, "ax"
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x00100780, 0x00000018
	.section .rom.0010085e, "ax"
	.incbin "baserom.gba", 0x0010085e, 0x00000054
	.section .rom.00100901, "ax"
	.incbin "baserom.gba", 0x00100901, 0x00000043
	.section .rom.00100944, "ax"
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x00100944, 0x0000000c
	.section .rom.00100950, "ax"
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x00100950, 0x00000024
	.section .rom.00100974, "ax"
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x00100974, 0x00000050
	.section .rom.00102028, "ax"
	.global UiIcon_PrepareObjectFar
	.type UiIcon_PrepareObjectFar, %function
	.thumb_func
UiIcon_PrepareObjectFar:
	.incbin "baserom.gba", 0x00102028, 0x00000008
	.section .rom.00102030, "ax"
	.global ItemMenu_Open
	.type ItemMenu_Open, %function
	.thumb_func
ItemMenu_Open:
	.incbin "baserom.gba", 0x00102030, 0x00000008
	.section .rom.00102038, "ax"
	.global ItemMenu_DrawItemDetailsFar
	.type ItemMenu_DrawItemDetailsFar, %function
	.thumb_func
ItemMenu_DrawItemDetailsFar:
	.incbin "baserom.gba", 0x00102038, 0x00000008
	.section .rom.00102040, "ax"
	.global Func_080f8040
	.type Func_080f8040, %function
	.thumb_func
Func_080f8040:
	.incbin "baserom.gba", 0x00102040, 0x00000008
	.section .rom.00102048, "ax"
	.global Palette_CopyObjectBankToBackground14Far
	.type Palette_CopyObjectBankToBackground14Far, %function
	.thumb_func
Palette_CopyObjectBankToBackground14Far:
	.incbin "baserom.gba", 0x00102048, 0x00000008
	.section .rom.00102050, "ax"
	.global Menu_UpdateEntryObjectTransformsFar
	.type Menu_UpdateEntryObjectTransformsFar, %function
	.thumb_func
Menu_UpdateEntryObjectTransformsFar:
	.incbin "baserom.gba", 0x00102050, 0x00000020
	.section .rom.00102170, "ax"
	.incbin "baserom.gba", 0x00102170, 0x000004a0
	.section .rom.00102888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x00102888, 0x0000003c
	.section .rom.001028d0, "ax"
	.global Func_080f88d0
	.type Func_080f88d0, %function
	.thumb_func
Func_080f88d0:
	.incbin "baserom.gba", 0x001028d0, 0x000001e4
	.section .rom.00102ab4, "ax"
	.global Func_080f8ab4
	.type Func_080f8ab4, %function
	.thumb_func
Func_080f8ab4:
	.incbin "baserom.gba", 0x00102ab4, 0x00000108
	.section .rom.00102ce6, "ax"
	.incbin "baserom.gba", 0x00102ce6, 0x00000002
	.section .rom.00102ce8, "ax"
	.global Func_080f8ce8
	.type Func_080f8ce8, %function
	.thumb_func
Func_080f8ce8:
	.incbin "baserom.gba", 0x00102ce8, 0x00000258
	.section .rom.00102f9c, "ax"
	.incbin "baserom.gba", 0x00102f9c, 0x0000016c
	.section .rom.001032dc, "ax"
	.incbin "baserom.gba", 0x001032dc, 0x00000098
	.section .rom.001033b6, "ax"
	.incbin "baserom.gba", 0x001033b6, 0x00000002
	.section .rom.001034a2, "ax"
	.incbin "baserom.gba", 0x001034a2, 0x00000002
	.section .rom.001034a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x001034a4, 0x00000f3c
	.section .rom.00104484, "ax"
	.incbin "baserom.gba", 0x00104484, 0x0000003c
	.section .rom.001045ec, "ax"
	.global Func_080fa5a4
	.type Func_080fa5a4, %function
	.thumb_func
Func_080fa5a4:
	.incbin "baserom.gba", 0x001045ec, 0x000002a8
	.section .rom.001048b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001048b8, 0x00000370
	.section .rom.00104ca0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00104ca0, 0x00000048
	.section .rom.00104d1e, "ax"
	.incbin "baserom.gba", 0x00104d1e, 0x00000002
	.section .rom.00104d20, "ax"
	.global Menu_HideEmptyEntryIcons
	.type Menu_HideEmptyEntryIcons, %function
	.thumb_func
Menu_HideEmptyEntryIcons:
	.incbin "baserom.gba", 0x00104d20, 0x00000044
	.section .rom.00104ed4, "ax"
	.incbin "baserom.gba", 0x00104ed4, 0x00000218
	.section .rom.0010514c, "ax"
	.incbin "baserom.gba", 0x0010514c, 0x00000544
	.section .rom.001057d8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001057d8, 0x0000012c
	.section .rom.00105910, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x00105910, 0x000004e8
	.section .rom.00105f30, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00105f30, 0x00000618
	.section .rom.00106620, "ax"
	.incbin "baserom.gba", 0x00106620, 0x00000594
	.section .rom.00106bb4, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00106bb4, 0x00000854
	.section .rom.0010745c, "ax"
	.incbin "baserom.gba", 0x0010745c, 0x00000244
	.section .rom.001076d0, "ax"
	.incbin "baserom.gba", 0x001076d0, 0x00000188
	.section .rom.00107898, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x00107898, 0x00000a78
	.section .rom.00108420, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x00108420, 0x0000030c
	.section .rom.0010872c, "ax"
	.global Func_080fe580
	.type Func_080fe580, %function
	.thumb_func
Func_080fe580:
	.incbin "baserom.gba", 0x0010872c, 0x00000884
	.section .rom.00108fec, "ax"
	.incbin "baserom.gba", 0x00108fec, 0x000003b4
	.section .rom.0010951c, "ax"
	.incbin "baserom.gba", 0x0010951c, 0x00000570
	.section .rom.00109af8, "ax"
	.incbin "baserom.gba", 0x00109af8, 0x00000b8c
	.section .rom.0010a70c, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x0010a70c, 0x000000e4
	.section .rom.0010a904, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010a904, 0x00000608
	.section .rom.0010b048, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x0010b048, 0x00000734
	.section .rom.0010b804, "ax"
	.incbin "baserom.gba", 0x0010b804, 0x00000490
	.section .rom.0010bc94, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x0010bc94, 0x00000144
	.section .rom.0010be0c, "ax"
	.incbin "baserom.gba", 0x0010be0c, 0x0000003c
	.section .rom.0010be48, "ax"
	.global Func_08101c7c
	.type Func_08101c7c, %function
	.thumb_func
Func_08101c7c:
	.incbin "baserom.gba", 0x0010be48, 0x000000b8
	.section .rom.0010bf28, "ax"
	.incbin "baserom.gba", 0x0010bf28, 0x00002cfc
	.section .rom.0010ed22, "ax"
	.incbin "baserom.gba", 0x0010ed22, 0x00000002
	.section .rom.0010ed24, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x0010ed24, 0x00000174
	.section .rom.0010ef28, "ax"
	.incbin "baserom.gba", 0x0010ef28, 0x0000004c
	.section .rom.0010ef74, "ax"
	.global Menu_UpdateEntryObjectTransforms
	.type Menu_UpdateEntryObjectTransforms, %function
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	.incbin "baserom.gba", 0x0010ef74, 0x00000238
	.section .rom.0010f1ac, "ax"
	.global Func_08104fe0
	.type Func_08104fe0, %function
	.thumb_func
Func_08104fe0:
	.incbin "baserom.gba", 0x0010f1ac, 0x000000ac
	.section .rom.0010f298, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x0010f298, 0x000000dc
	.section .rom.0010f374, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010f374, 0x000000c4
	.section .rom.0010f574, "ax"
	.incbin "baserom.gba", 0x0010f574, 0x000000c0
	.section .rom.0010f6ac, "ax"
	.incbin "baserom.gba", 0x0010f6ac, 0x000000a0
	.section .rom.00110148, "ax"
	.global Func_08108148
	.type Func_08108148, %function
	.thumb_func
Func_08108148:
	.incbin "baserom.gba", 0x00110148, 0x00000104
	.section .rom.0011024c, "ax"
	.global Func_0810824c
	.type Func_0810824c, %function
	.thumb_func
Func_0810824c:
	.incbin "baserom.gba", 0x0011024c, 0x00000078
	.section .rom.001102c4, "ax"
	.global Func_081082c4
	.type Func_081082c4, %function
	.thumb_func
Func_081082c4:
	.incbin "baserom.gba", 0x001102c4, 0x000001a0
	.section .rom.00110690, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00110690, 0x000000cc
	.section .rom.00110948, "ax"
	.balign 4
	.global Func_08108948
	.type Func_08108948, %function
	.thumb_func
Func_08108948:
	.incbin "baserom.gba", 0x00110948, 0x0000009c
	.section .rom.001109e4, "ax"
	.global Func_081089e4
	.type Func_081089e4, %function
	.thumb_func
Func_081089e4:
	.incbin "baserom.gba", 0x001109e4, 0x000000a4
	.section .rom.00110aa8, "ax"
	.balign 4
	.global Func_08108aa8
	.type Func_08108aa8, %function
	.thumb_func
Func_08108aa8:
	.incbin "baserom.gba", 0x00110aa8, 0x00000048
	.section .rom.00110b70, "ax"
	.incbin "baserom.gba", 0x00110b70, 0x00000618
	.section .rom.001111cc, "ax"
	.incbin "baserom.gba", 0x001111cc, 0x000000a4
	.section .rom.0011128c, "ax"
	.incbin "baserom.gba", 0x0011128c, 0x00000398
	.section .rom.001116f8, "ax"
	.global Func_081096f8
	.type Func_081096f8, %function
	.thumb_func
Func_081096f8:
	.incbin "baserom.gba", 0x001116f8, 0x000001c8
	.section .rom.00111ad8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00111ad8, 0x000001d4
	.section .rom.00111cac, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x00111cac, 0x0000035c
	.section .rom.00112008, "ax"
	.global Shop_DrawItemCount
	.type Shop_DrawItemCount, %function
	.thumb_func
Shop_DrawItemCount:
	.incbin "baserom.gba", 0x00112008, 0x00000104
	.section .rom.0011210c, "ax"
	.global Func_0810a108
	.type Func_0810a108, %function
	.thumb_func
Func_0810a108:
	.incbin "baserom.gba", 0x0011210c, 0x000001a4
	.section .rom.001122dc, "ax"
	.incbin "baserom.gba", 0x001122dc, 0x00000394
	.section .rom.00112674, "ax"
	.incbin "baserom.gba", 0x00112674, 0x00000088
	.section .rom.00112710, "ax"
	.global Forge_FindRecipe
	.type Forge_FindRecipe, %function
	.thumb_func
Forge_FindRecipe:
	.incbin "baserom.gba", 0x00112710, 0x0000003c
	.section .rom.001128f0, "ax"
	.incbin "baserom.gba", 0x001128f0, 0x00000074
	.section .rom.001129b0, "ax"
	.incbin "baserom.gba", 0x001129b0, 0x000000d4
	.section .rom.00112a84, "ax"
	.global Func_0810aa80
	.type Func_0810aa80, %function
	.thumb_func
Func_0810aa80:
	.incbin "baserom.gba", 0x00112a84, 0x00000424
	.section .rom.00112f28, "ax"
	.incbin "baserom.gba", 0x00112f28, 0x00000198
	.section .rom.001131b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001131b8, 0x000001c4
	.section .rom.0011337c, "ax"
	.global Func_0810b378
	.type Func_0810b378, %function
	.thumb_func
Func_0810b378:
	.incbin "baserom.gba", 0x0011337c, 0x00000074
	.section .rom.0011341c, "ax"
	.balign 4
	.global Func_0810b418
	.type Func_0810b418, %function
	.thumb_func
Func_0810b418:
	.incbin "baserom.gba", 0x0011341c, 0x00000108
	.section .rom.001135c8, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x001135c8, 0x000000f8
	.section .rom.001136d0, "ax"
	.incbin "baserom.gba", 0x001136d0, 0x000000d0
	.section .rom.001137b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x001137b8, 0x0000060c
	.section .rom.00113e08, "ax"
	.incbin "baserom.gba", 0x00113e08, 0x00000048
	.section .rom.00113ed0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x00113ed0, 0x000000dc
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
	.section .rom.00118088, "ax"
	.global Func_08118088
	.type Func_08118088, %function
	.thumb_func
Func_08118088:
	.incbin "baserom.gba", 0x00118088, 0x00000008
	.section .rom.00118090, "ax"
	.global BattleParty_ListActorIdsFar
	.type BattleParty_ListActorIdsFar, %function
	.thumb_func
BattleParty_ListActorIdsFar:
	.incbin "baserom.gba", 0x00118090, 0x00000008
	.section .rom.00118098, "ax"
	.global GetBattleObjectSlotFar
	.type GetBattleObjectSlotFar, %function
	.thumb_func
GetBattleObjectSlotFar:
	.incbin "baserom.gba", 0x00118098, 0x00000008
	.section .rom.001180a0, "ax"
	.global Battle_RunEncounterFar
	.type Battle_RunEncounterFar, %function
	.thumb_func
Battle_RunEncounterFar:
	.incbin "baserom.gba", 0x001180a0, 0x00000008
	.section .rom.001180a8, "ax"
	.global BattleMotion_ProjectPositionFar
	.type BattleMotion_ProjectPositionFar, %function
	.thumb_func
BattleMotion_ProjectPositionFar:
	.incbin "baserom.gba", 0x001180a8, 0x00000008
	.section .rom.001180b0, "ax"
	.global BattleMotion_ProjectScaledPositionFar
	.type BattleMotion_ProjectScaledPositionFar, %function
	.thumb_func
BattleMotion_ProjectScaledPositionFar:
	.incbin "baserom.gba", 0x001180b0, 0x00000008
	.section .rom.001180b8, "ax"
	.global BattleMotion_ProjectConditionalPositionFar
	.type BattleMotion_ProjectConditionalPositionFar, %function
	.thumb_func
BattleMotion_ProjectConditionalPositionFar:
	.incbin "baserom.gba", 0x001180b8, 0x00000008
	.section .rom.001180c0, "ax"
	.global BattleParty_ListLivingUnitsFar
	.type BattleParty_ListLivingUnitsFar, %function
	.thumb_func
BattleParty_ListLivingUnitsFar:
	.incbin "baserom.gba", 0x001180c0, 0x00000008
	.section .rom.001180c8, "ax"
	.global BattleParty_PrepareActiveOwnersFar
	.type BattleParty_PrepareActiveOwnersFar, %function
	.thumb_func
BattleParty_PrepareActiveOwnersFar:
	.incbin "baserom.gba", 0x001180c8, 0x00000008
	.section .rom.001180d0, "ax"
	.global Camera_ConfigureSceneFar
	.type Camera_ConfigureSceneFar, %function
	.thumb_func
Camera_ConfigureSceneFar:
	.incbin "baserom.gba", 0x001180d0, 0x00000008
	.section .rom.001180d8, "ax"
	.global GetMotionRecordFar
	.type GetMotionRecordFar, %function
	.thumb_func
GetMotionRecordFar:
	.incbin "baserom.gba", 0x001180d8, 0x00000008
	.section .rom.001180e0, "ax"
	.global BattlePres_SetActorModesFar
	.type BattlePres_SetActorModesFar, %function
	.thumb_func
BattlePres_SetActorModesFar:
	.incbin "baserom.gba", 0x001180e0, 0x00000008
	.section .rom.001180e8, "ax"
	.global Func_081180e8
	.type Func_081180e8, %function
	.thumb_func
Func_081180e8:
	.incbin "baserom.gba", 0x001180e8, 0x00000008
	.section .rom.001180f0, "ax"
	.global ColorBuffer_ScaleFar
	.type ColorBuffer_ScaleFar, %function
	.thumb_func
ColorBuffer_ScaleFar:
	.incbin "baserom.gba", 0x001180f0, 0x00000008
	.section .rom.001180f8, "ax"
	.global BattlePlacement_UpdateTimedEntriesFar
	.type BattlePlacement_UpdateTimedEntriesFar, %function
	.thumb_func
BattlePlacement_UpdateTimedEntriesFar:
	.incbin "baserom.gba", 0x001180f8, 0x00000008
	.section .rom.00118100, "ax"
	.global BattleMotion_GetSlotPaletteFar
	.type BattleMotion_GetSlotPaletteFar, %function
	.thumb_func
BattleMotion_GetSlotPaletteFar:
	.incbin "baserom.gba", 0x00118100, 0x00000008
	.section .rom.00118108, "ax"
	.global BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.type BattlePlacement_UpdateTimedEntriesTwentyTimesFar, %function
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimesFar:
	.incbin "baserom.gba", 0x00118108, 0x00000008
	.section .rom.00118110, "ax"
	.global DebugParty_LoadPresetFar
	.type DebugParty_LoadPresetFar, %function
	.thumb_func
DebugParty_LoadPresetFar:
	.incbin "baserom.gba", 0x00118110, 0x00000008
	.section .rom.00118118, "ax"
	.global BattleActor_CommitPlacementFar
	.type BattleActor_CommitPlacementFar, %function
	.thumb_func
BattleActor_CommitPlacementFar:
	.incbin "baserom.gba", 0x00118118, 0x00000008
	.section .rom.00118120, "ax"
	.global ReleaseBattleObjectRecordsFar
	.type ReleaseBattleObjectRecordsFar, %function
	.thumb_func
ReleaseBattleObjectRecordsFar:
	.incbin "baserom.gba", 0x00118120, 0x00000008
	.section .rom.00118128, "ax"
	.global Func_08118128
	.type Func_08118128, %function
	.thumb_func
Func_08118128:
	.incbin "baserom.gba", 0x00118128, 0x00000008
	.section .rom.00118130, "ax"
	.global Func_08118130
	.type Func_08118130, %function
	.thumb_func
Func_08118130:
	.incbin "baserom.gba", 0x00118130, 0x00000008
	.section .rom.00118138, "ax"
	.global BattleParty_PrepareReserveOwnersFar
	.type BattleParty_PrepareReserveOwnersFar, %function
	.thumb_func
BattleParty_PrepareReserveOwnersFar:
	.incbin "baserom.gba", 0x00118138, 0x00000008
	.section .rom.00118140, "ax"
	.global Func_08118140
	.type Func_08118140, %function
	.thumb_func
Func_08118140:
	.incbin "baserom.gba", 0x00118140, 0x00000008
	.section .rom.00118148, "ax"
	.global Func_08118148
	.type Func_08118148, %function
	.thumb_func
Func_08118148:
	.incbin "baserom.gba", 0x00118148, 0x00000008
	.section .rom.00118150, "ax"
	.global Func_08118150
	.type Func_08118150, %function
	.thumb_func
Func_08118150:
	.incbin "baserom.gba", 0x00118150, 0x00000008
	.section .rom.00118158, "ax"
	.global Func_08118158
	.type Func_08118158, %function
	.thumb_func
Func_08118158:
	.incbin "baserom.gba", 0x00118158, 0x00000008
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
	.incbin "baserom.gba", 0x001a1928, 0x00000864
	.section .rom.001a218c, "ax"
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x001a218c, 0x000003b0
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
	.incbin "baserom.gba", 0x001a729c, 0x00000298
	.section .rom.001a7534, "ax"
	.global Data_081a7514
	.type Data_081a7514, %function
	.thumb_func
Data_081a7514:
	.incbin "baserom.gba", 0x001a7534, 0x00000004
	.section .rom.001a7538, "ax"
	.global Func_081a7518
	.type Func_081a7518, %function
	.thumb_func
Func_081a7518:
	.incbin "baserom.gba", 0x001a7538, 0x000001e0
	.section .rom.001a7718, "ax"
	.global Func_081a76f8
	.type Func_081a76f8, %function
	.thumb_func
Func_081a76f8:
	.incbin "baserom.gba", 0x001a7718, 0x00000174
	.section .rom.001a788c, "ax"
	.global Data_081a786c
	.type Data_081a786c, %function
	.thumb_func
Data_081a786c:
	.incbin "baserom.gba", 0x001a788c, 0x00000004
	.section .rom.001a78e0, "ax"
	.global TitlePalette_UpdateFade
	.type TitlePalette_UpdateFade, %function
	.thumb_func
TitlePalette_UpdateFade:
	.incbin "baserom.gba", 0x001a78e0, 0x00000168
	.section .rom.001a7a48, "ax"
	.global Graphics_TransformPaletteBuffer
	.type Graphics_TransformPaletteBuffer, %function
	.thumb_func
Graphics_TransformPaletteBuffer:
	.incbin "baserom.gba", 0x001a7a48, 0x00000724
	.section .rom.001a82a6, "ax"
	.incbin "baserom.gba", 0x001a82a6, 0x000004bc
	.section .rom.001a885e, "ax"
	.incbin "baserom.gba", 0x001a885e, 0x00000002
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
	.section .rom.0068a0ca, "ax"
	.incbin "baserom.gba", 0x0068a0ca, 0x00000002
	.section .rom.006a45cd, "ax"
	.incbin "baserom.gba", 0x006a45cd, 0x00000003
	.section .rom.006a9fa2, "ax"
	.incbin "baserom.gba", 0x006a9fa2, 0x00000002
	.section .rom.006b15bf, "ax"
	.incbin "baserom.gba", 0x006b15bf, 0x00000001
	.section .rom.006cd800, "ax"
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd800, 0x00000f0c
	.section .rom.006d10fa, "ax"
	.incbin "baserom.gba", 0x006d10fa, 0x00000002
	.section .rom.006d553e, "ax"
	.incbin "baserom.gba", 0x006d553e, 0x00000002
	.section .rom.006e5cae, "ax"
	.incbin "baserom.gba", 0x006e5cae, 0x00000002
	.section .rom.006e929e, "ax"
	.incbin "baserom.gba", 0x006e929e, 0x00000002
	.section .rom.006f4d42, "ax"
	.incbin "baserom.gba", 0x006f4d42, 0x00000002
	.section .rom.007053f2, "ax"
	.incbin "baserom.gba", 0x007053f2, 0x00000002
	.section .rom.0070ce92, "ax"
	.incbin "baserom.gba", 0x0070ce92, 0x00000002
	.section .rom.007106d2, "ax"
	.incbin "baserom.gba", 0x007106d2, 0x00000002
	.section .rom.00719b06, "ax"
	.incbin "baserom.gba", 0x00719b06, 0x00000002
	.section .rom.00720cb2, "ax"
	.incbin "baserom.gba", 0x00720cb2, 0x00000002
	.section .rom.00728b6e, "ax"
	.incbin "baserom.gba", 0x00728b6e, 0x00000002
	.section .rom.0072d5fe, "ax"
	.incbin "baserom.gba", 0x0072d5fe, 0x00000002
	.section .rom.007318b2, "ax"
	.incbin "baserom.gba", 0x007318b2, 0x00000002
	.section .rom.00735232, "ax"
	.incbin "baserom.gba", 0x00735232, 0x00000002
	.section .rom.00741a6e, "ax"
	.incbin "baserom.gba", 0x00741a6e, 0x00000002
	.section .rom.0074d1ae, "ax"
	.incbin "baserom.gba", 0x0074d1ae, 0x00000002
	.section .rom.0075057e, "ax"
	.incbin "baserom.gba", 0x0075057e, 0x00000002
	.section .rom.0076249a, "ax"
	.incbin "baserom.gba", 0x0076249a, 0x00000002
	.section .rom.00765f02, "ax"
	.incbin "baserom.gba", 0x00765f02, 0x00000002
	.section .rom.007698f2, "ax"
	.incbin "baserom.gba", 0x007698f2, 0x00000002
	.section .rom.00779e5a, "ax"
	.incbin "baserom.gba", 0x00779e5a, 0x00000002
	.section .rom.0077dd8a, "ax"
	.incbin "baserom.gba", 0x0077dd8a, 0x00000002
	.section .rom.007817ae, "ax"
	.incbin "baserom.gba", 0x007817ae, 0x00000002
	.section .rom.00789ace, "ax"
	.incbin "baserom.gba", 0x00789ace, 0x00000002
	.section .rom.00791c3a, "ax"
	.incbin "baserom.gba", 0x00791c3a, 0x00000002
	.section .rom.0079ef06, "ax"
	.incbin "baserom.gba", 0x0079ef06, 0x00000002
	.section .rom.007a2dc2, "ax"
	.incbin "baserom.gba", 0x007a2dc2, 0x00000002
	.section .rom.007a6bbe, "ax"
	.incbin "baserom.gba", 0x007a6bbe, 0x00000002
	.section .rom.007ab08e, "ax"
	.incbin "baserom.gba", 0x007ab08e, 0x00000002
	.section .rom.007b2a6a, "ax"
	.incbin "baserom.gba", 0x007b2a6a, 0x00000002
	.section .rom.007b7096, "ax"
	.incbin "baserom.gba", 0x007b7096, 0x00000002
	.section .rom.007bed12, "ax"
	.incbin "baserom.gba", 0x007bed12, 0x00000002
	.section .rom.007c290e, "ax"
	.incbin "baserom.gba", 0x007c290e, 0x00000002
	.section .rom.007c62be, "ax"
	.incbin "baserom.gba", 0x007c62be, 0x00000002
	.section .rom.007ca70e, "ax"
	.incbin "baserom.gba", 0x007ca70e, 0x00000002
	.section .rom.007ce306, "ax"
	.incbin "baserom.gba", 0x007ce306, 0x00000002
	.section .rom.007d9f66, "ax"
	.incbin "baserom.gba", 0x007d9f66, 0x00000002
	.section .rom.007ddbb6, "ax"
	.incbin "baserom.gba", 0x007ddbb6, 0x00000002
	.section .rom.007e6136, "ax"
	.incbin "baserom.gba", 0x007e6136, 0x00000002
	.section .rom.007f2786, "ax"
	.incbin "baserom.gba", 0x007f2786, 0x00000002
	.section .rom.007f923a, "ax"
	.incbin "baserom.gba", 0x007f923a, 0x00000002
	.section .rom.00802502, "ax"
	.incbin "baserom.gba", 0x00802502, 0x00000002
	.section .rom.00809f26, "ax"
	.incbin "baserom.gba", 0x00809f26, 0x00000002
	.section .rom.0081fba6, "ax"
	.incbin "baserom.gba", 0x0081fba6, 0x00000002
	.section .rom.00828fea, "ax"
	.incbin "baserom.gba", 0x00828fea, 0x00000002
	.section .rom.00832206, "ax"
	.incbin "baserom.gba", 0x00832206, 0x00000002
	.section .rom.0083fb2e, "ax"
	.incbin "baserom.gba", 0x0083fb2e, 0x00000002
	.section .rom.00845982, "ax"
	.incbin "baserom.gba", 0x00845982, 0x00000002
	.section .rom.0084b33a, "ax"
	.incbin "baserom.gba", 0x0084b33a, 0x00000002
	.section .rom.0084babd, "ax"
	.incbin "baserom.gba", 0x0084babd, 0x00000003
	.section .rom.0084d40f, "ax"
	.incbin "baserom.gba", 0x0084d40f, 0x00000001
	.section .rom.0085022d, "ax"
	.incbin "baserom.gba", 0x0085022d, 0x00000003
	.section .rom.00854142, "ax"
	.incbin "baserom.gba", 0x00854142, 0x00000002
	.section .rom.008583c7, "ax"
	.incbin "baserom.gba", 0x008583c7, 0x00000001
	.section .rom.008587e7, "ax"
	.incbin "baserom.gba", 0x008587e7, 0x00000001
	.section .rom.00858a66, "ax"
	.incbin "baserom.gba", 0x00858a66, 0x00000002
	.section .rom.00858d86, "ax"
	.incbin "baserom.gba", 0x00858d86, 0x00000002
	.section .rom.008591a9, "ax"
	.incbin "baserom.gba", 0x008591a9, 0x00000003
	.section .rom.0085954e, "ax"
	.incbin "baserom.gba", 0x0085954e, 0x00000002
	.section .rom.008598bd, "ax"
	.incbin "baserom.gba", 0x008598bd, 0x00000003
	.section .rom.0085a47b, "ax"
	.incbin "baserom.gba", 0x0085a47b, 0x00000001
	.section .rom.0085a587, "ax"
	.incbin "baserom.gba", 0x0085a587, 0x00000001
	.section .rom.0085a725, "ax"
	.incbin "baserom.gba", 0x0085a725, 0x00000003
	.section .rom.0085ab92, "ax"
	.incbin "baserom.gba", 0x0085ab92, 0x00000002
	.section .rom.0085b10d, "ax"
	.incbin "baserom.gba", 0x0085b10d, 0x00000003
	.section .rom.0085bc0b, "ax"
	.incbin "baserom.gba", 0x0085bc0b, 0x00000001
	.section .rom.0085be0d, "ax"
	.incbin "baserom.gba", 0x0085be0d, 0x00000003
	.section .rom.0085e4af, "ax"
	.incbin "baserom.gba", 0x0085e4af, 0x00000001
	.section .rom.0085ec2a, "ax"
	.incbin "baserom.gba", 0x0085ec2a, 0x00000002
	.section .rom.0086026f, "ax"
	.incbin "baserom.gba", 0x0086026f, 0x00000001
	.section .rom.00860799, "ax"
	.incbin "baserom.gba", 0x00860799, 0x00000003
	.section .rom.00860fa2, "ax"
	.incbin "baserom.gba", 0x00860fa2, 0x00000002
	.section .rom.0086151b, "ax"
	.incbin "baserom.gba", 0x0086151b, 0x00000001
	.section .rom.00862c5a, "ax"
	.incbin "baserom.gba", 0x00862c5a, 0x00000002
	.section .rom.00865b23, "ax"
	.incbin "baserom.gba", 0x00865b23, 0x00000001
	.section .rom.00865dd5, "ax"
	.incbin "baserom.gba", 0x00865dd5, 0x00000003
	.section .rom.008671b7, "ax"
	.incbin "baserom.gba", 0x008671b7, 0x00000001
	.section .rom.0086b9f3, "ax"
	.incbin "baserom.gba", 0x0086b9f3, 0x00000001
	.section .rom.0086f199, "ax"
	.incbin "baserom.gba", 0x0086f199, 0x00000003
	.section .rom.00870ebe, "ax"
	.incbin "baserom.gba", 0x00870ebe, 0x00000002
	.section .rom.00872dbd, "ax"
	.incbin "baserom.gba", 0x00872dbd, 0x00000003
	.section .rom.0087476b, "ax"
	.incbin "baserom.gba", 0x0087476b, 0x00000001
	.section .rom.00875ce3, "ax"
	.incbin "baserom.gba", 0x00875ce3, 0x00000001
	.section .rom.00879916, "ax"
	.incbin "baserom.gba", 0x00879916, 0x00000002
	.section .rom.0087a474, "ax"
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a474, 0x00001c50
	.section .rom.0087c504, "ax"
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c504, 0x0000024c
	.section .rom.0087c750, "ax"
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c750, 0x00000198
	.section .rom.0087d111, "ax"
	.incbin "baserom.gba", 0x0087d111, 0x00000003
	.section .rom.0087e5e3, "ax"
	.incbin "baserom.gba", 0x0087e5e3, 0x00000001
	.section .rom.0088074d, "ax"
	.incbin "baserom.gba", 0x0088074d, 0x00000003
	.section .rom.00880aa3, "ax"
	.incbin "baserom.gba", 0x00880aa3, 0x00000001
	.section .rom.00880c27, "ax"
	.incbin "baserom.gba", 0x00880c27, 0x00000001
	.section .rom.008814e1, "ax"
	.incbin "baserom.gba", 0x008814e1, 0x00000003
	.section .rom.008830c5, "ax"
	.incbin "baserom.gba", 0x008830c5, 0x00000003
	.section .rom.00884c22, "ax"
	.incbin "baserom.gba", 0x00884c22, 0x00000002
	.section .rom.00885061, "ax"
	.incbin "baserom.gba", 0x00885061, 0x00000003
	.section .rom.00885476, "ax"
	.incbin "baserom.gba", 0x00885476, 0x00000002
	.section .rom.008857ad, "ax"
	.incbin "baserom.gba", 0x008857ad, 0x00000003
	.section .rom.0088687a, "ax"
	.incbin "baserom.gba", 0x0088687a, 0x00000002
	.section .rom.0088687c, "ax"
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088687c, 0x000002e8
	.section .rom.00886cb6, "ax"
	.incbin "baserom.gba", 0x00886cb6, 0x00000002
	.section .rom.008891c3, "ax"
	.incbin "baserom.gba", 0x008891c3, 0x00000001
	.section .rom.00889b7f, "ax"
	.incbin "baserom.gba", 0x00889b7f, 0x00000001
	.section .rom.0088acc2, "ax"
	.incbin "baserom.gba", 0x0088acc2, 0x00000002
	.section .rom.0088b940, "ax"
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b940, 0x0000002c
	.section .rom.0088b96c, "ax"
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b96c, 0x0000002c
	.section .rom.0088c155, "ax"
	.incbin "baserom.gba", 0x0088c155, 0x00000003
	.section .rom.0088d6d3, "ax"
	.incbin "baserom.gba", 0x0088d6d3, 0x00000001
	.section .rom.0088d9fd, "ax"
	.incbin "baserom.gba", 0x0088d9fd, 0x00000003
	.section .rom.0088df26, "ax"
	.incbin "baserom.gba", 0x0088df26, 0x00000002
	.section .rom.0088e456, "ax"
	.incbin "baserom.gba", 0x0088e456, 0x00000002
	.section .rom.0088f1d1, "ax"
	.incbin "baserom.gba", 0x0088f1d1, 0x00000003
	.section .rom.0088f7f1, "ax"
	.incbin "baserom.gba", 0x0088f7f1, 0x00000003
	.section .rom.0088fe1d, "ax"
	.incbin "baserom.gba", 0x0088fe1d, 0x00000003
	.section .rom.00890227, "ax"
	.incbin "baserom.gba", 0x00890227, 0x00000001
	.section .rom.00890481, "ax"
	.incbin "baserom.gba", 0x00890481, 0x00000003
	.section .rom.008909ff, "ax"
	.incbin "baserom.gba", 0x008909ff, 0x00000001
	.section .rom.00890ca9, "ax"
	.incbin "baserom.gba", 0x00890ca9, 0x00000003
	.section .rom.00891cc5, "ax"
	.incbin "baserom.gba", 0x00891cc5, 0x00000003
	.section .rom.00894777, "ax"
	.incbin "baserom.gba", 0x00894777, 0x00000001
	.section .rom.00895943, "ax"
	.incbin "baserom.gba", 0x00895943, 0x00000001
	.section .rom.00895fa3, "ax"
	.incbin "baserom.gba", 0x00895fa3, 0x00000001
	.section .rom.00896fdd, "ax"
	.incbin "baserom.gba", 0x00896fdd, 0x00000003
	.section .rom.00897633, "ax"
	.incbin "baserom.gba", 0x00897633, 0x00000001
	.section .rom.00897cb1, "ax"
	.incbin "baserom.gba", 0x00897cb1, 0x00000003
	.section .rom.008982d1, "ax"
	.incbin "baserom.gba", 0x008982d1, 0x00000003
	.section .rom.0089969e, "ax"
	.incbin "baserom.gba", 0x0089969e, 0x00000002
	.section .rom.0089a6e2, "ax"
	.incbin "baserom.gba", 0x0089a6e2, 0x00000002
	.section .rom.0089b16b, "ax"
	.incbin "baserom.gba", 0x0089b16b, 0x00000001
	.section .rom.0089b436, "ax"
	.incbin "baserom.gba", 0x0089b436, 0x00000002
	.section .rom.0089c171, "ax"
	.incbin "baserom.gba", 0x0089c171, 0x00000003
	.section .rom.0089d3ad, "ax"
	.incbin "baserom.gba", 0x0089d3ad, 0x00000003
	.section .rom.0089dcf7, "ax"
	.incbin "baserom.gba", 0x0089dcf7, 0x00000001
	.section .rom.0089e352, "ax"
	.incbin "baserom.gba", 0x0089e352, 0x00000002
	.section .rom.0089e87e, "ax"
	.incbin "baserom.gba", 0x0089e87e, 0x00000002
	.section .rom.008a0b39, "ax"
	.incbin "baserom.gba", 0x008a0b39, 0x00000003
	.section .rom.008a22cd, "ax"
	.incbin "baserom.gba", 0x008a22cd, 0x00000003
	.section .rom.008a29b1, "ax"
	.incbin "baserom.gba", 0x008a29b1, 0x00000003
	.section .rom.008a29b4, "ax"
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a29b4, 0x00001f4c
	.section .rom.008a6009, "ax"
	.incbin "baserom.gba", 0x008a6009, 0x00000003
	.section .rom.008a64fa, "ax"
	.incbin "baserom.gba", 0x008a64fa, 0x00000002
	.section .rom.008a6b42, "ax"
	.incbin "baserom.gba", 0x008a6b42, 0x00000002
	.section .rom.008a7767, "ax"
	.incbin "baserom.gba", 0x008a7767, 0x00000001
	.section .rom.008a7b29, "ax"
	.incbin "baserom.gba", 0x008a7b29, 0x00000003
	.section .rom.008a7cf3, "ax"
	.incbin "baserom.gba", 0x008a7cf3, 0x00000001
	.section .rom.008a823e, "ax"
	.incbin "baserom.gba", 0x008a823e, 0x00000002
	.section .rom.008a858a, "ax"
	.incbin "baserom.gba", 0x008a858a, 0x00000002
	.section .rom.008a858c, "ax"
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a858c, 0x0000076c
	.section .rom.008a933b, "ax"
	.incbin "baserom.gba", 0x008a933b, 0x00000001
	.section .rom.008a933c, "ax"
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a933c, 0x000000a8
	.section .rom.008a96fa, "ax"
	.incbin "baserom.gba", 0x008a96fa, 0x00000002
	.section .rom.008aa0a1, "ax"
	.incbin "baserom.gba", 0x008aa0a1, 0x00000003
	.section .rom.008aa0a4, "ax"
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008aa0a4, 0x000002f8
	.section .rom.008aaec9, "ax"
	.incbin "baserom.gba", 0x008aaec9, 0x00000003
	.section .rom.008aaecc, "ax"
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aaecc, 0x00000100
	.section .rom.008abc3d, "ax"
	.incbin "baserom.gba", 0x008abc3d, 0x00000003
	.section .rom.008ac485, "ax"
	.incbin "baserom.gba", 0x008ac485, 0x00000003
	.section .rom.008ae547, "ax"
	.incbin "baserom.gba", 0x008ae547, 0x00000001
	.section .rom.008af023, "ax"
	.incbin "baserom.gba", 0x008af023, 0x00000001
	.section .rom.008af6e6, "ax"
	.incbin "baserom.gba", 0x008af6e6, 0x00000002
	.section .rom.008af9cd, "ax"
	.incbin "baserom.gba", 0x008af9cd, 0x00000003
	.section .rom.008b0d33, "ax"
	.incbin "baserom.gba", 0x008b0d33, 0x00000001
	.section .rom.008b111d, "ax"
	.incbin "baserom.gba", 0x008b111d, 0x00000003
	.section .rom.008b14ef, "ax"
	.incbin "baserom.gba", 0x008b14ef, 0x00000001
	.section .rom.008b1d8f, "ax"
	.incbin "baserom.gba", 0x008b1d8f, 0x00000001
	.section .rom.008b2232, "ax"
	.incbin "baserom.gba", 0x008b2232, 0x00000002
	.section .rom.008b33eb, "ax"
	.incbin "baserom.gba", 0x008b33eb, 0x00000001
	.section .rom.008b48ae, "ax"
	.incbin "baserom.gba", 0x008b48ae, 0x00000002
	.section .rom.008b5877, "ax"
	.incbin "baserom.gba", 0x008b5877, 0x00000001
	.section .rom.008b5ad2, "ax"
	.incbin "baserom.gba", 0x008b5ad2, 0x00000002
	.section .rom.008b752d, "ax"
	.incbin "baserom.gba", 0x008b752d, 0x00000003
	.section .rom.008b7a79, "ax"
	.incbin "baserom.gba", 0x008b7a79, 0x00000003
	.section .rom.008b9802, "ax"
	.incbin "baserom.gba", 0x008b9802, 0x00000002
	.section .rom.008b9a7b, "ax"
	.incbin "baserom.gba", 0x008b9a7b, 0x00000001
	.section .rom.008b9f42, "ax"
	.incbin "baserom.gba", 0x008b9f42, 0x00000002
	.section .rom.008bbb17, "ax"
	.incbin "baserom.gba", 0x008bbb17, 0x00000001
	.section .rom.008bd715, "ax"
	.incbin "baserom.gba", 0x008bd715, 0x00000003
	.section .rom.008bd936, "ax"
	.incbin "baserom.gba", 0x008bd936, 0x00000002
	.section .rom.008bdd73, "ax"
	.incbin "baserom.gba", 0x008bdd73, 0x00000001
	.section .rom.008bde85, "ax"
	.incbin "baserom.gba", 0x008bde85, 0x00000003
	.section .rom.008be467, "ax"
	.incbin "baserom.gba", 0x008be467, 0x00000001
	.section .rom.008be905, "ax"
	.incbin "baserom.gba", 0x008be905, 0x00000003
	.section .rom.008bf61a, "ax"
	.incbin "baserom.gba", 0x008bf61a, 0x00000002
	.section .rom.008c039a, "ax"
	.incbin "baserom.gba", 0x008c039a, 0x00000002
	.section .rom.008c072b, "ax"
	.incbin "baserom.gba", 0x008c072b, 0x00000001
	.section .rom.008c2472, "ax"
	.incbin "baserom.gba", 0x008c2472, 0x00000002
	.section .rom.008c3249, "ax"
	.incbin "baserom.gba", 0x008c3249, 0x00000003
	.section .rom.008c3a76, "ax"
	.incbin "baserom.gba", 0x008c3a76, 0x00000002
	.section .rom.008c4022, "ax"
	.incbin "baserom.gba", 0x008c4022, 0x00000002
	.section .rom.008c41df, "ax"
	.incbin "baserom.gba", 0x008c41df, 0x00000001
	.section .rom.008c4f37, "ax"
	.incbin "baserom.gba", 0x008c4f37, 0x00000001
	.section .rom.008c58a9, "ax"
	.incbin "baserom.gba", 0x008c58a9, 0x00000003
	.section .rom.008c5c57, "ax"
	.incbin "baserom.gba", 0x008c5c57, 0x00000001
	.section .rom.008c7bfb, "ax"
	.incbin "baserom.gba", 0x008c7bfb, 0x00000001
	.section .rom.008c8997, "ax"
	.incbin "baserom.gba", 0x008c8997, 0x00000001
	.section .rom.008c8bb3, "ax"
	.incbin "baserom.gba", 0x008c8bb3, 0x00000001
	.section .rom.008c8eaf, "ax"
	.incbin "baserom.gba", 0x008c8eaf, 0x00000001
	.section .rom.008cb05d, "ax"
	.incbin "baserom.gba", 0x008cb05d, 0x00000003
	.section .rom.008cbe2b, "ax"
	.incbin "baserom.gba", 0x008cbe2b, 0x00000001
	.section .rom.008cd095, "ax"
	.incbin "baserom.gba", 0x008cd095, 0x00000003
	.section .rom.008cd5ef, "ax"
	.incbin "baserom.gba", 0x008cd5ef, 0x00000001
	.section .rom.008ceed9, "ax"
	.incbin "baserom.gba", 0x008ceed9, 0x00000003
	.section .rom.008cf77a, "ax"
	.incbin "baserom.gba", 0x008cf77a, 0x00000002
	.section .rom.008cfb57, "ax"
	.incbin "baserom.gba", 0x008cfb57, 0x00000001
	.section .rom.008cfde1, "ax"
	.incbin "baserom.gba", 0x008cfde1, 0x00000003
	.section .rom.008d0187, "ax"
	.incbin "baserom.gba", 0x008d0187, 0x00000001
	.section .rom.008d03e2, "ax"
	.incbin "baserom.gba", 0x008d03e2, 0x00000002
	.section .rom.008d079a, "ax"
	.incbin "baserom.gba", 0x008d079a, 0x00000002
	.section .rom.008d1c83, "ax"
	.incbin "baserom.gba", 0x008d1c83, 0x00000001
	.section .rom.008d3246, "ax"
	.incbin "baserom.gba", 0x008d3246, 0x00000002
	.section .rom.008d4a8a, "ax"
	.incbin "baserom.gba", 0x008d4a8a, 0x00000002
	.section .rom.008d4e0d, "ax"
	.incbin "baserom.gba", 0x008d4e0d, 0x00000003
	.section .rom.008d5abd, "ax"
	.incbin "baserom.gba", 0x008d5abd, 0x00000003
	.section .rom.008d6605, "ax"
	.incbin "baserom.gba", 0x008d6605, 0x00000003
	.section .rom.008d679e, "ax"
	.incbin "baserom.gba", 0x008d679e, 0x00000002
	.section .rom.008d702b, "ax"
	.incbin "baserom.gba", 0x008d702b, 0x00000001
	.section .rom.008d7bfa, "ax"
	.incbin "baserom.gba", 0x008d7bfa, 0x00000002
	.section .rom.008d8112, "ax"
	.incbin "baserom.gba", 0x008d8112, 0x00000002
	.section .rom.008d8736, "ax"
	.incbin "baserom.gba", 0x008d8736, 0x00000002
	.section .rom.008d8b59, "ax"
	.incbin "baserom.gba", 0x008d8b59, 0x00000003
	.section .rom.008d8df3, "ax"
	.incbin "baserom.gba", 0x008d8df3, 0x00000001
	.section .rom.008da78f, "ax"
	.incbin "baserom.gba", 0x008da78f, 0x00000001
	.section .rom.008dc506, "ax"
	.incbin "baserom.gba", 0x008dc506, 0x00000002
	.section .rom.008de47b, "ax"
	.incbin "baserom.gba", 0x008de47b, 0x00000001
	.section .rom.008de94b, "ax"
	.incbin "baserom.gba", 0x008de94b, 0x00000001
	.section .rom.008defdd, "ax"
	.incbin "baserom.gba", 0x008defdd, 0x00000003
	.section .rom.008dfa13, "ax"
	.incbin "baserom.gba", 0x008dfa13, 0x00000001
	.section .rom.008e0de5, "ax"
	.incbin "baserom.gba", 0x008e0de5, 0x00000003
	.section .rom.008e18b6, "ax"
	.incbin "baserom.gba", 0x008e18b6, 0x00000002
	.section .rom.008e23f3, "ax"
	.incbin "baserom.gba", 0x008e23f3, 0x00000001
	.section .rom.008e2a33, "ax"
	.incbin "baserom.gba", 0x008e2a33, 0x00000001
	.section .rom.008e3fba, "ax"
	.incbin "baserom.gba", 0x008e3fba, 0x00000002
	.section .rom.008e401d, "ax"
	.incbin "baserom.gba", 0x008e401d, 0x00000003
	.section .rom.008e438b, "ax"
	.incbin "baserom.gba", 0x008e438b, 0x00000001
	.section .rom.008e4929, "ax"
	.incbin "baserom.gba", 0x008e4929, 0x00000003
	.section .rom.008e492c, "ax"
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e492c, 0x00000204
	.section .rom.008e4e69, "ax"
	.incbin "baserom.gba", 0x008e4e69, 0x00000003
	.section .rom.008e5e82, "ax"
	.incbin "baserom.gba", 0x008e5e82, 0x00000002
	.section .rom.008e948e, "ax"
	.incbin "baserom.gba", 0x008e948e, 0x00000002
	.section .rom.008e9c31, "ax"
	.incbin "baserom.gba", 0x008e9c31, 0x00000003
	.section .rom.008eafc3, "ax"
	.incbin "baserom.gba", 0x008eafc3, 0x00000001
	.section .rom.008eb3f3, "ax"
	.incbin "baserom.gba", 0x008eb3f3, 0x00000001
	.section .rom.008ed1e9, "ax"
	.incbin "baserom.gba", 0x008ed1e9, 0x00000003
	.section .rom.008edafe, "ax"
	.incbin "baserom.gba", 0x008edafe, 0x00000002
	.section .rom.008ef632, "ax"
	.incbin "baserom.gba", 0x008ef632, 0x00000002
	.section .rom.008f0681, "ax"
	.incbin "baserom.gba", 0x008f0681, 0x00000003
	.section .rom.008f1729, "ax"
	.incbin "baserom.gba", 0x008f1729, 0x00000003
	.section .rom.0093c551, "ax"
	.incbin "baserom.gba", 0x0093c551, 0x00000003
	.section .rom.0093c769, "ax"
	.incbin "baserom.gba", 0x0093c769, 0x00000003
	.section .rom.0093c8bd, "ax"
	.incbin "baserom.gba", 0x0093c8bd, 0x00000003
	.section .rom.0093cd75, "ax"
	.incbin "baserom.gba", 0x0093cd75, 0x00000003
	.section .rom.0093e4e2, "ax"
	.incbin "baserom.gba", 0x0093e4e2, 0x00000002
	.section .rom.0093fa19, "ax"
	.incbin "baserom.gba", 0x0093fa19, 0x00000003
	.section .rom.0094186f, "ax"
	.incbin "baserom.gba", 0x0094186f, 0x00000001
	.section .rom.00943983, "ax"
	.incbin "baserom.gba", 0x00943983, 0x00000001
	.section .rom.009440fd, "ax"
	.incbin "baserom.gba", 0x009440fd, 0x00000003
	.section .rom.0094677b, "ax"
	.incbin "baserom.gba", 0x0094677b, 0x00000001
	.section .rom.00947183, "ax"
	.incbin "baserom.gba", 0x00947183, 0x00000001
	.section .rom.00949ea2, "ax"
	.incbin "baserom.gba", 0x00949ea2, 0x00000002
	.section .rom.0094a07a, "ax"
	.incbin "baserom.gba", 0x0094a07a, 0x00000002
	.section .rom.0094a4d9, "ax"
	.incbin "baserom.gba", 0x0094a4d9, 0x00000003
	.section .rom.0094bace, "ax"
	.incbin "baserom.gba", 0x0094bace, 0x00000002
	.section .rom.0094cf3e, "ax"
	.incbin "baserom.gba", 0x0094cf3e, 0x00000002
	.section .rom.0094d832, "ax"
	.incbin "baserom.gba", 0x0094d832, 0x00000002
	.section .rom.0094e159, "ax"
	.incbin "baserom.gba", 0x0094e159, 0x00000003
	.section .rom.0094f04a, "ax"
	.incbin "baserom.gba", 0x0094f04a, 0x00000002
	.section .rom.0094fc37, "ax"
	.incbin "baserom.gba", 0x0094fc37, 0x00000001
	.section .rom.0094fe12, "ax"
	.incbin "baserom.gba", 0x0094fe12, 0x00000002
	.section .rom.00950002, "ax"
	.incbin "baserom.gba", 0x00950002, 0x00000002
	.section .rom.00950b72, "ax"
	.incbin "baserom.gba", 0x00950b72, 0x00000002
	.section .rom.00952fad, "ax"
	.incbin "baserom.gba", 0x00952fad, 0x00000003
	.section .rom.0095357f, "ax"
	.incbin "baserom.gba", 0x0095357f, 0x00000001
	.section .rom.00953a1f, "ax"
	.incbin "baserom.gba", 0x00953a1f, 0x00000001
	.section .rom.00953d72, "ax"
	.incbin "baserom.gba", 0x00953d72, 0x00000002
	.section .rom.00953d74, "ax"
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953d74, 0x000002f8
	.section .rom.00954145, "ax"
	.incbin "baserom.gba", 0x00954145, 0x00000003
	.section .rom.00954148, "ax"
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00954148, 0x000002f0
	.section .rom.009544ca, "ax"
	.incbin "baserom.gba", 0x009544ca, 0x00000002
	.section .rom.0095507d, "ax"
	.incbin "baserom.gba", 0x0095507d, 0x00000003
	.section .rom.00955d2a, "ax"
	.incbin "baserom.gba", 0x00955d2a, 0x00000002
	.section .rom.0095682e, "ax"
	.incbin "baserom.gba", 0x0095682e, 0x00000002
	.section .rom.0095776d, "ax"
	.incbin "baserom.gba", 0x0095776d, 0x00000003
	.section .rom.00957fa9, "ax"
	.incbin "baserom.gba", 0x00957fa9, 0x00000003
	.section .rom.0095944e, "ax"
	.incbin "baserom.gba", 0x0095944e, 0x00000002
	.section .rom.00959f06, "ax"
	.incbin "baserom.gba", 0x00959f06, 0x00000002
	.section .rom.0095a231, "ax"
	.incbin "baserom.gba", 0x0095a231, 0x00000003
	.section .rom.0095b02e, "ax"
	.incbin "baserom.gba", 0x0095b02e, 0x00000002
	.section .rom.0095b830, "ax"
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b830, 0x00000400
	.section .rom.00967fea, "ax"
	.incbin "baserom.gba", 0x00967fea, 0x00000002
	.section .rom.00967fec, "ax"
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967fec, 0x00000100
	.section .rom.009680ec, "ax"
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x009680ec, 0x000004c8
	.section .rom.009685b4, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009685b4, 0x00000268
	.section .rom.0096881c, "ax"
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x0096881c, 0x000001c8
	.section .rom.00968e4b, "ax"
	.incbin "baserom.gba", 0x00968e4b, 0x00000001
	.section .rom.00969057, "ax"
	.incbin "baserom.gba", 0x00969057, 0x00000001
	.section .rom.0096925b, "ax"
	.incbin "baserom.gba", 0x0096925b, 0x00000001
	.section .rom.009692ea, "ax"
	.incbin "baserom.gba", 0x009692ea, 0x00000002
	.section .rom.009697cc, "ax"
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x009697cc, 0x00000070
	.section .rom.00969897, "ax"
	.incbin "baserom.gba", 0x00969897, 0x00000001
	.section .rom.009698ca, "ax"
	.incbin "baserom.gba", 0x009698ca, 0x00000002
	.section .rom.00969a96, "ax"
	.incbin "baserom.gba", 0x00969a96, 0x00000002
	.section .rom.00969a98, "ax"
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969a98, 0x0000024c
	.section .rom.00969dc6, "ax"
	.incbin "baserom.gba", 0x00969dc6, 0x00000002
	.section .rom.00969e81, "ax"
	.incbin "baserom.gba", 0x00969e81, 0x00000003
	.section .rom.0096a056, "ax"
	.incbin "baserom.gba", 0x0096a056, 0x00000002
	.section .rom.0096a0f0, "ax"
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a0f0, 0x00000024
	.section .rom.0096a266, "ax"
	.incbin "baserom.gba", 0x0096a266, 0x00000002
	.section .rom.0096a349, "ax"
	.incbin "baserom.gba", 0x0096a349, 0x00000003
	.section .rom.0096a419, "ax"
	.incbin "baserom.gba", 0x0096a419, 0x00000003
	.section .rom.0096a53f, "ax"
	.incbin "baserom.gba", 0x0096a53f, 0x00000001
	.section .rom.0096a56e, "ax"
	.incbin "baserom.gba", 0x0096a56e, 0x00000002
	.section .rom.0096a7d2, "ax"
	.incbin "baserom.gba", 0x0096a7d2, 0x00000002
	.section .rom.0096a86f, "ax"
	.incbin "baserom.gba", 0x0096a86f, 0x00000001
	.section .rom.0096a8d4, "ax"
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a8d4, 0x00000400
	.section .rom.0096acd4, "ax"
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096acd4, 0x000000b0
	.section .rom.0096ad84, "ax"
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096ad84, 0x00000560
	.section .rom.0096b2e4, "ax"
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b2e4, 0x00000074
	.section .rom.0096b358, "ax"
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b358, 0x00000048
	.section .rom.0096b3a0, "ax"
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b3a0, 0x00000050
	.section .rom.0096b3f0, "ax"
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b3f0, 0x00000050
	.section .rom.0096b440, "ax"
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b440, 0x00000058
	.section .rom.0096b498, "ax"
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b498, 0x00000058
	.section .rom.0096b4f0, "ax"
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b4f0, 0x00000040
	.section .rom.0096b530, "ax"
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b530, 0x00000044
	.section .rom.0096b574, "ax"
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b574, 0x00000054
	.section .rom.009707ff, "ax"
	.incbin "baserom.gba", 0x009707ff, 0x00000001
	.section .rom.0097374a, "ax"
	.incbin "baserom.gba", 0x0097374a, 0x00000002
	.section .rom.0097736d, "ax"
	.incbin "baserom.gba", 0x0097736d, 0x00000003
	.section .rom.009795b9, "ax"
	.incbin "baserom.gba", 0x009795b9, 0x00000003
	.section .rom.0097eb55, "ax"
	.incbin "baserom.gba", 0x0097eb55, 0x00000003
	.section .rom.009823cf, "ax"
	.incbin "baserom.gba", 0x009823cf, 0x00000001
	.section .rom.00988239, "ax"
	.incbin "baserom.gba", 0x00988239, 0x00000003
	.section .rom.0098930d, "ax"
	.incbin "baserom.gba", 0x0098930d, 0x00000003
	.section .rom.0098a379, "ax"
	.incbin "baserom.gba", 0x0098a379, 0x00000003
	.section .rom.0098f9d3, "ax"
	.incbin "baserom.gba", 0x0098f9d3, 0x00000001
	.section .rom.00992b15, "ax"
	.incbin "baserom.gba", 0x00992b15, 0x00000003
	.section .rom.00994bb5, "ax"
	.incbin "baserom.gba", 0x00994bb5, 0x00000003
	.section .rom.00996ed5, "ax"
	.incbin "baserom.gba", 0x00996ed5, 0x00000003
	.section .rom.00999b4e, "ax"
	.incbin "baserom.gba", 0x00999b4e, 0x00000002
	.section .rom.0099ba49, "ax"
	.incbin "baserom.gba", 0x0099ba49, 0x00000003
	.section .rom.009a3f8d, "ax"
	.incbin "baserom.gba", 0x009a3f8d, 0x00000003
	.section .rom.009acc0f, "ax"
	.incbin "baserom.gba", 0x009acc0f, 0x00000001
	.section .rom.009af789, "ax"
	.incbin "baserom.gba", 0x009af789, 0x00000003
	.section .rom.009b61c6, "ax"
	.incbin "baserom.gba", 0x009b61c6, 0x00000002
	.section .rom.009b8cba, "ax"
	.incbin "baserom.gba", 0x009b8cba, 0x00000002
	.section .rom.009ba7aa, "ax"
	.incbin "baserom.gba", 0x009ba7aa, 0x00000002
	.section .rom.009c04ff, "ax"
	.incbin "baserom.gba", 0x009c04ff, 0x00000001
	.section .rom.009c4ea6, "ax"
	.incbin "baserom.gba", 0x009c4ea6, 0x00000002
	.section .rom.009c5e42, "ax"
	.incbin "baserom.gba", 0x009c5e42, 0x00000002
	.section .rom.009c856d, "ax"
	.incbin "baserom.gba", 0x009c856d, 0x00000003
	.section .rom.009c9c82, "ax"
	.incbin "baserom.gba", 0x009c9c82, 0x00000002
	.section .rom.009d249b, "ax"
	.incbin "baserom.gba", 0x009d249b, 0x00000001
	.section .rom.009d6763, "ax"
	.incbin "baserom.gba", 0x009d6763, 0x00000001
	.section .rom.009ddf9d, "ax"
	.incbin "baserom.gba", 0x009ddf9d, 0x00000003
	.section .rom.009df782, "ax"
	.incbin "baserom.gba", 0x009df782, 0x00000002
	.section .rom.009e2519, "ax"
	.incbin "baserom.gba", 0x009e2519, 0x00000003
	.section .rom.009e3563, "ax"
	.incbin "baserom.gba", 0x009e3563, 0x00000001
	.section .rom.009ebaf3, "ax"
	.incbin "baserom.gba", 0x009ebaf3, 0x00000001
	.section .rom.009ed8ed, "ax"
	.incbin "baserom.gba", 0x009ed8ed, 0x00000003
	.section .rom.009eecb3, "ax"
	.incbin "baserom.gba", 0x009eecb3, 0x00000001
	.section .rom.009f5bb5, "ax"
	.incbin "baserom.gba", 0x009f5bb5, 0x00000003
	.section .rom.009f7eb6, "ax"
	.incbin "baserom.gba", 0x009f7eb6, 0x00000002
	.section .rom.009fd103, "ax"
	.incbin "baserom.gba", 0x009fd103, 0x00000001
	.section .rom.00a02502, "ax"
	.incbin "baserom.gba", 0x00a02502, 0x00000002
	.section .rom.00a046b6, "ax"
	.incbin "baserom.gba", 0x00a046b6, 0x00000002
	.section .rom.00a0881d, "ax"
	.incbin "baserom.gba", 0x00a0881d, 0x00000003
	.section .rom.00a0ba42, "ax"
	.incbin "baserom.gba", 0x00a0ba42, 0x00000002
	.section .rom.00a0d146, "ax"
	.incbin "baserom.gba", 0x00a0d146, 0x00000002
	.section .rom.00a13d2e, "ax"
	.incbin "baserom.gba", 0x00a13d2e, 0x00000002
	.section .rom.00a2030e, "ax"
	.incbin "baserom.gba", 0x00a2030e, 0x00000002
	.section .rom.00a233a2, "ax"
	.incbin "baserom.gba", 0x00a233a2, 0x00000002
	.section .rom.00a25c0d, "ax"
	.incbin "baserom.gba", 0x00a25c0d, 0x00000003
	.section .rom.00a276fe, "ax"
	.incbin "baserom.gba", 0x00a276fe, 0x00000002
	.section .rom.00a28516, "ax"
	.incbin "baserom.gba", 0x00a28516, 0x00000002
	.section .rom.00a29201, "ax"
	.incbin "baserom.gba", 0x00a29201, 0x00000003
	.section .rom.00a326be, "ax"
	.incbin "baserom.gba", 0x00a326be, 0x00000002
	.section .rom.00a333c9, "ax"
	.incbin "baserom.gba", 0x00a333c9, 0x00000003
	.section .rom.00a3462d, "ax"
	.incbin "baserom.gba", 0x00a3462d, 0x00000003
	.section .rom.00a3555f, "ax"
	.incbin "baserom.gba", 0x00a3555f, 0x00000001
	.section .rom.00a361cf, "ax"
	.incbin "baserom.gba", 0x00a361cf, 0x00000001
	.section .rom.00a36de7, "ax"
	.incbin "baserom.gba", 0x00a36de7, 0x00000001
	.section .rom.00a3755b, "ax"
	.incbin "baserom.gba", 0x00a3755b, 0x00000001
	.section .rom.00a38deb, "ax"
	.incbin "baserom.gba", 0x00a38deb, 0x00000001
	.section .rom.00a397b9, "ax"
	.incbin "baserom.gba", 0x00a397b9, 0x00000003
	.section .rom.00a3a3d7, "ax"
	.incbin "baserom.gba", 0x00a3a3d7, 0x00000001
	.section .rom.00a3ca8b, "ax"
	.incbin "baserom.gba", 0x00a3ca8b, 0x00000001
	.section .rom.00a3f19e, "ax"
	.incbin "baserom.gba", 0x00a3f19e, 0x00000002
	.section .rom.00a440f3, "ax"
	.incbin "baserom.gba", 0x00a440f3, 0x00000001
	.section .rom.00a4841d, "ax"
	.incbin "baserom.gba", 0x00a4841d, 0x00000003
	.section .rom.00a4900a, "ax"
	.incbin "baserom.gba", 0x00a4900a, 0x00000002
	.section .rom.00a4dbbf, "ax"
	.incbin "baserom.gba", 0x00a4dbbf, 0x00000001
	.section .rom.00a4ff29, "ax"
	.incbin "baserom.gba", 0x00a4ff29, 0x00000003
	.section .rom.00a518cb, "ax"
	.incbin "baserom.gba", 0x00a518cb, 0x00000001
	.section .rom.00a5618d, "ax"
	.incbin "baserom.gba", 0x00a5618d, 0x00000003
	.section .rom.00a5959d, "ax"
	.incbin "baserom.gba", 0x00a5959d, 0x00000003
	.section .rom.00a5d665, "ax"
	.incbin "baserom.gba", 0x00a5d665, 0x00000003
	.section .rom.00a60d2e, "ax"
	.incbin "baserom.gba", 0x00a60d2e, 0x00000002
	.section .rom.00a68cff, "ax"
	.incbin "baserom.gba", 0x00a68cff, 0x00000001
	.section .rom.00a7000f, "ax"
	.incbin "baserom.gba", 0x00a7000f, 0x00000001
	.section .rom.00a74f8f, "ax"
	.incbin "baserom.gba", 0x00a74f8f, 0x00000001
	.section .rom.00a79a11, "ax"
	.incbin "baserom.gba", 0x00a79a11, 0x00000003
	.section .rom.00a79a1f, "ax"
	.incbin "baserom.gba", 0x00a79a1f, 0x00000001
	.section .rom.00a79b6e, "ax"
	.incbin "baserom.gba", 0x00a79b6e, 0x00000002
	.section .rom.00a79caf, "ax"
	.incbin "baserom.gba", 0x00a79caf, 0x00000001
	.section .rom.00a79def, "ax"
	.incbin "baserom.gba", 0x00a79def, 0x00000001
	.section .rom.00a79f2f, "ax"
	.incbin "baserom.gba", 0x00a79f2f, 0x00000001
	.section .rom.00a7b18d, "ax"
	.incbin "baserom.gba", 0x00a7b18d, 0x00000003
	.section .rom.00a7b35e, "ax"
	.incbin "baserom.gba", 0x00a7b35e, 0x00000002
	.section .rom.00a7d3ff, "ax"
	.incbin "baserom.gba", 0x00a7d3ff, 0x00000001
	.section .rom.00a80661, "ax"
	.incbin "baserom.gba", 0x00a80661, 0x00000003
	.section .rom.00a85ed3, "ax"
	.incbin "baserom.gba", 0x00a85ed3, 0x00000001
	.section .rom.00a85fda, "ax"
	.incbin "baserom.gba", 0x00a85fda, 0x00000002
	.section .rom.00a871c5, "ax"
	.incbin "baserom.gba", 0x00a871c5, 0x00000003
	.section .rom.00a88e4d, "ax"
	.incbin "baserom.gba", 0x00a88e4d, 0x00000003
	.section .rom.00a8935b, "ax"
	.incbin "baserom.gba", 0x00a8935b, 0x00000001
	.section .rom.00a8949b, "ax"
	.incbin "baserom.gba", 0x00a8949b, 0x00000001
	.section .rom.00a8b11e, "ax"
	.incbin "baserom.gba", 0x00a8b11e, 0x00000002
	.section .rom.00a8daf6, "ax"
	.incbin "baserom.gba", 0x00a8daf6, 0x00000002
	.section .rom.00a902bf, "ax"
	.incbin "baserom.gba", 0x00a902bf, 0x00000001
	.section .rom.00a917df, "ax"
	.incbin "baserom.gba", 0x00a917df, 0x00000001
	.section .rom.00a93123, "ax"
	.incbin "baserom.gba", 0x00a93123, 0x00000001
	.section .rom.00a94bd1, "ax"
	.incbin "baserom.gba", 0x00a94bd1, 0x00000003
	.section .rom.00a94d45, "ax"
	.incbin "baserom.gba", 0x00a94d45, 0x00000003
	.section .rom.00a97b29, "ax"
	.incbin "baserom.gba", 0x00a97b29, 0x00000003
	.section .rom.00a9a3d3, "ax"
	.incbin "baserom.gba", 0x00a9a3d3, 0x00000001
	.section .rom.00a9e86a, "ax"
	.incbin "baserom.gba", 0x00a9e86a, 0x00000002
	.section .rom.00aa1be2, "ax"
	.incbin "baserom.gba", 0x00aa1be2, 0x00000002
	.section .rom.00aa46df, "ax"
	.incbin "baserom.gba", 0x00aa46df, 0x00000001
	.section .rom.00aa67bd, "ax"
	.incbin "baserom.gba", 0x00aa67bd, 0x00000003
	.section .rom.00aa8a09, "ax"
	.incbin "baserom.gba", 0x00aa8a09, 0x00000003
	.section .rom.00aa8b4b, "ax"
	.incbin "baserom.gba", 0x00aa8b4b, 0x00000001
	.section .rom.00aaa21e, "ax"
	.incbin "baserom.gba", 0x00aaa21e, 0x00000002
	.section .rom.00aaa31e, "ax"
	.incbin "baserom.gba", 0x00aaa31e, 0x00000002
	.section .rom.00aac211, "ax"
	.incbin "baserom.gba", 0x00aac211, 0x00000003
	.section .rom.00aadbe9, "ax"
	.incbin "baserom.gba", 0x00aadbe9, 0x00000003
	.section .rom.00ab061a, "ax"
	.incbin "baserom.gba", 0x00ab061a, 0x00000002
	.section .rom.00ab586b, "ax"
	.incbin "baserom.gba", 0x00ab586b, 0x00000001
	.section .rom.00ab81af, "ax"
	.incbin "baserom.gba", 0x00ab81af, 0x00000001
	.section .rom.00ab9682, "ax"
	.incbin "baserom.gba", 0x00ab9682, 0x00000002
	.section .rom.00abe44d, "ax"
	.incbin "baserom.gba", 0x00abe44d, 0x00000003
	.section .rom.00ac10f7, "ax"
	.incbin "baserom.gba", 0x00ac10f7, 0x00000001
	.section .rom.00ac432f, "ax"
	.incbin "baserom.gba", 0x00ac432f, 0x00000001
	.section .rom.00ac44c7, "ax"
	.incbin "baserom.gba", 0x00ac44c7, 0x00000001
	.section .rom.00ac72c7, "ax"
	.incbin "baserom.gba", 0x00ac72c7, 0x00000001
	.section .rom.00ac8a7b, "ax"
	.incbin "baserom.gba", 0x00ac8a7b, 0x00000001
	.section .rom.00ac9a16, "ax"
	.incbin "baserom.gba", 0x00ac9a16, 0x00000002
	.section .rom.00acc077, "ax"
	.incbin "baserom.gba", 0x00acc077, 0x00000001
	.section .rom.00acc21e, "ax"
	.incbin "baserom.gba", 0x00acc21e, 0x00000002
	.section .rom.00acf1d7, "ax"
	.incbin "baserom.gba", 0x00acf1d7, 0x00000001
	.section .rom.00acfe7d, "ax"
	.incbin "baserom.gba", 0x00acfe7d, 0x00000003
	.section .rom.00ad144d, "ax"
	.incbin "baserom.gba", 0x00ad144d, 0x00000003
	.section .rom.00ad9419, "ax"
	.incbin "baserom.gba", 0x00ad9419, 0x00000003
	.section .rom.00adaeff, "ax"
	.incbin "baserom.gba", 0x00adaeff, 0x00000001
	.section .rom.00adc3e9, "ax"
	.incbin "baserom.gba", 0x00adc3e9, 0x00000003
	.section .rom.00ae2613, "ax"
	.incbin "baserom.gba", 0x00ae2613, 0x00000001
	.section .rom.00ae2ae5, "ax"
	.incbin "baserom.gba", 0x00ae2ae5, 0x00000003
	.section .rom.00ae3c2d, "ax"
	.incbin "baserom.gba", 0x00ae3c2d, 0x00000003
	.section .rom.00ae3dde, "ax"
	.incbin "baserom.gba", 0x00ae3dde, 0x00000002
	.section .rom.00aefacb, "ax"
	.incbin "baserom.gba", 0x00aefacb, 0x00000001
	.section .rom.00aefbeb, "ax"
	.incbin "baserom.gba", 0x00aefbeb, 0x00000001
	.section .rom.00af1f8b, "ax"
	.incbin "baserom.gba", 0x00af1f8b, 0x00000001
	.section .rom.00af31d7, "ax"
	.incbin "baserom.gba", 0x00af31d7, 0x00000001
	.section .rom.00af5567, "ax"
	.incbin "baserom.gba", 0x00af5567, 0x00000001
	.section .rom.00af6e65, "ax"
	.incbin "baserom.gba", 0x00af6e65, 0x00000003
	.section .rom.00af7e56, "ax"
	.incbin "baserom.gba", 0x00af7e56, 0x00000002
	.section .rom.00afa3f2, "ax"
	.incbin "baserom.gba", 0x00afa3f2, 0x00000002
	.section .rom.00afea1a, "ax"
	.incbin "baserom.gba", 0x00afea1a, 0x00000002
	.section .rom.00aff0df, "ax"
	.incbin "baserom.gba", 0x00aff0df, 0x00000001
	.section .rom.00b01b75, "ax"
	.incbin "baserom.gba", 0x00b01b75, 0x00000003
	.section .rom.00b01cc6, "ax"
	.incbin "baserom.gba", 0x00b01cc6, 0x00000002
	.section .rom.00b040d6, "ax"
	.incbin "baserom.gba", 0x00b040d6, 0x00000002
	.section .rom.00b06257, "ax"
	.incbin "baserom.gba", 0x00b06257, 0x00000001
	.section .rom.00b06397, "ax"
	.incbin "baserom.gba", 0x00b06397, 0x00000001
	.section .rom.00b08e36, "ax"
	.incbin "baserom.gba", 0x00b08e36, 0x00000002
	.section .rom.00b08f87, "ax"
	.incbin "baserom.gba", 0x00b08f87, 0x00000001
	.section .rom.00b0b396, "ax"
	.incbin "baserom.gba", 0x00b0b396, 0x00000002
	.section .rom.00b0d517, "ax"
	.incbin "baserom.gba", 0x00b0d517, 0x00000001
	.section .rom.00b0d657, "ax"
	.incbin "baserom.gba", 0x00b0d657, 0x00000001
	.section .rom.00b10c86, "ax"
	.incbin "baserom.gba", 0x00b10c86, 0x00000002
	.section .rom.00b131ea, "ax"
	.incbin "baserom.gba", 0x00b131ea, 0x00000002
	.section .rom.00b1536b, "ax"
	.incbin "baserom.gba", 0x00b1536b, 0x00000001
	.section .rom.00b154ab, "ax"
	.incbin "baserom.gba", 0x00b154ab, 0x00000001
	.section .rom.00b1695b, "ax"
	.incbin "baserom.gba", 0x00b1695b, 0x00000001
	.section .rom.00b16a66, "ax"
	.incbin "baserom.gba", 0x00b16a66, 0x00000002
	.section .rom.00b185be, "ax"
	.incbin "baserom.gba", 0x00b185be, 0x00000002
	.section .rom.00b19c61, "ax"
	.incbin "baserom.gba", 0x00b19c61, 0x00000003
	.section .rom.00b19e22, "ax"
	.incbin "baserom.gba", 0x00b19e22, 0x00000002
	.section .rom.00b1abbe, "ax"
	.incbin "baserom.gba", 0x00b1abbe, 0x00000002
	.section .rom.00b1c816, "ax"
	.incbin "baserom.gba", 0x00b1c816, 0x00000002
	.section .rom.00b1e06d, "ax"
	.incbin "baserom.gba", 0x00b1e06d, 0x00000003
	.section .rom.00b1e22e, "ax"
	.incbin "baserom.gba", 0x00b1e22e, 0x00000002
	.section .rom.00b224a9, "ax"
	.incbin "baserom.gba", 0x00b224a9, 0x00000003
	.section .rom.00b2266a, "ax"
	.incbin "baserom.gba", 0x00b2266a, 0x00000002
	.section .rom.00b230e1, "ax"
	.incbin "baserom.gba", 0x00b230e1, 0x00000003
	.section .rom.00b231e9, "ax"
	.incbin "baserom.gba", 0x00b231e9, 0x00000003
	.section .rom.00b24eaa, "ax"
	.incbin "baserom.gba", 0x00b24eaa, 0x00000002
	.section .rom.00b26637, "ax"
	.incbin "baserom.gba", 0x00b26637, 0x00000001
	.section .rom.00b268ed, "ax"
	.incbin "baserom.gba", 0x00b268ed, 0x00000003
	.section .rom.00b283fd, "ax"
	.incbin "baserom.gba", 0x00b283fd, 0x00000003
	.section .rom.00b2ac8a, "ax"
	.incbin "baserom.gba", 0x00b2ac8a, 0x00000002
	.section .rom.00b2d47a, "ax"
	.incbin "baserom.gba", 0x00b2d47a, 0x00000002
	.section .rom.00b2e41a, "ax"
	.incbin "baserom.gba", 0x00b2e41a, 0x00000002
	.section .rom.00b3056d, "ax"
	.incbin "baserom.gba", 0x00b3056d, 0x00000003
	.section .rom.00b33bfa, "ax"
	.incbin "baserom.gba", 0x00b33bfa, 0x00000002
	.section .rom.00b3598f, "ax"
	.incbin "baserom.gba", 0x00b3598f, 0x00000001
	.section .rom.00b377ad, "ax"
	.incbin "baserom.gba", 0x00b377ad, 0x00000003
	.section .rom.00b39289, "ax"
	.incbin "baserom.gba", 0x00b39289, 0x00000003
	.section .rom.00b3a872, "ax"
	.incbin "baserom.gba", 0x00b3a872, 0x00000002
	.section .rom.00b3ce5a, "ax"
	.incbin "baserom.gba", 0x00b3ce5a, 0x00000002
	.section .rom.00b3cfd3, "ax"
	.incbin "baserom.gba", 0x00b3cfd3, 0x00000001
	.section .rom.00b3e492, "ax"
	.incbin "baserom.gba", 0x00b3e492, 0x00000002
	.section .rom.00b3fc96, "ax"
	.incbin "baserom.gba", 0x00b3fc96, 0x00000002
	.section .rom.00b4160e, "ax"
	.incbin "baserom.gba", 0x00b4160e, 0x00000002
	.section .rom.00b4252e, "ax"
	.incbin "baserom.gba", 0x00b4252e, 0x00000002
	.section .rom.00b43fda, "ax"
	.incbin "baserom.gba", 0x00b43fda, 0x00000002
	.section .rom.00b440e7, "ax"
	.incbin "baserom.gba", 0x00b440e7, 0x00000001
	.section .rom.00b46217, "ax"
	.incbin "baserom.gba", 0x00b46217, 0x00000001
	.section .rom.00b47ded, "ax"
	.incbin "baserom.gba", 0x00b47ded, 0x00000003
	.section .rom.00b483ff, "ax"
	.incbin "baserom.gba", 0x00b483ff, 0x00000001
	.section .rom.00b4853f, "ax"
	.incbin "baserom.gba", 0x00b4853f, 0x00000001
	.section .rom.00b4ac3f, "ax"
	.incbin "baserom.gba", 0x00b4ac3f, 0x00000001
	.section .rom.00b4c49e, "ax"
	.incbin "baserom.gba", 0x00b4c49e, 0x00000002
	.section .rom.00b4caaf, "ax"
	.incbin "baserom.gba", 0x00b4caaf, 0x00000001
	.section .rom.00b4cbef, "ax"
	.incbin "baserom.gba", 0x00b4cbef, 0x00000001
	.section .rom.00b4da6a, "ax"
	.incbin "baserom.gba", 0x00b4da6a, 0x00000002
	.section .rom.00b4db81, "ax"
	.incbin "baserom.gba", 0x00b4db81, 0x00000003
	.section .rom.00b4fe57, "ax"
	.incbin "baserom.gba", 0x00b4fe57, 0x00000001
	.section .rom.00b51aaa, "ax"
	.incbin "baserom.gba", 0x00b51aaa, 0x00000002
	.section .rom.00b520ef, "ax"
	.incbin "baserom.gba", 0x00b520ef, 0x00000001
	.section .rom.00b5222f, "ax"
	.incbin "baserom.gba", 0x00b5222f, 0x00000001
	.section .rom.00b52e35, "ax"
	.incbin "baserom.gba", 0x00b52e35, 0x00000003
	.section .rom.00b553e7, "ax"
	.incbin "baserom.gba", 0x00b553e7, 0x00000001
	.section .rom.00b570a6, "ax"
	.incbin "baserom.gba", 0x00b570a6, 0x00000002
	.section .rom.00b58b72, "ax"
	.incbin "baserom.gba", 0x00b58b72, 0x00000002
	.section .rom.00b59c46, "ax"
	.incbin "baserom.gba", 0x00b59c46, 0x00000002
	.section .rom.00b59dad, "ax"
	.incbin "baserom.gba", 0x00b59dad, 0x00000003
	.section .rom.00b5afc6, "ax"
	.incbin "baserom.gba", 0x00b5afc6, 0x00000002
	.section .rom.00b5bba5, "ax"
	.incbin "baserom.gba", 0x00b5bba5, 0x00000003
	.section .rom.00b5bd12, "ax"
	.incbin "baserom.gba", 0x00b5bd12, 0x00000002
	.section .rom.00b5eb63, "ax"
	.incbin "baserom.gba", 0x00b5eb63, 0x00000001
	.section .rom.00b61322, "ax"
	.incbin "baserom.gba", 0x00b61322, 0x00000002
	.section .rom.00b673d3, "ax"
	.incbin "baserom.gba", 0x00b673d3, 0x00000001
	.section .rom.00b68f03, "ax"
	.incbin "baserom.gba", 0x00b68f03, 0x00000001
	.section .rom.00b6b23a, "ax"
	.incbin "baserom.gba", 0x00b6b23a, 0x00000002
	.section .rom.00b6c3eb, "ax"
	.incbin "baserom.gba", 0x00b6c3eb, 0x00000001
	.section .rom.00b6dd0d, "ax"
	.incbin "baserom.gba", 0x00b6dd0d, 0x00000003
	.section .rom.00b6fb99, "ax"
	.incbin "baserom.gba", 0x00b6fb99, 0x00000003
	.section .rom.00b71d7a, "ax"
	.incbin "baserom.gba", 0x00b71d7a, 0x00000002
	.section .rom.00b75741, "ax"
	.incbin "baserom.gba", 0x00b75741, 0x00000003
	.section .rom.00b7582d, "ax"
	.incbin "baserom.gba", 0x00b7582d, 0x00000003
	.section .rom.00b78a66, "ax"
	.incbin "baserom.gba", 0x00b78a66, 0x00000002
	.section .rom.00b790dd, "ax"
	.incbin "baserom.gba", 0x00b790dd, 0x00000003
	.section .rom.00b7ce9f, "ax"
	.incbin "baserom.gba", 0x00b7ce9f, 0x00000001
	.section .rom.00b7f131, "ax"
	.incbin "baserom.gba", 0x00b7f131, 0x00000003
	.section .rom.00b80b4e, "ax"
	.incbin "baserom.gba", 0x00b80b4e, 0x00000002
	.section .rom.00b81c5b, "ax"
	.incbin "baserom.gba", 0x00b81c5b, 0x00000001
	.section .rom.00b84dfa, "ax"
	.incbin "baserom.gba", 0x00b84dfa, 0x00000002
	.section .rom.00b85ff2, "ax"
	.incbin "baserom.gba", 0x00b85ff2, 0x00000002
	.section .rom.00b86eb5, "ax"
	.incbin "baserom.gba", 0x00b86eb5, 0x00000003
	.section .rom.00b88f56, "ax"
	.incbin "baserom.gba", 0x00b88f56, 0x00000002
	.section .rom.00b8a9ab, "ax"
	.incbin "baserom.gba", 0x00b8a9ab, 0x00000001
	.section .rom.00b8be76, "ax"
	.incbin "baserom.gba", 0x00b8be76, 0x00000002
	.section .rom.00b8e07a, "ax"
	.incbin "baserom.gba", 0x00b8e07a, 0x00000002
	.section .rom.00b8e16f, "ax"
	.incbin "baserom.gba", 0x00b8e16f, 0x00000001
	.section .rom.00b8f742, "ax"
	.incbin "baserom.gba", 0x00b8f742, 0x00000002
	.section .rom.00b8f961, "ax"
	.incbin "baserom.gba", 0x00b8f961, 0x00000003
	.section .rom.00b91a45, "ax"
	.incbin "baserom.gba", 0x00b91a45, 0x00000003
	.section .rom.00b91c0a, "ax"
	.incbin "baserom.gba", 0x00b91c0a, 0x00000002
	.section .rom.00b93ccd, "ax"
	.incbin "baserom.gba", 0x00b93ccd, 0x00000003
	.section .rom.00b93dfd, "ax"
	.incbin "baserom.gba", 0x00b93dfd, 0x00000003
	.section .rom.00b968b6, "ax"
	.incbin "baserom.gba", 0x00b968b6, 0x00000002
	.section .rom.00b9840e, "ax"
	.incbin "baserom.gba", 0x00b9840e, 0x00000002
	.section .rom.00b99355, "ax"
	.incbin "baserom.gba", 0x00b99355, 0x00000003
	.section .rom.00b9ad5e, "ax"
	.incbin "baserom.gba", 0x00b9ad5e, 0x00000002
	.section .rom.00baaab3, "ax"
	.incbin "baserom.gba", 0x00baaab3, 0x00000001
	.section .rom.00baac03, "ax"
	.incbin "baserom.gba", 0x00baac03, 0x00000001
	.section .rom.00bad70e, "ax"
	.incbin "baserom.gba", 0x00bad70e, 0x00000002
	.section .rom.00baf317, "ax"
	.incbin "baserom.gba", 0x00baf317, 0x00000001
	.section .rom.00bb0d1e, "ax"
	.incbin "baserom.gba", 0x00bb0d1e, 0x00000002
	.section .rom.00bb2a39, "ax"
	.incbin "baserom.gba", 0x00bb2a39, 0x00000003
	.section .rom.00bb2b8b, "ax"
	.incbin "baserom.gba", 0x00bb2b8b, 0x00000001
	.section .rom.00bb4592, "ax"
	.incbin "baserom.gba", 0x00bb4592, 0x00000002
	.section .rom.00bb831a, "ax"
	.incbin "baserom.gba", 0x00bb831a, 0x00000002
	.section .rom.00bb84ad, "ax"
	.incbin "baserom.gba", 0x00bb84ad, 0x00000003
	.section .rom.00bbd4b7, "ax"
	.incbin "baserom.gba", 0x00bbd4b7, 0x00000001
	.section .rom.00bbfb39, "ax"
	.incbin "baserom.gba", 0x00bbfb39, 0x00000003
	.section .rom.00bbfe6f, "ax"
	.incbin "baserom.gba", 0x00bbfe6f, 0x00000001
	.section .rom.00bc62c5, "ax"
	.incbin "baserom.gba", 0x00bc62c5, 0x00000003
	.section .rom.00bc6412, "ax"
	.incbin "baserom.gba", 0x00bc6412, 0x00000002
	.section .rom.00bc8297, "ax"
	.incbin "baserom.gba", 0x00bc8297, 0x00000001
	.section .rom.00bc983e, "ax"
	.incbin "baserom.gba", 0x00bc983e, 0x00000002
	.section .rom.00bcb7e9, "ax"
	.incbin "baserom.gba", 0x00bcb7e9, 0x00000003
	.section .rom.00bcc75a, "ax"
	.incbin "baserom.gba", 0x00bcc75a, 0x00000002
	.section .rom.00bd7d63, "ax"
	.incbin "baserom.gba", 0x00bd7d63, 0x00000001
	.section .rom.00bd7e1f, "ax"
	.incbin "baserom.gba", 0x00bd7e1f, 0x00000001
	.section .rom.00bd8b5e, "ax"
	.incbin "baserom.gba", 0x00bd8b5e, 0x00000002
	.section .rom.00bd953b, "ax"
	.incbin "baserom.gba", 0x00bd953b, 0x00000001
	.section .rom.00bda153, "ax"
	.incbin "baserom.gba", 0x00bda153, 0x00000001
	.section .rom.00bdc00e, "ax"
	.incbin "baserom.gba", 0x00bdc00e, 0x00000002
	.section .rom.00bdc12a, "ax"
	.incbin "baserom.gba", 0x00bdc12a, 0x00000002
	.section .rom.00bde9ab, "ax"
	.incbin "baserom.gba", 0x00bde9ab, 0x00000001
	.section .rom.00be0136, "ax"
	.incbin "baserom.gba", 0x00be0136, 0x00000002
	.section .rom.00be4066, "ax"
	.incbin "baserom.gba", 0x00be4066, 0x00000002
	.section .rom.00be41af, "ax"
	.incbin "baserom.gba", 0x00be41af, 0x00000001
	.section .rom.00be685f, "ax"
	.incbin "baserom.gba", 0x00be685f, 0x00000001
	.section .rom.00be9002, "ax"
	.incbin "baserom.gba", 0x00be9002, 0x00000002
	.section .rom.00be9d8d, "ax"
	.incbin "baserom.gba", 0x00be9d8d, 0x00000003
	.section .rom.00becbce, "ax"
	.incbin "baserom.gba", 0x00becbce, 0x00000002
	.section .rom.00bef1f5, "ax"
	.incbin "baserom.gba", 0x00bef1f5, 0x00000003
	.section .rom.00bf0ee1, "ax"
	.incbin "baserom.gba", 0x00bf0ee1, 0x00000003
	.section .rom.00bf25e1, "ax"
	.incbin "baserom.gba", 0x00bf25e1, 0x00000003
	.section .rom.00bf3b5d, "ax"
	.incbin "baserom.gba", 0x00bf3b5d, 0x00000003
	.section .rom.00bf52cf, "ax"
	.incbin "baserom.gba", 0x00bf52cf, 0x00000001
	.section .rom.00bf7f5a, "ax"
	.incbin "baserom.gba", 0x00bf7f5a, 0x00000002
	.section .rom.00bf8fa6, "ax"
	.incbin "baserom.gba", 0x00bf8fa6, 0x00000002
	.section .rom.00bfa0a3, "ax"
	.incbin "baserom.gba", 0x00bfa0a3, 0x00000001
	.section .rom.00bfb877, "ax"
	.incbin "baserom.gba", 0x00bfb877, 0x00000001
	.section .rom.00bfcd3b, "ax"
	.incbin "baserom.gba", 0x00bfcd3b, 0x00000001
	.section .rom.00bff26e, "ax"
	.incbin "baserom.gba", 0x00bff26e, 0x00000002
	.section .rom.00bff3a2, "ax"
	.incbin "baserom.gba", 0x00bff3a2, 0x00000002
	.section .rom.00c01b0e, "ax"
	.incbin "baserom.gba", 0x00c01b0e, 0x00000002
	.section .rom.00c038e1, "ax"
	.incbin "baserom.gba", 0x00c038e1, 0x00000003
	.section .rom.00c071c5, "ax"
	.incbin "baserom.gba", 0x00c071c5, 0x00000003
	.section .rom.00c0992b, "ax"
	.incbin "baserom.gba", 0x00c0992b, 0x00000001
	.section .rom.00c0b295, "ax"
	.incbin "baserom.gba", 0x00c0b295, 0x00000003
	.section .rom.00c0dabe, "ax"
	.incbin "baserom.gba", 0x00c0dabe, 0x00000002
	.section .rom.00c11e9d, "ax"
	.incbin "baserom.gba", 0x00c11e9d, 0x00000003
	.section .rom.00c15247, "ax"
	.incbin "baserom.gba", 0x00c15247, 0x00000001
	.section .rom.00c1603a, "ax"
	.incbin "baserom.gba", 0x00c1603a, 0x00000002
	.section .rom.00c16db2, "ax"
	.incbin "baserom.gba", 0x00c16db2, 0x00000002
	.section .rom.00c1eeda, "ax"
	.incbin "baserom.gba", 0x00c1eeda, 0x00000002
	.section .rom.00c25259, "ax"
	.incbin "baserom.gba", 0x00c25259, 0x00000003
	.section .rom.00c25c5f, "ax"
	.incbin "baserom.gba", 0x00c25c5f, 0x00000001
	.section .rom.00c2770d, "ax"
	.incbin "baserom.gba", 0x00c2770d, 0x00000003
	.section .rom.00c28b9d, "ax"
	.incbin "baserom.gba", 0x00c28b9d, 0x00000003
	.section .rom.00c29007, "ax"
	.incbin "baserom.gba", 0x00c29007, 0x00000001
	.section .rom.00c291c6, "ax"
	.incbin "baserom.gba", 0x00c291c6, 0x00000002
	.section .rom.00c2b9ce, "ax"
	.incbin "baserom.gba", 0x00c2b9ce, 0x00000002
	.section .rom.00c2bb26, "ax"
	.incbin "baserom.gba", 0x00c2bb26, 0x00000002
	.section .rom.00c2e46d, "ax"
	.incbin "baserom.gba", 0x00c2e46d, 0x00000003
	.section .rom.00c30691, "ax"
	.incbin "baserom.gba", 0x00c30691, 0x00000003
	.section .rom.00c31b86, "ax"
	.incbin "baserom.gba", 0x00c31b86, 0x00000002
	.section .rom.00c33de9, "ax"
	.incbin "baserom.gba", 0x00c33de9, 0x00000003
	.section .rom.00c36697, "ax"
	.incbin "baserom.gba", 0x00c36697, 0x00000001
	.section .rom.00c3862a, "ax"
	.incbin "baserom.gba", 0x00c3862a, 0x00000002
	.section .rom.00c3964b, "ax"
	.incbin "baserom.gba", 0x00c3964b, 0x00000001
	.section .rom.00c3978b, "ax"
	.incbin "baserom.gba", 0x00c3978b, 0x00000001
	.section .rom.00c3c3aa, "ax"
	.incbin "baserom.gba", 0x00c3c3aa, 0x00000002
	.section .rom.00c3e82e, "ax"
	.incbin "baserom.gba", 0x00c3e82e, 0x00000002
	.section .rom.00c40915, "ax"
	.incbin "baserom.gba", 0x00c40915, 0x00000003
	.section .rom.00c42179, "ax"
	.incbin "baserom.gba", 0x00c42179, 0x00000003
	.section .rom.00c446df, "ax"
	.incbin "baserom.gba", 0x00c446df, 0x00000001
	.section .rom.00c44845, "ax"
	.incbin "baserom.gba", 0x00c44845, 0x00000003
	.section .rom.00c4703f, "ax"
	.incbin "baserom.gba", 0x00c4703f, 0x00000001
	.section .rom.00c49073, "ax"
	.incbin "baserom.gba", 0x00c49073, 0x00000001
	.section .rom.00c4ac9a, "ax"
	.incbin "baserom.gba", 0x00c4ac9a, 0x00000002
	.section .rom.00c4d6ba, "ax"
	.incbin "baserom.gba", 0x00c4d6ba, 0x00000002
	.section .rom.00c4e225, "ax"
	.incbin "baserom.gba", 0x00c4e225, 0x00000003
	.section .rom.00c4e30b, "ax"
	.incbin "baserom.gba", 0x00c4e30b, 0x00000001
	.section .rom.00c4fc55, "ax"
	.incbin "baserom.gba", 0x00c4fc55, 0x00000003
	.section .rom.00c515bb, "ax"
	.incbin "baserom.gba", 0x00c515bb, 0x00000001
	.section .rom.00c52989, "ax"
	.incbin "baserom.gba", 0x00c52989, 0x00000003
	.section .rom.00c52a6f, "ax"
	.incbin "baserom.gba", 0x00c52a6f, 0x00000001
	.section .rom.00c535dd, "ax"
	.incbin "baserom.gba", 0x00c535dd, 0x00000003
	.section .rom.00c536c3, "ax"
	.incbin "baserom.gba", 0x00c536c3, 0x00000001
	.section .rom.00c54427, "ax"
	.incbin "baserom.gba", 0x00c54427, 0x00000001
	.section .rom.00c5450b, "ax"
	.incbin "baserom.gba", 0x00c5450b, 0x00000001
	.section .rom.00c54d11, "ax"
	.incbin "baserom.gba", 0x00c54d11, 0x00000003
	.section .rom.00c54df7, "ax"
	.incbin "baserom.gba", 0x00c54df7, 0x00000001
	.section .rom.00c55957, "ax"
	.incbin "baserom.gba", 0x00c55957, 0x00000001
	.section .rom.00c561d5, "ax"
	.incbin "baserom.gba", 0x00c561d5, 0x00000003
	.section .rom.00c562d2, "ax"
	.incbin "baserom.gba", 0x00c562d2, 0x00000002
	.section .rom.00c5812b, "ax"
	.incbin "baserom.gba", 0x00c5812b, 0x00000001
	.section .rom.00c58f39, "ax"
	.incbin "baserom.gba", 0x00c58f39, 0x00000003
	.section .rom.00c5a1cd, "ax"
	.incbin "baserom.gba", 0x00c5a1cd, 0x00000003
	.section .rom.00c5b1a9, "ax"
	.incbin "baserom.gba", 0x00c5b1a9, 0x00000003
	.section .rom.00c5d5af, "ax"
	.incbin "baserom.gba", 0x00c5d5af, 0x00000001
	.section .rom.00c5d6ef, "ax"
	.incbin "baserom.gba", 0x00c5d6ef, 0x00000001
	.section .rom.00c5ddbd, "ax"
	.incbin "baserom.gba", 0x00c5ddbd, 0x00000003
	.section .rom.00c5de93, "ax"
	.incbin "baserom.gba", 0x00c5de93, 0x00000001
	.section .rom.00c5e709, "ax"
	.incbin "baserom.gba", 0x00c5e709, 0x00000003
	.section .rom.00c5ebba, "ax"
	.incbin "baserom.gba", 0x00c5ebba, 0x00000002
	.section .rom.00c60035, "ax"
	.incbin "baserom.gba", 0x00c60035, 0x00000003
	.section .rom.00c6084b, "ax"
	.incbin "baserom.gba", 0x00c6084b, 0x00000001
	.section .rom.00c614e1, "ax"
	.incbin "baserom.gba", 0x00c614e1, 0x00000003
	.section .rom.00c61686, "ax"
	.incbin "baserom.gba", 0x00c61686, 0x00000002
	.section .rom.00c6454a, "ax"
	.incbin "baserom.gba", 0x00c6454a, 0x00000002
	.section .rom.00c65ec9, "ax"
	.incbin "baserom.gba", 0x00c65ec9, 0x00000003
	.section .rom.00c66f16, "ax"
	.incbin "baserom.gba", 0x00c66f16, 0x00000002
	.section .rom.00c6e65b, "ax"
	.incbin "baserom.gba", 0x00c6e65b, 0x00000001
	.section .rom.00c75e4f, "ax"
	.incbin "baserom.gba", 0x00c75e4f, 0x00000001
	.section .rom.00c7c449, "ax"
	.incbin "baserom.gba", 0x00c7c449, 0x00000003
	.section .rom.00c7ee82, "ax"
	.incbin "baserom.gba", 0x00c7ee82, 0x00000002
	.section .rom.00c81753, "ax"
	.incbin "baserom.gba", 0x00c81753, 0x00000001
	.section .rom.00c85f9f, "ax"
	.incbin "baserom.gba", 0x00c85f9f, 0x00000001
	.section .rom.00c86f9d, "ax"
	.incbin "baserom.gba", 0x00c86f9d, 0x00000003
	.section .rom.00c870b6, "ax"
	.incbin "baserom.gba", 0x00c870b6, 0x00000002
	.section .rom.00c8865e, "ax"
	.incbin "baserom.gba", 0x00c8865e, 0x00000002
	.section .rom.00c8a0bb, "ax"
	.incbin "baserom.gba", 0x00c8a0bb, 0x00000001
	.section .rom.00c8a1f6, "ax"
	.incbin "baserom.gba", 0x00c8a1f6, 0x00000002
	.section .rom.00c8b6fd, "ax"
	.incbin "baserom.gba", 0x00c8b6fd, 0x00000003
	.section .rom.00c8e986, "ax"
	.incbin "baserom.gba", 0x00c8e986, 0x00000002
	.section .rom.00c8fdcd, "ax"
	.incbin "baserom.gba", 0x00c8fdcd, 0x00000003
	.section .rom.00c92ac5, "ax"
	.incbin "baserom.gba", 0x00c92ac5, 0x00000003
	.section .rom.00c94efb, "ax"
	.incbin "baserom.gba", 0x00c94efb, 0x00000001
	.section .rom.00c950d7, "ax"
	.incbin "baserom.gba", 0x00c950d7, 0x00000001
	.section .rom.00c981eb, "ax"
	.incbin "baserom.gba", 0x00c981eb, 0x00000001
	.section .rom.00c9966e, "ax"
	.incbin "baserom.gba", 0x00c9966e, 0x00000002
	.section .rom.00c9a6bd, "ax"
	.incbin "baserom.gba", 0x00c9a6bd, 0x00000003
	.section .rom.00c9c522, "ax"
	.incbin "baserom.gba", 0x00c9c522, 0x00000002
	.section .rom.00c9c6bd, "ax"
	.incbin "baserom.gba", 0x00c9c6bd, 0x00000003
	.section .rom.00c9c81b, "ax"
	.incbin "baserom.gba", 0x00c9c81b, 0x00000001
	.section .rom.00c9d79a, "ax"
	.incbin "baserom.gba", 0x00c9d79a, 0x00000002
	.section .rom.00c9d8b2, "ax"
	.incbin "baserom.gba", 0x00c9d8b2, 0x00000002
	.section .rom.00c9f945, "ax"
	.incbin "baserom.gba", 0x00c9f945, 0x00000003
	.section .rom.00ca17c7, "ax"
	.incbin "baserom.gba", 0x00ca17c7, 0x00000001
	.section .rom.00ca3ced, "ax"
	.incbin "baserom.gba", 0x00ca3ced, 0x00000003
	.section .rom.00ca3dfa, "ax"
	.incbin "baserom.gba", 0x00ca3dfa, 0x00000002
	.section .rom.00ca5e8d, "ax"
	.incbin "baserom.gba", 0x00ca5e8d, 0x00000003
	.section .rom.00ca9363, "ax"
	.incbin "baserom.gba", 0x00ca9363, 0x00000001
	.section .rom.00ca9e4d, "ax"
	.incbin "baserom.gba", 0x00ca9e4d, 0x00000003
	.section .rom.00ca9fbd, "ax"
	.incbin "baserom.gba", 0x00ca9fbd, 0x00000003
	.section .rom.00caced7, "ax"
	.incbin "baserom.gba", 0x00caced7, 0x00000001
	.section .rom.00caee62, "ax"
	.incbin "baserom.gba", 0x00caee62, 0x00000002
	.section .rom.00cb07cd, "ax"
	.incbin "baserom.gba", 0x00cb07cd, 0x00000003
	.section .rom.00cb62ba, "ax"
	.incbin "baserom.gba", 0x00cb62ba, 0x00000002
	.section .rom.00cb646a, "ax"
	.incbin "baserom.gba", 0x00cb646a, 0x00000002
	.section .rom.00cbade2, "ax"
	.incbin "baserom.gba", 0x00cbade2, 0x00000002
	.section .rom.00cbaf3a, "ax"
	.incbin "baserom.gba", 0x00cbaf3a, 0x00000002
	.section .rom.00cbe507, "ax"
	.incbin "baserom.gba", 0x00cbe507, 0x00000001
	.section .rom.00cbfd89, "ax"
	.incbin "baserom.gba", 0x00cbfd89, 0x00000003
	.section .rom.00cc1ab1, "ax"
	.incbin "baserom.gba", 0x00cc1ab1, 0x00000003
	.section .rom.00cc51d2, "ax"
	.incbin "baserom.gba", 0x00cc51d2, 0x00000002
	.section .rom.00cca35b, "ax"
	.incbin "baserom.gba", 0x00cca35b, 0x00000001
	.section .rom.00ccc929, "ax"
	.incbin "baserom.gba", 0x00ccc929, 0x00000003
	.section .rom.00cce2b5, "ax"
	.incbin "baserom.gba", 0x00cce2b5, 0x00000003
	.section .rom.00ccf3e2, "ax"
	.incbin "baserom.gba", 0x00ccf3e2, 0x00000002
	.section .rom.00ccff92, "ax"
	.incbin "baserom.gba", 0x00ccff92, 0x00000002
	.section .rom.00cd195a, "ax"
	.incbin "baserom.gba", 0x00cd195a, 0x00000002
	.section .rom.00cd1a3e, "ax"
	.incbin "baserom.gba", 0x00cd1a3e, 0x00000002
	.section .rom.00cd3e9b, "ax"
	.incbin "baserom.gba", 0x00cd3e9b, 0x00000001
	.section .rom.00cd3fdb, "ax"
	.incbin "baserom.gba", 0x00cd3fdb, 0x00000001
	.section .rom.00cd7651, "ax"
	.incbin "baserom.gba", 0x00cd7651, 0x00000003
	.section .rom.00cd77b5, "ax"
	.incbin "baserom.gba", 0x00cd77b5, 0x00000003
	.section .rom.00cd9e95, "ax"
	.incbin "baserom.gba", 0x00cd9e95, 0x00000003
	.section .rom.00cdcd45, "ax"
	.incbin "baserom.gba", 0x00cdcd45, 0x00000003
	.section .rom.00cde2ba, "ax"
	.incbin "baserom.gba", 0x00cde2ba, 0x00000002
	.section .rom.00ce23b6, "ax"
	.incbin "baserom.gba", 0x00ce23b6, 0x00000002
	.section .rom.00ce463b, "ax"
	.incbin "baserom.gba", 0x00ce463b, 0x00000001
	.section .rom.00ce621f, "ax"
	.incbin "baserom.gba", 0x00ce621f, 0x00000001
	.section .rom.00ce7f15, "ax"
	.incbin "baserom.gba", 0x00ce7f15, 0x00000003
	.section .rom.00cf5539, "ax"
	.incbin "baserom.gba", 0x00cf5539, 0x00000003
	.section .rom.00cf5661, "ax"
	.incbin "baserom.gba", 0x00cf5661, 0x00000003
	.section .rom.00cf7872, "ax"
	.incbin "baserom.gba", 0x00cf7872, 0x00000002
	.section .rom.00cf7a03, "ax"
	.incbin "baserom.gba", 0x00cf7a03, 0x00000001
	.section .rom.00cfeeb5, "ax"
	.incbin "baserom.gba", 0x00cfeeb5, 0x00000003
	.section .rom.00d0168b, "ax"
	.incbin "baserom.gba", 0x00d0168b, 0x00000001
	.section .rom.00d03de6, "ax"
	.incbin "baserom.gba", 0x00d03de6, 0x00000002
	.section .rom.00d03f99, "ax"
	.incbin "baserom.gba", 0x00d03f99, 0x00000003
	.section .rom.00d0569d, "ax"
	.incbin "baserom.gba", 0x00d0569d, 0x00000003
	.section .rom.00d076f7, "ax"
	.incbin "baserom.gba", 0x00d076f7, 0x00000001
	.section .rom.00d08a41, "ax"
	.incbin "baserom.gba", 0x00d08a41, 0x00000003
	.section .rom.00d0a48b, "ax"
	.incbin "baserom.gba", 0x00d0a48b, 0x00000001
	.section .rom.00d0a5af, "ax"
	.incbin "baserom.gba", 0x00d0a5af, 0x00000001
	.section .rom.00d0aa81, "ax"
	.incbin "baserom.gba", 0x00d0aa81, 0x00000003
	.section .rom.00d0cc65, "ax"
	.incbin "baserom.gba", 0x00d0cc65, 0x00000003
	.section .rom.00d0d139, "ax"
	.incbin "baserom.gba", 0x00d0d139, 0x00000003
	.section .rom.00d0f66f, "ax"
	.incbin "baserom.gba", 0x00d0f66f, 0x00000001
	.section .rom.00d0f7f7, "ax"
	.incbin "baserom.gba", 0x00d0f7f7, 0x00000001
	.section .rom.00d14099, "ax"
	.incbin "baserom.gba", 0x00d14099, 0x00000003
	.section .rom.00d141cf, "ax"
	.incbin "baserom.gba", 0x00d141cf, 0x00000001
	.section .rom.00d15e3b, "ax"
	.incbin "baserom.gba", 0x00d15e3b, 0x00000001
	.section .rom.00d16ff3, "ax"
	.incbin "baserom.gba", 0x00d16ff3, 0x00000001
	.section .rom.00d17d41, "ax"
	.incbin "baserom.gba", 0x00d17d41, 0x00000003
	.section .rom.00d18eca, "ax"
	.incbin "baserom.gba", 0x00d18eca, 0x00000002
	.section .rom.00d1a9c3, "ax"
	.incbin "baserom.gba", 0x00d1a9c3, 0x00000001
	.section .rom.00d1bdcf, "ax"
	.incbin "baserom.gba", 0x00d1bdcf, 0x00000001
	.section .rom.00d1c323, "ax"
	.incbin "baserom.gba", 0x00d1c323, 0x00000001
	.section .rom.00d1c95d, "ax"
	.incbin "baserom.gba", 0x00d1c95d, 0x00000003
	.section .rom.00d21cad, "ax"
	.incbin "baserom.gba", 0x00d21cad, 0x00000003
	.section .rom.00d2430e, "ax"
	.incbin "baserom.gba", 0x00d2430e, 0x00000002
	.section .rom.00d244a3, "ax"
	.incbin "baserom.gba", 0x00d244a3, 0x00000001
	.section .rom.00d26057, "ax"
	.incbin "baserom.gba", 0x00d26057, 0x00000001
	.section .rom.00d261b7, "ax"
	.incbin "baserom.gba", 0x00d261b7, 0x00000001
	.section .rom.00d28c5b, "ax"
	.incbin "baserom.gba", 0x00d28c5b, 0x00000001
	.section .rom.00d2ae09, "ax"
	.incbin "baserom.gba", 0x00d2ae09, 0x00000003
	.section .rom.00d2cc82, "ax"
	.incbin "baserom.gba", 0x00d2cc82, 0x00000002
	.section .rom.00d2cdc3, "ax"
	.incbin "baserom.gba", 0x00d2cdc3, 0x00000001
	.section .rom.00d33d3b, "ax"
	.incbin "baserom.gba", 0x00d33d3b, 0x00000001
	.section .rom.00d33e79, "ax"
	.incbin "baserom.gba", 0x00d33e79, 0x00000003
	.section .rom.00d35e4d, "ax"
	.incbin "baserom.gba", 0x00d35e4d, 0x00000003
	.section .rom.00d35f41, "ax"
	.incbin "baserom.gba", 0x00d35f41, 0x00000003
	.section .rom.00d37021, "ax"
	.incbin "baserom.gba", 0x00d37021, 0x00000003
	.section .rom.00d38efb, "ax"
	.incbin "baserom.gba", 0x00d38efb, 0x00000001
	.section .rom.00d39eb3, "ax"
	.incbin "baserom.gba", 0x00d39eb3, 0x00000001
	.section .rom.00d3bdfa, "ax"
	.incbin "baserom.gba", 0x00d3bdfa, 0x00000002
	.section .rom.00d3da05, "ax"
	.incbin "baserom.gba", 0x00d3da05, 0x00000003
	.section .rom.00d3db51, "ax"
	.incbin "baserom.gba", 0x00d3db51, 0x00000003
	.section .rom.00d3fc0f, "ax"
	.incbin "baserom.gba", 0x00d3fc0f, 0x00000001
	.section .rom.00d43cfe, "ax"
	.incbin "baserom.gba", 0x00d43cfe, 0x00000002
	.section .rom.00d499ba, "ax"
	.incbin "baserom.gba", 0x00d499ba, 0x00000002
	.section .rom.00d4bd1f, "ax"
	.incbin "baserom.gba", 0x00d4bd1f, 0x00000001
	.section .rom.00d50ca5, "ax"
	.incbin "baserom.gba", 0x00d50ca5, 0x00000003
	.section .rom.00d50dd2, "ax"
	.incbin "baserom.gba", 0x00d50dd2, 0x00000002
	.section .rom.00d57d45, "ax"
	.incbin "baserom.gba", 0x00d57d45, 0x00000003
	.section .rom.00d57e4a, "ax"
	.incbin "baserom.gba", 0x00d57e4a, 0x00000002
	.section .rom.00d57f8a, "ax"
	.incbin "baserom.gba", 0x00d57f8a, 0x00000002
	.section .rom.00d59956, "ax"
	.incbin "baserom.gba", 0x00d59956, 0x00000002
	.section .rom.00d59a4d, "ax"
	.incbin "baserom.gba", 0x00d59a4d, 0x00000003
	.section .rom.00d5c8a1, "ax"
	.incbin "baserom.gba", 0x00d5c8a1, 0x00000003
	.section .rom.00d5ca4a, "ax"
	.incbin "baserom.gba", 0x00d5ca4a, 0x00000002
	.section .rom.00d5f666, "ax"
	.incbin "baserom.gba", 0x00d5f666, 0x00000002
	.section .rom.00d62399, "ax"
	.incbin "baserom.gba", 0x00d62399, 0x00000003
	.section .rom.00d64083, "ax"
	.incbin "baserom.gba", 0x00d64083, 0x00000001
	.section .rom.00d678fd, "ax"
	.incbin "baserom.gba", 0x00d678fd, 0x00000003
	.section .rom.00d67a55, "ax"
	.incbin "baserom.gba", 0x00d67a55, 0x00000003
	.section .rom.00d69f99, "ax"
	.incbin "baserom.gba", 0x00d69f99, 0x00000003
	.section .rom.00d6de5e, "ax"
	.incbin "baserom.gba", 0x00d6de5e, 0x00000002
	.section .rom.00d6ff8e, "ax"
	.incbin "baserom.gba", 0x00d6ff8e, 0x00000002
	.section .rom.00d7419a, "ax"
	.incbin "baserom.gba", 0x00d7419a, 0x00000002
	.section .rom.00d7436e, "ax"
	.incbin "baserom.gba", 0x00d7436e, 0x00000002
	.section .rom.00d763ae, "ax"
	.incbin "baserom.gba", 0x00d763ae, 0x00000002
	.section .rom.00d777a5, "ax"
	.incbin "baserom.gba", 0x00d777a5, 0x00000003
	.section .rom.00d782e2, "ax"
	.incbin "baserom.gba", 0x00d782e2, 0x00000002
	.section .rom.00d7944d, "ax"
	.incbin "baserom.gba", 0x00d7944d, 0x00000003
	.section .rom.00d7a4f9, "ax"
	.incbin "baserom.gba", 0x00d7a4f9, 0x00000003
	.section .rom.00d7a6a5, "ax"
	.incbin "baserom.gba", 0x00d7a6a5, 0x00000003
	.section .rom.00d7c1b2, "ax"
	.incbin "baserom.gba", 0x00d7c1b2, 0x00000002
	.section .rom.00d7dbcf, "ax"
	.incbin "baserom.gba", 0x00d7dbcf, 0x00000001
	.section .rom.00d800a1, "ax"
	.incbin "baserom.gba", 0x00d800a1, 0x00000003
	.section .rom.00d81403, "ax"
	.incbin "baserom.gba", 0x00d81403, 0x00000001
	.section .rom.00d822b7, "ax"
	.incbin "baserom.gba", 0x00d822b7, 0x00000001
	.section .rom.00d838a5, "ax"
	.incbin "baserom.gba", 0x00d838a5, 0x00000003
	.section .rom.00d84deb, "ax"
	.incbin "baserom.gba", 0x00d84deb, 0x00000001
	.section .rom.00d85bb5, "ax"
	.incbin "baserom.gba", 0x00d85bb5, 0x00000003
	.section .rom.00d85d23, "ax"
	.incbin "baserom.gba", 0x00d85d23, 0x00000001
	.section .rom.00d86ce2, "ax"
	.incbin "baserom.gba", 0x00d86ce2, 0x00000002
	.section .rom.00d877ab, "ax"
	.incbin "baserom.gba", 0x00d877ab, 0x00000001
	.section .rom.00d88345, "ax"
	.incbin "baserom.gba", 0x00d88345, 0x00000003
	.section .rom.00d88495, "ax"
	.incbin "baserom.gba", 0x00d88495, 0x00000003
	.section .rom.00d8c743, "ax"
	.incbin "baserom.gba", 0x00d8c743, 0x00000001
	.section .rom.00d8e1dd, "ax"
	.incbin "baserom.gba", 0x00d8e1dd, 0x00000003
	.section .rom.00d8fbb3, "ax"
	.incbin "baserom.gba", 0x00d8fbb3, 0x00000001
	.section .rom.00d8fd32, "ax"
	.incbin "baserom.gba", 0x00d8fd32, 0x00000002
	.section .rom.00d93962, "ax"
	.incbin "baserom.gba", 0x00d93962, 0x00000002
	.section .rom.00d956bf, "ax"
	.incbin "baserom.gba", 0x00d956bf, 0x00000001
	.section .rom.00d98ce9, "ax"
	.incbin "baserom.gba", 0x00d98ce9, 0x00000003
	.section .rom.00d98e3a, "ax"
	.incbin "baserom.gba", 0x00d98e3a, 0x00000002
	.section .rom.00d9ea36, "ax"
	.incbin "baserom.gba", 0x00d9ea36, 0x00000002
	.section .rom.00da1373, "ax"
	.incbin "baserom.gba", 0x00da1373, 0x00000001
	.section .rom.00da2cb5, "ax"
	.incbin "baserom.gba", 0x00da2cb5, 0x00000003
	.section .rom.00da2e1e, "ax"
	.incbin "baserom.gba", 0x00da2e1e, 0x00000002
	.section .rom.00da9c1d, "ax"
	.incbin "baserom.gba", 0x00da9c1d, 0x00000003
	.section .rom.00daa1fb, "ax"
	.incbin "baserom.gba", 0x00daa1fb, 0x00000001
	.section .rom.00dab103, "ax"
	.incbin "baserom.gba", 0x00dab103, 0x00000001
	.section .rom.00dad94b, "ax"
	.incbin "baserom.gba", 0x00dad94b, 0x00000001
	.section .rom.00daef6a, "ax"
	.incbin "baserom.gba", 0x00daef6a, 0x00000002
	.section .rom.00db08cb, "ax"
	.incbin "baserom.gba", 0x00db08cb, 0x00000001
	.section .rom.00db36b3, "ax"
	.incbin "baserom.gba", 0x00db36b3, 0x00000001
	.section .rom.00db382e, "ax"
	.incbin "baserom.gba", 0x00db382e, 0x00000002
	.section .rom.00dc29f7, "ax"
	.incbin "baserom.gba", 0x00dc29f7, 0x00000001
	.section .rom.00dc2b5f, "ax"
	.incbin "baserom.gba", 0x00dc2b5f, 0x00000001
	.section .rom.00dc5182, "ax"
	.incbin "baserom.gba", 0x00dc5182, 0x00000002
	.section .rom.00dc52c5, "ax"
	.incbin "baserom.gba", 0x00dc52c5, 0x00000003
	.section .rom.00dc6b82, "ax"
	.incbin "baserom.gba", 0x00dc6b82, 0x00000002
	.section .rom.00dc9ae6, "ax"
	.incbin "baserom.gba", 0x00dc9ae6, 0x00000002
	.section .rom.00dcc47e, "ax"
	.incbin "baserom.gba", 0x00dcc47e, 0x00000002
	.section .rom.00dd0dd6, "ax"
	.incbin "baserom.gba", 0x00dd0dd6, 0x00000002
	.section .rom.00dd33ff, "ax"
	.incbin "baserom.gba", 0x00dd33ff, 0x00000001
	.section .rom.00dd35d3, "ax"
	.incbin "baserom.gba", 0x00dd35d3, 0x00000001
	.section .rom.00dd68f2, "ax"
	.incbin "baserom.gba", 0x00dd68f2, 0x00000002
	.section .rom.00dd6ac9, "ax"
	.incbin "baserom.gba", 0x00dd6ac9, 0x00000003
	.section .rom.00dd9e3d, "ax"
	.incbin "baserom.gba", 0x00dd9e3d, 0x00000003
	.section .rom.00ddb591, "ax"
	.incbin "baserom.gba", 0x00ddb591, 0x00000003
	.section .rom.00ddc51f, "ax"
	.incbin "baserom.gba", 0x00ddc51f, 0x00000001
	.section .rom.00ddde95, "ax"
	.incbin "baserom.gba", 0x00ddde95, 0x00000003
	.section .rom.00dddfcf, "ax"
	.incbin "baserom.gba", 0x00dddfcf, 0x00000001
	.section .rom.00ddfb11, "ax"
	.incbin "baserom.gba", 0x00ddfb11, 0x00000003
	.section .rom.00de30e9, "ax"
	.incbin "baserom.gba", 0x00de30e9, 0x00000003
	.section .rom.00de3d73, "ax"
	.incbin "baserom.gba", 0x00de3d73, 0x00000001
	.section .rom.00de3eb3, "ax"
	.incbin "baserom.gba", 0x00de3eb3, 0x00000001
	.section .rom.00de5c77, "ax"
	.incbin "baserom.gba", 0x00de5c77, 0x00000001
	.section .rom.00de5dea, "ax"
	.incbin "baserom.gba", 0x00de5dea, 0x00000002
	.section .rom.00de8252, "ax"
	.incbin "baserom.gba", 0x00de8252, 0x00000002
	.section .rom.00de9bee, "ax"
	.incbin "baserom.gba", 0x00de9bee, 0x00000002
	.section .rom.00deba23, "ax"
	.incbin "baserom.gba", 0x00deba23, 0x00000001
	.section .rom.00dec4da, "ax"
	.incbin "baserom.gba", 0x00dec4da, 0x00000002
	.section .rom.00dedef6, "ax"
	.incbin "baserom.gba", 0x00dedef6, 0x00000002
	.section .rom.00df0bc2, "ax"
	.incbin "baserom.gba", 0x00df0bc2, 0x00000002
	.section .rom.00df0d9d, "ax"
	.incbin "baserom.gba", 0x00df0d9d, 0x00000003
	.section .rom.00df28d3, "ax"
	.incbin "baserom.gba", 0x00df28d3, 0x00000001
	.section .rom.00df599d, "ax"
	.incbin "baserom.gba", 0x00df599d, 0x00000003
	.section .rom.00df6b59, "ax"
	.incbin "baserom.gba", 0x00df6b59, 0x00000003
	.section .rom.00df6d41, "ax"
	.incbin "baserom.gba", 0x00df6d41, 0x00000003
	.section .rom.00df6ed3, "ax"
	.incbin "baserom.gba", 0x00df6ed3, 0x00000001
	.section .rom.00df7c5a, "ax"
	.incbin "baserom.gba", 0x00df7c5a, 0x00000002
	.section .rom.00df7def, "ax"
	.incbin "baserom.gba", 0x00df7def, 0x00000001
	.section .rom.00df9ea3, "ax"
	.incbin "baserom.gba", 0x00df9ea3, 0x00000001
	.section .rom.00e01f26, "ax"
	.incbin "baserom.gba", 0x00e01f26, 0x00000002
	.section .rom.00e02067, "ax"
	.incbin "baserom.gba", 0x00e02067, 0x00000001
	.section .rom.00e063ef, "ax"
	.incbin "baserom.gba", 0x00e063ef, 0x00000001
	.section .rom.00e06b0e, "ax"
	.incbin "baserom.gba", 0x00e06b0e, 0x00000002
	.section .rom.00e07ecd, "ax"
	.incbin "baserom.gba", 0x00e07ecd, 0x00000003
	.section .rom.00e0bfe7, "ax"
	.incbin "baserom.gba", 0x00e0bfe7, 0x00000001
	.section .rom.00e0d067, "ax"
	.incbin "baserom.gba", 0x00e0d067, 0x00000001
	.section .rom.00e0e0e9, "ax"
	.incbin "baserom.gba", 0x00e0e0e9, 0x00000003
	.section .rom.00e0ec5a, "ax"
	.incbin "baserom.gba", 0x00e0ec5a, 0x00000002
	.section .rom.00e0fbab, "ax"
	.incbin "baserom.gba", 0x00e0fbab, 0x00000001
	.section .rom.00e0fd29, "ax"
	.incbin "baserom.gba", 0x00e0fd29, 0x00000003
	.section .rom.00e13283, "ax"
	.incbin "baserom.gba", 0x00e13283, 0x00000001
	.section .rom.00e148b6, "ax"
	.incbin "baserom.gba", 0x00e148b6, 0x00000002
	.section .rom.00e149f7, "ax"
	.incbin "baserom.gba", 0x00e149f7, 0x00000001
	.section .rom.00e17342, "ax"
	.incbin "baserom.gba", 0x00e17342, 0x00000002
	.section .rom.00e1fcea, "ax"
	.incbin "baserom.gba", 0x00e1fcea, 0x00000002
	.section .rom.00e22895, "ax"
	.incbin "baserom.gba", 0x00e22895, 0x00000003
	.section .rom.00e247d5, "ax"
	.incbin "baserom.gba", 0x00e247d5, 0x00000003
	.section .rom.00e25c49, "ax"
	.incbin "baserom.gba", 0x00e25c49, 0x00000003
	.section .rom.00e27903, "ax"
	.incbin "baserom.gba", 0x00e27903, 0x00000001
	.section .rom.00e349b2, "ax"
	.incbin "baserom.gba", 0x00e349b2, 0x00000002
	.section .rom.00e34af1, "ax"
	.incbin "baserom.gba", 0x00e34af1, 0x00000003
	.section .rom.00e36a16, "ax"
	.incbin "baserom.gba", 0x00e36a16, 0x00000002
	.section .rom.00e36b3f, "ax"
	.incbin "baserom.gba", 0x00e36b3f, 0x00000001
	.section .rom.00e38d0f, "ax"
	.incbin "baserom.gba", 0x00e38d0f, 0x00000001
	.section .rom.00e3b2df, "ax"
	.incbin "baserom.gba", 0x00e3b2df, 0x00000001
	.section .rom.00e3d5f1, "ax"
	.incbin "baserom.gba", 0x00e3d5f1, 0x00000003
	.section .rom.00e3de65, "ax"
	.incbin "baserom.gba", 0x00e3de65, 0x00000003
	.section .rom.00e3dfa7, "ax"
	.incbin "baserom.gba", 0x00e3dfa7, 0x00000001
	.section .rom.00e4105d, "ax"
	.incbin "baserom.gba", 0x00e4105d, 0x00000003
	.section .rom.00e42743, "ax"
	.incbin "baserom.gba", 0x00e42743, 0x00000001
	.section .rom.00e4430b, "ax"
	.incbin "baserom.gba", 0x00e4430b, 0x00000001
	.section .rom.00e45e86, "ax"
	.incbin "baserom.gba", 0x00e45e86, 0x00000002
	.section .rom.00e462d1, "ax"
	.incbin "baserom.gba", 0x00e462d1, 0x00000003
	.section .rom.00e48432, "ax"
	.incbin "baserom.gba", 0x00e48432, 0x00000002
	.section .rom.00e4854b, "ax"
	.incbin "baserom.gba", 0x00e4854b, 0x00000001
	.section .rom.00e49375, "ax"
	.incbin "baserom.gba", 0x00e49375, 0x00000003
	.section .rom.00e494d7, "ax"
	.incbin "baserom.gba", 0x00e494d7, 0x00000001
	.section .rom.00e4dd3f, "ax"
	.incbin "baserom.gba", 0x00e4dd3f, 0x00000001
	.section .rom.00e503fd, "ax"
	.incbin "baserom.gba", 0x00e503fd, 0x00000003
	.section .rom.00e509db, "ax"
	.incbin "baserom.gba", 0x00e509db, 0x00000001
	.section .rom.00e5296e, "ax"
	.incbin "baserom.gba", 0x00e5296e, 0x00000002
	.section .rom.00e5414e, "ax"
	.incbin "baserom.gba", 0x00e5414e, 0x00000002
	.section .rom.00e542b2, "ax"
	.incbin "baserom.gba", 0x00e542b2, 0x00000002
	.section .rom.00e56aaa, "ax"
	.incbin "baserom.gba", 0x00e56aaa, 0x00000002
	.section .rom.00e56c63, "ax"
	.incbin "baserom.gba", 0x00e56c63, 0x00000001
	.section .rom.00e56deb, "ax"
	.incbin "baserom.gba", 0x00e56deb, 0x00000001
	.section .rom.00e5860d, "ax"
	.incbin "baserom.gba", 0x00e5860d, 0x00000003
	.section .rom.00e591f6, "ax"
	.incbin "baserom.gba", 0x00e591f6, 0x00000002
	.section .rom.00e5a72f, "ax"
	.incbin "baserom.gba", 0x00e5a72f, 0x00000001
	.section .rom.00e5a8ca, "ax"
	.incbin "baserom.gba", 0x00e5a8ca, 0x00000002
	.section .rom.00e5cb76, "ax"
	.incbin "baserom.gba", 0x00e5cb76, 0x00000002
	.section .rom.00e5ed3d, "ax"
	.incbin "baserom.gba", 0x00e5ed3d, 0x00000003
	.section .rom.00e6043b, "ax"
	.incbin "baserom.gba", 0x00e6043b, 0x00000001
	.section .rom.00e60f5f, "ax"
	.incbin "baserom.gba", 0x00e60f5f, 0x00000001
	.section .rom.00e65e02, "ax"
	.incbin "baserom.gba", 0x00e65e02, 0x00000002
	.section .rom.00e681a9, "ax"
	.incbin "baserom.gba", 0x00e681a9, 0x00000003
	.section .rom.00e68892, "ax"
	.incbin "baserom.gba", 0x00e68892, 0x00000002
	.section .rom.00e6947e, "ax"
	.incbin "baserom.gba", 0x00e6947e, 0x00000002
	.section .rom.00e69f42, "ax"
	.incbin "baserom.gba", 0x00e69f42, 0x00000002
	.section .rom.00e6b4f3, "ax"
	.incbin "baserom.gba", 0x00e6b4f3, 0x00000001
	.section .rom.00e6b6a6, "ax"
	.incbin "baserom.gba", 0x00e6b6a6, 0x00000002
	.section .rom.00e6d8e5, "ax"
	.incbin "baserom.gba", 0x00e6d8e5, 0x00000003
	.section .rom.00e6e40b, "ax"
	.incbin "baserom.gba", 0x00e6e40b, 0x00000001
	.section .rom.00e79669, "ax"
	.incbin "baserom.gba", 0x00e79669, 0x00000003
	.section .rom.00e7c335, "ax"
	.incbin "baserom.gba", 0x00e7c335, 0x00000003
	.section .rom.00e7c4af, "ax"
	.incbin "baserom.gba", 0x00e7c4af, 0x00000001
	.section .rom.00e7c626, "ax"
	.incbin "baserom.gba", 0x00e7c626, 0x00000002
	.section .rom.00e7c7be, "ax"
	.incbin "baserom.gba", 0x00e7c7be, 0x00000002
	.section .rom.00e7c94f, "ax"
	.incbin "baserom.gba", 0x00e7c94f, 0x00000001
	.section .rom.00e7ebfb, "ax"
	.incbin "baserom.gba", 0x00e7ebfb, 0x00000001
	.section .rom.00e7ed13, "ax"
	.incbin "baserom.gba", 0x00e7ed13, 0x00000001
	.section .rom.00e80b15, "ax"
	.incbin "baserom.gba", 0x00e80b15, 0x00000003
	.section .rom.00e825da, "ax"
	.incbin "baserom.gba", 0x00e825da, 0x00000002
	.section .rom.00e82d02, "ax"
	.incbin "baserom.gba", 0x00e82d02, 0x00000002
	.section .rom.00e840fe, "ax"
	.incbin "baserom.gba", 0x00e840fe, 0x00000002
	.section .rom.00e84fd2, "ax"
	.incbin "baserom.gba", 0x00e84fd2, 0x00000002
	.section .rom.00e85116, "ax"
	.incbin "baserom.gba", 0x00e85116, 0x00000002
	.section .rom.00e874c7, "ax"
	.incbin "baserom.gba", 0x00e874c7, 0x00000001
	.section .rom.00e89cf5, "ax"
	.incbin "baserom.gba", 0x00e89cf5, 0x00000003
	.section .rom.00e8a0f2, "ax"
	.incbin "baserom.gba", 0x00e8a0f2, 0x00000002
	.section .rom.00e8b592, "ax"
	.incbin "baserom.gba", 0x00e8b592, 0x00000002
	.section .rom.00e8b6e6, "ax"
	.incbin "baserom.gba", 0x00e8b6e6, 0x00000002
	.section .rom.00e8f033, "ax"
	.incbin "baserom.gba", 0x00e8f033, 0x00000001
	.section .rom.00e9049e, "ax"
	.incbin "baserom.gba", 0x00e9049e, 0x00000002
	.section .rom.00e91977, "ax"
	.incbin "baserom.gba", 0x00e91977, 0x00000001
	.section .rom.00e91aca, "ax"
	.incbin "baserom.gba", 0x00e91aca, 0x00000002
	.section .rom.00e954d7, "ax"
	.incbin "baserom.gba", 0x00e954d7, 0x00000001
	.section .rom.00e966af, "ax"
	.incbin "baserom.gba", 0x00e966af, 0x00000001
	.section .rom.00e97eba, "ax"
	.incbin "baserom.gba", 0x00e97eba, 0x00000002
	.section .rom.00e9800a, "ax"
	.incbin "baserom.gba", 0x00e9800a, 0x00000002
	.section .rom.00e9a717, "ax"
	.incbin "baserom.gba", 0x00e9a717, 0x00000001
	.section .rom.00e9a87e, "ax"
	.incbin "baserom.gba", 0x00e9a87e, 0x00000002
	.section .rom.00e9d83f, "ax"
	.incbin "baserom.gba", 0x00e9d83f, 0x00000001
	.section .rom.00e9eaad, "ax"
	.incbin "baserom.gba", 0x00e9eaad, 0x00000003
	.section .rom.00ea163a, "ax"
	.incbin "baserom.gba", 0x00ea163a, 0x00000002
	.section .rom.00ea2021, "ax"
	.incbin "baserom.gba", 0x00ea2021, 0x00000003
	.section .rom.00ea29c2, "ax"
	.incbin "baserom.gba", 0x00ea29c2, 0x00000002
	.section .rom.00ea2b36, "ax"
	.incbin "baserom.gba", 0x00ea2b36, 0x00000002
	.section .rom.00ea4647, "ax"
	.incbin "baserom.gba", 0x00ea4647, 0x00000001
	.section .rom.00ea4769, "ax"
	.incbin "baserom.gba", 0x00ea4769, 0x00000003
	.section .rom.00ea7267, "ax"
	.incbin "baserom.gba", 0x00ea7267, 0x00000001
	.section .rom.00ea805e, "ax"
	.incbin "baserom.gba", 0x00ea805e, 0x00000002
	.section .rom.00ea9ffd, "ax"
	.incbin "baserom.gba", 0x00ea9ffd, 0x00000003
	.section .rom.00eab32d, "ax"
	.incbin "baserom.gba", 0x00eab32d, 0x00000003
	.section .rom.00eab481, "ax"
	.incbin "baserom.gba", 0x00eab481, 0x00000003
	.section .rom.00eac033, "ax"
	.incbin "baserom.gba", 0x00eac033, 0x00000001
	.section .rom.00ead80d, "ax"
	.incbin "baserom.gba", 0x00ead80d, 0x00000003
	.section .rom.00eae96d, "ax"
	.incbin "baserom.gba", 0x00eae96d, 0x00000003
	.section .rom.00eafa8b, "ax"
	.incbin "baserom.gba", 0x00eafa8b, 0x00000001
	.section .rom.00eafc92, "ax"
	.incbin "baserom.gba", 0x00eafc92, 0x00000002
	.section .rom.00eb0987, "ax"
	.incbin "baserom.gba", 0x00eb0987, 0x00000001
	.section .rom.00eb3129, "ax"
	.incbin "baserom.gba", 0x00eb3129, 0x00000003
	.section .rom.00eb489e, "ax"
	.incbin "baserom.gba", 0x00eb489e, 0x00000002
	.section .rom.00eb5b5b, "ax"
	.incbin "baserom.gba", 0x00eb5b5b, 0x00000001
	.section .rom.00eb639e, "ax"
	.incbin "baserom.gba", 0x00eb639e, 0x00000002
	.section .rom.00eb69a9, "ax"
	.incbin "baserom.gba", 0x00eb69a9, 0x00000003
	.section .rom.00eb6f23, "ax"
	.incbin "baserom.gba", 0x00eb6f23, 0x00000001
	.section .rom.00eb7063, "ax"
	.incbin "baserom.gba", 0x00eb7063, 0x00000001
	.section .rom.00eb71a3, "ax"
	.incbin "baserom.gba", 0x00eb71a3, 0x00000001
	.section .rom.00eb7d05, "ax"
	.incbin "baserom.gba", 0x00eb7d05, 0x00000003
	.section .rom.00eba4a9, "ax"
	.incbin "baserom.gba", 0x00eba4a9, 0x00000003
	.section .rom.00ebbc1e, "ax"
	.incbin "baserom.gba", 0x00ebbc1e, 0x00000002
	.section .rom.00ebcedb, "ax"
	.incbin "baserom.gba", 0x00ebcedb, 0x00000001
	.section .rom.00ebd71e, "ax"
	.incbin "baserom.gba", 0x00ebd71e, 0x00000002
	.section .rom.00ebdcbd, "ax"
	.incbin "baserom.gba", 0x00ebdcbd, 0x00000003
	.section .rom.00ebdccb, "ax"
	.incbin "baserom.gba", 0x00ebdccb, 0x00000001
	.section .rom.00ebde1a, "ax"
	.incbin "baserom.gba", 0x00ebde1a, 0x00000002
	.section .rom.00ebdf5b, "ax"
	.incbin "baserom.gba", 0x00ebdf5b, 0x00000001
	.section .rom.00ebe09b, "ax"
	.incbin "baserom.gba", 0x00ebe09b, 0x00000001
	.section .rom.00ebe1db, "ax"
	.incbin "baserom.gba", 0x00ebe1db, 0x00000001
	.section .rom.00ebe8f1, "ax"
	.incbin "baserom.gba", 0x00ebe8f1, 0x00000003
	.section .rom.00ebe8ff, "ax"
	.incbin "baserom.gba", 0x00ebe8ff, 0x00000001
	.section .rom.00ebea4e, "ax"
	.incbin "baserom.gba", 0x00ebea4e, 0x00000002
	.section .rom.00ebeb8f, "ax"
	.incbin "baserom.gba", 0x00ebeb8f, 0x00000001
	.section .rom.00ebeccf, "ax"
	.incbin "baserom.gba", 0x00ebeccf, 0x00000001
	.section .rom.00ebee0f, "ax"
	.incbin "baserom.gba", 0x00ebee0f, 0x00000001
	.section .rom.00ebf9d1, "ax"
	.incbin "baserom.gba", 0x00ebf9d1, 0x00000003
	.section .rom.00ebf9df, "ax"
	.incbin "baserom.gba", 0x00ebf9df, 0x00000001
	.section .rom.00ebfb2e, "ax"
	.incbin "baserom.gba", 0x00ebfb2e, 0x00000002
	.section .rom.00ebfc6f, "ax"
	.incbin "baserom.gba", 0x00ebfc6f, 0x00000001
	.section .rom.00ebfdaf, "ax"
	.incbin "baserom.gba", 0x00ebfdaf, 0x00000001
	.section .rom.00ebfeef, "ax"
	.incbin "baserom.gba", 0x00ebfeef, 0x00000001
	.section .rom.00ec04bd, "ax"
	.incbin "baserom.gba", 0x00ec04bd, 0x00000003
	.section .rom.00ec04cb, "ax"
	.incbin "baserom.gba", 0x00ec04cb, 0x00000001
	.section .rom.00ec061a, "ax"
	.incbin "baserom.gba", 0x00ec061a, 0x00000002
	.section .rom.00ec075b, "ax"
	.incbin "baserom.gba", 0x00ec075b, 0x00000001
	.section .rom.00ec089b, "ax"
	.incbin "baserom.gba", 0x00ec089b, 0x00000001
	.section .rom.00ec09db, "ax"
	.incbin "baserom.gba", 0x00ec09db, 0x00000001
	.section .rom.00ec1359, "ax"
	.incbin "baserom.gba", 0x00ec1359, 0x00000003
	.section .rom.00ec1367, "ax"
	.incbin "baserom.gba", 0x00ec1367, 0x00000001
	.section .rom.00ec14b6, "ax"
	.incbin "baserom.gba", 0x00ec14b6, 0x00000002
	.section .rom.00ec15f7, "ax"
	.incbin "baserom.gba", 0x00ec15f7, 0x00000001
	.section .rom.00ec1737, "ax"
	.incbin "baserom.gba", 0x00ec1737, 0x00000001
	.section .rom.00ec1877, "ax"
	.incbin "baserom.gba", 0x00ec1877, 0x00000001
	.section .rom.00ec1ca1, "ax"
	.incbin "baserom.gba", 0x00ec1ca1, 0x00000003
	.section .rom.00ec1caf, "ax"
	.incbin "baserom.gba", 0x00ec1caf, 0x00000001
	.section .rom.00ec1dfe, "ax"
	.incbin "baserom.gba", 0x00ec1dfe, 0x00000002
	.section .rom.00ec1f3f, "ax"
	.incbin "baserom.gba", 0x00ec1f3f, 0x00000001
	.section .rom.00ec207f, "ax"
	.incbin "baserom.gba", 0x00ec207f, 0x00000001
	.section .rom.00ec21bf, "ax"
	.incbin "baserom.gba", 0x00ec21bf, 0x00000001
	.section .rom.00ec27e9, "ax"
	.incbin "baserom.gba", 0x00ec27e9, 0x00000003
	.section .rom.00ec27f7, "ax"
	.incbin "baserom.gba", 0x00ec27f7, 0x00000001
	.section .rom.00ec2946, "ax"
	.incbin "baserom.gba", 0x00ec2946, 0x00000002
	.section .rom.00ec2a87, "ax"
	.incbin "baserom.gba", 0x00ec2a87, 0x00000001
	.section .rom.00ec2bc7, "ax"
	.incbin "baserom.gba", 0x00ec2bc7, 0x00000001
	.section .rom.00ec2d07, "ax"
	.incbin "baserom.gba", 0x00ec2d07, 0x00000001
	.section .rom.00ec3431, "ax"
	.incbin "baserom.gba", 0x00ec3431, 0x00000003
	.section .rom.00ec343f, "ax"
	.incbin "baserom.gba", 0x00ec343f, 0x00000001
	.section .rom.00ec358e, "ax"
	.incbin "baserom.gba", 0x00ec358e, 0x00000002
	.section .rom.00ec36cf, "ax"
	.incbin "baserom.gba", 0x00ec36cf, 0x00000001
	.section .rom.00ec380f, "ax"
	.incbin "baserom.gba", 0x00ec380f, 0x00000001
	.section .rom.00ec394f, "ax"
	.incbin "baserom.gba", 0x00ec394f, 0x00000001
