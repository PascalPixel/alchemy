	.section .rodata
	.global gWorldMapPalettes
gWorldMapPalettes:
	.incbin "games/tbs/src/graphics/world_map/lighthouse_transition_palette.gbapal"
	.global gWorldMapPackedPictures
gWorldMapPackedPictures:
	.incbin "games/tbs/src/graphics/world_map/lighthouse_transition_map.tilemap.lz"
	.global gWorldMapPackedTiles
gWorldMapPackedTiles:
	.incbin "games/tbs/src/graphics/world_map/lighthouse_transition.4bpp.lz"
	.balign 4, 0
