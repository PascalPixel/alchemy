@ BattleFx_RevealMaskE: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_E.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_E.bitmap.lz"
