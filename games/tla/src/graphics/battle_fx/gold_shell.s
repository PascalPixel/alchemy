	.section .rodata
	.global BattleEffect_GoldShellSheet
BattleEffect_GoldShellSheet:
	.incbin "games/tla/src/graphics/battle_fx/gold_shell_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleEffect_IceBlockSheet+0x1016
