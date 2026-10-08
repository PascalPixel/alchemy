	.section .rom.0032b09b, "ax"
	.incbin "baserom.gba", 0x0032b09b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b09c, 0x000086f8
	.section .rom.0075ba20, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075ba20, 0x00004600
