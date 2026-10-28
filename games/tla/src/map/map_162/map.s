	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map162
Field_Map162:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map162, .LField_Map162_CELL_PICTURES
	map_block Field_Map162, .LField_Map162_SHAPES
	map_block Field_Map162, .LField_Map162_GRID
	map_block Field_Map162, .LField_Map162_SHAPE_GRID
	map_block Field_Map162, .LField_Map162_ANIMATION
	map_block Field_Map162
	map_block Field_Map162, .LField_Map162_SCENE
.LField_Map162_CELL_PICTURES:
	.incbin "games/tla/src/map/map_162/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map162_SHAPES:
	.incbin "games/tla/src/map/map_162/shapes.table.lz"
	.balign 4, 0
.LField_Map162_GRID:
	.incbin "games/tla/src/map/map_162/grid.parts.lz"
	.balign 4, 0
.LField_Map162_SHAPE_GRID:
	.incbin "games/tla/src/map/map_162/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map162_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map162_SCENE:
	.incbin "games/tla/src/map/map_162/scene.parts.lz"
	.balign 4, 0
