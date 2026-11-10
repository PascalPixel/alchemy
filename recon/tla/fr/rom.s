	.section .rom.00046b90, "ax"
	.global Func_08046b08
	.type Func_08046b08, %function
	.thumb_func
Func_08046b08:
	.incbin "baserom.gba", 0x00046b90, 0x00001c40
	.section .rom.001185c2, "ax"
	.incbin "baserom.gba", 0x001185c2, 0x00000002
	.section .rom.0013fd84, "ax"
	.global BattleEffect_RunCruelRuin
	.type BattleEffect_RunCruelRuin, %function
	.thumb_func
BattleEffect_RunCruelRuin:
	.incbin "baserom.gba", 0x0013fd84, 0x000017b8
	.section .rom.0015b764, "ax"
	.balign 4
	.global BattleEffect_RunUlysses
	.type BattleEffect_RunUlysses, %function
	.thumb_func
BattleEffect_RunUlysses:
	.incbin "baserom.gba", 0x0015b764, 0x000011f4
	.section .rom.0016729c, "ax"
	.global BattleEffect_RunJudgment
	.type BattleEffect_RunJudgment, %function
	.thumb_func
BattleEffect_RunJudgment:
	.incbin "baserom.gba", 0x0016729c, 0x000017a4
	.section .rom.0016aeec, "ax"
	.balign 4
	.global Func_0816aeec
Func_0816aeec:
	.incbin "baserom.gba", 0x0016aeec, 0x000007e8
	.section .rom.0017ea58, "ax"
	.global BattleEffect_RunDaedalus
	.type BattleEffect_RunDaedalus, %function
	.thumb_func
BattleEffect_RunDaedalus:
	.incbin "baserom.gba", 0x0017ea58, 0x0000223c
	.section .rom.0018cb74, "ax"
	.global BattleEffect_RunTrident
	.type BattleEffect_RunTrident, %function
	.thumb_func
BattleEffect_RunTrident:
	.incbin "baserom.gba", 0x0018cb74, 0x00000a1c
