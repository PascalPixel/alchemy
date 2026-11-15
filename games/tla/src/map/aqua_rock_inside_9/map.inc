	.global Field_AquaRockInside9
Field_AquaRockInside9:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_CELL_PICTURES
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_SHAPES
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_GRID
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_SHAPE_GRID
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_ANIMATION
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_BLEND
	map_block Field_AquaRockInside9, .LField_AquaRockInside9_SCENE
.LField_AquaRockInside9_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_9/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside9_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_9/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside9_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_9/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside9_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_9/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside9_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_9/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside9_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside9_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_9/scene.scene.lz"
