	.global Field_TaopoSwamp3
Field_TaopoSwamp3:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_CELL_PICTURES
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_SHAPES
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_GRID
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_SHAPE_GRID
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_ANIMATION
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_BLEND
	map_block Field_TaopoSwamp3, .LField_TaopoSwamp3_SCENE
.LField_TaopoSwamp3_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp3_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_3/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp3_GRID:
	.incbin "games/tla/src/map/taopo_swamp_3/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp3_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp3_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_3/scene.scene.lz"
	.balign 4, 0
