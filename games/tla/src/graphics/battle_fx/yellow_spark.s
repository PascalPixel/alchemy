	.section .rodata
	.global BattleFx_YellowSparkSheet
BattleFx_YellowSparkSheet:
	.incbin "games/tla/src/graphics/battle_fx/yellow_spark_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/yellow_spark.parts.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleFx_VioletLightningSheetA+0x2fd
