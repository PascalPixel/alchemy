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
	.incbin "baserom.gba", 0x0001319c, 0x00000108
	.section .rom.000133c4, "ax"
	.balign 4
	.global Runtime_InstallIwramAndIrqs
	.type Runtime_InstallIwramAndIrqs, %function
	.thumb_func
Runtime_InstallIwramAndIrqs:
	.incbin "baserom.gba", 0x000133c4, 0x00000074
	.section .rom.000134b0, "ax"
	.global System_WaitForFrameInterrupt
	.type System_WaitForFrameInterrupt, %function
	.thumb_func
System_WaitForFrameInterrupt:
	.incbin "baserom.gba", 0x000134b0, 0x000000b0
	.section .rom.000138b4, "ax"
	.global Input_UpdateKeyRepeatAndDirection
	.type Input_UpdateKeyRepeatAndDirection, %function
	.thumb_func
Input_UpdateKeyRepeatAndDirection:
	.incbin "baserom.gba", 0x000138b4, 0x0000027c
	.section .rom.00013d84, "ax"
	.incbin "baserom.gba", 0x00013d84, 0x000000ec
	.section .rom.0001401c, "ax"
	.global AffineMatrix_BuildForEffect
	.type AffineMatrix_BuildForEffect, %function
	.thumb_func
AffineMatrix_BuildForEffect:
	.incbin "baserom.gba", 0x0001401c, 0x000000bc
	.section .rom.00014174, "ax"
	.global ResourceTable_AllocateBlocks
	.type ResourceTable_AllocateBlocks, %function
	.thumb_func
ResourceTable_AllocateBlocks:
	.incbin "baserom.gba", 0x00014174, 0x000000ac
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
	.section .rom.0001440c, "ax"
	.incbin "baserom.gba", 0x0001440c, 0x000000b4
	.section .rom.00014b70, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b70, 0x0000003c
	.section .rom.00015024, "ax"
	.global SceneTransform_ApplyPitch
	.type SceneTransform_ApplyPitch, %function
	.thumb_func
SceneTransform_ApplyPitch:
	.incbin "baserom.gba", 0x00015024, 0x00000044
	.section .rom.00015068, "ax"
	.global SceneTransform_ApplyYaw
	.type SceneTransform_ApplyYaw, %function
	.thumb_func
SceneTransform_ApplyYaw:
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
	.incbin "baserom.gba", 0x00015128, 0x00000070
	.section .rom.000151ac, "ax"
	.global Func_080151ac
	.type Func_080151ac, %function
	.thumb_func
Func_080151ac:
	.incbin "baserom.gba", 0x000151ac, 0x000000a0
	.section .rom.00015510, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00015510, 0x000001d8
	.section .rom.00015778, "ax"
	.global Render_ProjectPoint
	.type Render_ProjectPoint, %function
	.thumb_func
Render_ProjectPoint:
	.incbin "baserom.gba", 0x00015778, 0x000000b8
	.section .rom.0001596c, "ax"
	.global Func_0801596c
	.type Func_0801596c, %function
	.thumb_func
Func_0801596c:
	.incbin "baserom.gba", 0x0001596c, 0x000001b8
	.section .rom.00015ca4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00015ca4, 0x000001a0
	.section .rom.00016054, "ax"
	.balign 4
	.global Func_08016054
	.type Func_08016054, %function
	.thumb_func
Func_08016054:
	.incbin "baserom.gba", 0x00016054, 0x00000080
	.section .rom.00016180, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x00016180, 0x0000015c
	.section .rom.00016346, "ax"
	.incbin "baserom.gba", 0x00016346, 0x00000002
	.section .rom.000164e6, "ax"
	.incbin "baserom.gba", 0x000164e6, 0x00000002
	.section .rom.000165b4, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x000165b4, 0x000000e0
	.section .rom.00016694, "ax"
	.global SerialRuntime_HandleTransferInterrupt
	.type SerialRuntime_HandleTransferInterrupt, %function
	.thumb_func
SerialRuntime_HandleTransferInterrupt:
	.incbin "baserom.gba", 0x00016694, 0x00000118
	.section .rom.00016990, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x00016990, 0x00000248
	.section .rom.00016bdc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00016bdc, 0x000000c8
	.section .rom.00016dd0, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dd0, 0x00000028
	.section .rom.00016e02, "ax"
	.incbin "baserom.gba", 0x00016e02, 0x00000002
	.section .rom.00017ce4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00017ce4, 0x00000004
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
	.global Func_08026fc8
	.type Func_08026fc8, %function
	.thumb_func
Func_08026fc8:
	.incbin "baserom.gba", 0x00026fc8, 0x00000dc0
	.section .rom.00027d9c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00027d9c, 0x000019f0
	.section .rom.000297b4, "ax"
	.incbin "baserom.gba", 0x000297b4, 0x00000cf4
	.section .rom.0002a560, "ax"
	.global Func_0802a5e4
	.type Func_0802a5e4, %function
	.thumb_func
Func_0802a5e4:
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
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x0002b11c, 0x00000134
	.section .rom.0002b308, "ax"
	.global Map_CopyCellAttributeRect
	.type Map_CopyCellAttributeRect, %function
	.thumb_func
Map_CopyCellAttributeRect:
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
	.section .rom.0002b7f4, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x0002b7f4, 0x00000120
	.section .rom.0002b914, "ax"
	.global Func_0802b998
	.type Func_0802b998, %function
	.thumb_func
Func_0802b998:
	.incbin "baserom.gba", 0x0002b914, 0x00000370
	.section .rom.0002bd4c, "ax"
	.balign 4
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
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x0002c454, 0x0000038c
	.section .rom.0002c944, "ax"
	.global Func_0802c9c8
	.type Func_0802c9c8, %function
	.thumb_func
Func_0802c9c8:
	.incbin "baserom.gba", 0x0002c944, 0x00000140
	.section .rom.0002cae0, "ax"
	.global MapAnimation_Update
	.type MapAnimation_Update, %function
	.thumb_func
MapAnimation_Update:
	.incbin "baserom.gba", 0x0002cae0, 0x00000110
	.section .rom.0002cc18, "ax"
	.incbin "baserom.gba", 0x0002cc18, 0x000000b0
	.section .rom.0002d004, "ax"
	.incbin "baserom.gba", 0x0002d004, 0x000001b4
	.section .rom.0002d2ea, "ax"
	.incbin "baserom.gba", 0x0002d2ea, 0x00000002
	.section .rom.0002d3d8, "ax"
	.global Func_0802d45c
	.type Func_0802d45c, %function
	.thumb_func
Func_0802d45c:
	.incbin "baserom.gba", 0x0002d3d8, 0x0000007c
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
	.section .rom.0002da04, "ax"
	.global Func_0802da88
	.type Func_0802da88, %function
	.thumb_func
Func_0802da88:
	.incbin "baserom.gba", 0x0002da04, 0x00000038
	.section .rom.0002dcec, "ax"
	.incbin "baserom.gba", 0x0002dcec, 0x0000011c
	.section .rom.0002de08, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de08, 0x00000828
	.section .rom.0002e898, "ax"
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x0002e898, 0x00000230
	.section .rom.0002eac8, "ax"
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0002eac8, 0x0000004c
	.section .rom.0002ebc4, "ax"
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x0002ebc4, 0x00000058
	.section .rom.0002ec5c, "ax"
	.incbin "baserom.gba", 0x0002ec5c, 0x000000e4
	.section .rom.0002ef40, "ax"
	.global Map_TerrainHeightHandlers
Map_TerrainHeightHandlers:
	.incbin "baserom.gba", 0x0002ef40, 0x00000040
	.section .rom.0002ef80, "ax"
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x0002ef80, 0x0000008c
	.section .rom.0002f00c, "ax"
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x0002f00c, 0x00000140
	.section .rom.0002f14c, "ax"
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f14c, 0x00000018
	.section .rom.0002f164, "ax"
	.global Data_0802f1e8
Data_0802f1e8:
	.incbin "baserom.gba", 0x0002f164, 0x00000018
	.section .rom.0002f17c, "ax"
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f17c, 0x000000dc
	.section .rom.0002f258, "ax"
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x0002f258, 0x000000a4
	.section .rom.0003028c, "ax"
	.incbin "baserom.gba", 0x0003028c, 0x00000010
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x00000310
	.section .rom.000391b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000391b8, 0x0000009c
	.section .rom.00039254, "ax"
	.global UiWindow_Create
	.type UiWindow_Create, %function
	.thumb_func
UiWindow_Create:
	.incbin "baserom.gba", 0x00039254, 0x00000114
	.section .rom.0003938e, "ax"
	.incbin "baserom.gba", 0x0003938e, 0x00000002
	.section .rom.00039446, "ax"
	.incbin "baserom.gba", 0x00039446, 0x00000002
	.section .rom.00039448, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039448, 0x00000094
	.section .rom.00039540, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00039540, 0x00000154
	.section .rom.0003971e, "ax"
	.incbin "baserom.gba", 0x0003971e, 0x0000002a
	.section .rom.00039750, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00039750, 0x00000084
	.section .rom.000397d4, "ax"
	.global UiWork_ProcessRenderChannels
	.type UiWork_ProcessRenderChannels, %function
	.thumb_func
UiWork_ProcessRenderChannels:
	.incbin "baserom.gba", 0x000397d4, 0x00000690
	.section .rom.00039e64, "ax"
	.global UiWork_ProcessDirectWork
	.type UiWork_ProcessDirectWork, %function
	.thumb_func
UiWork_ProcessDirectWork:
	.incbin "baserom.gba", 0x00039e64, 0x00000184
	.section .rom.0003a244, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003a244, 0x00000108
	.section .rom.0003a3da, "ax"
	.incbin "baserom.gba", 0x0003a3da, 0x00000002
	.section .rom.0003a4e0, "ax"
	.global UiMessage_Show
	.type UiMessage_Show, %function
	.thumb_func
UiMessage_Show:
	.incbin "baserom.gba", 0x0003a4e0, 0x00000094
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
	.section .rom.0003a890, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a890, 0x000001b4
	.section .rom.0003aa84, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa84, 0x000001b8
	.section .rom.0003ac3c, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x0003ac3c, 0x000001fc
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
	.global UiText_GetResourceDimensionsAlt
	.type UiText_GetResourceDimensionsAlt, %function
	.thumb_func
UiText_GetResourceDimensionsAlt:
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
	.section .rom.0003c204, "ax"
	.balign 4
	.global Func_0803c198
	.type Func_0803c198, %function
	.thumb_func
Func_0803c198:
	.incbin "baserom.gba", 0x0003c204, 0x0000022c
	.section .rom.0003c4c4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c4c4, 0x0000013c
	.section .rom.0003c600, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c600, 0x00000464
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
	.incbin "baserom.gba", 0x0003cc5e, 0x00000002
	.section .rom.0003cdc0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cdc0, 0x00000114
	.section .rom.0003cf1c, "ax"
	.balign 4
	.global Func_0803ce64
	.type Func_0803ce64, %function
	.thumb_func
Func_0803ce64:
	.incbin "baserom.gba", 0x0003cf1c, 0x000000fc
	.section .rom.0003d024, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003d024, 0x00000124
	.section .rom.0003d148, "ax"
	.global Func_0803d020
	.type Func_0803d020, %function
	.thumb_func
Func_0803d020:
	.incbin "baserom.gba", 0x0003d148, 0x00000164
	.section .rom.0003d2f8, "ax"
	.incbin "baserom.gba", 0x0003d2f8, 0x0000010c
	.section .rom.0003d404, "ax"
	.global UiWork_SetTwoEntriesTo999
	.type UiWork_SetTwoEntriesTo999, %function
	.thumb_func
UiWork_SetTwoEntriesTo999:
	.incbin "baserom.gba", 0x0003d404, 0x00000020
	.section .rom.0003d618, "ax"
	.global Ui_BuildPairedPatternsToSlot
	.type Ui_BuildPairedPatternsToSlot, %function
	.thumb_func
Ui_BuildPairedPatternsToSlot:
	.incbin "baserom.gba", 0x0003d618, 0x000000b0
	.section .rom.0003d6f8, "ax"
	.balign 4
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
	.section .rom.0003daee, "ax"
	.incbin "baserom.gba", 0x0003daee, 0x00000002
	.section .rom.0003daf0, "ax"
	.global UiIcon_BuildAbilityIconTiles
	.type UiIcon_BuildAbilityIconTiles, %function
	.thumb_func
UiIcon_BuildAbilityIconTiles:
	.incbin "baserom.gba", 0x0003daf0, 0x000000bc
	.section .rom.0003dd50, "ax"
	.balign 4
	.global Func_0803dc1c
	.type Func_0803dc1c, %function
	.thumb_func
Func_0803dc1c:
	.incbin "baserom.gba", 0x0003dd50, 0x00000108
	.section .rom.0003decc, "ax"
	.balign 4
	.global Func_0803dd98
	.type Func_0803dd98, %function
	.thumb_func
Func_0803dd98:
	.incbin "baserom.gba", 0x0003decc, 0x00000110
	.section .rom.0003e058, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003e058, 0x00000684
	.section .rom.0003e8fc, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e8fc, 0x00000124
	.section .rom.0003eb88, "ax"
	.incbin "baserom.gba", 0x0003eb88, 0x00000188
	.section .rom.0003ed10, "ax"
	.global Menu_StepRight
	.type Menu_StepRight, %function
	.thumb_func
Menu_StepRight:
	.incbin "baserom.gba", 0x0003ed10, 0x000001bc
	.section .rom.0003eecc, "ax"
	.global Menu_StepLeft
	.type Menu_StepLeft, %function
	.thumb_func
Menu_StepLeft:
	.incbin "baserom.gba", 0x0003eecc, 0x000001b0
	.section .rom.0003f138, "ax"
	.incbin "baserom.gba", 0x0003f138, 0x000001d0
	.section .rom.0003f418, "ax"
	.incbin "baserom.gba", 0x0003f418, 0x000000e4
	.section .rom.0003f4fc, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f4fc, 0x00000248
	.section .rom.0003f758, "ax"
	.balign 4
	.global UiTextResource_Initialize
	.type UiTextResource_Initialize, %function
	.thumb_func
UiTextResource_Initialize:
	.incbin "baserom.gba", 0x0003f758, 0x00000074
	.section .rom.0003f8ac, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0003f8ac, 0x00000088
	.section .rom.0003f934, "ax"
	.global UiWindow_OpenMode1AndWaitFrame
	.type UiWindow_OpenMode1AndWaitFrame, %function
	.thumb_func
UiWindow_OpenMode1AndWaitFrame:
	.incbin "baserom.gba", 0x0003f934, 0x00000010
	.section .rom.0003faf4, "ax"
	.global Func_0803f9c0
	.type Func_0803f9c0, %function
	.thumb_func
Func_0803f9c0:
	.incbin "baserom.gba", 0x0003faf4, 0x000001a0
	.section .rom.0003fd6c, "ax"
	.incbin "baserom.gba", 0x0003fd6c, 0x000000dc
	.section .rom.0003fe5c, "ax"
	.incbin "baserom.gba", 0x0003fe5c, 0x000003b0
	.section .rom.0004020c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004020c, 0x000004c4
	.section .rom.0004074c, "ax"
	.incbin "baserom.gba", 0x0004074c, 0x00000364
	.section .rom.00040b14, "ax"
	.incbin "baserom.gba", 0x00040b14, 0x00000270
	.section .rom.00040e94, "ax"
	.incbin "baserom.gba", 0x00040e94, 0x000000f4
	.section .rom.0004118c, "ax"
	.incbin "baserom.gba", 0x0004118c, 0x0000019c
	.section .rom.000413b0, "ax"
	.incbin "baserom.gba", 0x000413b0, 0x00000440
	.section .rom.000417f0, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x000417f0, 0x00000410
	.section .rom.00041c00, "ax"
	.global UiWindow_ClearTileAttributesInRect
	.type UiWindow_ClearTileAttributesInRect, %function
	.thumb_func
UiWindow_ClearTileAttributesInRect:
	.incbin "baserom.gba", 0x00041c00, 0x000000ac
	.section .rom.00041cac, "ax"
	.global Func_08041b68
	.type Func_08041b68, %function
	.thumb_func
Func_08041b68:
	.incbin "baserom.gba", 0x00041cac, 0x000000a4
	.section .rom.00041d98, "ax"
	.balign 4
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
	.section .rom.000420e8, "ax"
	.balign 4
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
	.section .rom.00042240, "ax"
	.global UiText_DrawStringAtOffset
	.type UiText_DrawStringAtOffset, %function
	.thumb_func
UiText_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042240, 0x0000008c
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
	.global CreateSideObject
	.type CreateSideObject, %function
	.thumb_func
CreateSideObject:
	.incbin "baserom.gba", 0x00042594, 0x000000b8
	.section .rom.0004264c, "ax"
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x0004264c, 0x00000064
	.section .rom.000426be, "ax"
	.incbin "baserom.gba", 0x000426be, 0x00000002
	.section .rom.000426cc, "ax"
	.balign 4
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x000426cc, 0x00000074
	.section .rom.00042824, "ax"
	.incbin "baserom.gba", 0x00042824, 0x0000029c
	.section .rom.00042ac0, "ax"
	.global PtyStDraw
	.type PtyStDraw, %function
	.thumb_func
PtyStDraw:
	.incbin "baserom.gba", 0x00042ac0, 0x00000430
	.section .rom.00042f08, "ax"
	.incbin "baserom.gba", 0x00042f08, 0x00000064
	.section .rom.00043054, "ax"
	.global Func_08042f10
	.type Func_08042f10, %function
	.thumb_func
Func_08042f10:
	.incbin "baserom.gba", 0x00043054, 0x000000e0
	.section .rom.00043134, "ax"
	.global Func_08042ff0
	.type Func_08042ff0, %function
	.thumb_func
Func_08042ff0:
	.incbin "baserom.gba", 0x00043134, 0x000000d4
	.section .rom.00043208, "ax"
	.global Func_080430c4
	.type Func_080430c4, %function
	.thumb_func
Func_080430c4:
	.incbin "baserom.gba", 0x00043208, 0x000000ec
	.section .rom.000432f4, "ax"
	.global Text_FormatPlayTime
	.type Text_FormatPlayTime, %function
	.thumb_func
Text_FormatPlayTime:
	.incbin "baserom.gba", 0x000432f4, 0x00000080
	.section .rom.000433e8, "ax"
	.balign 4
	.global Func_080432a4
	.type Func_080432a4, %function
	.thumb_func
Func_080432a4:
	.incbin "baserom.gba", 0x000433e8, 0x000000b4
	.section .rom.0004349c, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x0004349c, 0x00000194
	.section .rom.00043678, "ax"
	.global Func_08043534
	.type Func_08043534, %function
	.thumb_func
Func_08043534:
	.incbin "baserom.gba", 0x00043678, 0x000000c8
	.section .rom.000437d4, "ax"
	.global Func_08043690
	.type Func_08043690, %function
	.thumb_func
Func_08043690:
	.incbin "baserom.gba", 0x000437d4, 0x000000e4
	.section .rom.00043ba8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ba8, 0x000000d0
	.section .rom.00043cb4, "ax"
	.global Func_08043b70
	.type Func_08043b70, %function
	.thumb_func
Func_08043b70:
	.incbin "baserom.gba", 0x00043cb4, 0x0000007c
	.section .rom.00043e1c, "ax"
	.global Func_08043cd8
	.type Func_08043cd8, %function
	.thumb_func
Func_08043cd8:
	.incbin "baserom.gba", 0x00043e1c, 0x00000764
	.section .rom.00044640, "ax"
	.global Func_08044558
	.type Func_08044558, %function
	.thumb_func
Func_08044558:
	.incbin "baserom.gba", 0x00044640, 0x00000610
	.section .rom.00044c54, "ax"
	.global Func_08044a58
	.type Func_08044a58, %function
	.thumb_func
Func_08044a58:
	.incbin "baserom.gba", 0x00044c54, 0x00000128
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
	.section .rom.00045214, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00045214, 0x00000030
	.section .rom.000452f8, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x000452f8, 0x000000a0
	.section .rom.000453b8, "ax"
	.incbin "baserom.gba", 0x000453b8, 0x00000090
	.section .rom.000454b6, "ax"
	.incbin "baserom.gba", 0x000454b6, 0x00000002
	.section .rom.000454b8, "ax"
	.global Graphics_MergeShiftedTileRows
	.type Graphics_MergeShiftedTileRows, %function
	.thumb_func
Graphics_MergeShiftedTileRows:
	.incbin "baserom.gba", 0x000454b8, 0x00000074
	.section .rom.00045722, "ax"
	.incbin "baserom.gba", 0x00045722, 0x00000002
	.section .rom.00045760, "ax"
	.incbin "baserom.gba", 0x00045760, 0x0000004c
	.section .rom.000458f4, "ax"
	.incbin "baserom.gba", 0x000458f4, 0x00000070
	.section .rom.0004597a, "ax"
	.incbin "baserom.gba", 0x0004597a, 0x00000002
	.section .rom.000459ca, "ax"
	.incbin "baserom.gba", 0x000459ca, 0x0000096a
	.section .rom.00046484, "ax"
	.incbin "baserom.gba", 0x00046484, 0x00000150
	.section .rom.00046612, "ax"
	.incbin "baserom.gba", 0x00046612, 0x00003592
	.section .rom.00049bf2, "ax"
	.incbin "baserom.gba", 0x00049bf2, 0x00000002
	.section .rom.00049c20, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00049c20, 0x00001c70
	.section .rom.0004b9b0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004b9b0, 0x000000bc
	.section .rom.0004baa6, "ax"
	.incbin "baserom.gba", 0x0004baa6, 0x000002ee
	.section .rom.0004bdbc, "ax"
	.incbin "baserom.gba", 0x0004bdbc, 0x00000038
	.section .rom.0004bdf4, "ax"
	.global Func_0804bc08
	.type Func_0804bc08, %function
	.thumb_func
Func_0804bc08:
	.incbin "baserom.gba", 0x0004bdf4, 0x000011a8
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
	.incbin "baserom.gba", 0x0004d7ac, 0x000001a8
	.section .rom.0004d9fc, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9fc, 0x0000016c
	.section .rom.0004dcba, "ax"
	.incbin "baserom.gba", 0x0004dcba, 0x00000002
	.section .rom.0004dd74, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x0004dd74, 0x00000254
	.section .rom.0004e0a0, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0a0, 0x000000c4
	.section .rom.0004e2ce, "ax"
	.incbin "baserom.gba", 0x0004e2ce, 0x00000002
	.section .rom.0004e2d0, "ax"
	.global Func_0804e0d0
	.type Func_0804e0d0, %function
	.thumb_func
Func_0804e0d0:
	.incbin "baserom.gba", 0x0004e2d0, 0x0000007c
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
	.incbin "baserom.gba", 0x0004e5f4, 0x00000190
	.section .rom.0004e884, "ax"
	.incbin "baserom.gba", 0x0004e884, 0x000004d4
	.section .rom.000599f0, "ax"
	.incbin "baserom.gba", 0x000599f0, 0x00000084
	.section .rom.00059b74, "ax"
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00059b74, 0x00000d58
	.section .rom.0005cdcc, "ax"
	.incbin "baserom.gba", 0x0005cdcc, 0x00000010
	.section .rom.0005ce64, "ax"
	.incbin "baserom.gba", 0x0005ce64, 0x00000010
	.section .rom.0005f674, "ax"
	.incbin "baserom.gba", 0x0005f674, 0x0000018c
	.section .rom.0005f800, "ax"
	.global Data_0805eb50
Data_0805eb50:
	.incbin "baserom.gba", 0x0005f800, 0x00000008
	.section .rom.0005f808, "ax"
	.global Data_0805eb58
Data_0805eb58:
	.incbin "baserom.gba", 0x0005f808, 0x00000024
	.section .rom.0005f82c, "ax"
	.global Data_0805eb7c
Data_0805eb7c:
	.incbin "baserom.gba", 0x0005f82c, 0x00000108
	.section .rom.00060134, "ax"
	.incbin "baserom.gba", 0x00060134, 0x00000081
	.section .rom.000601b5, "ax"
	.global Data_0805f585
Data_0805f585:
	.incbin "baserom.gba", 0x000601b5, 0x000000bb
	.section .rom.00060270, "ax"
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00060270, 0x00000026
	.section .rom.00060296, "ax"
	.global Data_0805f666
Data_0805f666:
	.incbin "baserom.gba", 0x00060296, 0x0000007a
	.section .rom.000603e0, "ax"
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x000603e0, 0x00000008
	.section .rom.00060488, "ax"
	.incbin "baserom.gba", 0x00060488, 0x0000004f
	.section .rom.000604d7, "ax"
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000604d7, 0x0000000c
	.section .rom.000604e3, "ax"
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000604e3, 0x00000025
	.section .rom.00060508, "ax"
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x00060508, 0x0000003c
	.section .rom.0009d0d0, "ax"
	.incbin "baserom.gba", 0x0009d0d0, 0x0000000c
	.section .rom.0009d11c, "ax"
	.incbin "baserom.gba", 0x0009d11c, 0x00000548
	.section .rom.0009d664, "ax"
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0009d664, 0x00000004
	.section .rom.000a8070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000a8070, 0x00000008
	.section .rom.000a8078, "ax"
	.global BtlActGet
	.type BtlActGet, %function
	.thumb_func
BtlActGet:
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
	.section .rom.000a8090, "ax"
	.global Func_080ad090
	.type Func_080ad090, %function
	.thumb_func
Func_080ad090:
	.incbin "baserom.gba", 0x000a8090, 0x00000008
	.section .rom.000a8098, "ax"
	.global Func_080ad098
	.type Func_080ad098, %function
	.thumb_func
Func_080ad098:
	.incbin "baserom.gba", 0x000a8098, 0x00000008
	.section .rom.000a80a0, "ax"
	.global Func_080ad0a0
	.type Func_080ad0a0, %function
	.thumb_func
Func_080ad0a0:
	.incbin "baserom.gba", 0x000a80a0, 0x00000008
	.section .rom.000a80a8, "ax"
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
	.section .rom.000a80c8, "ax"
	.global UnitEpAddFar
	.type UnitEpAddFar, %function
	.thumb_func
UnitEpAddFar:
	.incbin "baserom.gba", 0x000a80c8, 0x00000008
	.section .rom.000a80d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000a80d0, 0x00000008
	.section .rom.000a80d8, "ax"
	.global Owner_UpdateRatioPairFar
	.type Owner_UpdateRatioPairFar, %function
	.thumb_func
Owner_UpdateRatioPairFar:
	.incbin "baserom.gba", 0x000a80d8, 0x00000008
	.section .rom.000a80e0, "ax"
	.global Owner_UpdateSecondInputAndRatiosFar
	.type Owner_UpdateSecondInputAndRatiosFar, %function
	.thumb_func
Owner_UpdateSecondInputAndRatiosFar:
	.incbin "baserom.gba", 0x000a80e0, 0x00000008
	.section .rom.000a80e8, "ax"
	.global BtlUnitSetFar
	.type BtlUnitSetFar, %function
	.thumb_func
BtlUnitSetFar:
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
	.section .rom.000a8100, "ax"
	.global Party_ListActiveOwnersFar
	.type Party_ListActiveOwnersFar, %function
	.thumb_func
Party_ListActiveOwnersFar:
	.incbin "baserom.gba", 0x000a8100, 0x00000008
	.section .rom.000a8108, "ax"
	.global RollWeaponUnleashFar
	.type RollWeaponUnleashFar, %function
	.thumb_func
RollWeaponUnleashFar:
	.incbin "baserom.gba", 0x000a8108, 0x00000008
	.section .rom.000a8110, "ax"
	.global Party_RemoveActiveOwnerFar
	.type Party_RemoveActiveOwnerFar, %function
	.thumb_func
Party_RemoveActiveOwnerFar:
	.incbin "baserom.gba", 0x000a8110, 0x00000008
	.section .rom.000a8118, "ax"
	.global Item_GetEquippedElementFar
	.type Item_GetEquippedElementFar, %function
	.thumb_func
Item_GetEquippedElementFar:
	.incbin "baserom.gba", 0x000a8118, 0x00000008
	.section .rom.000a8120, "ax"
	.global BtlHitChk
	.type BtlHitChk, %function
	.thumb_func
BtlHitChk:
	.incbin "baserom.gba", 0x000a8120, 0x00000008
	.section .rom.000a8128, "ax"
	.global BtlAtkCalc
	.type BtlAtkCalc, %function
	.thumb_func
BtlAtkCalc:
	.incbin "baserom.gba", 0x000a8128, 0x00000008
	.section .rom.000a8130, "ax"
	.global BtlPowCalc
	.type BtlPowCalc, %function
	.thumb_func
BtlPowCalc:
	.incbin "baserom.gba", 0x000a8130, 0x00000008
	.section .rom.000a8138, "ax"
	.global BtlRcvCalc
	.type BtlRcvCalc, %function
	.thumb_func
BtlRcvCalc:
	.incbin "baserom.gba", 0x000a8138, 0x00000008
	.section .rom.000a8140, "ax"
	.global Owner_GetRecordFar
	.type Owner_GetRecordFar, %function
	.thumb_func
Owner_GetRecordFar:
	.incbin "baserom.gba", 0x000a8140, 0x00000008
	.section .rom.000a8148, "ax"
	.global BtlRndFar
	.type BtlRndFar, %function
	.thumb_func
BtlRndFar:
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
	.section .rom.000a8160, "ax"
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
	.section .rom.000a8170, "ax"
	.global Trade_AddOfferFar
	.type Trade_AddOfferFar, %function
	.thumb_func
Trade_AddOfferFar:
	.incbin "baserom.gba", 0x000a8170, 0x00000008
	.section .rom.000a8178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000a8178, 0x00000008
	.section .rom.000a8180, "ax"
	.global Trade_ListFlaggedEntriesFar
	.type Trade_ListFlaggedEntriesFar, %function
	.thumb_func
Trade_ListFlaggedEntriesFar:
	.incbin "baserom.gba", 0x000a8180, 0x00000008
	.section .rom.000a8188, "ax"
	.global SummonDefinition_Get
	.type SummonDefinition_Get, %function
	.thumb_func
SummonDefinition_Get:
	.incbin "baserom.gba", 0x000a8188, 0x00000008
	.section .rom.000a8190, "ax"
	.global Djinn_GetDefinitionHeaderFar
	.type Djinn_GetDefinitionHeaderFar, %function
	.thumb_func
Djinn_GetDefinitionHeaderFar:
	.incbin "baserom.gba", 0x000a8190, 0x00000008
	.section .rom.000a8198, "ax"
	.global Party_AdvanceOwnerCountToTargetFar
	.type Party_AdvanceOwnerCountToTargetFar, %function
	.thumb_func
Party_AdvanceOwnerCountToTargetFar:
	.incbin "baserom.gba", 0x000a8198, 0x00000008
	.section .rom.000a81a0, "ax"
	.global Owner_GetResistanceValueFar
	.type Owner_GetResistanceValueFar, %function
	.thumb_func
Owner_GetResistanceValueFar:
	.incbin "baserom.gba", 0x000a81a0, 0x00000008
	.section .rom.000a81a8, "ax"
	.global Trade_CountPendingOffersFar
	.type Trade_CountPendingOffersFar, %function
	.thumb_func
Trade_CountPendingOffersFar:
	.incbin "baserom.gba", 0x000a81a8, 0x00000008
	.section .rom.000a81b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000a81b0, 0x00000008
	.section .rom.000a81b8, "ax"
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
	.section .rom.000a81c8, "ax"
	.global Item_IsCompatibleWithOwnerFar
	.type Item_IsCompatibleWithOwnerFar, %function
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	.incbin "baserom.gba", 0x000a81c8, 0x00000008
	.section .rom.000a81d0, "ax"
	.global Inventory_FindEquippedFar
	.type Inventory_FindEquippedFar, %function
	.thumb_func
Inventory_FindEquippedFar:
	.incbin "baserom.gba", 0x000a81d0, 0x00000008
	.section .rom.000a81d8, "ax"
	.global Party_AdjustCoinsFar
	.type Party_AdjustCoinsFar, %function
	.thumb_func
Party_AdjustCoinsFar:
	.incbin "baserom.gba", 0x000a81d8, 0x00000008
	.section .rom.000a81e0, "ax"
	.global Item_GetEquipmentGroupFar
	.type Item_GetEquipmentGroupFar, %function
	.thumb_func
Item_GetEquipmentGroupFar:
	.incbin "baserom.gba", 0x000a81e0, 0x00000008
	.section .rom.000a81e8, "ax"
	.global Item_AdjustCounterFar
	.type Item_AdjustCounterFar, %function
	.thumb_func
Item_AdjustCounterFar:
	.incbin "baserom.gba", 0x000a81e8, 0x00000008
	.section .rom.000a81f0, "ax"
	.global Inventory_CountFar
	.type Inventory_CountFar, %function
	.thumb_func
Inventory_CountFar:
	.incbin "baserom.gba", 0x000a81f0, 0x00000008
	.section .rom.000a81f8, "ax"
	.global PartyInventory_HasSpaceFar
	.type PartyInventory_HasSpaceFar, %function
	.thumb_func
PartyInventory_HasSpaceFar:
	.incbin "baserom.gba", 0x000a81f8, 0x00000008
	.section .rom.000a8200, "ax"
	.global Owner_GetLevelThresholdFar
	.type Owner_GetLevelThresholdFar, %function
	.thumb_func
Owner_GetLevelThresholdFar:
	.incbin "baserom.gba", 0x000a8200, 0x00000008
	.section .rom.000a8208, "ax"
	.global Func_080ad208
	.type Func_080ad208, %function
	.thumb_func
Func_080ad208:
	.incbin "baserom.gba", 0x000a8208, 0x00000008
	.section .rom.000a8210, "ax"
	.global Func_080ad210
	.type Func_080ad210, %function
	.thumb_func
Func_080ad210:
	.incbin "baserom.gba", 0x000a8210, 0x00000008
	.section .rom.000a8218, "ax"
	.global Party_SetLeaderJasmineFar
	.type Party_SetLeaderJasmineFar, %function
	.thumb_func
Party_SetLeaderJasmineFar:
	.incbin "baserom.gba", 0x000a8218, 0x00000008
	.section .rom.000a8220, "ax"
	.global Party_SetLeaderGarciaFar
	.type Party_SetLeaderGarciaFar, %function
	.thumb_func
Party_SetLeaderGarciaFar:
	.incbin "baserom.gba", 0x000a8220, 0x00000008
	.section .rom.000a8228, "ax"
	.global Func_080ad228
	.type Func_080ad228, %function
	.thumb_func
Func_080ad228:
	.incbin "baserom.gba", 0x000a8228, 0x00000008
	.section .rom.000a8230, "ax"
	.global Func_080ad230
	.type Func_080ad230, %function
	.thumb_func
Func_080ad230:
	.incbin "baserom.gba", 0x000a8230, 0x00000008
	.section .rom.000a8238, "ax"
	.global Func_080ad238
	.type Func_080ad238, %function
	.thumb_func
Func_080ad238:
	.incbin "baserom.gba", 0x000a8238, 0x00000008
	.section .rom.000a8240, "ax"
	.global Func_080ad240
	.type Func_080ad240, %function
	.thumb_func
Func_080ad240:
	.incbin "baserom.gba", 0x000a8240, 0x00000008
	.section .rom.000a8248, "ax"
	.global Djinn_AddToLeastLoadedOwnerFar
	.type Djinn_AddToLeastLoadedOwnerFar, %function
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	.incbin "baserom.gba", 0x000a8248, 0x00000008
	.section .rom.000a8250, "ax"
	.global Party_SumDjinnCountsFar
	.type Party_SumDjinnCountsFar, %function
	.thumb_func
Party_SumDjinnCountsFar:
	.incbin "baserom.gba", 0x000a8250, 0x00000008
	.section .rom.000a8258, "ax"
	.global Party_AdjustSixDigitCounterBFar
	.type Party_AdjustSixDigitCounterBFar, %function
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	.incbin "baserom.gba", 0x000a8258, 0x00000008
	.section .rom.000a8260, "ax"
	.global Party_AdjustCounterCappedAt28Far
	.type Party_AdjustCounterCappedAt28Far, %function
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	.incbin "baserom.gba", 0x000a8260, 0x00000008
	.section .rom.000a8268, "ax"
	.global Func_080ad268
	.type Func_080ad268, %function
	.thumb_func
Func_080ad268:
	.incbin "baserom.gba", 0x000a8268, 0x00000008
	.section .rom.000a8270, "ax"
	.global Func_080ad270
	.type Func_080ad270, %function
	.thumb_func
Func_080ad270:
	.incbin "baserom.gba", 0x000a8270, 0x00000008
	.section .rom.000a8278, "ax"
	.global Inventory_DiscardFar
	.type Inventory_DiscardFar, %function
	.thumb_func
Inventory_DiscardFar:
	.incbin "baserom.gba", 0x000a8278, 0x00000008
	.section .rom.000a8280, "ax"
	.global BtlEfxRevChkFar
	.type BtlEfxRevChkFar, %function
	.thumb_func
BtlEfxRevChkFar:
	.incbin "baserom.gba", 0x000a8280, 0x00000008
	.section .rom.000a8288, "ax"
	.global Owner_RefreshClassActionsFar
	.type Owner_RefreshClassActionsFar, %function
	.thumb_func
Owner_RefreshClassActionsFar:
	.incbin "baserom.gba", 0x000a8288, 0x00000008
	.section .rom.000a8290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000a8290, 0x00000008
	.section .rom.000a8298, "ax"
	.global Owner_RefreshDerivedDataFar
	.type Owner_RefreshDerivedDataFar, %function
	.thumb_func
Owner_RefreshDerivedDataFar:
	.incbin "baserom.gba", 0x000a8298, 0x00000008
	.section .rom.000a82a0, "ax"
	.global Inventory_CountItemFar
	.type Inventory_CountItemFar, %function
	.thumb_func
Inventory_CountItemFar:
	.incbin "baserom.gba", 0x000a82a0, 0x00000008
	.section .rom.000a82a8, "ax"
	.global PartyInventory_CountItemFar
	.type PartyInventory_CountItemFar, %function
	.thumb_func
PartyInventory_CountItemFar:
	.incbin "baserom.gba", 0x000a82a8, 0x00000008
	.section .rom.000a82b0, "ax"
	.global PartyInventory_DiscardFar
	.type PartyInventory_DiscardFar, %function
	.thumb_func
PartyInventory_DiscardFar:
	.incbin "baserom.gba", 0x000a82b0, 0x00000008
	.section .rom.000a83f6, "ax"
	.incbin "baserom.gba", 0x000a83f6, 0x00000002
	.section .rom.000a83f8, "ax"
	.global UnitCalc
	.type UnitCalc, %function
	.thumb_func
UnitCalc:
	.incbin "baserom.gba", 0x000a83f8, 0x000007f4
	.section .rom.000a8bec, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x000a8bec, 0x000000a4
	.section .rom.000a8df0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000a8df0, 0x000002e8
	.section .rom.000a9354, "ax"
	.global Func_080ae358
	.type Func_080ae358, %function
	.thumb_func
Func_080ae358:
	.incbin "baserom.gba", 0x000a9354, 0x000000a4
	.section .rom.000a940c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000a940c, 0x000001ec
	.section .rom.000a95f8, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000a95f8, 0x000000cc
	.section .rom.000a96e0, "ax"
	.incbin "baserom.gba", 0x000a96e0, 0x0000014c
	.section .rom.000a9864, "ax"
	.global Func_080ae868
	.type Func_080ae868, %function
	.thumb_func
Func_080ae868:
	.incbin "baserom.gba", 0x000a9864, 0x000001c8
	.section .rom.000a9c62, "ax"
	.incbin "baserom.gba", 0x000a9c62, 0x00000002
	.section .rom.000a9c64, "ax"
	.global Item_GetEquipmentGroup
	.type Item_GetEquipmentGroup, %function
	.thumb_func
Item_GetEquipmentGroup:
	.incbin "baserom.gba", 0x000a9c64, 0x0000003c
	.section .rom.000a9f30, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x000a9f30, 0x000000d4
	.section .rom.000aa0de, "ax"
	.incbin "baserom.gba", 0x000aa0de, 0x00000002
	.section .rom.000aa0e0, "ax"
	.global Func_080af0e4
	.type Func_080af0e4, %function
	.thumb_func
Func_080af0e4:
	.incbin "baserom.gba", 0x000aa0e0, 0x00000064
	.section .rom.000aa144, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x000aa144, 0x00000080
	.section .rom.000aa4e0, "ax"
	.balign 4
	.global Owner_UpdateClassActions
	.type Owner_UpdateClassActions, %function
	.thumb_func
Owner_UpdateClassActions:
	.incbin "baserom.gba", 0x000aa4e0, 0x00000210
	.section .rom.000aa7a8, "ax"
	.incbin "baserom.gba", 0x000aa7a8, 0x00000120
	.section .rom.000aa918, "ax"
	.global Owner_LevelUp
	.type Owner_LevelUp, %function
	.thumb_func
Owner_LevelUp:
	.incbin "baserom.gba", 0x000aa918, 0x00000264
	.section .rom.000aab7c, "ax"
	.global Func_080afb80
	.type Func_080afb80, %function
	.thumb_func
Func_080afb80:
	.incbin "baserom.gba", 0x000aab7c, 0x00000034
	.section .rom.000aabe8, "ax"
	.incbin "baserom.gba", 0x000aabe8, 0x000001d0
	.section .rom.000aaf24, "ax"
	.incbin "baserom.gba", 0x000aaf24, 0x00000054
	.section .rom.000aafa8, "ax"
	.balign 4
	.global Owner_GetDigitValues
	.type Owner_GetDigitValues, %function
	.thumb_func
Owner_GetDigitValues:
	.incbin "baserom.gba", 0x000aafa8, 0x0000007c
	.section .rom.000ab056, "ax"
	.incbin "baserom.gba", 0x000ab056, 0x00000002
	.section .rom.000ab058, "ax"
	.global Owner_GetDefaultElement
	.type Owner_GetDefaultElement, %function
	.thumb_func
Owner_GetDefaultElement:
	.incbin "baserom.gba", 0x000ab058, 0x00000028
	.section .rom.000ab080, "ax"
	.global Owner_CalculateBaseElements
	.type Owner_CalculateBaseElements, %function
	.thumb_func
Owner_CalculateBaseElements:
	.incbin "baserom.gba", 0x000ab080, 0x000000c0
	.section .rom.000ab140, "ax"
	.global Owner_DetermineClass
	.type Owner_DetermineClass, %function
	.thumb_func
Owner_DetermineClass:
	.incbin "baserom.gba", 0x000ab140, 0x00000144
	.section .rom.000ab510, "ax"
	.incbin "baserom.gba", 0x000ab510, 0x00000194
	.section .rom.000ab758, "ax"
	.align 2, 0
	.incbin "baserom.gba", 0x000ab758, 0x00000338
	.section .rom.000abbb2, "ax"
	.incbin "baserom.gba", 0x000abbb2, 0x00000002
	.section .rom.000abbb4, "ax"
	.global Trade_CanOfferDjinn
	.type Trade_CanOfferDjinn, %function
	.thumb_func
Trade_CanOfferDjinn:
	.incbin "baserom.gba", 0x000abbb4, 0x000000c0
	.section .rom.000abd54, "ax"
	.global Trade_RemoveOffer
	.type Trade_RemoveOffer, %function
	.thumb_func
Trade_RemoveOffer:
	.incbin "baserom.gba", 0x000abd54, 0x000000ac
	.section .rom.000ac068, "ax"
	.global Func_080b1088
	.type Func_080b1088, %function
	.thumb_func
Func_080b1088:
	.incbin "baserom.gba", 0x000ac068, 0x00000160
	.section .rom.000ac1c8, "ax"
	.global Func_080b11f8
	.type Func_080b11f8, %function
	.thumb_func
Func_080b11f8:
	.incbin "baserom.gba", 0x000ac1c8, 0x00000084
	.section .rom.000ac24c, "ax"
	.global Data_080b127c
Data_080b127c:
	.incbin "baserom.gba", 0x000ac24c, 0x0000004c
	.section .rom.000acef8, "ax"
	.incbin "baserom.gba", 0x000acef8, 0x00000004
	.section .rom.000c1ad4, "ax"
	.global Data_080c6b04
Data_080c6b04:
	.incbin "baserom.gba", 0x000c1ad4, 0x00000014
	.section .rom.000c1ae8, "ax"
	.global Battle_Effect90Classes
Battle_Effect90Classes:
	.incbin "baserom.gba", 0x000c1ae8, 0x0000005e
	.section .rom.000c1b46, "ax"
	.global Battle_Effect91Classes
Battle_Effect91Classes:
	.incbin "baserom.gba", 0x000c1b46, 0x0000003a
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
	.section .rom.000c82a8, "ax"
	.global Func_080c82a8
	.type Func_080c82a8, %function
	.thumb_func
Func_080c82a8:
	.incbin "baserom.gba", 0x000c82a8, 0x00000018
	.section .rom.000c82c0, "ax"
	.global Func_080c82c0
	.type Func_080c82c0, %function
	.thumb_func
Func_080c82c0:
	.incbin "baserom.gba", 0x000c82c0, 0x00000018
	.section .rom.000c82d8, "ax"
	.global BattleFx_FindNearestObjectFar
	.type BattleFx_FindNearestObjectFar, %function
	.thumb_func
BattleFx_FindNearestObjectFar:
	.incbin "baserom.gba", 0x000c82d8, 0x00000008
	.section .rom.000c82e0, "ax"
	.global Func_080c82e0
	.type Func_080c82e0, %function
	.thumb_func
Func_080c82e0:
	.incbin "baserom.gba", 0x000c82e0, 0x00000008
	.section .rom.000c82e8, "ax"
	.global Func_080c82e8
	.type Func_080c82e8, %function
	.thumb_func
Func_080c82e8:
	.incbin "baserom.gba", 0x000c82e8, 0x00000010
	.section .rom.000c82f8, "ax"
	.global EventRuntime_PrepareCurrentObjectFar
	.type EventRuntime_PrepareCurrentObjectFar, %function
	.thumb_func
EventRuntime_PrepareCurrentObjectFar:
	.incbin "baserom.gba", 0x000c82f8, 0x00000038
	.section .rom.000c8330, "ax"
	.global Func_080c8330
	.type Func_080c8330, %function
	.thumb_func
Func_080c8330:
	.incbin "baserom.gba", 0x000c8330, 0x00000020
	.section .rom.000c8350, "ax"
	.global DisplayTransition_StartFar
	.type DisplayTransition_StartFar, %function
	.thumb_func
DisplayTransition_StartFar:
	.incbin "baserom.gba", 0x000c8350, 0x00000018
	.section .rom.000c8368, "ax"
	.global Func_080c8368
	.type Func_080c8368, %function
	.thumb_func
Func_080c8368:
	.incbin "baserom.gba", 0x000c8368, 0x00000010
	.section .rom.000c83a8, "ax"
	.global Event_StartScreenTransitionFar
	.type Event_StartScreenTransitionFar, %function
	.thumb_func
Event_StartScreenTransitionFar:
	.incbin "baserom.gba", 0x000c83a8, 0x00000008
	.section .rom.000c83b0, "ax"
	.global Event_FinishScreenTransitionFar
	.type Event_FinishScreenTransitionFar, %function
	.thumb_func
Event_FinishScreenTransitionFar:
	.incbin "baserom.gba", 0x000c83b0, 0x00000008
	.section .rom.000c83b8, "ax"
	.global Event_WaitTransitionFramesFar
	.type Event_WaitTransitionFramesFar, %function
	.thumb_func
Event_WaitTransitionFramesFar:
	.incbin "baserom.gba", 0x000c83b8, 0x00000008
	.section .rom.000c83c0, "ax"
	.global DisplayScroll_InitializeHBlankDmaFar
	.type DisplayScroll_InitializeHBlankDmaFar, %function
	.thumb_func
DisplayScroll_InitializeHBlankDmaFar:
	.incbin "baserom.gba", 0x000c83c0, 0x00000018
	.section .rom.000c83d8, "ax"
	.global Func_080c83d8
	.type Func_080c83d8, %function
	.thumb_func
Func_080c83d8:
	.incbin "baserom.gba", 0x000c83d8, 0x00000008
	.section .rom.000c83e0, "ax"
	.global Func_080c83e0
	.type Func_080c83e0, %function
	.thumb_func
Func_080c83e0:
	.incbin "baserom.gba", 0x000c83e0, 0x00000028
	.section .rom.000c8408, "ax"
	.global ObjectEvent_PlaceOnGroundFar
	.type ObjectEvent_PlaceOnGroundFar, %function
	.thumb_func
ObjectEvent_PlaceOnGroundFar:
	.incbin "baserom.gba", 0x000c8408, 0x00000018
	.section .rom.000c8420, "ax"
	.global Object_SetWideSpriteFar
	.type Object_SetWideSpriteFar, %function
	.thumb_func
Object_SetWideSpriteFar:
	.incbin "baserom.gba", 0x000c8420, 0x00000008
	.section .rom.000c8428, "ax"
	.global Object_ReturnHomeFar
	.type Object_ReturnHomeFar, %function
	.thumb_func
Object_ReturnHomeFar:
	.incbin "baserom.gba", 0x000c8428, 0x00000008
	.section .rom.000c8430, "ax"
	.global Func_080c8430
	.type Func_080c8430, %function
	.thumb_func
Func_080c8430:
	.incbin "baserom.gba", 0x000c8430, 0x00000008
	.section .rom.000c8438, "ax"
	.global Func_080c8438
	.type Func_080c8438, %function
	.thumb_func
Func_080c8438:
	.incbin "baserom.gba", 0x000c8438, 0x00000020
	.section .rom.000c8458, "ax"
	.global ObjectTable_GetFar
	.type ObjectTable_GetFar, %function
	.thumb_func
ObjectTable_GetFar:
	.incbin "baserom.gba", 0x000c8458, 0x00000028
	.section .rom.000c8480, "ax"
	.global Func_080c8480
	.type Func_080c8480, %function
	.thumb_func
Func_080c8480:
	.incbin "baserom.gba", 0x000c8480, 0x00000018
	.section .rom.000c8498, "ax"
	.global Field_DispatchTypeHandlerFar
	.type Field_DispatchTypeHandlerFar, %function
	.thumb_func
Field_DispatchTypeHandlerFar:
	.incbin "baserom.gba", 0x000c8498, 0x00000008
	.section .rom.000c84a0, "ax"
	.global BattleFx_LoadActionEffectResourcesFar
	.type BattleFx_LoadActionEffectResourcesFar, %function
	.thumb_func
BattleFx_LoadActionEffectResourcesFar:
	.incbin "baserom.gba", 0x000c84a0, 0x00000008
	.section .rom.000c84a8, "ax"
	.global BattleFx_SetupObjectPairFar
	.type BattleFx_SetupObjectPairFar, %function
	.thumb_func
BattleFx_SetupObjectPairFar:
	.incbin "baserom.gba", 0x000c84a8, 0x00000008
	.section .rom.000c84b0, "ax"
	.global EffectRuntime_StopCurrentObjectFar
	.type EffectRuntime_StopCurrentObjectFar, %function
	.thumb_func
EffectRuntime_StopCurrentObjectFar:
	.incbin "baserom.gba", 0x000c84b0, 0x00000008
	.section .rom.000c84b8, "ax"
	.global BattleEffect_CleanupSceneObjectsFar
	.type BattleEffect_CleanupSceneObjectsFar, %function
	.thumb_func
BattleEffect_CleanupSceneObjectsFar:
	.incbin "baserom.gba", 0x000c84b8, 0x00000008
	.section .rom.000c84c0, "ax"
	.global EventObject_InitializeFar
	.type EventObject_InitializeFar, %function
	.thumb_func
EventObject_InitializeFar:
	.incbin "baserom.gba", 0x000c84c0, 0x00000010
	.section .rom.000c84d0, "ax"
	.global BattleFx_PrepareBufferInterpolationFar
	.type BattleFx_PrepareBufferInterpolationFar, %function
	.thumb_func
BattleFx_PrepareBufferInterpolationFar:
	.incbin "baserom.gba", 0x000c84d0, 0x00000008
	.section .rom.000c84d8, "ax"
	.global EventRuntime_ResolveAllPendingActionsFar
	.type EventRuntime_ResolveAllPendingActionsFar, %function
	.thumb_func
EventRuntime_ResolveAllPendingActionsFar:
	.incbin "baserom.gba", 0x000c84d8, 0x00000008
	.section .rom.000c84e0, "ax"
	.global EventRuntime_ResolvePendingActionsFar
	.type EventRuntime_ResolvePendingActionsFar, %function
	.thumb_func
EventRuntime_ResolvePendingActionsFar:
	.incbin "baserom.gba", 0x000c84e0, 0x00000008
	.section .rom.000c84e8, "ax"
	.global Func_080c84e8
	.type Func_080c84e8, %function
	.thumb_func
Func_080c84e8:
	.incbin "baserom.gba", 0x000c84e8, 0x00000008
	.section .rom.000c84f0, "ax"
	.global Func_080c84f0
	.type Func_080c84f0, %function
	.thumb_func
Func_080c84f0:
	.incbin "baserom.gba", 0x000c84f0, 0x00000008
	.section .rom.000c84f8, "ax"
	.global Func_080c84f8
	.type Func_080c84f8, %function
	.thumb_func
Func_080c84f8:
	.incbin "baserom.gba", 0x000c84f8, 0x00000008
	.section .rom.000c8500, "ax"
	.global Func_080c8500
	.type Func_080c8500, %function
	.thumb_func
Func_080c8500:
	.incbin "baserom.gba", 0x000c8500, 0x00000008
	.section .rom.000c8520, "ax"
	.incbin "baserom.gba", 0x000c8520, 0x00000008
	.section .rom.000c8528, "ax"
	.global BattleFx_StartItemBreakFar
	.type BattleFx_StartItemBreakFar, %function
	.thumb_func
BattleFx_StartItemBreakFar:
	.incbin "baserom.gba", 0x000c8528, 0x00000008
	.section .rom.000c8530, "ax"
	.global BattleFx_SnapScaleToFullFar
	.type BattleFx_SnapScaleToFullFar, %function
	.thumb_func
BattleFx_SnapScaleToFullFar:
	.incbin "baserom.gba", 0x000c8530, 0x00000008
	.section .rom.000c8538, "ax"
	.global UpdateRisingParticleBurstFar
	.type UpdateRisingParticleBurstFar, %function
	.thumb_func
UpdateRisingParticleBurstFar:
	.incbin "baserom.gba", 0x000c8538, 0x00000008
	.section .rom.000c8540, "ax"
	.global BattleParty_ApplyDrainFar
	.type BattleParty_ApplyDrainFar, %function
	.thumb_func
BattleParty_ApplyDrainFar:
	.incbin "baserom.gba", 0x000c8540, 0x00000008
	.section .rom.000c8548, "ax"
	.global BattleParty_ApplyHealthDeltaFar
	.type BattleParty_ApplyHealthDeltaFar, %function
	.thumb_func
BattleParty_ApplyHealthDeltaFar:
	.incbin "baserom.gba", 0x000c8548, 0x00000010
	.section .rom.000c8558, "ax"
	.global Func_080c8558
	.type Func_080c8558, %function
	.thumb_func
Func_080c8558:
	.incbin "baserom.gba", 0x000c8558, 0x00000018
	.section .rom.000c8570, "ax"
	.global Party_PlaySavedMusicFar
	.type Party_PlaySavedMusicFar, %function
	.thumb_func
Party_PlaySavedMusicFar:
	.incbin "baserom.gba", 0x000c8570, 0x00000008
	.section .rom.000c8578, "ax"
	.global Scene_ResolveInteractionResultFar
	.type Scene_ResolveInteractionResultFar, %function
	.thumb_func
Scene_ResolveInteractionResultFar:
	.incbin "baserom.gba", 0x000c8578, 0x00000008
	.section .rom.000c8580, "ax"
	.global Func_080c8580
	.type Func_080c8580, %function
	.thumb_func
Func_080c8580:
	.incbin "baserom.gba", 0x000c8580, 0x00000008
	.section .rom.000c8588, "ax"
	.global EffectSlot_UpdateFar
	.type EffectSlot_UpdateFar, %function
	.thumb_func
EffectSlot_UpdateFar:
	.incbin "baserom.gba", 0x000c8588, 0x00000038
	.section .rom.000c85c0, "ax"
	.global Func_080c85c0
	.type Func_080c85c0, %function
	.thumb_func
Func_080c85c0:
	.incbin "baserom.gba", 0x000c85c0, 0x00000008
	.section .rom.000c85c8, "ax"
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
	.section .rom.000c85e8, "ax"
	.global Motion_LaunchFromFocusedObjectFar
	.type Motion_LaunchFromFocusedObjectFar, %function
	.thumb_func
Motion_LaunchFromFocusedObjectFar:
	.incbin "baserom.gba", 0x000c85e8, 0x00000008
	.section .rom.000c85f0, "ax"
	.global ObjectMotion_OffsetPositionAndResetFar
	.type ObjectMotion_OffsetPositionAndResetFar, %function
	.thumb_func
ObjectMotion_OffsetPositionAndResetFar:
	.incbin "baserom.gba", 0x000c85f0, 0x00000008
	.section .rom.000c85f8, "ax"
	.global ObjectMotion_CommitPositionAndActivateFar
	.type ObjectMotion_CommitPositionAndActivateFar, %function
	.thumb_func
ObjectMotion_CommitPositionAndActivateFar:
	.incbin "baserom.gba", 0x000c85f8, 0x00000008
	.section .rom.000c8600, "ax"
	.global Object_LinkObjectAndSetCallbackFar
	.type Object_LinkObjectAndSetCallbackFar, %function
	.thumb_func
Object_LinkObjectAndSetCallbackFar:
	.incbin "baserom.gba", 0x000c8600, 0x00000020
	.section .rom.000c8620, "ax"
	.global ObjectTable_ReadActiveValueFar
	.type ObjectTable_ReadActiveValueFar, %function
	.thumb_func
ObjectTable_ReadActiveValueFar:
	.incbin "baserom.gba", 0x000c8620, 0x00000008
	.section .rom.000c8648, "ax"
	.global Scene_GetAreaNameIndexFar
	.type Scene_GetAreaNameIndexFar, %function
	.thumb_func
Scene_GetAreaNameIndexFar:
	.incbin "baserom.gba", 0x000c8648, 0x00000038
	.section .rom.000c8680, "ax"
	.global Object_SetActionCallbackFar
	.type Object_SetActionCallbackFar, %function
	.thumb_func
Object_SetActionCallbackFar:
	.incbin "baserom.gba", 0x000c8680, 0x00000008
	.section .rom.000c8688, "ax"
	.global Func_080c8688
	.type Func_080c8688, %function
	.thumb_func
Func_080c8688:
	.incbin "baserom.gba", 0x000c8688, 0x00000008
	.section .rom.000c8690, "ax"
	.global Event_NoOpFar
	.type Event_NoOpFar, %function
	.thumb_func
Event_NoOpFar:
	.incbin "baserom.gba", 0x000c8690, 0x00000008
	.section .rom.000c8698, "ax"
	.global Func_080c8698
	.type Func_080c8698, %function
	.thumb_func
Func_080c8698:
	.incbin "baserom.gba", 0x000c8698, 0x00000010
	.section .rom.000c86a8, "ax"
	.global Func_080c86a8
	.type Func_080c86a8, %function
	.thumb_func
Func_080c86a8:
	.incbin "baserom.gba", 0x000c86a8, 0x00000008
	.section .rom.000c86b0, "ax"
	.global Func_080c86b0
	.type Func_080c86b0, %function
	.thumb_func
Func_080c86b0:
	.incbin "baserom.gba", 0x000c86b0, 0x00000008
	.section .rom.000c86b8, "ax"
	.global Func_080c86b8
	.type Func_080c86b8, %function
	.thumb_func
Func_080c86b8:
	.incbin "baserom.gba", 0x000c86b8, 0x00000008
	.section .rom.000c86c0, "ax"
	.global Func_080c86c0
	.type Func_080c86c0, %function
	.thumb_func
Func_080c86c0:
	.incbin "baserom.gba", 0x000c86c0, 0x00000010
	.section .rom.000c86d0, "ax"
	.global Func_080c86d0
	.type Func_080c86d0, %function
	.thumb_func
Func_080c86d0:
	.incbin "baserom.gba", 0x000c86d0, 0x00000008
	.section .rom.000c86d8, "ax"
	.global Func_080c86d8
	.type Func_080c86d8, %function
	.thumb_func
Func_080c86d8:
	.incbin "baserom.gba", 0x000c86d8, 0x00000008
	.section .rom.000c86e0, "ax"
	.global Func_080c86e0
	.type Func_080c86e0, %function
	.thumb_func
Func_080c86e0:
	.incbin "baserom.gba", 0x000c86e0, 0x00000008
	.section .rom.000c86e8, "ax"
	.global Func_080c86e8
	.type Func_080c86e8, %function
	.thumb_func
Func_080c86e8:
	.incbin "baserom.gba", 0x000c86e8, 0x00000008
	.section .rom.000c86f0, "ax"
	.global Func_080c86f0
	.type Func_080c86f0, %function
	.thumb_func
Func_080c86f0:
	.incbin "baserom.gba", 0x000c86f0, 0x00000008
	.section .rom.000c86f8, "ax"
	.global Func_080c86f8
	.type Func_080c86f8, %function
	.thumb_func
Func_080c86f8:
	.incbin "baserom.gba", 0x000c86f8, 0x00000008
	.section .rom.000c8700, "ax"
	.global Func_080c8700
	.type Func_080c8700, %function
	.thumb_func
Func_080c8700:
	.incbin "baserom.gba", 0x000c8700, 0x00000008
	.section .rom.000c8708, "ax"
	.global Func_080c8708
	.type Func_080c8708, %function
	.thumb_func
Func_080c8708:
	.incbin "baserom.gba", 0x000c8708, 0x00000008
	.section .rom.000c8710, "ax"
	.global Func_080c8710
	.type Func_080c8710, %function
	.thumb_func
Func_080c8710:
	.incbin "baserom.gba", 0x000c8710, 0x00000008
	.section .rom.000c8718, "ax"
	.global Func_080c8718
	.type Func_080c8718, %function
	.thumb_func
Func_080c8718:
	.incbin "baserom.gba", 0x000c8718, 0x00000008
	.section .rom.000c8720, "ax"
	.global Func_080c8720
	.type Func_080c8720, %function
	.thumb_func
Func_080c8720:
	.incbin "baserom.gba", 0x000c8720, 0x00000008
	.section .rom.000c8728, "ax"
	.global Func_080c8728
	.type Func_080c8728, %function
	.thumb_func
Func_080c8728:
	.incbin "baserom.gba", 0x000c8728, 0x00000008
	.section .rom.000c8730, "ax"
	.global Func_080c8730
	.type Func_080c8730, %function
	.thumb_func
Func_080c8730:
	.incbin "baserom.gba", 0x000c8730, 0x00000008
	.section .rom.000c8738, "ax"
	.global Func_080c8738
	.type Func_080c8738, %function
	.thumb_func
Func_080c8738:
	.incbin "baserom.gba", 0x000c8738, 0x00000008
	.section .rom.000c8740, "ax"
	.global Func_080c8740
	.type Func_080c8740, %function
	.thumb_func
Func_080c8740:
	.incbin "baserom.gba", 0x000c8740, 0x00000008
	.section .rom.000c8748, "ax"
	.global Func_080c8748
	.type Func_080c8748, %function
	.thumb_func
Func_080c8748:
	.incbin "baserom.gba", 0x000c8748, 0x00000008
	.section .rom.000c8750, "ax"
	.global Func_080c8750
	.type Func_080c8750, %function
	.thumb_func
Func_080c8750:
	.incbin "baserom.gba", 0x000c8750, 0x00000008
	.section .rom.000c8758, "ax"
	.global Object_SpawnFar
	.type Object_SpawnFar, %function
	.thumb_func
Object_SpawnFar:
	.incbin "baserom.gba", 0x000c8758, 0x00000008
	.section .rom.000c8760, "ax"
	.global Func_080c8760
	.type Func_080c8760, %function
	.thumb_func
Func_080c8760:
	.incbin "baserom.gba", 0x000c8760, 0x00000018
	.section .rom.000c8778, "ax"
	.global EventRuntime_GetControlledOwnerFar
	.type EventRuntime_GetControlledOwnerFar, %function
	.thumb_func
EventRuntime_GetControlledOwnerFar:
	.incbin "baserom.gba", 0x000c8778, 0x00000008
	.section .rom.000c8780, "ax"
	.global Func_080c8780
	.type Func_080c8780, %function
	.thumb_func
Func_080c8780:
	.incbin "baserom.gba", 0x000c8780, 0x00000018
	.section .rom.000c8798, "ax"
	.global Func_080c8798
	.type Func_080c8798, %function
	.thumb_func
Func_080c8798:
	.incbin "baserom.gba", 0x000c8798, 0x00000008
	.section .rom.000c87a0, "ax"
	.global Func_080c87a0
	.type Func_080c87a0, %function
	.thumb_func
Func_080c87a0:
	.incbin "baserom.gba", 0x000c87a0, 0x00000008
	.section .rom.000c87a8, "ax"
	.global Func_080c87a8
	.type Func_080c87a8, %function
	.thumb_func
Func_080c87a8:
	.incbin "baserom.gba", 0x000c87a8, 0x00000008
	.section .rom.000c87b0, "ax"
	.global Func_080c87b0
	.type Func_080c87b0, %function
	.thumb_func
Func_080c87b0:
	.incbin "baserom.gba", 0x000c87b0, 0x00000008
	.section .rom.000c87b8, "ax"
	.global Func_080c87b8
	.type Func_080c87b8, %function
	.thumb_func
Func_080c87b8:
	.incbin "baserom.gba", 0x000c87b8, 0x00000008
	.section .rom.000c87c0, "ax"
	.global Func_080c87c0
	.type Func_080c87c0, %function
	.thumb_func
Func_080c87c0:
	.incbin "baserom.gba", 0x000c87c0, 0x00000008
	.section .rom.000c87c8, "ax"
	.global Func_080c87c8
	.type Func_080c87c8, %function
	.thumb_func
Func_080c87c8:
	.incbin "baserom.gba", 0x000c87c8, 0x00000008
	.section .rom.000c87d0, "ax"
	.global Func_080c87d0
	.type Func_080c87d0, %function
	.thumb_func
Func_080c87d0:
	.incbin "baserom.gba", 0x000c87d0, 0x00000010
	.section .rom.000c87e0, "ax"
	.global BattleFx_StepVector3Far
	.type BattleFx_StepVector3Far, %function
	.thumb_func
BattleFx_StepVector3Far:
	.incbin "baserom.gba", 0x000c87e0, 0x00000008
	.section .rom.000c87e8, "ax"
	.global Func_080c87e8
	.type Func_080c87e8, %function
	.thumb_func
Func_080c87e8:
	.incbin "baserom.gba", 0x000c87e8, 0x00000008
	.section .rom.000c87f0, "ax"
	.global Func_080c87f0
	.type Func_080c87f0, %function
	.thumb_func
Func_080c87f0:
	.incbin "baserom.gba", 0x000c87f0, 0x00000008
	.section .rom.000c87f8, "ax"
	.global Func_080c87f8
	.type Func_080c87f8, %function
	.thumb_func
Func_080c87f8:
	.incbin "baserom.gba", 0x000c87f8, 0x00000008
	.section .rom.000c8800, "ax"
	.global Field_BeginPaletteTransitionFar
	.type Field_BeginPaletteTransitionFar, %function
	.thumb_func
Field_BeginPaletteTransitionFar:
	.incbin "baserom.gba", 0x000c8800, 0x00000008
	.section .rom.000c8808, "ax"
	.global BattleFx_StartArcEffectFar
	.type BattleFx_StartArcEffectFar, %function
	.thumb_func
BattleFx_StartArcEffectFar:
	.incbin "baserom.gba", 0x000c8808, 0x00000008
	.section .rom.000c8810, "ax"
	.global BattleFx_EndArcEffectFar
	.type BattleFx_EndArcEffectFar, %function
	.thumb_func
BattleFx_EndArcEffectFar:
	.incbin "baserom.gba", 0x000c8810, 0x00000008
	.section .rom.000c8818, "ax"
	.global BattleFx_WaitForArcGroupsFar
	.type BattleFx_WaitForArcGroupsFar, %function
	.thumb_func
BattleFx_WaitForArcGroupsFar:
	.incbin "baserom.gba", 0x000c8818, 0x00000008
	.section .rom.000c8820, "ax"
	.global BattleFx_SetObjectTickModeFar
	.type BattleFx_SetObjectTickModeFar, %function
	.thumb_func
BattleFx_SetObjectTickModeFar:
	.incbin "baserom.gba", 0x000c8820, 0x00000008
	.section .rom.000c88c8, "ax"
	.global Func_080c88c8
	.type Func_080c88c8, %function
	.thumb_func
Func_080c88c8:
	.incbin "baserom.gba", 0x000c88c8, 0x00000004
	.section .rom.000c88cc, "ax"
	.global BattleFx_DecodeFrameTranslationBase
BattleFx_DecodeFrameTranslationBase:
	.incbin "baserom.gba", 0x000c88cc, 0x00000004
	.section .rom.000c88d0, "ax"
	.global Func_080c88d0
	.type Func_080c88d0, %function
	.thumb_func
Func_080c88d0:
	.incbin "baserom.gba", 0x000c88d0, 0x00000008
	.section .rom.000c88d8, "ax"
	.global Func_080c88d8
	.type Func_080c88d8, %function
	.thumb_func
Func_080c88d8:
	.incbin "baserom.gba", 0x000c88d8, 0x00000008
	.section .rom.000c88e0, "ax"
	.global Func_080c88e0
	.type Func_080c88e0, %function
	.thumb_func
Func_080c88e0:
	.incbin "baserom.gba", 0x000c88e0, 0x00000008
	.section .rom.000c88e8, "ax"
	.global Func_080c88e8
	.type Func_080c88e8, %function
	.thumb_func
Func_080c88e8:
	.incbin "baserom.gba", 0x000c88e8, 0x00000004
	.section .rom.000c88ec, "ax"
	.global BattleFx_DecodeFrameUnusedBase
BattleFx_DecodeFrameUnusedBase:
	.incbin "baserom.gba", 0x000c88ec, 0x00000004
	.section .rom.000c88f0, "ax"
	.global Func_080c88f0
	.type Func_080c88f0, %function
	.thumb_func
Func_080c88f0:
	.incbin "baserom.gba", 0x000c88f0, 0x00000008
	.section .rom.000c88f8, "ax"
	.global ObjectMotion_ResetTargetsAndVelocityFar
	.type ObjectMotion_ResetTargetsAndVelocityFar, %function
	.thumb_func
ObjectMotion_ResetTargetsAndVelocityFar:
	.incbin "baserom.gba", 0x000c88f8, 0x00000008
	.section .rom.000c8900, "ax"
	.global ObjectEffect_BeginContextEffect26Far
	.type ObjectEffect_BeginContextEffect26Far, %function
	.thumb_func
ObjectEffect_BeginContextEffect26Far:
	.incbin "baserom.gba", 0x000c8900, 0x00000018
	.section .rom.000c8918, "ax"
	.global Func_080c8918
	.type Func_080c8918, %function
	.thumb_func
Func_080c8918:
	.incbin "baserom.gba", 0x000c8918, 0x00000010
	.section .rom.000c8928, "ax"
	.global Func_080c8928
	.type Func_080c8928, %function
	.thumb_func
Func_080c8928:
	.incbin "baserom.gba", 0x000c8928, 0x00000008
	.section .rom.000c8930, "ax"
	.global Func_080c8930
	.type Func_080c8930, %function
	.thumb_func
Func_080c8930:
	.incbin "baserom.gba", 0x000c8930, 0x00000010
	.section .rom.000c8940, "ax"
	.global Func_080c8940
	.type Func_080c8940, %function
	.thumb_func
Func_080c8940:
	.incbin "baserom.gba", 0x000c8940, 0x00000008
	.section .rom.000c8948, "ax"
	.global Func_080c8948
	.type Func_080c8948, %function
	.thumb_func
Func_080c8948:
	.incbin "baserom.gba", 0x000c8948, 0x00000008
	.section .rom.000c9694, "ax"
	.incbin "baserom.gba", 0x000c9694, 0x0000028c
	.section .rom.000c9944, "ax"
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
	.incbin "baserom.gba", 0x000ca4a8, 0x00000130
	.section .rom.000ca5d8, "ax"
	.global BattleFx_SelectBattleCue
	.type BattleFx_SelectBattleCue, %function
	.thumb_func
BattleFx_SelectBattleCue:
	.incbin "baserom.gba", 0x000ca5d8, 0x0000010c
	.section .rom.000ca6e8, "ax"
	.balign 4
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
	.type Func_080caa2c, %function
	.thumb_func
Func_080caa2c:
	.incbin "baserom.gba", 0x000caa2c, 0x00000020
	.section .rom.000caa4c, "ax"
	.global Func_080caa4c
	.type Func_080caa4c, %function
	.thumb_func
Func_080caa4c:
	.incbin "baserom.gba", 0x000caa4c, 0x00000274
	.section .rom.000cae5c, "ax"
	.global Func_080cae5c
	.type Func_080cae5c, %function
	.thumb_func
Func_080cae5c:
	.incbin "baserom.gba", 0x000cae5c, 0x00000110
	.section .rom.000cb09c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cb09c, 0x0000062c
	.section .rom.000cb91c, "ax"
	.incbin "baserom.gba", 0x000cb91c, 0x00000d58
	.section .rom.000cc674, "ax"
	.global Func_080cc67c
Func_080cc67c:
	.incbin "baserom.gba", 0x000cc674, 0x00000148
	.section .rom.000cc7bc, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000cc7bc, 0x000001d0
	.section .rom.000cc9f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cc9f4, 0x000002bc
	.section .rom.000cccb0, "ax"
	.global BattleAction_FindDescriptor
	.type BattleAction_FindDescriptor, %function
	.thumb_func
BattleAction_FindDescriptor:
	.incbin "baserom.gba", 0x000cccb0, 0x00000090
	.section .rom.000ccd6e, "ax"
	.incbin "baserom.gba", 0x000ccd6e, 0x00000002
	.section .rom.000ccd70, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.global SceneEvent_FindActiveRecord
	.type SceneEvent_FindActiveRecord, %function
	.thumb_func
SceneEvent_FindActiveRecord:
	.incbin "baserom.gba", 0x000ccd70, 0x0000010c
	.section .rom.000cce8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cce8c, 0x000006f0
	.section .rom.000cd5b4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cd5b4, 0x00000360
	.section .rom.000cd914, "ax"
	.global Func_080cd91c
	.type Func_080cd91c, %function
	.thumb_func
Func_080cd91c:
	.incbin "baserom.gba", 0x000cd914, 0x0000013c
	.section .rom.000cdab8, "ax"
	.balign 4
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000cdab8, 0x00000138
	.section .rom.000cdc6a, "ax"
	.incbin "baserom.gba", 0x000cdc6a, 0x00000002
	.section .rom.000cdc6c, "ax"
	.global Func_080cdc74
	.type Func_080cdc74, %function
	.thumb_func
Func_080cdc74:
	.incbin "baserom.gba", 0x000cdc6c, 0x000000f4
	.section .rom.000cdd60, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000cdd60, 0x00000128
	.section .rom.000cdf90, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x000cdf90, 0x0000036c
	.section .rom.000ce2fc, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000ce2fc, 0x0000013c
	.section .rom.000ce438, "ax"
	.global Func_080ce458
	.type Func_080ce458, %function
	.thumb_func
Func_080ce458:
	.incbin "baserom.gba", 0x000ce438, 0x0000011c
	.section .rom.000ce5fc, "ax"
	.incbin "baserom.gba", 0x000ce5fc, 0x000004e0
	.section .rom.000ceb88, "ax"
	.incbin "baserom.gba", 0x000ceb88, 0x0000007c
	.section .rom.000cec14, "ax"
	.incbin "baserom.gba", 0x000cec14, 0x0000020c
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
	.incbin "baserom.gba", 0x000cf15c, 0x000001c4
	.section .rom.000cf330, "ax"
	.balign 4
	.global Func_080cf350
	.type Func_080cf350, %function
	.thumb_func
Func_080cf350:
	.incbin "baserom.gba", 0x000cf330, 0x00000064
	.section .rom.000cf534, "ax"
	.incbin "baserom.gba", 0x000cf534, 0x000001a8
	.section .rom.000cf6dc, "ax"
	.global Func_080cf6fc
	.type Func_080cf6fc, %function
	.thumb_func
Func_080cf6fc:
	.incbin "baserom.gba", 0x000cf6dc, 0x00000090
	.section .rom.000cf76c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000cf76c, 0x0000096c
	.section .rom.000d01ac, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x000d01ac, 0x00000354
	.section .rom.000d0500, "ax"
	.global DisplayTransition_Finish
	.type DisplayTransition_Finish, %function
	.thumb_func
DisplayTransition_Finish:
	.incbin "baserom.gba", 0x000d0500, 0x0000020c
	.section .rom.000d0768, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d0768, 0x000001cc
	.section .rom.000d0934, "ax"
	.global Func_080d0954
	.type Func_080d0954, %function
	.thumb_func
Func_080d0954:
	.incbin "baserom.gba", 0x000d0934, 0x000000d4
	.section .rom.000d0a08, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x000d0a08, 0x00000154
	.section .rom.000d0b5c, "ax"
	.global DisplayTransition_FillTilemapAndSolidTile
	.type DisplayTransition_FillTilemapAndSolidTile, %function
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	.incbin "baserom.gba", 0x000d0b5c, 0x00000070
	.section .rom.000d0c80, "ax"
	.global BattlePalette_UpdateBlend
	.type BattlePalette_UpdateBlend, %function
	.thumb_func
BattlePalette_UpdateBlend:
	.incbin "baserom.gba", 0x000d0c80, 0x0000017c
	.section .rom.000d0dfc, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x000d0dfc, 0x00000868
	.section .rom.000d1740, "ax"
	.balign 4
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d1740, 0x0000004c
	.section .rom.000d181e, "ax"
	.incbin "baserom.gba", 0x000d181e, 0x00000002
	.section .rom.000d1820, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000d1820, 0x000001d8
	.section .rom.000d19f8, "ax"
	.global Func_080d1a18
	.type Func_080d1a18, %function
	.thumb_func
Func_080d1a18:
	.incbin "baserom.gba", 0x000d19f8, 0x000000ac
	.section .rom.000d1ac4, "ax"
	.global Func_080d1ae4
	.type Func_080d1ae4, %function
	.thumb_func
Func_080d1ae4:
	.incbin "baserom.gba", 0x000d1ac4, 0x000001dc
	.section .rom.000d1d8c, "ax"
	.global Func_080d1dac
	.type Func_080d1dac, %function
	.thumb_func
Func_080d1dac:
	.incbin "baserom.gba", 0x000d1d8c, 0x00000044
	.section .rom.000d1df8, "ax"
	.balign 4
	.global Func_080d1e18
	.type Func_080d1e18, %function
	.thumb_func
Func_080d1e18:
	.incbin "baserom.gba", 0x000d1df8, 0x00000048
	.section .rom.000d1e40, "ax"
	.global Func_080d1e60
	.type Func_080d1e60, %function
	.thumb_func
Func_080d1e60:
	.incbin "baserom.gba", 0x000d1e40, 0x00000024
	.section .rom.000d1ec8, "ax"
	.balign 4
	.global Func_080d1ee8
	.type Func_080d1ee8, %function
	.thumb_func
Func_080d1ee8:
	.incbin "baserom.gba", 0x000d1ec8, 0x00000024
	.section .rom.000d1f00, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000d1f00, 0x00000164
	.section .rom.000d20dc, "ax"
	.global Func_080d20fc
	.type Func_080d20fc, %function
	.thumb_func
Func_080d20fc:
	.incbin "baserom.gba", 0x000d20dc, 0x000000f8
	.section .rom.000d25ec, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d25ec, 0x000001e0
	.section .rom.000d2820, "ax"
	.global Inventory_PromptAndSetObjectMode
	.type Inventory_PromptAndSetObjectMode, %function
	.thumb_func
Inventory_PromptAndSetObjectMode:
	.incbin "baserom.gba", 0x000d2820, 0x0000011c
	.section .rom.000d294c, "ax"
	.balign 4
	.global Func_080d296c
	.type Func_080d296c, %function
	.thumb_func
Func_080d296c:
	.incbin "baserom.gba", 0x000d294c, 0x000000a0
	.section .rom.000d2b2c, "ax"
	.global Func_080d2b4c
	.type Func_080d2b4c, %function
	.thumb_func
	.align 2, 0
Func_080d2b4c:
	.incbin "baserom.gba", 0x000d2b2c, 0x000000b8
	.section .rom.000d3248, "ax"
	.global ObjectMotion_WaitForAnimationChange
	.type ObjectMotion_WaitForAnimationChange, %function
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	.incbin "baserom.gba", 0x000d3248, 0x00000050
	.section .rom.000d3356, "ax"
	.incbin "baserom.gba", 0x000d3356, 0x00000002
	.section .rom.000d3358, "ax"
	.incbin "baserom.gba", 0x000d3358, 0x000000e8
	.section .rom.000d3440, "ax"
	.global BattleFx_RunRisingObjectSequence
	.type BattleFx_RunRisingObjectSequence, %function
	.thumb_func
BattleFx_RunRisingObjectSequence:
	.incbin "baserom.gba", 0x000d3440, 0x0000013c
	.section .rom.000d3760, "ax"
	.global Func_080d3780
	.type Func_080d3780, %function
	.thumb_func
Func_080d3780:
	.incbin "baserom.gba", 0x000d3760, 0x00000054
	.section .rom.000d3920, "ax"
	.balign 4
	.global Func_080d3940
	.type Func_080d3940, %function
	.thumb_func
Func_080d3940:
	.incbin "baserom.gba", 0x000d3920, 0x000000d0
	.section .rom.000d39f0, "ax"
	.global Func_080d3a10
	.type Func_080d3a10, %function
	.thumb_func
Func_080d3a10:
	.incbin "baserom.gba", 0x000d39f0, 0x00000118
	.section .rom.000d3b08, "ax"
	.global Func_080d3b28
	.type Func_080d3b28, %function
	.thumb_func
Func_080d3b28:
	.incbin "baserom.gba", 0x000d3b08, 0x000000c0
	.section .rom.000d3c68, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d3c68, 0x00000328
	.section .rom.000d3f90, "ax"
	.global Func_080d3fb0
	.type Func_080d3fb0, %function
	.thumb_func
Func_080d3fb0:
	.incbin "baserom.gba", 0x000d3f90, 0x000000bc
	.section .rom.000d40bc, "ax"
	.balign 4
	.global Func_080d40dc
	.type Func_080d40dc, %function
	.thumb_func
Func_080d40dc:
	.incbin "baserom.gba", 0x000d40bc, 0x0000009c
	.section .rom.000d41c6, "ax"
	.incbin "baserom.gba", 0x000d41c6, 0x00000002
	.section .rom.000d41dc, "ax"
	.global Func_080d41fc
	.type Func_080d41fc, %function
	.thumb_func
Func_080d41fc:
	.incbin "baserom.gba", 0x000d41dc, 0x00000134
	.section .rom.000d4310, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000d4310, 0x00000054
	.section .rom.000d43ec, "ax"
	.balign 4
	.global Motion_CamBounds
	.type Motion_CamBounds, %function
	.thumb_func
Motion_CamBounds:
	.incbin "baserom.gba", 0x000d43ec, 0x00000100
	.section .rom.000d4794, "ax"
	.global Func_080d47b4
	.type Func_080d47b4, %function
	.thumb_func
Func_080d47b4:
	.incbin "baserom.gba", 0x000d4794, 0x000000b4
	.section .rom.000d487c, "ax"
	.balign 4
	.global Func_080d489c
	.type Func_080d489c, %function
	.thumb_func
Func_080d489c:
	.incbin "baserom.gba", 0x000d487c, 0x000000f0
	.section .rom.000d4ba4, "ax"
	.incbin "baserom.gba", 0x000d4ba4, 0x00000108
	.section .rom.000d4ce8, "ax"
	.balign 4
	.global Func_080d4d08
	.type Func_080d4d08, %function
	.thumb_func
Func_080d4d08:
	.incbin "baserom.gba", 0x000d4ce8, 0x000003f0
	.section .rom.000d50d8, "ax"
	.global Func_080d50f8
	.type Func_080d50f8, %function
	.thumb_func
Func_080d50f8:
	.incbin "baserom.gba", 0x000d50d8, 0x000002c0
	.section .rom.000d5398, "ax"
	.global Func_080d53b8
	.type Func_080d53b8, %function
	.thumb_func
Func_080d53b8:
	.incbin "baserom.gba", 0x000d5398, 0x00000834
	.section .rom.000d5bcc, "ax"
	.global Func_080d5bec
	.type Func_080d5bec, %function
	.thumb_func
Func_080d5bec:
	.incbin "baserom.gba", 0x000d5bcc, 0x00000184
	.section .rom.000d5d50, "ax"
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
	.section .rom.000d5e58, "ax"
	.balign 4
	.global ObjectEffect_EndContextEffect
	.type ObjectEffect_EndContextEffect, %function
	.thumb_func
ObjectEffect_EndContextEffect:
	.incbin "baserom.gba", 0x000d5e58, 0x000000a0
	.section .rom.000d605c, "ax"
	.global Func_080d607c
	.type Func_080d607c, %function
	.thumb_func
Func_080d607c:
	.incbin "baserom.gba", 0x000d605c, 0x0000038c
	.section .rom.000d64d0, "ax"
	.incbin "baserom.gba", 0x000d64d0, 0x00000184
	.section .rom.000d6654, "ax"
	.global Func_080d6674
	.type Func_080d6674, %function
	.thumb_func
Func_080d6674:
	.incbin "baserom.gba", 0x000d6654, 0x00000174
	.section .rom.000d67c8, "ax"
	.global Func_080d67e8
	.type Func_080d67e8, %function
	.thumb_func
Func_080d67e8:
	.incbin "baserom.gba", 0x000d67c8, 0x00000124
	.section .rom.000d68ec, "ax"
	.global Func_080d690c
	.type Func_080d690c, %function
	.thumb_func
Func_080d690c:
	.incbin "baserom.gba", 0x000d68ec, 0x00000284
	.section .rom.000d6b70, "ax"
	.global Func_080d6b90
	.type Func_080d6b90, %function
	.thumb_func
Func_080d6b90:
	.incbin "baserom.gba", 0x000d6b70, 0x000002c8
	.section .rom.000d6e38, "ax"
	.global Func_080d6e58
	.type Func_080d6e58, %function
	.thumb_func
Func_080d6e58:
	.incbin "baserom.gba", 0x000d6e38, 0x000000dc
	.section .rom.000d6f14, "ax"
	.global Func_080d6f34
	.type Func_080d6f34, %function
	.thumb_func
Func_080d6f34:
	.incbin "baserom.gba", 0x000d6f14, 0x000000f0
	.section .rom.000d7014, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000d7014, 0x0000020c
	.section .rom.000d7504, "ax"
	.incbin "baserom.gba", 0x000d7504, 0x00000510
	.section .rom.000d7ac0, "ax"
	.incbin "baserom.gba", 0x000d7ac0, 0x00000244
	.section .rom.000d7d4c, "ax"
	.incbin "baserom.gba", 0x000d7d4c, 0x0000097c
	.section .rom.000d86fc, "ax"
	.incbin "baserom.gba", 0x000d86fc, 0x0000023c
	.section .rom.000d89bc, "ax"
	.global Func_080d8a00
	.type Func_080d8a00, %function
	.thumb_func
Func_080d8a00:
	.incbin "baserom.gba", 0x000d89bc, 0x00000210
	.section .rom.000d8bcc, "ax"
	.global Func_080d8c10
	.type Func_080d8c10, %function
	.thumb_func
Func_080d8c10:
	.incbin "baserom.gba", 0x000d8bcc, 0x00000130
	.section .rom.000d8d24, "ax"
	.balign 4
	.global Func_080d8d68
	.type Func_080d8d68, %function
	.thumb_func
Func_080d8d68:
	.incbin "baserom.gba", 0x000d8d24, 0x00000240
	.section .rom.000d8f64, "ax"
	.global Func_080d8fa8
	.type Func_080d8fa8, %function
	.thumb_func
Func_080d8fa8:
	.incbin "baserom.gba", 0x000d8f64, 0x0000015c
	.section .rom.000d90c0, "ax"
	.global Func_080d9104
	.type Func_080d9104, %function
	.thumb_func
Func_080d9104:
	.incbin "baserom.gba", 0x000d90c0, 0x000001a0
	.section .rom.000d9290, "ax"
	.global Func_080d92d4
	.type Func_080d92d4, %function
	.thumb_func
Func_080d92d4:
	.incbin "baserom.gba", 0x000d9290, 0x0000043c
	.section .rom.000d96cc, "ax"
	.global Func_080d9710
	.type Func_080d9710, %function
	.thumb_func
Func_080d9710:
	.incbin "baserom.gba", 0x000d96cc, 0x00000268
	.section .rom.000d9a6c, "ax"
	.balign 4
	.global Func_080d9ab0
	.type Func_080d9ab0, %function
	.thumb_func
Func_080d9ab0:
	.incbin "baserom.gba", 0x000d9a6c, 0x00000020
	.section .rom.000d9b3c, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000d9b3c, 0x000001c0
	.section .rom.000d9cfc, "ax"
	.global Func_080d9d40
	.type Func_080d9d40, %function
	.thumb_func
Func_080d9d40:
	.incbin "baserom.gba", 0x000d9cfc, 0x00000134
	.section .rom.000d9e54, "ax"
	.global Func_080d9e98
	.type Func_080d9e98, %function
	.thumb_func
Func_080d9e98:
	.incbin "baserom.gba", 0x000d9e54, 0x000001c8
	.section .rom.000da01c, "ax"
	.global Func_080da060
	.type Func_080da060, %function
	.thumb_func
Func_080da060:
	.incbin "baserom.gba", 0x000da01c, 0x000008a8
	.section .rom.000da964, "ax"
	.incbin "baserom.gba", 0x000da964, 0x00000500
	.section .rom.000dae88, "ax"
	.global Func_080daecc
	.type Func_080daecc, %function
	.thumb_func
Func_080daecc:
	.incbin "baserom.gba", 0x000dae88, 0x000001e4
	.section .rom.000db06c, "ax"
	.global Func_080db0b0
	.type Func_080db0b0, %function
	.thumb_func
Func_080db0b0:
	.incbin "baserom.gba", 0x000db06c, 0x00000394
	.section .rom.000db400, "ax"
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
	.section .rom.000db962, "ax"
	.incbin "baserom.gba", 0x000db962, 0x00000002
	.section .rom.000dba18, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dba18, 0x000000e4
	.section .rom.000dbb34, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.incbin "baserom.gba", 0x000dbb34, 0x0000008c
	.section .rom.000dbe8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dbe8c, 0x00000174
	.section .rom.000dc250, "ax"
	.global BattleEffect_InitializeSharedScene
	.type BattleEffect_InitializeSharedScene, %function
	.thumb_func
BattleEffect_InitializeSharedScene:
	.incbin "baserom.gba", 0x000dc250, 0x000000e8
	.section .rom.000dc3cc, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x000dc3cc, 0x0000021c
	.section .rom.000dc694, "ax"
	.global Func_080dc6d8
	.type Func_080dc6d8, %function
	.thumb_func
Func_080dc6d8:
	.incbin "baserom.gba", 0x000dc694, 0x000000f4
	.section .rom.000dc7a4, "ax"
	.balign 4
	.global BattleEffect_CleanupSceneObjects
	.type BattleEffect_CleanupSceneObjects, %function
	.thumb_func
BattleEffect_CleanupSceneObjects:
	.incbin "baserom.gba", 0x000dc7a4, 0x0000016c
	.section .rom.000dc934, "ax"
	.balign 4
	.global Func_080dc978
	.type Func_080dc978, %function
	.thumb_func
Func_080dc978:
	.incbin "baserom.gba", 0x000dc934, 0x000000d8
	.section .rom.000dca98, "ax"
	.balign 4
	.global Func_080dcadc
	.type Func_080dcadc, %function
	.thumb_func
Func_080dcadc:
	.incbin "baserom.gba", 0x000dca98, 0x00000068
	.section .rom.000dcb00, "ax"
	.global Func_080dcb44
	.type Func_080dcb44, %function
	.thumb_func
Func_080dcb44:
	.incbin "baserom.gba", 0x000dcb00, 0x00000284
	.section .rom.000dd010, "ax"
	.global Func_080dd054
	.type Func_080dd054, %function
	.thumb_func
Func_080dd054:
	.incbin "baserom.gba", 0x000dd010, 0x000003e8
	.section .rom.000dd4e2, "ax"
	.incbin "baserom.gba", 0x000dd4e2, 0x00000002
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
	.incbin "baserom.gba", 0x000dd624, 0x000000e0
	.section .rom.000dd710, "ax"
	.incbin "baserom.gba", 0x000dd710, 0x000001fc
	.section .rom.000dd90c, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000dd90c, 0x000001ec
	.section .rom.000ddb94, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000ddb94, 0x0000063c
	.section .rom.000de1d8, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000de1d8, 0x00000334
	.section .rom.000de644, "ax"
	.global BattleFx_RunOrbitingParticles
	.type BattleFx_RunOrbitingParticles, %function
	.thumb_func
BattleFx_RunOrbitingParticles:
	.incbin "baserom.gba", 0x000de644, 0x00000190
	.section .rom.000de820, "ax"
	.incbin "baserom.gba", 0x000de820, 0x00000188
	.section .rom.000de9b4, "ax"
	.global Func_080de9f8
	.type Func_080de9f8, %function
	.thumb_func
Func_080de9f8:
	.incbin "baserom.gba", 0x000de9b4, 0x000001ac
	.section .rom.000deb94, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000deb94, 0x000000b4
	.section .rom.000dec74, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000dec74, 0x000000cc
	.section .rom.000deda4, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000deda4, 0x00000ac0
	.section .rom.000df89a, "ax"
	.incbin "baserom.gba", 0x000df89a, 0x00000002
	.section .rom.000df89c, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000df89c, 0x000001d4
	.section .rom.000dfbf8, "ax"
	.incbin "baserom.gba", 0x000dfbf8, 0x00000098
	.section .rom.000dfcbc, "ax"
	.global RunBattleEffect14
RunBattleEffect14:
	.incbin "baserom.gba", 0x000dfcbc, 0x000002fc
	.section .rom.000dffdc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dffdc, 0x000000fc
	.section .rom.000e00f0, "ax"
	.global Func_080e0134
	.type Func_080e0134, %function
	.thumb_func
Func_080e0134:
	.incbin "baserom.gba", 0x000e00f0, 0x000001b8
	.section .rom.000e037e, "ax"
	.incbin "baserom.gba", 0x000e037e, 0x00000002
	.section .rom.000e0388, "ax"
	.balign 4
	.global Func_080e03cc
	.type Func_080e03cc, %function
	.thumb_func
Func_080e03cc:
	.incbin "baserom.gba", 0x000e0388, 0x0000024c
	.section .rom.000e05d4, "ax"
	.incbin "baserom.gba", 0x000e05d4, 0x00000360
	.section .rom.000e097c, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000e097c, 0x000002c4
	.section .rom.000e0c68, "ax"
	.global Func_080e0cac
	.type Func_080e0cac, %function
	.thumb_func
Func_080e0cac:
	.incbin "baserom.gba", 0x000e0c68, 0x00000040
	.section .rom.000e0ca8, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000e0ca8, 0x000000e8
	.section .rom.000e0efc, "ax"
	.incbin "baserom.gba", 0x000e0efc, 0x00000214
	.section .rom.000e1110, "ax"
	.global Func_080e1154
	.type Func_080e1154, %function
	.thumb_func
Func_080e1154:
	.incbin "baserom.gba", 0x000e1110, 0x00000068
	.section .rom.000e1404, "ax"
	.global BattleFx_SpawnScaledArcObjects
	.type BattleFx_SpawnScaledArcObjects, %function
	.thumb_func
BattleFx_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000e1404, 0x000000c4
	.section .rom.000e14c8, "ax"
	.global BattleFx_UpdateArcEffect
	.type BattleFx_UpdateArcEffect, %function
	.thumb_func
BattleFx_UpdateArcEffect:
	.incbin "baserom.gba", 0x000e14c8, 0x000000f0
	.section .rom.000e1634, "ax"
	.incbin "baserom.gba", 0x000e1634, 0x000008ac
	.section .rom.000e1ee8, "ax"
	.global Func_080e1f2c
	.type Func_080e1f2c, %function
	.thumb_func
Func_080e1f2c:
	.incbin "baserom.gba", 0x000e1ee8, 0x000006bc
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
	.incbin "baserom.gba", 0x000e2890, 0x00000364
	.section .rom.000e2bf4, "ax"
	.global Func_080e2c38
	.type Func_080e2c38, %function
	.thumb_func
Func_080e2c38:
	.incbin "baserom.gba", 0x000e2bf4, 0x00000108
	.section .rom.000e2d20, "ax"
	.incbin "baserom.gba", 0x000e2d20, 0x000002fc
	.section .rom.000e3030, "ax"
	.global Func_080e3074
	.type Func_080e3074, %function
	.thumb_func
Func_080e3074:
	.incbin "baserom.gba", 0x000e3030, 0x00000624
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
	.incbin "baserom.gba", 0x000e379c, 0x0000051c
	.section .rom.000e3cc0, "ax"
	.balign 4
	.global Func_080e3d04
	.type Func_080e3d04, %function
	.thumb_func
Func_080e3d04:
	.incbin "baserom.gba", 0x000e3cc0, 0x00000540
	.section .rom.000e4200, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000e4200, 0x00000090
	.section .rom.000e4360, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e4360, 0x0000038c
	.section .rom.000e46ec, "ax"
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000e46ec, 0x000002fc
	.section .rom.000e49f0, "ax"
	.balign 4
	.global Func_080e4a34
	.type Func_080e4a34, %function
	.thumb_func
Func_080e4a34:
	.incbin "baserom.gba", 0x000e49f0, 0x00000cf0
	.section .rom.000e56e8, "ax"
	.balign 4
	.global Func_080e572c
	.type Func_080e572c, %function
	.thumb_func
Func_080e572c:
	.incbin "baserom.gba", 0x000e56e8, 0x000005c4
	.section .rom.000e5d18, "ax"
	.balign 4
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000e5d18, 0x0000062c
	.section .rom.000e63bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e63bc, 0x000004c8
	.section .rom.000e688c, "ax"
	.balign 4
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000e688c, 0x00000828
	.section .rom.000e7128, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e7128, 0x000000cc
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
	.section .rom.000e7800, "ax"
	.balign 4
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000e7800, 0x00000f7c
	.section .rom.000e877c, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e877c, 0x00000548
	.section .rom.000e8cf4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8cf4, 0x000004a0
	.section .rom.000e91f8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e91f8, 0x00000838
	.section .rom.000e9a38, "ax"
	.balign 4
	.global Func_080e9af4
	.type Func_080e9af4, %function
	.thumb_func
Func_080e9af4:
	.incbin "baserom.gba", 0x000e9a38, 0x000002c0
	.section .rom.000e9cf8, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000e9cf8, 0x00000130
	.section .rom.000e9eac, "ax"
	.incbin "baserom.gba", 0x000e9eac, 0x00000940
	.section .rom.000ea818, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ea818, 0x0000029c
	.section .rom.000eaadc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000eaadc, 0x00000390
	.section .rom.000eaedc, "ax"
	.incbin "baserom.gba", 0x000eaedc, 0x00000084
	.section .rom.000eaf60, "ax"
	.global Func_080eb01c
Func_080eb01c:
	.incbin "baserom.gba", 0x000eaf60, 0x0000027c
	.section .rom.000eb25c, "ax"
	.incbin "baserom.gba", 0x000eb25c, 0x00000204
	.section .rom.000eb4d8, "ax"
	.global Func_080eb594
	.type Func_080eb594, %function
	.thumb_func
Func_080eb594:
	.incbin "baserom.gba", 0x000eb4d8, 0x0000010c
	.section .rom.000eb5e4, "ax"
	.global Func_080eb6a0
	.type Func_080eb6a0, %function
	.thumb_func
Func_080eb6a0:
	.incbin "baserom.gba", 0x000eb5e4, 0x00000184
	.section .rom.000eb768, "ax"
	.incbin "baserom.gba", 0x000eb768, 0x0000010c
	.section .rom.000eb8a4, "ax"
	.incbin "baserom.gba", 0x000eb8a4, 0x000002d0
	.section .rom.000ebc68, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x000ebc68, 0x0000014c
	.section .rom.000ebe0c, "ax"
	.global EffectSlot_Initialize
	.type EffectSlot_Initialize, %function
	.thumb_func
EffectSlot_Initialize:
	.incbin "baserom.gba", 0x000ebe0c, 0x000000a0
	.section .rom.000ebed8, "ax"
	.incbin "baserom.gba", 0x000ebed8, 0x000001b8
	.section .rom.000ec114, "ax"
	.incbin "baserom.gba", 0x000ec114, 0x000000cc
	.section .rom.000ec200, "ax"
	.incbin "baserom.gba", 0x000ec200, 0x00001494
	.section .rom.000ed918, "ax"
	.incbin "baserom.gba", 0x000ed918, 0x000000c0
	.section .rom.000ef728, "ax"
	.incbin "baserom.gba", 0x000ef728, 0x00000008
	.section .rom.000f00c4, "ax"
	.incbin "baserom.gba", 0x000f00c4, 0x0000000c
	.section .rom.000f0212, "ax"
	.incbin "baserom.gba", 0x000f0212, 0x00000002
	.section .rom.000f0a5c, "ax"
	.incbin "baserom.gba", 0x000f0a5c, 0x0000002c
	.section .rom.000f0b08, "ax"
	.global Data_080f0bfc
Data_080f0bfc:
	.incbin "baserom.gba", 0x000f0b08, 0x00000008
	.section .rom.000f0b10, "ax"
	.global Data_080f0c04
Data_080f0c04:
	.incbin "baserom.gba", 0x000f0b10, 0x00000250
	.section .rom.000f0d60, "ax"
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000f0d60, 0x00000024
	.section .rom.000f0d84, "ax"
	.global Data_080f0e78
Data_080f0e78:
	.incbin "baserom.gba", 0x000f0d84, 0x00000024
	.section .rom.000f0da8, "ax"
	.global Data_080f0e9c
Data_080f0e9c:
	.incbin "baserom.gba", 0x000f0da8, 0x00000008
	.section .rom.000f0dd0, "ax"
	.global BattleFx_EightWayHeadingWords
BattleFx_EightWayHeadingWords:
	.incbin "baserom.gba", 0x000f0dd0, 0x00000024
	.section .rom.000f0df4, "ax"
	.global Data_080f0ee8
Data_080f0ee8:
	.incbin "baserom.gba", 0x000f0df4, 0x00000044
	.section .rom.000f0e38, "ax"
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000f0e38, 0x000001d4
	.section .rom.000f102c, "ax"
	.incbin "baserom.gba", 0x000f102c, 0x00000020
	.section .rom.000f104c, "ax"
	.global Data_080f1140
Data_080f1140:
	.incbin "baserom.gba", 0x000f104c, 0x00000668
	.section .rom.000f20dc, "ax"
	.incbin "baserom.gba", 0x000f20dc, 0x00000034
	.section .rom.000f3128, "ax"
	.incbin "baserom.gba", 0x000f3128, 0x000000b4
	.section .rom.000f31dc, "ax"
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f31dc, 0x00000010
	.section .rom.000f31ec, "ax"
	.global Data_080f32e0
Data_080f32e0:
	.incbin "baserom.gba", 0x000f31ec, 0x00000020
	.section .rom.000f320c, "ax"
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000f320c, 0x00000128
	.section .rom.000f3334, "ax"
	.global Data_080f3428
Data_080f3428:
	.incbin "baserom.gba", 0x000f3334, 0x000000bc
	.section .rom.000f33f0, "ax"
	.global Data_080f34e4
Data_080f34e4:
	.incbin "baserom.gba", 0x000f33f0, 0x00000004
	.section .rom.000f33f4, "ax"
	.global Data_080f34e8
Data_080f34e8:
	.incbin "baserom.gba", 0x000f33f4, 0x0000000c
	.section .rom.000f3400, "ax"
	.global Data_080f34f4
Data_080f34f4:
	.incbin "baserom.gba", 0x000f3400, 0x0000000c
	.section .rom.000f340c, "ax"
	.global Data_080f3500
Data_080f3500:
	.incbin "baserom.gba", 0x000f340c, 0x0000000c
	.section .rom.000f3418, "ax"
	.global Data_080f350c
Data_080f350c:
	.incbin "baserom.gba", 0x000f3418, 0x000000bc
	.section .rom.000f34d4, "ax"
	.global Data_080f35c8
Data_080f35c8:
	.incbin "baserom.gba", 0x000f34d4, 0x0000004c
	.section .rom.000f3520, "ax"
	.global Data_080f3614
Data_080f3614:
	.incbin "baserom.gba", 0x000f3520, 0x00000128
	.section .rom.000f3648, "ax"
	.global Data_080f373c
Data_080f373c:
	.incbin "baserom.gba", 0x000f3648, 0x00000014
	.section .rom.000f365c, "ax"
	.global Data_080f3750
Data_080f3750:
	.incbin "baserom.gba", 0x000f365c, 0x00000014
	.section .rom.000f3670, "ax"
	.global Data_080f3764
Data_080f3764:
	.incbin "baserom.gba", 0x000f3670, 0x0000001c
	.section .rom.000f368c, "ax"
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f368c, 0x00000018
	.section .rom.000f376a, "ax"
	.incbin "baserom.gba", 0x000f376a, 0x00000054
	.section .rom.000f380d, "ax"
	.incbin "baserom.gba", 0x000f380d, 0x00000043
	.section .rom.000f3850, "ax"
	.global Data_080f3944
Data_080f3944:
	.incbin "baserom.gba", 0x000f3850, 0x0000000c
	.section .rom.000f385c, "ax"
	.global Data_080f3950
Data_080f3950:
	.incbin "baserom.gba", 0x000f385c, 0x00000024
	.section .rom.000f3880, "ax"
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000f3880, 0x00000050
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x00000498
	.section .rom.000f8830, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8830, 0x0000003c
	.section .rom.000f8878, "ax"
	.global Func_080f88d0
	.type Func_080f88d0, %function
	.thumb_func
Func_080f88d0:
	.incbin "baserom.gba", 0x000f8878, 0x000001e4
	.section .rom.000f8a5c, "ax"
	.global Func_080f8ab4
	.type Func_080f8ab4, %function
	.thumb_func
Func_080f8ab4:
	.incbin "baserom.gba", 0x000f8a5c, 0x00000108
	.section .rom.000f8c8e, "ax"
	.incbin "baserom.gba", 0x000f8c8e, 0x00000002
	.section .rom.000f8c90, "ax"
	.global Func_080f8ce8
	.type Func_080f8ce8, %function
	.thumb_func
Func_080f8ce8:
	.incbin "baserom.gba", 0x000f8c90, 0x00000228
	.section .rom.000f8f14, "ax"
	.incbin "baserom.gba", 0x000f8f14, 0x0000016c
	.section .rom.000f9254, "ax"
	.incbin "baserom.gba", 0x000f9254, 0x00000098
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
	.section .rom.000fa4b8, "ax"
	.global Func_080fa5a4
	.type Func_080fa5a4, %function
	.thumb_func
Func_080fa5a4:
	.incbin "baserom.gba", 0x000fa4b8, 0x000002cc
	.section .rom.000fa7a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fa7a8, 0x00000370
	.section .rom.000fab90, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fab90, 0x00000048
	.section .rom.000fac0e, "ax"
	.incbin "baserom.gba", 0x000fac0e, 0x00000002
	.section .rom.000fac10, "ax"
	.global Menu_HideEmptyEntryIcons
	.type Menu_HideEmptyEntryIcons, %function
	.thumb_func
Menu_HideEmptyEntryIcons:
	.incbin "baserom.gba", 0x000fac10, 0x00000044
	.section .rom.000fadc4, "ax"
	.incbin "baserom.gba", 0x000fadc4, 0x00000218
	.section .rom.000fb03c, "ax"
	.incbin "baserom.gba", 0x000fb03c, 0x00000534
	.section .rom.000fb6b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fb6b8, 0x00000114
	.section .rom.000fb7d8, "ax"
	.global Func_080fb8b8
	.type Func_080fb8b8, %function
	.thumb_func
Func_080fb8b8:
	.incbin "baserom.gba", 0x000fb7d8, 0x00000500
	.section .rom.000fbe10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fbe10, 0x00000634
	.section .rom.000fc51c, "ax"
	.incbin "baserom.gba", 0x000fc51c, 0x00000560
	.section .rom.000fca7c, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x000fca7c, 0x000007d8
	.section .rom.000fd2a8, "ax"
	.incbin "baserom.gba", 0x000fd2a8, 0x00000244
	.section .rom.000fd51c, "ax"
	.incbin "baserom.gba", 0x000fd51c, 0x000001c8
	.section .rom.000fd724, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x000fd724, 0x00000a74
	.section .rom.000fe2a8, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x000fe2a8, 0x0000030c
	.section .rom.000fe5b4, "ax"
	.global Func_080fe580
	.type Func_080fe580, %function
	.thumb_func
Func_080fe580:
	.incbin "baserom.gba", 0x000fe5b4, 0x00000884
	.section .rom.000fee74, "ax"
	.incbin "baserom.gba", 0x000fee74, 0x000003b4
	.section .rom.000ff3a4, "ax"
	.incbin "baserom.gba", 0x000ff3a4, 0x00000570
	.section .rom.000ff980, "ax"
	.incbin "baserom.gba", 0x000ff980, 0x00000b40
	.section .rom.00100548, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x00100548, 0x000000e4
	.section .rom.00100740, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00100740, 0x00000608
	.section .rom.00100e84, "ax"
	.global Func_08100e7c
	.type Func_08100e7c, %function
	.thumb_func
Func_08100e7c:
	.incbin "baserom.gba", 0x00100e84, 0x00000734
	.section .rom.00101640, "ax"
	.incbin "baserom.gba", 0x00101640, 0x00000490
	.section .rom.00101ad0, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00101ad0, 0x00000144
	.section .rom.00101c48, "ax"
	.incbin "baserom.gba", 0x00101c48, 0x0000003c
	.section .rom.00101c84, "ax"
	.global Func_08101c7c
	.type Func_08101c7c, %function
	.thumb_func
Func_08101c7c:
	.incbin "baserom.gba", 0x00101c84, 0x000000b8
	.section .rom.00101d64, "ax"
	.incbin "baserom.gba", 0x00101d64, 0x00002ccc
	.section .rom.00104b2e, "ax"
	.incbin "baserom.gba", 0x00104b2e, 0x00000002
	.section .rom.00104b30, "ax"
	.global UiIcon_CreateStatChangeArrow
	.type UiIcon_CreateStatChangeArrow, %function
	.thumb_func
UiIcon_CreateStatChangeArrow:
	.incbin "baserom.gba", 0x00104b30, 0x00000174
	.section .rom.00104d34, "ax"
	.incbin "baserom.gba", 0x00104d34, 0x0000004c
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
	.section .rom.001050a4, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x001050a4, 0x000000dc
	.section .rom.00105180, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x00105180, 0x000000c4
	.section .rom.00105380, "ax"
	.incbin "baserom.gba", 0x00105380, 0x000000c0
	.section .rom.001054b8, "ax"
	.incbin "baserom.gba", 0x001054b8, 0x000000a0
	.section .rom.001055ce, "ax"
	.incbin "baserom.gba", 0x001055ce, 0x00000346
	.section .rom.00105914, "ax"
	.global Ui_HpRowString
Ui_HpRowString:
	.incbin "baserom.gba", 0x00105914, 0x00000008
	.section .rom.0010591c, "ax"
	.global Ui_EpRowString
Ui_EpRowString:
	.incbin "baserom.gba", 0x0010591c, 0x00000008
	.section .rom.00105924, "ax"
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
	.incbin "baserom.gba", 0x00105958, 0x00000024
	.section .rom.001059ac, "ax"
	.incbin "baserom.gba", 0x001059ac, 0x0000003a
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
	.incbin "baserom.gba", 0x00105a30, 0x00000008
	.section .rom.00105a38, "ax"
	.global Data_08105a40
Data_08105a40:
	.incbin "baserom.gba", 0x00105a38, 0x0000001c
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
	.incbin "baserom.gba", 0x00108b70, 0x00000600
	.section .rom.00109170, "ax"
	.global Func_08109188
	.type Func_08109188, %function
	.thumb_func
Func_08109188:
	.incbin "baserom.gba", 0x00109170, 0x000000f8
	.section .rom.0010928a, "ax"
	.incbin "baserom.gba", 0x0010928a, 0x0000039a
	.section .rom.001096f8, "ax"
	.global Func_081096f8
	.type Func_081096f8, %function
	.thumb_func
Func_081096f8:
	.incbin "baserom.gba", 0x001096f8, 0x000001d8
	.section .rom.00109ae8, "ax"
	.global Func_08109ad8
	.type Func_08109ad8, %function
	.thumb_func
Func_08109ad8:
	.incbin "baserom.gba", 0x00109ae8, 0x000001d4
	.section .rom.00109cbc, "ax"
	.global Func_08109cac
	.type Func_08109cac, %function
	.thumb_func
Func_08109cac:
	.incbin "baserom.gba", 0x00109cbc, 0x00000350
	.section .rom.0010a00c, "ax"
	.global Shop_DrawItemCount
	.type Shop_DrawItemCount, %function
	.thumb_func
Shop_DrawItemCount:
	.incbin "baserom.gba", 0x0010a00c, 0x00000104
	.section .rom.0010a110, "ax"
	.global Func_0810a108
	.type Func_0810a108, %function
	.thumb_func
Func_0810a108:
	.incbin "baserom.gba", 0x0010a110, 0x000005f8
	.section .rom.0010a71c, "ax"
	.global Forge_FindRecipe
	.type Forge_FindRecipe, %function
	.thumb_func
Forge_FindRecipe:
	.incbin "baserom.gba", 0x0010a71c, 0x0000003c
	.section .rom.0010a8fc, "ax"
	.incbin "baserom.gba", 0x0010a8fc, 0x00000074
	.section .rom.0010a9bc, "ax"
	.incbin "baserom.gba", 0x0010a9bc, 0x000000d4
	.section .rom.0010aa90, "ax"
	.global Func_0810aa80
	.type Func_0810aa80, %function
	.thumb_func
Func_0810aa80:
	.incbin "baserom.gba", 0x0010aa90, 0x00000424
	.section .rom.0010af34, "ax"
	.incbin "baserom.gba", 0x0010af34, 0x00000198
	.section .rom.0010b0cc, "ax"
	.global BattleFx_UpdateRadialMotion
BattleFx_UpdateRadialMotion:
	.incbin "baserom.gba", 0x0010b0cc, 0x000000ac
	.section .rom.0010b1c4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010b1c4, 0x000001c4
	.section .rom.0010b388, "ax"
	.global Func_0810b378
	.type Func_0810b378, %function
	.thumb_func
Func_0810b378:
	.incbin "baserom.gba", 0x0010b388, 0x00000074
	.section .rom.0010b428, "ax"
	.balign 4
	.global Func_0810b418
	.type Func_0810b418, %function
	.thumb_func
Func_0810b418:
	.incbin "baserom.gba", 0x0010b428, 0x00000108
	.section .rom.0010b5d4, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x0010b5d4, 0x000000f8
	.section .rom.0010b6dc, "ax"
	.incbin "baserom.gba", 0x0010b6dc, 0x000000d0
	.section .rom.0010b7c4, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x0010b7c4, 0x00000600
	.section .rom.0010be08, "ax"
	.incbin "baserom.gba", 0x0010be08, 0x00000048
	.section .rom.0010bed0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010bed0, 0x000000dc
	.section .rom.0010c01c, "ax"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x0010c01c, 0x00000140
	.section .rom.0010c15c, "ax"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x0010c15c, 0x00000200
	.section .rom.0010c398, "ax"
	.global Shop_GlyphRowOffsets
Shop_GlyphRowOffsets:
	.incbin "baserom.gba", 0x0010c398, 0x0000000a
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
	.incbin "baserom.gba", 0x00118078, 0x00000008
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
	.section .rom.001195ec, "ax"
	.incbin "baserom.gba", 0x001195ec, 0x00000148
	.section .rom.001197f0, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x001197f0, 0x00000844
	.section .rom.0011a248, "ax"
	.incbin "baserom.gba", 0x0011a248, 0x000000d0
	.section .rom.0011a398, "ax"
	.global Func_0811a39c
	.type Func_0811a39c, %function
	.thumb_func
Func_0811a39c:
	.incbin "baserom.gba", 0x0011a398, 0x000000b0
	.section .rom.0011a4da, "ax"
	.incbin "baserom.gba", 0x0011a4da, 0x0000011e
	.section .rom.0011a670, "ax"
	.incbin "baserom.gba", 0x0011a670, 0x00000138
	.section .rom.0011a7a8, "ax"
	.global BtlObjMake
	.type BtlObjMake, %function
	.thumb_func
BtlObjMake:
	.incbin "baserom.gba", 0x0011a7a8, 0x000009c0
	.section .rom.0011b17c, "ax"
	.incbin "baserom.gba", 0x0011b17c, 0x00000114
	.section .rom.0011b2c0, "ax"
	.global EnmUpd
	.type EnmUpd, %function
	.thumb_func
EnmUpd:
	.incbin "baserom.gba", 0x0011b2c0, 0x000001e8
	.section .rom.0011b4d4, "ax"
	.balign 4
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
	.section .rom.0011b9f8, "ax"
	.incbin "baserom.gba", 0x0011b9f8, 0x00000230
	.section .rom.0011bce8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bce8, 0x00000024
	.section .rom.0011bd4c, "ax"
	.global Func_0811bd50
	.type Func_0811bd50, %function
	.thumb_func
Func_0811bd50:
	.incbin "baserom.gba", 0x0011bd4c, 0x00000060
	.section .rom.0011bf14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bf14, 0x00000088
	.section .rom.0011c070, "ax"
	.balign 4
	.global Func_0811c074
	.type Func_0811c074, %function
	.thumb_func
Func_0811c074:
	.incbin "baserom.gba", 0x0011c070, 0x000000ac
	.section .rom.0011c11c, "ax"
	.incbin "baserom.gba", 0x0011c11c, 0x000000d8
	.section .rom.0011c3b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011c3b8, 0x000001d8
	.section .rom.0011c666, "ax"
	.incbin "baserom.gba", 0x0011c666, 0x00000002
	.section .rom.0011c70c, "ax"
	.global Func_0811c710
	.type Func_0811c710, %function
	.thumb_func
Func_0811c710:
	.incbin "baserom.gba", 0x0011c70c, 0x00000344
	.section .rom.0011ca50, "ax"
	.global Func_0811ca54
	.type Func_0811ca54, %function
	.thumb_func
Func_0811ca54:
	.incbin "baserom.gba", 0x0011ca50, 0x00000194
	.section .rom.0011cd78, "ax"
	.balign 4
	.global Camera_ConfigureScene
	.type Camera_ConfigureScene, %function
	.thumb_func
Camera_ConfigureScene:
	.incbin "baserom.gba", 0x0011cd78, 0x000000d4
	.section .rom.0011ce90, "ax"
	.global BattleEscape_CheckSuccess
	.type BattleEscape_CheckSuccess, %function
	.thumb_func
BattleEscape_CheckSuccess:
	.incbin "baserom.gba", 0x0011ce90, 0x00000110
	.section .rom.0011cfcc, "ax"
	.balign 4
	.global BattlePres_BuildOpponentEntries
	.type BattlePres_BuildOpponentEntries, %function
	.thumb_func
BattlePres_BuildOpponentEntries:
	.incbin "baserom.gba", 0x0011cfcc, 0x00000190
	.section .rom.0011d248, "ax"
	.incbin "baserom.gba", 0x0011d248, 0x000001c8
	.section .rom.0011d410, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x0011d410, 0x0000018c
	.section .rom.0011d9c8, "ax"
	.incbin "baserom.gba", 0x0011d9c8, 0x000009a0
	.section .rom.0011e3a6, "ax"
	.incbin "baserom.gba", 0x0011e3a6, 0x00000432
	.section .rom.0011e82c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011e82c, 0x00000800
	.section .rom.0011f02c, "ax"
	.global BattleMotion_SetRecordChildValues
	.type BattleMotion_SetRecordChildValues, %function
	.thumb_func
BattleMotion_SetRecordChildValues:
	.incbin "baserom.gba", 0x0011f02c, 0x00000300
	.section .rom.0011f4d0, "ax"
	.global Func_0811f4d4
	.type Func_0811f4d4, %function
	.thumb_func
Func_0811f4d4:
	.incbin "baserom.gba", 0x0011f4d0, 0x00000968
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
	.incbin "baserom.gba", 0x0012005c, 0x000000f8
	.section .rom.001201c0, "ax"
	.global BattleEv_DispatchQueued
	.type BattleEv_DispatchQueued, %function
	.thumb_func
BattleEv_DispatchQueued:
	.incbin "baserom.gba", 0x001201c0, 0x0000019c
	.section .rom.00120450, "ax"
	.global BtlTgtExec
	.type BtlTgtExec, %function
	.thumb_func
BtlTgtExec:
	.incbin "baserom.gba", 0x00120450, 0x0000206c
	.section .rom.001224d4, "ax"
	.incbin "baserom.gba", 0x001224d4, 0x00000740
	.section .rom.00122ccc, "ax"
	.incbin "baserom.gba", 0x00122ccc, 0x00000040
	.section .rom.00122d0c, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x00122d0c, 0x00000794
	.section .rom.00123570, "ax"
	.global Func_08123574
	.type Func_08123574, %function
	.thumb_func
Func_08123574:
	.incbin "baserom.gba", 0x00123570, 0x0000129c
	.section .rom.00124cc0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00124cc0, 0x00000eb4
	.section .rom.00125bb4, "ax"
	.incbin "baserom.gba", 0x00125bb4, 0x000000a4
	.section .rom.00125cf8, "ax"
	.incbin "baserom.gba", 0x00125cf8, 0x0000006c
	.section .rom.00125d70, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00125d70, 0x000004a4
	.section .rom.00126288, "ax"
	.global Func_0812628c
	.type Func_0812628c, %function
	.thumb_func
Func_0812628c:
	.incbin "baserom.gba", 0x00126288, 0x00000138
	.section .rom.001263f8, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x00126974, 0x00000044
	.section .rom.001269f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001269f4, 0x000000ec
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
	.incbin "baserom.gba", 0x00126cf8, 0x00000104
	.section .rom.00126dfc, "ax"
	.global Func_08126e00
	.type Func_08126e00, %function
	.thumb_func
Func_08126e00:
	.incbin "baserom.gba", 0x00126dfc, 0x00000238
	.section .rom.00127064, "ax"
	.incbin "baserom.gba", 0x00127064, 0x00000248
	.section .rom.00127304, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00127304, 0x00000264
	.section .rom.00127584, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00127584, 0x00000554
	.section .rom.00127ad8, "ax"
	.global EnmCntDec
	.type EnmCntDec, %function
	.thumb_func
EnmCntDec:
	.incbin "baserom.gba", 0x00127ad8, 0x000000c4
	.section .rom.00127c7a, "ax"
	.incbin "baserom.gba", 0x00127c7a, 0x00000002
	.section .rom.00127cd0, "ax"
	.incbin "baserom.gba", 0x00127cd0, 0x000003cc
	.section .rom.001281e6, "ax"
	.incbin "baserom.gba", 0x001281e6, 0x0000065a
	.section .rom.00128840, "ax"
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x00128840, 0x00000024
	.section .rom.00128894, "ax"
	.incbin "baserom.gba", 0x00128894, 0x000000da
	.section .rom.0012ce50, "ax"
	.incbin "baserom.gba", 0x0012ce50, 0x00000028
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
	.thumb_func
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
	.incbin "baserom.gba", 0x00173aac, 0x00007ec8
	.section .rom.0017b9bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0017b9bc, 0x000026c0
	.section .rom.0017e0c4, "ax"
	.balign 4
	.global Func_0817e0c0
	.thumb_func
Func_0817e0c0:
	.incbin "baserom.gba", 0x0017e0c4, 0x000005a8
	.section .rom.0017e69c, "ax"
	.balign 4
	.global Func_0817e698
	.thumb_func
Func_0817e698:
	.incbin "baserom.gba", 0x0017e69c, 0x0000037c
	.section .rom.0017ea5c, "ax"
	.incbin "baserom.gba", 0x0017ea5c, 0x00003458
	.section .rom.00181ed8, "ax"
	.balign 4
	.global Func_08181ed4
Func_08181ed4:
	.incbin "baserom.gba", 0x00181ed8, 0x000004bc
	.section .rom.001823ac, "ax"
	.balign 4
	.global Func_081823a8
Func_081823a8:
	.incbin "baserom.gba", 0x001823ac, 0x000004a4
	.section .rom.0018289c, "ax"
	.balign 4
	.global Func_08182898
	.type Func_08182898, %function
	.thumb_func
Func_08182898:
	.incbin "baserom.gba", 0x0018289c, 0x00007958
	.section .rom.0018a224, "ax"
	.balign 4
	.global Func_0818a220
Func_0818a220:
	.incbin "baserom.gba", 0x0018a224, 0x00000af0
	.section .rom.0018ad44, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad44, 0x00000f20
	.section .rom.0018bc9c, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc9c, 0x00000b08
	.section .rom.0018c7bc, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7bc, 0x00000dd8
	.section .rom.0018d5ac, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5ac, 0x00002024
	.section .rom.0018f624, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f624, 0x000024c0
	.section .rom.00191cc8, "ax"
	.incbin "baserom.gba", 0x00191cc8, 0x00000b64
	.section .rom.00192850, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00192850, 0x00003ba0
	.section .rom.00196408, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196408, 0x00000554
	.section .rom.00196a80, "ax"
	.incbin "baserom.gba", 0x00196a80, 0x00000560
	.section .rom.001973f4, "ax"
	.incbin "baserom.gba", 0x001973f4, 0x0000025c
	.section .rom.00197694, "ax"
	.incbin "baserom.gba", 0x00197694, 0x000001a4
	.section .rom.00197868, "ax"
	.incbin "baserom.gba", 0x00197868, 0x00000d02
	.section .rom.001985aa, "ax"
	.incbin "baserom.gba", 0x001985aa, 0x00001c02
	.section .rom.001a052c, "ax"
	.incbin "baserom.gba", 0x001a052c, 0x00000188
	.section .rom.001a06e0, "ax"
	.global Func_081a06e0
	.type Func_081a06e0, %function
	.thumb_func
Func_081a06e0:
	.incbin "baserom.gba", 0x001a06e0, 0x0000030c
	.section .rom.001a0a5c, "ax"
	.incbin "baserom.gba", 0x001a0a5c, 0x00000178
	.section .rom.001a0bd4, "ax"
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0bd4, 0x000000c4
	.section .rom.001a0d64, "ax"
	.global Func_081a0d78
	.type Func_081a0d78, %function
	.thumb_func
Func_081a0d78:
	.incbin "baserom.gba", 0x001a0d64, 0x00000240
	.section .rom.001a1008, "ax"
	.global DisplayScroll_DrawLine
	.type DisplayScroll_DrawLine, %function
	.thumb_func
DisplayScroll_DrawLine:
	.incbin "baserom.gba", 0x001a1008, 0x0000026c
	.section .rom.001a1280, "ax"
	.incbin "baserom.gba", 0x001a1280, 0x00000278
	.section .rom.001a14f8, "ax"
	.global Func_081a150c
	.type Func_081a150c, %function
	.thumb_func
Func_081a150c:
	.incbin "baserom.gba", 0x001a14f8, 0x000000d0
	.section .rom.001a15c8, "ax"
	.global Func_081a15dc
	.type Func_081a15dc, %function
	.thumb_func
Func_081a15dc:
	.incbin "baserom.gba", 0x001a15c8, 0x000001f4
	.section .rom.001a181c, "ax"
	.incbin "baserom.gba", 0x001a181c, 0x000000c8
	.section .rom.001a1914, "ax"
	.incbin "baserom.gba", 0x001a1914, 0x00000940
	.section .rom.001a2254, "ax"
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x001a2254, 0x00000398
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
	.incbin "baserom.gba", 0x001a8286, 0x000004b8
	.section .rom.001a883a, "ax"
	.incbin "baserom.gba", 0x001a883a, 0x00000002
	.section .rom.001ac026, "ax"
	.incbin "baserom.gba", 0x001ac026, 0x0000008a
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x00001234
	.section .rom.001b2008, "ax"
	.global Func_081b2008
	.type Func_081b2008, %function
	.thumb_func
Func_081b2008:
	.incbin "baserom.gba", 0x001b2008, 0x00000034
	.section .rom.001b20a0, "ax"
	.incbin "baserom.gba", 0x001b20a0, 0x000000b0
	.section .rom.001b21ec, "ax"
	.incbin "baserom.gba", 0x001b21ec, 0x00001b00
	.section .rom.001b3d90, "ax"
	.incbin "baserom.gba", 0x001b3d90, 0x000009f8
	.section .rom.001b8020, "ax"
	.incbin "baserom.gba", 0x001b8020, 0x00000088
	.section .rom.001b810c, "ax"
	.incbin "baserom.gba", 0x001b810c, 0x000002b8
	.section .rom.001b83c4, "ax"
	.global Func_081b83c4
	.type Func_081b83c4, %function
	.thumb_func
Func_081b83c4:
	.incbin "baserom.gba", 0x001b83c4, 0x00001f5c
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
	.section .rom.0069876a, "ax"
	.incbin "baserom.gba", 0x0069876a, 0x00000002
	.section .rom.006a5301, "ax"
	.incbin "baserom.gba", 0x006a5301, 0x00000003
	.section .rom.006aa7f5, "ax"
	.incbin "baserom.gba", 0x006aa7f5, 0x00000003
	.section .rom.006b1e13, "ax"
	.incbin "baserom.gba", 0x006b1e13, 0x00000001
	.section .rom.006ce054, "ax"
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
	.section .rom.0087c384, "ax"
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c384, 0x0000024c
	.section .rom.0087c5d0, "ax"
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c5d0, 0x00000198
	.section .rom.0087cf91, "ax"
	.incbin "baserom.gba", 0x0087cf91, 0x00000003
	.section .rom.0087e463, "ax"
	.incbin "baserom.gba", 0x0087e463, 0x00000001
	.section .rom.008805cd, "ax"
	.incbin "baserom.gba", 0x008805cd, 0x00000003
	.section .rom.00880923, "ax"
	.incbin "baserom.gba", 0x00880923, 0x00000001
	.section .rom.00880aa7, "ax"
	.incbin "baserom.gba", 0x00880aa7, 0x00000001
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
	.section .rom.0088562d, "ax"
	.incbin "baserom.gba", 0x0088562d, 0x00000003
	.section .rom.008866fa, "ax"
	.incbin "baserom.gba", 0x008866fa, 0x00000002
	.section .rom.008866fc, "ax"
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x008866fc, 0x000002e8
	.section .rom.00886b36, "ax"
	.incbin "baserom.gba", 0x00886b36, 0x00000002
	.section .rom.00889043, "ax"
	.incbin "baserom.gba", 0x00889043, 0x00000001
	.section .rom.008899ff, "ax"
	.incbin "baserom.gba", 0x008899ff, 0x00000001
	.section .rom.0088ab42, "ax"
	.incbin "baserom.gba", 0x0088ab42, 0x00000002
	.section .rom.0088b7c0, "ax"
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b7c0, 0x0000002c
	.section .rom.0088b7ec, "ax"
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b7ec, 0x0000002c
	.section .rom.0088bfd5, "ax"
	.incbin "baserom.gba", 0x0088bfd5, 0x00000003
	.section .rom.0088d553, "ax"
	.incbin "baserom.gba", 0x0088d553, 0x00000001
	.section .rom.0088d87d, "ax"
	.incbin "baserom.gba", 0x0088d87d, 0x00000003
	.section .rom.0088dda6, "ax"
	.incbin "baserom.gba", 0x0088dda6, 0x00000002
	.section .rom.0088e2d6, "ax"
	.incbin "baserom.gba", 0x0088e2d6, 0x00000002
	.section .rom.0088f051, "ax"
	.incbin "baserom.gba", 0x0088f051, 0x00000003
	.section .rom.0088f671, "ax"
	.incbin "baserom.gba", 0x0088f671, 0x00000003
	.section .rom.0088fc9d, "ax"
	.incbin "baserom.gba", 0x0088fc9d, 0x00000003
	.section .rom.008900a7, "ax"
	.incbin "baserom.gba", 0x008900a7, 0x00000001
	.section .rom.00890301, "ax"
	.incbin "baserom.gba", 0x00890301, 0x00000003
	.section .rom.0089087f, "ax"
	.incbin "baserom.gba", 0x0089087f, 0x00000001
	.section .rom.00890b29, "ax"
	.incbin "baserom.gba", 0x00890b29, 0x00000003
	.section .rom.00891b45, "ax"
	.incbin "baserom.gba", 0x00891b45, 0x00000003
	.section .rom.008945f7, "ax"
	.incbin "baserom.gba", 0x008945f7, 0x00000001
	.section .rom.008957c3, "ax"
	.incbin "baserom.gba", 0x008957c3, 0x00000001
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
	.section .rom.0089b2b6, "ax"
	.incbin "baserom.gba", 0x0089b2b6, 0x00000002
	.section .rom.0089bff1, "ax"
	.incbin "baserom.gba", 0x0089bff1, 0x00000003
	.section .rom.0089d22d, "ax"
	.incbin "baserom.gba", 0x0089d22d, 0x00000003
	.section .rom.0089db77, "ax"
	.incbin "baserom.gba", 0x0089db77, 0x00000001
	.section .rom.0089e1d2, "ax"
	.incbin "baserom.gba", 0x0089e1d2, 0x00000002
	.section .rom.0089e6fe, "ax"
	.incbin "baserom.gba", 0x0089e6fe, 0x00000002
	.section .rom.008a09b9, "ax"
	.incbin "baserom.gba", 0x008a09b9, 0x00000003
	.section .rom.008a214d, "ax"
	.incbin "baserom.gba", 0x008a214d, 0x00000003
	.section .rom.008a2831, "ax"
	.incbin "baserom.gba", 0x008a2831, 0x00000003
	.section .rom.008a2834, "ax"
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a2834, 0x00001f4c
	.section .rom.008a5e89, "ax"
	.incbin "baserom.gba", 0x008a5e89, 0x00000003
	.section .rom.008a637a, "ax"
	.incbin "baserom.gba", 0x008a637a, 0x00000002
	.section .rom.008a69c2, "ax"
	.incbin "baserom.gba", 0x008a69c2, 0x00000002
	.section .rom.008a75e7, "ax"
	.incbin "baserom.gba", 0x008a75e7, 0x00000001
	.section .rom.008a79a9, "ax"
	.incbin "baserom.gba", 0x008a79a9, 0x00000003
	.section .rom.008a7b73, "ax"
	.incbin "baserom.gba", 0x008a7b73, 0x00000001
	.section .rom.008a80be, "ax"
	.incbin "baserom.gba", 0x008a80be, 0x00000002
	.section .rom.008a840a, "ax"
	.incbin "baserom.gba", 0x008a840a, 0x00000002
	.section .rom.008a840c, "ax"
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a840c, 0x0000076c
	.section .rom.008a91bb, "ax"
	.incbin "baserom.gba", 0x008a91bb, 0x00000001
	.section .rom.008a91bc, "ax"
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a91bc, 0x000000a8
	.section .rom.008a957a, "ax"
	.incbin "baserom.gba", 0x008a957a, 0x00000002
	.section .rom.008a9f21, "ax"
	.incbin "baserom.gba", 0x008a9f21, 0x00000003
	.section .rom.008a9f24, "ax"
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9f24, 0x000002f8
	.section .rom.008aad49, "ax"
	.incbin "baserom.gba", 0x008aad49, 0x00000003
	.section .rom.008aad4c, "ax"
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aad4c, 0x00000100
	.section .rom.008ababd, "ax"
	.incbin "baserom.gba", 0x008ababd, 0x00000003
	.section .rom.008ac305, "ax"
	.incbin "baserom.gba", 0x008ac305, 0x00000003
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
	.section .rom.008b472e, "ax"
	.incbin "baserom.gba", 0x008b472e, 0x00000002
	.section .rom.008b56f7, "ax"
	.incbin "baserom.gba", 0x008b56f7, 0x00000001
	.section .rom.008b5952, "ax"
	.incbin "baserom.gba", 0x008b5952, 0x00000002
	.section .rom.008b73ad, "ax"
	.incbin "baserom.gba", 0x008b73ad, 0x00000003
	.section .rom.008b78f9, "ax"
	.incbin "baserom.gba", 0x008b78f9, 0x00000003
	.section .rom.008b9682, "ax"
	.incbin "baserom.gba", 0x008b9682, 0x00000002
	.section .rom.008b98fb, "ax"
	.incbin "baserom.gba", 0x008b98fb, 0x00000001
	.section .rom.008b9dc2, "ax"
	.incbin "baserom.gba", 0x008b9dc2, 0x00000002
	.section .rom.008bb997, "ax"
	.incbin "baserom.gba", 0x008bb997, 0x00000001
	.section .rom.008bd595, "ax"
	.incbin "baserom.gba", 0x008bd595, 0x00000003
	.section .rom.008bd7b6, "ax"
	.incbin "baserom.gba", 0x008bd7b6, 0x00000002
	.section .rom.008bdbf3, "ax"
	.incbin "baserom.gba", 0x008bdbf3, 0x00000001
	.section .rom.008bdd05, "ax"
	.incbin "baserom.gba", 0x008bdd05, 0x00000003
	.section .rom.008be2e7, "ax"
	.incbin "baserom.gba", 0x008be2e7, 0x00000001
	.section .rom.008be785, "ax"
	.incbin "baserom.gba", 0x008be785, 0x00000003
	.section .rom.008bf49a, "ax"
	.incbin "baserom.gba", 0x008bf49a, 0x00000002
	.section .rom.008c021a, "ax"
	.incbin "baserom.gba", 0x008c021a, 0x00000002
	.section .rom.008c05ab, "ax"
	.incbin "baserom.gba", 0x008c05ab, 0x00000001
	.section .rom.008c22f2, "ax"
	.incbin "baserom.gba", 0x008c22f2, 0x00000002
	.section .rom.008c30c9, "ax"
	.incbin "baserom.gba", 0x008c30c9, 0x00000003
	.section .rom.008c38f6, "ax"
	.incbin "baserom.gba", 0x008c38f6, 0x00000002
	.section .rom.008c3ea2, "ax"
	.incbin "baserom.gba", 0x008c3ea2, 0x00000002
	.section .rom.008c405f, "ax"
	.incbin "baserom.gba", 0x008c405f, 0x00000001
	.section .rom.008c4db7, "ax"
	.incbin "baserom.gba", 0x008c4db7, 0x00000001
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
	.section .rom.008caedd, "ax"
	.incbin "baserom.gba", 0x008caedd, 0x00000003
	.section .rom.008cbcab, "ax"
	.incbin "baserom.gba", 0x008cbcab, 0x00000001
	.section .rom.008ccf15, "ax"
	.incbin "baserom.gba", 0x008ccf15, 0x00000003
	.section .rom.008cd46f, "ax"
	.incbin "baserom.gba", 0x008cd46f, 0x00000001
	.section .rom.008ced59, "ax"
	.incbin "baserom.gba", 0x008ced59, 0x00000003
	.section .rom.008cf5fa, "ax"
	.incbin "baserom.gba", 0x008cf5fa, 0x00000002
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
	.section .rom.008d490a, "ax"
	.incbin "baserom.gba", 0x008d490a, 0x00000002
	.section .rom.008d4c8d, "ax"
	.incbin "baserom.gba", 0x008d4c8d, 0x00000003
	.section .rom.008d593d, "ax"
	.incbin "baserom.gba", 0x008d593d, 0x00000003
	.section .rom.008d6485, "ax"
	.incbin "baserom.gba", 0x008d6485, 0x00000003
	.section .rom.008d661e, "ax"
	.incbin "baserom.gba", 0x008d661e, 0x00000002
	.section .rom.008d6eab, "ax"
	.incbin "baserom.gba", 0x008d6eab, 0x00000001
	.section .rom.008d7a7a, "ax"
	.incbin "baserom.gba", 0x008d7a7a, 0x00000002
	.section .rom.008d7f92, "ax"
	.incbin "baserom.gba", 0x008d7f92, 0x00000002
	.section .rom.008d85b6, "ax"
	.incbin "baserom.gba", 0x008d85b6, 0x00000002
	.section .rom.008d89d9, "ax"
	.incbin "baserom.gba", 0x008d89d9, 0x00000003
	.section .rom.008d8c73, "ax"
	.incbin "baserom.gba", 0x008d8c73, 0x00000001
	.section .rom.008da60f, "ax"
	.incbin "baserom.gba", 0x008da60f, 0x00000001
	.section .rom.008dc386, "ax"
	.incbin "baserom.gba", 0x008dc386, 0x00000002
	.section .rom.008de2fb, "ax"
	.incbin "baserom.gba", 0x008de2fb, 0x00000001
	.section .rom.008de7cb, "ax"
	.incbin "baserom.gba", 0x008de7cb, 0x00000001
	.section .rom.008dee5d, "ax"
	.incbin "baserom.gba", 0x008dee5d, 0x00000003
	.section .rom.008df893, "ax"
	.incbin "baserom.gba", 0x008df893, 0x00000001
	.section .rom.008e0c65, "ax"
	.incbin "baserom.gba", 0x008e0c65, 0x00000003
	.section .rom.008e1736, "ax"
	.incbin "baserom.gba", 0x008e1736, 0x00000002
	.section .rom.008e2273, "ax"
	.incbin "baserom.gba", 0x008e2273, 0x00000001
	.section .rom.008e28b3, "ax"
	.incbin "baserom.gba", 0x008e28b3, 0x00000001
	.section .rom.008e3e3a, "ax"
	.incbin "baserom.gba", 0x008e3e3a, 0x00000002
	.section .rom.008e3e9d, "ax"
	.incbin "baserom.gba", 0x008e3e9d, 0x00000003
	.section .rom.008e420b, "ax"
	.incbin "baserom.gba", 0x008e420b, 0x00000001
	.section .rom.008e47a9, "ax"
	.incbin "baserom.gba", 0x008e47a9, 0x00000003
	.section .rom.008e47ac, "ax"
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e47ac, 0x00000204
	.section .rom.008e4ce9, "ax"
	.incbin "baserom.gba", 0x008e4ce9, 0x00000003
	.section .rom.008e5d02, "ax"
	.incbin "baserom.gba", 0x008e5d02, 0x00000002
	.section .rom.008e930e, "ax"
	.incbin "baserom.gba", 0x008e930e, 0x00000002
	.section .rom.008e9ab1, "ax"
	.incbin "baserom.gba", 0x008e9ab1, 0x00000003
	.section .rom.008eae43, "ax"
	.incbin "baserom.gba", 0x008eae43, 0x00000001
	.section .rom.008eb273, "ax"
	.incbin "baserom.gba", 0x008eb273, 0x00000001
	.section .rom.008ed069, "ax"
	.incbin "baserom.gba", 0x008ed069, 0x00000003
	.section .rom.008ed97e, "ax"
	.incbin "baserom.gba", 0x008ed97e, 0x00000002
	.section .rom.008ef4b2, "ax"
	.incbin "baserom.gba", 0x008ef4b2, 0x00000002
	.section .rom.008f0501, "ax"
	.incbin "baserom.gba", 0x008f0501, 0x00000003
	.section .rom.008f15a9, "ax"
	.incbin "baserom.gba", 0x008f15a9, 0x00000003
	.section .rom.0093c3d1, "ax"
	.incbin "baserom.gba", 0x0093c3d1, 0x00000003
	.section .rom.0093c5e9, "ax"
	.incbin "baserom.gba", 0x0093c5e9, 0x00000003
	.section .rom.0093c73d, "ax"
	.incbin "baserom.gba", 0x0093c73d, 0x00000003
	.section .rom.0093cbf5, "ax"
	.incbin "baserom.gba", 0x0093cbf5, 0x00000003
	.section .rom.0093e362, "ax"
	.incbin "baserom.gba", 0x0093e362, 0x00000002
	.section .rom.0093f899, "ax"
	.incbin "baserom.gba", 0x0093f899, 0x00000003
	.section .rom.009416ef, "ax"
	.incbin "baserom.gba", 0x009416ef, 0x00000001
	.section .rom.00943803, "ax"
	.incbin "baserom.gba", 0x00943803, 0x00000001
	.section .rom.00943f7d, "ax"
	.incbin "baserom.gba", 0x00943f7d, 0x00000003
	.section .rom.009465fb, "ax"
	.incbin "baserom.gba", 0x009465fb, 0x00000001
	.section .rom.00947003, "ax"
	.incbin "baserom.gba", 0x00947003, 0x00000001
	.section .rom.00949d22, "ax"
	.incbin "baserom.gba", 0x00949d22, 0x00000002
	.section .rom.00949efa, "ax"
	.incbin "baserom.gba", 0x00949efa, 0x00000002
	.section .rom.0094a359, "ax"
	.incbin "baserom.gba", 0x0094a359, 0x00000003
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
	.section .rom.0094fe82, "ax"
	.incbin "baserom.gba", 0x0094fe82, 0x00000002
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
	.section .rom.00953bf4, "ax"
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953bf4, 0x000002f8
	.section .rom.00953fc5, "ax"
	.incbin "baserom.gba", 0x00953fc5, 0x00000003
	.section .rom.00953fc8, "ax"
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
	.section .rom.00967f74, "ax"
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f74, 0x00000100
	.section .rom.00968074, "ax"
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968074, 0x000004c8
	.section .rom.0096853c, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x0096853c, 0x00000268
	.section .rom.009687a4, "ax"
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
	.section .rom.00969a20, "ax"
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x00969a20, 0x0000024c
	.section .rom.00969d4e, "ax"
	.incbin "baserom.gba", 0x00969d4e, 0x00000002
	.section .rom.00969e09, "ax"
	.incbin "baserom.gba", 0x00969e09, 0x00000003
	.section .rom.00969fde, "ax"
	.incbin "baserom.gba", 0x00969fde, 0x00000002
	.section .rom.0096a078, "ax"
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
	.section .rom.0096ac5c, "ax"
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096ac5c, 0x000000b0
	.section .rom.0096ad0c, "ax"
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096ad0c, 0x00000560
	.section .rom.0096b26c, "ax"
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b26c, 0x00000074
	.section .rom.0096b2e0, "ax"
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b2e0, 0x00000048
	.section .rom.0096b328, "ax"
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b328, 0x00000050
	.section .rom.0096b378, "ax"
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b378, 0x00000050
	.section .rom.0096b3c8, "ax"
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b3c8, 0x00000058
	.section .rom.0096b420, "ax"
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b420, 0x00000058
	.section .rom.0096b478, "ax"
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b478, 0x00000040
	.section .rom.0096b4b8, "ax"
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b4b8, 0x00000044
	.section .rom.0096b4fc, "ax"
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
	.section .rom.009c0487, "ax"
	.incbin "baserom.gba", 0x009c0487, 0x00000001
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
	.section .rom.00a799a7, "ax"
	.incbin "baserom.gba", 0x00a799a7, 0x00000001
	.section .rom.00a79af6, "ax"
	.incbin "baserom.gba", 0x00a79af6, 0x00000002
	.section .rom.00a79c37, "ax"
	.incbin "baserom.gba", 0x00a79c37, 0x00000001
	.section .rom.00a79d77, "ax"
	.incbin "baserom.gba", 0x00a79d77, 0x00000001
	.section .rom.00a79eb7, "ax"
	.incbin "baserom.gba", 0x00a79eb7, 0x00000001
	.section .rom.00a7b115, "ax"
	.incbin "baserom.gba", 0x00a7b115, 0x00000003
	.section .rom.00a7b2e6, "ax"
	.incbin "baserom.gba", 0x00a7b2e6, 0x00000002
	.section .rom.00a7d387, "ax"
	.incbin "baserom.gba", 0x00a7d387, 0x00000001
	.section .rom.00a805e9, "ax"
	.incbin "baserom.gba", 0x00a805e9, 0x00000003
	.section .rom.00a85e5b, "ax"
	.incbin "baserom.gba", 0x00a85e5b, 0x00000001
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
	.section .rom.00b1ab3e, "ax"
	.incbin "baserom.gba", 0x00b1ab3e, 0x00000002
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
	.section .rom.00c28aed, "ax"
	.incbin "baserom.gba", 0x00c28aed, 0x00000003
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
	.section .rom.00d2ac6d, "ax"
	.incbin "baserom.gba", 0x00d2ac6d, 0x00000003
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
	.section .rom.00d4981a, "ax"
	.incbin "baserom.gba", 0x00d4981a, 0x00000002
	.section .rom.00d4bb7f, "ax"
	.incbin "baserom.gba", 0x00d4bb7f, 0x00000001
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
	.section .rom.00e69d86, "ax"
	.incbin "baserom.gba", 0x00e69d86, 0x00000002
	.section .rom.00e6b337, "ax"
	.incbin "baserom.gba", 0x00e6b337, 0x00000001
	.section .rom.00e6b4ea, "ax"
	.incbin "baserom.gba", 0x00e6b4ea, 0x00000002
	.section .rom.00e6d729, "ax"
	.incbin "baserom.gba", 0x00e6d729, 0x00000003
	.section .rom.00e6e24f, "ax"
	.incbin "baserom.gba", 0x00e6e24f, 0x00000001
	.section .rom.00e794a5, "ax"
	.incbin "baserom.gba", 0x00e794a5, 0x00000003
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
	.section .rom.00eb67e5, "ax"
	.incbin "baserom.gba", 0x00eb67e5, 0x00000003
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
	.section .rom.00ebdb07, "ax"
	.incbin "baserom.gba", 0x00ebdb07, 0x00000001
	.section .rom.00ebdc56, "ax"
	.incbin "baserom.gba", 0x00ebdc56, 0x00000002
	.section .rom.00ebdd97, "ax"
	.incbin "baserom.gba", 0x00ebdd97, 0x00000001
	.section .rom.00ebded7, "ax"
	.incbin "baserom.gba", 0x00ebded7, 0x00000001
	.section .rom.00ebe017, "ax"
	.incbin "baserom.gba", 0x00ebe017, 0x00000001
	.section .rom.00ebe72d, "ax"
	.incbin "baserom.gba", 0x00ebe72d, 0x00000003
	.section .rom.00ebe73b, "ax"
	.incbin "baserom.gba", 0x00ebe73b, 0x00000001
	.section .rom.00ebe88a, "ax"
	.incbin "baserom.gba", 0x00ebe88a, 0x00000002
	.section .rom.00ebe9cb, "ax"
	.incbin "baserom.gba", 0x00ebe9cb, 0x00000001
	.section .rom.00ebeb0b, "ax"
	.incbin "baserom.gba", 0x00ebeb0b, 0x00000001
	.section .rom.00ebec4b, "ax"
	.incbin "baserom.gba", 0x00ebec4b, 0x00000001
	.section .rom.00ebf80d, "ax"
	.incbin "baserom.gba", 0x00ebf80d, 0x00000003
	.section .rom.00ebf81b, "ax"
	.incbin "baserom.gba", 0x00ebf81b, 0x00000001
	.section .rom.00ebf96a, "ax"
	.incbin "baserom.gba", 0x00ebf96a, 0x00000002
	.section .rom.00ebfaab, "ax"
	.incbin "baserom.gba", 0x00ebfaab, 0x00000001
	.section .rom.00ebfbeb, "ax"
	.incbin "baserom.gba", 0x00ebfbeb, 0x00000001
	.section .rom.00ebfd2b, "ax"
	.incbin "baserom.gba", 0x00ebfd2b, 0x00000001
	.section .rom.00ec02f9, "ax"
	.incbin "baserom.gba", 0x00ec02f9, 0x00000003
	.section .rom.00ec0307, "ax"
	.incbin "baserom.gba", 0x00ec0307, 0x00000001
	.section .rom.00ec0456, "ax"
	.incbin "baserom.gba", 0x00ec0456, 0x00000002
	.section .rom.00ec0597, "ax"
	.incbin "baserom.gba", 0x00ec0597, 0x00000001
	.section .rom.00ec06d7, "ax"
	.incbin "baserom.gba", 0x00ec06d7, 0x00000001
	.section .rom.00ec0817, "ax"
	.incbin "baserom.gba", 0x00ec0817, 0x00000001
	.section .rom.00ec1195, "ax"
	.incbin "baserom.gba", 0x00ec1195, 0x00000003
	.section .rom.00ec11a3, "ax"
	.incbin "baserom.gba", 0x00ec11a3, 0x00000001
	.section .rom.00ec12f2, "ax"
	.incbin "baserom.gba", 0x00ec12f2, 0x00000002
	.section .rom.00ec1433, "ax"
	.incbin "baserom.gba", 0x00ec1433, 0x00000001
	.section .rom.00ec1573, "ax"
	.incbin "baserom.gba", 0x00ec1573, 0x00000001
	.section .rom.00ec16b3, "ax"
	.incbin "baserom.gba", 0x00ec16b3, 0x00000001
	.section .rom.00ec1add, "ax"
	.incbin "baserom.gba", 0x00ec1add, 0x00000003
	.section .rom.00ec1aeb, "ax"
	.incbin "baserom.gba", 0x00ec1aeb, 0x00000001
	.section .rom.00ec1c3a, "ax"
	.incbin "baserom.gba", 0x00ec1c3a, 0x00000002
	.section .rom.00ec1d7b, "ax"
	.incbin "baserom.gba", 0x00ec1d7b, 0x00000001
	.section .rom.00ec1ebb, "ax"
	.incbin "baserom.gba", 0x00ec1ebb, 0x00000001
	.section .rom.00ec1ffb, "ax"
	.incbin "baserom.gba", 0x00ec1ffb, 0x00000001
	.section .rom.00ec2625, "ax"
	.incbin "baserom.gba", 0x00ec2625, 0x00000003
	.section .rom.00ec2633, "ax"
	.incbin "baserom.gba", 0x00ec2633, 0x00000001
	.section .rom.00ec2782, "ax"
	.incbin "baserom.gba", 0x00ec2782, 0x00000002
	.section .rom.00ec28c3, "ax"
	.incbin "baserom.gba", 0x00ec28c3, 0x00000001
	.section .rom.00ec2a03, "ax"
	.incbin "baserom.gba", 0x00ec2a03, 0x00000001
	.section .rom.00ec2b43, "ax"
	.incbin "baserom.gba", 0x00ec2b43, 0x00000001
	.section .rom.00ec326d, "ax"
	.incbin "baserom.gba", 0x00ec326d, 0x00000003
	.section .rom.00ec327b, "ax"
	.incbin "baserom.gba", 0x00ec327b, 0x00000001
	.section .rom.00ec33ca, "ax"
	.incbin "baserom.gba", 0x00ec33ca, 0x00000002
	.section .rom.00ec350b, "ax"
	.incbin "baserom.gba", 0x00ec350b, 0x00000001
	.section .rom.00ec364b, "ax"
	.incbin "baserom.gba", 0x00ec364b, 0x00000001
	.section .rom.00ec378b, "ax"
	.incbin "baserom.gba", 0x00ec378b, 0x00000001
