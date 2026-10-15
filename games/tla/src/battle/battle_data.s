@ Battle tables, laid out as The Broken Seal's battle_data.s, with The Lost Age's
@ three more classes and a second hit table.
	.section .rodata
	.balign 4
	.4byte 17
@ Party places from the centre out (The Broken Seal's gBattleParty_CenterOrderOffsets)
	.global gData_081287c8
gData_081287c8:
	.byte 0, -1, 1, -2, 2, -3, 3, -4, 4
	.balign 4
@ Weapon animations, one table each for classes 0, 1, 2, 3, 5, 4, 6 and 7
@ (The Broken Seal's gBattleUnit_WeaponAnimsClass0 and on)
	.global gData_081287d4
gData_081287d4:
	.2byte 0x1eb, 0x1ec, 0x1ed, 0x1ee, 0x1eb, 0x1eb, 0x1eb
	.global gData_081287e2
gData_081287e2:
	.2byte 0x1f2, 0x1f2, 0x1f1, 0x1f0, 0x1f0, 0x1f0, 0x1f0
	.global gData_081287f0
gData_081287f0:
	.2byte 0x1f7, 0x1f7, 0x1f7, 0x1f7, 0x1f5, 0x1f6, 0x1f5
	.global gData_081287fe
gData_081287fe:
	.2byte 0x1f9, 0x1f9, 0x1fb, 0x1fb, 0x1fa, 0x1f9, 0x1f9
	.global gData_0812880c
gData_0812880c:
	.2byte 0x204, 0x204, 0x204, 0x204, 0x202, 0x203, 0x202
	.global gData_0812881a
gData_0812881a:
	.2byte 0x1fd, 0x1fe, 0x1ff, 0x200, 0x1fd, 0x1fd, 0x1fd
	.global gData_08128828
gData_08128828:
	.2byte 0x208, 0x208, 0x208, 0x208, 0x206, 0x207, 0x206
	.global gData_08128836
gData_08128836:
	.2byte 0x20c, 0x20d, 0x20b, 0x20a, 0x20c, 0x20c, 0x20c
	.global gBattlePlacement_StepPairs
gBattlePlacement_StepPairs:
	.byte 80, 48
	.byte 80, 16
	.byte 80, -16
	.byte 80, -48
	.byte 0, 0
	.byte 80, 48
	.byte 80, 16
	.byte 80, -16
	.byte 80, -48
	.byte 0, 0
	.byte -80, 50
	.byte -80, 30
	.byte -80, 10
	.byte -80, -10
	.byte -80, -30
	.byte -80, -50
	.byte 0, 0
	.byte 0, 0
@ The fixed view matrix (The Broken Seal's gCamera_FlagTransformWork)
	.global gData_08128868
gData_08128868:
	.4byte 0x10000, 0x0, 0x0, -0xcccc
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
	.include "games/common/src/battle/battle_calc.s"
	.global Data_08128960
Data_08128960:
	.2byte 0xda, 0xdb, 0xdc, 0, 0, 0
@ A second hit rate by distance, the same as gHitTable
	.global Data_0812896c
Data_0812896c:
	.byte 100, 60, 30, 30, 30, 30
