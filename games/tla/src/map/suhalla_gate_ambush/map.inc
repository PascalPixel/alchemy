	.global Field_SuhallaGateAmbush
Field_SuhallaGateAmbush:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_CELL_TILES
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_SHAPES
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_GRID
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_SHAPE_GRID
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_ANIMATION
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_BLEND
	map_block Field_SuhallaGateAmbush, .LField_SuhallaGateAmbush_SCENE
.LField_SuhallaGateAmbush_CELL_TILES:
	.incbin "games/tla/src/map/suhalla_gate_ambush/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_ambush/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_GRID:
	.incbin "games/tla/src/map/suhalla_gate_ambush/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_ambush/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_ambush/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateAmbush_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
