	.section .rom.0032b09b, "ax"
	.incbin "baserom.gba", 0x0032b09b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b09c, 0x000086f8
	.section .rom.00609114, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609114, 0x00002904
	.section .rom.007177b8, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007177b8, 0x00001164
	.section .rom.0075ba20, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075ba20, 0x00004600
