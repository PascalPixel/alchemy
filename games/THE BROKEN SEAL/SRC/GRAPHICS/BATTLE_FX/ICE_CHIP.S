	.section .rodata
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_GlowOrbSheet+0x63f
