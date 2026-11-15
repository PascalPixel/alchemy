	.global Field_YampiDesert8
Field_YampiDesert8:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesert8, .LField_YampiDesert8_CELL_PICTURES
	map_block Field_YampiDesert8, .LField_YampiDesert8_SHAPES
	map_block Field_YampiDesert8, .LField_YampiDesert8_GRID
	map_block Field_YampiDesert8, .LField_YampiDesert8_SHAPE_GRID
	map_block Field_YampiDesert8, .LField_YampiDesert8_ANIMATION
	map_block Field_YampiDesert8, .LField_YampiDesert8_BLEND
	map_block Field_YampiDesert8, .LField_YampiDesert8_SCENE
.LField_YampiDesert8_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_8/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesert8_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_8/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesert8_GRID:
	.incbin "games/tla/src/map/yampi_desert_8/map.grid.lz"
	.balign 4, 0
.LField_YampiDesert8_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesert8_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_8/animation.script.lz"
	.balign 4, 0
.LField_YampiDesert8_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_YampiDesert8_SCENE:
	.incbin "games/tla/src/map/yampi_desert_8/scene.scene.lz"
