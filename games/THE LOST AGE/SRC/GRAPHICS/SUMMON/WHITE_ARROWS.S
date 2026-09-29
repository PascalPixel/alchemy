@ Summon_WhiteArrowsTiles: 47 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_WhiteArrowsTiles
Summon_WhiteArrowsTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_ARROWS.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_ARROWS.4bpp.lz"
