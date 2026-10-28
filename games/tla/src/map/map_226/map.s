	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map226
Field_Map226:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map226, .LField_Map226_CELL_PICTURES
	map_block Field_Map226, .LField_Map226_SHAPES
	map_block Field_Map226, .LField_Map226_GRID
	map_block Field_Map226, .LField_Map226_SHAPE_GRID
	map_block Field_Map226, .LField_Map226_ANIMATION
	map_block Field_Map226, .LField_Map226_BLEND
	map_block Field_Map226, .LField_Map226_SCENE
.LField_Map226_CELL_PICTURES:
	.incbin "games/tla/src/map/map_226/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map226_SHAPES:
	.incbin "games/tla/src/map/map_226/shapes.table.lz"
	.balign 4, 0
.LField_Map226_GRID:
	.incbin "games/tla/src/map/map_226/grid.parts.lz"
	.balign 4, 0
.LField_Map226_SHAPE_GRID:
	.incbin "games/tla/src/map/map_226/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map226_ANIMATION:
	.incbin "games/tla/src/map/map_226/animation.script.lz"
	.balign 4, 0
.LField_Map226_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map226_SCENE:
	.incbin "games/tla/src/map/map_226/scene.parts.lz"
	.balign 4, 0
