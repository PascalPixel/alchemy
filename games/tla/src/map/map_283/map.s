	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map283
Field_Map283:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map283, .LField_Map283_CELL_PICTURES
	map_block Field_Map283, .LField_Map283_SHAPES
	map_block Field_Map283, .LField_Map283_GRID
	map_block Field_Map283, .LField_Map283_SHAPE_GRID
	map_block Field_Map283, .LField_Map283_ANIMATION
	map_block Field_Map283, .LField_Map283_BLEND
	map_block Field_Map283, .LField_Map283_SCENE
.LField_Map283_CELL_PICTURES:
	.incbin "games/tla/src/map/map_283/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map283_SHAPES:
	.incbin "games/tla/src/map/map_283/shapes.table.lz"
	.balign 4, 0
.LField_Map283_GRID:
	.incbin "games/tla/src/map/map_283/grid.parts.lz"
	.balign 4, 0
.LField_Map283_SHAPE_GRID:
	.incbin "games/tla/src/map/map_283/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map283_ANIMATION:
	.incbin "games/tla/src/map/map_283/animation.script.lz"
	.balign 4, 0
.LField_Map283_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map283_SCENE:
	.incbin "games/tla/src/map/map_283/scene.parts.lz"
	.balign 4, 0
