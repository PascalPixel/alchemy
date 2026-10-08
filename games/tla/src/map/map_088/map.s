	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map088
Field_Map088:
	map_header 0, 0, 100, 128, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map088, .LField_Map088_METATILES
	map_block Field_Map088, .LField_Map088_SHAPES
	map_block Field_Map088, .LField_Map088_GRID
	map_block Field_Map088, .LField_Map088_SHAPE_GRID
	map_block Field_Map088, .LField_Map088_ANIMATION
	map_block Field_Map088, .LField_Map088_BLEND
	map_block Field_Map088, .LField_Map088_SCENE
.LField_Map088_METATILES:
	.incbin "map/map_088/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map088_SHAPES:
	.incbin "map/map_088/shapes.table.lz"
	.balign 4, 0
.LField_Map088_GRID:
	.incbin "map/map_088/grid.parts.lz"
	.balign 4, 0
.LField_Map088_SHAPE_GRID:
	.incbin "map/map_088/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map088_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map088_BLEND:
	.incbin "map/map_088/blend.script.lz"
	.balign 4, 0
.LField_Map088_SCENE:
	.incbin "map/map_088/scene.parts.lz"
