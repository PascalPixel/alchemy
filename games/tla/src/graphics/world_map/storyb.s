	.section .rodata
	.global gWorldMapPalettesB
gWorldMapPalettesB:
	.incbin "graphics/world_map/storypb.gbapal"
	.global gWorldMapPackedFramesB
gWorldMapPackedFramesB:
	.incbin "graphics/world_map/storymb.table.lz"
	.global gWorldMapPackedTilesB
gWorldMapPackedTilesB:
	.incbin "graphics/world_map/storyb.4bpp.lz"
