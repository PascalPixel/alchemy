	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032c89c, 0x000086f8
	.section .rom.0060ac10, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ac10, 0x00002904
	.section .rom.007192b4, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007192b4, 0x00001164
	.section .rom.0075d51c, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d51c, 0x00004600
