@ tbs-ja's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Rom_Start
Rom_Start:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00004fe4, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00004fe4, 0x000001f4
	.section .rom.000056cc, "ax"
	.global SaveState_InitializeWorkspace
	.type SaveState_InitializeWorkspace, %function
	.thumb_func
SaveState_InitializeWorkspace:
	.incbin "baserom.gba", 0x000056cc, 0x00000144
	.section .rom.0000615c, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x0000615c, 0x000000e4
	.section .rom.00006408, "ax"
	.global SerialRuntime_BeginTransferB
	.type SerialRuntime_BeginTransferB, %function
	.thumb_func
SerialRuntime_BeginTransferB:
	.global Party_Check
	.type Party_Check, %function
	.thumb_func
Party_Check:
	.incbin "baserom.gba", 0x00006408, 0x00000050
	.section .rom.0000655c, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x0000655c, 0x0000023c
	.section .rom.0000686e, "ax"
	.incbin "baserom.gba", 0x0000686e, 0x00000002
	.section .rom.00007320, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x00007320, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x00007676, 0x00000106
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "baserom.gba", 0x0000777c, 0x00000020
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x0000779c, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x0000795c, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x00007970, 0x00000024
	.section .rom.000079b0, "ax"
	.incbin "baserom.gba", 0x000079b0, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x000079b8, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a0c, 0x00000014
	.section .rom.00007a68, "ax"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .rom.00007aa4, "ax"
	.incbin "baserom.gba", 0x00007aa4, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007abc, 0x00000058
	.section .rom.00007b38, "ax"
	.incbin "baserom.gba", 0x00007b38, 0x0000008c
	.section .rom.00007bcc, "ax"
	.incbin "baserom.gba", 0x00007bcc, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007be4, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c10, 0x0000002c
	.section .rom.00007c64, "ax"
	.incbin "baserom.gba", 0x00007c64, 0x0000139c
	.section .rom.000092b8, "ax"
	.incbin "baserom.gba", 0x000092b8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000097b8, 0x00000400
	.section .rom.0000a0f8, "ax"
	.global Transform_UpdateVertices
Transform_UpdateVertices:
	.incbin "baserom.gba", 0x0000a0f8, 0x00000284
	.global Transform_UpdateVerticesEnd
Transform_UpdateVerticesEnd:
	.section .rom.0000a494, "ax"
	.global Object_UpdateAll
Object_UpdateAll:
	.incbin "baserom.gba", 0x0000a494, 0x000004e8
	.global Object_UpdateAllEnd
Object_UpdateAllEnd:
	.section .rom.0000aa0c, "ax"
	.global Func_0800aa0c
	.type Func_0800aa0c, %function
	.thumb_func
Func_0800aa0c:
	.incbin "baserom.gba", 0x0000aa0c, 0x00000668
	.section .rom.0000b6b8, "ax"
	.global ResourceSlot_Load
	.type ResourceSlot_Load, %function
	.thumb_func
ResourceSlot_Load:
	.incbin "baserom.gba", 0x0000b6b8, 0x000000e0
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.0000cacc, "ax"
	.global Object_UpdateAllThumb
	.type Object_UpdateAllThumb, %function
	.thumb_func
Object_UpdateAllThumb:
	.incbin "baserom.gba", 0x0000cacc, 0x00000664
	.section .rom.0000daf0, "ax"
	.incbin "baserom.gba", 0x0000daf0, 0x000001ec
	.section .rom.0000dd70, "ax"
	.incbin "baserom.gba", 0x0000dd70, 0x000004b0
	.section .rom.0000ebec, "ax"
	.incbin "baserom.gba", 0x0000ebec, 0x00000b50
	.section .rom.0000fa98, "ax"
	.global Map_LoadLayeredScene
	.type Map_LoadLayeredScene, %function
	.thumb_func
Map_LoadLayeredScene:
	.incbin "baserom.gba", 0x0000fa98, 0x00000364
	.section .rom.00010384, "ax"
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x00010384, 0x0000013c
	.section .rom.000104c0, "ax"
	.global Map_PlayMetatileCopySequence
	.type Map_PlayMetatileCopySequence, %function
	.thumb_func
Map_PlayMetatileCopySequence:
	.incbin "baserom.gba", 0x000104c0, 0x00000074
	.section .rom.00010534, "ax"
	.global Map_CopyMetatileCellsRect
	.type Map_CopyMetatileCellsRect, %function
	.thumb_func
Map_CopyMetatileCellsRect:
	.incbin "baserom.gba", 0x00010534, 0x00000130
	.section .rom.000106e8, "ax"
	.global Func_08010788
	.type Func_08010788, %function
	.thumb_func
Func_08010788:
	.incbin "baserom.gba", 0x000106e8, 0x0000013c
	.section .rom.00010844, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x00010844, 0x00000104
	.section .rom.00010f50, "ax"
	.global MapAnimation_ApplyAffineFrame
	.type MapAnimation_ApplyAffineFrame, %function
	.thumb_func
MapAnimation_ApplyAffineFrame:
	.incbin "baserom.gba", 0x00010f50, 0x000000f0
	.section .rom.00011344, "ax"
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x00011344, 0x000000bc
	.section .rom.00011400, "ax"
	.global Map_UpdateCurrentTileBlockUntilBlocked
	.type Map_UpdateCurrentTileBlockUntilBlocked, %function
	.thumb_func
Map_UpdateCurrentTileBlockUntilBlocked:
	.incbin "baserom.gba", 0x00011400, 0x000000c8
	.section .rom.00011b54, "ax"
	.global Func_08011bf4
	.type Func_08011bf4, %function
	.thumb_func
Func_08011bf4:
	.incbin "baserom.gba", 0x00011b54, 0x000000ec
	.section .rom.00011eb2, "ax"
	.incbin "baserom.gba", 0x00011eb2, 0x00000002
	.section .rom.00011eb4, "ax"
	.global Func_08011f54
	.type Func_08011f54, %function
	.thumb_func
Func_08011f54:
	.incbin "baserom.gba", 0x00011eb4, 0x00000084
	.section .rom.0001203c, "ax"
	.global Func_080120dc
	.type Func_080120dc, %function
	.thumb_func
Func_080120dc:
	.incbin "baserom.gba", 0x0001203c, 0x000000c0
	.section .rom.00012478, "ax"
	.global Ui_RunIconMonitor
	.type Ui_RunIconMonitor, %function
	.thumb_func
Ui_RunIconMonitor:
	.incbin "baserom.gba", 0x00012478, 0x000005e0
	.section .rom.00012a8c, "ax"
	.incbin "baserom.gba", 0x00012a8c, 0x000001f4
	.section .rom.00012e80, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012e80, 0x0000022c
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x000130ac, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x000130f0, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x000131a0, 0x00000014
	.global Data_08013254
Data_08013254:
	.incbin "baserom.gba", 0x000131b4, 0x00000078
	.global Data_080132cc
Data_080132cc:
	.incbin "baserom.gba", 0x0001322c, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x0001325c, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x0001335c, 0x00000140
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x0001349c, 0x00000048
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x000134e4, 0x00000008
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.incbin "baserom.gba", 0x000134ec, 0x00000004
	.global ObjectDispatch_Table0Script
ObjectDispatch_Table0Script:
	.incbin "baserom.gba", 0x000134f0, 0x00000018
	.global ObjectDispatch_Table1Script
ObjectDispatch_Table1Script:
	.incbin "baserom.gba", 0x00013508, 0x00000018
	.global ObjectDispatch_Table2Script
ObjectDispatch_Table2Script:
	.incbin "baserom.gba", 0x00013520, 0x00000018
	.global ObjectDispatch_Table3Script
ObjectDispatch_Table3Script:
	.incbin "baserom.gba", 0x00013538, 0x00000018
	.global ObjectDispatch_Table4Script
ObjectDispatch_Table4Script:
	.incbin "baserom.gba", 0x00013550, 0x00000018
	.global ObjectDispatch_Table5Script
ObjectDispatch_Table5Script:
	.incbin "baserom.gba", 0x00013568, 0x00000018
	.global ObjectDispatch_Table6Script
ObjectDispatch_Table6Script:
	.incbin "baserom.gba", 0x00013580, 0x000000c0
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00013640, 0x000019c0
	.section .rom.000155d0, "ax"
	.incbin "baserom.gba", 0x000155d0, 0x0000030c
	.global Tile_BuildMetatiles
Tile_BuildMetatiles:
	.incbin "baserom.gba", 0x000158dc, 0x000001f4
	.global Tile_BuildMetatilesEnd
Tile_BuildMetatilesEnd:
	.section .rom.00015f04, "ax"
	.global UiWork_InitializeWithResourceCounters
	.type UiWork_InitializeWithResourceCounters, %function
	.thumb_func
UiWork_InitializeWithResourceCounters:
	.incbin "baserom.gba", 0x00015f04, 0x000000d8
	.section .rom.00015fdc, "ax"
	.global UiWork_Initialize
	.type UiWork_Initialize, %function
	.thumb_func
UiWork_Initialize:
	.incbin "baserom.gba", 0x00015fdc, 0x000000d0
	.section .rom.00016482, "ax"
	.incbin "baserom.gba", 0x00016482, 0x00000002
	.section .rom.00016484, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x00016484, 0x00000098
	.section .rom.0001678a, "ax"
	.incbin "baserom.gba", 0x0001678a, 0x0000008a
	.section .rom.000168a0, "ax"
	.global UiWork_StepChannelScript
	.type UiWork_StepChannelScript, %function
	.thumb_func
UiWork_StepChannelScript:
	.incbin "baserom.gba", 0x000168a0, 0x000005dc
	.section .rom.000177fa, "ax"
	.incbin "baserom.gba", 0x000177fa, 0x00000202
	.section .rom.000179fc, "ax"
	.global UiText_RenderWideStringAtOffset
	.type UiText_RenderWideStringAtOffset, %function
	.thumb_func
UiText_RenderWideStringAtOffset:
	.incbin "baserom.gba", 0x000179fc, 0x00000144
	.section .rom.00017bae, "ax"
	.incbin "baserom.gba", 0x00017bae, 0x00000002
	.section .rom.00017bb0, "ax"
	.global UiText_RenderWideStringInWindow
	.type UiText_RenderWideStringInWindow, %function
	.thumb_func
UiText_RenderWideStringInWindow:
	.incbin "baserom.gba", 0x00017bb0, 0x00000144
	.section .rom.00017da8, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00017da8, 0x000007e4
	.section .rom.00018750, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x00018750, 0x0000023c
	.section .rom.0001898c, "ax"
	.global UiText_MeasureStringVariant
	.type UiText_MeasureStringVariant, %function
	.thumb_func
UiText_MeasureStringVariant:
	.incbin "baserom.gba", 0x0001898c, 0x00000670
	.section .rom.000191c8, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x000191c8, 0x00000480
	.section .rom.00019bf8, "ax"
	.incbin "baserom.gba", 0x00019bf8, 0x00000110
	.section .rom.00019e44, "ax"
	.global UiWork_FinalizeEntityMatchingLocalizedId
	.type UiWork_FinalizeEntityMatchingLocalizedId, %function
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedId:
	.incbin "baserom.gba", 0x00019e44, 0x00000070
	.section .rom.0001a080, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x0001a080, 0x0000021c
	.section .rom.0001a984, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x0001a984, 0x00000560
	.section .rom.0001b240, "ax"
	.global Menu_SetupSelectionSide
	.type Menu_SetupSelectionSide, %function
	.thumb_func
Menu_SetupSelectionSide:
	.incbin "baserom.gba", 0x0001b240, 0x00000124
	.section .rom.0001ba60, "ax"
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0001ba60, 0x000001cc
	.section .rom.0001be78, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0001be78, 0x00000244
	.section .rom.0001c494, "ax"
	.global Debug_SelectAbilityPair
	.type Debug_SelectAbilityPair, %function
	.thumb_func
Debug_SelectAbilityPair:
	.global Menu_Check
	.type Menu_Check, %function
	.thumb_func
Menu_Check:
	.incbin "baserom.gba", 0x0001c494, 0x00000360
	.section .rom.0001d9bc, "ax"
	.incbin "baserom.gba", 0x0001d9bc, 0x0000019c
	.section .rom.0001db58, "ax"
	.global Menu_RunWorkspaceSelectionLoop
	.type Menu_RunWorkspaceSelectionLoop, %function
	.thumb_func
Menu_RunWorkspaceSelectionLoop:
	.incbin "baserom.gba", 0x0001db58, 0x000002dc
	.section .rom.0001de34, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001de34, 0x00000424
	.section .rom.0001ef60, "ax"
	.incbin "baserom.gba", 0x0001ef60, 0x00000298
	.section .rom.0001f1f8, "ax"
	.global UiWindow_DrawPartyStatusContents
	.type UiWindow_DrawPartyStatusContents, %function
	.thumb_func
UiWindow_DrawPartyStatusContents:
	.incbin "baserom.gba", 0x0001f1f8, 0x000003d4
	.section .rom.00020144, "ax"
	.global SaveMenu_SelectSlot
	.type SaveMenu_SelectSlot, %function
	.thumb_func
SaveMenu_SelectSlot:
	.incbin "baserom.gba", 0x00020144, 0x000005f8
	.section .rom.00020944, "ax"
	.incbin "baserom.gba", 0x00020944, 0x00000068
	.section .rom.00020af0, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x00020af0, 0x000005e0
	.section .rom.000213f4, "ax"
	.global Party_ShowJoinedMessage
	.type Party_ShowJoinedMessage, %function
	.thumb_func
Party_ShowJoinedMessage:
	.incbin "baserom.gba", 0x000213f4, 0x000000f8
	.section .rom.00021ece, "ax"
	.incbin "baserom.gba", 0x00021ece, 0x00000902
	.section .rom.00022884, "ax"
	.global BattleLayout_HighlightPartyPanels
	.type BattleLayout_HighlightPartyPanels, %function
	.thumb_func
BattleLayout_HighlightPartyPanels:
	.incbin "baserom.gba", 0x00022884, 0x000000a0
	.section .rom.00022ae2, "ax"
	.incbin "baserom.gba", 0x00022ae2, 0x000026fa
	.section .rom.0002525c, "ax"
	.incbin "baserom.gba", 0x0002525c, 0x00001c80
	.section .rom.00027134, "ax"
	.incbin "baserom.gba", 0x00027134, 0x0000003c
	.section .rom.00027170, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x00027170, 0x0000108c
	.section .rom.000281fc, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x000281fc, 0x00000348
	.section .rom.000295ac, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x000295ac, 0x00000228
	.section .rom.000297d4, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x000297d4, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029968, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x00029a68, 0x000003f0
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00029e58, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00029f3c, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x0002a338, 0x00003b9c
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002ded4, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002e154, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x000308ec, 0x00000804
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000310f0, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x00031830, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x000318b0, 0x00000bc0
	.section .rom.000345e0, "ax"
	.incbin "baserom.gba", 0x000345e0, 0x000000a0
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00034680, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x00034a80, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x00034e80, 0x00002000
	.global Data_080366f8
Data_080366f8:
	.incbin "baserom.gba", 0x00036e80, 0x00000058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00036ed8, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x00036f51, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x00036f54, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x00036f56, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x00036f58, 0x00000006
	.global Data_080367d6
Data_080367d6:
	.incbin "baserom.gba", 0x00036f5e, 0x0000000e
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00036f6c, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00036f94, 0x00000954
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000378e8, 0x0000001e
	.global Data_080371fe
Data_080371fe:
	.incbin "baserom.gba", 0x00037906, 0x00000008
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x0003790e, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x0003791e, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x0003792e, 0x0000000a
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x00037938, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00037958, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00037988, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x000379c8, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x00037a08, 0x000000ef
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.incbin "baserom.gba", 0x00037af7, 0x00000008
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x00037aff, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x00037b0b, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x00037b17, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x00037b30, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x00037b34, 0x00000038
	.section .rom.0006c1a0, "ax"
	.incbin "baserom.gba", 0x0006c1a0, 0x0000000a
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x0006c1aa, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x0006c1ec, 0x000003a8
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0006c594, 0x00000a6c
	.section .rom.0006dd38, "ax"
	.global GameState_InitDefaults
	.type GameState_InitDefaults, %function
	.thumb_func
GameState_InitDefaults:
	.incbin "baserom.gba", 0x0006dd38, 0x00000208
	.section .rom.0006ebf0, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x0006ebf0, 0x00000238
	.section .rom.0006f460, "ax"
	.global BattleUnit_Assign
	.type BattleUnit_Assign, %function
	.thumb_func
BattleUnit_Assign:
	.incbin "baserom.gba", 0x0006f460, 0x0000019c
	.section .rom.0006fb24, "ax"
	.global Curve_LookupScaledValue
	.type Curve_LookupScaledValue, %function
	.thumb_func
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x0006fb24, 0x000000a0
	.section .rom.00070664, "ax"
	.global Func_0807a664
	.type Func_0807a664, %function
	.thumb_func
Func_0807a664:
	.incbin "baserom.gba", 0x00070664, 0x0000013c
	.section .rom.00070828, "ax"
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.incbin "baserom.gba", 0x00070828, 0x00000008
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "baserom.gba", 0x00070830, 0x00000c60
	.global Item_ArtifactSlotTable
Item_ArtifactSlotTable:
	.incbin "baserom.gba", 0x00071490, 0x00000200
	.global Character_StartingEquipOwnerIds
Character_StartingEquipOwnerIds:
	.incbin "baserom.gba", 0x00071690, 0x00000018
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000716a8, 0x000037b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x00074e58, 0x00002070
	.global Data_08080ec8
Data_08080ec8:
	.incbin "baserom.gba", 0x00076ec8, 0x00003624
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "baserom.gba", 0x0007a4ec, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x0007aa8c, 0x00000010
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x0007aa9c, 0x00000080
	.global Class_DefinitionTable
Class_DefinitionTable:
	.incbin "baserom.gba", 0x0007ab1c, 0x0000429c
	.global Data_08088db8
Data_08088db8:
	.incbin "baserom.gba", 0x0007edb8, 0x00000040
	.global Element_PowerResistByLevel
Element_PowerResistByLevel:
	.incbin "baserom.gba", 0x0007edf8, 0x00000040
	.global Enemy_ElementPresetTable
Enemy_ElementPresetTable:
	.incbin "baserom.gba", 0x0007ee38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0007f26c, 0x00001d94
	.section .rom.000816e4, "ax"
	.incbin "baserom.gba", 0x000816e4, 0x000001ec
	.section .rom.000818e4, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x000818e4, 0x00000264
	.section .rom.000823ea, "ax"
	.incbin "baserom.gba", 0x000823ea, 0x00000002
	.section .rom.000823ec, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000823ec, 0x00000260
	.section .rom.00082b2c, "ax"
	.global ObjectTable_Restore
	.type ObjectTable_Restore, %function
	.thumb_func
ObjectTable_Restore:
	.incbin "baserom.gba", 0x00082b2c, 0x00000118
	.section .rom.000834f8, "ax"
	.global Func_0808c4f8
	.type Func_0808c4f8, %function
	.thumb_func
Func_0808c4f8:
	.incbin "baserom.gba", 0x000834f8, 0x0000097c
	.section .rom.000849a4, "ax"
	.incbin "baserom.gba", 0x000849a4, 0x00000414
	.section .rom.00085e0c, "ax"
	.global BattleFx_EmitRandomParticle
	.type BattleFx_EmitRandomParticle, %function
	.thumb_func
BattleFx_EmitRandomParticle:
	.incbin "baserom.gba", 0x00085e0c, 0x000000d8
	.section .rom.0008652c, "ax"
	.global DisplayTransition_UpdateScanlineTable
	.type DisplayTransition_UpdateScanlineTable, %function
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	.incbin "baserom.gba", 0x0008652c, 0x0000090c
	.section .rom.00086efc, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x00086efc, 0x000002c4
	.section .rom.00087a5c, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x00087a5c, 0x00000718
	.section .rom.000882b8, "ax"
	.global Object_EffectSpawnCallback
	.type Object_EffectSpawnCallback, %function
	.thumb_func
Object_EffectSpawnCallback:
	.incbin "baserom.gba", 0x000882b8, 0x000001dc
	.section .rom.00089c60, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x00089c60, 0x0000030c
	.section .rom.0008af88, "ax"
	.global battle_owner_69
	.type battle_owner_69, %function
	.thumb_func
battle_owner_69:
	.incbin "baserom.gba", 0x0008af88, 0x000001b4
	.section .rom.0008b1c6, "ax"
	.incbin "baserom.gba", 0x0008b1c6, 0x00000102
	.section .rom.0008b52c, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x0008b52c, 0x000001ec
	.section .rom.0008b808, "ax"
	.incbin "baserom.gba", 0x0008b808, 0x00000188
	.section .rom.0008bab0, "ax"
	.global Unnamed_08094ac8
	.type Unnamed_08094ac8, %function
	.thumb_func
Unnamed_08094ac8:
	.incbin "baserom.gba", 0x0008bab0, 0x000000f4
	.section .rom.0008bba4, "ax"
	.global Unnamed_08094bbc
	.type Unnamed_08094bbc, %function
	.thumb_func
Unnamed_08094bbc:
	.incbin "baserom.gba", 0x0008bba4, 0x000001e4
	.section .rom.0008e62c, "ax"
	.global Func_08097644
	.type Func_08097644, %function
	.thumb_func
Func_08097644:
	.incbin "baserom.gba", 0x0008e62c, 0x00000224
	.section .rom.0008ec24, "ax"
	.global FunctionHead_08097c3c
	.type FunctionHead_08097c3c, %function
	.thumb_func
FunctionHead_08097c3c:
	.incbin "baserom.gba", 0x0008ec24, 0x00000344
	.section .rom.000909e2, "ax"
	.incbin "baserom.gba", 0x000909e2, 0x00000002
	.section .rom.000909e4, "ax"
	.global RunBattleEffect05
	.type RunBattleEffect05, %function
	.thumb_func
RunBattleEffect05:
	.incbin "baserom.gba", 0x000909e4, 0x00000328
	.section .rom.00090d98, "ax"
	.global Battle_unk3_2
	.type Battle_unk3_2, %function
	.thumb_func
Battle_unk3_2:
	.incbin "baserom.gba", 0x00090d98, 0x000004f0
	.section .rom.00091ba8, "ax"
	.global BattleEffect_RunFallbackObjectTransition
	.type BattleEffect_RunFallbackObjectTransition, %function
	.thumb_func
BattleEffect_RunFallbackObjectTransition:
	.incbin "baserom.gba", 0x00091ba8, 0x000001bc
	.section .rom.00091e56, "ax"
	.incbin "baserom.gba", 0x00091e56, 0x00000002
	.section .rom.00091e58, "ax"
	.global RunBattleEffect13
	.type RunBattleEffect13, %function
	.thumb_func
RunBattleEffect13:
	.incbin "baserom.gba", 0x00091e58, 0x0000024c
	.section .rom.00093404, "ax"
	.global Data_0809c410
Data_0809c410:
	.incbin "baserom.gba", 0x00093404, 0x00000100
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "baserom.gba", 0x00093504, 0x00000100
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x00093604, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x00094164, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x0009479c, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000948a4, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000949e4, 0x000003e8
	.global BattleFx_ConditionResources
BattleFx_ConditionResources:
	.incbin "baserom.gba", 0x00094dcc, 0x000003f0
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000951bc, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x00095254, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x0009546c, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000954b2, 0x000001b8
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x0009566a, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x0009569c, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x000956a4, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x00095740, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x00095860, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x00095884, 0x0000004e
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x000958d2, 0x00000102
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000959d4, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x00095be0, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x00095d64, 0x000002a4
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x00096008, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x00096088, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x00096094, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000960b8, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000960dc, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x00096100, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x00096144, 0x00000008
	.global WorldMap_MarkerBlendCycle
WorldMap_MarkerBlendCycle:
	.incbin "baserom.gba", 0x0009614c, 0x00000020
	.global WorldMap_CursorDirectionAngles
WorldMap_CursorDirectionAngles:
	.incbin "baserom.gba", 0x0009616c, 0x00000020
	.section .rom.000967d4, "ax"
	.incbin "baserom.gba", 0x000967d4, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x000967f4, 0x000003bc
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x00096bb0, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x00096bd0, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x00096be8, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x00096c00, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x00096c0c, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x00096c10, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x00096d1c, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x00096d28, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x00096de4, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x00096de8, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x00096df4, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x00096eb0, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x00096efc, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x00096f10, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x00096f24, 0x000000de
	.global FieldFx_MoteTiles
FieldFx_MoteTiles:
	.incbin "baserom.gba", 0x00097002, 0x0000009a
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x0009709c, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000970ec, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x0009710c, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x00097110, 0x0000000c
	.global WorldMap_PlaceMarkers
WorldMap_PlaceMarkers:
	.incbin "baserom.gba", 0x0009711c, 0x00000ee4
	.section .rom.000984f0, "ax"
	.global Ui_DrawValuePairRows
	.type Ui_DrawValuePairRows, %function
	.thumb_func
Ui_DrawValuePairRows:
	.incbin "baserom.gba", 0x000984f0, 0x000000ac
	.section .rom.00098a6a, "ax"
	.incbin "baserom.gba", 0x00098a6a, 0x00000002
	.section .rom.00098a6c, "ax"
	.global UiMenu_SlideCursor
	.type UiMenu_SlideCursor, %function
	.thumb_func
UiMenu_SlideCursor:
	.incbin "baserom.gba", 0x00098a6c, 0x00000108
	.section .rom.00098cb4, "ax"
	.global InventoryMenu_ShowModalMessage
	.type InventoryMenu_ShowModalMessage, %function
	.thumb_func
InventoryMenu_ShowModalMessage:
	.incbin "baserom.gba", 0x00098cb4, 0x000000f8
	.section .rom.00099454, "ax"
	.global RunAssetSelectionScreen
	.type RunAssetSelectionScreen, %function
	.thumb_func
RunAssetSelectionScreen:
	.incbin "baserom.gba", 0x00099454, 0x00000d74
	.section .rom.0009b008, "ax"
	.incbin "baserom.gba", 0x0009b008, 0x00000330
	.section .rom.0009b6b4, "ax"
	.incbin "baserom.gba", 0x0009b6b4, 0x0000010c
	.section .rom.0009b7c0, "ax"
	.global ItemMenu_DrawItemDetails
	.type ItemMenu_DrawItemDetails, %function
	.thumb_func
ItemMenu_DrawItemDetails:
	.incbin "baserom.gba", 0x0009b7c0, 0x000004ac
	.section .rom.0009bdc0, "ax"
	.incbin "baserom.gba", 0x0009bdc0, 0x000002e8
	.section .rom.0009c260, "ax"
	.incbin "baserom.gba", 0x0009c260, 0x000001a8
	.section .rom.0009cb94, "ax"
	.global Menu_ResolveSelectedAction
	.type Menu_ResolveSelectedAction, %function
	.thumb_func
Menu_ResolveSelectedAction:
	.incbin "baserom.gba", 0x0009cb94, 0x0000032c
	.section .rom.0009d4ec, "ax"
	.global Func_080a6614
	.type Func_080a6614, %function
	.thumb_func
Func_080a6614:
	.incbin "baserom.gba", 0x0009d4ec, 0x000001ec
	.section .rom.0009e3bc, "ax"
	.global ActionMenu_Open
	.type ActionMenu_Open, %function
	.thumb_func
ActionMenu_Open:
	.incbin "baserom.gba", 0x0009e3bc, 0x0000022c
	.section .rom.0009f490, "ax"
	.global StatusMenu_ShowOwnerProgressMessage
	.type StatusMenu_ShowOwnerProgressMessage, %function
	.thumb_func
StatusMenu_ShowOwnerProgressMessage:
	.incbin "baserom.gba", 0x0009f490, 0x0000008c
	.section .rom.0009f51c, "ax"
	.global CharacterMenu_DrawStatusAilments
	.type CharacterMenu_DrawStatusAilments, %function
	.thumb_func
CharacterMenu_DrawStatusAilments:
	.incbin "baserom.gba", 0x0009f51c, 0x00000300
	.section .rom.0009fbc8, "ax"
	.incbin "baserom.gba", 0x0009fbc8, 0x00000070
	.section .rom.0009fc38, "ax"
	.global PsynergyMenu_DrawRangePage
	.type PsynergyMenu_DrawRangePage, %function
	.thumb_func
PsynergyMenu_DrawRangePage:
	.incbin "baserom.gba", 0x0009fc38, 0x00000204
	.section .rom.0009fe3c, "ax"
	.global PsynergyMenu_DrawListPage
	.type PsynergyMenu_DrawListPage, %function
	.thumb_func
PsynergyMenu_DrawListPage:
	.incbin "baserom.gba", 0x0009fe3c, 0x0000017c
	.section .rom.000a1664, "ax"
	.global Func_080aa768
Func_080aa768:
	.incbin "baserom.gba", 0x000a1664, 0x0000051c
	.section .rom.000a1eb4, "ax"
	.global Func_080aafb8
Func_080aafb8:
	.incbin "baserom.gba", 0x000a1eb4, 0x0000023c
	.section .rom.000a24e0, "ax"
	.incbin "baserom.gba", 0x000a24e0, 0x00001308
	.section .rom.000a39a4, "ax"
	.global DjinnMenu_DrawStatPreview
	.type DjinnMenu_DrawStatPreview, %function
	.thumb_func
DjinnMenu_DrawStatPreview:
	.incbin "baserom.gba", 0x000a39a4, 0x000007f0
	.section .rom.000a432c, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000a432c, 0x000000fc
	.section .rom.000a45f4, "ax"
	.incbin "baserom.gba", 0x000a45f4, 0x00001040
	.section .rom.000a596c, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000a596c, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000a5a6c, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000a5aec, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000a5c6c, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000a5cec, 0x00000440
	.global Data_080af20c
Data_080af20c:
	.incbin "baserom.gba", 0x000a612c, 0x00000014
	.global ItemMenu_ArrangeKeysString
ItemMenu_ArrangeKeysString:
	.incbin "baserom.gba", 0x000a6140, 0x00000008
	.global ItemMenu_EquipmentKeyString
ItemMenu_EquipmentKeyString:
	.incbin "baserom.gba", 0x000a6148, 0x00000008
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000a6150, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000a6154, 0x00000014
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000a6168, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000a616c, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000a6170, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000a6174, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000a6178, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000a61a8, 0x00000028
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000a61d0, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000a61d9, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000a61e2, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000a61ed, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000a61f8, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000a620c, 0x00000014
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000a6220, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000a6228, 0x00000dd8
	.section .rom.000a7aac, "ax"
	.global Shop_SelBuy
	.type Shop_SelBuy, %function
	.thumb_func
Shop_SelBuy:
	.incbin "baserom.gba", 0x000a7aac, 0x000004f8
	.section .rom.000a80cc, "ax"
	.global Shop_DrawMoney
	.type Shop_DrawMoney, %function
	.thumb_func
Shop_DrawMoney:
	.incbin "baserom.gba", 0x000a80cc, 0x00000050
	.section .rom.000a81b4, "ax"
	.global Shop_DrawMsg
	.type Shop_DrawMsg, %function
	.thumb_func
Shop_DrawMsg:
	.incbin "baserom.gba", 0x000a81b4, 0x00000028
	.section .rom.000a8278, "ax"
	.incbin "baserom.gba", 0x000a8278, 0x00000210
	.section .rom.000a862c, "ax"
	.global Shop_SelectQuantity
	.type Shop_SelectQuantity, %function
	.thumb_func
Shop_SelectQuantity:
	.incbin "baserom.gba", 0x000a862c, 0x000001dc
	.section .rom.000aa964, "ax"
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "baserom.gba", 0x000aa964, 0x00000080
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "baserom.gba", 0x000aa9e4, 0x00000080
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "baserom.gba", 0x000aaa64, 0x00000080
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "baserom.gba", 0x000aaae4, 0x00000080
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "baserom.gba", 0x000aab64, 0x00000080
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "baserom.gba", 0x000aabe4, 0x00000180
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000aad64, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000aaea4, 0x00000280
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000ab124, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000ab160, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000ab16a, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000ab1d0, 0x00000906
	.global Data_080b4ab2
Data_080b4ab2:
	.incbin "baserom.gba", 0x000abad6, 0x00000004
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.incbin "baserom.gba", 0x000abada, 0x00000526
	.section .rom.000ac204, "ax"
	.incbin "baserom.gba", 0x000ac204, 0x00000008
	.section .rom.000ac218, "ax"
	.incbin "baserom.gba", 0x000ac218, 0x00000040
	.section .rom.000ac534, "ax"
	.incbin "baserom.gba", 0x000ac534, 0x000001ac
	.section .rom.000ac6e0, "ax"
	.global Unnamed_080b56e0
	.type Unnamed_080b56e0, %function
	.thumb_func
Unnamed_080b56e0:
	.incbin "baserom.gba", 0x000ac6e0, 0x00000184
	.section .rom.000acf0c, "ax"
	.incbin "baserom.gba", 0x000acf0c, 0x00000160
	.section .rom.000ad3c8, "ax"
	.global Battle_RunEncounter
	.type Battle_RunEncounter, %function
	.thumb_func
Battle_RunEncounter:
	.incbin "baserom.gba", 0x000ad3c8, 0x00000698
	.section .rom.000ae5dc, "ax"
	.incbin "baserom.gba", 0x000ae5dc, 0x00000130
	.section .rom.000ae738, "ax"
	.incbin "baserom.gba", 0x000ae738, 0x000001ac
	.section .rom.000aeb6a, "ax"
	.incbin "baserom.gba", 0x000aeb6a, 0x00000002
	.section .rom.000aeb6c, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x000aeb6c, 0x00000264
	.section .rom.000afc1c, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000afc1c, 0x0000019c
	.section .rom.000b0554, "ax"
	.incbin "baserom.gba", 0x000b0554, 0x000001d0
	.section .rom.000b0724, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x000b0724, 0x00000190
	.section .rom.000b0b2e, "ax"
	.incbin "baserom.gba", 0x000b0b2e, 0x00000206
	.section .rom.000b0ec0, "ax"
	.incbin "baserom.gba", 0x000b0ec0, 0x000003bc
	.section .rom.000b16ac, "ax"
	.incbin "baserom.gba", 0x000b16ac, 0x0000026c
	.section .rom.000b1976, "ax"
	.incbin "baserom.gba", 0x000b1976, 0x00000266
	.section .rom.000b1c6c, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x000b1c6c, 0x0000007c
	.section .rom.000b27c0, "ax"
	.global Unnamed_080bb7c0
	.type Unnamed_080bb7c0, %function
	.thumb_func
Unnamed_080bb7c0:
	.incbin "baserom.gba", 0x000b27c0, 0x00000118
	.section .rom.000b4422, "ax"
	.incbin "baserom.gba", 0x000b4422, 0x00000002
	.section .rom.000b4424, "ax"
	.global BattleCommand_SelectAutomatic
	.type BattleCommand_SelectAutomatic, %function
	.thumb_func
BattleCommand_SelectAutomatic:
	.incbin "baserom.gba", 0x000b4424, 0x00000380
	.section .rom.000b4850, "ax"
	.incbin "baserom.gba", 0x000b4850, 0x00000048
	.section .rom.000b4898, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x000b4898, 0x00000754
	.section .rom.000b518a, "ax"
	.incbin "baserom.gba", 0x000b518a, 0x0000107e
	.section .rom.000b6ba4, "ax"
	.incbin "baserom.gba", 0x000b6ba4, 0x00000414
	.section .rom.000b72a4, "ax"
	.incbin "baserom.gba", 0x000b72a4, 0x0000045c
	.section .rom.000b78ec, "ax"
	.global BattleBackground_Load
	.type BattleBackground_Load, %function
	.thumb_func
BattleBackground_Load:
	.incbin "baserom.gba", 0x000b78ec, 0x00000138
	.section .rom.000b7eea, "ax"
	.incbin "baserom.gba", 0x000b7eea, 0x000000ae
	.section .rom.000b8470, "ax"
	.incbin "baserom.gba", 0x000b8470, 0x00000260
	.section .rom.000b8796, "ax"
	.incbin "baserom.gba", 0x000b8796, 0x00000002
	.section .rom.000b8798, "ax"
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	.incbin "baserom.gba", 0x000b8798, 0x0000027c
	.section .rom.000b8ffc, "ax"
	.incbin "baserom.gba", 0x000b8ffc, 0x0000036c
	.section .rom.000b9a0a, "ax"
	.incbin "baserom.gba", 0x000b9a0a, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000b9a10, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000b9a1c, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000b9a2a, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000b9a38, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000b9a46, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000b9a54, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000b9a62, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000b9a7c, 0x0000003c
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000b9ab8, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000b9ac0, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000b9ad8, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000b9af0, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000b9b08, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000b9b20, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000b9b38, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000b9b50, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000b9b68, 0x00000a54
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000ba5bc, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000ba604, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000ba620, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000ba628, 0x0000010c
	.global BattlePres_AdvanceArrowTiles
BattlePres_AdvanceArrowTiles:
	.incbin "baserom.gba", 0x000ba734, 0x00000800
	.global Data_080c3f34
Data_080c3f34:
	.incbin "baserom.gba", 0x000baf34, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000bc938, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000bc93c, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000bc9a4, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000bc9c4, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000bc9e4, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000bca04, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000bca30, 0x000001e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000bcc10, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000bcc38, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000be3f8, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000be420, 0x00001be0
	.section .rom.000c01dc, "ax"
	.incbin "baserom.gba", 0x000c01dc, 0x00000a84
	.section .rom.000c0ca8, "ax"
	.global BattleFx_RunFiveMode
	.type BattleFx_RunFiveMode, %function
	.thumb_func
BattleFx_RunFiveMode:
	.incbin "baserom.gba", 0x000c0ca8, 0x0000053c
	.section .rom.000c11fc, "ax"
	.global BattleFx_RunParticlePool
	.type BattleFx_RunParticlePool, %function
	.thumb_func
BattleFx_RunParticlePool:
	.incbin "baserom.gba", 0x000c11fc, 0x00000380
	.section .rom.000c160c, "ax"
	.global BattleFx_RunTwelveMode
	.type BattleFx_RunTwelveMode, %function
	.thumb_func
BattleFx_RunTwelveMode:
	.incbin "baserom.gba", 0x000c160c, 0x00000b98
	.section .rom.000c24ec, "ax"
	.incbin "baserom.gba", 0x000c24ec, 0x0000030c
	.section .rom.000c27f8, "ax"
	.global Unnamed_080cb7f8
	.type Unnamed_080cb7f8, %function
	.thumb_func
Unnamed_080cb7f8:
	.incbin "baserom.gba", 0x000c27f8, 0x00000414
	.section .rom.000c2c0c, "ax"
	.global BattleEffect_RunTileAndPaletteAnimation
	.type BattleEffect_RunTileAndPaletteAnimation, %function
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	.incbin "baserom.gba", 0x000c2c0c, 0x000009cc
	.section .rom.000c35d8, "ax"
	.global Func_080cc5d8
	.type Func_080cc5d8, %function
	.thumb_func
Func_080cc5d8:
	.incbin "baserom.gba", 0x000c35d8, 0x00000388
	.section .rom.000c3c38, "ax"
	.global BattleFx_RunTwoResource
	.type BattleFx_RunTwoResource, %function
	.thumb_func
BattleFx_RunTwoResource:
	.incbin "baserom.gba", 0x000c3c38, 0x000004cc
	.section .rom.000c5034, "ax"
	.incbin "baserom.gba", 0x000c5034, 0x00000828
	.section .rom.000c5b54, "ax"
	.global BattleFx_RunMemberBurst
	.type BattleFx_RunMemberBurst, %function
	.thumb_func
BattleFx_RunMemberBurst:
	.incbin "baserom.gba", 0x000c5b54, 0x00000410
	.section .rom.000c5ff8, "ax"
	.global BattleFx_RunFortyEightFrameEffect
	.type BattleFx_RunFortyEightFrameEffect, %function
	.thumb_func
BattleFx_RunFortyEightFrameEffect:
	.incbin "baserom.gba", 0x000c5ff8, 0x000002a8
	.section .rom.000c62b8, "ax"
	.global BattleFx_RunMemberBeam
	.type BattleFx_RunMemberBeam, %function
	.thumb_func
BattleFx_RunMemberBeam:
	.incbin "baserom.gba", 0x000c62b8, 0x000005d4
	.section .rom.000c68e0, "ax"
	.global BattleFx_RunSevenMode
	.type BattleFx_RunSevenMode, %function
	.thumb_func
BattleFx_RunSevenMode:
	.incbin "baserom.gba", 0x000c68e0, 0x00000614
	.section .rom.000c75fc, "ax"
	.incbin "baserom.gba", 0x000c75fc, 0x00001118
	.section .rom.000c8714, "ax"
	.global Unnamed_080d1714
	.type Unnamed_080d1714, %function
	.thumb_func
Unnamed_080d1714:
	.incbin "baserom.gba", 0x000c8714, 0x00000d38
	.section .rom.000c9464, "ax"
	.global BattleEffect_RunPaletteParticles
	.type BattleEffect_RunPaletteParticles, %function
	.thumb_func
BattleEffect_RunPaletteParticles:
	.incbin "baserom.gba", 0x000c9464, 0x00000934
	.section .rom.000c9d98, "ax"
	.global BattleEffect_RunEmberColumns
	.type BattleEffect_RunEmberColumns, %function
	.thumb_func
BattleEffect_RunEmberColumns:
	.incbin "baserom.gba", 0x000c9d98, 0x00001354
	.section .rom.000cb1a4, "ax"
	.incbin "baserom.gba", 0x000cb1a4, 0x00000448
	.section .rom.000cb604, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x000cb604, 0x00000c54
	.section .rom.000cc2c8, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x000cc2c8, 0x000006e8
	.section .rom.000cce54, "ax"
	.incbin "baserom.gba", 0x000cce54, 0x000006b0
	.section .rom.000cd970, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000cd970, 0x00000cec
	.section .rom.000cf2b0, "ax"
	.incbin "baserom.gba", 0x000cf2b0, 0x00000698
	.section .rom.000cf9ac, "ax"
	.global BattleEffectA
	.type BattleEffectA, %function
	.thumb_func
BattleEffectA:
	.incbin "baserom.gba", 0x000cf9ac, 0x000007e8
	.section .rom.000d01dc, "ax"
	.global BattleEffectB
	.type BattleEffectB, %function
	.thumb_func
BattleEffectB:
	.incbin "baserom.gba", 0x000d01dc, 0x000008dc
	.section .rom.000d0ae8, "ax"
	.global RunPaletteRampEffect
	.type RunPaletteRampEffect, %function
	.thumb_func
RunPaletteRampEffect:
	.incbin "baserom.gba", 0x000d0ae8, 0x000004e0
	.section .rom.000d12ac, "ax"
	.incbin "baserom.gba", 0x000d12ac, 0x0000141c
	.section .rom.000d26e0, "ax"
	.global RunParticleFieldEffect
	.type RunParticleFieldEffect, %function
	.thumb_func
RunParticleFieldEffect:
	.incbin "baserom.gba", 0x000d26e0, 0x00000444
	.section .rom.000d3968, "ax"
	.global BattleEffect_RunStagedParticles
	.type BattleEffect_RunStagedParticles, %function
	.thumb_func
BattleEffect_RunStagedParticles:
	.incbin "baserom.gba", 0x000d3968, 0x00000944
	.section .rom.000d4de0, "ax"
	.incbin "baserom.gba", 0x000d4de0, 0x00000518
	.section .rom.000d52f8, "ax"
	.global BattleFx_PrepareCanvasEffect
	.type BattleFx_PrepareCanvasEffect, %function
	.thumb_func
BattleFx_PrepareCanvasEffect:
	.incbin "baserom.gba", 0x000d52f8, 0x0000067c
	.section .rom.000d5a6e, "ax"
	.incbin "baserom.gba", 0x000d5a6e, 0x00000002
	.section .rom.000d5a70, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x000d5a70, 0x00000e48
	.section .rom.000d6e2a, "ax"
	.incbin "baserom.gba", 0x000d6e2a, 0x000006fa
	.section .rom.000d7564, "ax"
	.incbin "baserom.gba", 0x000d7564, 0x0000035c
	.section .rom.000d8040, "ax"
	.incbin "baserom.gba", 0x000d8040, 0x0000051c
	.section .rom.000d85e8, "ax"
	.global BattleFx_InitializeMode12
	.type BattleFx_InitializeMode12, %function
	.thumb_func
BattleFx_InitializeMode12:
	.incbin "baserom.gba", 0x000d85e8, 0x0000130c
	.section .rom.000d9972, "ax"
	.incbin "baserom.gba", 0x000d9972, 0x00000002
	.section .rom.000d9974, "ax"
	.global BattlePres_RunBurstScene
	.type BattlePres_RunBurstScene, %function
	.thumb_func
BattlePres_RunBurstScene:
	.incbin "baserom.gba", 0x000d9974, 0x00000f44
	.section .rom.000daaa0, "ax"
	.global BattlePres_RunBeamSequence
	.type BattlePres_RunBeamSequence, %function
	.thumb_func
BattlePres_RunBeamSequence:
	.incbin "baserom.gba", 0x000daaa0, 0x00000604
	.section .rom.000db0a4, "ax"
	.global Unnamed_080e40a4
	.type Unnamed_080e40a4, %function
	.thumb_func
Unnamed_080e40a4:
	.incbin "baserom.gba", 0x000db0a4, 0x0000064c
	.section .rom.000db7b8, "ax"
	.global BattleFx_RunCastingImpact
	.type BattleFx_RunCastingImpact, %function
	.thumb_func
BattleFx_RunCastingImpact:
	.incbin "baserom.gba", 0x000db7b8, 0x00002190
	.section .rom.000dd98c, "ax"
	.incbin "baserom.gba", 0x000dd98c, 0x000003b0
	.section .rom.000ddeac, "ax"
	.incbin "baserom.gba", 0x000ddeac, 0x000003d0
	.section .rom.000de338, "ax"
	.incbin "baserom.gba", 0x000de338, 0x000000cc
	.section .rom.000de404, "ax"
	.global BattleEffect_RunParticleStreams
	.type BattleEffect_RunParticleStreams, %function
	.thumb_func
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x000de404, 0x00000e38
	.section .rom.000df23c, "ax"
	.global BattleEffect_RunCirclingFallingScene
	.type BattleEffect_RunCirclingFallingScene, %function
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	.incbin "baserom.gba", 0x000df23c, 0x00001e9c
	.section .rom.000e10d8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000e10d8, 0x0000167c
	.section .rom.000e2754, "ax"
	.global Unnamed_080eb754
	.type Unnamed_080eb754, %function
	.thumb_func
Unnamed_080eb754:
	.incbin "baserom.gba", 0x000e2754, 0x0000098c
	.section .rom.000e4408, "ax"
	.global Unnamed_080ed408
	.type Unnamed_080ed408, %function
	.thumb_func
Unnamed_080ed408:
	.global BattleEffect_LoadWork
BattleEffect_LoadWork:
	.incbin "baserom.gba", 0x000e4408, 0x00000678
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000e4a80, 0x00000038
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000e4ab8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000e4ac0, 0x00000028
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000e4ae8, 0x00000008
	.section .rom.000e4e48, "ax"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000e4e48, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000e4e5c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000e4e84, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000e4e96, 0x00000009
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000e4e9f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000e4ea5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000e4eab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e4eb2, 0x0000025a
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000e510c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000e511a, 0x0000019a
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000e52b4, 0x000006c0
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000e5974, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000e5994, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000e5998, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000e599e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000e59a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000e59a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000e59a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000e59b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000e59be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000e59d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000e59d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000e59da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000e59e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000e59e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000e59e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000e59ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000e59ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000e59f2, 0x00000156
	.global Data_080eeb48
Data_080eeb48:
	.incbin "baserom.gba", 0x000e5b48, 0x00000003
	.global Data_080eeb4b
Data_080eeb4b:
	.incbin "baserom.gba", 0x000e5b4b, 0x00000003
	.global Data_080eeb4e
Data_080eeb4e:
	.incbin "baserom.gba", 0x000e5b4e, 0x00000006
	.global Data_080eeb54
Data_080eeb54:
	.incbin "baserom.gba", 0x000e5b54, 0x00000004
	.global Data_080eeb58
Data_080eeb58:
	.incbin "baserom.gba", 0x000e5b58, 0x00000006
	.global Data_080eeb5e
Data_080eeb5e:
	.incbin "baserom.gba", 0x000e5b5e, 0x00000003
	.global Data_080eeb61
Data_080eeb61:
	.incbin "baserom.gba", 0x000e5b61, 0x00000010
	.global Data_080eeb71
Data_080eeb71:
	.incbin "baserom.gba", 0x000e5b71, 0x00000008
	.global Data_080eeb79
Data_080eeb79:
	.incbin "baserom.gba", 0x000e5b79, 0x00000007
	.global Data_080eeb80
Data_080eeb80:
	.incbin "baserom.gba", 0x000e5b80, 0x00000008
	.global Data_080eeb88
Data_080eeb88:
	.incbin "baserom.gba", 0x000e5b88, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000e5b96, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000e5ba6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000e5bae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000e5bb6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000e5bb9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000e5bc0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000e5bc8, 0x00000097
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000e5c5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000e5c63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5c68, 0x000001b6
	.global Data_080eee1e
Data_080eee1e:
	.incbin "baserom.gba", 0x000e5e1e, 0x0000000c
	.global Data_080eee2a
Data_080eee2a:
	.incbin "baserom.gba", 0x000e5e2a, 0x0000000c
	.global Data_080eee36
Data_080eee36:
	.incbin "baserom.gba", 0x000e5e36, 0x00000008
	.global Data_080eee3e
Data_080eee3e:
	.incbin "baserom.gba", 0x000e5e3e, 0x00000008
	.global Data_080eee46
Data_080eee46:
	.incbin "baserom.gba", 0x000e5e46, 0x00000008
	.global Data_080eee4e
Data_080eee4e:
	.incbin "baserom.gba", 0x000e5e4e, 0x0000011a
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000e5f68, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000e5f70, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000e5f78, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000e5f88, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000e5f96, 0x0000000e
	.section .rom.000e6014, "ax"
	.incbin "baserom.gba", 0x000e6014, 0x00000fec
	.section .rom.000e73f0, "ax"
	.global Func_080f03f0
	.type Func_080f03f0, %function
	.thumb_func
Func_080f03f0:
	.incbin "baserom.gba", 0x000e73f0, 0x00000148
	.section .rom.000e77f0, "ax"
	.global Func_080f07f0
Func_080f07f0:
	.incbin "baserom.gba", 0x000e77f0, 0x00000bd8
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000e83c8, 0x00000c38
	.section .rom.000e9028, "ax"
	.incbin "baserom.gba", 0x000e9028, 0x000006c4
	.section .rom.000e96ec, "ax"
	.global Func_080f26ec
	.type Func_080f26ec, %function
	.thumb_func
Func_080f26ec:
	.incbin "baserom.gba", 0x000e96ec, 0x00000480
	.section .rom.000e9b6c, "ax"
	.global Func_080f2b6c
	.type Func_080f2b6c, %function
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000e9b6c, 0x00000004
	.section .rom.000e9d54, "ax"
	.global Unnamed_080f2d54
	.type Unnamed_080f2d54, %function
	.thumb_func
Unnamed_080f2d54:
	.incbin "baserom.gba", 0x000e9d54, 0x00000164
	.section .rom.000ea078, "ax"
	.global Unnamed_080f3078
	.type Unnamed_080f3078, %function
	.thumb_func
Unnamed_080f3078:
	.incbin "baserom.gba", 0x000ea078, 0x00000704
	.section .rom.000ea8bc, "ax"
	.incbin "baserom.gba", 0x000ea8bc, 0x00000744
	.section .rom.000eb168, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000eb168, 0x00001e98
	.section .rom.000ed440, "ax"
	.incbin "baserom.gba", 0x000ed440, 0x00000e04
	.section .rom.000ee388, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000ee388, 0x00000950
	.section .rom.000eee9c, "ax"
	.incbin "baserom.gba", 0x000eee9c, 0x000007be
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000ef65a, 0x000009a6
	.section .rom.000f27a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000f27a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000f2830, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000f28e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000f2914, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000f292c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000f29b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000f29c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000f2a04, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000f2a14, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000f2a48, 0x00000030
	.section .rom.000f3504, "ax"
	.incbin "baserom.gba", 0x000f3504, 0x00000090
	.section .rom.000f3624, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000f3624, 0x00000060
	.section .rom.0017b698, "ax"
	.incbin "baserom.gba", 0x0017b698, 0x00000968
	.section .rom.00315f74, "ax"
	.incbin "baserom.gba", 0x00315f74, 0x0000108c
	.section .rom.00317fa0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00317fa0, 0x00000010
	.section .rom.0031b9e7, "ax"
	.incbin "baserom.gba", 0x0031b9e7, 0x00000001
	.section .rom.00322693, "ax"
	.incbin "baserom.gba", 0x00322693, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x00322694, 0x000086f8
	.section .rom.00332335, "ax"
	.incbin "baserom.gba", 0x00332335, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x00332338, 0x000002b4
	.section .rom.00336a2e, "ax"
	.incbin "baserom.gba", 0x00336a2e, 0x00000002
	.section .rom.0034719e, "ax"
	.incbin "baserom.gba", 0x0034719e, 0x00000002
	.section .rom.0034a78e, "ax"
	.incbin "baserom.gba", 0x0034a78e, 0x00000002
	.section .rom.00356232, "ax"
	.incbin "baserom.gba", 0x00356232, 0x00000002
	.section .rom.003668e2, "ax"
	.incbin "baserom.gba", 0x003668e2, 0x00000002
	.section .rom.0036e382, "ax"
	.incbin "baserom.gba", 0x0036e382, 0x00000002
	.section .rom.00371bc2, "ax"
	.incbin "baserom.gba", 0x00371bc2, 0x00000002
	.section .rom.0037aff6, "ax"
	.incbin "baserom.gba", 0x0037aff6, 0x00000002
	.section .rom.0038641e, "ax"
	.incbin "baserom.gba", 0x0038641e, 0x00000002
	.section .rom.0038e2da, "ax"
	.incbin "baserom.gba", 0x0038e2da, 0x00000002
	.section .rom.00392d6a, "ax"
	.incbin "baserom.gba", 0x00392d6a, 0x00000002
	.section .rom.0039701e, "ax"
	.incbin "baserom.gba", 0x0039701e, 0x00000002
	.section .rom.0039a99e, "ax"
	.incbin "baserom.gba", 0x0039a99e, 0x00000002
	.section .rom.003a71da, "ax"
	.incbin "baserom.gba", 0x003a71da, 0x00000002
	.section .rom.003b5b22, "ax"
	.incbin "baserom.gba", 0x003b5b22, 0x00000002
	.section .rom.003bb425, "ax"
	.incbin "baserom.gba", 0x003bb425, 0x00000003
	.section .rom.003bcdc7, "ax"
	.incbin "baserom.gba", 0x003bcdc7, 0x00000001
	.section .rom.003bfbe5, "ax"
	.incbin "baserom.gba", 0x003bfbe5, 0x00000003
	.section .rom.003c3339, "ax"
	.incbin "baserom.gba", 0x003c3339, 0x00000003
	.section .rom.003c4b4f, "ax"
	.incbin "baserom.gba", 0x003c4b4f, 0x00000001
	.section .rom.003c513a, "ax"
	.incbin "baserom.gba", 0x003c513a, 0x00000002
	.section .rom.003c57fe, "ax"
	.incbin "baserom.gba", 0x003c57fe, 0x00000002
	.section .rom.003c5ae5, "ax"
	.incbin "baserom.gba", 0x003c5ae5, 0x00000003
	.section .rom.003c6e4b, "ax"
	.incbin "baserom.gba", 0x003c6e4b, 0x00000001
	.section .rom.003c7235, "ax"
	.incbin "baserom.gba", 0x003c7235, 0x00000003
	.section .rom.003c7607, "ax"
	.incbin "baserom.gba", 0x003c7607, 0x00000001
	.section .rom.003c7ea7, "ax"
	.incbin "baserom.gba", 0x003c7ea7, 0x00000001
	.section .rom.003c834a, "ax"
	.incbin "baserom.gba", 0x003c834a, 0x00000002
	.section .rom.003c9503, "ax"
	.incbin "baserom.gba", 0x003c9503, 0x00000001
	.section .rom.003ca9c6, "ax"
	.incbin "baserom.gba", 0x003ca9c6, 0x00000002
	.section .rom.003cb98f, "ax"
	.incbin "baserom.gba", 0x003cb98f, 0x00000001
	.section .rom.003cbbea, "ax"
	.incbin "baserom.gba", 0x003cbbea, 0x00000002
	.section .rom.003cd645, "ax"
	.incbin "baserom.gba", 0x003cd645, 0x00000003
	.section .rom.003cdb91, "ax"
	.incbin "baserom.gba", 0x003cdb91, 0x00000003
	.section .rom.003cf91a, "ax"
	.incbin "baserom.gba", 0x003cf91a, 0x00000002
	.section .rom.003cfb93, "ax"
	.incbin "baserom.gba", 0x003cfb93, 0x00000001
	.section .rom.003d005a, "ax"
	.incbin "baserom.gba", 0x003d005a, 0x00000002
	.section .rom.003d1c2f, "ax"
	.incbin "baserom.gba", 0x003d1c2f, 0x00000001
	.section .rom.003d382d, "ax"
	.incbin "baserom.gba", 0x003d382d, 0x00000003
	.section .rom.003d3a4e, "ax"
	.incbin "baserom.gba", 0x003d3a4e, 0x00000002
	.section .rom.003d3e8b, "ax"
	.incbin "baserom.gba", 0x003d3e8b, 0x00000001
	.section .rom.003d3f9d, "ax"
	.incbin "baserom.gba", 0x003d3f9d, 0x00000003
	.section .rom.003d471b, "ax"
	.incbin "baserom.gba", 0x003d471b, 0x00000001
	.section .rom.003d4bb9, "ax"
	.incbin "baserom.gba", 0x003d4bb9, 0x00000003
	.section .rom.003d58ce, "ax"
	.incbin "baserom.gba", 0x003d58ce, 0x00000002
	.section .rom.003d664e, "ax"
	.incbin "baserom.gba", 0x003d664e, 0x00000002
	.section .rom.003d69df, "ax"
	.incbin "baserom.gba", 0x003d69df, 0x00000001
	.section .rom.003d8726, "ax"
	.incbin "baserom.gba", 0x003d8726, 0x00000002
	.section .rom.003d94fd, "ax"
	.incbin "baserom.gba", 0x003d94fd, 0x00000003
	.section .rom.003d9d2a, "ax"
	.incbin "baserom.gba", 0x003d9d2a, 0x00000002
	.section .rom.003da3b5, "ax"
	.incbin "baserom.gba", 0x003da3b5, 0x00000003
	.section .rom.003da573, "ax"
	.incbin "baserom.gba", 0x003da573, 0x00000001
	.section .rom.003db2cb, "ax"
	.incbin "baserom.gba", 0x003db2cb, 0x00000001
	.section .rom.003db817, "ax"
	.incbin "baserom.gba", 0x003db817, 0x00000001
	.section .rom.003dbbf1, "ax"
	.incbin "baserom.gba", 0x003dbbf1, 0x00000003
	.section .rom.003dbf9f, "ax"
	.incbin "baserom.gba", 0x003dbf9f, 0x00000001
	.section .rom.003ddf43, "ax"
	.incbin "baserom.gba", 0x003ddf43, 0x00000001
	.section .rom.003decdf, "ax"
	.incbin "baserom.gba", 0x003decdf, 0x00000001
	.section .rom.003deefb, "ax"
	.incbin "baserom.gba", 0x003deefb, 0x00000001
	.section .rom.003df1f7, "ax"
	.incbin "baserom.gba", 0x003df1f7, 0x00000001
	.section .rom.003e13a5, "ax"
	.incbin "baserom.gba", 0x003e13a5, 0x00000003
	.section .rom.003e2173, "ax"
	.incbin "baserom.gba", 0x003e2173, 0x00000001
	.section .rom.003e32d5, "ax"
	.incbin "baserom.gba", 0x003e32d5, 0x00000003
	.section .rom.003e382f, "ax"
	.incbin "baserom.gba", 0x003e382f, 0x00000001
	.section .rom.003e5119, "ax"
	.incbin "baserom.gba", 0x003e5119, 0x00000003
	.section .rom.003e59ba, "ax"
	.incbin "baserom.gba", 0x003e59ba, 0x00000002
	.section .rom.003e5d97, "ax"
	.incbin "baserom.gba", 0x003e5d97, 0x00000001
	.section .rom.003e6021, "ax"
	.incbin "baserom.gba", 0x003e6021, 0x00000003
	.section .rom.003e63c7, "ax"
	.incbin "baserom.gba", 0x003e63c7, 0x00000001
	.section .rom.003e6622, "ax"
	.incbin "baserom.gba", 0x003e6622, 0x00000002
	.section .rom.003e69da, "ax"
	.incbin "baserom.gba", 0x003e69da, 0x00000002
	.section .rom.003e7ec3, "ax"
	.incbin "baserom.gba", 0x003e7ec3, 0x00000001
	.section .rom.003e9486, "ax"
	.incbin "baserom.gba", 0x003e9486, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003e9488, 0x00000a6c
	.section .rom.003eacca, "ax"
	.incbin "baserom.gba", 0x003eacca, 0x00000002
	.section .rom.003eb04d, "ax"
	.incbin "baserom.gba", 0x003eb04d, 0x00000003
	.section .rom.003ebcfd, "ax"
	.incbin "baserom.gba", 0x003ebcfd, 0x00000003
	.section .rom.003ec845, "ax"
	.incbin "baserom.gba", 0x003ec845, 0x00000003
	.section .rom.003ec9de, "ax"
	.incbin "baserom.gba", 0x003ec9de, 0x00000002
	.section .rom.003ed26b, "ax"
	.incbin "baserom.gba", 0x003ed26b, 0x00000001
	.section .rom.003edd32, "ax"
	.incbin "baserom.gba", 0x003edd32, 0x00000002
	.section .rom.003ee24a, "ax"
	.incbin "baserom.gba", 0x003ee24a, 0x00000002
	.section .rom.003ee86e, "ax"
	.incbin "baserom.gba", 0x003ee86e, 0x00000002
	.section .rom.003ef0d5, "ax"
	.incbin "baserom.gba", 0x003ef0d5, 0x00000003
	.section .rom.003ef4f9, "ax"
	.incbin "baserom.gba", 0x003ef4f9, 0x00000003
	.section .rom.003ef793, "ax"
	.incbin "baserom.gba", 0x003ef793, 0x00000001
	.section .rom.003f112f, "ax"
	.incbin "baserom.gba", 0x003f112f, 0x00000001
	.section .rom.003f2ea6, "ax"
	.incbin "baserom.gba", 0x003f2ea6, 0x00000002
	.section .rom.003f4e1b, "ax"
	.incbin "baserom.gba", 0x003f4e1b, 0x00000001
	.section .rom.003f52eb, "ax"
	.incbin "baserom.gba", 0x003f52eb, 0x00000001
	.section .rom.003f597d, "ax"
	.incbin "baserom.gba", 0x003f597d, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003f5980, 0x00000a34
	.section .rom.003f7785, "ax"
	.incbin "baserom.gba", 0x003f7785, 0x00000003
	.section .rom.003f8256, "ax"
	.incbin "baserom.gba", 0x003f8256, 0x00000002
	.section .rom.003f8d93, "ax"
	.incbin "baserom.gba", 0x003f8d93, 0x00000001
	.section .rom.003f93d3, "ax"
	.incbin "baserom.gba", 0x003f93d3, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x003f93d4, 0x00001588
	.section .rom.003fa9bd, "ax"
	.incbin "baserom.gba", 0x003fa9bd, 0x00000003
	.section .rom.003fad2b, "ax"
	.incbin "baserom.gba", 0x003fad2b, 0x00000001
	.section .rom.003fb2c9, "ax"
	.incbin "baserom.gba", 0x003fb2c9, 0x00000003
	.section .rom.003fb5fd, "ax"
	.incbin "baserom.gba", 0x003fb5fd, 0x00000003
	.section .rom.003fb939, "ax"
	.incbin "baserom.gba", 0x003fb939, 0x00000003
	.section .rom.003fc952, "ax"
	.incbin "baserom.gba", 0x003fc952, 0x00000002
	.section .rom.003fff5e, "ax"
	.incbin "baserom.gba", 0x003fff5e, 0x00000002
	.section .rom.00400701, "ax"
	.incbin "baserom.gba", 0x00400701, 0x00000003
	.section .rom.00401a93, "ax"
	.incbin "baserom.gba", 0x00401a93, 0x00000001
	.section .rom.00401ec3, "ax"
	.incbin "baserom.gba", 0x00401ec3, 0x00000001
	.section .rom.00403cb9, "ax"
	.incbin "baserom.gba", 0x00403cb9, 0x00000003
	.section .rom.004045ce, "ax"
	.incbin "baserom.gba", 0x004045ce, 0x00000002
	.section .rom.00406102, "ax"
	.incbin "baserom.gba", 0x00406102, 0x00000002
	.section .rom.00407151, "ax"
	.incbin "baserom.gba", 0x00407151, 0x00000003
	.section .rom.00407807, "ax"
	.incbin "baserom.gba", 0x00407807, 0x00000001
	.section .rom.004088ad, "ax"
	.incbin "baserom.gba", 0x004088ad, 0x00000003
	.section .rom.0041bcbf, "ax"
	.incbin "baserom.gba", 0x0041bcbf, 0x00000001
	.section .rom.0041be11, "ax"
	.incbin "baserom.gba", 0x0041be11, 0x00000003
	.section .rom.0041c2a2, "ax"
	.incbin "baserom.gba", 0x0041c2a2, 0x00000002
	.section .rom.0041c499, "ax"
	.incbin "baserom.gba", 0x0041c499, 0x00000003
	.section .rom.0041db56, "ax"
	.incbin "baserom.gba", 0x0041db56, 0x00000002
	.section .rom.0041f075, "ax"
	.incbin "baserom.gba", 0x0041f075, 0x00000003
	.section .rom.0041fc41, "ax"
	.incbin "baserom.gba", 0x0041fc41, 0x00000003
	.section .rom.004207b6, "ax"
	.incbin "baserom.gba", 0x004207b6, 0x00000002
	.section .rom.0042265f, "ax"
	.incbin "baserom.gba", 0x0042265f, 0x00000001
	.section .rom.00423ded, "ax"
	.incbin "baserom.gba", 0x00423ded, 0x00000003
	.section .rom.0042529e, "ax"
	.incbin "baserom.gba", 0x0042529e, 0x00000002
	.section .rom.004279eb, "ax"
	.incbin "baserom.gba", 0x004279eb, 0x00000001
	.section .rom.00429285, "ax"
	.incbin "baserom.gba", 0x00429285, 0x00000003
	.section .rom.0042a84e, "ax"
	.incbin "baserom.gba", 0x0042a84e, 0x00000002
	.section .rom.0042c2de, "ax"
	.incbin "baserom.gba", 0x0042c2de, 0x00000002
	.section .rom.0042d1e2, "ax"
	.incbin "baserom.gba", 0x0042d1e2, 0x00000002
	.section .rom.00436e66, "ax"
	.incbin "baserom.gba", 0x00436e66, 0x00000002
	.section .rom.0043901a, "ax"
	.incbin "baserom.gba", 0x0043901a, 0x00000002
	.section .rom.0043d181, "ax"
	.incbin "baserom.gba", 0x0043d181, 0x00000003
	.section .rom.004403a6, "ax"
	.incbin "baserom.gba", 0x004403a6, 0x00000002
	.section .rom.00441aaa, "ax"
	.incbin "baserom.gba", 0x00441aaa, 0x00000002
	.section .rom.00448692, "ax"
	.incbin "baserom.gba", 0x00448692, 0x00000002
	.section .rom.00451016, "ax"
	.incbin "baserom.gba", 0x00451016, 0x00000002
	.section .rom.00458af2, "ax"
	.incbin "baserom.gba", 0x00458af2, 0x00000002
	.section .rom.0045bb86, "ax"
	.incbin "baserom.gba", 0x0045bb86, 0x00000002
	.section .rom.0045ef95, "ax"
	.incbin "baserom.gba", 0x0045ef95, 0x00000003
	.section .rom.00461801, "ax"
	.incbin "baserom.gba", 0x00461801, 0x00000003
	.section .rom.004632f2, "ax"
	.incbin "baserom.gba", 0x004632f2, 0x00000002
	.section .rom.0046410a, "ax"
	.incbin "baserom.gba", 0x0046410a, 0x00000002
	.section .rom.00464df5, "ax"
	.incbin "baserom.gba", 0x00464df5, 0x00000003
	.section .rom.0046e2b2, "ax"
	.incbin "baserom.gba", 0x0046e2b2, 0x00000002
	.section .rom.0046efbd, "ax"
	.incbin "baserom.gba", 0x0046efbd, 0x00000003
	.section .rom.004710d1, "ax"
	.incbin "baserom.gba", 0x004710d1, 0x00000003
	.section .rom.00471d43, "ax"
	.incbin "baserom.gba", 0x00471d43, 0x00000001
	.section .rom.0047295b, "ax"
	.incbin "baserom.gba", 0x0047295b, 0x00000001
	.section .rom.004730cf, "ax"
	.incbin "baserom.gba", 0x004730cf, 0x00000001
	.section .rom.0047495f, "ax"
	.incbin "baserom.gba", 0x0047495f, 0x00000001
	.section .rom.0047532d, "ax"
	.incbin "baserom.gba", 0x0047532d, 0x00000003
	.section .rom.00475f4b, "ax"
	.incbin "baserom.gba", 0x00475f4b, 0x00000001
	.section .rom.004785ff, "ax"
	.incbin "baserom.gba", 0x004785ff, 0x00000001
	.section .rom.0047ad12, "ax"
	.incbin "baserom.gba", 0x0047ad12, 0x00000002
	.section .rom.0047fc67, "ax"
	.incbin "baserom.gba", 0x0047fc67, 0x00000001
	.section .rom.00483f91, "ax"
	.incbin "baserom.gba", 0x00483f91, 0x00000003
	.section .rom.00484b7e, "ax"
	.incbin "baserom.gba", 0x00484b7e, 0x00000002
	.section .rom.00489733, "ax"
	.incbin "baserom.gba", 0x00489733, 0x00000001
	.section .rom.0048ba9d, "ax"
	.incbin "baserom.gba", 0x0048ba9d, 0x00000003
	.section .rom.0048d43f, "ax"
	.incbin "baserom.gba", 0x0048d43f, 0x00000001
	.section .rom.00491d01, "ax"
	.incbin "baserom.gba", 0x00491d01, 0x00000003
	.section .rom.00495dc9, "ax"
	.incbin "baserom.gba", 0x00495dc9, 0x00000003
	.section .rom.00499492, "ax"
	.incbin "baserom.gba", 0x00499492, 0x00000002
	.section .rom.004a1463, "ax"
	.incbin "baserom.gba", 0x004a1463, 0x00000001
	.section .rom.004a8773, "ax"
	.incbin "baserom.gba", 0x004a8773, 0x00000001
	.section .rom.004ad6f3, "ax"
	.incbin "baserom.gba", 0x004ad6f3, 0x00000001
	.section .rom.004b20a3, "ax"
	.incbin "baserom.gba", 0x004b20a3, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004b20a4, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004b20b0, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004b2200, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004b2340, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004b2480, 0x00000140
	.section .rom.004b79cf, "ax"
	.incbin "baserom.gba", 0x004b79cf, 0x00000001
	.section .rom.004b7b5d, "ax"
	.incbin "baserom.gba", 0x004b7b5d, 0x00000003
	.section .rom.004bcbbd, "ax"
	.incbin "baserom.gba", 0x004bcbbd, 0x00000003
	.section .rom.004c1129, "ax"
	.incbin "baserom.gba", 0x004c1129, 0x00000003
	.section .rom.004c62f1, "ax"
	.incbin "baserom.gba", 0x004c62f1, 0x00000003
	.section .rom.004c6487, "ax"
	.incbin "baserom.gba", 0x004c6487, 0x00000001
	.section .rom.004c90db, "ax"
	.incbin "baserom.gba", 0x004c90db, 0x00000001
	.section .rom.004cfd8f, "ax"
	.incbin "baserom.gba", 0x004cfd8f, 0x00000001
	.section .rom.004d2eca, "ax"
	.incbin "baserom.gba", 0x004d2eca, 0x00000002
	.section .rom.004d304b, "ax"
	.incbin "baserom.gba", 0x004d304b, 0x00000001
	.section .rom.004d5805, "ax"
	.incbin "baserom.gba", 0x004d5805, 0x00000003
	.section .rom.004d7e4e, "ax"
	.incbin "baserom.gba", 0x004d7e4e, 0x00000002
	.section .rom.004da532, "ax"
	.incbin "baserom.gba", 0x004da532, 0x00000002
	.section .rom.004db5a7, "ax"
	.incbin "baserom.gba", 0x004db5a7, 0x00000001
	.section .rom.004dd6fa, "ax"
	.incbin "baserom.gba", 0x004dd6fa, 0x00000002
	.section .rom.004dd88d, "ax"
	.incbin "baserom.gba", 0x004dd88d, 0x00000003
	.section .rom.004e00cf, "ax"
	.incbin "baserom.gba", 0x004e00cf, 0x00000001
	.section .rom.004e1ee5, "ax"
	.incbin "baserom.gba", 0x004e1ee5, 0x00000003
	.section .rom.004e45c6, "ax"
	.incbin "baserom.gba", 0x004e45c6, 0x00000002
	.section .rom.004e570d, "ax"
	.incbin "baserom.gba", 0x004e570d, 0x00000003
	.section .rom.004e8821, "ax"
	.incbin "baserom.gba", 0x004e8821, 0x00000003
	.section .rom.004ec989, "ax"
	.incbin "baserom.gba", 0x004ec989, 0x00000003
	.section .rom.004edf86, "ax"
	.incbin "baserom.gba", 0x004edf86, 0x00000002
	.section .rom.004ef24f, "ax"
	.incbin "baserom.gba", 0x004ef24f, 0x00000001
	.section .rom.004f2667, "ax"
	.incbin "baserom.gba", 0x004f2667, 0x00000001
	.section .rom.004f27f5, "ax"
	.incbin "baserom.gba", 0x004f27f5, 0x00000003
	.section .rom.004f45a6, "ax"
	.incbin "baserom.gba", 0x004f45a6, 0x00000002
	.section .rom.004f811d, "ax"
	.incbin "baserom.gba", 0x004f811d, 0x00000003
	.section .rom.004f962b, "ax"
	.incbin "baserom.gba", 0x004f962b, 0x00000001
	.section .rom.004fa1ff, "ax"
	.incbin "baserom.gba", 0x004fa1ff, 0x00000001
	.section .rom.004fa2fd, "ax"
	.incbin "baserom.gba", 0x004fa2fd, 0x00000003
	.section .rom.004fb6e2, "ax"
	.incbin "baserom.gba", 0x004fb6e2, 0x00000002
	.section .rom.004fc865, "ax"
	.incbin "baserom.gba", 0x004fc865, 0x00000003
	.section .rom.004fd1ee, "ax"
	.incbin "baserom.gba", 0x004fd1ee, 0x00000002
	.section .rom.004ffe6b, "ax"
	.incbin "baserom.gba", 0x004ffe6b, 0x00000001
	.section .rom.004fff4e, "ax"
	.incbin "baserom.gba", 0x004fff4e, 0x00000002
	.section .rom.00501139, "ax"
	.incbin "baserom.gba", 0x00501139, 0x00000003
	.section .rom.00502dc1, "ax"
	.incbin "baserom.gba", 0x00502dc1, 0x00000003
	.section .rom.005032cf, "ax"
	.incbin "baserom.gba", 0x005032cf, 0x00000001
	.section .rom.0050340f, "ax"
	.incbin "baserom.gba", 0x0050340f, 0x00000001
	.section .rom.0050462a, "ax"
	.incbin "baserom.gba", 0x0050462a, 0x00000002
	.section .rom.00504785, "ax"
	.incbin "baserom.gba", 0x00504785, 0x00000003
	.section .rom.00508bab, "ax"
	.incbin "baserom.gba", 0x00508bab, 0x00000001
	.section .rom.0050a8da, "ax"
	.incbin "baserom.gba", 0x0050a8da, 0x00000002
	.section .rom.0050b653, "ax"
	.incbin "baserom.gba", 0x0050b653, 0x00000001
	.section .rom.0050b787, "ax"
	.incbin "baserom.gba", 0x0050b787, 0x00000001
	.section .rom.0050dc07, "ax"
	.incbin "baserom.gba", 0x0050dc07, 0x00000001
	.section .rom.0050e983, "ax"
	.incbin "baserom.gba", 0x0050e983, 0x00000001
	.section .rom.0050f462, "ax"
	.incbin "baserom.gba", 0x0050f462, 0x00000002
	.section .rom.005102da, "ax"
	.incbin "baserom.gba", 0x005102da, 0x00000002
	.section .rom.005123ae, "ax"
	.incbin "baserom.gba", 0x005123ae, 0x00000002
	.section .rom.00512cf5, "ax"
	.incbin "baserom.gba", 0x00512cf5, 0x00000003
	.section .rom.00513942, "ax"
	.incbin "baserom.gba", 0x00513942, 0x00000002
	.section .rom.00513b69, "ax"
	.incbin "baserom.gba", 0x00513b69, 0x00000003
	.section .rom.005154ab, "ax"
	.incbin "baserom.gba", 0x005154ab, 0x00000001
	.section .rom.005155bf, "ax"
	.incbin "baserom.gba", 0x005155bf, 0x00000001
	.section .rom.00516f85, "ax"
	.incbin "baserom.gba", 0x00516f85, 0x00000003
	.section .rom.005170cf, "ax"
	.incbin "baserom.gba", 0x005170cf, 0x00000001
	.section .rom.0051919b, "ax"
	.incbin "baserom.gba", 0x0051919b, 0x00000001
	.section .rom.005192c3, "ax"
	.incbin "baserom.gba", 0x005192c3, 0x00000001
	.section .rom.0051ab4b, "ax"
	.incbin "baserom.gba", 0x0051ab4b, 0x00000001
	.section .rom.0051cded, "ax"
	.incbin "baserom.gba", 0x0051cded, 0x00000003
	.section .rom.0051cf2f, "ax"
	.incbin "baserom.gba", 0x0051cf2f, 0x00000001
	.section .rom.0051ef37, "ax"
	.incbin "baserom.gba", 0x0051ef37, 0x00000001
	.section .rom.0051f042, "ax"
	.incbin "baserom.gba", 0x0051f042, 0x00000002
	.section .rom.00521026, "ax"
	.incbin "baserom.gba", 0x00521026, 0x00000002
	.section .rom.0052274b, "ax"
	.incbin "baserom.gba", 0x0052274b, 0x00000001
	.section .rom.005237ff, "ax"
	.incbin "baserom.gba", 0x005237ff, 0x00000001
	.section .rom.005254e6, "ax"
	.incbin "baserom.gba", 0x005254e6, 0x00000002
	.section .rom.00525641, "ax"
	.incbin "baserom.gba", 0x00525641, 0x00000003
	.section .rom.0052758a, "ax"
	.incbin "baserom.gba", 0x0052758a, 0x00000002
	.section .rom.0052770f, "ax"
	.incbin "baserom.gba", 0x0052770f, 0x00000001
	.section .rom.0052a261, "ax"
	.incbin "baserom.gba", 0x0052a261, 0x00000003
	.section .rom.0052c926, "ax"
	.incbin "baserom.gba", 0x0052c926, 0x00000002
	.section .rom.0052f07e, "ax"
	.incbin "baserom.gba", 0x0052f07e, 0x00000002
	.section .rom.005308d1, "ax"
	.incbin "baserom.gba", 0x005308d1, 0x00000003
	.section .rom.00532fd1, "ax"
	.incbin "baserom.gba", 0x00532fd1, 0x00000003
	.section .rom.00534d9d, "ax"
	.incbin "baserom.gba", 0x00534d9d, 0x00000003
	.section .rom.005372e7, "ax"
	.incbin "baserom.gba", 0x005372e7, 0x00000001
	.section .rom.005385b2, "ax"
	.incbin "baserom.gba", 0x005385b2, 0x00000002
	.section .rom.00539263, "ax"
	.incbin "baserom.gba", 0x00539263, 0x00000001
	.section .rom.00539ccd, "ax"
	.incbin "baserom.gba", 0x00539ccd, 0x00000003
	.section .rom.00539dc3, "ax"
	.incbin "baserom.gba", 0x00539dc3, 0x00000001
	.section .rom.0053b822, "ax"
	.incbin "baserom.gba", 0x0053b822, 0x00000002
	.section .rom.0053c5c6, "ax"
	.incbin "baserom.gba", 0x0053c5c6, 0x00000002
	.section .rom.0053e0ff, "ax"
	.incbin "baserom.gba", 0x0053e0ff, 0x00000001
	.section .rom.00540fed, "ax"
	.incbin "baserom.gba", 0x00540fed, 0x00000003
	.section .rom.005487d3, "ax"
	.incbin "baserom.gba", 0x005487d3, 0x00000001
	.section .rom.0054889f, "ax"
	.incbin "baserom.gba", 0x0054889f, 0x00000001
	.section .rom.0054e14a, "ax"
	.incbin "baserom.gba", 0x0054e14a, 0x00000002
	.section .rom.0054e28b, "ax"
	.incbin "baserom.gba", 0x0054e28b, 0x00000001
	.section .rom.0054f6b9, "ax"
	.incbin "baserom.gba", 0x0054f6b9, 0x00000003
	.section .rom.005523f6, "ax"
	.incbin "baserom.gba", 0x005523f6, 0x00000002
	.section .rom.005544f2, "ax"
	.incbin "baserom.gba", 0x005544f2, 0x00000002
	.section .rom.0055befa, "ax"
	.incbin "baserom.gba", 0x0055befa, 0x00000002
	.section .rom.0055c096, "ax"
	.incbin "baserom.gba", 0x0055c096, 0x00000002
	.section .rom.00560931, "ax"
	.incbin "baserom.gba", 0x00560931, 0x00000003
	.section .rom.00562e32, "ax"
	.incbin "baserom.gba", 0x00562e32, 0x00000002
	.section .rom.0056a823, "ax"
	.incbin "baserom.gba", 0x0056a823, 0x00000001
	.section .rom.0056a9be, "ax"
	.incbin "baserom.gba", 0x0056a9be, 0x00000002
	.section .rom.0056d56d, "ax"
	.incbin "baserom.gba", 0x0056d56d, 0x00000003
	.section .rom.0056f641, "ax"
	.incbin "baserom.gba", 0x0056f641, 0x00000003
	.section .rom.0057169a, "ax"
	.incbin "baserom.gba", 0x0057169a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057169c, 0x00001cec
	.section .rom.00574b31, "ax"
	.incbin "baserom.gba", 0x00574b31, 0x00000003
	.section .rom.0057780d, "ax"
	.incbin "baserom.gba", 0x0057780d, 0x00000003
	.section .rom.005795df, "ax"
	.incbin "baserom.gba", 0x005795df, 0x00000001
	.section .rom.0057971f, "ax"
	.incbin "baserom.gba", 0x0057971f, 0x00000001
	.section .rom.0057c779, "ax"
	.incbin "baserom.gba", 0x0057c779, 0x00000003
	.section .rom.0057f6f6, "ax"
	.incbin "baserom.gba", 0x0057f6f6, 0x00000002
	.section .rom.00581fbd, "ax"
	.incbin "baserom.gba", 0x00581fbd, 0x00000003
	.section .rom.00588f25, "ax"
	.incbin "baserom.gba", 0x00588f25, 0x00000003
	.section .rom.0058a7cb, "ax"
	.incbin "baserom.gba", 0x0058a7cb, 0x00000001
	.section .rom.0058b0ca, "ax"
	.incbin "baserom.gba", 0x0058b0ca, 0x00000002
	.section .rom.0058be43, "ax"
	.incbin "baserom.gba", 0x0058be43, 0x00000001
	.section .rom.0058bfa3, "ax"
	.incbin "baserom.gba", 0x0058bfa3, 0x00000001
	.section .rom.0058dbfb, "ax"
	.incbin "baserom.gba", 0x0058dbfb, 0x00000001
	.section .rom.0058f497, "ax"
	.incbin "baserom.gba", 0x0058f497, 0x00000001
	.section .rom.0058f61b, "ax"
	.incbin "baserom.gba", 0x0058f61b, 0x00000001
	.section .rom.00592157, "ax"
	.incbin "baserom.gba", 0x00592157, 0x00000001
	.section .rom.005948d1, "ax"
	.incbin "baserom.gba", 0x005948d1, 0x00000003
	.section .rom.005958be, "ax"
	.incbin "baserom.gba", 0x005958be, 0x00000002
	.section .rom.00596f8b, "ax"
	.incbin "baserom.gba", 0x00596f8b, 0x00000001
	.section .rom.00597135, "ax"
	.incbin "baserom.gba", 0x00597135, 0x00000003
	.section .rom.00599ec9, "ax"
	.incbin "baserom.gba", 0x00599ec9, 0x00000003
	.section .rom.0059df5e, "ax"
	.incbin "baserom.gba", 0x0059df5e, 0x00000002
	.section .rom.0059e09f, "ax"
	.incbin "baserom.gba", 0x0059e09f, 0x00000001
	.section .rom.0059ff67, "ax"
	.incbin "baserom.gba", 0x0059ff67, 0x00000001
	.section .rom.005a0109, "ax"
	.incbin "baserom.gba", 0x005a0109, 0x00000003
	.section .rom.005a21ab, "ax"
	.incbin "baserom.gba", 0x005a21ab, 0x00000001
	.section .rom.005a3138, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005a3138, 0x000022d8
	.section .rom.005a7f01, "ax"
	.incbin "baserom.gba", 0x005a7f01, 0x00000003
	.section .rom.005a8086, "ax"
	.incbin "baserom.gba", 0x005a8086, 0x00000002
	.section .rom.005af7d3, "ax"
	.incbin "baserom.gba", 0x005af7d3, 0x00000001
	.section .rom.005afcdd, "ax"
	.incbin "baserom.gba", 0x005afcdd, 0x00000003
	.section .rom.005b13b5, "ax"
	.incbin "baserom.gba", 0x005b13b5, 0x00000003
	.section .rom.005b1559, "ax"
	.incbin "baserom.gba", 0x005b1559, 0x00000003
	.section .rom.005b2e5e, "ax"
	.incbin "baserom.gba", 0x005b2e5e, 0x00000002
	.section .rom.005b468b, "ax"
	.incbin "baserom.gba", 0x005b468b, 0x00000001
	.section .rom.005b4809, "ax"
	.incbin "baserom.gba", 0x005b4809, 0x00000003
	.section .rom.005b90fd, "ax"
	.incbin "baserom.gba", 0x005b90fd, 0x00000003
	.section .rom.005ba142, "ax"
	.incbin "baserom.gba", 0x005ba142, 0x00000002
	.section .rom.005baf66, "ax"
	.incbin "baserom.gba", 0x005baf66, 0x00000002
	.section .rom.005bc2ab, "ax"
	.incbin "baserom.gba", 0x005bc2ab, 0x00000001
	.section .rom.005bc405, "ax"
	.incbin "baserom.gba", 0x005bc405, 0x00000003
	.section .rom.005be9c3, "ax"
	.incbin "baserom.gba", 0x005be9c3, 0x00000001
	.section .rom.005beb3a, "ax"
	.incbin "baserom.gba", 0x005beb3a, 0x00000002
	.section .rom.005c342d, "ax"
	.incbin "baserom.gba", 0x005c342d, 0x00000003
	.section .rom.005c3e95, "ax"
	.incbin "baserom.gba", 0x005c3e95, 0x00000003
	.section .rom.005c64d7, "ax"
	.incbin "baserom.gba", 0x005c64d7, 0x00000001
	.section .rom.005c663e, "ax"
	.incbin "baserom.gba", 0x005c663e, 0x00000002
	.section .rom.005c7785, "ax"
	.incbin "baserom.gba", 0x005c7785, 0x00000003
	.section .rom.005c94e3, "ax"
	.incbin "baserom.gba", 0x005c94e3, 0x00000001
	.section .rom.005c964a, "ax"
	.incbin "baserom.gba", 0x005c964a, 0x00000002
	.section .rom.005cbed9, "ax"
	.incbin "baserom.gba", 0x005cbed9, 0x00000003
	.section .rom.005cc00e, "ax"
	.incbin "baserom.gba", 0x005cc00e, 0x00000002
	.section .rom.005cec4a, "ax"
	.incbin "baserom.gba", 0x005cec4a, 0x00000002
	.section .rom.005d23e9, "ax"
	.incbin "baserom.gba", 0x005d23e9, 0x00000003
	.section .rom.005d5737, "ax"
	.incbin "baserom.gba", 0x005d5737, 0x00000001
	.section .rom.005d58b3, "ax"
	.incbin "baserom.gba", 0x005d58b3, 0x00000001
	.section .rom.005d8462, "ax"
	.incbin "baserom.gba", 0x005d8462, 0x00000002
	.section .rom.005da6d6, "ax"
	.incbin "baserom.gba", 0x005da6d6, 0x00000002
	.section .rom.005dcf47, "ax"
	.incbin "baserom.gba", 0x005dcf47, 0x00000001
	.section .rom.005e043b, "ax"
	.incbin "baserom.gba", 0x005e043b, 0x00000001
	.section .rom.005e5062, "ax"
	.incbin "baserom.gba", 0x005e5062, 0x00000002
	.section .rom.005e5ce5, "ax"
	.incbin "baserom.gba", 0x005e5ce5, 0x00000003
	.section .rom.005e5e27, "ax"
	.incbin "baserom.gba", 0x005e5e27, 0x00000001
	.section .rom.005e7397, "ax"
	.incbin "baserom.gba", 0x005e7397, 0x00000001
	.section .rom.005e7502, "ax"
	.incbin "baserom.gba", 0x005e7502, 0x00000002
	.section .rom.005e925d, "ax"
	.incbin "baserom.gba", 0x005e925d, 0x00000003
	.section .rom.005e939f, "ax"
	.incbin "baserom.gba", 0x005e939f, 0x00000001
	.section .rom.005ebb9e, "ax"
	.incbin "baserom.gba", 0x005ebb9e, 0x00000002
	.section .rom.005ebcdf, "ax"
	.incbin "baserom.gba", 0x005ebcdf, 0x00000001
	.section .rom.005ed6f7, "ax"
	.incbin "baserom.gba", 0x005ed6f7, 0x00000001
	.section .rom.005eec72, "ax"
	.incbin "baserom.gba", 0x005eec72, 0x00000002
	.section .rom.005f01ae, "ax"
	.incbin "baserom.gba", 0x005f01ae, 0x00000002
	.section .rom.005f033e, "ax"
	.incbin "baserom.gba", 0x005f033e, 0x00000002
	.section .rom.005f25f7, "ax"
	.incbin "baserom.gba", 0x005f25f7, 0x00000001
	.section .rom.005f683e, "ax"
	.incbin "baserom.gba", 0x005f683e, 0x00000002
	.section .rom.005f861d, "ax"
	.incbin "baserom.gba", 0x005f861d, 0x00000003
	.section .rom.005fb7cb, "ax"
	.incbin "baserom.gba", 0x005fb7cb, 0x00000001
	.section .rom.005fddff, "ax"
	.incbin "baserom.gba", 0x005fddff, 0x00000001
	.section .rom.00600957, "ax"
	.incbin "baserom.gba", 0x00600957, 0x00000001
	.section .rom.00600b1f, "ax"
	.incbin "baserom.gba", 0x00600b1f, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00600b20, 0x00002904
	.section .rom.00606f29, "ax"
	.incbin "baserom.gba", 0x00606f29, 0x00000003
	.section .rom.0060849a, "ax"
	.incbin "baserom.gba", 0x0060849a, 0x00000002
	.section .rom.0060acc1, "ax"
	.incbin "baserom.gba", 0x0060acc1, 0x00000003
	.section .rom.0060ae31, "ax"
	.incbin "baserom.gba", 0x0060ae31, 0x00000003
	.section .rom.0060d6f3, "ax"
	.incbin "baserom.gba", 0x0060d6f3, 0x00000001
	.section .rom.0060febb, "ax"
	.incbin "baserom.gba", 0x0060febb, 0x00000001
	.section .rom.006111b5, "ax"
	.incbin "baserom.gba", 0x006111b5, 0x00000003
	.section .rom.00612afb, "ax"
	.incbin "baserom.gba", 0x00612afb, 0x00000001
	.section .rom.0061df8e, "ax"
	.incbin "baserom.gba", 0x0061df8e, 0x00000002
	.section .rom.0061e14e, "ax"
	.incbin "baserom.gba", 0x0061e14e, 0x00000002
	.section .rom.006214b5, "ax"
	.incbin "baserom.gba", 0x006214b5, 0x00000003
	.section .rom.006239a6, "ax"
	.incbin "baserom.gba", 0x006239a6, 0x00000002
	.section .rom.00626787, "ax"
	.incbin "baserom.gba", 0x00626787, 0x00000001
	.section .rom.00627321, "ax"
	.incbin "baserom.gba", 0x00627321, 0x00000003
	.section .rom.00628182, "ax"
	.incbin "baserom.gba", 0x00628182, 0x00000002
	.section .rom.006282da, "ax"
	.incbin "baserom.gba", 0x006282da, 0x00000002
	.section .rom.00630127, "ax"
	.incbin "baserom.gba", 0x00630127, 0x00000001
	.section .rom.00630293, "ax"
	.incbin "baserom.gba", 0x00630293, 0x00000001
	.section .rom.0063302f, "ax"
	.incbin "baserom.gba", 0x0063302f, 0x00000001
	.section .rom.006351be, "ax"
	.incbin "baserom.gba", 0x006351be, 0x00000002
	.section .rom.006358c6, "ax"
	.incbin "baserom.gba", 0x006358c6, 0x00000002
	.section .rom.006364da, "ax"
	.incbin "baserom.gba", 0x006364da, 0x00000002
	.section .rom.006475ad, "ax"
	.incbin "baserom.gba", 0x006475ad, 0x00000003
	.section .rom.00647742, "ax"
	.incbin "baserom.gba", 0x00647742, 0x00000002
	.section .rom.00648c52, "ax"
	.incbin "baserom.gba", 0x00648c52, 0x00000002
	.section .rom.0064aa37, "ax"
	.incbin "baserom.gba", 0x0064aa37, 0x00000001
	.section .rom.0064baca, "ax"
	.incbin "baserom.gba", 0x0064baca, 0x00000002
	.section .rom.0064dd3a, "ax"
	.incbin "baserom.gba", 0x0064dd3a, 0x00000002
	.section .rom.0064ded1, "ax"
	.incbin "baserom.gba", 0x0064ded1, 0x00000003
	.section .rom.0064fe77, "ax"
	.incbin "baserom.gba", 0x0064fe77, 0x00000001
	.section .rom.00654c4e, "ax"
	.incbin "baserom.gba", 0x00654c4e, 0x00000002
	.section .rom.006586af, "ax"
	.incbin "baserom.gba", 0x006586af, 0x00000001
	.section .rom.0065b3e5, "ax"
	.incbin "baserom.gba", 0x0065b3e5, 0x00000003
	.section .rom.0065d72d, "ax"
	.incbin "baserom.gba", 0x0065d72d, 0x00000003
	.section .rom.0065ef0b, "ax"
	.incbin "baserom.gba", 0x0065ef0b, 0x00000001
	.section .rom.0065f0bd, "ax"
	.incbin "baserom.gba", 0x0065f0bd, 0x00000003
	.section .rom.0066fa5e, "ax"
	.incbin "baserom.gba", 0x0066fa5e, 0x00000002
	.section .rom.0066fbbd, "ax"
	.incbin "baserom.gba", 0x0066fbbd, 0x00000003
	.section .rom.00673317, "ax"
	.incbin "baserom.gba", 0x00673317, 0x00000001
	.section .rom.0067347a, "ax"
	.incbin "baserom.gba", 0x0067347a, 0x00000002
	.section .rom.0067915d, "ax"
	.incbin "baserom.gba", 0x0067915d, 0x00000003
	.section .rom.0067bb43, "ax"
	.incbin "baserom.gba", 0x0067bb43, 0x00000001
	.section .rom.0067dd4e, "ax"
	.incbin "baserom.gba", 0x0067dd4e, 0x00000002
	.section .rom.0067de8f, "ax"
	.incbin "baserom.gba", 0x0067de8f, 0x00000001
	.section .rom.0067f8a6, "ax"
	.incbin "baserom.gba", 0x0067f8a6, 0x00000002
	.section .rom.0068660d, "ax"
	.incbin "baserom.gba", 0x0068660d, 0x00000003
	.section .rom.0068678b, "ax"
	.incbin "baserom.gba", 0x0068678b, 0x00000001
	.section .rom.00688735, "ax"
	.incbin "baserom.gba", 0x00688735, 0x00000003
	.section .rom.0068aeae, "ax"
	.incbin "baserom.gba", 0x0068aeae, 0x00000002
	.section .rom.0068c715, "ax"
	.incbin "baserom.gba", 0x0068c715, 0x00000003
	.section .rom.00690136, "ax"
	.incbin "baserom.gba", 0x00690136, 0x00000002
	.section .rom.006902ff, "ax"
	.incbin "baserom.gba", 0x006902ff, 0x00000001
	.section .rom.0069b2e7, "ax"
	.incbin "baserom.gba", 0x0069b2e7, 0x00000001
	.section .rom.0069d433, "ax"
	.incbin "baserom.gba", 0x0069d433, 0x00000001
	.section .rom.0069d5bd, "ax"
	.incbin "baserom.gba", 0x0069d5bd, 0x00000003
	.section .rom.0069f042, "ax"
	.incbin "baserom.gba", 0x0069f042, 0x00000002
	.section .rom.006a2079, "ax"
	.incbin "baserom.gba", 0x006a2079, 0x00000003
	.section .rom.006a223f, "ax"
	.incbin "baserom.gba", 0x006a223f, 0x00000001
	.section .rom.006a3173, "ax"
	.incbin "baserom.gba", 0x006a3173, 0x00000001
	.section .rom.006a5226, "ax"
	.incbin "baserom.gba", 0x006a5226, 0x00000002
	.section .rom.006a53cd, "ax"
	.incbin "baserom.gba", 0x006a53cd, 0x00000003
	.section .rom.006aa3f7, "ax"
	.incbin "baserom.gba", 0x006aa3f7, 0x00000001
	.section .rom.006ad187, "ax"
	.incbin "baserom.gba", 0x006ad187, 0x00000001
	.section .rom.006ad347, "ax"
	.incbin "baserom.gba", 0x006ad347, 0x00000001
	.section .rom.006af83b, "ax"
	.incbin "baserom.gba", 0x006af83b, 0x00000001
	.section .rom.006afa0d, "ax"
	.incbin "baserom.gba", 0x006afa0d, 0x00000003
	.section .rom.006b1a9e, "ax"
	.incbin "baserom.gba", 0x006b1a9e, 0x00000002
	.section .rom.006b3b9e, "ax"
	.incbin "baserom.gba", 0x006b3b9e, 0x00000002
	.section .rom.006b6752, "ax"
	.incbin "baserom.gba", 0x006b6752, 0x00000002
	.section .rom.006b723f, "ax"
	.incbin "baserom.gba", 0x006b723f, 0x00000001
	.section .rom.006b7412, "ax"
	.incbin "baserom.gba", 0x006b7412, 0x00000002
	.section .rom.006b9d41, "ax"
	.incbin "baserom.gba", 0x006b9d41, 0x00000003
	.section .rom.006bb4d9, "ax"
	.incbin "baserom.gba", 0x006bb4d9, 0x00000003
	.section .rom.006bc74d, "ax"
	.incbin "baserom.gba", 0x006bc74d, 0x00000003
	.section .rom.006bebdd, "ax"
	.incbin "baserom.gba", 0x006bebdd, 0x00000003
	.section .rom.006c0f29, "ax"
	.incbin "baserom.gba", 0x006c0f29, 0x00000003
	.section .rom.006c2113, "ax"
	.incbin "baserom.gba", 0x006c2113, 0x00000001
	.section .rom.006c229a, "ax"
	.incbin "baserom.gba", 0x006c229a, 0x00000002
	.section .rom.006c67e6, "ax"
	.incbin "baserom.gba", 0x006c67e6, 0x00000002
	.section .rom.006c693b, "ax"
	.incbin "baserom.gba", 0x006c693b, 0x00000001
	.section .rom.006c6a7b, "ax"
	.incbin "baserom.gba", 0x006c6a7b, 0x00000001
	.section .rom.006c774f, "ax"
	.incbin "baserom.gba", 0x006c774f, 0x00000001
	.section .rom.006c78ce, "ax"
	.incbin "baserom.gba", 0x006c78ce, 0x00000002
	.section .rom.006c9187, "ax"
	.incbin "baserom.gba", 0x006c9187, 0x00000001
	.section .rom.006c9309, "ax"
	.incbin "baserom.gba", 0x006c9309, 0x00000003
	.section .rom.006cad9b, "ax"
	.incbin "baserom.gba", 0x006cad9b, 0x00000001
	.section .rom.006caf15, "ax"
	.incbin "baserom.gba", 0x006caf15, 0x00000003
	.section .rom.006ccc53, "ax"
	.incbin "baserom.gba", 0x006ccc53, 0x00000001
	.section .rom.006ccd7f, "ax"
	.incbin "baserom.gba", 0x006ccd7f, 0x00000001
	.section .rom.006ccef5, "ax"
	.incbin "baserom.gba", 0x006ccef5, 0x00000003
	.section .rom.006ced9b, "ax"
	.incbin "baserom.gba", 0x006ced9b, 0x00000001
	.section .rom.006cef21, "ax"
	.incbin "baserom.gba", 0x006cef21, 0x00000003
	.section .rom.006cfe7f, "ax"
	.incbin "baserom.gba", 0x006cfe7f, 0x00000001
	.section .rom.006d1123, "ax"
	.incbin "baserom.gba", 0x006d1123, 0x00000001
	.section .rom.006d12de, "ax"
	.incbin "baserom.gba", 0x006d12de, 0x00000002
	.section .rom.006d3d82, "ax"
	.incbin "baserom.gba", 0x006d3d82, 0x00000002
	.section .rom.006d3ef5, "ax"
	.incbin "baserom.gba", 0x006d3ef5, 0x00000003
	.section .rom.006d52dd, "ax"
	.incbin "baserom.gba", 0x006d52dd, 0x00000003
	.section .rom.006d8ced, "ax"
	.incbin "baserom.gba", 0x006d8ced, 0x00000003
	.section .rom.006d8e97, "ax"
	.incbin "baserom.gba", 0x006d8e97, 0x00000001
	.section .rom.006db292, "ax"
	.incbin "baserom.gba", 0x006db292, 0x00000002
	.section .rom.006db421, "ax"
	.incbin "baserom.gba", 0x006db421, 0x00000003
	.section .rom.006de6a1, "ax"
	.incbin "baserom.gba", 0x006de6a1, 0x00000003
	.section .rom.006de882, "ax"
	.incbin "baserom.gba", 0x006de882, 0x00000002
	.section .rom.006e0e17, "ax"
	.incbin "baserom.gba", 0x006e0e17, 0x00000001
	.section .rom.006e0fa6, "ax"
	.incbin "baserom.gba", 0x006e0fa6, 0x00000002
	.section .rom.006e3b9e, "ax"
	.incbin "baserom.gba", 0x006e3b9e, 0x00000002
	.section .rom.006e46eb, "ax"
	.incbin "baserom.gba", 0x006e46eb, 0x00000001
	.section .rom.006e4852, "ax"
	.incbin "baserom.gba", 0x006e4852, 0x00000002
	.section .rom.006e7267, "ax"
	.incbin "baserom.gba", 0x006e7267, 0x00000001
	.section .rom.006e8ea6, "ax"
	.incbin "baserom.gba", 0x006e8ea6, 0x00000002
	.section .rom.006e9363, "ax"
	.incbin "baserom.gba", 0x006e9363, 0x00000001
	.section .rom.006ea6db, "ax"
	.incbin "baserom.gba", 0x006ea6db, 0x00000001
	.section .rom.006ebc1f, "ax"
	.incbin "baserom.gba", 0x006ebc1f, 0x00000001
	.section .rom.006ebda9, "ax"
	.incbin "baserom.gba", 0x006ebda9, 0x00000003
	.section .rom.006ece87, "ax"
	.incbin "baserom.gba", 0x006ece87, 0x00000001
	.section .rom.006ed015, "ax"
	.incbin "baserom.gba", 0x006ed015, 0x00000003
	.section .rom.006eec69, "ax"
	.incbin "baserom.gba", 0x006eec69, 0x00000003
	.section .rom.006f164b, "ax"
	.incbin "baserom.gba", 0x006f164b, 0x00000001
	.section .rom.006f4b75, "ax"
	.incbin "baserom.gba", 0x006f4b75, 0x00000003
	.section .rom.006f647f, "ax"
	.incbin "baserom.gba", 0x006f647f, 0x00000001
	.section .rom.006f8527, "ax"
	.incbin "baserom.gba", 0x006f8527, 0x00000001
	.section .rom.006faf7b, "ax"
	.incbin "baserom.gba", 0x006faf7b, 0x00000001
	.section .rom.006fcbc7, "ax"
	.incbin "baserom.gba", 0x006fcbc7, 0x00000001
	.section .rom.006fe00d, "ax"
	.incbin "baserom.gba", 0x006fe00d, 0x00000003
	.section .rom.006ffe8d, "ax"
	.incbin "baserom.gba", 0x006ffe8d, 0x00000003
	.section .rom.00701a5e, "ax"
	.incbin "baserom.gba", 0x00701a5e, 0x00000002
	.section .rom.00704192, "ax"
	.incbin "baserom.gba", 0x00704192, 0x00000002
	.section .rom.00705dd7, "ax"
	.incbin "baserom.gba", 0x00705dd7, 0x00000001
	.section .rom.0070721d, "ax"
	.incbin "baserom.gba", 0x0070721d, 0x00000003
	.section .rom.00708d9b, "ax"
	.incbin "baserom.gba", 0x00708d9b, 0x00000001
	.section .rom.00709fcb, "ax"
	.incbin "baserom.gba", 0x00709fcb, 0x00000001
	.section .rom.0070a125, "ax"
	.incbin "baserom.gba", 0x0070a125, 0x00000003
	.section .rom.0070d623, "ax"
	.incbin "baserom.gba", 0x0070d623, 0x00000001
	.section .rom.0070d78e, "ax"
	.incbin "baserom.gba", 0x0070d78e, 0x00000002
	.section .rom.0071012a, "ax"
	.incbin "baserom.gba", 0x0071012a, 0x00000002
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x0071012c, 0x00001164
	.section .rom.00712a75, "ax"
	.incbin "baserom.gba", 0x00712a75, 0x00000003
	.section .rom.00715cbb, "ax"
	.incbin "baserom.gba", 0x00715cbb, 0x00000001
	.section .rom.007185b2, "ax"
	.incbin "baserom.gba", 0x007185b2, 0x00000002
	.section .rom.007218df, "ax"
	.incbin "baserom.gba", 0x007218df, 0x00000001
	.section .rom.00722c0e, "ax"
	.incbin "baserom.gba", 0x00722c0e, 0x00000002
	.section .rom.00723e07, "ax"
	.incbin "baserom.gba", 0x00723e07, 0x00000001
	.section .rom.00723f69, "ax"
	.incbin "baserom.gba", 0x00723f69, 0x00000003
	.section .rom.00726935, "ax"
	.incbin "baserom.gba", 0x00726935, 0x00000003
	.section .rom.00729e61, "ax"
	.incbin "baserom.gba", 0x00729e61, 0x00000003
	.section .rom.0072e29f, "ax"
	.incbin "baserom.gba", 0x0072e29f, 0x00000001
	.section .rom.0072e412, "ax"
	.incbin "baserom.gba", 0x0072e412, 0x00000002
	.section .rom.00732677, "ax"
	.incbin "baserom.gba", 0x00732677, 0x00000001
	.section .rom.00735e92, "ax"
	.incbin "baserom.gba", 0x00735e92, 0x00000002
	.section .rom.007378ba, "ax"
	.incbin "baserom.gba", 0x007378ba, 0x00000002
	.section .rom.00739f67, "ax"
	.incbin "baserom.gba", 0x00739f67, 0x00000001
	.section .rom.0073bfcb, "ax"
	.incbin "baserom.gba", 0x0073bfcb, 0x00000001
	.section .rom.0073de87, "ax"
	.incbin "baserom.gba", 0x0073de87, 0x00000001
	.section .rom.007410d1, "ax"
	.incbin "baserom.gba", 0x007410d1, 0x00000003
	.section .rom.00742a93, "ax"
	.incbin "baserom.gba", 0x00742a93, 0x00000001
	.section .rom.00742c05, "ax"
	.incbin "baserom.gba", 0x00742c05, 0x00000003
	.section .rom.00745c12, "ax"
	.incbin "baserom.gba", 0x00745c12, 0x00000002
	.section .rom.00745dca, "ax"
	.incbin "baserom.gba", 0x00745dca, 0x00000002
	.section .rom.0074763b, "ax"
	.incbin "baserom.gba", 0x0074763b, 0x00000001
	.section .rom.007477ba, "ax"
	.incbin "baserom.gba", 0x007477ba, 0x00000002
	.section .rom.0074afc3, "ax"
	.incbin "baserom.gba", 0x0074afc3, 0x00000001
	.section .rom.0074b137, "ax"
	.incbin "baserom.gba", 0x0074b137, 0x00000001
	.section .rom.0074beff, "ax"
	.incbin "baserom.gba", 0x0074beff, 0x00000001
	.section .rom.0074c0a5, "ax"
	.incbin "baserom.gba", 0x0074c0a5, 0x00000003
	.section .rom.00750ee3, "ax"
	.incbin "baserom.gba", 0x00750ee3, 0x00000001
	.section .rom.007510ca, "ax"
	.incbin "baserom.gba", 0x007510ca, 0x00000002
	.section .rom.00754366, "ax"
	.incbin "baserom.gba", 0x00754366, 0x00000002
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00754368, 0x00004604
	.section .rom.00758a72, "ax"
	.incbin "baserom.gba", 0x00758a72, 0x00000002
	.section .rom.0075a649, "ax"
	.incbin "baserom.gba", 0x0075a649, 0x00000003
	.section .rom.0075c0f9, "ax"
	.incbin "baserom.gba", 0x0075c0f9, 0x00000003
	.section .rom.0075c255, "ax"
	.incbin "baserom.gba", 0x0075c255, 0x00000003
	.section .rom.0076249b, "ax"
	.incbin "baserom.gba", 0x0076249b, 0x00000001
	.section .rom.0076259f, "ax"
	.incbin "baserom.gba", 0x0076259f, 0x00000001
	.section .rom.00762fbb, "ax"
	.incbin "baserom.gba", 0x00762fbb, 0x00000001
	.section .rom.007630a1, "ax"
	.incbin "baserom.gba", 0x007630a1, 0x00000003
	.section .rom.0076333d, "ax"
	.incbin "baserom.gba", 0x0076333d, 0x00000003
	.section .rom.0076739a, "ax"
	.incbin "baserom.gba", 0x0076739a, 0x00000002
	.section .rom.0076794e, "ax"
	.incbin "baserom.gba", 0x0076794e, 0x00000002
	.section .rom.007688e3, "ax"
	.incbin "baserom.gba", 0x007688e3, 0x00000001
	.section .rom.0076a2e6, "ax"
	.incbin "baserom.gba", 0x0076a2e6, 0x00000002
	.section .rom.0076af55, "ax"
	.incbin "baserom.gba", 0x0076af55, 0x00000003
	.section .rom.0076b1fd, "ax"
	.incbin "baserom.gba", 0x0076b1fd, 0x00000003
	.section .rom.0076c082, "ax"
	.incbin "baserom.gba", 0x0076c082, 0x00000002
	.section .rom.0076d213, "ax"
	.incbin "baserom.gba", 0x0076d213, 0x00000001
	.section .rom.0076d3db, "ax"
	.incbin "baserom.gba", 0x0076d3db, 0x00000001
	.section .rom.0076d727, "ax"
	.incbin "baserom.gba", 0x0076d727, 0x00000001
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x0076d728, 0x0000000c
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x0076d734, 0x00000150
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x0076d884, 0x00000140
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x0076d9c4, 0x00000140
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x0076db04, 0x00000140
	.section .rom.0076df8f, "ax"
	.incbin "baserom.gba", 0x0076df8f, 0x00000001
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x0076df90, 0x0000000c
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x0076df9c, 0x00000150
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x0076e0ec, 0x00000140
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x0076e22c, 0x00000140
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x0076e36c, 0x00000140
	.section .rom.0076e7f7, "ax"
	.incbin "baserom.gba", 0x0076e7f7, 0x00000001
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x0076e7f8, 0x0000000c
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x0076e804, 0x00000150
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x0076e954, 0x00000140
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x0076ea94, 0x00000140
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x0076ebd4, 0x00000140
	.section .rom.0076f05f, "ax"
	.incbin "baserom.gba", 0x0076f05f, 0x00000001
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x0076f060, 0x0000000c
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x0076f06c, 0x00000150
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x0076f1bc, 0x00000140
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x0076f2fc, 0x00000140
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x0076f43c, 0x00000140
	.section .rom.0076f8c7, "ax"
	.incbin "baserom.gba", 0x0076f8c7, 0x00000001
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x0076f8c8, 0x0000000c
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x0076f8d4, 0x00000150
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x0076fa24, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x0076fb64, 0x00000140
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x0076fca4, 0x00000140
	.section .rom.0077012f, "ax"
	.incbin "baserom.gba", 0x0077012f, 0x00000001
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x00770130, 0x0000000c
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x0077013c, 0x00000150
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x0077028c, 0x00000140
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x007703cc, 0x00000140
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x0077050c, 0x00000140
	.section .rom.00770997, "ax"
	.incbin "baserom.gba", 0x00770997, 0x00000001
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00770998, 0x0000000c
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x007709a4, 0x00000150
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x00770af4, 0x00000140
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x00770c34, 0x00000140
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x00770d74, 0x00000140
	.section .rom.007711ff, "ax"
	.incbin "baserom.gba", 0x007711ff, 0x00000001
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x00771200, 0x0000000c
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x0077120c, 0x00000150
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x0077135c, 0x00000140
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x0077149c, 0x00000140
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x007715dc, 0x00000140
	.global Resource_Overlay36F
Resource_Overlay36F:
	.incbin "baserom.gba", 0x0077171c, 0x00000460
	.global Resource_Overlay370
Resource_Overlay370:
	.incbin "baserom.gba", 0x00771b7c, 0x000011a8
	.global Resource_Overlay371
Resource_Overlay371:
	.incbin "baserom.gba", 0x00772d24, 0x00003558
	.global Resource_Overlay372
Resource_Overlay372:
	.incbin "baserom.gba", 0x0077627c, 0x00002b80
	.global Resource_Overlay373
Resource_Overlay373:
	.incbin "baserom.gba", 0x00778dfc, 0x00003ac8
	.global Resource_Overlay374
Resource_Overlay374:
	.incbin "baserom.gba", 0x0077c8c4, 0x00001cdc
	.global Resource_Overlay375
Resource_Overlay375:
	.incbin "baserom.gba", 0x0077e5a0, 0x00000ed0
	.global Resource_Overlay376
Resource_Overlay376:
	.incbin "baserom.gba", 0x0077f470, 0x00000ef8
	.global Resource_Overlay377
Resource_Overlay377:
	.incbin "baserom.gba", 0x00780368, 0x000014c0
	.global Resource_Overlay378
Resource_Overlay378:
	.incbin "baserom.gba", 0x00781828, 0x00001960
	.global Resource_Overlay379
Resource_Overlay379:
	.incbin "baserom.gba", 0x00783188, 0x00000674
	.global Resource_Overlay37A
Resource_Overlay37A:
	.incbin "baserom.gba", 0x007837fc, 0x000014c4
	.global Resource_Overlay37B
Resource_Overlay37B:
	.incbin "baserom.gba", 0x00784cc0, 0x00001514
	.global Resource_Overlay37C
Resource_Overlay37C:
	.incbin "baserom.gba", 0x007861d4, 0x000000c0
	.global Resource_Overlay37D
Resource_Overlay37D:
	.incbin "baserom.gba", 0x00786294, 0x000000d8
	.global Resource_Overlay37E
Resource_Overlay37E:
	.incbin "baserom.gba", 0x0078636c, 0x000000d0
	.global Resource_Overlay37F
Resource_Overlay37F:
	.incbin "baserom.gba", 0x0078643c, 0x000010a0
	.global Resource_Overlay380
Resource_Overlay380:
	.incbin "baserom.gba", 0x007874dc, 0x0000280c
	.global Resource_Overlay381
Resource_Overlay381:
	.incbin "baserom.gba", 0x00789ce8, 0x00001fdc
	.global Resource_Overlay382
Resource_Overlay382:
	.incbin "baserom.gba", 0x0078bcc4, 0x0000135c
	.global Resource_Overlay383
Resource_Overlay383:
	.incbin "baserom.gba", 0x0078d020, 0x00002cf4
	.global Resource_Overlay384
Resource_Overlay384:
	.incbin "baserom.gba", 0x0078fd14, 0x00000254
	.global Resource_Overlay385
Resource_Overlay385:
	.incbin "baserom.gba", 0x0078ff68, 0x00000e14
	.global Resource_Overlay386
Resource_Overlay386:
	.incbin "baserom.gba", 0x00790d7c, 0x00000614
	.global Resource_Overlay387
Resource_Overlay387:
	.incbin "baserom.gba", 0x00791390, 0x00000bd0
	.global Resource_Overlay388
Resource_Overlay388:
	.incbin "baserom.gba", 0x00791f60, 0x00000124
	.global Resource_Overlay389
Resource_Overlay389:
	.incbin "baserom.gba", 0x00792084, 0x00001090
	.global Resource_Overlay38A
Resource_Overlay38A:
	.incbin "baserom.gba", 0x00793114, 0x0000067c
	.global Resource_Overlay38B
Resource_Overlay38B:
	.incbin "baserom.gba", 0x00793790, 0x00000f6c
	.global Resource_Overlay38C
Resource_Overlay38C:
	.incbin "baserom.gba", 0x007946fc, 0x0000066c
	.global Resource_Overlay38D
Resource_Overlay38D:
	.incbin "baserom.gba", 0x00794d68, 0x00001654
	.global Resource_Overlay38E
Resource_Overlay38E:
	.incbin "baserom.gba", 0x007963bc, 0x0000082c
	.global Resource_Overlay38F
Resource_Overlay38F:
	.incbin "baserom.gba", 0x00796be8, 0x00001a4c
	.global Resource_Overlay390
Resource_Overlay390:
	.incbin "baserom.gba", 0x00798634, 0x0000049c
	.global Resource_Overlay391
Resource_Overlay391:
	.incbin "baserom.gba", 0x00798ad0, 0x00001b28
	.global Resource_Overlay392
Resource_Overlay392:
	.incbin "baserom.gba", 0x0079a5f8, 0x00000c00
	.global Resource_Overlay393
Resource_Overlay393:
	.incbin "baserom.gba", 0x0079b1f8, 0x00000be8
	.global Resource_Overlay394
Resource_Overlay394:
	.incbin "baserom.gba", 0x0079bde0, 0x00000b84
	.global Resource_Overlay395
Resource_Overlay395:
	.incbin "baserom.gba", 0x0079c964, 0x00000ea8
	.global Resource_Overlay396
Resource_Overlay396:
	.incbin "baserom.gba", 0x0079d80c, 0x000015c4
	.global Resource_Overlay397
Resource_Overlay397:
	.incbin "baserom.gba", 0x0079edd0, 0x0000030c
	.global Resource_Overlay398
Resource_Overlay398:
	.incbin "baserom.gba", 0x0079f0dc, 0x000007b8
	.global Resource_Overlay399
Resource_Overlay399:
	.incbin "baserom.gba", 0x0079f894, 0x000019f4
	.global Resource_Overlay39A
Resource_Overlay39A:
	.incbin "baserom.gba", 0x007a1288, 0x000017a4
	.global Resource_Overlay39B
Resource_Overlay39B:
	.incbin "baserom.gba", 0x007a2a2c, 0x00001ea8
	.global Resource_Overlay39C
Resource_Overlay39C:
	.incbin "baserom.gba", 0x007a48d4, 0x00004120
	.global Resource_Overlay39D
Resource_Overlay39D:
	.incbin "baserom.gba", 0x007a89f4, 0x00001c78
	.global Resource_Overlay39E
Resource_Overlay39E:
	.incbin "baserom.gba", 0x007aa66c, 0x000024e0
	.global Resource_Overlay39F
Resource_Overlay39F:
	.incbin "baserom.gba", 0x007acb4c, 0x000020e4
	.global Resource_Overlay3A0
Resource_Overlay3A0:
	.incbin "baserom.gba", 0x007aec30, 0x00001124
	.global Resource_Overlay3A1
Resource_Overlay3A1:
	.incbin "baserom.gba", 0x007afd54, 0x0000078c
	.global Resource_Overlay3A2
Resource_Overlay3A2:
	.incbin "baserom.gba", 0x007b04e0, 0x00000d94
	.global Resource_Overlay3A3
Resource_Overlay3A3:
	.incbin "baserom.gba", 0x007b1274, 0x00000fe4
	.global Resource_Overlay3A4
Resource_Overlay3A4:
	.incbin "baserom.gba", 0x007b2258, 0x000029dc
	.global Resource_Overlay3A5
Resource_Overlay3A5:
	.incbin "baserom.gba", 0x007b4c34, 0x00001820
	.global Resource_Overlay3A6
Resource_Overlay3A6:
	.incbin "baserom.gba", 0x007b6454, 0x000016f0
	.global Resource_Overlay3A7
Resource_Overlay3A7:
	.incbin "baserom.gba", 0x007b7b44, 0x000013d4
	.global Resource_Overlay3A8
Resource_Overlay3A8:
	.incbin "baserom.gba", 0x007b8f18, 0x000026c0
	.global Resource_Overlay3A9
Resource_Overlay3A9:
	.incbin "baserom.gba", 0x007bb5d8, 0x00000760
	.global Resource_Overlay3AA
Resource_Overlay3AA:
	.incbin "baserom.gba", 0x007bbd38, 0x00000e60
	.global Resource_Overlay3AB
Resource_Overlay3AB:
	.incbin "baserom.gba", 0x007bcb98, 0x00001368
	.global Resource_Overlay3AC
Resource_Overlay3AC:
	.incbin "baserom.gba", 0x007bdf00, 0x00000584
	.global Resource_Overlay3AD
Resource_Overlay3AD:
	.incbin "baserom.gba", 0x007be484, 0x00000cb0
	.global Resource_Overlay3AE
Resource_Overlay3AE:
	.incbin "baserom.gba", 0x007bf134, 0x00000ffc
	.global Resource_Overlay3AF
Resource_Overlay3AF:
	.incbin "baserom.gba", 0x007c0130, 0x00002aa0
	.global Resource_Overlay3B0
Resource_Overlay3B0:
	.incbin "baserom.gba", 0x007c2bd0, 0x00000c84
	.global Resource_Overlay3B1
Resource_Overlay3B1:
	.incbin "baserom.gba", 0x007c3854, 0x00003aac
	.global Resource_Overlay3B2
Resource_Overlay3B2:
	.incbin "baserom.gba", 0x007c7300, 0x000020d4
	.global Resource_Overlay3B3
Resource_Overlay3B3:
	.incbin "baserom.gba", 0x007c93d4, 0x00002220
	.global Resource_Overlay3B4
Resource_Overlay3B4:
	.incbin "baserom.gba", 0x007cb5f4, 0x00001a0c
	.global Resource_Overlay3B5
Resource_Overlay3B5:
	.incbin "baserom.gba", 0x007cd000, 0x00000d44
	.global Resource_Overlay3B6
Resource_Overlay3B6:
	.incbin "baserom.gba", 0x007cdd44, 0x00000be0
	.global Resource_Overlay3B7
Resource_Overlay3B7:
	.incbin "baserom.gba", 0x007ce924, 0x0000128c
	.global Resource_Overlay3B8
Resource_Overlay3B8:
	.incbin "baserom.gba", 0x007cfbb0, 0x00001f54
	.global Resource_Overlay3B9
Resource_Overlay3B9:
	.incbin "baserom.gba", 0x007d1b04, 0x00001aec
	.global Resource_Overlay3BA
Resource_Overlay3BA:
	.incbin "baserom.gba", 0x007d35f0, 0x00002ac4
	.global Resource_Overlay3BB
Resource_Overlay3BB:
	.incbin "baserom.gba", 0x007d60b4, 0x00002da4
	.global Resource_Overlay3BC
Resource_Overlay3BC:
	.incbin "baserom.gba", 0x007d8e58, 0x000034e0
	.global Resource_Overlay3BD
Resource_Overlay3BD:
	.incbin "baserom.gba", 0x007dc338, 0x0000254c
	.global Resource_Overlay3BE
Resource_Overlay3BE:
	.incbin "baserom.gba", 0x007de884, 0x00001208
	.global Resource_Overlay3BF
Resource_Overlay3BF:
	.incbin "baserom.gba", 0x007dfa8c, 0x000039b8
	.global Resource_Overlay3C0
Resource_Overlay3C0:
	.incbin "baserom.gba", 0x007e3444, 0x00000ed0
	.global Resource_Overlay3C1
Resource_Overlay3C1:
	.incbin "baserom.gba", 0x007e4314, 0x000003a0
	.global Resource_Overlay3C2
Resource_Overlay3C2:
	.incbin "baserom.gba", 0x007e46b4, 0x000007c4
	.global Resource_Overlay3C3
Resource_Overlay3C3:
	.incbin "baserom.gba", 0x007e4e78, 0x00000738
	.global Resource_Overlay3C4
Resource_Overlay3C4:
	.incbin "baserom.gba", 0x007e55b0, 0x00002450
	.global Resource_Overlay3C5
Resource_Overlay3C5:
	.incbin "baserom.gba", 0x007e7a00, 0x00001f98
	.global Resource_Overlay3C6
Resource_Overlay3C6:
	.incbin "baserom.gba", 0x007e9998, 0x00000d30
	.global Resource_Overlay3C7
Resource_Overlay3C7:
	.incbin "baserom.gba", 0x007ea6c8, 0x00000d5c
	.global Resource_Overlay3C8
Resource_Overlay3C8:
	.incbin "baserom.gba", 0x007eb424, 0x00003f48
	.global Resource_Overlay3C9
Resource_Overlay3C9:
	.incbin "baserom.gba", 0x007ef36c, 0x00003688
	.global Resource_Overlay3CA
Resource_Overlay3CA:
	.incbin "baserom.gba", 0x007f29f4, 0x00000fbc
	.global Resource_Overlay3CB
Resource_Overlay3CB:
	.incbin "baserom.gba", 0x007f39b0, 0x00001174
	.global Resource_Overlay3CC
Resource_Overlay3CC:
	.incbin "baserom.gba", 0x007f4b24, 0x00000108
	.global Resource_Overlay3CD
Resource_Overlay3CD:
	.incbin "baserom.gba", 0x007f4c2c, 0x00000480
	.global Resource_Overlay3CE
Resource_Overlay3CE:
	.incbin "baserom.gba", 0x007f50ac, 0x0000af54
