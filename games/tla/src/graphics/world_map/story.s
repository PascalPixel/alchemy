	.section .rodata
	.global gWorldMapPalettes
gWorldMapPalettes:
	.incbin "games/tla/src/graphics/world_map/storypal.gbapal"
	.global gWorldMapPackedPictures
gWorldMapPackedPictures:
	.incbin "games/tla/src/graphics/world_map/storymap.table.lz"
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.incbin "games/tla/src/graphics/world_map/story.4bpp.lz"
