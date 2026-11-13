	.section .rodata
@ Battle tables, laid out as The Broken Seal's battle_data.s, with The Lost Age's
@ three more classes and a second hit table.
	.balign 4
	.4byte 17
@ Party places from the centre out (The Broken Seal's gBattleParty_CenterOrderOffsets)
	.global gBattleParty_CenterOrderOffsets
gBattleParty_CenterOrderOffsets:
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
	.global gBattleUnit_WeaponAnimationsIsaac
gBattleUnit_WeaponAnimationsIsaac:
	.2byte 0x1f9, 0x1f9, 0x1fb, 0x1fb, 0x1fa, 0x1f9, 0x1f9
	.global gBattleUnit_WeaponAnimationsGaret
gBattleUnit_WeaponAnimationsGaret:
	.2byte 0x204, 0x204, 0x204, 0x204, 0x202, 0x203, 0x202
	.global gBattleUnit_WeaponAnimationsIvan
gBattleUnit_WeaponAnimationsIvan:
	.2byte 0x1fd, 0x1fe, 0x1ff, 0x200, 0x1fd, 0x1fd, 0x1fd
	.global gBattleUnit_WeaponAnimationsMia
gBattleUnit_WeaponAnimationsMia:
	.2byte 0x208, 0x208, 0x208, 0x208, 0x206, 0x207, 0x206
	.global gBattleUnit_WeaponAnimationsJenna
gBattleUnit_WeaponAnimationsJenna:
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
	.include "games/tla/src/battle/range_falloff.s"
	.global Data_08128960
Data_08128960:
	.2byte 0xda, 0xdb, 0xdc, 0, 0, 0
@ A second hit rate by distance, the same as gHitTable
	.global Data_0812896c
Data_0812896c:
	.byte 100, 60, 30, 30, 30, 30
@ 734 ability statuses (same layout as The Broken Seal's actstat.table).
	.global Data_08128972
Data_08128972:
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 9
	.byte 9
	.byte 9
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 9
	.byte 9
	.byte 9
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 8
	.byte 2
	.byte 8
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 8
	.byte 2
	.byte 8
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 8
	.byte 8
	.byte 2
	.byte 8
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 2
	.byte 2
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 10
	.byte 6
	.byte 6
	.byte 6
	.byte 10
	.byte 6
	.byte 0
	.byte 0
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 2
	.byte 2
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 2
	.byte 11
	.byte 6
	.byte 6
	.byte 0
	.byte 0
	.byte 6
	.byte 2
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 0
	.byte 0
	.byte 6
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 6
	.byte 2
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 6
	.byte 0
	.byte 0
	.byte 3
	.byte 5
	.byte 5
	.byte 5
	.byte 9
	.byte 9
	.byte 9
	.byte 0
	.byte 3
	.byte 5
	.byte 5
	.byte 5
	.byte 5
	.byte 9
	.byte 5
	.byte 0
	.byte 3
	.byte 5
	.byte 5
	.byte 5
	.byte 12
	.byte 5
	.byte 9
	.byte 5
	.byte 13
	.byte 0
	.byte 3
	.byte 5
	.byte 9
	.byte 5
	.byte 9
	.byte 9
	.byte 9
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 8
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 8
	.byte 2
	.byte 2
	.byte 2
	.byte 8
	.byte 2
	.byte 2
	.byte 8
	.byte 8
	.byte 8
	.byte 8
	.byte 6
	.byte 0
	.byte 0
	.byte 0
	.byte 8
	.byte 8
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 2
	.byte 8
	.byte 2
	.byte 8
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 8
	.byte 2
	.byte 8
	.byte 8
	.byte 2
	.byte 8
	.byte 2
	.byte 2
	.byte 8
	.byte 8
	.byte 8
	.byte 8
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 2
	.byte 8
	.byte 8
	.byte 8
	.byte 8
	.byte 2
	.byte 2
	.byte 9
	.byte 8
	.byte 2
	.byte 8
	.byte 2
	.byte 2
	.byte 8
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 15
	.byte 16
	.byte 15
	.byte 16
	.byte 15
	.byte 16
	.byte 15
	.byte 16
	.byte 16
	.byte 16
	.byte 15
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 16
	.byte 0
	.byte 15
	.byte 16
	.byte 16
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 6
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 9
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 2
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 9
	.byte 6
	.byte 0
	.byte 2
	.byte 6
	.byte 0
	.byte 6
	.byte 0
	.byte 6
	.byte 0
@ 733 ability effects (effect ID, stage, flags), like TBS's gBattle_ActionFlags.
	.global Data_08128c50
Data_08128c50:
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x001, 0x0
	.2byte 0x1001, 0x0
	.2byte 0x2001, 0x0
	.2byte 0x002, 0x0
	.2byte 0x1002, 0x0
	.2byte 0x2002, 0x0
	.2byte 0x003, 0x0
	.2byte 0x1003, 0x0
	.2byte 0x2003, 0x0
	.2byte 0x004, 0x0
	.2byte 0x1004, 0x0
	.2byte 0x2004, 0x0
	.2byte 0x005, 0x0
	.2byte 0x1005, 0x0
	.2byte 0x2005, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x006, 0x0
	.2byte 0x1006, 0x0
	.2byte 0x2006, 0x0
	.2byte 0x007, 0x0
	.2byte 0x1007, 0x0
	.2byte 0x2007, 0x0
	.2byte 0x008, 0x0
	.2byte 0x1008, 0x0
	.2byte 0x2008, 0x0
	.2byte 0x009, 0x0
	.2byte 0x1009, 0x0
	.2byte 0x2009, 0x0
	.2byte 0x00a, 0x0
	.2byte 0x100a, 0x0
	.2byte 0x200a, 0x0
	.2byte 0x04b, 0x0
	.2byte 0x104b, 0x0
	.2byte 0x204b, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x00b, 0x0
	.2byte 0x100b, 0x0
	.2byte 0x200b, 0x0
	.2byte 0x00c, 0x0
	.2byte 0x100c, 0x0
	.2byte 0x200c, 0x0
	.2byte 0x00d, 0x0
	.2byte 0x100d, 0x0
	.2byte 0x200d, 0x0
	.2byte 0x00e, 0x0
	.2byte 0x100e, 0x0
	.2byte 0x200e, 0x0
	.2byte 0x00f, 0x0
	.2byte 0x100f, 0x0
	.2byte 0x200f, 0x0
	.2byte 0x048, 0x0
	.2byte 0x1048, 0x0
	.2byte 0x2048, 0x0
	.2byte 0x049, 0x0
	.2byte 0x1049, 0x0
	.2byte 0x2049, 0x0
	.2byte 0x010, 0x0
	.2byte 0x1010, 0x0
	.2byte 0x2010, 0x0
	.2byte 0x011, 0x0
	.2byte 0x1011, 0x0
	.2byte 0x2011, 0x0
	.2byte 0x012, 0x0
	.2byte 0x1012, 0x0
	.2byte 0x2012, 0x0
	.2byte 0x013, 0x0
	.2byte 0x1013, 0x0
	.2byte 0x2013, 0x0
	.2byte 0x014, 0x0
	.2byte 0x1014, 0x0
	.2byte 0x2014, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x047, 0x0
	.2byte 0x1047, 0x0
	.2byte 0x2047, 0x0
	.2byte 0x015, 0x0
	.2byte 0x1015, 0x0
	.2byte 0x2015, 0x0
	.2byte 0x016, 0x0
	.2byte 0x1016, 0x0
	.2byte 0x2016, 0x0
	.2byte 0x017, 0x0
	.2byte 0x1017, 0x0
	.2byte 0x2017, 0x0
	.2byte 0x018, 0x0
	.2byte 0x1018, 0x0
	.2byte 0x019, 0x0
	.2byte 0x01a, 0x0
	.2byte 0x101a, 0x0
	.2byte 0x01b, 0x0
	.2byte 0x101b, 0x0
	.2byte 0x01c, 0x0
	.2byte 0x101c, 0x0
	.2byte 0x01d, 0x0
	.2byte 0x101d, 0x0
	.2byte 0x01e, 0x0
	.2byte 0x101e, 0x0
	.2byte 0x01f, 0x0
	.2byte 0x101f, 0x0
	.2byte 0x020, 0x0
	.2byte 0x020, 0x0
	.2byte 0x021, 0x0
	.2byte 0x1021, 0x0
	.2byte 0x000, 0x0
	.2byte 0x024, 0x0
	.2byte 0x025, 0x0
	.2byte 0x026, 0x0
	.2byte 0x027, 0x0
	.2byte 0x029, 0x0
	.2byte 0x028, 0x0
	.2byte 0x02a, 0x0
	.2byte 0x102a, 0x0
	.2byte 0x02b, 0x0
	.2byte 0x02c, 0x0
	.2byte 0x02d, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x031, 0x0
	.2byte 0x032, 0x0
	.2byte 0x033, 0x0
	.2byte 0x034, 0x0
	.2byte 0x003, 0x0
	.2byte 0x1003, 0x0
	.2byte 0x2003, 0x0
	.2byte 0x00d, 0x0
	.2byte 0x100d, 0x0
	.2byte 0x200d, 0x0
	.2byte 0x01a, 0x0
	.2byte 0x101a, 0x0
	.2byte 0x01c, 0x0
	.2byte 0x01c, 0x0
	.2byte 0x01e, 0x0
	.2byte 0x101e, 0x0
	.2byte 0x035, 0x0
	.2byte 0x036, 0x0
	.2byte 0x037, 0x0
	.2byte 0x038, 0x0
	.2byte 0x1038, 0x0
	.2byte 0x1038, 0x0
	.2byte 0x00f, 0x0
	.2byte 0x100f, 0x0
	.2byte 0x200f, 0x0
	.2byte 0x014, 0x0
	.2byte 0x1014, 0x0
	.2byte 0x2014, 0x0
	.2byte 0x012, 0x0
	.2byte 0x1012, 0x0
	.2byte 0x2012, 0x0
	.2byte 0x021, 0x0
	.2byte 0x03a, 0x0
	.2byte 0x039, 0x0
	.2byte 0x03b, 0x0
	.2byte 0x1044, 0x8
	.2byte 0x03c, 0x0
	.2byte 0x053, 0x0
	.2byte 0x07d, 0x8
	.2byte 0x07b, 0x8
	.2byte 0x055, 0x0
	.2byte 0x056, 0x0
	.2byte 0x080, 0x0
	.2byte 0x054, 0x0
	.2byte 0x1035, 0x0
	.2byte 0x1031, 0x0
	.2byte 0x07f, 0x0
	.2byte 0x1033, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x408a, 0x0
	.2byte 0x408b, 0x0
	.2byte 0x408c, 0x0
	.2byte 0x408d, 0x0
	.2byte 0x408e, 0x0
	.2byte 0x408f, 0x0
	.2byte 0x4090, 0x0
	.2byte 0x03f, 0x0
	.2byte 0x4092, 0x0
	.2byte 0x042, 0x0
	.2byte 0x4094, 0x0
	.2byte 0x4095, 0x0
	.2byte 0x4096, 0x0
	.2byte 0x4097, 0x0
	.2byte 0x4098, 0x0
	.2byte 0x4099, 0x0
	.2byte 0x040, 0x0
	.2byte 0x409b, 0x0
	.2byte 0x041, 0x0
	.2byte 0x409d, 0x0
	.2byte 0x409e, 0x0
	.2byte 0x409f, 0x0
	.2byte 0x40a0, 0x0
	.2byte 0x40a1, 0x0
	.2byte 0x136, 0x0
	.2byte 0x136, 0x0
	.2byte 0x40a2, 0x0
	.2byte 0x03d, 0x0
	.2byte 0x03e, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x016, 0x0
	.2byte 0x016, 0x0
	.2byte 0x016, 0x0
	.2byte 0x016, 0x0
	.2byte 0x016, 0x0
	.2byte 0x0d4, 0x0
	.2byte 0x018, 0x0
	.2byte 0x1018, 0x0
	.2byte 0x019, 0x0
	.2byte 0x2017, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x17c, 0x0
	.2byte 0x17c, 0x0
	.2byte 0x0d4, 0x0
	.2byte 0x015, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x0f9, 0x0
	.2byte 0x4068, 0x0
	.2byte 0x0c8, 0x0
	.2byte 0x0c9, 0x0
	.2byte 0x0ca, 0x0
	.2byte 0x406b, 0x0
	.2byte 0x0cb, 0x0
	.2byte 0x4064, 0x0
	.2byte 0x406c, 0x0
	.2byte 0x0dc, 0x0
	.2byte 0x4070, 0x0
	.2byte 0x0ed, 0x0
	.2byte 0x0e1, 0x0
	.2byte 0x0fb, 0x0
	.2byte 0x0ef, 0x0
	.2byte 0x0e2, 0x0
	.2byte 0x0fd, 0x0
	.2byte 0x0fa, 0x0
	.2byte 0x0f4, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x0cc, 0x0
	.2byte 0x0cd, 0x0
	.2byte 0x4066, 0x0
	.2byte 0x0ce, 0x0
	.2byte 0x0cf, 0x0
	.2byte 0x0d0, 0x0
	.2byte 0x0d1, 0x0
	.2byte 0x4071, 0x0
	.2byte 0x406d, 0x0
	.2byte 0x0e3, 0x0
	.2byte 0x0de, 0x0
	.2byte 0x406e, 0x0
	.2byte 0x0dd, 0x0
	.2byte 0x0f0, 0x0
	.2byte 0x406f, 0x0
	.2byte 0x0f5, 0x0
	.2byte 0x0e6, 0x0
	.2byte 0x0fe, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x0d2, 0x0
	.2byte 0x4067, 0x0
	.2byte 0x0d3, 0x0
	.2byte 0x4069, 0x0
	.2byte 0x0d4, 0x0
	.2byte 0x0d5, 0x0
	.2byte 0x0d6, 0x0
	.2byte 0x0f6, 0x0
	.2byte 0x0ff, 0x0
	.2byte 0x0df, 0x0
	.2byte 0x101, 0x0
	.2byte 0x0e0, 0x0
	.2byte 0x0f8, 0x0
	.2byte 0x0ec, 0x0
	.2byte 0x0e7, 0x0
	.2byte 0x0fc, 0x0
	.2byte 0x100, 0x0
	.2byte 0x0f3, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x0db, 0x0
	.2byte 0x0d7, 0x0
	.2byte 0x0d8, 0x0
	.2byte 0x4065, 0x0
	.2byte 0x0d9, 0x0
	.2byte 0x406a, 0x0
	.2byte 0x0da, 0x0
	.2byte 0x0e4, 0x0
	.2byte 0x4075, 0x0
	.2byte 0x0e5, 0x0
	.2byte 0x0ea, 0x0
	.2byte 0x0ee, 0x0
	.2byte 0x4074, 0x0
	.2byte 0x0f7, 0x0
	.2byte 0x0e8, 0x0
	.2byte 0x0f2, 0x0
	.2byte 0x0f1, 0x0
	.2byte 0x0e9, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x118, 0x4
	.2byte 0x8001, 0x0
	.2byte 0x8005, 0x0
	.2byte 0x8009, 0x0
	.2byte 0x800d, 0x0
	.2byte 0x8012, 0x0
	.2byte 0x8018, 0x0
	.2byte 0x000, 0x0
	.2byte 0x119, 0x4
	.2byte 0x8002, 0x0
	.2byte 0x8006, 0x0
	.2byte 0x800a, 0x0
	.2byte 0x8010, 0x0
	.2byte 0x8014, 0x0
	.2byte 0x8016, 0x0
	.2byte 0x000, 0x0
	.2byte 0x11a, 0x4
	.2byte 0x8003, 0x0
	.2byte 0x8007, 0x0
	.2byte 0x800b, 0x0
	.2byte 0x800e, 0x0
	.2byte 0x8011, 0x0
	.2byte 0x8015, 0x0
	.2byte 0x801a, 0x0
	.2byte 0x8019, 0x0
	.2byte 0x000, 0x0
	.2byte 0x11b, 0x4
	.2byte 0x8004, 0x0
	.2byte 0x8008, 0x0
	.2byte 0x800c, 0x0
	.2byte 0x800f, 0x0
	.2byte 0x8013, 0x0
	.2byte 0x8017, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x190, 0x8
	.2byte 0x190, 0x8
	.2byte 0x190, 0x8
	.2byte 0x191, 0x8
	.2byte 0x191, 0x8
	.2byte 0x191, 0x8
	.2byte 0x192, 0x0
	.2byte 0x192, 0x0
	.2byte 0x192, 0x0
	.2byte 0x193, 0x8
	.2byte 0x193, 0x8
	.2byte 0x194, 0x8
	.2byte 0x194, 0x8
	.2byte 0x195, 0x8
	.2byte 0x1d7, 0x8
	.2byte 0x12c, 0x0
	.2byte 0x12c, 0x0
	.2byte 0x12d, 0x0
	.2byte 0x12e, 0x0
	.2byte 0x12f, 0x0
	.2byte 0x130, 0x0
	.2byte 0x131, 0x0
	.2byte 0x132, 0x0
	.2byte 0x133, 0x0
	.2byte 0x17d, 0x0
	.2byte 0x196, 0x8
	.2byte 0x197, 0x8
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x134, 0x0
	.2byte 0x135, 0x0
	.2byte 0x136, 0x0
	.2byte 0x137, 0x0
	.2byte 0x138, 0x0
	.2byte 0x139, 0x0
	.2byte 0x13a, 0x0
	.2byte 0x13b, 0x0
	.2byte 0x13c, 0x0
	.2byte 0x13d, 0x0
	.2byte 0x13e, 0x0
	.2byte 0x13f, 0x0
	.2byte 0x140, 0x0
	.2byte 0x141, 0x0
	.2byte 0x142, 0x0
	.2byte 0x143, 0x0
	.2byte 0x144, 0x0
	.2byte 0x145, 0x0
	.2byte 0x146, 0x0
	.2byte 0x147, 0x0
	.2byte 0x148, 0x0
	.2byte 0x149, 0x0
	.2byte 0x14a, 0x0
	.2byte 0x40a3, 0x0
	.2byte 0x14c, 0x0
	.2byte 0x14d, 0x0
	.2byte 0x14e, 0x0
	.2byte 0x14f, 0x0
	.2byte 0x150, 0x0
	.2byte 0x151, 0x0
	.2byte 0x152, 0x0
	.2byte 0x153, 0x0
	.2byte 0x154, 0x0
	.2byte 0x40a4, 0x0
	.2byte 0x156, 0x0
	.2byte 0x157, 0x0
	.2byte 0x158, 0x0
	.2byte 0x159, 0x0
	.2byte 0x15a, 0x0
	.2byte 0x15b, 0x0
	.2byte 0x15c, 0x0
	.2byte 0x15d, 0x0
	.2byte 0x15e, 0x0
	.2byte 0x15f, 0x0
	.2byte 0x160, 0x0
	.2byte 0x161, 0x0
	.2byte 0x162, 0x0
	.2byte 0x185, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x17e, 0x0
	.2byte 0x17f, 0x0
	.2byte 0x4098, 0x0
	.2byte 0x408d, 0x0
	.2byte 0x180, 0x8
	.2byte 0x181, 0x8
	.2byte 0x182, 0x0
	.2byte 0x183, 0x0
	.2byte 0x184, 0x0
	.2byte 0x000, 0x0
	.2byte 0x064, 0x0
	.2byte 0x40b9, 0x0
	.2byte 0x40ac, 0x0
	.2byte 0x40ad, 0x0
	.2byte 0x065, 0x0
	.2byte 0x40b3, 0x0
	.2byte 0x40c2, 0x0
	.2byte 0x067, 0x0
	.2byte 0x045, 0x0
	.2byte 0x07e, 0x0
	.2byte 0x068, 0x0
	.2byte 0x07c, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x069, 0x0
	.2byte 0x06a, 0x0
	.2byte 0x40a8, 0x0
	.2byte 0x40a9, 0x0
	.2byte 0x40aa, 0x0
	.2byte 0x40b4, 0x0
	.2byte 0x40ab, 0x0
	.2byte 0x06b, 0x0
	.2byte 0x40b6, 0x0
	.2byte 0x06c, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x40a6, 0x0
	.2byte 0x06d, 0x0
	.2byte 0x40a7, 0x0
	.2byte 0x06f, 0x0
	.2byte 0x070, 0x0
	.2byte 0x40b2, 0x0
	.2byte 0x071, 0x0
	.2byte 0x40b5, 0x0
	.2byte 0x40ae, 0x0
	.2byte 0x072, 0x0
	.2byte 0x073, 0x0
	.2byte 0x06e, 0x0
	.2byte 0x081, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x40af, 0x0
	.2byte 0x40b0, 0x0
	.2byte 0x074, 0x0
	.2byte 0x075, 0x0
	.2byte 0x076, 0x0
	.2byte 0x077, 0x0
	.2byte 0x40b1, 0x0
	.2byte 0x40b8, 0x0
	.2byte 0x801b, 0x0
	.2byte 0x078, 0x0
	.2byte 0x40ba, 0x0
	.2byte 0x079, 0x0
	.2byte 0x40b7, 0x0
	.2byte 0x40bc, 0x0
	.2byte 0x07a, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x04c, 0x0
	.2byte 0x04d, 0x0
	.2byte 0x190, 0x0
	.2byte 0x04e, 0x0
	.2byte 0x104e, 0x0
	.2byte 0x204e, 0x0
	.2byte 0x04f, 0x0
	.2byte 0x059, 0x0
	.2byte 0x058, 0x0
	.2byte 0x057, 0x0
	.2byte 0x05a, 0x0
	.2byte 0x05b, 0x0
	.2byte 0x05c, 0x0
	.2byte 0x05d, 0x0
	.2byte 0x000, 0x0
	.2byte 0x050, 0x0
	.2byte 0x40c4, 0x0
	.2byte 0x1be, 0x8
	.2byte 0x40c3, 0x0
	.2byte 0x190, 0x8
	.2byte 0x40c3, 0x0
	.2byte 0x12d, 0x0
	.2byte 0x40a3, 0x0
	.2byte 0x190, 0x8
	.2byte 0x190, 0x8
	.2byte 0x016, 0x0
	.2byte 0x40c5, 0x0
	.2byte 0x14f, 0x0
	.2byte 0x1c9, 0x0
	.2byte 0x190, 0x8
	.2byte 0x192, 0x8
	.2byte 0x016, 0x0
	.2byte 0x1017, 0x0
	.2byte 0x019, 0x0
	.2byte 0x13a, 0x0
	.2byte 0x1c9, 0x0
	.2byte 0x1c1, 0x8
	.2byte 0x190, 0x8
	.2byte 0x190, 0x8
	.2byte 0x016, 0x0
	.2byte 0x1016, 0x0
	.2byte 0x1017, 0x0
	.2byte 0x2047, 0x0
	.2byte 0x1b4, 0x0
	.2byte 0x000, 0x0
	.2byte 0x40c7, 0x0
	.2byte 0x21be, 0x8
	.2byte 0x1c0, 0x8
	.2byte 0x04a, 0x0
	.2byte 0x104a, 0x0
	.2byte 0x204a, 0x0
	.2byte 0x051, 0x0
	.2byte 0x052, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x198, 0x8
	.2byte 0x199, 0x8
	.2byte 0x19a, 0x8
	.2byte 0x19c, 0x8
	.2byte 0x19b, 0x8
	.2byte 0x19e, 0x8
	.2byte 0x19d, 0x8
	.2byte 0x19f, 0x8
	.2byte 0x1a0, 0x8
	.2byte 0x191, 0x8
	.2byte 0x1a0, 0x8
	.2byte 0x1d6, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x000, 0x0
	.2byte 0x40bd, 0x0
	.2byte 0x1c5, 0x0
	.2byte 0x1c6, 0x0
	.2byte 0x1c7, 0x0
	.2byte 0x1a9, 0x0
	.2byte 0x139, 0x0
	.2byte 0x186, 0x8
	.2byte 0x1d3, 0x0
	.2byte 0x1aa, 0x0
	.2byte 0x188, 0x0
	.2byte 0x1ae, 0x0
	.2byte 0x18b, 0x0
	.2byte 0x18a, 0x0
	.2byte 0x1c4, 0x0
	.2byte 0x1ca, 0x0
	.2byte 0x1bf, 0x0
	.2byte 0x187, 0x0
	.2byte 0x18c, 0x0
	.2byte 0x000, 0x0
	.2byte 0x40c1, 0x0
	.2byte 0x1d5, 0x0
	.2byte 0x1b2, 0x8
	.2byte 0x1c2, 0x0
	.2byte 0x801d, 0x0
	.2byte 0x1b3, 0x0
	.2byte 0x1b1, 0x0
	.2byte 0x18d, 0x0
	.2byte 0x18e, 0x0
	.2byte 0x40c4, 0x0
	.2byte 0x18f, 0x0
	.2byte 0x40c0, 0x0
	.2byte 0x1ab, 0x0
	.2byte 0x1a5, 0x0
	.2byte 0x1be, 0x8
	.2byte 0x11be, 0x8
	.2byte 0x1a8, 0x0
	.2byte 0x1a7, 0x0
	.2byte 0x1a6, 0x0
	.2byte 0x40c5, 0x0
	.2byte 0x40c6, 0x0
	.2byte 0x1b8, 0x0
	.2byte 0x1b9, 0x0
	.2byte 0x1ba, 0x0
	.2byte 0x1bb, 0x0
	.2byte 0x1bc, 0x0
	.2byte 0x1ad, 0x0
	.2byte 0x1ac, 0x0
	.2byte 0x189, 0x0
	.2byte 0x1b4, 0x0
	.2byte 0x1b0, 0x8
	.2byte 0x1c9, 0x0
	.2byte 0x1af, 0x0
	.2byte 0x21be, 0x8
	.2byte 0x1b5, 0x0
	.2byte 0x1bd, 0x8
	.2byte 0x40bf, 0x0
	.2byte 0x1c1, 0x8
	.2byte 0x1c0, 0x8
	.2byte 0x1c3, 0x0
	.2byte 0x40be, 0x0
	.2byte 0x1cd, 0x0
	.2byte 0x1ce, 0x0
	.2byte 0x1cb, 0x8
	.2byte 0x1cc, 0x0
	.2byte 0x801c, 0x0
	.2byte 0x1d2, 0x0
	.2byte 0x1cf, 0x0
	.2byte 0x152, 0x0
	.2byte 0x1d1, 0x0
	.2byte 0x000, 0x0
	.2byte 0x1d0, 0x0
	.2byte 0x2016, 0x0
	.2byte 0x1d4, 0x0
@ A copy of The Broken Seal's gBattleParty_TurnEndSideOrder
	.global gBattleParty_TurnEndSideOrder
gBattleParty_TurnEndSideOrder:
	.4byte 1, 2
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
@ A copy of The Broken Seal's gElementGlow_StarWorkOffsets
	.global gElementGlow_StarPictures
gElementGlow_StarPictures:
	.4byte 0, 1, 5, 14, 30, 46, 62
	.global gElementGlow_StarSizes
gElementGlow_StarSizes:
	.byte 1, 2, 3, 4, 4, 4, 4, 0
	.global Data_08129830
Data_08129830:
	.4byte 0x04e, 0x44e, 0x84e, 0xc4e
@ Two lists of pairs, each terminated by -1.
	.2byte 0, 0
	.2byte 1, 1
	.2byte 2, 2
	.2byte 3, 3
	.2byte 4, 4
	.2byte 5, 5
	.2byte 6, 6
	.2byte 7, 8
	.2byte -1, -1
	.2byte 246, 0
	.2byte 243, 1
	.2byte 245, 2
	.2byte 244, 3
	.2byte 201, 14
	.2byte 202, 15
	.2byte 205, 16
	.2byte 206, 17
	.2byte 198, 18
	.2byte 209, 19
	.2byte 199, 20
	.2byte 108, 54
	.2byte 109, 55
	.2byte 110, 56
	.2byte 134, 57
	.2byte 135, 58
	.2byte 130, 59
	.2byte 111, 60
	.2byte 56, 4
	.2byte 55, 4
	.2byte 57, 7
	.2byte 21, 30
	.2byte 18, 31
	.2byte 17, 32
	.2byte 20, 33
	.2byte 19, 34
	.2byte 24, 35
	.2byte 25, 36
	.2byte 26, 37
	.2byte 27, 37
	.2byte 41, 38
	.2byte 42, 39
	.2byte 60, 40
	.2byte 61, 41
	.2byte 16, 42
	.2byte 37, 43
	.2byte 38, 44
	.2byte 36, 45
	.2byte 29, 46
	.2byte 30, 47
	.2byte 32, 48
	.2byte 31, 49
	.2byte 34, 50
	.2byte 23, 51
	.2byte 33, 52
	.2byte 62, 9
	.2byte 43, 61
	.2byte 44, 62
	.2byte 46, 63
	.2byte 45, 64
	.2byte 104, 65
	.2byte 105, 66
	.2byte 129, 67
	.2byte 121, 68
	.2byte 119, 69
	.2byte 123, 70
	.2byte 63, 73
	.2byte 59, 74
	.2byte 66, 8
	.2byte 65, 10
	.2byte 64, 13
	.2byte 67, 71
	.2byte 68, 72
	.2byte 70, 11
	.2byte 69, 76
	.2byte -1, -1
	.section .rodata.a
	.global gScriptedBattle_Units
gScriptedBattle_Units:
	.byte 4, 7, 5, 0x80, 0x81
	.balign 4, 0
@ Debug party members (separated by owner -1).
@ The Broken Seal's table of this name, with six item columns for its four
	.global gDebug_ParameterSets
gDebug_ParameterSets:
	.byte 4, 90, 9, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 90, 0, 3, 3, 3; .2byte 1, 90, 127, 443, 0, 0, 140, 0, 0
	.byte 7, 90, 3, 0, 3, 3; .2byte 1, 90, 127, 444, 0, 0, 0, 0, 0
	.byte 5, 90, 3, 3, 0, 3; .2byte 55, 90, 127, 445, 0, 0, 0, 0, 0
	.byte 6, 90, 0, 0, 0, 9; .2byte 321, 104, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 50, 0, 0, 9, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 50, 9, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 50, 0, 9, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 50, 0, 0, 0, 9; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 90, 9, 0, 0, 0; .2byte 9, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 90, 0, 9, 0, 0; .2byte 276, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 90, 0, 0, 9, 0; .2byte 293, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 90, 0, 0, 0, 9; .2byte 322, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 90, 0, 0, 0, 0; .2byte 1, 90, 127, 444, 0, 0, 140, 0, 0
	.byte 7, 90, 1, 0, 1, 1; .2byte 1, 90, 127, 444, 0, 0, 0, 0, 0
	.byte 5, 90, 2, 2, 0, 2; .2byte 55, 90, 127, 444, 0, 0, 0, 0, 0
	.byte 6, 90, 3, 3, 3, 0; .2byte 55, 104, 127, 444, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 5, 0, 0, 0, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 5, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 5, 0, 0, 0, 0; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 6, 0, 0, 0, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 6, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 6, 0, 1, 0, 0; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 7, 0, 1, 0, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 7, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 7, 0, 0, 0, 1; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 8, 0, 1, 0, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 8, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 8, 0, 0, 0, 1; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 9, 0, 1, 1, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 9, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 9, 0, 0, 0, 1; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 10, 0, 1, 1, 0; .2byte 16, 90, 136, 166, 402, 0, 0, 0, 0
	.byte 4, 10, 1, 0, 0, 0; .2byte 31, 90, 118, 156, 402, 0, 140, 0, 0
	.byte 6, 10, 0, 0, 0, 1; .2byte 56, 90, 136, 166, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 11, 0, 1, 1, 0; .2byte 16, 90, 136, 166, 402, 0, 0, 0, 0
	.byte 4, 11, 1, 0, 0, 0; .2byte 31, 90, 118, 156, 402, 0, 140, 0, 0
	.byte 6, 11, 0, 0, 0, 1; .2byte 56, 90, 136, 166, 402, 414, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 12, 0, 1, 1, 0; .2byte 17, 104, 128, 166, 402, 0, 0, 0, 0
	.byte 4, 12, 2, 0, 0, 0; .2byte 2, 75, 118, 145, 402, 0, 140, 0, 0
	.byte 6, 12, 0, 0, 0, 2; .2byte 44, 104, 128, 166, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 13, 0, 1, 1, 0; .2byte 17, 92, 128, 157, 402, 0, 0, 0, 0
	.byte 4, 13, 2, 0, 0, 0; .2byte 2, 92, 119, 145, 402, 0, 140, 0, 0
	.byte 6, 13, 0, 0, 0, 3; .2byte 57, 92, 128, 157, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 14, 0, 1, 1, 0; .2byte 17, 92, 128, 157, 402, 0, 0, 0, 0
	.byte 4, 14, 2, 0, 0, 0; .2byte 2, 92, 119, 145, 402, 0, 140, 0, 0
	.byte 6, 14, 0, 0, 0, 3; .2byte 57, 92, 128, 157, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 15, 0, 1, 2, 0; .2byte 58, 92, 137, 157, 402, 0, 0, 0, 0
	.byte 4, 15, 2, 0, 0, 0; .2byte 32, 76, 119, 146, 402, 0, 140, 0, 0
	.byte 6, 15, 0, 0, 0, 3; .2byte 57, 92, 137, 157, 404, 414, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 16, 0, 1, 2, 0; .2byte 58, 92, 137, 157, 402, 0, 0, 0, 0
	.byte 4, 16, 2, 0, 0, 0; .2byte 32, 76, 119, 146, 402, 0, 140, 0, 0
	.byte 6, 16, 0, 0, 0, 3; .2byte 57, 92, 137, 157, 404, 414, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 17, 0, 1, 2, 0; .2byte 58, 92, 137, 157, 402, 0, 0, 0, 0
	.byte 4, 17, 2, 0, 0, 0; .2byte 32, 76, 119, 146, 402, 0, 140, 0, 0
	.byte 6, 17, 0, 0, 0, 3; .2byte 57, 92, 137, 157, 404, 414, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 18, 0, 2, 1, 0; .2byte 18, 105, 137, 167, 402, 0, 0, 0, 0
	.byte 4, 18, 2, 0, 1, 0; .2byte 18, 76, 120, 146, 402, 0, 140, 0, 0
	.byte 6, 18, 0, 0, 0, 3; .2byte 59, 105, 137, 167, 404, 414, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 19, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 19, 2, 0, 1, 0; .2byte 3, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 19, 0, 4, 0, 0; .2byte 3, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 19, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 20, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 20, 2, 0, 1, 0; .2byte 3, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 20, 0, 4, 0, 0; .2byte 3, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 20, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 21, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 21, 2, 0, 1, 0; .2byte 3, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 21, 0, 4, 0, 0; .2byte 3, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 21, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 22, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 22, 2, 0, 1, 0; .2byte 3, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 22, 0, 4, 0, 0; .2byte 3, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 22, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 23, 0, 0, 4, 0; .2byte 60, 107, 138, 158, 402, 0, 0, 0, 0
	.byte 4, 23, 4, 0, 0, 0; .2byte 3, 77, 129, 158, 402, 0, 140, 0, 0
	.byte 7, 23, 0, 5, 0, 0; .2byte 3, 77, 129, 158, 402, 0, 0, 0, 0
	.byte 6, 23, 0, 0, 0, 4; .2byte 59, 107, 138, 158, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 24, 0, 0, 4, 0; .2byte 61, 107, 138, 168, 402, 0, 0, 0, 0
	.byte 4, 24, 5, 0, 0, 0; .2byte 34, 78, 121, 148, 402, 0, 140, 0, 0
	.byte 7, 24, 0, 5, 0, 0; .2byte 4, 78, 121, 148, 402, 0, 0, 0, 0
	.byte 6, 24, 0, 0, 0, 5; .2byte 60, 107, 138, 168, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 25, 0, 1, 4, 0; .2byte 20, 94, 138, 169, 402, 0, 0, 0, 0
	.byte 4, 25, 5, 0, 0, 0; .2byte 5, 78, 121, 148, 402, 0, 140, 0, 0
	.byte 7, 25, 0, 5, 0, 0; .2byte 34, 78, 121, 148, 402, 0, 0, 0, 0
	.byte 6, 25, 0, 0, 0, 5; .2byte 46, 94, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 26, 0, 1, 4, 0; .2byte 20, 94, 138, 169, 402, 0, 0, 0, 0
	.byte 4, 26, 5, 0, 0, 0; .2byte 5, 78, 121, 148, 402, 0, 140, 0, 0
	.byte 7, 26, 0, 5, 0, 0; .2byte 34, 78, 121, 148, 402, 0, 0, 0, 0
	.byte 6, 26, 0, 0, 0, 5; .2byte 46, 94, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 27, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 27, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 0, 140, 0, 0
	.byte 7, 27, 0, 6, 0, 0; .2byte 34, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 27, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 28, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 28, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 0, 140, 0, 0
	.byte 7, 28, 0, 6, 0, 0; .2byte 34, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 28, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 29, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 29, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 326, 140, 0, 0
	.byte 7, 29, 0, 6, 0, 0; .2byte 34, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 29, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 30, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 30, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 326, 140, 0, 0
	.byte 7, 30, 0, 6, 0, 0; .2byte 34, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 30, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 31, 0, 0, 6, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 31, 6, 0, 0, 0; .2byte 21, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 31, 0, 7, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 31, 0, 0, 0, 6; .2byte 47, 109, 131, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 32, 0, 0, 7, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 32, 7, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 32, 0, 7, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 32, 0, 0, 0, 7; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 0, 0, 0, 0; .2byte 1, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 1, 1, 0, 0, 0, 0; .2byte 31, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 2, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 141, 0, 0
	.byte 3, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 0, 0, 0, 0; .2byte 1, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 1, 1, 0, 0, 0, 0; .2byte 31, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 2, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 141, 0, 0
	.byte 3, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 0, 0, 0, 0; .2byte 1, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 1, 1, 0, 0, 0, 0; .2byte 31, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 2, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 141, 0, 0
	.byte 3, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 0, 0, 0, 0; .2byte 1, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 1, 1, 0, 0, 0, 0; .2byte 31, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 2, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 141, 0, 0
	.byte 3, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 26, 0, 0, 4, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 26, 4, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 6, 26, 0, 0, 0, 4; .2byte 59, 106, 128, 157, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 27, 5, 0, 0, 0; .2byte 34, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 5, 27, 0, 0, 5, 0; .2byte 59, 79, 121, 149, 402, 0, 140, 0, 0
	.byte 6, 27, 0, 0, 0, 5; .2byte 61, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 7, 27, 0, 5, 0, 0; .2byte 5, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 32, 0, 0, 7, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 32, 7, 0, 0, 0; .2byte 49, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 32, 0, 7, 0, 0; .2byte 49, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 32, 0, 0, 0, 7; .2byte 49, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 5, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 5, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 5, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 29, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 29, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 326, 140, 0, 0
	.byte 7, 29, 0, 6, 0, 0; .2byte 283, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 29, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 20, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 20, 2, 0, 1, 0; .2byte 18, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 20, 0, 4, 0, 0; .2byte 18, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 20, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 29, 4, 0, 0, 5; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 29, 0, 0, 5, 4; .2byte 293, 79, 121, 149, 402, 326, 140, 0, 0
	.byte 7, 29, 7, 1, 0, 0; .2byte 283, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 29, 7, 0, 0, 1; .2byte 306, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 0, 0, 0, 0; .2byte 1, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 1, 1, 0, 0, 0, 0; .2byte 31, 75, 118, 145, 0, 0, 140, 0, 0
	.byte 2, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 141, 0, 0
	.byte 3, 1, 0, 0, 0, 0; .2byte 55, 104, 127, 156, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 5, 0, 0, 0, 0; .2byte 16, 90, 127, 156, 0, 0, 0, 0, 0
	.byte 4, 5, 1, 0, 0, 0; .2byte 1, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 5, 0, 0, 0, 0; .2byte 43, 90, 127, 156, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 9, 0, 1, 1, 0; .2byte 16, 90, 136, 166, 0, 0, 0, 0, 0
	.byte 4, 9, 1, 0, 0, 0; .2byte 31, 90, 118, 156, 0, 0, 140, 0, 0
	.byte 6, 9, 0, 0, 0, 1; .2byte 56, 90, 136, 166, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 12, 0, 1, 1, 0; .2byte 17, 104, 128, 166, 402, 0, 0, 0, 0
	.byte 4, 12, 2, 0, 0, 0; .2byte 2, 75, 118, 145, 402, 0, 140, 0, 0
	.byte 6, 12, 0, 0, 0, 2; .2byte 44, 104, 128, 166, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 14, 0, 1, 1, 0; .2byte 17, 92, 128, 157, 402, 0, 0, 0, 0
	.byte 4, 14, 2, 0, 0, 0; .2byte 2, 92, 119, 145, 402, 0, 140, 0, 0
	.byte 6, 14, 0, 0, 0, 3; .2byte 57, 92, 128, 157, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 15, 0, 1, 2, 0; .2byte 58, 92, 137, 157, 402, 0, 0, 0, 0
	.byte 4, 15, 2, 0, 0, 0; .2byte 32, 76, 119, 146, 402, 0, 140, 0, 0
	.byte 6, 15, 0, 0, 0, 3; .2byte 57, 92, 137, 157, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 17, 0, 2, 1, 0; .2byte 18, 105, 137, 167, 402, 0, 0, 0, 0
	.byte 4, 17, 2, 0, 1, 0; .2byte 18, 76, 120, 146, 402, 0, 140, 0, 0
	.byte 6, 17, 0, 0, 0, 3; .2byte 59, 105, 137, 167, 402, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 19, 0, 1, 2, 0; .2byte 60, 105, 129, 167, 402, 0, 0, 0, 0
	.byte 4, 19, 2, 0, 1, 0; .2byte 3, 77, 129, 147, 402, 0, 140, 0, 0
	.byte 7, 19, 0, 4, 0, 0; .2byte 3, 77, 129, 147, 402, 0, 0, 0, 0
	.byte 6, 19, 0, 0, 0, 3; .2byte 59, 105, 129, 167, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 23, 0, 0, 4, 0; .2byte 60, 107, 138, 158, 402, 0, 0, 0, 0
	.byte 4, 23, 4, 0, 0, 0; .2byte 3, 77, 129, 158, 402, 0, 140, 0, 0
	.byte 7, 23, 0, 5, 0, 0; .2byte 3, 77, 129, 158, 402, 0, 0, 0, 0
	.byte 6, 23, 0, 0, 0, 4; .2byte 59, 107, 138, 158, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 24, 0, 0, 4, 0; .2byte 61, 107, 138, 168, 402, 0, 0, 0, 0
	.byte 4, 24, 5, 0, 0, 0; .2byte 34, 78, 121, 148, 402, 0, 140, 0, 0
	.byte 7, 24, 0, 5, 0, 0; .2byte 4, 78, 121, 148, 402, 0, 0, 0, 0
	.byte 6, 24, 0, 0, 0, 5; .2byte 60, 107, 138, 168, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 25, 0, 1, 4, 0; .2byte 20, 94, 138, 169, 402, 0, 0, 0, 0
	.byte 4, 25, 5, 0, 0, 0; .2byte 5, 78, 121, 148, 402, 0, 140, 0, 0
	.byte 7, 25, 0, 5, 0, 0; .2byte 34, 78, 121, 148, 402, 0, 0, 0, 0
	.byte 6, 25, 0, 0, 0, 5; .2byte 46, 94, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 27, 0, 0, 5, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 27, 5, 0, 0, 0; .2byte 5, 79, 121, 149, 402, 0, 140, 0, 0
	.byte 7, 27, 0, 6, 0, 0; .2byte 34, 79, 121, 149, 402, 0, 0, 0, 0
	.byte 6, 27, 0, 0, 0, 5; .2byte 46, 109, 131, 169, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 31, 0, 0, 6, 0; .2byte 20, 94, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 31, 6, 0, 0, 0; .2byte 21, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 31, 0, 7, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 31, 0, 0, 0, 6; .2byte 47, 109, 131, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 32, 0, 0, 7, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 32, 7, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 32, 0, 7, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 32, 0, 0, 0, 7; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 33, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 33, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 33, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 33, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 35, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 35, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 35, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 35, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 40, 9, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 40, 0, 9, 0, 0; .2byte 37, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 40, 0, 0, 9, 0; .2byte 57, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 40, 0, 0, 0, 9; .2byte 60, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 30, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 30, 0, 4, 0, 0; .2byte 37, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 30, 0, 0, 4, 0; .2byte 57, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 30, 0, 0, 0, 4; .2byte 60, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 20, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 20, 0, 4, 0, 0; .2byte 37, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 20, 0, 0, 4, 0; .2byte 57, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 20, 0, 0, 0, 4; .2byte 60, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 15, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 15, 0, 4, 0, 0; .2byte 37, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 15, 0, 0, 4, 0; .2byte 57, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 15, 0, 0, 0, 4; .2byte 60, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 10, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 7, 10, 0, 4, 0, 0; .2byte 37, 79, 121, 149, 0, 0, 0, 0, 0
	.byte 5, 10, 0, 0, 4, 0; .2byte 57, 95, 142, 159, 0, 0, 0, 0, 0
	.byte 6, 10, 0, 0, 0, 4; .2byte 60, 108, 140, 171, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 40, 9, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 5, 40, 0, 0, 9, 0; .2byte 57, 93, 139, 169, 0, 0, 0, 0, 0
	.byte 6, 40, 0, 0, 0, 9; .2byte 57, 95, 142, 159, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 30, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 5, 30, 0, 0, 4, 0; .2byte 57, 93, 139, 169, 0, 0, 0, 0, 0
	.byte 6, 30, 0, 0, 0, 4; .2byte 57, 95, 142, 159, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 20, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 5, 20, 0, 0, 4, 0; .2byte 57, 93, 139, 169, 0, 0, 0, 0, 0
	.byte 6, 20, 0, 0, 0, 4; .2byte 57, 95, 142, 159, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 15, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 5, 15, 0, 0, 4, 0; .2byte 57, 93, 139, 169, 0, 0, 0, 0, 0
	.byte 6, 15, 0, 0, 0, 4; .2byte 57, 95, 142, 159, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 4, 10, 4, 0, 0, 0; .2byte 8, 78, 124, 149, 0, 0, 140, 0, 0
	.byte 5, 10, 0, 0, 4, 0; .2byte 57, 93, 139, 169, 0, 0, 0, 0, 0
	.byte 6, 10, 0, 0, 0, 4; .2byte 57, 95, 142, 159, 0, 0, 141, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 33, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 33, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 33, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 33, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 33, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 33, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 33, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 33, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 34, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 34, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 34, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 34, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 34, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 34, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 34, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 34, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 35, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 35, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 35, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 35, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 35, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 35, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 35, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 35, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 36, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 36, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 36, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 36, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 36, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 36, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 36, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 36, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 37, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 37, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 37, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 37, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 37, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 37, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 37, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 37, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 38, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 38, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 38, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 38, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 38, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 38, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 38, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 38, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 39, 0, 0, 8, 0; .2byte 21, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 39, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 39, 0, 8, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 39, 0, 0, 0, 8; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 39, 0, 0, 0, 8; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 39, 8, 0, 0, 0; .2byte 6, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 39, 0, 0, 8, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 39, 0, 8, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 40, 0, 0, 9, 0; .2byte 293, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 40, 9, 0, 0, 0; .2byte 10, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 40, 0, 9, 0, 0; .2byte 275, 80, 121, 150, 402, 268, 0, 0, 0
	.byte 6, 40, 0, 0, 0, 9; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 40, 0, 0, 0, 9; .2byte 26, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 40, 9, 0, 0, 0; .2byte 276, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 40, 0, 0, 9, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 40, 0, 9, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 50, 0, 0, 9, 0; .2byte 293, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 50, 9, 0, 0, 0; .2byte 10, 335, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 50, 0, 9, 0, 0; .2byte 275, 332, 121, 150, 402, 268, 0, 0, 0
	.byte 6, 50, 0, 0, 0, 9; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 50, 0, 0, 0, 9; .2byte 26, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 50, 9, 0, 0, 0; .2byte 276, 84, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 50, 0, 0, 9, 0; .2byte 34, 82, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 50, 0, 9, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 60, 0, 0, 9, 0; .2byte 293, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 60, 9, 0, 0, 0; .2byte 10, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 60, 0, 9, 0, 0; .2byte 275, 80, 121, 150, 402, 268, 0, 0, 0
	.byte 6, 60, 0, 0, 0, 9; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 60, 0, 0, 0, 9; .2byte 26, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 60, 9, 0, 0, 0; .2byte 276, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 60, 0, 0, 9, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 60, 0, 9, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 70, 0, 0, 9, 0; .2byte 293, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 70, 9, 0, 0, 0; .2byte 10, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 70, 0, 9, 0, 0; .2byte 275, 80, 121, 150, 402, 268, 0, 0, 0
	.byte 6, 70, 0, 0, 0, 9; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 70, 0, 0, 0, 9; .2byte 26, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 70, 9, 0, 0, 0; .2byte 276, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 70, 0, 0, 9, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 70, 0, 9, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 5, 35, 0, 0, 6, 0; .2byte 20, 109, 139, 169, 402, 0, 0, 0, 0
	.byte 4, 35, 6, 0, 0, 0; .2byte 10, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 7, 35, 0, 6, 0, 0; .2byte 34, 80, 121, 150, 402, 0, 0, 0, 0
	.byte 6, 35, 0, 0, 0, 6; .2byte 63, 110, 140, 159, 402, 0, 141, 144, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 2, 35, 0, 0, 0, 6; .2byte 21, 109, 139, 169, 402, 0, 141, 144, 0
	.byte 0, 35, 6, 0, 0, 0; .2byte 9, 80, 122, 150, 402, 0, 140, 0, 0
	.byte 1, 35, 0, 0, 6, 0; .2byte 34, 80, 121, 150, 402, 0, 140, 0, 0
	.byte 3, 35, 0, 6, 0, 0; .2byte 63, 110, 140, 159, 402, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte -1, 0, 0, 0, 0, 0; .2byte 0, 0, 0, 0, 0, 0, 0, 0, 0
@ The Broken Seal's table of this name in form (16 there)
	.global gBattleObject_EmptyScript
gBattleObject_EmptyScript:
	.4byte 17
@ Resource ID | slot << 9, terminated by 0xffff.
	.global gBattleUnit_WeaponKinds
gBattleUnit_WeaponKinds:
	.2byte 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008
	.2byte 0x0009, 0x000a, 0x000b, 0x020f, 0x0210, 0x0211, 0x0212, 0x0213
	.2byte 0x0214, 0x0215, 0x0216, 0x0217, 0x0218, 0x0219, 0x021a, 0x021b
	.2byte 0x041f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426
	.2byte 0x062b, 0x062c, 0x062d, 0x062e, 0x062f, 0x0630, 0x0631, 0x0632
	.2byte 0x0837, 0x0838, 0x0839, 0x0a3a, 0x083b, 0x083c, 0x0a3d, 0x083e
	.2byte 0x083f, 0x0840, 0x0841, 0x0110, 0x0111, 0x0112, 0x0113, 0x0114
	.2byte 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x011a, 0x011b, 0x031d
	.2byte 0x031e, 0x031f, 0x0320, 0x0321, 0x0322, 0x0323, 0x0324, 0x0325
	.2byte 0x0527, 0x0528, 0x0529, 0x052a, 0x052b, 0x052c, 0x052d, 0x052e
	.2byte 0x052f, 0x0731, 0x0732, 0x0733, 0x0734, 0x0735, 0x0736, 0x0737
	.2byte 0x0939, 0x093a, 0x093b, 0x093c, 0x093d, 0x093e, 0x093f, 0x0940
	.2byte 0x0941, 0x0942, 0x0b43, 0x0944, 0x03a1, 0x01a2, 0x03a3, 0x01a4
	.2byte 0x05a5, 0x05a6, 0x07a7, 0x07a8, 0x09a9, 0x09aa, 0x09ab, 0xffff
@ The Broken Seal's gBattleMotion_VariantAcceleration in form, ten for its eight
	.global Data_0812cbb0
Data_0812cbb0:
	.4byte 1 << 16, 1 << 16, 1 << 16, 1 << 16, 8 << 16, 1 << 16, 1 << 16, 1 << 16
	.4byte 1 << 16, 8 << 16
@ gBattleMotion_VariantSpeedLimit
	.global Data_0812cbd8
Data_0812cbd8:
	.4byte 4 << 16, 4 << 16, 4 << 16, 4 << 16, 20 << 16, 4 << 16, 4 << 16, 4 << 16
	.4byte 4 << 16, 20 << 16
@ gBattleMotion_VariantVelocityY
	.global gBattleKnockback_DistancePercent
gBattleKnockback_DistancePercent:
	.4byte 3 << 16, 6 << 16, 7 << 16, 10 << 16, 5 << 16, 2 << 16, 3 << 16, 0x58000
	.4byte 10 << 16, 5 << 16
@ gBattleMotion_VariantDistancePercent
	.global Data_0812cc28
Data_0812cc28:
	.4byte 140, 140, 150, 200, 200, 150, 100, 100
	.4byte 100, 300
	.4byte 0, 20, 38
@ BattleAI_ChooseAction's rolls for the first three action strategies, a row
@ each: the chance in 256 of each place in the enemy's actions.
	.global gBattleAI_ActionWeights
gBattleAI_ActionWeights:
	.byte 32, 32, 32, 32, 32, 32, 32, 32
	.byte 53, 47, 41, 35, 29, 23, 17, 11
	.byte 113, 56, 31, 20, 14, 10, 7, 5
@ A copy of The Broken Seal's gBattleTransition_CurtainVariants
	.global gBattleTransition_CurtainVariants
gBattleTransition_CurtainVariants:
	.incbin "games/tla/src/battle/presentation/tilevar.4bpp"
@ A copy of The Broken Seal's gBattleTransition_CurtainTiles
	.global gBattleTransition_CurtainTiles
gBattleTransition_CurtainTiles:
	.incbin "games/tla/src/battle/presentation/curtain.4bpp"
	.balign 4
	.include "games/tla/src/battle/presentation/highlight_ramp.s"
	.4byte 1, 38
@ 660 enemy formations.
	.global gBattleFormation_Records
gBattleFormation_Records:
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 4, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 5, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 6, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 7, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 8, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 9, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 10, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 11, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 12, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 13, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 14, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 19, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 20, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 21, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 22, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 23, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 24, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 25, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 26, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 27, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 28, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 29, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 33, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 34, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 35, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 36, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 37, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 38, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 39, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 40, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 41, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 42, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 43, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 48, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 49, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 50, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 51, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 52, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 53, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 54, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 55, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 56, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 57, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 58, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 60, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 60, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 60, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 66, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 69, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 72, 73, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 79, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 80, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 83, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 84, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 85, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 86, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 87, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 90, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 93, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 96, 99, 99, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 96, 100, 99, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 96, 100, 100, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 97, 100, 100, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 98, 100, 100, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 105, 102, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 105, 102, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 106, 103, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 187, 186, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 361, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 365, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 368, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 60, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 61, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 62, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 64, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 65, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 66, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 67, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 68, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 69, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 70, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 71, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 72, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 73, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 74, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 75, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 76, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 77, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 78, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 79, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 80, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 81, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 82, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 83, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 84, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 85, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 86, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 87, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 88, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 89, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 90, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 91, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 92, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 93, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 94, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 95, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 96, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 97, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 98, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 99, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 100, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 101, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 102, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 103, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 104, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 105, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 106, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 107, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 110, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 111, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 113, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 114, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 116, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 117, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 118, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 120, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 123, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 124, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 125, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 126, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 127, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 128, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 130, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 131, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 132, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 133, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 134, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 135, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 136, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 137, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 138, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 140, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 141, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 142, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 143, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 144, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 145, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 146, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 147, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 148, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 149, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 150, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 151, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 152, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 153, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 154, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 155, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 156, 370, 373, 371, 372; .byte 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0
	.2byte 157, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 158, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 159, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 160, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 161, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 162, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 163, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 164, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 165, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 166, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 167, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 168, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 169, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 170, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 171, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 172, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 173, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 174, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 175, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 176, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 177, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 178, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 179, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 180, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 181, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 182, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 183, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 184, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 185, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 186, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 187, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 188, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 189, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 190, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 191, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 192, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 193, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 194, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 195, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 196, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 197, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 198, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 199, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 200, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 201, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 202, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 203, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 204, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 205, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 206, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 207, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 208, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 209, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 210, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 211, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 212, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 213, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 214, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 215, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 361, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 365, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 368, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 222, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 222, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 222, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 63, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 63, 222, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 63, 222, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 222, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 63, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 222, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 63, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 319, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 109, 222, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 222, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 230, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 109, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 230, 109, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 230, 112, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 112, 230, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 240, 230, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 240, 112, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 112, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 249, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 230, 249, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 249, 112, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 109, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 112, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 249, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 246, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 240, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 243, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 255, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 252, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 115, 252, 0, 0, 0; .byte 0, 1, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 249, 252, 0, 0, 0; .byte 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 249, 115, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 255, 246, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 255, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 118, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 141, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 141, 118, 0, 0, 0; .byte 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 141, 118, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 141, 118, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
	.2byte 255, 121, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 118, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 118, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 258, 118, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 258, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 261, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 261, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 121, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 121, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 121, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 264, 141, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 121, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 129, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 129, 264, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 129, 264, 0, 0; .byte 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
	.2byte 320, 129, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 132, 264, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 132, 270, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 320, 270, 0, 0, 0; .byte 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 132, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 64, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 135, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 64, 135, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 135, 64, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 138, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 138, 135, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 67, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 67, 138, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 64, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 64, 135, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 135, 276, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 135, 64, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 241, 276, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 241, 244, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 138, 135, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 138, 241, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 67, 279, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 67, 150, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 150, 321, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 150, 67, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 279, 150, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 144, 150, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 144, 67, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 150, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 241, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 147, 241, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 147, 241, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 291, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 244, 241, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 291, 321, 0, 0, 0; .byte 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 195, 147, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 195, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 162, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 162, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 162, 0, 0, 0, 0; .byte 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 162, 0, 0, 0, 0; .byte 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 159, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 159, 77, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 171, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 171, 159, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 253, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 253, 171, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 165, 77, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 165, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 218, 110, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 65, 225, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 110, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 110, 65, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 116, 268, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 116, 110, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 231, 235, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 235, 65, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 218, 225, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 256, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 110, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 110, 231, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 235, 256, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 225, 110, 0, 0, 0; .byte 0, 1, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 151, 110, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 151, 218, 0, 0, 0; .byte 1, 1, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 78, 151, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 174, 151, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 174, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 271, 151, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 271, 231, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 168, 323, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 168, 174, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 168, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 113, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 236, 113, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 262, 113, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 306, 301, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 309, 113, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 153, 218, 113, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
	.2byte 153, 301, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 153, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 142, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 153, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 189, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 189, 142, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 272, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 204, 189, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 204, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 160, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 160, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 142, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 189, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 204, 272, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 295, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 312, 160, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 163, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 166, 163, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 166, 172, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 281, 172, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 81, 172, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 81, 166, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 169, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 169, 81, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 136, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 130, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 130, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 122, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 145, 122, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 145, 139, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 70, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 133, 70, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 91, 70, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 177, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 207, 177, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 207, 133, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 183, 177, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 183, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 314, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 305, 314, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 148, 305, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 193, 148, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 180, 193, 148, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0
	.2byte 180, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 196, 193, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 317, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 327, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 181, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 175, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 330, 175, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 190, 181, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 190, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 198, 190, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 198, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 228, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 228, 261, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 264, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 264, 228, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 264, 261, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 228, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 264, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 270, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 147, 241, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 244, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 241, 244, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 291, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 321, 241, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 244, 321, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 195, 147, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 195, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 113, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 236, 113, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 262, 113, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 306, 236, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 262, 301, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 192, 309, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 192, 153, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 153, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 272, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 295, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 295, 312, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 272, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 119, 295, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 142, 119, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 142, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 272, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 169, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 312, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 232, 266, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 272, 169, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 312, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 307, 266, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 307, 312, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 130, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 139, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 122, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 130, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 145, 139, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 145, 122, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 304, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 136, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 136, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 122, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 139, 278, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 139, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 122, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 275, 136, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 275, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 163, 0, 0, 0, 0; .byte 4, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 81, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 169, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 317, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 317, 169, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 180, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 180, 317, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 180, 169, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 161, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 173, 161, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 167, 205, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 205, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 82, 173, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 82, 161, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 170, 82, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 170, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 71, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 137, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 191, 71, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 191, 137, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 208, 71, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 208, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 188, 137, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 188, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 114, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 114, 199, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 182, 114, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 182, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 199, 114, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 199, 184, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 184, 182, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 184, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 134, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 134, 92, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 92, 178, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 92, 197, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 178, 134, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0
	.2byte 178, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 197, 178, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0
	.2byte 197, 0, 0, 0, 0; .byte 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 156, 370, 373, 371, 372; .byte 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0
	.2byte 201, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 213, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 210, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0
	.2byte 376, 374, 375, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 377, 378, 378, 0, 0; .byte 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 2, 0, 0, 0
	.2byte 109, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 104, 107, 0, 0, 0; .byte 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 124, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 125, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 126, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 127, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 128, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0; .byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 351, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 352, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 353, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 354, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 355, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 356, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 357, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 358, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.2byte 359, 0, 0, 0, 0; .byte 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.global Data_08130c5c
Data_08130c5c:
	.2byte 0x009, 0x00a, 0x00b, 0x00c, 0x00d, 0x00e, 0x00f, 0x010
	.2byte 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017, 0x018
	.2byte 0x019, 0x01a, 0x01b, 0x01c, 0x01d, 0x01e, 0x01f, 0x020
	.2byte 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027, 0x028
	.2byte 0x029, 0x02a, 0x02b, 0x02c, 0x02d, 0x02e, 0x02f, 0x030
	.2byte 0x031, 0x032, 0x033, 0x034, 0x035, 0x036, 0x037, 0x038
	.2byte 0x039, 0x03a, 0x03b, 0x03c, 0x03d, 0x03e, 0x03f, 0x040
	.2byte 0x041, 0x042, 0x05b, 0x05c, 0x05d, 0x05e, 0x066, 0x068
	.2byte 0x069, 0x06b, 0x06e, 0x070, 0x071, 0x073, 0x0db, 0x0dc
	.2byte 0x156, 0x158, 0x15a, 0x160, 0x163, 0x170, 0x171, 0x172
	.2byte 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x000
@ 387 battle graphics entries.
	.global gSummon_EntryTable
gSummon_EntryTable:
	.2byte 607; .byte 0xa2, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0x82, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0xa2, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0xa2, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0xa2, 0x0, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 600; .byte 0x48, 0x0, 32, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 603; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 602; .byte 0x48, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 601; .byte 0xa8, 0x0, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 570; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 570; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 570; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 534; .byte 0xd, 0x20, 0, 0, 0, 0
	.2byte 534; .byte 0xd, 0x26, 0, 0, 0, 0
	.2byte 534; .byte 0xd, 0x24, 0, 0, 0, 0
	.2byte 531; .byte 0xc2, 0x40, 0, 0, 0, 0
	.2byte 531; .byte 0xc2, 0x46, 0, 0, 0, 0
	.2byte 531; .byte 0xc2, 0x4a, 0, 0, 0, 0
	.2byte 572; .byte 0x8, 0x40, 0, 0, 0, 0
	.2byte 572; .byte 0x8, 0x46, 0, 0, 0, 0
	.2byte 572; .byte 0x8, 0x4a, 0, 0, 0, 0
	.2byte 564; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 574; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 574; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 574; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 611; .byte 0x7, 0x0, 0, 0, 0, 0
	.2byte 611; .byte 0x7, 0xa, 0, 0, 0, 0
	.2byte 611; .byte 0x7, 0x6, 0, 0, 0, 0
	.2byte 611; .byte 0x7, 0x0, 0, 0, 0, 0
	.2byte 556; .byte 0x82, 0x22, 0, 0, 0, 0
	.2byte 556; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 556; .byte 0x82, 0x26, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 561; .byte 0x82, 0x61, 0, 0, 0, 0
	.2byte 569; .byte 0xa, 0x21, 0, 0, 0, 0
	.2byte 569; .byte 0xa, 0x23, 0, 0, 0, 0
	.2byte 569; .byte 0xa, 0x29, 0, 0, 0, 0
	.2byte 585; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 585; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 585; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 578; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 578; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 578; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 577; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 577; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 577; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 575; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 575; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 575; .byte 0xe, 0x41, 0, 0, 0, 0
	.2byte 568; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 568; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 568; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 558; .byte 0x82, 0x20, 42, 0, 0, 0
	.2byte 558; .byte 0x82, 0x26, 42, 0, 0, 0
	.2byte 558; .byte 0x82, 0x28, 42, 0, 0, 0
	.2byte 571; .byte 0x87, 0x0, 0, 0, 0, 0
	.2byte 571; .byte 0x87, 0xa, 0, 0, 0, 0
	.2byte 571; .byte 0x87, 0x6, 0, 0, 0, 0
	.2byte 549; .byte 0xa2, 0x20, 0, 0, 0, 0
	.2byte 549; .byte 0xa2, 0x22, 0, 0, 0, 0
	.2byte 549; .byte 0xa2, 0x2a, 0, 0, 0, 0
	.2byte 567; .byte 0xe5, 0x0, 0, 0, 0, 0
	.2byte 567; .byte 0xe5, 0x6, 0, 0, 0, 0
	.2byte 567; .byte 0xe5, 0x4, 0, 0, 0, 0
	.2byte 529; .byte 0xa2, 0x20, 0, 0, 0, 0
	.2byte 529; .byte 0xa2, 0x26, 0, 0, 0, 0
	.2byte 529; .byte 0xa2, 0x24, 0, 0, 0, 0
	.2byte 540; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 540; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 540; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 540; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 540; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 542; .byte 0x10, 0x20, 0, 0, 0, 0
	.2byte 542; .byte 0x10, 0x26, 0, 0, 0, 0
	.2byte 542; .byte 0x10, 0x22, 0, 0, 0, 0
	.2byte 552; .byte 0x82, 0x22, 0, 0, 0, 0
	.2byte 552; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 552; .byte 0x82, 0x28, 0, 0, 0, 0
	.2byte 537; .byte 0xa2, 0x26, 0, 0, 0, 0
	.2byte 537; .byte 0xa2, 0x20, 0, 0, 0, 0
	.2byte 537; .byte 0xa2, 0x22, 0, 0, 0, 0
	.2byte 551; .byte 0x84, 0x26, 0, 0, 0, 0
	.2byte 551; .byte 0x84, 0x22, 0, 0, 0, 0
	.2byte 551; .byte 0x84, 0x28, 0, 0, 0, 0
	.2byte 553; .byte 0xe2, 0x22, 0, 0, 0, 0
	.2byte 553; .byte 0xe2, 0x20, 0, 0, 0, 0
	.2byte 553; .byte 0xe2, 0x28, 0, 0, 0, 0
	.2byte 536; .byte 0x6, 0x26, 0, 0, 0, 0
	.2byte 536; .byte 0x6, 0x24, 0, 0, 0, 0
	.2byte 536; .byte 0x6, 0x28, 0, 0, 0, 0
	.2byte 539; .byte 0x82, 0x24, 0, 0, 0, 0
	.2byte 539; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 539; .byte 0x82, 0x26, 0, 0, 0, 0
	.2byte 532; .byte 0xb3, 0x6, 0, 0, 0, 0
	.2byte 532; .byte 0xb3, 0x0, 0, 0, 0, 0
	.2byte 532; .byte 0xb3, 0xa, 0, 0, 0, 0
	.2byte 548; .byte 0x10, 0x20, 0, 0, 0, 0
	.2byte 548; .byte 0x10, 0x22, 0, 0, 0, 0
	.2byte 548; .byte 0x10, 0x28, 0, 0, 0, 0
	.2byte 544; .byte 0x14, 0x20, 0, 0, 0, 0
	.2byte 544; .byte 0x14, 0x20, 0, 0, 0, 0
	.2byte 544; .byte 0x14, 0x20, 0, 0, 0, 0
	.2byte 559; .byte 0x13, 0x0, 0, 0, 0, 0
	.2byte 559; .byte 0x13, 0x4, 0, 0, 0, 0
	.2byte 559; .byte 0x13, 0x6, 0, 0, 0, 0
	.2byte 557; .byte 0xb, 0x0, 0, 0, 0, 0
	.2byte 557; .byte 0xb, 0x6, 0, 0, 0, 0
	.2byte 557; .byte 0xb, 0xa, 0, 0, 0, 0
	.2byte 573; .byte 0x6, 0x28, 0, 0, 0, 0
	.2byte 573; .byte 0x6, 0x20, 0, 0, 0, 0
	.2byte 573; .byte 0x6, 0x26, 0, 0, 0, 0
	.2byte 563; .byte 0xf0, 0x40, 60, 0, 0, 0
	.2byte 563; .byte 0xf0, 0x46, 60, 0, 0, 0
	.2byte 563; .byte 0xf0, 0x44, 60, 0, 0, 0
	.2byte 565; .byte 0x12, 0x20, 0, 0, 0, 0
	.2byte 565; .byte 0x12, 0x24, 0, 0, 0, 0
	.2byte 565; .byte 0x12, 0x2a, 0, 0, 0, 0
	.2byte 566; .byte 0xc2, 0x40, 0, 0, 0, 0
	.2byte 566; .byte 0xc2, 0x44, 0, 0, 0, 0
	.2byte 566; .byte 0xc2, 0x4a, 0, 0, 0, 0
	.2byte 555; .byte 0xc2, 0x40, 0, 0, 0, 0
	.2byte 555; .byte 0xc2, 0x44, 0, 0, 0, 0
	.2byte 555; .byte 0xc2, 0x4a, 0, 0, 0, 0
	.2byte 546; .byte 0xd2, 0x20, 0, 0, 0, 0
	.2byte 546; .byte 0xd2, 0x24, 0, 0, 0, 0
	.2byte 546; .byte 0xd2, 0x2a, 0, 0, 0, 0
	.2byte 526; .byte 0xa4, 0x40, 0, 0, 0, 0
	.2byte 526; .byte 0xa4, 0x42, 0, 0, 0, 0
	.2byte 526; .byte 0xa4, 0x48, 0, 0, 0, 0
	.2byte 530; .byte 0xa4, 0x41, 0, 0, 0, 0
	.2byte 530; .byte 0xa4, 0x41, 0, 0, 0, 0
	.2byte 530; .byte 0xa4, 0x49, 0, 0, 0, 0
	.2byte 550; .byte 0xa2, 0x41, 0, 0, 0, 0
	.2byte 550; .byte 0xa2, 0x49, 0, 0, 0, 0
	.2byte 550; .byte 0xa2, 0x43, 0, 0, 0, 0
	.2byte 543; .byte 0xa2, 0x20, 0, 0, 0, 0
	.2byte 543; .byte 0xa2, 0x26, 0, 0, 0, 0
	.2byte 543; .byte 0xa2, 0x28, 0, 0, 0, 0
	.2byte 527; .byte 0x82, 0x21, 0, 0, 0, 0
	.2byte 527; .byte 0x82, 0x23, 0, 0, 0, 0
	.2byte 527; .byte 0x82, 0x29, 0, 0, 0, 0
	.2byte 528; .byte 0x82, 0x41, 0, 0, 0, 0
	.2byte 528; .byte 0x82, 0x47, 0, 0, 0, 0
	.2byte 528; .byte 0x82, 0x49, 0, 0, 0, 0
	.2byte 560; .byte 0xa2, 0x41, 0, 0, 0, 0
	.2byte 560; .byte 0xa2, 0x41, 0, 0, 0, 0
	.2byte 560; .byte 0xa2, 0x41, 0, 0, 0, 0
	.2byte 547; .byte 0xa2, 0x41, 0, 0, 0, 0
	.2byte 547; .byte 0xa2, 0x47, 0, 0, 0, 0
	.2byte 547; .byte 0xa2, 0x43, 0, 0, 0, 0
	.2byte 545; .byte 0x10, 0x21, 0, 0, 0, 0
	.2byte 545; .byte 0x10, 0x27, 0, 0, 0, 0
	.2byte 545; .byte 0x10, 0x25, 0, 0, 0, 0
	.2byte 538; .byte 0x82, 0x41, 0, 0, 0, 0
	.2byte 538; .byte 0x82, 0x41, 0, 0, 0, 0
	.2byte 538; .byte 0x82, 0x41, 0, 0, 0, 0
	.2byte 554; .byte 0xe2, 0x41, 0, 0, 0, 0
	.2byte 554; .byte 0xe2, 0x41, 0, 0, 0, 0
	.2byte 554; .byte 0xe2, 0x41, 0, 0, 0, 0
	.2byte 607; .byte 0x47, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0x47, 0x0, 0, 0, 0, 0
	.2byte 607; .byte 0x47, 0xa, 0, 0, 0, 0
	.2byte 610; .byte 0xc9, 0x0, 0, 0, 0, 0
	.2byte 610; .byte 0xc9, 0x0, 0, 0, 0, 0
	.2byte 610; .byte 0xc9, 0x8, 0, 0, 0, 0
	.2byte 610; .byte 0xc9, 0x8, 0, 0, 0, 0
	.2byte 608; .byte 0x9, 0x0, 0, 0, 0, 0
	.2byte 608; .byte 0x9, 0x0, 0, 0, 0, 0
	.2byte 608; .byte 0x9, 0x2, 0, 0, 0, 0
	.2byte 609; .byte 0x3, 0x2, 0, 0, 0, 0
	.2byte 609; .byte 0x3, 0x2, 0, 0, 0, 0
	.2byte 609; .byte 0x3, 0x6, 0, 0, 0, 0
	.2byte 609; .byte 0x3, 0x0, 0, 0, 0, 0
	.2byte 591; .byte 0x80, 0x0, 0, 0, 0, 0
	.2byte 591; .byte 0x80, 0x6, 0, 0, 0, 0
	.2byte 591; .byte 0x80, 0x8, 0, 0, 0, 0
	.2byte 591; .byte 0x0, 0x4, 0, 0, 0, 0
	.2byte 588; .byte 0x2, 0x20, 0, 0, 0, 0
	.2byte 588; .byte 0x2, 0x22, 0, 0, 0, 0
	.2byte 588; .byte 0x2, 0x28, 0, 0, 0, 0
	.2byte 641; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 641; .byte 0x2, 0x28, 0, 0, 0, 0
	.2byte 641; .byte 0x2, 0x20, 0, 0, 0, 0
	.2byte 623; .byte 0x2, 0x20, 0, 0, 0, 0
	.2byte 623; .byte 0x82, 0x26, 0, 0, 0, 0
	.2byte 623; .byte 0xe2, 0x24, 0, 0, 0, 0
	.2byte 624; .byte 0x1, 0x2, 0, 0, 0, 0
	.2byte 624; .byte 0x1, 0x4, 0, 0, 0, 0
	.2byte 624; .byte 0x21, 0x0, 0, 0, 0, 0
	.2byte 612; .byte 0xab, 0x0, 8, 0, 0, 0
	.2byte 612; .byte 0xab, 0x2, 8, 0, 0, 0
	.2byte 612; .byte 0xab, 0x8, 8, 0, 0, 0
	.2byte 593; .byte 0x86, 0x0, 0, 0, 0, 0
	.2byte 593; .byte 0x86, 0x8, 0, 0, 0, 0
	.2byte 593; .byte 0x86, 0x4, 0, 0, 0, 0
	.2byte 614; .byte 0xc7, 0x0, 0, 0, 0, 0
	.2byte 614; .byte 0xc7, 0x2, 0, 0, 0, 0
	.2byte 614; .byte 0xc7, 0x6, 0, 0, 0, 0
	.2byte 587; .byte 0x0, 0x20, 0, 0, 0, 0
	.2byte 587; .byte 0x0, 0x2a, 0, 0, 0, 0
	.2byte 587; .byte 0x0, 0x24, 0, 0, 0, 0
	.2byte 615; .byte 0x1, 0x0, 16, 0, 0, 0
	.2byte 615; .byte 0x1, 0x2, 16, 0, 0, 0
	.2byte 615; .byte 0x1, 0x8, 16, 0, 0, 0
	.2byte 622; .byte 0xa1, 0x0, 0, 0, 0, 0
	.2byte 622; .byte 0xa1, 0x8, 0, 0, 0, 0
	.2byte 622; .byte 0xa1, 0x6, 0, 0, 0, 0
	.2byte 594; .byte 0x0, 0x20, 0, 0, 0, 0
	.2byte 594; .byte 0x0, 0x24, 0, 0, 0, 0
	.2byte 594; .byte 0x0, 0x28, 0, 0, 0, 0
	.2byte 605; .byte 0xa4, 0x20, 0, 0, 0, 0
	.2byte 605; .byte 0xa4, 0x22, 0, 0, 0, 0
	.2byte 605; .byte 0xa4, 0x26, 0, 0, 0, 0
	.2byte 589; .byte 0xa4, 0x20, 0, 0, 0, 0
	.2byte 589; .byte 0xa4, 0x24, 0, 0, 0, 0
	.2byte 589; .byte 0xa4, 0x26, 0, 0, 0, 0
	.2byte 606; .byte 0xc0, 0x0, 0, 0, 0, 0
	.2byte 606; .byte 0xc0, 0x6, 0, 0, 0, 0
	.2byte 606; .byte 0xc0, 0x4, 0, 0, 0, 0
	.2byte 616; .byte 0x5, 0x0, 16, 0, 0, 0
	.2byte 616; .byte 0x5, 0x2, 16, 0, 0, 0
	.2byte 616; .byte 0x5, 0x4, 16, 0, 0, 0
	.2byte 617; .byte 0xc5, 0x0, 0, 0, 0, 0
	.2byte 617; .byte 0xc5, 0x8, 0, 0, 0, 0
	.2byte 617; .byte 0xc5, 0x6, 0, 0, 0, 0
	.2byte 619; .byte 0xa0, 0x20, 0, 0, 0, 0
	.2byte 619; .byte 0xa0, 0x26, 0, 0, 0, 0
	.2byte 619; .byte 0xa0, 0x24, 0, 0, 0, 0
	.2byte 618; .byte 0x7, 0x0, 0, 0, 0, 0
	.2byte 618; .byte 0x7, 0x6, 0, 0, 0, 0
	.2byte 618; .byte 0x7, 0x2, 0, 0, 0, 0
	.2byte 599; .byte 0x60, 0x20, 0, 0, 0, 0
	.2byte 599; .byte 0x60, 0x24, 0, 0, 0, 0
	.2byte 599; .byte 0x60, 0x26, 0, 0, 0, 0
	.2byte 620; .byte 0x0, 0x20, 32, 0, 0, 0
	.2byte 620; .byte 0x0, 0x2a, 32, 0, 0, 0
	.2byte 620; .byte 0x0, 0x26, 32, 0, 0, 0
	.2byte 596; .byte 0xe4, 0x20, 0, 0, 0, 0
	.2byte 596; .byte 0xe4, 0x22, 0, 0, 0, 0
	.2byte 596; .byte 0xe4, 0x28, 0, 0, 0, 0
	.2byte 597; .byte 0x0, 0x20, 0, 0, 0, 0
	.2byte 597; .byte 0x0, 0x22, 0, 0, 0, 0
	.2byte 597; .byte 0x0, 0x24, 0, 0, 0, 0
	.2byte 598; .byte 0xa0, 0x20, 64, 0, 0, 0
	.2byte 598; .byte 0xa0, 0x26, 64, 0, 0, 0
	.2byte 598; .byte 0xa0, 0x22, 64, 0, 0, 0
	.2byte 592; .byte 0xa, 0x20, 64, 0, 0, 0
	.2byte 592; .byte 0xa, 0x22, 64, 0, 0, 0
	.2byte 592; .byte 0xa, 0x28, 64, 0, 0, 0
	.2byte 590; .byte 0x0, 0x20, 0, 0, 0, 0
	.2byte 590; .byte 0x0, 0x22, 0, 0, 0, 0
	.2byte 590; .byte 0x0, 0x24, 0, 0, 0, 0
	.2byte 604; .byte 0xc0, 0x22, 0, 0, 0, 0
	.2byte 604; .byte 0xc0, 0x20, 0, 0, 0, 0
	.2byte 604; .byte 0xc0, 0x2a, 0, 0, 0, 0
	.2byte 613; .byte 0xe1, 0x0, 32, 0, 0, 0
	.2byte 613; .byte 0xe1, 0x2, 32, 0, 0, 0
	.2byte 613; .byte 0xe1, 0x8, 32, 0, 0, 0
	.2byte 595; .byte 0xa4, 0x20, 0, 0, 0, 0
	.2byte 595; .byte 0xa4, 0x24, 0, 0, 0, 0
	.2byte 595; .byte 0xa4, 0x26, 0, 0, 0, 0
	.2byte 629; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 629; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 629; .byte 0x0, 0x2, 0, 0, 0, 0
	.2byte 629; .byte 0x0, 0x6, 0, 0, 0, 0
	.2byte 630; .byte 0xa0, 0x20, 0, 0, 0, 0
	.2byte 630; .byte 0xa0, 0x2a, 0, 0, 0, 0
	.2byte 630; .byte 0xa0, 0x22, 0, 0, 0, 0
	.2byte 626; .byte 0xa, 0x20, 32, 0, 0, 0
	.2byte 626; .byte 0xa, 0x26, 32, 0, 0, 0
	.2byte 626; .byte 0xa, 0x24, 32, 0, 0, 0
	.2byte 628; .byte 0xa0, 0x20, 42, 0, 0, 0
	.2byte 628; .byte 0xa0, 0x26, 42, 0, 0, 0
	.2byte 628; .byte 0xa0, 0x2a, 42, 0, 0, 0
	.2byte 621; .byte 0xa, 0x24, 48, 0, 0, 0
	.2byte 621; .byte 0xa, 0x20, 48, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x20, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x20, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x22, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x22, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x24, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x24, 0, 0, 0, 0
	.2byte 636; .byte 0xa, 0x20, 0, 0, 0, 0
	.2byte 631; .byte 0x0, 0x40, 0, 0, 0, 0
	.2byte 625; .byte 0xa4, 0x20, 0, 0, 0, 0
	.2byte 625; .byte 0xa4, 0x22, 0, 0, 0, 0
	.2byte 625; .byte 0xa4, 0x28, 0, 0, 0, 0
	.2byte 633; .byte 0xa0, 0x0, 0, 0, 0, 0
	.2byte 633; .byte 0xa0, 0x0, 0, 0, 0, 0
	.2byte 633; .byte 0xa0, 0x1, 0, 0, 0, 0
	.2byte 634; .byte 0xa, 0x0, 0, 0, 0, 0
	.2byte 634; .byte 0xa, 0x1, 0, 0, 0, 0
	.2byte 638; .byte 0xc0, 0x61, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 627; .byte 0xc0, 0x0, 32, 0, 0, 0
	.2byte 588; .byte 0x2, 0x20, 0, 0, 0, 0
	.2byte 579; .byte 0xc0, 0x41, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 581; .byte 0xc0, 0x61, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 583; .byte 0xc0, 0x61, 0, 0, 0, 0
	.2byte 0; .byte 0x0, 0x0, 0, 0, 0, 0
	.2byte 576; .byte 0x13, 0x0, 0, 0, 0, 0
	.2byte 576; .byte 0x13, 0x2, 0, 0, 0, 0
	.2byte 576; .byte 0x13, 0xa, 0, 0, 0, 0
	.2byte 576; .byte 0x13, 0x8, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x20, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x22, 0, 0, 0, 0
	.2byte 640; .byte 0xa0, 0x24, 0, 0, 0, 0
	.2byte 641; .byte 0x82, 0x20, 0, 0, 0, 0
	.2byte 641; .byte 0x2, 0x28, 0, 0, 0, 0
