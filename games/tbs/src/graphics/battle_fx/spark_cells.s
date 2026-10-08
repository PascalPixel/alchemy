	.section .rodata
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "graphics/battle_fx/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleFx_DaggerSheet+0x196
