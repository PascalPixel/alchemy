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
	.incbin "games/tla/src/battle/actstat.table"
@ 733 ability effects (effect ID, stage, flags), like TBS's gBattle_ActionFlags.
	.global Data_08128c50
Data_08128c50:
	.incbin "games/tla/src/battle/actflags.table"
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
	.incbin "games/tla/src/battle/debug_parameter_sets.table"
@ The Broken Seal's table of this name in form (16 there)
	.global gBattleObject_EmptyScript
gBattleObject_EmptyScript:
	.4byte 17
@ Resource ID | slot << 9, terminated by 0xffff.
	.global gResource_SlotAssignments
gResource_SlotAssignments:
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
	.global Data_0812cc5c
Data_0812cc5c:
	.byte 32, 32, 32, 32, 32, 32, 32, 32
	.global Data_0812cc64
Data_0812cc64:
	.byte 53, 47, 41, 35, 29, 23, 17, 11
	.global Data_0812cc6c
Data_0812cc6c:
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
	.incbin "games/tla/src/battle/formatn.table"
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
	.incbin "games/tla/src/battle/summon_sprites.table"
