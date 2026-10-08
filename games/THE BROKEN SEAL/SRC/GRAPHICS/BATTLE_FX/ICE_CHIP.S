	.section .rodata
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/ICE_CHIP.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_GlowOrbSheet at +0x63f */
	.byte 0x04
