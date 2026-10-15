	.section .rodata
@ 遭遇する敵の組 110 個
	.global Encounter_EnemyGroupTable
Encounter_EnemyGroupTable:
	.incbin "games/tla/src/battle/effect/encounter.table"
