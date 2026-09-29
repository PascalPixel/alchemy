@ Summon_WhiteWispsTiles: 143 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_WhiteWispsTiles
Summon_WhiteWispsTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_WISPS.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_WISPS.4bpp.lz"
