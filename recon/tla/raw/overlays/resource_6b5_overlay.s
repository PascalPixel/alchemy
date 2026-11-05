	.set LOBBY_DE_ES_FR_IT, 0
	.ifdef TLA_EDITION_DE
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gData_020022b8
gData_020022b8:
	.space 0x00000004
	.global gData_020022bc
gData_020022bc:
