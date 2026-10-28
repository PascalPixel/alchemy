	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map199
Field_Map199:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map199, .LField_Map199_CELL_PICTURES
	map_block Field_Map199, .LField_Map199_SHAPES
	map_block Field_Map199, .LField_Map199_GRID
	map_block Field_Map199, .LField_Map199_SHAPE_GRID
	map_block Field_Map199, .LField_Map199_ANIMATION
	map_block Field_Map199
	map_block Field_Map199, .LField_Map199_SCENE
.LField_Map199_CELL_PICTURES:
	.incbin "games/tla/src/map/map_199/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map199_SHAPES:
	.incbin "games/tla/src/map/map_199/shapes.table.lz"
	.balign 4, 0
.LField_Map199_GRID:
	.incbin "games/tla/src/map/map_199/grid.parts.lz"
	.balign 4, 0
.LField_Map199_SHAPE_GRID:
	.incbin "games/tla/src/map/map_199/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map199_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map199_SCENE:
	.incbin "games/tla/src/map/map_199/scene.parts.lz"
	.balign 4, 0
