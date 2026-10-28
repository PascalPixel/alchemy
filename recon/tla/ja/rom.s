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
	.section .rom.0001399c, "ax"
	.balign 4
	.global Func_0801399c
	.type Func_0801399c, %function
	.thumb_func
Func_0801399c:
	.incbin "baserom.gba", 0x0001399c, 0x00000194
	.section .rom.00014174, "ax"
	.global VramCache_AllocateBlocks
	.type VramCache_AllocateBlocks, %function
	.thumb_func
VramCache_AllocateBlocks:
	.incbin "baserom.gba", 0x00014174, 0x00000080
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
	.incbin "baserom.gba", 0x0001596c, 0x00000164
	.section .rom.00016180, "ax"
	.global Func_08016180
	.type Func_08016180, %function
	.thumb_func
Func_08016180:
	.incbin "baserom.gba", 0x00016180, 0x0000015c
	.section .rom.00016990, "ax"
	.global Func_08016990
	.type Func_08016990, %function
	.thumb_func
Func_08016990:
	.incbin "baserom.gba", 0x00016990, 0x00000248
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
	.section .rom.00027064, "ax"
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
	.section .rom.00028a28, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x0002a9f0, 0x00000148
	.section .rom.0002ac0c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0002ac0c, 0x0000030c
	.section .rom.0002af18, "ax"
	.global Func_0802af9c
	.type Func_0802af9c, %function
	.thumb_func
Func_0802af9c:
	.incbin "baserom.gba", 0x0002af18, 0x00000204
	.section .rom.0002b11c, "ax"
	.global Map_CopyCellPictureIndicesRect
	.type Map_CopyCellPictureIndicesRect, %function
	.thumb_func
Map_CopyCellPictureIndicesRect:
	.incbin "baserom.gba", 0x0002b11c, 0x00000134
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
	.section .rom.0002c508, "ax"
	.incbin "baserom.gba", 0x0002c508, 0x000002d8
	.section .rom.0002d004, "ax"
	.global Func_0802d088
	.type Func_0802d088, %function
	.thumb_func
Func_0802d088:
	.incbin "baserom.gba", 0x0002d004, 0x000001b4
	.section .rom.0002dcec, "ax"
	.incbin "baserom.gba", 0x0002dcec, 0x0000011c
	.section .rom.0002de08, "ax"
	.global Func_0802de8c
	.type Func_0802de8c, %function
	.thumb_func
Func_0802de8c:
	.incbin "baserom.gba", 0x0002de08, 0x000005b8
	.section .rom.0003028c, "ax"
	.incbin "baserom.gba", 0x0003028c, 0x00000010
	.section .rom.000385e0, "ax"
	.incbin "baserom.gba", 0x000385e0, 0x00000310
	.section .rom.0003938e, "ax"
	.incbin "baserom.gba", 0x0003938e, 0x00000002
	.section .rom.00039446, "ax"
	.incbin "baserom.gba", 0x00039446, 0x00000002
	.section .rom.00039448, "ax"
	.global Window_ClearInteriorTiles
	.type Window_ClearInteriorTiles, %function
	.thumb_func
Window_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00039448, 0x00000094
	.section .rom.00039540, "ax"
	.balign 4
	.global Message_Start
	.type Message_Start, %function
	.thumb_func
Message_Start:
	.incbin "baserom.gba", 0x00039540, 0x000000b0
	.section .rom.0003971e, "ax"
	.incbin "baserom.gba", 0x0003971e, 0x00000002
	.section .rom.00039858, "ax"
	.global Func_08039864
	.type Func_08039864, %function
	.thumb_func
Func_08039864:
	.incbin "baserom.gba", 0x00039858, 0x0000060c
	.section .rom.00039f28, "ax"
	.global Func_08039f94
	.type Func_08039f94, %function
	.thumb_func
Func_08039f94:
	.incbin "baserom.gba", 0x00039f28, 0x000000c0
	.section .rom.0003a3da, "ax"
	.incbin "baserom.gba", 0x0003a3da, 0x00000002
	.section .rom.0003a890, "ax"
	.balign 4
	.global Func_0803a8fc
	.type Func_0803a8fc, %function
	.thumb_func
Func_0803a8fc:
	.incbin "baserom.gba", 0x0003a890, 0x000001b4
	.section .rom.0003aa84, "ax"
	.global Text_DrawWideStringAtOffset
	.type Text_DrawWideStringAtOffset, %function
	.thumb_func
Text_DrawWideStringAtOffset:
	.incbin "baserom.gba", 0x0003aa84, 0x00000144
	.section .rom.0003ae38, "ax"
	.global Message_Decode
	.type Message_Decode, %function
	.thumb_func
Message_Decode:
	.incbin "baserom.gba", 0x0003ae38, 0x00000944
	.section .rom.0003b77c, "ax"
	.global Window_FitOnScreen
	.type Window_FitOnScreen, %function
	.thumb_func
Window_FitOnScreen:
	.incbin "baserom.gba", 0x0003b77c, 0x000000f8
	.section .rom.0003b924, "ax"
	.global Text_MeasureEntry
	.type Text_MeasureEntry, %function
	.thumb_func
Text_MeasureEntry:
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
	.incbin "baserom.gba", 0x0003c204, 0x000000d4
	.global Func_0803c274
	.type Func_0803c274, %function
	.thumb_func
Func_0803c274:
	.incbin "baserom.gba", 0x0003c2d8, 0x00000158
	.section .rom.0003c4c4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003c4c4, 0x0000013c
	.section .rom.0003c600, "ax"
	.global WindowSprite_UpdateAll
	.type WindowSprite_UpdateAll, %function
	.thumb_func
WindowSprite_UpdateAll:
	.incbin "baserom.gba", 0x0003c600, 0x00000464
	.section .rom.0003d7b4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0003d7b4, 0x0000022c
	.section .rom.0003e058, "ax"
	.global Func_0803df24
	.type Func_0803df24, %function
	.thumb_func
Func_0803df24:
	.incbin "baserom.gba", 0x0003e058, 0x00000564
	.section .rom.0003e8fc, "ax"
	.balign 4
	.global Func_0803e7c8
	.type Func_0803e7c8, %function
	.thumb_func
Func_0803e7c8:
	.incbin "baserom.gba", 0x0003e8fc, 0x00000124
	.section .rom.0003ec60, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0003ec60, 0x000000b0
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
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0003f138, 0x000001d0
	.section .rom.0003f4fc, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0003f4fc, 0x00000248
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
	.global Func_080409f0
	.type Func_080409f0, %function
	.thumb_func
Func_080409f0:
	.incbin "baserom.gba", 0x00040b14, 0x00000058
	.section .rom.00040e94, "ax"
	.incbin "baserom.gba", 0x00040e94, 0x000000f4
	.section .rom.000416c4, "ax"
	.incbin "baserom.gba", 0x000416c4, 0x0000012c
	.section .rom.000417f0, "ax"
	.global Text_DrawStringAsTiles
	.type Text_DrawStringAsTiles, %function
	.thumb_func
Text_DrawStringAsTiles:
	.incbin "baserom.gba", 0x000417f0, 0x00000410
	.section .rom.000420e8, "ax"
	.balign 4
	.global Text_DrawMessage
	.type Text_DrawMessage, %function
	.thumb_func
Text_DrawMessage:
	.incbin "baserom.gba", 0x000420e8, 0x0000006c
	.section .rom.00042154, "ax"
	.global Text_DrawCharacterAtOffset
	.type Text_DrawCharacterAtOffset, %function
	.thumb_func
Text_DrawCharacterAtOffset:
	.incbin "baserom.gba", 0x00042154, 0x00000098
	.section .rom.00042240, "ax"
	.global Text_DrawStringAtOffset
	.type Text_DrawStringAtOffset, %function
	.thumb_func
Text_DrawStringAtOffset:
	.incbin "baserom.gba", 0x00042240, 0x0000008c
	.section .rom.000423b8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000423b8, 0x000000a0
	.section .rom.000426be, "ax"
	.incbin "baserom.gba", 0x000426be, 0x00000002
	.section .rom.00042824, "ax"
	.incbin "baserom.gba", 0x00042824, 0x0000029c
	.section .rom.00042ac0, "ax"
	.global PartyStatus_Draw
	.type PartyStatus_Draw, %function
	.thumb_func
PartyStatus_Draw:
	.incbin "baserom.gba", 0x00042ac0, 0x00000430
	.section .rom.0004349c, "ax"
	.global Save_BuildFileSummary
	.type Save_BuildFileSummary, %function
	.thumb_func
Save_BuildFileSummary:
	.incbin "baserom.gba", 0x0004349c, 0x00000194
	.section .rom.00043ba8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00043ba8, 0x000000d0
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
	.section .rom.0004d9fc, "ax"
	.global Func_0804d7fc
	.type Func_0804d7fc, %function
	.thumb_func
Func_0804d7fc:
	.incbin "baserom.gba", 0x0004d9fc, 0x0000016c
	.section .rom.0004dcba, "ax"
	.incbin "baserom.gba", 0x0004dcba, 0x00000002
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
	.section .rom.000a83f6, "ax"
	.incbin "baserom.gba", 0x000a83f6, 0x00000002
	.section .rom.000a83f8, "ax"
	.global Unit_Calculate
	.type Unit_Calculate, %function
	.thumb_func
Unit_Calculate:
	.incbin "baserom.gba", 0x000a83f8, 0x000007f4
	.section .rom.000a8df0, "ax"
	.global Func_080addf0
	.type Func_080addf0, %function
	.thumb_func
Func_080addf0:
	.incbin "baserom.gba", 0x000a8df0, 0x000002e8
	.section .rom.000a940c, "ax"
	.balign 4
	.global Func_080ae410
	.type Func_080ae410, %function
	.thumb_func
Func_080ae410:
	.incbin "baserom.gba", 0x000a940c, 0x000001ec
	.section .rom.000a95f8, "ax"
	.global Func_080ae5fc
	.type Func_080ae5fc, %function
	.thumb_func
Func_080ae5fc:
	.incbin "baserom.gba", 0x000a95f8, 0x000000cc
	.section .rom.000a96e0, "ax"
	.balign 4
	.global Func_080ae6e4
	.type Func_080ae6e4, %function
	.thumb_func
Func_080ae6e4:
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
	.section .rom.000aa0de, "ax"
	.incbin "baserom.gba", 0x000aa0de, 0x00000002
	.section .rom.000aabe8, "ax"
	.balign 4
	.global Func_080afbec
	.type Func_080afbec, %function
	.thumb_func
Func_080afbec:
	.incbin "baserom.gba", 0x000aabe8, 0x000001d0
	.section .rom.000ab080, "ax"
	.global Unit_CalculateBaseElements
	.type Unit_CalculateBaseElements, %function
	.thumb_func
Unit_CalculateBaseElements:
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
	.section .rom.000c9fd8, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x000c9fd8, 0x0000018c
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
	.global Func_080cb09c
	.type Func_080cb09c, %function
	.thumb_func
Func_080cb09c:
	.incbin "baserom.gba", 0x000cb09c, 0x0000062c
	.section .rom.000cb91c, "ax"
	.global SceneSequence_Run
	.type SceneSequence_Run, %function
	.thumb_func
SceneSequence_Run:
	.incbin "baserom.gba", 0x000cb91c, 0x00000c28
	.global Func_080cc54c
	.type Func_080cc54c, %function
	.thumb_func
Func_080cc54c:
	.incbin "baserom.gba", 0x000cc544, 0x00000130
	.section .rom.000cc7bc, "ax"
	.global Func_080cc7c4
Func_080cc7c4:
	.incbin "baserom.gba", 0x000cc7bc, 0x000001d0
	.section .rom.000cc9f4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000cc9f4, 0x000002bc
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
	.global Func_080ccec8
	.type Func_080ccec8, %function
	.thumb_func
Func_080ccec8:
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
	.section .rom.000ce08c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000ce08c, 0x00000270
	.section .rom.000ce2fc, "ax"
	.global Func_080ce31c
	.type Func_080ce31c, %function
	.thumb_func
Func_080ce31c:
	.incbin "baserom.gba", 0x000ce2fc, 0x0000013c
	.section .rom.000ce5fc, "ax"
	.incbin "baserom.gba", 0x000ce5fc, 0x000004e0
	.section .rom.000cec14, "ax"
	.incbin "baserom.gba", 0x000cec14, 0x0000020c
	.section .rom.000cf534, "ax"
	.incbin "baserom.gba", 0x000cf534, 0x000001a8
	.section .rom.000cf76c, "ax"
	.global Func_080cf78c
	.type Func_080cf78c, %function
	.thumb_func
Func_080cf78c:
	.incbin "baserom.gba", 0x000cf76c, 0x0000096c
	.section .rom.000d01ac, "ax"
	.global ScreenTransition_Start
	.type ScreenTransition_Start, %function
	.thumb_func
ScreenTransition_Start:
	.incbin "baserom.gba", 0x000d01ac, 0x00000354
	.section .rom.000d0c80, "ax"
	.global BattlePalette_UpdateBlend
	.type BattlePalette_UpdateBlend, %function
	.thumb_func
BattlePalette_UpdateBlend:
	.incbin "baserom.gba", 0x000d0c80, 0x0000017c
	.section .rom.000d0dfc, "ax"
	.global BattleEffect_BuildBuffer
	.type BattleEffect_BuildBuffer, %function
	.thumb_func
BattleEffect_BuildBuffer:
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
	.section .rom.000d1f00, "ax"
	.global Func_080d1f20
	.type Func_080d1f20, %function
	.thumb_func
Func_080d1f20:
	.incbin "baserom.gba", 0x000d1f00, 0x00000164
	.section .rom.000d25ec, "ax"
	.global PartyInventory_GiveItem
	.type PartyInventory_GiveItem, %function
	.thumb_func
PartyInventory_GiveItem:
	.incbin "baserom.gba", 0x000d25ec, 0x000001e0
	.section .rom.000d3356, "ax"
	.incbin "baserom.gba", 0x000d3356, 0x00000002
	.section .rom.000d3358, "ax"
	.global Func_080d3378
	.type Func_080d3378, %function
	.thumb_func
Func_080d3378:
	.incbin "baserom.gba", 0x000d3358, 0x000000e8
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
	.global FieldMessage_ShowAtActor
	.type FieldMessage_ShowAtActor, %function
	.thumb_func
FieldMessage_ShowAtActor:
	.incbin "baserom.gba", 0x000d3c68, 0x00000328
	.section .rom.000d41c6, "ax"
	.incbin "baserom.gba", 0x000d41c6, 0x00000002
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
	.section .rom.000d7014, "ax"
	.global Func_080d7034
	.type Func_080d7034, %function
	.thumb_func
Func_080d7034:
	.incbin "baserom.gba", 0x000d7014, 0x0000020c
	.section .rom.000d7504, "ax"
	.global Func_080d7524
	.type Func_080d7524, %function
	.thumb_func
Func_080d7524:
	.incbin "baserom.gba", 0x000d7504, 0x0000024c
	.section .rom.000d7808, "ax"
	.incbin "baserom.gba", 0x000d7808, 0x000000f0
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
	.section .rom.000d829c, "ax"
	.global Func_080d82e0
	.type Func_080d82e0, %function
	.thumb_func
Func_080d82e0:
	.incbin "baserom.gba", 0x000d829c, 0x000002d8
	.section .rom.000d86fc, "ax"
	.global Func_080d8740
	.type Func_080d8740, %function
	.thumb_func
Func_080d8740:
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
	.section .rom.000daa6c, "ax"
	.balign 4
	.global Func_080daab0
	.type Func_080daab0, %function
	.thumb_func
Func_080daab0:
	.incbin "baserom.gba", 0x000daa6c, 0x00000208
	.global Func_080dacb8
	.type Func_080dacb8, %function
	.thumb_func
Func_080dacb8:
	.incbin "baserom.gba", 0x000dac74, 0x00000184
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
	.section .rom.000dbb34, "ax"
	.balign 4
	.global Func_080dbb78
Func_080dbb78:
	.incbin "baserom.gba", 0x000dbb34, 0x0000008c
	.section .rom.000dbe8c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000dbe8c, 0x00000174
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
	.section .rom.000dcb00, "ax"
	.balign 4
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
	.section .rom.000dd7dc, "ax"
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
	.section .rom.000de1d8, "ax"
	.global Func_080de21c
	.type Func_080de21c, %function
	.thumb_func
Func_080de21c:
	.incbin "baserom.gba", 0x000de1d8, 0x00000334
	.section .rom.000dec74, "ax"
	.global Func_080decb8
	.type Func_080decb8, %function
	.thumb_func
Func_080decb8:
	.incbin "baserom.gba", 0x000dec74, 0x000000cc
	.section .rom.000deda4, "ax"
	.global FieldEffect_RunDouse
FieldEffect_RunDouse:
	.incbin "baserom.gba", 0x000deda4, 0x0000038c
	.section .rom.000df1b8, "ax"
	.global Func_080df1fc
	.type Func_080df1fc, %function
	.thumb_func
Func_080df1fc:
	.incbin "baserom.gba", 0x000df1b8, 0x000004f8
	.section .rom.000df89a, "ax"
	.incbin "baserom.gba", 0x000df89a, 0x00000002
	.section .rom.000df89c, "ax"
	.global Func_080df8e0
	.type Func_080df8e0, %function
	.thumb_func
Func_080df8e0:
	.incbin "baserom.gba", 0x000df89c, 0x000001d4
	.section .rom.000dfcbc, "ax"
	.global FieldEffect_RunWhirlwind
FieldEffect_RunWhirlwind:
	.incbin "baserom.gba", 0x000dfcbc, 0x000002fc
	.section .rom.000e037e, "ax"
	.incbin "baserom.gba", 0x000e037e, 0x00000002
	.section .rom.000e05d4, "ax"
	.incbin "baserom.gba", 0x000e05d4, 0x00000360
	.section .rom.000e097c, "ax"
	.global Func_080e09c0
	.type Func_080e09c0, %function
	.thumb_func
Func_080e09c0:
	.incbin "baserom.gba", 0x000e097c, 0x000002c4
	.section .rom.000e0fe0, "ax"
	.global Func_080e1024
	.type Func_080e1024, %function
	.thumb_func
Func_080e1024:
	.incbin "baserom.gba", 0x000e0fe0, 0x00000130
	.section .rom.000e1404, "ax"
	.global BattleEffect_SpawnScaledArcObjects
	.type BattleEffect_SpawnScaledArcObjects, %function
	.thumb_func
BattleEffect_SpawnScaledArcObjects:
	.incbin "baserom.gba", 0x000e1404, 0x000000c4
	.section .rom.000e14c8, "ax"
	.global BattleEffect_UpdateArcEffect
	.type BattleEffect_UpdateArcEffect, %function
	.thumb_func
BattleEffect_UpdateArcEffect:
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
	.incbin "baserom.gba", 0x000e2890, 0x00000194
	.section .rom.000e2a88, "ax"
	.incbin "baserom.gba", 0x000e2a88, 0x0000016c
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
	.section .rom.000e3884, "ax"
	.incbin "baserom.gba", 0x000e3884, 0x00000434
	.section .rom.000e3cc0, "ax"
	.balign 4
	.global Func_080e3d04
	.type Func_080e3d04, %function
	.thumb_func
Func_080e3d04:
	.incbin "baserom.gba", 0x000e3cc0, 0x00000540
	.section .rom.000e4428, "ax"
	.incbin "baserom.gba", 0x000e4428, 0x00000134
	.section .rom.000e46ec, "ax"
	.balign 4
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
	.incbin "baserom.gba", 0x000e7800, 0x0000074c
	.section .rom.000e8034, "ax"
	.incbin "baserom.gba", 0x000e8034, 0x00000250
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
	.section .rom.000e9884, "ax"
	.incbin "baserom.gba", 0x000e9884, 0x000001ac
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
	.incbin "baserom.gba", 0x000e9eac, 0x000001e4
	.global Func_080ea14c
	.type Func_080ea14c, %function
	.thumb_func
Func_080ea14c:
	.incbin "baserom.gba", 0x000ea090, 0x00000674
	.global Func_080ea7c0
	.type Func_080ea7c0, %function
	.thumb_func
Func_080ea7c0:
	.incbin "baserom.gba", 0x000ea704, 0x000000e8
	.section .rom.000ea818, "ax"
	.balign 4
	.global Func_080ea8d4
	.type Func_080ea8d4, %function
	.thumb_func
Func_080ea8d4:
	.incbin "baserom.gba", 0x000ea818, 0x00000140
	.global Func_080eaa14
	.type Func_080eaa14, %function
	.thumb_func
Func_080eaa14:
	.incbin "baserom.gba", 0x000ea958, 0x0000015c
	.section .rom.000eab14, "ax"
	.balign 4
	.global Func_080eabd0
	.type Func_080eabd0, %function
	.thumb_func
Func_080eabd0:
	.incbin "baserom.gba", 0x000eab14, 0x00000118
	.global Func_080eace8
	.type Func_080eace8, %function
	.thumb_func
Func_080eace8:
	.incbin "baserom.gba", 0x000eac2c, 0x00000114
	.global Func_080eadfc
	.type Func_080eadfc, %function
	.thumb_func
Func_080eadfc:
	.incbin "baserom.gba", 0x000ead40, 0x000000b8
	.section .rom.000eaf60, "ax"
	.global Func_080eb01c
	.type Func_080eb01c, %function
	.thumb_func
Func_080eb01c:
	.incbin "baserom.gba", 0x000eaf60, 0x0000027c
	.section .rom.000eb768, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000eb768, 0x0000010c
	.section .rom.000eb8a4, "ax"
	.global Func_080eb960
	.type Func_080eb960, %function
	.thumb_func
Func_080eb960:
	.incbin "baserom.gba", 0x000eb8a4, 0x000002d0
	.section .rom.000ebed8, "ax"
	.incbin "baserom.gba", 0x000ebed8, 0x000001b8
	.section .rom.000ec114, "ax"
	.incbin "baserom.gba", 0x000ec114, 0x000000cc
	.section .rom.000ec200, "ax"
	.incbin "baserom.gba", 0x000ec200, 0x00000c8c
	.section .rom.000ed1b0, "ax"
	.incbin "baserom.gba", 0x000ed1b0, 0x000004e4
	.section .rom.000ef728, "ax"
	.incbin "baserom.gba", 0x000ef728, 0x00000008
	.section .rom.000f3890, "ax"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3890, 0x00000040
	.section .rom.000f8170, "ax"
	.global Func_080f8170
	.type Func_080f8170, %function
	.thumb_func
Func_080f8170:
	.incbin "baserom.gba", 0x000f8170, 0x00000498
	.section .rom.000f933e, "ax"
	.incbin "baserom.gba", 0x000f933e, 0x00000002
	.section .rom.000f942a, "ax"
	.incbin "baserom.gba", 0x000f942a, 0x00000002
	.section .rom.000f942c, "ax"
	.global Func_080f94a4
	.type Func_080f94a4, %function
	.thumb_func
Func_080f94a4:
	.incbin "baserom.gba", 0x000f942c, 0x00000db0
	.section .rom.000fa7a8, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fa7a8, 0x00000370
	.section .rom.000fac0e, "ax"
	.incbin "baserom.gba", 0x000fac0e, 0x00000002
	.section .rom.000fadc4, "ax"
	.incbin "baserom.gba", 0x000fadc4, 0x00000218
	.section .rom.000fb03c, "ax"
	.incbin "baserom.gba", 0x000fb03c, 0x00000450
	.section .rom.000fbe10, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fbe10, 0x000002e0
	.section .rom.000fc2a4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fc2a4, 0x000001a0
	.section .rom.000fc680, "ax"
	.incbin "baserom.gba", 0x000fc680, 0x000003fc
	.section .rom.000fcfc4, "ax"
	.global Func_080fd000
	.type Func_080fd000, %function
	.thumb_func
Func_080fd000:
	.incbin "baserom.gba", 0x000fcfc4, 0x00000290
	.section .rom.000fd2a8, "ax"
	.global Func_080fd2e8
	.type Func_080fd2e8, %function
	.thumb_func
Func_080fd2e8:
	.incbin "baserom.gba", 0x000fd2a8, 0x00000244
	.section .rom.000fd51c, "ax"
	.incbin "baserom.gba", 0x000fd51c, 0x000001c8
	.section .rom.000fd724, "ax"
	.global Func_080fd6f0
	.type Func_080fd6f0, %function
	.thumb_func
Func_080fd6f0:
	.incbin "baserom.gba", 0x000fd724, 0x0000011c
	.section .rom.000fd8d4, "ax"
	.incbin "baserom.gba", 0x000fd8d4, 0x000000c8
	.section .rom.000fdb08, "ax"
	.global Func_080fdad4
	.type Func_080fdad4, %function
	.thumb_func
Func_080fdad4:
	.incbin "baserom.gba", 0x000fdb08, 0x00000690
	.section .rom.000fe66c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x000fe66c, 0x0000025c
	.global Func_080fe894
	.type Func_080fe894, %function
	.thumb_func
Func_080fe894:
	.incbin "baserom.gba", 0x000fe8c8, 0x000002c4
	.global Func_080feb58
	.type Func_080feb58, %function
	.thumb_func
Func_080feb58:
	.incbin "baserom.gba", 0x000feb8c, 0x000001a4
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
	.incbin "baserom.gba", 0x000ff3a4, 0x0000024c
	.section .rom.000ff882, "ax"
	.incbin "baserom.gba", 0x000ff882, 0x00000002
	.section .rom.000ff980, "ax"
	.incbin "baserom.gba", 0x000ff980, 0x00000208
	.section .rom.000ffd00, "ax"
	.global Func_080ffcd4
	.type Func_080ffcd4, %function
	.thumb_func
Func_080ffcd4:
	.incbin "baserom.gba", 0x000ffd00, 0x00000210
	.section .rom.00100248, "ax"
	.global Func_0810021c
	.type Func_0810021c, %function
	.thumb_func
Func_0810021c:
	.incbin "baserom.gba", 0x00100248, 0x00000278
	.section .rom.00100740, "ax"
	.balign 4
	.global Func_08100738
	.type Func_08100738, %function
	.thumb_func
Func_08100738:
	.incbin "baserom.gba", 0x00100740, 0x000000c0
	.global BattleEffect_ApplyToTargets
	.type BattleEffect_ApplyToTargets, %function
	.thumb_func
BattleEffect_ApplyToTargets:
	.incbin "baserom.gba", 0x00100800, 0x00000548
	.section .rom.00101074, "ax"
	.global Func_0810106c
	.type Func_0810106c, %function
	.thumb_func
Func_0810106c:
	.incbin "baserom.gba", 0x00101074, 0x00000544
	.section .rom.00101640, "ax"
	.incbin "baserom.gba", 0x00101640, 0x00000228
	.section .rom.00101ad0, "ax"
	.global DjinnMenu_DrawPage
DjinnMenu_DrawPage:
	.incbin "baserom.gba", 0x00101ad0, 0x00000144
	.section .rom.00101d64, "ax"
	.incbin "baserom.gba", 0x00101d64, 0x000012d8
	.global Func_08103064
	.type Func_08103064, %function
	.thumb_func
Func_08103064:
	.incbin "baserom.gba", 0x0010303c, 0x00000104
	.section .rom.001031ee, "ax"
	.incbin "baserom.gba", 0x001031ee, 0x00000002
	.global Func_08103218
	.type Func_08103218, %function
	.thumb_func
Func_08103218:
	.incbin "baserom.gba", 0x001031f0, 0x00001710
	.section .rom.00104964, "ax"
	.balign 4
	.global Func_0810498c
	.type Func_0810498c, %function
	.thumb_func
Func_0810498c:
	.incbin "baserom.gba", 0x00104964, 0x0000003c
	.section .rom.00104e10, "ax"
	.incbin "baserom.gba", 0x00104e10, 0x000000c0
	.section .rom.00104ed0, "ax"
	.global Func_08104ef8
	.type Func_08104ef8, %function
	.thumb_func
Func_08104ef8:
	.incbin "baserom.gba", 0x00104ed0, 0x000000e8
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
	.section .rom.001054b8, "ax"
	.incbin "baserom.gba", 0x001054b8, 0x000000a0
	.section .rom.00108b70, "ax"
	.global Func_08108b70
	.type Func_08108b70, %function
	.thumb_func
Func_08108b70:
	.incbin "baserom.gba", 0x00108b70, 0x00000600
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
	.incbin "baserom.gba", 0x0010a110, 0x000001a4
	.section .rom.0010a2e0, "ax"
	.incbin "baserom.gba", 0x0010a2e0, 0x00000394
	.section .rom.0010ac00, "ax"
	.global Func_0810abf0
	.type Func_0810abf0, %function
	.thumb_func
Func_0810abf0:
	.incbin "baserom.gba", 0x0010ac00, 0x000002b4
	.section .rom.0010b05a, "ax"
	.incbin "baserom.gba", 0x0010b05a, 0x00000002
	.section .rom.0010b1c4, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0010b1c4, 0x000001c4
	.section .rom.0010b5d4, "ax"
	.global Func_0810b5c4
	.type Func_0810b5c4, %function
	.thumb_func
Func_0810b5c4:
	.incbin "baserom.gba", 0x0010b5d4, 0x000000f8
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
	.incbin "baserom.gba", 0x00118958, 0x00000184
	.section .rom.00119052, "ax"
	.incbin "baserom.gba", 0x00119052, 0x00000002
	.section .rom.001197f0, "ax"
	.global Func_081197f0
	.type Func_081197f0, %function
	.thumb_func
Func_081197f0:
	.incbin "baserom.gba", 0x001197f0, 0x00000844
	.section .rom.0011a7a8, "ax"
	.global BattleObject_Make
	.type BattleObject_Make, %function
	.thumb_func
BattleObject_Make:
	.incbin "baserom.gba", 0x0011a7a8, 0x000009c0
	.section .rom.0011b376, "ax"
	.incbin "baserom.gba", 0x0011b376, 0x00000002
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
	.section .rom.0011c532, "ax"
	.incbin "baserom.gba", 0x0011c532, 0x00000002
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
	.incbin "baserom.gba", 0x0011d9c8, 0x0000040c
	.global Func_0811ddd8
	.type Func_0811ddd8, %function
	.thumb_func
Func_0811ddd8:
	.incbin "baserom.gba", 0x0011ddd4, 0x000000a0
	.section .rom.0011df6c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0011df6c, 0x000003fc
	.section .rom.0011e3a6, "ax"
	.incbin "baserom.gba", 0x0011e3a6, 0x00000002
	.global Func_0811e3ac
	.type Func_0811e3ac, %function
	.thumb_func
Func_0811e3ac:
	.incbin "baserom.gba", 0x0011e3a8, 0x00000430
	.section .rom.0011e82c, "ax"
	.balign 4
	.global Func_0811e830
	.type Func_0811e830, %function
	.thumb_func
Func_0811e830:
	.incbin "baserom.gba", 0x0011e82c, 0x00000580
	.section .rom.0011f668, "ax"
	.balign 4
	.global Func_0811f66c
	.type Func_0811f66c, %function
	.thumb_func
Func_0811f66c:
	.incbin "baserom.gba", 0x0011f668, 0x000007d0
	.section .rom.0011ff02, "ax"
	.incbin "baserom.gba", 0x0011ff02, 0x00000002
	.section .rom.00120450, "ax"
	.global BattleTarget_Execute
	.type BattleTarget_Execute, %function
	.thumb_func
BattleTarget_Execute:
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
	.section .rom.00124e1c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00124e1c, 0x00000c80
	.section .rom.00125bb4, "ax"
	.global Func_08125bb8
	.type Func_08125bb8, %function
	.thumb_func
Func_08125bb8:
	.incbin "baserom.gba", 0x00125bb4, 0x00000054
	.section .rom.00125d70, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00125d70, 0x000004a4
	.section .rom.001263f8, "ax"
	.balign 4
	.global Func_081263fc
	.type Func_081263fc, %function
	.thumb_func
Func_081263fc:
	.incbin "baserom.gba", 0x001263f8, 0x0000014c
	.global BattleCamera_SetRange
	.type BattleCamera_SetRange, %function
	.thumb_func
BattleCamera_SetRange:
	.incbin "baserom.gba", 0x00126544, 0x000002bc
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
	.global Func_08127308
	.type Func_08127308, %function
	.thumb_func
Func_08127308:
	.incbin "baserom.gba", 0x00127304, 0x00000264
	.section .rom.00127ad8, "ax"
	.global BattleEnemy_ClaimNameLetter
	.type BattleEnemy_ClaimNameLetter, %function
	.thumb_func
BattleEnemy_ClaimNameLetter:
	.incbin "baserom.gba", 0x00127ad8, 0x000000c4
	.section .rom.00127cd0, "ax"
	.incbin "baserom.gba", 0x00127cd0, 0x000003cc
	.section .rom.001284bc, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001284bc, 0x00000304
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
	.section .rom.00148e9c, "ax"
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
	.section .rom.0015e288, "ax"
	.balign 4
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
	.section .rom.0015f16c, "ax"
	.balign 4
	.global Func_0815f16c
	.type Func_0815f16c, %function
	.thumb_func
Func_0815f16c:
	.incbin "baserom.gba", 0x0015f16c, 0x00004764
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
	.thumb_func
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
	.incbin "baserom.gba", 0x00178710, 0x000007d8
	.global BattleEffect_RunHeartbreak
	.type BattleEffect_RunHeartbreak, %function
	.thumb_func
BattleEffect_RunHeartbreak:
	.incbin "baserom.gba", 0x00178ee8, 0x000004b0
	.global BattleEffect_RunRagingHeat
	.type BattleEffect_RunRagingHeat, %function
	.thumb_func
BattleEffect_RunRagingHeat:
	.incbin "baserom.gba", 0x00179398, 0x00000600
	.global BattleEffect_RunJuggle
	.type BattleEffect_RunJuggle, %function
	.thumb_func
BattleEffect_RunJuggle:
	.incbin "baserom.gba", 0x00179998, 0x00000584
	.global BattleEffect_RunFlora
	.type BattleEffect_RunFlora, %function
	.thumb_func
BattleEffect_RunFlora:
	.incbin "baserom.gba", 0x00179f1c, 0x000016bc
	.section .rom.0017b670, "ax"
	.balign 4
	.global BattleEffect_RunSabreDance
	.type BattleEffect_RunSabreDance, %function
	.thumb_func
BattleEffect_RunSabreDance:
	.incbin "baserom.gba", 0x0017b670, 0x00000304
	.section .rom.0017b9bc, "ax"
	.balign 4
	.global BattleEffect_RunBackstab
	.type BattleEffect_RunBackstab, %function
	.thumb_func
BattleEffect_RunBackstab:
	.incbin "baserom.gba", 0x0017b9bc, 0x000003d0
	.global BattleEffect_RunFirePuppet
	.type BattleEffect_RunFirePuppet, %function
	.thumb_func
BattleEffect_RunFirePuppet:
	.incbin "baserom.gba", 0x0017bd8c, 0x000004f4
	.global BattleEffect_RunWhiplash
	.type BattleEffect_RunWhiplash, %function
	.thumb_func
BattleEffect_RunWhiplash:
	.incbin "baserom.gba", 0x0017c280, 0x00000430
	.global BattleEffect_RunFume
	.type BattleEffect_RunFume, %function
	.thumb_func
BattleEffect_RunFume:
	.incbin "baserom.gba", 0x0017c6b0, 0x00000584
	.global BattleEffect_RunOceanFist
	.type BattleEffect_RunOceanFist, %function
	.thumb_func
BattleEffect_RunOceanFist:
	.incbin "baserom.gba", 0x0017cc34, 0x00000544
	.global BattleEffect_RunStarMine
	.type BattleEffect_RunStarMine, %function
	.thumb_func
BattleEffect_RunStarMine:
	.incbin "baserom.gba", 0x0017d178, 0x00000550
	.section .rom.0017d728, "ax"
	.balign 4
	.global BattleEffect_RunRisingDragonEnemy
	.type BattleEffect_RunRisingDragonEnemy, %function
	.thumb_func
BattleEffect_RunRisingDragonEnemy:
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
	.global BattleEffect_RunDaedalus
	.type BattleEffect_RunDaedalus, %function
	.thumb_func
BattleEffect_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea5c, 0x0000223c
	.global BattleEffect_RunDaedalusAlt
	.type BattleEffect_RunDaedalusAlt, %function
	.thumb_func
BattleEffect_RunDaedalusAlt:
	.incbin "baserom.gba", 0x00180c98, 0x00000540
	.global BattleEffect_RunVengeance
	.type BattleEffect_RunVengeance, %function
	.thumb_func
BattleEffect_RunVengeance:
	.incbin "baserom.gba", 0x001811d8, 0x0000073c
	.global BattleEffect_RunLifeShear
	.type BattleEffect_RunLifeShear, %function
	.thumb_func
BattleEffect_RunLifeShear:
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
	.section .rom.00182aac, "ax"
	.balign 4
	.global BattleEffect_RunCoatlicue
	.type BattleEffect_RunCoatlicue, %function
	.thumb_func
BattleEffect_RunCoatlicue:
	.incbin "baserom.gba", 0x00182aac, 0x00001954
	.global BattleEffect_RunCatastrophe
	.type BattleEffect_RunCatastrophe, %function
	.thumb_func
BattleEffect_RunCatastrophe:
	.incbin "baserom.gba", 0x00184400, 0x00002a00
	.global BattleEffect_RunHaures
	.type BattleEffect_RunHaures, %function
	.thumb_func
BattleEffect_RunHaures:
	.incbin "baserom.gba", 0x00186e00, 0x00001918
	.global BattleEffect_RunDeathPlunge
	.type BattleEffect_RunDeathPlunge, %function
	.thumb_func
BattleEffect_RunDeathPlunge:
	.incbin "baserom.gba", 0x00188718, 0x0000059c
	.global BattleEffect_RunHelmSplitter
	.type BattleEffect_RunHelmSplitter, %function
	.thumb_func
BattleEffect_RunHelmSplitter:
	.incbin "baserom.gba", 0x00188cb4, 0x00000498
	.global BattleEffect_RunDragonCloud
	.type BattleEffect_RunDragonCloud, %function
	.thumb_func
BattleEffect_RunDragonCloud:
	.incbin "baserom.gba", 0x0018914c, 0x000009cc
	.global BattleEffect_RunLegend
	.type BattleEffect_RunLegend, %function
	.thumb_func
BattleEffect_RunLegend:
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
	.global BattleEffect_RunTripleChomp
	.type BattleEffect_RunTripleChomp, %function
	.thumb_func
BattleEffect_RunTripleChomp:
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
	.incbin "baserom.gba", 0x0018c7bc, 0x000002f0
	.section .rom.0018cb78, "ax"
	.global BattleEffect_RunTrident
	.type BattleEffect_RunTrident, %function
	.thumb_func
BattleEffect_RunTrident:
	.incbin "baserom.gba", 0x0018cb78, 0x00000a1c
	.section .rom.0018d5ac, "ax"
	.balign 4
	.global Func_0818d5a8
Func_0818d5a8:
	.incbin "baserom.gba", 0x0018d5ac, 0x00000854
	.section .rom.0018de40, "ax"
	.incbin "baserom.gba", 0x0018de40, 0x00000120
	.global BattleEffect_RunMeteorBlow
	.type BattleEffect_RunMeteorBlow, %function
	.thumb_func
BattleEffect_RunMeteorBlow:
	.incbin "baserom.gba", 0x0018df60, 0x00000e48
	.global BattleEffect_RunLiquifier
	.type BattleEffect_RunLiquifier, %function
	.thumb_func
BattleEffect_RunLiquifier:
	.incbin "baserom.gba", 0x0018eda8, 0x00000828
	.section .rom.0018f624, "ax"
	.balign 4
	.global Func_0818f620
	.type Func_0818f620, %function
	.thumb_func
Func_0818f620:
	.incbin "baserom.gba", 0x0018f624, 0x000008dc
	.global BattleEffect_RunMegiddo
	.type BattleEffect_RunMegiddo, %function
	.thumb_func
BattleEffect_RunMegiddo:
	.incbin "baserom.gba", 0x0018ff00, 0x00000c78
	.global BattleEffect_RunOdyssey
	.type BattleEffect_RunOdyssey, %function
	.thumb_func
BattleEffect_RunOdyssey:
	.incbin "baserom.gba", 0x00190b78, 0x00000f6c
	.section .rom.00191d64, "ax"
	.balign 4
	.global BattleEffect_RunMoloch
	.type BattleEffect_RunMoloch, %function
	.thumb_func
BattleEffect_RunMoloch:
	.incbin "baserom.gba", 0x00191d64, 0x000007dc
	.section .rom.0019264c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x0019264c, 0x000000f4
	.section .rom.00192850, "ax"
	.balign 4
	.incbin "baserom.gba", 0x00192850, 0x00000450
	.global BattleEffect_RunIris
	.type BattleEffect_RunIris, %function
	.thumb_func
BattleEffect_RunIris:
	.incbin "baserom.gba", 0x00192ca0, 0x00003750
	.section .rom.00196408, "ax"
	.global Func_08196404
	.type Func_08196404, %function
	.thumb_func
Func_08196404:
	.incbin "baserom.gba", 0x00196408, 0x00000508
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
	.incbin "baserom.gba", 0x001a08c4, 0x00000128
	.section .rom.001a0bd4, "ax"
	.balign 4
	.global DisplayScroll_DrawMessageLine
	.type DisplayScroll_DrawMessageLine, %function
	.thumb_func
DisplayScroll_DrawMessageLine:
	.incbin "baserom.gba", 0x001a0bd4, 0x000000c4
	.section .rom.001a1008, "ax"
	.global DisplayScroll_DrawLine
	.type DisplayScroll_DrawLine, %function
	.thumb_func
DisplayScroll_DrawLine:
	.incbin "baserom.gba", 0x001a1008, 0x0000026c
	.section .rom.001a1328, "ax"
	.incbin "baserom.gba", 0x001a1328, 0x000001d0
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
	.incbin "baserom.gba", 0x001a729c, 0x00000278
	.section .rom.001a7514, "ax"
	.global Title_NoOpPalette
	.type Title_NoOpPalette, %function
	.thumb_func
Title_NoOpPalette:
	.incbin "baserom.gba", 0x001a7514, 0x00000004
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
	.section .rom.001b243c, "ax"
	.balign 4
	.incbin "baserom.gba", 0x001b243c, 0x00000d14
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
	.incbin "baserom.gba", 0x001b8180, 0x000001cc
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
	.global Func_081c075a
	.type Func_081c075a, %function
	.thumb_func
Func_081c075a:
	.incbin "baserom.gba", 0x001c075a, 0x00000018
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
