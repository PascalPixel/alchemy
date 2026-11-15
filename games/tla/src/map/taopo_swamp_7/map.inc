	.global Field_TaopoSwamp7
Field_TaopoSwamp7:
	map_header 0, 0, 90, 100, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_CELL_PICTURES
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_SHAPES
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_GRID
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_SHAPE_GRID
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_ANIMATION
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_BLEND
	map_block Field_TaopoSwamp7, .LField_TaopoSwamp7_SCENE
.LField_TaopoSwamp7_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_7/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp7_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_7/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp7_GRID:
	.incbin "games/tla/src/map/taopo_swamp_7/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp7_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp7_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_7/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp7_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp7_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_7/scene.scene.lz"
