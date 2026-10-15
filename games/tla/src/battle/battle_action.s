	.section .rodata
@ 技ごとの状態 (The Broken Seal の actstat.table と同じ形、734 個)
	.global Data_08128972
Data_08128972:
	.incbin "games/tla/src/battle/actstat.table"
@ 技ごとの演出 (効果番号と段階, フラグ): The Broken Seal の gBattle_ActionFlags と同じ形、733 個
	.global Data_08128c50
Data_08128c50:
	.incbin "games/tla/src/battle/actflags.table"
