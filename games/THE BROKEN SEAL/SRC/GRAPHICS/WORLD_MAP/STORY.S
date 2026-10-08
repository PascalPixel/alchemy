	.section .rodata
	.global gWorldMapPalettes
gWorldMapPalettes:
	.incbin "GRAPHICS/WORLD_MAP/STORYPAL.gbapal"
	.global gWorldMapPackedFrames
gWorldMapPackedFrames:
	.incbin "GRAPHICS/WORLD_MAP/STORYMAP.table.lz"
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.incbin "GRAPHICS/WORLD_MAP/STORY.4bpp.lz"
	.balign 4, 0
