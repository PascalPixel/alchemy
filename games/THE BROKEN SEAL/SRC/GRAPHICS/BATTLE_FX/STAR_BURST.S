@ The star burst and slash sheet: two 120x120 frames, a 64-colour palette
@ and a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.bitmap.lz"
