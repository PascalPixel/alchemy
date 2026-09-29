@ BattleFx_PaletteRampImage: 12 pictures (listed in SPARK_CELLS.TSV) with no palette of their
@ own, drawn with BattleFx_PinkBurstSheet's; joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_PaletteRampImage
BattleFx_PaletteRampImage:
	.incbin "GRAPHICS/BATTLE_FX/SPARK_CELLS.parts.lz"
