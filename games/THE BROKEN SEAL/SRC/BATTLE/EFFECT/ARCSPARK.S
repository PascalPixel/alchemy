	.section .rodata
	.balign 4
@ 弧の火花 (32x16)
	.global BattleFx_SparkTiles
BattleFx_SparkTiles:
	.incbin "BATTLE/EFFECT/SPARK_A.4bpp32x16"
	.global BattleFx_ArcSparkTiles
BattleFx_ArcSparkTiles:
	.incbin "BATTLE/EFFECT/SPARK_B.4bpp32x16"
@ 遭遇する敵の組 104 個
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "BATTLE/EFFECT/ENCOUNT.table"
