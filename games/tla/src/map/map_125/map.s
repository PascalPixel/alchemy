	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map125
Field_Map125:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map125, .LField_Map125_CELL_PICTURES
	map_block Field_Map125, .LField_Map125_SHAPES
	map_block Field_Map125, .LField_Map125_GRID
	map_block Field_Map125, .LField_Map125_SHAPE_GRID
	map_block Field_Map125, .LField_Map125_ANIMATION
	map_block Field_Map125, .LField_Map125_BLEND
	map_block Field_Map125, .LField_Map125_SCENE
.LField_Map125_CELL_PICTURES:
	.incbin "games/tla/src/map/map_125/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map125_SHAPES:
	.incbin "games/tla/src/map/map_125/shapes.table.lz"
	.balign 4, 0
.LField_Map125_GRID:
	.incbin "games/tla/src/map/map_125/grid.parts.lz"
	.balign 4, 0
.LField_Map125_SHAPE_GRID:
	.incbin "games/tla/src/map/map_125/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map125_ANIMATION:
	.incbin "games/tla/src/map/map_125/animation.script.lz"
	.balign 4, 0
.LField_Map125_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map125_SCENE:
	.incbin "games/tla/src/map/map_125/scene.parts.lz"
