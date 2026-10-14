	.section .rodata
	.global gWorldMapPalettes
gWorldMapPalettes:
	.incbin "games/tbs/src/graphics/world_map/storypal.gbapal"
	.global gWorldMapPackedFrames
gWorldMapPackedFrames:
	.incbin "games/tbs/src/graphics/world_map/storymap.table.lz"
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.incbin "games/tbs/src/graphics/world_map/story.4bpp.lz"
	.balign 4, 0
