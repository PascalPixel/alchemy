@ BattleFx_RevealMaskD: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskD
BattleFx_RevealMaskD:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_D.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_D.bitmap.lz"
