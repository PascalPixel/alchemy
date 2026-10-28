	.section .rodata
	.global BattleEffect_YellowSparkSheet
BattleEffect_YellowSparkSheet:
	.incbin "games/tla/src/graphics/battle_effects/yellow_spark_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/yellow_spark.parts.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x2fd
