	.section .rodata
	.balign 4
@ 弧の火花 (32x16)
	.global gBattleFx_SparkTiles
gBattleFx_SparkTiles:
	.incbin "games/common/src/battle/effect/spark_a.4bpp32x16"
	.global gBattleFx_ArcSparkTiles
gBattleFx_ArcSparkTiles:
	.incbin "games/common/src/battle/effect/spark_b.4bpp32x16"
@ 遭遇する敵の組 104 個
	.global gEncounter_EnemyGroupTable
gEncounter_EnemyGroupTable:
	.incbin "games/tbs/src/object/encounter.table"
