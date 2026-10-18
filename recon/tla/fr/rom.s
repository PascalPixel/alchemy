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
	.section .rom.00013e10, "ax"
	.global Func_08013de4
	.type Func_08013de4, %function
	.thumb_func
Func_08013de4:
	.incbin "baserom.gba", 0x00013e10, 0x0000008c
	.section .rom.000141a0, "ax"
	.global VramCache_AllocateBlocks
	.type VramCache_AllocateBlocks, %function
	.thumb_func
VramCache_AllocateBlocks:
	.incbin "baserom.gba", 0x000141a0, 0x00000080
	.section .rom.00014422, "ax"
	.incbin "baserom.gba", 0x00014422, 0x00000002
	.section .rom.00014b9c, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b9c, 0x0000003c
	.section .rom.0001553c, "ax"
	.global Transform_BuildLookAt
	.type Transform_BuildLookAt, %function
	.thumb_func
Transform_BuildLookAt:
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
	.section .rom.00016dfc, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dfc, 0x00000028
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
	.section .rom.00017d10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00017d10, 0x00000004
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
	.global ResourceMetadata_UnregisterFar
	.type ResourceMetadata_UnregisterFar, %function
	.thumb_func
ResourceMetadata_UnregisterFar:
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
	.section .rom.00022d40, "ax"
	.global ResourceObject_Create
	.type ResourceObject_Create, %function
	.thumb_func
ResourceObject_Create:
	.incbin "baserom.gba", 0x00022d40, 0x00000150
	.section .rom.00022f22, "ax"
	.incbin "baserom.gba", 0x00022f22, 0x00000002
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
	.global Func_08023f9c
	.type Func_08023f9c, %function
	.thumb_func
Func_08023f9c:
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
	.global Func_0802493c
	.type Func_0802493c, %function
	.thumb_func
Func_0802493c:
	.incbin "baserom.gba", 0x0002493c, 0x00000314
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
	.global UiWork_DrawByAttributes
	.type UiWork_DrawByAttributes, %function
	.thumb_func
UiWork_DrawByAttributes:
	.incbin "baserom.gba", 0x000391c4, 0x0000009c
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
	.global UiText_QueueRenderEntries
	.type UiText_QueueRenderEntries, %function
	.thumb_func
UiText_QueueRenderEntries:
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
	.section .rom.00039f60, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f60, 0x000000c0
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
	.incbin "baserom.gba", 0x0003b024, 0x00000778
	.section .rom.0003b79c, "ax"
	.global UiWindow_FitOnScreen
	.type UiWindow_FitOnScreen, %function
	.thumb_func
UiWindow_FitOnScreen:
	.incbin "baserom.gba", 0x0003b79c, 0x000000d4
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
	.incbin "baserom.gba", 0x0003c184, 0x000000dc
	.section .rom.0003c260, "ax"
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c260, 0x00000104
	.section .rom.0003c3f8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c3f8, 0x0000013c
	.section .rom.0003c534, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c534, 0x00000464
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
	.incbin "baserom.gba", 0x0003cd4c, 0x00000054
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
	.incbin "baserom.gba", 0x0004066c, 0x00000170
	.section .rom.000407dc, "ax"
	.global Func_08040798
	.type Func_08040798, %function
	.thumb_func
Func_08040798:
	.incbin "baserom.gba", 0x000407dc, 0x000001f4
	.section .rom.00040a34, "ax"
	.incbin "baserom.gba", 0x00040a34, 0x00000270
	.section .rom.00040db4, "ax"
	.incbin "baserom.gba", 0x00040db4, 0x000000f4
	.section .rom.000410ac, "ax"
	.incbin "baserom.gba", 0x000410ac, 0x0000019c
	.section .rom.000412d0, "ax"
	.global Func_0804128c
	.type Func_0804128c, %function
	.thumb_func
Func_0804128c:
	.incbin "baserom.gba", 0x000412d0, 0x000000c4
	.section .rom.000415e4, "ax"
	.incbin "baserom.gba", 0x000415e4, 0x0000012c
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
	.section .rom.000422c4, "ax"
	.balign 4
	.global UiText_DrawPrefixedNumberAtOffset
	.type UiText_DrawPrefixedNumberAtOffset, %function
	.thumb_func
UiText_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422c4, 0x000000a0
	.section .rom.00042558, "ax"
	.balign 4
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x00042558, 0x00000064
	.section .rom.000425ca, "ax"
	.incbin "baserom.gba", 0x000425ca, 0x00000002
	.section .rom.00042730, "ax"
	.incbin "baserom.gba", 0x00042730, 0x0000029c
	.section .rom.000429cc, "ax"
	.global PartyStatusDraw
	.type PartyStatusDraw, %function
	.thumb_func
PartyStatusDraw:
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
	.section .rom.0004d610, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d610, 0x0000009c
	.section .rom.0004d8a0, "ax"
	.incbin "baserom.gba", 0x0004d8a0, 0x00000170
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
	.section .rom.000b2070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000b2070, 0x00000008
	.section .rom.000b2078, "ax"
	.global BattleActGet
	.type BattleActGet, %function
	.thumb_func
BattleActGet:
	.incbin "baserom.gba", 0x000b2078, 0x00000008
	.section .rom.000b2080, "ax"
	.global Unit_LearnPsynergyFar
	.type Unit_LearnPsynergyFar, %function
	.thumb_func
Unit_LearnPsynergyFar:
	.incbin "baserom.gba", 0x000b2080, 0x00000008
	.section .rom.000b2088, "ax"
	.global Unit_KnowsActionFar
	.type Unit_KnowsActionFar, %function
	.thumb_func
Unit_KnowsActionFar:
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
	.global Unit_AddHpFar
	.type Unit_AddHpFar, %function
	.thumb_func
Unit_AddHpFar:
	.incbin "baserom.gba", 0x000b20c0, 0x00000008
	.section .rom.000b20c8, "ax"
	.global Unit_AddPpFar
	.type Unit_AddPpFar, %function
	.thumb_func
Unit_AddPpFar:
	.incbin "baserom.gba", 0x000b20c8, 0x00000008
	.section .rom.000b20d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000b20d0, 0x00000008
	.section .rom.000b20d8, "ax"
	.global Unit_SetHpFar
	.type Unit_SetHpFar, %function
	.thumb_func
Unit_SetHpFar:
	.incbin "baserom.gba", 0x000b20d8, 0x00000008
	.section .rom.000b20e0, "ax"
	.global Unit_SetPpFar
	.type Unit_SetPpFar, %function
	.thumb_func
Unit_SetPpFar:
	.incbin "baserom.gba", 0x000b20e0, 0x00000008
	.section .rom.000b20e8, "ax"
	.global BattleUnitSetFar
	.type BattleUnitSetFar, %function
	.thumb_func
BattleUnitSetFar:
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
	.global BattleHitCheck
	.type BattleHitCheck, %function
	.thumb_func
BattleHitCheck:
	.incbin "baserom.gba", 0x000b2120, 0x00000008
	.section .rom.000b2128, "ax"
	.global BattleAtkCalc
	.type BattleAtkCalc, %function
	.thumb_func
BattleAtkCalc:
	.incbin "baserom.gba", 0x000b2128, 0x00000008
	.section .rom.000b2130, "ax"
	.global BattlePowerCalc
	.type BattlePowerCalc, %function
	.thumb_func
BattlePowerCalc:
	.incbin "baserom.gba", 0x000b2130, 0x00000008
	.section .rom.000b2138, "ax"
	.global BattleRecoveryCalc
	.type BattleRecoveryCalc, %function
	.thumb_func
BattleRecoveryCalc:
	.incbin "baserom.gba", 0x000b2138, 0x00000008
	.section .rom.000b2140, "ax"
	.global Enemy_GetDefinitionFar
	.type Enemy_GetDefinitionFar, %function
	.thumb_func
Enemy_GetDefinitionFar:
	.incbin "baserom.gba", 0x000b2140, 0x00000008
	.section .rom.000b2148, "ax"
	.global BattleRandomFar
	.type BattleRandomFar, %function
	.thumb_func
BattleRandomFar:
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
	.global DjinnRecovery_RemoveFar
	.type DjinnRecovery_RemoveFar, %function
	.thumb_func
DjinnRecovery_RemoveFar:
	.incbin "baserom.gba", 0x000b2168, 0x00000008
	.section .rom.000b2170, "ax"
	.global DjinnRecovery_AddStandbyFar
	.type DjinnRecovery_AddStandbyFar, %function
	.thumb_func
DjinnRecovery_AddStandbyFar:
	.incbin "baserom.gba", 0x000b2170, 0x00000008
	.section .rom.000b2178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000b2178, 0x00000008
	.section .rom.000b2180, "ax"
	.global Summon_ListLearnedFar
	.type Summon_ListLearnedFar, %function
	.thumb_func
Summon_ListLearnedFar:
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
	.global Unit_LevelUpToFar
	.type Unit_LevelUpToFar, %function
	.thumb_func
Unit_LevelUpToFar:
	.incbin "baserom.gba", 0x000b2198, 0x00000008
	.section .rom.000b21a0, "ax"
	.global Unit_GetElementLevelFar
	.type Unit_GetElementLevelFar, %function
	.thumb_func
Unit_GetElementLevelFar:
	.incbin "baserom.gba", 0x000b21a0, 0x00000008
	.section .rom.000b21a8, "ax"
	.global DjinnRecovery_CountStandbyFar
	.type DjinnRecovery_CountStandbyFar, %function
	.thumb_func
DjinnRecovery_CountStandbyFar:
	.incbin "baserom.gba", 0x000b21a8, 0x00000008
	.section .rom.000b21b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000b21b0, 0x00000008
	.section .rom.000b21b8, "ax"
	.global Djinn_IsReadyFar
	.type Djinn_IsReadyFar, %function
	.thumb_func
Djinn_IsReadyFar:
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
	.global Unit_GetExperienceForLevelFar
	.type Unit_GetExperienceForLevelFar, %function
	.thumb_func
Unit_GetExperienceForLevelFar:
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
	.global Party_SetLeaderJennaFar
	.type Party_SetLeaderJennaFar, %function
	.thumb_func
Party_SetLeaderJennaFar:
	.incbin "baserom.gba", 0x000b2218, 0x00000008
	.section .rom.000b2220, "ax"
	.global Party_SetLeaderFelixFar
	.type Party_SetLeaderFelixFar, %function
	.thumb_func
Party_SetLeaderFelixFar:
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
	.global Party_AdjustShopCreditFar
	.type Party_AdjustShopCreditFar, %function
	.thumb_func
Party_AdjustShopCreditFar:
	.incbin "baserom.gba", 0x000b2258, 0x00000008
	.section .rom.000b2260, "ax"
	.global Party_AdjustShopGiftsFar
	.type Party_AdjustShopGiftsFar, %function
	.thumb_func
Party_AdjustShopGiftsFar:
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
	.global Battle_IsReviveEffectFar
	.type Battle_IsReviveEffectFar, %function
	.thumb_func
Battle_IsReviveEffectFar:
	.incbin "baserom.gba", 0x000b2280, 0x00000008
	.section .rom.000b2288, "ax"
	.global Unit_RefreshPsynergyFar
	.type Unit_RefreshPsynergyFar, %function
	.thumb_func
Unit_RefreshPsynergyFar:
	.incbin "baserom.gba", 0x000b2288, 0x00000008
	.section .rom.000b2290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000b2290, 0x00000008
	.section .rom.000b2298, "ax"
	.global Unit_UpdateClassFar
	.type Unit_UpdateClassFar, %function
	.thumb_func
Unit_UpdateClassFar:
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
	.section .rom.000b4bec, "ax"
	.incbin "baserom.gba", 0x000b4bec, 0x000001d0
	.section .rom.000b5084, "ax"
	.global Unit_CalcBaseElements
	.type Unit_CalcBaseElements, %function
	.thumb_func
Unit_CalcBaseElements:
	.incbin "baserom.gba", 0x000b5084, 0x000000c0
	.section .rom.000b5144, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000b5144, 0x00000144
	.section .rom.000b575c, "ax"
	.align 2, 0
	.incbin "baserom.gba", 0x000b575c, 0x00000338
	.section .rom.000b5d58, "ax"
	.global DjinnRecovery_Remove
	.type DjinnRecovery_Remove, %function
	.thumb_func
DjinnRecovery_Remove:
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
	.section .rom.000b6f28, "ax"
	.incbin "baserom.gba", 0x000b6f28, 0x00000004
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
	.global Encounter_ResolveWorldMapZoneAndBackdrop
	.type Encounter_ResolveWorldMapZoneAndBackdrop, %function
	.thumb_func
Encounter_ResolveWorldMapZoneAndBackdrop:
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
	.incbin "baserom.gba", 0x000cf4a8, 0x0000006c
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
	.global Scene_FindActorPlacement
	.type Scene_FindActorPlacement, %function
	.thumb_func
Scene_FindActorPlacement:
	.incbin "baserom.gba", 0x000d1cb8, 0x00000090
	.section .rom.000d1d76, "ax"
	.incbin "baserom.gba", 0x000d1d76, 0x00000002
	.section .rom.000d1d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d1d78, 0x0000010c
	.section .rom.000d1ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d1ec8, 0x000006bc
	.section .rom.000d25bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d25bc, 0x000000c4
	.section .rom.000d2808, "ax"
	.balign 4
	.global Func_080cd808
	.type Func_080cd808, %function
	.thumb_func
Func_080cd808:
	.incbin "baserom.gba", 0x000d2808, 0x00000114
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
	.global Func_080cf158
	.type Func_080cf158, %function
	.thumb_func
Func_080cf158:
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
	.section .rom.000d5a28, "ax"
	.balign 4
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
	.global Func_080d3378
	.type Func_080d3378, %function
	.thumb_func
Func_080d3378:
	.incbin "baserom.gba", 0x000d836c, 0x000000e8
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
	.global Camera_MoveWithinMapBounds
	.type Camera_MoveWithinMapBounds, %function
	.thumb_func
Camera_MoveWithinMapBounds:
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
	.section .rom.000dac64, "ax"
	.incbin "baserom.gba", 0x000dac64, 0x00000100
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
	.global Func_080d64f0
	.type Func_080d64f0, %function
	.thumb_func
Func_080d64f0:
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
	.section .rom.000db9e8, "ax"
	.incbin "baserom.gba", 0x000db9e8, 0x0000019c
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
	.global Func_080d7b04
	.type Func_080d7b04, %function
	.thumb_func
Func_080d7b04:
	.incbin "baserom.gba", 0x000dcaf8, 0x00000100
	.section .rom.000dcd84, "ax"
	.incbin "baserom.gba", 0x000dcd84, 0x0000045c
	.section .rom.000dd2d4, "ax"
	.incbin "baserom.gba", 0x000dd2d4, 0x0000042c
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
	.section .rom.000e265c, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000e2bcc, 0x000003fc
	.section .rom.000e3054, "ax"
	.balign 4
	.global Func_080de060
	.type Func_080de060, %function
	.thumb_func
Func_080de060:
	.incbin "baserom.gba", 0x000e3054, 0x000000f4
	.global Func_080de154
	.type Func_080de154, %function
	.thumb_func
Func_080de154:
	.incbin "baserom.gba", 0x000e3148, 0x000000c0
	.section .rom.000e3210, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000e3210, 0x00000334
	.section .rom.000e38c4, "ax"
	.global Func_080de8d0
	.type Func_080de8d0, %function
	.thumb_func
Func_080de8d0:
	.incbin "baserom.gba", 0x000e38c4, 0x0000011c
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
	.incbin "baserom.gba", 0x000e3ddc, 0x0000038c
	.section .rom.000e41f0, "ax"
	.global Func_080df1fc
	.type Func_080df1fc, %function
	.thumb_func
Func_080df1fc:
	.incbin "baserom.gba", 0x000e41f0, 0x00000624
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
	.section .rom.000e9460, "ax"
	.incbin "baserom.gba", 0x000e9460, 0x00000134
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
	.global BattleFx_SinkAndShrinkObject
BattleFx_SinkAndShrinkObject:
	.incbin "baserom.gba", 0x000eace4, 0x00000698
	.section .rom.000eb37c, "ax"
	.global BattleFx_UpdateGrowingParticleSprite
BattleFx_UpdateGrowingParticleSprite:
	.incbin "baserom.gba", 0x000eb37c, 0x00000d70
	.section .rom.000ec0ec, "ax"
	.global BattleFx_DropObjectAtAnchor
	.type BattleFx_DropObjectAtAnchor, %function
	.thumb_func
BattleFx_DropObjectAtAnchor:
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
	.incbin "baserom.gba", 0x000ed7b8, 0x00000220
	.global Func_080e89e4
	.type Func_080e89e4, %function
	.thumb_func
Func_080e89e4:
	.incbin "baserom.gba", 0x000ed9d8, 0x00000160
	.global Func_080e8b44
	.type Func_080e8b44, %function
	.thumb_func
Func_080e8b44:
	.incbin "baserom.gba", 0x000edb38, 0x00000158
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000edc90, 0x00000060
	.section .rom.000edda4, "ax"
	.global Func_080e8db0
Func_080e8db0:
	.incbin "baserom.gba", 0x000edda4, 0x000004a0
	.section .rom.000ee244, "ax"
	.global BattleFx_ShiftRowsDown
BattleFx_ShiftRowsDown:
	.incbin "baserom.gba", 0x000ee244, 0x00000578
	.section .rom.000ee890, "ax"
	.incbin "baserom.gba", 0x000ee890, 0x00000518
	.section .rom.000eeda8, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000eeda8, 0x000000d8
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
	.section .rom.000f03c4, "ax"
	.balign 4
	.global Func_080eb3d0
	.type Func_080eb3d0, %function
	.thumb_func
Func_080eb3d0:
	.incbin "baserom.gba", 0x000f03c4, 0x000000d0
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
	.section .rom.000f4810, "ax"
	.incbin "baserom.gba", 0x000f4810, 0x00000008
	.section .rom.000f8978, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f8978, 0x00000040
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
	.section .rom.000fa9a4, "ax"
	.balign 4
	.global Func_080f89a4
	.type Func_080f89a4, %function
	.thumb_func
Func_080f89a4:
	.incbin "baserom.gba", 0x000fa9a4, 0x00000110
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
	.incbin "baserom.gba", 0x000feb58, 0x000004d4
	.section .rom.000ff074, "ax"
	.incbin "baserom.gba", 0x000ff074, 0x00000338
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
	.section .rom.00101010, "ax"
	.balign 4
	.global Func_080feec0
	.type Func_080feec0, %function
	.thumb_func
Func_080feec0:
	.incbin "baserom.gba", 0x00101010, 0x00000334
	.section .rom.001014c0, "ax"
	.global Func_080ff370
	.type Func_080ff370, %function
	.thumb_func
Func_080ff370:
	.incbin "baserom.gba", 0x001014c0, 0x00000570
	.section .rom.00101a9c, "ax"
	.incbin "baserom.gba", 0x00101a9c, 0x00000b8c
	.section .rom.001026b0, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x001026b0, 0x000000a4
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
	.incbin "baserom.gba", 0x001037a8, 0x0000036c
	.section .rom.00103b74, "ax"
	.balign 4
	.global Func_08101a04
	.type Func_08101a04, %function
	.thumb_func
Func_08101a04:
	.incbin "baserom.gba", 0x00103b74, 0x00000050
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
	.incbin "baserom.gba", 0x00103ecc, 0x000012f0
	.global Func_08103064
	.type Func_08103064, %function
	.thumb_func
Func_08103064:
	.incbin "baserom.gba", 0x001051bc, 0x000018c4
	.section .rom.00106ae4, "ax"
	.balign 4
	.global Func_0810498c
	.type Func_0810498c, %function
	.thumb_func
Func_0810498c:
	.incbin "baserom.gba", 0x00106ae4, 0x000000cc
	.section .rom.00106d58, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00106d58, 0x000000cc
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
	.section .rom.00108b70, "ax"
	.incbin "baserom.gba", 0x00108b70, 0x00000618
	.section .rom.001091cc, "ax"
	.incbin "baserom.gba", 0x001091cc, 0x000000a4
	.section .rom.0010928c, "ax"
	.incbin "baserom.gba", 0x0010928c, 0x00000324
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
	.section .rom.0010aa84, "ax"
	.balign 4
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
	.global Func_0810bdf4
	.type Func_0810bdf4, %function
	.thumb_func
Func_0810bdf4:
	.incbin "baserom.gba", 0x0010bdf8, 0x00000048
	.section .rom.0010bec0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010bec0, 0x000000dc
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
	.global ColorBuffer_ScaleKernelCode
ColorBuffer_ScaleKernelCode:
	.incbin "baserom.gba", 0x00118394, 0x0000007c
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
	.incbin "baserom.gba", 0x00118958, 0x00000274
	.section .rom.00118c68, "ax"
	.incbin "baserom.gba", 0x00118c68, 0x000000c4
	.section .rom.00118f6c, "ax"
	.balign 4
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
	.section .rom.0011a4f6, "ax"
	.incbin "baserom.gba", 0x0011a4f6, 0x0000011e
	.section .rom.0011a738, "ax"
	.incbin "baserom.gba", 0x0011a738, 0x0000008c
	.section .rom.0011a7c4, "ax"
	.global BattleObjMake
	.type BattleObjMake, %function
	.thumb_func
BattleObjMake:
	.incbin "baserom.gba", 0x0011a7c4, 0x000009c0
	.section .rom.0011b198, "ax"
	.incbin "baserom.gba", 0x0011b198, 0x00000114
	.section .rom.0011b2dc, "ax"
	.global EnemyUpdate
	.type EnemyUpdate, %function
	.thumb_func
EnemyUpdate:
	.incbin "baserom.gba", 0x0011b2dc, 0x000001e8
	.section .rom.0011b4f0, "ax"
	.balign 4
	.global BattleUnit_BuildStatusFlags
	.type BattleUnit_BuildStatusFlags, %function
	.thumb_func
BattleUnit_BuildStatusFlags:
	.incbin "baserom.gba", 0x0011b4f0, 0x000000c0
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
	.section .rom.0011bf30, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bf30, 0x00000088
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
	.incbin "baserom.gba", 0x0011c728, 0x00000274
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
	.section .rom.0011f0a0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011f0a0, 0x000002a8
	.section .rom.0011f4ec, "ax"
	.global Func_0811f4d4
	.type Func_0811f4d4, %function
	.thumb_func
Func_0811f4d4:
	.incbin "baserom.gba", 0x0011f4ec, 0x000000d0
	.section .rom.0011f684, "ax"
	.balign 4
	.global Func_0811f66c
	.type Func_0811f66c, %function
	.thumb_func
Func_0811f66c:
	.incbin "baserom.gba", 0x0011f684, 0x000007d0
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
	.section .rom.0012046c, "ax"
	.global BattleTargetExec
	.type BattleTargetExec, %function
	.thumb_func
BattleTargetExec:
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
	.incbin "baserom.gba", 0x00125bd0, 0x00000054
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
	.incbin "baserom.gba", 0x00126990, 0x00000034
	.section .rom.00126a10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00126a10, 0x000000ec
	.section .rom.00126be4, "ax"
	.global Graphics_AdvancePaletteCycle
	.type Graphics_AdvancePaletteCycle, %function
	.thumb_func
Graphics_AdvancePaletteCycle:
	.incbin "baserom.gba", 0x00126be4, 0x00000130
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
	.incbin "baserom.gba", 0x001275a0, 0x00000484
	.section .rom.00127af4, "ax"
	.global EnemyCountDecrement
	.type EnemyCountDecrement, %function
	.thumb_func
EnemyCountDecrement:
	.incbin "baserom.gba", 0x00127af4, 0x000000c4
	.section .rom.00127c96, "ax"
	.incbin "baserom.gba", 0x00127c96, 0x00000002
	.section .rom.00127cec, "ax"
	.incbin "baserom.gba", 0x00127cec, 0x000003cc
	.section .rom.00128202, "ax"
	.incbin "baserom.gba", 0x00128202, 0x0000003e
	.section .rom.00128264, "ax"
	.global Func_0812824c
	.type Func_0812824c, %function
	.thumb_func
Func_0812824c:
	.incbin "baserom.gba", 0x00128264, 0x00000578
	.section .rom.0013ba50, "ax"
	.incbin "baserom.gba", 0x0013ba50, 0x000000e8
	.section .rom.0013bbd8, "ax"
	.global BattleFx_RunIce
	.type BattleFx_RunIce, %function
	.thumb_func
BattleFx_RunIce:
	.incbin "baserom.gba", 0x0013bbd8, 0x000004e0
	.global BattleFx_RunDouse
	.type BattleFx_RunDouse, %function
	.thumb_func
BattleFx_RunDouse:
	.incbin "baserom.gba", 0x0013c0b8, 0x000004f4
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
	.global BattleFx_RunCruelRuin
	.type BattleFx_RunCruelRuin, %function
	.thumb_func
BattleFx_RunCruelRuin:
	.incbin "baserom.gba", 0x0013fd84, 0x000017b8
	.global BattleFx_RunEclipse
	.type BattleFx_RunEclipse, %function
	.thumb_func
BattleFx_RunEclipse:
	.incbin "baserom.gba", 0x0014153c, 0x00001408
	.section .rom.00142944, "ax"
	.global Func_08142944
	.type Func_08142944, %function
	.thumb_func
Func_08142944:
	.incbin "baserom.gba", 0x00142944, 0x000006bc
	.section .rom.00143354, "ax"
	.incbin "baserom.gba", 0x00143354, 0x00000134
	.section .rom.0014355c, "ax"
	.balign 4
	.global Func_0814355c
	.type Func_0814355c, %function
	.thumb_func
Func_0814355c:
	.incbin "baserom.gba", 0x0014355c, 0x00000084
	.section .rom.001435e0, "ax"
	.global BattleFx_BeginCanvasLayer
	.type BattleFx_BeginCanvasLayer, %function
	.thumb_func
BattleFx_BeginCanvasLayer:
	.incbin "baserom.gba", 0x001435e0, 0x00000260
	.section .rom.00143d80, "ax"
	.incbin "baserom.gba", 0x00143d80, 0x00000134
	.section .rom.00143eb4, "ax"
	.global Func_08143eb4
	.type Func_08143eb4, %function
	.thumb_func
Func_08143eb4:
	.incbin "baserom.gba", 0x00143eb4, 0x00000190
	.global BattleFx_RunHaunt
	.type BattleFx_RunHaunt, %function
	.thumb_func
BattleFx_RunHaunt:
	.incbin "baserom.gba", 0x00144044, 0x00000324
	.global BattleFx_RunOrbitingSpirals
	.type BattleFx_RunOrbitingSpirals, %function
	.thumb_func
BattleFx_RunOrbitingSpirals:
	.incbin "baserom.gba", 0x00144368, 0x00000290
	.section .rom.00144628, "ax"
	.balign 4
	.global Func_08144628
	.type Func_08144628, %function
	.thumb_func
Func_08144628:
	.incbin "baserom.gba", 0x00144628, 0x00000484
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
	.incbin "baserom.gba", 0x0014512c, 0x00000658
	.global BattleFx_RunCondemn
	.type BattleFx_RunCondemn, %function
	.thumb_func
BattleFx_RunCondemn:
	.incbin "baserom.gba", 0x00145784, 0x000004e4
	.section .rom.00145df8, "ax"
	.global BattleFx_RunBind
	.type BattleFx_RunBind, %function
	.thumb_func
BattleFx_RunBind:
	.incbin "baserom.gba", 0x00145df8, 0x00000484
	.global BattleFx_RunTriangleStrike
	.type BattleFx_RunTriangleStrike, %function
	.thumb_func
BattleFx_RunTriangleStrike:
	.incbin "baserom.gba", 0x0014627c, 0x000003bc
	.global BattleFx_RunAstralBlast
	.type BattleFx_RunAstralBlast, %function
	.thumb_func
BattleFx_RunAstralBlast:
	.incbin "baserom.gba", 0x00146638, 0x00000434
	.global BattleFx_RunShiningStar
	.type BattleFx_RunShiningStar, %function
	.thumb_func
BattleFx_RunShiningStar:
	.incbin "baserom.gba", 0x00146a6c, 0x00000394
	.global BattleFx_RunProcne
	.type BattleFx_RunProcne, %function
	.thumb_func
BattleFx_RunProcne:
	.incbin "baserom.gba", 0x00146e00, 0x00000cd4
	.section .rom.00147aec, "ax"
	.balign 4
	.global Func_08147aec
	.type Func_08147aec, %function
	.thumb_func
Func_08147aec:
	.incbin "baserom.gba", 0x00147aec, 0x0000096c
	.global BattleFx_RunNereid
	.type BattleFx_RunNereid, %function
	.thumb_func
BattleFx_RunNereid:
	.incbin "baserom.gba", 0x00148458, 0x000005fc
	.global BattleFx_RunFroth
	.type BattleFx_RunFroth, %function
	.thumb_func
BattleFx_RunFroth:
	.incbin "baserom.gba", 0x00148a54, 0x00000448
	.global BattleFx_RunWhirlwind
	.type BattleFx_RunWhirlwind, %function
	.thumb_func
BattleFx_RunWhirlwind:
	.incbin "baserom.gba", 0x00148e9c, 0x000003cc
	.global BattleFx_RunPrism
	.type BattleFx_RunPrism, %function
	.thumb_func
BattleFx_RunPrism:
	.incbin "baserom.gba", 0x00149268, 0x0000052c
	.global BattleFx_RunPlasma
	.type BattleFx_RunPlasma, %function
	.thumb_func
BattleFx_RunPlasma:
	.incbin "baserom.gba", 0x00149794, 0x00000400
	.section .rom.00149bac, "ax"
	.balign 4
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x00149bac, 0x000006b8
	.global BattleFx_RunVolcano
	.type BattleFx_RunVolcano, %function
	.thumb_func
BattleFx_RunVolcano:
	.incbin "baserom.gba", 0x0014a264, 0x00000544
	.section .rom.0014a812, "ax"
	.incbin "baserom.gba", 0x0014a812, 0x00000002
	.section .rom.0014a814, "ax"
	.global BattleFx_RunTargetMotes
	.type BattleFx_RunTargetMotes, %function
	.thumb_func
BattleFx_RunTargetMotes:
	.incbin "baserom.gba", 0x0014a814, 0x000006a8
	.global BattleFx_RunSleep
	.type BattleFx_RunSleep, %function
	.thumb_func
BattleFx_RunSleep:
	.incbin "baserom.gba", 0x0014aebc, 0x000002a4
	.global BattleFx_RunCurse
	.type BattleFx_RunCurse, %function
	.thumb_func
BattleFx_RunCurse:
	.incbin "baserom.gba", 0x0014b160, 0x00000864
	.section .rom.0014b9e8, "ax"
	.balign 4
	.global Func_0814b9e8
	.type Func_0814b9e8, %function
	.thumb_func
Func_0814b9e8:
	.incbin "baserom.gba", 0x0014b9e8, 0x000004f8
	.global BattleFx_RunZagan
	.type BattleFx_RunZagan, %function
	.thumb_func
BattleFx_RunZagan:
	.incbin "baserom.gba", 0x0014bee0, 0x00000a48
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
	.global BattleFx_RunCybele
	.type BattleFx_RunCybele, %function
	.thumb_func
BattleFx_RunCybele:
	.incbin "baserom.gba", 0x0014ce30, 0x00000cd4
	.global BattleFx_RunBoreas
	.type BattleFx_RunBoreas, %function
	.thumb_func
BattleFx_RunBoreas:
	.incbin "baserom.gba", 0x0014db04, 0x00000c40
	.global BattleFx_RunBreak
	.type BattleFx_RunBreak, %function
	.thumb_func
BattleFx_RunBreak:
	.incbin "baserom.gba", 0x0014e744, 0x000002f8
	.section .rom.0014ea58, "ax"
	.balign 4
	.global Func_0814ea58
	.type Func_0814ea58, %function
	.thumb_func
Func_0814ea58:
	.incbin "baserom.gba", 0x0014ea58, 0x00000430
	.section .rom.0014ef44, "ax"
	.balign 4
	.global BattleFx_RunHealingMotes
	.type BattleFx_RunHealingMotes, %function
	.thumb_func
BattleFx_RunHealingMotes:
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
	.incbin "baserom.gba", 0x001504d8, 0x000004dc
	.global BattleFx_RunFlare
	.type BattleFx_RunFlare, %function
	.thumb_func
BattleFx_RunFlare:
	.incbin "baserom.gba", 0x001509b4, 0x00000264
	.section .rom.00150c74, "ax"
	.global BattleFx_RunQuake
	.type BattleFx_RunQuake, %function
	.thumb_func
BattleFx_RunQuake:
	.incbin "baserom.gba", 0x00150c74, 0x000003cc
	.global BattleFx_RunFire
	.type BattleFx_RunFire, %function
	.thumb_func
BattleFx_RunFire:
	.incbin "baserom.gba", 0x00151040, 0x0000047c
	.global BattleFx_RunFrost
	.type BattleFx_RunFrost, %function
	.thumb_func
BattleFx_RunFrost:
	.incbin "baserom.gba", 0x001514bc, 0x0000068c
	.global BattleFx_RunRay
	.type BattleFx_RunRay, %function
	.thumb_func
BattleFx_RunRay:
	.incbin "baserom.gba", 0x00151b48, 0x00000490
	.section .rom.00151ff0, "ax"
	.balign 4
	.global Func_08151ff0
	.type Func_08151ff0, %function
	.thumb_func
Func_08151ff0:
	.incbin "baserom.gba", 0x00151ff0, 0x00000414
	.section .rom.0015265a, "ax"
	.incbin "baserom.gba", 0x0015265a, 0x00000002
	.section .rom.0015265c, "ax"
	.global BattleFx_RunProjectileVolleyB
	.type BattleFx_RunProjectileVolleyB, %function
	.thumb_func
BattleFx_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x00001860
	.global BattleFx_RunAtalanta
	.type BattleFx_RunAtalanta, %function
	.thumb_func
BattleFx_RunAtalanta:
	.incbin "baserom.gba", 0x00153ebc, 0x000009fc
	.section .rom.001548d0, "ax"
	.balign 4
	.global Func_081548d0
	.type Func_081548d0, %function
	.thumb_func
Func_081548d0:
	.incbin "baserom.gba", 0x001548d0, 0x0000048c
	.section .rom.00154d5c, "ax"
	.global BattleFx_RunVine
	.type BattleFx_RunVine, %function
	.thumb_func
BattleFx_RunVine:
	.incbin "baserom.gba", 0x00154d5c, 0x00000228
	.global BattleFx_RunThorn
	.type BattleFx_RunThorn, %function
	.thumb_func
BattleFx_RunThorn:
	.incbin "baserom.gba", 0x00154f84, 0x000003f8
	.global BattleFx_RunBolt
	.type BattleFx_RunBolt, %function
	.thumb_func
BattleFx_RunBolt:
	.incbin "baserom.gba", 0x0015537c, 0x000004e0
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
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x00000002
	.global BattleFx_RunMercury
	.type BattleFx_RunMercury, %function
	.thumb_func
BattleFx_RunMercury:
	.incbin "baserom.gba", 0x00157638, 0x00000398
	.global BattleFx_RunJupiter
	.type BattleFx_RunJupiter, %function
	.thumb_func
BattleFx_RunJupiter:
	.incbin "baserom.gba", 0x001579d0, 0x00000324
	.section .rom.00157d30, "ax"
	.global BattleFx_RunVenus
	.type BattleFx_RunVenus, %function
	.thumb_func
BattleFx_RunVenus:
	.incbin "baserom.gba", 0x00157d30, 0x0000033c
	.global BattleFx_RunMars
	.type BattleFx_RunMars, %function
	.thumb_func
BattleFx_RunMars:
	.incbin "baserom.gba", 0x0015806c, 0x000003c0
	.global BattleFx_RunHail
	.type BattleFx_RunHail, %function
	.thumb_func
BattleFx_RunHail:
	.incbin "baserom.gba", 0x0015842c, 0x000003b8
	.global BattleFx_RunGround
	.type BattleFx_RunGround, %function
	.thumb_func
BattleFx_RunGround:
	.incbin "baserom.gba", 0x001587e4, 0x00000584
	.section .rom.00158dc8, "ax"
	.balign 4
	.global BattleFx_RunThor
	.type BattleFx_RunThor, %function
	.thumb_func
BattleFx_RunThor:
	.incbin "baserom.gba", 0x00158dc8, 0x00000ee4
	.global BattleFx_RunSpire
	.type BattleFx_RunSpire, %function
	.thumb_func
BattleFx_RunSpire:
	.incbin "baserom.gba", 0x00159cac, 0x00000398
	.section .rom.0015a110, "ax"
	.global BattleFx_RunBurstScene
	.type BattleFx_RunBurstScene, %function
	.thumb_func
BattleFx_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x000008d4
	.global BattleFx_RunGaia
	.type BattleFx_RunGaia, %function
	.thumb_func
BattleFx_RunGaia:
	.incbin "baserom.gba", 0x0015a9e4, 0x00000868
	.section .rom.0015b290, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0015b290, 0x00000180
	.section .rom.0015b434, "ax"
	.incbin "baserom.gba", 0x0015b434, 0x00000330
	.global BattleFx_RunUlysses
	.type BattleFx_RunUlysses, %function
	.thumb_func
BattleFx_RunUlysses:
	.incbin "baserom.gba", 0x0015b764, 0x000011f4
	.section .rom.0015c970, "ax"
	.balign 4
	.global Func_0815c970
	.type Func_0815c970, %function
	.thumb_func
Func_0815c970:
	.incbin "baserom.gba", 0x0015c970, 0x00000260
	.section .rom.0015cbd0, "ax"
	.global BattleFx_RunSpiderWeb
	.type BattleFx_RunSpiderWeb, %function
	.thumb_func
BattleFx_RunSpiderWeb:
	.incbin "baserom.gba", 0x0015cbd0, 0x00000218
	.global BattleFx_RunAzul
	.type BattleFx_RunAzul, %function
	.thumb_func
BattleFx_RunAzul:
	.incbin "baserom.gba", 0x0015cde8, 0x00001388
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
	.incbin "baserom.gba", 0x0015f16c, 0x00004764
	.global BattleFx_RunTorch
	.type BattleFx_RunTorch, %function
	.thumb_func
BattleFx_RunTorch:
	.incbin "baserom.gba", 0x001638d0, 0x00000330
	.section .rom.00163c2c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163c2c, 0x0000015c
	.section .rom.00163e14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163e14, 0x00000da0
	.section .rom.00164c0a, "ax"
	.incbin "baserom.gba", 0x00164c0a, 0x000000ba
	.section .rom.00164cc4, "ax"
	.global BattleFx_RunParticleStreams
BattleFx_RunParticleStreams:
	.incbin "baserom.gba", 0x00164cc4, 0x00000df0
	.global BattleFx_RunRamses
	.type BattleFx_RunRamses, %function
	.thumb_func
BattleFx_RunRamses:
	.incbin "baserom.gba", 0x00165ab4, 0x00000728
	.global BattleFx_RunAnnihilation
	.type BattleFx_RunAnnihilation, %function
	.thumb_func
BattleFx_RunAnnihilation:
	.incbin "baserom.gba", 0x001661dc, 0x000003c8
	.global BattleFx_RunRagnarok
	.type BattleFx_RunRagnarok, %function
	.thumb_func
BattleFx_RunRagnarok:
	.incbin "baserom.gba", 0x001665a4, 0x00000554
	.section .rom.00166b10, "ax"
	.balign 4
	.global Func_08166b10
	.type Func_08166b10, %function
	.thumb_func
Func_08166b10:
	.incbin "baserom.gba", 0x00166b10, 0x0000078c
	.global BattleFx_RunJudgment
	.type BattleFx_RunJudgment, %function
	.thumb_func
BattleFx_RunJudgment:
	.incbin "baserom.gba", 0x0016729c, 0x000017a4
	.global BattleFx_RunKirin
	.type BattleFx_RunKirin, %function
	.thumb_func
BattleFx_RunKirin:
	.incbin "baserom.gba", 0x00168a40, 0x00000970
	.section .rom.001693d0, "ax"
	.global BattleFx_RunNeptune
	.type BattleFx_RunNeptune, %function
	.thumb_func
BattleFx_RunNeptune:
	.incbin "baserom.gba", 0x001693d0, 0x00000e48
	.section .rom.0016a2d8, "ax"
	.balign 4
	.global Func_0816a2d8
Func_0816a2d8:
	.incbin "baserom.gba", 0x0016a2d8, 0x00000458
	.global BattleFx_RunUndeadSword
	.type BattleFx_RunUndeadSword, %function
	.thumb_func
BattleFx_RunUndeadSword:
	.incbin "baserom.gba", 0x0016a730, 0x000002d8
	.global BattleFx_RunPlumeEdge
	.type BattleFx_RunPlumeEdge, %function
	.thumb_func
BattleFx_RunPlumeEdge:
	.incbin "baserom.gba", 0x0016aa08, 0x000004cc
	.section .rom.0016aeec, "ax"
	.balign 4
	.global Func_0816aeec
Func_0816aeec:
	.incbin "baserom.gba", 0x0016aeec, 0x000007e8
	.section .rom.0016b6ec, "ax"
	.balign 4
	.global Func_0816b6ec
Func_0816b6ec:
	.incbin "baserom.gba", 0x0016b6ec, 0x00000584
	.global BattleFx_RunDiamondBerg
	.type BattleFx_RunDiamondBerg, %function
	.thumb_func
BattleFx_RunDiamondBerg:
	.incbin "baserom.gba", 0x0016bc70, 0x00000604
	.global BattleFx_RunCrystal
	.type BattleFx_RunCrystal, %function
	.thumb_func
BattleFx_RunCrystal:
	.incbin "baserom.gba", 0x0016c274, 0x00000484
	.global BattleFx_RunGeode
	.type BattleFx_RunGeode, %function
	.thumb_func
BattleFx_RunGeode:
	.incbin "baserom.gba", 0x0016c6f8, 0x00000a38
	.global BattleFx_RunNirvana
	.type BattleFx_RunNirvana, %function
	.thumb_func
BattleFx_RunNirvana:
	.incbin "baserom.gba", 0x0016d130, 0x000002a8
	.global BattleFx_RunStoneJustice
	.type BattleFx_RunStoneJustice, %function
	.thumb_func
BattleFx_RunStoneJustice:
	.incbin "baserom.gba", 0x0016d3d8, 0x0000034c
	.global BattleFx_RunSoulShatter
	.type BattleFx_RunSoulShatter, %function
	.thumb_func
BattleFx_RunSoulShatter:
	.incbin "baserom.gba", 0x0016d724, 0x0000052c
	.global BattleFx_RunSerac
	.type BattleFx_RunSerac, %function
	.thumb_func
BattleFx_RunSerac:
	.incbin "baserom.gba", 0x0016dc50, 0x000009f4
	.section .rom.0016e65c, "ax"
	.balign 4
	.global Func_0816e65c
Func_0816e65c:
	.incbin "baserom.gba", 0x0016e65c, 0x000005fc
	.global BattleFx_RunCool
	.type BattleFx_RunCool, %function
	.thumb_func
BattleFx_RunCool:
	.incbin "baserom.gba", 0x0016ec58, 0x00000428
	.global BattleFx_RunWyrdCurse
	.type BattleFx_RunWyrdCurse, %function
	.thumb_func
BattleFx_RunWyrdCurse:
	.incbin "baserom.gba", 0x0016f080, 0x00000514
	.global BattleFx_RunRadiantFire
	.type BattleFx_RunRadiantFire, %function
	.thumb_func
BattleFx_RunRadiantFire:
	.incbin "baserom.gba", 0x0016f594, 0x00000394
	.global BattleFx_RunHurricane
	.type BattleFx_RunHurricane, %function
	.thumb_func
BattleFx_RunHurricane:
	.incbin "baserom.gba", 0x0016f928, 0x00000368
	.section .rom.0016fca8, "ax"
	.balign 4
	.global Func_0816fca8
Func_0816fca8:
	.incbin "baserom.gba", 0x0016fca8, 0x000006bc
	.global BattleFx_RunMegaera
	.type BattleFx_RunMegaera, %function
	.thumb_func
BattleFx_RunMegaera:
	.incbin "baserom.gba", 0x00170364, 0x00000a48
	.global BattleFx_RunVoidBeam
	.type BattleFx_RunVoidBeam, %function
	.thumb_func
BattleFx_RunVoidBeam:
	.incbin "baserom.gba", 0x00170dac, 0x00000684
	.global BattleFx_RunLightSurge
	.type BattleFx_RunLightSurge, %function
	.thumb_func
BattleFx_RunLightSurge:
	.incbin "baserom.gba", 0x00171430, 0x00000744
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
	.incbin "baserom.gba", 0x00173aac, 0x000024c8
	.global BattleFx_RunCharon
	.type BattleFx_RunCharon, %function
	.thumb_func
BattleFx_RunCharon:
	.incbin "baserom.gba", 0x00175f74, 0x0000270c
	.section .rom.00178710, "ax"
	.balign 4
	.global BattleFx_RunRisingDragon
	.type BattleFx_RunRisingDragon, %function
	.thumb_func
BattleFx_RunRisingDragon:
	.incbin "baserom.gba", 0x00178710, 0x000007d4
	.global BattleFx_RunHeartbreak
	.type BattleFx_RunHeartbreak, %function
	.thumb_func
BattleFx_RunHeartbreak:
	.incbin "baserom.gba", 0x00178ee4, 0x000004b0
	.global BattleFx_RunRagingHeat
	.type BattleFx_RunRagingHeat, %function
	.thumb_func
BattleFx_RunRagingHeat:
	.incbin "baserom.gba", 0x00179394, 0x00000600
	.global BattleFx_RunJuggle
	.type BattleFx_RunJuggle, %function
	.thumb_func
BattleFx_RunJuggle:
	.incbin "baserom.gba", 0x00179994, 0x00000584
	.global BattleFx_RunFlora
	.type BattleFx_RunFlora, %function
	.thumb_func
BattleFx_RunFlora:
	.incbin "baserom.gba", 0x00179f18, 0x000016bc
	.section .rom.0017b66c, "ax"
	.balign 4
	.global BattleFx_RunSabreDance
	.type BattleFx_RunSabreDance, %function
	.thumb_func
BattleFx_RunSabreDance:
	.incbin "baserom.gba", 0x0017b66c, 0x00000304
	.section .rom.0017b9b8, "ax"
	.balign 4
	.global BattleFx_RunBackstab
	.type BattleFx_RunBackstab, %function
	.thumb_func
BattleFx_RunBackstab:
	.incbin "baserom.gba", 0x0017b9b8, 0x000003d0
	.global BattleFx_RunFirePuppet
	.type BattleFx_RunFirePuppet, %function
	.thumb_func
BattleFx_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd88, 0x000004f4
	.global BattleFx_RunWhiplash
	.type BattleFx_RunWhiplash, %function
	.thumb_func
BattleFx_RunWhiplash:
	.incbin "baserom.gba", 0x0017c27c, 0x00000430
	.global BattleFx_RunFume
	.type BattleFx_RunFume, %function
	.thumb_func
BattleFx_RunFume:
	.incbin "baserom.gba", 0x0017c6ac, 0x00000584
	.global BattleFx_RunOceanFist
	.type BattleFx_RunOceanFist, %function
	.thumb_func
BattleFx_RunOceanFist:
	.incbin "baserom.gba", 0x0017cc30, 0x00000544
	.global BattleFx_RunStarMine
	.type BattleFx_RunStarMine, %function
	.thumb_func
BattleFx_RunStarMine:
	.incbin "baserom.gba", 0x0017d174, 0x00000550
	.section .rom.0017d724, "ax"
	.balign 4
	.global BattleFx_RunRisingDragonEnemy
	.type BattleFx_RunRisingDragonEnemy, %function
	.thumb_func
BattleFx_RunRisingDragonEnemy:
	.incbin "baserom.gba", 0x0017d724, 0x00000954
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
	.global BattleFx_RunDaedalus
	.type BattleFx_RunDaedalus, %function
	.thumb_func
BattleFx_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea58, 0x0000223c
	.global BattleFx_RunDaedalusAlt
	.type BattleFx_RunDaedalusAlt, %function
	.thumb_func
BattleFx_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c94, 0x00000540
	.global BattleFx_RunVengeance
	.type BattleFx_RunVengeance, %function
	.thumb_func
BattleFx_RunVengeance:
	.incbin "baserom.gba", 0x001811d4, 0x0000073c
	.global BattleFx_RunLifeShear
	.type BattleFx_RunLifeShear, %function
	.thumb_func
BattleFx_RunLifeShear:
	.incbin "baserom.gba", 0x00181910, 0x000005a0
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
	.incbin "baserom.gba", 0x00182898, 0x00000210
	.global BattleFx_RunCoatlicue
	.type BattleFx_RunCoatlicue, %function
	.thumb_func
BattleFx_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aa8, 0x00001954
	.global BattleFx_RunCatastrophe
	.type BattleFx_RunCatastrophe, %function
	.thumb_func
BattleFx_RunCatastrophe:
	.incbin "baserom.gba", 0x001843fc, 0x00002a00
	.global BattleFx_RunHaures
	.type BattleFx_RunHaures, %function
	.thumb_func
BattleFx_RunHaures:
	.incbin "baserom.gba", 0x00186dfc, 0x00001918
	.global BattleFx_RunDeathPlunge
	.type BattleFx_RunDeathPlunge, %function
	.thumb_func
BattleFx_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188714, 0x0000059c
	.global BattleFx_RunHelmSplitter
	.type BattleFx_RunHelmSplitter, %function
	.thumb_func
BattleFx_RunHelmSplitter:
	.incbin "baserom.gba", 0x00188cb0, 0x00000498
	.global BattleFx_RunDragonCloud
	.type BattleFx_RunDragonCloud, %function
	.thumb_func
BattleFx_RunDragonCloud:
	.incbin "baserom.gba", 0x00189148, 0x000009cc
	.global BattleFx_RunLegend
	.type BattleFx_RunLegend, %function
	.thumb_func
BattleFx_RunLegend:
	.incbin "baserom.gba", 0x00189b14, 0x000006dc
	.section .rom.0018a220, "ax"
	.balign 4
	.global Func_0818a220
Func_0818a220:
	.incbin "baserom.gba", 0x0018a220, 0x00000af0
	.section .rom.0018ad40, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad40, 0x00000b28
	.global BattleFx_RunTripleChomp
	.type BattleFx_RunTripleChomp, %function
	.thumb_func
BattleFx_RunTripleChomp:
	.incbin "baserom.gba", 0x0018b868, 0x000003f8
	.section .rom.0018bc98, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc98, 0x00000b08
	.section .rom.0018c7b8, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7b8, 0x000003bc
	.global BattleFx_RunTrident
	.type BattleFx_RunTrident, %function
	.thumb_func
BattleFx_RunTrident:
	.incbin "baserom.gba", 0x0018cb74, 0x00000a1c
	.section .rom.0018d5a8, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5a8, 0x00000854
	.section .rom.0018de3c, "ax"
	.incbin "baserom.gba", 0x0018de3c, 0x00000120
	.global BattleFx_RunMeteorBlow
	.type BattleFx_RunMeteorBlow, %function
	.thumb_func
BattleFx_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df5c, 0x00000e48
	.global BattleFx_RunLiquifier
	.type BattleFx_RunLiquifier, %function
	.thumb_func
BattleFx_RunLiquifier:
	.incbin "baserom.gba", 0x0018eda4, 0x00000828
	.section .rom.0018f620, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f620, 0x000008dc
	.global BattleFx_RunMegiddo
	.type BattleFx_RunMegiddo, %function
	.thumb_func
BattleFx_RunMegiddo:
	.incbin "baserom.gba", 0x0018fefc, 0x00000c78
	.global BattleFx_RunOdyssey
	.type BattleFx_RunOdyssey, %function
	.thumb_func
BattleFx_RunOdyssey:
	.incbin "baserom.gba", 0x00190b74, 0x00000f6c
	.section .rom.00191cc4, "ax"
	.incbin "baserom.gba", 0x00191cc4, 0x0000009c
	.global BattleFx_RunMoloch
	.type BattleFx_RunMoloch, %function
	.thumb_func
BattleFx_RunMoloch:
	.incbin "baserom.gba", 0x00191d60, 0x00000ac8
	.section .rom.0019284c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0019284c, 0x00000450
	.global BattleFx_RunIris
	.type BattleFx_RunIris, %function
	.thumb_func
BattleFx_RunIris:
	.incbin "baserom.gba", 0x00192c9c, 0x00003750
	.section .rom.00196404, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000508
	.section .rom.001a052c, "ax"
	.incbin "baserom.gba", 0x001a052c, 0x00000148
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
	.global PaletteFade_Update
	.type PaletteFade_Update, %function
	.thumb_func
PaletteFade_Update:
	.incbin "baserom.gba", 0x001a78c0, 0x00000168
	.section .rom.001a7a28, "ax"
	.global Palette_Transform
	.type Palette_Transform, %function
	.thumb_func
Palette_Transform:
	.incbin "baserom.gba", 0x001a7a28, 0x00000724
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x000011ec
	.section .rom.001b21ec, "ax"
	.incbin "baserom.gba", 0x001b21ec, 0x000012bc
	.section .rom.001b34a8, "ax"
	.global ReelGame_Run
	.type ReelGame_Run, %function
	.thumb_func
ReelGame_Run:
	.incbin "baserom.gba", 0x001b34a8, 0x0000098c
	.section .rom.001b3ed8, "ax"
	.incbin "baserom.gba", 0x001b3ed8, 0x000009b0
	.section .rom.001b8180, "ax"
	.balign 4
	.global Func_081b8180
	.type Func_081b8180, %function
	.thumb_func
Func_081b8180:
	.incbin "baserom.gba", 0x001b8180, 0x00000244
	.section .rom.001b83c4, "ax"
	.global Func_081b83c4
	.type Func_081b83c4, %function
	.thumb_func
Func_081b83c4:
	.incbin "baserom.gba", 0x001b83c4, 0x00001f40
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
	.section .rom.001c0428, "ax"
	.global Func_081c0428
Func_081c0428:
	.incbin "baserom.gba", 0x001c0428, 0x00000064
	.section .rom.001c0490, "ax"
	.balign 4
	.global Func_081c0490
	.type Func_081c0490, %function
	.thumb_func
Func_081c0490:
	.incbin "baserom.gba", 0x001c0490, 0x00000158
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
	.section .rom.001c1014, "ax"
	.balign 4
	.global Func_081c1014
	.type Func_081c1014, %function
	.thumb_func
Func_081c1014:
	.incbin "baserom.gba", 0x001c1014, 0x00000144
	.section .rom.001c342a, "ax"
	.incbin "baserom.gba", 0x001c342a, 0x00000002
	.section .rom.001c3430, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001c3430, 0x0000000c
	.section .rom.0085a3ef, "ax"
	.incbin "baserom.gba", 0x0085a3ef, 0x00000001
	.section .rom.00890411, "ax"
	.incbin "baserom.gba", 0x00890411, 0x00000003
	.section .rom.008abbcd, "ax"
	.incbin "baserom.gba", 0x008abbcd, 0x00000003
	.section .rom.0093c4e1, "ax"
	.incbin "baserom.gba", 0x0093c4e1, 0x00000003
	.section .rom.0094a00a, "ax"
	.incbin "baserom.gba", 0x0094a00a, 0x00000002
	.section .rom.00969dc9, "ax"
	.incbin "baserom.gba", 0x00969dc9, 0x00000003
	.section .rom.00969f9e, "ax"
	.incbin "baserom.gba", 0x00969f9e, 0x00000002
	.section .rom.0096a487, "ax"
	.incbin "baserom.gba", 0x0096a487, 0x00000001
	.section .rom.00a3f0e6, "ax"
	.incbin "baserom.gba", 0x00a3f0e6, 0x00000002
	.section .rom.00b41556, "ax"
	.incbin "baserom.gba", 0x00b41556, 0x00000002
	.section .rom.00bad656, "ax"
	.incbin "baserom.gba", 0x00bad656, 0x00000002
	.section .rom.00c2e3b5, "ax"
	.incbin "baserom.gba", 0x00c2e3b5, 0x00000003
	.section .rom.00d3fb57, "ax"
	.incbin "baserom.gba", 0x00d3fb57, 0x00000001
	.section .rom.00d57ed2, "ax"
	.incbin "baserom.gba", 0x00d57ed2, 0x00000002
	.section .rom.00e0e031, "ax"
	.incbin "baserom.gba", 0x00e0e031, 0x00000003
	.section .rom.00eba3f1, "ax"
	.incbin "baserom.gba", 0x00eba3f1, 0x00000003
