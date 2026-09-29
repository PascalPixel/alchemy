@ Reveal mask B: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskB
BattleFx_RevealMaskB:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.bitmap.lz"
