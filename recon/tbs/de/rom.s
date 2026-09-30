@ tbs-de's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00002e20, "ax"
	.global System_Initialize
	.type System_Initialize, %function
	.thumb_func
System_Initialize:
	.incbin "baserom.gba", 0x00002e20, 0x000000f4
	.section .rom.00002f40, "ax"
	.global Resource_LoadWorkHeader
	.type Resource_LoadWorkHeader, %function
	.thumb_func
Resource_LoadWorkHeader:
	.incbin "baserom.gba", 0x00002f40, 0x0000002c
	.section .rom.00002f6c, "ax"
	.global RuntimeDispatch_NoOpHook
	.type RuntimeDispatch_NoOpHook, %function
	.thumb_func
RuntimeDispatch_NoOpHook:
	.incbin "baserom.gba", 0x00002f6c, 0x00000004
	.section .rom.00002f70, "ax"
	.global Resource_GetTableEntry
	.type Resource_GetTableEntry, %function
	.thumb_func
Resource_GetTableEntry:
	.incbin "baserom.gba", 0x00002f70, 0x00000070
	.section .rom.00002fe0, "ax"
	.global Resource_LoadCode
	.type Resource_LoadCode, %function
	.thumb_func
Resource_LoadCode:
	.incbin "baserom.gba", 0x00002fe0, 0x00000058
	.section .rom.0000303a, "ax"
	.incbin "baserom.gba", 0x0000303a, 0x00000072
	.section .rom.00003e88, "ax"
	.global ResourceTable_AllocateBlocks
	.type ResourceTable_AllocateBlocks, %function
	.thumb_func
ResourceTable_AllocateBlocks:
	.incbin "baserom.gba", 0x00003e88, 0x0000007c
	.section .rom.000047cc, "ax"
	.global Ui_LoadWindowGraphics
	.type Ui_LoadWindowGraphics, %function
	.thumb_func
Ui_LoadWindowGraphics:
	.incbin "baserom.gba", 0x000047cc, 0x0000009c
	.section .rom.00005014, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00005014, 0x000001f4
	.section .rom.000056fc, "ax"
	.global SaveState_InitializeWorkspace
	.type SaveState_InitializeWorkspace, %function
	.thumb_func
SaveState_InitializeWorkspace:
	.incbin "baserom.gba", 0x000056fc, 0x00000144
	.section .rom.0000618c, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x0000618c, 0x000000e4
	.section .rom.00006438, "ax"
	.global SerialRuntime_BeginTransferB
	.type SerialRuntime_BeginTransferB, %function
	.thumb_func
SerialRuntime_BeginTransferB:
	.global Party_Check
	.type Party_Check, %function
	.thumb_func
Party_Check:
	.incbin "baserom.gba", 0x00006438, 0x00000050
	.section .rom.0000658c, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x0000658c, 0x0000023c
	.section .rom.0000689e, "ax"
	.incbin "baserom.gba", 0x0000689e, 0x00000002
	.section .rom.00007350, "ax"
	.incbin "baserom.gba", 0x00007350, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x000076a6, 0x00000126
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x000077cc, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x0000798c, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x000079a0, 0x00000024
	.section .rom.000079e0, "ax"
	.incbin "baserom.gba", 0x000079e0, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x000079e8, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a3c, 0x00000014
	.section .rom.00007a98, "ax"
	.incbin "baserom.gba", 0x00007a98, 0x00000024
	.section .rom.00007ad4, "ax"
	.incbin "baserom.gba", 0x00007ad4, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007aec, 0x00000058
	.section .rom.00007b68, "ax"
	.incbin "baserom.gba", 0x00007b68, 0x0000008c
	.section .rom.00007bfc, "ax"
	.incbin "baserom.gba", 0x00007bfc, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007c14, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c40, 0x0000002c
	.section .rom.00007c94, "ax"
	.incbin "baserom.gba", 0x00007c94, 0x00000b6c
	.section .rom.00008ab8, "ax"
	.incbin "baserom.gba", 0x00008ab8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x00008fb8, 0x00000400
	.section .rom.00009590, "ax"
	.incbin "baserom.gba", 0x00009590, 0x000000ec
	.global Render_DecodeFrameEnd
Render_DecodeFrameEnd:
	.section .rom.000098f8, "ax"
	.incbin "baserom.gba", 0x000098f8, 0x00000284
	.section .rom.00009c94, "ax"
	.incbin "baserom.gba", 0x00009c94, 0x000004e8
	.section .rom.0000a20c, "ax"
	.global Func_0800aa0c
	.type Func_0800aa0c, %function
	.thumb_func
Func_0800aa0c:
	.incbin "baserom.gba", 0x0000a20c, 0x00000668
	.section .rom.0000a966, "ax"
	.incbin "baserom.gba", 0x0000a966, 0x00000002
	.section .rom.0000a968, "ax"
	.global Render_ApplyProjectedPlacement
	.type Render_ApplyProjectedPlacement, %function
	.thumb_func
Render_ApplyProjectedPlacement:
	.incbin "baserom.gba", 0x0000a968, 0x00000220
	.section .rom.0000aeb8, "ax"
	.global ResourceSlot_Load
	.type ResourceSlot_Load, %function
	.thumb_func
ResourceSlot_Load:
	.incbin "baserom.gba", 0x0000aeb8, 0x000000e0
	.section .rom.0000bad8, "ax"
	.global ObjectDispatch_Initialize
	.type ObjectDispatch_Initialize, %function
	.thumb_func
ObjectDispatch_Initialize:
	.incbin "baserom.gba", 0x0000bad8, 0x00000028
	.section .rom.0000bb00, "ax"
	.global ObjectDispatch_ApplyArgumentToChildren
	.type ObjectDispatch_ApplyArgumentToChildren, %function
	.thumb_func
ObjectDispatch_ApplyArgumentToChildren:
	.incbin "baserom.gba", 0x0000bb00, 0x00000044
	.section .rom.0000bb44, "ax"
	.global ObjectDispatch_ApplyValueToChildren
	.type ObjectDispatch_ApplyValueToChildren, %function
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	.incbin "baserom.gba", 0x0000bb44, 0x00000044
	.section .rom.0000bb88, "ax"
	.global ObjectDispatch_ApplyPairToChildren
	.type ObjectDispatch_ApplyPairToChildren, %function
	.thumb_func
ObjectDispatch_ApplyPairToChildren:
	.incbin "baserom.gba", 0x0000bb88, 0x00000064
	.section .rom.0000bbec, "ax"
	.global ObjectDispatch_SetChildField1e
	.type ObjectDispatch_SetChildField1e, %function
	.thumb_func
ObjectDispatch_SetChildField1e:
	.incbin "baserom.gba", 0x0000bbec, 0x0000001c
	.section .rom.0000bc08, "ax"
	.global Animation_SetIndexAndInitObjects
	.type Animation_SetIndexAndInitObjects, %function
	.thumb_func
Animation_SetIndexAndInitObjects:
	.incbin "baserom.gba", 0x0000bc08, 0x00000028
	.section .rom.0000bc30, "ax"
	.global ObjectDispatch_RegisterChildMetadata
	.type ObjectDispatch_RegisterChildMetadata, %function
	.thumb_func
ObjectDispatch_RegisterChildMetadata:
	.incbin "baserom.gba", 0x0000bc30, 0x00000024
	.section .rom.0000bc54, "ax"
	.global ObjectDispatch_InitFromTable5WithArgument
	.type ObjectDispatch_InitFromTable5WithArgument, %function
	.thumb_func
ObjectDispatch_InitFromTable5WithArgument:
	.incbin "baserom.gba", 0x0000bc54, 0x00000018
	.section .rom.0000bc6c, "ax"
	.global ObjectDispatch_InitFromTable0
	.type ObjectDispatch_InitFromTable0, %function
	.thumb_func
ObjectDispatch_InitFromTable0:
	.incbin "baserom.gba", 0x0000bc6c, 0x00000010
	.section .rom.0000bc7c, "ax"
	.global ObjectDispatch_InitFromTable1
	.type ObjectDispatch_InitFromTable1, %function
	.thumb_func
ObjectDispatch_InitFromTable1:
	.incbin "baserom.gba", 0x0000bc7c, 0x00000010
	.section .rom.0000bc8c, "ax"
	.global ObjectDispatch_InitFromTable2
	.type ObjectDispatch_InitFromTable2, %function
	.thumb_func
ObjectDispatch_InitFromTable2:
	.incbin "baserom.gba", 0x0000bc8c, 0x00000010
	.section .rom.0000bc9c, "ax"
	.global ObjectDispatch_InitFromTable3
	.type ObjectDispatch_InitFromTable3, %function
	.thumb_func
ObjectDispatch_InitFromTable3:
	.incbin "baserom.gba", 0x0000bc9c, 0x00000010
	.section .rom.0000bcac, "ax"
	.global ObjectDispatch_InitFromTable6
	.type ObjectDispatch_InitFromTable6, %function
	.thumb_func
ObjectDispatch_InitFromTable6:
	.incbin "baserom.gba", 0x0000bcac, 0x00000010
	.section .rom.0000bcbc, "ax"
	.global ObjectDispatch_InitFromTable4WithArgument
	.type ObjectDispatch_InitFromTable4WithArgument, %function
	.thumb_func
ObjectDispatch_InitFromTable4WithArgument:
	.incbin "baserom.gba", 0x0000bcbc, 0x00000030
	.section .rom.0000bcec, "ax"
	.global ObjectDispatch_WaitForValue16
	.type ObjectDispatch_WaitForValue16, %function
	.thumb_func
ObjectDispatch_WaitForValue16:
	.incbin "baserom.gba", 0x0000bcec, 0x0000003c
	.section .rom.0000bd28, "ax"
	.global ObjectDispatch_SetSingleChildField26
	.type ObjectDispatch_SetSingleChildField26, %function
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	.incbin "baserom.gba", 0x0000bd28, 0x00000020
	.section .rom.0000bd48, "ax"
	.global Animation_SetStateField5Bits2To3
	.type Animation_SetStateField5Bits2To3, %function
	.thumb_func
Animation_SetStateField5Bits2To3:
	.incbin "baserom.gba", 0x0000bd48, 0x00000028
	.section .rom.0000bd70, "ax"
	.global Animation_SetStateField1dBit1
	.type Animation_SetStateField1dBit1, %function
	.thumb_func
Animation_SetStateField1dBit1:
	.incbin "baserom.gba", 0x0000bd70, 0x00000028
	.section .rom.0000bd98, "ax"
	.global Animation_ApplyChildValues
	.type Animation_ApplyChildValues, %function
	.thumb_func
Animation_ApplyChildValues:
	.incbin "baserom.gba", 0x0000bd98, 0x0000001c
	.section .rom.0000bdb4, "ax"
	.global Graphics_EnableObjLayerAndCallbacks
	.type Graphics_EnableObjLayerAndCallbacks, %function
	.thumb_func
Graphics_EnableObjLayerAndCallbacks:
	.incbin "baserom.gba", 0x0000bdb4, 0x00000048
	.section .rom.0000bdfc, "ax"
	.global ObjectDispatch_StopCallbacksAndHideLayers
	.type ObjectDispatch_StopCallbacksAndHideLayers, %function
	.thumb_func
ObjectDispatch_StopCallbacksAndHideLayers:
	.incbin "baserom.gba", 0x0000bdfc, 0x0000002c
	.section .rom.0000be2c, "ax"
	.global ObjectSystem_UpdateCamera
	.type ObjectSystem_UpdateCamera, %function
	.thumb_func
ObjectSystem_UpdateCamera:
	.incbin "baserom.gba", 0x0000be2c, 0x00000250
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.0000c2cc, "ax"
	.global Object_UpdateAllThumb
	.type Object_UpdateAllThumb, %function
	.thumb_func
Object_UpdateAllThumb:
	.incbin "baserom.gba", 0x0000c2cc, 0x00000664
	.section .rom.0000cb04, "ax"
	.incbin "baserom.gba", 0x0000cb04, 0x0000003c
	.section .rom.0000d2f0, "ax"
	.incbin "baserom.gba", 0x0000d2f0, 0x000001ec
	.section .rom.0000d570, "ax"
	.incbin "baserom.gba", 0x0000d570, 0x000004b0
	.section .rom.0000e3ec, "ax"
	.incbin "baserom.gba", 0x0000e3ec, 0x00000bf0
	.section .rom.0000eff2, "ax"
	.incbin "baserom.gba", 0x0000eff2, 0x000001da
	.section .rom.0000f338, "ax"
	.global Map_LoadLayeredScene
	.type Map_LoadLayeredScene, %function
	.thumb_func
Map_LoadLayeredScene:
	.incbin "baserom.gba", 0x0000f338, 0x00000364
	.section .rom.0000fc24, "ax"
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x0000fc24, 0x0000013c
	.section .rom.0000fd60, "ax"
	.global Map_PlayMetatileCopySequence
	.type Map_PlayMetatileCopySequence, %function
	.thumb_func
Map_PlayMetatileCopySequence:
	.incbin "baserom.gba", 0x0000fd60, 0x00000074
	.section .rom.0000fdd4, "ax"
	.global Map_CopyMetatileCellsRect
	.type Map_CopyMetatileCellsRect, %function
	.thumb_func
Map_CopyMetatileCellsRect:
	.incbin "baserom.gba", 0x0000fdd4, 0x00000130
	.section .rom.0000ff88, "ax"
	.global Func_08010788
	.type Func_08010788, %function
	.thumb_func
Func_08010788:
	.incbin "baserom.gba", 0x0000ff88, 0x0000013c
	.section .rom.000100e4, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x000100e4, 0x00000104
	.section .rom.000101e8, "ax"
	.global Map_InitializePerspectiveScene
	.type Map_InitializePerspectiveScene, %function
	.thumb_func
Map_InitializePerspectiveScene:
	.incbin "baserom.gba", 0x000101e8, 0x00000360
	.section .rom.000107f0, "ax"
	.incbin "baserom.gba", 0x000107f0, 0x000000f0
	.section .rom.00010be4, "ax"
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x00010be4, 0x000000bc
	.section .rom.00010ca0, "ax"
	.global Map_UpdateCurrentTileBlockUntilBlocked
	.type Map_UpdateCurrentTileBlockUntilBlocked, %function
	.thumb_func
Map_UpdateCurrentTileBlockUntilBlocked:
	.incbin "baserom.gba", 0x00010ca0, 0x000000c8
	.section .rom.00010e44, "ax"
	.global Map_LoadAreaGraphics
	.type Map_LoadAreaGraphics, %function
	.thumb_func
Map_LoadAreaGraphics:
	.incbin "baserom.gba", 0x00010e44, 0x000000f8
	.section .rom.00010f3c, "ax"
	.global Map_LoadDefaultCellsAndUpdateBlock
	.type Map_LoadDefaultCellsAndUpdateBlock, %function
	.thumb_func
Map_LoadDefaultCellsAndUpdateBlock:
	.incbin "baserom.gba", 0x00010f3c, 0x00000060
	.section .rom.00010f9c, "ax"
	.global MapAnimation_Update
	.type MapAnimation_Update, %function
	.thumb_func
MapAnimation_Update:
	.incbin "baserom.gba", 0x00010f9c, 0x0000010c
	.section .rom.000113f4, "ax"
	.global Func_08011bf4
	.type Func_08011bf4, %function
	.thumb_func
Func_08011bf4:
	.incbin "baserom.gba", 0x000113f4, 0x000000ec
	.section .rom.00011752, "ax"
	.incbin "baserom.gba", 0x00011752, 0x00000002
	.section .rom.00011754, "ax"
	.global Func_08011f54
	.type Func_08011f54, %function
	.thumb_func
Func_08011f54:
	.incbin "baserom.gba", 0x00011754, 0x00000084
	.section .rom.000118dc, "ax"
	.global Func_080120dc
	.type Func_080120dc, %function
	.thumb_func
Func_080120dc:
	.incbin "baserom.gba", 0x000118dc, 0x000000c0
	.section .rom.00011d18, "ax"
	.global Ui_RunIconMonitor
	.type Ui_RunIconMonitor, %function
	.thumb_func
Ui_RunIconMonitor:
	.incbin "baserom.gba", 0x00011d18, 0x000005e0
	.section .rom.0001232c, "ax"
	.incbin "baserom.gba", 0x0001232c, 0x000001f4
	.section .rom.00012720, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012720, 0x0000022c
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001294c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00012990, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00012a40, 0x0000008c
	.global Data_080132cc
Data_080132cc:
	.incbin "baserom.gba", 0x00012acc, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x00012afc, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x00012bfc, 0x00000140
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x00012d3c, 0x00000048
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x00012d84, 0x00000008
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.incbin "baserom.gba", 0x00012d8c, 0x00000154
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00012ee0, 0x00000e20
	.section .rom.000142d0, "ax"
	.incbin "baserom.gba", 0x000142d0, 0x00000508
	.section .rom.000151ae, "ax"
	.incbin "baserom.gba", 0x000151ae, 0x00000002
	.section .rom.000151b0, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x000151b0, 0x00000098
	.section .rom.000154ba, "ax"
	.incbin "baserom.gba", 0x000154ba, 0x0000008a
	.section .rom.000155d0, "ax"
	.global UiWork_StepChannelScript
	.type UiWork_StepChannelScript, %function
	.thumb_func
UiWork_StepChannelScript:
	.incbin "baserom.gba", 0x000155d0, 0x000005fc
	.section .rom.0001640c, "ax"
	.global UiText_ShowPositionedMessageAndWait
	.type UiText_ShowPositionedMessageAndWait, %function
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	.incbin "baserom.gba", 0x0001640c, 0x00000130
	.section .rom.00016704, "ax"
	.incbin "baserom.gba", 0x00016704, 0x00000028
	.section .rom.00016912, "ax"
	.incbin "baserom.gba", 0x00016912, 0x00000002
	.section .rom.00016914, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x00016914, 0x0000012c
	.section .rom.00016af4, "ax"
	.incbin "baserom.gba", 0x00016af4, 0x00000164
	.section .rom.00016c58, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00016c58, 0x000006ac
	.section .rom.00017304, "ax"
	.global UiWindow_FitOnScreen
	.type UiWindow_FitOnScreen, %function
	.thumb_func
UiWindow_FitOnScreen:
	.incbin "baserom.gba", 0x00017304, 0x000000d8
	.section .rom.0001749c, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x0001749c, 0x0000042c
	.global Func_08018cac
Func_08018cac:
	.incbin "baserom.gba", 0x000178c8, 0x00000250
	.section .rom.00017ca8, "ax"
	.incbin "baserom.gba", 0x00017ca8, 0x00000140
	.section .rom.00017de8, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x00017de8, 0x00000480
	.section .rom.000182e0, "ax"
	.global UiText_DecodeMessage
	.type UiText_DecodeMessage, %function
	.thumb_func
UiText_DecodeMessage:
	.incbin "baserom.gba", 0x000182e0, 0x00000148
	.section .rom.00018870, "ax"
	.incbin "baserom.gba", 0x00018870, 0x00000110
	.section .rom.00018cfc, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x00018cfc, 0x0000021c
	.section .rom.00019170, "ax"
	.global UiGlyph_LoadEntryWithPalette
	.type UiGlyph_LoadEntryWithPalette, %function
	.thumb_func
UiGlyph_LoadEntryWithPalette:
	.incbin "baserom.gba", 0x00019170, 0x000000a4
	.section .rom.00019468, "ax"
	.global MenuSelection_BuildEntries
	.type MenuSelection_BuildEntries, %function
	.thumb_func
MenuSelection_BuildEntries:
	.incbin "baserom.gba", 0x00019468, 0x00000118
	.section .rom.00019600, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x00019600, 0x00000560
	.section .rom.00019ebc, "ax"
	.global Menu_SetupSelectionSide
	.type Menu_SetupSelectionSide, %function
	.thumb_func
Menu_SetupSelectionSide:
	.incbin "baserom.gba", 0x00019ebc, 0x00000124
	.section .rom.0001a6dc, "ax"
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0001a6dc, 0x000001cc
	.section .rom.0001aaf4, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0001aaf4, 0x00000244
	.section .rom.0001adfa, "ax"
	.incbin "baserom.gba", 0x0001adfa, 0x00000002
	.section .rom.0001adfc, "ax"
	.global Menu_LoadSelectedResource
	.type Menu_LoadSelectedResource, %function
	.thumb_func
Menu_LoadSelectedResource:
	.incbin "baserom.gba", 0x0001adfc, 0x00000094
	.section .rom.0001aeb8, "ax"
	.global Menu_RunTopSelection
	.type Menu_RunTopSelection, %function
	.thumb_func
Menu_RunTopSelection:
	.incbin "baserom.gba", 0x0001aeb8, 0x000000d0
	.section .rom.0001aff0, "ax"
	.global UiTimedNotice_Create
	.type UiTimedNotice_Create, %function
	.thumb_func
UiTimedNotice_Create:
	.incbin "baserom.gba", 0x0001aff0, 0x0000009c
	.section .rom.0001b140, "ax"
	.global Debug_SelectAbilityPair
	.type Debug_SelectAbilityPair, %function
	.thumb_func
Debug_SelectAbilityPair:
	.global Menu_Check
	.type Menu_Check, %function
	.thumb_func
Menu_Check:
	.incbin "baserom.gba", 0x0001b140, 0x00000360
	.section .rom.0001bbea, "ax"
	.incbin "baserom.gba", 0x0001bbea, 0x00000002
	.section .rom.0001bbec, "ax"
	.global GraphicsPalette_LoadSelectionResourcesAndAdvance
	.type GraphicsPalette_LoadSelectionResourcesAndAdvance, %function
	.thumb_func
GraphicsPalette_LoadSelectionResourcesAndAdvance:
	.incbin "baserom.gba", 0x0001bbec, 0x000000cc
	.section .rom.0001c170, "ax"
	.global Menu_RunWorkspaceOptions
	.type Menu_RunWorkspaceOptions, %function
	.thumb_func
Menu_RunWorkspaceOptions:
	.incbin "baserom.gba", 0x0001c170, 0x00000480
	.section .rom.0001c678, "ax"
	.global Menu_CreateWorkspaceWindows
	.type Menu_CreateWorkspaceWindows, %function
	.thumb_func
Menu_CreateWorkspaceWindows:
	.incbin "baserom.gba", 0x0001c678, 0x0000019c
	.section .rom.0001c9cc, "ax"
	.incbin "baserom.gba", 0x0001c9cc, 0x00000134
	.section .rom.0001cb00, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001cb00, 0x00000404
	.section .rom.0001cfbc, "ax"
	.global UiWindow_MarkVisibleTileAttributes
	.type UiWindow_MarkVisibleTileAttributes, %function
	.thumb_func
UiWindow_MarkVisibleTileAttributes:
	.incbin "baserom.gba", 0x0001cfbc, 0x000000b0
	.section .rom.0001da8e, "ax"
	.incbin "baserom.gba", 0x0001da8e, 0x00000002
	.section .rom.0001da90, "ax"
	.global UiWindow_FillFromScene
	.type UiWindow_FillFromScene, %function
	.thumb_func
UiWindow_FillFromScene:
	.incbin "baserom.gba", 0x0001da90, 0x0000007c
	.section .rom.0001dc0c, "ax"
	.incbin "baserom.gba", 0x0001dc0c, 0x00000298
	.section .rom.0001dea4, "ax"
	.global UiWindow_DrawPartyStatusContents
	.type UiWindow_DrawPartyStatusContents, %function
	.thumb_func
UiWindow_DrawPartyStatusContents:
	.incbin "baserom.gba", 0x0001dea4, 0x000003d4
	.section .rom.0001e4bc, "ax"
	.global SaveState_BuildSummaryHeader
	.type SaveState_BuildSummaryHeader, %function
	.thumb_func
SaveState_BuildSummaryHeader:
	.incbin "baserom.gba", 0x0001e4bc, 0x0000019c
	.section .rom.0001eace, "ax"
	.incbin "baserom.gba", 0x0001eace, 0x000000ea
	.section .rom.0001eee8, "ax"
	.global SaveMenu_SelectSlot
	.type SaveMenu_SelectSlot, %function
	.thumb_func
SaveMenu_SelectSlot:
	.incbin "baserom.gba", 0x0001eee8, 0x00000580
	.section .rom.0001f468, "ax"
	.global Save_WriteSelectedSlot
	.type Save_WriteSelectedSlot, %function
	.thumb_func
Save_WriteSelectedSlot:
	.incbin "baserom.gba", 0x0001f468, 0x00000120
	.section .rom.0001f87a, "ax"
	.incbin "baserom.gba", 0x0001f87a, 0x00000002
	.section .rom.0001f87c, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x0001f87c, 0x000004b4
	.section .rom.0001feec, "ax"
	.global Djinn_ShowJoinedMessage
	.type Djinn_ShowJoinedMessage, %function
	.thumb_func
Djinn_ShowJoinedMessage:
	.incbin "baserom.gba", 0x0001feec, 0x00000138
	.section .rom.00020054, "ax"
	.global Party_ShowJoinedMessage
	.type Party_ShowJoinedMessage, %function
	.thumb_func
Party_ShowJoinedMessage:
	.incbin "baserom.gba", 0x00020054, 0x000000f8
	.section .rom.0002014c, "ax"
	.global Party_ShowPairJoinedMessage
	.type Party_ShowPairJoinedMessage, %function
	.thumb_func
Party_ShowPairJoinedMessage:
	.incbin "baserom.gba", 0x0002014c, 0x00000158
	.section .rom.000203ac, "ax"
	.global RenderResource_LoadFrame
	.type RenderResource_LoadFrame, %function
	.thumb_func
RenderResource_LoadFrame:
	.incbin "baserom.gba", 0x000203ac, 0x00000068
	.section .rom.00020414, "ax"
	.global RenderResource_CreateFrame
	.type RenderResource_CreateFrame, %function
	.thumb_func
RenderResource_CreateFrame:
	.incbin "baserom.gba", 0x00020414, 0x00000054
	.section .rom.00020928, "ax"
	.global Resource_LoadIndexedIntoBuffer
	.type Resource_LoadIndexedIntoBuffer, %function
	.thumb_func
Resource_LoadIndexedIntoBuffer:
	.incbin "baserom.gba", 0x00020928, 0x00000054
	.section .rom.0002097c, "ax"
	.global UiText_LoadRemappedGlyph
	.type UiText_LoadRemappedGlyph, %function
	.thumb_func
UiText_LoadRemappedGlyph:
	.incbin "baserom.gba", 0x0002097c, 0x000000d0
	.section .rom.00020b2e, "ax"
	.incbin "baserom.gba", 0x00020b2e, 0x000008fe
	.section .rom.000214e0, "ax"
	.global BattleLayout_HighlightPartyPanels
	.type BattleLayout_HighlightPartyPanels, %function
	.thumb_func
BattleLayout_HighlightPartyPanels:
	.incbin "baserom.gba", 0x000214e0, 0x000000a0
	.section .rom.0002173e, "ax"
	.incbin "baserom.gba", 0x0002173e, 0x00002706
	.section .rom.00023ec4, "ax"
	.incbin "baserom.gba", 0x00023ec4, 0x00001c80
	.section .rom.00025dd8, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x00025dd8, 0x00001080
	.section .rom.00026e58, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x00026e58, 0x00000348
	.section .rom.00027238, "ax"
	.global Menu_RunResourceSelectionLoop
	.type Menu_RunResourceSelectionLoop, %function
	.thumb_func
Menu_RunResourceSelectionLoop:
	.incbin "baserom.gba", 0x00027238, 0x00000234
	.section .rom.0002746c, "ax"
	.global Menu_AppendResourceEntry
	.type Menu_AppendResourceEntry, %function
	.thumb_func
Menu_AppendResourceEntry:
	.incbin "baserom.gba", 0x0002746c, 0x00000060
	.section .rom.0002756c, "ax"
	.incbin "baserom.gba", 0x0002756c, 0x000000f4
	.section .rom.00027660, "ax"
	.global Menu_AnimateSelectionToEntry
	.type Menu_AnimateSelectionToEntry, %function
	.thumb_func
Menu_AnimateSelectionToEntry:
	.incbin "baserom.gba", 0x00027660, 0x0000004c
	.section .rom.000276ac, "ax"
	.global Menu_SelectSaveSlotAction
	.type Menu_SelectSaveSlotAction, %function
	.thumb_func
Menu_SelectSaveSlotAction:
	.incbin "baserom.gba", 0x000276ac, 0x0000021c
	.section .rom.000278c8, "ax"
	.global Menu_SelectResourceLayout
	.type Menu_SelectResourceLayout, %function
	.thumb_func
Menu_SelectResourceLayout:
	.incbin "baserom.gba", 0x000278c8, 0x00000170
	.section .rom.00027a38, "ax"
	.global Menu_SelectEntry11To14
	.type Menu_SelectEntry11To14, %function
	.thumb_func
Menu_SelectEntry11To14:
	.incbin "baserom.gba", 0x00027a38, 0x00000040
	.section .rom.00027a78, "ax"
	.global Menu_SelectEntry19To1c
	.type Menu_SelectEntry19To1c, %function
	.thumb_func
Menu_SelectEntry19To1c:
	.incbin "baserom.gba", 0x00027a78, 0x00000040
	.section .rom.00027ab8, "ax"
	.global Menu_RunConfirmSelection
	.type Menu_RunConfirmSelection, %function
	.thumb_func
Menu_RunConfirmSelection:
	.incbin "baserom.gba", 0x00027ab8, 0x00000060
	.section .rom.00027b18, "ax"
	.global Menu_RunConfirmSelectionAt
	.type Menu_RunConfirmSelectionAt, %function
	.thumb_func
Menu_RunConfirmSelectionAt:
	.incbin "baserom.gba", 0x00027b18, 0x00000054
	.section .rom.00027bb4, "ax"
	.global Menu_DrawSelectionRow
	.type Menu_DrawSelectionRow, %function
	.thumb_func
Menu_DrawSelectionRow:
	.incbin "baserom.gba", 0x00027bb4, 0x000000a8
	.section .rom.00027c5c, "ax"
	.global Menu_RunSelection
	.type Menu_RunSelection, %function
	.thumb_func
Menu_RunSelection:
	.incbin "baserom.gba", 0x00027c5c, 0x000000fc
	.section .rom.00028218, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x00028218, 0x00000228
	.section .rom.00028440, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x00028440, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x000285d4, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x000286d4, 0x000003f0
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00028ac4, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00028ba8, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x00028fa4, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002cb4c, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002cdcc, 0x0000279c
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002f568, 0x00000804
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x0002fd6c, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x000304ac, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x0003052c, 0x000006a8
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x00030bd4, 0x00000400
	.section .rom.00032bd4, "ax"
	.incbin "baserom.gba", 0x00032bd4, 0x0000014c
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00032d20, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x00033120, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x00033520, 0x00002058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00035578, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x000355f1, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x000355f4, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x000355f6, 0x0000000e
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00035604, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x0003560c, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00035634, 0x000009d4
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00036008, 0x00000032
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x0003603a, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x0003604a, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x0003605a, 0x0000000a
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x00036064, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00036084, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x000360b4, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x000360f4, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x00036134, 0x00000128
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x0003625c, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x00036260, 0x00000038
	.section .rom.0007a770, "ax"
	.incbin "baserom.gba", 0x0007a770, 0x00000158
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x0007a8c8, 0x00000040
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x0007a908, 0x00000114
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0007aa1c, 0x000003e4
	.section .rom.0007b120, "ax"
	.global Owner_RefreshAndResetZero
	.type Owner_RefreshAndResetZero, %function
	.thumb_func
Owner_RefreshAndResetZero:
	.incbin "baserom.gba", 0x0007b120, 0x00000010
	.section .rom.0007b130, "ax"
	.global Trade_GetOfferState
	.type Trade_GetOfferState, %function
	.thumb_func
Trade_GetOfferState:
	.incbin "baserom.gba", 0x0007b130, 0x00000018
	.section .rom.0007b148, "ax"
	.global Party_GetAverageLevel
	.type Party_GetAverageLevel, %function
	.thumb_func
Party_GetAverageLevel:
	.incbin "baserom.gba", 0x0007b148, 0x0000004c
	.section .rom.0007ba10, "ax"
	.global GameFlag_RefreshLureCap
	.type GameFlag_RefreshLureCap, %function
	.thumb_func
GameFlag_RefreshLureCap:
	.incbin "baserom.gba", 0x0007ba10, 0x000000a8
	.section .rom.0007bab8, "ax"
	.global Runtime_GetBuildStampTime
	.type Runtime_GetBuildStampTime, %function
	.thumb_func
Runtime_GetBuildStampTime:
	.incbin "baserom.gba", 0x0007bab8, 0x00000080
	.section .rom.0007bb38, "ax"
	.global GameState_InitDefaults
	.type GameState_InitDefaults, %function
	.thumb_func
GameState_InitDefaults:
	.incbin "baserom.gba", 0x0007bb38, 0x00000208
	.section .rom.0007c2ae, "ax"
	.incbin "baserom.gba", 0x0007c2ae, 0x00000002
	.section .rom.0007c2b0, "ax"
	.global Inventory_GetQuantity
	.type Inventory_GetQuantity, %function
	.thumb_func
Inventory_GetQuantity:
	.incbin "baserom.gba", 0x0007c2b0, 0x00000028
	.section .rom.0007c2d8, "ax"
	.global Inventory_Count
	.type Inventory_Count, %function
	.thumb_func
Inventory_Count:
	.incbin "baserom.gba", 0x0007c2d8, 0x00000028
	.section .rom.0007c300, "ax"
	.global PartyInventory_HasSpace
	.type PartyInventory_HasSpace, %function
	.thumb_func
PartyInventory_HasSpace:
	.incbin "baserom.gba", 0x0007c300, 0x00000050
	.section .rom.0007c350, "ax"
	.global PartyInventory_CountFreeSlots
	.type PartyInventory_CountFreeSlots, %function
	.thumb_func
PartyInventory_CountFreeSlots:
	.incbin "baserom.gba", 0x0007c350, 0x00000038
	.section .rom.0007c388, "ax"
	.global Inventory_AddItem
	.type Inventory_AddItem, %function
	.thumb_func
Inventory_AddItem:
	.incbin "baserom.gba", 0x0007c388, 0x00000090
	.section .rom.0007c418, "ax"
	.global PartyInventory_Add
	.type PartyInventory_Add, %function
	.thumb_func
PartyInventory_Add:
	.incbin "baserom.gba", 0x0007c418, 0x0000004c
	.section .rom.0007c464, "ax"
	.global Inventory_Find
	.type Inventory_Find, %function
	.thumb_func
Inventory_Find:
	.incbin "baserom.gba", 0x0007c464, 0x00000034
	.section .rom.0007c498, "ax"
	.global PartyInventory_FindOwner
	.type PartyInventory_FindOwner, %function
	.thumb_func
PartyInventory_FindOwner:
	.incbin "baserom.gba", 0x0007c498, 0x00000070
	.section .rom.0007c508, "ax"
	.global Inventory_Equip
	.type Inventory_Equip, %function
	.thumb_func
Inventory_Equip:
	.incbin "baserom.gba", 0x0007c508, 0x000000d4
	.section .rom.0007c5dc, "ax"
	.global Inventory_FindEquipped
	.type Inventory_FindEquipped, %function
	.thumb_func
Inventory_FindEquipped:
	.incbin "baserom.gba", 0x0007c5dc, 0x00000050
	.section .rom.0007c62c, "ax"
	.global Inventory_GetEquippedDefinition
	.type Inventory_GetEquippedDefinition, %function
	.thumb_func
Inventory_GetEquippedDefinition:
	.incbin "baserom.gba", 0x0007c62c, 0x00000044
	.section .rom.0007c670, "ax"
	.global Inventory_GetEquippedItem
	.type Inventory_GetEquippedItem, %function
	.thumb_func
Inventory_GetEquippedItem:
	.incbin "baserom.gba", 0x0007c670, 0x00000054
	.section .rom.0007c6c4, "ax"
	.global Inventory_Remove
	.type Inventory_Remove, %function
	.thumb_func
Inventory_Remove:
	.incbin "baserom.gba", 0x0007c6c4, 0x00000084
	.section .rom.0007c748, "ax"
	.global Inventory_Discard
	.type Inventory_Discard, %function
	.thumb_func
Inventory_Discard:
	.incbin "baserom.gba", 0x0007c748, 0x00000038
	.section .rom.0007c780, "ax"
	.global Inventory_CheckDiscard
	.type Inventory_CheckDiscard, %function
	.thumb_func
Inventory_CheckDiscard:
	.incbin "baserom.gba", 0x0007c780, 0x0000005c
	.section .rom.0007c7dc, "ax"
	.global PartyInventory_Remove
	.type PartyInventory_Remove, %function
	.thumb_func
PartyInventory_Remove:
	.incbin "baserom.gba", 0x0007c7dc, 0x0000002c
	.section .rom.0007c808, "ax"
	.global PartyInventory_Discard
	.type PartyInventory_Discard, %function
	.thumb_func
PartyInventory_Discard:
	.incbin "baserom.gba", 0x0007c808, 0x0000002c
	.section .rom.0007c834, "ax"
	.global Inventory_Break
	.type Inventory_Break, %function
	.thumb_func
Inventory_Break:
	.incbin "baserom.gba", 0x0007c834, 0x0000002c
	.section .rom.0007c860, "ax"
	.global Inventory_Repair
	.type Inventory_Repair, %function
	.thumb_func
Inventory_Repair:
	.incbin "baserom.gba", 0x0007c860, 0x0000002c
	.section .rom.0007c9f0, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x0007c9f0, 0x00000238
	.section .rom.0007d260, "ax"
	.global BattleUnit_Assign
	.type BattleUnit_Assign, %function
	.thumb_func
BattleUnit_Assign:
	.incbin "baserom.gba", 0x0007d260, 0x0000019c
	.section .rom.0007d924, "ax"
	.global Curve_LookupScaledValue
	.type Curve_LookupScaledValue, %function
	.thumb_func
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x0007d924, 0x000000a0
	.section .rom.0007e464, "ax"
	.global Func_0807a664
	.type Func_0807a664, %function
	.thumb_func
Func_0807a664:
	.incbin "baserom.gba", 0x0007e464, 0x0000013c
	.section .rom.0007e628, "ax"
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.incbin "baserom.gba", 0x0007e628, 0x00000008
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "baserom.gba", 0x0007e630, 0x00000c60
	.global Item_ArtifactSlotTable
Item_ArtifactSlotTable:
	.incbin "baserom.gba", 0x0007f290, 0x00000200
	.global Character_StartingEquipOwnerIds
Character_StartingEquipOwnerIds:
	.incbin "baserom.gba", 0x0007f490, 0x00000018
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x0007f4a8, 0x000037b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x00082c58, 0x00002070
	.global Data_08080ec8
Data_08080ec8:
	.incbin "baserom.gba", 0x00084cc8, 0x00003624
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "baserom.gba", 0x000882ec, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x0008888c, 0x00000010
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x0008889c, 0x00000080
	.global Class_DefinitionTable
Class_DefinitionTable:
	.incbin "baserom.gba", 0x0008891c, 0x0000429c
	.global Data_08088db8
Data_08088db8:
	.incbin "baserom.gba", 0x0008cbb8, 0x00000040
	.global Element_PowerResistByLevel
Element_PowerResistByLevel:
	.incbin "baserom.gba", 0x0008cbf8, 0x00000040
	.global Enemy_ElementPresetTable
Enemy_ElementPresetTable:
	.incbin "baserom.gba", 0x0008cc38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008d06c, 0x00000b94
	.section .rom.0008dcec, "ax"
	.incbin "baserom.gba", 0x0008dcec, 0x000001ec
	.section .rom.0008deec, "ax"
	.incbin "baserom.gba", 0x0008deec, 0x00000264
	.section .rom.0008e47c, "ax"
	.global Encounter_SelectEnemyGroup
	.type Encounter_SelectEnemyGroup, %function
	.thumb_func
Encounter_SelectEnemyGroup:
	.incbin "baserom.gba", 0x0008e47c, 0x000001b8
	.section .rom.0008e760, "ax"
	.incbin "baserom.gba", 0x0008e760, 0x00000080
	.section .rom.0008e8b8, "ax"
	.global BattleFx_SelectResultPointer
	.type BattleFx_SelectResultPointer, %function
	.thumb_func
BattleFx_SelectResultPointer:
	.incbin "baserom.gba", 0x0008e8b8, 0x00000070
	.section .rom.0008e9f2, "ax"
	.incbin "baserom.gba", 0x0008e9f2, 0x00000002
	.section .rom.0008e9f4, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x0008e9f4, 0x00000260
	.section .rom.0008f134, "ax"
	.incbin "baserom.gba", 0x0008f134, 0x00000118
	.section .rom.0008fb00, "ax"
	.incbin "baserom.gba", 0x0008fb00, 0x0000097c
	.section .rom.00090a60, "ax"
	.incbin "baserom.gba", 0x00090a60, 0x00000034
	.section .rom.00090a94, "ax"
	.global BattleFx_FindDescriptor
	.type BattleFx_FindDescriptor, %function
	.thumb_func
BattleFx_FindDescriptor:
	.incbin "baserom.gba", 0x00090a94, 0x0000092c
	.section .rom.00091754, "ax"
	.global Event_FindFacingTrigger
	.type Event_FindFacingTrigger, %function
	.thumb_func
Event_FindFacingTrigger:
	.incbin "baserom.gba", 0x00091754, 0x00000368
	.section .rom.00091fc8, "ax"
	.incbin "baserom.gba", 0x00091fc8, 0x00000254
	.section .rom.00092414, "ax"
	.incbin "baserom.gba", 0x00092414, 0x000000d8
	.section .rom.00092894, "ax"
	.incbin "baserom.gba", 0x00092894, 0x00000048
	.section .rom.000928dc, "ax"
	.global BattleFx_SpawnRandomParticleAtPosition
	.type BattleFx_SpawnRandomParticleAtPosition, %function
	.thumb_func
BattleFx_SpawnRandomParticleAtPosition:
	.incbin "baserom.gba", 0x000928dc, 0x00000094
	.section .rom.00092b98, "ax"
	.global DisplayTransition_UpdateScanlineTable
	.type DisplayTransition_UpdateScanlineTable, %function
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	.incbin "baserom.gba", 0x00092b98, 0x0000090c
	.section .rom.00093568, "ax"
	.incbin "baserom.gba", 0x00093568, 0x000002c4
	.section .rom.00093cc2, "ax"
	.incbin "baserom.gba", 0x00093cc2, 0x00000002
	.section .rom.00093cc4, "ax"
	.global DisplayTransition_UpdateFrame
	.type DisplayTransition_UpdateFrame, %function
	.thumb_func
DisplayTransition_UpdateFrame:
	.incbin "baserom.gba", 0x00093cc4, 0x00000158
	.section .rom.000940c8, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x000940c8, 0x00000718
	.section .rom.00094924, "ax"
	.global Object_EffectSpawnCallback
	.type Object_EffectSpawnCallback, %function
	.thumb_func
Object_EffectSpawnCallback:
	.incbin "baserom.gba", 0x00094924, 0x000001dc
	.section .rom.000950c4, "ax"
	.incbin "baserom.gba", 0x000950c4, 0x000001f4
	.section .rom.00095464, "ax"
	.incbin "baserom.gba", 0x00095464, 0x000000a4
	.section .rom.0009554c, "ax"
	.incbin "baserom.gba", 0x0009554c, 0x000000e0
	.section .rom.000962dc, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x000962dc, 0x00000344
	.section .rom.0009763c, "ax"
	.incbin "baserom.gba", 0x0009763c, 0x000001b4
	.section .rom.0009787a, "ax"
	.incbin "baserom.gba", 0x0009787a, 0x00000102
	.section .rom.00097be0, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x00097be0, 0x000001ec
	.section .rom.00097ebc, "ax"
	.incbin "baserom.gba", 0x00097ebc, 0x00000188
	.section .rom.00098164, "ax"
	.incbin "baserom.gba", 0x00098164, 0x000003b4
	.section .rom.000992a4, "ax"
	.global BattleEffect_UpdatePhasedRadialParticle
	.type BattleEffect_UpdatePhasedRadialParticle, %function
	.thumb_func
BattleEffect_UpdatePhasedRadialParticle:
	.incbin "baserom.gba", 0x000992a4, 0x000001c8
	.section .rom.00099eac, "ax"
	.global BattleFx_Run
	.type BattleFx_Run, %function
	.thumb_func
BattleFx_Run:
	.incbin "baserom.gba", 0x00099eac, 0x00000150
	.section .rom.00099ffc, "ax"
	.global BattleFx_DispatchRequestKind
	.type BattleFx_DispatchRequestKind, %function
	.thumb_func
BattleFx_DispatchRequestKind:
	.incbin "baserom.gba", 0x00099ffc, 0x00000150
	.section .rom.0009a14c, "ax"
	.global BattleFx_ClearChildValueOnMismatch
	.type BattleFx_ClearChildValueOnMismatch, %function
	.thumb_func
BattleFx_ClearChildValueOnMismatch:
	.incbin "baserom.gba", 0x0009a14c, 0x00000040
	.section .rom.0009a64c, "ax"
	.global BattleFx_LoadActionEffectResources
	.type BattleFx_LoadActionEffectResources, %function
	.thumb_func
BattleFx_LoadActionEffectResources:
	.incbin "baserom.gba", 0x0009a64c, 0x00000148
	.section .rom.0009ace0, "ax"
	.global Func_08097644
	.type Func_08097644, %function
	.thumb_func
Func_08097644:
	.incbin "baserom.gba", 0x0009ace0, 0x00000224
	.section .rom.0009b2d8, "ax"
	.incbin "baserom.gba", 0x0009b2d8, 0x00000344
	.section .rom.0009ba3c, "ax"
	.incbin "baserom.gba", 0x0009ba3c, 0x0000012c
	.section .rom.0009cb76, "ax"
	.incbin "baserom.gba", 0x0009cb76, 0x00000002
	.section .rom.0009cb78, "ax"
	.global RunBattleEffect03
	.type RunBattleEffect03, %function
	.thumb_func
RunBattleEffect03:
	.incbin "baserom.gba", 0x0009cb78, 0x000001a8
	.section .rom.0009d096, "ax"
	.incbin "baserom.gba", 0x0009d096, 0x00000002
	.section .rom.0009d098, "ax"
	.global RunBattleEffect05
	.type RunBattleEffect05, %function
	.thumb_func
RunBattleEffect05:
	.incbin "baserom.gba", 0x0009d098, 0x00000328
	.section .rom.0009d44c, "ax"
	.incbin "baserom.gba", 0x0009d44c, 0x000004f0
	.section .rom.0009e25c, "ax"
	.global BattleEffect_RunFallbackObjectTransition
	.type BattleEffect_RunFallbackObjectTransition, %function
	.thumb_func
BattleEffect_RunFallbackObjectTransition:
	.incbin "baserom.gba", 0x0009e25c, 0x000001bc
	.section .rom.0009e50a, "ax"
	.incbin "baserom.gba", 0x0009e50a, 0x00000002
	.section .rom.0009e50c, "ax"
	.global RunBattleEffect13
	.type RunBattleEffect13, %function
	.thumb_func
RunBattleEffect13:
	.incbin "baserom.gba", 0x0009e50c, 0x0000024c
	.section .rom.0009ea0c, "ax"
	.global BattleFx_UpdateDescendingParticlePositiveArc
	.type BattleFx_UpdateDescendingParticlePositiveArc, %function
	.thumb_func
BattleFx_UpdateDescendingParticlePositiveArc:
	.incbin "baserom.gba", 0x0009ea0c, 0x00000074
	.section .rom.0009ea80, "ax"
	.global BattleFx_UpdateDescendingParticleNegativeArc
	.type BattleFx_UpdateDescendingParticleNegativeArc, %function
	.thumb_func
BattleFx_UpdateDescendingParticleNegativeArc:
	.incbin "baserom.gba", 0x0009ea80, 0x00000078
	.section .rom.0009ec84, "ax"
	.incbin "baserom.gba", 0x0009ec84, 0x0000006c
	.section .rom.0009ed40, "ax"
	.incbin "baserom.gba", 0x0009ed40, 0x0000016c
	.section .rom.0009ef9c, "ax"
	.global EffectSlot_UpdateMotion
	.type EffectSlot_UpdateMotion, %function
	.thumb_func
EffectSlot_UpdateMotion:
	.incbin "baserom.gba", 0x0009ef9c, 0x00000140
	.section .rom.0009f3a0, "ax"
	.incbin "baserom.gba", 0x0009f3a0, 0x000006dc
	.section .rom.0009fb78, "ax"
	.incbin "baserom.gba", 0x0009fb78, 0x00000200
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x0009fd78, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a08d8, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x000a0f10, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000a1018, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a1158, 0x000007e8
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a1940, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a19d8, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x000a1bf0, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000a1c36, 0x000001b8
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x000a1dee, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x000a1e20, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x000a1e28, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x000a1ec4, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000a1fe4, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x000a2008, 0x00000150
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000a2158, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x000a2364, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x000a24e8, 0x000002a4
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x000a278c, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x000a280c, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000a2818, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000a283c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000a2860, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x000a2884, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000a28c8, 0x00000048
	.section .rom.000a2f58, "ax"
	.incbin "baserom.gba", 0x000a2f58, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x000a2f78, 0x000003bc
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x000a3334, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x000a3354, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x000a336c, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000a3384, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x000a3390, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x000a3394, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x000a34a0, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x000a34ac, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x000a3568, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x000a356c, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x000a3578, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x000a3634, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x000a3680, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x000a3694, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x000a36a8, 0x00000178
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x000a3820, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a3870, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a3890, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a3894, 0x0000056c
	.section .rom.000a43f0, "ax"
	.global UiText_DrawStatComparison
	.type UiText_DrawStatComparison, %function
	.thumb_func
UiText_DrawStatComparison:
	.incbin "baserom.gba", 0x000a43f0, 0x0000013c
	.section .rom.000a4670, "ax"
	.global PsynergyMenu_InitializeEntryObjects
	.type PsynergyMenu_InitializeEntryObjects, %function
	.thumb_func
PsynergyMenu_InitializeEntryObjects:
	.incbin "baserom.gba", 0x000a4670, 0x000000ec
	.section .rom.000a48be, "ax"
	.incbin "baserom.gba", 0x000a48be, 0x00000002
	.section .rom.000a48c0, "ax"
	.global UiMenu_SlideCursor
	.type UiMenu_SlideCursor, %function
	.thumb_func
UiMenu_SlideCursor:
	.incbin "baserom.gba", 0x000a48c0, 0x00000108
	.section .rom.000a52d0, "ax"
	.global RunAssetSelectionScreen
	.type RunAssetSelectionScreen, %function
	.thumb_func
RunAssetSelectionScreen:
	.incbin "baserom.gba", 0x000a52d0, 0x00000de8
	.section .rom.000a66d0, "ax"
	.incbin "baserom.gba", 0x000a66d0, 0x00000338
	.section .rom.000a6cee, "ax"
	.incbin "baserom.gba", 0x000a6cee, 0x00000002
	.section .rom.000a6cf0, "ax"
	.global ItemMenu_DrawEquipPreview
	.type ItemMenu_DrawEquipPreview, %function
	.thumb_func
ItemMenu_DrawEquipPreview:
	.incbin "baserom.gba", 0x000a6cf0, 0x000001bc
	.section .rom.000a6f4a, "ax"
	.incbin "baserom.gba", 0x000a6f4a, 0x0000060a
	.section .rom.000a7d08, "ax"
	.incbin "baserom.gba", 0x000a7d08, 0x000002c8
	.section .rom.000a8188, "ax"
	.incbin "baserom.gba", 0x000a8188, 0x000001ac
	.section .rom.000a8992, "ax"
	.incbin "baserom.gba", 0x000a8992, 0x00000002
	.section .rom.000a8994, "ax"
	.global Menu_OpenConfirmPrompt
	.type Menu_OpenConfirmPrompt, %function
	.thumb_func
Menu_OpenConfirmPrompt:
	.incbin "baserom.gba", 0x000a8994, 0x0000086c
	.section .rom.000a9490, "ax"
	.incbin "baserom.gba", 0x000a9490, 0x00000180
	.section .rom.000aa620, "ax"
	.global Func_080a77a4
	.type Func_080a77a4, %function
	.thumb_func
Func_080a77a4:
	.global CharacterMenu_SelectOwner
	.type CharacterMenu_SelectOwner, %function
	.thumb_func
CharacterMenu_SelectOwner:
	.incbin "baserom.gba", 0x000aa620, 0x000000ac
	.section .rom.000ab480, "ax"
	.global CharacterMenu_DrawStatusAilments
	.type CharacterMenu_DrawStatusAilments, %function
	.thumb_func
CharacterMenu_DrawStatusAilments:
	.incbin "baserom.gba", 0x000ab480, 0x00000300
	.section .rom.000ac414, "ax"
	.global Shop_DrawItemPage
	.type Shop_DrawItemPage, %function
	.thumb_func
Shop_DrawItemPage:
	.incbin "baserom.gba", 0x000ac414, 0x00000140
	.section .rom.000acd8c, "ax"
	.global BattleEffect_ApplyToTargets
	.type BattleEffect_ApplyToTargets, %function
	.thumb_func
BattleEffect_ApplyToTargets:
	.incbin "baserom.gba", 0x000acd8c, 0x00000538
	.section .rom.000ad3e8, "ax"
	.global ItemMenu_Run
	.type ItemMenu_Run, %function
	.thumb_func
ItemMenu_Run:
	.incbin "baserom.gba", 0x000ad3e8, 0x00000718
	.section .rom.000ade34, "ax"
	.global Func_080aafb8
Func_080aafb8:
	.incbin "baserom.gba", 0x000ade34, 0x0000023c
	.section .rom.000ae190, "ax"
	.incbin "baserom.gba", 0x000ae190, 0x000015e8
	.section .rom.000af934, "ax"
	.global DjinnMenu_DrawStatPreview
	.type DjinnMenu_DrawStatPreview, %function
	.thumb_func
DjinnMenu_DrawStatPreview:
	.incbin "baserom.gba", 0x000af934, 0x000007bc
	.section .rom.000b0288, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000b0288, 0x000000fc
	.section .rom.000b0550, "ax"
	.incbin "baserom.gba", 0x000b0550, 0x00001040
	.section .rom.000b18c8, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000b18c8, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000b19c8, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000b1a48, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000b1bc8, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000b1c48, 0x00000440
	.global Data_080af20c
Data_080af20c:
	.incbin "baserom.gba", 0x000b2088, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000b208c, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000b2090, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000b2094, 0x00000004
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000b2098, 0x00000004
	.global Data_080af220
Data_080af220:
	.incbin "baserom.gba", 0x000b209c, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000b20a0, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000b20a4, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000b20a8, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000b20ac, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000b20b0, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000b20b4, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000b20b8, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000b20e8, 0x00000028
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000b2110, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000b2119, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000b2122, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000b212d, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000b2138, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000b214c, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000b2160, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000b2178, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000b2180, 0x00000480
	.section .rom.000b270c, "ax"
	.incbin "baserom.gba", 0x000b270c, 0x000000f8
	.section .rom.000b2878, "ax"
	.global Shop_Run
	.type Shop_Run, %function
	.thumb_func
Shop_Run:
	.incbin "baserom.gba", 0x000b2878, 0x000001cc
	.section .rom.000b2ada, "ax"
	.incbin "baserom.gba", 0x000b2ada, 0x00000002
	.section .rom.000b2adc, "ax"
	.global UiMessage_ShowAndWait
	.type UiMessage_ShowAndWait, %function
	.thumb_func
UiMessage_ShowAndWait:
	.incbin "baserom.gba", 0x000b2adc, 0x00000098
	.section .rom.000b2b74, "ax"
	.global UiMessage_ShowAndRestoreState
	.type UiMessage_ShowAndRestoreState, %function
	.thumb_func
UiMessage_ShowAndRestoreState:
	.incbin "baserom.gba", 0x000b2b74, 0x000000c0
	.section .rom.000b2c34, "ax"
	.global UiMessage_ShowChoice
	.type UiMessage_ShowChoice, %function
	.thumb_func
UiMessage_ShowChoice:
	.incbin "baserom.gba", 0x000b2c34, 0x00000060
	.section .rom.000b2e40, "ax"
	.global Shop_RestoreSceneTiles
	.type Shop_RestoreSceneTiles, %function
	.thumb_func
Shop_RestoreSceneTiles:
	.incbin "baserom.gba", 0x000b2e40, 0x00000054
	.section .rom.000b2e94, "ax"
	.global Shop_InitEffect
	.type Shop_InitEffect, %function
	.thumb_func
Shop_InitEffect:
	.incbin "baserom.gba", 0x000b2e94, 0x00000024
	.section .rom.000b301e, "ax"
	.incbin "baserom.gba", 0x000b301e, 0x00000002
	.section .rom.000b3020, "ax"
	.global ShopCursor_SetPositionImmediate
	.type ShopCursor_SetPositionImmediate, %function
	.thumb_func
ShopCursor_SetPositionImmediate:
	.incbin "baserom.gba", 0x000b3020, 0x0000004c
	.section .rom.000b30ac, "ax"
	.incbin "baserom.gba", 0x000b30ac, 0x000004f8
	.section .rom.000b3860, "ax"
	.incbin "baserom.gba", 0x000b3860, 0x00000210
	.section .rom.000b3c14, "ax"
	.global Shop_SelectQuantity
	.type Shop_SelectQuantity, %function
	.thumb_func
Shop_SelectQuantity:
	.incbin "baserom.gba", 0x000b3c14, 0x00000254
	.section .rom.000b3e68, "ax"
	.global Shop_ConfirmEquip
	.type Shop_ConfirmEquip, %function
	.thumb_func
Shop_ConfirmEquip:
	.incbin "baserom.gba", 0x000b3e68, 0x00000104
	.section .rom.000b3f6c, "ax"
	.global Shop_SellOld
	.type Shop_SellOld, %function
	.thumb_func
Shop_SellOld:
	.incbin "baserom.gba", 0x000b3f6c, 0x00000060
	.section .rom.000b3fcc, "ax"
	.global Shop_SalePrice
	.type Shop_SalePrice, %function
	.thumb_func
Shop_SalePrice:
	.incbin "baserom.gba", 0x000b3fcc, 0x00000048
	.section .rom.000b4014, "ax"
	.global Shop_PickUnit
	.type Shop_PickUnit, %function
	.thumb_func
Shop_PickUnit:
	.incbin "baserom.gba", 0x000b4014, 0x000003d8
	.section .rom.000b43ec, "ax"
	.global Shop_DrawUnitGrid
	.type Shop_DrawUnitGrid, %function
	.thumb_func
Shop_DrawUnitGrid:
	.incbin "baserom.gba", 0x000b43ec, 0x00000160
	.section .rom.000b471c, "ax"
	.incbin "baserom.gba", 0x000b471c, 0x0000020c
	.section .rom.000b4d78, "ax"
	.global Shop_ServicePrice
	.type Shop_ServicePrice, %function
	.thumb_func
Shop_ServicePrice:
	.incbin "baserom.gba", 0x000b4d78, 0x00000038
	.section .rom.000b4db0, "ax"
	.global Shop_CanServe
	.type Shop_CanServe, %function
	.thumb_func
Shop_CanServe:
	.incbin "baserom.gba", 0x000b4db0, 0x000000d4
	.section .rom.000b4e84, "ax"
	.global Shop_MsgByMode
	.type Shop_MsgByMode, %function
	.thumb_func
Shop_MsgByMode:
	.incbin "baserom.gba", 0x000b4e84, 0x00000124
	.section .rom.000b4fa8, "ax"
	.global Shop_ConfirmAct
	.type Shop_ConfirmAct, %function
	.thumb_func
Shop_ConfirmAct:
	.incbin "baserom.gba", 0x000b4fa8, 0x00000400
	.section .rom.000b554c, "ax"
	.incbin "baserom.gba", 0x000b554c, 0x00000338
	.section .rom.000b5884, "ax"
	.global Inn_CheckIn
	.type Inn_CheckIn, %function
	.thumb_func
Inn_CheckIn:
	.incbin "baserom.gba", 0x000b5884, 0x00000114
	.section .rom.000b5998, "ax"
	.global Inn_PlaySleep
	.type Inn_PlaySleep, %function
	.thumb_func
Inn_PlaySleep:
	.incbin "baserom.gba", 0x000b5998, 0x000000ac
	.section .rom.000b5a44, "ax"
	.global Shop_PickUnitItem
	.type Shop_PickUnitItem, %function
	.thumb_func
Shop_PickUnitItem:
	.incbin "baserom.gba", 0x000b5a44, 0x000008fc
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000b6340, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000b6480, 0x00000280
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000b6700, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000b673c, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000b6746, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000b67ac, 0x00000e54
	.section .rom.000b7804, "ax"
	.incbin "baserom.gba", 0x000b7804, 0x00000008
	.section .rom.000b7818, "ax"
	.incbin "baserom.gba", 0x000b7818, 0x00000040
	.section .rom.000b7b34, "ax"
	.incbin "baserom.gba", 0x000b7b34, 0x000001ac
	.section .rom.000b7ce0, "ax"
	.global Unnamed_080b56e0
	.type Unnamed_080b56e0, %function
	.thumb_func
Unnamed_080b56e0:
	.incbin "baserom.gba", 0x000b7ce0, 0x00000184
	.section .rom.000b8522, "ax"
	.incbin "baserom.gba", 0x000b8522, 0x00000162
	.section .rom.000b89e0, "ax"
	.global Battle_RunEncounter
	.type Battle_RunEncounter, %function
	.thumb_func
Battle_RunEncounter:
	.incbin "baserom.gba", 0x000b89e0, 0x00000698
	.section .rom.000b9bf4, "ax"
	.incbin "baserom.gba", 0x000b9bf4, 0x00000130
	.section .rom.000b9d50, "ax"
	.incbin "baserom.gba", 0x000b9d50, 0x000001ac
	.section .rom.000ba182, "ax"
	.incbin "baserom.gba", 0x000ba182, 0x00000002
	.section .rom.000ba184, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x000ba184, 0x00000264
	.section .rom.000bab8a, "ax"
	.incbin "baserom.gba", 0x000bab8a, 0x0000017a
	.section .rom.000bb234, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000bb234, 0x0000019c
	.section .rom.000bbb6c, "ax"
	.incbin "baserom.gba", 0x000bbb6c, 0x000001d0
	.section .rom.000bbd3c, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x000bbd3c, 0x00000190
	.section .rom.000bc146, "ax"
	.incbin "baserom.gba", 0x000bc146, 0x00000206
	.section .rom.000bc3dc, "ax"
	.incbin "baserom.gba", 0x000bc3dc, 0x000004b8
	.section .rom.000bc8d6, "ax"
	.incbin "baserom.gba", 0x000bc8d6, 0x000002c6
	.section .rom.000bccc4, "ax"
	.incbin "baserom.gba", 0x000bccc4, 0x0000026c
	.section .rom.000bcf8e, "ax"
	.incbin "baserom.gba", 0x000bcf8e, 0x00000266
	.section .rom.000bd284, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x000bd284, 0x0000007c
	.section .rom.000bd456, "ax"
	.incbin "baserom.gba", 0x000bd456, 0x0000074a
	.section .rom.000bdc74, "ax"
	.global BattlePresentation_WaitForAdvance
	.type BattlePresentation_WaitForAdvance, %function
	.thumb_func
BattlePresentation_WaitForAdvance:
	.incbin "baserom.gba", 0x000bdc74, 0x00000164
	.section .rom.000bddd8, "ax"
	.global Unnamed_080bb7c0
	.type Unnamed_080bb7c0, %function
	.thumb_func
Unnamed_080bb7c0:
	.incbin "baserom.gba", 0x000bddd8, 0x00000118
	.section .rom.000bfa3a, "ax"
	.incbin "baserom.gba", 0x000bfa3a, 0x00000002
	.section .rom.000bfa3c, "ax"
	.global BattleCommand_SelectAutomatic
	.type BattleCommand_SelectAutomatic, %function
	.thumb_func
BattleCommand_SelectAutomatic:
	.incbin "baserom.gba", 0x000bfa3c, 0x00000380
	.section .rom.000bfe68, "ax"
	.incbin "baserom.gba", 0x000bfe68, 0x00000048
	.section .rom.000bfeb0, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x000bfeb0, 0x00000754
	.section .rom.000c07a2, "ax"
	.incbin "baserom.gba", 0x000c07a2, 0x0000107e
	.section .rom.000c21bc, "ax"
	.incbin "baserom.gba", 0x000c21bc, 0x00000414
	.section .rom.000c28bc, "ax"
	.incbin "baserom.gba", 0x000c28bc, 0x0000045c
	.section .rom.000c2f04, "ax"
	.global BattleBackground_Load
	.type BattleBackground_Load, %function
	.thumb_func
BattleBackground_Load:
	.incbin "baserom.gba", 0x000c2f04, 0x00000138
	.section .rom.000c3a88, "ax"
	.incbin "baserom.gba", 0x000c3a88, 0x00000260
	.section .rom.000c3dae, "ax"
	.incbin "baserom.gba", 0x000c3dae, 0x00000002
	.section .rom.000c3db0, "ax"
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	.incbin "baserom.gba", 0x000c3db0, 0x0000027c
	.section .rom.000c4614, "ax"
	.incbin "baserom.gba", 0x000c4614, 0x0000036c
	.section .rom.000c4b08, "ax"
	.global BattleEnemy_RecordDefeat
	.type BattleEnemy_RecordDefeat, %function
	.thumb_func
BattleEnemy_RecordDefeat:
	.incbin "baserom.gba", 0x000c4b08, 0x00000234
	.section .rom.000c5022, "ax"
	.incbin "baserom.gba", 0x000c5022, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000c5028, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000c5034, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000c5042, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000c5050, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000c505e, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000c506c, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000c507a, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000c5094, 0x0000003c
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000c50d0, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000c50d8, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000c50f0, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000c5108, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000c5120, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000c5138, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000c5150, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000c5168, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000c5180, 0x00000a54
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000c5bd4, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000c5c1c, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000c5c38, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000c5c40, 0x0000090c
	.global Data_080c3f34
Data_080c3f34:
	.incbin "baserom.gba", 0x000c654c, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000c7f50, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000c7f54, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000c7fbc, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000c7fdc, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000c7ffc, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000c801c, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000c8048, 0x000001e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c8228, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000c8250, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000c9a10, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000c9a38, 0x000013c8
	.section .rom.000cafdc, "ax"
	.incbin "baserom.gba", 0x000cafdc, 0x00000a84
	.section .rom.000cbaa8, "ax"
	.global BattleFx_RunFiveMode
	.type BattleFx_RunFiveMode, %function
	.thumb_func
BattleFx_RunFiveMode:
	.incbin "baserom.gba", 0x000cbaa8, 0x0000053c
	.section .rom.000cbffc, "ax"
	.global BattleFx_RunParticlePool
	.type BattleFx_RunParticlePool, %function
	.thumb_func
BattleFx_RunParticlePool:
	.incbin "baserom.gba", 0x000cbffc, 0x00000380
	.section .rom.000cc40c, "ax"
	.global BattleFx_RunTwelveMode
	.type BattleFx_RunTwelveMode, %function
	.thumb_func
BattleFx_RunTwelveMode:
	.incbin "baserom.gba", 0x000cc40c, 0x000011ec
	.section .rom.000cd5f8, "ax"
	.global Unnamed_080cb7f8
	.type Unnamed_080cb7f8, %function
	.thumb_func
Unnamed_080cb7f8:
	.incbin "baserom.gba", 0x000cd5f8, 0x00000414
	.section .rom.000cda0c, "ax"
	.global BattleEffect_RunTileAndPaletteAnimation
	.type BattleEffect_RunTileAndPaletteAnimation, %function
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	.incbin "baserom.gba", 0x000cda0c, 0x000009cc
	.section .rom.000ce3d8, "ax"
	.global Func_080cc5d8
	.type Func_080cc5d8, %function
	.thumb_func
Func_080cc5d8:
	.incbin "baserom.gba", 0x000ce3d8, 0x00000388
	.section .rom.000ce8ec, "ax"
	.global BattlePresentation_PrepareScene
	.type BattlePresentation_PrepareScene, %function
	.thumb_func
BattlePresentation_PrepareScene:
	.incbin "baserom.gba", 0x000ce8ec, 0x000000f0
	.section .rom.000ce9dc, "ax"
	.global BattleFx_ScheduleCallbacksAndReleaseBlocks
	.type BattleFx_ScheduleCallbacksAndReleaseBlocks, %function
	.thumb_func
BattleFx_ScheduleCallbacksAndReleaseBlocks:
	.incbin "baserom.gba", 0x000ce9dc, 0x00000528
	.section .rom.000cfe34, "ax"
	.incbin "baserom.gba", 0x000cfe34, 0x00000afc
	.section .rom.000d0954, "ax"
	.global BattleFx_RunMemberBurst
	.type BattleFx_RunMemberBurst, %function
	.thumb_func
BattleFx_RunMemberBurst:
	.incbin "baserom.gba", 0x000d0954, 0x00000410
	.section .rom.000d0df8, "ax"
	.global BattleFx_RunFortyEightFrameEffect
	.type BattleFx_RunFortyEightFrameEffect, %function
	.thumb_func
BattleFx_RunFortyEightFrameEffect:
	.incbin "baserom.gba", 0x000d0df8, 0x000002a8
	.section .rom.000d10b8, "ax"
	.global BattleFx_RunMemberBeam
	.type BattleFx_RunMemberBeam, %function
	.thumb_func
BattleFx_RunMemberBeam:
	.incbin "baserom.gba", 0x000d10b8, 0x000005d4
	.section .rom.000d16e0, "ax"
	.global BattleFx_RunSevenMode
	.type BattleFx_RunSevenMode, %function
	.thumb_func
BattleFx_RunSevenMode:
	.incbin "baserom.gba", 0x000d16e0, 0x00000b88
	.section .rom.000d23fc, "ax"
	.incbin "baserom.gba", 0x000d23fc, 0x00001118
	.section .rom.000d3514, "ax"
	.global Unnamed_080d1714
	.type Unnamed_080d1714, %function
	.thumb_func
Unnamed_080d1714:
	.incbin "baserom.gba", 0x000d3514, 0x00000d38
	.section .rom.000d4264, "ax"
	.global BattleEffect_RunPaletteParticles
	.type BattleEffect_RunPaletteParticles, %function
	.thumb_func
BattleEffect_RunPaletteParticles:
	.incbin "baserom.gba", 0x000d4264, 0x00000934
	.section .rom.000d4b98, "ax"
	.global BattleEffect_RunEmberColumns
	.type BattleEffect_RunEmberColumns, %function
	.thumb_func
BattleEffect_RunEmberColumns:
	.incbin "baserom.gba", 0x000d4b98, 0x00001354
	.section .rom.000d5fa4, "ax"
	.incbin "baserom.gba", 0x000d5fa4, 0x00000448
	.section .rom.000d6404, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x000d6404, 0x00000c54
	.section .rom.000d70c8, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x000d70c8, 0x0000123c
	.section .rom.000d8770, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000d8770, 0x00000cec
	.section .rom.000d945c, "ax"
	.global BattleFx_InitializeMode10
	.type BattleFx_InitializeMode10, %function
	.thumb_func
BattleFx_InitializeMode10:
	.incbin "baserom.gba", 0x000d945c, 0x000012ec
	.section .rom.000da7ac, "ax"
	.global BattleEffectA
	.type BattleEffectA, %function
	.thumb_func
BattleEffectA:
	.incbin "baserom.gba", 0x000da7ac, 0x000007e8
	.section .rom.000dafdc, "ax"
	.global BattleEffectB
	.type BattleEffectB, %function
	.thumb_func
BattleEffectB:
	.incbin "baserom.gba", 0x000dafdc, 0x000008dc
	.section .rom.000db8e8, "ax"
	.global RunPaletteRampEffect
	.type RunPaletteRampEffect, %function
	.thumb_func
RunPaletteRampEffect:
	.incbin "baserom.gba", 0x000db8e8, 0x00000764
	.section .rom.000dc0ac, "ax"
	.incbin "baserom.gba", 0x000dc0ac, 0x0000141c
	.section .rom.000dd4e0, "ax"
	.global RunParticleFieldEffect
	.type RunParticleFieldEffect, %function
	.thumb_func
RunParticleFieldEffect:
	.incbin "baserom.gba", 0x000dd4e0, 0x00000444
	.section .rom.000dd9dc, "ax"
	.incbin "baserom.gba", 0x000dd9dc, 0x00000d8c
	.section .rom.000de768, "ax"
	.global BattleEffect_RunStagedParticles
	.type BattleEffect_RunStagedParticles, %function
	.thumb_func
BattleEffect_RunStagedParticles:
	.incbin "baserom.gba", 0x000de768, 0x00000944
	.section .rom.000df0c4, "ax"
	.global BattleFx_RunDualTable
	.type BattleFx_RunDualTable, %function
	.thumb_func
BattleFx_RunDualTable:
	.incbin "baserom.gba", 0x000df0c4, 0x00001034
	.section .rom.000e00f8, "ax"
	.global BattleFx_PrepareCanvasEffect
	.type BattleFx_PrepareCanvasEffect, %function
	.thumb_func
BattleFx_PrepareCanvasEffect:
	.incbin "baserom.gba", 0x000e00f8, 0x0000067c
	.section .rom.000e086e, "ax"
	.incbin "baserom.gba", 0x000e086e, 0x00000002
	.section .rom.000e0870, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x000e0870, 0x00000e48
	.section .rom.000e1848, "ax"
	.global BattleFx_RunParticleFieldVariant
	.type BattleFx_RunParticleFieldVariant, %function
	.thumb_func
BattleFx_RunParticleFieldVariant:
	.incbin "baserom.gba", 0x000e1848, 0x00000394
	.section .rom.000e1c2a, "ax"
	.incbin "baserom.gba", 0x000e1c2a, 0x000006fa
	.section .rom.000e2364, "ax"
	.incbin "baserom.gba", 0x000e2364, 0x00000ff8
	.section .rom.000e33e8, "ax"
	.global BattleFx_InitializeMode12
	.type BattleFx_InitializeMode12, %function
	.thumb_func
BattleFx_InitializeMode12:
	.incbin "baserom.gba", 0x000e33e8, 0x0000130c
	.section .rom.000e4772, "ax"
	.incbin "baserom.gba", 0x000e4772, 0x00000002
	.section .rom.000e4774, "ax"
	.global BattlePres_RunBurstScene
	.type BattlePres_RunBurstScene, %function
	.thumb_func
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x000e4774, 0x00000f44
	.section .rom.000e58a0, "ax"
	.global BattlePres_RunBeamSequence
	.type BattlePres_RunBeamSequence, %function
	.thumb_func
BattlePres_RunBeamSequence:
	.incbin "baserom.gba", 0x000e58a0, 0x00000604
	.section .rom.000e5ea4, "ax"
	.global Unnamed_080e40a4
	.type Unnamed_080e40a4, %function
	.thumb_func
Unnamed_080e40a4:
	.incbin "baserom.gba", 0x000e5ea4, 0x0000064c
	.section .rom.000e65b8, "ax"
	.global BattleFx_RunCastingImpact
	.type BattleFx_RunCastingImpact, %function
	.thumb_func
BattleFx_RunCastingImpact:
	.incbin "baserom.gba", 0x000e65b8, 0x00002190
	.section .rom.000e878c, "ax"
	.incbin "baserom.gba", 0x000e878c, 0x000003b0
	.section .rom.000e8cac, "ax"
	.incbin "baserom.gba", 0x000e8cac, 0x000003d0
	.section .rom.000e9138, "ax"
	.incbin "baserom.gba", 0x000e9138, 0x000000cc
	.section .rom.000e9204, "ax"
	.global BattleEffect_RunParticleStreams
	.type BattleEffect_RunParticleStreams, %function
	.thumb_func
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x000e9204, 0x00000e38
	.section .rom.000ea03c, "ax"
	.global BattleEffect_RunCirclingFallingScene
	.type BattleEffect_RunCirclingFallingScene, %function
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	.incbin "baserom.gba", 0x000ea03c, 0x00001e9c
	.section .rom.000ebed8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000ebed8, 0x0000167c
	.section .rom.000ed554, "ax"
	.global Unnamed_080eb754
	.type Unnamed_080eb754, %function
	.thumb_func
Unnamed_080eb754:
	.incbin "baserom.gba", 0x000ed554, 0x0000098c
	.section .rom.000edf00, "ax"
	.global BattleFx_InitializeMode6
	.type BattleFx_InitializeMode6, %function
	.thumb_func
BattleFx_InitializeMode6:
	.incbin "baserom.gba", 0x000edf00, 0x00000d7c
	.section .rom.000eecf4, "ax"
	.global BattleFx_RunRevealColumn
	.type BattleFx_RunRevealColumn, %function
	.thumb_func
BattleFx_RunRevealColumn:
	.incbin "baserom.gba", 0x000eecf4, 0x00000514
	.section .rom.000ef208, "ax"
	.global Unnamed_080ed408
	.type Unnamed_080ed408, %function
	.thumb_func
Unnamed_080ed408:
	.global BattleEffect_LoadWork
BattleEffect_LoadWork:
	.incbin "baserom.gba", 0x000ef208, 0x000006b0
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000ef8b8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000ef8c0, 0x00000030
	.section .rom.000efc48, "ax"
	.incbin "baserom.gba", 0x000efc48, 0x0000046c
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000f00b4, 0x00000b6a
	.global Data_080eee1e
Data_080eee1e:
	.incbin "baserom.gba", 0x000f0c1e, 0x0000000c
	.global Data_080eee2a
Data_080eee2a:
	.incbin "baserom.gba", 0x000f0c2a, 0x0000000c
	.global Data_080eee36
Data_080eee36:
	.incbin "baserom.gba", 0x000f0c36, 0x00000008
	.global Data_080eee3e
Data_080eee3e:
	.incbin "baserom.gba", 0x000f0c3e, 0x00000008
	.global Data_080eee46
Data_080eee46:
	.incbin "baserom.gba", 0x000f0c46, 0x00000008
	.global Data_080eee4e
Data_080eee4e:
	.incbin "baserom.gba", 0x000f0c4e, 0x00000156
	.section .rom.000f0e14, "ax"
	.incbin "baserom.gba", 0x000f0e14, 0x000007ec
	.section .rom.000f19f0, "ax"
	.global Func_080f03f0
	.type Func_080f03f0, %function
	.thumb_func
Func_080f03f0:
	.incbin "baserom.gba", 0x000f19f0, 0x00000148
	.section .rom.000f1df0, "ax"
	.global Func_080f07f0
Func_080f07f0:
	.incbin "baserom.gba", 0x000f1df0, 0x00000aac
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f289c, 0x00000d64
	.section .rom.000f3628, "ax"
	.incbin "baserom.gba", 0x000f3628, 0x000006c4
	.section .rom.000f3cec, "ax"
	.global Func_080f26ec
	.type Func_080f26ec, %function
	.thumb_func
Func_080f26ec:
	.incbin "baserom.gba", 0x000f3cec, 0x00000494
	.section .rom.000f4180, "ax"
	.global Func_080f2b6c
	.type Func_080f2b6c, %function
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f4180, 0x00000004
	.section .rom.000f4184, "ax"
	.global Title_ShowSplashScreen
	.type Title_ShowSplashScreen, %function
	.thumb_func
Title_ShowSplashScreen:
	.incbin "baserom.gba", 0x000f4184, 0x000001e4
	.section .rom.000f4368, "ax"
	.global Unnamed_080f2d54
	.type Unnamed_080f2d54, %function
	.thumb_func
Unnamed_080f2d54:
	.incbin "baserom.gba", 0x000f4368, 0x00000164
	.section .rom.000f468c, "ax"
	.global Unnamed_080f3078
	.type Unnamed_080f3078, %function
	.thumb_func
Unnamed_080f3078:
	.incbin "baserom.gba", 0x000f468c, 0x00000704
	.section .rom.000f4ed0, "ax"
	.incbin "baserom.gba", 0x000f4ed0, 0x00000730
	.section .rom.000f5768, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000f5768, 0x00001e98
	.section .rom.000f7a40, "ax"
	.incbin "baserom.gba", 0x000f7a40, 0x00001020
	.section .rom.000f8a60, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000f8a60, 0x00000954
	.section .rom.000f9578, "ax"
	.incbin "baserom.gba", 0x000f9578, 0x00000888
	.section .rom.000fc5a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fc5a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fc630, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fc6e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fc714, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fc72c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fc7b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fc7c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fc804, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fc814, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fc848, 0x00000030
	.section .rom.000fd304, "ax"
	.incbin "baserom.gba", 0x000fd304, 0x00000090
	.section .rom.000fd424, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fd424, 0x00000060
	.section .rom.00185498, "ax"
	.incbin "baserom.gba", 0x00185498, 0x00000968
	.section .rom.0031fdd8, "ax"
	.incbin "baserom.gba", 0x0031fdd8, 0x00000fd8
	.section .rom.003247e7, "ax"
	.incbin "baserom.gba", 0x003247e7, 0x00000001
	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x000086f9
	.section .rom.003355e5, "ax"
	.incbin "baserom.gba", 0x003355e5, 0x00000003
	.section .rom.00336ef5, "ax"
	.incbin "baserom.gba", 0x00336ef5, 0x00000003
	.section .rom.0033a9f9, "ax"
	.incbin "baserom.gba", 0x0033a9f9, 0x0000087b
	.section .rom.0033f6b6, "ax"
	.incbin "baserom.gba", 0x0033f6b6, 0x00000002
	.section .rom.0034fe26, "ax"
	.incbin "baserom.gba", 0x0034fe26, 0x00000002
	.section .rom.00353416, "ax"
	.incbin "baserom.gba", 0x00353416, 0x00000002
	.section .rom.0035eeba, "ax"
	.incbin "baserom.gba", 0x0035eeba, 0x00000002
	.section .rom.0036f56a, "ax"
	.incbin "baserom.gba", 0x0036f56a, 0x00000002
	.section .rom.0037700a, "ax"
	.incbin "baserom.gba", 0x0037700a, 0x00000002
	.section .rom.0037a84a, "ax"
	.incbin "baserom.gba", 0x0037a84a, 0x00000002
	.section .rom.00383c7e, "ax"
	.incbin "baserom.gba", 0x00383c7e, 0x00000002
	.section .rom.0038f0a6, "ax"
	.incbin "baserom.gba", 0x0038f0a6, 0x00000002
	.section .rom.00396f62, "ax"
	.incbin "baserom.gba", 0x00396f62, 0x00000002
	.section .rom.0039b9f2, "ax"
	.incbin "baserom.gba", 0x0039b9f2, 0x00000002
	.section .rom.0039fca6, "ax"
	.incbin "baserom.gba", 0x0039fca6, 0x00000002
	.section .rom.003a3626, "ax"
	.incbin "baserom.gba", 0x003a3626, 0x00000002
	.section .rom.003afe62, "ax"
	.incbin "baserom.gba", 0x003afe62, 0x00000002
	.section .rom.003be7aa, "ax"
	.incbin "baserom.gba", 0x003be7aa, 0x00000002
	.section .rom.003c40ad, "ax"
	.incbin "baserom.gba", 0x003c40ad, 0x00000003
	.section .rom.003c5a2f, "ax"
	.incbin "baserom.gba", 0x003c5a2f, 0x00000001
	.section .rom.003c884d, "ax"
	.incbin "baserom.gba", 0x003c884d, 0x00000003
	.section .rom.003cc349, "ax"
	.incbin "baserom.gba", 0x003cc349, 0x00000003
	.section .rom.003cdb5f, "ax"
	.incbin "baserom.gba", 0x003cdb5f, 0x00000001
	.section .rom.003ce14a, "ax"
	.incbin "baserom.gba", 0x003ce14a, 0x00000002
	.section .rom.003ce80e, "ax"
	.incbin "baserom.gba", 0x003ce80e, 0x00000002
	.section .rom.003ceaf5, "ax"
	.incbin "baserom.gba", 0x003ceaf5, 0x00000003
	.section .rom.003cfe5b, "ax"
	.incbin "baserom.gba", 0x003cfe5b, 0x00000001
	.section .rom.003d0245, "ax"
	.incbin "baserom.gba", 0x003d0245, 0x00000003
	.section .rom.003d0617, "ax"
	.incbin "baserom.gba", 0x003d0617, 0x00000001
	.section .rom.003d0eb7, "ax"
	.incbin "baserom.gba", 0x003d0eb7, 0x00000001
	.section .rom.003d135a, "ax"
	.incbin "baserom.gba", 0x003d135a, 0x00000002
	.section .rom.003d2513, "ax"
	.incbin "baserom.gba", 0x003d2513, 0x00000001
	.section .rom.003d39d6, "ax"
	.incbin "baserom.gba", 0x003d39d6, 0x00000002
	.section .rom.003d499f, "ax"
	.incbin "baserom.gba", 0x003d499f, 0x00000001
	.section .rom.003d4bfa, "ax"
	.incbin "baserom.gba", 0x003d4bfa, 0x00000002
	.section .rom.003d6655, "ax"
	.incbin "baserom.gba", 0x003d6655, 0x00000003
	.section .rom.003d6ba1, "ax"
	.incbin "baserom.gba", 0x003d6ba1, 0x00000003
	.section .rom.003d892a, "ax"
	.incbin "baserom.gba", 0x003d892a, 0x00000002
	.section .rom.003d8ba3, "ax"
	.incbin "baserom.gba", 0x003d8ba3, 0x00000001
	.section .rom.003d906a, "ax"
	.incbin "baserom.gba", 0x003d906a, 0x00000002
	.section .rom.003dac3f, "ax"
	.incbin "baserom.gba", 0x003dac3f, 0x00000001
	.section .rom.003dc83d, "ax"
	.incbin "baserom.gba", 0x003dc83d, 0x00000003
	.section .rom.003dca5e, "ax"
	.incbin "baserom.gba", 0x003dca5e, 0x00000002
	.section .rom.003dce9b, "ax"
	.incbin "baserom.gba", 0x003dce9b, 0x00000001
	.section .rom.003dcfad, "ax"
	.incbin "baserom.gba", 0x003dcfad, 0x00000003
	.section .rom.003dd72b, "ax"
	.incbin "baserom.gba", 0x003dd72b, 0x00000001
	.section .rom.003ddbc9, "ax"
	.incbin "baserom.gba", 0x003ddbc9, 0x00000003
	.section .rom.003de8de, "ax"
	.incbin "baserom.gba", 0x003de8de, 0x00000002
	.section .rom.003df65e, "ax"
	.incbin "baserom.gba", 0x003df65e, 0x00000002
	.section .rom.003df9ef, "ax"
	.incbin "baserom.gba", 0x003df9ef, 0x00000001
	.section .rom.003e1736, "ax"
	.incbin "baserom.gba", 0x003e1736, 0x00000002
	.section .rom.003e250d, "ax"
	.incbin "baserom.gba", 0x003e250d, 0x00000003
	.section .rom.003e2d3a, "ax"
	.incbin "baserom.gba", 0x003e2d3a, 0x00000002
	.section .rom.003e33c5, "ax"
	.incbin "baserom.gba", 0x003e33c5, 0x00000003
	.section .rom.003e3583, "ax"
	.incbin "baserom.gba", 0x003e3583, 0x00000001
	.section .rom.003e42db, "ax"
	.incbin "baserom.gba", 0x003e42db, 0x00000001
	.section .rom.003e4827, "ax"
	.incbin "baserom.gba", 0x003e4827, 0x00000001
	.section .rom.003e4c01, "ax"
	.incbin "baserom.gba", 0x003e4c01, 0x00000003
	.section .rom.003e4faf, "ax"
	.incbin "baserom.gba", 0x003e4faf, 0x00000001
	.section .rom.003e6f53, "ax"
	.incbin "baserom.gba", 0x003e6f53, 0x00000001
	.section .rom.003e7cef, "ax"
	.incbin "baserom.gba", 0x003e7cef, 0x00000001
	.section .rom.003e7f0b, "ax"
	.incbin "baserom.gba", 0x003e7f0b, 0x00000001
	.section .rom.003e8207, "ax"
	.incbin "baserom.gba", 0x003e8207, 0x00000001
	.section .rom.003ea3b5, "ax"
	.incbin "baserom.gba", 0x003ea3b5, 0x00000003
	.section .rom.003eb183, "ax"
	.incbin "baserom.gba", 0x003eb183, 0x00000001
	.section .rom.003ec2e5, "ax"
	.incbin "baserom.gba", 0x003ec2e5, 0x00000003
	.section .rom.003ec83f, "ax"
	.incbin "baserom.gba", 0x003ec83f, 0x00000001
	.section .rom.003ee129, "ax"
	.incbin "baserom.gba", 0x003ee129, 0x00000003
	.section .rom.003ee9ca, "ax"
	.incbin "baserom.gba", 0x003ee9ca, 0x00000002
	.section .rom.003eeda7, "ax"
	.incbin "baserom.gba", 0x003eeda7, 0x00000001
	.section .rom.003ef031, "ax"
	.incbin "baserom.gba", 0x003ef031, 0x00000003
	.section .rom.003ef3d7, "ax"
	.incbin "baserom.gba", 0x003ef3d7, 0x00000001
	.section .rom.003ef632, "ax"
	.incbin "baserom.gba", 0x003ef632, 0x00000002
	.section .rom.003ef9ea, "ax"
	.incbin "baserom.gba", 0x003ef9ea, 0x00000002
	.section .rom.003f0ed3, "ax"
	.incbin "baserom.gba", 0x003f0ed3, 0x00000001
	.section .rom.003f2496, "ax"
	.incbin "baserom.gba", 0x003f2496, 0x00000a6e
	.section .rom.003f3cda, "ax"
	.incbin "baserom.gba", 0x003f3cda, 0x00000002
	.section .rom.003f405d, "ax"
	.incbin "baserom.gba", 0x003f405d, 0x00000003
	.section .rom.003f4d0d, "ax"
	.incbin "baserom.gba", 0x003f4d0d, 0x00000003
	.section .rom.003f5855, "ax"
	.incbin "baserom.gba", 0x003f5855, 0x00000003
	.section .rom.003f59ee, "ax"
	.incbin "baserom.gba", 0x003f59ee, 0x00000002
	.section .rom.003f627b, "ax"
	.incbin "baserom.gba", 0x003f627b, 0x00000001
	.section .rom.003f6d42, "ax"
	.incbin "baserom.gba", 0x003f6d42, 0x00000002
	.section .rom.003f725a, "ax"
	.incbin "baserom.gba", 0x003f725a, 0x00000002
	.section .rom.003f787e, "ax"
	.incbin "baserom.gba", 0x003f787e, 0x00000002
	.section .rom.003f80e5, "ax"
	.incbin "baserom.gba", 0x003f80e5, 0x00000003
	.section .rom.003f8509, "ax"
	.incbin "baserom.gba", 0x003f8509, 0x00000003
	.section .rom.003f87a3, "ax"
	.incbin "baserom.gba", 0x003f87a3, 0x00000001
	.section .rom.003fa13f, "ax"
	.incbin "baserom.gba", 0x003fa13f, 0x00000001
	.section .rom.003fbeb6, "ax"
	.incbin "baserom.gba", 0x003fbeb6, 0x00000002
	.section .rom.003fde2b, "ax"
	.incbin "baserom.gba", 0x003fde2b, 0x00000001
	.section .rom.003fe2fb, "ax"
	.incbin "baserom.gba", 0x003fe2fb, 0x00000001
	.section .rom.003fe98d, "ax"
	.incbin "baserom.gba", 0x003fe98d, 0x00000a37
	.section .rom.00400795, "ax"
	.incbin "baserom.gba", 0x00400795, 0x00000003
	.section .rom.00401266, "ax"
	.incbin "baserom.gba", 0x00401266, 0x00000002
	.section .rom.00401da3, "ax"
	.incbin "baserom.gba", 0x00401da3, 0x00000001
	.section .rom.004023e3, "ax"
	.incbin "baserom.gba", 0x004023e3, 0x00001589
	.section .rom.004039cd, "ax"
	.incbin "baserom.gba", 0x004039cd, 0x00000003
	.section .rom.00403d3b, "ax"
	.incbin "baserom.gba", 0x00403d3b, 0x00000001
	.section .rom.004042d9, "ax"
	.incbin "baserom.gba", 0x004042d9, 0x00000003
	.section .rom.0040460d, "ax"
	.incbin "baserom.gba", 0x0040460d, 0x00000003
	.section .rom.00404949, "ax"
	.incbin "baserom.gba", 0x00404949, 0x00000003
	.section .rom.00405962, "ax"
	.incbin "baserom.gba", 0x00405962, 0x00000002
	.section .rom.00408f6e, "ax"
	.incbin "baserom.gba", 0x00408f6e, 0x00000002
	.section .rom.00409711, "ax"
	.incbin "baserom.gba", 0x00409711, 0x00000003
	.section .rom.0040aaa3, "ax"
	.incbin "baserom.gba", 0x0040aaa3, 0x00000001
	.section .rom.0040aed3, "ax"
	.incbin "baserom.gba", 0x0040aed3, 0x00000001
	.section .rom.0040ccc9, "ax"
	.incbin "baserom.gba", 0x0040ccc9, 0x00000003
	.section .rom.0040d5de, "ax"
	.incbin "baserom.gba", 0x0040d5de, 0x00000002
	.section .rom.0040f112, "ax"
	.incbin "baserom.gba", 0x0040f112, 0x00000002
	.section .rom.00410161, "ax"
	.incbin "baserom.gba", 0x00410161, 0x00000003
	.section .rom.00410817, "ax"
	.incbin "baserom.gba", 0x00410817, 0x00000001
	.section .rom.004118bd, "ax"
	.incbin "baserom.gba", 0x004118bd, 0x00000003
	.section .rom.00424ccf, "ax"
	.incbin "baserom.gba", 0x00424ccf, 0x00000001
	.section .rom.00424e21, "ax"
	.incbin "baserom.gba", 0x00424e21, 0x00000003
	.section .rom.004252b2, "ax"
	.incbin "baserom.gba", 0x004252b2, 0x00000002
	.section .rom.004254a9, "ax"
	.incbin "baserom.gba", 0x004254a9, 0x00000003
	.section .rom.00426b66, "ax"
	.incbin "baserom.gba", 0x00426b66, 0x00000002
	.section .rom.00428085, "ax"
	.incbin "baserom.gba", 0x00428085, 0x00000003
	.section .rom.00428c51, "ax"
	.incbin "baserom.gba", 0x00428c51, 0x00000003
	.section .rom.004297c6, "ax"
	.incbin "baserom.gba", 0x004297c6, 0x00000002
	.section .rom.0042b66f, "ax"
	.incbin "baserom.gba", 0x0042b66f, 0x00000001
	.section .rom.0042cdfd, "ax"
	.incbin "baserom.gba", 0x0042cdfd, 0x00000003
	.section .rom.0042e2ae, "ax"
	.incbin "baserom.gba", 0x0042e2ae, 0x00000002
	.section .rom.004309fb, "ax"
	.incbin "baserom.gba", 0x004309fb, 0x00000001
	.section .rom.00432295, "ax"
	.incbin "baserom.gba", 0x00432295, 0x00000003
	.section .rom.0043385e, "ax"
	.incbin "baserom.gba", 0x0043385e, 0x00000002
	.section .rom.004352ee, "ax"
	.incbin "baserom.gba", 0x004352ee, 0x00000002
	.section .rom.004360ea, "ax"
	.incbin "baserom.gba", 0x004360ea, 0x00000002
	.section .rom.0043fa5a, "ax"
	.incbin "baserom.gba", 0x0043fa5a, 0x00000002
	.section .rom.00441c0e, "ax"
	.incbin "baserom.gba", 0x00441c0e, 0x00000002
	.section .rom.00445d75, "ax"
	.incbin "baserom.gba", 0x00445d75, 0x00000003
	.section .rom.00448f9a, "ax"
	.incbin "baserom.gba", 0x00448f9a, 0x00000002
	.section .rom.0044a69e, "ax"
	.incbin "baserom.gba", 0x0044a69e, 0x00000002
	.section .rom.00451286, "ax"
	.incbin "baserom.gba", 0x00451286, 0x00000002
	.section .rom.00459c0a, "ax"
	.incbin "baserom.gba", 0x00459c0a, 0x00000002
	.section .rom.004616e6, "ax"
	.incbin "baserom.gba", 0x004616e6, 0x00000002
	.section .rom.0046477a, "ax"
	.incbin "baserom.gba", 0x0046477a, 0x00000002
	.section .rom.00467b89, "ax"
	.incbin "baserom.gba", 0x00467b89, 0x00000003
	.section .rom.0046a3f5, "ax"
	.incbin "baserom.gba", 0x0046a3f5, 0x00000003
	.section .rom.0046bee6, "ax"
	.incbin "baserom.gba", 0x0046bee6, 0x00000002
	.section .rom.0046ccfe, "ax"
	.incbin "baserom.gba", 0x0046ccfe, 0x00000002
	.section .rom.0046d9e9, "ax"
	.incbin "baserom.gba", 0x0046d9e9, 0x00000003
	.section .rom.00476ea6, "ax"
	.incbin "baserom.gba", 0x00476ea6, 0x00000002
	.section .rom.00477bb1, "ax"
	.incbin "baserom.gba", 0x00477bb1, 0x00000003
	.section .rom.00479cc5, "ax"
	.incbin "baserom.gba", 0x00479cc5, 0x00000003
	.section .rom.0047a937, "ax"
	.incbin "baserom.gba", 0x0047a937, 0x00000001
	.section .rom.0047b54f, "ax"
	.incbin "baserom.gba", 0x0047b54f, 0x00000001
	.section .rom.0047bcc3, "ax"
	.incbin "baserom.gba", 0x0047bcc3, 0x00000001
	.section .rom.0047d553, "ax"
	.incbin "baserom.gba", 0x0047d553, 0x00000001
	.section .rom.0047df21, "ax"
	.incbin "baserom.gba", 0x0047df21, 0x00000003
	.section .rom.0047eb3f, "ax"
	.incbin "baserom.gba", 0x0047eb3f, 0x00000001
	.section .rom.004811f3, "ax"
	.incbin "baserom.gba", 0x004811f3, 0x00000001
	.section .rom.00483906, "ax"
	.incbin "baserom.gba", 0x00483906, 0x00000002
	.section .rom.0048885b, "ax"
	.incbin "baserom.gba", 0x0048885b, 0x00000001
	.section .rom.0048cb85, "ax"
	.incbin "baserom.gba", 0x0048cb85, 0x00000003
	.section .rom.0048d772, "ax"
	.incbin "baserom.gba", 0x0048d772, 0x00000002
	.section .rom.00492327, "ax"
	.incbin "baserom.gba", 0x00492327, 0x00000001
	.section .rom.00494691, "ax"
	.incbin "baserom.gba", 0x00494691, 0x00000003
	.section .rom.00496033, "ax"
	.incbin "baserom.gba", 0x00496033, 0x00000001
	.section .rom.0049a8f5, "ax"
	.incbin "baserom.gba", 0x0049a8f5, 0x00000003
	.section .rom.0049e9bd, "ax"
	.incbin "baserom.gba", 0x0049e9bd, 0x00000003
	.section .rom.004a2086, "ax"
	.incbin "baserom.gba", 0x004a2086, 0x00000002
	.section .rom.004aa057, "ax"
	.incbin "baserom.gba", 0x004aa057, 0x00000001
	.section .rom.004b1367, "ax"
	.incbin "baserom.gba", 0x004b1367, 0x00000001
	.section .rom.004b62e7, "ax"
	.incbin "baserom.gba", 0x004b62e7, 0x00000001
	.section .rom.004bac97, "ax"
	.incbin "baserom.gba", 0x004bac97, 0x0000051d
	.section .rom.004c05c3, "ax"
	.incbin "baserom.gba", 0x004c05c3, 0x00000001
	.section .rom.004c0751, "ax"
	.incbin "baserom.gba", 0x004c0751, 0x00000003
	.section .rom.004c57b1, "ax"
	.incbin "baserom.gba", 0x004c57b1, 0x00000003
	.section .rom.004c9d1d, "ax"
	.incbin "baserom.gba", 0x004c9d1d, 0x00000003
	.section .rom.004ceee5, "ax"
	.incbin "baserom.gba", 0x004ceee5, 0x00000003
	.section .rom.004cf07b, "ax"
	.incbin "baserom.gba", 0x004cf07b, 0x00000001
	.section .rom.004d1ccf, "ax"
	.incbin "baserom.gba", 0x004d1ccf, 0x00000001
	.section .rom.004d8983, "ax"
	.incbin "baserom.gba", 0x004d8983, 0x00000001
	.section .rom.004dbabe, "ax"
	.incbin "baserom.gba", 0x004dbabe, 0x00000002
	.section .rom.004dbc3f, "ax"
	.incbin "baserom.gba", 0x004dbc3f, 0x00000001
	.section .rom.004de3f9, "ax"
	.incbin "baserom.gba", 0x004de3f9, 0x00000003
	.section .rom.004e0a42, "ax"
	.incbin "baserom.gba", 0x004e0a42, 0x00000002
	.section .rom.004e3126, "ax"
	.incbin "baserom.gba", 0x004e3126, 0x00000002
	.section .rom.004e419b, "ax"
	.incbin "baserom.gba", 0x004e419b, 0x00000001
	.section .rom.004e62ee, "ax"
	.incbin "baserom.gba", 0x004e62ee, 0x00000002
	.section .rom.004e6481, "ax"
	.incbin "baserom.gba", 0x004e6481, 0x00000003
	.section .rom.004e8cc3, "ax"
	.incbin "baserom.gba", 0x004e8cc3, 0x00000001
	.section .rom.004eaad9, "ax"
	.incbin "baserom.gba", 0x004eaad9, 0x00000003
	.section .rom.004ed1ba, "ax"
	.incbin "baserom.gba", 0x004ed1ba, 0x00000002
	.section .rom.004ee301, "ax"
	.incbin "baserom.gba", 0x004ee301, 0x00000003
	.section .rom.004f1415, "ax"
	.incbin "baserom.gba", 0x004f1415, 0x00000003
	.section .rom.004f557d, "ax"
	.incbin "baserom.gba", 0x004f557d, 0x00000003
	.section .rom.004f6b7a, "ax"
	.incbin "baserom.gba", 0x004f6b7a, 0x00000002
	.section .rom.004f7e43, "ax"
	.incbin "baserom.gba", 0x004f7e43, 0x00000001
	.section .rom.004fb25b, "ax"
	.incbin "baserom.gba", 0x004fb25b, 0x00000001
	.section .rom.004fb3e9, "ax"
	.incbin "baserom.gba", 0x004fb3e9, 0x00000003
	.section .rom.004fd19a, "ax"
	.incbin "baserom.gba", 0x004fd19a, 0x00000002
	.section .rom.00500d11, "ax"
	.incbin "baserom.gba", 0x00500d11, 0x00000003
	.section .rom.0050221f, "ax"
	.incbin "baserom.gba", 0x0050221f, 0x00000001
	.section .rom.00502df3, "ax"
	.incbin "baserom.gba", 0x00502df3, 0x00000001
	.section .rom.00502ef1, "ax"
	.incbin "baserom.gba", 0x00502ef1, 0x00000003
	.section .rom.005042d6, "ax"
	.incbin "baserom.gba", 0x005042d6, 0x00000002
	.section .rom.00505459, "ax"
	.incbin "baserom.gba", 0x00505459, 0x00000003
	.section .rom.00505de2, "ax"
	.incbin "baserom.gba", 0x00505de2, 0x00000002
	.section .rom.00508a5f, "ax"
	.incbin "baserom.gba", 0x00508a5f, 0x00000001
	.section .rom.00508b42, "ax"
	.incbin "baserom.gba", 0x00508b42, 0x00000002
	.section .rom.00509d2d, "ax"
	.incbin "baserom.gba", 0x00509d2d, 0x00000003
	.section .rom.0050b9b5, "ax"
	.incbin "baserom.gba", 0x0050b9b5, 0x00000003
	.section .rom.0050bec3, "ax"
	.incbin "baserom.gba", 0x0050bec3, 0x00000001
	.section .rom.0050c003, "ax"
	.incbin "baserom.gba", 0x0050c003, 0x00000001
	.section .rom.0050d21e, "ax"
	.incbin "baserom.gba", 0x0050d21e, 0x00000002
	.section .rom.0050d379, "ax"
	.incbin "baserom.gba", 0x0050d379, 0x00000003
	.section .rom.0051179f, "ax"
	.incbin "baserom.gba", 0x0051179f, 0x00000001
	.section .rom.005134ce, "ax"
	.incbin "baserom.gba", 0x005134ce, 0x00000002
	.section .rom.00514247, "ax"
	.incbin "baserom.gba", 0x00514247, 0x00000001
	.section .rom.0051437b, "ax"
	.incbin "baserom.gba", 0x0051437b, 0x00000001
	.section .rom.005167fb, "ax"
	.incbin "baserom.gba", 0x005167fb, 0x00000001
	.section .rom.00517577, "ax"
	.incbin "baserom.gba", 0x00517577, 0x00000001
	.section .rom.00518056, "ax"
	.incbin "baserom.gba", 0x00518056, 0x00000002
	.section .rom.00518ece, "ax"
	.incbin "baserom.gba", 0x00518ece, 0x00000002
	.section .rom.0051afa2, "ax"
	.incbin "baserom.gba", 0x0051afa2, 0x00000002
	.section .rom.0051b8e9, "ax"
	.incbin "baserom.gba", 0x0051b8e9, 0x00000003
	.section .rom.0051c536, "ax"
	.incbin "baserom.gba", 0x0051c536, 0x00000002
	.section .rom.0051c75d, "ax"
	.incbin "baserom.gba", 0x0051c75d, 0x00000003
	.section .rom.0051e09f, "ax"
	.incbin "baserom.gba", 0x0051e09f, 0x00000001
	.section .rom.0051e1b3, "ax"
	.incbin "baserom.gba", 0x0051e1b3, 0x00000001
	.section .rom.0051fb79, "ax"
	.incbin "baserom.gba", 0x0051fb79, 0x00000003
	.section .rom.0051fcc3, "ax"
	.incbin "baserom.gba", 0x0051fcc3, 0x00000001
	.section .rom.00521d8f, "ax"
	.incbin "baserom.gba", 0x00521d8f, 0x00000001
	.section .rom.00521eb7, "ax"
	.incbin "baserom.gba", 0x00521eb7, 0x00000001
	.section .rom.0052373f, "ax"
	.incbin "baserom.gba", 0x0052373f, 0x00000001
	.section .rom.005259e1, "ax"
	.incbin "baserom.gba", 0x005259e1, 0x00000003
	.section .rom.00525b23, "ax"
	.incbin "baserom.gba", 0x00525b23, 0x00000001
	.section .rom.00527b2b, "ax"
	.incbin "baserom.gba", 0x00527b2b, 0x00000001
	.section .rom.00527c36, "ax"
	.incbin "baserom.gba", 0x00527c36, 0x00000002
	.section .rom.00529c1a, "ax"
	.incbin "baserom.gba", 0x00529c1a, 0x00000002
	.section .rom.0052b33f, "ax"
	.incbin "baserom.gba", 0x0052b33f, 0x00000001
	.section .rom.0052c3f3, "ax"
	.incbin "baserom.gba", 0x0052c3f3, 0x00000001
	.section .rom.0052e0da, "ax"
	.incbin "baserom.gba", 0x0052e0da, 0x00000002
	.section .rom.0052e235, "ax"
	.incbin "baserom.gba", 0x0052e235, 0x00000003
	.section .rom.0053017e, "ax"
	.incbin "baserom.gba", 0x0053017e, 0x00000002
	.section .rom.00530303, "ax"
	.incbin "baserom.gba", 0x00530303, 0x00000001
	.section .rom.00532e55, "ax"
	.incbin "baserom.gba", 0x00532e55, 0x00000003
	.section .rom.0053551a, "ax"
	.incbin "baserom.gba", 0x0053551a, 0x00000002
	.section .rom.00537c72, "ax"
	.incbin "baserom.gba", 0x00537c72, 0x00000002
	.section .rom.005394c5, "ax"
	.incbin "baserom.gba", 0x005394c5, 0x00000003
	.section .rom.0053bbc5, "ax"
	.incbin "baserom.gba", 0x0053bbc5, 0x00000003
	.section .rom.0053d991, "ax"
	.incbin "baserom.gba", 0x0053d991, 0x00000003
	.section .rom.0053fedb, "ax"
	.incbin "baserom.gba", 0x0053fedb, 0x00000001
	.section .rom.005411a6, "ax"
	.incbin "baserom.gba", 0x005411a6, 0x00000002
	.section .rom.00541e57, "ax"
	.incbin "baserom.gba", 0x00541e57, 0x00000001
	.section .rom.005428c1, "ax"
	.incbin "baserom.gba", 0x005428c1, 0x00000003
	.section .rom.005429b7, "ax"
	.incbin "baserom.gba", 0x005429b7, 0x00000001
	.section .rom.00544416, "ax"
	.incbin "baserom.gba", 0x00544416, 0x00000002
	.section .rom.005451ba, "ax"
	.incbin "baserom.gba", 0x005451ba, 0x00000002
	.section .rom.00546cf3, "ax"
	.incbin "baserom.gba", 0x00546cf3, 0x00000001
	.section .rom.00549be1, "ax"
	.incbin "baserom.gba", 0x00549be1, 0x00000003
	.section .rom.005513c7, "ax"
	.incbin "baserom.gba", 0x005513c7, 0x00000001
	.section .rom.00551493, "ax"
	.incbin "baserom.gba", 0x00551493, 0x00000001
	.section .rom.00556d3e, "ax"
	.incbin "baserom.gba", 0x00556d3e, 0x00000002
	.section .rom.00556e7f, "ax"
	.incbin "baserom.gba", 0x00556e7f, 0x00000001
	.section .rom.005582ad, "ax"
	.incbin "baserom.gba", 0x005582ad, 0x00000003
	.section .rom.0055afea, "ax"
	.incbin "baserom.gba", 0x0055afea, 0x00000002
	.section .rom.0055d0e6, "ax"
	.incbin "baserom.gba", 0x0055d0e6, 0x00000002
	.section .rom.00564aee, "ax"
	.incbin "baserom.gba", 0x00564aee, 0x00000002
	.section .rom.00564c8a, "ax"
	.incbin "baserom.gba", 0x00564c8a, 0x00000002
	.section .rom.00569525, "ax"
	.incbin "baserom.gba", 0x00569525, 0x00000003
	.section .rom.0056ba26, "ax"
	.incbin "baserom.gba", 0x0056ba26, 0x00000002
	.section .rom.00573417, "ax"
	.incbin "baserom.gba", 0x00573417, 0x00000001
	.section .rom.005735b2, "ax"
	.incbin "baserom.gba", 0x005735b2, 0x00000002
	.section .rom.00576161, "ax"
	.incbin "baserom.gba", 0x00576161, 0x00000003
	.section .rom.00578235, "ax"
	.incbin "baserom.gba", 0x00578235, 0x00000003
	.section .rom.0057a28e, "ax"
	.incbin "baserom.gba", 0x0057a28e, 0x00001cee
	.section .rom.0057d725, "ax"
	.incbin "baserom.gba", 0x0057d725, 0x00000003
	.section .rom.00580401, "ax"
	.incbin "baserom.gba", 0x00580401, 0x00000003
	.section .rom.005821d3, "ax"
	.incbin "baserom.gba", 0x005821d3, 0x00000001
	.section .rom.00582313, "ax"
	.incbin "baserom.gba", 0x00582313, 0x00000001
	.section .rom.0058536d, "ax"
	.incbin "baserom.gba", 0x0058536d, 0x00000003
	.section .rom.005882ea, "ax"
	.incbin "baserom.gba", 0x005882ea, 0x00000002
	.section .rom.0058abb1, "ax"
	.incbin "baserom.gba", 0x0058abb1, 0x00000003
	.section .rom.00591b19, "ax"
	.incbin "baserom.gba", 0x00591b19, 0x00000003
	.section .rom.005933bf, "ax"
	.incbin "baserom.gba", 0x005933bf, 0x00000001
	.section .rom.00593cbe, "ax"
	.incbin "baserom.gba", 0x00593cbe, 0x00000002
	.section .rom.00594a37, "ax"
	.incbin "baserom.gba", 0x00594a37, 0x00000001
	.section .rom.00594b97, "ax"
	.incbin "baserom.gba", 0x00594b97, 0x00000001
	.section .rom.005967ef, "ax"
	.incbin "baserom.gba", 0x005967ef, 0x00000001
	.section .rom.0059808b, "ax"
	.incbin "baserom.gba", 0x0059808b, 0x00000001
	.section .rom.0059820f, "ax"
	.incbin "baserom.gba", 0x0059820f, 0x00000001
	.section .rom.0059ad4b, "ax"
	.incbin "baserom.gba", 0x0059ad4b, 0x00000001
	.section .rom.0059d4c5, "ax"
	.incbin "baserom.gba", 0x0059d4c5, 0x00000003
	.section .rom.0059e4b2, "ax"
	.incbin "baserom.gba", 0x0059e4b2, 0x00000002
	.section .rom.0059fb7f, "ax"
	.incbin "baserom.gba", 0x0059fb7f, 0x00000001
	.section .rom.0059fd29, "ax"
	.incbin "baserom.gba", 0x0059fd29, 0x00000003
	.section .rom.005a2abd, "ax"
	.incbin "baserom.gba", 0x005a2abd, 0x00000003
	.section .rom.005a6b52, "ax"
	.incbin "baserom.gba", 0x005a6b52, 0x00000002
	.section .rom.005a6c93, "ax"
	.incbin "baserom.gba", 0x005a6c93, 0x00000001
	.section .rom.005a8b5b, "ax"
	.incbin "baserom.gba", 0x005a8b5b, 0x00000001
	.section .rom.005a8cfd, "ax"
	.incbin "baserom.gba", 0x005a8cfd, 0x00000003
	.section .rom.005aad9f, "ax"
	.incbin "baserom.gba", 0x005aad9f, 0x00000001
	.section .rom.005abd2c, "ax"
	.incbin "baserom.gba", 0x005abd2c, 0x000022d8
	.section .rom.005b0af5, "ax"
	.incbin "baserom.gba", 0x005b0af5, 0x00000003
	.section .rom.005b0c7a, "ax"
	.incbin "baserom.gba", 0x005b0c7a, 0x00000002
	.section .rom.005b83c7, "ax"
	.incbin "baserom.gba", 0x005b83c7, 0x00000001
	.section .rom.005b88d1, "ax"
	.incbin "baserom.gba", 0x005b88d1, 0x00000003
	.section .rom.005b9fa9, "ax"
	.incbin "baserom.gba", 0x005b9fa9, 0x00000003
	.section .rom.005ba14d, "ax"
	.incbin "baserom.gba", 0x005ba14d, 0x00000003
	.section .rom.005bba52, "ax"
	.incbin "baserom.gba", 0x005bba52, 0x00000002
	.section .rom.005bd27f, "ax"
	.incbin "baserom.gba", 0x005bd27f, 0x00000001
	.section .rom.005bd3fd, "ax"
	.incbin "baserom.gba", 0x005bd3fd, 0x00000003
	.section .rom.005c1cf1, "ax"
	.incbin "baserom.gba", 0x005c1cf1, 0x00000003
	.section .rom.005c2d36, "ax"
	.incbin "baserom.gba", 0x005c2d36, 0x00000002
	.section .rom.005c3b5a, "ax"
	.incbin "baserom.gba", 0x005c3b5a, 0x00000002
	.section .rom.005c4e9f, "ax"
	.incbin "baserom.gba", 0x005c4e9f, 0x00000001
	.section .rom.005c4ff9, "ax"
	.incbin "baserom.gba", 0x005c4ff9, 0x00000003
	.section .rom.005c75b7, "ax"
	.incbin "baserom.gba", 0x005c75b7, 0x00000001
	.section .rom.005c772e, "ax"
	.incbin "baserom.gba", 0x005c772e, 0x00000002
	.section .rom.005cc021, "ax"
	.incbin "baserom.gba", 0x005cc021, 0x00000003
	.section .rom.005cca89, "ax"
	.incbin "baserom.gba", 0x005cca89, 0x00000003
	.section .rom.005cf0cb, "ax"
	.incbin "baserom.gba", 0x005cf0cb, 0x00000001
	.section .rom.005cf232, "ax"
	.incbin "baserom.gba", 0x005cf232, 0x00000002
	.section .rom.005d0379, "ax"
	.incbin "baserom.gba", 0x005d0379, 0x00000003
	.section .rom.005d20d7, "ax"
	.incbin "baserom.gba", 0x005d20d7, 0x00000001
	.section .rom.005d223e, "ax"
	.incbin "baserom.gba", 0x005d223e, 0x00000002
	.section .rom.005d4acd, "ax"
	.incbin "baserom.gba", 0x005d4acd, 0x00000003
	.section .rom.005d4c02, "ax"
	.incbin "baserom.gba", 0x005d4c02, 0x00000002
	.section .rom.005d783e, "ax"
	.incbin "baserom.gba", 0x005d783e, 0x00000002
	.section .rom.005dafdd, "ax"
	.incbin "baserom.gba", 0x005dafdd, 0x00000003
	.section .rom.005de32b, "ax"
	.incbin "baserom.gba", 0x005de32b, 0x00000001
	.section .rom.005de4a7, "ax"
	.incbin "baserom.gba", 0x005de4a7, 0x00000001
	.section .rom.005e1056, "ax"
	.incbin "baserom.gba", 0x005e1056, 0x00000002
	.section .rom.005e32ca, "ax"
	.incbin "baserom.gba", 0x005e32ca, 0x00000002
	.section .rom.005e5b3b, "ax"
	.incbin "baserom.gba", 0x005e5b3b, 0x00000001
	.section .rom.005e902f, "ax"
	.incbin "baserom.gba", 0x005e902f, 0x00000001
	.section .rom.005edc56, "ax"
	.incbin "baserom.gba", 0x005edc56, 0x00000002
	.section .rom.005ee8d9, "ax"
	.incbin "baserom.gba", 0x005ee8d9, 0x00000003
	.section .rom.005eea1b, "ax"
	.incbin "baserom.gba", 0x005eea1b, 0x00000001
	.section .rom.005eff8b, "ax"
	.incbin "baserom.gba", 0x005eff8b, 0x00000001
	.section .rom.005f00f6, "ax"
	.incbin "baserom.gba", 0x005f00f6, 0x00000002
	.section .rom.005f1e51, "ax"
	.incbin "baserom.gba", 0x005f1e51, 0x00000003
	.section .rom.005f1f93, "ax"
	.incbin "baserom.gba", 0x005f1f93, 0x00000001
	.section .rom.005f4792, "ax"
	.incbin "baserom.gba", 0x005f4792, 0x00000002
	.section .rom.005f48d3, "ax"
	.incbin "baserom.gba", 0x005f48d3, 0x00000001
	.section .rom.005f62eb, "ax"
	.incbin "baserom.gba", 0x005f62eb, 0x00000001
	.section .rom.005f7866, "ax"
	.incbin "baserom.gba", 0x005f7866, 0x00000002
	.section .rom.005f8da2, "ax"
	.incbin "baserom.gba", 0x005f8da2, 0x00000002
	.section .rom.005f8f32, "ax"
	.incbin "baserom.gba", 0x005f8f32, 0x00000002
	.section .rom.005fb1eb, "ax"
	.incbin "baserom.gba", 0x005fb1eb, 0x00000001
	.section .rom.005ff432, "ax"
	.incbin "baserom.gba", 0x005ff432, 0x00000002
	.section .rom.00601215, "ax"
	.incbin "baserom.gba", 0x00601215, 0x00000003
	.section .rom.006043c3, "ax"
	.incbin "baserom.gba", 0x006043c3, 0x00000001
	.section .rom.006069f7, "ax"
	.incbin "baserom.gba", 0x006069f7, 0x00000001
	.section .rom.0060954f, "ax"
	.incbin "baserom.gba", 0x0060954f, 0x00000001
	.section .rom.00609717, "ax"
	.incbin "baserom.gba", 0x00609717, 0x00002905
	.section .rom.0060fb21, "ax"
	.incbin "baserom.gba", 0x0060fb21, 0x00000003
	.section .rom.00611092, "ax"
	.incbin "baserom.gba", 0x00611092, 0x00000002
	.section .rom.006138b9, "ax"
	.incbin "baserom.gba", 0x006138b9, 0x00000003
	.section .rom.00613a29, "ax"
	.incbin "baserom.gba", 0x00613a29, 0x00000003
	.section .rom.006162eb, "ax"
	.incbin "baserom.gba", 0x006162eb, 0x00000001
	.section .rom.00618ab3, "ax"
	.incbin "baserom.gba", 0x00618ab3, 0x00000001
	.section .rom.00619dad, "ax"
	.incbin "baserom.gba", 0x00619dad, 0x00000003
	.section .rom.0061b6f3, "ax"
	.incbin "baserom.gba", 0x0061b6f3, 0x00000001
	.section .rom.00626b86, "ax"
	.incbin "baserom.gba", 0x00626b86, 0x00000002
	.section .rom.00626d46, "ax"
	.incbin "baserom.gba", 0x00626d46, 0x00000002
	.section .rom.0062a0ad, "ax"
	.incbin "baserom.gba", 0x0062a0ad, 0x00000003
	.section .rom.0062c59e, "ax"
	.incbin "baserom.gba", 0x0062c59e, 0x00000002
	.section .rom.0062f37f, "ax"
	.incbin "baserom.gba", 0x0062f37f, 0x00000001
	.section .rom.0062ff19, "ax"
	.incbin "baserom.gba", 0x0062ff19, 0x00000003
	.section .rom.00630d7a, "ax"
	.incbin "baserom.gba", 0x00630d7a, 0x00000002
	.section .rom.00630ed2, "ax"
	.incbin "baserom.gba", 0x00630ed2, 0x00000002
	.section .rom.00638d1f, "ax"
	.incbin "baserom.gba", 0x00638d1f, 0x00000001
	.section .rom.00638e8b, "ax"
	.incbin "baserom.gba", 0x00638e8b, 0x00000001
	.section .rom.0063bc27, "ax"
	.incbin "baserom.gba", 0x0063bc27, 0x00000001
	.section .rom.0063ddb6, "ax"
	.incbin "baserom.gba", 0x0063ddb6, 0x00000002
	.section .rom.0063e4be, "ax"
	.incbin "baserom.gba", 0x0063e4be, 0x00000002
	.section .rom.0063f0d2, "ax"
	.incbin "baserom.gba", 0x0063f0d2, 0x00000002
	.section .rom.006501a5, "ax"
	.incbin "baserom.gba", 0x006501a5, 0x00000003
	.section .rom.0065033a, "ax"
	.incbin "baserom.gba", 0x0065033a, 0x00000002
	.section .rom.0065184a, "ax"
	.incbin "baserom.gba", 0x0065184a, 0x00000002
	.section .rom.0065362f, "ax"
	.incbin "baserom.gba", 0x0065362f, 0x00000001
	.section .rom.006546c2, "ax"
	.incbin "baserom.gba", 0x006546c2, 0x00000002
	.section .rom.00656932, "ax"
	.incbin "baserom.gba", 0x00656932, 0x00000002
	.section .rom.00656ac9, "ax"
	.incbin "baserom.gba", 0x00656ac9, 0x00000003
	.section .rom.00658a6f, "ax"
	.incbin "baserom.gba", 0x00658a6f, 0x00000001
	.section .rom.0065d846, "ax"
	.incbin "baserom.gba", 0x0065d846, 0x00000002
	.section .rom.006612a7, "ax"
	.incbin "baserom.gba", 0x006612a7, 0x00000001
	.section .rom.00663fdd, "ax"
	.incbin "baserom.gba", 0x00663fdd, 0x00000003
	.section .rom.00666325, "ax"
	.incbin "baserom.gba", 0x00666325, 0x00000003
	.section .rom.00667b03, "ax"
	.incbin "baserom.gba", 0x00667b03, 0x00000001
	.section .rom.00667cb5, "ax"
	.incbin "baserom.gba", 0x00667cb5, 0x00000003
	.section .rom.00678656, "ax"
	.incbin "baserom.gba", 0x00678656, 0x00000002
	.section .rom.006787b5, "ax"
	.incbin "baserom.gba", 0x006787b5, 0x00000003
	.section .rom.0067bf0f, "ax"
	.incbin "baserom.gba", 0x0067bf0f, 0x00000001
	.section .rom.0067c072, "ax"
	.incbin "baserom.gba", 0x0067c072, 0x00000002
	.section .rom.00681d55, "ax"
	.incbin "baserom.gba", 0x00681d55, 0x00000003
	.section .rom.0068473b, "ax"
	.incbin "baserom.gba", 0x0068473b, 0x00000001
	.section .rom.00686946, "ax"
	.incbin "baserom.gba", 0x00686946, 0x00000002
	.section .rom.00686a87, "ax"
	.incbin "baserom.gba", 0x00686a87, 0x00000001
	.section .rom.0068849e, "ax"
	.incbin "baserom.gba", 0x0068849e, 0x00000002
	.section .rom.0068f205, "ax"
	.incbin "baserom.gba", 0x0068f205, 0x00000003
	.section .rom.0068f383, "ax"
	.incbin "baserom.gba", 0x0068f383, 0x00000001
	.section .rom.0069132d, "ax"
	.incbin "baserom.gba", 0x0069132d, 0x00000003
	.section .rom.00693aa6, "ax"
	.incbin "baserom.gba", 0x00693aa6, 0x00000002
	.section .rom.0069530d, "ax"
	.incbin "baserom.gba", 0x0069530d, 0x00000003
	.section .rom.00698d2e, "ax"
	.incbin "baserom.gba", 0x00698d2e, 0x00000002
	.section .rom.00698ef7, "ax"
	.incbin "baserom.gba", 0x00698ef7, 0x00000001
	.section .rom.006a3edf, "ax"
	.incbin "baserom.gba", 0x006a3edf, 0x00000001
	.section .rom.006a602b, "ax"
	.incbin "baserom.gba", 0x006a602b, 0x00000001
	.section .rom.006a61b5, "ax"
	.incbin "baserom.gba", 0x006a61b5, 0x00000003
	.section .rom.006a7c3a, "ax"
	.incbin "baserom.gba", 0x006a7c3a, 0x00000002
	.section .rom.006aac71, "ax"
	.incbin "baserom.gba", 0x006aac71, 0x00000003
	.section .rom.006aae37, "ax"
	.incbin "baserom.gba", 0x006aae37, 0x00000001
	.section .rom.006abd6b, "ax"
	.incbin "baserom.gba", 0x006abd6b, 0x00000001
	.section .rom.006ade1e, "ax"
	.incbin "baserom.gba", 0x006ade1e, 0x00000002
	.section .rom.006adfc5, "ax"
	.incbin "baserom.gba", 0x006adfc5, 0x00000003
	.section .rom.006b2fef, "ax"
	.incbin "baserom.gba", 0x006b2fef, 0x00000001
	.section .rom.006b5d7f, "ax"
	.incbin "baserom.gba", 0x006b5d7f, 0x00000001
	.section .rom.006b5f3f, "ax"
	.incbin "baserom.gba", 0x006b5f3f, 0x00000001
	.section .rom.006b8433, "ax"
	.incbin "baserom.gba", 0x006b8433, 0x00000001
	.section .rom.006b8605, "ax"
	.incbin "baserom.gba", 0x006b8605, 0x00000003
	.section .rom.006ba696, "ax"
	.incbin "baserom.gba", 0x006ba696, 0x00000002
	.section .rom.006bc796, "ax"
	.incbin "baserom.gba", 0x006bc796, 0x00000002
	.section .rom.006bf34a, "ax"
	.incbin "baserom.gba", 0x006bf34a, 0x00000002
	.section .rom.006bfe37, "ax"
	.incbin "baserom.gba", 0x006bfe37, 0x00000001
	.section .rom.006c000a, "ax"
	.incbin "baserom.gba", 0x006c000a, 0x00000002
	.section .rom.006c2939, "ax"
	.incbin "baserom.gba", 0x006c2939, 0x00000003
	.section .rom.006c40d1, "ax"
	.incbin "baserom.gba", 0x006c40d1, 0x00000003
	.section .rom.006c5345, "ax"
	.incbin "baserom.gba", 0x006c5345, 0x00000003
	.section .rom.006c77d5, "ax"
	.incbin "baserom.gba", 0x006c77d5, 0x00000003
	.section .rom.006c9b21, "ax"
	.incbin "baserom.gba", 0x006c9b21, 0x00000003
	.section .rom.006cad0b, "ax"
	.incbin "baserom.gba", 0x006cad0b, 0x00000001
	.section .rom.006cae92, "ax"
	.incbin "baserom.gba", 0x006cae92, 0x00000002
	.section .rom.006cf3de, "ax"
	.incbin "baserom.gba", 0x006cf3de, 0x00000002
	.section .rom.006cf533, "ax"
	.incbin "baserom.gba", 0x006cf533, 0x00000001
	.section .rom.006cf673, "ax"
	.incbin "baserom.gba", 0x006cf673, 0x00000001
	.section .rom.006d0347, "ax"
	.incbin "baserom.gba", 0x006d0347, 0x00000001
	.section .rom.006d04c6, "ax"
	.incbin "baserom.gba", 0x006d04c6, 0x00000002
	.section .rom.006d1d7f, "ax"
	.incbin "baserom.gba", 0x006d1d7f, 0x00000001
	.section .rom.006d1f01, "ax"
	.incbin "baserom.gba", 0x006d1f01, 0x00000003
	.section .rom.006d3993, "ax"
	.incbin "baserom.gba", 0x006d3993, 0x00000001
	.section .rom.006d3b0d, "ax"
	.incbin "baserom.gba", 0x006d3b0d, 0x00000003
	.section .rom.006d584b, "ax"
	.incbin "baserom.gba", 0x006d584b, 0x00000001
	.section .rom.006d5977, "ax"
	.incbin "baserom.gba", 0x006d5977, 0x00000001
	.section .rom.006d5aed, "ax"
	.incbin "baserom.gba", 0x006d5aed, 0x00000003
	.section .rom.006d7993, "ax"
	.incbin "baserom.gba", 0x006d7993, 0x00000001
	.section .rom.006d7b19, "ax"
	.incbin "baserom.gba", 0x006d7b19, 0x00000003
	.section .rom.006d8a77, "ax"
	.incbin "baserom.gba", 0x006d8a77, 0x00000001
	.section .rom.006d9d1b, "ax"
	.incbin "baserom.gba", 0x006d9d1b, 0x00000001
	.section .rom.006d9ed6, "ax"
	.incbin "baserom.gba", 0x006d9ed6, 0x00000002
	.section .rom.006dc97a, "ax"
	.incbin "baserom.gba", 0x006dc97a, 0x00000002
	.section .rom.006dcaed, "ax"
	.incbin "baserom.gba", 0x006dcaed, 0x00000003
	.section .rom.006dded5, "ax"
	.incbin "baserom.gba", 0x006dded5, 0x00000003
	.section .rom.006e09b5, "ax"
	.incbin "baserom.gba", 0x006e09b5, 0x00000003
	.section .rom.006e0b5f, "ax"
	.incbin "baserom.gba", 0x006e0b5f, 0x00000001
	.section .rom.006e2f5a, "ax"
	.incbin "baserom.gba", 0x006e2f5a, 0x00000002
	.section .rom.006e30e9, "ax"
	.incbin "baserom.gba", 0x006e30e9, 0x00000003
	.section .rom.006e6369, "ax"
	.incbin "baserom.gba", 0x006e6369, 0x00000003
	.section .rom.006e654a, "ax"
	.incbin "baserom.gba", 0x006e654a, 0x00000002
	.section .rom.006e8adf, "ax"
	.incbin "baserom.gba", 0x006e8adf, 0x00000001
	.section .rom.006e8c6e, "ax"
	.incbin "baserom.gba", 0x006e8c6e, 0x00000002
	.section .rom.006eb866, "ax"
	.incbin "baserom.gba", 0x006eb866, 0x00000002
	.section .rom.006ec3b3, "ax"
	.incbin "baserom.gba", 0x006ec3b3, 0x00000001
	.section .rom.006ec51a, "ax"
	.incbin "baserom.gba", 0x006ec51a, 0x00000002
	.section .rom.006eef2f, "ax"
	.incbin "baserom.gba", 0x006eef2f, 0x00000001
	.section .rom.006f0b6e, "ax"
	.incbin "baserom.gba", 0x006f0b6e, 0x00000002
	.section .rom.006f102b, "ax"
	.incbin "baserom.gba", 0x006f102b, 0x00000001
	.section .rom.006f23a3, "ax"
	.incbin "baserom.gba", 0x006f23a3, 0x00000001
	.section .rom.006f38e7, "ax"
	.incbin "baserom.gba", 0x006f38e7, 0x00000001
	.section .rom.006f3a71, "ax"
	.incbin "baserom.gba", 0x006f3a71, 0x00000003
	.section .rom.006f4b4f, "ax"
	.incbin "baserom.gba", 0x006f4b4f, 0x00000001
	.section .rom.006f4cdd, "ax"
	.incbin "baserom.gba", 0x006f4cdd, 0x00000003
	.section .rom.006f6931, "ax"
	.incbin "baserom.gba", 0x006f6931, 0x00000003
	.section .rom.006f9313, "ax"
	.incbin "baserom.gba", 0x006f9313, 0x00000001
	.section .rom.006fc83d, "ax"
	.incbin "baserom.gba", 0x006fc83d, 0x00000003
	.section .rom.006fe147, "ax"
	.incbin "baserom.gba", 0x006fe147, 0x00000001
	.section .rom.007001ef, "ax"
	.incbin "baserom.gba", 0x007001ef, 0x00000001
	.section .rom.00702c43, "ax"
	.incbin "baserom.gba", 0x00702c43, 0x00000001
	.section .rom.0070488f, "ax"
	.incbin "baserom.gba", 0x0070488f, 0x00000001
	.section .rom.00705cd5, "ax"
	.incbin "baserom.gba", 0x00705cd5, 0x00000003
	.section .rom.00707b55, "ax"
	.incbin "baserom.gba", 0x00707b55, 0x00000003
	.section .rom.00709726, "ax"
	.incbin "baserom.gba", 0x00709726, 0x00000002
	.section .rom.0070be5a, "ax"
	.incbin "baserom.gba", 0x0070be5a, 0x00000002
	.section .rom.0070da9f, "ax"
	.incbin "baserom.gba", 0x0070da9f, 0x00000001
	.section .rom.0070eee5, "ax"
	.incbin "baserom.gba", 0x0070eee5, 0x00000003
	.section .rom.00710a63, "ax"
	.incbin "baserom.gba", 0x00710a63, 0x00000001
	.section .rom.00711c93, "ax"
	.incbin "baserom.gba", 0x00711c93, 0x00000001
	.section .rom.00711ded, "ax"
	.incbin "baserom.gba", 0x00711ded, 0x00000003
	.section .rom.007152f3, "ax"
	.incbin "baserom.gba", 0x007152f3, 0x00000001
	.section .rom.0071545e, "ax"
	.incbin "baserom.gba", 0x0071545e, 0x00000002
	.section .rom.00717dae, "ax"
	.incbin "baserom.gba", 0x00717dae, 0x00001166
	.section .rom.0071a6f9, "ax"
	.incbin "baserom.gba", 0x0071a6f9, 0x00000003
	.section .rom.0071d93f, "ax"
	.incbin "baserom.gba", 0x0071d93f, 0x00000001
	.section .rom.00720236, "ax"
	.incbin "baserom.gba", 0x00720236, 0x00000002
	.section .rom.0072956f, "ax"
	.incbin "baserom.gba", 0x0072956f, 0x00000001
	.section .rom.0072a89e, "ax"
	.incbin "baserom.gba", 0x0072a89e, 0x00000002
	.section .rom.0072ba97, "ax"
	.incbin "baserom.gba", 0x0072ba97, 0x00000001
	.section .rom.0072bbf9, "ax"
	.incbin "baserom.gba", 0x0072bbf9, 0x00000003
	.section .rom.0072e5c5, "ax"
	.incbin "baserom.gba", 0x0072e5c5, 0x00000003
	.section .rom.00731af1, "ax"
	.incbin "baserom.gba", 0x00731af1, 0x00000003
	.section .rom.00735f2f, "ax"
	.incbin "baserom.gba", 0x00735f2f, 0x00000001
	.section .rom.007360a2, "ax"
	.incbin "baserom.gba", 0x007360a2, 0x00000002
	.section .rom.0073a307, "ax"
	.incbin "baserom.gba", 0x0073a307, 0x00000001
	.section .rom.0073db22, "ax"
	.incbin "baserom.gba", 0x0073db22, 0x00000002
	.section .rom.0073f54a, "ax"
	.incbin "baserom.gba", 0x0073f54a, 0x00000002
	.section .rom.00741bf7, "ax"
	.incbin "baserom.gba", 0x00741bf7, 0x00000001
	.section .rom.00743c5b, "ax"
	.incbin "baserom.gba", 0x00743c5b, 0x00000001
	.section .rom.00745b17, "ax"
	.incbin "baserom.gba", 0x00745b17, 0x00000001
	.section .rom.00748d61, "ax"
	.incbin "baserom.gba", 0x00748d61, 0x00000003
	.section .rom.0074a723, "ax"
	.incbin "baserom.gba", 0x0074a723, 0x00000001
	.section .rom.0074a895, "ax"
	.incbin "baserom.gba", 0x0074a895, 0x00000003
	.section .rom.0074d8a2, "ax"
	.incbin "baserom.gba", 0x0074d8a2, 0x00000002
	.section .rom.0074da5a, "ax"
	.incbin "baserom.gba", 0x0074da5a, 0x00000002
	.section .rom.0074f2cb, "ax"
	.incbin "baserom.gba", 0x0074f2cb, 0x00000001
	.section .rom.0074f44a, "ax"
	.incbin "baserom.gba", 0x0074f44a, 0x00000002
	.section .rom.00752c53, "ax"
	.incbin "baserom.gba", 0x00752c53, 0x00000001
	.section .rom.00752dc7, "ax"
	.incbin "baserom.gba", 0x00752dc7, 0x00000001
	.section .rom.00753b8f, "ax"
	.incbin "baserom.gba", 0x00753b8f, 0x00000001
	.section .rom.00753d35, "ax"
	.incbin "baserom.gba", 0x00753d35, 0x00000003
	.section .rom.00755d90, "ax"
	.incbin "baserom.gba", 0x00755d90, 0x000000e4
	.section .rom.00758b8b, "ax"
	.incbin "baserom.gba", 0x00758b8b, 0x00000001
	.section .rom.00758d72, "ax"
	.incbin "baserom.gba", 0x00758d72, 0x00000002
	.section .rom.0075c00e, "ax"
	.incbin "baserom.gba", 0x0075c00e, 0x00004606
	.section .rom.0076071a, "ax"
	.incbin "baserom.gba", 0x0076071a, 0x00000002
	.section .rom.007622ed, "ax"
	.incbin "baserom.gba", 0x007622ed, 0x00000003
	.section .rom.00763d9d, "ax"
	.incbin "baserom.gba", 0x00763d9d, 0x00000003
	.section .rom.00763ef9, "ax"
	.incbin "baserom.gba", 0x00763ef9, 0x00000003
	.section .rom.0076a13f, "ax"
	.incbin "baserom.gba", 0x0076a13f, 0x00000001
	.section .rom.0076a243, "ax"
	.incbin "baserom.gba", 0x0076a243, 0x00000001
	.section .rom.0076ac5f, "ax"
	.incbin "baserom.gba", 0x0076ac5f, 0x00000001
	.section .rom.0076ad45, "ax"
	.incbin "baserom.gba", 0x0076ad45, 0x00000003
	.section .rom.0076afe1, "ax"
	.incbin "baserom.gba", 0x0076afe1, 0x00000003
	.section .rom.0076f03e, "ax"
	.incbin "baserom.gba", 0x0076f03e, 0x00000002
	.section .rom.0076f5f2, "ax"
	.incbin "baserom.gba", 0x0076f5f2, 0x00000002
	.section .rom.00770587, "ax"
	.incbin "baserom.gba", 0x00770587, 0x00000001
	.section .rom.00771f8a, "ax"
	.incbin "baserom.gba", 0x00771f8a, 0x00000002
	.section .rom.00772bf9, "ax"
	.incbin "baserom.gba", 0x00772bf9, 0x00000003
	.section .rom.00772ea1, "ax"
	.incbin "baserom.gba", 0x00772ea1, 0x00000003
	.section .rom.00773d26, "ax"
	.incbin "baserom.gba", 0x00773d26, 0x00000002
	.section .rom.00774eb7, "ax"
	.incbin "baserom.gba", 0x00774eb7, 0x00000001
	.section .rom.0077507f, "ax"
	.incbin "baserom.gba", 0x0077507f, 0x00000001
	.section .rom.007753cb, "ax"
	.incbin "baserom.gba", 0x007753cb, 0x0000051d
	.section .rom.00775c33, "ax"
	.incbin "baserom.gba", 0x00775c33, 0x0000051d
	.section .rom.0077649b, "ax"
	.incbin "baserom.gba", 0x0077649b, 0x0000051d
	.section .rom.00776d03, "ax"
	.incbin "baserom.gba", 0x00776d03, 0x0000051d
	.section .rom.0077756b, "ax"
	.incbin "baserom.gba", 0x0077756b, 0x0000051d
	.section .rom.00777dd3, "ax"
	.incbin "baserom.gba", 0x00777dd3, 0x0000051d
	.section .rom.0077863b, "ax"
	.incbin "baserom.gba", 0x0077863b, 0x0000051d
	.section .rom.00778ea3, "ax"
	.incbin "baserom.gba", 0x00778ea3, 0x0008715d
