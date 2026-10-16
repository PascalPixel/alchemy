	.section .rodata
@ Two world map encounter tables: two keys, flag (-1 always), value; key -1 ends.
	.global gData_080eedbc
gData_080eedbc:
	.incbin "games/tla/src/battle/effect/world_encounters.table"
	.global gData_080eef34
gData_080eef34:
	.incbin "games/tla/src/battle/effect/world_encounters_2.table"
@ 80 rows of two arguments for BattleFx_GetPhaseResult.
	.global gEncounter_AreaEntryTable
gEncounter_AreaEntryTable:
	.incbin "games/tla/src/battle/effect/encounter_areas.table"
