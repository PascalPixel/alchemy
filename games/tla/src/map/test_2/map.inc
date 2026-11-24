	.global Field_Test2
Field_Test2:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Test2, .LField_Test2_CELL_TILES
	map_block Field_Test2, .LField_Test2_SHAPES
	map_block Field_Test2, .LField_Test2_GRID
	map_block Field_Test2, .LField_Test2_SHAPE_GRID
	map_block Field_Test2
	map_block Field_Test2
	map_block Field_Test2, .LField_Test2_SCENE
.LField_Test2_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test2_SHAPES:
	.incbin "games/tla/src/map/contigo/shapes.table.lz"
	.balign 4, 0
.LField_Test2_GRID:
	.incbin "games/tla/src/map/test_2/map.grid.lz"
	.balign 4, 0
.LField_Test2_SHAPE_GRID:
	.incbin "games/tla/src/map/test_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Test2_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data626
Resource_Data626:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test2+0xb
	.global Resource_Data627
Resource_Data627:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0x63	@ PACKER: left in the buffer by Field_Test2+0x14e
	.global Resource_Data628
Resource_Data628:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data627+0x13f
	.global Resource_Data629
Resource_Data629:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data628+0x13f
	.global Resource_Data62A
Resource_Data62A:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data629+0x13f
