@ Reveal mask E: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskE
BattleFx_RevealMaskE:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_E.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_E.bitmap.lz"
