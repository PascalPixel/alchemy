	.section .rodata
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "GRAPHICS/BATTLE_FX/TARGET_BURST.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_SlashSheet at +0x83e */
	.byte 0x2f, 0x00
