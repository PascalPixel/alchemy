	.section .rodata
	.global BattleFx_IceChipSheet
BattleFx_IceChipSheet:
	.incbin "graphics/battle_fx/ice_chip_00.gbapal"
	.incbin "graphics/battle_fx/ice_chip.parts.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleFx_GlowOrbSheet+0x63f
