	.section .rom.00007360, "ax"
	.incbin "baserom.gba", 0x00007398, 0x0000031e
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
	.global Save_Signature
Save_Signature:
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
	.section .rom.000092b8, "ax"
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "baserom.gba", 0x000092b8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000097b8, 0x00000400
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00012f20, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012f20, 0x00000080
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x00012fa0, 0x000000dc
	.global AnimationFacing_Kind1
AnimationFacing_Kind1:
	.incbin "baserom.gba", 0x0001307c, 0x00000010
	.global AnimationFacing_Kind22
AnimationFacing_Kind22:
	.incbin "baserom.gba", 0x0001308c, 0x00000008
	.global AnimationFacing_Kind2
AnimationFacing_Kind2:
	.incbin "baserom.gba", 0x00013094, 0x00000008
	.global AnimationFacing_Kind3
AnimationFacing_Kind3:
	.incbin "baserom.gba", 0x0001309c, 0x00000010
	.global AnimationFacing_Kind5
AnimationFacing_Kind5:
	.incbin "baserom.gba", 0x000130ac, 0x00000010
	.global AnimationFacing_Kind8
AnimationFacing_Kind8:
	.incbin "baserom.gba", 0x000130bc, 0x00000008
	.global AnimationFacing_Kind88
AnimationFacing_Kind88:
	.incbin "baserom.gba", 0x000130c4, 0x00000008
	.global AnimationFacing_Kind4
AnimationFacing_Kind4:
	.incbin "baserom.gba", 0x000130cc, 0x00000040
	.global AnimationFacing_Kind6
AnimationFacing_Kind6:
	.incbin "baserom.gba", 0x0001310c, 0x00000040
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001314c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00013190, 0x00000030
	.global ScriptObject_CycleTable
ScriptObject_CycleTable:
	.incbin "baserom.gba", 0x000131c0, 0x00000080
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00013240, 0x00000014
	.global WorldMap_KeyHeadings
WorldMap_KeyHeadings:
	.incbin "baserom.gba", 0x00013254, 0x00000020
	.global WorldMap_FootprintActions
WorldMap_FootprintActions:
	.incbin "baserom.gba", 0x00013274, 0x00000018
	.global WorldMap_KeyYaws
WorldMap_KeyYaws:
	.incbin "baserom.gba", 0x0001328c, 0x00000040
	.global WorldMap_GraphicsResources
WorldMap_GraphicsResources:
	.incbin "baserom.gba", 0x000132cc, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x000132fc, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x000133fc, 0x00000100
	.global Map_TerrainHeightFunctions
Map_TerrainHeightFunctions:
	.incbin "baserom.gba", 0x000134fc, 0x00000040
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
	.incbin "baserom.gba", 0x00013620, 0x00000004
	.global ScriptObject_CommandTable
ScriptObject_CommandTable:
	.incbin "baserom.gba", 0x00013624, 0x000000bc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x000136e0, 0x000000a4
	.global Map_LayeredScenes
Map_LayeredScenes:
	.incbin "baserom.gba", 0x00013784, 0x000008c5
	.section .rom.00029a3c, "ax"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029a3c, 0x00000100
	.global RomBytes_08029a10
	.global UiIcon_FramePointerTable
RomBytes_08029a10:
UiIcon_FramePointerTable:
	.incbin "baserom.gba", 0x00029b3c, 0x000000bc
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.incbin "baserom.gba", 0x00029bf8, 0x0000009c
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.incbin "baserom.gba", 0x00029c94, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00029f2c, 0x000000e4
	.section .rom.000311cc, "ax"
	.incbin "baserom.gba", 0x000311cc, 0x00000004
	.global Resource_FixedBlockBTiles
	.global RomBytes_080310a4
Resource_FixedBlockBTiles:
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000311d0, 0x00000300
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "baserom.gba", 0x000314d0, 0x00000080
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "baserom.gba", 0x00031550, 0x000003c0
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
	.incbin "baserom.gba", 0x00033fa8, 0x0000003c
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.incbin "baserom.gba", 0x00033fe4, 0x00000010
	.global UiSprite_CursorBobOffsets
UiSprite_CursorBobOffsets:
	.incbin "baserom.gba", 0x00033ff4, 0x00000050
	.global UiSprite_WobbleOffsets
UiSprite_WobbleOffsets:
	.incbin "baserom.gba", 0x00034044, 0x00000038
	.global UiSprite_BounceOffsets
UiSprite_BounceOffsets:
	.incbin "baserom.gba", 0x0003407c, 0x00000010
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x0003408c, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x0003448c, 0x00000400
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "baserom.gba", 0x0003488c, 0x00002000
	.global Ui_BounceScales
Ui_BounceScales:
	.incbin "baserom.gba", 0x0003688c, 0x00000048
	.global Menu_CursorBobOffsets
Menu_CursorBobOffsets:
	.incbin "baserom.gba", 0x000368d4, 0x00000010
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x000368e4, 0x00000079
	.global OptionMenu_SpeedFrames
OptionMenu_SpeedFrames:
	.incbin "baserom.gba", 0x0003695d, 0x00000003
	.global OptionMenu_SpeechFrames
OptionMenu_SpeechFrames:
	.incbin "baserom.gba", 0x00036960, 0x00000002
	.global OptionMenu_SleepFrames
OptionMenu_SleepFrames:
	.incbin "baserom.gba", 0x00036962, 0x00000002
	.global OptionMenu_FirstColorPresets
OptionMenu_FirstColorPresets:
	.incbin "baserom.gba", 0x00036964, 0x00000006
	.global OptionMenu_SecondColorPresets
OptionMenu_SecondColorPresets:
	.incbin "baserom.gba", 0x0003696a, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00036970, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00036978, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x000369a0, 0x000000c8
	.global UiSprite_CursorFrameTiles
UiSprite_CursorFrameTiles:
	.incbin "baserom.gba", 0x00036a68, 0x00000800
	.global UiText_GlyphWidths
UiText_GlyphWidths:
	.incbin "baserom.gba", 0x00037268, 0x000000e0
	.global UiText_ColourPens
UiText_ColourPens:
	.incbin "baserom.gba", 0x00037348, 0x00000010
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
	.incbin "baserom.gba", 0x00037358, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x00037374, 0x00000016
	.global NameEntry_CaretWave
NameEntry_CaretWave:
	.incbin "baserom.gba", 0x0003738a, 0x00000008
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
	.incbin "baserom.gba", 0x00037494, 0x00000008
	.global DjinnMenu_BlinkTile
DjinnMenu_BlinkTile:
	.incbin "baserom.gba", 0x0003749c, 0x00000020
	.global OwnerStatus_StatCursors
OwnerStatus_StatCursors:
	.incbin "baserom.gba", 0x000374bc, 0x00000080
	.global OwnerStatus_EquipmentCursors
OwnerStatus_EquipmentCursors:
	.incbin "baserom.gba", 0x0003753c, 0x00000010
	.global OwnerStatus_ConditionCursors
OwnerStatus_ConditionCursors:
	.incbin "baserom.gba", 0x0003754c, 0x00000020
	.global OwnerStatus_NivString
OwnerStatus_NivString:
	.incbin "baserom.gba", 0x0003756c, 0x00000004
	.global OwnerStatus_NvString
OwnerStatus_NvString:
	.incbin "baserom.gba", 0x00037570, 0x00000004
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.incbin "baserom.gba", 0x00037574, 0x00000004
	.global OwnerStatus_KpString
OwnerStatus_KpString:
	.incbin "baserom.gba", 0x00037578, 0x00000004
	.global OwnerStatus_PvString
OwnerStatus_PvString:
	.incbin "baserom.gba", 0x0003757c, 0x00000004
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.incbin "baserom.gba", 0x00037580, 0x00000004
	.global OwnerStatus_SlashString
OwnerStatus_SlashString:
	.incbin "baserom.gba", 0x00037584, 0x00000004
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.incbin "baserom.gba", 0x00037588, 0x00000003
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.incbin "baserom.gba", 0x0003758b, 0x00000004
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.incbin "baserom.gba", 0x0003758f, 0x00000004
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
	.incbin "baserom.gba", 0x000375d0, 0x00000014
	.global Workspace_ItemTitle
Workspace_ItemTitle:
	.incbin "baserom.gba", 0x000375e4, 0x00000008
	.global Workspace_EnergyTitle
Workspace_EnergyTitle:
	.incbin "baserom.gba", 0x000375ec, 0x00000008
	.global Workspace_StatusTitle
Workspace_StatusTitle:
	.incbin "baserom.gba", 0x000375f4, 0x00000008
	.global Workspace_PageLabel
Workspace_PageLabel:
	.incbin "baserom.gba", 0x000375fc, 0x00000008
	.global Workspace_RangeDash
Workspace_RangeDash:
	.incbin "baserom.gba", 0x00037604, 0x00000004
	.section .rom.00072c78, "ax"
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "baserom.gba", 0x00072c78, 0x0000010d
	.global UiText_SpeedDelays
UiText_SpeedDelays:
	.incbin "baserom.gba", 0x00072d85, 0x00000003
	.global UiText_SpeedSteps
UiText_SpeedSteps:
	.incbin "baserom.gba", 0x00072d88, 0x00000003
	.global UiText_SpeedCueGaps
UiText_SpeedCueGaps:
	.incbin "baserom.gba", 0x00072d8b, 0x00000003
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00072d8e, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x00072dd0, 0x00000010
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.incbin "baserom.gba", 0x00072de0, 0x00000104
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x00072ee4, 0x00000004
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
	.global Enemy_Definitions
Enemy_Definitions:
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
	.incbin "baserom.gba", 0x00088e38, 0x00000420
	.global Element_PowerResistCurve
Element_PowerResistCurve:
	.incbin "baserom.gba", 0x00089258, 0x00000014
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008926c, 0x000003b6
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
	.global Scene_AreaNameRules
Scene_AreaNameRules:
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
	.incbin "baserom.gba", 0x0009e626, 0x000001b2
	.global Field_TileMessageOffsets
Field_TileMessageOffsets:
	.incbin "baserom.gba", 0x0009e7d8, 0x00000006
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
	.incbin "baserom.gba", 0x0009e9f8, 0x0000000c
	.global DisplayTransition_NoiseWidths
DisplayTransition_NoiseWidths:
	.incbin "baserom.gba", 0x0009ea04, 0x00000022
	.global DisplayTransition_EdgeOffsets
DisplayTransition_EdgeOffsets:
	.incbin "baserom.gba", 0x0009ea26, 0x00000020
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x0009ea46, 0x00000040
	.global BattlePalette_ChannelCurve
BattlePalette_ChannelCurve:
	.incbin "baserom.gba", 0x0009ea86, 0x00000040
	.global BattlePalette_ChannelCurveLight
BattlePalette_ChannelCurveLight:
	.incbin "baserom.gba", 0x0009eac6, 0x00000040
	.global BattlePalette_ChannelCurveContrast
BattlePalette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x0009eb06, 0x00000042
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x0009eb48, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x0009ed54, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x0009eed8, 0x00000004
	.global DisplayScroll_WaveSine
DisplayScroll_WaveSine:
	.incbin "baserom.gba", 0x0009eedc, 0x00000200
	.global FieldFx_GroundParticleFrames
FieldFx_GroundParticleFrames:
	.incbin "baserom.gba", 0x0009f0dc, 0x000000a0
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x0009f17c, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x0009f1fc, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x0009f208, 0x0000000c
	.global BattleFx_SourceHoldScript
BattleFx_SourceHoldScript:
	.incbin "baserom.gba", 0x0009f214, 0x00000018
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x0009f22c, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x0009f250, 0x00000020
	.global Data_0809f118
Data_0809f118:
	.incbin "baserom.gba", 0x0009f270, 0x00000004
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x0009f274, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x0009f2b8, 0x00000008
	.global WorldMap_MarkerBlendCycle
WorldMap_MarkerBlendCycle:
	.incbin "baserom.gba", 0x0009f2c0, 0x00000020
	.global WorldMap_CursorDirectionAngles
WorldMap_CursorDirectionAngles:
	.incbin "baserom.gba", 0x0009f2e0, 0x00000020
	.section .rom.0009f948, "ax"
	.incbin "baserom.gba", 0x0009f948, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x0009f968, 0x00000030
	.global DisplayTransition_Shapes
DisplayTransition_Shapes:
	.incbin "baserom.gba", 0x0009f998, 0x0000038c
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
	.incbin "baserom.gba", 0x000a0098, 0x00000018
	.global FieldFx_GroundParticleTiles
FieldFx_GroundParticleTiles:
	.incbin "baserom.gba", 0x000a00b0, 0x000000c6
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
	.incbin "baserom.gba", 0x000a0284, 0x0000000c
	.global WorldMap_PlaceMarkers
WorldMap_PlaceMarkers:
	.incbin "baserom.gba", 0x000a0290, 0x0000014c
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
	.incbin "baserom.gba", 0x000aee44, 0x000002c0
	.global Data_080af08c
Data_080af08c:
	.incbin "baserom.gba", 0x000af104, 0x00000180
	.global Ui_LvString
Ui_LvString:
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
	.global Menu_TimesString
Menu_TimesString:
	.incbin "baserom.gba", 0x000af294, 0x00000004
	.global Menu_PointString
Menu_PointString:
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
	.global Menu_SlashString
Menu_SlashString:
	.incbin "baserom.gba", 0x000af2a8, 0x00000004
	.global Menu_HpString
Menu_HpString:
	.incbin "baserom.gba", 0x000af2ac, 0x00000004
	.global Menu_PpString
Menu_PpString:
	.incbin "baserom.gba", 0x000af2b0, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000af2b4, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000af2e4, 0x00000020
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.incbin "baserom.gba", 0x000af304, 0x00000004
	.global DjinnMenu_TextSlash
DjinnMenu_TextSlash:
	.incbin "baserom.gba", 0x000af308, 0x00000004
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
	.global FourObjectMotion_ResourceIds
	.global RomBytes_080af304
FourObjectMotion_ResourceIds:
RomBytes_080af304:
	.incbin "baserom.gba", 0x000af37c, 0x0000000d
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
	.incbin "baserom.gba", 0x000b4ab6, 0x0000000c
	.section .rom.000b5204, "ax"
	.incbin "baserom.gba", 0x000b5204, 0x00000008
	.section .rom.000b5218, "ax"
	.incbin "baserom.gba", 0x000b5218, 0x00000040
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
	.global HitTbl
HitTbl:
	.incbin "baserom.gba", 0x000c2ad0, 0x00000008
	.global EpLossTbl
EpLossTbl:
	.incbin "baserom.gba", 0x000c2ad8, 0x00000018
	.global HpRcvTbl
HpRcvTbl:
	.incbin "baserom.gba", 0x000c2af0, 0x00000018
	.global EpDmgTbl
EpDmgTbl:
	.incbin "baserom.gba", 0x000c2b08, 0x00000018
	.global HpDmgTbl5
HpDmgTbl5:
	.incbin "baserom.gba", 0x000c2b20, 0x00000018
	.global HpDmgTbl8
HpDmgTbl8:
	.incbin "baserom.gba", 0x000c2b38, 0x00000018
	.global HpDmgTbl6
HpDmgTbl6:
	.incbin "baserom.gba", 0x000c2b50, 0x00000018
	.global EpRcvTbl
EpRcvTbl:
	.incbin "baserom.gba", 0x000c2b68, 0x00000018
	.global HpDmgTbl
HpDmgTbl:
	.incbin "baserom.gba", 0x000c2b80, 0x00000018
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.incbin "baserom.gba", 0x000c2b98, 0x00000018
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "baserom.gba", 0x000c2bb0, 0x00000208
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "baserom.gba", 0x000c2db8, 0x0000081c
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
	.global Debug_PartyPresets
Debug_PartyPresets:
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
	.incbin "baserom.gba", 0x000c5a48, 0x00000100
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "baserom.gba", 0x000c5b48, 0x000000e0
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
	.incbin "baserom.gba", 0x000c7438, 0x0000055b
	.section .rom.000e2972, "ax"
	.incbin "baserom.gba", 0x000e2972, 0x00000002
	.section .rom.000eda78, "ax"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000eda78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000eda80, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000eda88, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000eda98, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000edab0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000edab8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000edac0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000edac8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000edad0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000edad8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000edae0, 0x00000008
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
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000eded6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000ededc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000edee8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000edefc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000edf04, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000edf58, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000edf5e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000edf64, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000edf70, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000edf76, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000edf7b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000edf7f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000edf83, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000edf88, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000edf90, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000edfb1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000edfd2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000ee016, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000ee037, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000ee058, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000ee05c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000ee060, 0x00000004
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
	.incbin "baserom.gba", 0x000ee088, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000ee090, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000ee096, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000ee09c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000ee09f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000ee0a2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000ee0aa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000ee0b0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000ee0b3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000ee0b6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000ee0c4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000ee0d6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000ee0e8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000ee10c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000ee11a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000ee128, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000ee134, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000ee140, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000ee158, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000ee15a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000ee163, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000ee16c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000ee174, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000ee177, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000ee17a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000ee17e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000ee184, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000ee188, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000ee18a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000ee18e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000ee194, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000ee19a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000ee1a0, 0x0000000c
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000ee1ac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000ee1b4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000ee1c4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000ee1ca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000ee1d3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000ee1f5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000ee1fb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000ee207, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000ee214, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000ee244, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000ee250, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000ee25e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000ee262, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000ee294, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000ee29a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000ee29d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000ee2a9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000ee2ae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000ee2b4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000ee910, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000ee916, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000ee920, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000ee925, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000ee92a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000ee930, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000ee934, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000ee93e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000ee943, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000ee948, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000ee952, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000ee958, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000ee966, 0x0000000e
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
	.incbin "baserom.gba", 0x000ee9f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000ee9f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000eea08, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000eea20, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000eea2c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000eea38, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000eea41, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000eea44, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000eea4a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000eea50, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000eea56, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000eea62, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000eea88, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000eea91, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000eea99, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000eeaa2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000eeab2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000eeab8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000eeabb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000eeac3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000eeacc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000eeadc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000eeae2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000eeaec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000eeafa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000eeb40, 0x00000008
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000eeb48, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000eeb4b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000eeb4e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000eeb54, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000eeb58, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000eeb5e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000eeb61, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000eeb71, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000eeb79, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000eeb80, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
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
	.incbin "baserom.gba", 0x000eebe6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000eebe9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000eebec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000eec28, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000eec2f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000eec36, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000eec3d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000eec44, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000eec52, 0x00000008
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
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000eecaa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000eecae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000eecb2, 0x00000040
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
	.incbin "baserom.gba", 0x000eed1e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000eed3e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000eed7e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000eed90, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000eed9a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000eeda0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000eeda3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000eeda6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000eedac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000eedb2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000eedb8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000eedbe, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000eedca, 0x00000006
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
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000eedf4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000eedfb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eee02, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000eee10, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000eee17, 0x00000007
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000eee1e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000eee2a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000eee36, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000eee3e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000eee46, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000eee4e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000eee56, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000eee5e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eee66, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000eee76, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000eeea0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000eeebc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000eeeca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000eeed8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000eeee1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000eeeea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000eeef8, 0x0000000e
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000eef06, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000eef0c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000eef12, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000eef18, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000eef28, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000eef30, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000eef3e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000eef4a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000eef50, 0x00000006
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000eef56, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000eef5f, 0x00000009
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
	.incbin "baserom.gba", 0x000ef014, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000ef034, 0x00000020
	.section .rom.000f0a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f0a5c, 0x000007e9
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f1245, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f12a8, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f17ec, 0x000002f3
	.section .rom.000f38bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f38bc, 0x000000c7
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f3983, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f3989, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f39c6, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f3a06, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f3a46, 0x00000040
	.section .rom.000f53dc, "ax"
	.incbin "baserom.gba", 0x000f53dc, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000f53fc, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000f5400, 0x00000008
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000f5408, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000f541a, 0x0000000c
	.section .rom.000f8708, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000f8708, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000f871c, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000f8722, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000f872a, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000f8738, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f8746, 0x00000008
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
	.section .rom.003f20f8, "ax"
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f20f8, 0x00000a6c
	.section .rom.003fe5f0, "ax"
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe5f0, 0x00000a34
	.section .rom.00402044, "ax"
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00402044, 0x00001588
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
