	.section .rom.00322693, "ax"
	.incbin "baserom.gba", 0x00322693, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x00322694, 0x000086f8
	.section .rom.00754368, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00754368, 0x00004604
