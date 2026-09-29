@ Summon_ForestCanopyTiles: their 16-colour palette, then 271 4-bit tiles drawn
@ as the sprites listed in FOREST_CANOPY.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_ForestCanopyTiles
Summon_ForestCanopyTiles:
	.incbin "GRAPHICS/SUMMON/FOREST_CANOPY_1.gbapal"
	.incbin "GRAPHICS/SUMMON/FOREST_CANOPY.parts.lz"
