	.global Field_Test4
Field_Test4:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Test4, .LField_Test4_CELL_TILES
	map_block Field_Test4, .LField_Test4_SHAPES
	map_block Field_Test4, .LField_Test4_GRID
	map_block Field_Test4, .LField_Test4_SHAPE_GRID
	map_block Field_Test4
	map_block Field_Test4
	map_block Field_Test4, .LField_Test4_SCENE
.LField_Test4_CELL_TILES:
	.incbin "games/tla/src/map/empty_map/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Test4_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shapes.table.lz"
	.balign 4, 0
.LField_Test4_GRID:
	.incbin "games/tla/src/map/test_4/map.grid.lz"
	.balign 4, 0
.LField_Test4_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shape_grid.plane.lz"
	.balign 4, 0
.LField_Test4_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data632
Resource_Data632:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test4+0xb
	.global Resource_Data633
Resource_Data633:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x70, 0x84	@ PACKER: left in the buffer by Field_Test4+0x14e
	.global Resource_Data634
Resource_Data634:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data633+0x13f
	.global Resource_Data635
Resource_Data635:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data634+0x13f
	.global Resource_Data636
Resource_Data636:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data635+0x13f
