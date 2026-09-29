@ BattleFx_ShieldSheet: a 40x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ShieldSheet
BattleFx_ShieldSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHIELD.bitmap.lz"
