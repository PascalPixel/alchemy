	.section .rodata
	.balign 4
	.4byte 16
@ 中央からの並び
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.byte 0, -1, 1, -2, 2, -3, 3, -4, 4
	.balign 4
@ クラスごとの武器の動作
	.global BattleUnit_WeaponAnimsClass0
BattleUnit_WeaponAnimsClass0:
	.2byte 0x140, 0x14e, 0x146, 0x147, 0x140, 0x140, 0x140
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.2byte 0x149, 0x149, 0x148, 0x141, 0x141, 0x141, 0x141
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.2byte 0x14b, 0x14b, 0x14b, 0x14b, 0x142, 0x14a, 0x142
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.2byte 0x143, 0x143, 0x14d, 0x14d, 0x14c, 0x143, 0x143
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.2byte 0x150, 0x150, 0x145, 0x145, 0x145, 0x145, 0x145
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
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
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.4byte 0x10000, 0x0, 0x0, -0xcccc
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x500000, 0x0, 0x0
	.include "games/common/src/battle/battle_calc.s"
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.byte 32, 32, 32, 32, 32, 32, 32, 32
	.byte 53, 47, 41, 35, 29, 23, 17, 11
	.byte 113, 56, 31, 20, 14, 10, 7, 5
@ 技ごとの結果の状態
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "games/tbs/src/battle/actstat.table"
	.balign 4, 0
@ 技ごとの演出 (効果番号と段階, フラグ)
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "games/tbs/src/battle/actflags.table"
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.4byte 1, 2
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
	.4byte 0x10000, 0x0, 0x0, 0x0
@ 星の作業領域の位置
	.global BattleFx_StarWorkOffsets
BattleFx_StarWorkOffsets:
	.4byte 0, 1, 5, 14, 30, 46, 62
	.global BattleFx_StarSizes
BattleFx_StarSizes:
	.byte 1, 2, 3, 4, 4, 4, 4
	.balign 4
	.global BattleFx_RingFrameOffsets
BattleFx_RingFrameOffsets:
	.4byte 0x4e, 0x44e, 0x84e, 0xc4e
	.4byte 0, 10, 20, 0
	.global BattleFx_CharacterIdMapCopy
BattleFx_CharacterIdMapCopy:
	.2byte 18, 56
	.2byte 1, 1
	.2byte 19, 57
	.2byte 2, 2
	.2byte 3, 3
	.2byte 4, 4
	.2byte 5, 5
	.2byte 6, 6
	.2byte -1, -1
	.global BattleFx_ActorKindIdMapCopy
BattleFx_ActorKindIdMapCopy:
	.2byte 93, 0
	.2byte 90, 1
	.2byte 92, 2
	.2byte 91, 3
	.2byte 30, 4
	.2byte 32, 5
	.2byte 33, 6
	.2byte 43, 7
	.2byte 31, 8
	.2byte 53, 9
	.2byte 38, 10
	.2byte 42, 11
	.2byte 37, 12
	.2byte 54, 13
	.2byte 58, 14
	.2byte 69, 15
	.2byte 71, 16
	.2byte 56, 17
	.2byte 51, 18
	.2byte 49, 19
	.2byte 45, 20
	.2byte 40, 21
	.2byte 39, 22
	.2byte 41, 23
	.2byte 44, 24
	.2byte 50, 25
	.2byte 52, 26
	.2byte 57, 27
	.2byte 61, 28
	.2byte 47, 29
	.2byte 46, 30
	.2byte 66, 31
	.2byte 67, 32
	.2byte 121, 33
	.2byte 122, 34
	.2byte 124, 35
	.2byte 125, 36
	.2byte 118, 37
	.2byte 128, 38
	.2byte 119, 39
	.2byte 18, 40
	.2byte 19, 41
	.2byte 36, 42
	.2byte 48, 43
	.2byte 34, 44
	.2byte 55, 45
	.2byte 35, 46
	.2byte 65, 47
	.2byte 70, 48
	.2byte -1, -1
