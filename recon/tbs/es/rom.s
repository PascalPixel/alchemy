@ tbs-es's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Rom_Start
Rom_Start:
	.incbin "baserom.gba", 0x00000000, 0x000000af
	.global Rom_LanguageCode
Rom_LanguageCode:
	.incbin "baserom.gba", 0x000000af, 0x00000011
	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00005044, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00005044, 0x000001f4
	.section .rom.000061bc, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x000061bc, 0x000000e4
	.section .rom.000065bc, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x000065bc, 0x0000023c
	.section .rom.000068ce, "ax"
	.incbin "baserom.gba", 0x000068ce, 0x00000002
	.section .rom.00007380, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x00007380, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x000076d6, 0x00000106
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "baserom.gba", 0x000077dc, 0x00000020
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x000077fc, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x000079bc, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x000079d0, 0x00000024
	.section .rom.00007a10, "ax"
	.global Save_Signature
Save_Signature:
	.incbin "baserom.gba", 0x00007a10, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x00007a18, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a6c, 0x00000014
	.section .rom.00007ac8, "ax"
	.incbin "baserom.gba", 0x00007ac8, 0x00000024
	.section .rom.00007b04, "ax"
	.incbin "baserom.gba", 0x00007b04, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007b1c, 0x00000058
	.section .rom.00007b98, "ax"
	.incbin "baserom.gba", 0x00007b98, 0x0000008c
	.section .rom.00007c2c, "ax"
	.incbin "baserom.gba", 0x00007c2c, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007c44, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c70, 0x0000002c
	.section .rom.00007cc4, "ax"
	.incbin "baserom.gba", 0x00007cc4, 0x00000b3c
	.section .rom.00008ab8, "ax"
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "baserom.gba", 0x00008ab8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x00008fb8, 0x00000400
	.section .rom.000098f8, "ax"
	.global Transform_UpdateVertices
Transform_UpdateVertices:
	.incbin "baserom.gba", 0x000098f8, 0x00000284
	.global Transform_UpdateVerticesEnd
Transform_UpdateVerticesEnd:
	.section .rom.00009c94, "ax"
	.global Object_UpdateAll
Object_UpdateAll:
	.incbin "baserom.gba", 0x00009c94, 0x000004e8
	.global Object_UpdateAllEnd
Object_UpdateAllEnd:
	.section .rom.0000a20c, "ax"
	.global Func_0800aa0c
	.type Func_0800aa0c, %function
	.thumb_func
Func_0800aa0c:
	.incbin "baserom.gba", 0x0000a20c, 0x00000668
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.0000c2cc, "ax"
	.global Object_UpdateAllThumb
	.type Object_UpdateAllThumb, %function
	.thumb_func
Object_UpdateAllThumb:
	.incbin "baserom.gba", 0x0000c2cc, 0x00000664
	.section .rom.0000d2f0, "ax"
	.incbin "baserom.gba", 0x0000d2f0, 0x000001ec
	.section .rom.0000e3ec, "ax"
	.incbin "baserom.gba", 0x0000e3ec, 0x0000070c
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
	.section .rom.000113f4, "ax"
	.global Func_08011bf4
	.type Func_08011bf4, %function
	.thumb_func
Func_08011bf4:
	.incbin "baserom.gba", 0x000113f4, 0x000000ec
	.section .rom.00011752, "ax"
	.incbin "baserom.gba", 0x00011752, 0x00000002
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
	.incbin "baserom.gba", 0x00012720, 0x00000080
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x000127a0, 0x000001ac
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001294c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00012990, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00012a40, 0x00000014
	.global Data_08013254
Data_08013254:
	.incbin "baserom.gba", 0x00012a54, 0x00000020
	.global Data_08013274
Data_08013274:
	.incbin "baserom.gba", 0x00012a74, 0x00000018
	.global Data_0801328c
Data_0801328c:
	.incbin "baserom.gba", 0x00012a8c, 0x00000040
	.global Data_080132cc
Data_080132cc:
	.incbin "baserom.gba", 0x00012acc, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x00012afc, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x00012bfc, 0x00000100
	.global Map_TerrainHeightFunctions
Map_TerrainHeightFunctions:
	.incbin "baserom.gba", 0x00012cfc, 0x00000040
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.incbin "baserom.gba", 0x00012d3c, 0x00000048
	.global Battle_FormationPlacementScale
Battle_FormationPlacementScale:
	.incbin "baserom.gba", 0x00012d84, 0x00000008
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.incbin "baserom.gba", 0x00012d8c, 0x00000004
	.global ObjectDispatch_Table0Script
ObjectDispatch_Table0Script:
	.incbin "baserom.gba", 0x00012d90, 0x00000018
	.global ObjectDispatch_Table1Script
ObjectDispatch_Table1Script:
	.incbin "baserom.gba", 0x00012da8, 0x00000018
	.global ObjectDispatch_Table2Script
ObjectDispatch_Table2Script:
	.incbin "baserom.gba", 0x00012dc0, 0x00000018
	.global ObjectDispatch_Table3Script
ObjectDispatch_Table3Script:
	.incbin "baserom.gba", 0x00012dd8, 0x00000018
	.global ObjectDispatch_Table4Script
ObjectDispatch_Table4Script:
	.incbin "baserom.gba", 0x00012df0, 0x00000018
	.global ObjectDispatch_Table5Script
ObjectDispatch_Table5Script:
	.incbin "baserom.gba", 0x00012e08, 0x00000018
	.global ObjectDispatch_Table6Script
ObjectDispatch_Table6Script:
	.incbin "baserom.gba", 0x00012e20, 0x000000c0
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00012ee0, 0x000000a4
	.global Map_LayeredScenes
Map_LayeredScenes:
	.incbin "baserom.gba", 0x00012f84, 0x0000107c
	.section .rom.000145d0, "ax"
	.incbin "baserom.gba", 0x000145d0, 0x000002f4
	.global Tile_BuildMetatiles
Tile_BuildMetatiles:
	.incbin "baserom.gba", 0x000148c4, 0x00000214
	.global Tile_BuildMetatilesEnd
Tile_BuildMetatilesEnd:
	.section .rom.000154ae, "ax"
	.incbin "baserom.gba", 0x000154ae, 0x00000002
	.section .rom.000158d0, "ax"
	.global UiWork_StepChannelScript
	.type UiWork_StepChannelScript, %function
	.thumb_func
UiWork_StepChannelScript:
	.incbin "baserom.gba", 0x000158d0, 0x0000062c
	.section .rom.00016fde, "ax"
	.incbin "baserom.gba", 0x00016fde, 0x00000002
	.section .rom.00016fe0, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00016fe0, 0x00000660
	.section .rom.000177d8, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x000177d8, 0x000001f8
	.section .rom.000179d0, "ax"
	.global UiText_MeasureStringVariant
	.type UiText_MeasureStringVariant, %function
	.thumb_func
UiText_MeasureStringVariant:
	.incbin "baserom.gba", 0x000179d0, 0x00000264
	.global Func_08018cac
Func_08018cac:
	.incbin "baserom.gba", 0x00017c34, 0x00000250
	.section .rom.00018154, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x00018154, 0x00000480
	.section .rom.00018bdc, "ax"
	.incbin "baserom.gba", 0x00018bdc, 0x00000110
	.section .rom.0001996c, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x0001996c, 0x00000560
	.section .rom.0001ce6c, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001ce6c, 0x00000410
	.section .rom.0001f260, "ax"
	.global SaveMenu_SelectSlot
	.type SaveMenu_SelectSlot, %function
	.thumb_func
SaveMenu_SelectSlot:
	.incbin "baserom.gba", 0x0001f260, 0x00000580
	.section .rom.0001fbf2, "ax"
	.incbin "baserom.gba", 0x0001fbf2, 0x00000002
	.section .rom.0001fbf4, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x0001fbf4, 0x000004b4
	.section .rom.00020ea6, "ax"
	.incbin "baserom.gba", 0x00020ea6, 0x000008fe
	.section .rom.00021ab6, "ax"
	.incbin "baserom.gba", 0x00021ab6, 0x000027b2
	.section .rom.000242e8, "ax"
	.incbin "baserom.gba", 0x000242e8, 0x00001d58
	.section .rom.000262d4, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x000262d4, 0x00001080
	.section .rom.00027354, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x00027354, 0x00000348
	.section .rom.00028714, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x00028714, 0x00000228
	.section .rom.0002893c, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x0002893c, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00028ad0, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x00028bd0, 0x000000bc
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.incbin "baserom.gba", 0x00028c8c, 0x0000009c
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.incbin "baserom.gba", 0x00028d28, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00028fc0, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x000290a4, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x000294a0, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002d048, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002d2c8, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002fa60, 0x00000804
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x00030264, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x000309a4, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00030a24, 0x00000690
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x000310b4, 0x00000400
	.section .rom.000330b4, "ax"
	.incbin "baserom.gba", 0x000330b4, 0x0000001c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x000330d0, 0x00000030
	.global UiText_IndefiniteArticles
UiText_IndefiniteArticles:
	.incbin "baserom.gba", 0x00033100, 0x000000c0
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x000331c0, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000335c0, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x000339c0, 0x00002000
	.global Data_080366f8
Data_080366f8:
	.incbin "baserom.gba", 0x000359c0, 0x00000058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00035a18, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x00035a91, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x00035a94, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x00035a96, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x00035a98, 0x00000006
	.global Data_080367d6
Data_080367d6:
	.incbin "baserom.gba", 0x00035a9e, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00035aa4, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00035aac, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00035ad4, 0x000009b8
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
	.incbin "baserom.gba", 0x0003648c, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000364a8, 0x00000028
	.global Data_080371fe
Data_080371fe:
	.incbin "baserom.gba", 0x000364d0, 0x00000008
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x000364d8, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x000364e8, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x000364f8, 0x00000008
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x00036500, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00036520, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00036550, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x00036590, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x000365d0, 0x000000ff
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.incbin "baserom.gba", 0x000366cf, 0x00000008
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000366d7, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000366e3, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x000366ef, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x00036708, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x0003670c, 0x00000038
	.section .rom.00073368, "ax"
	.incbin "baserom.gba", 0x00073368, 0x0000000a
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073372, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x000733b4, 0x00000120
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000734d4, 0x00008b2c
	.section .rom.0007cd38, "ax"
	.global GameState_InitDefaults
	.type GameState_InitDefaults, %function
	.thumb_func
GameState_InitDefaults:
	.incbin "baserom.gba", 0x0007cd38, 0x00000208
	.section .rom.0007dbf0, "ax"
	.global Owner_RefreshClassActions
	.type Owner_RefreshClassActions, %function
	.thumb_func
Owner_RefreshClassActions:
	.incbin "baserom.gba", 0x0007dbf0, 0x00000238
	.section .rom.0007eb24, "ax"
	.global Curve_LookupScaledValue
	.type Curve_LookupScaledValue, %function
	.thumb_func
Curve_LookupScaledValue:
	.incbin "baserom.gba", 0x0007eb24, 0x000000a0
	.section .rom.0007f664, "ax"
	.global Func_0807a664
	.type Func_0807a664, %function
	.thumb_func
Func_0807a664:
	.incbin "baserom.gba", 0x0007f664, 0x0000013c
	.section .rom.0007f828, "ax"
	.global Character_ElementGroupTable
Character_ElementGroupTable:
	.incbin "baserom.gba", 0x0007f828, 0x00000008
	.global Character_LevelExpTable
Character_LevelExpTable:
	.incbin "baserom.gba", 0x0007f830, 0x00000c60
	.global Item_ArtifactSlotTable
Item_ArtifactSlotTable:
	.incbin "baserom.gba", 0x00080490, 0x00000200
	.global Character_StartingEquipOwnerIds
Character_StartingEquipOwnerIds:
	.incbin "baserom.gba", 0x00080690, 0x00000018
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "baserom.gba", 0x000806a8, 0x000037b0
	.global BattleAction_DefinitionTable
BattleAction_DefinitionTable:
	.incbin "baserom.gba", 0x00083e58, 0x00002070
	.global Data_08080ec8
Data_08080ec8:
	.incbin "baserom.gba", 0x00085ec8, 0x00003624
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "baserom.gba", 0x000894ec, 0x000005a0
	.global Summon_OrderList
Summon_OrderList:
	.incbin "baserom.gba", 0x00089a8c, 0x00000010
	.global Summon_DefinitionTable
Summon_DefinitionTable:
	.incbin "baserom.gba", 0x00089a9c, 0x00000080
	.global Class_DefinitionTable
Class_DefinitionTable:
	.incbin "baserom.gba", 0x00089b1c, 0x0000429c
	.global Data_08088db8
Data_08088db8:
	.incbin "baserom.gba", 0x0008ddb8, 0x00000040
	.global Element_PowerResistByLevel
Element_PowerResistByLevel:
	.incbin "baserom.gba", 0x0008ddf8, 0x00000040
	.global Enemy_ElementPresetTable
Enemy_ElementPresetTable:
	.incbin "baserom.gba", 0x0008de38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008e26c, 0x00000d94
	.section .rom.00091500, "ax"
	.global Func_0808c4f8
	.type Func_0808c4f8, %function
	.thumb_func
Func_0808c4f8:
	.incbin "baserom.gba", 0x00091500, 0x0000097c
	.section .rom.000929ac, "ax"
	.incbin "baserom.gba", 0x000929ac, 0x00000414
	.section .rom.00094598, "ax"
	.global DisplayTransition_UpdateScanlineTable
	.type DisplayTransition_UpdateScanlineTable, %function
	.thumb_func
DisplayTransition_UpdateScanlineTable:
	.incbin "baserom.gba", 0x00094598, 0x0000090c
	.section .rom.00095ac8, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x00095ac8, 0x00000718
	.section .rom.000995d0, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x000995d0, 0x000001ec
	.section .rom.0009ccc8, "ax"
	.global FunctionHead_08097c3c
	.type FunctionHead_08097c3c, %function
	.thumb_func
FunctionHead_08097c3c:
	.incbin "baserom.gba", 0x0009ccc8, 0x00000344
	.section .rom.0009ea86, "ax"
	.incbin "baserom.gba", 0x0009ea86, 0x00000002
	.section .rom.0009ea88, "ax"
	.global RunBattleEffect05
	.type RunBattleEffect05, %function
	.thumb_func
RunBattleEffect05:
	.incbin "baserom.gba", 0x0009ea88, 0x00000328
	.section .rom.0009ee3c, "ax"
	.global Battle_unk3_2
	.type Battle_unk3_2, %function
	.thumb_func
Battle_unk3_2:
	.incbin "baserom.gba", 0x0009ee3c, 0x000004f0
	.section .rom.0009fefa, "ax"
	.incbin "baserom.gba", 0x0009fefa, 0x00000002
	.section .rom.0009fefc, "ax"
	.global RunBattleEffect13
	.type RunBattleEffect13, %function
	.thumb_func
RunBattleEffect13:
	.incbin "baserom.gba", 0x0009fefc, 0x0000024c
	.section .rom.000a0d90, "ax"
	.global Map_UpdateWorldMapMarkers
	.type Map_UpdateWorldMapMarkers, %function
	.thumb_func
Map_UpdateWorldMapMarkers:
	.incbin "baserom.gba", 0x000a0d90, 0x00000500
	.section .rom.000a1568, "ax"
	.global Data_0809c410
Data_0809c410:
	.incbin "baserom.gba", 0x000a1568, 0x00000100
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "baserom.gba", 0x000a1668, 0x00000100
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x000a1768, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a22c8, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x000a2900, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x000a2a08, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a2b48, 0x000003e8
	.global BattleFx_ConditionResources
BattleFx_ConditionResources:
	.incbin "baserom.gba", 0x000a2f30, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a3330, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a33c8, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x000a35e0, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000a3626, 0x000001b8
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x000a37de, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x000a3810, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x000a3818, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x000a38b4, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x000a39d4, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x000a39f8, 0x0000004e
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x000a3a46, 0x00000102
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000a3b48, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x000a3d54, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x000a3ed8, 0x00000204
	.global FieldFx_GroundParticleFrames
FieldFx_GroundParticleFrames:
	.incbin "baserom.gba", 0x000a40dc, 0x000000a0
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x000a417c, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x000a41fc, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x000a4208, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000a422c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000a4250, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x000a4274, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000a42b8, 0x00000048
	.section .rom.000a4948, "ax"
	.incbin "baserom.gba", 0x000a4948, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x000a4968, 0x000003bc
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x000a4d24, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x000a4d44, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x000a4d5c, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x000a4d74, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x000a4d80, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x000a4d84, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x000a4e90, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x000a4e9c, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x000a4f58, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x000a4f5c, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x000a4f68, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x000a5024, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x000a5070, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x000a5084, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x000a5098, 0x00000018
	.global FieldFx_GroundParticleTiles
FieldFx_GroundParticleTiles:
	.incbin "baserom.gba", 0x000a50b0, 0x000000c6
	.global FieldFx_MoteTiles
FieldFx_MoteTiles:
	.incbin "baserom.gba", 0x000a5176, 0x0000009a
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x000a5210, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a5260, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a5280, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a5284, 0x0000057c
	.section .rom.000a893e, "ax"
	.incbin "baserom.gba", 0x000a893e, 0x00000002
	.section .rom.000a8940, "ax"
	.global Func_080a414c
	.type Func_080a414c, %function
	.thumb_func
Func_080a414c:
	.incbin "baserom.gba", 0x000a8940, 0x00000340
	.section .rom.000aa530, "ax"
	.global Menu_ResolveSelectedAction
	.type Menu_ResolveSelectedAction, %function
	.thumb_func
Menu_ResolveSelectedAction:
	.incbin "baserom.gba", 0x000aa530, 0x00000320
	.section .rom.000aeff0, "ax"
	.global Func_080aa768
Func_080aa768:
	.incbin "baserom.gba", 0x000aeff0, 0x0000051c
	.section .rom.000afe6c, "ax"
	.incbin "baserom.gba", 0x000afe6c, 0x000012fc
	.section .rom.000b1c78, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000b1c78, 0x000000fc
	.section .rom.000b1f40, "ax"
	.incbin "baserom.gba", 0x000b1f40, 0x00001040
	.section .rom.000b32b8, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000b32b8, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000b33b8, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000b3438, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000b35b8, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000b3638, 0x000002c0
	.global Data_080af08c
Data_080af08c:
	.incbin "baserom.gba", 0x000b38f8, 0x00000180
	.global Data_080af20c
Data_080af20c:
	.incbin "baserom.gba", 0x000b3a78, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000b3a7c, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000b3a80, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000b3a84, 0x00000004
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000b3a88, 0x00000004
	.global Data_080af220
Data_080af220:
	.incbin "baserom.gba", 0x000b3a8c, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000b3a90, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000b3a94, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000b3a98, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000b3a9c, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000b3aa0, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000b3aa4, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000b3aa8, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000b3ad8, 0x00000020
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.incbin "baserom.gba", 0x000b3af8, 0x00000004
	.global DjinnMenu_TextSlash
DjinnMenu_TextSlash:
	.incbin "baserom.gba", 0x000b3afc, 0x00000004
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000b3b00, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000b3b09, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000b3b12, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000b3b1d, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000b3b28, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000b3b3c, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000b3b50, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000b3b68, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000b3b70, 0x00000490
	.section .rom.000b4aac, "ax"
	.global Shop_SelBuy
	.type Shop_SelBuy, %function
	.thumb_func
Shop_SelBuy:
	.incbin "baserom.gba", 0x000b4aac, 0x000004f8
	.section .rom.000b5260, "ax"
	.incbin "baserom.gba", 0x000b5260, 0x00000210
	.section .rom.000b7940, "ax"
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "baserom.gba", 0x000b7940, 0x00000080
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "baserom.gba", 0x000b79c0, 0x00000080
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "baserom.gba", 0x000b7a40, 0x00000080
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "baserom.gba", 0x000b7ac0, 0x00000080
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "baserom.gba", 0x000b7b40, 0x00000080
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "baserom.gba", 0x000b7bc0, 0x00000180
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000b7d40, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000b7e80, 0x00000100
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "baserom.gba", 0x000b7f80, 0x00000180
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000b8100, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000b813c, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000b8146, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000b81ac, 0x00000906
	.global Data_080b4ab2
Data_080b4ab2:
	.incbin "baserom.gba", 0x000b8ab2, 0x00000004
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.incbin "baserom.gba", 0x000b8ab6, 0x0000054a
	.section .rom.000b9204, "ax"
	.incbin "baserom.gba", 0x000b9204, 0x00000008
	.section .rom.000b9218, "ax"
	.incbin "baserom.gba", 0x000b9218, 0x00000040
	.section .rom.000b9534, "ax"
	.global DebugBattle_ViewMessages
	.type DebugBattle_ViewMessages, %function
	.thumb_func
DebugBattle_ViewMessages:
	.incbin "baserom.gba", 0x000b9534, 0x000001ac
	.section .rom.000ba3e0, "ax"
	.global Battle_RunEncounter
	.type Battle_RunEncounter, %function
	.thumb_func
Battle_RunEncounter:
	.incbin "baserom.gba", 0x000ba3e0, 0x00000698
	.section .rom.000bb750, "ax"
	.incbin "baserom.gba", 0x000bb750, 0x000001ac
	.section .rom.000bcc34, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000bcc34, 0x0000019c
	.section .rom.000bded8, "ax"
	.global BattlePresentation_RunUnitTransition
	.type BattlePresentation_RunUnitTransition, %function
	.thumb_func
BattlePresentation_RunUnitTransition:
	.incbin "baserom.gba", 0x000bded8, 0x000003bc
	.section .rom.000be6c4, "ax"
	.global Func_080ba6ac
	.type Func_080ba6ac, %function
	.thumb_func
Func_080ba6ac:
	.incbin "baserom.gba", 0x000be6c4, 0x0000026c
	.section .rom.000be98e, "ax"
	.incbin "baserom.gba", 0x000be98e, 0x00000002
	.global Func_080ba978
	.type Func_080ba978, %function
	.thumb_func
Func_080ba978:
	.incbin "baserom.gba", 0x000be990, 0x00000264
	.section .rom.000c143a, "ax"
	.incbin "baserom.gba", 0x000c143a, 0x00000002
	.section .rom.000c143c, "ax"
	.global BattleCommand_SelectAutomatic
	.type BattleCommand_SelectAutomatic, %function
	.thumb_func
BattleCommand_SelectAutomatic:
	.incbin "baserom.gba", 0x000c143c, 0x00000380
	.section .rom.000c1868, "ax"
	.incbin "baserom.gba", 0x000c1868, 0x00000048
	.section .rom.000c18b0, "ax"
	.global BattleEvent_Playback
	.type BattleEvent_Playback, %function
	.thumb_func
BattleEvent_Playback:
	.incbin "baserom.gba", 0x000c18b0, 0x00000754
	.section .rom.000c3bbc, "ax"
	.global BattleUnit_ProcessTurnEnd
	.type BattleUnit_ProcessTurnEnd, %function
	.thumb_func
BattleUnit_ProcessTurnEnd:
	.incbin "baserom.gba", 0x000c3bbc, 0x00000414
	.section .rom.000c57ae, "ax"
	.incbin "baserom.gba", 0x000c57ae, 0x00000002
	.section .rom.000c6014, "ax"
	.incbin "baserom.gba", 0x000c6014, 0x0000036c
	.section .rom.000c6a22, "ax"
	.incbin "baserom.gba", 0x000c6a22, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000c6a28, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000c6a34, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000c6a42, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000c6a50, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000c6a5e, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000c6a6c, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000c6a7a, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000c6a94, 0x0000003c
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000c6ad0, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000c6ad8, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000c6af0, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000c6b08, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000c6b20, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000c6b38, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000c6b50, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000c6b68, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000c6b80, 0x00000030
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "baserom.gba", 0x000c6bb0, 0x00000208
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "baserom.gba", 0x000c6db8, 0x0000081c
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000c75d4, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000c761c, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000c7638, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000c7640, 0x0000010c
	.global BattlePres_AdvanceArrowTiles
BattlePres_AdvanceArrowTiles:
	.incbin "baserom.gba", 0x000c774c, 0x00000800
	.global Data_080c3f34
Data_080c3f34:
	.incbin "baserom.gba", 0x000c7f4c, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000c9950, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000c9954, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000c99bc, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000c99dc, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000c99fc, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000c9a1c, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000c9a48, 0x00000100
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "baserom.gba", 0x000c9b48, 0x000000e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c9c28, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000c9c50, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000cb410, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000cb438, 0x000013c8
	.section .rom.000ceff8, "ax"
	.global Unnamed_080cb7f8
	.type Unnamed_080cb7f8, %function
	.thumb_func
Unnamed_080cb7f8:
	.incbin "baserom.gba", 0x000ceff8, 0x00000414
	.section .rom.000cf40c, "ax"
	.global BattleEffect_RunTileAndPaletteAnimation
	.type BattleEffect_RunTileAndPaletteAnimation, %function
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	.incbin "baserom.gba", 0x000cf40c, 0x000009cc
	.section .rom.000cfdd8, "ax"
	.global Func_080cc5d8
	.type Func_080cc5d8, %function
	.thumb_func
Func_080cc5d8:
	.incbin "baserom.gba", 0x000cfdd8, 0x00000388
	.section .rom.000d1834, "ax"
	.incbin "baserom.gba", 0x000d1834, 0x00000828
	.section .rom.000d2354, "ax"
	.global BattleFx_RunMemberBurst
	.type BattleFx_RunMemberBurst, %function
	.thumb_func
BattleFx_RunMemberBurst:
	.incbin "baserom.gba", 0x000d2354, 0x00000410
	.section .rom.000d2ab8, "ax"
	.global BattleFx_RunMemberBeam
	.type BattleFx_RunMemberBeam, %function
	.thumb_func
BattleFx_RunMemberBeam:
	.incbin "baserom.gba", 0x000d2ab8, 0x000005d4
	.section .rom.000d30e0, "ax"
	.global BattleFx_RunSevenMode
	.type BattleFx_RunSevenMode, %function
	.thumb_func
BattleFx_RunSevenMode:
	.incbin "baserom.gba", 0x000d30e0, 0x00000614
	.section .rom.000d4f14, "ax"
	.global Unnamed_080d1714
	.type Unnamed_080d1714, %function
	.thumb_func
Unnamed_080d1714:
	.incbin "baserom.gba", 0x000d4f14, 0x00000d38
	.section .rom.000d5c64, "ax"
	.global BattleEffect_RunPaletteParticles
	.type BattleEffect_RunPaletteParticles, %function
	.thumb_func
BattleEffect_RunPaletteParticles:
	.incbin "baserom.gba", 0x000d5c64, 0x00000934
	.section .rom.000da170, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000da170, 0x00000cec
	.section .rom.000e226e, "ax"
	.incbin "baserom.gba", 0x000e226e, 0x00000002
	.section .rom.000e2270, "ax"
	.global BattleFx_RunProjectileVolley
	.type BattleFx_RunProjectileVolley, %function
	.thumb_func
BattleFx_RunProjectileVolley:
	.incbin "baserom.gba", 0x000e2270, 0x00000e48
	.section .rom.000e362a, "ax"
	.incbin "baserom.gba", 0x000e362a, 0x00000002
	.section .rom.000e4de8, "ax"
	.global BattleFx_InitializeMode12
	.type BattleFx_InitializeMode12, %function
	.thumb_func
BattleFx_InitializeMode12:
	.incbin "baserom.gba", 0x000e4de8, 0x00000f50
	.section .rom.000e6172, "ax"
	.incbin "baserom.gba", 0x000e6172, 0x00000002
	.section .rom.000e682c, "ax"
	.incbin "baserom.gba", 0x000e682c, 0x0000088c
	.section .rom.000e72a0, "ax"
	.global BattlePres_RunBeamSequence
	.type BattlePres_RunBeamSequence, %function
	.thumb_func
BattlePres_RunBeamSequence:
	.incbin "baserom.gba", 0x000e72a0, 0x00000604
	.section .rom.000e78a4, "ax"
	.global Unnamed_080e40a4
	.type Unnamed_080e40a4, %function
	.thumb_func
Unnamed_080e40a4:
	.incbin "baserom.gba", 0x000e78a4, 0x0000064c
	.section .rom.000eab38, "ax"
	.incbin "baserom.gba", 0x000eab38, 0x000000cc
	.section .rom.000eac04, "ax"
	.global BattleEffect_RunParticleStreams
	.type BattleEffect_RunParticleStreams, %function
	.thumb_func
BattleEffect_RunParticleStreams:
	.incbin "baserom.gba", 0x000eac04, 0x00000e38
	.section .rom.000eba3c, "ax"
	.global BattleEffect_RunCirclingFallingScene
	.type BattleEffect_RunCirclingFallingScene, %function
	.thumb_func
BattleEffect_RunCirclingFallingScene:
	.incbin "baserom.gba", 0x000eba3c, 0x00000e6c
	.section .rom.000ed8d8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000ed8d8, 0x0000167c
	.section .rom.000f1278, "ax"
	.incbin "baserom.gba", 0x000f1278, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000f1280, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000f1288, 0x00000030
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000f12b8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000f12c0, 0x00000018
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000f12d8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000f12e0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000f12e8, 0x00000008
	.section .rom.000f1648, "ax"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000f1648, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000f165c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000f1684, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000f1696, 0x00000009
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000f169f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000f16a5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000f16ab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f16b2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000f16be, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000f16ca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000f16d0, 0x00000006
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000f16d6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000f16dc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000f16e8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000f16fc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000f1704, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000f1758, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000f175e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000f1764, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000f1770, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000f1776, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000f177b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000f177f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000f1783, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1788, 0x000000dc
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000f1864, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000f186a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1870, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000f187c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000f1888, 0x0000000e
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1896, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000f189c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000f189f, 0x00000003
	.incbin "baserom.gba", 0x000f18a2, 0x0000006a
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000f190c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000f191a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000f1928, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000f1934, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000f1940, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000f1958, 0x00000002
	.incbin "baserom.gba", 0x000f195a, 0x00000052
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000f19ac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000f19b4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000f19c4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000f19ca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000f19d3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000f19f5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000f19fb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000f1a07, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000f1a14, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000f1a44, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000f1a50, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000f1a5e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000f1a62, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000f1a94, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000f1a9a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000f1a9d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000f1aa9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000f1aae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000f1ab4, 0x000006c0
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000f2174, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000f2194, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000f2198, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000f219e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000f21a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000f21a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000f21a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000f21b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000f21be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000f21d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000f21d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000f21da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000f21e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000f21e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000f21e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000f21ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000f21ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000f21f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000f21f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000f2208, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000f2220, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000f222c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000f2238, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000f2241, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000f2244, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000f224a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000f2250, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000f2256, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000f2262, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000f2288, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000f2291, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000f2299, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000f22a2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000f22b2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000f22b8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000f22bb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000f22c3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000f22cc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000f22dc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000f22e2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000f22ec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000f22fa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000f2340, 0x00000008
	.global Data_080eeb48
Data_080eeb48:
	.incbin "baserom.gba", 0x000f2348, 0x00000003
	.global Data_080eeb4b
Data_080eeb4b:
	.incbin "baserom.gba", 0x000f234b, 0x00000003
	.global Data_080eeb4e
Data_080eeb4e:
	.incbin "baserom.gba", 0x000f234e, 0x00000006
	.global Data_080eeb54
Data_080eeb54:
	.incbin "baserom.gba", 0x000f2354, 0x00000004
	.global Data_080eeb58
Data_080eeb58:
	.incbin "baserom.gba", 0x000f2358, 0x00000006
	.global Data_080eeb5e
Data_080eeb5e:
	.incbin "baserom.gba", 0x000f235e, 0x00000003
	.global Data_080eeb61
Data_080eeb61:
	.incbin "baserom.gba", 0x000f2361, 0x00000010
	.global Data_080eeb71
Data_080eeb71:
	.incbin "baserom.gba", 0x000f2371, 0x00000008
	.global Data_080eeb79
Data_080eeb79:
	.incbin "baserom.gba", 0x000f2379, 0x00000007
	.global Data_080eeb80
Data_080eeb80:
	.incbin "baserom.gba", 0x000f2380, 0x00000008
	.global Data_080eeb88
Data_080eeb88:
	.incbin "baserom.gba", 0x000f2388, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000f2396, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000f23a6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000f23ae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000f23b6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000f23b9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000f23c0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000f23c8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000f23d6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000f23e2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000f23e6, 0x00000002
	.incbin "baserom.gba", 0x000f23e8, 0x00000072
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000f245a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000f245f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000f2463, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2468, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000f2470, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000f2474, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000f247d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000f2486, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000f2498, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000f24a1, 0x00000009
	.incbin "baserom.gba", 0x000f24aa, 0x00000008
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000f24b2, 0x0000002a
	.incbin "baserom.gba", 0x000f24dc, 0x00000016
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000f24f2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000f24f7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000f24fc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000f24ff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000f250e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000f251e, 0x0000001e
	.incbin "baserom.gba", 0x000f253c, 0x00000002
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000f253e, 0x00000040
	.incbin "baserom.gba", 0x000f257e, 0x00000052
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000f25d0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000f25d4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000f25e2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000f25ea, 0x0000000a
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000f25f4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000f25fb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2602, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000f2610, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000f2617, 0x00000007
	.global Data_080eee1e
Data_080eee1e:
	.incbin "baserom.gba", 0x000f261e, 0x0000000c
	.global Data_080eee2a
Data_080eee2a:
	.incbin "baserom.gba", 0x000f262a, 0x0000000c
	.global Data_080eee36
Data_080eee36:
	.incbin "baserom.gba", 0x000f2636, 0x00000008
	.global Data_080eee3e
Data_080eee3e:
	.incbin "baserom.gba", 0x000f263e, 0x00000008
	.global Data_080eee46
Data_080eee46:
	.incbin "baserom.gba", 0x000f2646, 0x00000008
	.global Data_080eee4e
Data_080eee4e:
	.incbin "baserom.gba", 0x000f264e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000f2656, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000f265e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2666, 0x000000a0
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000f2706, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000f270c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000f2712, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000f2718, 0x0000003e
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000f2756, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000f275f, 0x00000009
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000f2768, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000f2770, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000f2778, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000f2788, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000f2796, 0x0000000e
	.section .rom.000f2814, "ax"
	.incbin "baserom.gba", 0x000f2814, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000f2834, 0x000007cc
	.section .rom.000f3a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f3a5c, 0x000007d9
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f4235, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f4298, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f47dc, 0x00000824
	.section .rom.000f5028, "ax"
	.global Func_080f2028
	.type Func_080f2028, %function
	.thumb_func
Func_080f2028:
	.incbin "baserom.gba", 0x000f5028, 0x00000478
	.section .rom.000f5b6c, "ax"
	.global Func_080f2b6c
	.type Func_080f2b6c, %function
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f5b6c, 0x00000004
	.section .rom.000f6078, "ax"
	.global Unnamed_080f3078
	.type Unnamed_080f3078, %function
	.thumb_func
Unnamed_080f3078:
	.incbin "baserom.gba", 0x000f6078, 0x00000704
	.section .rom.000f68bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f68bc, 0x000000b3
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f696f, 0x00000691
	.section .rom.000f7168, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000f7168, 0x00001e98
	.section .rom.000f9440, "ax"
	.incbin "baserom.gba", 0x000f9440, 0x00000f28
	.section .rom.000fa4ac, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000fa4ac, 0x00000954
	.section .rom.000fb744, "ax"
	.incbin "baserom.gba", 0x000fb744, 0x0000003f
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000fb783, 0x0000007d
	.section .rom.000fdfa0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fdfa0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fe030, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fe0e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fe114, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fe12c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fe1b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fe1c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fe204, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fe214, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fe248, 0x00000030
	.section .rom.000fed04, "ax"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.000fee24, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fee24, 0x00000060
	.section .rom.00186e98, "ax"
	.incbin "baserom.gba", 0x00186e98, 0x00000968
	.section .rom.003217d8, "ax"
	.incbin "baserom.gba", 0x003217d8, 0x00000028
	.section .rom.003227a0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x003227a0, 0x00000010
	.section .rom.003261e7, "ax"
	.incbin "baserom.gba", 0x003261e7, 0x00000001
	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032c89c, 0x000086f8
	.section .rom.00336fe5, "ax"
	.incbin "baserom.gba", 0x00336fe5, 0x00000003
	.section .rom.003388f5, "ax"
	.incbin "baserom.gba", 0x003388f5, 0x00000003
	.section .rom.0033c3f9, "ax"
	.incbin "baserom.gba", 0x0033c3f9, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c3fc, 0x0000071c
	.section .rom.00340f5a, "ax"
	.incbin "baserom.gba", 0x00340f5a, 0x00000002
	.section .rom.003516ca, "ax"
	.incbin "baserom.gba", 0x003516ca, 0x00000002
	.section .rom.00354cba, "ax"
	.incbin "baserom.gba", 0x00354cba, 0x00000002
	.section .rom.0036075e, "ax"
	.incbin "baserom.gba", 0x0036075e, 0x00000002
	.section .rom.00370e0e, "ax"
	.incbin "baserom.gba", 0x00370e0e, 0x00000002
	.section .rom.003788ae, "ax"
	.incbin "baserom.gba", 0x003788ae, 0x00000002
	.section .rom.0037c0ee, "ax"
	.incbin "baserom.gba", 0x0037c0ee, 0x00000002
	.section .rom.00385522, "ax"
	.incbin "baserom.gba", 0x00385522, 0x00000002
	.section .rom.0039094a, "ax"
	.incbin "baserom.gba", 0x0039094a, 0x00000002
	.section .rom.00398806, "ax"
	.incbin "baserom.gba", 0x00398806, 0x00000002
	.section .rom.0039d296, "ax"
	.incbin "baserom.gba", 0x0039d296, 0x00000002
	.section .rom.003a154a, "ax"
	.incbin "baserom.gba", 0x003a154a, 0x00000002
	.section .rom.003a4eca, "ax"
	.incbin "baserom.gba", 0x003a4eca, 0x00000002
	.section .rom.003b1706, "ax"
	.incbin "baserom.gba", 0x003b1706, 0x00000002
	.section .rom.003c004e, "ax"
	.incbin "baserom.gba", 0x003c004e, 0x00000002
	.section .rom.003c5951, "ax"
	.incbin "baserom.gba", 0x003c5951, 0x00000003
	.section .rom.003c72d3, "ax"
	.incbin "baserom.gba", 0x003c72d3, 0x00000001
	.section .rom.003ca0f1, "ax"
	.incbin "baserom.gba", 0x003ca0f1, 0x00000003
	.section .rom.003cd06e, "ax"
	.incbin "baserom.gba", 0x003cd06e, 0x00000002
	.section .rom.003cd8b5, "ax"
	.incbin "baserom.gba", 0x003cd8b5, 0x00000003
	.section .rom.003cf0cb, "ax"
	.incbin "baserom.gba", 0x003cf0cb, 0x00000001
	.section .rom.003cf6b6, "ax"
	.incbin "baserom.gba", 0x003cf6b6, 0x00000002
	.section .rom.003cfd7a, "ax"
	.incbin "baserom.gba", 0x003cfd7a, 0x00000002
	.section .rom.003d0061, "ax"
	.incbin "baserom.gba", 0x003d0061, 0x00000003
	.section .rom.003d13c7, "ax"
	.incbin "baserom.gba", 0x003d13c7, 0x00000001
	.section .rom.003d17b1, "ax"
	.incbin "baserom.gba", 0x003d17b1, 0x00000003
	.section .rom.003d1b83, "ax"
	.incbin "baserom.gba", 0x003d1b83, 0x00000001
	.section .rom.003d2423, "ax"
	.incbin "baserom.gba", 0x003d2423, 0x00000001
	.section .rom.003d28c6, "ax"
	.incbin "baserom.gba", 0x003d28c6, 0x00000002
	.section .rom.003d3a7f, "ax"
	.incbin "baserom.gba", 0x003d3a7f, 0x00000001
	.section .rom.003d4f42, "ax"
	.incbin "baserom.gba", 0x003d4f42, 0x00000002
	.section .rom.003d5f0b, "ax"
	.incbin "baserom.gba", 0x003d5f0b, 0x00000001
	.section .rom.003d6166, "ax"
	.incbin "baserom.gba", 0x003d6166, 0x00000002
	.section .rom.003d7bc1, "ax"
	.incbin "baserom.gba", 0x003d7bc1, 0x00000003
	.section .rom.003d810d, "ax"
	.incbin "baserom.gba", 0x003d810d, 0x00000003
	.section .rom.003d9e96, "ax"
	.incbin "baserom.gba", 0x003d9e96, 0x00000002
	.section .rom.003da10f, "ax"
	.incbin "baserom.gba", 0x003da10f, 0x00000001
	.section .rom.003da5d6, "ax"
	.incbin "baserom.gba", 0x003da5d6, 0x00000002
	.section .rom.003dc1ab, "ax"
	.incbin "baserom.gba", 0x003dc1ab, 0x00000001
	.section .rom.003ddda9, "ax"
	.incbin "baserom.gba", 0x003ddda9, 0x00000003
	.section .rom.003ddfca, "ax"
	.incbin "baserom.gba", 0x003ddfca, 0x00000002
	.section .rom.003de407, "ax"
	.incbin "baserom.gba", 0x003de407, 0x00000001
	.section .rom.003de519, "ax"
	.incbin "baserom.gba", 0x003de519, 0x00000003
	.section .rom.003dec97, "ax"
	.incbin "baserom.gba", 0x003dec97, 0x00000001
	.section .rom.003df135, "ax"
	.incbin "baserom.gba", 0x003df135, 0x00000003
	.section .rom.003dfe4a, "ax"
	.incbin "baserom.gba", 0x003dfe4a, 0x00000002
	.section .rom.003e0bca, "ax"
	.incbin "baserom.gba", 0x003e0bca, 0x00000002
	.section .rom.003e0f5b, "ax"
	.incbin "baserom.gba", 0x003e0f5b, 0x00000001
	.section .rom.003e2ca2, "ax"
	.incbin "baserom.gba", 0x003e2ca2, 0x00000002
	.section .rom.003e3a79, "ax"
	.incbin "baserom.gba", 0x003e3a79, 0x00000003
	.section .rom.003e42a6, "ax"
	.incbin "baserom.gba", 0x003e42a6, 0x00000002
	.section .rom.003e4931, "ax"
	.incbin "baserom.gba", 0x003e4931, 0x00000003
	.section .rom.003e4aef, "ax"
	.incbin "baserom.gba", 0x003e4aef, 0x00000001
	.section .rom.003e5847, "ax"
	.incbin "baserom.gba", 0x003e5847, 0x00000001
	.section .rom.003e5d93, "ax"
	.incbin "baserom.gba", 0x003e5d93, 0x00000001
	.section .rom.003e616d, "ax"
	.incbin "baserom.gba", 0x003e616d, 0x00000003
	.section .rom.003e651b, "ax"
	.incbin "baserom.gba", 0x003e651b, 0x00000001
	.section .rom.003e84bf, "ax"
	.incbin "baserom.gba", 0x003e84bf, 0x00000001
	.section .rom.003e925b, "ax"
	.incbin "baserom.gba", 0x003e925b, 0x00000001
	.section .rom.003e9477, "ax"
	.incbin "baserom.gba", 0x003e9477, 0x00000001
	.section .rom.003e9773, "ax"
	.incbin "baserom.gba", 0x003e9773, 0x00000001
	.section .rom.003eb921, "ax"
	.incbin "baserom.gba", 0x003eb921, 0x00000003
	.section .rom.003ec6ef, "ax"
	.incbin "baserom.gba", 0x003ec6ef, 0x00000001
	.section .rom.003ed851, "ax"
	.incbin "baserom.gba", 0x003ed851, 0x00000003
	.section .rom.003eddab, "ax"
	.incbin "baserom.gba", 0x003eddab, 0x00000001
	.section .rom.003ef695, "ax"
	.incbin "baserom.gba", 0x003ef695, 0x00000003
	.section .rom.003eff36, "ax"
	.incbin "baserom.gba", 0x003eff36, 0x00000002
	.section .rom.003f0313, "ax"
	.incbin "baserom.gba", 0x003f0313, 0x00000001
	.section .rom.003f059d, "ax"
	.incbin "baserom.gba", 0x003f059d, 0x00000003
	.section .rom.003f0943, "ax"
	.incbin "baserom.gba", 0x003f0943, 0x00000001
	.section .rom.003f0b9e, "ax"
	.incbin "baserom.gba", 0x003f0b9e, 0x00000002
	.section .rom.003f0f56, "ax"
	.incbin "baserom.gba", 0x003f0f56, 0x00000002
	.section .rom.003f243f, "ax"
	.incbin "baserom.gba", 0x003f243f, 0x00000001
	.section .rom.003f3a02, "ax"
	.incbin "baserom.gba", 0x003f3a02, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f3a04, 0x00000a6c
	.section .rom.003f5246, "ax"
	.incbin "baserom.gba", 0x003f5246, 0x00000002
	.section .rom.003f55c9, "ax"
	.incbin "baserom.gba", 0x003f55c9, 0x00000003
	.section .rom.003f6279, "ax"
	.incbin "baserom.gba", 0x003f6279, 0x00000003
	.section .rom.003f6dc1, "ax"
	.incbin "baserom.gba", 0x003f6dc1, 0x00000003
	.section .rom.003f6f5a, "ax"
	.incbin "baserom.gba", 0x003f6f5a, 0x00000002
	.section .rom.003f77e7, "ax"
	.incbin "baserom.gba", 0x003f77e7, 0x00000001
	.section .rom.003f82ae, "ax"
	.incbin "baserom.gba", 0x003f82ae, 0x00000002
	.section .rom.003f87c6, "ax"
	.incbin "baserom.gba", 0x003f87c6, 0x00000002
	.section .rom.003f8dea, "ax"
	.incbin "baserom.gba", 0x003f8dea, 0x00000002
	.section .rom.003f9651, "ax"
	.incbin "baserom.gba", 0x003f9651, 0x00000003
	.section .rom.003f9a75, "ax"
	.incbin "baserom.gba", 0x003f9a75, 0x00000003
	.section .rom.003f9d0f, "ax"
	.incbin "baserom.gba", 0x003f9d0f, 0x00000001
	.section .rom.003fb6ab, "ax"
	.incbin "baserom.gba", 0x003fb6ab, 0x00000001
	.section .rom.003fd422, "ax"
	.incbin "baserom.gba", 0x003fd422, 0x00000002
	.section .rom.003ff397, "ax"
	.incbin "baserom.gba", 0x003ff397, 0x00000001
	.section .rom.003ff867, "ax"
	.incbin "baserom.gba", 0x003ff867, 0x00000001
	.section .rom.003ffef9, "ax"
	.incbin "baserom.gba", 0x003ffef9, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003ffefc, 0x00000a34
	.section .rom.00401d01, "ax"
	.incbin "baserom.gba", 0x00401d01, 0x00000003
	.section .rom.004027d2, "ax"
	.incbin "baserom.gba", 0x004027d2, 0x00000002
	.section .rom.0040330f, "ax"
	.incbin "baserom.gba", 0x0040330f, 0x00000001
	.section .rom.0040394f, "ax"
	.incbin "baserom.gba", 0x0040394f, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00403950, 0x00001588
	.section .rom.00404f39, "ax"
	.incbin "baserom.gba", 0x00404f39, 0x00000003
	.section .rom.004052a7, "ax"
	.incbin "baserom.gba", 0x004052a7, 0x00000001
	.section .rom.00405845, "ax"
	.incbin "baserom.gba", 0x00405845, 0x00000003
	.section .rom.00405b79, "ax"
	.incbin "baserom.gba", 0x00405b79, 0x00000003
	.section .rom.00405eb5, "ax"
	.incbin "baserom.gba", 0x00405eb5, 0x00000003
	.section .rom.00406ece, "ax"
	.incbin "baserom.gba", 0x00406ece, 0x00000002
	.section .rom.0040a4da, "ax"
	.incbin "baserom.gba", 0x0040a4da, 0x00000002
	.section .rom.0040ac7d, "ax"
	.incbin "baserom.gba", 0x0040ac7d, 0x00000003
	.section .rom.0040c00f, "ax"
	.incbin "baserom.gba", 0x0040c00f, 0x00000001
	.section .rom.0040c43f, "ax"
	.incbin "baserom.gba", 0x0040c43f, 0x00000001
	.section .rom.0040e235, "ax"
	.incbin "baserom.gba", 0x0040e235, 0x00000003
	.section .rom.0040eb4a, "ax"
	.incbin "baserom.gba", 0x0040eb4a, 0x00000002
	.section .rom.0041067e, "ax"
	.incbin "baserom.gba", 0x0041067e, 0x00000002
	.section .rom.004116cd, "ax"
	.incbin "baserom.gba", 0x004116cd, 0x00000003
	.section .rom.00411d83, "ax"
	.incbin "baserom.gba", 0x00411d83, 0x00000001
	.section .rom.00412e29, "ax"
	.incbin "baserom.gba", 0x00412e29, 0x00000003
	.section .rom.0042623b, "ax"
	.incbin "baserom.gba", 0x0042623b, 0x00000001
	.section .rom.0042638d, "ax"
	.incbin "baserom.gba", 0x0042638d, 0x00000003
	.section .rom.0042681e, "ax"
	.incbin "baserom.gba", 0x0042681e, 0x00000002
	.section .rom.00426a15, "ax"
	.incbin "baserom.gba", 0x00426a15, 0x00000003
	.section .rom.004280d2, "ax"
	.incbin "baserom.gba", 0x004280d2, 0x00000002
	.section .rom.004295f1, "ax"
	.incbin "baserom.gba", 0x004295f1, 0x00000003
	.section .rom.0042a1bd, "ax"
	.incbin "baserom.gba", 0x0042a1bd, 0x00000003
	.section .rom.0042ad32, "ax"
	.incbin "baserom.gba", 0x0042ad32, 0x00000002
	.section .rom.0042cbdb, "ax"
	.incbin "baserom.gba", 0x0042cbdb, 0x00000001
	.section .rom.0042e369, "ax"
	.incbin "baserom.gba", 0x0042e369, 0x00000003
	.section .rom.0042f81a, "ax"
	.incbin "baserom.gba", 0x0042f81a, 0x00000002
	.section .rom.00431f67, "ax"
	.incbin "baserom.gba", 0x00431f67, 0x00000001
	.section .rom.00433801, "ax"
	.incbin "baserom.gba", 0x00433801, 0x00000003
	.section .rom.00434dca, "ax"
	.incbin "baserom.gba", 0x00434dca, 0x00000002
	.section .rom.0043685a, "ax"
	.incbin "baserom.gba", 0x0043685a, 0x00000002
	.section .rom.00441032, "ax"
	.incbin "baserom.gba", 0x00441032, 0x00000002
	.section .rom.004431e6, "ax"
	.incbin "baserom.gba", 0x004431e6, 0x00000002
	.section .rom.0044734d, "ax"
	.incbin "baserom.gba", 0x0044734d, 0x00000003
	.section .rom.0044a572, "ax"
	.incbin "baserom.gba", 0x0044a572, 0x00000002
	.section .rom.0044bc76, "ax"
	.incbin "baserom.gba", 0x0044bc76, 0x00000002
	.section .rom.0045285e, "ax"
	.incbin "baserom.gba", 0x0045285e, 0x00000002
	.section .rom.0045b1e2, "ax"
	.incbin "baserom.gba", 0x0045b1e2, 0x00000002
	.section .rom.00462cbe, "ax"
	.incbin "baserom.gba", 0x00462cbe, 0x00000002
	.section .rom.00465d52, "ax"
	.incbin "baserom.gba", 0x00465d52, 0x00000002
	.section .rom.00469161, "ax"
	.incbin "baserom.gba", 0x00469161, 0x00000003
	.section .rom.0046b9cd, "ax"
	.incbin "baserom.gba", 0x0046b9cd, 0x00000003
	.section .rom.0046d4be, "ax"
	.incbin "baserom.gba", 0x0046d4be, 0x00000002
	.section .rom.0046e2d6, "ax"
	.incbin "baserom.gba", 0x0046e2d6, 0x00000002
	.section .rom.0046efc1, "ax"
	.incbin "baserom.gba", 0x0046efc1, 0x00000003
	.section .rom.0047847e, "ax"
	.incbin "baserom.gba", 0x0047847e, 0x00000002
	.section .rom.00479189, "ax"
	.incbin "baserom.gba", 0x00479189, 0x00000003
	.section .rom.0047b29d, "ax"
	.incbin "baserom.gba", 0x0047b29d, 0x00000003
	.section .rom.0047bf0f, "ax"
	.incbin "baserom.gba", 0x0047bf0f, 0x00000001
	.section .rom.0047cb27, "ax"
	.incbin "baserom.gba", 0x0047cb27, 0x00000001
	.section .rom.0047d29b, "ax"
	.incbin "baserom.gba", 0x0047d29b, 0x00000001
	.section .rom.0047eb2b, "ax"
	.incbin "baserom.gba", 0x0047eb2b, 0x00000001
	.section .rom.0047f4f9, "ax"
	.incbin "baserom.gba", 0x0047f4f9, 0x00000003
	.section .rom.00480117, "ax"
	.incbin "baserom.gba", 0x00480117, 0x00000001
	.section .rom.004827cb, "ax"
	.incbin "baserom.gba", 0x004827cb, 0x00000001
	.section .rom.00484ede, "ax"
	.incbin "baserom.gba", 0x00484ede, 0x00000002
	.section .rom.00489e33, "ax"
	.incbin "baserom.gba", 0x00489e33, 0x00000001
	.section .rom.0048e15d, "ax"
	.incbin "baserom.gba", 0x0048e15d, 0x00000003
	.section .rom.0048ed4a, "ax"
	.incbin "baserom.gba", 0x0048ed4a, 0x00000002
	.section .rom.004938ff, "ax"
	.incbin "baserom.gba", 0x004938ff, 0x00000001
	.section .rom.00495c69, "ax"
	.incbin "baserom.gba", 0x00495c69, 0x00000003
	.section .rom.0049760b, "ax"
	.incbin "baserom.gba", 0x0049760b, 0x00000001
	.section .rom.0049becd, "ax"
	.incbin "baserom.gba", 0x0049becd, 0x00000003
	.section .rom.0049ff95, "ax"
	.incbin "baserom.gba", 0x0049ff95, 0x00000003
	.section .rom.004a365e, "ax"
	.incbin "baserom.gba", 0x004a365e, 0x00000002
	.section .rom.004ab62f, "ax"
	.incbin "baserom.gba", 0x004ab62f, 0x00000001
	.section .rom.004b293f, "ax"
	.incbin "baserom.gba", 0x004b293f, 0x00000001
	.section .rom.004b78bf, "ax"
	.incbin "baserom.gba", 0x004b78bf, 0x00000001
	.section .rom.004bc26f, "ax"
	.incbin "baserom.gba", 0x004bc26f, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004bc270, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004bc27c, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bc3cc, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bc50c, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bc64c, 0x00000140
	.section .rom.004c1b9b, "ax"
	.incbin "baserom.gba", 0x004c1b9b, 0x00000001
	.section .rom.004c1d29, "ax"
	.incbin "baserom.gba", 0x004c1d29, 0x00000003
	.section .rom.004c6d89, "ax"
	.incbin "baserom.gba", 0x004c6d89, 0x00000003
	.section .rom.004cb2f5, "ax"
	.incbin "baserom.gba", 0x004cb2f5, 0x00000003
	.section .rom.004d04b9, "ax"
	.incbin "baserom.gba", 0x004d04b9, 0x00000003
	.section .rom.004d064f, "ax"
	.incbin "baserom.gba", 0x004d064f, 0x00000001
	.section .rom.004d32a3, "ax"
	.incbin "baserom.gba", 0x004d32a3, 0x00000001
	.section .rom.004d9f57, "ax"
	.incbin "baserom.gba", 0x004d9f57, 0x00000001
	.section .rom.004dd092, "ax"
	.incbin "baserom.gba", 0x004dd092, 0x00000002
	.section .rom.004dd213, "ax"
	.incbin "baserom.gba", 0x004dd213, 0x00000001
	.section .rom.004df9cd, "ax"
	.incbin "baserom.gba", 0x004df9cd, 0x00000003
	.section .rom.004e2016, "ax"
	.incbin "baserom.gba", 0x004e2016, 0x00000002
	.section .rom.004e46fa, "ax"
	.incbin "baserom.gba", 0x004e46fa, 0x00000002
	.section .rom.004e576f, "ax"
	.incbin "baserom.gba", 0x004e576f, 0x00000001
	.section .rom.004e78c2, "ax"
	.incbin "baserom.gba", 0x004e78c2, 0x00000002
	.section .rom.004e7a55, "ax"
	.incbin "baserom.gba", 0x004e7a55, 0x00000003
	.section .rom.004ea297, "ax"
	.incbin "baserom.gba", 0x004ea297, 0x00000001
	.section .rom.004ec0ad, "ax"
	.incbin "baserom.gba", 0x004ec0ad, 0x00000003
	.section .rom.004ee78e, "ax"
	.incbin "baserom.gba", 0x004ee78e, 0x00000002
	.section .rom.004ef8d5, "ax"
	.incbin "baserom.gba", 0x004ef8d5, 0x00000003
	.section .rom.004f29e9, "ax"
	.incbin "baserom.gba", 0x004f29e9, 0x00000003
	.section .rom.004f6b51, "ax"
	.incbin "baserom.gba", 0x004f6b51, 0x00000003
	.section .rom.004f814e, "ax"
	.incbin "baserom.gba", 0x004f814e, 0x00000002
	.section .rom.004f9417, "ax"
	.incbin "baserom.gba", 0x004f9417, 0x00000001
	.section .rom.004fc82f, "ax"
	.incbin "baserom.gba", 0x004fc82f, 0x00000001
	.section .rom.004fc9bd, "ax"
	.incbin "baserom.gba", 0x004fc9bd, 0x00000003
	.section .rom.004fe76e, "ax"
	.incbin "baserom.gba", 0x004fe76e, 0x00000002
	.section .rom.005022e5, "ax"
	.incbin "baserom.gba", 0x005022e5, 0x00000003
	.section .rom.005037f3, "ax"
	.incbin "baserom.gba", 0x005037f3, 0x00000001
	.section .rom.005043c7, "ax"
	.incbin "baserom.gba", 0x005043c7, 0x00000001
	.section .rom.005044c5, "ax"
	.incbin "baserom.gba", 0x005044c5, 0x00000003
	.section .rom.005058aa, "ax"
	.incbin "baserom.gba", 0x005058aa, 0x00000002
	.section .rom.00506a2d, "ax"
	.incbin "baserom.gba", 0x00506a2d, 0x00000003
	.section .rom.005073b6, "ax"
	.incbin "baserom.gba", 0x005073b6, 0x00000002
	.section .rom.0050a033, "ax"
	.incbin "baserom.gba", 0x0050a033, 0x00000001
	.section .rom.0050a116, "ax"
	.incbin "baserom.gba", 0x0050a116, 0x00000002
	.section .rom.0050b301, "ax"
	.incbin "baserom.gba", 0x0050b301, 0x00000003
	.section .rom.0050cf89, "ax"
	.incbin "baserom.gba", 0x0050cf89, 0x00000003
	.section .rom.0050d497, "ax"
	.incbin "baserom.gba", 0x0050d497, 0x00000001
	.section .rom.0050d5d7, "ax"
	.incbin "baserom.gba", 0x0050d5d7, 0x00000001
	.section .rom.0050e7f2, "ax"
	.incbin "baserom.gba", 0x0050e7f2, 0x00000002
	.section .rom.0050e94d, "ax"
	.incbin "baserom.gba", 0x0050e94d, 0x00000003
	.section .rom.00512d73, "ax"
	.incbin "baserom.gba", 0x00512d73, 0x00000001
	.section .rom.00514aa2, "ax"
	.incbin "baserom.gba", 0x00514aa2, 0x00000002
	.section .rom.0051581b, "ax"
	.incbin "baserom.gba", 0x0051581b, 0x00000001
	.section .rom.0051594f, "ax"
	.incbin "baserom.gba", 0x0051594f, 0x00000001
	.section .rom.00517dd2, "ax"
	.incbin "baserom.gba", 0x00517dd2, 0x00000002
	.section .rom.00518b4f, "ax"
	.incbin "baserom.gba", 0x00518b4f, 0x00000001
	.section .rom.0051962e, "ax"
	.incbin "baserom.gba", 0x0051962e, 0x00000002
	.section .rom.0051a4a6, "ax"
	.incbin "baserom.gba", 0x0051a4a6, 0x00000002
	.section .rom.0051c57a, "ax"
	.incbin "baserom.gba", 0x0051c57a, 0x00000002
	.section .rom.0051cec1, "ax"
	.incbin "baserom.gba", 0x0051cec1, 0x00000003
	.section .rom.0051db0e, "ax"
	.incbin "baserom.gba", 0x0051db0e, 0x00000002
	.section .rom.0051dd35, "ax"
	.incbin "baserom.gba", 0x0051dd35, 0x00000003
	.section .rom.0051f677, "ax"
	.incbin "baserom.gba", 0x0051f677, 0x00000001
	.section .rom.0051f78b, "ax"
	.incbin "baserom.gba", 0x0051f78b, 0x00000001
	.section .rom.00521151, "ax"
	.incbin "baserom.gba", 0x00521151, 0x00000003
	.section .rom.0052129b, "ax"
	.incbin "baserom.gba", 0x0052129b, 0x00000001
	.section .rom.00523367, "ax"
	.incbin "baserom.gba", 0x00523367, 0x00000001
	.section .rom.0052348f, "ax"
	.incbin "baserom.gba", 0x0052348f, 0x00000001
	.section .rom.00524d17, "ax"
	.incbin "baserom.gba", 0x00524d17, 0x00000001
	.section .rom.00526fb9, "ax"
	.incbin "baserom.gba", 0x00526fb9, 0x00000003
	.section .rom.005270fb, "ax"
	.incbin "baserom.gba", 0x005270fb, 0x00000001
	.section .rom.00529103, "ax"
	.incbin "baserom.gba", 0x00529103, 0x00000001
	.section .rom.0052920e, "ax"
	.incbin "baserom.gba", 0x0052920e, 0x00000002
	.section .rom.0052b1f2, "ax"
	.incbin "baserom.gba", 0x0052b1f2, 0x00000002
	.section .rom.0052c917, "ax"
	.incbin "baserom.gba", 0x0052c917, 0x00000001
	.section .rom.0052d9cb, "ax"
	.incbin "baserom.gba", 0x0052d9cb, 0x00000001
	.section .rom.0052f6b2, "ax"
	.incbin "baserom.gba", 0x0052f6b2, 0x00000002
	.section .rom.0052f80d, "ax"
	.incbin "baserom.gba", 0x0052f80d, 0x00000003
	.section .rom.00531756, "ax"
	.incbin "baserom.gba", 0x00531756, 0x00000002
	.section .rom.005318db, "ax"
	.incbin "baserom.gba", 0x005318db, 0x00000001
	.section .rom.0053442d, "ax"
	.incbin "baserom.gba", 0x0053442d, 0x00000003
	.section .rom.00536af2, "ax"
	.incbin "baserom.gba", 0x00536af2, 0x00000002
	.section .rom.0053924a, "ax"
	.incbin "baserom.gba", 0x0053924a, 0x00000002
	.section .rom.0053aa9d, "ax"
	.incbin "baserom.gba", 0x0053aa9d, 0x00000003
	.section .rom.0053d19d, "ax"
	.incbin "baserom.gba", 0x0053d19d, 0x00000003
	.section .rom.0053ef69, "ax"
	.incbin "baserom.gba", 0x0053ef69, 0x00000003
	.section .rom.005414b3, "ax"
	.incbin "baserom.gba", 0x005414b3, 0x00000001
	.section .rom.0054277e, "ax"
	.incbin "baserom.gba", 0x0054277e, 0x00000002
	.section .rom.0054342f, "ax"
	.incbin "baserom.gba", 0x0054342f, 0x00000001
	.section .rom.00543e99, "ax"
	.incbin "baserom.gba", 0x00543e99, 0x00000003
	.section .rom.00543f8f, "ax"
	.incbin "baserom.gba", 0x00543f8f, 0x00000001
	.section .rom.005459ee, "ax"
	.incbin "baserom.gba", 0x005459ee, 0x00000002
	.section .rom.00546792, "ax"
	.incbin "baserom.gba", 0x00546792, 0x00000002
	.section .rom.005482cb, "ax"
	.incbin "baserom.gba", 0x005482cb, 0x00000001
	.section .rom.0054b1b9, "ax"
	.incbin "baserom.gba", 0x0054b1b9, 0x00000003
	.section .rom.0055299f, "ax"
	.incbin "baserom.gba", 0x0055299f, 0x00000001
	.section .rom.00552a6b, "ax"
	.incbin "baserom.gba", 0x00552a6b, 0x00000001
	.section .rom.0055822d, "ax"
	.incbin "baserom.gba", 0x0055822d, 0x00000003
	.section .rom.0055836f, "ax"
	.incbin "baserom.gba", 0x0055836f, 0x00000001
	.section .rom.0055979d, "ax"
	.incbin "baserom.gba", 0x0055979d, 0x00000003
	.section .rom.0055c4da, "ax"
	.incbin "baserom.gba", 0x0055c4da, 0x00000002
	.section .rom.0055e5d6, "ax"
	.incbin "baserom.gba", 0x0055e5d6, 0x00000002
	.section .rom.00565fe2, "ax"
	.incbin "baserom.gba", 0x00565fe2, 0x00000002
	.section .rom.0056617e, "ax"
	.incbin "baserom.gba", 0x0056617e, 0x00000002
	.section .rom.0056aa19, "ax"
	.incbin "baserom.gba", 0x0056aa19, 0x00000003
	.section .rom.0056cf1a, "ax"
	.incbin "baserom.gba", 0x0056cf1a, 0x00000002
	.section .rom.0057490b, "ax"
	.incbin "baserom.gba", 0x0057490b, 0x00000001
	.section .rom.00574aa6, "ax"
	.incbin "baserom.gba", 0x00574aa6, 0x00000002
	.section .rom.00577655, "ax"
	.incbin "baserom.gba", 0x00577655, 0x00000003
	.section .rom.00579729, "ax"
	.incbin "baserom.gba", 0x00579729, 0x00000003
	.section .rom.0057b782, "ax"
	.incbin "baserom.gba", 0x0057b782, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057b784, 0x00001cec
	.section .rom.0057ec19, "ax"
	.incbin "baserom.gba", 0x0057ec19, 0x00000003
	.section .rom.005818f5, "ax"
	.incbin "baserom.gba", 0x005818f5, 0x00000003
	.section .rom.005836c7, "ax"
	.incbin "baserom.gba", 0x005836c7, 0x00000001
	.section .rom.00583807, "ax"
	.incbin "baserom.gba", 0x00583807, 0x00000001
	.section .rom.00586861, "ax"
	.incbin "baserom.gba", 0x00586861, 0x00000003
	.section .rom.005897de, "ax"
	.incbin "baserom.gba", 0x005897de, 0x00000002
	.section .rom.0058c0a5, "ax"
	.incbin "baserom.gba", 0x0058c0a5, 0x00000003
	.section .rom.0059300d, "ax"
	.incbin "baserom.gba", 0x0059300d, 0x00000003
	.section .rom.005948b3, "ax"
	.incbin "baserom.gba", 0x005948b3, 0x00000001
	.section .rom.005951b2, "ax"
	.incbin "baserom.gba", 0x005951b2, 0x00000002
	.section .rom.00595f2b, "ax"
	.incbin "baserom.gba", 0x00595f2b, 0x00000001
	.section .rom.0059608b, "ax"
	.incbin "baserom.gba", 0x0059608b, 0x00000001
	.section .rom.00597ce3, "ax"
	.incbin "baserom.gba", 0x00597ce3, 0x00000001
	.section .rom.0059957b, "ax"
	.incbin "baserom.gba", 0x0059957b, 0x00000001
	.section .rom.005996ff, "ax"
	.incbin "baserom.gba", 0x005996ff, 0x00000001
	.section .rom.0059c23b, "ax"
	.incbin "baserom.gba", 0x0059c23b, 0x00000001
	.section .rom.0059e9b5, "ax"
	.incbin "baserom.gba", 0x0059e9b5, 0x00000003
	.section .rom.0059f9a2, "ax"
	.incbin "baserom.gba", 0x0059f9a2, 0x00000002
	.section .rom.005a1063, "ax"
	.incbin "baserom.gba", 0x005a1063, 0x00000001
	.section .rom.005a120d, "ax"
	.incbin "baserom.gba", 0x005a120d, 0x00000003
	.section .rom.005a3fa1, "ax"
	.incbin "baserom.gba", 0x005a3fa1, 0x00000003
	.section .rom.005a8036, "ax"
	.incbin "baserom.gba", 0x005a8036, 0x00000002
	.section .rom.005a8177, "ax"
	.incbin "baserom.gba", 0x005a8177, 0x00000001
	.section .rom.005aa03f, "ax"
	.incbin "baserom.gba", 0x005aa03f, 0x00000001
	.section .rom.005aa1e1, "ax"
	.incbin "baserom.gba", 0x005aa1e1, 0x00000003
	.section .rom.005ac283, "ax"
	.incbin "baserom.gba", 0x005ac283, 0x00000001
	.section .rom.005ad210, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ad210, 0x000022d8
	.section .rom.005b1fd9, "ax"
	.incbin "baserom.gba", 0x005b1fd9, 0x00000003
	.section .rom.005b215e, "ax"
	.incbin "baserom.gba", 0x005b215e, 0x00000002
	.section .rom.005b98ab, "ax"
	.incbin "baserom.gba", 0x005b98ab, 0x00000001
	.section .rom.005b9db5, "ax"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .rom.005bb48d, "ax"
	.incbin "baserom.gba", 0x005bb48d, 0x00000003
	.section .rom.005bb631, "ax"
	.incbin "baserom.gba", 0x005bb631, 0x00000003
	.section .rom.005bcf36, "ax"
	.incbin "baserom.gba", 0x005bcf36, 0x00000002
	.section .rom.005be763, "ax"
	.incbin "baserom.gba", 0x005be763, 0x00000001
	.section .rom.005be8e1, "ax"
	.incbin "baserom.gba", 0x005be8e1, 0x00000003
	.section .rom.005c31d5, "ax"
	.incbin "baserom.gba", 0x005c31d5, 0x00000003
	.section .rom.005c421a, "ax"
	.incbin "baserom.gba", 0x005c421a, 0x00000002
	.section .rom.005c503e, "ax"
	.incbin "baserom.gba", 0x005c503e, 0x00000002
	.section .rom.005c6383, "ax"
	.incbin "baserom.gba", 0x005c6383, 0x00000001
	.section .rom.005c64dd, "ax"
	.incbin "baserom.gba", 0x005c64dd, 0x00000003
	.section .rom.005c8a9b, "ax"
	.incbin "baserom.gba", 0x005c8a9b, 0x00000001
	.section .rom.005c8c12, "ax"
	.incbin "baserom.gba", 0x005c8c12, 0x00000002
	.section .rom.005cd505, "ax"
	.incbin "baserom.gba", 0x005cd505, 0x00000003
	.section .rom.005cdf6d, "ax"
	.incbin "baserom.gba", 0x005cdf6d, 0x00000003
	.section .rom.005d05af, "ax"
	.incbin "baserom.gba", 0x005d05af, 0x00000001
	.section .rom.005d0716, "ax"
	.incbin "baserom.gba", 0x005d0716, 0x00000002
	.section .rom.005d185d, "ax"
	.incbin "baserom.gba", 0x005d185d, 0x00000003
	.section .rom.005d35bb, "ax"
	.incbin "baserom.gba", 0x005d35bb, 0x00000001
	.section .rom.005d3722, "ax"
	.incbin "baserom.gba", 0x005d3722, 0x00000002
	.section .rom.005d5fb1, "ax"
	.incbin "baserom.gba", 0x005d5fb1, 0x00000003
	.section .rom.005d60e6, "ax"
	.incbin "baserom.gba", 0x005d60e6, 0x00000002
	.section .rom.005d8d22, "ax"
	.incbin "baserom.gba", 0x005d8d22, 0x00000002
	.section .rom.005dc4c1, "ax"
	.incbin "baserom.gba", 0x005dc4c1, 0x00000003
	.section .rom.005df80f, "ax"
	.incbin "baserom.gba", 0x005df80f, 0x00000001
	.section .rom.005df98b, "ax"
	.incbin "baserom.gba", 0x005df98b, 0x00000001
	.section .rom.005e253a, "ax"
	.incbin "baserom.gba", 0x005e253a, 0x00000002
	.section .rom.005e47ae, "ax"
	.incbin "baserom.gba", 0x005e47ae, 0x00000002
	.section .rom.005e701f, "ax"
	.incbin "baserom.gba", 0x005e701f, 0x00000001
	.section .rom.005ea513, "ax"
	.incbin "baserom.gba", 0x005ea513, 0x00000001
	.section .rom.005ef13a, "ax"
	.incbin "baserom.gba", 0x005ef13a, 0x00000002
	.section .rom.005efdbd, "ax"
	.incbin "baserom.gba", 0x005efdbd, 0x00000003
	.section .rom.005efeff, "ax"
	.incbin "baserom.gba", 0x005efeff, 0x00000001
	.section .rom.005f146f, "ax"
	.incbin "baserom.gba", 0x005f146f, 0x00000001
	.section .rom.005f15da, "ax"
	.incbin "baserom.gba", 0x005f15da, 0x00000002
	.section .rom.005f3335, "ax"
	.incbin "baserom.gba", 0x005f3335, 0x00000003
	.section .rom.005f3477, "ax"
	.incbin "baserom.gba", 0x005f3477, 0x00000001
	.section .rom.005f5c76, "ax"
	.incbin "baserom.gba", 0x005f5c76, 0x00000002
	.section .rom.005f5db7, "ax"
	.incbin "baserom.gba", 0x005f5db7, 0x00000001
	.section .rom.005f77cf, "ax"
	.incbin "baserom.gba", 0x005f77cf, 0x00000001
	.section .rom.005f8d4a, "ax"
	.incbin "baserom.gba", 0x005f8d4a, 0x00000002
	.section .rom.005fa286, "ax"
	.incbin "baserom.gba", 0x005fa286, 0x00000002
	.section .rom.005fa416, "ax"
	.incbin "baserom.gba", 0x005fa416, 0x00000002
	.section .rom.005fc6cf, "ax"
	.incbin "baserom.gba", 0x005fc6cf, 0x00000001
	.section .rom.00600916, "ax"
	.incbin "baserom.gba", 0x00600916, 0x00000002
	.section .rom.0060270d, "ax"
	.incbin "baserom.gba", 0x0060270d, 0x00000003
	.section .rom.006058bb, "ax"
	.incbin "baserom.gba", 0x006058bb, 0x00000001
	.section .rom.00607eef, "ax"
	.incbin "baserom.gba", 0x00607eef, 0x00000001
	.section .rom.0060aa47, "ax"
	.incbin "baserom.gba", 0x0060aa47, 0x00000001
	.section .rom.0060ac0f, "ax"
	.incbin "baserom.gba", 0x0060ac0f, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ac10, 0x00002904
	.section .rom.00611019, "ax"
	.incbin "baserom.gba", 0x00611019, 0x00000003
	.section .rom.0061258a, "ax"
	.incbin "baserom.gba", 0x0061258a, 0x00000002
	.section .rom.00614db1, "ax"
	.incbin "baserom.gba", 0x00614db1, 0x00000003
	.section .rom.00614f21, "ax"
	.incbin "baserom.gba", 0x00614f21, 0x00000003
	.section .rom.006177e3, "ax"
	.incbin "baserom.gba", 0x006177e3, 0x00000001
	.section .rom.00619fab, "ax"
	.incbin "baserom.gba", 0x00619fab, 0x00000001
	.section .rom.0061b2a5, "ax"
	.incbin "baserom.gba", 0x0061b2a5, 0x00000003
	.section .rom.0061cbeb, "ax"
	.incbin "baserom.gba", 0x0061cbeb, 0x00000001
	.section .rom.0062807e, "ax"
	.incbin "baserom.gba", 0x0062807e, 0x00000002
	.section .rom.0062823e, "ax"
	.incbin "baserom.gba", 0x0062823e, 0x00000002
	.section .rom.0062b5a5, "ax"
	.incbin "baserom.gba", 0x0062b5a5, 0x00000003
	.section .rom.0062da96, "ax"
	.incbin "baserom.gba", 0x0062da96, 0x00000002
	.section .rom.00630877, "ax"
	.incbin "baserom.gba", 0x00630877, 0x00000001
	.section .rom.00631411, "ax"
	.incbin "baserom.gba", 0x00631411, 0x00000003
	.section .rom.00632272, "ax"
	.incbin "baserom.gba", 0x00632272, 0x00000002
	.section .rom.006323ca, "ax"
	.incbin "baserom.gba", 0x006323ca, 0x00000002
	.section .rom.0063a217, "ax"
	.incbin "baserom.gba", 0x0063a217, 0x00000001
	.section .rom.0063a383, "ax"
	.incbin "baserom.gba", 0x0063a383, 0x00000001
	.section .rom.0063d11f, "ax"
	.incbin "baserom.gba", 0x0063d11f, 0x00000001
	.section .rom.0063f2ae, "ax"
	.incbin "baserom.gba", 0x0063f2ae, 0x00000002
	.section .rom.0063f9b6, "ax"
	.incbin "baserom.gba", 0x0063f9b6, 0x00000002
	.section .rom.006405ca, "ax"
	.incbin "baserom.gba", 0x006405ca, 0x00000002
	.section .rom.0065169d, "ax"
	.incbin "baserom.gba", 0x0065169d, 0x00000003
	.section .rom.00651832, "ax"
	.incbin "baserom.gba", 0x00651832, 0x00000002
	.section .rom.00652d42, "ax"
	.incbin "baserom.gba", 0x00652d42, 0x00000002
	.section .rom.00654b27, "ax"
	.incbin "baserom.gba", 0x00654b27, 0x00000001
	.section .rom.00655bba, "ax"
	.incbin "baserom.gba", 0x00655bba, 0x00000002
	.section .rom.00657e2a, "ax"
	.incbin "baserom.gba", 0x00657e2a, 0x00000002
	.section .rom.00657fc1, "ax"
	.incbin "baserom.gba", 0x00657fc1, 0x00000003
	.section .rom.00659f67, "ax"
	.incbin "baserom.gba", 0x00659f67, 0x00000001
	.section .rom.0065ed3e, "ax"
	.incbin "baserom.gba", 0x0065ed3e, 0x00000002
	.section .rom.0066279f, "ax"
	.incbin "baserom.gba", 0x0066279f, 0x00000001
	.section .rom.006654d5, "ax"
	.incbin "baserom.gba", 0x006654d5, 0x00000003
	.section .rom.0066781d, "ax"
	.incbin "baserom.gba", 0x0066781d, 0x00000003
	.section .rom.00668ffb, "ax"
	.incbin "baserom.gba", 0x00668ffb, 0x00000001
	.section .rom.006691ad, "ax"
	.incbin "baserom.gba", 0x006691ad, 0x00000003
	.section .rom.00679b4e, "ax"
	.incbin "baserom.gba", 0x00679b4e, 0x00000002
	.section .rom.00679cad, "ax"
	.incbin "baserom.gba", 0x00679cad, 0x00000003
	.section .rom.0067d407, "ax"
	.incbin "baserom.gba", 0x0067d407, 0x00000001
	.section .rom.0067d56a, "ax"
	.incbin "baserom.gba", 0x0067d56a, 0x00000002
	.section .rom.0068324d, "ax"
	.incbin "baserom.gba", 0x0068324d, 0x00000003
	.section .rom.00685c33, "ax"
	.incbin "baserom.gba", 0x00685c33, 0x00000001
	.section .rom.00687e3e, "ax"
	.incbin "baserom.gba", 0x00687e3e, 0x00000002
	.section .rom.00687f7f, "ax"
	.incbin "baserom.gba", 0x00687f7f, 0x00000001
	.section .rom.00689996, "ax"
	.incbin "baserom.gba", 0x00689996, 0x00000002
	.section .rom.006906fd, "ax"
	.incbin "baserom.gba", 0x006906fd, 0x00000003
	.section .rom.0069087b, "ax"
	.incbin "baserom.gba", 0x0069087b, 0x00000001
	.section .rom.00692825, "ax"
	.incbin "baserom.gba", 0x00692825, 0x00000003
	.section .rom.00694f9e, "ax"
	.incbin "baserom.gba", 0x00694f9e, 0x00000002
	.section .rom.00696805, "ax"
	.incbin "baserom.gba", 0x00696805, 0x00000003
	.section .rom.0069a226, "ax"
	.incbin "baserom.gba", 0x0069a226, 0x00000002
	.section .rom.0069a3ef, "ax"
	.incbin "baserom.gba", 0x0069a3ef, 0x00000001
	.section .rom.006a53d7, "ax"
	.incbin "baserom.gba", 0x006a53d7, 0x00000001
	.section .rom.006a7523, "ax"
	.incbin "baserom.gba", 0x006a7523, 0x00000001
	.section .rom.006a76ad, "ax"
	.incbin "baserom.gba", 0x006a76ad, 0x00000003
	.section .rom.006a9132, "ax"
	.incbin "baserom.gba", 0x006a9132, 0x00000002
	.section .rom.006ac169, "ax"
	.incbin "baserom.gba", 0x006ac169, 0x00000003
	.section .rom.006ac32f, "ax"
	.incbin "baserom.gba", 0x006ac32f, 0x00000001
	.section .rom.006ad263, "ax"
	.incbin "baserom.gba", 0x006ad263, 0x00000001
	.section .rom.006af316, "ax"
	.incbin "baserom.gba", 0x006af316, 0x00000002
	.section .rom.006af4bd, "ax"
	.incbin "baserom.gba", 0x006af4bd, 0x00000003
	.section .rom.006b44e7, "ax"
	.incbin "baserom.gba", 0x006b44e7, 0x00000001
	.section .rom.006b7277, "ax"
	.incbin "baserom.gba", 0x006b7277, 0x00000001
	.section .rom.006b7437, "ax"
	.incbin "baserom.gba", 0x006b7437, 0x00000001
	.section .rom.006b992b, "ax"
	.incbin "baserom.gba", 0x006b992b, 0x00000001
	.section .rom.006b9afd, "ax"
	.incbin "baserom.gba", 0x006b9afd, 0x00000003
	.section .rom.006bbb8e, "ax"
	.incbin "baserom.gba", 0x006bbb8e, 0x00000002
	.section .rom.006bdc8e, "ax"
	.incbin "baserom.gba", 0x006bdc8e, 0x00000002
	.section .rom.006c0842, "ax"
	.incbin "baserom.gba", 0x006c0842, 0x00000002
	.section .rom.006c132f, "ax"
	.incbin "baserom.gba", 0x006c132f, 0x00000001
	.section .rom.006c1502, "ax"
	.incbin "baserom.gba", 0x006c1502, 0x00000002
	.section .rom.006c3e31, "ax"
	.incbin "baserom.gba", 0x006c3e31, 0x00000003
	.section .rom.006c55c9, "ax"
	.incbin "baserom.gba", 0x006c55c9, 0x00000003
	.section .rom.006c683d, "ax"
	.incbin "baserom.gba", 0x006c683d, 0x00000003
	.section .rom.006c8ccd, "ax"
	.incbin "baserom.gba", 0x006c8ccd, 0x00000003
	.section .rom.006cb019, "ax"
	.incbin "baserom.gba", 0x006cb019, 0x00000003
	.section .rom.006cc203, "ax"
	.incbin "baserom.gba", 0x006cc203, 0x00000001
	.section .rom.006cc38a, "ax"
	.incbin "baserom.gba", 0x006cc38a, 0x00000002
	.section .rom.006d08d6, "ax"
	.incbin "baserom.gba", 0x006d08d6, 0x00000002
	.section .rom.006d0a2b, "ax"
	.incbin "baserom.gba", 0x006d0a2b, 0x00000001
	.section .rom.006d0b6b, "ax"
	.incbin "baserom.gba", 0x006d0b6b, 0x00000001
	.section .rom.006d183f, "ax"
	.incbin "baserom.gba", 0x006d183f, 0x00000001
	.section .rom.006d19be, "ax"
	.incbin "baserom.gba", 0x006d19be, 0x00000002
	.section .rom.006d3277, "ax"
	.incbin "baserom.gba", 0x006d3277, 0x00000001
	.section .rom.006d33f9, "ax"
	.incbin "baserom.gba", 0x006d33f9, 0x00000003
	.section .rom.006d4e8b, "ax"
	.incbin "baserom.gba", 0x006d4e8b, 0x00000001
	.section .rom.006d5005, "ax"
	.incbin "baserom.gba", 0x006d5005, 0x00000003
	.section .rom.006d6d43, "ax"
	.incbin "baserom.gba", 0x006d6d43, 0x00000001
	.section .rom.006d6e6f, "ax"
	.incbin "baserom.gba", 0x006d6e6f, 0x00000001
	.section .rom.006d6fe5, "ax"
	.incbin "baserom.gba", 0x006d6fe5, 0x00000003
	.section .rom.006d8e8b, "ax"
	.incbin "baserom.gba", 0x006d8e8b, 0x00000001
	.section .rom.006d9011, "ax"
	.incbin "baserom.gba", 0x006d9011, 0x00000003
	.section .rom.006d9f6f, "ax"
	.incbin "baserom.gba", 0x006d9f6f, 0x00000001
	.section .rom.006db21f, "ax"
	.incbin "baserom.gba", 0x006db21f, 0x00000001
	.section .rom.006db3da, "ax"
	.incbin "baserom.gba", 0x006db3da, 0x00000002
	.section .rom.006dde7e, "ax"
	.incbin "baserom.gba", 0x006dde7e, 0x00000002
	.section .rom.006ddff1, "ax"
	.incbin "baserom.gba", 0x006ddff1, 0x00000003
	.section .rom.006df3d9, "ax"
	.incbin "baserom.gba", 0x006df3d9, 0x00000003
	.section .rom.006e1eb9, "ax"
	.incbin "baserom.gba", 0x006e1eb9, 0x00000003
	.section .rom.006e2063, "ax"
	.incbin "baserom.gba", 0x006e2063, 0x00000001
	.section .rom.006e445e, "ax"
	.incbin "baserom.gba", 0x006e445e, 0x00000002
	.section .rom.006e45ed, "ax"
	.incbin "baserom.gba", 0x006e45ed, 0x00000003
	.section .rom.006e786d, "ax"
	.incbin "baserom.gba", 0x006e786d, 0x00000003
	.section .rom.006e7a4e, "ax"
	.incbin "baserom.gba", 0x006e7a4e, 0x00000002
	.section .rom.006e9fe3, "ax"
	.incbin "baserom.gba", 0x006e9fe3, 0x00000001
	.section .rom.006ea172, "ax"
	.incbin "baserom.gba", 0x006ea172, 0x00000002
	.section .rom.006ecd6a, "ax"
	.incbin "baserom.gba", 0x006ecd6a, 0x00000002
	.section .rom.006ed8b7, "ax"
	.incbin "baserom.gba", 0x006ed8b7, 0x00000001
	.section .rom.006eda1e, "ax"
	.incbin "baserom.gba", 0x006eda1e, 0x00000002
	.section .rom.006f0433, "ax"
	.incbin "baserom.gba", 0x006f0433, 0x00000001
	.section .rom.006f2072, "ax"
	.incbin "baserom.gba", 0x006f2072, 0x00000002
	.section .rom.006f252f, "ax"
	.incbin "baserom.gba", 0x006f252f, 0x00000001
	.section .rom.006f38a7, "ax"
	.incbin "baserom.gba", 0x006f38a7, 0x00000001
	.section .rom.006f4deb, "ax"
	.incbin "baserom.gba", 0x006f4deb, 0x00000001
	.section .rom.006f4f75, "ax"
	.incbin "baserom.gba", 0x006f4f75, 0x00000003
	.section .rom.006f6053, "ax"
	.incbin "baserom.gba", 0x006f6053, 0x00000001
	.section .rom.006f61e1, "ax"
	.incbin "baserom.gba", 0x006f61e1, 0x00000003
	.section .rom.006f7e35, "ax"
	.incbin "baserom.gba", 0x006f7e35, 0x00000003
	.section .rom.006fa817, "ax"
	.incbin "baserom.gba", 0x006fa817, 0x00000001
	.section .rom.006fdd41, "ax"
	.incbin "baserom.gba", 0x006fdd41, 0x00000003
	.section .rom.006ff64b, "ax"
	.incbin "baserom.gba", 0x006ff64b, 0x00000001
	.section .rom.007016f3, "ax"
	.incbin "baserom.gba", 0x007016f3, 0x00000001
	.section .rom.00704147, "ax"
	.incbin "baserom.gba", 0x00704147, 0x00000001
	.section .rom.00705d93, "ax"
	.incbin "baserom.gba", 0x00705d93, 0x00000001
	.section .rom.007071d9, "ax"
	.incbin "baserom.gba", 0x007071d9, 0x00000003
	.section .rom.00709059, "ax"
	.incbin "baserom.gba", 0x00709059, 0x00000003
	.section .rom.0070ac2a, "ax"
	.incbin "baserom.gba", 0x0070ac2a, 0x00000002
	.section .rom.0070d35e, "ax"
	.incbin "baserom.gba", 0x0070d35e, 0x00000002
	.section .rom.0070efa3, "ax"
	.incbin "baserom.gba", 0x0070efa3, 0x00000001
	.section .rom.007103e9, "ax"
	.incbin "baserom.gba", 0x007103e9, 0x00000003
	.section .rom.00711f67, "ax"
	.incbin "baserom.gba", 0x00711f67, 0x00000001
	.section .rom.00713197, "ax"
	.incbin "baserom.gba", 0x00713197, 0x00000001
	.section .rom.007132f1, "ax"
	.incbin "baserom.gba", 0x007132f1, 0x00000003
	.section .rom.007167f7, "ax"
	.incbin "baserom.gba", 0x007167f7, 0x00000001
	.section .rom.00716962, "ax"
	.incbin "baserom.gba", 0x00716962, 0x00000002
	.section .rom.007192b2, "ax"
	.incbin "baserom.gba", 0x007192b2, 0x00000002
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007192b4, 0x00001164
	.section .rom.0071bbfd, "ax"
	.incbin "baserom.gba", 0x0071bbfd, 0x00000003
	.section .rom.0071ee43, "ax"
	.incbin "baserom.gba", 0x0071ee43, 0x00000001
	.section .rom.0072173a, "ax"
	.incbin "baserom.gba", 0x0072173a, 0x00000002
	.section .rom.0072aa73, "ax"
	.incbin "baserom.gba", 0x0072aa73, 0x00000001
	.section .rom.0072bda2, "ax"
	.incbin "baserom.gba", 0x0072bda2, 0x00000002
	.section .rom.0072cf9b, "ax"
	.incbin "baserom.gba", 0x0072cf9b, 0x00000001
	.section .rom.0072d0fd, "ax"
	.incbin "baserom.gba", 0x0072d0fd, 0x00000003
	.section .rom.0072fac9, "ax"
	.incbin "baserom.gba", 0x0072fac9, 0x00000003
	.section .rom.00732ff5, "ax"
	.incbin "baserom.gba", 0x00732ff5, 0x00000003
	.section .rom.00737437, "ax"
	.incbin "baserom.gba", 0x00737437, 0x00000001
	.section .rom.007375aa, "ax"
	.incbin "baserom.gba", 0x007375aa, 0x00000002
	.section .rom.0073b80f, "ax"
	.incbin "baserom.gba", 0x0073b80f, 0x00000001
	.section .rom.0073f02a, "ax"
	.incbin "baserom.gba", 0x0073f02a, 0x00000002
	.section .rom.00740a52, "ax"
	.incbin "baserom.gba", 0x00740a52, 0x00000002
	.section .rom.007430ff, "ax"
	.incbin "baserom.gba", 0x007430ff, 0x00000001
	.section .rom.00745163, "ax"
	.incbin "baserom.gba", 0x00745163, 0x00000001
	.section .rom.0074701f, "ax"
	.incbin "baserom.gba", 0x0074701f, 0x00000001
	.section .rom.0074a269, "ax"
	.incbin "baserom.gba", 0x0074a269, 0x00000003
	.section .rom.0074bc2f, "ax"
	.incbin "baserom.gba", 0x0074bc2f, 0x00000001
	.section .rom.0074bda1, "ax"
	.incbin "baserom.gba", 0x0074bda1, 0x00000003
	.section .rom.0074edae, "ax"
	.incbin "baserom.gba", 0x0074edae, 0x00000002
	.section .rom.0074ef66, "ax"
	.incbin "baserom.gba", 0x0074ef66, 0x00000002
	.section .rom.007507d7, "ax"
	.incbin "baserom.gba", 0x007507d7, 0x00000001
	.section .rom.00750956, "ax"
	.incbin "baserom.gba", 0x00750956, 0x00000002
	.section .rom.0075415f, "ax"
	.incbin "baserom.gba", 0x0075415f, 0x00000001
	.section .rom.007542d3, "ax"
	.incbin "baserom.gba", 0x007542d3, 0x00000001
	.section .rom.0075509b, "ax"
	.incbin "baserom.gba", 0x0075509b, 0x00000001
	.section .rom.00755241, "ax"
	.incbin "baserom.gba", 0x00755241, 0x00000003
	.section .rom.0075729c, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x0075729c, 0x000000e4
	.section .rom.0075a097, "ax"
	.incbin "baserom.gba", 0x0075a097, 0x00000001
	.section .rom.0075a27e, "ax"
	.incbin "baserom.gba", 0x0075a27e, 0x00000002
	.section .rom.0075d51a, "ax"
	.incbin "baserom.gba", 0x0075d51a, 0x00000002
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d51c, 0x00004600
	.section .rom.00761c22, "ax"
	.incbin "baserom.gba", 0x00761c22, 0x00000002
	.section .rom.007637f5, "ax"
	.incbin "baserom.gba", 0x007637f5, 0x00000003
	.section .rom.007652a5, "ax"
	.incbin "baserom.gba", 0x007652a5, 0x00000003
	.section .rom.00765401, "ax"
	.incbin "baserom.gba", 0x00765401, 0x00000003
	.section .rom.0076b647, "ax"
	.incbin "baserom.gba", 0x0076b647, 0x00000001
	.section .rom.0076b74b, "ax"
	.incbin "baserom.gba", 0x0076b74b, 0x00000001
	.section .rom.0076c167, "ax"
	.incbin "baserom.gba", 0x0076c167, 0x00000001
	.section .rom.0076c24d, "ax"
	.incbin "baserom.gba", 0x0076c24d, 0x00000003
	.section .rom.0076c4e9, "ax"
	.incbin "baserom.gba", 0x0076c4e9, 0x00000003
	.section .rom.00770546, "ax"
	.incbin "baserom.gba", 0x00770546, 0x00000002
	.section .rom.00770afa, "ax"
	.incbin "baserom.gba", 0x00770afa, 0x00000002
	.section .rom.00771a8f, "ax"
	.incbin "baserom.gba", 0x00771a8f, 0x00000001
	.section .rom.00773492, "ax"
	.incbin "baserom.gba", 0x00773492, 0x00000002
	.section .rom.00774101, "ax"
	.incbin "baserom.gba", 0x00774101, 0x00000003
	.section .rom.007743a9, "ax"
	.incbin "baserom.gba", 0x007743a9, 0x00000003
	.section .rom.0077522e, "ax"
	.incbin "baserom.gba", 0x0077522e, 0x00000002
	.section .rom.007763bf, "ax"
	.incbin "baserom.gba", 0x007763bf, 0x00000001
	.section .rom.00776587, "ax"
	.incbin "baserom.gba", 0x00776587, 0x00000001
	.section .rom.007768d3, "ax"
	.incbin "baserom.gba", 0x007768d3, 0x00000001
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x007768d4, 0x0000000c
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x007768e0, 0x00000150
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x00776a30, 0x00000140
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x00776b70, 0x00000140
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x00776cb0, 0x00000140
	.section .rom.0077713b, "ax"
	.incbin "baserom.gba", 0x0077713b, 0x00000001
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x0077713c, 0x0000000c
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x00777148, 0x00000150
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x00777298, 0x00000140
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x007773d8, 0x00000140
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x00777518, 0x00000140
	.section .rom.007779a3, "ax"
	.incbin "baserom.gba", 0x007779a3, 0x00000001
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x007779a4, 0x0000000c
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x007779b0, 0x00000150
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x00777b00, 0x00000140
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x00777c40, 0x00000140
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x00777d80, 0x00000140
	.section .rom.0077820b, "ax"
	.incbin "baserom.gba", 0x0077820b, 0x00000001
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x0077820c, 0x0000000c
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x00778218, 0x00000150
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x00778368, 0x00000140
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x007784a8, 0x00000140
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x007785e8, 0x00000140
	.section .rom.00778a73, "ax"
	.incbin "baserom.gba", 0x00778a73, 0x00000001
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x00778a74, 0x0000000c
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x00778a80, 0x00000150
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x00778bd0, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x00778d10, 0x00000140
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x00778e50, 0x00000140
	.section .rom.007792db, "ax"
	.incbin "baserom.gba", 0x007792db, 0x00000001
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x007792dc, 0x0000000c
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x007792e8, 0x00000150
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x00779438, 0x00000140
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x00779578, 0x00000140
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x007796b8, 0x00000140
	.section .rom.00779b43, "ax"
	.incbin "baserom.gba", 0x00779b43, 0x00000001
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00779b44, 0x0000000c
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x00779b50, 0x00000150
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x00779ca0, 0x00000140
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x00779de0, 0x00000140
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x00779f20, 0x00000140
	.section .rom.0077a3ab, "ax"
	.incbin "baserom.gba", 0x0077a3ab, 0x00000001
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x0077a3ac, 0x0000000c
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x0077a3b8, 0x00000150
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x0077a508, 0x00000140
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x0077a648, 0x00000140
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x0077a788, 0x00000140
