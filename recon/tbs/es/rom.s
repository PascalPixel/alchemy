	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00007ac8, "ax"
	.incbin "baserom.gba", 0x00007ac8, 0x00000024
	.section .rom.00007b98, "ax"
	.incbin "baserom.gba", 0x00007b98, 0x00000004
	.section .rom.000290a4, "ax"
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
	.incbin "baserom.gba", 0x0002fa60, 0x00000800
	.section .rom.000a22c8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a22c8, 0x00000638
	.section .rom.000a2b48, "ax"
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
	.section .rom.000f3ae0, "ax"
	.incbin "baserom.gba", 0x000f3ae0, 0x00000755
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f4235, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f4298, 0x00000544
	.section .rom.000f68bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f68bc, 0x000000ad
	.section .rom.000fed04, "ax"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032c89c, 0x000086f8
	.section .rom.0033c3fc, "ax"
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c3fc, 0x0000071c
	.section .rom.005d8d22, "ax"
	.incbin "baserom.gba", 0x005d8d22, 0x00000002
	.section .rom.005ef13a, "ax"
	.incbin "baserom.gba", 0x005ef13a, 0x00000002
	.section .rom.0060ac10, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ac10, 0x00002904
	.section .rom.00692825, "ax"
	.incbin "baserom.gba", 0x00692825, 0x00000003
	.section .rom.006d6fe5, "ax"
	.incbin "baserom.gba", 0x006d6fe5, 0x00000003
	.section .rom.007192b4, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007192b4, 0x00001164
	.section .rom.0074701f, "ax"
	.incbin "baserom.gba", 0x0074701f, 0x00000001
	.section .rom.0075729c, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x0075729c, 0x000000e4
	.section .rom.0075d51c, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d51c, 0x00004600
