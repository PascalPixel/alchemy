@ Reveal mask F: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_F.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_F.bitmap.lz"
