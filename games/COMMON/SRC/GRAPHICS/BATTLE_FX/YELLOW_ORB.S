@ BattleFx_YellowOrbSheet: a 40x288 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_YellowOrbSheet
BattleFx_YellowOrbSheet:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_ORB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_ORB.bitmap.lz"
