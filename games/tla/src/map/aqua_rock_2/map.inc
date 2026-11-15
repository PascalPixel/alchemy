	.global Field_AquaRock2
Field_AquaRock2:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRock2, .LField_AquaRock2_CELL_PICTURES
	map_block Field_AquaRock2, .LField_AquaRock2_SHAPES
	map_block Field_AquaRock2, .LField_AquaRock2_GRID
	map_block Field_AquaRock2, .LField_AquaRock2_SHAPE_GRID
	map_block Field_AquaRock2, .LField_AquaRock2_ANIMATION
	map_block Field_AquaRock2, .LField_AquaRock2_BLEND
	map_block Field_AquaRock2, .LField_AquaRock2_SCENE
.LField_AquaRock2_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRock2_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_2/shapes.table.lz"
	.balign 4, 0
.LField_AquaRock2_GRID:
	.incbin "games/tla/src/map/aqua_rock_2/map.grid.lz"
	.balign 4, 0
.LField_AquaRock2_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRock2_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_2/animation.script.lz"
	.balign 4, 0
.LField_AquaRock2_BLEND:
	.incbin "games/tla/src/map/aqua_rock_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRock2_SCENE:
	.incbin "games/tla/src/map/aqua_rock_2/scene.scene.lz"
