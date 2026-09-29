@ Summon_ForestCanopyTiles: 271 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_ForestCanopyTiles
Summon_ForestCanopyTiles:
	.incbin "GRAPHICS/SUMMON/FOREST_CANOPY.gbapal"
	.incbin "GRAPHICS/SUMMON/FOREST_CANOPY.4bpp.lz"
