@ Summon_WhiteArrowsTiles: 47 4-bit tiles as the part list of its sprite runs (by shape), their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_WhiteArrowsTiles
Summon_WhiteArrowsTiles:
	.incbin "GRAPHICS/SUMMON/WHITE_ARROWS_1.gbapal"
	.incbin "GRAPHICS/SUMMON/WHITE_ARROWS.parts.lz"
