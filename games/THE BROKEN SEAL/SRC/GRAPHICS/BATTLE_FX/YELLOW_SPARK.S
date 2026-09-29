@ BattleFx_YellowSparkSheet: the 64-colour palette of its first picture, then its 3 pictures
@ (listed in YELLOW_SPARK.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_YellowSparkSheet
BattleFx_YellowSparkSheet:
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_SPARK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/YELLOW_SPARK.parts.lz"
