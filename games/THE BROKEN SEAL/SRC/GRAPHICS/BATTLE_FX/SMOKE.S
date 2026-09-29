@ BattleFx_SmokeSheet: twelve 24x24 frames of smoke, its 64-colour palette and
@ then its pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SmokeSheet
BattleFx_SmokeSheet:
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SMOKE.bitmap.lz"
