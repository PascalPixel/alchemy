	.global Field_TestScenes4
Field_TestScenes4:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TestScenes4, .LField_TestScenes4_CELL_PICTURES
	map_block Field_TestScenes4, .LField_TestScenes4_SHAPES
	map_block Field_TestScenes4, .LField_TestScenes4_GRID
	map_block Field_TestScenes4, .LField_TestScenes4_SHAPE_GRID
	map_block Field_TestScenes4
	map_block Field_TestScenes4
	map_block Field_TestScenes4, .LField_TestScenes4_SCENE
.LField_TestScenes4_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_TestScenes4_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_fog/shapes.table.lz"
	.balign 4, 0
.LField_TestScenes4_GRID:
	.incbin "games/tla/src/map/test_scenes_4/map.grid.lz"
	.balign 4, 0
.LField_TestScenes4_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_fog/shape_grid.plane.lz"
	.balign 4, 0
.LField_TestScenes4_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Resource_Data62C
Resource_Data62C:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_TestScenes4+0xb
	.global Resource_Data62D
Resource_Data62D:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x00, 0x01	@ PACKER: left in the buffer by Field_TestScenes4+0x14e
	.global Resource_Data62E
Resource_Data62E:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data62D+0x13f
	.global Resource_Data62F
Resource_Data62F:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data62E+0x13f
	.global Resource_Data630
Resource_Data630:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data62F+0x13f
