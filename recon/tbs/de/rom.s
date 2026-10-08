	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032ae9c, 0x000086f8
	.section .rom.00609718, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609718, 0x00002904
	.section .rom.00717db0, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717db0, 0x00001164
	.section .rom.0075c010, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075c010, 0x00004604
