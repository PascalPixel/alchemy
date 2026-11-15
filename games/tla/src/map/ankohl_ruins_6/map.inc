	.global Field_AnkohlRuins6
Field_AnkohlRuins6:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_CELL_PICTURES
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_SHAPES
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_GRID
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_SHAPE_GRID
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_ANIMATION
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_BLEND
	map_block Field_AnkohlRuins6, .LField_AnkohlRuins6_SCENE
.LField_AnkohlRuins6_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins6_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_6/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins6_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_6/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins6_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins6_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_2/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuins6_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_6/scene.scene.lz"
	.balign 4, 0
