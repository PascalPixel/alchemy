	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Test1
Field_Test1:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Test1, .LField_Test1_CELL_PICTURES
	map_offset Field_Test1, .LField_Test1_SHAPES
	map_offset Field_Test1, .LField_Test1_GRID
	map_offset Field_Test1
	map_offset Field_Test1
	map_offset Field_Test1, .LField_Test1_MARKERS
.LField_Test1_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_001/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Test1_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Test1_GRID:
	.incbin "games/tbs/src/map/map_001/map.grid.lz"
	.balign 4, 0
.LField_Test1_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Test1Palette
Tileset_Test1Palette:
	.incbin "games/tbs/src/graphics/tileset/test_0/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test1+0xb
	.global Tileset_Test1TilesA
Tileset_Test1TilesA:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Test1+0x14e
	.global Tileset_Test1TilesB
Tileset_Test1TilesB:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test1TilesA+0x13f
	.global Tileset_Test1TilesC
Tileset_Test1TilesC:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test1TilesA+0x13f
	.global Tileset_Test1TilesD
Tileset_Test1TilesD:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test1TilesA+0x13f
