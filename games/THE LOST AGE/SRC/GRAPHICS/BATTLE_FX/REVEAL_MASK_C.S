@ BattleFx_RevealMaskC: a 120x240 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskC
BattleFx_RevealMaskC:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_C.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_MASK_C.bitmap.lz"
