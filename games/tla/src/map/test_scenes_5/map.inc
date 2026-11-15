	.global Field_TestScenes5
Field_TestScenes5:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TestScenes5, .LField_TestScenes5_CELL_PICTURES
	map_block Field_TestScenes5, .LField_TestScenes5_SHAPES
	map_block Field_TestScenes5, .LField_TestScenes5_GRID
	map_block Field_TestScenes5, .LField_TestScenes5_SHAPE_GRID
	map_block Field_TestScenes5
	map_block Field_TestScenes5
	map_block Field_TestScenes5, .LField_TestScenes5_SCENE
.LField_TestScenes5_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_TestScenes5_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shapes.table.lz"
	.balign 4, 0
.LField_TestScenes5_GRID:
	.incbin "games/tla/src/map/test_scenes_5/map.grid.lz"
	.balign 4, 0
.LField_TestScenes5_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shape_grid.plane.lz"
	.balign 4, 0
.LField_TestScenes5_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data632
Resource_Data632:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_TestScenes5+0xb
	.global Resource_Data633
Resource_Data633:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x70, 0x84	@ PACKER: left in the buffer by Field_TestScenes5+0x14e
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
