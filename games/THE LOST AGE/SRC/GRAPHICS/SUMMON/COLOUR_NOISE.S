@ Summon_ColourNoiseTiles: 19 4-bit tiles as the part list of its sprite runs (by shape), their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_ColourNoiseTiles
Summon_ColourNoiseTiles:
	.incbin "GRAPHICS/SUMMON/COLOUR_NOISE_1.gbapal"
	.incbin "GRAPHICS/SUMMON/COLOUR_NOISE.parts.lz"
