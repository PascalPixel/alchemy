@ tbs-it's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Rom_Start
Rom_Start:
	.incbin "baserom.gba", 0x00000000, 0x000000af
	.global Rom_LanguageCode
Rom_LanguageCode:
	.incbin "baserom.gba", 0x000000af, 0x00000011
	.section .rom.00005024, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00005024, 0x000001f4
	.section .rom.0000570c, "ax"
	.global SaveState_InitializeWorkspace
	.type SaveState_InitializeWorkspace, %function
	.thumb_func
SaveState_InitializeWorkspace:
	.incbin "baserom.gba", 0x0000570c, 0x00000144
	.section .rom.0000619c, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x0000619c, 0x000000e4
	.section .rom.0000659c, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x0000659c, 0x0000023c
	.section .rom.000068ae, "ax"
	.incbin "baserom.gba", 0x000068ae, 0x00000002
	.section .rom.00007360, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x00007360, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x000076b6, 0x00000106
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "baserom.gba", 0x000077bc, 0x00000020
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x000077dc, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x0000799c, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x000079b0, 0x00000024
	.section .rom.000079f0, "ax"
	.incbin "baserom.gba", 0x000079f0, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x000079f8, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a4c, 0x00000014
	.section .rom.00007aa8, "ax"
	.incbin "baserom.gba", 0x00007aa8, 0x00000024
	.section .rom.00007ae4, "ax"
	.incbin "baserom.gba", 0x00007ae4, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007afc, 0x00000058
	.section .rom.00007b78, "ax"
	.incbin "baserom.gba", 0x00007b78, 0x0000008c
	.section .rom.00007c0c, "ax"
	.incbin "baserom.gba", 0x00007c0c, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007c24, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c50, 0x0000002c
	.section .rom.00007ca4, "ax"
	.incbin "baserom.gba", 0x00007ca4, 0x0000135c
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
	.incbin "baserom.gba", 0x0000dd70, 0x00000194
	.section .rom.0000ebec, "ax"
	.incbin "baserom.gba", 0x0000ebec, 0x0000070c
	.section .rom.0000fb38, "ax"
	.global Map_LoadLayeredScene
	.type Map_LoadLayeredScene, %function
	.thumb_func
Map_LoadLayeredScene:
	.incbin "baserom.gba", 0x0000fb38, 0x00000364
	.section .rom.00010424, "ax"
	.global Map_CopyMetatileIndicesRect
	.type Map_CopyMetatileIndicesRect, %function
	.thumb_func
Map_CopyMetatileIndicesRect:
	.incbin "baserom.gba", 0x00010424, 0x0000013c
	.section .rom.00010560, "ax"
	.global Map_PlayMetatileCopySequence
	.type Map_PlayMetatileCopySequence, %function
	.thumb_func
Map_PlayMetatileCopySequence:
	.incbin "baserom.gba", 0x00010560, 0x00000074
	.section .rom.000105d4, "ax"
	.global Map_CopyMetatileCellsRect
	.type Map_CopyMetatileCellsRect, %function
	.thumb_func
Map_CopyMetatileCellsRect:
	.incbin "baserom.gba", 0x000105d4, 0x00000130
	.section .rom.00010788, "ax"
	.global Func_08010788
	.type Func_08010788, %function
	.thumb_func
Func_08010788:
	.incbin "baserom.gba", 0x00010788, 0x0000013c
	.section .rom.000108e4, "ax"
	.global Map_WriteLayerCellTile
	.type Map_WriteLayerCellTile, %function
	.thumb_func
Map_WriteLayerCellTile:
	.incbin "baserom.gba", 0x000108e4, 0x00000104
	.section .rom.00010ff0, "ax"
	.global MapAnimation_ApplyAffineFrame
	.type MapAnimation_ApplyAffineFrame, %function
	.thumb_func
MapAnimation_ApplyAffineFrame:
	.incbin "baserom.gba", 0x00010ff0, 0x000000f0
	.section .rom.000113e4, "ax"
	.global Map_UpdateCurrentTileBlock
	.type Map_UpdateCurrentTileBlock, %function
	.thumb_func
Map_UpdateCurrentTileBlock:
	.incbin "baserom.gba", 0x000113e4, 0x000000bc
	.section .rom.000114a0, "ax"
	.global Map_UpdateCurrentTileBlockUntilBlocked
	.type Map_UpdateCurrentTileBlockUntilBlocked, %function
	.thumb_func
Map_UpdateCurrentTileBlockUntilBlocked:
	.incbin "baserom.gba", 0x000114a0, 0x000000c8
	.section .rom.00011bf4, "ax"
	.global Func_08011bf4
	.type Func_08011bf4, %function
	.thumb_func
Func_08011bf4:
	.incbin "baserom.gba", 0x00011bf4, 0x000000ec
	.section .rom.00011f52, "ax"
	.incbin "baserom.gba", 0x00011f52, 0x00000002
	.section .rom.00011f54, "ax"
	.global Func_08011f54
	.type Func_08011f54, %function
	.thumb_func
Func_08011f54:
	.incbin "baserom.gba", 0x00011f54, 0x00000084
	.section .rom.000120dc, "ax"
	.global Func_080120dc
	.type Func_080120dc, %function
	.thumb_func
Func_080120dc:
	.incbin "baserom.gba", 0x000120dc, 0x000000c0
	.section .rom.00012518, "ax"
	.global Ui_RunIconMonitor
	.type Ui_RunIconMonitor, %function
	.thumb_func
Ui_RunIconMonitor:
	.incbin "baserom.gba", 0x00012518, 0x000005e0
	.section .rom.00012b2c, "ax"
	.incbin "baserom.gba", 0x00012b2c, 0x000001f4
	.section .rom.00012f20, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012f20, 0x0000022c
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001314c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00013190, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00013240, 0x00000014
	.global Data_08013254
Data_08013254:
	.incbin "baserom.gba", 0x00013254, 0x00000020
	.global Data_08013274
Data_08013274:
	.incbin "baserom.gba", 0x00013274, 0x00000018
	.global Data_0801328c
Data_0801328c:
	.incbin "baserom.gba", 0x0001328c, 0x00000040
	.global Data_080132cc
Data_080132cc:
	.incbin "baserom.gba", 0x000132cc, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x000132fc, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x000133fc, 0x00000140
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x0001353c, 0x00000048
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x00013584, 0x00000008
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.incbin "baserom.gba", 0x0001358c, 0x00000004
	.global ObjectDispatch_Table0Script
ObjectDispatch_Table0Script:
	.incbin "baserom.gba", 0x00013590, 0x00000018
	.global ObjectDispatch_Table1Script
ObjectDispatch_Table1Script:
	.incbin "baserom.gba", 0x000135a8, 0x00000018
	.global ObjectDispatch_Table2Script
ObjectDispatch_Table2Script:
	.incbin "baserom.gba", 0x000135c0, 0x00000018
	.global ObjectDispatch_Table3Script
ObjectDispatch_Table3Script:
	.incbin "baserom.gba", 0x000135d8, 0x00000018
	.global ObjectDispatch_Table4Script
ObjectDispatch_Table4Script:
	.incbin "baserom.gba", 0x000135f0, 0x00000018
	.global ObjectDispatch_Table5Script
ObjectDispatch_Table5Script:
	.incbin "baserom.gba", 0x00013608, 0x00000018
	.global ObjectDispatch_Table6Script
ObjectDispatch_Table6Script:
	.incbin "baserom.gba", 0x00013620, 0x000000c0
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x000136e0, 0x00001920
	.section .rom.000155d0, "ax"
	.incbin "baserom.gba", 0x000155d0, 0x000002f4
	.global Tile_BuildMetatiles
Tile_BuildMetatiles:
	.incbin "baserom.gba", 0x000158c4, 0x00000214
	.global Tile_BuildMetatilesEnd
Tile_BuildMetatilesEnd:
	.section .rom.000164ae, "ax"
	.incbin "baserom.gba", 0x000164ae, 0x00000002
	.section .rom.000164b0, "ax"
	.global UiWindow_ClearInteriorTiles
	.type UiWindow_ClearInteriorTiles, %function
	.thumb_func
UiWindow_ClearInteriorTiles:
	.incbin "baserom.gba", 0x000164b0, 0x00000098
	.section .rom.000167ba, "ax"
	.incbin "baserom.gba", 0x000167ba, 0x0000008a
	.section .rom.000168d0, "ax"
	.global UiWork_StepChannelScript
	.type UiWork_StepChannelScript, %function
	.thumb_func
UiWork_StepChannelScript:
	.incbin "baserom.gba", 0x000168d0, 0x0000060c
	.section .rom.00017f42, "ax"
	.incbin "baserom.gba", 0x00017f42, 0x00000002
	.section .rom.00017f44, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00017f44, 0x00000688
	.section .rom.00018764, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x00018764, 0x000001f8
	.section .rom.0001895c, "ax"
	.global UiText_MeasureStringVariant
	.type UiText_MeasureStringVariant, %function
	.thumb_func
UiText_MeasureStringVariant:
	.incbin "baserom.gba", 0x0001895c, 0x00000264
	.global Func_08018cac
Func_08018cac:
	.incbin "baserom.gba", 0x00018bc0, 0x00000250
	.section .rom.000190e0, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x000190e0, 0x00000480
	.section .rom.00019b68, "ax"
	.incbin "baserom.gba", 0x00019b68, 0x00000110
	.section .rom.00019ff4, "ax"
	.global ItemIcon_Compose
	.type ItemIcon_Compose, %function
	.thumb_func
ItemIcon_Compose:
	.incbin "baserom.gba", 0x00019ff4, 0x0000021c
	.section .rom.0001a8f8, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x0001a8f8, 0x00000560
	.section .rom.0001b9d4, "ax"
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0001b9d4, 0x000001cc
	.section .rom.0001bdec, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0001bdec, 0x00000244
	.section .rom.0001c438, "ax"
	.global Debug_SelectAbilityPair
	.type Debug_SelectAbilityPair, %function
	.thumb_func
Debug_SelectAbilityPair:
	.global Menu_Check
	.type Menu_Check, %function
	.thumb_func
Menu_Check:
	.incbin "baserom.gba", 0x0001c438, 0x00000360
	.section .rom.0001d970, "ax"
	.global Menu_CreateWorkspaceWindows
	.type Menu_CreateWorkspaceWindows, %function
	.thumb_func
Menu_CreateWorkspaceWindows:
	.incbin "baserom.gba", 0x0001d970, 0x0000019c
	.section .rom.0001dcc4, "ax"
	.incbin "baserom.gba", 0x0001dcc4, 0x00000134
	.section .rom.0001ddf8, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001ddf8, 0x00000410
	.section .rom.0001ef10, "ax"
	.incbin "baserom.gba", 0x0001ef10, 0x00000298
	.section .rom.0001f1a8, "ax"
	.global UiWindow_DrawPartyStatusContents
	.type UiWindow_DrawPartyStatusContents, %function
	.thumb_func
UiWindow_DrawPartyStatusContents:
	.incbin "baserom.gba", 0x0001f1a8, 0x000003d4
	.section .rom.000201ec, "ax"
	.global SaveMenu_SelectSlot
	.type SaveMenu_SelectSlot, %function
	.thumb_func
SaveMenu_SelectSlot:
	.incbin "baserom.gba", 0x000201ec, 0x00000580
	.section .rom.00020b7e, "ax"
	.incbin "baserom.gba", 0x00020b7e, 0x00000002
	.section .rom.00020b80, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x00020b80, 0x00000494
	.section .rom.00021338, "ax"
	.global Party_ShowJoinedMessage
	.type Party_ShowJoinedMessage, %function
	.thumb_func
Party_ShowJoinedMessage:
	.incbin "baserom.gba", 0x00021338, 0x000000f8
	.section .rom.00021e12, "ax"
	.incbin "baserom.gba", 0x00021e12, 0x000008fe
	.section .rom.00022a22, "ax"
	.incbin "baserom.gba", 0x00022a22, 0x000027b2
	.section .rom.00025254, "ax"
	.incbin "baserom.gba", 0x00025254, 0x00001d58
	.section .rom.00027240, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x00027240, 0x00001080
	.section .rom.000282c0, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x000282c0, 0x00000348
	.section .rom.00029680, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x00029680, 0x00000228
	.section .rom.000298a8, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x000298a8, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029a3c, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x00029b3c, 0x000003f0
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00029f2c, 0x000000e4
	.section .rom.000311cc, "ax"
	.incbin "baserom.gba", 0x000311cc, 0x00000004
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000311d0, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x00031910, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00031990, 0x000005dc
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x00031f6c, 0x00000400
	.section .rom.00033f6c, "ax"
	.incbin "baserom.gba", 0x00033f6c, 0x0000003c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00033fa8, 0x000000e4
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x0003408c, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x0003448c, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x0003488c, 0x00002000
	.global Data_080366f8
Data_080366f8:
	.incbin "baserom.gba", 0x0003688c, 0x00000058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x000368e4, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x0003695d, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x00036960, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x00036962, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x00036964, 0x00000006
	.global Data_080367d6
Data_080367d6:
	.incbin "baserom.gba", 0x0003696a, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00036970, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00036978, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x000369a0, 0x000009d4
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00037374, 0x0000001e
	.global Data_080371fe
Data_080371fe:
	.incbin "baserom.gba", 0x00037392, 0x00000008
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x0003739a, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x000373aa, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x000373ba, 0x0000000a
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x000373c4, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x000373e4, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00037414, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x00037454, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x00037494, 0x000000ff
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.incbin "baserom.gba", 0x00037593, 0x00000008
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x0003759b, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000375a7, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x000375b3, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x000375cc, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x000375d0, 0x00000038
	.section .rom.00072c78, "ax"
	.incbin "baserom.gba", 0x00072c78, 0x00000116
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00072d8e, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x00072dd0, 0x00000114
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x00072ee4, 0x0000411c
	.section .rom.00077d38, "ax"
	.global GameState_InitDefaults
	.type GameState_InitDefaults, %function
	.thumb_func
GameState_InitDefaults:
	.incbin "baserom.gba", 0x00077d38, 0x00000208
	.section .rom.00078bf0, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x00078bf0, 0x00000238
	.section .rom.00079460, "ax"
	.global BattleUnit_Assign
	.type BattleUnit_Assign, %function
	.thumb_func
BattleUnit_Assign:
	.incbin "baserom.gba", 0x00079460, 0x0000019c
	.section .rom.00079b24, "ax"
	.global Curve_LookupScaledValue
	.type Curve_LookupScaledValue, %function
	.thumb_func
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x00079b24, 0x000000a0
	.section .rom.0007a664, "ax"
	.global Func_0807a664
	.type Func_0807a664, %function
	.thumb_func
Func_0807a664:
	.incbin "baserom.gba", 0x0007a664, 0x0000013c
	.section .rom.0007a828, "ax"
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.incbin "baserom.gba", 0x0007a828, 0x00000008
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "baserom.gba", 0x0007a830, 0x00000c60
	.global Item_ArtifactSlotTable
Item_ArtifactSlotTable:
	.incbin "baserom.gba", 0x0007b490, 0x00000200
	.global Character_StartingEquipOwnerIds
Character_StartingEquipOwnerIds:
	.incbin "baserom.gba", 0x0007b690, 0x00000018
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x0007b6a8, 0x000037b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x0007ee58, 0x00002070
	.global Data_08080ec8
Data_08080ec8:
	.incbin "baserom.gba", 0x00080ec8, 0x00003624
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "baserom.gba", 0x000844ec, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x00084a8c, 0x00000010
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x00084a9c, 0x00000080
	.global Class_DefinitionTable
Class_DefinitionTable:
	.incbin "baserom.gba", 0x00084b1c, 0x0000429c
	.global Data_08088db8
Data_08088db8:
	.incbin "baserom.gba", 0x00088db8, 0x00000040
	.global Element_PowerResistByLevel
Element_PowerResistByLevel:
	.incbin "baserom.gba", 0x00088df8, 0x00000040
	.global Enemy_ElementPresetTable
Enemy_ElementPresetTable:
	.incbin "baserom.gba", 0x00088e38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008926c, 0x00000d94
	.section .rom.0008a6ec, "ax"
	.incbin "baserom.gba", 0x0008a6ec, 0x000001ec
	.section .rom.0008a8ec, "ax"
	.global Game_ResetForNewGame
	.type Game_ResetForNewGame, %function
	.thumb_func
Game_ResetForNewGame:
	.incbin "baserom.gba", 0x0008a8ec, 0x00000264
	.section .rom.0008b3f2, "ax"
	.incbin "baserom.gba", 0x0008b3f2, 0x00000002
	.section .rom.0008b3f4, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x0008b3f4, 0x00000260
	.section .rom.0008bb34, "ax"
	.global ObjectTable_Restore
	.type ObjectTable_Restore, %function
	.thumb_func
ObjectTable_Restore:
	.incbin "baserom.gba", 0x0008bb34, 0x00000118
	.section .rom.0008c500, "ax"
	.global Func_0808c4f8
	.type Func_0808c4f8, %function
	.thumb_func
Func_0808c4f8:
	.incbin "baserom.gba", 0x0008c500, 0x0000097c
	.section .rom.0008d9ac, "ax"
	.incbin "baserom.gba", 0x0008d9ac, 0x00000414
	.section .rom.0008f598, "ax"
	.global DisplayTransition_UpdateScanlineTable
	.type DisplayTransition_UpdateScanlineTable, %function
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	.incbin "baserom.gba", 0x0008f598, 0x0000090c
	.section .rom.0008ff68, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x0008ff68, 0x000002c4
	.section .rom.00090ac8, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x00090ac8, 0x00000718
	.section .rom.00091324, "ax"
	.global Object_EffectSpawnCallback
	.type Object_EffectSpawnCallback, %function
	.thumb_func
Object_EffectSpawnCallback:
	.incbin "baserom.gba", 0x00091324, 0x000001dc
	.section .rom.00092ccc, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x00092ccc, 0x00000344
	.section .rom.0009402c, "ax"
	.global battle_owner_69
	.type battle_owner_69, %function
	.thumb_func
battle_owner_69:
	.incbin "baserom.gba", 0x0009402c, 0x000001b4
	.section .rom.000945d0, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x000945d0, 0x000001ec
	.section .rom.000948ac, "ax"
	.incbin "baserom.gba", 0x000948ac, 0x00000188
	.section .rom.00094b54, "ax"
	.global Unnamed_08094ac8
	.type Unnamed_08094ac8, %function
	.thumb_func
Unnamed_08094ac8:
	.incbin "baserom.gba", 0x00094b54, 0x000000f4
	.section .rom.00094c48, "ax"
	.global Unnamed_08094bbc
	.type Unnamed_08094bbc, %function
	.thumb_func
Unnamed_08094bbc:
	.incbin "baserom.gba", 0x00094c48, 0x000001e4
	.section .rom.000976d0, "ax"
	.global Func_08097644
	.type Func_08097644, %function
	.thumb_func
Func_08097644:
	.incbin "baserom.gba", 0x000976d0, 0x00000224
	.section .rom.00097cc8, "ax"
	.global FunctionHead_08097c3c
	.type FunctionHead_08097c3c, %function
	.thumb_func
FunctionHead_08097c3c:
	.incbin "baserom.gba", 0x00097cc8, 0x00000344
	.section .rom.00099a86, "ax"
	.incbin "baserom.gba", 0x00099a86, 0x00000002
	.section .rom.00099a88, "ax"
	.global RunBattleEffect05
	.type RunBattleEffect05, %function
	.thumb_func
RunBattleEffect05:
	.incbin "baserom.gba", 0x00099a88, 0x00000328
	.section .rom.00099e3c, "ax"
	.global Battle_unk3_2
	.type Battle_unk3_2, %function
	.thumb_func
Battle_unk3_2:
	.incbin "baserom.gba", 0x00099e3c, 0x000004f0
	.section .rom.0009ac4c, "ax"
	.global BattleEffect_RunFallbackObjectTransition
	.type BattleEffect_RunFallbackObjectTransition, %function
	.thumb_func
BattleEffect_RunFallbackObjectTransition:
	.incbin "baserom.gba", 0x0009ac4c, 0x000001bc
	.section .rom.0009aefa, "ax"
	.incbin "baserom.gba", 0x0009aefa, 0x00000002
	.section .rom.0009aefc, "ax"
	.global RunBattleEffect13
	.type RunBattleEffect13, %function
	.thumb_func
RunBattleEffect13:
	.incbin "baserom.gba", 0x0009aefc, 0x0000024c
	.section .rom.0009bd90, "ax"
	.global Map_UpdateWorldMapMarkers
	.type Map_UpdateWorldMapMarkers, %function
	.thumb_func
Map_UpdateWorldMapMarkers:
	.incbin "baserom.gba", 0x0009bd90, 0x00000500
	.section .rom.0009c568, "ax"
	.global Data_0809c410
Data_0809c410:
	.incbin "baserom.gba", 0x0009c568, 0x00000100
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "baserom.gba", 0x0009c668, 0x00000100
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x0009c768, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d2c8, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x0009d900, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x0009da08, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x0009db48, 0x000003e8
	.global BattleFx_ConditionResources
BattleFx_ConditionResources:
	.incbin "baserom.gba", 0x0009df30, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x0009e330, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x0009e3c8, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x0009e5e0, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x0009e626, 0x000001b8
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x0009e7de, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x0009e810, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x0009e818, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x0009e8b4, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x0009e9d4, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x0009e9f8, 0x0000004e
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x0009ea46, 0x00000102
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x0009eb48, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x0009ed54, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x0009eed8, 0x000002a4
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x0009f17c, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x0009f1fc, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x0009f208, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x0009f22c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x0009f250, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x0009f274, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x0009f2b8, 0x00000048
	.section .rom.0009f948, "ax"
	.incbin "baserom.gba", 0x0009f948, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x0009f968, 0x000003bc
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x0009fd24, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x0009fd44, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x0009fd5c, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x0009fd74, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x0009fd80, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x0009fd84, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x0009fe90, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x0009fe9c, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x0009ff58, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x0009ff5c, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x0009ff68, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x000a0024, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x000a0070, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x000a0084, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x000a0098, 0x000000de
	.global FieldFx_MoteTiles
FieldFx_MoteTiles:
	.incbin "baserom.gba", 0x000a0176, 0x0000009a
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x000a0210, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a0260, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a0280, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a0284, 0x00000d7c
	.section .rom.000a1abe, "ax"
	.incbin "baserom.gba", 0x000a1abe, 0x00000002
	.section .rom.000a1ac0, "ax"
	.global UiMenu_SlideCursor
	.type UiMenu_SlideCursor, %function
	.thumb_func
UiMenu_SlideCursor:
	.incbin "baserom.gba", 0x000a1ac0, 0x00000108
	.section .rom.000a24d0, "ax"
	.global RunAssetSelectionScreen
	.type RunAssetSelectionScreen, %function
	.thumb_func
RunAssetSelectionScreen:
	.incbin "baserom.gba", 0x000a24d0, 0x000001b0
	.section .rom.000a414a, "ax"
	.incbin "baserom.gba", 0x000a414a, 0x00000002
	.section .rom.000a414c, "ax"
	.global Func_080a414c
	.type Func_080a414c, %function
	.thumb_func
Func_080a414c:
	.incbin "baserom.gba", 0x000a414c, 0x00000340
	.section .rom.000a4f08, "ax"
	.global Func_080a4f08
Func_080a4f08:
	.incbin "baserom.gba", 0x000a4f08, 0x000002c8
	.section .rom.000a5388, "ax"
	.global Unnamed_080a5388
	.type Unnamed_080a5388, %function
	.thumb_func
Unnamed_080a5388:
	.incbin "baserom.gba", 0x000a5388, 0x000001ac
	.section .rom.000a5d3c, "ax"
	.global Menu_ResolveSelectedAction
	.type Menu_ResolveSelectedAction, %function
	.thumb_func
Menu_ResolveSelectedAction:
	.incbin "baserom.gba", 0x000a5d3c, 0x00000320
	.section .rom.000a6690, "ax"
	.global Func_080a6614
	.type Func_080a6614, %function
	.thumb_func
Func_080a6614:
	.incbin "baserom.gba", 0x000a6690, 0x00000180
	.section .rom.000a8680, "ax"
	.global CharacterMenu_DrawStatusAilments
	.type CharacterMenu_DrawStatusAilments, %function
	.thumb_func
CharacterMenu_DrawStatusAilments:
	.incbin "baserom.gba", 0x000a8680, 0x00000300
	.section .rom.000aa7fc, "ax"
	.global Func_080aa768
Func_080aa768:
	.incbin "baserom.gba", 0x000aa7fc, 0x0000051c
	.section .rom.000ab04c, "ax"
	.global Func_080aafb8
Func_080aafb8:
	.incbin "baserom.gba", 0x000ab04c, 0x0000023c
	.section .rom.000ab678, "ax"
	.incbin "baserom.gba", 0x000ab678, 0x000012fc
	.section .rom.000acb30, "ax"
	.global DjinnMenu_DrawStatPreview
	.type DjinnMenu_DrawStatPreview, %function
	.thumb_func
DjinnMenu_DrawStatPreview:
	.incbin "baserom.gba", 0x000acb30, 0x000007bc
	.section .rom.000ad484, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000ad484, 0x000000fc
	.section .rom.000ad74c, "ax"
	.incbin "baserom.gba", 0x000ad74c, 0x00001040
	.section .rom.000aeac4, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000aeac4, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000aebc4, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000aec44, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000aedc4, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000aee44, 0x00000440
	.global Data_080af20c
Data_080af20c:
	.incbin "baserom.gba", 0x000af284, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000af288, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000af28c, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000af290, 0x00000004
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000af294, 0x00000004
	.global Data_080af220
Data_080af220:
	.incbin "baserom.gba", 0x000af298, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000af29c, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000af2a0, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000af2a4, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000af2a8, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000af2ac, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000af2b0, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000af2b4, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000af2e4, 0x00000028
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000af30c, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000af315, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000af31e, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000af329, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000af334, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000af348, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000af35c, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000af374, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000af37c, 0x00000c84
	.section .rom.000b0aac, "ax"
	.global Shop_SelBuy
	.type Shop_SelBuy, %function
	.thumb_func
Shop_SelBuy:
	.incbin "baserom.gba", 0x000b0aac, 0x000004f8
	.section .rom.000b1260, "ax"
	.incbin "baserom.gba", 0x000b1260, 0x00000210
	.section .rom.000b3940, "ax"
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "baserom.gba", 0x000b3940, 0x00000080
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "baserom.gba", 0x000b39c0, 0x00000080
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "baserom.gba", 0x000b3a40, 0x00000080
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "baserom.gba", 0x000b3ac0, 0x00000080
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "baserom.gba", 0x000b3b40, 0x00000080
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "baserom.gba", 0x000b3bc0, 0x00000180
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000b3d40, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000b3e80, 0x00000100
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "baserom.gba", 0x000b3f80, 0x00000180
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000b4100, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000b413c, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000b4146, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000b41ac, 0x00000906
	.global Data_080b4ab2
Data_080b4ab2:
	.incbin "baserom.gba", 0x000b4ab2, 0x00000004
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.incbin "baserom.gba", 0x000b4ab6, 0x0000054a
	.section .rom.000b5204, "ax"
	.incbin "baserom.gba", 0x000b5204, 0x00000008
	.section .rom.000b5218, "ax"
	.incbin "baserom.gba", 0x000b5218, 0x00000040
	.section .rom.000b5534, "ax"
	.incbin "baserom.gba", 0x000b5534, 0x000001ac
	.section .rom.000b56e0, "ax"
	.global Unnamed_080b56e0
	.type Unnamed_080b56e0, %function
	.thumb_func
Unnamed_080b56e0:
	.incbin "baserom.gba", 0x000b56e0, 0x00000184
	.section .rom.000b5f22, "ax"
	.incbin "baserom.gba", 0x000b5f22, 0x00000162
	.section .rom.000b63e0, "ax"
	.global Battle_RunEncounter
	.type Battle_RunEncounter, %function
	.thumb_func
Battle_RunEncounter:
	.incbin "baserom.gba", 0x000b63e0, 0x00000698
	.section .rom.000b75f4, "ax"
	.incbin "baserom.gba", 0x000b75f4, 0x00000130
	.section .rom.000b7750, "ax"
	.incbin "baserom.gba", 0x000b7750, 0x000001ac
	.section .rom.000b7b82, "ax"
	.incbin "baserom.gba", 0x000b7b82, 0x00000002
	.section .rom.000b7b84, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x000b7b84, 0x00000264
	.section .rom.000b8c34, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000b8c34, 0x0000019c
	.section .rom.000b956c, "ax"
	.incbin "baserom.gba", 0x000b956c, 0x000001d0
	.section .rom.000b973c, "ax"
	.global BattlePresentation_AppendLinkedActions
	.type BattlePresentation_AppendLinkedActions, %function
	.thumb_func
BattlePresentation_AppendLinkedActions:
	.incbin "baserom.gba", 0x000b973c, 0x00000190
	.section .rom.000b9b46, "ax"
	.incbin "baserom.gba", 0x000b9b46, 0x00000206
	.section .rom.000b9ed8, "ax"
	.incbin "baserom.gba", 0x000b9ed8, 0x000003bc
	.section .rom.000ba6c4, "ax"
	.incbin "baserom.gba", 0x000ba6c4, 0x0000026c
	.section .rom.000ba98e, "ax"
	.incbin "baserom.gba", 0x000ba98e, 0x00000266
	.section .rom.000bac84, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x000bac84, 0x0000007c
	.section .rom.000bb7d8, "ax"
	.global Unnamed_080bb7c0
	.type Unnamed_080bb7c0, %function
	.thumb_func
Unnamed_080bb7c0:
	.incbin "baserom.gba", 0x000bb7d8, 0x00000118
	.section .rom.000bd43a, "ax"
	.incbin "baserom.gba", 0x000bd43a, 0x00000002
	.section .rom.000bd43c, "ax"
	.global BattleCommand_SelectAutomatic
	.type BattleCommand_SelectAutomatic, %function
	.thumb_func
BattleCommand_SelectAutomatic:
	.incbin "baserom.gba", 0x000bd43c, 0x00000380
	.section .rom.000bd868, "ax"
	.incbin "baserom.gba", 0x000bd868, 0x00000048
	.section .rom.000bd8b0, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x000bd8b0, 0x00000754
	.section .rom.000be1a2, "ax"
	.incbin "baserom.gba", 0x000be1a2, 0x0000107e
	.section .rom.000bfbbc, "ax"
	.incbin "baserom.gba", 0x000bfbbc, 0x00000414
	.section .rom.000c02bc, "ax"
	.incbin "baserom.gba", 0x000c02bc, 0x0000045c
	.section .rom.000c0904, "ax"
	.global BattleBackground_Load
	.type BattleBackground_Load, %function
	.thumb_func
BattleBackground_Load:
	.incbin "baserom.gba", 0x000c0904, 0x00000138
	.section .rom.000c1488, "ax"
	.incbin "baserom.gba", 0x000c1488, 0x00000260
	.section .rom.000c17ae, "ax"
	.incbin "baserom.gba", 0x000c17ae, 0x00000002
	.section .rom.000c17b0, "ax"
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	.incbin "baserom.gba", 0x000c17b0, 0x0000027c
	.section .rom.000c2014, "ax"
	.incbin "baserom.gba", 0x000c2014, 0x0000036c
	.section .rom.000c2a22, "ax"
	.incbin "baserom.gba", 0x000c2a22, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000c2a28, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000c2a34, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000c2a42, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000c2a50, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000c2a5e, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000c2a6c, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000c2a7a, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000c2a94, 0x0000003c
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000c2ad0, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000c2ad8, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000c2af0, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000c2b08, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000c2b20, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000c2b38, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000c2b50, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000c2b68, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000c2b80, 0x00000a54
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000c35d4, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000c361c, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000c3638, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000c3640, 0x0000010c
	.global BattlePres_AdvanceArrowTiles
BattlePres_AdvanceArrowTiles:
	.incbin "baserom.gba", 0x000c374c, 0x00000800
	.global Data_080c3f34
Data_080c3f34:
	.incbin "baserom.gba", 0x000c3f4c, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000c5950, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000c5954, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000c59bc, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000c59dc, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000c59fc, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000c5a1c, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000c5a48, 0x000001e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c5c28, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000c5c50, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000c7410, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000c7438, 0x00001bc8
	.section .rom.000c91dc, "ax"
	.incbin "baserom.gba", 0x000c91dc, 0x00000a84
	.section .rom.000c9ca8, "ax"
	.global BattleFx_RunFiveMode
	.type BattleFx_RunFiveMode, %function
	.thumb_func
BattleFx_RunFiveMode:
	.incbin "baserom.gba", 0x000c9ca8, 0x0000053c
	.section .rom.000ca1fc, "ax"
	.global BattleFx_RunParticlePool
	.type BattleFx_RunParticlePool, %function
	.thumb_func
BattleFx_RunParticlePool:
	.incbin "baserom.gba", 0x000ca1fc, 0x00000380
	.section .rom.000ca60c, "ax"
	.global BattleFx_RunTwelveMode
	.type BattleFx_RunTwelveMode, %function
	.thumb_func
BattleFx_RunTwelveMode:
	.incbin "baserom.gba", 0x000ca60c, 0x00000b98
	.section .rom.000cb4ec, "ax"
	.incbin "baserom.gba", 0x000cb4ec, 0x0000030c
	.section .rom.000cb7f8, "ax"
	.global Unnamed_080cb7f8
	.type Unnamed_080cb7f8, %function
	.thumb_func
Unnamed_080cb7f8:
	.incbin "baserom.gba", 0x000cb7f8, 0x00000414
	.section .rom.000cbc0c, "ax"
	.global BattleEffect_RunTileAndPaletteAnimation
	.type BattleEffect_RunTileAndPaletteAnimation, %function
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	.incbin "baserom.gba", 0x000cbc0c, 0x000009cc
	.section .rom.000cc5d8, "ax"
	.global Func_080cc5d8
	.type Func_080cc5d8, %function
	.thumb_func
Func_080cc5d8:
	.incbin "baserom.gba", 0x000cc5d8, 0x00000388
	.section .rom.000ccebc, "ax"
	.incbin "baserom.gba", 0x000ccebc, 0x00000248
	.section .rom.000ce034, "ax"
	.incbin "baserom.gba", 0x000ce034, 0x00000828
	.section .rom.000ceb54, "ax"
	.global BattleFx_RunMemberBurst
	.type BattleFx_RunMemberBurst, %function
	.thumb_func
BattleFx_RunMemberBurst:
	.incbin "baserom.gba", 0x000ceb54, 0x00000410
	.section .rom.000ceff8, "ax"
	.global BattleFx_RunFortyEightFrameEffect
	.type BattleFx_RunFortyEightFrameEffect, %function
	.thumb_func
BattleFx_RunFortyEightFrameEffect:
	.incbin "baserom.gba", 0x000ceff8, 0x000002a8
	.section .rom.000cf2b8, "ax"
	.global BattleFx_RunMemberBeam
	.type BattleFx_RunMemberBeam, %function
	.thumb_func
BattleFx_RunMemberBeam:
	.incbin "baserom.gba", 0x000cf2b8, 0x000005d4
	.section .rom.000cf8e0, "ax"
	.global BattleFx_RunSevenMode
	.type BattleFx_RunSevenMode, %function
	.thumb_func
BattleFx_RunSevenMode:
	.incbin "baserom.gba", 0x000cf8e0, 0x00000614
	.section .rom.000d05fc, "ax"
	.incbin "baserom.gba", 0x000d05fc, 0x00001118
	.section .rom.000d1714, "ax"
	.global Unnamed_080d1714
	.type Unnamed_080d1714, %function
	.thumb_func
Unnamed_080d1714:
	.incbin "baserom.gba", 0x000d1714, 0x00000d38
	.section .rom.000d2464, "ax"
	.global BattleEffect_RunPaletteParticles
	.type BattleEffect_RunPaletteParticles, %function
	.thumb_func
BattleEffect_RunPaletteParticles:
	.incbin "baserom.gba", 0x000d2464, 0x00000934
	.section .rom.000d2d98, "ax"
	.global BattleEffect_RunEmberColumns
	.type BattleEffect_RunEmberColumns, %function
	.thumb_func
BattleEffect_RunEmberColumns:
	.incbin "baserom.gba", 0x000d2d98, 0x00001354
	.section .rom.000d41a4, "ax"
	.incbin "baserom.gba", 0x000d41a4, 0x00000448
	.section .rom.000d4604, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x000d4604, 0x00000c54
	.section .rom.000d52c8, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x000d52c8, 0x000006e8
	.section .rom.000d5e54, "ax"
	.incbin "baserom.gba", 0x000d5e54, 0x000006b0
	.section .rom.000d6970, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000d6970, 0x00000cec
	.section .rom.000d82b0, "ax"
	.incbin "baserom.gba", 0x000d82b0, 0x00000698
	.section .rom.000d89ac, "ax"
	.global BattleEffectA
	.type BattleEffectA, %function
	.thumb_func
BattleEffectA:
	.incbin "baserom.gba", 0x000d89ac, 0x000007e8
	.section .rom.000d91dc, "ax"
	.global BattleEffectB
	.type BattleEffectB, %function
	.thumb_func
BattleEffectB:
	.incbin "baserom.gba", 0x000d91dc, 0x000008dc
	.section .rom.000d9ae8, "ax"
	.global RunPaletteRampEffect
	.type RunPaletteRampEffect, %function
	.thumb_func
RunPaletteRampEffect:
	.incbin "baserom.gba", 0x000d9ae8, 0x000004e0
	.section .rom.000da2ac, "ax"
	.incbin "baserom.gba", 0x000da2ac, 0x0000141c
	.section .rom.000db6e0, "ax"
	.global RunParticleFieldEffect
	.type RunParticleFieldEffect, %function
	.thumb_func
RunParticleFieldEffect:
	.incbin "baserom.gba", 0x000db6e0, 0x00000444
	.section .rom.000dea6e, "ax"
	.incbin "baserom.gba", 0x000dea6e, 0x00000002
	.section .rom.000dea70, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x000dea70, 0x00000e48
	.section .rom.000dfe2a, "ax"
	.incbin "baserom.gba", 0x000dfe2a, 0x00000002
	.section .rom.000e15e8, "ax"
	.global BattleFx_InitializeMode12
	.type BattleFx_InitializeMode12, %function
	.thumb_func
BattleFx_InitializeMode12:
	.incbin "baserom.gba", 0x000e15e8, 0x00000f50
	.section .rom.000e2972, "ax"
	.incbin "baserom.gba", 0x000e2972, 0x00000002
	.section .rom.000e302c, "ax"
	.incbin "baserom.gba", 0x000e302c, 0x0000088c
	.section .rom.000e3aa0, "ax"
	.global BattlePres_RunBeamSequence
	.type BattlePres_RunBeamSequence, %function
	.thumb_func
BattlePres_RunBeamSequence:
	.incbin "baserom.gba", 0x000e3aa0, 0x00000604
	.section .rom.000e40a4, "ax"
	.global Unnamed_080e40a4
	.type Unnamed_080e40a4, %function
	.thumb_func
Unnamed_080e40a4:
	.incbin "baserom.gba", 0x000e40a4, 0x0000064c
	.section .rom.000e47b8, "ax"
	.global BattleFx_RunCastingImpact
	.type BattleFx_RunCastingImpact, %function
	.thumb_func
BattleFx_RunCastingImpact:
	.incbin "baserom.gba", 0x000e47b8, 0x00002190
	.section .rom.000e698c, "ax"
	.incbin "baserom.gba", 0x000e698c, 0x000003b0
	.section .rom.000e6eac, "ax"
	.incbin "baserom.gba", 0x000e6eac, 0x000003d0
	.section .rom.000e7338, "ax"
	.incbin "baserom.gba", 0x000e7338, 0x000000cc
	.section .rom.000e7404, "ax"
	.global BattleEffect_RunParticleStreams
	.type BattleEffect_RunParticleStreams, %function
	.thumb_func
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x000e7404, 0x00000e38
	.section .rom.000e823c, "ax"
	.global BattleEffect_RunCirclingFallingScene
	.type BattleEffect_RunCirclingFallingScene, %function
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	.incbin "baserom.gba", 0x000e823c, 0x00001e9c
	.section .rom.000ea0d8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000ea0d8, 0x0000167c
	.section .rom.000eb754, "ax"
	.global Unnamed_080eb754
	.type Unnamed_080eb754, %function
	.thumb_func
Unnamed_080eb754:
	.incbin "baserom.gba", 0x000eb754, 0x0000098c
	.section .rom.000ed408, "ax"
	.global Unnamed_080ed408
	.type Unnamed_080ed408, %function
	.thumb_func
Unnamed_080ed408:
	.global BattleEffect_LoadWork
BattleEffect_LoadWork:
	.incbin "baserom.gba", 0x000ed408, 0x00000678
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000eda80, 0x00000038
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000edab8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000edac0, 0x00000028
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000edae8, 0x00000008
	.section .rom.000ede48, "ax"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000ede48, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000ede5c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000ede84, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000ede96, 0x00000009
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000ede9f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000edea5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000edeab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000edeb2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000edebe, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000edeca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000eded0, 0x00000006
	.incbin "baserom.gba", 0x000eded6, 0x0000018e
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000ee064, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000ee06a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000ee070, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000ee07c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000ee088, 0x00000084
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000ee10c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000ee11a, 0x0000019a
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000ee2b4, 0x000006c0
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000ee974, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000ee994, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000ee998, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000ee99e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000ee9a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000ee9a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000ee9a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000ee9b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000ee9be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000ee9d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000ee9d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000ee9da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000ee9e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000ee9e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000ee9e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000ee9ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000ee9ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000ee9f2, 0x0000014e
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000eeb40, 0x00000008
	.global Data_080eeb48
Data_080eeb48:
	.incbin "baserom.gba", 0x000eeb48, 0x00000003
	.global Data_080eeb4b
Data_080eeb4b:
	.incbin "baserom.gba", 0x000eeb4b, 0x00000003
	.global Data_080eeb4e
Data_080eeb4e:
	.incbin "baserom.gba", 0x000eeb4e, 0x00000006
	.global Data_080eeb54
Data_080eeb54:
	.incbin "baserom.gba", 0x000eeb54, 0x00000004
	.global Data_080eeb58
Data_080eeb58:
	.incbin "baserom.gba", 0x000eeb58, 0x00000006
	.global Data_080eeb5e
Data_080eeb5e:
	.incbin "baserom.gba", 0x000eeb5e, 0x00000003
	.global Data_080eeb61
Data_080eeb61:
	.incbin "baserom.gba", 0x000eeb61, 0x00000010
	.global Data_080eeb71
Data_080eeb71:
	.incbin "baserom.gba", 0x000eeb71, 0x00000008
	.global Data_080eeb79
Data_080eeb79:
	.incbin "baserom.gba", 0x000eeb79, 0x00000007
	.global Data_080eeb80
Data_080eeb80:
	.incbin "baserom.gba", 0x000eeb80, 0x00000008
	.global Data_080eeb88
Data_080eeb88:
	.incbin "baserom.gba", 0x000eeb88, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000eeb96, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000eeba6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000eebae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000eebb6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000eebb9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000eebc0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000eebc8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000eebd6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000eebe2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000eebe6, 0x00000002
	.incbin "baserom.gba", 0x000eebe8, 0x00000072
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000eec5a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000eec5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000eec63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eec68, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000eec70, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000eec74, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000eec7d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000eec86, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000eec98, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000eeca1, 0x00000009
	.incbin "baserom.gba", 0x000eecaa, 0x00000008
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000eecb2, 0x0000002a
	.incbin "baserom.gba", 0x000eecdc, 0x00000016
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000eecf2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000eecf7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000eecfc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000eecff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000eed0e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000eed1e, 0x0000001e
	.incbin "baserom.gba", 0x000eed3c, 0x00000002
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000eed3e, 0x00000040
	.incbin "baserom.gba", 0x000eed7e, 0x00000052
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000eedd0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000eedd4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000eede2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000eedea, 0x0000000a
	.incbin "baserom.gba", 0x000eedf4, 0x0000002a
	.global Data_080eee1e
Data_080eee1e:
	.incbin "baserom.gba", 0x000eee1e, 0x0000000c
	.global Data_080eee2a
Data_080eee2a:
	.incbin "baserom.gba", 0x000eee2a, 0x0000000c
	.global Data_080eee36
Data_080eee36:
	.incbin "baserom.gba", 0x000eee36, 0x00000008
	.global Data_080eee3e
Data_080eee3e:
	.incbin "baserom.gba", 0x000eee3e, 0x00000008
	.global Data_080eee46
Data_080eee46:
	.incbin "baserom.gba", 0x000eee46, 0x00000008
	.global Data_080eee4e
Data_080eee4e:
	.incbin "baserom.gba", 0x000eee4e, 0x0000011a
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000eef68, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000eef70, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000eef78, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000eef88, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000eef96, 0x0000000e
	.section .rom.000ef014, "ax"
	.incbin "baserom.gba", 0x000ef014, 0x00000fec
	.section .rom.000f07f0, "ax"
	.global Func_080f07f0
Func_080f07f0:
	.incbin "baserom.gba", 0x000f07f0, 0x0000026c
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f0a5c, 0x0000084c
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f12a8, 0x00000d58
	.section .rom.000f2028, "ax"
	.incbin "baserom.gba", 0x000f2028, 0x000006c4
	.section .rom.000f26ec, "ax"
	.global Func_080f26ec
	.type Func_080f26ec, %function
	.thumb_func
Func_080f26ec:
	.incbin "baserom.gba", 0x000f26ec, 0x00000480
	.section .rom.000f2b6c, "ax"
	.global Func_080f2b6c
	.type Func_080f2b6c, %function
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f2b6c, 0x00000004
	.section .rom.000f3078, "ax"
	.global Unnamed_080f3078
	.type Unnamed_080f3078, %function
	.thumb_func
Unnamed_080f3078:
	.incbin "baserom.gba", 0x000f3078, 0x00000704
	.section .rom.000f38bc, "ax"
	.incbin "baserom.gba", 0x000f38bc, 0x00000744
	.section .rom.000f4168, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000f4168, 0x00001e98
	.section .rom.000f6440, "ax"
	.incbin "baserom.gba", 0x000f6440, 0x00000eec
	.section .rom.000f7470, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000f7470, 0x00000954
	.section .rom.000f7f88, "ax"
	.incbin "baserom.gba", 0x000f7f88, 0x000007be
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f8746, 0x000008ba
	.section .rom.000fb7a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fb7a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fb830, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fb8e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fb914, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fb92c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fb9b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fb9c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fba04, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fba14, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fba48, 0x00000030
	.section .rom.000fc504, "ax"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .rom.000fc624, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fc624, 0x00000060
	.section .rom.00184698, "ax"
	.incbin "baserom.gba", 0x00184698, 0x00000968
	.section .rom.0031efe0, "ax"
	.incbin "baserom.gba", 0x0031efe0, 0x00001020
	.section .rom.00320fa0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320fa0, 0x00000010
	.section .rom.003249e7, "ax"
	.incbin "baserom.gba", 0x003249e7, 0x00000001
	.section .rom.0032b09b, "ax"
	.incbin "baserom.gba", 0x0032b09b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b09c, 0x000086f8
	.section .rom.003357e5, "ax"
	.incbin "baserom.gba", 0x003357e5, 0x00000003
	.section .rom.003370f5, "ax"
	.incbin "baserom.gba", 0x003370f5, 0x00000003
	.section .rom.0033abf9, "ax"
	.incbin "baserom.gba", 0x0033abf9, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033abfc, 0x00000688
	.section .rom.0033f6c6, "ax"
	.incbin "baserom.gba", 0x0033f6c6, 0x00000002
	.section .rom.0034fe36, "ax"
	.incbin "baserom.gba", 0x0034fe36, 0x00000002
	.section .rom.00353426, "ax"
	.incbin "baserom.gba", 0x00353426, 0x00000002
	.section .rom.0035eeca, "ax"
	.incbin "baserom.gba", 0x0035eeca, 0x00000002
	.section .rom.0036f57a, "ax"
	.incbin "baserom.gba", 0x0036f57a, 0x00000002
	.section .rom.0037701a, "ax"
	.incbin "baserom.gba", 0x0037701a, 0x00000002
	.section .rom.0037a85a, "ax"
	.incbin "baserom.gba", 0x0037a85a, 0x00000002
	.section .rom.00383c8e, "ax"
	.incbin "baserom.gba", 0x00383c8e, 0x00000002
	.section .rom.0038f0b6, "ax"
	.incbin "baserom.gba", 0x0038f0b6, 0x00000002
	.section .rom.00396f72, "ax"
	.incbin "baserom.gba", 0x00396f72, 0x00000002
	.section .rom.0039ba02, "ax"
	.incbin "baserom.gba", 0x0039ba02, 0x00000002
	.section .rom.0039fcb6, "ax"
	.incbin "baserom.gba", 0x0039fcb6, 0x00000002
	.section .rom.003a3636, "ax"
	.incbin "baserom.gba", 0x003a3636, 0x00000002
	.section .rom.003afe72, "ax"
	.incbin "baserom.gba", 0x003afe72, 0x00000002
	.section .rom.003be7ba, "ax"
	.incbin "baserom.gba", 0x003be7ba, 0x00000002
	.section .rom.003c40bd, "ax"
	.incbin "baserom.gba", 0x003c40bd, 0x00000003
	.section .rom.003c5a3f, "ax"
	.incbin "baserom.gba", 0x003c5a3f, 0x00000001
	.section .rom.003c885d, "ax"
	.incbin "baserom.gba", 0x003c885d, 0x00000003
	.section .rom.003cbfa9, "ax"
	.incbin "baserom.gba", 0x003cbfa9, 0x00000003
	.section .rom.003cd7bf, "ax"
	.incbin "baserom.gba", 0x003cd7bf, 0x00000001
	.section .rom.003cddaa, "ax"
	.incbin "baserom.gba", 0x003cddaa, 0x00000002
	.section .rom.003ce46e, "ax"
	.incbin "baserom.gba", 0x003ce46e, 0x00000002
	.section .rom.003ce755, "ax"
	.incbin "baserom.gba", 0x003ce755, 0x00000003
	.section .rom.003cfabb, "ax"
	.incbin "baserom.gba", 0x003cfabb, 0x00000001
	.section .rom.003cfea5, "ax"
	.incbin "baserom.gba", 0x003cfea5, 0x00000003
	.section .rom.003d0277, "ax"
	.incbin "baserom.gba", 0x003d0277, 0x00000001
	.section .rom.003d0b17, "ax"
	.incbin "baserom.gba", 0x003d0b17, 0x00000001
	.section .rom.003d0fba, "ax"
	.incbin "baserom.gba", 0x003d0fba, 0x00000002
	.section .rom.003d2173, "ax"
	.incbin "baserom.gba", 0x003d2173, 0x00000001
	.section .rom.003d3636, "ax"
	.incbin "baserom.gba", 0x003d3636, 0x00000002
	.section .rom.003d45ff, "ax"
	.incbin "baserom.gba", 0x003d45ff, 0x00000001
	.section .rom.003d485a, "ax"
	.incbin "baserom.gba", 0x003d485a, 0x00000002
	.section .rom.003d62b5, "ax"
	.incbin "baserom.gba", 0x003d62b5, 0x00000003
	.section .rom.003d6801, "ax"
	.incbin "baserom.gba", 0x003d6801, 0x00000003
	.section .rom.003d858a, "ax"
	.incbin "baserom.gba", 0x003d858a, 0x00000002
	.section .rom.003d8803, "ax"
	.incbin "baserom.gba", 0x003d8803, 0x00000001
	.section .rom.003d8cca, "ax"
	.incbin "baserom.gba", 0x003d8cca, 0x00000002
	.section .rom.003da89f, "ax"
	.incbin "baserom.gba", 0x003da89f, 0x00000001
	.section .rom.003dc49d, "ax"
	.incbin "baserom.gba", 0x003dc49d, 0x00000003
	.section .rom.003dc6be, "ax"
	.incbin "baserom.gba", 0x003dc6be, 0x00000002
	.section .rom.003dcafb, "ax"
	.incbin "baserom.gba", 0x003dcafb, 0x00000001
	.section .rom.003dcc0d, "ax"
	.incbin "baserom.gba", 0x003dcc0d, 0x00000003
	.section .rom.003dd38b, "ax"
	.incbin "baserom.gba", 0x003dd38b, 0x00000001
	.section .rom.003dd829, "ax"
	.incbin "baserom.gba", 0x003dd829, 0x00000003
	.section .rom.003de53e, "ax"
	.incbin "baserom.gba", 0x003de53e, 0x00000002
	.section .rom.003df2be, "ax"
	.incbin "baserom.gba", 0x003df2be, 0x00000002
	.section .rom.003df64f, "ax"
	.incbin "baserom.gba", 0x003df64f, 0x00000001
	.section .rom.003e1396, "ax"
	.incbin "baserom.gba", 0x003e1396, 0x00000002
	.section .rom.003e216d, "ax"
	.incbin "baserom.gba", 0x003e216d, 0x00000003
	.section .rom.003e299a, "ax"
	.incbin "baserom.gba", 0x003e299a, 0x00000002
	.section .rom.003e3025, "ax"
	.incbin "baserom.gba", 0x003e3025, 0x00000003
	.section .rom.003e31e3, "ax"
	.incbin "baserom.gba", 0x003e31e3, 0x00000001
	.section .rom.003e3f3b, "ax"
	.incbin "baserom.gba", 0x003e3f3b, 0x00000001
	.section .rom.003e4487, "ax"
	.incbin "baserom.gba", 0x003e4487, 0x00000001
	.section .rom.003e4861, "ax"
	.incbin "baserom.gba", 0x003e4861, 0x00000003
	.section .rom.003e4c0f, "ax"
	.incbin "baserom.gba", 0x003e4c0f, 0x00000001
	.section .rom.003e6bb3, "ax"
	.incbin "baserom.gba", 0x003e6bb3, 0x00000001
	.section .rom.003e794f, "ax"
	.incbin "baserom.gba", 0x003e794f, 0x00000001
	.section .rom.003e7b6b, "ax"
	.incbin "baserom.gba", 0x003e7b6b, 0x00000001
	.section .rom.003e7e67, "ax"
	.incbin "baserom.gba", 0x003e7e67, 0x00000001
	.section .rom.003ea015, "ax"
	.incbin "baserom.gba", 0x003ea015, 0x00000003
	.section .rom.003eade3, "ax"
	.incbin "baserom.gba", 0x003eade3, 0x00000001
	.section .rom.003ebf45, "ax"
	.incbin "baserom.gba", 0x003ebf45, 0x00000003
	.section .rom.003ec49f, "ax"
	.incbin "baserom.gba", 0x003ec49f, 0x00000001
	.section .rom.003edd89, "ax"
	.incbin "baserom.gba", 0x003edd89, 0x00000003
	.section .rom.003ee62a, "ax"
	.incbin "baserom.gba", 0x003ee62a, 0x00000002
	.section .rom.003eea07, "ax"
	.incbin "baserom.gba", 0x003eea07, 0x00000001
	.section .rom.003eec91, "ax"
	.incbin "baserom.gba", 0x003eec91, 0x00000003
	.section .rom.003ef037, "ax"
	.incbin "baserom.gba", 0x003ef037, 0x00000001
	.section .rom.003ef292, "ax"
	.incbin "baserom.gba", 0x003ef292, 0x00000002
	.section .rom.003ef64a, "ax"
	.incbin "baserom.gba", 0x003ef64a, 0x00000002
	.section .rom.003f0b33, "ax"
	.incbin "baserom.gba", 0x003f0b33, 0x00000001
	.section .rom.003f20f6, "ax"
	.incbin "baserom.gba", 0x003f20f6, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f20f8, 0x00000a6c
	.section .rom.003f393a, "ax"
	.incbin "baserom.gba", 0x003f393a, 0x00000002
	.section .rom.003f3cbd, "ax"
	.incbin "baserom.gba", 0x003f3cbd, 0x00000003
	.section .rom.003f496d, "ax"
	.incbin "baserom.gba", 0x003f496d, 0x00000003
	.section .rom.003f54b5, "ax"
	.incbin "baserom.gba", 0x003f54b5, 0x00000003
	.section .rom.003f564e, "ax"
	.incbin "baserom.gba", 0x003f564e, 0x00000002
	.section .rom.003f5edb, "ax"
	.incbin "baserom.gba", 0x003f5edb, 0x00000001
	.section .rom.003f69a2, "ax"
	.incbin "baserom.gba", 0x003f69a2, 0x00000002
	.section .rom.003f6eba, "ax"
	.incbin "baserom.gba", 0x003f6eba, 0x00000002
	.section .rom.003f74de, "ax"
	.incbin "baserom.gba", 0x003f74de, 0x00000002
	.section .rom.003f7d45, "ax"
	.incbin "baserom.gba", 0x003f7d45, 0x00000003
	.section .rom.003f8169, "ax"
	.incbin "baserom.gba", 0x003f8169, 0x00000003
	.section .rom.003f8403, "ax"
	.incbin "baserom.gba", 0x003f8403, 0x00000001
	.section .rom.003f9d9f, "ax"
	.incbin "baserom.gba", 0x003f9d9f, 0x00000001
	.section .rom.003fbb16, "ax"
	.incbin "baserom.gba", 0x003fbb16, 0x00000002
	.section .rom.003fda8b, "ax"
	.incbin "baserom.gba", 0x003fda8b, 0x00000001
	.section .rom.003fdf5b, "ax"
	.incbin "baserom.gba", 0x003fdf5b, 0x00000001
	.section .rom.003fe5ed, "ax"
	.incbin "baserom.gba", 0x003fe5ed, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe5f0, 0x00000a34
	.section .rom.004003f5, "ax"
	.incbin "baserom.gba", 0x004003f5, 0x00000003
	.section .rom.00400ec6, "ax"
	.incbin "baserom.gba", 0x00400ec6, 0x00000002
	.section .rom.00401a03, "ax"
	.incbin "baserom.gba", 0x00401a03, 0x00000001
	.section .rom.00402043, "ax"
	.incbin "baserom.gba", 0x00402043, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00402044, 0x00001588
	.section .rom.0040362d, "ax"
	.incbin "baserom.gba", 0x0040362d, 0x00000003
	.section .rom.0040399b, "ax"
	.incbin "baserom.gba", 0x0040399b, 0x00000001
	.section .rom.00403f39, "ax"
	.incbin "baserom.gba", 0x00403f39, 0x00000003
	.section .rom.0040426d, "ax"
	.incbin "baserom.gba", 0x0040426d, 0x00000003
	.section .rom.004045a9, "ax"
	.incbin "baserom.gba", 0x004045a9, 0x00000003
	.section .rom.004055c2, "ax"
	.incbin "baserom.gba", 0x004055c2, 0x00000002
	.section .rom.00408bce, "ax"
	.incbin "baserom.gba", 0x00408bce, 0x00000002
	.section .rom.00409371, "ax"
	.incbin "baserom.gba", 0x00409371, 0x00000003
	.section .rom.0040a703, "ax"
	.incbin "baserom.gba", 0x0040a703, 0x00000001
	.section .rom.0040ab33, "ax"
	.incbin "baserom.gba", 0x0040ab33, 0x00000001
	.section .rom.0040c929, "ax"
	.incbin "baserom.gba", 0x0040c929, 0x00000003
	.section .rom.0040d23e, "ax"
	.incbin "baserom.gba", 0x0040d23e, 0x00000002
	.section .rom.0040ed72, "ax"
	.incbin "baserom.gba", 0x0040ed72, 0x00000002
	.section .rom.0040fdc1, "ax"
	.incbin "baserom.gba", 0x0040fdc1, 0x00000003
	.section .rom.00410477, "ax"
	.incbin "baserom.gba", 0x00410477, 0x00000001
	.section .rom.0041151d, "ax"
	.incbin "baserom.gba", 0x0041151d, 0x00000003
	.section .rom.0042492f, "ax"
	.incbin "baserom.gba", 0x0042492f, 0x00000001
	.section .rom.00424a81, "ax"
	.incbin "baserom.gba", 0x00424a81, 0x00000003
	.section .rom.00424f12, "ax"
	.incbin "baserom.gba", 0x00424f12, 0x00000002
	.section .rom.00425109, "ax"
	.incbin "baserom.gba", 0x00425109, 0x00000003
	.section .rom.004267c6, "ax"
	.incbin "baserom.gba", 0x004267c6, 0x00000002
	.section .rom.00427ce5, "ax"
	.incbin "baserom.gba", 0x00427ce5, 0x00000003
	.section .rom.004288b1, "ax"
	.incbin "baserom.gba", 0x004288b1, 0x00000003
	.section .rom.00429426, "ax"
	.incbin "baserom.gba", 0x00429426, 0x00000002
	.section .rom.0042b2cf, "ax"
	.incbin "baserom.gba", 0x0042b2cf, 0x00000001
	.section .rom.0042ca5d, "ax"
	.incbin "baserom.gba", 0x0042ca5d, 0x00000003
	.section .rom.0042df0e, "ax"
	.incbin "baserom.gba", 0x0042df0e, 0x00000002
	.section .rom.0043065b, "ax"
	.incbin "baserom.gba", 0x0043065b, 0x00000001
	.section .rom.00431ef5, "ax"
	.incbin "baserom.gba", 0x00431ef5, 0x00000003
	.section .rom.004334be, "ax"
	.incbin "baserom.gba", 0x004334be, 0x00000002
	.section .rom.00434f4e, "ax"
	.incbin "baserom.gba", 0x00434f4e, 0x00000002
	.section .rom.0043f536, "ax"
	.incbin "baserom.gba", 0x0043f536, 0x00000002
	.section .rom.004416ea, "ax"
	.incbin "baserom.gba", 0x004416ea, 0x00000002
	.section .rom.00445851, "ax"
	.incbin "baserom.gba", 0x00445851, 0x00000003
	.section .rom.00448a76, "ax"
	.incbin "baserom.gba", 0x00448a76, 0x00000002
	.section .rom.0044a17a, "ax"
	.incbin "baserom.gba", 0x0044a17a, 0x00000002
	.section .rom.00450d62, "ax"
	.incbin "baserom.gba", 0x00450d62, 0x00000002
	.section .rom.004596e6, "ax"
	.incbin "baserom.gba", 0x004596e6, 0x00000002
	.section .rom.004611c2, "ax"
	.incbin "baserom.gba", 0x004611c2, 0x00000002
	.section .rom.00464256, "ax"
	.incbin "baserom.gba", 0x00464256, 0x00000002
	.section .rom.00467665, "ax"
	.incbin "baserom.gba", 0x00467665, 0x00000003
	.section .rom.00469ed1, "ax"
	.incbin "baserom.gba", 0x00469ed1, 0x00000003
	.section .rom.0046b9c2, "ax"
	.incbin "baserom.gba", 0x0046b9c2, 0x00000002
	.section .rom.0046c7da, "ax"
	.incbin "baserom.gba", 0x0046c7da, 0x00000002
	.section .rom.0046d4c5, "ax"
	.incbin "baserom.gba", 0x0046d4c5, 0x00000003
	.section .rom.00476982, "ax"
	.incbin "baserom.gba", 0x00476982, 0x00000002
	.section .rom.0047768d, "ax"
	.incbin "baserom.gba", 0x0047768d, 0x00000003
	.section .rom.004797a1, "ax"
	.incbin "baserom.gba", 0x004797a1, 0x00000003
	.section .rom.0047a413, "ax"
	.incbin "baserom.gba", 0x0047a413, 0x00000001
	.section .rom.0047b02b, "ax"
	.incbin "baserom.gba", 0x0047b02b, 0x00000001
	.section .rom.0047b79f, "ax"
	.incbin "baserom.gba", 0x0047b79f, 0x00000001
	.section .rom.0047d02f, "ax"
	.incbin "baserom.gba", 0x0047d02f, 0x00000001
	.section .rom.0047d9fd, "ax"
	.incbin "baserom.gba", 0x0047d9fd, 0x00000003
	.section .rom.0047e61b, "ax"
	.incbin "baserom.gba", 0x0047e61b, 0x00000001
	.section .rom.00480ccf, "ax"
	.incbin "baserom.gba", 0x00480ccf, 0x00000001
	.section .rom.004833e2, "ax"
	.incbin "baserom.gba", 0x004833e2, 0x00000002
	.section .rom.00488337, "ax"
	.incbin "baserom.gba", 0x00488337, 0x00000001
	.section .rom.0048c661, "ax"
	.incbin "baserom.gba", 0x0048c661, 0x00000003
	.section .rom.0048d24e, "ax"
	.incbin "baserom.gba", 0x0048d24e, 0x00000002
	.section .rom.00491e03, "ax"
	.incbin "baserom.gba", 0x00491e03, 0x00000001
	.section .rom.0049416d, "ax"
	.incbin "baserom.gba", 0x0049416d, 0x00000003
	.section .rom.00495b0f, "ax"
	.incbin "baserom.gba", 0x00495b0f, 0x00000001
	.section .rom.0049a3d1, "ax"
	.incbin "baserom.gba", 0x0049a3d1, 0x00000003
	.section .rom.0049e499, "ax"
	.incbin "baserom.gba", 0x0049e499, 0x00000003
	.section .rom.004a1b62, "ax"
	.incbin "baserom.gba", 0x004a1b62, 0x00000002
	.section .rom.004a9b33, "ax"
	.incbin "baserom.gba", 0x004a9b33, 0x00000001
	.section .rom.004b0e43, "ax"
	.incbin "baserom.gba", 0x004b0e43, 0x00000001
	.section .rom.004b5dc3, "ax"
	.incbin "baserom.gba", 0x004b5dc3, 0x00000001
	.section .rom.004ba773, "ax"
	.incbin "baserom.gba", 0x004ba773, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004ba774, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004ba780, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004ba8d0, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004baa10, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bab50, 0x00000140
	.section .rom.004c009f, "ax"
	.incbin "baserom.gba", 0x004c009f, 0x00000001
	.section .rom.004c022d, "ax"
	.incbin "baserom.gba", 0x004c022d, 0x00000003
	.section .rom.004c528d, "ax"
	.incbin "baserom.gba", 0x004c528d, 0x00000003
	.section .rom.004c97f9, "ax"
	.incbin "baserom.gba", 0x004c97f9, 0x00000003
	.section .rom.004ce9bd, "ax"
	.incbin "baserom.gba", 0x004ce9bd, 0x00000003
	.section .rom.004ceb53, "ax"
	.incbin "baserom.gba", 0x004ceb53, 0x00000001
	.section .rom.004d17a7, "ax"
	.incbin "baserom.gba", 0x004d17a7, 0x00000001
	.section .rom.004d845b, "ax"
	.incbin "baserom.gba", 0x004d845b, 0x00000001
	.section .rom.004db596, "ax"
	.incbin "baserom.gba", 0x004db596, 0x00000002
	.section .rom.004db717, "ax"
	.incbin "baserom.gba", 0x004db717, 0x00000001
	.section .rom.004dded1, "ax"
	.incbin "baserom.gba", 0x004dded1, 0x00000003
	.section .rom.004e051a, "ax"
	.incbin "baserom.gba", 0x004e051a, 0x00000002
	.section .rom.004e2bfe, "ax"
	.incbin "baserom.gba", 0x004e2bfe, 0x00000002
	.section .rom.004e3c73, "ax"
	.incbin "baserom.gba", 0x004e3c73, 0x00000001
	.section .rom.004e5dc6, "ax"
	.incbin "baserom.gba", 0x004e5dc6, 0x00000002
	.section .rom.004e5f59, "ax"
	.incbin "baserom.gba", 0x004e5f59, 0x00000003
	.section .rom.004e879b, "ax"
	.incbin "baserom.gba", 0x004e879b, 0x00000001
	.section .rom.004ea5b1, "ax"
	.incbin "baserom.gba", 0x004ea5b1, 0x00000003
	.section .rom.004ecc92, "ax"
	.incbin "baserom.gba", 0x004ecc92, 0x00000002
	.section .rom.004eddd9, "ax"
	.incbin "baserom.gba", 0x004eddd9, 0x00000003
	.section .rom.004f0eed, "ax"
	.incbin "baserom.gba", 0x004f0eed, 0x00000003
	.section .rom.004f5055, "ax"
	.incbin "baserom.gba", 0x004f5055, 0x00000003
	.section .rom.004f6652, "ax"
	.incbin "baserom.gba", 0x004f6652, 0x00000002
	.section .rom.004f791b, "ax"
	.incbin "baserom.gba", 0x004f791b, 0x00000001
	.section .rom.004fad33, "ax"
	.incbin "baserom.gba", 0x004fad33, 0x00000001
	.section .rom.004faec1, "ax"
	.incbin "baserom.gba", 0x004faec1, 0x00000003
	.section .rom.004fcc72, "ax"
	.incbin "baserom.gba", 0x004fcc72, 0x00000002
	.section .rom.005007e9, "ax"
	.incbin "baserom.gba", 0x005007e9, 0x00000003
	.section .rom.00501cf7, "ax"
	.incbin "baserom.gba", 0x00501cf7, 0x00000001
	.section .rom.005028cb, "ax"
	.incbin "baserom.gba", 0x005028cb, 0x00000001
	.section .rom.005029c9, "ax"
	.incbin "baserom.gba", 0x005029c9, 0x00000003
	.section .rom.00503dae, "ax"
	.incbin "baserom.gba", 0x00503dae, 0x00000002
	.section .rom.00504f31, "ax"
	.incbin "baserom.gba", 0x00504f31, 0x00000003
	.section .rom.005058ba, "ax"
	.incbin "baserom.gba", 0x005058ba, 0x00000002
	.section .rom.00508537, "ax"
	.incbin "baserom.gba", 0x00508537, 0x00000001
	.section .rom.0050861a, "ax"
	.incbin "baserom.gba", 0x0050861a, 0x00000002
	.section .rom.00509805, "ax"
	.incbin "baserom.gba", 0x00509805, 0x00000003
	.section .rom.0050b48d, "ax"
	.incbin "baserom.gba", 0x0050b48d, 0x00000003
	.section .rom.0050b99b, "ax"
	.incbin "baserom.gba", 0x0050b99b, 0x00000001
	.section .rom.0050badb, "ax"
	.incbin "baserom.gba", 0x0050badb, 0x00000001
	.section .rom.0050ccf6, "ax"
	.incbin "baserom.gba", 0x0050ccf6, 0x00000002
	.section .rom.0050ce51, "ax"
	.incbin "baserom.gba", 0x0050ce51, 0x00000003
	.section .rom.00511277, "ax"
	.incbin "baserom.gba", 0x00511277, 0x00000001
	.section .rom.00512fa6, "ax"
	.incbin "baserom.gba", 0x00512fa6, 0x00000002
	.section .rom.00513d1f, "ax"
	.incbin "baserom.gba", 0x00513d1f, 0x00000001
	.section .rom.00513e53, "ax"
	.incbin "baserom.gba", 0x00513e53, 0x00000001
	.section .rom.005162d6, "ax"
	.incbin "baserom.gba", 0x005162d6, 0x00000002
	.section .rom.00517053, "ax"
	.incbin "baserom.gba", 0x00517053, 0x00000001
	.section .rom.00517b32, "ax"
	.incbin "baserom.gba", 0x00517b32, 0x00000002
	.section .rom.005189aa, "ax"
	.incbin "baserom.gba", 0x005189aa, 0x00000002
	.section .rom.0051aa7e, "ax"
	.incbin "baserom.gba", 0x0051aa7e, 0x00000002
	.section .rom.0051b3c5, "ax"
	.incbin "baserom.gba", 0x0051b3c5, 0x00000003
	.section .rom.0051c012, "ax"
	.incbin "baserom.gba", 0x0051c012, 0x00000002
	.section .rom.0051c239, "ax"
	.incbin "baserom.gba", 0x0051c239, 0x00000003
	.section .rom.0051db7b, "ax"
	.incbin "baserom.gba", 0x0051db7b, 0x00000001
	.section .rom.0051dc8f, "ax"
	.incbin "baserom.gba", 0x0051dc8f, 0x00000001
	.section .rom.0051f655, "ax"
	.incbin "baserom.gba", 0x0051f655, 0x00000003
	.section .rom.0051f79f, "ax"
	.incbin "baserom.gba", 0x0051f79f, 0x00000001
	.section .rom.0052186b, "ax"
	.incbin "baserom.gba", 0x0052186b, 0x00000001
	.section .rom.00521993, "ax"
	.incbin "baserom.gba", 0x00521993, 0x00000001
	.section .rom.0052321b, "ax"
	.incbin "baserom.gba", 0x0052321b, 0x00000001
	.section .rom.005254bd, "ax"
	.incbin "baserom.gba", 0x005254bd, 0x00000003
	.section .rom.005255ff, "ax"
	.incbin "baserom.gba", 0x005255ff, 0x00000001
	.section .rom.00527607, "ax"
	.incbin "baserom.gba", 0x00527607, 0x00000001
	.section .rom.00527712, "ax"
	.incbin "baserom.gba", 0x00527712, 0x00000002
	.section .rom.005296f6, "ax"
	.incbin "baserom.gba", 0x005296f6, 0x00000002
	.section .rom.0052ae1b, "ax"
	.incbin "baserom.gba", 0x0052ae1b, 0x00000001
	.section .rom.0052becf, "ax"
	.incbin "baserom.gba", 0x0052becf, 0x00000001
	.section .rom.0052dbb6, "ax"
	.incbin "baserom.gba", 0x0052dbb6, 0x00000002
	.section .rom.0052dd11, "ax"
	.incbin "baserom.gba", 0x0052dd11, 0x00000003
	.section .rom.0052fc5a, "ax"
	.incbin "baserom.gba", 0x0052fc5a, 0x00000002
	.section .rom.0052fddf, "ax"
	.incbin "baserom.gba", 0x0052fddf, 0x00000001
	.section .rom.00532931, "ax"
	.incbin "baserom.gba", 0x00532931, 0x00000003
	.section .rom.00534ff6, "ax"
	.incbin "baserom.gba", 0x00534ff6, 0x00000002
	.section .rom.0053774e, "ax"
	.incbin "baserom.gba", 0x0053774e, 0x00000002
	.section .rom.00538fa1, "ax"
	.incbin "baserom.gba", 0x00538fa1, 0x00000003
	.section .rom.0053b6a1, "ax"
	.incbin "baserom.gba", 0x0053b6a1, 0x00000003
	.section .rom.0053d46d, "ax"
	.incbin "baserom.gba", 0x0053d46d, 0x00000003
	.section .rom.0053f9b7, "ax"
	.incbin "baserom.gba", 0x0053f9b7, 0x00000001
	.section .rom.00540c82, "ax"
	.incbin "baserom.gba", 0x00540c82, 0x00000002
	.section .rom.00541933, "ax"
	.incbin "baserom.gba", 0x00541933, 0x00000001
	.section .rom.0054239d, "ax"
	.incbin "baserom.gba", 0x0054239d, 0x00000003
	.section .rom.00542493, "ax"
	.incbin "baserom.gba", 0x00542493, 0x00000001
	.section .rom.00543ef2, "ax"
	.incbin "baserom.gba", 0x00543ef2, 0x00000002
	.section .rom.00544c96, "ax"
	.incbin "baserom.gba", 0x00544c96, 0x00000002
	.section .rom.005467cf, "ax"
	.incbin "baserom.gba", 0x005467cf, 0x00000001
	.section .rom.005496bd, "ax"
	.incbin "baserom.gba", 0x005496bd, 0x00000003
	.section .rom.00550ea3, "ax"
	.incbin "baserom.gba", 0x00550ea3, 0x00000001
	.section .rom.00550f6f, "ax"
	.incbin "baserom.gba", 0x00550f6f, 0x00000001
	.section .rom.00556731, "ax"
	.incbin "baserom.gba", 0x00556731, 0x00000003
	.section .rom.00556873, "ax"
	.incbin "baserom.gba", 0x00556873, 0x00000001
	.section .rom.00557ca1, "ax"
	.incbin "baserom.gba", 0x00557ca1, 0x00000003
	.section .rom.0055a9de, "ax"
	.incbin "baserom.gba", 0x0055a9de, 0x00000002
	.section .rom.0055cada, "ax"
	.incbin "baserom.gba", 0x0055cada, 0x00000002
	.section .rom.005644e6, "ax"
	.incbin "baserom.gba", 0x005644e6, 0x00000002
	.section .rom.00564682, "ax"
	.incbin "baserom.gba", 0x00564682, 0x00000002
	.section .rom.00568f1d, "ax"
	.incbin "baserom.gba", 0x00568f1d, 0x00000003
	.section .rom.0056b41e, "ax"
	.incbin "baserom.gba", 0x0056b41e, 0x00000002
	.section .rom.00572e0f, "ax"
	.incbin "baserom.gba", 0x00572e0f, 0x00000001
	.section .rom.00572faa, "ax"
	.incbin "baserom.gba", 0x00572faa, 0x00000002
	.section .rom.00575b59, "ax"
	.incbin "baserom.gba", 0x00575b59, 0x00000003
	.section .rom.00577c2d, "ax"
	.incbin "baserom.gba", 0x00577c2d, 0x00000003
	.section .rom.00579c86, "ax"
	.incbin "baserom.gba", 0x00579c86, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x00579c88, 0x00001cec
	.section .rom.0057d11d, "ax"
	.incbin "baserom.gba", 0x0057d11d, 0x00000003
	.section .rom.0057fdf9, "ax"
	.incbin "baserom.gba", 0x0057fdf9, 0x00000003
	.section .rom.00581bcb, "ax"
	.incbin "baserom.gba", 0x00581bcb, 0x00000001
	.section .rom.00581d0b, "ax"
	.incbin "baserom.gba", 0x00581d0b, 0x00000001
	.section .rom.00584d65, "ax"
	.incbin "baserom.gba", 0x00584d65, 0x00000003
	.section .rom.00587ce2, "ax"
	.incbin "baserom.gba", 0x00587ce2, 0x00000002
	.section .rom.0058a5a9, "ax"
	.incbin "baserom.gba", 0x0058a5a9, 0x00000003
	.section .rom.00591511, "ax"
	.incbin "baserom.gba", 0x00591511, 0x00000003
	.section .rom.00592db7, "ax"
	.incbin "baserom.gba", 0x00592db7, 0x00000001
	.section .rom.005936b6, "ax"
	.incbin "baserom.gba", 0x005936b6, 0x00000002
	.section .rom.0059442f, "ax"
	.incbin "baserom.gba", 0x0059442f, 0x00000001
	.section .rom.0059458f, "ax"
	.incbin "baserom.gba", 0x0059458f, 0x00000001
	.section .rom.005961e7, "ax"
	.incbin "baserom.gba", 0x005961e7, 0x00000001
	.section .rom.00597a7f, "ax"
	.incbin "baserom.gba", 0x00597a7f, 0x00000001
	.section .rom.00597c03, "ax"
	.incbin "baserom.gba", 0x00597c03, 0x00000001
	.section .rom.0059a73f, "ax"
	.incbin "baserom.gba", 0x0059a73f, 0x00000001
	.section .rom.0059ceb9, "ax"
	.incbin "baserom.gba", 0x0059ceb9, 0x00000003
	.section .rom.0059dea6, "ax"
	.incbin "baserom.gba", 0x0059dea6, 0x00000002
	.section .rom.0059f567, "ax"
	.incbin "baserom.gba", 0x0059f567, 0x00000001
	.section .rom.0059f711, "ax"
	.incbin "baserom.gba", 0x0059f711, 0x00000003
	.section .rom.005a24a5, "ax"
	.incbin "baserom.gba", 0x005a24a5, 0x00000003
	.section .rom.005a653a, "ax"
	.incbin "baserom.gba", 0x005a653a, 0x00000002
	.section .rom.005a667b, "ax"
	.incbin "baserom.gba", 0x005a667b, 0x00000001
	.section .rom.005a8543, "ax"
	.incbin "baserom.gba", 0x005a8543, 0x00000001
	.section .rom.005a86e5, "ax"
	.incbin "baserom.gba", 0x005a86e5, 0x00000003
	.section .rom.005aa787, "ax"
	.incbin "baserom.gba", 0x005aa787, 0x00000001
	.section .rom.005ab714, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ab714, 0x000022d8
	.section .rom.005b04dd, "ax"
	.incbin "baserom.gba", 0x005b04dd, 0x00000003
	.section .rom.005b0662, "ax"
	.incbin "baserom.gba", 0x005b0662, 0x00000002
	.section .rom.005b7daf, "ax"
	.incbin "baserom.gba", 0x005b7daf, 0x00000001
	.section .rom.005b82b9, "ax"
	.incbin "baserom.gba", 0x005b82b9, 0x00000003
	.section .rom.005b9991, "ax"
	.incbin "baserom.gba", 0x005b9991, 0x00000003
	.section .rom.005b9b35, "ax"
	.incbin "baserom.gba", 0x005b9b35, 0x00000003
	.section .rom.005bb43a, "ax"
	.incbin "baserom.gba", 0x005bb43a, 0x00000002
	.section .rom.005bcc67, "ax"
	.incbin "baserom.gba", 0x005bcc67, 0x00000001
	.section .rom.005bcde5, "ax"
	.incbin "baserom.gba", 0x005bcde5, 0x00000003
	.section .rom.005c16d9, "ax"
	.incbin "baserom.gba", 0x005c16d9, 0x00000003
	.section .rom.005c271e, "ax"
	.incbin "baserom.gba", 0x005c271e, 0x00000002
	.section .rom.005c3542, "ax"
	.incbin "baserom.gba", 0x005c3542, 0x00000002
	.section .rom.005c4887, "ax"
	.incbin "baserom.gba", 0x005c4887, 0x00000001
	.section .rom.005c49e1, "ax"
	.incbin "baserom.gba", 0x005c49e1, 0x00000003
	.section .rom.005c6f9f, "ax"
	.incbin "baserom.gba", 0x005c6f9f, 0x00000001
	.section .rom.005c7116, "ax"
	.incbin "baserom.gba", 0x005c7116, 0x00000002
	.section .rom.005cba09, "ax"
	.incbin "baserom.gba", 0x005cba09, 0x00000003
	.section .rom.005cc471, "ax"
	.incbin "baserom.gba", 0x005cc471, 0x00000003
	.section .rom.005ceab3, "ax"
	.incbin "baserom.gba", 0x005ceab3, 0x00000001
	.section .rom.005cec1a, "ax"
	.incbin "baserom.gba", 0x005cec1a, 0x00000002
	.section .rom.005cfd61, "ax"
	.incbin "baserom.gba", 0x005cfd61, 0x00000003
	.section .rom.005d1abf, "ax"
	.incbin "baserom.gba", 0x005d1abf, 0x00000001
	.section .rom.005d1c26, "ax"
	.incbin "baserom.gba", 0x005d1c26, 0x00000002
	.section .rom.005d44b5, "ax"
	.incbin "baserom.gba", 0x005d44b5, 0x00000003
	.section .rom.005d45ea, "ax"
	.incbin "baserom.gba", 0x005d45ea, 0x00000002
	.section .rom.005d7226, "ax"
	.incbin "baserom.gba", 0x005d7226, 0x00000002
	.section .rom.005da9c5, "ax"
	.incbin "baserom.gba", 0x005da9c5, 0x00000003
	.section .rom.005ddd13, "ax"
	.incbin "baserom.gba", 0x005ddd13, 0x00000001
	.section .rom.005dde8f, "ax"
	.incbin "baserom.gba", 0x005dde8f, 0x00000001
	.section .rom.005e0a3e, "ax"
	.incbin "baserom.gba", 0x005e0a3e, 0x00000002
	.section .rom.005e2cb2, "ax"
	.incbin "baserom.gba", 0x005e2cb2, 0x00000002
	.section .rom.005e5523, "ax"
	.incbin "baserom.gba", 0x005e5523, 0x00000001
	.section .rom.005e8a17, "ax"
	.incbin "baserom.gba", 0x005e8a17, 0x00000001
	.section .rom.005ed63e, "ax"
	.incbin "baserom.gba", 0x005ed63e, 0x00000002
	.section .rom.005ee2c1, "ax"
	.incbin "baserom.gba", 0x005ee2c1, 0x00000003
	.section .rom.005ee403, "ax"
	.incbin "baserom.gba", 0x005ee403, 0x00000001
	.section .rom.005ef973, "ax"
	.incbin "baserom.gba", 0x005ef973, 0x00000001
	.section .rom.005efade, "ax"
	.incbin "baserom.gba", 0x005efade, 0x00000002
	.section .rom.005f1839, "ax"
	.incbin "baserom.gba", 0x005f1839, 0x00000003
	.section .rom.005f197b, "ax"
	.incbin "baserom.gba", 0x005f197b, 0x00000001
	.section .rom.005f417a, "ax"
	.incbin "baserom.gba", 0x005f417a, 0x00000002
	.section .rom.005f42bb, "ax"
	.incbin "baserom.gba", 0x005f42bb, 0x00000001
	.section .rom.005f5cd3, "ax"
	.incbin "baserom.gba", 0x005f5cd3, 0x00000001
	.section .rom.005f724e, "ax"
	.incbin "baserom.gba", 0x005f724e, 0x00000002
	.section .rom.005f878a, "ax"
	.incbin "baserom.gba", 0x005f878a, 0x00000002
	.section .rom.005f891a, "ax"
	.incbin "baserom.gba", 0x005f891a, 0x00000002
	.section .rom.005fabd3, "ax"
	.incbin "baserom.gba", 0x005fabd3, 0x00000001
	.section .rom.005fee1a, "ax"
	.incbin "baserom.gba", 0x005fee1a, 0x00000002
	.section .rom.00600c11, "ax"
	.incbin "baserom.gba", 0x00600c11, 0x00000003
	.section .rom.00603dbf, "ax"
	.incbin "baserom.gba", 0x00603dbf, 0x00000001
	.section .rom.006063f3, "ax"
	.incbin "baserom.gba", 0x006063f3, 0x00000001
	.section .rom.00608f4b, "ax"
	.incbin "baserom.gba", 0x00608f4b, 0x00000001
	.section .rom.00609113, "ax"
	.incbin "baserom.gba", 0x00609113, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609114, 0x00002904
	.section .rom.0060f51d, "ax"
	.incbin "baserom.gba", 0x0060f51d, 0x00000003
	.section .rom.00610a8e, "ax"
	.incbin "baserom.gba", 0x00610a8e, 0x00000002
	.section .rom.006132b5, "ax"
	.incbin "baserom.gba", 0x006132b5, 0x00000003
	.section .rom.00613425, "ax"
	.incbin "baserom.gba", 0x00613425, 0x00000003
	.section .rom.00615ce7, "ax"
	.incbin "baserom.gba", 0x00615ce7, 0x00000001
	.section .rom.006184af, "ax"
	.incbin "baserom.gba", 0x006184af, 0x00000001
	.section .rom.006197a9, "ax"
	.incbin "baserom.gba", 0x006197a9, 0x00000003
	.section .rom.0061b0ef, "ax"
	.incbin "baserom.gba", 0x0061b0ef, 0x00000001
	.section .rom.00626582, "ax"
	.incbin "baserom.gba", 0x00626582, 0x00000002
	.section .rom.00626742, "ax"
	.incbin "baserom.gba", 0x00626742, 0x00000002
	.section .rom.00629aa9, "ax"
	.incbin "baserom.gba", 0x00629aa9, 0x00000003
	.section .rom.0062bf9a, "ax"
	.incbin "baserom.gba", 0x0062bf9a, 0x00000002
	.section .rom.0062ed7b, "ax"
	.incbin "baserom.gba", 0x0062ed7b, 0x00000001
	.section .rom.0062f915, "ax"
	.incbin "baserom.gba", 0x0062f915, 0x00000003
	.section .rom.00630776, "ax"
	.incbin "baserom.gba", 0x00630776, 0x00000002
	.section .rom.006308ce, "ax"
	.incbin "baserom.gba", 0x006308ce, 0x00000002
	.section .rom.0063871b, "ax"
	.incbin "baserom.gba", 0x0063871b, 0x00000001
	.section .rom.00638887, "ax"
	.incbin "baserom.gba", 0x00638887, 0x00000001
	.section .rom.0063b623, "ax"
	.incbin "baserom.gba", 0x0063b623, 0x00000001
	.section .rom.0063d7b2, "ax"
	.incbin "baserom.gba", 0x0063d7b2, 0x00000002
	.section .rom.0063deba, "ax"
	.incbin "baserom.gba", 0x0063deba, 0x00000002
	.section .rom.0063eace, "ax"
	.incbin "baserom.gba", 0x0063eace, 0x00000002
	.section .rom.0064fba1, "ax"
	.incbin "baserom.gba", 0x0064fba1, 0x00000003
	.section .rom.0064fd36, "ax"
	.incbin "baserom.gba", 0x0064fd36, 0x00000002
	.section .rom.00651246, "ax"
	.incbin "baserom.gba", 0x00651246, 0x00000002
	.section .rom.0065302b, "ax"
	.incbin "baserom.gba", 0x0065302b, 0x00000001
	.section .rom.006540be, "ax"
	.incbin "baserom.gba", 0x006540be, 0x00000002
	.section .rom.0065632e, "ax"
	.incbin "baserom.gba", 0x0065632e, 0x00000002
	.section .rom.006564c5, "ax"
	.incbin "baserom.gba", 0x006564c5, 0x00000003
	.section .rom.0065846b, "ax"
	.incbin "baserom.gba", 0x0065846b, 0x00000001
	.section .rom.0065d242, "ax"
	.incbin "baserom.gba", 0x0065d242, 0x00000002
	.section .rom.00660ca3, "ax"
	.incbin "baserom.gba", 0x00660ca3, 0x00000001
	.section .rom.006639d9, "ax"
	.incbin "baserom.gba", 0x006639d9, 0x00000003
	.section .rom.00665d21, "ax"
	.incbin "baserom.gba", 0x00665d21, 0x00000003
	.section .rom.006674ff, "ax"
	.incbin "baserom.gba", 0x006674ff, 0x00000001
	.section .rom.006676b1, "ax"
	.incbin "baserom.gba", 0x006676b1, 0x00000003
	.section .rom.00678052, "ax"
	.incbin "baserom.gba", 0x00678052, 0x00000002
	.section .rom.006781b1, "ax"
	.incbin "baserom.gba", 0x006781b1, 0x00000003
	.section .rom.0067b90b, "ax"
	.incbin "baserom.gba", 0x0067b90b, 0x00000001
	.section .rom.0067ba6e, "ax"
	.incbin "baserom.gba", 0x0067ba6e, 0x00000002
	.section .rom.00681751, "ax"
	.incbin "baserom.gba", 0x00681751, 0x00000003
	.section .rom.00684137, "ax"
	.incbin "baserom.gba", 0x00684137, 0x00000001
	.section .rom.00686342, "ax"
	.incbin "baserom.gba", 0x00686342, 0x00000002
	.section .rom.00686483, "ax"
	.incbin "baserom.gba", 0x00686483, 0x00000001
	.section .rom.00687e9a, "ax"
	.incbin "baserom.gba", 0x00687e9a, 0x00000002
	.section .rom.0068ec01, "ax"
	.incbin "baserom.gba", 0x0068ec01, 0x00000003
	.section .rom.0068ed7f, "ax"
	.incbin "baserom.gba", 0x0068ed7f, 0x00000001
	.section .rom.00690d29, "ax"
	.incbin "baserom.gba", 0x00690d29, 0x00000003
	.section .rom.006934a2, "ax"
	.incbin "baserom.gba", 0x006934a2, 0x00000002
	.section .rom.00694d09, "ax"
	.incbin "baserom.gba", 0x00694d09, 0x00000003
	.section .rom.0069872a, "ax"
	.incbin "baserom.gba", 0x0069872a, 0x00000002
	.section .rom.006988f3, "ax"
	.incbin "baserom.gba", 0x006988f3, 0x00000001
	.section .rom.006a38db, "ax"
	.incbin "baserom.gba", 0x006a38db, 0x00000001
	.section .rom.006a5a27, "ax"
	.incbin "baserom.gba", 0x006a5a27, 0x00000001
	.section .rom.006a5bb1, "ax"
	.incbin "baserom.gba", 0x006a5bb1, 0x00000003
	.section .rom.006a7636, "ax"
	.incbin "baserom.gba", 0x006a7636, 0x00000002
	.section .rom.006aa66d, "ax"
	.incbin "baserom.gba", 0x006aa66d, 0x00000003
	.section .rom.006aa833, "ax"
	.incbin "baserom.gba", 0x006aa833, 0x00000001
	.section .rom.006ab767, "ax"
	.incbin "baserom.gba", 0x006ab767, 0x00000001
	.section .rom.006ad81a, "ax"
	.incbin "baserom.gba", 0x006ad81a, 0x00000002
	.section .rom.006ad9c1, "ax"
	.incbin "baserom.gba", 0x006ad9c1, 0x00000003
	.section .rom.006b29eb, "ax"
	.incbin "baserom.gba", 0x006b29eb, 0x00000001
	.section .rom.006b577b, "ax"
	.incbin "baserom.gba", 0x006b577b, 0x00000001
	.section .rom.006b593b, "ax"
	.incbin "baserom.gba", 0x006b593b, 0x00000001
	.section .rom.006b7e2f, "ax"
	.incbin "baserom.gba", 0x006b7e2f, 0x00000001
	.section .rom.006b8001, "ax"
	.incbin "baserom.gba", 0x006b8001, 0x00000003
	.section .rom.006ba092, "ax"
	.incbin "baserom.gba", 0x006ba092, 0x00000002
	.section .rom.006bc192, "ax"
	.incbin "baserom.gba", 0x006bc192, 0x00000002
	.section .rom.006bed46, "ax"
	.incbin "baserom.gba", 0x006bed46, 0x00000002
	.section .rom.006bf833, "ax"
	.incbin "baserom.gba", 0x006bf833, 0x00000001
	.section .rom.006bfa06, "ax"
	.incbin "baserom.gba", 0x006bfa06, 0x00000002
	.section .rom.006c2335, "ax"
	.incbin "baserom.gba", 0x006c2335, 0x00000003
	.section .rom.006c3acd, "ax"
	.incbin "baserom.gba", 0x006c3acd, 0x00000003
	.section .rom.006c4d41, "ax"
	.incbin "baserom.gba", 0x006c4d41, 0x00000003
	.section .rom.006c71d1, "ax"
	.incbin "baserom.gba", 0x006c71d1, 0x00000003
	.section .rom.006c951d, "ax"
	.incbin "baserom.gba", 0x006c951d, 0x00000003
	.section .rom.006ca707, "ax"
	.incbin "baserom.gba", 0x006ca707, 0x00000001
	.section .rom.006ca88e, "ax"
	.incbin "baserom.gba", 0x006ca88e, 0x00000002
	.section .rom.006cedda, "ax"
	.incbin "baserom.gba", 0x006cedda, 0x00000002
	.section .rom.006cef2f, "ax"
	.incbin "baserom.gba", 0x006cef2f, 0x00000001
	.section .rom.006cf06f, "ax"
	.incbin "baserom.gba", 0x006cf06f, 0x00000001
	.section .rom.006cfd43, "ax"
	.incbin "baserom.gba", 0x006cfd43, 0x00000001
	.section .rom.006cfec2, "ax"
	.incbin "baserom.gba", 0x006cfec2, 0x00000002
	.section .rom.006d177b, "ax"
	.incbin "baserom.gba", 0x006d177b, 0x00000001
	.section .rom.006d18fd, "ax"
	.incbin "baserom.gba", 0x006d18fd, 0x00000003
	.section .rom.006d338f, "ax"
	.incbin "baserom.gba", 0x006d338f, 0x00000001
	.section .rom.006d3509, "ax"
	.incbin "baserom.gba", 0x006d3509, 0x00000003
	.section .rom.006d5247, "ax"
	.incbin "baserom.gba", 0x006d5247, 0x00000001
	.section .rom.006d5373, "ax"
	.incbin "baserom.gba", 0x006d5373, 0x00000001
	.section .rom.006d54e9, "ax"
	.incbin "baserom.gba", 0x006d54e9, 0x00000003
	.section .rom.006d738f, "ax"
	.incbin "baserom.gba", 0x006d738f, 0x00000001
	.section .rom.006d7515, "ax"
	.incbin "baserom.gba", 0x006d7515, 0x00000003
	.section .rom.006d8473, "ax"
	.incbin "baserom.gba", 0x006d8473, 0x00000001
	.section .rom.006d9723, "ax"
	.incbin "baserom.gba", 0x006d9723, 0x00000001
	.section .rom.006d98de, "ax"
	.incbin "baserom.gba", 0x006d98de, 0x00000002
	.section .rom.006dc382, "ax"
	.incbin "baserom.gba", 0x006dc382, 0x00000002
	.section .rom.006dc4f5, "ax"
	.incbin "baserom.gba", 0x006dc4f5, 0x00000003
	.section .rom.006dd8dd, "ax"
	.incbin "baserom.gba", 0x006dd8dd, 0x00000003
	.section .rom.006e03bd, "ax"
	.incbin "baserom.gba", 0x006e03bd, 0x00000003
	.section .rom.006e0567, "ax"
	.incbin "baserom.gba", 0x006e0567, 0x00000001
	.section .rom.006e2962, "ax"
	.incbin "baserom.gba", 0x006e2962, 0x00000002
	.section .rom.006e2af1, "ax"
	.incbin "baserom.gba", 0x006e2af1, 0x00000003
	.section .rom.006e5d71, "ax"
	.incbin "baserom.gba", 0x006e5d71, 0x00000003
	.section .rom.006e5f52, "ax"
	.incbin "baserom.gba", 0x006e5f52, 0x00000002
	.section .rom.006e84e7, "ax"
	.incbin "baserom.gba", 0x006e84e7, 0x00000001
	.section .rom.006e8676, "ax"
	.incbin "baserom.gba", 0x006e8676, 0x00000002
	.section .rom.006eb26e, "ax"
	.incbin "baserom.gba", 0x006eb26e, 0x00000002
	.section .rom.006ebdbb, "ax"
	.incbin "baserom.gba", 0x006ebdbb, 0x00000001
	.section .rom.006ebf22, "ax"
	.incbin "baserom.gba", 0x006ebf22, 0x00000002
	.section .rom.006ee937, "ax"
	.incbin "baserom.gba", 0x006ee937, 0x00000001
	.section .rom.006f0576, "ax"
	.incbin "baserom.gba", 0x006f0576, 0x00000002
	.section .rom.006f0a33, "ax"
	.incbin "baserom.gba", 0x006f0a33, 0x00000001
	.section .rom.006f1dab, "ax"
	.incbin "baserom.gba", 0x006f1dab, 0x00000001
	.section .rom.006f32ef, "ax"
	.incbin "baserom.gba", 0x006f32ef, 0x00000001
	.section .rom.006f3479, "ax"
	.incbin "baserom.gba", 0x006f3479, 0x00000003
	.section .rom.006f4557, "ax"
	.incbin "baserom.gba", 0x006f4557, 0x00000001
	.section .rom.006f46e5, "ax"
	.incbin "baserom.gba", 0x006f46e5, 0x00000003
	.section .rom.006f6339, "ax"
	.incbin "baserom.gba", 0x006f6339, 0x00000003
	.section .rom.006f8d1b, "ax"
	.incbin "baserom.gba", 0x006f8d1b, 0x00000001
	.section .rom.006fc245, "ax"
	.incbin "baserom.gba", 0x006fc245, 0x00000003
	.section .rom.006fdb4f, "ax"
	.incbin "baserom.gba", 0x006fdb4f, 0x00000001
	.section .rom.006ffbf7, "ax"
	.incbin "baserom.gba", 0x006ffbf7, 0x00000001
	.section .rom.0070264b, "ax"
	.incbin "baserom.gba", 0x0070264b, 0x00000001
	.section .rom.00704297, "ax"
	.incbin "baserom.gba", 0x00704297, 0x00000001
	.section .rom.007056dd, "ax"
	.incbin "baserom.gba", 0x007056dd, 0x00000003
	.section .rom.0070755d, "ax"
	.incbin "baserom.gba", 0x0070755d, 0x00000003
	.section .rom.0070912e, "ax"
	.incbin "baserom.gba", 0x0070912e, 0x00000002
	.section .rom.0070b862, "ax"
	.incbin "baserom.gba", 0x0070b862, 0x00000002
	.section .rom.0070d4a7, "ax"
	.incbin "baserom.gba", 0x0070d4a7, 0x00000001
	.section .rom.0070e8ed, "ax"
	.incbin "baserom.gba", 0x0070e8ed, 0x00000003
	.section .rom.0071046b, "ax"
	.incbin "baserom.gba", 0x0071046b, 0x00000001
	.section .rom.0071169b, "ax"
	.incbin "baserom.gba", 0x0071169b, 0x00000001
	.section .rom.007117f5, "ax"
	.incbin "baserom.gba", 0x007117f5, 0x00000003
	.section .rom.00714cfb, "ax"
	.incbin "baserom.gba", 0x00714cfb, 0x00000001
	.section .rom.00714e66, "ax"
	.incbin "baserom.gba", 0x00714e66, 0x00000002
	.section .rom.007177b6, "ax"
	.incbin "baserom.gba", 0x007177b6, 0x00000002
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007177b8, 0x00001164
	.section .rom.0071a101, "ax"
	.incbin "baserom.gba", 0x0071a101, 0x00000003
	.section .rom.0071d347, "ax"
	.incbin "baserom.gba", 0x0071d347, 0x00000001
	.section .rom.0071fc3e, "ax"
	.incbin "baserom.gba", 0x0071fc3e, 0x00000002
	.section .rom.00728f77, "ax"
	.incbin "baserom.gba", 0x00728f77, 0x00000001
	.section .rom.0072a2a6, "ax"
	.incbin "baserom.gba", 0x0072a2a6, 0x00000002
	.section .rom.0072b49f, "ax"
	.incbin "baserom.gba", 0x0072b49f, 0x00000001
	.section .rom.0072b601, "ax"
	.incbin "baserom.gba", 0x0072b601, 0x00000003
	.section .rom.0072dfcd, "ax"
	.incbin "baserom.gba", 0x0072dfcd, 0x00000003
	.section .rom.007314f9, "ax"
	.incbin "baserom.gba", 0x007314f9, 0x00000003
	.section .rom.0073593b, "ax"
	.incbin "baserom.gba", 0x0073593b, 0x00000001
	.section .rom.00735aae, "ax"
	.incbin "baserom.gba", 0x00735aae, 0x00000002
	.section .rom.00739d13, "ax"
	.incbin "baserom.gba", 0x00739d13, 0x00000001
	.section .rom.0073d52e, "ax"
	.incbin "baserom.gba", 0x0073d52e, 0x00000002
	.section .rom.0073ef56, "ax"
	.incbin "baserom.gba", 0x0073ef56, 0x00000002
	.section .rom.00741603, "ax"
	.incbin "baserom.gba", 0x00741603, 0x00000001
	.section .rom.00743667, "ax"
	.incbin "baserom.gba", 0x00743667, 0x00000001
	.section .rom.00745523, "ax"
	.incbin "baserom.gba", 0x00745523, 0x00000001
	.section .rom.0074876d, "ax"
	.incbin "baserom.gba", 0x0074876d, 0x00000003
	.section .rom.0074a133, "ax"
	.incbin "baserom.gba", 0x0074a133, 0x00000001
	.section .rom.0074a2a5, "ax"
	.incbin "baserom.gba", 0x0074a2a5, 0x00000003
	.section .rom.0074d2b2, "ax"
	.incbin "baserom.gba", 0x0074d2b2, 0x00000002
	.section .rom.0074d46a, "ax"
	.incbin "baserom.gba", 0x0074d46a, 0x00000002
	.section .rom.0074ecdb, "ax"
	.incbin "baserom.gba", 0x0074ecdb, 0x00000001
	.section .rom.0074ee5a, "ax"
	.incbin "baserom.gba", 0x0074ee5a, 0x00000002
	.section .rom.00752663, "ax"
	.incbin "baserom.gba", 0x00752663, 0x00000001
	.section .rom.007527d7, "ax"
	.incbin "baserom.gba", 0x007527d7, 0x00000001
	.section .rom.0075359f, "ax"
	.incbin "baserom.gba", 0x0075359f, 0x00000001
	.section .rom.00753745, "ax"
	.incbin "baserom.gba", 0x00753745, 0x00000003
	.section .rom.007557a0, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x007557a0, 0x000000e4
	.section .rom.0075859b, "ax"
	.incbin "baserom.gba", 0x0075859b, 0x00000001
	.section .rom.00758782, "ax"
	.incbin "baserom.gba", 0x00758782, 0x00000002
	.section .rom.0075ba1e, "ax"
	.incbin "baserom.gba", 0x0075ba1e, 0x00000002
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075ba20, 0x00004600
	.section .rom.00760126, "ax"
	.incbin "baserom.gba", 0x00760126, 0x00000002
	.section .rom.00761cf9, "ax"
	.incbin "baserom.gba", 0x00761cf9, 0x00000003
	.section .rom.007637a9, "ax"
	.incbin "baserom.gba", 0x007637a9, 0x00000003
	.section .rom.00763905, "ax"
	.incbin "baserom.gba", 0x00763905, 0x00000003
	.section .rom.00769b4b, "ax"
	.incbin "baserom.gba", 0x00769b4b, 0x00000001
	.section .rom.00769c4f, "ax"
	.incbin "baserom.gba", 0x00769c4f, 0x00000001
	.section .rom.0076a66b, "ax"
	.incbin "baserom.gba", 0x0076a66b, 0x00000001
	.section .rom.0076a751, "ax"
	.incbin "baserom.gba", 0x0076a751, 0x00000003
	.section .rom.0076a9ed, "ax"
	.incbin "baserom.gba", 0x0076a9ed, 0x00000003
	.section .rom.0076ea4a, "ax"
	.incbin "baserom.gba", 0x0076ea4a, 0x00000002
	.section .rom.0076effe, "ax"
	.incbin "baserom.gba", 0x0076effe, 0x00000002
	.section .rom.0076ff93, "ax"
	.incbin "baserom.gba", 0x0076ff93, 0x00000001
	.section .rom.00771996, "ax"
	.incbin "baserom.gba", 0x00771996, 0x00000002
	.section .rom.00772605, "ax"
	.incbin "baserom.gba", 0x00772605, 0x00000003
	.section .rom.007728ad, "ax"
	.incbin "baserom.gba", 0x007728ad, 0x00000003
	.section .rom.00773732, "ax"
	.incbin "baserom.gba", 0x00773732, 0x00000002
	.section .rom.007748c3, "ax"
	.incbin "baserom.gba", 0x007748c3, 0x00000001
	.section .rom.00774a8b, "ax"
	.incbin "baserom.gba", 0x00774a8b, 0x00000001
	.section .rom.00774dd7, "ax"
	.incbin "baserom.gba", 0x00774dd7, 0x00000001
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x00774dd8, 0x0000000c
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x00774de4, 0x00000150
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x00774f34, 0x00000140
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x00775074, 0x00000140
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x007751b4, 0x00000140
	.section .rom.0077563f, "ax"
	.incbin "baserom.gba", 0x0077563f, 0x00000001
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x00775640, 0x0000000c
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x0077564c, 0x00000150
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x0077579c, 0x00000140
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x007758dc, 0x00000140
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x00775a1c, 0x00000140
	.section .rom.00775ea7, "ax"
	.incbin "baserom.gba", 0x00775ea7, 0x00000001
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x00775ea8, 0x0000000c
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x00775eb4, 0x00000150
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x00776004, 0x00000140
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x00776144, 0x00000140
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x00776284, 0x00000140
	.section .rom.0077670f, "ax"
	.incbin "baserom.gba", 0x0077670f, 0x00000001
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x00776710, 0x0000000c
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x0077671c, 0x00000150
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x0077686c, 0x00000140
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x007769ac, 0x00000140
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x00776aec, 0x00000140
	.section .rom.00776f77, "ax"
	.incbin "baserom.gba", 0x00776f77, 0x00000001
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x00776f78, 0x0000000c
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x00776f84, 0x00000150
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x007770d4, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x00777214, 0x00000140
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x00777354, 0x00000140
	.section .rom.007777df, "ax"
	.incbin "baserom.gba", 0x007777df, 0x00000001
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x007777e0, 0x0000000c
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x007777ec, 0x00000150
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x0077793c, 0x00000140
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x00777a7c, 0x00000140
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x00777bbc, 0x00000140
	.section .rom.00778047, "ax"
	.incbin "baserom.gba", 0x00778047, 0x00000001
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00778048, 0x0000000c
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x00778054, 0x00000150
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x007781a4, 0x00000140
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x007782e4, 0x00000140
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x00778424, 0x00000140
	.section .rom.007788af, "ax"
	.incbin "baserom.gba", 0x007788af, 0x00000001
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x007788b0, 0x0000000c
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x007788bc, 0x00000150
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x00778a0c, 0x00000140
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x00778b4c, 0x00000140
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x00778c8c, 0x00000140
