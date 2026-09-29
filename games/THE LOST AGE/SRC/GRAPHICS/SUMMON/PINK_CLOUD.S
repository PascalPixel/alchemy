@ Summon_PinkCloudTiles: 135 4-bit tiles in one column, their 16-colour palette and
@ then the tiles, packed by the resource packer.
	.section .rodata
	.global Summon_PinkCloudTiles
Summon_PinkCloudTiles:
	.incbin "GRAPHICS/SUMMON/PINK_CLOUD.gbapal"
	.incbin "GRAPHICS/SUMMON/PINK_CLOUD.4bpp.lz"
