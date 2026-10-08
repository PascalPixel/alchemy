	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032ae9c, 0x000086f8
