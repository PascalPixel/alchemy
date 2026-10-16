	.section .rodata
@ 734 ability statuses (same layout as The Broken Seal's actstat.table).
	.global Data_08128972
Data_08128972:
	.incbin "games/tla/src/battle/actstat.table"
@ 733 ability effects (effect ID, stage, flags), like TBS's gBattle_ActionFlags.
	.global Data_08128c50
Data_08128c50:
	.incbin "games/tla/src/battle/actflags.table"
