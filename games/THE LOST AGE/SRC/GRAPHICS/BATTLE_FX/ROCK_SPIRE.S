@ BattleFx_RockSpireSheet: a 32x96 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RockSpireSheet
BattleFx_RockSpireSheet:
	.incbin "GRAPHICS/BATTLE_FX/ROCK_SPIRE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ROCK_SPIRE.bitmap.lz"
