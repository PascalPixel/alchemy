	.section .rodata
	.global BattleFx_WaterSparkleSheet
BattleFx_WaterSparkleSheet:
	.incbin "graphics/battle_fx/water_sparkle.gbapal"
	.incbin "graphics/battle_fx/water_sparkle.bitmap.lz"
	.byte 0x11	@ PACKER: left in the buffer by BattleFx_CrystalSheet+0x6b3
