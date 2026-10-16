	.section .rodata
@ 48 enemy elemental presets (same layout as TBS's gEnemy_ElementPresetTable).
	.global gData_080c6684
gData_080c6684:
	.incbin "games/tla/src/game/character/elempre.table"
	.include "games/common/src/game/element_curve.s"
@ The classes Battle_IsEffect90Class and Battle_IsEffect91Class look for.
	.global gBattle_Effect90Classes
gBattle_Effect90Classes:
	.2byte 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 143, 144, 145
	.2byte 149, 150, 151, 176, 177, 178, 185, 186, 187, 191, 192, 193, 194, 195, 196, 299
	.2byte 300, 301, 323, 324, 325, 357, 369, 370, 371, 372, 373, 374, 375, 376, 377
	.global gBattle_Effect91Classes
gBattle_Effect91Classes:
	.2byte 84, 85, 86, 87, 88, 89, 90, 101, 102, 103, 167, 168, 169, 170, 171, 172
	.2byte 173, 174, 175, 176, 177, 178, 179, 180, 181, 330, 331, 332, 348
