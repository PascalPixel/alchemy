	.section .unidentified.08007a68,"a"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .unidentified.08007b38,"a"
	.incbin "baserom.gba", 0x00007b38, 0x00000004
	.section .unidentified.0809d170,"a"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d170, 0x00000638
	.section .unidentified.0809d9f0,"a"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x0009d9f0, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x0009ddd8, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x0009e1d8, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x0009e270, 0x00000218
	.section .unidentified.0832b0a6,"a"
	.incbin "baserom.gba", 0x0032b0a6, 0x00000002
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b0a8, 0x000086f8
	.section .unidentified.085d764a,"a"
	.incbin "baserom.gba", 0x005d764a, 0x00000002
	.section .unidentified.085eda62,"a"
	.incbin "baserom.gba", 0x005eda62, 0x00000002
	.section .unidentified.08609520,"a"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.section .unidentified.08691135,"a"
	.incbin "baserom.gba", 0x00691135, 0x00000003
	.section .unidentified.086d58f5,"a"
	.incbin "baserom.gba", 0x006d58f5, 0x00000003
	.section .unidentified.08717b8c,"a"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.section .unidentified.087458f7,"a"
	.incbin "baserom.gba", 0x007458f7, 0x00000001
	.section .unidentified.0875bdd8,"a"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
