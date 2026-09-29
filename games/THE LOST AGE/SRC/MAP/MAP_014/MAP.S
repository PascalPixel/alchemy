@ Field_Map014: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
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
	.incbin "MAP/MAP_014/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map014_SHAPES:
	.incbin "MAP/MAP_014/SHAPES.table.lz"
	.balign 4, 0
.LField_Map014_GRID:
	.incbin "MAP/MAP_014/GRID.parts.lz"
	.balign 4, 0
.LField_Map014_SHAPE_GRID:
	.incbin "MAP/MAP_014/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map014_ANIMATION:
	.incbin "MAP/MAP_014/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map014_BLEND:
	.incbin "MAP/MAP_014/BLEND.script.lz"
	.balign 4, 0
.LField_Map014_SCENE:
	.incbin "MAP/MAP_014/SCENE.parts.lz"
