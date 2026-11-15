	.global Field_YampiDesert12
Field_YampiDesert12:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesert12, .LField_YampiDesert12_CELL_PICTURES
	map_block Field_YampiDesert12, .LField_YampiDesert12_SHAPES
	map_block Field_YampiDesert12, .LField_YampiDesert12_GRID
	map_block Field_YampiDesert12, .LField_YampiDesert12_SHAPE_GRID
	map_block Field_YampiDesert12, .LField_YampiDesert12_ANIMATION
	map_block Field_YampiDesert12, .LField_YampiDesert12_BLEND
	map_block Field_YampiDesert12, .LField_YampiDesert12_SCENE
.LField_YampiDesert12_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_12/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesert12_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_12/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesert12_GRID:
	.incbin "games/tla/src/map/yampi_desert_12/map.grid.lz"
	.balign 4, 0
.LField_YampiDesert12_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_12/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesert12_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_12/animation.script.lz"
	.balign 4, 0
.LField_YampiDesert12_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_YampiDesert12_SCENE:
	.incbin "games/tla/src/map/yampi_desert_12/scene.scene.lz"
