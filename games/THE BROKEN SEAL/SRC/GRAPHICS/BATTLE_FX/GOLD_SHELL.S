	.section .rodata
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GOLD_SHELL.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_IceBlockSheet at +0x1016 */
	.byte 0x15, 0xb2
