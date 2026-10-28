	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map034
Field_Map034:
	map_header 0, 0, 128, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_CELL_PICTURES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_SHAPE_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034, .LField_Map034_BLEND
	map_block Field_Map034, .LField_Map034_SCENE
.LField_Map034_CELL_PICTURES:
	.incbin "games/tla/src/map/map_034/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tla/src/map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tla/src/map/map_034/grid.parts.lz"
	.balign 4, 0
.LField_Map034_SHAPE_GRID:
	.incbin "games/tla/src/map/map_034/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map034_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map034_SCENE:
	.incbin "games/tla/src/map/map_034/scene.parts.lz"
	.balign 4, 0
