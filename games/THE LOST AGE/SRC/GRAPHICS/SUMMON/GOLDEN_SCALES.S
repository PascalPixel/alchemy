@ Summon_GoldenScalesTiles: 95 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_GoldenScalesTiles
Summon_GoldenScalesTiles:
	.incbin "GRAPHICS/SUMMON/GOLDEN_SCALES.gbapal"
	.incbin "GRAPHICS/SUMMON/GOLDEN_SCALES.4bpp.lz"
