	.section .rodata
	.balign 4
@ The arc spark (32x16)
	.global gBattleFx_SparkTiles
gBattleFx_SparkTiles:
	.incbin "games/common/src/battle/effect/spark_a.4bpp32x16"
	.global gBattleFx_ArcSparkTiles
gBattleFx_ArcSparkTiles:
	.incbin "games/common/src/battle/effect/spark_b.4bpp32x16"
@ The 104 encounter zones (encounter.tsv)
	.global gEncounter_EnemyGroupTable
gEncounter_EnemyGroupTable:
	.incbin "games/tbs/src/object/encounter.table"
