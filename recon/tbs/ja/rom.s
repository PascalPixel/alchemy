	.section .rom.00007a68, "ax"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .rom.00007b38, "ax"
	.incbin "baserom.gba", 0x00007b38, 0x00000004
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00029f3c, "ax"
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
	.incbin "baserom.gba", 0x000308ec, 0x00000800
	.section .rom.00094164, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x00094164, 0x00000638
	.section .rom.000949e4, "ax"
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
	.section .rom.000d9972, "ax"
	.incbin "baserom.gba", 0x000d9972, 0x00000002
	.section .rom.00322693, "ax"
	.incbin "baserom.gba", 0x00322693, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x00322694, 0x000086f8
	.section .rom.005cec4a, "ax"
	.incbin "baserom.gba", 0x005cec4a, 0x00000002
	.section .rom.005e5062, "ax"
	.incbin "baserom.gba", 0x005e5062, 0x00000002
	.section .rom.00600b20, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00600b20, 0x00002904
	.section .rom.00688735, "ax"
	.incbin "baserom.gba", 0x00688735, 0x00000003
	.section .rom.006ccef5, "ax"
	.incbin "baserom.gba", 0x006ccef5, 0x00000003
	.section .rom.0071012c, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x0071012c, 0x00001164
	.section .rom.0073de87, "ax"
	.incbin "baserom.gba", 0x0073de87, 0x00000001
	.section .rom.00754368, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00754368, 0x00004604
