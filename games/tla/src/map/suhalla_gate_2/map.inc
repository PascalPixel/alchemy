	.global Field_SuhallaGate2
Field_SuhallaGate2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_CELL_PICTURES
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_SHAPES
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_GRID
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_SHAPE_GRID
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_ANIMATION
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_BLEND
	map_block Field_SuhallaGate2, .LField_SuhallaGate2_SCENE
.LField_SuhallaGate2_CELL_PICTURES:
	.incbin "games/tla/src/map/suhalla_gate_2/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGate2_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_2/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGate2_GRID:
	.incbin "games/tla/src/map/suhalla_gate_2/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGate2_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGate2_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_2/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGate2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGate2_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
