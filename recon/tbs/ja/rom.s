	.section .rom.00322693, "ax"
	.incbin "baserom.gba", 0x00322693, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x00322694, 0x000086f8
	.section .rom.00600b20, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00600b20, 0x00002904
	.section .rom.0071012c, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x0071012c, 0x00001164
	.section .rom.00754368, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00754368, 0x00004604
