	.section .rom.00007aa8, "ax"
	.incbin "baserom.gba", 0x00007aa8, 0x00000024
	.section .rom.00007b78, "ax"
	.incbin "baserom.gba", 0x00007b78, 0x00000004
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00072c78, "ax"
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "baserom.gba", 0x00072c78, 0x0000010d
	.section .rom.0009d2c8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d2c8, 0x00000638
	.section .rom.0009db48, "ax"
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
	.section .rom.000e2972, "ax"
	.incbin "baserom.gba", 0x000e2972, 0x00000002
	.section .rom.000f0ae0, "ax"
	.incbin "baserom.gba", 0x000f0ae0, 0x00000765
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f1245, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f12a8, 0x00000544
	.section .rom.0032b09b, "ax"
	.incbin "baserom.gba", 0x0032b09b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b09c, 0x000086f8
	.section .rom.005d7226, "ax"
	.incbin "baserom.gba", 0x005d7226, 0x00000002
	.section .rom.005ed63e, "ax"
	.incbin "baserom.gba", 0x005ed63e, 0x00000002
	.section .rom.00609114, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609114, 0x00002904
	.section .rom.00690d29, "ax"
	.incbin "baserom.gba", 0x00690d29, 0x00000003
	.section .rom.006d54e9, "ax"
	.incbin "baserom.gba", 0x006d54e9, 0x00000003
	.section .rom.007177b8, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007177b8, 0x00001164
	.section .rom.00745523, "ax"
	.incbin "baserom.gba", 0x00745523, 0x00000001
	.section .rom.007557a0, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x007557a0, 0x000000e4
	.section .rom.0075ba20, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075ba20, 0x00004600
