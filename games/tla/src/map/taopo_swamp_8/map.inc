	.global Field_TaopoSwamp8
Field_TaopoSwamp8:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_CELL_PICTURES
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_SHAPES
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_GRID
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_SHAPE_GRID
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_ANIMATION
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_BLEND
	map_block Field_TaopoSwamp8, .LField_TaopoSwamp8_SCENE
.LField_TaopoSwamp8_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_8/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp8_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_8/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp8_GRID:
	.incbin "games/tla/src/map/taopo_swamp_8/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp8_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp8_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_7/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp8_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp8_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_8/scene.scene.lz"
	.balign 4, 0
