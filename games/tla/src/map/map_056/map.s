	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map056
Field_Map056:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map056, .LField_Map056_CELL_PICTURES
	map_block Field_Map056, .LField_Map056_SHAPES
	map_block Field_Map056, .LField_Map056_GRID
	map_block Field_Map056, .LField_Map056_SHAPE_GRID
	map_block Field_Map056, .LField_Map056_ANIMATION
	map_block Field_Map056, .LField_Map056_BLEND
	map_block Field_Map056, .LField_Map056_SCENE
.LField_Map056_CELL_PICTURES:
	.incbin "games/tla/src/map/map_056/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map056_SHAPES:
	.incbin "games/tla/src/map/map_056/shapes.table.lz"
	.balign 4, 0
.LField_Map056_GRID:
	.incbin "games/tla/src/map/map_056/grid.parts.lz"
	.balign 4, 0
.LField_Map056_SHAPE_GRID:
	.incbin "games/tla/src/map/map_056/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map056_ANIMATION:
	.incbin "games/tla/src/map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map056_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map056_SCENE:
	.incbin "games/tla/src/map/map_056/scene.parts.lz"
	.balign 4, 0
