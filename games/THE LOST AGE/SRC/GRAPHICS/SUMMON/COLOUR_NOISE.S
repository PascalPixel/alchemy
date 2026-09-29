@ Summon_ColourNoiseTiles: 19 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_ColourNoiseTiles
Summon_ColourNoiseTiles:
	.incbin "GRAPHICS/SUMMON/COLOUR_NOISE.gbapal"
	.incbin "GRAPHICS/SUMMON/COLOUR_NOISE.4bpp.lz"
