	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map014
Field_Map014:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map014, .LField_Map014_METATILES
	map_block Field_Map014, .LField_Map014_SHAPES
	map_block Field_Map014, .LField_Map014_GRID
	map_block Field_Map014, .LField_Map014_SHAPE_GRID
	map_block Field_Map014, .LField_Map014_ANIMATION
	map_block Field_Map014, .LField_Map014_BLEND
	map_block Field_Map014, .LField_Map014_SCENE
.LField_Map014_METATILES:
	.incbin "map/map_014/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map014_SHAPES:
	.incbin "map/map_014/shapes.table.lz"
	.balign 4, 0
.LField_Map014_GRID:
	.incbin "map/map_014/grid.parts.lz"
	.balign 4, 0
.LField_Map014_SHAPE_GRID:
	.incbin "map/map_014/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map014_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map014_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map014_SCENE:
	.incbin "map/map_014/scene.parts.lz"
