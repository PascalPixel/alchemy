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
	.section .rom.000139c8, "ax"
	.balign 4
	.global Func_0801399c
	.type Func_0801399c, %function
	.thumb_func
Func_0801399c:
	.incbin "baserom.gba", 0x000139c8, 0x00000194
	.section .rom.000141a0, "ax"
	.global VramCache_AllocateBlocks
	.type VramCache_AllocateBlocks, %function
	.thumb_func
VramCache_AllocateBlocks:
	.incbin "baserom.gba", 0x000141a0, 0x00000080
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
	.incbin "baserom.gba", 0x00015998, 0x00000164
	.section .rom.000161ac, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x000161ac, 0x0000015c
	.section .rom.000169bc, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x000169bc, 0x00000248
	.section .rom.00016e2e, "ax"
	.incbin "baserom.gba", 0x00016e2e, 0x00000002
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
	.section .rom.00028aac, "ax"
	.balign 4
	.global Func_08028aac
	.type Func_08028aac, %function
	.thumb_func
Func_08028aac:
	.incbin "baserom.gba", 0x00028aac, 0x00000648
	.section .rom.000292fc, "ax"
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
	.section .rom.000298c0, "ax"
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
	.incbin "baserom.gba", 0x0002aa74, 0x00000148
	.section .rom.0002ac90, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0002ac90, 0x0000030c
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
	.global Func_0802bdd0
	.type Func_0802bdd0, %function
	.thumb_func
Func_0802bdd0:
	.incbin "baserom.gba", 0x0002bdd0, 0x0000007c
	.section .rom.0002be4c, "ax"
	.global Func_0802be4c
	.type Func_0802be4c, %function
	.thumb_func
Func_0802be4c:
	.incbin "baserom.gba", 0x0002be4c, 0x00000328
	.section .rom.0002c240, "ax"
	.global Func_0802c240
	.type Func_0802c240, %function
	.thumb_func
Func_0802c240:
	.incbin "baserom.gba", 0x0002c240, 0x00000298
	.section .rom.0002c58c, "ax"
	.incbin "baserom.gba", 0x0002c58c, 0x000002d8
	.section .rom.0002d088, "ax"
	.global Func_0802d088
	.type Func_0802d088, %function
	.thumb_func
Func_0802d088:
	.incbin "baserom.gba", 0x0002d088, 0x000001b4
	.section .rom.0002dd70, "ax"
	.incbin "baserom.gba", 0x0002dd70, 0x0000011c
	.section .rom.0002de8c, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de8c, 0x000005b8
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
	.incbin "baserom.gba", 0x0003954c, 0x000000b0
	.section .rom.0003972a, "ax"
	.incbin "baserom.gba", 0x0003972a, 0x0000002a
	.section .rom.00039864, "ax"
	.global Func_08039864
	.type Func_08039864, %function
	.thumb_func
Func_08039864:
	.incbin "baserom.gba", 0x00039864, 0x0000062c
	.section .rom.00039f54, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f54, 0x000000c0
	.section .rom.0003a406, "ax"
	.incbin "baserom.gba", 0x0003a406, 0x00000002
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
	.incbin "baserom.gba", 0x0003aa8c, 0x0000017c
	.section .rom.0003ae60, "ax"
	.incbin "baserom.gba", 0x0003ae60, 0x0000016c
	.section .rom.0003afcc, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.incbin "baserom.gba", 0x0003afcc, 0x000007e0
	.section .rom.0003b930, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0003b930, 0x00000234
	.global Func_0803bb58
	.type Func_0803bb58, %function
	.thumb_func
Func_0803bb58:
	.incbin "baserom.gba", 0x0003bb64, 0x00000500
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
	.incbin "baserom.gba", 0x0003c194, 0x000000dc
	.section .rom.0003c270, "ax"
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c270, 0x00000104
	.section .rom.0003c408, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c408, 0x0000013c
	.section .rom.0003c544, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x0003c544, 0x00000464
	.section .rom.0003d6d4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d6d4, 0x0000022c
	.section .rom.0003df78, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003df78, 0x00000564
	.section .rom.0003e81c, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e81c, 0x00000124
	.section .rom.0003eb80, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003eb80, 0x000000b0
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
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0003f058, 0x000001d0
	.section .rom.0003f41c, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f41c, 0x00000248
	.section .rom.0003fd7c, "ax"
	.incbin "baserom.gba", 0x0003fd7c, 0x000003c0
	.section .rom.0004013c, "ax"
	.global Func_080400e8
	.type Func_080400e8, %function
	.thumb_func
Func_080400e8:
	.incbin "baserom.gba", 0x0004013c, 0x000004c4
	.section .rom.0004067c, "ax"
	.incbin "baserom.gba", 0x0004067c, 0x00000170
	.section .rom.000407ec, "ax"
	.global Func_08040798
	.type Func_08040798, %function
	.thumb_func
Func_08040798:
	.incbin "baserom.gba", 0x000407ec, 0x000001f4
	.section .rom.00040a44, "ax"
	.global Func_080409f0
	.type Func_080409f0, %function
	.thumb_func
Func_080409f0:
	.incbin "baserom.gba", 0x00040a44, 0x00000058
	.section .rom.00040dc4, "ax"
	.incbin "baserom.gba", 0x00040dc4, 0x000000f4
	.section .rom.000415f4, "ax"
	.incbin "baserom.gba", 0x000415f4, 0x0000012c
	.section .rom.00041720, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x00041720, 0x000003f0
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
	.section .rom.000422c8, "ax"
	.balign 4
	.global UiText_DrawPrefixedNumberAtOffset
	.type UiText_DrawPrefixedNumberAtOffset, %function
	.thumb_func
UiText_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000422c8, 0x000000a0
	.section .rom.000425ce, "ax"
	.incbin "baserom.gba", 0x000425ce, 0x00000002
	.section .rom.00042734, "ax"
	.incbin "baserom.gba", 0x00042734, 0x0000029c
	.section .rom.000429d0, "ax"
	.global PartyStatus_Draw
	.type PartyStatus_Draw, %function
	.thumb_func
PartyStatus_Draw:
	.incbin "baserom.gba", 0x000429d0, 0x00000430
	.section .rom.000433ac, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x000433ac, 0x00000194
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
	.section .rom.00045170, "ax"
	.global Func_080450fc
	.type Func_080450fc, %function
	.thumb_func
Func_080450fc:
	.incbin "baserom.gba", 0x00045170, 0x000000a0
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
	.section .rom.0004d378, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x0004d378, 0x00000120
	.section .rom.0004d5f4, "ax"
	.global Menu_CenterResourceEntries
	.type Menu_CenterResourceEntries, %function
	.thumb_func
Menu_CenterResourceEntries:
	.incbin "baserom.gba", 0x0004d5f4, 0x0000009c
	.section .rom.0004da08, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004da08, 0x0000016c
	.section .rom.0004dcc6, "ax"
	.incbin "baserom.gba", 0x0004dcc6, 0x00000002
	.section .rom.0004e0ac, "ax"
	.global Menu_DrawFlagBitTable
	.type Menu_DrawFlagBitTable, %function
	.thumb_func
Menu_DrawFlagBitTable:
	.incbin "baserom.gba", 0x0004e0ac, 0x000000c4
	.section .rom.0004e3e4, "ax"
	.global Func_0804e1d8
	.type Func_0804e1d8, %function
	.thumb_func
Func_0804e1d8:
	.incbin "baserom.gba", 0x0004e3e4, 0x0000021c
	.section .rom.000ba3f6, "ax"
	.incbin "baserom.gba", 0x000ba3f6, 0x00000002
	.section .rom.000ba3f8, "ax"
	.global Unit_Calculate
	.type Unit_Calculate, %function
	.thumb_func
Unit_Calculate:
	.incbin "baserom.gba", 0x000ba3f8, 0x000007f4
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
	.section .rom.000bc0e2, "ax"
	.incbin "baserom.gba", 0x000bc0e2, 0x00000002
	.section .rom.000bcbec, "ax"
	.global Func_080afbec
	.type Func_080afbec, %function
	.thumb_func
Func_080afbec:
	.incbin "baserom.gba", 0x000bcbec, 0x000001d0
	.section .rom.000bd084, "ax"
	.global Unit_CalculateBaseElements
	.type Unit_CalculateBaseElements, %function
	.thumb_func
Unit_CalculateBaseElements:
	.incbin "baserom.gba", 0x000bd084, 0x000000c0
	.section .rom.000bd144, "ax"
	.global Class_Determine
	.type Class_Determine, %function
	.thumb_func
Class_Determine:
	.incbin "baserom.gba", 0x000bd144, 0x00000144
	.section .rom.000bdd58, "ax"
	.global DjinnRecovery_Remove
	.type DjinnRecovery_Remove, %function
	.thumb_func
DjinnRecovery_Remove:
	.incbin "baserom.gba", 0x000bdd58, 0x000000ac
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
	.section .rom.000d6e48, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d6e48, 0x000000e4
	.section .rom.000d6fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000d6fd8, 0x0000018c
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
	.global Func_080cb09c
	.type Func_080cb09c, %function
	.thumb_func
Func_080cb09c:
	.incbin "baserom.gba", 0x000d809c, 0x0000062c
	.section .rom.000d891c, "ax"
	.global Field_RunScene
	.type Field_RunScene, %function
	.thumb_func
Field_RunScene:
	.incbin "baserom.gba", 0x000d891c, 0x00000c30
	.section .rom.000d954c, "ax"
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000d954c, 0x00000130
	.section .rom.000d97c4, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000d97c4, 0x000001d0
	.section .rom.000d99fc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000d99fc, 0x000002bc
	.section .rom.000d9d76, "ax"
	.incbin "baserom.gba", 0x000d9d76, 0x00000002
	.section .rom.000d9d78, "ax"
	.global Func_080ccd78
	.type Func_080ccd78, %function
	.thumb_func
Func_080ccd78:
	.incbin "baserom.gba", 0x000d9d78, 0x0000010c
	.section .rom.000d9ec8, "ax"
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
	.incbin "baserom.gba", 0x000d9ec8, 0x000006bc
	.section .rom.000da808, "ax"
	.balign 4
	.global Func_080cd808
	.type Func_080cd808, %function
	.thumb_func
Func_080cd808:
	.incbin "baserom.gba", 0x000da808, 0x00000114
	.section .rom.000daac0, "ax"
	.global Func_080cdac0
Func_080cdac0:
	.incbin "baserom.gba", 0x000daac0, 0x00000138
	.section .rom.000dad80, "ax"
	.global Func_080cdd80
	.type Func_080cdd80, %function
	.thumb_func
Func_080cdd80:
	.incbin "baserom.gba", 0x000dad80, 0x00000128
	.section .rom.000db0ac, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000db0ac, 0x00000270
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
	.section .rom.000dbc34, "ax"
	.incbin "baserom.gba", 0x000dbc34, 0x0000020c
	.section .rom.000dc050, "ax"
	.global Func_080cf050
	.type Func_080cf050, %function
	.thumb_func
Func_080cf050:
	.incbin "baserom.gba", 0x000dc050, 0x00000080
	.section .rom.000dc554, "ax"
	.incbin "baserom.gba", 0x000dc554, 0x000001a8
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
	.section .rom.000ddca0, "ax"
	.global BattlePalette_UpdateBlend
	.type BattlePalette_UpdateBlend, %function
	.thumb_func
BattlePalette_UpdateBlend:
	.incbin "baserom.gba", 0x000ddca0, 0x0000017c
	.section .rom.000dde1c, "ax"
	.global BattleEffect_BuildBuffer
	.type BattleEffect_BuildBuffer, %function
	.thumb_func
BattleEffect_BuildBuffer:
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
	.section .rom.000def20, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000def20, 0x00000164
	.section .rom.000df60c, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000df60c, 0x000001e0
	.section .rom.000e0376, "ax"
	.incbin "baserom.gba", 0x000e0376, 0x00000002
	.section .rom.000e0378, "ax"
	.global Func_080d3378
	.type Func_080d3378, %function
	.thumb_func
Func_080d3378:
	.incbin "baserom.gba", 0x000e0378, 0x000000e8
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
	.section .rom.000e0c88, "ax"
	.balign 4
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000e0c88, 0x00000328
	.section .rom.000e11e6, "ax"
	.incbin "baserom.gba", 0x000e11e6, 0x00000002
	.section .rom.000e140c, "ax"
	.balign 4
	.global Camera_MoveWithinMapBounds
	.type Camera_MoveWithinMapBounds, %function
	.thumb_func
Camera_MoveWithinMapBounds:
	.incbin "baserom.gba", 0x000e140c, 0x00000100
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
	.section .rom.000e2d70, "ax"
	.global Func_080d5d70
	.type Func_080d5d70, %function
	.thumb_func
Func_080d5d70:
	.incbin "baserom.gba", 0x000e2d70, 0x00000070
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
	.global Func_080d64f0
	.type Func_080d64f0, %function
	.thumb_func
Func_080d64f0:
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
	.section .rom.000e39f4, "ax"
	.global Func_080d69f4
	.type Func_080d69f4, %function
	.thumb_func
Func_080d69f4:
	.incbin "baserom.gba", 0x000e39f4, 0x0000019c
	.section .rom.000e4034, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000e4034, 0x0000020c
	.section .rom.000e4524, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000e4524, 0x00000264
	.section .rom.000e484c, "ax"
	.incbin "baserom.gba", 0x000e484c, 0x000000f0
	.section .rom.000e4b04, "ax"
	.global Func_080d7b04
	.type Func_080d7b04, %function
	.thumb_func
Func_080d7b04:
	.incbin "baserom.gba", 0x000e4b04, 0x00000100
	.section .rom.000e4dbc, "ax"
	.global BattleEffect_UpdatePhasedRadialParticle
	.type BattleEffect_UpdatePhasedRadialParticle, %function
	.thumb_func
BattleEffect_UpdatePhasedRadialParticle:
	.incbin "baserom.gba", 0x000e4dbc, 0x000001c4
	.section .rom.000e5174, "ax"
	.incbin "baserom.gba", 0x000e5174, 0x00000078
	.section .rom.000e52e0, "ax"
	.global Func_080d82e0
	.type Func_080d82e0, %function
	.thumb_func
Func_080d82e0:
	.incbin "baserom.gba", 0x000e52e0, 0x0000042c
	.section .rom.000e5740, "ax"
	.global Func_080d8740
	.type Func_080d8740, %function
	.thumb_func
Func_080d8740:
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
	.incbin "baserom.gba", 0x000e6974, 0x00000004
	.section .rom.000e6b80, "ax"
	.global Func_080d9b80
	.type Func_080d9b80, %function
	.thumb_func
Func_080d9b80:
	.incbin "baserom.gba", 0x000e6b80, 0x000001c0
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
	.section .rom.000e7ab0, "ax"
	.balign 4
	.global Func_080daab0
	.type Func_080daab0, %function
	.thumb_func
Func_080daab0:
	.incbin "baserom.gba", 0x000e7ab0, 0x00000208
	.global Func_080dacb8
	.type Func_080dacb8, %function
	.thumb_func
Func_080dacb8:
	.incbin "baserom.gba", 0x000e7cb8, 0x00000184
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
	.section .rom.000e8ed0, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000e8ed0, 0x00000174
	.section .rom.000e9410, "ax"
	.global BattleEffect_LoadActionEffectResources
	.type BattleEffect_LoadActionEffectResources, %function
	.thumb_func
BattleEffect_LoadActionEffectResources:
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
	.section .rom.000e9b44, "ax"
	.balign 4
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
	.section .rom.000ea820, "ax"
	.global Func_080dd820
	.type Func_080dd820, %function
	.thumb_func
Func_080dd820:
	.incbin "baserom.gba", 0x000ea820, 0x00000130
	.section .rom.000ea950, "ax"
	.global Func_080dd950
	.type Func_080dd950, %function
	.thumb_func
Func_080dd950:
	.incbin "baserom.gba", 0x000ea950, 0x000000e0
	.section .rom.000eabd8, "ax"
	.global Func_080ddbd8
	.type Func_080ddbd8, %function
	.thumb_func
Func_080ddbd8:
	.incbin "baserom.gba", 0x000eabd8, 0x000001c8
	.global Func_080ddda0
	.type Func_080ddda0, %function
	.thumb_func
Func_080ddda0:
	.incbin "baserom.gba", 0x000eada0, 0x00000108
	.global Func_080ddea8
	.type Func_080ddea8, %function
	.thumb_func
Func_080ddea8:
	.incbin "baserom.gba", 0x000eaea8, 0x0000012c
	.section .rom.000eb060, "ax"
	.balign 4
	.global Func_080de060
	.type Func_080de060, %function
	.thumb_func
Func_080de060:
	.incbin "baserom.gba", 0x000eb060, 0x000000f4
	.global Func_080de154
	.type Func_080de154, %function
	.thumb_func
Func_080de154:
	.incbin "baserom.gba", 0x000eb154, 0x000000c0
	.section .rom.000eb21c, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000eb21c, 0x00000334
	.section .rom.000eb8d0, "ax"
	.global Func_080de8d0
	.type Func_080de8d0, %function
	.thumb_func
Func_080de8d0:
	.incbin "baserom.gba", 0x000eb8d0, 0x0000011c
	.section .rom.000ebcb8, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000ebcb8, 0x000000cc
	.section .rom.000ebde8, "ax"
	.global FieldEffect_RunDouse
FieldEffect_RunDouse:
	.incbin "baserom.gba", 0x000ebde8, 0x0000038c
	.section .rom.000ec1fc, "ax"
	.global Func_080df1fc
	.type Func_080df1fc, %function
	.thumb_func
Func_080df1fc:
	.incbin "baserom.gba", 0x000ec1fc, 0x000004f8
	.global Func_080df6f4
	.type Func_080df6f4, %function
	.thumb_func
Func_080df6f4:
	.incbin "baserom.gba", 0x000ec6f4, 0x0000012c
	.section .rom.000ec8de, "ax"
	.incbin "baserom.gba", 0x000ec8de, 0x00000002
	.section .rom.000ec8e0, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000ec8e0, 0x000001d4
	.section .rom.000ecd00, "ax"
	.global FieldEffect_RunWhirlwind
FieldEffect_RunWhirlwind:
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
	.section .rom.000edcec, "ax"
	.global Func_080e0cec
	.type Func_080e0cec, %function
	.thumb_func
Func_080e0cec:
	.incbin "baserom.gba", 0x000edcec, 0x000000e8
	.section .rom.000edf40, "ax"
	.incbin "baserom.gba", 0x000edf40, 0x000000e4
	.global Func_080e1024
	.type Func_080e1024, %function
	.thumb_func
Func_080e1024:
	.incbin "baserom.gba", 0x000ee024, 0x00000130
	.section .rom.000ee448, "ax"
	.global BattleEffect_SpawnScaledArcObjects
	.type BattleEffect_SpawnScaledArcObjects, %function
	.thumb_func
BattleEffect_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000ee448, 0x000000c4
	.section .rom.000ee50c, "ax"
	.global BattleEffect_UpdateArcEffect
	.type BattleEffect_UpdateArcEffect, %function
	.thumb_func
BattleEffect_UpdateArcEffect:
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
	.incbin "baserom.gba", 0x000ef8d4, 0x00000194
	.section .rom.000efacc, "ax"
	.incbin "baserom.gba", 0x000efacc, 0x0000016c
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
	.section .rom.000f08c8, "ax"
	.incbin "baserom.gba", 0x000f08c8, 0x00000434
	.section .rom.000f0d04, "ax"
	.global Func_080e3d04
	.type Func_080e3d04, %function
	.thumb_func
Func_080e3d04:
	.incbin "baserom.gba", 0x000f0d04, 0x00000540
	.section .rom.000f1244, "ax"
	.global Func_080e4244
	.type Func_080e4244, %function
	.thumb_func
Func_080e4244:
	.incbin "baserom.gba", 0x000f1244, 0x00000090
	.section .rom.000f146c, "ax"
	.incbin "baserom.gba", 0x000f146c, 0x00000134
	.section .rom.000f15a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f15a8, 0x00000104
	.section .rom.000f1730, "ax"
	.balign 4
	.global Func_080e4730
	.type Func_080e4730, %function
	.thumb_func
Func_080e4730:
	.incbin "baserom.gba", 0x000f1730, 0x000002fc
	.section .rom.000f1a34, "ax"
	.global Func_080e4a34
	.type Func_080e4a34, %function
	.thumb_func
Func_080e4a34:
	.incbin "baserom.gba", 0x000f1a34, 0x00000cf0
	.section .rom.000f272c, "ax"
	.global Func_080e572c
	.type Func_080e572c, %function
	.thumb_func
Func_080e572c:
	.incbin "baserom.gba", 0x000f272c, 0x000005c4
	.section .rom.000f2d5c, "ax"
	.global Func_080e5d5c
	.type Func_080e5d5c, %function
	.thumb_func
Func_080e5d5c:
	.incbin "baserom.gba", 0x000f2d5c, 0x0000062c
	.section .rom.000f3400, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f3400, 0x000004c8
	.section .rom.000f38d0, "ax"
	.global Func_080e68d0
	.type Func_080e68d0, %function
	.thumb_func
Func_080e68d0:
	.incbin "baserom.gba", 0x000f38d0, 0x00000828
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
	.section .rom.000f4844, "ax"
	.global Func_080e7844
	.type Func_080e7844, %function
	.thumb_func
Func_080e7844:
	.incbin "baserom.gba", 0x000f4844, 0x00000750
	.section .rom.000f507c, "ax"
	.incbin "baserom.gba", 0x000f507c, 0x00000250
	.global Func_080e82cc
	.type Func_080e82cc, %function
	.thumb_func
Func_080e82cc:
	.incbin "baserom.gba", 0x000f52cc, 0x000004f8
	.section .rom.000f57c4, "ax"
	.global Func_080e87c4
	.type Func_080e87c4, %function
	.thumb_func
Func_080e87c4:
	.incbin "baserom.gba", 0x000f57c4, 0x00000220
	.global Func_080e89e4
	.type Func_080e89e4, %function
	.thumb_func
Func_080e89e4:
	.incbin "baserom.gba", 0x000f59e4, 0x00000160
	.section .rom.000f5c9c, "ax"
	.global Func_080e8c9c
	.type Func_080e8c9c, %function
	.thumb_func
Func_080e8c9c:
	.incbin "baserom.gba", 0x000f5c9c, 0x00000060
	.section .rom.000f5db0, "ax"
	.balign 4
	.global Func_080e8db0
	.type Func_080e8db0, %function
	.thumb_func
Func_080e8db0:
	.incbin "baserom.gba", 0x000f5db0, 0x000002e0
	.global Func_080e9090
	.type Func_080e9090, %function
	.thumb_func
Func_080e9090:
	.incbin "baserom.gba", 0x000f6090, 0x000001c0
	.section .rom.000f62b4, "ax"
	.balign 4
	.global Func_080e92b4
	.type Func_080e92b4, %function
	.thumb_func
Func_080e92b4:
	.incbin "baserom.gba", 0x000f62b4, 0x00000514
	.section .rom.000f6940, "ax"
	.incbin "baserom.gba", 0x000f6940, 0x000001ac
	.section .rom.000f6af4, "ax"
	.global Func_080e9af4
	.type Func_080e9af4, %function
	.thumb_func
Func_080e9af4:
	.incbin "baserom.gba", 0x000f6af4, 0x000002c0
	.section .rom.000f6db4, "ax"
	.global Func_080e9db4
	.type Func_080e9db4, %function
	.thumb_func
Func_080e9db4:
	.incbin "baserom.gba", 0x000f6db4, 0x000000d8
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
	.section .rom.000f801c, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000f801c, 0x0000027c
	.section .rom.000f8824, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000f8824, 0x0000010c
	.section .rom.000f8960, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000f8960, 0x000002d0
	.section .rom.000f8f94, "ax"
	.incbin "baserom.gba", 0x000f8f94, 0x000001b8
	.section .rom.000f916c, "ax"
	.incbin "baserom.gba", 0x000f916c, 0x00000064
	.section .rom.000f91d0, "ax"
	.incbin "baserom.gba", 0x000f91d0, 0x000000c8
	.section .rom.000f92b8, "ax"
	.incbin "baserom.gba", 0x000f92b8, 0x00000cbc
	.section .rom.000fa2a4, "ax"
	.incbin "baserom.gba", 0x000fa2a4, 0x000004e4
	.section .rom.000fc81c, "ax"
	.incbin "baserom.gba", 0x000fc81c, 0x00000008
	.section .rom.00100984, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x00100984, 0x00000040
	.section .rom.00102170, "ax"
	.global Func_080f8170
	.type Func_080f8170, %function
	.thumb_func
Func_080f8170:
	.incbin "baserom.gba", 0x00102170, 0x000004a0
	.section .rom.001029a4, "ax"
	.balign 4
	.global Func_080f89a4
	.type Func_080f89a4, %function
	.thumb_func
Func_080f89a4:
	.incbin "baserom.gba", 0x001029a4, 0x000000a0
	.section .rom.00102f9c, "ax"
	.incbin "baserom.gba", 0x00102f9c, 0x0000016c
	.section .rom.001033b6, "ax"
	.incbin "baserom.gba", 0x001033b6, 0x00000002
	.section .rom.001034a2, "ax"
	.incbin "baserom.gba", 0x001034a2, 0x00000002
	.section .rom.001034a4, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x001034a4, 0x00000e38
	.section .rom.00104484, "ax"
	.incbin "baserom.gba", 0x00104484, 0x0000003c
	.section .rom.001048b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001048b8, 0x00000370
	.section .rom.00104d1e, "ax"
	.incbin "baserom.gba", 0x00104d1e, 0x00000002
	.section .rom.00104ed4, "ax"
	.incbin "baserom.gba", 0x00104ed4, 0x00000218
	.section .rom.0010514c, "ax"
	.incbin "baserom.gba", 0x0010514c, 0x00000460
	.section .rom.00105f30, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00105f30, 0x00000340
	.section .rom.001063a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001063a4, 0x000001a4
	.section .rom.00106784, "ax"
	.incbin "baserom.gba", 0x00106784, 0x00000430
	.section .rom.00106bb4, "ax"
	.global Func_080fcab8
	.type Func_080fcab8, %function
	.thumb_func
Func_080fcab8:
	.incbin "baserom.gba", 0x00106bb4, 0x00000198
	.section .rom.00107174, "ax"
	.global Func_080fd000
	.type Func_080fd000, %function
	.thumb_func
Func_080fd000:
	.incbin "baserom.gba", 0x00107174, 0x00000294
	.section .rom.0010745c, "ax"
	.global Func_080fd2e8
	.type Func_080fd2e8, %function
	.thumb_func
Func_080fd2e8:
	.incbin "baserom.gba", 0x0010745c, 0x00000244
	.section .rom.001076d0, "ax"
	.incbin "baserom.gba", 0x001076d0, 0x00000188
	.section .rom.00107898, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x00107898, 0x0000011c
	.section .rom.00107a48, "ax"
	.incbin "baserom.gba", 0x00107a48, 0x000000cc
	.section .rom.00107c80, "ax"
	.global Func_080fdad4
	.type Func_080fdad4, %function
	.thumb_func
Func_080fdad4:
	.incbin "baserom.gba", 0x00107c80, 0x00000690
	.section .rom.00108420, "ax"
	.balign 4
	.global Func_080fe274
	.type Func_080fe274, %function
	.thumb_func
Func_080fe274:
	.incbin "baserom.gba", 0x00108420, 0x00000228
	.section .rom.001087e4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001087e4, 0x0000025c
	.global Func_080fe894
	.type Func_080fe894, %function
	.thumb_func
Func_080fe894:
	.incbin "baserom.gba", 0x00108a40, 0x000002c4
	.global Func_080feb58
	.type Func_080feb58, %function
	.thumb_func
Func_080feb58:
	.incbin "baserom.gba", 0x00108d04, 0x000002ac
	.section .rom.0010906c, "ax"
	.balign 4
	.global Func_080feec0
	.type Func_080feec0, %function
	.thumb_func
Func_080feec0:
	.incbin "baserom.gba", 0x0010906c, 0x00000334
	.section .rom.0010951c, "ax"
	.global Func_080ff370
	.type Func_080ff370, %function
	.thumb_func
Func_080ff370:
	.incbin "baserom.gba", 0x0010951c, 0x0000024c
	.section .rom.00109af8, "ax"
	.incbin "baserom.gba", 0x00109af8, 0x00000210
	.section .rom.00109e80, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x00109e80, 0x00000230
	.section .rom.0010a3e8, "ax"
	.global Func_0810021c
	.type Func_0810021c, %function
	.thumb_func
Func_0810021c:
	.incbin "baserom.gba", 0x0010a3e8, 0x0000029c
	.section .rom.0010a904, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x0010a904, 0x000000c0
	.global BattleEffect_ApplyToTargets
	.type BattleEffect_ApplyToTargets, %function
	.thumb_func
BattleEffect_ApplyToTargets:
	.incbin "baserom.gba", 0x0010a9c4, 0x00000548
	.section .rom.0010b238, "ax"
	.global Func_0810106c
	.type Func_0810106c, %function
	.thumb_func
Func_0810106c:
	.incbin "baserom.gba", 0x0010b238, 0x00000544
	.section .rom.0010b804, "ax"
	.incbin "baserom.gba", 0x0010b804, 0x0000036c
	.section .rom.0010bc94, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x0010bc94, 0x00000144
	.section .rom.0010bf28, "ax"
	.incbin "baserom.gba", 0x0010bf28, 0x00001308
	.global Func_08103064
	.type Func_08103064, %function
	.thumb_func
Func_08103064:
	.incbin "baserom.gba", 0x0010d230, 0x00000104
	.section .rom.0010d3e2, "ax"
	.incbin "baserom.gba", 0x0010d3e2, 0x00000002
	.global Func_08103218
	.type Func_08103218, %function
	.thumb_func
Func_08103218:
	.incbin "baserom.gba", 0x0010d3e4, 0x00001710
	.section .rom.0010eb58, "ax"
	.balign 4
	.global Func_0810498c
	.type Func_0810498c, %function
	.thumb_func
Func_0810498c:
	.incbin "baserom.gba", 0x0010eb58, 0x0000003c
	.section .rom.0010f004, "ax"
	.incbin "baserom.gba", 0x0010f004, 0x000000c0
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x0010f0c4, 0x000000e8
	.section .rom.0010f298, "ax"
	.global MenuIcon_UpdateTransforms
	.type MenuIcon_UpdateTransforms, %function
	.thumb_func
MenuIcon_UpdateTransforms:
	.incbin "baserom.gba", 0x0010f298, 0x000000dc
	.section .rom.0010f374, "ax"
	.global Func_081051a8
	.type Func_081051a8, %function
	.thumb_func
Func_081051a8:
	.incbin "baserom.gba", 0x0010f374, 0x000000c4
	.section .rom.0010f6ac, "ax"
	.incbin "baserom.gba", 0x0010f6ac, 0x000000a0
	.section .rom.00110b70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x00110b70, 0x00000618
	.section .rom.0011128c, "ax"
	.incbin "baserom.gba", 0x0011128c, 0x00000324
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
	.section .rom.0011210c, "ax"
	.global Func_0810a108
	.type Func_0810a108, %function
	.thumb_func
Func_0810a108:
	.incbin "baserom.gba", 0x0011210c, 0x000001a4
	.section .rom.001122dc, "ax"
	.incbin "baserom.gba", 0x001122dc, 0x00000394
	.section .rom.00112bf4, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x00112bf4, 0x000002b4
	.section .rom.0011304e, "ax"
	.incbin "baserom.gba", 0x0011304e, 0x00000002
	.section .rom.001131b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001131b8, 0x000001c4
	.section .rom.001135c8, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x001135c8, 0x000000f8
	.section .rom.001137b8, "ax"
	.balign 4
	.global Func_0810b7b4
	.type Func_0810b7b4, %function
	.thumb_func
Func_0810b7b4:
	.incbin "baserom.gba", 0x001137b8, 0x0000060c
	.section .rom.00113e08, "ax"
	.global Func_0810bdf4
	.type Func_0810bdf4, %function
	.thumb_func
Func_0810bdf4:
	.incbin "baserom.gba", 0x00113e08, 0x00000048
	.section .rom.00118230, "ax"
	.incbin "baserom.gba", 0x00118230, 0x00000008
	.section .rom.00118244, "ax"
	.incbin "baserom.gba", 0x00118244, 0x00000040
	.section .rom.0011843c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011843c, 0x00000188
	.section .rom.00118738, "ax"
	.balign 4
	.global DebugParty_LoadPreset
	.type DebugParty_LoadPreset, %function
	.thumb_func
DebugParty_LoadPreset:
	.incbin "baserom.gba", 0x00118738, 0x0000021c
	.section .rom.00118958, "ax"
	.balign 4
	.global Func_08118958
	.type Func_08118958, %function
	.thumb_func
Func_08118958:
	.incbin "baserom.gba", 0x00118958, 0x00000274
	.section .rom.00119052, "ax"
	.incbin "baserom.gba", 0x00119052, 0x00000002
	.section .rom.00119604, "ax"
	.incbin "baserom.gba", 0x00119604, 0x00000110
	.section .rom.00119808, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x00119808, 0x00000848
	.section .rom.0011a7c4, "ax"
	.global BattleObject_Make
	.type BattleObject_Make, %function
	.thumb_func
BattleObject_Make:
	.incbin "baserom.gba", 0x0011a7c4, 0x000009c0
	.section .rom.0011b392, "ax"
	.incbin "baserom.gba", 0x0011b392, 0x00000002
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
	.section .rom.0011c54e, "ax"
	.incbin "baserom.gba", 0x0011c54e, 0x00000002
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
	.incbin "baserom.gba", 0x0011d9e4, 0x0000040c
	.global Func_0811ddd8
	.type Func_0811ddd8, %function
	.thumb_func
Func_0811ddd8:
	.incbin "baserom.gba", 0x0011ddf0, 0x000000a0
	.section .rom.0011df88, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011df88, 0x000003fc
	.section .rom.0011e3c2, "ax"
	.incbin "baserom.gba", 0x0011e3c2, 0x00000002
	.global Func_0811e3ac
	.type Func_0811e3ac, %function
	.thumb_func
Func_0811e3ac:
	.incbin "baserom.gba", 0x0011e3c4, 0x00000430
	.section .rom.0011e848, "ax"
	.balign 4
	.global Func_0811e830
	.type Func_0811e830, %function
	.thumb_func
Func_0811e830:
	.incbin "baserom.gba", 0x0011e848, 0x00000580
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
	.section .rom.0012046c, "ax"
	.global BattleTarget_Execute
	.type BattleTarget_Execute, %function
	.thumb_func
BattleTarget_Execute:
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
	.section .rom.00124e38, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00124e38, 0x00000c80
	.section .rom.00125bd0, "ax"
	.global Func_08125bb8
	.type Func_08125bb8, %function
	.thumb_func
Func_08125bb8:
	.incbin "baserom.gba", 0x00125bd0, 0x00000054
	.section .rom.00125d8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00125d8c, 0x000004a4
	.section .rom.00126414, "ax"
	.balign 4
	.global Func_081263fc
	.type Func_081263fc, %function
	.thumb_func
Func_081263fc:
	.incbin "baserom.gba", 0x00126414, 0x0000014c
	.global BattleCamera_SetRange
	.type BattleCamera_SetRange, %function
	.thumb_func
BattleCamera_SetRange:
	.incbin "baserom.gba", 0x00126560, 0x000002bc
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
	.global Func_08127308
	.type Func_08127308, %function
	.thumb_func
Func_08127308:
	.incbin "baserom.gba", 0x00127320, 0x00000264
	.section .rom.00127af4, "ax"
	.global BattleEnemy_ClaimNameLetter
	.type BattleEnemy_ClaimNameLetter, %function
	.thumb_func
BattleEnemy_ClaimNameLetter:
	.incbin "baserom.gba", 0x00127af4, 0x000000c4
	.section .rom.00127cec, "ax"
	.incbin "baserom.gba", 0x00127cec, 0x000003cc
	.section .rom.001284d8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001284d8, 0x00000304
	.section .rom.0013bbd8, "ax"
	.global BattleEffect_RunIce
	.type BattleEffect_RunIce, %function
	.thumb_func
BattleEffect_RunIce:
	.incbin "baserom.gba", 0x0013bbd8, 0x000004e0
	.global BattleEffect_RunDouse
	.type BattleEffect_RunDouse, %function
	.thumb_func
BattleEffect_RunDouse:
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
	.global BattleEffect_RunCruelRuin
	.type BattleEffect_RunCruelRuin, %function
	.thumb_func
BattleEffect_RunCruelRuin:
	.incbin "baserom.gba", 0x0013fd84, 0x000017b8
	.global BattleEffect_RunEclipse
	.type BattleEffect_RunEclipse, %function
	.thumb_func
BattleEffect_RunEclipse:
	.incbin "baserom.gba", 0x0014153c, 0x00001408
	.section .rom.00142944, "ax"
	.global Func_08142944
	.type Func_08142944, %function
	.thumb_func
Func_08142944:
	.incbin "baserom.gba", 0x00142944, 0x000006bc
	.section .rom.00144044, "ax"
	.balign 4
	.global BattleEffect_RunHaunt
	.type BattleEffect_RunHaunt, %function
	.thumb_func
BattleEffect_RunHaunt:
	.incbin "baserom.gba", 0x00144044, 0x00000324
	.global BattleEffect_RunOrbitingSpirals
	.type BattleEffect_RunOrbitingSpirals, %function
	.thumb_func
BattleEffect_RunOrbitingSpirals:
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
	.global BattleEffect_RunCondemn
	.type BattleEffect_RunCondemn, %function
	.thumb_func
BattleEffect_RunCondemn:
	.incbin "baserom.gba", 0x00145784, 0x000004e4
	.section .rom.00145df8, "ax"
	.global BattleEffect_RunBind
	.type BattleEffect_RunBind, %function
	.thumb_func
BattleEffect_RunBind:
	.incbin "baserom.gba", 0x00145df8, 0x00000484
	.global BattleEffect_RunTriangleStrike
	.type BattleEffect_RunTriangleStrike, %function
	.thumb_func
BattleEffect_RunTriangleStrike:
	.incbin "baserom.gba", 0x0014627c, 0x000003bc
	.global BattleEffect_RunAstralBlast
	.type BattleEffect_RunAstralBlast, %function
	.thumb_func
BattleEffect_RunAstralBlast:
	.incbin "baserom.gba", 0x00146638, 0x00000434
	.global BattleEffect_RunShiningStar
	.type BattleEffect_RunShiningStar, %function
	.thumb_func
BattleEffect_RunShiningStar:
	.incbin "baserom.gba", 0x00146a6c, 0x00000394
	.global BattleEffect_RunProcne
	.type BattleEffect_RunProcne, %function
	.thumb_func
BattleEffect_RunProcne:
	.incbin "baserom.gba", 0x00146e00, 0x00000cd4
	.section .rom.00147aec, "ax"
	.balign 4
	.global Func_08147aec
	.type Func_08147aec, %function
	.thumb_func
Func_08147aec:
	.incbin "baserom.gba", 0x00147aec, 0x0000096c
	.global BattleEffect_RunNereid
	.type BattleEffect_RunNereid, %function
	.thumb_func
BattleEffect_RunNereid:
	.incbin "baserom.gba", 0x00148458, 0x000005fc
	.global BattleEffect_RunFroth
	.type BattleEffect_RunFroth, %function
	.thumb_func
BattleEffect_RunFroth:
	.incbin "baserom.gba", 0x00148a54, 0x00000448
	.global BattleEffect_RunWhirlwind
	.type BattleEffect_RunWhirlwind, %function
	.thumb_func
BattleEffect_RunWhirlwind:
	.incbin "baserom.gba", 0x00148e9c, 0x000003cc
	.global BattleEffect_RunPrism
	.type BattleEffect_RunPrism, %function
	.thumb_func
BattleEffect_RunPrism:
	.incbin "baserom.gba", 0x00149268, 0x0000052c
	.global BattleEffect_RunPlasma
	.type BattleEffect_RunPlasma, %function
	.thumb_func
BattleEffect_RunPlasma:
	.incbin "baserom.gba", 0x00149794, 0x00000400
	.section .rom.00149bac, "ax"
	.balign 4
	.global BattleEffect_RunSparkGroups
	.type BattleEffect_RunSparkGroups, %function
	.thumb_func
BattleEffect_RunSparkGroups:
	.incbin "baserom.gba", 0x00149bac, 0x000006b8
	.global BattleEffect_RunVolcano
	.type BattleEffect_RunVolcano, %function
	.thumb_func
BattleEffect_RunVolcano:
	.incbin "baserom.gba", 0x0014a264, 0x00000544
	.section .rom.0014a812, "ax"
	.incbin "baserom.gba", 0x0014a812, 0x00000002
	.section .rom.0014a814, "ax"
	.global BattleEffect_RunTargetMotes
	.type BattleEffect_RunTargetMotes, %function
	.thumb_func
BattleEffect_RunTargetMotes:
	.incbin "baserom.gba", 0x0014a814, 0x000006a8
	.global BattleEffect_RunSleep
	.type BattleEffect_RunSleep, %function
	.thumb_func
BattleEffect_RunSleep:
	.incbin "baserom.gba", 0x0014aebc, 0x000002a4
	.global BattleEffect_RunCurse
	.type BattleEffect_RunCurse, %function
	.thumb_func
BattleEffect_RunCurse:
	.incbin "baserom.gba", 0x0014b160, 0x00000864
	.section .rom.0014b9e8, "ax"
	.balign 4
	.global Func_0814b9e8
	.type Func_0814b9e8, %function
	.thumb_func
Func_0814b9e8:
	.incbin "baserom.gba", 0x0014b9e8, 0x000004f8
	.global BattleEffect_RunZagan
	.type BattleEffect_RunZagan, %function
	.thumb_func
BattleEffect_RunZagan:
	.incbin "baserom.gba", 0x0014bee0, 0x00000a48
	.section .rom.0014cca8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0014cca8, 0x000000a0
	.section .rom.0014ce30, "ax"
	.balign 4
	.global BattleEffect_RunCybele
	.type BattleEffect_RunCybele, %function
	.thumb_func
BattleEffect_RunCybele:
	.incbin "baserom.gba", 0x0014ce30, 0x00000cd4
	.global BattleEffect_RunBoreas
	.type BattleEffect_RunBoreas, %function
	.thumb_func
BattleEffect_RunBoreas:
	.incbin "baserom.gba", 0x0014db04, 0x00000c40
	.global BattleEffect_RunBreak
	.type BattleEffect_RunBreak, %function
	.thumb_func
BattleEffect_RunBreak:
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
	.global BattleEffect_RunHealingMotes
	.type BattleEffect_RunHealingMotes, %function
	.thumb_func
BattleEffect_RunHealingMotes:
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
	.global SummonEffect_RunCanvasEffect
SummonEffect_RunCanvasEffect:
	.incbin "baserom.gba", 0x001504d8, 0x000004dc
	.section .rom.00150c74, "ax"
	.global BattleEffect_RunQuake
	.type BattleEffect_RunQuake, %function
	.thumb_func
BattleEffect_RunQuake:
	.incbin "baserom.gba", 0x00150c74, 0x000003cc
	.global BattleEffect_RunFire
	.type BattleEffect_RunFire, %function
	.thumb_func
BattleEffect_RunFire:
	.incbin "baserom.gba", 0x00151040, 0x0000047c
	.global BattleEffect_RunFrost
	.type BattleEffect_RunFrost, %function
	.thumb_func
BattleEffect_RunFrost:
	.incbin "baserom.gba", 0x001514bc, 0x0000068c
	.global BattleEffect_RunRay
	.type BattleEffect_RunRay, %function
	.thumb_func
BattleEffect_RunRay:
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
	.global BattleEffect_RunProjectileVolleyB
	.type BattleEffect_RunProjectileVolleyB, %function
	.thumb_func
BattleEffect_RunProjectileVolleyB:
	.incbin "baserom.gba", 0x0015265c, 0x00001860
	.global BattleEffect_RunAtalanta
	.type BattleEffect_RunAtalanta, %function
	.thumb_func
BattleEffect_RunAtalanta:
	.incbin "baserom.gba", 0x00153ebc, 0x000009fc
	.section .rom.001548d0, "ax"
	.balign 4
	.global Func_081548d0
	.type Func_081548d0, %function
	.thumb_func
Func_081548d0:
	.incbin "baserom.gba", 0x001548d0, 0x0000048c
	.section .rom.00154d5c, "ax"
	.global BattleEffect_RunVine
	.type BattleEffect_RunVine, %function
	.thumb_func
BattleEffect_RunVine:
	.incbin "baserom.gba", 0x00154d5c, 0x00000228
	.global BattleEffect_RunThorn
	.type BattleEffect_RunThorn, %function
	.thumb_func
BattleEffect_RunThorn:
	.incbin "baserom.gba", 0x00154f84, 0x000003f8
	.global BattleEffect_RunBolt
	.type BattleEffect_RunBolt, %function
	.thumb_func
BattleEffect_RunBolt:
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
	.global BattleEffect_RunProjectileVolley
	.type BattleEffect_RunProjectileVolley, %function
	.thumb_func
BattleEffect_RunProjectileVolley:
	.incbin "baserom.gba", 0x00156140, 0x000013a0
	.section .rom.00157636, "ax"
	.incbin "baserom.gba", 0x00157636, 0x00000002
	.global BattleEffect_RunMercury
	.type BattleEffect_RunMercury, %function
	.thumb_func
BattleEffect_RunMercury:
	.incbin "baserom.gba", 0x00157638, 0x00000398
	.global BattleEffect_RunJupiter
	.type BattleEffect_RunJupiter, %function
	.thumb_func
BattleEffect_RunJupiter:
	.incbin "baserom.gba", 0x001579d0, 0x00000324
	.section .rom.00157d30, "ax"
	.global BattleEffect_RunVenus
	.type BattleEffect_RunVenus, %function
	.thumb_func
BattleEffect_RunVenus:
	.incbin "baserom.gba", 0x00157d30, 0x0000033c
	.global BattleEffect_RunMars
	.type BattleEffect_RunMars, %function
	.thumb_func
BattleEffect_RunMars:
	.incbin "baserom.gba", 0x0015806c, 0x000003c0
	.global BattleEffect_RunHail
	.type BattleEffect_RunHail, %function
	.thumb_func
BattleEffect_RunHail:
	.incbin "baserom.gba", 0x0015842c, 0x000003b8
	.global BattleEffect_RunGround
	.type BattleEffect_RunGround, %function
	.thumb_func
BattleEffect_RunGround:
	.incbin "baserom.gba", 0x001587e4, 0x000004fc
	.section .rom.00158dc8, "ax"
	.balign 4
	.global BattleEffect_RunThor
	.type BattleEffect_RunThor, %function
	.thumb_func
BattleEffect_RunThor:
	.incbin "baserom.gba", 0x00158dc8, 0x00000ee4
	.global BattleEffect_RunSpire
	.type BattleEffect_RunSpire, %function
	.thumb_func
BattleEffect_RunSpire:
	.incbin "baserom.gba", 0x00159cac, 0x00000398
	.section .rom.0015a110, "ax"
	.global BattleEffect_RunBurstScene
	.type BattleEffect_RunBurstScene, %function
	.thumb_func
BattleEffect_RunBurstScene:
	.incbin "baserom.gba", 0x0015a110, 0x000008d4
	.global BattleEffect_RunGaia
	.type BattleEffect_RunGaia, %function
	.thumb_func
BattleEffect_RunGaia:
	.incbin "baserom.gba", 0x0015a9e4, 0x00000868
	.section .rom.0015b434, "ax"
	.incbin "baserom.gba", 0x0015b434, 0x00000330
	.global BattleEffect_RunUlysses
	.type BattleEffect_RunUlysses, %function
	.thumb_func
BattleEffect_RunUlysses:
	.incbin "baserom.gba", 0x0015b764, 0x000011f4
	.section .rom.0015c970, "ax"
	.balign 4
	.global Func_0815c970
	.type Func_0815c970, %function
	.thumb_func
Func_0815c970:
	.incbin "baserom.gba", 0x0015c970, 0x00000260
	.section .rom.0015cbd0, "ax"
	.global BattleEffect_RunSpiderWeb
	.type BattleEffect_RunSpiderWeb, %function
	.thumb_func
BattleEffect_RunSpiderWeb:
	.incbin "baserom.gba", 0x0015cbd0, 0x00000218
	.global BattleEffect_RunAzul
	.type BattleEffect_RunAzul, %function
	.thumb_func
BattleEffect_RunAzul:
	.incbin "baserom.gba", 0x0015cde8, 0x00001388
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
	.section .rom.0015f16c, "ax"
	.balign 4
	.global Func_0815f16c
	.type Func_0815f16c, %function
	.thumb_func
Func_0815f16c:
	.incbin "baserom.gba", 0x0015f16c, 0x00004764
	.section .rom.00163c2c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163c2c, 0x0000015c
	.section .rom.00163e14, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00163e14, 0x00000618
	.section .rom.0016467c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0016467c, 0x000003d0
	.section .rom.00164c0a, "ax"
	.incbin "baserom.gba", 0x00164c0a, 0x000000ba
	.section .rom.00164cc4, "ax"
	.global BattleEffect_RunParticleStreams
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x00164cc4, 0x00000df0
	.global BattleEffect_RunRamses
	.type BattleEffect_RunRamses, %function
	.thumb_func
BattleEffect_RunRamses:
	.incbin "baserom.gba", 0x00165ab4, 0x00000728
	.global BattleEffect_RunAnnihilation
	.type BattleEffect_RunAnnihilation, %function
	.thumb_func
BattleEffect_RunAnnihilation:
	.incbin "baserom.gba", 0x001661dc, 0x000003c8
	.global BattleEffect_RunRagnarok
	.type BattleEffect_RunRagnarok, %function
	.thumb_func
BattleEffect_RunRagnarok:
	.incbin "baserom.gba", 0x001665a4, 0x00000554
	.section .rom.00166b10, "ax"
	.balign 4
	.global Func_08166b10
	.type Func_08166b10, %function
	.thumb_func
Func_08166b10:
	.incbin "baserom.gba", 0x00166b10, 0x0000078c
	.global BattleEffect_RunJudgment
	.type BattleEffect_RunJudgment, %function
	.thumb_func
BattleEffect_RunJudgment:
	.incbin "baserom.gba", 0x0016729c, 0x000017a4
	.global BattleEffect_RunKirin
	.type BattleEffect_RunKirin, %function
	.thumb_func
BattleEffect_RunKirin:
	.incbin "baserom.gba", 0x00168a40, 0x00000970
	.section .rom.001693d0, "ax"
	.global BattleEffect_RunNeptune
	.type BattleEffect_RunNeptune, %function
	.thumb_func
BattleEffect_RunNeptune:
	.incbin "baserom.gba", 0x001693d0, 0x00000e48
	.section .rom.0016a2d8, "ax"
	.balign 4
	.global Func_0816a2d8
Func_0816a2d8:
	.incbin "baserom.gba", 0x0016a2d8, 0x00000458
	.global BattleEffect_RunUndeadSword
	.type BattleEffect_RunUndeadSword, %function
	.thumb_func
BattleEffect_RunUndeadSword:
	.incbin "baserom.gba", 0x0016a730, 0x000002d8
	.global BattleEffect_RunPlumeEdge
	.type BattleEffect_RunPlumeEdge, %function
	.thumb_func
BattleEffect_RunPlumeEdge:
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
	.global BattleEffect_RunDiamondBerg
	.type BattleEffect_RunDiamondBerg, %function
	.thumb_func
BattleEffect_RunDiamondBerg:
	.incbin "baserom.gba", 0x0016bc70, 0x00000604
	.global BattleEffect_RunCrystal
	.type BattleEffect_RunCrystal, %function
	.thumb_func
BattleEffect_RunCrystal:
	.incbin "baserom.gba", 0x0016c274, 0x00000484
	.global BattleEffect_RunGeode
	.type BattleEffect_RunGeode, %function
	.thumb_func
BattleEffect_RunGeode:
	.incbin "baserom.gba", 0x0016c6f8, 0x00000a38
	.global BattleEffect_RunNirvana
	.type BattleEffect_RunNirvana, %function
	.thumb_func
BattleEffect_RunNirvana:
	.incbin "baserom.gba", 0x0016d130, 0x000002a8
	.global BattleEffect_RunStoneJustice
	.type BattleEffect_RunStoneJustice, %function
	.thumb_func
BattleEffect_RunStoneJustice:
	.incbin "baserom.gba", 0x0016d3d8, 0x0000034c
	.global BattleEffect_RunSoulShatter
	.type BattleEffect_RunSoulShatter, %function
	.thumb_func
BattleEffect_RunSoulShatter:
	.incbin "baserom.gba", 0x0016d724, 0x0000052c
	.global BattleEffect_RunSerac
	.type BattleEffect_RunSerac, %function
	.thumb_func
BattleEffect_RunSerac:
	.incbin "baserom.gba", 0x0016dc50, 0x000009f4
	.section .rom.0016e65c, "ax"
	.balign 4
	.global Func_0816e65c
Func_0816e65c:
	.incbin "baserom.gba", 0x0016e65c, 0x000005fc
	.global BattleEffect_RunCool
	.type BattleEffect_RunCool, %function
	.thumb_func
BattleEffect_RunCool:
	.incbin "baserom.gba", 0x0016ec58, 0x00000428
	.global BattleEffect_RunWyrdCurse
	.type BattleEffect_RunWyrdCurse, %function
	.thumb_func
BattleEffect_RunWyrdCurse:
	.incbin "baserom.gba", 0x0016f080, 0x00000514
	.global BattleEffect_RunRadiantFire
	.type BattleEffect_RunRadiantFire, %function
	.thumb_func
BattleEffect_RunRadiantFire:
	.incbin "baserom.gba", 0x0016f594, 0x00000394
	.global BattleEffect_RunHurricane
	.type BattleEffect_RunHurricane, %function
	.thumb_func
BattleEffect_RunHurricane:
	.incbin "baserom.gba", 0x0016f928, 0x00000368
	.section .rom.0016fca8, "ax"
	.balign 4
	.global Func_0816fca8
Func_0816fca8:
	.incbin "baserom.gba", 0x0016fca8, 0x000006bc
	.global BattleEffect_RunMegaera
	.type BattleEffect_RunMegaera, %function
	.thumb_func
BattleEffect_RunMegaera:
	.incbin "baserom.gba", 0x00170364, 0x00000a48
	.global BattleEffect_RunVoidBeam
	.type BattleEffect_RunVoidBeam, %function
	.thumb_func
BattleEffect_RunVoidBeam:
	.incbin "baserom.gba", 0x00170dac, 0x00000684
	.global BattleEffect_RunLightSurge
	.type BattleEffect_RunLightSurge, %function
	.thumb_func
BattleEffect_RunLightSurge:
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
	.global BattleEffect_RunCharon
	.type BattleEffect_RunCharon, %function
	.thumb_func
BattleEffect_RunCharon:
	.incbin "baserom.gba", 0x00175f74, 0x0000270c
	.section .rom.00178710, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragon
	.type BattleEffect_RunRisingDragon, %function
	.thumb_func
BattleEffect_RunRisingDragon:
	.incbin "baserom.gba", 0x00178710, 0x000007d4
	.global BattleEffect_RunHeartbreak
	.type BattleEffect_RunHeartbreak, %function
	.thumb_func
BattleEffect_RunHeartbreak:
	.incbin "baserom.gba", 0x00178ee4, 0x000004b0
	.global BattleEffect_RunRagingHeat
	.type BattleEffect_RunRagingHeat, %function
	.thumb_func
BattleEffect_RunRagingHeat:
	.incbin "baserom.gba", 0x00179394, 0x00000600
	.global BattleEffect_RunJuggle
	.type BattleEffect_RunJuggle, %function
	.thumb_func
BattleEffect_RunJuggle:
	.incbin "baserom.gba", 0x00179994, 0x00000584
	.global BattleEffect_RunFlora
	.type BattleEffect_RunFlora, %function
	.thumb_func
BattleEffect_RunFlora:
	.incbin "baserom.gba", 0x00179f18, 0x000016bc
	.section .rom.0017b66c, "ax"
	.balign 4
	.global BattleEffect_RunSabreDance
	.type BattleEffect_RunSabreDance, %function
	.thumb_func
BattleEffect_RunSabreDance:
	.incbin "baserom.gba", 0x0017b66c, 0x00000304
	.section .rom.0017b9b8, "ax"
	.balign 4
	.global BattleEffect_RunBackstab
	.type BattleEffect_RunBackstab, %function
	.thumb_func
BattleEffect_RunBackstab:
	.incbin "baserom.gba", 0x0017b9b8, 0x000003d0
	.global BattleEffect_RunFirePuppet
	.type BattleEffect_RunFirePuppet, %function
	.thumb_func
BattleEffect_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd88, 0x000004f4
	.global BattleEffect_RunWhiplash
	.type BattleEffect_RunWhiplash, %function
	.thumb_func
BattleEffect_RunWhiplash:
	.incbin "baserom.gba", 0x0017c27c, 0x00000430
	.global BattleEffect_RunFume
	.type BattleEffect_RunFume, %function
	.thumb_func
BattleEffect_RunFume:
	.incbin "baserom.gba", 0x0017c6ac, 0x00000584
	.global BattleEffect_RunOceanFist
	.type BattleEffect_RunOceanFist, %function
	.thumb_func
BattleEffect_RunOceanFist:
	.incbin "baserom.gba", 0x0017cc30, 0x00000544
	.global BattleEffect_RunStarMine
	.type BattleEffect_RunStarMine, %function
	.thumb_func
BattleEffect_RunStarMine:
	.incbin "baserom.gba", 0x0017d174, 0x00000550
	.section .rom.0017d724, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragonEnemy
	.type BattleEffect_RunRisingDragonEnemy, %function
	.thumb_func
BattleEffect_RunRisingDragonEnemy:
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
	.global BattleEffect_RunDaedalus
	.type BattleEffect_RunDaedalus, %function
	.thumb_func
BattleEffect_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea58, 0x0000223c
	.global BattleEffect_RunDaedalusAlt
	.type BattleEffect_RunDaedalusAlt, %function
	.thumb_func
BattleEffect_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c94, 0x00000540
	.global BattleEffect_RunVengeance
	.type BattleEffect_RunVengeance, %function
	.thumb_func
BattleEffect_RunVengeance:
	.incbin "baserom.gba", 0x001811d4, 0x0000073c
	.global BattleEffect_RunLifeShear
	.type BattleEffect_RunLifeShear, %function
	.thumb_func
BattleEffect_RunLifeShear:
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
	.section .rom.00182aa8, "ax"
	.balign 4
	.global BattleEffect_RunCoatlicue
	.type BattleEffect_RunCoatlicue, %function
	.thumb_func
BattleEffect_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aa8, 0x00001954
	.global BattleEffect_RunCatastrophe
	.type BattleEffect_RunCatastrophe, %function
	.thumb_func
BattleEffect_RunCatastrophe:
	.incbin "baserom.gba", 0x001843fc, 0x00002a00
	.global BattleEffect_RunHaures
	.type BattleEffect_RunHaures, %function
	.thumb_func
BattleEffect_RunHaures:
	.incbin "baserom.gba", 0x00186dfc, 0x00001918
	.global BattleEffect_RunDeathPlunge
	.type BattleEffect_RunDeathPlunge, %function
	.thumb_func
BattleEffect_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188714, 0x0000059c
	.global BattleEffect_RunHelmSplitter
	.type BattleEffect_RunHelmSplitter, %function
	.thumb_func
BattleEffect_RunHelmSplitter:
	.incbin "baserom.gba", 0x00188cb0, 0x00000498
	.global BattleEffect_RunDragonCloud
	.type BattleEffect_RunDragonCloud, %function
	.thumb_func
BattleEffect_RunDragonCloud:
	.incbin "baserom.gba", 0x00189148, 0x000009cc
	.global BattleEffect_RunLegend
	.type BattleEffect_RunLegend, %function
	.thumb_func
BattleEffect_RunLegend:
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
	.global BattleEffect_RunTripleChomp
	.type BattleEffect_RunTripleChomp, %function
	.thumb_func
BattleEffect_RunTripleChomp:
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
	.global BattleEffect_RunTrident
	.type BattleEffect_RunTrident, %function
	.thumb_func
BattleEffect_RunTrident:
	.incbin "baserom.gba", 0x0018cb74, 0x00000a1c
	.section .rom.0018d5a8, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5a8, 0x00000854
	.section .rom.0018de3c, "ax"
	.incbin "baserom.gba", 0x0018de3c, 0x00000120
	.global BattleEffect_RunMeteorBlow
	.type BattleEffect_RunMeteorBlow, %function
	.thumb_func
BattleEffect_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df5c, 0x00000e48
	.global BattleEffect_RunLiquifier
	.type BattleEffect_RunLiquifier, %function
	.thumb_func
BattleEffect_RunLiquifier:
	.incbin "baserom.gba", 0x0018eda4, 0x00000828
	.section .rom.0018f620, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f620, 0x000008dc
	.global BattleEffect_RunMegiddo
	.type BattleEffect_RunMegiddo, %function
	.thumb_func
BattleEffect_RunMegiddo:
	.incbin "baserom.gba", 0x0018fefc, 0x00000c78
	.global BattleEffect_RunOdyssey
	.type BattleEffect_RunOdyssey, %function
	.thumb_func
BattleEffect_RunOdyssey:
	.incbin "baserom.gba", 0x00190b74, 0x00000f6c
	.section .rom.00191d60, "ax"
	.balign 4
	.global BattleEffect_RunMoloch
	.type BattleEffect_RunMoloch, %function
	.thumb_func
BattleEffect_RunMoloch:
	.incbin "baserom.gba", 0x00191d60, 0x000007dc
	.section .rom.00192648, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00192648, 0x000000f4
	.section .rom.0019284c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0019284c, 0x00000450
	.global BattleEffect_RunIris
	.type BattleEffect_RunIris, %function
	.thumb_func
BattleEffect_RunIris:
	.incbin "baserom.gba", 0x00192c9c, 0x00003750
	.section .rom.00196404, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196404, 0x00000508
	.section .rom.001a06e0, "ax"
	.global Func_081a06e0
	.type Func_081a06e0, %function
	.thumb_func
Func_081a06e0:
	.incbin "baserom.gba", 0x001a06e0, 0x000001e4
	.global Func_081a08c4
	.type Func_081a08c4, %function
	.thumb_func
Func_081a08c4:
	.incbin "baserom.gba", 0x001a08c4, 0x0000013c
	.section .rom.001a0be8, "ax"
	.balign 4
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0be8, 0x000000c4
	.section .rom.001a101c, "ax"
	.global DisplayScroll_DrawLine
	.type DisplayScroll_DrawLine, %function
	.thumb_func
DisplayScroll_DrawLine:
	.incbin "baserom.gba", 0x001a101c, 0x0000026c
	.section .rom.001a133c, "ax"
	.incbin "baserom.gba", 0x001a133c, 0x000001d0
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
	.section .rom.001a6154, "ax"
	.global Func_081a6154
	.type Func_081a6154, %function
	.thumb_func
Func_081a6154:
	.incbin "baserom.gba", 0x001a6154, 0x000004e4
	.global Func_081a6638
	.type Func_081a6638, %function
	.thumb_func
Func_081a6638:
	.incbin "baserom.gba", 0x001a6638, 0x000000ec
	.global Func_081a6724
	.type Func_081a6724, %function
	.thumb_func
Func_081a6724:
	.incbin "baserom.gba", 0x001a6724, 0x00000390
	.section .rom.001a6b50, "ax"
	.incbin "baserom.gba", 0x001a6b50, 0x00000350
	.global Func_081a6ea0
	.type Func_081a6ea0, %function
	.thumb_func
Func_081a6ea0:
	.incbin "baserom.gba", 0x001a6ea0, 0x00000328
	.section .rom.001a729c, "ax"
	.balign 4
	.global Func_081a729c
	.type Func_081a729c, %function
	.thumb_func
Func_081a729c:
	.incbin "baserom.gba", 0x001a729c, 0x00000298
	.section .rom.001a7534, "ax"
	.global Title_NoOpPalette
	.type Title_NoOpPalette, %function
	.thumb_func
Title_NoOpPalette:
	.incbin "baserom.gba", 0x001a7534, 0x00000004
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
	.section .rom.001a7a48, "ax"
	.global Palette_Transform
	.type Palette_Transform, %function
	.thumb_func
Palette_Transform:
	.incbin "baserom.gba", 0x001a7a48, 0x00000724
	.section .rom.001ac15c, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x001ac15c, 0x000011ec
	.section .rom.001b243c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001b243c, 0x00000e58
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
	.incbin "baserom.gba", 0x001b8180, 0x000001cc
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
	.global Func_081c075a
	.type Func_081c075a, %function
	.thumb_func
Func_081c075a:
	.incbin "baserom.gba", 0x001c075a, 0x00000018
	.section .rom.00858d86, "ax"
	.incbin "baserom.gba", 0x00858d86, 0x00000002
	.section .rom.008591a9, "ax"
	.incbin "baserom.gba", 0x008591a9, 0x00000003
	.section .rom.00890481, "ax"
	.incbin "baserom.gba", 0x00890481, 0x00000003
	.section .rom.008abc3d, "ax"
	.incbin "baserom.gba", 0x008abc3d, 0x00000003
	.section .rom.0093c551, "ax"
	.incbin "baserom.gba", 0x0093c551, 0x00000003
	.section .rom.0094a07a, "ax"
	.incbin "baserom.gba", 0x0094a07a, 0x00000002
	.section .rom.00969e81, "ax"
	.incbin "baserom.gba", 0x00969e81, 0x00000003
	.section .rom.0096a056, "ax"
	.incbin "baserom.gba", 0x0096a056, 0x00000002
	.section .rom.0096a53f, "ax"
	.incbin "baserom.gba", 0x0096a53f, 0x00000001
	.section .rom.00a3f19e, "ax"
	.incbin "baserom.gba", 0x00a3f19e, 0x00000002
	.section .rom.00b4160e, "ax"
	.incbin "baserom.gba", 0x00b4160e, 0x00000002
	.section .rom.00bad70e, "ax"
	.incbin "baserom.gba", 0x00bad70e, 0x00000002
	.section .rom.00c2e46d, "ax"
	.incbin "baserom.gba", 0x00c2e46d, 0x00000003
	.section .rom.00d3fc0f, "ax"
	.incbin "baserom.gba", 0x00d3fc0f, 0x00000001
	.section .rom.00d57f8a, "ax"
	.incbin "baserom.gba", 0x00d57f8a, 0x00000002
	.section .rom.00e0e0e9, "ax"
	.incbin "baserom.gba", 0x00e0e0e9, 0x00000003
	.section .rom.00eba4a9, "ax"
	.incbin "baserom.gba", 0x00eba4a9, 0x00000003
