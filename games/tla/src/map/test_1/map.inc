	.global Field_Test1
Field_Test1:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Test1, .LField_Test1_CELL_TILES
	map_block Field_Test1, .LField_Test1_SHAPES
	map_block Field_Test1, .LField_Test1_GRID
	map_block Field_Test1, .LField_Test1_SHAPE_GRID
	map_block Field_Test1
	map_block Field_Test1
	map_block Field_Test1, .LField_Test1_SCENE
.LField_Test1_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test1_SHAPES:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/shapes.table.lz"
	.balign 4, 0
.LField_Test1_GRID:
	.incbin "games/tla/src/map/test_1/map.grid.lz"
	.balign 4, 0
.LField_Test1_SHAPE_GRID:
	.incbin "games/tla/src/map/test_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Test1_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data620
Resource_Data620:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test1+0xb
	.global Resource_Data621
Resource_Data621:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x80, 0x06	@ PACKER: left in the buffer by Field_Test1+0x14e
	.global Resource_Data622
Resource_Data622:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data621+0x13f
	.global Resource_Data623
Resource_Data623:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data622+0x13f
	.global Resource_Data624
Resource_Data624:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data623+0x13f
