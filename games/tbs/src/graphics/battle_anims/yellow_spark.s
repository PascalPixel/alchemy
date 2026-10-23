	.section .rodata
	.global BattleAnim_YellowSparkSheet
BattleAnim_YellowSparkSheet:
	.incbin "games/tbs/src/graphics/battle_anims/yellow_spark_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/yellow_spark.parts.lz"
	.byte 0x17, 0x6e, 0xa0	@ PACKER: left in the buffer by BattleAnim_VioletLightningSheetA+0x2fd
