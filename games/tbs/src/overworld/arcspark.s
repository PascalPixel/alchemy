	.section .rodata
	.balign 4
@ The arc spark (32x16)
	.global gFieldEffect_SparkTiles
gFieldEffect_SparkTiles:
	.incbin "games/tbs/src/battle/effect/spark_a.4bpp32x16"
	.global gFieldEffect_ArcSparkTiles
gFieldEffect_ArcSparkTiles:
	.incbin "games/tbs/src/battle/effect/spark_b.4bpp32x16"
@ The 104 encounter zones (encounter.tsv)
	.global gEncounterZones
gEncounterZones:
	.incbin "games/tbs/src/overworld/encounter.table"
