	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map252
Field_Map252:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map252, .LField_Map252_CELL_PICTURES
	map_block Field_Map252, .LField_Map252_SHAPES
	map_block Field_Map252, .LField_Map252_GRID
	map_block Field_Map252, .LField_Map252_SHAPE_GRID
	map_block Field_Map252, .LField_Map252_ANIMATION
	map_block Field_Map252, .LField_Map252_BLEND
	map_block Field_Map252, .LField_Map252_SCENE
.LField_Map252_CELL_PICTURES:
	.incbin "games/tla/src/map/map_252/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map252_SHAPES:
	.incbin "games/tla/src/map/map_252/shapes.table.lz"
	.balign 4, 0
.LField_Map252_GRID:
	.incbin "games/tla/src/map/map_252/grid.parts.lz"
	.balign 4, 0
.LField_Map252_SHAPE_GRID:
	.incbin "games/tla/src/map/map_252/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map252_ANIMATION:
	.incbin "games/tla/src/map/map_252/animation.script.lz"
	.balign 4, 0
.LField_Map252_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map252_SCENE:
	.incbin "games/tla/src/map/map_252/scene.parts.lz"
	.balign 4, 0
