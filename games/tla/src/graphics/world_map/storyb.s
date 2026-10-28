	.section .rodata
	.global gWorldMapPalettesB
gWorldMapPalettesB:
	.incbin "games/tla/src/graphics/world_map/storypb.gbapal"
	.global gWorldMapPackedPicturesB
gWorldMapPackedPicturesB:
	.incbin "games/tla/src/graphics/world_map/storymb.table.lz"
	.global gWorldMapPackedTilesB
gWorldMapPackedTilesB:
	.incbin "games/tla/src/graphics/world_map/storyb.4bpp.lz"
