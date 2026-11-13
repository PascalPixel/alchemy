	.global Field_Test6
Field_Test6:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Test6, .LField_Test6_CELL_PICTURES
	map_offset Field_Test6, .LField_Test6_SHAPES
	map_offset Field_Test6, .LField_Test6_GRID
	map_offset Field_Test6
	map_offset Field_Test6
	map_offset Field_Test6, .LField_Test6_MARKERS
.LField_Test6_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_001/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Test6_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Test6_GRID:
	.incbin "games/tbs/src/map/map_001/map.grid.lz"
	.balign 4, 0
.LField_Test6_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Test6Palette
Tileset_Test6Palette:
	.incbin "games/tbs/src/graphics/tileset/test_0/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test6+0xb
	.global Tileset_Test6TilesA
Tileset_Test6TilesA:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Test6+0x14e
	.global Tileset_Test6TilesB
Tileset_Test6TilesB:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test6TilesA+0x13f
	.global Tileset_Test6TilesC
Tileset_Test6TilesC:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test6TilesA+0x13f
	.global Tileset_Test6TilesD
Tileset_Test6TilesD:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test6TilesA+0x13f
	.balign 4, 0
