	.global Field_Test7
Field_Test7:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Test7, .LField_Test7_CELL_TILES
	map_block Field_Test7, .LField_Test7_SHAPES
	map_block Field_Test7, .LField_Test7_GRID
	map_block Field_Test7, .LField_Test7_SHAPE_GRID
	map_block Field_Test7
	map_block Field_Test7
	map_block Field_Test7, .LField_Test7_SCENE
.LField_Test7_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test7_SHAPES:
	.incbin "games/tla/src/map/daila_flooded/shapes.table.lz"
	.balign 4, 0
.LField_Test7_GRID:
	.incbin "games/tla/src/map/test_7/map.grid.lz"
	.balign 4, 0
.LField_Test7_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_flooded/shape_grid.plane.lz"
	.balign 4, 0
.LField_Test7_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data644
Resource_Data644:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test7+0xb
	.global Resource_Data645
Resource_Data645:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x10, 0x80	@ PACKER: left in the buffer by Field_Test7+0x14e
	.global Resource_Data646
Resource_Data646:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data645+0x13f
	.global Resource_Data647
Resource_Data647:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data646+0x13f
	.global Resource_Data648
Resource_Data648:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data647+0x13f
