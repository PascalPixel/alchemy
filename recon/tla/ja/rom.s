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
	.section .rom.00013de4, "ax"
	.global Func_08013de4
	.type Func_08013de4, %function
	.thumb_func
Func_08013de4:
	.incbin "baserom.gba", 0x00013de4, 0x0000008c
	.section .rom.00014174, "ax"
	.global VramCache_AllocateBlocks
	.type VramCache_AllocateBlocks, %function
	.thumb_func
VramCache_AllocateBlocks:
	.incbin "baserom.gba", 0x00014174, 0x00000080
	.section .rom.00014b70, "ax"
	.global Func_08014b70
	.type Func_08014b70, %function
	.thumb_func
Func_08014b70:
	.incbin "baserom.gba", 0x00014b70, 0x0000003c
	.section .rom.00015510, "ax"
	.global Transform_BuildLookAt
	.type Transform_BuildLookAt, %function
	.thumb_func
Transform_BuildLookAt:
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
	.section .rom.00016180, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x00016180, 0x0000015c
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
	.section .rom.00016dd0, "ax"
	.global Func_08016dd0
	.type Func_08016dd0, %function
	.thumb_func
Func_08016dd0:
	.incbin "baserom.gba", 0x00016dd0, 0x00000028
	.section .rom.00016e02, "ax"
	.incbin "baserom.gba", 0x00016e02, 0x00000002
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
	.section .rom.00023220, "ax"
	.balign 4
	.global Func_08023220
	.type Func_08023220, %function
	.thumb_func
Func_08023220:
	.incbin "baserom.gba", 0x00023220, 0x00000188
	.section .rom.00023680, "ax"
	.balign 4
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
	.section .rom.0002386c, "ax"
	.global Func_0802386c
	.type Func_0802386c, %function
	.thumb_func
Func_0802386c:
	.incbin "baserom.gba", 0x0002386c, 0x00000510
	.section .rom.00023f9c, "ax"
	.global Func_08023f9c
	.type Func_08023f9c, %function
	.thumb_func
Func_08023f9c:
	.incbin "baserom.gba", 0x00023f9c, 0x0000071c
	.section .rom.00024738, "ax"
	.balign 4
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
	.section .rom.00025698, "ax"
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
	.section .rom.00026fc8, "ax"
	.global Func_08026fc8
	.type Func_08026fc8, %function
	.thumb_func
Func_08026fc8:
	.incbin "baserom.gba", 0x00026fc8, 0x0000009c
	.global Func_08027064
	.type Func_08027064, %function
	.thumb_func
Func_08027064:
	.incbin "baserom.gba", 0x00027064, 0x000008c8
	.global Func_080279b0
	.type Func_080279b0, %function
	.thumb_func
Func_080279b0:
	.incbin "baserom.gba", 0x0002792c, 0x0000045c
	.section .rom.00027d9c, "ax"
	.balign 4
	.global Func_08027e20
	.type Func_08027e20, %function
	.thumb_func
Func_08027e20:
	.incbin "baserom.gba", 0x00027d9c, 0x00000714
	.global Func_08028534
	.type Func_08028534, %function
	.thumb_func
Func_08028534:
	.incbin "baserom.gba", 0x000284b0, 0x00000578
	.global Func_08028aac
	.type Func_08028aac, %function
	.thumb_func
Func_08028aac:
	.incbin "baserom.gba", 0x00028a28, 0x00000648
	.section .rom.00029278, "ax"
	.global Func_080292fc
	.type Func_080292fc, %function
	.thumb_func
Func_080292fc:
	.incbin "baserom.gba", 0x00029278, 0x00000274
	.global Func_08029570
	.type Func_08029570, %function
	.thumb_func
Func_08029570:
	.incbin "baserom.gba", 0x000294ec, 0x000002a0
	.section .rom.0002983c, "ax"
	.global Func_080298c0
	.type Func_080298c0, %function
	.thumb_func
Func_080298c0:
	.incbin "baserom.gba", 0x0002983c, 0x00000864
	.global Func_0802a124
	.type Func_0802a124, %function
	.thumb_func
Func_0802a124:
	.incbin "baserom.gba", 0x0002a0a0, 0x00000408
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
	.incbin "baserom.gba", 0x0002bdc8, 0x00000328
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
	.section .rom.0002d3d8, "ax"
	.global Func_0802d45c
	.type Func_0802d45c, %function
	.thumb_func
Func_0802d45c:
	.incbin "baserom.gba", 0x0002d3d8, 0x0000007c
	.section .rom.0002dcec, "ax"
	.incbin "baserom.gba", 0x0002dcec, 0x0000011c
	.section .rom.0002de08, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de08, 0x000005b8
	.section .rom.0002e444, "ax"
	.incbin "baserom.gba", 0x0002e444, 0x000001ec
	.section .rom.0003028c, "ax"
	.incbin "baserom.gba", 0x0003028c, 0x00000010
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x00000310
	.section .rom.000391b8, "ax"
	.balign 4
	.global UiWork_DrawByAttributes
	.type UiWork_DrawByAttributes, %function
	.thumb_func
UiWork_DrawByAttributes:
	.incbin "baserom.gba", 0x000391b8, 0x0000009c
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
	.global UiText_QueueRenderEntries
	.type UiText_QueueRenderEntries, %function
	.thumb_func
UiText_QueueRenderEntries:
	.incbin "baserom.gba", 0x00039540, 0x000000b0
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
	.section .rom.00039f28, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f28, 0x000000c0
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
	.incbin "baserom.gba", 0x0003ac3c, 0x00000138
	.section .rom.0003ae38, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003ae38, 0x00000944
	.section .rom.0003b77c, "ax"
	.global UiWindow_FitOnScreen
	.type UiWindow_FitOnScreen, %function
	.thumb_func
UiWindow_FitOnScreen:
	.incbin "baserom.gba", 0x0003b77c, 0x000000f8
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
	.section .rom.0003cdc0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003cdc0, 0x00000054
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
	.section .rom.0003d7b4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d7b4, 0x0000022c
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
	.incbin "baserom.gba", 0x0004074c, 0x00000170
	.section .rom.000408bc, "ax"
	.global Func_08040798
	.type Func_08040798, %function
	.thumb_func
Func_08040798:
	.incbin "baserom.gba", 0x000408bc, 0x000001f4
	.section .rom.00040b14, "ax"
	.incbin "baserom.gba", 0x00040b14, 0x00000270
	.section .rom.00040e94, "ax"
	.incbin "baserom.gba", 0x00040e94, 0x000000f4
	.section .rom.0004118c, "ax"
	.incbin "baserom.gba", 0x0004118c, 0x0000019c
	.section .rom.000413b0, "ax"
	.global Func_0804128c
	.type Func_0804128c, %function
	.thumb_func
Func_0804128c:
	.incbin "baserom.gba", 0x000413b0, 0x000000c4
	.section .rom.000416c4, "ax"
	.incbin "baserom.gba", 0x000416c4, 0x0000012c
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
	.section .rom.0004264c, "ax"
	.balign 4
	.global Ui_LoadCharacterEntryForSlot
	.type Ui_LoadCharacterEntryForSlot, %function
	.thumb_func
Ui_LoadCharacterEntryForSlot:
	.incbin "baserom.gba", 0x0004264c, 0x00000064
	.section .rom.000426be, "ax"
	.incbin "baserom.gba", 0x000426be, 0x00000002
	.section .rom.00042824, "ax"
	.incbin "baserom.gba", 0x00042824, 0x0000029c
	.section .rom.00042ac0, "ax"
	.global PartyStatusDraw
	.type PartyStatusDraw, %function
	.thumb_func
PartyStatusDraw:
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
	.section .rom.0004d878, "ax"
	.incbin "baserom.gba", 0x0004d878, 0x000000dc
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
	.section .rom.000a8070, "ax"
	.global Inventory_GetEquippedItemFar
	.type Inventory_GetEquippedItemFar, %function
	.thumb_func
Inventory_GetEquippedItemFar:
	.incbin "baserom.gba", 0x000a8070, 0x00000008
	.section .rom.000a8078, "ax"
	.global BattleActGet
	.type BattleActGet, %function
	.thumb_func
BattleActGet:
	.incbin "baserom.gba", 0x000a8078, 0x00000008
	.section .rom.000a8080, "ax"
	.global Unit_LearnPsynergyFar
	.type Unit_LearnPsynergyFar, %function
	.thumb_func
Unit_LearnPsynergyFar:
	.incbin "baserom.gba", 0x000a8080, 0x00000008
	.section .rom.000a8088, "ax"
	.global Unit_KnowsActionFar
	.type Unit_KnowsActionFar, %function
	.thumb_func
Unit_KnowsActionFar:
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
	.global Unit_AddHpFar
	.type Unit_AddHpFar, %function
	.thumb_func
Unit_AddHpFar:
	.incbin "baserom.gba", 0x000a80c0, 0x00000008
	.section .rom.000a80c8, "ax"
	.global Unit_AddPpFar
	.type Unit_AddPpFar, %function
	.thumb_func
Unit_AddPpFar:
	.incbin "baserom.gba", 0x000a80c8, 0x00000008
	.section .rom.000a80d0, "ax"
	.global UnitRatioFar
	.type UnitRatioFar, %function
	.thumb_func
UnitRatioFar:
	.incbin "baserom.gba", 0x000a80d0, 0x00000008
	.section .rom.000a80d8, "ax"
	.global Unit_SetHpFar
	.type Unit_SetHpFar, %function
	.thumb_func
Unit_SetHpFar:
	.incbin "baserom.gba", 0x000a80d8, 0x00000008
	.section .rom.000a80e0, "ax"
	.global Unit_SetPpFar
	.type Unit_SetPpFar, %function
	.thumb_func
Unit_SetPpFar:
	.incbin "baserom.gba", 0x000a80e0, 0x00000008
	.section .rom.000a80e8, "ax"
	.global BattleUnitSetFar
	.type BattleUnitSetFar, %function
	.thumb_func
BattleUnitSetFar:
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
	.global BattleHitCheck
	.type BattleHitCheck, %function
	.thumb_func
BattleHitCheck:
	.incbin "baserom.gba", 0x000a8120, 0x00000008
	.section .rom.000a8128, "ax"
	.global BattleAtkCalc
	.type BattleAtkCalc, %function
	.thumb_func
BattleAtkCalc:
	.incbin "baserom.gba", 0x000a8128, 0x00000008
	.section .rom.000a8130, "ax"
	.global BattlePowerCalc
	.type BattlePowerCalc, %function
	.thumb_func
BattlePowerCalc:
	.incbin "baserom.gba", 0x000a8130, 0x00000008
	.section .rom.000a8138, "ax"
	.global BattleRecoveryCalc
	.type BattleRecoveryCalc, %function
	.thumb_func
BattleRecoveryCalc:
	.incbin "baserom.gba", 0x000a8138, 0x00000008
	.section .rom.000a8140, "ax"
	.global Enemy_GetDefinitionFar
	.type Enemy_GetDefinitionFar, %function
	.thumb_func
Enemy_GetDefinitionFar:
	.incbin "baserom.gba", 0x000a8140, 0x00000008
	.section .rom.000a8148, "ax"
	.global BattleRandomFar
	.type BattleRandomFar, %function
	.thumb_func
BattleRandomFar:
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
	.global DjinnRecovery_RemoveFar
	.type DjinnRecovery_RemoveFar, %function
	.thumb_func
DjinnRecovery_RemoveFar:
	.incbin "baserom.gba", 0x000a8168, 0x00000008
	.section .rom.000a8170, "ax"
	.global DjinnRecovery_AddStandbyFar
	.type DjinnRecovery_AddStandbyFar, %function
	.thumb_func
DjinnRecovery_AddStandbyFar:
	.incbin "baserom.gba", 0x000a8170, 0x00000008
	.section .rom.000a8178, "ax"
	.global Djinn_TransferFar
	.type Djinn_TransferFar, %function
	.thumb_func
Djinn_TransferFar:
	.incbin "baserom.gba", 0x000a8178, 0x00000008
	.section .rom.000a8180, "ax"
	.global Summon_ListLearnedFar
	.type Summon_ListLearnedFar, %function
	.thumb_func
Summon_ListLearnedFar:
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
	.global Unit_LevelUpToFar
	.type Unit_LevelUpToFar, %function
	.thumb_func
Unit_LevelUpToFar:
	.incbin "baserom.gba", 0x000a8198, 0x00000008
	.section .rom.000a81a0, "ax"
	.global Unit_GetElementLevelFar
	.type Unit_GetElementLevelFar, %function
	.thumb_func
Unit_GetElementLevelFar:
	.incbin "baserom.gba", 0x000a81a0, 0x00000008
	.section .rom.000a81a8, "ax"
	.global DjinnRecovery_CountStandbyFar
	.type DjinnRecovery_CountStandbyFar, %function
	.thumb_func
DjinnRecovery_CountStandbyFar:
	.incbin "baserom.gba", 0x000a81a8, 0x00000008
	.section .rom.000a81b0, "ax"
	.global Djinn_IsActiveFar
	.type Djinn_IsActiveFar, %function
	.thumb_func
Djinn_IsActiveFar:
	.incbin "baserom.gba", 0x000a81b0, 0x00000008
	.section .rom.000a81b8, "ax"
	.global Djinn_IsReadyFar
	.type Djinn_IsReadyFar, %function
	.thumb_func
Djinn_IsReadyFar:
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
	.global Unit_GetExperienceForLevelFar
	.type Unit_GetExperienceForLevelFar, %function
	.thumb_func
Unit_GetExperienceForLevelFar:
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
	.global Party_SetLeaderJennaFar
	.type Party_SetLeaderJennaFar, %function
	.thumb_func
Party_SetLeaderJennaFar:
	.incbin "baserom.gba", 0x000a8218, 0x00000008
	.section .rom.000a8220, "ax"
	.global Party_SetLeaderFelixFar
	.type Party_SetLeaderFelixFar, %function
	.thumb_func
Party_SetLeaderFelixFar:
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
	.global Party_AdjustShopCreditFar
	.type Party_AdjustShopCreditFar, %function
	.thumb_func
Party_AdjustShopCreditFar:
	.incbin "baserom.gba", 0x000a8258, 0x00000008
	.section .rom.000a8260, "ax"
	.global Party_AdjustShopGiftsFar
	.type Party_AdjustShopGiftsFar, %function
	.thumb_func
Party_AdjustShopGiftsFar:
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
	.global Battle_IsReviveEffectFar
	.type Battle_IsReviveEffectFar, %function
	.thumb_func
Battle_IsReviveEffectFar:
	.incbin "baserom.gba", 0x000a8280, 0x00000008
	.section .rom.000a8288, "ax"
	.global Unit_RefreshPsynergyFar
	.type Unit_RefreshPsynergyFar, %function
	.thumb_func
Unit_RefreshPsynergyFar:
	.incbin "baserom.gba", 0x000a8288, 0x00000008
	.section .rom.000a8290, "ax"
	.global Party_GetAverageLevelFar
	.type Party_GetAverageLevelFar, %function
	.thumb_func
Party_GetAverageLevelFar:
	.incbin "baserom.gba", 0x000a8290, 0x00000008
	.section .rom.000a8298, "ax"
	.global Unit_UpdateClassFar
	.type Unit_UpdateClassFar, %function
	.thumb_func
Unit_UpdateClassFar:
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
	.section .rom.000aabe8, "ax"
	.incbin "baserom.gba", 0x000aabe8, 0x000001d0
	.section .rom.000ab080, "ax"
	.global Unit_CalcBaseElements
	.type Unit_CalcBaseElements, %function
	.thumb_func
Unit_CalcBaseElements:
	.incbin "baserom.gba", 0x000ab080, 0x000000c0
	.section .rom.000ab140, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000ab140, 0x00000144
	.section .rom.000abd54, "ax"
	.global DjinnRecovery_Remove
	.type DjinnRecovery_Remove, %function
	.thumb_func
DjinnRecovery_Remove:
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
	.section .rom.000c977c, "ax"
	.incbin "baserom.gba", 0x000c977c, 0x000001a4
	.section .rom.000c9944, "ax"
	.incbin "baserom.gba", 0x000c9944, 0x000002cc
	.section .rom.000c9c38, "ax"
	.incbin "baserom.gba", 0x000c9c38, 0x00000190
	.section .rom.000c9e48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000c9e48, 0x000000e4
	.section .rom.000c9fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000c9fd8, 0x0000018c
	.section .rom.000ca2fc, "ax"
	.incbin "baserom.gba", 0x000ca2fc, 0x0000006c
	.section .rom.000ca4a8, "ax"
	.incbin "baserom.gba", 0x000ca4a8, 0x0000006c
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
	.global Field_RunScene
	.type Field_RunScene, %function
	.thumb_func
Field_RunScene:
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
	.global Scene_FindActorPlacement
	.type Scene_FindActorPlacement, %function
	.thumb_func
Scene_FindActorPlacement:
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
	.section .rom.000ccec0, "ax"
	.incbin "baserom.gba", 0x000ccec0, 0x000006bc
	.section .rom.000cd800, "ax"
	.balign 4
	.global Func_080cd808
	.type Func_080cd808, %function
	.thumb_func
Func_080cd808:
	.incbin "baserom.gba", 0x000cd800, 0x00000114
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
	.section .rom.000cec14, "ax"
	.incbin "baserom.gba", 0x000cec14, 0x0000020c
	.section .rom.000cf030, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000cf030, 0x00000080
	.section .rom.000cf15c, "ax"
	.global BattleFx_StartRandomParticleEmitter
	.type BattleFx_StartRandomParticleEmitter, %function
	.thumb_func
BattleFx_StartRandomParticleEmitter:
	.incbin "baserom.gba", 0x000cf15c, 0x000001c4
	.section .rom.000cf534, "ax"
	.incbin "baserom.gba", 0x000cf534, 0x000001a8
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
	.section .rom.000d0a08, "ax"
	.balign 4
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
	.section .rom.000d3356, "ax"
	.incbin "baserom.gba", 0x000d3356, 0x00000002
	.section .rom.000d3358, "ax"
	.global Func_080d3378
	.type Func_080d3378, %function
	.thumb_func
Func_080d3378:
	.incbin "baserom.gba", 0x000d3358, 0x000000e8
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
	.section .rom.000d3c68, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000d3c68, 0x00000328
	.section .rom.000d41c6, "ax"
	.incbin "baserom.gba", 0x000d41c6, 0x00000002
	.section .rom.000d4310, "ax"
	.global Func_080d4330
	.type Func_080d4330, %function
	.thumb_func
Func_080d4330:
	.incbin "baserom.gba", 0x000d4310, 0x00000054
	.section .rom.000d43ec, "ax"
	.balign 4
	.global Camera_MoveWithinMapBounds
	.type Camera_MoveWithinMapBounds, %function
	.thumb_func
Camera_MoveWithinMapBounds:
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
	.section .rom.000d5c50, "ax"
	.incbin "baserom.gba", 0x000d5c50, 0x00000100
	.section .rom.000d5d50, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000d5d50, 0x00000070
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
	.global Func_080d64f0
	.type Func_080d64f0, %function
	.thumb_func
Func_080d64f0:
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
	.section .rom.000d69d4, "ax"
	.global Func_080d69f4
	.type Func_080d69f4, %function
	.thumb_func
Func_080d69f4:
	.incbin "baserom.gba", 0x000d69d4, 0x0000019c
	.section .rom.000d6c40, "ax"
	.global Func_080d6c60
	.type Func_080d6c60, %function
	.thumb_func
Func_080d6c60:
	.incbin "baserom.gba", 0x000d6c40, 0x000001f8
	.section .rom.000d7014, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000d7014, 0x0000020c
	.section .rom.000d7504, "ax"
	.incbin "baserom.gba", 0x000d7504, 0x00000510
	.section .rom.000d7ac0, "ax"
	.global Func_080d7b04
	.type Func_080d7b04, %function
	.thumb_func
Func_080d7b04:
	.incbin "baserom.gba", 0x000d7ac0, 0x00000100
	.section .rom.000d7d78, "ax"
	.global BattleEffect_UpdatePhasedRadialParticle
	.type BattleEffect_UpdatePhasedRadialParticle, %function
	.thumb_func
BattleEffect_UpdatePhasedRadialParticle:
	.incbin "baserom.gba", 0x000d7d78, 0x000001c4
	.section .rom.000d8130, "ax"
	.incbin "baserom.gba", 0x000d8130, 0x00000078
	.section .rom.000d829c, "ax"
	.incbin "baserom.gba", 0x000d829c, 0x0000042c
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
	.section .rom.000d9b3c, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000d9b3c, 0x000001c0
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
	.incbin "baserom.gba", 0x000da964, 0x00000494
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
	.section .rom.000dd710, "ax"
	.incbin "baserom.gba", 0x000dd710, 0x000000cc
	.global Func_080dd820
	.type Func_080dd820, %function
	.thumb_func
Func_080dd820:
	.incbin "baserom.gba", 0x000dd7dc, 0x00000130
	.section .rom.000dd90c, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000dd90c, 0x000000e0
	.section .rom.000ddb94, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000ddb94, 0x000001c8
	.global Func_080ddda0
	.type Func_080ddda0, %function
	.thumb_func
Func_080ddda0:
	.incbin "baserom.gba", 0x000ddd5c, 0x00000108
	.global Func_080ddea8
	.type Func_080ddea8, %function
	.thumb_func
Func_080ddea8:
	.incbin "baserom.gba", 0x000dde64, 0x0000012c
	.section .rom.000de01c, "ax"
	.balign 4
	.global Func_080de060
	.type Func_080de060, %function
	.thumb_func
Func_080de060:
	.incbin "baserom.gba", 0x000de01c, 0x000000f4
	.global Func_080de154
	.type Func_080de154, %function
	.thumb_func
Func_080de154:
	.incbin "baserom.gba", 0x000de110, 0x000000c0
	.section .rom.000de1d8, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000de1d8, 0x00000334
	.section .rom.000de88c, "ax"
	.global Func_080de8d0
	.type Func_080de8d0, %function
	.thumb_func
Func_080de8d0:
	.incbin "baserom.gba", 0x000de88c, 0x0000011c
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
	.incbin "baserom.gba", 0x000deda4, 0x0000038c
	.section .rom.000df1b8, "ax"
	.global Func_080df1fc
	.type Func_080df1fc, %function
	.thumb_func
Func_080df1fc:
	.incbin "baserom.gba", 0x000df1b8, 0x000004f8
	.global Func_080df6f4
	.type Func_080df6f4, %function
	.thumb_func
Func_080df6f4:
	.incbin "baserom.gba", 0x000df6b0, 0x0000012c
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
	.section .rom.000e0ca8, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000e0ca8, 0x000000e8
	.section .rom.000e0efc, "ax"
	.incbin "baserom.gba", 0x000e0efc, 0x000000e4
	.global Func_080e1024
	.type Func_080e1024, %function
	.thumb_func
Func_080e1024:
	.incbin "baserom.gba", 0x000e0fe0, 0x00000130
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
	.section .rom.000e3884, "ax"
	.incbin "baserom.gba", 0x000e3884, 0x00000434
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
	.section .rom.000e4428, "ax"
	.incbin "baserom.gba", 0x000e4428, 0x000002c4
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
	.incbin "baserom.gba", 0x000e7800, 0x00000a84
	.global Func_080e82cc
	.type Func_080e82cc, %function
	.thumb_func
Func_080e82cc:
	.incbin "baserom.gba", 0x000e8284, 0x000004f8
	.section .rom.000e877c, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000e877c, 0x00000220
	.global Func_080e89e4
	.type Func_080e89e4, %function
	.thumb_func
Func_080e89e4:
	.incbin "baserom.gba", 0x000e899c, 0x00000148
	.global Func_080e8b44
	.type Func_080e8b44, %function
	.thumb_func
Func_080e8b44:
	.incbin "baserom.gba", 0x000e8ae4, 0x00000108
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000e8bec, 0x00000054
	.section .rom.000e8cf4, "ax"
	.balign 4
	.global Func_080e8db0
	.type Func_080e8db0, %function
	.thumb_func
Func_080e8db0:
	.incbin "baserom.gba", 0x000e8cf4, 0x000002e0
	.global Func_080e9090
	.type Func_080e9090, %function
	.thumb_func
Func_080e9090:
	.incbin "baserom.gba", 0x000e8fd4, 0x000001c0
	.section .rom.000e91f8, "ax"
	.balign 4
	.global Func_080e92b4
	.type Func_080e92b4, %function
	.thumb_func
Func_080e92b4:
	.incbin "baserom.gba", 0x000e91f8, 0x00000514
	.section .rom.000e97e0, "ax"
	.incbin "baserom.gba", 0x000e97e0, 0x00000250
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
	.incbin "baserom.gba", 0x000e9cf8, 0x000000d8
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
	.section .rom.000eb314, "ax"
	.balign 4
	.global Func_080eb3d0
	.type Func_080eb3d0, %function
	.thumb_func
Func_080eb3d0:
	.incbin "baserom.gba", 0x000eb314, 0x000000d0
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
	.section .rom.000ef728, "ax"
	.incbin "baserom.gba", 0x000ef728, 0x00000008
	.section .rom.000f3890, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3890, 0x00000040
	.section .rom.000f8170, "ax"
	.incbin "baserom.gba", 0x000f8170, 0x00000498
	.section .rom.000f8830, "ax"
	.global UiIcon_PrepareObject
	.type UiIcon_PrepareObject, %function
	.thumb_func
UiIcon_PrepareObject:
	.incbin "baserom.gba", 0x000f8830, 0x0000003c
	.section .rom.000f894c, "ax"
	.balign 4
	.global Func_080f89a4
	.type Func_080f89a4, %function
	.thumb_func
Func_080f89a4:
	.incbin "baserom.gba", 0x000f894c, 0x00000110
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
	.incbin "baserom.gba", 0x000fca7c, 0x0000045c
	.section .rom.000fcf20, "ax"
	.incbin "baserom.gba", 0x000fcf20, 0x00000334
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
	.section .rom.000feef4, "ax"
	.balign 4
	.global Func_080feec0
	.type Func_080feec0, %function
	.thumb_func
Func_080feec0:
	.incbin "baserom.gba", 0x000feef4, 0x00000334
	.section .rom.000ff3a4, "ax"
	.global Func_080ff370
	.type Func_080ff370, %function
	.thumb_func
Func_080ff370:
	.incbin "baserom.gba", 0x000ff3a4, 0x00000570
	.section .rom.000ff980, "ax"
	.incbin "baserom.gba", 0x000ff980, 0x00000b40
	.section .rom.00100548, "ax"
	.global ItemMenu_DrawEquippedItemNames
	.type ItemMenu_DrawEquippedItemNames, %function
	.thumb_func
ItemMenu_DrawEquippedItemNames:
	.incbin "baserom.gba", 0x00100548, 0x000000a4
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
	.incbin "baserom.gba", 0x00101640, 0x0000036c
	.section .rom.00101a0c, "ax"
	.balign 4
	.global Func_08101a04
	.type Func_08101a04, %function
	.thumb_func
Func_08101a04:
	.incbin "baserom.gba", 0x00101a0c, 0x00000050
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
	.incbin "baserom.gba", 0x00101d64, 0x000012d8
	.global Func_08103064
	.type Func_08103064, %function
	.thumb_func
Func_08103064:
	.incbin "baserom.gba", 0x0010303c, 0x000018c4
	.section .rom.00104964, "ax"
	.balign 4
	.global Func_0810498c
	.type Func_0810498c, %function
	.thumb_func
Func_0810498c:
	.incbin "baserom.gba", 0x00104964, 0x000000cc
	.section .rom.00104bd8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00104bd8, 0x000000cc
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
	.incbin "baserom.gba", 0x00108b70, 0x00000600
	.section .rom.00109170, "ax"
	.global Func_08109188
	.type Func_08109188, %function
	.thumb_func
Func_08109188:
	.incbin "baserom.gba", 0x00109170, 0x000000f8
	.section .rom.0010928a, "ax"
	.incbin "baserom.gba", 0x0010928a, 0x00000326
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
	.section .rom.0010aa90, "ax"
	.balign 4
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
	.global Func_0810bdf4
	.type Func_0810bdf4, %function
	.thumb_func
Func_0810bdf4:
	.incbin "baserom.gba", 0x0010be08, 0x00000048
	.section .rom.0010bed0, "ax"
	.global Func_0810bebc
	.type Func_0810bebc, %function
	.thumb_func
Func_0810bebc:
	.incbin "baserom.gba", 0x0010bed0, 0x000000dc
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
	.section .rom.001195ec, "ax"
	.incbin "baserom.gba", 0x001195ec, 0x00000148
	.section .rom.001197f0, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x001197f0, 0x00000844
	.section .rom.0011a4da, "ax"
	.incbin "baserom.gba", 0x0011a4da, 0x0000011e
	.section .rom.0011a71c, "ax"
	.incbin "baserom.gba", 0x0011a71c, 0x0000008c
	.section .rom.0011a7a8, "ax"
	.global BattleObjMake
	.type BattleObjMake, %function
	.thumb_func
BattleObjMake:
	.incbin "baserom.gba", 0x0011a7a8, 0x000009c0
	.section .rom.0011b17c, "ax"
	.incbin "baserom.gba", 0x0011b17c, 0x00000114
	.section .rom.0011b2c0, "ax"
	.global EnemyUpdate
	.type EnemyUpdate, %function
	.thumb_func
EnemyUpdate:
	.incbin "baserom.gba", 0x0011b2c0, 0x000001e8
	.section .rom.0011b4d4, "ax"
	.balign 4
	.global BattleUnit_BuildStatusFlags
	.type BattleUnit_BuildStatusFlags, %function
	.thumb_func
BattleUnit_BuildStatusFlags:
	.incbin "baserom.gba", 0x0011b4d4, 0x000000c0
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
	.section .rom.0011bf14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011bf14, 0x00000088
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
	.incbin "baserom.gba", 0x0011c70c, 0x00000274
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
	.section .rom.0011f084, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011f084, 0x000002a8
	.section .rom.0011f4d0, "ax"
	.global Func_0811f4d4
	.type Func_0811f4d4, %function
	.thumb_func
Func_0811f4d4:
	.incbin "baserom.gba", 0x0011f4d0, 0x000000d0
	.section .rom.0011f668, "ax"
	.balign 4
	.global Func_0811f66c
	.type Func_0811f66c, %function
	.thumb_func
Func_0811f66c:
	.incbin "baserom.gba", 0x0011f668, 0x000007d0
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
	.section .rom.00120450, "ax"
	.global BattleTargetExec
	.type BattleTargetExec, %function
	.thumb_func
BattleTargetExec:
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
	.incbin "baserom.gba", 0x00125bb4, 0x00000054
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
	.incbin "baserom.gba", 0x00126974, 0x00000034
	.section .rom.001269f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001269f4, 0x000000ec
	.section .rom.00126bc8, "ax"
	.global Graphics_AdvancePaletteCycle
	.type Graphics_AdvancePaletteCycle, %function
	.thumb_func
Graphics_AdvancePaletteCycle:
	.incbin "baserom.gba", 0x00126bc8, 0x00000130
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
	.incbin "baserom.gba", 0x00127584, 0x00000484
	.section .rom.00127ad8, "ax"
	.global EnemyCountDecrement
	.type EnemyCountDecrement, %function
	.thumb_func
EnemyCountDecrement:
	.incbin "baserom.gba", 0x00127ad8, 0x000000c4
	.section .rom.00127c7a, "ax"
	.incbin "baserom.gba", 0x00127c7a, 0x00000002
	.section .rom.00127cd0, "ax"
	.incbin "baserom.gba", 0x00127cd0, 0x000003cc
	.section .rom.001281e6, "ax"
	.incbin "baserom.gba", 0x001281e6, 0x00000062
	.section .rom.00128248, "ax"
	.global Func_0812824c
	.type Func_0812824c, %function
	.thumb_func
Func_0812824c:
	.incbin "baserom.gba", 0x00128248, 0x00000578
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
	.thumb_func
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
	.incbin "baserom.gba", 0x00178710, 0x000007d8
	.global BattleFx_RunHeartbreak
	.type BattleFx_RunHeartbreak, %function
	.thumb_func
BattleFx_RunHeartbreak:
	.incbin "baserom.gba", 0x00178ee8, 0x000004b0
	.global BattleFx_RunRagingHeat
	.type BattleFx_RunRagingHeat, %function
	.thumb_func
BattleFx_RunRagingHeat:
	.incbin "baserom.gba", 0x00179398, 0x00000600
	.global BattleFx_RunJuggle
	.type BattleFx_RunJuggle, %function
	.thumb_func
BattleFx_RunJuggle:
	.incbin "baserom.gba", 0x00179998, 0x00000584
	.global BattleFx_RunFlora
	.type BattleFx_RunFlora, %function
	.thumb_func
BattleFx_RunFlora:
	.incbin "baserom.gba", 0x00179f1c, 0x000016bc
	.section .rom.0017b670, "ax"
	.balign 4
	.global BattleFx_RunSabreDance
	.type BattleFx_RunSabreDance, %function
	.thumb_func
BattleFx_RunSabreDance:
	.incbin "baserom.gba", 0x0017b670, 0x00000304
	.section .rom.0017b9bc, "ax"
	.balign 4
	.global BattleFx_RunBackstab
	.type BattleFx_RunBackstab, %function
	.thumb_func
BattleFx_RunBackstab:
	.incbin "baserom.gba", 0x0017b9bc, 0x000003d0
	.global BattleFx_RunFirePuppet
	.type BattleFx_RunFirePuppet, %function
	.thumb_func
BattleFx_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd8c, 0x000004f4
	.global BattleFx_RunWhiplash
	.type BattleFx_RunWhiplash, %function
	.thumb_func
BattleFx_RunWhiplash:
	.incbin "baserom.gba", 0x0017c280, 0x00000430
	.global BattleFx_RunFume
	.type BattleFx_RunFume, %function
	.thumb_func
BattleFx_RunFume:
	.incbin "baserom.gba", 0x0017c6b0, 0x00000584
	.global BattleFx_RunOceanFist
	.type BattleFx_RunOceanFist, %function
	.thumb_func
BattleFx_RunOceanFist:
	.incbin "baserom.gba", 0x0017cc34, 0x00000544
	.global BattleFx_RunStarMine
	.type BattleFx_RunStarMine, %function
	.thumb_func
BattleFx_RunStarMine:
	.incbin "baserom.gba", 0x0017d178, 0x00000550
	.section .rom.0017d728, "ax"
	.balign 4
	.global BattleFx_RunRisingDragonEnemy
	.type BattleFx_RunRisingDragonEnemy, %function
	.thumb_func
BattleFx_RunRisingDragonEnemy:
	.incbin "baserom.gba", 0x0017d728, 0x00000954
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
	.global BattleFx_RunDaedalus
	.type BattleFx_RunDaedalus, %function
	.thumb_func
BattleFx_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea5c, 0x0000223c
	.global BattleFx_RunDaedalusAlt
	.type BattleFx_RunDaedalusAlt, %function
	.thumb_func
BattleFx_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c98, 0x00000540
	.global BattleFx_RunVengeance
	.type BattleFx_RunVengeance, %function
	.thumb_func
BattleFx_RunVengeance:
	.incbin "baserom.gba", 0x001811d8, 0x0000073c
	.global BattleFx_RunLifeShear
	.type BattleFx_RunLifeShear, %function
	.thumb_func
BattleFx_RunLifeShear:
	.incbin "baserom.gba", 0x00181914, 0x000005a0
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
	.incbin "baserom.gba", 0x0018289c, 0x00000210
	.global BattleFx_RunCoatlicue
	.type BattleFx_RunCoatlicue, %function
	.thumb_func
BattleFx_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aac, 0x00001954
	.global BattleFx_RunCatastrophe
	.type BattleFx_RunCatastrophe, %function
	.thumb_func
BattleFx_RunCatastrophe:
	.incbin "baserom.gba", 0x00184400, 0x00002a00
	.global BattleFx_RunHaures
	.type BattleFx_RunHaures, %function
	.thumb_func
BattleFx_RunHaures:
	.incbin "baserom.gba", 0x00186e00, 0x00001918
	.global BattleFx_RunDeathPlunge
	.type BattleFx_RunDeathPlunge, %function
	.thumb_func
BattleFx_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188718, 0x0000059c
	.global BattleFx_RunHelmSplitter
	.type BattleFx_RunHelmSplitter, %function
	.thumb_func
BattleFx_RunHelmSplitter:
	.incbin "baserom.gba", 0x00188cb4, 0x00000498
	.global BattleFx_RunDragonCloud
	.type BattleFx_RunDragonCloud, %function
	.thumb_func
BattleFx_RunDragonCloud:
	.incbin "baserom.gba", 0x0018914c, 0x000009cc
	.global BattleFx_RunLegend
	.type BattleFx_RunLegend, %function
	.thumb_func
BattleFx_RunLegend:
	.incbin "baserom.gba", 0x00189b18, 0x000006dc
	.section .rom.0018a224, "ax"
	.balign 4
	.global Func_0818a220
Func_0818a220:
	.incbin "baserom.gba", 0x0018a224, 0x00000af0
	.section .rom.0018ad44, "ax"
	.balign 4
	.global Func_0818ad40
Func_0818ad40:
	.incbin "baserom.gba", 0x0018ad44, 0x00000b28
	.global BattleFx_RunTripleChomp
	.type BattleFx_RunTripleChomp, %function
	.thumb_func
BattleFx_RunTripleChomp:
	.incbin "baserom.gba", 0x0018b86c, 0x000003f8
	.section .rom.0018bc9c, "ax"
	.balign 4
	.global Func_0818bc98
Func_0818bc98:
	.incbin "baserom.gba", 0x0018bc9c, 0x00000b08
	.section .rom.0018c7bc, "ax"
	.balign 4
	.global Func_0818c7b8
Func_0818c7b8:
	.incbin "baserom.gba", 0x0018c7bc, 0x000003bc
	.global BattleFx_RunTrident
	.type BattleFx_RunTrident, %function
	.thumb_func
BattleFx_RunTrident:
	.incbin "baserom.gba", 0x0018cb78, 0x00000a1c
	.section .rom.0018d5ac, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5ac, 0x00000854
	.section .rom.0018de40, "ax"
	.incbin "baserom.gba", 0x0018de40, 0x00000120
	.global BattleFx_RunMeteorBlow
	.type BattleFx_RunMeteorBlow, %function
	.thumb_func
BattleFx_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df60, 0x00000e48
	.global BattleFx_RunLiquifier
	.type BattleFx_RunLiquifier, %function
	.thumb_func
BattleFx_RunLiquifier:
	.incbin "baserom.gba", 0x0018eda8, 0x00000828
	.section .rom.0018f624, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f624, 0x000008dc
	.global BattleFx_RunMegiddo
	.type BattleFx_RunMegiddo, %function
	.thumb_func
BattleFx_RunMegiddo:
	.incbin "baserom.gba", 0x0018ff00, 0x00000c78
	.global BattleFx_RunOdyssey
	.type BattleFx_RunOdyssey, %function
	.thumb_func
BattleFx_RunOdyssey:
	.incbin "baserom.gba", 0x00190b78, 0x00000f6c
	.section .rom.00191cc8, "ax"
	.incbin "baserom.gba", 0x00191cc8, 0x0000009c
	.global BattleFx_RunMoloch
	.type BattleFx_RunMoloch, %function
	.thumb_func
BattleFx_RunMoloch:
	.incbin "baserom.gba", 0x00191d64, 0x00000ac8
	.section .rom.00192850, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00192850, 0x00000450
	.global BattleFx_RunIris
	.type BattleFx_RunIris, %function
	.thumb_func
BattleFx_RunIris:
	.incbin "baserom.gba", 0x00192ca0, 0x00003750
	.section .rom.00196408, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196408, 0x00000508
	.section .rom.001a052c, "ax"
	.incbin "baserom.gba", 0x001a052c, 0x00000148
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
	.incbin "baserom.gba", 0x001b21ec, 0x00001178
	.section .rom.001b3364, "ax"
	.global ReelGame_Run
	.type ReelGame_Run, %function
	.thumb_func
ReelGame_Run:
	.incbin "baserom.gba", 0x001b3364, 0x00000988
	.section .rom.001b3d90, "ax"
	.incbin "baserom.gba", 0x001b3d90, 0x000009b0
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
	.incbin "baserom.gba", 0x001b83c4, 0x00001f48
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
	.section .rom.00890301, "ax"
	.incbin "baserom.gba", 0x00890301, 0x00000003
	.section .rom.008ababd, "ax"
	.incbin "baserom.gba", 0x008ababd, 0x00000003
	.section .rom.0093c3d1, "ax"
	.incbin "baserom.gba", 0x0093c3d1, 0x00000003
	.section .rom.00949efa, "ax"
	.incbin "baserom.gba", 0x00949efa, 0x00000002
	.section .rom.0096a4c7, "ax"
	.incbin "baserom.gba", 0x0096a4c7, 0x00000001
	.section .rom.00bad68e, "ax"
	.incbin "baserom.gba", 0x00bad68e, 0x00000002
	.section .rom.00c2e3bd, "ax"
	.incbin "baserom.gba", 0x00c2e3bd, 0x00000003
	.section .rom.00d57dea, "ax"
	.incbin "baserom.gba", 0x00d57dea, 0x00000002
	.section .rom.00e0df49, "ax"
	.incbin "baserom.gba", 0x00e0df49, 0x00000003
	.section .rom.00eba2e5, "ax"
	.incbin "baserom.gba", 0x00eba2e5, 0x00000003
