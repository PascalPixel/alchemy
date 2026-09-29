@ Unidentified ROM data, read from your own ROM at build time as early pret
@ projects read their base ROM. Each section shrinks as its data gains source.
	.section .unidentified.08000000,"a"
	.global Rom_Start
Rom_Start:
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .unidentified.08007320,"a"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x00007320, 0x00000038
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
	.section .unidentified.08007c64,"a"
	.incbin "baserom.gba", 0x00007c64, 0x0000139c
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
	.global Data_0801307c
Data_0801307c:
	.incbin "baserom.gba", 0x0001307c, 0x00000010
	.global Data_0801308c
Data_0801308c:
	.incbin "baserom.gba", 0x0001308c, 0x00000008
	.global Data_08013094
Data_08013094:
	.incbin "baserom.gba", 0x00013094, 0x00000008
	.global Data_0801309c
Data_0801309c:
	.incbin "baserom.gba", 0x0001309c, 0x00000010
	.global Data_080130ac
Data_080130ac:
	.incbin "baserom.gba", 0x000130ac, 0x00000010
	.global Data_080130bc
Data_080130bc:
	.incbin "baserom.gba", 0x000130bc, 0x00000008
	.global Data_080130c4
Data_080130c4:
	.incbin "baserom.gba", 0x000130c4, 0x00000008
	.global Data_080130cc
Data_080130cc:
	.incbin "baserom.gba", 0x000130cc, 0x00000040
	.global Data_0801310c
Data_0801310c:
	.incbin "baserom.gba", 0x0001310c, 0x00000040
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001314c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00013190, 0x00000030
	.global Data_080131c0
Data_080131c0:
	.incbin "baserom.gba", 0x000131c0, 0x00000080
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
	.incbin "baserom.gba", 0x000133fc, 0x00000100
	.global Func_080134fc
Func_080134fc:
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
	.global Data_08013624
Data_08013624:
	.incbin "baserom.gba", 0x00013624, 0x000000bc
	.global Script_OperandHandlerTable
Script_OperandHandlerTable:
	.incbin "baserom.gba", 0x000136e0, 0x000000a4
	.global Data_08013784
Data_08013784:
	.incbin "baserom.gba", 0x00013784, 0x0000187c
	.section .unidentified.08029910,"a"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029910, 0x00000100
	.global RomBytes_08029a10
RomBytes_08029a10:
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.incbin "baserom.gba", 0x00029a10, 0x000000bc
	.global Data_08029acc
Data_08029acc:
	.incbin "baserom.gba", 0x00029acc, 0x0000009c
	.global Data_08029b68
Data_08029b68:
	.incbin "baserom.gba", 0x00029b68, 0x00000298
	.global UiIcon_OverlayPointerTable
UiIcon_OverlayPointerTable:
	.incbin "baserom.gba", 0x00029e00, 0x000000e4
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00029ee4, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x0002a2e0, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002de88, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002e108, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x000308a0, 0x00000804
	.global Data_080310a4
Data_080310a4:
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.global RomBytes_080310a4
RomBytes_080310a4:
	.incbin "baserom.gba", 0x000310a4, 0x00000300
	.global Data_080313a4
Data_080313a4:
	.incbin "baserom.gba", 0x000313a4, 0x00000080
	.global Data_08031424
Data_08031424:
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
	.global Data_08033e60
Data_08033e60:
	.incbin "baserom.gba", 0x00033e60, 0x00000050
	.global Data_08033eb0
Data_08033eb0:
	.incbin "baserom.gba", 0x00033eb0, 0x00000038
	.global Data_08033ee8
Data_08033ee8:
	.incbin "baserom.gba", 0x00033ee8, 0x00000010
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "baserom.gba", 0x00033ef8, 0x00000400
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000342f8, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x000346f8, 0x00002000
	.global Data_080366f8
Data_080366f8:
	.incbin "baserom.gba", 0x000366f8, 0x00000048
	.global Data_08036740
Data_08036740:
	.incbin "baserom.gba", 0x00036740, 0x00000010
	.global PaletteGlow_WaveTable
PaletteGlow_WaveTable:
	.incbin "baserom.gba", 0x00036750, 0x00000079
	.global Data_080367c9
Data_080367c9:
	.incbin "baserom.gba", 0x000367c9, 0x00000003
	.global Data_080367cc
Data_080367cc:
	.incbin "baserom.gba", 0x000367cc, 0x00000002
	.global Data_080367ce
Data_080367ce:
	.incbin "baserom.gba", 0x000367ce, 0x00000002
	.global Data_080367d0
Data_080367d0:
	.incbin "baserom.gba", 0x000367d0, 0x00000006
	.global Data_080367d6
Data_080367d6:
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
	.global Data_080368d4
Data_080368d4:
	.incbin "baserom.gba", 0x000368d4, 0x000008f0
	.global Data_080371c4
Data_080371c4:
	.incbin "baserom.gba", 0x000371c4, 0x0000001c
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000371e0, 0x00000016
	.global Data_080371f6
Data_080371f6:
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
	.global Data_08037308
Data_08037308:
	.incbin "baserom.gba", 0x00037308, 0x00000020
	.global Data_08037328
Data_08037328:
	.incbin "baserom.gba", 0x00037328, 0x00000080
	.global Data_080373a8
Data_080373a8:
	.incbin "baserom.gba", 0x000373a8, 0x00000010
	.global Data_080373b8
Data_080373b8:
	.incbin "baserom.gba", 0x000373b8, 0x00000020
	.global Data_080373d8
Data_080373d8:
	.incbin "baserom.gba", 0x000373d8, 0x00000004
	.global Data_080373dc
Data_080373dc:
	.incbin "baserom.gba", 0x000373dc, 0x00000004
	.global Data_080373e0
Data_080373e0:
	.incbin "baserom.gba", 0x000373e0, 0x00000004
	.global Data_080373e4
Data_080373e4:
	.incbin "baserom.gba", 0x000373e4, 0x00000002
	.global Data_080373e7
Data_080373e7:
	.incbin "baserom.gba", 0x000373e6, 0x00000004
	.global Data_080373eb
Data_080373eb:
	.incbin "baserom.gba", 0x000373ea, 0x00000005
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
	.global Data_08037440
Data_08037440:
	.incbin "baserom.gba", 0x00037440, 0x00000008
	.global Data_08037448
Data_08037448:
	.incbin "baserom.gba", 0x00037448, 0x00000008
	.global Data_08037450
Data_08037450:
	.incbin "baserom.gba", 0x00037450, 0x00000008
	.global Data_08037458
Data_08037458:
	.incbin "baserom.gba", 0x00037458, 0x00000008
	.global Data_08037460
Data_08037460:
	.incbin "baserom.gba", 0x00037460, 0x00000004
	.section .unidentified.08073808,"a"
	.incbin "baserom.gba", 0x00073808, 0x0000000a
	.global WorkspaceOptions_SliderPalette
WorkspaceOptions_SliderPalette:
	.incbin "baserom.gba", 0x00073812, 0x00000042
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x00073854, 0x00000114
	.global Data_08073968
Data_08073968:
	.incbin "baserom.gba", 0x00073968, 0x00003698
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
	.global Data_08084b1c
Data_08084b1c:
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
	.global Data_08089258
Data_08089258:
	.incbin "baserom.gba", 0x00089258, 0x00000014
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008926c, 0x00000d94
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
	.incbin "baserom.gba", 0x0009d9f0, 0x000007e8
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
	.incbin "baserom.gba", 0x0009e4ce, 0x000001b8
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
	.global Data_0809e8ac
Data_0809e8ac:
	.incbin "baserom.gba", 0x0009e8ac, 0x00000022
	.global Data_0809e8ce
Data_0809e8ce:
	.incbin "baserom.gba", 0x0009e8ce, 0x00000060
	.global Data_0809e92e
Data_0809e92e:
	.incbin "baserom.gba", 0x0009e92e, 0x00000040
	.global Data_0809e96e
Data_0809e96e:
	.incbin "baserom.gba", 0x0009e96e, 0x00000040
	.global Data_0809e9ae
Data_0809e9ae:
	.incbin "baserom.gba", 0x0009e9ae, 0x00000042
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x0009e9f0, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x0009ebfc, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x0009ed80, 0x00000204
	.global Data_0809ef84
Data_0809ef84:
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
	.global Data_0809f0bc
Data_0809f0bc:
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
	.global Data_0809f840
Data_0809f840:
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
	.incbin "baserom.gba", 0x0009ff58, 0x00000160
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
	.incbin "baserom.gba", 0x000a0138, 0x00000ec8
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
	.global Data_080af20c
Data_080af20c:
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
	.global Data_080af21c
Data_080af21c:
	.incbin "baserom.gba", 0x000af21c, 0x00000004
	.global Data_080af220
Data_080af220:
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
	.global Data_080af230
Data_080af230:
	.incbin "baserom.gba", 0x000af230, 0x00000004
	.global Data_080af234
Data_080af234:
	.incbin "baserom.gba", 0x000af234, 0x00000004
	.global Data_080af238
Data_080af238:
	.incbin "baserom.gba", 0x000af238, 0x00000004
	.global Data_080af23c
Data_080af23c:
	.incbin "baserom.gba", 0x000af23c, 0x00000030
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "baserom.gba", 0x000af26c, 0x00000020
	.global Data_080af28c
Data_080af28c:
	.incbin "baserom.gba", 0x000af28c, 0x00000008
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.incbin "baserom.gba", 0x000af294, 0x00000009
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.incbin "baserom.gba", 0x000af29d, 0x00000009
	.global Data_080af2a6
Data_080af2a6:
	.incbin "baserom.gba", 0x000af2a6, 0x0000000b
	.global Data_080af2b1
Data_080af2b1:
	.incbin "baserom.gba", 0x000af2b1, 0x0000000b
	.global Data_080af2bc
Data_080af2bc:
	.incbin "baserom.gba", 0x000af2bc, 0x00000014
	.global Data_080af2d0
Data_080af2d0:
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
	.incbin "baserom.gba", 0x000af304, 0x00000cfc
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
	.incbin "baserom.gba", 0x000b4ab6, 0x0000054a
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
	.global HitFalloff
HitFalloff:
	.incbin "baserom.gba", 0x000c2ab8, 0x00000008
	.global PpLossFalloff
PpLossFalloff:
	.incbin "baserom.gba", 0x000c2ac0, 0x00000018
	.global HpHealFalloff
HpHealFalloff:
	.incbin "baserom.gba", 0x000c2ad8, 0x00000018
	.global PpDmgFalloff
PpDmgFalloff:
	.incbin "baserom.gba", 0x000c2af0, 0x00000018
	.global HpDmgFalloff5
HpDmgFalloff5:
	.incbin "baserom.gba", 0x000c2b08, 0x00000018
	.global HpDmgFalloff8
HpDmgFalloff8:
	.incbin "baserom.gba", 0x000c2b20, 0x00000018
	.global HpDmgFalloff6
HpDmgFalloff6:
	.incbin "baserom.gba", 0x000c2b38, 0x00000018
	.global PpHealFalloff
PpHealFalloff:
	.incbin "baserom.gba", 0x000c2b50, 0x00000018
	.global HpDmgFalloff
HpDmgFalloff:
	.incbin "baserom.gba", 0x000c2b68, 0x00000030
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
	.incbin "baserom.gba", 0x000c3628, 0x0000090c
	.global Data_080c3f34
Data_080c3f34:
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
	.global Data_080c5b30
Data_080c5b30:
	.incbin "baserom.gba", 0x000c5b30, 0x000000e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000c5c10, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.global Data_080c5c38
Data_080c5c38:
	.incbin "baserom.gba", 0x000c5c38, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000c73f8, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000c7420, 0x00001be0
	.section .unidentified.080eda78,"a"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000eda78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000eda80, 0x00000008
	.global Data_080eda88
Data_080eda88:
	.incbin "baserom.gba", 0x000eda88, 0x00000028
	.global Data_080edab0
Data_080edab0:
	.incbin "baserom.gba", 0x000edab0, 0x00000008
	.global Data_080edab8
Data_080edab8:
	.incbin "baserom.gba", 0x000edab8, 0x00000008
	.global Data_080edac0
Data_080edac0:
	.incbin "baserom.gba", 0x000edac0, 0x00000008
	.global Data_080edac8
Data_080edac8:
	.incbin "baserom.gba", 0x000edac8, 0x00000020
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000edae8, 0x00000008
	.section .unidentified.080ede48,"a"
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
	.global Data_080edebe
Data_080edebe:
	.incbin "baserom.gba", 0x000edebe, 0x0000000c
	.global Data_080edeca
Data_080edeca:
	.incbin "baserom.gba", 0x000edeca, 0x00000006
	.global Data_080eded0
Data_080eded0:
	.incbin "baserom.gba", 0x000eded0, 0x00000006
	.global Data_080eded6
Data_080eded6:
	.incbin "baserom.gba", 0x000eded6, 0x00000006
	.global Data_080ededc
Data_080ededc:
	.incbin "baserom.gba", 0x000ededc, 0x00000028
	.global Data_080edf04
Data_080edf04:
	.incbin "baserom.gba", 0x000edf04, 0x00000054
	.global Data_080edf58
Data_080edf58:
	.incbin "baserom.gba", 0x000edf58, 0x00000006
	.global Data_080edf5e
Data_080edf5e:
	.incbin "baserom.gba", 0x000edf5e, 0x00000006
	.global Data_080edf64
Data_080edf64:
	.incbin "baserom.gba", 0x000edf64, 0x0000000c
	.global Data_080edf70
Data_080edf70:
	.incbin "baserom.gba", 0x000edf70, 0x00000006
	.global Data_080edf76
Data_080edf76:
	.incbin "baserom.gba", 0x000edf76, 0x00000004
	.global Data_080edf7b
Data_080edf7b:
	.incbin "baserom.gba", 0x000edf7a, 0x00000004
	.global Data_080edf7f
Data_080edf7f:
	.incbin "baserom.gba", 0x000edf7e, 0x00000004
	.global Data_080edf83
Data_080edf83:
	.incbin "baserom.gba", 0x000edf82, 0x00000006
	.global Data_080edf88
Data_080edf88:
	.incbin "baserom.gba", 0x000edf88, 0x000000d0
	.global Data_080ee058
Data_080ee058:
	.incbin "baserom.gba", 0x000ee058, 0x00000004
	.global Data_080ee05c
Data_080ee05c:
	.incbin "baserom.gba", 0x000ee05c, 0x00000004
	.global Data_080ee060
Data_080ee060:
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
	.global Data_080ee090
Data_080ee090:
	.incbin "baserom.gba", 0x000ee090, 0x00000012
	.global Data_080ee0a2
Data_080ee0a2:
	.incbin "baserom.gba", 0x000ee0a2, 0x00000008
	.global Data_080ee0aa
Data_080ee0aa:
	.incbin "baserom.gba", 0x000ee0aa, 0x00000006
	.global Data_080ee0b0
Data_080ee0b0:
	.incbin "baserom.gba", 0x000ee0b0, 0x00000002
	.global Data_080ee0b3
Data_080ee0b3:
	.incbin "baserom.gba", 0x000ee0b2, 0x00000004
	.global Data_080ee0b6
Data_080ee0b6:
	.incbin "baserom.gba", 0x000ee0b6, 0x0000000e
	.global Data_080ee0c4
Data_080ee0c4:
	.incbin "baserom.gba", 0x000ee0c4, 0x00000012
	.global Data_080ee0d6
Data_080ee0d6:
	.incbin "baserom.gba", 0x000ee0d6, 0x00000012
	.global Data_080ee0e8
Data_080ee0e8:
	.incbin "baserom.gba", 0x000ee0e8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000ee10c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000ee11a, 0x0000000e
	.global Data_080ee128
Data_080ee128:
	.incbin "baserom.gba", 0x000ee128, 0x00000030
	.global Data_080ee158
Data_080ee158:
	.incbin "baserom.gba", 0x000ee158, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000ee15a, 0x00000008
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000ee162, 0x0000000a
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000ee16c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000ee174, 0x00000002
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000ee176, 0x00000004
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000ee17a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000ee17e, 0x0000002e
	.global Data_080ee1ac
Data_080ee1ac:
	.incbin "baserom.gba", 0x000ee1ac, 0x00000008
	.global Data_080ee1b4
Data_080ee1b4:
	.incbin "baserom.gba", 0x000ee1b4, 0x0000001e
	.global Data_080ee1d3
Data_080ee1d3:
	.incbin "baserom.gba", 0x000ee1d2, 0x00000022
	.global Data_080ee1f5
Data_080ee1f5:
	.incbin "baserom.gba", 0x000ee1f4, 0x00000006
	.global Data_080ee1fb
Data_080ee1fb:
	.incbin "baserom.gba", 0x000ee1fa, 0x0000000c
	.global Data_080ee207
Data_080ee207:
	.incbin "baserom.gba", 0x000ee206, 0x0000000e
	.global Data_080ee214
Data_080ee214:
	.incbin "baserom.gba", 0x000ee214, 0x00000030
	.global Data_080ee244
Data_080ee244:
	.incbin "baserom.gba", 0x000ee244, 0x0000000c
	.global Data_080ee250
Data_080ee250:
	.incbin "baserom.gba", 0x000ee250, 0x0000000e
	.global Data_080ee25e
Data_080ee25e:
	.incbin "baserom.gba", 0x000ee25e, 0x00000004
	.global Data_080ee262
Data_080ee262:
	.incbin "baserom.gba", 0x000ee262, 0x00000032
	.global Data_080ee294
Data_080ee294:
	.incbin "baserom.gba", 0x000ee294, 0x00000006
	.global Data_080ee29a
Data_080ee29a:
	.incbin "baserom.gba", 0x000ee29a, 0x00000002
	.global Data_080ee29d
Data_080ee29d:
	.incbin "baserom.gba", 0x000ee29c, 0x0000000c
	.global Data_080ee2a9
Data_080ee2a9:
	.incbin "baserom.gba", 0x000ee2a8, 0x00000006
	.global Data_080ee2ae
Data_080ee2ae:
	.incbin "baserom.gba", 0x000ee2ae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000ee2b4, 0x0000065c
	.global Data_080ee910
Data_080ee910:
	.incbin "baserom.gba", 0x000ee910, 0x00000006
	.global Data_080ee916
Data_080ee916:
	.incbin "baserom.gba", 0x000ee916, 0x0000000a
	.global Data_080ee920
Data_080ee920:
	.incbin "baserom.gba", 0x000ee920, 0x00000004
	.global Data_080ee925
Data_080ee925:
	.incbin "baserom.gba", 0x000ee924, 0x00000006
	.global Data_080ee92a
Data_080ee92a:
	.incbin "baserom.gba", 0x000ee92a, 0x00000006
	.global Data_080ee930
Data_080ee930:
	.incbin "baserom.gba", 0x000ee930, 0x00000004
	.global Data_080ee934
Data_080ee934:
	.incbin "baserom.gba", 0x000ee934, 0x0000000a
	.global Data_080ee93e
Data_080ee93e:
	.incbin "baserom.gba", 0x000ee93e, 0x00000004
	.global Data_080ee943
Data_080ee943:
	.incbin "baserom.gba", 0x000ee942, 0x00000006
	.global Data_080ee948
Data_080ee948:
	.incbin "baserom.gba", 0x000ee948, 0x0000000a
	.global Data_080ee952
Data_080ee952:
	.incbin "baserom.gba", 0x000ee952, 0x00000006
	.global Data_080ee958
Data_080ee958:
	.incbin "baserom.gba", 0x000ee958, 0x0000000e
	.global Data_080ee966
Data_080ee966:
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
	.global Data_080ee9f8
Data_080ee9f8:
	.incbin "baserom.gba", 0x000ee9f8, 0x00000010
	.global Data_080eea08
Data_080eea08:
	.incbin "baserom.gba", 0x000eea08, 0x00000018
	.global Data_080eea20
Data_080eea20:
	.incbin "baserom.gba", 0x000eea20, 0x0000000c
	.global Data_080eea2c
Data_080eea2c:
	.incbin "baserom.gba", 0x000eea2c, 0x0000000c
	.global Data_080eea38
Data_080eea38:
	.incbin "baserom.gba", 0x000eea38, 0x00000008
	.global Data_080eea41
Data_080eea41:
	.incbin "baserom.gba", 0x000eea40, 0x00000004
	.global Data_080eea44
Data_080eea44:
	.incbin "baserom.gba", 0x000eea44, 0x00000006
	.global Data_080eea4a
Data_080eea4a:
	.incbin "baserom.gba", 0x000eea4a, 0x00000006
	.global Data_080eea50
Data_080eea50:
	.incbin "baserom.gba", 0x000eea50, 0x00000006
	.global Data_080eea56
Data_080eea56:
	.incbin "baserom.gba", 0x000eea56, 0x0000000c
	.global Data_080eea62
Data_080eea62:
	.incbin "baserom.gba", 0x000eea62, 0x00000026
	.global Data_080eea88
Data_080eea88:
	.incbin "baserom.gba", 0x000eea88, 0x00000008
	.global Data_080eea91
Data_080eea91:
	.incbin "baserom.gba", 0x000eea90, 0x00000008
	.global Data_080eea99
Data_080eea99:
	.incbin "baserom.gba", 0x000eea98, 0x0000000a
	.global Data_080eeaa2
Data_080eeaa2:
	.incbin "baserom.gba", 0x000eeaa2, 0x00000010
	.global Data_080eeab2
Data_080eeab2:
	.incbin "baserom.gba", 0x000eeab2, 0x00000006
	.global Data_080eeab8
Data_080eeab8:
	.incbin "baserom.gba", 0x000eeab8, 0x00000002
	.global Data_080eeabb
Data_080eeabb:
	.incbin "baserom.gba", 0x000eeaba, 0x00000008
	.global Data_080eeac3
Data_080eeac3:
	.incbin "baserom.gba", 0x000eeac2, 0x0000000a
	.global Data_080eeacc
Data_080eeacc:
	.incbin "baserom.gba", 0x000eeacc, 0x00000010
	.global Data_080eeadc
Data_080eeadc:
	.incbin "baserom.gba", 0x000eeadc, 0x00000006
	.global Data_080eeae2
Data_080eeae2:
	.incbin "baserom.gba", 0x000eeae2, 0x0000000a
	.global Data_080eeaec
Data_080eeaec:
	.incbin "baserom.gba", 0x000eeaec, 0x0000000e
	.global Data_080eeafa
Data_080eeafa:
	.incbin "baserom.gba", 0x000eeafa, 0x0000004e
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
	.global Data_080eebd6
Data_080eebd6:
	.incbin "baserom.gba", 0x000eebd6, 0x0000000c
	.global Data_080eebe2
Data_080eebe2:
	.incbin "baserom.gba", 0x000eebe2, 0x00000004
	.global Data_080eebe6
Data_080eebe6:
	.incbin "baserom.gba", 0x000eebe6, 0x00000002
	.global Data_080eebe9
Data_080eebe9:
	.incbin "baserom.gba", 0x000eebe8, 0x00000004
	.global Data_080eebec
Data_080eebec:
	.incbin "baserom.gba", 0x000eebec, 0x0000003c
	.global Data_080eec28
Data_080eec28:
	.incbin "baserom.gba", 0x000eec28, 0x00000006
	.global Data_080eec2f
Data_080eec2f:
	.incbin "baserom.gba", 0x000eec2e, 0x00000008
	.global Data_080eec36
Data_080eec36:
	.incbin "baserom.gba", 0x000eec36, 0x00000006
	.global Data_080eec3d
Data_080eec3d:
	.incbin "baserom.gba", 0x000eec3c, 0x00000008
	.global Data_080eec44
Data_080eec44:
	.incbin "baserom.gba", 0x000eec44, 0x0000000e
	.global Data_080eec52
Data_080eec52:
	.incbin "baserom.gba", 0x000eec52, 0x00000008
	.global Data_080eec5a
Data_080eec5a:
	.incbin "baserom.gba", 0x000eec5a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000eec5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000eec63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eec68, 0x000000d6
	.global Data_080eed3e
Data_080eed3e:
	.incbin "baserom.gba", 0x000eed3e, 0x00000040
	.global Data_080eed7e
Data_080eed7e:
	.incbin "baserom.gba", 0x000eed7e, 0x00000012
	.global Data_080eed90
Data_080eed90:
	.incbin "baserom.gba", 0x000eed90, 0x0000000a
	.global Data_080eed9a
Data_080eed9a:
	.incbin "baserom.gba", 0x000eed9a, 0x00000006
	.global Data_080eeda0
Data_080eeda0:
	.incbin "baserom.gba", 0x000eeda0, 0x00000002
	.global Data_080eeda3
Data_080eeda3:
	.incbin "baserom.gba", 0x000eeda2, 0x00000004
	.global Data_080eeda6
Data_080eeda6:
	.incbin "baserom.gba", 0x000eeda6, 0x00000006
	.global Data_080eedac
Data_080eedac:
	.incbin "baserom.gba", 0x000eedac, 0x00000006
	.global Data_080eedb2
Data_080eedb2:
	.incbin "baserom.gba", 0x000eedb2, 0x00000006
	.global Data_080eedb8
Data_080eedb8:
	.incbin "baserom.gba", 0x000eedb8, 0x00000006
	.global Data_080eedbe
Data_080eedbe:
	.incbin "baserom.gba", 0x000eedbe, 0x0000000c
	.global Data_080eedca
Data_080eedca:
	.incbin "baserom.gba", 0x000eedca, 0x0000002a
	.global Data_080eedf4
Data_080eedf4:
	.incbin "baserom.gba", 0x000eedf4, 0x00000006
	.global Data_080eedfb
Data_080eedfb:
	.incbin "baserom.gba", 0x000eedfa, 0x00000008
	.global Data_080eee02
Data_080eee02:
	.incbin "baserom.gba", 0x000eee02, 0x0000000e
	.global Data_080eee10
Data_080eee10:
	.incbin "baserom.gba", 0x000eee10, 0x00000006
	.global Data_080eee17
Data_080eee17:
	.incbin "baserom.gba", 0x000eee16, 0x00000008
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
	.incbin "baserom.gba", 0x000eee4e, 0x0000008a
	.global Data_080eeed8
Data_080eeed8:
	.incbin "baserom.gba", 0x000eeed8, 0x00000008
	.global Data_080eeee1
Data_080eeee1:
	.incbin "baserom.gba", 0x000eeee0, 0x0000000a
	.global Data_080eeeea
Data_080eeeea:
	.incbin "baserom.gba", 0x000eeeea, 0x0000000e
	.global Data_080eeef8
Data_080eeef8:
	.incbin "baserom.gba", 0x000eeef8, 0x0000001a
	.global Data_080eef12
Data_080eef12:
	.incbin "baserom.gba", 0x000eef12, 0x00000006
	.global Data_080eef18
Data_080eef18:
	.incbin "baserom.gba", 0x000eef18, 0x00000050
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
	.section .unidentified.080ef014,"a"
	.incbin "baserom.gba", 0x000ef014, 0x00000020
	.global Data_080ef034
Data_080ef034:
	.incbin "baserom.gba", 0x000ef034, 0x00000fcc
	.section .unidentified.080f0a5c,"a"
	.incbin "baserom.gba", 0x000f0a5c, 0x00000760
	.global Data_080f11bd
Data_080f11bd:
	.incbin "baserom.gba", 0x000f11bc, 0x00000064
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f1220, 0x00000550
	.global Data_080f1770
Data_080f1770:
	.incbin "baserom.gba", 0x000f1770, 0x00000890
	.section .unidentified.080f2b6c,"ax"
	.global Func_080f2b6c
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f2b6c, 0x00000004
	.section .unidentified.080f38bc,"a"
	.incbin "baserom.gba", 0x000f38bc, 0x000000ee
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f39aa, 0x00000006
	.global Data_080f39b1
Data_080f39b1:
	.incbin "baserom.gba", 0x000f39b0, 0x0000003e
	.global Data_080f39ee
Data_080f39ee:
	.incbin "baserom.gba", 0x000f39ee, 0x00000040
	.global Data_080f3a2e
Data_080f3a2e:
	.incbin "baserom.gba", 0x000f3a2e, 0x00000040
	.global Data_080f3a6e
Data_080f3a6e:
	.incbin "baserom.gba", 0x000f3a6e, 0x00000592
	.section .unidentified.080f53dc,"a"
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
	.incbin "baserom.gba", 0x000f541a, 0x00000be6
	.section .unidentified.080f86f8,"a"
	.global Data_080f86f8
Data_080f86f8:
	.incbin "baserom.gba", 0x000f86f8, 0x00000014
	.global Data_080f870c
Data_080f870c:
	.incbin "baserom.gba", 0x000f870c, 0x00000006
	.global Data_080f8712
Data_080f8712:
	.incbin "baserom.gba", 0x000f8712, 0x00000008
	.global Data_080f871a
Data_080f871a:
	.incbin "baserom.gba", 0x000f871a, 0x0000000e
	.global Data_080f8728
Data_080f8728:
	.incbin "baserom.gba", 0x000f8728, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f8736, 0x000008ca
	.section .unidentified.080fb7a0,"a"
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
	.section .unidentified.080fc504,"a"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .unidentified.080fc624,"a"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fc624, 0x00000060
	.section .unidentified.08184698,"a"
	.incbin "baserom.gba", 0x00184698, 0x00000968
	.section .unidentified.0831efd8,"a"
	.incbin "baserom.gba", 0x0031efd8, 0x00001028
	.section .unidentified.083203c8,"a"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320fa0, 0x00000010
	.section .unidentified.083249e7,"a"
	.incbin "baserom.gba", 0x003249e7, 0x00000001
	.section .unidentified.0832b0a6,"a"
	.incbin "baserom.gba", 0x0032b0a6, 0x00000002
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b0a8, 0x000086f8
	.section .unidentified.083357f1,"a"
	.incbin "baserom.gba", 0x003357f1, 0x00000003
	.section .unidentified.08337101,"a"
	.incbin "baserom.gba", 0x00337101, 0x00000003
	.section .unidentified.0833ac05,"a"
	.incbin "baserom.gba", 0x0033ac05, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033ac08, 0x000002b4
	.section .unidentified.0833f2fe,"a"
	.incbin "baserom.gba", 0x0033f2fe, 0x00000002
	.section .unidentified.0834fa6e,"a"
	.incbin "baserom.gba", 0x0034fa6e, 0x00000002
	.section .unidentified.0835305e,"a"
	.incbin "baserom.gba", 0x0035305e, 0x00000002
	.section .unidentified.0835eb02,"a"
	.incbin "baserom.gba", 0x0035eb02, 0x00000002
	.section .unidentified.0836f1b2,"a"
	.incbin "baserom.gba", 0x0036f1b2, 0x00000002
	.section .unidentified.08376c52,"a"
	.incbin "baserom.gba", 0x00376c52, 0x00000002
	.section .unidentified.0837a492,"a"
	.incbin "baserom.gba", 0x0037a492, 0x00000002
	.section .unidentified.083838c6,"a"
	.incbin "baserom.gba", 0x003838c6, 0x00000002
	.section .unidentified.0838ecee,"a"
	.incbin "baserom.gba", 0x0038ecee, 0x00000002
	.section .unidentified.08396baa,"a"
	.incbin "baserom.gba", 0x00396baa, 0x00000002
	.section .unidentified.0839b63a,"a"
	.incbin "baserom.gba", 0x0039b63a, 0x00000002
	.section .unidentified.0839f8ee,"a"
	.incbin "baserom.gba", 0x0039f8ee, 0x00000002
	.section .unidentified.083a326e,"a"
	.incbin "baserom.gba", 0x003a326e, 0x00000002
	.section .unidentified.083afaaa,"a"
	.incbin "baserom.gba", 0x003afaaa, 0x00000002
	.section .unidentified.083be3f2,"a"
	.incbin "baserom.gba", 0x003be3f2, 0x00000002
	.section .unidentified.083c3cf5,"a"
	.incbin "baserom.gba", 0x003c3cf5, 0x00000003
	.section .unidentified.083c5677,"a"
	.incbin "baserom.gba", 0x003c5677, 0x00000001
	.section .unidentified.083c8495,"a"
	.incbin "baserom.gba", 0x003c8495, 0x00000003
	.section .unidentified.083cb4b9,"a"
	.incbin "baserom.gba", 0x003cb4b9, 0x00000003
	.section .unidentified.083cbd01,"a"
	.incbin "baserom.gba", 0x003cbd01, 0x00000003
	.section .unidentified.083cd517,"a"
	.incbin "baserom.gba", 0x003cd517, 0x00000001
	.section .unidentified.083cdb02,"a"
	.incbin "baserom.gba", 0x003cdb02, 0x00000002
	.section .unidentified.083ce1c6,"a"
	.incbin "baserom.gba", 0x003ce1c6, 0x00000002
	.section .unidentified.083ce4ad,"a"
	.incbin "baserom.gba", 0x003ce4ad, 0x00000003
	.section .unidentified.083cf813,"a"
	.incbin "baserom.gba", 0x003cf813, 0x00000001
	.section .unidentified.083cfbfd,"a"
	.incbin "baserom.gba", 0x003cfbfd, 0x00000003
	.section .unidentified.083cffcf,"a"
	.incbin "baserom.gba", 0x003cffcf, 0x00000001
	.section .unidentified.083d086f,"a"
	.incbin "baserom.gba", 0x003d086f, 0x00000001
	.section .unidentified.083d0d12,"a"
	.incbin "baserom.gba", 0x003d0d12, 0x00000002
	.section .unidentified.083d1ecb,"a"
	.incbin "baserom.gba", 0x003d1ecb, 0x00000001
	.section .unidentified.083d338e,"a"
	.incbin "baserom.gba", 0x003d338e, 0x00000002
	.section .unidentified.083d4357,"a"
	.incbin "baserom.gba", 0x003d4357, 0x00000001
	.section .unidentified.083d45b2,"a"
	.incbin "baserom.gba", 0x003d45b2, 0x00000002
	.section .unidentified.083d600d,"a"
	.incbin "baserom.gba", 0x003d600d, 0x00000003
	.section .unidentified.083d6559,"a"
	.incbin "baserom.gba", 0x003d6559, 0x00000003
	.section .unidentified.083d82e2,"a"
	.incbin "baserom.gba", 0x003d82e2, 0x00000002
	.section .unidentified.083d855b,"a"
	.incbin "baserom.gba", 0x003d855b, 0x00000001
	.section .unidentified.083d8a22,"a"
	.incbin "baserom.gba", 0x003d8a22, 0x00000002
	.section .unidentified.083da5f7,"a"
	.incbin "baserom.gba", 0x003da5f7, 0x00000001
	.section .unidentified.083dc1f5,"a"
	.incbin "baserom.gba", 0x003dc1f5, 0x00000003
	.section .unidentified.083dc416,"a"
	.incbin "baserom.gba", 0x003dc416, 0x00000002
	.section .unidentified.083dc853,"a"
	.incbin "baserom.gba", 0x003dc853, 0x00000001
	.section .unidentified.083dc965,"a"
	.incbin "baserom.gba", 0x003dc965, 0x00000003
	.section .unidentified.083dd0e3,"a"
	.incbin "baserom.gba", 0x003dd0e3, 0x00000001
	.section .unidentified.083dd581,"a"
	.incbin "baserom.gba", 0x003dd581, 0x00000003
	.section .unidentified.083de296,"a"
	.incbin "baserom.gba", 0x003de296, 0x00000002
	.section .unidentified.083df016,"a"
	.incbin "baserom.gba", 0x003df016, 0x00000002
	.section .unidentified.083df3a7,"a"
	.incbin "baserom.gba", 0x003df3a7, 0x00000001
	.section .unidentified.083e10ee,"a"
	.incbin "baserom.gba", 0x003e10ee, 0x00000002
	.section .unidentified.083e1ec5,"a"
	.incbin "baserom.gba", 0x003e1ec5, 0x00000003
	.section .unidentified.083e26f2,"a"
	.incbin "baserom.gba", 0x003e26f2, 0x00000002
	.section .unidentified.083e2d7d,"a"
	.incbin "baserom.gba", 0x003e2d7d, 0x00000003
	.section .unidentified.083e2f3b,"a"
	.incbin "baserom.gba", 0x003e2f3b, 0x00000001
	.section .unidentified.083e3c93,"a"
	.incbin "baserom.gba", 0x003e3c93, 0x00000001
	.section .unidentified.083e41df,"a"
	.incbin "baserom.gba", 0x003e41df, 0x00000001
	.section .unidentified.083e45b9,"a"
	.incbin "baserom.gba", 0x003e45b9, 0x00000003
	.section .unidentified.083e4967,"a"
	.incbin "baserom.gba", 0x003e4967, 0x00000001
	.section .unidentified.083e690b,"a"
	.incbin "baserom.gba", 0x003e690b, 0x00000001
	.section .unidentified.083e76a7,"a"
	.incbin "baserom.gba", 0x003e76a7, 0x00000001
	.section .unidentified.083e78c3,"a"
	.incbin "baserom.gba", 0x003e78c3, 0x00000001
	.section .unidentified.083e7bbf,"a"
	.incbin "baserom.gba", 0x003e7bbf, 0x00000001
	.section .unidentified.083e9d6d,"a"
	.incbin "baserom.gba", 0x003e9d6d, 0x00000003
	.section .unidentified.083eab3b,"a"
	.incbin "baserom.gba", 0x003eab3b, 0x00000001
	.section .unidentified.083ebc9d,"a"
	.incbin "baserom.gba", 0x003ebc9d, 0x00000003
	.section .unidentified.083ec1f7,"a"
	.incbin "baserom.gba", 0x003ec1f7, 0x00000001
	.section .unidentified.083edae1,"a"
	.incbin "baserom.gba", 0x003edae1, 0x00000003
	.section .unidentified.083ee382,"a"
	.incbin "baserom.gba", 0x003ee382, 0x00000002
	.section .unidentified.083ee75f,"a"
	.incbin "baserom.gba", 0x003ee75f, 0x00000001
	.section .unidentified.083ee9e9,"a"
	.incbin "baserom.gba", 0x003ee9e9, 0x00000003
	.section .unidentified.083eed8f,"a"
	.incbin "baserom.gba", 0x003eed8f, 0x00000001
	.section .unidentified.083eefea,"a"
	.incbin "baserom.gba", 0x003eefea, 0x00000002
	.section .unidentified.083ef3a2,"a"
	.incbin "baserom.gba", 0x003ef3a2, 0x00000002
	.section .unidentified.083f088b,"a"
	.incbin "baserom.gba", 0x003f088b, 0x00000001
	.section .unidentified.083f1e4e,"a"
	.incbin "baserom.gba", 0x003f1e4e, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f1e50, 0x00000a6c
	.section .unidentified.083f3692,"a"
	.incbin "baserom.gba", 0x003f3692, 0x00000002
	.section .unidentified.083f3a15,"a"
	.incbin "baserom.gba", 0x003f3a15, 0x00000003
	.section .unidentified.083f46c5,"a"
	.incbin "baserom.gba", 0x003f46c5, 0x00000003
	.section .unidentified.083f520d,"a"
	.incbin "baserom.gba", 0x003f520d, 0x00000003
	.section .unidentified.083f53a6,"a"
	.incbin "baserom.gba", 0x003f53a6, 0x00000002
	.section .unidentified.083f5c33,"a"
	.incbin "baserom.gba", 0x003f5c33, 0x00000001
	.section .unidentified.083f66fa,"a"
	.incbin "baserom.gba", 0x003f66fa, 0x00000002
	.section .unidentified.083f6c12,"a"
	.incbin "baserom.gba", 0x003f6c12, 0x00000002
	.section .unidentified.083f7236,"a"
	.incbin "baserom.gba", 0x003f7236, 0x00000002
	.section .unidentified.083f7a9d,"a"
	.incbin "baserom.gba", 0x003f7a9d, 0x00000003
	.section .unidentified.083f7ec1,"a"
	.incbin "baserom.gba", 0x003f7ec1, 0x00000003
	.section .unidentified.083f815b,"a"
	.incbin "baserom.gba", 0x003f815b, 0x00000001
	.section .unidentified.083f9af7,"a"
	.incbin "baserom.gba", 0x003f9af7, 0x00000001
	.section .unidentified.083fb86e,"a"
	.incbin "baserom.gba", 0x003fb86e, 0x00000002
	.section .unidentified.083fd7e3,"a"
	.incbin "baserom.gba", 0x003fd7e3, 0x00000001
	.section .unidentified.083fdcb3,"a"
	.incbin "baserom.gba", 0x003fdcb3, 0x00000001
	.section .unidentified.083fe345,"a"
	.incbin "baserom.gba", 0x003fe345, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe348, 0x00000a34
	.section .unidentified.0840014d,"a"
	.incbin "baserom.gba", 0x0040014d, 0x00000003
	.section .unidentified.08400c1e,"a"
	.incbin "baserom.gba", 0x00400c1e, 0x00000002
	.section .unidentified.0840175b,"a"
	.incbin "baserom.gba", 0x0040175b, 0x00000001
	.section .unidentified.08401d9b,"a"
	.incbin "baserom.gba", 0x00401d9b, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00401d9c, 0x00001588
	.section .unidentified.08403385,"a"
	.incbin "baserom.gba", 0x00403385, 0x00000003
	.section .unidentified.084036f3,"a"
	.incbin "baserom.gba", 0x004036f3, 0x00000001
	.section .unidentified.08403c91,"a"
	.incbin "baserom.gba", 0x00403c91, 0x00000003
	.section .unidentified.08403fc5,"a"
	.incbin "baserom.gba", 0x00403fc5, 0x00000003
	.section .unidentified.08404301,"a"
	.incbin "baserom.gba", 0x00404301, 0x00000003
	.section .unidentified.0840531a,"a"
	.incbin "baserom.gba", 0x0040531a, 0x00000002
	.section .unidentified.08408926,"a"
	.incbin "baserom.gba", 0x00408926, 0x00000002
	.section .unidentified.084090c9,"a"
	.incbin "baserom.gba", 0x004090c9, 0x00000003
	.section .unidentified.0840a45b,"a"
	.incbin "baserom.gba", 0x0040a45b, 0x00000001
	.section .unidentified.0840a88b,"a"
	.incbin "baserom.gba", 0x0040a88b, 0x00000001
	.section .unidentified.0840c681,"a"
	.incbin "baserom.gba", 0x0040c681, 0x00000003
	.section .unidentified.0840cf96,"a"
	.incbin "baserom.gba", 0x0040cf96, 0x00000002
	.section .unidentified.0840eaca,"a"
	.incbin "baserom.gba", 0x0040eaca, 0x00000002
	.section .unidentified.0840fb19,"a"
	.incbin "baserom.gba", 0x0040fb19, 0x00000003
	.section .unidentified.084101cf,"a"
	.incbin "baserom.gba", 0x004101cf, 0x00000001
	.section .unidentified.08411275,"a"
	.incbin "baserom.gba", 0x00411275, 0x00000003
	.section .unidentified.08424687,"a"
	.incbin "baserom.gba", 0x00424687, 0x00000001
	.section .unidentified.084247d9,"a"
	.incbin "baserom.gba", 0x004247d9, 0x00000003
	.section .unidentified.08424c6a,"a"
	.incbin "baserom.gba", 0x00424c6a, 0x00000002
	.section .unidentified.08424e61,"a"
	.incbin "baserom.gba", 0x00424e61, 0x00000003
	.section .unidentified.0842651e,"a"
	.incbin "baserom.gba", 0x0042651e, 0x00000002
	.section .unidentified.08427a3d,"a"
	.incbin "baserom.gba", 0x00427a3d, 0x00000003
	.section .unidentified.08428609,"a"
	.incbin "baserom.gba", 0x00428609, 0x00000003
	.section .unidentified.0842917e,"a"
	.incbin "baserom.gba", 0x0042917e, 0x00000002
	.section .unidentified.0842b027,"a"
	.incbin "baserom.gba", 0x0042b027, 0x00000001
	.section .unidentified.0842c7b5,"a"
	.incbin "baserom.gba", 0x0042c7b5, 0x00000003
	.section .unidentified.0842dc66,"a"
	.incbin "baserom.gba", 0x0042dc66, 0x00000002
	.section .unidentified.084303b3,"a"
	.incbin "baserom.gba", 0x004303b3, 0x00000001
	.section .unidentified.08431c4d,"a"
	.incbin "baserom.gba", 0x00431c4d, 0x00000003
	.section .unidentified.08433216,"a"
	.incbin "baserom.gba", 0x00433216, 0x00000002
	.section .unidentified.08434ca6,"a"
	.incbin "baserom.gba", 0x00434ca6, 0x00000002
	.section .unidentified.08435ef7,"a"
	.incbin "baserom.gba", 0x00435ef7, 0x00000001
	.section .unidentified.0843f866,"a"
	.incbin "baserom.gba", 0x0043f866, 0x00000002
	.section .unidentified.08441a1a,"a"
	.incbin "baserom.gba", 0x00441a1a, 0x00000002
	.section .unidentified.08445b81,"a"
	.incbin "baserom.gba", 0x00445b81, 0x00000003
	.section .unidentified.08448da6,"a"
	.incbin "baserom.gba", 0x00448da6, 0x00000002
	.section .unidentified.0844a4aa,"a"
	.incbin "baserom.gba", 0x0044a4aa, 0x00000002
	.section .unidentified.08451092,"a"
	.incbin "baserom.gba", 0x00451092, 0x00000002
	.section .unidentified.08459a16,"a"
	.incbin "baserom.gba", 0x00459a16, 0x00000002
	.section .unidentified.084614f2,"a"
	.incbin "baserom.gba", 0x004614f2, 0x00000002
	.section .unidentified.08464586,"a"
	.incbin "baserom.gba", 0x00464586, 0x00000002
	.section .unidentified.08467995,"a"
	.incbin "baserom.gba", 0x00467995, 0x00000003
	.section .unidentified.0846a201,"a"
	.incbin "baserom.gba", 0x0046a201, 0x00000003
	.section .unidentified.0846bcf2,"a"
	.incbin "baserom.gba", 0x0046bcf2, 0x00000002
	.section .unidentified.0846cb0a,"a"
	.incbin "baserom.gba", 0x0046cb0a, 0x00000002
	.section .unidentified.0846d7f5,"a"
	.incbin "baserom.gba", 0x0046d7f5, 0x00000003
	.section .unidentified.08476cb2,"a"
	.incbin "baserom.gba", 0x00476cb2, 0x00000002
	.section .unidentified.084779bd,"a"
	.incbin "baserom.gba", 0x004779bd, 0x00000003
	.section .unidentified.08479ad1,"a"
	.incbin "baserom.gba", 0x00479ad1, 0x00000003
	.section .unidentified.0847a743,"a"
	.incbin "baserom.gba", 0x0047a743, 0x00000001
	.section .unidentified.0847b35b,"a"
	.incbin "baserom.gba", 0x0047b35b, 0x00000001
	.section .unidentified.0847bacf,"a"
	.incbin "baserom.gba", 0x0047bacf, 0x00000001
	.section .unidentified.0847d35f,"a"
	.incbin "baserom.gba", 0x0047d35f, 0x00000001
	.section .unidentified.0847dd2d,"a"
	.incbin "baserom.gba", 0x0047dd2d, 0x00000003
	.section .unidentified.0847e94b,"a"
	.incbin "baserom.gba", 0x0047e94b, 0x00000001
	.section .unidentified.08480fff,"a"
	.incbin "baserom.gba", 0x00480fff, 0x00000001
	.section .unidentified.08483712,"a"
	.incbin "baserom.gba", 0x00483712, 0x00000002
	.section .unidentified.08488667,"a"
	.incbin "baserom.gba", 0x00488667, 0x00000001
	.section .unidentified.0848c991,"a"
	.incbin "baserom.gba", 0x0048c991, 0x00000003
	.section .unidentified.0848d57e,"a"
	.incbin "baserom.gba", 0x0048d57e, 0x00000002
	.section .unidentified.08492133,"a"
	.incbin "baserom.gba", 0x00492133, 0x00000001
	.section .unidentified.0849449d,"a"
	.incbin "baserom.gba", 0x0049449d, 0x00000003
	.section .unidentified.08495e3f,"a"
	.incbin "baserom.gba", 0x00495e3f, 0x00000001
	.section .unidentified.0849a701,"a"
	.incbin "baserom.gba", 0x0049a701, 0x00000003
	.section .unidentified.0849e7c9,"a"
	.incbin "baserom.gba", 0x0049e7c9, 0x00000003
	.section .unidentified.084a1e92,"a"
	.incbin "baserom.gba", 0x004a1e92, 0x00000002
	.section .unidentified.084a9e63,"a"
	.incbin "baserom.gba", 0x004a9e63, 0x00000001
	.section .unidentified.084b1173,"a"
	.incbin "baserom.gba", 0x004b1173, 0x00000001
	.section .unidentified.084b60f3,"a"
	.incbin "baserom.gba", 0x004b60f3, 0x00000001
	.section .unidentified.084baaa4,"a"
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004baaa4, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004baab0, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bac00, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bad40, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bae80, 0x00000140
	.section .unidentified.084c055d,"a"
	.incbin "baserom.gba", 0x004c055d, 0x00000003
	.section .unidentified.084c55bd,"a"
	.incbin "baserom.gba", 0x004c55bd, 0x00000003
	.section .unidentified.084c9b29,"a"
	.incbin "baserom.gba", 0x004c9b29, 0x00000003
	.section .unidentified.084cee87,"a"
	.incbin "baserom.gba", 0x004cee87, 0x00000001
	.section .unidentified.084d1adb,"a"
	.incbin "baserom.gba", 0x004d1adb, 0x00000001
	.section .unidentified.084d878f,"a"
	.incbin "baserom.gba", 0x004d878f, 0x00000001
	.section .unidentified.084dba4b,"a"
	.incbin "baserom.gba", 0x004dba4b, 0x00000001
	.section .unidentified.084de205,"a"
	.incbin "baserom.gba", 0x004de205, 0x00000003
	.section .unidentified.084e084e,"a"
	.incbin "baserom.gba", 0x004e084e, 0x00000002
	.section .unidentified.084e2f32,"a"
	.incbin "baserom.gba", 0x004e2f32, 0x00000002
	.section .unidentified.084e3fa7,"a"
	.incbin "baserom.gba", 0x004e3fa7, 0x00000001
	.section .unidentified.084e628d,"a"
	.incbin "baserom.gba", 0x004e628d, 0x00000003
	.section .unidentified.084e8acf,"a"
	.incbin "baserom.gba", 0x004e8acf, 0x00000001
	.section .unidentified.084ea8e5,"a"
	.incbin "baserom.gba", 0x004ea8e5, 0x00000003
	.section .unidentified.084ecfc6,"a"
	.incbin "baserom.gba", 0x004ecfc6, 0x00000002
	.section .unidentified.084ee10d,"a"
	.incbin "baserom.gba", 0x004ee10d, 0x00000003
	.section .unidentified.084f5389,"a"
	.incbin "baserom.gba", 0x004f5389, 0x00000003
	.section .unidentified.084f6986,"a"
	.incbin "baserom.gba", 0x004f6986, 0x00000002
	.section .unidentified.084f7c4f,"a"
	.incbin "baserom.gba", 0x004f7c4f, 0x00000001
	.section .unidentified.084fb1f5,"a"
	.incbin "baserom.gba", 0x004fb1f5, 0x00000003
	.section .unidentified.084fcfa6,"a"
	.incbin "baserom.gba", 0x004fcfa6, 0x00000002
	.section .unidentified.08500b1d,"a"
	.incbin "baserom.gba", 0x00500b1d, 0x00000003
	.section .unidentified.0850202b,"a"
	.incbin "baserom.gba", 0x0050202b, 0x00000001
	.section .unidentified.08502cfd,"a"
	.incbin "baserom.gba", 0x00502cfd, 0x00000003
	.section .unidentified.085040e2,"a"
	.incbin "baserom.gba", 0x005040e2, 0x00000002
	.section .unidentified.08505265,"a"
	.incbin "baserom.gba", 0x00505265, 0x00000003
	.section .unidentified.08505bee,"a"
	.incbin "baserom.gba", 0x00505bee, 0x00000002
	.section .unidentified.0850894e,"a"
	.incbin "baserom.gba", 0x0050894e, 0x00000002
	.section .unidentified.08509b39,"a"
	.incbin "baserom.gba", 0x00509b39, 0x00000003
	.section .unidentified.0850b7c1,"a"
	.incbin "baserom.gba", 0x0050b7c1, 0x00000003
	.section .unidentified.0850bccf,"a"
	.incbin "baserom.gba", 0x0050bccf, 0x00000001
	.section .unidentified.0850be0f,"a"
	.incbin "baserom.gba", 0x0050be0f, 0x00000001
	.section .unidentified.0850d185,"a"
	.incbin "baserom.gba", 0x0050d185, 0x00000003
	.section .unidentified.085115ab,"a"
	.incbin "baserom.gba", 0x005115ab, 0x00000001
	.section .unidentified.085132da,"a"
	.incbin "baserom.gba", 0x005132da, 0x00000002
	.section .unidentified.08514187,"a"
	.incbin "baserom.gba", 0x00514187, 0x00000001
	.section .unidentified.08516607,"a"
	.incbin "baserom.gba", 0x00516607, 0x00000001
	.section .unidentified.08517383,"a"
	.incbin "baserom.gba", 0x00517383, 0x00000001
	.section .unidentified.08517e62,"a"
	.incbin "baserom.gba", 0x00517e62, 0x00000002
	.section .unidentified.0851adae,"a"
	.incbin "baserom.gba", 0x0051adae, 0x00000002
	.section .unidentified.0851b6f5,"a"
	.incbin "baserom.gba", 0x0051b6f5, 0x00000003
	.section .unidentified.0851c342,"a"
	.incbin "baserom.gba", 0x0051c342, 0x00000002
	.section .unidentified.0851c569,"a"
	.incbin "baserom.gba", 0x0051c569, 0x00000003
	.section .unidentified.0851dfbf,"a"
	.incbin "baserom.gba", 0x0051dfbf, 0x00000001
	.section .unidentified.0851f985,"a"
	.incbin "baserom.gba", 0x0051f985, 0x00000003
	.section .unidentified.0851facf,"a"
	.incbin "baserom.gba", 0x0051facf, 0x00000001
	.section .unidentified.08521cc3,"a"
	.incbin "baserom.gba", 0x00521cc3, 0x00000001
	.section .unidentified.0852354b,"a"
	.incbin "baserom.gba", 0x0052354b, 0x00000001
	.section .unidentified.085257ed,"a"
	.incbin "baserom.gba", 0x005257ed, 0x00000003
	.section .unidentified.0852592f,"a"
	.incbin "baserom.gba", 0x0052592f, 0x00000001
	.section .unidentified.08527a42,"a"
	.incbin "baserom.gba", 0x00527a42, 0x00000002
	.section .unidentified.08529a26,"a"
	.incbin "baserom.gba", 0x00529a26, 0x00000002
	.section .unidentified.0852b14b,"a"
	.incbin "baserom.gba", 0x0052b14b, 0x00000001
	.section .unidentified.0852c1ff,"a"
	.incbin "baserom.gba", 0x0052c1ff, 0x00000001
	.section .unidentified.0852e041,"a"
	.incbin "baserom.gba", 0x0052e041, 0x00000003
	.section .unidentified.0853010f,"a"
	.incbin "baserom.gba", 0x0053010f, 0x00000001
	.section .unidentified.08532c61,"a"
	.incbin "baserom.gba", 0x00532c61, 0x00000003
	.section .unidentified.08535326,"a"
	.incbin "baserom.gba", 0x00535326, 0x00000002
	.section .unidentified.08537a7e,"a"
	.incbin "baserom.gba", 0x00537a7e, 0x00000002
	.section .unidentified.085392d1,"a"
	.incbin "baserom.gba", 0x005392d1, 0x00000003
	.section .unidentified.0853d79d,"a"
	.incbin "baserom.gba", 0x0053d79d, 0x00000003
	.section .unidentified.0853fce7,"a"
	.incbin "baserom.gba", 0x0053fce7, 0x00000001
	.section .unidentified.08540fb2,"a"
	.incbin "baserom.gba", 0x00540fb2, 0x00000002
	.section .unidentified.08541c63,"a"
	.incbin "baserom.gba", 0x00541c63, 0x00000001
	.section .unidentified.085427c3,"a"
	.incbin "baserom.gba", 0x005427c3, 0x00000001
	.section .unidentified.08544222,"a"
	.incbin "baserom.gba", 0x00544222, 0x00000002
	.section .unidentified.08544fc6,"a"
	.incbin "baserom.gba", 0x00544fc6, 0x00000002
	.section .unidentified.085499ed,"a"
	.incbin "baserom.gba", 0x005499ed, 0x00000003
	.section .unidentified.0855129f,"a"
	.incbin "baserom.gba", 0x0055129f, 0x00000001
	.section .unidentified.08556b4a,"a"
	.incbin "baserom.gba", 0x00556b4a, 0x00000002
	.section .unidentified.08556c8b,"a"
	.incbin "baserom.gba", 0x00556c8b, 0x00000001
	.section .unidentified.0855adf6,"a"
	.incbin "baserom.gba", 0x0055adf6, 0x00000002
	.section .unidentified.0855cef2,"a"
	.incbin "baserom.gba", 0x0055cef2, 0x00000002
	.section .unidentified.08564a96,"a"
	.incbin "baserom.gba", 0x00564a96, 0x00000002
	.section .unidentified.08569331,"a"
	.incbin "baserom.gba", 0x00569331, 0x00000003
	.section .unidentified.0856b832,"a"
	.incbin "baserom.gba", 0x0056b832, 0x00000002
	.section .unidentified.085733be,"a"
	.incbin "baserom.gba", 0x005733be, 0x00000002
	.section .unidentified.08575f6d,"a"
	.incbin "baserom.gba", 0x00575f6d, 0x00000003
	.section .unidentified.08578041,"a"
	.incbin "baserom.gba", 0x00578041, 0x00000003
	.section .unidentified.0857a09a,"a"
	.incbin "baserom.gba", 0x0057a09a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057a09c, 0x00001cec
	.section .unidentified.0858020d,"a"
	.incbin "baserom.gba", 0x0058020d, 0x00000003
	.section .unidentified.08581fdf,"a"
	.incbin "baserom.gba", 0x00581fdf, 0x00000001
	.section .unidentified.0858211f,"a"
	.incbin "baserom.gba", 0x0058211f, 0x00000001
	.section .unidentified.085880f6,"a"
	.incbin "baserom.gba", 0x005880f6, 0x00000002
	.section .unidentified.0858a9bd,"a"
	.incbin "baserom.gba", 0x0058a9bd, 0x00000003
	.section .unidentified.085931cb,"a"
	.incbin "baserom.gba", 0x005931cb, 0x00000001
	.section .unidentified.08593aca,"a"
	.incbin "baserom.gba", 0x00593aca, 0x00000002
	.section .unidentified.085949a3,"a"
	.incbin "baserom.gba", 0x005949a3, 0x00000001
	.section .unidentified.085965fb,"a"
	.incbin "baserom.gba", 0x005965fb, 0x00000001
	.section .unidentified.0859801b,"a"
	.incbin "baserom.gba", 0x0059801b, 0x00000001
	.section .unidentified.0859ab57,"a"
	.incbin "baserom.gba", 0x0059ab57, 0x00000001
	.section .unidentified.0859d2d1,"a"
	.incbin "baserom.gba", 0x0059d2d1, 0x00000003
	.section .unidentified.0859e2be,"a"
	.incbin "baserom.gba", 0x0059e2be, 0x00000002
	.section .unidentified.0859fb35,"a"
	.incbin "baserom.gba", 0x0059fb35, 0x00000003
	.section .unidentified.085a28c9,"a"
	.incbin "baserom.gba", 0x005a28c9, 0x00000003
	.section .unidentified.085a695e,"a"
	.incbin "baserom.gba", 0x005a695e, 0x00000002
	.section .unidentified.085a6a9f,"a"
	.incbin "baserom.gba", 0x005a6a9f, 0x00000001
	.section .unidentified.085a8b09,"a"
	.incbin "baserom.gba", 0x005a8b09, 0x00000003
	.section .unidentified.085aabab,"a"
	.incbin "baserom.gba", 0x005aabab, 0x00000001
	.section .unidentified.085abb38,"a"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005abb38, 0x000022d8
	.section .unidentified.085b0a86,"a"
	.incbin "baserom.gba", 0x005b0a86, 0x00000002
	.section .unidentified.085b81d3,"a"
	.incbin "baserom.gba", 0x005b81d3, 0x00000001
	.section .unidentified.085b86dd,"a"
	.incbin "baserom.gba", 0x005b86dd, 0x00000003
	.section .unidentified.085b9f59,"a"
	.incbin "baserom.gba", 0x005b9f59, 0x00000003
	.section .unidentified.085bb85e,"a"
	.incbin "baserom.gba", 0x005bb85e, 0x00000002
	.section .unidentified.085bd209,"a"
	.incbin "baserom.gba", 0x005bd209, 0x00000003
	.section .unidentified.085c1afd,"a"
	.incbin "baserom.gba", 0x005c1afd, 0x00000003
	.section .unidentified.085c2b42,"a"
	.incbin "baserom.gba", 0x005c2b42, 0x00000002
	.section .unidentified.085c3966,"a"
	.incbin "baserom.gba", 0x005c3966, 0x00000002
	.section .unidentified.085c4e05,"a"
	.incbin "baserom.gba", 0x005c4e05, 0x00000003
	.section .unidentified.085c753a,"a"
	.incbin "baserom.gba", 0x005c753a, 0x00000002
	.section .unidentified.085cbe2d,"a"
	.incbin "baserom.gba", 0x005cbe2d, 0x00000003
	.section .unidentified.085cc895,"a"
	.incbin "baserom.gba", 0x005cc895, 0x00000003
	.section .unidentified.085cf03e,"a"
	.incbin "baserom.gba", 0x005cf03e, 0x00000002
	.section .unidentified.085d0185,"a"
	.incbin "baserom.gba", 0x005d0185, 0x00000003
	.section .unidentified.085d204a,"a"
	.incbin "baserom.gba", 0x005d204a, 0x00000002
	.section .unidentified.085d4a0e,"a"
	.incbin "baserom.gba", 0x005d4a0e, 0x00000002
	.section .unidentified.085d764a,"a"
	.incbin "baserom.gba", 0x005d764a, 0x00000002
	.section .unidentified.085dade9,"a"
	.incbin "baserom.gba", 0x005dade9, 0x00000003
	.section .unidentified.085de2b3,"a"
	.incbin "baserom.gba", 0x005de2b3, 0x00000001
	.section .unidentified.085e0e62,"a"
	.incbin "baserom.gba", 0x005e0e62, 0x00000002
	.section .unidentified.085e30d6,"a"
	.incbin "baserom.gba", 0x005e30d6, 0x00000002
	.section .unidentified.085e5947,"a"
	.incbin "baserom.gba", 0x005e5947, 0x00000001
	.section .unidentified.085e8e3b,"a"
	.incbin "baserom.gba", 0x005e8e3b, 0x00000001
	.section .unidentified.085eda62,"a"
	.incbin "baserom.gba", 0x005eda62, 0x00000002
	.section .unidentified.085ee6e5,"a"
	.incbin "baserom.gba", 0x005ee6e5, 0x00000003
	.section .unidentified.085ee827,"a"
	.incbin "baserom.gba", 0x005ee827, 0x00000001
	.section .unidentified.085eff02,"a"
	.incbin "baserom.gba", 0x005eff02, 0x00000002
	.section .unidentified.085f1c5d,"a"
	.incbin "baserom.gba", 0x005f1c5d, 0x00000003
	.section .unidentified.085f1d9f,"a"
	.incbin "baserom.gba", 0x005f1d9f, 0x00000001
	.section .unidentified.085f459e,"a"
	.incbin "baserom.gba", 0x005f459e, 0x00000002
	.section .unidentified.085f46df,"a"
	.incbin "baserom.gba", 0x005f46df, 0x00000001
	.section .unidentified.085f7672,"a"
	.incbin "baserom.gba", 0x005f7672, 0x00000002
	.section .unidentified.085f8d3e,"a"
	.incbin "baserom.gba", 0x005f8d3e, 0x00000002
	.section .unidentified.085faff7,"a"
	.incbin "baserom.gba", 0x005faff7, 0x00000001
	.section .unidentified.085ff23e,"a"
	.incbin "baserom.gba", 0x005ff23e, 0x00000002
	.section .unidentified.0860101d,"a"
	.incbin "baserom.gba", 0x0060101d, 0x00000003
	.section .unidentified.086041cb,"a"
	.incbin "baserom.gba", 0x006041cb, 0x00000001
	.section .unidentified.086067ff,"a"
	.incbin "baserom.gba", 0x006067ff, 0x00000001
	.section .unidentified.0860951f,"a"
	.incbin "baserom.gba", 0x0060951f, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.section .unidentified.0860f929,"a"
	.incbin "baserom.gba", 0x0060f929, 0x00000003
	.section .unidentified.08610e9a,"a"
	.incbin "baserom.gba", 0x00610e9a, 0x00000002
	.section .unidentified.08613831,"a"
	.incbin "baserom.gba", 0x00613831, 0x00000003
	.section .unidentified.086160f3,"a"
	.incbin "baserom.gba", 0x006160f3, 0x00000001
	.section .unidentified.086188bb,"a"
	.incbin "baserom.gba", 0x006188bb, 0x00000001
	.section .unidentified.08619bb5,"a"
	.incbin "baserom.gba", 0x00619bb5, 0x00000003
	.section .unidentified.0861b4fb,"a"
	.incbin "baserom.gba", 0x0061b4fb, 0x00000001
	.section .unidentified.08626b4e,"a"
	.incbin "baserom.gba", 0x00626b4e, 0x00000002
	.section .unidentified.08629eb5,"a"
	.incbin "baserom.gba", 0x00629eb5, 0x00000003
	.section .unidentified.0862c3a6,"a"
	.incbin "baserom.gba", 0x0062c3a6, 0x00000002
	.section .unidentified.0862f187,"a"
	.incbin "baserom.gba", 0x0062f187, 0x00000001
	.section .unidentified.0862fd21,"a"
	.incbin "baserom.gba", 0x0062fd21, 0x00000003
	.section .unidentified.08630cda,"a"
	.incbin "baserom.gba", 0x00630cda, 0x00000002
	.section .unidentified.08638c93,"a"
	.incbin "baserom.gba", 0x00638c93, 0x00000001
	.section .unidentified.0863ba2f,"a"
	.incbin "baserom.gba", 0x0063ba2f, 0x00000001
	.section .unidentified.0863dbbe,"a"
	.incbin "baserom.gba", 0x0063dbbe, 0x00000002
	.section .unidentified.0863e2c6,"a"
	.incbin "baserom.gba", 0x0063e2c6, 0x00000002
	.section .unidentified.0863eeda,"a"
	.incbin "baserom.gba", 0x0063eeda, 0x00000002
	.section .unidentified.08650142,"a"
	.incbin "baserom.gba", 0x00650142, 0x00000002
	.section .unidentified.08651652,"a"
	.incbin "baserom.gba", 0x00651652, 0x00000002
	.section .unidentified.08653437,"a"
	.incbin "baserom.gba", 0x00653437, 0x00000001
	.section .unidentified.086544ca,"a"
	.incbin "baserom.gba", 0x006544ca, 0x00000002
	.section .unidentified.086568d1,"a"
	.incbin "baserom.gba", 0x006568d1, 0x00000003
	.section .unidentified.0865d64e,"a"
	.incbin "baserom.gba", 0x0065d64e, 0x00000002
	.section .unidentified.086610af,"a"
	.incbin "baserom.gba", 0x006610af, 0x00000001
	.section .unidentified.08663de5,"a"
	.incbin "baserom.gba", 0x00663de5, 0x00000003
	.section .unidentified.0866612d,"a"
	.incbin "baserom.gba", 0x0066612d, 0x00000003
	.section .unidentified.08667abd,"a"
	.incbin "baserom.gba", 0x00667abd, 0x00000003
	.section .unidentified.086785bd,"a"
	.incbin "baserom.gba", 0x006785bd, 0x00000003
	.section .unidentified.0867be7a,"a"
	.incbin "baserom.gba", 0x0067be7a, 0x00000002
	.section .unidentified.08684543,"a"
	.incbin "baserom.gba", 0x00684543, 0x00000001
	.section .unidentified.0868674e,"a"
	.incbin "baserom.gba", 0x0068674e, 0x00000002
	.section .unidentified.0868688f,"a"
	.incbin "baserom.gba", 0x0068688f, 0x00000001
	.section .unidentified.086882a6,"a"
	.incbin "baserom.gba", 0x006882a6, 0x00000002
	.section .unidentified.0868f18b,"a"
	.incbin "baserom.gba", 0x0068f18b, 0x00000001
	.section .unidentified.08691135,"a"
	.incbin "baserom.gba", 0x00691135, 0x00000003
	.section .unidentified.086938ae,"a"
	.incbin "baserom.gba", 0x006938ae, 0x00000002
	.section .unidentified.08695115,"a"
	.incbin "baserom.gba", 0x00695115, 0x00000003
	.section .unidentified.08698cff,"a"
	.incbin "baserom.gba", 0x00698cff, 0x00000001
	.section .unidentified.086a5fbd,"a"
	.incbin "baserom.gba", 0x006a5fbd, 0x00000003
	.section .unidentified.086a7a42,"a"
	.incbin "baserom.gba", 0x006a7a42, 0x00000002
	.section .unidentified.086aac3f,"a"
	.incbin "baserom.gba", 0x006aac3f, 0x00000001
	.section .unidentified.086addcd,"a"
	.incbin "baserom.gba", 0x006addcd, 0x00000003
	.section .unidentified.086b2df7,"a"
	.incbin "baserom.gba", 0x006b2df7, 0x00000001
	.section .unidentified.086b5d47,"a"
	.incbin "baserom.gba", 0x006b5d47, 0x00000001
	.section .unidentified.086b840d,"a"
	.incbin "baserom.gba", 0x006b840d, 0x00000003
	.section .unidentified.086ba49e,"a"
	.incbin "baserom.gba", 0x006ba49e, 0x00000002
	.section .unidentified.086bc59e,"a"
	.incbin "baserom.gba", 0x006bc59e, 0x00000002
	.section .unidentified.086bf152,"a"
	.incbin "baserom.gba", 0x006bf152, 0x00000002
	.section .unidentified.086bfe12,"a"
	.incbin "baserom.gba", 0x006bfe12, 0x00000002
	.section .unidentified.086c2741,"a"
	.incbin "baserom.gba", 0x006c2741, 0x00000003
	.section .unidentified.086c3ed9,"a"
	.incbin "baserom.gba", 0x006c3ed9, 0x00000003
	.section .unidentified.086c514d,"a"
	.incbin "baserom.gba", 0x006c514d, 0x00000003
	.section .unidentified.086c75dd,"a"
	.incbin "baserom.gba", 0x006c75dd, 0x00000003
	.section .unidentified.086c9929,"a"
	.incbin "baserom.gba", 0x006c9929, 0x00000003
	.section .unidentified.086cac9a,"a"
	.incbin "baserom.gba", 0x006cac9a, 0x00000002
	.section .unidentified.086cf1e6,"a"
	.incbin "baserom.gba", 0x006cf1e6, 0x00000002
	.section .unidentified.086cf33b,"a"
	.incbin "baserom.gba", 0x006cf33b, 0x00000001
	.section .unidentified.086cf47b,"a"
	.incbin "baserom.gba", 0x006cf47b, 0x00000001
	.section .unidentified.086d02ce,"a"
	.incbin "baserom.gba", 0x006d02ce, 0x00000002
	.section .unidentified.086d1d09,"a"
	.incbin "baserom.gba", 0x006d1d09, 0x00000003
	.section .unidentified.086d3915,"a"
	.incbin "baserom.gba", 0x006d3915, 0x00000003
	.section .unidentified.086d577f,"a"
	.incbin "baserom.gba", 0x006d577f, 0x00000001
	.section .unidentified.086d58f5,"a"
	.incbin "baserom.gba", 0x006d58f5, 0x00000003
	.section .unidentified.086d7921,"a"
	.incbin "baserom.gba", 0x006d7921, 0x00000003
	.section .unidentified.086d9cde,"a"
	.incbin "baserom.gba", 0x006d9cde, 0x00000002
	.section .unidentified.086dc8f5,"a"
	.incbin "baserom.gba", 0x006dc8f5, 0x00000003
	.section .unidentified.086e0967,"a"
	.incbin "baserom.gba", 0x006e0967, 0x00000001
	.section .unidentified.086e2ef1,"a"
	.incbin "baserom.gba", 0x006e2ef1, 0x00000003
	.section .unidentified.086e6352,"a"
	.incbin "baserom.gba", 0x006e6352, 0x00000002
	.section .unidentified.086e8a76,"a"
	.incbin "baserom.gba", 0x006e8a76, 0x00000002
	.section .unidentified.086eb66e,"a"
	.incbin "baserom.gba", 0x006eb66e, 0x00000002
	.section .unidentified.086ec322,"a"
	.incbin "baserom.gba", 0x006ec322, 0x00000002
	.section .unidentified.086eed37,"a"
	.incbin "baserom.gba", 0x006eed37, 0x00000001
	.section .unidentified.086f0976,"a"
	.incbin "baserom.gba", 0x006f0976, 0x00000002
	.section .unidentified.086f0e33,"a"
	.incbin "baserom.gba", 0x006f0e33, 0x00000001
	.section .unidentified.086f21ab,"a"
	.incbin "baserom.gba", 0x006f21ab, 0x00000001
	.section .unidentified.086f3879,"a"
	.incbin "baserom.gba", 0x006f3879, 0x00000003
	.section .unidentified.086f4ae5,"a"
	.incbin "baserom.gba", 0x006f4ae5, 0x00000003
	.section .unidentified.086f6739,"a"
	.incbin "baserom.gba", 0x006f6739, 0x00000003
	.section .unidentified.086f911b,"a"
	.incbin "baserom.gba", 0x006f911b, 0x00000001
	.section .unidentified.086fc645,"a"
	.incbin "baserom.gba", 0x006fc645, 0x00000003
	.section .unidentified.086fdf4f,"a"
	.incbin "baserom.gba", 0x006fdf4f, 0x00000001
	.section .unidentified.08702a4b,"a"
	.incbin "baserom.gba", 0x00702a4b, 0x00000001
	.section .unidentified.08704697,"a"
	.incbin "baserom.gba", 0x00704697, 0x00000001
	.section .unidentified.08705add,"a"
	.incbin "baserom.gba", 0x00705add, 0x00000003
	.section .unidentified.0870795d,"a"
	.incbin "baserom.gba", 0x0070795d, 0x00000003
	.section .unidentified.0870952e,"a"
	.incbin "baserom.gba", 0x0070952e, 0x00000002
	.section .unidentified.0870bc62,"a"
	.incbin "baserom.gba", 0x0070bc62, 0x00000002
	.section .unidentified.0870d8a7,"a"
	.incbin "baserom.gba", 0x0070d8a7, 0x00000001
	.section .unidentified.0870eced,"a"
	.incbin "baserom.gba", 0x0070eced, 0x00000003
	.section .unidentified.0871086b,"a"
	.incbin "baserom.gba", 0x0071086b, 0x00000001
	.section .unidentified.08711bf5,"a"
	.incbin "baserom.gba", 0x00711bf5, 0x00000003
	.section .unidentified.0871525e,"a"
	.incbin "baserom.gba", 0x0071525e, 0x00000002
	.section .unidentified.08717b8c,"a"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.section .unidentified.0872b9d5,"a"
	.incbin "baserom.gba", 0x0072b9d5, 0x00000003
	.section .unidentified.0872e3a1,"a"
	.incbin "baserom.gba", 0x0072e3a1, 0x00000003
	.section .unidentified.08735e82,"a"
	.incbin "baserom.gba", 0x00735e82, 0x00000002
	.section .unidentified.0873a0e7,"a"
	.incbin "baserom.gba", 0x0073a0e7, 0x00000001
	.section .unidentified.0873d902,"a"
	.incbin "baserom.gba", 0x0073d902, 0x00000002
	.section .unidentified.0873f32a,"a"
	.incbin "baserom.gba", 0x0073f32a, 0x00000002
	.section .unidentified.08743a3b,"a"
	.incbin "baserom.gba", 0x00743a3b, 0x00000001
	.section .unidentified.087458f7,"a"
	.incbin "baserom.gba", 0x007458f7, 0x00000001
	.section .unidentified.08748b41,"a"
	.incbin "baserom.gba", 0x00748b41, 0x00000003
	.section .unidentified.0874a675,"a"
	.incbin "baserom.gba", 0x0074a675, 0x00000003
	.section .unidentified.0874d83a,"a"
	.incbin "baserom.gba", 0x0074d83a, 0x00000002
	.section .unidentified.0874f22a,"a"
	.incbin "baserom.gba", 0x0074f22a, 0x00000002
	.section .unidentified.08752ba7,"a"
	.incbin "baserom.gba", 0x00752ba7, 0x00000001
	.section .unidentified.08753b15,"a"
	.incbin "baserom.gba", 0x00753b15, 0x00000003
	.section .unidentified.08758b3a,"a"
	.incbin "baserom.gba", 0x00758b3a, 0x00000002
	.section .unidentified.0875bdd8,"a"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
	.section .unidentified.087604e2,"a"
	.incbin "baserom.gba", 0x007604e2, 0x00000002
	.section .unidentified.08763cc1,"a"
	.incbin "baserom.gba", 0x00763cc1, 0x00000003
	.section .unidentified.0876a00b,"a"
	.incbin "baserom.gba", 0x0076a00b, 0x00000001
	.section .unidentified.0876ab0d,"a"
	.incbin "baserom.gba", 0x0076ab0d, 0x00000003
	.section .unidentified.0876ada9,"a"
	.incbin "baserom.gba", 0x0076ada9, 0x00000003
	.section .unidentified.0876ee06,"a"
	.incbin "baserom.gba", 0x0076ee06, 0x00000002
	.section .unidentified.0876f3ba,"a"
	.incbin "baserom.gba", 0x0076f3ba, 0x00000002
	.section .unidentified.08771d52,"a"
	.incbin "baserom.gba", 0x00771d52, 0x00000002
	.section .unidentified.087729c1,"a"
	.incbin "baserom.gba", 0x007729c1, 0x00000003
	.section .unidentified.08772c69,"a"
	.incbin "baserom.gba", 0x00772c69, 0x00000003
	.section .unidentified.08773aee,"a"
	.incbin "baserom.gba", 0x00773aee, 0x00000002
	.section .unidentified.08774e47,"a"
	.incbin "baserom.gba", 0x00774e47, 0x00000001
	.section .unidentified.08775194,"a"
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x00775194, 0x0000000c
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x007751a0, 0x00000150
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x007752f0, 0x00000140
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x00775430, 0x00000140
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x00775570, 0x00000140
	.section .unidentified.087759fc,"a"
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x007759fc, 0x0000000c
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x00775a08, 0x00000150
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x00775b58, 0x00000140
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x00775c98, 0x00000140
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x00775dd8, 0x00000140
	.section .unidentified.08776264,"a"
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x00776264, 0x0000000c
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x00776270, 0x00000150
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x007763c0, 0x00000140
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x00776500, 0x00000140
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x00776640, 0x00000140
	.section .unidentified.08776acc,"a"
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x00776acc, 0x0000000c
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x00776ad8, 0x00000150
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x00776c28, 0x00000140
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x00776d68, 0x00000140
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x00776ea8, 0x00000140
	.section .unidentified.08777334,"a"
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x00777334, 0x0000000c
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x00777340, 0x00000150
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x00777490, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x007775d0, 0x00000140
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x00777710, 0x00000140
	.section .unidentified.08777b9c,"a"
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x00777b9c, 0x0000000c
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x00777ba8, 0x00000150
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x00777cf8, 0x00000140
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x00777e38, 0x00000140
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x00777f78, 0x00000140
	.section .unidentified.08778404,"a"
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00778404, 0x0000000c
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x00778410, 0x00000150
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x00778560, 0x00000140
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x007786a0, 0x00000140
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x007787e0, 0x00000140
	.section .unidentified.08778c6c,"a"
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x00778c6c, 0x0000000c
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x00778c78, 0x00000150
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x00778dc8, 0x00000140
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x00778f08, 0x00000140
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x00779048, 0x00000140
	.section .unidentified.087fd4b9,"a"
	.incbin "baserom.gba", 0x007fd4b9, 0x00002b47
	.section .unidentified.084baaa3,"a"
	.incbin "baserom.gba", 0x004baaa3, 0x00000001
	.section .unidentified.084c03cf,"a"
	.incbin "baserom.gba", 0x004c03cf, 0x00000001
	.section .unidentified.084cecf1,"a"
	.incbin "baserom.gba", 0x004cecf1, 0x00000003
	.section .unidentified.084db8ca,"a"
	.incbin "baserom.gba", 0x004db8ca, 0x00000002
	.section .unidentified.084e60fa,"a"
	.incbin "baserom.gba", 0x004e60fa, 0x00000002
	.section .unidentified.084f1221,"a"
	.incbin "baserom.gba", 0x004f1221, 0x00000003
	.section .unidentified.084fb067,"a"
	.incbin "baserom.gba", 0x004fb067, 0x00000001
	.section .unidentified.08502bff,"a"
	.incbin "baserom.gba", 0x00502bff, 0x00000001
	.section .unidentified.0850886b,"a"
	.incbin "baserom.gba", 0x0050886b, 0x00000001
	.section .unidentified.0850d02a,"a"
	.incbin "baserom.gba", 0x0050d02a, 0x00000002
	.section .unidentified.08514053,"a"
	.incbin "baserom.gba", 0x00514053, 0x00000001
	.section .unidentified.08518cda,"a"
	.incbin "baserom.gba", 0x00518cda, 0x00000002
	.section .unidentified.0851d292,"a"
	.incbin "baserom.gba", 0x0051d292, 0x00000002
	.section .unidentified.0851deab,"a"
	.incbin "baserom.gba", 0x0051deab, 0x00000001
	.section .unidentified.08521b9b,"a"
	.incbin "baserom.gba", 0x00521b9b, 0x00000001
	.section .unidentified.08527937,"a"
	.incbin "baserom.gba", 0x00527937, 0x00000001
	.section .unidentified.0852dee6,"a"
	.incbin "baserom.gba", 0x0052dee6, 0x00000002
	.section .unidentified.0852ff8a,"a"
	.incbin "baserom.gba", 0x0052ff8a, 0x00000002
	.section .unidentified.0853b9d1,"a"
	.incbin "baserom.gba", 0x0053b9d1, 0x00000003
	.section .unidentified.085426cd,"a"
	.incbin "baserom.gba", 0x005426cd, 0x00000003
	.section .unidentified.08546aff,"a"
	.incbin "baserom.gba", 0x00546aff, 0x00000001
	.section .unidentified.085511d3,"a"
	.incbin "baserom.gba", 0x005511d3, 0x00000001
	.section .unidentified.085580b9,"a"
	.incbin "baserom.gba", 0x005580b9, 0x00000003
	.section .unidentified.08560fce,"a"
	.incbin "baserom.gba", 0x00560fce, 0x00000002
	.section .unidentified.08561f13,"a"
	.incbin "baserom.gba", 0x00561f13, 0x00000001
	.section .unidentified.085648fa,"a"
	.incbin "baserom.gba", 0x005648fa, 0x00000002
	.section .unidentified.0856e772,"a"
	.incbin "baserom.gba", 0x0056e772, 0x00000002
	.section .unidentified.0857153f,"a"
	.incbin "baserom.gba", 0x0057153f, 0x00000001
	.section .unidentified.08573223,"a"
	.incbin "baserom.gba", 0x00573223, 0x00000001
	.section .unidentified.0857d531,"a"
	.incbin "baserom.gba", 0x0057d531, 0x00000003
	.section .unidentified.08583423,"a"
	.incbin "baserom.gba", 0x00583423, 0x00000001
	.section .unidentified.08585179,"a"
	.incbin "baserom.gba", 0x00585179, 0x00000003
	.section .unidentified.08591925,"a"
	.incbin "baserom.gba", 0x00591925, 0x00000003
	.section .unidentified.08594843,"a"
	.incbin "baserom.gba", 0x00594843, 0x00000001
	.section .unidentified.08597e97,"a"
	.incbin "baserom.gba", 0x00597e97, 0x00000001
	.section .unidentified.0859f98b,"a"
	.incbin "baserom.gba", 0x0059f98b, 0x00000001
	.section .unidentified.085a8967,"a"
	.incbin "baserom.gba", 0x005a8967, 0x00000001
	.section .unidentified.085b0901,"a"
	.incbin "baserom.gba", 0x005b0901, 0x00000003
	.section .unidentified.085b9db5,"a"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .unidentified.085bd08b,"a"
	.incbin "baserom.gba", 0x005bd08b, 0x00000001
	.section .unidentified.085c4cab,"a"
	.incbin "baserom.gba", 0x005c4cab, 0x00000001
	.section .unidentified.085c73c3,"a"
	.incbin "baserom.gba", 0x005c73c3, 0x00000001
	.section .unidentified.085ceed7,"a"
	.incbin "baserom.gba", 0x005ceed7, 0x00000001
	.section .unidentified.085d1ee3,"a"
	.incbin "baserom.gba", 0x005d1ee3, 0x00000001
	.section .unidentified.085d48d9,"a"
	.incbin "baserom.gba", 0x005d48d9, 0x00000003
	.section .unidentified.085de137,"a"
	.incbin "baserom.gba", 0x005de137, 0x00000001
	.section .unidentified.085efd97,"a"
	.incbin "baserom.gba", 0x005efd97, 0x00000001
	.section .unidentified.085f60f7,"a"
	.incbin "baserom.gba", 0x005f60f7, 0x00000001
	.section .unidentified.085f8bae,"a"
	.incbin "baserom.gba", 0x005f8bae, 0x00000002
	.section .unidentified.08609357,"a"
	.incbin "baserom.gba", 0x00609357, 0x00000001
	.section .unidentified.08611cb3,"a"
	.incbin "baserom.gba", 0x00611cb3, 0x00000001
	.section .unidentified.086136c1,"a"
	.incbin "baserom.gba", 0x006136c1, 0x00000003
	.section .unidentified.0861d005,"a"
	.incbin "baserom.gba", 0x0061d005, 0x00000003
	.section .unidentified.0862096f,"a"
	.incbin "baserom.gba", 0x0062096f, 0x00000001
	.section .unidentified.08622ae7,"a"
	.incbin "baserom.gba", 0x00622ae7, 0x00000001
	.section .unidentified.08623a0e,"a"
	.incbin "baserom.gba", 0x00623a0e, 0x00000002
	.section .unidentified.0862698e,"a"
	.incbin "baserom.gba", 0x0062698e, 0x00000002
	.section .unidentified.08630b82,"a"
	.incbin "baserom.gba", 0x00630b82, 0x00000002
	.section .unidentified.0863193f,"a"
	.incbin "baserom.gba", 0x0063193f, 0x00000001
	.section .unidentified.0863270f,"a"
	.incbin "baserom.gba", 0x0063270f, 0x00000001
	.section .unidentified.0863399e,"a"
	.incbin "baserom.gba", 0x0063399e, 0x00000002
	.section .unidentified.08634886,"a"
	.incbin "baserom.gba", 0x00634886, 0x00000002
	.section .unidentified.0863595f,"a"
	.incbin "baserom.gba", 0x0063595f, 0x00000001
	.section .unidentified.08638b27,"a"
	.incbin "baserom.gba", 0x00638b27, 0x00000001
	.section .unidentified.086424ae,"a"
	.incbin "baserom.gba", 0x006424ae, 0x00000002
	.section .unidentified.08645fb6,"a"
	.incbin "baserom.gba", 0x00645fb6, 0x00000002
	.section .unidentified.08649cc6,"a"
	.incbin "baserom.gba", 0x00649cc6, 0x00000002
	.section .unidentified.0864dd25,"a"
	.incbin "baserom.gba", 0x0064dd25, 0x00000003
	.section .unidentified.0864ffad,"a"
	.incbin "baserom.gba", 0x0064ffad, 0x00000003
	.section .unidentified.0865673a,"a"
	.incbin "baserom.gba", 0x0065673a, 0x00000002
	.section .unidentified.08658877,"a"
	.incbin "baserom.gba", 0x00658877, 0x00000001
	.section .unidentified.0866790b,"a"
	.incbin "baserom.gba", 0x0066790b, 0x00000001
	.section .unidentified.08669033,"a"
	.incbin "baserom.gba", 0x00669033, 0x00000001
	.section .unidentified.0866ba02,"a"
	.incbin "baserom.gba", 0x0066ba02, 0x00000002
	.section .unidentified.0866dccb,"a"
	.incbin "baserom.gba", 0x0066dccb, 0x00000001
	.section .unidentified.0866f2a1,"a"
	.incbin "baserom.gba", 0x0066f2a1, 0x00000003
	.section .unidentified.08671362,"a"
	.incbin "baserom.gba", 0x00671362, 0x00000002
	.section .unidentified.0867368d,"a"
	.incbin "baserom.gba", 0x0067368d, 0x00000003
	.section .unidentified.086762b2,"a"
	.incbin "baserom.gba", 0x006762b2, 0x00000002
	.section .unidentified.0867773b,"a"
	.incbin "baserom.gba", 0x0067773b, 0x00000001
	.section .unidentified.0867845e,"a"
	.incbin "baserom.gba", 0x0067845e, 0x00000002
	.section .unidentified.08679efa,"a"
	.incbin "baserom.gba", 0x00679efa, 0x00000002
	.section .unidentified.0867bd17,"a"
	.incbin "baserom.gba", 0x0067bd17, 0x00000001
	.section .unidentified.0867f33d,"a"
	.incbin "baserom.gba", 0x0067f33d, 0x00000003
	.section .unidentified.08681b5d,"a"
	.incbin "baserom.gba", 0x00681b5d, 0x00000003
	.section .unidentified.0868a786,"a"
	.incbin "baserom.gba", 0x0068a786, 0x00000002
	.section .unidentified.0868d06e,"a"
	.incbin "baserom.gba", 0x0068d06e, 0x00000002
	.section .unidentified.0868f00d,"a"
	.incbin "baserom.gba", 0x0068f00d, 0x00000003
	.section .unidentified.08698b36,"a"
	.incbin "baserom.gba", 0x00698b36, 0x00000002
	.section .unidentified.086a0dab,"a"
	.incbin "baserom.gba", 0x006a0dab, 0x00000001
	.section .unidentified.086a1f23,"a"
	.incbin "baserom.gba", 0x006a1f23, 0x00000001
	.section .unidentified.086a3ce7,"a"
	.incbin "baserom.gba", 0x006a3ce7, 0x00000001
	.section .unidentified.086a5e33,"a"
	.incbin "baserom.gba", 0x006a5e33, 0x00000001
	.section .unidentified.086aaa79,"a"
	.incbin "baserom.gba", 0x006aaa79, 0x00000003
	.section .unidentified.086abb73,"a"
	.incbin "baserom.gba", 0x006abb73, 0x00000001
	.section .unidentified.086adc26,"a"
	.incbin "baserom.gba", 0x006adc26, 0x00000002
	.section .unidentified.086b5b87,"a"
	.incbin "baserom.gba", 0x006b5b87, 0x00000001
	.section .unidentified.086b823b,"a"
	.incbin "baserom.gba", 0x006b823b, 0x00000001
	.section .unidentified.086bfc3f,"a"
	.incbin "baserom.gba", 0x006bfc3f, 0x00000001
	.section .unidentified.086cab13,"a"
	.incbin "baserom.gba", 0x006cab13, 0x00000001
	.section .unidentified.086d014f,"a"
	.incbin "baserom.gba", 0x006d014f, 0x00000001
	.section .unidentified.086d1b87,"a"
	.incbin "baserom.gba", 0x006d1b87, 0x00000001
	.section .unidentified.086d379b,"a"
	.incbin "baserom.gba", 0x006d379b, 0x00000001
	.section .unidentified.086d5653,"a"
	.incbin "baserom.gba", 0x006d5653, 0x00000001
	.section .unidentified.086d779b,"a"
	.incbin "baserom.gba", 0x006d779b, 0x00000001
	.section .unidentified.086d887f,"a"
	.incbin "baserom.gba", 0x006d887f, 0x00000001
	.section .unidentified.086d9b23,"a"
	.incbin "baserom.gba", 0x006d9b23, 0x00000001
	.section .unidentified.086dc782,"a"
	.incbin "baserom.gba", 0x006dc782, 0x00000002
	.section .unidentified.086ddcdd,"a"
	.incbin "baserom.gba", 0x006ddcdd, 0x00000003
	.section .unidentified.086e07bd,"a"
	.incbin "baserom.gba", 0x006e07bd, 0x00000003
	.section .unidentified.086e2d62,"a"
	.incbin "baserom.gba", 0x006e2d62, 0x00000002
	.section .unidentified.086e6171,"a"
	.incbin "baserom.gba", 0x006e6171, 0x00000003
	.section .unidentified.086e725f,"a"
	.incbin "baserom.gba", 0x006e725f, 0x00000001
	.section .unidentified.086e88e7,"a"
	.incbin "baserom.gba", 0x006e88e7, 0x00000001
	.section .unidentified.086ec1bb,"a"
	.incbin "baserom.gba", 0x006ec1bb, 0x00000001
	.section .unidentified.086f36ef,"a"
	.incbin "baserom.gba", 0x006f36ef, 0x00000001
	.section .unidentified.086f4957,"a"
	.incbin "baserom.gba", 0x006f4957, 0x00000001
	.section .unidentified.086ffff7,"a"
	.incbin "baserom.gba", 0x006ffff7, 0x00000001
	.section .unidentified.08711a9b,"a"
	.incbin "baserom.gba", 0x00711a9b, 0x00000001
	.section .unidentified.08712b9e,"a"
	.incbin "baserom.gba", 0x00712b9e, 0x00000002
	.section .unidentified.0871421a,"a"
	.incbin "baserom.gba", 0x0071421a, 0x00000002
	.section .unidentified.087150f3,"a"
	.incbin "baserom.gba", 0x007150f3, 0x00000001
	.section .unidentified.08716c16,"a"
	.incbin "baserom.gba", 0x00716c16, 0x00000002
	.section .unidentified.08717b8a,"a"
	.incbin "baserom.gba", 0x00717b8a, 0x00000002
	.section .unidentified.0871a4d5,"a"
	.incbin "baserom.gba", 0x0071a4d5, 0x00000003
	.section .unidentified.0871c3d3,"a"
	.incbin "baserom.gba", 0x0071c3d3, 0x00000001
	.section .unidentified.0871d71b,"a"
	.incbin "baserom.gba", 0x0071d71b, 0x00000001
	.section .unidentified.08720012,"a"
	.incbin "baserom.gba", 0x00720012, 0x00000002
	.section .unidentified.08722b97,"a"
	.incbin "baserom.gba", 0x00722b97, 0x00000001
	.section .unidentified.08723f83,"a"
	.incbin "baserom.gba", 0x00723f83, 0x00000001
	.section .unidentified.0872548a,"a"
	.incbin "baserom.gba", 0x0072548a, 0x00000002
	.section .unidentified.0872934b,"a"
	.incbin "baserom.gba", 0x0072934b, 0x00000001
	.section .unidentified.0872a67a,"a"
	.incbin "baserom.gba", 0x0072a67a, 0x00000002
	.section .unidentified.0872b873,"a"
	.incbin "baserom.gba", 0x0072b873, 0x00000001
	.section .unidentified.087318cd,"a"
	.incbin "baserom.gba", 0x007318cd, 0x00000003
	.section .unidentified.08735d0f,"a"
	.incbin "baserom.gba", 0x00735d0f, 0x00000001
	.section .unidentified.087419d7,"a"
	.incbin "baserom.gba", 0x007419d7, 0x00000001
	.section .unidentified.0874a503,"a"
	.incbin "baserom.gba", 0x0074a503, 0x00000001
	.section .unidentified.0874bde6,"a"
	.incbin "baserom.gba", 0x0074bde6, 0x00000002
	.section .unidentified.0874d682,"a"
	.incbin "baserom.gba", 0x0074d682, 0x00000002
	.section .unidentified.0874f0ab,"a"
	.incbin "baserom.gba", 0x0074f0ab, 0x00000001
	.section .unidentified.08750b55,"a"
	.incbin "baserom.gba", 0x00750b55, 0x00000003
	.section .unidentified.08752a33,"a"
	.incbin "baserom.gba", 0x00752a33, 0x00000001
	.section .unidentified.0875396f,"a"
	.incbin "baserom.gba", 0x0075396f, 0x00000001
	.section .unidentified.08758953,"a"
	.incbin "baserom.gba", 0x00758953, 0x00000001
	.section .unidentified.0875b137,"a"
	.incbin "baserom.gba", 0x0075b137, 0x00000001
	.section .unidentified.0875bdd6,"a"
	.incbin "baserom.gba", 0x0075bdd6, 0x00000002
	.section .unidentified.087620b5,"a"
	.incbin "baserom.gba", 0x007620b5, 0x00000003
	.section .unidentified.08763b65,"a"
	.incbin "baserom.gba", 0x00763b65, 0x00000003
	.section .unidentified.0876519f,"a"
	.incbin "baserom.gba", 0x0076519f, 0x00000001
	.section .unidentified.087666ee,"a"
	.incbin "baserom.gba", 0x007666ee, 0x00000002
	.section .unidentified.087685b1,"a"
	.incbin "baserom.gba", 0x007685b1, 0x00000003
	.section .unidentified.08769f07,"a"
	.incbin "baserom.gba", 0x00769f07, 0x00000001
	.section .unidentified.0876aa27,"a"
	.incbin "baserom.gba", 0x0076aa27, 0x00000001
	.section .unidentified.0877034f,"a"
	.incbin "baserom.gba", 0x0077034f, 0x00000001
	.section .unidentified.08774c7f,"a"
	.incbin "baserom.gba", 0x00774c7f, 0x00000001
	.section .unidentified.08775193,"a"
	.incbin "baserom.gba", 0x00775193, 0x00000001
	.section .unidentified.087759fb,"a"
	.incbin "baserom.gba", 0x007759fb, 0x00000001
	.section .unidentified.08776263,"a"
	.incbin "baserom.gba", 0x00776263, 0x00000001
	.section .unidentified.08776acb,"a"
	.incbin "baserom.gba", 0x00776acb, 0x00000001
	.section .unidentified.08777333,"a"
	.incbin "baserom.gba", 0x00777333, 0x00000001
	.section .unidentified.08777b9b,"a"
	.incbin "baserom.gba", 0x00777b9b, 0x00000001
	.section .unidentified.08778403,"a"
	.incbin "baserom.gba", 0x00778403, 0x00000001
	.section .unidentified.08778c6b,"a"
	.incbin "baserom.gba", 0x00778c6b, 0x00000001
