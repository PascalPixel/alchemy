@ Reveal mask C: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_RevealMaskC
BattleFx_RevealMaskC:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_C.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_C.bitmap.lz"
