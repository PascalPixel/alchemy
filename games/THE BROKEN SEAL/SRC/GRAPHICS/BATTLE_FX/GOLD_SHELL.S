	.section .rodata
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x1016
