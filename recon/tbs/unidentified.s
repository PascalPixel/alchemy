	.section .unidentified.08007320,"a"
	.incbin "baserom.gba", 0x00007358, 0x0000031e
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
	.section .unidentified.080079b0,"a"
	.global Save_Signature
Save_Signature:
	.incbin "baserom.gba", 0x000079b0, 0x00000008
	.global Save_HeaderTemplate
Save_HeaderTemplate:
	.incbin "baserom.gba", 0x000079b8, 0x00000008
	.incbin "baserom.gba", 0x000079c0, 0x0000004c
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00007a0c, 0x00000014
	.section .unidentified.08007a68,"a"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .unidentified.08007aa4,"a"
	.incbin "baserom.gba", 0x00007aa4, 0x00000010
	.incbin "baserom.gba", 0x00007ab4, 0x00000008
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00007abc, 0x00000058
	.section .unidentified.08007b38,"a"
	.incbin "baserom.gba", 0x00007b38, 0x0000008c
	.section .unidentified.08007bcc,"a"
	.incbin "baserom.gba", 0x00007bcc, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00007be4, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00007c10, 0x0000002c
	.section .unidentified.080092b8,"a"
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "baserom.gba", 0x000092b8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000097b8, 0x00000400
	.section .unidentified.08012f20,"a"
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
	.section .unidentified.08029910,"a"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029910, 0x00000100
	.global RomBytes_08029a10
RomBytes_08029a10:
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.incbin "baserom.gba", 0x00029a10, 0x000000bc
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.incbin "baserom.gba", 0x00029acc, 0x0000009c
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.incbin "baserom.gba", 0x00029b68, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00029e00, 0x000000e4
	.section .unidentified.080310a0,"a"
	.incbin "baserom.gba", 0x000310a0, 0x00000004
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000310a4, 0x00000300
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "baserom.gba", 0x000313a4, 0x00000080
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "baserom.gba", 0x00031424, 0x000003c0
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x000317e4, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00031864, 0x000005c0
	.global UiText_SecondGlyphs
UiText_SecondGlyphs:
	.incbin "baserom.gba", 0x00031e24, 0x00000400
	.section .unidentified.08033e24,"a"
	.incbin "baserom.gba", 0x00033e24, 0x0000001c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00033e40, 0x00000020
	.global UiSprite_CursorBobOffsets
UiSprite_CursorBobOffsets:
	.incbin "baserom.gba", 0x00033e60, 0x00000050
	.global UiSprite_WobbleOffsets
UiSprite_WobbleOffsets:
	.incbin "baserom.gba", 0x00033eb0, 0x00000038
	.global UiSprite_BounceOffsets
UiSprite_BounceOffsets:
	.incbin "baserom.gba", 0x00033ee8, 0x00000010
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00033ef8, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000342f8, 0x00000400
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "baserom.gba", 0x000346f8, 0x00002000
	.global Ui_BounceScales
Ui_BounceScales:
	.incbin "baserom.gba", 0x000366f8, 0x00000048
	.global Menu_CursorBobOffsets
Menu_CursorBobOffsets:
	.incbin "baserom.gba", 0x00036740, 0x00000010
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00036750, 0x00000079
	.global OptionMenu_SpeedFrames
OptionMenu_SpeedFrames:
	.incbin "baserom.gba", 0x000367c9, 0x00000003
	.global OptionMenu_SpeechFrames
OptionMenu_SpeechFrames:
	.incbin "baserom.gba", 0x000367cc, 0x00000002
	.global OptionMenu_SleepFrames
OptionMenu_SleepFrames:
	.incbin "baserom.gba", 0x000367ce, 0x00000002
	.global OptionMenu_FirstColorPresets
OptionMenu_FirstColorPresets:
	.incbin "baserom.gba", 0x000367d0, 0x00000006
	.global OptionMenu_SecondColorPresets
OptionMenu_SecondColorPresets:
	.incbin "baserom.gba", 0x000367d6, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x000367dc, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x000367e4, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x0003680c, 0x000000c8
	.global UiSprite_CursorFrameTiles
UiSprite_CursorFrameTiles:
	.incbin "baserom.gba", 0x000368d4, 0x00000800
	.global UiText_GlyphWidths
UiText_GlyphWidths:
	.incbin "baserom.gba", 0x000370d4, 0x000000e0
	.global UiText_ColourPens
UiText_ColourPens:
	.incbin "baserom.gba", 0x000371b4, 0x00000010
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
	.incbin "baserom.gba", 0x000371c4, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000371e0, 0x00000016
	.global NameEntry_CaretWave
NameEntry_CaretWave:
	.incbin "baserom.gba", 0x000371f6, 0x00000008
	.global Data_080371fe
Data_080371fe:
	.incbin "baserom.gba", 0x000371fe, 0x00000008
	.global Party_CharacterValues
Party_CharacterValues:
	.incbin "baserom.gba", 0x00037206, 0x00000010
	.global Party_CharacterValuesFlag32
Party_CharacterValuesFlag32:
	.incbin "baserom.gba", 0x00037216, 0x00000010
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
	.incbin "baserom.gba", 0x00037226, 0x0000000a
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.incbin "baserom.gba", 0x00037230, 0x00000020
	.global Data_08037250
Data_08037250:
	.incbin "baserom.gba", 0x00037250, 0x00000030
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "baserom.gba", 0x00037280, 0x00000040
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.incbin "baserom.gba", 0x000372c0, 0x00000040
	.global Link_TimeLabelString
Link_TimeLabelString:
	.incbin "baserom.gba", 0x00037300, 0x00000008
	.global DjinnMenu_BlinkTile
DjinnMenu_BlinkTile:
	.incbin "baserom.gba", 0x00037308, 0x00000020
	.global OwnerStatus_StatCursors
OwnerStatus_StatCursors:
	.incbin "baserom.gba", 0x00037328, 0x00000080
	.global OwnerStatus_EquipmentCursors
OwnerStatus_EquipmentCursors:
	.incbin "baserom.gba", 0x000373a8, 0x00000010
	.global OwnerStatus_ConditionCursors
OwnerStatus_ConditionCursors:
	.incbin "baserom.gba", 0x000373b8, 0x00000020
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.incbin "baserom.gba", 0x000373d8, 0x00000004
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.incbin "baserom.gba", 0x000373dc, 0x00000004
	.global OwnerStatus_SlashString
OwnerStatus_SlashString:
	.incbin "baserom.gba", 0x000373e0, 0x00000004
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.incbin "baserom.gba", 0x000373e4, 0x00000003
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.incbin "baserom.gba", 0x000373e7, 0x00000003
	.incbin "baserom.gba", 0x000373ea, 0x00000001
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.incbin "baserom.gba", 0x000373eb, 0x00000004
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.incbin "baserom.gba", 0x000373ef, 0x00000008
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.incbin "baserom.gba", 0x000373f7, 0x0000000c
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.incbin "baserom.gba", 0x00037403, 0x0000000c
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.incbin "baserom.gba", 0x0003740f, 0x00000019
	.global Menu_ColonString
Menu_ColonString:
	.incbin "baserom.gba", 0x00037428, 0x00000004
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.incbin "baserom.gba", 0x0003742c, 0x00000014
	.global Workspace_ItemTitle
Workspace_ItemTitle:
	.incbin "baserom.gba", 0x00037440, 0x00000008
	.global Workspace_EnergyTitle
Workspace_EnergyTitle:
	.incbin "baserom.gba", 0x00037448, 0x00000008
	.global Workspace_StatusTitle
Workspace_StatusTitle:
	.incbin "baserom.gba", 0x00037450, 0x00000008
	.global Workspace_PageLabel
Workspace_PageLabel:
	.incbin "baserom.gba", 0x00037458, 0x00000008
	.global Workspace_RangeDash
Workspace_RangeDash:
	.incbin "baserom.gba", 0x00037460, 0x00000004
	.section .unidentified.08073808,"a"
	.global UiText_SpeedDelays
UiText_SpeedDelays:
	.incbin "baserom.gba", 0x00073808, 0x00000003
	.global UiText_SpeedSteps
UiText_SpeedSteps:
	.incbin "baserom.gba", 0x0007380b, 0x00000003
	.global UiText_SpeedCueGaps
UiText_SpeedCueGaps:
	.incbin "baserom.gba", 0x0007380e, 0x00000004
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073812, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x00073854, 0x00000010
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.incbin "baserom.gba", 0x00073864, 0x00000104
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x00073968, 0x00000004
	.section .unidentified.0807a828,"a"
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
	.section .unidentified.0809c410,"a"
	.global Data_0809c410
Data_0809c410:
	.incbin "baserom.gba", 0x0009c410, 0x00000100
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "baserom.gba", 0x0009c510, 0x00000100
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "baserom.gba", 0x0009c610, 0x00000b60
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d170, 0x00000638
	.global BattleFx_ResultRules
BattleFx_ResultRules:
	.incbin "baserom.gba", 0x0009d7a8, 0x00000108
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "baserom.gba", 0x0009d8b0, 0x00000140
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x0009d9f0, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x0009ddd8, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x0009e1d8, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x0009e270, 0x00000218
	.global gBattleCueTable
gBattleCueTable:
	.incbin "baserom.gba", 0x0009e488, 0x00000046
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x0009e4ce, 0x000001b2
	.global Field_TileMessageOffsets
Field_TileMessageOffsets:
	.incbin "baserom.gba", 0x0009e680, 0x00000006
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.incbin "baserom.gba", 0x0009e686, 0x00000032
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.incbin "baserom.gba", 0x0009e6b8, 0x00000008
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.incbin "baserom.gba", 0x0009e6c0, 0x0000009c
	.global RomBytes_0809e75c
RomBytes_0809e75c:
	.incbin "baserom.gba", 0x0009e75c, 0x00000120
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.incbin "baserom.gba", 0x0009e87c, 0x00000024
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.incbin "baserom.gba", 0x0009e8a0, 0x0000000c
	.global DisplayTransition_NoiseWidths
DisplayTransition_NoiseWidths:
	.incbin "baserom.gba", 0x0009e8ac, 0x00000022
	.global DisplayTransition_EdgeOffsets
DisplayTransition_EdgeOffsets:
	.incbin "baserom.gba", 0x0009e8ce, 0x00000020
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x0009e8ee, 0x00000040
	.global BattlePalette_ChannelCurve
BattlePalette_ChannelCurve:
	.incbin "baserom.gba", 0x0009e92e, 0x00000040
	.global BattlePalette_ChannelCurveLight
BattlePalette_ChannelCurveLight:
	.incbin "baserom.gba", 0x0009e96e, 0x00000040
	.global BattlePalette_ChannelCurveContrast
BattlePalette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x0009e9ae, 0x00000042
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x0009e9f0, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x0009ebfc, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x0009ed80, 0x00000004
	.global DisplayScroll_WaveSine
DisplayScroll_WaveSine:
	.incbin "baserom.gba", 0x0009ed84, 0x00000200
	.global FieldFx_GroundParticleFrames
FieldFx_GroundParticleFrames:
	.incbin "baserom.gba", 0x0009ef84, 0x000000a0
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x0009f024, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x0009f0a4, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x0009f0b0, 0x0000000c
	.global BattleFx_SourceHoldScript
BattleFx_SourceHoldScript:
	.incbin "baserom.gba", 0x0009f0bc, 0x00000018
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x0009f0d4, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x0009f0f8, 0x00000020
	.global Data_0809f118
Data_0809f118:
	.incbin "baserom.gba", 0x0009f118, 0x00000004
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x0009f11c, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x0009f160, 0x00000008
	.global WorldMap_MarkerBlendCycle
WorldMap_MarkerBlendCycle:
	.incbin "baserom.gba", 0x0009f168, 0x00000020
	.global WorldMap_CursorDirectionAngles
WorldMap_CursorDirectionAngles:
	.incbin "baserom.gba", 0x0009f188, 0x00000020
	.section .unidentified.0809f7f0,"a"
	.incbin "baserom.gba", 0x0009f7f0, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x0009f810, 0x00000030
	.global DisplayTransition_Shapes
DisplayTransition_Shapes:
	.incbin "baserom.gba", 0x0009f840, 0x0000038c
	.global ObjectMotion_LaunchScript
ObjectMotion_LaunchScript:
	.incbin "baserom.gba", 0x0009fbcc, 0x00000020
	.global BattleFx_BurstParticleScriptA
BattleFx_BurstParticleScriptA:
	.incbin "baserom.gba", 0x0009fbec, 0x00000018
	.global BattleFx_BurstParticleScriptB
BattleFx_BurstParticleScriptB:
	.incbin "baserom.gba", 0x0009fc04, 0x00000018
	.global ObjectMotion_StepAngleScript
ObjectMotion_StepAngleScript:
	.incbin "baserom.gba", 0x0009fc1c, 0x0000000c
	.global Ui_RenderResultValues
Ui_RenderResultValues:
	.incbin "baserom.gba", 0x0009fc28, 0x00000004
	.global BattleFx_LinkedObjectScript
BattleFx_LinkedObjectScript:
	.incbin "baserom.gba", 0x0009fc2c, 0x0000010c
	.global Data_0809fd38
Data_0809fd38:
	.incbin "baserom.gba", 0x0009fd38, 0x0000000c
	.global ObjectMotion_ActionKind2Script
ObjectMotion_ActionKind2Script:
	.incbin "baserom.gba", 0x0009fd44, 0x000000bc
	.global ObjectMotion_ActionKind1Script
ObjectMotion_ActionKind1Script:
	.incbin "baserom.gba", 0x0009fe00, 0x00000004
	.global ObjectMotion_ResetActionScript
ObjectMotion_ResetActionScript:
	.incbin "baserom.gba", 0x0009fe04, 0x0000000c
	.global ObjectMotion_ActionKind3Script
ObjectMotion_ActionKind3Script:
	.incbin "baserom.gba", 0x0009fe10, 0x000000bc
	.global ObjectMotion_ActionKind4Script
ObjectMotion_ActionKind4Script:
	.incbin "baserom.gba", 0x0009fecc, 0x0000004c
	.global ObjectMotion_MoveTowardTargetScript
ObjectMotion_MoveTowardTargetScript:
	.incbin "baserom.gba", 0x0009ff18, 0x00000014
	.global ObjectMotion_TurnTowardLinkedScript
ObjectMotion_TurnTowardLinkedScript:
	.incbin "baserom.gba", 0x0009ff2c, 0x00000014
	.global ObjectMotion_LinkedActionScript
ObjectMotion_LinkedActionScript:
	.incbin "baserom.gba", 0x0009ff40, 0x00000018
	.global FieldFx_GroundParticleTiles
FieldFx_GroundParticleTiles:
	.incbin "baserom.gba", 0x0009ff58, 0x000000c6
	.global FieldFx_MoteTiles
FieldFx_MoteTiles:
	.incbin "baserom.gba", 0x000a001e, 0x0000009a
	.global Data_080a00b8
Data_080a00b8:
	.incbin "baserom.gba", 0x000a00b8, 0x00000050
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a0108, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a0128, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a012c, 0x0000000c
	.global WorldMap_PlaceMarkers
WorldMap_PlaceMarkers:
	.incbin "baserom.gba", 0x000a0138, 0x0000014c
	.section .unidentified.080aea4c,"a"
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "baserom.gba", 0x000aea4c, 0x00000100
	.global Data_080aeb4c
Data_080aeb4c:
	.incbin "baserom.gba", 0x000aeb4c, 0x00000080
	.global Data_080aebcc
Data_080aebcc:
	.incbin "baserom.gba", 0x000aebcc, 0x00000180
	.global Data_080aed4c
Data_080aed4c:
	.incbin "baserom.gba", 0x000aed4c, 0x00000080
	.global Data_080aedcc
Data_080aedcc:
	.incbin "baserom.gba", 0x000aedcc, 0x000002c0
	.global Data_080af08c
Data_080af08c:
	.incbin "baserom.gba", 0x000af08c, 0x00000180
	.global Ui_LvString
Ui_LvString:
	.incbin "baserom.gba", 0x000af20c, 0x00000004
	.global Ui_HpString
Ui_HpString:
	.incbin "baserom.gba", 0x000af210, 0x00000004
	.global Ui_SlashString
Ui_SlashString:
	.incbin "baserom.gba", 0x000af214, 0x00000004
	.global Ui_PpString
Ui_PpString:
	.incbin "baserom.gba", 0x000af218, 0x00000004
	.global Menu_TimesString
Menu_TimesString:
	.incbin "baserom.gba", 0x000af21c, 0x00000004
	.global Menu_PointString
Menu_PointString:
	.incbin "baserom.gba", 0x000af220, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000af224, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000af228, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000af22c, 0x00000004
	.global Menu_SlashString
Menu_SlashString:
	.incbin "baserom.gba", 0x000af230, 0x00000004
	.global Menu_HpString
Menu_HpString:
	.incbin "baserom.gba", 0x000af234, 0x00000004
	.global Menu_PpString
Menu_PpString:
	.incbin "baserom.gba", 0x000af238, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000af23c, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000af26c, 0x00000020
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.incbin "baserom.gba", 0x000af28c, 0x00000004
	.global DjinnMenu_TextSlash
DjinnMenu_TextSlash:
	.incbin "baserom.gba", 0x000af290, 0x00000004
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000af294, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000af29d, 0x00000009
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.incbin "baserom.gba", 0x000af2a6, 0x0000000b
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.incbin "baserom.gba", 0x000af2b1, 0x0000000b
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.incbin "baserom.gba", 0x000af2bc, 0x00000014
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.incbin "baserom.gba", 0x000af2d0, 0x00000014
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.incbin "baserom.gba", 0x000af2e4, 0x00000018
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.incbin "baserom.gba", 0x000af2fc, 0x00000008
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.global RomBytes_080af304
RomBytes_080af304:
	.incbin "baserom.gba", 0x000af304, 0x0000000d
	.section .unidentified.080b3940,"a"
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
	.incbin "baserom.gba", 0x000b3bc0, 0x00000080
	.incbin "baserom.gba", 0x000b3c40, 0x00000100
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "baserom.gba", 0x000b3d40, 0x00000140
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "baserom.gba", 0x000b3e80, 0x00000100
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "baserom.gba", 0x000b3f80, 0x00000100
	.incbin "baserom.gba", 0x000b4080, 0x00000080
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
	.section .unidentified.080b5204,"a"
	.incbin "baserom.gba", 0x000b5204, 0x00000008
	.section .unidentified.080b5218,"a"
	.incbin "baserom.gba", 0x000b5218, 0x00000040
	.section .unidentified.080c2a0a,"a"
	.incbin "baserom.gba", 0x000c2a0a, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000c2a10, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000c2a1c, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000c2a2a, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000c2a38, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000c2a46, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000c2a54, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000c2a62, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000c2a7c, 0x0000003c
	.global HitTbl
HitTbl:
	.incbin "baserom.gba", 0x000c2ab8, 0x00000008
	.global EpLossTbl
EpLossTbl:
	.incbin "baserom.gba", 0x000c2ac0, 0x00000018
	.global HpRcvTbl
HpRcvTbl:
	.incbin "baserom.gba", 0x000c2ad8, 0x00000018
	.global EpDmgTbl
EpDmgTbl:
	.incbin "baserom.gba", 0x000c2af0, 0x00000018
	.global HpDmgTbl5
HpDmgTbl5:
	.incbin "baserom.gba", 0x000c2b08, 0x00000018
	.global HpDmgTbl8
HpDmgTbl8:
	.incbin "baserom.gba", 0x000c2b20, 0x00000018
	.global HpDmgTbl6
HpDmgTbl6:
	.incbin "baserom.gba", 0x000c2b38, 0x00000018
	.global EpRcvTbl
EpRcvTbl:
	.incbin "baserom.gba", 0x000c2b50, 0x00000018
	.global HpDmgTbl
HpDmgTbl:
	.incbin "baserom.gba", 0x000c2b68, 0x00000018
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.incbin "baserom.gba", 0x000c2b80, 0x00000018
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "baserom.gba", 0x000c2b98, 0x00000208
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "baserom.gba", 0x000c2da0, 0x0000081c
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000c35bc, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000c3604, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000c3620, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000c3628, 0x0000010c
	.global BattlePres_AdvanceArrowTiles
BattlePres_AdvanceArrowTiles:
	.incbin "baserom.gba", 0x000c3734, 0x00000800
	.global Debug_PartyPresets
Debug_PartyPresets:
	.incbin "baserom.gba", 0x000c3f34, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000c5938, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000c593c, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000c59a4, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000c59c4, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000c59e4, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000c5a04, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000c5a30, 0x00000100
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "baserom.gba", 0x000c5b30, 0x000000e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c5c10, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000c5c38, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000c73f8, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000c7420, 0x0000055b
	.section .unidentified.080eda78,"a"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000eda78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000eda80, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.global Data_080eda88
Data_080eda88:
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
	.section .unidentified.080f0ae0,"a"
	.incbin "baserom.gba", 0x000f0ae0, 0x000006dc
	.incbin "baserom.gba", 0x000f11bc, 0x00000001
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f11bd, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f1220, 0x00000550
	.section .unidentified.080f38bc,"a"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f38bc, 0x000000ee
	.incbin "baserom.gba", 0x000f39aa, 0x00000001
	.section .unidentified.080fc504,"a"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .unidentified.0832b0a6,"a"
	.incbin "baserom.gba", 0x0032b0a6, 0x00000002
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b0a8, 0x000086f8
	.section .unidentified.0833ac08,"a"
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033ac08, 0x000002b4
	.section .unidentified.085d764a,"a"
	.incbin "baserom.gba", 0x005d764a, 0x00000002
	.section .unidentified.085eda62,"a"
	.incbin "baserom.gba", 0x005eda62, 0x00000002
	.section .unidentified.08609520,"a"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.section .unidentified.08691135,"a"
	.incbin "baserom.gba", 0x00691135, 0x00000003
	.section .unidentified.086d58f5,"a"
	.incbin "baserom.gba", 0x006d58f5, 0x00000003
	.section .unidentified.08717b8c,"a"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.section .unidentified.087458f7,"a"
	.incbin "baserom.gba", 0x007458f7, 0x00000001
	.section .unidentified.0875bdd8,"a"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
