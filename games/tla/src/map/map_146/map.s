	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map146
Field_Map146:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map146, .LField_Map146_CELL_PICTURES
	map_block Field_Map146, .LField_Map146_SHAPES
	map_block Field_Map146, .LField_Map146_GRID
	map_block Field_Map146, .LField_Map146_SHAPE_GRID
	map_block Field_Map146, .LField_Map146_ANIMATION
	map_block Field_Map146, .LField_Map146_BLEND
	map_block Field_Map146, .LField_Map146_SCENE
.LField_Map146_CELL_PICTURES:
	.incbin "games/tla/src/map/map_146/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map146_SHAPES:
	.incbin "games/tla/src/map/map_146/shapes.table.lz"
	.balign 4, 0
.LField_Map146_GRID:
	.incbin "games/tla/src/map/map_146/grid.parts.lz"
	.balign 4, 0
.LField_Map146_SHAPE_GRID:
	.incbin "games/tla/src/map/map_146/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map146_ANIMATION:
	.incbin "games/tla/src/map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map146_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map146_SCENE:
	.incbin "games/tla/src/map/map_146/scene.parts.lz"
