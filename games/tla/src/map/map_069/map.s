	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map069
Field_Map069:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map069, .LField_Map069_METATILES
	map_block Field_Map069, .LField_Map069_SHAPES
	map_block Field_Map069, .LField_Map069_GRID
	map_block Field_Map069, .LField_Map069_SHAPE_GRID
	map_block Field_Map069, .LField_Map069_ANIMATION
	map_block Field_Map069, .LField_Map069_BLEND
	map_block Field_Map069, .LField_Map069_SCENE
.LField_Map069_METATILES:
	.incbin "games/tla/src/map/map_069/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map069_SHAPES:
	.incbin "games/common/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map069_GRID:
	.incbin "games/tla/src/map/map_069/grid.parts.lz"
	.balign 4, 0
.LField_Map069_SHAPE_GRID:
	.incbin "games/tla/src/map/map_069/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map069_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map069_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map069_SCENE:
	.incbin "games/tla/src/map/map_069/scene.parts.lz"
	.balign 4, 0
