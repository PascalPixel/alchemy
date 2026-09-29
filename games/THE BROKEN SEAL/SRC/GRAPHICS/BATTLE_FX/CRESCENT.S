@ The crescent sheet: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.bitmap.lz"
