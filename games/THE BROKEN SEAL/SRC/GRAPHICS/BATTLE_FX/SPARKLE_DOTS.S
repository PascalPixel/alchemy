	.section .rodata
	.global BattleFx_SparkleDots
BattleFx_SparkleDots:
	.incbin "GRAPHICS/BATTLE_FX/SPARKLE_DOTS.parts.lz"
	.byte 0x63, 0x34, 0x77	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x61
