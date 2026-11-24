	.global Field_AnkohlRuinsFaceHalves
Field_AnkohlRuinsFaceHalves:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_CELL_TILES
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_SHAPES
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_GRID
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_SHAPE_GRID
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_ANIMATION
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_BLEND
	map_block Field_AnkohlRuinsFaceHalves, .LField_AnkohlRuinsFaceHalves_SCENE
.LField_AnkohlRuinsFaceHalves_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_face_halves/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_face_halves/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_face_halves/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_face_halves/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsFaceHalves_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_face_halves/scene.scene.lz"
	.balign 4, 0
