	.global Field_AquaRockInside4
Field_AquaRockInside4:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_CELL_PICTURES
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_SHAPES
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_GRID
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_SHAPE_GRID
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_ANIMATION
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_BLEND
	map_block Field_AquaRockInside4, .LField_AquaRockInside4_SCENE
.LField_AquaRockInside4_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside4_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_4/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside4_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_4/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside4_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside4_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside4_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside4_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_4/scene.scene.lz"
