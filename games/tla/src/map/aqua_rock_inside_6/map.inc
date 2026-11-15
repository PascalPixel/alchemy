	.global Field_AquaRockInside6
Field_AquaRockInside6:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_CELL_PICTURES
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_SHAPES
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_GRID
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_SHAPE_GRID
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_ANIMATION
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_BLEND
	map_block Field_AquaRockInside6, .LField_AquaRockInside6_SCENE
.LField_AquaRockInside6_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside6_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_6/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside6_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_6/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside6_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside6_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_6/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside6_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside6_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_6/scene.scene.lz"
	.balign 4, 0
