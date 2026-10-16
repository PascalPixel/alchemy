	.section .rodata
@ Definitions of the 519 battle actions.
	.global gBattleAction_DefinitionTable
gBattleAction_DefinitionTable:
	.incbin "games/tbs/src/game/actions.table"
