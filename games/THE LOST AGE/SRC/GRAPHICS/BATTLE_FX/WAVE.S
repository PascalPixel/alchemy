@ BattleFx_WaveSheet: a 48x96 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_WaveSheet
BattleFx_WaveSheet:
	.incbin "GRAPHICS/BATTLE_FX/WAVE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WAVE.bitmap.lz"
