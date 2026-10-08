	.section .rodata
	.global BattleFx_CrystalSheet
BattleFx_CrystalSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRYSTAL.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_TornadoSheet at +0x104d */
	.byte 0x13, 0x42, 0x03
