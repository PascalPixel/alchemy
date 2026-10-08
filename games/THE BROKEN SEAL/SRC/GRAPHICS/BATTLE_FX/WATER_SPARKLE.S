	.section .rodata
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WATER_SPARKLE.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleFx_CrystalSheet+0x6b3
