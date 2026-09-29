@ BattleFx_ImpFaceSheet: a 32x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ImpFaceSheet
BattleFx_ImpFaceSheet:
	.incbin "GRAPHICS/BATTLE_FX/IMP_FACE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/IMP_FACE.bitmap.lz"
