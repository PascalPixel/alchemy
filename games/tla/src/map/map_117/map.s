	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map117
Field_Map117:
	map_header 0, 0, 128, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map117, .LField_Map117_METATILES
	map_block Field_Map117, .LField_Map117_SHAPES
	map_block Field_Map117, .LField_Map117_GRID
	map_block Field_Map117, .LField_Map117_SHAPE_GRID
	map_block Field_Map117, .LField_Map117_ANIMATION
	map_block Field_Map117, .LField_Map117_BLEND
	map_block Field_Map117, .LField_Map117_SCENE
.LField_Map117_METATILES:
	.incbin "map/map_117/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map117_SHAPES:
	.incbin "map/map_117/shapes.table.lz"
	.balign 4, 0
.LField_Map117_GRID:
	.incbin "map/map_117/grid.parts.lz"
	.balign 4, 0
.LField_Map117_SHAPE_GRID:
	.incbin "map/map_117/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map117_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map117_BLEND:
	.incbin "map/map_113/blend.script.lz"
	.balign 4, 0
.LField_Map117_SCENE:
	.incbin "map/map_117/scene.parts.lz"
