	.section .rodata
	.balign 4
@ デバッグ用の仲間 (owner -1 で区切り)
	.global Debug_PartyPresets
Debug_PartyPresets:
	.incbin "games/tbs/src/battle/presets.table"
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.4byte 16
@ 資源番号 | 枠 << 9, 0xffff で終わり
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.2byte 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008
	.2byte 0x0009, 0x000a, 0x000b, 0x020f, 0x0210, 0x0211, 0x0212, 0x0213
	.2byte 0x0214, 0x0215, 0x0216, 0x0217, 0x0218, 0x0219, 0x021a, 0x021b
	.2byte 0x041f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426
	.2byte 0x062b, 0x062c, 0x062d, 0x062e, 0x062f, 0x0630, 0x0631, 0x0632
	.2byte 0x0837, 0x0838, 0x0839, 0x0a3a, 0x083b, 0x083c, 0x0a3d, 0x083e
	.2byte 0x083f, 0x0840, 0x0841, 0xffff
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.4byte 1 << 16, 1 << 16, 1 << 16, 1 << 16, 8 << 16, 1 << 16, 1 << 16, 1 << 16
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.4byte 4 << 16, 4 << 16, 4 << 16, 4 << 16, 20 << 16, 4 << 16, 4 << 16, 4 << 16
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.4byte 3 << 16, 6 << 16, 7 << 16, 10 << 16, 5 << 16, 2 << 16, 3 << 16, 0x58000
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.4byte 140, 140, 150, 200, 200, 150, 100, 100
	.4byte 0, 20, 27
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "games/common/src/battle/presentation/tilevar.4bpp"
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "games/common/src/battle/presentation/curtain.4bpp"
	.include "games/common/src/battle/presentation/highlight_ramp.s"
	.4byte 1, 27
@ 敵の編成 380 個
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "games/tbs/src/battle/formatn.table"
	.global BattleFormation_LevelMatchedEnemies
BattleFormation_LevelMatchedEnemies:
	.2byte 0x75, 0x7a, 0x7d, 0x85, 0x87, 0x89, 0x9e, 0x9f, 0xa0, 0xa1
	.2byte 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab
@ 戦闘の絵 172 個
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "games/tbs/src/battle/sprites.table"
