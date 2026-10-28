	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map124
Field_Map124:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map124, .LField_Map124_CELL_PICTURES
	map_block Field_Map124, .LField_Map124_SHAPES
	map_block Field_Map124, .LField_Map124_GRID
	map_block Field_Map124, .LField_Map124_SHAPE_GRID
	map_block Field_Map124, .LField_Map124_ANIMATION
	map_block Field_Map124, .LField_Map124_BLEND
	map_block Field_Map124, .LField_Map124_SCENE
.LField_Map124_CELL_PICTURES:
	.incbin "games/tla/src/map/map_124/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map124_SHAPES:
	.incbin "games/tla/src/map/map_124/shapes.table.lz"
	.balign 4, 0
.LField_Map124_GRID:
	.incbin "games/tla/src/map/map_124/grid.parts.lz"
	.balign 4, 0
.LField_Map124_SHAPE_GRID:
	.incbin "games/tla/src/map/map_124/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map124_ANIMATION:
	.incbin "games/tla/src/map/map_124/animation.script.lz"
	.balign 4, 0
.LField_Map124_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map124_SCENE:
	.incbin "games/tla/src/map/map_124/scene.parts.lz"
