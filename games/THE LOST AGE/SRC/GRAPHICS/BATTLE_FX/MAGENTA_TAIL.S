@ BattleFx_MagentaTailSheet: a 88x144 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MagentaTailSheet
BattleFx_MagentaTailSheet:
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_TAIL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_TAIL.bitmap.lz"
