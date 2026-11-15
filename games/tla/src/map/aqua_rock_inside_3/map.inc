	.global Field_AquaRockInside3
Field_AquaRockInside3:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_CELL_PICTURES
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_SHAPES
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_GRID
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_SHAPE_GRID
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_ANIMATION
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_BLEND
	map_block Field_AquaRockInside3, .LField_AquaRockInside3_SCENE
.LField_AquaRockInside3_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside3_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_3/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside3_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_3/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside3_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside3_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_3/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside3_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside3_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_3/scene.scene.lz"
	.balign 4, 0
