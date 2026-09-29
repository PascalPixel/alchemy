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
	.global Data_08007abc
Data_08007abc:
	.incbin "baserom.gba", 0x00007abc, 0x00000058
	.section .unidentified.08007b38,"a"
	.incbin "baserom.gba", 0x00007b38, 0x0000008c
	.section .unidentified.08007bcc,"a"
	.incbin "baserom.gba", 0x00007bcc, 0x00000018
	.global Data_08007be4
Data_08007be4:
	.incbin "baserom.gba", 0x00007be4, 0x0000002c
	.global Data_08007c10
Data_08007c10:
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
	.incbin "baserom.gba", 0x00012fa0, 0x000001ac
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001314c, 0x00000044
	.global Camera_FixedViewMatrix
Camera_FixedViewMatrix:
	.incbin "baserom.gba", 0x00013190, 0x000000b0
	.global Script_MainScript
Script_MainScript:
	.incbin "baserom.gba", 0x00013240, 0x0000008c
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
	.section .unidentified.08029910,"a"
	.global WorkspaceOptions_SliderTiles
WorkspaceOptions_SliderTiles:
	.incbin "baserom.gba", 0x00029910, 0x00000100
	.global RomBytes_08029a10
RomBytes_08029a10:
	.global UiIcon_FramePointerTable
UiIcon_FramePointerTable:
	.incbin "baserom.gba", 0x00029a10, 0x000003f0
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
	.global Data_08032224
Data_08032224:
	.incbin "baserom.gba", 0x00032224, 0x000020d4
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "baserom.gba", 0x000342f8, 0x00000400
	.global Data_080346f8
Data_080346f8:
	.incbin "baserom.gba", 0x000346f8, 0x00002058
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
	.incbin "baserom.gba", 0x0003680c, 0x000009d4
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
	.incbin "baserom.gba", 0x000371e0, 0x00000026
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
	.incbin "baserom.gba", 0x00037300, 0x000000ef
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
	.incbin "baserom.gba", 0x0003742c, 0x00000038
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
	.incbin "baserom.gba", 0x00088e38, 0x00000434
	.global Djinn_DefinitionTable
Djinn_DefinitionTable:
	.incbin "baserom.gba", 0x0008926c, 0x00000d94
	.section .unidentified.0809c410,"a"
	.incbin "baserom.gba", 0x0009c410, 0x00000200
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
	.incbin "baserom.gba", 0x0009e8a0, 0x00000150
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.incbin "baserom.gba", 0x0009e9f0, 0x0000020c
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.incbin "baserom.gba", 0x0009ebfc, 0x00000184
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.incbin "baserom.gba", 0x0009ed80, 0x000002a4
	.global Data_0809f024
Data_0809f024:
	.incbin "baserom.gba", 0x0009f024, 0x00000080
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.incbin "baserom.gba", 0x0009f0a4, 0x0000000c
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.incbin "baserom.gba", 0x0009f0b0, 0x00000024
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.incbin "baserom.gba", 0x0009f0d4, 0x00000024
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.incbin "baserom.gba", 0x0009f0f8, 0x00000024
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.incbin "baserom.gba", 0x0009f11c, 0x00000044
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.incbin "baserom.gba", 0x0009f160, 0x00000048
	.section .unidentified.0809f7f0,"a"
	.incbin "baserom.gba", 0x0009f7f0, 0x00000020
	.global Data_0809f810
Data_0809f810:
	.incbin "baserom.gba", 0x0009f810, 0x000003bc
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
	.incbin "baserom.gba", 0x0009ff40, 0x000001c8
	.global Data_080a0108
Data_080a0108:
	.incbin "baserom.gba", 0x000a0108, 0x00000020
	.global BattleFx_UntargetedObjectScript
BattleFx_UntargetedObjectScript:
	.incbin "baserom.gba", 0x000a0128, 0x00000004
	.global gEffectScripts
gEffectScripts:
	.incbin "baserom.gba", 0x000a012c, 0x00000ed4
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
	.incbin "baserom.gba", 0x000aedcc, 0x00000440
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
	.incbin "baserom.gba", 0x000c5938, 0x0000006c
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
	.incbin "baserom.gba", 0x000eda78, 0x00000008
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
	.incbin "baserom.gba", 0x000edeb2, 0x0000025a
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
	.incbin "baserom.gba", 0x000ee9f2, 0x00000156
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
	.incbin "baserom.gba", 0x000eebc8, 0x00000097
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000eec5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000eec63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eec68, 0x000001b6
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
	.section .unidentified.080ef014,"a"
	.incbin "baserom.gba", 0x000ef014, 0x00000fec
	.section .unidentified.080f0a5c,"a"
	.incbin "baserom.gba", 0x000f0a5c, 0x000007c4
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f1220, 0x00000de0
	.section .unidentified.080f2b6c,"ax"
	.global Func_080f2b6c
	.thumb_func
Func_080f2b6c:
	.incbin "baserom.gba", 0x000f2b6c, 0x00000004
	.section .unidentified.080f38bc,"a"
	.incbin "baserom.gba", 0x000f38bc, 0x00000744
	.section .unidentified.080f53dc,"a"
	.incbin "baserom.gba", 0x000f53dc, 0x00000c24
	.section .unidentified.080f86f8,"a"
	.incbin "baserom.gba", 0x000f86f8, 0x0000003e
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
	.section .unidentified.08185024,"a"
	.global Character_DescriptorTable
Character_DescriptorTable:
	.incbin "baserom.gba", 0x00185024, 0x00003aa8
	.section .unidentified.08188adc,"a"
	.incbin "baserom.gba", 0x00188adc, 0x00000d10
	.section .unidentified.08189814,"a"
	.incbin "baserom.gba", 0x00189814, 0x001967ec
	.section .unidentified.083203c8,"a"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320fa0, 0x00000010
	.global Ui_WindowTiles
Ui_WindowTiles:
	.incbin "baserom.gba", 0x00320fb0, 0x00002000
	.global UiText_GlyphData
UiText_GlyphData:
	.incbin "baserom.gba", 0x00322fb0, 0x000008c0
	.global Title_IntroGraphicsA
Title_IntroGraphicsA:
	.incbin "baserom.gba", 0x00323870, 0x00001178
	.global Title_IntroGraphicsB
Title_IntroGraphicsB:
	.incbin "baserom.gba", 0x003249e8, 0x000066c0
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b0a8, 0x000086f8
	.global Title_NintendoLogo
Title_NintendoLogo:
	.incbin "baserom.gba", 0x003337a0, 0x00000828
	.global Title_IntroTiles
Title_IntroTiles:
	.incbin "baserom.gba", 0x00333fc8, 0x0000182c
	.global Title_GoldenSunLogo
Title_GoldenSunLogo:
	.incbin "baserom.gba", 0x003357f4, 0x00001910
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "baserom.gba", 0x00337104, 0x00003b04
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033ac08, 0x000002b4
	.global Battle_SuharaGateBackdrop
Battle_SuharaGateBackdrop:
	.incbin "baserom.gba", 0x0033aebc, 0x00004444
	.global Battle_TakaraShimaBackdrop
Battle_TakaraShimaBackdrop:
	.incbin "baserom.gba", 0x0033f300, 0x00004700
	.global Battle_TakaraShimaCaveBackdrop
Battle_TakaraShimaCaveBackdrop:
	.incbin "baserom.gba", 0x00343a00, 0x0000411c
	.global Battle_KorosseoBackdrop
Battle_KorosseoBackdrop:
	.incbin "baserom.gba", 0x00347b1c, 0x00004218
	.global Battle_VinasuChojoStormBackdrop
Battle_VinasuChojoStormBackdrop:
	.incbin "baserom.gba", 0x0034bd34, 0x00003d3c
	.global Battle_VinasuChojoBackdrop
Battle_VinasuChojoBackdrop:
	.incbin "baserom.gba", 0x0034fa70, 0x000035f0
	.global Battle_VinasuHeyaBackdrop
Battle_VinasuHeyaBackdrop:
	.incbin "baserom.gba", 0x00353060, 0x00003e58
	.global Battle_SuharaSabakuBackdrop
Battle_SuharaSabakuBackdrop:
	.incbin "baserom.gba", 0x00356eb8, 0x00003b74
	.global Battle_ArutamiraDouBackdrop
Battle_ArutamiraDouBackdrop:
	.incbin "baserom.gba", 0x0035aa2c, 0x000040d8
	.global Battle_BabiChikaBackdrop
Battle_BabiChikaBackdrop:
	.incbin "baserom.gba", 0x0035eb04, 0x00003f7c
	.global Battle_BabiIriguchiBackdrop
Battle_BabiIriguchiBackdrop:
	.incbin "baserom.gba", 0x00362a80, 0x00004400
	.global Battle_RamakanSabakuBackdrop
Battle_RamakanSabakuBackdrop:
	.incbin "baserom.gba", 0x00366e80, 0x0000400c
	.global Battle_MogoruMoriBackdrop
Battle_MogoruMoriBackdrop:
	.incbin "baserom.gba", 0x0036ae8c, 0x00004328
	.global Battle_MakyuriChojoBackdrop
Battle_MakyuriChojoBackdrop:
	.incbin "baserom.gba", 0x0036f1b4, 0x00003a28
	.global Battle_FuneHeyaBackdrop
Battle_FuneHeyaBackdrop:
	.incbin "baserom.gba", 0x00372bdc, 0x00004078
	.global Battle_ImiruFuchinBackdrop
Battle_ImiruFuchinBackdrop:
	.incbin "baserom.gba", 0x00376c54, 0x00003840
	.global Battle_KorimaMoriBackdrop
Battle_KorimaMoriBackdrop:
	.incbin "baserom.gba", 0x0037a494, 0x00004a38
	.global Battle_ToretoHeyaBackdrop
Battle_ToretoHeyaBackdrop:
	.incbin "baserom.gba", 0x0037eecc, 0x000049fc
	.global Battle_GomaDouBackdrop
Battle_GomaDouBackdrop:
	.incbin "baserom.gba", 0x003838c8, 0x0000427c
	.global Battle_InteriorBackdrop
Battle_InteriorBackdrop:
	.incbin "baserom.gba", 0x00387b44, 0x00003660
	.global Battle_ArutinYamaBackdrop
Battle_ArutinYamaBackdrop:
	.incbin "baserom.gba", 0x0038b1a4, 0x00003b4c
	.global Battle_FuneKanpanBackdrop
Battle_FuneKanpanBackdrop:
	.incbin "baserom.gba", 0x0038ecf0, 0x00003e88
	.global Battle_MakyuriHeyaBackdrop
Battle_MakyuriHeyaBackdrop:
	.incbin "baserom.gba", 0x00392b78, 0x00004034
	.global Battle_HaidiaArashiBackdrop
Battle_HaidiaArashiBackdrop:
	.incbin "baserom.gba", 0x00396bac, 0x00004a90
	.global Battle_ToretoEdaBackdrop
Battle_ToretoEdaBackdrop:
	.incbin "baserom.gba", 0x0039b63c, 0x000042b4
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "baserom.gba", 0x0039f8f0, 0x00003980
	.global Battle_SnowfieldBackdrop
Battle_SnowfieldBackdrop:
	.incbin "baserom.gba", 0x003a3270, 0x00004210
	.global Battle_GrasslandBackdrop
Battle_GrasslandBackdrop:
	.incbin "baserom.gba", 0x003a7480, 0x00004374
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "baserom.gba", 0x003ab7f4, 0x000042b8
	.global Battle_ForestBackdrop
Battle_ForestBackdrop:
	.incbin "baserom.gba", 0x003afaac, 0x0000471c
	.global Battle_VioletSkyBackdrop
Battle_VioletSkyBackdrop:
	.incbin "baserom.gba", 0x003b41c8, 0x00003484
	.global Battle_DesertBackdrop
Battle_DesertBackdrop:
	.incbin "baserom.gba", 0x003b764c, 0x000033fc
	.global Battle_StormCloudBackdrop
Battle_StormCloudBackdrop:
	.incbin "baserom.gba", 0x003baa48, 0x000039ac
	.global Battle_AerialViewBackdrop
Battle_AerialViewBackdrop:
	.incbin "baserom.gba", 0x003be3f4, 0x00005180
	.global BattleFx_ParticleSequenceDataA
BattleFx_ParticleSequenceDataA:
	.incbin "baserom.gba", 0x003c3574, 0x00000784
	.global BattleFx_ParticleSequenceDataB
BattleFx_ParticleSequenceDataB:
	.incbin "baserom.gba", 0x003c3cf8, 0x00001334
	.global BattleFx_ParticleSequenceDataC
BattleFx_ParticleSequenceDataC:
	.incbin "baserom.gba", 0x003c502c, 0x0000064c
	.global LuckyDice_GraphicsA
LuckyDice_GraphicsA:
	.incbin "baserom.gba", 0x003c5678, 0x00002e20
	.global LuckyDice_GraphicsB
LuckyDice_GraphicsB:
	.incbin "baserom.gba", 0x003c8498, 0x00003024
	.global BattleFx_TileAnimationMask
BattleFx_TileAnimationMask:
	.incbin "baserom.gba", 0x003cb4bc, 0x00000848
	.global BattleFx_CyanSparkSheet
BattleFx_CyanSparkSheet:
	.incbin "baserom.gba", 0x003cbd04, 0x00001200
	.global BattleFx_VioletPaletteA
BattleFx_VioletPaletteA:
	.incbin "baserom.gba", 0x003ccf04, 0x00000084
	.global BattleFx_RedPaletteA
BattleFx_RedPaletteA:
	.incbin "baserom.gba", 0x003ccf88, 0x00000084
	.global BattleFx_YellowPaletteA
BattleFx_YellowPaletteA:
	.incbin "baserom.gba", 0x003cd00c, 0x00000084
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
	.section .unidentified.083d2324,"a"
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "baserom.gba", 0x003d2324, 0x0000106c
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "baserom.gba", 0x003d3390, 0x00000fc8
	.section .unidentified.083d45b2,"a"
	.incbin "baserom.gba", 0x003d45b2, 0x00000002
	.section .unidentified.083d5d10,"a"
	.global BattleFx_YellowSparkSheet
BattleFx_YellowSparkSheet:
	.incbin "baserom.gba", 0x003d5d10, 0x00000300
	.global BattleFx_ProjectileVolleyImage
BattleFx_ProjectileVolleyImage:
	.incbin "baserom.gba", 0x003d6010, 0x000001b4
	.section .unidentified.083d6559,"a"
	.incbin "baserom.gba", 0x003d6559, 0x00000003
	.global BattleFx_FlareSheet
BattleFx_FlareSheet:
	.incbin "baserom.gba", 0x003d655c, 0x00001d88
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "baserom.gba", 0x003d82e4, 0x00000278
	.global BattleFx_VioletPaletteB
BattleFx_VioletPaletteB:
	.incbin "baserom.gba", 0x003d855c, 0x00000084
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "baserom.gba", 0x003d85e0, 0x00000444
	.section .unidentified.083da5f7,"a"
	.incbin "baserom.gba", 0x003da5f7, 0x00000001
	.section .unidentified.083dc1f5,"a"
	.incbin "baserom.gba", 0x003dc1f5, 0x00000003
	.section .unidentified.083dc416,"a"
	.incbin "baserom.gba", 0x003dc416, 0x00000002
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "baserom.gba", 0x003dc418, 0x0000043c
	.section .unidentified.083dc965,"a"
	.incbin "baserom.gba", 0x003dc965, 0x00000003
	.global BattleFx_OverlayImageB
BattleFx_OverlayImageB:
	.incbin "baserom.gba", 0x003dc968, 0x0000019c
	.section .unidentified.083dd0e3,"a"
	.incbin "baserom.gba", 0x003dd0e3, 0x00000001
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "baserom.gba", 0x003dd0e4, 0x000004a0
	.global BattleFx_SlashSheet
BattleFx_SlashSheet:
	.incbin "baserom.gba", 0x003dd584, 0x00000d14
	.section .unidentified.083de7d8,"a"
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "baserom.gba", 0x003de7d8, 0x00000840
	.section .unidentified.083df3a7,"a"
	.incbin "baserom.gba", 0x003df3a7, 0x00000001
	.global BattleFx_WaterSpraySheet
BattleFx_WaterSpraySheet:
	.incbin "baserom.gba", 0x003df3a8, 0x00001258
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "baserom.gba", 0x003e0600, 0x00000af0
	.section .unidentified.083e1ec5,"a"
	.incbin "baserom.gba", 0x003e1ec5, 0x00000003
	.section .unidentified.083e26f2,"a"
	.incbin "baserom.gba", 0x003e26f2, 0x00000002
	.section .unidentified.083e2ae4,"a"
	.global BattleFx_ParticleSpritesA
BattleFx_ParticleSpritesA:
	.incbin "baserom.gba", 0x003e2ae4, 0x0000029c
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "baserom.gba", 0x003e2d80, 0x000001bc
	.global BattleFx_ParticleSpritesC
BattleFx_ParticleSpritesC:
	.incbin "baserom.gba", 0x003e2f3c, 0x00000940
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "baserom.gba", 0x003e387c, 0x00000418
	.global BattleFx_GreenPalette
BattleFx_GreenPalette:
	.incbin "baserom.gba", 0x003e3c94, 0x00000084
	.global BattleFx_SwordSlashSheet
BattleFx_SwordSlashSheet:
	.incbin "baserom.gba", 0x003e3d18, 0x000004c8
	.section .unidentified.083e45b9,"a"
	.incbin "baserom.gba", 0x003e45b9, 0x00000003
	.section .unidentified.083e4967,"a"
	.incbin "baserom.gba", 0x003e4967, 0x00000001
	.global BattleFx_EarthWallSheet
BattleFx_EarthWallSheet:
	.incbin "baserom.gba", 0x003e4968, 0x00000ad4
	.global BattleFx_OrangePaletteA
BattleFx_OrangePaletteA:
	.incbin "baserom.gba", 0x003e543c, 0x00000084
	.section .unidentified.083e690b,"a"
	.incbin "baserom.gba", 0x003e690b, 0x00000001
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "baserom.gba", 0x003e690c, 0x00000d9c
	.section .unidentified.083e78c3,"a"
	.incbin "baserom.gba", 0x003e78c3, 0x00000001
	.section .unidentified.083e7bbf,"a"
	.incbin "baserom.gba", 0x003e7bbf, 0x00000001
	.section .unidentified.083e7f60,"a"
	.global BattleFx_WhirlwindSheet
BattleFx_WhirlwindSheet:
	.incbin "baserom.gba", 0x003e7f60, 0x000016b0
	.global BattleFx_GreenPillarSheet
BattleFx_GreenPillarSheet:
	.incbin "baserom.gba", 0x003e9610, 0x00000760
	.global BattleFx_VineSheet
BattleFx_VineSheet:
	.incbin "baserom.gba", 0x003e9d70, 0x00000dcc
	.global BattleFx_FirePillarSheetA
BattleFx_FirePillarSheetA:
	.incbin "baserom.gba", 0x003eab3c, 0x00000c74
	.global BattleFx_RedPaletteB
BattleFx_RedPaletteB:
	.incbin "baserom.gba", 0x003eb7b0, 0x00000084
	.global BattleFx_OrangePaletteB
BattleFx_OrangePaletteB:
	.incbin "baserom.gba", 0x003eb834, 0x00000084
	.global BattleFx_VioletCometSheet
BattleFx_VioletCometSheet:
	.incbin "baserom.gba", 0x003eb8b8, 0x000003e8
	.section .unidentified.083ec1f7,"a"
	.incbin "baserom.gba", 0x003ec1f7, 0x00000001
	.global BattleFx_BoulderSheet
BattleFx_BoulderSheet:
	.incbin "baserom.gba", 0x003ec1f8, 0x00000c18
	.section .unidentified.083edae1,"a"
	.incbin "baserom.gba", 0x003edae1, 0x00000003
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "baserom.gba", 0x003edae4, 0x000008a0
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
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "baserom.gba", 0x003f5210, 0x00000198
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "baserom.gba", 0x003f53a8, 0x0000088c
	.global BattleFx_BluePalette
BattleFx_BluePalette:
	.incbin "baserom.gba", 0x003f5c34, 0x00000084
	.global BattleFx_LimePalette
BattleFx_LimePalette:
	.incbin "baserom.gba", 0x003f5cb8, 0x00000084
	.global BattleFx_PinkPalette
BattleFx_PinkPalette:
	.incbin "baserom.gba", 0x003f5d3c, 0x00000084
	.global BattleFx_VioletPaletteC
BattleFx_VioletPaletteC:
	.incbin "baserom.gba", 0x003f5dc0, 0x00000084
	.global BattleFx_CyanPalette
BattleFx_CyanPalette:
	.incbin "baserom.gba", 0x003f5e44, 0x00000084
	.global BattleFx_TanPalette
BattleFx_TanPalette:
	.incbin "baserom.gba", 0x003f5ec8, 0x00000084
	.section .unidentified.083f66fa,"a"
	.incbin "baserom.gba", 0x003f66fa, 0x00000002
	.section .unidentified.083f6c12,"a"
	.incbin "baserom.gba", 0x003f6c12, 0x00000002
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "baserom.gba", 0x003f6c14, 0x00000624
	.section .unidentified.083f7a9d,"a"
	.incbin "baserom.gba", 0x003f7a9d, 0x00000003
	.section .unidentified.083f7ec1,"a"
	.incbin "baserom.gba", 0x003f7ec1, 0x00000003
	.global BattleFx_HeartSheet
BattleFx_HeartSheet:
	.incbin "baserom.gba", 0x003f7ec4, 0x00000298
	.global BattleFx_CounterRevealSheet
BattleFx_CounterRevealSheet:
	.incbin "baserom.gba", 0x003f815c, 0x00001500
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "baserom.gba", 0x003f965c, 0x0000049c
	.global BattleFx_BlueBeastSheet
BattleFx_BlueBeastSheet:
	.incbin "baserom.gba", 0x003f9af8, 0x0000198c
	.section .unidentified.083fb86e,"a"
	.incbin "baserom.gba", 0x003fb86e, 0x00000002
	.section .unidentified.083fd7e3,"a"
	.incbin "baserom.gba", 0x003fd7e3, 0x00000001
	.section .unidentified.083fdcb3,"a"
	.incbin "baserom.gba", 0x003fdcb3, 0x00000001
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "baserom.gba", 0x003fdcb4, 0x00000694
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe348, 0x00000a34
	.global BattleFx_EmberStreakSheet
BattleFx_EmberStreakSheet:
	.incbin "baserom.gba", 0x003fed7c, 0x00000bfc
	.section .unidentified.0840014d,"a"
	.incbin "baserom.gba", 0x0040014d, 0x00000003
	.section .unidentified.08400c1e,"a"
	.incbin "baserom.gba", 0x00400c1e, 0x00000002
	.section .unidentified.0840175b,"a"
	.incbin "baserom.gba", 0x0040175b, 0x00000001
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "baserom.gba", 0x0040175c, 0x00000640
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00401d9c, 0x00001588
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "baserom.gba", 0x00403324, 0x00000064
	.section .unidentified.084036f3,"a"
	.incbin "baserom.gba", 0x004036f3, 0x00000001
	.section .unidentified.08403c91,"a"
	.incbin "baserom.gba", 0x00403c91, 0x00000003
	.section .unidentified.08403fc5,"a"
	.incbin "baserom.gba", 0x00403fc5, 0x00000003
	.section .unidentified.08404301,"a"
	.incbin "baserom.gba", 0x00404301, 0x00000003
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "baserom.gba", 0x00404304, 0x00001018
	.global BattleFx_BlastSheet
BattleFx_BlastSheet:
	.incbin "baserom.gba", 0x0040531c, 0x0000166c
	.section .unidentified.08408926,"a"
	.incbin "baserom.gba", 0x00408926, 0x00000002
	.section .unidentified.084090c9,"a"
	.incbin "baserom.gba", 0x004090c9, 0x00000003
	.section .unidentified.0840a0e0,"a"
	.global BattleFx_BeamSequenceImage
BattleFx_BeamSequenceImage:
	.incbin "baserom.gba", 0x0040a0e0, 0x0000037c
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "baserom.gba", 0x0040a45c, 0x00000430
	.global BattleFx_WindStreakSheet
BattleFx_WindStreakSheet:
	.incbin "baserom.gba", 0x0040a88c, 0x000010cc
	.global BattleFx_YellowPaletteB
BattleFx_YellowPaletteB:
	.incbin "baserom.gba", 0x0040b958, 0x00000084
	.global BattleFx_BlueRingSheet
BattleFx_BlueRingSheet:
	.incbin "baserom.gba", 0x0040b9dc, 0x000004e8
	.global BattleFx_RedPaletteC
BattleFx_RedPaletteC:
	.incbin "baserom.gba", 0x0040bec4, 0x00000084
	.global BattleFx_VioletPaletteD
BattleFx_VioletPaletteD:
	.incbin "baserom.gba", 0x0040bf48, 0x00000084
	.global BattleFx_BlueBeamSheet
BattleFx_BlueBeamSheet:
	.incbin "baserom.gba", 0x0040bfcc, 0x000006b8
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "baserom.gba", 0x0040c684, 0x00000914
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "baserom.gba", 0x0040cf98, 0x00001b34
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "baserom.gba", 0x0040eacc, 0x00001050
	.section .unidentified.084101cf,"a"
	.incbin "baserom.gba", 0x004101cf, 0x00000001
	.section .unidentified.08411275,"a"
	.incbin "baserom.gba", 0x00411275, 0x00000003
	.global BattleFx_StagedParticleData
BattleFx_StagedParticleData:
	.incbin "baserom.gba", 0x00411278, 0x000001a8
	.global BattleFx_DualStreamData
BattleFx_DualStreamData:
	.incbin "baserom.gba", 0x00411420, 0x00000054
	.global Field_PerspectiveDataA
Field_PerspectiveDataA:
	.incbin "baserom.gba", 0x00411474, 0x0000f828
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "baserom.gba", 0x00420c9c, 0x000039ec
	.global Field_PerspectiveDataB
Field_PerspectiveDataB:
	.incbin "baserom.gba", 0x00424688, 0x00000154
	.global Field_PerspectiveDataC
Field_PerspectiveDataC:
	.incbin "baserom.gba", 0x004247dc, 0x00000490
	.global Graphics_DataA
Graphics_DataA:
	.incbin "baserom.gba", 0x00424c6c, 0x000001f8
	.global Graphics_TilesA
Graphics_TilesA:
	.incbin "baserom.gba", 0x00424e64, 0x000016bc
	.global Graphics_TilesB
Graphics_TilesB:
	.incbin "baserom.gba", 0x00426520, 0x00001520
	.global Graphics_TilesC
Graphics_TilesC:
	.incbin "baserom.gba", 0x00427a40, 0x00000bcc
	.global Graphics_TilesD
Graphics_TilesD:
	.incbin "baserom.gba", 0x0042860c, 0x00000b74
	.global Graphics_TilesE
Graphics_TilesE:
	.incbin "baserom.gba", 0x00429180, 0x00001ea8
	.global Graphics_DataB
Graphics_DataB:
	.incbin "baserom.gba", 0x0042b028, 0x000001f0
	.global Graphics_TilesF
Graphics_TilesF:
	.incbin "baserom.gba", 0x0042b218, 0x000015a0
	.global Graphics_TilesG
Graphics_TilesG:
	.incbin "baserom.gba", 0x0042c7b8, 0x000014b0
	.global Graphics_TilesH
Graphics_TilesH:
	.incbin "baserom.gba", 0x0042dc68, 0x00000908
	.global Graphics_TilesI
Graphics_TilesI:
	.incbin "baserom.gba", 0x0042e570, 0x00000904
	.global Graphics_TilesJ
Graphics_TilesJ:
	.incbin "baserom.gba", 0x0042ee74, 0x00001540
	.global Graphics_PictureA
Graphics_PictureA:
	.incbin "baserom.gba", 0x004303b4, 0x0000189c
	.global Graphics_PictureB
Graphics_PictureB:
	.incbin "baserom.gba", 0x00431c50, 0x000015c8
	.global Graphics_PictureC
Graphics_PictureC:
	.incbin "baserom.gba", 0x00433218, 0x00001a90
	.global Graphics_TilesK
Graphics_TilesK:
	.incbin "baserom.gba", 0x00434ca8, 0x00001250
	.global Shop_CursorFrameA
Shop_CursorFrameA:
	.incbin "baserom.gba", 0x00435ef8, 0x00000100
	.global Shop_CursorFrameB
Shop_CursorFrameB:
	.incbin "baserom.gba", 0x00435ff8, 0x00000100
	.global Shop_CursorFrameC
Shop_CursorFrameC:
	.incbin "baserom.gba", 0x004360f8, 0x00000100
	.global Shop_CursorFrameD
Shop_CursorFrameD:
	.incbin "baserom.gba", 0x004361f8, 0x00000100
	.global Shop_CursorFrameE
Shop_CursorFrameE:
	.incbin "baserom.gba", 0x004362f8, 0x00000100
	.global Shop_CursorFrameF
Shop_CursorFrameF:
	.incbin "baserom.gba", 0x004363f8, 0x00000100
	.global Shop_CursorFrameG
Shop_CursorFrameG:
	.incbin "baserom.gba", 0x004364f8, 0x00000100
	.global Shop_CursorFrameH
Shop_CursorFrameH:
	.incbin "baserom.gba", 0x004365f8, 0x00000100
	.global Ui_Icons
Ui_Icons:
	.incbin "baserom.gba", 0x004366f8, 0x0000592c
	.global Ui_CommandIcons
Ui_CommandIcons:
	.incbin "baserom.gba", 0x0043c024, 0x00003844
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x0043f868, 0x000021b4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x00441a1c, 0x000018f4
	.global Resource_Data0F4
Resource_Data0F4:
	.incbin "baserom.gba", 0x00443310, 0x00002874
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x00445b84, 0x00003224
	.global Resource_Data0F6
Resource_Data0F6:
	.incbin "baserom.gba", 0x00448da8, 0x00001704
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x0044a4ac, 0x00002c9c
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x0044d148, 0x0000193c
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x0044ea84, 0x00002610
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x00451094, 0x00004b04
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x00455b98, 0x00003e80
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x00459a18, 0x00004490
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x0045dea8, 0x0000364c
	.global Resource_Data0FE
Resource_Data0FE:
	.incbin "baserom.gba", 0x004614f4, 0x00003094
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x00464588, 0x00003410
	.global Resource_Data100
Resource_Data100:
	.incbin "baserom.gba", 0x00467998, 0x0000286c
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x0046a204, 0x00000ca8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x0046aeac, 0x00000e48
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x0046bcf4, 0x00000e18
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x0046cb0c, 0x00000cec
	.global Resource_Data105
Resource_Data105:
	.incbin "baserom.gba", 0x0046d7f8, 0x00002b48
	.global Resource_Data106
Resource_Data106:
	.incbin "baserom.gba", 0x00470340, 0x00002324
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x00472664, 0x000035f4
	.global Resource_Data108
Resource_Data108:
	.incbin "baserom.gba", 0x00475c58, 0x0000105c
	.global Resource_Data109
Resource_Data109:
	.incbin "baserom.gba", 0x00476cb4, 0x00000d0c
	.global Resource_Data10A
Resource_Data10A:
	.incbin "baserom.gba", 0x004779c0, 0x00000a84
	.global Resource_Data10B
Resource_Data10B:
	.incbin "baserom.gba", 0x00478444, 0x00000eb0
	.global Resource_Data10C
Resource_Data10C:
	.incbin "baserom.gba", 0x004792f4, 0x000007e0
	.global Resource_Data10D
Resource_Data10D:
	.incbin "baserom.gba", 0x00479ad4, 0x00000c70
	.global Resource_Data10E
Resource_Data10E:
	.incbin "baserom.gba", 0x0047a744, 0x00000c18
	.global Resource_Data10F
Resource_Data10F:
	.incbin "baserom.gba", 0x0047b35c, 0x00000774
	.global Resource_Data110
Resource_Data110:
	.incbin "baserom.gba", 0x0047bad0, 0x00000c18
	.global Resource_Data111
Resource_Data111:
	.incbin "baserom.gba", 0x0047c6e8, 0x00000c78
	.global Resource_Data112
Resource_Data112:
	.incbin "baserom.gba", 0x0047d360, 0x000009d0
	.global Resource_Data113
Resource_Data113:
	.incbin "baserom.gba", 0x0047dd30, 0x00000c1c
	.global Resource_Data114
Resource_Data114:
	.incbin "baserom.gba", 0x0047e94c, 0x000026b4
	.global Resource_Data115
Resource_Data115:
	.incbin "baserom.gba", 0x00481000, 0x00002714
	.global Resource_Data116
Resource_Data116:
	.incbin "baserom.gba", 0x00483714, 0x00004f54
	.global Resource_Data117
Resource_Data117:
	.incbin "baserom.gba", 0x00488668, 0x000018f0
	.global Resource_Data118
Resource_Data118:
	.incbin "baserom.gba", 0x00489f58, 0x00002a3c
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x0048c994, 0x00000bec
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x0048d580, 0x00004bb4
	.global Resource_Data11B
Resource_Data11B:
	.incbin "baserom.gba", 0x00492134, 0x0000236c
	.global Resource_Data11C
Resource_Data11C:
	.incbin "baserom.gba", 0x004944a0, 0x000019a0
	.global Resource_Data11D
Resource_Data11D:
	.incbin "baserom.gba", 0x00495e40, 0x00003bfc
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x00499a3c, 0x00000cc8
	.global Resource_Data11F
Resource_Data11F:
	.incbin "baserom.gba", 0x0049a704, 0x000040c8
	.global Resource_Data120
Resource_Data120:
	.incbin "baserom.gba", 0x0049e7cc, 0x000036c8
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x004a1e94, 0x00004228
	.global Resource_Data122
Resource_Data122:
	.incbin "baserom.gba", 0x004a60bc, 0x00003da8
	.global Resource_Data123
Resource_Data123:
	.incbin "baserom.gba", 0x004a9e64, 0x00002b1c
	.global Resource_Data124
Resource_Data124:
	.incbin "baserom.gba", 0x004ac980, 0x000047f4
	.global Resource_Data125
Resource_Data125:
	.incbin "baserom.gba", 0x004b1174, 0x00004f80
	.global Resource_Data126
Resource_Data126:
	.incbin "baserom.gba", 0x004b60f4, 0x0000279c
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x004b8890, 0x00001ec8
	.global Resource_Data128
Resource_Data128:
	.incbin "baserom.gba", 0x004ba758, 0x0000034c
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
	.global Resource_Data12E
Resource_Data12E:
	.incbin "baserom.gba", 0x004bafc0, 0x00005410
	.section .unidentified.084c055d,"a"
	.incbin "baserom.gba", 0x004c055d, 0x00000003
	.section .unidentified.084c55bd,"a"
	.incbin "baserom.gba", 0x004c55bd, 0x00000003
	.section .unidentified.084c9b29,"a"
	.incbin "baserom.gba", 0x004c9b29, 0x00000003
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x004c9b2c, 0x000051c8
	.section .unidentified.084cee87,"a"
	.incbin "baserom.gba", 0x004cee87, 0x00000001
	.section .unidentified.084d1adb,"a"
	.incbin "baserom.gba", 0x004d1adb, 0x00000001
	.section .unidentified.084d878f,"a"
	.incbin "baserom.gba", 0x004d878f, 0x00000001
	.global Resource_Data13A
Resource_Data13A:
	.incbin "baserom.gba", 0x004d8790, 0x0000313c
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
	.global Resource_Data140
Resource_Data140:
	.incbin "baserom.gba", 0x004e3fa8, 0x00002154
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
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x004ee110, 0x00003114
	.section .unidentified.084f5389,"a"
	.incbin "baserom.gba", 0x004f5389, 0x00000003
	.section .unidentified.084f6986,"a"
	.incbin "baserom.gba", 0x004f6986, 0x00000002
	.section .unidentified.084f7c4f,"a"
	.incbin "baserom.gba", 0x004f7c4f, 0x00000001
	.global Resource_Data14C
Resource_Data14C:
	.incbin "baserom.gba", 0x004f7c50, 0x00003418
	.section .unidentified.084fb1f5,"a"
	.incbin "baserom.gba", 0x004fb1f5, 0x00000003
	.section .unidentified.084fcfa6,"a"
	.incbin "baserom.gba", 0x004fcfa6, 0x00000002
	.section .unidentified.08500b1d,"a"
	.incbin "baserom.gba", 0x00500b1d, 0x00000003
	.section .unidentified.0850202b,"a"
	.incbin "baserom.gba", 0x0050202b, 0x00000001
	.global Resource_Data152
Resource_Data152:
	.incbin "baserom.gba", 0x0050202c, 0x00000bd4
	.section .unidentified.08502cfd,"a"
	.incbin "baserom.gba", 0x00502cfd, 0x00000003
	.section .unidentified.085040e2,"a"
	.incbin "baserom.gba", 0x005040e2, 0x00000002
	.section .unidentified.08505265,"a"
	.incbin "baserom.gba", 0x00505265, 0x00000003
	.section .unidentified.08505bee,"a"
	.incbin "baserom.gba", 0x00505bee, 0x00000002
	.section .unidentified.08506444,"a"
	.global Resource_Data158
Resource_Data158:
	.incbin "baserom.gba", 0x00506444, 0x00002428
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
	.global Resource_Data15E
Resource_Data15E:
	.incbin "baserom.gba", 0x0050be10, 0x0000121c
	.section .unidentified.0850d185,"a"
	.incbin "baserom.gba", 0x0050d185, 0x00000003
	.section .unidentified.085115ab,"a"
	.incbin "baserom.gba", 0x005115ab, 0x00000001
	.section .unidentified.085132da,"a"
	.incbin "baserom.gba", 0x005132da, 0x00000002
	.global Resource_Data164
Resource_Data164:
	.incbin "baserom.gba", 0x005132dc, 0x00000d78
	.section .unidentified.08514187,"a"
	.incbin "baserom.gba", 0x00514187, 0x00000001
	.section .unidentified.08516607,"a"
	.incbin "baserom.gba", 0x00516607, 0x00000001
	.section .unidentified.08517383,"a"
	.incbin "baserom.gba", 0x00517383, 0x00000001
	.section .unidentified.08517e62,"a"
	.incbin "baserom.gba", 0x00517e62, 0x00000002
	.global Resource_Data169
Resource_Data169:
	.incbin "baserom.gba", 0x00517e64, 0x00000e78
	.section .unidentified.0851adae,"a"
	.incbin "baserom.gba", 0x0051adae, 0x00000002
	.section .unidentified.0851b6f5,"a"
	.incbin "baserom.gba", 0x0051b6f5, 0x00000003
	.section .unidentified.0851c342,"a"
	.incbin "baserom.gba", 0x0051c342, 0x00000002
	.section .unidentified.0851c569,"a"
	.incbin "baserom.gba", 0x0051c569, 0x00000003
	.global Resource_Data16F
Resource_Data16F:
	.incbin "baserom.gba", 0x0051c56c, 0x00000d28
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x0051d294, 0x00000c18
	.section .unidentified.0851dfbf,"a"
	.incbin "baserom.gba", 0x0051dfbf, 0x00000001
	.section .unidentified.0851f985,"a"
	.incbin "baserom.gba", 0x0051f985, 0x00000003
	.section .unidentified.0851facf,"a"
	.incbin "baserom.gba", 0x0051facf, 0x00000001
	.section .unidentified.085200f0,"a"
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x005200f0, 0x00001aac
	.section .unidentified.08521cc3,"a"
	.incbin "baserom.gba", 0x00521cc3, 0x00000001
	.section .unidentified.0852354b,"a"
	.incbin "baserom.gba", 0x0052354b, 0x00000001
	.section .unidentified.085257ed,"a"
	.incbin "baserom.gba", 0x005257ed, 0x00000003
	.section .unidentified.0852592f,"a"
	.incbin "baserom.gba", 0x0052592f, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x00525930, 0x00002008
	.section .unidentified.08527a42,"a"
	.incbin "baserom.gba", 0x00527a42, 0x00000002
	.section .unidentified.08529a26,"a"
	.incbin "baserom.gba", 0x00529a26, 0x00000002
	.section .unidentified.0852b14b,"a"
	.incbin "baserom.gba", 0x0052b14b, 0x00000001
	.section .unidentified.0852c1ff,"a"
	.incbin "baserom.gba", 0x0052c1ff, 0x00000001
	.global Resource_Data182
Resource_Data182:
	.incbin "baserom.gba", 0x0052c200, 0x00001ce8
	.section .unidentified.0852e041,"a"
	.incbin "baserom.gba", 0x0052e041, 0x00000003
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x0052e044, 0x00001f48
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
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x005392d4, 0x00002700
	.section .unidentified.0853d79d,"a"
	.incbin "baserom.gba", 0x0053d79d, 0x00000003
	.section .unidentified.0853fce7,"a"
	.incbin "baserom.gba", 0x0053fce7, 0x00000001
	.section .unidentified.08540fb2,"a"
	.incbin "baserom.gba", 0x00540fb2, 0x00000002
	.section .unidentified.08541c63,"a"
	.incbin "baserom.gba", 0x00541c63, 0x00000001
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x00541c64, 0x00000a6c
	.section .unidentified.085427c3,"a"
	.incbin "baserom.gba", 0x005427c3, 0x00000001
	.section .unidentified.08544222,"a"
	.incbin "baserom.gba", 0x00544222, 0x00000002
	.section .unidentified.08544fc6,"a"
	.incbin "baserom.gba", 0x00544fc6, 0x00000002
	.global Resource_Data196
Resource_Data196:
	.incbin "baserom.gba", 0x00544fc8, 0x00001b38
	.section .unidentified.085499ed,"a"
	.incbin "baserom.gba", 0x005499ed, 0x00000003
	.section .unidentified.085500e0,"a"
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x005500e0, 0x000010f4
	.section .unidentified.0855129f,"a"
	.incbin "baserom.gba", 0x0055129f, 0x00000001
	.section .unidentified.08556b4a,"a"
	.incbin "baserom.gba", 0x00556b4a, 0x00000002
	.section .unidentified.08556c8b,"a"
	.incbin "baserom.gba", 0x00556c8b, 0x00000001
	.global Resource_Data1A2
Resource_Data1A2:
	.incbin "baserom.gba", 0x00556c8c, 0x00001430
	.section .unidentified.0855adf6,"a"
	.incbin "baserom.gba", 0x0055adf6, 0x00000002
	.section .unidentified.0855cef2,"a"
	.incbin "baserom.gba", 0x0055cef2, 0x00000002
	.section .unidentified.0855f2ac,"a"
	.global Resource_Data1A8
Resource_Data1A8:
	.incbin "baserom.gba", 0x0055f2ac, 0x00001d24
	.global Resource_Data1A9
Resource_Data1A9:
	.incbin "baserom.gba", 0x00560fd0, 0x00000f44
	.global Resource_Data1AA
Resource_Data1AA:
	.incbin "baserom.gba", 0x00561f14, 0x000029e8
	.section .unidentified.08564a96,"a"
	.incbin "baserom.gba", 0x00564a96, 0x00000002
	.section .unidentified.08569331,"a"
	.incbin "baserom.gba", 0x00569331, 0x00000003
	.section .unidentified.0856b832,"a"
	.incbin "baserom.gba", 0x0056b832, 0x00000002
	.global Resource_Data1B0
Resource_Data1B0:
	.incbin "baserom.gba", 0x0056b834, 0x00002f40
	.global Resource_Data1B1
Resource_Data1B1:
	.incbin "baserom.gba", 0x0056e774, 0x00002dcc
	.global Resource_Data1B2
Resource_Data1B2:
	.incbin "baserom.gba", 0x00571540, 0x00001ce4
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
	.global Resource_Data1B8
Resource_Data1B8:
	.incbin "baserom.gba", 0x0057bd88, 0x000017ac
	.section .unidentified.0858020d,"a"
	.incbin "baserom.gba", 0x0058020d, 0x00000003
	.section .unidentified.08581fdf,"a"
	.incbin "baserom.gba", 0x00581fdf, 0x00000001
	.section .unidentified.0858211f,"a"
	.incbin "baserom.gba", 0x0058211f, 0x00000001
	.global Resource_Data1BD
Resource_Data1BD:
	.incbin "baserom.gba", 0x00582120, 0x00001304
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00583424, 0x00001d58
	.section .unidentified.085880f6,"a"
	.incbin "baserom.gba", 0x005880f6, 0x00000002
	.section .unidentified.0858a9bd,"a"
	.incbin "baserom.gba", 0x0058a9bd, 0x00000003
	.section .unidentified.0858eeb4,"a"
	.global Resource_Data1C4
Resource_Data1C4:
	.incbin "baserom.gba", 0x0058eeb4, 0x00002a74
	.section .unidentified.085931cb,"a"
	.incbin "baserom.gba", 0x005931cb, 0x00000001
	.section .unidentified.08593aca,"a"
	.incbin "baserom.gba", 0x00593aca, 0x00000002
	.global Resource_Data1C8
Resource_Data1C8:
	.incbin "baserom.gba", 0x00593acc, 0x00000d78
	.section .unidentified.085949a3,"a"
	.incbin "baserom.gba", 0x005949a3, 0x00000001
	.section .unidentified.085965fb,"a"
	.incbin "baserom.gba", 0x005965fb, 0x00000001
	.global Resource_Data1CB
Resource_Data1CB:
	.incbin "baserom.gba", 0x005965fc, 0x0000189c
	.section .unidentified.0859801b,"a"
	.incbin "baserom.gba", 0x0059801b, 0x00000001
	.section .unidentified.0859ab57,"a"
	.incbin "baserom.gba", 0x0059ab57, 0x00000001
	.section .unidentified.0859d2d1,"a"
	.incbin "baserom.gba", 0x0059d2d1, 0x00000003
	.section .unidentified.0859e2be,"a"
	.incbin "baserom.gba", 0x0059e2be, 0x00000002
	.global Resource_Data1D0
Resource_Data1D0:
	.incbin "baserom.gba", 0x0059e2c0, 0x000016cc
	.section .unidentified.0859fb35,"a"
	.incbin "baserom.gba", 0x0059fb35, 0x00000003
	.section .unidentified.085a28c9,"a"
	.incbin "baserom.gba", 0x005a28c9, 0x00000003
	.section .unidentified.085a695e,"a"
	.incbin "baserom.gba", 0x005a695e, 0x00000002
	.section .unidentified.085a6a9f,"a"
	.incbin "baserom.gba", 0x005a6a9f, 0x00000001
	.global Resource_Data1D6
Resource_Data1D6:
	.incbin "baserom.gba", 0x005a6aa0, 0x00001ec8
	.section .unidentified.085a8b09,"a"
	.incbin "baserom.gba", 0x005a8b09, 0x00000003
	.section .unidentified.085aabab,"a"
	.incbin "baserom.gba", 0x005aabab, 0x00000001
	.section .unidentified.085abb38,"a"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005abb38, 0x000022d8
	.section .unidentified.085af064,"a"
	.global Resource_Data1DC
Resource_Data1DC:
	.incbin "baserom.gba", 0x005af064, 0x000018a0
	.section .unidentified.085b0a86,"a"
	.incbin "baserom.gba", 0x005b0a86, 0x00000002
	.section .unidentified.085b81d3,"a"
	.incbin "baserom.gba", 0x005b81d3, 0x00000001
	.section .unidentified.085b86dd,"a"
	.incbin "baserom.gba", 0x005b86dd, 0x00000003
	.global Resource_Data1E2
Resource_Data1E2:
	.incbin "baserom.gba", 0x005b86e0, 0x000016d8
	.section .unidentified.085b9f59,"a"
	.incbin "baserom.gba", 0x005b9f59, 0x00000003
	.section .unidentified.085bb85e,"a"
	.incbin "baserom.gba", 0x005bb85e, 0x00000002
	.global Resource_Data1E5
Resource_Data1E5:
	.incbin "baserom.gba", 0x005bb860, 0x0000182c
	.section .unidentified.085bd209,"a"
	.incbin "baserom.gba", 0x005bd209, 0x00000003
	.section .unidentified.085c1afd,"a"
	.incbin "baserom.gba", 0x005c1afd, 0x00000003
	.section .unidentified.085c2b42,"a"
	.incbin "baserom.gba", 0x005c2b42, 0x00000002
	.section .unidentified.085c3966,"a"
	.incbin "baserom.gba", 0x005c3966, 0x00000002
	.global Resource_Data1EB
Resource_Data1EB:
	.incbin "baserom.gba", 0x005c3968, 0x00001344
	.section .unidentified.085c4e05,"a"
	.incbin "baserom.gba", 0x005c4e05, 0x00000003
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x005c4e08, 0x000025bc
	.section .unidentified.085c753a,"a"
	.incbin "baserom.gba", 0x005c753a, 0x00000002
	.section .unidentified.085cbe2d,"a"
	.incbin "baserom.gba", 0x005cbe2d, 0x00000003
	.section .unidentified.085cc895,"a"
	.incbin "baserom.gba", 0x005cc895, 0x00000003
	.section .unidentified.085cd0d8,"a"
	.global Resource_Data1F3
Resource_Data1F3:
	.incbin "baserom.gba", 0x005cd0d8, 0x00001e00
	.section .unidentified.085cf03e,"a"
	.incbin "baserom.gba", 0x005cf03e, 0x00000002
	.section .unidentified.085d0185,"a"
	.incbin "baserom.gba", 0x005d0185, 0x00000003
	.global Resource_Data1F6
Resource_Data1F6:
	.incbin "baserom.gba", 0x005d0188, 0x00001d5c
	.section .unidentified.085d204a,"a"
	.incbin "baserom.gba", 0x005d204a, 0x00000002
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x005d204c, 0x00002890
	.section .unidentified.085d4a0e,"a"
	.incbin "baserom.gba", 0x005d4a0e, 0x00000002
	.section .unidentified.085d764a,"a"
	.incbin "baserom.gba", 0x005d764a, 0x00000002
	.section .unidentified.085dade9,"a"
	.incbin "baserom.gba", 0x005dade9, 0x00000003
	.section .unidentified.085dc868,"a"
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x005dc868, 0x000018d0
	.section .unidentified.085de2b3,"a"
	.incbin "baserom.gba", 0x005de2b3, 0x00000001
	.section .unidentified.085e0e62,"a"
	.incbin "baserom.gba", 0x005e0e62, 0x00000002
	.section .unidentified.085e30d6,"a"
	.incbin "baserom.gba", 0x005e30d6, 0x00000002
	.section .unidentified.085e5947,"a"
	.incbin "baserom.gba", 0x005e5947, 0x00000001
	.section .unidentified.085e6820,"a"
	.global Resource_Data204
Resource_Data204:
	.incbin "baserom.gba", 0x005e6820, 0x0000248c
	.section .unidentified.085e8e3b,"a"
	.incbin "baserom.gba", 0x005e8e3b, 0x00000001
	.section .unidentified.085eda62,"a"
	.incbin "baserom.gba", 0x005eda62, 0x00000002
	.section .unidentified.085ee6e5,"a"
	.incbin "baserom.gba", 0x005ee6e5, 0x00000003
	.section .unidentified.085ee827,"a"
	.incbin "baserom.gba", 0x005ee827, 0x00000001
	.global Resource_Data20A
Resource_Data20A:
	.incbin "baserom.gba", 0x005ee828, 0x00001570
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
	.global Resource_Data210
Resource_Data210:
	.incbin "baserom.gba", 0x005f46e0, 0x00001a18
	.section .unidentified.085f7672,"a"
	.incbin "baserom.gba", 0x005f7672, 0x00000002
	.global Resource_Data213
Resource_Data213:
	.incbin "baserom.gba", 0x005f7674, 0x0000153c
	.section .unidentified.085f8d3e,"a"
	.incbin "baserom.gba", 0x005f8d3e, 0x00000002
	.section .unidentified.085faff7,"a"
	.incbin "baserom.gba", 0x005faff7, 0x00000001
	.global Resource_Data216
Resource_Data216:
	.incbin "baserom.gba", 0x005faff8, 0x00001758
	.section .unidentified.085ff23e,"a"
	.incbin "baserom.gba", 0x005ff23e, 0x00000002
	.global Resource_Data219
Resource_Data219:
	.incbin "baserom.gba", 0x005ff240, 0x00001c98
	.section .unidentified.0860101d,"a"
	.incbin "baserom.gba", 0x0060101d, 0x00000003
	.section .unidentified.086041cb,"a"
	.incbin "baserom.gba", 0x006041cb, 0x00000001
	.section .unidentified.086067ff,"a"
	.incbin "baserom.gba", 0x006067ff, 0x00000001
	.section .unidentified.08606a9c,"a"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x00606a9c, 0x000028bc
	.section .unidentified.0860951f,"a"
	.incbin "baserom.gba", 0x0060951f, 0x00000001
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.section .unidentified.0860f929,"a"
	.incbin "baserom.gba", 0x0060f929, 0x00000003
	.section .unidentified.08610e9a,"a"
	.incbin "baserom.gba", 0x00610e9a, 0x00000002
	.global Resource_Data225
Resource_Data225:
	.incbin "baserom.gba", 0x00610e9c, 0x00000e18
	.global Resource_Data226
Resource_Data226:
	.incbin "baserom.gba", 0x00611cb4, 0x00001a10
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
	.global Resource_Data22C
Resource_Data22C:
	.incbin "baserom.gba", 0x0061b4fc, 0x00001b0c
	.global Resource_Data22D
Resource_Data22D:
	.incbin "baserom.gba", 0x0061d008, 0x00001e40
	.global Resource_Data22E
Resource_Data22E:
	.incbin "baserom.gba", 0x0061ee48, 0x00001b28
	.global Resource_Data22F
Resource_Data22F:
	.incbin "baserom.gba", 0x00620970, 0x00002178
	.global Resource_Data230
Resource_Data230:
	.incbin "baserom.gba", 0x00622ae8, 0x00000f28
	.global Resource_Data231
Resource_Data231:
	.incbin "baserom.gba", 0x00623a10, 0x00002f80
	.section .unidentified.08626b4e,"a"
	.incbin "baserom.gba", 0x00626b4e, 0x00000002
	.section .unidentified.08628d08,"a"
	.global Resource_Data234
Resource_Data234:
	.incbin "baserom.gba", 0x00628d08, 0x00000fd4
	.section .unidentified.08629eb5,"a"
	.incbin "baserom.gba", 0x00629eb5, 0x00000003
	.section .unidentified.0862c3a6,"a"
	.incbin "baserom.gba", 0x0062c3a6, 0x00000002
	.section .unidentified.0862f187,"a"
	.incbin "baserom.gba", 0x0062f187, 0x00000001
	.section .unidentified.0862fd21,"a"
	.incbin "baserom.gba", 0x0062fd21, 0x00000003
	.global Resource_Data23A
Resource_Data23A:
	.incbin "baserom.gba", 0x0062fd24, 0x00000e60
	.section .unidentified.08630cda,"a"
	.incbin "baserom.gba", 0x00630cda, 0x00000002
	.global Resource_Data23C
Resource_Data23C:
	.incbin "baserom.gba", 0x00630cdc, 0x00000c64
	.global Resource_Data23D
Resource_Data23D:
	.incbin "baserom.gba", 0x00631940, 0x00000dd0
	.global Resource_Data23E
Resource_Data23E:
	.incbin "baserom.gba", 0x00632710, 0x00001290
	.global Resource_Data23F
Resource_Data23F:
	.incbin "baserom.gba", 0x006339a0, 0x00000ee8
	.global Resource_Data240
Resource_Data240:
	.incbin "baserom.gba", 0x00634888, 0x000010d8
	.global Resource_Data241
Resource_Data241:
	.incbin "baserom.gba", 0x00635960, 0x000031c8
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
	.global Resource_Data247
Resource_Data247:
	.incbin "baserom.gba", 0x0063eedc, 0x000035d4
	.global Resource_Data248
Resource_Data248:
	.incbin "baserom.gba", 0x006424b0, 0x00003b08
	.global Resource_Data249
Resource_Data249:
	.incbin "baserom.gba", 0x00645fb8, 0x00003d10
	.global Resource_Data24A
Resource_Data24A:
	.incbin "baserom.gba", 0x00649cc8, 0x00004060
	.global Resource_Data24B
Resource_Data24B:
	.incbin "baserom.gba", 0x0064dd28, 0x00002288
	.section .unidentified.08650142,"a"
	.incbin "baserom.gba", 0x00650142, 0x00000002
	.section .unidentified.08651652,"a"
	.incbin "baserom.gba", 0x00651652, 0x00000002
	.section .unidentified.08653437,"a"
	.incbin "baserom.gba", 0x00653437, 0x00000001
	.section .unidentified.086544ca,"a"
	.incbin "baserom.gba", 0x006544ca, 0x00000002
	.section .unidentified.08654a20,"a"
	.global Resource_Data251
Resource_Data251:
	.incbin "baserom.gba", 0x00654a20, 0x00001d1c
	.section .unidentified.086568d1,"a"
	.incbin "baserom.gba", 0x006568d1, 0x00000003
	.global Resource_Data253
Resource_Data253:
	.incbin "baserom.gba", 0x006568d4, 0x00001fa4
	.section .unidentified.0865d64e,"a"
	.incbin "baserom.gba", 0x0065d64e, 0x00000002
	.section .unidentified.086610af,"a"
	.incbin "baserom.gba", 0x006610af, 0x00000001
	.global Resource_Data259
Resource_Data259:
	.incbin "baserom.gba", 0x006610b0, 0x00002b6c
	.section .unidentified.08663de5,"a"
	.incbin "baserom.gba", 0x00663de5, 0x00000003
	.section .unidentified.0866612d,"a"
	.incbin "baserom.gba", 0x0066612d, 0x00000003
	.global Resource_Data25C
Resource_Data25C:
	.incbin "baserom.gba", 0x00666130, 0x000017dc
	.section .unidentified.08667abd,"a"
	.incbin "baserom.gba", 0x00667abd, 0x00000003
	.global Resource_Data25E
Resource_Data25E:
	.incbin "baserom.gba", 0x00667ac0, 0x00001574
	.global Resource_Data25F
Resource_Data25F:
	.incbin "baserom.gba", 0x00669034, 0x000029d0
	.global Resource_Data260
Resource_Data260:
	.incbin "baserom.gba", 0x0066ba04, 0x000022c8
	.global Resource_Data261
Resource_Data261:
	.incbin "baserom.gba", 0x0066dccc, 0x000015d8
	.global Resource_Data262
Resource_Data262:
	.incbin "baserom.gba", 0x0066f2a4, 0x000020c0
	.global Resource_Data263
Resource_Data263:
	.incbin "baserom.gba", 0x00671364, 0x0000232c
	.global Resource_Data264
Resource_Data264:
	.incbin "baserom.gba", 0x00673690, 0x00002c24
	.global Resource_Data265
Resource_Data265:
	.incbin "baserom.gba", 0x006762b4, 0x00001488
	.global Resource_Data266
Resource_Data266:
	.incbin "baserom.gba", 0x0067773c, 0x00000d24
	.section .unidentified.086785bd,"a"
	.incbin "baserom.gba", 0x006785bd, 0x00000003
	.global Resource_Data268
Resource_Data268:
	.incbin "baserom.gba", 0x006785c0, 0x0000193c
	.global Resource_Data269
Resource_Data269:
	.incbin "baserom.gba", 0x00679efc, 0x00001e1c
	.section .unidentified.0867be7a,"a"
	.incbin "baserom.gba", 0x0067be7a, 0x00000002
	.global Resource_Data26B
Resource_Data26B:
	.incbin "baserom.gba", 0x0067be7c, 0x000034c4
	.global Resource_Data26C
Resource_Data26C:
	.incbin "baserom.gba", 0x0067f340, 0x00002820
	.section .unidentified.08684543,"a"
	.incbin "baserom.gba", 0x00684543, 0x00000001
	.section .unidentified.0868674e,"a"
	.incbin "baserom.gba", 0x0068674e, 0x00000002
	.section .unidentified.0868688f,"a"
	.incbin "baserom.gba", 0x0068688f, 0x00000001
	.section .unidentified.086882a6,"a"
	.incbin "baserom.gba", 0x006882a6, 0x00000002
	.global Resource_Data272
Resource_Data272:
	.incbin "baserom.gba", 0x006882a8, 0x000024e0
	.global Resource_Data273
Resource_Data273:
	.incbin "baserom.gba", 0x0068a788, 0x000028e8
	.global Resource_Data274
Resource_Data274:
	.incbin "baserom.gba", 0x0068d070, 0x00001fa0
	.section .unidentified.0868f18b,"a"
	.incbin "baserom.gba", 0x0068f18b, 0x00000001
	.section .unidentified.08691135,"a"
	.incbin "baserom.gba", 0x00691135, 0x00000003
	.section .unidentified.086938ae,"a"
	.incbin "baserom.gba", 0x006938ae, 0x00000002
	.section .unidentified.08695115,"a"
	.incbin "baserom.gba", 0x00695115, 0x00000003
	.section .unidentified.08695d1c,"a"
	.global Resource_Data27A
Resource_Data27A:
	.incbin "baserom.gba", 0x00695d1c, 0x00002e1c
	.section .unidentified.08698cff,"a"
	.incbin "baserom.gba", 0x00698cff, 0x00000001
	.section .unidentified.0869ffb0,"a"
	.global Resource_Data280
Resource_Data280:
	.incbin "baserom.gba", 0x0069ffb0, 0x00000dfc
	.global Resource_Data281
Resource_Data281:
	.incbin "baserom.gba", 0x006a0dac, 0x00001178
	.global Resource_Data282
Resource_Data282:
	.incbin "baserom.gba", 0x006a1f24, 0x00001dc4
	.section .unidentified.086a3e6c,"a"
	.global Resource_Data284
Resource_Data284:
	.incbin "baserom.gba", 0x006a3e6c, 0x00001fc8
	.section .unidentified.086a5fbd,"a"
	.incbin "baserom.gba", 0x006a5fbd, 0x00000003
	.section .unidentified.086a7a42,"a"
	.incbin "baserom.gba", 0x006a7a42, 0x00000002
	.section .unidentified.086a83c0,"a"
	.global Resource_Data288
Resource_Data288:
	.incbin "baserom.gba", 0x006a83c0, 0x000026bc
	.section .unidentified.086aac3f,"a"
	.incbin "baserom.gba", 0x006aac3f, 0x00000001
	.global Resource_Data28A
Resource_Data28A:
	.incbin "baserom.gba", 0x006aac40, 0x00000f34
	.section .unidentified.086abcc0,"a"
	.global Resource_Data28C
Resource_Data28C:
	.incbin "baserom.gba", 0x006abcc0, 0x00001f68
	.section .unidentified.086addcd,"a"
	.incbin "baserom.gba", 0x006addcd, 0x00000003
	.section .unidentified.086b2df7,"a"
	.incbin "baserom.gba", 0x006b2df7, 0x00000001
	.section .unidentified.086b4d80,"a"
	.global Resource_Data291
Resource_Data291:
	.incbin "baserom.gba", 0x006b4d80, 0x00000e08
	.section .unidentified.086b5d47,"a"
	.incbin "baserom.gba", 0x006b5d47, 0x00000001
	.section .unidentified.086b70a0,"a"
	.global Resource_Data294
Resource_Data294:
	.incbin "baserom.gba", 0x006b70a0, 0x0000119c
	.section .unidentified.086b840d,"a"
	.incbin "baserom.gba", 0x006b840d, 0x00000003
	.section .unidentified.086ba49e,"a"
	.incbin "baserom.gba", 0x006ba49e, 0x00000002
	.section .unidentified.086bc59e,"a"
	.incbin "baserom.gba", 0x006bc59e, 0x00000002
	.section .unidentified.086bf152,"a"
	.incbin "baserom.gba", 0x006bf152, 0x00000002
	.global Resource_Data29A
Resource_Data29A:
	.incbin "baserom.gba", 0x006bf154, 0x00000aec
	.section .unidentified.086bfe12,"a"
	.incbin "baserom.gba", 0x006bfe12, 0x00000002
	.global Resource_Data29C
Resource_Data29C:
	.incbin "baserom.gba", 0x006bfe14, 0x00001358
	.section .unidentified.086c2741,"a"
	.incbin "baserom.gba", 0x006c2741, 0x00000003
	.section .unidentified.086c3ed9,"a"
	.incbin "baserom.gba", 0x006c3ed9, 0x00000003
	.section .unidentified.086c514d,"a"
	.incbin "baserom.gba", 0x006c514d, 0x00000003
	.section .unidentified.086c56b8,"a"
	.global Resource_Data2A2
Resource_Data2A2:
	.incbin "baserom.gba", 0x006c56b8, 0x00001d80
	.section .unidentified.086c75dd,"a"
	.incbin "baserom.gba", 0x006c75dd, 0x00000003
	.global Resource_Data2A4
Resource_Data2A4:
	.incbin "baserom.gba", 0x006c75e0, 0x000021b8
	.section .unidentified.086c9929,"a"
	.incbin "baserom.gba", 0x006c9929, 0x00000003
	.global Resource_Data2A6
Resource_Data2A6:
	.incbin "baserom.gba", 0x006c992c, 0x000011e8
	.section .unidentified.086cac9a,"a"
	.incbin "baserom.gba", 0x006cac9a, 0x00000002
	.section .unidentified.086cf1e6,"a"
	.incbin "baserom.gba", 0x006cf1e6, 0x00000002
	.section .unidentified.086cf33b,"a"
	.incbin "baserom.gba", 0x006cf33b, 0x00000001
	.section .unidentified.086cf47b,"a"
	.incbin "baserom.gba", 0x006cf47b, 0x00000001
	.global Resource_Data2AC
Resource_Data2AC:
	.incbin "baserom.gba", 0x006cf47c, 0x00000cd4
	.section .unidentified.086d02ce,"a"
	.incbin "baserom.gba", 0x006d02ce, 0x00000002
	.global Resource_Data2AE
Resource_Data2AE:
	.incbin "baserom.gba", 0x006d02d0, 0x000018b8
	.section .unidentified.086d1d09,"a"
	.incbin "baserom.gba", 0x006d1d09, 0x00000003
	.global Resource_Data2B0
Resource_Data2B0:
	.incbin "baserom.gba", 0x006d1d0c, 0x00001a90
	.section .unidentified.086d3915,"a"
	.incbin "baserom.gba", 0x006d3915, 0x00000003
	.global Resource_Data2B2
Resource_Data2B2:
	.incbin "baserom.gba", 0x006d3918, 0x00001d3c
	.section .unidentified.086d577f,"a"
	.incbin "baserom.gba", 0x006d577f, 0x00000001
	.section .unidentified.086d58f5,"a"
	.incbin "baserom.gba", 0x006d58f5, 0x00000003
	.global Resource_Data2B5
Resource_Data2B5:
	.incbin "baserom.gba", 0x006d58f8, 0x00001ea4
	.section .unidentified.086d7921,"a"
	.incbin "baserom.gba", 0x006d7921, 0x00000003
	.global Resource_Data2B7
Resource_Data2B7:
	.incbin "baserom.gba", 0x006d7924, 0x00000f5c
	.section .unidentified.086d8a18,"a"
	.global Resource_Data2B9
Resource_Data2B9:
	.incbin "baserom.gba", 0x006d8a18, 0x0000110c
	.section .unidentified.086d9cde,"a"
	.incbin "baserom.gba", 0x006d9cde, 0x00000002
	.global Resource_Data2BB
Resource_Data2BB:
	.incbin "baserom.gba", 0x006d9ce0, 0x00002aa4
	.section .unidentified.086dc8f5,"a"
	.incbin "baserom.gba", 0x006dc8f5, 0x00000003
	.global Resource_Data2BD
Resource_Data2BD:
	.incbin "baserom.gba", 0x006dc8f8, 0x000013e8
	.section .unidentified.086dde98,"a"
	.global Resource_Data2BF
Resource_Data2BF:
	.incbin "baserom.gba", 0x006dde98, 0x00002928
	.section .unidentified.086e0967,"a"
	.incbin "baserom.gba", 0x006e0967, 0x00000001
	.global Resource_Data2C1
Resource_Data2C1:
	.incbin "baserom.gba", 0x006e0968, 0x000023fc
	.section .unidentified.086e2ef1,"a"
	.incbin "baserom.gba", 0x006e2ef1, 0x00000003
	.global Resource_Data2C3
Resource_Data2C3:
	.incbin "baserom.gba", 0x006e2ef4, 0x00003280
	.section .unidentified.086e6352,"a"
	.incbin "baserom.gba", 0x006e6352, 0x00000002
	.global Resource_Data2C5
Resource_Data2C5:
	.incbin "baserom.gba", 0x006e6354, 0x00000f0c
	.global Resource_Data2C6
Resource_Data2C6:
	.incbin "baserom.gba", 0x006e7260, 0x00001688
	.section .unidentified.086e8a76,"a"
	.incbin "baserom.gba", 0x006e8a76, 0x00000002
	.global Resource_Data2C8
Resource_Data2C8:
	.incbin "baserom.gba", 0x006e8a78, 0x00002a70
	.section .unidentified.086eb66e,"a"
	.incbin "baserom.gba", 0x006eb66e, 0x00000002
	.global Resource_Data2CA
Resource_Data2CA:
	.incbin "baserom.gba", 0x006eb670, 0x00000b4c
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
	.global Resource_Data2D0
Resource_Data2D0:
	.incbin "baserom.gba", 0x006f21ac, 0x00001544
	.section .unidentified.086f3879,"a"
	.incbin "baserom.gba", 0x006f3879, 0x00000003
	.global Resource_Data2D2
Resource_Data2D2:
	.incbin "baserom.gba", 0x006f387c, 0x000010dc
	.section .unidentified.086f4ae5,"a"
	.incbin "baserom.gba", 0x006f4ae5, 0x00000003
	.global Resource_Data2D4
Resource_Data2D4:
	.incbin "baserom.gba", 0x006f4ae8, 0x00001b04
	.section .unidentified.086f6739,"a"
	.incbin "baserom.gba", 0x006f6739, 0x00000003
	.section .unidentified.086f911b,"a"
	.incbin "baserom.gba", 0x006f911b, 0x00000001
	.section .unidentified.086fc645,"a"
	.incbin "baserom.gba", 0x006fc645, 0x00000003
	.section .unidentified.086fdf4f,"a"
	.incbin "baserom.gba", 0x006fdf4f, 0x00000001
	.global Resource_Data2DA
Resource_Data2DA:
	.incbin "baserom.gba", 0x006fdf50, 0x000020a8
	.section .unidentified.08702a4b,"a"
	.incbin "baserom.gba", 0x00702a4b, 0x00000001
	.section .unidentified.08704697,"a"
	.incbin "baserom.gba", 0x00704697, 0x00000001
	.section .unidentified.08705add,"a"
	.incbin "baserom.gba", 0x00705add, 0x00000003
	.section .unidentified.0870795d,"a"
	.incbin "baserom.gba", 0x0070795d, 0x00000003
	.global Resource_Data2E0
Resource_Data2E0:
	.incbin "baserom.gba", 0x00707960, 0x00001a94
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
	.global Resource_Data2E6
Resource_Data2E6:
	.incbin "baserom.gba", 0x0071086c, 0x00001230
	.section .unidentified.08711bf5,"a"
	.incbin "baserom.gba", 0x00711bf5, 0x00000003
	.global Resource_Data2E8
Resource_Data2E8:
	.incbin "baserom.gba", 0x00711bf8, 0x00000fa8
	.global Resource_Data2E9
Resource_Data2E9:
	.incbin "baserom.gba", 0x00712ba0, 0x0000167c
	.global Resource_Data2EA
Resource_Data2EA:
	.incbin "baserom.gba", 0x0071421c, 0x00000ed8
	.section .unidentified.0871525e,"a"
	.incbin "baserom.gba", 0x0071525e, 0x00000002
	.global Resource_Data2EC
Resource_Data2EC:
	.incbin "baserom.gba", 0x00715260, 0x000019b8
	.global Resource_Data2ED
Resource_Data2ED:
	.incbin "baserom.gba", 0x00716c18, 0x00000f74
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.section .unidentified.08718e8c,"a"
	.global Resource_Data2F0
Resource_Data2F0:
	.incbin "baserom.gba", 0x00718e8c, 0x0000164c
	.section .unidentified.0871a618,"a"
	.global Resource_Data2F2
Resource_Data2F2:
	.incbin "baserom.gba", 0x0071a618, 0x00001dbc
	.global Resource_Data2F3
Resource_Data2F3:
	.incbin "baserom.gba", 0x0071c3d4, 0x00001348
	.section .unidentified.0871d898,"a"
	.global Resource_Data2F5
Resource_Data2F5:
	.incbin "baserom.gba", 0x0071d898, 0x0000277c
	.section .unidentified.087201ac,"a"
	.global Resource_Data2F7
Resource_Data2F7:
	.incbin "baserom.gba", 0x007201ac, 0x000029ec
	.global Resource_Data2F8
Resource_Data2F8:
	.incbin "baserom.gba", 0x00722b98, 0x000013ec
	.global Resource_Data2F9
Resource_Data2F9:
	.incbin "baserom.gba", 0x00723f84, 0x00001508
	.global Resource_Data2FA
Resource_Data2FA:
	.incbin "baserom.gba", 0x0072548c, 0x00003ec0
	.section .unidentified.0872950c,"a"
	.global Resource_Data2FC
Resource_Data2FC:
	.incbin "baserom.gba", 0x0072950c, 0x00001170
	.section .unidentified.0872a7d4,"a"
	.global Resource_Data2FE
Resource_Data2FE:
	.incbin "baserom.gba", 0x0072a7d4, 0x000010a0
	.section .unidentified.0872b9d5,"a"
	.incbin "baserom.gba", 0x0072b9d5, 0x00000003
	.section .unidentified.0872e3a1,"a"
	.incbin "baserom.gba", 0x0072e3a1, 0x00000003
	.global Resource_Data301
Resource_Data301:
	.incbin "baserom.gba", 0x0072e3a4, 0x000012ec
	.global Resource_Data302
Resource_Data302:
	.incbin "baserom.gba", 0x0072f690, 0x00002240
	.section .unidentified.087319c8,"a"
	.global Resource_Data304
Resource_Data304:
	.incbin "baserom.gba", 0x007319c8, 0x00002a70
	.global Resource_Data305
Resource_Data305:
	.incbin "baserom.gba", 0x00734438, 0x000018d8
	.section .unidentified.08735e82,"a"
	.incbin "baserom.gba", 0x00735e82, 0x00000002
	.global Resource_Data307
Resource_Data307:
	.incbin "baserom.gba", 0x00735e84, 0x00001b68
	.section .unidentified.0873a0e7,"a"
	.incbin "baserom.gba", 0x0073a0e7, 0x00000001
	.section .unidentified.0873d902,"a"
	.incbin "baserom.gba", 0x0073d902, 0x00000002
	.section .unidentified.0873f32a,"a"
	.incbin "baserom.gba", 0x0073f32a, 0x00000002
	.global Resource_Data30D
Resource_Data30D:
	.incbin "baserom.gba", 0x0073f32c, 0x000026ac
	.section .unidentified.08743a3b,"a"
	.incbin "baserom.gba", 0x00743a3b, 0x00000001
	.section .unidentified.087458f7,"a"
	.incbin "baserom.gba", 0x007458f7, 0x00000001
	.section .unidentified.08748b41,"a"
	.incbin "baserom.gba", 0x00748b41, 0x00000003
	.global Resource_Data313
Resource_Data313:
	.incbin "baserom.gba", 0x00748b44, 0x000019c0
	.section .unidentified.0874a675,"a"
	.incbin "baserom.gba", 0x0074a675, 0x00000003
	.global Resource_Data315
Resource_Data315:
	.incbin "baserom.gba", 0x0074a678, 0x00001770
	.global Resource_Data316
Resource_Data316:
	.incbin "baserom.gba", 0x0074bde8, 0x0000189c
	.section .unidentified.0874d83a,"a"
	.incbin "baserom.gba", 0x0074d83a, 0x00000002
	.global Resource_Data318
Resource_Data318:
	.incbin "baserom.gba", 0x0074d83c, 0x00001870
	.section .unidentified.0874f22a,"a"
	.incbin "baserom.gba", 0x0074f22a, 0x00000002
	.global Resource_Data31A
Resource_Data31A:
	.incbin "baserom.gba", 0x0074f22c, 0x0000192c
	.global Resource_Data31B
Resource_Data31B:
	.incbin "baserom.gba", 0x00750b58, 0x00001edc
	.section .unidentified.08752ba7,"a"
	.incbin "baserom.gba", 0x00752ba7, 0x00000001
	.section .unidentified.08752d30,"a"
	.global Resource_Data31E
Resource_Data31E:
	.incbin "baserom.gba", 0x00752d30, 0x00000c40
	.section .unidentified.08753b15,"a"
	.incbin "baserom.gba", 0x00753b15, 0x00000003
	.global Resource_Data320
Resource_Data320:
	.incbin "baserom.gba", 0x00753b18, 0x00002040
	.section .unidentified.08755c3c,"a"
	.global Resource_Data322
Resource_Data322:
	.incbin "baserom.gba", 0x00755c3c, 0x00002d18
	.section .unidentified.08758b3a,"a"
	.incbin "baserom.gba", 0x00758b3a, 0x00000002
	.global Resource_Data324
Resource_Data324:
	.incbin "baserom.gba", 0x00758b3c, 0x000025fc
	.global Resource_Data325
Resource_Data325:
	.incbin "baserom.gba", 0x0075b138, 0x00000ca0
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
	.section .unidentified.087604e2,"a"
	.incbin "baserom.gba", 0x007604e2, 0x00000002
	.global Resource_Data328
Resource_Data328:
	.incbin "baserom.gba", 0x007604e4, 0x00001bd4
	.section .unidentified.08762228,"a"
	.global Resource_Data32A
Resource_Data32A:
	.incbin "baserom.gba", 0x00762228, 0x00001940
	.section .unidentified.08763cc1,"a"
	.incbin "baserom.gba", 0x00763cc1, 0x00000003
	.global Resource_Data32C
Resource_Data32C:
	.incbin "baserom.gba", 0x00763cc4, 0x000014dc
	.global Resource_Data32D
Resource_Data32D:
	.incbin "baserom.gba", 0x007651a0, 0x00001550
	.global Resource_Data32E
Resource_Data32E:
	.incbin "baserom.gba", 0x007666f0, 0x00001ec4
	.global Resource_Data32F
Resource_Data32F:
	.incbin "baserom.gba", 0x007685b4, 0x00001954
	.section .unidentified.0876a00b,"a"
	.incbin "baserom.gba", 0x0076a00b, 0x00000001
	.global Resource_Data331
Resource_Data331:
	.incbin "baserom.gba", 0x0076a00c, 0x00000a1c
	.section .unidentified.0876ab0d,"a"
	.incbin "baserom.gba", 0x0076ab0d, 0x00000003
	.section .unidentified.0876ada9,"a"
	.incbin "baserom.gba", 0x0076ada9, 0x00000003
	.section .unidentified.0876ee06,"a"
	.incbin "baserom.gba", 0x0076ee06, 0x00000002
	.section .unidentified.0876f3ba,"a"
	.incbin "baserom.gba", 0x0076f3ba, 0x00000002
	.global Resource_Data337
Resource_Data337:
	.incbin "baserom.gba", 0x0076f3bc, 0x00000f94
	.section .unidentified.08771d52,"a"
	.incbin "baserom.gba", 0x00771d52, 0x00000002
	.section .unidentified.087729c1,"a"
	.incbin "baserom.gba", 0x007729c1, 0x00000003
	.section .unidentified.08772c69,"a"
	.incbin "baserom.gba", 0x00772c69, 0x00000003
	.section .unidentified.08773aee,"a"
	.incbin "baserom.gba", 0x00773aee, 0x00000002
	.global Resource_Data33D
Resource_Data33D:
	.incbin "baserom.gba", 0x00773af0, 0x00001190
	.section .unidentified.08774e47,"a"
	.incbin "baserom.gba", 0x00774e47, 0x00000001
	.global Resource_Data33F
Resource_Data33F:
	.incbin "baserom.gba", 0x00774e48, 0x0000034c
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
	.global Resource_Data345
Resource_Data345:
	.incbin "baserom.gba", 0x007756b0, 0x0000034c
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
	.global Resource_Data34B
Resource_Data34B:
	.incbin "baserom.gba", 0x00775f18, 0x0000034c
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
	.global Resource_Data351
Resource_Data351:
	.incbin "baserom.gba", 0x00776780, 0x0000034c
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
	.global Resource_Data357
Resource_Data357:
	.incbin "baserom.gba", 0x00776fe8, 0x0000034c
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
	.global Resource_Data35D
Resource_Data35D:
	.incbin "baserom.gba", 0x00777850, 0x0000034c
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
	.global Resource_Data363
Resource_Data363:
	.incbin "baserom.gba", 0x007780b8, 0x0000034c
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
	.global Resource_Data369
Resource_Data369:
	.incbin "baserom.gba", 0x00778920, 0x0000034c
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
