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
	.incbin "baserom.gba", 0x000092b8, 0x00000500
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "baserom.gba", 0x000097b8, 0x00000400
	.section .unidentified.08012f20,"a"
	.global Object_ShadowTiles
Object_ShadowTiles:
	.incbin "baserom.gba", 0x00012f20, 0x00000080
	.incbin "baserom.gba", 0x00012fa0, 0x000001ac
	.global Map_TileDissolveOrder
Map_TileDissolveOrder:
	.incbin "baserom.gba", 0x0001314c, 0x000000f4
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
	.incbin "baserom.gba", 0x00013584, 0x0000000c
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
	.incbin "baserom.gba", 0x00031864, 0x000009c0
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
	.incbin "baserom.gba", 0x00037428, 0x0000003c
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
	.incbin "baserom.gba", 0x0009e87c, 0x00000174
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
	.incbin "baserom.gba", 0x000af20c, 0x00000010
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
	.incbin "baserom.gba", 0x000eef78, 0x0000002c
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
	.incbin "baserom.gba", 0x000fba04, 0x00000044
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
	.incbin "baserom.gba", 0x003203c8, 0x00000bd8
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
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "baserom.gba", 0x003cd090, 0x00000488
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "baserom.gba", 0x003cd518, 0x000005ec
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "baserom.gba", 0x003cdb04, 0x000006c4
	.global BattleFx_RevealMaskB
BattleFx_RevealMaskB:
	.incbin "baserom.gba", 0x003ce1c8, 0x000002e8
	.global BattleFx_RevealMaskC
BattleFx_RevealMaskC:
	.incbin "baserom.gba", 0x003ce4b0, 0x0000057c
	.global BattleFx_RevealMaskD
BattleFx_RevealMaskD:
	.incbin "baserom.gba", 0x003cea2c, 0x000006e8
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "baserom.gba", 0x003cf114, 0x00000700
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "baserom.gba", 0x003cf814, 0x000003ec
	.global BattleFx_SparkSheet
BattleFx_SparkSheet:
	.incbin "baserom.gba", 0x003cfc00, 0x000003d0
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "baserom.gba", 0x003cffd0, 0x000008a0
	.global BattleFx_FlameSheetA
BattleFx_FlameSheetA:
	.incbin "baserom.gba", 0x003d0870, 0x000004a4
	.global BattleFx_DemonFaceSheet
BattleFx_DemonFaceSheet:
	.incbin "baserom.gba", 0x003d0d14, 0x00000534
	.global BattleFx_FlameBladeSheet
BattleFx_FlameBladeSheet:
	.incbin "baserom.gba", 0x003d1248, 0x00000c84
	.global BattleFx_SwordSheet
BattleFx_SwordSheet:
	.incbin "baserom.gba", 0x003d1ecc, 0x00000458
	.global BattleFx_IceTileSheet
BattleFx_IceTileSheet:
	.incbin "baserom.gba", 0x003d2324, 0x0000106c
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "baserom.gba", 0x003d3390, 0x00000fc8
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "baserom.gba", 0x003d4358, 0x0000025c
	.global BattleFx_VioletLightningSheetA
BattleFx_VioletLightningSheetA:
	.incbin "baserom.gba", 0x003d45b4, 0x0000175c
	.global BattleFx_YellowSparkSheet
BattleFx_YellowSparkSheet:
	.incbin "baserom.gba", 0x003d5d10, 0x00000300
	.global BattleFx_ProjectileVolleyImage
BattleFx_ProjectileVolleyImage:
	.incbin "baserom.gba", 0x003d6010, 0x000001b4
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "baserom.gba", 0x003d61c4, 0x00000398
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
	.global BattleFx_VioletLightningSheetB
BattleFx_VioletLightningSheetB:
	.incbin "baserom.gba", 0x003d8a24, 0x00001bd4
	.global BattleFx_FireBurstSheet
BattleFx_FireBurstSheet:
	.incbin "baserom.gba", 0x003da5f8, 0x00001c00
	.global BattleFx_BlueCrescentSheet
BattleFx_BlueCrescentSheet:
	.incbin "baserom.gba", 0x003dc1f8, 0x00000220
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "baserom.gba", 0x003dc418, 0x0000043c
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "baserom.gba", 0x003dc854, 0x00000114
	.global BattleFx_OverlayImageB
BattleFx_OverlayImageB:
	.incbin "baserom.gba", 0x003dc968, 0x0000019c
	.global BattleFx_FireballSheet
BattleFx_FireballSheet:
	.incbin "baserom.gba", 0x003dcb04, 0x000005e0
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
	.incbin "baserom.gba", 0x0043f868, 0x00339920
	.section .unidentified.087fd4b9,"a"
	.incbin "baserom.gba", 0x007fd4b9, 0x00002b47
