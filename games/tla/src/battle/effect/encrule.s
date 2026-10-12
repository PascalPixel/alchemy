	.section .rodata
@ ワールドマップの遭遇 2 表: 鍵 2 個、フラグ (-1 は常に)、値。鍵 -1 で終わり
	.global Data_080eedbc
Data_080eedbc:
	.incbin "battle/effect/encworld.table"
	.global Data_080eef34
Data_080eef34:
	.incbin "battle/effect/encwor2.table"
@ BattleFx_GetPhaseResult の引数 2 個、80 行
	.global Encounter_AreaEntryTable
Encounter_AreaEntryTable:
	.incbin "battle/effect/encarea.table"
