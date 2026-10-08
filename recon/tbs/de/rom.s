	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00007a98, "ax"
	.incbin "baserom.gba", 0x00007a98, 0x00000024
	.section .rom.00007b68, "ax"
	.incbin "baserom.gba", 0x00007b68, 0x00000004
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.00028ba8, "ax"
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00028ba8, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x00028fa4, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002cb4c, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002cdcc, 0x0000279c
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002f568, 0x00000800
	.section .rom.0007a770, "ax"
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "baserom.gba", 0x0007a770, 0x0000010d
	.section .rom.0007a886, "ax"
	.incbin "baserom.gba", 0x0007a886, 0x00000002
	.section .rom.000a08d8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a08d8, 0x00000638
	.section .rom.000a1158, "ax"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a1158, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x000a1540, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a1940, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a19d8, 0x00000218
	.section .rom.000e4772, "ax"
	.incbin "baserom.gba", 0x000e4772, 0x00000002
	.section .rom.000f20e0, "ax"
	.incbin "baserom.gba", 0x000f20e0, 0x00000759
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f2839, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f289c, 0x00000544
	.section .rom.000f4ed0, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f4ed0, 0x000000f4
	.section .rom.000fd304, "ax"
	.incbin "baserom.gba", 0x000fd304, 0x00000090
	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032ae9c, 0x000086f8
	.section .rom.005d783e, "ax"
	.incbin "baserom.gba", 0x005d783e, 0x00000002
	.section .rom.005edc56, "ax"
	.incbin "baserom.gba", 0x005edc56, 0x00000002
	.section .rom.00609718, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609718, 0x00002904
	.section .rom.0069132d, "ax"
	.incbin "baserom.gba", 0x0069132d, 0x00000003
	.section .rom.006d5aed, "ax"
	.incbin "baserom.gba", 0x006d5aed, 0x00000003
	.section .rom.00717db0, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717db0, 0x00001164
	.section .rom.00745b17, "ax"
	.incbin "baserom.gba", 0x00745b17, 0x00000001
	.section .rom.00755d90, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x00755d90, 0x000000e4
	.section .rom.0075c010, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075c010, 0x00004604
