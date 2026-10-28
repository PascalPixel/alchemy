	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map085
Field_Map085:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map085, .LField_Map085_CELL_PICTURES
	map_block Field_Map085, .LField_Map085_SHAPES
	map_block Field_Map085, .LField_Map085_GRID
	map_block Field_Map085, .LField_Map085_SHAPE_GRID
	map_block Field_Map085, .LField_Map085_ANIMATION
	map_block Field_Map085, .LField_Map085_BLEND
	map_block Field_Map085, .LField_Map085_SCENE
.LField_Map085_CELL_PICTURES:
	.incbin "games/tla/src/map/map_085/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map085_SHAPES:
	.incbin "games/tla/src/map/map_085/shapes.table.lz"
	.balign 4, 0
.LField_Map085_GRID:
	.incbin "games/tla/src/map/map_085/grid.parts.lz"
	.balign 4, 0
.LField_Map085_SHAPE_GRID:
	.incbin "games/tla/src/map/map_085/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map085_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map085_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map085_SCENE:
	.incbin "games/tla/src/map/map_085/scene.parts.lz"
	.balign 4, 0
