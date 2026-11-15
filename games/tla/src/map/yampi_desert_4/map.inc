	.global Field_YampiDesert4
Field_YampiDesert4:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesert4, .LField_YampiDesert4_CELL_PICTURES
	map_block Field_YampiDesert4, .LField_YampiDesert4_SHAPES
	map_block Field_YampiDesert4, .LField_YampiDesert4_GRID
	map_block Field_YampiDesert4, .LField_YampiDesert4_SHAPE_GRID
	map_block Field_YampiDesert4, .LField_YampiDesert4_ANIMATION
	map_block Field_YampiDesert4, .LField_YampiDesert4_BLEND
	map_block Field_YampiDesert4, .LField_YampiDesert4_SCENE
.LField_YampiDesert4_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesert4_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_4/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesert4_GRID:
	.incbin "games/tla/src/map/yampi_desert_4/map.grid.lz"
	.balign 4, 0
.LField_YampiDesert4_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesert4_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesert4_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_YampiDesert4_SCENE:
	.incbin "games/tla/src/map/yampi_desert_4/scene.scene.lz"
	.balign 4, 0
