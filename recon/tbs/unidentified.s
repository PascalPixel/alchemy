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
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "baserom.gba", 0x003cfc00, 0x000003d0
	.section .unidentified.083d086f,"a"
	.incbin "baserom.gba", 0x003d086f, 0x00000001
	.section .unidentified.083d0d12,"a"
	.incbin "baserom.gba", 0x003d0d12, 0x00000002
	.section .unidentified.083d1248,"a"
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
	.global BattleFx_WaveSheet
BattleFx_WaveSheet:
	.incbin "baserom.gba", 0x003de298, 0x00000540
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "baserom.gba", 0x003de7d8, 0x00000840
	.global BattleFx_PinkStarSheet
BattleFx_PinkStarSheet:
	.incbin "baserom.gba", 0x003df018, 0x00000390
	.global BattleFx_WaterSpraySheet
BattleFx_WaterSpraySheet:
	.incbin "baserom.gba", 0x003df3a8, 0x00001258
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "baserom.gba", 0x003e0600, 0x00000af0
	.global BattleFx_ExplosionSheet
BattleFx_ExplosionSheet:
	.incbin "baserom.gba", 0x003e10f0, 0x00000dd8
	.global BattleFx_MagentaSwirlSheet
BattleFx_MagentaSwirlSheet:
	.incbin "baserom.gba", 0x003e1ec8, 0x0000082c
	.global BattleFx_MagentaTailSheet
BattleFx_MagentaTailSheet:
	.incbin "baserom.gba", 0x003e26f4, 0x000003f0
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
	.global BattleFx_RuneSheet
BattleFx_RuneSheet:
	.incbin "baserom.gba", 0x003e41e0, 0x000003dc
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "baserom.gba", 0x003e45bc, 0x000003ac
	.global BattleFx_EarthWallSheet
BattleFx_EarthWallSheet:
	.incbin "baserom.gba", 0x003e4968, 0x00000ad4
	.global BattleFx_OrangePaletteA
BattleFx_OrangePaletteA:
	.incbin "baserom.gba", 0x003e543c, 0x00000084
	.global BattleFx_FlashBurstSheet
BattleFx_FlashBurstSheet:
	.incbin "baserom.gba", 0x003e54c0, 0x0000144c
	.global BattleFx_ThornSheet
BattleFx_ThornSheet:
	.incbin "baserom.gba", 0x003e690c, 0x00000d9c
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "baserom.gba", 0x003e76a8, 0x0000021c
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "baserom.gba", 0x003e78c4, 0x000002fc
	.global BattleFx_CometSheetC
BattleFx_CometSheetC:
	.incbin "baserom.gba", 0x003e7bc0, 0x000003a0
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
	.global BattleFx_FlameballSheet
BattleFx_FlameballSheet:
	.incbin "baserom.gba", 0x003ebca0, 0x00000558
	.global BattleFx_BoulderSheet
BattleFx_BoulderSheet:
	.incbin "baserom.gba", 0x003ec1f8, 0x00000c18
	.global BattleFx_RockWallSheet
BattleFx_RockWallSheet:
	.incbin "baserom.gba", 0x003ece10, 0x00000cd4
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "baserom.gba", 0x003edae4, 0x000008a0
	.global BattleFx_MarsDjinnSheet
BattleFx_MarsDjinnSheet:
	.incbin "baserom.gba", 0x003ee384, 0x000003dc
	.global BattleFx_MarsDjinnSmallSheet
BattleFx_MarsDjinnSmallSheet:
	.incbin "baserom.gba", 0x003ee760, 0x0000028c
	.global BattleFx_JupiterDjinnSheet
BattleFx_JupiterDjinnSheet:
	.incbin "baserom.gba", 0x003ee9ec, 0x000003a4
	.global BattleFx_JupiterDjinnSmallSheet
BattleFx_JupiterDjinnSmallSheet:
	.incbin "baserom.gba", 0x003eed90, 0x0000025c
	.global BattleFx_MercuryDjinnSheet
BattleFx_MercuryDjinnSheet:
	.incbin "baserom.gba", 0x003eefec, 0x000003b8
	.global BattleFx_MercuryDjinnSmallSheet
BattleFx_MercuryDjinnSmallSheet:
	.incbin "baserom.gba", 0x003ef3a4, 0x0000026c
	.global BattleFx_VenusDjinnSheet
BattleFx_VenusDjinnSheet:
	.incbin "baserom.gba", 0x003ef610, 0x00000440
	.global BattleFx_VenusDjinnSmallSheet
BattleFx_VenusDjinnSmallSheet:
	.incbin "baserom.gba", 0x003efa50, 0x000002b4
	.global BattleFx_FireBlobSheet
BattleFx_FireBlobSheet:
	.incbin "baserom.gba", 0x003efd04, 0x00000b88
	.global BattleFx_FireStreakSheet
BattleFx_FireStreakSheet:
	.incbin "baserom.gba", 0x003f088c, 0x000015c4
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f1e50, 0x00000a6c
	.global BattleFx_YellowRingSheet
BattleFx_YellowRingSheet:
	.incbin "baserom.gba", 0x003f28bc, 0x000003c4
	.global BattleFx_YellowOrbSheet
BattleFx_YellowOrbSheet:
	.incbin "baserom.gba", 0x003f2c80, 0x00000a14
	.global BattleFx_BubbleSheet
BattleFx_BubbleSheet:
	.incbin "baserom.gba", 0x003f3694, 0x00000384
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "baserom.gba", 0x003f3a18, 0x00000cb0
	.global BattleFx_DaggerSheet
BattleFx_DaggerSheet:
	.incbin "baserom.gba", 0x003f46c8, 0x00000b48
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
	.global BattleFx_WispSheet
BattleFx_WispSheet:
	.incbin "baserom.gba", 0x003f5f4c, 0x000007b0
	.global BattleFx_GreenVineSheet
BattleFx_GreenVineSheet:
	.incbin "baserom.gba", 0x003f66fc, 0x00000518
	.global BattleFx_PortalSheet
BattleFx_PortalSheet:
	.incbin "baserom.gba", 0x003f6c14, 0x00000624
	.global BattleFx_SheepSheet
BattleFx_SheepSheet:
	.incbin "baserom.gba", 0x003f7238, 0x00000868
	.global BattleFx_SkullSheet
BattleFx_SkullSheet:
	.incbin "baserom.gba", 0x003f7aa0, 0x00000424
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
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "baserom.gba", 0x003fb484, 0x000003ec
	.global BattleFx_SpiralSheet
BattleFx_SpiralSheet:
	.incbin "baserom.gba", 0x003fb870, 0x000012e8
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "baserom.gba", 0x003fcb58, 0x00000c8c
	.global BattleFx_CrescentMoonSheet
BattleFx_CrescentMoonSheet:
	.incbin "baserom.gba", 0x003fd7e4, 0x000004d0
	.global BattleFx_ThornVineSheet
BattleFx_ThornVineSheet:
	.incbin "baserom.gba", 0x003fdcb4, 0x00000694
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe348, 0x00000a34
	.global BattleFx_EmberStreakSheet
BattleFx_EmberStreakSheet:
	.incbin "baserom.gba", 0x003fed7c, 0x00000bfc
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "baserom.gba", 0x003ff978, 0x000007d8
	.global BattleFx_BlueArcSheetB
BattleFx_BlueArcSheetB:
	.incbin "baserom.gba", 0x00400150, 0x00000ad0
	.global BattleFx_GlowOrbSheet
BattleFx_GlowOrbSheet:
	.incbin "baserom.gba", 0x00400c20, 0x00000b3c
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "baserom.gba", 0x0040175c, 0x00000640
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00401d9c, 0x00001588
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "baserom.gba", 0x00403324, 0x00000064
	.global BattleFx_PinkBurstSheet
BattleFx_PinkBurstSheet:
	.incbin "baserom.gba", 0x00403388, 0x0000036c
	.global BattleFx_LightFanSheet
BattleFx_LightFanSheet:
	.incbin "baserom.gba", 0x004036f4, 0x000005a0
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "baserom.gba", 0x00403c94, 0x00000334
	.global BattleFx_DustPuffSheet
BattleFx_DustPuffSheet:
	.incbin "baserom.gba", 0x00403fc8, 0x0000033c
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "baserom.gba", 0x00404304, 0x00001018
	.global BattleFx_BlastSheet
BattleFx_BlastSheet:
	.incbin "baserom.gba", 0x0040531c, 0x0000166c
	.global BattleFx_FlameSheetB
BattleFx_FlameSheetB:
	.incbin "baserom.gba", 0x00406988, 0x00000da0
	.global BattleFx_FirePillarSheetB
BattleFx_FirePillarSheetB:
	.incbin "baserom.gba", 0x00407728, 0x00001200
	.global BattleFx_RockSpireSheet
BattleFx_RockSpireSheet:
	.incbin "baserom.gba", 0x00408928, 0x000007a4
	.global BattleFx_LightningBoltSheet
BattleFx_LightningBoltSheet:
	.incbin "baserom.gba", 0x004090cc, 0x00001014
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
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "baserom.gba", 0x0040fb1c, 0x000006b4
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "baserom.gba", 0x004101d0, 0x000010a8
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
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x004c03d0, 0x00000190
	.global Resource_Data130
Resource_Data130:
	.incbin "baserom.gba", 0x004c0560, 0x00002b34
	.global Resource_Data131
Resource_Data131:
	.incbin "baserom.gba", 0x004c3094, 0x0000252c
	.global Resource_Data132
Resource_Data132:
	.incbin "baserom.gba", 0x004c55c0, 0x000026dc
	.global Resource_Data133
Resource_Data133:
	.incbin "baserom.gba", 0x004c7c9c, 0x00001e90
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x004c9b2c, 0x000051c8
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x004cecf4, 0x00000194
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x004cee88, 0x00002c54
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x004d1adc, 0x00002608
	.global Resource_Data138
Resource_Data138:
	.incbin "baserom.gba", 0x004d40e4, 0x000027e0
	.global Resource_Data139
Resource_Data139:
	.incbin "baserom.gba", 0x004d68c4, 0x00001ecc
	.global Resource_Data13A
Resource_Data13A:
	.incbin "baserom.gba", 0x004d8790, 0x0000313c
	.global Resource_Data13B
Resource_Data13B:
	.incbin "baserom.gba", 0x004db8cc, 0x00000180
	.global Resource_Data13C
Resource_Data13C:
	.incbin "baserom.gba", 0x004dba4c, 0x000027bc
	.global Resource_Data13D
Resource_Data13D:
	.incbin "baserom.gba", 0x004de208, 0x00002648
	.global Resource_Data13E
Resource_Data13E:
	.incbin "baserom.gba", 0x004e0850, 0x000026e4
	.global Resource_Data13F
Resource_Data13F:
	.incbin "baserom.gba", 0x004e2f34, 0x00001074
	.global Resource_Data140
Resource_Data140:
	.incbin "baserom.gba", 0x004e3fa8, 0x00002154
	.global Resource_Data141
Resource_Data141:
	.incbin "baserom.gba", 0x004e60fc, 0x00000194
	.global Resource_Data142
Resource_Data142:
	.incbin "baserom.gba", 0x004e6290, 0x00002840
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x004e8ad0, 0x00001e18
	.global Resource_Data144
Resource_Data144:
	.incbin "baserom.gba", 0x004ea8e8, 0x000026e0
	.global Resource_Data145
Resource_Data145:
	.incbin "baserom.gba", 0x004ecfc8, 0x00001148
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x004ee110, 0x00003114
	.global Resource_Data147
Resource_Data147:
	.incbin "baserom.gba", 0x004f1224, 0x000001ac
	.global Resource_Data148
Resource_Data148:
	.incbin "baserom.gba", 0x004f13d0, 0x00001a7c
	.global Resource_Data149
Resource_Data149:
	.incbin "baserom.gba", 0x004f2e4c, 0x00002540
	.global Resource_Data14A
Resource_Data14A:
	.incbin "baserom.gba", 0x004f538c, 0x000015fc
	.global Resource_Data14B
Resource_Data14B:
	.incbin "baserom.gba", 0x004f6988, 0x000012c8
	.global Resource_Data14C
Resource_Data14C:
	.incbin "baserom.gba", 0x004f7c50, 0x00003418
	.global Resource_Data14D
Resource_Data14D:
	.incbin "baserom.gba", 0x004fb068, 0x00000190
	.global Resource_Data14E
Resource_Data14E:
	.incbin "baserom.gba", 0x004fb1f8, 0x00001db0
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x004fcfa8, 0x0000256c
	.global Resource_Data150
Resource_Data150:
	.incbin "baserom.gba", 0x004ff514, 0x0000160c
	.global Resource_Data151
Resource_Data151:
	.incbin "baserom.gba", 0x00500b20, 0x0000150c
	.global Resource_Data152
Resource_Data152:
	.incbin "baserom.gba", 0x0050202c, 0x00000bd4
	.global Resource_Data153
Resource_Data153:
	.incbin "baserom.gba", 0x00502c00, 0x00000100
	.global Resource_Data154
Resource_Data154:
	.incbin "baserom.gba", 0x00502d00, 0x000013e4
	.global Resource_Data155
Resource_Data155:
	.incbin "baserom.gba", 0x005040e4, 0x00001184
	.global Resource_Data156
Resource_Data156:
	.incbin "baserom.gba", 0x00505268, 0x00000988
	.global Resource_Data157
Resource_Data157:
	.incbin "baserom.gba", 0x00505bf0, 0x00000854
	.global Resource_Data158
Resource_Data158:
	.incbin "baserom.gba", 0x00506444, 0x00002428
	.global Resource_Data159
Resource_Data159:
	.incbin "baserom.gba", 0x0050886c, 0x000000e4
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x00508950, 0x000011ec
	.global Resource_Data15B
Resource_Data15B:
	.incbin "baserom.gba", 0x00509b3c, 0x00001c88
	.global Resource_Data15C
Resource_Data15C:
	.incbin "baserom.gba", 0x0050b7c4, 0x0000050c
	.global Resource_Data15D
Resource_Data15D:
	.incbin "baserom.gba", 0x0050bcd0, 0x00000140
	.global Resource_Data15E
Resource_Data15E:
	.incbin "baserom.gba", 0x0050be10, 0x0000121c
	.global Resource_Data15F
Resource_Data15F:
	.incbin "baserom.gba", 0x0050d02c, 0x0000015c
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x0050d188, 0x000029d8
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x0050fb60, 0x00001a4c
	.global Resource_Data162
Resource_Data162:
	.incbin "baserom.gba", 0x005115ac, 0x000014ec
	.global Resource_Data163
Resource_Data163:
	.incbin "baserom.gba", 0x00512a98, 0x00000844
	.global Resource_Data164
Resource_Data164:
	.incbin "baserom.gba", 0x005132dc, 0x00000d78
	.global Resource_Data165
Resource_Data165:
	.incbin "baserom.gba", 0x00514054, 0x00000134
	.global Resource_Data166
Resource_Data166:
	.incbin "baserom.gba", 0x00514188, 0x00002480
	.global Resource_Data167
Resource_Data167:
	.incbin "baserom.gba", 0x00516608, 0x00000d7c
	.global Resource_Data168
Resource_Data168:
	.incbin "baserom.gba", 0x00517384, 0x00000ae0
	.global Resource_Data169
Resource_Data169:
	.incbin "baserom.gba", 0x00517e64, 0x00000e78
	.global Resource_Data16A
Resource_Data16A:
	.incbin "baserom.gba", 0x00518cdc, 0x00000154
	.global Resource_Data16B
Resource_Data16B:
	.incbin "baserom.gba", 0x00518e30, 0x00001f80
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x0051adb0, 0x00000948
	.global Resource_Data16D
Resource_Data16D:
	.incbin "baserom.gba", 0x0051b6f8, 0x00000c4c
	.global Resource_Data16E
Resource_Data16E:
	.incbin "baserom.gba", 0x0051c344, 0x00000228
	.global Resource_Data16F
Resource_Data16F:
	.incbin "baserom.gba", 0x0051c56c, 0x00000d28
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x0051d294, 0x00000c18
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x0051deac, 0x00000114
	.global Resource_Data172
Resource_Data172:
	.incbin "baserom.gba", 0x0051dfc0, 0x000010bc
	.global Resource_Data173
Resource_Data173:
	.incbin "baserom.gba", 0x0051f07c, 0x0000090c
	.global Resource_Data174
Resource_Data174:
	.incbin "baserom.gba", 0x0051f988, 0x00000148
	.global Resource_Data175
Resource_Data175:
	.incbin "baserom.gba", 0x0051fad0, 0x00000620
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x005200f0, 0x00001aac
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x00521b9c, 0x00000128
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x00521cc4, 0x00001888
	.global Resource_Data179
Resource_Data179:
	.incbin "baserom.gba", 0x0052354c, 0x00001364
	.global Resource_Data17A
Resource_Data17A:
	.incbin "baserom.gba", 0x005248b0, 0x00000f40
	.global Resource_Data17B
Resource_Data17B:
	.incbin "baserom.gba", 0x005257f0, 0x00000140
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x00525930, 0x00002008
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x00527938, 0x0000010c
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x00527a44, 0x00001fe4
	.global Resource_Data17F
Resource_Data17F:
	.incbin "baserom.gba", 0x00529a28, 0x00001724
	.global Resource_Data180
Resource_Data180:
	.incbin "baserom.gba", 0x0052b14c, 0x00000f74
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x0052c0c0, 0x00000140
	.global Resource_Data182
Resource_Data182:
	.incbin "baserom.gba", 0x0052c200, 0x00001ce8
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x0052dee8, 0x0000015c
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x0052e044, 0x00001f48
	.global Resource_Data185
Resource_Data185:
	.incbin "baserom.gba", 0x0052ff8c, 0x00000184
	.global Resource_Data186
Resource_Data186:
	.incbin "baserom.gba", 0x00530110, 0x00002b54
	.global Resource_Data187
Resource_Data187:
	.incbin "baserom.gba", 0x00532c64, 0x000026c4
	.global Resource_Data188
Resource_Data188:
	.incbin "baserom.gba", 0x00535328, 0x00002758
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x00537a80, 0x00001854
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x005392d4, 0x00002700
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x0053b9d4, 0x000001a0
	.global Resource_Data18C
Resource_Data18C:
	.incbin "baserom.gba", 0x0053bb74, 0x00001c2c
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x0053d7a0, 0x00002548
	.global Resource_Data18E
Resource_Data18E:
	.incbin "baserom.gba", 0x0053fce8, 0x000012cc
	.global Resource_Data18F
Resource_Data18F:
	.incbin "baserom.gba", 0x00540fb4, 0x00000cb0
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x00541c64, 0x00000a6c
	.global Resource_Data191
Resource_Data191:
	.incbin "baserom.gba", 0x005426d0, 0x000000f4
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x005427c4, 0x0000112c
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x005438f0, 0x000007ac
	.global Resource_Data194
Resource_Data194:
	.incbin "baserom.gba", 0x0054409c, 0x00000188
	.global Resource_Data195
Resource_Data195:
	.incbin "baserom.gba", 0x00544224, 0x00000da4
	.global Resource_Data196
Resource_Data196:
	.incbin "baserom.gba", 0x00544fc8, 0x00001b38
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x00546b00, 0x0000019c
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00546c9c, 0x00002d54
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x005499f0, 0x0000268c
	.global Resource_Data19A
Resource_Data19A:
	.incbin "baserom.gba", 0x0054c07c, 0x000027e0
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0054e85c, 0x00001884
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x005500e0, 0x000010f4
	.global Resource_Data19D
Resource_Data19D:
	.incbin "baserom.gba", 0x005511d4, 0x000000cc
	.global Resource_Data19E
Resource_Data19E:
	.incbin "baserom.gba", 0x005512a0, 0x000027a4
	.global Resource_Data19F
Resource_Data19F:
	.incbin "baserom.gba", 0x00553a44, 0x0000215c
	.global Resource_Data1A0
Resource_Data1A0:
	.incbin "baserom.gba", 0x00555ba0, 0x00000fac
	.global Resource_Data1A1
Resource_Data1A1:
	.incbin "baserom.gba", 0x00556b4c, 0x00000140
	.global Resource_Data1A2
Resource_Data1A2:
	.incbin "baserom.gba", 0x00556c8c, 0x00001430
	.global Resource_Data1A3
Resource_Data1A3:
	.incbin "baserom.gba", 0x005580bc, 0x00000124
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x005581e0, 0x00002c18
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x0055adf8, 0x000020fc
	.global Resource_Data1A6
Resource_Data1A6:
	.incbin "baserom.gba", 0x0055cef4, 0x00001da4
	.global Resource_Data1A7
Resource_Data1A7:
	.incbin "baserom.gba", 0x0055ec98, 0x00000614
	.global Resource_Data1A8
Resource_Data1A8:
	.incbin "baserom.gba", 0x0055f2ac, 0x00001d24
	.global Resource_Data1A9
Resource_Data1A9:
	.incbin "baserom.gba", 0x00560fd0, 0x00000f44
	.global Resource_Data1AA
Resource_Data1AA:
	.incbin "baserom.gba", 0x00561f14, 0x000029e8
	.global Resource_Data1AB
Resource_Data1AB:
	.incbin "baserom.gba", 0x005648fc, 0x0000019c
	.global Resource_Data1AC
Resource_Data1AC:
	.incbin "baserom.gba", 0x00564a98, 0x00002444
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00566edc, 0x00002458
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00569334, 0x00000ac8
	.global Resource_Data1AF
Resource_Data1AF:
	.incbin "baserom.gba", 0x00569dfc, 0x00001a38
	.global Resource_Data1B0
Resource_Data1B0:
	.incbin "baserom.gba", 0x0056b834, 0x00002f40
	.global Resource_Data1B1
Resource_Data1B1:
	.incbin "baserom.gba", 0x0056e774, 0x00002dcc
	.global Resource_Data1B2
Resource_Data1B2:
	.incbin "baserom.gba", 0x00571540, 0x00001ce4
	.global Resource_Data1B3
Resource_Data1B3:
	.incbin "baserom.gba", 0x00573224, 0x0000019c
	.global Resource_Data1B4
Resource_Data1B4:
	.incbin "baserom.gba", 0x005733c0, 0x00002bb0
	.global Resource_Data1B5
Resource_Data1B5:
	.incbin "baserom.gba", 0x00575f70, 0x000020d4
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x00578044, 0x00002058
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057a09c, 0x00001cec
	.global Resource_Data1B8
Resource_Data1B8:
	.incbin "baserom.gba", 0x0057bd88, 0x000017ac
	.global Resource_Data1B9
Resource_Data1B9:
	.incbin "baserom.gba", 0x0057d534, 0x0000019c
	.global Resource_Data1BA
Resource_Data1BA:
	.incbin "baserom.gba", 0x0057d6d0, 0x00002b40
	.global Resource_Data1BB
Resource_Data1BB:
	.incbin "baserom.gba", 0x00580210, 0x00001dd0
	.global Resource_Data1BC
Resource_Data1BC:
	.incbin "baserom.gba", 0x00581fe0, 0x00000140
	.global Resource_Data1BD
Resource_Data1BD:
	.incbin "baserom.gba", 0x00582120, 0x00001304
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00583424, 0x00001d58
	.global Resource_Data1BF
Resource_Data1BF:
	.incbin "baserom.gba", 0x0058517c, 0x00000184
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00585300, 0x00002df8
	.global Resource_Data1C1
Resource_Data1C1:
	.incbin "baserom.gba", 0x005880f8, 0x000028c8
	.global Resource_Data1C2
Resource_Data1C2:
	.incbin "baserom.gba", 0x0058a9c0, 0x0000295c
	.global Resource_Data1C3
Resource_Data1C3:
	.incbin "baserom.gba", 0x0058d31c, 0x00001b98
	.global Resource_Data1C4
Resource_Data1C4:
	.incbin "baserom.gba", 0x0058eeb4, 0x00002a74
	.global Resource_Data1C5
Resource_Data1C5:
	.incbin "baserom.gba", 0x00591928, 0x000001a8
	.global Resource_Data1C6
Resource_Data1C6:
	.incbin "baserom.gba", 0x00591ad0, 0x000016fc
	.global Resource_Data1C7
Resource_Data1C7:
	.incbin "baserom.gba", 0x005931cc, 0x00000900
	.global Resource_Data1C8
Resource_Data1C8:
	.incbin "baserom.gba", 0x00593acc, 0x00000d78
	.global Resource_Data1C9
Resource_Data1C9:
	.incbin "baserom.gba", 0x00594844, 0x00000160
	.global Resource_Data1CA
Resource_Data1CA:
	.incbin "baserom.gba", 0x005949a4, 0x00001c58
	.global Resource_Data1CB
Resource_Data1CB:
	.incbin "baserom.gba", 0x005965fc, 0x0000189c
	.global Resource_Data1CC
Resource_Data1CC:
	.incbin "baserom.gba", 0x00597e98, 0x00000184
	.global Resource_Data1CD
Resource_Data1CD:
	.incbin "baserom.gba", 0x0059801c, 0x00002b3c
	.global Resource_Data1CE
Resource_Data1CE:
	.incbin "baserom.gba", 0x0059ab58, 0x0000277c
	.global Resource_Data1CF
Resource_Data1CF:
	.incbin "baserom.gba", 0x0059d2d4, 0x00000fec
	.global Resource_Data1D0
Resource_Data1D0:
	.incbin "baserom.gba", 0x0059e2c0, 0x000016cc
	.global Resource_Data1D1
Resource_Data1D1:
	.incbin "baserom.gba", 0x0059f98c, 0x000001ac
	.global Resource_Data1D2
Resource_Data1D2:
	.incbin "baserom.gba", 0x0059fb38, 0x00002d94
	.global Resource_Data1D3
Resource_Data1D3:
	.incbin "baserom.gba", 0x005a28cc, 0x00002718
	.global Resource_Data1D4
Resource_Data1D4:
	.incbin "baserom.gba", 0x005a4fe4, 0x0000197c
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x005a6960, 0x00000140
	.global Resource_Data1D6
Resource_Data1D6:
	.incbin "baserom.gba", 0x005a6aa0, 0x00001ec8
	.global Resource_Data1D7
Resource_Data1D7:
	.incbin "baserom.gba", 0x005a8968, 0x000001a4
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x005a8b0c, 0x000020a0
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x005aabac, 0x00000f8c
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005abb38, 0x000022d8
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x005ade10, 0x00001254
	.global Resource_Data1DC
Resource_Data1DC:
	.incbin "baserom.gba", 0x005af064, 0x000018a0
	.global Resource_Data1DD
Resource_Data1DD:
	.incbin "baserom.gba", 0x005b0904, 0x00000184
	.global Resource_Data1DE
Resource_Data1DE:
	.incbin "baserom.gba", 0x005b0a88, 0x00002c20
	.global Resource_Data1DF
Resource_Data1DF:
	.incbin "baserom.gba", 0x005b36a8, 0x000022e4
	.global Resource_Data1E0
Resource_Data1E0:
	.incbin "baserom.gba", 0x005b598c, 0x00002848
	.global Resource_Data1E1
Resource_Data1E1:
	.incbin "baserom.gba", 0x005b81d4, 0x0000050c
	.global Resource_Data1E2
Resource_Data1E2:
	.incbin "baserom.gba", 0x005b86e0, 0x000016d8
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x005b9db8, 0x000001a4
	.global Resource_Data1E4
Resource_Data1E4:
	.incbin "baserom.gba", 0x005b9f5c, 0x00001904
	.global Resource_Data1E5
Resource_Data1E5:
	.incbin "baserom.gba", 0x005bb860, 0x0000182c
	.global Resource_Data1E6
Resource_Data1E6:
	.incbin "baserom.gba", 0x005bd08c, 0x00000180
	.global Resource_Data1E7
Resource_Data1E7:
	.incbin "baserom.gba", 0x005bd20c, 0x00002b84
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x005bfd90, 0x00001d70
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x005c1b00, 0x00001044
	.global Resource_Data1EA
Resource_Data1EA:
	.incbin "baserom.gba", 0x005c2b44, 0x00000e24
	.global Resource_Data1EB
Resource_Data1EB:
	.incbin "baserom.gba", 0x005c3968, 0x00001344
	.global Resource_Data1EC
Resource_Data1EC:
	.incbin "baserom.gba", 0x005c4cac, 0x0000015c
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x005c4e08, 0x000025bc
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x005c73c4, 0x00000178
	.global Resource_Data1EF
Resource_Data1EF:
	.incbin "baserom.gba", 0x005c753c, 0x00002b84
	.global Resource_Data1F0
Resource_Data1F0:
	.incbin "baserom.gba", 0x005ca0c0, 0x00001d70
	.global Resource_Data1F1
Resource_Data1F1:
	.incbin "baserom.gba", 0x005cbe30, 0x00000a68
	.global Resource_Data1F2
Resource_Data1F2:
	.incbin "baserom.gba", 0x005cc898, 0x00000840
	.global Resource_Data1F3
Resource_Data1F3:
	.incbin "baserom.gba", 0x005cd0d8, 0x00001e00
	.global Resource_Data1F4
Resource_Data1F4:
	.incbin "baserom.gba", 0x005ceed8, 0x00000168
	.global Resource_Data1F5
Resource_Data1F5:
	.incbin "baserom.gba", 0x005cf040, 0x00001148
	.global Resource_Data1F6
Resource_Data1F6:
	.incbin "baserom.gba", 0x005d0188, 0x00001d5c
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x005d1ee4, 0x00000168
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x005d204c, 0x00002890
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x005d48dc, 0x00000134
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x005d4a10, 0x00002c3c
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x005d764c, 0x00001fdc
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x005d9628, 0x000017c4
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x005dadec, 0x00001a7c
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x005dc868, 0x000018d0
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x005de138, 0x0000017c
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x005de2b4, 0x00002bb0
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x005e0e64, 0x00002274
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x005e30d8, 0x00002870
	.global Resource_Data203
Resource_Data203:
	.incbin "baserom.gba", 0x005e5948, 0x00000ed8
	.global Resource_Data204
Resource_Data204:
	.incbin "baserom.gba", 0x005e6820, 0x0000248c
	.global Resource_Data205
Resource_Data205:
	.incbin "baserom.gba", 0x005e8cac, 0x00000190
	.global Resource_Data206
Resource_Data206:
	.incbin "baserom.gba", 0x005e8e3c, 0x00001fe4
	.global Resource_Data207
Resource_Data207:
	.incbin "baserom.gba", 0x005eae20, 0x00002c44
	.global Resource_Data208
Resource_Data208:
	.incbin "baserom.gba", 0x005eda64, 0x00000c84
	.global Resource_Data209
Resource_Data209:
	.incbin "baserom.gba", 0x005ee6e8, 0x00000140
	.global Resource_Data20A
Resource_Data20A:
	.incbin "baserom.gba", 0x005ee828, 0x00001570
	.global Resource_Data20B
Resource_Data20B:
	.incbin "baserom.gba", 0x005efd98, 0x0000016c
	.global Resource_Data20C
Resource_Data20C:
	.incbin "baserom.gba", 0x005eff04, 0x00001d5c
	.global Resource_Data20D
Resource_Data20D:
	.incbin "baserom.gba", 0x005f1c60, 0x00000140
	.global Resource_Data20E
Resource_Data20E:
	.incbin "baserom.gba", 0x005f1da0, 0x00002800
	.global Resource_Data20F
Resource_Data20F:
	.incbin "baserom.gba", 0x005f45a0, 0x00000140
	.global Resource_Data210
Resource_Data210:
	.incbin "baserom.gba", 0x005f46e0, 0x00001a18
	.global Resource_Data211
Resource_Data211:
	.incbin "baserom.gba", 0x005f60f8, 0x000001a0
	.global Resource_Data212
Resource_Data212:
	.incbin "baserom.gba", 0x005f6298, 0x000013dc
	.global Resource_Data213
Resource_Data213:
	.incbin "baserom.gba", 0x005f7674, 0x0000153c
	.global Resource_Data214
Resource_Data214:
	.incbin "baserom.gba", 0x005f8bb0, 0x00000190
	.global Resource_Data215
Resource_Data215:
	.incbin "baserom.gba", 0x005f8d40, 0x000022b8
	.global Resource_Data216
Resource_Data216:
	.incbin "baserom.gba", 0x005faff8, 0x00001758
	.global Resource_Data217
Resource_Data217:
	.incbin "baserom.gba", 0x005fc750, 0x00000188
	.global Resource_Data218
Resource_Data218:
	.incbin "baserom.gba", 0x005fc8d8, 0x00002968
	.global Resource_Data219
Resource_Data219:
	.incbin "baserom.gba", 0x005ff240, 0x00001c98
	.global Resource_Data21A
Resource_Data21A:
	.incbin "baserom.gba", 0x00600ed8, 0x00000148
	.global Resource_Data21B
Resource_Data21B:
	.incbin "baserom.gba", 0x00601020, 0x00001cf4
	.global Resource_Data21C
Resource_Data21C:
	.incbin "baserom.gba", 0x00602d14, 0x000014b8
	.global Resource_Data21D
Resource_Data21D:
	.incbin "baserom.gba", 0x006041cc, 0x00002634
	.global Resource_Data21E
Resource_Data21E:
	.incbin "baserom.gba", 0x00606800, 0x0000029c
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x00606a9c, 0x000028bc
	.global Resource_Data220
Resource_Data220:
	.incbin "baserom.gba", 0x00609358, 0x000001c8
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.global Resource_Data222
Resource_Data222:
	.incbin "baserom.gba", 0x0060be24, 0x00001eac
	.global Resource_Data223
Resource_Data223:
	.incbin "baserom.gba", 0x0060dcd0, 0x00001c5c
	.global Resource_Data224
Resource_Data224:
	.incbin "baserom.gba", 0x0060f92c, 0x00001570
	.global Resource_Data225
Resource_Data225:
	.incbin "baserom.gba", 0x00610e9c, 0x00000e18
	.global Resource_Data226
Resource_Data226:
	.incbin "baserom.gba", 0x00611cb4, 0x00001a10
	.global Resource_Data227
Resource_Data227:
	.incbin "baserom.gba", 0x006136c4, 0x00000170
	.global Resource_Data228
Resource_Data228:
	.incbin "baserom.gba", 0x00613834, 0x000028c0
	.global Resource_Data229
Resource_Data229:
	.incbin "baserom.gba", 0x006160f4, 0x000027c8
	.global Resource_Data22A
Resource_Data22A:
	.incbin "baserom.gba", 0x006188bc, 0x000012fc
	.global Resource_Data22B
Resource_Data22B:
	.incbin "baserom.gba", 0x00619bb8, 0x00001944
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
	.global Resource_Data232
Resource_Data232:
	.incbin "baserom.gba", 0x00626990, 0x000001c0
	.global Resource_Data233
Resource_Data233:
	.incbin "baserom.gba", 0x00626b50, 0x000021b8
	.global Resource_Data234
Resource_Data234:
	.incbin "baserom.gba", 0x00628d08, 0x00000fd4
	.global Resource_Data235
Resource_Data235:
	.incbin "baserom.gba", 0x00629cdc, 0x000001dc
	.global Resource_Data236
Resource_Data236:
	.incbin "baserom.gba", 0x00629eb8, 0x000024f0
	.global Resource_Data237
Resource_Data237:
	.incbin "baserom.gba", 0x0062c3a8, 0x0000186c
	.global Resource_Data238
Resource_Data238:
	.incbin "baserom.gba", 0x0062dc14, 0x00001574
	.global Resource_Data239
Resource_Data239:
	.incbin "baserom.gba", 0x0062f188, 0x00000b9c
	.global Resource_Data23A
Resource_Data23A:
	.incbin "baserom.gba", 0x0062fd24, 0x00000e60
	.global Resource_Data23B
Resource_Data23B:
	.incbin "baserom.gba", 0x00630b84, 0x00000158
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
	.global Resource_Data242
Resource_Data242:
	.incbin "baserom.gba", 0x00638b28, 0x0000016c
	.global Resource_Data243
Resource_Data243:
	.incbin "baserom.gba", 0x00638c94, 0x00002d9c
	.global Resource_Data244
Resource_Data244:
	.incbin "baserom.gba", 0x0063ba30, 0x00002190
	.global Resource_Data245
Resource_Data245:
	.incbin "baserom.gba", 0x0063dbc0, 0x00000708
	.global Resource_Data246
Resource_Data246:
	.incbin "baserom.gba", 0x0063e2c8, 0x00000c14
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
	.global Resource_Data24C
Resource_Data24C:
	.incbin "baserom.gba", 0x0064ffb0, 0x00000194
	.global Resource_Data24D
Resource_Data24D:
	.incbin "baserom.gba", 0x00650144, 0x00001510
	.global Resource_Data24E
Resource_Data24E:
	.incbin "baserom.gba", 0x00651654, 0x00001de4
	.global Resource_Data24F
Resource_Data24F:
	.incbin "baserom.gba", 0x00653438, 0x00001094
	.global Resource_Data250
Resource_Data250:
	.incbin "baserom.gba", 0x006544cc, 0x00000554
	.global Resource_Data251
Resource_Data251:
	.incbin "baserom.gba", 0x00654a20, 0x00001d1c
	.global Resource_Data252
Resource_Data252:
	.incbin "baserom.gba", 0x0065673c, 0x00000198
	.global Resource_Data253
Resource_Data253:
	.incbin "baserom.gba", 0x006568d4, 0x00001fa4
	.global Resource_Data254
Resource_Data254:
	.incbin "baserom.gba", 0x00658878, 0x00000164
	.global Resource_Data255
Resource_Data255:
	.incbin "baserom.gba", 0x006589dc, 0x00002c10
	.global Resource_Data256
Resource_Data256:
	.incbin "baserom.gba", 0x0065b5ec, 0x00002064
	.global Resource_Data257
Resource_Data257:
	.incbin "baserom.gba", 0x0065d650, 0x00002754
	.global Resource_Data258
Resource_Data258:
	.incbin "baserom.gba", 0x0065fda4, 0x0000130c
	.global Resource_Data259
Resource_Data259:
	.incbin "baserom.gba", 0x006610b0, 0x00002b6c
	.global Resource_Data25A
Resource_Data25A:
	.incbin "baserom.gba", 0x00663c1c, 0x000001cc
	.global Resource_Data25B
Resource_Data25B:
	.incbin "baserom.gba", 0x00663de8, 0x00002348
	.global Resource_Data25C
Resource_Data25C:
	.incbin "baserom.gba", 0x00666130, 0x000017dc
	.global Resource_Data25D
Resource_Data25D:
	.incbin "baserom.gba", 0x0066790c, 0x000001b4
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
	.global Resource_Data267
Resource_Data267:
	.incbin "baserom.gba", 0x00678460, 0x00000160
	.global Resource_Data268
Resource_Data268:
	.incbin "baserom.gba", 0x006785c0, 0x0000193c
	.global Resource_Data269
Resource_Data269:
	.incbin "baserom.gba", 0x00679efc, 0x00001e1c
	.global Resource_Data26A
Resource_Data26A:
	.incbin "baserom.gba", 0x0067bd18, 0x00000164
	.global Resource_Data26B
Resource_Data26B:
	.incbin "baserom.gba", 0x0067be7c, 0x000034c4
	.global Resource_Data26C
Resource_Data26C:
	.incbin "baserom.gba", 0x0067f340, 0x00002820
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00681b60, 0x000000f4
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00681c54, 0x000028f0
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00684544, 0x0000220c
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00686750, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00686890, 0x00001a18
	.global Resource_Data272
Resource_Data272:
	.incbin "baserom.gba", 0x006882a8, 0x000024e0
	.global Resource_Data273
Resource_Data273:
	.incbin "baserom.gba", 0x0068a788, 0x000028e8
	.global Resource_Data274
Resource_Data274:
	.incbin "baserom.gba", 0x0068d070, 0x00001fa0
	.global Resource_Data275
Resource_Data275:
	.incbin "baserom.gba", 0x0068f010, 0x0000017c
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x0068f18c, 0x00001fac
	.global Resource_Data277
Resource_Data277:
	.incbin "baserom.gba", 0x00691138, 0x00002778
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x006938b0, 0x00001868
	.global Resource_Data279
Resource_Data279:
	.incbin "baserom.gba", 0x00695118, 0x00000c04
	.global Resource_Data27A
Resource_Data27A:
	.incbin "baserom.gba", 0x00695d1c, 0x00002e1c
	.global Resource_Data27B
Resource_Data27B:
	.incbin "baserom.gba", 0x00698b38, 0x000001c8
	.global Resource_Data27C
Resource_Data27C:
	.incbin "baserom.gba", 0x00698d00, 0x00002378
	.global Resource_Data27D
Resource_Data27D:
	.incbin "baserom.gba", 0x0069b078, 0x0000206c
	.global Resource_Data27E
Resource_Data27E:
	.incbin "baserom.gba", 0x0069d0e4, 0x00001bf8
	.global Resource_Data27F
Resource_Data27F:
	.incbin "baserom.gba", 0x0069ecdc, 0x000012d4
	.global Resource_Data280
Resource_Data280:
	.incbin "baserom.gba", 0x0069ffb0, 0x00000dfc
	.global Resource_Data281
Resource_Data281:
	.incbin "baserom.gba", 0x006a0dac, 0x00001178
	.global Resource_Data282
Resource_Data282:
	.incbin "baserom.gba", 0x006a1f24, 0x00001dc4
	.global Resource_Data283
Resource_Data283:
	.incbin "baserom.gba", 0x006a3ce8, 0x00000184
	.global Resource_Data284
Resource_Data284:
	.incbin "baserom.gba", 0x006a3e6c, 0x00001fc8
	.global Resource_Data285
Resource_Data285:
	.incbin "baserom.gba", 0x006a5e34, 0x0000018c
	.global Resource_Data286
Resource_Data286:
	.incbin "baserom.gba", 0x006a5fc0, 0x00001a84
	.global Resource_Data287
Resource_Data287:
	.incbin "baserom.gba", 0x006a7a44, 0x0000097c
	.global Resource_Data288
Resource_Data288:
	.incbin "baserom.gba", 0x006a83c0, 0x000026bc
	.global Resource_Data289
Resource_Data289:
	.incbin "baserom.gba", 0x006aaa7c, 0x000001c4
	.global Resource_Data28A
Resource_Data28A:
	.incbin "baserom.gba", 0x006aac40, 0x00000f34
	.global Resource_Data28B
Resource_Data28B:
	.incbin "baserom.gba", 0x006abb74, 0x0000014c
	.global Resource_Data28C
Resource_Data28C:
	.incbin "baserom.gba", 0x006abcc0, 0x00001f68
	.global Resource_Data28D
Resource_Data28D:
	.incbin "baserom.gba", 0x006adc28, 0x000001a8
	.global Resource_Data28E
Resource_Data28E:
	.incbin "baserom.gba", 0x006addd0, 0x00002c44
	.global Resource_Data28F
Resource_Data28F:
	.incbin "baserom.gba", 0x006b0a14, 0x000023e4
	.global Resource_Data290
Resource_Data290:
	.incbin "baserom.gba", 0x006b2df8, 0x00001f88
	.global Resource_Data291
Resource_Data291:
	.incbin "baserom.gba", 0x006b4d80, 0x00000e08
	.global Resource_Data292
Resource_Data292:
	.incbin "baserom.gba", 0x006b5b88, 0x000001c0
	.global Resource_Data293
Resource_Data293:
	.incbin "baserom.gba", 0x006b5d48, 0x00001358
	.global Resource_Data294
Resource_Data294:
	.incbin "baserom.gba", 0x006b70a0, 0x0000119c
	.global Resource_Data295
Resource_Data295:
	.incbin "baserom.gba", 0x006b823c, 0x000001d4
	.global Resource_Data296
Resource_Data296:
	.incbin "baserom.gba", 0x006b8410, 0x00002090
	.global Resource_Data297
Resource_Data297:
	.incbin "baserom.gba", 0x006ba4a0, 0x00002100
	.global Resource_Data298
Resource_Data298:
	.incbin "baserom.gba", 0x006bc5a0, 0x00002218
	.global Resource_Data299
Resource_Data299:
	.incbin "baserom.gba", 0x006be7b8, 0x0000099c
	.global Resource_Data29A
Resource_Data29A:
	.incbin "baserom.gba", 0x006bf154, 0x00000aec
	.global Resource_Data29B
Resource_Data29B:
	.incbin "baserom.gba", 0x006bfc40, 0x000001d4
	.global Resource_Data29C
Resource_Data29C:
	.incbin "baserom.gba", 0x006bfe14, 0x00001358
	.global Resource_Data29D
Resource_Data29D:
	.incbin "baserom.gba", 0x006c116c, 0x000001e4
	.global Resource_Data29E
Resource_Data29E:
	.incbin "baserom.gba", 0x006c1350, 0x000013f4
	.global Resource_Data29F
Resource_Data29F:
	.incbin "baserom.gba", 0x006c2744, 0x00001798
	.global Resource_Data2A0
Resource_Data2A0:
	.incbin "baserom.gba", 0x006c3edc, 0x00001274
	.global Resource_Data2A1
Resource_Data2A1:
	.incbin "baserom.gba", 0x006c5150, 0x00000568
	.global Resource_Data2A2
Resource_Data2A2:
	.incbin "baserom.gba", 0x006c56b8, 0x00001d80
	.global Resource_Data2A3
Resource_Data2A3:
	.incbin "baserom.gba", 0x006c7438, 0x000001a8
	.global Resource_Data2A4
Resource_Data2A4:
	.incbin "baserom.gba", 0x006c75e0, 0x000021b8
	.global Resource_Data2A5
Resource_Data2A5:
	.incbin "baserom.gba", 0x006c9798, 0x00000194
	.global Resource_Data2A6
Resource_Data2A6:
	.incbin "baserom.gba", 0x006c992c, 0x000011e8
	.global Resource_Data2A7
Resource_Data2A7:
	.incbin "baserom.gba", 0x006cab14, 0x00000188
	.global Resource_Data2A8
Resource_Data2A8:
	.incbin "baserom.gba", 0x006cac9c, 0x00001ed8
	.global Resource_Data2A9
Resource_Data2A9:
	.incbin "baserom.gba", 0x006ccb74, 0x00002674
	.global Resource_Data2AA
Resource_Data2AA:
	.incbin "baserom.gba", 0x006cf1e8, 0x00000154
	.global Resource_Data2AB
Resource_Data2AB:
	.incbin "baserom.gba", 0x006cf33c, 0x00000140
	.global Resource_Data2AC
Resource_Data2AC:
	.incbin "baserom.gba", 0x006cf47c, 0x00000cd4
	.global Resource_Data2AD
Resource_Data2AD:
	.incbin "baserom.gba", 0x006d0150, 0x00000180
	.global Resource_Data2AE
Resource_Data2AE:
	.incbin "baserom.gba", 0x006d02d0, 0x000018b8
	.global Resource_Data2AF
Resource_Data2AF:
	.incbin "baserom.gba", 0x006d1b88, 0x00000184
	.global Resource_Data2B0
Resource_Data2B0:
	.incbin "baserom.gba", 0x006d1d0c, 0x00001a90
	.global Resource_Data2B1
Resource_Data2B1:
	.incbin "baserom.gba", 0x006d379c, 0x0000017c
	.global Resource_Data2B2
Resource_Data2B2:
	.incbin "baserom.gba", 0x006d3918, 0x00001d3c
	.global Resource_Data2B3
Resource_Data2B3:
	.incbin "baserom.gba", 0x006d5654, 0x0000012c
	.global Resource_Data2B4
Resource_Data2B4:
	.incbin "baserom.gba", 0x006d5780, 0x00000178
	.global Resource_Data2B5
Resource_Data2B5:
	.incbin "baserom.gba", 0x006d58f8, 0x00001ea4
	.global Resource_Data2B6
Resource_Data2B6:
	.incbin "baserom.gba", 0x006d779c, 0x00000188
	.global Resource_Data2B7
Resource_Data2B7:
	.incbin "baserom.gba", 0x006d7924, 0x00000f5c
	.global Resource_Data2B8
Resource_Data2B8:
	.incbin "baserom.gba", 0x006d8880, 0x00000198
	.global Resource_Data2B9
Resource_Data2B9:
	.incbin "baserom.gba", 0x006d8a18, 0x0000110c
	.global Resource_Data2BA
Resource_Data2BA:
	.incbin "baserom.gba", 0x006d9b24, 0x000001bc
	.global Resource_Data2BB
Resource_Data2BB:
	.incbin "baserom.gba", 0x006d9ce0, 0x00002aa4
	.global Resource_Data2BC
Resource_Data2BC:
	.incbin "baserom.gba", 0x006dc784, 0x00000174
	.global Resource_Data2BD
Resource_Data2BD:
	.incbin "baserom.gba", 0x006dc8f8, 0x000013e8
	.global Resource_Data2BE
Resource_Data2BE:
	.incbin "baserom.gba", 0x006ddce0, 0x000001b8
	.global Resource_Data2BF
Resource_Data2BF:
	.incbin "baserom.gba", 0x006dde98, 0x00002928
	.global Resource_Data2C0
Resource_Data2C0:
	.incbin "baserom.gba", 0x006e07c0, 0x000001a8
	.global Resource_Data2C1
Resource_Data2C1:
	.incbin "baserom.gba", 0x006e0968, 0x000023fc
	.global Resource_Data2C2
Resource_Data2C2:
	.incbin "baserom.gba", 0x006e2d64, 0x00000190
	.global Resource_Data2C3
Resource_Data2C3:
	.incbin "baserom.gba", 0x006e2ef4, 0x00003280
	.global Resource_Data2C4
Resource_Data2C4:
	.incbin "baserom.gba", 0x006e6174, 0x000001e0
	.global Resource_Data2C5
Resource_Data2C5:
	.incbin "baserom.gba", 0x006e6354, 0x00000f0c
	.global Resource_Data2C6
Resource_Data2C6:
	.incbin "baserom.gba", 0x006e7260, 0x00001688
	.global Resource_Data2C7
Resource_Data2C7:
	.incbin "baserom.gba", 0x006e88e8, 0x00000190
	.global Resource_Data2C8
Resource_Data2C8:
	.incbin "baserom.gba", 0x006e8a78, 0x00002a70
	.global Resource_Data2C9
Resource_Data2C9:
	.incbin "baserom.gba", 0x006eb4e8, 0x00000188
	.global Resource_Data2CA
Resource_Data2CA:
	.incbin "baserom.gba", 0x006eb670, 0x00000b4c
	.global Resource_Data2CB
Resource_Data2CB:
	.incbin "baserom.gba", 0x006ec1bc, 0x00000168
	.global Resource_Data2CC
Resource_Data2CC:
	.incbin "baserom.gba", 0x006ec324, 0x00002a14
	.global Resource_Data2CD
Resource_Data2CD:
	.incbin "baserom.gba", 0x006eed38, 0x00001c40
	.global Resource_Data2CE
Resource_Data2CE:
	.incbin "baserom.gba", 0x006f0978, 0x000004bc
	.global Resource_Data2CF
Resource_Data2CF:
	.incbin "baserom.gba", 0x006f0e34, 0x00001378
	.global Resource_Data2D0
Resource_Data2D0:
	.incbin "baserom.gba", 0x006f21ac, 0x00001544
	.global Resource_Data2D1
Resource_Data2D1:
	.incbin "baserom.gba", 0x006f36f0, 0x0000018c
	.global Resource_Data2D2
Resource_Data2D2:
	.incbin "baserom.gba", 0x006f387c, 0x000010dc
	.global Resource_Data2D3
Resource_Data2D3:
	.incbin "baserom.gba", 0x006f4958, 0x00000190
	.global Resource_Data2D4
Resource_Data2D4:
	.incbin "baserom.gba", 0x006f4ae8, 0x00001b04
	.global Resource_Data2D5
Resource_Data2D5:
	.incbin "baserom.gba", 0x006f65ec, 0x00000150
	.global Resource_Data2D6
Resource_Data2D6:
	.incbin "baserom.gba", 0x006f673c, 0x000029e0
	.global Resource_Data2D7
Resource_Data2D7:
	.incbin "baserom.gba", 0x006f911c, 0x00001c94
	.global Resource_Data2D8
Resource_Data2D8:
	.incbin "baserom.gba", 0x006fadb0, 0x00001898
	.global Resource_Data2D9
Resource_Data2D9:
	.incbin "baserom.gba", 0x006fc648, 0x00001908
	.global Resource_Data2DA
Resource_Data2DA:
	.incbin "baserom.gba", 0x006fdf50, 0x000020a8
	.global Resource_Data2DB
Resource_Data2DB:
	.incbin "baserom.gba", 0x006ffff8, 0x0000014c
	.global Resource_Data2DC
Resource_Data2DC:
	.incbin "baserom.gba", 0x00700144, 0x00002908
	.global Resource_Data2DD
Resource_Data2DD:
	.incbin "baserom.gba", 0x00702a4c, 0x00001c4c
	.global Resource_Data2DE
Resource_Data2DE:
	.incbin "baserom.gba", 0x00704698, 0x00001448
	.global Resource_Data2DF
Resource_Data2DF:
	.incbin "baserom.gba", 0x00705ae0, 0x00001e80
	.global Resource_Data2E0
Resource_Data2E0:
	.incbin "baserom.gba", 0x00707960, 0x00001a94
	.global Resource_Data2E1
Resource_Data2E1:
	.incbin "baserom.gba", 0x007093f4, 0x0000013c
	.global Resource_Data2E2
Resource_Data2E2:
	.incbin "baserom.gba", 0x00709530, 0x00002734
	.global Resource_Data2E3
Resource_Data2E3:
	.incbin "baserom.gba", 0x0070bc64, 0x00001c44
	.global Resource_Data2E4
Resource_Data2E4:
	.incbin "baserom.gba", 0x0070d8a8, 0x00001448
	.global Resource_Data2E5
Resource_Data2E5:
	.incbin "baserom.gba", 0x0070ecf0, 0x00001b7c
	.global Resource_Data2E6
Resource_Data2E6:
	.incbin "baserom.gba", 0x0071086c, 0x00001230
	.global Resource_Data2E7
Resource_Data2E7:
	.incbin "baserom.gba", 0x00711a9c, 0x0000015c
	.global Resource_Data2E8
Resource_Data2E8:
	.incbin "baserom.gba", 0x00711bf8, 0x00000fa8
	.global Resource_Data2E9
Resource_Data2E9:
	.incbin "baserom.gba", 0x00712ba0, 0x0000167c
	.global Resource_Data2EA
Resource_Data2EA:
	.incbin "baserom.gba", 0x0071421c, 0x00000ed8
	.global Resource_Data2EB
Resource_Data2EB:
	.incbin "baserom.gba", 0x007150f4, 0x0000016c
	.global Resource_Data2EC
Resource_Data2EC:
	.incbin "baserom.gba", 0x00715260, 0x000019b8
	.global Resource_Data2ED
Resource_Data2ED:
	.incbin "baserom.gba", 0x00716c18, 0x00000f74
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00718cf0, 0x0000019c
	.global Resource_Data2F0
Resource_Data2F0:
	.incbin "baserom.gba", 0x00718e8c, 0x0000164c
	.global Resource_Data2F1
Resource_Data2F1:
	.incbin "baserom.gba", 0x0071a4d8, 0x00000140
	.global Resource_Data2F2
Resource_Data2F2:
	.incbin "baserom.gba", 0x0071a618, 0x00001dbc
	.global Resource_Data2F3
Resource_Data2F3:
	.incbin "baserom.gba", 0x0071c3d4, 0x00001348
	.global Resource_Data2F4
Resource_Data2F4:
	.incbin "baserom.gba", 0x0071d71c, 0x0000017c
	.global Resource_Data2F5
Resource_Data2F5:
	.incbin "baserom.gba", 0x0071d898, 0x0000277c
	.global Resource_Data2F6
Resource_Data2F6:
	.incbin "baserom.gba", 0x00720014, 0x00000198
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
	.global Resource_Data2FB
Resource_Data2FB:
	.incbin "baserom.gba", 0x0072934c, 0x000001c0
	.global Resource_Data2FC
Resource_Data2FC:
	.incbin "baserom.gba", 0x0072950c, 0x00001170
	.global Resource_Data2FD
Resource_Data2FD:
	.incbin "baserom.gba", 0x0072a67c, 0x00000158
	.global Resource_Data2FE
Resource_Data2FE:
	.incbin "baserom.gba", 0x0072a7d4, 0x000010a0
	.global Resource_Data2FF
Resource_Data2FF:
	.incbin "baserom.gba", 0x0072b874, 0x00000164
	.global Resource_Data300
Resource_Data300:
	.incbin "baserom.gba", 0x0072b9d8, 0x000029cc
	.global Resource_Data301
Resource_Data301:
	.incbin "baserom.gba", 0x0072e3a4, 0x000012ec
	.global Resource_Data302
Resource_Data302:
	.incbin "baserom.gba", 0x0072f690, 0x00002240
	.global Resource_Data303
Resource_Data303:
	.incbin "baserom.gba", 0x007318d0, 0x000000f8
	.global Resource_Data304
Resource_Data304:
	.incbin "baserom.gba", 0x007319c8, 0x00002a70
	.global Resource_Data305
Resource_Data305:
	.incbin "baserom.gba", 0x00734438, 0x000018d8
	.global Resource_Data306
Resource_Data306:
	.incbin "baserom.gba", 0x00735d10, 0x00000174
	.global Resource_Data307
Resource_Data307:
	.incbin "baserom.gba", 0x00735e84, 0x00001b68
	.global Resource_Data308
Resource_Data308:
	.incbin "baserom.gba", 0x007379ec, 0x000000e4
	.global Resource_Data309
Resource_Data309:
	.incbin "baserom.gba", 0x00737ad0, 0x00002618
	.global Resource_Data30A
Resource_Data30A:
	.incbin "baserom.gba", 0x0073a0e8, 0x000027b0
	.global Resource_Data30B
Resource_Data30B:
	.incbin "baserom.gba", 0x0073c898, 0x0000106c
	.global Resource_Data30C
Resource_Data30C:
	.incbin "baserom.gba", 0x0073d904, 0x00001a28
	.global Resource_Data30D
Resource_Data30D:
	.incbin "baserom.gba", 0x0073f32c, 0x000026ac
	.global Resource_Data30E
Resource_Data30E:
	.incbin "baserom.gba", 0x007419d8, 0x000001e4
	.global Resource_Data30F
Resource_Data30F:
	.incbin "baserom.gba", 0x00741bbc, 0x00001e80
	.global Resource_Data310
Resource_Data310:
	.incbin "baserom.gba", 0x00743a3c, 0x00001ebc
	.global Resource_Data311
Resource_Data311:
	.incbin "baserom.gba", 0x007458f8, 0x00001c3c
	.global Resource_Data312
Resource_Data312:
	.incbin "baserom.gba", 0x00747534, 0x00001610
	.global Resource_Data313
Resource_Data313:
	.incbin "baserom.gba", 0x00748b44, 0x000019c0
	.global Resource_Data314
Resource_Data314:
	.incbin "baserom.gba", 0x0074a504, 0x00000174
	.global Resource_Data315
Resource_Data315:
	.incbin "baserom.gba", 0x0074a678, 0x00001770
	.global Resource_Data316
Resource_Data316:
	.incbin "baserom.gba", 0x0074bde8, 0x0000189c
	.global Resource_Data317
Resource_Data317:
	.incbin "baserom.gba", 0x0074d684, 0x000001b8
	.global Resource_Data318
Resource_Data318:
	.incbin "baserom.gba", 0x0074d83c, 0x00001870
	.global Resource_Data319
Resource_Data319:
	.incbin "baserom.gba", 0x0074f0ac, 0x00000180
	.global Resource_Data31A
Resource_Data31A:
	.incbin "baserom.gba", 0x0074f22c, 0x0000192c
	.global Resource_Data31B
Resource_Data31B:
	.incbin "baserom.gba", 0x00750b58, 0x00001edc
	.global Resource_Data31C
Resource_Data31C:
	.incbin "baserom.gba", 0x00752a34, 0x00000174
	.global Resource_Data31D
Resource_Data31D:
	.incbin "baserom.gba", 0x00752ba8, 0x00000188
	.global Resource_Data31E
Resource_Data31E:
	.incbin "baserom.gba", 0x00752d30, 0x00000c40
	.global Resource_Data31F
Resource_Data31F:
	.incbin "baserom.gba", 0x00753970, 0x000001a8
	.global Resource_Data320
Resource_Data320:
	.incbin "baserom.gba", 0x00753b18, 0x00002040
	.global Resource_Data321
Resource_Data321:
	.incbin "baserom.gba", 0x00755b58, 0x000000e4
	.global Resource_Data322
Resource_Data322:
	.incbin "baserom.gba", 0x00755c3c, 0x00002d18
	.global Resource_Data323
Resource_Data323:
	.incbin "baserom.gba", 0x00758954, 0x000001e8
	.global Resource_Data324
Resource_Data324:
	.incbin "baserom.gba", 0x00758b3c, 0x000025fc
	.global Resource_Data325
Resource_Data325:
	.incbin "baserom.gba", 0x0075b138, 0x00000ca0
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
	.global Resource_Data327
Resource_Data327:
	.incbin "baserom.gba", 0x007603dc, 0x00000108
	.global Resource_Data328
Resource_Data328:
	.incbin "baserom.gba", 0x007604e4, 0x00001bd4
	.global Resource_Data329
Resource_Data329:
	.incbin "baserom.gba", 0x007620b8, 0x00000170
	.global Resource_Data32A
Resource_Data32A:
	.incbin "baserom.gba", 0x00762228, 0x00001940
	.global Resource_Data32B
Resource_Data32B:
	.incbin "baserom.gba", 0x00763b68, 0x0000015c
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
	.global Resource_Data330
Resource_Data330:
	.incbin "baserom.gba", 0x00769f08, 0x00000104
	.global Resource_Data331
Resource_Data331:
	.incbin "baserom.gba", 0x0076a00c, 0x00000a1c
	.global Resource_Data332
Resource_Data332:
	.incbin "baserom.gba", 0x0076aa28, 0x000000e8
	.global Resource_Data333
Resource_Data333:
	.incbin "baserom.gba", 0x0076ab10, 0x0000029c
	.global Resource_Data334
Resource_Data334:
	.incbin "baserom.gba", 0x0076adac, 0x00002168
	.global Resource_Data335
Resource_Data335:
	.incbin "baserom.gba", 0x0076cf14, 0x00001ef4
	.global Resource_Data336
Resource_Data336:
	.incbin "baserom.gba", 0x0076ee08, 0x000005b4
	.global Resource_Data337
Resource_Data337:
	.incbin "baserom.gba", 0x0076f3bc, 0x00000f94
	.global Resource_Data338
Resource_Data338:
	.incbin "baserom.gba", 0x00770350, 0x000001ac
	.global Resource_Data339
Resource_Data339:
	.incbin "baserom.gba", 0x007704fc, 0x00001858
	.global Resource_Data33A
Resource_Data33A:
	.incbin "baserom.gba", 0x00771d54, 0x00000c70
	.global Resource_Data33B
Resource_Data33B:
	.incbin "baserom.gba", 0x007729c4, 0x000002a8
	.global Resource_Data33C
Resource_Data33C:
	.incbin "baserom.gba", 0x00772c6c, 0x00000e84
	.global Resource_Data33D
Resource_Data33D:
	.incbin "baserom.gba", 0x00773af0, 0x00001190
	.global Resource_Data33E
Resource_Data33E:
	.incbin "baserom.gba", 0x00774c80, 0x000001c8
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
