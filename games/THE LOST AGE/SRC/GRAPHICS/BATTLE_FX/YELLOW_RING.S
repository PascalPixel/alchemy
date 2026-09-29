@ BattleFx_YellowRingSheet: a 40x72 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_YellowRingSheet
BattleFx_YellowRingSheet:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_RING.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_RING.bitmap.lz"
