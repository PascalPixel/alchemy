	.section .rodata
	.global gLemuriaPalace_FirstMapTiles
gLemuriaPalace_FirstMapTiles:
	.incbin "games/tla/src/graphics/lemuria_palace/tiles_a.8bpp.lz"
	.global gLemuriaPalace_SecondMapTiles
gLemuriaPalace_SecondMapTiles:
	.incbin "games/tla/src/graphics/lemuria_palace/tiles_b.8bpp.lz"
	.global gLemuriaPalace_MapFadeLevels
gLemuriaPalace_MapFadeLevels:
	.byte 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 2, 1, 0
