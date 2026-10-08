	.section .rodata
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "graphics/battle_fx/gold_shell_00.gbapal"
	.incbin "graphics/battle_fx/gold_shell.parts.lz"
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x1016
