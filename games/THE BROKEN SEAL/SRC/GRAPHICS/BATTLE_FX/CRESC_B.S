@ BattleFx_BlueCrescentSheet: a 24x48 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_BlueCrescentSheet
BattleFx_BlueCrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESC_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESC_B.bitmap.lz"
