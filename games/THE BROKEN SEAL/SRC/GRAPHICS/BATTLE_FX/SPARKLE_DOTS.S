	.section .rodata
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "GRAPHICS/BATTLE_FX/SPARKLE_DOTS.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_IceBlockSheet at +0x61 */
	.byte 0x63, 0x34, 0x77
