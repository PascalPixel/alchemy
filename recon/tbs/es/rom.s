	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00007380, "ax"
	.incbin "baserom.gba", 0x000073b8, 0x0000031e
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
	.section .rom.00008ab8, "ax"
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "baserom.gba", 0x00008ab8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x00008fb8, 0x00000400
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.00012720, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012720, 0x00000080
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x000127a0, 0x000000dc
	.global AnimationFacing_Kind1
AnimationFacing_Kind1:
	.incbin "baserom.gba", 0x0001287c, 0x00000010
	.global AnimationFacing_Kind22
AnimationFacing_Kind22:
	.incbin "baserom.gba", 0x0001288c, 0x00000008
	.global AnimationFacing_Kind2
AnimationFacing_Kind2:
	.incbin "baserom.gba", 0x00012894, 0x00000008
	.global AnimationFacing_Kind3
AnimationFacing_Kind3:
	.incbin "baserom.gba", 0x0001289c, 0x00000010
	.global AnimationFacing_Kind5
AnimationFacing_Kind5:
	.incbin "baserom.gba", 0x000128ac, 0x00000010
	.global AnimationFacing_Kind8
AnimationFacing_Kind8:
	.incbin "baserom.gba", 0x000128bc, 0x00000008
	.global AnimationFacing_Kind88
AnimationFacing_Kind88:
	.incbin "baserom.gba", 0x000128c4, 0x00000008
	.global AnimationFacing_Kind4
AnimationFacing_Kind4:
	.incbin "baserom.gba", 0x000128cc, 0x00000040
	.global AnimationFacing_Kind6
AnimationFacing_Kind6:
	.incbin "baserom.gba", 0x0001290c, 0x00000040
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001294c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00012990, 0x00000030
	.global ScriptObject_CycleTable
ScriptObject_CycleTable:
	.incbin "baserom.gba", 0x000129c0, 0x00000080
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00012a40, 0x00000014
	.global WorldMap_KeyHeadings
WorldMap_KeyHeadings:
	.incbin "baserom.gba", 0x00012a54, 0x00000020
	.global WorldMap_FootprintActions
WorldMap_FootprintActions:
	.incbin "baserom.gba", 0x00012a74, 0x00000018
	.global WorldMap_KeyYaws
WorldMap_KeyYaws:
	.incbin "baserom.gba", 0x00012a8c, 0x00000040
	.global WorldMap_GraphicsResources
WorldMap_GraphicsResources:
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
	.incbin "baserom.gba", 0x00012e20, 0x00000004
	.global ScriptObject_CommandTable
ScriptObject_CommandTable:
	.incbin "baserom.gba", 0x00012e24, 0x000000bc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00012ee0, 0x000000a4
	.global Map_LayeredScenes
Map_LayeredScenes:
	.incbin "baserom.gba", 0x00012f84, 0x000008c5
	.section .rom.00028ad0, "ax"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00028ad0, 0x00000100
	.global RomBytes_08029a10
	.global UiIcon_FramePointerTable
RomBytes_08029a10:
UiIcon_FramePointerTable:
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
	.global RomBytes_080310a4
Resource_FixedBlockBTiles:
RomBytes_080310a4:
	.incbin "baserom.gba", 0x00030264, 0x00000300
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "baserom.gba", 0x00030564, 0x00000080
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "baserom.gba", 0x000305e4, 0x000003c0
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
	.incbin "baserom.gba", 0x00033100, 0x00000020
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.incbin "baserom.gba", 0x00033120, 0x00000008
	.global UiSprite_CursorBobOffsets
UiSprite_CursorBobOffsets:
	.incbin "baserom.gba", 0x00033128, 0x00000050
	.global UiSprite_WobbleOffsets
UiSprite_WobbleOffsets:
	.incbin "baserom.gba", 0x00033178, 0x00000038
	.global UiSprite_BounceOffsets
UiSprite_BounceOffsets:
	.incbin "baserom.gba", 0x000331b0, 0x00000010
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x000331c0, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000335c0, 0x00000400
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "baserom.gba", 0x000339c0, 0x00002000
	.global Ui_BounceScales
Ui_BounceScales:
	.incbin "baserom.gba", 0x000359c0, 0x00000048
	.global Menu_CursorBobOffsets
Menu_CursorBobOffsets:
	.incbin "baserom.gba", 0x00035a08, 0x00000010
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00035a18, 0x00000079
	.global OptionMenu_SpeedFrames
OptionMenu_SpeedFrames:
	.incbin "baserom.gba", 0x00035a91, 0x00000003
	.global OptionMenu_SpeechFrames
OptionMenu_SpeechFrames:
	.incbin "baserom.gba", 0x00035a94, 0x00000002
	.global OptionMenu_SleepFrames
OptionMenu_SleepFrames:
	.incbin "baserom.gba", 0x00035a96, 0x00000002
	.global OptionMenu_FirstColorPresets
OptionMenu_FirstColorPresets:
	.incbin "baserom.gba", 0x00035a98, 0x00000006
	.global OptionMenu_SecondColorPresets
OptionMenu_SecondColorPresets:
	.incbin "baserom.gba", 0x00035a9e, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00035aa4, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00035aac, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00035ad4, 0x000000c8
	.global UiSprite_CursorFrameTiles
UiSprite_CursorFrameTiles:
	.incbin "baserom.gba", 0x00035b9c, 0x00000800
	.global UiText_GlyphWidths
UiText_GlyphWidths:
	.incbin "baserom.gba", 0x0003639c, 0x000000e0
	.global UiText_ColourPens
UiText_ColourPens:
	.incbin "baserom.gba", 0x0003647c, 0x00000010
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
	.incbin "baserom.gba", 0x0003648c, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000364a8, 0x00000018
	.global NameEntry_CaretWave
NameEntry_CaretWave:
	.incbin "baserom.gba", 0x000364c0, 0x00000008
	.global NameEntry_DeleteKeyLabel
NameEntry_DeleteKeyLabel:
	.incbin "baserom.gba", 0x000364c8, 0x00000004
	.global NameEntry_EndKeyLabel
NameEntry_EndKeyLabel:
	.incbin "baserom.gba", 0x000364cc, 0x00000004
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
	.incbin "baserom.gba", 0x000365d0, 0x00000008
	.global DjinnMenu_BlinkTile
DjinnMenu_BlinkTile:
	.incbin "baserom.gba", 0x000365d8, 0x00000020
	.global OwnerStatus_StatCursors
OwnerStatus_StatCursors:
	.incbin "baserom.gba", 0x000365f8, 0x00000080
	.global OwnerStatus_EquipmentCursors
OwnerStatus_EquipmentCursors:
	.incbin "baserom.gba", 0x00036678, 0x00000010
	.global OwnerStatus_ConditionCursors
OwnerStatus_ConditionCursors:
	.incbin "baserom.gba", 0x00036688, 0x00000020
	.global OwnerStatus_NivString
OwnerStatus_NivString:
	.incbin "baserom.gba", 0x000366a8, 0x00000004
	.global OwnerStatus_NvString
OwnerStatus_NvString:
	.incbin "baserom.gba", 0x000366ac, 0x00000004
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.incbin "baserom.gba", 0x000366b0, 0x00000004
	.global OwnerStatus_KpString
OwnerStatus_KpString:
	.incbin "baserom.gba", 0x000366b4, 0x00000004
	.global OwnerStatus_PvString
OwnerStatus_PvString:
	.incbin "baserom.gba", 0x000366b8, 0x00000004
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.incbin "baserom.gba", 0x000366bc, 0x00000004
	.global OwnerStatus_SlashString
OwnerStatus_SlashString:
	.incbin "baserom.gba", 0x000366c0, 0x00000004
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.incbin "baserom.gba", 0x000366c4, 0x00000003
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.incbin "baserom.gba", 0x000366c7, 0x00000004
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.incbin "baserom.gba", 0x000366cb, 0x00000004
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
	.incbin "baserom.gba", 0x0003670c, 0x00000014
	.global Workspace_ItemTitle
Workspace_ItemTitle:
	.incbin "baserom.gba", 0x00036720, 0x00000008
	.global Workspace_EnergyTitle
Workspace_EnergyTitle:
	.incbin "baserom.gba", 0x00036728, 0x00000008
	.global Workspace_StatusTitle
Workspace_StatusTitle:
	.incbin "baserom.gba", 0x00036730, 0x00000008
	.global Workspace_PageLabel
Workspace_PageLabel:
	.incbin "baserom.gba", 0x00036738, 0x00000008
	.global Workspace_RangeDash
Workspace_RangeDash:
	.incbin "baserom.gba", 0x00036740, 0x00000004
	.section .rom.00073368, "ax"
	.global UiText_SpeedDelays
UiText_SpeedDelays:
	.incbin "baserom.gba", 0x00073368, 0x00000003
	.global UiText_SpeedSteps
UiText_SpeedSteps:
	.incbin "baserom.gba", 0x0007336b, 0x00000003
	.global UiText_SpeedCueGaps
UiText_SpeedCueGaps:
	.incbin "baserom.gba", 0x0007336e, 0x00000004
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073372, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x000733b4, 0x00000010
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.incbin "baserom.gba", 0x000733c4, 0x00000110
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x000734d4, 0x00000004
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
	.global Enemy_Definitions
Enemy_Definitions:
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
	.incbin "baserom.gba", 0x0008de38, 0x00000420
	.global Element_PowerResistCurve
Element_PowerResistCurve:
	.incbin "baserom.gba", 0x0008e258, 0x00000014
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008e26c, 0x000003b6
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
	.global Scene_AreaNameRules
Scene_AreaNameRules:
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
	.incbin "baserom.gba", 0x000a3626, 0x000001b2
	.global Field_TileMessageOffsets
Field_TileMessageOffsets:
	.incbin "baserom.gba", 0x000a37d8, 0x00000006
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
	.incbin "baserom.gba", 0x000a39f8, 0x0000000c
	.global DisplayTransition_NoiseWidths
DisplayTransition_NoiseWidths:
	.incbin "baserom.gba", 0x000a3a04, 0x00000022
	.global DisplayTransition_EdgeOffsets
DisplayTransition_EdgeOffsets:
	.incbin "baserom.gba", 0x000a3a26, 0x00000020
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x000a3a46, 0x00000040
	.global BattlePalette_ChannelCurve
BattlePalette_ChannelCurve:
	.incbin "baserom.gba", 0x000a3a86, 0x00000040
	.global BattlePalette_ChannelCurveLight
BattlePalette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000a3ac6, 0x00000040
	.global BattlePalette_ChannelCurveContrast
BattlePalette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000a3b06, 0x00000042
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000a3b48, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x000a3d54, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x000a3ed8, 0x00000004
	.global DisplayScroll_WaveSine
DisplayScroll_WaveSine:
	.incbin "baserom.gba", 0x000a3edc, 0x00000200
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
	.incbin "baserom.gba", 0x000a4208, 0x0000000c
	.global BattleFx_SourceHoldScript
BattleFx_SourceHoldScript:
	.incbin "baserom.gba", 0x000a4214, 0x00000018
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000a422c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000a4250, 0x00000020
	.global Data_0809f118
Data_0809f118:
	.incbin "baserom.gba", 0x000a4270, 0x00000004
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x000a4274, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x000a42b8, 0x00000008
	.global WorldMap_MarkerBlendCycle
WorldMap_MarkerBlendCycle:
	.incbin "baserom.gba", 0x000a42c0, 0x00000020
	.global WorldMap_CursorDirectionAngles
WorldMap_CursorDirectionAngles:
	.incbin "baserom.gba", 0x000a42e0, 0x00000020
	.section .rom.000a4948, "ax"
	.incbin "baserom.gba", 0x000a4948, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x000a4968, 0x00000030
	.global DisplayTransition_Shapes
DisplayTransition_Shapes:
	.incbin "baserom.gba", 0x000a4998, 0x0000038c
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
	.incbin "baserom.gba", 0x000a5284, 0x0000000c
	.global WorldMap_PlaceMarkers
WorldMap_PlaceMarkers:
	.incbin "baserom.gba", 0x000a5290, 0x0000014c
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
	.global Ui_LvString
Ui_LvString:
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
	.global Menu_TimesString
Menu_TimesString:
	.incbin "baserom.gba", 0x000b3a88, 0x00000004
	.global Menu_PointString
Menu_PointString:
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
	.global Menu_SlashString
Menu_SlashString:
	.incbin "baserom.gba", 0x000b3a9c, 0x00000004
	.global Menu_HpString
Menu_HpString:
	.incbin "baserom.gba", 0x000b3aa0, 0x00000004
	.global Menu_PpString
Menu_PpString:
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
	.global FourObjectMotion_ResourceIds
	.global RomBytes_080af304
FourObjectMotion_ResourceIds:
RomBytes_080af304:
	.incbin "baserom.gba", 0x000b3b70, 0x0000000d
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
	.incbin "baserom.gba", 0x000b8ab6, 0x0000000c
	.section .rom.000b9204, "ax"
	.incbin "baserom.gba", 0x000b9204, 0x00000008
	.section .rom.000b9218, "ax"
	.incbin "baserom.gba", 0x000b9218, 0x00000040
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
	.global HitTbl
HitTbl:
	.incbin "baserom.gba", 0x000c6ad0, 0x00000008
	.global EpLossTbl
EpLossTbl:
	.incbin "baserom.gba", 0x000c6ad8, 0x00000018
	.global HpRcvTbl
HpRcvTbl:
	.incbin "baserom.gba", 0x000c6af0, 0x00000018
	.global EpDmgTbl
EpDmgTbl:
	.incbin "baserom.gba", 0x000c6b08, 0x00000018
	.global HpDmgTbl5
HpDmgTbl5:
	.incbin "baserom.gba", 0x000c6b20, 0x00000018
	.global HpDmgTbl8
HpDmgTbl8:
	.incbin "baserom.gba", 0x000c6b38, 0x00000018
	.global HpDmgTbl6
HpDmgTbl6:
	.incbin "baserom.gba", 0x000c6b50, 0x00000018
	.global EpRcvTbl
EpRcvTbl:
	.incbin "baserom.gba", 0x000c6b68, 0x00000018
	.global HpDmgTbl
HpDmgTbl:
	.incbin "baserom.gba", 0x000c6b80, 0x00000018
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.incbin "baserom.gba", 0x000c6b98, 0x00000018
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
	.global Debug_PartyPresets
Debug_PartyPresets:
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
	.incbin "baserom.gba", 0x000cb438, 0x0000055b
	.section .rom.000e6172, "ax"
	.incbin "baserom.gba", 0x000e6172, 0x00000002
	.section .rom.000f1278, "ax"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000f1278, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000f1280, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000f1288, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000f1298, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000f12b0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000f12b8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000f12c0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000f12c8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000f12d0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000f12d8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000f12e0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000f12e8, 0x00000008
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
	.incbin "baserom.gba", 0x000f47dc, 0x000002f3
	.section .rom.000f68bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f68bc, 0x000000ad
	.section .rom.000fed04, "ax"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.000fee24, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fee24, 0x00000060
	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032c89c, 0x000086f8
	.section .rom.0033c3fc, "ax"
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c3fc, 0x0000071c
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
