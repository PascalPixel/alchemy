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
	.section .rom.00024efe, "ax"
	.incbin "baserom.gba", 0x00024efe, 0x00000002
	.global Script_CallWithOperand
	.type Script_CallWithOperand, %function
	.thumb_func
Script_CallWithOperand:
	.incbin "baserom.gba", 0x00024f00, 0x00000020
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
	.incbin "baserom.gba", 0x000397e0, 0x000006d4
	.section .rom.00039f78, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f78, 0x000000c0
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
	.incbin "baserom.gba", 0x0003afe4, 0x000007cc
	.section .rom.0003b7b0, "ax"
	.global UiWindow_FitOnScreen
	.type UiWindow_FitOnScreen, %function
	.thumb_func
UiWindow_FitOnScreen:
	.incbin "baserom.gba", 0x0003b7b0, 0x000000d4
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
	.incbin "baserom.gba", 0x0003c198, 0x000000dc
	.section .rom.0003c274, "ax"
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c274, 0x00000104
	.section .rom.0003c40c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c40c, 0x0000013c
	.section .rom.0003c548, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c548, 0x00000464
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
	.incbin "baserom.gba", 0x0003cd60, 0x00000054
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
	.incbin "baserom.gba", 0x00040680, 0x00000170
	.section .rom.000407f0, "ax"
	.global Func_08040798
	.type Func_08040798, %function
	.thumb_func
Func_08040798:
	.incbin "baserom.gba", 0x000407f0, 0x000001f4
	.section .rom.00040a48, "ax"
	.incbin "baserom.gba", 0x00040a48, 0x00000270
	.section .rom.00040dc8, "ax"
	.incbin "baserom.gba", 0x00040dc8, 0x000000f4
	.section .rom.000410c0, "ax"
	.incbin "baserom.gba", 0x000410c0, 0x0000019c
	.section .rom.000412e4, "ax"
	.global Func_0804128c
	.type Func_0804128c, %function
	.thumb_func
Func_0804128c:
	.incbin "baserom.gba", 0x000412e4, 0x000000c4
	.section .rom.000415f8, "ax"
	.incbin "baserom.gba", 0x000415f8, 0x0000012c
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
	.section .rom.000422d8, "ax"
	.balign 4
	.global UiText_DrawPrefixedNumberAtOffset
	.type UiText_DrawPrefixedNumberAtOffset, %function
	.thumb_func
UiText_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422d8, 0x000000a0
	.section .rom.0004256c, "ax"
	.balign 4
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x0004256c, 0x00000064
	.section .rom.000425de, "ax"
	.incbin "baserom.gba", 0x000425de, 0x00000002
	.section .rom.00042744, "ax"
	.incbin "baserom.gba", 0x00042744, 0x0000029c
	.section .rom.000429e0, "ax"
	.global PartyStatusDraw
	.type PartyStatusDraw, %function
	.thumb_func
PartyStatusDraw:
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
	.section .rom.0004d5dc, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5dc, 0x0000009c
	.section .rom.0004d948, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0004d948, 0x00000080
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
	.section .rom.000ad070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000ad070, 0x00000008
	.section .rom.000ad078, "ax"
	.global BattleActGet
	.type BattleActGet, %function
	.thumb_func
BattleActGet:
	.incbin "baserom.gba", 0x000ad078, 0x00000008
	.section .rom.000ad080, "ax"
	.global Unit_LearnPsynergyFar
	.type Unit_LearnPsynergyFar, %function
	.thumb_func
Unit_LearnPsynergyFar:
	.incbin "baserom.gba", 0x000ad080, 0x00000008
	.section .rom.000ad088, "ax"
	.global Unit_KnowsActionFar
	.type Unit_KnowsActionFar, %function
	.thumb_func
Unit_KnowsActionFar:
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
	.global Unit_AddHpFar
	.type Unit_AddHpFar, %function
	.thumb_func
Unit_AddHpFar:
	.incbin "baserom.gba", 0x000ad0c0, 0x00000008
	.section .rom.000ad0c8, "ax"
	.global Unit_AddPpFar
	.type Unit_AddPpFar, %function
	.thumb_func
Unit_AddPpFar:
	.incbin "baserom.gba", 0x000ad0c8, 0x00000008
	.section .rom.000ad0d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000ad0d0, 0x00000008
	.section .rom.000ad0d8, "ax"
	.global Unit_SetHpFar
	.type Unit_SetHpFar, %function
	.thumb_func
Unit_SetHpFar:
	.incbin "baserom.gba", 0x000ad0d8, 0x00000008
	.section .rom.000ad0e0, "ax"
	.global Unit_SetPpFar
	.type Unit_SetPpFar, %function
	.thumb_func
Unit_SetPpFar:
	.incbin "baserom.gba", 0x000ad0e0, 0x00000008
	.section .rom.000ad0e8, "ax"
	.global BattleUnitSetFar
	.type BattleUnitSetFar, %function
	.thumb_func
BattleUnitSetFar:
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
	.global BattleHitCheck
	.type BattleHitCheck, %function
	.thumb_func
BattleHitCheck:
	.incbin "baserom.gba", 0x000ad120, 0x00000008
	.section .rom.000ad128, "ax"
	.global BattleAtkCalc
	.type BattleAtkCalc, %function
	.thumb_func
BattleAtkCalc:
	.incbin "baserom.gba", 0x000ad128, 0x00000008
	.section .rom.000ad130, "ax"
	.global BattlePowerCalc
	.type BattlePowerCalc, %function
	.thumb_func
BattlePowerCalc:
	.incbin "baserom.gba", 0x000ad130, 0x00000008
	.section .rom.000ad138, "ax"
	.global BattleRecoveryCalc
	.type BattleRecoveryCalc, %function
	.thumb_func
BattleRecoveryCalc:
	.incbin "baserom.gba", 0x000ad138, 0x00000008
	.section .rom.000ad140, "ax"
	.global Enemy_GetDefinitionFar
	.type Enemy_GetDefinitionFar, %function
	.thumb_func
Enemy_GetDefinitionFar:
	.incbin "baserom.gba", 0x000ad140, 0x00000008
	.section .rom.000ad148, "ax"
	.global BattleRandomFar
	.type BattleRandomFar, %function
	.thumb_func
BattleRandomFar:
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
	.global DjinnRecovery_RemoveFar
	.type DjinnRecovery_RemoveFar, %function
	.thumb_func
DjinnRecovery_RemoveFar:
	.incbin "baserom.gba", 0x000ad168, 0x00000008
	.section .rom.000ad170, "ax"
	.global DjinnRecovery_AddStandbyFar
	.type DjinnRecovery_AddStandbyFar, %function
	.thumb_func
DjinnRecovery_AddStandbyFar:
	.incbin "baserom.gba", 0x000ad170, 0x00000008
	.section .rom.000ad178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000ad178, 0x00000008
	.section .rom.000ad180, "ax"
	.global Summon_ListLearnedFar
	.type Summon_ListLearnedFar, %function
	.thumb_func
Summon_ListLearnedFar:
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
	.global Unit_LevelUpToFar
	.type Unit_LevelUpToFar, %function
	.thumb_func
Unit_LevelUpToFar:
	.incbin "baserom.gba", 0x000ad198, 0x00000008
	.section .rom.000ad1a0, "ax"
	.global Unit_GetElementLevelFar
	.type Unit_GetElementLevelFar, %function
	.thumb_func
Unit_GetElementLevelFar:
	.incbin "baserom.gba", 0x000ad1a0, 0x00000008
	.section .rom.000ad1a8, "ax"
	.global DjinnRecovery_CountStandbyFar
	.type DjinnRecovery_CountStandbyFar, %function
	.thumb_func
DjinnRecovery_CountStandbyFar:
	.incbin "baserom.gba", 0x000ad1a8, 0x00000008
	.section .rom.000ad1b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000ad1b0, 0x00000008
	.section .rom.000ad1b8, "ax"
	.global Djinn_IsReadyFar
	.type Djinn_IsReadyFar, %function
	.thumb_func
Djinn_IsReadyFar:
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
	.global Unit_GetExperienceForLevelFar
	.type Unit_GetExperienceForLevelFar, %function
	.thumb_func
Unit_GetExperienceForLevelFar:
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
	.global Party_SetLeaderJennaFar
	.type Party_SetLeaderJennaFar, %function
	.thumb_func
Party_SetLeaderJennaFar:
	.incbin "baserom.gba", 0x000ad218, 0x00000008
	.section .rom.000ad220, "ax"
	.global Party_SetLeaderFelixFar
	.type Party_SetLeaderFelixFar, %function
	.thumb_func
Party_SetLeaderFelixFar:
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
	.global Party_AdjustShopCreditFar
	.type Party_AdjustShopCreditFar, %function
	.thumb_func
Party_AdjustShopCreditFar:
	.incbin "baserom.gba", 0x000ad258, 0x00000008
	.section .rom.000ad260, "ax"
	.global Party_AdjustShopGiftsFar
	.type Party_AdjustShopGiftsFar, %function
	.thumb_func
Party_AdjustShopGiftsFar:
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
	.global Battle_IsReviveEffectFar
	.type Battle_IsReviveEffectFar, %function
	.thumb_func
Battle_IsReviveEffectFar:
	.incbin "baserom.gba", 0x000ad280, 0x00000008
	.section .rom.000ad288, "ax"
	.global Unit_RefreshPsynergyFar
	.type Unit_RefreshPsynergyFar, %function
	.thumb_func
Unit_RefreshPsynergyFar:
	.incbin "baserom.gba", 0x000ad288, 0x00000008
	.section .rom.000ad290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000ad290, 0x00000008
	.section .rom.000ad298, "ax"
	.global Unit_UpdateClassFar
	.type Unit_UpdateClassFar, %function
	.thumb_func
Unit_UpdateClassFar:
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
	.section .rom.000afbf8, "ax"
	.incbin "baserom.gba", 0x000afbf8, 0x000001d0
	.section .rom.000b0090, "ax"
	.global Unit_CalcBaseElements
	.type Unit_CalcBaseElements, %function
	.thumb_func
Unit_CalcBaseElements:
	.incbin "baserom.gba", 0x000b0090, 0x000000c0
	.section .rom.000b0150, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000b0150, 0x00000144
	.section .rom.000b0d64, "ax"
	.global DjinnRecovery_Remove
	.type DjinnRecovery_Remove, %function
	.thumb_func
DjinnRecovery_Remove:
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
	.section .rom.000b1f34, "ax"
	.incbin "baserom.gba", 0x000b1f34, 0x00000004
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
	.global FieldEvent_WaitFramesFar
	.type FieldEvent_WaitFramesFar, %function
	.thumb_func
FieldEvent_WaitFramesFar:
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
	.global Event_AskYesNoWithActorAnimationFar
	.type Event_AskYesNoWithActorAnimationFar, %function
	.thumb_func
Event_AskYesNoWithActorAnimationFar:
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
	.global Ui_SetMessageVoiceFromActorFar
	.type Ui_SetMessageVoiceFromActorFar, %function
	.thumb_func
Ui_SetMessageVoiceFromActorFar:
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
	.global Camera_FollowActorFar
	.type Camera_FollowActorFar, %function
	.thumb_func
Camera_FollowActorFar:
	.incbin "baserom.gba", 0x000c8228, 0x00000008
	.section .rom.000c8230, "ax"
	.global Camera_SetSpeedParametersFar
	.type Camera_SetSpeedParametersFar, %function
	.thumb_func
Camera_SetSpeedParametersFar:
	.incbin "baserom.gba", 0x000c8230, 0x00000008
	.section .rom.000c8238, "ax"
	.global Camera_MoveWithinMapBoundsFar
	.type Camera_MoveWithinMapBoundsFar, %function
	.thumb_func
Camera_MoveWithinMapBoundsFar:
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
	.global EffectSlot_HasPendingTargetFar
	.type EffectSlot_HasPendingTargetFar, %function
	.thumb_func
EffectSlot_HasPendingTargetFar:
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
	.global Encounter_ResolveWorldMapZoneAndBackdrop
	.type Encounter_ResolveWorldMapZoneAndBackdrop, %function
	.thumb_func
Encounter_ResolveWorldMapZoneAndBackdrop:
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
	.incbin "baserom.gba", 0x000ca4a8, 0x0000006c
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
	.global Scene_FindActorPlacement
	.type Scene_FindActorPlacement, %function
	.thumb_func
Scene_FindActorPlacement:
	.incbin "baserom.gba", 0x000cccb8, 0x00000090
	.section .rom.000ccd76, "ax"
	.incbin "baserom.gba", 0x000ccd76, 0x00000002
	.section .rom.000ccd78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000ccd78, 0x0000010c
	.section .rom.000ccec8, "ax"
	.global Func_080d0520
	.type Func_080d0520, %function
	.thumb_func
Func_080d0520:
	.incbin "baserom.gba", 0x000ccec8, 0x000006bc
	.section .rom.000cd584, "ax"
	.global Scene_RunRaisedEvent
Scene_RunRaisedEvent:
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
	.global Func_080cf158
	.type Func_080cf158, %function
	.thumb_func
Func_080cf158:
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
	.section .rom.000d0a28, "ax"
	.balign 4
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
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
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
	.global Func_080d1760
	.type Func_080d1760, %function
	.thumb_func
Func_080d1760:
	.incbin "baserom.gba", 0x000d1760, 0x0000004c
	.section .rom.000d183e, "ax"
	.incbin "baserom.gba", 0x000d183e, 0x00000002
	.section .rom.000d1840, "ax"
	.global Func_080d1840
	.type Func_080d1840, %function
	.thumb_func
Func_080d1840:
	.incbin "baserom.gba", 0x000d1840, 0x000001d8
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
	.global Func_080d3378
	.type Func_080d3378, %function
	.thumb_func
Func_080d3378:
	.incbin "baserom.gba", 0x000d336c, 0x000000e8
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
	.global Camera_MoveWithinMapBounds
	.type Camera_MoveWithinMapBounds, %function
	.thumb_func
Camera_MoveWithinMapBounds:
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
	.incbin "baserom.gba", 0x000d53ac, 0x00000834
	.section .rom.000d5c64, "ax"
	.incbin "baserom.gba", 0x000d5c64, 0x00000100
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
	.global Func_080d64f0
	.type Func_080d64f0, %function
	.thumb_func
Func_080d64f0:
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
	.section .rom.000d69e8, "ax"
	.incbin "baserom.gba", 0x000d69e8, 0x0000019c
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
	.global Func_080d7b04
	.type Func_080d7b04, %function
	.thumb_func
Func_080d7b04:
	.incbin "baserom.gba", 0x000d7af8, 0x00000100
	.section .rom.000d7db0, "ax"
	.global BattleEffect_UpdatePhasedRadialParticle
	.type BattleEffect_UpdatePhasedRadialParticle, %function
	.thumb_func
BattleEffect_UpdatePhasedRadialParticle:
	.incbin "baserom.gba", 0x000d7db0, 0x000001c4
	.section .rom.000d8168, "ax"
	.incbin "baserom.gba", 0x000d8168, 0x00000078
	.section .rom.000d82d4, "ax"
	.incbin "baserom.gba", 0x000d82d4, 0x0000042c
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
	.incbin "baserom.gba", 0x000da99c, 0x00000494
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
	.global BattleFx_GetObjectPriority
	.type BattleFx_GetObjectPriority, %function
	.thumb_func
BattleFx_GetObjectPriority:
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
	.incbin "baserom.gba", 0x000ddbcc, 0x000003fc
	.section .rom.000de054, "ax"
	.balign 4
	.global Func_080de060
	.type Func_080de060, %function
	.thumb_func
Func_080de060:
	.incbin "baserom.gba", 0x000de054, 0x000000f4
	.global Func_080de154
	.type Func_080de154, %function
	.thumb_func
Func_080de154:
	.incbin "baserom.gba", 0x000de148, 0x000000c0
	.section .rom.000de210, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000de210, 0x00000334
	.section .rom.000de8c4, "ax"
	.global Func_080de8d0
	.type Func_080de8d0, %function
	.thumb_func
Func_080de8d0:
	.incbin "baserom.gba", 0x000de8c4, 0x0000011c
	.section .rom.000debcc, "ax"
	.balign 4
	.global Func_080debd8
	.type Func_080debd8, %function
	.thumb_func
Func_080debd8:
	.incbin "baserom.gba", 0x000debcc, 0x000000b4
	.section .rom.000decac, "ax"
	.global Field_FindNearbyTalkTarget
	.type Field_FindNearbyTalkTarget, %function
	.thumb_func
Field_FindNearbyTalkTarget:
	.incbin "baserom.gba", 0x000decac, 0x000000cc
	.section .rom.000deddc, "ax"
	.global RunBattleEffect05
RunBattleEffect05:
	.incbin "baserom.gba", 0x000deddc, 0x0000038c
	.section .rom.000df1f0, "ax"
	.global Func_080df1fc
	.type Func_080df1fc, %function
	.thumb_func
Func_080df1fc:
	.incbin "baserom.gba", 0x000df1f0, 0x00000624
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
	.global BattleFx_CallEffect22
BattleFx_CallEffect22:
	.incbin "baserom.gba", 0x000e7830, 0x00000f88
	.section .rom.000e87b8, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e87b8, 0x00000220
	.global Func_080e89e4
	.type Func_080e89e4, %function
	.thumb_func
Func_080e89e4:
	.incbin "baserom.gba", 0x000e89d8, 0x00000160
	.global Func_080e8b44
	.type Func_080e8b44, %function
	.thumb_func
Func_080e8b44:
	.incbin "baserom.gba", 0x000e8b38, 0x00000158
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000e8c90, 0x00000060
	.section .rom.000e8da4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8da4, 0x000004a0
	.section .rom.000e92a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e92a8, 0x00000514
	.section .rom.000e9890, "ax"
	.incbin "baserom.gba", 0x000e9890, 0x00000250
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
	.incbin "baserom.gba", 0x000e9da8, 0x000000d8
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
	.section .rom.000eb3c4, "ax"
	.balign 4
	.global Func_080eb3d0
	.type Func_080eb3d0, %function
	.thumb_func
Func_080eb3d0:
	.incbin "baserom.gba", 0x000eb3c4, 0x000000d0
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
	.section .rom.000ef810, "ax"
	.incbin "baserom.gba", 0x000ef810, 0x00000008
	.section .rom.000f3978, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3978, 0x00000040
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x000004a0
	.section .rom.000f8888, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8888, 0x0000003c
	.section .rom.000f89a4, "ax"
	.balign 4
	.global Func_080f89a4
	.type Func_080f89a4, %function
	.thumb_func
Func_080f89a4:
	.incbin "baserom.gba", 0x000f89a4, 0x00000110
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
	.incbin "baserom.gba", 0x000fcb04, 0x000004d4
	.section .rom.000fd020, "ax"
	.incbin "baserom.gba", 0x000fd020, 0x00000338
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
	.section .rom.000fefbc, "ax"
	.balign 4
	.global Func_080feec0
	.type Func_080feec0, %function
	.thumb_func
Func_080feec0:
	.incbin "baserom.gba", 0x000fefbc, 0x00000334
	.section .rom.000ff46c, "ax"
	.global Func_080ff370
	.type Func_080ff370, %function
	.thumb_func
Func_080ff370:
	.incbin "baserom.gba", 0x000ff46c, 0x00000570
	.section .rom.000ffa48, "ax"
	.incbin "baserom.gba", 0x000ffa48, 0x00000b8c
	.section .rom.0010065c, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x0010065c, 0x000000a4
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
	.incbin "baserom.gba", 0x00101754, 0x0000036c
	.section .rom.00101b20, "ax"
	.balign 4
	.global Func_08101a04
	.type Func_08101a04, %function
	.thumb_func
Func_08101a04:
	.incbin "baserom.gba", 0x00101b20, 0x00000050
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
	.incbin "baserom.gba", 0x00101e7c, 0x000012fc
	.global Func_08103064
	.type Func_08103064, %function
	.thumb_func
Func_08103064:
	.incbin "baserom.gba", 0x00103178, 0x000018c4
	.section .rom.00104aa0, "ax"
	.balign 4
	.global Func_0810498c
	.type Func_0810498c, %function
	.thumb_func
Func_0810498c:
	.incbin "baserom.gba", 0x00104aa0, 0x000000cc
	.section .rom.00104d14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00104d14, 0x000000cc
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
	.incbin "baserom.gba", 0x0010aa84, 0x00000404
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
	.global Func_0810bdf4
	.type Func_0810bdf4, %function
	.thumb_func
Func_0810bdf4:
	.incbin "baserom.gba", 0x0010bde8, 0x00000048
	.section .rom.0010beb0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010beb0, 0x000000dc
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
	.incbin "baserom.gba", 0x001b83c4, 0x00001f44
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
	.section .rom.0088fec5, "ax"
	.incbin "baserom.gba", 0x0088fec5, 0x00000003
	.section .rom.008ab681, "ax"
	.incbin "baserom.gba", 0x008ab681, 0x00000003
	.section .rom.0093bf95, "ax"
	.incbin "baserom.gba", 0x0093bf95, 0x00000003
	.section .rom.00949abe, "ax"
	.incbin "baserom.gba", 0x00949abe, 0x00000002
	.section .rom.00969741, "ax"
	.incbin "baserom.gba", 0x00969741, 0x00000003
	.section .rom.00969916, "ax"
	.incbin "baserom.gba", 0x00969916, 0x00000002
	.section .rom.00969dff, "ax"
	.incbin "baserom.gba", 0x00969dff, 0x00000001
	.section .rom.00a3ea5e, "ax"
	.incbin "baserom.gba", 0x00a3ea5e, 0x00000002
	.section .rom.00b40ece, "ax"
	.incbin "baserom.gba", 0x00b40ece, 0x00000002
	.section .rom.00bacfce, "ax"
	.incbin "baserom.gba", 0x00bacfce, 0x00000002
	.section .rom.00c2dd2d, "ax"
	.incbin "baserom.gba", 0x00c2dd2d, 0x00000003
	.section .rom.00d3f4cf, "ax"
	.incbin "baserom.gba", 0x00d3f4cf, 0x00000001
	.section .rom.00d5784a, "ax"
	.incbin "baserom.gba", 0x00d5784a, 0x00000002
	.section .rom.00e0d9a9, "ax"
	.incbin "baserom.gba", 0x00e0d9a9, 0x00000003
	.section .rom.00eb9d69, "ax"
	.incbin "baserom.gba", 0x00eb9d69, 0x00000003
