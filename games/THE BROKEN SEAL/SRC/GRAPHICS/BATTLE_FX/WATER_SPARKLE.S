@ BattleFx_WaterSparkleSheet: a 24x112 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.bitmap.lz"
