@ Field_Map258: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map258
Field_Map258:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map258, .LField_Map258_METATILES
	map_block Field_Map258, .LField_Map258_SHAPES
	map_block Field_Map258, .LField_Map258_GRID
	map_block Field_Map258, .LField_Map258_SHAPE_GRID
	map_block Field_Map258, .LField_Map258_ANIMATION
	map_block Field_Map258
	map_block Field_Map258, .LField_Map258_SCENE
.LField_Map258_METATILES:
	.incbin "MAP/MAP_258/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map258_SHAPES:
	.incbin "MAP/MAP_258/SHAPES.table.lz"
	.balign 4, 0
.LField_Map258_GRID:
	.incbin "MAP/MAP_258/GRID.parts.lz"
	.balign 4, 0
.LField_Map258_SHAPE_GRID:
	.incbin "MAP/MAP_258/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map258_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map258_SCENE:
	.incbin "MAP/MAP_258/SCENE.parts.lz"
