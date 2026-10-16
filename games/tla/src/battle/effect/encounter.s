	.section .rodata
@ 110 enemy encounter groups.
	.global gEncounter_EnemyGroupTable
gEncounter_EnemyGroupTable:
	.incbin "games/tla/src/battle/effect/encounter.table"
