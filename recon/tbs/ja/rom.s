	.section .rom.00007320, "ax"
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
	.section .rom.000079b0, "ax"
	.global Save_Signature
Save_Signature:
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
	.section .rom.000092b8, "ax"
	.global ResourceSlot_ConversionTables
ResourceSlot_ConversionTables:
	.incbin "baserom.gba", 0x000092b8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000097b8, 0x00000400
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00012e80, "ax"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012e80, 0x00000080
	.global ResourceSlot_NumberTable
ResourceSlot_NumberTable:
	.incbin "baserom.gba", 0x00012f00, 0x000000dc
	.global AnimationFacing_Kind1
AnimationFacing_Kind1:
	.incbin "baserom.gba", 0x00012fdc, 0x00000010
	.global AnimationFacing_Kind22
AnimationFacing_Kind22:
	.incbin "baserom.gba", 0x00012fec, 0x00000008
	.global AnimationFacing_Kind2
AnimationFacing_Kind2:
	.incbin "baserom.gba", 0x00012ff4, 0x00000008
	.global AnimationFacing_Kind3
AnimationFacing_Kind3:
	.incbin "baserom.gba", 0x00012ffc, 0x00000010
	.global AnimationFacing_Kind5
AnimationFacing_Kind5:
	.incbin "baserom.gba", 0x0001300c, 0x00000010
	.global AnimationFacing_Kind8
AnimationFacing_Kind8:
	.incbin "baserom.gba", 0x0001301c, 0x00000008
	.global AnimationFacing_Kind88
AnimationFacing_Kind88:
	.incbin "baserom.gba", 0x00013024, 0x00000008
	.global AnimationFacing_Kind4
AnimationFacing_Kind4:
	.incbin "baserom.gba", 0x0001302c, 0x00000040
	.global AnimationFacing_Kind6
AnimationFacing_Kind6:
	.incbin "baserom.gba", 0x0001306c, 0x00000040
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x000130ac, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x000130f0, 0x00000030
	.global ScriptObject_CycleTable
ScriptObject_CycleTable:
	.incbin "baserom.gba", 0x00013120, 0x00000080
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x000131a0, 0x00000014
	.global WorldMap_KeyHeadings
WorldMap_KeyHeadings:
	.incbin "baserom.gba", 0x000131b4, 0x00000020
	.global WorldMap_FootprintActions
WorldMap_FootprintActions:
	.incbin "baserom.gba", 0x000131d4, 0x00000018
	.global WorldMap_KeyYaws
WorldMap_KeyYaws:
	.incbin "baserom.gba", 0x000131ec, 0x00000040
	.global WorldMap_GraphicsResources
WorldMap_GraphicsResources:
	.incbin "baserom.gba", 0x0001322c, 0x00000030
	.global Curve_LerpWeightTable
Curve_LerpWeightTable:
	.incbin "baserom.gba", 0x0001325c, 0x00000100
	.global Curve_SampleIndexTable
Curve_SampleIndexTable:
	.incbin "baserom.gba", 0x0001335c, 0x00000100
	.global Map_TerrainHeightFunctions
Map_TerrainHeightFunctions:
	.incbin "baserom.gba", 0x0001345c, 0x00000040
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
	.incbin "baserom.gba", 0x00013580, 0x00000004
	.global ScriptObject_CommandTable
ScriptObject_CommandTable:
	.incbin "baserom.gba", 0x00013584, 0x000000bc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x00013640, 0x000000a4
	.global Map_LayeredScenes
Map_LayeredScenes:
	.incbin "baserom.gba", 0x000136e4, 0x000008c5
	.section .rom.00029968, "ax"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029968, 0x00000100
	.global RomBytes_08029a10
	.global UiIcon_FramePointerTable
RomBytes_08029a10:
UiIcon_FramePointerTable:
	.incbin "baserom.gba", 0x00029a68, 0x000000bc
	.global UiIcon_MarkPointers
UiIcon_MarkPointers:
	.incbin "baserom.gba", 0x00029b24, 0x0000009c
	.global UiIcon_DigitPointers
UiIcon_DigitPointers:
	.incbin "baserom.gba", 0x00029bc0, 0x00000298
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
	.global RomBytes_080310a4
Resource_FixedBlockBTiles:
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000310f0, 0x00000300
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "baserom.gba", 0x000313f0, 0x00000080
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "baserom.gba", 0x00031470, 0x000003c0
	.global RomBytes_080317e4
RomBytes_080317e4:
	.incbin "baserom.gba", 0x00031830, 0x00000080
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x000318b0, 0x00000bc0
	.section .rom.000345e0, "ax"
	.global UiSprite_CursorBobOffsets
UiSprite_CursorBobOffsets:
	.incbin "baserom.gba", 0x000345e8, 0x00000050
	.global UiSprite_WobbleOffsets
UiSprite_WobbleOffsets:
	.incbin "baserom.gba", 0x00034638, 0x00000038
	.global UiSprite_BounceOffsets
UiSprite_BounceOffsets:
	.incbin "baserom.gba", 0x00034670, 0x00000010
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00034680, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x00034a80, 0x00000400
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "baserom.gba", 0x00034e80, 0x00002000
	.global Ui_BounceScales
Ui_BounceScales:
	.incbin "baserom.gba", 0x00036e80, 0x00000048
	.global Menu_CursorBobOffsets
Menu_CursorBobOffsets:
	.incbin "baserom.gba", 0x00036ec8, 0x00000010
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00036ed8, 0x00000079
	.global OptionMenu_SpeedFrames
OptionMenu_SpeedFrames:
	.incbin "baserom.gba", 0x00036f51, 0x00000003
	.global OptionMenu_SpeechFrames
OptionMenu_SpeechFrames:
	.incbin "baserom.gba", 0x00036f54, 0x00000002
	.global OptionMenu_SleepFrames
OptionMenu_SleepFrames:
	.incbin "baserom.gba", 0x00036f56, 0x00000002
	.global OptionMenu_FirstColorPresets
OptionMenu_FirstColorPresets:
	.incbin "baserom.gba", 0x00036f58, 0x00000006
	.global OptionMenu_SecondColorPresets
OptionMenu_SecondColorPresets:
	.incbin "baserom.gba", 0x00036f5e, 0x00000006
	.global Menu_WorkspaceIconFrames
Menu_WorkspaceIconFrames:
	.incbin "baserom.gba", 0x00036f64, 0x00000008
	.global SideObject_CharacterIdMap
SideObject_CharacterIdMap:
	.incbin "baserom.gba", 0x00036f6c, 0x00000028
	.global SideObject_ActorKindIdMap
SideObject_ActorKindIdMap:
	.incbin "baserom.gba", 0x00036f94, 0x000000c8
	.global UiSprite_CursorFrameTiles
UiSprite_CursorFrameTiles:
	.incbin "baserom.gba", 0x0003705c, 0x00000800
	.global UiText_GlyphWidths
UiText_GlyphWidths:
	.incbin "baserom.gba", 0x0003785c, 0x00000060
	.global UiText_ColourPens
UiText_ColourPens:
	.incbin "baserom.gba", 0x000378bc, 0x00000010
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
	.incbin "baserom.gba", 0x000378cc, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000378e8, 0x00000002
	.global NameEntry_DakutenRanges
NameEntry_DakutenRanges:
	.incbin "baserom.gba", 0x000378ea, 0x0000000f
	.global NameEntry_HandakutenRanges
NameEntry_HandakutenRanges:
	.incbin "baserom.gba", 0x000378f9, 0x00000005
	.global NameEntry_CaretWave
NameEntry_CaretWave:
	.incbin "baserom.gba", 0x000378fe, 0x00000008
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
	.incbin "baserom.gba", 0x00037a08, 0x00000008
	.global DjinnMenu_BlinkTile
DjinnMenu_BlinkTile:
	.incbin "baserom.gba", 0x00037a10, 0x00000020
	.global OwnerStatus_StatCursors
OwnerStatus_StatCursors:
	.incbin "baserom.gba", 0x00037a30, 0x00000080
	.global OwnerStatus_EquipmentCursors
OwnerStatus_EquipmentCursors:
	.incbin "baserom.gba", 0x00037ab0, 0x00000010
	.global OwnerStatus_ConditionCursors
OwnerStatus_ConditionCursors:
	.incbin "baserom.gba", 0x00037ac0, 0x00000020
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.incbin "baserom.gba", 0x00037ae0, 0x00000004
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.incbin "baserom.gba", 0x00037ae4, 0x00000004
	.global OwnerStatus_SlashString
OwnerStatus_SlashString:
	.incbin "baserom.gba", 0x00037ae8, 0x00000004
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.incbin "baserom.gba", 0x00037aec, 0x00000003
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.incbin "baserom.gba", 0x00037aef, 0x00000004
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.incbin "baserom.gba", 0x00037af3, 0x00000004
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
	.incbin "baserom.gba", 0x00037b34, 0x00000014
	.global Workspace_ItemTitle
Workspace_ItemTitle:
	.incbin "baserom.gba", 0x00037b48, 0x00000008
	.global Workspace_EnergyTitle
Workspace_EnergyTitle:
	.incbin "baserom.gba", 0x00037b50, 0x00000008
	.global Workspace_StatusTitle
Workspace_StatusTitle:
	.incbin "baserom.gba", 0x00037b58, 0x00000008
	.global Workspace_PageLabel
Workspace_PageLabel:
	.incbin "baserom.gba", 0x00037b60, 0x00000008
	.global Workspace_RangeDash
Workspace_RangeDash:
	.incbin "baserom.gba", 0x00037b68, 0x00000004
	.section .rom.0006c1a0, "ax"
	.global UiText_SpeedDelays
UiText_SpeedDelays:
	.incbin "baserom.gba", 0x0006c1a0, 0x00000003
	.global UiText_SpeedSteps
UiText_SpeedSteps:
	.incbin "baserom.gba", 0x0006c1a3, 0x00000003
	.global UiText_SpeedCueGaps
UiText_SpeedCueGaps:
	.incbin "baserom.gba", 0x0006c1a6, 0x00000004
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x0006c1aa, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x0006c1ec, 0x00000010
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.incbin "baserom.gba", 0x0006c1fc, 0x0000015a
	.global NameEntry_AlternateKeyboardTilemap
NameEntry_AlternateKeyboardTilemap:
	.incbin "baserom.gba", 0x0006c356, 0x0000023e
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0006c594, 0x00000004
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
	.global Enemy_Definitions
Enemy_Definitions:
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
	.incbin "baserom.gba", 0x0007ee38, 0x00000420
	.global Element_PowerResistCurve
Element_PowerResistCurve:
	.incbin "baserom.gba", 0x0007f258, 0x00000014
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0007f26c, 0x000003b6
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
	.global Scene_AreaNameRules
Scene_AreaNameRules:
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
	.incbin "baserom.gba", 0x000954b2, 0x000001b2
	.global Field_TileMessageOffsets
Field_TileMessageOffsets:
	.incbin "baserom.gba", 0x00095664, 0x00000006
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
	.incbin "baserom.gba", 0x00095884, 0x0000000c
	.global DisplayTransition_NoiseWidths
DisplayTransition_NoiseWidths:
	.incbin "baserom.gba", 0x00095890, 0x00000022
	.global DisplayTransition_EdgeOffsets
DisplayTransition_EdgeOffsets:
	.incbin "baserom.gba", 0x000958b2, 0x00000020
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.incbin "baserom.gba", 0x000958d2, 0x00000040
	.global BattlePalette_ChannelCurve
BattlePalette_ChannelCurve:
	.incbin "baserom.gba", 0x00095912, 0x00000040
	.global BattlePalette_ChannelCurveLight
BattlePalette_ChannelCurveLight:
	.incbin "baserom.gba", 0x00095952, 0x00000040
	.global BattlePalette_ChannelCurveContrast
BattlePalette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x00095992, 0x00000042
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x000959d4, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x00095be0, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x00095d64, 0x00000004
	.global DisplayScroll_WaveSine
DisplayScroll_WaveSine:
	.incbin "baserom.gba", 0x00095d68, 0x00000200
	.global FieldFx_GroundParticleFrames
FieldFx_GroundParticleFrames:
	.incbin "baserom.gba", 0x00095f68, 0x000000a0
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x00096008, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x00096088, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x00096094, 0x0000000c
	.global BattleFx_SourceHoldScript
BattleFx_SourceHoldScript:
	.incbin "baserom.gba", 0x000960a0, 0x00000018
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x000960b8, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x000960dc, 0x00000020
	.global Data_0809f118
Data_0809f118:
	.incbin "baserom.gba", 0x000960fc, 0x00000004
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
	.incbin "baserom.gba", 0x000967f4, 0x00000030
	.global DisplayTransition_Shapes
DisplayTransition_Shapes:
	.incbin "baserom.gba", 0x00096824, 0x0000038c
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
	.incbin "baserom.gba", 0x00096f24, 0x00000018
	.global FieldFx_GroundParticleTiles
FieldFx_GroundParticleTiles:
	.incbin "baserom.gba", 0x00096f3c, 0x000000c6
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
	.incbin "baserom.gba", 0x0009711c, 0x0000014c
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
	.incbin "baserom.gba", 0x000a5cec, 0x000002c0
	.global Data_080af08c
Data_080af08c:
	.incbin "baserom.gba", 0x000a5fac, 0x00000180
	.global Ui_LvString
Ui_LvString:
	.incbin "baserom.gba", 0x000a612c, 0x00000004
	.global Ui_HpRowString
Ui_HpRowString:
	.incbin "baserom.gba", 0x000a6130, 0x00000008
	.global Ui_EpRowString
Ui_EpRowString:
	.incbin "baserom.gba", 0x000a6138, 0x00000008
	.global ItemMenu_ArrangeKeysString
ItemMenu_ArrangeKeysString:
	.incbin "baserom.gba", 0x000a6140, 0x00000008
	.global ItemMenu_EquipmentKeyString
ItemMenu_EquipmentKeyString:
	.incbin "baserom.gba", 0x000a6148, 0x00000004
	.global Menu_PointString
Menu_PointString:
	.incbin "baserom.gba", 0x000a614c, 0x00000004
	.global Menu_PlusSignString
Menu_PlusSignString:
	.incbin "baserom.gba", 0x000a6150, 0x00000004
	.global Menu_MinusSignString
Menu_MinusSignString:
	.incbin "baserom.gba", 0x000a6154, 0x00000004
	.global Menu_ShortcutLString
Menu_ShortcutLString:
	.incbin "baserom.gba", 0x000a6158, 0x00000004
	.global Menu_ShortcutLColonString
Menu_ShortcutLColonString:
	.incbin "baserom.gba", 0x000a615c, 0x00000004
	.global Menu_ShortcutRString
Menu_ShortcutRString:
	.incbin "baserom.gba", 0x000a6160, 0x00000004
	.global Menu_ShortcutRColonString
Menu_ShortcutRColonString:
	.incbin "baserom.gba", 0x000a6164, 0x00000004
	.global Menu_LvString
Menu_LvString:
	.incbin "baserom.gba", 0x000a6168, 0x00000004
	.global Menu_SlashString
Menu_SlashString:
	.incbin "baserom.gba", 0x000a616c, 0x00000004
	.global Menu_HpString
Menu_HpString:
	.incbin "baserom.gba", 0x000a6170, 0x00000004
	.global Menu_PpString
Menu_PpString:
	.incbin "baserom.gba", 0x000a6174, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000a6178, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000a61a8, 0x00000020
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.incbin "baserom.gba", 0x000a61c8, 0x00000004
	.global DjinnMenu_TextSlash
DjinnMenu_TextSlash:
	.incbin "baserom.gba", 0x000a61cc, 0x00000004
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
	.global FourObjectMotion_ResourceIds
	.global RomBytes_080af304
FourObjectMotion_ResourceIds:
RomBytes_080af304:
	.incbin "baserom.gba", 0x000a6228, 0x0000000d
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
	.incbin "baserom.gba", 0x000aaea4, 0x00000100
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "baserom.gba", 0x000aafa4, 0x00000180
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
	.incbin "baserom.gba", 0x000abada, 0x0000000c
	.section .rom.000ac204, "ax"
	.incbin "baserom.gba", 0x000ac204, 0x00000008
	.section .rom.000ac218, "ax"
	.incbin "baserom.gba", 0x000ac218, 0x00000040
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
	.global HitTbl
HitTbl:
	.incbin "baserom.gba", 0x000b9ab8, 0x00000008
	.global EpLossTbl
EpLossTbl:
	.incbin "baserom.gba", 0x000b9ac0, 0x00000018
	.global HpRcvTbl
HpRcvTbl:
	.incbin "baserom.gba", 0x000b9ad8, 0x00000018
	.global EpDmgTbl
EpDmgTbl:
	.incbin "baserom.gba", 0x000b9af0, 0x00000018
	.global HpDmgTbl5
HpDmgTbl5:
	.incbin "baserom.gba", 0x000b9b08, 0x00000018
	.global HpDmgTbl8
HpDmgTbl8:
	.incbin "baserom.gba", 0x000b9b20, 0x00000018
	.global HpDmgTbl6
HpDmgTbl6:
	.incbin "baserom.gba", 0x000b9b38, 0x00000018
	.global EpRcvTbl
EpRcvTbl:
	.incbin "baserom.gba", 0x000b9b50, 0x00000018
	.global HpDmgTbl
HpDmgTbl:
	.incbin "baserom.gba", 0x000b9b68, 0x00000018
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.incbin "baserom.gba", 0x000b9b80, 0x00000018
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "baserom.gba", 0x000b9b98, 0x00000208
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "baserom.gba", 0x000b9da0, 0x0000081c
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
	.global Debug_PartyPresets
Debug_PartyPresets:
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
	.incbin "baserom.gba", 0x000bca30, 0x00000100
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "baserom.gba", 0x000bcb30, 0x000000e0
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
	.incbin "baserom.gba", 0x000be420, 0x0000055b
	.section .rom.000d9972, "ax"
	.incbin "baserom.gba", 0x000d9972, 0x00000002
	.section .rom.000e4a78, "ax"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000e4a78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000e4a80, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000e4a88, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000e4a98, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000e4ab0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000e4ab8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000e4ac0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000e4ac8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000e4ad0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000e4ad8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000e4ae0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000e4ae8, 0x00000008
	.section .rom.000e7a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000e7a5c, 0x00000909
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000e8365, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000e83c8, 0x00000540
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000e8908, 0x000002f3
	.section .rom.000ea8bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000ea8bc, 0x000000ef
	.section .rom.000ef61c, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000ef61c, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000ef630, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000ef636, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000ef63e, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000ef64c, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000ef65a, 0x00000007
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
