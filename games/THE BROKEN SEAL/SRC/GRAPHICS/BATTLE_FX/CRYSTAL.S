	.section .rodata
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleFx_TornadoSheet+0x104d
