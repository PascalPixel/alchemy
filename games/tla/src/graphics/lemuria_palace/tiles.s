	.section .rodata
	.global LemuriaPalace_FirstMapTiles
LemuriaPalace_FirstMapTiles:
	.incbin "games/tla/src/graphics/lemuria_palace/tiles_a.8bpp.lz"
	.global LemuriaPalace_SecondMapTiles
LemuriaPalace_SecondMapTiles:
	.incbin "games/tla/src/graphics/lemuria_palace/tiles_b.8bpp.lz"
	.global LemuriaPalaceMapFadeLevels
LemuriaPalaceMapFadeLevels:
	.byte 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 2, 1, 0
