	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032ae9c, 0x000086f8
	.section .rom.0075c010, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075c010, 0x00004604
