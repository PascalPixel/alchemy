	.section .rodata
	.balign 4
@ 弧の火花 (32x16)
	.global BattleFx_SparkTiles
BattleFx_SparkTiles:
	.incbin "games/common/src/battle/effect/spark_a.4bpp32x16"
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "games/common/src/battle/effect/spark_b.4bpp32x16"
@ 遭遇する敵の組 104 個
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "games/tbs/src/object/encounter.table"
