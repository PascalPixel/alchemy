	.global Field_Test4
Field_Test4:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Test4, .LField_Test4_CELL_TILES
	map_offset Field_Test4, .LField_Test4_SHAPES
	map_offset Field_Test4, .LField_Test4_GRID
	map_offset Field_Test4
	map_offset Field_Test4
	map_offset Field_Test4, .LField_Test4_MARKERS
.LField_Test4_CELL_TILES:
	.incbin "games/tbs/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test4_SHAPES:
	.incbin "games/tbs/src/map/empty_map/shapes.table.lz"
	.balign 4, 0
.LField_Test4_GRID:
	.incbin "games/tbs/src/map/empty_map/map.grid.lz"
	.balign 4, 0
.LField_Test4_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Test4Palette
Tileset_Test4Palette:
	.incbin "games/tbs/src/graphics/tileset/test_0/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test4+0xb
	.global Tileset_Test4TilesA
Tileset_Test4TilesA:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Test4+0x14e
	.global Tileset_Test4TilesB
Tileset_Test4TilesB:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test4TilesA+0x13f
	.global Tileset_Test4TilesC
Tileset_Test4TilesC:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test4TilesA+0x13f
	.global Tileset_Test4TilesD
Tileset_Test4TilesD:
	.incbin "games/tbs/src/graphics/tileset/test_0/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Test4TilesA+0x13f
	.balign 4, 0
