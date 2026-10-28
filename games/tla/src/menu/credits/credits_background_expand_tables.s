.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.global BattleEffect_ExpandCaseTable
BattleEffect_ExpandCaseTable:
	.4byte	BattleEffect_ExpandCase2
	.4byte	BattleEffect_ExpandCase4Plus
	.4byte	BattleEffect_ExpandCase8
	.4byte	BattleEffect_ExpandCase6Plus
	.4byte	BattleEffect_ExpandCase2
	.4byte	BattleEffect_ExpandCase4Minus
	.4byte	BattleEffect_ExpandCase10
	.4byte	BattleEffect_ExpandCase6Minus
