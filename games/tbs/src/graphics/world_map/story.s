	.section .rodata
	.global gWorldMapPalettes
gWorldMapPalettes:
	.incbin "graphics/world_map/storypal.gbapal"
	.global gWorldMapPackedFrames
gWorldMapPackedFrames:
	.incbin "graphics/world_map/storymap.table.lz"
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.incbin "graphics/world_map/story.4bpp.lz"
	.balign 4, 0
