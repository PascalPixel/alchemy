	.section .rodata
	.balign 4
	.4byte 16
@ Formation order from the center.
	.global gBattleParty_CenterOrderOffsets
gBattleParty_CenterOrderOffsets:
	.byte 0, -1, 1, -2, 2, -3, 3, -4, 4
	.balign 4
@ Weapon motions by class.
	.global gBattleUnit_WeaponAnimationsIsaac
gBattleUnit_WeaponAnimationsIsaac:
	.2byte 0x140, 0x14e, 0x146, 0x147, 0x140, 0x140, 0x140
	.global gBattleUnit_WeaponAnimationsGaret
gBattleUnit_WeaponAnimationsGaret:
	.2byte 0x149, 0x149, 0x148, 0x141, 0x141, 0x141, 0x141
	.global gBattleUnit_WeaponAnimationsIvan
gBattleUnit_WeaponAnimationsIvan:
	.2byte 0x14b, 0x14b, 0x14b, 0x14b, 0x142, 0x14a, 0x142
	.global gBattleUnit_WeaponAnimationsMia
gBattleUnit_WeaponAnimationsMia:
	.2byte 0x143, 0x143, 0x14d, 0x14d, 0x14c, 0x143, 0x143
	.global gBattleUnit_WeaponAnimationsJenna
gBattleUnit_WeaponAnimationsJenna:
	.2byte 0x150, 0x150, 0x145, 0x145, 0x145, 0x145, 0x145
	.global gBattlePlacement_StepPairs
gBattlePlacement_StepPairs:
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
	.balign 4
	.global gBattleCamera_CommandMatrix
gBattleCamera_CommandMatrix:
	.4byte 0x10000, 0x0, 0x0, -0xcccc
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x500000, 0x0, 0x0
	.include "games/tbs/src/battle/battle_calc.s"
	.global gBattleAI_ActionWeights
gBattleAI_ActionWeights:
	.byte 32, 32, 32, 32, 32, 32, 32, 32
	.byte 53, 47, 41, 35, 29, 23, 17, 11
	.byte 113, 56, 31, 20, 14, 10, 7, 5
