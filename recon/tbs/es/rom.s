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
	.incbin "baserom.gba", 0x000f1788, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000f1790, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000f17b1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000f17d2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000f1816, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000f1837, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000f1858, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000f185c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000f1860, 0x00000004
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
	.incbin "baserom.gba", 0x000f1888, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000f1890, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1896, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000f189c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000f189f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000f18a2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000f18aa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000f18b0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000f18b3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000f18b6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000f18c4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000f18d6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000f18e8, 0x00000024
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
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000f195a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000f1963, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000f196c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000f1974, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000f1977, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000f197a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000f197e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000f1984, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000f1988, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000f198a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000f198e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000f1994, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000f199a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000f19a0, 0x0000000c
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
	.incbin "baserom.gba", 0x000f1ab4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000f2110, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000f2116, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000f2120, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000f2125, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000f212a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000f2130, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000f2134, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000f213e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000f2143, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000f2148, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000f2152, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000f2158, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000f2166, 0x0000000e
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
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000f2348, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000f234b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000f234e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000f2354, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000f2358, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000f235e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000f2361, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000f2371, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000f2379, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000f2380, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
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
	.incbin "baserom.gba", 0x000f23e6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000f23e9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000f23ec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000f2428, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000f242f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000f2436, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000f243d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000f2444, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000f2452, 0x00000008
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
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000f24aa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000f24ae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000f24b2, 0x00000040
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
	.incbin "baserom.gba", 0x000f251e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000f253e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000f257e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000f2590, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000f259a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000f25a0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000f25a3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000f25a6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000f25ac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000f25b2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000f25b8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000f25be, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000f25ca, 0x00000006
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
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000f261e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000f262a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000f2636, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000f263e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000f2646, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000f264e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000f2656, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000f265e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2666, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000f2676, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000f26a0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000f26bc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000f26ca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000f26d8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000f26e1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000f26ea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000f26f8, 0x0000000e
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
	.incbin "baserom.gba", 0x000f2718, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000f2728, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000f2730, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000f273e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000f274a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000f2750, 0x00000006
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
	.incbin "baserom.gba", 0x000f2834, 0x00000020
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
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f6969, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f696f, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f69ac, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f69ec, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f6a2c, 0x00000040
	.section .rom.000f83dc, "ax"
	.incbin "baserom.gba", 0x000f83dc, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000f83fc, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000f8400, 0x00000008
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000f8408, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000f841a, 0x0000000c
	.section .rom.000fb744, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000fb744, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000fb758, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000fb75e, 0x00000009
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000fb767, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000fb775, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000fb783, 0x00000009
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
	.section .rom.003c72d3, "ax"
	.incbin "baserom.gba", 0x003c72d3, 0x00000001
	.section .rom.003cd06e, "ax"
	.incbin "baserom.gba", 0x003cd06e, 0x00000002
	.section .rom.003cd8b5, "ax"
	.incbin "baserom.gba", 0x003cd8b5, 0x00000003
	.section .rom.003d9e96, "ax"
	.incbin "baserom.gba", 0x003d9e96, 0x00000002
	.section .rom.003f3a04, "ax"
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f3a04, 0x00000a6c
	.section .rom.003ffefc, "ax"
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003ffefc, 0x00000a34
	.section .rom.00403950, "ax"
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00403950, 0x00001588
	.section .rom.00441032, "ax"
	.incbin "baserom.gba", 0x00441032, 0x00000002
	.section .rom.00489e33, "ax"
	.incbin "baserom.gba", 0x00489e33, 0x00000001
	.section .rom.004bc270, "ax"
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
	.section .rom.00517dd2, "ax"
	.incbin "baserom.gba", 0x00517dd2, 0x00000002
	.section .rom.00518b4f, "ax"
	.incbin "baserom.gba", 0x00518b4f, 0x00000001
	.section .rom.0051c57a, "ax"
	.incbin "baserom.gba", 0x0051c57a, 0x00000002
	.section .rom.0055822d, "ax"
	.incbin "baserom.gba", 0x0055822d, 0x00000003
	.section .rom.0057b784, "ax"
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057b784, 0x00001cec
	.section .rom.005ad210, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ad210, 0x000022d8
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
