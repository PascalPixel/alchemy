@ BattleFx_HeartSheet: a 22x42 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_HeartSheet
BattleFx_HeartSheet:
	.incbin "GRAPHICS/BATTLE_FX/HEART.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/HEART.bitmap.lz"
