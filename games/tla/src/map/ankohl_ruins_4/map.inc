	.global Field_AnkohlRuins4
Field_AnkohlRuins4:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_CELL_PICTURES
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_SHAPES
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_GRID
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_SHAPE_GRID
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_ANIMATION
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_BLEND
	map_block Field_AnkohlRuins4, .LField_AnkohlRuins4_SCENE
.LField_AnkohlRuins4_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_4/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins4_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_4/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins4_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_4/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins4_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins4_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_2/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuins4_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_4/scene.scene.lz"
	.balign 4, 0
