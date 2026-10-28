	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map047
Field_Map047:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map047, .LField_Map047_CELL_PICTURES
	map_block Field_Map047, .LField_Map047_SHAPES
	map_block Field_Map047, .LField_Map047_GRID
	map_block Field_Map047, .LField_Map047_SHAPE_GRID
	map_block Field_Map047, .LField_Map047_ANIMATION
	map_block Field_Map047, .LField_Map047_BLEND
	map_block Field_Map047, .LField_Map047_SCENE
.LField_Map047_CELL_PICTURES:
	.incbin "games/tla/src/map/map_047/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map047_SHAPES:
	.incbin "games/tla/src/map/map_047/shapes.table.lz"
	.balign 4, 0
.LField_Map047_GRID:
	.incbin "games/tla/src/map/map_047/grid.parts.lz"
	.balign 4, 0
.LField_Map047_SHAPE_GRID:
	.incbin "games/tla/src/map/map_047/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map047_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map047_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map047_SCENE:
	.incbin "games/tla/src/map/map_047/scene.parts.lz"
