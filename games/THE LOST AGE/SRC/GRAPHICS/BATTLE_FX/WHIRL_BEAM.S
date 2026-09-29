@ BattleFx_WhirlBeamSheet: a 96x192 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_WhirlBeamSheet
BattleFx_WhirlBeamSheet:
	.incbin "GRAPHICS/BATTLE_FX/WHIRL_BEAM.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WHIRL_BEAM.bitmap.lz"
