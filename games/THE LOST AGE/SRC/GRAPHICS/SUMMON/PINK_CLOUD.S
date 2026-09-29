@ Summon_PinkCloudTiles: their 16-colour palette, then 135 4-bit tiles drawn
@ as the sprites listed in PINK_CLOUD.TSV, joined and packed by the resource packer.
	.section .rodata
	.global Summon_PinkCloudTiles
Summon_PinkCloudTiles:
	.incbin "GRAPHICS/SUMMON/PINK_CLOUD_1.gbapal"
	.incbin "GRAPHICS/SUMMON/PINK_CLOUD.parts.lz"
