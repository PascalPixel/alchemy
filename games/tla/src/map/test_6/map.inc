	.global Field_Test6
Field_Test6:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Test6, .LField_Test6_CELL_TILES
	map_block Field_Test6, .LField_Test6_SHAPES
	map_block Field_Test6, .LField_Test6_GRID
	map_block Field_Test6, .LField_Test6_SHAPE_GRID
	map_block Field_Test6
	map_block Field_Test6
	map_block Field_Test6, .LField_Test6_SCENE
.LField_Test6_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test6_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/shapes.table.lz"
	.balign 4, 0
.LField_Test6_GRID:
	.incbin "games/tla/src/map/test_6/map.grid.lz"
	.balign 4, 0
.LField_Test6_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/shape_grid.plane.lz"
	.balign 4, 0
.LField_Test6_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data63E
Resource_Data63E:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test6+0xb
	.global Resource_Data63F
Resource_Data63F:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by Field_Test6+0x14e
	.global Resource_Data640
Resource_Data640:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data63F+0x13f
	.global Resource_Data641
Resource_Data641:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data640+0x13f
	.global Resource_Data642
Resource_Data642:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data641+0x13f
