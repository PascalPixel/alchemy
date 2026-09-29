@ Reveal mask D: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskD
BattleFx_RevealMaskD:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_D.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_D.bitmap.lz"
