	.section .rodata
	.balign 4
@ The arc spark (32x16)
	.global gFieldFx_SparkTiles
gFieldFx_SparkTiles:
	.incbin "games/tbs/src/battle/effect/spark_a.4bpp32x16"
	.global gFieldFx_ArcSparkTiles
gFieldFx_ArcSparkTiles:
	.incbin "games/tbs/src/battle/effect/spark_b.4bpp32x16"
@ The 104 encounter zones (encounter.tsv)
	.global gEncounter_EnemyGroupTable
gEncounter_EnemyGroupTable:
	.incbin "games/tbs/src/object/encounter.table"
