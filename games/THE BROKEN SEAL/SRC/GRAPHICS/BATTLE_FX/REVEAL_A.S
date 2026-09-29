@ Reveal mask A: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.bitmap.lz"
