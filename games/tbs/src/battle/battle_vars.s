	.section .rodata
	.global gBattleObject_EmptyScript
gBattleObject_EmptyScript:
	.4byte 16
@ Resource id | slot << 9, terminated by 0xffff.
	.global gResource_SlotAssignments
gResource_SlotAssignments:
	.2byte 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008
	.2byte 0x0009, 0x000a, 0x000b, 0x020f, 0x0210, 0x0211, 0x0212, 0x0213
	.2byte 0x0214, 0x0215, 0x0216, 0x0217, 0x0218, 0x0219, 0x021a, 0x021b
	.2byte 0x041f, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426
	.2byte 0x062b, 0x062c, 0x062d, 0x062e, 0x062f, 0x0630, 0x0631, 0x0632
	.2byte 0x0837, 0x0838, 0x0839, 0x0a3a, 0x083b, 0x083c, 0x0a3d, 0x083e
	.2byte 0x083f, 0x0840, 0x0841, 0xffff
	.global gBattleKnockback_Acceleration
gBattleKnockback_Acceleration:
	.4byte 1 << 16, 1 << 16, 1 << 16, 1 << 16, 8 << 16, 1 << 16, 1 << 16, 1 << 16
	.global gBattleKnockback_SpeedLimit
gBattleKnockback_SpeedLimit:
	.4byte 4 << 16, 4 << 16, 4 << 16, 4 << 16, 20 << 16, 4 << 16, 4 << 16, 4 << 16
	.global gBattleKnockback_VelocityY
gBattleKnockback_VelocityY:
	.4byte 3 << 16, 6 << 16, 7 << 16, 10 << 16, 5 << 16, 2 << 16, 3 << 16, 0x58000
	.global gBattleKnockback_DistancePercent
gBattleKnockback_DistancePercent:
	.4byte 140, 140, 150, 200, 200, 150, 100, 100
	.4byte 0, 20, 27
	.global gBattleTransition_CurtainVariants
gBattleTransition_CurtainVariants:
	.incbin "games/tbs/src/battle/presentation/tilevar.4bpp"
	.global gBattleTransition_CurtainTiles
gBattleTransition_CurtainTiles:
	.incbin "games/tbs/src/battle/presentation/curtain.4bpp"
	.include "games/tbs/src/battle/presentation/highlight_ramp.s"
	.4byte 1, 27
