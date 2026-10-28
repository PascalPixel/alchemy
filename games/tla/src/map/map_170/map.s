	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map170
Field_Map170:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map170, .LField_Map170_CELL_PICTURES
	map_block Field_Map170, .LField_Map170_SHAPES
	map_block Field_Map170, .LField_Map170_GRID
	map_block Field_Map170, .LField_Map170_SHAPE_GRID
	map_block Field_Map170, .LField_Map170_ANIMATION
	map_block Field_Map170, .LField_Map170_BLEND
	map_block Field_Map170, .LField_Map170_SCENE
.LField_Map170_CELL_PICTURES:
	.incbin "games/tla/src/map/map_170/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map170_SHAPES:
	.incbin "games/tla/src/map/map_170/shapes.table.lz"
	.balign 4, 0
.LField_Map170_GRID:
	.incbin "games/tla/src/map/map_170/grid.parts.lz"
	.balign 4, 0
.LField_Map170_SHAPE_GRID:
	.incbin "games/tla/src/map/map_170/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map170_ANIMATION:
	.incbin "games/tla/src/map/map_170/animation.script.lz"
	.balign 4, 0
.LField_Map170_BLEND:
	.incbin "games/tla/src/map/map_168/blend.script.lz"
	.balign 4, 0
.LField_Map170_SCENE:
	.incbin "games/tla/src/map/map_170/scene.parts.lz"
