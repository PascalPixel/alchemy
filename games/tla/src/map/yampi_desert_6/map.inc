	.global Field_YampiDesert6
Field_YampiDesert6:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesert6, .LField_YampiDesert6_CELL_PICTURES
	map_block Field_YampiDesert6, .LField_YampiDesert6_SHAPES
	map_block Field_YampiDesert6, .LField_YampiDesert6_GRID
	map_block Field_YampiDesert6, .LField_YampiDesert6_SHAPE_GRID
	map_block Field_YampiDesert6, .LField_YampiDesert6_ANIMATION
	map_block Field_YampiDesert6, .LField_YampiDesert6_BLEND
	map_block Field_YampiDesert6, .LField_YampiDesert6_SCENE
.LField_YampiDesert6_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesert6_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesert6_GRID:
	.incbin "games/tla/src/map/yampi_desert_6/map.grid.lz"
	.balign 4, 0
.LField_YampiDesert6_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesert6_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesert6_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_YampiDesert6_SCENE:
	.incbin "games/tla/src/map/yampi_desert_6/scene.scene.lz"
	.balign 4, 0
