	.global Field_AnkohlRuins2
Field_AnkohlRuins2:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_CELL_PICTURES
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_SHAPES
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_GRID
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_SHAPE_GRID
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_ANIMATION
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_BLEND
	map_block Field_AnkohlRuins2, .LField_AnkohlRuins2_SCENE
.LField_AnkohlRuins2_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins2_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_2/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins2_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_2/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins2_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins2_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_2/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins2_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuins2_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_2/scene.scene.lz"
	.balign 4, 0
