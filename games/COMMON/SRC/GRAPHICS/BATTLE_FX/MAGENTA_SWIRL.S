@ BattleFx_MagentaSwirlSheet: a 112x228 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MagentaSwirlSheet
BattleFx_MagentaSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MAGENTA_SWIRL.bitmap.lz"
