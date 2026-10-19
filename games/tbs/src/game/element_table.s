	.section .rodata
	.balign 4
	.global gOwner_FourColumnValues
gOwner_FourColumnValues:
	.4byte 1, 3, 4, 5, 8, 6, 9, 10
	.4byte 4, 3, 2, 5, 8, 10, 9, 7
@ Elemental power and resistance at each elemental level.
	.global gElement_PowerResistByLevel
gElement_PowerResistByLevel:
	.2byte 75, 85
	.2byte 80, 90
	.2byte 85, 95
	.2byte 90, 100
	.2byte 95, 105
	.2byte 100, 110
	.2byte 105, 115
	.2byte 110, 120
	.2byte 115, 125
	.2byte 120, 130
	.2byte 125, 135
	.2byte 130, 140
	.2byte 135, 145
	.2byte 140, 150
	.2byte 145, 160
	.2byte 150, 165
@ The 44 enemy element presets.
	.global gEnemy_ElementPresetTable
gEnemy_ElementPresetTable:
	.incbin "games/tbs/src/game/elempre.table"
	.include "games/tbs/src/game/element_curve.s"
