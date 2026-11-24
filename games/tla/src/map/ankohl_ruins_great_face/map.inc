	.global Field_AnkohlRuinsGreatFace
Field_AnkohlRuinsGreatFace:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_CELL_TILES
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_SHAPES
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_GRID
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_SHAPE_GRID
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_ANIMATION
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_BLEND
	map_block Field_AnkohlRuinsGreatFace, .LField_AnkohlRuinsGreatFace_SCENE
.LField_AnkohlRuinsGreatFace_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_great_face/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_great_face/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_great_face/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_great_face/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsGreatFace_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_great_face/scene.scene.lz"
	.balign 4, 0
