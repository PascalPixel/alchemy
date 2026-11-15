	.global Field_AnkohlRuins3
Field_AnkohlRuins3:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_CELL_PICTURES
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_SHAPES
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_GRID
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_SHAPE_GRID
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_ANIMATION
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_BLEND
	map_block Field_AnkohlRuins3, .LField_AnkohlRuins3_SCENE
.LField_AnkohlRuins3_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins3_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_3/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins3_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_3/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins3_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins3_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_1/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins3_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuins3_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_3/scene.scene.lz"
	.balign 4, 0
