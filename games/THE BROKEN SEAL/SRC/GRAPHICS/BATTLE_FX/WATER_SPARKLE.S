	.section .rodata
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CrystalSheet at +0x6b3 */
	.byte 0x11
