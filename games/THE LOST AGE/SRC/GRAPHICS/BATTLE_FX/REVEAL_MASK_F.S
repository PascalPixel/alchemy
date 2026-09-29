@ BattleFx_RevealMaskF: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_F.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_F.bitmap.lz"
