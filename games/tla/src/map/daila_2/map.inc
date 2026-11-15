	.global Field_Daila2
Field_Daila2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Daila2, .LField_Daila2_CELL_PICTURES
	map_block Field_Daila2, .LField_Daila2_SHAPES
	map_block Field_Daila2, .LField_Daila2_GRID
	map_block Field_Daila2, .LField_Daila2_SHAPE_GRID
	map_block Field_Daila2, .LField_Daila2_ANIMATION
	map_block Field_Daila2
	map_block Field_Daila2, .LField_Daila2_SCENE
.LField_Daila2_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Daila2_SHAPES:
	.incbin "games/tla/src/map/daila_2/shapes.table.lz"
	.balign 4, 0
.LField_Daila2_GRID:
	.incbin "games/tla/src/map/daila_2/map.grid.lz"
	.balign 4, 0
.LField_Daila2_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Daila2_ANIMATION:
	.incbin "games/tla/src/map/daila_1/animation.script.lz"
	.balign 4, 0
.LField_Daila2_SCENE:
	.incbin "games/tla/src/map/daila_2/scene.scene.lz"
