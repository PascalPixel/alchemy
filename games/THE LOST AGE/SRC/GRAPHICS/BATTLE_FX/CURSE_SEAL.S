@ BattleFx_CurseSealSheet: a 128x128 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CurseSealSheet
BattleFx_CurseSealSheet:
	.incbin "GRAPHICS/BATTLE_FX/CURSE_SEAL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CURSE_SEAL.bitmap.lz"
