	.section .rodata
@ ワールドマップの遭遇 2 表: 鍵 2 個、フラグ (-1 は常に)、値。鍵 -1 で終わり
	.global gData_080eedbc
gData_080eedbc:
	.incbin "games/tla/src/battle/effect/world_encounters.table"
	.global gData_080eef34
gData_080eef34:
	.incbin "games/tla/src/battle/effect/world_encounters_2.table"
@ BattleFx_GetPhaseResult の引数 2 個、80 行
	.global gEncounter_AreaEntryTable
gEncounter_AreaEntryTable:
	.incbin "games/tla/src/battle/effect/encounter_areas.table"
