@ Field_Map077: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
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
	.incbin "MAP/MAP_077/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map077_SHAPES:
	.incbin "MAP/MAP_077/SHAPES.table.lz"
	.balign 4, 0
.LField_Map077_GRID:
	.incbin "MAP/MAP_077/GRID.parts.lz"
	.balign 4, 0
.LField_Map077_SHAPE_GRID:
	.incbin "MAP/MAP_077/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map077_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map077_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map077_SCENE:
	.incbin "MAP/MAP_077/SCENE.parts.lz"
