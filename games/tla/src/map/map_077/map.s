	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map077
Field_Map077:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map077, .LField_Map077_METATILES
	map_block Field_Map077, .LField_Map077_SHAPES
	map_block Field_Map077, .LField_Map077_GRID
	map_block Field_Map077, .LField_Map077_SHAPE_GRID
	map_block Field_Map077, .LField_Map077_ANIMATION
	map_block Field_Map077, .LField_Map077_BLEND
	map_block Field_Map077, .LField_Map077_SCENE
.LField_Map077_METATILES:
	.incbin "map/map_077/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map077_SHAPES:
	.incbin "map/map_077/shapes.table.lz"
	.balign 4, 0
.LField_Map077_GRID:
	.incbin "map/map_077/grid.parts.lz"
	.balign 4, 0
.LField_Map077_SHAPE_GRID:
	.incbin "map/map_077/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map077_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map077_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map077_SCENE:
	.incbin "map/map_077/scene.parts.lz"
