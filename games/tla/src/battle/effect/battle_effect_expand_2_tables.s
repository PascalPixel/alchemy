.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.global BattleEffect_Expand2CaseTable
BattleEffect_Expand2CaseTable:
	.4byte	BattleEffect_Expand2Case2
	.4byte	BattleEffect_Expand2Case4Plus
	.4byte	BattleEffect_Expand2Case8
	.4byte	BattleEffect_Expand2Case6Plus
	.4byte	BattleEffect_Expand2Case2
	.4byte	BattleEffect_Expand2Case4Minus
	.4byte	BattleEffect_Expand2Case10
	.4byte	BattleEffect_Expand2Case6Minus
