@ tbs-fr's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "ax"
	.global Rom_Start
Rom_Start:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .rom.00004ff4, "ax"
	.global Graphics_PrepareTransfer
	.type Graphics_PrepareTransfer, %function
	.thumb_func
Graphics_PrepareTransfer:
	.incbin "baserom.gba", 0x00004ff4, 0x000001f4
	.section .rom.0000616c, "ax"
	.global SerialRuntime_CollectReceivedPayloads
	.type SerialRuntime_CollectReceivedPayloads, %function
	.thumb_func
SerialRuntime_CollectReceivedPayloads:
	.incbin "baserom.gba", 0x0000616c, 0x000000e4
	.section .rom.0000656c, "ax"
	.global SerialRuntime_StepBlockTransfer
	.type SerialRuntime_StepBlockTransfer, %function
	.thumb_func
SerialRuntime_StepBlockTransfer:
	.incbin "baserom.gba", 0x0000656c, 0x0000023c
	.section .rom.0000687e, "ax"
	.incbin "baserom.gba", 0x0000687e, 0x00000002
	.section .rom.00007330, "ax"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x00007330, 0x00000356
	.global Math_ArcTanTable
Math_ArcTanTable:
	.incbin "baserom.gba", 0x00007686, 0x00000106
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "baserom.gba", 0x0000778c, 0x00000020
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "baserom.gba", 0x000077ac, 0x000001c0
	.global RomBytes_0800795c
RomBytes_0800795c:
	.incbin "baserom.gba", 0x0000796c, 0x00000014
	.global Text_PowersOfTen
Text_PowersOfTen:
	.incbin "baserom.gba", 0x00007980, 0x00000024
	.section .rom.000079c0, "ax"
	.global Save_Signature
Save_Signature:
	.incbin "baserom.gba", 0x000079c0, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x000079c8, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a1c, 0x00000014
	.section .rom.00007a78, "ax"
	.incbin "baserom.gba", 0x00007a78, 0x00000024
	.section .rom.00007ab4, "ax"
	.incbin "baserom.gba", 0x00007ab4, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007acc, 0x00000058
	.section .rom.00007b48, "ax"
	.incbin "baserom.gba", 0x00007b48, 0x0000008c
	.section .rom.00007bdc, "ax"
	.incbin "baserom.gba", 0x00007bdc, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007bf4, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c20, 0x0000002c
	.section .rom.00007c74, "ax"
	.incbin "baserom.gba", 0x00007c74, 0x00000b8c
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
	.incbin "baserom.gba", 0x00012ee0, 0x00001120
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
	.incbin "baserom.gba", 0x000158d0, 0x0000060c
	.section .rom.00016f5e, "ax"
	.incbin "baserom.gba", 0x00016f5e, 0x00000002
	.section .rom.00016f60, "ax"
	.global UiText_BuildRenderEntries
	.type UiText_BuildRenderEntries, %function
	.thumb_func
UiText_BuildRenderEntries:
	.global Func_08018038
Func_08018038:
	.incbin "baserom.gba", 0x00016f60, 0x0000064c
	.section .rom.00017744, "ax"
	.global UiText_MeasureEntryDimensions
	.type UiText_MeasureEntryDimensions, %function
	.thumb_func
UiText_MeasureEntryDimensions:
	.incbin "baserom.gba", 0x00017744, 0x000001f8
	.section .rom.0001793c, "ax"
	.global UiText_MeasureStringVariant
	.type UiText_MeasureStringVariant, %function
	.thumb_func
UiText_MeasureStringVariant:
	.incbin "baserom.gba", 0x0001793c, 0x00000248
	.global Func_08018cac
Func_08018cac:
	.incbin "baserom.gba", 0x00017b84, 0x00000250
	.section .rom.000180a4, "ax"
	.global UiWork_AnimateSpriteSlots
	.type UiWork_AnimateSpriteSlots, %function
	.thumb_func
UiWork_AnimateSpriteSlots:
	.incbin "baserom.gba", 0x000180a4, 0x00000480
	.section .rom.00018b2c, "ax"
	.incbin "baserom.gba", 0x00018b2c, 0x00000110
	.section .rom.000198bc, "ax"
	.global MenuSelection_DrawFrame
	.type MenuSelection_DrawFrame, %function
	.thumb_func
MenuSelection_DrawFrame:
	.incbin "baserom.gba", 0x000198bc, 0x00000560
	.section .rom.0001a998, "ax"
	.global Menu_ScrollSelectionList
	.type Menu_ScrollSelectionList, %function
	.thumb_func
Menu_ScrollSelectionList:
	.incbin "baserom.gba", 0x0001a998, 0x000001cc
	.section .rom.0001adb0, "ax"
	.global Menu_ConfirmSelection
	.type Menu_ConfirmSelection, %function
	.thumb_func
Menu_ConfirmSelection:
	.incbin "baserom.gba", 0x0001adb0, 0x00000244
	.section .rom.0001b3fc, "ax"
	.global Debug_SelectAbilityPair
	.type Debug_SelectAbilityPair, %function
	.thumb_func
Debug_SelectAbilityPair:
	.global Menu_Check
	.type Menu_Check, %function
	.thumb_func
Menu_Check:
	.incbin "baserom.gba", 0x0001b3fc, 0x00000360
	.section .rom.0001c934, "ax"
	.global Menu_CreateWorkspaceWindows
	.type Menu_CreateWorkspaceWindows, %function
	.thumb_func
Menu_CreateWorkspaceWindows:
	.incbin "baserom.gba", 0x0001c934, 0x0000019c
	.section .rom.0001cc88, "ax"
	.incbin "baserom.gba", 0x0001cc88, 0x00000134
	.section .rom.0001cdbc, "ax"
	.global UiText_RenderStringTiles
	.type UiText_RenderStringTiles, %function
	.thumb_func
UiText_RenderStringTiles:
	.incbin "baserom.gba", 0x0001cdbc, 0x00000410
	.section .rom.0001ded4, "ax"
	.incbin "baserom.gba", 0x0001ded4, 0x00000298
	.section .rom.0001e16c, "ax"
	.global UiWindow_DrawPartyStatusContents
	.type UiWindow_DrawPartyStatusContents, %function
	.thumb_func
UiWindow_DrawPartyStatusContents:
	.incbin "baserom.gba", 0x0001e16c, 0x000003d4
	.section .rom.0001f1c8, "ax"
	.global SaveMenu_SelectSlot
	.type SaveMenu_SelectSlot, %function
	.thumb_func
SaveMenu_SelectSlot:
	.incbin "baserom.gba", 0x0001f1c8, 0x00000580
	.section .rom.0001fb5a, "ax"
	.incbin "baserom.gba", 0x0001fb5a, 0x00000002
	.section .rom.0001fb5c, "ax"
	.global NameEntry_EditOwnerName
	.type NameEntry_EditOwnerName, %function
	.thumb_func
NameEntry_EditOwnerName:
	.incbin "baserom.gba", 0x0001fb5c, 0x000004b4
	.section .rom.00020334, "ax"
	.global Party_ShowJoinedMessage
	.type Party_ShowJoinedMessage, %function
	.thumb_func
Party_ShowJoinedMessage:
	.incbin "baserom.gba", 0x00020334, 0x000000f8
	.section .rom.00020e0e, "ax"
	.incbin "baserom.gba", 0x00020e0e, 0x000008fe
	.section .rom.00021a1e, "ax"
	.incbin "baserom.gba", 0x00021a1e, 0x00002706
	.section .rom.000241a4, "ax"
	.incbin "baserom.gba", 0x000241a4, 0x00001c80
	.section .rom.000260b8, "ax"
	.global Battle_CollectPartyCommands
	.type Battle_CollectPartyCommands, %function
	.thumb_func
Battle_CollectPartyCommands:
	.incbin "baserom.gba", 0x000260b8, 0x00001080
	.section .rom.00027138, "ax"
	.global AffineEffect_UpdateFrame
	.type AffineEffect_UpdateFrame, %function
	.thumb_func
AffineEffect_UpdateFrame:
	.incbin "baserom.gba", 0x00027138, 0x00000348
	.section .rom.0002850c, "ax"
	.global DebugMenu_BrowseIcons
	.type DebugMenu_BrowseIcons, %function
	.thumb_func
DebugMenu_BrowseIcons:
	.incbin "baserom.gba", 0x0002850c, 0x00000228
	.section .rom.00028734, "ax"
	.global DebugMenu_BrowseEntryGlyphs
	.type DebugMenu_BrowseEntryGlyphs, %function
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	.incbin "baserom.gba", 0x00028734, 0x00000194
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x000288c8, 0x00000100
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.global RomBytes_08029a10
RomBytes_08029a10:
	.incbin "baserom.gba", 0x000289c8, 0x000000bc
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.incbin "baserom.gba", 0x00028a84, 0x0000009c
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.incbin "baserom.gba", 0x00028b20, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00028db8, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00028e9c, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x00029298, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002ce40, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002d0c0, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002f858, 0x00000804
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x0003005c, 0x00000740
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x0003079c, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x0003081c, 0x000006b0
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x00030ecc, 0x00000400
	.section .rom.00032ecc, "ax"
	.incbin "baserom.gba", 0x00032ecc, 0x0000002c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00032ef8, 0x000000c0
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00032fb8, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000333b8, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x000337b8, 0x00002000
	.global Data_080366f8
Data_080366f8:
	.incbin "baserom.gba", 0x000357b8, 0x00000058
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00035810, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x00035889, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x0003588c, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x0003588e, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x00035890, 0x00000006
	.global Data_080367d6
Data_080367d6:
	.incbin "baserom.gba", 0x00035896, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x0003589c, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x000358a4, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x000358cc, 0x000009cc
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00036298, 0x00000028
	.global Data_080371fe
Data_080371fe:
	.incbin "baserom.gba", 0x000362c0, 0x00000008
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x000362c8, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x000362d8, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x000362e8, 0x00000008
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x000362f0, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00036310, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00036340, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x00036380, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x000363c0, 0x000000ef
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.incbin "baserom.gba", 0x000364af, 0x00000008
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000364b7, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x000364c3, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x000364cf, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x000364e8, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x000364ec, 0x00000038
	.section .rom.00073a6c, "ax"
	.incbin "baserom.gba", 0x00073a6c, 0x0000000a
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073a76, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x00073ab8, 0x00000114
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x00073bcc, 0x00008434
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
	.section .rom.0007e460, "ax"
	.global BattleUnit_Assign
	.type BattleUnit_Assign, %function
	.thumb_func
BattleUnit_Assign:
	.incbin "baserom.gba", 0x0007e460, 0x0000019c
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
	.section .rom.000903f2, "ax"
	.incbin "baserom.gba", 0x000903f2, 0x00000002
	.section .rom.000903f4, "ax"
	.global Event_SpawnObjectTable
	.type Event_SpawnObjectTable, %function
	.thumb_func
Event_SpawnObjectTable:
	.incbin "baserom.gba", 0x000903f4, 0x00000260
	.section .rom.00090b34, "ax"
	.global ObjectTable_Restore
	.type ObjectTable_Restore, %function
	.thumb_func
ObjectTable_Restore:
	.incbin "baserom.gba", 0x00090b34, 0x00000118
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
	.section .rom.00094f68, "ax"
	.global DisplayTransition_Start
	.type DisplayTransition_Start, %function
	.thumb_func
DisplayTransition_Start:
	.incbin "baserom.gba", 0x00094f68, 0x000002c4
	.section .rom.00095ac8, "ax"
	.global BattleFx_BuildBuffer
	.type BattleFx_BuildBuffer, %function
	.thumb_func
BattleFx_BuildBuffer:
	.incbin "baserom.gba", 0x00095ac8, 0x00000718
	.section .rom.00096324, "ax"
	.global Object_EffectSpawnCallback
	.type Object_EffectSpawnCallback, %function
	.thumb_func
Object_EffectSpawnCallback:
	.incbin "baserom.gba", 0x00096324, 0x000001dc
	.section .rom.00097ccc, "ax"
	.global UiText_OpenMessageAtObject
	.type UiText_OpenMessageAtObject, %function
	.thumb_func
UiText_OpenMessageAtObject:
	.incbin "baserom.gba", 0x00097ccc, 0x00000344
	.section .rom.0009902c, "ax"
	.global battle_owner_69
	.type battle_owner_69, %function
	.thumb_func
battle_owner_69:
	.incbin "baserom.gba", 0x0009902c, 0x000001b4
	.section .rom.000995d0, "ax"
	.global DisplayScroll_BuildAndSwapHBlankPage
	.type DisplayScroll_BuildAndSwapHBlankPage, %function
	.thumb_func
DisplayScroll_BuildAndSwapHBlankPage:
	.incbin "baserom.gba", 0x000995d0, 0x000001ec
	.section .rom.000998ac, "ax"
	.incbin "baserom.gba", 0x000998ac, 0x00000188
	.section .rom.00099b54, "ax"
	.global Unnamed_08094ac8
	.type Unnamed_08094ac8, %function
	.thumb_func
Unnamed_08094ac8:
	.incbin "baserom.gba", 0x00099b54, 0x000000f4
	.section .rom.00099c48, "ax"
	.global Unnamed_08094bbc
	.type Unnamed_08094bbc, %function
	.thumb_func
Unnamed_08094bbc:
	.incbin "baserom.gba", 0x00099c48, 0x000001e4
	.section .rom.0009c6d0, "ax"
	.global Func_08097644
	.type Func_08097644, %function
	.thumb_func
Func_08097644:
	.incbin "baserom.gba", 0x0009c6d0, 0x00000224
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
	.section .rom.0009fc4c, "ax"
	.global BattleEffect_RunFallbackObjectTransition
	.type BattleEffect_RunFallbackObjectTransition, %function
	.thumb_func
BattleEffect_RunFallbackObjectTransition:
	.incbin "baserom.gba", 0x0009fc4c, 0x000001bc
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
	.incbin "baserom.gba", 0x000a3ed8, 0x000002a4
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
	.incbin "baserom.gba", 0x000a5098, 0x000000de
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
	.section .rom.000a62be, "ax"
	.incbin "baserom.gba", 0x000a62be, 0x00000002
	.section .rom.000a62c0, "ax"
	.global UiMenu_SlideCursor
	.type UiMenu_SlideCursor, %function
	.thumb_func
UiMenu_SlideCursor:
	.incbin "baserom.gba", 0x000a62c0, 0x00000108
	.section .rom.000a6cd0, "ax"
	.global RunAssetSelectionScreen
	.type RunAssetSelectionScreen, %function
	.thumb_func
RunAssetSelectionScreen:
	.incbin "baserom.gba", 0x000a6cd0, 0x000001b0
	.section .rom.000a895a, "ax"
	.incbin "baserom.gba", 0x000a895a, 0x00000002
	.section .rom.000a895c, "ax"
	.global Func_080a414c
	.type Func_080a414c, %function
	.thumb_func
Func_080a414c:
	.incbin "baserom.gba", 0x000a895c, 0x00000340
	.section .rom.000a9718, "ax"
	.global Func_080a4f08
Func_080a4f08:
	.incbin "baserom.gba", 0x000a9718, 0x000002c8
	.section .rom.000a9b98, "ax"
	.global Unnamed_080a5388
	.type Unnamed_080a5388, %function
	.thumb_func
Unnamed_080a5388:
	.incbin "baserom.gba", 0x000a9b98, 0x000001ac
	.section .rom.000aa54c, "ax"
	.global Menu_ResolveSelectedAction
	.type Menu_ResolveSelectedAction, %function
	.thumb_func
Menu_ResolveSelectedAction:
	.incbin "baserom.gba", 0x000aa54c, 0x00000320
	.section .rom.000aaea0, "ax"
	.global Func_080a6614
	.type Func_080a6614, %function
	.thumb_func
Func_080a6614:
	.incbin "baserom.gba", 0x000aaea0, 0x00000180
	.section .rom.000ace90, "ax"
	.global CharacterMenu_DrawStatusAilments
	.type CharacterMenu_DrawStatusAilments, %function
	.thumb_func
CharacterMenu_DrawStatusAilments:
	.incbin "baserom.gba", 0x000ace90, 0x00000300
	.section .rom.000aeff4, "ax"
	.global Func_080aa768
Func_080aa768:
	.incbin "baserom.gba", 0x000aeff4, 0x0000051c
	.section .rom.000af844, "ax"
	.global Func_080aafb8
Func_080aafb8:
	.incbin "baserom.gba", 0x000af844, 0x0000023c
	.section .rom.000afe70, "ax"
	.incbin "baserom.gba", 0x000afe70, 0x000012fc
	.section .rom.000b1328, "ax"
	.global DjinnMenu_DrawStatPreview
	.type DjinnMenu_DrawStatPreview, %function
	.thumb_func
DjinnMenu_DrawStatPreview:
	.incbin "baserom.gba", 0x000b1328, 0x000007bc
	.section .rom.000b1c7c, "ax"
	.global FourObjectMotion_UpdateBottomRow
	.type FourObjectMotion_UpdateBottomRow, %function
	.thumb_func
FourObjectMotion_UpdateBottomRow:
	.incbin "baserom.gba", 0x000b1c7c, 0x000000fc
	.section .rom.000b1f44, "ax"
	.incbin "baserom.gba", 0x000b1f44, 0x00001040
	.section .rom.000b32bc, "ax"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000b32bc, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000b33bc, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000b343c, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000b35bc, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000b363c, 0x00000440
	.global Data_080af20c
Data_080af20c:
	.incbin "baserom.gba", 0x000b3a7c, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000b3a80, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000b3a84, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000b3a88, 0x00000004
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000b3a8c, 0x00000004
	.global Data_080af220
Data_080af220:
	.incbin "baserom.gba", 0x000b3a90, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000b3a94, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000b3a98, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000b3a9c, 0x00000004
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000b3aa0, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000b3aa4, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000b3aa8, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000b3aac, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000b3adc, 0x00000028
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000b3b04, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000b3b0d, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000b3b16, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000b3b21, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000b3b2c, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000b3b40, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000b3b54, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000b3b6c, 0x00000008
	.global RomBytes_080af304
RomBytes_080af304:
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.incbin "baserom.gba", 0x000b3b74, 0x0000048c
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
	.section .rom.000bb5f4, "ax"
	.incbin "baserom.gba", 0x000bb5f4, 0x00000130
	.section .rom.000bb750, "ax"
	.incbin "baserom.gba", 0x000bb750, 0x000001ac
	.section .rom.000bbb82, "ax"
	.incbin "baserom.gba", 0x000bbb82, 0x00000002
	.section .rom.000bbb84, "ax"
	.global BattleActor_SpawnObjectsForList
	.type BattleActor_SpawnObjectsForList, %function
	.thumb_func
BattleActor_SpawnObjectsForList:
	.incbin "baserom.gba", 0x000bbb84, 0x00000264
	.section .rom.000bcc34, "ax"
	.global BattlePres_RunUnitAction
	.type BattlePres_RunUnitAction, %function
	.thumb_func
BattlePres_RunUnitAction:
	.incbin "baserom.gba", 0x000bcc34, 0x0000019c
	.section .rom.000bdb46, "ax"
	.incbin "baserom.gba", 0x000bdb46, 0x00000206
	.section .rom.000bded8, "ax"
	.incbin "baserom.gba", 0x000bded8, 0x000003bc
	.section .rom.000be6c4, "ax"
	.incbin "baserom.gba", 0x000be6c4, 0x0000026c
	.section .rom.000be98e, "ax"
	.incbin "baserom.gba", 0x000be98e, 0x00000266
	.section .rom.000bec84, "ax"
	.global BattleActor_RemoveFromLists
	.type BattleActor_RemoveFromLists, %function
	.thumb_func
BattleActor_RemoveFromLists:
	.incbin "baserom.gba", 0x000bec84, 0x0000007c
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
	.section .rom.000c21a2, "ax"
	.incbin "baserom.gba", 0x000c21a2, 0x0000107e
	.section .rom.000c3bbc, "ax"
	.incbin "baserom.gba", 0x000c3bbc, 0x00000414
	.section .rom.000c42bc, "ax"
	.incbin "baserom.gba", 0x000c42bc, 0x0000045c
	.section .rom.000c57ae, "ax"
	.incbin "baserom.gba", 0x000c57ae, 0x00000002
	.section .rom.000c57b0, "ax"
	.global BattleFx_PlayUnitElementEffect
	.type BattleFx_PlayUnitElementEffect, %function
	.thumb_func
BattleFx_PlayUnitElementEffect:
	.incbin "baserom.gba", 0x000c57b0, 0x0000027c
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
	.incbin "baserom.gba", 0x000c6b80, 0x00000a54
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
	.incbin "baserom.gba", 0x000c9a48, 0x000001e0
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
	.section .rom.000cc9dc, "ax"
	.incbin "baserom.gba", 0x000cc9dc, 0x00000a84
	.section .rom.000cd4a8, "ax"
	.global BattleFx_RunFiveMode
	.type BattleFx_RunFiveMode, %function
	.thumb_func
BattleFx_RunFiveMode:
	.incbin "baserom.gba", 0x000cd4a8, 0x0000053c
	.section .rom.000cd9fc, "ax"
	.global BattleFx_RunParticlePool
	.type BattleFx_RunParticlePool, %function
	.thumb_func
BattleFx_RunParticlePool:
	.incbin "baserom.gba", 0x000cd9fc, 0x00000380
	.section .rom.000cde0c, "ax"
	.global BattleFx_RunTwelveMode
	.type BattleFx_RunTwelveMode, %function
	.thumb_func
BattleFx_RunTwelveMode:
	.incbin "baserom.gba", 0x000cde0c, 0x00000b98
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
	.section .rom.000d27f8, "ax"
	.global BattleFx_RunFortyEightFrameEffect
	.type BattleFx_RunFortyEightFrameEffect, %function
	.thumb_func
BattleFx_RunFortyEightFrameEffect:
	.incbin "baserom.gba", 0x000d27f8, 0x000002a8
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
	.section .rom.000d3dfc, "ax"
	.incbin "baserom.gba", 0x000d3dfc, 0x00001118
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
	.section .rom.000d6598, "ax"
	.global BattleEffect_RunEmberColumns
	.type BattleEffect_RunEmberColumns, %function
	.thumb_func
BattleEffect_RunEmberColumns:
	.incbin "baserom.gba", 0x000d6598, 0x00001354
	.section .rom.000d79a4, "ax"
	.incbin "baserom.gba", 0x000d79a4, 0x00000448
	.section .rom.000d7e04, "ax"
	.global BattleFx_RunSparkGroups
	.type BattleFx_RunSparkGroups, %function
	.thumb_func
BattleFx_RunSparkGroups:
	.incbin "baserom.gba", 0x000d7e04, 0x00000c54
	.section .rom.000d8ac8, "ax"
	.global BattleFx_RenderMode
	.type BattleFx_RenderMode, %function
	.thumb_func
BattleFx_RenderMode:
	.incbin "baserom.gba", 0x000d8ac8, 0x000006e8
	.section .rom.000d9654, "ax"
	.incbin "baserom.gba", 0x000d9654, 0x000006b0
	.section .rom.000da170, "ax"
	.global BattleEffect_RunDitherDissolveScene
	.type BattleEffect_RunDitherDissolveScene, %function
	.thumb_func
BattleEffect_RunDitherDissolveScene:
	.incbin "baserom.gba", 0x000da170, 0x00000cec
	.section .rom.000dbab0, "ax"
	.incbin "baserom.gba", 0x000dbab0, 0x00000698
	.section .rom.000dc1ac, "ax"
	.global BattleEffectA
	.type BattleEffectA, %function
	.thumb_func
BattleEffectA:
	.incbin "baserom.gba", 0x000dc1ac, 0x000007e8
	.section .rom.000dc9dc, "ax"
	.global BattleEffectB
	.type BattleEffectB, %function
	.thumb_func
BattleEffectB:
	.incbin "baserom.gba", 0x000dc9dc, 0x000008dc
	.section .rom.000dd2e8, "ax"
	.global RunPaletteRampEffect
	.type RunPaletteRampEffect, %function
	.thumb_func
RunPaletteRampEffect:
	.incbin "baserom.gba", 0x000dd2e8, 0x000004e0
	.section .rom.000ddaac, "ax"
	.incbin "baserom.gba", 0x000ddaac, 0x0000141c
	.section .rom.000deee0, "ax"
	.global RunParticleFieldEffect
	.type RunParticleFieldEffect, %function
	.thumb_func
RunParticleFieldEffect:
	.incbin "baserom.gba", 0x000deee0, 0x00000444
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
	.section .rom.000e7fb8, "ax"
	.global BattleFx_RunCastingImpact
	.type BattleFx_RunCastingImpact, %function
	.thumb_func
BattleFx_RunCastingImpact:
	.incbin "baserom.gba", 0x000e7fb8, 0x00002190
	.section .rom.000ea18c, "ax"
	.incbin "baserom.gba", 0x000ea18c, 0x000003b0
	.section .rom.000ea6ac, "ax"
	.incbin "baserom.gba", 0x000ea6ac, 0x000003d0
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
	.incbin "baserom.gba", 0x000eba3c, 0x00001e9c
	.section .rom.000ed8d8, "ax"
	.global Unnamed_080ea0d8
	.type Unnamed_080ea0d8, %function
	.thumb_func
Unnamed_080ea0d8:
	.incbin "baserom.gba", 0x000ed8d8, 0x0000167c
	.section .rom.000eef54, "ax"
	.global Unnamed_080eb754
	.type Unnamed_080eb754, %function
	.thumb_func
Unnamed_080eb754:
	.incbin "baserom.gba", 0x000eef54, 0x0000098c
	.section .rom.000f1278, "ax"
	.incbin "baserom.gba", 0x000f1278, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000f1280, 0x00000038
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000f12b8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000f12c0, 0x00000028
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
	.incbin "baserom.gba", 0x000f16d6, 0x000000a9
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
	.incbin "baserom.gba", 0x000f1888, 0x00000084
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000f190c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000f191a, 0x0000019a
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
	.incbin "baserom.gba", 0x000f21f2, 0x0000014e
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
	.incbin "baserom.gba", 0x000f25f4, 0x0000002a
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
	.incbin "baserom.gba", 0x000f264e, 0x0000011a
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
	.section .rom.000f37f0, "ax"
	.global Func_080f07f0
Func_080f07f0:
	.incbin "baserom.gba", 0x000f37f0, 0x0000026c
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f3a5c, 0x00000848
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f42a4, 0x00000d5c
	.section .rom.000f5028, "ax"
	.incbin "baserom.gba", 0x000f5028, 0x000006c4
	.section .rom.000f56ec, "ax"
	.global Func_080f26ec
	.type Func_080f26ec, %function
	.thumb_func
Func_080f26ec:
	.incbin "baserom.gba", 0x000f56ec, 0x00000480
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
	.incbin "baserom.gba", 0x000f68bc, 0x00000744
	.section .rom.000f7168, "ax"
	.global LuckyDice_Run
	.type LuckyDice_Run, %function
	.thumb_func
LuckyDice_Run:
	.incbin "baserom.gba", 0x000f7168, 0x00001e98
	.section .rom.000f9440, "ax"
	.incbin "baserom.gba", 0x000f9440, 0x00000eec
	.section .rom.000fa470, "ax"
	.global Unnamed_080f7460
	.type Unnamed_080f7460, %function
	.thumb_func
Unnamed_080f7460:
	.incbin "baserom.gba", 0x000fa470, 0x00000954
	.section .rom.000faf88, "ax"
	.incbin "baserom.gba", 0x000faf88, 0x000007be
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000fb746, 0x000000ba
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
	.section .rom.00338909, "ax"
	.incbin "baserom.gba", 0x00338909, 0x00000003
	.section .rom.0033c40d, "ax"
	.incbin "baserom.gba", 0x0033c40d, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c410, 0x000006a8
	.section .rom.00340efa, "ax"
	.incbin "baserom.gba", 0x00340efa, 0x00000002
	.section .rom.0035166a, "ax"
	.incbin "baserom.gba", 0x0035166a, 0x00000002
	.section .rom.00354c5a, "ax"
	.incbin "baserom.gba", 0x00354c5a, 0x00000002
	.section .rom.003606fe, "ax"
	.incbin "baserom.gba", 0x003606fe, 0x00000002
	.section .rom.00370dae, "ax"
	.incbin "baserom.gba", 0x00370dae, 0x00000002
	.section .rom.0037884e, "ax"
	.incbin "baserom.gba", 0x0037884e, 0x00000002
	.section .rom.0037c08e, "ax"
	.incbin "baserom.gba", 0x0037c08e, 0x00000002
	.section .rom.003854c2, "ax"
	.incbin "baserom.gba", 0x003854c2, 0x00000002
	.section .rom.003908ea, "ax"
	.incbin "baserom.gba", 0x003908ea, 0x00000002
	.section .rom.003987a6, "ax"
	.incbin "baserom.gba", 0x003987a6, 0x00000002
	.section .rom.0039d236, "ax"
	.incbin "baserom.gba", 0x0039d236, 0x00000002
	.section .rom.003a14ea, "ax"
	.incbin "baserom.gba", 0x003a14ea, 0x00000002
	.section .rom.003a4e6a, "ax"
	.incbin "baserom.gba", 0x003a4e6a, 0x00000002
	.section .rom.003b16a6, "ax"
	.incbin "baserom.gba", 0x003b16a6, 0x00000002
	.section .rom.003bffee, "ax"
	.incbin "baserom.gba", 0x003bffee, 0x00000002
	.section .rom.003c58f1, "ax"
	.incbin "baserom.gba", 0x003c58f1, 0x00000003
	.section .rom.003c7273, "ax"
	.incbin "baserom.gba", 0x003c7273, 0x00000001
	.section .rom.003ca091, "ax"
	.incbin "baserom.gba", 0x003ca091, 0x00000003
	.section .rom.003cd23b, "ax"
	.incbin "baserom.gba", 0x003cd23b, 0x00000001
	.section .rom.003cda81, "ax"
	.incbin "baserom.gba", 0x003cda81, 0x00000003
	.section .rom.003cf297, "ax"
	.incbin "baserom.gba", 0x003cf297, 0x00000001
	.section .rom.003cf882, "ax"
	.incbin "baserom.gba", 0x003cf882, 0x00000002
	.section .rom.003cff46, "ax"
	.incbin "baserom.gba", 0x003cff46, 0x00000002
	.section .rom.003d022d, "ax"
	.incbin "baserom.gba", 0x003d022d, 0x00000003
	.section .rom.003d1593, "ax"
	.incbin "baserom.gba", 0x003d1593, 0x00000001
	.section .rom.003d197d, "ax"
	.incbin "baserom.gba", 0x003d197d, 0x00000003
	.section .rom.003d1d4f, "ax"
	.incbin "baserom.gba", 0x003d1d4f, 0x00000001
	.section .rom.003d25ef, "ax"
	.incbin "baserom.gba", 0x003d25ef, 0x00000001
	.section .rom.003d2a92, "ax"
	.incbin "baserom.gba", 0x003d2a92, 0x00000002
	.section .rom.003d3c4b, "ax"
	.incbin "baserom.gba", 0x003d3c4b, 0x00000001
	.section .rom.003d510e, "ax"
	.incbin "baserom.gba", 0x003d510e, 0x00000002
	.section .rom.003d60d7, "ax"
	.incbin "baserom.gba", 0x003d60d7, 0x00000001
	.section .rom.003d6332, "ax"
	.incbin "baserom.gba", 0x003d6332, 0x00000002
	.section .rom.003d7d8d, "ax"
	.incbin "baserom.gba", 0x003d7d8d, 0x00000003
	.section .rom.003d82d9, "ax"
	.incbin "baserom.gba", 0x003d82d9, 0x00000003
	.section .rom.003da062, "ax"
	.incbin "baserom.gba", 0x003da062, 0x00000002
	.section .rom.003da2db, "ax"
	.incbin "baserom.gba", 0x003da2db, 0x00000001
	.section .rom.003da7a2, "ax"
	.incbin "baserom.gba", 0x003da7a2, 0x00000002
	.section .rom.003dc377, "ax"
	.incbin "baserom.gba", 0x003dc377, 0x00000001
	.section .rom.003ddf75, "ax"
	.incbin "baserom.gba", 0x003ddf75, 0x00000003
	.section .rom.003de196, "ax"
	.incbin "baserom.gba", 0x003de196, 0x00000002
	.section .rom.003de5d3, "ax"
	.incbin "baserom.gba", 0x003de5d3, 0x00000001
	.section .rom.003de6e5, "ax"
	.incbin "baserom.gba", 0x003de6e5, 0x00000003
	.section .rom.003dee63, "ax"
	.incbin "baserom.gba", 0x003dee63, 0x00000001
	.section .rom.003df301, "ax"
	.incbin "baserom.gba", 0x003df301, 0x00000003
	.section .rom.003e0016, "ax"
	.incbin "baserom.gba", 0x003e0016, 0x00000002
	.section .rom.003e0d96, "ax"
	.incbin "baserom.gba", 0x003e0d96, 0x00000002
	.section .rom.003e1127, "ax"
	.incbin "baserom.gba", 0x003e1127, 0x00000001
	.section .rom.003e2e6e, "ax"
	.incbin "baserom.gba", 0x003e2e6e, 0x00000002
	.section .rom.003e3c45, "ax"
	.incbin "baserom.gba", 0x003e3c45, 0x00000003
	.section .rom.003e4472, "ax"
	.incbin "baserom.gba", 0x003e4472, 0x00000002
	.section .rom.003e4afd, "ax"
	.incbin "baserom.gba", 0x003e4afd, 0x00000003
	.section .rom.003e4cbb, "ax"
	.incbin "baserom.gba", 0x003e4cbb, 0x00000001
	.section .rom.003e5a13, "ax"
	.incbin "baserom.gba", 0x003e5a13, 0x00000001
	.section .rom.003e5f5f, "ax"
	.incbin "baserom.gba", 0x003e5f5f, 0x00000001
	.section .rom.003e6339, "ax"
	.incbin "baserom.gba", 0x003e6339, 0x00000003
	.section .rom.003e66e7, "ax"
	.incbin "baserom.gba", 0x003e66e7, 0x00000001
	.section .rom.003e868b, "ax"
	.incbin "baserom.gba", 0x003e868b, 0x00000001
	.section .rom.003e9427, "ax"
	.incbin "baserom.gba", 0x003e9427, 0x00000001
	.section .rom.003e9643, "ax"
	.incbin "baserom.gba", 0x003e9643, 0x00000001
	.section .rom.003e993f, "ax"
	.incbin "baserom.gba", 0x003e993f, 0x00000001
	.section .rom.003ebaed, "ax"
	.incbin "baserom.gba", 0x003ebaed, 0x00000003
	.section .rom.003ec8bb, "ax"
	.incbin "baserom.gba", 0x003ec8bb, 0x00000001
	.section .rom.003eda1d, "ax"
	.incbin "baserom.gba", 0x003eda1d, 0x00000003
	.section .rom.003edf77, "ax"
	.incbin "baserom.gba", 0x003edf77, 0x00000001
	.section .rom.003ef861, "ax"
	.incbin "baserom.gba", 0x003ef861, 0x00000003
	.section .rom.003f0102, "ax"
	.incbin "baserom.gba", 0x003f0102, 0x00000002
	.section .rom.003f04df, "ax"
	.incbin "baserom.gba", 0x003f04df, 0x00000001
	.section .rom.003f0769, "ax"
	.incbin "baserom.gba", 0x003f0769, 0x00000003
	.section .rom.003f0b0f, "ax"
	.incbin "baserom.gba", 0x003f0b0f, 0x00000001
	.section .rom.003f0d6a, "ax"
	.incbin "baserom.gba", 0x003f0d6a, 0x00000002
	.section .rom.003f1122, "ax"
	.incbin "baserom.gba", 0x003f1122, 0x00000002
	.section .rom.003f260b, "ax"
	.incbin "baserom.gba", 0x003f260b, 0x00000001
	.section .rom.003f3bce, "ax"
	.incbin "baserom.gba", 0x003f3bce, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f3bd0, 0x00000a6c
	.section .rom.003f5412, "ax"
	.incbin "baserom.gba", 0x003f5412, 0x00000002
	.section .rom.003f5795, "ax"
	.incbin "baserom.gba", 0x003f5795, 0x00000003
	.section .rom.003f6445, "ax"
	.incbin "baserom.gba", 0x003f6445, 0x00000003
	.section .rom.003f6f8d, "ax"
	.incbin "baserom.gba", 0x003f6f8d, 0x00000003
	.section .rom.003f7126, "ax"
	.incbin "baserom.gba", 0x003f7126, 0x00000002
	.section .rom.003f79b3, "ax"
	.incbin "baserom.gba", 0x003f79b3, 0x00000001
	.section .rom.003f847a, "ax"
	.incbin "baserom.gba", 0x003f847a, 0x00000002
	.section .rom.003f8992, "ax"
	.incbin "baserom.gba", 0x003f8992, 0x00000002
	.section .rom.003f8fb6, "ax"
	.incbin "baserom.gba", 0x003f8fb6, 0x00000002
	.section .rom.003f981d, "ax"
	.incbin "baserom.gba", 0x003f981d, 0x00000003
	.section .rom.003f9c41, "ax"
	.incbin "baserom.gba", 0x003f9c41, 0x00000003
	.section .rom.003f9edb, "ax"
	.incbin "baserom.gba", 0x003f9edb, 0x00000001
	.section .rom.003fb877, "ax"
	.incbin "baserom.gba", 0x003fb877, 0x00000001
	.section .rom.003fd5ee, "ax"
	.incbin "baserom.gba", 0x003fd5ee, 0x00000002
	.section .rom.003ff563, "ax"
	.incbin "baserom.gba", 0x003ff563, 0x00000001
	.section .rom.003ffa33, "ax"
	.incbin "baserom.gba", 0x003ffa33, 0x00000001
	.section .rom.004000c5, "ax"
	.incbin "baserom.gba", 0x004000c5, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x004000c8, 0x00000a34
	.section .rom.00401ecd, "ax"
	.incbin "baserom.gba", 0x00401ecd, 0x00000003
	.section .rom.0040299e, "ax"
	.incbin "baserom.gba", 0x0040299e, 0x00000002
	.section .rom.004034db, "ax"
	.incbin "baserom.gba", 0x004034db, 0x00000001
	.section .rom.00403b1b, "ax"
	.incbin "baserom.gba", 0x00403b1b, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00403b1c, 0x00001588
	.section .rom.00405105, "ax"
	.incbin "baserom.gba", 0x00405105, 0x00000003
	.section .rom.00405473, "ax"
	.incbin "baserom.gba", 0x00405473, 0x00000001
	.section .rom.00405a11, "ax"
	.incbin "baserom.gba", 0x00405a11, 0x00000003
	.section .rom.00405d45, "ax"
	.incbin "baserom.gba", 0x00405d45, 0x00000003
	.section .rom.00406081, "ax"
	.incbin "baserom.gba", 0x00406081, 0x00000003
	.section .rom.0040709a, "ax"
	.incbin "baserom.gba", 0x0040709a, 0x00000002
	.section .rom.0040a6a6, "ax"
	.incbin "baserom.gba", 0x0040a6a6, 0x00000002
	.section .rom.0040ae49, "ax"
	.incbin "baserom.gba", 0x0040ae49, 0x00000003
	.section .rom.0040c1db, "ax"
	.incbin "baserom.gba", 0x0040c1db, 0x00000001
	.section .rom.0040c60b, "ax"
	.incbin "baserom.gba", 0x0040c60b, 0x00000001
	.section .rom.0040e401, "ax"
	.incbin "baserom.gba", 0x0040e401, 0x00000003
	.section .rom.0040ed16, "ax"
	.incbin "baserom.gba", 0x0040ed16, 0x00000002
	.section .rom.0041084a, "ax"
	.incbin "baserom.gba", 0x0041084a, 0x00000002
	.section .rom.00411899, "ax"
	.incbin "baserom.gba", 0x00411899, 0x00000003
	.section .rom.00411f4f, "ax"
	.incbin "baserom.gba", 0x00411f4f, 0x00000001
	.section .rom.00412ff5, "ax"
	.incbin "baserom.gba", 0x00412ff5, 0x00000003
	.section .rom.00426407, "ax"
	.incbin "baserom.gba", 0x00426407, 0x00000001
	.section .rom.00426559, "ax"
	.incbin "baserom.gba", 0x00426559, 0x00000003
	.section .rom.004269ea, "ax"
	.incbin "baserom.gba", 0x004269ea, 0x00000002
	.section .rom.00426be1, "ax"
	.incbin "baserom.gba", 0x00426be1, 0x00000003
	.section .rom.0042829e, "ax"
	.incbin "baserom.gba", 0x0042829e, 0x00000002
	.section .rom.004297bd, "ax"
	.incbin "baserom.gba", 0x004297bd, 0x00000003
	.section .rom.0042a389, "ax"
	.incbin "baserom.gba", 0x0042a389, 0x00000003
	.section .rom.0042aefe, "ax"
	.incbin "baserom.gba", 0x0042aefe, 0x00000002
	.section .rom.0042cda7, "ax"
	.incbin "baserom.gba", 0x0042cda7, 0x00000001
	.section .rom.0042e535, "ax"
	.incbin "baserom.gba", 0x0042e535, 0x00000003
	.section .rom.0042f9e6, "ax"
	.incbin "baserom.gba", 0x0042f9e6, 0x00000002
	.section .rom.00432133, "ax"
	.incbin "baserom.gba", 0x00432133, 0x00000001
	.section .rom.004339cd, "ax"
	.incbin "baserom.gba", 0x004339cd, 0x00000003
	.section .rom.00434f96, "ax"
	.incbin "baserom.gba", 0x00434f96, 0x00000002
	.section .rom.00436a26, "ax"
	.incbin "baserom.gba", 0x00436a26, 0x00000002
	.section .rom.004377db, "ax"
	.incbin "baserom.gba", 0x004377db, 0x00000001
	.section .rom.0044114a, "ax"
	.incbin "baserom.gba", 0x0044114a, 0x00000002
	.section .rom.004432fe, "ax"
	.incbin "baserom.gba", 0x004432fe, 0x00000002
	.section .rom.00447465, "ax"
	.incbin "baserom.gba", 0x00447465, 0x00000003
	.section .rom.0044a68a, "ax"
	.incbin "baserom.gba", 0x0044a68a, 0x00000002
	.section .rom.0044bd8e, "ax"
	.incbin "baserom.gba", 0x0044bd8e, 0x00000002
	.section .rom.00452976, "ax"
	.incbin "baserom.gba", 0x00452976, 0x00000002
	.section .rom.0045b2fa, "ax"
	.incbin "baserom.gba", 0x0045b2fa, 0x00000002
	.section .rom.00462dd6, "ax"
	.incbin "baserom.gba", 0x00462dd6, 0x00000002
	.section .rom.00465e6a, "ax"
	.incbin "baserom.gba", 0x00465e6a, 0x00000002
	.section .rom.00469279, "ax"
	.incbin "baserom.gba", 0x00469279, 0x00000003
	.section .rom.0046bae5, "ax"
	.incbin "baserom.gba", 0x0046bae5, 0x00000003
	.section .rom.0046d5d6, "ax"
	.incbin "baserom.gba", 0x0046d5d6, 0x00000002
	.section .rom.0046e3ee, "ax"
	.incbin "baserom.gba", 0x0046e3ee, 0x00000002
	.section .rom.0046f0d9, "ax"
	.incbin "baserom.gba", 0x0046f0d9, 0x00000003
	.section .rom.00478596, "ax"
	.incbin "baserom.gba", 0x00478596, 0x00000002
	.section .rom.004792a1, "ax"
	.incbin "baserom.gba", 0x004792a1, 0x00000003
	.section .rom.0047b3b5, "ax"
	.incbin "baserom.gba", 0x0047b3b5, 0x00000003
	.section .rom.0047c027, "ax"
	.incbin "baserom.gba", 0x0047c027, 0x00000001
	.section .rom.0047cc3f, "ax"
	.incbin "baserom.gba", 0x0047cc3f, 0x00000001
	.section .rom.0047d3b3, "ax"
	.incbin "baserom.gba", 0x0047d3b3, 0x00000001
	.section .rom.0047ec43, "ax"
	.incbin "baserom.gba", 0x0047ec43, 0x00000001
	.section .rom.0047f611, "ax"
	.incbin "baserom.gba", 0x0047f611, 0x00000003
	.section .rom.0048022f, "ax"
	.incbin "baserom.gba", 0x0048022f, 0x00000001
	.section .rom.004828e3, "ax"
	.incbin "baserom.gba", 0x004828e3, 0x00000001
	.section .rom.00484ff6, "ax"
	.incbin "baserom.gba", 0x00484ff6, 0x00000002
	.section .rom.00489f4b, "ax"
	.incbin "baserom.gba", 0x00489f4b, 0x00000001
	.section .rom.0048e275, "ax"
	.incbin "baserom.gba", 0x0048e275, 0x00000003
	.section .rom.0048ee62, "ax"
	.incbin "baserom.gba", 0x0048ee62, 0x00000002
	.section .rom.00493a17, "ax"
	.incbin "baserom.gba", 0x00493a17, 0x00000001
	.section .rom.00495d81, "ax"
	.incbin "baserom.gba", 0x00495d81, 0x00000003
	.section .rom.00497723, "ax"
	.incbin "baserom.gba", 0x00497723, 0x00000001
	.section .rom.0049bfe5, "ax"
	.incbin "baserom.gba", 0x0049bfe5, 0x00000003
	.section .rom.004a00ad, "ax"
	.incbin "baserom.gba", 0x004a00ad, 0x00000003
	.section .rom.004a3776, "ax"
	.incbin "baserom.gba", 0x004a3776, 0x00000002
	.section .rom.004ab747, "ax"
	.incbin "baserom.gba", 0x004ab747, 0x00000001
	.section .rom.004b2a57, "ax"
	.incbin "baserom.gba", 0x004b2a57, 0x00000001
	.section .rom.004b79d7, "ax"
	.incbin "baserom.gba", 0x004b79d7, 0x00000001
	.section .rom.004bc387, "ax"
	.incbin "baserom.gba", 0x004bc387, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004bc388, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004bc394, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bc4e4, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bc624, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bc764, 0x00000140
	.section .rom.004c1cb3, "ax"
	.incbin "baserom.gba", 0x004c1cb3, 0x00000001
	.section .rom.004c1e41, "ax"
	.incbin "baserom.gba", 0x004c1e41, 0x00000003
	.section .rom.004c6ea1, "ax"
	.incbin "baserom.gba", 0x004c6ea1, 0x00000003
	.section .rom.004cb40d, "ax"
	.incbin "baserom.gba", 0x004cb40d, 0x00000003
	.section .rom.004d05d1, "ax"
	.incbin "baserom.gba", 0x004d05d1, 0x00000003
	.section .rom.004d0767, "ax"
	.incbin "baserom.gba", 0x004d0767, 0x00000001
	.section .rom.004d33bb, "ax"
	.incbin "baserom.gba", 0x004d33bb, 0x00000001
	.section .rom.004da06f, "ax"
	.incbin "baserom.gba", 0x004da06f, 0x00000001
	.section .rom.004dd1aa, "ax"
	.incbin "baserom.gba", 0x004dd1aa, 0x00000002
	.section .rom.004dd32b, "ax"
	.incbin "baserom.gba", 0x004dd32b, 0x00000001
	.section .rom.004dfae5, "ax"
	.incbin "baserom.gba", 0x004dfae5, 0x00000003
	.section .rom.004e212e, "ax"
	.incbin "baserom.gba", 0x004e212e, 0x00000002
	.section .rom.004e4812, "ax"
	.incbin "baserom.gba", 0x004e4812, 0x00000002
	.section .rom.004e5887, "ax"
	.incbin "baserom.gba", 0x004e5887, 0x00000001
	.section .rom.004e79da, "ax"
	.incbin "baserom.gba", 0x004e79da, 0x00000002
	.section .rom.004e7b6d, "ax"
	.incbin "baserom.gba", 0x004e7b6d, 0x00000003
	.section .rom.004ea3af, "ax"
	.incbin "baserom.gba", 0x004ea3af, 0x00000001
	.section .rom.004ec1c5, "ax"
	.incbin "baserom.gba", 0x004ec1c5, 0x00000003
	.section .rom.004ee8a6, "ax"
	.incbin "baserom.gba", 0x004ee8a6, 0x00000002
	.section .rom.004ef9ed, "ax"
	.incbin "baserom.gba", 0x004ef9ed, 0x00000003
	.section .rom.004f2b01, "ax"
	.incbin "baserom.gba", 0x004f2b01, 0x00000003
	.section .rom.004f6c69, "ax"
	.incbin "baserom.gba", 0x004f6c69, 0x00000003
	.section .rom.004f8266, "ax"
	.incbin "baserom.gba", 0x004f8266, 0x00000002
	.section .rom.004f952f, "ax"
	.incbin "baserom.gba", 0x004f952f, 0x00000001
	.section .rom.004fc947, "ax"
	.incbin "baserom.gba", 0x004fc947, 0x00000001
	.section .rom.004fcad5, "ax"
	.incbin "baserom.gba", 0x004fcad5, 0x00000003
	.section .rom.004fe886, "ax"
	.incbin "baserom.gba", 0x004fe886, 0x00000002
	.section .rom.005023fd, "ax"
	.incbin "baserom.gba", 0x005023fd, 0x00000003
	.section .rom.0050390b, "ax"
	.incbin "baserom.gba", 0x0050390b, 0x00000001
	.section .rom.005044df, "ax"
	.incbin "baserom.gba", 0x005044df, 0x00000001
	.section .rom.005045dd, "ax"
	.incbin "baserom.gba", 0x005045dd, 0x00000003
	.section .rom.005059c2, "ax"
	.incbin "baserom.gba", 0x005059c2, 0x00000002
	.section .rom.00506b45, "ax"
	.incbin "baserom.gba", 0x00506b45, 0x00000003
	.section .rom.005074ce, "ax"
	.incbin "baserom.gba", 0x005074ce, 0x00000002
	.section .rom.0050a14b, "ax"
	.incbin "baserom.gba", 0x0050a14b, 0x00000001
	.section .rom.0050a22e, "ax"
	.incbin "baserom.gba", 0x0050a22e, 0x00000002
	.section .rom.0050b419, "ax"
	.incbin "baserom.gba", 0x0050b419, 0x00000003
	.section .rom.0050d0a1, "ax"
	.incbin "baserom.gba", 0x0050d0a1, 0x00000003
	.section .rom.0050d5af, "ax"
	.incbin "baserom.gba", 0x0050d5af, 0x00000001
	.section .rom.0050d6ef, "ax"
	.incbin "baserom.gba", 0x0050d6ef, 0x00000001
	.section .rom.0050e90a, "ax"
	.incbin "baserom.gba", 0x0050e90a, 0x00000002
	.section .rom.0050ea65, "ax"
	.incbin "baserom.gba", 0x0050ea65, 0x00000003
	.section .rom.00512e8b, "ax"
	.incbin "baserom.gba", 0x00512e8b, 0x00000001
	.section .rom.00514bba, "ax"
	.incbin "baserom.gba", 0x00514bba, 0x00000002
	.section .rom.00515933, "ax"
	.incbin "baserom.gba", 0x00515933, 0x00000001
	.section .rom.00515a67, "ax"
	.incbin "baserom.gba", 0x00515a67, 0x00000001
	.section .rom.00517eea, "ax"
	.incbin "baserom.gba", 0x00517eea, 0x00000002
	.section .rom.00518c67, "ax"
	.incbin "baserom.gba", 0x00518c67, 0x00000001
	.section .rom.00519746, "ax"
	.incbin "baserom.gba", 0x00519746, 0x00000002
	.section .rom.0051a5be, "ax"
	.incbin "baserom.gba", 0x0051a5be, 0x00000002
	.section .rom.0051c692, "ax"
	.incbin "baserom.gba", 0x0051c692, 0x00000002
	.section .rom.0051cfd9, "ax"
	.incbin "baserom.gba", 0x0051cfd9, 0x00000003
	.section .rom.0051dc26, "ax"
	.incbin "baserom.gba", 0x0051dc26, 0x00000002
	.section .rom.0051de4d, "ax"
	.incbin "baserom.gba", 0x0051de4d, 0x00000003
	.section .rom.0051f78f, "ax"
	.incbin "baserom.gba", 0x0051f78f, 0x00000001
	.section .rom.0051f8a3, "ax"
	.incbin "baserom.gba", 0x0051f8a3, 0x00000001
	.section .rom.00521269, "ax"
	.incbin "baserom.gba", 0x00521269, 0x00000003
	.section .rom.005213b3, "ax"
	.incbin "baserom.gba", 0x005213b3, 0x00000001
	.section .rom.0052347f, "ax"
	.incbin "baserom.gba", 0x0052347f, 0x00000001
	.section .rom.005235a7, "ax"
	.incbin "baserom.gba", 0x005235a7, 0x00000001
	.section .rom.00524e2f, "ax"
	.incbin "baserom.gba", 0x00524e2f, 0x00000001
	.section .rom.005270d1, "ax"
	.incbin "baserom.gba", 0x005270d1, 0x00000003
	.section .rom.00527213, "ax"
	.incbin "baserom.gba", 0x00527213, 0x00000001
	.section .rom.0052921b, "ax"
	.incbin "baserom.gba", 0x0052921b, 0x00000001
	.section .rom.00529326, "ax"
	.incbin "baserom.gba", 0x00529326, 0x00000002
	.section .rom.0052b30a, "ax"
	.incbin "baserom.gba", 0x0052b30a, 0x00000002
	.section .rom.0052ca2f, "ax"
	.incbin "baserom.gba", 0x0052ca2f, 0x00000001
	.section .rom.0052dae3, "ax"
	.incbin "baserom.gba", 0x0052dae3, 0x00000001
	.section .rom.0052f7ca, "ax"
	.incbin "baserom.gba", 0x0052f7ca, 0x00000002
	.section .rom.0052f925, "ax"
	.incbin "baserom.gba", 0x0052f925, 0x00000003
	.section .rom.0053186e, "ax"
	.incbin "baserom.gba", 0x0053186e, 0x00000002
	.section .rom.005319f3, "ax"
	.incbin "baserom.gba", 0x005319f3, 0x00000001
	.section .rom.00534545, "ax"
	.incbin "baserom.gba", 0x00534545, 0x00000003
	.section .rom.00536c0a, "ax"
	.incbin "baserom.gba", 0x00536c0a, 0x00000002
	.section .rom.00539362, "ax"
	.incbin "baserom.gba", 0x00539362, 0x00000002
	.section .rom.0053abb5, "ax"
	.incbin "baserom.gba", 0x0053abb5, 0x00000003
	.section .rom.0053d2b5, "ax"
	.incbin "baserom.gba", 0x0053d2b5, 0x00000003
	.section .rom.0053f081, "ax"
	.incbin "baserom.gba", 0x0053f081, 0x00000003
	.section .rom.005415cb, "ax"
	.incbin "baserom.gba", 0x005415cb, 0x00000001
	.section .rom.00542896, "ax"
	.incbin "baserom.gba", 0x00542896, 0x00000002
	.section .rom.00543547, "ax"
	.incbin "baserom.gba", 0x00543547, 0x00000001
	.section .rom.00543fb1, "ax"
	.incbin "baserom.gba", 0x00543fb1, 0x00000003
	.section .rom.005440a7, "ax"
	.incbin "baserom.gba", 0x005440a7, 0x00000001
	.section .rom.00545b06, "ax"
	.incbin "baserom.gba", 0x00545b06, 0x00000002
	.section .rom.005468aa, "ax"
	.incbin "baserom.gba", 0x005468aa, 0x00000002
	.section .rom.005483e3, "ax"
	.incbin "baserom.gba", 0x005483e3, 0x00000001
	.section .rom.0054b2d1, "ax"
	.incbin "baserom.gba", 0x0054b2d1, 0x00000003
	.section .rom.00552ab7, "ax"
	.incbin "baserom.gba", 0x00552ab7, 0x00000001
	.section .rom.00552b83, "ax"
	.incbin "baserom.gba", 0x00552b83, 0x00000001
	.section .rom.00558345, "ax"
	.incbin "baserom.gba", 0x00558345, 0x00000003
	.section .rom.00558487, "ax"
	.incbin "baserom.gba", 0x00558487, 0x00000001
	.section .rom.005598b5, "ax"
	.incbin "baserom.gba", 0x005598b5, 0x00000003
	.section .rom.0055c5f2, "ax"
	.incbin "baserom.gba", 0x0055c5f2, 0x00000002
	.section .rom.0055e6ee, "ax"
	.incbin "baserom.gba", 0x0055e6ee, 0x00000002
	.section .rom.005660fa, "ax"
	.incbin "baserom.gba", 0x005660fa, 0x00000002
	.section .rom.00566296, "ax"
	.incbin "baserom.gba", 0x00566296, 0x00000002
	.section .rom.0056ab31, "ax"
	.incbin "baserom.gba", 0x0056ab31, 0x00000003
	.section .rom.0056d032, "ax"
	.incbin "baserom.gba", 0x0056d032, 0x00000002
	.section .rom.00574a23, "ax"
	.incbin "baserom.gba", 0x00574a23, 0x00000001
	.section .rom.00574bbe, "ax"
	.incbin "baserom.gba", 0x00574bbe, 0x00000002
	.section .rom.0057776d, "ax"
	.incbin "baserom.gba", 0x0057776d, 0x00000003
	.section .rom.00579841, "ax"
	.incbin "baserom.gba", 0x00579841, 0x00000003
	.section .rom.0057b89a, "ax"
	.incbin "baserom.gba", 0x0057b89a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057b89c, 0x00001cec
	.section .rom.0057ed31, "ax"
	.incbin "baserom.gba", 0x0057ed31, 0x00000003
	.section .rom.00581a0d, "ax"
	.incbin "baserom.gba", 0x00581a0d, 0x00000003
	.section .rom.005837df, "ax"
	.incbin "baserom.gba", 0x005837df, 0x00000001
	.section .rom.0058391f, "ax"
	.incbin "baserom.gba", 0x0058391f, 0x00000001
	.section .rom.00586979, "ax"
	.incbin "baserom.gba", 0x00586979, 0x00000003
	.section .rom.005898f6, "ax"
	.incbin "baserom.gba", 0x005898f6, 0x00000002
	.section .rom.0058c1bd, "ax"
	.incbin "baserom.gba", 0x0058c1bd, 0x00000003
	.section .rom.00593125, "ax"
	.incbin "baserom.gba", 0x00593125, 0x00000003
	.section .rom.005949cb, "ax"
	.incbin "baserom.gba", 0x005949cb, 0x00000001
	.section .rom.005952ca, "ax"
	.incbin "baserom.gba", 0x005952ca, 0x00000002
	.section .rom.00596043, "ax"
	.incbin "baserom.gba", 0x00596043, 0x00000001
	.section .rom.005961a3, "ax"
	.incbin "baserom.gba", 0x005961a3, 0x00000001
	.section .rom.00597dfb, "ax"
	.incbin "baserom.gba", 0x00597dfb, 0x00000001
	.section .rom.00599693, "ax"
	.incbin "baserom.gba", 0x00599693, 0x00000001
	.section .rom.00599817, "ax"
	.incbin "baserom.gba", 0x00599817, 0x00000001
	.section .rom.0059c353, "ax"
	.incbin "baserom.gba", 0x0059c353, 0x00000001
	.section .rom.0059eacd, "ax"
	.incbin "baserom.gba", 0x0059eacd, 0x00000003
	.section .rom.0059faba, "ax"
	.incbin "baserom.gba", 0x0059faba, 0x00000002
	.section .rom.005a117b, "ax"
	.incbin "baserom.gba", 0x005a117b, 0x00000001
	.section .rom.005a1325, "ax"
	.incbin "baserom.gba", 0x005a1325, 0x00000003
	.section .rom.005a40b9, "ax"
	.incbin "baserom.gba", 0x005a40b9, 0x00000003
	.section .rom.005a814e, "ax"
	.incbin "baserom.gba", 0x005a814e, 0x00000002
	.section .rom.005a828f, "ax"
	.incbin "baserom.gba", 0x005a828f, 0x00000001
	.section .rom.005aa157, "ax"
	.incbin "baserom.gba", 0x005aa157, 0x00000001
	.section .rom.005aa2f9, "ax"
	.incbin "baserom.gba", 0x005aa2f9, 0x00000003
	.section .rom.005ac39b, "ax"
	.incbin "baserom.gba", 0x005ac39b, 0x00000001
	.section .rom.005ad328, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ad328, 0x000022d8
	.section .rom.005b20f1, "ax"
	.incbin "baserom.gba", 0x005b20f1, 0x00000003
	.section .rom.005b2276, "ax"
	.incbin "baserom.gba", 0x005b2276, 0x00000002
	.section .rom.005b99c3, "ax"
	.incbin "baserom.gba", 0x005b99c3, 0x00000001
	.section .rom.005b9ecd, "ax"
	.incbin "baserom.gba", 0x005b9ecd, 0x00000003
	.section .rom.005bb5a5, "ax"
	.incbin "baserom.gba", 0x005bb5a5, 0x00000003
	.section .rom.005bb749, "ax"
	.incbin "baserom.gba", 0x005bb749, 0x00000003
	.section .rom.005bd04e, "ax"
	.incbin "baserom.gba", 0x005bd04e, 0x00000002
	.section .rom.005be87b, "ax"
	.incbin "baserom.gba", 0x005be87b, 0x00000001
	.section .rom.005be9f9, "ax"
	.incbin "baserom.gba", 0x005be9f9, 0x00000003
	.section .rom.005c32ed, "ax"
	.incbin "baserom.gba", 0x005c32ed, 0x00000003
	.section .rom.005c4332, "ax"
	.incbin "baserom.gba", 0x005c4332, 0x00000002
	.section .rom.005c5156, "ax"
	.incbin "baserom.gba", 0x005c5156, 0x00000002
	.section .rom.005c649b, "ax"
	.incbin "baserom.gba", 0x005c649b, 0x00000001
	.section .rom.005c65f5, "ax"
	.incbin "baserom.gba", 0x005c65f5, 0x00000003
	.section .rom.005c8bb3, "ax"
	.incbin "baserom.gba", 0x005c8bb3, 0x00000001
	.section .rom.005c8d2a, "ax"
	.incbin "baserom.gba", 0x005c8d2a, 0x00000002
	.section .rom.005cd61d, "ax"
	.incbin "baserom.gba", 0x005cd61d, 0x00000003
	.section .rom.005ce085, "ax"
	.incbin "baserom.gba", 0x005ce085, 0x00000003
	.section .rom.005d06c7, "ax"
	.incbin "baserom.gba", 0x005d06c7, 0x00000001
	.section .rom.005d082e, "ax"
	.incbin "baserom.gba", 0x005d082e, 0x00000002
	.section .rom.005d1975, "ax"
	.incbin "baserom.gba", 0x005d1975, 0x00000003
	.section .rom.005d36d3, "ax"
	.incbin "baserom.gba", 0x005d36d3, 0x00000001
	.section .rom.005d383a, "ax"
	.incbin "baserom.gba", 0x005d383a, 0x00000002
	.section .rom.005d60c9, "ax"
	.incbin "baserom.gba", 0x005d60c9, 0x00000003
	.section .rom.005d61fe, "ax"
	.incbin "baserom.gba", 0x005d61fe, 0x00000002
	.section .rom.005d8e3a, "ax"
	.incbin "baserom.gba", 0x005d8e3a, 0x00000002
	.section .rom.005dc5d9, "ax"
	.incbin "baserom.gba", 0x005dc5d9, 0x00000003
	.section .rom.005df927, "ax"
	.incbin "baserom.gba", 0x005df927, 0x00000001
	.section .rom.005dfaa3, "ax"
	.incbin "baserom.gba", 0x005dfaa3, 0x00000001
	.section .rom.005e2652, "ax"
	.incbin "baserom.gba", 0x005e2652, 0x00000002
	.section .rom.005e48c6, "ax"
	.incbin "baserom.gba", 0x005e48c6, 0x00000002
	.section .rom.005e7137, "ax"
	.incbin "baserom.gba", 0x005e7137, 0x00000001
	.section .rom.005ea62b, "ax"
	.incbin "baserom.gba", 0x005ea62b, 0x00000001
	.section .rom.005ef252, "ax"
	.incbin "baserom.gba", 0x005ef252, 0x00000002
	.section .rom.005efed5, "ax"
	.incbin "baserom.gba", 0x005efed5, 0x00000003
	.section .rom.005f0017, "ax"
	.incbin "baserom.gba", 0x005f0017, 0x00000001
	.section .rom.005f1587, "ax"
	.incbin "baserom.gba", 0x005f1587, 0x00000001
	.section .rom.005f16f2, "ax"
	.incbin "baserom.gba", 0x005f16f2, 0x00000002
	.section .rom.005f344d, "ax"
	.incbin "baserom.gba", 0x005f344d, 0x00000003
	.section .rom.005f358f, "ax"
	.incbin "baserom.gba", 0x005f358f, 0x00000001
	.section .rom.005f5d8e, "ax"
	.incbin "baserom.gba", 0x005f5d8e, 0x00000002
	.section .rom.005f5ecf, "ax"
	.incbin "baserom.gba", 0x005f5ecf, 0x00000001
	.section .rom.005f78e7, "ax"
	.incbin "baserom.gba", 0x005f78e7, 0x00000001
	.section .rom.005f8e62, "ax"
	.incbin "baserom.gba", 0x005f8e62, 0x00000002
	.section .rom.005fa39e, "ax"
	.incbin "baserom.gba", 0x005fa39e, 0x00000002
	.section .rom.005fa52e, "ax"
	.incbin "baserom.gba", 0x005fa52e, 0x00000002
	.section .rom.005fc7e7, "ax"
	.incbin "baserom.gba", 0x005fc7e7, 0x00000001
	.section .rom.00600a2e, "ax"
	.incbin "baserom.gba", 0x00600a2e, 0x00000002
	.section .rom.00602825, "ax"
	.incbin "baserom.gba", 0x00602825, 0x00000003
	.section .rom.006059d3, "ax"
	.incbin "baserom.gba", 0x006059d3, 0x00000001
	.section .rom.00608007, "ax"
	.incbin "baserom.gba", 0x00608007, 0x00000001
	.section .rom.0060ab5f, "ax"
	.incbin "baserom.gba", 0x0060ab5f, 0x00000001
	.section .rom.0060ad27, "ax"
	.incbin "baserom.gba", 0x0060ad27, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ad28, 0x00002904
	.section .rom.00611131, "ax"
	.incbin "baserom.gba", 0x00611131, 0x00000003
	.section .rom.006126a2, "ax"
	.incbin "baserom.gba", 0x006126a2, 0x00000002
	.section .rom.00614ec9, "ax"
	.incbin "baserom.gba", 0x00614ec9, 0x00000003
	.section .rom.00615039, "ax"
	.incbin "baserom.gba", 0x00615039, 0x00000003
	.section .rom.006178fb, "ax"
	.incbin "baserom.gba", 0x006178fb, 0x00000001
	.section .rom.0061a0c3, "ax"
	.incbin "baserom.gba", 0x0061a0c3, 0x00000001
	.section .rom.0061b3bd, "ax"
	.incbin "baserom.gba", 0x0061b3bd, 0x00000003
	.section .rom.0061cd03, "ax"
	.incbin "baserom.gba", 0x0061cd03, 0x00000001
	.section .rom.00628196, "ax"
	.incbin "baserom.gba", 0x00628196, 0x00000002
	.section .rom.00628356, "ax"
	.incbin "baserom.gba", 0x00628356, 0x00000002
	.section .rom.0062b6bd, "ax"
	.incbin "baserom.gba", 0x0062b6bd, 0x00000003
	.section .rom.0062dbae, "ax"
	.incbin "baserom.gba", 0x0062dbae, 0x00000002
	.section .rom.0063098f, "ax"
	.incbin "baserom.gba", 0x0063098f, 0x00000001
	.section .rom.00631529, "ax"
	.incbin "baserom.gba", 0x00631529, 0x00000003
	.section .rom.0063238a, "ax"
	.incbin "baserom.gba", 0x0063238a, 0x00000002
	.section .rom.006324e2, "ax"
	.incbin "baserom.gba", 0x006324e2, 0x00000002
	.section .rom.0063a32f, "ax"
	.incbin "baserom.gba", 0x0063a32f, 0x00000001
	.section .rom.0063a49b, "ax"
	.incbin "baserom.gba", 0x0063a49b, 0x00000001
	.section .rom.0063d237, "ax"
	.incbin "baserom.gba", 0x0063d237, 0x00000001
	.section .rom.0063f3c6, "ax"
	.incbin "baserom.gba", 0x0063f3c6, 0x00000002
	.section .rom.0063face, "ax"
	.incbin "baserom.gba", 0x0063face, 0x00000002
	.section .rom.006406e2, "ax"
	.incbin "baserom.gba", 0x006406e2, 0x00000002
	.section .rom.006517b5, "ax"
	.incbin "baserom.gba", 0x006517b5, 0x00000003
	.section .rom.0065194a, "ax"
	.incbin "baserom.gba", 0x0065194a, 0x00000002
	.section .rom.00652e5a, "ax"
	.incbin "baserom.gba", 0x00652e5a, 0x00000002
	.section .rom.00654c3f, "ax"
	.incbin "baserom.gba", 0x00654c3f, 0x00000001
	.section .rom.00655cd2, "ax"
	.incbin "baserom.gba", 0x00655cd2, 0x00000002
	.section .rom.00657f42, "ax"
	.incbin "baserom.gba", 0x00657f42, 0x00000002
	.section .rom.006580d9, "ax"
	.incbin "baserom.gba", 0x006580d9, 0x00000003
	.section .rom.0065a07f, "ax"
	.incbin "baserom.gba", 0x0065a07f, 0x00000001
	.section .rom.0065ee56, "ax"
	.incbin "baserom.gba", 0x0065ee56, 0x00000002
	.section .rom.006628b7, "ax"
	.incbin "baserom.gba", 0x006628b7, 0x00000001
	.section .rom.006655ed, "ax"
	.incbin "baserom.gba", 0x006655ed, 0x00000003
	.section .rom.00667935, "ax"
	.incbin "baserom.gba", 0x00667935, 0x00000003
	.section .rom.00669113, "ax"
	.incbin "baserom.gba", 0x00669113, 0x00000001
	.section .rom.006692c5, "ax"
	.incbin "baserom.gba", 0x006692c5, 0x00000003
	.section .rom.00679c66, "ax"
	.incbin "baserom.gba", 0x00679c66, 0x00000002
	.section .rom.00679dc5, "ax"
	.incbin "baserom.gba", 0x00679dc5, 0x00000003
	.section .rom.0067d51f, "ax"
	.incbin "baserom.gba", 0x0067d51f, 0x00000001
	.section .rom.0067d682, "ax"
	.incbin "baserom.gba", 0x0067d682, 0x00000002
	.section .rom.00683365, "ax"
	.incbin "baserom.gba", 0x00683365, 0x00000003
	.section .rom.00685d4b, "ax"
	.incbin "baserom.gba", 0x00685d4b, 0x00000001
	.section .rom.00687f56, "ax"
	.incbin "baserom.gba", 0x00687f56, 0x00000002
	.section .rom.00688097, "ax"
	.incbin "baserom.gba", 0x00688097, 0x00000001
	.section .rom.00689aae, "ax"
	.incbin "baserom.gba", 0x00689aae, 0x00000002
	.section .rom.00690815, "ax"
	.incbin "baserom.gba", 0x00690815, 0x00000003
	.section .rom.00690993, "ax"
	.incbin "baserom.gba", 0x00690993, 0x00000001
	.section .rom.0069293d, "ax"
	.incbin "baserom.gba", 0x0069293d, 0x00000003
	.section .rom.006950b6, "ax"
	.incbin "baserom.gba", 0x006950b6, 0x00000002
	.section .rom.0069691d, "ax"
	.incbin "baserom.gba", 0x0069691d, 0x00000003
	.section .rom.0069a33e, "ax"
	.incbin "baserom.gba", 0x0069a33e, 0x00000002
	.section .rom.0069a507, "ax"
	.incbin "baserom.gba", 0x0069a507, 0x00000001
	.section .rom.006a54ef, "ax"
	.incbin "baserom.gba", 0x006a54ef, 0x00000001
	.section .rom.006a763b, "ax"
	.incbin "baserom.gba", 0x006a763b, 0x00000001
	.section .rom.006a77c5, "ax"
	.incbin "baserom.gba", 0x006a77c5, 0x00000003
	.section .rom.006a924a, "ax"
	.incbin "baserom.gba", 0x006a924a, 0x00000002
	.section .rom.006ac281, "ax"
	.incbin "baserom.gba", 0x006ac281, 0x00000003
	.section .rom.006ac447, "ax"
	.incbin "baserom.gba", 0x006ac447, 0x00000001
	.section .rom.006ad37b, "ax"
	.incbin "baserom.gba", 0x006ad37b, 0x00000001
	.section .rom.006af42e, "ax"
	.incbin "baserom.gba", 0x006af42e, 0x00000002
	.section .rom.006af5d5, "ax"
	.incbin "baserom.gba", 0x006af5d5, 0x00000003
	.section .rom.006b45ff, "ax"
	.incbin "baserom.gba", 0x006b45ff, 0x00000001
	.section .rom.006b738f, "ax"
	.incbin "baserom.gba", 0x006b738f, 0x00000001
	.section .rom.006b754f, "ax"
	.incbin "baserom.gba", 0x006b754f, 0x00000001
	.section .rom.006b9a43, "ax"
	.incbin "baserom.gba", 0x006b9a43, 0x00000001
	.section .rom.006b9c15, "ax"
	.incbin "baserom.gba", 0x006b9c15, 0x00000003
	.section .rom.006bbca6, "ax"
	.incbin "baserom.gba", 0x006bbca6, 0x00000002
	.section .rom.006bdda6, "ax"
	.incbin "baserom.gba", 0x006bdda6, 0x00000002
	.section .rom.006c095a, "ax"
	.incbin "baserom.gba", 0x006c095a, 0x00000002
	.section .rom.006c1447, "ax"
	.incbin "baserom.gba", 0x006c1447, 0x00000001
	.section .rom.006c161a, "ax"
	.incbin "baserom.gba", 0x006c161a, 0x00000002
	.section .rom.006c3f49, "ax"
	.incbin "baserom.gba", 0x006c3f49, 0x00000003
	.section .rom.006c56e1, "ax"
	.incbin "baserom.gba", 0x006c56e1, 0x00000003
	.section .rom.006c6955, "ax"
	.incbin "baserom.gba", 0x006c6955, 0x00000003
	.section .rom.006c8de5, "ax"
	.incbin "baserom.gba", 0x006c8de5, 0x00000003
	.section .rom.006cb131, "ax"
	.incbin "baserom.gba", 0x006cb131, 0x00000003
	.section .rom.006cc31b, "ax"
	.incbin "baserom.gba", 0x006cc31b, 0x00000001
	.section .rom.006cc4a2, "ax"
	.incbin "baserom.gba", 0x006cc4a2, 0x00000002
	.section .rom.006d09ee, "ax"
	.incbin "baserom.gba", 0x006d09ee, 0x00000002
	.section .rom.006d0b43, "ax"
	.incbin "baserom.gba", 0x006d0b43, 0x00000001
	.section .rom.006d0c83, "ax"
	.incbin "baserom.gba", 0x006d0c83, 0x00000001
	.section .rom.006d1957, "ax"
	.incbin "baserom.gba", 0x006d1957, 0x00000001
	.section .rom.006d1ad6, "ax"
	.incbin "baserom.gba", 0x006d1ad6, 0x00000002
	.section .rom.006d338f, "ax"
	.incbin "baserom.gba", 0x006d338f, 0x00000001
	.section .rom.006d3511, "ax"
	.incbin "baserom.gba", 0x006d3511, 0x00000003
	.section .rom.006d4fa3, "ax"
	.incbin "baserom.gba", 0x006d4fa3, 0x00000001
	.section .rom.006d511d, "ax"
	.incbin "baserom.gba", 0x006d511d, 0x00000003
	.section .rom.006d6e5b, "ax"
	.incbin "baserom.gba", 0x006d6e5b, 0x00000001
	.section .rom.006d6f87, "ax"
	.incbin "baserom.gba", 0x006d6f87, 0x00000001
	.section .rom.006d70fd, "ax"
	.incbin "baserom.gba", 0x006d70fd, 0x00000003
	.section .rom.006d8fa3, "ax"
	.incbin "baserom.gba", 0x006d8fa3, 0x00000001
	.section .rom.006d9129, "ax"
	.incbin "baserom.gba", 0x006d9129, 0x00000003
	.section .rom.006da087, "ax"
	.incbin "baserom.gba", 0x006da087, 0x00000001
	.section .rom.006db337, "ax"
	.incbin "baserom.gba", 0x006db337, 0x00000001
	.section .rom.006db4f2, "ax"
	.incbin "baserom.gba", 0x006db4f2, 0x00000002
	.section .rom.006ddf96, "ax"
	.incbin "baserom.gba", 0x006ddf96, 0x00000002
	.section .rom.006de109, "ax"
	.incbin "baserom.gba", 0x006de109, 0x00000003
	.section .rom.006df4f1, "ax"
	.incbin "baserom.gba", 0x006df4f1, 0x00000003
	.section .rom.006e1fd1, "ax"
	.incbin "baserom.gba", 0x006e1fd1, 0x00000003
	.section .rom.006e217b, "ax"
	.incbin "baserom.gba", 0x006e217b, 0x00000001
	.section .rom.006e4576, "ax"
	.incbin "baserom.gba", 0x006e4576, 0x00000002
	.section .rom.006e4705, "ax"
	.incbin "baserom.gba", 0x006e4705, 0x00000003
	.section .rom.006e7985, "ax"
	.incbin "baserom.gba", 0x006e7985, 0x00000003
	.section .rom.006e7b66, "ax"
	.incbin "baserom.gba", 0x006e7b66, 0x00000002
	.section .rom.006ea0fb, "ax"
	.incbin "baserom.gba", 0x006ea0fb, 0x00000001
	.section .rom.006ea28a, "ax"
	.incbin "baserom.gba", 0x006ea28a, 0x00000002
	.section .rom.006ece82, "ax"
	.incbin "baserom.gba", 0x006ece82, 0x00000002
	.section .rom.006ed9cf, "ax"
	.incbin "baserom.gba", 0x006ed9cf, 0x00000001
	.section .rom.006edb36, "ax"
	.incbin "baserom.gba", 0x006edb36, 0x00000002
	.section .rom.006f054b, "ax"
	.incbin "baserom.gba", 0x006f054b, 0x00000001
	.section .rom.006f218a, "ax"
	.incbin "baserom.gba", 0x006f218a, 0x00000002
	.section .rom.006f2647, "ax"
	.incbin "baserom.gba", 0x006f2647, 0x00000001
	.section .rom.006f39bf, "ax"
	.incbin "baserom.gba", 0x006f39bf, 0x00000001
	.section .rom.006f4f03, "ax"
	.incbin "baserom.gba", 0x006f4f03, 0x00000001
	.section .rom.006f508d, "ax"
	.incbin "baserom.gba", 0x006f508d, 0x00000003
	.section .rom.006f616b, "ax"
	.incbin "baserom.gba", 0x006f616b, 0x00000001
	.section .rom.006f62f9, "ax"
	.incbin "baserom.gba", 0x006f62f9, 0x00000003
	.section .rom.006f7f4d, "ax"
	.incbin "baserom.gba", 0x006f7f4d, 0x00000003
	.section .rom.006fa92f, "ax"
	.incbin "baserom.gba", 0x006fa92f, 0x00000001
	.section .rom.006fde59, "ax"
	.incbin "baserom.gba", 0x006fde59, 0x00000003
	.section .rom.006ff763, "ax"
	.incbin "baserom.gba", 0x006ff763, 0x00000001
	.section .rom.0070180b, "ax"
	.incbin "baserom.gba", 0x0070180b, 0x00000001
	.section .rom.0070425f, "ax"
	.incbin "baserom.gba", 0x0070425f, 0x00000001
	.section .rom.00705eab, "ax"
	.incbin "baserom.gba", 0x00705eab, 0x00000001
	.section .rom.007072f1, "ax"
	.incbin "baserom.gba", 0x007072f1, 0x00000003
	.section .rom.00709171, "ax"
	.incbin "baserom.gba", 0x00709171, 0x00000003
	.section .rom.0070ad42, "ax"
	.incbin "baserom.gba", 0x0070ad42, 0x00000002
	.section .rom.0070d476, "ax"
	.incbin "baserom.gba", 0x0070d476, 0x00000002
	.section .rom.0070f0bb, "ax"
	.incbin "baserom.gba", 0x0070f0bb, 0x00000001
	.section .rom.00710501, "ax"
	.incbin "baserom.gba", 0x00710501, 0x00000003
	.section .rom.0071207f, "ax"
	.incbin "baserom.gba", 0x0071207f, 0x00000001
	.section .rom.007132af, "ax"
	.incbin "baserom.gba", 0x007132af, 0x00000001
	.section .rom.00713409, "ax"
	.incbin "baserom.gba", 0x00713409, 0x00000003
	.section .rom.0071690f, "ax"
	.incbin "baserom.gba", 0x0071690f, 0x00000001
	.section .rom.00716a7a, "ax"
	.incbin "baserom.gba", 0x00716a7a, 0x00000002
	.section .rom.007193ca, "ax"
	.incbin "baserom.gba", 0x007193ca, 0x00000002
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007193cc, 0x00001164
	.section .rom.0071bd15, "ax"
	.incbin "baserom.gba", 0x0071bd15, 0x00000003
	.section .rom.0071ef5b, "ax"
	.incbin "baserom.gba", 0x0071ef5b, 0x00000001
	.section .rom.00721852, "ax"
	.incbin "baserom.gba", 0x00721852, 0x00000002
	.section .rom.0072ab8b, "ax"
	.incbin "baserom.gba", 0x0072ab8b, 0x00000001
	.section .rom.0072beba, "ax"
	.incbin "baserom.gba", 0x0072beba, 0x00000002
	.section .rom.0072d0b3, "ax"
	.incbin "baserom.gba", 0x0072d0b3, 0x00000001
	.section .rom.0072d215, "ax"
	.incbin "baserom.gba", 0x0072d215, 0x00000003
	.section .rom.0072fbe1, "ax"
	.incbin "baserom.gba", 0x0072fbe1, 0x00000003
	.section .rom.0073310d, "ax"
	.incbin "baserom.gba", 0x0073310d, 0x00000003
	.section .rom.0073754f, "ax"
	.incbin "baserom.gba", 0x0073754f, 0x00000001
	.section .rom.007376c2, "ax"
	.incbin "baserom.gba", 0x007376c2, 0x00000002
	.section .rom.0073b927, "ax"
	.incbin "baserom.gba", 0x0073b927, 0x00000001
	.section .rom.0073f142, "ax"
	.incbin "baserom.gba", 0x0073f142, 0x00000002
	.section .rom.00740b6a, "ax"
	.incbin "baserom.gba", 0x00740b6a, 0x00000002
	.section .rom.00743217, "ax"
	.incbin "baserom.gba", 0x00743217, 0x00000001
	.section .rom.0074527b, "ax"
	.incbin "baserom.gba", 0x0074527b, 0x00000001
	.section .rom.00747137, "ax"
	.incbin "baserom.gba", 0x00747137, 0x00000001
	.section .rom.0074a381, "ax"
	.incbin "baserom.gba", 0x0074a381, 0x00000003
	.section .rom.0074bd47, "ax"
	.incbin "baserom.gba", 0x0074bd47, 0x00000001
	.section .rom.0074beb9, "ax"
	.incbin "baserom.gba", 0x0074beb9, 0x00000003
	.section .rom.0074eec6, "ax"
	.incbin "baserom.gba", 0x0074eec6, 0x00000002
	.section .rom.0074f07e, "ax"
	.incbin "baserom.gba", 0x0074f07e, 0x00000002
	.section .rom.007508ef, "ax"
	.incbin "baserom.gba", 0x007508ef, 0x00000001
	.section .rom.00750a6e, "ax"
	.incbin "baserom.gba", 0x00750a6e, 0x00000002
	.section .rom.00754277, "ax"
	.incbin "baserom.gba", 0x00754277, 0x00000001
	.section .rom.007543eb, "ax"
	.incbin "baserom.gba", 0x007543eb, 0x00000001
	.section .rom.007551b3, "ax"
	.incbin "baserom.gba", 0x007551b3, 0x00000001
	.section .rom.00755359, "ax"
	.incbin "baserom.gba", 0x00755359, 0x00000003
	.section .rom.007573b4, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x007573b4, 0x000000e4
	.section .rom.0075a1af, "ax"
	.incbin "baserom.gba", 0x0075a1af, 0x00000001
	.section .rom.0075a396, "ax"
	.incbin "baserom.gba", 0x0075a396, 0x00000002
	.section .rom.0075d632, "ax"
	.incbin "baserom.gba", 0x0075d632, 0x00000002
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d634, 0x00004600
	.section .rom.00761d3a, "ax"
	.incbin "baserom.gba", 0x00761d3a, 0x00000002
	.section .rom.0076390d, "ax"
	.incbin "baserom.gba", 0x0076390d, 0x00000003
	.section .rom.007653bd, "ax"
	.incbin "baserom.gba", 0x007653bd, 0x00000003
	.section .rom.00765519, "ax"
	.incbin "baserom.gba", 0x00765519, 0x00000003
	.section .rom.0076b75f, "ax"
	.incbin "baserom.gba", 0x0076b75f, 0x00000001
	.section .rom.0076b863, "ax"
	.incbin "baserom.gba", 0x0076b863, 0x00000001
	.section .rom.0076c27f, "ax"
	.incbin "baserom.gba", 0x0076c27f, 0x00000001
	.section .rom.0076c365, "ax"
	.incbin "baserom.gba", 0x0076c365, 0x00000003
	.section .rom.0076c601, "ax"
	.incbin "baserom.gba", 0x0076c601, 0x00000003
	.section .rom.0077065e, "ax"
	.incbin "baserom.gba", 0x0077065e, 0x00000002
	.section .rom.00770c12, "ax"
	.incbin "baserom.gba", 0x00770c12, 0x00000002
	.section .rom.00771ba7, "ax"
	.incbin "baserom.gba", 0x00771ba7, 0x00000001
	.section .rom.007735aa, "ax"
	.incbin "baserom.gba", 0x007735aa, 0x00000002
	.section .rom.00774219, "ax"
	.incbin "baserom.gba", 0x00774219, 0x00000003
	.section .rom.007744c1, "ax"
	.incbin "baserom.gba", 0x007744c1, 0x00000003
	.section .rom.00775346, "ax"
	.incbin "baserom.gba", 0x00775346, 0x00000002
	.section .rom.007764d7, "ax"
	.incbin "baserom.gba", 0x007764d7, 0x00000001
	.section .rom.0077669f, "ax"
	.incbin "baserom.gba", 0x0077669f, 0x00000001
	.section .rom.007769eb, "ax"
	.incbin "baserom.gba", 0x007769eb, 0x00000001
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x007769ec, 0x0000000c
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x007769f8, 0x00000150
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x00776b48, 0x00000140
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x00776c88, 0x00000140
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x00776dc8, 0x00000140
	.section .rom.00777253, "ax"
	.incbin "baserom.gba", 0x00777253, 0x00000001
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x00777254, 0x0000000c
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x00777260, 0x00000150
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x007773b0, 0x00000140
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x007774f0, 0x00000140
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x00777630, 0x00000140
	.section .rom.00777abb, "ax"
	.incbin "baserom.gba", 0x00777abb, 0x00000001
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x00777abc, 0x0000000c
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x00777ac8, 0x00000150
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x00777c18, 0x00000140
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x00777d58, 0x00000140
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x00777e98, 0x00000140
	.section .rom.00778323, "ax"
	.incbin "baserom.gba", 0x00778323, 0x00000001
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x00778324, 0x0000000c
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x00778330, 0x00000150
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x00778480, 0x00000140
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x007785c0, 0x00000140
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x00778700, 0x00000140
	.section .rom.00778b8b, "ax"
	.incbin "baserom.gba", 0x00778b8b, 0x00000001
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x00778b8c, 0x0000000c
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x00778b98, 0x00000150
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x00778ce8, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x00778e28, 0x00000140
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x00778f68, 0x00000140
	.section .rom.007793f3, "ax"
	.incbin "baserom.gba", 0x007793f3, 0x00000001
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x007793f4, 0x0000000c
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x00779400, 0x00000150
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x00779550, 0x00000140
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x00779690, 0x00000140
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x007797d0, 0x00000140
	.section .rom.00779c5b, "ax"
	.incbin "baserom.gba", 0x00779c5b, 0x00000001
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00779c5c, 0x0000000c
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x00779c68, 0x00000150
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x00779db8, 0x00000140
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x00779ef8, 0x00000140
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x0077a038, 0x00000140
	.section .rom.0077a4c3, "ax"
	.incbin "baserom.gba", 0x0077a4c3, 0x00000001
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x0077a4c4, 0x0000000c
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x0077a4d0, 0x00000150
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x0077a620, 0x00000140
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x0077a760, 0x00000140
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x0077a8a0, 0x00000140
