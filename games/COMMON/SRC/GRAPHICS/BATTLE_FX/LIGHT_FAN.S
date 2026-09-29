@ BattleFx_LightFanSheet: a 32x96 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_LightFanSheet
BattleFx_LightFanSheet:
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_FAN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/LIGHT_FAN.bitmap.lz"
