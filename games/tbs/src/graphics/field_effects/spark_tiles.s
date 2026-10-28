	.section .rodata
	.balign 4
@ The arc spark (32x16)
	.global gFieldEffect_SparkTiles
gFieldEffect_SparkTiles:
	.incbin "games/tbs/src/graphics/field_effects/spark.4bpp32x16"
	.global gFieldEffect_ArcSparkTiles
gFieldEffect_ArcSparkTiles:
	.incbin "games/tbs/src/graphics/field_effects/arc_spark.4bpp32x16"
