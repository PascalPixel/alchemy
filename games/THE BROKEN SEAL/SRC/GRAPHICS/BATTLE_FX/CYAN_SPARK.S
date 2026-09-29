@ BattleFx_CyanSparkSheet: the 64-colour palette of its first picture, then its 12 pictures
@ (listed in CYAN_SPARK.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_CyanSparkSheet
BattleFx_CyanSparkSheet:
	.incbin "GRAPHICS/BATTLE_FX/CYAN_SPARK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CYAN_SPARK.parts.lz"
