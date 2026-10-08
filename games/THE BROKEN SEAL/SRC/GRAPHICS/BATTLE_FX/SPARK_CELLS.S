	.section .rodata
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "GRAPHICS/BATTLE_FX/SPARK_CELLS.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_DaggerSheet at +0x196 */
	.byte 0x01, 0x0d
