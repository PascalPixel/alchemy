	.section .rodata
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "GRAPHICS/BATTLE_FX/TARGET_BURST.bitmap.lz"
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleFx_SlashSheet+0x83e
