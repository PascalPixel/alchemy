	.section .rodata
	.balign 4
@ Eight party members.
	.global gOwner_GrowthRecords
gOwner_GrowthRecords:
	.incbin "games/tla/src/game/character/chars.table"
